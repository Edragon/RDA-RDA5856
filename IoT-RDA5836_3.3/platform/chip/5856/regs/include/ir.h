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

#ifndef _IR_H_
#define _IR_H_
/*
#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'gpio'."
#endif
*/
#include "8809p_generic_config.h"

extern void IR_Init();

#define REG_IR_BASE               (0x01A0E000)

typedef struct
{
    volatile                             U32                        ir_ctrl; /*0x0  , RW   , 0x49260A11, No description.               */
    volatile                             U32                      ir_time_1; /*0x4  , RW   , 0x000000FF, No description.               */
    volatile                             U32                      ir_time_2; /*0x8  , RW   , 0x0000013F, No description.               */
    volatile                             U32                      ir_time_3; /*0xc  , RW   , 0x0000019F, No description.               */
    volatile                             U32                      ir_time_4; /*0x10 , RW   , 0x000001BF, No description.               */
    volatile                             U32                      ir_time_5; /*0x14 , RW   , 0x00000C7F, No description.               */
    volatile                             U32                        ir_code; /*0x18 , RO   , 0x00000000, No description.               */
    volatile                             U32                         ir_fsm; /*0x1c , RO   , 0x00000000, No description.               */
} HWP_IR_T;


#define hwp_ir                    ((HWP_IR_T*) KSEG1(REG_IR_BASE))

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
