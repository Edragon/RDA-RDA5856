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

#ifndef _BCPU_TAG_ASM_H_
#define _BCPU_TAG_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'bcpu_tag'."
#endif



//==============================================================================
// bcpu_tag
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_BCPU_TAG_BASE           0x0190B000

#define REG_BCPU_TAG_BASE_HI        BASE_HI(REG_BCPU_TAG_BASE)
#define REG_BCPU_TAG_BASE_LO        BASE_LO(REG_BCPU_TAG_BASE)

#define REG_BCPU_TAG_LINE_0         REG_BCPU_TAG_BASE_LO + 0x00000000
#define REG_BCPU_TAG_LINE_1         REG_BCPU_TAG_BASE_LO + 0x00000004
#define REG_BCPU_TAG_LINE_2         REG_BCPU_TAG_BASE_LO + 0x00000008
#define REG_BCPU_TAG_LINE_3         REG_BCPU_TAG_BASE_LO + 0x0000000C
#define REG_BCPU_TAG_LINE_4         REG_BCPU_TAG_BASE_LO + 0x00000010
#define REG_BCPU_TAG_LINE_5         REG_BCPU_TAG_BASE_LO + 0x00000014
#define REG_BCPU_TAG_LINE_6         REG_BCPU_TAG_BASE_LO + 0x00000018
#define REG_BCPU_TAG_LINE_7         REG_BCPU_TAG_BASE_LO + 0x0000001C
#define REG_BCPU_TAG_LINE_8         REG_BCPU_TAG_BASE_LO + 0x00000020
#define REG_BCPU_TAG_LINE_9         REG_BCPU_TAG_BASE_LO + 0x00000024
#define REG_BCPU_TAG_LINE_10        REG_BCPU_TAG_BASE_LO + 0x00000028
#define REG_BCPU_TAG_LINE_11        REG_BCPU_TAG_BASE_LO + 0x0000002C
#define REG_BCPU_TAG_LINE_12        REG_BCPU_TAG_BASE_LO + 0x00000030
#define REG_BCPU_TAG_LINE_13        REG_BCPU_TAG_BASE_LO + 0x00000034
#define REG_BCPU_TAG_LINE_14        REG_BCPU_TAG_BASE_LO + 0x00000038
#define REG_BCPU_TAG_LINE_15        REG_BCPU_TAG_BASE_LO + 0x0000003C
#define REG_BCPU_TAG_LINE_16        REG_BCPU_TAG_BASE_LO + 0x00000040
#define REG_BCPU_TAG_LINE_17        REG_BCPU_TAG_BASE_LO + 0x00000044
#define REG_BCPU_TAG_LINE_18        REG_BCPU_TAG_BASE_LO + 0x00000048
#define REG_BCPU_TAG_LINE_19        REG_BCPU_TAG_BASE_LO + 0x0000004C
#define REG_BCPU_TAG_LINE_20        REG_BCPU_TAG_BASE_LO + 0x00000050
#define REG_BCPU_TAG_LINE_21        REG_BCPU_TAG_BASE_LO + 0x00000054
#define REG_BCPU_TAG_LINE_22        REG_BCPU_TAG_BASE_LO + 0x00000058
#define REG_BCPU_TAG_LINE_23        REG_BCPU_TAG_BASE_LO + 0x0000005C
#define REG_BCPU_TAG_LINE_24        REG_BCPU_TAG_BASE_LO + 0x00000060
#define REG_BCPU_TAG_LINE_25        REG_BCPU_TAG_BASE_LO + 0x00000064
#define REG_BCPU_TAG_LINE_26        REG_BCPU_TAG_BASE_LO + 0x00000068
#define REG_BCPU_TAG_LINE_27        REG_BCPU_TAG_BASE_LO + 0x0000006C
#define REG_BCPU_TAG_LINE_28        REG_BCPU_TAG_BASE_LO + 0x00000070
#define REG_BCPU_TAG_LINE_29        REG_BCPU_TAG_BASE_LO + 0x00000074
#define REG_BCPU_TAG_LINE_30        REG_BCPU_TAG_BASE_LO + 0x00000078
#define REG_BCPU_TAG_LINE_31        REG_BCPU_TAG_BASE_LO + 0x0000007C
#define REG_BCPU_TAG_LINE_32        REG_BCPU_TAG_BASE_LO + 0x00000080
#define REG_BCPU_TAG_LINE_33        REG_BCPU_TAG_BASE_LO + 0x00000084
#define REG_BCPU_TAG_LINE_34        REG_BCPU_TAG_BASE_LO + 0x00000088
#define REG_BCPU_TAG_LINE_35        REG_BCPU_TAG_BASE_LO + 0x0000008C
#define REG_BCPU_TAG_LINE_36        REG_BCPU_TAG_BASE_LO + 0x00000090
#define REG_BCPU_TAG_LINE_37        REG_BCPU_TAG_BASE_LO + 0x00000094
#define REG_BCPU_TAG_LINE_38        REG_BCPU_TAG_BASE_LO + 0x00000098
#define REG_BCPU_TAG_LINE_39        REG_BCPU_TAG_BASE_LO + 0x0000009C
#define REG_BCPU_TAG_LINE_40        REG_BCPU_TAG_BASE_LO + 0x000000A0
#define REG_BCPU_TAG_LINE_41        REG_BCPU_TAG_BASE_LO + 0x000000A4
#define REG_BCPU_TAG_LINE_42        REG_BCPU_TAG_BASE_LO + 0x000000A8
#define REG_BCPU_TAG_LINE_43        REG_BCPU_TAG_BASE_LO + 0x000000AC
#define REG_BCPU_TAG_LINE_44        REG_BCPU_TAG_BASE_LO + 0x000000B0
#define REG_BCPU_TAG_LINE_45        REG_BCPU_TAG_BASE_LO + 0x000000B4
#define REG_BCPU_TAG_LINE_46        REG_BCPU_TAG_BASE_LO + 0x000000B8
#define REG_BCPU_TAG_LINE_47        REG_BCPU_TAG_BASE_LO + 0x000000BC
#define REG_BCPU_TAG_LINE_48        REG_BCPU_TAG_BASE_LO + 0x000000C0
#define REG_BCPU_TAG_LINE_49        REG_BCPU_TAG_BASE_LO + 0x000000C4
#define REG_BCPU_TAG_LINE_50        REG_BCPU_TAG_BASE_LO + 0x000000C8
#define REG_BCPU_TAG_LINE_51        REG_BCPU_TAG_BASE_LO + 0x000000CC
#define REG_BCPU_TAG_LINE_52        REG_BCPU_TAG_BASE_LO + 0x000000D0
#define REG_BCPU_TAG_LINE_53        REG_BCPU_TAG_BASE_LO + 0x000000D4
#define REG_BCPU_TAG_LINE_54        REG_BCPU_TAG_BASE_LO + 0x000000D8
#define REG_BCPU_TAG_LINE_55        REG_BCPU_TAG_BASE_LO + 0x000000DC
#define REG_BCPU_TAG_LINE_56        REG_BCPU_TAG_BASE_LO + 0x000000E0
#define REG_BCPU_TAG_LINE_57        REG_BCPU_TAG_BASE_LO + 0x000000E4
#define REG_BCPU_TAG_LINE_58        REG_BCPU_TAG_BASE_LO + 0x000000E8
#define REG_BCPU_TAG_LINE_59        REG_BCPU_TAG_BASE_LO + 0x000000EC
#define REG_BCPU_TAG_LINE_60        REG_BCPU_TAG_BASE_LO + 0x000000F0
#define REG_BCPU_TAG_LINE_61        REG_BCPU_TAG_BASE_LO + 0x000000F4
#define REG_BCPU_TAG_LINE_62        REG_BCPU_TAG_BASE_LO + 0x000000F8
#define REG_BCPU_TAG_LINE_63        REG_BCPU_TAG_BASE_LO + 0x000000FC

//Line
#define BCPU_TAG_TAG(n)             (((n)&0x7FFF)<<10)
#define BCPU_TAG_TAG_MASK           (0x7FFF<<10)
#define BCPU_TAG_TAG_SHIFT          (10)
#define BCPU_TAG_VALID              (1<<29)
#define BCPU_TAG_VALID_MASK         (1<<29)
#define BCPU_TAG_VALID_SHIFT        (29)




#endif
