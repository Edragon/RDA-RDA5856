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
// File name      : abb                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:25                                                  
// Last modified  : 2016-12-14 15:57:25                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:25 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __ABB_H__
#define __ABB_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ID
typedef union
{
    U32 v;
    struct
    {
        U32 RevID                          :4 ; /*3 :0 , No description.               */
        U32 ChipID                         :12; /*15:4 , No description.               */
        U32 ID_reserved_0                  :16; /*31:16, No description.               */
    } b;
} T_ABB_ID;

//codec_cfg0
typedef union
{
    U32 v;
    struct
    {
        U32 ibias_bit                      :2 ; /*1 :0 , adjust bias current           */
        U32 vcombuf_sel                    :4 ; /*5 :2 , vcombuffer select             */
        U32 vcom_mode                      :3 ; /*8 :6 , vcomref mode select           */
        U32 vmic_bit                       :2 ; /*10:9 , adjust the mic bias voltage   */
        U32 sel_bg                         :1 ; /*11:11, select bandgap                */
        U32 dac_vos_bit                    :4 ; /*15:12, adjust dac vos                */
        U32 codec_cfg0_reserved_0          :16; /*31:16, No description.               */
    } b;
} T_ABB_codec_cfg0;

//codec_cfg1
typedef union
{
    U32 v;
    struct
    {
        U32 dac_clk_inv_sel                :1 ; /*0 :0 , select at which edge of clk_dac the input signal would be sampled0: sampling at rising edge1: sampling at falling edge*/
        U32 dac_vreg_bit                   :3 ; /*3 :1 , adjust dac vreg voltage       */
        U32 adc_clk_inv_sel                :1 ; /*4 :4 , select at which edge of clk_adc the input signal would be sampled0: sampling at rising edge1: sampling at falling edge*/
        U32 adc_ibit                       :2 ; /*6 :5 , adjust adc current            */
        U32 micpga_r_ibit                  :2 ; /*8 :7 , adjust micpga_r current       */
        U32 micpga_l_ibit                  :2 ; /*10:9 , adjust micpga_l current       */
        U32 micpga_r_chop_en               :1 ; /*11:11, turn on/off micpga_r chop function*/
        U32 micpga_l_chop_en               :1 ; /*12:12, turn on/off micpga_l chop function*/
        U32 codec_cfg1_reserved_0          :19; /*31:13, No description.               */
    } b;
} T_ABB_codec_cfg1;

//ADC_Gain
typedef union
{
    U32 v;
    struct
    {
        U32 adc_r_gain_bit                 :3 ; /*2 :0 , adc_r gain select000: mute001~111 -3dB ~ +15dB 3dB/step*/
        U32 adc_l_gain_bit                 :3 ; /*5 :3 , adc_l gain select000: mute001~111 -3dB ~ +15dB 3dB/step*/
        U32 micpga_r_gain_bit              :2 ; /*7 :6 , micpga_r_gain select00: 0dB01: 9dB10: 21dB11: 30dB*/
        U32 micpga_l_gain_bit              :2 ; /*9 :8 , micpga_l_gain select00: 0dB01: 9dB10: 21dB11: 30dB*/
        U32 ADC_Gain_reserved_0            :22; /*31:10, No description.               */
    } b;
} T_ABB_ADC_Gain;

//DAC_Gain
typedef union
{
    U32 v;
    struct
    {
        U32 hp_gain_bit                    :3 ; /*2 :0 , 0~18dB 3dB/step               */
        U32 mux_gain_bit                   :2 ; /*4 :3 , mux gain select00: 0dB01: 3dB10: 6dB11: 9dB*/
        U32 dac_gain_bit                   :4 ; /*8 :5 , dac gain select (fine adjust)~0.5dB/step*/
        U32 DAC_Gain_reserved_0            :23; /*31:9 , No description.               */
    } b;
} T_ABB_DAC_Gain;

//Codec_Mode0
typedef union
{
    U32 v;
    struct
    {
        U32 micinput_r_sel                 :4 ; /*3 :0 , micpga input mode select      */
        U32 micinput_l_sel                 :4 ; /*7 :4 , micpga input mode select      */
        U32 hp_inputsel                    :2 ; /*9 :8 , HP input select               */
        U32 mux_ibit                       :2 ; /*11:10, mux current                   */
        U32 hp_ibit                        :2 ; /*13:12, hp current                    */
        U32 Codec_Mode0_reserved_0         :18; /*31:14, No description.               */
    } b;
} T_ABB_Codec_Mode0;

//Codec_Mode1
typedef union
{
    U32 v;
    struct
    {
        U32 adc_smplrate_sel               :2 ; /*1 :0 , adc sample rate select:10/11: 48K/44.1K mode01:      16KSPS mode00:       8KSPS mode*/
        U32 adc_lv_mode                    :1 ; /*2 :2 , adc low supply voltage mode   */
        U32 ivref_lv_mode                  :1 ; /*3 :3 , ivref low supply voltage mode */
        U32 hp_diff_mode                   :1 ; /*4 :4 , headphone amplifier differential work mode*/
        U32 linein_mode                    :1 ; /*5 :5 , linein output mode select     */
        U32 dac_r_mode                     :1 ; /*6 :6 , dac_r output mode select      */
        U32 dac_l_mode                     :1 ; /*7 :7 , dac_l output mode select      */
        U32 micloop_r_mode                 :1 ; /*8 :8 , micpga_r output loop mode select*/
        U32 micloop_l_mode                 :1 ; /*9 :9 , micpga_l output loop mode select*/
        U32 Codec_Mode1_reserved_0         :22; /*31:10, No description.               */
    } b;
} T_ABB_Codec_Mode1;

//Reset_Ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 reset_hp                       :1 ; /*0 :0 , reset hp                      */
        U32 reset_mux_r                    :1 ; /*1 :1 , reset mux_r                   */
        U32 reset_mux_l                    :1 ; /*2 :2 , reset mux_l                   */
        U32 reset_adc                      :1 ; /*3 :3 , reset adc                     */
        U32 reset_micpga_r                 :1 ; /*4 :4 , reset micpga_r                */
        U32 reset_micpga_l                 :1 ; /*5 :5 , reset micpga_l                */
        U32 Reset_Ctrl_reserved_0          :26; /*31:6 , No description.               */
    } b;
} T_ABB_Reset_Ctrl;

//Power_Ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pu_mux_r                       :1 ; /*0 :0 , power up mux_r                */
        U32 pu_mux_l                       :1 ; /*1 :1 , power up mux_l                */
        U32 pu_dac_r                       :1 ; /*2 :2 , power up dac_r                */
        U32 pu_dac_l                       :1 ; /*3 :3 , power up dac_l                */
        U32 pu_adc_r                       :1 ; /*4 :4 , power up adc_r                */
        U32 pu_adc_l                       :1 ; /*5 :5 , power up adc_l                */
        U32 pu_micpga_r                    :1 ; /*6 :6 , power up micpga_r             */
        U32 pu_micpga_l                    :1 ; /*7 :7 , power up micpga_l             */
        U32 pu_vmic                        :1 ; /*8 :8 , power up vmic                 */
        U32 pu_vrefln                      :1 ; /*9 :9 , power up vrefln               */
        U32 pu_vcombuf_c                   :1 ; /*10:10, power up vcombuf_c            */
        U32 pu_vcombuf_b                   :1 ; /*11:11, power up vcombuf_b            */
        U32 pu_ivref                       :1 ; /*12:12, power up ivref                */
        U32 pu_all                         :1 ; /*13:13, power up codec                */
        U32 Power_Ctrl_reserved_0          :18; /*31:14, No description.               */
    } b;
} T_ABB_Power_Ctrl;

//vref_cal
typedef union
{
    U32 v;
    struct
    {
        U32 vref_cal_bit_reg               :3 ; /*2 :0 , vref calibration bits to be adjusted */
        U32 vref_cal_bit_dr                :1 ; /*3 :3 , 1'b1: use vref_cal_bit_reg control_x000D_1'b0: use hardware calibration logic control*/
        U32 vref_cal_clk_inv               :1 ; /*4 :4 , vref calibration clock inversion*/
        U32 vref_cal_out_polarity          :1 ; /*5 :5 , vref calibration output polarity selection*/
        U32 resetn_vref_cal_reg            :1 ; /*6 :6 , vref calibration resetn control register*/
        U32 resetn_vref_cal_dr             :1 ; /*7 :7 , vref calibration resetn control:0: hardware automatic control1: resetn_vref_cal_reg control*/
        U32 vref_cal_ready                 :1 ; /*8 :8 , vref calibration ready        */
        U32 vref_cal_reserved_0            :23; /*31:9 , No description.               */
    } b;
} T_ABB_vref_cal;

//vrefc_cal
typedef union
{
    U32 v;
    struct
    {
        U32 vrefc_cal_bit_reg              :3 ; /*2 :0 , vrefc calibration bits to be adjusted */
        U32 vrefc_cal_bit_dr               :1 ; /*3 :3 , 1'b1: use vrefc_cal_bit_reg control_x000D_1'b0: use hardware calibration logic control*/
        U32 vrefc_cal_clk_inv              :1 ; /*4 :4 , vrefc calibration clock inversion*/
        U32 vrefc_cal_out_polarity         :1 ; /*5 :5 , vrefc calibration output polarity selection*/
        U32 resetn_vrefc_cal_reg           :1 ; /*6 :6 , vrefc calibration resetn control register*/
        U32 resetn_vrefc_cal_dr            :1 ; /*7 :7 , vrefc calibration resetn control:0: hardware automatic control1: resetn_vrefc_cal_reg control*/
        U32 vrefc_cal_ready                :1 ; /*8 :8 , vrefc calibration ready       */
        U32 vrefc_cal_reserved_0           :23; /*31:9 , No description.               */
    } b;
} T_ABB_vrefc_cal;

//vadc_cal
typedef union
{
    U32 v;
    struct
    {
        U32 vadc_cal_bit_reg               :4 ; /*3 :0 , vadc calibration bits to be adjusted */
        U32 vadc_cal_bit_dr                :1 ; /*4 :4 , 1'b1: use vadc_cal_bit_reg control_x000D_1'b0: use hardware calibration logic control*/
        U32 vadc_cal_clk_inv               :1 ; /*5 :5 , vadc calibration clock inversion*/
        U32 vadc_cal_out_polarity          :1 ; /*6 :6 , vadc calibration output polarity selection*/
        U32 resetn_vadc_cal_reg            :1 ; /*7 :7 , vadc calibration resetn control register*/
        U32 resetn_vadc_cal_dr             :1 ; /*8 :8 , vadc calibration resetn control:0: hardware automatic control1: resetn_vadc_cal_reg control*/
        U32 vadc_cal_ready                 :1 ; /*9 :9 , vadc calibration ready        */
        U32 vadc_cal_reserved_0            :22; /*31:10, No description.               */
    } b;
} T_ABB_vadc_cal;

//vdac_cal
typedef union
{
    U32 v;
    struct
    {
        U32 vdac_cal_bit_reg               :3 ; /*2 :0 , vdac calibration bits to be adjusted */
        U32 vdac_cal_bit_dr                :1 ; /*3 :3 , 1'b1: use vdac_cal_bit_reg control_x000D_1'b0: use hardware calibration logic control*/
        U32 vdac_cal_clk_inv               :1 ; /*4 :4 , vdac calibration clock inversion*/
        U32 vdac_cal_out_polarity          :1 ; /*5 :5 , vdac calibration output polarity selection*/
        U32 resetn_vdac_cal_reg            :1 ; /*6 :6 , vdac calibration resetn control register*/
        U32 resetn_vdac_cal_dr             :1 ; /*7 :7 , vdac calibration resetn control:0: hardware automatic control1: resetn_vdac_cal_reg control*/
        U32 vdac_cal_ready                 :1 ; /*8 :8 , vdac calibration ready        */
        U32 vdac_cal_reserved_0            :23; /*31:9 , No description.               */
    } b;
} T_ABB_vdac_cal;

//aupll_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 aupll_sdm_clk_sel              :1 ; /*0 :0 , select clock to output as sdm_clk*/
        U32 aupll_refmulti2_en             :1 ; /*1 :1 , double ref clk                */
        U32 aupll_pcon_mode                :1 ; /*2 :2 , division mode control         */
        U32 aupll_cpr2_bit                 :3 ; /*5 :3 , No description.               */
        U32 aupll_cpc2_bit                 :3 ; /*8 :6 , No description.               */
        U32 aupll_vco_reg_bit              :4 ; /*12:9 , digi reg output voltage control*/
        U32 aupll_setting1_reserved_0      :19; /*31:13, No description.               */
    } b;
} T_ABB_aupll_setting1;

//aupll_setting2
typedef union
{
    U32 v;
    struct
    {
        U32 pu_aupll                       :1 ; /*0 :0 , pll enable                    */
        U32 aupll_rstb                     :1 ; /*1 :1 , pll resetn                    */
        U32 aupll_vco_high_test            :1 ; /*2 :2 , pull vco control voltage to avdd*/
        U32 aupll_vco_low_test             :1 ; /*3 :3 , pull vco control voltage to avss*/
        U32 aupll_clkout_en                :3 ; /*6 :4 , aupll output controllbit2: aupll to cpu clkout enablebit1: fm adc clock enablebit0: audio codec clock enable*/
        U32 aupll_sdm_clk_test_en          :1 ; /*7 :7 , select clock to output as clk2p_test*/
        U32 aupll_test_en                  :1 ; /*8 :8 , control test clock clk2p_test output*/
        U32 aupll_ready_timer              :2 ; /*10:9 , add delay between power up pll and pll ready00: 50us 01: 100us 10: 200us 11: 400us*/
        U32 aupll_setting2_reserved_0      :21; /*31:11, No description.               */
    } b;
} T_ABB_aupll_setting2;

//aupll_setting3
typedef union
{
    U32 v;
    struct
    {
        U32 aupll_dig_reg_res_mode_vbit    :3 ; /*2 :0 , No description.               */
        U32 aupll_cpbias_bit               :3 ; /*5 :3 , No description.               */
        U32 aupll_dig_reg_rout_bit         :4 ; /*9 :6 , No description.               */
        U32 aupll_dig_reg_range_bit        :4 ; /*13:10, No description.               */
        U32 aupll_lock                     :1 ; /*14:14, aupll lock signal             */
        U32 aupll_ready                    :1 ; /*15:15, aupll ready signal            */
        U32 aupll_setting3_reserved_0      :16; /*31:16, No description.               */
    } b;
} T_ABB_aupll_setting3;

//sdmsetting
typedef union
{
    U32 v;
    struct
    {
        U32 sdm_dither_bypass              :1 ; /*0 :0 , 1: bypass dither 0: not bypass dither*/
        U32 sdm_int_dec_sel                :3 ; /*3 :1 , 100: integer 011: 3bit decimal010: 2bit decimal001: 1bit decimal000: no decimal*/
        U32 sdm_resetn                     :1 ; /*4 :4 , sdm resetn                    */
        U32 sdm_clk_inv                    :1 ; /*5 :5 , sdm feedback polarity select1: inverter0: normal*/
        U32 refdiv2_en                     :1 ; /*6 :6 , refclk is divided by 2, which indicates that pll division is divided by 2.*/
        U32 sdmsetting_reserved_0          :25; /*31:7 , No description.               */
    } b;
} T_ABB_sdmsetting;

//sdmfreq1
typedef union
{
    U32 v;
    struct
    {
        U32 sdm_freq_high                  :16; /*15:0 , pll division ratio, integer part is bit 31 to bit 27, fraction part is bit 26 to bit 0*/
        U32 sdmfreq1_reserved_0            :16; /*31:16, No description.               */
    } b;
} T_ABB_sdmfreq1;

//sdmfreq2
typedef union
{
    U32 v;
    struct
    {
        U32 sdm_freq_low                   :16; /*15:0 , pll division ratio, integer part is bit 31 to bit 27, fraction part is bit 26 to bit 0*/
        U32 sdmfreq2_reserved_0            :16; /*31:16, No description.               */
    } b;
} T_ABB_sdmfreq2;

//clk_doubler_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 pu_clkdblr                     :1 ; /*0 :0 , power up clock doubler        */
        U32 clkdblr_ibias_ctrl             :1 ; /*1 :1 , No description.               */
        U32 clkdblr_vbias_ctrl             :2 ; /*3 :2 , No description.               */
        U32 clkdblr_r_ctrl                 :3 ; /*6 :4 , No description.               */
        U32 clk_doubler_cfg_reserved_0     :25; /*31:7 , No description.               */
    } b;
} T_ABB_clk_doubler_cfg;

//usb_phy_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pu_usb                         :1 ; /*0 :0 , Power up USB PHY              */
        U32 usb_vreg_bit                   :3 ; /*3 :1 , USB TX drive current selection*/
        U32 usb_ctrl                       :3 ; /*6 :4 , USB DP/DM pull up/down selection0: NONE No pull up and pull down1: PUDP Pull up DP2: PUDM Pull up DM3: PUDPDM Pull up DP and DM4: PDDPDM Pull down DP and DM5: RESERVED06: RESERVED17: RESERVED2*/
        U32 usb_phy_ctrl_reserved_0        :25; /*31:7 , No description.               */
    } b;
} T_ABB_usb_phy_ctrl;

//clk_chop_div
typedef union
{
    U32 v;
    struct
    {
        U32 clk_chop_en                    :1 ; /*0 :0 , chop clock enable             */
        U32 clk_chop_div_base              :10; /*10:1 , chop clock divider base       */
        U32 clk_chop_div_reserved_0        :21; /*31:11, No description.               */
    } b;
} T_ABB_clk_chop_div;

//abb_rsvd_status
typedef union
{
    U32 v;
    struct
    {
        U32 abb_status                     :8 ; /*7 :0 , abb reserved status           */
        U32 abb_cfg_bak                    :8 ; /*15:8 , abb backup reserved config    */
        U32 abb_rsvd_status_reserved_0     :16; /*31:16, No description.               */
    } b;
} T_ABB_abb_rsvd_status;

//abb_rsvd_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 abb_cfg                        :16; /*15:0 , abb reserved config           */
        U32 abb_rsvd_cfg_reserved_0        :16; /*31:16, No description.               */
    } b;
} T_ABB_abb_rsvd_cfg;

//hp_chg_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 hp_chg_res_en                  :1 ; /*0 :0 , enable the method which charge the dc blocking capacitor to Vcom through a charging resistor*/
        U32 hp_chg_res_bit                 :2 ; /*2 :1 , Change the resistance of the charging resistor*/
        U32 hp_chg_cfg_reserved_0          :29; /*31:3 , No description.               */
    } b;
} T_ABB_hp_chg_cfg;

//denoise_pwr_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 hp_dac_chg_ibit                :4 ; /*3 :0 , configure the gain of the charging dac according the capacitance of dc blocking capacitor*/
        U32 pu_hp_r_dac_chg                :1 ; /*4 :4 , power up the charging dac of the right channel*/
        U32 pu_hp_l_dac_chg                :1 ; /*5 :5 , power up the charging dac of the left channel*/
        U32 pu_adc_det_r                   :1 ; /*6 :6 , power up the detection adc of the right channel*/
        U32 pu_adc_det_l                   :1 ; /*7 :7 , power up the detection adc of the left channel*/
        U32 pu_hp_r_bias                   :1 ; /*8 :8 , power up the headphone amp.bias circuit of the right channel*/
        U32 pu_hp_l_bias                   :1 ; /*9 :9 , power up the headphone amp.bias circuit of the left channel*/
        U32 pu_hp_r_mpath                  :1 ; /*10:10, power up the headphone amp.rest circuit of the right channel*/
        U32 pu_hp_l_mpath                  :1 ; /*11:11, power up the headphone amp.rest circuit of the left channel*/
        U32 soft_start_en                  :1 ; /*12:12, enable the soft start up method of the headphone amplifier*/
        U32 denoise_pwr_cfg_reserved_0     :19; /*31:13, No description.               */
    } b;
} T_ABB_denoise_pwr_cfg;

//denoise_elimination_dac_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 denoise_dac_clk_div            :7 ; /*6 :0 , denoise dac clk divider base  */
        U32 denoise_dac_clk_inv            :2 ; /*8 :7 , denoise dac clk inverter      */
        U32 denoise_dac_clk_ld             :1 ; /*9 :9 , ld denoise dac clk div        */
        U32 denoise_elimination_dac_clk_cfg_reserved_0 :22; /*31:10, No description.               */
    } b;
} T_ABB_denoise_elimination_dac_clk_cfg;

//denoise_adc_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 denoise_adc_clk_div            :10; /*9 :0 , denoise adc clk divider base  */
        U32 denoise_adc_clk_inv            :2 ; /*11:10, denoise adc clk inverter      */
        U32 denoise_adc_clk_ld             :1 ; /*12:12, ld denoise adc clk div        */
        U32 denoise_adc_clk_cfg_reserved_0 :19; /*31:13, No description.               */
    } b;
} T_ABB_denoise_adc_clk_cfg;

//denoise_ramp_cfg_l
typedef union
{
    U32 v;
    struct
    {
        U32 start_denoise_l                :1 ; /*0 :0 , start denoise procedure       */
        U32 stable_time_l                  :5 ; /*5 :1 , set denoise module stable time before ramp down, step is 1024 dac clock cycle*/
        U32 rampup_cycle_l                 :4 ; /*9 :6 , set rampup cycle, step is 512 dac clock cycle*/
        U32 rampdown_en_l                  :1 ; /*10:10, enable denoise module ramp down function*/
        U32 adc_det_th_l                   :4 ; /*14:11, left channel adc detection threshold*/
        U32 ramp_done_l                    :1 ; /*15:15, indicate left channel denoise ramp is finish*/
        U32 denoise_ramp_cfg_l_reserved_0  :16; /*31:16, No description.               */
    } b;
} T_ABB_denoise_ramp_cfg_l;

//denoise_rampup_cfg_l
typedef union
{
    U32 v;
    struct
    {
        U32 rampup_th_value_l              :12; /*11:0 , denoise rampup threshold value*/
        U32 denoise_rampup_cfg_l_reserved_0 :20; /*31:12, No description.               */
    } b;
} T_ABB_denoise_rampup_cfg_l;

//denoise_rampdown_cfg_l
typedef union
{
    U32 v;
    struct
    {
        U32 rampdown_final_value_l         :12; /*11:0 , denoise rampdown final value  */
        U32 denoise_rampdown_cfg_l_reserved_0 :20; /*31:12, No description.               */
    } b;
} T_ABB_denoise_rampdown_cfg_l;

//denoise_ramp_cfg_r
typedef union
{
    U32 v;
    struct
    {
        U32 start_denoise_r                :1 ; /*0 :0 , start denoise procedure       */
        U32 stable_time_r                  :5 ; /*5 :1 , set denoise module stable time before ramp down, step is 1024 dac clock cycle*/
        U32 rampup_cycle_r                 :4 ; /*9 :6 , set rampup cycle, step is 512 dac clock cycle*/
        U32 rampdown_en_r                  :1 ; /*10:10, enable denoise module ramp down function*/
        U32 adc_det_th_r                   :4 ; /*14:11, right channel adc detection threshold*/
        U32 ramp_done_r                    :1 ; /*15:15, indicate right channel denoise ramp is finish*/
        U32 denoise_ramp_cfg_r_reserved_0  :16; /*31:16, No description.               */
    } b;
} T_ABB_denoise_ramp_cfg_r;

//denoise_rampup_cfg_r
typedef union
{
    U32 v;
    struct
    {
        U32 rampup_th_value_r              :12; /*11:0 , denoise rampup threshold value*/
        U32 denoise_rampup_cfg_r_reserved_0 :20; /*31:12, No description.               */
    } b;
} T_ABB_denoise_rampup_cfg_r;

//denoise_rampdown_cfg_r
typedef union
{
    U32 v;
    struct
    {
        U32 rampdown_final_value_r         :12; /*11:0 , denoise rampdown final value  */
        U32 denoise_rampdown_cfg_r_reserved_0 :20; /*31:12, No description.               */
    } b;
} T_ABB_denoise_rampdown_cfg_r;

//denoise_other_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 hp_r_pullup_en                 :1 ; /*0 :0 , pull up the inside node voltage of the dc blocking capacitor of right channel through a pulldown resistor*/
        U32 hp_l_pullup_en                 :1 ; /*1 :1 , pull up the inside node voltage of the dc blocking capacitor of left channel through a pulldown resistor*/
        U32 hp_r_dischg_res_en             :1 ; /*2 :2 , pull down the inside node voltage of the dc blocking capacitor of the right channel through a pull down resistor*/
        U32 hp_l_dischg_res_en             :1 ; /*3 :3 , pull down the inside node voltage of the dc blocking capacitor of the left channel through a pull down resistor*/
        U32 hp_dischg_res_bit              :2 ; /*5 :4 , change the resistance of the pulldown resistor*/
        U32 denoise_other_cfg_reserved_0   :26; /*31:6 , No description.               */
    } b;
} T_ABB_denoise_other_cfg;

//ResetControl
typedef union
{
    U32 v;
    struct
    {
        U32 Soft_resetn                    :1 ; /*0 :0 , System software reset0: reset*/
        U32 Register_resetn                :1 ; /*1 :1 , Reset register file0: reset  */
        U32 ResetControl_reserved_0        :30; /*31:2 , No description.               */
    } b;
} T_ABB_ResetControl;

typedef struct
{
    volatile                        T_ABB_ID                             ID; /*0x0  , RO   , 0x0000ABB0, No description.               */
    volatile                T_ABB_codec_cfg0                     codec_cfg0; /*0x4  , RW   , 0x00001897, No description.               */
    volatile                T_ABB_codec_cfg1                     codec_cfg1; /*0x8  , RW   , 0x000007EC, No description.               */
    volatile                  T_ABB_ADC_Gain                       ADC_Gain; /*0xc  , RW   , 0x00000009, No description.               */
    volatile                  T_ABB_DAC_Gain                       DAC_Gain; /*0x10 , RW   , 0x00000145, No description.               */
    volatile               T_ABB_Codec_Mode0                    Codec_Mode0; /*0x14 , RW   , 0x00000033, No description.               */
    volatile               T_ABB_Codec_Mode1                    Codec_Mode1; /*0x18 , RW   , 0x00000002, No description.               */
    volatile                T_ABB_Reset_Ctrl                     Reset_Ctrl; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                T_ABB_Power_Ctrl                     Power_Ctrl; /*0x20 , RW   , 0x00000000, No description.               */
    volatile                  T_ABB_vref_cal                       vref_cal; /*0x24 , RW   , 0x00000087, No description.               */
    volatile                 T_ABB_vrefc_cal                      vrefc_cal; /*0x28 , RW   , 0x00000047, No description.               */
    volatile                  T_ABB_vadc_cal                       vadc_cal; /*0x2c , RW   , 0x0000008F, No description.               */
    volatile                  T_ABB_vdac_cal                       vdac_cal; /*0x30 , RW   , 0x00000047, No description.               */
    volatile            T_ABB_aupll_setting1                 aupll_setting1; /*0x34 , RW   , 0x00000004, No description.               */
    volatile            T_ABB_aupll_setting2                 aupll_setting2; /*0x38 , RW   , 0x00000000, No description.               */
    volatile            T_ABB_aupll_setting3                 aupll_setting3; /*0x3c , RW   , 0x00000000, No description.               */
    volatile                T_ABB_sdmsetting                     sdmsetting; /*0x40 , RW   , 0x00000000, No description.               */
    volatile                  T_ABB_sdmfreq1                       sdmfreq1; /*0x44 , RW   , 0x0000C000, No description.               */
    volatile                  T_ABB_sdmfreq2                       sdmfreq2; /*0x48 , RW   , 0x00000000, No description.               */
    volatile           T_ABB_clk_doubler_cfg                clk_doubler_cfg; /*0x4c , RW   , 0x00000000, clock doubler config          */
    volatile              T_ABB_usb_phy_ctrl                   usb_phy_ctrl; /*0x50 , RW   , 0x00000012, USB PHY Control Register      */
    volatile              T_ABB_clk_chop_div                   clk_chop_div; /*0x54 , RW   , 0x00000400, codec chop clock divider      */
    volatile           T_ABB_abb_rsvd_status                abb_rsvd_status; /*0x58 , RO   , 0x00000000, abb module reserved input     */
    volatile              T_ABB_abb_rsvd_cfg                   abb_rsvd_cfg; /*0x5c , RW   , 0x00000000, abb module reserved input     */
    volatile                T_ABB_hp_chg_cfg                     hp_chg_cfg; /*0x60 , RW   , 0x00000006, pop noise elimination old method config registers*/
    volatile           T_ABB_denoise_pwr_cfg                denoise_pwr_cfg; /*0x64 , RW   , 0x00000000, pop noise elimination new method power config registers*/
    volatile T_ABB_denoise_elimination_dac_clk_cfg denoise_elimination_dac_clk_cfg; /*0x68 , RW   , 0x00000050, pop noise elimination dac clock cfg*/
    volatile       T_ABB_denoise_adc_clk_cfg            denoise_adc_clk_cfg; /*0x6c , RW   , 0x000003E8, pop noise elimination adc clock cfg*/
    volatile        T_ABB_denoise_ramp_cfg_l             denoise_ramp_cfg_l; /*0x70 , RW   , 0x00003842, pop noise elimination ramp cfg for left channel*/
    volatile      T_ABB_denoise_rampup_cfg_l           denoise_rampup_cfg_l; /*0x74 , RW   , 0x00000000, pop noise elimination ramp up cfg for left channel*/
    volatile    T_ABB_denoise_rampdown_cfg_l         denoise_rampdown_cfg_l; /*0x78 , RW   , 0x00000000, pop noise elimination ramp down cfg for left channel*/
    volatile        T_ABB_denoise_ramp_cfg_r             denoise_ramp_cfg_r; /*0x7c , RW   , 0x00003842, pop noise elimination ramp cfg for right channel*/
    volatile      T_ABB_denoise_rampup_cfg_r           denoise_rampup_cfg_r; /*0x80 , RW   , 0x00000000, pop noise elimination ramp up cfg for right channel*/
    volatile    T_ABB_denoise_rampdown_cfg_r         denoise_rampdown_cfg_r; /*0x84 , RW   , 0x00000000, pop noise elimination ramp down cfg for right channel*/
    volatile         T_ABB_denoise_other_cfg              denoise_other_cfg; /*0x88 , RW   , 0x00000030, pop noise elimination other operation registers*/
    volatile              T_ABB_ResetControl                   ResetControl; /*0x8c , RW   , 0x00000003, No description.               */
} T_HWP_ABB_T;
#else
typedef struct
{
    volatile                             U32                             ID; /*0x0  , RO   , 0x0000ABB0, No description.               */
    volatile                             U32                     codec_cfg0; /*0x4  , RW   , 0x00001897, No description.               */
    volatile                             U32                     codec_cfg1; /*0x8  , RW   , 0x000007EC, No description.               */
    volatile                             U32                       ADC_Gain; /*0xc  , RW   , 0x00000009, No description.               */
    volatile                             U32                       DAC_Gain; /*0x10 , RW   , 0x00000145, No description.               */
    volatile                             U32                    Codec_Mode0; /*0x14 , RW   , 0x00000033, No description.               */
    volatile                             U32                    Codec_Mode1; /*0x18 , RW   , 0x00000002, No description.               */
    volatile                             U32                     Reset_Ctrl; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                             U32                     Power_Ctrl; /*0x20 , RW   , 0x00000000, No description.               */
    volatile                             U32                       vref_cal; /*0x24 , RW   , 0x00000087, No description.               */
    volatile                             U32                      vrefc_cal; /*0x28 , RW   , 0x00000047, No description.               */
    volatile                             U32                       vadc_cal; /*0x2c , RW   , 0x0000008F, No description.               */
    volatile                             U32                       vdac_cal; /*0x30 , RW   , 0x00000047, No description.               */
    volatile                             U32                 aupll_setting1; /*0x34 , RW   , 0x00000004, No description.               */
    volatile                             U32                 aupll_setting2; /*0x38 , RW   , 0x00000000, No description.               */
    volatile                             U32                 aupll_setting3; /*0x3c , RW   , 0x00000000, No description.               */
    volatile                             U32                     sdmsetting; /*0x40 , RW   , 0x00000000, No description.               */
    volatile                             U32                       sdmfreq1; /*0x44 , RW   , 0x0000C000, No description.               */
    volatile                             U32                       sdmfreq2; /*0x48 , RW   , 0x00000000, No description.               */
    volatile                             U32                clk_doubler_cfg; /*0x4c , RW   , 0x00000000, clock doubler config          */
    volatile                             U32                   usb_phy_ctrl; /*0x50 , RW   , 0x00000012, USB PHY Control Register      */
    volatile                             U32                   clk_chop_div; /*0x54 , RW   , 0x00000400, codec chop clock divider      */
    volatile                             U32                abb_rsvd_status; /*0x58 , RO   , 0x00000000, abb module reserved input     */
    volatile                             U32                   abb_rsvd_cfg; /*0x5c , RW   , 0x00000000, abb module reserved input     */
    volatile                             U32                     hp_chg_cfg; /*0x60 , RW   , 0x00000006, pop noise elimination old method config registers*/
    volatile                             U32                denoise_pwr_cfg; /*0x64 , RW   , 0x00000000, pop noise elimination new method power config registers*/
    volatile                             U32 denoise_elimination_dac_clk_cfg; /*0x68 , RW   , 0x00000050, pop noise elimination dac clock cfg*/
    volatile                             U32            denoise_adc_clk_cfg; /*0x6c , RW   , 0x000003E8, pop noise elimination adc clock cfg*/
    volatile                             U32             denoise_ramp_cfg_l; /*0x70 , RW   , 0x00003842, pop noise elimination ramp cfg for left channel*/
    volatile                             U32           denoise_rampup_cfg_l; /*0x74 , RW   , 0x00000000, pop noise elimination ramp up cfg for left channel*/
    volatile                             U32         denoise_rampdown_cfg_l; /*0x78 , RW   , 0x00000000, pop noise elimination ramp down cfg for left channel*/
    volatile                             U32             denoise_ramp_cfg_r; /*0x7c , RW   , 0x00003842, pop noise elimination ramp cfg for right channel*/
    volatile                             U32           denoise_rampup_cfg_r; /*0x80 , RW   , 0x00000000, pop noise elimination ramp up cfg for right channel*/
    volatile                             U32         denoise_rampdown_cfg_r; /*0x84 , RW   , 0x00000000, pop noise elimination ramp down cfg for right channel*/
    volatile                             U32              denoise_other_cfg; /*0x88 , RW   , 0x00000030, pop noise elimination other operation registers*/
    volatile                             U32                   ResetControl; /*0x8c , RW   , 0x00000003, No description.               */
} T_HWP_ABB_T;


//id
#define ABB_REVID(n)                   (((n)&0xF)<<0)      
#define ABB_REVID_MASK                 (0xF<<0)            
#define ABB_REVID_SHIFT                (0)                 

#define ABB_CHIPID(n)                  (((n)&0xFFF)<<4)    
#define ABB_CHIPID_MASK                (0xFFF<<4)          
#define ABB_CHIPID_SHIFT               (4)                 

//codec_cfg0
#define ABB_IBIAS_BIT(n)               (((n)&0x3)<<0)      
#define ABB_IBIAS_BIT_MASK             (0x3<<0)            
#define ABB_IBIAS_BIT_SHIFT            (0)                 

#define ABB_VCOMBUF_SEL(n)             (((n)&0xF)<<2)      
#define ABB_VCOMBUF_SEL_MASK           (0xF<<2)            
#define ABB_VCOMBUF_SEL_SHIFT          (2)                 

#define ABB_VCOM_MODE(n)               (((n)&0x7)<<6)      
#define ABB_VCOM_MODE_MASK             (0x7<<6)            
#define ABB_VCOM_MODE_SHIFT            (6)                 

#define ABB_VMIC_BIT(n)                (((n)&0x3)<<9)      
#define ABB_VMIC_BIT_MASK              (0x3<<9)            
#define ABB_VMIC_BIT_SHIFT             (9)                 

#define ABB_SEL_BG                     (1<<11)             

#define ABB_DAC_VOS_BIT(n)             (((n)&0xF)<<12)     
#define ABB_DAC_VOS_BIT_MASK           (0xF<<12)           
#define ABB_DAC_VOS_BIT_SHIFT          (12)                

//codec_cfg1
#define ABB_DAC_CLK_INV_SEL            (1<<0)              
#define ABB_DAC_CLK_INV_SEL_SAMPLING_RISING   (0<<0)              
#define ABB_DAC_CLK_INV_SEL_SAMPLING_FALLING   (1<<0)              

#define ABB_DAC_VREG_BIT(n)            (((n)&0x7)<<1)      
#define ABB_DAC_VREG_BIT_MASK          (0x7<<1)            
#define ABB_DAC_VREG_BIT_SHIFT         (1)                 

#define ABB_ADC_CLK_INV_SEL            (1<<4)              
#define ABB_ADC_CLK_INV_SEL_SAMPLING_RISING   (0<<4)              
#define ABB_ADC_CLK_INV_SEL_SAMPLING_FALLING   (1<<4)              

#define ABB_ADC_IBIT(n)                (((n)&0x3)<<5)      
#define ABB_ADC_IBIT_MASK              (0x3<<5)            
#define ABB_ADC_IBIT_SHIFT             (5)                 

#define ABB_MICPGA_R_IBIT(n)           (((n)&0x3)<<7)      
#define ABB_MICPGA_R_IBIT_MASK         (0x3<<7)            
#define ABB_MICPGA_R_IBIT_SHIFT        (7)                 

#define ABB_MICPGA_L_IBIT(n)           (((n)&0x3)<<9)      
#define ABB_MICPGA_L_IBIT_MASK         (0x3<<9)            
#define ABB_MICPGA_L_IBIT_SHIFT        (9)                 

#define ABB_MICPGA_R_CHOP_EN           (1<<11)             

#define ABB_MICPGA_L_CHOP_EN           (1<<12)             

//adc_gain
#define ABB_ADC_R_GAIN_BIT(n)          (((n)&0x7)<<0)      
#define ABB_ADC_R_GAIN_BIT_MASK        (0x7<<0)            
#define ABB_ADC_R_GAIN_BIT_SHIFT       (0)                 

#define ABB_ADC_L_GAIN_BIT(n)          (((n)&0x7)<<3)      
#define ABB_ADC_L_GAIN_BIT_MASK        (0x7<<3)            
#define ABB_ADC_L_GAIN_BIT_SHIFT       (3)                 

#define ABB_MICPGA_R_GAIN_BIT(n)       (((n)&0x3)<<6)      
#define ABB_MICPGA_R_GAIN_BIT_0DB      (((0)&0x3)<<6)      
#define ABB_MICPGA_R_GAIN_BIT_9DB      (((1)&0x3)<<6)      
#define ABB_MICPGA_R_GAIN_BIT_21DB     (((2)&0x3)<<6)      
#define ABB_MICPGA_R_GAIN_BIT_30DB     (((3)&0x3)<<6)      
#define ABB_MICPGA_R_GAIN_BIT_MASK     (0x3<<6)            
#define ABB_MICPGA_R_GAIN_BIT_SHIFT    (6)                 

#define ABB_MICPGA_L_GAIN_BIT(n)       (((n)&0x3)<<8)      
#define ABB_MICPGA_L_GAIN_BIT_0DB      (((0)&0x3)<<8)      
#define ABB_MICPGA_L_GAIN_BIT_9DB      (((1)&0x3)<<8)      
#define ABB_MICPGA_L_GAIN_BIT_21DB     (((2)&0x3)<<8)      
#define ABB_MICPGA_L_GAIN_BIT_30DB     (((3)&0x3)<<8)      
#define ABB_MICPGA_L_GAIN_BIT_MASK     (0x3<<8)            
#define ABB_MICPGA_L_GAIN_BIT_SHIFT    (8)                 

//dac_gain
#define ABB_HP_GAIN_BIT(n)             (((n)&0x7)<<0)      
#define ABB_HP_GAIN_BIT_MASK           (0x7<<0)            
#define ABB_HP_GAIN_BIT_SHIFT          (0)                 

#define ABB_MUX_GAIN_BIT(n)            (((n)&0x3)<<3)      
#define ABB_MUX_GAIN_BIT_0DB           (((0)&0x3)<<3)      
#define ABB_MUX_GAIN_BIT_3DB           (((1)&0x3)<<3)      
#define ABB_MUX_GAIN_BIT_6DB           (((2)&0x3)<<3)      
#define ABB_MUX_GAIN_BIT_9DB           (((3)&0x3)<<3)      
#define ABB_MUX_GAIN_BIT_MASK          (0x3<<3)            
#define ABB_MUX_GAIN_BIT_SHIFT         (3)                 

#define ABB_DAC_GAIN_BIT(n)            (((n)&0xF)<<5)      
#define ABB_DAC_GAIN_BIT_MASK          (0xF<<5)            
#define ABB_DAC_GAIN_BIT_SHIFT         (5)                 

//codec_mode0
#define ABB_MICINPUT_R_SEL(n)          (((n)&0xF)<<0)      
#define ABB_MICINPUT_R_SEL_MASK        (0xF<<0)            
#define ABB_MICINPUT_R_SEL_SHIFT       (0)                 

#define ABB_MICINPUT_L_SEL(n)          (((n)&0xF)<<4)      
#define ABB_MICINPUT_L_SEL_MASK        (0xF<<4)            
#define ABB_MICINPUT_L_SEL_SHIFT       (4)                 

#define ABB_HP_INPUTSEL(n)             (((n)&0x3)<<8)      
#define ABB_HP_INPUTSEL_MASK           (0x3<<8)            
#define ABB_HP_INPUTSEL_SHIFT          (8)                 

#define ABB_MUX_IBIT(n)                (((n)&0x3)<<10)     
#define ABB_MUX_IBIT_MASK              (0x3<<10)           
#define ABB_MUX_IBIT_SHIFT             (10)                

#define ABB_HP_IBIT(n)                 (((n)&0x3)<<12)     
#define ABB_HP_IBIT_MASK               (0x3<<12)           
#define ABB_HP_IBIT_SHIFT              (12)                

//codec_mode1
#define ABB_ADC_SMPLRATE_SEL(n)        (((n)&0x3)<<0)      
#define ABB_ADC_SMPLRATE_SEL_16KSPS    (((01)&0x3)<<0)     
#define ABB_ADC_SMPLRATE_SEL_8KSPS     (((00)&0x3)<<0)     
#define ABB_ADC_SMPLRATE_SEL_MASK      (0x3<<0)            
#define ABB_ADC_SMPLRATE_SEL_SHIFT     (0)                 

#define ABB_ADC_LV_MODE                (1<<2)              

#define ABB_IVREF_LV_MODE              (1<<3)              

#define ABB_HP_DIFF_MODE               (1<<4)              

#define ABB_LINEIN_MODE                (1<<5)              

#define ABB_DAC_R_MODE                 (1<<6)              

#define ABB_DAC_L_MODE                 (1<<7)              

#define ABB_MICLOOP_R_MODE             (1<<8)              

#define ABB_MICLOOP_L_MODE             (1<<9)              

//reset_ctrl
#define ABB_RESET_HP                   (1<<0)              

#define ABB_RESET_MUX_R                (1<<1)              

#define ABB_RESET_MUX_L                (1<<2)              

#define ABB_RESET_ADC                  (1<<3)              

#define ABB_RESET_MICPGA_R             (1<<4)              

#define ABB_RESET_MICPGA_L             (1<<5)              

//power_ctrl
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
#define ABB_VREF_CAL_BIT_REG_MASK      (0x7<<0)            
#define ABB_VREF_CAL_BIT_REG_SHIFT     (0)                 

#define ABB_VREF_CAL_BIT_DR            (1<<3)              
#define ABB_VREF_CAL_BIT_DR_USE_REG        (1<<3)              
#define ABB_VREF_CAL_BIT_DR_USE_HARDWARE        (0<<3)              

#define ABB_VREF_CAL_CLK_INV           (1<<4)              

#define ABB_VREF_CAL_OUT_POLARITY      (1<<5)              

#define ABB_RESETN_VREF_CAL_REG        (1<<6)              

#define ABB_RESETN_VREF_CAL_DR         (1<<7)              
#define ABB_RESETN_VREF_CAL_DR_HARDWARE (0<<7)              
#define ABB_RESETN_VREF_CAL_DR_RESETN_VREF_CAL_REG (1<<7)              

#define ABB_VREF_CAL_READY             (1<<8)              

//vrefc_cal
#define ABB_VREFC_CAL_BIT_REG(n)       (((n)&0x7)<<0)      
#define ABB_VREFC_CAL_BIT_REG_MASK     (0x7<<0)            
#define ABB_VREFC_CAL_BIT_REG_SHIFT    (0)                 

#define ABB_VREFC_CAL_BIT_DR           (1<<3)              
#define ABB_VREFC_CAL_BIT_DR_USE_REG       (1<<3)              
#define ABB_VREFC_CAL_BIT_DR_USE_HARDWARE       (0<<3)              

#define ABB_VREFC_CAL_CLK_INV          (1<<4)              

#define ABB_VREFC_CAL_OUT_POLARITY     (1<<5)              

#define ABB_RESETN_VREFC_CAL_REG       (1<<6)              

#define ABB_RESETN_VREFC_CAL_DR        (1<<7)              
#define ABB_RESETN_VREFC_CAL_DR_HARDWARE (0<<7)              
#define ABB_RESETN_VREFC_CAL_DR_RESETN_VREFC_CAL_REG (1<<7)              

#define ABB_VREFC_CAL_READY            (1<<8)              

//vadc_cal
#define ABB_VADC_CAL_BIT_REG(n)        (((n)&0xF)<<0)      
#define ABB_VADC_CAL_BIT_REG_MASK      (0xF<<0)            
#define ABB_VADC_CAL_BIT_REG_SHIFT     (0)                 

#define ABB_VADC_CAL_BIT_DR            (1<<4)              
#define ABB_VADC_CAL_BIT_DR_USE_REG        (1<<4)              
#define ABB_VADC_CAL_BIT_DR_USE_HARDWARE        (0<<4)              

#define ABB_VADC_CAL_CLK_INV           (1<<5)              

#define ABB_VADC_CAL_OUT_POLARITY      (1<<6)              

#define ABB_RESETN_VADC_CAL_REG        (1<<7)              

#define ABB_RESETN_VADC_CAL_DR         (1<<8)              
#define ABB_RESETN_VADC_CAL_DR_HARDWARE (0<<8)              
#define ABB_RESETN_VADC_CAL_DR_RESETN_VADC_CAL_REG (1<<8)              

#define ABB_VADC_CAL_READY             (1<<9)              

//vdac_cal
#define ABB_VDAC_CAL_BIT_REG(n)        (((n)&0x7)<<0)      
#define ABB_VDAC_CAL_BIT_REG_MASK      (0x7<<0)            
#define ABB_VDAC_CAL_BIT_REG_SHIFT     (0)                 

#define ABB_VDAC_CAL_BIT_DR            (1<<3)              
#define ABB_VDAC_CAL_BIT_DR_USE_REG        (1<<3)              
#define ABB_VDAC_CAL_BIT_DR_USE_HARDWARE        (0<<3)              

#define ABB_VDAC_CAL_CLK_INV           (1<<4)              

#define ABB_VDAC_CAL_OUT_POLARITY      (1<<5)              

#define ABB_RESETN_VDAC_CAL_REG        (1<<6)              

#define ABB_RESETN_VDAC_CAL_DR         (1<<7)              
#define ABB_RESETN_VDAC_CAL_DR_HARDWARE (0<<7)              
#define ABB_RESETN_VDAC_CAL_DR_RESETN_VDAC_CAL_REG (1<<7)              

#define ABB_VDAC_CAL_READY             (1<<8)              

//aupll_setting1
#define ABB_AUPLL_SDM_CLK_SEL          (1<<0)              

#define ABB_AUPLL_REFMULTI2_EN         (1<<1)              

#define ABB_AUPLL_PCON_MODE            (1<<2)              

#define ABB_AUPLL_CPR2_BIT(n)          (((n)&0x7)<<3)      
#define ABB_AUPLL_CPR2_BIT_MASK        (0x7<<3)            
#define ABB_AUPLL_CPR2_BIT_SHIFT       (3)                 

#define ABB_AUPLL_CPC2_BIT(n)          (((n)&0x7)<<6)      
#define ABB_AUPLL_CPC2_BIT_MASK        (0x7<<6)            
#define ABB_AUPLL_CPC2_BIT_SHIFT       (6)                 

#define ABB_AUPLL_VCO_REG_BIT(n)       (((n)&0xF)<<9)      
#define ABB_AUPLL_VCO_REG_BIT_MASK     (0xF<<9)            
#define ABB_AUPLL_VCO_REG_BIT_SHIFT    (9)                 

////aupll_setting2
#define ABB_PU_AUPLL                   (1<<0)              

#define ABB_AUPLL_RSTB                 (1<<1)              

#define ABB_AUPLL_VCO_HIGH_TEST        (1<<2)              

#define ABB_AUPLL_VCO_LOW_TEST         (1<<3)              

#define ABB_AUPLL_CLKOUT_EN(n)         (((n)&0x7)<<4)      
#define ABB_AUPLL_CLKOUT_EN_MASK       (0x7<<4)            
#define ABB_AUPLL_CLKOUT_EN_SHIFT      (4)                 

#define ABB_AUPLL_SDM_CLK_TEST_EN      (1<<7)              

#define ABB_AUPLL_TEST_EN              (1<<8)              

#define ABB_AUPLL_READY_TIMER(n)       (((n)&0x3)<<9)      
#define ABB_AUPLL_READY_TIMER_50US     (((0)&0x3)<<9)      
#define ABB_AUPLL_READY_TIMER_100US    (((1)&0x3)<<9)      
#define ABB_AUPLL_READY_TIMER_200US    (((2)&0x3)<<9)      
#define ABB_AUPLL_READY_TIMER_400US    (((3)&0x3)<<9)      
#define ABB_AUPLL_READY_TIMER_MASK     (0x3<<9)            
#define ABB_AUPLL_READY_TIMER_SHIFT    (9)                 

//aupll_setting3
#define ABB_AUPLL_DIG_REG_RES_MODE_VBIT(n) (((n)&0x7)<<0)      
#define ABB_AUPLL_DIG_REG_RES_MODE_VBIT_MASK (0x7<<0)            
#define ABB_AUPLL_DIG_REG_RES_MODE_VBIT_SHIFT (0)                 

#define ABB_AUPLL_CPBIAS_BIT(n)        (((n)&0x7)<<3)      
#define ABB_AUPLL_CPBIAS_BIT_MASK      (0x7<<3)            
#define ABB_AUPLL_CPBIAS_BIT_SHIFT     (3)                 

#define ABB_AUPLL_DIG_REG_ROUT_BIT(n)  (((n)&0xF)<<6)      
#define ABB_AUPLL_DIG_REG_ROUT_BIT_MASK (0xF<<6)            
#define ABB_AUPLL_DIG_REG_ROUT_BIT_SHIFT (6)                 

#define ABB_AUPLL_DIG_REG_RANGE_BIT(n) (((n)&0xF)<<10)     
#define ABB_AUPLL_DIG_REG_RANGE_BIT_MASK (0xF<<10)           
#define ABB_AUPLL_DIG_REG_RANGE_BIT_SHIFT (10)                

#define ABB_AUPLL_LOCK                 (1<<14)             

#define ABB_AUPLL_READY                (1<<15)             

//sdm_setting
#define ABB_SDM_DITHER_BYPASS          (1<<0)              

#define ABB_SDM_INT_DEC_SEL(n)         (((n)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_INTEGER    (((0)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_3BIT       (((1)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_2BIT       (((2)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_1BIT       (((3)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_NO         (((4)&0x7)<<1)      
#define ABB_SDM_INT_DEC_SEL_MASK       (0x7<<1)            
#define ABB_SDM_INT_DEC_SEL_SHIFT      (1)                 

#define ABB_SDM_RESETN                 (1<<4)              

#define ABB_SDM_CLK_INV                (1<<5)              
#define ABB_SDM_CLK_INV_INVERTER       (1<<5)              
#define ABB_SDM_CLK_INV_NORMAL         (0<<5)              

#define ABB_REFDIV2_EN                 (1<<6)              

//sdm_freq1
#define ABB_SDM_FREQ_HIGH(n)           (((n)&0xFFFF)<<0)   
#define ABB_SDM_FREQ_HIGH_MASK         (0xFFFF<<0)         
#define ABB_SDM_FREQ_HIGH_SHIFT        (0)                 

//sdm_freq2
#define ABB_SDM_FREQ_LOW(n)            (((n)&0xFFFF)<<0)   
#define ABB_SDM_FREQ_LOW_MASK          (0xFFFF<<0)         
#define ABB_SDM_FREQ_LOW_SHIFT         (0)                 

//clk_doubler_cfg
#define ABB_PU_CLKDBLR                 (1<<0)              

#define ABB_CLKDBLR_IBIAS_CTRL         (1<<1)              

#define ABB_CLKDBLR_VBIAS_CTRL(n)      (((n)&0x3)<<2)      
#define ABB_CLKDBLR_VBIAS_CTRL_MASK    (0x3<<2)            
#define ABB_CLKDBLR_VBIAS_CTRL_SHIFT   (2)                 

#define ABB_CLKDBLR_R_CTRL(n)          (((n)&0x7)<<4)      
#define ABB_CLKDBLR_R_CTRL_MASK        (0x7<<4)            
#define ABB_CLKDBLR_R_CTRL_SHIFT       (4)                 

//usb_phy_ctrl
#define ABB_PU_USB                     (1<<0)              

#define ABB_USB_VREG_BIT(n)            (((n)&0x7)<<1)      
#define ABB_USB_VREG_BIT_MASK          (0x7<<1)            
#define ABB_USB_VREG_BIT_SHIFT         (1)                 

#define ABB_USB_CTRL(n)                (((n)&0x7)<<4)      
#define ABB_USB_CTRL_NONE              (((0)&0x7)<<4)      
#define ABB_USB_CTRL_PUDP              (((1)&0x7)<<4)      
#define ABB_USB_CTRL_PUDM              (((2)&0x7)<<4)      
#define ABB_USB_CTRL_PUDPDM            (((3)&0x7)<<4)      
#define ABB_USB_CTRL_PDDPDM            (((4)&0x7)<<4)      
#define ABB_USB_CTRL_RESERVED0         (((5)&0x7)<<4)      
#define ABB_USB_CTRL_RESERVED1         (((6)&0x7)<<4)      
#define ABB_USB_CTRL_RESERVED2         (((7)&0x7)<<4)      
#define ABB_USB_CTRL_MASK              (0x7<<4)            
#define ABB_USB_CTRL_SHIFT             (4)                 

//clk_chop_div
#define ABB_CLK_CHOP_EN                (1<<0)              

#define ABB_CLK_CHOP_DIV_BASE(n)       (((n)&0x3FF)<<1)    
#define ABB_CLK_CHOP_DIV_BASE_MASK     (0x3FF<<1)          
#define ABB_CLK_CHOP_DIV_BASE_SHIFT    (1)                 

//abb_rsvd_status
#define ABB_ABB_STATUS(n)              (((n)&0xFF)<<0)     
#define ABB_ABB_STATUS_MASK            (0xFF<<0)           
#define ABB_ABB_STATUS_SHIFT           (0)                 

#define ABB_ABB_CFG_BAK(n)             (((n)&0xFF)<<8)     
#define ABB_ABB_CFG_BAK_MASK           (0xFF<<8)           
#define ABB_ABB_CFG_BAK_SHIFT          (8)                 

//abb_rsvd_cfg
#define ABB_ABB_CFG(n)                 (((n)&0xFFFF)<<0)   
#define ABB_ABB_CFG_MASK               (0xFFFF<<0)         
#define ABB_ABB_CFG_SHIFT              (0)        
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
#define ABB_HP_L_CHG_RES_BIT_MASK        (0x3<<1)            
#define ABB_HP_L_CHG_RES_BIT_SHIFT       (1)                 

//denoise_pwr_cfg
#define ABB_HP_DAC_CHG_IBIT(n)         (((n)&0xF)<<0)      
#define ABB_HP_DAC_CHG_IBIT_MASK       (0xF<<0)            
#define ABB_HP_DAC_CHG_IBIT_SHIFT      (0)                 

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
#define ABB_DENOISE_DAC_CLK_DIV_MASK   (0x7F<<0)           
#define ABB_DENOISE_DAC_CLK_DIV_SHIFT  (0)                 

#define ABB_DENOISE_DAC_CLK_INV(n)     (((n)&0x3)<<7)      
#define ABB_DENOISE_DAC_CLK_INV_MASK   (0x3<<7)            
#define ABB_DENOISE_DAC_CLK_INV_SHIFT  (7)                 

#define ABB_DENOISE_DAC_CLK_LD         (1<<9)              

//denoise_adc_clk_cfg
#define ABB_DENOISE_ADC_CLK_DIV(n)     (((n)&0x3FF)<<0)    
#define ABB_DENOISE_ADC_CLK_DIV_MASK   (0x3FF<<0)          
#define ABB_DENOISE_ADC_CLK_DIV_SHIFT  (0)                 

#define ABB_DENOISE_ADC_CLK_INV(n)     (((n)&0x3)<<10)     
#define ABB_DENOISE_ADC_CLK_INV_MASK   (0x3<<10)           
#define ABB_DENOISE_ADC_CLK_INV_SHIFT  (10)                

#define ABB_DENOISE_ADC_CLK_LD         (1<<12)             

//denoise_ramp_cfg_l
#define ABB_START_DENOISE_L            (1<<0)              

#define ABB_STABLE_TIME_L(n)           (((n)&0x1F)<<1)     
#define ABB_STABLE_TIME_L_MASK         (0x1F<<1)           
#define ABB_STABLE_TIME_L_SHIFT        (1)                 

#define ABB_RAMPUP_CYCLE_L(n)          (((n)&0xF)<<6)      
#define ABB_RAMPUP_CYCLE_L_MASK        (0xF<<6)            
#define ABB_RAMPUP_CYCLE_L_SHIFT       (6)                 

#define ABB_RAMPDOWN_EN_L              (1<<10)             

#define ABB_ADC_DET_TH_L(n)            (((n)&0xF)<<11)     
#define ABB_ADC_DET_TH_L_MASK          (0xF<<11)           
#define ABB_ADC_DET_TH_L_SHIFT         (11)                

#define ABB_RAMP_DONE_L                (1<<15)             

//denoise_rampup_cfg_l
#define ABB_RAMPUP_TH_VALUE_L(n)       (((n)&0xFFF)<<0)    
#define ABB_RAMPUP_TH_VALUE_L_MASK     (0xFFF<<0)          
#define ABB_RAMPUP_TH_VALUE_L_SHIFT    (0)                 

//denoise_rampdown_cfg_l
#define ABB_RAMPDOWN_FINAL_VALUE_L(n)  (((n)&0xFFF)<<0)    
#define ABB_RAMPDOWN_FINAL_VALUE_L_MASK (0xFFF<<0)          
#define ABB_RAMPDOWN_FINAL_VALUE_L_SHIFT (0)                 

//denoise_ramp_cfg_r
#define ABB_START_DENOISE_R            (1<<0)              

#define ABB_STABLE_TIME_R(n)           (((n)&0x1F)<<1)     
#define ABB_STABLE_TIME_R_MASK         (0x1F<<1)           
#define ABB_STABLE_TIME_R_SHIFT        (1)                 

#define ABB_RAMPUP_CYCLE_R(n)          (((n)&0xF)<<6)      
#define ABB_RAMPUP_CYCLE_R_MASK        (0xF<<6)            
#define ABB_RAMPUP_CYCLE_R_SHIFT       (6)                 

#define ABB_RAMPDOWN_EN_R              (1<<10)             

#define ABB_ADC_DET_TH_R(n)            (((n)&0xF)<<11)     
#define ABB_ADC_DET_TH_R_MASK          (0xF<<11)           
#define ABB_ADC_DET_TH_R_SHIFT         (11)                

#define ABB_RAMP_DONE_R                (1<<15)             

//denoise_rampup_cfg_r
#define ABB_RAMPUP_TH_VALUE_R(n)       (((n)&0xFFF)<<0)    
#define ABB_RAMPUP_TH_VALUE_R_MASK     (0xFFF<<0)          
#define ABB_RAMPUP_TH_VALUE_R_SHIFT    (0)                 

//denoise_rampdown_cfg_r
#define ABB_RAMPDOWN_FINAL_VALUE_R(n)  (((n)&0xFFF)<<0)    
#define ABB_RAMPDOWN_FINAL_VALUE_R_MASK (0xFFF<<0)          
#define ABB_RAMPDOWN_FINAL_VALUE_R_SHIFT (0)                 

//denoise_other_cfg
#define ABB_HP_R_PULLUP_EN             (1<<0)              

#define ABB_HP_L_PULLUP_EN             (1<<1)              

#define ABB_HP_R_DISCHG_RES_EN         (1<<2)              

#define ABB_HP_L_DISCHG_RES_EN         (1<<3)              

#define ABB_HP_DISCHG_RES_BIT(n)       (((n)&0x3)<<4)      
#define ABB_HP_DISCHG_RES_BIT_MASK     (0x3<<4)            
#define ABB_HP_DISCHG_RES_BIT_SHIFT    (4)                 

//Reset  Control
#define ABB_SOFT_RESETN                (1<<0)              

#define ABB_REGISTER_RESETN            (1<<1)              


#endif

#endif


