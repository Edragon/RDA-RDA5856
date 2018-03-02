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
//
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
#include "rf_if.h"
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
#include "hal_bt.h"

// External drivers
#include "rfd_xcv.h"
#include "pmd_m.h"
#include "sxr_tksd.h"
#include "sxr_tls.h"

#include "cos.h"
#include "string.h"

#if OS_KERNEL == FREERTOS
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#endif

#include "tgt_app_cfg.h"
#include "boot_sector_chip.h"
#include "boot.h"
#include "hal_overlay.h"
#include "boot_sector.h"
#include "hal_open.h"
#include "tgt_hal_cfg.h"
#include "sxr_tim.h"


extern PROTECTED VOID boot_save_context(VOID);
extern int Mcd_UsbHostDeInit(uint8 port);
extern VOID hal_musb_start_synops();
#ifndef DISABLE_USB_SUPPORT
extern PUBLIC BOOL hal_UsbIsPlugIn(VOID);
extern VOID hal_musb_start_synops(VOID);
#if (COMBINED_HOST !=0)
extern int Mcd_UsbHostDeInit(uint8 port);
#endif
#endif

//#undef HAL_FUNC_INTERNAL
//#define HAL_FUNC_INTERNAL
/// Default wrap value for the TCU for a GSM frame
#define LPS_TCU_DEFAULT_WRAP_COUNT      4999

/// Ticks of 1 GSM frame
#define HAL_LPS_TIME_ONE_GSM_FRAME      (HAL_TICK1S*60/13000 + 2)

#define HAL_LPS_BT_AUXTIMER_TO_OSTIMER

#define DEEPSLEEP_MIN_TICKS_WAKEUP_BY_AUXTIMER (5 MS_WAITING)
#define DEEPSLEEP_MIN_TICKS_WAKEUP_BY_RTCTIMER      (500 MS_WAITING)


/// Weather system has entered LPS sleep before. It is used to speed up booting process.
PUBLIC BOOL g_halLpsSleptBefore = FALSE;

PROTECTED HAL_LPS_CONTEXT_T g_halLpsCtx;

VOLATILE UINT32 HAL_UNCACHED_BSS g_halLpsForceNoSleep = 0;

/// put the variable for ECO MODE PIN in internal ram
PRIVATE UINT32 HAL_DATA_INTERNAL g_halLpModeGpo = 0;
unsigned int g_isBTWakeup;
// ============================================================================
// g_halSysLpsLock
// ----------------------------------------------------------------------------
/// Prevent the hal_SysSetRequestFreq function from changing the system
/// frequency. This lock is used by the LPS mechanism to keep a coherent
/// calibration.
// ============================================================================
PROTECTED UINT8 g_halSysLpsLock = HAL_SYS_LPS_LOCK_UNLOCKED;

extern INT32  g_current_module;
extern UINT8 gpio_led_on;

extern BOOL APP_Wakeup(void);
extern PUBLIC BOOL bal_GetSleepState(VOID);
extern INT32 DSM_ReadUserData(void);
extern void APP_ReadComPara(void);
extern void MUSIC_GetPara(void);
extern INT32 DSM_DevInit(void);
extern void SYSconfig_Trap_Initialise(void);

VOID hal_LpsRtcTimer2OsTimerSet(UINT32 ticks);

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
    switch (clk)
    {
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
PUBLIC VOID  hal_LpsCalibrate()
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

                    }
                    else
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
                }
                else
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

PUBLIC VOID  hal_LpsAllowSkipFrame(BOOL allow, UINT16 lastEvt)
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
    }
    else

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

void test_printf(void)
{
    //hal_HstSendEvent(BOOT_EVENT,0x99999999);
}

uint32 IrqData=0;
void test(void)
{
    HAL_TIM_RTC_TIME_T RtcTime;
    RtcTime.year = 16;
    RtcTime.month = 12;
    RtcTime.wDay = 2;
    RtcTime.day = 20;
    RtcTime.hour = 16;
    RtcTime.min = 55;
    RtcTime.sec = 15;

    hal_TimRtcSetTime(&RtcTime);
    RtcTime.sec = 20;
    //IrqData = hwp_sysIrq->mask_set;
    //hwp_sysIrq->mask_clear = 0xffffffff;
    hwp_sysIrq->sc = 1;
    //hwp_sysCtrl->wakeup_ctrl |= SYS_CTRL_FORCE_WAKEUP;

    hal_TimRtcSetAlarm(&RtcTime);
    hwp_calendar->cmd |=  CALENDAR_ALARM_ENABLE_SET;
    hal_TimRtcAlarmIrqSetHandler(test_printf);
    //hal_HstSendEvent(BOOT_EVENT,0x33334446);
}

VOID hal_LpsRtcTimer2OsTimerHandler(VOID)
{
    //hal_HstSendEvent(BOOT_EVENT,0x3333444f);
}

VOID hal_LpsRtcTimer2OsTimerSet(UINT32 ticks)
{
    HAL_TIM_RTC_TIME_T rtctime;
    UINT32 sleep_seconds;
    
    hal_TimRtcAlarmDisable();

    sleep_seconds = (ticks + (HAL_TICK1S / 2)) / HAL_TICK1S;
    sleep_seconds = MAX(sleep_seconds, 1);

    //hal_HstSendEvent(BOOT_EVENT,0x33334445);
    //hal_HstSendEvent(BOOT_EVENT,sleep_seconds);

    memset(&rtctime, 0, sizeof(rtctime));
    hal_TimRtcSetTime(&rtctime);

    rtctime.sec = (UINT8) sleep_seconds % 60;
    sleep_seconds /= 60;
    rtctime.min = (UINT8) sleep_seconds % 60;
    sleep_seconds /= 60;

    HAL_ASSERT(sleep_seconds< 2, "Too large %d(hours)", sleep_seconds);
    rtctime.hour = (UINT8) sleep_seconds;

    //IrqData = hwp_sysIrq->mask_set;
    //hwp_sysIrq->mask_clear = 0xffffffff;
    //hwp_sysIrq->sc = 1;
    //hwp_sysCtrl->wakeup_ctrl |= SYS_CTRL_FORCE_WAKEUP;
    //hwp_sysIrq->wakeup_mask |=  (1<<24);

    hal_TimRtcSetAlarm(&rtctime);
    //hwp_calendar->cmd |=  CALENDAR_ALARM_ENABLE_SET;
    hal_TimRtcAlarmEnable();
    hal_TimRtcAlarmIrqSetHandler(hal_LpsRtcTimer2OsTimerHandler);

    //hal_HstSendEvent(BOOT_EVENT,hwp_calendar->calendar_curval_h);
    //hal_HstSendEvent(BOOT_EVENT,hwp_calendar->calendar_curval_l);

}

//void  hal_LpsRealSleep(void)
void  HAL_FUNC_INTERNAL hal_LpsInLightSleep(void)
{
    uint8 i;
    hwp_pmuc->pwr_sys_ctrl &=  ~PMUC_PM_REG_MASK;

    hal_SysXcpuSleep();
    // --------------------------------------------------------------------
    // An interrupt occurred which woke the CPU
    // --------------------------------------------------------------------
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
}

PUBLIC VOID hal_LpsSleep(VOID)
{
    UINT32 cs;
    UINT32 resourceActive = 0;
    UINT32 i;
    UINT32 deepSleepWUCause = 0;
    BOOL lpsReady = FALSE;
    // System enters LPS sleep for at least once

#if defined(__AT_MOD_SPECIAL__)
    return; // no sleep for audio calib;
#endif

    if(!g_halLpsSleptBefore)
    {
        /* don't sleep before system sleep first */
        if(!bal_GetSleepState())
            return;
    }
    g_halLpsSleptBefore = TRUE;
    if (g_halLpsForceNoSleep != 0)
    {
        return; // no sleep for some tests
    }
    // Enter CS
    cs = hal_SysEnterSC();

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

    if ((resourceActive==0) && lpsReady && (g_current_module!=7)&& (g_current_module!=18))// fm and linein no deep sleep
    {
        HAL_PROFILE_WINDOW_ENTER(DEEP_SLEEP);
#if 0
#ifndef DISABLE_USB_SUPPORT
            Mcd_UsbHostDeInit(0);

            hwp_sysCtrl->per_module_en &= ~SYS_CTRL_USBC_EN ;
           hal_musb_start_synops();
#endif
 #endif

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

        // Enter SX scheduling critical section
        HAL_PROFILE_WINDOW_EXIT(DEEP_SLEEP);
    }
    else
    {
#if 1
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

            // Shut down XCPU clock
            HAL_PROFILE_WINDOW_ENTER(LIGHT_SLEEP);
            hwp_sysIrq->wakeup_mask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_DEBUG_UART;
            hal_LpsInLightSleep();
            HAL_PROFILE_WINDOW_EXIT(LIGHT_SLEEP);
#endif
        }
#else
        //hal_LpsNap();
#endif
    }
    // Update the GPADC Requested Frequency
    hal_AnaGpadcWakeUp();

    hal_SysExitSC(cs);
    //

    // call the callback (if any) only if we went through Deep Sleep
    // we do it here and not in hal_LpsDeepSleep to avoid beeing in critical
    // section and because we want GPADC back on
    // also added some trace useful in debugging low power
#if 0
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
#endif
    if(hwp_pmuc->pwr_sys_ctrl&PMUC_PM_REG_MASK)
    {
        //remove power_mode after wake up
        hwp_pmuc->pwr_sys_ctrl &=  ~PMUC_PM_REG_MASK;
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
    return TRUE;
}

BOOL hal_LpsFastBootReady(void)
{
    INT32 tickRemain=0;

    tickRemain = (INT32) hal_TimTickGetValSigned();

    //hal_HstSendEvent(BOOT_EVENT,0x33334447);
    //hal_HstSendEvent(BOOT_EVENT,tickRemain);
    //hal_HstSendEvent(BOOT_EVENT,gpio_led_on);
    //hal_HstSendEvent(BOOT_EVENT,hal_UsbIsPlugIn());

#ifdef HAL_LPS_BT_AUXTIMER_TO_OSTIMER
    //Not enter fast boot when timer is coming after 5 ms
    if(hal_IsBtStart())
    {
        if(tickRemain< DEEPSLEEP_MIN_TICKS_WAKEUP_BY_AUXTIMER)
            return FALSE;
    }
    else
#endif /* HAL_LPS_BT_AUXTIMER_TO_OSTIMER */
    {
        //Use RTC clock to reminder
        if(tickRemain < DEEPSLEEP_MIN_TICKS_WAKEUP_BY_RTCTIMER)
            return FALSE;
    }

    //Maybe reboot in fastboot mode when charge ac on
    if (hwp_pmuc->ana_status & PMUC_CHR_AC_ON)
        return FALSE;

    //Not enter fast boot when gpio led is on
    if(gpio_led_on != 0)
        return FALSE;

#ifndef DISABLE_USB_SUPPORT
    if(hal_UsbIsPlugIn())
        return FALSE;
#endif

    return TRUE;

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


// =============================================================================
// boot_Pm
// -----------------------------------------------------------------------------
/// set power mode
/// @param pmmode the number of power mode(we have four power modes: 0(normal),1,2,4(sleep))
// =============================================================================
void boot_Pm(PMD_POWERMODE_T pmmode)
{
    //uint32 data;

    g_fastEntry = BOOT_FASTENTRY_VALID_TAG;
    pmd_SetPowerMode(pmmode);
    hwp_pmuc->pwr_sys_ctrl |= PMUC_TAB_RST_BYPASS;
    //save context before sleep
    boot_save_context();

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    //Clear fastboot flag
    g_fastEntry = 0;
}


extern void BT_Wakeup_Configuration_step2(void);
extern void BT_Wakeup_Configuration_step1(void);
HAL_BTOVERLAY_ID_T g_halBTOverlayOwners;

PUBLIC VOID HAL_FUNC_INTERNAL hal_SysXcpuSleep(VOID)
{
    hwp_sysIrq->cpu_sleep = SYS_IRQ_SLEEP;
    asm("nop");

    //when mips16, "sw al, 28(v0)" can't located at 0xXXXX_XXX8.
    //when mips32, "sw al, 28(v0)" can't located at 0xXXXX_XXX0.

    // flush the write buffer to ensure we sleep before exiting this function
    UINT32 clkSysDisable __attribute__((unused)) = hwp_sysIrq->cpu_sleep;
}


void HAL_FUNC_INTERNAL hal_LpsIn32KDeepSleep(void)
{
     uint8 i;
    //hal_HstSendEvent(BOOT_EVENT,0x3333444d);
    //hal_HstSendEvent(BOOT_EVENT,hwp_pmuc->pwr_sys_ctrl );
    pmd_SetPowerMode(0);
    
    hwp_rf_if->adpll_setting1 &= ~RF_IF_SYS_FAST_CLK_SEL_MASK;
    hwp_rf_if->adpll_setting1 &= ~RF_IF_SYS_CLK_SEL_FAST;
   
    hal_SysXcpuSleep();
    
    // --------------------------------------------------------------------
    // An interrupt occurred which woke the CPU
    // --------------------------------------------------------------------
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  
    hwp_rf_if->adpll_setting1 |= RF_IF_SYS_CLK_SEL_FAST | RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;
    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    
  //hal_HstSendEvent(BOOT_EVENT,hwp_sysIrq->cause);

}
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
extern void SYSpwr_Reset_Sleep_Time(void);
extern unsigned int Get_Frames_To_Sleep_Pending(void);
#endif

void HAL_FUNC_INTERNAL hal_LpsInDeepSleep(void)
{
    UINT32* dst;
    UINT32 irqmaskset, per_module_en, remain_ticks, wakeup_cause;
    UINT32 sleep_ticks;
    UINT32 bt_native_timestampe = 0;
    UINT32 spiflash_clk_cfg, spiflash_cfg;
    UINT32 wakeup_init_time = 0;
    BOOL isBTStart = hal_IsBtStart();
    UINT32 cs;

    hwp_pmuc->pmu_ana_rsvd = 0x7;

    //save mask set before sleep
    irqmaskset = hwp_sysIrq->mask_set;
    per_module_en = hwp_sysCtrl->per_module_en;
    spiflash_clk_cfg = hwp_sysCtrl->spiflash_clk_cfg;
    spiflash_cfg = hwp_spiflash->flash_config;

    remain_ticks = hal_TimTickGetValSigned();
    hal_TickPreTampax();
    //hal_HstSendEvent(BOOT_EVENT,0x33334444);
    //hal_HstSendEvent(BOOT_EVENT,remain_ticks);
    if(isBTStart)
    {
        SYSpwr_Reset_Sleep_Time();
    }

#ifdef HAL_LPS_BT_AUXTIMER_TO_OSTIMER
    if(isBTStart)
    {
        bt_native_timestampe = *(UINT32*)(0XA170001C); //BTtimer_Get_Native_Clock();
    }
    else 
#endif /* HAL_LPS_BT_AUXTIMER_TO_OSTIMER */
    {
        hal_LpsRtcTimer2OsTimerSet(remain_ticks);
    }


    hwp_pmuc->xtal_cfg_ctrl |= PMUC_XTAL_LP_MODE_EN_REG;
    hwp_pmuc->ldo_cfg &= ~PMUC_PU_VIO;//cut Vio before sleep to disable LED after wake up
    //set low power mode and entry fastboot sleep
    boot_Pm(PMD_LOWPOWER1);
    
    
#if 0 // if some module use Vio, please open this block, and then set gpio1&gpio2 as output function!!!!   
    hwp_iomux->cfg[1] |= IOMUX_PULL_FORCE|IOMUX_OEN_REG|IOMUX_OEN_FORCE;
    hwp_iomux->cfg[2] |= IOMUX_PULL_FORCE|IOMUX_OEN_REG|IOMUX_OEN_FORCE;
    hwp_pmuc->ldo_cfg |= PMUC_PU_VIO;
#endif    

    //after wake up we need several initial procedure
    //enable timer and debug host
    hwp_sysCtrl->per_module_en = per_module_en;
    hwp_sysCtrl->wakeup_ctrl |= SYS_CTRL_FORCE_WAKEUP; //Fix calendar value invaild after wakeup

    //restore mask set after wake up
    //if(Get_Frames_To_Sleep_Pending())
		//irqmaskset &=  0xffffc10;// if BT still need sleep then only AUX irq enabled
    hwp_sysIrq->mask_set = irqmaskset;
    wakeup_cause = hwp_sysIrq->cause;

    wakeup_init_time = hal_TimGetUpTime(); //BTtimer_Get_Native_Clock();
#if 1
    //Reenable irq in ram for debug exception in Deepsleep
    cs = hal_SysEnterSC();

    asm volatile(".set noat\n\t"
    "li    $at,    (0xff01)\n\t"
                 //"mtc0  $0, $13\n\t"
                 //"nop\n\t"
                 "mtc0  $at, $12\n\t"
                 "addi  $k1, $k1,1\n\t"
                 ".set at\n\t"
                 );
#endif

    if(isBTStart)
    {
    	BT_Wakeup_Configuration_step1();
    }


    
    // Clear BSS in Dualport SRAM.
    for (dst = (UINT32*) &_dualport_bss_start+4096;//big buffer no need to clear
            dst < (UINT32*) &_dualport_bss_end;
            dst++)
    {
        *dst = 0;
    }

    //set up PLL
    hal_SysSetupPLL();

    //reinit spi flash cfg after wake up
    hwp_sysCtrl->spiflash_clk_cfg = spiflash_clk_cfg;
    hwp_spiflash->flash_config = spiflash_cfg;


    //enable keysense
    hwp_pmu_intf->keysense_ctrl1 |=PMU_INTF_KEY_ENABLE;
    //clear mask
    hwp_pmu_intf->pmu_intf_imr = 0x1c;

    //module initial configuration
    hal_Open(tgt_GetHalConfig());

    if(g_halBTOverlayOwners==HAL_BTOVERLAY_FILESYSTEM)  
    {
        //boot_InitFlashDriver();
        //music parameter initial
        //DSM_ReadUserData();
        //APP_ReadComPara();
#if APP_SUPPORT_MUSIC==1
        //MUSIC_GetPara(); 
#endif
        //DSM_DevInit();
    }

    if((wakeup_cause & 0X1FF) && isBTStart)
    {
        //wake up by bt irq, raise sys clock to 52MHz
        //hal_SysSetupSystemClock(HAL_SYS_FREQ_52M);
    }

    hal_SysPMUIntfXtalCtrl(1);     //BT ADC en    
    if(isBTStart)
    {
        SYSconfig_Trap_Initialise();//modify trap after sleep
    }

#ifndef DISABLE_USB_SUPPORT
    hal_musb_start_synops();
#endif
    if(isBTStart)
    {
     	BT_Wakeup_Configuration_step2();
    }


#ifdef HAL_LPS_BT_AUXTIMER_TO_OSTIMER
    if(isBTStart)
    {
        bt_native_timestampe = *(UINT32*)(0XA170001C) - bt_native_timestampe;
        //convert to 1.6KHz clock reportedPeriod = reportedPeriod *16384 /3200;
        sleep_ticks = bt_native_timestampe *128 /25;

        sleep_ticks = MAX(DEEPSLEEP_MIN_TICKS_WAKEUP_BY_AUXTIMER, sleep_ticks);
    }
    else
#endif /* HAL_LPS_BT_AUXTIMER_TO_OSTIMER */
    {
        HAL_TIM_RTC_TIME_T t;
        if(HAL_ERR_NO == hal_TimRtcGetTime(&t))
        {
            sleep_ticks = (t.sec + t.min * 60 + t.hour * 3600) * HAL_TICK1S;
        }
        else
        {
            sleep_ticks = 0;
        }
    //hal_HstSendEvent(BOOT_EVENT,0x33334446);
    //hal_HstSendEvent(BOOT_EVENT,hwp_calendar->status );
    //hal_HstSendEvent(BOOT_EVENT,hwp_calendar->calendar_curval_h);
    //hal_HstSendEvent(BOOT_EVENT,hwp_calendar->calendar_curval_l);

        sleep_ticks = MAX(DEEPSLEEP_MIN_TICKS_WAKEUP_BY_RTCTIMER, sleep_ticks);
    }

    //hal_HstSendEvent(BOOT_EVENT,0x33334449);
    //hal_HstSendEvent(BOOT_EVENT,wakeup_cause);
    //hal_HstSendEvent(BOOT_EVENT,sleep_ticks);

    if(remain_ticks < (sleep_ticks>>1))
    {
        //Print warning if sleep ticks large than twice of remain ticks
        hal_HstSendEvent(SYS_EVENT,0x20170799);
    }
    
    sxr_Tampax(sleep_ticks, 0);
   

    wakeup_init_time = hal_TimGetUpTime() - wakeup_init_time;
    if(wakeup_init_time > 10 MS_WAITING)
    {
        hal_HstSendEvent(SYS_EVENT,0x201707BB);
        hal_HstSendEvent(SYS_EVENT,wakeup_init_time);
    }

#if 1
    asm volatile(".set noat\n\t"
    "li    $at,    (0xff00)\n\t"
                 //"mtc0  $0, $13\n\t"
                 //"nop\n\t"
                 "mtc0  $at, $12\n\t"
                 "addi  $k1, $k1,-1\n\t"
                 ".set at\n\t"
                 );
    hal_SysExitSC(cs);
#endif


}

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
UINT32  hal_LpsDeepSleep (BOOL die)
{
    register UINT32 WUMask;
    BOOL isBTWakeup;
    BOOL isPllOn; //Remember pll status before sleep for revert pll after woke up
    UINT32 RFIFCtrl;
    isBTWakeup = hal_IsBtStart() && !hal_IsBtSleep();

    RFIFCtrl = hal_SysEnterRFIFCtrlAPB();
    //hal_HstSendEvent(BOOT_EVENT,0x33334446);

    //hal_RfspiInitQueue();
#ifdef GSM_SUPPORT
    // Send Xcv to "real" sleep
    rfd_XcvSleep(RFD_POWER_OFF);
#endif /*GSM_SUPPORT*/
    // Power down audio codec
    hal_AudDeepPowerDown();

    // prepare PM to go to LOW POWER MODE
    //pmd_SetPowerMode(PMD_LOWPOWER);

    // Check if we must die
    if (die)
    {
        hwp_sysIrq->wakeup_mask = 0;
        WUMask = 0;
    }
    else
    {
        // prepare wakup mask : all activated irq + force LPS to be there
        // + Debug uart in case of command coming to the host from
        // trace tool.
        WUMask = hwp_sysIrq->mask_set | SYS_IRQ_SYS_IRQ_DEBUG_UART;
        hwp_sysIrq->wakeup_mask = 0;
    }
    if(isPllOn)
    {

    }

    if(!isBTWakeup)
    {

    }
	
    #ifdef CT_NO_DEBUG
    hal_TimWatchDogClose();  //Disable Watchdog in debug mode
    #endif
	
    hwp_sysIrq->wakeup_mask = WUMask;

    if(hal_LpsFastBootReady())
    {
        hal_LpsInDeepSleep();
    }
    else
    {
        hal_LpsIn32KDeepSleep();
    }
    // exit deeper low power immediately to enable V_ANA on opal

    if(!isBTWakeup)
    {

    }

    if(isPllOn)
    {

    }


    hwp_pmuc->pwr_sys_ctrl &= ~PMUC_TAB_RST_BYPASS;

    //hal_HstSendEvent(BOOT_EVENT,0x33334448);
    #ifdef CT_NO_DEBUG
    hal_TimWatchDogOpen(30 SECOND);
    #endif

    // PM go to IDLE POWER MODE
    //pmd_SetPowerMode(PMD_IDLEPOWER);

    HAL_PROFILE_WINDOW_ENTER(XCV_WAKEUP);

    hal_SysExitRFIFCtrlAPB(RFIFCtrl);

    // Return the irq cause, 0 is a normal LPS wake up
    return hwp_sysIrq->cause;
}


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


