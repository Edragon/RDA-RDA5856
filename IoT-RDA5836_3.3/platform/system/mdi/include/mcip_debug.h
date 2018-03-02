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
/// @file mcip_debug.h                                                          //
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hal_debug.h"
#include "sxs_io.h"

#ifndef _MCIP_DEBUG_H_
#define _MCIP_DEBUG_H_


// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define MCI_WARN_TRC       (_MCI | TLEVEL(1))
/// AUDIO specific trace
#define MCI_AUDIO_TRC        (_MCI | TLEVEL(2))
/// VIDEO specific trace
#define MCI_VIDEO_TRC       (_MCI | TLEVEL(3))
/// VIDEO RECORDER specific trace
#define MCI_VIDREC_TRC       (_MCI | TLEVEL(4))
/// VIDEO RECORDER specific trace
#define MCI_CAMERA_TRC       (_MCI | TLEVEL(5))
/// MALLOC specific trace 
#define MCI_MALLOC_TRC       (_MCI | TLEVEL(6))
/// AnalogTV specific trace 
#define MCI_ANALOGTV_TRC       (_MCI | TLEVEL(7))

// add more here
/// reserved for debug (can be very verbose, but should probably not stay in code)
#define MCI_DBG_TRC        (_MCI | TLEVEL(16))

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
// Conditional Printf Usage
#ifndef MCI_NO_TRACE
#define MCI_TRACE(level, tstmap, format, ...)  hal_DbgTrace(_MCI | level,tstmap,format, ##__VA_ARGS__)
#else
#define MCI_TRACE(level, tstmap, format, ...)  
#endif //MCI_NO_PRINTF



#ifdef MCI_NO_ASSERT
#define MCI_ASSERT(boolCondition, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is true, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define MCI_ASSERT(boolCondition, format, ...)                   \
    if ((boolCondition)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif





//#define diag_printf(format,...) MCI_TRACE(TSTDOUT | TLEVEL(1), 0, format, ##__VA_ARGS__)
#define diag_printf(format,...) MCI_TRACE(MCI_VIDEO_TRC, 0, format, ##__VA_ARGS__)

#define ass( boolCondition )     MCI_ASSERT(boolCondition, "pls view GDB")
#define assinfo( boolCondition, format,... )      MCI_ASSERT(boolCondition, format,##__VA_ARGS__)

#define convertaddr(x) (uint32 *)((uint32)(x) | 0xa0000000)



#endif

//#define malloc(x) (malloc(x) ? : ass(1)); 

