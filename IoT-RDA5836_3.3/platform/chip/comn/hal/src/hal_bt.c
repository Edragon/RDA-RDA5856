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
/// @file hal_bt.c                                                            //
/// BT IRQ handler                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "cfg_regs.h"
#include "gpio.h"

#include "chip_id.h"

#include "halp_bt.h"
#include "halp_sys.h"
#include "halp_debug.h"

#include "hal_bt.h"
#include "boot_sector.h"

#include "cos.h"


// =============================================================================
//  MACROS
// =============================================================================
#define RDA_APB_SCU_32KCLKCALSET_REG            0xa161001c
#define RDA_APB_GPIO_DATAOUTPUT_REG             0xa1600008

#define RDA_SCU_CLK_REQ_OUT_MODE_BIT               15

#ifdef CT_NO_DEBUG
#define HAL_BT_FREQ                             HAL_SYS_FREQ_52M
#else
#define HAL_BT_FREQ                             HAL_SYS_FREQ_78M
#endif

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// btcpu_main_entry
// -----------------------------------------------------------------------------
/// Pointer to the first function lauched by the btcpu.
// =============================================================================
EXPORT UINT32* btcpu_main_entry;

// =============================================================================
// btcpu_int_entry
// -----------------------------------------------------------------------------
/// Pointer to the function to handle IRQ.
// =============================================================================
EXPORT UINT32* btcpu_int_entry;

// =============================================================================
// btcpu_sp_context
// -----------------------------------------------------------------------------
/// Variable holding the pointer where to save the btcpu context
// =============================================================================
EXPORT UINT32* btcpu_sp_context;

// =============================================================================
// btcpu_error_code
// -----------------------------------------------------------------------------
/// Global variable holding the btcpu error code.
// =============================================================================
EXPORT volatile UINT32 btcpu_error_code;

// =============================================================================
// btcpu_error_status
// -----------------------------------------------------------------------------
/// Global variable holding the btcpu error status.
// =============================================================================
EXPORT volatile UINT32 btcpu_error_status;

// =============================================================================
// btcpu_stack_base
// -----------------------------------------------------------------------------
/// Variable holding the stack pointer for the btcpu. This variable must
/// be set before the btcpu is started.
// =============================================================================
EXPORT UINT32 btcpu_stack_base;

// =============================================================================
// g_halBtHostIrqHandler
// -----------------------------------------------------------------------------
/// Variable holding the user irq handler to process data sent from BT.
// =============================================================================
HAL_BT_HOST_IRQ_HANDLER_T g_halBtHostIrqHandler = NULL;

// =============================================================================
// g_halBtStarted
// -----------------------------------------------------------------------------
/// Variable to tell whether BT is started.
// =============================================================================
BOOL g_halBtStarted = FALSE;

// =============================================================================
// g_halBtStarted
// -----------------------------------------------------------------------------
/// Variable to tell whether BT is in low power mode.
// =============================================================================
BOOL g_halBTSleep = FALSE;

volatile UINT8 HAL_SHARED_BSS g_btcpu_wake_ind=0;

// =============================================================================
//  FUNCTIONS
// =============================================================================
extern BOOL DSM_UpgradeStructGetValid(VOID);

void hal_bt_mask_irq(void)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
      hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_IRQ_MASK_MASK);
#endif
}

PUBLIC UINT32 hal_BtReadBTClock(void)
{
    return *((UINT32*)0xa1700018);
}

PUBLIC UINT32 hal_BtReadNativeClock(void)
{
    return *((UINT32*)0xa170001c);
}
// =============================================================================
// hal_BtSetIrqHandler
// -----------------------------------------------------------------------------
/// Set an function called when an irq indicating that BT has sent some data
/// to host.
// =============================================================================
PUBLIC VOID hal_BtSetHostIrqHandler(HAL_BT_HOST_IRQ_HANDLER_T handler)
{
    g_halBtHostIrqHandler = handler;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_halBtStarted)
    {
        if (handler)
        {
            hwp_sysCtrl->BT_Cfg |= SYS_CTRL_BT_IRQ_MASK_DATAIN;
        }
        else
        {
            hwp_sysCtrl->BT_Cfg &= ~SYS_CTRL_BT_IRQ_MASK_DATAIN;
        }
    }
    hal_SysExitCriticalSection(status);
#endif
}

// =============================================================================
// hal_BtIrqHandler
// -----------------------------------------------------------------------------
/// Handles BT IRQ.
// =============================================================================
PROTECTED VOID hal_BtIrqHandler(UINT8 interruptId)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    UINT32 status;
    //hal_HstSendEvent(SYS_EVENT, 0x09170000);
    //hal_HstSendEvent(SYS_EVENT, hwp_sysCtrl->BT_Cfg);
    if (hwp_sysCtrl->BT_Cfg & SYS_CTRL_BT_WAKE_STATUS )
    {
        // btcpu will deepsleep
        //hal_HstSendEvent(SYS_EVENT, 0x14070222);
        // clear wake status interrupt
        RDA_WRITE_REG(RDA_APB_SCU_32KCLKCALSET_REG,RDA_READ_REG(RDA_APB_SCU_32KCLKCALSET_REG)&~RDA_BIT(RDA_SCU_CLK_REQ_OUT_MODE_BIT));
        g_halBTSleep = TRUE;
        hal_BtSleep();
        
    }
    if (hwp_sysCtrl->BT_Cfg & SYS_CTRL_BT_DATAIN_STATUS)
    {
        if(g_halBTSleep)
        {
            g_halBTSleep = FALSE;
            hal_BtWakeUp();
        }
        else if (g_halBtHostIrqHandler)
        {
            // Tell BT to clear the IRQ cause, and
            // disallow BT to send more data
            status = hal_SysEnterCriticalSection();
            hwp_sysCtrl->BT_Cfg &= ~SYS_CTRL_BT_DATAIN_READY;
            hal_SysExitCriticalSection(status);
            //SYSinit_Gpio_Write(2,0); // clear datain interrupt
            RDA_WRITE_REG(RDA_APB_GPIO_DATAOUTPUT_REG,RDA_READ_REG(RDA_APB_GPIO_DATAOUTPUT_REG)&~RDA_BIT(2));
            // Process the data
            (*g_halBtHostIrqHandler)();
        }
        else
        {
            // No user handler yet. Just disable the irq
            status = hal_SysEnterCriticalSection();
            hwp_sysCtrl->BT_Cfg &= ~SYS_CTRL_BT_IRQ_MASK_DATAIN;
            hal_SysExitCriticalSection(status);
        }
    }
#endif
}


// =============================================================================
// hal_BtStart
// -----------------------------------------------------------------------------
/// Start BT CPU.
// =============================================================================
PUBLIC VOID hal_BtStart(VOID *main, VOID *irqHdlr, VOID *stackStartAddr)
{
    UINT32 status;
#ifdef SERVICE_BASE_FS_SUPPORT
    if(DSM_UpgradeStructGetValid())
    {
        BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();
        if(pUpgradeStruct->step != 3) // upgrade finish
            return;
    }
#endif    
    hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_BT_FREQ, NULL);
    status = hal_SysEnterCriticalSection();

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)

    hwp_configRegs->Alt_mux_select = CFG_REGS_TPORT_TPORT|CFG_REGS_DEBUG_PORT_DAI_SIMPLE;// 0x803;

    hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_WAKEBT|SYS_CTRL_BT_DATA2BT|SYS_CTRL_BT_IRQ_MASK_MASK);  //mask interrupt set data2bt low  
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_Sys_Enable2 = SYS_CTRL_ENABLE_SYS_BT;
    hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_BT26M|SYS_CTRL_ENABLE_OC_BT32K/*|SYS_CTRL_ENABLE_OC_CLK_OUT*/;
    hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_BT;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
	//hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_WAKEBT|SYS_CTRL_BT_DATA2BT|SYS_CTRL_BT_IRQ_MASK_MASK);  //mask interrupt set data2bt low  
	
    hwp_sysCtrl->BT_Cfg |= SYS_CTRL_BT_IRQ_MASK_WAKE|
                           SYS_CTRL_BT_IRQ_MASK_DATAIN|
                           SYS_CTRL_BT_WAKEBT;

#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    //hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_SYS_FREQ_52M, NULL);
    hwp_sysIrq->mask_set = 0x1ff;
    hwp_sysCtrl->per_module_en |= SYS_CTRL_BT_EN;
#endif
    g_halBtStarted = TRUE;
    hal_SysExitCriticalSection(status);
}


// =============================================================================
// hal_BtStop
// -----------------------------------------------------------------------------
/// Stop BT CPU.
// =============================================================================
PUBLIC VOID hal_BtStop(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_BT;
    hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_BT26M|SYS_CTRL_DISABLE_OC_BT32K;
    hwp_sysCtrl->Clk_Sys_Disable2 = SYS_CTRL_DISABLE_SYS_BT;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_IRQ_MASK_MASK|SYS_CTRL_BT_WAKEBT);
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    hwp_sysIrq->mask_clear = 0x1ff;
    hwp_sysCtrl->per_module_en &= ~SYS_CTRL_BT_EN;
#endif
    g_halBtStarted = FALSE;

    hal_SysExitCriticalSection(status);
    hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_SYS_FREQ_32K, NULL);
}

// =============================================================================
// hal_BtSleep
// -----------------------------------------------------------------------------
/// Try to release frequence for BT if it is in sleep state.
// =============================================================================
PUBLIC VOID hal_BtSleep(VOID)
{
        HAL_TRACE(HAL_DBG_TRC, 0, "Enter hal_BtSleep");
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        //hwp_sysCtrl->Clk_Sys_Disable2 = SYS_CTRL_DISABLE_SYS_BT;
        hwp_sysCtrl ->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_BT26M;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;	
#endif
        //hwp_sysCtrl->BT_Cfg |= SYS_CTRL_BT_IRQ_MASK_WAKE;
        //hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_WAKEBT);
        hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_SYS_FREQ_32K, NULL);
}

// =============================================================================
// hal_BtWakeUp
// -----------------------------------------------------------------------------
/// BT CPU wants wake up.
// =============================================================================
PUBLIC VOID hal_BtWakeUp(VOID)
{
        HAL_TRACE(HAL_DBG_TRC, 0, "Enter hal_BtWakeUp");
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        //hwp_sysCtrl->Clk_Sys_Enable2 = SYS_CTRL_ENABLE_SYS_BT;
        hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_BT26M;
         hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
        //hwp_sysCtrl->BT_Cfg &= ~(SYS_CTRL_BT_WAKEBT);
        hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_BT_FREQ, NULL);
}


// =============================================================================
// hal_BtNotifyDataSent
// -----------------------------------------------------------------------------
/// Tell BT that host just sends some data.
// =============================================================================
PUBLIC VOID hal_BtNotifyDataSent(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
    // Toggle SYS_CTRL_BT_DATA2BT bit
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    if (hwp_sysCtrl->BT_Cfg & SYS_CTRL_BT_DATA2BT)
    {
        hwp_sysCtrl->BT_Cfg &= ~SYS_CTRL_BT_DATA2BT;
    }
    else
    {
        hwp_sysCtrl->BT_Cfg |= SYS_CTRL_BT_DATA2BT;
    }
#endif
    hal_SysExitCriticalSection(status);
}

// =============================================================================
// hal_IsBtStart
// -----------------------------------------------------------------------------
/// Tell if BT is started.
// =============================================================================
PUBLIC BOOL hal_IsBtStart(VOID)
{
	return g_halBtStarted;
}

// =============================================================================
// hal_IsBtSleep
// -----------------------------------------------------------------------------
/// Tell if BT is sleeped.
// =============================================================================
PUBLIC BOOL hal_IsBtSleep(VOID)
{
	return g_halBTSleep;
}

PUBLIC BOOL hal_IsBtDead(VOID)
{
       if(!g_halBtStarted)
            return FALSE;
       if(g_btcpu_wake_ind)
       {
            g_btcpu_wake_ind = 0;
            return FALSE;
       }
	return TRUE;
}
PUBLIC void hal_BtEnableSleep(BOOL sleep)
{
    if(sleep)
        hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_SYS_FREQ_32K, NULL);
    else
        hal_SysRequestFreq(HAL_SYS_FREQ_BTCPU, HAL_BT_FREQ, NULL);
}


