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
/// @file hal_comregs.h
/// This file describes the communication registers module driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HAL_COMREGS_H_
#define  _HAL_COMREGS_H_

#include "cs_types.h"

/// @defgroup comregs HAL Comregs
/// This document describes the characteristics of  the Comregs module
/// and how to use it via its HAL API.
/// 
/// @{
/// 

// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
// HAL_COMREGS_XCPU_IRQ_SLOT_T
// -----------------------------------------------------------------------------
///     The comregs provides two irq 'slots' from the bcpu to the  
///     xcpu. The user can set up independent functions to 
///     handle irq occuring on each one of them. \n
///     Their status is 4 bits wide.
///     They are on different IRQ lines, and therefore different priorities
// =============================================================================
typedef enum
{
    /// Prioritary slot
    HAL_COMREGS_XCPU_IRQ_SLOT_0 = 0,
    /// Secondary slot
    HAL_COMREGS_XCPU_IRQ_SLOT_1,
    HAL_COMREGS_XCPU_IRQ_SLOT_QTY
} HAL_COMREGS_XCPU_IRQ_SLOT_T;


// =============================================================================
// HAL_COMREGS_BCPU_IRQ_SLOT_T
// -----------------------------------------------------------------------------
///     The comregs provides two irq 'slots' from the xcpu to the  
///     bcpu. The user can set up independent functions to 
///     handle irq occuring on each one of them. \n
///     Their status is 4 bits wide.
///     They are on different IRQ lines, and therefore different priorities
// =============================================================================
typedef enum
{
    /// Prioritary slot
    HAL_COMREGS_BCPU_IRQ_SLOT_0 = 0,
    /// Secondary slot
    HAL_COMREGS_BCPU_IRQ_SLOT_1,
    HAL_COMREGS_BCPU_IRQ_SLOT_QTY
} HAL_COMREGS_BCPU_IRQ_SLOT_T;



// =============================================================================
// HAL_COMREGS_IRQ_MASK_T
// -----------------------------------------------------------------------------
/// This structures defines a generic mask for each one of the comregs slot.
// =============================================================================
typedef struct
{
#if (!CHIP_HAS_ASYNC_TCU)
    UINT32 mask:4;
#else
    UINT32 mask:8;
#endif
} HAL_COMREGS_IRQ_MASK_T;

typedef struct
{
#if (!CHIP_HAS_ASYNC_TCU)
    UINT32 status:4;
#else
    UINT32 status:8;
#endif
} HAL_COMREGS_IRQ_STATUS_T;

// =============================================================================
// HAL_COMREGS_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// This is a generic type for a user irq handler, called when one slot 
/// of the comregs trigs an IRQ.
// =============================================================================
/// Type of the user handler function
typedef  void (*HAL_COMREGS_IRQ_HANDLER_T)(HAL_COMREGS_IRQ_STATUS_T);




// =============================================================================
//  FUNCTIONS 
// =============================================================================

// =============================================================================
// hal_ComregsGetSnap
// -----------------------------------------------------------------------------
/// Get the comregs IT snapshot value
// =============================================================================
PUBLIC UINT32 hal_ComregsGetSnap(VOID);


// =============================================================================
// hal_ComregsSetSnapCfg
// -----------------------------------------------------------------------------
/// Set the snapshot configuration register
/// @param isTrio When \c TRUE, the snapshot wrap value is 3 \n
///                        \c FALSE, it is 2.
// =============================================================================
PUBLIC VOID hal_ComregsSetSnapCfg(BOOL isTrio);


#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_ComregsSetReqReg
// -----------------------------------------------------------------------------
/// Set the comregs Req register
/// @param reg  Bitfield where each bit at '1' sets the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bit unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsSetReqReg(UINT32 reg);

// =============================================================================
// hal_ComregsGetReqReg
// -----------------------------------------------------------------------------
/// Get the comregs Req register.
/// @return the Comregs REQ register value.
// =============================================================================
PUBLIC UINT32 hal_ComregsGetReqReg(VOID);


// =============================================================================
// hal_ComregsClrReqReg
// -----------------------------------------------------------------------------
/// Clear the comregs Req register
/// @param reg Bitfield where each bit at '1' clears the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bits unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsClrReqReg(UINT32 reg);


// =============================================================================
// hal_ComregsSetAckReg
// -----------------------------------------------------------------------------
/// Set the comregs Acknowledge register
/// @param reg  Bitfield where each bit at '1' sets the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bit unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsSetAckReg(UINT32 reg);




// =============================================================================
// hal_ComregsGetAckReg
// -----------------------------------------------------------------------------
/// Get the comregs Ack register.
/// @return the Comregs REQ register value.
// =============================================================================
PUBLIC UINT32 hal_ComregsGetAckReg(VOID);



// =============================================================================
// hal_ComregsClrAckReg
// -----------------------------------------------------------------------------
/// Clear the comregs Ack register
/// @param reg Bitfield where each bit at '1' clears the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bits unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsClrAckReg(UINT32 reg);
#endif // !CHIP_HAS_ASYNC_TCU



// IRQ handling functions
// =============================================================================
// hal_ComregsSetIrqHandler
// -----------------------------------------------------------------------------
/// Set an function called when an irq occurs 
/// on the corresponding slot
/// 
/// @param slot The slot to associate an handler with
/// @param handler The handler function
/// 
// =============================================================================
PUBLIC VOID hal_ComregsSetIrqHandler(HAL_COMREGS_XCPU_IRQ_SLOT_T slot,
                               HAL_COMREGS_IRQ_HANDLER_T handler);

// =============================================================================
// hal_ComregsSetMask
// -----------------------------------------------------------------------------
/// Set the mask of a slot
///
/// The width is the one specified for each slot (4 bits here).
/// Only the lowest bits are used. This mask operate on a 
/// set-clear basis. This is the set function
///
/// @param slot Slot whose mask is to be set
/// @param mask Mask to set. Bits to '1' will be set. The ones to 
/// '0' remain unchanged
// =============================================================================
PUBLIC VOID hal_ComregsSetMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask);



// =============================================================================
// hal_ComregsClrMask
// -----------------------------------------------------------------------------
/// Clear the mask of a slot
///
/// The width is the one specified for each slot (4 bits here).
/// Only the lowest bits are used. This mask operate on a 
/// set-clear basis. This is the clear function
///
/// @param slot Slot whose mask is to be set
/// @param mask Mask to set. Bits to '1' will be cleared. The ones to 
/// '0' remain unchanged
// =============================================================================
PUBLIC VOID hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask);


// =============================================================================
// hal_ComregsGetMask
// -----------------------------------------------------------------------------
/// Get the mask of a slot
///
/// @param slot The slot whose masks is wanted
/// @return The value of the mask set for the slot given
/// as a parameter.
// ============================================================================= 
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot);





// =============================================================================
// hal_ComregsBcpuIrqTrigger
// -----------------------------------------------------------------------------
/// Trigger IRQs on the BCPU
///
/// @param slot BCPU IRQ slot on which trigger the IRQ.
/// @param mask IRQ cause sent to the BPCU
/// leave the corresponding bit unchanged.
/// 
// =============================================================================
PUBLIC VOID hal_ComregsBcpuIrqTrigger(HAL_COMREGS_BCPU_IRQ_SLOT_T slot,
                                        HAL_COMREGS_IRQ_MASK_T mask);

// =============================================================================
// hal_ComregsXcpuIrqTrigger
// -----------------------------------------------------------------------------
/// Trigger IRQs on the XCPU
///
/// @param slot XCPU IRQ slot on which trigger the IRQ.
/// @param mask IRQ cause sent to the BPCU
/// leave the corresponding bit unchanged.
/// 
// =============================================================================
PUBLIC VOID hal_ComregsXcpuIrqTrigger(HAL_COMREGS_XCPU_IRQ_SLOT_T slot,
                                        HAL_COMREGS_IRQ_MASK_T mask);





// =============================================================================
// hal_ComregsBcpuIrqGetCauseStatus
// -----------------------------------------------------------------------------
/// Get cause of IRQ triggerd  on the BCPU
/// @param slot BCPU IRQ slot from which to get the cause.
/// @return The interrupt status of the given slot.
// =============================================================================
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsBcpuIrqGetCauseStatus(HAL_COMREGS_BCPU_IRQ_SLOT_T slot);


// =============================================================================
// hal_ComregsXcpuIrqGetCauseStatus
// -----------------------------------------------------------------------------
/// Get cause of IRQ triggerd  on the XCPU
/// @param slot BCPU IRQ slot from which to get the cause.
/// @return The interrupt status of the given slot.
// =============================================================================
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsXcpuIrqGetCauseStatus(HAL_COMREGS_XCPU_IRQ_SLOT_T slot);




// =============================================================================
// hal_ComregsBcpuIrqClearCause
// -----------------------------------------------------------------------------
/// Clear
/// @param slot BCPU IRQ slot on which clear  the IRQ cause.
/// @param mask IRQ cause to clear, leaving the others unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsBcpuIrqClearCause(HAL_COMREGS_BCPU_IRQ_SLOT_T slot,
                                        HAL_COMREGS_IRQ_MASK_T mask);

// =============================================================================
// hal_ComregsXcpuIrqClearCause
// -----------------------------------------------------------------------------
/// Clear IRQs on the XCPU
///
/// @param slot XCPU IRQ slot on which clear  the IRQ cause.
/// @param mask IRQ cause to clear, leaving the others unchanged.
/// 
// =============================================================================
PUBLIC VOID hal_ComregsXcpuIrqClearCause(HAL_COMREGS_XCPU_IRQ_SLOT_T slot,
                                        HAL_COMREGS_IRQ_MASK_T mask);




/// @}

#endif // _HAL_COMREGS_H_




