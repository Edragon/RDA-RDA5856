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
// File name      : bt_modem                                            
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
#ifndef __BT_MODEM_H__
#define __BT_MODEM_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//bypass_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 sel_rssi_src                   :1 ; /*0 :0 , Select RSSI Source1'h0:: srrc_output: RSSI input data is from SRRC filter output1'h1:: mixer_output: RSSI input data is from mixer output*/
        U32 bypass_smppt_dpsk              :1 ; /*1 :1 , Bypass DPSK Sample Step1'h0:: not_bypass: RX DPSK Sample Step is 11'h1:: bypass: RX DPSK Sample Step is 0*/
        U32 bypass_patch_dpsk              :1 ; /*2 :2 , Bypass DPSK Patch1'h0:: not_bypass: DPSK Patch is not bypassed1'h1:: bypass: DPSK Patch is bypassed*/
        U32 bypass_derr2_dpsk              :1 ; /*3 :3 , Bypass DPSK Derr21'h0:: not_bypass: RX DPSK Derr2 is not bypassed1'h1:: bypass: RX DPSK Derr2 is bypassed*/
        U32 bypass_derr1_dpsk              :1 ; /*4 :4 , Bypass DPSK Derr11'h0:: not_bypass: RX DPSK Derr1 is not bypassed1'h1:: bypass: RX DPSK Derr1 is bypassed*/
        U32 bypass_smppt_gfsk              :1 ; /*5 :5 , Bypass GFSK Sample Step1'h0:: not_bypass: RX GFSK Sample Step is 11'h1:: bypass: RX GFSK Sample Step is 0*/
        U32 bypass_patch_gfsk              :1 ; /*6 :6 , Bypass GFSK Patch1'h0:: not_bypass: GFSK Patch is not bypassed1'h1:: bypass: GFSK Patch is bypassed*/
        U32 bypass_derr2_gfsk              :1 ; /*7 :7 , Bypass GFSK Derr21'h0:: not_bypass: RX GFSK Derr2 is not bypassed1'h1:: bypass: RX GFSK Derr2 is bypassed*/
        U32 bypass_derr1_gfsk              :1 ; /*8 :8 , Bypass GFSK Derr11'h0:: not_bypass: RX GFSK Derr1 is not bypassed1'h1:: bypass: RX GFSK Derr1 is bypassed*/
        U32 bypass_ctrl_reserved_0         :3 ; /*11:9 , RESERVED                      */
        U32 bypass_srrc                    :1 ; /*12:12, Bypass Square-root-raised-cosine Filter1'h0:: not_bypass: RX SRRC filter is not bypassed1'h1:: bypass: RX SRRC filter is bypassed*/
        U32 bypass_mixer                   :1 ; /*13:13, Bypass Mixer1'h0:: not_bypass: RX Mixer is not bypassed1'h1:: bypass: RX Mixer is bypassed*/
        U32 bypass_dccancel2               :1 ; /*14:14, RESERVED                      */
        U32 bypass_dccancel                :1 ; /*15:15, Bypass DC Cancel1'h0:: not_bypass: RX DC Cancel is not bypassed1'h1:: bypass: RX DC Cancel is bypassed*/
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
} T_BT_MODEM_bypass_ctrl;

//sw_swap_dccal
typedef union
{
    U32 v;
    struct
    {
        U32 sel_sumerr_range               :3 ; /*2 :0 , Sum Error Range Control3'h0 Left shift 3 bits of sum err and limit sumerr within [-2^-4, 2^-4]3'h1 Left shift 2 bits of sum err and limit sumerr within [-2^-3, 2^-3]3'h2 Left shift 1 bits of sum err and limit sumerr within [-2^-2, 2^-2]3'h3 Hold the sum err3'h4 Right shift 1 bits of sum err3'h5 Right shift 2 bits of sum err3'h6 Right shift 3 bits of sum err3'h7 Right shift 4 bits of sum err;*/
        U32 sw_swap_dccal_reserved_1       :2 ; /*4 :3 , RESERVED                      */
        U32 iq_sel_pol                     :1 ; /*5 :5 , SRRC IQ_SEL Polarity1'h0:: iq_sel_inv1'h1:: iq_sel_raw*/
        U32 swch_sign_rx                   :1 ; /*6 :6 , Switch RX ADC IQ data sign1'h0:: unsigned: RX ADC data is unsigned; analog common format1'h1:: signed: RX ADC data is signed*/
        U32 swch_sign_q_tx                 :1 ; /*7 :7 , Switch TX DAC dataq sign1'h0:: unsigned: TX DAC dataq is unsigned; analog common format1'h1:: signed: TX DAC dataq is signed*/
        U32 swch_sign_i_tx                 :1 ; /*8 :8 , Switch TX DAC datai sign1'h0:: unsigned: TX DAC datai is unsigned; analog common format1'h1:: signed: TX DAC datai is signed*/
        U32 lpf_dwidth_sel                 :2 ; /*10:9 , LPF Data Width Select2'h0:: shift_9bits2'h1:: shift_8bits2'h2:: shift_7bits2'h3:: shift_6bits*/
        U32 rsvd0                          :1 ; /*11:11, RESERVED                      */
        U32 swch_clk_dac                   :1 ; /*12:12, Switch DAC Clock Edge1'h0:: not_switch: DAC clock edge is not switched1'h1:: switch: DAC clock edge is switched*/
        U32 sel_sync                       :1 ; /*13:13, Select New Packet1'h0:: from_LL: newpacket_dsp is from baseband1'h1:: from_reg: newpacket_dsp is from newpacket_reg*/
        U32 swch_clk_adc                   :1 ; /*14:14, Switch ADC Clock Edge1'h0:: not_switch: ADC clock edge is not switched1'h1:: switch: ADC clock edge is switched*/
        U32 sw_swap_dccal_reserved_0       :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_sw_swap_dccal;

//dem750_afc_freq
typedef union
{
    U32 v;
    struct
    {
        U32 afc_smtif                      :16; /*15:0 , Set the AFC frequency of dem750 of rx linkdec2hex(2^16-round(2*740/13e3*2^16))*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_dem750_afc_freq;

//dpsk_gfsk_tx_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 delay_dpsk                     :4 ; /*3 :0 , Set the delay of input dpsk symbol, delay unit is 13MHz clock cycle.*/
        U32 delay_gfsk                     :4 ; /*7 :4 , Set the delay of input gfsk symbol, delay unit is 13MHz clock cycle.*/
        U32 tx_gain_dpsk                   :8 ; /*15:8 , DPSK TX Gain in EDR           */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_dpsk_gfsk_tx_ctrl;

//tx_gain_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 tx_gain_gfsk                   :8 ; /*7 :0 , GFSK TX Gain in BR            */
        U32 rsvd4                          :1 ; /*8 :8 , RESERVED                      */
        U32 tx_gain_ctrl_reserved_0        :1 ; /*9 :9 , RESERVED                      */
        U32 cnt_guard_ini                  :6 ; /*15:10, Control the guard time length of bt frameGuard time = (55-cnt_guard_ini)*T13Mclk*/
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
} T_BT_MODEM_tx_gain_ctrl;

//rssi_out1
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_out                       :10; /*9 :0 , RSSI Output.                  */
        U32 rssi_out1_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_rssi_out1;

//sw_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 tx_gain_gfsk_edr               :8 ; /*7 :0 , GFSK TX Gain in EDR           */
        U32 notch_disb                     :1 ; /*8 :8 , Disable the ramping for edr guard time in ramp_gain_tx*/
        U32 swap_pn_q_dccl_0               :1 ; /*9 :9 , Swap ddcl input Q data polarity1'h0:: no_swap: Q data polarity of ddcl input is not swaped1'h1:: swap: Q data polarity of ddcl input is swaped*/
        U32 swap_pn_i_dccl_0               :1 ; /*10:10, Swap ddcl input I data polarity1'h0:: no_swap: I data polarity of ddcl input is not swaped1'h1:: swap: I data polarity of ddcl input is swaped*/
        U32 swap_iq_mixer_0                :1 ; /*11:11, Swap I/Q of Mixer output data1'h0:: no_swap: I/Q of mixer output data is not swaped1'h1:: swap: I/Q of mixer output data is swaped*/
        U32 swap_iq_smtif_0                :1 ; /*12:12, RESERVED                      */
        U32 swap_iq_dccl_0                 :1 ; /*13:13, Swap I/Q of ddcl input data1'h0:: no_swap: I/Q of ddcl input data is not swaped1'h1:: swap: I/Q of ddcl input data is swaped*/
        U32 swap_iq                        :1 ; /*14:14, Swap I/Q1'h0:: no_swap: I/Q is not swaped1'h1:: swap: I/Q is swaped*/
        U32 rssi_lock_by_agc               :1 ; /*15:15, Receiver Strength Signal Indicator lock by agc done signal*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_sw_ctrl;

//adcclk_sw_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ct_u_sp_gfsk                   :4 ; /*3 :0 , gfsk ct_u_sp for rx demod     */
        U32 ct_u_1_gfsk                    :5 ; /*8 :4 , gfsk u_dc 4/512               */
        U32 ct_u_gfsk                      :4 ; /*12:9 , gfsk u_err 10/32              */
        U32 pckt_sel                       :1 ; /*13:13, Packet select1'h0:: packet721'h1:: new_packet*/
        U32 en_bbpkg_flg                   :1 ; /*14:14, BB Newpacket flag enable1'h0:: Disable: the BB Newpacket flag enable1'h1:: Enable: the BB Newpacket flag enable*/
        U32 adcclk_sw_ctrl_reserved_0      :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_adcclk_sw_ctrl;

//dpsk_demod_sw
typedef union
{
    U32 v;
    struct
    {
        U32 ct_u_sp_dpsk                   :4 ; /*3 :0 , Switch dpsk ct_u for bt_dsp rx demod*/
        U32 ct_u_1_dpsk                    :5 ; /*8 :4 , dpsk u_dc 5/64                */
        U32 ct_u_dpsk                      :4 ; /*12:9 , dpsk u_err 8/32               */
        U32 ct_u_patch                     :3 ; /*15:13, Switch err_in_patch for bt_dsp rx demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_dpsk_demod_sw;

//min_phase_err
typedef union
{
    U32 v;
    struct
    {
        U32 min_error_th                   :16; /*15:0 , Set the minimum phase error for  rx demod.*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_min_phase_err;

//afc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 th_gfsk_dem                    :10; /*9 :0 , GFSK demod threshold          */
        U32 rsvd1                          :2 ; /*11:10, RESERVED                      */
        U32 sel_afc_dpskseek               :2 ; /*13:12, Select the DPSK AFC of demod  */
        U32 sel_afc_gfskdem                :2 ; /*15:14, Select the GFSK AFC of demod  */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_afc_ctrl;

//dpsk_gfsk_smp_th
typedef union
{
    U32 v;
    struct
    {
        U32 th_gfsk_dem2                   :10; /*9 :0 , GFSK sample 2nd threshold for demod*/
        U32 sp_th_sel_gfsk                 :2 ; /*11:10, GFSK sample threshold for demod*/
        U32 sp_th_sel_dpsk                 :2 ; /*13:12, DPSK sample threshold for demod*/
        U32 diff_enable_gfsk               :1 ; /*14:14, GFSK diff enable1'h0:: Disable1'h1:: Enable*/
        U32 diff_enable_dpsk               :1 ; /*15:15, DPSK diff enable1'h0:: Disable1'h1:: Enable*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_dpsk_gfsk_smp_th;

//dpsk_gfsk_smp_th_1
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a2_sek                     :10; /*9 :0 , gfsk sample reference a2 for demod*/
        U32 dpsk_gfsk_smp_th_1_reserved_0  :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_dpsk_gfsk_smp_th_1;

//gfsk_smp_ref_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a3_sek                     :8 ; /*7 :0 , GFSK sample reference a3 for demod*/
        U32 ref_a1_sek                     :8 ; /*15:8 , GFSK sample reference a1 for demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_smp_ref_ctrl;

//gfsk_smp_ref_ctrl_1
typedef union
{
    U32 v;
    struct
    {
        U32 dc_ct2                         :9 ; /*8 :0 , RESERVED                      */
        U32 guard_time_conf                :5 ; /*13:9 , tx guard timing delay to switch amp in ramp_gain_tx; counter in 13M*/
        U32 rsvd2                          :2 ; /*15:14, RESERVED                      */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_smp_ref_ctrl_1;

//rateconv_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 dac_data_sel                   :6 ; /*5 :0 , DAC Data Mux Select6'b000000:: tx_52m_i:      tx_52m_q6'b000001:: tx_26m_i:     tx_26m_q6'b000010:: iqim_cancel_i:      iqim_cancel_q6'b000011:: tx_13m_i:      tx_13m_q6'b000100:: mixer_tx_i:      mixer_tx_q6'b000101:: accu_tx:     blend_tx6'b000110:: gfilter_tx:     diff_tx6'b000111:: ampm_am:     ampm_pm6'b001000:: cordic_tx_amp:     cordic_tx_ang6'b001001:: symbol2iq_tx_i:     symbol2iq_tx_q6'b001010:: tx_test_data0:      tx_test_data16'b100000:: angle:       angle_rc6'b100001:: adc_data_i:       adc_data_q6'b100010:: adc_din_i:       adc_din_q6'b100011:: lpf_i:       lpf_q6'b100100:: rateconv_i:      rateconv_q6'b100101:: calib_i:         calib_q6'b100110:: dc_calib_i:      dc_calib_q6'b100111:: cancel_flt_i:       cancel_flt_q6'b101000:: notch_i:       notch_q6'b101001:: gain_i:        gain_q6'b101010:: ble_mux_i:     ble_mux_q6'b101011:: mixer_i:      mixer_q6'b101100:: srrc_i:     srrc_q6'b101101:: mixer_i_13_0:    mixer_q[13:0]6'b101110:: srrc_i_11_0:     srrc_q[11:0]6'b101111:: err_gfsk:       err_dpsk6'b110000:: afc_gfsk:       afc_in6'b110001:: angle_offset:      angle_offset16'b110010:: rssi_out:      rssi_out6'b110011:: rx_test_data0:       rx_test_data16'b110100:: rx_test_data2:      rx_test_data3*/
        U32 dac_test_en                    :1 ; /*6 :6 , DAC Test Enable1'h0 dac data is 52m_tx IQ1'h1 dac data depends on dac_data_sel*/
        U32 dc_ct                          :9 ; /*15:7 , DC Cancle ct code for demod   */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rateconv_ctrl1;

//rateconv_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_tm_th                     :6 ; /*5 :0 , RSSI threshold                */
        U32 dc_hold_edr_en                 :1 ; /*6 :6 , DC cancle1 edr dc hold enable */
        U32 rateconv_ctrl2_reserved_0      :9 ; /*15:7 , RESERVED                      */
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
} T_BT_MODEM_rateconv_ctrl2;

//demod_smp_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 cnt_sample_ini                 :4 ; /*3 :0 , Sample point initial value    */
        U32 ref_ready_th                   :6 ; /*9 :4 , GFSK iph th reference for demod.*/
        U32 ini_rst_th                     :6 ; /*15:10, Count sample threshold reset for demod.*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_demod_smp_ctrl;

//agc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 agc_sinc_over_th               :2 ; /*1 :0 , Threshold of Rounding after sinc.*/
        U32 agc_sinc_over_en               :1 ; /*2 :2 , Rounding enable after sinc.1'h0:: Disable1'h1:: Enable*/
        U32 agc_mod_fix7                   :1 ; /*3 :3 , Fix7 mode select during bt dsp demod1'h0:: threshold_21'h1:: threshold_3*/
        U32 agc_en_fix7                    :1 ; /*4 :4 , Fix7 enable during demod1'h0:: Disable1'h1:: Enable*/
        U32 dpsk_seek_st_cnt               :5 ; /*9 :5 , DPSK Seek Start Count         */
        U32 mix_guard_th                   :6 ; /*15:10, Guard time length threshold for demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_agc_ctrl;

//agc_th_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 agc_th_min                     :8 ; /*7 :0 , AGC minimum threshold for demod*/
        U32 agc_th_max                     :8 ; /*15:8 , AGC maximum threshold for demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_agc_th_ctrl1;

//agc_th_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 agc_th_min_lg                  :8 ; /*7 :0 , AGC minimum large threshold for demod*/
        U32 agc_th_max_lg                  :8 ; /*15:8 , AGC maximum large threshold for demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_agc_th_ctrl2;

//agc_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 agc_mod_dem                    :2 ; /*1 :0 , Demod mode select             */
        U32 agc_index_ini_dsp              :4 ; /*5 :2 , AGC gain index initial value for bt dsp*/
        U32 agc_en_dly                     :3 ; /*8 :6 , Delay timer count enable3'b000:: Delay_0us3'b001:: Delay_0p5us3'b010:: Delay_1us3'b011:: Delay_2us3'b100:: Delay_3us3'b101:: Delay_4us3'b110:: Delay_6us3'b111:: Delay_8us*/
        U32 agc_step_over                  :2 ; /*10:9 , AGC step over                 */
        U32 agc_step_mode                  :2 ; /*12:11, AGC step mode for demod2'b00:: AGC_step_12'b01:: AGC_step_22'b10:: AGC_step_32'b11:: AGC_step_4*/
        U32 agc_en_lg_stp                  :1 ; /*13:13, AGC logarithmic step enable for demod1'h0:: Disable1'h1:: Enable*/
        U32 agc_ct_th_min                  :1 ; /*14:14, AGC minimum threshold for demod*/
        U32 agc_ctrl1_reserved_0           :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_agc_ctrl1;

//agc_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 agc_tm_intv_lg                 :7 ; /*6 :0 , AGC tm_intv_int logarithmic initial value for demod*/
        U32 agc_tm_intv                    :7 ; /*13:7 , AGCtm_intv_int initial value for demod*/
        U32 agc_th_min_gn                  :2 ; /*15:14, AGC minimum threshold gain select for demod2'b00:: Gain_22'b01:: Gain_42'b10:: Gain_82'b11:: Gain_16*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_agc_ctrl2;

//agc_dgc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 agc_mode_dsp                   :3 ; /*2 :0 , AGC mode for dsp3'h0:: Normal3'h1:: RESERVED3'h2:: Hold_after_timer3'h3:: fix_to_index_ini3'h4:: Hold_by_FSM3'h5:: Th_large_mode: select by FSMothers RESERVED*/
        U32 agc_dgc_ctrl_reserved_0        :3 ; /*5 :3 , RESERVED                      */
        U32 newpacket_zero_wd_cnt          :2 ; /*7 :6 , Newpacket zero bytes number2'b00 If the 1st  byte of newpacket  is zero, newpacket_bb_sel is logic high, else low2'b01 If the 1st &amp; 2nd  byte of newpacket  is zero, newpacket_bb_sel is logic high, else low2'b10 If the 1st &amp; 2nd &amp; 3rd byte of newpacket  is zero,newpacket_bb_sel is logic high, else low2'b11 If the 1st &amp; 2nd &amp; 3rd &amp;4th byte of newpacket  is zero, newpacket_bb_sel is logic high, else low*/
        U32 pass_newpacket_sel             :1 ; /*8 :8 , Newpacket select for demod1'h0 If newpacket from BB has one zero byte, select GID for demod, else select newpacket from BB1'h1 Select newpacket from BB*/
        U32 dgc_index_max                  :3 ; /*11:9 , Maximum agc gain index        */
        U32 dgc_index_dsp                  :3 ; /*14:12, DGC gain index                */
        U32 sel_reg_agc                    :1 ; /*15:15, AGC index select1'h0:: dgc_index_dsp1'h1:: dgc_index_mx*/
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
} T_BT_MODEM_agc_dgc_ctrl;

//agc_dccal_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 rx_fix_dcofst                  :1 ; /*0 :0 , DC offset fix select for rx1'h0:: by_calib: DC offset data set by calibration1'h1:: by_reg: DC offset data set by register*/
        U32 dc_cal_rx_dly                  :2 ; /*2 :1 , RX DC Calibration Delay for 1 loop2'h0:: 0p6ms2'h1:: 1p2ms2'h2:: 2p4ms2'h3:: 4p8ms*/
        U32 rx_dc_cal_done                 :1 ; /*3 :3 , RX DC Calibration Done        */
        U32 agc_dccal_ctrl_reserved_0      :2 ; /*5 :4 , RESERVED                      */
        U32 agc_tm_hold                    :5 ; /*10:6 , AGC hold time length          */
        U32 agc_tm_wait                    :5 ; /*15:11, AGC hold waiting time length  */
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
} T_BT_MODEM_agc_dccal_ctrl;

//rx_dccal_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 rx_dc_cali_q_fix               :8 ; /*7 :0 , RX DC fixed offset data for Q path when if_fix_dcofst is 1; otherwise use the auto calc values.*/
        U32 rx_dc_cali_i_fix               :8 ; /*15:8 , RX DC fixed offset data for I path when if_fix_dcofst is 1; otherwise use the auto calc values.*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rx_dccal_ctrl;

//rx_dcofst_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 rx_dcoffset_q                  :8 ; /*7 :0 , rx dc offset for dc calibration; selected from dc_cali_q_fix &amp; dc_i2d_work_q*/
        U32 rx_dcoffset_i                  :8 ; /*15:8 , rx dc offset for dc calibration; selected from dc_cali_i_fix &amp; dc_i2d_work_i*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rx_dcofst_inuse;

//tx_dc_calib
typedef union
{
    U32 v;
    struct
    {
        U32 bypass_tx_cal                  :1 ; /*0 :0 , Bypass TX Calibration Offset1'b0:: not_bypass1'b1:: bypass*/
        U32 tx_apc                         :3 ; /*3 :1 , TX Gain Table Pointer during work*/
        U32 tx_cal_shift                   :2 ; /*5 :4 , TX Calibration Offset Shift2'b00:: x4: left shift by 2 bits2'b01:: x2: left shift by 1 bit2'b10:: x1: no shift*/
        U32 tx_cal_sel                     :2 ; /*7 :6 , TX Calibration Selection2'b00:: mean: (tx_cal1 + tx_cal2)/22'b01:: tx_cal12'b10:: tx_cal2*/
        U32 tx_cal_pol                     :1 ; /*8 :8 , TX Calibration Offset Polarity0:: no_switch: the polarity of TX calibration offset1:: switch: the polarity of TX calibration offset*/
        U32 tx_cal_cmp_pol                 :1 ; /*9 :9 , TX Calibration Comparison Polarity*/
        U32 tx_cal_cnt                     :2 ; /*11:10, TX Calibration Step Counters for 25KHz2'b00:: 0p125_range2'b01:: 0p25_range2'b10:: 0p5_range2'b11:: full_range*/
        U32 tx_fix_dcofst                  :1 ; /*12:12, Fix TX DC Offset              */
        U32 tx_dc_calib_reserved_0         :2 ; /*14:13, RESERVED                      */
        U32 tx_dc_cal_done                 :1 ; /*15:15, TX Calibration Done           */
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
} T_BT_MODEM_tx_dc_calib;

//tx_fix_i_dcofst
typedef union
{
    U32 v;
    struct
    {
        U32 tx_dc_idata_fix                :12; /*11:0 , Fixed TX I Signed Data for DC offset*/
        U32 tx_fix_i_dcofst_reserved_0     :4 ; /*15:12, RESERVED                      */
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
} T_BT_MODEM_tx_fix_i_dcofst;

//tx_fix_q_dcofst
typedef union
{
    U32 v;
    struct
    {
        U32 tx_dc_qdata_fix                :12; /*11:0 , TX Q Signed Data for DC offset*/
        U32 tx_fix_q_dcofst_reserved_0     :4 ; /*15:12, RESERVED                      */
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
} T_BT_MODEM_tx_fix_q_dcofst;

//tx_i_dcofst_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 dc_cal_tx_idata                :12; /*11:0 , TX I Signed Data offset in use*/
        U32 tx_i_dcofst_inuse_reserved_0   :4 ; /*15:12, RESERVED                      */
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
} T_BT_MODEM_tx_i_dcofst_inuse;

//tx_q_dcofst_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 dc_cal_tx_qdata                :12; /*11:0 , TX Q Signed Data offset in use*/
        U32 tx_q_dcofst_inuse_reserved_0   :4 ; /*15:12, RESERVED                      */
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
} T_BT_MODEM_tx_q_dcofst_inuse;

//rssi_gain_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain0001              :7 ; /*6 :0 , ARSSI gain 0001               */
        U32 rssi_gain_ctrl1_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain0000              :7 ; /*14:8 , RSSI gain 0000                */
        U32 rssi_gain_ctrl1_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl1;

//rssi_gain_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain0011              :7 ; /*6 :0 , ARSSI gain 0011               */
        U32 rssi_gain_ctrl2_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain0010              :7 ; /*14:8 , RSSI gain 0010                */
        U32 rssi_gain_ctrl2_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl2;

//rssi_gain_ctrl3
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain0101              :7 ; /*6 :0 , ARSSI gain 0101               */
        U32 rssi_gain_ctrl3_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain0100              :7 ; /*14:8 , RSSI gain 0100                */
        U32 rssi_gain_ctrl3_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl3;

//rssi_gain_ctrl4
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain0111              :7 ; /*6 :0 , ARSSI gain 0111               */
        U32 rssi_gain_ctrl4_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain0110              :7 ; /*14:8 , RSSI gain 0110                */
        U32 rssi_gain_ctrl4_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl4;

//rssi_gain_ctrl5
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain1001              :7 ; /*6 :0 , ARSSI gain 1001               */
        U32 rssi_gain_ctrl5_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain1000              :7 ; /*14:8 , RSSI gain 1000                */
        U32 rssi_gain_ctrl5_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl5;

//rssi_gain_ctrl6
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain1011              :7 ; /*6 :0 , ARSSI gain 1011               */
        U32 rssi_gain_ctrl6_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain1010              :7 ; /*14:8 , RSSI gain 1010                */
        U32 rssi_gain_ctrl6_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl6;

//rssi_gain_ctrl7
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain1101              :7 ; /*6 :0 , ARSSI gain 1101               */
        U32 rssi_gain_ctrl7_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain1100              :7 ; /*14:8 , RSSI gain 1100                */
        U32 rssi_gain_ctrl7_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl7;

//rssi_gain_ctrl8
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_ana_gain1111              :7 ; /*6 :0 , ARSSI gain 1111               */
        U32 rssi_gain_ctrl8_reserved_1     :1 ; /*7 :7 , RESERVED                      */
        U32 rssi_ana_gain1110              :7 ; /*14:8 , RSSI gain 1110                */
        U32 rssi_gain_ctrl8_reserved_0     :1 ; /*15:15, RESERVED                      */
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_rssi_gain_ctrl8;

//modem_tpd_sel
typedef union
{
    U32 v;
    struct
    {
        U32 tpd_trig_sel                   :4 ; /*3 :0 , Test Ports Trigger Select4'h0::  dem_st_chg4'h1::  agc_st_chg4'h2::  agc_flg_dem4'h3::  ble_access_rb4'h4::  if_peak4'h5::  if_seeked_all4'h6::  seek_en4'h7::  flg_getsymbol4'h8::  tx_symbol_off_gfsk4'h9::  tx_amp_sel4'ha::  tx_flg_start*/
        U32 tpd_data_sel                   :4 ; /*7 :4 , Test Ports Data Select4'h0::  dac_data_i4'h1::  dac_data_q4'h2::  dout_tx_i_sum4'h3::  dout_tx_q_sum4'h4::  dout_tx_dac_i: depends on dac_data_sel4'h5::  dout_tx_dac_q4'h6::  dout_rx_dac_i4'h7::  dout_rx_dac_q4'h8::  dout_tx_dac_i_13m: by en_tx_13m4'h9::  dout_tx_dac_q_13m: by en_tx_13m4'ha::  dout_rx_dac_i_13m: by en_rx_13m4'hb::  dout_rx_dac_q_13m: by en_rx_13m4'hc::  dout_rx_dac_i_14m: by en_rx_14m4'hd::  dout_rx_dac_q_14m: by en_rx_14m4'he::  dout_tx_dac_i_26m: by en_tx_26m4'hf::  dout_tx_dac_q_26m: by en_tx_26m*/
        U32 tpd_clk_sel                    :1 ; /*8 :8 , Test Ports Clock Select0:: clk_rx1:: clk_tx */
        U32 dac_clk_force_en               :1 ; /*9 :9 , DAC Clock Force Enable while rx data to dac*/
        U32 modem_tpd_sel_reserved_0       :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_modem_tpd_sel;

//demod_smp_th_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 dem_sp_th1                     :8 ; /*7 :0 , Demod sample threshold1       */
        U32 dem_sp_th2                     :8 ; /*15:8 , Demod sample threshold2       */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_demod_smp_th_ctrl;

//newpacket_byte4
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_4                    :16; /*15:0 , The 4th byte newpacket for demod when sel_sync(register_41[13]) is 1*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte4;

//newpacket_byte3
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_3                    :16; /*15:0 , The 3rd byte newpacket for demod when sel_sync(register_41[13]) is 1*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte3;

//newpacket_byte2
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_2                    :16; /*15:0 , The 2nd byte newpacket for demod when sel_sync(register_41[13]) is 1*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte2;

//newpacket_byte1
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_1                    :16; /*15:0 , The 1st byte newpacket for demod when sel_sync(register_41[13]) is 1*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte1;

//gfsk_mod_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a2_dem                     :10; /*9 :0 , GFSK demod a2 reference for rx demod*/
        U32 gfsk_mod_ctrl_reserved_0       :2 ; /*11:10, RESERVED                      */
        U32 rssi_ct_u                      :3 ; /*14:12, RSSI gain control             */
        U32 bt_mod                         :1 ; /*15:15, Bluetooth GFSK modulation filter select*/
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
} T_BT_MODEM_gfsk_mod_ctrl;

//gfsk_mod_ref_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a3_dem                     :8 ; /*7 :0 , GFSK demod a3 reference for rx demod*/
        U32 ref_a1_dem                     :8 ; /*15:8 , GFSK demod a1 reference for rx demod*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_mod_ref_ctrl;

//sym_dly_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 delay_gfsk_1m                  :3 ; /*2 :0 , GFSK symbol delay, with 1MHz clk step*/
        U32 delay_dpsk_1m                  :3 ; /*5 :3 , DPSK symbol delay, with 1MHz clk step*/
        U32 delay_gfsk_off_1m              :3 ; /*8 :6 , GFSK symbol end flag delay, with 1MHz clk step*/
        U32 delay_gfsk_off                 :4 ; /*12:9 , GFSK symbol end flag delay, with 13MHz clk step*/
        U32 ramp_speed_gfsk                :1 ; /*13:13, GFSK ramp speed select1'h0:: Slow1'h1:: Fast*/
        U32 sym_dly_ctrl_reserved_0        :2 ; /*15:14, RESERVED                      */
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
} T_BT_MODEM_sym_dly_ctrl;

//rssi_out2
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_noise_out                 :10; /*9 :0 , RSSI noise out                */
        U32 rssi_out2_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_rssi_out2;

//trx_clk_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 trx_clk_ctrl_reserved_2        :6 ; /*5 :0 , RESERVED                      */
        U32 apc_switch_mode                :4 ; /*9 :6 , RESERVED                      */
        U32 trx_clk_ctrl_reserved_1        :2 ; /*11:10, RESERVED                      */
        U32 tx_rx_reg                      :1 ; /*12:12, TX/RX flag1'h0:: RX1'h1:: TX*/
        U32 tx_rx_dir                      :1 ; /*13:13, TX/RX direction1'h0:: by_hw: TX/RX flag setting by deleying resetn_dsp_tx1'h1:: by_reg: TX/RX flag setting by register*/
        U32 swch_clk_52m_rx                :1 ; /*14:14, Switch the clk edge to sample rf ADC data1'h0:: negedge: to sample the RF ADC data1'h1:: posedge: to sample the RF ADC data*/
        U32 trx_clk_ctrl_reserved_0        :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_trx_clk_ctrl;

//iq_swap_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 notch_en_0                     :1 ; /*0 :0 , Notch Filter Enable in Channel Group01'h0:: bypass1'h1:: enable*/
        U32 rate_conv_en_0                 :1 ; /*1 :1 , Rate Converter Enable in Channel Group01'h0:: bypass1'h1:: enable*/
        U32 lpf_en_0                       :1 ; /*2 :2 , Low Pass Filter Enable in Channel Group01'h0:: bypass1'h1:: enable*/
        U32 iq_swap_ctrl_reserved_1        :1 ; /*3 :3 , RESERVED                      */
        U32 notch_en_1                     :1 ; /*4 :4 , Notch Filter Enable in Channel Group11'h0:: bypass1'h1:: enable*/
        U32 rate_conv_en_1                 :1 ; /*5 :5 , Rate Converter Enable in Channel Group11'h0:: bypass1'h1:: enable*/
        U32 lpf_en_1                       :1 ; /*6 :6 , Low Pass Filter Enable in Channel Group11'h0:: bypass1'h1:: enable*/
        U32 iq_swap_ctrl_reserved_0        :5 ; /*11:7 , RESERVED                      */
        U32 iq_swap_srrc                   :1 ; /*12:12, SRRC filter output I/Q swap1'h0:: Not_Swap1'h1:: Swap*/
        U32 iq_swap_lpf                    :1 ; /*13:13, Rate converter LPF filter output I/Q swap1'h0:: Not_Swap1'h1:: Swap*/
        U32 iq_swap_gain2                  :1 ; /*14:14, Digital gain2 output I/Q swap1'h0:: Not_Swap1'h1:: Swap*/
        U32 swch_clk_52m_tx                :1 ; /*15:15, TX link 52M clk edge switch1'h0:: Not_Switch1'h1:: Switch*/
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_iq_swap_ctrl;

//gfsk_sync_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 dynamic_sync_th                :10; /*9 :0 , Dynamic sync threshold        */
        U32 dynamic_sync_en                :1 ; /*10:10, Dynamic sync enable for demod of rx link1'h0:: static1'h1:: Dynamic*/
        U32 gfsk_sync_ctrl_reserved_0      :5 ; /*15:11, RESERVED                      */
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
} T_BT_MODEM_gfsk_sync_ctrl;

//gfsk_demod_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 min_error_th2                  :16; /*15:0 , The 2nd minimum sync phase error threshold*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_demod_ctrl;

//gfsk_mod_idx
typedef union
{
    U32 v;
    struct
    {
        U32 h_gain                         :16; /*15:0 , GFSK modulation index         */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_mod_idx;

//dpsk_gfsk_misc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 dpsk_gfsk_misc_ctrl_reserved_1 :3 ; /*2 :0 , RESERVED                      */
        U32 dly_ct_amp                     :3 ; /*5 :3 , DPSK amplitude delay after dpsk modulation*/
        U32 dly_ct_dpsk                    :3 ; /*8 :6 , DPSK delay after dpsk modulation*/
        U32 dly_ct_gfsk                    :3 ; /*11:9 , GFSK delay after gfsk modulation*/
        U32 dpsk_gfsk_misc_ctrl_reserved_0 :3 ; /*14:12, RESERVED                      */
        U32 iq_swap_tx                     :1 ; /*15:15, Tx link IQ swap1'h0:: Not_swap1'h1:: Swap*/
        U32 reserved_2                     :1 ; /*16:16,                               */
        U32 reserved_3                     :1 ; /*17:17,                               */
        U32 reserved_4                     :1 ; /*18:18,                               */
        U32 reserved_5                     :1 ; /*19:19,                               */
        U32 reserved_6                     :1 ; /*20:20,                               */
        U32 reserved_7                     :1 ; /*21:21,                               */
        U32 reserved_8                     :1 ; /*22:22,                               */
        U32 reserved_9                     :1 ; /*23:23,                               */
        U32 reserved_10                    :1 ; /*24:24,                               */
        U32 reserved_11                    :1 ; /*25:25,                               */
        U32 reserved_12                    :1 ; /*26:26,                               */
        U32 reserved_13                    :1 ; /*27:27,                               */
        U32 reserved_14                    :1 ; /*28:28,                               */
        U32 reserved_15                    :1 ; /*29:29,                               */
        U32 reserved_16                    :1 ; /*30:30,                               */
        U32 reserved_17                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_dpsk_gfsk_misc_ctrl;

//modem_dbm_sel
typedef union
{
    U32 v;
    struct
    {
        U32 dbm_data_sel                   :5 ; /*4 :0 , Debug Master Data Select5'h0::  gfilter_tx_dout5'h1::  symbol2iq_tx_dout_q: symbol2iq_tx_dout_i5'h2::  cordic_tx_amp_dout: cordic_tx_angle_dout5'h3::  ampm_tx_dout_am: ampm_tx_dout_pm5'h4::  diff_tx_dout5'h5::  freq_blend_tx_dout5'h6::  intigrate_tx_dout5'h7::  cordic_iq_tx_dout_q: cordic_iq_tx_dout_i5'h8::  dout_tx_13m_q: dout_tx_13m_i5'h9::  iqim_cancel_dout_q: iqim_cancel_dout_i5'ha::  dout_tx_26m_q: dout_tx_26m_i5'hb::  dout_tx_52m_q: dout_tx_52m_i5'hc:: dac_grp_bit_q_outp: dac_grp_bit_i_outp5'h10:: adc_data_q: adc_data_i5'h11:: adc_din_q: adc_din_i5'h12:: lpf_q: lpf_i5'h13:: rateconv_q: rateconv_i5'h14:: calib_q: calib_i5'h15:: dc_calib_q: dc_calib_i5'h16:: cancel_flt_i: cancel_flt_q5'h17:: notch_q: notch_i5'h18:: gain_q: gain_i5'h19:: ble_mux_q: ble_mux_i5'h1a:: mixer_q: mixer_i5'h1b:: srrc_q: srrc_i5'h1c:: rssi_out5'h1d:: angle_rc: angle5'h1e:: angle_offset1: angle_offset5'h1f:: err_dpsk: err_gfsk*/
        U32 modem_dbm_sel_reserved_0       :11; /*15:5 , RESERVED                      */
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
} T_BT_MODEM_modem_dbm_sel;

//gfsk_mod_idx_le
typedef union
{
    U32 v;
    struct
    {
        U32 h_gain_le                      :16; /*15:0 , GFSK modulation index for BLE mode*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_gfsk_mod_idx_le;

//newpacket_byte4_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_dsp4                 :16; /*15:0 , newpacket byte 4 inuse; selected from newpacket_reg, GID &amp; newpacket_bb*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte4_inuse;

//newpacket_byte3_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_dsp3                 :16; /*15:0 , newpacket byte 3 inuse; selected from newpacket_reg, GID &amp; newpacket_bb*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte3_inuse;

//newpacket_byte2_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_dsp2                 :16; /*15:0 , newpacket byte 2 inuse; selected from newpacket_reg, GID &amp; newpacket_bb*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte2_inuse;

//newpacket_byte1_inuse
typedef union
{
    U32 v;
    struct
    {
        U32 newpacket_dsp1                 :16; /*15:0 , newpacket byte 1 inuse; selected from newpacket_reg, GID &amp; newpacket_bb*/
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_newpacket_byte1_inuse;

//le_mode_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a1_dem_le                  :8 ; /*7 :0 , ??                            */
        U32 ref_a1_sek_le                  :8 ; /*15:8 , ??                            */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_le_mode_ctrl1;

//le_mode_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a2_dem_le                  :8 ; /*7 :0 , ??                            */
        U32 ref_a2_sek_le                  :8 ; /*15:8 , ??                            */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_le_mode_ctrl2;

//le_mode_ctrl3
typedef union
{
    U32 v;
    struct
    {
        U32 ref_a3_dem_le                  :8 ; /*7 :0 , ??                            */
        U32 ref_a3_sek_le                  :8 ; /*15:8 , ??                            */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_le_mode_ctrl3;

//le_mode_ctrl4
typedef union
{
    U32 v;
    struct
    {
        U32 min_error_th_le                :16; /*15:0 , ??                            */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_le_mode_ctrl4;

//le_mode_ctrl5
typedef union
{
    U32 v;
    struct
    {
        U32 ref_ready_th_le                :6 ; /*5 :0 , ??                            */
        U32 mix_guard_th_le                :6 ; /*11:6 , ??                            */
        U32 ref_point_sel_le               :2 ; /*13:12, ??                            */
        U32 rsvd5                          :2 ; /*15:14, RESERVED                      */
        U32 reserved_0                     :1 ; /*16:16,                               */
        U32 reserved_1                     :1 ; /*17:17,                               */
        U32 reserved_2                     :1 ; /*18:18,                               */
        U32 reserved_3                     :1 ; /*19:19,                               */
        U32 reserved_4                     :1 ; /*20:20,                               */
        U32 reserved_5                     :1 ; /*21:21,                               */
        U32 reserved_6                     :1 ; /*22:22,                               */
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_BT_MODEM_le_mode_ctrl5;

//rf_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 sync_shift_mode                :1 ; /*0 :0 , ??                            */
        U32 rf_ctrl_reserved_0             :15; /*15:1 , RESERVED                      */
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
} T_BT_MODEM_rf_ctrl;

//tx_q_im
typedef union
{
    U32 v;
    struct
    {
        U32 tx_iqim_phase_err              :15; /*14:0 , Error on Q to reduce IQ mismatch Image*/
        U32 tx_q_im_reserved_0             :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_tx_q_im;

//tx_i_im
typedef union
{
    U32 v;
    struct
    {
        U32 tx_iqim_amp_err                :15; /*14:0 , Error on I to reduce IQ mismatch Image*/
        U32 tx_i_im_reserved_0             :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_tx_i_im;

//tx_am_p0
typedef union
{
    U32 v;
    struct
    {
        U32 am_p0                          :10; /*9 :0 , AMAM Compensation Coef0       */
        U32 am_comp_bypass                 :1 ; /*10:10, AM Compensation Bypass1'b0:: enable1'b1:: bypass*/
        U32 pm_comp_bypass                 :1 ; /*11:11, PM Compensation Bypass1'b0:: enable1'b1:: bypass*/
        U32 pm_shift                       :3 ; /*14:12, PM Compensation Shift         */
        U32 tx_am_p0_reserved_0            :1 ; /*15:15, RESERVED                      */
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
} T_BT_MODEM_tx_am_p0;

//tx_am_p1
typedef union
{
    U32 v;
    struct
    {
        U32 am_p1                          :10; /*9 :0 , AMAM Compensation Coef1       */
        U32 tx_am_p1_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p1;

//tx_am_p2
typedef union
{
    U32 v;
    struct
    {
        U32 am_p2                          :10; /*9 :0 , AMAM Compensation Coef2       */
        U32 tx_am_p2_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p2;

//tx_am_p3
typedef union
{
    U32 v;
    struct
    {
        U32 am_p3                          :10; /*9 :0 , AMAM Compensation Coef3       */
        U32 tx_am_p3_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p3;

//tx_am_p4
typedef union
{
    U32 v;
    struct
    {
        U32 am_p4                          :10; /*9 :0 , AMAM Compensation Coef4       */
        U32 tx_am_p4_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p4;

//tx_am_p5
typedef union
{
    U32 v;
    struct
    {
        U32 am_p5                          :10; /*9 :0 , AMAM Compensation Coef5       */
        U32 tx_am_p5_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p5;

//tx_am_p6
typedef union
{
    U32 v;
    struct
    {
        U32 am_p6                          :10; /*9 :0 , AMAM Compensation Coef6       */
        U32 tx_am_p6_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p6;

//tx_am_p7
typedef union
{
    U32 v;
    struct
    {
        U32 am_p7                          :10; /*9 :0 , AMAM Compensation Coef7       */
        U32 tx_am_p7_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p7;

//tx_am_p8
typedef union
{
    U32 v;
    struct
    {
        U32 am_p8                          :10; /*9 :0 , AMAM Compensation Coef8       */
        U32 tx_am_p8_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p8;

//tx_am_p9
typedef union
{
    U32 v;
    struct
    {
        U32 am_p9                          :10; /*9 :0 , AMAM Compensation Coef9       */
        U32 tx_am_p9_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p9;

//tx_am_p10
typedef union
{
    U32 v;
    struct
    {
        U32 am_p10                         :10; /*9 :0 , AMAM Compensation Coef10      */
        U32 tx_am_p10_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p10;

//tx_am_p11
typedef union
{
    U32 v;
    struct
    {
        U32 am_p11                         :10; /*9 :0 , AMAM Compensation Coef11      */
        U32 tx_am_p11_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p11;

//tx_am_p12
typedef union
{
    U32 v;
    struct
    {
        U32 am_p12                         :10; /*9 :0 , AMAM Compensation Coef12      */
        U32 tx_am_p12_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p12;

//tx_am_p13
typedef union
{
    U32 v;
    struct
    {
        U32 am_p13                         :10; /*9 :0 , AMAM Compensation Coef13      */
        U32 tx_am_p13_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p13;

//tx_am_p14
typedef union
{
    U32 v;
    struct
    {
        U32 am_p14                         :10; /*9 :0 , AMAM Compensation Coef14      */
        U32 tx_am_p14_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p14;

//tx_am_p15
typedef union
{
    U32 v;
    struct
    {
        U32 am_p15                         :10; /*9 :0 , AMAM Compensation Coef15      */
        U32 tx_am_p15_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p15;

//tx_am_p16
typedef union
{
    U32 v;
    struct
    {
        U32 am_p16                         :10; /*9 :0 , AMAM Compensation Coef16      */
        U32 tx_am_p16_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_am_p16;

//tx_pm_p0
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p0                          :10; /*9 :0 , AMPM Compensation Coef0       */
        U32 tx_pm_p0_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p0;

//tx_pm_p1
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p1                          :10; /*9 :0 , AMPM Compensation Coef1       */
        U32 tx_pm_p1_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p1;

//tx_pm_p2
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p2                          :10; /*9 :0 , AMPM Compensation Coef2       */
        U32 tx_pm_p2_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p2;

//tx_pm_p3
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p3                          :10; /*9 :0 , AMPM Compensation Coef3       */
        U32 tx_pm_p3_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p3;

//tx_pm_p4
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p4                          :10; /*9 :0 , AMPM Compensation Coef4       */
        U32 tx_pm_p4_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p4;

//tx_pm_p5
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p5                          :10; /*9 :0 , AMPM Compensation Coef5       */
        U32 tx_pm_p5_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p5;

//tx_pm_p6
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p6                          :10; /*9 :0 , AMPM Compensation Coef6       */
        U32 tx_pm_p6_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p6;

//tx_pm_p7
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p7                          :10; /*9 :0 , AMPM Compensation Coef7       */
        U32 tx_pm_p7_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p7;

//tx_pm_p8
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p8                          :10; /*9 :0 , AMPM Compensation Coef8       */
        U32 tx_pm_p8_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p8;

//tx_pm_p9
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p9                          :10; /*9 :0 , AMPM Compensation Coef9       */
        U32 tx_pm_p9_reserved_0            :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p9;

//tx_pm_p10
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p10                         :10; /*9 :0 , AMPM Compensation Coef10      */
        U32 tx_pm_p10_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p10;

//tx_pm_p11
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p11                         :10; /*9 :0 , AMPM Compensation Coef11      */
        U32 tx_pm_p11_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p11;

//tx_pm_p12
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p12                         :10; /*9 :0 , AMPM Compensation Coef12      */
        U32 tx_pm_p12_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p12;

//tx_pm_p13
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p13                         :10; /*9 :0 , AMPM Compensation Coef13      */
        U32 tx_pm_p13_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p13;

//tx_pm_p14
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p14                         :10; /*9 :0 , AMPM Compensation Coef14      */
        U32 tx_pm_p14_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p14;

//tx_pm_p15
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p15                         :10; /*9 :0 , AMPM Compensation Coef15      */
        U32 tx_pm_p15_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p15;

//tx_pm_p16
typedef union
{
    U32 v;
    struct
    {
        U32 pm_p16                         :10; /*9 :0 , AMPM Compensation Coef16      */
        U32 tx_pm_p16_reserved_0           :6 ; /*15:10, RESERVED                      */
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
} T_BT_MODEM_tx_pm_p16;

//notch_coef
typedef union
{
    U32 v;
    struct
    {
        U32 notch_b                        :14; /*13:0 , Notch Filter Coefficient B    */
        U32 notch_coef_reserved_0          :2 ; /*15:14, RESERVED                      */
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
} T_BT_MODEM_notch_coef;

//adapt_edr3_demod
typedef union
{
    U32 v;
    struct
    {
        U32 ct_u_1_dpsk1                   :5 ; /*4 :0 , second u_dc of the dpsk 2/512 */
        U32 ct_u_dpsk1                     :4 ; /*8 :5 , second u_err of the dpsk 2/32 */
        U32 edr3_adapt_en                  :1 ; /*9 :9 , EDR3 Adapt Demodulation Enable1'b0:: disable1'b1:: enable*/
        U32 adapt_edr3_demod_reserved_0    :2 ; /*11:10, RESERVED                      */
        U32 notch_a                        :4 ; /*15:12, Notch Filter Coefficient A    */
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
} T_BT_MODEM_adapt_edr3_demod;

//adapt_edr3_thresh
typedef union
{
    U32 v;
    struct
    {
        U32 edr3_adapt_th                  :12; /*11:0 , EDR3 Adapt Demodulation Threshold*/
        U32 adapt_edr3_thresh_reserved_0   :4 ; /*15:12, RESERVED                      */
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
} T_BT_MODEM_adapt_edr3_thresh;

typedef struct
{
    volatile          T_BT_MODEM_bypass_ctrl                    bypass_ctrl; /*0x0  , RW   , 0x00004000, Bypass Control Register       */
    volatile        T_BT_MODEM_sw_swap_dccal                  sw_swap_dccal; /*0x4  , RW   , 0x00001203, Switch,Swap,DC Calibration Control Register*/
    volatile      T_BT_MODEM_dem750_afc_freq                dem750_afc_freq; /*0x8  , RW   , 0x0000E2DB, Dem750 AFC Frequency Control Register*/
    volatile    T_BT_MODEM_dpsk_gfsk_tx_ctrl              dpsk_gfsk_tx_ctrl; /*0xc  , RW   , 0x0000FF48, DPSK/GFSK TX Control Register */
    volatile         T_BT_MODEM_tx_gain_ctrl                   tx_gain_ctrl; /*0x10 , RW   , 0x0000A1FF, TX Gain Control Register      */
    volatile            T_BT_MODEM_rssi_out1                      rssi_out1; /*0x14 , RO   , 0x00000000, RSSII Output Register         */
    volatile              T_BT_MODEM_sw_ctrl                        sw_ctrl; /*0x18 , RW   , 0x00000188, Swap Control Register         */
    volatile       T_BT_MODEM_adcclk_sw_ctrl                 adcclk_sw_ctrl; /*0x1c , RW   , 0x000055C9, Switch adc_clk, packet and demod ct_u Control Register*/
    volatile        T_BT_MODEM_dpsk_demod_sw                  dpsk_demod_sw; /*0x20 , RW   , 0x00003059, Switch DPSK demod ct_u Control Register*/
    volatile        T_BT_MODEM_min_phase_err                  min_phase_err; /*0x24 , RW   , 0x00000CF6, Gfsk Minimum Phase Error Control Register*/
    volatile             T_BT_MODEM_afc_ctrl                       afc_ctrl; /*0x28 , RW   , 0x0000A029, AFC Control Register          */
    volatile     T_BT_MODEM_dpsk_gfsk_smp_th               dpsk_gfsk_smp_th; /*0x2c , RW   , 0x0000000A, DPSK/GFSK Sample Threhold Control Register*/
    volatile   T_BT_MODEM_dpsk_gfsk_smp_th_1             dpsk_gfsk_smp_th_1; /*0x30 , RW   , 0x0000006F, DPSK/GFSK Sample Threhold Control Register*/
    volatile    T_BT_MODEM_gfsk_smp_ref_ctrl              gfsk_smp_ref_ctrl; /*0x34 , RW   , 0x00001919, GFSK Sample Refernece Control Register*/
    volatile  T_BT_MODEM_gfsk_smp_ref_ctrl_1            gfsk_smp_ref_ctrl_1; /*0x38 , RW   , 0x00004220, GFSK Sample Refernece Control Register*/
    volatile       T_BT_MODEM_rateconv_ctrl1                 rateconv_ctrl1; /*0x3c , RW   , 0x00004400, Rate Converter,DAC, DC Cancel ControlRegister*/
    volatile       T_BT_MODEM_rateconv_ctrl2                 rateconv_ctrl2; /*0x40 , RW   , 0x0000007C, Rate Converter Control Register*/
    volatile       T_BT_MODEM_demod_smp_ctrl                 demod_smp_ctrl; /*0x44 , RW   , 0x00000405, Demod Sample Control Register */
    volatile             T_BT_MODEM_agc_ctrl                       agc_ctrl; /*0x48 , RW   , 0x00000551, AGC Control Register          */
    volatile         T_BT_MODEM_agc_th_ctrl1                   agc_th_ctrl1; /*0x4c , RW   , 0x00006018, AGC Threshold Control Register 1*/
    volatile         T_BT_MODEM_agc_th_ctrl2                   agc_th_ctrl2; /*0x50 , RW   , 0x0000DC28, AGC Threshold Control Register 2*/
    volatile            T_BT_MODEM_agc_ctrl1                      agc_ctrl1; /*0x54 , RW   , 0x00002DB7, AGC Control Register 1        */
    volatile            T_BT_MODEM_agc_ctrl2                      agc_ctrl2; /*0x58 , RW   , 0x00004C30, AGC Control Register 2        */
    volatile         T_BT_MODEM_agc_dgc_ctrl                   agc_dgc_ctrl; /*0x5c , RW   , 0x00008804, AGC DGC Control Register      */
    volatile       T_BT_MODEM_agc_dccal_ctrl                 agc_dccal_ctrl; /*0x60 , RW   , 0x00003A82, AGC &amp; DC Calibration Control Register*/
    volatile        T_BT_MODEM_rx_dccal_ctrl                  rx_dccal_ctrl; /*0x64 , RW   , 0x00000000, RX DC Calibration Control Register*/
    volatile      T_BT_MODEM_rx_dcofst_inuse                rx_dcofst_inuse; /*0x68 , RO   , 0x00000000, RX DC Offset Inuse Register   */
    volatile          T_BT_MODEM_tx_dc_calib                    tx_dc_calib; /*0x6c , RW   , 0x0000092E, TX DC Calibration Register    */
    volatile      T_BT_MODEM_tx_fix_i_dcofst                tx_fix_i_dcofst; /*0x70 , RW   , 0x00000000, TX Idata Fix DC Offset Register*/
    volatile      T_BT_MODEM_tx_fix_q_dcofst                tx_fix_q_dcofst; /*0x74 , RW   , 0x00000000, TX Qdata Fix DC Offset Register*/
    volatile    T_BT_MODEM_tx_i_dcofst_inuse              tx_i_dcofst_inuse; /*0x78 , RO   , 0x00000000, TX Idata DC Offset  Inuse Register*/
    volatile    T_BT_MODEM_tx_q_dcofst_inuse              tx_q_dcofst_inuse; /*0x7c , RO   , 0x00000000, TX Qdata DC Offset Inuse Register*/
    volatile      T_BT_MODEM_rssi_gain_ctrl1                rssi_gain_ctrl1; /*0x80 , RW   , 0x00000913, RSSI Gain Control Register 1  */
    volatile      T_BT_MODEM_rssi_gain_ctrl2                rssi_gain_ctrl2; /*0x84 , RW   , 0x00001B23, RSSI Gain Control Register 2  */
    volatile      T_BT_MODEM_rssi_gain_ctrl3                rssi_gain_ctrl3; /*0x88 , RW   , 0x00002931, RSSI Gain Control Register 3  */
    volatile      T_BT_MODEM_rssi_gain_ctrl4                rssi_gain_ctrl4; /*0x8c , RW   , 0x0000373F, RSSI Gain Control Register 4  */
    volatile      T_BT_MODEM_rssi_gain_ctrl5                rssi_gain_ctrl5; /*0x90 , RW   , 0x00000913, RSSI Gain Control Register 5  */
    volatile      T_BT_MODEM_rssi_gain_ctrl6                rssi_gain_ctrl6; /*0x94 , RW   , 0x00001B23, RSSI Gain Control Register 6  */
    volatile      T_BT_MODEM_rssi_gain_ctrl7                rssi_gain_ctrl7; /*0x98 , RW   , 0x00002931, RSSI Gain Control Register 7  */
    volatile      T_BT_MODEM_rssi_gain_ctrl8                rssi_gain_ctrl8; /*0x9c , RW   , 0x0000373F, RSSI Gain Control Register 8  */
    volatile        T_BT_MODEM_modem_tpd_sel                  modem_tpd_sel; /*0xa0 , RW   , 0x00000000, Test Port Selection Register  */
    volatile    T_BT_MODEM_demod_smp_th_ctrl              demod_smp_th_ctrl; /*0xa4 , RW   , 0x0000A240, Demod Sample Threhold Control Register*/
    volatile      T_BT_MODEM_newpacket_byte4                newpacket_byte4; /*0xa8 , RW   , 0x0000C000, Newpacket Byte4 Register      */
    volatile      T_BT_MODEM_newpacket_byte3                newpacket_byte3; /*0xac , RW   , 0x00006666, Newpacket Byte3 Register      */
    volatile      T_BT_MODEM_newpacket_byte2                newpacket_byte2; /*0xb0 , RW   , 0x00003333, Newpacket Byte2 Register      */
    volatile      T_BT_MODEM_newpacket_byte1                newpacket_byte1; /*0xb4 , RW   , 0x00005555, Newpacket Byte1 Register      */
    volatile        T_BT_MODEM_gfsk_mod_ctrl                  gfsk_mod_ctrl; /*0xb8 , RW   , 0x0000C075, GFSK Modulation and RSSI Control Register*/
    volatile    T_BT_MODEM_gfsk_mod_ref_ctrl              gfsk_mod_ref_ctrl; /*0xbc , RW   , 0x00001616, GFSK Modulation Reference Control Register*/
    volatile         T_BT_MODEM_sym_dly_ctrl                   sym_dly_ctrl; /*0xc0 , RW   , 0x00001100, GFSK/DPSK Symbol Delay and Test Control Register*/
    volatile            T_BT_MODEM_rssi_out2                      rssi_out2; /*0xc4 , RO   , 0x00000000, RSSI Noise Out Register       */
    volatile         T_BT_MODEM_trx_clk_ctrl                   trx_clk_ctrl; /*0xc8 , RW   , 0x000000C0, TX/RX and Demod Clock Control Register*/
    volatile         T_BT_MODEM_iq_swap_ctrl                   iq_swap_ctrl; /*0xcc , RW   , 0x00008074, IQ Swap Control Register      */
    volatile       T_BT_MODEM_gfsk_sync_ctrl                 gfsk_sync_ctrl; /*0xd0 , RW   , 0x00000000, GFSK Sync Control Register    */
    volatile      T_BT_MODEM_gfsk_demod_ctrl                gfsk_demod_ctrl; /*0xd4 , RW   , 0x00000000, GFSK DemodControl Register    */
    volatile         T_BT_MODEM_gfsk_mod_idx                   gfsk_mod_idx; /*0xd8 , RW   , 0x000031A0, GFSK Modulation Index Register*/
    volatile  T_BT_MODEM_dpsk_gfsk_misc_ctrl            dpsk_gfsk_misc_ctrl; /*0xdc , RW   , 0x00000C18, DPSK/GFSK Misc Control  Register*/
    volatile        T_BT_MODEM_modem_dbm_sel                  modem_dbm_sel; /*0xe0 , RW   , 0x00000000, Modem DBM Select Register     */
    volatile      T_BT_MODEM_gfsk_mod_idx_le                gfsk_mod_idx_le; /*0xe4 , RW   , 0x00004EC5, GFSK BLE Modulation Index Register*/
    volatile T_BT_MODEM_newpacket_byte4_inuse          newpacket_byte4_inuse; /*0xe8 , RO   , 0x00007E70, Newpacket Byte4 Inuse Register*/
    volatile T_BT_MODEM_newpacket_byte3_inuse          newpacket_byte3_inuse; /*0xec , RO   , 0x000041E3, Newpacket Byte3 Inuse Register*/
    volatile T_BT_MODEM_newpacket_byte2_inuse          newpacket_byte2_inuse; /*0xf0 , RO   , 0x00004000, Newpacket Byte2 Inuse Register*/
    volatile T_BT_MODEM_newpacket_byte1_inuse          newpacket_byte1_inuse; /*0xf4 , RO   , 0x0000000D, Newpacket Byte1 Inuse Register*/
    volatile        T_BT_MODEM_le_mode_ctrl1                  le_mode_ctrl1; /*0xf8 , RW   , 0x00002823, Le Mode Control Register1     */
    volatile        T_BT_MODEM_le_mode_ctrl2                  le_mode_ctrl2; /*0xfc , RW   , 0x0000B0BA, Le Mode Control Register2     */
    volatile        T_BT_MODEM_le_mode_ctrl3                  le_mode_ctrl3; /*0x100, RW   , 0x00002823, Le Mode Control Register3     */
    volatile        T_BT_MODEM_le_mode_ctrl4                  le_mode_ctrl4; /*0x104, RW   , 0x0000091E, Le Mode Control Register4     */
    volatile        T_BT_MODEM_le_mode_ctrl5                  le_mode_ctrl5; /*0x108, RW   , 0x00002103, Le Mode Control Register5     */
    volatile              T_BT_MODEM_rf_ctrl                        rf_ctrl; /*0x10c, RW   , 0x00000000, RFControl Register            */
    volatile              T_BT_MODEM_tx_q_im                        tx_q_im; /*0x110, RW   , 0x00000000, TX Q Image Register           */
    volatile              T_BT_MODEM_tx_i_im                        tx_i_im; /*0x114, RW   , 0x00000000, TX I Image Register           */
    volatile             T_BT_MODEM_tx_am_p0                       tx_am_p0; /*0x118, RW   , 0x00000C00, TX AM Comp Register0          */
    volatile             T_BT_MODEM_tx_am_p1                       tx_am_p1; /*0x11c, RW   , 0x00000000, TX AM Comp Register1          */
    volatile             T_BT_MODEM_tx_am_p2                       tx_am_p2; /*0x120, RW   , 0x00000000, TX AM Comp Register2          */
    volatile             T_BT_MODEM_tx_am_p3                       tx_am_p3; /*0x124, RW   , 0x00000000, TX AM Comp Register3          */
    volatile             T_BT_MODEM_tx_am_p4                       tx_am_p4; /*0x128, RW   , 0x00000000, TX AM Comp Register4          */
    volatile             T_BT_MODEM_tx_am_p5                       tx_am_p5; /*0x12c, RW   , 0x00000000, TX AM Comp Register5          */
    volatile             T_BT_MODEM_tx_am_p6                       tx_am_p6; /*0x130, RW   , 0x00000000, TX AM Comp Register6          */
    volatile             T_BT_MODEM_tx_am_p7                       tx_am_p7; /*0x134, RW   , 0x00000000, TX AM Comp Register7          */
    volatile             T_BT_MODEM_tx_am_p8                       tx_am_p8; /*0x138, RW   , 0x00000000, TX AM Comp Register8          */
    volatile             T_BT_MODEM_tx_am_p9                       tx_am_p9; /*0x13c, RW   , 0x00000000, TX AM Comp Register9          */
    volatile            T_BT_MODEM_tx_am_p10                      tx_am_p10; /*0x140, RW   , 0x00000000, TX AM Comp Register10         */
    volatile            T_BT_MODEM_tx_am_p11                      tx_am_p11; /*0x144, RW   , 0x00000000, TX AM Comp Register11         */
    volatile            T_BT_MODEM_tx_am_p12                      tx_am_p12; /*0x148, RW   , 0x00000000, TX AM Comp Register12         */
    volatile            T_BT_MODEM_tx_am_p13                      tx_am_p13; /*0x14c, RW   , 0x00000000, TX AM Comp Register13         */
    volatile            T_BT_MODEM_tx_am_p14                      tx_am_p14; /*0x150, RW   , 0x00000000, TX AM Comp Register14         */
    volatile            T_BT_MODEM_tx_am_p15                      tx_am_p15; /*0x154, RW   , 0x00000000, TX AM Comp Register15         */
    volatile            T_BT_MODEM_tx_am_p16                      tx_am_p16; /*0x158, RW   , 0x00000000, TX AM Comp Register16         */
    volatile             T_BT_MODEM_tx_pm_p0                       tx_pm_p0; /*0x15c, RW   , 0x00000000, TX PM Comp Register0          */
    volatile             T_BT_MODEM_tx_pm_p1                       tx_pm_p1; /*0x160, RW   , 0x00000000, TX PM Comp Register1          */
    volatile             T_BT_MODEM_tx_pm_p2                       tx_pm_p2; /*0x164, RW   , 0x00000000, TX PM Comp Register2          */
    volatile             T_BT_MODEM_tx_pm_p3                       tx_pm_p3; /*0x168, RW   , 0x00000000, TX PM Comp Register3          */
    volatile             T_BT_MODEM_tx_pm_p4                       tx_pm_p4; /*0x16c, RW   , 0x00000000, TX PM Comp Register4          */
    volatile             T_BT_MODEM_tx_pm_p5                       tx_pm_p5; /*0x170, RW   , 0x00000000, TX PM Comp Register5          */
    volatile             T_BT_MODEM_tx_pm_p6                       tx_pm_p6; /*0x174, RW   , 0x00000000, TX PM Comp Register6          */
    volatile             T_BT_MODEM_tx_pm_p7                       tx_pm_p7; /*0x178, RW   , 0x00000000, TX PM Comp Register7          */
    volatile             T_BT_MODEM_tx_pm_p8                       tx_pm_p8; /*0x17c, RW   , 0x00000000, TX PM Comp Register8          */
    volatile             T_BT_MODEM_tx_pm_p9                       tx_pm_p9; /*0x180, RW   , 0x00000000, TX PM Comp Register9          */
    volatile            T_BT_MODEM_tx_pm_p10                      tx_pm_p10; /*0x184, RW   , 0x00000000, TX PM Comp Register10         */
    volatile            T_BT_MODEM_tx_pm_p11                      tx_pm_p11; /*0x188, RW   , 0x00000000, TX PM Comp Register11         */
    volatile            T_BT_MODEM_tx_pm_p12                      tx_pm_p12; /*0x18c, RW   , 0x00000000, TX PM Comp Register12         */
    volatile            T_BT_MODEM_tx_pm_p13                      tx_pm_p13; /*0x190, RW   , 0x00000000, TX PM Comp Register13         */
    volatile            T_BT_MODEM_tx_pm_p14                      tx_pm_p14; /*0x194, RW   , 0x00000000, TX PM Comp Register14         */
    volatile            T_BT_MODEM_tx_pm_p15                      tx_pm_p15; /*0x198, RW   , 0x00000000, TX PM Comp Register15         */
    volatile            T_BT_MODEM_tx_pm_p16                      tx_pm_p16; /*0x19c, RW   , 0x00000000, TX PM Comp Register16         */
    volatile           T_BT_MODEM_notch_coef                     notch_coef; /*0x1a0, RW   , 0x00003BF3, Notch Filter Coefficient Register*/
    volatile     T_BT_MODEM_adapt_edr3_demod               adapt_edr3_demod; /*0x1a4, RW   , 0x0000E05A, Adapt EDR3 Demod Register     */
    volatile    T_BT_MODEM_adapt_edr3_thresh              adapt_edr3_thresh; /*0x1a8, RW   , 0x00000333, Adapt EDR3 Demod Register     */
} T_HWP_BT_MODEM_T;
#else
typedef struct
{
    volatile                             U32                    bypass_ctrl; /*0x0  , RW   , 0x00004000, Bypass Control Register       */
    volatile                             U32                  sw_swap_dccal; /*0x4  , RW   , 0x00001203, Switch,Swap,DC Calibration Control Register*/
    volatile                             U32                dem750_afc_freq; /*0x8  , RW   , 0x0000E2DB, Dem750 AFC Frequency Control Register*/
    volatile                             U32              dpsk_gfsk_tx_ctrl; /*0xc  , RW   , 0x0000FF48, DPSK/GFSK TX Control Register */
    volatile                             U32                   tx_gain_ctrl; /*0x10 , RW   , 0x0000A1FF, TX Gain Control Register      */
    volatile                             U32                      rssi_out1; /*0x14 , RO   , 0x00000000, RSSII Output Register         */
    volatile                             U32                        sw_ctrl; /*0x18 , RW   , 0x00000188, Swap Control Register         */
    volatile                             U32                 adcclk_sw_ctrl; /*0x1c , RW   , 0x000055C9, Switch adc_clk, packet and demod ct_u Control Register*/
    volatile                             U32                  dpsk_demod_sw; /*0x20 , RW   , 0x00003059, Switch DPSK demod ct_u Control Register*/
    volatile                             U32                  min_phase_err; /*0x24 , RW   , 0x00000CF6, Gfsk Minimum Phase Error Control Register*/
    volatile                             U32                       afc_ctrl; /*0x28 , RW   , 0x0000A029, AFC Control Register          */
    volatile                             U32               dpsk_gfsk_smp_th; /*0x2c , RW   , 0x0000000A, DPSK/GFSK Sample Threhold Control Register*/
    volatile                             U32             dpsk_gfsk_smp_th_1; /*0x30 , RW   , 0x0000006F, DPSK/GFSK Sample Threhold Control Register*/
    volatile                             U32              gfsk_smp_ref_ctrl; /*0x34 , RW   , 0x00001919, GFSK Sample Refernece Control Register*/
    volatile                             U32            gfsk_smp_ref_ctrl_1; /*0x38 , RW   , 0x00004220, GFSK Sample Refernece Control Register*/
    volatile                             U32                 rateconv_ctrl1; /*0x3c , RW   , 0x00004400, Rate Converter,DAC, DC Cancel ControlRegister*/
    volatile                             U32                 rateconv_ctrl2; /*0x40 , RW   , 0x0000007C, Rate Converter Control Register*/
    volatile                             U32                 demod_smp_ctrl; /*0x44 , RW   , 0x00000405, Demod Sample Control Register */
    volatile                             U32                       agc_ctrl; /*0x48 , RW   , 0x00000551, AGC Control Register          */
    volatile                             U32                   agc_th_ctrl1; /*0x4c , RW   , 0x00006018, AGC Threshold Control Register 1*/
    volatile                             U32                   agc_th_ctrl2; /*0x50 , RW   , 0x0000DC28, AGC Threshold Control Register 2*/
    volatile                             U32                      agc_ctrl1; /*0x54 , RW   , 0x00002DB7, AGC Control Register 1        */
    volatile                             U32                      agc_ctrl2; /*0x58 , RW   , 0x00004C30, AGC Control Register 2        */
    volatile                             U32                   agc_dgc_ctrl; /*0x5c , RW   , 0x00008804, AGC DGC Control Register      */
    volatile                             U32                 agc_dccal_ctrl; /*0x60 , RW   , 0x00003A82, AGC &amp; DC Calibration Control Register*/
    volatile                             U32                  rx_dccal_ctrl; /*0x64 , RW   , 0x00000000, RX DC Calibration Control Register*/
    volatile                             U32                rx_dcofst_inuse; /*0x68 , RO   , 0x00000000, RX DC Offset Inuse Register   */
    volatile                             U32                    tx_dc_calib; /*0x6c , RW   , 0x0000092E, TX DC Calibration Register    */
    volatile                             U32                tx_fix_i_dcofst; /*0x70 , RW   , 0x00000000, TX Idata Fix DC Offset Register*/
    volatile                             U32                tx_fix_q_dcofst; /*0x74 , RW   , 0x00000000, TX Qdata Fix DC Offset Register*/
    volatile                             U32              tx_i_dcofst_inuse; /*0x78 , RO   , 0x00000000, TX Idata DC Offset  Inuse Register*/
    volatile                             U32              tx_q_dcofst_inuse; /*0x7c , RO   , 0x00000000, TX Qdata DC Offset Inuse Register*/
    volatile                             U32                rssi_gain_ctrl1; /*0x80 , RW   , 0x00000913, RSSI Gain Control Register 1  */
    volatile                             U32                rssi_gain_ctrl2; /*0x84 , RW   , 0x00001B23, RSSI Gain Control Register 2  */
    volatile                             U32                rssi_gain_ctrl3; /*0x88 , RW   , 0x00002931, RSSI Gain Control Register 3  */
    volatile                             U32                rssi_gain_ctrl4; /*0x8c , RW   , 0x0000373F, RSSI Gain Control Register 4  */
    volatile                             U32                rssi_gain_ctrl5; /*0x90 , RW   , 0x00000913, RSSI Gain Control Register 5  */
    volatile                             U32                rssi_gain_ctrl6; /*0x94 , RW   , 0x00001B23, RSSI Gain Control Register 6  */
    volatile                             U32                rssi_gain_ctrl7; /*0x98 , RW   , 0x00002931, RSSI Gain Control Register 7  */
    volatile                             U32                rssi_gain_ctrl8; /*0x9c , RW   , 0x0000373F, RSSI Gain Control Register 8  */
    volatile                             U32                  modem_tpd_sel; /*0xa0 , RW   , 0x00000000, Test Port Selection Register  */
    volatile                             U32              demod_smp_th_ctrl; /*0xa4 , RW   , 0x0000A240, Demod Sample Threhold Control Register*/
    volatile                             U32                newpacket_byte4; /*0xa8 , RW   , 0x0000C000, Newpacket Byte4 Register      */
    volatile                             U32                newpacket_byte3; /*0xac , RW   , 0x00006666, Newpacket Byte3 Register      */
    volatile                             U32                newpacket_byte2; /*0xb0 , RW   , 0x00003333, Newpacket Byte2 Register      */
    volatile                             U32                newpacket_byte1; /*0xb4 , RW   , 0x00005555, Newpacket Byte1 Register      */
    volatile                             U32                  gfsk_mod_ctrl; /*0xb8 , RW   , 0x0000C075, GFSK Modulation and RSSI Control Register*/
    volatile                             U32              gfsk_mod_ref_ctrl; /*0xbc , RW   , 0x00001616, GFSK Modulation Reference Control Register*/
    volatile                             U32                   sym_dly_ctrl; /*0xc0 , RW   , 0x00001100, GFSK/DPSK Symbol Delay and Test Control Register*/
    volatile                             U32                      rssi_out2; /*0xc4 , RO   , 0x00000000, RSSI Noise Out Register       */
    volatile                             U32                   trx_clk_ctrl; /*0xc8 , RW   , 0x000000C0, TX/RX and Demod Clock Control Register*/
    volatile                             U32                   iq_swap_ctrl; /*0xcc , RW   , 0x00008074, IQ Swap Control Register      */
    volatile                             U32                 gfsk_sync_ctrl; /*0xd0 , RW   , 0x00000000, GFSK Sync Control Register    */
    volatile                             U32                gfsk_demod_ctrl; /*0xd4 , RW   , 0x00000000, GFSK DemodControl Register    */
    volatile                             U32                   gfsk_mod_idx; /*0xd8 , RW   , 0x000031A0, GFSK Modulation Index Register*/
    volatile                             U32            dpsk_gfsk_misc_ctrl; /*0xdc , RW   , 0x00000C18, DPSK/GFSK Misc Control  Register*/
    volatile                             U32                  modem_dbm_sel; /*0xe0 , RW   , 0x00000000, Modem DBM Select Register     */
    volatile                             U32                gfsk_mod_idx_le; /*0xe4 , RW   , 0x00004EC5, GFSK BLE Modulation Index Register*/
    volatile                             U32          newpacket_byte4_inuse; /*0xe8 , RO   , 0x00007E70, Newpacket Byte4 Inuse Register*/
    volatile                             U32          newpacket_byte3_inuse; /*0xec , RO   , 0x000041E3, Newpacket Byte3 Inuse Register*/
    volatile                             U32          newpacket_byte2_inuse; /*0xf0 , RO   , 0x00004000, Newpacket Byte2 Inuse Register*/
    volatile                             U32          newpacket_byte1_inuse; /*0xf4 , RO   , 0x0000000D, Newpacket Byte1 Inuse Register*/
    volatile                             U32                  le_mode_ctrl1; /*0xf8 , RW   , 0x00002823, Le Mode Control Register1     */
    volatile                             U32                  le_mode_ctrl2; /*0xfc , RW   , 0x0000B0BA, Le Mode Control Register2     */
    volatile                             U32                  le_mode_ctrl3; /*0x100, RW   , 0x00002823, Le Mode Control Register3     */
    volatile                             U32                  le_mode_ctrl4; /*0x104, RW   , 0x0000091E, Le Mode Control Register4     */
    volatile                             U32                  le_mode_ctrl5; /*0x108, RW   , 0x00002103, Le Mode Control Register5     */
    volatile                             U32                        rf_ctrl; /*0x10c, RW   , 0x00000000, RFControl Register            */
    volatile                             U32                        tx_q_im; /*0x110, RW   , 0x00000000, TX Q Image Register           */
    volatile                             U32                        tx_i_im; /*0x114, RW   , 0x00000000, TX I Image Register           */
    volatile                             U32                       tx_am_p0; /*0x118, RW   , 0x00000C00, TX AM Comp Register0          */
    volatile                             U32                       tx_am_p1; /*0x11c, RW   , 0x00000000, TX AM Comp Register1          */
    volatile                             U32                       tx_am_p2; /*0x120, RW   , 0x00000000, TX AM Comp Register2          */
    volatile                             U32                       tx_am_p3; /*0x124, RW   , 0x00000000, TX AM Comp Register3          */
    volatile                             U32                       tx_am_p4; /*0x128, RW   , 0x00000000, TX AM Comp Register4          */
    volatile                             U32                       tx_am_p5; /*0x12c, RW   , 0x00000000, TX AM Comp Register5          */
    volatile                             U32                       tx_am_p6; /*0x130, RW   , 0x00000000, TX AM Comp Register6          */
    volatile                             U32                       tx_am_p7; /*0x134, RW   , 0x00000000, TX AM Comp Register7          */
    volatile                             U32                       tx_am_p8; /*0x138, RW   , 0x00000000, TX AM Comp Register8          */
    volatile                             U32                       tx_am_p9; /*0x13c, RW   , 0x00000000, TX AM Comp Register9          */
    volatile                             U32                      tx_am_p10; /*0x140, RW   , 0x00000000, TX AM Comp Register10         */
    volatile                             U32                      tx_am_p11; /*0x144, RW   , 0x00000000, TX AM Comp Register11         */
    volatile                             U32                      tx_am_p12; /*0x148, RW   , 0x00000000, TX AM Comp Register12         */
    volatile                             U32                      tx_am_p13; /*0x14c, RW   , 0x00000000, TX AM Comp Register13         */
    volatile                             U32                      tx_am_p14; /*0x150, RW   , 0x00000000, TX AM Comp Register14         */
    volatile                             U32                      tx_am_p15; /*0x154, RW   , 0x00000000, TX AM Comp Register15         */
    volatile                             U32                      tx_am_p16; /*0x158, RW   , 0x00000000, TX AM Comp Register16         */
    volatile                             U32                       tx_pm_p0; /*0x15c, RW   , 0x00000000, TX PM Comp Register0          */
    volatile                             U32                       tx_pm_p1; /*0x160, RW   , 0x00000000, TX PM Comp Register1          */
    volatile                             U32                       tx_pm_p2; /*0x164, RW   , 0x00000000, TX PM Comp Register2          */
    volatile                             U32                       tx_pm_p3; /*0x168, RW   , 0x00000000, TX PM Comp Register3          */
    volatile                             U32                       tx_pm_p4; /*0x16c, RW   , 0x00000000, TX PM Comp Register4          */
    volatile                             U32                       tx_pm_p5; /*0x170, RW   , 0x00000000, TX PM Comp Register5          */
    volatile                             U32                       tx_pm_p6; /*0x174, RW   , 0x00000000, TX PM Comp Register6          */
    volatile                             U32                       tx_pm_p7; /*0x178, RW   , 0x00000000, TX PM Comp Register7          */
    volatile                             U32                       tx_pm_p8; /*0x17c, RW   , 0x00000000, TX PM Comp Register8          */
    volatile                             U32                       tx_pm_p9; /*0x180, RW   , 0x00000000, TX PM Comp Register9          */
    volatile                             U32                      tx_pm_p10; /*0x184, RW   , 0x00000000, TX PM Comp Register10         */
    volatile                             U32                      tx_pm_p11; /*0x188, RW   , 0x00000000, TX PM Comp Register11         */
    volatile                             U32                      tx_pm_p12; /*0x18c, RW   , 0x00000000, TX PM Comp Register12         */
    volatile                             U32                      tx_pm_p13; /*0x190, RW   , 0x00000000, TX PM Comp Register13         */
    volatile                             U32                      tx_pm_p14; /*0x194, RW   , 0x00000000, TX PM Comp Register14         */
    volatile                             U32                      tx_pm_p15; /*0x198, RW   , 0x00000000, TX PM Comp Register15         */
    volatile                             U32                      tx_pm_p16; /*0x19c, RW   , 0x00000000, TX PM Comp Register16         */
    volatile                             U32                     notch_coef; /*0x1a0, RW   , 0x00003BF3, Notch Filter Coefficient Register*/
    volatile                             U32               adapt_edr3_demod; /*0x1a4, RW   , 0x0000E05A, Adapt EDR3 Demod Register     */
    volatile                             U32              adapt_edr3_thresh; /*0x1a8, RW   , 0x00000333, Adapt EDR3 Demod Register     */
} T_HWP_BT_MODEM_T;

#define BT_MODEM_SEL_RSSI_SRC          (1<<0)              
#define BT_MODEM_SEL_RSSI_SRC_SRRC_OUTPUT (1<<0)              
#define BT_MODEM_SEL_RSSI_SRC_MIXER_OUTPUT (1<<0)              

#define BT_MODEM_BYPASS_SMPPT_DPSK     (1<<1)              
#define BT_MODEM_BYPASS_SMPPT_DPSK_NOT_BYPASS (1<<1)              
#define BT_MODEM_BYPASS_SMPPT_DPSK_BYPASS (1<<1)              

#define BT_MODEM_BYPASS_PATCH_DPSK     (1<<2)              
#define BT_MODEM_BYPASS_PATCH_DPSK_NOT_BYPASS (1<<2)              
#define BT_MODEM_BYPASS_PATCH_DPSK_BYPASS (1<<2)              

#define BT_MODEM_BYPASS_DERR2_DPSK     (1<<3)              
#define BT_MODEM_BYPASS_DERR2_DPSK_NOT_BYPASS (1<<3)              
#define BT_MODEM_BYPASS_DERR2_DPSK_BYPASS (1<<3)              

#define BT_MODEM_BYPASS_DERR1_DPSK     (1<<4)              
#define BT_MODEM_BYPASS_DERR1_DPSK_NOT_BYPASS (1<<4)              
#define BT_MODEM_BYPASS_DERR1_DPSK_BYPASS (1<<4)              

#define BT_MODEM_BYPASS_SMPPT_GFSK     (1<<5)              
#define BT_MODEM_BYPASS_SMPPT_GFSK_NOT_BYPASS (1<<5)              
#define BT_MODEM_BYPASS_SMPPT_GFSK_BYPASS (1<<5)              

#define BT_MODEM_BYPASS_PATCH_GFSK     (1<<6)              
#define BT_MODEM_BYPASS_PATCH_GFSK_NOT_BYPASS (1<<6)              
#define BT_MODEM_BYPASS_PATCH_GFSK_BYPASS (1<<6)              

#define BT_MODEM_BYPASS_DERR2_GFSK     (1<<7)              
#define BT_MODEM_BYPASS_DERR2_GFSK_NOT_BYPASS (1<<7)              
#define BT_MODEM_BYPASS_DERR2_GFSK_BYPASS (1<<7)              

#define BT_MODEM_BYPASS_DERR1_GFSK     (1<<8)              
#define BT_MODEM_BYPASS_DERR1_GFSK_NOT_BYPASS (1<<8)              
#define BT_MODEM_BYPASS_DERR1_GFSK_BYPASS (1<<8)              

#define BT_MODEM_BYPASS_SRRC           (1<<12)             
#define BT_MODEM_BYPASS_SRRC_NOT_BYPASS (1<<12)             
#define BT_MODEM_BYPASS_SRRC_BYPASS    (1<<12)             

#define BT_MODEM_BYPASS_MIXER          (1<<13)             
#define BT_MODEM_BYPASS_MIXER_NOT_BYPASS (1<<13)             
#define BT_MODEM_BYPASS_MIXER_BYPASS   (1<<13)             

#define BT_MODEM_BYPASS_DCCANCEL2      (1<<14)             

#define BT_MODEM_BYPASS_DCCANCEL       (1<<15)             
#define BT_MODEM_BYPASS_DCCANCEL_NOT_BYPASS (1<<15)             
#define BT_MODEM_BYPASS_DCCANCEL_BYPASS (1<<15)             


#define BT_MODEM_SEL_SUMERR_RANGE(n)   (((n)&0x7)<<0)      
#define BT_MODEM_SEL_SUMERR_RANGE_MASK (0x7<<0)            
#define BT_MODEM_SEL_SUMERR_RANGE_SHIFT (0)                 

#define BT_MODEM_IQ_SEL_POL            (1<<5)              
#define BT_MODEM_IQ_SEL_POL_IQ_SEL_INV (1<<5)              
#define BT_MODEM_IQ_SEL_POL_IQ_SEL_RAW (1<<5)              

#define BT_MODEM_SWCH_SIGN_RX          (1<<6)              
#define BT_MODEM_SWCH_SIGN_RX_UNSIGNED (1<<6)              
#define BT_MODEM_SWCH_SIGN_RX_SIGNED   (1<<6)              

#define BT_MODEM_SWCH_SIGN_Q_TX        (1<<7)              
#define BT_MODEM_SWCH_SIGN_Q_TX_UNSIGNED (1<<7)              
#define BT_MODEM_SWCH_SIGN_Q_TX_SIGNED (1<<7)              

#define BT_MODEM_SWCH_SIGN_I_TX        (1<<8)              
#define BT_MODEM_SWCH_SIGN_I_TX_UNSIGNED (1<<8)              
#define BT_MODEM_SWCH_SIGN_I_TX_SIGNED (1<<8)              

#define BT_MODEM_LPF_DWIDTH_SEL(n)     (((n)&0x3)<<9)      
#define BT_MODEM_LPF_DWIDTH_SEL_SHIFT_9BITS (((0)&0x3)<<9)      
#define BT_MODEM_LPF_DWIDTH_SEL_SHIFT_8BITS (((1)&0x3)<<9)      
#define BT_MODEM_LPF_DWIDTH_SEL_SHIFT_7BITS (((2)&0x3)<<9)      
#define BT_MODEM_LPF_DWIDTH_SEL_SHIFT_6BITS (((3)&0x3)<<9)      
#define BT_MODEM_LPF_DWIDTH_SEL_MASK   (0x3<<9)            
#define BT_MODEM_LPF_DWIDTH_SEL_SHIFT  (9)                 

#define BT_MODEM_RSVD0                 (1<<11)             

#define BT_MODEM_SWCH_CLK_DAC          (1<<12)             
#define BT_MODEM_SWCH_CLK_DAC_NOT_SWITCH (1<<12)             
#define BT_MODEM_SWCH_CLK_DAC_SWITCH   (1<<12)             

#define BT_MODEM_SEL_SYNC              (1<<13)             
#define BT_MODEM_SEL_SYNC_FROM_LL      (1<<13)             
#define BT_MODEM_SEL_SYNC_FROM_REG     (1<<13)             

#define BT_MODEM_SWCH_CLK_ADC          (1<<14)             
#define BT_MODEM_SWCH_CLK_ADC_NOT_SWITCH (1<<14)             
#define BT_MODEM_SWCH_CLK_ADC_SWITCH   (1<<14)             


#define BT_MODEM_AFC_SMTIF(n)          (((n)&0xFFFF)<<0)   
#define BT_MODEM_AFC_SMTIF_MASK        (0xFFFF<<0)         
#define BT_MODEM_AFC_SMTIF_SHIFT       (0)                 


#define BT_MODEM_DELAY_DPSK(n)         (((n)&0xF)<<0)      
#define BT_MODEM_DELAY_DPSK_MASK       (0xF<<0)            
#define BT_MODEM_DELAY_DPSK_SHIFT      (0)                 

#define BT_MODEM_DELAY_GFSK(n)         (((n)&0xF)<<4)      
#define BT_MODEM_DELAY_GFSK_MASK       (0xF<<4)            
#define BT_MODEM_DELAY_GFSK_SHIFT      (4)                 

#define BT_MODEM_TX_GAIN_DPSK(n)       (((n)&0xFF)<<8)     
#define BT_MODEM_TX_GAIN_DPSK_MASK     (0xFF<<8)           
#define BT_MODEM_TX_GAIN_DPSK_SHIFT    (8)                 


#define BT_MODEM_TX_GAIN_GFSK(n)       (((n)&0xFF)<<0)     
#define BT_MODEM_TX_GAIN_GFSK_MASK     (0xFF<<0)           
#define BT_MODEM_TX_GAIN_GFSK_SHIFT    (0)                 

#define BT_MODEM_RSVD4                 (1<<8)              

#define BT_MODEM_CNT_GUARD_INI(n)      (((n)&0x3F)<<10)    
#define BT_MODEM_CNT_GUARD_INI_MASK    (0x3F<<10)          
#define BT_MODEM_CNT_GUARD_INI_SHIFT   (10)                


#define BT_MODEM_RSSI_OUT(n)           (((n)&0x3FF)<<0)    
#define BT_MODEM_RSSI_OUT_MASK         (0x3FF<<0)          
#define BT_MODEM_RSSI_OUT_SHIFT        (0)                 


#define BT_MODEM_TX_GAIN_GFSK_EDR(n)   (((n)&0xFF)<<0)     
#define BT_MODEM_TX_GAIN_GFSK_EDR_MASK (0xFF<<0)           
#define BT_MODEM_TX_GAIN_GFSK_EDR_SHIFT (0)                 

#define BT_MODEM_NOTCH_DISB            (1<<8)              

#define BT_MODEM_SWAP_PN_Q_DCCL_0      (1<<9)              
#define BT_MODEM_SWAP_PN_Q_DCCL_0_NO_SWAP (1<<9)              
#define BT_MODEM_SWAP_PN_Q_DCCL_0_SWAP (1<<9)              

#define BT_MODEM_SWAP_PN_I_DCCL_0      (1<<10)             
#define BT_MODEM_SWAP_PN_I_DCCL_0_NO_SWAP (1<<10)             
#define BT_MODEM_SWAP_PN_I_DCCL_0_SWAP (1<<10)             

#define BT_MODEM_SWAP_IQ_MIXER_0       (1<<11)             
#define BT_MODEM_SWAP_IQ_MIXER_0_NO_SWAP (1<<11)             
#define BT_MODEM_SWAP_IQ_MIXER_0_SWAP  (1<<11)             

#define BT_MODEM_SWAP_IQ_SMTIF_0       (1<<12)             

#define BT_MODEM_SWAP_IQ_DCCL_0        (1<<13)             
#define BT_MODEM_SWAP_IQ_DCCL_0_NO_SWAP (1<<13)             
#define BT_MODEM_SWAP_IQ_DCCL_0_SWAP   (1<<13)             

#define BT_MODEM_SWAP_IQ               (1<<14)             
#define BT_MODEM_SWAP_IQ_NO_SWAP       (1<<14)             
#define BT_MODEM_SWAP_IQ_SWAP          (1<<14)             

#define BT_MODEM_RSSI_LOCK_BY_AGC      (1<<15)             


#define BT_MODEM_CT_U_SP_GFSK(n)       (((n)&0xF)<<0)      
#define BT_MODEM_CT_U_SP_GFSK_MASK     (0xF<<0)            
#define BT_MODEM_CT_U_SP_GFSK_SHIFT    (0)                 

#define BT_MODEM_CT_U_1_GFSK(n)        (((n)&0x1F)<<4)     
#define BT_MODEM_CT_U_1_GFSK_MASK      (0x1F<<4)           
#define BT_MODEM_CT_U_1_GFSK_SHIFT     (4)                 

#define BT_MODEM_CT_U_GFSK(n)          (((n)&0xF)<<9)      
#define BT_MODEM_CT_U_GFSK_MASK        (0xF<<9)            
#define BT_MODEM_CT_U_GFSK_SHIFT       (9)                 

#define BT_MODEM_PCKT_SEL              (1<<13)             
#define BT_MODEM_PCKT_SEL_PACKET72     (1<<13)             
#define BT_MODEM_PCKT_SEL_NEW_PACKET   (1<<13)             

#define BT_MODEM_EN_BBPKG_FLG          (1<<14)             
#define BT_MODEM_EN_BBPKG_FLG_DISABLE  (1<<14)             
#define BT_MODEM_EN_BBPKG_FLG_ENABLE   (1<<14)             


#define BT_MODEM_CT_U_SP_DPSK(n)       (((n)&0xF)<<0)      
#define BT_MODEM_CT_U_SP_DPSK_MASK     (0xF<<0)            
#define BT_MODEM_CT_U_SP_DPSK_SHIFT    (0)                 

#define BT_MODEM_CT_U_1_DPSK(n)        (((n)&0x1F)<<4)     
#define BT_MODEM_CT_U_1_DPSK_MASK      (0x1F<<4)           
#define BT_MODEM_CT_U_1_DPSK_SHIFT     (4)                 

#define BT_MODEM_CT_U_DPSK(n)          (((n)&0xF)<<9)      
#define BT_MODEM_CT_U_DPSK_MASK        (0xF<<9)            
#define BT_MODEM_CT_U_DPSK_SHIFT       (9)                 

#define BT_MODEM_CT_U_PATCH(n)         (((n)&0x7)<<13)     
#define BT_MODEM_CT_U_PATCH_MASK       (0x7<<13)           
#define BT_MODEM_CT_U_PATCH_SHIFT      (13)                


#define BT_MODEM_MIN_ERROR_TH(n)       (((n)&0xFFFF)<<0)   
#define BT_MODEM_MIN_ERROR_TH_MASK     (0xFFFF<<0)         
#define BT_MODEM_MIN_ERROR_TH_SHIFT    (0)                 


#define BT_MODEM_TH_GFSK_DEM(n)        (((n)&0x3FF)<<0)    
#define BT_MODEM_TH_GFSK_DEM_MASK      (0x3FF<<0)          
#define BT_MODEM_TH_GFSK_DEM_SHIFT     (0)                 

#define BT_MODEM_RSVD1(n)              (((n)&0x3)<<10)     
#define BT_MODEM_RSVD1_MASK            (0x3<<10)           
#define BT_MODEM_RSVD1_SHIFT           (10)                

#define BT_MODEM_SEL_AFC_DPSKSEEK(n)   (((n)&0x3)<<12)     
#define BT_MODEM_SEL_AFC_DPSKSEEK_MASK (0x3<<12)           
#define BT_MODEM_SEL_AFC_DPSKSEEK_SHIFT (12)                

#define BT_MODEM_SEL_AFC_GFSKDEM(n)    (((n)&0x3)<<14)     
#define BT_MODEM_SEL_AFC_GFSKDEM_MASK  (0x3<<14)           
#define BT_MODEM_SEL_AFC_GFSKDEM_SHIFT (14)                


#define BT_MODEM_TH_GFSK_DEM2(n)       (((n)&0x3FF)<<0)    
#define BT_MODEM_TH_GFSK_DEM2_MASK     (0x3FF<<0)          
#define BT_MODEM_TH_GFSK_DEM2_SHIFT    (0)                 

#define BT_MODEM_SP_TH_SEL_GFSK(n)     (((n)&0x3)<<10)     
#define BT_MODEM_SP_TH_SEL_GFSK_MASK   (0x3<<10)           
#define BT_MODEM_SP_TH_SEL_GFSK_SHIFT  (10)                

#define BT_MODEM_SP_TH_SEL_DPSK(n)     (((n)&0x3)<<12)     
#define BT_MODEM_SP_TH_SEL_DPSK_MASK   (0x3<<12)           
#define BT_MODEM_SP_TH_SEL_DPSK_SHIFT  (12)                

#define BT_MODEM_DIFF_ENABLE_GFSK      (1<<14)             
#define BT_MODEM_DIFF_ENABLE_GFSK_DISABLE (1<<14)             
#define BT_MODEM_DIFF_ENABLE_GFSK_ENABLE (1<<14)             

#define BT_MODEM_DIFF_ENABLE_DPSK      (1<<15)             
#define BT_MODEM_DIFF_ENABLE_DPSK_DISABLE (1<<15)             
#define BT_MODEM_DIFF_ENABLE_DPSK_ENABLE (1<<15)             


#define BT_MODEM_REF_A2_SEK(n)         (((n)&0x3FF)<<0)    
#define BT_MODEM_REF_A2_SEK_MASK       (0x3FF<<0)          
#define BT_MODEM_REF_A2_SEK_SHIFT      (0)                 


#define BT_MODEM_REF_A3_SEK(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A3_SEK_MASK       (0xFF<<0)           
#define BT_MODEM_REF_A3_SEK_SHIFT      (0)                 

#define BT_MODEM_REF_A1_SEK(n)         (((n)&0xFF)<<8)     
#define BT_MODEM_REF_A1_SEK_MASK       (0xFF<<8)           
#define BT_MODEM_REF_A1_SEK_SHIFT      (8)                 


#define BT_MODEM_DC_CT2(n)             (((n)&0x1FF)<<0)    
#define BT_MODEM_DC_CT2_MASK           (0x1FF<<0)          
#define BT_MODEM_DC_CT2_SHIFT          (0)                 

#define BT_MODEM_GUARD_TIME_CONF(n)    (((n)&0x1F)<<9)     
#define BT_MODEM_GUARD_TIME_CONF_MASK  (0x1F<<9)           
#define BT_MODEM_GUARD_TIME_CONF_SHIFT (9)                 

#define BT_MODEM_RSVD2(n)              (((n)&0x3)<<14)     
#define BT_MODEM_RSVD2_MASK            (0x3<<14)           
#define BT_MODEM_RSVD2_SHIFT           (14)                


#define BT_MODEM_DAC_DATA_SEL(n)       (((n)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_TX_52M_I (((0)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_TX_26M_I (((1)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_IQIM_CANCEL_I (((2)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_TX_13M_I (((3)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_MIXER_TX_I (((4)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_ACCU_TX  (((5)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_GFILTER_TX (((6)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_AMPM_AM  (((7)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_CORDIC_TX_AMP (((8)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_SYMBOL2IQ_TX_I (((9)&0x3F)<<0)     
#define BT_MODEM_DAC_DATA_SEL_TX_TEST_DATA0 (((10)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_ANGLE    (((11)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_ADC_DATA_I (((12)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_ADC_DIN_I (((13)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_LPF_I    (((14)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_RATECONV_I (((15)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_CALIB_I  (((16)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_DC_CALIB_I (((17)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_CANCEL_FLT_I (((18)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_NOTCH_I  (((19)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_GAIN_I   (((20)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_BLE_MUX_I (((21)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_MIXER_I  (((22)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_SRRC_I   (((23)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_MIXER_I_13_0 (((24)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_SRRC_I_11_0 (((25)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_ERR_GFSK (((26)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_AFC_GFSK (((27)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_ANGLE_OFFSET (((28)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_RSSI_OUT (((29)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_RX_TEST_DATA0 (((30)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_RX_TEST_DATA2 (((31)&0x3F)<<0)    
#define BT_MODEM_DAC_DATA_SEL_MASK     (0x3F<<0)           
#define BT_MODEM_DAC_DATA_SEL_SHIFT    (0)                 

#define BT_MODEM_DAC_TEST_EN           (1<<6)              

#define BT_MODEM_DC_CT(n)              (((n)&0x1FF)<<7)    
#define BT_MODEM_DC_CT_MASK            (0x1FF<<7)          
#define BT_MODEM_DC_CT_SHIFT           (7)                 


#define BT_MODEM_RSSI_TM_TH(n)         (((n)&0x3F)<<0)     
#define BT_MODEM_RSSI_TM_TH_MASK       (0x3F<<0)           
#define BT_MODEM_RSSI_TM_TH_SHIFT      (0)                 

#define BT_MODEM_DC_HOLD_EDR_EN        (1<<6)              


#define BT_MODEM_CNT_SAMPLE_INI(n)     (((n)&0xF)<<0)      
#define BT_MODEM_CNT_SAMPLE_INI_MASK   (0xF<<0)            
#define BT_MODEM_CNT_SAMPLE_INI_SHIFT  (0)                 

#define BT_MODEM_REF_READY_TH(n)       (((n)&0x3F)<<4)     
#define BT_MODEM_REF_READY_TH_MASK     (0x3F<<4)           
#define BT_MODEM_REF_READY_TH_SHIFT    (4)                 

#define BT_MODEM_INI_RST_TH(n)         (((n)&0x3F)<<10)    
#define BT_MODEM_INI_RST_TH_MASK       (0x3F<<10)          
#define BT_MODEM_INI_RST_TH_SHIFT      (10)                


#define BT_MODEM_AGC_SINC_OVER_TH(n)   (((n)&0x3)<<0)      
#define BT_MODEM_AGC_SINC_OVER_TH_MASK (0x3<<0)            
#define BT_MODEM_AGC_SINC_OVER_TH_SHIFT (0)                 

#define BT_MODEM_AGC_SINC_OVER_EN      (1<<2)              
#define BT_MODEM_AGC_SINC_OVER_EN_DISABLE (1<<2)              
#define BT_MODEM_AGC_SINC_OVER_EN_ENABLE (1<<2)              

#define BT_MODEM_AGC_MOD_FIX7          (1<<3)              
#define BT_MODEM_AGC_MOD_FIX7_THRESHOLD_2 (1<<3)              
#define BT_MODEM_AGC_MOD_FIX7_THRESHOLD_3 (1<<3)              

#define BT_MODEM_AGC_EN_FIX7           (1<<4)              
#define BT_MODEM_AGC_EN_FIX7_DISABLE   (1<<4)              
#define BT_MODEM_AGC_EN_FIX7_ENABLE    (1<<4)              

#define BT_MODEM_DPSK_SEEK_ST_CNT(n)   (((n)&0x1F)<<5)     
#define BT_MODEM_DPSK_SEEK_ST_CNT_MASK (0x1F<<5)           
#define BT_MODEM_DPSK_SEEK_ST_CNT_SHIFT (5)                 

#define BT_MODEM_MIX_GUARD_TH(n)       (((n)&0x3F)<<10)    
#define BT_MODEM_MIX_GUARD_TH_MASK     (0x3F<<10)          
#define BT_MODEM_MIX_GUARD_TH_SHIFT    (10)                


#define BT_MODEM_AGC_TH_MIN(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_AGC_TH_MIN_MASK       (0xFF<<0)           
#define BT_MODEM_AGC_TH_MIN_SHIFT      (0)                 

#define BT_MODEM_AGC_TH_MAX(n)         (((n)&0xFF)<<8)     
#define BT_MODEM_AGC_TH_MAX_MASK       (0xFF<<8)           
#define BT_MODEM_AGC_TH_MAX_SHIFT      (8)                 


#define BT_MODEM_AGC_TH_MIN_LG(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_AGC_TH_MIN_LG_MASK    (0xFF<<0)           
#define BT_MODEM_AGC_TH_MIN_LG_SHIFT   (0)                 

#define BT_MODEM_AGC_TH_MAX_LG(n)      (((n)&0xFF)<<8)     
#define BT_MODEM_AGC_TH_MAX_LG_MASK    (0xFF<<8)           
#define BT_MODEM_AGC_TH_MAX_LG_SHIFT   (8)                 


#define BT_MODEM_AGC_MOD_DEM(n)        (((n)&0x3)<<0)      
#define BT_MODEM_AGC_MOD_DEM_MASK      (0x3<<0)            
#define BT_MODEM_AGC_MOD_DEM_SHIFT     (0)                 

#define BT_MODEM_AGC_INDEX_INI_DSP(n)  (((n)&0xF)<<2)      
#define BT_MODEM_AGC_INDEX_INI_DSP_MASK (0xF<<2)            
#define BT_MODEM_AGC_INDEX_INI_DSP_SHIFT (2)                 

#define BT_MODEM_AGC_EN_DLY(n)         (((n)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_0US  (((0)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_0P5US (((1)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_1US  (((2)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_2US  (((3)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_3US  (((4)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_4US  (((5)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_6US  (((6)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_DELAY_8US  (((7)&0x7)<<6)      
#define BT_MODEM_AGC_EN_DLY_MASK       (0x7<<6)            
#define BT_MODEM_AGC_EN_DLY_SHIFT      (6)                 

#define BT_MODEM_AGC_STEP_OVER(n)      (((n)&0x3)<<9)      
#define BT_MODEM_AGC_STEP_OVER_MASK    (0x3<<9)            
#define BT_MODEM_AGC_STEP_OVER_SHIFT   (9)                 

#define BT_MODEM_AGC_STEP_MODE(n)      (((n)&0x3)<<11)     
#define BT_MODEM_AGC_STEP_MODE_AGC_STEP_1 (((0)&0x3)<<11)     
#define BT_MODEM_AGC_STEP_MODE_AGC_STEP_2 (((1)&0x3)<<11)     
#define BT_MODEM_AGC_STEP_MODE_AGC_STEP_3 (((2)&0x3)<<11)     
#define BT_MODEM_AGC_STEP_MODE_AGC_STEP_4 (((3)&0x3)<<11)     
#define BT_MODEM_AGC_STEP_MODE_MASK    (0x3<<11)           
#define BT_MODEM_AGC_STEP_MODE_SHIFT   (11)                

#define BT_MODEM_AGC_EN_LG_STP         (1<<13)             
#define BT_MODEM_AGC_EN_LG_STP_DISABLE (1<<13)             
#define BT_MODEM_AGC_EN_LG_STP_ENABLE  (1<<13)             

#define BT_MODEM_AGC_CT_TH_MIN         (1<<14)             


#define BT_MODEM_AGC_TM_INTV_LG(n)     (((n)&0x7F)<<0)     
#define BT_MODEM_AGC_TM_INTV_LG_MASK   (0x7F<<0)           
#define BT_MODEM_AGC_TM_INTV_LG_SHIFT  (0)                 

#define BT_MODEM_AGC_TM_INTV(n)        (((n)&0x7F)<<7)     
#define BT_MODEM_AGC_TM_INTV_MASK      (0x7F<<7)           
#define BT_MODEM_AGC_TM_INTV_SHIFT     (7)                 

#define BT_MODEM_AGC_TH_MIN_GN(n)      (((n)&0x3)<<14)     
#define BT_MODEM_AGC_TH_MIN_GN_GAIN_2  (((0)&0x3)<<14)     
#define BT_MODEM_AGC_TH_MIN_GN_GAIN_4  (((1)&0x3)<<14)     
#define BT_MODEM_AGC_TH_MIN_GN_GAIN_8  (((2)&0x3)<<14)     
#define BT_MODEM_AGC_TH_MIN_GN_GAIN_16 (((3)&0x3)<<14)     
#define BT_MODEM_AGC_TH_MIN_GN_MASK    (0x3<<14)           
#define BT_MODEM_AGC_TH_MIN_GN_SHIFT   (14)                


#define BT_MODEM_AGC_MODE_DSP(n)       (((n)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_NORMAL   (((0)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_RESERVED (((1)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_HOLD_AFTER_TIMER (((2)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_FIX_TO_INDEX_INI (((3)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_HOLD_BY_FSM (((4)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_TH_LARGE_MODE (((5)&0x7)<<0)      
#define BT_MODEM_AGC_MODE_DSP_MASK     (0x7<<0)            
#define BT_MODEM_AGC_MODE_DSP_SHIFT    (0)                 

#define BT_MODEM_NEWPACKET_ZERO_WD_CNT(n) (((n)&0x3)<<6)      
#define BT_MODEM_NEWPACKET_ZERO_WD_CNT_MASK (0x3<<6)            
#define BT_MODEM_NEWPACKET_ZERO_WD_CNT_SHIFT (6)                 

#define BT_MODEM_PASS_NEWPACKET_SEL    (1<<8)              

#define BT_MODEM_DGC_INDEX_MAX(n)      (((n)&0x7)<<9)      
#define BT_MODEM_DGC_INDEX_MAX_MASK    (0x7<<9)            
#define BT_MODEM_DGC_INDEX_MAX_SHIFT   (9)                 

#define BT_MODEM_DGC_INDEX_DSP(n)      (((n)&0x7)<<12)     
#define BT_MODEM_DGC_INDEX_DSP_MASK    (0x7<<12)           
#define BT_MODEM_DGC_INDEX_DSP_SHIFT   (12)                

#define BT_MODEM_SEL_REG_AGC           (1<<15)             
#define BT_MODEM_SEL_REG_AGC_DGC_INDEX_DSP (1<<15)             
#define BT_MODEM_SEL_REG_AGC_DGC_INDEX_MX (1<<15)             


#define BT_MODEM_RX_FIX_DCOFST         (1<<0)              
#define BT_MODEM_RX_FIX_DCOFST_BY_CALIB (1<<0)              
#define BT_MODEM_RX_FIX_DCOFST_BY_REG  (1<<0)              

#define BT_MODEM_DC_CAL_RX_DLY(n)      (((n)&0x3)<<1)      
#define BT_MODEM_DC_CAL_RX_DLY_0P6MS   (((0)&0x3)<<1)      
#define BT_MODEM_DC_CAL_RX_DLY_1P2MS   (((1)&0x3)<<1)      
#define BT_MODEM_DC_CAL_RX_DLY_2P4MS   (((2)&0x3)<<1)      
#define BT_MODEM_DC_CAL_RX_DLY_4P8MS   (((3)&0x3)<<1)      
#define BT_MODEM_DC_CAL_RX_DLY_MASK    (0x3<<1)            
#define BT_MODEM_DC_CAL_RX_DLY_SHIFT   (1)                 

#define BT_MODEM_RX_DC_CAL_DONE        (1<<3)              

#define BT_MODEM_AGC_TM_HOLD(n)        (((n)&0x1F)<<6)     
#define BT_MODEM_AGC_TM_HOLD_MASK      (0x1F<<6)           
#define BT_MODEM_AGC_TM_HOLD_SHIFT     (6)                 

#define BT_MODEM_AGC_TM_WAIT(n)        (((n)&0x1F)<<11)    
#define BT_MODEM_AGC_TM_WAIT_MASK      (0x1F<<11)          
#define BT_MODEM_AGC_TM_WAIT_SHIFT     (11)                


#define BT_MODEM_RX_DC_CALI_Q_FIX(n)   (((n)&0xFF)<<0)     
#define BT_MODEM_RX_DC_CALI_Q_FIX_MASK (0xFF<<0)           
#define BT_MODEM_RX_DC_CALI_Q_FIX_SHIFT (0)                 

#define BT_MODEM_RX_DC_CALI_I_FIX(n)   (((n)&0xFF)<<8)     
#define BT_MODEM_RX_DC_CALI_I_FIX_MASK (0xFF<<8)           
#define BT_MODEM_RX_DC_CALI_I_FIX_SHIFT (8)                 


#define BT_MODEM_RX_DCOFFSET_Q(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_RX_DCOFFSET_Q_MASK    (0xFF<<0)           
#define BT_MODEM_RX_DCOFFSET_Q_SHIFT   (0)                 

#define BT_MODEM_RX_DCOFFSET_I(n)      (((n)&0xFF)<<8)     
#define BT_MODEM_RX_DCOFFSET_I_MASK    (0xFF<<8)           
#define BT_MODEM_RX_DCOFFSET_I_SHIFT   (8)                 


#define BT_MODEM_BYPASS_TX_CAL         (1<<0)              
#define BT_MODEM_BYPASS_TX_CAL_NOT_BYPASS (1<<0)              
#define BT_MODEM_BYPASS_TX_CAL_BYPASS  (1<<0)              

#define BT_MODEM_TX_APC(n)             (((n)&0x7)<<1)      
#define BT_MODEM_TX_APC_MASK           (0x7<<1)            
#define BT_MODEM_TX_APC_SHIFT          (1)                 

#define BT_MODEM_TX_CAL_SHIFT(n)       (((n)&0x3)<<4)      
#define BT_MODEM_TX_CAL_SHIFT_X4       (((0)&0x3)<<4)      
#define BT_MODEM_TX_CAL_SHIFT_X2       (((1)&0x3)<<4)      
#define BT_MODEM_TX_CAL_SHIFT_X1       (((2)&0x3)<<4)      
#define BT_MODEM_TX_CAL_SHIFT_MASK     (0x3<<4)            
#define BT_MODEM_TX_CAL_SHIFT_SHIFT    (4)                 

#define BT_MODEM_TX_CAL_SEL(n)         (((n)&0x3)<<6)      
#define BT_MODEM_TX_CAL_SEL_MEAN       (((0)&0x3)<<6)      
#define BT_MODEM_TX_CAL_SEL_TX_CAL1    (((1)&0x3)<<6)      
#define BT_MODEM_TX_CAL_SEL_TX_CAL2    (((2)&0x3)<<6)      
#define BT_MODEM_TX_CAL_SEL_MASK       (0x3<<6)            
#define BT_MODEM_TX_CAL_SEL_SHIFT      (6)                 

#define BT_MODEM_TX_CAL_POL            (1<<8)              
#define BT_MODEM_TX_CAL_POL_NO_SWITCH  (1<<8)              
#define BT_MODEM_TX_CAL_POL_SWITCH     (1<<8)              

#define BT_MODEM_TX_CAL_CMP_POL        (1<<9)              

#define BT_MODEM_TX_CAL_CNT(n)         (((n)&0x3)<<10)     
#define BT_MODEM_TX_CAL_CNT_0P125_RANGE (((0)&0x3)<<10)     
#define BT_MODEM_TX_CAL_CNT_0P25_RANGE (((1)&0x3)<<10)     
#define BT_MODEM_TX_CAL_CNT_0P5_RANGE  (((2)&0x3)<<10)     
#define BT_MODEM_TX_CAL_CNT_FULL_RANGE (((3)&0x3)<<10)     
#define BT_MODEM_TX_CAL_CNT_MASK       (0x3<<10)           
#define BT_MODEM_TX_CAL_CNT_SHIFT      (10)                

#define BT_MODEM_TX_FIX_DCOFST         (1<<12)             

#define BT_MODEM_TX_DC_CAL_DONE        (1<<15)             


#define BT_MODEM_TX_DC_IDATA_FIX(n)    (((n)&0xFFF)<<0)    
#define BT_MODEM_TX_DC_IDATA_FIX_MASK  (0xFFF<<0)          
#define BT_MODEM_TX_DC_IDATA_FIX_SHIFT (0)                 


#define BT_MODEM_TX_DC_QDATA_FIX(n)    (((n)&0xFFF)<<0)    
#define BT_MODEM_TX_DC_QDATA_FIX_MASK  (0xFFF<<0)          
#define BT_MODEM_TX_DC_QDATA_FIX_SHIFT (0)                 


#define BT_MODEM_DC_CAL_TX_IDATA(n)    (((n)&0xFFF)<<0)    
#define BT_MODEM_DC_CAL_TX_IDATA_MASK  (0xFFF<<0)          
#define BT_MODEM_DC_CAL_TX_IDATA_SHIFT (0)                 


#define BT_MODEM_DC_CAL_TX_QDATA(n)    (((n)&0xFFF)<<0)    
#define BT_MODEM_DC_CAL_TX_QDATA_MASK  (0xFFF<<0)          
#define BT_MODEM_DC_CAL_TX_QDATA_SHIFT (0)                 


#define BT_MODEM_RSSI_ANA_GAIN0001(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0001_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN0001_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN0000(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN0000_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN0000_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN0011(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0011_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN0011_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN0010(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN0010_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN0010_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN0101(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0101_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN0101_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN0100(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN0100_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN0100_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN0111(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN0111_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN0111_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN0110(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN0110_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN0110_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN1001(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1001_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN1001_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN1000(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN1000_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN1000_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN1011(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1011_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN1011_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN1010(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN1010_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN1010_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN1101(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1101_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN1101_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN1100(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN1100_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN1100_SHIFT (8)                 


#define BT_MODEM_RSSI_ANA_GAIN1111(n)  (((n)&0x7F)<<0)     
#define BT_MODEM_RSSI_ANA_GAIN1111_MASK (0x7F<<0)           
#define BT_MODEM_RSSI_ANA_GAIN1111_SHIFT (0)                 

#define BT_MODEM_RSSI_ANA_GAIN1110(n)  (((n)&0x7F)<<8)     
#define BT_MODEM_RSSI_ANA_GAIN1110_MASK (0x7F<<8)           
#define BT_MODEM_RSSI_ANA_GAIN1110_SHIFT (8)                 


#define BT_MODEM_TPD_TRIG_SEL(n)       (((n)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_DEM_ST_CHG (((0)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_AGC_ST_CHG (((1)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_AGC_FLG_DEM (((2)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_BLE_ACCESS_RB (((3)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_IF_PEAK  (((4)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_IF_SEEKED_ALL (((5)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_SEEK_EN  (((6)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_FLG_GETSYMBOL (((7)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_TX_SYMBOL_OFF_GFSK (((8)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_TX_AMP_SEL (((9)&0xF)<<0)      
#define BT_MODEM_TPD_TRIG_SEL_MASK     (0xF<<0)            
#define BT_MODEM_TPD_TRIG_SEL_SHIFT    (0)                 

#define BT_MODEM_TPD_DATA_SEL(n)       (((n)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DAC_DATA_I (((0)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DAC_DATA_Q (((1)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_I_SUM (((2)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_Q_SUM (((3)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_DAC_I (((4)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_DAC_Q (((5)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_RX_DAC_I (((6)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_RX_DAC_Q (((7)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_DAC_I_13M (((8)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_DOUT_TX_DAC_Q_13M (((9)&0xF)<<4)      
#define BT_MODEM_TPD_DATA_SEL_MASK     (0xF<<4)            
#define BT_MODEM_TPD_DATA_SEL_SHIFT    (4)                 

#define BT_MODEM_TPD_CLK_SEL           (1<<8)              
#define BT_MODEM_TPD_CLK_SEL_CLK_RX    (1<<8)              
#define BT_MODEM_TPD_CLK_SEL_CLK_TX    (1<<8)              

#define BT_MODEM_DAC_CLK_FORCE_EN      (1<<9)              


#define BT_MODEM_DEM_SP_TH1(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_DEM_SP_TH1_MASK       (0xFF<<0)           
#define BT_MODEM_DEM_SP_TH1_SHIFT      (0)                 

#define BT_MODEM_DEM_SP_TH2(n)         (((n)&0xFF)<<8)     
#define BT_MODEM_DEM_SP_TH2_MASK       (0xFF<<8)           
#define BT_MODEM_DEM_SP_TH2_SHIFT      (8)                 


#define BT_MODEM_NEWPACKET_4(n)        (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_4_MASK      (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_4_SHIFT     (0)                 


#define BT_MODEM_NEWPACKET_3(n)        (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_3_MASK      (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_3_SHIFT     (0)                 


#define BT_MODEM_NEWPACKET_2(n)        (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_2_MASK      (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_2_SHIFT     (0)                 


#define BT_MODEM_NEWPACKET_1(n)        (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_1_MASK      (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_1_SHIFT     (0)                 


#define BT_MODEM_REF_A2_DEM(n)         (((n)&0x3FF)<<0)    
#define BT_MODEM_REF_A2_DEM_MASK       (0x3FF<<0)          
#define BT_MODEM_REF_A2_DEM_SHIFT      (0)                 

#define BT_MODEM_RSSI_CT_U(n)          (((n)&0x7)<<12)     
#define BT_MODEM_RSSI_CT_U_MASK        (0x7<<12)           
#define BT_MODEM_RSSI_CT_U_SHIFT       (12)                

#define BT_MODEM_BT_MOD                (1<<15)             


#define BT_MODEM_REF_A3_DEM(n)         (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A3_DEM_MASK       (0xFF<<0)           
#define BT_MODEM_REF_A3_DEM_SHIFT      (0)                 

#define BT_MODEM_REF_A1_DEM(n)         (((n)&0xFF)<<8)     
#define BT_MODEM_REF_A1_DEM_MASK       (0xFF<<8)           
#define BT_MODEM_REF_A1_DEM_SHIFT      (8)                 


#define BT_MODEM_DELAY_GFSK_1M(n)      (((n)&0x7)<<0)      
#define BT_MODEM_DELAY_GFSK_1M_MASK    (0x7<<0)            
#define BT_MODEM_DELAY_GFSK_1M_SHIFT   (0)                 

#define BT_MODEM_DELAY_DPSK_1M(n)      (((n)&0x7)<<3)      
#define BT_MODEM_DELAY_DPSK_1M_MASK    (0x7<<3)            
#define BT_MODEM_DELAY_DPSK_1M_SHIFT   (3)                 

#define BT_MODEM_DELAY_GFSK_OFF_1M(n)  (((n)&0x7)<<6)      
#define BT_MODEM_DELAY_GFSK_OFF_1M_MASK (0x7<<6)            
#define BT_MODEM_DELAY_GFSK_OFF_1M_SHIFT (6)                 

#define BT_MODEM_DELAY_GFSK_OFF(n)     (((n)&0xF)<<9)      
#define BT_MODEM_DELAY_GFSK_OFF_MASK   (0xF<<9)            
#define BT_MODEM_DELAY_GFSK_OFF_SHIFT  (9)                 

#define BT_MODEM_RAMP_SPEED_GFSK       (1<<13)             
#define BT_MODEM_RAMP_SPEED_GFSK_SLOW  (1<<13)             
#define BT_MODEM_RAMP_SPEED_GFSK_FAST  (1<<13)             


#define BT_MODEM_RSSI_NOISE_OUT(n)     (((n)&0x3FF)<<0)    
#define BT_MODEM_RSSI_NOISE_OUT_MASK   (0x3FF<<0)          
#define BT_MODEM_RSSI_NOISE_OUT_SHIFT  (0)                 


#define BT_MODEM_APC_SWITCH_MODE(n)    (((n)&0xF)<<6)      
#define BT_MODEM_APC_SWITCH_MODE_MASK  (0xF<<6)            
#define BT_MODEM_APC_SWITCH_MODE_SHIFT (6)                 

#define BT_MODEM_TX_RX_REG             (1<<12)             
#define BT_MODEM_TX_RX_REG_RX          (1<<12)             
#define BT_MODEM_TX_RX_REG_TX          (1<<12)             

#define BT_MODEM_TX_RX_DIR             (1<<13)             
#define BT_MODEM_TX_RX_DIR_BY_HW       (1<<13)             
#define BT_MODEM_TX_RX_DIR_BY_REG      (1<<13)             

#define BT_MODEM_SWCH_CLK_52M_RX       (1<<14)             
#define BT_MODEM_SWCH_CLK_52M_RX_NEGEDGE (1<<14)             
#define BT_MODEM_SWCH_CLK_52M_RX_POSEDGE (1<<14)             


#define BT_MODEM_NOTCH_EN_0            (1<<0)              
#define BT_MODEM_NOTCH_EN_0_BYPASS     (1<<0)              
#define BT_MODEM_NOTCH_EN_0_ENABLE     (1<<0)              

#define BT_MODEM_RATE_CONV_EN_0        (1<<1)              
#define BT_MODEM_RATE_CONV_EN_0_BYPASS (1<<1)              
#define BT_MODEM_RATE_CONV_EN_0_ENABLE (1<<1)              

#define BT_MODEM_LPF_EN_0              (1<<2)              
#define BT_MODEM_LPF_EN_0_BYPASS       (1<<2)              
#define BT_MODEM_LPF_EN_0_ENABLE       (1<<2)              

#define BT_MODEM_NOTCH_EN_1            (1<<4)              
#define BT_MODEM_NOTCH_EN_1_BYPASS     (1<<4)              
#define BT_MODEM_NOTCH_EN_1_ENABLE     (1<<4)              

#define BT_MODEM_RATE_CONV_EN_1        (1<<5)              
#define BT_MODEM_RATE_CONV_EN_1_BYPASS (1<<5)              
#define BT_MODEM_RATE_CONV_EN_1_ENABLE (1<<5)              

#define BT_MODEM_LPF_EN_1              (1<<6)              
#define BT_MODEM_LPF_EN_1_BYPASS       (1<<6)              
#define BT_MODEM_LPF_EN_1_ENABLE       (1<<6)              

#define BT_MODEM_IQ_SWAP_SRRC          (1<<12)             
#define BT_MODEM_IQ_SWAP_SRRC_NOT_SWAP (1<<12)             
#define BT_MODEM_IQ_SWAP_SRRC_SWAP     (1<<12)             

#define BT_MODEM_IQ_SWAP_LPF           (1<<13)             
#define BT_MODEM_IQ_SWAP_LPF_NOT_SWAP  (1<<13)             
#define BT_MODEM_IQ_SWAP_LPF_SWAP      (1<<13)             

#define BT_MODEM_IQ_SWAP_GAIN2         (1<<14)             
#define BT_MODEM_IQ_SWAP_GAIN2_NOT_SWAP (1<<14)             
#define BT_MODEM_IQ_SWAP_GAIN2_SWAP    (1<<14)             

#define BT_MODEM_SWCH_CLK_52M_TX       (1<<15)             
#define BT_MODEM_SWCH_CLK_52M_TX_NOT_SWITCH (1<<15)             
#define BT_MODEM_SWCH_CLK_52M_TX_SWITCH (1<<15)             


#define BT_MODEM_DYNAMIC_SYNC_TH(n)    (((n)&0x3FF)<<0)    
#define BT_MODEM_DYNAMIC_SYNC_TH_MASK  (0x3FF<<0)          
#define BT_MODEM_DYNAMIC_SYNC_TH_SHIFT (0)                 

#define BT_MODEM_DYNAMIC_SYNC_EN       (1<<10)             
#define BT_MODEM_DYNAMIC_SYNC_EN_STATIC (1<<10)             
#define BT_MODEM_DYNAMIC_SYNC_EN_DYNAMIC (1<<10)             


#define BT_MODEM_MIN_ERROR_TH2(n)      (((n)&0xFFFF)<<0)   
#define BT_MODEM_MIN_ERROR_TH2_MASK    (0xFFFF<<0)         
#define BT_MODEM_MIN_ERROR_TH2_SHIFT   (0)                 


#define BT_MODEM_H_GAIN(n)             (((n)&0xFFFF)<<0)   
#define BT_MODEM_H_GAIN_MASK           (0xFFFF<<0)         
#define BT_MODEM_H_GAIN_SHIFT          (0)                 


#define BT_MODEM_DLY_CT_AMP(n)         (((n)&0x7)<<3)      
#define BT_MODEM_DLY_CT_AMP_MASK       (0x7<<3)            
#define BT_MODEM_DLY_CT_AMP_SHIFT      (3)                 

#define BT_MODEM_DLY_CT_DPSK(n)        (((n)&0x7)<<6)      
#define BT_MODEM_DLY_CT_DPSK_MASK      (0x7<<6)            
#define BT_MODEM_DLY_CT_DPSK_SHIFT     (6)                 

#define BT_MODEM_DLY_CT_GFSK(n)        (((n)&0x7)<<9)      
#define BT_MODEM_DLY_CT_GFSK_MASK      (0x7<<9)            
#define BT_MODEM_DLY_CT_GFSK_SHIFT     (9)                 

#define BT_MODEM_IQ_SWAP_TX            (1<<15)             
#define BT_MODEM_IQ_SWAP_TX_NOT_SWAP   (1<<15)             
#define BT_MODEM_IQ_SWAP_TX_SWAP       (1<<15)             


#define BT_MODEM_DBM_DATA_SEL(n)       (((n)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_GFILTER_TX_DOUT (((0)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_SYMBOL2IQ_TX_DOUT_Q (((1)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_CORDIC_TX_AMP_DOUT (((2)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_AMPM_TX_DOUT_AM (((3)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_DIFF_TX_DOUT (((4)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_FREQ_BLEND_TX_DOUT (((5)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_INTIGRATE_TX_DOUT (((6)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_CORDIC_IQ_TX_DOUT_Q (((7)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_DOUT_TX_13M_Q (((8)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_IQIM_CANCEL_DOUT_Q (((9)&0x1F)<<0)     
#define BT_MODEM_DBM_DATA_SEL_ADC_DATA_Q (((16)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_ADC_DIN_Q (((17)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_LPF_Q    (((18)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_RATECONV_Q (((19)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_CALIB_Q  (((20)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_DC_CALIB_Q (((21)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_CANCEL_FLT_I (((22)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_NOTCH_Q  (((23)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_GAIN_Q   (((24)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_BLE_MUX_Q (((25)&0x1F)<<0)    
#define BT_MODEM_DBM_DATA_SEL_MASK     (0x1F<<0)           
#define BT_MODEM_DBM_DATA_SEL_SHIFT    (0)                 


#define BT_MODEM_H_GAIN_LE(n)          (((n)&0xFFFF)<<0)   
#define BT_MODEM_H_GAIN_LE_MASK        (0xFFFF<<0)         
#define BT_MODEM_H_GAIN_LE_SHIFT       (0)                 


#define BT_MODEM_NEWPACKET_DSP4(n)     (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_DSP4_MASK   (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_DSP4_SHIFT  (0)                 


#define BT_MODEM_NEWPACKET_DSP3(n)     (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_DSP3_MASK   (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_DSP3_SHIFT  (0)                 


#define BT_MODEM_NEWPACKET_DSP2(n)     (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_DSP2_MASK   (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_DSP2_SHIFT  (0)                 


#define BT_MODEM_NEWPACKET_DSP1(n)     (((n)&0xFFFF)<<0)   
#define BT_MODEM_NEWPACKET_DSP1_MASK   (0xFFFF<<0)         
#define BT_MODEM_NEWPACKET_DSP1_SHIFT  (0)                 


#define BT_MODEM_REF_A1_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A1_DEM_LE_MASK    (0xFF<<0)           
#define BT_MODEM_REF_A1_DEM_LE_SHIFT   (0)                 

#define BT_MODEM_REF_A1_SEK_LE(n)      (((n)&0xFF)<<8)     
#define BT_MODEM_REF_A1_SEK_LE_MASK    (0xFF<<8)           
#define BT_MODEM_REF_A1_SEK_LE_SHIFT   (8)                 


#define BT_MODEM_REF_A2_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A2_DEM_LE_MASK    (0xFF<<0)           
#define BT_MODEM_REF_A2_DEM_LE_SHIFT   (0)                 

#define BT_MODEM_REF_A2_SEK_LE(n)      (((n)&0xFF)<<8)     
#define BT_MODEM_REF_A2_SEK_LE_MASK    (0xFF<<8)           
#define BT_MODEM_REF_A2_SEK_LE_SHIFT   (8)                 


#define BT_MODEM_REF_A3_DEM_LE(n)      (((n)&0xFF)<<0)     
#define BT_MODEM_REF_A3_DEM_LE_MASK    (0xFF<<0)           
#define BT_MODEM_REF_A3_DEM_LE_SHIFT   (0)                 

#define BT_MODEM_REF_A3_SEK_LE(n)      (((n)&0xFF)<<8)     
#define BT_MODEM_REF_A3_SEK_LE_MASK    (0xFF<<8)           
#define BT_MODEM_REF_A3_SEK_LE_SHIFT   (8)                 


#define BT_MODEM_MIN_ERROR_TH_LE(n)    (((n)&0xFFFF)<<0)   
#define BT_MODEM_MIN_ERROR_TH_LE_MASK  (0xFFFF<<0)         
#define BT_MODEM_MIN_ERROR_TH_LE_SHIFT (0)                 


#define BT_MODEM_REF_READY_TH_LE(n)    (((n)&0x3F)<<0)     
#define BT_MODEM_REF_READY_TH_LE_MASK  (0x3F<<0)           
#define BT_MODEM_REF_READY_TH_LE_SHIFT (0)                 

#define BT_MODEM_MIX_GUARD_TH_LE(n)    (((n)&0x3F)<<6)     
#define BT_MODEM_MIX_GUARD_TH_LE_MASK  (0x3F<<6)           
#define BT_MODEM_MIX_GUARD_TH_LE_SHIFT (6)                 

#define BT_MODEM_REF_POINT_SEL_LE(n)   (((n)&0x3)<<12)     
#define BT_MODEM_REF_POINT_SEL_LE_MASK (0x3<<12)           
#define BT_MODEM_REF_POINT_SEL_LE_SHIFT (12)                

#define BT_MODEM_RSVD5(n)              (((n)&0x3)<<14)     
#define BT_MODEM_RSVD5_MASK            (0x3<<14)           
#define BT_MODEM_RSVD5_SHIFT           (14)                


#define BT_MODEM_SYNC_SHIFT_MODE       (1<<0)              


#define BT_MODEM_TX_IQIM_PHASE_ERR(n)  (((n)&0x7FFF)<<0)   
#define BT_MODEM_TX_IQIM_PHASE_ERR_MASK (0x7FFF<<0)         
#define BT_MODEM_TX_IQIM_PHASE_ERR_SHIFT (0)                 


#define BT_MODEM_TX_IQIM_AMP_ERR(n)    (((n)&0x7FFF)<<0)   
#define BT_MODEM_TX_IQIM_AMP_ERR_MASK  (0x7FFF<<0)         
#define BT_MODEM_TX_IQIM_AMP_ERR_SHIFT (0)                 


#define BT_MODEM_AM_P0(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P0_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P0_SHIFT           (0)                 

#define BT_MODEM_AM_COMP_BYPASS        (1<<10)             
#define BT_MODEM_AM_COMP_BYPASS_ENABLE (1<<10)             
#define BT_MODEM_AM_COMP_BYPASS_BYPASS (1<<10)             

#define BT_MODEM_PM_COMP_BYPASS        (1<<11)             
#define BT_MODEM_PM_COMP_BYPASS_ENABLE (1<<11)             
#define BT_MODEM_PM_COMP_BYPASS_BYPASS (1<<11)             

#define BT_MODEM_PM_SHIFT(n)           (((n)&0x7)<<12)     
#define BT_MODEM_PM_SHIFT_MASK         (0x7<<12)           
#define BT_MODEM_PM_SHIFT_SHIFT        (12)                


#define BT_MODEM_AM_P1(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P1_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P1_SHIFT           (0)                 


#define BT_MODEM_AM_P2(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P2_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P2_SHIFT           (0)                 


#define BT_MODEM_AM_P3(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P3_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P3_SHIFT           (0)                 


#define BT_MODEM_AM_P4(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P4_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P4_SHIFT           (0)                 


#define BT_MODEM_AM_P5(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P5_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P5_SHIFT           (0)                 


#define BT_MODEM_AM_P6(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P6_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P6_SHIFT           (0)                 


#define BT_MODEM_AM_P7(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P7_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P7_SHIFT           (0)                 


#define BT_MODEM_AM_P8(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P8_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P8_SHIFT           (0)                 


#define BT_MODEM_AM_P9(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P9_MASK            (0x3FF<<0)          
#define BT_MODEM_AM_P9_SHIFT           (0)                 


#define BT_MODEM_AM_P10(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P10_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P10_SHIFT          (0)                 


#define BT_MODEM_AM_P11(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P11_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P11_SHIFT          (0)                 


#define BT_MODEM_AM_P12(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P12_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P12_SHIFT          (0)                 


#define BT_MODEM_AM_P13(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P13_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P13_SHIFT          (0)                 


#define BT_MODEM_AM_P14(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P14_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P14_SHIFT          (0)                 


#define BT_MODEM_AM_P15(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P15_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P15_SHIFT          (0)                 


#define BT_MODEM_AM_P16(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_AM_P16_MASK           (0x3FF<<0)          
#define BT_MODEM_AM_P16_SHIFT          (0)                 


#define BT_MODEM_PM_P0(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P0_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P0_SHIFT           (0)                 


#define BT_MODEM_PM_P1(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P1_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P1_SHIFT           (0)                 


#define BT_MODEM_PM_P2(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P2_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P2_SHIFT           (0)                 


#define BT_MODEM_PM_P3(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P3_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P3_SHIFT           (0)                 


#define BT_MODEM_PM_P4(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P4_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P4_SHIFT           (0)                 


#define BT_MODEM_PM_P5(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P5_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P5_SHIFT           (0)                 


#define BT_MODEM_PM_P6(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P6_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P6_SHIFT           (0)                 


#define BT_MODEM_PM_P7(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P7_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P7_SHIFT           (0)                 


#define BT_MODEM_PM_P8(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P8_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P8_SHIFT           (0)                 


#define BT_MODEM_PM_P9(n)              (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P9_MASK            (0x3FF<<0)          
#define BT_MODEM_PM_P9_SHIFT           (0)                 


#define BT_MODEM_PM_P10(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P10_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P10_SHIFT          (0)                 


#define BT_MODEM_PM_P11(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P11_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P11_SHIFT          (0)                 


#define BT_MODEM_PM_P12(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P12_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P12_SHIFT          (0)                 


#define BT_MODEM_PM_P13(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P13_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P13_SHIFT          (0)                 


#define BT_MODEM_PM_P14(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P14_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P14_SHIFT          (0)                 


#define BT_MODEM_PM_P15(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P15_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P15_SHIFT          (0)                 


#define BT_MODEM_PM_P16(n)             (((n)&0x3FF)<<0)    
#define BT_MODEM_PM_P16_MASK           (0x3FF<<0)          
#define BT_MODEM_PM_P16_SHIFT          (0)                 


#define BT_MODEM_NOTCH_B(n)            (((n)&0x3FFF)<<0)   
#define BT_MODEM_NOTCH_B_MASK          (0x3FFF<<0)         
#define BT_MODEM_NOTCH_B_SHIFT         (0)                 


#define BT_MODEM_CT_U_1_DPSK1(n)       (((n)&0x1F)<<0)     
#define BT_MODEM_CT_U_1_DPSK1_MASK     (0x1F<<0)           
#define BT_MODEM_CT_U_1_DPSK1_SHIFT    (0)                 

#define BT_MODEM_CT_U_DPSK1(n)         (((n)&0xF)<<5)      
#define BT_MODEM_CT_U_DPSK1_MASK       (0xF<<5)            
#define BT_MODEM_CT_U_DPSK1_SHIFT      (5)                 

#define BT_MODEM_EDR3_ADAPT_EN         (1<<9)              
#define BT_MODEM_EDR3_ADAPT_EN_DISABLE (1<<9)              
#define BT_MODEM_EDR3_ADAPT_EN_ENABLE  (1<<9)              

#define BT_MODEM_NOTCH_A(n)            (((n)&0xF)<<12)     
#define BT_MODEM_NOTCH_A_MASK          (0xF<<12)           
#define BT_MODEM_NOTCH_A_SHIFT         (12)                


#define BT_MODEM_EDR3_ADAPT_TH(n)      (((n)&0xFFF)<<0)    
#define BT_MODEM_EDR3_ADAPT_TH_MASK    (0xFFF<<0)          
#define BT_MODEM_EDR3_ADAPT_TH_SHIFT   (0)                 


#endif

#endif


