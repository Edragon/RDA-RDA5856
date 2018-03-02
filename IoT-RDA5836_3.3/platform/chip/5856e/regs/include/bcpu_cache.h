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


#ifndef _BCPU_CACHE_H_
#define _BCPU_CACHE_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bcpu_cache'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BCPU_CACHE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BCPU_IDATA_BASE         0x0190C000
#define REG_BCPU_DDATA_BASE         0x0190D000

typedef volatile struct
{
    struct
    {
        /// The BCpu cache is accessible only when it is disabled or the cpu is stalled.
        /// 
        /// The bits "9:4" of the address select the line. 
        /// If the Tag at the corresponding line is valid, the bits "24:10" of the
        /// tag are the corresponding address bits. 
        /// The bits "3:0" of the address select the Data in the line.
        REG32                      Data[4];                      //0x00000000
    } Line[EXP2(6)];
} HWP_BCPU_CACHE_T;

#define hwp_bcpuIdata               ((HWP_BCPU_CACHE_T*) KSEG1(REG_BCPU_IDATA_BASE))
#define hwp_bcpuDdata               ((HWP_BCPU_CACHE_T*) KSEG1(REG_BCPU_DDATA_BASE))


//Data
#define BCPU_CACHE_DATA(n)          (((n)&0xFFFFFFFF)<<0)





#endif

