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
/// @file bootp_host.h
///
/// Boot host protected function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_HOST_H_
#define _BOOTP_HOST_H_

#include "cs_types.h"
#include "boot.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

PROTECTED VOID mon_Event(UINT32 evt);

PROTECTED VOID boot_HstMonitorX(VOID);

PROTECTED BOOT_MONITOR_OP_STATUS_T boot_HstMonitor(VOID);

#endif // _BOOTP_HOST_H_
