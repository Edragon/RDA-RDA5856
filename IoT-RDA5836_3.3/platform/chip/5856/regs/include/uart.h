/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef _UART_H_
#define _UART_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'uart'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define UART_RX_FIFO_SIZE                        (32)
#define UART_TX_FIFO_SIZE                        (16)
#define NB_RX_FIFO_BITS                          (5)
#define NB_TX_FIFO_BITS                          (4)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// UART_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_UART_BASE               0x01A15000
#define REG_UART2_BASE              0x01A16000

typedef volatile struct
{
    REG32                          ctrl;                         //0x00000000
    REG32                          status;                       //0x00000004
    REG32                          rxtx_buffer;                  //0x00000008
    REG32                          irq_mask;                     //0x0000000C
    REG32                          irq_cause;                    //0x00000010
    REG32                          triggers;                     //0x00000014
    REG32                          cmd_set;                      //0x00000018
    REG32                          cmd_clr;                      //0x0000001C
    REG32                          AUTO_BAUD;                    //0x00000020    
} HWP_UART_T;

#define hwp_uart                    ((HWP_UART_T*) KSEG1(REG_UART_BASE))
#define hwp_uart2                   ((HWP_UART_T*) KSEG1(REG_UART2_BASE))


//ctrl
#define UART_ENABLE                 (1<<0)
#define UART_ENABLE_DISABLE         (0<<0)
#define UART_ENABLE_ENABLE          (1<<0)
#define UART_DATA_BITS              (1<<1)
#define UART_DATA_BITS_7_BITS       (0<<1)
#define UART_DATA_BITS_8_BITS       (1<<1)
#define UART_TX_STOP_BITS           (1<<2)
#define UART_TX_STOP_BITS_1_BIT     (0<<2)
#define UART_TX_STOP_BITS_2_BITS    (1<<2)
#define UART_PARITY_ENABLE          (1<<3)
#define UART_PARITY_ENABLE_NO       (0<<3)
#define UART_PARITY_ENABLE_YES      (1<<3)
#define UART_PARITY_SELECT(n)       (((n)&3)<<4)
#define UART_PARITY_SELECT_ODD      (0<<4)
#define UART_PARITY_SELECT_EVEN     (1<<4)
#define UART_PARITY_SELECT_SPACE    (2<<4)
#define UART_PARITY_SELECT_MARK     (3<<4)
#define UART_DIVISOR_MODE           (1<<20)
#define UART_IRDA_ENABLE            (1<<21)
#define UART_DMA_MODE               (1<<22)
#define UART_DMA_MODE_DISABLE       (0<<22)
#define UART_DMA_MODE_ENABLE        (1<<22)
#define UART_AUTO_FLOW_CONTROL      (1<<23)
#define UART_AUTO_FLOW_CONTROL_ENABLE (1<<23)
#define UART_AUTO_FLOW_CONTROL_DISABLE (0<<23)
#define UART_LOOP_BACK_MODE         (1<<24)
#define UART_RX_LOCK_ERR            (1<<25)
#define UART_RX_BREAK_LENGTH(n)     (((n)&15)<<28)

//status
#define UART_RX_FIFO_LEVEL(n)       (((n)&0x3F)<<0)
#define UART_RX_FIFO_LEVEL_MASK     (0x3F<<0)
#define UART_RX_FIFO_LEVEL_SHIFT    (0)
#define UART_TX_FIFO_SPACE(n)       (((n)&31)<<8)
#define UART_TX_FIFO_SPACE_MASK     (31<<8)
#define UART_TX_FIFO_SPACE_SHIFT    (8)
#define UART_TX_ACTIVE              (1<<14)
#define UART_RX_ACTIVE              (1<<15)
#define UART_RX_OVERFLOW_ERR        (1<<16)
#define UART_TX_OVERFLOW_ERR        (1<<17)
#define UART_RX_PARITY_ERR          (1<<18)
#define UART_RX_FRAMING_ERR         (1<<19)
#define UART_RX_BREAK_INT           (1<<20)
#define UART_DCTS                   (1<<24)
#define UART_CTS                    (1<<25)
#define UART_DTR                    (1<<28)
#define UART_CLK_ENABLED            (1<<31)

//rxtx_buffer
#define UART_RX_DATA(n)             (((n)&0xFF)<<0)
#define UART_TX_DATA(n)             (((n)&0xFF)<<0)

//irq_mask
#define UART_TX_MODEM_STATUS        (1<<0)
#define UART_RX_DATA_AVAILABLE      (1<<1)
#define UART_TX_DATA_NEEDED         (1<<2)
#define UART_RX_TIMEOUT             (1<<3)
#define UART_RX_LINE_ERR            (1<<4)
#define UART_TX_DMA_DONE            (1<<5)
#define UART_RX_DMA_DONE            (1<<6)
#define UART_RX_DMA_TIMEOUT         (1<<7)
#define UART_DTR_RISE               (1<<8)
#define UART_DTR_FALL               (1<<9)

//irq_cause
//#define UART_TX_MODEM_STATUS      (1<<0)
//#define UART_RX_DATA_AVAILABLE    (1<<1)
//#define UART_TX_DATA_NEEDED       (1<<2)
//#define UART_RX_TIMEOUT           (1<<3)
//#define UART_RX_LINE_ERR          (1<<4)
//#define UART_TX_DMA_DONE          (1<<5)
//#define UART_RX_DMA_DONE          (1<<6)
//#define UART_RX_DMA_TIMEOUT       (1<<7)
//#define UART_DTR_RISE             (1<<8)
//#define UART_DTR_FALL             (1<<9)
#define UART_TX_MODEM_STATUS_U      (1<<16)
#define UART_RX_DATA_AVAILABLE_U    (1<<17)
#define UART_TX_DATA_NEEDED_U       (1<<18)
#define UART_RX_TIMEOUT_U           (1<<19)
#define UART_RX_LINE_ERR_U          (1<<20)
#define UART_TX_DMA_DONE_U          (1<<21)
#define UART_RX_DMA_DONE_U          (1<<22)
#define UART_RX_DMA_TIMEOUT_U       (1<<23)
#define UART_DTR_RISE_U             (1<<24)
#define UART_DTR_FALL_U             (1<<25)

//triggers
#define UART_RX_TRIGGER(n)          (((n)&31)<<0)
#define UART_TX_TRIGGER(n)          (((n)&15)<<8)
#define UART_AFC_LEVEL(n)           (((n)&31)<<16)

//CMD_Set
#define UART_RI                     (1<<0)
#define UART_DCD                    (1<<1)
#define UART_DSR                    (1<<2)
#define UART_TX_BREAK_CONTROL       (1<<3)
#define UART_TX_FINISH_N_WAIT       (1<<4)
#define UART_RX_RTS                 (1<<5)
#define UART_RX_FIFO_RESET          (1<<6)
#define UART_TX_FIFO_RESET          (1<<7)

//UART_CMD_CLR
#define UART_RI                        (1<<0)              
#define UART_DCR                       (1<<1)              
#define UART_DSR                       (1<<2)              
#define UART_TX_BREAK_CONTROL          (1<<3)              
#define UART_TX_FINISH_N_WAIT          (1<<4)              
#define UART_RX_CPU_RTS                (1<<5)              

//UART_AUTO_BAUD
#define UART_AUTO_ENABLE               (1<<0)              
#define UART_AUTO_TRACKING             (1<<1)              
#define UART_VERIFY_2BYTE              (1<<2)              
#define UART_VERIFY_CHAR0(n)           (((n)&0xFF)<<8)     
#define UART_VERIFY_CHAR1(n)           (((n)&0xFF)<<16)    


#endif

