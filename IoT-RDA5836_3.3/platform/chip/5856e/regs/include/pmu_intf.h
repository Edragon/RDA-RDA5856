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
// File name      : pmu_intf                                            
// Author         : liqiu                                                 
// Date of created: 2016-11-01 11:06:10                                                  
// Last modified  : 2016-11-01 11:06:10                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-11-01 11:06:10 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __PMU_INTF_H__
#define __PMU_INTF_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//efuse_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 efuse_out                      :8 ; /*7 :0 , No description.               */
        U32 efuse_sense_en                 :1 ; /*8 :8 , No description.               */
        U32 efuse_sense_en_reg             :1 ; /*9 :9 , No description.               */
        U32 efuse_sense_en_dr              :1 ; /*10:10, No description.               */
        U32 efuse_sense                    :1 ; /*11:11, No description.               */
        U32 efuse_sense_reg                :1 ; /*12:12, No description.               */
        U32 efuse_sense_dr                 :1 ; /*13:13, No description.               */
        U32 efuse_sel_word_reg             :4 ; /*17:14, efuse word select             */
        U32 efuse_sel_word_dr              :1 ; /*18:18, efuse word select             */
        U32 efuse_sel                      :8 ; /*26:19, efuse bit select              */
        U32 efuse_ref_res_bit              :2 ; /*28:27, referece res select bit       */
        U32 resetn_efuse_reg               :1 ; /*29:29, reset efuse control logic     */
        U32 efuse_rd_done                  :1 ; /*30:30, 1:power up efuse reading complete*/
        U32 reserved_0                     :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_efuse_ctrl1;

//efuse_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 efuse_sel_word_auto_0          :4 ; /*3 :0 , No description.               */
        U32 efuse_sel_word_auto_1          :4 ; /*7 :4 , No description.               */
        U32 efuse_sel_word_auto_2          :4 ; /*11:8 , No description.               */
        U32 efuse_sel_word_auto_3          :4 ; /*15:12, No description.               */
        U32 reg_prog_efuse_en              :16; /*31:16, write babe to prog efuse      */
    } b;
} T_PMU_INTF_efuse_ctrl2;

//efuse_readout
typedef union
{
    U32 v;
    struct
    {
        U32 efuse_readout_0                :8 ; /*7 :0 , No description.               */
        U32 efuse_readout_1                :8 ; /*15:8 , No description.               */
        U32 efuse_readout_2                :8 ; /*23:16, No description.               */
        U32 efuse_readout_3                :8 ; /*31:24, No description.               */
    } b;
} T_PMU_INTF_efuse_readout;

//misc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pu_xtal_32k                    :1 ; /*0 :0 , No description.               */
        U32 vbat_monitor_en                :1 ; /*1 :1 , No description.               */
        U32 reserved_0                     :1 ; /*2 :2 ,                               */
        U32 reserved_1                     :1 ; /*3 :3 ,                               */
        U32 reserved_2                     :1 ; /*4 :4 ,                               */
        U32 reserved_3                     :1 ; /*5 :5 ,                               */
        U32 reserved_4                     :1 ; /*6 :6 ,                               */
        U32 reserved_5                     :1 ; /*7 :7 ,                               */
        U32 reserved_6                     :1 ; /*8 :8 ,                               */
        U32 reserved_7                     :1 ; /*9 :9 ,                               */
        U32 reserved_8                     :1 ; /*10:10,                               */
        U32 reserved_9                     :1 ; /*11:11,                               */
        U32 reserved_10                    :1 ; /*12:12,                               */
        U32 reserved_11                    :1 ; /*13:13,                               */
        U32 reserved_12                    :1 ; /*14:14,                               */
        U32 reserved_13                    :1 ; /*15:15,                               */
        U32 reserved_14                    :1 ; /*16:16,                               */
        U32 reserved_15                    :1 ; /*17:17,                               */
        U32 reserved_16                    :1 ; /*18:18,                               */
        U32 reserved_17                    :1 ; /*19:19,                               */
        U32 reserved_18                    :1 ; /*20:20,                               */
        U32 reserved_19                    :1 ; /*21:21,                               */
        U32 reserved_20                    :1 ; /*22:22,                               */
        U32 reserved_21                    :1 ; /*23:23,                               */
        U32 reserved_22                    :1 ; /*24:24,                               */
        U32 reserved_23                    :1 ; /*25:25,                               */
        U32 reserved_24                    :1 ; /*26:26,                               */
        U32 reserved_25                    :1 ; /*27:27,                               */
        U32 reserved_26                    :1 ; /*28:28,                               */
        U32 reserved_27                    :1 ; /*29:29,                               */
        U32 reserved_28                    :1 ; /*30:30,                               */
        U32 reserved_29                    :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_misc_ctrl;

//buck_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 sync_clock_polarity            :1 ; /*0 :0 , No description.               */
        U32 buck_clk_cfg_ld                :1 ; /*1 :1 , No description.               */
        U32 pu_buck_clk_reg                :1 ; /*2 :2 , No description.               */
        U32 pu_buck_clk_dr                 :1 ; /*3 :3 , No description.               */
        U32 reg_sync_clk_num               :2 ; /*5 :4 , No description.               */
        U32 reg_sync_clk_denom             :5 ; /*10:6 , No description.               */
        U32 pfm_mode_clk_prd_buck          :2 ; /*12:11, No description.               */
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
} T_PMU_INTF_buck_clk_cfg;

//gpadc_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 resetn_gpadc_reg               :1 ; /*0 :0 , No description.               */
        U32 resetn_gpadc_dr                :1 ; /*1 :1 , No description.               */
        U32 gpadc_start_reg                :1 ; /*2 :2 , No description.               */
        U32 gpadc_start_dr                 :1 ; /*3 :3 , No description.               */
        U32 gpadc_start                    :1 ; /*4 :4 , No description.               */
        U32 resetn_gpadc                   :1 ; /*5 :5 , No description.               */
        U32 time_samp_neg_gpadc            :1 ; /*6 :6 , No description.               */
        U32 time_samp_pos_gpadc            :1 ; /*7 :7 , No description.               */
        U32 timer_sar_stable_sel           :3 ; /*10:8 , No description.               */
        U32 delay_before_samp_gpadc        :2 ; /*12:11, No description.               */
        U32 sar_out_polarity_gpadc         :1 ; /*13:13, No description.               */
        U32 sar_adc_mode_gpadc             :1 ; /*14:14, No description.               */
        U32 conv_clk_inv_gpadc             :1 ; /*15:15, No description.               */
        U32 samp_clk_inv_gpadc             :1 ; /*16:16, No description.               */
        U32 gpadc_eoc_phase_sel            :1 ; /*17:17, No description.               */
        U32 gpadc_channel_en_reg           :8 ; /*25:18, No description.               */
        U32 gpadc_channel_en_dr            :1 ; /*26:26, No description.               */
        U32 gpadc_enable                   :1 ; /*27:27, No description.               */
        U32 gpadc_clk_div_en               :1 ; /*28:28, No description.               */
        U32 gpadc_start_time_sel           :3 ; /*31:29, No description.               */
    } b;
} T_PMU_INTF_gpadc_ctrl1;

//gpadc_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 gpadc_vref_ms_enable           :1 ; /*0 :0 , No description.               */
        U32 gpadc_vref_bit                 :2 ; /*2 :1 , No description.               */
        U32 gpadc_channel_en               :8 ; /*10:3 , No description.               */
        U32 pu_gpadc                       :1 ; /*11:11, No description.               */
        U32 pu_gpadc_reg                   :1 ; /*12:12, No description.               */
        U32 pu_gpadc_dr                    :1 ; /*13:13, No description.               */
        U32 gpadc_channel_sel              :8 ; /*21:14, No description.               */
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
} T_PMU_INTF_gpadc_ctrl2;

//keysense_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 key_pullup2x                   :1 ; /*0 :0 , No description.               */
        U32 key_en                         :1 ; /*1 :1 , No description.               */
        U32 key_en_reg                     :1 ; /*2 :2 , No description.               */
        U32 key_en_dr                      :1 ; /*3 :3 , No description.               */
        U32 key_int_en                     :1 ; /*4 :4 , No description.               */
        U32 key_int_en_reg                 :1 ; /*5 :5 , No description.               */
        U32 key_int_en_dr                  :1 ; /*6 :6 , No description.               */
        U32 key_enable                     :1 ; /*7 :7 , No description.               */
        U32 time_samp_neg_tsc              :1 ; /*8 :8 , No description.               */
        U32 time_samp_pos_tsc              :1 ; /*9 :9 , No description.               */
        U32 timer_x_to_y_sel               :2 ; /*11:10, No description.               */
        U32 timer_int_to_x_sel             :3 ; /*14:12, No description.               */
        U32 timer_penirq_stable            :3 ; /*17:15, No description.               */
        U32 delay_before_samp_tsc          :2 ; /*19:18, No description.               */
        U32 bypass_bb_read                 :1 ; /*20:20, No description.               */
        U32 conv_clk_inv                   :1 ; /*21:21, No description.               */
        U32 samp_clk_inv                   :1 ; /*22:22, No description.               */
        U32 sar_adc_mode                   :1 ; /*23:23, No description.               */
        U32 sar_out_polarity               :1 ; /*24:24, No description.               */
        U32 wait_bb_read_timesel           :2 ; /*26:25, No description.               */
        U32 key_clk_div_en                 :1 ; /*27:27, No description.               */
        U32 reserved_0                     :1 ; /*28:28,                               */
        U32 reserved_1                     :1 ; /*29:29,                               */
        U32 reserved_2                     :1 ; /*30:30,                               */
        U32 reserved_3                     :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_keysense_ctrl1;

//xtal_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 xtal_clk_bb_en_lv              :1 ; /*0 :0 , No description.               */
        U32 xtal_clk_rsvd_en_lv            :1 ; /*1 :1 , No description.               */
        U32 xtal_clk_aupll_en_lv           :1 ; /*2 :2 , No description.               */
        U32 xtal_clk_adpll_en_lv           :1 ; /*3 :3 , No description.               */
        U32 xtal_clk_adc_en_lv             :1 ; /*4 :4 , No description.               */
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 reserved_3                     :1 ; /*8 :8 ,                               */
        U32 reserved_4                     :1 ; /*9 :9 ,                               */
        U32 reserved_5                     :1 ; /*10:10,                               */
        U32 reserved_6                     :1 ; /*11:11,                               */
        U32 reserved_7                     :1 ; /*12:12,                               */
        U32 reserved_8                     :1 ; /*13:13,                               */
        U32 reserved_9                     :1 ; /*14:14,                               */
        U32 reserved_10                    :1 ; /*15:15,                               */
        U32 reserved_11                    :1 ; /*16:16,                               */
        U32 reserved_12                    :1 ; /*17:17,                               */
        U32 reserved_13                    :1 ; /*18:18,                               */
        U32 reserved_14                    :1 ; /*19:19,                               */
        U32 reserved_15                    :1 ; /*20:20,                               */
        U32 reserved_16                    :1 ; /*21:21,                               */
        U32 reserved_17                    :1 ; /*22:22,                               */
        U32 reserved_18                    :1 ; /*23:23,                               */
        U32 reserved_19                    :1 ; /*24:24,                               */
        U32 reserved_20                    :1 ; /*25:25,                               */
        U32 reserved_21                    :1 ; /*26:26,                               */
        U32 reserved_22                    :1 ; /*27:27,                               */
        U32 reserved_23                    :1 ; /*28:28,                               */
        U32 reserved_24                    :1 ; /*29:29,                               */
        U32 reserved_25                    :1 ; /*30:30,                               */
        U32 reserved_26                    :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_xtal_ctrl;

//gpadc_key_value
typedef union
{
    U32 v;
    struct
    {
        U32 key_value_valid                :1 ; /*0 :0 , No description.               */
        U32 key_value                      :10; /*10:1 , No description.               */
        U32 gpadc_channel_num              :3 ; /*13:11, No description.               */
        U32 gpadc_value_valid              :1 ; /*14:14, No description.               */
        U32 gpadc_value                    :10; /*24:15, No description.               */
        U32 reserved_0                     :1 ; /*25:25,                               */
        U32 reserved_1                     :1 ; /*26:26,                               */
        U32 reserved_2                     :1 ; /*27:27,                               */
        U32 reserved_3                     :1 ; /*28:28,                               */
        U32 reserved_4                     :1 ; /*29:29,                               */
        U32 reserved_5                     :1 ; /*30:30,                               */
        U32 reserved_6                     :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_gpadc_key_value;

//boost_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 pu_boost                       :1 ; /*0 :0 , en boost                      */
        U32 dutysel_boost                  :1 ; /*1 :1 , max-duty mode select_x000D_
1'b0: default, auto control_x000D_
1'b1: by register*/
        U32 duty_bit_boost                 :3 ; /*4 :2 , max-duty select_x000D_
3'b000: default*/
        U32 curlimit_sel_boost             :3 ; /*7 :5 , current limit_x000D_
3'b000: default, ~500mA*/
        U32 boost_vbit                     :4 ; /*11:8 , output voltage select_x000D_
4'b0000: default, 4.6V*/
        U32 reg_rsvd_boost                 :4 ; /*15:12, reserved bit_x000D_
4'b0000   */
        U32 reg_clk_freq_sel_boost         :3 ; /*18:16, No description.               */
        U32 reg_clk_duty_sel_boost         :3 ; /*21:19, No description.               */
        U32 boost_clk_enable               :1 ; /*22:22, No description.               */
        U32 reserved_0                     :1 ; /*23:23,                               */
        U32 reserved_1                     :1 ; /*24:24,                               */
        U32 reserved_2                     :1 ; /*25:25,                               */
        U32 reserved_3                     :1 ; /*26:26,                               */
        U32 reserved_4                     :1 ; /*27:27,                               */
        U32 reserved_5                     :1 ; /*28:28,                               */
        U32 reserved_6                     :1 ; /*29:29,                               */
        U32 reserved_7                     :1 ; /*30:30,                               */
        U32 reserved_8                     :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_boost_ctrl;

//pmu_intf_icr
typedef union
{
    U32 v;
    struct
    {
        U32 key_int_clear                  :1 ; /*0 :0 , No description.               */
        U32 key_measure_int_clear          :1 ; /*1 :1 , No description.               */
        U32 gpadc_int_clear                :1 ; /*2 :2 , No description.               */
        U32 lowbat_int_clear               :1 ; /*3 :3 , No description.               */
        U32 ac_on_int_clear                :1 ; /*4 :4 , No description.               */
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 reserved_3                     :1 ; /*8 :8 ,                               */
        U32 reserved_4                     :1 ; /*9 :9 ,                               */
        U32 reserved_5                     :1 ; /*10:10,                               */
        U32 reserved_6                     :1 ; /*11:11,                               */
        U32 reserved_7                     :1 ; /*12:12,                               */
        U32 reserved_8                     :1 ; /*13:13,                               */
        U32 reserved_9                     :1 ; /*14:14,                               */
        U32 reserved_10                    :1 ; /*15:15,                               */
        U32 reserved_11                    :1 ; /*16:16,                               */
        U32 reserved_12                    :1 ; /*17:17,                               */
        U32 reserved_13                    :1 ; /*18:18,                               */
        U32 reserved_14                    :1 ; /*19:19,                               */
        U32 reserved_15                    :1 ; /*20:20,                               */
        U32 reserved_16                    :1 ; /*21:21,                               */
        U32 reserved_17                    :1 ; /*22:22,                               */
        U32 reserved_18                    :1 ; /*23:23,                               */
        U32 reserved_19                    :1 ; /*24:24,                               */
        U32 reserved_20                    :1 ; /*25:25,                               */
        U32 reserved_21                    :1 ; /*26:26,                               */
        U32 reserved_22                    :1 ; /*27:27,                               */
        U32 reserved_23                    :1 ; /*28:28,                               */
        U32 reserved_24                    :1 ; /*29:29,                               */
        U32 reserved_25                    :1 ; /*30:30,                               */
        U32 reserved_26                    :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_pmu_intf_icr;

//pmu_intf_imr
typedef union
{
    U32 v;
    struct
    {
        U32 key_int_mask                   :1 ; /*0 :0 , No description.               */
        U32 key_measure_int_mask           :1 ; /*1 :1 , No description.               */
        U32 gpadc_int_mask                 :1 ; /*2 :2 , No description.               */
        U32 lowbat_int_mask                :1 ; /*3 :3 , No description.               */
        U32 ac_on_int_mask                 :1 ; /*4 :4 , No description.               */
        U32 switch_int_mask                :1 ; /*5 :5 , No description.               */
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
} T_PMU_INTF_pmu_intf_imr;

//pmu_intf_irsr
typedef union
{
    U32 v;
    struct
    {
        U32 key_int_raw_status             :1 ; /*0 :0 , No description.               */
        U32 key_measure_int_raw_status     :1 ; /*1 :1 , No description.               */
        U32 gpadc_int_raw_status           :1 ; /*2 :2 , No description.               */
        U32 lowbat_int_raw_status          :1 ; /*3 :3 , No description.               */
        U32 ac_on_int_raw_status           :1 ; /*4 :4 , No description.               */
        U32 switch_int_raw_status          :1 ; /*5 :5 , No description.               */
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
} T_PMU_INTF_pmu_intf_irsr;

//pmu_intf_isr
typedef union
{
    U32 v;
    struct
    {
        U32 key_int_status                 :1 ; /*0 :0 , No description.               */
        U32 key_measure_int_status         :1 ; /*1 :1 , No description.               */
        U32 gpadc_int_status               :1 ; /*2 :2 , No description.               */
        U32 lowbat_int_status              :1 ; /*3 :3 , No description.               */
        U32 ac_on_int_status               :1 ; /*4 :4 , No description.               */
        U32 switch_int_status              :1 ; /*5 :5 , No description.               */
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
} T_PMU_INTF_pmu_intf_isr;

//pmu_ana_rsvd
typedef union
{
    U32 v;
    struct
    {
        U32 pmu_ana_rsvd_out               :12; /*11:0 , reserved for future pmu ana use*/
        U32 pmu_ana_rsvd_in                :6 ; /*17:12, reserved for future pmu ana use*/
        U32 reserved_0                     :1 ; /*18:18,                               */
        U32 reserved_1                     :1 ; /*19:19,                               */
        U32 reserved_2                     :1 ; /*20:20,                               */
        U32 reserved_3                     :1 ; /*21:21,                               */
        U32 reserved_4                     :1 ; /*22:22,                               */
        U32 reserved_5                     :1 ; /*23:23,                               */
        U32 reserved_6                     :1 ; /*24:24,                               */
        U32 reserved_7                     :1 ; /*25:25,                               */
        U32 reserved_8                     :1 ; /*26:26,                               */
        U32 reserved_9                     :1 ; /*27:27,                               */
        U32 reserved_10                    :1 ; /*28:28,                               */
        U32 reserved_11                    :1 ; /*29:29,                               */
        U32 reserved_12                    :1 ; /*30:30,                               */
        U32 reserved_13                    :1 ; /*31:31,                               */
    } b;
} T_PMU_INTF_pmu_ana_rsvd;

typedef struct
{
    volatile          T_PMU_INTF_efuse_ctrl1                    efuse_ctrl1; /*0x0  , RW   , 0x30000000, No description.               */
    volatile          T_PMU_INTF_efuse_ctrl2                    efuse_ctrl2; /*0x4  , RW   , 0x00003210, No description.               */
    volatile        T_PMU_INTF_efuse_readout                  efuse_readout; /*0x8  , RO   , 0x00000000, No description.               */
    volatile            T_PMU_INTF_misc_ctrl                      misc_ctrl; /*0xc  , RW   , 0x00000000, No description.               */
    volatile         T_PMU_INTF_buck_clk_cfg                   buck_clk_cfg; /*0x10 , RW   , 0x00001420, No description.               */
    volatile          T_PMU_INTF_gpadc_ctrl1                    gpadc_ctrl1; /*0x14 , RW   , 0x800008C0, No description.               */
    volatile          T_PMU_INTF_gpadc_ctrl2                    gpadc_ctrl2; /*0x18 , RW   , 0x003FD004, No description.               */
    volatile       T_PMU_INTF_keysense_ctrl1                 keysense_ctrl1; /*0x1c , RW   , 0x04089300, No description.               */
    volatile            T_PMU_INTF_xtal_ctrl                      xtal_ctrl; /*0x20 , RW   , 0x00000009, No description.               */
    volatile      T_PMU_INTF_gpadc_key_value                gpadc_key_value; /*0x24 , RO   , 0x00000000, No description.               */
    volatile           T_PMU_INTF_boost_ctrl                     boost_ctrl; /*0x28 , RW   , 0x00240000, No description.               */
    volatile         T_PMU_INTF_pmu_intf_icr                   pmu_intf_icr; /*0x2c , RW   , 0x00000000, No description.               */
    volatile         T_PMU_INTF_pmu_intf_imr                   pmu_intf_imr; /*0x30 , RW   , 0x0000001F, No description.               */
    volatile        T_PMU_INTF_pmu_intf_irsr                  pmu_intf_irsr; /*0x34 , RO   , 0x00000000, No description.               */
    volatile         T_PMU_INTF_pmu_intf_isr                   pmu_intf_isr; /*0x38 , RO   , 0x00000000, No description.               */
    volatile         T_PMU_INTF_pmu_ana_rsvd                   pmu_ana_rsvd; /*0x3c , RW   , 0x00000333, No description.               */
} T_HWP_PMU_INTF_T;
#else
typedef struct
{
    volatile                             U32                    efuse_ctrl1; /*0x0  , RW   , 0x30000000, No description.               */
    volatile                             U32                    efuse_ctrl2; /*0x4  , RW   , 0x00003210, No description.               */
    volatile                             U32                  efuse_readout; /*0x8  , RO   , 0x00000000, No description.               */
    volatile                             U32                      misc_ctrl; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                   buck_clk_cfg; /*0x10 , RW   , 0x00001420, No description.               */
    volatile                             U32                    gpadc_ctrl1; /*0x14 , RW   , 0x800008C0, No description.               */
    volatile                             U32                    gpadc_ctrl2; /*0x18 , RW   , 0x003FD004, No description.               */
    volatile                             U32                 keysense_ctrl1; /*0x1c , RW   , 0x04089300, No description.               */
    volatile                             U32                      xtal_ctrl; /*0x20 , RW   , 0x00000009, No description.               */
    volatile                             U32                gpadc_key_value; /*0x24 , RO   , 0x00000000, No description.               */
    volatile                             U32                     boost_ctrl; /*0x28 , RW   , 0x00240000, No description.               */
    volatile                             U32                   pmu_intf_icr; /*0x2c , RW   , 0x00000000, No description.               */
    volatile                             U32                   pmu_intf_imr; /*0x30 , RW   , 0x0000001F, No description.               */
    volatile                             U32                  pmu_intf_irsr; /*0x34 , RO   , 0x00000000, No description.               */
    volatile                             U32                   pmu_intf_isr; /*0x38 , RO   , 0x00000000, No description.               */
    volatile                             U32                   pmu_ana_rsvd; /*0x3c , RW   , 0x00000333, No description.               */
} T_HWP_PMU_INTF_T;

//efuse_ctrl1
#define PMU_INTF_EFUSE_OUT(n)          (((n)&0xFF)<<0)     
#define PMU_INTF_EFUSE_OUT_MASK        (0xFF<<0)           
#define PMU_INTF_EFUSE_OUT_SHIFT       (0)                 

#define PMU_INTF_EFUSE_SENSE_EN        (1<<8)              

#define PMU_INTF_EFUSE_SENSE_EN_REG    (1<<9)              

#define PMU_INTF_EFUSE_SENSE_EN_DR     (1<<10)             

#define PMU_INTF_EFUSE_SENSE           (1<<11)             

#define PMU_INTF_EFUSE_SENSE_REG       (1<<12)             

#define PMU_INTF_EFUSE_SENSE_DR        (1<<13)             

#define PMU_INTF_EFUSE_SEL_WORD_REG(n) (((n)&0xF)<<14)     
#define PMU_INTF_EFUSE_SEL_WORD_REG_MASK (0xF<<14)           
#define PMU_INTF_EFUSE_SEL_WORD_REG_SHIFT (14)                

#define PMU_INTF_EFUSE_SEL_WORD_DR     (1<<18)             

#define PMU_INTF_EFUSE_SEL(n)          (((n)&0xFF)<<19)    
#define PMU_INTF_EFUSE_SEL_MASK        (0xFF<<19)          
#define PMU_INTF_EFUSE_SEL_SHIFT       (19)                

#define PMU_INTF_EFUSE_REF_RES_BIT(n)  (((n)&0x3)<<27)     
#define PMU_INTF_EFUSE_REF_RES_BIT_MASK (0x3<<27)           
#define PMU_INTF_EFUSE_REF_RES_BIT_SHIFT (27)                

#define PMU_INTF_RESETN_EFUSE_REG      (1<<29)             

#define PMU_INTF_EFUSE_RD_DONE         (1<<30)             


//efuse_ctrl2
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_0(n) (((n)&0xF)<<0)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_0_MASK (0xF<<0)            
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_0_SHIFT (0)                 

#define PMU_INTF_EFUSE_SEL_WORD_AUTO_1(n) (((n)&0xF)<<4)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_1_MASK (0xF<<4)            
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_1_SHIFT (4)                 

#define PMU_INTF_EFUSE_SEL_WORD_AUTO_2(n) (((n)&0xF)<<8)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_2_MASK (0xF<<8)            
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_2_SHIFT (8)                 

#define PMU_INTF_EFUSE_SEL_WORD_AUTO_3(n) (((n)&0xF)<<12)     
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_3_MASK (0xF<<12)           
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_3_SHIFT (12)                

#define PMU_INTF_REG_PROG_EFUSE_EN(n)  (((n)&0xFFFF)<<16)  
#define PMU_INTF_REG_PROG_EFUSE_EN_MASK (0xFFFF<<16)        
#define PMU_INTF_REG_PROG_EFUSE_EN_SHIFT (16)                


//efuse_readout
#define PMU_INTF_EFUSE_READOUT_0(n)    (((n)&0xFF)<<0)     
#define PMU_INTF_EFUSE_READOUT_0_MASK  (0xFF<<0)           
#define PMU_INTF_EFUSE_READOUT_0_SHIFT (0)                 

#define PMU_INTF_EFUSE_READOUT_1(n)    (((n)&0xFF)<<8)     
#define PMU_INTF_EFUSE_READOUT_1_MASK  (0xFF<<8)           
#define PMU_INTF_EFUSE_READOUT_1_SHIFT (8)                 

#define PMU_INTF_EFUSE_READOUT_2(n)    (((n)&0xFF)<<16)    
#define PMU_INTF_EFUSE_READOUT_2_MASK  (0xFF<<16)          
#define PMU_INTF_EFUSE_READOUT_2_SHIFT (16)                

#define PMU_INTF_EFUSE_READOUT_3(n)    (((n)&0xFF)<<24)    
#define PMU_INTF_EFUSE_READOUT_3_MASK  (0xFF<<24)          
#define PMU_INTF_EFUSE_READOUT_3_SHIFT (24)                

//misc_ctrl
#define PMU_INTF_PU_XTAL_32K           (1<<0)              

#define PMU_INTF_VBAT_MONITOR_EN       (1<<1)              

//buck_clk_cfg
#define PMU_INTF_SYNC_CLOCK_POLARITY   (1<<0)              

#define PMU_INTF_BUCK_CLK_CFG_LD       (1<<1)              

#define PMU_INTF_PU_BUCK_CLK_REG       (1<<2)              

#define PMU_INTF_PU_BUCK_CLK_DR        (1<<3)              

#define PMU_INTF_REG_SYNC_CLK_NUM(n)   (((n)&0x3)<<4)      
#define PMU_INTF_REG_SYNC_CLK_NUM_MASK (0x3<<4)            
#define PMU_INTF_REG_SYNC_CLK_NUM_SHIFT (4)                 

#define PMU_INTF_REG_SYNC_CLK_DENOM(n) (((n)&0x1F)<<6)     
#define PMU_INTF_REG_SYNC_CLK_DENOM_MASK (0x1F<<6)           
#define PMU_INTF_REG_SYNC_CLK_DENOM_SHIFT (6)                 

#define PMU_INTF_PFM_MODE_CLK_PRD_BUCK(n) (((n)&0x3)<<11)     
#define PMU_INTF_PFM_MODE_CLK_PRD_BUCK_MASK (0x3<<11)           
#define PMU_INTF_PFM_MODE_CLK_PRD_BUCK_SHIFT (11)                

//gpadc_ctrl1
#define PMU_INTF_RESETN_GPADC_REG      (1<<0)              

#define PMU_INTF_RESETN_GPADC_DR       (1<<1)              

#define PMU_INTF_GPADC_START_REG       (1<<2)              

#define PMU_INTF_GPADC_START_DR        (1<<3)              

#define PMU_INTF_GPADC_START           (1<<4)              

#define PMU_INTF_RESETN_GPADC          (1<<5)              

#define PMU_INTF_TIME_SAMP_NEG_GPADC   (1<<6)              

#define PMU_INTF_TIME_SAMP_POS_GPADC   (1<<7)              

#define PMU_INTF_TIMER_SAR_STABLE_SEL(n) (((n)&0x7)<<8)      
#define PMU_INTF_TIMER_SAR_STABLE_SEL_MASK (0x7<<8)            
#define PMU_INTF_TIMER_SAR_STABLE_SEL_SHIFT (8)                 

#define PMU_INTF_DELAY_BEFORE_SAMP_GPADC(n) (((n)&0x3)<<11)     
#define PMU_INTF_DELAY_BEFORE_SAMP_GPADC_MASK (0x3<<11)           
#define PMU_INTF_DELAY_BEFORE_SAMP_GPADC_SHIFT (11)                

#define PMU_INTF_SAR_OUT_POLARITY_GPADC (1<<13)             

#define PMU_INTF_SAR_ADC_MODE_GPADC    (1<<14)             

#define PMU_INTF_CONV_CLK_INV_GPADC    (1<<15)             

#define PMU_INTF_SAMP_CLK_INV_GPADC    (1<<16)             

#define PMU_INTF_GPADC_EOC_PHASE_SEL   (1<<17)             

#define PMU_INTF_GPADC_CHANNEL_EN_REG(n) (((n)&0xFF)<<18)    
#define PMU_INTF_GPADC_CHANNEL_EN_REG_MASK (0xFF<<18)          
#define PMU_INTF_GPADC_CHANNEL_EN_REG_SHIFT (18)                

#define PMU_INTF_GPADC_CHANNEL_EN_DR   (1<<26)             

#define PMU_INTF_GPADC_ENABLE          (1<<27)             

#define PMU_INTF_GPADC_CLK_DIV_EN      (1<<28)             

#define PMU_INTF_GPADC_START_TIME_SEL(n) (((n)&0x7)<<29)     
#define PMU_INTF_GPADC_START_TIME_SEL_MASK (0x7<<29)           
#define PMU_INTF_GPADC_START_TIME_SEL_SHIFT (29)                


//gpadc_ctrl2
#define PMU_INTF_GPADC_VREF_MS_ENABLE  (1<<0)              

#define PMU_INTF_GPADC_VREF_BIT(n)     (((n)&0x3)<<1)      
#define PMU_INTF_GPADC_VREF_BIT_MASK   (0x3<<1)            
#define PMU_INTF_GPADC_VREF_BIT_SHIFT  (1)                 

#define PMU_INTF_GPADC_CHANNEL_EN(n)   (((n)&0xFF)<<3)     
#define PMU_INTF_GPADC_CHANNEL_EN_MASK (0xFF<<3)           
#define PMU_INTF_GPADC_CHANNEL_EN_SHIFT (3)                 

#define PMU_INTF_PU_GPADC              (1<<11)             

#define PMU_INTF_PU_GPADC_REG          (1<<12)             

#define PMU_INTF_PU_GPADC_DR           (1<<13)             

#define PMU_INTF_GPADC_CHANNEL_SEL(n)  (((n)&0xFF)<<14)    
#define PMU_INTF_GPADC_CHANNEL_SEL_MASK (0xFF<<14)          
#define PMU_INTF_GPADC_CHANNEL_SEL_SHIFT (14)                


//keysense_ctrl1
#define PMU_INTF_KEY_PULLUP2X          (1<<0)              

#define PMU_INTF_KEY_EN                (1<<1)              

#define PMU_INTF_KEY_EN_REG            (1<<2)              

#define PMU_INTF_KEY_EN_DR             (1<<3)              

#define PMU_INTF_KEY_INT_EN            (1<<4)              

#define PMU_INTF_KEY_INT_EN_REG        (1<<5)              

#define PMU_INTF_KEY_INT_EN_DR         (1<<6)              

#define PMU_INTF_KEY_ENABLE            (1<<7)              

#define PMU_INTF_TIME_SAMP_NEG_TSC     (1<<8)              

#define PMU_INTF_TIME_SAMP_POS_TSC     (1<<9)              

#define PMU_INTF_TIMER_X_TO_Y_SEL(n)   (((n)&0x3)<<10)     
#define PMU_INTF_TIMER_X_TO_Y_SEL_MASK (0x3<<10)           
#define PMU_INTF_TIMER_X_TO_Y_SEL_SHIFT (10)                

#define PMU_INTF_TIMER_INT_TO_X_SEL(n) (((n)&0x7)<<12)     
#define PMU_INTF_TIMER_INT_TO_X_SEL_MASK (0x7<<12)           
#define PMU_INTF_TIMER_INT_TO_X_SEL_SHIFT (12)                

#define PMU_INTF_TIMER_PENIRQ_STABLE(n) (((n)&0x7)<<15)     
#define PMU_INTF_TIMER_PENIRQ_STABLE_MASK (0x7<<15)           
#define PMU_INTF_TIMER_PENIRQ_STABLE_SHIFT (15)                

#define PMU_INTF_DELAY_BEFORE_SAMP_TSC(n) (((n)&0x3)<<18)     
#define PMU_INTF_DELAY_BEFORE_SAMP_TSC_MASK (0x3<<18)           
#define PMU_INTF_DELAY_BEFORE_SAMP_TSC_SHIFT (18)                

#define PMU_INTF_BYPASS_BB_READ        (1<<20)             

#define PMU_INTF_CONV_CLK_INV          (1<<21)             

#define PMU_INTF_SAMP_CLK_INV          (1<<22)             

#define PMU_INTF_SAR_ADC_MODE          (1<<23)             

#define PMU_INTF_SAR_OUT_POLARITY      (1<<24)             

#define PMU_INTF_WAIT_BB_READ_TIMESEL(n) (((n)&0x3)<<25)     
#define PMU_INTF_WAIT_BB_READ_TIMESEL_MASK (0x3<<25)           
#define PMU_INTF_WAIT_BB_READ_TIMESEL_SHIFT (25)                

#define PMU_INTF_KEY_CLK_DIV_EN        (1<<27)             

//xtal_ctrl
#define PMU_INTF_XTAL_CLK_BB_EN_LV     (1<<0)              

#define PMU_INTF_XTAL_CLK_RSVD_EN_LV   (1<<1)              

#define PMU_INTF_XTAL_CLK_AUPLL_EN_LV  (1<<2)              

#define PMU_INTF_XTAL_CLK_ADPLL_EN_LV  (1<<3)              

#define PMU_INTF_XTAL_CLK_ADC_EN_LV    (1<<4)              


//gpadc_key_value
#define PMU_INTF_KEY_VALUE_VALID       (1<<0)              

#define PMU_INTF_KEY_VALUE(n)          (((n)&0x3FF)<<1)    
#define PMU_INTF_KEY_VALUE_MASK        (0x3FF<<1)          
#define PMU_INTF_KEY_VALUE_SHIFT       (1)                 

#define PMU_INTF_GPADC_CHANNEL_NUM(n)  (((n)&0x7)<<11)     
#define PMU_INTF_GPADC_CHANNEL_NUM_MASK (0x7<<11)           
#define PMU_INTF_GPADC_CHANNEL_NUM_SHIFT (11)                

#define PMU_INTF_GPADC_VALUE_VALID     (1<<14)             

#define PMU_INTF_GPADC_VALUE(n)        (((n)&0x3FF)<<15)   
#define PMU_INTF_GPADC_VALUE_MASK      (0x3FF<<15)         
#define PMU_INTF_GPADC_VALUE_SHIFT     (15)                

//boost_ctrl
#define PMU_INTF_PU_BOOST              (1<<0)              

#define PMU_INTF_DUTYSEL_BOOST         (1<<1)              
#define PMU_INTF_DUTYSEL_BOOST_DEFAULT (0<<1)              
#define PMU_INTF_DUTYSEL_BOOST_BY      (1<<1)              

#define PMU_INTF_DUTY_BIT_BOOST(n)     (((n)&0x7)<<2)      
#define PMU_INTF_DUTY_BIT_BOOST_MASK   (0x7<<2)            
#define PMU_INTF_DUTY_BIT_BOOST_SHIFT  (2)                 

#define PMU_INTF_CURLIMIT_SEL_BOOST(n) (((n)&0x7)<<5)      
#define PMU_INTF_CURLIMIT_SEL_BOOST_MASK (0x7<<5)            
#define PMU_INTF_CURLIMIT_SEL_BOOST_SHIFT (5)                 

#define PMU_INTF_BOOST_VBIT(n)         (((n)&0xF)<<8)      
#define PMU_INTF_BOOST_VBIT_MASK       (0xF<<8)            
#define PMU_INTF_BOOST_VBIT_SHIFT      (8)                 

#define PMU_INTF_REG_RSVD_BOOST(n)     (((n)&0xF)<<12)     
#define PMU_INTF_REG_RSVD_BOOST_MASK   (0xF<<12)           
#define PMU_INTF_REG_RSVD_BOOST_SHIFT  (12)                

#define PMU_INTF_REG_CLK_FREQ_SEL_BOOST(n) (((n)&0x7)<<16)     
#define PMU_INTF_REG_CLK_FREQ_SEL_BOOST_MASK (0x7<<16)           
#define PMU_INTF_REG_CLK_FREQ_SEL_BOOST_SHIFT (16)                

#define PMU_INTF_REG_CLK_DUTY_SEL_BOOST(n) (((n)&0x7)<<19)     
#define PMU_INTF_REG_CLK_DUTY_SEL_BOOST_MASK (0x7<<19)           
#define PMU_INTF_REG_CLK_DUTY_SEL_BOOST_SHIFT (19)                

#define PMU_INTF_BOOST_CLK_ENABLE      (1<<22)             

//pmu_intf_icr
#define PMU_INTF_KEY_INT_CLEAR         (1<<0)              

#define PMU_INTF_KEY_MEASURE_INT_CLEAR (1<<1)              

#define PMU_INTF_GPADC_INT_CLEAR       (1<<2)              

#define PMU_INTF_LOWBAT_INT_CLEAR      (1<<3)              

#define PMU_INTF_AC_ON_INT_CLEAR       (1<<4)              


//pmu_intf_imr
#define PMU_INTF_KEY_INT_MASK          (1<<0)              

#define PMU_INTF_KEY_MEASURE_INT_MASK  (1<<1)              

#define PMU_INTF_GPADC_INT_MASK        (1<<2)              

#define PMU_INTF_LOWBAT_INT_MASK       (1<<3)              

#define PMU_INTF_AC_ON_INT_MASK        (1<<4)              

#define PMU_INTF_SWITCH_INT_MASK       (1<<5)              


//pmu_intf_irsr
#define PMU_INTF_KEY_INT_RAW_STATUS    (1<<0)              

#define PMU_INTF_KEY_MEASURE_INT_RAW_STATUS (1<<1)              

#define PMU_INTF_GPADC_INT_RAW_STATUS  (1<<2)              

#define PMU_INTF_LOWBAT_INT_RAW_STATUS (1<<3)              

#define PMU_INTF_AC_ON_INT_RAW_STATUS  (1<<4)              

#define PMU_INTF_SWITCH_INT_RAW_STATUS (1<<5)              


//pmu_intf_isr
#define PMU_INTF_KEY_INT_STATUS        (1<<0)              

#define PMU_INTF_KEY_MEASURE_INT_STATUS (1<<1)              

#define PMU_INTF_GPADC_INT_STATUS      (1<<2)              

#define PMU_INTF_LOWBAT_INT_STATUS     (1<<3)              

#define PMU_INTF_AC_ON_INT_STATUS      (1<<4)              

#define PMU_INTF_SWITCH_INT_STATUS     (1<<5)              

//pmu_ana_rsvd
#define PMU_INTF_PMU_ANA_RSVD_OUT(n)   (((n)&0xFFF)<<0)    
#define PMU_INTF_PMU_ANA_RSVD_OUT_MASK (0xFFF<<0)          
#define PMU_INTF_PMU_ANA_RSVD_OUT_SHIFT (0)                 

#define PMU_INTF_PMU_ANA_RSVD_IN(n)    (((n)&0x3F)<<12)    
#define PMU_INTF_PMU_ANA_RSVD_IN_MASK  (0x3F<<12)          
#define PMU_INTF_PMU_ANA_RSVD_IN_SHIFT (12)                


#endif

#endif


