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

#ifndef _IOT_GPIO_H_
#define _IOT_GPIO_H_

#include "cs_types.h"

// ============================================================================
// gpio_Open
// ----------------------------------------------------------------------------
///
///
/// @param port, GPIO port number
/// @param direction, Open GPIO as input or output or both
// ===========================================================================
VOID  gpio_Open(UINT8 port, UINT8 direction)

// ============================================================================
// gpio_Close
// ----------------------------------------------------------------------------
/// Close the GPIO port
///
/// @param port, GPIO port number
// ===========================================================================
VOID  gpio_Close(UINT8 port);

// ============================================================================
// gpio_Write
// ----------------------------------------------------------------------------
/// Write data to GPIO port
///
/// @param port, GPIO port number
/// @param data, data write to GPIO
// ===========================================================================
VOID gpio_Write(UINT8 port, UINT8 data); 

// ============================================================================
// gpio_Read
// ----------------------------------------------------------------------------
/// Read data from GPIO port
///
/// @param ptr Memory pointer 
// ===========================================================================
UINT32 gpio_Read(UINT8 port);

#endif

