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
//                                                                            
/// @file hal_debug.c
/// That file contains the implementation of HAL debug 
/// driver
//                                                                            
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "debug_host.h"
#include "debug_port.h"
#include "page_spy.h"
#include "sys_ctrl.h"
#include "cfg_regs.h"

#include "hal_debug.h"
#include "hal_signal_spy.h"
#include "hal_ebc.h"
#include "hal_timers.h"
#include "halp_timers.h"
#include "halp_debug.h"
#include "hal_map.h"

#include "boot.h"

#include "stdarg.h"

#include "sxs_io.h"
#include "cos.h"

#if defined(ENABLE_PXTS) || defined(ENABLE_EXL)
// =============================================================================
// hal_DbgPortSetup
// -----------------------------------------------------------------------------
/// Select the mode under which use the debug port.
/// @param mode Mode under which the debug port is used. Refer to the type
/// definition for details.
// =============================================================================
PUBLIC VOID hal_DbgPortSetup(HAL_DBG_PORT_SEL_T mode)
{
    UINT32 debugPortMode;

// We need to set the debug_port in debug mode to use is
// FIXME: Implement that through a real IOmuxing driver
#ifndef CHIP_5856E
    hwp_configRegs->Alt_mux_select &= ~CFG_REGS_DEBUG_PORT_MASK;
    hwp_configRegs->Alt_mux_select |= CFG_REGS_DEBUG_PORT_DEBUG;
#endif
    switch (mode)
    {
        case HAL_DBG_EXL:
            debugPortMode = DEBUG_PORT_MODE_CONFIG_MODE_EXL;
            break;

        case HAL_DBG_PXTS:
            debugPortMode = DEBUG_PORT_MODE_CONFIG_MODE_PXTS;
            break;

        case HAL_DBG_EXL_AND_PXTS:
            debugPortMode =  DEBUG_PORT_MODE_CONFIG_MODE_EXL
                            | DEBUG_PORT_MODE_CONFIG_MODE_PXTS;
            break;

        case HAL_DBG_DEBUG_BUS:
            debugPortMode = DEBUG_PORT_MODE_CONFIG_MODE_SIGNAL_SPY;
            break;

        case HAL_DBG_NONE:
        default:
            debugPortMode = DEBUG_PORT_MODE_CONFIG_MODE_PXTS_EXL_OFF;
            break;
    }

    // Generated clock is the 156 Mhz divided by (this + 2), thus 52 Mhz.
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    hwp_sysCtrl->Cfg_Clk_Dbg_Div = SYS_CTRL_CLK_DBG_DIVIDER(1);
#endif
    hwp_debugPort->Debug_Port_Mode = debugPortMode
// this flag's value can be set in your customvars.mk
#ifdef CHIP_DEBUG_PORT_PXTS_EXL_CLK_INV_POL
                                    | DEBUG_PORT_PXTS_EXL_CLOCK_POL
#endif
                                    ;

}
#endif // ENABLE_PXTS || ENABLE_EXL



// =============================================================================
// hal_DbgFatalTrigger
// -----------------------------------------------------------------------------
/// Triggers a fatal error.
/// This trigger is sent to the Flamulator tool.
// =============================================================================
PUBLIC VOID hal_DbgFatalTrigger(UINT8 trigId)
{
    hal_DbgPxtsSendTrigger(HAL_DBG_PXTS_HAL);
#ifdef CHIP_CPU_IS_MIPS
    boot_DbgExlTrigFunction0();
#endif
}



// PXTS profile stuff in hal_profile.c



// Assert written in hal_assert.c 



// =============================================================================
// hal_DbgSignalSpyCfg
// -----------------------------------------------------------------------------
/// This function configures the Signal Spy for the chosen mode.
/// (The device on the bus is the signal spy).
/// 
/// @param rxOn If \c TRUE outputs RX_ON \n
///             If \c FALSE outputs GMSK_ON
/// @param txOn If \c TRUE outputs TX_ON \n
///             If \c FALSE outputs RX_DEC_ON
/// @param busMode Select the mode within: - #HAL_DBG_SIGNAL_SPY_MODE_SYS 
///                                        - #HAL_DBG_SIGNAL_SPY_MODE_BB
///                                        - #HAL_DBG_SIGNAL_SPY_MODE_TX
///                                        - #HAL_DBG_SIGNAL_SPY_MODE_RX
/// @param irqSel1 Select the signal for DBG_DATA_4 when the mode allows it
/// @param irqSel2 Select the signal for DBG_DATA_7
/// @param clkSel Select the clock to output on the DBG_CLK pin.
// =============================================================================
PUBLIC VOID hal_DbgSignalSpyCfg(
        BOOL rxOn,
        BOOL txOn, 
        HAL_DBG_SIGNAL_SPY_MODE_T busMode, 
        HAL_DBG_SIGNAL_SPY_IRQ_SEL_T irqSel1,
        HAL_DBG_SIGNAL_SPY_IRQ_SEL_T irqSel2,
        HAL_DBG_SIGNAL_SPY_CLK_SEL_T clkSel)
{
// TODO Add in the Doc that rxOn and txOn are
// ignored.
    hwp_debugPort->Signal_Spy_Cfg = 
        DEBUG_PORT_DBG_BUS_MODE(busMode)
      | DEBUG_PORT_DBG_BUS_SIG_SEL0(irqSel1)
      | DEBUG_PORT_DBG_BUS_SIG_SEL1(irqSel2);

    hwp_debugPort->Dbg_Clk_Cfg = DEBUG_PORT_SEL_SOURCE(clkSel)
                                | DEBUG_PORT_ENABLE_DBG_ENABLED;
}


#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
// =============================================================================
// hal_DbgXcpuBkptSetup
// -----------------------------------------------------------------------------
/// Setups a hardware breakpoint on the XCPU
///
/// @param bkptType Type of the breakpoint
/// @param bkptAddr Address of the hardware breakpoint
// =============================================================================
PUBLIC VOID hal_DbgXcpuBkptSetup(HAL_DBG_BKPT_TYPE_T bkptType, UINT32 bkptAddr)
{
    hal_TimWatchDogStop();
#ifndef  CHIP_5856E   
    hwp_sysCtrl->xcpu_dbg_bkp= SYS_CTRL_BKPT_EN | SYS_CTRL_BKPT_MODE(bkptType);
    hwp_sysCtrl->xcpu_dbg_addr=bkptAddr;
#endif    
}



// =============================================================================
// hal_DbgXcpuBkptDisable
// -----------------------------------------------------------------------------
/// Disables all previously setup breakpoints on the XCPU 
// =============================================================================
PUBLIC VOID hal_DbgXcpuBkptDisable(VOID)
{
#ifndef  CHIP_5856E    
    hwp_sysCtrl->xcpu_dbg_bkp=(hwp_sysCtrl->xcpu_dbg_bkp) & (~SYS_CTRL_BKPT_EN & ~SYS_CTRL_STALLED);
    hwp_sysCtrl->xcpu_dbg_bkp = 0;
#endif    
    hal_TimWatchDogKeepAlive();
}

// =============================================================================
// hal_DbgBcpuBkptSetup
// -----------------------------------------------------------------------------
/// Setups a hardware breakpoint on the BCPU
///
/// @param bkptType Type of the breakpoint
/// @param bkptAddr Address of the hardware breakpoint
// =============================================================================
PUBLIC VOID hal_DbgBcpuBkptSetup(HAL_DBG_BKPT_TYPE_T bkptType, UINT32 bkptAddr)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    hal_TimWatchDogStop();
    hwp_sysCtrl->BCpu_Dbg_BKP= SYS_CTRL_BKPT_EN | SYS_CTRL_BKPT_MODE(bkptType);
    hwp_sysCtrl->BCpu_Dbg_Addr=bkptAddr;
#endif
}



// =============================================================================
// hal_DbgBcpuBkptDisable
// -----------------------------------------------------------------------------
/// Disables all previously setup breakpoints on the BCPU 
// =============================================================================
PUBLIC VOID hal_DbgBcpuBkptDisable(VOID)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    //hwp_sysCtrl->Bcpu_Dbg_BKP=(hwp_sysCtrl->Bcpu_Dbg_BKP) & (~SYS_CTRL_BKPT_EN & ~SYS_CTRL_STALLED);
    hwp_sysCtrl->BCpu_Dbg_BKP = 0;
    hal_TimWatchDogKeepAlive();
#endif
}


// =============================================================================
// hal_DbgXcpuBkptEd
// -----------------------------------------------------------------------------
/// Check if the XCPU is breakpointed 
// =============================================================================
PUBLIC BOOL hal_DbgXcpuBkptEd(VOID)
{
#ifndef  CHIP_5856E    
    //hwp_sysCtrl->XCpu_Dbg_BKP=(hwp_sysCtrl->XCpu_Dbg_BKP) & (~SYS_CTRL_BKPT_EN & ~SYS_CTRL_STALLED);
    return (hwp_sysCtrl->xcpu_dbg_bkp & SYS_CTRL_STALLED);
#else
    return true;
#endif
}

#endif

// =============================================================================
// hal_DbgWatchdogGetContext
// -----------------------------------------------------------------------------
/// Watchdog context saving variable 
/// 
/// This function returns a pointer to the global variable used to store the 
/// context of a watchdog reset.
/// As it is read after a reset, (during the boot process) the field \c 
/// softResetMarker is used to know if this boot is normal or due to 
/// the watchdog, in which case the \c errorContext field is filled with
/// relevant information.
// ============================================================================= 
PUBLIC HAL_DBG_WATCHDOG_CTX_T* hal_DbgWatchdogGetContext(VOID)
{
    return (&g_halWatchdogCtx);
}



// =============================================================================
// hal_DbgTrace
// -----------------------------------------------------------------------------
/// Function to call when 'building' a trace to print an a host computer
/// plugged to the mobile.
/// @param level Trace level.
/// @param tsmap Bitfield which defines which parameter is a string.
/// @param fmt String format a-la printf, displayed in the trace.
// =============================================================================
PUBLIC VOID hal_DbgTrace(UINT32 level, UINT32 tsmap, CONST CHAR *fmt, ...)
{
    UINT32 id;
    va_list args;

    id = level | TSMAP(tsmap);
    va_start(args, fmt);
    #if OS_KERNEL == SXR
    sxs_vprintf(id, fmt, args);
    #else
    vvprintf(id, fmt, args);
    #endif
    va_end(args);
}

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
// =============================================================================
// hal_DbgXcpuBkptForce
// -----------------------------------------------------------------------------
/// Stall the CPU where this function is called.
// =============================================================================
PUBLIC VOID hal_DbgXcpuBkptForce(VOID)
{
#ifndef  CHIP_5856E    
    hwp_sysCtrl->xcpu_dbg_bkp = SYS_CTRL_STALLED;
    UINT32 tmp __attribute__((unused)) = hwp_sysCtrl->xcpu_dbg_bkp;
#endif    
}
#endif
/**
    @brief Configure logging

    This function is used to configure the logging parameters: which CPU is
    recorded, what is the branch type to be looked at, and what is the record 
    type (branch taken or not, both, etc ...) \n
    This function doesn't start the logging.

    @param RecordType Type of the record (jump and branch, branch not taken, ...)
    @param EnableInterruptLog Trigs also when an interrupt occurs
    @param BranchType Type of the branch (BLTZ, BNE,...)
    @param CPU Selects the CPU whose PCs are recorded

*/
PUBLIC VOID hal_DbgExlCfg(
        HAL_DBG_EXL_RECORD_TYPE_T recordType,
        BOOL enableInterruptLog,            // triggers also when an interrupt occurs
        HAL_DBG_EXL_BRANCH_TYPE_T branchType,
        HAL_DBG_EXL_CPU_SEL_T cpu)
{
    UINT32 ctrlReg = 0;

    // Build register config
    if (branchType == HAL_DBG_EXL_BRACH_TYPE_ANY_BRANCH)
    {
        ctrlReg |= DEBUG_PORT_BRANCH_TYPE(branchType)
                | DEBUG_PORT_RECORD_TRIGGER_TYPE(recordType)
                | DEBUG_PORT_ANY_BRANCH;
    }
    else
    {
        ctrlReg |= DEBUG_PORT_BRANCH_TYPE(branchType)
                | DEBUG_PORT_RECORD_TRIGGER_TYPE(recordType);
    }

    if (enableInterruptLog == TRUE)
    {
        ctrlReg |= DEBUG_PORT_INT_EN;
    }

    if (cpu == HAL_DBG_EXL_XCPU)
    {
        ctrlReg |= DEBUG_PORT_CPU_SELECT_XCPU;
    }
    else
    {
        ctrlReg |= DEBUG_PORT_CPU_SELECT_BCPU;
    }

    // Write register
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    hwp_debugPort->Pxts_Exl_Cfg = ctrlReg;
#endif

}


// =============================================================================
// hal_DbgPxtsSetup
// -----------------------------------------------------------------------------
/// Enable the PXTS on the level passed as parameter.
/// That function configure the debug_port in debug and enable the 
/// PXTS mode.
/// @param mask A bitfield where a '1' means that the corresponding 
/// PXTS level is enabled.
// =============================================================================
PUBLIC VOID hal_DbgPxtsSetup(UINT16 mask)
{
// #else? That configuration is not needed for chips
// without the PXTS levels feature.
#ifdef CHIP_HAS_PXTS_LEVELS
    hwp_debugPort->Pxts_Exl_Cfg &= ~DEBUG_PORT_ENABLE_PXTS_TAG_MASK;
    hwp_debugPort->Pxts_Exl_Cfg |= DEBUG_PORT_ENABLE_PXTS_TAG(mask);
#endif

#if  defined(HAL_PROFILE_NOT_ON_ROMULATOR)
    // We use the same upper 16 bits for level configuration 
    // in the profile control map structure.
    UINT32* pConfig = &g_halMapAccess.profileControl.config;
    *pConfig &= ~DEBUG_PORT_ENABLE_PXTS_TAG_MASK;
    *pConfig |= DEBUG_PORT_ENABLE_PXTS_TAG(mask);
#endif // HAL_PROFILE_NOT_ON_ROMULATOR
}


