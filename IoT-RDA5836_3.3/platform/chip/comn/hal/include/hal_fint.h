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
/// @file hal_fint.h                                                          //
/// Manage the fint irq: handler set, masking, etc                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_FINT_H_
#define _HAL_FINT_H_


/// @defgroup fint HAL Frame interrupt managing
///
/// @{


// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_FINT_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type of the user handler function
// =============================================================================
typedef  VOID (*HAL_FINT_IRQ_HANDLER_T)(VOID);


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_FintIrqSetHandler
// -----------------------------------------------------------------------------
/// Set the user function called in case of FINT
/// @param handler Handler function to set
// =============================================================================
PUBLIC VOID hal_FintIrqSetHandler(HAL_FINT_IRQ_HANDLER_T handler);

// =============================================================================
// hal_FintIrqSetMask
// -----------------------------------------------------------------------------
/// Set the mask for the FINT
/// @param mask Mask to set. If \c TRUE, FINT are enabled. If \c FALSE,
/// they don't occur.
// =============================================================================
PUBLIC VOID hal_FintIrqSetMask(BOOL mask);


// =============================================================================
// hal_FintIrqGetMask
// -----------------------------------------------------------------------------
/// This function is used to recover the FINT mask
/// @return The Fint mask: \c TRUE if FINT can happen, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_FintIrqGetMask(VOID);

// =============================================================================
// hal_FintIrqGetStatus
// -----------------------------------------------------------------------------
/// Check the Frame interrupt status.
/// @return \c TRUE if the FINT had occured \n
///         \c FALSE otherwise
// =============================================================================
PUBLIC BOOL hal_FintIrqGetStatus(VOID);


/// @} <-- End of the fint group

#endif // _HAL_FINT_H_


