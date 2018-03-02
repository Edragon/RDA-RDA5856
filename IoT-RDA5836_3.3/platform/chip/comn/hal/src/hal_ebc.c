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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///                                                                           //
///     @file hal_ebc.h                                                       //
///                                                                           //
///     This file contains the implementation of Garnet's              //
///     HAL EBC driver                                                        //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 


#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "mem_bridge.h"
#include "page_spy.h"

#include "hal_ebc.h"
#include "hal_mem_map.h"
#include "halp_irq.h"

#include "hal_sys.h"

#include "halp_sys.h"
#include "halp_ebc.h"
#include "halp_debug.h"
#include "halp_boot_sector_driver.h"


#undef HAL_BOOT_FUNC
#define HAL_BOOT_FUNC

// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false,
/// raise an assert.
// =============================================================================
#ifdef CHIP_CPU_IS_MIPS
#define EBC_ASSERT(condition, format, ...) \
    if (UNLIKELY(!condition)) { asm("break 2"); }
#else
#define EBC_ASSERT(condition, format, ...) 
#endif

/// Wait for the EBC config to be coherent
/// WARNING: When the BB side is running, on a multi-fifo
/// system, this must be implemented using the EBC_Status
/// CS_Update bit.
/// (Reading the Fifo flush just ensure the fifo is empty).
INLINE VOID hal_EbcWaitReady(VOID)
{
   UINT32 flush __attribute__((unused)) = hwp_memFifo->Flush;
}

/// TODO Implement the remap driver.

PUBLIC VOID hal_EbcConfigRemap(HAL_EBC_FLSH_PHYSADD_T physAddr)
{
    hwp_memFifo->EBC_CS4_Block_Remap = physAddr;
}

/// Special handler in internal sram, will call a func that must also be in sram that is responsible for
/// switching the flash/sram in burst mode. No function not in internal sram should be called.
/// stack must be also in internal sram
/// no other master (bcpu, dma, voc ...) should access the CS, ebc lock prevent this

// NOTE: With O1 or above optimization (-ftree-switch-conversion),
// the initializations of the switch statement will be translated
// to initializations from a scalar array in .rodata.
// This will lead to external RAM access, for .rodata in this
// file is located in RAM.
// In order to avoid this issue, no-tree-switch-conversion
// optimization is specified for hal_EbcCsSetupWithCallback.
#define OPT_NO_TREE_SWITCH_CONV __attribute__((optimize("no-tree-switch-conversion")))

PRIVATE VOID HAL_BOOT_FUNC_INTERNAL OPT_NO_TREE_SWITCH_CONV
hal_EbcCsSetupWithCallback(HAL_EBC_CS_T cs, CONST HAL_EBC_CS_CFG_T* csConfig, HAL_EBC_BURSTCALLBACK_T* cbFunc)
{
    // union representing the status bitfield in a 32 bits value
    // loadable in the register
    union 
    {
        UINT32 reg;
        HAL_EBC_TIME_CFG_T bitfield;
    } timeCfgUnion;

    // union representing the status bitfield in a 32 bits value
    // loadable in the register   
    union
    {
        UINT32 reg;
        HAL_EBC_MODE_CFG_T bitfield;
    } modeCfgUnion;    

    UINT32 pageCfg;
    UINT16* csBase;

    timeCfgUnion.bitfield = csConfig->time;
    modeCfgUnion.bitfield = csConfig->mode;

    pageCfg = hwp_pageSpy->enable;
    hwp_pageSpy->disable = pageCfg;
    hwp_memBridge->FIFO_Ctrl = MEM_BRIDGE_LOCK_ENABLE;

    // WARNING: In case of two fifos system (one for the BB side,
    // one for the SYS side), we should check here that both fifo
    // are empty. We are currently assuming that the baseband is
    // not running when we are calling those functions.

    switch(cs)
    {
        case HAL_EBC_FLASH :
            csBase = (UINT16*)hwp_cs0;
            break;

        case HAL_EBC_SRAM :
            csBase = (UINT16*)hwp_cs1;
            break;

        default:
            csBase = NULL;
            break;
    }

    // call the callback
    if (cbFunc)
    {
        cbFunc(csBase,&(hwp_memFifo->EBC_CRE));
    }

    hal_EbcWaitReady(); // OK because inline
    hwp_memBridge->CS_Time_Write          = timeCfgUnion.reg;
    switch(cs)
    {
        case HAL_EBC_FLASH :
#ifndef __PRJ_WITH_SPIFLSH__
            hwp_memBridge->CS_Config[0].CS_Mode   = modeCfgUnion.reg;
#endif
            break;

         case HAL_EBC_SRAM :
            hwp_memBridge->CS_Config[1].CS_Mode   = modeCfgUnion.reg;

            // Save the config in the Boot Sector structure, so that
            // it can be used by the Boot Sector code to configure the
            // EBC CS RAM mode and timings, it is especially useful
            // in burst mode.
            hal_BootSectorSetEbcConfig(timeCfgUnion.reg, modeCfgUnion.reg);

            break;

        default:
            break;
    }

    hal_EbcWaitReady(); // OK because inline

    hwp_memBridge->FIFO_Ctrl = MEM_BRIDGE_LOCK_DISABLE;

//if granite_test_chip
    {
        // remove fifo regs access from fifo
        UINT16 read __attribute__((unused)) = *((volatile UINT16 *)csBase);
        read = *((volatile u16 *)csBase);
    }
//endif

    hwp_pageSpy->enable = pageCfg;
    // flush cpu write buffer
    {
        UINT32 unused __attribute__((unused)) = hwp_pageSpy->enable;
    }

}


// ============================================================================
// hal_EbcFlashOpen
// ----------------------------------------------------------------------------
/// Enable the CS for FLASH. That chip select is named like this for its 
/// common use, but can in fact be plugged to a RAM.
/// This function must be called before any other EBC driver function, 
/// as the Flash Chip is the device imposing the frequency on the memory
/// bridge. This optimal frequency is set as a parameter and the corresponding
/// EBC settings are also specified.
/// The Chip Select used is not a parameter of the function as it is always
/// #HAL_EBC_FLASH (ie. CS0)
///
/// @param optimalFreq Optimal Frequency to use the chip plugged on the 
/// #HAL_EBC_FLASH chip select.
/// @param csConfig Configuration for the chip select. The \c csEn enable bit
/// of the mode configuration must be set to 1 !
/// @param cbFunc callback function for enabling the burst mode on sram
/// or NULL if not needed. Note: cbFunc must not be in flash
/// @return Base address of the configured space
// ============================================================================
PUBLIC VOID* HAL_BOOT_FUNC hal_EbcFlashOpen(HAL_SYS_FREQ_T optimalFreq, CONST HAL_EBC_CS_CFG_T* csConfig, HAL_EBC_BURSTCALLBACK_T* cbFunc)
{
#ifdef CHIP_HAS_EBC_CS2_BUG
    // union representing the status bitfield in a 32 bits value
    // loadable in the register
    union 
    {
        UINT32 reg;
        HAL_EBC_TIME_CFG_T bitfield;
    } timeCfgUnion;
    
    // union representing the status bitfield in a 32 bits value
    // loadable in the register   
    union 
    {
        UINT32 reg;
        HAL_EBC_MODE_CFG_T bitfield;
    } modeCfgUnion;        
#endif 

    VOID* retval = NULL;
#if (RAM_SIZE==0)
    retval = (VOID*)hwp_cs0;
    return retval;
#endif
    
    //ENTER CRITICAL
    UINT32 status = hal_SysEnterCriticalSection();

    // Wait if the lock is locked
    hal_EbcWaitReady();

    // Set EBC frequency
    // FIXME The membridge clock must be set-up before the system clock, and done 
    // only once. The flash open should (must) be done before hal_Open for 
    // the Admux. As long as it's not, hal_EbcCsSetup is called by hal_Open.
    // hal_EbcCsSetup(optimalFreq);

    // Write the pointer into the table
    //g_halEbcCsConfigArray[0]              = csConfig;

    hal_EbcCsSetupWithCallback(HAL_EBC_FLASH, csConfig, cbFunc);

    retval = (VOID*)hwp_cs0;

#ifdef CHIP_HAS_EBC_CS2_BUG
    timeCfgUnion.bitfield = csConfig->time;
    modeCfgUnion.bitfield = csConfig->mode;

    g_halCs0TimeReg  = timeCfgUnion.reg;
    g_halCs0ModeReg  = modeCfgUnion.reg;
#endif 

    //EXIT CRITICAL
    hal_SysExitCriticalSection(status);

    return retval;
}


// ============================================================================
// hal_EbcSramOpen
// ----------------------------------------------------------------------------
/// Enable a CS for SRAM. The chip selects for 
/// FLASH have been set before by calling #hal_EbcFlashOpen.
/// The enabling of a CS returns the base address of the configured space.
/// The settings are given at the opening of the peripheral on the given 
/// chip select, and must support the external maximal clock as it has been
/// set by the configuring of the CS0 (Flash) chipo select.
///
/// @param csConfig Configuration for the chip select. The \c csEn enable bit
/// of the mode configuration must be set to 1 there if the chip select
/// of the opened peripheral have to be enabled !
/// @param cbFunc callback function for enabling the burst mode on sram
/// or NULL if not needed. Note: cbFunc must not be in sram
/// @return Base address of the configured space
// ============================================================================
PUBLIC VOID* HAL_BOOT_FUNC hal_EbcSramOpen(CONST HAL_EBC_CS_CFG_T* csConfig, HAL_EBC_BURSTCALLBACK_T* cbFunc)
{
    VOID* retval = NULL;
    
    //ENTER CRITICAL
    UINT32 status = hal_SysEnterCriticalSection();

    hal_EbcCsSetupWithCallback(HAL_EBC_SRAM, csConfig, cbFunc);

    retval = (VOID*)hwp_cs1;

    //EXIT CRITICAL
    hal_SysExitCriticalSection(status);
    
    return retval;
}


// ============================================================================
// hal_EbcCsOpen
// ----------------------------------------------------------------------------
/// Enable a CS other than the one for FLASH. The chip selects for 
/// FLASH have been set before by calling #hal_EbcFlashOpen.
/// The enabling of a CS returns the base address of the configured space.
/// The settings are given at the opening of the peripheral on the given 
/// chip select, and must support the external maximal clock as it has been
/// set by the configuring of the CS0 (Flash) chip select.
///
/// @param cs Chip Select to Enable. (HAL_EBC_SRAM and CS2 or above)
/// @param csConfig Configuration for the chip select. The \c csEn enable bit
/// of the mode configuration must be set to 1 there if the chip select
/// of the opened peripheral have to be enabled !
/// @return Base address of the configured space
// ============================================================================
PUBLIC VOID* HAL_BOOT_FUNC hal_EbcCsOpen(HAL_EBC_CS_T cs, CONST HAL_EBC_CS_CFG_T* csConfig)
{
    union 
    {
        UINT32 reg;
        HAL_EBC_TIME_CFG_T bitfield;
    } timeCfgUnion; // union representing the status bitfield in a 32 bits value
         // loadable in the register
    
    union 
    {
        UINT32 reg;
        HAL_EBC_MODE_CFG_T bitfield;
    } modeCfgUnion; // union representing the status bitfield in a 32 bits value
         // loadable in the register     
    
    VOID* retval = NULL;
    
    //ENTER CRITICAL
    UINT32 status = hal_SysEnterCriticalSection();

    timeCfgUnion.bitfield = csConfig->time;
    modeCfgUnion.bitfield = csConfig->mode;
  
    // Wait if the lock is locked
    hal_EbcWaitReady();
    switch(cs)
    {
        case HAL_EBC_FLASH :
           #ifdef CHIP_CPU_IS_MIPS
            EBC_ASSERT(FALSE, "Improper use of the hal_EbcCsOpen function."
                    "It cannot be used to open CS0 ! Use hal_EbcFlashOpen"
                    " to do that.");
           #endif
            break;

         case HAL_EBC_SRAM :
            // Write the pointer into the table
            //g_halEbcCsConfigArray[1]         = csConfig;
            hwp_memBridge->CS_Time_Write          = timeCfgUnion.reg;
            hwp_memBridge->CS_Config[1].CS_Mode   = modeCfgUnion.reg;

            // Save the config in the Boot Sector structure, so that
            // it can be used by the Boot Sector code to configure the
            // EBC CS RAM mode and timings, it is especially useful
            // in burst mode.
            hal_BootSectorSetEbcConfig(timeCfgUnion.reg, modeCfgUnion.reg);

            retval = (VOID*)hwp_cs1;
            break;

        case HAL_EBC_CS2 :
            // Write the pointer into the table
            //g_halEbcCsConfigArray[2]         = csConfig;
            hwp_memBridge->CS_Time_Write          = timeCfgUnion.reg;
            hwp_memBridge->CS_Config[2].CS_Mode   = modeCfgUnion.reg;

            retval = (VOID*)hwp_cs2;
            break;
            
        case HAL_EBC_CS3 :
            //g_halEbcCsConfigArray[3]         = csConfig;
            hwp_memBridge->CS_Time_Write          = timeCfgUnion.reg;
            hwp_memBridge->CS_Config[3].CS_Mode   = modeCfgUnion.reg;

            retval = (VOID*)hwp_cs3;
            break;
            
        case HAL_EBC_CS4:
            //g_halEbcCsConfigArray[4]         = csConfig;
            hwp_memBridge->CS_Time_Write          = timeCfgUnion.reg;
            hwp_memBridge->CS_Config[4].CS_Mode   = modeCfgUnion.reg;

            retval = (VOID*)hwp_cs4;
            break;

        default:
            break;
    }

    //EXIT CRITICAL
    hal_SysExitCriticalSection(status);

    return retval;
}


// ============================================================================
// hal_EbcCsClose
// ----------------------------------------------------------------------------
/// Disables a CS (except FLASH and SRAM).
/// The address space previously allocated for this chip select becomes
/// unavailable after a call to this function
/// 
/// @param cs The Chip Select to Disable
// ============================================================================
PUBLIC VOID hal_EbcCsClose(HAL_EBC_CS_T cs)
{
    UINT32 status = hal_SysEnterCriticalSection();
    hal_EbcWaitReady();
    hwp_memBridge->CS_Time_Write = 0;
    switch(cs) {
    case HAL_EBC_CS2:
        hwp_memBridge->CS_Config[2].CS_Mode = 0;
        //g_halEbcCsConfigArray[2]= NULL;
        break;
    case HAL_EBC_CS3:
        hwp_memBridge->CS_Config[3].CS_Mode = 0;
        //g_halEbcCsConfigArray[3]= NULL;
        break;
    case HAL_EBC_CS4:
        hwp_memBridge->CS_Config[4].CS_Mode = 0;
        //g_halEbcCsConfigArray[4]= NULL;
        break;
    default:
        break;
   }
    hal_SysExitCriticalSection(status);
}


// ============================================================================
// hal_EbcGetFrequency 
// ----------------------------------------------------------------------------
/// Gets the EBC frequency
///
/// @return The EBC frequency
// ============================================================================
PUBLIC HAL_SYS_MEM_FREQ_T hal_EbcGetFrequency(VOID)
{
    return hal_BootSectorGetEbcFreq();  
}


// ============================================================================
// hal_EbcGetCsAddress
// ----------------------------------------------------------------------------
/// Return the address of a given Chip Select.
///
/// @param cs Chip Select
/// @return Base address of the configured space
// ============================================================================
PUBLIC VOID* hal_EbcGetCsAddress(HAL_EBC_CS_T cs)
{
    VOID* retval = NULL;

    switch(cs)
    {
        case HAL_EBC_FLASH :
            retval = (VOID*)hwp_cs0;
            break;

         case HAL_EBC_SRAM :
            retval = (VOID*)hwp_cs1;
            break;

        case HAL_EBC_CS2 :
            retval = (VOID*)hwp_cs2;
            break;

        case HAL_EBC_CS3 :
            retval = (VOID*)hwp_cs3;
            break;

        case HAL_EBC_CS4:
            retval = (VOID*)hwp_cs4;
            break;

        default:
            break;
    }

    return retval;
}


// ============================================================================
//                            PROTECTED
// ============================================================================

// ============================================================================
// hal_EbcCsSetup
// ----------------------------------------------------------------------------
/// Configure the CS settings according to the system clock frequency
/// This function configures the CS time and mode settings.
///
/// @param sysFreq System frequency to which adjust 
/// the CS settings
// ============================================================================
PROTECTED VOID HAL_BOOT_FUNC hal_EbcCsSetup(HAL_SYS_MEM_FREQ_T ebcFreq)
{
#if (CHIP_ASIC_ID <= CHIP_ASIC_ID_5856)
    UINT32 reg;
#endif
    // We need the Pll to do that. Check it is locked
    // Not on the FPGA ..
#ifndef FPGA
    // TODO
    // Cf. hal_ebcFlashOpen.
    // EBC_ASSERT(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED, "pll not enabled before setting ebc clock");
    // IMPLEMENT
#else
    // FPGA
    ebcFreq = HAL_SYS_MEM_FREQ_39M;
#endif
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    hal_EbcWaitReady();
    switch (ebcFreq)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_MEM_FREQ_13M:
            reg = SYS_CTRL_MEM_FREQ_13M;
            break;

        case HAL_SYS_MEM_FREQ_26M:
            reg = SYS_CTRL_MEM_FREQ_26M;
            break;

        case HAL_SYS_MEM_FREQ_39M:
            reg = SYS_CTRL_MEM_FREQ_39M;
            break;

        case HAL_SYS_MEM_FREQ_52M:
            reg = SYS_CTRL_MEM_FREQ_52M;
            break;

        case HAL_SYS_MEM_FREQ_78M:
            reg = SYS_CTRL_MEM_FREQ_78M;
            break;
        
        case HAL_SYS_MEM_FREQ_89M:
            reg = SYS_CTRL_MEM_FREQ_89M;
            break;

        case HAL_SYS_MEM_FREQ_104M:
            reg = SYS_CTRL_MEM_FREQ_104M;
            break;
#else // (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_MEM_FREQ_26M:
            reg = SYS_CTRL_MEM_FREQ_26M;
            break;

        case HAL_SYS_MEM_FREQ_39M:
            reg = SYS_CTRL_MEM_FREQ_39M;
            break;

        case HAL_SYS_MEM_FREQ_52M:
            reg = SYS_CTRL_MEM_FREQ_52M;
            break;

        case HAL_SYS_MEM_FREQ_78M:
            reg = SYS_CTRL_MEM_FREQ_78M;
            break;

        case HAL_SYS_MEM_FREQ_89M:
            reg = SYS_CTRL_MEM_FREQ_89M;
            break;

        case HAL_SYS_MEM_FREQ_104M:
            reg = SYS_CTRL_MEM_FREQ_104M;
            break;

        case HAL_SYS_MEM_FREQ_113M:
            reg = SYS_CTRL_MEM_FREQ_113M;
            break;

        case HAL_SYS_MEM_FREQ_125M:
            reg = SYS_CTRL_MEM_FREQ_125M;
            break;

        case HAL_SYS_MEM_FREQ_139M:
            reg = SYS_CTRL_MEM_FREQ_139M;
            break;

        case HAL_SYS_MEM_FREQ_156M:
            reg = SYS_CTRL_MEM_FREQ_156M;
            break;

        case HAL_SYS_MEM_FREQ_178M:
            reg = SYS_CTRL_MEM_FREQ_178M;
            break;

        case HAL_SYS_MEM_FREQ_208M:
            reg = SYS_CTRL_MEM_FREQ_208M;
            break;

        case HAL_SYS_MEM_FREQ_250M:
            reg = SYS_CTRL_MEM_FREQ_250M;
            break;

        case HAL_SYS_MEM_FREQ_312M:
            reg = SYS_CTRL_MEM_FREQ_312M;
            break;

#endif // (!CHIP_HAS_ASYNC_TCU)

        default:
            EBC_ASSERT(FALSE, "Configuring the EBC clock with a bad frequency: %d",
                    (UINT32)ebcFreq);
            reg = 0;
            break;
    }

#if (CHIP_HAS_ASYNC_TCU)
    reg |= SYS_CTRL_MEM_DDR_DISABLE;
#endif
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Cfg_Clk_Mem_Bridge = reg;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif    
#endif
    // Hold that in the global var --> the membridge clock is set
    hal_BootSectorSetEbcFreq(ebcFreq);
}


