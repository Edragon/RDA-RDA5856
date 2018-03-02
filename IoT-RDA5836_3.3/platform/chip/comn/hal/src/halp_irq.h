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
/// @file   halp_irq.h                                               
/// Prototypes for private IRQ functions and defines                 
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef  _HALP_IRQ_H_
#define  _HALP_IRQ_H_

#include "cs_types.h"




/// 
///     Function pointer type used to set module irq handlers
/// 
typedef VOID (*HAL_MODULE_IRQ_HANDLER_T)(UINT8);



// Global Var

/// 
///     The registry array 
/// 


extern HAL_MODULE_IRQ_HANDLER_T hal_HwModuleIrqHandler[];

/// 
///     Irq id array
/// 
extern UINT8 hal_HwModuleIrqId[];



// API Functions
//  ---------- Init Irq ----------------- 
/// 
///     Irq init (used only for test without sx)
/// 
VOID hal_IrqInit(BOOL irq_in_rom, UINT8 mask);




#endif //  HAL_IRQ_H 


