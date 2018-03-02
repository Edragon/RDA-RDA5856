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
// File name      : iomux_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:41                                                  
// Last modified  : 2016-12-14 15:57:41                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:41 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __IOMUX_ASM_H__
#define __IOMUX_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_cfg_0                      REG_IOMUX_BASE+0x0  
#define REG_cfg_1                      REG_IOMUX_BASE+0x4  
#define REG_cfg_2                      REG_IOMUX_BASE+0x8  
#define REG_cfg_3                      REG_IOMUX_BASE+0xc  
#define REG_cfg_4                      REG_IOMUX_BASE+0x10 
#define REG_cfg_5                      REG_IOMUX_BASE+0x14 
#define REG_cfg_6                      REG_IOMUX_BASE+0x18 
#define REG_cfg_7                      REG_IOMUX_BASE+0x1c 
#define REG_cfg_8                      REG_IOMUX_BASE+0x20 
#define REG_cfg_9                      REG_IOMUX_BASE+0x24 
#define REG_cfg_10                     REG_IOMUX_BASE+0x28 
#define REG_cfg_11                     REG_IOMUX_BASE+0x2c 
#define REG_cfg_12                     REG_IOMUX_BASE+0x30 
#define REG_cfg_13                     REG_IOMUX_BASE+0x34 

//cfg
#define IOMUX_FUNC_SEL(n)              (((n)&0xF)<<0)      
#define IOMUX_OUT_REG                  (1<<4)              
#define IOMUX_OUT_FORCE                (1<<5)              
#define IOMUX_OEN_REG                  (1<<8)              
#define IOMUX_OEN_FORCE                (1<<9)              
#define IOMUX_PU_REG                   (1<<12)             
#define IOMUX_PD_REG                   (1<<13)             
#define IOMUX_PULL_FORCE               (1<<14)             
#define IOMUX_DRV(n)                   (((n)&0x3)<<20)     
#define IOMUX_PDN                      (1<<24)             


#endif


