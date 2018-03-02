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

extern VOID hal_abbEnablePa(BOOL on);
extern VOID hal_abbPdAll(BOOL on);
extern PROTECTED VOID hal_XCpuEnterSleep();
// =============================================================================
//  MACROS
// =============================================================================
#ifdef FPGA
#define HAL_SYS_MAX_SPEED   HAL_SYS_FREQ_52M
#else
#define HAL_SYS_MAX_SPEED   CHIP_MAX_SPEED
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
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

#if 1//def CT_NO_DEBUG
#define HAL_SYS_CLK_EVENT(x)  
#else
#define HAL_SYS_CLK_EVENT(x)  hal_HstSendEvent(BOOT_EVENT, x)
#endif

#define HAL_SYS_RF_IF_CTRL_APB_ENTER(prev_value)    do{ \
                if((hwp_sysCtrl->RF_IF_CTRL & SYS_CTRL_RF_REG_SRC_SEL) == SYS_CTRL_RF_REG_SRC_SEL_SPI)\
                {\
                    prev_value = SYS_CTRL_RF_REG_SRC_SEL_SPI;\
                    hwp_sysCtrl->RF_IF_CTRL = SYS_CTRL_RF_REG_SRC_SEL_APB;\
                }\
                else\
                {\
                    prev_value = SYS_CTRL_RF_REG_SRC_SEL_APB;\
                }\
            }while(0)


#define HAL_SYS_RF_IF_CTRL_APB_EXIT(prev_value)    do{ \
            if((prev_value) != hwp_sysCtrl->RF_IF_CTRL)\
                hwp_sysCtrl->RF_IF_CTRL = prev_value;\
            }while(0)

#define HAL_SYS_PLL_SRC_ADPLL()  (0 ==(hwp_rf_if->adpll_setting1 & RF_IF_SYS_PLL_CLK_SRC_SEL_AUPLL))
#define HAL_SYS_PLL_SRC_AUPLL()  (0 !=(hwp_rf_if->adpll_setting1 & RF_IF_SYS_PLL_CLK_SRC_SEL_AUPLL))
// =============================================================================
// HAL_SYS_PLL_FREQ_T
// -----------------------------------------------------------------------------
/// This list the frequence of sys pll (adpll < 200Mhz) 
// =============================================================================

typedef enum
{
    HAL_SYS_PLL_FREQ_32K = 0,

    // 28M * N  for BT
    HAL_SYS_PLL_FREQ_28M      = 28000000,
    HAL_SYS_PLL_FREQ_56M      = 56000000,
    HAL_SYS_PLL_FREQ_84M      = 84000000,
    HAL_SYS_PLL_FREQ_112M     = 112000000,
    HAL_SYS_PLL_FREQ_140M     = 140000000,
    HAL_SYS_PLL_FREQ_168M     = 168000000,
    HAL_SYS_PLL_FREQ_196M     = 196000000,

    // 48M * N  for USB
    HAL_SYS_PLL_FREQ_48M      = 48000000,
    HAL_SYS_PLL_FREQ_96M      = 96000000,
    HAL_SYS_PLL_FREQ_144M     = 144000000,
    HAL_SYS_PLL_FREQ_192M     = 192000000,

    HAL_SYS_PLL_FREQ_180M     = 180000000,

    HAL_SYS_PLL_FREQ_UNKNOWN = 0,

} HAL_SYS_PLL_FREQ_T;


// =============================================================================
// HAL_SYS_PLL_FREQ_T
// -----------------------------------------------------------------------------
    //Fomula is freq*2^27/crystal_clk"
// =============================================================================


typedef enum
{
    HAL_SYS_ADPLL_SDM_32K = 0,

    // 28M * N  for BT
    HAL_SYS_ADPLL_SDM_28M      = 0X89D89D8,
    HAL_SYS_ADPLL_SDM_56M      = 0X113B13B1,
    HAL_SYS_ADPLL_SDM_84M      = 0X19D89D89,
    HAL_SYS_ADPLL_SDM_112M     = 0X22762762,
    HAL_SYS_ADPLL_SDM_140M     = 0X2B13B13B,
    HAL_SYS_ADPLL_SDM_168M     = 0X33B13B13,
    HAL_SYS_ADPLL_SDM_196M     = 0X3C4EC4EC,

    // 48M * N  for USB
    HAL_SYS_ADPLL_SDM_48M      = 0XEC4EC4E,
    HAL_SYS_ADPLL_SDM_96M      = 0X1D89D89D,
    HAL_SYS_ADPLL_SDM_144M     = 0X2C4EC4EC,
    HAL_SYS_ADPLL_SDM_192M     = 0X3B13B13B,


    HAL_SYS_ADPLL_SDM_UNKNOWN = 0,

} HAL_SYS_ADPLL_SDM_T;


typedef enum
{
    HAL_SYS_AUPLL_SDM_32K = 0,

    // 28M * N  for BT
    HAL_SYS_AUPLL_SDM_28M      = 0X89D89D8,
    HAL_SYS_AUPLL_SDM_56M      = 0X113B13B1,
    HAL_SYS_AUPLL_SDM_84M      = 0X19D89D89,
    HAL_SYS_AUPLL_SDM_112M     = 0X22762762,
    HAL_SYS_AUPLL_SDM_140M     = 0X2B13B13B,
    HAL_SYS_AUPLL_SDM_168M     = 0X33B13B13,
    HAL_SYS_AUPLL_SDM_196M     = 0X3C4EC4EC,

    // 48M * N  for USB
    HAL_SYS_AUPLL_SDM_48M      = 0XEC4EC4E,
    HAL_SYS_AUPLL_SDM_96M      = 0X1D89D89D,
    HAL_SYS_AUPLL_SDM_144M     = 0X2C4EC4EC,
    HAL_SYS_AUPLL_SDM_192M     = 0X3B13B13B,


    HAL_SYS_AUPLL_SDM_UNKNOWN = 0,

} HAL_SYS_AUPLL_SDM_T;


typedef VOID (*HAL_SYS_PLL_FREQ_CHANGE_HANDLER_T)(HAL_SYS_PLL_FREQ_T);

typedef struct
{
    BOOL adpll_on;
    BOOL adpll_selected;
    HAL_SYS_PLL_FREQ_T adpll_freq;
    BOOL aupll_on;
    BOOL aupll_selected;
    HAL_SYS_PLL_FREQ_T aupll_freq;
}HAL_SYS_PLL_CONFIG;
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

//PRIVATE UINT32 g_halSysAuxClkUserNum = 0;

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


PROTECTED HAL_SYS_PLL_FREQ_T g_halSysPllFreqRequest[HAL_SYS_PLL_TOTAL_USERS_QTY] = {0};

PRIVATE HAL_SYS_PLL_FREQ_CHANGE_HANDLER_T g_halSysPllFreqHandlerRegistry[HAL_SYS_PLL_TOTAL_USERS_QTY] = {NULL};


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

PRIVATE VOID hal_SysChangeSystemClockByOneStep(HAL_SYS_FREQ_T fastClockSetup);

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
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
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
#endif
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
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)    
#if (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->BB_Rst_Set = SYS_CTRL_SET_RST_BCPU;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#else // (CHIP_HAS_ASYNC_BCPU)
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_UNLOCK;
    hwp_bbCtrl->BB_Rst_Set = BB_CTRL_SET_RST_BCPU;
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_LOCK;
#endif // (!CHIP_HAS_ASYNC_BCPU)
#endif
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
    UINT32 callerAddr;

    SXS_GET_RA(&callerAddr);
    HAL_SYS_CLK_EVENT(0x20170400);
    HAL_SYS_CLK_EVENT(userId);
    HAL_SYS_CLK_EVENT(minFreq);
    HAL_SYS_CLK_EVENT(callerAddr);

    // Frequency clamping
    if (requiredFreq > HAL_SYS_MAX_SPEED)
    {
        HAL_TRACE(HAL_LPS_TRC, 0, "hal_SysRequestFreq clamped the request for a %d frequency. %d", requiredFreq, HAL_SYS_MAX_SPEED);
        requiredFreq = HAL_SYS_MAX_SPEED;
    }

    HAL_TRACE(HAL_LPS_TRC,0,"hal_SysRequestFreq %d %d from 0x%lx",userId,minFreq,handler); 

    // Keep a coherent table
    status = hal_SysEnterSC();

    // Convert freq and set in the registry holding the frequency
    // required by each module
    hal_SysFreqScalSet(userId, hal_SysSF2FSF(requiredFreq));

    g_halSysFreqHandlerRegistry[userId] = handler;

    hal_SysUpdateSystemFrequency();

    hal_SysExitSC(status);
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
#if 0//(CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
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

        case HAL_SYS_FREQ_144M:
            convValue =  HAL_SYS_FREQ_SCAL_144M;
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

        case HAL_SYS_FREQ_SCAL_144M:
            convValue =  HAL_SYS_FREQ_144M;
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
    UINT32 t_adpll_setting1;

    // Check that the alarm has finished to be set
    // if an alarm has been programmed before shutting 
    // down
    while (hwp_calendar->cmd & CALENDAR_ALARM_ENABLE_SET);

    UINT32 status __attribute__((unused))  = hal_SysEnterSC();

    hal_HstSendEvent(SYS_EVENT,0x1a11dead);
    hal_abbEnablePa(FALSE);
    hal_abbPdAll(TRUE);
    // Enter power-save mode
    pmd_Close();

    //5856E bug:  softreset fail in adpll 
    //switch to non-pll clock before shutdown
    hwp_sysCtrl->RF_IF_CTRL = 0;
	t_adpll_setting1 = hwp_rf_if->adpll_setting1;
    t_adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_DIV_MASK;
    t_adpll_setting1 |= (RF_IF_SYS_PLL_CLK_DIV(10) & RF_IF_SYS_PLL_CLK_DIV_MASK);
    t_adpll_setting1 |= RF_IF_SYS_PLL_CLK_LD_CFG;
    hwp_rf_if->adpll_setting1 = t_adpll_setting1;
    hwp_rf_if->adpll_setting1 = RF_IF_SYS_CLK_SEL_FAST | RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;

    hwp_pmuc->xtal_cfg_ctrl &= ~(PMUC_XTAL_REG_BIT_LV_MASK|PMUC_XTAL_LP_MODE_EN_REG);
    hwp_pmuc->xtal_cfg_ctrl |= PMUC_PU_XTAL_26M_DR;
    hwp_pmuc->pwr_sys_ctrl |=  PMUC_CLK_RAW_SEL;
    hwp_pmuc->pm_fsm_frc_ctrl1 &= ~(PMUC_MEM_GRP1_ON|PMUC_MEM_GRP2_ON |PMUC_MEM_GRP3_ON);
    pmd_SetPowerMode(PMD_LOWPOWER3);
    hwp_pmuc->ldo_cfg &= ~PMUC_PU_VIO;//cut Vio before sleep to disable LED after wake up
    hal_XCpuEnterSleep();

}


// =============================================================================
// hal_SysSoftReset
// -----------------------------------------------------------------------------
/// Does a soft reset on the chip.
/// Generally, you should NOT use this function, use hal_SysRestart() instead.
// =============================================================================
PUBLIC VOID hal_SysSoftReset(VOID)
{
    UINT32 t_adpll_setting1;
    UINT32 status __attribute__((unused))  = hal_SysEnterSC();

#ifdef __SPIFLSH_SUPPORT_SOFTRESET__
	memd_FlashRst();
#endif
	hal_HstSendEvent(BOOT_EVENT,0x77887788);

    //5856E bug:  softreset fail in adpll 
    //switch to non-pll clock before reset
    hwp_sysCtrl->RF_IF_CTRL = 0;
	t_adpll_setting1 = hwp_rf_if->adpll_setting1;
    t_adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_DIV_MASK;
    t_adpll_setting1 |= (RF_IF_SYS_PLL_CLK_DIV(10) & RF_IF_SYS_PLL_CLK_DIV_MASK);
    t_adpll_setting1 |= RF_IF_SYS_PLL_CLK_LD_CFG;
    hwp_rf_if->adpll_setting1 = t_adpll_setting1;
    hwp_rf_if->adpll_setting1 = RF_IF_SYS_CLK_SEL_FAST | RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;

    hwp_sysCtrl->per_soft_reset1 &= ~(SYS_CTRL_GLOBAL_RSTB);


    while (1); // We die here ...

    // hal_SysExitCriticalSection(status);

}

PUBLIC VOID hal_SysSetBootMode(UINT8 mode)
{
#if 0
    hwp_sysCtrl->Reset_Cause &= SYS_CTRL_BOOT_MODE_MASK;
    hwp_sysCtrl->Reset_Cause |= SYS_CTRL_BOOT_MODE(mode);
#endif
}

// =============================================================================
// hal_SysRestart
// -----------------------------------------------------------------------------
/// A call to this function reboots the system.
/// First, this function does a power cycle on USB
// =============================================================================
PUBLIC VOID hal_SysRestart(VOID)
{
#if 0//(CHIP_HAS_USB == 1)
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
    if(0)
    {
    }
    //aupll is selected
    else if(HAL_SYS_PLL_SRC_AUPLL() && (hwp_abb->aupll_setting3 & ABB_AUPLL_LOCK))
    {
        return TRUE;
    }
    //adpll is selected
    else if((hwp_rf_if->adpll_status & RF_IF_ADPLL_LOCK_DET) && (hwp_rf_if->adpll_status & RF_IF_ADPLL_LOCK_FLAG)) //boot_usbinitclockenable use RF_IF_ADPLL_LOCK_FLAG
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// =============================================================================
// hal_SysGetAdPLLFreq
// -----------------------------------------------------------------------------
/// Return ad PLL Frequence 
/// @return 
// =============================================================================
PUBLIC HAL_SYS_PLL_FREQ_T hal_SysGetAdPLLFreq(VOID)
{
    HAL_SYS_PLL_FREQ_T pll_freq = HAL_SYS_PLL_FREQ_UNKNOWN;

    UINT32 reg_RF_IF_CTRL;
    HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);

    switch(hwp_rf_if->adpll_sdm_setting1 & RF_IF_ADPLL_SDM_FREQ_REG_MASK)
    {
        case HAL_SYS_ADPLL_SDM_32K: pll_freq = HAL_SYS_PLL_FREQ_32K; break;
        
        case HAL_SYS_ADPLL_SDM_28M: pll_freq = HAL_SYS_PLL_FREQ_28M; break;
        case HAL_SYS_ADPLL_SDM_56M: pll_freq = HAL_SYS_PLL_FREQ_56M; break;
        case HAL_SYS_ADPLL_SDM_84M: pll_freq = HAL_SYS_PLL_FREQ_84M; break;
        case HAL_SYS_ADPLL_SDM_112M: pll_freq = HAL_SYS_PLL_FREQ_112M; break;
        case HAL_SYS_ADPLL_SDM_140M: pll_freq = HAL_SYS_PLL_FREQ_140M; break;
        case HAL_SYS_ADPLL_SDM_168M: pll_freq = HAL_SYS_PLL_FREQ_168M; break;
        case HAL_SYS_ADPLL_SDM_196M: pll_freq = HAL_SYS_PLL_FREQ_196M; break;

        case HAL_SYS_ADPLL_SDM_48M: pll_freq = HAL_SYS_PLL_FREQ_48M; break;
        case HAL_SYS_ADPLL_SDM_96M: pll_freq = HAL_SYS_PLL_FREQ_96M; break;
        case HAL_SYS_ADPLL_SDM_144M: pll_freq = HAL_SYS_PLL_FREQ_144M; break;
        case HAL_SYS_ADPLL_SDM_192M: pll_freq = HAL_SYS_PLL_FREQ_192M; break;
        

        default:
            HAL_ASSERT(FALSE, "GetPLLFreq Unsupport freq");
            break;
    }
    HAL_SYS_RF_IF_CTRL_APB_EXIT(reg_RF_IF_CTRL);

    return pll_freq;
}

PROTECTED VOID hal_SysSetAdPllFreq(HAL_SYS_PLL_FREQ_T freq)
{
    UINT32 i;
    HAL_SYS_ADPLL_SDM_T freq_reg = HAL_SYS_ADPLL_SDM_UNKNOWN;
  
    //"To be used when adpll_sdm_freq_dr=1.
    HAL_SYS_CLK_EVENT(0x201704aa);
    HAL_SYS_CLK_EVENT(freq);
    switch(freq)
    {
        case HAL_SYS_PLL_FREQ_28M: freq_reg = HAL_SYS_ADPLL_SDM_28M; break;
        case HAL_SYS_PLL_FREQ_56M: freq_reg = HAL_SYS_ADPLL_SDM_56M; break;
        case HAL_SYS_PLL_FREQ_84M: freq_reg = HAL_SYS_ADPLL_SDM_84M; break;
        case HAL_SYS_PLL_FREQ_112M: freq_reg = HAL_SYS_ADPLL_SDM_112M; break;
        case HAL_SYS_PLL_FREQ_140M: freq_reg = HAL_SYS_ADPLL_SDM_140M; break;
        case HAL_SYS_PLL_FREQ_168M: freq_reg = HAL_SYS_ADPLL_SDM_168M; break;
        case HAL_SYS_PLL_FREQ_196M: freq_reg = HAL_SYS_ADPLL_SDM_196M; break;

        case HAL_SYS_PLL_FREQ_48M: freq_reg = HAL_SYS_ADPLL_SDM_48M; break;
        case HAL_SYS_PLL_FREQ_96M: freq_reg = HAL_SYS_ADPLL_SDM_96M; break;
        case HAL_SYS_PLL_FREQ_144M: freq_reg = HAL_SYS_ADPLL_SDM_144M; break;
        case HAL_SYS_PLL_FREQ_192M: freq_reg = HAL_SYS_ADPLL_SDM_192M; break;

        default:
            HAL_ASSERT(FALSE, "SetPllFreq Unsupport freq");
            break;
    }

    HAL_SYS_CLK_EVENT(freq_reg);

    hwp_rf_if->adpll_sdm_setting1 = RF_IF_ADPLL_SDM_FREQ_REG(freq_reg);
    hwp_rf_if->adpll_sdm_setting0 |=RF_IF_ADPLL_SDM_FREQ_DR;



    //*****start SDM***********************************//  
	//5. hwp_rf_if->adpll_sdm_setting0[adpll_sdm_resetn_reg] = 0
    hwp_rf_if->adpll_sdm_setting0 &=~RF_IF_ADPLL_SDM_RESETN_REG;
	
	//6.[Xian]: 请在此处添加adpll_sdm_clk_sel设置，sdm初始时钟应选择为reference clock
    hwp_rf_if->adpll_sdm_setting0 |=RF_IF_ADPLL_SDM_CLK_SEL_0;
    hwp_rf_if->adpll_sdm_setting0 |=RF_IF_ADPLL_SDM_CLK_SEL_1;

	//7．hwp_rf_if->adpll_sdm_setting0[adpll_sdm_resetn_dr] = 1  //reset sdm并释放
    hwp_rf_if->adpll_sdm_setting0 |=RF_IF_ADPLL_SDM_RESETN_DR;
	
	for(i=0;i<52;i++)
	{
    asm volatile("nop");
	}

    //8. hwp_rf_if->adpll_sdm_setting0[adpll_sdm_resetn_reg] = 1
    hwp_rf_if->adpll_sdm_setting0 |=RF_IF_ADPLL_SDM_RESETN_REG;
	
	//9.[Xian]: 请在此处添加adpll_sdm_clk_sel设置，sdm reset释放后时钟应选择为fbc clock
	hwp_rf_if->adpll_sdm_setting0 &=~RF_IF_ADPLL_SDM_CLK_SEL_0;
    hwp_rf_if->adpll_sdm_setting0 &=~RF_IF_ADPLL_SDM_CLK_SEL_1;

	//3. Rf_if.adpll_setting1[adpll_cpu_sel] = 1
    hwp_rf_if->adpll_setting1 |=RF_IF_ADPLL_CPU_SEL;
	
	//NEW*********NOP*NOP*********************************//
	for(i=0;i<52;i++)
	{
    asm volatile("nop");
	}
	
    //4. Rf_if.adpll_setting1[adpll_clk_cpu_en] = 1
    hwp_rf_if->adpll_setting1 |=RF_IF_ADPLL_CLK_CPU_EN;



    HAL_SYS_CLK_EVENT(0x201704bb);

}


PUBLIC VOID hal_SysPowerOnAdpll(VOID)
{
    UINT32 i;
    
    hwp_pmu_intf->xtal_ctrl |=PMU_INTF_XTAL_CLK_ADPLL_EN_LV; 

    //*****PU ADPLL & enable clock out******************//
	//设置lock延迟为100us
	hwp_rf_if->adpll_setting1 &= ~RF_IF_RESET_ADPLL_LOCK_DELAY_MASK;
	hwp_rf_if->adpll_setting1 |= RF_IF_RESET_ADPLL_LOCK_DELAY(1);
	
    //NEW 增加PLL RESET
    hwp_rf_if->adpll_setting0 &=~RF_IF_PU_ADPLL_REG;
    hwp_rf_if->adpll_setting0 &=~RF_IF_ADPLL_RSTN_REG;  
	
    //1．Rf_if.adpll_setting0[pu_adpll_dr] = 1
    hwp_rf_if->adpll_setting0 |=RF_IF_PU_ADPLL_DR;
	hwp_rf_if->adpll_setting0 |=RF_IF_ADPLL_RSTN_DR;    
	
    //2. Rf_if.adpll_setting0[pu_adpll_reg] = 1
    hwp_rf_if->adpll_setting0 |=RF_IF_PU_ADPLL_REG;
	
	//NEW*********NOP*NOP*********************************//
	
	for(i=0;i<52;i++)
	{
    asm volatile("nop");
	}
	
	hwp_rf_if->adpll_setting0 |=RF_IF_ADPLL_RSTN_REG;  

}

PUBLIC VOID hal_SysPowerOffAdpll(VOID)
{
#if (0)
        //There is an inner digen which is write to 1 
        //when outter digen is 1 and clock is updated.
        //So we update clock to ensure inner digen is equal out digen here.
        //Or xcpu will die for no clock......
        #if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
        hwp_sysCtrl->Cfg_Clk_Sys |= SYS_CTRL_FORCE_DIV_UPDATE;
        hwp_sysCtrl->Cfg_Clk_SpiFlash = hwp_sysCtrl->Cfg_Clk_SpiFlash;
        hwp_sysCtrl->Cfg_Clk_Voc = hwp_sysCtrl->Cfg_Clk_Voc;
        #endif
        //DisablePLL
        hwp_sysCtrl->Pll_Ctrl &= (~SYS_CTRL_PLL_ENABLE_ENABLE);
        //Close pll div
        //3 Switch to dig_52M ERROR when there is no pll and bypass is set 0, so we MUST set it 1 here
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_BYPASS_BYPASS;
#else /* 0 */
        hwp_rf_if->adpll_setting0 &= (~(RF_IF_PU_ADPLL_REG ));
        hwp_rf_if->adpll_setting0 |=  RF_IF_PU_ADPLL_DR;
        hwp_rf_if->adpll_setting0 &= (~(RF_IF_ADPLL_RSTN_REG | RF_IF_ADPLL_RSTN_DR));
        hwp_pmu_intf->xtal_ctrl &= ~PMU_INTF_XTAL_CLK_ADPLL_EN_LV; 

        hwp_rf_if->adpll_setting1 &= ~RF_IF_ADPLL_CLK_CPU_EN;

#endif /* 0 */

}

#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__) && defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
PROTECTED HAL_SYS_PLL_FREQ_T hal_SysGetAuPllFreq(VOID)
{
    UINT32 freq_reg;
    HAL_SYS_PLL_FREQ_T freq;

    freq_reg  = hwp_abb->sdmfreq1 & 0XF000;

    switch(freq_reg)
    {
        case 0XF000: 
            freq = HAL_SYS_PLL_FREQ_196M;
            break;
            
        case 0XD000:
            freq = HAL_SYS_PLL_FREQ_180M;
            break;

        default:
            freq = HAL_SYS_PLL_FREQ_32K;
            HAL_ASSERT(FALSE,"Unknow frequence");
    }

    return freq;
}
PROTECTED VOID hal_SysSetAuPllFreq(HAL_SYS_PLL_FREQ_T freq)
{
    UINT32 i;
    HAL_SYS_AUPLL_SDM_T freq_reg = HAL_SYS_ADPLL_SDM_UNKNOWN;
  
    //"To be used when aupll_sdm_freq_dr=1.
    HAL_SYS_CLK_EVENT(0x201705aa);
    HAL_SYS_CLK_EVENT(freq);
    switch(freq)
    {
        case HAL_SYS_PLL_FREQ_28M: freq_reg = HAL_SYS_AUPLL_SDM_28M; break;
        case HAL_SYS_PLL_FREQ_56M: freq_reg = HAL_SYS_AUPLL_SDM_56M; break;
        case HAL_SYS_PLL_FREQ_84M: freq_reg = HAL_SYS_AUPLL_SDM_84M; break;
        case HAL_SYS_PLL_FREQ_112M: freq_reg = HAL_SYS_AUPLL_SDM_112M; break;
        case HAL_SYS_PLL_FREQ_140M: freq_reg = HAL_SYS_AUPLL_SDM_140M; break;
        case HAL_SYS_PLL_FREQ_168M: freq_reg = HAL_SYS_AUPLL_SDM_168M; break;
        case HAL_SYS_PLL_FREQ_196M: freq_reg = HAL_SYS_AUPLL_SDM_196M; break;

        case HAL_SYS_PLL_FREQ_48M: freq_reg = HAL_SYS_AUPLL_SDM_48M; break;
        case HAL_SYS_PLL_FREQ_96M: freq_reg = HAL_SYS_AUPLL_SDM_96M; break;
        case HAL_SYS_PLL_FREQ_144M: freq_reg = HAL_SYS_AUPLL_SDM_144M; break;
        case HAL_SYS_PLL_FREQ_192M: freq_reg = HAL_SYS_AUPLL_SDM_192M; break;

        default:
            HAL_ASSERT(FALSE, "SetPllFreq Unsupport freq");
            break;
    }

    HAL_SYS_CLK_EVENT(freq_reg);

    hwp_abb->sdmfreq1 = (freq_reg >>16) & 0XFFFF;
    hwp_abb->sdmfreq2 = freq_reg & 0XFFFF;


    //*****start SDM***********************************//  
	//5. hwp_abb->aupll_sdm_setting0[aupll_sdm_resetn_reg] = 0
    hwp_abb->sdmsetting &=~ABB_SDM_RESETN;
	

	//7．hwp_rf_if->aupll_sdm_setting0[aupll_sdm_resetn_dr] = 1  //reset sdm并释放
   // hwp_abb->aupll_sdm_setting0 |=RF_IF_AUPLL_SDM_RESETN_DR;
	
	for(i=0;i<52;i++)
	{
		asm volatile("nop");
	}



    //8. hwp_abb->aupll_sdm_setting0[aupll_sdm_resetn_reg] = 1
    hwp_abb->sdmsetting |=ABB_SDM_RESETN;




	//3. Rf_if.aupll_setting1[aupll_cpu_sel] = 1
    //hwp_abb->aupll_setting1 |=RF_IF_AUPLL_CPU_SEL;
	
	//NEW*********NOP*NOP*********************************//
	for(i=0;i<52;i++)
	{
    	asm volatile("nop");
	}
	
    //4. Rf_if.aupll_setting1[aupll_clk_cpu_en] = 1
    hwp_abb->aupll_setting2 |=ABB_AUPLL_CLKOUT_EN(7);



    HAL_SYS_CLK_EVENT(0x201705bb);

}


PUBLIC VOID hal_SysPowerOnAupll(VOID)
{
    UINT32 i;
    
    hwp_pmu_intf->xtal_ctrl |=PMU_INTF_XTAL_CLK_AUPLL_EN_LV; 

    //*****PU AUPLL & enable clock out******************//
	//设置lock延迟为100us
	//hwp_abb->aupll_setting1 &= ~RF_IF_RESET_AUPLL_LOCK_DELAY_MASK;
	//hwp_abb->aupll_setting1 |= RF_IF_RESET_AUPLL_LOCK_DELAY(1);
	
    //NEW 增加PLL RESET
    hwp_abb->Power_Ctrl |=ABB_PU_ALL;
	
    hwp_abb->aupll_setting2 |=ABB_PU_AUPLL;
	
    //hwp_abb->aupll_setting0 &=~RF_IF_AUPLL_RSTN_REG;  
	
    //1．Rf_if.aupll_setting0[pu_aupll_dr] = 1
   // hwp_abb->aupll_setting0 |=RF_IF_PU_AUPLL_DR;
	//hwp_abb->aupll_setting0 |=RF_IF_AUPLL_RSTN_DR;    
	
    //2. Rf_if.aupll_setting0[pu_aupll_reg] = 1
    //hwp_abb->aupll_setting0 |=RF_IF_PU_AUPLL_REG;
	
	//NEW*********NOP*NOP*********************************//
	
	for(i=0;i<52;i++)
	{
    asm volatile("nop");
	}
    hwp_abb->aupll_setting2 |=ABB_AUPLL_RSTB;
	
	//hwp_abb->aupll_setting0 |=RF_IF_AUPLL_RSTN_REG;  

}

PUBLIC VOID hal_SysPowerOffAupll(VOID)
{
	hwp_abb->aupll_setting2 &=~ABB_PU_AUPLL;

	hwp_abb->aupll_setting2 &=~ABB_AUPLL_RSTB;

	hwp_abb->aupll_setting2 &= ~ABB_AUPLL_CLKOUT_EN_MASK;
	hwp_pmu_intf->xtal_ctrl &= ~PMU_INTF_XTAL_CLK_AUPLL_EN_LV; 

       // hwp_abb->aupll_setting0 &= (~(RF_IF_PU_AUPLL_REG ));
      //  hwp_abb->aupll_setting0 |=  RF_IF_PU_AUPLL_DR;
      //  hwp_abb->aupll_setting0 &= (~(RF_IF_AUPLL_RSTN_REG | RF_IF_AUPLL_RSTN_DR));

       // hwp_abb->aupll_setting1 &= ~RF_IF_AUPLL_CLK_CPU_EN;
}

/*
   return TRUE if aupll is working for some modules
*/
PUBLIC BOOL hal_SysAupllWorkForAudio(VOID)
{
    //If aupll is off, aif/codec/codec_simu maybe use 26M/doubler
    if((ABB_PU_AUPLL | ABB_AUPLL_RSTB) != (hwp_abb->aupll_setting2 & (ABB_PU_AUPLL | ABB_AUPLL_RSTB)))
    {
        return FALSE;
    }
    //aupll is on, check aif,codec_ana,codec_dig
    else if((hwp_abb->Power_Ctrl & (ABB_PU_DAC_L | ABB_PU_DAC_R | ABB_PU_ADC_L | ABB_PU_ADC_R))
            ||(hwp_audio_codec->codec_cfg & AUDIO_CODEC_S_CODEC_EN)
            ||(hwp_aif->ctrl &AIF_ENABLE_ENABLE)
            )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


PUBLIC VOID hal_SysPowerOnDoubler(VOID)
{
    // Why doubler need set  PMU_INTF_XTAL_CLK_AUPLL_EN_LV  1
    hwp_pmu_intf->xtal_ctrl |= PMU_INTF_XTAL_CLK_AUPLL_EN_LV | PMU_INTF_XTAL_CLK_BB_EN_LV; 
    hwp_abb->clk_doubler_cfg |= ABB_PU_CLKDBLR;
}


PROTECTED HAL_SYS_PLL_FREQ_T hal_SysGetPLLFreq(VOID)
{
    HAL_SYS_PLL_FREQ_T ret = HAL_SYS_PLL_FREQ_32K;
    
	if(HAL_SYS_PLL_SRC_ADPLL())
	{
		ret = hal_SysGetAdPLLFreq();
	}
#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__) && defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)    
	else if(HAL_SYS_PLL_SRC_AUPLL())
	{
		ret = hal_SysGetAuPllFreq();
	}
#endif    
	else
	{
		HAL_ASSERT(FALSE, "No pll src selected!");
	}

	return ret;
}
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
PROTECTED VOID hal_SysRequestPll(HAL_SYS_PLL_USERS_T userId, HAL_SYS_PLL_FREQ_T freq, HAL_SYS_PLL_FREQ_CHANGE_HANDLER_T cbk)
{
    UINT32 status = hal_SysEnterCriticalSection();
    UINT16 i;
    HAL_SYS_PLL_FREQ_T min_pll_freq;
    UINT32 reg_RF_IF_CTRL;
    HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);
    
    HAL_SYS_CLK_EVENT(0x20160613);


    g_halSysPllFreqRequest[userId] = freq;
    g_halSysPllFreqHandlerRegistry[userId] = cbk;

    if(HAL_SYS_PLL_FREQ_32K != g_halSysPllFreqRequest[HAL_SYS_PLL_USB])
    {
        min_pll_freq = HAL_SYS_PLL_FREQ_48M;
        
        for(i = 0; i < HAL_SYS_PLL_TOTAL_USERS_QTY; i++)
        {
            while(min_pll_freq < g_halSysPllFreqRequest[i]) 
                min_pll_freq += HAL_SYS_PLL_FREQ_48M;
        }
    }
    else if(HAL_SYS_PLL_FREQ_32K != g_halSysPllFreqRequest[HAL_SYS_PLL_BT])
    {
        min_pll_freq = HAL_SYS_PLL_FREQ_28M;
        
        for(i = 0; i < HAL_SYS_PLL_TOTAL_USERS_QTY; i++)
        {
            while(min_pll_freq < g_halSysPllFreqRequest[i]) 
                min_pll_freq += HAL_SYS_PLL_FREQ_28M;
        }
    }
    else
    {
        min_pll_freq = HAL_SYS_PLL_FREQ_32K;
        
        for(i = 0; i < HAL_SYS_PLL_TOTAL_USERS_QTY; i++)
        {
            if(min_pll_freq < g_halSysPllFreqRequest[i]) 
                min_pll_freq = g_halSysPllFreqRequest[i];
        }
    }

    HAL_SYS_CLK_EVENT((userId <<16) | (freq / 1000000));
    HAL_SYS_CLK_EVENT(min_pll_freq);

    if(min_pll_freq > HAL_SYS_PLL_FREQ_32K)
    {
#ifdef __HAL_SYS_SWITCH_PLL_DYNAMIC__
        if(!hal_SysGetPLLLock())
        {

            //__HAL_SYS_USE_AUX_PLL_ALWAYS__
            //hal_AudEnableCodecAuxPll(on);

            hal_SysPowerOnAdpll();
        }
        
        hal_SysSetAdPllFreq(min_pll_freq);
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */
        
        while(!hal_SysGetPLLLock());
    }
    else
    {
#ifdef __HAL_SYS_SWITCH_PLL_DYNAMIC__
    	  hal_SysPowerOffAdpll();
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */
    }

    HAL_SYS_CLK_EVENT(0x20160614);
    HAL_SYS_RF_IF_CTRL_APB_EXIT(reg_RF_IF_CTRL);
    hal_SysExitCriticalSection(status);

}

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
                                                        HAL_SYS_PLL_CONFIG*    pllConfig)
{
    HAL_SYS_PLL_FREQ_T PllFreq;
    HAL_SYS_FREQ_T realFreq = fastClock;

#if (0)
    UINT32 PrimeNumOfPllFreq, PrimeNumOffastClock;
    //Calculate prime number if needs pll
    if(fastClock > HAL_SYS_FREQ_52M)
    {
        *sysCtrlRegConfig = 0;

        HAL_ASSERT((PllFreq % 1000000) == 0, "Frequence pll should be in MHz");
        HAL_ASSERT((fastClock % 1000000) == 0, "Frequence clk should be in MHz");


        PrimeNumOfPllFreq = PllFreq / 1000000; //to MHz
        PrimeNumOffastClock = fastClock / 1000000; //to MHz
        
        if(PrimeNumOfPllFreq > 2)
        {
            for(; (0 == (PrimeNumOfPllFreq & 1)); PrimeNumOfPllFreq>>1);
        }
        else
        {
            PrimeNumOfPllFreq = 1;
        }

        if(PrimeNumOffastClock > 2)
        {
            for(; (0 == (PrimeNumOffastClock & 1)); PrimeNumOffastClock>>1);
        }
        else
        {
            PrimeNumOffastClock = 1;
        }
        
    }
#else /* 0 */

#endif /* 0 */
    HAL_SYS_CLK_EVENT(0x20170403);
    HAL_SYS_CLK_EVENT(fastClock);

    memset(pllConfig, 0, sizeof(HAL_SYS_PLL_CONFIG));

    if(fastClock < HAL_SYS_FREQ_26M)
    {
        *sysCtrlRegConfig = RF_IF_HCLK_PCLK_DIV_DIV_1;
        realFreq = HAL_SYS_FREQ_26M;
        HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                    fastClock, realFreq);
    }
    else if(fastClock <= HAL_SYS_FREQ_52M)
    {
        *sysCtrlRegConfig = RF_IF_HCLK_PCLK_DIV_DIV_1;
    }
    else
    {
#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__) && defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
        //Switch to adpll for USB
        if(hal_SysGetRequestFreq(HAL_SYS_FREQ_USB) > HAL_SYS_FREQ_32K)
        {
            pllConfig->adpll_on = TRUE;
            pllConfig->adpll_selected = TRUE;
            pllConfig->adpll_freq = HAL_SYS_PLL_FREQ_144M;
            PllFreq = HAL_SYS_PLL_FREQ_144M;
        }
        else
        {
            pllConfig->aupll_on = TRUE;
            pllConfig->aupll_selected = TRUE;
            if(hal_SysAupllWorkForAudio())
            {
                pllConfig->aupll_freq = hal_SysGetAuPllFreq();
            }
            else
            {
                pllConfig->aupll_freq = HAL_SYS_PLL_FREQ_144M;
            }
            PllFreq = pllConfig->aupll_freq;
        }
#else
        pllConfig->adpll_on = TRUE;
        pllConfig->adpll_selected = TRUE;
        pllConfig->adpll_freq = HAL_SYS_PLL_FREQ_144M;
        PllFreq = HAL_SYS_PLL_FREQ_144M;
#endif /* __HAL_SYS_USE_AUX_PLL_ALWAYS__ && __HAL_SYS_SWITCH_PLL_DYNAMIC__*/


        HAL_SYS_CLK_EVENT((pllConfig->adpll_on<<24) |(pllConfig->adpll_selected<<16)
                           |(pllConfig->aupll_on<<8) | (pllConfig->aupll_selected<<0));
        HAL_SYS_CLK_EVENT(pllConfig->adpll_freq);
        HAL_SYS_CLK_EVENT(pllConfig->aupll_freq);

        
        /*
                fastClock = PllFreq * (PllClkDiv + 1) /HclkPclkDiv /16

                PllClkDiv <=15
                HclkPclkDiv = 1, 2, 4, 8, 16
        */

        //if(0 == (PllFreq % HAL_SYS_PLL_FREQ_48M))
        if(HAL_SYS_PLL_FREQ_196M == PllFreq)
        {
            switch(fastClock)
            {
                case HAL_SYS_FREQ_72M:
                case HAL_SYS_FREQ_78M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0x5) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_72M;
                    break;

                case HAL_SYS_FREQ_99M:
                case HAL_SYS_FREQ_104M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0x7) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_99M;
                    break;

                case HAL_SYS_FREQ_144M:
                case HAL_SYS_FREQ_156M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xA) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                case HAL_SYS_FREQ_208M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xA) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                default:
                    realFreq = HAL_SYS_FREQ_26M;
                    HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                                fastClock, realFreq);

                    break;

            }
        }
        else if(HAL_SYS_PLL_FREQ_180M == PllFreq)
        {
            switch(fastClock)
            {
                case HAL_SYS_FREQ_72M:
                case HAL_SYS_FREQ_78M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0x5) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_72M;
                    break;

                case HAL_SYS_FREQ_99M:
                case HAL_SYS_FREQ_104M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0x7) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_99M;
                    break;

                case HAL_SYS_FREQ_144M:
                case HAL_SYS_FREQ_156M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xB) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                case HAL_SYS_FREQ_208M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xB) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                default:
                    realFreq = HAL_SYS_FREQ_26M;
                    HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                                fastClock, realFreq);

                    break;

            }
        }        
        else if(HAL_SYS_PLL_FREQ_144M == PllFreq)
        {
            switch(fastClock)
            {
                case HAL_SYS_FREQ_72M:
                case HAL_SYS_FREQ_78M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0x7) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_72M;
                    break;

                case HAL_SYS_FREQ_99M:
                case HAL_SYS_FREQ_104M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xA) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_99M;
                    break;

                case HAL_SYS_FREQ_144M:
                case HAL_SYS_FREQ_156M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xF) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                case HAL_SYS_FREQ_208M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xF) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_144M;
                    break;

                default:
                    realFreq = HAL_SYS_FREQ_26M;
                    HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                                fastClock, realFreq);

                    break;

            }
        }
/*
        else if(HAL_SYS_PLL_FREQ_112M == PllFreq)
        {
            switch(fastClock)
            {
                case HAL_SYS_FREQ_77M:
                case HAL_SYS_FREQ_78M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xA) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_77M;
                    break;

                case HAL_SYS_FREQ_98M:
                case HAL_SYS_FREQ_104M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xD) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_98M;
                    break;

                case HAL_SYS_FREQ_112M:
                case HAL_SYS_FREQ_144M:
                case HAL_SYS_FREQ_156M:
                case HAL_SYS_FREQ_208M:
                    *sysCtrlRegConfig = RF_IF_SYS_PLL_CLK_DIV(0xF) | RF_IF_HCLK_PCLK_DIV_DIV_1;
                    realFreq = HAL_SYS_FREQ_112M;
                    HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                                fastClock, realFreq);
                    break;
                default:
                    realFreq = HAL_SYS_FREQ_26M;
                    HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                                fastClock, realFreq);
                    break;

            }
        }
*/
        else
        {
            HAL_ASSERT(FALSE, "FrequencyConfiguration usupport freq");
        }

        HAL_SYS_CLK_EVENT(0x20170404);
        HAL_SYS_CLK_EVENT(realFreq);
        HAL_SYS_CLK_EVENT(*sysCtrlRegConfig);

    }



    return realFreq;
}



#if (0)//CHIP_HAS_ASYNC_TCU)
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
#if 0
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
#endif
}

// ============================================================================
// hal_SysSetupPLL
// ----------------------------------------------------------------------------
/// Setup PLL at early system boot time.
/// enable doubler and pll
// ============================================================================
PUBLIC VOID HAL_BOOT_FUNC hal_SysSetupPLL(VOID)
{
    UINT32 reg_RF_IF_CTRL;

    HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);

    //Select 26M clk reference
    hwp_rf_if->adpll_setting0 &=~RF_IF_ADPLL_REFMULTI2_EN;

    HAL_SYS_CLK_EVENT(0x20170301);
    HAL_SYS_CLK_EVENT(hwp_rf_if->adpll_setting1);

    hal_SysPowerOnDoubler();
    
    //Set 52MHz clock default during system boot
    hwp_rf_if->adpll_setting1 &= ~RF_IF_SYS_FAST_CLK_SEL_MASK;
    hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_52M;

#if !defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
    hal_SysPowerOnAupll();
    hal_SysSetAuPllFreq(HAL_SYS_PLL_FREQ_144M);
#else
    hal_SysPowerOnAdpll();
    hal_SysSetAdPllFreq(HAL_SYS_PLL_FREQ_144M);
#endif
    while(!hal_SysGetPLLLock());
#endif

#if 0//(COMBINED_HOST ==0)
    //Set 52MHz clock  during system boot only for HCI version
    hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_52M;
#endif
    HAL_SYS_RF_IF_CTRL_APB_EXIT(reg_RF_IF_CTRL);
}

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
PUBLIC VOID HAL_BOOT_FUNC hal_SysEnableClkOut(uint8 div)
{
#if 0
    hwp_configRegs->audio_pd_set |= CFG_REGS_AU_REF_PD_N;

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
   
    hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_DEBUG | SYS_CTRL_CLKOUT_SEL_DIVIDER;
    hwp_sysCtrl->Clk_Other_Enable |= SYS_CTRL_ENABLE_OC_CLK_OUT | SYS_CTRL_ENABLE_OC_BCK ; 
    hwp_sysCtrl->Cfg_Clk_AudioBCK_Div =div;
	
    //wangxin temp
    hwp_debugPort->Dbg_Clk_Cfg = DEBUG_PORT_SEL_SOURCE(63)
                                | DEBUG_PORT_ENABLE_DBG_ENABLED;
	
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK; 
#endif
}
#endif

PUBLIC VOID HAL_BOOT_FUNC hal_SysEnable8KSampleRate(VOID)
{
#if 0
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Cfg_Clk_AudioBCK_Div =16;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
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
    //UINT32         sysFreq        = 0;
    //HAL_SYS_PLL_CONFIG pll_config;
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

    
    HAL_SYS_CLK_EVENT(0x20170302);
    UINT32 reg_RF_IF_CTRL;

    HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);
    HAL_SYS_CLK_EVENT(hwp_rf_if->adpll_setting1);
    HAL_SYS_RF_IF_CTRL_APB_EXIT(reg_RF_IF_CTRL);


    hal_SysChangeSystemClockByOneStep(fastClockSetup);

/*
    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClock, &sysFreq, &pll_config);

    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }

    // Do not set VCORE here for PMU is initialized after setting up sys clk
    {

        UINT32 status = hal_SysEnterCriticalSection();

        hal_SysExitCriticalSection(status);
    }

    if (g_halSysSystemFreq > fastClock)
    {  // when going to a faster clock need to setup everything first
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    // Do not set VCORE here for PMU is initialized after setting up sys clk

    // update global variables
    g_halSysSystemFreq = fastClock;
*/
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
    UINT32 i, sysFreq;
    HAL_SYS_PLL_CONFIG pll_config;


    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClockSetup, &sysFreq, &pll_config);

    if (g_halSysSystemFreq == fastClockSetup)
    {
        return;
    }

    HAL_SYS_CLK_EVENT(0x20170402);
    HAL_SYS_CLK_EVENT(fastClockSetup);
    HAL_SYS_CLK_EVENT(g_halSysSystemFreq);
    HAL_SYS_CLK_EVENT(fastClock);
    
    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    HAL_SYS_CLK_EVENT(0x20170621);

    {
        UINT32 status = hal_SysEnterCriticalSection();
        UINT32 t_adpll_setting1;
        UINT32 reg_RF_IF_CTRL;

        HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);


#if 1
        /*
            Fix hardware bug:
            switch from low pll clk divider (< 5/16) to 26M will cause crash

            WORKAROUND:
            raise divider up (> 10/16) before switch to 26M
        */
        t_adpll_setting1 = hwp_rf_if->adpll_setting1;
        t_adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_DIV_MASK;
        t_adpll_setting1 |= (RF_IF_SYS_PLL_CLK_DIV(10) & RF_IF_SYS_PLL_CLK_DIV_MASK);
        t_adpll_setting1 |= RF_IF_SYS_PLL_CLK_LD_CFG;
        hwp_rf_if->adpll_setting1 = t_adpll_setting1;
        for(i=0;i<10;i++)
        {
            asm volatile("nop");
        }
#endif
        //switch to non-pll clock before change pll        
        hwp_rf_if->adpll_setting1 &= ~RF_IF_SYS_FAST_CLK_SEL_MASK;
        hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;

        if(fastClock > HAL_SYS_FREQ_52M)//Enable SysPLL
        {
            //hal_SysRequestPll(HAL_SYS_PLL_SYS, HAL_SYS_PLL_FREQ_144M,NULL);

#ifdef __HAL_SYS_SWITCH_PLL_DYNAMIC__

            if(pll_config.adpll_on)
            {
                hal_SysPowerOnAdpll();
                hal_SysSetAdPllFreq(pll_config.adpll_freq);
            }

#ifdef __HAL_SYS_USE_AUX_PLL_ALWAYS__
            if(pll_config.aupll_on)
            {
                if(!hal_SysAupllWorkForAudio())
                {
                    hal_SysPowerOnAupll();
                    hal_SysSetAuPllFreq(pll_config.aupll_freq);
                }
            }
#endif /* __HAL_SYS_USE_AUX_PLL_ALWAYS__ */

            if(pll_config.adpll_selected)
            {
                hwp_rf_if->adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_SRC_SEL_AUPLL;
            }
            else if(pll_config.aupll_selected)
            {
                hwp_rf_if->adpll_setting1 |= RF_IF_SYS_PLL_CLK_SRC_SEL_AUPLL;
            }

#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */
            
            while(!hal_SysGetPLLLock());

        }
        else if(fastClock > HAL_SYS_FREQ_26M) //Enable doubler
        {
            //pmd_SetDoubler(TRUE);
        }
        else
        {
        }


        t_adpll_setting1 = hwp_rf_if->adpll_setting1;

        HAL_SYS_CLK_EVENT(0x20170405);
        HAL_SYS_CLK_EVENT(t_adpll_setting1);
 

        //output frequency is clk_cpu_adpll*(sys_pll_clk_div+1)/16
        t_adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_DIV_MASK;
        t_adpll_setting1 |= (sysFreq & RF_IF_SYS_PLL_CLK_DIV_MASK);


        //HCLK/PCLK clock divider   0:: div_1   1:: div_2  2:: div_4  3:: div_8  4:: div_16
        t_adpll_setting1 &= ~RF_IF_HCLK_PCLK_DIV_MASK;
        t_adpll_setting1 |= (sysFreq & RF_IF_HCLK_PCLK_DIV_MASK);

        //ld cfg
        t_adpll_setting1 |= RF_IF_HCLK_PCLK_LD_CFG | RF_IF_SYS_PLL_CLK_LD_CFG;
        hwp_rf_if->adpll_setting1 = t_adpll_setting1;

        
        hwp_rf_if->adpll_setting1&= ~RF_IF_SYS_FAST_CLK_SEL_MASK;
        // Configure System Clock.
        switch(fastClock)
        {
            case HAL_SYS_FREQ_26M:
                 hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;
                break;

            case HAL_SYS_FREQ_39M:
                 hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_39M;
                break;

            case HAL_SYS_FREQ_52M:
                 hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_XTAL_52M;
                break;

            default:
                hwp_rf_if->adpll_setting1 |= RF_IF_SYS_FAST_CLK_SEL_SYS_PLL_DIV_CLK;
                break;

        }
 
        //Disable SysPLL
       // if (fastClock <= HAL_SYS_FREQ_52M)
        {
            //hal_SysRequestPll(HAL_SYS_PLL_SYS, HAL_SYS_PLL_FREQ_32K,NULL);
#ifdef __HAL_SYS_SWITCH_PLL_DYNAMIC__

            if(!pll_config.adpll_on)
            {
                hal_SysPowerOffAdpll();
            }

#ifdef __HAL_SYS_USE_AUX_PLL_ALWAYS__
            if(!pll_config.aupll_on)
            {
                if(!hal_SysAupllWorkForAudio())
                {
                    hal_SysPowerOffAupll();
                }
            }
#endif /* __HAL_SYS_USE_AUX_PLL_ALWAYS__ */
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */
        }

        //Disable Doubler
        if (fastClock <= HAL_SYS_FREQ_26M)
        {
            //pmd_SetDoubler(FALSE);
        }

        HAL_SYS_RF_IF_CTRL_APB_EXIT(reg_RF_IF_CTRL);
        
        hal_SysExitCriticalSection(status);
    }

    if (g_halSysSystemFreq > fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    HAL_SYS_CLK_EVENT(0x20170407);
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
    HAL_SYS_PLL_CONFIG pll_config;

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClockSetup, &sysFreq, &pll_config);


    /*HAL_SYS_CLK_EVENT(0x20170401);
    HAL_SYS_CLK_EVENT(fastClockSetup);
    HAL_SYS_CLK_EVENT(g_halSysSystemFreq);
    HAL_SYS_CLK_EVENT(fastClock);
    HAL_SYS_CLK_EVENT(hwp_rf_if->adpll_setting1);*/


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
        #if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
            pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SYS, PMD_CORE_VOLTAGE_HIGH);
        #endif
    }


#if 0
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
#endif


    hal_SysChangeSystemClockByOneStep(fastClock);

    if (g_halSysSystemFreq > HAL_SYS_FREQ_104M && fastClock <=  HAL_SYS_FREQ_104M)
    {
        #if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
            pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SYS, PMD_CORE_VOLTAGE_LOW);
        #endif
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
#if 0
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
#endif
}
#endif



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
#if 0
    UINT32 oldClkField;
    UINT32 newClkField;
    HAL_SYS_VOC_FREQ_T oldClock;
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL	
	HAL_SYS_VOC_FREQ_T gCurr_newClock=newClock;
#endif

    UINT32 status = hal_SysEnterCriticalSection();
    HAL_SYS_CLK_EVENT(0x20160601);

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


    HAL_SYS_CLK_EVENT(0x20160602);

    hal_SysExitCriticalSection(status);

    return oldClock;
#endif
    return 0;
}


PUBLIC HAL_SYS_SPIFLASH_FREQ_T hal_SysSetSpiFlashClock(HAL_SYS_SPIFLASH_FREQ_T newClock)
{
    UINT32 oldClkField;
    UINT32 newClkField = SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M;
    UINT32 pll_clk, pll_clk_div;
    HAL_SYS_SPIFLASH_FREQ_T oldClock = HAL_SYS_SPIFLASH_FREQ_312M;
    
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL	
    newClock = HAL_SYS_SPIFLASH_FREQ_26M;
#endif


    //avoid ruin chip in case of crash
    //hal_TimDelay(1 SECOND);

    UINT32 status = hal_SysEnterSC();

    HAL_SYS_CLK_EVENT(0x20160603);
    HAL_SYS_CLK_EVENT(hwp_sysCtrl->spiflash_clk_cfg);

    oldClkField = hwp_sysCtrl->spiflash_clk_cfg & SYS_CTRL_SPIFLASH_FAST_CLK_SEL_MASK;


    switch(newClock)
    {
        case HAL_SYS_SPIFLASH_FREQ_26M  : newClkField = SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M;
            break;
        case HAL_SYS_SPIFLASH_FREQ_39M  : newClkField = SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_39M;
            break; 
        case HAL_SYS_SPIFLASH_FREQ_52M  : newClkField = SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_52M;
            break; 

        case HAL_SYS_SPIFLASH_FREQ_78M  : 
        case HAL_SYS_SPIFLASH_FREQ_89M  :
        case HAL_SYS_SPIFLASH_FREQ_104M :
        case HAL_SYS_SPIFLASH_FREQ_113M :
        case HAL_SYS_SPIFLASH_FREQ_125M :
        case HAL_SYS_SPIFLASH_FREQ_139M :
        case HAL_SYS_SPIFLASH_FREQ_156M :
        case HAL_SYS_SPIFLASH_FREQ_178M :
        case HAL_SYS_SPIFLASH_FREQ_208M :
            newClkField = SYS_CTRL_SPIFLASH_FAST_CLK_SEL_SPIFLASH_PLL_DIV_CLK;
            break; 

        case HAL_SYS_SPIFLASH_FREQ_250M :
        case HAL_SYS_SPIFLASH_FREQ_312M :
            HAL_ASSERT(FALSE, "SPI FLASH max clock is 104M");
            break; 

        default                    : break;
    }
    if (oldClock <= HAL_SYS_SPIFLASH_FREQ_78M && newClock > HAL_SYS_SPIFLASH_FREQ_78M)
    {
        //pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SPIFLASH, PMD_CORE_VOLTAGE_HIGH);
    }

    if(newClock > HAL_SYS_SPIFLASH_FREQ_52M)//Enable SysPLL
    {
        //adPll must be power on before set spi flash clk, 
        //because in early boot time ram is NOT initialized
        //we check the pll lock to 
        HAL_ASSERT(hal_SysGetPLLLock(), "pll must be power on in hal_SysSetupPLL");

        pll_clk = hal_SysGetPLLFreq();
        //Output frequency is sys_pll_clk *sys_pll_clk_div +1) /16
        //pll_clk_div = ((newClock << 4) - 1 /* + (pll_clk>>1) should less than request clk*/) / pll_clk;

        if(pll_clk > HAL_SYS_SPIFLASH_FREQ_104M)
            pll_clk_div = 0x7;
        else
            pll_clk_div = 0xF;
        

        //switch to 26M before change pll divider
        hwp_sysCtrl->spiflash_clk_cfg &= ~SYS_CTRL_SPIFLASH_FAST_CLK_SEL_MASK;
    	hwp_sysCtrl->spiflash_clk_cfg |= SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M;

        hwp_sysCtrl->spiflash_clk_cfg &= ~SYS_CTRL_SPIFLASH_PLL_CLK_DIV_MASK;
        hwp_sysCtrl->spiflash_clk_cfg |= SYS_CTRL_SPIFLASH_PLL_CLK_LD_CFG | SYS_CTRL_SPIFLASH_PLL_CLK_DIV(pll_clk_div);
    }

    hwp_sysCtrl->spiflash_clk_cfg &= ~SYS_CTRL_SPIFLASH_FAST_CLK_SEL_MASK;
	hwp_sysCtrl->spiflash_clk_cfg |= newClkField;


    if (oldClock > HAL_SYS_SPIFLASH_FREQ_78M && newClock <=  HAL_SYS_SPIFLASH_FREQ_78M)
    {
        //pmd_SetCoreVoltage(PMD_CORE_VOLTAGE_USER_SPIFLASH, PMD_CORE_VOLTAGE_LOW);
    }

    HAL_SYS_CLK_EVENT(0x20160604);

    hal_SysExitSC(status);

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
#if 0
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
#endif
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
#ifndef CHIP_5856E    
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
#else
    UINT32 chipId = hwp_configRegs->chip_id;

    if (part == HAL_SYS_CHIP_PROD_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_PROD_ID);
    }
    else if (part == HAL_SYS_CHIP_BOND_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_EXT_PROD_ID);
    }
    else if (part == HAL_SYS_CHIP_METAL_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_METAL_ID);
    }
    else // HAL_SYS_CHIP_ID_WHOLE
    {
        // returning the whole chip ID
    }
#endif
    

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
#if 0
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
        HAL_SYS_CLK_EVENT(0x22225);
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
#endif
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
#if 0
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
#endif
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
        #ifndef CHIP_5856E    
            hwp_configRegs->Alt_mux_select =
                (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
                CFG_REGS_PWL1_CLK_32K;
        #endif
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
            #ifndef CHIP_5856E    
                hwp_configRegs->Alt_mux_select =
                    (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
                    CFG_REGS_PWL1_GPO_6;
            #endif
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
    // The checking order should comply with the priority of reset causes.
    // In case of multiple reset causes in place simultaneously, only the
    // reset cause with highest priority is returned.
    // E.g.,
    // If an alarm is triggered during the shutdown process, the system will
    // reboot after the shutdown process, for the wakeup signal is kept high
    // by the RTC alarm. After the system boots up, ALARM and RESTART causes
    // will be valid, but only ALARM should be returned.
    if (g_bootResetCause & SYS_CTRL_WDT_RST)
    {
        return HAL_SYS_RESET_CAUSE_WATCHDOG;
    }

    if (g_bootResetCause & SYS_CTRL_DBG_HST_RST)
    {
        return HAL_SYS_RESET_CAUSE_HOST_DEBUG;
    }

    if (g_bootResetCause & SYS_CTRL_ALARM_CAUSE)
    {
        return HAL_SYS_RESET_CAUSE_ALARM;
    }

    // Reset has the lowest priority
    if (g_bootResetCause & SYS_CTRL_GLOBAL_SOFT_RST)
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

VOID hal_SysExitSC(UINT32 status)
{
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
    *g_halSysCsAddress = status;
}


UINT32 hal_SysEnterSC(VOID)
{
    UINT32  myLocalVar = *g_halSysCsAddress;
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
    return myLocalVar;
}

VOID hal_SysExitCriticalSection(UINT32 status)
{
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    hwp_sysIrq->mask_set = status;

}


UINT32 hal_SysEnterCriticalSection(VOID)
{
   // UINT32  myLocalVar = *g_halSysCsAddress;
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
   // asm volatile ("");
   // return myLocalVar;
   UINT32  myLocalVar = hwp_sysIrq->mask_set & 0xfffff000;
   hwp_sysIrq->mask_clear = 0xfffff000;
   return myLocalVar;
}

UINT32 hal_BTEnterCriticalSection(VOID)
{
   UINT32  myLocalVar = hwp_sysIrq->mask_set & 0xfff;
   hwp_sysIrq->mask_clear = 0xfff;
   return myLocalVar;
}


//PUBLIC VOLATILE UINT32* HAL_BOOT_DATA_INTERNAL g_bootSysCsAddress = (UINT32*)&hwp_sysIrq->sc;

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

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)       
PUBLIC VOID hal_SysWriteRFUsingSPI(void)
{
    hwp_sysCtrl->RF_IF_CTRL = 1;
}

PUBLIC VOID hal_SysWriteRFUsingAPB(void)
{
    hwp_sysCtrl->RF_IF_CTRL = 0;
}

PUBLIC UINT32 hal_SysEnterRFIFCtrlAPB(VOID)
{
        UINT32 reg_RF_IF_CTRL;

        HAL_SYS_RF_IF_CTRL_APB_ENTER(reg_RF_IF_CTRL);
        
        return reg_RF_IF_CTRL;
}
PUBLIC VOID hal_SysExitRFIFCtrlAPB(UINT32 value)
{
    HAL_SYS_RF_IF_CTRL_APB_EXIT(value);
}

PUBLIC VOID hal_SysPMUIntfXtalCtrl(UINT8 enable)
{
    if(enable)
        hwp_pmu_intf->xtal_ctrl |= PMU_INTF_XTAL_CLK_ADC_EN_LV;   
    else
        hwp_pmu_intf->xtal_ctrl &= ~PMU_INTF_XTAL_CLK_ADC_EN_LV;   
}

#endif
