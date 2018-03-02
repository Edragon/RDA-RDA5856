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
// File name      : gpio_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:40                                                  
// Last modified  : 2016-12-14 15:57:40                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:40 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __GPIO_ASM_H__
#define __GPIO_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_low_in                     REG_GPIO_BASE+0x00  
#define REG_high_in                    REG_GPIO_BASE+0x04  
#define REG_low_out                    REG_GPIO_BASE+0x08  
#define REG_high_out                   REG_GPIO_BASE+0x0C  
#define REG_low_oenb                   REG_GPIO_BASE+0x10  
#define REG_high_oenb                  REG_GPIO_BASE+0x14  
#define REG_int_cfg1_set               REG_GPIO_BASE+0x18  
#define REG_int_cfg2_set               REG_GPIO_BASE+0x1C  
#define REG_int_cfg1_clr               REG_GPIO_BASE+0x20  
#define REG_int_cfg2_clr               REG_GPIO_BASE+0x24  
#define REG_int_clear                  REG_GPIO_BASE+0x28  
#define REG_int_status                 REG_GPIO_BASE+0x2C  

//low_in
#define GPIO_GPIO_LOW_IN(n)            (((n)&0x3FFF)<<0)   

//high_in

//low_out
#define GPIO_GPIO_LOW_OUT(n)           (((n)&0x3FFF)<<0)   

//high_out

//low_oenb
#define GPIO_GPIO_LOW_OENB(n)          (((n)&0x3FFF)<<0)   

//high_oenb

//int_cfg1_set
#define GPIO_SET_GPINT_CTRL_R(n)       (((n)&0x3FFF)<<0)   
#define GPIO_SET_GPINT_CTRL_F(n)       (((n)&0x3FFF)<<16)  

//int_cfg2_set
#define GPIO_SET_GPINT_DBN_EN(n)       (((n)&0xFFFF)<<0)   
#define GPIO_SET_GPINT_MODE(n)         (((n)&0x3FFF)<<16)  

//int_cfg1_clr
#define GPIO_CLR_GPINT_CTRL_R(n)       (((n)&0x3FFF)<<0)   
#define GPIO_CLR_GPINT_CTRL_F(n)       (((n)&0x3FFF)<<16)  

//int_cfg2_clr
#define GPIO_CLR_GPINT_DBN_EN(n)       (((n)&0xFFFF)<<0)   
#define GPIO_CLR_GPINT_MODE(n)         (((n)&0x3FFF)<<16)  

//int_clear
#define GPIO_GPINT_CLEAR(n)            (((n)&0x3FFF)<<0)   

//int_status
#define GPIO_GPINT_LEVEL(n)            (((n)&0x3FFF)<<0)   


#endif


