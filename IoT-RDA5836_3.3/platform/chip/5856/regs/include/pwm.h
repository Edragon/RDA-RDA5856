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

#ifndef _PWM_H_
#define _PWM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'pwm'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// PWM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_PWM_BASE                0x01A06000

typedef volatile struct
{
    REG32                          PWT_Config;                   //0x00000000
    REG32                          LPG_Config;                   //0x00000004
    REG32                          LPG_ontime;                   //0x00000008
    REG32                          LPG_offtime;                  //0x0000000C
    REG32                          LPG_Fifo_Pattern;             //0x00000010
    REG32                          PWL0_Config;                  //0x00000014
    REG32                          PWL1_Config;                  //0x00000018

    REG32                          PWT_Config_CH1;               //0x0000001C
    REG32                          LPG_Config_CH1;               //0x00000020
    REG32                          LPG_ontime_CH1;               //0x00000024
    REG32                          LPG_offtime_CH1;              //0x00000028
    REG32                          LPG_Fifo_Pattern_CH1;         //0x0000002C
    REG32                          PWL0_Config_CH1;              //0x00000030
    REG32                          PWL1_Config_CH1;              //0x00000034

    REG32                          PWT_Config_CH2;               //0x00000038
    REG32                          LPG_Config_CH2;               //0x0000003C
    REG32                          LPG_ontime_CH2;               //0x00000040
    REG32                          LPG_offtime_CH2;              //0x00000044
    REG32                          LPG_Fifo_Pattern_CH2;         //0x00000048
    REG32                          PWL0_Config_CH2;              //0x0000004C
    REG32                          PWL1_Config_CH2;              //0x00000050

    REG32                          PWT_Config_CH3;               //0x00000054
    REG32                          LPG_Config_CH3;               //0x00000058
    REG32                          LPG_ontime_CH3;               //0x0000005C
    REG32                          LPG_offtime_CH3;              //0x00000060
    REG32                          LPG_Fifo_Pattern_CH3;         //0x00000064
    REG32                          PWL0_Config_CH3;              //0x00000068
    REG32                          PWL1_Config_CH3;              //0x0000006C

    REG32                          PWM_OUT_SEL;                  //0x00000070
    REG32                          TSC_DATA;                     //0x00000074
    //REG32                          GPADC_DATA;                 //0x00000014
    REG32                          GPADC_DATA_CH[8];             //0x00000078
} HWP_PWM_T;

#define hwp_pwm                     ((HWP_PWM_T*) KSEG1(REG_PWM_BASE))


//PWT_Config
#define PWM_PWT_ENABLE              (1<<0)
#define PWM_PWT_DUTY(n)             (((n)&0x3FF)<<4)
#define PWM_PWT_PERIOD(n)           (((n)&0x7FF)<<16)

//LPG_Config
#define PWM_LPG_RESET_L             (1<<0)
#define PWM_LPG_TEST                (1<<1)
#define PWM_LPG_ONTIME(n)           (((n)&15)<<4)
#define PWM_LPG_ONTIME_MASK         (15<<4)
#define PWM_LPG_ONTIME_SHIFT        (4)
#define PWM_LPG_ONTIME_UNDEFINED    (0<<4)
#define PWM_LPG_ONTIME_15_6MS       (1<<4)
#define PWM_LPG_ONTIME_31_2MS       (2<<4)
#define PWM_LPG_ONTIME_46_8MS       (3<<4)
#define PWM_LPG_ONTIME_62MS         (4<<4)
#define PWM_LPG_ONTIME_78MS         (5<<4)
#define PWM_LPG_ONTIME_94MS         (6<<4)
#define PWM_LPG_ONTIME_110MS        (7<<4)
#define PWM_LPG_ONTIME_125MS        (8<<4)
#define PWM_LPG_ONTIME_140MS        (9<<4)
#define PWM_LPG_ONTIME_156MS        (10<<4)
#define PWM_LPG_ONTIME_172MS        (11<<4)
#define PWM_LPG_ONTIME_188MS        (12<<4)
#define PWM_LPG_ONTIME_200MS        (13<<4)
#define PWM_LPG_ONTIME_218MS        (14<<4)
#define PWM_LPG_ONTIME_234MS        (15<<4)
#define PWM_LPG_PERIOD(n)           (((n)&7)<<16)
#define PWM_LPG_PERIOD_MASK         (7<<16)
#define PWM_LPG_PERIOD_SHIFT        (16)
#define PWM_LPG_PERIOD_0_125S       (0<<16)
#define PWM_LPG_PERIOD_0_25S        (1<<16)
#define PWM_LPG_PERIOD_0_5S         (2<<16)
#define PWM_LPG_PERIOD_0_75S        (3<<16)
#define PWM_LPG_PERIOD_1S           (4<<16)
#define PWM_LPG_PERIOD_1_25S        (5<<16)
#define PWM_LPG_PERIOD_1_5S         (6<<16)
#define PWM_LPG_PERIOD_1_75S        (7<<16)

//PWL0_Config
#define PWM_PWL_MIN(n)              (((n)&0xFF)<<0)
#define PWM_PWL_MAX(n)              (((n)&0xFF)<<8)
#define PWM_PWL0_EN_H               (1<<16)
#define PWM_PWL0_FORCE_L            (1<<17)
#define PWM_PWL0_FORCE_H            (1<<18)
#define PWM_PWL_PULSE_EN            (1<<19)
#define PWM_PWL0_SET_OE             (1<<20)
#define PWM_PWL0_CLR_OE             (1<<21)
#define PWM_PWL_SET_MUX             (1<<22)
#define PWM_PWL_CLR_MUX             (1<<23)
#define PWM_PWL_PULSE_PER(n)        (((n)&0xFF)<<24)

//PWL1_Config
#define PWM_PWL1_THRESHOLD(n)       (((n)&0xFF)<<0)
#define PWM_LFSR_REG(n)             (((n)&0xFF)<<8)
#define PWM_PWL1_EN_H               (1<<16)
#define PWM_PWL1_FORCE_L            (1<<17)
#define PWM_PWL1_FORCE_H            (1<<18)
#define PWM_PWL1_SET_OE             (1<<20)
#define PWM_PWL1_CLR_OE             (1<<21)

//TSC_DATA
#define PWM_TSC_X_VALUE(n)          (((n)&0x3FF)<<0)
#define PWM_TSC_X_VALUE_MASK        (0x3FF<<0)
#define PWM_TSC_X_VALUE_SHIFT       (0)
#define PWM_TSC_X_VALID             (1<<10)
#define PWM_TSC_Y_VALUE(n)          (((n)&0x3FF)<<11)
#define PWM_TSC_Y_VALUE_MASK        (0x3FF<<11)
#define PWM_TSC_Y_VALUE_SHIFT       (11)
#define PWM_TSC_Y_VALID             (1<<21)

//GPADC_DATA
#define PWM_GPADC_VALUE(n)          (((n)&0x3FF)<<0)
#define PWM_GPADC_VALUE_MASK        (0x3FF<<0)
#define PWM_GPADC_VALUE_SHIFT       (0)
#define PWM_GPADC_VALID             (1<<10)


//GPADC_DATA_CH
#define CFG_REGS_GPADC_DATA(n)     (((n)&0x3FF)<<0)
#define CFG_REGS_GPADC_DATA_MASK   (0x3FF<<0)
#define CFG_REGS_GPADC_DATA_SHIFT  (0)
#define CFG_REGS_GPADC_VALID       (1<<10)
#define CFG_REGS_GPADC_CH_EN       (1<<31)

#endif

