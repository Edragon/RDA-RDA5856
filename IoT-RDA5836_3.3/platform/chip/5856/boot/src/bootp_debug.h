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
/// @file bootp_debug.h
///
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_DEBUG_H_
#define _BOOTP_DEBUG_H_

#include "cs_types.h"

#include "debug_port.h"

#include "boot_profile_codes.h"

#include "hal_debug.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================

#define CPEXITFLAG 0x8000

// =============================================================================
//  BOOT_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define BOOT_PROFILE_PULSE(pulseName)                                   \
do                                                                      \
{                                                                       \
    hwp_debugPort->Pxts_tag[HAL_DBG_PXTS_BOOT] = (CP_ ## pulseName);    \
}                                                                       \
while(0)

// =============================================================================
//  BOOT_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define BOOT_PROFILE_FUNCTION_ENTER(pulseName)                          \
do                                                                      \
{                                                                       \
    hwp_debugPort->Pxts_tag[HAL_DBG_PXTS_BOOT] = (CP_ ## pulseName);    \
}                                                                       \
while(0)

// =============================================================================
//  BOOT_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define BOOT_PROFILE_FUNCTION_EXIT(pulseName)                           \
do                                                                      \
{                                                                       \
    hwp_debugPort->Pxts_tag[HAL_DBG_PXTS_BOOT] = (CP_ ## pulseName | CPEXITFLAG);\
}                                                                       \
while(0)


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


#endif // _BOOTP_DEBUG_H_
