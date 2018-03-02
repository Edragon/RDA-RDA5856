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
// File name      : rf_if_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-05 14:09:03                                                  
// Last modified  : 2016-12-05 14:09:03                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-05 14:09:03 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __RF_IF_ASM_H__
#define __RF_IF_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_revision                   REG_RF_IF_BASE+0x00 
#define REG_sys_control                REG_RF_IF_BASE+0x04 
#define REG_bt_control                 REG_RF_IF_BASE+0x08 
#define REG_fm_control                 REG_RF_IF_BASE+0x0c 
#define REG_inter_freq_setting         REG_RF_IF_BASE+0x10 
#define REG_ldo_setting                REG_RF_IF_BASE+0x14 
#define REG_bt_lna_rmx_setting         REG_RF_IF_BASE+0x18 
#define REG_fm_lna_rmx_rxflt_setting   REG_RF_IF_BASE+0x1c 
#define REG_rxflt_adc_setting          REG_RF_IF_BASE+0x20 
#define REG_bt_bbdac_setting           REG_RF_IF_BASE+0x24 
#define REG_bt_padrv_tmx_setting       REG_RF_IF_BASE+0x28 
#define REG_fm_dsp_setting             REG_RF_IF_BASE+0x2c 
#define REG_vco_setting                REG_RF_IF_BASE+0x30 
#define REG_rfpll_setting0             REG_RF_IF_BASE+0x34 
#define REG_rfpll_setting1             REG_RF_IF_BASE+0x38 
#define REG_rfpll_setting2             REG_RF_IF_BASE+0x3c 
#define REG_rfpll_status               REG_RF_IF_BASE+0x40 
#define REG_adpll_setting0             REG_RF_IF_BASE+0x44 
#define REG_adpll_setting1             REG_RF_IF_BASE+0x48 
#define REG_adpll_status               REG_RF_IF_BASE+0x4c 
#define REG_rfpll_sdm_setting0         REG_RF_IF_BASE+0x50 
#define REG_rfpll_sdm_setting1         REG_RF_IF_BASE+0x54 
#define REG_adpll_sdm_setting0         REG_RF_IF_BASE+0x58 
#define REG_adpll_sdm_setting1         REG_RF_IF_BASE+0x5c 
#define REG_dc_cal_setting0            REG_RF_IF_BASE+0x60 
#define REG_dc_cal_setting1            REG_RF_IF_BASE+0x64 
#define REG_dc_cal_status              REG_RF_IF_BASE+0x68 
#define REG_vco_cal_setting            REG_RF_IF_BASE+0x6c 
#define REG_rfpll_cal_setting0         REG_RF_IF_BASE+0x70 
#define REG_rfpll_cal_setting1         REG_RF_IF_BASE+0x74 
#define REG_cal_dr_setting             REG_RF_IF_BASE+0x78 
#define REG_cal_status                 REG_RF_IF_BASE+0x7c 
#define REG_power_dr                   REG_RF_IF_BASE+0x80 
#define REG_power_reg                  REG_RF_IF_BASE+0x84 
#define REG_power_status               REG_RF_IF_BASE+0x88 
#define REG_bt_gain_table_0            REG_RF_IF_BASE+0x8c 
#define REG_bt_gain_table_1            REG_RF_IF_BASE+0x90 
#define REG_bt_gain_table_2            REG_RF_IF_BASE+0x94 
#define REG_bt_gain_table_3            REG_RF_IF_BASE+0x98 
#define REG_bt_gain_table_4            REG_RF_IF_BASE+0x9c 
#define REG_bt_gain_table_5            REG_RF_IF_BASE+0xa0 
#define REG_bt_gain_table_6            REG_RF_IF_BASE+0xa4 
#define REG_bt_gain_table_7            REG_RF_IF_BASE+0xa8 
#define REG_bt_ibit_table_0            REG_RF_IF_BASE+0xac 
#define REG_bt_ibit_table_1            REG_RF_IF_BASE+0xb0 
#define REG_bt_ibit_table_2            REG_RF_IF_BASE+0xb4 
#define REG_bt_ibit_table_3            REG_RF_IF_BASE+0xb8 
#define REG_bt_ibit_table_4            REG_RF_IF_BASE+0xbc 
#define REG_bt_padrv_tmx_gain_table_0  REG_RF_IF_BASE+0xc0 
#define REG_bt_padrv_tmx_gain_table_1  REG_RF_IF_BASE+0xc4 
#define REG_fm_lna_gain_table          REG_RF_IF_BASE+0xc8 
#define REG_fm_rmx_gain_table_0        REG_RF_IF_BASE+0xcc 
#define REG_fm_rmx_gain_table_1        REG_RF_IF_BASE+0xd0 
#define REG_fm_rxflt_gain_table        REG_RF_IF_BASE+0xd4 
#define REG_test_buffer_setting        REG_RF_IF_BASE+0xd8 
#define REG_reserved_reg               REG_RF_IF_BASE+0xdc 
#define REG_channel_setting            REG_RF_IF_BASE+0xe0 
#define REG_reserved_in_reg            REG_RF_IF_BASE+0xe4 
#define REG_sys_clk_cfg                REG_RF_IF_BASE+0xe8 

//revision
#define RF_IF_REVID(n)                 (((n)&0xF)<<0)      

//sys_control
#define RF_IF_SOFT_RESETN              (1<<0)              
#define RF_IF_CHIP_SELF_CAL_ENABLE     (1<<1)              
#define RF_IF_RF_MODE                  (1<<2)              
#define RF_IF_CLK_DIGITAL_ENABLE_REG   (1<<3)              
#define RF_IF_BT_TUNE_DIFF_EN          (1<<4)              
#define RF_IF_BT_HOPPING_EN            (1<<5)              
#define RF_IF_FM_ADCLK_MODE(n)         (((n)&0x3)<<6)      

//bt_control
#define RF_IF_BT_ARFCN(n)              (((n)&0x7F)<<0)     
#define RF_IF_BT_CH_TYPE               (1<<7)              

//fm_control
#define RF_IF_FM_TUNE                  (1<<0)              
#define RF_IF_FM_CHAN_REG(n)           (((n)&0x7FF)<<1)    
#define RF_IF_FM_FREQ_MODE             (1<<12)             
#define RF_IF_FM_BAND_SEL(n)           (((n)&0x3)<<13)     
#define RF_IF_FM_FREQ_DIRECT(n)        (((n)&0xFFFF)<<16)  

//inter_freq_setting
#define RF_IF_BT_IF(n)                 (((n)&0x7FF)<<0)    
#define RF_IF_BT_IMGREJ                (1<<11)             
#define RF_IF_BT_ZIF                   (1<<12)             
#define RF_IF_FM_IF(n)                 (((n)&0x7FF)<<16)   
#define RF_IF_FM_IMGREJ                (1<<27)             

//ldo_setting
#define RF_IF_RF_IVREF_AVDD_BIT        (1<<0)              

//bt_lna_rmx_setting
#define RF_IF_BT_RMX_REG_BYPASS        (1<<0)              
#define RF_IF_BT_RMX_REG_BIT(n)        (((n)&0x7)<<1)      
#define RF_IF_BT_RMX_LO_IFVCOM_BIT(n)  (((n)&0xF)<<6)      
#define RF_IF_BT_RMX_LO_IFVCOM_SEL     (1<<10)             
#define RF_IF_BT_LNA_SELFBIAS_EN       (1<<16)             
#define RF_IF_BT_LNA_VCAS_BIT(n)       (((n)&0x3)<<17)     
#define RF_IF_BT_LNA_REG_BYPASS        (1<<22)             
#define RF_IF_BT_LNA_REG_BIT(n)        (((n)&0x7)<<23)     
#define RF_IF_BT_BALUN_CAP_BIT_TX(n)   (((n)&0x7)<<26)     
#define RF_IF_BT_BALUN_CAP_BIT_RX(n)   (((n)&0x7)<<29)     

//fm_lna_rmx_rxflt_setting
#define RF_IF_FM_RXFLT_OP_IBIT(n)      (((n)&0x3)<<0)      
#define RF_IF_FM_RMX_LOBIAS_BIT(n)     (((n)&0x3)<<2)      
#define RF_IF_FM_RMX_HARM_REJ_EN       (1<<4)              
#define RF_IF_FM_LNA_IBIT(n)           (((n)&0x7)<<5)      

//rxflt_adc_setting
#define RF_IF_ADC_SKIP                 (1<<0)              
#define RF_IF_ADC_VREF_VBIT(n)         (((n)&0x3)<<1)      
#define RF_IF_ADC_VREF_RBIT(n)         (((n)&0x3)<<3)      
#define RF_IF_ADC_DELAY_BIT(n)         (((n)&0xF)<<5)      
#define RF_IF_ADC_CLK_DIV2             (1<<9)              
#define RF_IF_ADC_CLK_EDGE_SEL         (1<<10)             
#define RF_IF_ADC_CLK_SEL_REG(n)       (((n)&0x3)<<11)     
#define RF_IF_ADC_CLK_SEL_DR           (1<<13)             
#define RF_IF_ADC_CLK_XTAL_EN_REG      (1<<14)             
#define RF_IF_ADC_CLK_XTAL_EN_DR       (1<<15)             
#define RF_IF_RXFLT_IQSWAP             (1<<16)             
#define RF_IF_RXFLT_OP_DRIVE_BIT(n)    (((n)&0x7)<<17)     
#define RF_IF_RXFLT_CAP_BIT(n)         (((n)&0xF)<<20)     
#define RF_IF_RXFLT_FM_BP_MODE(n)      (((n)&0x3)<<24)     
#define RF_IF_RXFLT_BT_BP_MODE(n)      (((n)&0x3)<<26)     
#define RF_IF_RXFLT_FM_MODE_REG        (1<<28)             
#define RF_IF_RXFLT_FM_MODE_DR         (1<<29)             
#define RF_IF_RXFLT_RSTN_REG           (1<<30)             
#define RF_IF_RXFLT_RSTN_DR            (1<<31)             

//bt_bbdac_setting
#define RF_IF_BT_BBDAC_VTR_SEL         (1<<0)              
#define RF_IF_BT_BBDAC_CLK_EDGE_SEL    (1<<1)              
#define RF_IF_BT_BBDAC_VLOW_BIT(n)     (((n)&0x7)<<2)      

//bt_padrv_tmx_setting
#define RF_IF_BT_TMX_REG_BIT(n)        (((n)&0x7)<<0)      
#define RF_IF_BT_TMX_BBTEST_EN         (1<<3)              
#define RF_IF_BT_TMX_PLLTEST_EN        (1<<4)              
#define RF_IF_BT_TMX_IBIT(n)           (((n)&0x3)<<9)      
#define RF_IF_BT_PADRV_IBIT(n)         (((n)&0x3F)<<16)    
#define RF_IF_BT_DELAY_PADRV(n)        (((n)&0xFF)<<22)    

//fm_dsp_setting
#define RF_IF_FM_DSP_RESETN_REG        (1<<0)              
#define RF_IF_FM_DSP_RESETN_DR         (1<<1)              
#define RF_IF_FM_DSP_RESETN_DELAY(n)   (((n)&0x3)<<2)      

//vco_setting
#define RF_IF_VCO_DIV2_BIAS_BIT(n)     (((n)&0x7)<<0)      
#define RF_IF_VCO_BUF_BIAS_BIT(n)      (((n)&0x3)<<3)      
#define RF_IF_VCO_BUF_IBIT(n)          (((n)&0xF)<<5)      
#define RF_IF_VCO_REG_BYPASS           (1<<9)              

//rfpll_setting0
#define RF_IF_RFPLL_DIV_FM_ADCLK_REG(n) (((n)&0x1F)<<0)     
#define RF_IF_RFPLL_DIV_FM_ADCLK_DR    (1<<5)              
#define RF_IF_RFPLL_DIV_FM_REG(n)      (((n)&0xF)<<6)      
#define RF_IF_RFPLL_DIV_FM_DR          (1<<10)             
#define RF_IF_RFPLL_DVDD_R_BIT(n)      (((n)&0xF)<<13)     
#define RF_IF_RFPLL_BYPASS_NOTCH       (1<<17)             
#define RF_IF_RFPLL_PRESC_REG_BIT(n)   (((n)&0xF)<<18)     
#define RF_IF_RFPLL_REFMULTI2_EN       (1<<22)             
#define RF_IF_RESET_RFPLL_LOCK_DELAY(n) (((n)&0x3)<<23)     
#define RF_IF_RFPLL_CP_R_BIT_RX(n)     (((n)&0x3)<<25)     
#define RF_IF_RFPLL_CP_R_BIT_TX(n)     (((n)&0x3)<<27)     

//rfpll_setting1
#define RF_IF_RFPLL_CP_BIT_TX(n)       (((n)&0x3F)<<0)     
#define RF_IF_RFPLL_GAIN_BIT_TX(n)     (((n)&0x7)<<6)      
#define RF_IF_MDLL_DIV_TX(n)           (((n)&0xF)<<12)     
#define RF_IF_RFPLL_CP_BIT_RX(n)       (((n)&0x3F)<<16)    
#define RF_IF_RFPLL_GAIN_BIT_RX(n)     (((n)&0x7)<<22)     
#define RF_IF_RFPLL_PRESC_REG_RC(n)    (((n)&0x7)<<25)     
#define RF_IF_MDLL_DIV_RX(n)           (((n)&0xF)<<28)     

//rfpll_setting2
#define RF_IF_RFPLL_PFD_RES_BIT_TX(n)  (((n)&0x3F)<<0)     
#define RF_IF_RFPLL_LPF_GAIN_BIT_TX(n) (((n)&0x1F)<<6)     
#define RF_IF_RFPLL_R_BIT_TX(n)        (((n)&0x1F)<<11)    
#define RF_IF_RFPLL_PFD_RES_BIT_RX(n)  (((n)&0x3F)<<16)    
#define RF_IF_RFPLL_LPF_GAIN_BIT_RX(n) (((n)&0x1F)<<22)    
#define RF_IF_RFPLL_R_BIT_RX(n)        (((n)&0x1F)<<27)    

//rfpll_status
#define RF_IF_RFPLL_LOCK_DET           (1<<0)              
#define RF_IF_RFPLL_LOCK_FLAG          (1<<1)              

//adpll_setting0
#define RF_IF_ADPLL_CLK_TEST_EN        (1<<0)              
#define RF_IF_ADPLL_SDM_CLK_TEST_EN    (1<<1)              
#define RF_IF_ADPLL_PCON_MODE          (1<<2)              
#define RF_IF_ADPLL_REFMULTI2_EN       (1<<3)              
#define RF_IF_ADPLL_CP_R2_BIT(n)       (((n)&0x7)<<4)      
#define RF_IF_ADPLL_CP_C2_BIT(n)       (((n)&0x7)<<7)      
#define RF_IF_ADPLL_CP_BIAS_BIT(n)     (((n)&0x7)<<10)     
#define RF_IF_ADPLL_DIGI_REG_ROUT_BIT(n) (((n)&0xF)<<13)     
#define RF_IF_ADPLL_DIGI_REG_RES_BIT(n) (((n)&0x7)<<17)     
#define RF_IF_ADPLL_DIGI_REG_RANGE_BIT(n) (((n)&0xF)<<20)     
#define RF_IF_ADPLL_VREG_BIT(n)        (((n)&0xF)<<24)     
#define RF_IF_ADPLL_RSTN_REG           (1<<28)             
#define RF_IF_ADPLL_RSTN_DR            (1<<29)             
#define RF_IF_PU_ADPLL_REG             (1<<30)             
#define RF_IF_PU_ADPLL_DR              (1<<31)             

//adpll_setting1
#define RF_IF_RESET_ADPLL_LOCK_DELAY(n) (((n)&0x3)<<0)      
#define RF_IF_ADPLL_VCO_LOW_TEST       (1<<2)              
#define RF_IF_ADPLL_VCO_HIGH_TEST      (1<<3)              
#define RF_IF_ADPLL_FM_SEL             (1<<4)              
#define RF_IF_ADPLL_CPU_SEL            (1<<5)              
#define RF_IF_ADPLL_BT_SEL             (1<<6)              
#define RF_IF_ADPLL_CLK_FM_EN          (1<<7)              
#define RF_IF_ADPLL_CLK_CPU_EN         (1<<8)              
#define RF_IF_ADPLL_CLK_BT_EN          (1<<9)              
#define RF_IF_USBC_PHY_CLK_DIV(n)      (((n)&0x3)<<16)     
#define RF_IF_USBC_PHY_CLK_LD_CFG      (1<<18)             
#define RF_IF_SYS_CLK_SEL_FAST         (1<<19)             
#define RF_IF_SYS_FAST_CLK_SEL(n)      (((n)&0x3)<<20)     
#define RF_IF_SYS_PLL_CLK_DIV(n)       (((n)&0xF)<<22)     
#define RF_IF_SYS_PLL_CLK_LD_CFG       (1<<26)             
#define RF_IF_HCLK_PCLK_DIV(n)         (((n)&0x7)<<27)     
#define RF_IF_HCLK_PCLK_LD_CFG         (1<<30)             
#define RF_IF_SYS_PLL_CLK_SRC_SEL      (1<<31)             

//adpll_status
#define RF_IF_ADPLL_LOCK_DET           (1<<0)              
#define RF_IF_ADPLL_LOCK_FLAG          (1<<1)              

//rfpll_sdm_setting0
#define RF_IF_RFPLL_SDM_DELAY_SEL(n)   (((n)&0xF)<<0)      
#define RF_IF_RFPLL_SDM_DITHER_BYPASS  (1<<4)              
#define RF_IF_RFPLL_SDM_INT_DEC_SEL(n) (((n)&0x3)<<5)      
#define RF_IF_RFPLL_SDM_CLK_EDGE       (1<<7)              
#define RF_IF_RFPLL_SDM_RESETN_REG     (1<<8)              
#define RF_IF_RFPLL_SDM_RESETN_DR      (1<<9)              
#define RF_IF_RESET_RFPLL_SDM_DELAY(n) (((n)&0x3)<<10)     
#define RF_IF_RFPLL_SDM_CLK_SEL_1      (1<<12)             
#define RF_IF_RFPLL_SDM_CLK_SEL_0      (1<<13)             
#define RF_IF_RFPLL_DIV_DR             (1<<14)             

//rfpll_sdm_setting1
#define RF_IF_RFPLL_DIV_REG(n)         (((n)&0x7FFFFFFF)<<0)

//adpll_sdm_setting0
#define RF_IF_ADPLL_SDM_REFDIV2_EN     (1<<0)              
#define RF_IF_ADPLL_SDM_DITHER_BYPASS  (1<<1)              
#define RF_IF_ADPLL_SDM_INT_DEC_SEL(n) (((n)&0x7)<<2)      
#define RF_IF_ADPLL_SDM_CLK_FBC_INV    (1<<5)              
#define RF_IF_ADPLL_SDM_RESETN_REG     (1<<6)              
#define RF_IF_ADPLL_SDM_RESETN_DR      (1<<7)              
#define RF_IF_RESET_ADPLL_SDM_DELAY(n) (((n)&0x3)<<8)      
#define RF_IF_ADPLL_SDM_CLK_SEL_1      (1<<10)             
#define RF_IF_ADPLL_SDM_CLK_SEL_0      (1<<11)             
#define RF_IF_ADPLL_SDM_FREQ_DR        (1<<12)             

//adpll_sdm_setting1
#define RF_IF_ADPLL_SDM_FREQ_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dc_cal_setting0
#define RF_IF_DC_OFFSET_INIT_DELAY(n)  (((n)&0x3)<<0)      
#define RF_IF_DC_CAL_CNT(n)            (((n)&0xF)<<2)      
#define RF_IF_DC_CAL_IQSWAP            (1<<6)              
#define RF_IF_DC_CAL_POLARITY          (1<<7)              
#define RF_IF_DC_CAL_MODE(n)           (((n)&0x3)<<8)      
#define RF_IF_DC_CAL_CLK_EDGE          (1<<10)             
#define RF_IF_FM_DC_TABLE_FULL_MODE    (1<<11)             
#define RF_IF_BT_DC_TABLE_FULL_MODE    (1<<12)             
#define RF_IF_RXFLT_CALCLK_EDGE_SEL    (1<<13)             
#define RF_IF_RXFLT_CAL_MODE           (1<<14)             
#define RF_IF_RXFLT_OFFSET_RANGE_BIT(n) (((n)&0x7)<<15)     
#define RF_IF_RXFLT_OFFSET_BIT_DR      (1<<18)             

//dc_cal_setting1
#define RF_IF_RXFLT_OFFSET_Q_BIT_REG(n) (((n)&0xFF)<<0)     
#define RF_IF_RXFLT_OFFSET_I_BIT_REG(n) (((n)&0xFF)<<8)     

//dc_cal_status
#define RF_IF_RXFLT_OFFSET_Q_BIT(n)    (((n)&0xFF)<<0)     
#define RF_IF_RXFLT_OFFSET_I_BIT(n)    (((n)&0xFF)<<8)     

//vco_cal_setting
#define RF_IF_VCO_CAL_POLARITY         (1<<0)              
#define RF_IF_VCO_CAL_INIT_DELAY(n)    (((n)&0x3)<<1)      
#define RF_IF_VCO_CAL_CNT(n)           (((n)&0x3)<<3)      
#define RF_IF_VCO_CAL_MODE             (1<<5)              
#define RF_IF_VCO_CAL_CLK_EDGE         (1<<6)              
#define RF_IF_VCO_PKD_CLK_EDGE_SEL     (1<<7)              
#define RF_IF_VCO_PKD_REF_BIT(n)       (((n)&0x7)<<8)      
#define RF_IF_VCO_IBIT_REG(n)          (((n)&0xF)<<11)     
#define RF_IF_VCO_IBIT_DR              (1<<15)             

//rfpll_cal_setting0
#define RF_IF_RFPLL_INIT_DELAY(n)      (((n)&0x7)<<0)      
#define RF_IF_RFPLL_CAL_CNT_SEL(n)     (((n)&0x7)<<3)      
#define RF_IF_RFPLL_CAL_OPT            (1<<6)              
#define RF_IF_RFPLL_VCO_BIT_HOLD_TIME(n) (((n)&0x7)<<7)      
#define RF_IF_RFPLL_CNT_ENABLE_POLARITY (1<<10)             
#define RF_IF_RFPLL_CAL_FREQ_DR        (1<<11)             
#define RF_IF_VCO_BAND_REG(n)          (((n)&0x3FF)<<12)   
#define RF_IF_VCO_BAND_DR              (1<<22)             
#define RF_IF_RFPLL_OPEN_EN            (1<<23)             
#define RF_IF_RFPLL_CAL_BIT(n)         (((n)&0x3)<<24)     
#define RF_IF_RFPLL_CAL_CLK_SEL        (1<<26)             

//rfpll_cal_setting1
#define RF_IF_RFPLL_CAL_FREQ_REG(n)    (((n)&0xFFFF)<<0)   

//cal_dr_setting
#define RF_IF_SELF_CAL_READY_REG       (1<<0)              
#define RF_IF_SELF_CAL_READY_DR        (1<<1)              
#define RF_IF_RFPLL_CAL_RESETN_REG     (1<<2)              
#define RF_IF_RFPLL_CAL_RESETN_DR      (1<<3)              
#define RF_IF_VCO_CAL_RESETN_REG       (1<<4)              
#define RF_IF_VCO_CAL_RESETN_DR        (1<<5)              
#define RF_IF_BT_DC_CAL_FSM_RESETN_REG (1<<6)              
#define RF_IF_BT_DC_CAL_FSM_RESETN_DR  (1<<7)              
#define RF_IF_FM_DC_CAL_FSM_RESETN_REG (1<<8)              
#define RF_IF_FM_DC_CAL_FSM_RESETN_DR  (1<<9)              

//cal_status
#define RF_IF_BT_SELF_CAL_READY        (1<<0)              
#define RF_IF_FM_SELF_CAL_READY        (1<<1)              
#define RF_IF_DC_CAL_READY             (1<<2)              
#define RF_IF_RFPLL_CAL_READY          (1<<3)              
#define RF_IF_VCO_CAL_READY            (1<<4)              
#define RF_IF_BT_DC_CAL_FSM_READY      (1<<5)              
#define RF_IF_FM_DC_CAL_FSM_READY      (1<<6)              

//power_dr
#define RF_IF_PU_RFPLL_FM_ADCLK_DR     (1<<1)              
#define RF_IF_PU_RFPLL_FMCLK_DR        (1<<2)              
#define RF_IF_PU_RFPLL_PRESC_DR        (1<<3)              
#define RF_IF_PU_RFPLL_DIGI_DR         (1<<4)              
#define RF_IF_PU_VCO_PKD_DR            (1<<5)              
#define RF_IF_PU_LO_RX_DR              (1<<6)              
#define RF_IF_PU_LO_TX_DR              (1<<7)              
#define RF_IF_PU_VCO_DR                (1<<8)              
#define RF_IF_PU_BT_TMX_DR             (1<<9)              
#define RF_IF_PU_BT_PADRV_DR           (1<<10)             
#define RF_IF_PU_BT_BBDAC_DR           (1<<11)             
#define RF_IF_PU_ADC_DR                (1<<12)             
#define RF_IF_PU_RXFLT_DR              (1<<13)             
#define RF_IF_PU_BT_RMX_DR             (1<<14)             
#define RF_IF_PU_BT_LNA_DR             (1<<15)             
#define RF_IF_PU_FM_RMX_DR             (1<<16)             
#define RF_IF_PU_FM_LNA_DR             (1<<17)             
#define RF_IF_PU_RF_IVREF_DR           (1<<18)             

//power_reg
#define RF_IF_PU_RFPLL_FM_ADCLK_REG    (1<<1)              
#define RF_IF_PU_RFPLL_FMCLK_REG       (1<<2)              
#define RF_IF_PU_RFPLL_PRESC_REG       (1<<3)              
#define RF_IF_PU_RFPLL_DIGI_REG        (1<<4)              
#define RF_IF_PU_VCO_PKD_REG           (1<<5)              
#define RF_IF_PU_LO_RX_REG             (1<<6)              
#define RF_IF_PU_LO_TX_REG             (1<<7)              
#define RF_IF_PU_VCO_REG               (1<<8)              
#define RF_IF_PU_BT_TMX_REG            (1<<9)              
#define RF_IF_PU_BT_PADRV_REG          (1<<10)             
#define RF_IF_PU_BT_BBDAC_REG          (1<<11)             
#define RF_IF_PU_ADC_REG               (1<<12)             
#define RF_IF_PU_RXFLT_REG             (1<<13)             
#define RF_IF_PU_BT_RMX_REG            (1<<14)             
#define RF_IF_PU_BT_LNA_REG            (1<<15)             
#define RF_IF_PU_FM_RMX_REG            (1<<16)             
#define RF_IF_PU_FM_LNA_REG            (1<<17)             
#define RF_IF_PU_RF_IVREF_REG          (1<<18)             
#define RF_IF_PU_TBUF_REG              (1<<19)             
#define RF_IF_PU_RF_IOBUF_REG          (1<<20)             

//power_status
#define RF_IF_PU_ADPLL                 (1<<0)              
#define RF_IF_PU_RFPLL_FM_ADCLK        (1<<1)              
#define RF_IF_PU_RFPLL_FMCLK           (1<<2)              
#define RF_IF_PU_RFPLL_PRESC           (1<<3)              
#define RF_IF_PU_RFPLL_DIGI            (1<<4)              
#define RF_IF_PU_VCO_PKD               (1<<5)              
#define RF_IF_PU_LO_RX                 (1<<6)              
#define RF_IF_PU_LO_TX                 (1<<7)              
#define RF_IF_PU_VCO                   (1<<8)              
#define RF_IF_PU_BT_TMX                (1<<9)              
#define RF_IF_PU_BT_PADRV              (1<<10)             
#define RF_IF_PU_BT_BBDAC              (1<<11)             
#define RF_IF_PU_ADC                   (1<<12)             
#define RF_IF_PU_RXFLT                 (1<<13)             
#define RF_IF_PU_BT_RMX                (1<<14)             
#define RF_IF_PU_BT_LNA                (1<<15)             
#define RF_IF_PU_FM_RMX                (1<<16)             
#define RF_IF_PU_FM_LNA                (1<<17)             
#define RF_IF_PU_RF_IVREF              (1<<18)             
#define RF_IF_PU_TBUF                  (1<<19)             
#define RF_IF_PU_RF_IOBUF              (1<<20)             

//bt_gain_table_0
#define RF_IF_BT_RXFLT_GAIN2_1(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_1(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_1(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_1(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_1(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_0(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_0(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_0(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_0(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_0(n)        (((n)&0xF)<<26)     
#define RF_IF_BT_AGC_GAIN_DR           (1<<30)             

//bt_gain_table_1
#define RF_IF_BT_RXFLT_GAIN2_3(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_3(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_3(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_3(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_3(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_2(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_2(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_2(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_2(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_2(n)        (((n)&0xF)<<26)     

//bt_gain_table_2
#define RF_IF_BT_RXFLT_GAIN2_5(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_5(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_5(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_5(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_5(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_4(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_4(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_4(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_4(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_4(n)        (((n)&0xF)<<26)     

//bt_gain_table_3
#define RF_IF_BT_RXFLT_GAIN2_7(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_7(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_7(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_7(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_7(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_6(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_6(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_6(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_6(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_6(n)        (((n)&0xF)<<26)     

//bt_gain_table_4
#define RF_IF_BT_RXFLT_GAIN2_9(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_9(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_9(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_9(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_9(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_8(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_8(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_8(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_8(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_8(n)        (((n)&0xF)<<26)     

//bt_gain_table_5
#define RF_IF_BT_RXFLT_GAIN2_B(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_B(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_B(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_B(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_B(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_A(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_A(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_A(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_A(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_A(n)        (((n)&0xF)<<26)     

//bt_gain_table_6
#define RF_IF_BT_RXFLT_GAIN2_D(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_D(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_D(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_D(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_D(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_C(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_C(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_C(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_C(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_C(n)        (((n)&0xF)<<26)     

//bt_gain_table_7
#define RF_IF_BT_RXFLT_GAIN2_F(n)      (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_GAIN1_F(n)      (((n)&0x3)<<2)      
#define RF_IF_BT_RMX_GAIN_F(n)         (((n)&0xF)<<4)      
#define RF_IF_BT_LNA_GAIN2_F(n)        (((n)&0x3)<<8)      
#define RF_IF_BT_LNA_GAIN1_F(n)        (((n)&0xF)<<10)     
#define RF_IF_BT_RXFLT_GAIN2_E(n)      (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_GAIN1_E(n)      (((n)&0x3)<<18)     
#define RF_IF_BT_RMX_GAIN_E(n)         (((n)&0xF)<<20)     
#define RF_IF_BT_LNA_GAIN2_E(n)        (((n)&0x3)<<24)     
#define RF_IF_BT_LNA_GAIN1_E(n)        (((n)&0xF)<<26)     

//bt_ibit_table_0
#define RF_IF_BT_LNA_IBIT2_3(n)        (((n)&0xF)<<0)      
#define RF_IF_BT_LNA_IBIT1_3(n)        (((n)&0x7)<<4)      
#define RF_IF_BT_LNA_IBIT2_2(n)        (((n)&0xF)<<8)      
#define RF_IF_BT_LNA_IBIT1_2(n)        (((n)&0x7)<<12)     
#define RF_IF_BT_LNA_IBIT2_1(n)        (((n)&0xF)<<16)     
#define RF_IF_BT_LNA_IBIT1_1(n)        (((n)&0x7)<<20)     
#define RF_IF_BT_LNA_IBIT2_0(n)        (((n)&0xF)<<24)     
#define RF_IF_BT_LNA_IBIT1_0(n)        (((n)&0x7)<<28)     

//bt_ibit_table_1
#define RF_IF_BT_LNA_IBIT2_7(n)        (((n)&0xF)<<0)      
#define RF_IF_BT_LNA_IBIT1_7(n)        (((n)&0x7)<<4)      
#define RF_IF_BT_LNA_IBIT2_6(n)        (((n)&0xF)<<8)      
#define RF_IF_BT_LNA_IBIT1_6(n)        (((n)&0x7)<<12)     
#define RF_IF_BT_LNA_IBIT2_5(n)        (((n)&0xF)<<16)     
#define RF_IF_BT_LNA_IBIT1_5(n)        (((n)&0x7)<<20)     
#define RF_IF_BT_LNA_IBIT2_4(n)        (((n)&0xF)<<24)     
#define RF_IF_BT_LNA_IBIT1_4(n)        (((n)&0x7)<<28)     

//bt_ibit_table_2
#define RF_IF_BT_LNA_IBIT2_B(n)        (((n)&0xF)<<0)      
#define RF_IF_BT_LNA_IBIT1_B(n)        (((n)&0x7)<<4)      
#define RF_IF_BT_LNA_IBIT2_A(n)        (((n)&0xF)<<8)      
#define RF_IF_BT_LNA_IBIT1_A(n)        (((n)&0x7)<<12)     
#define RF_IF_BT_LNA_IBIT2_9(n)        (((n)&0xF)<<16)     
#define RF_IF_BT_LNA_IBIT1_9(n)        (((n)&0x7)<<20)     
#define RF_IF_BT_LNA_IBIT2_8(n)        (((n)&0xF)<<24)     
#define RF_IF_BT_LNA_IBIT1_8(n)        (((n)&0x7)<<28)     

//bt_ibit_table_3
#define RF_IF_BT_LNA_IBIT2_F(n)        (((n)&0xF)<<0)      
#define RF_IF_BT_LNA_IBIT1_F(n)        (((n)&0x7)<<4)      
#define RF_IF_BT_LNA_IBIT2_E(n)        (((n)&0xF)<<8)      
#define RF_IF_BT_LNA_IBIT1_E(n)        (((n)&0x7)<<12)     
#define RF_IF_BT_LNA_IBIT2_D(n)        (((n)&0xF)<<16)     
#define RF_IF_BT_LNA_IBIT1_D(n)        (((n)&0x7)<<20)     
#define RF_IF_BT_LNA_IBIT2_C(n)        (((n)&0xF)<<24)     
#define RF_IF_BT_LNA_IBIT1_C(n)        (((n)&0x7)<<28)     

//bt_ibit_table_4
#define RF_IF_BT_RXFLT_OP_IBIT_F(n)    (((n)&0x3)<<0)      
#define RF_IF_BT_RXFLT_OP_IBIT_E(n)    (((n)&0x3)<<2)      
#define RF_IF_BT_RXFLT_OP_IBIT_D(n)    (((n)&0x3)<<4)      
#define RF_IF_BT_RXFLT_OP_IBIT_C(n)    (((n)&0x3)<<6)      
#define RF_IF_BT_RXFLT_OP_IBIT_B(n)    (((n)&0x3)<<8)      
#define RF_IF_BT_RXFLT_OP_IBIT_A(n)    (((n)&0x3)<<10)     
#define RF_IF_BT_RXFLT_OP_IBIT_9(n)    (((n)&0x3)<<12)     
#define RF_IF_BT_RXFLT_OP_IBIT_8(n)    (((n)&0x3)<<14)     
#define RF_IF_BT_RXFLT_OP_IBIT_7(n)    (((n)&0x3)<<16)     
#define RF_IF_BT_RXFLT_OP_IBIT_6(n)    (((n)&0x3)<<18)     
#define RF_IF_BT_RXFLT_OP_IBIT_5(n)    (((n)&0x3)<<20)     
#define RF_IF_BT_RXFLT_OP_IBIT_4(n)    (((n)&0x3)<<22)     
#define RF_IF_BT_RXFLT_OP_IBIT_3(n)    (((n)&0x3)<<24)     
#define RF_IF_BT_RXFLT_OP_IBIT_2(n)    (((n)&0x3)<<26)     
#define RF_IF_BT_RXFLT_OP_IBIT_1(n)    (((n)&0x3)<<28)     
#define RF_IF_BT_RXFLT_OP_IBIT_0(n)    (((n)&0x3)<<30)     

//bt_padrv_tmx_gain_table_0
#define RF_IF_BT_PADRV_GAIN_3(n)       (((n)&0x7)<<0)      
#define RF_IF_BT_TMX_GAIN_3(n)         (((n)&0x7)<<3)      
#define RF_IF_BT_PADRV_GAIN_2(n)       (((n)&0x7)<<8)      
#define RF_IF_BT_TMX_GAIN_2(n)         (((n)&0x7)<<11)     
#define RF_IF_BT_PADRV_GAIN_1(n)       (((n)&0x7)<<16)     
#define RF_IF_BT_TMX_GAIN_1(n)         (((n)&0x7)<<19)     
#define RF_IF_BT_PADRV_GAIN_0(n)       (((n)&0x7)<<24)     
#define RF_IF_BT_TMX_GAIN_0(n)         (((n)&0x7)<<27)     

//bt_padrv_tmx_gain_table_1
#define RF_IF_BT_PADRV_GAIN_7(n)       (((n)&0x7)<<0)      
#define RF_IF_BT_TMX_GAIN_7(n)         (((n)&0x7)<<3)      
#define RF_IF_BT_PADRV_GAIN_6(n)       (((n)&0x7)<<8)      
#define RF_IF_BT_TMX_GAIN_6(n)         (((n)&0x7)<<11)     
#define RF_IF_BT_PADRV_GAIN_5(n)       (((n)&0x7)<<16)     
#define RF_IF_BT_TMX_GAIN_5(n)         (((n)&0x7)<<19)     
#define RF_IF_BT_PADRV_GAIN_4(n)       (((n)&0x7)<<24)     
#define RF_IF_BT_TMX_GAIN_4(n)         (((n)&0x7)<<27)     

//fm_lna_gain_table
#define RF_IF_FM_LNA_GAIN_BIT_F(n)     (((n)&0x3)<<0)      
#define RF_IF_FM_LNA_GAIN_BIT_E(n)     (((n)&0x3)<<2)      
#define RF_IF_FM_LNA_GAIN_BIT_D(n)     (((n)&0x3)<<4)      
#define RF_IF_FM_LNA_GAIN_BIT_C(n)     (((n)&0x3)<<6)      
#define RF_IF_FM_LNA_GAIN_BIT_B(n)     (((n)&0x3)<<8)      
#define RF_IF_FM_LNA_GAIN_BIT_A(n)     (((n)&0x3)<<10)     
#define RF_IF_FM_LNA_GAIN_BIT_9(n)     (((n)&0x3)<<12)     
#define RF_IF_FM_LNA_GAIN_BIT_8(n)     (((n)&0x3)<<14)     
#define RF_IF_FM_LNA_GAIN_BIT_7(n)     (((n)&0x3)<<16)     
#define RF_IF_FM_LNA_GAIN_BIT_6(n)     (((n)&0x3)<<18)     
#define RF_IF_FM_LNA_GAIN_BIT_5(n)     (((n)&0x3)<<20)     
#define RF_IF_FM_LNA_GAIN_BIT_4(n)     (((n)&0x3)<<22)     
#define RF_IF_FM_LNA_GAIN_BIT_3(n)     (((n)&0x3)<<24)     
#define RF_IF_FM_LNA_GAIN_BIT_2(n)     (((n)&0x3)<<26)     
#define RF_IF_FM_LNA_GAIN_BIT_1(n)     (((n)&0x3)<<28)     
#define RF_IF_FM_LNA_GAIN_BIT_0(n)     (((n)&0x3)<<30)     

//fm_rmx_gain_table_0
#define RF_IF_FM_RMX_GAIN_BIT_7(n)     (((n)&0x7)<<0)      
#define RF_IF_FM_RMX_GAIN_BIT_6(n)     (((n)&0x7)<<4)      
#define RF_IF_FM_RMX_GAIN_BIT_5(n)     (((n)&0x7)<<8)      
#define RF_IF_FM_RMX_GAIN_BIT_4(n)     (((n)&0x7)<<12)     
#define RF_IF_FM_RMX_GAIN_BIT_3(n)     (((n)&0x7)<<16)     
#define RF_IF_FM_RMX_GAIN_BIT_2(n)     (((n)&0x7)<<20)     
#define RF_IF_FM_RMX_GAIN_BIT_1(n)     (((n)&0x7)<<24)     
#define RF_IF_FM_RMX_GAIN_BIT_0(n)     (((n)&0x7)<<28)     
#define RF_IF_FM_AGC_GAIN_DR           (1<<31)             

//fm_rmx_gain_table_1
#define RF_IF_FM_RMX_GAIN_BIT_F(n)     (((n)&0x7)<<0)      
#define RF_IF_FM_RMX_GAIN_BIT_E(n)     (((n)&0x7)<<4)      
#define RF_IF_FM_RMX_GAIN_BIT_D(n)     (((n)&0x7)<<8)      
#define RF_IF_FM_RMX_GAIN_BIT_C(n)     (((n)&0x7)<<12)     
#define RF_IF_FM_RMX_GAIN_BIT_B(n)     (((n)&0x7)<<16)     
#define RF_IF_FM_RMX_GAIN_BIT_A(n)     (((n)&0x7)<<20)     
#define RF_IF_FM_RMX_GAIN_BIT_9(n)     (((n)&0x7)<<24)     
#define RF_IF_FM_RMX_GAIN_BIT_8(n)     (((n)&0x7)<<28)     

//fm_rxflt_gain_table
#define RF_IF_FM_RXFLT_GAIN_BIT_F(n)   (((n)&0x3)<<0)      
#define RF_IF_FM_RXFLT_GAIN_BIT_E(n)   (((n)&0x3)<<2)      
#define RF_IF_FM_RXFLT_GAIN_BIT_D(n)   (((n)&0x3)<<4)      
#define RF_IF_FM_RXFLT_GAIN_BIT_C(n)   (((n)&0x3)<<6)      
#define RF_IF_FM_RXFLT_GAIN_BIT_B(n)   (((n)&0x3)<<8)      
#define RF_IF_FM_RXFLT_GAIN_BIT_A(n)   (((n)&0x3)<<10)     
#define RF_IF_FM_RXFLT_GAIN_BIT_9(n)   (((n)&0x3)<<12)     
#define RF_IF_FM_RXFLT_GAIN_BIT_8(n)   (((n)&0x3)<<14)     
#define RF_IF_FM_RXFLT_GAIN_BIT_7(n)   (((n)&0x3)<<16)     
#define RF_IF_FM_RXFLT_GAIN_BIT_6(n)   (((n)&0x3)<<18)     
#define RF_IF_FM_RXFLT_GAIN_BIT_5(n)   (((n)&0x3)<<20)     
#define RF_IF_FM_RXFLT_GAIN_BIT_4(n)   (((n)&0x3)<<22)     
#define RF_IF_FM_RXFLT_GAIN_BIT_3(n)   (((n)&0x3)<<24)     
#define RF_IF_FM_RXFLT_GAIN_BIT_2(n)   (((n)&0x3)<<26)     
#define RF_IF_FM_RXFLT_GAIN_BIT_1(n)   (((n)&0x3)<<28)     
#define RF_IF_FM_RXFLT_GAIN_BIT_0(n)   (((n)&0x3)<<30)     

//test_buffer_setting
#define RF_IF_AUX_OUT_MODE             (1<<0)              
#define RF_IF_AUX_FLT_EN               (1<<1)              
#define RF_IF_TBUF_VTR_SEL             (1<<2)              
#define RF_IF_TBUF_OP_IBIT(n)          (((n)&0x3)<<3)      
#define RF_IF_TBUF_MUX_EN(n)           (((n)&0x3)<<5)      
#define RF_IF_TBUF_RANGE_BIT(n)        (((n)&0x3)<<7)      
#define RF_IF_TBUF_CM_BIT(n)           (((n)&0x3)<<9)      
#define RF_IF_TBUF_RSTN_REG            (1<<11)             
#define RF_IF_TBUF_RSTN_DR             (1<<12)             

//reserved_reg
#define RF_IF_ANA_RSVD(n)              (((n)&0xFFFF)<<0)   
#define RF_IF_DIG_RSVD(n)              (((n)&0xFFFF)<<16)  

//channel_setting
#define RF_IF_BT_TMX_LOBIAS(n)         (((n)&0x1F)<<0)     
#define RF_IF_BT_TMX_CAP_BIT(n)        (((n)&0xF)<<5)      
#define RF_IF_BT_RMX_LO_VCOM_BIT(n)    (((n)&0x3)<<9)      
#define RF_IF_BT_LNA_CLOAD_BIT(n)      (((n)&0x7)<<11)     

//reserved_in_reg
#define RF_IF_RF_RSVD_IN(n)            (((n)&0xFFFF)<<0)   

//sys_clk_cfg
#define RF_IF_DIG_RSVD_2(n)            (((n)&0xFFFF)<<0)   


#endif


