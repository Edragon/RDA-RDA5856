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



#ifndef _BCPU_TAG_H_
#define _BCPU_TAG_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bcpu_tag'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BCPU_TAG_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BCPU_TAG_BASE           0x0190B000

typedef volatile struct
{
    /// The BCpu tags are accessible only when the corresponding cache is disabled
    /// or the cpu is stalled. 
    /// The bits "9:4" of the address select the line
    REG32                          Line[EXP2(6)];                //0x00000000
} HWP_BCPU_TAG_T;

#define hwp_bcpuTag                 ((HWP_BCPU_TAG_T*) KSEG1(REG_BCPU_TAG_BASE))


//Line
#define BCPU_TAG_TAG(n)             (((n)&0x7FFF)<<10)
#define BCPU_TAG_TAG_MASK           (0x7FFF<<10)
#define BCPU_TAG_TAG_SHIFT          (10)
#define BCPU_TAG_VALID              (1<<29)
#define BCPU_TAG_VALID_MASK         (1<<29)
#define BCPU_TAG_VALID_SHIFT        (29)





#endif

