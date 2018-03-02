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
/// @file halp_sim.h                                                          //
/// Prototypes for private SIM functions                     
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_SIM_H_
#define  _HALP_SIM_H_

#include "cs_types.h"
#include "hal_sys.h"



// ============================================================================
// hal_SimIrqHandler
// ----------------------------------------------------------------------------
/// SIM module irq handler for the IRQ module.
///
/// Clear the IRQ and call the IRQ handler user function.
/// @param interruptId Identifier of the interrupt.
// ============================================================================
PROTECTED VOID hal_SimIrqHandler(UINT8 interruptId);



// ============================================================================
// hal_SimCheckClockStatus
// ----------------------------------------------------------------------------
/// This function checks the clock status of the SIM modules. When it is not
/// needed, the request for a clock is given up (ie request for no clock
/// is done.)
// ============================================================================
PROTECTED VOID hal_SimCheckClockStatus(VOID);



#endif //  _HALP_SIM_H_ 


