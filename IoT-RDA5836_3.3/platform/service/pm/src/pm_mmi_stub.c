/* 
 * Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <csw.h>
#include <errorcode.h>
#include <sul.h>
#include <pm.h>
#include <ts.h>

#include "csw_debug.h"
#include "dbg.h"
#include "csw_csp.h"

#include "pmd_m.h"
#if (CSW_EXTENDED_API_PMD_USB == 1) && (CHIP_HAS_USB == 1)
#include "uctls_m.h"
#endif

#include "hal_ana_gpadc.h"
#include "hal_lps.h"
#include "pmd_m.h"

#include "cos.h" // for HVY_TIMER_IN
#include "sxr_tim.h"
#include "sxr_tls.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"
#include "assert.h"

#include "tgt_m.h"

#ifdef VMIC_POWER_ON_WORKAROUND
extern BOOL hal_AudVMicValid(VOID);
#endif

#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
VOID tgt_ForbidFactorySettingsFlushing(BOOL forbidden);
#define PM_DISABLE_FACT_FLUSH_LEVEL 3700
#define PM_DISABLE_FACT_FLUSH_BACKOFF 50
#endif

// Can be set to 0 if battery is 1500mA or above
#ifndef PM_INCREASE_BATTERY_DETECT
#define PM_INCREASE_BATTERY_DETECT 1
#endif

// PM_VMIC_POWER_POLICY
// when VMIC is On earpiece detection is active
// 0 : VMIC Always on
// 1 : VMIC off during low power
// 2 : VMIC off every other paging during low power
#ifndef PM_VMIC_POWER_POLICY
#define PM_VMIC_POWER_POLICY 1
#endif

#if defined(VMIC_POWER_ON_WORKAROUND) && (PM_VMIC_POWER_POLICY != 1)
#error "PM_VMIC_POWER_POLICY should be 1 when VMIC_POWER_ON_WORKAROUND is enabled."
#endif

// Whether to debouce earpiece GPADC detection
#ifndef PM_EARPIECE_DET_DEBOUNCE
#define PM_EARPIECE_DET_DEBOUNCE 0
#endif

#define PM_FILTRATE_BATTERY_STEP    800

// Number of steps for the MEAN calculation
#define PM_AVERAGE_BATTERY_NB       4 // 32
// change threshold needed for stable value update
// 10 mV is 1% with current battery technology
#define PM_HYSTERESIS_BATTERY_STEP  50
// Normal hysteresis will not update for the last % close to 0% and 100%
// the percent value where the hysteresis should be ignored
// it will be ignored in [0% .. n%] and [100-n% .. 100%]
// (note: it should be larger that PM_HYSTERESIS_BATTERY_STEP but in % not mV)
#define PM_HYSTERESIS_BATTERY_PERCENT_EDGE 7

#define PM_BATTERY_MEDIAN_FILTER_FACTOR 4

// The index type of battery level array is UINT8
#if (PM_AVERAGE_BATTERY_NB > 255)
#error "Invalid PM_AVERAGE_BATTERY_NB"
#endif

// The median average should contain at minimum 1/3 samples
// (or filter at maximum 2/3 samples)
#if (PM_BATTERY_MEDIAN_FILTER_FACTOR < 3)
#error "Invalid PM_BATTERY_MEDIAN_FILTER_FACTOR"
#endif

#define PM_SCREEN_ANTI_FLICKER_BACKOFF 100

/// delay between battery monitoring during IDLE
#ifdef PMD_TWO_BATTARY
#define PM_BAT_MONITOR_IDLE_TIME         40 * HAL_TICK1S
#else
#if (PM_INCREASE_BATTERY_DETECT)
#define PM_BAT_MONITOR_IDLE_TIME         15 * HAL_TICK1S
#else
#define PM_BAT_MONITOR_IDLE_TIME         1 * 60 * HAL_TICK1S
#endif
#endif
/// delay between battery monitoring during CALL
#if (PM_INCREASE_BATTERY_DETECT)
#define PM_BAT_MONITOR_CALL_TIME         1 * HAL_TICK1S
#else
#define PM_BAT_MONITOR_CALL_TIME         5 * HAL_TICK1S
#endif
/// delay between battery monitoring during IDLE with low battery
#ifdef PMD_TWO_BATTARY
#define PM_BAT_MONITOR_IDLE_LOWBAT_TIME   8 * HAL_TICK1S
#else
    #if (PM_INCREASE_BATTERY_DETECT)
    #define PM_BAT_MONITOR_IDLE_LOWBAT_TIME   1 * HAL_TICK1S
    #else
    #define PM_BAT_MONITOR_IDLE_LOWBAT_TIME   15 * HAL_TICK1S
#endif
#endif
#if (PM_INCREASE_BATTERY_DETECT)
#define PM_BAT_MONITOR_IDLE_CRITICAL_TIME  1 * HAL_TICK1S
#else
#define PM_BAT_MONITOR_IDLE_CRITICAL_TIME  5 * HAL_TICK1S

#endif

/// delay in polling mode between earpiece detection during IDLE but active
#define PM_EARPIECE_DET_POLLING_IDLE_TIME  1 * HAL_TICK1S
/// delay in interrupt mode between earpiece detection during IDLE but active
#define PM_EARPIECE_DET_INT_IDLE_TIME      3 * HAL_TICK1S
/// delay between earpiece detection during CALL
#define PM_EARPIECE_DET_CALL_TIME          3 * HAL_TICK1S / 10

#define PM_EARPIECE_DET_MAX_TIME           0xFFFFFFFF

#if (PM_EARPIECE_DET_DEBOUNCE)
// A quick manual earpiece key press lasts for about 120 ms
#define PM_EARPIECE_DET_DEBOUNCE_TIME      6 * HAL_TICK1S / 100
#define PM_EARPIECE_DET_DEBOUNCE_VALUE     200

#if ((PM_EARPIECE_DET_DEBOUNCE_TIME) > (PM_EARPIECE_DET_CALL_TIME))
#error "Earpiece GPADC debounce time should NOT be greater than detection period during a call!"
#endif
#endif // PM_EARPIECE_DET_DEBOUNCE


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// PM_BATTERY_STABLE_T
// -----------------------------------------------------------------------------
/// This structure groups the variables needed for battery stable value
/// using averaging (to flatten GPADC variation) and histeresys (to avoid
/// blinking bar)
// =============================================================================
typedef struct
{
    /// sum the battery measures
    UINT32 sum;
    /// calculated mean level
    UINT16 avg;
    /// histeresys stable value
    UINT16 stable;
    /// last instant value
    UINT16 instant;
    /// array of measures to maintaint the mean
    UINT16 levels[PM_AVERAGE_BATTERY_NB];
    /// current index in the array
    UINT8 idx;
    /// number of measures (for init)
    UINT8 nb;
    /// array of sorted measure indices
    UINT8 sortedIdx[PM_AVERAGE_BATTERY_NB];
    /// calculated percent from the stable value
    UINT8 precent;

} PM_BATTERY_STABLE_T;

// Number of debounce for the ear detect
#define PM_AVERAGE_EAR_NB       10

// =============================================================================
// PM_EARDETECT_T
// -----------------------------------------------------------------------------
/// This structure groups the variables needed for detect the earphone
// =============================================================================

typedef struct
{
    UINT8 mode;
    BOOL earon;
    BOOL stateStable;
    BOOL gpadcMonStarted;
    BOOL gpadcMonDisabled;
#if (PM_EARPIECE_DET_DEBOUNCE)
    BOOL debouncing;
    UINT16 prevValue;
#endif // PM_EARPIECE_DET_DEBOUNCE
} PM_EAR_DETECT_T;

// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================

PM_EAR_DETECT_T g_pmEarDetect = 
{
    .mode = 0,
    .earon = FALSE,
    .stateStable = FALSE,
    .gpadcMonStarted = FALSE,
    .gpadcMonDisabled = FALSE,
#if (PM_EARPIECE_DET_DEBOUNCE)
    .debouncing = FALSE,
    .prevValue = 0,
#endif // PM_EARPIECE_DET_DEBOUNCE
};




// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================

// =============================================================================
// g_pmBatteryStable 
// -----------------------------------------------------------------------------
/// This structure groups the variables needed fot battery averaging
// =============================================================================
PRIVATE PM_BATTERY_STABLE_T g_pmBatteryStable =
{
    .idx     = 0,
    .nb      = 0,
    .avg     = 0,
    .instant = 0,
    .levels  = { 0 },//[0 ... PM_AVERAGE_BATTERY_NB-1] = 
    .precent = 0
};

#ifdef PMD_TWO_BATTARY
PRIVATE PM_BATTERY_STABLE_T g_pmBatteryStable_sub =
{
    .idx     = 0,
    .nb      = 0,
    .avg     = 0,
    .instant = 0,
    .levels  = { 0 },//[0 ... PM_AVERAGE_BATTERY_NB-1] = 
    .precent = 0
};


#endif

PRIVATE UINT32 Time_To_MMI_Evt      = 0;

/// battery monitoring perriod
PRIVATE UINT32 g_pmBatMonTime     = PM_BAT_MONITOR_IDLE_TIME;
PRIVATE UINT32 g_pmBatMonLastDate = 0;
/// earpiece detect perriod
PRIVATE UINT32 g_pmEarPieceDetTime     = PM_EARPIECE_DET_MAX_TIME;
PRIVATE UINT32 g_pmEarPieceDetLastDate = 0;
PRIVATE UINT8 g_pmChargeFlag           = 0;
PRIVATE UINT8 g_pmRemindTime           = 0;

#if ((PM_VMIC_POWER_POLICY == 1) || (PM_VMIC_POWER_POLICY == 2))
PRIVATE BOOL g_pmVMICActive = FALSE;
#endif

// added private funtion to do the job with pmd
PRIVATE UINT8 g_pm_poweronvolt = 0;
PRIVATE UINT8 g_pm_powerdowervolt = 0;
#ifdef PMD_TWO_BATTARY
PRIVATE UINT8 g_pm_sub_poweronvolt = 0;
#endif

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE) // 8808 or 8809
PRIVATE BOOL g_pmScreenAntiFlickerEn = FALSE;
#endif

PMD_CHARGER_STATUS_T g_pm_oldStatus = PMD_CHARGER_UNKNOWN;
/// @param status to convert
/// @return Bcs value
PRIVATE UINT8 pm_PmdChargerStatus2Bcs(PMD_CHARGER_STATUS_T status)
{
    UINT8 bcs = PM_CHARGER_CONNECTED;

    // always go to the step 'connected' before any charge step
    if ( (g_pm_oldStatus <= PMD_CHARGER_UNPLUGGED)
        && (status > PMD_CHARGER_UNPLUGGED) )
    {
        g_pm_oldStatus = PMD_CHARGER_PLUGGED;
        return bcs;
    }
    g_pm_oldStatus = status;

    switch (status)
    {
        case PMD_CHARGER_UNKNOWN:
        case PMD_CHARGER_UNPLUGGED:
            bcs = PM_CHARGER_DISCONNECTED;
            break;
        case PMD_CHARGER_PLUGGED:
            bcs = PM_CHARGER_CONNECTED;
            break;
        case PMD_CHARGER_PRECHARGE:
        case PMD_CHARGER_FAST_CHARGE:
        case PMD_CHARGER_PULSED_CHARGE:
        case PMD_CHARGER_PULSED_CHARGE_WAIT:
            bcs = PM_CHARGER_CHRGING;
            break;
        case PMD_CHARGER_FULL_CHARGE:
            bcs = PM_CHARGER_FINISHED;
            break;
        case PMD_CHARGER_TEMPERATURE_ERROR:
            bcs = PM_CHARGER_ERROR_TEMPERATURE;
            break;
        case PMD_CHARGER_VOLTAGE_ERROR:
            bcs = PM_CHARGER_ERROR_VOLTAGE;
            break;
        case PMD_CHARGER_ERROR:
        case PMD_CHARGER_TIMED_OUT:
        default:
            bcs = PM_CHARGER_ERROR_UNKNOWN;
            break;
    }
    return bcs;
}

/// @param batLevel battery level input to convert
/// @param pBcl pointer on Bcl to update
/// @param pBatState pointer on BatState to load
/// @return FALSE when invalid parameter
PRIVATE BOOL pm_BatteryLevel2State(UINT16 batLevel, UINT8* pBcl, UINT16* pBatState)
{


    if ((pBcl == NULL) || (pBatState == NULL))
    {
        return FALSE;
    }
    if (batLevel == PMD_BAD_VALUE)
    {
        *pBcl      = 0;
        *pBatState = PM_BATTERY_STATE_UNKNOWN;
    }
    else
    {
        if (*pBcl == 0)
        {
            *pBcl      = 1;
            *pBatState = PM_BATTERY_STATE_SHUTDOWN;
        }
        else
        {
            /// @todo: not hardcode levels
            if (*pBcl <= g_pm_powerdowervolt) // if (*pBcl < 20)
            {
                *pBatState = PM_BATTERY_STATE_SHUTDOWN;
            }
            else if (*pBcl <= (g_pm_powerdowervolt +5) )//if (*pBcl < 25)
            {
                *pBatState = PM_BATTERY_STATE_CRITICAL;
            }
            else if (*pBcl <= (g_pm_powerdowervolt +10)) //if (*pBcl < 30)
            {
                *pBatState = PM_BATTERY_STATE_LOW;
            }
            else
            {
                *pBatState = PM_BATTERY_STATE_NORMAL;
            }
        }
      //  *pBatState = PM_BATTERY_STATE_NORMAL; // only use the narmal status.
    }
    
    return TRUE;
}

#ifdef PMD_TWO_BATTARY
extern PUBLIC BOOL pmd_GetMainStatus(VOID);
#define PM_MAIN_DETECTCOUNT 30
UINT16 g_main_battery_detector = PM_AVERAGE_BATTERY_NB;
BOOL g_main_battery_flag = FALSE,g_start_quick_detect_flag = FALSE;
#define MOINTER_TEMP_FOR_PLUGIN_MIAN_BATTERY 220 MS_WAITING

PRIVATE INT pm_GetStableBatteryLevel_sub(UINT8 * pPercent)
{
    UINT8 bcl = 0,i=0;
    UINT8 avgPercent;
    INT16 batLevelTemp1 = 0,batLevelTemp2 = 0;
    UINT16 batLevel = 0;
    if(pmd_GetCurrentBattery() == PMD_BATTERY_MAIN)
    {
        pmd_SetCurrentBattery(PMD_BATTERY_SUB);
        batLevel = pmd_GetBatteryLevel(&bcl);
        pmd_SetCurrentBattery(PMD_BATTERY_MAIN);
    }
    else
    {
        batLevel = pmd_GetBatteryLevel(&bcl);
    }
 
    if (batLevel == PMD_BAD_VALUE)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm: sub PMD_BAD_VALUE");
        return PMD_BAD_VALUE;
    }
    if (batLevel < 2500)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm: sub PMD_BAD_VALUE too small");
        return PMD_BAD_VALUE;
    }

 
    if (g_pmBatteryStable_sub.nb == PM_AVERAGE_BATTERY_NB)
    {
        if (g_pmBatteryStable_sub.avg > batLevel)
        {
            if ((g_pmBatteryStable_sub.avg - batLevel) > PM_FILTRATE_BATTERY_STEP )
            {
                CSW_TRACE(CFW_PM_TS_ID, "pm: sub too small %dmv",batLevel);
                return PMD_BAD_VALUE;
            }
        }

    }
    g_pmBatteryStable_sub.sum -= g_pmBatteryStable_sub.levels[g_pmBatteryStable_sub.idx];
    g_pmBatteryStable_sub.sum += batLevel;
    g_pmBatteryStable_sub.levels[g_pmBatteryStable_sub.idx] = batLevel;
    if (g_pmBatteryStable_sub.nb < PM_AVERAGE_BATTERY_NB)
    {
        g_pmBatteryStable_sub.nb++;
    }
    g_pmBatteryStable_sub.idx++;
    if (g_pmBatteryStable_sub.idx >= PM_AVERAGE_BATTERY_NB)
    {
        g_pmBatteryStable_sub.idx = 0;
    }
    //.....
    if (g_pmBatteryStable_sub.nb > 10)
    {
        batLevelTemp1 = g_pmBatteryStable_sub.levels[0];
        for (i=0;i<g_pmBatteryStable_sub.nb;i++)
        {
          if (batLevelTemp1 > g_pmBatteryStable_sub.levels[i])
          {
            batLevelTemp1 = g_pmBatteryStable_sub.levels[i];
          }
        }
        batLevelTemp2 = g_pmBatteryStable_sub.levels[0];
        for (i=0;i<g_pmBatteryStable_sub.nb;i++)
        {
          if (batLevelTemp2 < g_pmBatteryStable_sub.levels[i])
          {
            batLevelTemp2 = g_pmBatteryStable_sub.levels[i];
          }
        }
        batLevelTemp1 += batLevelTemp2;
        g_pmBatteryStable_sub.avg = (g_pmBatteryStable_sub.sum - batLevelTemp1 ) / (g_pmBatteryStable_sub.nb - 2);

    }
    else
    {
        g_pmBatteryStable_sub.avg = g_pmBatteryStable_sub.sum / g_pmBatteryStable_sub.nb;
    }
    // original code enter the current level an other time if avg < 3570
    // if the volt is low then about 3600,the volt will decline very quickly when calling
    // we implement it in a different way in pm_doMonitoring() by measuring more often when instant battery level is low.
    g_pmBatteryStable_sub.instant = batLevel;
    g_pmBatteryStable_sub.avg = g_pmBatteryStable_sub.sum / g_pmBatteryStable_sub.nb;
    // hysteresis leveling:
    if ((((INT32)g_pmBatteryStable_sub.stable - (INT32)g_pmBatteryStable_sub.avg)
            >= PM_HYSTERESIS_BATTERY_STEP)
        || (((INT32)g_pmBatteryStable_sub.avg - (INT32)g_pmBatteryStable_sub.stable)
            >= PM_HYSTERESIS_BATTERY_STEP))
    {
        g_pmBatteryStable_sub.stable = g_pmBatteryStable_sub.avg;
    }
    // forcing hysteresis for the last few % (near 0% and 100%) 
    avgPercent = pmd_BatteryLevelToPercent(g_pmBatteryStable_sub.avg);
    if ((avgPercent <= PM_HYSTERESIS_BATTERY_PERCENT_EDGE + 13)
        || (avgPercent >= (100-PM_HYSTERESIS_BATTERY_PERCENT_EDGE)))
    {
        g_pmBatteryStable_sub.stable = g_pmBatteryStable_sub.avg;
    }


    // calculate %, print and return value...
    g_pmBatteryStable_sub.precent = pmd_BatteryLevelToPercent(g_pmBatteryStable_sub.stable);
    if (pPercent != NULL)
    {
        *pPercent = g_pmBatteryStable_sub.precent;
    }
    CSW_TRACE(CFW_PM_TS_ID, "pm: sub instant: %dmV, %d%%; mean: %dmV, %d%%; stable: %dmV, %d%%",
                              batLevel, bcl, g_pmBatteryStable_sub.avg, avgPercent, g_pmBatteryStable_sub.stable, g_pmBatteryStable_sub.precent);
    return g_pmBatteryStable_sub.stable;
}

#endif

PRIVATE INT pm_GetStableBatteryLevel(UINT8 * pPercent)
{
    INT32 i, rank, newRank;
    UINT8 bcl = 0;
    UINT8 avgPercent;
    UINT16 batLevel = PMD_BAD_VALUE;

#ifdef PMD_TWO_BATTARY
    if (pmd_GetMainStatus() == FALSE)
    {
      return PMD_BAD_VALUE;
    }

    if(pmd_GetCurrentBattery() == PMD_BATTERY_SUB)
    {
        pmd_SetCurrentBattery(PMD_BATTERY_MAIN);

        batLevel = pmd_GetBatteryLevel(&bcl);
        pmd_SetCurrentBattery(PMD_BATTERY_SUB);
    }
    else
    {
        batLevel = pmd_GetBatteryLevel(&bcl);
    }

    if (pmd_GetMainStatus() == FALSE)
    {
      CSW_TRACE(CFW_PM_TS_ID, "pm:  main battery off");   
      return PMD_BAD_VALUE;
    }
    
    if (batLevel > 5500)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm:  PMD_BAD_VALUE too larger");
        return PMD_BAD_VALUE;
    }
#else
    batLevel = pmd_GetBatteryLevel(&bcl);
#endif

    if (batLevel == PMD_BAD_VALUE)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm:  PMD_BAD_VALUE");
        return PMD_BAD_VALUE;
    }
    if (batLevel < 2500)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm:  PMD_BAD_VALUE too small");
        return PMD_BAD_VALUE;
    }
 
    if (g_pmBatteryStable.nb == PM_AVERAGE_BATTERY_NB)
    {
        if (g_pmBatteryStable.avg > batLevel)
        {
            if ((g_pmBatteryStable.avg - batLevel) > PM_FILTRATE_BATTERY_STEP )
            {
                CSW_TRACE(CFW_PM_TS_ID, "pm:  too small %dmv",batLevel);
                #ifndef PMD_TWO_BATTARY
                return PMD_BAD_VALUE;
                #endif
            }
        }
    }

    g_pmBatteryStable.levels[g_pmBatteryStable.idx] = batLevel;
    if (g_pmBatteryStable.nb < PM_AVERAGE_BATTERY_NB)
    {
        g_pmBatteryStable.nb++;
    }

    for (i=0; i<PM_AVERAGE_BATTERY_NB; i++)
    {
        if (g_pmBatteryStable.sortedIdx[i] == g_pmBatteryStable.idx)
        {
            break;
        }
    }
    if (i == PM_AVERAGE_BATTERY_NB)
    {
        __assert("Corrupted g_pmBatteryStable.sortedIdx");
    }
    rank = i;

    if (rank > 0 && 
        g_pmBatteryStable.levels[g_pmBatteryStable.sortedIdx[rank-1]] < batLevel)
    {
        if (rank == 1)
        {
            newRank = 0;
        }
        else
        {
            for (i=rank-2; i>=0; i--)
            {
                if (g_pmBatteryStable.levels[g_pmBatteryStable.sortedIdx[i]] >= batLevel)
                {
                    break;
                }
            }
            newRank = i + 1;
        }
        for (i=rank; i>newRank; i--)
        {
            g_pmBatteryStable.sortedIdx[i] = g_pmBatteryStable.sortedIdx[i-1];
        }
        g_pmBatteryStable.sortedIdx[newRank] = g_pmBatteryStable.idx;
    }
    else if (rank < (g_pmBatteryStable.nb - 1) &&
        batLevel < g_pmBatteryStable.levels[g_pmBatteryStable.sortedIdx[rank+1]])
    {
        if (rank == g_pmBatteryStable.nb - 2)
        {
            newRank = g_pmBatteryStable.nb - 1;
        }
        else
        {
            for (i=rank+2; i<g_pmBatteryStable.nb; i++)
            {
                if (batLevel >= g_pmBatteryStable.levels[g_pmBatteryStable.sortedIdx[i]])
                {
                    break;
                }
            }
            newRank = i - 1;
        }
        for (i=rank; i<newRank; i++)
        {
            g_pmBatteryStable.sortedIdx[i] = g_pmBatteryStable.sortedIdx[i+1];
        }
        g_pmBatteryStable.sortedIdx[newRank] = g_pmBatteryStable.idx;
    }

    g_pmBatteryStable.sum = 0;
    if (g_pmBatteryStable.nb == PM_AVERAGE_BATTERY_NB)
    {
        for (i=PM_AVERAGE_BATTERY_NB/PM_BATTERY_MEDIAN_FILTER_FACTOR; 
             i<PM_AVERAGE_BATTERY_NB-PM_AVERAGE_BATTERY_NB/PM_BATTERY_MEDIAN_FILTER_FACTOR;
             i++)
        {
            g_pmBatteryStable.sum += g_pmBatteryStable.levels[
                                        g_pmBatteryStable.sortedIdx[i] ];
        }
        g_pmBatteryStable.avg = g_pmBatteryStable.sum / 
                (PM_AVERAGE_BATTERY_NB-2*(PM_AVERAGE_BATTERY_NB/PM_BATTERY_MEDIAN_FILTER_FACTOR));
    }
    else
    {
        for (i=g_pmBatteryStable.nb/PM_BATTERY_MEDIAN_FILTER_FACTOR; 
             i<g_pmBatteryStable.nb-g_pmBatteryStable.nb/PM_BATTERY_MEDIAN_FILTER_FACTOR;
             i++)
        {
            g_pmBatteryStable.sum += g_pmBatteryStable.levels[
                                        g_pmBatteryStable.sortedIdx[i] ];
        }
        g_pmBatteryStable.avg = g_pmBatteryStable.sum / 
                (g_pmBatteryStable.nb-2*(g_pmBatteryStable.nb/PM_BATTERY_MEDIAN_FILTER_FACTOR));
    }

    g_pmBatteryStable.idx++;
    if (g_pmBatteryStable.idx >= PM_AVERAGE_BATTERY_NB)
    {
        g_pmBatteryStable.idx = 0;
    }
    g_pmBatteryStable.instant = batLevel;

    // hysteresis leveling:
    if ((((INT32)g_pmBatteryStable.stable - (INT32)g_pmBatteryStable.avg)
            >= PM_HYSTERESIS_BATTERY_STEP)
        || (((INT32)g_pmBatteryStable.avg - (INT32)g_pmBatteryStable.stable)
            >= PM_HYSTERESIS_BATTERY_STEP))
    {
        g_pmBatteryStable.stable = g_pmBatteryStable.avg;
    }
    // forcing hysteresis for the last few % (near 0% and 100%) 
    avgPercent = pmd_BatteryLevelToPercent(g_pmBatteryStable.avg);
    if ((avgPercent <= PM_HYSTERESIS_BATTERY_PERCENT_EDGE + 13)
        || (avgPercent >= (100-PM_HYSTERESIS_BATTERY_PERCENT_EDGE)))
    {
        g_pmBatteryStable.stable = g_pmBatteryStable.avg;
    }

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE) // 8808 or 8809
    if (g_pmBatteryStable.stable <= pmd_GetScreenAntiFlickerVolt())
    {
        if (!g_pmScreenAntiFlickerEn)
        {
            g_pmScreenAntiFlickerEn = TRUE;
            pmd_EnableScreenAntiFlicker(TRUE);
        }
    }
    else if (g_pmBatteryStable.stable > pmd_GetScreenAntiFlickerVolt() +
                                        PM_SCREEN_ANTI_FLICKER_BACKOFF)
    {
        if (g_pmScreenAntiFlickerEn)
        {
            g_pmScreenAntiFlickerEn = FALSE;
            pmd_EnableScreenAntiFlicker(FALSE);
        }
    }
#endif // 8808 or 8809

#ifndef FACT_SETTING_FLUSH_ALWAYS
#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    if (g_pmBatteryStable.stable <= PM_DISABLE_FACT_FLUSH_LEVEL)
    {
        tgt_ForbidFactorySettingsFlushing(TRUE);
    }
    else if (g_pmBatteryStable.stable > PM_DISABLE_FACT_FLUSH_LEVEL +
                                        PM_DISABLE_FACT_FLUSH_BACKOFF)
    {
        tgt_ForbidFactorySettingsFlushing(FALSE);
    }
#endif
#endif

    // calculate %, print and return value...
    g_pmBatteryStable.precent = pmd_BatteryLevelToPercent(g_pmBatteryStable.stable);
    // force histeresys to 100% when charge is full
    switch (pmd_GetChargerStatus())
    {
    case PMD_CHARGER_FULL_CHARGE:
        {
            g_pmBatteryStable.stable = g_pmBatteryStable.avg;
            g_pmBatteryStable.precent = 100;
            bcl = 100;
        }
        break;
    case PMD_CHARGER_PLUGGED:
    case PMD_CHARGER_PRECHARGE:
    case PMD_CHARGER_FAST_CHARGE:
    case PMD_CHARGER_PULSED_CHARGE:
    case PMD_CHARGER_PULSED_CHARGE_WAIT:
        {
            if (bcl > 99)
            {
                g_pmBatteryStable.precent = 99;
                bcl = 99;
            }
        }
        break;
    default:
        break;
    }

    if (pPercent != NULL)
    {
        *pPercent = g_pmBatteryStable.precent;
    }
    CSW_TRACE(CFW_PM_TS_ID, "pm:    instant: %dmV, %d%%; mean: %dmV, %d%%; stable: %dmV, %d%%",
                              batLevel, bcl, g_pmBatteryStable.avg, avgPercent, g_pmBatteryStable.stable, g_pmBatteryStable.precent);

    #ifdef PMD_TWO_BATTARY
    CSW_TRACE(CFW_PM_TS_ID, "pm:here flag =%d, count = %d flag %d",g_start_quick_detect_flag,g_main_battery_detector,g_start_quick_detect_flag);
    if (g_start_quick_detect_flag == TRUE)
    {   
        if(g_main_battery_detector <= (PM_AVERAGE_BATTERY_NB -3))
        { 
            g_main_battery_detector ++;
            if (batLevel > g_pmBatteryStable.stable)
            {
                if (batLevel - g_pmBatteryStable.stable > 80)
                {
                  CSW_TRACE(CFW_PM_TS_ID, "pm: > plugin Mbattery, %dmv count = %d",batLevel,g_main_battery_detector);
                  return PMD_BAD_VALUE;
                }
            }
            else if (batLevel < g_pmBatteryStable.stable)
            {
                if (g_pmBatteryStable.stable - batLevel > 150)
                {
                  CSW_TRACE(CFW_PM_TS_ID, "pm: < plugin Mbattery, %dmv count =%d",batLevel,g_main_battery_detector);
                  return PMD_BAD_VALUE;
                }
            }
        }
    }
    #endif

    return g_pmBatteryStable.stable;
}

#if (PM_VMIC_POWER_POLICY != 0)
/// callback to sync with paging perriod
PRIVATE VOID pm_lpsCallback(VOID)
{
    COS_EVENT ev;
    ev.nEventId = EV_CSW_PRV_LPS_WAKEUP;
    ev.nParam1  = 0;
//    ev.nParam2  = 0;//for delete nParam2
//    ev.nParam3  = 0;//for delete nParam3
    //COS_SendEvent(MOD_SYS,&ev,COS_WAIT_FOREVER,COS_EVENT_PRI_NORMAL);
}
#endif

/*Description: g_pmChargeFlag
*  send an event to update the battery status */
 BOOL pm_BattChargeRemind(PMD_CHARGER_STATUS_T status)
{
    UINT8  bcl      = 0;
    UINT8  bcs      = 0;
    UINT16 batState = 0;
    UINT16 batLevel = 0;
    COS_EVENT ev;
    if (g_pmChargeFlag == 1)
    {
        #ifndef PMD_TWO_BATTARY

        batLevel = g_pmBatteryStable.stable;
        bcl      = g_pmBatteryStable.precent;
        CSW_TRACE(CFW_PM_TS_ID, "pm: Remind when plugin or out");
        #else
        if(pmd_GetCurrentBattery() == PMD_BATTERY_SUB)
        {
            batLevel = g_pmBatteryStable_sub.stable;
            bcl  = g_pmBatteryStable_sub.precent;
           CSW_TRACE(CFW_PM_TS_ID, "pm: Remind when plugin or out sub bcl=%d",bcl);
        }
        else
        {
           batLevel = g_pmBatteryStable.stable;
           bcl      = g_pmBatteryStable.precent;
           CSW_TRACE(CFW_PM_TS_ID, "pm: Remind when plugin or out main bcl=%d",bcl);
        }

        #endif
        
    }
    else
    {
        #ifdef PMD_TWO_BATTARY
        UINT8  bcl_sub = 0;
        CSW_TRACE(CFW_PM_TS_ID, "pm: pm_BattChargeRemind .");
        batLevel = pm_GetStableBatteryLevel(&bcl);
        UINT16 batLevel_sub = pm_GetStableBatteryLevel_sub(&bcl_sub);
        if(pmd_GetCurrentBattery() == PMD_BATTERY_SUB)
        {
            batLevel = batLevel_sub;
            bcl = bcl_sub;
        }
        #else
        batLevel = pm_GetStableBatteryLevel(&bcl);
        #endif
    }
      
    // send the full charge status only once
    // TODO: if toping up is enabled, the condition might be to wait until
    // the charger is unplugged or disabled before allowing to send the
    // FULL CHARGE message to the mmi again
    if ( (g_pm_oldStatus == PMD_CHARGER_FULL_CHARGE)&& (status == PMD_CHARGER_FULL_CHARGE))
    {
        CSW_TRACE(CSW_PM_TRC, 0, "pm: Remind Full charge not reporting to MMI anymore Bcl=%d%%, batLevel=%dmV", bcl, batLevel);
        return TRUE;
    }
    if ( (g_pm_oldStatus == PMD_CHARGER_TIMED_OUT)&& (status == PMD_CHARGER_TIMED_OUT))
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm: Remind timeout charge not reporting to MMI anymore Bcl=%d%%, batLevel=%dmV", bcl, batLevel);
        return TRUE;
    }
    
    if ((batLevel == PMD_BAD_VALUE) || (batLevel < 2600))
    { 
        CSW_TRACE(CFW_PM_TS_ID, "pm: Remind PMD_BAD_VALUE");
        return FALSE;
       
    }

    bcs = pm_PmdChargerStatus2Bcs(status);
    pm_BatteryLevel2State(batLevel, &bcl, &batState);

    CSW_TRACE(CFW_PM_TS_ID, "pm:..  bcs %d  batLevel =%dmv",bcs,batLevel);

    if (bcs == PM_CHARGER_DISCONNECTED)
    {
        if (batState == PM_BATTERY_STATE_SHUTDOWN)
        {
          CSW_TRACE(CFW_PM_TS_ID, "pm: PM_BATTERY_STATE_SHUTDOWN times= %d ,V =%dmV",g_pmRemindTime,batLevel);
          g_pmRemindTime ++;
          if (g_pmRemindTime <= 15)
          {
              return FALSE;
          }
          else
          {
              g_pmRemindTime = 0;
          }
        }
        else
        {
            g_pmRemindTime = 0;
        }
    }
    else
    {
        g_pmRemindTime = 15;
    }
    
    // when no measure valid for battery : bcl = BATTERY_PERCENT_LEVEL_40
    if (batState == PM_BATTERY_STATE_UNKNOWN)
    {
        CSW_TRACE(CFW_PM_TS_ID, "pm: Remind PMD_BAD_VALUE ,error status");
        return FALSE;
    }

    // force histeresys to 100% when charge is full
    switch (status)
    {
    case PMD_CHARGER_FULL_CHARGE:
        {
            g_pmBatteryStable.stable = g_pmBatteryStable.avg;
            g_pmBatteryStable.precent = 100;
            bcl = 100;
        }
        break;
    case PMD_CHARGER_PLUGGED:
    case PMD_CHARGER_PRECHARGE:
    case PMD_CHARGER_FAST_CHARGE:
    case PMD_CHARGER_PULSED_CHARGE:
    case PMD_CHARGER_PULSED_CHARGE_WAIT:
        {
            if (bcl > 99)
            {
                g_pmBatteryStable.precent = 99;
                bcl = 99;
            }
        }
        break;
    default:
        break;
    }

    ev.nEventId = EV_PM_BC_IND;
    ev.nParam1  = MAKEUINT32(bcl, bcs);
    //ev.nParam2  = MAKEUINT32(batState, 0);
//    ev.nParam2  = MAKEUINT32(PM_BATTERY_STATE_NORMAL, 0);//for delete nParam2
//    ev.nParam3  = MAKEUINT32(batLevel, 0);//for delete nParam3
    dm_SendPMMessage(&ev);

    CSW_TRACE(CFW_PM_TS_ID, "pm: Remind Bcs=%d, Bcl=%d%%, batState=%d, stable_volt=%dmV", bcs, bcl, batState, batLevel);
    if (g_pmBatteryStable.stable < 3250)
    {
      if (PM_CHARGER_DISCONNECTED == bcs)
      {
          CSW_TRACE(CFW_PM_TS_ID, "pm: CSW send shutdown message to mmi,but no response");
      }
    }
    return TRUE;
}

VOID pm_ChargerHandler(PMD_CHARGER_STATUS_T status)
{
#if (CSW_EXTENDED_API_PMD_USB == 1) && (CHIP_HAS_USB == 1)
    if(status != PMD_CHARGER_UNPLUGGED)
    {
        uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
    }
    else
    {
        uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
    }
#endif

    g_pmChargeFlag = 1;
    pm_BattChargeRemind(status);
    g_pmChargeFlag = 0;
}

PRIVATE VOID pm_EarPieceDetect(BOOL fromLowPowerEvent)
{
    CONST UINT32 vmicChannel = pmd_GetEarpieceGpadcChannel();

    if (g_pmEarDetect.mode == PMD_EAR_DETECT_INT)
    {
#ifdef ABB_HP_DETECT
        if (!g_pmEarDetect.earon)
#else
        if (!g_pmEarDetect.earon || !g_pmEarDetect.gpadcMonStarted)
#endif
        {
            // Disable gpadc channel
            hal_AnaGpadcClose(vmicChannel);
            // Disable ear piece power
            pmd_EnablePower(PMD_POWER_EARPIECE, FALSE);
            // Update status
            g_pmVMICActive = FALSE;

            return;
        }
    }

#if (PM_VMIC_POWER_POLICY == 0)
    // 0: VMIC always on, direct measure
    if (g_pmEarDetect.gpadcMonStarted && g_pmEarDetect.stateStable)
    {
        HAL_ANA_GPADC_MV_T ret = 0;
        COS_EVENT ev;
        ret = hal_AnaGpadcGet(vmicChannel);
        if (ret != HAL_ANA_GPADC_BAD_VALUE)
        {
            CSW_TRACE(CFW_PM_TS_ID, "0 earpiece detect: measure = %d", ret);
            ev.nEventId = EV_DM_GPADC_IND;
            ev.nParam1  = ret;
  //          ev.nParam2  = g_pmEarDetect.earon;//for delete nParam2
 //           ev.nParam3  = g_pmEarDetect.mode;//for delete nParam3
            COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            CSW_TRACE(CFW_PM_TS_ID, "pm: POLICY 0 send ear volt to mmi = %d,onstatus =%d", ret,g_pmEarDetect.earon); 
        }
    }
#elif ((PM_VMIC_POWER_POLICY == 1) || (PM_VMIC_POWER_POLICY == 2))
    // 1: off during low power
    // 2: every other paging during during low power

    if (
#if (PM_VMIC_POWER_POLICY == 1)
#ifdef VMIC_POWER_ON_WORKAROUND
        // Get VMIC voltage only if VMIC is valid
        hal_AudVMicValid() &&
#endif
#endif
        g_pmEarDetect.gpadcMonStarted && 
        g_pmVMICActive && 
        g_pmEarDetect.stateStable
       )
    {
        HAL_ANA_GPADC_MV_T ret = 0;
        COS_EVENT ev;
        ret = hal_AnaGpadcGet(vmicChannel);

        if (ret != HAL_ANA_GPADC_BAD_VALUE)
        {
            CSW_TRACE(CFW_PM_TS_ID, "1 earpiece detect: measure = %d", ret); 
#if (PM_EARPIECE_DET_DEBOUNCE)
            if (g_pmEarDetect.debouncing)
            {
                UINT32 abs;
                abs = (ret > g_pmEarDetect.prevValue) ?
                      (ret - g_pmEarDetect.prevValue) :
                      (g_pmEarDetect.prevValue - ret);
                if (abs <= PM_EARPIECE_DET_DEBOUNCE_VALUE)
                {
                    ret = (ret + g_pmEarDetect.prevValue) / 2;
                    g_pmEarDetect.debouncing = FALSE;
                }
                else
                {
                    g_pmEarDetect.prevValue = ret;
                }
            }
            else
            {
                g_pmEarDetect.debouncing = TRUE;
                g_pmEarDetect.prevValue = ret;
            }
            // If debounced
            if (!g_pmEarDetect.debouncing)
#endif // PM_EARPIECE_DET_DEBOUNCE
            {
                ev.nEventId = EV_DM_GPADC_IND;
                ev.nParam1  = ret;
  //              ev.nParam2  = g_pmEarDetect.earon;//for delete nParam2
  //              ev.nParam3  = g_pmEarDetect.mode;//for delete nParam3
                COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
                CSW_TRACE(CFW_PM_TS_ID, "pm: POLICY 1 send ear volt to mmi = %d,onstatus =%d", ret,g_pmEarDetect.earon); 
            }
        }
    }
#if (PM_EARPIECE_DET_DEBOUNCE)
    else
    {
        g_pmEarDetect.debouncing = FALSE;
    }
#endif // PM_EARPIECE_DET_DEBOUNCE

    if (fromLowPowerEvent == TRUE)
    {
#if (PM_VMIC_POWER_POLICY == 1)
        // recieved low power event, disable VMIC if needed
        if(g_pmVMICActive)
        {
            // disable gpadc channel
            hal_AnaGpadcClose(vmicChannel);
            // disable ear piece power
            pmd_EnablePower(PMD_POWER_EARPIECE, FALSE);
            // update status
            g_pmVMICActive = FALSE;
        }
#else
        // recieved low power event, toggle VMIC
        if(g_pmVMICActive)
        {
            // disable gpadc channel
            hal_AnaGpadcClose(vmicChannel);
            // disable ear piece power
            pmd_EnablePower(PMD_POWER_EARPIECE, FALSE);
            // update status
            g_pmVMICActive = FALSE;
        }
        else
        {
            // set high Vmic for earpiece detect. use the dedicated earpiece ressource
            pmd_EnablePower(PMD_POWER_EARPIECE, TRUE);             
            // update status
            g_pmVMICActive = TRUE;
            // ear piece is connected to GPADC channel 1 detection
            hal_AnaGpadcOpen(vmicChannel, HAL_ANA_GPADC_ATP_100MS);
        }
#endif
    }
    else
    {
        // recieved timer, enable VMIC if needed
        if(!g_pmVMICActive)
        {
            // set high Vmic for earpiece detect. use the dedicated earpiece ressource
            pmd_EnablePower(PMD_POWER_EARPIECE, TRUE);             
            // update status
            g_pmVMICActive = TRUE;
            // ear piece is connected to GPADC channel 1 detection
            hal_AnaGpadcOpen(vmicChannel, HAL_ANA_GPADC_ATP_100MS);
        }
    }
#endif
}

#ifdef PMD_TWO_BATTARY
#define PM_BATTERY_DETECT_INTERVAL 3700
#else
#define PM_BATTERY_DETECT_INTERVAL 3620
#endif

PRIVATE VOID pm_doMonitoring(BOOL fromLowPowerEvent)
{
    UINT32 now;
    // timer are set to the shorter time / paging event occurs about 
    // every 500ms, 1s or 2s 
    // so we must check elapsed time between each actual measuring
    // to avoid doing too much measures.
    now = hal_TimGetUpTime();
    if((now - g_pmBatMonLastDate) >= g_pmBatMonTime)
    {
        // monitor battery & charger and tell mmi
        PMD_CHARGER_STATUS_T status = pmd_GetChargerStatus();
        if (pm_BattChargeRemind(status))
        {
            // successful, record it
            g_pmBatMonLastDate = now;
        }
    }

    // g_pmEarPieceDetLastDate and g_pmEarPieceDetTime might be
    // changed in pm_ear_detect_inter().
    // But it causes no harm even if they are inconsistent here,
    // as the ultimate purpose is to run pm_EarPieceDetect().
    if ( fromLowPowerEvent ||
         (now - g_pmEarPieceDetLastDate) >= g_pmEarPieceDetTime )
    {
        g_pmEarPieceDetLastDate = now;

        // earpiece detect and tell mmi
        pm_EarPieceDetect(fromLowPowerEvent);
    }

    UINT32 callStatus = 0x00;
    if (fromLowPowerEvent)
    {
        // called from lp event, assuming not in call then ;)
    }
    else
    {
#if 0 // tianwq
#ifdef CFW_MULTI_SIM
        for(UINT8 nTmp = CFW_SIM_0; nTmp < CFW_SIM_COUNT; nTmp++)
        {
            callStatus |= CFW_CcGetCallStatus(nTmp);
        }
#else
        callStatus = CFW_CcGetCallStatus();
#endif
#endif
    }

    if (callStatus == 0 /*CC_STATE_NULL*/)
    {   // not in call
        UINT16 batState;
        UINT16 batLevel = g_pmBatteryStable.instant;
        UINT8  bcl = pmd_BatteryLevelToPercent(batLevel);
        // if battery level is low, use a different time
        // using pm_BatteryLevel2State for criteria
        pm_BatteryLevel2State(batLevel, &bcl, &batState);
        if (batState == PM_BATTERY_STATE_LOW)
        {
            g_pmBatMonTime = PM_BAT_MONITOR_IDLE_LOWBAT_TIME;

        }
        else if (batState == PM_BATTERY_STATE_CRITICAL)
        {
            g_pmBatMonTime = PM_BAT_MONITOR_IDLE_CRITICAL_TIME;
        }
        else if (batState == PM_BATTERY_STATE_SHUTDOWN)
        {
            g_pmBatMonTime = PM_BAT_MONITOR_IDLE_CRITICAL_TIME;
        }
        else
        {   
            if (g_pmBatteryStable.stable > PM_BATTERY_DETECT_INTERVAL)
            {
                g_pmBatMonTime = PM_BAT_MONITOR_IDLE_TIME;
            }
            else
            {
                g_pmBatMonTime = PM_BAT_MONITOR_IDLE_LOWBAT_TIME;
            }
        }

        if (g_pmEarDetect.mode == PMD_EAR_DETECT_POLLING)
        {
            g_pmEarPieceDetTime = PM_EARPIECE_DET_POLLING_IDLE_TIME;
        }
        else
        {
            // Avoid inconsistent states of g_pmEarDetect.earon and g_pmEarPieceDetTime,
            // as they might be changed in pm_ear_detect_inter and PM_StartEarPieceGpadcMonitor
            UINT32 scStatus = hal_SysEnterCriticalSection();
#ifdef ABB_HP_DETECT
            if ( g_pmVMICActive || g_pmEarDetect.earon )
#else
            if ( g_pmVMICActive || 
                 (g_pmEarDetect.earon && g_pmEarDetect.gpadcMonStarted) )
#endif
            {
                // To close VMIC, or open VMIC, or get GPADC value
#if (PM_EARPIECE_DET_DEBOUNCE)
                if (g_pmEarDetect.debouncing)
                {
                    g_pmEarPieceDetTime = PM_EARPIECE_DET_DEBOUNCE_TIME;
                }
                else
#endif // PM_EARPIECE_DET_DEBOUNCE
                {
                    g_pmEarPieceDetTime = PM_EARPIECE_DET_INT_IDLE_TIME;
                }
            }
            else
            {
                g_pmEarPieceDetTime = PM_EARPIECE_DET_MAX_TIME;
#if (PM_EARPIECE_DET_DEBOUNCE)
                g_pmEarDetect.debouncing = FALSE;
#endif // PM_EARPIECE_DET_DEBOUNCE
            }
            hal_SysExitCriticalSection(scStatus);
        }
    }
    else
    {   // in call
        g_pmBatMonTime = PM_BAT_MONITOR_CALL_TIME;

        if (g_pmEarDetect.mode == PMD_EAR_DETECT_POLLING)
        {
            g_pmEarPieceDetTime = PM_EARPIECE_DET_CALL_TIME;
        }
        else
        {
            // Avoid inconsistent states of g_pmEarDetect.earon and g_pmEarPieceDetTime,
            // as they might be changed in pm_ear_detect_inter and PM_StartEarPieceGpadcMonitor
            UINT32 scStatus = hal_SysEnterCriticalSection();
#ifdef ABB_HP_DETECT
            if ( g_pmVMICActive || g_pmEarDetect.earon )
#else
            if ( g_pmVMICActive ||
                 (g_pmEarDetect.earon && g_pmEarDetect.gpadcMonStarted) )
#endif
            {
                // To close VMIC, or open VMIC, or get GPADC value
#if (PM_EARPIECE_DET_DEBOUNCE)
                if (g_pmEarDetect.debouncing)
                {
                    g_pmEarPieceDetTime = PM_EARPIECE_DET_DEBOUNCE_TIME;
                }
                else
#endif // PM_EARPIECE_DET_DEBOUNCE
                {
                    g_pmEarPieceDetTime = PM_EARPIECE_DET_CALL_TIME;
                }
            }
            else
            {
                g_pmEarPieceDetTime = PM_EARPIECE_DET_MAX_TIME;
#if (PM_EARPIECE_DET_DEBOUNCE)
                g_pmEarDetect.debouncing = FALSE;
#endif // PM_EARPIECE_DET_DEBOUNCE
            }
            hal_SysExitCriticalSection(scStatus);
        }
    }

#ifdef PMD_TWO_BATTARY
    if (g_pmBatMonTime > PM_BAT_MONITOR_IDLE_TIME)
    {
        g_pmBatMonTime = PM_BAT_MONITOR_IDLE_TIME;
    }
    if (pmd_GetMainStatus() == FALSE)
    {
      g_main_battery_detector = 0;
      g_start_quick_detect_flag = FALSE;
    }
    else
    {
       g_start_quick_detect_flag = TRUE; 
       if (g_main_battery_detector <= (PM_AVERAGE_BATTERY_NB -3))
       {
           g_pmBatMonTime = MOINTER_TEMP_FOR_PLUGIN_MIAN_BATTERY;
       }
    }
#endif

    Time_To_MMI_Evt = MIN(g_pmBatMonTime, g_pmEarPieceDetTime);
}

extern UINT8 COS_GetSysTaskMbx(void);

VOID PM_StartEarPieceGpadcMonitor(BOOL start)
{
    UINT32 scStatus = hal_SysEnterCriticalSection();

    BOOL oldMonState = g_pmEarDetect.gpadcMonStarted;

    if (g_pmEarDetect.mode == PMD_EAR_DETECT_INT &&
        !g_pmEarDetect.earon)
    {
        // Earpiece is plugged-out
        start = FALSE;
    }
    if (g_pmEarDetect.gpadcMonDisabled)
    {
        // Earpiece GPADC monitoring is disabled
        start = FALSE;
    }

    g_pmEarDetect.gpadcMonStarted = start;

    if (g_pmEarDetect.mode == PMD_EAR_DETECT_INT &&
        oldMonState != start)
    {
        // Start monitoring right away
        // on -- start the earpiece key detection
        // off -- power off vmic
        g_pmEarPieceDetTime = PM_EARPIECE_DET_CALL_TIME;
        g_pmEarPieceDetLastDate = hal_TimGetUpTime() - g_pmEarPieceDetTime;

        hal_SysExitCriticalSection(scStatus);

        UINT8 mbx = COS_GetSysTaskMbx();//((TASK_HANDLE*)BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY))->nMailBoxId;
        //UINT32 mbxEvt[SXR_EVT_MBX_SIZE];
        UINT32 *mbxEvt = sxr_HMalloc(SXR_EVT_MBX_SIZE*sizeof(UINT32));
        mbxEvt[0] = HVY_TIMER_IN+PRV_BAL_BATTERY_TIMER2;
        mbxEvt[1] = NULL;
        #if OS_KERNEL == SXR 
        sxs_StopTimer(mbxEvt[0], NULL, mbx);
        sxr_Send(&mbxEvt, mbx, SXR_SEND_EVT);
        #else
        //TODO More needs to do 
        COS_SendEvent(MOD_SYS,(COS_EVENT*)mbxEvt, COS_WAIT_FOREVER,COS_EVENT_PRI_NORMAL);
        #endif

        sxr_HFree(mbxEvt);
        mbxEvt = NULL;
    }
    else
    {
        hal_SysExitCriticalSection(scStatus);
    }
}

BOOL PM_GetEarPieceGpadcMonitorStatus(VOID)
{
    return g_pmEarDetect.gpadcMonStarted;
}

VOID PM_DisableEarPieceGpadcMonitor(BOOL disable)
{
    UINT32 scStatus = hal_SysEnterCriticalSection();

    g_pmEarDetect.gpadcMonDisabled = disable;

    if (disable && g_pmEarDetect.gpadcMonStarted)
    {
        PM_StartEarPieceGpadcMonitor(FALSE);
    }
    else if (!disable && !g_pmEarDetect.gpadcMonStarted)
    {
        PM_StartEarPieceGpadcMonitor(TRUE);
    }

    hal_SysExitCriticalSection(scStatus);
}

void PM_SetBattery(UINT8 value)
{
	g_pmBatteryStable.precent = value;
}
/*Description:
*   this function gets:    pBcs[out]:the state of line.
*                          pBcl[out]:percent
*                          pMpc[out]:current,not surpport now.*/
BOOL PM_GetBatteryInfo(UINT8* pBcs, UINT8* pBcl, UINT16* pMpc)
{
	PMD_CHARGER_STATUS_T status;
    CSW_PROFILE_FUNCTION_ENTER(PM_GetBatteryInfo);
    if (pBcs == NULL || pBcl == NULL || pMpc == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(PM_GetBatteryInfo);
        return FALSE;
    }
    // *pBcs //Battery Charging  of status
    // from coolmmi/adamptation/src/adp_Events.c
    // 0: VBAT_STATUS
    // 1,2 : CHARGER_IN
    // 3: CHARGE_COMPLETE
    // 4: INVALID_CHARGER
    // 5: OVERBATTEMP
    {
        status = pmd_GetChargerStatus();
        if (status >= PMD_CHARGER_PLUGGED)
        {
            *pBcs = PM_CHARGER_CONNECTED;
        }
        else
        {
            *pBcs = PM_CHARGER_DISCONNECTED;
        }
        // calling pm_PmdChargerStatus2Bcs(status); here whould prevent event to
        // be sent correctly with the intermediate CHARGING_ADAPTER_IS_CONNECTED
        // state before any charge state that the MMI require
        // or the switch/case of pm_PmdChargerStatus2Bcs could be repeated here
    }
    
    // *pBcl //Battery Charging  of level
    // from coolmmi/adamptation/src/adp_Events.c
    // 0: no measure
    // 1..100: estimated charge %

    {
        UINT16 batLevel;
        UINT16 batState;
        // don't actually read from battery, get value from g_pmBatteryStable
        // if available. So that the mean calculation is done on the regular reporting basis only.
        CSW_TRACE(CFW_PM_TS_ID, "g_pmBatteryStable.nb   =%d",g_pmBatteryStable.nb);
        if (g_pmBatteryStable.nb == 0)
        {
        #ifdef PMD_TWO_BATTARY
        if(pmd_GetCurrentBattery() == PMD_BATTERY_SUB)
        {
            pm_GetStableBatteryLevel(pBcl);
            batLevel = pm_GetStableBatteryLevel_sub(pBcl);
        }
        else
        {
            batLevel = pm_GetStableBatteryLevel(pBcl);
        }
        #else
        batLevel = pm_GetStableBatteryLevel(pBcl);
        #endif
          
        }
        else
        {
            #ifndef PMD_TWO_BATTARY

            batLevel = g_pmBatteryStable.stable;
            *pBcl = g_pmBatteryStable.precent;
            #else
            if(pmd_GetCurrentBattery() == PMD_BATTERY_SUB)
            {
                batLevel = g_pmBatteryStable_sub.stable;
               *pBcl = g_pmBatteryStable_sub.precent;
              
            }
            else
            {
                batLevel = g_pmBatteryStable.stable;
               *pBcl = g_pmBatteryStable.precent;

            }
            #endif
        }
        pm_BatteryLevel2State(batLevel, pBcl, &batState);
        CSW_TRACE(CFW_PM_TS_ID, "PM_GetBatteryInfo Bcs=%d, Bcl=%d%% ,batLevel =%d mv", *pBcs, *pBcl,batLevel);
    }
    // *pMpc //Power consumption 
    // from coolmmi/adamptation/src/adp_Events.c
    *pMpc = 0;

    CSW_TRACE(CFW_PM_TS_ID, "PM_GetBatteryInfo Bcs=%d, Bcl=%d%% mv", *pBcs, *pBcl);


    CSW_PROFILE_FUNCTION_EXIT(PM_GetBatteryInfo);
    return TRUE;
}

// The following functions are only called from somewhere in platform

BOOL PM_CheckValidPowerOnVoltage(UINT16 nVoltage)
{
    return TRUE;
}

/*Description:
*   in this function, some numbers of this structure are
*not support now.
*Parameter:
*  pBatInfo[out]:
*Return:
*  BOOL type.TRUE is success,FALSE if fail.*/
BOOL PM_GetBatteryState(PM_BATTERY_INFO* pBatInfo)
{
    CSW_PROFILE_FUNCTION_ENTER(PM_GetBatteryState);

    // get voltage info from g_pmBatteryStable structure
    // any measure done yet ?
    if (g_pmBatteryStable.nb == 0)
    {
        // no! just exit
        CSW_PROFILE_FUNCTION_EXIT(PM_GetBatteryState);
        return FALSE;
    }
    pBatInfo->nBatteryRealTimeVoltage = g_pmBatteryStable.instant;
    
    pBatInfo->nBatteryVoltage = g_pmBatteryStable.stable;

    pBatInfo->nBatteryLifePercent  = g_pmBatteryStable.precent;

    pBatInfo->nBatteryTemperature  = pmd_GetBatteryTemperature();
    // no way to measure:
    pBatInfo->nBatteryCurrent = 0;
#ifdef BATT_FULL_TIME
    pBatInfo->nBatteryLifeTime     = BATT_FULL_TIME * g_pmBatteryStable.precent / 100;
    pBatInfo->nBatteryFullLifeTime = BATT_FULL_TIME;
#else
    pBatInfo->nBatteryLifeTime     = 0;
    pBatInfo->nBatteryFullLifeTime = 0;
#endif
    {
        UINT8 bcl       = g_pmBatteryStable.precent;
        UINT16 batState = 0;
        pm_BatteryLevel2State(pBatInfo->nBatteryRealTimeVoltage, &bcl, &batState);
        pBatInfo->nBatteryFlag = batState;
    }
    CSW_PROFILE_FUNCTION_EXIT(PM_GetBatteryState);
    return TRUE;
}

/*Description:
*   this function not surpport now
*Parameter:
*  none
*Return:
*  BOOL type.TRUE is success,FALSE if fail.*/
BOOL PM_BatteryChemistry (UINT8* pChemistryStatus)
{
    CSW_PROFILE_FUNCTION_ENTER(PM_BatteryChemistry);
    if(pChemistryStatus == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(PM_BatteryChemistry);
        return FALSE;
    }
    * pChemistryStatus = PM_BATTERY_CHEMISTRY_UNKNOWN;
    CSW_PROFILE_FUNCTION_EXIT(PM_BatteryChemistry);
    return TRUE;
}

// from charge_mon.c

/*Description:
there are two timer event ,charge event and mmi event ,share one system timer
to monitor the charge and mmi display
*  init PM module parameters
*Parameter:
*  none
*Return:
*  BOOL type.TRUE if successful,FALSE if fail.*/

#ifdef PMD_TWO_BATTARY

//#define POWER_ON_VOL_PER 25
extern VOID pmd_SetPMSub();
extern VOID pmd_SetPMMain();

#else
//#define POWER_ON_VOL_PER 10
#endif

VOID pm_ear_detect_inter(PMD_EAR_STATUS_T ear_status)
{
    CSW_TRACE(CFW_PM_TS_ID, "pm: ear int = %d", ear_status); 
    COS_EVENT ev;

    if (ear_status == PMD_EAR_STATUS_UNKNOWN)
    {
        g_pmEarDetect.stateStable = FALSE;
        return;
    }
    g_pmEarDetect.stateStable = TRUE;

    g_pmEarDetect.earon = (ear_status == PMD_EAR_STATUS_IN);
    ev.nEventId = EV_DM_GPADC_IND;
    if (g_pmEarDetect.earon)
    {
        ev.nParam1  = 512;
    }
    else
    {
        ev.nParam1  = 2000;
    }
//    ev.nParam2  = g_pmEarDetect.earon;//for delete nParam2
//    ev.nParam3  = g_pmEarDetect.mode;//for delete nParam3
    COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    PM_StartEarPieceGpadcMonitor(g_pmEarDetect.earon);
}

BOOL pm_BatteryInit(VOID)
{

    CSW_PROFILE_FUNCTION_ENTER(pm_BatteryInit);
    UINT32 now = hal_TimGetUpTime();
    Time_To_MMI_Evt = MIN(g_pmBatMonTime, g_pmEarPieceDetTime);
    // force last date to be in the past so that first reporting will trigger an update
    g_pmBatMonLastDate = now - g_pmBatMonTime;
    g_pmEarPieceDetLastDate = now - g_pmEarPieceDetTime;

    for (int i=0; i<PM_AVERAGE_BATTERY_NB; i++)
    {
        g_pmBatteryStable.sortedIdx[i] = i;
    }
#ifndef FPGA

    /// shutdown if battery too low & no charger
    {
        UINT16 batLevel = PMD_BAD_VALUE;
        UINT8 bcl = 40;
        #ifdef PMD_TWO_BATTARY
        UINT16 batLevel_sub = PMD_BAD_VALUE;
        UINT8 bcl_sub = 40;
        #endif
/*         UINT16 batState = 0; */
        PMD_CHARGER_STATUS_T status = PMD_CHARGER_UNKNOWN;
        g_pm_poweronvolt = pmd_GetPowerOnVolt();
        g_pm_powerdowervolt = pmd_GetPowerDownVolt();
        #ifdef PMD_TWO_BATTARY
        g_pm_sub_poweronvolt = pmd_GetSubPowerOnVolt();
        #endif

        // wait for a valid charger detection and valid battery measurement
        while ((batLevel == PMD_BAD_VALUE) || (status == PMD_CHARGER_UNKNOWN))
        {
            status = pmd_GetChargerStatus();
            #ifdef PMD_TWO_BATTARY
            pmd_SetCurrentBattery(PMD_BATTERY_SUB);
            batLevel_sub = pmd_GetBatteryLevel(&bcl_sub);
            pmd_SetCurrentBattery(PMD_BATTERY_MAIN);
            batLevel = pmd_GetBatteryLevel(&bcl);
            if (batLevel == PMD_BAD_VALUE)
            {
                batLevel = batLevel_sub;
                bcl = bcl_sub;
                pmd_SetCurrentBattery(PMD_BATTERY_SUB);
            }
            #else
            batLevel = pmd_GetBatteryLevel(&bcl);
            #endif
        }
            #ifdef PMD_TWO_BATTARY
            pmd_SetPMMain();
            #endif

        //pm_BatteryLevel2State(batLevel, &bcl, &batState);
        if ((status == PMD_CHARGER_UNPLUGGED) && 
            //(batState == PM_BATTERY_STATE_SHUTDOWN)
            (bcl <= g_pm_poweronvolt) // less than 10%, mmi will shut down if <20%, it will shut off immediately, however it's better for the user than no message at all.
            )
        {   
            #ifdef PMD_TWO_BATTARY
                if (bcl_sub <= g_pm_sub_poweronvolt)
                {
                    hal_SysShutdown();
                }
                else
                {
                    pmd_SetCurrentBattery(PMD_BATTERY_SUB);
                    pmd_SetPMSub();
                }
                  
            #else
               hal_SysShutdown();
            #endif
        }
    }
#endif // !FPGA
    // timer to monitor the bat level for mmi set 100 ms for the first reporting
    //COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY), PRV_BAL_BATTERY_TIMER2, COS_TIMER_MODE_SINGLE, HAL_TICK1S / 10); 

    // Initiate charger status
    pm_ChargerHandler(pmd_GetChargerStatus());
    // register the charger handler to get charger event faster
    pmd_SetChargerStatusHandler(pm_ChargerHandler);

#if 0 // ear detect is in edrv/gpio
    if (pmd_GetEarModeStatus() == PMD_EAR_DETECT_POLLING)
    {
        g_pmEarDetect.mode = PMD_EAR_DETECT_POLLING;
        g_pmEarDetect.stateStable = TRUE;
        PM_StartEarPieceGpadcMonitor(TRUE);
        g_pmEarPieceDetTime = PM_EARPIECE_DET_POLLING_IDLE_TIME;
        // Set high Vmic for earpiece detect
        pmd_EnablePower(PMD_POWER_EARPIECE, TRUE);
        g_pmVMICActive = TRUE;
        // Enable earpiece GPADC measurement
        hal_AnaGpadcOpen(pmd_GetEarpieceGpadcChannel(), HAL_ANA_GPADC_ATP_100MS);
    }
    else
    {
        g_pmEarDetect.mode = PMD_EAR_DETECT_INT;
        pmd_SetEarDetectHandler(pm_ear_detect_inter);
    }
#endif

#if (PM_VMIC_POWER_POLICY != 0)
    hal_LpsSetCallback(pm_lpsCallback);
#endif

    CSW_PROFILE_FUNCTION_EXIT(pm_BatteryInit);
    return TRUE;
}

/*Description:
*  the main monitor process. dispose voltage and charge.
*it is important process.
*Parameter:
*  pev[in]: the event from sysytem.
*Return:
*  BOOL type.TRUE if successful,FALSE if fail.*/
/*first is it a low power event ?
second is it the timer1 event ? 
    which elapse 1 ms to verify whether the Adapter is on/off ?
third is it the timer2 event ?
    first it measures all the ADC status, and save the new value of the ADC 
    second it deals with  the MMI event
    third it deals with the charge event
    both the MMI event phase and the charge event phase  we do not need to call the functions
    which are related with ADC, we can only get the data from the data structure.*/

BOOL pm_BatteryMonitorProc(COS_EVENT * pEv)
{
    CSW_PROFILE_FUNCTION_ENTER(pm_BatteryMonitorProc);
    
#if 0
    if(EV_CSW_PRV_LPS_WAKEUP == pEv->nEventId)
    {
        //CSW_TRACE(CSW_PM_TRC, 0, "Power UP event");

        // kill the timer, to start a long one (at least 3s) so that
        // paging occur more often than timer
        //sxs_StopTimer(HVY_TIMER_IN+ PRV_BAL_BATTERY_TIMER2, NULL, ((TASK_HANDLE*)BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY))->nMailBoxId);

        // monitor battery & charger, earpiece detect and tell mmi
        pm_doMonitoring(TRUE);

        //sxs_StartTimer(MAX(Time_To_MMI_Evt, 3 * HAL_TICK1S), HVY_TIMER_IN+PRV_BAL_BATTERY_TIMER2, NULL, FALSE, ((TASK_HANDLE*)BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY))->nMailBoxId);

    }
    else if(EV_TIMER == pEv->nEventId)
    {
        //mmi event timer occured, it makes jade enter low power mode


        if(PRV_BAL_BATTERY_TIMER2 == pEv->nParam1)
        {
            // monitor battery & charger, earpiece detect and tell mmi
            pm_doMonitoring(FALSE);

            //sxs_StartTimer(Time_To_MMI_Evt, HVY_TIMER_IN+PRV_BAL_BATTERY_TIMER2, NULL, FALSE, ((TASK_HANDLE*)BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY))->nMailBoxId);
        }
    }
#endif
    pm_doMonitoring(FALSE);

    CSW_PROFILE_FUNCTION_EXIT(pm_BatteryMonitorProc);
    return ERR_SUCCESS;    
}


// functions added to mmi: for fixing battery level accoring to backlight (= mmi activity)
VOID Set_BackLight_Flag_False(VOID)
{
    ;//pmd_SetHighActivity(FALSE);
}
VOID Set_BackLight_Flag_True(VOID)
{
    ;//pmd_SetHighActivity(TRUE);
}

// just for the factory mode now
// note: the battery volt is double of the return value.
//       0xffff mean the bad value

UINT16 PM_GetGpadcValue(PM_GPADC_CHAN_T channel)
{
    CONST UINT32 vbatChannel = pmd_GetBatteryGpadcChannel();
    CONST UINT32 vmicChannel = pmd_GetEarpieceGpadcChannel();
    CONST UINT32 vChrChannel =  pmd_GetChargerGpadcChannel();
    UINT32 scStatus;
    // skip earpiece/battery/charger measurement channels
    if (channel != vmicChannel && channel != vbatChannel && channel != vChrChannel)
    {
        hal_AnaGpadcOpen(channel, HAL_ANA_GPADC_ATP_1MS);
    }
    scStatus = hal_SysEnterCriticalSection();
    if (channel == vmicChannel)
    {
        if (g_pmEarDetect.mode != PMD_EAR_DETECT_POLLING && !g_pmEarDetect.earon)
        {
            hal_AnaGpadcOpen(vmicChannel, HAL_ANA_GPADC_ATP_1MS);
        }
    }
    else if (channel == vChrChannel)
    {
        if ( !pmd_ChargerChrVoltMonEnabled() || 
             g_pm_oldStatus == PMD_CHARGER_UNKNOWN ||
             g_pm_oldStatus == PMD_CHARGER_UNPLUGGED )
        {
            hal_AnaGpadcOpen(vChrChannel, HAL_ANA_GPADC_ATP_1MS);
        }
    }
    hal_SysExitCriticalSection(scStatus);
    COS_Sleep(5);

    UINT16 ret;
    for (int i=0; i<2; i++) // try once again if failure
    {
        if (channel == vbatChannel)
        {
            ret = pmd_GetGpadcBatteryLevel();
        }
        else if (pmd_ChargerChrVoltMonEnabled() && channel == vChrChannel)
        {
            ret = pmd_GetGpadcChargerLevel();
        }
        else
        {
            ret = hal_AnaGpadcGet(channel);
        }
    
        if (ret != 0xFFFF)
        {
            break;
        }
        COS_Sleep(5);
    }

    // skip earpiece/battery/charger measurement channels
    if (channel != vmicChannel && channel != vbatChannel && channel != vChrChannel)
    {
        hal_AnaGpadcClose(channel);
    }
    scStatus = hal_SysEnterCriticalSection();
    if (channel == vmicChannel)
    {
        if (g_pmEarDetect.mode != PMD_EAR_DETECT_POLLING && !g_pmEarDetect.earon)
        {
            hal_AnaGpadcClose(vmicChannel);
        }
    }
    else if (channel == vChrChannel)
    {
        if ( !pmd_ChargerChrVoltMonEnabled() || 
             g_pm_oldStatus == PMD_CHARGER_UNKNOWN ||
             g_pm_oldStatus == PMD_CHARGER_UNPLUGGED )
        {
            hal_AnaGpadcClose(vChrChannel);
        }
    }
    hal_SysExitCriticalSection(scStatus);

    CSW_TRACE(CFW_PM_TS_ID, "pm: PM_GetGpadcValue = %dmv, channel =%d", ret, channel);
    return ret;
}




VOID pm_ForceUpdateChargerStatus(VOID)
{
    pm_ChargerHandler(pmd_GetChargerStatus());
}

// pm_GetUsbDownLoadStatus() tells whether USB inserted is for USB-Update or not.
// In current design, special USB cables which have short MIC_DET, are made for USB-Update.
// When a USB is plug-in, and this function returns TRUE, the phone goes into USB-Update mode directly.
BOOL pm_GetUsbDownLoadStatus(VOID)
{
    BOOL ret = FALSE;
    if(hal_AnaGpadcGet(pmd_GetEarpieceGpadcChannel()) < 300)
    {
        ret = TRUE;
    }
    return ret;
}

VOID pm_TFlashDetect(BOOL cardPlugged)
{
        COS_EVENT ev;

        ev.nEventId = EV_DM_GPIO_IND;
        ev.nParam1  = cardPlugged;
//        ev.nParam2  = 0xff;//for delete nParam2
//        ev.nParam3  = 0;//for delete nParam3
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

