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
#ifndef _UART2_HCI_TRANSPORT_H_
#define _UART2_HCI_TRANSPORT_H_

int uart2_hcit_init(void) ;
int uart2_hcit_shutdown(void) ;
void uart2_hcit_rx_data(UINT8 *pdu,UINT32 length);


#endif // _UART2_HCI_TRANSPORT_H_

