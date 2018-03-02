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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_abb_regs.h                                                       //
///                                                                           //
/// This file decribes the registers of RDA Analog Base Band.                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_ABB_REGS_H_
#define _HALP_ABB_REGS_H_

#include "cs_types.h"

// ----------------
// Registers
// ----------------

typedef enum
{
	CODEC_ID                    = 0x00,
	CODEC_LDO                   = 0x02,
	CODEC_ADC                   = 0x04,
	CODEC_MIC_BIAS              = 0x05,
	CODEC_DAC_SETTING1          = 0x06,
	CODEC_DAC_SETTING2          = 0x07,
	CODEC_DAC_SETTING3          = 0x08,
	CODEC_PLL_SETTING1          = 0x09,
	CODEC_AUX_PLL_SETTING1      = 0x0a,
	CODEC_AUX_PLL_SETTING2      = 0x0b,
	CODEC_MISC                  = 0x0c,
	CODEC_VCOM_VREF_IBIAS       = 0x0d,
	CODEC_RESET_CONTROL    		= 0x0f,
	CODEC_SDM2_FREQ1    		= 0x10,
	CODEC_SDM2_FREQ2		    = 0x11,
	CODEC_SDM2_SETTING1         = 0x12,
	CODEC_DCDC2_SETTING         = 0x13,
	CODEC_DCDC3_SETTING         = 0x14,
    CODEC_EP_DET_COEF3          =0x110,
	CODEC_EP_DET_TIME           =0x111,
	CODEC_EP_DET_FLG            =0x112,
	CODEC_GAIN_CTRL             =0x113,
	CODEC_RSVD9                 =0x114,
	CODEC_RSVD10                =0x115,
	CODEC_RSVD11                =0x116,
	CODEC_RSVD12                =0x117,
	CODEC_RSVD13                =0x118,
	CODEC_RSVD0                 =0x120,
	CODEC_RSVD1                 =0x121,
	CODEC_RSVD3                 =0x123,
	CODEC_RSVD4                 =0x124,
	CODEC_RSVD5                 =0x125,
	CODEC_RSVD6                 =0x126,
	CODEC_DAC_RAMP              =0x127,
	CODEC_ADC_CLK               =0x128,
	CODEC_CLK_CTRL              =0x129,
    CODEC_DAC_VOLUME            =0x12A,
	CODEC_TEST_CTRL             =0x12B,
	CODEC_DAC_SRO               =0x12C,
	CODEC_DAC_SR1               =0x12D,
	CODEC_TPORTS                =0x12E,
	CODEC_FID                   =0x12F,
	CODEC_DEEMPH_b0             =0x130,
	CODEC_DEEMPH_b1             =0x131,
	CODEC_DEEMPH_a1             =0x132,
	CODEC_ADC_SR                =0x133,
	CODEC_EP_DET0               =0x134,
	CODEC_EP_DET1               =0x135,
	CODEC_RSVD7                 =0x136,
	CODEC_RSVD8                 =0x137,
	CODEC_LPF20_A2              =0x138,
	CODEC_LPF20_A1              =0x139,
	CODEC_EP_DET2               =0x13A,
	CODEC_EP_DET_COEF0          =0x13C,
	CODEC_EP_DET_CTRL           =0x13D,
	CODEC_EP_DET_COEF1          =0x13E,
	CODEC_EP_DET_COEF2          =0x13F
	    
} ABB_REG_INDEX_T;


#define CODEC_DIG_MIN_REG_INDEX (0x20)


// -------------------
// Register bit fields
// -------------------

//ID
#define ABB_REVID(n)                   (((n)&0xF)<<0)      

#define ABB_CHIPID(n)                  (((n)&0xFFF)<<4)    


//ldo
#define ABB_PD_LDO                     (1<<0)              

#define ABB_PD_BG                      (1<<1)              

#define ABB_SEL_BG                     (1<<2)              

#define ABB_LDO_VBIT(n)                (((n)&0x7)<<3)      

#define ABB_LDO_TEST_ENABLE            (1<<6)              

#define ABB_LDO_TEST_BIT(n)            (((n)&0x3)<<7)      

#define ABB_LDO_BYPASS                 (1<<9)              


//ADC
#define ABB_MASH_EN_ADC                (1<<0)              

#define ABB_RESET_EN_ADC               (1<<1)              

#define ABB_TEST_EN_ADC                (1<<2)              

#define ABB_PD_ADC                     (1<<3)              

#define ABB_BOOST_BP_MIC               (1<<4)              

#define ABB_MIC_AUX_EN                 (1<<5)              

#define ABB_MIC_STD_EN                 (1<<6)              

#define ABB_MCPGA_GAIN_BIT(n)          (((n)&0x7)<<7)      

#define ABB_RESET_MIC                  (1<<10)             


//MICBias
#define ABB_MB_SEL_BIT(n)              (((n)&0x3)<<0)      

#define ABB_MIC_HPF_MODE_EN            (1<<2)              

#define ABB_MICIN_CHOP_EN              (1<<3)              

#define ABB_MICCAPLESS_MODE_EN         (1<<4)              

#define ABB_MICIN_IBPGA_EN             (1<<5)              


//DACsettings1
#define ABB_RECI_GAIN_BIT(n)           (((n)&0x7)<<0)      

#define ABB_PA_ENB_RECI                (1<<3)              

#define ABB_RESET_PA_RECI              (1<<4)              

#define ABB_RECI_LINEIN_EN             (1<<5)              

#define ABB_RECI_DAC_EN                (1<<6)              

#define ABB_DAC_MODE_R_EN              (1<<7)              

#define ABB_DAC_MODE_L_EN              (1<<8)              

#define ABB_PD_DAC_R                   (1<<9)              

#define ABB_PD_DAC_L                   (1<<10)             

#define ABB_SYN_EN_DAC                 (1<<11)             

#define ABB_RESET_DAC(n)               (((n)&0x3)<<12)     


//DACsettings2
#define ABB_MUXIN_RIGHT_EN             (1<<0)              

#define ABB_MUXIN_LEFT_EN              (1<<1)              

#define ABB_RESET_MUX_EN               (1<<2)              

#define ABB_MICDAC_MODE_EN             (1<<3)              

#define ABB_LINE_MODE_EN               (1<<4)              

#define ABB_HP_GAIN_BIT(n)             (((n)&0x7)<<5)      

#define ABB_PA_EN_HP                   (1<<8)              

#define ABB_RESET_PA_HP                (1<<9)              


//DACsettings3
#define ABB_PADET_EN                   (1<<0)              

#define ABB_RESET_PADET                (1<<1)              

#define ABB_PADET_IBIT(n)              (((n)&0x7)<<2)      

#define ABB_SSI_VREFP_RBIT(n)          (((n)&0x3)<<5)      

#define ABB_SSI_VREFN_RBIT(n)          (((n)&0x3)<<7)      

#define ABB_PD_SSI                     (1<<9)              

#define ABB_DSSI_B                     (1<<10)             

#define ABB_DSSI_A                     (1<<11)             


//pllsetting1
#define ABB_PLL_RESET                  (1<<0)              

#define ABB_PLL_LOCK                   (1<<1)              


//aux_pllsetting1
#define ABB_PLL_AUX_REFMULTI2          (1<<0)              

#define ABB_PLL_AUX_SDM_CLK_TEST_EN    (1<<1)              

#define ABB_PLL_AUX_SDM_CLK_SEL        (1<<2)              

#define ABB_PLL_AUX_CP_BIT(n)          (((n)&0x7)<<3)      

#define ABB_PLL_AUX_INT_MODE           (1<<6)              

#define ABB_PLL_AUX_FILTER_IBIT(n)     (((n)&0x7)<<7)      

#define ABB_PLL_AUX_CPAUX_BIT(n)       (((n)&0x7)<<10)     

#define ABB_PLL_AUX_CLKOUT_EN(n)       (((n)&0x3)<<13)     


//aux_pllsetting2
#define ABB_PD_PLL_AUX                 (1<<0)              

#define ABB_PLL_AUX_TEST_EN            (1<<1)              

#define ABB_PLL_AUX_VREG_BIT(n)        (((n)&0xF)<<2)      

#define ABB_PLL_AUX_RESET              (1<<6)              

#define ABB_PLL_AUX_VCO_LOW_TEST       (1<<7)              

#define ABB_PLL_AUX_VCO_HIGH_TEST      (1<<8)              

#define ABB_PLL_AUX_LOCK               (1<<9)              


//MISC
#define ABB_SPI_OUT_ABB                (0<<0)

#define ABB_SPI_OUT_CODEC              (1<<0)

#define ABB_SPI_OUT_MASK               (1<<0)

#define ABB_DAC_EN_CODEC               (1<<1)              

#define ABB_ADC_EN_CODEC               (1<<2)              

#define ABB_RSTN_CODEC                 (1<<3)              

#define ABB_PD_ALL                     (1<<4)              

#define ABB_RESERVER_ANALOG           (0X3F<<5)    

#define ABB_ISPHONE_CODEC              (1<<11)             

#define ABB_CODEC_EN_CODEC             (1<<12)             

#define ABB_CAL_READY                  (1<<13)             


//VCOM/VREF/IBIAS
#define ABB_CAL_BIT_REG(n)             (((n)&0x7)<<0)      

#define ABB_CAL_BIT_DR                 (1<<3)              

#define ABB_CAL_CLK_INV                (1<<4)              

#define ABB_CAL_OUT_POLARITY           (1<<5)              

#define ABB_RESETN_CAL_REG             (1<<6)              

#define ABB_RESETN_CAL_DR              (1<<7)              

#define ABB_ISEL_BIT_B(n)              (((n)&0x3)<<8)      

#define ABB_VREF_MODE(n)               (((n)&0x7)<<10)     

#define ABB_VCOM_MODE(n)               (((n)&0x7)<<13)     


//ResetControl
#define ABB_SOFT_RESET_SET                 (1<<0)              
#define ABB_SOFT_RESET_CLR                 (0<<0)    

#define ABB_REG_RESET_SET                   (1<<1)              
#define ABB_REG_RESET_CLR                   (0<<1)           

//sdm2freq1
#define ABB_SDM2_FREQ_HIGH(n)          (((n)&0xFFFF)<<0)   


//sdm2freq2
#define ABB_SDM2_FREQ_LOW(n)           (((n)&0xFFF)<<0)    


//sdm2setting1
#define ABB_SDM2_DITHER_BYPASS         (1<<0)              

#define ABB_SDM2_INT_DEC_SEL(n)        (((n)&0x7)<<1)      

#define ABB_SDM2_RESETN                (1<<4)              
#define ABB_SDM2_DITHER_BYPASS         (1<<0) 
#define ABB_SDM2_RESERVED(n)        (((n)&0xFF)<<5)      


//dcdc2setting
#define ABB_DCDC_CLKGEN2_RESETN_REG    (1<<0)              

#define ABB_DCDC_CLKGEN2_RESETN_DR     (1<<1)              

#define ABB_DCDC_CLKGEN2_DITHER_CT(n)  (((n)&0x7)<<2)      

#define ABB_DCDC_CLKGEN2_DIV_BASE(n)   (((n)&0xFF)<<5)     

#define ABB_DCDC_CLKGEN2_CLK_SEL       (1<<13)             

#define ABB_DCDC2_RESETN_TIME_SEL(n)   (((n)&0x3)<<14)     


//dcdc3setting
#define ABB_DCDC_CLKGEN3_RESETN_REG    (1<<0)              

#define ABB_DCDC_CLKGEN3_RESETN_DR     (1<<1)              

#define ABB_DCDC_CLKGEN3_DITHER_CT(n)  (((n)&0x7)<<2)      

#define ABB_DCDC_CLKGEN3_DIV_BASE(n)   (((n)&0xFF)<<5)     

#define ABB_DCDC_CLKGEN3_CLK_SEL       (1<<13)             

#define ABB_DCDC3_RESETN_TIME_SEL(n)   (((n)&0x3)<<14)    

//audio_ep_det_coef3
#define AUDIO_EP_DET_B1_23_8(n)        (((n)&0xFFFF)<<0)   

//audio_ep_det_time
#define AUDIO_EP_DET_TIME(n)           (((n)&0xFFFF)<<0)   

//audio_ep_det_flg
#define AUDIO_EP_DET_RSSI(n)           (((n)&0x7F)<<0)     
#define AUDIO_EP_DET_FLAG              (1<<7)              
#define AUDIO_RSVD0(n)                 (((n)&0xFF)<<8)     

//audio_gain_ctrl
#define AUDIO_GAIN_CTRL_NUM(n)         (((n)&0x3F)<<0)     
#define AUDIO_GAIN_CTRL_TH(n)          (((n)&0x7F)<<6)     
#define AUDIO_RSVD0_GAIN_CTRL(n)       (((n)&0x7)<<13)

//audio_rsvd9
#define AUDIO_RSVD0_9(n)                 (((n)&0xFFF)<<0)
#define AUDIO_RSVD1_9(n)                 (((n)&0xF)<<12)

//audio_rsvd10
#define AUDIO_RSVD0_10(n)              (((n)&0xFFFF)<<4)

//audio_rsvd11
#define AUDIO_RSVD0_11(n)              (((n)&0xF)<<0)
#define AUDIO_RSVD1_11(n)              (((n)&0xFFF)<<4)

//audio_rsvd12
#define AUDIO_RSVD0_12(n)              (((n)&0xFF)<<0)
#define AUDIO_RSVD1_12(n)              (((n)&0xFF)<<8)

//audio_rsvd13
#define AUDIO_RSVD0_13(n)              (((n)&0xFFFF)<<0)

//audio_rsvd0
//#define AUDIO_RSVD0(n)                  (((n)&0xFFFF)<<0)

//audio_rsvd1
#define AUDIO_RSVD1(n)                  (((n)&0xFFFF)<<0)

//audio_rsvd3
#define AUDIO_RSVD3(n)                  (((n)&0xFFFF)<<0)

//audio_rsvd4
#define AUDIO_RSVD4(n)                  (((n)&0xFFFF)<<0)

//audio_rsvd5
#define AUDIO_RSVD5(n)                  (((n)&0xFFFF)<<0)

//audio_rsvd6
#define AUDIO_RSVD6(n)                  (((n)&0xFFFF)<<0)

//audio_dac_ramp
#define AUDIO_RAMPDN_STEP(n)           (((n)&0x7F)<<0)     
#define AUDIO_RAMPUP_STEP(n)           (((n)&0x7F)<<7)     
#define AUDIO_RAMP_BYPASS              (1<<14)             
#define AUDIO_RAMP_IND                 (1<<15)             

//audio_adc_clk
#define AUDIO_FM_MODE                  (1<<0)              
#define AUDIO_RSVD0_CLK                (1<<1)
#define AUDIO_SLEEP_MODE               (1<<2)              
#define AUDIO_RSVD1_CLK                (1<<3)
#define AUDIO_S_ADC_OSR_SEL(n)         (((n)&0x3)<<4)      
#define AUDIO_CNT_ADC_CLK_INT(n)       (((n)&0x7F)<<6)     
#define AUDIO_CNT_ADC_CLK_DEC(n)       (((n)&0x7)<<13)     

//audio_clk_ctrl
#define AUDIO_RSVD0_CLK_CTRL           (1<<0)
#define AUDIO_S_HPF_BYPASS             (1<<1)              
#define AUDIO_S_CODEC_EN               (1<<2)              
#define AUDIO_S_ISPHONE                (1<<3)              
#define AUDIO_EARPHONE_DET_CLK_INV     (1<<4)              
#define AUDIO_EARPHONE_DET_CLK_DIV2_EN (1<<5)              
#define AUDIO_ANA_ADC_CLK_INV          (1<<6)              
#define AUDIO_ANA_DAC_CLK_INV          (1<<7)              
#define AUDIO_S_ADC_IN_INV             (1<<8)              
#define AUDIO_MASH_EN_ADC              (1<<9)              
#define AUDIO_S_DITHER_BYPASS          (1<<10)             
#define AUDIO_S_DWA_EN                 (1<<11)             
#define AUDIO_S_PHONE_SEL              (1<<12)             
#define AUDIO_RSVD1_CLK_CTRL(n)        (((n)&0x7)<<13)

//audio_dac_volume
#define AUDIO_S_MUTE_R                 (1<<0)              
#define AUDIO_S_MUTE_L                 (1<<1)              
#define AUDIO_S_DAC_VOLUME(n)          (((n)&0x1F)<<2)     
#define AUDIO_S_DAC_VOLUME_MUTE        (1<<7)              
#define AUDIO_DITHER_GAIN_4_3(n)       (((n)&0x3)<<8)      
#define AUDIO_SDM_GAIN(n)              (((n)&0x3)<<10)     
#define AUDIO_S_ADC_DVST_CTL_3_0(n)    (((n)&0xF)<<12)     

//audio_test_ctrl
#define AUDIO_S_ADC_DVST_CTL_4         (1<<0)              
#define AUDIO_TPORTS_SEL(n)            (((n)&0x1F)<<1)     
#define AUDIO_TPORTS_EN                (1<<6)              
#define AUDIO_S_LOOP                   (1<<7)              
#define AUDIO_S_DAC_1K                 (1<<8)              
#define AUDIO_S_ADC_VOLUME(n)          (((n)&0xF)<<9)      
#define AUDIO_DITHER_GAIN_2_0(n)       (((n)&0x7)<<13)     

//audio_dac_sr0
#define AUDIO_S_FS_SEL(n)              (((n)&0x7)<<0)      
#define AUDIO_RSVD0_DAC_SR0(n)         (((n)&0x3)<<3)
#define AUDIO_DAC_PATH_RESET           (1<<5)              
#define AUDIO_ADC_PATH_RESET           (1<<6)              
#define AUDIO_S_DAC_OSR_SEL(n)         (((n)&0x3)<<7)      
#define AUDIO_RSVD1_DAC_SR0            (1<<9)
#define AUDIO_S_DAC_GAIN(n)            (((n)&0x3)<<10)     
#define AUDIO_FS_CNT_DEC_3_0(n)        (((n)&0xF)<<12)     

//audio_dac_sr1
#define AUDIO_FS_CNT_DEC_9_4(n)        (((n)&0x3F)<<0)     
#define AUDIO_FS_CNT_INT(n)            (((n)&0x3FF)<<6)    
#define AUDIO_FS_ADC_CNT_INT(n)    (((n)&0x7F)<<6)    

//audio_tports
#define AUDIO_TPORTS(n)                (((n)&0xFFFF)<<0)   

//audio_fid
#define AUDIO_FID(n)                   (((n)&0xFFFF)<<0)   

//audio_deemph_b0
#define AUDIO_DE_EMPH_B0(n)            (((n)&0x1FFF)<<0)   
#define AUDIO_RSVD0_DEEMPH_B0(n)       (((n)&0x7)<<13)

//audio_deemph_b1
#define AUDIO_DE_EMPH_B1(n)            (((n)&0x1FFF)<<0)   
#define AUDIO_RSVD0_DEEMPH_B1(n)       (((n)&0x7)<<13)

//audio_deemph_a1
#define AUDIO_DE_EMPH_A1(n)            (((n)&0x1FFF)<<0)   
//#define AUDIO_RSVD0(n)                 (((n)&0x7)<<13)

//audio_adc_sr
#define AUDIO_DAC_CLKGEN_MODE          (1<<0)              
#define AUDIO_ADC_CLKGEN_MODE          (1<<1)              
#define AUDIO_EP_DET_FLG_CODE(n)       (((n)&0xFF)<<2)     
#define AUDIO_ADC_LPF_BYPASS           (1<<10)             
#define AUDIO_RECORD_32K               (1<<11)             
#define AUDIO_RECORD_16K               (1<<12)             
//#define AUDIO_RSVD0(n)                 (((n)&0x7)<<13)

//audio_ep_det0
#define AUDIO_EP_DET_TH_L(n)           (((n)&0x1FF)<<0)    
#define AUDIO_EP_DET_CNT_TH(n)         (((n)&0xF)<<9)      
//#define AUDIO_RSVD0(n)                 (((n)&0x7)<<13)

//audio_ep_det1
#define AUDIO_EP_DET_TH_H(n)           (((n)&0x1FF)<<0)    
#define AUDIO_SPK_GAIN_SEL(n)          (((n)&0x3)<<9)      
#define AUDIO_RSVD0_EP_DET1(n)         (((n)&0x1F)<<11)

//audio_rsvd7
#define AUDIO_RSVD(n)                  (((n)&0xFFFF)<<0)   

//audio_rsvd8
#define AUDIO_RSVD(n)                  (((n)&0xFFFF)<<0)   

//audio_lpf20_a2
#define AUDIO_LPF20_CF_A2(n)           (((n)&0xFFFF)<<0)   

//audio_lpf20_a1
#define AUDIO_LPF20_CF_A1(n)           (((n)&0xFFFF)<<0)   

//audio_ep_det2
#define AUDIO_LPF20_CF_B0B2(n)         (((n)&0x3F)<<0)     
#define AUDIO_EP_DET_GAIN(n)           (((n)&0x3)<<6)      
#define AUDIO_EP_DET_HPF_CT_U(n)       (((n)&0x3)<<8)      
#define AUDIO_EP_DET_LPF20_BYPASS      (1<<10)             
#define AUDIO_RSVD0_EP_DET2(n)         (((n)&0x1F)<<11)

//audio_ep_det_coef0
#define AUDIO_EP_DET_COEF_15_0(n)      (((n)&0xFFFF)<<0)   

//audio_ep_det_ctrl
#define AUDIO_EP_DET_COEF_19_16(n)     (((n)&0xF)<<0)      
#define AUDIO_EP_DET_TH(n)             (((n)&0x7F)<<4)     
#define AUDIO_EP_DET_SEL               (1<<11)             
#define AUDIO_EP_DET_BYPASS            (1<<12)             
#define AUDIO_NOTCH20_BYPASS           (1<<13)             
#define AUDIO_DAC_HPF_MODE             (1<<14)             
#define AUDIO_TONE20_GEN_EN            (1<<15)             

//audio_ep_det_coef1
#define AUDIO_EP_DET_A1_15_0(n)        (((n)&0xFFFF)<<0)   

//audio_ep_det_coef2
#define AUDIO_EP_DET_A1_23_16(n)       (((n)&0xFF)<<0)     
#define AUDIO_EP_DET_B1_7_0(n)         (((n)&0xFF)<<8)     

#endif // _HALP_ABB_REGS_H_


