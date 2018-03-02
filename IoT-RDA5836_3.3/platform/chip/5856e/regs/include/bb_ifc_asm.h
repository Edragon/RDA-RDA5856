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

                  
//------------------------------------------------------------------------
// File name      : bb_ifc_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:12:53                                                  
// Last modified  : 2016-09-29 11:12:53                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:12:53 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __BB_IFC_ASM_H__
#define __BB_IFC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ch_0__control              REG_BB_IFC_BASE+0x0 
#define REG_ch_0__status               REG_BB_IFC_BASE+0x4 
#define REG_ch_0__start_addr           REG_BB_IFC_BASE+0x8 
#define REG_ch_0__fifo_size            REG_BB_IFC_BASE+0xc 
#define REG_ch_0__reserved             REG_BB_IFC_BASE+0x10
#define REG_ch_0__int_mask             REG_BB_IFC_BASE+0x14
#define REG_ch_0__int_clear            REG_BB_IFC_BASE+0x18
#define REG_ch_0__cur_ahb_addr         REG_BB_IFC_BASE+0x1c
#define REG_ch_1__control              REG_BB_IFC_BASE+0x20
#define REG_ch_1__status               REG_BB_IFC_BASE+0x24
#define REG_ch_1__start_addr           REG_BB_IFC_BASE+0x28
#define REG_ch_1__fifo_size            REG_BB_IFC_BASE+0x2c
#define REG_ch_1__reserved             REG_BB_IFC_BASE+0x30
#define REG_ch_1__int_mask             REG_BB_IFC_BASE+0x34
#define REG_ch_1__int_clear            REG_BB_IFC_BASE+0x38
#define REG_ch_1__cur_ahb_addr         REG_BB_IFC_BASE+0x3c
#define REG_ch_2__control              REG_BB_IFC_BASE+0x40
#define REG_ch_2__status               REG_BB_IFC_BASE+0x44
#define REG_ch_2__start_addr           REG_BB_IFC_BASE+0x48
#define REG_ch_2__fifo_size            REG_BB_IFC_BASE+0x4c
#define REG_ch_2__reserved             REG_BB_IFC_BASE+0x50
#define REG_ch_2__int_mask             REG_BB_IFC_BASE+0x54
#define REG_ch_2__int_clear            REG_BB_IFC_BASE+0x58
#define REG_ch_2__cur_ahb_addr         REG_BB_IFC_BASE+0x5c

//ch_0__control
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_DISABLE                 (1<<1)              
#define BB_IFC_AUTO_DISABLE            (1<<4)              

//ch_0__status
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_FIFO_EMPTY              (1<<4)              
#define BB_IFC_CAUSE_IEF               (1<<8)              
#define BB_IFC_CAUSE_IHF               (1<<9)              
#define BB_IFC_CAUSE_I4F               (1<<10)             
#define BB_IFC_CAUSE_I3_4F             (1<<11)             
#define BB_IFC_IEF                     (1<<16)             
#define BB_IFC_IHF                     (1<<17)             
#define BB_IFC_I4F                     (1<<18)             
#define BB_IFC_I3_4F                   (1<<19)             

//ch_0__start_addr
#define BB_IFC_START_ADDR(n)           (((n)&0x3FFFFFFF)<<2)

//ch_0__fifo_size
#define BB_IFC_FIFO_SIZE(n)            (((n)&0x7FF)<<4)    

//ch_0__reserved

//ch_0__int_mask
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_0__int_clear
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_0__cur_ahb_addr
#define BB_IFC_CUR_AHB_ADDR(n)         (((n)&0x3FFFFFFF)<<2)

//ch_1__control
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_DISABLE                 (1<<1)              
#define BB_IFC_AUTO_DISABLE            (1<<4)              

//ch_1__status
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_FIFO_EMPTY              (1<<4)              
#define BB_IFC_CAUSE_IEF               (1<<8)              
#define BB_IFC_CAUSE_IHF               (1<<9)              
#define BB_IFC_CAUSE_I4F               (1<<10)             
#define BB_IFC_CAUSE_I3_4F             (1<<11)             
#define BB_IFC_IEF                     (1<<16)             
#define BB_IFC_IHF                     (1<<17)             
#define BB_IFC_I4F                     (1<<18)             
#define BB_IFC_I3_4F                   (1<<19)             

//ch_1__start_addr
#define BB_IFC_START_ADDR(n)           (((n)&0x3FFFFFFF)<<2)

//ch_1__fifo_size
#define BB_IFC_FIFO_SIZE(n)            (((n)&0x7FF)<<4)    

//ch_1__reserved

//ch_1__int_mask
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_1__int_clear
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_1__cur_ahb_addr
#define BB_IFC_CUR_AHB_ADDR(n)         (((n)&0x3FFFFFFF)<<2)

//ch_2__control
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_DISABLE                 (1<<1)              
#define BB_IFC_AUTO_DISABLE            (1<<4)              

//ch_2__status
#define BB_IFC_ENABLE                  (1<<0)              
#define BB_IFC_FIFO_EMPTY              (1<<4)              
#define BB_IFC_CAUSE_IEF               (1<<8)              
#define BB_IFC_CAUSE_IHF               (1<<9)              
#define BB_IFC_CAUSE_I4F               (1<<10)             
#define BB_IFC_CAUSE_I3_4F             (1<<11)             
#define BB_IFC_IEF                     (1<<16)             
#define BB_IFC_IHF                     (1<<17)             
#define BB_IFC_I4F                     (1<<18)             
#define BB_IFC_I3_4F                   (1<<19)             

//ch_2__start_addr
#define BB_IFC_START_ADDR(n)           (((n)&0x3FFFFFFF)<<2)

//ch_2__fifo_size
#define BB_IFC_FIFO_SIZE(n)            (((n)&0x7FF)<<4)    

//ch_2__reserved

//ch_2__int_mask
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_2__int_clear
#define BB_IFC_END_FIFO                (1<<8)              
#define BB_IFC_HALF_FIFO               (1<<9)              
#define BB_IFC_QUARTER_FIFO            (1<<10)             
#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             

//ch_2__cur_ahb_addr
#define BB_IFC_CUR_AHB_ADDR(n)         (((n)&0x3FFFFFFF)<<2)


#endif


