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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_uart.h 
///     Definition of the private functions of HAL's UART Driver. 
///     Caution: This file is strictly confidential!!!
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_UART_H_
#define  _HALP_UART_H_

#include "cs_types.h"
#include "hal_uart.h"

// ============================================================================
// hal_UartIrqHandler
// ----------------------------------------------------------------------------
/// UART module IRQ handler
/// Clear the IRQ and call the IRQ handler user function
// ============================================================================
PROTECTED VOID hal_UartIrqHandler(UINT8);


#endif //  HAL_UART_IRQ_H 

