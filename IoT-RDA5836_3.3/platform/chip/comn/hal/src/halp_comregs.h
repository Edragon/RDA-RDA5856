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
/// @file halp_comregs.h                                                      //
/// Definition of the private functions of Garnet's comregs Driver            //
/// Caution: This file is strictly confidential!!!                                                                         //
//////////////////////////////////////////////////////////////////////////////// 

#ifndef  _HALP_COMREGS_H_
#define  _HALP_COMREGS_H_

#include "cs_types.h"

// =============================================================================
// hal_ComregsIrqHandler
// -----------------------------------------------------------------------------
/// Clear the IRQ and call the IRQ handler user function.
/// @param interrupt_id Enables to choose one of the two interrupt 
///     lines (\c comregs0_irq or comregs1_irq)
// =============================================================================
PROTECTED VOID hal_ComregsIrqHandler (UINT8 interruptId);

// =============================================================================
// hal_ComregsSetMask
// -----------------------------------------------------------------------------
/// Configure the comregs module IRQ mask
/// @param interrupt_id Enables to choose one of the two interrupt 
///     lines (\c comregs0_irq or comregs1_irq)
// =============================================================================
//PROTECTED VOID hal_ComregsSetMask(UINT8 interrupt_id, UINT16 mask);

#endif // HAL_comregs_IRQ_H
