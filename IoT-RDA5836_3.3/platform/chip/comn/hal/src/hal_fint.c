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
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_fint.c
/// Manage the fint irq: handler set, masking, etc
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "global_macros.h"
#include "sys_irq.h"

#include "hal_fint.h"
#include "hal_mem_map.h"
#include "halp_fint.h"
#include "halp_irq.h"
#include "halp_sys.h"
#include "halp_lps.h"



#define PULSE_ID_2_BITMASK(id) (1<<((id)-SYS_IRQ_TCU0))

// Variable to store the user handler
PRIVATE HAL_FINT_IRQ_HANDLER_T g_fintRegistry = NULL;


// =============================================================================
// hal_FintIrqSetHandler
// -----------------------------------------------------------------------------
/// Set the user function called in case of FINT
/// @param handler Handler function to set
// =============================================================================
PUBLIC VOID hal_FintIrqSetHandler(HAL_FINT_IRQ_HANDLER_T handler)
{
    g_fintRegistry = handler;
}

// =============================================================================
// hal_FintIrqSetMask
// -----------------------------------------------------------------------------
/// Set the mask for the FINT
/// @param mask Mask to set. If \c TRUE, FINT are enabled. If \c FALSE,
/// they don't occur.
// =============================================================================
PUBLIC VOID hal_FintIrqSetMask(BOOL mask)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
    if (mask)
    {
        hwp_sysIrq->pulse_mask_set = SYS_IRQ_SYS_IRQ_FRAME;
    }
    else
    {
        hwp_sysIrq->pulse_mask_clr = SYS_IRQ_SYS_IRQ_FRAME;
    }
#endif
}

// =============================================================================
// hal_FintIrqGetMask
// -----------------------------------------------------------------------------
/// This function is used to recover the FINT mask
/// @return The Fint mask: \c TRUE if FINT can happen, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_FintIrqGetMask(VOID)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
    if (hwp_sysIrq->pulse_mask_clr & SYS_IRQ_SYS_IRQ_FRAME)
    {
        return TRUE; // mask set
    }
    else 
    {
        return FALSE; // mask not set
    }
#endif
    return true;
}

// =============================================================================
// hal_FintIrqGetStatus
// -----------------------------------------------------------------------------
/// Check the Frame interrupt status.
/// @return \c TRUE if the FINT had occured \n
///         \c FALSE otherwise
// =============================================================================
PUBLIC BOOL HAL_FUNC_INTERNAL hal_FintIrqGetStatus(VOID)
{
    if (hwp_sysIrq->status & SYS_IRQ_SYS_IRQ_FRAME)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// =============================================================================
// hal_FintIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
PROTECTED VOID HAL_FUNC_INTERNAL hal_FintIrqHandler(UINT8 interruptId)
{
    UINT32 tmpRead;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
    hwp_sysIrq->pulse_clear = PULSE_ID_2_BITMASK(interruptId);
    tmpRead = hwp_sysIrq->pulse_clear;
#endif    

#if (!CHIP_HAS_ASYNC_TCU)
    // TODO somewhere there: do a frequency change there if
    // one was attempted during g_halLpsCtx.lpsSkipFrameNumber != HAL_LPS_UNDEF_VALUE
    UINT32 status=hal_SysEnterCriticalSection();

    if (g_halSysLpsLock != HAL_SYS_LPS_LOCK_UNLOCKED)
    {
        g_halSysLpsLock&=~HAL_SYS_LPS_LOCK_FINT;
        if(g_halSysLpsLock == HAL_SYS_LPS_LOCK_UNLOCKED)
        {
            hal_SysUpdateSystemFrequency();
        }
    }
    hal_SysExitCriticalSection(status);
#endif

    // Call user handler
    hal_FintUserHandler();
}

// =============================================================================
// hal_FintUserHandler
// -----------------------------------------------------------------------------
/// called by hal_FintIrqHandler()
// =============================================================================
PROTECTED VOID HAL_FUNC_INTERNAL hal_FintUserHandler(VOID)
{
    // Call user handler
    if (g_fintRegistry)
        g_fintRegistry();
}

