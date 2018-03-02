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
// File name      : cfg_regs_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:36                                                  
// Last modified  : 2016-12-14 15:57:36                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:36 --  Create by liqiu                                               
//------------------------------------------------------------------------

#ifndef _CFG_REGS_ASM_H_
#define _CFG_REGS_ASM_H_

//Auto-gen by reg_gen
#include "base_address.h"

#define REG_chip_id                    REG_CFG_REGS_BASE+0x00
#define REG_build_version              REG_CFG_REGS_BASE+0x4

//chip_id
#define CFG_REGS_METAL_ID(n)           (((n)&0xFFF)<<0)    
#define CFG_REGS_EXT_PROD_ID(n)        (((n)&0xF)<<12)     
#define CFG_REGS_PROD_ID(n)            (((n)&0xFFFF)<<16)  

//build_version
#define CFG_REGS_DAY(n)                (((n)&0xFF)<<0)     
#define CFG_REGS_MONTH(n)              (((n)&0xFF)<<8)     
#define CFG_REGS_YEAR(n)               (((n)&0xFF)<<16)    
#define CFG_REGS_MAJOR(n)              (((n)&0xFF)<<24)    


#endif


