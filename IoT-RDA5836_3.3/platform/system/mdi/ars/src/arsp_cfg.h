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
/// @file playsp_cfg.h                                                          //
/// That file provides the trace, assert and profiling macros for the         //
//  PLAYS module.                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_debug.h"
/// @todo include the profile codes here
#include "sxs_io.h"
#include "ars_profile_codes.h"

#ifndef _ARSP_CFG_H_
#define _ARSP_CFG_H_



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement, replacing level by _HAL or something
// =============================================================================
#ifndef ARS_NO_PRINTF
#define ARS_TRACE(level, tsmap, format, ...)  hal_DbgTrace(level,tsmap,format, ##__VA_ARGS__)
#else
#define ARS_TRACE(level, tsmap, fmt, ...)
#endif



#ifdef ARS_NO_ASSERT
#define ARS_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define ARS_ASSERT(BOOL, format, ...)                   \
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
#ifdef ARS_PROFILING


// =============================================================================
//  ARS_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define ARS_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))


// =============================================================================
//  ARS_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define ARS_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  ARS_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define ARS_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  ARS_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define ARS_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  ARS_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define ARS_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


#else // ARS_PROFILING

#define ARS_PROFILE_FUNCTION_ENTER(eventName)
#define ARS_PROFILE_FUNCTION_EXIT(eventName)
#define ARS_PROFILE_WINDOW_ENTER(eventName)
#define ARS_PROFILE_WINDOW_EXIT(eventName)
#define ARS_PROFILE_PULSE(pulseName)

#endif // ARS_PROFILING



#endif // _ARSP_CFG_H_

