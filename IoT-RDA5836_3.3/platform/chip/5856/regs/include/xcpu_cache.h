/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _XCPU_CACHE_H_
#define _XCPU_CACHE_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'xcpu_cache'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// XCPU_CACHE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_XCPU_IDATA_BASE         0x01A1B000
#define REG_XCPU_DDATA_BASE         0x01A1C000

typedef volatile struct
{
    struct
    {
        /// The XCpu cache is accessible only when it is disabled or the cpu is stalled.
        /// 
        /// The bits "3:0" of the address select the Data in the line. 
        /// The bits "13:4" of the address select the line. 
        /// If the Tag at the corresponding line is valid, the bits "25:14" of the
        /// tag are the corresponding address bits.
        REG32                      Data[4];                      //0x00000000
    } Line[EXP2(8)];
} HWP_XCPU_CACHE_T;

#define hwp_xcpuIdata               ((HWP_XCPU_CACHE_T*) KSEG1(REG_XCPU_IDATA_BASE))
#define hwp_xcpuDdata               ((HWP_XCPU_CACHE_T*) KSEG1(REG_XCPU_DDATA_BASE))


//Data
#define XCPU_CACHE_DATA(n)          (((n)&0xFFFFFFFF)<<0)





#endif

