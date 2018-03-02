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
///     @file hal_zsp.c
///     Implementation of HAL's ZsP Driver. 
///     Caution: This file is strictly confidential!!!
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "zsp_ahb.h"
#include "zsp_cfg.h"
#include "zsp_ram.h"
#include "sys_ctrl.h"

#include "hal_mem_map.h"
#include "hal_error.h"
#include "hal_zsp.h"
#include "halp_zsp.h"
#include "halp_debug.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "sxr_tls.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
#include "hal_zsp.h"


//#define zsp_dtcm_share_mem_flash_start               (0)  //Load$$ZSP_DTCM_SHARE_MEM_RW$$Base
//#define zsp_dtcm_share_mem_ram_start                 (0)  //Image$$ZSP_DTCM_SHARE_MEM_RW$$Base
//#define zsp_dtcm_share_mem_flash_length              (0)  //  Image$$ZSP_DTCM_SHARE_MEM_RW$$Length

 UINT16 zsp_dtcm_share_mem_flash_start = 0; 
 UINT16 zsp_dtcm_share_mem_ram_start = 0; 
 UINT16 zsp_dtcm_share_mem_flash_length = 0; 

#else
//  ---------- Internal defines. ---------- 

// There might be a bug in ZSP h/w - the semaphore might never be available
// in some cases, and when this happens, it keeps so even after soft reset.
// Workaround:
// Disable h/w semaphore checking (zspCfg sema)
#define CHECK_ZSP_CFG_SEMA 0

#define toZspLAddr(n)    (((INT32)(n)>>1) & ZSP_AHB_DMA_LADDR_MASK)
#define toZspEAddr(n)    (((INT32)(n))    & ZSP_AHB_DMA_EADDR_MASK)


// Global variable storing the zsp irq handler
PRIVATE HAL_ZSP_IRQ_HANDLER_T g_halZspRegistry   = NULL; 

// Global variable containing the last loaded ZsP code
PRIVATE INT32 * g_halZspLoadedCode  = NULL;

// Global variable containing the pcVal corresponding to the loaded ZsP code
PRIVATE UINT16 g_halZspPcVal  = 0;

// Global variables containing the critical section range
PRIVATE UINT16 g_halZspPcValCriticalSecMin  = 0;
PRIVATE UINT16 g_halZspPcValCriticalSecMax  = 0;
#endif
// Global variable telling if the open procedure is finished
PRIVATE volatile HAL_ZSP_STATE_T g_halZspState  = HAL_ZSP_CLOSE_DONE;
// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
void hal_zsp_clock_enable(void)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
     hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->BB_Rst_Clr = SYS_CTRL_CLR_RST_ZSP;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
#endif
}

HAL_ZSP_CFG_T   ZSP_DTCM_SHARE_MEM g_zspCfg = {{0x0},0x1,0x0,0x1,0x0,0x0,{0x0},{0x0},{0xff}};

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC HAL_ERR_T hal_ZspOpen (CONST HAL_ZSP_CFG_T * pCfg)
{
    // g_zspCfg.zspMode = ZSP_MODE_ESCO; 
    // 0x50000000 
    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFC; 
    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFE; 
    // 0x50000038, set LSB 24 bits 
    hwp_sysCtrl->zsp_ctrl_status = 0x9001; 

    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFF; 

    // Booting ZSP
    hal_ZspMbSend(HAL_ZSP_MB_MCU2ZSP, HAL_ZSP_MB_ID_OPEN);
}
// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC VOID hal_ZspMbSend(HAL_ZSP_MB_DIR_T direction, HAL_MCU2ZSP_MB_ID_T id)
{
    if (HAL_ZSP_MB_ZSP2MCU == direction) 
    {
     // mcu2zsp_imr, clear id bit to enable its interrupt
     hwp_ahb_mailbox->mcu2zsp_imr &= ~((UINT32)id); 
     // After tirgger ZSP, the state will kept until zsp2mcu_icr set to 1, in other words, the state 
     // must be clear manually
     hwp_ahb_mailbox->mcu2zsp_irq_trig  = id; 
    } 
    else if (HAL_ZSP_MB_MCU2ZSP == direction) 
    {

    }
    else 
    {
        hal_DbgAssert("Invalid MB DIR.");
    }

}

#else

PUBLIC HAL_ERR_T hal_ZspOpen(CONST HAL_ZSP_CFG_T * pCfg)
{
    union {
        UINT32 reg;
        HAL_ZSP_WAKEUP_EVENT_T bitfield;
    } event;

    union 
    {
        UINT32 reg;
        HAL_ZSP_IRQ_STATUS_T bitfield;
    } irq;

    HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspOpen : starting open procedure...");

    BOOL openZsp = FALSE;

    UINT32 status = hal_SysEnterCriticalSection();
    if (g_halZspState == HAL_ZSP_CLOSE_DONE)
    {
        g_halZspState = HAL_ZSP_OPEN_STARTED;
        openZsp = TRUE;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->BB_Rst_Clr = SYS_CTRL_CLR_RST_ZSP;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
    }
    hal_SysExitCriticalSection(status);

    if (!openZsp)
    {
        HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0,
            "hal_ZspOpen : exiting on ERROR, zsp state busy: %d",
            g_halZspState);
        return HAL_ERR_RESOURCE_BUSY;
    }

    // -----------------------------------
    // Check if all resources are available
    // -----------------------------------
#if (CHECK_ZSP_CFG_SEMA)
    if (hwp_zspCfg->Sema == 0) 
    {
        // The ZsP semaphore is used as a general semaphore
        // for the whole open procedure. If its value is 0,
        // it means that we are attempting to open the ZsP 
        // module while it is already opened (and not closed).

        // someone else is using ZSP
        g_halZspState = HAL_ZSP_OPEN_FAILED;

        HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspOpen : exiting on ERROR, zsp sema already taken.");
        return HAL_ERR_RESOURCE_BUSY;
    }
#endif // CHECK_ZSPCFG_SEMA

    // -----------------------------------
    // Start the ZsP open procedure
    // -----------------------------------

    // -----------------------------------
    // Set the ZsP irq mask and handler
    // -----------------------------------
    hal_SysRequestFreq(HAL_SYS_FREQ_ZSP, HAL_SYS_FREQ_26M, 0);
    // Clear the interrupt mask and clear all XCPU cause
    hwp_zspAhb->Irq_Mask_Clr = ZSP_AHB_XCPU_IRQ_MASK_MASK;
    hwp_zspAhb->Irq_Status = ZSP_AHB_XCPU_IRQ_STATUS_MASK;

    // Set ZsP mask on XCPU irq (the unused fields should not be set)
    irq.bitfield = pCfg->irqMask;
    irq.bitfield.unused1 = 0;
    irq.bitfield.unused2 = 0;
    hwp_zspAhb->Irq_Mask_Set = (irq.reg) & ZSP_AHB_XCPU_IRQ_MASK_MASK;

    // Set the user's irq handler
    g_halZspRegistry = pCfg->zspIrqHandler;


    // -----------------------------------
    // Clear the wake status 
    // Set the ZsP wakeup mask
    // -----------------------------------

    hwp_zspCfg->Wakeup_Status = ZSP_CFG_WAKEUP_STATUS_MASK;
    event.bitfield = pCfg->eventMask;
    hwp_zspCfg->Wakeup_Mask = event.reg;

    if (pCfg->zspCode == NULL)
    {
        // If the pointer is null, only the ZsP DMA will be used,
        // so the semaphore will be taken.
        // The ZsP DMA cannot be used if the driver is not opened.
        // Normally the ZsP DMA cannot be busy at this point, but
        // this check can prevent from bad usage of the driver.
        if (hwp_zspAhb->DMA_Sema == 0) 
        {
            // someone else is using ZSP DMA
            g_halZspState = HAL_ZSP_OPEN_FAILED;

            HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspOpen : exiting on ERROR, dma sema already taken.");
         hal_SysRequestFreq(HAL_SYS_FREQ_ZSP, HAL_SYS_FREQ_32K, 0);
            return HAL_ERR_RESOURCE_BUSY;
        }
    }

    // -----------------------------------
    // Load the ZsP code (if necessary)
    // -----------------------------------

    // reload only if the new code is different.
    // If the pointer is null, only the ZsP DMA will be used,
    // so there is no need to load a ZsP code.
    if ((g_halZspLoadedCode != (INT32 *)pCfg->zspCode) && (pCfg->zspCode != NULL))
    {
        // update the loaded ZsP code variables
        g_halZspLoadedCode          = (INT32 *)pCfg->zspCode;
        g_halZspPcVal               = pCfg->pcVal;
        g_halZspPcValCriticalSecMin = pCfg->pcValCriticalSecMin;
        g_halZspPcValCriticalSecMax = pCfg->pcValCriticalSecMax;

        // check if the semaphore is available
        if (hwp_zspAhb->DMA_Sema == 0) 
        {
            // someone else is using ZSP DMA
            g_halZspState = HAL_ZSP_OPEN_FAILED;

            HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspOpen : exiting on ERROR, dma sema already taken.");
         hal_SysRequestFreq(HAL_SYS_FREQ_ZSP, HAL_SYS_FREQ_32K, 0);
            return HAL_ERR_RESOURCE_BUSY;
        }

        // load code in RAM_I
        hwp_zspAhb->DMA_LAddr = toZspLAddr(hwp_zspRam->zsp_ram_i_base);
        hwp_zspAhb->DMA_EAddr = toZspEAddr(pCfg->zspCode);
        hwp_zspAhb->DMA_Cfg   = ZSP_AHB_DMA_RUN | ZSP_AHB_DMA_SIZE(pCfg->zspCodeSize);

        if (pCfg->needOpenDoneIrq)
        {
            // set ZsP DMA irq mask 
            hwp_zspAhb->Irq_Mask_Set = ZSP_AHB_XCPU_DMA_IRQ_MASK;

            // return, but the open procedure is not finished
            g_halZspState = HAL_ZSP_OPEN_IN_PROGRESS;

            HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspOpen : exiting, need IRQ notification to finish procedure.");            
            return HAL_ERR_RESOURCE_RESET;
        }

        // TODO : set timeout
        // wait done
        while (!(hwp_zspAhb->Irq_Status & ZSP_AHB_XCPU_DMA_IRQ_STATUS))
        {
            //sxr_Sleep(0);
        }

        // clear status
        hwp_zspAhb->Irq_Status = ZSP_AHB_XCPU_DMA_IRQ_STATUS;

        // the code is loaded
        g_halZspState = HAL_ZSP_OPEN_DONE;

        // but a reset is required
        HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspOpen : exiting on SUCCESS.");    
        return HAL_ERR_RESOURCE_RESET;           
    }

    // the code is already loaded
    g_halZspState = HAL_ZSP_OPEN_DONE;

    // no further action required    
    // the open procedure is finished without errors
    HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspOpen : exiting on SUCCESS.");    
    return HAL_ERR_NO;
}

#endif
// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC VOID hal_ZspClose(VOID)
{

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    BOOL closeZsp = FALSE;

    UINT32 status = hal_SysEnterCriticalSection();
    HAL_ZSP_STATE_T prevZspState = g_halZspState;
    if (g_halZspState != HAL_ZSP_CLOSE_DONE &&
        g_halZspState != HAL_ZSP_CLOSE_STARTED &&
        g_halZspState != HAL_ZSP_OPEN_STARTED)
    {
        g_halZspState = HAL_ZSP_CLOSE_STARTED;
        closeZsp = TRUE;
    }
    hal_SysExitCriticalSection(status);

    if (!closeZsp)
    {
        if (prevZspState == HAL_ZSP_CLOSE_DONE)
        {
            // Already closed.
         hal_SysRequestFreq(HAL_SYS_FREQ_ZSP, HAL_SYS_FREQ_32K, 0);
            HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspClose : aborting, already closed");
        }
        else
        {
            // Someone else is configuring ZSP
            HAL_ASSERT(FALSE, "hal_ZspClose : aborting, busy: %d", prevZspState);
            //HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspClose : aborting, busy: %d", prevZspState);
        }
        return;
    }

    // Check if the module is open
    if (prevZspState == HAL_ZSP_OPEN_IN_PROGRESS)
    {
        // Closing while opening in progress...
        // If the open procedure is still ongoing,
        // it will be stopped before completion, so the code 
        // state will be unknown.
        HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspClose : closing while open is in progress");    
        g_halZspLoadedCode = NULL;
    }

    // Clear the variable that stores the zsp irq handler.
    // No more zsp irq will be handled after this point.
    g_halZspRegistry = NULL;

    // Clear the wakeup mask.
    // ZsP cannot wakeup on a non-XCPU event after this point. 
    // This prevents from inopportune wakeup of the ZsP module
    // triggered by a HW wakeup event.
    hwp_zspCfg->Wakeup_Mask = ZSP_CFG_WAKEUP_MASK(0);

    // Stop the ZsP module (if running) and flush its pipe.
    // ZsP cannot be inopportunely woken up.
    hwp_zspCfg->Ctrl = ZSP_CFG_RUN_STOP;

    // INFO : This check is normally unnecessary. 
    // After this point the ZsP module is stopped.
    while (hwp_zspCfg->Ctrl & ZSP_CFG_RUNNING);


    // this is another possibility to stop properly the DMA ....
    // -----------------------------------------------------

    // Stop the ZsP DMA (if running) without changing the DMA configuration,
    // in particular without changing a write to a read transfer, 
    // which can crash the AHB.
    hwp_zspAhb->DMA_Cfg &= ~ZSP_AHB_DMA_RUN;

    // Wait for the last burst (if any).
    while (hwp_zspAhb->DMA_Status & ZSP_AHB_DMA_ON);

    // After this point, no interrupts can be received from ZsP
    // Clear the interrupt mask and all XCPU cause.
    hwp_zspAhb->Irq_Mask_Clr = ZSP_AHB_XCPU_IRQ_MASK_MASK;
    hwp_zspAhb->Irq_Status = ZSP_AHB_XCPU_IRQ_STATUS_MASK;

    // clear the wakeup status
    hwp_zspCfg->Wakeup_Status = ZSP_CFG_WAKEUP_STATUS_MASK;

    // Reset g_halZspLoadedCode to NULL, so that ZSP can be started next time
    // even if its PC value is invalid.
#if 0
    // if the ZsP module's PC is inside the critical section corresponding
    // to the code reload function, the ZsP loaded code variable is cleared
    // because the exact state of the Zsp code is not known.
    if ((hwp_zspCfg->PC >= g_halZspPcValCriticalSecMin) && (hwp_zspCfg->PC < g_halZspPcValCriticalSecMax))
#endif
    {
        g_halZspLoadedCode = NULL;
    }

    // Release the semaphores (the order is important).
    hwp_zspAhb->DMA_Sema = 1;
    hwp_zspCfg->Sema = 1;

    status = hal_SysEnterCriticalSection();
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E) && (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK; 
    hwp_sysCtrl->BB_Rst_Set = SYS_CTRL_SET_RST_ZSP;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
    hal_SysExitCriticalSection(status);

    // Clear the open procedure
    g_halZspState = HAL_ZSP_CLOSE_DONE;

    HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspClose : exiting on SUCCESS");
    hal_SysRequestFreq(HAL_SYS_FREQ_ZSP, HAL_SYS_FREQ_32K, 0);
#endif
    return;
}
// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC HAL_ERR_T hal_ZspWakeup(HAL_ZSP_WAKEUP_ID_T wakeupId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    if (g_halZspState != HAL_ZSP_OPEN_DONE)
    {
        // the open procedure is not started or finished
        HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0,
            "hal_ZspWakeup : exiting on ERROR, zsp not opened: %d",
            g_halZspState);
        return HAL_ERR_RESOURCE_NOT_ENABLED;
    }

    if (wakeupId == HAL_ZSP_START)
    {
        if (hwp_zspCfg->Ctrl & ZSP_CFG_RUNNING)
        {
            // attempting to start ZsP while it is still running
            HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0, "hal_ZspWakeup : exiting on ERROR, zsp is running!");    
            return HAL_ERR_RESOURCE_BUSY;
        }

        // set the PC start address
        hwp_zspCfg->PC   = g_halZspPcVal;

        // set the 16-bit stack to the top of RAM_X
        hwp_zspCfg->SP16 = ZSP_CFG_SP16(toZspLAddr(&(hwp_zspRam->zsp_ram_x_base[ZSP_RAM_X_SIZE])) - 1);

        // set the 32-bit stack to the top of RAM_Y
        hwp_zspCfg->SP32 = ZSP_CFG_SP32(toZspLAddr(&(hwp_zspRam->zsp_ram_y_base[ZSP_RAM_Y_SIZE])) - 2);

        // start executing ZsP code
        hwp_zspCfg->Ctrl = ZSP_CFG_RUN_START;
    }
    else
    {
        // set a soft wakeup event
        hwp_zspCfg->Ctrl = (wakeupId == HAL_ZSP_WAKEUP_EVENT_1) ? ZSP_CFG_RUN_SOFTWAKEUP1 : ZSP_CFG_RUN_SOFTWAKEUP0;
    }

    // HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspWakeup : exiting on SUCCESS");    
#endif
    return HAL_ERR_NO;
}


// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC VOID* hal_ZspGetPointer(INT32 zspLocalAddr)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    // return an global pointer to zspLocalAddr
    return ((VOID*)(&(hwp_zspRam->zsp_ram_x_base)[zspLocalAddr*2]));
#else
    return NULL;
#endif
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC INT32 * hal_ZspGetDmaiPointer(INT32 * zspExternAddr, HAL_ZSP_DMA_DIR_T wr1Rd0, HAL_ZSP_DMA_TRANSFER_T sngl1brst0, HAL_ZSP_DMA_B2S_T b2s)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    if (NULL == zspExternAddr)
    { 
        // protect (do not create write NULL pointers)
        return NULL;
    }
    else
    {
        INT32 DmaiEAddr = ((INT32)zspExternAddr) & ZSP_CFG_DMA_EADDR_MASK;

        // configure the single/burst parameter
        if (sngl1brst0 == HAL_ZSP_DMA_SINGLE)
        {
            DmaiEAddr |= ZSP_CFG_DMA_SINGLE;
        }

        // configure the read/write parameter
        if (wr1Rd0 == HAL_ZSP_DMA_WRITE)
        {
            DmaiEAddr |= ZSP_CFG_DMA_WRITE_WRITE;
        }   

        // return an DMAI configuration pointer for the hwp_zspAhb->DMA_EAddr
        return (INT32 *)DmaiEAddr;
    }
#else
    return NULL;
#endif
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC HAL_ERR_T hal_ZspDmaStart (CONST HAL_ZSP_DMA_CFG_T * pCfg)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    UINT32 dmaCfg;

    // check if sema is available and that the open procedure is finished
    if ((hwp_zspAhb->DMA_Sema == 0) || (g_halZspState != HAL_ZSP_OPEN_DONE)) 
    {
        HAL_TRACE(HAL_ZSP_TRC|TSTDOUT, 0,
            "hal_ZspDmaStart : exiting on ERROR, zsp not opened or in use: %d",
            g_halZspState);
        return HAL_ERR_RESOURCE_BUSY;
    }

    // clear ZsP DMA status & irq mask
    hwp_zspAhb->Irq_Status   = ZSP_AHB_XCPU_DMA_IRQ_STATUS;
    hwp_zspAhb->Irq_Mask_Clr = ZSP_AHB_XCPU_DMA_IRQ_MASK; 

    // load non_regression test
    hwp_zspAhb->DMA_LAddr = toZspLAddr(pCfg->zspLocalAddr);
    hwp_zspAhb->DMA_EAddr = toZspEAddr(pCfg->extAddr);

    dmaCfg = ZSP_AHB_DMA_RUN | ZSP_AHB_DMA_SIZE(pCfg->size);

    if (pCfg->wr1Rd0 == HAL_ZSP_DMA_WRITE) 
    {
        dmaCfg |= ZSP_AHB_DMA_DIR_WRITE;
    }

    // if an interrupt is to be generated, enable the mask
    if (pCfg->needIrq)
    {
        hwp_zspAhb->Irq_Mask_Set = ZSP_AHB_XCPU_DMA_IRQ_MASK;
    }

    // configure the rest of the parameters and start the transfer
    hwp_zspAhb->DMA_Cfg   = dmaCfg;

    HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspDmaStart : exiting on SUCCESS");    
    return HAL_ERR_NO;
#else
    return NULL;
#endif 
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC BOOL hal_ZspDmaDone(VOID)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    return ((hwp_zspAhb->Irq_Status & ZSP_AHB_XCPU_DMA_IRQ_STATUS) ? TRUE : FALSE);   
#else
    return TRUE;
#endif
}


// ============================================================================
// hal_ZspIrqHandler
// ----------------------------------------------------------------------------
/// ZsP IRQ handler, clearing the IRQ cause regiter and calling the user handler
/// defined by hal_ZspIrqSetHandler. It also checks if the open procedure is finished
/// when the user chooses to confirm the end of the open procedure with an ZsP DMA 
/// interrupt
/// @param interruptId Id with which the handler is entered
// ============================================================================
PROTECTED VOID hal_ZspIrqHandler(UINT8 interruptId)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    union
    {
        UINT32 reg;
        HAL_ZSP_IRQ_STATUS_T bitfield;
    } cause;

    // read the cause
    cause.reg = hwp_zspAhb->Irq_Status & ZSP_AHB_XCPU_IRQ_CAUSE_MASK;

    // clear all XCPU cause 
    hwp_zspAhb->Irq_Status = cause.reg;

    BOOL codeLoadingDone = FALSE;

    UINT32 status = hal_SysEnterCriticalSection();
    HAL_ZSP_STATE_T prevZspState = g_halZspState;
    // If the open procedure is ongoing
    if (g_halZspState == HAL_ZSP_OPEN_IN_PROGRESS)
    {
        // the only allowed irq during the open procedure is dmaDone
        if (hwp_zspAhb->Irq_Status & ZSP_AHB_XCPU_DMA_IRQ_STATUS)
        {
            // The code loading is done, so the open procedure is finished 
            g_halZspState = HAL_ZSP_OPEN_DONE;
            codeLoadingDone = TRUE;
            // Clr the ZsP DMA irq mask 
            hwp_zspAhb->Irq_Mask_Clr = ZSP_AHB_XCPU_DMA_IRQ_MASK;
        }
    }
    hal_SysExitCriticalSection(status);

    if (prevZspState == HAL_ZSP_OPEN_IN_PROGRESS)
    {
        if (codeLoadingDone)
        {
            HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspIrqHandler : open procedure finished on SUCCESS");    
        }
    }
    else 
    {
        // call the user IRQ handler function.
        if (g_halZspRegistry)
        {
            // HAL_TRACE(HAL_ZSP_TRC, 0, "hal_ZspIrqHandler : calling user handler");    
            g_halZspRegistry(&cause.bitfield);
        }
    }
#endif 
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC BOOL HAL_FUNC_RAM hal_GetZspDma(void)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    return (g_halZspState  != HAL_ZSP_CLOSE_DONE);
#else
    return 0;
#endif
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC UINT32 hal_ZspIntrClose(VOID)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    UINT32 now,zsp_time;
    now = hal_TimGetUpTime();

    if(g_halZspState != HAL_ZSP_OPEN_DONE)
                asm("break 7");

    UINT32 my_Wakeup_Mask = hwp_zspCfg->Wakeup_Mask;
    hwp_zspCfg->Wakeup_Mask = my_Wakeup_Mask&0xc;
    while ((hwp_zspCfg->Ctrl & ZSP_CFG_RUNNING) | (hwp_zspAhb->DMA_Status & ZSP_AHB_DMA_ON))
    {
        zsp_time = hal_TimGetUpTime();
        if (zsp_time - now >(16384>>2)) 
        {
            asm("break 7");
            return 0xffffffff;
        }

        if(g_halZspState != HAL_ZSP_OPEN_DONE)
        {
            asm("break 7");
            return 0xffffffff;
        }
    }
    return my_Wakeup_Mask;
#else
    return 0;
#endif
}

// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
/// @param 
// =============================================================================
PUBLIC VOID  hal_ZspIntrOpen(UINT32 my_Wakeup_Mask)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    if(my_Wakeup_Mask == 0xffffffff)
        return;
    UINT32 new_Wakeup_Mask = hwp_zspCfg->Wakeup_Mask;
//    hwp_zspCfg->Wakeup_Status = ZSP_CFG_WAKEUP_STATUS_MASK;    
    hwp_zspCfg->Wakeup_Mask = (my_Wakeup_Mask&0x33) | (new_Wakeup_Mask&0xc);
#endif
}

#include "hal_aif.h"

extern HAL_AIF_CONFIG_T g_audCodecToneAifCfg; 

INT16 zsp_TestBuff1[960] = {
0xFFF6, 0xFFE1, 0x0007, 0x005A, 0x007B, 0x0043, 0x0005, 0x000C, 
0x002D, 0x0010, 0xFFC4, 0xFFA2, 0xFFC8, 0xFFF6, 0xFFFB, 0xFFF5, 
0x0005, 0x001F, 0x002A, 0x002D, 0x003D, 0x004D, 0x0055, 0x0059, 
0x005E, 0x005A, 0x0048, 0x003B, 0x0038, 0x0031, 0x001F, 0x0012, 
0x0016, 0x0017, 0x0007, 0xFFF6, 0xFFF6, 0xFFFC, 0xFFF6, 0xFFE9, 
0xFFE8, 0xFFF0, 0xFFF8, 0xFFFE, 0x0008, 0x000E, 0x000B, 0x0009, 
0x0012, 0x001A, 0x0017, 0x000C, 0x0006, 0x0004, 0xFFFC, 0xFFF6, 
0xFFF5, 0xFFF9, 0xFFF9, 0xFFF6, 0xFFF4, 0xFFF4, 0xFFEC, 0xFFE4, 
0xFFE1, 0xFFE2, 0xFFE0, 0xFFE2, 0xFFED, 0xFFF8, 0xFFF6, 0xFFE8, 
0xFFE1, 0xFFE7, 0xFFEC, 0xFFE8, 0xFFE6, 0xFFEA, 0xFFEF, 0xFFEF, 
0xFFED, 0xFFED, 0xFFEB, 0xFFEA, 0xFFEA, 0xFFEB, 0xFFEA, 0xFFE5, 
0xFFE4, 0xFFE6, 0xFFEA, 0xFFED, 0xFFF1, 0xFFF4, 0xFFF2, 0xFFF4, 
0xFFFC, 0x0007, 0x0009, 0x0001, 0xFFFA, 0xFFF8, 0xFFFA, 0xFFFD, 
0x0002, 0x0008, 0x000B, 0x000F, 0x0016, 0x001D, 0x001C, 0x0019, 
0x001C, 0x0026, 0x0029, 0x0021, 0x0016, 0x000C, 0x0004, 0x0000, 
0x0002, 0x0003, 0xFFFC, 0xFFF1, 0xFFEC, 0xFFEF, 0xFFF0, 0xFFEB, 
0xFFED, 0xFFFA, 0x0008, 0x000C, 0x0005, 0x0002, 0x0003, 0x0008, 
0x0009, 0x000C, 0x0010, 0x0015, 0x0015, 0x000D, 0x0003, 0xFFFA, 
0xFFF5, 0xFFF2, 0xFFEC, 0xFFE4, 0xFFDA, 0xFFD0, 0xFFC7, 0xFFC2, 
0xFFC3, 0xFFC7, 0xFFC7, 0xFFC5, 0xFFC9, 0xFFD7, 0xFFE9, 0xFFF6, 
0x0002, 0x000E, 0x0019, 0x0024, 0x002F, 0x003D, 0x0049, 0x004E, 
0x004F, 0x0050, 0x004E, 0x0046, 0x0039, 0x002A, 0x001A, 0x0009, 
0xFFFA, 0xFFF2, 0xFFEE, 0xFFE5, 0xFFD2, 0xFFBE, 0xFFBA, 0xFFC1, 
0xFFC9, 0xFFC7, 0xFFC0, 0xFFC3, 0xFFD1, 0xFFE4, 0xFFF2, 0xFFFE, 
0x0008, 0x000E, 0x0012, 0x0019, 0x0020, 0x0020, 0x0017, 0x0010, 
0x0012, 0x0014, 0x0012, 0x000B, 0x000A, 0x000D, 0x000C, 0x0005, 
0xFFFD, 0xFFF6, 0xFFEB, 0xFFE1, 0xFFE0, 0xFFEA, 0xFFF1, 0xFFF5, 
0xFFFE, 0x000D, 0x001A, 0x0015, 0x0004, 0xFFFF, 0x000C, 0x0022, 
0x002D, 0x002C, 0x0023, 0x001C, 0x001A, 0x0019, 0x0016, 0x000D, 
0xFFFC, 0xFFEF, 0xFFEA, 0xFFEB, 0xFFE2, 0xFFCE, 0xFFBA, 0xFFB4, 
0xFFBA, 0xFFC2, 0xFFC7, 0xFFCC, 0xFFD6, 0xFFE2, 0xFFEC, 0xFFFB, 
0x000F, 0x001D, 0x0022, 0x001E, 0x001E, 0x0021, 0x0025, 0x0029, 
0x002C, 0x002E, 0x0029, 0x0021, 0x0021, 0x0025, 0x0027, 0x0024, 
0x0024, 0x002A, 0x002D, 0x0025, 0x001B, 0x0018, 0x0018, 0x0015, 
0x0011, 0x0011, 0x0018, 0x001E, 0x0021, 0x0022, 0x0023, 0x0023, 
0x0024, 0x0021, 0x0016, 0x000B, 0x000A, 0x0011, 0x0013, 0x0007, 
0xFFF7, 0xFFF0, 0xFFF1, 0xFFEB, 0xFFD8, 0xFFC0, 0xFFAE, 0xFFA5, 
0xFFA3, 0xFFA8, 0xFFAF, 0xFFB2, 0xFFB6, 0xFFBD, 0xFFC6, 0xFFCA, 
0xFFCA, 0xFFD3, 0xFFE4, 0xFFF6, 0x0001, 0x000A, 0x0018, 0x002A, 
0x0038, 0x003C, 0x0035, 0x0026, 0x001A, 0x001A, 0x001E, 0x001E, 
0x001B, 0x001C, 0x0020, 0x001D, 0x000E, 0xFFFE, 0xFFF7, 0xFFF4, 
0xFFEF, 0xFFF1, 0xFFFF, 0x000E, 0x000E, 0x0005, 0x0001, 0x0004, 
0x0009, 0x0011, 0x0022, 0x0033, 0x0034, 0x0026, 0x001D, 0x0024, 
0x0030, 0x0034, 0x002E, 0x002C, 0x002D, 0x002E, 0x002C, 0x0028, 
0x0022, 0x0019, 0x000F, 0x0008, 0x0005, 0x0002, 0xFFFB, 0xFFF4, 
0xFFEC, 0xFFE7, 0xFFE4, 0xFFE4, 0xFFDD, 0xFFD4, 0xFFD3, 0xFFDD, 
0xFFEC, 0xFFF3, 0xFFF1, 0xFFEC, 0xFFE6, 0xFFE7, 0xFFED, 0xFFF7, 
0xFFFD, 0xFFFB, 0xFFF9, 0xFFFA, 0xFFFC, 0xFFF6, 0xFFEA, 0xFFE3, 
0xFFE2, 0xFFE1, 0xFFDB, 0xFFD3, 0xFFD1, 0xFFD5, 0xFFDD, 0xFFE6, 
0xFFEE, 0xFFF6, 0xFFFF, 0x0003, 0x0002, 0x0003, 0x000A, 0x0013, 
0x000D, 0xFFF4, 0xFFE3, 0xFFEC, 0x0002, 0x000A, 0x0001, 0xFFFF, 
0x000D, 0x0017, 0x000F, 0x0004, 0x0008, 0x0016, 0x001C, 0x0019, 
0x0017, 0x001C, 0x001E, 0x0017, 0x000F, 0x0012, 0x001C, 0x0024, 
0x0023, 0x001E, 0x0019, 0x001A, 0x001F, 0x0024, 0x0023, 0x0020, 
0x001A, 0x0014, 0x0009, 0xFFFD, 0xFFF9, 0xFFFB, 0xFFF5, 0xFFE3, 
0xFFCF, 0xFFC7, 0xFFC9, 0xFFCB, 0xFFCF, 0xFFD6, 0xFFDF, 0xFFDE, 
0xFFD7, 0xFFD1, 0xFFD3, 0xFFD8, 0xFFDB, 0xFFE0, 0xFFEA, 0xFFF3, 
0xFFF5, 0xFFF2, 0xFFF3, 0xFFFA, 0x0002, 0x0008, 0x0009, 0x0007, 
0x0003, 0x0002, 0x0003, 0xFFFD, 0xFFEE, 0xFFE0, 0xFFD7, 0xFFD3, 
0xFFC9, 0xFFBC, 0xFFB7, 0xFFC2, 0xFFD0, 0xFFDE, 0xFFEA, 0xFFF1, 
0xFFF4, 0xFFF3, 0xFFF9, 0x000D, 0x0022, 0x002E, 0x0030, 0x0030, 
0x0036, 0x003E, 0x0044, 0x0042, 0x0037, 0x002B, 0x0024, 0x0022, 
0x001D, 0x0013, 0x000C, 0x0008, 0x0002, 0xFFF8, 0xFFF1, 0xFFEF, 
0xFFE8, 0xFFDE, 0xFFDB, 0xFFE4, 0xFFEE, 0xFFEF, 0xFFEF, 0xFFF8, 
0x0007, 0x000B, 0x000A, 0x000A, 0x000E, 0x000D, 0x0009, 0x0008, 
0x0009, 0x0007, 0x0004, 0x0008, 0x0011, 0x0011, 0x0006, 0xFFFB, 
0xFFF5, 0xFFF0, 0xFFE1, 0xFFD3, 0xFFCD, 0xFFCB, 0xFFCA, 0xFFC9, 
0xFFD0, 0xFFD8, 0xFFE0, 0xFFEA, 0xFFF9, 0x0005, 0x000B, 0x000E, 
0x0018, 0x0025, 0x0031, 0x0039, 0x003D, 0x003D, 0x0038, 0x0032, 
0x0031, 0x002E, 0x0026, 0x001E, 0x0019, 0x0015, 0x000D, 0x0002, 
0xFFF9, 0xFFF2, 0xFFEB, 0xFFE9, 0xFFF0, 0xFFFC, 0x0003, 0x0005, 
0x0008, 0x000D, 0x000E, 0x000D, 0x000F, 0x0011, 0x000F, 0x000A, 
0x0007, 0x000A, 0x0008, 0x0003, 0x0003, 0x0007, 0x000A, 0x0003, 
0xFFF9, 0xFFF1, 0xFFEC, 0xFFE6, 0xFFE1, 0xFFDA, 0xFFD1, 0xFFC7, 
0xFFC1, 0xFFC1, 0xFFC4, 0xFFCA, 0xFFD7, 0xFFE8, 0xFFF5, 0xFFF7, 
0xFFF9, 0x0003, 0x0010, 0x0015, 0x0015, 0x001B, 0x0022, 0x0026, 
0x0022, 0x001C, 0x001A, 0x001B, 0x0019, 0x0016, 0x000E, 0xFFFE, 
0xFFE9, 0xFFDF, 0xFFE7, 0xFFF5, 0xFFFE, 0xFFF9, 0xFFF4, 0xFFF9, 
0x000B, 0x001D, 0x0023, 0x001D, 0x001D, 0x0026, 0x0035, 0x003B, 
0x0038, 0x0038, 0x0038, 0x0037, 0x002C, 0x001B, 0x000E, 0x0008, 
0x0007, 0x0002, 0xFFF8, 0xFFE8, 0xFFDB, 0xFFD3, 0xFFD0, 0xFFCB, 
0xFFC3, 0xFFBD, 0xFFBF, 0xFFC3, 0xFFC4, 0xFFC6, 0xFFCA, 0xFFD3, 
0xFFDC, 0xFFE8, 0xFFF6, 0x0005, 0x0012, 0x001A, 0x0023, 0x002B, 
0x002C, 0x0025, 0x001D, 0x0014, 0x000E, 0x000D, 0x000A, 0x0004, 
0xFFF9, 0xFFF6, 0xFFFC, 0x0003, 0xFFFB, 0xFFEB, 0xFFE1, 0xFFE4, 
0xFFF2, 0xFFFF, 0x0006, 0x000C, 0x000B, 0x0005, 0x0004, 0x000C, 
0x0015, 0x0016, 0x0011, 0x000C, 0x0007, 0x0004, 0xFFFE, 0xFFFA, 
0xFFF5, 0xFFEF, 0xFFEC, 0xFFF0, 0xFFF2, 0xFFED, 0xFFE4, 0xFFE4, 
0xFFE8, 0xFFE8, 0xFFE3, 0xFFE8, 0xFFF5, 0xFFFC, 0xFFF3, 0xFFE7, 
0xFFEA, 0xFFF5, 0xFFFA, 0xFFF6, 0xFFF7, 0xFFFC, 0xFFFF, 0xFFFB, 
0xFFFB, 0x0003, 0x0009, 0x0007, 0x0003, 0x0005, 0x000C, 0x000F, 
0x000C, 0x0006, 0x0000, 0xFFFB, 0xFFF5, 0xFFED, 0xFFE5, 0xFFE2, 
0xFFE3, 0xFFE0, 0xFFDF, 0xFFE3, 0xFFEC, 0xFFF5, 0xFFF5, 0xFFF1, 
0xFFF0, 0xFFFB, 0x0009, 0x0015, 0x001A, 0x001F, 0x0023, 0x0026, 
0x0025, 0x001D, 0x0014, 0x000E, 0x0010, 0x0010, 0x0006, 0xFFF9, 
0xFFF6, 0xFFF9, 0xFFF6, 0xFFEA, 0xFFDE, 0xFFDD, 0xFFE2, 0xFFE4, 
0xFFE4, 0xFFE7, 0xFFF1, 0xFFFC, 0x000B, 0x0016, 0x001A, 0x0017, 
0x001A, 0x0027, 0x0036, 0x003C, 0x003C, 0x003E, 0x0040, 0x003A, 
0x0032, 0x002A, 0x0023, 0x0018, 0x0010, 0x0010, 0x0014, 0x0010, 
0x0002, 0xFFF7, 0xFFF3, 0xFFF2, 0xFFF3, 0xFFF7, 0xFFFC, 0xFFFA, 
0xFFFA, 0x0006, 0x0019, 0x0023, 0x001C, 0x0014, 0x0017, 0x001E, 
0x0025, 0x002E, 0x0038, 0x0038, 0x002C, 0x001F, 0x0025, 0x0030, 
0x002E, 0x001D, 0x000E, 0x0005, 0xFFFA, 0xFFF1, 0xFFF5, 0x0001, 
0x0007, 0xFFFC, 0xFFF2, 0xFFF0, 0xFFF3, 0xFFF7, 0xFFFF, 0x0008, 
0x000A, 0x0003, 0xFFFC, 0xFFFF, 0x0007, 0x000A, 0x0008, 0x0003, 
0xFFFF, 0xFFFD, 0x0002, 0x000E, 0x0013, 0x000B, 0xFFFE, 0xFFF7, 
0xFFF9, 0xFFF9, 0xFFF5, 0xFFF5, 0xFFF9, 0xFFFC, 0xFFF8, 0xFFF8, 
0xFFFF, 0x000D, 0x0013, 0x0010, 0x0006, 0x0000, 0x0002, 0x000C, 
0x0016, 0x0015, 0x000B, 0x0004, 0x0005, 0x0009, 0x0008, 0x0004, 
0xFFFF, 0xFFF7, 0xFFED, 0xFFEB, 0xFFF0, 0xFFF4, 0xFFE6, 0xFFD5, 
0xFFD0, 0xFFDB, 0xFFE7, 0xFFE6, 0xFFDD, 0xFFDA, 0xFFE1, 0xFFE9, 
0xFFEE, 0xFFF6, 0x0000, 0x0008, 0x000C, 0x000E, 0x000E, 0x000B, 
0x0009, 0x000C, 0x000B, 0x0007, 0x0000, 0xFFFD, 0xFFFB, 0xFFF4, 
0xFFE7, 0xFFDA, 0xFFDB, 0xFFE0, 0xFFE5, 0xFFE7, 0xFFEB, 0xFFED, 
0xFFF2, 0xFFFB, 0x000A, 0x0013, 0x0016, 0x001B, 0x0025, 0x002F, 
0x0033, 0x0037, 0x003E, 0x0047, 0x0047, 0x0046, 0x0049, 0x004D, 
0x0049, 0x0039, 0x002B, 0x0026, 0x0023, 0x0017, 0x000D, 0x0008, 
0x0008, 0x0003, 0xFFFA, 0xFFF6, 0xFFF2, 0xFFE5, 0xFFD3, 0xFFC6, 
0xFFC1, 0xFFC4, 0xFFC8, 0xFFD4, 0xFFE6, 0xFFF5, 0xFFF6, 0xFFED, 
0xFFE5, 0xFFE3, 0xFFE5, 0xFFE5, 0xFFE5, 0xFFE9, 0xFFEB, 0xFFE7,
};

//58个字节补0  还需要改
INT16 zsp_TestBuff2[232] = {
0x00AD, 0xC500, 0x0000, 0x0000, 0x6D77, 0xDDB6, 0x6DDB, 0x76B7, 
0x6DDB, 0xB6DD, 0x77DB, 0xB66D, 0x13DA, 0x376A, 0x1387, 0x216E, 
0xDCA4, 0x6D77, 0xD1B6, 0x6DE4, 0x76B7, 0x6DDB, 0x9659, 0x7793, 
0xB66D, 0xDBDD, 0xB66D, 0xDA78, 0xAD70, 0x0000, 0x00C5, 0x0000, 
0x7900, 0x368D, 0xE3A1, 0xA569, 0x1B85, 0x6A6A, 0xE3B8, 0x6D77, 
0xDDB6, 0x6DDB, 0x57B8, 0x8D13, 0x27DA, 0x771B, 0xB66D, 0x9CD9, 
0xA78D, 0xDB76, 0x9D6E, 0xDB26, 0x7266, 0x9D36, 0x6DDB, 0x68B6, 
0x50D3, 0x00AD, 0xC500, 0x0000, 0x0000, 0x8956, 0xE5B8, 0x6DA2, 
0x86C8, 0x69DB, 0x46A5, 0x77DB, 0xB66D, 0x9ADD, 0xB88D, 0xDC74, 
0xA969, 0xDC96, 0x8D86, 0xE5C4, 0x695C, 0x56B6, 0x8DA3, 0xB6A6, 
0x949B, 0x3471, 0x5ADA, 0x388D, 0xDB66, 0xAD6C, 0x0000, 0x00C5, 
0x0000, 0x7700, 0xB66D, 0x5ADA, 0x358D, 0xDA44, 0x214E, 0xDCA4, 
0x8D87, 0x9DA6, 0x6DDB, 0x96B7, 0x4EDB, 0x3652, 0x77DB, 0xB66D, 
0x9BD9, 0xC76D, 0xDB76, 0x216E, 0xDCA4, 0x6D98, 0xE226, 0x8D22, 
0x6635, 0x6CE4, 0x00AD, 0xC500, 0x0000, 0x0000, 0x6D77, 0xDDB6, 
0x6DDB, 0x78B7, 0x6D94, 0xB6DD, 0xA6DB, 0x3696, 0xDBDD, 0xBA6D, 
0xD356, 0x216E, 0xDAB6, 0x9179, 0x22B9, 0x6D1B, 0x68A8, 0x4EDC, 
0x371D, 0x681A, 0xA84D, 0x9BD9, 0xC66D, 0xDB96, 0xAD88, 0x0000, 
0x00C5, 0x0000, 0x8500, 0x3871, 0xD4E1, 0x456D, 0xDC54, 0xA692, 
0xDB48, 0x6A96, 0x9936, 0x8D9C, 0x57A7, 0x691B, 0xB6DD, 0x96DB, 
0xB789, 0x2316, 0xB64D, 0xDB66, 0xDD71, 0xDBB6, 0x5158, 0x89B6, 
0x691B, 0x8737, 0x6C13, 0x00AD, 0xC500, 0x0000, 0x0000, 0x6D88, 
0x9DB6, 0x6DDB, 0x76B7, 0x6DDB, 0xC861, 0x66DA, 0x3672, 0x93A5, 
0xB68D, 0xDB86, 0xE56D, 0xD3B6, 0x6D77, 0xE6B6, 0x72A3, 0x8636, 
0x6DDB, 0xB6A1, 0x95DB, 0xA671, 0x9BE1, 0xB86D, 0xDB66, 0xAD6C, 
0x0000, 0x00C5, 0x0000, 0x7400, 0x348E, 0x14DE, 0xB669, 0xDB86, 
0x166E, 0xDB26, 0x6D86, 0xE2B6, 0x4D1A, 0x66C7, 0x6EA3, 0x371A, 
0x8513, 0xB689, 0x1BDA, 0xB66D, 0x1B95, 0x216E, 0xDAB6, 0x7276, 
0xE234, 0x695B, 0x64A6, 0x4892, 0x00AD, 0xC500, 0x0000, 0x0000, 

};

PUBLIC VOID hal_ZspTest(VOID)
{
    UINT32 i = 0;
      
    hal_HstSendEvent(BOOT_EVENT,0x2011);

    // 0x50000000 
    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFC; 
    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFE; 
    
    for (i=0; i<(UINT32)(&zsp_dtcm_share_mem_flash_length)/2; i++)
    {
      *((INT16*)&zsp_dtcm_share_mem_ram_start+i) = *((INT16*)&zsp_dtcm_share_mem_flash_start+i);
    }

    //speaker
    for (i=0; i<114; i++)
    {
      *((INT16*)&g_zspCfg.zspInputBuff[1024]+i) = *((INT16*)&zsp_TestBuff2+i);
    }
    
    hal_HstSendEvent(BOOT_EVENT, g_zspCfg.zspMode);
    hal_HstSendEvent(BOOT_EVENT, g_zspCfg.zspStatus);
    hal_HstSendEvent(BOOT_EVENT, g_zspCfg.zspUpdatePara);
    hal_HstSendEvent(BOOT_EVENT, g_zspCfg.mcu2zspMb);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspInputBuff[1024]);


    // 0x50000038, set LSB 24 bits 
    //hwp_sysCtrl->zsp_ctrl_status = 0x012002; 
    hwp_sysCtrl->zsp_ctrl_status = 0x09001;  //必须要写成ZSP端的word地址    
    hwp_sysCtrl->per_soft_reset1 = 0xFFFFFFFF;;

    hal_TimDelay(5000 MS_WAITING);

    //Trigger ZSP
    // Clear mask, enable all 32 mailbox ID 
    hwp_ahb_mailbox->mcu2zsp_imr = 0x00000000; 
    // zsp2mcu_irsr , raw status, no matter mask set or not
    // zsp2mcu_isr, is mask set, then it may different with zsp2mcu_irsr   
    hal_TimDelay(50 MS_WAITING);

    hwp_ahb_mailbox->mcu2zsp_irq_trig = AHB_MAILBOX_MCU2ZSP_IRQ_TRIG(1); 
    // After tirgger ZSP, the state will kept until mcu2zsp_icr set to 1, in other words, the state 
    // must be clear manually 

    while(1)
    {
         hal_TimDelay(50 MS_WAITING);

        if (g_zspCfg.zspStatus == 0x1)
        {
            hal_HstSendEvent(BOOT_EVENT,  0xaaaa);
            break;
        }       
    };

#if 1   

    //HAL_AIF_STREAM_T   aif_cfg =  {(UINT32*)(&zsp_TestBuff1), 64<<1, HAL_AIF_FREQ_8000HZ, HAL_AIF_MONO, 0, 0,  NULL, NULL };

    hal_HstSendEvent(BOOT_EVENT, 0x63636370);

    hal_HstSendEvent(BOOT_EVENT, (&zsp_TestBuff1));

    hal_HstSendEvent(BOOT_EVENT, (&zsp_TestBuff2));

    hal_HstSendEvent(BOOT_EVENT, (&g_zspCfg.zspInputBuff));
    
    hal_HstSendEvent(BOOT_EVENT, (&g_zspCfg.zspOutputBuff));

    hal_SysSetMclkDiv(2);

    hal_SysSetBclkDiv(17);
    
    hal_SysSetTxStbDiv(17*16);

    hal_SysSetPerClkForceon(SYS_CTRL_AUDIFC_CLK_FORCE_ON);

    
    hwp_aif->ctrl = AIF_LOOP_BACK; 
    //hwp_aif->ctrl |=AIF_PARALLEL_IN_SET; 
    //hwp_aif->ctrl |= AIF_PARALLEL_OUT_SET; 
    //hwp_aif->ctrl |= AIF_TX_STB_MODE; 
    
    hwp_aif->serial_ctrl  = AIF_MASTER_MODE|AIF_RX_DLY(1)|AIF_LRCK_POL;  
    hwp_aif->ctrl |= AIF_ENABLE; 

    hwp_bbIfc->ch[1].start_addr = ((UINT32)& zsp_TestBuff1);
    hwp_bbIfc->ch[1].fifo_size  = 480*2;
    hwp_bbIfc->ch[0].start_addr = ((UINT32)& g_zspCfg.zspInputBuff);
    hwp_bbIfc->ch[0].fifo_size  = 480*2;

    hwp_bbIfc->ch[0].int_mask = BB_IFC_END_FIFO; 
    hwp_bbIfc->ch[1].int_mask = BB_IFC_END_FIFO; 

    hwp_bbIfc->ch[1].control  = BB_IFC_AUTO_DISABLE|BB_IFC_ENABLE;
    hwp_bbIfc->ch[0].control  = BB_IFC_AUTO_DISABLE|BB_IFC_ENABLE;


    #endif

    hal_HstSendEvent(BOOT_EVENT, 0x2012);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspMode);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspStatus);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspUpdatePara);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zsp2mcuMb);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.mcu2zspMb);  
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspInputBuff);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.zspOutputBuff);
    hal_HstSendEvent(BOOT_EVENT, &g_zspCfg.reserved);

        
    while(1)
    {
         hal_TimDelay(50 MS_WAITING);
         /*if (g_zspCfg.zspUpdatePara == 0)
         {       
            hal_HstSendEvent(BOOT_EVENT, 0xcccc);
            g_zspCfg.zspUpdatePara = 1;
         }*/
         
        if (hwp_ahb_mailbox->zsp2mcu_isr == 0x1)
        {
            hal_HstSendEvent(BOOT_EVENT,  0xbbbb);          
            hwp_ahb_mailbox->zsp2mcu_icr = hwp_ahb_mailbox->zsp2mcu_isr; 
            
            //break;
        }       
      
    };
    
    
}

PUBLIC BOOL hal_ZspIsClose(VOID)
{
    return (g_halZspState == HAL_ZSP_CLOSE_DONE);
}
