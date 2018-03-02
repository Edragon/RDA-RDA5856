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
/// @file hal_zsp.h
/// This file describes the API of the HAL VOC  Driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////





#ifndef _HAL_ZSP_H_
#define _HAL_ZSP_H_

#include "cs_types.h"
#include "hal_error.h"



// ============================================================================
// Types
// ============================================================================

// ============================================================================
// HAL_ZSP_WAKEUP_ID_T
// ----------------------------------------------------------------------------
/// Defines the way that the VoC module should be woken up.
// ============================================================================
typedef enum  
{
    /// generate a wakeup event with ID=0
    HAL_ZSP_WAKEUP_EVENT_0 = 0, 
    /// generate a wakeup event with ID=1
    HAL_ZSP_WAKEUP_EVENT_1 = 1,
    /// start the VoC module (has effect only if the module is stalled)
    HAL_ZSP_START  = 2 
            
} HAL_ZSP_WAKEUP_ID_T;

// ============================================================================
// HAL_ZSP_DMA_DIR_T
// ----------------------------------------------------------------------------
/// Defines the direction (read or write) of the DMA transfer
// ============================================================================
typedef enum  
{
    /// read VoC DMA read transfer
    HAL_ZSP_DMA_READ = 0, 
    /// read VoC DMA write transfer
    HAL_ZSP_DMA_WRITE = 1
            
} HAL_ZSP_DMA_DIR_T;

// ============================================================================
// HAL_ZSP_DMA_TRANSFER_T
// ----------------------------------------------------------------------------
/// Defines the type (single or burst) of the DMA transfer
// ============================================================================
typedef enum  
{
    /// single VoC DMA write transfer
    HAL_ZSP_DMA_BURST = 0,
    /// burst VoC DMA read transfer
    HAL_ZSP_DMA_SINGLE = 1
            
} HAL_ZSP_DMA_TRANSFER_T;

// ============================================================================
// HAL_ZSP_DMA_B2S_T
// ----------------------------------------------------------------------------
/// Defines the byte-to-short mode of the DMA transfer (ZSP2.1 and later)
// ============================================================================
typedef enum  
{
    /// b2s disabled (normal burst)
    HAL_ZSP_DMA_B2S_NO = 0,
    /// trace mode (b2s disabled, no stall of the voc core with single transfers)
    HAL_ZSP_DMA_B2S_TRACE = 1,
    /// unsigned b2s
    HAL_ZSP_DMA_B2S_UNSIGNED = 2,
    /// signed b2s
    HAL_ZSP_DMA_B2S_SIGNED = 3
            
} HAL_ZSP_DMA_B2S_T;

// ============================================================================
// HAL_ZSP_WAKEUP_EVENT_T
// ----------------------------------------------------------------------------
/// This structure is used to represent the wake uo events status and mask.
/// These events are recorded in the VoC wakeup event handler and wakeup
/// the VoC module if the corresponding bits in the Event Mask are enabled.
// ============================================================================
typedef struct
{
    /// IFC0 event when the IFC0 reaches a half or full buffer condition
    UINT32 wakeupIfc0:1;
    /// IFC1 event when the IFC1 reaches a half or full buffer condition
    UINT32 wakeupIfc1:1;
    /// DMAE event when the ZsP DMA transfer configured by the XCPU is finished.
    UINT32 wakeupDmae:1;
    /// DMAI event when the ZsP DMA transfer configured by the ZsP module is finished.
    UINT32 wakeupDmai:1;
    /// SOF0 event (generated with function hal_VocWakeupSoftEvent)
    UINT32 wakeupSof0:1;
    /// SOF1 event (generated with function hal_VocWakeupSoftEvent)
    UINT32 wakeupSof1:1; 
    
} HAL_ZSP_WAKEUP_EVENT_T;

// ============================================================================
// HAL_ZSP_IRQ_STATUS_T
// ----------------------------------------------------------------------------
/// This structure is used to represent the irq status and mask
// ============================================================================
typedef struct 
{
    /// IRQ from voc to the XCPU
    UINT32 zsp:1;
    /// unused field
    UINT32 unused1:1; 
    /// IRQ upon finish of a DMA transfert to/from ZSP configured by the ZsP
    UINT32 dmaZsp:1;
    /// unused field
    UINT32 unused2:1; 
    
} HAL_ZSP_IRQ_STATUS_T;

// ============================================================================
// HAL_ZSP_IRQ_HANDLER_T
// ----------------------------------------------------------------------------
/// User handler function type
// ============================================================================
typedef void (*HAL_ZSP_IRQ_HANDLER_T)(HAL_ZSP_IRQ_STATUS_T*);

#define ZSP_DTCM_SHARE_MEM __attribute__((section (".zsp_dtcm_share_mem")))

#define ZSP_TRACE_BUFF_LEN  512
#define ZSP_INPUT_BUFF_LEN  0x1000
#define ZSP_OUTPUT_BUFF_LEN 0x1000

// ============================================================================
// HAL_ZSP_CFG_T
// ----------------------------------------------------------------------------
/// General configuration of the ZsP module
// ============================================================================
typedef struct 
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
    UINT32    zspTrace[ZSP_TRACE_BUFF_LEN/4];
    UINT16    zspMode; 
    UINT16    zspStatus; 
    UINT16    zspUpdatePara; 
	UINT16    reserved1;
    UINT32    zsp2mcuMb; 
    UINT32    mcu2zspMb; 
    INT16     zspInputBuff[ZSP_INPUT_BUFF_LEN/2]; 
    INT16     zspOutputBuff[ZSP_OUTPUT_BUFF_LEN/2];
    UINT16     reserved[16];
#else
    /// Pointer to the location of the ZsP code which will be executed
    CONST INT32 * zspCode;
    /// Size in bytes of the ZsP code
    UINT32 zspCodeSize;
    /// Value of the ZsP program counter, on which to start the ZsP program execution        
    UINT16 pcVal;
    /// Start of the ZsP code critical section (inclusive)        
    UINT16 pcValCriticalSecMin;
    /// End of the ZsP code critical section (exclusive)
    UINT16 pcValCriticalSecMax;
    /// Select if an IRQ should be generated at the end of the open procedure 
    BOOL needOpenDoneIrq;
    /// Set ZsP wakeup event mask
    HAL_ZSP_WAKEUP_EVENT_T eventMask;
    /// Set ZsP interrupt mask
    HAL_ZSP_IRQ_STATUS_T irqMask;
    /// Set ZsP user defined interrupt handler function
    HAL_ZSP_IRQ_HANDLER_T zspIrqHandler;
#endif
    
} HAL_ZSP_CFG_T;
                        
// ============================================================================
// HAL_ZSP_DMA_CFG_T
// ----------------------------------------------------------------------------
/// Configuration of a DMA transfer between a buffer in a ZsP RAM and a buffer in
/// a external memory (any memory space mapped on AHB).
// ============================================================================
typedef struct 
{
    /// External (AHB) byte address
    INT32* extAddr;
    /// Local (ZsP RAM) byte address.
    INT32* zspLocalAddr;
    /// Size of the transfer in bytes.
    INT32  size;
    /// 0 : read (extAddr is the source),
    /// 1 : write (extAddr is the destination).
    HAL_ZSP_DMA_DIR_T  wr1Rd0;
    /// Select if an IRQ needs to be generated at the end of the transfer.
    BOOL   needIrq;

} HAL_ZSP_DMA_CFG_T;


// ============================================================================
// 
// ----------------------------------------------------------------------------
/// 
// ============================================================================
typedef enum
{
    HAL_ZSP_MB_ZSP2MCU, 
    HAL_ZSP_MB_MCU2ZSP,
    HAL_ZSP_MB_INVALID
}HAL_ZSP_MB_DIR_T;
// ============================================================================
// 
// ----------------------------------------------------------------------------
/// 
// ============================================================================
typedef enum  
{
    HAL_ZSP_MB_ID_OPEN = 0x01, 
    HAL_ZSP_MB_ID_CONTINUE = 0x02,
    HAL_ZSP_MB_ID_CLOSE = 0x04
}HAL_MCU2ZSP_MB_ID_T; 

typedef enum  
{
    HAL_ZSP_MB_ID_DATA_IND = 0x01, 
    HAL_ZSP_MB_ID_TRACE_IND = 0x02
}HAL_ZSP2MCU_MB_ID_T; 
// ============================================================================
// FUNCTIONS
// ============================================================================

// ============================================================================
// hal_ZspOpen
// ----------------------------------------------------------------------------
// ============================================================================
PUBLIC HAL_ERR_T hal_ZspOpen (CONST HAL_ZSP_CFG_T * pCfg);

// ============================================================================
// hal_ZspClose
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC VOID hal_ZspClose(VOID);

// ============================================================================
// hal_ZspWake
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC HAL_ERR_T hal_ZspWakeup(HAL_ZSP_WAKEUP_ID_T wakeupId);

// ============================================================================
// hal_ZspGetPointer
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC VOID* hal_ZspGetPointer(INT32 zspLocalAddr);

// ============================================================================
// hal_ZspGetDmaiPointer
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC INT32 * hal_ZspGetDmaiPointer(INT32 * zspExternAddr, HAL_ZSP_DMA_DIR_T wr1Rd0, HAL_ZSP_DMA_TRANSFER_T sngl1brst0, HAL_ZSP_DMA_B2S_T b2s);

// ============================================================================
// hal_ZspDmaStart
// ----------------------------------------------------------------------------
////
// ============================================================================
PUBLIC HAL_ERR_T hal_ZspDmaStart (CONST HAL_ZSP_DMA_CFG_T * pCfg);

// ============================================================================
// hal_ZspDmaDone
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC BOOL hal_ZspDmaDone(VOID);
// ============================================================================
// hal_ZspDmaDone
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC VOID hal_ZspMbSend(HAL_ZSP_MB_DIR_T direction,  HAL_MCU2ZSP_MB_ID_T id);

// ============================================================================
// hal_ZspDmaDone
// ----------------------------------------------------------------------------
///
// ============================================================================
PUBLIC VOID hal_ZspTest(VOID);
///
/// @} // <-- End of the zsp group
///

#endif

