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
// File name      : pmuc                                            
// Author         : liqiu                                                 
// Date of created: 2016-11-01 11:06:11                                                  
// Last modified  : 2016-11-01 11:06:11                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-11-01 11:06:11 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __PMUC_H__
#define __PMUC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#define  NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//test_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 scan_mode0                     :1 ; /*0 :0 , No description.               */
        U32 scan_mode1                     :1 ; /*1 :1 , No description.               */
        U32 scan_rstb                      :1 ; /*2 :2 , No description.               */
        U32 bist_mode0                     :1 ; /*3 :3 , No description.               */
        U32 bist_mode1                     :1 ; /*4 :4 , No description.               */
        U32 func_test_mode                 :1 ; /*5 :5 , No description.               */
        U32 edt_bypass                     :1 ; /*6 :6 , No description.               */
        U32 occ_lf_dc_mode                 :1 ; /*7 :7 , No description.               */
        U32 occ_hf_dc_mode                 :1 ; /*8 :8 , No description.               */
        U32 spiflash_test_mode             :1 ; /*9 :9 , No description.               */
        U32 reserved_0                     :1 ; /*10:10,                               */
        U32 reserved_1                     :1 ; /*11:11,                               */
        U32 reserved_2                     :1 ; /*12:12,                               */
        U32 reserved_3                     :1 ; /*13:13,                               */
        U32 reserved_4                     :1 ; /*14:14,                               */
        U32 reserved_5                     :1 ; /*15:15,                               */
        U32 reserved_6                     :1 ; /*16:16,                               */
        U32 reserved_7                     :1 ; /*17:17,                               */
        U32 reserved_8                     :1 ; /*18:18,                               */
        U32 reserved_9                     :1 ; /*19:19,                               */
        U32 reserved_10                    :1 ; /*20:20,                               */
        U32 reserved_11                    :1 ; /*21:21,                               */
        U32 reserved_12                    :1 ; /*22:22,                               */
        U32 reserved_13                    :1 ; /*23:23,                               */
        U32 reserved_14                    :1 ; /*24:24,                               */
        U32 reserved_15                    :1 ; /*25:25,                               */
        U32 reserved_16                    :1 ; /*26:26,                               */
        U32 reserved_17                    :1 ; /*27:27,                               */
        U32 reserved_18                    :1 ; /*28:28,                               */
        U32 reserved_19                    :1 ; /*29:29,                               */
        U32 reserved_20                    :1 ; /*30:30,                               */
        U32 reserved_21                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_test_cfg;

//pwr_sys_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 keysense_enb                   :1 ; /*0 :0 , No description.               */
        U32 self_resetn                    :1 ; /*1 :1 , No description.               */
        U32 pm_reg                         :3 ; /*4 :2 , power mode register           */
        U32 lp_mode_b                      :1 ; /*5 :5 , No description.               */
        U32 key_thld                       :2 ; /*7 :6 , No description.               */
        U32 clk_fine_sel                   :1 ; /*8 :8 , No description.               */
        U32 clk_raw_sel                    :1 ; /*9 :9 , No description.               */
        U32 clk_pmuc_sel                   :1 ; /*10:10, No description.               */
        U32 chr_ac_on_reset_bypass         :1 ; /*11:11, No description.               */
        U32 lowbat_off_bypass              :1 ; /*12:12, No description.               */
        U32 xtal_rdy_time_sel              :3 ; /*15:13, No description.               */
        U32 chr_ac_on_time_sel             :2 ; /*17:16, No description.               */
        U32 i2cs_disable                   :1 ; /*18:18, No description.               */
        U32 hst_clk_disable                :1 ; /*19:19, No description.               */
        U32 tab_rst_bypass                 :1 ; /*20:20, No description.               */
        U32 reserved_0                     :1 ; /*21:21,                               */
        U32 reserved_1                     :1 ; /*22:22,                               */
        U32 reserved_2                     :1 ; /*23:23,                               */
        U32 reserved_3                     :1 ; /*24:24,                               */
        U32 reserved_4                     :1 ; /*25:25,                               */
        U32 reserved_5                     :1 ; /*26:26,                               */
        U32 reserved_6                     :1 ; /*27:27,                               */
        U32 reserved_7                     :1 ; /*28:28,                               */
        U32 reserved_8                     :1 ; /*29:29,                               */
        U32 reserved_9                     :1 ; /*30:30,                               */
        U32 reserved_10                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_pwr_sys_ctrl;

//pm_fsm_status
typedef union
{
    U32 v;
    struct
    {
        U32 pm_fsm_state                   :7 ; /*6 :0 , No description.               */
        U32 pwrswitch_en                   :1 ; /*7 :7 , No description.               */
        U32 pu_bg_pmu                      :1 ; /*8 :8 , No description.               */
        U32 pu_xtal_26m_lv                 :1 ; /*9 :9 , No description.               */
        U32 pu_mem_grp1                    :1 ; /*10:10, No description.               */
        U32 pu_mem_grp2                    :1 ; /*11:11, No description.               */
        U32 pu_mem_grp3                    :1 ; /*12:12, No description.               */
        U32 pu_buck_ldo                    :1 ; /*13:13, No description.               */
        U32 pu_ldo_vcore                   :1 ; /*14:14, No description.               */
        U32 rstb_dbb                       :1 ; /*15:15, No description.               */
        U32 vrtc_out_sel                   :1 ; /*16:16, No description.               */
        U32 pwrswitch_en_dly               :1 ; /*17:17, No description.               */
        U32 pu_bt_aon                      :1 ; /*18:18, No description.               */
        U32 reserved_0                     :1 ; /*19:19,                               */
        U32 reserved_1                     :1 ; /*20:20,                               */
        U32 reserved_2                     :1 ; /*21:21,                               */
        U32 reserved_3                     :1 ; /*22:22,                               */
        U32 reserved_4                     :1 ; /*23:23,                               */
        U32 reserved_5                     :1 ; /*24:24,                               */
        U32 reserved_6                     :1 ; /*25:25,                               */
        U32 reserved_7                     :1 ; /*26:26,                               */
        U32 reserved_8                     :1 ; /*27:27,                               */
        U32 reserved_9                     :1 ; /*28:28,                               */
        U32 reserved_10                    :1 ; /*29:29,                               */
        U32 reserved_11                    :1 ; /*30:30,                               */
        U32 reserved_12                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_pm_fsm_status;

//pm_fsm_frc_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 pwrswitch_en_reg               :1 ; /*0 :0 , No description.               */
        U32 pwrswitch_en_dr                :1 ; /*1 :1 , No description.               */
        U32 pu_bg_reg                      :1 ; /*2 :2 , No description.               */
        U32 pu_bg_dr                       :1 ; /*3 :3 , No description.               */
        U32 pu_26m_reg                     :1 ; /*4 :4 , No description.               */
        U32 pu_26m_dr                      :1 ; /*5 :5 , No description.               */
        U32 pu_mem_grp1_reg                :1 ; /*6 :6 , No description.               */
        U32 pu_mem_grp1_dr                 :1 ; /*7 :7 , No description.               */
        U32 pu_mem_grp2_reg                :1 ; /*8 :8 , No description.               */
        U32 pu_mem_grp2_dr                 :1 ; /*9 :9 , No description.               */
        U32 pu_mem_grp3_reg                :1 ; /*10:10, No description.               */
        U32 pu_mem_grp3_dr                 :1 ; /*11:11, No description.               */
        U32 pu_buck_ldo_reg                :1 ; /*12:12, No description.               */
        U32 pu_buck_ldo_dr                 :1 ; /*13:13, No description.               */
        U32 pu_ldo_vcore_reg               :1 ; /*14:14, No description.               */
        U32 pu_ldo_vcore_dr                :1 ; /*15:15, No description.               */
        U32 rstb_dbb_reg                   :1 ; /*16:16, No description.               */
        U32 rstb_dbb_dr                    :1 ; /*17:17, No description.               */
        U32 vrtc_out_sel_reg               :1 ; /*18:18, No description.               */
        U32 vrtc_out_sel_dr                :1 ; /*19:19, No description.               */
        U32 pwrswitch_en_dly_reg           :1 ; /*20:20, No description.               */
        U32 pwrswitch_en_dly_dr            :1 ; /*21:21, No description.               */
        U32 pu_bt_aon_reg                  :1 ; /*22:22, No description.               */
        U32 pu_bt_aon_dr                   :1 ; /*23:23, No description.               */
        U32 mem_grp1_on                    :1 ; /*24:24, No description.               */
        U32 mem_grp2_on                    :1 ; /*25:25, No description.               */
        U32 mem_grp3_on                    :1 ; /*26:26, No description.               */
        U32 reserved_0                     :1 ; /*27:27,                               */
        U32 reserved_1                     :1 ; /*28:28,                               */
        U32 reserved_2                     :1 ; /*29:29,                               */
        U32 reserved_3                     :1 ; /*30:30,                               */
        U32 reserved_4                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_pm_fsm_frc_ctrl1;

//xtal_clk32k_div
typedef union
{
    U32 v;
    struct
    {
        U32 step_offset_lp                 :8 ; /*7 :0 , No description.               */
        U32 step_offset_normal             :8 ; /*15:8 , No description.               */
        U32 step_offset_update             :1 ; /*16:16, No description.               */
        U32 div_lp_mode_h                  :1 ; /*17:17, No description.               */
        U32 div_lp_mode_h_reg              :1 ; /*18:18, No description.               */
        U32 div_lp_mode_h_dr               :1 ; /*19:19, No description.               */
        U32 xtal_vamp_ibit_xosc_active     :4 ; /*23:20, No description.               */
        U32 xtal_fix_ibit_xosc_active      :5 ; /*28:24, No description.               */
        U32 reserved_0                     :1 ; /*29:29,                               */
        U32 reserved_1                     :1 ; /*30:30,                               */
        U32 reserved_2                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_xtal_clk32k_div;

//xtal_cfg_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 xtal_vamp_ibit_xosc_lp         :4 ; /*3 :0 , No description.               */
        U32 xtal_reg_bit_lv                :4 ; /*7 :4 , No description.               */
        U32 xtal_capbank_bit_lv            :4 ; /*11:8 , No description.               */
        U32 xtal_cap_fine_lv               :6 ; /*17:12, No description.               */
        U32 xtal_fix_ibit_xosc_lp          :5 ; /*22:18, No description.               */
        U32 xtal_clk_lp_en_lv              :1 ; /*23:23, xtal_6m5_en                   */
        U32 pu_xtal_26m_reg                :1 ; /*24:24, No description.               */
        U32 pu_xtal_26m_dr                 :1 ; /*25:25, No description.               */
        U32 xtal_lp_mode_en_reg            :1 ; /*26:26, No description.               */
        U32 xtal_lp_mode_en_dr             :1 ; /*27:27, No description.               */
        U32 reserved_0                     :1 ; /*28:28,                               */
        U32 reserved_1                     :1 ; /*29:29,                               */
        U32 reserved_2                     :1 ; /*30:30,                               */
        U32 reserved_3                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_xtal_cfg_ctrl;

//ldo_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ldo_ulp_vpad_vbit              :3 ; /*2 :0 , tuning output of ulp_vpadldo_ulp_vpad_vbit&lt;2&gt;=1 2.8Vldo_ulp_vpad_vbit&lt;2&gt;=1 1.8V*/
        U32 ldo_ulp_vrtc_vbit              :2 ; /*4 :3 , tuning output of ulp_vrtc     */
        U32 vbuck_ldo_bit                  :3 ; /*7 :5 , No description.               */
        U32 ldo_vcore_vbit                 :3 ; /*10:8 , output tuning bits            */
        U32 vout_sel_buck_ldo              :1 ; /*11:11, No description.               */
        U32 ldo_vcore_discharge_en         :1 ; /*12:12, No description.               */
        U32 reserved_0                     :1 ; /*13:13,                               */
        U32 reserved_1                     :1 ; /*14:14,                               */
        U32 reserved_2                     :1 ; /*15:15,                               */
        U32 reserved_3                     :1 ; /*16:16,                               */
        U32 reserved_4                     :1 ; /*17:17,                               */
        U32 reserved_5                     :1 ; /*18:18,                               */
        U32 reserved_6                     :1 ; /*19:19,                               */
        U32 reserved_7                     :1 ; /*20:20,                               */
        U32 reserved_8                     :1 ; /*21:21,                               */
        U32 reserved_9                     :1 ; /*22:22,                               */
        U32 reserved_10                    :1 ; /*23:23,                               */
        U32 reserved_11                    :1 ; /*24:24,                               */
        U32 reserved_12                    :1 ; /*25:25,                               */
        U32 reserved_13                    :1 ; /*26:26,                               */
        U32 reserved_14                    :1 ; /*27:27,                               */
        U32 reserved_15                    :1 ; /*28:28,                               */
        U32 reserved_16                    :1 ; /*29:29,                               */
        U32 reserved_17                    :1 ; /*30:30,                               */
        U32 reserved_18                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_ldo_ctrl;

//bandgap_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 bg_cal_tc_bit                  :5 ; /*4 :0 , tunning the temperatrue coefficient bandgap voltage.*/
        U32 bg_freq_bit                    :2 ; /*6 :5 , tunning chopping frequency. 00: ~400kHz;01:~750kHz; 10:~1.1MHz; 11:1.5MHz*/
        U32 bg_ts_i_bit                    :4 ; /*10:7 , tunning the current for TS. 0000 or pu_ts:0uA; 0001:10uA;  0010 or 0100:20uA; 0011 or 0101:30uA; 0110:40uA; 0111 or 1000:50uA; 1001:60uA; 1100 or 1010:70uA; 1101 or 1011:80uA; 1110:90uA; 1111:100uA*/
        U32 pu_ts_bg                       :1 ; /*11:11, disable temperature sense of battery. 1:enable temperature sense;0:disable temperature sense.*/
        U32 lp_mode_bg                     :1 ; /*12:12, low power mode of bandgap.1:low power mode;0:normal mode.*/
        U32 bg_chopper_enb                 :1 ; /*13:13, disable chopping function.1:disable; 0:enable*/
        U32 ref_sel_bg                     :1 ; /*14:14, select output reference between bandgap out and vref_pre_reg. ref_sel should follow below sequence: when power up, rel_sel should first be 1(the output v1200m=vref_pre_reg), after about 50us when chopping is working, change ref_sel to 0(the output v1200m=vref_bg with chopping)*/
        U32 reserved_0                     :1 ; /*15:15,                               */
        U32 reserved_1                     :1 ; /*16:16,                               */
        U32 reserved_2                     :1 ; /*17:17,                               */
        U32 reserved_3                     :1 ; /*18:18,                               */
        U32 reserved_4                     :1 ; /*19:19,                               */
        U32 reserved_5                     :1 ; /*20:20,                               */
        U32 reserved_6                     :1 ; /*21:21,                               */
        U32 reserved_7                     :1 ; /*22:22,                               */
        U32 reserved_8                     :1 ; /*23:23,                               */
        U32 reserved_9                     :1 ; /*24:24,                               */
        U32 reserved_10                    :1 ; /*25:25,                               */
        U32 reserved_11                    :1 ; /*26:26,                               */
        U32 reserved_12                    :1 ; /*27:27,                               */
        U32 reserved_13                    :1 ; /*28:28,                               */
        U32 reserved_14                    :1 ; /*29:29,                               */
        U32 reserved_15                    :1 ; /*30:30,                               */
        U32 reserved_16                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_bandgap_cfg;

//buck_cp_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pu_buck                        :1 ; /*0 :0 , enable dcdc                   */
        U32 pfm_mode_sel_buck              :1 ; /*1 :1 , pfm mode select1:pfm mode and pwm mode auto switch0:force pwm mode*/
        U32 vbuck_bit                      :4 ; /*5 :2 , dcdc output voltage fine adjust*/
        U32 discharge_en_buck              :1 ; /*6 :6 , discharge the output of buck when power down*/
        U32 heavy_load_buck                :1 ; /*7 :7 , increase peak detect current to increase load current*/
        U32 antiring_disable_buck          :1 ; /*8 :8 , disable antiring              */
        U32 low_sense_buck                 :1 ; /*9 :9 , discharge the output of buck when power down*/
        U32 pfm_buck_threshold             :2 ; /*11:10, pfm mode current threshold    */
        U32 osc_freq_buck                  :2 ; /*13:12, internal clock frequency control*/
        U32 counter_disable_buck           :1 ; /*14:14, disable internal counter      */
        U32 hys_vbp_lowbat_buck            :1 ; /*15:15, ?                             */
        U32 pfm_clk_disable_buck           :1 ; /*16:16, ?                             */
        U32 nonov_delay_buck               :2 ; /*18:17, dcdc nonovlap clk dead time adjust*/
        U32 buck_ldo_vref_sel              :1 ; /*19:19, vref select bit when working under ldo mode*/
        U32 pfm_vref_only_buck             :1 ; /*20:20, No description.               */
        U32 charge_pump_ldo_en             :1 ; /*21:21, No description.               */
        U32 reserved_0                     :1 ; /*22:22,                               */
        U32 reserved_1                     :1 ; /*23:23,                               */
        U32 reserved_2                     :1 ; /*24:24,                               */
        U32 reserved_3                     :1 ; /*25:25,                               */
        U32 reserved_4                     :1 ; /*26:26,                               */
        U32 reserved_5                     :1 ; /*27:27,                               */
        U32 reserved_6                     :1 ; /*28:28,                               */
        U32 reserved_7                     :1 ; /*29:29,                               */
        U32 reserved_8                     :1 ; /*30:30,                               */
        U32 reserved_9                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_buck_cp_ctrl;

//ldo_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 vusb_vbit                      :3 ; /*2 :0 , No description.               */
        U32 vusb_vsel                      :2 ; /*4 :3 , No description.               */
        U32 vusb_excap_en                  :1 ; /*5 :5 , No description.               */
        U32 pu_vusb                        :1 ; /*6 :6 , No description.               */
        U32 vio_vsel                       :2 ; /*8 :7 , output sel1:2.8V0:3.3V      */
        U32 vio_vbit                       :3 ; /*11:9 , output tuning bits            */
        U32 pu_vio                         :1 ; /*12:12, power down ldo_vio280:power down mode1:normal mode*/
        U32 vsel_va                        :1 ; /*13:13, output sel1:2.4V0:2.8V      */
        U32 va_vbit                        :3 ; /*16:14, output tuning bits            */
        U32 va_cc_disable                  :1 ; /*17:17, No description.               */        U32 pu_va_normal                   :1 ; /*18:18, power down ldo_va0:power down mode1:normal mode*/
        U32 pu_va_lp                       :1 ; /*19:19, No description.               */
        U32 lv_detect_pmu                  :1 ; /*20:20, No description.               */
        U32 vio_excap_en                   :1 ; /*21:21, No description.               */
        U32 reserved_0                     :1 ; /*22:22,                               */
        U32 reserved_1                     :1 ; /*23:23,                               */
        U32 reserved_2                     :1 ; /*24:24,                               */
        U32 reserved_3                     :1 ; /*25:25,                               */
        U32 reserved_4                     :1 ; /*26:26,                               */
        U32 reserved_5                     :1 ; /*27:27,                               */
        U32 reserved_6                     :1 ; /*28:28,                               */
        U32 reserved_7                     :1 ; /*29:29,                               */
        U32 reserved_8                     :1 ; /*30:30,                               */
        U32 reserved_9                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_ldo_cfg;

//charger_cfg_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pre_chr_check_bypass           :1 ; /*0 :0 , No description.               */
        U32 chr_vfb_sel                    :4 ; /*4 :1 , tunning the floating output voltage of charger.*/
        U32 chr_vfb_sel_reg                :4 ; /*8 :5 , No description.               */
        U32 chr_vfb_sel_dr                 :1 ; /*9 :9 , No description.               */
        U32 chr_pre_i_bit                  :3 ; /*12:10, pre-charging current tunning bits*/
        U32 chr_cc_i_bit                   :3 ; /*15:13, constant current charging tunning bits*/
        U32 chr_cc_mode                    :1 ; /*16:16, constant current charging mode. 1:constant current mode; 0:pre-charge mode.*/
        U32 chr_cc_mode_reg                :1 ; /*17:17, No description.               */
        U32 chr_cc_mode_dr                 :1 ; /*18:18, No description.               */
        U32 chr_cv_lowgain                 :1 ; /*19:19, constant voltage charging lowgain mode, if it is unstable under cv mode, then change this value to 1, it can help the stability of the system. 1: low gain mode; 0:normal gain mode*/
        U32 uv_sel                         :2 ; /*21:20, tunning threshold of under voltage lockout.00: L-&gt;H(low to high threshold) is 3.21V, H-&gt;L(high to low threshold) is 2.57V;01:L-&gt;H is 3.21V, H-&gt;L is  2.73V;10: L-&gt;H is 3.21V; H-&gt;L is 2.88V;11: L-&gt;H is 3.21V, H-&gt;L is 3.03V.(thresload needed modify)*/
        U32 uv_sel_reg                     :2 ; /*23:22, No description.               */
        U32 uv_sel_dr                      :1 ; /*24:24, No description.               */
        U32 chr_auxadc_sel                 :2 ; /*26:25, select the auxout to gpadc.                    00: sel sense voltage of charging current to gpadc;01: sel TS(Temperature sense) voltage to gpadc; 10: sel ac_div_in (ac/6) to gpadc;11:NC*/
        U32 chr_enable                     :1 ; /*27:27, enable charger. 1:enable charger; 0:disable charger*/
        U32 chr_enable_reg                 :1 ; /*28:28, No description.               */
        U32 chr_enable_dr                  :1 ; /*29:29, No description.               */
        U32 chr_ac_uvlo                    :1 ; /*30:30, No description.               */
        U32 reserved_0                     :1 ; /*31:31,                               */
    } b;
} T_PMUC_charger_cfg_ctrl;

//ana_status
typedef union
{
    U32 v;
    struct
    {
        U32 chr_creg                       :1 ; /*0 :0 , indicates that the battery voltage is high enough to enter constant current mode. 1: can enter constant current mode; 0: can't enter.*/
        U32 chr_vreg                       :1 ; /*1 :1 , indicates that the battery voltage is higher than 4.0V. 1: higher than 4.0V; 0:lower than 4.0V.*/
        U32 chr_term                       :1 ; /*2 :2 , indicates that the battery is full charged*/
        U32 chr_ac_on                      :1 ; /*3 :3 , indicates that the adaptor is plug in*/
        U32 lowbat_det_out                 :1 ; /*4 :4 , battery voltage lower than uvlo threshold voltage set by uv_sel&lt;1:0&gt;. When it is ture, the system is not allowed to power on*/
        U32 bat_on_det_out                 :1 ; /*5 :5 , battery on detect out. 1:battery is on; 0: no battery.*/
        U32 reserved_0                     :1 ; /*6 :6 ,                               */
        U32 reserved_1                     :1 ; /*7 :7 ,                               */
        U32 reserved_2                     :1 ; /*8 :8 ,                               */
        U32 reserved_3                     :1 ; /*9 :9 ,                               */
        U32 reserved_4                     :1 ; /*10:10,                               */
        U32 reserved_5                     :1 ; /*11:11,                               */
        U32 reserved_6                     :1 ; /*12:12,                               */
        U32 reserved_7                     :1 ; /*13:13,                               */
        U32 reserved_8                     :1 ; /*14:14,                               */
        U32 reserved_9                     :1 ; /*15:15,                               */
        U32 reserved_10                    :1 ; /*16:16,                               */
        U32 reserved_11                    :1 ; /*17:17,                               */
        U32 reserved_12                    :1 ; /*18:18,                               */
        U32 reserved_13                    :1 ; /*19:19,                               */
        U32 reserved_14                    :1 ; /*20:20,                               */
        U32 reserved_15                    :1 ; /*21:21,                               */
        U32 reserved_16                    :1 ; /*22:22,                               */
        U32 reserved_17                    :1 ; /*23:23,                               */
        U32 reserved_18                    :1 ; /*24:24,                               */
        U32 reserved_19                    :1 ; /*25:25,                               */
        U32 reserved_20                    :1 ; /*26:26,                               */
        U32 reserved_21                    :1 ; /*27:27,                               */
        U32 reserved_22                    :1 ; /*28:28,                               */
        U32 reserved_23                    :1 ; /*29:29,                               */
        U32 reserved_24                    :1 ; /*30:30,                               */
        U32 reserved_25                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_ana_status;

//pmu_ana_rsvd
typedef union
{
    U32 v;
    struct
    {
        U32 pmu_ana_rsvd_out               :4 ; /*3 :0 , reserved for future pmu ana use
bit[3] is used as pwrkey_mode_en
bit[0] used as i2c isolation enable signal, low active*/
        U32 pmu_ana_rsvd_in                :2 ; /*5 :4 , reserved for future pmu ana use*/
        U32 reserved_0                     :1 ; /*6 :6 ,                               */
        U32 reserved_1                     :1 ; /*7 :7 ,                               */
        U32 reserved_2                     :1 ; /*8 :8 ,                               */
        U32 reserved_3                     :1 ; /*9 :9 ,                               */
        U32 reserved_4                     :1 ; /*10:10,                               */
        U32 reserved_5                     :1 ; /*11:11,                               */
        U32 reserved_6                     :1 ; /*12:12,                               */
        U32 reserved_7                     :1 ; /*13:13,                               */
        U32 reserved_8                     :1 ; /*14:14,                               */
        U32 reserved_9                     :1 ; /*15:15,                               */
        U32 reserved_10                    :1 ; /*16:16,                               */
        U32 reserved_11                    :1 ; /*17:17,                               */
        U32 reserved_12                    :1 ; /*18:18,                               */
        U32 reserved_13                    :1 ; /*19:19,                               */
        U32 reserved_14                    :1 ; /*20:20,                               */
        U32 reserved_15                    :1 ; /*21:21,                               */
        U32 reserved_16                    :1 ; /*22:22,                               */
        U32 reserved_17                    :1 ; /*23:23,                               */
        U32 reserved_18                    :1 ; /*24:24,                               */
        U32 reserved_19                    :1 ; /*25:25,                               */
        U32 reserved_20                    :1 ; /*26:26,                               */
        U32 reserved_21                    :1 ; /*27:27,                               */
        U32 reserved_22                    :1 ; /*28:28,                               */
        U32 reserved_23                    :1 ; /*29:29,                               */
        U32 reserved_24                    :1 ; /*30:30,                               */
        U32 reserved_25                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_pmu_ana_rsvd;

//sw_rsvd
typedef union
{
    U32 v;
    struct
    {
        U32 sw_rsvd                        :32; /*31:0 , reserved for future sw use    */
    } b;
} T_PMUC_sw_rsvd;

//switch_mode
typedef union
{
    U32 v;
    struct
    {
        U32 switch_mode                    :1 ; /*0 :0 , switch_mode indication
0: keysense
1: pwrkey*/
        U32 reserved_0                     :1 ; /*1 :1 ,                               */
        U32 reserved_1                     :1 ; /*2 :2 ,                               */
        U32 reserved_2                     :1 ; /*3 :3 ,                               */
        U32 reserved_3                     :1 ; /*4 :4 ,                               */
        U32 reserved_4                     :1 ; /*5 :5 ,                               */
        U32 reserved_5                     :1 ; /*6 :6 ,                               */
        U32 reserved_6                     :1 ; /*7 :7 ,                               */
        U32 reserved_7                     :1 ; /*8 :8 ,                               */
        U32 reserved_8                     :1 ; /*9 :9 ,                               */
        U32 reserved_9                     :1 ; /*10:10,                               */
        U32 reserved_10                    :1 ; /*11:11,                               */
        U32 reserved_11                    :1 ; /*12:12,                               */
        U32 reserved_12                    :1 ; /*13:13,                               */
        U32 reserved_13                    :1 ; /*14:14,                               */
        U32 reserved_14                    :1 ; /*15:15,                               */
        U32 reserved_15                    :1 ; /*16:16,                               */
        U32 reserved_16                    :1 ; /*17:17,                               */
        U32 reserved_17                    :1 ; /*18:18,                               */
        U32 reserved_18                    :1 ; /*19:19,                               */
        U32 reserved_19                    :1 ; /*20:20,                               */
        U32 reserved_20                    :1 ; /*21:21,                               */
        U32 reserved_21                    :1 ; /*22:22,                               */
        U32 reserved_22                    :1 ; /*23:23,                               */
        U32 reserved_23                    :1 ; /*24:24,                               */
        U32 reserved_24                    :1 ; /*25:25,                               */
        U32 reserved_25                    :1 ; /*26:26,                               */
        U32 reserved_26                    :1 ; /*27:27,                               */
        U32 reserved_27                    :1 ; /*28:28,                               */
        U32 reserved_28                    :1 ; /*29:29,                               */
        U32 reserved_29                    :1 ; /*30:30,                               */
        U32 reserved_30                    :1 ; /*31:31,                               */
    } b;
} T_PMUC_switch_mode;

typedef struct
{
    volatile                 T_PMUC_test_cfg                       test_cfg; /*0x0  , RW   , 0x00000004, No description.               */
    volatile             T_PMUC_pwr_sys_ctrl                   pwr_sys_ctrl; /*0x4  , RW   , 0x0001E022, No description.               */
    volatile            T_PMUC_pm_fsm_status                  pm_fsm_status; /*0x8  , RO   , 0x00000000, No description.               */
    volatile         T_PMUC_pm_fsm_frc_ctrl1               pm_fsm_frc_ctrl1; /*0xc  , RW   , 0x07000000, No description.               */
    volatile          T_PMUC_xtal_clk32k_div                xtal_clk32k_div; /*0x10 , RW   , 0x10200000, No description.               */
    volatile            T_PMUC_xtal_cfg_ctrl                  xtal_cfg_ctrl; /*0x14 , RW   , 0x058C0E71, No description.               */
    volatile                 T_PMUC_ldo_ctrl                       ldo_ctrl; /*0x18 , RW   , 0x00000492, No description.               */
    volatile              T_PMUC_bandgap_cfg                    bandgap_cfg; /*0x1c , RW   , 0x00000457, No description.               */
    volatile             T_PMUC_buck_cp_ctrl                   buck_cp_ctrl; /*0x20 , RW   , 0x00256A24, No description.               */
    volatile                  T_PMUC_ldo_cfg                        ldo_cfg; /*0x24 , RW   , 0x0005185C, No description.               */
    volatile         T_PMUC_charger_cfg_ctrl               charger_cfg_ctrl; /*0x28 , RW   , 0x30005101, No description.               */
    volatile               T_PMUC_ana_status                     ana_status; /*0x2c , RO   , 0x00000000, No description.               */
    volatile             T_PMUC_pmu_ana_rsvd                   pmu_ana_rsvd; /*0x30 , RW   , 0x00000003, No description.               */
    volatile                  T_PMUC_sw_rsvd                        sw_rsvd; /*0x34 , RW   , 0x000000FF, No description.               */
    volatile              T_PMUC_switch_mode                    switch_mode; /*0x38 , RW   , 0x00000000, No description.               */
} T_HWP_PMUC_T;
#else
typedef struct
{
    volatile                             U32                       test_cfg; /*0x0  , RW   , 0x00000004, No description.               */
    volatile                             U32                   pwr_sys_ctrl; /*0x4  , RW   , 0x0001E022, No description.               */
    volatile                             U32                  pm_fsm_status; /*0x8  , RO   , 0x00000000, No description.               */
    volatile                             U32               pm_fsm_frc_ctrl1; /*0xc  , RW   , 0x07000000, No description.               */
    volatile                             U32                xtal_clk32k_div; /*0x10 , RW   , 0x10200000, No description.               */
    volatile                             U32                  xtal_cfg_ctrl; /*0x14 , RW   , 0x058C0E71, No description.               */
    volatile                             U32                       ldo_ctrl; /*0x18 , RW   , 0x00000492, No description.               */
    volatile                             U32                    bandgap_cfg; /*0x1c , RW   , 0x00000457, No description.               */
    volatile                             U32                   buck_cp_ctrl; /*0x20 , RW   , 0x00256A24, No description.               */
    volatile                             U32                        ldo_cfg; /*0x24 , RW   , 0x0005185C, No description.               */
    volatile                             U32               charger_cfg_ctrl; /*0x28 , RW   , 0x30005101, No description.               */
    volatile                             U32                     ana_status; /*0x2c , RO   , 0x00000000, No description.               */
    volatile                             U32                   pmu_ana_rsvd; /*0x30 , RW   , 0x00000003, No description.               */
    volatile                             U32                        sw_rsvd; /*0x34 , RW   , 0x000000FF, No description.               */
    volatile                             U32                    switch_mode; /*0x38 , RW   , 0x00000000, No description.               */
} T_HWP_PMUC_T;

//test_cfg
#define PMUC_SCAN_MODE0                (1<<0)              

#define PMUC_SCAN_MODE1                (1<<1)              

#define PMUC_SCAN_RSTB                 (1<<2)              

#define PMUC_BIST_MODE0                (1<<3)              

#define PMUC_BIST_MODE1                (1<<4)              

#define PMUC_FUNC_TEST_MODE            (1<<5)              

#define PMUC_EDT_BYPASS                (1<<6)              

#define PMUC_OCC_LF_DC_MODE            (1<<7)              

#define PMUC_OCC_HF_DC_MODE            (1<<8)              

#define PMUC_SPIFLASH_TEST_MODE        (1<<9)              

//pwr_sys_ctrl
#define PMUC_KEYSENSE_ENB              (1<<0)              

#define PMUC_SELF_RESETN               (1<<1)              

#define PMUC_PM_REG(n)                 (((n)&0x7)<<2)      
#define PMUC_PM_REG_MASK               (0x7<<2)            
#define PMUC_PM_REG_SHIFT              (2)                 

#define PMUC_LP_MODE_B                 (1<<5)              

#define PMUC_KEY_THLD(n)               (((n)&0x3)<<6)      
#define PMUC_KEY_THLD_MASK             (0x3<<6)            
#define PMUC_KEY_THLD_SHIFT            (6)                 

#define PMUC_CLK_FINE_SEL              (1<<8)              

#define PMUC_CLK_RAW_SEL               (1<<9)              

#define PMUC_CLK_PMUC_SEL              (1<<10)             

#define PMUC_CHR_AC_ON_RESET_BYPASS    (1<<11)             

#define PMUC_LOWBAT_OFF_BYPASS         (1<<12)             

#define PMUC_XTAL_RDY_TIME_SEL(n)      (((n)&0x7)<<13)     
#define PMUC_XTAL_RDY_TIME_SEL_MASK    (0x7<<13)           
#define PMUC_XTAL_RDY_TIME_SEL_SHIFT   (13)                

#define PMUC_CHR_AC_ON_TIME_SEL(n)     (((n)&0x3)<<16)     
#define PMUC_CHR_AC_ON_TIME_SEL_MASK   (0x3<<16)           
#define PMUC_CHR_AC_ON_TIME_SEL_SHIFT  (16)                

#define PMUC_I2CS_DISABLE              (1<<18)             

#define PMUC_HST_CLK_DISABLE           (1<<19)             

#define PMUC_TAB_RST_BYPASS            (1<<20)             


//pm_fsm_status
#define PMUC_PM_FSM_STATE(n)           (((n)&0x7F)<<0)     
#define PMUC_PM_FSM_STATE_MASK         (0x7F<<0)           
#define PMUC_PM_FSM_STATE_SHIFT        (0)                 

#define PMUC_PWRSWITCH_EN              (1<<7)              

#define PMUC_PU_BG_PMU                 (1<<8)              

#define PMUC_PU_XTAL_26M_LV            (1<<9)              

#define PMUC_PU_MEM_GRP1               (1<<10)             

#define PMUC_PU_MEM_GRP2               (1<<11)             

#define PMUC_PU_MEM_GRP3               (1<<12)             

#define PMUC_PU_BUCK_LDO               (1<<13)             

#define PMUC_PU_LDO_VCORE              (1<<14)             

#define PMUC_RSTB_DBB                  (1<<15)             

#define PMUC_VRTC_OUT_SEL              (1<<16)             

#define PMUC_PWRSWITCH_EN_DLY          (1<<17)             

#define PMUC_PU_BT_AON                 (1<<18)             


//pm_fsm_frc_ctrl1
#define PMUC_PWRSWITCH_EN_REG          (1<<0)              

#define PMUC_PWRSWITCH_EN_DR           (1<<1)              

#define PMUC_PU_BG_REG                 (1<<2)              

#define PMUC_PU_BG_DR                  (1<<3)              

#define PMUC_PU_26M_REG                (1<<4)              

#define PMUC_PU_26M_DR                 (1<<5)              

#define PMUC_PU_MEM_GRP1_REG           (1<<6)              

#define PMUC_PU_MEM_GRP1_DR            (1<<7)              

#define PMUC_PU_MEM_GRP2_REG           (1<<8)              

#define PMUC_PU_MEM_GRP2_DR            (1<<9)              

#define PMUC_PU_MEM_GRP3_REG           (1<<10)             

#define PMUC_PU_MEM_GRP3_DR            (1<<11)             

#define PMUC_PU_BUCK_LDO_REG           (1<<12)             

#define PMUC_PU_BUCK_LDO_DR            (1<<13)             

#define PMUC_PU_LDO_VCORE_REG          (1<<14)             

#define PMUC_PU_LDO_VCORE_DR           (1<<15)             

#define PMUC_RSTB_DBB_REG              (1<<16)             

#define PMUC_RSTB_DBB_DR               (1<<17)             

#define PMUC_VRTC_OUT_SEL_REG          (1<<18)             

#define PMUC_VRTC_OUT_SEL_DR           (1<<19)             

#define PMUC_PWRSWITCH_EN_DLY_REG      (1<<20)             

#define PMUC_PWRSWITCH_EN_DLY_DR       (1<<21)             

#define PMUC_PU_BT_AON_REG             (1<<22)             

#define PMUC_PU_BT_AON_DR              (1<<23)             

#define PMUC_MEM_GRP1_ON               (1<<24)             

#define PMUC_MEM_GRP2_ON               (1<<25)             

#define PMUC_MEM_GRP3_ON               (1<<26)             

//xtal_clk32k_div
#define PMUC_STEP_OFFSET_LP(n)         (((n)&0xFF)<<0)     
#define PMUC_STEP_OFFSET_LP_MASK       (0xFF<<0)           
#define PMUC_STEP_OFFSET_LP_SHIFT      (0)                 

#define PMUC_STEP_OFFSET_NORMAL(n)     (((n)&0xFF)<<8)     
#define PMUC_STEP_OFFSET_NORMAL_MASK   (0xFF<<8)           
#define PMUC_STEP_OFFSET_NORMAL_SHIFT  (8)                 

#define PMUC_STEP_OFFSET_UPDATE        (1<<16)             

#define PMUC_DIV_LP_MODE_H             (1<<17)             

#define PMUC_DIV_LP_MODE_H_REG         (1<<18)             

#define PMUC_DIV_LP_MODE_H_DR          (1<<19)             

#define PMUC_XTAL_VAMP_IBIT_XOSC_ACTIVE(n) (((n)&0xF)<<20)     
#define PMUC_XTAL_VAMP_IBIT_XOSC_ACTIVE_MASK (0xF<<20)           
#define PMUC_XTAL_VAMP_IBIT_XOSC_ACTIVE_SHIFT (20)                

#define PMUC_XTAL_FIX_IBIT_XOSC_ACTIVE(n) (((n)&0x1F)<<24)    
#define PMUC_XTAL_FIX_IBIT_XOSC_ACTIVE_MASK (0x1F<<24)          
#define PMUC_XTAL_FIX_IBIT_XOSC_ACTIVE_SHIFT (24)                

//xtal_cfg_ctrl
#define PMUC_XTAL_VAMP_IBIT_XOSC_LP(n) (((n)&0xF)<<0)      
#define PMUC_XTAL_VAMP_IBIT_XOSC_LP_MASK (0xF<<0)            
#define PMUC_XTAL_VAMP_IBIT_XOSC_LP_SHIFT (0)                 

#define PMUC_XTAL_REG_BIT_LV(n)        (((n)&0xF)<<4)      
#define PMUC_XTAL_REG_BIT_LV_MASK      (0xF<<4)            
#define PMUC_XTAL_REG_BIT_LV_SHIFT     (4)                 

#define PMUC_XTAL_CAPBANK_BIT_LV(n)    (((n)&0xF)<<8)      
#define PMUC_XTAL_CAPBANK_BIT_LV_MASK  (0xF<<8)            
#define PMUC_XTAL_CAPBANK_BIT_LV_SHIFT (8)                 

#define PMUC_XTAL_CAP_FINE_LV(n)       (((n)&0x3F)<<12)    
#define PMUC_XTAL_CAP_FINE_LV_MASK     (0x3F<<12)          
#define PMUC_XTAL_CAP_FINE_LV_SHIFT    (12)                

#define PMUC_XTAL_FIX_IBIT_XOSC_LP(n)  (((n)&0x1F)<<18)    
#define PMUC_XTAL_FIX_IBIT_XOSC_LP_MASK (0x1F<<18)          
#define PMUC_XTAL_FIX_IBIT_XOSC_LP_SHIFT (18)                

#define PMUC_XTAL_CLK_LP_EN_LV         (1<<23)             

#define PMUC_PU_XTAL_26M_REG           (1<<24)             

#define PMUC_PU_XTAL_26M_DR            (1<<25)             

#define PMUC_XTAL_LP_MODE_EN_REG       (1<<26)             

#define PMUC_XTAL_LP_MODE_EN_DR        (1<<27)             



//ldo_ctrl
#define PMUC_LDO_ULP_VPAD_VBIT(n)      (((n)&0x7)<<0)      
#define PMUC_LDO_ULP_VPAD_VBIT_MASK    (0x7<<0)            
#define PMUC_LDO_ULP_VPAD_VBIT_SHIFT   (0)                 

#define PMUC_LDO_ULP_VRTC_VBIT(n)      (((n)&0x3)<<3)      
#define PMUC_LDO_ULP_VRTC_VBIT_MASK    (0x3<<3)            
#define PMUC_LDO_ULP_VRTC_VBIT_SHIFT   (3)                 

#define PMUC_VBUCK_LDO_BIT(n)          (((n)&0x7)<<5)      
#define PMUC_VBUCK_LDO_BIT_MASK        (0x7<<5)            
#define PMUC_VBUCK_LDO_BIT_SHIFT       (5)                 

#define PMUC_LDO_VCORE_VBIT(n)         (((n)&0x7)<<8)      
#define PMUC_LDO_VCORE_VBIT_MASK       (0x7<<8)            
#define PMUC_LDO_VCORE_VBIT_SHIFT      (8)                 

#define PMUC_VOUT_SEL_BUCK_LDO         (1<<11)             

#define PMUC_LDO_VCORE_DISCHARGE_EN    (1<<12)             

//bandgap_cfg
#define PMUC_BG_CAL_TC_BIT(n)          (((n)&0x1F)<<0)     
#define PMUC_BG_CAL_TC_BIT_MASK        (0x1F<<0)           
#define PMUC_BG_CAL_TC_BIT_SHIFT       (0)                 

#define PMUC_BG_FREQ_BIT(n)            (((n)&0x3)<<5)      
#define PMUC_BG_FREQ_BIT_750KHZ       (((0)&0x3)<<5)      
#define PMUC_BG_FREQ_BIT_1P1MHZ       (((1)&0x3)<<5)      
#define PMUC_BG_FREQ_BIT_1P5MHZ        (((2)&0x3)<<5)      
#define PMUC_BG_FREQ_BIT_MASK          (0x3<<5)            
#define PMUC_BG_FREQ_BIT_SHIFT         (5)                 

#define PMUC_BG_TS_I_BIT(n)            (((n)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_0UA           (((0)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_10UA          (((1)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_20UA         (((2)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_20UA_         (((3)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_30UA         (((4)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_30UA_        (((5)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_40UA         (((6)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_50UA         (((7)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_50UA_        (((8)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_60UA         (((9)&0xF)<<7)      
#define PMUC_BG_TS_I_BIT_70UA         (((10)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_70UA_        (((11)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_80UA         (((12)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_80UA_        (((13)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_90UA         (((14)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_100UA         (((15)&0xF)<<7)     
#define PMUC_BG_TS_I_BIT_MASK          (0xF<<7)            
#define PMUC_BG_TS_I_BIT_SHIFT         (7)                 

#define PMUC_PU_TS_BG                  (1<<11)             

#define PMUC_LP_MODE_BG                (1<<12)             
#define PMUC_LP_MODE_BG_LOW_POWER_MODE (1<<12)             
#define PMUC_LP_MODE_BG_NORMAL_MODE    (1<<12)             

#define PMUC_BG_CHOPPER_ENB            (1<<13)             
#define PMUC_BG_CHOPPER_ENB_DISABLE    (1<<13)             
#define PMUC_BG_CHOPPER_ENB_ENABLE     (1<<13)             

#define PMUC_REF_SEL_BG                (1<<14)             


//buck_cp_ctrl
#define PMUC_PU_BUCK                   (1<<0)              

#define PMUC_PFM_MODE_SEL_BUCK         (1<<1)              
#define PMUC_PFM_MODE_SEL_BUCK_PFM     (1<<1)              
#define PMUC_PFM_MODE_SEL_BUCK_FORCE   (0<<1)              

#define PMUC_VBUCK_BIT(n)              (((n)&0xF)<<2)      
#define PMUC_VBUCK_BIT_MASK            (0xF<<2)            
#define PMUC_VBUCK_BIT_SHIFT           (2)                 

#define PMUC_DISCHARGE_EN_BUCK         (1<<6)              

#define PMUC_HEAVY_LOAD_BUCK           (1<<7)              

#define PMUC_ANTIRING_DISABLE_BUCK     (1<<8)              

#define PMUC_LOW_SENSE_BUCK            (1<<9)              

#define PMUC_PFM_BUCK_THRESHOLD(n)     (((n)&0x3)<<10)     
#define PMUC_PFM_BUCK_THRESHOLD_MASK   (0x3<<10)           
#define PMUC_PFM_BUCK_THRESHOLD_SHIFT  (10)                

#define PMUC_OSC_FREQ_BUCK(n)          (((n)&0x3)<<12)     
#define PMUC_OSC_FREQ_BUCK_MASK        (0x3<<12)           
#define PMUC_OSC_FREQ_BUCK_SHIFT       (12)                

#define PMUC_COUNTER_DISABLE_BUCK      (1<<14)             

#define PMUC_HYS_VBP_LOWBAT_BUCK       (1<<15)             

#define PMUC_PFM_CLK_DISABLE_BUCK      (1<<16)             

#define PMUC_NONOV_DELAY_BUCK(n)       (((n)&0x3)<<17)     
#define PMUC_NONOV_DELAY_BUCK_MASK     (0x3<<17)           
#define PMUC_NONOV_DELAY_BUCK_SHIFT    (17)                

#define PMUC_BUCK_LDO_VREF_SEL         (1<<19)             

#define PMUC_PFM_VREF_ONLY_BUCK        (1<<20)             

#define PMUC_CHARGE_PUMP_LDO_EN        (1<<21)             

//ldo_cfg
#define PMUC_VUSB_VBIT(n)              (((n)&0x7)<<0)      
#define PMUC_VUSB_VBIT_MASK            (0x7<<0)            
#define PMUC_VUSB_VBIT_SHIFT           (0)                 

#define PMUC_VUSB_VSEL(n)              (((n)&0x3)<<3)      
#define PMUC_VUSB_VSEL_MASK            (0x3<<3)            
#define PMUC_VUSB_VSEL_SHIFT           (3)                 

#define PMUC_VUSB_EXCAP_EN             (1<<5)              

#define PMUC_PU_VUSB                   (1<<6)              

#define PMUC_VIO_VSEL(n)               (((n)&0x3)<<7)      
#define PMUC_VIO_VSEL_2P8V             (((1)&0x3)<<7)      
#define PMUC_VIO_VSEL_3P3V             (((0)&0x3)<<7)      
#define PMUC_VIO_VSEL_MASK             (0x3<<7)            
#define PMUC_VIO_VSEL_SHIFT            (7)                 

#define PMUC_VIO_VBIT(n)               (((n)&0x7)<<9)      
#define PMUC_VIO_VBIT_MASK             (0x7<<9)            
#define PMUC_VIO_VBIT_SHIFT            (9)                 

#define PMUC_PU_VIO                    (1<<12)             
#define PMUC_PU_VIO_POWER_DOWN_MODE    (0<<12)             
#define PMUC_PU_VIO_NORMAL_MODE        (1<<12)             

#define PMUC_VSEL_VA                   (1<<13)             
#define PMUC_VSEL_VA_2P4V              (1<<13)             
#define PMUC_VSEL_VA_2P8V              (0<<13)             

#define PMUC_VA_VBIT(n)                (((n)&0x7)<<14)     
#define PMUC_VA_VBIT_MASK              (0x7<<14)           
#define PMUC_VA_VBIT_SHIFT             (14)                

#define PMUC_VA_CC_DISABLE             (1<<17)             

#define PMUC_PU_VA_NORMAL              (1<<18)             
#define PMUC_PU_VA_NORMAL_POWER_DOWN_MODE (0<<18)             
#define PMUC_PU_VA_NORMAL_NORMAL_MODE  (1<<18)             

#define PMUC_PU_VA_LP                  (1<<19)             

#define PMUC_LV_DETECT_PMU             (1<<20)             

#define PMUC_VIO_EXCAP_EN              (1<<21)             

//charger_cfg_ctrl
#define PMUC_PRE_CHR_CHECK_BYPASS      (1<<0)              

#define PMUC_CHR_VFB_SEL(n)            (((n)&0xF)<<1)      
#define PMUC_CHR_VFB_SEL_MASK          (0xF<<1)            
#define PMUC_CHR_VFB_SEL_SHIFT         (1)                 

#define PMUC_CHR_VFB_SEL_REG(n)        (((n)&0xF)<<5)      
#define PMUC_CHR_VFB_SEL_REG_MASK      (0xF<<5)            
#define PMUC_CHR_VFB_SEL_REG_SHIFT     (5)                 

#define PMUC_CHR_VFB_SEL_DR            (1<<9)              

#define PMUC_CHR_PRE_I_BIT(n)          (((n)&0x7)<<10)     
#define PMUC_CHR_PRE_I_BIT_MASK        (0x7<<10)           
#define PMUC_CHR_PRE_I_BIT_SHIFT       (10)                

#define PMUC_CHR_CC_I_BIT(n)           (((n)&0x7)<<13)     
#define PMUC_CHR_CC_I_BIT_MASK         (0x7<<13)           
#define PMUC_CHR_CC_I_BIT_SHIFT        (13)                

#define PMUC_CHR_CC_MODE               (1<<16)             
#define PMUC_CHR_CC_MODE_CONSTANT_CURRENT_MODE (1<<16)             
#define PMUC_CHR_CC_MODE_PRE_CHARGE_MODE (0<<16)             

#define PMUC_CHR_CC_MODE_REG           (1<<17)             

#define PMUC_CHR_CC_MODE_DR            (1<<18)             

#define PMUC_CHR_CV_LOWGAIN            (1<<19)             
#define PMUC_CHR_CV_LOWGAIN_LOW_GAIN_MODE (1<<19)             
#define PMUC_CHR_CV_LOWGAIN_NORMAL_GAIN_MODE (0<<19)             

#define PMUC_UV_SEL(n)                 (((n)&0x3)<<20)     
#define PMUC_UV_SEL_321_257            (((0)&0x3)<<20)     
#define PMUC_UV_SEL_321_273            (((1)&0x3)<<20)     
#define PMUC_UV_SEL_321_288            (((2)&0x3)<<20)     
#define PMUC_UV_SEL_MASK               (0x3<<20)           
#define PMUC_UV_SEL_SHIFT              (20)                

#define PMUC_UV_SEL_REG(n)             (((n)&0x3)<<22)     
#define PMUC_UV_SEL_REG_MASK           (0x3<<22)           
#define PMUC_UV_SEL_REG_SHIFT          (22)                

#define PMUC_UV_SEL_DR                 (1<<24)             

#define PMUC_CHR_AUXADC_SEL(n)         (((n)&0x3)<<25)     
#define PMUC_CHR_AUXADC_SEL_TS         (((0)&0x3)<<25)     
#define PMUC_CHR_AUXADC_SEL_AC_DIV_IN  (((1)&0x3)<<25)     
#define PMUC_CHR_AUXADC_SEL_NC         (((2)&0x3)<<25)     
#define PMUC_CHR_AUXADC_SEL_MASK       (0x3<<25)           
#define PMUC_CHR_AUXADC_SEL_SHIFT      (25)                

#define PMUC_CHR_ENABLE                (1<<27)             
#define PMUC_CHR_ENABLE_ENABLED        (1<<27)             
#define PMUC_CHR_ENABLE_DISABLED       (0<<27)             

#define PMUC_CHR_ENABLE_REG            (1<<28)             

#define PMUC_CHR_ENABLE_DR             (1<<29)             

#define PMUC_CHR_AC_UVLO               (1<<30)             

//ana_status
#define PMUC_CHR_CREG                  (1<<0)              

#define PMUC_CHR_VREG                  (1<<1)              

#define PMUC_CHR_TERM                  (1<<2)              

#define PMUC_CHR_AC_ON                 (1<<3)              

#define PMUC_LOWBAT_DET_OUT            (1<<4)              

#define PMUC_BAT_ON_DET_OUT            (1<<5)              

////pmu_ana_rsvd
#define PMUC_PMU_ANA_RSVD_OUT(n)       (((n)&0xF)<<0)      
#define PMUC_PMU_ANA_RSVD_OUT_MASK     (0xF<<0)            
#define PMUC_PMU_ANA_RSVD_OUT_SHIFT    (0)                 

#define PMUC_PMU_ANA_RSVD_IN(n)        (((n)&0x3)<<4)      
#define PMUC_PMU_ANA_RSVD_IN_MASK      (0x3<<4)            
#define PMUC_PMU_ANA_RSVD_IN_SHIFT     (4)                 


//sw_rsvd
#define PMUC_SW_RSVD(n)                (((n)&0xFFFFFFFF)<<0)
#define PMUC_SW_RSVD_MASK              (0xFFFFFFFF<<0)     
#define PMUC_SW_RSVD_SHIFT             (0)                 

//switch_mode
#define PMUC_SWITCH_MODE               (1<<0)              
#define PMUC_SWITCH_MODE_KEYSENSE      (1<<0)              
#define PMUC_SWITCH_MODE_PWRKEY        (1<<0)              


#endif

#endif


