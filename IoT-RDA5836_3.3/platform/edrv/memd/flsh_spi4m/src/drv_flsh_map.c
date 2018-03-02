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
/// @file drv_flsh_map.c                                                      //
/// That file provides the flah mapping structures.                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// Bank and sector mapping of the flash chip.

#include "cs_types.h"
#include "memd_m.h"


// =============================================================================
//  MACROS
// =============================================================================
#define KBYTES  1024
#define MBYTES  (1024*KBYTES)

// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS 1
#define BANK0_BASE      0

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS]=
{
    { BANK0_BASE, { {128 , 4*KBYTES}, {0 , 0*KBYTES} } }
};
PUBLIC MEMD_FLASH_LAYOUT_T g_memdFlashLayout;

PUBLIC VOID memd_FlashSetLayout()
{
    g_memdFlashLayout.numberOfBank = NUMBER_OF_BANKS;
    g_memdFlashLayout.bankLayout = g_memdFlashBankLayout;
    g_memdFlashLayout.fullSize = FLASH_SIZE;
    g_memdFlashLayout.userDataBase = USER_DATA_BASE;
    g_memdFlashLayout.userDataSize = USER_DATA_SIZE;
    g_memdFlashLayout.userDataSectorLogicalBlockSize = USER_DATA_BLOCK_SIZE;
}


