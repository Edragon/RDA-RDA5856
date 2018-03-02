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
/// @file audp_debug.h                                                          //
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hal_debug.h"
#include "sxs_io.h"
#include "aud_profile_codes.h"
#include "edrvp_debug.h"


#ifndef _AUDP_DEBUG_H_
#define _AUDP_DEBUG_H_

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define AUD_WARN_TRC EDRV_WARN_TRC
/// Informational trace interesting for PMD only
#define AUD_INFO_TRC EDRV_AUD_TRC
/// reserved for debug (can be very verbose, but should probably not stay in code)
#define AUD_DBG_TRC EDRV_DBG_TRC

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
// Conditional Printf Usage
#ifndef AUD_NO_PRINTF
#define AUD_TRACE(level, tstmap, format, ...)  EDRV_TRACE(level,tstmap,format, ##__VA_ARGS__)
#else
#define AUD_TRACE(level, tstmap, format, ...)  
#endif //AUD_NO_PRINTF



#ifdef AUD_NO_ASSERT
#define AUD_ASSERT(boolCondition, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#ifndef EDRV_NO_TRACE
#define AUD_ASSERT(boolCondition, format, ...)                   \
    if (!(boolCondition)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#else
#define AUD_ASSERT(boolCondition, format, ...)                   \
    if (!(boolCondition)) {            \
        hal_DbgAssert("View GDB");                             \
    }

#endif
#endif



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with hal_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef AUD_PROFILING


// =============================================================================
//  AUD_PROFILE_FUNCTION_ENTRY
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define AUD_PROFILE_FUNCTION_ENTRY(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  AUD_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define AUD_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  AUD_PROFILE_WINDOW_ENTRY
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define AUD_PROFILE_WINDOW_ENTRY(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  AUD_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define AUD_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  AUD_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define AUD_PROFILE_PULSE(pulseName)  \
                hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_EDRV, (CP_ ## pulseName))


#else // AUD_PROFILING

#define AUD_PROFILE_FUNCTION_ENTRY(eventName)
#define AUD_PROFILE_FUNCTION_EXIT(eventName)
#define AUD_PROFILE_WINDOW_ENTRY(eventName)
#define AUD_PROFILE_WINDOW_EXIT(eventName)
#define AUD_PROFILE_PULSE(pulseName)

#endif // AUD_PROFILING



#endif // _AUDP_DEBUG_H_







