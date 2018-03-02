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
/// @file mcip_debug.h
/// Debug features to be used only inside the module (protected).
/// If this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//
////////////////////////////////////////////////////////////////////////////////

#ifndef  _MCIP_DEBUG_H_
#define  _MCIP_DEBUG_H_

#include "hal_debug.h"
//#include "mci_profile_codes.h"
#include "sxs_io.h"



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement, replacing level by _HAL or something
// =============================================================================
// TODO replace TSTDOUT by an ID
#define MCI_LCD_TRC_LVL TLEVEL(1)
#define MCI_CAM_TRC_LVL TLEVEL(2)
#define MCI_VID_TRC_LVL TLEVEL(3)
#define MCI_AUD_TRC_LVL TLEVEL(4)
#define MCI_USB_TRC_LVL TLEVEL(5)

#ifdef MCI_PRINTF
#define MCI_TRACE(level, tstmap, format, ...) \
     hal_DbgTrace(level,tstmap,format, ##__VA_ARGS__)
#else
#define MCI_TRACE(level, tsmap, fmt, ...)
#endif



#ifndef MCI_NO_ASSERT
#define MCI_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false,
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define MCI_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with hal_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef MCI_PROFILING


// =============================================================================
//  MCI_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define MCI_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_CSW, (CP_ ## pulseName))


// =============================================================================
//  MCI_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define MCI_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  MCI_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define MCI_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  MCI_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define MCI_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  MCI_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define MCI_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


#else // MCI_PROFILING

#define MCI_PROFILE_FUNCTION_ENTER(eventName)
#define MCI_PROFILE_FUNCTION_EXIT(eventName)
#define MCI_PROFILE_WINDOW_ENTER(eventName)
#define MCI_PROFILE_WINDOW_EXIT(eventName)
#define MCI_PROFILE_PULSE(pulseName)

#endif // MCI_PROFILING



#endif //_MCIP_DEBUG_H_


