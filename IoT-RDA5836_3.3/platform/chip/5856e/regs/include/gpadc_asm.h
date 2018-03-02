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
// File name      : gpadc_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:49                                                  
// Last modified  : 2016-08-29 14:59:49                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:49 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __GPADC_ASM_H__
#define __GPADC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_key_data                   REG_GPADC_BASE+0x00 
#define REG_gpadc_data_ch_0            REG_GPADC_BASE+0x4  
#define REG_gpadc_data_ch_1            REG_GPADC_BASE+0x8  
#define REG_gpadc_data_ch_2            REG_GPADC_BASE+0xc  
#define REG_gpadc_data_ch_3            REG_GPADC_BASE+0x10 
#define REG_gpadc_data_ch_4            REG_GPADC_BASE+0x14 
#define REG_gpadc_data_ch_5            REG_GPADC_BASE+0x18 
#define REG_gpadc_data_ch_6            REG_GPADC_BASE+0x1c 
#define REG_gpadc_data_ch_7            REG_GPADC_BASE+0x20 

//key_data
#define GPADC_KEY_VALUE_BIT(n)         (((n)&0x3FF)<<0)    
#define GPADC_KEY_VALUE_VALID          (1<<10)             

//gpadc_data_ch
#define GPADC_GPADC_VALUE_BIT(n)       (((n)&0x3FF)<<0)    
#define GPADC_GPADC_VALUE_VALID        (1<<10)             
#define GPADC_GPADC_EN                 (1<<31)             


#endif


