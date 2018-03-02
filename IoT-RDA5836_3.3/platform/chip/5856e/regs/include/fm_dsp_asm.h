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
// File name      : fm_dsp_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:43                                                  
// Last modified  : 2016-12-21 13:55:43                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:43 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __FM_DSP_ASM_H__
#define __FM_DSP_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_tx_fifo_rdata              REG_FM_DSP_BASE+0x00
#define REG_revision                   REG_FM_DSP_BASE+0x04
#define REG_ctrl                       REG_FM_DSP_BASE+0x08
#define REG_tx_ctrl                    REG_FM_DSP_BASE+0x0c
#define REG_seek_ctrl0                 REG_FM_DSP_BASE+0x10
#define REG_seek_ctrl1                 REG_FM_DSP_BASE+0x14
#define REG_seek_ctrl2                 REG_FM_DSP_BASE+0x18
#define REG_seek_ctrl3                 REG_FM_DSP_BASE+0x1c
#define REG_seek_ctrl4                 REG_FM_DSP_BASE+0x20
#define REG_afc_ctrl                   REG_FM_DSP_BASE+0x24
#define REG_agc_ctrl0                  REG_FM_DSP_BASE+0x28
#define REG_agc_ctrl1                  REG_FM_DSP_BASE+0x2c
#define REG_ana_gain_rssi_tb0          REG_FM_DSP_BASE+0x30
#define REG_ana_gain_rssi_tb1          REG_FM_DSP_BASE+0x34
#define REG_ana_gain_rssi_tb2          REG_FM_DSP_BASE+0x38
#define REG_ana_gain_rssi_tb3          REG_FM_DSP_BASE+0x3c
#define REG_noise_cancel_ctrl0         REG_FM_DSP_BASE+0x40
#define REG_noise_cancel_ctrl1         REG_FM_DSP_BASE+0x44
#define REG_noise_cancel_ctrl2         REG_FM_DSP_BASE+0x48
#define REG_datapath_ctrl0             REG_FM_DSP_BASE+0x4c
#define REG_datapath_ctrl1             REG_FM_DSP_BASE+0x50
#define REG_datapath_ctrl2             REG_FM_DSP_BASE+0x54
#define REG_datapath_ctrl3             REG_FM_DSP_BASE+0x58
#define REG_datapath_ctrl4             REG_FM_DSP_BASE+0x5c
#define REG_datapath_ctrl5             REG_FM_DSP_BASE+0x60
#define REG_log_ctrl0                  REG_FM_DSP_BASE+0x64
#define REG_status0                    REG_FM_DSP_BASE+0x68
#define REG_status1                    REG_FM_DSP_BASE+0x6c
#define REG_status2                    REG_FM_DSP_BASE+0x70
#define REG_status3                    REG_FM_DSP_BASE+0x74
#define REG_rsvd_reg                   REG_FM_DSP_BASE+0x78

//tx_fifo_rdata
#define FM_DSP_TX_DATA_Q(n)            (((n)&0xFFFF)<<0)   
#define FM_DSP_TX_DATA_I(n)            (((n)&0xFFFF)<<16)  

//revision
#define FM_DSP_REV_ID(n)               (((n)&0xF)<<0)      

//ctrl
#define FM_DSP_MUTE                    (1<<0)              
#define FM_DSP_STEREO_IN               (1<<1)              
#define FM_DSP_DEEMPH_MODE             (1<<2)              
#define FM_DSP_SOFTMUTE_EN             (1<<3)              
#define FM_DSP_SOFTBLEND_OFF           (1<<4)              
#define FM_DSP_AFC_DISABLE             (1<<5)              
#define FM_DSP_IMGREJ                  (1<<6)              
#define FM_DSP_IMGREJ_DSP              (1<<7)              
#define FM_DSP_IQ_SWAP                 (1<<8)              
#define FM_DSP_PLLDEM_SWAP             (1<<9)              
#define FM_DSP_LR_SWAP                 (1<<10)             
#define FM_DSP_FIRCUT_BYPASS           (1<<11)             
#define FM_DSP_FIRCUT_BYPASS_SK        (1<<12)             
#define FM_DSP_LPF_BYPASS              (1<<13)             
#define FM_DSP_DEEMPH_BYPASS           (1<<14)             
#define FM_DSP_PILOT_COSINE            (1<<15)             
#define FM_DSP_ADC_CLK_INVERT          (1<<16)             
#define FM_DSP_NOISE_SRC_SEL(n)        (((n)&0x3)<<17)     
#define FM_DSP_NOISE_CANCEL_SRC_SEL    (1<<19)             
#define FM_DSP_SEEK_RSSI_SRC_SEL       (1<<20)             
#define FM_DSP_SEEK_OFFSET_SRC_SEL     (1<<21)             
#define FM_DSP_FMDEM_SRC_SEL           (1<<22)             
#define FM_DSP_I2S_EN                  (1<<23)             
#define FM_DSP_I2S_DELAY_1T            (1<<24)             
#define FM_DSP_TX_EN                   (1<<25)             
#define FM_DSP_FORCE_CLK_ON            (1<<26)             
#define FM_DSP_ADC_DATA_FORMAT         (1<<27)             
#define FM_DSP_DBG_OUT_EN              (1<<28)             

//tx_ctrl
#define FM_DSP_TX_SEL(n)               (((n)&0x1F)<<0)     
#define FM_DSP_TX_FIFO_CLR             (1<<5)              
#define FM_DSP_TX_FIFO_OVERFLOW        (1<<6)              
#define FM_DSP_TX_FIFO_UNDERFLOW       (1<<7)              
#define FM_DSP_TX_FIFO_USEDW(n)        (((n)&0x7)<<8)      

//seek_ctrl0
#define FM_DSP_SK_CMP_GRP_EN(n)        (((n)&0x1F)<<0)     
#define FM_DSP_SEEK_MODE(n)            (((n)&0x7)<<5)      
#define FM_DSP_SEEK_AFC_ON             (1<<8)              
#define FM_DSP_SEEK_RANGE(n)           (((n)&0x1F)<<9)     
#define FM_DSP_SNR_TH(n)               (((n)&0x3F)<<14)    
#define FM_DSP_DELTA_RSSI_TH(n)        (((n)&0x3F)<<20)    
#define FM_DSP_SNR_CNT_TH(n)           (((n)&0xF)<<26)     

//seek_ctrl1
#define FM_DSP_OFFSET_CNT_TH(n)        (((n)&0xFF)<<0)     
#define FM_DSP_NOISE_CNT_TH(n)         (((n)&0xFF)<<8)     
#define FM_DSP_RSSI_CNT_TH(n)          (((n)&0xFF)<<16)    
#define FM_DSP_PILOT_CNT_TH(n)         (((n)&0xFF)<<24)    

//seek_ctrl2
#define FM_DSP_RSSI_H_TH(n)            (((n)&0x7F)<<0)     
#define FM_DSP_RSSI_L_TH(n)            (((n)&0x7F)<<7)     
#define FM_DSP_NOISE_H_TH(n)           (((n)&0x7F)<<14)    
#define FM_DSP_NOISE_L_TH(n)           (((n)&0x7F)<<21)    

//seek_ctrl3
#define FM_DSP_PILOT_H_TH(n)           (((n)&0x7F)<<0)     
#define FM_DSP_PILOT_L_TH(n)           (((n)&0x7F)<<7)     
#define FM_DSP_OFFSET_H_TH(n)          (((n)&0x7F)<<14)    
#define FM_DSP_OFFSET_L_TH(n)          (((n)&0x7F)<<21)    

//seek_ctrl4
#define FM_DSP_SK_TIMER1(n)            (((n)&0xF)<<0)      
#define FM_DSP_SK_TIMER2(n)            (((n)&0x3F)<<4)     
#define FM_DSP_SK_TIMER3(n)            (((n)&0x3F)<<10)    
#define FM_DSP_SK_TIMER4(n)            (((n)&0x3F)<<16)    

//afc_ctrl
#define FM_DSP_AFC_INV                 (1<<0)              
#define FM_DSP_AFC_RANGE(n)            (((n)&0x3F)<<1)     
#define FM_DSP_AFC_CT(n)               (((n)&0x3F)<<7)     
#define FM_DSP_AFC_CT_SK(n)            (((n)&0x3F)<<13)    

//agc_ctrl0
#define FM_DSP_CT_ENDELAY(n)           (((n)&0x7)<<0)      
#define FM_DSP_AGC_INDEX_IN(n)         (((n)&0xF)<<3)      
#define FM_DSP_AGC_THD(n)              (((n)&0x1F)<<7)     
#define FM_DSP_AGC_UPDATE0(n)          (((n)&0x7)<<12)     
#define FM_DSP_AGC_UPDATE1(n)          (((n)&0x7)<<15)     
#define FM_DSP_AGC_LOOP_GAIN0(n)       (((n)&0x7)<<18)     
#define FM_DSP_AGC_LOOP_GAIN1(n)       (((n)&0x7)<<21)     
#define FM_DSP_AGC_TEST                (1<<24)             
#define FM_DSP_AGC_TARGET_PWR(n)       (((n)&0x3F)<<25)    

//agc_ctrl1
#define FM_DSP_LOG_OVER_SEL            (1<<0)              
#define FM_DSP_LOG_AGC_TH(n)           (((n)&0x3F)<<1)     
#define FM_DSP_OVER_STEP(n)            (((n)&0x3F)<<7)     
#define FM_DSP_AGC_CT_U(n)             (((n)&0x3)<<13)     
#define FM_DSP_DIG_GAIN_IN(n)          (((n)&0x1F)<<15)    

//ana_gain_rssi_tb0
#define FM_DSP_ANA_GAIN_RSSI_0(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_1(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_2(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_3(n)      (((n)&0xFF)<<24)    

//ana_gain_rssi_tb1
#define FM_DSP_ANA_GAIN_RSSI_4(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_5(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_6(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_7(n)      (((n)&0xFF)<<24)    

//ana_gain_rssi_tb2
#define FM_DSP_ANA_GAIN_RSSI_8(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_9(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_A(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_B(n)      (((n)&0xFF)<<24)    

//ana_gain_rssi_tb3
#define FM_DSP_ANA_GAIN_RSSI_C(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_D(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_E(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_F(n)      (((n)&0xFF)<<24)    

//noise_cancel_ctrl0
#define FM_DSP_FMDEM_SEL_GRP(n)        (((n)&0x7)<<0)      
#define FM_DSP_PLLDEM_TH_MAX(n)        (((n)&0x3F)<<3)     
#define FM_DSP_PLLDEM_TH_MIN(n)        (((n)&0x3F)<<9)     
#define FM_DSP_TH_MAX_BW(n)            (((n)&0x7F)<<15)    
#define FM_DSP_TH_MIN_BW(n)            (((n)&0x7F)<<22)    

//noise_cancel_ctrl1
#define FM_DSP_TH_MIN_HCC(n)           (((n)&0x3F)<<0)     
#define FM_DSP_SEL_FIRCUT1(n)          (((n)&0x7)<<6)      
#define FM_DSP_SEL_FIRCUT2(n)          (((n)&0x7)<<9)      
#define FM_DSP_SEL_FIRCUT3(n)          (((n)&0x7)<<12)     
#define FM_DSP_SEL_FIRCUT_SK(n)        (((n)&0x3F)<<15)    
#define FM_DSP_FIRCUT_SEL_FORCE_ON     (1<<21)             

//noise_cancel_ctrl2
#define FM_DSP_GAIN_38K1(n)            (((n)&0x7FFF)<<0)   
#define FM_DSP_GAIN_38K2(n)            (((n)&0x7FFF)<<15)  

//datapath_ctrl0
#define FM_DSP_DC_CT(n)                (((n)&0xF)<<0)      
#define FM_DSP_LIMIT_SEL               (1<<4)              
#define FM_DSP_OVER_TH_SEL(n)          (((n)&0x7)<<5)      
#define FM_DSP_SINC_LIMIT_GAIN(n)      (((n)&0x3)<<8)      
#define FM_DSP_DIG_DELAYCT(n)          (((n)&0x1F)<<10)    
#define FM_DSP_GAINCT(n)               (((n)&0x1FF)<<15)   
#define FM_DSP_HBF2_BYPASS             (1<<24)             
#define FM_DSP_HBF1_BYPASS             (1<<25)             

//datapath_ctrl1
#define FM_DSP_SOFTMUTE_TH3(n)         (((n)&0x3F)<<0)     
#define FM_DSP_SOFTMUTE_TH2(n)         (((n)&0x3F)<<6)     
#define FM_DSP_SOFTMUTE_TH1(n)         (((n)&0x3F)<<12)    
#define FM_DSP_NCO_CT_U(n)             (((n)&0x7)<<18)     
#define FM_DSP_NCO_CT_BW(n)            (((n)&0xF)<<21)     
#define FM_DSP_NCO_CT_U_DT_SK(n)       (((n)&0x7)<<25)     
#define FM_DSP_NCO_CT_U_DT(n)          (((n)&0x7)<<28)     

//datapath_ctrl2
#define FM_DSP_DEEMPH_HCC(n)           (((n)&0x3)<<0)      
#define FM_DSP_LPF_BW_REG(n)           (((n)&0x7)<<2)      
#define FM_DSP_LPF_BW_DR               (1<<5)              
#define FM_DSP_OFFSET_FLT_CT_U(n)      (((n)&0x3)<<6)      
#define FM_DSP_TH_SOFTBLEND2(n)        (((n)&0x3F)<<8)     
#define FM_DSP_TH_SOFTBLEND1(n)        (((n)&0x3F)<<14)    
#define FM_DSP_SM_ATTENU(n)            (((n)&0xF)<<20)     
#define FM_DSP_SM_RATE(n)              (((n)&0x3)<<24)     
#define FM_DSP_SOFTMUTE_TH4(n)         (((n)&0x3F)<<26)    

//datapath_ctrl3
#define FM_DSP_INTERVAL_REG(n)         (((n)&0xFFFF)<<0)   
#define FM_DSP_STEP19K_REG(n)          (((n)&0x7FFF)<<16)  

//datapath_ctrl4
#define FM_DSP_NOTCH_FLT_Z0_I(n)       (((n)&0xFFFFF)<<0)  
#define FM_DSP_NOTCH_FLT_KA(n)         (((n)&0x3)<<20)     
#define FM_DSP_NOTCH_FLT_BYPASS        (1<<22)             

//datapath_ctrl5
#define FM_DSP_NOTCH_FLT_Z0_Q(n)       (((n)&0xFFFFF)<<0)  

//log_ctrl0
#define FM_DSP_CT_U_PILOT(n)           (((n)&0x3F)<<0)     
#define FM_DSP_CT_U_RSSI(n)            (((n)&0x3F)<<6)     
#define FM_DSP_CT_U_SIGNAL(n)          (((n)&0x3F)<<12)    
#define FM_DSP_CT_U_NOISE(n)           (((n)&0x3F)<<18)    

//status0
#define FM_DSP_OFFSET2(n)              (((n)&0xFF)<<0)     
#define FM_DSP_SIGNAL_DB2(n)           (((n)&0x7F)<<8)     
#define FM_DSP_SNR_OUT(n)              (((n)&0x3F)<<15)    
#define FM_DSP_RSSI_DB2(n)             (((n)&0x7F)<<21)    
#define FM_DSP_SEEK_DONE               (1<<28)             
#define FM_DSP_SEEK_READY              (1<<29)             

//status1
#define FM_DSP_SNR_CNT(n)              (((n)&0xF)<<0)      
#define FM_DSP_SK_CMP_GRP_FLAG(n)      (((n)&0x1F)<<4)     
#define FM_DSP_PILOT_DB2(n)            (((n)&0x7F)<<9)     
#define FM_DSP_SNR19K_OUT(n)           (((n)&0x3F)<<16)    
#define FM_DSP_NOISE_DB2(n)            (((n)&0x7F)<<22)    

//status2
#define FM_DSP_OFFSET_CNT(n)           (((n)&0xFF)<<0)     
#define FM_DSP_PILOT_CNT(n)            (((n)&0xFF)<<8)     
#define FM_DSP_NOISE_CNT(n)            (((n)&0xFF)<<16)    
#define FM_DSP_RSSI_CNT(n)             (((n)&0xFF)<<24)    

//status3
#define FM_DSP_AGC_INDEX(n)            (((n)&0xF)<<0)      
#define FM_DSP_DIG_GAINCT(n)           (((n)&0xFF)<<4)     
#define FM_DSP_FMDEM_SEL               (1<<12)             
#define FM_DSP_FIRCUT_BW(n)            (((n)&0x7)<<13)     
#define FM_DSP_CT_HCC(n)               (((n)&0x7)<<16)     

//rsvd_reg
#define FM_DSP_RSVD_OUT(n)             (((n)&0xFFFF)<<0)   
#define FM_DSP_RSVD_IN(n)              (((n)&0xFFFF)<<16)  


#endif


