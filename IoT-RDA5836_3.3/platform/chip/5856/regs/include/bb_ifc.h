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



#ifndef _BB_IFC_H_
#define _BB_IFC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bb_ifc'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================
#define BB_IFC_ADDR_LEN                          (15)
#define BB_IFC_ADDR_ALIGN                        (2)
#define BB_IFC_TC_LEN                            (8)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BB_IFC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BB_IFC_BASE             0x01901000

typedef volatile struct
{
    /// The Channel 0 conveys data from the AIF to the memory.
    ///  The Channel 1 conveys data from the memory to the AIF. 
    /// These Channels only exist with Voice Option.
    struct
    {
        REG32                      control;                      //0x00000000
        REG32                      status;                       //0x00000004
        REG32                      start_addr;                   //0x00000008
        REG32                      Fifo_Size;                    //0x0000000C
        REG32 Reserved_00000010;                //0x00000010
        REG32                      int_mask;                     //0x00000014
        REG32                      int_clear;                    //0x00000018
        REG32                      cur_ahb_addr;                 //0x0000001C
    } ch[2];
} HWP_BB_IFC_T;

#define hwp_bbIfc                   ((HWP_BB_IFC_T*) KSEG1(REG_BB_IFC_BASE))


//control
#define BB_IFC_ENABLE               (1<<0)
#define BB_IFC_DISABLE              (1<<1)
#define BB_IFC_AUTO_DISABLE         (1<<4)

//status
//#define BB_IFC_ENABLE             (1<<0)
#define BB_IFC_FIFO_EMPTY           (1<<4)
#define BB_IFC_CAUSE_IEF            (1<<8)
#define BB_IFC_CAUSE_IHF            (1<<9)
#define BB_IFC_CAUSE_I4F            (1<<10)
#define BB_IFC_CAUSE_I3_4F          (1<<11)
#define BB_IFC_IEF                  (1<<16)
#define BB_IFC_IHF                  (1<<17)
#define BB_IFC_I4F                  (1<<18)
#define BB_IFC_I3_4F                (1<<19)

//start_addr
#define BB_IFC_START_ADDR(n)        (((n)&0xFFFFFF)<<2)

//Fifo_Size
#define BB_IFC_FIFO_SIZE(n)         (((n)&0x7FF)<<4)

//int_mask
#define BB_IFC_END_FIFO             (1<<8)
#define BB_IFC_HALF_FIFO            (1<<9)
#define BB_IFC_QUARTER_FIFO         (1<<10)
#define BB_IFC_THREE_QUARTER_FIFO   (1<<11)

//int_clear
//#define BB_IFC_END_FIFO           (1<<8)
//#define BB_IFC_HALF_FIFO          (1<<9)
//#define BB_IFC_QUARTER_FIFO       (1<<10)
//#define BB_IFC_THREE_QUARTER_FIFO (1<<11)

//cur_ahb_addr
#define BB_IFC_CUR_AHB_ADDR(n)      (((n)&0xFFFFFF)<<2)


#endif

