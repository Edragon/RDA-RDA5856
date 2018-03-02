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
// File name      : ir_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:51                                                  
// Last modified  : 2016-08-29 14:59:51                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:51 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __IR_ASM_H__
#define __IR_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ir_ctrl                    REG_IR_BASE+0x00    
#define REG_ir_time_1                  REG_IR_BASE+0x04    
#define REG_ir_time_2                  REG_IR_BASE+0x08    
#define REG_ir_time_3                  REG_IR_BASE+0x0C    
#define REG_ir_time_4                  REG_IR_BASE+0x10    
#define REG_ir_time_5                  REG_IR_BASE+0x14    
#define REG_ir_code                    REG_IR_BASE+0x18    
#define REG_ir_fsm                     REG_IR_BASE+0x1C    

//ir_ctrl
#define IR_IR_EN                       (1<<0)              
#define IR_IR_MODE(n)                  (((n)&0x3)<<1)      
#define IR_IR_END_DETECT_EN            (1<<3)              
#define IR_IR_INT_EN                   (1<<4)              
#define IR_IR_INT_VERIFY_EN            (1<<5)              
#define IR_IR_USERCODE_VERIFY          (1<<6)              
#define IR_IR_DATACODE_VERIFY          (1<<7)              
#define IR_BIT_TIME_1(n)               (((n)&0x7F)<<8)     
#define IR_UNUSED                      (1<<15)             
#define IR_BIT_TIME_2(n)               (((n)&0x7F)<<16)    
#define IR_IR_BIT_CYCLE(n)             (((n)&0x7F)<<24)    

//ir_time_1
#define IR_IR_TIME_S1(n)               (((n)&0xFFF)<<0)    

//ir_time_2
#define IR_IR_TIME_S2(n)               (((n)&0xFFF)<<0)    

//ir_time_3
#define IR_IR_TIME_S3(n)               (((n)&0xFFF)<<0)    

//ir_time_4
#define IR_IR_TIME_S4(n)               (((n)&0xFFF)<<0)    

//ir_time_5
#define IR_IR_TIME_S5(n)               (((n)&0xFFF)<<0)    

//ir_code
#define IR_IR_USERCODE(n)              (((n)&0xFFFF)<<0)   
#define IR_IR_DATACODE(n)              (((n)&0xFFFF)<<16)  

//ir_fsm
#define IR_R_IR_RECEIVED_OK            (1<<0)              
#define IR_R_IR_USERCODE_VERIFY        (1<<1)              
#define IR_R_IR_DATACODE_VERIFY        (1<<2)              
#define IR_R_IR_REPEAT                 (1<<3)              


#endif


