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
// File name      : ir                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:51                                                  
// Last modified  : 2016-08-29 14:59:51                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:51 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __IR_H__
#define __IR_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ir_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ir_en                          :1 ; /*0 :0 , 0: Disable Disable IR function1: Enable Enable IR function*/
        U32 ir_mode                        :2 ; /*2 :1 , 0: NEC NEC mode1: 9012 Toshiba 9012 mode2: RC5 Philips RC5*/
        U32 ir_end_detect_en               :1 ; /*3 :3 , 0: Disalbe Disable detect end bit1: Enable Enable detect end bit*/
        U32 ir_int_en                      :1 ; /*4 :4 , 0: Disable Disable ir interrupt generation1: Enable Enable ir interrupt generation*/
        U32 ir_int_verify_en               :1 ; /*5 :5 , 0: Diable Disable ir interrupt generation by verify1: Enable Enable ir interrupt generation by verify*/
        U32 ir_usercode_verify             :1 ; /*6 :6 , 0: Disable Disable verify usercode1: Enable Enable verify usercode*/
        U32 ir_datacode_verify             :1 ; /*7 :7 , 0: Disable Disable verify datacode1: Enable Enable verify datacode*/
        U32 bit_time_1                     :7 ; /*14:8 , bit time 1                    */
        U32 unused                         :1 ; /*15:15, No description.               */
        U32 bit_time_2                     :7 ; /*22:16, bit time 2                    */
        U32 ir_ctrl_reserved_0             :1 ; /*23:23, No description.               */
        U32 ir_bit_cycle                   :7 ; /*30:24, bit cycle                     */
        U32 reserved_1                     :1 ; /*31:31,                               */
    } b;
} T_IR_ir_ctrl;

//ir_time_1
typedef union
{
    U32 v;
    struct
    {
        U32 ir_time_s1                     :12; /*11:0 , 9ms high time                 */
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_IR_ir_time_1;

//ir_time_2
typedef union
{
    U32 v;
    struct
    {
        U32 ir_time_s2                     :12; /*11:0 , 2.25ms low time               */
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_IR_ir_time_2;

//ir_time_3
typedef union
{
    U32 v;
    struct
    {
        U32 ir_time_s3                     :12; /*11:0 , 4.5ms low time                */
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_IR_ir_time_3;

//ir_time_4
typedef union
{
    U32 v;
    struct
    {
        U32 ir_time_s4                     :12; /*11:0 , s3 max value                  */
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_IR_ir_time_4;

//ir_time_5
typedef union
{
    U32 v;
    struct
    {
        U32 ir_time_s5                     :12; /*11:0 , one frame time                */
        U32 reserved_0                     :1 ; /*12:12,                               */
        U32 reserved_1                     :1 ; /*13:13,                               */
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 reserved_4                     :1 ; /*16:16,                               */
        U32 reserved_5                     :1 ; /*17:17,                               */
        U32 reserved_6                     :1 ; /*18:18,                               */
        U32 reserved_7                     :1 ; /*19:19,                               */
        U32 reserved_8                     :1 ; /*20:20,                               */
        U32 reserved_9                     :1 ; /*21:21,                               */
        U32 reserved_10                    :1 ; /*22:22,                               */
        U32 reserved_11                    :1 ; /*23:23,                               */
        U32 reserved_12                    :1 ; /*24:24,                               */
        U32 reserved_13                    :1 ; /*25:25,                               */
        U32 reserved_14                    :1 ; /*26:26,                               */
        U32 reserved_15                    :1 ; /*27:27,                               */
        U32 reserved_16                    :1 ; /*28:28,                               */
        U32 reserved_17                    :1 ; /*29:29,                               */
        U32 reserved_18                    :1 ; /*30:30,                               */
        U32 reserved_19                    :1 ; /*31:31,                               */
    } b;
} T_IR_ir_time_5;

//ir_code
typedef union
{
    U32 v;
    struct
    {
        U32 ir_usercode                    :16; /*15:0 , ir user code                  */
        U32 ir_datacode                    :16; /*31:16, ir data code                  */
    } b;
} T_IR_ir_code;

//ir_fsm
typedef union
{
    U32 v;
    struct
    {
        U32 r_ir_received_ok               :1 ; /*0 :0 , No description.               */
        U32 r_ir_usercode_verify           :1 ; /*1 :1 , No description.               */
        U32 r_ir_datacode_verify           :1 ; /*2 :2 , No description.               */
        U32 r_ir_repeat                    :1 ; /*3 :3 , No description.               */
        U32 ir_fsm_reserved_0              :28; /*31:4 , No description.               */
    } b;
} T_IR_ir_fsm;

typedef struct
{
    volatile                    T_IR_ir_ctrl                        ir_ctrl; /*0x0  , RW   , 0x49260A11, No description.               */
    volatile                  T_IR_ir_time_1                      ir_time_1; /*0x4  , RW   , 0x000000FF, No description.               */
    volatile                  T_IR_ir_time_2                      ir_time_2; /*0x8  , RW   , 0x0000013F, No description.               */
    volatile                  T_IR_ir_time_3                      ir_time_3; /*0xc  , RW   , 0x0000019F, No description.               */
    volatile                  T_IR_ir_time_4                      ir_time_4; /*0x10 , RW   , 0x000001BF, No description.               */
    volatile                  T_IR_ir_time_5                      ir_time_5; /*0x14 , RW   , 0x00000C7F, No description.               */
    volatile                    T_IR_ir_code                        ir_code; /*0x18 , RO   , 0x00000000, No description.               */
    volatile                     T_IR_ir_fsm                         ir_fsm; /*0x1c , RO   , 0x00000000, No description.               */
} T_HWP_IR_T;
#else
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
} T_HWP_IR_T;

//ir_ctrl
#define IR_IR_EN                       (1<<0)              
#define IR_IR_EN_DISABLE               (0<<0)              
#define IR_IR_EN_ENABLE                (1<<0)              

#define IR_IR_MODE(n)                  (((n)&0x3)<<1)      
#define IR_IR_MODE_NEC                 (((0)&0x3)<<1)      
#define IR_IR_MODE_9012                (((1)&0x3)<<1)      
#define IR_IR_MODE_RC5                 (((2)&0x3)<<1)      
#define IR_IR_MODE_MASK                (0x3<<1)            
#define IR_IR_MODE_SHIFT               (1)                 

#define IR_IR_END_DETECT_EN            (1<<3)              
#define IR_IR_END_DETECT_EN_DISALBE    (0<<3)              
#define IR_IR_END_DETECT_EN_ENABLE     (1<<3)              

#define IR_IR_INT_EN                   (1<<4)              
#define IR_IR_INT_EN_DISABLE           (0<<4)              
#define IR_IR_INT_EN_ENABLE            (1<<4)              

#define IR_IR_INT_VERIFY_EN            (1<<5)              
#define IR_IR_INT_VERIFY_EN_DIABLE     (0<<5)              
#define IR_IR_INT_VERIFY_EN_ENABLE     (1<<5)              

#define IR_IR_USERCODE_VERIFY          (1<<6)              
#define IR_IR_USERCODE_VERIFY_DISABLE  (0<<6)              
#define IR_IR_USERCODE_VERIFY_ENABLE   (1<<6)              

#define IR_IR_DATACODE_VERIFY          (1<<7)              
#define IR_IR_DATACODE_VERIFY_DISABLE  (0<<7)              
#define IR_IR_DATACODE_VERIFY_ENABLE   (1<<7)              

#define IR_BIT_TIME_1(n)               (((n)&0x7F)<<8)     
#define IR_BIT_TIME_1_MASK             (0x7F<<8)           
#define IR_BIT_TIME_1_SHIFT            (8)                 

#define IR_UNUSED                      (1<<15)             

#define IR_BIT_TIME_2(n)               (((n)&0x7F)<<16)    
#define IR_BIT_TIME_2_MASK             (0x7F<<16)          
#define IR_BIT_TIME_2_SHIFT            (16)                

#define IR_IR_BIT_CYCLE(n)             (((n)&0x7F)<<24)    
#define IR_IR_BIT_CYCLE_MASK           (0x7F<<24)          
#define IR_IR_BIT_CYCLE_SHIFT          (24)                


//ir_time_1
#define IR_IR_TIME_S1(n)               (((n)&0xFFF)<<0)    
#define IR_IR_TIME_S1_MASK             (0xFFF<<0)          
#define IR_IR_TIME_S1_SHIFT            (0)                 


//ir_time_2
#define IR_IR_TIME_S2(n)               (((n)&0xFFF)<<0)    
#define IR_IR_TIME_S2_MASK             (0xFFF<<0)          
#define IR_IR_TIME_S2_SHIFT            (0)                 


//ir_time_3
#define IR_IR_TIME_S3(n)               (((n)&0xFFF)<<0)    
#define IR_IR_TIME_S3_MASK             (0xFFF<<0)          
#define IR_IR_TIME_S3_SHIFT            (0)                 


//ir_time_4
#define IR_IR_TIME_S4(n)               (((n)&0xFFF)<<0)    
#define IR_IR_TIME_S4_MASK             (0xFFF<<0)          
#define IR_IR_TIME_S4_SHIFT            (0)                 


//ir_time_5
#define IR_IR_TIME_S5(n)               (((n)&0xFFF)<<0)    
#define IR_IR_TIME_S5_MASK             (0xFFF<<0)          
#define IR_IR_TIME_S5_SHIFT            (0)                 


//ir_code
#define IR_IR_USERCODE(n)              (((n)&0xFFFF)<<0)   
#define IR_IR_USERCODE_MASK            (0xFFFF<<0)         
#define IR_IR_USERCODE_SHIFT           (0)                 

#define IR_IR_DATACODE(n)              (((n)&0xFFFF)<<16)  
#define IR_IR_DATACODE_MASK            (0xFFFF<<16)        
#define IR_IR_DATACODE_SHIFT           (16)                


//ir_fsm
#define IR_R_IR_RECEIVED_OK            (1<<0)              

#define IR_R_IR_USERCODE_VERIFY        (1<<1)              

#define IR_R_IR_DATACODE_VERIFY        (1<<2)              

#define IR_R_IR_REPEAT                 (1<<3)              


#endif

#endif


