//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : abb_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-05 14:08:44                                                  
// Last modified  : 2016-12-05 14:08:44                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-05 14:08:44 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __ABB_ASM_H__
#define __ABB_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ID                         REG_ABB_BASE+0x00   
#define REG_codec_cfg0                 REG_ABB_BASE+0x04   
#define REG_codec_cfg1                 REG_ABB_BASE+0x08   
#define REG_ADC_Gain                   REG_ABB_BASE+0x0C   
#define REG_DAC_Gain                   REG_ABB_BASE+0x10   
#define REG_Codec_Mode0                REG_ABB_BASE+0x14   
#define REG_Codec_Mode1                REG_ABB_BASE+0x18   
#define REG_Reset_Ctrl                 REG_ABB_BASE+0x1C   
#define REG_Power_Ctrl                 REG_ABB_BASE+0x20   
#define REG_vref_cal                   REG_ABB_BASE+0x24   
#define REG_vrefc_cal                  REG_ABB_BASE+0x28   
#define REG_vadc_cal                   REG_ABB_BASE+0x2C   
#define REG_vdac_cal                   REG_ABB_BASE+0x30   
#define REG_aupll_setting1             REG_ABB_BASE+0x34   
#define REG_aupll_setting2             REG_ABB_BASE+0x38   
#define REG_aupll_setting3             REG_ABB_BASE+0x3C   
#define REG_sdmsetting                 REG_ABB_BASE+0x40   
#define REG_sdmfreq1                   REG_ABB_BASE+0x44   
#define REG_sdmfreq2                   REG_ABB_BASE+0x48   
#define REG_clk_doubler_cfg            REG_ABB_BASE+0x4C   
#define REG_usb_phy_ctrl               REG_ABB_BASE+0x50   
#define REG_clk_chop_div               REG_ABB_BASE+0x54   
#define REG_abb_rsvd_status            REG_ABB_BASE+0x58   
#define REG_abb_rsvd_cfg               REG_ABB_BASE+0x5C   
#define REG_hp_chg_cfg                 REG_ABB_BASE+0x60   
#define REG_denoise_pwr_cfg            REG_ABB_BASE+0x64   
#define REG_denoise_elimination_dac_clk_cfg REG_ABB_BASE+0x68   
#define REG_denoise_adc_clk_cfg        REG_ABB_BASE+0x6C   
#define REG_denoise_ramp_cfg_l         REG_ABB_BASE+0x70   
#define REG_denoise_rampup_cfg_l       REG_ABB_BASE+0x74   
#define REG_denoise_rampdown_cfg_l     REG_ABB_BASE+0x78   
#define REG_denoise_ramp_cfg_r         REG_ABB_BASE+0x7C   
#define REG_denoise_rampup_cfg_r       REG_ABB_BASE+0x80   
#define REG_denoise_rampdown_cfg_r     REG_ABB_BASE+0x84   
#define REG_denoise_other_cfg          REG_ABB_BASE+0x88   
#define REG_ResetControl               REG_ABB_BASE+0x8C   

//ID
#define ABB_REVID(n)                   (((n)&0xF)<<0)      
#define ABB_CHIPID(n)                  (((n)&0xFFF)<<4)    

//codec_cfg0
#define ABB_IBIAS_BIT(n)               (((n)&0x3)<<0)      
#define ABB_VCOMBUF_SEL(n)             (((n)&0xF)<<2)      
#define ABB_VCOM_MODE(n)               (((n)&0x7)<<6)      
#define ABB_VMIC_BIT(n)                (((n)&0x3)<<9)      
#define ABB_SEL_BG                     (1<<11)             
#define ABB_DAC_VOS_BIT(n)             (((n)&0xF)<<12)     

//codec_cfg1
#define ABB_DAC_CLK_INV_SEL            (1<<0)              
#define ABB_DAC_VREG_BIT(n)            (((n)&0x7)<<1)      
#define ABB_ADC_CLK_INV_SEL            (1<<4)              
#define ABB_ADC_IBIT(n)                (((n)&0x3)<<5)      
#define ABB_MICPGA_R_IBIT(n)           (((n)&0x3)<<7)      
#define ABB_MICPGA_L_IBIT(n)           (((n)&0x3)<<9)      
#define ABB_MICPGA_R_CHOP_EN           (1<<11)             
#define ABB_MICPGA_L_CHOP_EN           (1<<12)             

//ADC_Gain
#define ABB_ADC_R_GAIN_BIT(n)          (((n)&0x7)<<0)      
#define ABB_ADC_L_GAIN_BIT(n)          (((n)&0x7)<<3)      
#define ABB_MICPGA_R_GAIN_BIT(n)       (((n)&0x3)<<6)      
#define ABB_MICPGA_L_GAIN_BIT(n)       (((n)&0x3)<<8)      

//DAC_Gain
#define ABB_HP_GAIN_BIT(n)             (((n)&0x7)<<0)      
#define ABB_MUX_GAIN_BIT(n)            (((n)&0x3)<<3)      
#define ABB_DAC_GAIN_BIT(n)            (((n)&0xF)<<5)      

//Codec_Mode0
#define ABB_MICINPUT_R_SEL(n)          (((n)&0xF)<<0)      
#define ABB_MICINPUT_L_SEL(n)          (((n)&0xF)<<4)      
#define ABB_HP_INPUTSEL(n)             (((n)&0x3)<<8)      
#define ABB_MUX_IBIT(n)                (((n)&0x3)<<10)     
#define ABB_HP_IBIT(n)                 (((n)&0x3)<<12)     

//Codec_Mode1
#define ABB_ADC_SMPLRATE_SEL(n)        (((n)&0x3)<<0)      
#define ABB_ADC_LV_MODE                (1<<2)              
#define ABB_IVREF_LV_MODE              (1<<3)              
#define ABB_HP_DIFF_MODE               (1<<4)              
#define ABB_LINEIN_MODE                (1<<5)              
#define ABB_DAC_R_MODE                 (1<<6)              
#define ABB_DAC_L_MODE                 (1<<7)              
#define ABB_MICLOOP_R_MODE             (1<<8)              
#define ABB_MICLOOP_L_MODE             (1<<9)              

//Reset_Ctrl
#define ABB_RESET_HP                   (1<<0)              
#define ABB_RESET_MUX_R                (1<<1)              
#define ABB_RESET_MUX_L                (1<<2)              
#define ABB_RESET_ADC                  (1<<3)              
#define ABB_RESET_MICPGA_R             (1<<4)              
#define ABB_RESET_MICPGA_L             (1<<5)              

//Power_Ctrl
#define ABB_PU_MUX_R                   (1<<0)              
#define ABB_PU_MUX_L                   (1<<1)              
#define ABB_PU_DAC_R                   (1<<2)              
#define ABB_PU_DAC_L                   (1<<3)              
#define ABB_PU_ADC_R                   (1<<4)              
#define ABB_PU_ADC_L                   (1<<5)              
#define ABB_PU_MICPGA_R                (1<<6)              
#define ABB_PU_MICPGA_L                (1<<7)              
#define ABB_PU_VMIC                    (1<<8)              
#define ABB_PU_VREFLN                  (1<<9)              
#define ABB_PU_VCOMBUF_C               (1<<10)             
#define ABB_PU_VCOMBUF_B               (1<<11)             
#define ABB_PU_IVREF                   (1<<12)             
#define ABB_PU_ALL                     (1<<13)             

//vref_cal
#define ABB_VREF_CAL_BIT_REG(n)        (((n)&0x7)<<0)      
#define ABB_VREF_CAL_BIT_DR            (1<<3)              
#define ABB_VREF_CAL_CLK_INV           (1<<4)              
#define ABB_VREF_CAL_OUT_POLARITY      (1<<5)              
#define ABB_RESETN_VREF_CAL_REG        (1<<6)              
#define ABB_RESETN_VREF_CAL_DR         (1<<7)              
#define ABB_VREF_CAL_READY             (1<<8)              

//vrefc_cal
#define ABB_VREFC_CAL_BIT_REG(n)       (((n)&0x7)<<0)      
#define ABB_VREFC_CAL_BIT_DR           (1<<3)              
#define ABB_VREFC_CAL_CLK_INV          (1<<4)              
#define ABB_VREFC_CAL_OUT_POLARITY     (1<<5)              
#define ABB_RESETN_VREFC_CAL_REG       (1<<6)              
#define ABB_RESETN_VREFC_CAL_DR        (1<<7)              
#define ABB_VREFC_CAL_READY            (1<<8)              

//vadc_cal
#define ABB_VADC_CAL_BIT_REG(n)        (((n)&0xF)<<0)      
#define ABB_VADC_CAL_BIT_DR            (1<<4)              
#define ABB_VADC_CAL_CLK_INV           (1<<5)              
#define ABB_VADC_CAL_OUT_POLARITY      (1<<6)              
#define ABB_RESETN_VADC_CAL_REG        (1<<7)              
#define ABB_RESETN_VADC_CAL_DR         (1<<8)              
#define ABB_VADC_CAL_READY             (1<<9)              

//vdac_cal
#define ABB_VDAC_CAL_BIT_REG(n)        (((n)&0x7)<<0)      
#define ABB_VDAC_CAL_BIT_DR            (1<<3)              
#define ABB_VDAC_CAL_CLK_INV           (1<<4)              
#define ABB_VDAC_CAL_OUT_POLARITY      (1<<5)              
#define ABB_RESETN_VDAC_CAL_REG        (1<<6)              
#define ABB_RESETN_VDAC_CAL_DR         (1<<7)              
#define ABB_VDAC_CAL_READY             (1<<8)              

//aupll_setting1
#define ABB_AUPLL_SDM_CLK_SEL          (1<<0)              
#define ABB_AUPLL_REFMULTI2_EN         (1<<1)              
#define ABB_AUPLL_PCON_MODE            (1<<2)              
#define ABB_AUPLL_CPR2_BIT(n)          (((n)&0x7)<<3)      
#define ABB_AUPLL_CPC2_BIT(n)          (((n)&0x7)<<6)      
#define ABB_AUPLL_VCO_REG_BIT(n)       (((n)&0xF)<<9)      

//aupll_setting2
#define ABB_PU_AUPLL                   (1<<0)              
#define ABB_AUPLL_RSTB                 (1<<1)              
#define ABB_AUPLL_VCO_HIGH_TEST        (1<<2)              
#define ABB_AUPLL_VCO_LOW_TEST         (1<<3)              
#define ABB_AUPLL_CLKOUT_EN(n)         (((n)&0x7)<<4)      
#define ABB_AUPLL_SDM_CLK_TEST_EN      (1<<7)              
#define ABB_AUPLL_TEST_EN              (1<<8)              
#define ABB_AUPLL_READY_TIMER(n)       (((n)&0x3)<<9)      

//aupll_setting3
#define ABB_AUPLL_DIG_REG_RES_MODE_VBIT(n) (((n)&0x7)<<0)      
#define ABB_AUPLL_CPBIAS_BIT(n)        (((n)&0x7)<<3)      
#define ABB_AUPLL_DIG_REG_ROUT_BIT(n)  (((n)&0xF)<<6)      
#define ABB_AUPLL_DIG_REG_RANGE_BIT(n) (((n)&0xF)<<10)     
#define ABB_AUPLL_LOCK                 (1<<14)             
#define ABB_AUPLL_READY                (1<<15)             

//sdmsetting
#define ABB_SDM_DITHER_BYPASS          (1<<0)              
#define ABB_SDM_INT_DEC_SEL(n)         (((n)&0x7)<<1)      
#define ABB_SDM_RESETN                 (1<<4)              
#define ABB_SDM_CLK_INV                (1<<5)              
#define ABB_REFDIV2_EN                 (1<<6)              

//sdmfreq1
#define ABB_SDM_FREQ_HIGH(n)           (((n)&0xFFFF)<<0)   

//sdmfreq2
#define ABB_SDM_FREQ_LOW(n)            (((n)&0xFFFF)<<0)   

//clk_doubler_cfg
#define ABB_PU_CLKDBLR                 (1<<0)              
#define ABB_CLKDBLR_IBIAS_CTRL         (1<<1)              
#define ABB_CLKDBLR_VBIAS_CTRL(n)      (((n)&0x3)<<2)      
#define ABB_CLKDBLR_R_CTRL(n)          (((n)&0x7)<<4)      

//usb_phy_ctrl
#define ABB_PU_USB                     (1<<0)              
#define ABB_USB_VREG_BIT(n)            (((n)&0x7)<<1)      
#define ABB_USB_CTRL(n)                (((n)&0x7)<<4)      

//clk_chop_div
#define ABB_CLK_CHOP_EN                (1<<0)              
#define ABB_CLK_CHOP_DIV_BASE(n)       (((n)&0x3FF)<<1)    

//abb_rsvd_status
#define ABB_ABB_STATUS(n)              (((n)&0xFF)<<0)     
#define ABB_ABB_CFG_BAK(n)             (((n)&0xFF)<<8)     

//abb_rsvd_cfg
#define ABB_ABB_CFG(n)                 (((n)&0xFFFF)<<0)   
#define VCOMBUF_CAPLESS_HP_DOUBLEPATH_EN   (1<<0) 
#define HP_L_CHG_RES_NPART_EN            (1<<1)
#define HP_R_CAPLESS_MODE                   (1<<2)
#define HP_L_CAPLESS_MODE                   (1<<3)
#define HP_L_DISCHG_RES_NPART_EN     (1<<4)
#define HP_GAIN_DOUBLE_EN                   (1<<5)
#define HP_R_CHG_RES_EN                       (1<<6)
#define HP_R_SOFT_START_EN                   (1<<7)
#define HP_R_DIFF_MODE                         (1<<8)
#define HP_R_CHG_RES_NAPRT_EN           (1<<9)
#define HP_R_DISCHG_RES_NPART_EN      (1<<10)
#define VCOMBUF_CAPLESS_HP_FB_SEL(n)(((n)&0x3)<<11)  
#define VCOMBUF_CAPLESS_HP_IBIT(n)   (((n)&0x3)<<13)  
#define PU_VCOMBUF_CAPLESS_HP            (1<<15)

//hp_chg_cfg
#define ABB_HP_CHG_RES_EN              (1<<0)              
#define ABB_HP_L_CHG_RES_BIT(n)          (((n)&0x3)<<1)      

//denoise_pwr_cfg
#define ABB_HP_DAC_CHG_IBIT(n)         (((n)&0xF)<<0)      
#define ABB_PU_HP_R_DAC_CHG            (1<<4)              
#define ABB_PU_HP_L_DAC_CHG            (1<<5)              
#define ABB_PU_ADC_DET_R               (1<<6)              
#define ABB_PU_ADC_DET_L               (1<<7)              
#define ABB_PU_HP_R_BIAS               (1<<8)              
#define ABB_PU_HP_L_BIAS               (1<<9)              
#define ABB_PU_HP_R_MPATH              (1<<10)             
#define ABB_PU_HP_L_MPATH              (1<<11)             
#define ABB_HP_L_SOFT_START_EN              (1<<12)             

//denoise_elimination_dac_clk_cfg
#define ABB_DENOISE_DAC_CLK_DIV(n)     (((n)&0x7F)<<0)     
#define ABB_DENOISE_DAC_CLK_INV(n)     (((n)&0x3)<<7)      
#define ABB_DENOISE_DAC_CLK_LD         (1<<9)              

//denoise_adc_clk_cfg
#define ABB_DENOISE_ADC_CLK_DIV(n)     (((n)&0x3FF)<<0)    
#define ABB_DENOISE_ADC_CLK_INV(n)     (((n)&0x3)<<10)     
#define ABB_DENOISE_ADC_CLK_LD         (1<<12)             

//denoise_ramp_cfg_l
#define ABB_START_DENOISE_L            (1<<0)              
#define ABB_STABLE_TIME_L(n)           (((n)&0x1F)<<1)     
#define ABB_RAMPUP_CYCLE_L(n)          (((n)&0xF)<<6)      
#define ABB_RAMPDOWN_EN_L              (1<<10)             
#define ABB_ADC_DET_TH_L(n)            (((n)&0xF)<<11)     
#define ABB_RAMP_DONE_L                (1<<15)             

//denoise_rampup_cfg_l
#define ABB_RAMPUP_TH_VALUE_L(n)       (((n)&0xFFF)<<0)    

//denoise_rampdown_cfg_l
#define ABB_RAMPDOWN_FINAL_VALUE_L(n)  (((n)&0xFFF)<<0)    

//denoise_ramp_cfg_r
#define ABB_START_DENOISE_R            (1<<0)              
#define ABB_STABLE_TIME_R(n)           (((n)&0x1F)<<1)     
#define ABB_RAMPUP_CYCLE_R(n)          (((n)&0xF)<<6)      
#define ABB_RAMPDOWN_EN_R              (1<<10)             
#define ABB_ADC_DET_TH_R(n)            (((n)&0xF)<<11)     
#define ABB_RAMP_DONE_R                (1<<15)             

//denoise_rampup_cfg_r
#define ABB_RAMPUP_TH_VALUE_R(n)       (((n)&0xFFF)<<0)    

//denoise_rampdown_cfg_r
#define ABB_RAMPDOWN_FINAL_VALUE_R(n)  (((n)&0xFFF)<<0)    

//denoise_other_cfg
#define ABB_HP_R_PULLUP_EN             (1<<0)              
#define ABB_HP_L_PULLUP_EN             (1<<1)              
#define ABB_HP_R_DISCHG_RES_EN         (1<<2)              
#define ABB_HP_L_DISCHG_RES_EN         (1<<3)              
#define ABB_HP_DISCHG_RES_BIT(n)       (((n)&0x3)<<4)      

//ResetControl
#define ABB_SOFT_RESETN                (1<<0)              
#define ABB_REGISTER_RESETN            (1<<1)              


#endif


