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
                                                                          
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _BOOT_MAP_H_
#define _BOOT_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================

// ============================================================================
// BOOT_MAIN_TAG_T
// -----------------------------------------------------------------------------
/// Magic tags used by the boot loader to switch between boot modes.
// =============================================================================
typedef enum
{
    BOOT_MAIN_TAG_NORMAL                        = 0x00000000,
    BOOT_MAIN_TAG_BREAKBOOT                     = 0xB4EAB007,
    BOOT_MAIN_TAG_CALIB                         = 0xCA1BCA1B
} BOOT_MAIN_TAG_T;


// ============================================================================
// BOOT_SECTOR_CMD_T
// -----------------------------------------------------------------------------
/// List of Boot Sector commands.
// =============================================================================
typedef enum
{
/// The Boot Sector must jump into the ROMed Boot Monitor.
    BOOT_SECTOR_CMD_ENTER_BOOT_MONITOR          = 0x4E6A821C
} BOOT_SECTOR_CMD_T;

/// Value written in the Boot Sector structure to say that the EBC configuration
/// present in this structure is valid and can be programmed. Used for the field
/// ebcConfigValidTag.
#define BOOT_SECTOR_EBC_VALID_TAG                (0XB0075EC7)
/// Value written in the Boot Sector structure to say that the EBC extension configuration
/// present in this structure is valid and can be programmed. Used for the field
/// ebcConfigValidTag.
#define BOOT_SECTOR_EBC_EXT_VALID_TAG            (0XEBC00E28)

#define BOOT_SECTOR_IOMUX_VALID_TAG              (0X10AD0E28)
// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BOOT_SECTOR_STRUCT_T
// -----------------------------------------------------------------------------
/// Structure type used by the Boot Sector to determine in which mode it must boot
/// and how the EBC RAM must be configured (this is useful in case of burst mode
/// RAM). This structure contains information written by HAL during the normal execution
/// of the code and used by the Boot Sector when the phone boots.
// =============================================================================
typedef struct
{
    BOOT_SECTOR_CMD_T              command;                      //0x00000000
    UINT32                         ebcConfigValidTag;            //0x00000004
    UINT32                         ebcConfigRamTimings;          //0x00000008
    UINT32                         ebcConfigRamMode;             //0x0000000C
    UINT32                         ebcExtConfigValidTag;         //0x00000010
    UINT32                         ebcFreq;                      //0x00000014
    UINT8                          ramId;                        //0x00000018
    UINT8                          reserved[2];                  //0x00000019
    UINT8                          ebcConfigVersion;             //0x0000001B
} BOOT_SECTOR_STRUCT_T; //Size : 0x1C



// ============================================================================
// BOOT_MAP_GLOBALS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32              reserved;
} BOOT_MAP_GLOBALS_T; //Size : 0x0





#endif

