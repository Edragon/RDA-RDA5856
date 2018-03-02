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


#ifndef _RDABT_ADP_UART_DRV_H_
#define _RDABT_ADP_UART_DRV_H_

#define BT_UART_PORT	1
#define BT_UART_PORT2	2

void rdabt_adp_uart_start(UINT8 port);
void rdabt_adp_uart_stop(UINT8 port);
void rdabt_adp_uart_configure(UINT8 port, UINT32 baud,char flow_control);
void rdabt_uart_register_cb(UINT8 port);

void rdabt_adp_uart_rx(UINT8 port);
UINT8 rdabt_adp_uart_tx(UINT8 port, CONST UINT8 *buf, UINT16 num_to_send, UINT16 *num_send);
void rdabt_uart_eint_hdlr(void);


#endif // _RDABT_ADP_UART_DRV_H_

