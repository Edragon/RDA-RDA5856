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
// File name      : bt_modem_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:35                                                  
// Last modified  : 2016-12-21 13:55:35                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:35 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __BT_MODEM_ASM_H__
#define __BT_MODEM_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_bypass_ctrl                REG_BT_MODEM_BASE+0x000
#define REG_sw_swap_dccal              REG_BT_MODEM_BASE+0x004
#define REG_dem750_afc_freq            REG_BT_MODEM_BASE+0x008
#define REG_dpsk_gfsk_tx_ctrl          REG_BT_MODEM_BASE+0x00C
#define REG_tx_gain_ctrl               REG_BT_MODEM_BASE+0x010
#define REG_rssi_out1                  REG_BT_MODEM_BASE+0x014
#define REG_sw_ctrl                    REG_BT_MODEM_BASE+0x018
#define REG_adcclk_sw_ctrl             REG_BT_MODEM_BASE+0x01C
#define REG_dpsk_demod_sw              REG_BT_MODEM_BASE+0x020
#define REG_min_phase_err              REG_BT_MODEM_BASE+0x024
#define REG_afc_ctrl                   REG_BT_MODEM_BASE+0x028
#define REG_dpsk_gfsk_smp_th           REG_BT_MODEM_BASE+0x02C
#define REG_dpsk_gfsk_smp_th_1         REG_BT_MODEM_BASE+0x030
#define REG_gfsk_smp_ref_ctrl          REG_BT_MODEM_BASE+0x034
#define REG_gfsk_smp_ref_ctrl_1        REG_BT_MODEM_BASE+0x038
#define REG_rateconv_ctrl1             REG_BT_MODEM_BASE+0x03C
#define REG_rateconv_ctrl2             REG_BT_MODEM_BASE+0x040
#define REG_demod_smp_ctrl             REG_BT_MODEM_BASE+0x044
#define REG_agc_ctrl                   REG_BT_MODEM_BASE+0x048
#define REG_agc_th_ctrl1               REG_BT_MODEM_BASE+0x04C
#define REG_agc_th_ctrl2               REG_BT_MODEM_BASE+0x050
#define REG_agc_ctrl1                  REG_BT_MODEM_BASE+0x054
#define REG_agc_ctrl2                  REG_BT_MODEM_BASE+0x058
#define REG_agc_dgc_ctrl               REG_BT_MODEM_BASE+0x05C
#define REG_agc_dccal_ctrl             REG_BT_MODEM_BASE+0x060
#define REG_rx_dccal_ctrl              REG_BT_MODEM_BASE+0x064
#define REG_rx_dcofst_inuse            REG_BT_MODEM_BASE+0x068
#define REG_tx_dc_calib                REG_BT_MODEM_BASE+0x06C
#define REG_tx_fix_i_dcofst            REG_BT_MODEM_BASE+0x070
#define REG_tx_fix_q_dcofst            REG_BT_MODEM_BASE+0x074
#define REG_tx_i_dcofst_inuse          REG_BT_MODEM_BASE+0x078
#define REG_tx_q_dcofst_inuse          REG_BT_MODEM_BASE+0x07C
#define REG_rssi_gain_ctrl1            REG_BT_MODEM_BASE+0x080
#define REG_rssi_gain_ctrl2            REG_BT_MODEM_BASE+0x084
#define REG_rssi_gain_ctrl3            REG_BT_MODEM_BASE+0x088
#define REG_rssi_gain_ctrl4            REG_BT_MODEM_BASE+0x08C
#define REG_rssi_gain_ctrl5            REG_BT_MODEM_BASE+0x090
#define REG_rssi_gain_ctrl6            REG_BT_MODEM_BASE+0x094
#define REG_rssi_gain_ctrl7            REG_BT_MODEM_BASE+0x098
#define REG_rssi_gain_ctrl8            REG_BT_MODEM_BASE+0x09C
#define REG_modem_tpd_sel              REG_BT_MODEM_BASE+0x0A0
#define REG_demod_smp_th_ctrl          REG_BT_MODEM_BASE+0x0A4
#define REG_newpacket_byte4            REG_BT_MODEM_BASE+0x0A8
#define REG_newpacket_byte3            REG_BT_MODEM_BASE+0x0AC
#define REG_newpacket_byte2            REG_BT_MODEM_BASE+0x0B0
#define REG_newpacket_byte1            REG_BT_MODEM_BASE+0x0B4
#define REG_gfsk_mod_ctrl              REG_BT_MODEM_BASE+0x0B8
#define REG_gfsk_mod_ref_ctrl          REG_BT_MODEM_BASE+0x0BC
#define REG_sym_dly_ctrl               REG_BT_MODEM_BASE+0x0C0
#define REG_rssi_out2                  REG_BT_MODEM_BASE+0x0C4
#define REG_trx_clk_ctrl               REG_BT_MODEM_BASE+0x0C8
#define REG_iq_swap_ctrl               REG_BT_MODEM_BASE+0x0CC
#define REG_gfsk_sync_ctrl             REG_BT_MODEM_BASE+0x0D0
#define REG_gfsk_demod_ctrl            REG_BT_MODEM_BASE+0x0D4
#define REG_gfsk_mod_idx               REG_BT_MODEM_BASE+0x0D8
#define REG_dpsk_gfsk_misc_ctrl        REG_BT_MODEM_BASE+0x0DC
#define REG_modem_dbm_sel              REG_BT_MODEM_BASE+0x0E0
#define REG_gfsk_mod_idx_le            REG_BT_MODEM_BASE+0x0E4
#define REG_newpacket_byte4_inuse      REG_BT_MODEM_BASE+0x0E8
#define REG_newpacket_byte3_inuse      REG_BT_MODEM_BASE+0x0EC
#define REG_newpacket_byte2_inuse      REG_BT_MODEM_BASE+0x0F0
#define REG_newpacket_byte1_inuse      REG_BT_MODEM_BASE+0x0F4
#define REG_le_mode_ctrl1              REG_BT_MODEM_BASE+0x0F8
#define REG_le_mode_ctrl2              REG_BT_MODEM_BASE+0x0FC
#define REG_le_mode_ctrl3              REG_BT_MODEM_BASE+0x100
#define REG_le_mode_ctrl4              REG_BT_MODEM_BASE+0x104
#define REG_le_mode_ctrl5              REG_BT_MODEM_BASE+0x108
#define REG_rf_ctrl                    REG_BT_MODEM_BASE+0x10C
#define REG_tx_q_im                    REG_BT_MODEM_BASE+0x110
#define REG_tx_i_im                    REG_BT_MODEM_BASE+0x114
#define REG_tx_am_p0                   REG_BT_MODEM_BASE+0x118
#define REG_tx_am_p1                   REG_BT_MODEM_BASE+0x11C
#define REG_tx_am_p2                   REG_BT_MODEM_BASE+0x120
#define REG_tx_am_p3                   REG_BT_MODEM_BASE+0x124
#define REG_tx_am_p4                   REG_BT_MODEM_BASE+0x128
#define REG_tx_am_p5                   REG_BT_MODEM_BASE+0x12C
#define REG_tx_am_p6                   REG_BT_MODEM_BASE+0x130
#define REG_tx_am_p7                   REG_BT_MODEM_BASE+0x134
#define REG_tx_am_p8                   REG_BT_MODEM_BASE+0x138
#define REG_tx_am_p9                   REG_BT_MODEM_BASE+0x13C
#define REG_tx_am_p10                  REG_BT_MODEM_BASE+0x140
#define REG_tx_am_p11                  REG_BT_MODEM_BASE+0x144
#define REG_tx_am_p12                  REG_BT_MODEM_BASE+0x148
#define REG_tx_am_p13                  REG_BT_MODEM_BASE+0x14C
#define REG_tx_am_p14                  REG_BT_MODEM_BASE+0x150
#define REG_tx_am_p15                  REG_BT_MODEM_BASE+0x154
#define REG_tx_am_p16                  REG_BT_MODEM_BASE+0x158
#define REG_tx_pm_p0                   REG_BT_MODEM_BASE+0x15C
#define REG_tx_pm_p1                   REG_BT_MODEM_BASE+0x160
#define REG_tx_pm_p2                   REG_BT_MODEM_BASE+0x164
#define REG_tx_pm_p3                   REG_BT_MODEM_BASE+0x168
#define REG_tx_pm_p4                   REG_BT_MODEM_BASE+0x16C
#define REG_tx_pm_p5                   REG_BT_MODEM_BASE+0x170
#define REG_tx_pm_p6                   REG_BT_MODEM_BASE+0x174
#define REG_tx_pm_p7                   REG_BT_MODEM_BASE+0x178
#define REG_tx_pm_p8                   REG_BT_MODEM_BASE+0x17C
#define REG_tx_pm_p9                   REG_BT_MODEM_BASE+0x180
#define REG_tx_pm_p10                  REG_BT_MODEM_BASE+0x184
#define REG_tx_pm_p11                  REG_BT_MODEM_BASE+0x188
#define REG_tx_pm_p12                  REG_BT_MODEM_BASE+0x18C
#define REG_tx_pm_p13                  REG_BT_MODEM_BASE+0x190
#define REG_tx_pm_p14                  REG_BT_MODEM_BASE+0x194
#define REG_tx_pm_p15                  REG_BT_MODEM_BASE+0x198
#define REG_tx_pm_p16                  REG_BT_MODEM_BASE+0x19C
#define REG_notch_coef                 REG_BT_MODEM_BASE+0x1A0
#define REG_adapt_edr3_demod           REG_BT_MODEM_BASE+0x1A4
#define REG_adapt_edr3_thresh          REG_BT_MODEM_BASE+0x1A8

//bypass_ctrl
#define BT_MODEM_SEL_RSSI_SRC          (1<<0)              
#define BT_MODEM_BYPASS_SMPPT_DPSK     (1<<1)              
#define BT_MODEM_BYPASS_PATCH_DPSK     (1<<2)              
#define BT_MODEM_BYPASS_DERR2_DPSK     (1<<3)              
#define BT_MODEM_BYPASS_DERR1_DPSK     (1<<4)              
#define BT_MODEM_BYPASS_SMPPT_GFSK     (1<<5)              
#define BT_MODEM_BYPASS_PATCH_GFSK     (1<<6)              
#define BT_MODEM_BYPASS_DERR2_GFSK     (1<<7)              
#define BT_MODEM_BYPASS_DERR1_GFSK     (1<<8)              
#define BT_MODEM_BYPASS_SRRC           (1<<12)             
#define BT_MODEM_BYPASS_MIXER          (1<<13)             
#define BT_MODEM_BYPASS_DCCANCEL2      (1<<14)             
#define BT_MODEM_BYPASS_DCCANCEL       (1<<15)             

//sw_swap_dccal
#define BT_MODEM_SEL_SUMERR_RANGE(n)   (((n)&0x7)<<0)      
#define BT_MODEM_IQ_SEL_POL            (1<<5)              
#define BT_MODEM_SWCH_SIGN_RX          (1<<6)              
#define BT_MODEM_SWCH_SIGN_Q_TX        (1<<7)              
#define BT_MODEM_SWCH_SIGN_I_TX        (1<<8)              
#define BT_MODEM_LPF_DWIDTH_SEL(n)     (((n)&0x3)<<9)      
#define BT_MODEM_RSVD0                 (1<<11)             
#define BT_MODEM_SWCH_CLK_DAC          (1<<12)             
#define BT_MODEM_SEL_SYNC              (1<<13)             
#define BT_MODEM_SWCH_CLK_ADC          (1<<14)             

//dem750_afc_freq
#define BT_MODEM_AFC_SMTIF(n)          (((n)&0xFFFF)<<0)   

//dpsk_gfsk_tx_ctrl
#define BT_MODEM_DELAY_DPSK(n)         (((n)&0xF)<<0)      
#define BT_MODEM_DELAY_GFSK(n)         (((n)&0xF)<<4)      
#define BT_MODEM_TX_GAIN_DPSK(n)       (((n)&0xFF)<<8)     

//tx_gain_ctrl
#define BT_MODEM_TX_GAIN_GFSK(n)       (((n)&0xFF)<<0)     
#define BT_MODEM_RSVD4                 (1<<8)              
#define BT_MODEM_CNT_GUARD_INI(n)      (((n)&0x3F)<<10)    

//rssi_out1
#define BT_MODEM_RSSI_OUT(n)           (((n)&0x3FF)<<0)    

//sw_ctrl
#define BT_MODEM_TX_GAIN_GFSK_EDR(n)   (((n)&0xFF)<<0)     
#define BT_MODEM_NOTCH_DISB            (1<<8)              
#define BT_MODEM_SWAP_PN_Q_DCCL_0      (1<<9)              
#define BT_MODEM_SWAP_PN_I_DCCL_0      (1<<10)             
#define BT_MODEM_SWAP_IQ_MIXER_0       (1<<11)             
#define BT_MODEM_SWAP_IQ_SMTIF_0       (1<<12)             
#define BT_MODEM_SWAP_IQ_DCCL_0        (1<<13)             
#define BT_MODEM_SWAP_IQ               (1<<14)             
#define BT_MODEM_RSSI_LOCK_BY_AGC      (1<<15)             

//adcclk_sw_ctrl
#define BT_MODEM_CT_U_SP_GFSK(n)       (((n)&0xF)<<0)      
#define BT_MODEM_CT_U_1_GFSK(n)        (((n)&0x1F)<<4)     
#define BT_MODEM_CT_U_GFSK(n)          (((n)&0xF)<<9)      
#define BT_MODEM_PCKT_SEL              (1<<13)             
#define BT_MODEM_EN_BBPKG_FLG          (1<<14)             

//dpsk_demod_sw
#define BT_MODEM_CT_U_SP_DPSK(n)       (((n)&0xF)<<0)      
#define BT_MODEM_CT_U_1_DPSK(n)        (((n)&0x1F)<<4)     
#define BT_MODEM_CT_U_DPSK(n)          (((n)&0xF)<<9)      
#define BT_MODEM_CT_U_PATCH(n)         (((n)&0x7)<<13)     

//min_phase_err
#define BT_MODEM_MIN_ERROR_TH(n)       (((n)&0xFFFF)<<0)   

//afc_ctrl
#define BT_MODEM_TH_GFSK_DEM(n)        (((n)&0x3FF)<<0)    
#define BT_MODEM_RSVD1(n)              (((n)&0x3)<<10)     
#define BT_MODEM_SEL_AFC_DPSKSEEK(n)   (((n)&0x3)<<12)     
#define BT_MODEM_SEL_AFC_GFSKDEM(n)    (((n)&0x3)<<14)     

//dpsk_gfsk_smp_th
#define BT_MODEM_TH_GFSK_DEM2(n)       (((n)&0x3FF)<<0)    
#define BT_MODEM_SP_TH_SEL_GFSK(n)     (((n)&0x3)<<10)     
#define BT_MODEM_SP_TH_SEL_DPSK(n)     (((n)&0x3)<<12)     
#define BT_MODEM_DIFF_ENABLE_GFSK      (1<<14)             
#define BT_MODEM_DIFF_ENABLE_DPSK      (1<<15)             

//dpsk_gfsk_smp_th_1
#define BT_MODEM_REF_A2_SEK(n)         (((n)&0x3FF)<<0)    

//gfsk_smp_ref_ctrl
#define BT_MODEM_REF_A3_SEK(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A1_SEK(n)         (((n)&0xFF)<<8)     

//gfsk_smp_ref_ctrl_1
#define BT_MODEM_DC_CT2(n)             (((n)&0x1FF)<<0)    
#define BT_MODEM_GUARD_TIME_CONF(n)    (((n)&0x1F)<<9)     
#define BT_MODEM_RSVD2(n)              (((n)&0x3)<<14)     

//rateconv_ctrl1
#define BT_MODEM_DAC_DATA_SEL(n)       (((n)&0x3F)<<0)     
#define BT_MODEM_DAC_TEST_EN           (1<<6)              
#define BT_MODEM_DC_CT(n)              (((n)&0x1FF)<<7)    

//rateconv_ctrl2
#define BT_MODEM_RSSI_TM_TH(n)         (((n)&0x3F)<<0)     
#define BT_MODEM_DC_HOLD_EDR_EN        (1<<6)              

//demod_smp_ctrl
#define BT_MODEM_CNT_SAMPLE_INI(n)     (((n)&0xF)<<0)      
#define BT_MODEM_REF_READY_TH(n)       (((n)&0x3F)<<4)     
#define BT_MODEM_INI_RST_TH(n)         (((n)&0x3F)<<10)    

//agc_ctrl
#define BT_MODEM_AGC_SINC_OVER_TH(n)   (((n)&0x3)<<0)      
#define BT_MODEM_AGC_SINC_OVER_EN      (1<<2)              
#define BT_MODEM_AGC_MOD_FIX7          (1<<3)              
#define BT_MODEM_AGC_EN_FIX7           (1<<4)              
#define BT_MODEM_DPSK_SEEK_ST_CNT(n)   (((n)&0x1F)<<5)     
#define BT_MODEM_MIX_GUARD_TH(n)       (((n)&0x3F)<<10)    

//agc_th_ctrl1
#define BT_MODEM_AGC_TH_MIN(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_AGC_TH_MAX(n)         (((n)&0xFF)<<8)     

//agc_th_ctrl2
#define BT_MODEM_AGC_TH_MIN_LG(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_AGC_TH_MAX_LG(n)      (((n)&0xFF)<<8)     

//agc_ctrl1
#define BT_MODEM_AGC_MOD_DEM(n)        (((n)&0x3)<<0)      
#define BT_MODEM_AGC_INDEX_INI_DSP(n)  (((n)&0xF)<<2)      
#define BT_MODEM_AGC_EN_DLY(n)         (((n)&0x7)<<6)      
#define BT_MODEM_AGC_STEP_OVER(n)      (((n)&0x3)<<9)      
#define BT_MODEM_AGC_STEP_MODE(n)      (((n)&0x3)<<11)     
#define BT_MODEM_AGC_EN_LG_STP         (1<<13)             
#define BT_MODEM_AGC_CT_TH_MIN         (1<<14)             

//agc_ctrl2
#define BT_MODEM_AGC_TM_INTV_LG(n)     (((n)&0x7F)<<0)     
#define BT_MODEM_AGC_TM_INTV(n)        (((n)&0x7F)<<7)     
#define BT_MODEM_AGC_TH_MIN_GN(n)      (((n)&0x3)<<14)     

//agc_dgc_ctrl
#define BT_MODEM_AGC_MODE_DSP(n)       (((n)&0x7)<<0)      
#define BT_MODEM_NEWPACKET_ZERO_WD_CNT(n) (((n)&0x3)<<6)      
#define BT_MODEM_PASS_NEWPACKET_SEL    (1<<8)              
#define BT_MODEM_DGC_INDEX_MAX(n)      (((n)&0x7)<<9)      
#define BT_MODEM_DGC_INDEX_DSP(n)      (((n)&0x7)<<12)     
#define BT_MODEM_SEL_REG_AGC           (1<<15)             

//agc_dccal_ctrl
#define BT_MODEM_RX_FIX_DCOFST         (1<<0)              
#define BT_MODEM_DC_CAL_RX_DLY(n)      (((n)&0x3)<<1)      
#define BT_MODEM_RX_DC_CAL_DONE        (1<<3)              
#define BT_MODEM_AGC_TM_HOLD(n)        (((n)&0x1F)<<6)     
#define BT_MODEM_AGC_TM_WAIT(n)        (((n)&0x1F)<<11)    

//rx_dccal_ctrl
#define BT_MODEM_RX_DC_CALI_Q_FIX(n)   (((n)&0xFF)<<0)     
#define BT_MODEM_RX_DC_CALI_I_FIX(n)   (((n)&0xFF)<<8)     

//rx_dcofst_inuse
#define BT_MODEM_RX_DCOFFSET_Q(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_RX_DCOFFSET_I(n)      (((n)&0xFF)<<8)     

//tx_dc_calib
#define BT_MODEM_BYPASS_TX_CAL         (1<<0)              
#define BT_MODEM_TX_APC(n)             (((n)&0x7)<<1)      
#define BT_MODEM_TX_CAL_SHIFT(n)       (((n)&0x3)<<4)      
#define BT_MODEM_TX_CAL_SEL(n)         (((n)&0x3)<<6)      
#define BT_MODEM_TX_CAL_POL            (1<<8)              
#define BT_MODEM_TX_CAL_CMP_POL        (1<<9)              
#define BT_MODEM_TX_CAL_CNT(n)         (((n)&0x3)<<10)     
#define BT_MODEM_TX_FIX_DCOFST         (1<<12)             
#define BT_MODEM_TX_DC_CAL_DONE        (1<<15)             

//tx_fix_i_dcofst
#define BT_MODEM_TX_DC_IDATA_FIX(n)    (((n)&0xFFF)<<0)    

//tx_fix_q_dcofst
#define BT_MODEM_TX_DC_QDATA_FIX(n)    (((n)&0xFFF)<<0)    

//tx_i_dcofst_inuse
#define BT_MODEM_DC_CAL_TX_IDATA(n)    (((n)&0xFFF)<<0)    

//tx_q_dcofst_inuse
#define BT_MODEM_DC_CAL_TX_QDATA(n)    (((n)&0xFFF)<<0)    

//rssi_gain_ctrl1
#define BT_MODEM_RSSI_ANA_GAIN0001(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0000(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl2
#define BT_MODEM_RSSI_ANA_GAIN0011(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0010(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl3
#define BT_MODEM_RSSI_ANA_GAIN0101(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0100(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl4
#define BT_MODEM_RSSI_ANA_GAIN0111(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0110(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl5
#define BT_MODEM_RSSI_ANA_GAIN1001(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1000(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl6
#define BT_MODEM_RSSI_ANA_GAIN1011(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1010(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl7
#define BT_MODEM_RSSI_ANA_GAIN1101(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1100(n)  (((n)&0x7F)<<8)     

//rssi_gain_ctrl8
#define BT_MODEM_RSSI_ANA_GAIN1111(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1110(n)  (((n)&0x7F)<<8)     

//modem_tpd_sel
#define BT_MODEM_TPD_TRIG_SEL(n)       (((n)&0xF)<<0)      
#define BT_MODEM_TPD_DATA_SEL(n)       (((n)&0xF)<<4)      
#define BT_MODEM_TPD_CLK_SEL           (1<<8)              
#define BT_MODEM_DAC_CLK_FORCE_EN      (1<<9)              

//demod_smp_th_ctrl
#define BT_MODEM_DEM_SP_TH1(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_DEM_SP_TH2(n)         (((n)&0xFF)<<8)     

//newpacket_byte4
#define BT_MODEM_NEWPACKET_4(n)        (((n)&0xFFFF)<<0)   

//newpacket_byte3
#define BT_MODEM_NEWPACKET_3(n)        (((n)&0xFFFF)<<0)   

//newpacket_byte2
#define BT_MODEM_NEWPACKET_2(n)        (((n)&0xFFFF)<<0)   

//newpacket_byte1
#define BT_MODEM_NEWPACKET_1(n)        (((n)&0xFFFF)<<0)   

//gfsk_mod_ctrl
#define BT_MODEM_REF_A2_DEM(n)         (((n)&0x3FF)<<0)    
#define BT_MODEM_RSSI_CT_U(n)          (((n)&0x7)<<12)     
#define BT_MODEM_BT_MOD                (1<<15)             

//gfsk_mod_ref_ctrl
#define BT_MODEM_REF_A3_DEM(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A1_DEM(n)         (((n)&0xFF)<<8)     

//sym_dly_ctrl
#define BT_MODEM_DELAY_GFSK_1M(n)      (((n)&0x7)<<0)      
#define BT_MODEM_DELAY_DPSK_1M(n)      (((n)&0x7)<<3)      
#define BT_MODEM_DELAY_GFSK_OFF_1M(n)  (((n)&0x7)<<6)      
#define BT_MODEM_DELAY_GFSK_OFF(n)     (((n)&0xF)<<9)      
#define BT_MODEM_RAMP_SPEED_GFSK       (1<<13)             

//rssi_out2
#define BT_MODEM_RSSI_NOISE_OUT(n)     (((n)&0x3FF)<<0)    

//trx_clk_ctrl
#define BT_MODEM_APC_SWITCH_MODE(n)    (((n)&0xF)<<6)      
#define BT_MODEM_TX_RX_REG             (1<<12)             
#define BT_MODEM_TX_RX_DIR             (1<<13)             
#define BT_MODEM_SWCH_CLK_52M_RX       (1<<14)             

//iq_swap_ctrl
#define BT_MODEM_NOTCH_EN_0            (1<<0)              
#define BT_MODEM_RATE_CONV_EN_0        (1<<1)              
#define BT_MODEM_LPF_EN_0              (1<<2)              
#define BT_MODEM_NOTCH_EN_1            (1<<4)              
#define BT_MODEM_RATE_CONV_EN_1        (1<<5)              
#define BT_MODEM_LPF_EN_1              (1<<6)              
#define BT_MODEM_IQ_SWAP_SRRC          (1<<12)             
#define BT_MODEM_IQ_SWAP_LPF           (1<<13)             
#define BT_MODEM_IQ_SWAP_GAIN2         (1<<14)             
#define BT_MODEM_SWCH_CLK_52M_TX       (1<<15)             

//gfsk_sync_ctrl
#define BT_MODEM_DYNAMIC_SYNC_TH(n)    (((n)&0x3FF)<<0)    
#define BT_MODEM_DYNAMIC_SYNC_EN       (1<<10)             

//gfsk_demod_ctrl
#define BT_MODEM_MIN_ERROR_TH2(n)      (((n)&0xFFFF)<<0)   

//gfsk_mod_idx
#define BT_MODEM_H_GAIN(n)             (((n)&0xFFFF)<<0)   

//dpsk_gfsk_misc_ctrl
#define BT_MODEM_DLY_CT_AMP(n)         (((n)&0x7)<<3)      
#define BT_MODEM_DLY_CT_DPSK(n)        (((n)&0x7)<<6)      
#define BT_MODEM_DLY_CT_GFSK(n)        (((n)&0x7)<<9)      
#define BT_MODEM_IQ_SWAP_TX            (1<<15)             

//modem_dbm_sel
#define BT_MODEM_DBM_DATA_SEL(n)       (((n)&0x1F)<<0)     

//gfsk_mod_idx_le
#define BT_MODEM_H_GAIN_LE(n)          (((n)&0xFFFF)<<0)   

//newpacket_byte4_inuse
#define BT_MODEM_NEWPACKET_DSP4(n)     (((n)&0xFFFF)<<0)   

//newpacket_byte3_inuse
#define BT_MODEM_NEWPACKET_DSP3(n)     (((n)&0xFFFF)<<0)   

//newpacket_byte2_inuse
#define BT_MODEM_NEWPACKET_DSP2(n)     (((n)&0xFFFF)<<0)   

//newpacket_byte1_inuse
#define BT_MODEM_NEWPACKET_DSP1(n)     (((n)&0xFFFF)<<0)   

//le_mode_ctrl1
#define BT_MODEM_REF_A1_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A1_SEK_LE(n)      (((n)&0xFF)<<8)     

//le_mode_ctrl2
#define BT_MODEM_REF_A2_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A2_SEK_LE(n)      (((n)&0xFF)<<8)     

//le_mode_ctrl3
#define BT_MODEM_REF_A3_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A3_SEK_LE(n)      (((n)&0xFF)<<8)     

//le_mode_ctrl4
#define BT_MODEM_MIN_ERROR_TH_LE(n)    (((n)&0xFFFF)<<0)   

//le_mode_ctrl5
#define BT_MODEM_REF_READY_TH_LE(n)    (((n)&0x3F)<<0)     
#define BT_MODEM_MIX_GUARD_TH_LE(n)    (((n)&0x3F)<<6)     
#define BT_MODEM_REF_POINT_SEL_LE(n)   (((n)&0x3)<<12)     
#define BT_MODEM_RSVD5(n)              (((n)&0x3)<<14)     

//rf_ctrl
#define BT_MODEM_SYNC_SHIFT_MODE       (1<<0)              

//tx_q_im
#define BT_MODEM_TX_IQIM_PHASE_ERR(n)  (((n)&0x7FFF)<<0)   

//tx_i_im
#define BT_MODEM_TX_IQIM_AMP_ERR(n)    (((n)&0x7FFF)<<0)   

//tx_am_p0
#define BT_MODEM_AM_P0(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_COMP_BYPASS        (1<<10)             
#define BT_MODEM_PM_COMP_BYPASS        (1<<11)             
#define BT_MODEM_PM_SHIFT(n)           (((n)&0x7)<<12)     

//tx_am_p1
#define BT_MODEM_AM_P1(n)              (((n)&0x3FF)<<0)    

//tx_am_p2
#define BT_MODEM_AM_P2(n)              (((n)&0x3FF)<<0)    

//tx_am_p3
#define BT_MODEM_AM_P3(n)              (((n)&0x3FF)<<0)    

//tx_am_p4
#define BT_MODEM_AM_P4(n)              (((n)&0x3FF)<<0)    

//tx_am_p5
#define BT_MODEM_AM_P5(n)              (((n)&0x3FF)<<0)    

//tx_am_p6
#define BT_MODEM_AM_P6(n)              (((n)&0x3FF)<<0)    

//tx_am_p7
#define BT_MODEM_AM_P7(n)              (((n)&0x3FF)<<0)    

//tx_am_p8
#define BT_MODEM_AM_P8(n)              (((n)&0x3FF)<<0)    

//tx_am_p9
#define BT_MODEM_AM_P9(n)              (((n)&0x3FF)<<0)    

//tx_am_p10
#define BT_MODEM_AM_P10(n)             (((n)&0x3FF)<<0)    

//tx_am_p11
#define BT_MODEM_AM_P11(n)             (((n)&0x3FF)<<0)    

//tx_am_p12
#define BT_MODEM_AM_P12(n)             (((n)&0x3FF)<<0)    

//tx_am_p13
#define BT_MODEM_AM_P13(n)             (((n)&0x3FF)<<0)    

//tx_am_p14
#define BT_MODEM_AM_P14(n)             (((n)&0x3FF)<<0)    

//tx_am_p15
#define BT_MODEM_AM_P15(n)             (((n)&0x3FF)<<0)    

//tx_am_p16
#define BT_MODEM_AM_P16(n)             (((n)&0x3FF)<<0)    

//tx_pm_p0
#define BT_MODEM_PM_P0(n)              (((n)&0x3FF)<<0)    

//tx_pm_p1
#define BT_MODEM_PM_P1(n)              (((n)&0x3FF)<<0)    

//tx_pm_p2
#define BT_MODEM_PM_P2(n)              (((n)&0x3FF)<<0)    

//tx_pm_p3
#define BT_MODEM_PM_P3(n)              (((n)&0x3FF)<<0)    

//tx_pm_p4
#define BT_MODEM_PM_P4(n)              (((n)&0x3FF)<<0)    

//tx_pm_p5
#define BT_MODEM_PM_P5(n)              (((n)&0x3FF)<<0)    

//tx_pm_p6
#define BT_MODEM_PM_P6(n)              (((n)&0x3FF)<<0)    

//tx_pm_p7
#define BT_MODEM_PM_P7(n)              (((n)&0x3FF)<<0)    

//tx_pm_p8
#define BT_MODEM_PM_P8(n)              (((n)&0x3FF)<<0)    

//tx_pm_p9
#define BT_MODEM_PM_P9(n)              (((n)&0x3FF)<<0)    

//tx_pm_p10
#define BT_MODEM_PM_P10(n)             (((n)&0x3FF)<<0)    

//tx_pm_p11
#define BT_MODEM_PM_P11(n)             (((n)&0x3FF)<<0)    

//tx_pm_p12
#define BT_MODEM_PM_P12(n)             (((n)&0x3FF)<<0)    

//tx_pm_p13
#define BT_MODEM_PM_P13(n)             (((n)&0x3FF)<<0)    

//tx_pm_p14
#define BT_MODEM_PM_P14(n)             (((n)&0x3FF)<<0)    

//tx_pm_p15
#define BT_MODEM_PM_P15(n)             (((n)&0x3FF)<<0)    

//tx_pm_p16
#define BT_MODEM_PM_P16(n)             (((n)&0x3FF)<<0)    

//notch_coef
#define BT_MODEM_NOTCH_B(n)            (((n)&0x3FFF)<<0)   

//adapt_edr3_demod
#define BT_MODEM_CT_U_1_DPSK1(n)       (((n)&0x1F)<<0)     
#define BT_MODEM_CT_U_DPSK1(n)         (((n)&0xF)<<5)      
#define BT_MODEM_EDR3_ADAPT_EN         (1<<9)              
#define BT_MODEM_NOTCH_A(n)            (((n)&0xF)<<12)     

//adapt_edr3_thresh
#define BT_MODEM_EDR3_ADAPT_TH(n)      (((n)&0xFFF)<<0)    


#endif


