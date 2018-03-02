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

#ifndef _IOT_UART_H_
#define _IOT_UART_H_

#include "cs_types.h"

// ============================================================================
// uart_Open
// ----------------------------------------------------------------------------
///
///
/// @param id, UART id
/// @param baudrate, UART baurate
// ===========================================================================
VOID uart_Open(UINT8 id, UINT32 baudrate);

// ============================================================================
// uart_Close
// ----------------------------------------------------------------------------
///
///
/// @param id, UART id
// ===========================================================================
VOID uart_Close(UINT8 id);

// ============================================================================
// uart_Write
// ----------------------------------------------------------------------------
///
///
/// @param id, UART id
/// @param ch, One char to write. Only write one char at a time for simplify
// ===========================================================================
VOID uart_Write(UINT8 id, CHAR ch);

// ============================================================================
// uart_Read
// ----------------------------------------------------------------------------
///
///
/// @param id, UART ID
/// @param buff, data buffer
/// @param len, actual data length read
// ===========================================================================
UINT32 uart_Read(UINT8 id, CHAR* buff);

#endif
