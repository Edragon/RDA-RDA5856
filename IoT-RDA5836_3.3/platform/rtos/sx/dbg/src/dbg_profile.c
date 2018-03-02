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


////////////////////////////////////////////////////////////////////////////////
//
/// @file dbg_profile.c
/// Implement the profiling mechanisms. Please refer to the CoolProfile tool 
/// documentation for details.
//
////////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "chip_id.h"

#include "hal_debug.h"



// =============================================================================
// DBG_PXTS_LEVEL
// -----------------------------------------------------------------------------
/// When several levels are available for PXTS code, that one is used by 
/// DBG.
// =============================================================================
// FIXME Choose a value
#define DBG_PXTS_LEVEL  HAL_DBG_PXTS_MMI


// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
// =============================================================================

// =============================================================================
// dbg_PxtsSendTrigger
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code correponding to a pre-defined "trigger
/// tag". If the "PXTS Trigger" mode is enabled in the profiling tool, this
/// "trigger tag" will stop the PXTS recording. This can be used to stop the
/// PXTS recording from the embedded code.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsSendTrigger(VOID)
{
    hal_DbgPxtsSendTrigger(DBG_PXTS_LEVEL);
}



// =============================================================================
// dbg_PxtsProfileFunctionEnter
// -----------------------------------------------------------------------------
/// This function has to be called when entering the function you want to profile.
/// 
/// @param functionId Code representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileFunctionEnter(UINT16 functionId)
{
    hal_DbgPxtsProfileFunctionEnter(DBG_PXTS_LEVEL, functionId);
}



// =============================================================================
// dbg_PxtsProfileFunctionExit
// -----------------------------------------------------------------------------
/// This function has to be called when exiting the function you want to profile.
///
/// @param functionId Code representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileFunctionExit(UINT16 functionId)
{
    hal_DbgPxtsProfileFunctionExit(DBG_PXTS_LEVEL, functionId);
}



// =============================================================================
// dbg_PxtsProfileWindowEnter
// -----------------------------------------------------------------------------
/// This function has to be called to mark the entrance in a window.
/// 
/// @param windowId Code representing the window which has been entered in.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileWindowEnter(UINT16 windowId)
{
    hal_DbgPxtsProfileWindowEnter(DBG_PXTS_LEVEL, windowId);
}



// =============================================================================
// dbg_PxtsProfileWindowExit
// -----------------------------------------------------------------------------
/// This function has to be called to mark the exit of a window.
/// 
/// @param windowId Code representing the window which has been exited.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileWindowExit(UINT16 windowId)
{
    hal_DbgPxtsProfileWindowExit(DBG_PXTS_LEVEL, windowId);
}





// =============================================================================
// dbg_PxtsProfilePulse
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code correponding to a pulse.
/// 
/// @param code Code representing the pulse
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfilePulse(UINT16 code)
{
    hal_DbgPxtsProfilePulse(DBG_PXTS_LEVEL, code);
}

