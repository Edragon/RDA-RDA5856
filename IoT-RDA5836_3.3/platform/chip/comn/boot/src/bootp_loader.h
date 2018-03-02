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
/// @file bootp_loader.h
///
/// Private declaration of boot_loader
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_LOADER_H_
#define _BOOTP_LOADER_H_

#include "cs_types.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


/// Value of the event sent by the Boot Loader when it starts the main() of
/// the code.
#define BOOT_LOADER_EVENT_START_CODE 0x00000057


/// Value of the event sent by the Boot Loader when the version of the ROM of
/// the chip mismatch the version of the ROM against which the code is linked.
#define BOOT_LOADER_EVENT_ROM_ERROR 0x00000054


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================
// =============================================================================
//  boot_LoaderEnter
// -----------------------------------------------------------------------------
/// The parameter "param" has a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID boot_LoaderEnter(UINT32 param);

#endif // _BOOTP_LOADER_H_



