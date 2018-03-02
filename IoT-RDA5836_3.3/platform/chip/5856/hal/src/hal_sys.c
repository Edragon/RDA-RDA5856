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
/// @file hal_sys.c
/// Contains the SysCtrl driver implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"

#include "tgt_hal_cfg.h"
#include "string.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "tcu.h"

#if (CHIP_HAS_ASYNC_BCPU)
#include "bb_ctrl.h"
#endif

#include "cfg_regs.h"
#include "mem_bridge.h"
#include "calendar.h"
#include "sys_irq.h"

#include "halp_lps.h"
#include "halp_config.h"
#include "halp_sys.h"
#include "halp_ebc.h"
#include "halp_host.h"
#include "halp_camera.h"
#include "halp_gdb_stub.h"

#include "halp_debug.h"

#include "hal_debug.h"
#include "hal_mem_map.h"
#include "hal_sys.h"
#include "hal_lps.h"
#include "hal_host.h" // this is to send debug events
#include "debug_port.h"
#include "boot.h"

#if (CHIP_HAS_USB == 1)
#include "halp_usb_monitor.h"
#include "halp_usb.h"
#include "boot_usb_monitor.h"
#endif

#include "pmd_m.h"
#include "memd_m.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
#include "rfd_xcv.h"
#endif

extern BOOL Codec_Need_Audpll_flag;

// =============================================================================
//  MACROS
// =============================================================================
#ifdef FPGA
#define HAL_SYS_MAX_SPEED   HAL_SYS_FREQ_52M
#else
#define HAL_SYS_MAX_SPEED   CHIP_MAX_SPEED
#endif
#if 0//(CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
#define SYS_CTRL_BCPU_FREQ  SYS_CTRL_BB_FREQ_26M
#else
#define SYS_CTRL_BCPU_FREQ  SYS_CTRL_BB_FREQ_39M
#endif
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
#undef SYS_CTRL_BCPU_FREQ
#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
#define SYS_CTRL_BCPU_FREQ  SYS_CTRL_BB_FREQ_52M
#else
#define SYS_CTRL_BCPU_FREQ  SYS_CTRL_BB_FREQ_26M
#endif
#endif
// Don't limit the clock if we use PXTS on RAM or TRACE.
#if (defined(PXTS_MAX_SUPPORTED_FREQUENCY) && !defined(HAL_PROFILE_NOT_ON_ROMULATOR))
#if (PXTS_MAX_SUPPORTED_FREQUENCY < HAL_SYS_MAX_SPEED)
#undef HAL_SYS_MAX_SPEED
#define HAL_SYS_MAX_SPEED PXTS_MAX_SUPPORTED_FREQUENCY
#endif
#endif

/// extern HAL_SYS_CLOCK_OUT_FREQ_T enum
#define HAL_SYS_CLOCK_OUT_OFF 0

#define SYS_SHUTDOWN_MAGIC_NUM 0xa1b2c3fe


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE UINT32 HAL_DATA_BACKUP g_halSysStateShutdownFlag;

#if (CHIP_HAS_ASYNC_TCU)
// ============================================================================
// g_halSysTcuRunningAt26M
// ----------------------------------------------------------------------------
///  Whether TCU is running at 26M
// ============================================================================
PROTECTED BOOL HAL_BOOT_DATA_INTERNAL g_halSysTcuRunningAt26M = FALSE;
#endif

// =============================================================================
// g_halSysFreqScalRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the frequency required by each frequency scaling
/// user, one per byte.
// =============================================================================
PROTECTED UINT32 g_halSysFreqScalRegistry[HAL_SYS_FREQ_MAX_RESOURCE_NB/4];

// =============================================================================
// g_halSysFreqHandlerRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the user handlers to be called at frequency 
/// change.
// =============================================================================
PRIVATE HAL_SYS_FREQ_CHANGE_HANDLER_T g_halSysFreqHandlerRegistry[HAL_SYS_TOTAL_USERS_QTY];

// ============================================================================
// g_halSysSystemFreq
// ----------------------------------------------------------------------------
/// Global var to have a view of the system frequency.
/// At boot, we are first on the 26Mhz Rf clock.
// ============================================================================
PROTECTED HAL_SYS_FREQ_T g_halSysSystemFreq = HAL_SYS_FREQ_26M;

#if (CHIP_HAS_USB == 1)
PRIVATE BOOL g_halUsbHostEnable = FALSE;
#endif /* CHIP_HAS_USB */

// ============================================================================
// g_halNumberOfUserPerFreq
// ----------------------------------------------------------------------------
/// This array is used to calculate the minimum system frequency to set
/// (that is the maximum required by a module among all the module) in a 
/// constant time (relative to the number of users).
/// 
// ============================================================================
PROTECTED UINT8 g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_QTY];

// =============================================================================
// g_halSysClockOutRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the frequency required by each users for clock out.
// =============================================================================
PROTECTED UINT32 g_halSysClockOutRegistry[HAL_SYS_CLOCK_OUT_ID_QTY] = {0};

PRIVATE UINT32 g_halSysAuxClkUserNum = 0;

PRIVATE UINT32 g_halSys32kClkUserNum = 0;

#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// g_halSysClockSwitchQBitError
// -----------------------------------------------------------------------------
/// To compensate error. Each sys clock switch induces a pll clock error on TCU qbit
//  The value counts in clocks at 312 Mhz (1 QBit = 288 clocks)
// =============================================================================
PRIVATE INT32 g_halSysClockSwitchQBitError = 0;

// =============================================================================
// g_halSysClockSwitchErrorTable312
// -----------------------------------------------------------------------------
/// Clock switch error compensation table (in 312 MHz clocks) when the PLL clock is 312 Mhz
// =============================================================================
CONST INT8 g_halSysClockSwitchQBitErrorCompensate312[6][6] =
{  //  26    39    52    78   104  156
    {   0 ,   8 ,   0 ,   8 ,  18,  20}, //   26 
    {  -4 ,   0 ,   4 ,   8 ,  14,  16}, //   39 
    {  -6 ,  -4 ,   0 ,   2 ,   6, 8}, //   52 
    {  -8 ,  -8 ,   0 ,   0 ,   2,  4 }, //   78 
    {  -9 , -10 ,   0 ,  -1 ,  0,  2 },  //  104 
    {  -12 , -12 ,   0 ,  -1 , -2, 0 }  //  156     
};
#endif // (!CHIP_HAS_ASYNC_TCU)

// =============================================================================
// g_halSysFreqSteps
// -----------------------------------------------------------------------------
// Frequency switching steps table
// =============================================================================
PRIVATE CONST HAL_SYS_FREQ_T g_halSysFreqSteps[] =
{
    HAL_SYS_FREQ_52M,
    HAL_SYS_FREQ_104M,
    HAL_SYS_FREQ_156M,
    HAL_SYS_FREQ_208M,
#if (CHIP_HAS_ASYNC_TCU)
    HAL_SYS_FREQ_250M,
    HAL_SYS_FREQ_312M,
#endif
};


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_SysCallFreqChangeHandlers
// -----------------------------------------------------------------------------
/// Call all the registered frequency change handlers for the given frequency.
/// @param sysFreq target frequency of the current change.
// =============================================================================
PRIVATE VOID hal_SysCallFreqChangeHandlers(HAL_SYS_FREQ_T sysFreq);

// =============================================================================
// hal_SysChangeSystemClock
// -----------------------------------------------------------------------------
/// Change the system clock (previously started and configured) to a (possibly)
/// new value
/// @param fastClockSetup New System clock applied to the system.
// =============================================================================
PRIVATE VOID hal_SysChangeSystemClock(HAL_SYS_FREQ_T fastClockSetup);

#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)&&defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
extern VOID Boot_AudEnableCodecAuxPll(VOID);
#endif
extern PUBLIC VOID hal_AudEnableCodecAuxPll(BOOL on);
extern VOID hal_abbPdAll(BOOL on);
extern VOID hal_abbEnablePa(BOOL on);

// =============================================================================
// hal_SysStartBcpu
// -----------------------------------------------------------------------------
/// Start the BCPU with a stack to at the given address.
/// @param bcpuMain Main function to run on the BCPU.
/// @param stackStartAddr Stack top for the program running on the BCPU
// =============================================================================
#ifdef GSM_SUPPORT
PUBLIC VOID hal_SysStartBcpu(VOID* bcpuMain, VOID* stackStartAddr)
{
#ifdef CHIP_HAS_BCPU_ROM
#ifdef REWRITE_ROM_AT_BOOT
#ifdef FPGA
    // Copy data from flash space to internal ROM.
    // This must be done before starting BCPU, as the addresses of
    // bcpu_stack_base and bcpu_main_entry might have been changed.

    UINT32 *src, *dst;
    // Unlock.
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;

    for (src = (UINT32*) &_bcpu_rom_flash_start_location,
             dst = (UINT32*) &_bcpu_rom_start;
         src < (UINT32*) &_bcpu_rom_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // Lock.
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif // FPGA
#endif // REWRITE_ROM_AT_BOOT
#endif // CHIP_HAS_BCPU_ROM

    UINT32 status = hal_SysEnterCriticalSection();
    bcpu_stack_base = (UINT32) stackStartAddr;
    bcpu_main_entry = (UINT32*) bcpuMain;
#if (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_BB_Enable = SYS_CTRL_ENABLE_BB_BCPU;
    hwp_sysCtrl->BB_Rst_Clr = SYS_CTRL_CLR_RST_BCPU;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#else // (CHIP_HAS_ASYNC_BCPU)
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_UNLOCK;
    hwp_bbCtrl->Clk_BB_Enable = BB_CTRL_ENABLE_BB_BCPU;
    hwp_bbCtrl->BB_Rst_Clr = BB_CTRL_CLR_RST_BCPU;
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_LOCK;
#endif // (!CHIP_HAS_ASYNC_BCPU)
    hal_SysExitCriticalSection(status);
}


// =============================================================================
// hal_SysStopBcpu
// -----------------------------------------------------------------------------
/// Stop the BCPU
// =============================================================================
PUBLIC VOID hal_SysStopBcpu(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
#if (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->BB_Rst_Set = SYS_CTRL_SET_RST_BCPU;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#else // (CHIP_HAS_ASYNC_BCPU)
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_UNLOCK;
    hwp_bbCtrl->BB_Rst_Set = BB_CTRL_SET_RST_BCPU;
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_LOCK;
#endif // (!CHIP_HAS_ASYNC_BCPU)
    hal_SysExitCriticalSection(status);
}
#endif

// =============================================================================
// hal_SysSetRequestFreq
// -----------------------------------------------------------------------------
/// When a given users requires a minimum frequency to be able to execute a
/// task, this function must be called.
/// @param userId Identifier of the calling user.
/// @param minFreq Minimum required frequency.
/// @param handler User function registered for this userId 
/// The function is called when the frequency is switched by any following call
/// to hal_SysSetRequestFreq() including the current one.
/// If \c NULL, no function is registered.
// =============================================================================
PUBLIC VOID hal_SysRequestFreq(HAL_SYS_FREQ_USER_ID_T userId, HAL_SYS_FREQ_T minFreq,
                                HAL_SYS_FREQ_CHANGE_HANDLER_T handler)
{
    UINT32 status;
    UINT32 requiredFreq = minFreq;
//    UINT32 callerAddr;

//    SXS_GET_RA(&callerAddr);

    // Frequency clamping
    if (requiredFreq > HAL_SYS_MAX_SPEED)
    {
        HAL_TRACE(HAL_LPS_TRC, 0, "hal_SysRequestFreq clamped the request for a %d frequency. %d", requiredFreq, HAL_SYS_MAX_SPEED);
        requiredFreq = HAL_SYS_MAX_SPEED;
    }

    HAL_TRACE(HAL_LPS_TRC,0,"hal_SysRequestFreq %d %d from 0x%lx",userId,minFreq,handler); 

    // Keep a coherent table
    status = hal_SysEnterCriticalSection();

    // Convert freq and set in the registry holding the frequency
    // required by each module
    hal_SysFreqScalSet(userId, hal_SysSF2FSF(requiredFreq));

    g_halSysFreqHandlerRegistry[userId] = handler;

    hal_SysUpdateSystemFrequency();
    
    hal_SysExitCriticalSection(status);
}

// =============================================================================
// hal_SysCallFreqChangeHandlers
// -----------------------------------------------------------------------------
/// Call all the registered frequency change handlers for the given frequency.
/// @param sysFreq target frequency of the current change.
// =============================================================================
PRIVATE VOID hal_SysCallFreqChangeHandlers(HAL_SYS_FREQ_T sysFreq)
{
    UINT32 i;
    for(i=0; i<HAL_SYS_TOTAL_USERS_QTY; i++)
    {
        if(g_halSysFreqHandlerRegistry[i])
        {
            g_halSysFreqHandlerRegistry[i](sysFreq);
        }
    }
}


// =============================================================================
// hal_SysUpdateSystemFrequency
// -----------------------------------------------------------------------------
/// Set the system frequency to the highest minimal frequency required 
/// by the user of the system.
// =============================================================================
PROTECTED VOID hal_SysUpdateSystemFrequency(VOID)
{
    UINT32 i;
    HAL_SYS_FREQ_T newSysFreq;

    // Look for the min to meet all requirement (ie the biggest)
    i = HAL_SYS_FREQ_SCAL_QTY-1;

    // Check i>0 to avoid problems when running into here while
    // hal_SysSetupSystemClock() has not been called yet.
    // E.g., crash dump in hal_Open() -- pal_SleepSerial().
    while (g_halNumberOfUserPerFreq[i]==0 && i>0)
    {
        i--;
    }

    // i hold the highest frequency which is required by a least
    // on user, in the FSF (Frequency scaling Fequency format)
    newSysFreq = hal_SysFSF2SF(i);

    // Workaround for 8808 and later chips
#if (CHIP_HAS_ASYNC_TCU)
    if (newSysFreq <= HAL_SYS_FREQ_26M)
    {
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
        // XCPU and BCPU cannot run at 26M due to TCU limitation
        newSysFreq = HAL_SYS_FREQ_39M;
#endif
    }
#endif
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
	newSysFreq = HAL_SYS_FREQ_52M;
#else
	newSysFreq = HAL_SYS_FREQ_26M;
#endif
#endif
    // reconvert freq and apply change
    hal_SysChangeSystemClock(newSysFreq);
}



// =============================================================================
// hal_SysSF2FSF
// -----------------------------------------------------------------------------
/// Convert a System Frequency into a Frequency Scaling Frequency
/// @todo Speed could be improved by ordering according to the the more
/// used HAL_SYS_FREQ_xxx values.
// =============================================================================
PROTECTED HAL_SYS_FREQ_SCAL_FREQ_T hal_SysSF2FSF(HAL_SYS_FREQ_T freqSetup)
{
    HAL_SYS_FREQ_SCAL_FREQ_T convValue = 0;
    // The freq parameter holds the required frequency
    // at the beginning, so that we can adujst dependending 
    // on the system limitation.
    HAL_SYS_FREQ_T freq = freqSetup;

    switch (freq)
    {
        case  HAL_SYS_FREQ_32K:
            convValue =  HAL_SYS_FREQ_SCAL_32K;
            break;

        case HAL_SYS_FREQ_13M:
            convValue =  HAL_SYS_FREQ_SCAL_13M;
            break;

        case HAL_SYS_FREQ_26M:
            convValue =  HAL_SYS_FREQ_SCAL_26M;
            break;

        case HAL_SYS_FREQ_39M:
            convValue =  HAL_SYS_FREQ_SCAL_39M;
            break;

        case HAL_SYS_FREQ_52M:
            convValue =  HAL_SYS_FREQ_SCAL_52M;
            break;

        case HAL_SYS_FREQ_78M:
            convValue =  HAL_SYS_FREQ_SCAL_78M;
            break;

        case HAL_SYS_FREQ_104M:
            convValue =  HAL_SYS_FREQ_SCAL_104M;
            break;

        case HAL_SYS_FREQ_156M:
            convValue =  HAL_SYS_FREQ_SCAL_156M;
            break;
#if (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_FREQ_208M:
            convValue =  HAL_SYS_FREQ_SCAL_208M;
            break;

        case HAL_SYS_FREQ_250M:
            convValue =  HAL_SYS_FREQ_SCAL_250M;
            break;

        case HAL_SYS_FREQ_312M:
            convValue =  HAL_SYS_FREQ_SCAL_312M;
            break;
#endif
        default:
            HAL_ASSERT(FALSE, "Improper system frequency: %d", freq);
            break;
    }

    return convValue;
}



// =============================================================================
// hal_SysFSF2SF
// -----------------------------------------------------------------------------
/// Convert a Frequency Scaling Frequency into a System Frequency
// =============================================================================
PROTECTED HAL_SYS_FREQ_T hal_SysFSF2SF(HAL_SYS_FREQ_SCAL_FREQ_T fsfFreq)
{
    HAL_SYS_FREQ_T convValue = 0;

    switch (fsfFreq)
    {
        case  HAL_SYS_FREQ_SCAL_32K:
            convValue =  HAL_SYS_FREQ_32K;
            break;

        case HAL_SYS_FREQ_SCAL_13M:
            convValue =  HAL_SYS_FREQ_13M;
            break;

        case HAL_SYS_FREQ_SCAL_26M:
            convValue =  HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_SCAL_39M:
            convValue =  HAL_SYS_FREQ_39M;
            break;

        case HAL_SYS_FREQ_SCAL_52M:
            convValue =  HAL_SYS_FREQ_52M;
            break;

        case HAL_SYS_FREQ_SCAL_78M:
            convValue =  HAL_SYS_FREQ_78M;
            break;

        case HAL_SYS_FREQ_SCAL_104M:
            convValue =  HAL_SYS_FREQ_104M;
            break;

        case HAL_SYS_FREQ_SCAL_156M:
            convValue =  HAL_SYS_FREQ_156M;
            break;

#if (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_FREQ_SCAL_208M:
            convValue =  HAL_SYS_FREQ_208M;
            break;

        case HAL_SYS_FREQ_SCAL_250M:
            convValue =  HAL_SYS_FREQ_250M;
            break;

        case HAL_SYS_FREQ_SCAL_312M:
            convValue =  HAL_SYS_FREQ_312M;
            break;
#endif
        default:
            HAL_ASSERT(FALSE, "Improper Freq. scaling frequency: %d", fsfFreq);
            break;
    }

    return convValue;
}

// =============================================================================
// hal_SysGetRequestFreq
// -----------------------------------------------------------------------------
/// Get the minimum frequency required by a given user
/// @param userId Identifier of the calling user.
/// @return The minimum required frequency for this user.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetRequestFreq(HAL_SYS_FREQ_USER_ID_T userId)
{
    return (hal_SysFSF2SF(hal_SysFreqScalGet(userId)));
}



// =============================================================================
// hal_SysGetFreq
// -----------------------------------------------------------------------------
/// Get the current system clock frequency.
/// @return The current system clock frequency.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetFreq(VOID)
{
    return g_halSysSystemFreq;
}



// =============================================================================
// hal_SysGetShutdownFlag
// -----------------------------------------------------------------------------
/// Get the shutdown flag, and then reset the flag.
// =============================================================================
PUBLIC BOOL hal_SysGetShutdownFlag(VOID)
{
    if(g_halSysStateShutdownFlag == SYS_SHUTDOWN_MAGIC_NUM)
    {
        g_halSysStateShutdownFlag = 0x0;
        return TRUE;
    }
    return FALSE;
}



// =============================================================================
// hal_SysShutdown
// -----------------------------------------------------------------------------
/// A call to this function shuts the system down.
// =============================================================================
PUBLIC VOID hal_SysShutdown(VOID)
{
    // Check that the alarm has finished to be set
    // if an alarm has been programmed before shutting 
    // down
    while (hwp_calendar->cmd & CALENDAR_ALARM_ENABLE_SET);

    UINT32 status __attribute__((unused))  = hal_SysEnterCriticalSection();

    hal_HstSendEvent(SYS_EVENT,0x1a11dead);
    hal_abbEnablePa(FALSE);
    hal_abbPdAll(TRUE);
    // Enter power-save mode
    pmd_Close();
    // Power down
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->WakeUp  = 0;

    // Force to flush the write buffer.
    // VOLATILE is needed as the compiler optimizes the while (TRUE)
    // too much.
    VOLATILE UINT32 dummyRead __attribute__((unused)) = hwp_sysCtrl->WakeUp;

    // Wait 1 second ...
    hal_TimDelay(HAL_TICK1S);

    // We are still alive ?

    // Set the shutdown flag to record that we just went through the shutdown
    // process
    g_halSysStateShutdownFlag = SYS_SHUTDOWN_MAGIC_NUM;

    // Just reset so we don't get stuck here!
    hal_SysRestart();

    while (TRUE);
}


// =============================================================================
// hal_SysSoftReset
// -----------------------------------------------------------------------------
/// Does a soft reset on the chip.
/// Generally, you should NOT use this function, use hal_SysRestart() instead.
// =============================================================================
PUBLIC VOID hal_SysSoftReset(VOID)
{
    UINT32 status __attribute__((unused))  = hal_SysEnterCriticalSection();

#ifdef __SPIFLSH_SUPPORT_SOFTRESET__
	memd_FlashRst();
#endif

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#if (CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->Sys_Rst_Set= SYS_CTRL_SYS_SOFT_RST;
#else
    hwp_sysCtrl->Sys_Rst_Set= SYS_CTRL_SOFT_RST;
#endif

    while (1); // We die here ...

    // hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    // hal_SysExitCriticalSection(status);
}

PUBLIC VOID hal_SysSetBootMode(UINT8 mode)
{
    hwp_sysCtrl->Reset_Cause &= SYS_CTRL_BOOT_MODE_MASK;
    hwp_sysCtrl->Reset_Cause |= SYS_CTRL_BOOT_MODE(mode);
}

// =============================================================================
// hal_SysRestart
// -----------------------------------------------------------------------------
/// A call to this function reboots the system.
/// First, this function does a power cycle on USB
// =============================================================================
PUBLIC VOID hal_SysRestart(VOID)
{
#if (CHIP_HAS_USB == 1)&& !defined(DISABLE_USB_SUPPORT)
    UINT32 startTime;

    startTime = hal_TimGetUpTime();
    hal_UsbReset(TRUE);
    while(hal_TimGetUpTime() < startTime + 8000);
    hal_UsbReset(FALSE);
#endif // (CHIP_HAS_USB == 1)

    hal_SysSoftReset();
}



// =============================================================================
// hal_SysGetPLLLock
// -----------------------------------------------------------------------------
/// Return PLL lock 
/// @return PLL status
///         If \c TRUE, PLL locked.
///         If \c FALSE, PLL not locked.
// =============================================================================
PROTECTED BOOL hal_SysGetPLLLock(VOID)
{
    if(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED)
    {
        return TRUE;
    }
    return FALSE;
}

#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
// =============================================================================
// hal_SysDisablePLL
// -----------------------------------------------------------------------------
/// Disable PLL and switch on low clock
// =============================================================================
PROTECTED VOID hal_SysDisablePLL(VOID)
{
    // FIXME Implement
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Pll_Ctrl &= (~SYS_CTRL_PLL_ENABLE_ENABLE);
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
}



// =============================================================================
// hal_SysEnablePLL
// -----------------------------------------------------------------------------
/// Enable PLL
// =============================================================================
PROTECTED VOID hal_SysEnablePLL(VOID)
{
    // FIXME Implement
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_ENABLE_ENABLE;
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
}
/******************************************************************************
 * hal_SysIsPllNeeded: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC BOOL hal_SysIsPllNeeded(VOID)
{
    //Check if spi flash need pll
    if((hwp_sysCtrl->Cfg_Clk_SpiFlash & SYS_CTRL_SPIFLASH_FREQ_MASK) > SYS_CTRL_SPIFLASH_FREQ_52M)
        return TRUE;

    //Check if voc need pll
    if((hwp_sysCtrl->Cfg_Clk_Voc & SYS_CTRL_VOC_FREQ_MASK) > SYS_CTRL_VOC_FREQ_52M)
        return TRUE;

    //Check if sys need pll
    if((hwp_sysCtrl->Cfg_Clk_Sys & SYS_CTRL_FREQ_MASK) > SYS_CTRL_FREQ_52M)
        return TRUE;

    //If sel_clock is fast clock and dig_en is off
    if((0 == (hwp_sysCtrl->Sel_Clock & SYS_CTRL_SYS_SEL_FAST_SLOW)) && (0 == (hwp_sysCtrl->Sel_Clock & SYS_CTRL_DIGEN_H_ENABLE)))
       return TRUE;

#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
    if(Codec_Need_Audpll_flag == TRUE)
       return TRUE;
#endif

    return FALSE;
}
#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
/******************************************************************************
 * hal_SysRequestPll: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PROTECTED VOID hal_SysRequestPll(HAL_SYS_PLL_USERS_T userId, BOOL on)
{
    UINT32 status = hal_SysEnterCriticalSection();

    //hal_HstSendEvent(SYS_EVENT, 0x20160613);
    //hal_HstSendEvent(SYS_EVENT, (userId <<16) | on);

    hal_AudEnableCodecAuxPll(on);

    if(on || hal_SysIsPllNeeded())
    {
        //Enable pll div
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_BYPASS_PASS
                              | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                                | SYS_CTRL_PLL_LOCK_RESET_RESET;
        //check pll lock
        hwp_sysCtrl->Pll_Ctrl   |= SYS_CTRL_PLL_LOCK_RESET_NO_RESET;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

        //Wait pll locked
        while(!hal_SysGetPLLLock());
    }
    else
    {
        //Close pll div
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_BYPASS_BYPASS;
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    }

    //hal_HstSendEvent(SYS_EVENT, 0x20160614);
    hal_SysExitCriticalSection(status);

}

#else
/******************************************************************************
 * hal_SysRequestPll: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PROTECTED VOID hal_SysRequestPll(HAL_SYS_PLL_USERS_T userId, BOOL on)
{
    UINT32 status = hal_SysEnterCriticalSection();

    //hal_HstSendEvent(SYS_EVENT, 0x20160613);
    //hal_HstSendEvent(SYS_EVENT, (userId <<16) | on);



    if(on || hal_SysIsPllNeeded())
    {
        //hal_HstSendEvent(SYS_EVENT, 0x11111111);
        //Enable sys_pll and pll div
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_ENABLE_ENABLE;
        hwp_sysCtrl->Pll_Ctrl &= ~(SYS_CTRL_PLL_BYPASS_BYPASS | SYS_CTRL_PLL_LOCK_RESET_NO_RESET);
        hwp_sysCtrl->Pll_Ctrl   |= SYS_CTRL_PLL_LOCK_RESET_NO_RESET;
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;

        while(!hal_SysGetPLLLock());
    }
    else
    {
        //hal_HstSendEvent(SYS_EVENT, 0x00000000);
        //There is an inner digen which is write to 1 
        //when outter digen is 1 and clock is updated.
        //So we update clock to ensure inner digen is equal out digen here.
        //Or xcpu will die for no clock......
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Cfg_Clk_Sys |= SYS_CTRL_FORCE_DIV_UPDATE;
        hwp_sysCtrl->Cfg_Clk_SpiFlash = hwp_sysCtrl->Cfg_Clk_SpiFlash;
        hwp_sysCtrl->Cfg_Clk_Voc = hwp_sysCtrl->Cfg_Clk_Voc;
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;

        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        //DisablePLL
        hwp_sysCtrl->Pll_Ctrl &= (~SYS_CTRL_PLL_ENABLE_ENABLE);
        //Close pll div
        //3 Switch to dig_52M ERROR when there is no pll and bypass is set 0, so we MUST set it 1 here
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_BYPASS_BYPASS;
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    }

    //hal_HstSendEvent(SYS_EVENT, 0x20160614);
    //hal_SysExitCriticalSection(status);

}
#endif /* __HAL_SYS_USE_AUX_PLL_ALWAYS__ */
#else
PROTECTED VOID hal_SysRequestPll(HAL_SYS_PLL_USERS_T userId, BOOL on)
{
    ;//Use default pll status, So we do nothing here
}
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */

// =============================================================================
// hal_SysGetFrequencyConfiguration
// -----------------------------------------------------------------------------
/// This function returns the frequency configuration to apply to the system 
/// when required to set a given frequency. If the system cannot produce that
/// frequency, the closest available higher frequency will be applied.
/// The real frequency to apply is returned. The second pointer parameter will
/// store the bitfield value to use in the chip register.
///
/// @param fastClock Minimum frequency the system must output.
/// @param sysCtrlRegConfig Value to set in the sysCtrl bitfield to get the 
/// configured frequency.
/// @param enableCpuTurbo Pointer to a variable set to TRUE if, and only if,
/// the X-CPU clock doubler needs to be enabled at that frequency. This is
/// enabled only on chip supporting that feature.
/// @return The actual frequency that is going to be applied with that 
/// sysCtrl bitfield value.
// =============================================================================
PRIVATE HAL_SYS_FREQ_T hal_SysGetFrequencyConfiguration(HAL_SYS_FREQ_T fastClock,
                                                        UINT32*        sysCtrlRegConfig,
                                                        BOOL*          enableCpuTurbo)
{
    HAL_SYS_FREQ_T realFreq = fastClock;
    *enableCpuTurbo = FALSE;

#if defined(FPGA)

#if 0
    switch(fastClock)
    {
        case HAL_SYS_FREQ_32K:
            // Not relevant, as we stay on the slow clock.
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_13M:
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_39M:
        case HAL_SYS_FREQ_52M:
        case HAL_SYS_FREQ_78M:
        case HAL_SYS_FREQ_104M:
        case HAL_SYS_FREQ_156M:
        case HAL_SYS_FREQ_208M:
#ifdef FPGA_SLOW
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;
#else
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            realFreq = HAL_SYS_FREQ_52M;
            break;
#endif
        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
    }
#else
    // Fixed to 52M for FPGA
    *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
    realFreq = HAL_SYS_FREQ_52M;
#endif

#else // !FPGA

#if (CHIP_HAS_ASYNC_TCU)

    switch(fastClock)
    {
        case HAL_SYS_FREQ_32K:
            // Not relevant, as we stay on the slow clock.
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_13M:
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_39M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_39M;
            break;

        case HAL_SYS_FREQ_52M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            break;

        case HAL_SYS_FREQ_78M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_78M;
            break;

        case HAL_SYS_FREQ_89M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_89M;
            break;

        case HAL_SYS_FREQ_104M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            break;

        case HAL_SYS_FREQ_113M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_113M;
            break;

        case HAL_SYS_FREQ_125M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_125M;
            break;

        case HAL_SYS_FREQ_139M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_139M;
            break;

        case HAL_SYS_FREQ_156M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_156M;
            break;

        case HAL_SYS_FREQ_178M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_178M;
            break;

        case HAL_SYS_FREQ_208M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_208M;
            break;

        case HAL_SYS_FREQ_250M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_250M;
            break;

        case HAL_SYS_FREQ_312M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_312M;
            break;

        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
    }

#else // !CHIP_HAS_ASYNC_TCU

    switch(fastClock)
    {
        case HAL_SYS_FREQ_32K:
            // Not relevant, as we stay on the slow clock.
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_13M:
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_39M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_39M;
            break;

        case HAL_SYS_FREQ_52M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            break;

        case HAL_SYS_FREQ_78M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_78M;
            break;

        case HAL_SYS_FREQ_104M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            break;

        case HAL_SYS_FREQ_156M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_156M;
            break;

        case HAL_SYS_FREQ_208M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            realFreq = HAL_SYS_FREQ_104M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
            break;
        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
    }

#endif // !CHIP_HAS_ASYNC_TCU

#endif // !FPGA

    return realFreq;
}



#if (CHIP_HAS_ASYNC_TCU)
// ============================================================================
// hal_SysTcuClockIs26M
// ----------------------------------------------------------------------------
/// Check whether TCU can only be running at 26M.
/// @return TRUE if TCU should always running at 26M, FALSE otherwise.
// ============================================================================
PRIVATE BOOL HAL_BOOT_FUNC hal_SysTcuClockIs26M(VOID)
{
#if 0
    UINT32 productId = hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID);
    if ( (productId == HAL_SYS_CHIP_PROD_ID_8808) ||
         ( productId == HAL_SYS_CHIP_PROD_ID_8808S && 
           hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) == HAL_SYS_CHIP_METAL_ID_U00 ) )
#endif
    {
        // TCU is always running @26MHz on 8808 and 8808s-U0,
        // and @13MHz on later chips (8808s-U2, 8809 ...)
        return TRUE;
    }
    return FALSE;
}
#endif


PUBLIC VOID HAL_BOOT_FUNC hal_SysSetupNonePLL(VOID)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    UINT32 tcuClock;

    tcuClock = SYS_CTRL_SYS_SEL_TCU_26M;

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;


    // Select clock source


        // Only 52Mhz available through PLL on FPGA:
        // - 26 MHz is done with the RF clock.
        // - 52 Mhz is done with the Pll.

    hwp_sysCtrl->Sel_Clock = hwp_sysCtrl->Sel_Clock 
    						&(~SYS_CTRL_SYS_SEL_FAST_SLOW) | SYS_CTRL_SYS_SEL_FAST_FAST
							&(~SYS_CTRL_DIGEN_H_ENABLE) | SYS_CTRL_DIGEN_H_ENABLE
    						;
	
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
}

// ============================================================================
// hal_SysSetupPLL
// ----------------------------------------------------------------------------
/// Setup PLL at early system boot time.
// ============================================================================
PUBLIC VOID HAL_BOOT_FUNC hal_SysSetupPLL(VOID)
{
#if (CHIP_HAS_ASYNC_TCU)
    g_halSysTcuRunningAt26M = hal_SysTcuClockIs26M();

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

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#ifdef HAL_SYS_SETUP_CLOCK_32K

    // Turning off the Pll and reset of the Lock detector
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

#else // !HAL_SYS_SETUP_CLOCK_32K
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
     // Turning off the Pll and reset of the Lock detector
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

#if (!CHIP_HAS_ASYNC_TCU)

    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET;

#else // CHIP_HAS_ASYNC_TCU

#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_PLL_BYPASS_BYPASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_PLL_LOCK_RESET_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_RESET;
#else // !USE_DIG_CLK_ONLY_AT_IDLE
    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;
#endif // !USE_DIG_CLK_ONLY_AT_IDLE

#endif // CHIP_HAS_ASYNC_TCU
#endif //(CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    // Wait for lock.
    while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));

    // Allow clock through
#if (!CHIP_HAS_ASYNC_TCU)

    hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE;

#else // CHIP_HAS_ASYNC_TCU

#ifndef USE_DIG_CLK_ONLY_AT_IDLE
    hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                           | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE;
#endif // !USE_DIG_CLK_ONLY_AT_IDLE

#ifdef USE_DIGITAL_CLK
    hwp_sysCtrl->Sel_Clock |= (SYS_CTRL_DIGEN_H_ENABLE | SYS_CTRL_MPMC_SYS_SAME_ENABLE);
#endif // USE_DIGITAL_CLK

#endif // CHIP_HAS_ASYNC_TCU

#endif // !HAL_SYS_SETUP_CLOCK_32K

    // Select clock source
#ifdef FPGA

        // Only 52Mhz available through PLL on FPGA:
        // - 26 MHz is done with the RF clock.
        // - 52 Mhz is done with the Pll.
#ifdef HAL_SYS_SETUP_CLOCK_26M
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
        | SYS_CTRL_SYS_SEL_FAST_SLOW
#if (CHIP_HAS_ASYNC_TCU)
        | SYS_CTRL_BB_SEL_FAST_SLOW
        | SYS_CTRL_MPMC_SYS_SAME_ENABLE
        | tcuClock
#endif
        ;
#else // !HAL_SYS_SETUP_CLOCK_26M
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
        | SYS_CTRL_SYS_SEL_FAST_FAST
#if (CHIP_HAS_ASYNC_TCU)
        | SYS_CTRL_BB_SEL_FAST_FAST
        | SYS_CTRL_MPMC_SYS_SAME_ENABLE
        | tcuClock
#endif
        ;
#endif // !HAL_SYS_SETUP_CLOCK_26M
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
    //Switch to slow clock 26M
    hwp_sysCtrl->Sel_Clock |= SYS_CTRL_DIGEN_H_ENABLE | SYS_CTRL_SYS_SEL_FAST_SLOW;
    while(0 == (hwp_sysCtrl->Sel_Clock & SYS_CTRL_SYS_SEL_FAST_SLOW)); 

    
    //Close system pll here....
    hwp_sysCtrl->Pll_Ctrl &= (~SYS_CTRL_PLL_ENABLE_ENABLE);
    // Put Lock in Reset.
    hwp_sysCtrl->Pll_Ctrl &= ~(SYS_CTRL_PLL_LOCK_RESET_MASK); 

#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
    Boot_AudEnableCodecAuxPll();
#endif

#else
    hwp_sysCtrl->Sel_Clock |= SYS_CTRL_DIGEN_H_ENABLE;
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */

#else // !FPGA

    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
            | SYS_CTRL_SYS_SEL_FAST_FAST
#if (CHIP_HAS_ASYNC_TCU)
#ifdef USE_DIGITAL_CLK
            | SYS_CTRL_DIGEN_H_ENABLE
#endif // USE_DIGITAL_CLK
            | SYS_CTRL_BB_SEL_FAST_FAST
            | SYS_CTRL_MPMC_SYS_SAME_ENABLE
            | tcuClock
#endif // CHIP_HAS_ASYNC_TCU
            ;

#endif // !FPGA

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
}

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
PUBLIC VOID HAL_BOOT_FUNC hal_SysEnableClkOut(uint8 div)
{
    hwp_configRegs->audio_pd_set |= CFG_REGS_AU_REF_PD_N;

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
   
    hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_DEBUG | SYS_CTRL_CLKOUT_SEL_DIVIDER;
    hwp_sysCtrl->Clk_Other_Enable |= SYS_CTRL_ENABLE_OC_CLK_OUT | SYS_CTRL_ENABLE_OC_BCK ; 
    hwp_sysCtrl->Cfg_Clk_AudioBCK_Div = div-2;
	
    //wangxin temp
    hwp_debugPort->Dbg_Clk_Cfg = DEBUG_PORT_SEL_SOURCE(63)
                                | DEBUG_PORT_ENABLE_DBG_ENABLED;
	
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK; 
}
#endif

PUBLIC VOID HAL_BOOT_FUNC hal_SysEnable8KSampleRate(VOID)
{
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Cfg_Clk_AudioBCK_Div =16;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
}

// =============================================================================
// hal_SysSetupSystemClock
// -----------------------------------------------------------------------------
/// Configure the initial settings of the system clocks.
/// This function is to be called only by hal_init.
/// It also checks the validity of the public enum for HAL_SYS_FREQ_T and 
/// the internal one HAL_SYS_FREQ_SCAL_USERS_T use for frequency scaling.
/// @param fastClockSetup Initial System Clock.
// =============================================================================
PROTECTED VOID hal_SysSetupSystemClock(HAL_SYS_FREQ_T fastClockSetup)
{
    HAL_SYS_FREQ_T fastClock      = fastClockSetup;
    UINT32         sysFreq        = 0;
    BOOL           enableCpuTurbo = FALSE;
    UINT32         i;

    // Frequency clamping
    if (fastClock > HAL_SYS_MAX_SPEED)
    {
        HAL_TRACE(HAL_WARN_TRC, 0, "hal_SysSetupSystemClock clamped the request for a %d frequency. %d", fastClock, HAL_SYS_MAX_SPEED);
        fastClock = HAL_SYS_MAX_SPEED;
    }

    // Check the validity of the two enums
    // The HAL user Id are encroaching the system ones
    HAL_ASSERT(HAL_SYS_FREQ_SYS_LAST_USER<HAL_SYS_USER_START,
        "Bad value for the HAL_SYS_FREQ_USER_ID_T enum\n  "
        "The first public user,%d, is less than the "
        "last hal private one,%d.",HAL_SYS_USER_START,HAL_SYS_FREQ_SYS_LAST_USER);

    HAL_ASSERT(HAL_SYS_FREQ_PLATFORM_LAST_USER<HAL_SYS_FREQ_APP_FIRST_USER,
        "Bad value for the HAL_SYS_FREQ_USER_ID_T enum\n  "
        "The first application user,%d, is less than the "
        "last platform one,%d.",HAL_SYS_FREQ_APP_FIRST_USER,HAL_SYS_FREQ_PLATFORM_LAST_USER);

    HAL_ASSERT(HAL_SYS_TOTAL_USERS_QTY<=HAL_SYS_FREQ_MAX_RESOURCE_NB,
        "Bad value for the HAL_SYS_FREQ_USER_ID_T enum\n  "
        "The last public user,%d, is greater than the "
        "total available number in hal %d.",HAL_SYS_TOTAL_USERS_QTY,HAL_SYS_FREQ_MAX_RESOURCE_NB);

    // Init the freq scal registries
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_32K ] = HAL_SYS_TOTAL_USERS_QTY ;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_13M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_26M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_39M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_52M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_78M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_104M] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_156M] = 0;
#if (CHIP_HAS_ASYNC_TCU)
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_208M] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_250M] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_312M] = 0;
#endif
    // Init the freq request list
    for (i = 0; i < HAL_SYS_FREQ_MAX_RESOURCE_NB/4; i++)
    {
        g_halSysFreqScalRegistry[i] = 0;
    }

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClock, &sysFreq, &enableCpuTurbo);

    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }

    // Do not set VCORE here for PMU is initialized after setting up sys clk
    {

        UINT32 status = hal_SysEnterCriticalSection();
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#if (!CHIP_HAS_ASYNC_TCU)
            hwp_sysCtrl->Cfg_Clk_Sys = sysFreq | SYS_CTRL_FORCE_DIV_UPDATE; 
#else // CHIP_HAS_ASYNC_TCU
            hwp_sysCtrl->Cfg_Clk_Sys = sysFreq | SYS_CTRL_FORCE_DIV_UPDATE;
            hwp_sysCtrl->Cfg_Clk_BB = SYS_CTRL_BCPU_FREQ | SYS_CTRL_BB_FORCE_DIV_UPDATE;
#endif // CHIP_HAS_ASYNC_TCU
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SYS_SEL_FAST_SLOW;
#endif

        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        hal_SysExitCriticalSection(status);
    }

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    // setup gsm clocks: (TCU clock)
    hwp_tcu->Cfg_Clk_Div= TCU_ENABLE_QBIT_ENABLED;
#endif /* CHIP_ASIC_ID != CHIP_ASIC_ID_5856 */

    if (g_halSysSystemFreq > fastClock)
    {  // when going to a faster clock need to setup everything first
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    // Do not set VCORE here for PMU is initialized after setting up sys clk

    // update global variables
    g_halSysSystemFreq = fastClock;
}


// =============================================================================
// hal_SysChangeSystemClockByOneStep
// -----------------------------------------------------------------------------
/// Change the system clock (previously started and configured) to a (possibly)
/// new value by one step
/// @param fastClockSetup New System clock applied to the system.
// =============================================================================
PRIVATE VOID hal_SysChangeSystemClockByOneStep(HAL_SYS_FREQ_T fastClockSetup)
{
    HAL_SYS_FREQ_T fastClock;
    UINT32 sysFreq;
    BOOL enableCpuTurbo;

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClockSetup, &sysFreq, &enableCpuTurbo);

    if (g_halSysSystemFreq == fastClock)
    {
        return;
    }

    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }

    {
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

        UINT32 status = hal_SysEnterCriticalSection();

#if (!CHIP_HAS_ASYNC_TCU)
        // Accumulate the QBit error induced by the clock switch
        {
            UINT32 oldSysFreq = hwp_sysCtrl->Cfg_Clk_Sys & SYS_CTRL_FREQ_MASK;
            g_halSysClockSwitchQBitError += g_halSysClockSwitchQBitErrorCompensate312[sysFreq-SYS_CTRL_FREQ_26M][oldSysFreq-SYS_CTRL_FREQ_26M];
        }
#endif // (!CHIP_HAS_ASYNC_TCU)

        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#ifdef USE_DIG_CLK_ONLY_AT_IDLE
        if (sysFreq < SYS_CTRL_FREQ_78M) //Disable SysPLL
        {
            // Configure System Clock.
            hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ(sysFreq);
            hal_TimDelay(1);
            hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                                  | SYS_CTRL_PLL_BYPASS_BYPASS
                                  | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                                  | SYS_CTRL_PLL_LOCK_RESET_RESET
                                  | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                                  | SYS_CTRL_BB_PLL_BYPASS_BYPASS
                                  | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                                  | SYS_CTRL_BB_PLL_LOCK_RESET_RESET;
        }
        else //Enable SysPLL
        {
            UINT32 pll_ctrl_reg = SYS_CTRL_PLL_ENABLE_ENABLE
                                  | SYS_CTRL_PLL_BYPASS_PASS
                                  | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                                  | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                                  | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                                  | SYS_CTRL_BB_PLL_BYPASS_PASS
                                  | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE
                                  | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;
            hwp_sysCtrl->Pll_Ctrl = pll_ctrl_reg;
            hal_TimDelay(1);
            // Wait for lock.
            //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));
            //while(!((hwp_sysCtrl->Sel_Clock & (SYS_CTRL_BB_PLL_LOCKED|SYS_CTRL_PLL_LOCKED))==
            //    (SYS_CTRL_BB_PLL_LOCKED|SYS_CTRL_PLL_LOCKED)));
            //hwp_sysCtrl->Pll_Ctrl = pll_ctrl_reg | SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE;
            // Configure System Clock.
            hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ(sysFreq);
        }
#elif defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)

        if(sysFreq > SYS_CTRL_FREQ_52M)//Enable SysPLL
        {
            hal_SysRequestPll(HAL_SYS_PLL_SYS, TRUE);
        }
        else if(sysFreq > SYS_CTRL_FREQ_26M) //Enable doubler
        {
            //pmd_SetDoubler(TRUE);
        }
        else
        {
        }

        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

        // Configure System Clock.
        hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ(sysFreq)
#if (CHIP_HAS_ASYNC_TCU)
                | SYS_CTRL_FORCE_DIV_UPDATE
#endif
                ;
        //Switch clock
        if(sysFreq > SYS_CTRL_FREQ_26M)
        {
            hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SYS_SEL_FAST_SLOW;
        }
        else
        {
            hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
        }
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

        //Disable SysPLL
        if (sysFreq < SYS_CTRL_FREQ_78M)
        {
            hal_SysRequestPll(HAL_SYS_PLL_SYS, FALSE);
        }

        //Disable Doubler
        if (sysFreq < SYS_CTRL_FREQ_39M)
        {
            //pmd_SetDoubler(FALSE);
        }
        
#else // !USE_DIG_CLK_ONLY_AT_IDLE
        // Configure System Clock.
        hwp_sysCtrl->Cfg_Clk_Sys = SYS_CTRL_FREQ(sysFreq)
#if (CHIP_HAS_ASYNC_TCU)
                | SYS_CTRL_FORCE_DIV_UPDATE
#endif
                ;
#endif // !USE_DIG_CLK_ONLY_AT_IDLE

#ifdef FPGA
        // For FPGA, up to now, 26 Mhz can only be done
        // through the RF clock.
        if (fastClock == HAL_SYS_FREQ_26M)
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                | SYS_CTRL_SYS_SEL_FAST_SLOW
#if (CHIP_HAS_ASYNC_TCU)
                | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                | tcuClock
#endif
                ;
        }
        else
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                | SYS_CTRL_SYS_SEL_FAST_FAST
#if (CHIP_HAS_ASYNC_TCU)
                | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                | tcuClock
#endif
                ;
        }
#endif // FPGA
        
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        
        hal_SysExitCriticalSection(status);
    }

    if (g_halSysSystemFreq > fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }
}


// =============================================================================
// hal_SysChangeSystemClock
// -----------------------------------------------------------------------------
/// Change the system clock (previously started and configured) to a (possibly)
/// new value, possibly by several steps.
/// @param fastClockSetup New System clock applied to the system.
// =============================================================================
PRIVATE VOID hal_SysChangeSystemClock(HAL_SYS_FREQ_T fastClockSetup)
{
    // assert setup already done (pll on / sys is fast)
    HAL_SYS_FREQ_T fastClock;
    UINT32 sysFreq;
    BOOL enableCpuTurbo;

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClockSetup, &sysFreq, &enableCpuTurbo);

    // If the newly asked frequency is the same as before,
    // nevermind ...
    if (g_halSysSystemFreq == fastClock)
    {
        return;
    }

    // Check Wether LPS allows us to change the system frequency.
    // If we are lock by the LPS, we do not change the frequency here to keep 
    // synch'ed.
    // The LPS calibration depends on the system frequency, 
    // this function will invalidate it if needed.
    if (!hal_LpsClockSysChangeAllowed(fastClock))
    {
        return;
    }

    if (g_halSysSystemFreq <= HAL_SYS_FREQ_104M && fastClock > HAL_SYS_FREQ_104M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SYS, PMD_CORE_VOLTAGE_HIGH);
    }

    // ISSUE:
    // The system frequency change will lead to an instant clock jitter for
    // peripheral modules, if they are divided from the system frequency.
    // Some of these modules, such as SIM, are clock sensitive, and they
    // might fall into troubles when the frequency is changed in a large
    // range.
    // WORKAROUND:
    // Change the system frequency in a pre-defined range, and change
    // multiple times till reaching the target frequency if needed.
    // PERFORMANCE NOTE:
    // It takes 3 ticks to arise the system frequency from 39M to 250M,
    // and the intermediate frequencies include: 52M, 104M, 156M, and 208M.

    INT32 i, start, end; // Must be signed
    BOOL moreThanOneStep = FALSE;
    i = 0;
    if (g_halSysSystemFreq > fastClock)
    {
        for (; i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i++)
        {
            if (fastClock < g_halSysFreqSteps[i])
            {
                break;
            }
        }
        // Sys freq can be changed directly if start and end are both
        // at the same freq step
        if (g_halSysSystemFreq > g_halSysFreqSteps[i])
        {
            end = i;
            i++;
            for (; i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i++)
            {
                if (g_halSysSystemFreq <= g_halSysFreqSteps[i])
                {
                    if (g_halSysSystemFreq == g_halSysFreqSteps[i] &&
                        end == i - 1)
                    {
                        moreThanOneStep = TRUE;
                    }
                    break;
                }
            }
            start = i - 1;
            // Sys freq can be changed directly if (start == end) and
            // the distance is smaller or equal than one step
            if (start > end || moreThanOneStep)
            {
                for (i=start; i>=end && i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i--)
                {
                    hal_SysChangeSystemClockByOneStep(g_halSysFreqSteps[i]);
                }
            }
        }
    }
    else
    {
        for (; i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i++)
        {
            if (g_halSysSystemFreq < g_halSysFreqSteps[i])
            {
                break;
            }
        }
        // Sys freq can be changed directly if start and end are both
        // at the same freq step
        if (fastClock > g_halSysFreqSteps[i])
        {
            start = i;
            i++;
            for (; i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i++)
            {
                if (fastClock <= g_halSysFreqSteps[i])
                {
                    if (fastClock == g_halSysFreqSteps[i] &&
                        start == i - 1)
                    {
                        moreThanOneStep = TRUE;
                    }
                    break;
                }
            }
            end = i - 1;
            // Sys freq can be changed directly if (start == end) and
            // the distance is smaller or equal than one step
            if (start < end || moreThanOneStep)
            {
                for (i=start; i<=end && i<sizeof(g_halSysFreqSteps)/sizeof(g_halSysFreqSteps[0]); i++)
                {
                    hal_SysChangeSystemClockByOneStep(g_halSysFreqSteps[i]);
                }
            }
        }
    }
    hal_SysChangeSystemClockByOneStep(fastClock);

    if (g_halSysSystemFreq > HAL_SYS_FREQ_104M && fastClock <=  HAL_SYS_FREQ_104M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SYS, PMD_CORE_VOLTAGE_LOW);
    }

#ifdef HAL_CLK_PROFILING

        switch(g_halSysSystemFreq)
        {
            case HAL_SYS_FREQ_32K  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_32K); break; 
            case HAL_SYS_FREQ_13M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_13M); break; 
            case HAL_SYS_FREQ_26M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_26M); break; 
            case HAL_SYS_FREQ_39M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_39M); break; 
            case HAL_SYS_FREQ_52M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_52M); break; 
            case HAL_SYS_FREQ_104M : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_104M); break;
            default                : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_OTHER); break;
        }

        switch(fastClock)
        {
            case HAL_SYS_FREQ_32K  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_32K); break; 
            case HAL_SYS_FREQ_13M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_13M); break; 
            case HAL_SYS_FREQ_26M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_26M); break; 
            case HAL_SYS_FREQ_39M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_39M); break; 
            case HAL_SYS_FREQ_52M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_52M); break; 
            case HAL_SYS_FREQ_104M : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_104M); break;
            default                : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_OTHER); break;
        }

#endif // HAL_CLK_PROFILING

    g_halSysSystemFreq = fastClock;
}


#if (CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_SysSetBBClock
// -----------------------------------------------------------------------------
/// Set the BB clock.
/// @param newClock New BB clock.
// =============================================================================
PUBLIC VOID hal_SysSetBBClock(HAL_SYS_BB_FREQ_T newClock)
{
    UINT32 newClkField;
    newClkField = SYS_CTRL_BCPU_FREQ;

    switch(newClock)
    {
        case HAL_SYS_BB_FREQ_26M  : newClkField = SYS_CTRL_BB_FREQ_26M; break; 
        case HAL_SYS_BB_FREQ_39M  : newClkField = SYS_CTRL_BB_FREQ_39M; break; 
        case HAL_SYS_BB_FREQ_52M  : newClkField = SYS_CTRL_BB_FREQ_52M; break; 
        case HAL_SYS_BB_FREQ_78M  : newClkField = SYS_CTRL_BB_FREQ_78M; break; 
        case HAL_SYS_BB_FREQ_104M : newClkField = SYS_CTRL_BB_FREQ_104M; break; 
        case HAL_SYS_BB_FREQ_156M : newClkField = SYS_CTRL_BB_FREQ_156M; break; 
        case HAL_SYS_BB_FREQ_208M : newClkField = SYS_CTRL_BB_FREQ_208M; break; 
        case HAL_SYS_BB_FREQ_312M : newClkField = SYS_CTRL_BB_FREQ_312M; break; 
        default                    : break;
    }
    hwp_sysCtrl->Cfg_Clk_BB = newClkField;
}
#endif


PUBLIC void hal_SysSetVocClock_312M(void)
{
	hal_SysSetVocClock(HAL_SYS_VOC_FREQ_312M); 
}

PUBLIC void hal_SysSetVocClock_208M(void)
{
	hal_SysSetVocClock(HAL_SYS_VOC_FREQ_208M); 
}

PUBLIC void hal_SysSetVocClockDefault(void)
{
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
		hal_SysSetVocClock(HAL_SYS_VOC_FREQ_52M);
#else
		hal_SysSetVocClock(HAL_SYS_VOC_FREQ_26M);
#endif

#else	
#ifdef FPGA
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_26M);
#else
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_78M);
#else
#ifdef VOC_CLK_IS_52M
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_52M);
#else
#ifdef VOC_CLK_IS_78M
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_78M);
#else
#ifdef VOC_CLK_IS_113M
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_113M);
#else
#ifdef VOC_CLK_IS_156M
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_156M);
#else
#ifdef VOC_CLK_IS_208M
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_208M);
#else
#ifdef VOC_CLK_IS_250M
    #ifdef __PRJ_WITH_SDRAM__
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_208M);
    #else
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_250M);
    #endif
#else
#ifdef VOC_CLK_IS_312M
    #ifdef __PRJ_WITH_SDRAM__
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_208M);
    #else
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_312M);
    #endif
#else	
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_78M);
#endif	//312M
#endif	//250M
#endif	//208M
#endif	//156M
#endif	//113M
#endif	//78M
#endif	//52M
#endif
#endif
#endif

}

// =============================================================================
// hal_SysSetVocClock
// -----------------------------------------------------------------------------
/// Set the VOC clock.
/// @param newClock New VOC clock.
/// @return Old VOC clock.
// =============================================================================
PUBLIC HAL_SYS_VOC_FREQ_T hal_SysSetVocClock(HAL_SYS_VOC_FREQ_T newClock)
{
    UINT32 oldClkField;
    UINT32 newClkField;
    HAL_SYS_VOC_FREQ_T oldClock;
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL	
	HAL_SYS_VOC_FREQ_T gCurr_newClock=newClock;
#endif

    UINT32 status = hal_SysEnterCriticalSection();
    //hal_HstSendEvent(SYS_EVENT, 0x20160601);

#if (!CHIP_HAS_ASYNC_TCU)
    newClkField = SYS_CTRL_VOC_FREQ_13M;
    oldClock = SYS_CTRL_VOC_FREQ_13M;
#else
    newClkField = SYS_CTRL_VOC_FREQ_26M;
    oldClock = SYS_CTRL_VOC_FREQ_26M;
#endif

    oldClkField = hwp_sysCtrl->Cfg_Clk_Voc & SYS_CTRL_VOC_FREQ_MASK;
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL	
#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
	if (newClock>HAL_SYS_VOC_FREQ_26M)
	{
		gCurr_newClock=HAL_SYS_VOC_FREQ_52M;
	}
#else
	if (newClock>HAL_SYS_VOC_FREQ_26M)
	{
		gCurr_newClock=HAL_SYS_VOC_FREQ_26M;
	}
#endif

#endif
	switch(oldClkField)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        case SYS_CTRL_VOC_FREQ_13M  : oldClock = HAL_SYS_VOC_FREQ_13M; break; 
#endif
        case SYS_CTRL_VOC_FREQ_26M  : oldClock = HAL_SYS_VOC_FREQ_26M; break; 
        case SYS_CTRL_VOC_FREQ_39M  : oldClock = HAL_SYS_VOC_FREQ_39M; break; 
        case SYS_CTRL_VOC_FREQ_52M  : oldClock = HAL_SYS_VOC_FREQ_52M; break; 
        case SYS_CTRL_VOC_FREQ_78M  : oldClock = HAL_SYS_VOC_FREQ_78M; break; 
        case SYS_CTRL_VOC_FREQ_89M  : oldClock = HAL_SYS_VOC_FREQ_89M; break; 
        case SYS_CTRL_VOC_FREQ_104M : oldClock = HAL_SYS_VOC_FREQ_104M; break; 
#if (CHIP_HAS_ASYNC_TCU)
        case SYS_CTRL_VOC_FREQ_113M : oldClock = HAL_SYS_VOC_FREQ_113M; break; 
        case SYS_CTRL_VOC_FREQ_125M : oldClock = HAL_SYS_VOC_FREQ_125M; break; 
        case SYS_CTRL_VOC_FREQ_139M : oldClock = HAL_SYS_VOC_FREQ_139M; break; 
        case SYS_CTRL_VOC_FREQ_156M : oldClock = HAL_SYS_VOC_FREQ_156M; break; 
        case SYS_CTRL_VOC_FREQ_178M : oldClock = HAL_SYS_VOC_FREQ_178M; break; 
        case SYS_CTRL_VOC_FREQ_208M : oldClock = HAL_SYS_VOC_FREQ_208M; break; 
        case SYS_CTRL_VOC_FREQ_250M : oldClock = HAL_SYS_VOC_FREQ_250M; break; 
        case SYS_CTRL_VOC_FREQ_312M : oldClock = HAL_SYS_VOC_FREQ_312M; break; 
#endif
        default                     : break;
    }
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
    switch(gCurr_newClock)
#else
    switch(newClock)
#endif	
    {
#if (!CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_VOC_FREQ_13M  : newClkField = SYS_CTRL_VOC_FREQ_13M; break; 
#endif
        case HAL_SYS_VOC_FREQ_26M  : newClkField = SYS_CTRL_VOC_FREQ_26M; break; 
        case HAL_SYS_VOC_FREQ_39M  : newClkField = SYS_CTRL_VOC_FREQ_39M; break; 
        case HAL_SYS_VOC_FREQ_52M  : newClkField = SYS_CTRL_VOC_FREQ_52M; break; 
        case HAL_SYS_VOC_FREQ_78M  : newClkField = SYS_CTRL_VOC_FREQ_78M; break; 
        case HAL_SYS_VOC_FREQ_89M  : newClkField = SYS_CTRL_VOC_FREQ_89M; break; 
        case HAL_SYS_VOC_FREQ_104M : newClkField = SYS_CTRL_VOC_FREQ_104M; break; 
#if (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_VOC_FREQ_113M : newClkField = SYS_CTRL_VOC_FREQ_113M; break; 
        case HAL_SYS_VOC_FREQ_125M : newClkField = SYS_CTRL_VOC_FREQ_125M; break; 
        case HAL_SYS_VOC_FREQ_139M : newClkField = SYS_CTRL_VOC_FREQ_139M; break; 
        case HAL_SYS_VOC_FREQ_156M : newClkField = SYS_CTRL_VOC_FREQ_156M; break; 
        case HAL_SYS_VOC_FREQ_178M : newClkField = SYS_CTRL_VOC_FREQ_178M; break; 
        case HAL_SYS_VOC_FREQ_208M : newClkField = SYS_CTRL_VOC_FREQ_208M; break; 
        case HAL_SYS_VOC_FREQ_250M : newClkField = SYS_CTRL_VOC_FREQ_250M; break; 
        case HAL_SYS_VOC_FREQ_312M : newClkField = SYS_CTRL_VOC_FREQ_312M; break; 
#endif
        default                    : break;
    }
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL

    if (oldClock <= HAL_SYS_VOC_FREQ_78M && gCurr_newClock > HAL_SYS_VOC_FREQ_78M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_VOC, PMD_CORE_VOLTAGE_HIGH);
    }
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_SLOW; //SYS_CTRL_VOC_SEL_PLL_PLL | newClkField;
    if (oldClock > HAL_SYS_VOC_FREQ_78M && gCurr_newClock <=  HAL_SYS_VOC_FREQ_78M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_VOC, PMD_CORE_VOLTAGE_LOW);
    }
#else
    if (oldClock <= HAL_SYS_VOC_FREQ_78M && newClock > HAL_SYS_VOC_FREQ_78M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_VOC, PMD_CORE_VOLTAGE_HIGH);
    }

    if(newClock > HAL_SYS_VOC_FREQ_52M)//Enable SysPLL
    {
        hal_SysRequestPll(HAL_SYS_PLL_VOC, TRUE);
    }

   // hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_SLOW; //SYS_CTRL_VOC_SEL_PLL_PLL | newClkField;
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | newClkField;

    if (oldClock > HAL_SYS_VOC_FREQ_78M && newClock <=  HAL_SYS_VOC_FREQ_78M)
    {
        pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_VOC, PMD_CORE_VOLTAGE_LOW);
    }

    if(newClock < HAL_SYS_VOC_FREQ_78M)//Disable SysPLL
    {
        hal_SysRequestPll(HAL_SYS_PLL_VOC, FALSE);
    }


#endif


    //hal_HstSendEvent(SYS_EVENT, 0x20160602);

    hal_SysExitCriticalSection(status);

    return oldClock;
}


PUBLIC HAL_SYS_SPIFLASH_FREQ_T hal_SysSetSpiFlashClock(HAL_SYS_SPIFLASH_FREQ_T newClock)
{
    UINT32 oldClkField;
    UINT32 newClkField;
    HAL_SYS_SPIFLASH_FREQ_T oldClock;

//In case of dead
//hal_TimDelay(1 SECOND);

    UINT32 status = hal_SysEnterCriticalSection();

    newClkField = SYS_CTRL_SPIFLASH_FREQ_26M;
    oldClock = SYS_CTRL_SPIFLASH_FREQ_26M;

    oldClkField = hwp_sysCtrl->Cfg_Clk_SpiFlash & SYS_CTRL_SPIFLASH_FREQ_MASK;
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL	
    newClock = HAL_SYS_SPIFLASH_FREQ_26M;
#endif

	switch(oldClkField)
    {
        case SYS_CTRL_SPIFLASH_FREQ_26M  : oldClock = HAL_SYS_SPIFLASH_FREQ_26M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_39M  : oldClock = HAL_SYS_SPIFLASH_FREQ_39M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_52M  : oldClock = HAL_SYS_SPIFLASH_FREQ_52M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_78M  : oldClock = HAL_SYS_SPIFLASH_FREQ_78M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_89M  : oldClock = HAL_SYS_SPIFLASH_FREQ_89M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_104M : oldClock = HAL_SYS_SPIFLASH_FREQ_104M; break; 
#if (CHIP_HAS_ASYNC_TCU)
        case SYS_CTRL_SPIFLASH_FREQ_113M : oldClock = HAL_SYS_SPIFLASH_FREQ_113M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_125M : oldClock = HAL_SYS_SPIFLASH_FREQ_125M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_139M : oldClock = HAL_SYS_SPIFLASH_FREQ_139M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_156M : oldClock = HAL_SYS_SPIFLASH_FREQ_156M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_178M : oldClock = HAL_SYS_SPIFLASH_FREQ_178M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_208M : oldClock = HAL_SYS_SPIFLASH_FREQ_208M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_250M : oldClock = HAL_SYS_SPIFLASH_FREQ_250M; break; 
        case SYS_CTRL_SPIFLASH_FREQ_312M : oldClock = HAL_SYS_SPIFLASH_FREQ_312M; break; 
#endif
        default                     : break;
    }
    
    switch(newClock)
    {
        case HAL_SYS_SPIFLASH_FREQ_26M  : newClkField = SYS_CTRL_SPIFLASH_FREQ_26M; break; 
        case HAL_SYS_SPIFLASH_FREQ_39M  : newClkField = SYS_CTRL_SPIFLASH_FREQ_39M; break; 
        case HAL_SYS_SPIFLASH_FREQ_52M  : newClkField = SYS_CTRL_SPIFLASH_FREQ_52M; break; 
        case HAL_SYS_SPIFLASH_FREQ_78M  : newClkField = SYS_CTRL_SPIFLASH_FREQ_78M; break; 
        case HAL_SYS_SPIFLASH_FREQ_89M  : newClkField = SYS_CTRL_SPIFLASH_FREQ_89M; break; 
        case HAL_SYS_SPIFLASH_FREQ_104M : newClkField = SYS_CTRL_SPIFLASH_FREQ_104M; break; 
#if (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_SPIFLASH_FREQ_113M : newClkField = SYS_CTRL_SPIFLASH_FREQ_113M; break; 
        case HAL_SYS_SPIFLASH_FREQ_125M : newClkField = SYS_CTRL_SPIFLASH_FREQ_125M; break; 
        case HAL_SYS_SPIFLASH_FREQ_139M : newClkField = SYS_CTRL_SPIFLASH_FREQ_139M; break; 
        case HAL_SYS_SPIFLASH_FREQ_156M : newClkField = SYS_CTRL_SPIFLASH_FREQ_156M; break; 
        case HAL_SYS_SPIFLASH_FREQ_178M : newClkField = SYS_CTRL_SPIFLASH_FREQ_178M; break; 
        case HAL_SYS_SPIFLASH_FREQ_208M : newClkField = SYS_CTRL_SPIFLASH_FREQ_208M; break; 
        case HAL_SYS_SPIFLASH_FREQ_250M : newClkField = SYS_CTRL_SPIFLASH_FREQ_250M; break; 
        case HAL_SYS_SPIFLASH_FREQ_312M : newClkField = SYS_CTRL_SPIFLASH_FREQ_312M; break; 
#endif
        default                    : break;
    }
    if (oldClock <= HAL_SYS_SPIFLASH_FREQ_78M && newClock > HAL_SYS_SPIFLASH_FREQ_78M)
    {
        //pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SPIFLASH, PMD_CORE_VOLTAGE_HIGH);
    }

    if(newClock > HAL_SYS_SPIFLASH_FREQ_52M)//Enable SysPLL
    {
        hal_SysRequestPll(HAL_SYS_PLL_SPIFLASH, TRUE);
    }

	hwp_sysCtrl->Cfg_Clk_SpiFlash = newClkField;

    if(newClock < HAL_SYS_SPIFLASH_FREQ_78M)//Disable SysPLL
    {
        hal_SysRequestPll(HAL_SYS_PLL_SPIFLASH, FALSE);
    }

    if (oldClock > HAL_SYS_SPIFLASH_FREQ_78M && newClock <=  HAL_SYS_SPIFLASH_FREQ_78M)
    {
        //pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SPIFLASH, PMD_CORE_VOLTAGE_LOW);
    }


    hal_SysExitCriticalSection(status);

    return oldClock;
}


// =============================================================================
// hal_SysResetOut
// -----------------------------------------------------------------------------
/// A call to this function resets an external device through the reset-out
/// line, if the parameter is \c TRUE.
/// @param resetOut If \c TRUE, rises the reset-out line.
///                 If \c FALSE, pulls it down.
// =============================================================================
PUBLIC VOID hal_SysResetOut(BOOL resetOut)
{
    if (resetOut == FALSE)
    {
        hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_OUT;
    }
    else
    {
        UINT32 status = hal_SysEnterCriticalSection();
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_OUT;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        hal_SysExitCriticalSection(status);
    }
}

// =============================================================================
// hal_SysInvalidateCache
// -----------------------------------------------------------------------------
/// This function invalidates in the cache the line corresponding to the given 
/// memory area, referred by a pointer and a size.
/// It must be called with the destination buffer after a DMA copy.
/// @param buffer Pointer to the first address whose cache line must be 
/// invalidated
/// @param size Size of the buffer.
// =============================================================================
PUBLIC VOID hal_SysPwmManualMode(VOID)
{
     hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

     hwp_sysCtrl->Clk_Sys_Mode |= SYS_CTRL_MODE_SYSD_PWM_MANUAL;
	 hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

}

// =============================================================================
// hal_SysInvalidateCache
// -----------------------------------------------------------------------------
/// This function invalidates in the cache the line corresponding to the given 
/// memory area, referred by a pointer and a size.
/// It must be called with the destination buffer after a DMA copy.
/// @param buffer Pointer to the first address whose cache line must be 
/// invalidated
/// @param size Size of the buffer.
// =============================================================================
PUBLIC VOID hal_SysInvalidateCache(VOID* buffer, UINT32 size)
{
    if (size > 512)
    {
        // flush cache
        hal_GdbFlushCacheData();
    } else {
        // do a memcpy from uncached to cached to insure coherency of cache
        memcpy( buffer, // destination
                (VOID*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)buffer), // source
                size); // size
    }
}



// =============================================================================
// hal_SysGetChipId
// -----------------------------------------------------------------------------
/// That function gives the chip ID the software is running on.
/// @param part Which part of the chip ID
/// @return The requested part of the chip ID.
// =============================================================================
PUBLIC UINT32 HAL_BOOT_FUNC hal_SysGetChipId(HAL_SYS_CHIP_ID_T part)
{
    UINT32 chipId = hwp_configRegs->CHIP_ID;

    if (part == HAL_SYS_CHIP_PROD_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_PROD_ID);
    }
    else if (part == HAL_SYS_CHIP_BOND_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_BOND_ID);
    }
    else if (part == HAL_SYS_CHIP_METAL_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_METAL_ID);
    }
    else // HAL_SYS_CHIP_ID_WHOLE
    {
        // returning the whole chip ID
    }

    return chipId; 
}


// =============================================================================
// hal_SysClkOut
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function requests a resource of #HAL_SYS_FREQ_26M or release it 
/// to #HAL_SYS_FREQ_32K depending on the \p freq parameter value.
///
/// @param freq the required frequency or #HAL_SYS_CLOCK_OUT_OFF
// =============================================================================
PRIVATE VOID hal_SysClkOut(HAL_SYS_CLOCK_OUT_FREQ_T freq)
{

    if (freq == HAL_SYS_CLOCK_OUT_FREQ_26M)
    {
        // Need the RF clock
        hal_SysRequestFreq(HAL_SYS_FREQ_CLK_OUT, HAL_SYS_FREQ_26M, NULL);
    }

    UINT32 status = hal_SysEnterCriticalSection();
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    // TODO Implement the divider
    // management.
    if (freq == HAL_SYS_CLOCK_OUT_FREQ_26M)
    {
        hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_SEL_RF;
    }
    else
    {
        hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_SEL_OSC;
    }

    if (freq != HAL_SYS_CLOCK_OUT_OFF)
    {
        hal_HstSendEvent(SYS_EVENT,0x22225);
        //hwp_sysCtrl->Clk_Other_Enable |= SYS_CTRL_ENABLE_OC_CLK_OUT;
    }
    else
    {
        //hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_CLK_OUT;
    }
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
    hal_SysExitCriticalSection(status);

    if (freq != HAL_SYS_CLOCK_OUT_FREQ_26M)
    {
        // Free the resource
        hal_SysRequestFreq(HAL_SYS_FREQ_CLK_OUT, HAL_SYS_FREQ_32K, NULL);
    }
}


// =============================================================================
// hal_SysClkOutOpen
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function requests a resource of #HAL_SYS_FREQ_26M or not
/// (keep #HAL_SYS_FREQ_32K) depending on the \p freq parameter value.
///
/// @param freq the required frequency
/// @return the user id to be used for #hal_SysClkOutClose
///         or #HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE when the clock is already
///         in use at a different frequency.
// =============================================================================
PUBLIC HAL_SYS_CLOCK_OUT_ID_T hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_T freq)
{
    INT32 id = HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
    INT32 loopId;
    UINT8 curFreq=HAL_SYS_CLOCK_OUT_OFF;
    UINT32 status;

    HAL_ASSERT(((freq <= HAL_SYS_CLOCK_OUT_FREQ_26M) 
                && ( freq >= HAL_SYS_CLOCK_OUT_FREQ_32K))
               , "Clock Out Frequency out of range: %d", freq);

    status = hal_SysEnterCriticalSection();

    for(loopId = 0; loopId < HAL_SYS_CLOCK_OUT_ID_QTY; loopId ++)
    {
        // empty spot?
        if (HAL_SYS_CLOCK_OUT_OFF == g_halSysClockOutRegistry[loopId])
        {
            id = loopId;
        }
        else
        {
            curFreq = g_halSysClockOutRegistry[loopId];
        }
    }
    // no more spot
    if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == id)
    {
        hal_SysExitCriticalSection(status);
        return HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
    }
    // no freqency yet or same freq, ok open it !
    if ((HAL_SYS_CLOCK_OUT_OFF == curFreq) || (freq == curFreq))
    {
        g_halSysClockOutRegistry[id] = freq;
        if (HAL_SYS_CLOCK_OUT_OFF == curFreq)
        {
            hal_SysClkOut(freq);
        }
        hal_SysExitCriticalSection(status);
        return id;
    }
    // else an other frequency is already set
    hal_SysExitCriticalSection(status);
    return HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
}



// =============================================================================
// hal_SysClkOutClose
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function release a resource to #HAL_SYS_FREQ_32K if no more 
/// Clock Out user needs it.
///
/// @param id the user id returned by #hal_SysClkOutOpen
// =============================================================================
PUBLIC VOID hal_SysClkOutClose(HAL_SYS_CLOCK_OUT_ID_T id)
{
    INT32 loopId;
    UINT8 curFreq=HAL_SYS_CLOCK_OUT_OFF;
    UINT32 status;
    HAL_ASSERT(((id < HAL_SYS_CLOCK_OUT_ID_QTY) 
                && (id >= HAL_SYS_CLOCK_OUT_ID_1))
               , "Clock Out Id out of range: %d", id);
    HAL_ASSERT(g_halSysClockOutRegistry[id] != HAL_SYS_CLOCK_OUT_OFF, "Invalid clock out Id (not opened): %d", id);
    status = hal_SysEnterCriticalSection();
    // clear request
    g_halSysClockOutRegistry[id] = HAL_SYS_CLOCK_OUT_OFF;
    for(loopId = 0; loopId < HAL_SYS_CLOCK_OUT_ID_QTY; loopId ++)
    {
        // freq request?
        if (HAL_SYS_CLOCK_OUT_OFF != g_halSysClockOutRegistry[loopId])
        {
            curFreq = g_halSysClockOutRegistry[loopId];
            // one found, stop all must match or be off
            hal_SysExitCriticalSection(status);
            return ;
        }
    }
    // No clock out needed.
    hal_SysClkOut(HAL_SYS_CLOCK_OUT_OFF);
    hal_SysExitCriticalSection(status);
}



// ==========================================================================
// hal_SysAuxClkOut
// --------------------------------------------------------------------------
/// Manage the output of the 26M AUX Clock, to an external device
/// @param enable If \c TRUE, enable AuxClk output.
///               If \c FALSE, disable AuxClk output.
// ==========================================================================
PUBLIC VOID hal_SysAuxClkOut(BOOL enable)
{
    UINT32 status = hal_SysEnterCriticalSection();
    if (enable)
    {
        if (g_halSysAuxClkUserNum == 0)
        {
            hwp_sysCtrl->Cfg_Clk_Auxclk = SYS_CTRL_AUXCLK_EN_ENABLE;
        }
        g_halSysAuxClkUserNum++;
    }
    else
    {
        if (g_halSysAuxClkUserNum > 0)
        {
            g_halSysAuxClkUserNum--;
            if (g_halSysAuxClkUserNum == 0)
            {
                hwp_sysCtrl->Cfg_Clk_Auxclk = SYS_CTRL_AUXCLK_EN_DISABLE;
            }
        }
    }
    hal_SysExitCriticalSection(status);
}


// ==========================================================================
// hal_Sys32kClkOut
// --------------------------------------------------------------------------
/// Manage the output of the 32K Clock, to an external device
/// @param enable If \c TRUE, enable 32K clock output.
///               If \c FALSE, disable 32K clock output.
// ==========================================================================
PUBLIC VOID hal_Sys32kClkOut(BOOL enable)
{
    HAL_ASSERT(g_halCfg->useClk32k == TRUE, "32K clock is not configured");

    UINT32 status = hal_SysEnterCriticalSection();
    if (enable)
    {
        if (g_halSys32kClkUserNum == 0)
        {
            // Setup the pin as 32K clock output
            hwp_configRegs->Alt_mux_select =
                (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
                CFG_REGS_PWL1_CLK_32K;
        }
        g_halSys32kClkUserNum++;
    }
    else
    {
        if (g_halSys32kClkUserNum > 0)
        {
            g_halSys32kClkUserNum--;
            if (g_halSys32kClkUserNum == 0)
            {
                // Setup the pin as GPO (and low ouput has been
                // configured in hal_BoardSetup())
                hwp_configRegs->Alt_mux_select =
                    (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
                    CFG_REGS_PWL1_GPO_6;
            }
        }
    }
    hal_SysExitCriticalSection(status);
}



// =============================================================================
// hal_SysIrqPending
// -----------------------------------------------------------------------------
/// This function checks the IRQ pending status. It should be called in critical
/// section to know if some IRQ are pending and then decide to release the 
/// critical section.
/// @return \c TRUE if IRQ are pending.
///         \c FALSE Otherwise.
// =============================================================================
PUBLIC BOOL hal_SysIrqPending(VOID)
{
    if (hwp_sysIrq->cause != 0)
    {
        // IRQ are pending
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



// =============================================================================
// hal_SysGetResetCause
// -----------------------------------------------------------------------------
/// Get the cause of the last reset.
/// This function only returns the reset cause. It does not proceed to any test
/// to now if the boot was due to a press on the Power On button or because
/// the charger was plugged. Those tests are to be done by the user code, 
/// when this function returns HAL_SYS_RESET_CAUSE_ALARM.
/// @return The last reset cause
// =============================================================================
PUBLIC HAL_SYS_RESET_CAUSE_T hal_SysGetResetCause(VOID)
{
    // NOTE:
    // The checking order should comply with the priority of reset causes.
    // In case of multiple reset causes in place simultaneously, only the
    // reset cause with highest priority is returned.
    // E.g.,
    // If an alarm is triggered during the shutdown process, the system will
    // reboot after the shutdown process, for the wakeup signal is kept high
    // by the RTC alarm. After the system boots up, ALARM and RESTART causes
    // will be valid, but only ALARM should be returned.

    if (g_bootResetCause & SYS_CTRL_WATCHDOG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_WATCHDOG;
    }

    if (g_bootResetCause & SYS_CTRL_HOSTDEBUG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_HOST_DEBUG;
    }

    if (g_bootResetCause & SYS_CTRL_ALARMCAUSE_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_ALARM;
    }

    // Reset has the lowest priority
    if (g_bootResetCause & SYS_CTRL_GLOBALSOFT_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_RESTART;
    }

    // else
    return HAL_SYS_RESET_CAUSE_NORMAL;
}



// =============================================================================
// hal_SysUsbHostEnable
// -----------------------------------------------------------------------------
/// Enable or disable usb host.
/// This is useful to inform hal_SysProcessHostMonitor() that the USB Host functions
/// must be called to process the host commands.
/// @param enable \c TRUE to enable, \c FALSE to disable.
// =============================================================================
PROTECTED VOID hal_SysUsbHostEnable(BOOL enable)
{
#if (CHIP_HAS_USB == 1)
    g_halUsbHostEnable = enable;
#else
    enable             = enable;
#endif/* CHIP_HAS_USB */
}



// =============================================================================
// hal_SysProcessHostMonitor
// -----------------------------------------------------------------------------
/// This function is called in the idle task or in gdb loop.
/// It is used to execute the function processing possible commands
/// from the Host.
// =============================================================================
PUBLIC VOID hal_SysProcessHostMonitor(VOID)
{
    static BOOL locked = FALSE;

    if (locked)
    {
        return;
    }

    locked = TRUE;

    // Process commands from host monitor
#if 0 // tianwq: no usb monitor support
#if (CHIP_HAS_USB == 1)
    static BOOL gdbUsbOpened = FALSE;

    if (hal_GdbIsInGdbLoop())
    {
        if (!gdbUsbOpened)
        {
            hal_UsbReset(TRUE);
            hal_TimDelay(HAL_TICK1S);
            hal_UsbReset(FALSE);
            hal_UsbOpen(hal_HostUsbDeviceCallback);
            hal_HostUsbOpen();
            gdbUsbOpened = TRUE;
        }
        // Handle USB irqs if any
        hal_UsbHandleIrqInGdb();
    }

    if(g_halUsbHostEnable)
    {
#ifdef HAL_USB_DRIVER_DONT_USE_ROMED_CODE
        hal_HostUsbRecv();
        hal_HostUsbSend();
#else
        boot_HostUsbRecv();
        boot_HostUsbSend();
#endif // HAL_USB_DRIVER_DONT_USE_ROMED_CODE
    }
#endif
#endif
    locked = FALSE;
}



// =============================================================================
// hal_SysProcessIdle
// -----------------------------------------------------------------------------
/// This function is called in the idle task. 
/// It is used to execute in idle the function processing possible commands
/// from the Host, and thus, for example, call the executor...
// =============================================================================
PUBLIC VOID hal_SysProcessIdle(VOID)
{
    // Process commands from host
    hal_SysProcessHostMonitor();
    hal_HstRamClientIdle();
}



#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_SysQBitError
// -----------------------------------------------------------------------------
/// This function returns error on QBit due to clock switch mechanism.
/// If the error has to be compensated the error is cleared.
/// @param forceReset force to clear the error counter.
/// @return -1, 0 or 1 QBit error to be compensated.
// =============================================================================
PUBLIC INT32 hal_SysQBitError(BOOL forceReset)
{
    UINT32 status = hal_SysEnterCriticalSection();

    // return value
    INT32 returnVal = (g_halSysClockSwitchQBitError >=  288) ?  1 :
                      (g_halSysClockSwitchQBitError <= -288) ? -1 :
                      0;

    // update g_halSysClockSwitchQBitError
    if (forceReset)
    {
        // force reset
        // TODO : should we reset when the error is too big?
       g_halSysClockSwitchQBitError = 0;
    }
    else 
    {
        g_halSysClockSwitchQBitError -= (288*returnVal);
    }

    hal_SysExitCriticalSection(status);
    return returnVal;
}
#endif // (!CHIP_HAS_ASYNC_TCU)


#ifdef CHIP_CPU_IS_MIPS

// =============================================================================
// hal_SysWaitMicrosecond
// -----------------------------------------------------------------------------
/// This function wait some time in microseconds.
/// It calculates CPU instruction cycles needed, depending on current system frequency.
/// Note that it does NOT have critical section operations.
/// @param us Microseconds to wait.
// =============================================================================
PUBLIC VOID hal_SysWaitMicrosecond(UINT32 us)
{
// 1 (sltu) + 1 (addiu) + 3 (btnez) = 5 cycles
#define WAIT_US_LOOP_CYCLE 5

    register UINT32 counter = g_halSysSystemFreq/1000000*us/WAIT_US_LOOP_CYCLE;

    asm volatile("move $7, %0\n\t"
            "li $6, 0\n"
            "_counter_check:\n\t"
            "sltu $6, $7\n\t"
            "addiu $6, 1\n\t"
            "btnez _counter_check"
            : /* no output */ : "r"(counter) : "$6", "$7");
}


PUBLIC VOID hal_IsRunningInIrqNow(u32* K1)
{
	u32 tmp;
	if(K1==NULL)
		return ;

	asm volatile("move %0, $27":"=d"(tmp));
	*K1 = tmp;
}

VOID hal_SysExitCriticalSection(UINT32 status)
{
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
   *g_halSysCsAddress = status;
}


UINT32 hal_SysEnterCriticalSection(VOID)
{
    UINT32  myLocalVar = *g_halSysCsAddress;
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
    return myLocalVar;
}

PUBLIC VOLATILE UINT32* HAL_BOOT_DATA_INTERNAL g_bootSysCsAddress = (UINT32*)&hwp_sysIrq->sc;

// For library backward compatibility only. To be removed after all the libraries are updated.
PUBLIC VOLATILE UINT32* HAL_BOOT_DATA_INTERNAL g_halSysCsAddress = (UINT32*)&hwp_sysIrq->sc;
#else // arm
VOID hal_SysExitCriticalSection(UINT32 status)
{
}
UINT32 hal_SysEnterCriticalSection(VOID)
{
    return 0;
}
#endif


