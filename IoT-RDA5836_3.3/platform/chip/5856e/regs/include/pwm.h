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
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//pwt_config
typedef union
{
    U32 v;
    struct
    {
        U32 pwt_enable                     :1 ; /*0 :0 , Enables the Pulse Width Tone output 
1 = Enable PWT output 
0 = Disable PWT output*/
        U32 pwt_start                      :1 ; /*1 :1 , No description.               */
        U32 pwt_config_reserved_2          :2 ; /*3 :2 , No description.               */
        U32 pwt_duty                       :10; /*13:4 , The PWT_Duty value can be used to set the approximate volume of the tone. 
The PWT_Duty value must be less than or equal to half the PWT_Period value and must be at least a value of 8, otherwise no tone will be generated.*/
        U32 pwt_config_reserved_1          :2 ; /*15:14, No description.               */
        U32 pwt_period                     :11; /*26:16, PWT_Period is the divider value to produce a tone of a given frequency. 
To calculate the PWT_Period value, Use the following formula: 
PWT_Period = FBASE/FNOTE 
where FBASE is the frequency of the PWM module clock (it is based on the system frequency, 26, 39, 52, 78 or 104 MHz divided by 5). FNOTE is the frequency of the desired tone.*/
        U32 pwt_config_reserved_0          :5 ; /*31:27, No description.               */
    } b;
} T_PWM_pwt_config;

//lpg_config
typedef union
{
    U32 v;
    struct
    {
        U32 lpg_en                         :1 ; /*0 :0 , No description.               */
        U32 lpg_mode                       :1 ; /*1 :1 , 0: infinite squre wave 1: fifo wave*/
        U32 lpg_fifo_full                  :1 ; /*2 :2 , No description.               */
        U32 lpg_fifo_empty                 :1 ; /*3 :3 , No description.               */
        U32 lpg_fifo_count                 :4 ; /*7 :4 , No description.               */
        U32 lpg_irq_msk                    :1 ; /*8 :8 , 1: IRQ is masked 0: IRQ is unmasked*/
        U32 lpg_config_reserved_0          :23; /*31:9 , No description.               */
    } b;
} T_PWM_lpg_config;

//lpg_ontime
typedef union
{
    U32 v;
    struct
    {
        U32 lpg_high_cycle                 :32; /*31:0 , minimum value is 1, 0 is illegal value*/
    } b;
} T_PWM_lpg_ontime;

//lpg_offtime
typedef union
{
    U32 v;
    struct
    {
        U32 lpg_low_cycle                  :32; /*31:0 , minimum value is 1, 0 is illegal value*/
    } b;
} T_PWM_lpg_offtime;

//lpg_fifo_pattern
typedef union
{
    U32 v;
    struct
    {
        U32 lpg_pattern_cycle              :31; /*30:0 , minimum value is 1, 0 is illegal value*/
        U32 lpg_level                      :1 ; /*31:31, No description.               */
    } b;
} T_PWM_lpg_fifo_pattern;

//pwl0_config
typedef union
{
    U32 v;
    struct
    {
        U32 pwl_min                        :8 ; /*7 :0 , Sets the lower boundary for PWL pulse. When pulse mode is not used, this is the threshold value for the PWL0. Reading this value will return the current value used for the threshold.*/
        U32 pwl_max                        :8 ; /*15:8 , Sets the upper boundary for PWL pulse. When pulse mode is not used, this value is ignored. Reading this value will return the LFSR value used for generating the PWL outputs.*/
        U32 pwl0_en_h                      :1 ; /*16:16, When this bit is written with '1', the PWL 0 is enabled and the output is a PRBS whose average on-time is proportional to PWL_Min. This bit is cleared when either of the Force bits are written. Reading this bit will return the current state of the PWL0 enable.*/
        U32 pwl0_force_l                   :1 ; /*17:17, Writing a '1' to this bit will force the PWL0 to output a low value. If the PWL0 was previously enabled, this will clear the bit.*/
        U32 pwl0_force_h                   :1 ; /*18:18, Writing a '1' to this bit will force the PWL0 to output a high value. If the PWL0 was previously enabled, this will clear the bit.*/
        U32 pwl_pulse_en                   :1 ; /*19:19, This will enable the PWL pulse mode. The threshold will dynamically sweep between PWL_Min and PWL_Max at a rate depending on PWL_Pulse_Per.*/
        U32 pwl0_set_oe                    :1 ; /*20:20, Writing '1' to this bit will set the output enable. Reading this bit will return the current status.*/
        U32 pwl0_clr_oe                    :1 ; /*21:21, Writing '1' to this bit will clear the output enable.*/
        U32 pwl_set_mux                    :1 ; /*22:22, Writing a '1' to this bit will swap the PWL0 and PWL1 outputs. Reading this bit will return the current status.*/
        U32 pwl_clr_mux                    :1 ; /*23:23, Writing a '1' to this bit will unswap the PWL0/PWL1 outputs.*/
        U32 pwl_pulse_per                  :8 ; /*31:24, This value will adjust the pulse period when pulsing is enabled.*/
    } b;
} T_PWM_pwl0_config;

//pwl1_config
typedef union
{
    U32 v;
    struct
    {
        U32 pwl1_threshold                 :8 ; /*7 :0 , Average duty cycle for the Pulse Width Light 1 output. The average duty cycle is calculated as PWL1_Threshold/256.*/
        U32 lfsr_reg                       :8 ; /*15:8 , LFSR value for PWL.           */
        U32 pwl1_en_h                      :1 ; /*16:16, When this bit is written with '1', the PWL 1 is enabled and the output is a PRBS whose average on-time is proportional to PWL1_Threshold. This bit is cleared when either of the Force bits are written. Reading this bit will return the current state of the PWL1 enable.*/
        U32 pwl1_force_l                   :1 ; /*17:17, Writing a '1' to this bit will force the PWL1 to output a low value. If the PWL1 was previously enabled, this will clear the bit.*/
        U32 pwl1_force_h                   :1 ; /*18:18, Writing a '1' to this bit will force the PWL1 to output a high value. If the PWL1 was previously enabled, this will clear the bit.*/
        U32 pwl1_config_reserved_1         :1 ; /*19:19, No description.               */
        U32 pwl1_set_oe                    :1 ; /*20:20, Writing '1' to this bit will set the output enable. Reading this bit will return the current status.*/
        U32 pwl1_clr_oe                    :1 ; /*21:21, Writing '1' to this bit will clear the output enable.*/
        U32 pwl1_config_reserved_0         :10; /*31:22, No description.               */
    } b;
} T_PWM_pwl1_config;

//pwm_out_sel
typedef union
{
    U32 v;
    struct
    {
        U32 ch0_sel                        :2 ; /*1 :0 , 00: PWT output    01:PWL0 output  10: PWL1 output  11: LPG output*/
        U32 ch1_sel                        :2 ; /*3 :2 , 00: PWT output    01:PWL0 output  10: PWL1 output  11: LPG output*/
        U32 ch2_sel                        :2 ; /*5 :4 , 00: PWT output    01:PWL0 output  10: PWL1 output  11: LPG output*/
        U32 ch3_sel                        :2 ; /*7 :6 , 00: PWT output    01:PWL0 output  10: PWL1 output  11: LPG output*/
        U32 pwm_out_sel_reserved_0         :24; /*31:8 , No description.               */
    } b;
} T_PWM_pwm_out_sel;

typedef struct
{
    volatile                T_PWM_pwt_config                     pwt_config;
    volatile                T_PWM_lpg_config                     lpg_config;
    volatile                T_PWM_lpg_ontime                     lpg_ontime;
    volatile               T_PWM_lpg_offtime                    lpg_offtime;
    volatile          T_PWM_lpg_fifo_pattern               lpg_fifo_pattern;
    volatile               T_PWM_pwl0_config                    pwl0_config;
    volatile               T_PWM_pwl1_config                    pwl1_config;

} T_B;

typedef struct
{
    volatile                             T_B                          ch[4]; /*     , B    , NA        , struct block                  */
    volatile               T_PWM_pwm_out_sel                    pwm_out_sel; /*0x70 , RW   , 0x00000000, No description.               */
} T_HWP_PWM_T;
#else
typedef struct
{
    volatile                uint32                     pwt_config;
    volatile                uint32                     lpg_config;
    volatile                uint32                     lpg_ontime;
    volatile                uint32                    lpg_offtime;
    volatile                uint32               lpg_fifo_pattern;
    volatile                uint32                    pwl0_config;
    volatile                uint32                    pwl1_config;

} T_B;

typedef struct
{
    volatile                             T_B                          ch[4]; /*     , B    , NA        , struct block                  */
    volatile                             U32                    pwm_out_sel; /*0x70 , RW   , 0x00000000, No description.               */
} T_HWP_PWM_T;

#define PWM_PWT_ENABLE                 (1<<0)              

#define PWM_PWT_START                  (1<<1)              

#define PWM_PWT_DUTY(n)                (((n)&0x3FF)<<4)    
#define PWM_PWT_DUTY_MASK              (0x3FF<<4)          
#define PWM_PWT_DUTY_SHIFT             (4)                 

#define PWM_PWT_PERIOD(n)              (((n)&0x7FF)<<16)   
#define PWM_PWT_PERIOD_MASK            (0x7FF<<16)         
#define PWM_PWT_PERIOD_SHIFT           (16)                


#define PWM_LPG_EN                     (1<<0)              

#define PWM_LPG_MODE                   (1<<1)              

#define PWM_LPG_FIFO_FULL              (1<<2)              

#define PWM_LPG_FIFO_EMPTY             (1<<3)              

#define PWM_LPG_FIFO_COUNT(n)          (((n)&0xF)<<4)      
#define PWM_LPG_FIFO_COUNT_MASK        (0xF<<4)            
#define PWM_LPG_FIFO_COUNT_SHIFT       (4)                 

#define PWM_LPG_IRQ_MSK                (1<<8)              


#define PWM_LPG_HIGH_CYCLE(n)          (((n)&0xFFFFFFFF)<<0)
#define PWM_LPG_HIGH_CYCLE_MASK        (0xFFFFFFFF<<0)     
#define PWM_LPG_HIGH_CYCLE_SHIFT       (0)                 


#define PWM_LPG_LOW_CYCLE(n)           (((n)&0xFFFFFFFF)<<0)
#define PWM_LPG_LOW_CYCLE_MASK         (0xFFFFFFFF<<0)     
#define PWM_LPG_LOW_CYCLE_SHIFT        (0)                 


#define PWM_LPG_PATTERN_CYCLE(n)       (((n)&0x7FFFFFFF)<<0)
#define PWM_LPG_PATTERN_CYCLE_MASK     (0x7FFFFFFF<<0)     
#define PWM_LPG_PATTERN_CYCLE_SHIFT    (0)                 

#define PWM_LPG_LEVEL                  (1<<31)             


#define PWM_PWL_MIN(n)                 (((n)&0xFF)<<0)     
#define PWM_PWL_MIN_MASK               (0xFF<<0)           
#define PWM_PWL_MIN_SHIFT              (0)                 

#define PWM_PWL_MAX(n)                 (((n)&0xFF)<<8)     
#define PWM_PWL_MAX_MASK               (0xFF<<8)           
#define PWM_PWL_MAX_SHIFT              (8)                 

#define PWM_PWL0_EN_H                  (1<<16)             

#define PWM_PWL0_FORCE_L               (1<<17)             

#define PWM_PWL0_FORCE_H               (1<<18)             

#define PWM_PWL_PULSE_EN               (1<<19)             

#define PWM_PWL0_SET_OE                (1<<20)             

#define PWM_PWL0_CLR_OE                (1<<21)             

#define PWM_PWL_SET_MUX                (1<<22)             

#define PWM_PWL_CLR_MUX                (1<<23)             

#define PWM_PWL_PULSE_PER(n)           (((n)&0xFF)<<24)    
#define PWM_PWL_PULSE_PER_MASK         (0xFF<<24)          
#define PWM_PWL_PULSE_PER_SHIFT        (24)                


#define PWM_PWL1_THRESHOLD(n)          (((n)&0xFF)<<0)     
#define PWM_PWL1_THRESHOLD_MASK        (0xFF<<0)           
#define PWM_PWL1_THRESHOLD_SHIFT       (0)                 

#define PWM_LFSR_REG(n)                (((n)&0xFF)<<8)     
#define PWM_LFSR_REG_MASK              (0xFF<<8)           
#define PWM_LFSR_REG_SHIFT             (8)                 

#define PWM_PWL1_EN_H                  (1<<16)             

#define PWM_PWL1_FORCE_L               (1<<17)             

#define PWM_PWL1_FORCE_H               (1<<18)             

#define PWM_PWL1_SET_OE                (1<<20)             

#define PWM_PWL1_CLR_OE                (1<<21)             


#define PWM_CH0_SEL(n)                 (((n)&0x3)<<0)      
#define PWM_CH0_SEL_MASK               (0x3<<0)            
#define PWM_CH0_SEL_SHIFT              (0)                 

#define PWM_CH1_SEL(n)                 (((n)&0x3)<<2)      
#define PWM_CH1_SEL_MASK               (0x3<<2)            
#define PWM_CH1_SEL_SHIFT              (2)                 

#define PWM_CH2_SEL(n)                 (((n)&0x3)<<4)      
#define PWM_CH2_SEL_MASK               (0x3<<4)            
#define PWM_CH2_SEL_SHIFT              (4)                 

#define PWM_CH3_SEL(n)                 (((n)&0x3)<<6)      
#define PWM_CH3_SEL_MASK               (0x3<<6)            
#define PWM_CH3_SEL_SHIFT              (6)                 


#endif

#endif

