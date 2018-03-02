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
// File name      : dbm_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:41                                                  
// Last modified  : 2016-12-21 13:55:41                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:41 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DBM_ASM_H__
#define __DBM_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_sel                        REG_DBM_BASE+0x00   
#define REG_trans                      REG_DBM_BASE+0x04   
#define REG_start_addr                 REG_DBM_BASE+0x08   
#define REG_mask                       REG_DBM_BASE+0x0C   
#define REG_status                     REG_DBM_BASE+0x10   
#define REG_soft_reset                 REG_DBM_BASE+0x14   
#define REG_soft_trig                  REG_DBM_BASE+0x18   

//sel
#define DBM_DBM_EN                     (1<<0)              

//trans
#define DBM_BURST_LEN(n)               (((n)&0xF)<<0)      
#define DBM_BURST_TYPE(n)              (((n)&0x3)<<4)      
#define DBM_WRAP_EN                    (1<<6)              
#define DBM_DUMP_TRIG_SEL              (1<<7)              
#define DBM_DATA_SEL(n)                (((n)&0xF)<<8)      
#define DBM_WORD_LEN(n)                (((n)&0xFFFFF)<<12) 

//start_addr
#define DBM_START_ADDR(n)              (((n)&0xFFFFFFFF)<<0)

//mask
#define DBM_MASK_COMP_END              (1<<0)              
#define DBM_MASK_COMP_HALF             (1<<1)              
#define DBM_MASK_OVFL                  (1<<2)              
#define DBM_MASK_TRANS_ERR             (1<<3)              

//status
#define DBM_SRST_DONE_STATUS           (1<<0)              
#define DBM_INT_COMP_END_STATUS        (1<<1)              
#define DBM_INT_COMP_HALF_STATUS       (1<<2)              
#define DBM_INT_OVFL_STATUS            (1<<3)              
#define DBM_INT_TRANS_ERR_STATUS       (1<<4)              

//soft_reset
#define DBM_S_RESET                    (1<<0)              

//soft_trig
#define DBM_S_DP_TRIG                  (1<<0)              


#endif


