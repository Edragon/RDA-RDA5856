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
#ifndef UART2_H
#define UART2_H

#define UART_RX_BUFFER 1024
#define UART_TX_BUFFER 1024
#define UART2_PORT       1

void uart2_start(void);
void uart2_stop(void);
void uart2_wakeup();

void uart2_deal_data(UINT32 param);
UINT32 uart2_send_data(const UINT8* data, UINT32 length);

#endif
