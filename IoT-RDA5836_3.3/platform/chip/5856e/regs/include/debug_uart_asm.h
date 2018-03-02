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
//------------------------------------------------------------------------
// File name      : debug_uart_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:47                                                  
// Last modified  : 2016-08-29 14:59:47                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:47 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DEBUG_UART_ASM_H__
#define __DEBUG_UART_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ctrl                       REG_DEBUG_UART_BASE+0x00
#define REG_status                     REG_DEBUG_UART_BASE+0x04
#define REG_rxtx_buffer                REG_DEBUG_UART_BASE+0x08
#define REG_irq_mask                   REG_DEBUG_UART_BASE+0x0C
#define REG_irq_cause                  REG_DEBUG_UART_BASE+0x10
#define REG_triggers                   REG_DEBUG_UART_BASE+0x14
#define REG_xchar                      REG_DEBUG_UART_BASE+0x18

//ctrl
#define DEBUG_UART_ENABLE              (1<<0)              
#define DEBUG_UART_DATA_BITS           (1<<1)              
#define DEBUG_UART_TX_STOP_BITS        (1<<2)              
#define DEBUG_UART_PARITY_ENABLE       (1<<3)              
#define DEBUG_UART_PARITY_SELECT(n)    (((n)&0x3)<<4)      
#define DEBUG_UART_TX_BREAK_CONTROL    (1<<6)              
#define DEBUG_UART_RX_FIFO_RESET       (1<<7)              
#define DEBUG_UART_TX_FIFO_RESET       (1<<8)              
#define DEBUG_UART_DMA_MODE            (1<<9)              
#define DEBUG_UART_SWRX_FLOW_CTRL(n)   (((n)&0x3)<<12)     
#define DEBUG_UART_SWTX_FLOW_CTRL(n)   (((n)&0x3)<<14)     
#define DEBUG_UART_BACKSLASH_EN        (1<<16)             
#define DEBUG_UART_SEND_XON            (1<<17)             
#define DEBUG_UART_TX_FINISH_N_WAIT    (1<<19)             
#define DEBUG_UART_DIVISOR_MODE        (1<<20)             
#define DEBUG_UART_IRDA_ENABLE         (1<<21)             
#define DEBUG_UART_RX_RTS              (1<<22)             
#define DEBUG_UART_AUTO_FLOW_CONTROL   (1<<23)             
#define DEBUG_UART_LOOP_BACK_MODE      (1<<24)             
#define DEBUG_UART_RX_LOCK_ERR         (1<<25)             
#define DEBUG_UART_HST_TXD_OEN         (1<<26)             
#define DEBUG_UART_RX_BREAK_LENGTH(n)  (((n)&0xF)<<28)     

//status
#define DEBUG_UART_RX_FIFO_LEVEL(n)    (((n)&0x1F)<<0)     
#define DEBUG_UART_TX_FIFO_LEVEL(n)    (((n)&0x1F)<<8)     
#define DEBUG_UART_TX_ACTIVE           (1<<13)             
#define DEBUG_UART_RX_ACTIVE           (1<<14)             
#define DEBUG_UART_RX_OVERFLOW_ERR     (1<<16)             
#define DEBUG_UART_TX_OVERFLOW_ERR     (1<<17)             
#define DEBUG_UART_RX_PARITY_ERR       (1<<18)             
#define DEBUG_UART_RX_FRAMING_ERR      (1<<19)             
#define DEBUG_UART_RX_BREAK_INT        (1<<20)             
#define DEBUG_UART_TX_DCTS             (1<<24)             
#define DEBUG_UART_TX_CTS              (1<<25)             
#define DEBUG_UART_TX_FIFO_RSTED_L     (1<<28)             
#define DEBUG_UART_RX_FIFO_RSTED_L     (1<<29)             
#define DEBUG_UART_ENABLE_N_FINISHED   (1<<30)             
#define DEBUG_UART_CLK_ENABLED         (1<<31)             

//rxtx_buffer
#define DEBUG_UART_DATA_BUFFER(n)      (((n)&0xFF)<<0)     

//irq_mask
#define DEBUG_UART_TX_MODEM_STATUS     (1<<0)              
#define DEBUG_UART_RX_DATA_AVAILABLE   (1<<1)              
#define DEBUG_UART_TX_DATA_NEEDED      (1<<2)              
#define DEBUG_UART_RX_TIMEOUT          (1<<3)              
#define DEBUG_UART_RX_LINE_ERR         (1<<4)              
#define DEBUG_UART_TX_DMA_DONE         (1<<5)              
#define DEBUG_UART_RX_DMA_DONE         (1<<6)              
#define DEBUG_UART_RX_DMA_TIMEOUT      (1<<7)              
#define DEBUG_UART_XOFF_DETECTED       (1<<8)              

//irq_cause
#define DEBUG_UART_TX_MODEM_STATUS     (1<<0)              
#define DEBUG_UART_RX_DATA_AVAILABLE   (1<<1)              
#define DEBUG_UART_TX_DATA_NEEDED      (1<<2)              
#define DEBUG_UART_RX_TIMEOUT          (1<<3)              
#define DEBUG_UART_RX_LINE_ERR         (1<<4)              
#define DEBUG_UART_TX_DMA_DONE         (1<<5)              
#define DEBUG_UART_RX_DMA_DONE         (1<<6)              
#define DEBUG_UART_RX_DMA_TIMEOUT      (1<<7)              
#define DEBUG_UART_XOFF_DETECTED       (1<<8)              
#define DEBUG_UART_TX_MODEM_STATUS_U   (1<<16)             
#define DEBUG_UART_RX_DATA_AVAILABLE_U (1<<17)             
#define DEBUG_UART_TX_DATA_NEEDED_U    (1<<18)             
#define DEBUG_UART_RX_TIMEOUT_U        (1<<19)             
#define DEBUG_UART_RX_LINE_ERR_U       (1<<20)             
#define DEBUG_UART_TX_DMA_DONE_U       (1<<21)             
#define DEBUG_UART_RX_DMA_DONE_U       (1<<22)             
#define DEBUG_UART_RX_DMA_TIMEOUT_U    (1<<23)             
#define DEBUG_UART_XOFF_DETECTED_U     (1<<24)             

//triggers
#define DEBUG_UART_RX_TRIGGER(n)       (((n)&0xF)<<0)      
#define DEBUG_UART_TX_TRIGGER(n)       (((n)&0xF)<<4)      
#define DEBUG_UART_AFC_LEVEL(n)        (((n)&0xF)<<8)      

//xchar
#define DEBUG_UART_XON1(n)             (((n)&0xFF)<<0)     
#define DEBUG_UART_XOFF1(n)            (((n)&0xFF)<<8)     
#define DEBUG_UART_XON2(n)             (((n)&0xFF)<<16)    
#define DEBUG_UART_XOFF2(n)            (((n)&0xFF)<<24)    


#endif


