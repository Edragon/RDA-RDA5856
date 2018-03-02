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
/// @file hal_comregs
/// This file describes the communication registers module driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"

#include "globals.h"
#include "global_macros.h"
#include "comregs.h"

#include "halp_irq.h"

#include "halp_debug.h"
#include "hal_comregs.h"


#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_ComregsSetReqReg
// -----------------------------------------------------------------------------
/// Set the comregs Req register
/// @param reg  Bitfield where each bit at '1' sets the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bit unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsSetReqReg(UINT32 reg)
{
    hwp_sysComregs->ReqReg_Set = reg;
}

// =============================================================================
// hal_ComregsGetReqReg
// -----------------------------------------------------------------------------
/// Get the comregs Req register.
/// @return the Comregs REQ register value.
// =============================================================================
PUBLIC UINT32 hal_ComregsGetReqReg(VOID)
{
    return hwp_sysComregs->ReqReg_Set;
}

// =============================================================================
// hal_ComregsClrReqReg
// -----------------------------------------------------------------------------
/// Clear the comregs Req register
/// @param reg Bitfield where each bit at '1' clears the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bits unchanged.
// =============================================================================
VOID hal_ComregsClrReqReg(UINT32 reg)
{
    hwp_sysComregs->ReqReg_Clr = reg;
}

// =============================================================================
// hal_ComregsSetAckReg
// -----------------------------------------------------------------------------
/// Set the comregs Acknowledge register
/// @param reg  Bitfield where each bit at '1' sets the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bit unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsSetAckReg(UINT32 reg)
{
    hwp_sysComregs->AckReg_Set = reg;
}

// =============================================================================
// hal_ComregsGetAckReg
// -----------------------------------------------------------------------------
/// Get the comregs Ack register.
/// @return the Comregs REQ register value.
// =============================================================================
PUBLIC UINT32 hal_ComregsGetAckReg(VOID)
{
    return hwp_sysComregs->AckReg_Set;
}

// =============================================================================
// hal_ComregsClrAckReg
// -----------------------------------------------------------------------------
/// Clear the comregs Ack register
/// @param reg Bitfield where each bit at '1' clears the 
/// corresponding bit in the set/clear register and bits at '0' 
/// leave the corresponding bits unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsClrAckReg(UINT32 reg)
{
    hwp_sysComregs->AckReg_Clr = reg;
}
#endif // CHIP_HAS_ASYNC_TCU

// =============================================================================
// hal_ComregsGetSnap
// -----------------------------------------------------------------------------
/// Get the comregs IT snapshot value
// =============================================================================
PUBLIC UINT32 HAL_FUNC_INTERNAL hal_ComregsGetSnap(VOID)
{
    return hwp_sysComregs->Snapshot;
}

// =============================================================================
// hal_ComregsSetSnapCfg
// -----------------------------------------------------------------------------
/// Set the snapshot configuration register
/// @param isTrio When \c TRUE, the snapshot wrap value is 3 \n
///                        \c FALSE, it is 2.
// =============================================================================
PUBLIC VOID hal_ComregsSetSnapCfg(BOOL isTrio)
{
#if (CHIP_HAS_ASYNC_BCPU)
    // No configuration anymore
#else // (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysComregs->Snapshot_Cfg = (isTrio?1:0);
#endif // (CHIP_HAS_ASYNC_BCPU)
}

//  IRQ FUNC 
/// Private array for storing user irq handler.
PRIVATE HAL_COMREGS_IRQ_HANDLER_T g_halComregsRegistry[HAL_COMREGS_XCPU_IRQ_SLOT_QTY]  = {NULL,NULL};


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
PUBLIC VOID hal_ComregsSetIrqHandler(HAL_COMREGS_XCPU_IRQ_SLOT_T slot,HAL_COMREGS_IRQ_HANDLER_T handler)
{
    HAL_ASSERT((slot >= 0 && slot < HAL_COMREGS_XCPU_IRQ_SLOT_QTY), "Mbx overslot %d", slot);
    g_halComregsRegistry[slot] = handler;
}


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
PUBLIC VOID HAL_FUNC_INTERNAL hal_ComregsSetMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->Mask_Set = COMREGS_X_IRQ0_MASK_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->Mask_Set = COMREGS_IRQ0_MASK_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->Mask_Set = COMREGS_X_IRQ1_MASK_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->Mask_Set = COMREGS_IRQ1_MASK_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
}

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
PUBLIC VOID HAL_FUNC_INTERNAL hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->Mask_Clr = COMREGS_X_IRQ0_MASK_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->Mask_Clr = COMREGS_IRQ0_MASK_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->Mask_Clr = COMREGS_X_IRQ1_MASK_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->Mask_Clr = COMREGS_IRQ1_MASK_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
}


// =============================================================================
// hal_ComregsGetMask
// -----------------------------------------------------------------------------
/// Get the mask of a slot
///
/// @param slot The slot whose masks is wanted
/// @return The value of the mask set for the slot given
/// as a parameter.
// ============================================================================= 
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_T slot)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;    
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->Mask_Set,COMREGS_X_IRQ0_MASK_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_sysComregs->Mask_Set,COMREGS_IRQ0_MASK_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->Mask_Set,COMREGS_X_IRQ1_MASK_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_sysComregs->Mask_Set,COMREGS_IRQ1_MASK_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }  
}


// PRIVATE FUNCTION !!!! (internal to hal)
// Discreminate around the slot and call the appropriate 
// previously defined handler. This is the function called
// by the irq hw module.
PROTECTED VOID hal_ComregsIrqHandler (UINT8 interruptId)
{
    UINT32 tmpRead;
    UINT32 status;

    union
    {
        UINT32 slotStatus;
        HAL_COMREGS_IRQ_STATUS_T returnedStatus;
    } u;

    if (interruptId == SYS_IRQ_COM0)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        status =  GET_BITFIELD(hwp_sysComregs->Cause, COMREGS_X_IRQ0_CAUSE);
        // Clear ITs
        hwp_sysComregs->ItReg_Clr = COMREGS_X_IRQ0_CAUSE(status);
#else // CHIP_HAS_ASYNC_TCU
        status =  GET_BITFIELD(hwp_sysComregs->Cause, COMREGS_IRQ0_CAUSE);
        // Clear ITs
        hwp_sysComregs->ItReg_Clr = COMREGS_IRQ0_CAUSE(status);
#endif // CHIP_HAS_ASYNC_TCU
        tmpRead = hwp_sysComregs->ItReg_Clr;

        // mask for the first slot
        u.slotStatus = status;
    
        if(g_halComregsRegistry[HAL_COMREGS_XCPU_IRQ_SLOT_0])
        {
            g_halComregsRegistry[HAL_COMREGS_XCPU_IRQ_SLOT_0](u.returnedStatus);
        }
    }
    else if (interruptId == SYS_IRQ_COM1)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        // interruptId  == SYS_IRQ_COM1
        status =  GET_BITFIELD(hwp_sysComregs->Cause, COMREGS_X_IRQ1_CAUSE);
        // Clear ITs
        hwp_sysComregs->ItReg_Clr = COMREGS_X_IRQ1_CAUSE(status);
#else // CHIP_HAS_ASYNC_TCU
        // interruptId  == SYS_IRQ_COM1
        status =  GET_BITFIELD(hwp_sysComregs->Cause, COMREGS_IRQ1_CAUSE);
        // Clear ITs
        hwp_sysComregs->ItReg_Clr = COMREGS_IRQ1_CAUSE(status);
#endif // CHIP_HAS_ASYNC_TCU
        tmpRead = hwp_sysComregs->ItReg_Clr;

        u.slotStatus = status;
        
        if(g_halComregsRegistry[HAL_COMREGS_XCPU_IRQ_SLOT_1])
        {
            g_halComregsRegistry[HAL_COMREGS_XCPU_IRQ_SLOT_1](u.returnedStatus);
        }
    }
    
    //If others, do others
    //...
}



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
PUBLIC VOID hal_ComregsBcpuIrqTrigger(HAL_COMREGS_BCPU_IRQ_SLOT_T slot,HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_BCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Set = COMREGS_B_IRQ0_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_bbComregs->ItReg_Set = COMREGS_IRQ0_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_BCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Set = COMREGS_B_IRQ1_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_bbComregs->ItReg_Set = COMREGS_IRQ1_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    } 
}

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
PUBLIC VOID hal_ComregsXcpuIrqTrigger(HAL_COMREGS_XCPU_IRQ_SLOT_T slot,HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Set = COMREGS_X_IRQ0_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->ItReg_Set = COMREGS_IRQ0_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Set = COMREGS_X_IRQ1_SET(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->ItReg_Set = COMREGS_IRQ1_SET(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    } 
}


// =============================================================================
// hal_ComregsBcpuIrqGetCauseStatus
// -----------------------------------------------------------------------------
/// Get cause of IRQ triggerd  on the BCPU
/// @param slot BCPU IRQ slot from which to get the cause.
/// @return The interrupt status of the given slot.
// =============================================================================
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsBcpuIrqGetCauseStatus(HAL_COMREGS_BCPU_IRQ_SLOT_T slot)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;    
    
    if(slot == HAL_COMREGS_BCPU_IRQ_SLOT_0)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_B_IRQ0_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_bbComregs->ItReg_Set,COMREGS_IRQ0_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_BCPU_IRQ_SLOT_1, "Improper mbx slot");
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_B_IRQ1_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_bbComregs->ItReg_Set,COMREGS_IRQ1_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }  
}


// =============================================================================
// hal_ComregsXcpuIrqGetCauseStatus
// -----------------------------------------------------------------------------
/// Get cause of IRQ triggerd  on the XCPU
/// @param slot BCPU IRQ slot from which to get the cause.
/// @return The interrupt status of the given slot.
// =============================================================================
PUBLIC HAL_COMREGS_IRQ_MASK_T hal_ComregsXcpuIrqGetCauseStatus(HAL_COMREGS_XCPU_IRQ_SLOT_T slot)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;    
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_X_IRQ0_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_IRQ0_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
#if (!CHIP_HAS_ASYNC_TCU)
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_X_IRQ1_SET);
#else // CHIP_HAS_ASYNC_TCU
        u.reg = GET_BITFIELD(hwp_sysComregs->ItReg_Set,COMREGS_IRQ1_SET);
#endif // CHIP_HAS_ASYNC_TCU
        return u.bitfield;
    }  
}


// =============================================================================
// hal_ComregsBcpuIrqClearCause
// -----------------------------------------------------------------------------
/// Clear
/// @param slot BCPU IRQ slot on which clear  the IRQ cause.
/// @param mask IRQ cause to clear, leaving the others unchanged.
// =============================================================================
PUBLIC VOID hal_ComregsBcpuIrqClearCause(HAL_COMREGS_BCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_BCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Clr = COMREGS_B_IRQ0_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_bbComregs->ItReg_Clr = COMREGS_IRQ0_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_BCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Clr = COMREGS_B_IRQ1_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_bbComregs->ItReg_Clr = COMREGS_IRQ1_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }  
}

// =============================================================================
// hal_ComregsXcpuIrqClearCause
// -----------------------------------------------------------------------------
/// Clear IRQs on the XCPU
///
/// @param slot XCPU IRQ slot on which clear  the IRQ cause.
/// @param mask IRQ cause to clear, leaving the others unchanged.
/// 
// =============================================================================
PUBLIC VOID hal_ComregsXcpuIrqClearCause(HAL_COMREGS_XCPU_IRQ_SLOT_T slot, HAL_COMREGS_IRQ_MASK_T mask)
{
    union
    {
        UINT32 reg;
        HAL_COMREGS_IRQ_MASK_T bitfield;
    } u;
    
    u.bitfield = mask;
    
    if(slot == HAL_COMREGS_XCPU_IRQ_SLOT_0)
    {
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Clr = COMREGS_X_IRQ0_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->ItReg_Clr = COMREGS_IRQ0_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }
    else
    {
        HAL_ASSERT(slot == HAL_COMREGS_XCPU_IRQ_SLOT_1, "Improper mbx slot");
        // built the mask as it should be in the hw register
#if (!CHIP_HAS_ASYNC_TCU)
        hwp_sysComregs->ItReg_Clr = COMREGS_X_IRQ1_CLR(u.reg);
#else // CHIP_HAS_ASYNC_TCU
        hwp_sysComregs->ItReg_Clr = COMREGS_IRQ1_CLR(u.reg);
#endif // CHIP_HAS_ASYNC_TCU
    }  
}

