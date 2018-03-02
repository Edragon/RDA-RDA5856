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
/// @file halp_gpio.h                                                         //
/// This file contains Granite's GPIO driver implementation                   //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 


#ifndef  _HALP_GPIO_H_
#define  _HALP_GPIO_H_

#include "hal_gpio.h"

// =============================================================================
// MACROS
// =============================================================================

// =============================================================================
// HAL_GPIO_BIT
// -----------------------------------------------------------------------------
/// This macro is used by internal code to convert gpio number to bit.
/// It masks the upper bit so it can be used directly with #HAL_GPIO_GPIO_ID_T.
// =============================================================================
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
#define HAL_GPIO_BIT(n)    (1<<((n)&0x3f))
#else
#define HAL_GPIO_BIT(arrayIndex, offset)    \
             (1 << ((((arrayIndex)%4)&0x7) * 8 +((offset)&0x7)))
#endif

// =============================================================================
// HAL_GPO_BIT
// -----------------------------------------------------------------------------
/// This macro is used by internal code to convert gpio number to bit.
/// It masks the upper bit so it can be used directly with #HAL_GPIO_GPO_ID_T.
// =============================================================================
#define HAL_GPO_BIT(n)    (1<<((n)&0x3f))




// =============================================================================
// hal_GpioIrqHandler
// -----------------------------------------------------------------------------
/// GPIO module IRQ handler
/// 
///     Clear the IRQ and call the IRQ handler
///     user function
///     @param interruptId The interruption ID
// =============================================================================  
PROTECTED VOID hal_GpioIrqHandler(UINT8 interruptId);


#endif //HAL_GPIO_H
