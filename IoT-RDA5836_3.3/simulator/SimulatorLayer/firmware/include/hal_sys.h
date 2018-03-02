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
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_sys.h
/// That file describes the SYSTEM driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_SYS_H_
#define _HAL_SYS_H_

#include "cs_types.h"


// =============================================================================
// HAL_SYS_CHIP_ID_T
// -----------------------------------------------------------------------------
/// Describes the cause of a system reset. This type is returned by the function
/// #hal_SysGetResetCause.
// =============================================================================
typedef enum
{
    /// Chip product ID
    HAL_SYS_CHIP_PROD_ID,

    /// Chip bond ID
    HAL_SYS_CHIP_BOND_ID,

    /// Chip metal ID
    HAL_SYS_CHIP_METAL_ID,

    /// Chip ID whole
    HAL_SYS_CHIP_ID_WHOLE,

} HAL_SYS_CHIP_ID_T;


UINT32 hal_SysEnterCriticalSection(VOID);

VOID hal_SysExitCriticalSection(UINT32 status);



#endif // _HAL_SYS_H_

