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
/// @file hal_tcu.c                                                           //
/// Implementation of the  TCU Driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "chip_id.h"
#include "global_macros.h"
#include "tcu.h"
#include "sys_irq.h"
#include "register.h"

#include "hal_tcu.h"
#include "hal_timers.h"
#include "hal_mem_map.h"
#include "hal_lps.h"

#include "halp_tcu.h"
#include "halp_debug.h"

#include "sxr_tls.h"

// =============================================================================
//  MACROS
// =============================================================================
#undef HAL_FUNC_INTERNAL
#define HAL_FUNC_INTERNAL


// FORCE_ALL is clearer, and forcelatch_mask has
// a bit to 1 for each latch ...
#define FORCE_ALL TCU_FORCELATCH_MASK

// could be an array of conversion, using the fact that the order respect the hardware
#define PULSE_ID_2_BITMASK(id) (1<<((id)-SYS_IRQ_TCU0))


extern BOOL COS_SleepByTick (UINT32 ticks);
extern PUBLIC UINT32 pmd_ConfigureRfLowPower(BOOL xcvUseVRF, BOOL xcvLowPowerKeepVRFOn);
// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Pointer in the event table
PRIVATE UINT8 HAL_DATA_INTERNAL g_halTcuEvtPtr;

/// Variable to store the user handler
PRIVATE HAL_TCU_IRQ_HANDLER_T HAL_DATA_INTERNAL g_tcuRegistry[2] 
         = {NULL, NULL};




// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_TcuInit
// -----------------------------------------------------------------------------
/// Initialize the TCU
/// This function opens the TCU and configures the TCO polarities.
/// @param tcoPolarity Bitfield where '0' means the corresponding. Only the
/// lowest 17 bits are relevant.
/// TCO output is active high and '1' that it is active low
// =============================================================================
PUBLIC VOID hal_TcuOpen(UINT32 tcoPolarity)
{
    hwp_tcu->Setup = tcoPolarity;
}



// =============================================================================
// hal_TcuStart
// -----------------------------------------------------------------------------
/// Start the TCU with an initial Value
/// @param value Value loaded in the TCU counter when it starts.
// =============================================================================
PUBLIC VOID hal_TcuStart(UINT16 value)
{
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
	return;
#endif
    hwp_tcu->Latch = TCU_CLEARPROGAREA;
    hwp_tcu->Latch = FORCE_ALL;
    // Normal behaviour for divider and latch (those bits are 0)
    hwp_tcu->Ctrl = TCU_LOAD_VAL(value) | TCU_ENABLE_ENABLED | TCU_LOAD;

}



// =============================================================================
// hal_TcuStop
// -----------------------------------------------------------------------------
/// Stops the TCU
// =============================================================================
PUBLIC VOID hal_TcuStop(VOID)
{
    hwp_tcu->Latch = TCU_CLEARPROGAREA;
    hwp_tcu->Latch = FORCE_ALL;
    hwp_tcu->Ctrl = TCU_ENABLE_DISABLED;
}



// =============================================================================
// hal_TcuSetTco
// -----------------------------------------------------------------------------
/// This function sets the corresponding TCO line to its
/// active level (which depends on TCO's polarity)
/// @param tcoNumber : index of the TCO to be set
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_TcuSetTco(UINT8 tcoNumber)
{
    hwp_tcu->Set_TCO = (1 << tcoNumber);
}



// =============================================================================
// hal_TcuClrTco
// -----------------------------------------------------------------------------
/// This function sets the corresponding TCO line to its
/// inactive level (which depends on TCO's polarity)
/// @param tcoNumber : index of the TCO to be reset
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_TcuClrTco(UINT8 tcoNumber)
{
    hwp_tcu->Clr_TCO = (1 << tcoNumber);
}


// =============================================================================
// hal_TcuPulseTco
// -----------------------------------------------------------------------------
/// This function  must only be called when the TCU timings are not available.
/// Send a pulse on a TCO.
/// @param tcoNumber : index of the TCO.
/// @param time Time in number of 16384Hz tick between the rise and the fall
// =============================================================================
PUBLIC VOID hal_TcuPulseTco(UINT8 tcoNumber, UINT16 time)
{
    hwp_tcu->Set_TCO = (1 << tcoNumber);
    // Wait for the pulse duration
    extern VOID hal_GetK1(UINT32* k1);
    UINT32 k1 = 1;
    hal_GetK1(&k1);
    if (k1 != 0)
    { // Interrupt or startup context
        hal_TimDelay(time);
    }
    else
    { // Task context
       COS_SleepByTick(time);
    }
    hwp_tcu->Clr_TCO = (1 << tcoNumber);
}


// =============================================================================
// hal_TcuGetCount
// -----------------------------------------------------------------------------
/// Get the TCU counter
/// Only the less significant 14 bits are 
/// relevant.
/// @return The TCU counter
// =============================================================================
PUBLIC UINT32 hal_TcuGetCount(VOID)
{
    return (hwp_tcu->Cur_Val);
}


// =============================================================================
// hal_TcuSetWrap
// -----------------------------------------------------------------------------
/// Set the wrap value
/// It is the time when the programming table becomes
/// the active table. The counter's next value is 0.
/// Only the less significant 14 bits are 
/// relevant to set the counter.
/// @param value Wrap Value
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_TcuSetWrap(UINT32 value)
{
    HAL_ASSERT((value<16384), "0 < TCU Val (= %d) <= 16384 ...", value);
    hwp_tcu->Wrap_Val = value;
}



// =============================================================================
// hal_TcuSetEvent
// -----------------------------------------------------------------------------
/// Set a TCU event.
/// This function handles the increment of TCU's event
/// pointer.
/// @param evId Event id, that is the event to set.
/// @param time Time when the foresaid event is to be
/// triggered.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_TcuSetEvent(HAL_TCU_EVENT_ID_T evId, UINT16 time)
{
    HAL_ASSERT((g_halTcuEvtPtr<NB_TCU_PROG_EVENTS), "No more TCU events");
    hwp_tcu->Event[g_halTcuEvtPtr++] = TCU_EVENT_TIME(time) | TCU_EVENT_ID(evId);
}


// =============================================================================
// hal_TcuClrEvtRange
// -----------------------------------------------------------------------------
/// Remove a range of events.
///
/// @param first first event to be removed.
/// @param last last event to be removed.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_TcuClrEvtRange(UINT8 first, UINT8 last)
{
    UINT32 i;

    for (i=first; i<=last; i++)
    {
        hwp_tcu->Event[i] = TCU_EVENT_TIME(0) | TCU_EVENT_ID(HAL_TCU_NO_EVENT);
    }
}

// =============================================================================
// hal_TcuClrAllEvents
// -----------------------------------------------------------------------------
/// Remove all active and programmed events
///
// =============================================================================
PUBLIC VOID hal_TcuClrAllEvents(VOID)
{
    hwp_tcu->Latch = TCU_CLEARPROGAREA;
    hwp_tcu->Latch = FORCE_ALL;
}


// =============================================================================
// hal_TcuSetEvtPtr
// -----------------------------------------------------------------------------
/// Set TCU's event pointer 
/// @param tcuPtr new pointer position.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL  hal_TcuSetEvtPtr(UINT8 tcuPtr)
{
    g_halTcuEvtPtr = tcuPtr;
}



// =============================================================================
// hal_TcuGetEvtPtr
// -----------------------------------------------------------------------------
/// Get current value of TCU's event pointer.
/// @return The current value of the TCU's event pointer.
// =============================================================================
PUBLIC UINT8 HAL_FUNC_INTERNAL  hal_TcuGetEvtPtr(VOID)
{
    return (g_halTcuEvtPtr);
}


// ----------------------- IRQ HANDLER ---------------------------

// =============================================================================
// hal_TcuIrqSetHandler
// -----------------------------------------------------------------------------
/// Set the user function to call when a tcu interrupt occurs.
/// @tcu Choose the TCU interrupt that will call this user handler.
/// @handler Handler called by this TCU interrupt.
// =============================================================================
PROTECTED VOID hal_TcuIrqSetHandler(HAL_TCU_IRQ_ID_T tcu, HAL_TCU_IRQ_HANDLER_T handler)
{
    if (tcu == HAL_TCU0_IRQ)
    {
        g_tcuRegistry[0] = handler;
    }
    else
    {
        g_tcuRegistry[1] = handler;
    }
}


// =============================================================================
// hal_TcuIrqSetMask
// -----------------------------------------------------------------------------
/// Set the interruption mask for the specified TCU IRQ.
/// @param tcu Id of the TCU  IRQ
/// @mask If \c TRUE, the IRQ can be triggered. Oterwise, it can't occur.
// =============================================================================
PROTECTED VOID hal_TcuIrqSetMask(HAL_TCU_IRQ_ID_T tcu, BOOL mask)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)    
    if (tcu == HAL_TCU0_IRQ)
    {
        if (mask)
        {
            hwp_sysIrq->pulse_mask_set = SYS_IRQ_SYS_IRQ_TCU0;
        }
        else
        {
            hwp_sysIrq->pulse_mask_clr = SYS_IRQ_SYS_IRQ_TCU0;
        }
    }
    else
    {
        if (mask)
        {
            hwp_sysIrq->pulse_mask_set = SYS_IRQ_SYS_IRQ_TCU1;
        }
        else
        {
            hwp_sysIrq->pulse_mask_clr = SYS_IRQ_SYS_IRQ_TCU1;
        }
    }
#endif
}



// =============================================================================
// hal_TcuIrqGetMask
// -----------------------------------------------------------------------------
//  Gets the interrupt mask of a TCU  IRQ
/// @param tcu A TCU IRQ Id.
/// @return The mask
// =============================================================================
BOOL hal_TcuIrqGetMask(HAL_TCU_IRQ_ID_T tcu)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    if (tcu == HAL_TCU0_IRQ)
    {
        if (hwp_sysIrq->pulse_mask_clr & SYS_IRQ_SYS_IRQ_TCU0)
        {
            return TRUE; // mask set
        }
        else 
        {
            return FALSE; // mask not set
        }
    }
    else
    // TCU1
    {
        if (hwp_sysIrq->pulse_mask_clr & SYS_IRQ_SYS_IRQ_TCU1)
        {
            return TRUE; // mask set
        }
        else 
        {
            return FALSE; // mask not set
        }
    }        
#endif

    return FALSE; // mask not set
    
}



// =============================================================================
// hal_TcuIrqHandler
// -----------------------------------------------------------------------------
/// TCU IRQ handler called by the IRQ module
/// @param interruptId of the IRQ calling this handler
// =============================================================================
#define TCUIRQ_FUNC_LOC
#if defined(RFSPI_IFC_WORKAROUND) && defined(PAL_WINDOWS_LOCK_MCP_ON_RX)
#undef TCUIRQ_FUNC_LOC
#define TCUIRQ_FUNC_LOC HAL_FUNC_INTERNAL
#endif // RFSPI_IFC_WORKAROUND && PAL_WINDOWS_LOCK_MCP_ON_RX

PROTECTED VOID TCUIRQ_FUNC_LOC hal_TcuIrqHandler(UINT8 interruptId)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    // Clear the interrupt
    UINT32 tmpRead;
    hwp_sysIrq->pulse_clear = PULSE_ID_2_BITMASK(interruptId);
    tmpRead = hwp_sysIrq->pulse_clear;

    // Call user handler
    if (interruptId == SYS_IRQ_TCU0)
    {
        if (g_tcuRegistry[0])
        {
            g_tcuRegistry[0]();
        }
    }

    if (interruptId == SYS_IRQ_TCU1)
    {
        if (g_tcuRegistry[1])
        {
            g_tcuRegistry[1]();
        }
    }
#endif
}

PUBLIC VOID hal_TcuInit(void)
{
    HAL_LPS_CONFIG_T lpsCfg;
    union
    {
        UINT32 reg;
        HAL_LPS_POWERUP_CTRL_T bitfield;
    } lpsPuCtrl;
    HAL_LPS_POWERUP_TIMINGS_T lpsPuTimings;

    // TCU  & LPS Inits
    hal_TcuOpen(0);
    hal_TcuSetWrap(4999);

    // TODO : pass all xcv's PU parameters -> add missing params to RFD API
    lpsPuTimings.puCoState1ToCoState2 = 0;
    lpsPuTimings.puCoState2ToRfClkEnState = 1;
    lpsPuTimings.puRfClkEnStateToRfClkReadyState = 10;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    lpsPuTimings.puRfClkReadyStateToPllClockReadyState = 1;
#else // 8808 or later
    lpsPuTimings.puRfClkReadyStateToPllClockReadyState = 31;
#endif
#ifdef USE_DIG_CLK_ONLY_AT_IDLE
    lpsPuTimings.puWaitPllLock = 0;
#else
    lpsPuTimings.puWaitPllLock = 1;
#endif

    // LPSCO0 power down state and polarity
    // TODO move this to PMD as Opal is not using LPSCO0 for VRF
    lpsPuCtrl.reg = pmd_ConfigureRfLowPower(FALSE, FALSE);
    // default power down state
    lpsPuCtrl.bitfield.puClkRfEnForceOn = 0;
    lpsPuCtrl.bitfield.puSpiForceOn = 0;
    // default polarity
    lpsPuCtrl.bitfield.puClkRfEnPol = 0;
    // power up actions
    lpsPuCtrl.bitfield.puSpiOnState = 0;
    lpsPuCtrl.bitfield.puCoActivationMaskInCoState1 = 1;
    lpsPuCtrl.bitfield.puCoActivationMaskInCoState2 = 3;
    lpsPuCtrl.bitfield.puMaskRfClkEn = 1;
    lpsPuCtrl.bitfield.puMaskPllEn = 1;
    lpsPuCtrl.bitfield.puMaskSwitchToRfClk = 1;
    lpsPuCtrl.bitfield.puMaskSwitchToPllClk = 1;
    lpsPuCtrl.bitfield.puEnableTcuWakeup = 0;

#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
    lpsPuTimings.puWaitPllLock = 0;
    lpsPuCtrl.bitfield.puMaskPllEn = 0;
    lpsPuCtrl.bitfield.puMaskSwitchToPllClk = 0;
#endif
    // PAL should only set this
    lpsCfg.lpsWakeUpFrameNbr = 0;
    lpsCfg.puTimings = lpsPuTimings;
    lpsCfg.puCtrl = lpsPuCtrl.bitfield;
    lpsCfg.lpsAccuracy = 16; // Max accuracy
    hal_LpsOpen(&lpsCfg);
}

