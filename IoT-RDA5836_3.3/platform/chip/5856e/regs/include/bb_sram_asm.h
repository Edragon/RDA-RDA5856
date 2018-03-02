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

#ifndef _BB_SRAM_ASM_H_
#define _BB_SRAM_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'bb_sram'."
#endif

#include "globals_asm.h"


//==============================================================================
// bb_sram
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_BB_SRAM_BASE            0x01980000

#define REG_BB_SRAM_BASE_HI         BASE_HI(REG_BB_SRAM_BASE)
#define REG_BB_SRAM_BASE_LO         BASE_LO(REG_BB_SRAM_BASE)


//==============================================================================
// bt_sram
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_BT_SRAM_BASE            0x01c80000

#define REG_BT_SRAM_BASE_HI         BASE_HI(REG_BT_SRAM_BASE)
#define REG_BT_SRAM_BASE_LO         BASE_LO(REG_BT_SRAM_BASE)


//==============================================================================
// dualport_sram
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_DUALPORT_SRAM_BASE            0x01d00000

#define REG_DUALPORT_SRAM_BASE_HI         BASE_HI(REG_DUALPORT_SRAM_BASE)
#define REG_DUALPORT_SRAM_BASE_LO         BASE_LO(REG_DUALPORT_SRAM_BASE)


#endif
