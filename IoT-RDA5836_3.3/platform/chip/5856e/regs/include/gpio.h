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
// File name      : gpio                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:40                                                  
// Last modified  : 2016-12-14 15:57:40                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:40 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __GPIO_H__
#define __GPIO_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//low_in
typedef union
{
    U32 v;
    struct
    {
        U32 gpio_low_in                    :14; /*13:0 , read value of GPIO_IN[13:0]   */
        U32 low_in_reserved_0              :18; /*31:14, reserved                      */
    } b;
} T_GPIO_low_in;

//high_in
typedef union
{
    U32 v;
    struct
    {
        U32 high_in_reserved_0             :32; /*31:0 , reserved                      */
    } b;
} T_GPIO_high_in;

//low_out
typedef union
{
    U32 v;
    struct
    {
        U32 gpio_low_out                   :14; /*13:0 , read value of GPIO_OUT[13:0]  */
        U32 low_out_reserved_0             :18; /*31:14, reserved                      */
    } b;
} T_GPIO_low_out;

//high_out
typedef union
{
    U32 v;
    struct
    {
        U32 high_out_reserved_0            :32; /*31:0 , reserved                      */
    } b;
} T_GPIO_high_out;

//low_oenb
typedef union
{
    U32 v;
    struct
    {
        U32 gpio_low_oenb                  :14; /*13:0 , set direction of GPIO[13:0]   */
        U32 low_oenb_reserved_0            :18; /*31:14, reserved                      */
    } b;
} T_GPIO_low_oenb;

//high_oenb
typedef union
{
    U32 v;
    struct
    {
        U32 high_oenb_reserved_0           :32; /*31:0 , reserved                      */
    } b;
} T_GPIO_high_oenb;

//int_cfg1_set
typedef union
{
    U32 v;
    struct
    {
        U32 set_gpint_ctrl_r               :14; /*13:0 , GPIO INT[13:0]: gpio in high or rising active*/
        U32 int_cfg1_set_reserved_1        :2 ; /*15:14, reserved                      */
        U32 set_gpint_ctrl_f               :14; /*29:16, GPIO INT[13:0]: gpio in low or falling active*/
        U32 int_cfg1_set_reserved_0        :2 ; /*31:30, reserved                      */
    } b;
} T_GPIO_int_cfg1_set;

//int_cfg2_set
typedef union
{
    U32 v;
    struct
    {
        U32 set_gpint_dbn_en               :16; /*15:0 , 1: GPIO INT[13:0] debounce by 32K enable
0: GPIO INT[13:0] debounce by 32K disable*/
        U32 set_gpint_mode                 :14; /*29:16, 1: GPIO INT[13:0]: gpio in use level detect mode
0: GPIO INT[13:0]: gpio in use edge detect mode*/
        U32 int_cfg2_set_reserved_0        :2 ; /*31:30, reserved                      */
    } b;
} T_GPIO_int_cfg2_set;

//int_cfg1_clr
typedef union
{
    U32 v;
    struct
    {
        U32 clr_gpint_ctrl_r               :14; /*13:0 , GPIO INT[13:0]: gpio in high or rising active*/
        U32 int_cfg1_clr_reserved_1        :2 ; /*15:14, reserved                      */
        U32 clr_gpint_ctrl_f               :14; /*29:16, GPIO INT[13:0]: gpio in low or falling active*/
        U32 int_cfg1_clr_reserved_0        :2 ; /*31:30, reserved                      */
    } b;
} T_GPIO_int_cfg1_clr;

//int_cfg2_clr
typedef union
{
    U32 v;
    struct
    {
        U32 clr_gpint_dbn_en               :16; /*15:0 , 1: GPIO INT[13:0] debounce by 32K enable
0: GPIO INT[13:0] debounce by 32K disable*/
        U32 clr_gpint_mode                 :14; /*29:16, 1: GPIO INT[13:0]: gpio in use level detect mode
0: GPIO INT[13:0]: gpio in use edge detect mode*/
        U32 int_cfg2_clr_reserved_0        :2 ; /*31:30, reserved                      */
    } b;
} T_GPIO_int_cfg2_clr;

//int_clear
typedef union
{
    U32 v;
    struct
    {
        U32 gpint_clear                    :14; /*13:0 , GPIO INT[13:0] Status Clear   */
        U32 int_clear_reserved_0           :18; /*31:14, reserved                      */
    } b;
} T_GPIO_int_clear;

//int_status
typedef union
{
    U32 v;
    struct
    {
        U32 gpint_level                    :14; /*13:0 , GPIO INT[13:0] Status, clear by register 0x28[13:0]*/
        U32 int_status_reserved_0          :18; /*31:14, reserved                      */
    } b;
} T_GPIO_int_status;

typedef struct
{
    volatile                   T_GPIO_low_in                         low_in; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                  T_GPIO_high_in                        high_in; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                  T_GPIO_low_out                        low_out; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                 T_GPIO_high_out                       high_out; /*0xc  , RO   , 0x00000000, No description.               */
    volatile                 T_GPIO_low_oenb                       low_oenb; /*0x10 , RW   , 0x00003FFF, No description.               */
    volatile                T_GPIO_high_oenb                      high_oenb; /*0x14 , RO   , 0x00000000, No description.               */
    volatile             T_GPIO_int_cfg1_set                   int_cfg1_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile             T_GPIO_int_cfg2_set                   int_cfg2_set; /*0x1c , RW   , 0x0000FFFF, No description.               */
    volatile             T_GPIO_int_cfg1_clr                   int_cfg1_clr; /*0x20 , RW   , 0x00000000, No description.               */
    volatile             T_GPIO_int_cfg2_clr                   int_cfg2_clr; /*0x24 , RW   , 0x0000FFFF, No description.               */
    volatile                T_GPIO_int_clear                      int_clear; /*0x28 , RW   , 0x00000000, No description.               */
    volatile               T_GPIO_int_status                     int_status; /*0x2c , RO   , 0x00000000, No description.               */
} T_HWP_GPIO_T;
#else
typedef struct
{
    volatile                             U32                         low_in; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32                        high_in; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                        low_out; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                       high_out; /*0xc  , RO   , 0x00000000, No description.               */
    volatile                             U32                       low_oenb; /*0x10 , RW   , 0x00003FFF, No description.               */
    volatile                             U32                      high_oenb; /*0x14 , RO   , 0x00000000, No description.               */
    volatile                             U32                   int_cfg1_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                             U32                   int_cfg2_set; /*0x1c , RW   , 0x0000FFFF, No description.               */
    volatile                             U32                   int_cfg1_clr; /*0x20 , RW   , 0x00000000, No description.               */
    volatile                             U32                   int_cfg2_clr; /*0x24 , RW   , 0x0000FFFF, No description.               */
    volatile                             U32                      int_clear; /*0x28 , RW   , 0x00000000, No description.               */
    volatile                             U32                     int_status; /*0x2c , RO   , 0x00000000, No description.               */
} T_HWP_GPIO_T;

//low_in
#define GPIO_GPIO_LOW_IN(n)            (((n)&0x3FFF)<<0) 
#define GPIO_GPIO_LOW_IN_MASK          (0x3FFF<<0)       
#define GPIO_GPIO_LOW_IN_SHIFT         (0)                 


//high_in

//low_out
#define GPIO_GPIO_LOW_OUT(n)           (((n)&0x3FFF)<<0) 
#define GPIO_GPIO_LOW_OUT_MASK         (0x3FFF<<0)       
#define GPIO_GPIO_LOW_OUT_SHIFT        (0)                 


//high_out

//low_oenb
#define GPIO_GPIO_LOW_OENB(n)          (((n)&0x3FFF)<<0) 
#define GPIO_GPIO_LOW_OENB_MASK        (0x3FFF<<0)       
#define GPIO_GPIO_LOW_OENB_SHIFT       (0)                 


//high_oenb

//int_cfg1_set
#define GPIO_SET_GPINT_CTRL_R(n)       (((n)&0x3FFF)<<0)   
#define GPIO_SET_GPINT_CTRL_R_MASK     (0x3FFF<<0)         
#define GPIO_SET_GPINT_CTRL_R_SHIFT    (0)                 

#define GPIO_SET_GPINT_CTRL_F(n)       (((n)&0x3FFF)<<16)  
#define GPIO_SET_GPINT_CTRL_F_MASK     (0x3FFF<<16)        
#define GPIO_SET_GPINT_CTRL_F_SHIFT    (16)                


//int_cfg2_set
#define GPIO_SET_GPINT_DBN_EN(n)       (((n)&0xFFFF)<<0)   
#define GPIO_SET_GPINT_DBN_EN_MASK     (0xFFFF<<0)         
#define GPIO_SET_GPINT_DBN_EN_SHIFT    (0)                 

#define GPIO_SET_GPINT_MODE(n)         (((n)&0x3FFF)<<16)  
#define GPIO_SET_GPINT_MODE_MASK       (0x3FFF<<16)        
#define GPIO_SET_GPINT_MODE_SHIFT      (16)                


//int_cfg1_clr
#define GPIO_CLR_GPINT_CTRL_R(n)       (((n)&0x3FFF)<<0)   
#define GPIO_CLR_GPINT_CTRL_R_MASK     (0x3FFF<<0)         
#define GPIO_CLR_GPINT_CTRL_R_SHIFT    (0)                 

#define GPIO_CLR_GPINT_CTRL_F(n)       (((n)&0x3FFF)<<16)  
#define GPIO_CLR_GPINT_CTRL_F_MASK     (0x3FFF<<16)        
#define GPIO_CLR_GPINT_CTRL_F_SHIFT    (16)                


//int_cfg2_clr
#define GPIO_CLR_GPINT_DBN_EN(n)       (((n)&0xFFFF)<<0)   
#define GPIO_CLR_GPINT_DBN_EN_MASK     (0xFFFF<<0)         
#define GPIO_CLR_GPINT_DBN_EN_SHIFT    (0)                 

#define GPIO_CLR_GPINT_MODE(n)         (((n)&0x3FFF)<<16)  
#define GPIO_CLR_GPINT_MODE_MASK       (0x3FFF<<16)        
#define GPIO_CLR_GPINT_MODE_SHIFT      (16)                


//int_clear
#define GPIO_GPINT_CLEAR(n)            (((n)&0x3FFF)<<0)   
#define GPIO_GPINT_CLEAR_MASK          (0x3FFF<<0)         
#define GPIO_GPINT_CLEAR_SHIFT         (0)                 


//int_statusGPIO_GPINT_LEVEL_MASK
#define GPIO_GPINT_LEVEL(n)            (((n)&0x3FFF)<<0)   
#define GPIO_GPINT_LEVEL_MASK          (0x3FFF<<0)         
#define GPIO_GPINT_LEVEL_SHIFT         (0)                 


#endif

#endif


