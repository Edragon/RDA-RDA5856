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


#ifndef _PWM_ASM_H_
#define _PWM_ASM_H_

//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ch_0__pwt_config           REG_PWM_BASE+0x0    
#define REG_ch_0__lpg_config           REG_PWM_BASE+0x4    
#define REG_ch_0__lpg_ontime           REG_PWM_BASE+0x8    
#define REG_ch_0__lpg_offtime          REG_PWM_BASE+0xc    
#define REG_ch_0__lpg_fifo_pattern     REG_PWM_BASE+0x10   
#define REG_ch_0__pwl0_config          REG_PWM_BASE+0x14   
#define REG_ch_0__pwl1_config          REG_PWM_BASE+0x18   
#define REG_ch_1__pwt_config           REG_PWM_BASE+0x1c   
#define REG_ch_1__lpg_config           REG_PWM_BASE+0x20   
#define REG_ch_1__lpg_ontime           REG_PWM_BASE+0x24   
#define REG_ch_1__lpg_offtime          REG_PWM_BASE+0x28   
#define REG_ch_1__lpg_fifo_pattern     REG_PWM_BASE+0x2c   
#define REG_ch_1__pwl0_config          REG_PWM_BASE+0x30   
#define REG_ch_1__pwl1_config          REG_PWM_BASE+0x34   
#define REG_ch_2__pwt_config           REG_PWM_BASE+0x38   
#define REG_ch_2__lpg_config           REG_PWM_BASE+0x3c   
#define REG_ch_2__lpg_ontime           REG_PWM_BASE+0x40   
#define REG_ch_2__lpg_offtime          REG_PWM_BASE+0x44   
#define REG_ch_2__lpg_fifo_pattern     REG_PWM_BASE+0x48   
#define REG_ch_2__pwl0_config          REG_PWM_BASE+0x4c   
#define REG_ch_2__pwl1_config          REG_PWM_BASE+0x50   
#define REG_ch_3__pwt_config           REG_PWM_BASE+0x54   
#define REG_ch_3__lpg_config           REG_PWM_BASE+0x58   
#define REG_ch_3__lpg_ontime           REG_PWM_BASE+0x5c   
#define REG_ch_3__lpg_offtime          REG_PWM_BASE+0x60   
#define REG_ch_3__lpg_fifo_pattern     REG_PWM_BASE+0x64   
#define REG_ch_3__pwl0_config          REG_PWM_BASE+0x68   
#define REG_ch_3__pwl1_config          REG_PWM_BASE+0x6c   
#define REG_pwm_out_sel                REG_PWM_BASE+0x70   

//ch_0__pwt_config
#define PWM_PWT_ENABLE                 (1<<0)              
#define PWM_PWT_START                  (1<<1)              
#define PWM_PWT_DUTY(n)                (((n)&0x3FF)<<4)    
#define PWM_PWT_PERIOD(n)              (((n)&0x7FF)<<16)   

//ch_0__lpg_config
#define PWM_LPG_EN                     (1<<0)              
#define PWM_LPG_MODE                   (1<<1)              
#define PWM_LPG_FIFO_FULL              (1<<2)              
#define PWM_LPG_FIFO_EMPTY             (1<<3)              
#define PWM_LPG_FIFO_COUNT(n)          (((n)&0xF)<<4)      
#define PWM_LPG_IRQ_MSK                (1<<8)              

//ch_0__lpg_ontime
#define PWM_LPG_HIGH_CYCLE(n)          (((n)&0xFFFFFFFF)<<0)

//ch_0__lpg_offtime
#define PWM_LPG_LOW_CYCLE(n)           (((n)&0xFFFFFFFF)<<0)

//ch_0__lpg_fifo_pattern
#define PWM_LPG_PATTERN_CYCLE(n)       (((n)&0x7FFFFFFF)<<0)
#define PWM_LPG_LEVEL                  (1<<31)             

//ch_0__pwl0_config
#define PWM_PWL_MIN(n)                 (((n)&0xFF)<<0)     
#define PWM_PWL_MAX(n)                 (((n)&0xFF)<<8)     
#define PWM_PWL0_EN_H                  (1<<16)             
#define PWM_PWL0_FORCE_L               (1<<17)             
#define PWM_PWL0_FORCE_H               (1<<18)             
#define PWM_PWL_PULSE_EN               (1<<19)             
#define PWM_PWL0_SET_OE                (1<<20)             
#define PWM_PWL0_CLR_OE                (1<<21)             
#define PWM_PWL_SET_MUX                (1<<22)             
#define PWM_PWL_CLR_MUX                (1<<23)             
#define PWM_PWL_PULSE_PER(n)           (((n)&0xFF)<<24)    

//ch_0__pwl1_config
#define PWM_PWL1_THRESHOLD(n)          (((n)&0xFF)<<0)     
#define PWM_LFSR_REG(n)                (((n)&0xFF)<<8)     
#define PWM_PWL1_EN_H                  (1<<16)             
#define PWM_PWL1_FORCE_L               (1<<17)             
#define PWM_PWL1_FORCE_H               (1<<18)             
#define PWM_PWL1_SET_OE                (1<<20)             
#define PWM_PWL1_CLR_OE                (1<<21)             

//ch_1__pwt_config
#define PWM_PWT_ENABLE                 (1<<0)              
#define PWM_PWT_START                  (1<<1)              
#define PWM_PWT_DUTY(n)                (((n)&0x3FF)<<4)    
#define PWM_PWT_PERIOD(n)              (((n)&0x7FF)<<16)   

//ch_1__lpg_config
#define PWM_LPG_EN                     (1<<0)              
#define PWM_LPG_MODE                   (1<<1)              
#define PWM_LPG_FIFO_FULL              (1<<2)              
#define PWM_LPG_FIFO_EMPTY             (1<<3)              
#define PWM_LPG_FIFO_COUNT(n)          (((n)&0xF)<<4)      
#define PWM_LPG_IRQ_MSK                (1<<8)              

//ch_1__lpg_ontime
#define PWM_LPG_HIGH_CYCLE(n)          (((n)&0xFFFFFFFF)<<0)

//ch_1__lpg_offtime
#define PWM_LPG_LOW_CYCLE(n)           (((n)&0xFFFFFFFF)<<0)

//ch_1__lpg_fifo_pattern
#define PWM_LPG_PATTERN_CYCLE(n)       (((n)&0x7FFFFFFF)<<0)
#define PWM_LPG_LEVEL                  (1<<31)             

//ch_1__pwl0_config
#define PWM_PWL_MIN(n)                 (((n)&0xFF)<<0)     
#define PWM_PWL_MAX(n)                 (((n)&0xFF)<<8)     
#define PWM_PWL0_EN_H                  (1<<16)             
#define PWM_PWL0_FORCE_L               (1<<17)             
#define PWM_PWL0_FORCE_H               (1<<18)             
#define PWM_PWL_PULSE_EN               (1<<19)             
#define PWM_PWL0_SET_OE                (1<<20)             
#define PWM_PWL0_CLR_OE                (1<<21)             
#define PWM_PWL_SET_MUX                (1<<22)             
#define PWM_PWL_CLR_MUX                (1<<23)             
#define PWM_PWL_PULSE_PER(n)           (((n)&0xFF)<<24)    

//ch_1__pwl1_config
#define PWM_PWL1_THRESHOLD(n)          (((n)&0xFF)<<0)     
#define PWM_LFSR_REG(n)                (((n)&0xFF)<<8)     
#define PWM_PWL1_EN_H                  (1<<16)             
#define PWM_PWL1_FORCE_L               (1<<17)             
#define PWM_PWL1_FORCE_H               (1<<18)             
#define PWM_PWL1_SET_OE                (1<<20)             
#define PWM_PWL1_CLR_OE                (1<<21)             

//ch_2__pwt_config
#define PWM_PWT_ENABLE                 (1<<0)              
#define PWM_PWT_START                  (1<<1)              
#define PWM_PWT_DUTY(n)                (((n)&0x3FF)<<4)    
#define PWM_PWT_PERIOD(n)              (((n)&0x7FF)<<16)   

//ch_2__lpg_config
#define PWM_LPG_EN                     (1<<0)              
#define PWM_LPG_MODE                   (1<<1)              
#define PWM_LPG_FIFO_FULL              (1<<2)              
#define PWM_LPG_FIFO_EMPTY             (1<<3)              
#define PWM_LPG_FIFO_COUNT(n)          (((n)&0xF)<<4)      
#define PWM_LPG_IRQ_MSK                (1<<8)              

//ch_2__lpg_ontime
#define PWM_LPG_HIGH_CYCLE(n)          (((n)&0xFFFFFFFF)<<0)

//ch_2__lpg_offtime
#define PWM_LPG_LOW_CYCLE(n)           (((n)&0xFFFFFFFF)<<0)

//ch_2__lpg_fifo_pattern
#define PWM_LPG_PATTERN_CYCLE(n)       (((n)&0x7FFFFFFF)<<0)
#define PWM_LPG_LEVEL                  (1<<31)             

//ch_2__pwl0_config
#define PWM_PWL_MIN(n)                 (((n)&0xFF)<<0)     
#define PWM_PWL_MAX(n)                 (((n)&0xFF)<<8)     
#define PWM_PWL0_EN_H                  (1<<16)             
#define PWM_PWL0_FORCE_L               (1<<17)             
#define PWM_PWL0_FORCE_H               (1<<18)             
#define PWM_PWL_PULSE_EN               (1<<19)             
#define PWM_PWL0_SET_OE                (1<<20)             
#define PWM_PWL0_CLR_OE                (1<<21)             
#define PWM_PWL_SET_MUX                (1<<22)             
#define PWM_PWL_CLR_MUX                (1<<23)             
#define PWM_PWL_PULSE_PER(n)           (((n)&0xFF)<<24)    

//ch_2__pwl1_config
#define PWM_PWL1_THRESHOLD(n)          (((n)&0xFF)<<0)     
#define PWM_LFSR_REG(n)                (((n)&0xFF)<<8)     
#define PWM_PWL1_EN_H                  (1<<16)             
#define PWM_PWL1_FORCE_L               (1<<17)             
#define PWM_PWL1_FORCE_H               (1<<18)             
#define PWM_PWL1_SET_OE                (1<<20)             
#define PWM_PWL1_CLR_OE                (1<<21)             

//ch_3__pwt_config
#define PWM_PWT_ENABLE                 (1<<0)              
#define PWM_PWT_START                  (1<<1)              
#define PWM_PWT_DUTY(n)                (((n)&0x3FF)<<4)    
#define PWM_PWT_PERIOD(n)              (((n)&0x7FF)<<16)   

//ch_3__lpg_config
#define PWM_LPG_EN                     (1<<0)              
#define PWM_LPG_MODE                   (1<<1)              
#define PWM_LPG_FIFO_FULL              (1<<2)              
#define PWM_LPG_FIFO_EMPTY             (1<<3)              
#define PWM_LPG_FIFO_COUNT(n)          (((n)&0xF)<<4)      
#define PWM_LPG_IRQ_MSK                (1<<8)              

//ch_3__lpg_ontime
#define PWM_LPG_HIGH_CYCLE(n)          (((n)&0xFFFFFFFF)<<0)

//ch_3__lpg_offtime
#define PWM_LPG_LOW_CYCLE(n)           (((n)&0xFFFFFFFF)<<0)

//ch_3__lpg_fifo_pattern
#define PWM_LPG_PATTERN_CYCLE(n)       (((n)&0x7FFFFFFF)<<0)
#define PWM_LPG_LEVEL                  (1<<31)             

//ch_3__pwl0_config
#define PWM_PWL_MIN(n)                 (((n)&0xFF)<<0)     
#define PWM_PWL_MAX(n)                 (((n)&0xFF)<<8)     
#define PWM_PWL0_EN_H                  (1<<16)             
#define PWM_PWL0_FORCE_L               (1<<17)             
#define PWM_PWL0_FORCE_H               (1<<18)             
#define PWM_PWL_PULSE_EN               (1<<19)             
#define PWM_PWL0_SET_OE                (1<<20)             
#define PWM_PWL0_CLR_OE                (1<<21)             
#define PWM_PWL_SET_MUX                (1<<22)             
#define PWM_PWL_CLR_MUX                (1<<23)             
#define PWM_PWL_PULSE_PER(n)           (((n)&0xFF)<<24)    

//ch_3__pwl1_config
#define PWM_PWL1_THRESHOLD(n)          (((n)&0xFF)<<0)     
#define PWM_LFSR_REG(n)                (((n)&0xFF)<<8)     
#define PWM_PWL1_EN_H                  (1<<16)             
#define PWM_PWL1_FORCE_L               (1<<17)             
#define PWM_PWL1_FORCE_H               (1<<18)             
#define PWM_PWL1_SET_OE                (1<<20)             
#define PWM_PWL1_CLR_OE                (1<<21)             

//pwm_out_sel
#define PWM_CH0_SEL(n)                 (((n)&0x3)<<0)      
#define PWM_CH1_SEL(n)                 (((n)&0x3)<<2)      
#define PWM_CH2_SEL(n)                 (((n)&0x3)<<4)      
#define PWM_CH3_SEL(n)                 (((n)&0x3)<<6)      


#endif


