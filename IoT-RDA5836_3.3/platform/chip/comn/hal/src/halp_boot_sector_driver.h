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
//
/// @file halp_boot_sector_driver.h
///
/// HAL Boot Sector driver.
/// 
/// Provides functions to allow HAL EBC and HAL Host Monitor to write some
/// informations in the Boot Sector structure and to validate this structure.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_BOOT_SECTOR_DRIVER_H_
#define _HALP_BOOT_SECTOR_DRIVER_H_

#include "cs_types.h"

#include "chip_id.h"

#include "boot_sector.h"
#include "hal_sys.h"

// =============================================================================
// FUNCTIONS                                                            
// =============================================================================


// =============================================================================
// hal_BootSectorSetCommand
// -----------------------------------------------------------------------------
/// Set the command which must be executed by the Boot Sector.
/// Then validate the Boot Sector structure.
/// 
/// @param valid Command used during the next boot.
// =============================================================================
PROTECTED VOID hal_BootSectorSetCommand(BOOT_SECTOR_CMD_T command);


// =============================================================================
// hal_BootSectorSetEbcConfig
// -----------------------------------------------------------------------------
/// Set the configuration of the EBC for the RAM chip select. This
/// configuration will be written by the Boot Sector code to be sure the
/// RAM configured properly after a reset (in this case, the RAM chip
/// might not get the system reset). Without this, a RAM configured in burst
/// mode could be in an unknown state.
/// 
/// Then validate the EBC configuration and the Boot Sector structure.
/// 
/// This function is *forced* inline because it will be used by HAL and by
/// the Boot Sector and the linkage of HAL and Boot Sector can be independent.
/// 
/// @param timings EBC RAM chip select timing configuration as defined by
/// the EBC CS_Time_Write register.
/// @param mode EBC RAM chip select mode configuration as defined by the
/// EBC CS_Mode register.
// =============================================================================
PROTECTED VOID hal_BootSectorSetEbcConfig(UINT32 timings, UINT32 mode);


// =============================================================================
// hal_BootSectorSetEbcFreq
// -----------------------------------------------------------------------------
/// Save EBC frequency for the RAM chip select. This
/// configuration will be written by the Boot Sector code to be sure the
/// RAM configured properly after a reset (in this case, the RAM chip
/// might not get the system reset). Without this, a RAM configured in burst
/// mode could be in an unknown state.
/// 
/// Then validate the EBC extension configuration.
/// 
/// @param ebcFreq the EBC frequency.
// =============================================================================
PROTECTED VOID hal_BootSectorSetEbcFreq(HAL_SYS_MEM_FREQ_T ebcFreq);


// =============================================================================
// hal_BootSectorGetEbcFreq
// -----------------------------------------------------------------------------
/// Get the EBC frequency in the EBC extension configuration.
/// 
/// @return the EBC frequency.
// =============================================================================
PROTECTED HAL_SYS_MEM_FREQ_T hal_BootSectorGetEbcFreq(VOID);



#endif // _HALP_BOOT_SECTOR_DRIVER_H_
