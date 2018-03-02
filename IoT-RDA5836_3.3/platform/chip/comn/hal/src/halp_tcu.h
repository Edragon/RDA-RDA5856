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
/// @file halp_tcu.h
/// TCU private API. It holds the IRQ handling functions, types and mechanisms
/// to enable the TCU interrupt on the System Side. If they appear to be useful,
/// put them in the public interface.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef _HALP_TCU_H_
#define _HALP_TCU_H_


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_TCU_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// User IRQ handler type
// =============================================================================
typedef VOID (*HAL_TCU_IRQ_HANDLER_T)(VOID);



// =============================================================================
// HAL_TCU_IRQ_ID_T
// -----------------------------------------------------------------------------
/// Use  to discriminate the tcu irqs
// =============================================================================
typedef enum{
    HAL_TCU0_IRQ = 2,
    HAL_TCU1_IRQ = 3
} HAL_TCU_IRQ_ID_T;





// =============================================================================
//  FUNCTIONS
// =============================================================================




// =============================================================================
// hal_TcuIrqSetHandler
// -----------------------------------------------------------------------------
/// Set the user function to call when a tcu interrupt occurs.
/// @tcu Choose the TCU interrupt that will call this user handler.
/// @handler Handler called by this TCU interrupt.
// =============================================================================
PROTECTED VOID hal_TcuIrqSetHandler(HAL_TCU_IRQ_ID_T tcu, HAL_TCU_IRQ_HANDLER_T handler);


// =============================================================================
// hal_TcuIrqSetMask
// -----------------------------------------------------------------------------
/// Set the interruption mask for the specified TCU IRQ.
/// @param tcu Id of the TCU  IRQ
/// @mask If \c TRUE, the IRQ can be triggered. Oterwise, it can't occur.
// =============================================================================
PROTECTED VOID hal_TcuIrqSetMask(HAL_TCU_IRQ_ID_T tcu, BOOL mask);



// =============================================================================
// hal_TcuIrqGetMask
// -----------------------------------------------------------------------------
//  Gets the interrupt mask of a TCU  IRQ
/// @param tcu A TCU IRQ Id.
/// @return The mask
// =============================================================================
BOOL hal_TcuIrqGetMask(HAL_TCU_IRQ_ID_T tcu);


// =============================================================================
// hal_TcuIrqHandler
// -----------------------------------------------------------------------------
/// TCU IRQ handler called by the IRQ module
/// @param interruptId of the IRQ calling this handler
// =============================================================================
PROTECTED VOID hal_TcuIrqHandler(UINT8 interruptId);


#endif



