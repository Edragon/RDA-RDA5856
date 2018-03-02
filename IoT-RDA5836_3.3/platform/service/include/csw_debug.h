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
/// @file cswp_cfg.h                                                          //
/// That file provides assert, trace and profiling macros for use in          //
/// CSW implementation                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hal_debug.h"
#include "sxs_io.h"
#include "csw_profile_codes.h"
#include "chip_id.h"

#ifndef CSWP_CFG_H
#define CSWP_CFG_H



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
// =============================================================================
#ifdef CSW_PROFILING


// =============================================================================
//  CSW_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define CSW_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  CSW_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define CSW_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  CSW_PROFILE_WINDOW_ENTRY
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define CSW_PROFILE_WINDOW_ENTRY(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  CSW_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define CSW_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_CSW, (CP_ ## eventName))


// =============================================================================
//  CSW_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define CSW_PROFILE_PULSE(pulseName)  \
                hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_CSW, (CP_ ## pulseName))


#else // CSW_PROFILING

#define CSW_PROFILE_FUNCTION_ENTER(eventName)
#define CSW_PROFILE_FUNCTION_EXIT(eventName)
#define CSW_PROFILE_WINDOW_ENTRY(eventName)
#define CSW_PROFILE_WINDOW_EXIT(eventName)
#define CSW_PROFILE_PULSE(pulseName)

#endif // CSW_PROFILING

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define CSW_WARN_TRC        (_CSW | TLEVEL(1))
/// BAL specific trace
#define CSW_BAL_TRC         (_CSW | TLEVEL(2))
/// PM specific trace
#define CSW_PM_TRC          (_CSW | TLEVEL(3))
/// @todo add levels and use them !
// add more here
/// reserved for debug (can be very verbose, but should probably not stay in code)
#define CSW_DBG_TRC         (_CSW | TLEVEL(16))

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
// Conditional Printf Usage
/*
#ifndef CSW_NO_PRINTF
#define CSW_TRACE(level, tstmap, format, ...) \
     hal_DbgTrace(_CSW |(level),tstmap,format, ##__VA_ARGS__)
#else
#define CSW_TRACE(level, tsmap, fmt, ...)
#endif
*/

#endif // CSWP_CFG_H







