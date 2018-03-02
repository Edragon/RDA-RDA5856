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
//                                                                            //
/// @file pmdp_debug.h                                                        //
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _PMDP_DEBUG_H_
#define  _PMDP_DEBUG_H_

#include "hal_debug.h"
#include "pmd_profile_codes.h"
#include "sxs_io.h"
#include "edrvp_debug.h"

// =============================================================================
//  TRACE Level mapping
// =============================================================================
/// Important trace that can be enabled all the time (low rate)
#define PMD_WARN_TRC EDRV_WARN_TRC
/// Informational trace interesting for PMD only
#define PMD_INFO_TRC EDRV_PMD_TRC
/// reserved for debug (can be very verbose, but should probably not stay in code)
#define PMD_DBG_TRC EDRV_DBG_TRC

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
#ifndef PMD_NO_PRINTF
#define PMD_TRACE(level, tstmap, format, ...)  EDRV_TRACE(level,tstmap,format, ##__VA_ARGS__)
#else
#define PMD_TRACE(level, tsmap, fmt, ...)
#endif



#ifdef PMD_NO_ASSERT
#define PMD_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#ifndef EDRV_NO_TRACE
#define PMD_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#else
#define PMD_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert("View GDB");                             \
    }
#endif
#endif


// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
// =============================================================================
#ifdef PMD_PROFILING


// =============================================================================
//  PMD_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define PMD_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_EDRV, (CP_ ## pulseName))


// =============================================================================
//  PMD_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define PMD_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  PMD_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define PMD_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  PMD_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define PMD_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


// =============================================================================
//  PMD_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define PMD_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_EDRV, (CP_ ## eventName))


#else // PMD_PROFILING

#define PMD_PROFILE_FUNCTION_ENTER(eventName)
#define PMD_PROFILE_FUNCTION_EXIT(eventName)
#define PMD_PROFILE_WINDOW_ENTER(eventName)
#define PMD_PROFILE_WINDOW_EXIT(eventName)
#define PMD_PROFILE_PULSE(pulseName)

#endif // PMD_PROFILING



#endif //_PMDP_DEBUG_H_


