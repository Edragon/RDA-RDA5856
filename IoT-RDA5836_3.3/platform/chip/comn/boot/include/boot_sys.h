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
/// @file boot_sys.h
/// That file describes the BOOT SYSTEM driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOT_SYS_H_
#define _BOOT_SYS_H_

#include "cs_types.h"
#include "hal_sys.h"


/// @defgroup system BOOT Sytem Driver
/// description
/// ...
/// @{



// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// ============================================================================
// hal_SysSetupPLL
// ----------------------------------------------------------------------------
/// Setup PLL at early system boot time.
// ============================================================================
PUBLIC VOID boot_SysSetupPLL(VOID);


// =============================================================================
// hal_SysGetChipId
// -----------------------------------------------------------------------------
/// That function gives the chip ID the software is running on.
/// @param part Which part of the chip ID
/// @return The requested part of the chip ID.
// =============================================================================
PUBLIC UINT32 boot_SysGetChipId(HAL_SYS_CHIP_ID_T part);



/// @} // end of the system group



#endif // _BOOT_SYS_H_

