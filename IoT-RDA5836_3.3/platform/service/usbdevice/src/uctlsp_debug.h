/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
////////////////////////////////////////////////////////////////////////////////
//
/// @file mrsp_debug.h
///
/// Multimedia player service debug function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UCTLSP_DEBUG_H_
#define _UCTLSP_DEBUG_H_

#include "cs_types.h"

#include "hal_debug.h"

#include "sxs_io.h"

//#include "uctls_profile_codes.h"
//#include "svcp_debug.h"

// =============================================================================
// MACROS
// =============================================================================

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
// =============================================================================
#define UCTLS_WARN_TRC  0 // SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define UCTLS_INFO_TRC   0 //SVC_UCTLS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define UCTLS_DBG_TRC    0 //SVC_DBG_TRC



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
#ifndef UCTLS_NO_PRINTF
#define UCTLS_TRACE(level, tsmap, format, ...)   CSW_TRACE(level,format, ##__VA_ARGS__)
#else
#define UCTLS_TRACE(level, tsmap, fmt, ...)
#endif


#ifdef UCTLS_NO_ASSERT
#define UCTLS_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define UCTLS_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif

#ifdef UCTLS_PROFILING

// =============================================================================
//  UCTLS_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define UCTLS_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))

// =============================================================================
//  UCTLS_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define UCTLS_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))

// =============================================================================
//  UCTLS_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define UCTLS_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))

#else

#define UCTLS_PROFILE_FUNCTION_ENTER(eventName)
#define UCTLS_PROFILE_FUNCTION_EXIT(eventName)
#define UCTLS_PROFILE_PULSE(pulseName)

#endif /* UCTLS_PROFILING */

// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

#endif // _UCTLSP_DEBUG_H_
