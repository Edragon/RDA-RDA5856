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


#ifndef _RDABT_ADP_TRANSPORT_UART_H_
#define _RDABT_ADP_TRANSPORT_UART_H_

#ifdef BT_SHAREMEM_TRANSPORT

t_api rdabt_hcit_sharemem_init(void);
t_api rdabt_hcit_sharemem_shutdown(void) ;

void rdabt_hcit_sharemem_rx_data(void);
t_api rdabt_hcit_sharemem_tx_pdu(t_DataBuf *pdu, UINT8 type, UINT16 flags, UINT16 hci_handle_flags);
t_api rdabt_hcit_sharemem_tx_data(UINT8 port, UINT8 *pdu, UINT16 len);


#define RDABT_Tranport_Init           rdabt_hcit_sharemem_init
#define RDABT_Tranport_Shutdown       rdabt_hcit_sharemem_shutdown
#define RDABT_Tranport_Rx_Data        rdabt_hcit_sharemem_rx_data
#define RDABT_Tranport_Tx_Data        rdabt_hcit_sharemem_tx_data
#define RDABT_Tranport_Tx_PDU         rdabt_hcit_sharemem_tx_pdu

#else

t_api rdabt_hcit_uart_init(void);
t_api rdabt_hcit_uart_shutdown(void) ;
void rdabt_uart_rx_data(u_int8 *pdu,u_int32 length);
t_api rdabt_uart_tx_pdu( t_DataBuf *pdu, u_int8 type, u_int16 flags, u_int16 hci_handle_flags) ;
t_api rdabt_uart_tx_data(u_int8 port, u_int8 *pdu, u_int8 len) ;

#define RDABT_Tranport_Init           rdabt_hcit_uart_init
#define RDABT_Tranport_Shutdown       rdabt_hcit_uart_shutdown
#define RDABT_Tranport_Rx_Data        rdabt_uart_rx_data
#define RDABT_Tranport_Tx_Data        rdabt_uart_tx_data
#define RDABT_Tranport_Tx_PDU         rdabt_uart_tx_pdu

#endif

#endif // _RDABT_ADP_TRANSPORT_UART_H_

