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
/// @file bootp_sector.h
///
/// Boot sector variable
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_SECTOR_H_
#define _BOOTP_SECTOR_H_

#include "cs_types.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================


/// Value of the event sent by the Boot Sector when it configures the EBC
/// CS RAM (useful for RAM in burst mode, after a soft reset).
#define BOOT_SECTOR_EVENT_CONFIG_RAM    0x00000055


/// /// Value of the event sent by the Boot Sector when it detects that it
/// must enter in the boot monitor.
#define BOOT_SECTOR_EVENT_ENTER_MONITOR 0x00000056


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


// =============================================================================
// boot_SectorEnterBootMonitor
// -----------------------------------------------------------------------------
/// From the Boot Sector, enter in the Boot Monitor.
/// And, depending on the chip, make the modifications required to have a
/// fully working monitor.
/// This function is implemented in "boot/CT_ASIC/src/boot_sector_chip.c".
// =============================================================================
PROTECTED VOID boot_SectorEnterBootMonitor(VOID);

// =============================================================================
// boot_SectorFixRom
// -----------------------------------------------------------------------------
/// Use to fix some startup missing feature (like usb clock on greenstone)
/// This function is implemented in "boot/CT_ASIC/src/boot_sector_chip.c".
// =============================================================================
PROTECTED VOID boot_SectorFixRom(VOID);


#endif // _BOOTP_SECTOR_H_
