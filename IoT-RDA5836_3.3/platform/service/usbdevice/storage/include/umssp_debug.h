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

#ifndef _UMSSP_DEBUG_H_
#define _UMSSP_DEBUG_H_

#include "cs_types.h"
//#include "svcp_debug.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
// =============================================================================
#define UMSS_WARN_TRC  5 // SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define UMSS_INFO_TRC   5 //SVC_UMSS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define UMSS_DBG_TRC    5 //SVC_DBG_TRC

#ifndef UMSS_NO_PRINTF
  #define UMSS_TRACE(level, tsmap, format, ...)   CSW_TRACE(level,format, ##__VA_ARGS__)
#else
  #define UMSS_TRACE(level, tsmap, fmt, ...)
#endif // UMSS_NO_PRINTF

// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


#endif // _UMSSP_DEBUG_H_
