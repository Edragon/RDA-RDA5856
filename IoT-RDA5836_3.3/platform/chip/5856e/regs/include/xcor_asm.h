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
#ifndef _XCOR_ASM_H_
#define _XCOR_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'xcor'."
#endif

#include "globals_asm.h"

#define XCOR_SYMBOL_SIZE                         (VITAC_MULT_SIZE)
#define XCOR_SADDER_SIZE                         (XCOR_SYMBOL_SIZE+1 +6)

//==============================================================================
// xcor
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_XCOR_BASE               0x01900000

#define REG_XCOR_BASE_HI            BASE_HI(REG_XCOR_BASE)
#define REG_XCOR_BASE_LO            BASE_LO(REG_XCOR_BASE)

#define REG_XCOR_COMMAND            REG_XCOR_BASE_LO + 0x00000000
#define REG_XCOR_STATUS             REG_XCOR_BASE_LO + 0x00000004
#define REG_XCOR_HV0                REG_XCOR_BASE_LO + 0x00000008
#define REG_XCOR_HV1                REG_XCOR_BASE_LO + 0x0000000C
#define REG_XCOR_DATA_0             REG_XCOR_BASE_LO + 0x00000010
#define REG_XCOR_DATA_1             REG_XCOR_BASE_LO + 0x00000014
#define REG_XCOR_DATA_2             REG_XCOR_BASE_LO + 0x00000018
#define REG_XCOR_DATA_3             REG_XCOR_BASE_LO + 0x0000001C
#define REG_XCOR_DATA_4             REG_XCOR_BASE_LO + 0x00000020
#define REG_XCOR_DATA_5             REG_XCOR_BASE_LO + 0x00000024
#define REG_XCOR_ACCUI              REG_XCOR_BASE_LO + 0x00000028
#define REG_XCOR_ACCUQ              REG_XCOR_BASE_LO + 0x0000002C
#define REG_XCOR_ADDR0              REG_XCOR_BASE_LO + 0x00000030

//command
#define XCOR_BITERR_EN              (1<<0)
#define XCOR_DCO1P_EN               (1<<1)
#define XCOR_DCO2P_EN               (1<<2)
#define XCOR_DCO3P_EN               (1<<3)
#define XCOR_TSCXC_EN               (1<<4)
#define XCOR_SREC_EN                (1<<5)
#define XCOR_BEXT_EN                (1<<6)
#define XCOR_SPROC_EN               (1<<7)
#define XCOR_CHEST_EN               (1<<8)
#define XCOR_FCHXC_EN               (1<<9)
#define XCOR_SLDWIN_EN              (1<<10)
#define XCOR_IT_MASK                (1<<11)
#define XCOR_PACK_IQ                (1<<12)
#define XCOR_DEROTATION_EN          (1<<13)
#define XCOR_NB_ILOOP(n)            (((n)&0xFF)<<16)
#define XCOR_NB_SYMB(n)             (((n)&0xFF)<<24)

//status
#define XCOR_OP_PENDING             (1<<0)
#define XCOR_IT_CAUSE               (1<<16)
#define XCOR_IT_STATUS              (1<<31)

//hv0
#define XCOR_HV0(n)                 (((n)&0xFFFFFFFF)<<0)

//hv1
#define XCOR_HV1(n)                 (((n)&0xFFFFFFFF)<<0)

//data
#define XCOR_DATA(n)                (((n)&0xFFFFFFFF)<<0)

//accuI
#define XCOR_ACCUI(n)               (((n)&0x1FFFFF)<<0)

//accuQ
#define XCOR_ACCUQ(n)               (((n)&0x1FFFFF)<<0)

//addr0
#define XCOR_ADDR0(n)               (((n)&0xFFF)<<2)




#endif
