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
// File name      : debug_uart                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:38                                                  
// Last modified  : 2016-12-14 15:57:38                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:38 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DEBUG_UART_H__
#define __DEBUG_UART_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Allows to turn off the UART:0: Disable1: Enable*/
        U32 data_bits                      :1 ; /*1 :1 , Number of data bits per character (least significant bit first):0: 7_bits1: 8_bits This bit will be masked to '1' if debug host is enabled.*/
        U32 tx_stop_bits                   :1 ; /*2 :2 , Stop bits controls the number of stop bits transmitted. Can receive with one stop bit (more inaccuracy can be compensated with two stop bits when divisor mode is set to 0).0 = one stop bit is transmitted in the serial data.1 = two stop bits are generated and transmitted in the serial data out. This bit will be masked to '0' if debug host is enabled.0:   1_BIT1:   2_BITS*/
        U32 parity_enable                  :1 ; /*3 :3 , Parity is enabled when this bit is set. This bit will be masked to '0' if debug host is enabled.0:   NO1:   YES*/
        U32 parity_select                  :2 ; /*5 :4 , Controls the parity format when parity is enabled:0: odd an odd number of received 1 bits is checked, or transmitted (the parity bit is included).1: even an even number of received 1 bits is checked or transmitted (the parity bit is included).2: space a space is generated and received as parity bit.3:  mark a mark is generated and received as parity bit. These bit will be ignored if debug host is enabled.*/
        U32 tx_break_control               :1 ; /*6 :6 , Sends a break signal by holding the Uart_Tx line low until this bit is cleared.This bit will be masked to '0' if debug host is enabled.0:   OFF1:   ON*/
        U32 rx_fifo_reset                  :1 ; /*7 :7 , Writing a 1 to this bit resets and flushes the Receive Fifo. This bit does not need to be cleared.*/
        U32 tx_fifo_reset                  :1 ; /*8 :8 , Writing a 1 to this bit resets and flushes the Transmit Fifo. This bit does not need to be cleared.*/
        U32 dma_mode                       :1 ; /*9 :9 , Enables the DMA signaling for the Uart_Dma_Tx_Req_H and Uart_Dma_Rx_Req_H to the IFC.0:   DISABLE1:   ENABLE*/
        U32 reserved_0                     :1 ; /*10:10,                               */
        U32 reserved_1                     :1 ; /*11:11,                               */
        U32 swrx_flow_ctrl                 :2 ; /*13:12, When this field is "00" and SWTX_flow_Ctrl is also "00", hardwre flow ctrl is used. Otherwise, software flow control is used: 0: no transmit flow control. 1: transmit XON1/XOFF1 as flow control bytes2: transmit XON2/XOFF2 as flow control bytes3: transmit XON1 and XON2/XOFF1 and XOFF2 as flow control bytes*/
        U32 swtx_flow_ctrl                 :2 ; /*15:14, When this field is "00" and SWRX_flow_Ctrl is also "00", hardwre flow ctrl is used. Otherwise, software flow control is used: 00 = no receive flow control01 = receive XON1/XOFF1 as flow control bytes10 = receive XON2/XOFF2 as flow control bytes11 = receive XON1 and XON2/XOFF1 and XOFF2 as flow control bytesNote: If single XON/XOFF character is used for flow contol, the received XON/XOFF character will not be put into Rx FIFO. This is also the case if XON is received when XOFF is expected. If double XON/XOFF characters are expected, the XON1/XOFF1 must followed sequently by XON2/XOFF2 to be considered as patterns, which will not be put into Rx FIFO. Otherwise they will be considered as data. This is also the case if XOFF1 is followed by character other than XOFF2. */
        U32 backslash_en                   :1 ; /*16:16, When soft flow control characters or backslash are encountered in the data file, they will be inverted and a backslash will be added before them. for example, if tx data is XON(0x11) with BackSlash_En = '1', then uart will send 5Ch(Backslash) + EEh (~XON).*/
        U32 send_xon                       :1 ; /*17:17, Write in this bit will send an XON. This command should be used only for debug. It can be used in case when UART has not received any byte for long time after previous XON is sent. This bit does not need to be cleared. */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 tx_finish_n_wait               :1 ; /*19:19, When this bit is set the Tx engine terminates to send the current byte and then it stops to send data.*/
        U32 divisor_mode                   :1 ; /*20:20, Selects the divisor value used to generate the baud rate frequency (BCLK) from the SCLK (see UART Operation for details). If IrDA is enable, this bit is ignored and the divisor used will be 16.0 = (BCLK = SCLK / 4)1 = (BCLK = SCLK / 16) This bit will be masked to '0' if debug host is enabled.*/
        U32 irda_enable                    :1 ; /*21:21, When set, the UART is in IrDA mode and the baud rate divisor used is 16 (see UART Operation for details). This bit will be masked to '0' if debug host is enabled.*/
        U32 rx_rts                         :1 ; /*22:22, Controls the Uart_RTS output (not directly in auto flow control mode).0 = the Uart_RTS will be inactive high1 = the Uart_RTS will be active low This bit will be masked to '1' if debug host is enabled.0:   INACTIVE1:   ACTIVE*/
        U32 auto_flow_control              :1 ; /*23:23, Enables the auto flow control. In case HW flow control (both swTx_Flow_ctrl=0 and swRx_Flow_Ctrl=0), If Auto_Flow_Control is enabled, Uart_RTS is controlled by the Rx RTS bit in CMD_Set register and the UART Auto Control Flow System(flow controlled by Rx Fifo Level and AFC_Level in Triggers register). Tx data flow is stopped If Uart_CTS become inactive high.If Auto_Flow_Control is disabled, Uart_RTS is controlled only by the Rx RTS bit in CMD_Set register. Uart_CTS will not take effect. In case SW flow control(either swTx_Flow_ctrl/=0 or swRx_Flow_Ctrl/=0), If Auto_Flow_Control is enabled, XON/XOFF will be controlled by the Rx RTS bit in CMD_Set register and the UART Auto Control Flow System(flow controlled by Rx Fifo Level and AFC_Level in Triggers register). If Auto_Flow_Control is disabled, XON/XOFF will be controlled only by Rx RTS bit in CMD_Set register. Tx data flow will be stoped when XOFF is received either this bit is enable or disabled.This bit will be masked to '1' if debug host is enabled.1:  ENABLE0:  DISABLE*/
        U32 loop_back_mode                 :1 ; /*24:24, When set, data on the Uart_Tx line is held high, while the serial output is looped back to the serial input line, internally. In this mode all the interrupts are fully functional. This feature is used for diagnostic purposes. Also, in loop back mode, the modem control input Uart_CTS is disconnected and the modem control output Uart_RTS are looped back to the inputs, internally. In IrDA mode, Uart_Tx signal is inverted (see IrDA SIR Mode Support).*/
        U32 rx_lock_err                    :1 ; /*25:25, Allow to stop the data receiving when an error is detected (framing, parity or break). The data in the fifo are kept. This bit will be masked to '0' if debug host is enabled.0:   DISABLE1:   ENABLE*/
        U32 hst_txd_oen                    :1 ; /*26:26, HST TXD output enable. '0' enable.1:   DISABLE0:   ENABLE*/
        U32 reserved_3                     :1 ; /*27:27,                               */
        U32 rx_break_length                :4 ; /*31:28, Length of a break, in number of bits. This bit will be masked to "1011" if debug host is enabled.*/
    } b;
} T_DEBUG_UART_ctrl;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 rx_fifo_level                  :5 ; /*4 :0 , Those bits indicate the number of data available in the Rx Fifo. Those data can be read.*/
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 tx_fifo_level                  :5 ; /*12:8 , Those bits indicate the number of data available in the Tx Fifo. Those data will be sent.*/
        U32 tx_active                      :1 ; /*13:13, This bit indicates that the UART is sending data. If no data is in the fifo, the UART is currently sending the last one through the serial interface.*/
        U32 rx_active                      :1 ; /*14:14, This bit indicates that the UART is receiving a byte.*/
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 rx_overflow_err                :1 ; /*16:16, This bit indicates that the receiver received a new character when the fifo was already full. The new character is discarded. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 tx_overflow_err                :1 ; /*17:17, This bit indicates that the user tried to write a character when fifo was already full. The written data will not be kept. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_parity_err                  :1 ; /*18:18, This bit is set if the parity is enabled and a parity error occurred in the received data. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_framing_err                 :1 ; /*19:19, This bit is set whenever there is a framing error occured. A framing error occurs when the receiver does not detect a valid STOP bit in the received data. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 rx_break_int                   :1 ; /*20:20, This bit is set whenever the serial input is held in a logic 0 state for longer than the length of x bits, where x is the value programmed Rx Break Length. A null word will be written in the Rx Fifo. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 reserved_4                     :1 ; /*21:21,                               */
        U32 reserved_5                     :1 ; /*22:22,                               */
        U32 reserved_6                     :1 ; /*23:23,                               */
        U32 tx_dcts                        :1 ; /*24:24, In case HW flow ctrl(both swRx_Flow_Ctrl=0 and swTx_Flow_Ctrl=0), This bit is set when the Uart_CTS line changed since the last time this register has been written. In case SW flow ctrl(either swRx_Flow_Ctrl/=0 or swTx_Flow_Ctrl/=0), This bit is set when received XON/XOFF status changed since the last time this register has been writtern. This bit is cleared when the UART_STATUS register is written with any value.*/
        U32 tx_cts                         :1 ; /*25:25, In case HW flow ctrl(both swRx_Flow_Ctrl=0 and swTx_Flow_Ctrl=0), current value of the Uart_CTS line. '1' = Tx not allowed. '0' = Tx allowed. In case SW flow ctrl(either swRx_Flow_Ctrl/=0 or swTx_Flow_Ctrl/=0), current state of software flow control. '1' = when XOFF received. '0' = when XON received.*/
        U32 reserved_7                     :1 ; /*26:26,                               */
        U32 reserved_8                     :1 ; /*27:27,                               */
        U32 tx_fifo_rsted_l                :1 ; /*28:28, This bit is set when Tx Fifo Reset command is received by CTRL register and is cleared when Tx fifo reset process has finished.*/
        U32 rx_fifo_rsted_l                :1 ; /*29:29, This bit is set when Rx Fifo Reset command is received by CTRL register and is cleared when Rx fifo reset process has finished.*/
        U32 enable_n_finished              :1 ; /*30:30, This bit is set when bit enable is changed from '0' to '1' or from '1' to '0', it is cleared when the enable process has finished.*/
        U32 clk_enabled                    :1 ; /*31:31, This bit is set when Uart Clk has been enabled and received by UART after Need Uart Clock becomes active. It serves to avoid enabling Rx RTS too early.*/
    } b;
} T_DEBUG_UART_status;

//rxtx_buffer
typedef union
{
    U32 v;
    struct
    {
        U32 data_buffer                    :8 ; /*7 :0 , When read:The UART_RECEIVE_BUFFER register is a read-only register that contains the data byte received on the serial input port. This register accesses the head of the receive FIFO. If the receive FIFO is full and this register is not read before the next data character arrives, then the data already in the FIFO will be preserved but any incoming data will be lost. An overflow error will also occur.when write:The UART_TRANSMIT_HOLDING register is a write-only register that contains data to be transmitted on the serial output port. 16 characters of data may be written to the UART_TRANSMIT_HOLDING register before the FIFO is full. Any attempt to write data when the FIFO is full results in the write data being lost.*/
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
} T_DEBUG_UART_rxtx_buffer;

//irq_mask
typedef union
{
    U32 v;
    struct
    {
        U32 tx_modem_status                :1 ; /*0 :0 , Clear to send signal change or XON/XOFF detected.*/
        U32 rx_data_available              :1 ; /*1 :1 , Rx Fifo at or upper threshold level (current level &gt;= Rx Fifo trigger level).*/
        U32 tx_data_needed                 :1 ; /*2 :2 , Tx Fifo at or below threshold level (current level &lt;= Tx Fifo trigger level).*/
        U32 rx_timeout                     :1 ; /*3 :3 , No characters in or out of the Rx Fifo during the last 4 character times and there is at least 1 character in it during this time.*/
        U32 rx_line_err                    :1 ; /*4 :4 , Tx Overflow, Rx Overflow, Parity Error, Framing Error or Break Interrupt.*/
        U32 tx_dma_done                    :1 ; /*5 :5 , Pulse detected on Uart_Dma_Tx_Done_H signal.*/
        U32 rx_dma_done                    :1 ; /*6 :6 , Pulse detected on Uart_Dma_Rx_Done_H signal.*/
        U32 rx_dma_timeout                 :1 ; /*7 :7 , In DMA mode, there is at least 1 character that has been read in or out the Rx Fifo. Then before received Rx DMA Done, No characters in or out of the Rx Fifo during the last 4 character times.*/
        U32 xoff_detected                  :1 ; /*8 :8 , No description.               */
        U32 reserved_0                     :1 ; /*9 :9 ,                               */
        U32 reserved_1                     :1 ; /*10:10,                               */
        U32 reserved_2                     :1 ; /*11:11,                               */
        U32 reserved_3                     :1 ; /*12:12,                               */
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 reserved_5                     :1 ; /*14:14,                               */
        U32 reserved_6                     :1 ; /*15:15,                               */
        U32 reserved_7                     :1 ; /*16:16,                               */
        U32 reserved_8                     :1 ; /*17:17,                               */
        U32 reserved_9                     :1 ; /*18:18,                               */
        U32 reserved_10                    :1 ; /*19:19,                               */
        U32 reserved_11                    :1 ; /*20:20,                               */
        U32 reserved_12                    :1 ; /*21:21,                               */
        U32 reserved_13                    :1 ; /*22:22,                               */
        U32 reserved_14                    :1 ; /*23:23,                               */
        U32 reserved_15                    :1 ; /*24:24,                               */
        U32 reserved_16                    :1 ; /*25:25,                               */
        U32 reserved_17                    :1 ; /*26:26,                               */
        U32 reserved_18                    :1 ; /*27:27,                               */
        U32 reserved_19                    :1 ; /*28:28,                               */
        U32 reserved_20                    :1 ; /*29:29,                               */
        U32 reserved_21                    :1 ; /*30:30,                               */
        U32 reserved_22                    :1 ; /*31:31,                               */
    } b;
} T_DEBUG_UART_irq_mask;

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
        U32 xoff_detected                  :1 ; /*8 :8 , No description.               */
        U32 reserved_0                     :1 ; /*9 :9 ,                               */
        U32 reserved_1                     :1 ; /*10:10,                               */
        U32 reserved_2                     :1 ; /*11:11,                               */
        U32 reserved_3                     :1 ; /*12:12,                               */
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 reserved_5                     :1 ; /*14:14,                               */
        U32 reserved_6                     :1 ; /*15:15,                               */
        U32 tx_modem_status_u              :1 ; /*16:16, tx_modem_status_unmask        */
        U32 rx_data_available_u            :1 ; /*17:17, Same as previous, not masked. */
        U32 tx_data_needed_u               :1 ; /*18:18, Same as previous, not masked. */
        U32 rx_timeout_u                   :1 ; /*19:19, Same as previous, not masked. */
        U32 rx_line_err_u                  :1 ; /*20:20, Same as previous, not masked. */
        U32 tx_dma_done_u                  :1 ; /*21:21, Same as previous, not masked. */
        U32 rx_dma_done_u                  :1 ; /*22:22, Same as previous, not masked. */
        U32 rx_dma_timeout_u               :1 ; /*23:23, Same as previous, not masked. */
        U32 xoff_detected_u                :1 ; /*24:24, Same as previous, not masked. */
        U32 reserved_7                     :1 ; /*25:25,                               */
        U32 reserved_8                     :1 ; /*26:26,                               */
        U32 reserved_9                     :1 ; /*27:27,                               */
        U32 reserved_10                    :1 ; /*28:28,                               */
        U32 reserved_11                    :1 ; /*29:29,                               */
        U32 reserved_12                    :1 ; /*30:30,                               */
        U32 reserved_13                    :1 ; /*31:31,                               */
    } b;
} T_DEBUG_UART_irq_cause;

//triggers
typedef union
{
    U32 v;
    struct
    {
        U32 rx_trigger                     :4 ; /*3 :0 , Defines the threshold level at which the Data Available Interrupt will be generated. The Data Available interrupt is generated when quantity of data in Rx Fifo &gt; Rx Trigger.*/
        U32 tx_trigger                     :4 ; /*7 :4 , Defines the threshold level at which the Data Needed Interrupt will be generated.The Data Needed Interrupt is generated when quantity of data in Tx Fifo &lt;= Tx Trigger.*/
        U32 afc_level                      :4 ; /*11:8 , Controls the Rx Fifo level at which the Uart_RTS Auto Flow Control will be set inactive high (see UART Operation for more details on AFC).The Uart_RTS Auto Flow Control will be set inactive high when quantity of data in Rx Fifo &gt; AFC Level.*/
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_DEBUG_UART_triggers;

//xchar
typedef union
{
    U32 v;
    struct
    {
        U32 xon1                           :8 ; /*7 :0 , XON1 character value. Reset Value is CTRL-Q 0x11.*/
        U32 xoff1                          :8 ; /*15:8 , XOFF1 character value. Reset Value is CTRL-S 0x13*/
        U32 xon2                           :8 ; /*23:16, XON2 character value.         */
        U32 xoff2                          :8 ; /*31:24, XOFF2 character value.        */
    } b;
} T_DEBUG_UART_xchar;

typedef struct
{
    volatile               T_DEBUG_UART_ctrl                           ctrl; /*0x0  , RW   , 0xF0015000, No description.               */
    volatile             T_DEBUG_UART_status                         status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile        T_DEBUG_UART_rxtx_buffer                    rxtx_buffer; /*0x8  , RW   , 0x00000000, No description.               */
    volatile           T_DEBUG_UART_irq_mask                       irq_mask; /*0xc  , RW   , 0x00000000, No description.               */
    volatile          T_DEBUG_UART_irq_cause                      irq_cause; /*0x10 , RO   , 0x00000000, No description.               */
    volatile           T_DEBUG_UART_triggers                       triggers; /*0x14 , RW   , 0x00000000, No description.               */
    volatile              T_DEBUG_UART_xchar                          xchar; /*0x18 , RW   , 0x00001311, No description.               */
} T_HWP_DEBUG_UART_T;
#else
typedef struct
{
    volatile                             U32                           ctrl; /*0x0  , RW   , 0xF0015000, No description.               */
    volatile                             U32                         status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                    rxtx_buffer; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                       irq_mask; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                      irq_cause; /*0x10 , RO   , 0x00000000, No description.               */
    volatile                             U32                       triggers; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                             U32                          xchar; /*0x18 , RW   , 0x00001311, No description.               */
} T_HWP_DEBUG_UART_T;

//ctrl
#define DEBUG_UART_ENABLE              (1<<0)              
#define DEBUG_UART_ENABLE_DISABLE      (0<<0)              
#define DEBUG_UART_ENABLE_ENABLE       (1<<0)              

#define DEBUG_UART_DATA_BITS           (1<<1)              
#define DEBUG_UART_DATA_BITS_7_BITS    (0<<1)              
#define DEBUG_UART_DATA_BITS_8_BITS    (1<<1)              

#define DEBUG_UART_TX_STOP_BITS        (1<<2)              
#define DEBUG_UART_TX_STOP_BITS_1_BIT  (0<<2)              
#define DEBUG_UART_TX_STOP_BITS_2_BITS (1<<2)              

#define DEBUG_UART_PARITY_ENABLE       (1<<3)              
#define DEBUG_UART_PARITY_ENABLE_NO    (0<<3)              
#define DEBUG_UART_PARITY_ENABLE_YES   (1<<3)              

#define DEBUG_UART_PARITY_SELECT(n)    (((n)&0x3)<<4)      
#define DEBUG_UART_PARITY_SELECT_ODD   (((0)&0x3)<<4)      
#define DEBUG_UART_PARITY_SELECT_EVEN  (((1)&0x3)<<4)      
#define DEBUG_UART_PARITY_SELECT_SPACE (((2)&0x3)<<4)      
#define DEBUG_UART_PARITY_SELECT_MARK  (((3)&0x3)<<4)      
#define DEBUG_UART_PARITY_SELECT_MASK  (0x3<<4)            
#define DEBUG_UART_PARITY_SELECT_SHIFT (4)                 

#define DEBUG_UART_TX_BREAK_CONTROL    (1<<6)              
#define DEBUG_UART_TX_BREAK_CONTROL_OFF (0<<6)              
#define DEBUG_UART_TX_BREAK_CONTROL_ON (1<<6)              

#define DEBUG_UART_RX_FIFO_RESET       (1<<7)              

#define DEBUG_UART_TX_FIFO_RESET       (1<<8)              

#define DEBUG_UART_DMA_MODE            (1<<9)              
#define DEBUG_UART_DMA_MODE_DISABLE    (0<<9)              
#define DEBUG_UART_DMA_MODE_ENABLE     (1<<9)              

#define DEBUG_UART_SWRX_FLOW_CTRL(n)   (((n)&0x3)<<12)     
#define DEBUG_UART_SWRX_FLOW_CTRL_NO_FLOW_CTRL (((0)&0x3)<<12)     
#define DEBUG_UART_SWRX_FLOW_CTRL_MODE1 (((1)&0x3)<<12)     
#define DEBUG_UART_SWRX_FLOW_CTRL_MODE2 (((2)&0x3)<<12)     
#define DEBUG_UART_SWRX_FLOW_CTRL_MODE3 (((3)&0x3)<<12)     
#define DEBUG_UART_SWRX_FLOW_CTRL_MASK (0x3<<12)           
#define DEBUG_UART_SWRX_FLOW_CTRL_SHIFT (12)                

#define DEBUG_UART_SWTX_FLOW_CTRL(n)   (((n)&0x3)<<14)     
#define DEBUG_UART_SWTX_FLOW_CTRL_MASK (0x3<<14)           
#define DEBUG_UART_SWTX_FLOW_CTRL_SHIFT (14)                

#define DEBUG_UART_BACKSLASH_EN        (1<<16)             

#define DEBUG_UART_SEND_XON            (1<<17)             

#define DEBUG_UART_TX_FINISH_N_WAIT    (1<<19)             

#define DEBUG_UART_DIVISOR_MODE        (1<<20)             
#define DEBUG_UART_DIVISOR_MODE_DIV_16 (1<<20)             
#define DEBUG_UART_DIVISOR_MODE_DIV_4  (1<<20)             

#define DEBUG_UART_IRDA_ENABLE         (1<<21)             

#define DEBUG_UART_RX_RTS              (1<<22)             
#define DEBUG_UART_RX_RTS_INACTIVE     (0<<22)             
#define DEBUG_UART_RX_RTS_ACTIVE       (1<<22)             

#define DEBUG_UART_AUTO_FLOW_CONTROL   (1<<23)             
#define DEBUG_UART_AUTO_FLOW_CONTROL_ENABLE (1<<23)             
#define DEBUG_UART_AUTO_FLOW_CONTROL_DISABLE (0<<23)             

#define DEBUG_UART_LOOP_BACK_MODE      (1<<24)             

#define DEBUG_UART_RX_LOCK_ERR         (1<<25)             
#define DEBUG_UART_RX_LOCK_ERR_DISABLE (0<<25)             
#define DEBUG_UART_RX_LOCK_ERR_ENABLE  (1<<25)             

#define DEBUG_UART_HST_TXD_OEN         (1<<26)             
#define DEBUG_UART_HST_TXD_OEN_DISABLE (1<<26)             
#define DEBUG_UART_HST_TXD_OEN_ENABLE  (0<<26)             

#define DEBUG_UART_RX_BREAK_LENGTH(n)  (((n)&0xF)<<28)     
#define DEBUG_UART_RX_BREAK_LENGTH_MASK (0xF<<28)           
#define DEBUG_UART_RX_BREAK_LENGTH_SHIFT (28)                


//status
#define DEBUG_UART_RX_FIFO_LEVEL(n)    (((n)&0x1F)<<0)     
#define DEBUG_UART_RX_FIFO_LEVEL_MASK  (0x1F<<0)           
#define DEBUG_UART_RX_FIFO_LEVEL_SHIFT (0)                 

#define DEBUG_UART_TX_FIFO_LEVEL(n)    (((n)&0x1F)<<8)     
#define DEBUG_UART_TX_FIFO_LEVEL_MASK  (0x1F<<8)           
#define DEBUG_UART_TX_FIFO_LEVEL_SHIFT (8)                 

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
#define DEBUG_UART_DATA_BUFFER_MASK    (0xFF<<0)           
#define DEBUG_UART_DATA_BUFFER_SHIFT   (0)                 


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
#define DEBUG_UART_RX_TRIGGER_MASK     (0xF<<0)            
#define DEBUG_UART_RX_TRIGGER_SHIFT    (0)                 

#define DEBUG_UART_TX_TRIGGER(n)       (((n)&0xF)<<4)      
#define DEBUG_UART_TX_TRIGGER_MASK     (0xF<<4)            
#define DEBUG_UART_TX_TRIGGER_SHIFT    (4)                 

#define DEBUG_UART_AFC_LEVEL(n)        (((n)&0xF)<<8)      
#define DEBUG_UART_AFC_LEVEL_MASK      (0xF<<8)            
#define DEBUG_UART_AFC_LEVEL_SHIFT     (8)                 


//xchar
#define DEBUG_UART_XON1(n)             (((n)&0xFF)<<0)     
#define DEBUG_UART_XON1_MASK           (0xFF<<0)           
#define DEBUG_UART_XON1_SHIFT          (0)                 

#define DEBUG_UART_XOFF1(n)            (((n)&0xFF)<<8)     
#define DEBUG_UART_XOFF1_MASK          (0xFF<<8)           
#define DEBUG_UART_XOFF1_SHIFT         (8)                 

#define DEBUG_UART_XON2(n)             (((n)&0xFF)<<16)    
#define DEBUG_UART_XON2_MASK           (0xFF<<16)          
#define DEBUG_UART_XON2_SHIFT          (16)                

#define DEBUG_UART_XOFF2(n)            (((n)&0xFF)<<24)    
#define DEBUG_UART_XOFF2_MASK          (0xFF<<24)          
#define DEBUG_UART_XOFF2_SHIFT         (24)                


#endif

#endif


