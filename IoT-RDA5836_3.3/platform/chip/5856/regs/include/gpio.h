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
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _GPIO_H_
#define _GPIO_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'gpio'."
#endif


#include "8809p_generic_config.h"

// =============================================================================
//  MACROS
// =============================================================================
#define IDX_GPIO_DCON                            (0)
#define IDX_GPO_CHG                                (0)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// GPIO_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_GPIO_BASE               0x01A03000


/*
typedef volatile struct
{
    REG32                           gpio_in[2];//0x00000000
    REG32                           gpio_out[2];//0x00000008
    REG32                           gpio_oenb[2];//0x00000010
    REG32                           gpint_ctrl_set;//0x00000018//?
    REG32                           gpint_ctrl_clr;//0x0000001c//?
    REG8                            gpint_clear;//0x00000020
    REG8                            gpio_reserved1[3];
    REG8                            gpint_level;//0x00000024
    REG8                            gpio_reserved2[3];
    REG32                           gpio_ruen[2];//0x00000028
    REG32                           gpio_rden[2];//0x00000030
    REG32                           gpio_cfg[8];//0x00000038
    REG16                           gpio_drv[8];//0x00000058
    REG16                           gpio_p0_ana_sel;//0x00000068
    REG16                           gpio_reserved3;
    REG32                           gpio_vio2p8_sel[2];//0x0000006c
} HWP_GPIO_T;
*/

typedef volatile struct
{
    REG32                           gpio_in[2];                //0x00000000
    REG32                           gpio_out[2];               //0x00000008
    REG32                           gpio_oenb[2];              //0x00000010
    REG32                           gpint_int_p7_0_cfg_set;    //0x00000018
    REG32                           gpint_int_p15_8_cfg_set;   //0x0000001c
    REG32                           gpint_int_p7_0_cfg_clr;    //0x00000020
    REG32                           gpint_int_p15_8_cfg_clr;   //0x00000024
    REG32                           int_clear;           //0x00000028
    REG32                           int_status;          //0x0000002c
} HWP_GPIO_T;



#define hwp_gpio                    ((HWP_GPIO_T*) KSEG1(REG_GPIO_BASE))


//gpio_oen_val
#define GPIO_OEN_VAL(n)             (((n)&0xFFFFFFFF)<<0)
#define GPIO_OEN_VAL_INPUT          (0x1<<0)
#define GPIO_OEN_VAL_OUTPUT         (0x0<<0)

//gpio_oen_set_out
#define GPIO_OEN_SET_OUT(n)         (((n)&0xFFFFFFFF)<<0)

//gpio_oen_set_in
#define GPIO_OEN_SET_IN(n)          (((n)&0xFFFFFFFF)<<0)

//gpio_oen_set_up_enable
#define GPIO_OEN_SET_UP(n)         (((n)&0xFFFFFFFF)<<0)

//gpio_oen_set_down_enable
#define GPIO_OEN_SET_DOWN(n)          (((n)&0xFFFFFFFF)<<0)//clear corresponding bit

//gpio_val
#define GPIO_GPIO_VAL(n)            (((n)&0xFFFFFFFF)<<0)

//gpio_set
#define GPIO_GPIO_SET(n)            (((n)&0xFFFFFFFF)<<0)

//gpio_clr
#define GPIO_GPIO_CLR(n)            (((n)&0xFFFFFFFF)<<0)


//gpint_ctrl_set
#define GPIO_GPINT_R_SET(n)         (((n)&0xFF)<<0)
#define GPIO_GPINT_F_SET(n)         (((n)&0xFF)<<8)
#define GPIO_DBN_EN_SET(n)          (((n)&0xFF)<<16)
#define GPIO_GPINT_MODE_SET(n)      (((n)&0xFF)<<24)

//gpint_ctrl_clr
#define GPIO_GPINT_R_CLR(n)         (((n)&0xFF)<<0)
#define GPIO_GPINT_F_CLR(n)         (((n)&0xFF)<<8)
#define GPIO_DBN_EN_CLR(n)          (((n)&0xFF)<<16)
#define GPIO_GPINT_MODE_CLR(n)      (((n)&0xFF)<<24)


//int_clr
#define GPIO_GPINT_CLR(n)           (((n)&0xFF)<<0)

//int_status
#define GPIO_GPINT_STATUS(n)        (((n)&0xFF)<<0)
#define GPIO_GPINT_STATUS_MASK      (0xFF<<0)
#define GPIO_GPINT_STATUS_SHIFT     (0)

//chg_ctrl
#define GPIO_OUT_TIME(n)            (((n)&15)<<0)
#define GPIO_WAIT_TIME(n)           (((n)&0x3F)<<4)
#define GPIO_INT_MODE_L2H           (0<<16)
#define GPIO_INT_MODE_H2L           (1<<16)
#define GPIO_INT_MODE_RR            (3<<16)

//chg_cmd
#define GPIO_DCON_MODE_SET          (1<<0)
#define GPIO_CHG_MODE_SET           (1<<4)
#define GPIO_DCON_MODE_CLR          (1<<8)
#define GPIO_CHG_MODE_CLR           (1<<12)
#define GPIO_CHG_DOWN               (1<<24)

//gpo_set
#define GPIO_GPO_SET(n)             (((n)&0xFFF)<<0)

//gpo_clr
#define GPIO_GPO_CLR(n)             (((n)&0xFFF)<<0)


#endif

