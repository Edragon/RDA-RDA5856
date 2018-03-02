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

//
// Created by             :RDA SH-DIGITAL
// Filename               :ir_asm.h
// Author                 :tianwq
// Created On             :2015-08-29 14:51:09
// Last Modified          :
// Update Count           :2015-08-29 14:51:09
// Description            :
//
//======================================================================
#ifndef __IR_ASM_H__
#define __IR_ASM_H__
//Auto-gen by reg_gen
#define REG_IR_BASE                    (0x01A0E000)

#define REG_IR_BASE_HI                 BASE_HI(REG_IR_BASE)
#define REG_IR_BASE_LO                 BASE_LO(REG_IR_BASE)

#define REG_ir_ctrl                    REG_IR_BASE_LO+0x00 
#define REG_ir_time_1                  REG_IR_BASE_LO+0x04 
#define REG_ir_time_2                  REG_IR_BASE_LO+0x08 
#define REG_ir_time_3                  REG_IR_BASE_LO+0x0C 
#define REG_ir_time_4                  REG_IR_BASE_LO+0x10 
#define REG_ir_time_5                  REG_IR_BASE_LO+0x14 
#define REG_ir_code                    REG_IR_BASE_LO+0x18 
#define REG_ir_fsm                     REG_IR_BASE_LO+0x1C 

//ir_ctrl
#define IR_IR_EN                       (1<<0)              
#define IR_IR_MODE(n)                  (((n)&0x3)<<1)      
#define IR_IR_END_DETECT_EN            (1<<3)              
#define IR_IR_INT_EN                   (1<<4)              
#define IR_IR_INT_VERIFY_EN            (1<<5)              
#define IR_IR_USERCODE_VERIFY          (1<<6)              
#define IR_IR_DATACODE_VERIFY          (1<<7)              
#define IR_BIT_TIME_1(n)               (((n)&0x7F)<<8)     
#define IR_BIT_TIME_2(n)               (((n)&0xFF)<<16)    
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
#define IR_R_GPINT_CTRL_R(n)           (((n)&0xFF)<<0)     

//ir_fsm
#define IR_IR_STATUS(n)                (((n)&0xFF)<<0)     


#endif


