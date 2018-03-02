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



#ifndef _BIST_H_
#define _BIST_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bist'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BIST_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_SYS_BIST_BASE           0x01A1F000
#define REG_BB_BIST_BASE            0x01909000

typedef volatile struct
{
    REG32                          control;                      //0x00000000
    REG32                          status;                       //0x00000004
    REG32                          mem_addr;                     //0x00000008
    REG32                          mem_size;                     //0x0000000C
    REG32                          data_mask;                    //0x00000010
} HWP_BIST_T;

#define hwp_sysBist                 ((HWP_BIST_T*) KSEG1(REG_SYS_BIST_BASE))
#define hwp_bbBist                  ((HWP_BIST_T*) KSEG1(REG_BB_BIST_BASE))


//control
#define BIST_START                  (1<<0)

//status
#define BIST_ENABLE                 (1<<0)
#define BIST_FAIL                   (1<<4)

//mem_addr
#define BIST_MEM_ADDR(n)            (((n)&0x3FFFFFF)<<0)

//mem_size
#define BIST_MEM_SIZE(n)            (((n)&0xFFFF)<<0)
#define BIST_MEM_WIDTH(n)           (((n)&3)<<24)
#define BIST_MEM_WIDTH_MASK         (3<<24)
#define BIST_MEM_WIDTH_SHIFT        (24)
#define BIST_MEM_WIDTH_MEM_WIDTH_DEFAULT (0<<24)
#define BIST_MEM_WIDTH_MEM_WIDTH_8  (1<<24)
#define BIST_MEM_WIDTH_MEM_WIDTH_16 (2<<24)
#define BIST_MEM_WIDTH_MEM_WIDTH_32 (3<<24)

//data_mask
#define BIST_DATA_MASK(n)           (((n)&0xFFFFFFFF)<<0)





#endif

