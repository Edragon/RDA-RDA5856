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
///                                                                           //
///     @file   halp_key.h 
///     
///     Keypad module irq handler
///                                                                           //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 

#ifndef  _HALP_KEY_H_
#define  _HALP_KEY_H_

#include "cs_types.h"

// ============================================================================
//      hal_KeyIrqHandler
// ----------------------------------------------------------------------------
///     Internal interrupt handler for the keypad physical interruptions.
///     
///     It handles the edge detection by starting the timer IRQ.
///     It handles the key timer interruptions by debouncing and calling the user
///     callback function for each key event (up, down or pressed).
///     It manages the hw interrupts of the keypad (clears the IRQ in the module).
///     
///     It handles also the interrupt for the GPIO used for the on-off button.
///     About the interrupt for the on-off button: Physically, this button is
///     mapped on a GPIO. The GPIO HAL handler will call this function when an
///     interrupt is detected on the corresponding GPIO. When this rising edge is
///     detected, the behavior is the same as the one that occurs when a normal
///     key is pressed: it disable the edge IRQ and starts the key timer IRQ.
///
///     @param interruptId The Id of the interrruption
// ============================================================================
PROTECTED VOID hal_KeyIrqHandler(UINT8 interruptId);



#endif //  _HALP_KEY_H_ 
