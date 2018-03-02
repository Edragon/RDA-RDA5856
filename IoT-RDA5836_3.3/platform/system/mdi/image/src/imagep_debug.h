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
/// @file imagep_debug.h
/// Debug features to be used only inside the module (protected).
/// If this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//
////////////////////////////////////////////////////////////////////////////////

#ifndef  _IMAGEP_DEBUG_H_
#define  _IMAGEP_DEBUG_H_

#ifdef MMI_ON_HARDWARE_P
#include "hal_debug.h"
#endif

// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
// =============================================================================
//#define IMAGE_PROFILING
#ifdef IMAGE_PROFILING

#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000
// =============================================================================
//  HAL_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define IMGDP_ENTRY(id)         hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, (id) & CPMASK)

// =============================================================================
//  HAL_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define IMGDP_EXIT(id)          hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, (id) & CPMASK));

#else // IMAGE_PROFILING

#define IMGDP_ENTRY(id)
#define IMGDP_EXIT(id)

#endif // IMAGE_PROFILING



#endif //_IMAGEP_DEBUG_H_


