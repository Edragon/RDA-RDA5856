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



#ifndef _BB_SRAM_H_
#define _BB_SRAM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bb_sram'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BB_SRAM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BB_SRAM_BASE            0x01980000

typedef volatile struct
{
    UINT8 ram_array[13312];                     //0x00000000
} HWP_BB_SRAM_T;

#define hwp_bbSram                  ((HWP_BB_SRAM_T*) KSEG1(REG_BB_SRAM_BASE))






#endif

