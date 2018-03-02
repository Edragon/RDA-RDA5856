//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : uart                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 15:00:02                                                  
// Last modified  : 2016-08-29 15:00:02                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 15:00:02 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __UART_H__
#define __UART_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#include "register.h"

#define hwp_uart2   hwp_uart
#define HWP_UART_T     T_HWP_UART_T

// =============================================================================
//  MACROS
// =============================================================================
#define UART_RX_FIFO_SIZE                        (32)
#define UART_TX_FIFO_SIZE                        (16)
#define NB_RX_FIFO_BITS                          (5)
#define NB_TX_FIFO_BITS                          (4)


#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Allows to turn off the UART:0: Disable1: Enable*/
        U32 data_bits                      :1 ; /*1 :1 , Number of data bits per character (least significant bit first):0: 7_bits1: 8_bits*/
        U32 tx_stop_bits                   :1 ; /*2 :2 , Stop bits controls the number of stop bits transmitted. Can receive with one stop bit (more inaccuracy can be compensated with two stop bits when divisor mode is set to 0).0: 1_bit one stop bit is transmitted in the serial data.1: 2_bits two stop bits are generated and transmitted in the serial data out.*/
        U32 parity_enable                  :1 ; /*3 :3 , Parity is enabled when this bit is set.0: NO1: YES*/
        U32 parity_select                  :2 ; /*5 :4 , Controls the parity format when parity is enabled:0: odd an odd number of received 1 bits is checked, or transmitted (the parity bit is included).1: even an even number of received 1 bits is checked or transmitted (the parity bit is included).2: space a space is generated and received as parity bit.3: mark a mark is generated and received as parity bit.*/
        U32 ctrl_reserved_1                :14; /*19:6 , No description.               */
        U32 divisor_mode                   :1 ; /*20:20, Selects the divisor value used to generate the baud rate frequency (BCLK) from the SCLK (see UART Operation for details). If IrDA is enable, this bit is ignored and the divisor used will be 16.0 = (BCLK = SCLK / 4)1 = (BCLK = SCLK / 16)*/
        U32 irda_enable                    :1 ; /*21:21, When set, the UART is in IrDA mode and the baud rate divisor used is 16 (see UART Operation for details).*/
        U32 dma_mode                       :1 ; /*22:22, Enables the DMA signaling for the Uart_Dma_Tx_Req_H and Uart_Dma_Rx_Req_H to the IFC.0: DISABLE1: ENABLE*/
        U32 auto_flow_control              :1 ; /*23:23, Enables the auto flow control. Uart_RTS is controlled by the Rx RTS bit and the UART Auto Control Flow System. If Uart_CTS become inactive high, the Tx data flow is stopped.1: ENABLE0: DISABLE*/
        U32 loop_back_mode                 :1 ; /*24:24, When set, data on the Uart_Tx line is held high, while the serial output is looped back to the serial input line, internally. In this mode all the interrupts are fully functional. This feature is used for diagnostic purposes. Also, in loop back mode, the modem control input Uart_CTS is disconnected and the modem control output Uart_RTS are looped back to the inputs, internally. In IrDA mode, Uart_Tx signal is inverted (see IrDA SIR Mode Support).*/
        U32 rx_lock_err                    :1 ; /*25:25, Allow to stop the data receiving when an error is detected (framing, parity or break). The data in the fifo are kept.*/
        U32 ctrl_reserved_0                :2 ; /*27:26, No description.               */
        U32 rx_break_length                :4 ; /*31:28, Length of a break, in number of bits.*/
    } b;
} T_UART_ctrl;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 rx_fifo_level                  :7 ; /*6 :0 , Those bits indicate the number of data available in the Rx Fifo. Those data can be read.*/
        U32 status_reserved_3              :1 ; /*7 :7 , No description.               */
        U32 tx_fifo_space                  :5 ; /*12:8 , Those bits indicate the number of space available in the Tx Fifo.*/
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 tx_active                      :1 ; /*14:14, This bit indicates that the UART is sending data. If no data is in the fifo, the UART is currently sending the last one through the serial interface.*/
        U32 rx_active                      :1 ; /*15:15, This bit indicates that the UART is receiving a byte.*/
        U32 rx_overflow_err                :1 ; /*16:16, This bit indicates that the receiver received a new character when the fifo was already full. The new character is discarded. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 tx_overflow_err                :1 ; /*17:17, This bit indicates that the user tried to write a character when fifo was already full. The written data will not be kept. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_parity_err                  :1 ; /*18:18, This bit is set if the parity is enabled and a parity error occurred in the received data. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_framing_err                 :1 ; /*19:19, This bit is set whenever there is a framing error occured. A framing error occurs when the receiver does not detect a valid STOP bit in the received data. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_break_int                   :1 ; /*20:20, This bit is set whenever the serial input is held in a logic 0 state for longer than the length of x bits, where x is the value programmed Rx Break Length. A null word will be written in the Rx Fifo. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 status_reserved_2              :3 ; /*23:21, No description.               */
        U32 dcts                           :1 ; /*24:24, This bit is set when the Uart_CTS line changed since the last time this register has been written. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 cts                            :1 ; /*25:25, current value of the Uart_CTS line. '1' = Tx not allowed. '0' = Tx allowed.*/
        U32 status_reserved_1              :2 ; /*27:26, No description.               */
        U32 dtr                            :1 ; /*28:28, Current value of the DTR line.*/
        U32 status_reserved_0              :2 ; /*30:29, No description.               */
        U32 clk_enabled                    :1 ; /*31:31, This bit is set when Uart Clk has been enabled and received by UART after Need Uart Clock becomes active. It serves to avoid enabling RTS too early.*/
    } b;
} T_UART_status;

//rxtx_buffer
typedef union
{
    U32 v;
    struct
    {
        U32 data                           :8 ; /*7 :0 , The UART_TRANSMIT_HOLDING register is a write-only register that contains data to be transmitted on the serial output port. 16 characters of data may be written to the UART_TRANSMIT_HOLDING register before the FIFO is full. Any attempt to write data when the FIFO is full results in the write data being lost.*/
        U32 reserved_0                     :1 ; /*8 :8 ,                               */
        U32 reserved_1                     :1 ; /*9 :9 ,                               */
        U32 reserved_2                     :1 ; /*10:10,                               */
        U32 reserved_3                     :1 ; /*11:11,                               */
        U32 reserved_4                     :1 ; /*12:12,                               */
        U32 reserved_5                     :1 ; /*13:13,                               */
        U32 reserved_6                     :1 ; /*14:14,                               */
        U32 reserved_7                     :1 ; /*15:15,                               */
        U32 reserved_8                     :1 ; /*16:16,                               */
        U32 reserved_9                     :1 ; /*17:17,                               */
        U32 reserved_10                    :1 ; /*18:18,                               */
        U32 reserved_11                    :1 ; /*19:19,                               */
        U32 reserved_12                    :1 ; /*20:20,                               */
        U32 reserved_13                    :1 ; /*21:21,                               */
        U32 reserved_14                    :1 ; /*22:22,                               */
        U32 reserved_15                    :1 ; /*23:23,                               */
        U32 reserved_16                    :1 ; /*24:24,                               */
        U32 reserved_17                    :1 ; /*25:25,                               */
        U32 reserved_18                    :1 ; /*26:26,                               */
        U32 reserved_19                    :1 ; /*27:27,                               */
        U32 reserved_20                    :1 ; /*28:28,                               */
        U32 reserved_21                    :1 ; /*29:29,                               */
        U32 reserved_22                    :1 ; /*30:30,                               */
        U32 reserved_23                    :1 ; /*31:31,                               */
    } b;
} T_UART_rxtx_buffer;

//irq_mask
typedef union
{
    U32 v;
    struct
    {
        U32 tx_modem_status                :1 ; /*0 :0 , Clear to send signal change detected.*/
        U32 rx_data_available              :1 ; /*1 :1 , Rx Fifo at or upper threshold level (current level &gt;= Rx Fifo trigger level).*/
        U32 tx_data_needed                 :1 ; /*2 :2 , Tx Fifo at or below threshold level (current level &lt;= Tx Fifo trigger level).*/
        U32 rx_timeout                     :1 ; /*3 :3 , No characters in or out of the Rx Fifo during the last 4 character times and there is at least 1 character in it during this time.*/
        U32 rx_line_err                    :1 ; /*4 :4 , Tx Overflow, Rx Overflow, Parity Error, Framing Error or Break Interrupt.*/
        U32 tx_dma_done                    :1 ; /*5 :5 , Pulse detected on Uart_Dma_Tx_Done_H signal.*/
        U32 rx_dma_done                    :1 ; /*6 :6 , Pulse detected on Uart_Dma_Rx_Done_H signal*/
        U32 rx_dma_timeout                 :1 ; /*7 :7 , In DMA mode, there is at least 1 character that has been read in or out the Rx Fifo. Then before received Rx DMA Done, No characters in or out of the Rx Fifo during the last 4 character times.*/
        U32 dtr_rise                       :1 ; /*8 :8 , Rising edge detected on the UART_DTR signal.*/
        U32 dtr_fall                       :1 ; /*9 :9 , Falling edge detected on the UART_DTR signal.*/
        U32 irq_mask_reserved_0            :22; /*31:10, No description.               */
    } b;
} T_UART_irq_mask;

//irq_cause
typedef union
{
    U32 v;
    struct
    {
        U32 tx_modem_status                :1 ; /*0 :0 , Clear to send signal detected. Reset control: This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_data_available              :1 ; /*1 :1 , Rx Fifo at or upper threshold level (current level &gt;= Rx Fifo trigger level). Reset control: Reading the UART_RECEIVE_BUFFER until the Fifo drops below the trigger level.*/
        U32 tx_data_needed                 :1 ; /*2 :2 , Tx Fifo at or below threshold level (current level &lt;= Tx Fifo trigger level). Reset control: Writing into UART_TRANSMIT_HOLDING register above threshold level.*/
        U32 rx_timeout                     :1 ; /*3 :3 , No characters in or out of the Rx Fifo during the last 4 character times and there is at least 1 character in it during this time. Reset control: Reading from the UART_RECEIVE_BUFFER register.*/
        U32 rx_line_err                    :1 ; /*4 :4 , Tx Overflow, Rx Overflow, Parity Error, Framing Error or Break Interrupt. Reset control: This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 tx_dma_done                    :1 ; /*5 :5 , This interrupt is generated when a pulse is detected on the Uart_Dma_Tx_Done_H signal. Reset control: Write one in this register.*/
        U32 rx_dma_done                    :1 ; /*6 :6 , This interrupt is generated when a pulse is detected on the Uart_Dma_Rx_Done_H signal. Reset control: Write one in this register.*/
        U32 rx_dma_timeout                 :1 ; /*7 :7 , In DMA mode, there is at least 1 character that has been read in or out the Rx Fifo. Then before received Rx DMA Done, No characters in or out of the Rx Fifo during the last 4 character times.*/
        U32 dtr_rise                       :1 ; /*8 :8 , This interrupt is generated when a rising edge is detected on the UART_DTR signal. Reset control: Write one in this register.*/
        U32 dtr_fall                       :1 ; /*9 :9 , This interrupt is generated when a falling edge is detected on the UART_DTR signal. Reset control: Write one in this register.*/
        U32 irq_cause_reserved_1           :6 ; /*15:10, No description.               */
        U32 tx_modem_status_u              :1 ; /*16:16, Same as previous, not masked. */
        U32 rx_data_available_u            :1 ; /*17:17, Same as previous, not masked. */
        U32 tx_data_needed_u               :1 ; /*18:18, Same as previous, not masked. */
        U32 rx_timeout_u                   :1 ; /*19:19, Same as previous, not masked. */
        U32 rx_line_err_u                  :1 ; /*20:20, Same as previous, not masked. */
        U32 tx_dma_done_u                  :1 ; /*21:21, Same as previous, not masked. */
        U32 rx_dma_done_u                  :1 ; /*22:22, Same as previous, not masked. */
        U32 rx_dma_timeout_u               :1 ; /*23:23, Same as previous, not masked. */
        U32 dtr_rise_u                     :1 ; /*24:24, Same as previous, not masked. */
        U32 dtr_fall_u                     :1 ; /*25:25, Same as previous, not masked. */
        U32 irq_cause_reserved_0           :6 ; /*31:26, No description.               */
    } b;
} T_UART_irq_cause;

//triggers
typedef union
{
    U32 v;
    struct
    {
        U32 rx_trigger                     :6 ; /*5 :0 , Defines the empty threshold level at which the Data Available Interrupt will be generated. The Data Available interrupt is generated when quantity of data in Rx Fifo &gt; Rx Trigger.*/
        U32 triggers_reserved_2            :2 ; /*7 :6 , No description.               */
        U32 tx_trigger                     :4 ; /*11:8 , Defines the empty threshold level at which the Data Needed Interrupt will be generated.The Data Needed Interrupt is generated when quantity of data in Tx Fifo &lt;= Tx Trigger.*/
        U32 triggers_reserved_1            :4 ; /*15:12, No description.               */
        U32 afc_level                      :6 ; /*21:16, Controls the Rx Fifo level at which the Uart_RTS Auto Flow Control will be set inactive high (see UART Operation for more details on AFC).The Uart_RTS Auto Flow Control will be set inactive high when quantity of data in Rx Fifo &gt; AFC Level.*/
        U32 triggers_reserved_0            :10; /*31:22, No description.               */
    } b;
} T_UART_triggers;

//cmd_set
typedef union
{
    U32 v;
    struct
    {
        U32 ri                             :1 ; /*0 :0 , refer to bit [5]              */
        U32 dcd                            :1 ; /*1 :1 , refer to bit [5]              */
        U32 dsr                            :1 ; /*2 :2 , refer to bit [5]              */
        U32 tx_break_control               :1 ; /*3 :3 , refer to bit [5]              */
        U32 tx_finish_n_wait               :1 ; /*4 :4 , refer to bit [5]              */
        U32 rx_rts                         :1 ; /*5 :5 , this bit is set to 1 when writing 1, cleared to 0 when corresponding filed is cleared in UART_CMD_CLR*/
        U32 rx_fifo_reset                  :1 ; /*6 :6 , Writing a 1 to this bit resets and flushes the Receive Fifo. This bit does not need to be cleared.*/
        U32 tx_fifo_reset                  :1 ; /*7 :7 , Writing a 1 to this bit resets and flushes the Transmit Fifo. This bit does not need to be cleared.*/
        U32 cmd_set_reserved_0             :24; /*31:8 , No description.               */
    } b;
} T_UART_cmd_set;

//cmd_clr
typedef union
{
    U32 v;
    struct
    {
        U32 ri                             :1 ; /*0 :0 , refer to bit [5]              */
        U32 dcr                            :1 ; /*1 :1 , refer to bit [5]              */
        U32 dsr                            :1 ; /*2 :2 , refer to bit [5]              */
        U32 tx_break_control               :1 ; /*3 :3 , refer to bit [5]              */
        U32 tx_finish_n_wait               :1 ; /*4 :4 , refer to bit [5]              */
        U32 rx_cpu_rts                     :1 ; /*5 :5 , this bit is cleared to 0 when writing 1, set to 1 when corresponding filed is set in UART_CMD_SET*/
        U32 cmd_clr_reserved_0             :26; /*31:6 , No description.               */
    } b;
} T_UART_cmd_clr;

//auto_baud
typedef union
{
    U32 v;
    struct
    {
        U32 auto_enable                    :1 ; /*0 :0 , No description.               */
        U32 auto_tracking                  :1 ; /*1 :1 , No description.               */
        U32 verify_2byte                   :1 ; /*2 :2 , No description.               */
        U32 auto_baud_reserved_1           :5 ; /*7 :3 , No description.               */
        U32 verify_char0                   :8 ; /*15:8 , No description.               */
        U32 verify_char1                   :8 ; /*23:16, No description.               */
        U32 auto_baud_reserved_0           :8 ; /*31:24, No description.               */
    } b;
} T_UART_auto_baud;

typedef struct
{
    volatile                     T_UART_ctrl                           ctrl; /*0x0  , RW   , 0xF0000000, No description.               */
    volatile                   T_UART_status                         status; /*0x4  , RO   , 0x02000000, No description.               */
    volatile              T_UART_rxtx_buffer                    rxtx_buffer; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                 T_UART_irq_mask                       irq_mask; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                T_UART_irq_cause                      irq_cause; /*0x10 , RO   , 0x00000000, No description.               */
    volatile                 T_UART_triggers                       triggers; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                  T_UART_cmd_set                        cmd_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                  T_UART_cmd_clr                        cmd_clr; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                T_UART_auto_baud                      auto_baud; /*0x20 , RW   , 0x00544100, No description.               */
} T_HWP_UART_T;
#else
typedef struct
{
    volatile                             U32                           ctrl; /*0x0  , RW   , 0xF0000000, No description.               */
    volatile                             U32                         status; /*0x4  , RO   , 0x02000000, No description.               */
    volatile                             U32                    rxtx_buffer; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                       irq_mask; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                      irq_cause; /*0x10 , RO   , 0x00000000, No description.               */
    volatile                             U32                       triggers; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                             U32                        cmd_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                             U32                        cmd_clr; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                             U32                      auto_baud; /*0x20 , RW   , 0x00544100, No description.               */
} T_HWP_UART_T;

//ctrl
#define UART_ENABLE                    (1<<0)              
#define UART_ENABLE_DISABLE            (0<<0)              
#define UART_ENABLE_ENABLE             (1<<0)              

#define UART_DATA_BITS                 (1<<1)              
#define UART_DATA_BITS_7_BITS          (0<<1)              
#define UART_DATA_BITS_8_BITS          (1<<1)              

#define UART_TX_STOP_BITS              (1<<2)              
#define UART_TX_STOP_BITS_1_BIT        (0<<2)              
#define UART_TX_STOP_BITS_2_BITS       (1<<2)              

#define UART_PARITY_ENABLE             (1<<3)              
#define UART_PARITY_ENABLE_NO          (0<<3)              
#define UART_PARITY_ENABLE_YES         (1<<3)              

#define UART_PARITY_SELECT(n)          (((n)&0x3)<<4)      
#define UART_PARITY_SELECT_ODD         (((0)&0x3)<<4)      
#define UART_PARITY_SELECT_EVEN        (((1)&0x3)<<4)      
#define UART_PARITY_SELECT_SPACE       (((2)&0x3)<<4)      
#define UART_PARITY_SELECT_MARK        (((3)&0x3)<<4)      
#define UART_PARITY_SELECT_MASK        (0x3<<4)            
#define UART_PARITY_SELECT_SHIFT       (4)                 

#define UART_DIVISOR_MODE              (1<<20)             

#define UART_IRDA_ENABLE               (1<<21)             

#define UART_DMA_MODE                  (1<<22)             
#define UART_DMA_MODE_DISABLE          (0<<22)             
#define UART_DMA_MODE_ENABLE           (1<<22)             

#define UART_AUTO_FLOW_CONTROL         (1<<23)             
#define UART_AUTO_FLOW_CONTROL_ENABLE  (1<<23)             
#define UART_AUTO_FLOW_CONTROL_DISABLE (0<<23)             

#define UART_LOOP_BACK_MODE            (1<<24)             

#define UART_RX_LOCK_ERR               (1<<25)             

#define UART_RX_BREAK_LENGTH(n)        (((n)&0xF)<<28)     
#define UART_RX_BREAK_LENGTH_MASK      (0xF<<28)           
#define UART_RX_BREAK_LENGTH_SHIFT     (28)                


//status
#define UART_RX_FIFO_LEVEL(n)          (((n)&0x7F)<<0)     
#define UART_RX_FIFO_LEVEL_MASK        (0x7F<<0)           
#define UART_RX_FIFO_LEVEL_SHIFT       (0)                 

#define UART_TX_FIFO_SPACE(n)          (((n)&0x1F)<<8)     
#define UART_TX_FIFO_SPACE_MASK        (0x1F<<8)           
#define UART_TX_FIFO_SPACE_SHIFT       (8)                 

#define UART_TX_ACTIVE                 (1<<14)             

#define UART_RX_ACTIVE                 (1<<15)             

#define UART_RX_OVERFLOW_ERR           (1<<16)             

#define UART_TX_OVERFLOW_ERR           (1<<17)             

#define UART_RX_PARITY_ERR             (1<<18)             

#define UART_RX_FRAMING_ERR            (1<<19)             

#define UART_RX_BREAK_INT              (1<<20)             

#define UART_DCTS                      (1<<24)             

#define UART_CTS                       (1<<25)             

#define UART_DTR                       (1<<28)             

#define UART_CLK_ENABLED               (1<<31)             


//rxtx_buffer
#define UART_DATA(n)                   (((n)&0xFF)<<0)     
#define UART_DATA_MASK                 (0xFF<<0)           
#define UART_DATA_SHIFT                (0)                 


//irq_mask
#define UART_TX_MODEM_STATUS           (1<<0)              

#define UART_RX_DATA_AVAILABLE         (1<<1)              

#define UART_TX_DATA_NEEDED            (1<<2)              

#define UART_RX_TIMEOUT                (1<<3)              

#define UART_RX_LINE_ERR               (1<<4)              

#define UART_TX_DMA_DONE               (1<<5)              

#define UART_RX_DMA_DONE               (1<<6)              

#define UART_RX_DMA_TIMEOUT            (1<<7)              

#define UART_DTR_RISE                  (1<<8)              

#define UART_DTR_FALL                  (1<<9)              


//irq_cause
#define UART_TX_MODEM_STATUS           (1<<0)              

#define UART_RX_DATA_AVAILABLE         (1<<1)              

#define UART_TX_DATA_NEEDED            (1<<2)              

#define UART_RX_TIMEOUT                (1<<3)              

#define UART_RX_LINE_ERR               (1<<4)              

#define UART_TX_DMA_DONE               (1<<5)              

#define UART_RX_DMA_DONE               (1<<6)              

#define UART_RX_DMA_TIMEOUT            (1<<7)              

#define UART_DTR_RISE                  (1<<8)              

#define UART_DTR_FALL                  (1<<9)              

#define UART_TX_MODEM_STATUS_U         (1<<16)             

#define UART_RX_DATA_AVAILABLE_U       (1<<17)             

#define UART_TX_DATA_NEEDED_U          (1<<18)             

#define UART_RX_TIMEOUT_U              (1<<19)             

#define UART_RX_LINE_ERR_U             (1<<20)             

#define UART_TX_DMA_DONE_U             (1<<21)             

#define UART_RX_DMA_DONE_U             (1<<22)             

#define UART_RX_DMA_TIMEOUT_U          (1<<23)             

#define UART_DTR_RISE_U                (1<<24)             

#define UART_DTR_FALL_U                (1<<25)             


//triggers
#define UART_RX_TRIGGER(n)             (((n)&0x3F)<<0)     
#define UART_RX_TRIGGER_MASK           (0x3F<<0)           
#define UART_RX_TRIGGER_SHIFT          (0)                 

#define UART_TX_TRIGGER(n)             (((n)&0xF)<<8)      
#define UART_TX_TRIGGER_MASK           (0xF<<8)            
#define UART_TX_TRIGGER_SHIFT          (8)                 

#define UART_AFC_LEVEL(n)              (((n)&0x3F)<<16)    
#define UART_AFC_LEVEL_MASK            (0x3F<<16)          
#define UART_AFC_LEVEL_SHIFT           (16)                


//cmd_set
#define UART_RI                        (1<<0)              

#define UART_DCD                       (1<<1)              

#define UART_DSR                       (1<<2)              

#define UART_TX_BREAK_CONTROL          (1<<3)              

#define UART_TX_FINISH_N_WAIT          (1<<4)              

#define UART_RX_RTS                    (1<<5)              

#define UART_RX_FIFO_RESET             (1<<6)              

#define UART_TX_FIFO_RESET             (1<<7)              


//cmd_clr
#define UART_RI                        (1<<0)              

#define UART_DCR                       (1<<1)              

#define UART_DSR                       (1<<2)              

#define UART_TX_BREAK_CONTROL          (1<<3)              

#define UART_TX_FINISH_N_WAIT          (1<<4)              

#define UART_RX_CPU_RTS                (1<<5)              


//auto_baud
#define UART_AUTO_ENABLE               (1<<0)              

#define UART_AUTO_TRACKING             (1<<1)              

#define UART_VERIFY_2BYTE              (1<<2)              

#define UART_VERIFY_CHAR0(n)           (((n)&0xFF)<<8)     
#define UART_VERIFY_CHAR0_MASK         (0xFF<<8)           
#define UART_VERIFY_CHAR0_SHIFT        (8)                 

#define UART_VERIFY_CHAR1(n)           (((n)&0xFF)<<16)    
#define UART_VERIFY_CHAR1_MASK         (0xFF<<16)          
#define UART_VERIFY_CHAR1_SHIFT        (16)                


#endif

#endif


