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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_lps.c                                                           //
/// That file contains the LowPower Synchronizer (LPS) driver                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "global_macros.h"

#include "chip_id.h"

// HW modules
#include "tcu.h"
#include "gpio.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "sys_ctrl.h"
#include "cfg_regs.h"

// HAL drivers
#include "hal_sys.h"
#include "hal_lps.h"
#include "hal_mem_map.h"
#include "hal_rda_audio.h"
#include "halp_debug.h"
#include "halp_lps.h"
#include "halp_sys.h"
#include "halp_sim.h"
#include "halp_fint.h"
#include "halp_ana_gpadc.h"
#include "halp_gpio.h"
#include "hal_host.h"

// External drivers
#include "rfd_xcv.h"
#include "pmd_m.h"
#include "sxr_tksd.h"
#include "sxr_tls.h"

#include "cos.h"

#if OS_KERNEL == FREERTOS
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#endif

#undef HAL_FUNC_INTERNAL
#define HAL_FUNC_INTERNAL

/// Default wrap value for the TCU for a GSM frame
#define LPS_TCU_DEFAULT_WRAP_COUNT      4999

/// Ticks of 1 GSM frame
#define HAL_LPS_TIME_ONE_GSM_FRAME      (HAL_TICK1S*60/13000 + 2)

/// Weather system has entered LPS sleep before. It is used to speed up booting process.
PUBLIC BOOL g_halLpsSleptBefore = FALSE;

PROTECTED HAL_LPS_CONTEXT_T g_halLpsCtx;

VOLATILE UINT32 HAL_UNCACHED_BSS g_halLpsForceNoSleep = 0;

/// put the variable for ECO MODE PIN in internal ram
PRIVATE UINT32 HAL_DATA_INTERNAL g_halLpModeGpo = 0;

// ============================================================================
// g_halSysLpsLock
// ----------------------------------------------------------------------------
/// Prevent the hal_SysSetRequestFreq function from changing the system
/// frequency. This lock is used by the LPS mechanism to keep a coherent
/// calibration.
// ============================================================================
PROTECTED UINT8 g_halSysLpsLock = HAL_SYS_LPS_LOCK_UNLOCKED;

extern BOOL bal_GetSleepState(VOID);
extern BOOL hal_IsBtStart(VOID);
extern BOOL hal_IsBtSleep(VOID);

// =============================================================================
// hal_LpsClockToFactor
// -----------------------------------------------------------------------------
/// compute the factor of 13MHz of the given clock.
/// @return clk/13MHz
// todo: profile to check whether a divide or a case is faster for 
// implementation...
// =============================================================================
INLINE UINT32 hal_LpsClockToFactor(HAL_SYS_FREQ_T clk)
{
#if 0
    UINT32 factor;
    switch (clk) {
    case HAL_SYS_FREQ_13M: 
        factor = 1; 
        break;
    case HAL_SYS_FREQ_26M: 
        factor = 2; 
        break;
    case HAL_SYS_FREQ_39M: 
        factor = 3; 
        break;
    case HAL_SYS_FREQ_52M: 
        factor = 4; 
        break;
    case HAL_SYS_FREQ_78M: 
        factor = 6; 
        break;
    case HAL_SYS_FREQ_104M: 
        factor = 8; 
        break;
    default: // assert
        // 26 MHz
        factor = 2;
    }
#else
    return clk/HAL_SYS_FREQ_13M;
#endif
}

// prototype
PRIVATE VOID hal_LpsStartLowPowerCount(VOID);

// =============================================================================
// hal_LpsOpen
// -----------------------------------------------------------------------------
/// Initialize the lps module
/// @param pLpsConfig State machine configuration
// =============================================================================
PUBLIC VOID hal_LpsOpen(CONST HAL_LPS_CONFIG_T* pLpsConfig)
{
    union
    {
        UINT32 reg;
        HAL_LPS_POWERUP_CTRL_T bitfield;
    } puctrl;
    union
    {
        UINT32 reg;
        HAL_LPS_POWERUP_TIMINGS_T bitfield;
    } putimings;

    puctrl.bitfield=pLpsConfig->puCtrl;
    putimings.bitfield=pLpsConfig->puTimings;
    // ensure unspecified fields are 0
    // so keep only fields existing in the config structure
#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    puctrl.reg &= TCU_LPS_PU_CO_ON_MASK | TCU_LPS_PU_CLK_RF_ON 
           | TCU_LPS_PU_SPI_ON | TCU_LPS_PU_CO_POL_MASK | TCU_LPS_PU_CLK_RF_POL
           | TCU_LPS_PU_SPI_STEP_DONE
           | TCU_LPS_PU_MASK_CO_S1_MASK | TCU_LPS_PU_MASK_CO_S2_MASK
           | TCU_LPS_PU_MASK_CLK_RF_ENABLE | TCU_LPS_PU_MASK_ST_PLL_IGNORE
           | TCU_LPS_PU_MASK_SW_RF_SWITCH | TCU_LPS_PU_MASK_SW_FAST_IGNORE
           | TCU_LPS_PU_RUN_TCU_WAKEUP;
#else
    puctrl.reg &= TCU_LPS_PU_CO_ON_MASK | TCU_LPS_PU_CLK_RF_ON 
           | TCU_LPS_PU_SPI_ON | TCU_LPS_PU_CO_POL_MASK | TCU_LPS_PU_CLK_RF_POL
           | TCU_LPS_PU_SPI_STEP_DONE
           | TCU_LPS_PU_MASK_CO_S1_MASK | TCU_LPS_PU_MASK_CO_S2_MASK
           | TCU_LPS_PU_MASK_CLK_RF_ENABLE | TCU_LPS_PU_MASK_ST_PLL_ENABLE
           | TCU_LPS_PU_MASK_SW_RF_SWITCH | TCU_LPS_PU_MASK_SW_FAST_SWITCH
           | TCU_LPS_PU_RUN_TCU_WAKEUP;
#endif

    // HAL_TRACE(HAL_DBG_TRC |TSTDOUT , 0, "lpsPuCtrl:0x%x" ,puctrl.reg );

    g_halLpsCtx.lpsState = HAL_LPS_IDLE;
    g_halLpsCtx.lpsInvalidateCalib = FALSE;
    g_halLpsCtx.lpsAccuracy = pLpsConfig->lpsAccuracy;
    g_halLpsCtx.lpsRate = HAL_LPS_UNDEF_VALUE;
    g_halLpsCtx.calibFreq = 0;
    g_halLpsCtx.lpsRateLast = HAL_LPS_UNDEF_VALUE;
    g_halLpsCtx.calibFreqLast = 0;
    g_halLpsCtx.lpsSkipFrameAllowed = FALSE;
    g_halLpsCtx.lpsSkipFrameNumber = HAL_LPS_UNDEF_VALUE;
    g_halLpsCtx.lpsPuCtrl = puctrl.reg;
    g_halLpsCtx.lpsWakeUpFrameNbr = pLpsConfig->lpsWakeUpFrameNbr;
    g_halLpsCtx.lpsCallback= NULL;
    // setup the power up SM timings
    hwp_tcu->LPS_PU_Times = putimings.reg;

    // Add slow running to the wake up mask:
    // most of the time the resource checking conditioning the call
    // to hal_LpsDeepSleep will be done before "slow running" state is
    // reached, preventing to go into DeepSleep (and go into light Sleep
    // instead). Therefore we must get out of lightSleep to finally go to
    // deepSleep -> wake up on slow-running IT.
    hwp_tcu->LPS_PU_IRQ_Mask = TCU_LPS_IRQ_SLOW_RUNNING_MASK;

    // Init the lock
    g_halSysLpsLock = HAL_SYS_LPS_LOCK_UNLOCKED;
}

PRIVATE char HAL_POSSIBLY_UNUSED g__hal_LpsCalibrate__TRACE_START[] =
    "start calib processing (req %d clock %d)";
PRIVATE char HAL_POSSIBLY_UNUSED g__hal_LpsCalibrate__TRACE_STABLE[] =
    "End of LPS Calib. Stable Rate:0x%x";
PRIVATE char HAL_POSSIBLY_UNUSED g__hal_LpsCalibrate__TRACE_END[] =
    "End of LPS Calib. Rate:0x%x";

// ***  hal_LpsCalibrate() ***
// This function must be called at every frame interrupt to keep the calibration up to date
PUBLIC VOID HAL_FUNC_INTERNAL hal_LpsCalibrate()
{
    if (g_halLpsCtx.lpsState == HAL_LPS_IDLE)
    {
        g_halLpsCtx.lpsInvalidateCalib = FALSE;
    }
    if (g_halLpsCtx.lpsInvalidateCalib == TRUE)
    {
        // Need to invalidate
        g_halLpsCtx.lpsState = HAL_LPS_IDLE;
        g_halLpsCtx.lpsRate = HAL_LPS_UNDEF_VALUE;
        g_halLpsCtx.lpsInvalidateCalib = FALSE;
        // disable LPS SM - Keep the SF_ENABLE bit on if it was already enabled
        hwp_tcu->LPS_SF_Ctrl &= ~TCU_LPS_SF_LOWPOWER_MASK;
        HAL_PROFILE_WINDOW_EXIT(CALIB_VALID);
        return;
    }

    switch (g_halLpsCtx.lpsState)
    {
        case HAL_LPS_CALIBRATED:
        {
            // we are already calibrated - do nothing
            break;
        }
        case HAL_LPS_CALIB_RUNNING:
        {
            // A calibration is currently running
            // Check if it is done
            if (hwp_tcu->LPS_SF_Status & TCU_LPS_SF_CALIBRATIONDONE)
            {
                // The previously requested calibration has finished
                UINT32 lpsRate;
                UINT32 lpsRateAdjust;
                UINT32 NbSysClocks = hwp_tcu->LPS_SF_Sys_Count;

                // Get the measured rate
                if (LP_FRAC_NB_BITS < g_halLpsCtx.lpsAccuracy)
                {
                    lpsRate = NbSysClocks >> (g_halLpsCtx.lpsAccuracy - LP_FRAC_NB_BITS);
                }
                else 
                {
                    lpsRate = NbSysClocks << (LP_FRAC_NB_BITS - g_halLpsCtx.lpsAccuracy);
                }


                // adjust rate to the clock frequency of last calibration
                if (g_halLpsCtx.calibFreq == g_halLpsCtx.calibFreqLast)
                {
                    lpsRateAdjust = lpsRate;
                }
                else if (g_halLpsCtx.calibFreqLast != 0)
                {
                    UINT32 factor = hal_LpsClockToFactor(g_halLpsCtx.calibFreq);
                    UINT32 factorLast = hal_LpsClockToFactor(g_halLpsCtx.calibFreqLast);
                    lpsRateAdjust = (lpsRate * factor) / factorLast;
                }
                else
                {
                    // will be different as last rate is HAL_LPS_UNDEF_VALUE
                    lpsRateAdjust = lpsRate;
                }

                
                // only validate calibration when 2 measures returns the same value
                // allow measure error of 4 clocks (at g_halLpsCtx.calibFreqLast)
                if ( (lpsRateAdjust<g_halLpsCtx.lpsRateLast)
                    ?((g_halLpsCtx.lpsRateLast-lpsRateAdjust)<=4)
                    :((lpsRateAdjust-g_halLpsCtx.lpsRateLast)<=4) ) 
                {
                    // We measured comparable values 2 times in a row, save the result
                    // this makes the calibrated value legal
                    g_halLpsCtx.lpsRate = lpsRate;
                    g_halLpsCtx.lpsRateLast = lpsRate;
                    g_halLpsCtx.calibFreqLast = g_halLpsCtx.calibFreq;
                    
   
                    HAL_TRACE(HAL_LPS_TRC, 0, g__hal_LpsCalibrate__TRACE_STABLE , g_halLpsCtx.lpsRate);
    
                    // disable LPS SM - Keep the SF_ENABLE bit on if it was already enabled
                    hwp_tcu->LPS_SF_Ctrl &= ~TCU_LPS_SF_LOWPOWER_MASK;
                    g_halLpsCtx.lpsState=HAL_LPS_CALIBRATED;
                    // Exit the CALIB RUNNING Window
                    HAL_PROFILE_WINDOW_EXIT(CALIB_RUNNING);
                    // As the calib value is now valid - enter the CALIB_VALID window
                    HAL_PROFILE_WINDOW_ENTER(CALIB_VALID);
            
                } else 
                {
                    // The value is different from previous measurments - we can not consider it valid yet
                    HAL_TRACE(HAL_LPS_TRC, 0, g__hal_LpsCalibrate__TRACE_END, lpsRate);
                    // save the last measured rate
                    g_halLpsCtx.lpsRateLast = lpsRate;
                    g_halLpsCtx.calibFreqLast = g_halLpsCtx.calibFreq;
                    // disable LPS SM - Keep the SF_ENABLE bit on if it was already enabled
                    hwp_tcu->LPS_SF_Ctrl &= ~TCU_LPS_SF_LOWPOWER_MASK;
                    g_halLpsCtx.lpsState=HAL_LPS_IDLE;
                    // Exit the CALIB RUNNING Window
                    HAL_PROFILE_WINDOW_EXIT(CALIB_RUNNING);
                }
            } else
            {
                // The running calibration is not done yet
                // Do nothing this time and wait for later
            }
            break;
        }
        case HAL_LPS_IDLE:
        {
            // if hal_LpsClockSysChangeAllowed was not called, use current freq
            if (g_halLpsCtx.calibFreq == 0)
            {
                g_halLpsCtx.calibFreq = g_halSysSystemFreq;
            }
            // current freq must be requested calibration freq...
            // assert (g_halLpsCtx.calibFreq == g_halSysSystemFreq)

            HAL_TRACE(HAL_LPS_TRC, 0, g__hal_LpsCalibrate__TRACE_START, g_halLpsCtx.calibFreq, g_halSysSystemFreq);

            // the LPS is not calibrated yet
            // Start LPS Calibration with requested Accuracy
            HAL_PROFILE_WINDOW_ENTER(CALIB_RUNNING);
            g_halLpsCtx.lpsState=HAL_LPS_CALIB_RUNNING;
            hwp_tcu->LPS_SF_Restart_Time = (1<<g_halLpsCtx.lpsAccuracy)-1;
            hwp_tcu->LPS_SF_Ctrl |= TCU_LPS_SF_LOWPOWER_CALIB;
            break;
        }
        default:
        {
        }
    } 
}


PROTECTED BOOL hal_LpsClockSysChangeAllowed(HAL_SYS_FREQ_T nextClock)
{
    if (!g_halLpsSleptBefore)
    {
        // System is in booting process yet. Do not lower down the system frequency.
        if (nextClock < hal_SysGetFreq())
        {
            return FALSE;
        }
    }

#if (CHIP_HAS_ASYNC_TCU)

    // TCU has its own frequency source to calibrate, which is unrelated to system frequency.
    return TRUE;

#else

    // If a skip frame is in progress, we do not change the frequency 
    // in hal_SysChangeSystemClock() to keep synch'ed.
    // In the case this is called through hal_LpsGetElapsed & hal_FintIrqHandler
    // g_halSysLpsLock is HAL_SYS_LPS_LOCK_UNLOCKED.
    if(g_halSysLpsLock != HAL_SYS_LPS_LOCK_UNLOCKED)
    {
        return FALSE;
    }

    // Next clock is faster: need to recalibrate
    if (nextClock > g_halLpsCtx.calibFreq)
    {
        g_halLpsCtx.calibFreq = nextClock;
        HAL_PROFILE_PULSE(hal_LpsInvalidateCalib);
        HAL_TRACE(HAL_LPS_TRC, 0, "hal_LpsClockSysChangeAllowed clock switch up: Invalidate calib (%d)",nextClock);
        g_halLpsCtx.lpsInvalidateCalib = TRUE;
        return TRUE;        
    }

    // else avoid changing clock while calibration 
    if (g_halLpsCtx.lpsState != HAL_LPS_CALIBRATED)
    {
        return FALSE;
    }

    // we already have a valid calibration, allow clock change:
    // the rate will be computed for lower changes.
    return TRUE;

#endif
}

PUBLIC VOID hal_LpsInvalidateCalib()
{
#if (CHIP_HAS_ASYNC_TCU)

    // TCU frequency is fixed and one time of calibration is enough

#else

    // force doing a full calibration again
    HAL_PROFILE_PULSE(hal_LpsInvalidateCalib);
    HAL_TRACE(HAL_LPS_TRC, 0, "hal_LpsInvalidateCalib");
    g_halLpsCtx.lpsInvalidateCalib = TRUE;
    g_halLpsCtx.lpsRateLast = HAL_LPS_UNDEF_VALUE;
    g_halLpsCtx.calibFreqLast = 0;
    g_halLpsCtx.calibFreq = 0;

#endif
}

PUBLIC VOID HAL_FUNC_INTERNAL hal_LpsAllowSkipFrame(BOOL allow, UINT16 lastEvt)
{    
    g_halLpsCtx.lpsSkipFrameAllowed=allow;
    g_halLpsCtx.lpsFrameLastEvt=lastEvt;

#ifdef HAL_LPS156M_WORKAROUD
    if(g_halSysSystemFreq == HAL_SYS_FREQ_156M)
    {
        g_halLpsCtx.lpsSkipFrameAllowed = FALSE;
    }
#endif
#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    if(g_halSysSystemFreq != HAL_SYS_FREQ_26M)
    {
        g_halLpsCtx.lpsSkipFrameAllowed = FALSE;
    }
#endif
}

// =============================================================================
// hal_LpsSetCallback
// -----------------------------------------------------------------------------
/// @parameter cbfunc function to be called at the end of each power up, should
/// just send an event or similar simple code.
// =============================================================================
PUBLIC VOID hal_LpsSetCallback(HAL_LPS_CALLBACK_T* cbFunc)
{
    g_halLpsCtx.lpsCallback = cbFunc;
}


PUBLIC BOOL hal_LpsSkipFrame (UINT16 frameQty)
{
    // Check wether we can Suspend the fint right now or not:
    if ( (frameQty > 3) && (g_halLpsCtx.lpsSkipFrameAllowed) )
    {
#if (!CHIP_HAS_ASYNC_TCU)
        // Lock the System Frequency
        g_halSysLpsLock = HAL_SYS_LPS_LOCK_LGEF | HAL_SYS_LPS_LOCK_FINT;
#endif

        // Next frameQty frames without Fint
        if (frameQty < 512)
        {
            // if requested number below the max limit
            g_halLpsCtx.lpsSkipFrameNumber = frameQty;
        }
        else
        {
            // if requested number above the max limit - saturate it to the max
            g_halLpsCtx.lpsSkipFrameNumber = 512;
        }
        hwp_tcu->LPS_SF_Frames = TCU_LPS_SF_FRAME(g_halLpsCtx.lpsSkipFrameNumber-1)
            | TCU_LPS_SF_PU_FRAME(g_halLpsCtx.lpsSkipFrameNumber-g_halLpsCtx.lpsWakeUpFrameNbr);
        hwp_tcu->LPS_SF_Ctrl |= TCU_LPS_SF_ENABLE
#if (CHIP_HAS_ASYNC_TCU)
            | TCU_LPS_SF_WAKEUP0_ENABLED
#endif
            ;

        // Save current time
        g_halLpsCtx.lpsLastTimeToSkipFrame = hal_TimGetUpTime();

        // As starting the LP counter switches the TCU to the slow
        // clock, we should do this only with a working RTC
        // run counters on low power
        // If we have a valid Calibration...
        if (g_halLpsCtx.lpsState == HAL_LPS_CALIBRATED)
        {
            // ...setup the LP counters
            hal_LpsStartLowPowerCount();
        }

        return TRUE;
    } else

    return FALSE;
}

PUBLIC VOID hal_LpsResumeFrame(VOID)
{
    UINT32 tmp;

    if (hal_LpsFrameSkipping())
    {
        // Get the tick time elapsed since the frame skip starts
        tmp = hal_TimGetUpTime() - g_halLpsCtx.lpsLastTimeToSkipFrame;
        // Check if the frame resume and skip is very close
        if (tmp < HAL_LPS_TIME_ONE_GSM_FRAME)
        {
#if (CHIP_HAS_ASYNC_TCU)
            if (g_halLpsCtx.lpsState != HAL_LPS_CALIBRATED ||
                // LPS SF state machine is running, but does NOT enter
                // slow running state yet
                (hwp_tcu->LPS_SF_Status &
                    (TCU_LPS_SF_READY|TCU_LPS_SF_SLOWRUNNING)) == 0
               )
#else
            if (g_halLpsCtx.lpsWakeUpFrameNbr == 0 &&
                g_halLpsCtx.lpsState == HAL_LPS_CALIBRATED &&
                // LPS SF state machine is running, but does NOT enter
                // slow running state yet
                (hwp_tcu->LPS_SF_Status &
                    (TCU_LPS_SF_READY|TCU_LPS_SF_SLOWRUNNING)) == 0
               )
#endif
            {
                // The resume action and the skip action occur in the same frame.
                // This is not allowed if the wakeup frame number is 0, as the
                // frame has been shortened to save power, and the time must be
                // compensated via going through the slow running phase.

                // Starting from 8808 (CHIP_HAS_ASYNC_TCU),
                // if the last frame skip is stopped by manual resume,
                // LPS_SF_Elapsed_Frames will only be updated 1 frame after
                // the next frame skip starts; before that time it saves the
                // elasped frames in the last resume action. However, if 
                // the last frame skip is stopped automatically,
                // LPS_SF_Elapsed_Frames will be reset to zero in the
                // first FINT after SF is disabled.
                // So it is not allowed to check LPS_SF_Elapsed_Frames within
                // 1 frame since the frame skip starts.

                // Wait until 1 GSM frame elapses
                COS_SleepByTick(HAL_LPS_TIME_ONE_GSM_FRAME - tmp);
            }
        }
    }

    if (hal_LpsFrameSkipping()) 
    {
        // Do NOT stop the low power if it is in the last frame before power up
        if (g_halLpsCtx.lpsSkipFrameNumber != (hwp_tcu->LPS_SF_Elapsed_Frames + 1))
        {
            // If in low power calib, do nothing - if in low power skip frame, stop the low power
            // keep the skip frame in both cases to allow reading the elapsed frames
            tmp = hwp_tcu->LPS_SF_Ctrl & TCU_LPS_SF_LOWPOWER_MASK;
            if (tmp == TCU_LPS_SF_LOWPOWER_SKIPFRAME)
            {
                hwp_tcu->LPS_SF_Ctrl = TCU_LPS_SF_ENABLE
                    | TCU_LPS_SF_LOWPOWER_STOP
#if (CHIP_HAS_ASYNC_TCU)
                    | TCU_LPS_SF_WAKEUP0_ENABLED
#endif
                    ;
            }

            // Unmask Fint at last (to avoid race condition between here and hal_LpsGetElapsedFrames).
            // It is required to set hwp_tcu->LPS_SF_Ctrl in hal_LpsGetElapsedFrames (in Fint)
            // after setting it in hal_LpsResumeFrame. Otherwise TCU_LPS_SF_ENABLE will NOT
            // be cleared, and the skipped frame counter will keep going.

            // Setting the last frame to 0 will unmask the Fint
            hwp_tcu->LPS_SF_Frames = TCU_LPS_SF_FRAME(0) | TCU_LPS_SF_PU_FRAME(0);
        }
    }
}

PUBLIC UINT16 hal_LpsGetElapsedFrames(void)
{
    if (g_halLpsCtx.lpsSkipFrameNumber == HAL_LPS_UNDEF_VALUE)
    {
        // no skip has been programmed - return 1
        return 1;
    } 
    else
    {
        UINT32 tmp;
        UINT16 elapsed = (UINT16)hwp_tcu->LPS_SF_Elapsed_Frames;
        g_halLpsCtx.lpsSkipFrameNumber = HAL_LPS_UNDEF_VALUE;

#if (!CHIP_HAS_ASYNC_TCU)
        // TODO somewhere there: do a frequency change there if
        // one was attempted during g_halLpsCtx.lpsSkipFrameNumber != HAL_LPS_UNDEF_VALUE
        UINT32 status=hal_SysEnterCriticalSection();
        if (g_halSysLpsLock != HAL_SYS_LPS_LOCK_UNLOCKED)
        {
            g_halSysLpsLock &= ~HAL_SYS_LPS_LOCK_LGEF;
            if (g_halSysLpsLock == HAL_SYS_LPS_LOCK_UNLOCKED)
            {
                hal_SysUpdateSystemFrequency();
            }
        }
        hal_SysExitCriticalSection(status);
#endif

        // If not in Calibration, disable the Skip Frame & the low-power 
        tmp = hwp_tcu->LPS_SF_Ctrl & TCU_LPS_SF_LOWPOWER_MASK;
        if (tmp == TCU_LPS_SF_LOWPOWER_CALIB)
        {
            hwp_tcu->LPS_SF_Ctrl = TCU_LPS_SF_LOWPOWER_CALIB;
        } 
        else
        {
            hwp_tcu->LPS_SF_Ctrl = TCU_LPS_SF_LOWPOWER_STOP;
        } 
        // Return the number of elapsed frames during last skip
        return elapsed;
    }
}

// =============================================================================
// hal_LpsFrameSkipping
// -----------------------------------------------------------------------------
/// Check if LPS is in frame skipping status.
// ============================================================================= 
PROTECTED BOOL hal_LpsFrameSkipping(VOID)
{
    if ( g_halLpsCtx.lpsSkipFrameNumber != HAL_LPS_UNDEF_VALUE ||
         (hwp_tcu->LPS_SF_Ctrl & TCU_LPS_SF_ENABLE) != 0 )
    {
        return TRUE;
    }

    return FALSE;
}

PUBLIC VOID hal_LpsSleep(VOID)
{
    UINT32 cs;
    UINT32 resourceActive = 0;
    UINT32 i;
    UINT32 deepSleepWUCause = 0;
    BOOL lpsReady = FALSE;

    if(!g_halLpsSleptBefore)
    {
        /* don't sleep before system sleep first */
        if(!bal_GetSleepState())
            return;
    }

    // System enters LPS sleep for at least once
    g_halLpsSleptBefore = TRUE;

#if defined(__AT_MOD_SPECIAL__)
    return; // no sleep for audio calib;
#else
    if (g_halLpsForceNoSleep != 0)
    {
        return; // no sleep for some tests
    }
#endif
    // Enter CS
    cs = hal_SysEnterCriticalSection();

    // Update SIM Requested Frequency when not 32k
    //if (hal_SysGetRequestFreq(HAL_SYS_FREQ_SIM) != HAL_SYS_FREQ_32K)
    {
        // SIM is currently active - try to make it sleep
        //hal_SimCheckClockStatus();
    }

    // Update the GPADC Requested Frequency
    hal_AnaGpadcSleep();

#define TRACE_DEEP_SLEEP_CONDITION 0

#if (TRACE_DEEP_SLEEP_CONDITION == 1)
    static int trcCount = 0;
    trcCount++;
#endif // (TRACE_DEEP_SLEEP_CONDITION == 1)

    // Check all resources
    for (i = 0; i < HAL_SYS_FREQ_MAX_RESOURCE_NB/4; i++)
    {
#if (TRACE_DEEP_SLEEP_CONDITION == 1)
        // trace every 16 times when in sleep
        if ((trcCount&0xf) == 0x0 && g_halSysFreqScalRegistry[i])
        {
            HAL_TRACE(HAL_LPS_TRC|TSTDOUT, 0,
                "Busy app: [%d] = 0x%08x", i, g_halSysFreqScalRegistry[i]);
        }
#endif // (TRACE_DEEP_SLEEP_CONDITION == 1)

        resourceActive |= g_halSysFreqScalRegistry[i];
    }

    // check lps ready
    lpsReady = hal_LpsReady();
#if (TRACE_DEEP_SLEEP_CONDITION == 1)
    // trace every 16 times when in sleep
    if ((trcCount&0xf) == 0x0)
    {
        HAL_TRACE(HAL_LPS_TRC|TSTDOUT, 0,
            "lpsReady: %d", lpsReady);
    }
#endif // (TRACE_DEEP_SLEEP_CONDITION == 1)
    

    if ((resourceActive==0) && lpsReady)
    {
       //hwp_sysIrq->wakeup_mask = 0xffffffff;
        
        //hal_HstSendEvent(SYS_EVENT, 0x11050555);
        HAL_PROFILE_WINDOW_ENTER(DEEP_SLEEP);

        // Enter SX scheduling critical section
        #if OS_KERNEL == SXR
        sxr_EnterScSchedule();
        #else
        portENTER_CRITICAL();
        #endif

        // call hal_LpsDeepSleep() after setting XCPU's internal critical section 
        // and setting the stack pointer to the internal ram (using IRQ stack)
        deepSleepWUCause = hal_LpsDeepSleepWrapper(FALSE);
        
        // Exit SX scheduling critical section
        #if OS_KERNEL == SXR
        sxr_ExitScSchedule();
        #else
        portEXIT_CRITICAL();
        #endif

        HAL_PROFILE_WINDOW_EXIT(DEEP_SLEEP);
        //hal_HstSendEvent(SYS_EVENT, 0x11050554);
    } 
    else 
    {
        // Check the trace stream status
        // and shut down the CPU only if there is no
        // trace to send... 
        // Check the Sim status
        // and shut down the CPU only if the clock si is already off,
        // else the call to hal_SimCheckClockStatus() above will shut it down 
        // after a short hold time (the sim need the clock a few more cycles).
#if 1
        if ((hal_SysGetRequestFreq(HAL_SYS_FREQ_TRACE) == HAL_SYS_FREQ_32K)
            &&(hal_SysGetRequestFreq(HAL_SYS_FREQ_SIM) == HAL_SYS_FREQ_32K))
        {
#if (TRACE_DEEP_SLEEP_CONDITION == 1)
            if ((trcCount&0xf) == 0x0)
            {
                HAL_TRACE(HAL_LPS_TRC|TSTDOUT, 0,"LIGHT_SLEEP");
            }
#endif // (TRACE_DEEP_SLEEP_CONDITION == 1)
            // Set Wakeup Mask
            // + Debug uart in case of command coming to the host from 
            // trace tool.
            hwp_sysIrq->wakeup_mask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_DEBUG_UART;
            // Shut down XCPU clock
            HAL_PROFILE_WINDOW_ENTER(LIGHT_SLEEP);
            hal_SysXcpuSleep();
            //Add some nop to avoid crash after wake up in 5856
            {
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
            }
            HAL_PROFILE_WINDOW_EXIT(LIGHT_SLEEP);
        }
#else
        //hal_LpsNap();
#endif
    }

    // Update the GPADC Requested Frequency
    hal_AnaGpadcWakeUp();

    hal_SysExitCriticalSection(cs);
    

    // call the callback (if any) only if we went through Deep Sleep
    // we do it here and not in hal_LpsDeepSleep to avoid beeing in critical
    // section and because we want GPADC back on
    // also added some trace useful in debugging low power
    if ((resourceActive==0) && lpsReady)
    {
        //hal_HstSendEvent(SYS_EVENT, 0x11050553);
        if (deepSleepWUCause)
        {
            HAL_TRACE(HAL_LPS_TRC,0,"LPS Deep Sleep Wakeup because of Irq (0x%08x).",deepSleepWUCause);
        }
        else
        {
            HAL_TRACE(HAL_LPS_TRC,0,"LPS Deep Sleep Wakeup because of LPS Frame Reached.");
        }
        if (g_halLpsCtx.lpsCallback != NULL)
        {
            g_halLpsCtx.lpsCallback();
        }
        //hal_HstSendEvent(SYS_EVENT, 0x11050552);
    }
}

// -------------------  Private functions ---------------------------

PRIVATE VOID hal_LpsStartLowPowerCount(VOID)
{
    // assert skip_frame already active
    UINT16 tcu_wrap = LPS_TCU_DEFAULT_WRAP_COUNT;
    UINT16 tcu_reload = LPS_TCU_DEFAULT_WRAP_COUNT - 5; // 5QBit margin
    UINT32 rate;
    UINT32 divider;
    UINT16 stop=LPS_TCU_DEFAULT_WRAP_COUNT;
#if (!CHIP_HAS_ASYNC_TCU)
    UINT32 sysFactor   = hal_LpsClockToFactor(g_halSysSystemFreq);
    UINT32 calibFactor = hal_LpsClockToFactor(g_halLpsCtx.calibFreq);
    divider = sysFactor * 12;
    // /!\ rate calculation can saturate for high factors: 
    // OK up to 156Mhz: 
    // 13Mhz rate is about 26000000 (13Mhz/32768Hz*2^LP_FRAC_NB_BITS)
    // so 26000000*calibFactor*sysFactor must fit an UINT32.
    // Its OK for both clock at 156MHz where factor is 12.
    rate = (g_halLpsCtx.lpsRate * sysFactor) / calibFactor;
    HAL_TRACE(HAL_LPS_TRC, 0, "rate computed:0x%x for clock %d.", rate, g_halSysSystemFreq);
#else // (CHIP_HAS_ASYNC_TCU)
    if (g_halSysTcuRunningAt26M)
    {
        divider = 24;
    }
    else
    {
        divider = 12;
    }
    rate = g_halLpsCtx.lpsRate;
#endif // (!CHIP_HAS_ASYNC_TCU)

    if (g_halLpsCtx.lpsWakeUpFrameNbr == 0) 
    {
        // ensure LPS FSM is ready
        while (!(hwp_tcu->LPS_SF_Status & TCU_LPS_SF_READY));

        stop = hwp_tcu->Cur_Val + 20; // add a qbit margin
        if (stop < g_halLpsCtx.lpsFrameLastEvt)
        {
            stop = g_halLpsCtx.lpsFrameLastEvt;
        }
        if (stop > LPS_TCU_DEFAULT_WRAP_COUNT)
        {
            stop = LPS_TCU_DEFAULT_WRAP_COUNT;
        }
    }

    hwp_tcu->LPS_SF_Frame_Period = (tcu_wrap+1) * divider;
    hwp_tcu->LPS_SF_Restart_Time = ((tcu_reload+1) * divider) - 1; //  -1 clk &  +1 1/4bits to start <- to test 
    hwp_tcu->Ctrl = TCU_LOAD_VAL(tcu_reload + stop - LPS_TCU_DEFAULT_WRAP_COUNT) | TCU_ENABLE_ENABLED;
    hwp_tcu->LPS_SF_Rate = rate;
    // FIXME
    hwp_tcu->LPS_SF_Ctrl = TCU_LPS_SF_ENABLE
        | TCU_LPS_SF_LOWPOWER_SKIPFRAME
#if (CHIP_HAS_ASYNC_TCU)
        | TCU_LPS_SF_WAKEUP0_ENABLED
#endif
        ;
    // force frame to be shorter to enter in low power sooner
    hal_TcuSetWrap(stop);
}


BOOL hal_LpsReady(void)
{

    // if TCU is not enabled, then LPS does not need to be programmed
    if ((hwp_tcu->Ctrl & TCU_ENABLE_ENABLED) == TCU_ENABLE_DISABLED)
    {
        return TRUE;
    }
    return (
            // LPS Calibrated
            (g_halLpsCtx.lpsState == HAL_LPS_CALIBRATED) &&
            // Skip frame enabled and low power counter in skip frame mode
            ((hwp_tcu->LPS_SF_Ctrl & (TCU_LPS_SF_LOWPOWER_MASK | TCU_LPS_SF_ENABLE)) == (TCU_LPS_SF_LOWPOWER_SKIPFRAME | TCU_LPS_SF_ENABLE)) &&
            // State Machine running in Low Power (32k counter)
            (hwp_tcu->LPS_SF_Status & TCU_LPS_SF_SLOWRUNNING) //&&
            // more than lpsWakeUpFrameNbr+1 frames remaining 
            // if lpsWakeUpFrameNbr == 0 compare to 2 anyway
            //((g_halLpsCtx.lpsSkipFrameNumber - hwp_tcu->LPS_SF_Elapsed_Frames)
            // > (UINT32)(g_halLpsCtx.lpsWakeUpFrameNbr?1+g_halLpsCtx.lpsWakeUpFrameNbr:2))
        );
}


// =============================================================================
// hal_LpsIsResourceInactive
// -----------------------------------------------------------------------------
/// Checks if all the resource is inactive.
/// @param VOID;
/// @return TRUE if inactive, FALSE otherwise.
// ============================================================================= 
PUBLIC BOOL hal_LpsIsResourceInactive(VOID)
{
    UINT32 resourceActive = 0;
    UINT32 i;

    // Check all resources
    for (i = 0; i < HAL_SYS_FREQ_MAX_RESOURCE_NB/4; i++)
    {
        resourceActive |= g_halSysFreqScalRegistry[i];
    }

    if(resourceActive != 0)
    {
        return FALSE;
    }  
    return TRUE;
}


// =============================================================================
// hal_LpsRemainingFramesReady
// -----------------------------------------------------------------------------
/// Checks if PAL can go to low power mode for given number of frames.
/// @param frameNum the number of frames that can be in low power mode;
/// @return TRUE if can, FALSE otherwise.
// ============================================================================= 
PUBLIC BOOL hal_LpsRemainingFramesReady(UINT32 frameNum)
{

    // if TCU is not enabled, then LPS does not need to be programmed
    if ((hwp_tcu->Ctrl & TCU_ENABLE_ENABLED) == TCU_ENABLE_DISABLED)
    {
        return TRUE;
    }
    
    if (
        // LPS Calibrated
        (g_halLpsCtx.lpsState == HAL_LPS_CALIBRATED) &&
        // Skip frame enabled and low power counter in skip frame mode
        ((hwp_tcu->LPS_SF_Ctrl & (TCU_LPS_SF_LOWPOWER_MASK | TCU_LPS_SF_ENABLE)) == (TCU_LPS_SF_LOWPOWER_SKIPFRAME | TCU_LPS_SF_ENABLE)) &&
        // State Machine running in Low Power (32k counter)
        (hwp_tcu->LPS_SF_Status & TCU_LPS_SF_SLOWRUNNING) &&
        // lpsSkipFrameNumber valid
        (g_halLpsCtx.lpsSkipFrameNumber != HAL_LPS_UNDEF_VALUE) &&
        // more than frameNum frames remaining 
        ((int)((int)g_halLpsCtx.lpsSkipFrameNumber - (int)hwp_tcu->LPS_SF_Elapsed_Frames)
            > (int)frameNum)
       )
    {
        return TRUE;
    }

    return  FALSE;
}


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
/******************************************************************************
 * hal_LpsDeepSleep: 
 * DESCRIPTION: - No tcu FSM involved
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
UINT32 HAL_FUNC_INTERNAL hal_LpsDeepSleep (BOOL die)
{
    //UINT32 previousMask; // To store the previous Irq mask
    //register UINT32 puCtrl = g_halLpsCtx.lpsPuCtrl;
    register UINT32 WUMask;
    BOOL isBTWakeup;
    BOOL isPllOn; //Remember pll status before sleep for revert pll after woke up

    isBTWakeup = hal_IsBtStart() && !hal_IsBtSleep();


    hal_RfspiInitQueue();
#ifdef GSM_SUPPORT	
    // Send Xcv to "real" sleep 
    rfd_XcvSleep(RFD_POWER_OFF);
#endif /*GSM_SUPPORT*/
    // Power down audio codec
    hal_AudDeepPowerDown();
    
    // prepare PM to go to LOW POWER MODE
    pmd_SetPowerMode(PMD_LOWPOWER);

    // Check if we must die
    if (die)
    {
        hwp_sysIrq->wakeup_mask = 0;
#ifdef GSM_SUPPORT
        hwp_bbIrq->wakeup_mask = 0;
#endif
        WUMask = 0;
    }
    else
    {
        // prepare wakup mask : all activated irq + force LPS to be there
        // + Debug uart in case of command coming to the host from 
        // trace tool.
        WUMask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_LPS| SYS_IRQ_SYS_IRQ_DEBUG_UART|SYS_IRQ_SYS_IRQ_BT;
        hwp_sysIrq->wakeup_mask = 0;
    }

    //Disable tcu
    hwp_tcu->LPS_PU_Ctrl &= ~TCU_LPS_PU_ENABLE;

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    //Enable TCU reg write clock to close XTAL 26M
    //hwp_sysCtrl->Clk_Sys_Enable2 = SYS_CTRL_ENABLE_SYS_TCU; //26M LP MODE disturb FM SINAD

#ifdef __FORCE_WORK_IN_26MHZ__LOCAL

    // Stop using VCO - Switch to 32K clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_OSCILLATOR
						   | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
#if (CHIP_HAS_ASYNC_TCU)
#ifdef USE_DIGITAL_CLK
                           | SYS_CTRL_DIGEN_H_ENABLE
#endif // USE_DIGITAL_CLK
#if (1)
                           | SYS_CTRL_BB_PLL_BYPASS_LOCK
                           | SYS_CTRL_PLL_BYPASS_LOCK
#endif
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock
#endif // CHIP_HAS_ASYNC_TCU
                           ;
#else //! __FORCE_WORK_IN_26MHZ__

#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    // PLL enable. Clk_sys switching from Dig_Clk to Xtal26M needs Pll enable.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;
    hal_TimDelay(1);
    //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));
    //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_BB_PLL_LOCKED));

    // Stop using PLLs - Switch to 26M clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock;
    hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ_26M | SYS_CTRL_FORCE_DIV_UPDATE;
    hwp_sysCtrl->Cfg_Clk_BB = SYS_CTRL_BB_FREQ_26M | SYS_CTRL_BB_FORCE_DIV_UPDATE;

    // Switch PLL to 26M.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_BYPASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;

    // Power down PLL.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_PLL_BYPASS_BYPASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_RESET;

    // Stop using VCO - Switch to 32K clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_OSCILLATOR
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock;

#else // !USE_DIG_CLK_ONLY_AT_IDLE

    // Stop using PLLs - Switch to 26M clock
    hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SLOW_SEL_RF_OSCILLATOR;
    hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
                           
#ifdef USE_DIGITAL_CLK
    hwp_sysCtrl->Cfg_Clk_Sys |= SYS_CTRL_FORCE_DIV_UPDATE;
    hwp_sysCtrl->Cfg_Clk_BB |= SYS_CTRL_BB_FORCE_DIV_UPDATE;
#endif //USE_DIGITAL_CLK

    // Insert delay
    VOLATILE UINT32 i;
    for (i=0; i<10; i++);

    // FIXME Implement clock out !

    isPllOn = ((hwp_sysCtrl->Pll_Ctrl & SYS_CTRL_PLL_ENABLE_ENABLE) != 0);

    if(isPllOn)
    {
        // stop PLL

        // Disable output.
        hwp_sysCtrl->Pll_Ctrl &= ~(
                                   SYS_CTRL_PLL_CLK_FAST_ENABLE_MASK
                                  );
        // Put Lock in Reset.
        hwp_sysCtrl->Pll_Ctrl &= ~(
                                   SYS_CTRL_PLL_LOCK_RESET_MASK
                                  ); 

#if (!CHIP_HAS_ASYNC_TCU)
        // FIXME what the point in waiting for deLock ?
        while(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED);
#endif

        // Power down PLL.
        hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                              | SYS_CTRL_PLL_BYPASS_PASS
                              | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                              | SYS_CTRL_PLL_LOCK_RESET_RESET
                              ;
    }
    
    if(!isBTWakeup)
    {
        // Stop using VCO - Switch to 32K clock
        hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SLOW_SEL_RF_OSCILLATOR;
    }

#endif // !USE_DIG_CLK_ONLY_AT_IDLE

#endif // __FORCE_WORK_IN_26MHZ__

    // TODO: power down relevant analog stuff in RDA IPs

    //Close 26M
    if(!isBTWakeup)
    {
        hwp_tcu->LPS_PU_Ctrl &=~TCU_LPS_PU_CLK_RF_ON;
    }

// ----------------------------------------------------------------------------
// Going in PMIC Low Power mode - No access to external RAM/FLASH Allowed
// ----------------------------------------------------------------------------

    // enter deeper low power
#if 0//(CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    hwp_gpio->gpo_set = g_halLpModeGpo;
#endif

    //if(bt_sleep_die_tst)
        //WUMask = 0;
    
    hwp_sysIrq->wakeup_mask = WUMask;

    // Send XCPU to sleep
    // Here we can call this function as it is static inline
    hal_SysXcpuSleep();
    //---------------------------------------------------------------------
    // HERE THE CPU SLEEPS 
    //---------------------------------------------------------------------

    // --------------------------------------------------------------------
    // An interrupt occurred which woke the CPU
    // --------------------------------------------------------------------
    //Add some nop to avoid crash after wake up in 5856
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }

    // exit deeper low power immediately to enable V_ANA on opal
    if(!isBTWakeup)
    {
        //open 26M
        hwp_tcu->LPS_PU_Ctrl |=TCU_LPS_PU_CLK_RF_ON;

        // switch to 26M
        hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SLOW_SEL_RF_OSCILLATOR;
        while((hwp_sysCtrl->Sel_Clock & SYS_CTRL_SLOW_SEL_RF_OSCILLATOR)); 
    }

    if(isPllOn)
    {
        // power on pll
        hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                              | SYS_CTRL_PLL_BYPASS_PASS
                              | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                              | SYS_CTRL_PLL_LOCK_RESET_NO_RESET;

       while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED)); 


        // Put Lock in NO Reset.?
        //hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_LOCK_RESET_MASK; 

        // Enable output.
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_MASK;


        // Switch to Fast clock
        hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SYS_SEL_FAST_SLOW;
    }

    //Disbale TCU write clock to avoid wrong access TCU
    hwp_sysCtrl->Clk_Sys_Disable2 = SYS_CTRL_DISABLE_SYS_TCU;

    // Insert delay
    //VOLATILE UINT32 i;
    for (i=0; i<10; i++);

    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    // -------------------------------------------------------------
    // The system clock is back to the normal frequency (fast clock)
    // -------------------------------------------------------------

    // PM go to IDLE POWER MODE
    pmd_SetPowerMode(PMD_IDLEPOWER);

    HAL_PROFILE_WINDOW_ENTER(XCV_WAKEUP);

    // ----------------------------------------------------------------------------
    // Getting out of PMIC Low Power mode - Access to External RAM/FLASh allowed again
    // ----------------------------------------------------------------------------

    // Return the irq cause, 0 is a normal LPS wake up
    return hwp_sysIrq->cause;
}
#if 0
BOOL hal_LpsNap_32k = FALSE;
/******************************************************************************
 * hal_LpsNap: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID hal_LpsNap (VOID)
{
    //BOOL isBTWakeup = TRUE;

    register UINT32 WUMask;

    // prepare wakup mask : all activated irq + force LPS to be there
    // + Debug uart in case of command coming to the host from 
    // trace tool.
    WUMask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_DEBUG_UART|SYS_IRQ_SYS_IRQ_BT;

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;


    if(hal_LpsNap_32k)
    {
        // Stop using VCO - Switch to 32K clock
        hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SLOW_SEL_RF_OSCILLATOR;
        hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
    }


    hwp_sysIrq->wakeup_mask = WUMask;

    // Send XCPU to sleep
    // Here we can call this function as it is static inline
    hal_SysXcpuSleep();
    //---------------------------------------------------------------------
    // HERE THE CPU SLEEPS 
    //---------------------------------------------------------------------

    // --------------------------------------------------------------------
    // An interrupt occurred which woke the CPU
    // --------------------------------------------------------------------
    //Add some nop to avoid crash after woke up in 5856
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }

    if(hal_LpsNap_32k)
    {
        // switch to 26M
        hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SLOW_SEL_RF_OSCILLATOR;
        while((hwp_sysCtrl->Sel_Clock & SYS_CTRL_SLOW_SEL_RF_OSCILLATOR)); 
    }
    
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;

}
#endif /* 0 */

#else /* (CHIP_ASIC_ID != CHIP_ASIC_ID_5856) */
/******************************************************************************
 * hal_LpsDeepSleep: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
UINT32 HAL_FUNC_INTERNAL hal_LpsDeepSleep (BOOL die)
{
    UINT32 previousMask; // To store the previous Irq mask
    register UINT32 puCtrl = g_halLpsCtx.lpsPuCtrl;
    register UINT32 WUMask;

#if (CHIP_HAS_ASYNC_TCU)
    UINT32 tcuClock;
    if (g_halSysTcuRunningAt26M)
    {
        tcuClock = SYS_CTRL_SYS_SEL_TCU_26M;
    }
    else
    {
        tcuClock = SYS_CTRL_SYS_SEL_TCU_13M;
    }
#endif

    hal_RfspiInitQueue();
#ifdef GSM_SUPPORT	
    // Send Xcv to "real" sleep 
    rfd_XcvSleep(RFD_POWER_OFF);
#endif /*GSM_SUPPORT*/
    // Power down audio codec
    hal_AudDeepPowerDown();

    // prepare PM to go to LOW POWER MODE
    pmd_SetPowerMode(PMD_LOWPOWER);

    // Check if we must die
    if (die)
    {
        hwp_sysIrq->wakeup_mask = 0;
        hwp_bbIrq->wakeup_mask = 0;
        WUMask = 0;
    }
    else
    {
        // prepare wakup mask : all activated irq + force LPS to be there
        // + Debug uart in case of command coming to the host from 
        // trace tool.
        WUMask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_LPS| SYS_IRQ_SYS_IRQ_DEBUG_UART;
        hwp_sysIrq->wakeup_mask = 0;
    }

    // clear LPS irq
    hwp_tcu->LPS_PU_IRQ = TCU_LPS_PU_IRQ_CAUSE_MASK;

    // Save previous LPS irq mask
    previousMask=hwp_tcu->LPS_PU_IRQ_Mask;

    // allow LPS irq from power up sequence to wake us up
    hwp_tcu->LPS_PU_IRQ_Mask = TCU_LPS_IRQ_PU_DONE_MASK | TCU_LPS_IRQ_PU_READY_MASK | TCU_LPS_IRQ_PU_REACHED_MASK;

    // enable the power up FSM
    // Keep CO0, CO1, RFCLK_26M_EN in their active state
    if (!die)
    {
        hwp_tcu->LPS_PU_Ctrl = puCtrl | TCU_LPS_PU_ENABLE | TCU_LPS_PU_SPI_ON |
                TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON;
    }
    else
    {
        hwp_tcu->LPS_PU_Ctrl = puCtrl | TCU_LPS_PU_SPI_ON |
                TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON;
    }


    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#ifdef __FORCE_WORK_IN_26MHZ__LOCAL

    // Stop using VCO - Switch to 32K clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_OSCILLATOR
						   | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
#if (CHIP_HAS_ASYNC_TCU)
#ifdef USE_DIGITAL_CLK
                           | SYS_CTRL_DIGEN_H_ENABLE
#endif // USE_DIGITAL_CLK
#if (1)
                           | SYS_CTRL_BB_PLL_BYPASS_LOCK
                           | SYS_CTRL_PLL_BYPASS_LOCK
#endif
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock
#endif // CHIP_HAS_ASYNC_TCU
                           ;
#else //! __FORCE_WORK_IN_26MHZ__

#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    // PLL enable. Clk_sys switching from Dig_Clk to Xtal26M needs Pll enable.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;
    hal_TimDelay(1);
    //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));
    //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_BB_PLL_LOCKED));

    // Stop using PLLs - Switch to 26M clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock;
    hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ_26M | SYS_CTRL_FORCE_DIV_UPDATE;
    hwp_sysCtrl->Cfg_Clk_BB = SYS_CTRL_BB_FREQ_26M | SYS_CTRL_BB_FORCE_DIV_UPDATE;

    // Switch PLL to 26M.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_BYPASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;

    // Power down PLL.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_PLL_BYPASS_BYPASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_RESET;

    // Stop using VCO - Switch to 32K clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_OSCILLATOR
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock;

#else // !USE_DIG_CLK_ONLY_AT_IDLE

    // Stop using PLLs - Switch to 26M clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
#if (CHIP_HAS_ASYNC_TCU)
                           | SYS_CTRL_BB_SEL_FAST_SLOW
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock
#endif
                           ;
#ifdef USE_DIGITAL_CLK
    hwp_sysCtrl->Cfg_Clk_Sys |= SYS_CTRL_FORCE_DIV_UPDATE;
    hwp_sysCtrl->Cfg_Clk_BB |= SYS_CTRL_BB_FORCE_DIV_UPDATE;
#endif //USE_DIGITAL_CLK

    // Insert delay
    VOLATILE UINT32 i;
    for (i=0; i<10; i++);

    // FIXME Implement clock out !

    // stop PLL

    // Disable output.
    hwp_sysCtrl->Pll_Ctrl &= ~(
                               SYS_CTRL_PLL_CLK_FAST_ENABLE_MASK
#if (CHIP_HAS_ASYNC_TCU)
                             | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_MASK
#endif
                              );
    // Put Lock in Reset.
    hwp_sysCtrl->Pll_Ctrl &= ~(
                               SYS_CTRL_PLL_LOCK_RESET_MASK
#if (CHIP_HAS_ASYNC_TCU)
                             | SYS_CTRL_BB_PLL_LOCK_RESET_MASK
#endif
                              ); 

#if (!CHIP_HAS_ASYNC_TCU)
    // FIXME what the point in waiting for deLock ?
    while(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED);
#endif

    // Power down PLL.
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_PLL_LOCK_RESET_RESET
#if (CHIP_HAS_ASYNC_TCU)
                          | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_RESET
#endif
                          ;

    // Stop using VCO - Switch to 32K clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_OSCILLATOR
                           | SYS_CTRL_SYS_SEL_FAST_SLOW
#if (CHIP_HAS_ASYNC_TCU)
#ifdef USE_DIGITAL_CLK
                           | SYS_CTRL_DIGEN_H_ENABLE
#endif // USE_DIGITAL_CLK
                           | SYS_CTRL_BB_SEL_FAST_SLOW
#if (1)
                           | SYS_CTRL_BB_PLL_BYPASS_LOCK
                           | SYS_CTRL_PLL_BYPASS_LOCK
#endif
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | tcuClock
#endif // CHIP_HAS_ASYNC_TCU
                           ;

#endif // !USE_DIG_CLK_ONLY_AT_IDLE

#endif // __FORCE_WORK_IN_26MHZ__

    // TODO: power down relevant analog stuff in RDA IPs

    // die == TRUE low power dead mode
    if (die)
    {
        // do not enable power on sequence
        hwp_tcu->LPS_PU_Ctrl = puCtrl;
#if (!CHIP_HAS_ASYNC_TCU)
        // Stop all clock generation, simulate infinite sleep time
        hwp_tcu->Cfg_Clk_Div &= ~(TCU_ENABLE_QBIT_ENABLED |TCU_ENABLE_DAI_SIMPLE_208K_ENABLED );
#else // (CHIP_HAS_ASYNC_TCU)
        // Stop TCU clock - Will not work on FPGA
        hwp_sysCtrl->Clk_Other_Disable=SYS_CTRL_DISABLE_CLK_OTHER(SYS_CTRL_DISABLE_OC_TCU);
#endif // (CHIP_HAS_ASYNC_TCU)
    }
    else
    {
        // Put CO0, CO1, RFCLK_26M_EN in their inactive state
        // At that stage, There is no more clock from the transceiver
        hwp_tcu->LPS_PU_Ctrl = puCtrl | TCU_LPS_PU_ENABLE;
    }


// ----------------------------------------------------------------------------
// Going in PMIC Low Power mode - No access to external RAM/FLASH Allowed
// ----------------------------------------------------------------------------

    // enter deeper low power
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
    hwp_gpio->gpo_set = g_halLpModeGpo;
#endif
    hwp_sysIrq->wakeup_mask = WUMask;

    // Send XCPU to sleep
    // Here we can call this function as it is static inline
    hal_SysXcpuSleep();
    //---------------------------------------------------------------------
    // HERE THE CPU SLEEPS 
    //---------------------------------------------------------------------

    // --------------------------------------------------------------------
    // An interrupt occurred which woke the CPU
    // --------------------------------------------------------------------
    // exit deeper low power immediately to enable V_ANA on opal
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
    hwp_gpio->gpo_clr = g_halLpModeGpo;
#endif
    // remove PU reached cause from mask
    hwp_tcu->LPS_PU_IRQ_Mask = TCU_LPS_IRQ_PU_DONE_MASK | TCU_LPS_IRQ_PU_READY_MASK;

    // here we only wait for the PU FSM other IRQ cause were checked previously
    hwp_sysIrq->wakeup_mask = SYS_IRQ_SYS_IRQ_LPS;

    // wait until we get PU Ready (PU FSM is also started by XCPU WakeUp mask)
    while(0 == (hwp_tcu->LPS_PU_IRQ & TCU_LPS_IRQ_PU_READY_CAUSE))
    {
        // Send XCPU to sleep
        // Here we can call this function as it is static inline
        hal_SysXcpuSleep();
        //---------------------------------------------------------------------
        // HERE THE CPU SLEEPS 
        //---------------------------------------------------------------------

        // --------------------------------------------------------------------
        // An interrupt occurred which woke the CPU
        // --------------------------------------------------------------------
    }

#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    // Switch to 26M
    //hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
    //                       | SYS_CTRL_SYS_SEL_FAST_SLOW
    //                       | SYS_CTRL_BB_SEL_FAST_SLOW
    //                       | SYS_CTRL_SYS_PLL_DISABLE_LPS
    //                       | SYS_CTRL_MPMC_SYS_SAME_ENABLE
    //                       | SYS_CTRL_DIGEN_H_ENABLE;
    // Switch to fast clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                           | SYS_CTRL_SYS_SEL_FAST_FAST
                           | SYS_CTRL_BB_SEL_FAST_FAST
                           | SYS_CTRL_SYS_PLL_DISABLE_LPS
                           | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                           | SYS_CTRL_DIGEN_H_ENABLE
                           | tcuClock;
    hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ_26M | SYS_CTRL_FORCE_DIV_UPDATE;
    hwp_sysCtrl->Cfg_Clk_BB = SYS_CTRL_BCPU_FREQ | SYS_CTRL_BB_FORCE_DIV_UPDATE;
#endif // USE_DIG_CLK_ONLY_AT_IDLE

    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    // -------------------------------------------------------------
    // The system clock is back to the normal frequency (fast clock)
    // -------------------------------------------------------------

    // PM go to IDLE POWER MODE
    pmd_SetPowerMode(PMD_IDLEPOWER);

    HAL_PROFILE_WINDOW_ENTER(XCV_WAKEUP);

    // ----------------------------------------------------------------------------
    // Getting out of PMIC Low Power mode - Access to External RAM/FLASh allowed again
    // ----------------------------------------------------------------------------

    // remove PU ready cause from mask
    hwp_tcu->LPS_PU_IRQ_Mask = TCU_LPS_IRQ_PU_DONE_MASK;
#ifdef GSM_SUPPORT
    HAL_PROFILE_WINDOW_ENTER(XCV_WAKEUP);

    // configure the transciever so the Xtal setting is correct
    // AFC setup is done here too
    rfd_XcvWakeUp();

    HAL_PROFILE_WINDOW_EXIT(XCV_WAKEUP);
#endif /*GSM_SUPPORT*/
    // Force CO0, CO1, RFCLK_26M_EN in their active state
    // keep the power up FSM enabled
    hwp_tcu->LPS_PU_Ctrl =  puCtrl | TCU_LPS_PU_ENABLE | TCU_LPS_PU_SPI_ON |
        TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON;

    // Clear LPS READY IRQs
    hwp_tcu->LPS_PU_IRQ = TCU_LPS_IRQ_PU_READY_CAUSE;
    

    // -------------------------------------------------------------
    // Here the system clock must be synchronized as we will let the TCU use it again
    // -------------------------------------------------------------
    // Tell the state machine to go in the Done state
    hwp_tcu->LPS_PU_Ctrl =  puCtrl | TCU_LPS_PU_ENABLE | TCU_LPS_PU_CPU_DONE | TCU_LPS_PU_SPI_ON |
        TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON;

    // wait until we get PU done
    while(0 == (hwp_tcu->LPS_PU_IRQ & TCU_LPS_IRQ_PU_DONE_CAUSE))
    {
        hal_SysXcpuSleep();
    }
    HAL_PROFILE_WINDOW_EXIT(XCV_WAKEUP);

#if (!CHIP_HAS_ASYNC_TCU)
    // With g_halLpsCtx.lpsWakeUpFrameNbr == 0 optimisation we skip the wake up fint
    // As this fint is only used to program events for the next frame we
    // don't need it to be really on exact time, but hal_LpsGetElapsedFrames()
    // must be correct during this fake fint.
    {
        // The order of the read is important 
        UINT16 elapsed = (UINT16)hwp_tcu->LPS_SF_Elapsed_Frames;
        UINT32 fastCount;

        // During TCU wakeup frame, check the restart date
        if (elapsed == g_halLpsCtx.lpsSkipFrameNumber - 1)
        {
            // LPS_SF_Sys_Count start reflecting the actual counter only one 32kHz clock after TCU_LPS_IRQ_PU_DONE_CAUSE.
            fastCount = hwp_tcu->LPS_SF_Sys_Count;
            
            // So pool until it moves.
            while(fastCount == hwp_tcu->LPS_SF_Sys_Count);

        }

        // Recheck elapsed frame in case it changed during pooling...
        elapsed = (UINT16)hwp_tcu->LPS_SF_Elapsed_Frames;

        // This first read gives information on the SW/HW synchroniozation.
        // if elapsed < g_halLpsCtx.lpsSkipFrameNumber - 1, then it was the 
        // case at PU time, so we are sure that wakeup fint will be generated
        if (elapsed == g_halLpsCtx.lpsSkipFrameNumber - 1)
        {
            // When elapsed == g_halLpsCtx.lpsSkipFrameNumber - 1
            // the situation is different depending on the position in the frame.
            // 
            // We define the 'risky zone' as the lapse of time starting 5 Qbit before
            // the LPS FINT, when the TCU would be started if the PU has finished before its border
            // and the LPS FINT, which is the border at which the frame counter is incremented.
            //
            // Several cases can occur :
            // 1. we detect being in the risky zone but the TCU is started
            // 2. we detect being in the risky zone but the TCU is not started
            // 3. we detect being out of the risky zone and the current frame is still g_halLpsCtx.lpsSkipFrameNumber - 1
            // 4. we detect being out of the risky zone and the current is already g_halLpsCtx.lpsSkipFrameNumber
            //
            // Any other case is not possible if we assume that he delay 
            // between the two read is inferior to the risky zone duration (5 Qb)
            
            fastCount = hwp_tcu->LPS_SF_Sys_Count;

            if (fastCount > hwp_tcu->LPS_SF_Restart_Time)
            {
                // Cases 1,2 : we detect being in the risky zone,
                // the current frame is g_halLpsCtx.lpsSkipFrameNumber - 1.
                // Read the TCU current value. 
                UINT32 tcuCurVal = hwp_tcu->Cur_Val;
  
                // The hardware is after the risky zone start, 
                // wait until it gets after the g_halLpsCtx.lpsSkipFrameNumber border
                while(elapsed < g_halLpsCtx.lpsSkipFrameNumber) 
                {
                    elapsed = (UINT16)hwp_tcu->LPS_SF_Elapsed_Frames;
                }

                // Read again the tcu current value :
                // if the case is 1, we would be polling quite a long time ( ~ 5Qb), 
                // so if the TCU is running, its counter would be moving.
                if (tcuCurVal != hwp_tcu->Cur_Val)
                {
                    // case 1 : do not generate a fake IT, 
                    // because the HW actually woke before the risky zone
                    // and the TCU was started in frame g_halLpsCtx.lpsSkipFrameNumber - 1
                    // so a real wake up FINT is would be generated
                }
                else
                {
                    // case 2 : generate a fake IT,
                    // because the TCU is not started whereas we are already in frame 
                    // g_halLpsCtx.lpsSkipFrameNumber
                    hal_FintUserHandler();
                }
            }
            else
            {
                // Cases 3,4 : we are out of the risky zone, 
                // but we do not know if we are too early or too late.
                if ((UINT16)hwp_tcu->LPS_SF_Elapsed_Frames < g_halLpsCtx.lpsSkipFrameNumber)
                {
                    // case 3 : we are too early, the TCU will be started on time
                    // in frame g_halLpsCtx.lpsSkipFrameNumber - 1, no fake IT
                }
                else
                {
                    // case 4 : we are too late, we actually woke in the risky zone
                    // we are already in frame g_halLpsCtx.lpsSkipFrameNumber
                    // and the counter already wrapped, so generate a fake IT
                    hal_FintUserHandler();
                }
            }
        }
        else if (elapsed == g_halLpsCtx.lpsSkipFrameNumber)
        {
            // general case, we woke on the LPS power up
            // generate a fake IT
            hal_FintUserHandler();
        }
    }
#endif

    // clear LPS irq for power up done
    hwp_tcu->LPS_PU_IRQ = TCU_LPS_IRQ_PU_DONE_CAUSE;

    // clear LPS irq for PU reached if not enabled in initial mask
    if (0 == (previousMask & TCU_LPS_IRQ_PU_REACHED_MASK))
    {
        hwp_tcu->LPS_PU_IRQ = TCU_LPS_IRQ_PU_REACHED_CAUSE;
    }

    // put back the normal mask for LPS IRQ -> calibration
    hwp_tcu->LPS_PU_IRQ_Mask=previousMask;

    // Return the irq cause, 0 is a normal LPS wake up
    return hwp_sysIrq->Cause;
}
#endif /* (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) */

// =============================================================================
// hal_LpsSetControlPin
// -----------------------------------------------------------------------------
/// Configure the pin used by lps to enter le deepest low power mode 
/// (ECO_MODE or LP_MODE pin)
/// @param lpModePin Must be a GPO or HAL_GPO_NONE for current implementation.
// =============================================================================
PUBLIC VOID hal_LpsSetControlPin(CONST HAL_APO_ID_T lpModePin)
{
    HAL_ASSERT((lpModePin.type == HAL_GPIO_TYPE_NONE) 
                || (lpModePin.type == HAL_GPIO_TYPE_O),
                "lpModePin musr be a GPO or NONE(%x)",lpModePin.gpoId)
    if (lpModePin.type == HAL_GPIO_TYPE_NONE)
    {
        g_halLpModeGpo = 0;
    }
    else
    {
        g_halLpModeGpo = 1 << lpModePin.id;
    }
}


// =============================================================================
// hal_LpsCOCtrl
// -----------------------------------------------------------------------------
/// Change the state of LPSCOs
/// @param puCtrl only the following fields are used:
///  - HAL_LPS_POWERUP_CTRL_T.puCO0ForceOn
///  - HAL_LPS_POWERUP_CTRL_T.puCO1ForceOn
///  - HAL_LPS_POWERUP_CTRL_T.puCO0Pol
///  - HAL_LPS_POWERUP_CTRL_T.puCO1Pol
///  .
// =============================================================================
PUBLIC VOID hal_LpsCOCtrl(HAL_LPS_POWERUP_CTRL_T puCtrl)
{
    union
    {
        UINT32 reg;
        HAL_LPS_POWERUP_CTRL_T bitfield;
    } puctrl;
    puctrl.bitfield=puCtrl;
    // must ensure unspecified fields are 0
    // also this function should allow changing only LPSCO pins
    puctrl.reg &= TCU_LPS_PU_CO_ON_MASK | TCU_LPS_PU_CO_POL_MASK;
    // modify LPSCO control bits
    g_halLpsCtx.lpsPuCtrl = (g_halLpsCtx.lpsPuCtrl & ~(TCU_LPS_PU_CO_ON_MASK | TCU_LPS_PU_CO_POL_MASK)) | puctrl.reg;
    // HAL_TRACE(HAL_DBG_TRC |TSTDOUT , 0, "lpsPuCtrl:0x%x" ,g_halLpsCtx.lpsPuCtrl);
    // set new pin state
    hwp_tcu->LPS_PU_Ctrl = g_halLpsCtx.lpsPuCtrl | TCU_LPS_PU_SPI_ON |
            TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON;
}

// ----------  IRQ functions ---------------- //

#if 0 // probably useless
VOID hal_LpsIrqSetMask(HAL_LPS_IRQ_STATUS_T *mask)
{
    union
    {
        UINT32 reg;
        HAL_LPS_IRQ_STATUS_T bitfield;
    } u;

    u.bitfield = *mask;
    hwp_tcu->LPS_PU_IRQ_Mask = u.reg; 
}

void hal_LpsIrqGetMask(HAL_LPS_IRQ_STATUS_T *mask)
{
    union
    {
        UINT32 reg;
        HAL_LPS_IRQ_STATUS_T bitfield;
    } u;
    
    u.reg = hwp_tcu->LPS_PU_IRQ_Mask;
    *mask = u.bitfield;
}
#endif



//-------------------------------------------------
// ------------ Private Irq functions -------------
//-------------------------------------------------
void hal_LpsIrqHandler(UINT8 interrupt_id)
{   
    union
    {
        UINT32 reg;
        HAL_LPS_IRQ_STATUS_T bitfield;
    } status;

    status.reg = hwp_tcu->LPS_PU_IRQ;
    // clear all
    hwp_tcu->LPS_PU_IRQ = status.reg;

    if (g_halLpsCtx.lpsWakeUpFrameNbr == 0)
    {
        // If LPS calibration is not ready we are not entering low power sleep
        // but low power calibration !!
        if ((g_halLpsCtx.lpsState == HAL_LPS_CALIBRATED)
            && (status.bitfield.lpsSlowRunning == 1))
        {
            hal_TcuSetWrap(LPS_TCU_DEFAULT_WRAP_COUNT);
        }
    }
}


