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
// File name      : audio_codec_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:32                                                  
// Last modified  : 2016-12-21 13:55:32                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:32 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AUDIO_CODEC_ASM_H__
#define __AUDIO_CODEC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_audio_function_id          REG_AUDIO_CODEC_BASE+0x00
#define REG_codec_cfg                  REG_AUDIO_CODEC_BASE+0x04
#define REG_adc_clk_cfg                REG_AUDIO_CODEC_BASE+0x08
#define REG_adc_pathcfg                REG_AUDIO_CODEC_BASE+0x0C
#define REG_dac_clk_cfg                REG_AUDIO_CODEC_BASE+0x10
#define REG_dac_path_cfg1              REG_AUDIO_CODEC_BASE+0x14
#define REG_dac_path_cfg2              REG_AUDIO_CODEC_BASE+0x18
#define REG_dac_ramp                   REG_AUDIO_CODEC_BASE+0x1C
#define REG_side_tone_cfg              REG_AUDIO_CODEC_BASE+0x20
#define REG_test_ctrl                  REG_AUDIO_CODEC_BASE+0x24
#define REG_test_port_lo               REG_AUDIO_CODEC_BASE+0x28
#define REG_test_port_hi               REG_AUDIO_CODEC_BASE+0x2C
#define REG_eq_ctrl                    REG_AUDIO_CODEC_BASE+0x30
#define REG_eq_coef0_low               REG_AUDIO_CODEC_BASE+0x34
#define REG_eq_coef0_high              REG_AUDIO_CODEC_BASE+0x38
#define REG_eq_coef1_low               REG_AUDIO_CODEC_BASE+0x3C
#define REG_eq_coef1_high              REG_AUDIO_CODEC_BASE+0x40
#define REG_eq_coef2_low               REG_AUDIO_CODEC_BASE+0x44
#define REG_eq_coef2_high              REG_AUDIO_CODEC_BASE+0x48
#define REG_eq_coef3_low               REG_AUDIO_CODEC_BASE+0x4C
#define REG_eq_coef3_high              REG_AUDIO_CODEC_BASE+0x50
#define REG_eq_coef4_low               REG_AUDIO_CODEC_BASE+0x54
#define REG_eq_coef4_high              REG_AUDIO_CODEC_BASE+0x58
#define REG_eq_coef5_low               REG_AUDIO_CODEC_BASE+0x5C
#define REG_eq_coef5_high              REG_AUDIO_CODEC_BASE+0x60
#define REG_eq_coef6_low               REG_AUDIO_CODEC_BASE+0x64
#define REG_eq_coef6_high              REG_AUDIO_CODEC_BASE+0x68
#define REG_eq_coef7_low               REG_AUDIO_CODEC_BASE+0x6C
#define REG_eq_coef7_high              REG_AUDIO_CODEC_BASE+0x70
#define REG_eq_coef8_low               REG_AUDIO_CODEC_BASE+0x74
#define REG_eq_coef8_high              REG_AUDIO_CODEC_BASE+0x78
#define REG_eq_coef9_low               REG_AUDIO_CODEC_BASE+0x7C
#define REG_eq_coef9_high              REG_AUDIO_CODEC_BASE+0x80
#define REG_eq_coef10_low              REG_AUDIO_CODEC_BASE+0x84
#define REG_eq_coef10_high             REG_AUDIO_CODEC_BASE+0x88
#define REG_eq_coef11_low              REG_AUDIO_CODEC_BASE+0x8C
#define REG_eq_coef11_high             REG_AUDIO_CODEC_BASE+0x90
#define REG_eq_coef12_low              REG_AUDIO_CODEC_BASE+0x94
#define REG_eq_coef12_high             REG_AUDIO_CODEC_BASE+0x98
#define REG_eq_coef13_low              REG_AUDIO_CODEC_BASE+0x9C
#define REG_eq_coef13_high             REG_AUDIO_CODEC_BASE+0xA0
#define REG_eq_coef14_low              REG_AUDIO_CODEC_BASE+0xA4
#define REG_eq_coef14_high             REG_AUDIO_CODEC_BASE+0xA8
#define REG_eq_coef15_low              REG_AUDIO_CODEC_BASE+0xAC
#define REG_eq_coef15_high             REG_AUDIO_CODEC_BASE+0xB0
#define REG_eq_coef16_low              REG_AUDIO_CODEC_BASE+0xB4
#define REG_eq_coef16_high             REG_AUDIO_CODEC_BASE+0xB8
#define REG_eq_coef17_low              REG_AUDIO_CODEC_BASE+0xBC
#define REG_eq_coef17_high             REG_AUDIO_CODEC_BASE+0xC0
#define REG_eq_coef18_low              REG_AUDIO_CODEC_BASE+0xC4
#define REG_eq_coef18_high             REG_AUDIO_CODEC_BASE+0xC8
#define REG_eq_coef19_low              REG_AUDIO_CODEC_BASE+0xCC
#define REG_eq_coef19_high             REG_AUDIO_CODEC_BASE+0xD0
#define REG_eq_coef20_low              REG_AUDIO_CODEC_BASE+0xD4
#define REG_eq_coef20_high             REG_AUDIO_CODEC_BASE+0xD8
#define REG_eq_coef21_low              REG_AUDIO_CODEC_BASE+0xDC
#define REG_eq_coef21_high             REG_AUDIO_CODEC_BASE+0xE0
#define REG_eq_coef22_low              REG_AUDIO_CODEC_BASE+0xE4
#define REG_eq_coef22_high             REG_AUDIO_CODEC_BASE+0xE8
#define REG_eq_coef23_low              REG_AUDIO_CODEC_BASE+0xEC
#define REG_eq_coef23_high             REG_AUDIO_CODEC_BASE+0xF0
#define REG_eq_coef24_low              REG_AUDIO_CODEC_BASE+0xF4
#define REG_eq_coef24_high             REG_AUDIO_CODEC_BASE+0xF8
#define REG_eq_coef25_low              REG_AUDIO_CODEC_BASE+0xFC
#define REG_eq_coef25_high             REG_AUDIO_CODEC_BASE+0x100
#define REG_eq_coef26_low              REG_AUDIO_CODEC_BASE+0x104
#define REG_eq_coef26_high             REG_AUDIO_CODEC_BASE+0x108
#define REG_eq_coef27_low              REG_AUDIO_CODEC_BASE+0x10C
#define REG_eq_coef27_high             REG_AUDIO_CODEC_BASE+0x110
#define REG_eq_coef28_low              REG_AUDIO_CODEC_BASE+0x114
#define REG_eq_coef28_high             REG_AUDIO_CODEC_BASE+0x118
#define REG_eq_coef29_low              REG_AUDIO_CODEC_BASE+0x11C
#define REG_eq_coef29_high             REG_AUDIO_CODEC_BASE+0x120
#define REG_eq_coef30_low              REG_AUDIO_CODEC_BASE+0x124
#define REG_eq_coef30_high             REG_AUDIO_CODEC_BASE+0x128
#define REG_eq_coef31_low              REG_AUDIO_CODEC_BASE+0x12C
#define REG_eq_coef31_high             REG_AUDIO_CODEC_BASE+0x130
#define REG_eq_coef32_low              REG_AUDIO_CODEC_BASE+0x134
#define REG_eq_coef32_high             REG_AUDIO_CODEC_BASE+0x138
#define REG_eq_coef33_low              REG_AUDIO_CODEC_BASE+0x13C
#define REG_eq_coef33_high             REG_AUDIO_CODEC_BASE+0x140
#define REG_eq_coef34_low              REG_AUDIO_CODEC_BASE+0x144
#define REG_eq_coef34_high             REG_AUDIO_CODEC_BASE+0x148
#define REG_coef_a1_lo                 REG_AUDIO_CODEC_BASE+0x14C
#define REG_coef_a1_hi                 REG_AUDIO_CODEC_BASE+0x150
#define REG_coef_a2_lo                 REG_AUDIO_CODEC_BASE+0x154
#define REG_coef_a2_hi                 REG_AUDIO_CODEC_BASE+0x158
#define REG_coef_b1_lo                 REG_AUDIO_CODEC_BASE+0x15C
#define REG_coef_b1_hi                 REG_AUDIO_CODEC_BASE+0x160

//audio_function_id
#define AUDIO_CODEC_FID(n)             (((n)&0xFFFF)<<0)   

//codec_cfg
#define AUDIO_CODEC_S_ISPHONE          (1<<0)              
#define AUDIO_CODEC_S_PHONE_SEL        (1<<1)              
#define AUDIO_CODEC_S_LOOP             (1<<2)              
#define AUDIO_CODEC_S_DAC_1K           (1<<3)              
#define AUDIO_CODEC_FM_MODE            (1<<4)              
#define AUDIO_CODEC_SLEEP_MODE         (1<<5)              
#define AUDIO_CODEC_S_CODEC_EN         (1<<6)              

//adc_clk_cfg
#define AUDIO_CODEC_ADC_CLK_LD         (1<<0)              
#define AUDIO_CODEC_CNT_ADC_CLK_INT(n) (((n)&0x7F)<<1)     

//adc_pathcfg
#define AUDIO_CODEC_ADC_CLKGEN_MODE    (1<<0)              
#define AUDIO_CODEC_ANA_ADC_CLK_INV    (1<<1)              
#define AUDIO_CODEC_S_ADC_OSR_SEL(n)   (((n)&0x7)<<2)      
#define AUDIO_CODEC_ADC_LPF_BYPASS     (1<<5)              
#define AUDIO_CODEC_S_HPF_BYPASS       (1<<6)              
#define AUDIO_CODEC_S_ADC_VOLUME(n)    (((n)&0xF)<<7)      
#define AUDIO_CODEC_ADC_DIG_SEL        (1<<11)             
#define AUDIO_CODEC_DIG_MIC_SWITCH     (1<<12)             
#define AUDIO_CODEC_ADC_PATH_RESET     (1<<13)             
#define AUDIO_CODEC_S_ADC_EN           (1<<14)             
#define AUDIO_CODEC_ADC_OUT_FORMAT     (1<<15)             

//dac_clk_cfg
#define AUDIO_CODEC_DAC_CLK_LD         (1<<0)              
#define AUDIO_CODEC_FS_CNT_INT(n)      (((n)&0x3FF)<<1)    

//dac_path_cfg1
#define AUDIO_CODEC_S_DAC_VOLUME_MUTE  (1<<0)              
#define AUDIO_CODEC_S_MUTE_R           (1<<1)              
#define AUDIO_CODEC_S_MUTE_L           (1<<2)              
#define AUDIO_CODEC_S_DAC_VOLUME(n)    (((n)&0x3F)<<3)     
#define AUDIO_CODEC_S_DAC_GAIN(n)      (((n)&0x3)<<9)      
#define AUDIO_CODEC_S_DAC_OSR_SEL(n)   (((n)&0x3)<<11)     
#define AUDIO_CODEC_DAC_PATH_RESET     (1<<13)             
#define AUDIO_CODEC_S_DAC_EN           (1<<14)             

//dac_path_cfg2
#define AUDIO_CODEC_S_FS_SEL(n)        (((n)&0x7)<<0)      
#define AUDIO_CODEC_SDM_GAIN(n)        (((n)&0x3)<<3)      
#define AUDIO_CODEC_DITHER_GAIN_2_0(n) (((n)&0x7)<<5)      
#define AUDIO_CODEC_DITHER_GAIN_4_3(n) (((n)&0x3)<<8)      
#define AUDIO_CODEC_S_DITHER_BYPASS    (1<<10)             
#define AUDIO_CODEC_S_DWA_EN           (1<<11)             
#define AUDIO_CODEC_DAC_CLKGEN_MODE    (1<<12)             
#define AUDIO_CODEC_ANA_DAC_CLK_INV    (1<<13)             

//dac_ramp
#define AUDIO_CODEC_RAMPDN_STEP(n)     (((n)&0x7F)<<0)     
#define AUDIO_CODEC_RAMPUP_STEP(n)     (((n)&0x7F)<<7)     
#define AUDIO_CODEC_RAMP_BYPASS        (1<<14)             
#define AUDIO_CODEC_RAMP_IND           (1<<15)             

//side_tone_cfg
#define AUDIO_CODEC_S_ADC_DVST_CTL_3_0(n) (((n)&0xF)<<0)      
#define AUDIO_CODEC_S_ADC_DVST_CTL_4   (1<<4)              

//test_ctrl
#define AUDIO_CODEC_TPORTS_SEL(n)      (((n)&0x1F)<<0)     
#define AUDIO_CODEC_TPORTS_EN          (1<<5)              
#define AUDIO_CODEC_TPORTS_OUT_SEL     (1<<6)              

//test_port_lo
#define AUDIO_CODEC_TPORTS_LO(n)       (((n)&0xFFFF)<<0)   

//test_port_hi
#define AUDIO_CODEC_TPORTS_HI(n)       (((n)&0xFFFF)<<0)   

//eq_ctrl
#define AUDIO_CODEC_EQ_RESET           (1<<0)              
#define AUDIO_CODEC_EQ_BYPASS          (1<<1)              

//eq_coef0_low
#define AUDIO_CODEC_EQ_COEF0_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef0_high
#define AUDIO_CODEC_EQ_COEF0_HI(n)     (((n)&0xFF)<<0)     

//eq_coef1_low
#define AUDIO_CODEC_EQ_COEF1_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef1_high
#define AUDIO_CODEC_EQ_COEF1_HI(n)     (((n)&0xFF)<<0)     

//eq_coef2_low
#define AUDIO_CODEC_EQ_COEF2_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef2_high
#define AUDIO_CODEC_EQ_COEF2_HI(n)     (((n)&0xFF)<<0)     

//eq_coef3_low
#define AUDIO_CODEC_EQ_COEF3_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef3_high
#define AUDIO_CODEC_EQ_COEF3_HI(n)     (((n)&0xFF)<<0)     

//eq_coef4_low
#define AUDIO_CODEC_EQ_COEF4_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef4_high
#define AUDIO_CODEC_EQ_COEF4_HI(n)     (((n)&0xFF)<<0)     

//eq_coef5_low
#define AUDIO_CODEC_EQ_COEF5_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef5_high
#define AUDIO_CODEC_EQ_COEF5_HI(n)     (((n)&0xFF)<<0)     

//eq_coef6_low
#define AUDIO_CODEC_EQ_COEF6_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef6_high
#define AUDIO_CODEC_EQ_COEF6_HI(n)     (((n)&0xFF)<<0)     

//eq_coef7_low
#define AUDIO_CODEC_EQ_COEF7_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef7_high
#define AUDIO_CODEC_EQ_COEF7_HI(n)     (((n)&0xFF)<<0)     

//eq_coef8_low
#define AUDIO_CODEC_EQ_COEF8_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef8_high
#define AUDIO_CODEC_EQ_COEF8_HI(n)     (((n)&0xFF)<<0)     

//eq_coef9_low
#define AUDIO_CODEC_EQ_COEF9_LO(n)     (((n)&0xFFFF)<<0)   

//eq_coef9_high
#define AUDIO_CODEC_EQ_COEF9_HI(n)     (((n)&0xFF)<<0)     

//eq_coef10_low
#define AUDIO_CODEC_EQ_COEF10_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef10_high
#define AUDIO_CODEC_EQ_COEF10_HI(n)    (((n)&0xFF)<<0)     

//eq_coef11_low
#define AUDIO_CODEC_EQ_COEF11_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef11_high
#define AUDIO_CODEC_EQ_COEF11_HI(n)    (((n)&0xFF)<<0)     

//eq_coef12_low
#define AUDIO_CODEC_EQ_COEF12_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef12_high
#define AUDIO_CODEC_EQ_COEF12_HI(n)    (((n)&0xFF)<<0)     

//eq_coef13_low
#define AUDIO_CODEC_EQ_COEF13_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef13_high
#define AUDIO_CODEC_EQ_COEF13_HI(n)    (((n)&0xFF)<<0)     

//eq_coef14_low
#define AUDIO_CODEC_EQ_COEF14_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef14_high
#define AUDIO_CODEC_EQ_COEF14_HI(n)    (((n)&0xFF)<<0)     

//eq_coef15_low
#define AUDIO_CODEC_EQ_COEF15_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef15_high
#define AUDIO_CODEC_EQ_COEF15_HI(n)    (((n)&0xFF)<<0)     

//eq_coef16_low
#define AUDIO_CODEC_EQ_COEF16_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef16_high
#define AUDIO_CODEC_EQ_COEF16_HI(n)    (((n)&0xFF)<<0)     

//eq_coef17_low
#define AUDIO_CODEC_EQ_COEF17_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef17_high
#define AUDIO_CODEC_EQ_COEF17_HI(n)    (((n)&0xFF)<<0)     

//eq_coef18_low
#define AUDIO_CODEC_EQ_COEF18_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef18_high
#define AUDIO_CODEC_EQ_COEF18_HI(n)    (((n)&0xFF)<<0)     

//eq_coef19_low
#define AUDIO_CODEC_EQ_COEF19_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef19_high
#define AUDIO_CODEC_EQ_COEF19_HI(n)    (((n)&0xFF)<<0)     

//eq_coef20_low
#define AUDIO_CODEC_EQ_COEF20_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef20_high
#define AUDIO_CODEC_EQ_COEF20_HI(n)    (((n)&0xFF)<<0)     

//eq_coef21_low
#define AUDIO_CODEC_EQ_COEF21_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef21_high
#define AUDIO_CODEC_EQ_COEF21_HI(n)    (((n)&0xFF)<<0)     

//eq_coef22_low
#define AUDIO_CODEC_EQ_COEF22_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef22_high
#define AUDIO_CODEC_EQ_COEF22_HI(n)    (((n)&0xFF)<<0)     

//eq_coef23_low
#define AUDIO_CODEC_EQ_COEF23_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef23_high
#define AUDIO_CODEC_EQ_COEF23_HI(n)    (((n)&0xFF)<<0)     

//eq_coef24_low
#define AUDIO_CODEC_EQ_COEF24_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef24_high
#define AUDIO_CODEC_EQ_COEF24_HI(n)    (((n)&0xFF)<<0)     

//eq_coef25_low
#define AUDIO_CODEC_EQ_COEF25_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef25_high
#define AUDIO_CODEC_EQ_COEF25_HI(n)    (((n)&0xFF)<<0)     

//eq_coef26_low
#define AUDIO_CODEC_EQ_COEF26_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef26_high
#define AUDIO_CODEC_EQ_COEF26_HI(n)    (((n)&0xFF)<<0)     

//eq_coef27_low
#define AUDIO_CODEC_EQ_COEF27_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef27_high
#define AUDIO_CODEC_EQ_COEF27_HI(n)    (((n)&0xFF)<<0)     

//eq_coef28_low
#define AUDIO_CODEC_EQ_COEF28_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef28_high
#define AUDIO_CODEC_EQ_COEF28_HI(n)    (((n)&0xFF)<<0)     

//eq_coef29_low
#define AUDIO_CODEC_EQ_COEF29_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef29_high
#define AUDIO_CODEC_EQ_COEF29_HI(n)    (((n)&0xFF)<<0)     

//eq_coef30_low
#define AUDIO_CODEC_EQ_COEF30_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef30_high
#define AUDIO_CODEC_EQ_COEF30_HI(n)    (((n)&0xFF)<<0)     

//eq_coef31_low
#define AUDIO_CODEC_EQ_COEF31_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef31_high
#define AUDIO_CODEC_EQ_COEF31_HI(n)    (((n)&0xFF)<<0)     

//eq_coef32_low
#define AUDIO_CODEC_EQ_COEF32_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef32_high
#define AUDIO_CODEC_EQ_COEF32_HI(n)    (((n)&0xFF)<<0)     

//eq_coef33_low
#define AUDIO_CODEC_EQ_COEF33_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef33_high
#define AUDIO_CODEC_EQ_COEF33_HI(n)    (((n)&0xFF)<<0)     

//eq_coef34_low
#define AUDIO_CODEC_EQ_COEF34_LO(n)    (((n)&0xFFFF)<<0)   

//eq_coef34_high
#define AUDIO_CODEC_EQ_COEF34_HI(n)    (((n)&0xFF)<<0)     

//coef_a1_lo
#define AUDIO_CODEC_HPF_COEF_A1_LO(n)  (((n)&0xFFFF)<<0)   

//coef_a1_hi
#define AUDIO_CODEC_HPF_COEF_A1_HI(n)  (((n)&0xF)<<0)      

//coef_a2_lo
#define AUDIO_CODEC_HPF_COEF_A2_LO(n)  (((n)&0xFFFF)<<0)   

//coef_a2_hi
#define AUDIO_CODEC_HPF_COEF_A2_HI(n)  (((n)&0xF)<<0)      

//coef_b1_lo
#define AUDIO_CODEC_HPF_COEF_B1_LO(n)  (((n)&0xFFFF)<<0)   

//coef_b1_hi
#define AUDIO_CODEC_HPF_COEF_B1_HI(n)  (((n)&0xF)<<0)      


#endif


