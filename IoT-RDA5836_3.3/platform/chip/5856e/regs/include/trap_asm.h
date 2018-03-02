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
// File name      : trap_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:57                                                  
// Last modified  : 2016-12-21 13:55:57                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:57 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __TRAP_ASM_H__
#define __TRAP_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_addr_in_0                  REG_TRAP_BASE+0x0   
#define REG_addr_in_1                  REG_TRAP_BASE+0x4   
#define REG_addr_in_2                  REG_TRAP_BASE+0x8   
#define REG_addr_in_3                  REG_TRAP_BASE+0xc   
#define REG_addr_in_4                  REG_TRAP_BASE+0x10  
#define REG_addr_in_5                  REG_TRAP_BASE+0x14  
#define REG_addr_in_6                  REG_TRAP_BASE+0x18  
#define REG_addr_in_7                  REG_TRAP_BASE+0x1c  
#define REG_addr_in_8                  REG_TRAP_BASE+0x20  
#define REG_addr_in_9                  REG_TRAP_BASE+0x24  
#define REG_addr_in_10                 REG_TRAP_BASE+0x28  
#define REG_addr_in_11                 REG_TRAP_BASE+0x2c  
#define REG_addr_in_12                 REG_TRAP_BASE+0x30  
#define REG_addr_in_13                 REG_TRAP_BASE+0x34  
#define REG_addr_in_14                 REG_TRAP_BASE+0x38  
#define REG_addr_in_15                 REG_TRAP_BASE+0x3c  
#define REG_addr_out_0                 REG_TRAP_BASE+0x40  
#define REG_addr_out_1                 REG_TRAP_BASE+0x44  
#define REG_addr_out_2                 REG_TRAP_BASE+0x48  
#define REG_addr_out_3                 REG_TRAP_BASE+0x4c  
#define REG_addr_out_4                 REG_TRAP_BASE+0x50  
#define REG_addr_out_5                 REG_TRAP_BASE+0x54  
#define REG_addr_out_6                 REG_TRAP_BASE+0x58  
#define REG_addr_out_7                 REG_TRAP_BASE+0x5c  
#define REG_addr_out_8                 REG_TRAP_BASE+0x60  
#define REG_addr_out_9                 REG_TRAP_BASE+0x64  
#define REG_addr_out_10                REG_TRAP_BASE+0x68  
#define REG_addr_out_11                REG_TRAP_BASE+0x6c  
#define REG_addr_out_12                REG_TRAP_BASE+0x70  
#define REG_addr_out_13                REG_TRAP_BASE+0x74  
#define REG_addr_out_14                REG_TRAP_BASE+0x78  
#define REG_addr_out_15                REG_TRAP_BASE+0x7c  
#define REG_en                         REG_TRAP_BASE+0x80  

//addr_in
#define TRAP_ADDR_IN(n)                (((n)&0xFFFF)<<0)   

//addr_out
#define TRAP_ADDR_OUT(n)               (((n)&0xFFFFFFF)<<0)

//en
#define TRAP_EN(n)                     (((n)&0xFFFF)<<0)   


#endif


