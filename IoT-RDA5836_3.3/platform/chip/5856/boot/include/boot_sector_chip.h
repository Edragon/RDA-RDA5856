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
/// @file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOT_SECTOR_CHIP_H_
#define _BOOT_SECTOR_CHIP_H_


#include "global_macros.h"
#ifdef CT_ASM
#include "mem_bridge_asm.h"
#else
#include "mem_bridge.h"
#endif


//=============================================================================
// BOOT_SECTOR_ENTRY_POINT
//-----------------------------------------------------------------------------
/// First address where to jump in flash, to execute code
//=============================================================================
#define BOOT_SECTOR_ENTRY_POINT (KSEG0(REG_CS0_BASE)+0x10)


//=============================================================================
// BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS
//-----------------------------------------------------------------------------
/// Address of the programmed flash pattern.
//=============================================================================
#define BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS ((UINT32*)KSEG0(REG_CS0_BASE))


#endif // _BOOT_SECTOR_CHIP_H_


