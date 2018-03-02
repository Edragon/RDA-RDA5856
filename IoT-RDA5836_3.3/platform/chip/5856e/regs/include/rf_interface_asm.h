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
// File name      : rf_interface_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:52                                                  
// Last modified  : 2016-08-29 14:59:52                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:52 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __RF_INTERFACE_ASM_H__
#define __RF_INTERFACE_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_revision                   REG_RF_INTERFACE_BASE+0x00
#define REG_sys_control                REG_RF_INTERFACE_BASE+0x01
#define REG_bt_control                 REG_RF_INTERFACE_BASE+0x02
#define REG_bt_status                  REG_RF_INTERFACE_BASE+0x03
#define REG_fm_control0                REG_RF_INTERFACE_BASE+0x04
#define REG_fm_control1                REG_RF_INTERFACE_BASE+0x05
#define REG_fm_control2                REG_RF_INTERFACE_BASE+0x06
#define REG_fm_status                  REG_RF_INTERFACE_BASE+0x07
#define REG_pll_freq_setting0          REG_RF_INTERFACE_BASE+0x08
#define REG_pll_freq_setting1          REG_RF_INTERFACE_BASE+0x09
#define REG_bt_lo_freq_setting         REG_RF_INTERFACE_BASE+0x0a
#define REG_fm_lo_freq_setting         REG_RF_INTERFACE_BASE+0x0b
#define REG_bt_lna_setting             REG_RF_INTERFACE_BASE+0x0c
#define REG_bt_rmx_setting             REG_RF_INTERFACE_BASE+0x0d
#define REG_bt_rxflt_setting0          REG_RF_INTERFACE_BASE+0x0e
#define REG_bt_self_cal_setting0       REG_RF_INTERFACE_BASE+0x0f
#define REG_bt_rx_cal_setting          REG_RF_INTERFACE_BASE+0x10
#define REG_bt_rx_cal_status           REG_RF_INTERFACE_BASE+0x11
#define REG_bt_adc_setting             REG_RF_INTERFACE_BASE+0x12
#define REG_bt_padrv_setting0          REG_RF_INTERFACE_BASE+0x13
#define REG_bt_padrv_setting1          REG_RF_INTERFACE_BASE+0x14
#define REG_bt_bbdac_setting0          REG_RF_INTERFACE_BASE+0x15
#define REG_fm_lna_mixer_setting       REG_RF_INTERFACE_BASE+0x16
#define REG_fm_filter_setting0         REG_RF_INTERFACE_BASE+0x17
#define REG_fm_filter_setting1         REG_RF_INTERFACE_BASE+0x18
#define REG_fm_filter_setting2         REG_RF_INTERFACE_BASE+0x19
#define REG_fm_filter_status           REG_RF_INTERFACE_BASE+0x1a
#define REG_fm_adc_setting             REG_RF_INTERFACE_BASE+0x1b
#define REG_fm_dsp_setting             REG_RF_INTERFACE_BASE+0x1c
#define REG_vco_setting0               REG_RF_INTERFACE_BASE+0x1d
#define REG_vco_setting1               REG_RF_INTERFACE_BASE+0x1e
#define REG_pll_setting0               REG_RF_INTERFACE_BASE+0x1f
#define REG_pll_setting1               REG_RF_INTERFACE_BASE+0x20
#define REG_pll_setting2               REG_RF_INTERFACE_BASE+0x21
#define REG_pll_setting3               REG_RF_INTERFACE_BASE+0x22
#define REG_pll_setting4               REG_RF_INTERFACE_BASE+0x23
#define REG_pll_setting5               REG_RF_INTERFACE_BASE+0x24
#define REG_pll_status                 REG_RF_INTERFACE_BASE+0x25
#define REG_vco_cal_setting            REG_RF_INTERFACE_BASE+0x26
#define REG_pll_cal_setting            REG_RF_INTERFACE_BASE+0x27
#define REG_sdm_setting                REG_RF_INTERFACE_BASE+0x28
#define REG_cal_dr_setting             REG_RF_INTERFACE_BASE+0x29
#define REG_ldo_setting                REG_RF_INTERFACE_BASE+0x2a
#define REG_power_status0              REG_RF_INTERFACE_BASE+0x2b
#define REG_power_status1              REG_RF_INTERFACE_BASE+0x2c
#define REG_power_reg0                 REG_RF_INTERFACE_BASE+0x2d
#define REG_power_reg1                 REG_RF_INTERFACE_BASE+0x2e
#define REG_bt_gain_table_0            REG_RF_INTERFACE_BASE+0x2f
#define REG_bt_gain_table_1            REG_RF_INTERFACE_BASE+0x30
#define REG_bt_gain_table_2            REG_RF_INTERFACE_BASE+0x31
#define REG_bt_gain_table_3            REG_RF_INTERFACE_BASE+0x32
#define REG_bt_gain_table_4            REG_RF_INTERFACE_BASE+0x33
#define REG_bt_gain_table_5            REG_RF_INTERFACE_BASE+0x34
#define REG_bt_gain_table_6            REG_RF_INTERFACE_BASE+0x35
#define REG_bt_gain_table_7            REG_RF_INTERFACE_BASE+0x36
#define REG_bt_gain_table_8            REG_RF_INTERFACE_BASE+0x37
#define REG_bt_gain_table_9            REG_RF_INTERFACE_BASE+0x38
#define REG_bt_gain_table_a            REG_RF_INTERFACE_BASE+0x39
#define REG_bt_gain_table_b            REG_RF_INTERFACE_BASE+0x3a
#define REG_bt_gain_table_c            REG_RF_INTERFACE_BASE+0x3b
#define REG_bt_gain_table_d            REG_RF_INTERFACE_BASE+0x3c
#define REG_bt_gain_table_e            REG_RF_INTERFACE_BASE+0x3d
#define REG_bt_gain_table_f            REG_RF_INTERFACE_BASE+0x3e
#define REG_bt_ibit_table_0            REG_RF_INTERFACE_BASE+0x3f
#define REG_bt_ibit_table_1            REG_RF_INTERFACE_BASE+0x40
#define REG_bt_ibit_table_2            REG_RF_INTERFACE_BASE+0x41
#define REG_bt_ibit_table_3            REG_RF_INTERFACE_BASE+0x42
#define REG_bt_ibit_table_4            REG_RF_INTERFACE_BASE+0x43
#define REG_bt_ibit_table_5            REG_RF_INTERFACE_BASE+0x44
#define REG_bt_ibit_table_6            REG_RF_INTERFACE_BASE+0x45
#define REG_bt_ibit_table_7            REG_RF_INTERFACE_BASE+0x46
#define REG_bt_ibit_table_8            REG_RF_INTERFACE_BASE+0x47
#define REG_bt_ibit_table_9            REG_RF_INTERFACE_BASE+0x48
#define REG_tx_padrv_gain_setting_0    REG_RF_INTERFACE_BASE+0x49
#define REG_tx_padrv_gain_setting_1    REG_RF_INTERFACE_BASE+0x4a
#define REG_tx_padrv_gain_setting_2    REG_RF_INTERFACE_BASE+0x4b
#define REG_tx_padrv_gain_setting_3    REG_RF_INTERFACE_BASE+0x4c
#define REG_tx_padrv_gain_setting_4    REG_RF_INTERFACE_BASE+0x4d
#define REG_tx_padrv_gain_setting_5    REG_RF_INTERFACE_BASE+0x4e
#define REG_tx_padrv_gain_setting_6    REG_RF_INTERFACE_BASE+0x4f
#define REG_tx_padrv_gain_setting_7    REG_RF_INTERFACE_BASE+0x50
#define REG_fm_lna_gain_setting0       REG_RF_INTERFACE_BASE+0x51
#define REG_fm_lna_gain_setting1       REG_RF_INTERFACE_BASE+0x52
#define REG_fm_mixer_gain_setting0     REG_RF_INTERFACE_BASE+0x53
#define REG_fm_mixer_gain_setting1     REG_RF_INTERFACE_BASE+0x54
#define REG_fm_mixer_gain_setting2     REG_RF_INTERFACE_BASE+0x55
#define REG_fm_mixer_gain_setting3     REG_RF_INTERFACE_BASE+0x56
#define REG_fm_filter_gain_setting0    REG_RF_INTERFACE_BASE+0x57
#define REG_fm_filter_gain_setting1    REG_RF_INTERFACE_BASE+0x58
#define REG_fm_adc_gain_setting0       REG_RF_INTERFACE_BASE+0x59
#define REG_fm_adc_gain_setting1       REG_RF_INTERFACE_BASE+0x5a
#define REG_sys_clk_cfg       REG_RF_INTERFACE_BASE+0xe8

//sys_clk_cfg
#define SYS_CTRL_SYS_CLK_SEL_FAST_SLOW  (0<<3) 

//revision
#define RF_INTERFACE_REVID(n)          (((n)&0xF)<<0)      
#define RF_INTERFACE_CHIPID(n)         (((n)&0xFFF)<<4)    

//sys_control
#define RF_INTERFACE_SOFT_RESETN       (1<<0)              
#define RF_INTERFACE_CHIP_SELF_CAL_ENABLE (1<<1)              
#define RF_INTERFACE_PLL_FREQ_MODE     (1<<2)              
#define RF_INTERFACE_ZF_NZF            (1<<3)              
#define RF_INTERFACE_PD_ALL_B_REG      (1<<4)              
#define RF_INTERFACE_DIRECT_REG        (1<<5)              
#define RF_INTERFACE_TUNE_DIFF_EN      (1<<6)              
#define RF_INTERFACE_CLK_DIGITAL_ENABLE_REG (1<<7)              
#define RF_INTERFACE_RF_MODE           (1<<8)              

//bt_control
#define RF_INTERFACE_ARFCN(n)          (((n)&0x7F)<<0)     
#define RF_INTERFACE_APC(n)            (((n)&0xF)<<7)      

//bt_status
#define RF_INTERFACE_BT_SELF_CAL_READY (1<<0)              

//fm_control0
#define RF_INTERFACE_TUNE              (1<<0)              
#define RF_INTERFACE_SEEK              (1<<1)              
#define RF_INTERFACE_SKMODE            (1<<2)              
#define RF_INTERFACE_SEEKUP            (1<<3)              
#define RF_INTERFACE_BAND_SEL(n)       (((n)&0x3)<<4)      
#define RF_INTERFACE_SPACE_SEL(n)      (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_FREQ_MODE      (1<<8)              

//fm_control1
#define RF_INTERFACE_CHAN_REG(n)       (((n)&0x3FF)<<0)    

//fm_control2
#define RF_INTERFACE_FM_FREQ_DIRECT(n) (((n)&0xFFFF)<<0)   

//fm_status
#define RF_INTERFACE_FM_SELF_CAL_READY (1<<0)              
#define RF_INTERFACE_STC               (1<<1)              
#define RF_INTERFACE_SF                (1<<2)              
#define RF_INTERFACE_FM_TRUE           (1<<3)              
#define RF_INTERFACE_FM_READY          (1<<4)              
#define RF_INTERFACE_READCHAN(n)       (((n)&0x3FF)<<5)    

//pll_freq_setting0
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_MSB(n) (((n)&0x7FFF)<<0)   

//pll_freq_setting1
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_LSB(n) (((n)&0xFFFF)<<0)   

//bt_lo_freq_setting
#define RF_INTERFACE_BT_DIGITAL_LO_FREQ(n) (((n)&0x7FF)<<0)    
#define RF_INTERFACE_BT_IMGREJ         (1<<11)             

//fm_lo_freq_setting
#define RF_INTERFACE_FM_DIGITAL_LO_FREQ(n) (((n)&0x7FF)<<0)    
#define RF_INTERFACE_FM_IMGREJ         (1<<11)             

//bt_lna_setting
#define RF_INTERFACE_BT_LNA_CLOAD_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_LNA_REG_BYPASS (1<<3)              
#define RF_INTERFACE_BT_LNA_REG_BIT(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_BALUN_CBIT_TX(n) (((n)&0x7)<<7)      
#define RF_INTERFACE_BT_BALUN_CBIT_RX(n) (((n)&0x7)<<10)     

//bt_rmx_setting
#define RF_INTERFACE_BT_RMX_REG_BYPASS (1<<0)              
#define RF_INTERFACE_BT_RMX_LO_VCOM_BIT(n) (((n)&0x3)<<1)      
#define RF_INTERFACE_BT_RMX_REG_BIT(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_BT_RMX_IFVCM_SEL  (1<<6)              
#define RF_INTERFACE_BT_RMX_IQSWAP     (1<<7)              
#define RF_INTERFACE_BT_RMX_LO_IFVCOM_BIT(n) (((n)&0xF)<<8)      

//bt_rxflt_setting0
#define RF_INTERFACE_BT_RXFLT_BP_MODE(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_OFFSET_RANGE_BIT(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_BT_RXFLT_CAP_BIT(n) (((n)&0xF)<<6)      
#define RF_INTERFACE_BT_RXFLT_CAL_MODE (1<<10)             
#define RF_INTERFACE_BT_RXFLT_RSTN_DR  (1<<11)             
#define RF_INTERFACE_BT_RXFLT_RSTN_REG (1<<12)             
#define RF_INTERFACE_BT_RXFLT_CALCLK_EDGE_SEL (1<<13)             

//bt_self_cal_setting0
#define RF_INTERFACE_BT_RX_CAL_IQSWAP  (1<<0)              
#define RF_INTERFACE_BT_RX_CAL_POLARITY (1<<1)              
#define RF_INTERFACE_BT_RXFLT_OFFSET_BIT_DR (1<<2)              
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_DC_TABLE_FULL_MODE (1<<5)              
#define RF_INTERFACE_BT_RX_CAL_CNT(n)  (((n)&0xF)<<6)      
#define RF_INTERFACE_BT_DC_CAL_MODE(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_RX_CAL_CLK_EDGE (1<<12)             

//bt_rx_cal_setting
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_REG(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_REG(n) (((n)&0xFF)<<8)     

//bt_rx_cal_status
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT(n) (((n)&0xFF)<<8)     

//bt_adc_setting
#define RF_INTERFACE_BT_ADC_CLK_EDGE_SEL (1<<0)              
#define RF_INTERFACE_BT_ADC_CLK_DIV2   (1<<1)              
#define RF_INTERFACE_BT_ADC_DELAY_BIT(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_ADC_VREF_BIT(n) (((n)&0x3)<<4)      

//bt_padrv_setting0
#define RF_INTERFACE_BT_PADRV_IBIT(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_BT_TMX_LOBIAS(n)  (((n)&0x1F)<<6)     
#define RF_INTERFACE_BT_TMX_IBIT(n)    (((n)&0x3)<<11)     

//bt_padrv_setting1
#define RF_INTERFACE_BT_TMX_CAP_BIT(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_TMX_PLLTEST_EN (1<<4)              
#define RF_INTERFACE_BT_TMX_BBTEST_EN  (1<<5)              
#define RF_INTERFACE_BT_DELAY_PADRV(n) (((n)&0xFF)<<6)     

//bt_bbdac_setting0
#define RF_INTERFACE_BT_BBDAC_VLOW_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_BBDAC_DVDD_SEL (1<<3)              
#define RF_INTERFACE_BT_BBDAC_CLK_EDGE_SEL (1<<4)              
#define RF_INTERFACE_BT_BBDAC_VTR_SEL  (1<<5)              
#define RF_INTERFACE_BT_BBDAC_OUT_EN   (1<<6)              
#define RF_INTERFACE_BT_BBDAC_TEST     (1<<7)              

//fm_lna_mixer_setting
#define RF_INTERFACE_FM_LNA_IBIT(n)    (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_LNA_CFG_BIT(n) (((n)&0xF)<<3)      
#define RF_INTERFACE_FM_MIXER_LO_BIAS_BIT(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_FM_MIXER_HARM_REJ_EN (1<<9)              

//fm_filter_setting0
#define RF_INTERFACE_FM_FILTER_IQSWAP  (1<<0)              
#define RF_INTERFACE_FM_FILTER_BW_BIT(n) (((n)&0x3)<<1)      
#define RF_INTERFACE_FM_FILTER_OP_IBIT(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_FM_FILTER_CAL_MODE (1<<5)              
#define RF_INTERFACE_FM_FILTER_CAL_RANGE(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_FILTER_CAL_CLK_EDGE_INN (1<<9)              
#define RF_INTERFACE_FM_FILTER_CAL_CLK_EDGE (1<<10)             
#define RF_INTERFACE_FM_DC_CAL_INIT_DEALY(n) (((n)&0x3)<<11)     
#define RF_INTERFACE_FM_FILTER_CAL_POLARITY (1<<13)             
#define RF_INTERFACE_FM_DCOFFSET_CAL_ONETIME (1<<14)             
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_DR (1<<15)             

//fm_filter_setting1
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_REG(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_REG(n) (((n)&0xFF)<<8)     

//fm_filter_setting2
#define RF_INTERFACE_FM_FILTER_RESETN_REG (1<<0)              
#define RF_INTERFACE_FM_FILTER_RESETN_DR (1<<1)              
#define RF_INTERFACE_FM_DC_CAL_CNT(n)  (((n)&0xF)<<2)      

//fm_filter_status
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q(n) (((n)&0xFF)<<8)     

//fm_adc_setting
#define RF_INTERFACE_FM_ADC_ISEL_BIT(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_IQSWAP     (1<<2)              
#define RF_INTERFACE_FM_ADC_DAC_BIAS_BIT(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_FM_ADC_LP_MODE_EN (1<<5)              
#define RF_INTERFACE_FM_ADC_TEST_ENABLE (1<<6)              
#define RF_INTERFACE_FM_ADC_CAL_POLARITY (1<<7)              
#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_DR (1<<8)              
#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_REG(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_ADC_RESETN_DR  (1<<12)             
#define RF_INTERFACE_FM_ADC_RESETN_REG (1<<13)             

//fm_dsp_setting
#define RF_INTERFACE_FM_DSP_RESETN_REG (1<<0)              
#define RF_INTERFACE_FM_DSP_RESETN_DR  (1<<1)              
#define RF_INTERFACE_FM_DSP_RESETN_DELAY(n) (((n)&0x3)<<2)      

//vco_setting0
#define RF_INTERFACE_VCO_PKD_REF_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_VCO_REG_BYPASS    (1<<3)              
#define RF_INTERFACE_VCO_DIV2_BIAS_BIT(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_VCO_BUF_BIAS_IBIT(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_VCO_BUF_IBIT(n)   (((n)&0xF)<<11)     

//vco_setting1
#define RF_INTERFACE_VCO_IBIT_REG(n)   (((n)&0xF)<<0)      
#define RF_INTERFACE_VCO_IBIT_DR       (1<<4)              

//pll_setting0
#define RF_INTERFACE_PLL_BYPASS_CP_NA  (1<<0)              
#define RF_INTERFACE_PLL_BYPASS_NOTCH  (1<<1)              
#define RF_INTERFACE_PLL_PRESC_REG_BIT(n) (((n)&0xF)<<2)      
#define RF_INTERFACE_PLL_SDM_CLK_SEL_REG (1<<6)              
#define RF_INTERFACE_PLL_SDM_CLK_SEL_DR (1<<7)              
#define RF_INTERFACE_PLL_REFMULTI2_EN  (1<<8)              
#define RF_INTERFACE_PLL_OPEN_EN       (1<<9)              
#define RF_INTERFACE_PLL_CAL_CLK_SEL   (1<<10)             
#define RF_INTERFACE_PLL_CP_R_BIT_RX(n) (((n)&0x3)<<11)     
#define RF_INTERFACE_PLL_CP_R_BIT_TX(n) (((n)&0x3)<<13)     

//pll_setting1
#define RF_INTERFACE_VCO_BAND_REG(n)   (((n)&0x7FF)<<0)    
#define RF_INTERFACE_VCO_BAND_DR       (1<<11)             
#define RF_INTERFACE_PLL_CAL_BIT(n)    (((n)&0x3)<<12)     

//pll_setting2
#define RF_INTERFACE_PLL_CP_BIT_RX(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_GAIN_BIT_RX(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_PLL_FBC_SEL_RX(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_MDLL_DIV_RX(n)    (((n)&0xF)<<12)     

//pll_setting3
#define RF_INTERFACE_PLL_CP_BIT_TX(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_GAIN_BIT_TX(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_PLL_FBC_SEL_TX(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_MDLL_DIV_TX(n)    (((n)&0xF)<<12)     

//pll_setting4
#define RF_INTERFACE_PLL_PFD_RES_BIT_RX(n) (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_RX(n) (((n)&0x1F)<<6)     
#define RF_INTERFACE_PLL_R_BIT_RX(n)   (((n)&0x1F)<<11)    

//pll_setting5
#define RF_INTERFACE_PLL_PFD_RES_BIT_TX(n) (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_TX(n) (((n)&0x1F)<<6)     
#define RF_INTERFACE_PLL_R_BIT_TX(n)   (((n)&0x1F)<<11)    

//pll_status
#define RF_INTERFACE_PLL_LOCK_DET      (1<<0)              

//vco_cal_setting
#define RF_INTERFACE_VCO_PKD_CLK_EDGE_SEL (1<<0)              
#define RF_INTERFACE_VCO_CAL_POLARITY  (1<<1)              
#define RF_INTERFACE_VCO_CAL_INIT_DELAY(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_VCO_CAL_WAIT_CNT(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_VCO_CAL_CNT(n)    (((n)&0x3)<<6)      
#define RF_INTERFACE_VCO_CAL_MODE      (1<<8)              

//pll_cal_setting
#define RF_INTERFACE_PLL_INIT_DELAY(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_OPT       (1<<6)              
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME(n) (((n)&0x7)<<7)      
#define RF_INTERFACE_PLL_CNT_ENABLE_POLARITY (1<<10)             
#define RF_INTERFACE_RESET_PLL_LOCK_DELAY(n) (((n)&0x3)<<11)     

//sdm_setting
#define RF_INTERFACE_SDM_DELAY_SEL(n)  (((n)&0xF)<<0)      
#define RF_INTERFACE_SDM_DITHER_BYPASS (1<<4)              
#define RF_INTERFACE_SDM_INT_DEC_SEL(n) (((n)&0x3)<<5)      
#define RF_INTERFACE_SDM_CLK_EDGE      (1<<7)              
#define RF_INTERFACE_PLL_SDM_RESETN_REG (1<<8)              
#define RF_INTERFACE_PLL_SDM_RESETN_DR (1<<9)              
#define RF_INTERFACE_RESET_PLL_SDM_DELAY(n) (((n)&0x3)<<10)     

//cal_dr_setting
#define RF_INTERFACE_SELF_CAL_READY_REG (1<<0)              
#define RF_INTERFACE_SELF_CAL_READY_DR (1<<1)              
#define RF_INTERFACE_PLL_CAL_RESETN_REG (1<<2)              
#define RF_INTERFACE_PLL_CAL_RESETN_DR (1<<3)              
#define RF_INTERFACE_BT_DC_CAL_FSM_RESETN_REG (1<<4)              
#define RF_INTERFACE_BT_DC_CAL_FSM_RESETN_DR (1<<5)              
#define RF_INTERFACE_FM_DC_CAL_FSM_RESETN_REG (1<<6)              
#define RF_INTERFACE_FM_DC_CAL_FSM_RESETN_DR (1<<7)              
#define RF_INTERFACE_FM_ADC_CAL_FSM_RESETN_REG (1<<8)              
#define RF_INTERFACE_FM_ADC_CAL_FSM_RESETN_DR (1<<9)              
#define RF_INTERFACE_VCO_CAL_RESETN_REG (1<<10)             
#define RF_INTERFACE_VCO_CAL_RESETN_DR (1<<11)             

//ldo_setting
#define RF_INTERFACE_IVREF_AVDD_SEL    (1<<0)              
#define RF_INTERFACE_PD_IVREF_REG      (1<<1)              
#define RF_INTERFACE_PD_IVREF_DR       (1<<2)              

//power_status0
#define RF_INTERFACE_BT_PD_BBDAC       (1<<0)              
#define RF_INTERFACE_BT_PD_PADRV       (1<<1)              
#define RF_INTERFACE_BT_PD_ADC         (1<<2)              
#define RF_INTERFACE_BT_PD_RXFLT       (1<<3)              
#define RF_INTERFACE_BT_PD_RMX         (1<<4)              
#define RF_INTERFACE_BT_PD_LNA         (1<<5)              
#define RF_INTERFACE_PD_LO_RX          (1<<6)              
#define RF_INTERFACE_PD_LO_TX          (1<<7)              
#define RF_INTERFACE_PD_VCO_PKD        (1<<8)              
#define RF_INTERFACE_PD_VCO            (1<<9)              
#define RF_INTERFACE_PD_PLL_FMCLK      (1<<10)             
#define RF_INTERFACE_PD_PLL_PRESC      (1<<11)             
#define RF_INTERFACE_PD_PLL_DIGI       (1<<12)             
#define RF_INTERFACE_BT_PD_TMX         (1<<13)             

//power_status1
#define RF_INTERFACE_FM_PD_ADC         (1<<0)              
#define RF_INTERFACE_FM_PD_FILTER      (1<<1)              
#define RF_INTERFACE_FM_PD_MIXER       (1<<2)              
#define RF_INTERFACE_FM_PD_LNA         (1<<3)              

//power_reg0
#define RF_INTERFACE_BT_PD_BBDAC_REG   (1<<0)              
#define RF_INTERFACE_BT_PD_PADRV_REG   (1<<1)              
#define RF_INTERFACE_BT_PD_ADC_REG     (1<<2)              
#define RF_INTERFACE_BT_PD_RXFLT_REG   (1<<3)              
#define RF_INTERFACE_BT_PD_RMX_REG     (1<<4)              
#define RF_INTERFACE_BT_PD_LNA_REG     (1<<5)              
#define RF_INTERFACE_PD_LO_RX_REG      (1<<6)              
#define RF_INTERFACE_PD_LO_TX_REG      (1<<7)              
#define RF_INTERFACE_PD_VCO_PKD_REG    (1<<8)              
#define RF_INTERFACE_PD_VCO_REG        (1<<9)              
#define RF_INTERFACE_PD_PLL_FMCLK_REG  (1<<10)             
#define RF_INTERFACE_PD_PLL_PRESC_REG  (1<<11)             
#define RF_INTERFACE_PD_PLL_DIGI_REG   (1<<12)             
#define RF_INTERFACE_BT_PD_TMX_REG     (1<<13)             

//power_reg1
#define RF_INTERFACE_FM_PD_ADC_REG     (1<<0)              
#define RF_INTERFACE_FM_PD_FILTER_REG  (1<<1)              
#define RF_INTERFACE_FM_PD_MIXER_REG   (1<<2)              
#define RF_INTERFACE_FM_PD_LNA_REG     (1<<3)              

//bt_gain_table_0
#define RF_INTERFACE_BT_RXFLT_GAIN_0(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_0(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_0(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_0(n) (((n)&0xF)<<9)      

//bt_gain_table_1
#define RF_INTERFACE_BT_RXFLT_GAIN_1(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_1(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_1(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_1(n) (((n)&0xF)<<9)      

//bt_gain_table_2
#define RF_INTERFACE_BT_RXFLT_GAIN_2(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_2(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_2(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_2(n) (((n)&0xF)<<9)      

//bt_gain_table_3
#define RF_INTERFACE_BT_RXFLT_GAIN_3(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_3(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_3(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_3(n) (((n)&0xF)<<9)      

//bt_gain_table_4
#define RF_INTERFACE_BT_RXFLT_GAIN_4(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_4(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_4(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_4(n) (((n)&0xF)<<9)      

//bt_gain_table_5
#define RF_INTERFACE_BT_RXFLT_GAIN_5(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_5(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_5(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_5(n) (((n)&0xF)<<9)      

//bt_gain_table_6
#define RF_INTERFACE_BT_RXFLT_GAIN_6(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_6(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_6(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_6(n) (((n)&0xF)<<9)      

//bt_gain_table_7
#define RF_INTERFACE_BT_RXFLT_GAIN_7(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_7(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_7(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_7(n) (((n)&0xF)<<9)      

//bt_gain_table_8
#define RF_INTERFACE_BT_RXFLT_GAIN_8(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_8(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_8(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_8(n) (((n)&0xF)<<9)      

//bt_gain_table_9
#define RF_INTERFACE_BT_RXFLT_GAIN_9(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_9(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_9(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_9(n) (((n)&0xF)<<9)      

//bt_gain_table_a
#define RF_INTERFACE_BT_RXFLT_GAIN_A(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_A(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_A(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_A(n) (((n)&0xF)<<9)      

//bt_gain_table_b
#define RF_INTERFACE_BT_RXFLT_GAIN_B(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_B(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_B(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_B(n) (((n)&0xF)<<9)      

//bt_gain_table_c
#define RF_INTERFACE_BT_RXFLT_GAIN_C(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_C(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_C(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_C(n) (((n)&0xF)<<9)      

//bt_gain_table_d
#define RF_INTERFACE_BT_RXFLT_GAIN_D(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_D(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_D(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_D(n) (((n)&0xF)<<9)      

//bt_gain_table_e
#define RF_INTERFACE_BT_RXFLT_GAIN_E(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_E(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_E(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_E(n) (((n)&0xF)<<9)      

//bt_gain_table_f
#define RF_INTERFACE_BT_RXFLT_GAIN_F(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RMX_GAIN_F(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_LNA_GAIN2_F(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN1_F(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_AGC_GAIN_DR    (1<<13)             

//bt_ibit_table_0
#define RF_INTERFACE_BT_LNA_IBIT2_1(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_1(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_0(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_0(n) (((n)&0x7)<<11)     

//bt_ibit_table_1
#define RF_INTERFACE_BT_LNA_IBIT2_3(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_3(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_2(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_2(n) (((n)&0x7)<<11)     

//bt_ibit_table_2
#define RF_INTERFACE_BT_LNA_IBIT2_5(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_5(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_4(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_4(n) (((n)&0x7)<<11)     

//bt_ibit_table_3
#define RF_INTERFACE_BT_LNA_IBIT2_7(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_7(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_6(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_6(n) (((n)&0x7)<<11)     

//bt_ibit_table_4
#define RF_INTERFACE_BT_LNA_IBIT2_9(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_9(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_8(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_8(n) (((n)&0x7)<<11)     

//bt_ibit_table_5
#define RF_INTERFACE_BT_LNA_IBIT2_B(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_B(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_A(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_A(n) (((n)&0x7)<<11)     

//bt_ibit_table_6
#define RF_INTERFACE_BT_LNA_IBIT2_D(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_D(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_C(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_C(n) (((n)&0x7)<<11)     

//bt_ibit_table_7
#define RF_INTERFACE_BT_LNA_IBIT2_F(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT1_F(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT2_E(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT1_E(n) (((n)&0x7)<<11)     

//bt_ibit_table_8
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_7(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_6(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_5(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_4(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_3(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_2(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_1(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_0(n) (((n)&0x3)<<14)     

//bt_ibit_table_9
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_F(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_E(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_D(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_C(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_B(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_A(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_9(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_8(n) (((n)&0x3)<<14)     

//tx_padrv_gain_setting_0
#define RF_INTERFACE_BT_PADRV_GAIN_1(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_0(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_1
#define RF_INTERFACE_BT_PADRV_GAIN_3(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_2(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_2
#define RF_INTERFACE_BT_PADRV_GAIN_5(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_4(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_3
#define RF_INTERFACE_BT_PADRV_GAIN_7(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_6(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_4
#define RF_INTERFACE_BT_PADRV_GAIN_9(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_8(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_5
#define RF_INTERFACE_BT_PADRV_GAIN_B(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_A(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_6
#define RF_INTERFACE_BT_PADRV_GAIN_D(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_C(n) (((n)&0xFF)<<8)     

//tx_padrv_gain_setting_7
#define RF_INTERFACE_BT_PADRV_GAIN_F(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_E(n) (((n)&0xFF)<<8)     

//fm_lna_gain_setting0
#define RF_INTERFACE_FM_LNA_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_7(n) (((n)&0x3)<<14)     

//fm_lna_gain_setting1
#define RF_INTERFACE_FM_LNA_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_F(n) (((n)&0x3)<<14)     

//fm_mixer_gain_setting0
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_0(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_1(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_2(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_3(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_4(n) (((n)&0x7)<<12)     

//fm_mixer_gain_setting1
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_5(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_6(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_7(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_8(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_9(n) (((n)&0x7)<<12)     

//fm_mixer_gain_setting2
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_A(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_B(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_C(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_D(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_E(n) (((n)&0x7)<<12)     

//fm_mixer_gain_setting3
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_F(n) (((n)&0x7)<<0)      

//fm_filter_gain_setting0
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_7(n) (((n)&0x3)<<14)     

//fm_filter_gain_setting1
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_F(n) (((n)&0x3)<<14)     

//fm_adc_gain_setting0
#define RF_INTERFACE_FM_ADC_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_7(n) (((n)&0x3)<<14)     

//fm_adc_gain_setting1
#define RF_INTERFACE_FM_ADC_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_F(n) (((n)&0x3)<<14)     


#endif


