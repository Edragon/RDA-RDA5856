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
/// @file csw_ver.c
/// Implement the functions returning the 
//
////////////////////////////////////////////////////////////////////////////////


#include "chip_id.h"
#include "cs_types.h"
#include "csw_ver.h"

#include "hal_map_engine.h"



// =============================================================================
// GLOBAL VARIABLES
// =============================================================================



// =============================================================================
// g_cswMmiMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of the MMI, when the MMI had filled it with the
/// function csw_VerSetMmiVersion().
// =============================================================================
PROTECTED HAL_MAP_VERSION_T g_cswMmiMapVersion;



// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
//csw_VerGetRevision
// -----------------------------------------------------------------------------
/// Return the revision of a given module.
/// 
/// @param  moduleId Id the module.
/// @return The SVN revision of that version of the module.
// =============================================================================
PUBLIC UINT32 platform_VerGetRevision(void)
{
    return 0x00;//SVN_REVISION;
}


// =============================================================================
//csw_VerGetNumber
// -----------------------------------------------------------------------------
/// Return the version number of a given module.
/// 
/// @param  moduleId Id the module.
/// @return The version number of that version of the module.
// =============================================================================
PUBLIC UINT32 platform_VerGetNumber(void)
{
    return 0x0201;  // 2.1
}


// =============================================================================
//csw_VerGetDate
// -----------------------------------------------------------------------------
/// Return the date of build of a given module.
/// The format is 0xYYYYMMDD.
/// 
/// @param  moduleId Id the module.
/// @return The build date of that version of the module.
// =============================================================================
PUBLIC UINT32 platform_VerGetDate(void)
{
    return BUILD_DATE;
}



