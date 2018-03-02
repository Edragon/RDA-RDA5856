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
// File name      : fm_dsp                                            
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
#ifndef __FM_DSP_H__
#define __FM_DSP_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//tx_fifo_rdata
typedef union
{
    U32 v;
    struct
    {
        U32 tx_data_q                      :16; /*15:0 , TX data Q. Everytime this register is read, the data will be popped out of the tx_data_fifo.*/
        U32 tx_data_i                      :16; /*31:16, TX data I. Everytime this register is read, the data will be popped out of the tx_data_fifo.*/
    } b;
} T_FM_DSP_tx_fifo_rdata;

//revision
typedef union
{
    U32 v;
    struct
    {
        U32 rev_id                         :4 ; /*3 :0 , revision id.                  */
        U32 revision_reserved_0            :28; /*31:4 , No description.               */
    } b;
} T_FM_DSP_revision;

//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 mute                           :1 ; /*0 :0 , mute.1'd0:: normal1'd1:: mute*/
        U32 stereo_in                      :1 ; /*1 :1 , mono select.1'd0:: force : mono1'd1:: stereo*/
        U32 deemph_mode                    :1 ; /*2 :2 , de-emphasis.1'd0:: 75us1'd1:: 50us*/
        U32 softmute_en                    :1 ; /*3 :3 , soft mute enable.1'd0:: disable1'd1:: enable*/
        U32 softblend_off                  :1 ; /*4 :4 , soft blend off.1'd0:: enable1'd1:: disable*/
        U32 afc_disable                    :1 ; /*5 :5 , AFC disable.1'd0:: enable1'd1:: disable*/
        U32 imgrej                         :1 ; /*6 :6 , lo selection.1'd0:: lowlow if; Default is +125KHz.1'd1:: highhigh if. Default is -125KHz*/
        U32 imgrej_dsp                     :1 ; /*7 :7 , IQ swap before 125KHz mixer.  */
        U32 iq_swap                        :1 ; /*8 :8 , IQ swap after 125KHz mixer.   */
        U32 plldem_swap                    :1 ; /*9 :9 , IQ swap for fmdem.            */
        U32 lr_swap                        :1 ; /*10:10, LR swap.                      */
        U32 fircut_bypass                  :1 ; /*11:11, bypass fircut.                */
        U32 fircut_bypass_sk               :1 ; /*12:12, bypass fircut during seeking. */
        U32 lpf_bypass                     :1 ; /*13:13, bypass 15KHz LPF.             */
        U32 deemph_bypass                  :1 ; /*14:14, bypass deemphasis.            */
        U32 pilot_cosine                   :1 ; /*15:15, pilot phase.1'd0:: sin1'd1:: cos*/
        U32 adc_clk_invert                 :1 ; /*16:16, adc clock invert.             */
        U32 noise_src_sel                  :2 ; /*18:17, noise source selection.2'd0:: dangle02'd1:: dangle12'd2:: dangle 2*/
        U32 noise_cancel_src_sel           :1 ; /*19:19, noise cancel source source selection.1'd0:: noise_db21'd1:: rssi_db2*/
        U32 seek_rssi_src_sel              :1 ; /*20:20, rssi source during seek seelction.1'd0:: rssi_db11'd1:: signal_db1*/
        U32 seek_offset_src_sel            :1 ; /*21:21, offset source selection.1'd0:: output_afc1'd1:: output_offset_filter*/
        U32 fmdem_src_sel                  :1 ; /*22:22, coherent fmdemsource selection.1'd0:: output_lpfil1'd1:: output_dig_gain*/
        U32 i2s_en                         :1 ; /*23:23, i2s enable.1'd0:: disable1'd1:: enable*/
        U32 i2s_delay_1t                   :1 ; /*24:24, i2s delay 1t enable.          */
        U32 tx_en                          :1 ; /*25:25, tx data enable.1'd0:: disable1'd1:: enable*/
        U32 force_clk_on                   :1 ; /*26:26, force clock on.               */
        U32 adc_data_format                :1 ; /*27:27, 1'd0:: unsigned1'd1:: 2s_complementary*/
        U32 dbg_out_en                     :1 ; /*28:28, debug output enable.          */
        U32 ctrl_reserved_0                :3 ; /*31:29, No description.               */
    } b;
} T_FM_DSP_ctrl;

//tx_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 tx_sel                         :5 ; /*4 :0 , tx data selection.            */
        U32 tx_fifo_clr                    :1 ; /*5 :5 , clear tx fifo.                */
        U32 tx_fifo_overflow               :1 ; /*6 :6 , tx fifo overflow. User is not able to read tx_fifo_rdata in time so that fm_dsp discard valid data.*/
        U32 tx_fifo_underflow              :1 ; /*7 :7 , tx fifo underflow. User reads tx_fifo_rdata while no data valid in it.*/
        U32 tx_fifo_usedw                  :3 ; /*10:8 , the number of data words in tx fifowhich are valid for read.*/
        U32 tx_ctrl_reserved_0             :21; /*31:11, No description.               */
    } b;
} T_FM_DSP_tx_ctrl;

//seek_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 sk_cmp_grp_en                  :5 ; /*4 :0 , 1'd0: disable1'd1: enable[4]: seek with  pilot;[3]: seek with offset;[2]: seek with snr;[1]: seek with rssi;[0]: seek with noise.*/
        U32 seek_mode                      :3 ; /*7 :5 , seek mode.3'd0:: seek_current_only3'd1:: seek_current_or_adjacentsuccess when either current or adjacent freq is successful;3'd2:: seek_current_and_adjacentsuccess when both current and adjacent freq are successful;3'd3:: snr_sttest mode. stop at SNR_ST;3'd4:: center_sttest mode. stop at CENTER_ST;3'd5:: upper_sttest mode. stop at UPPER_ST;3'd6:: lower_sttestmode. stop at LOWER_ST;3'd7:: seek_bypass*/
        U32 seek_afc_on                    :1 ; /*8 :8 , 1'd0:: disabledisable afc during seeking;1'd1:: enableenable afc during seeking.*/
        U32 seek_range                     :5 ; /*13:9 , seek upper/lower adjacent freq setting. Unit is 5.12KHz.*/
        U32 snr_th                         :6 ; /*19:14, threshold for SNR. Unit is db.*/
        U32 delta_rssi_th                  :6 ; /*25:20, delta rssi threshold during UPPER/LOWER seeking. Unit is db.*/
        U32 snr_cnt_th                     :4 ; /*29:26, SNR counter threshold.        */
        U32 seek_ctrl0_reserved_0          :2 ; /*31:30, No description.               */
    } b;
} T_FM_DSP_seek_ctrl0;

//seek_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 offset_cnt_th                  :8 ; /*7 :0 , offset counter threshold.     */
        U32 noise_cnt_th                   :8 ; /*15:8 , noise counter threshold.      */
        U32 rssi_cnt_th                    :8 ; /*23:16, rssi counter threshold.       */
        U32 pilot_cnt_th                   :8 ; /*31:24, pilot counter threshold.      */
    } b;
} T_FM_DSP_seek_ctrl1;

//seek_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 rssi_h_th                      :7 ; /*6 :0 , RSSI high threshold. Unit is db.*/
        U32 rssi_l_th                      :7 ; /*13:7 , RSSI low threshold. Unit is db.*/
        U32 noise_h_th                     :7 ; /*20:14, noise high threshold. Unit is db.*/
        U32 noise_l_th                     :7 ; /*27:21, noise low threshold. Unit is db.*/
        U32 seek_ctrl2_reserved_0          :4 ; /*31:28, No description.               */
    } b;
} T_FM_DSP_seek_ctrl2;

//seek_ctrl3
typedef union
{
    U32 v;
    struct
    {
        U32 pilot_h_th                     :7 ; /*6 :0 , pilot high threshold. Unit is db.*/
        U32 pilot_l_th                     :7 ; /*13:7 , pilot low threshold. Unit is db.*/
        U32 offset_h_th                    :7 ; /*20:14, offset high threshold. Unit is db.*/
        U32 offset_l_th                    :7 ; /*27:21, offset low threshold. Unit is db.*/
        U32 seek_ctrl3_reserved_0          :4 ; /*31:28, No description.               */
    } b;
} T_FM_DSP_seek_ctrl3;

//seek_ctrl4
typedef union
{
    U32 v;
    struct
    {
        U32 sk_timer1                      :4 ; /*3 :0 , seek time for agc/afc stable. Unit is 0.75ms.*/
        U32 sk_timer2                      :6 ; /*9 :4 , seek time for current freq. Unit is 0.75ms.*/
        U32 sk_timer3                      :6 ; /*15:10, seek time for upper/lower adjacent freq. Unit is 0.75ms.*/
        U32 sk_timer4                      :6 ; /*21:16, seek time for SNR detect. Unit is 0.75ms.*/
        U32 seek_ctrl4_reserved_0          :10; /*31:22, No description.               */
    } b;
} T_FM_DSP_seek_ctrl4;

//afc_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 afc_inv                        :1 ; /*0 :0 , inverse afc adjust value.     */
        U32 afc_range                      :6 ; /*6 :1 , afc following range. Unit is 2.5KHz.*/
        U32 afc_ct                         :6 ; /*12:7 , [5:3]: afc dc filter bandwidth setting during seek is ready.[2:0]: afc acc step setting during seek is ready.*/
        U32 afc_ct_sk                      :6 ; /*18:13, [5:3]: afc dc filter bandwidth setting during seeking.[2:0]: afc acc step setting during seeking.*/
        U32 afc_ctrl_reserved_0            :13; /*31:19, No description.               */
    } b;
} T_FM_DSP_afc_ctrl;

//agc_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 ct_endelay                     :3 ; /*2 :0 , agc enable delay time after reset.3'd0:: 0 : 0.375us3'd1:: 1 : 3us3'd2:: 2 : 6us3'd3:: 3 : 9.74us3'd4:: 4 : 13.875us3'd5:: 5 : 18us3'd6:: 6 : 21us3'd7: 7 : 24us*/
        U32 agc_index_in                   :4 ; /*6 :3 , agc initial index.            */
        U32 agc_thd                        :5 ; /*11:7 , agc digital gain threshold. Unit is 2db.*/
        U32 agc_update0                    :3 ; /*14:12, agc update time during seeking.*/
        U32 agc_update1                    :3 ; /*17:15, agc update time during normal.*/
        U32 agc_loop_gain0                 :3 ; /*20:18, agc loop gian during seeking. */
        U32 agc_loop_gain1                 :3 ; /*23:21, agc loop gain during normal.  */
        U32 agc_test                       :1 ; /*24:24, agc test mode. Fix gain.      */
        U32 agc_target_pwr                 :6 ; /*30:25, agc target power. Unit is 2db.*/
        U32 agc_ctrl0_reserved_0           :1 ; /*31:31, No description.               */
    } b;
} T_FM_DSP_agc_ctrl0;

//agc_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 log_over_sel                   :1 ; /*0 :0 , if 1, adjust agc_index sub step when log_agc&gt;log_agc_th*/
        U32 log_agc_th                     :6 ; /*6 :1 , threshold for agc lopp adjust. Unit is 1db.*/
        U32 over_step                      :6 ; /*12:7 , [5:3]: agc loop sub step when sinc_over or log_agc&gt;log_agc_th[2:0]: agc loop sub step when acc I saturation.*/
        U32 agc_ct_u                       :2 ; /*14:13, agc IIR bandwidth.            */
        U32 dig_gain_in                    :5 ; /*19:15, basic dig gain. Unit is db.   */
        U32 agc_ctrl1_reserved_0           :12; /*31:20, No description.               */
    } b;
} T_FM_DSP_agc_ctrl1;

//ana_gain_rssi_tb0
typedef union
{
    U32 v;
    struct
    {
        U32 ana_gain_rssi_0                :8 ; /*7 :0 , ana gain rssi for agc_index=0 */
        U32 ana_gain_rssi_1                :8 ; /*15:8 , ana gain rssi for agc_index=1 */
        U32 ana_gain_rssi_2                :8 ; /*23:16, ana gain rssi for agc_index=2 */
        U32 ana_gain_rssi_3                :8 ; /*31:24, ana gain rssi for agc_index=3 */
    } b;
} T_FM_DSP_ana_gain_rssi_tb0;

//ana_gain_rssi_tb1
typedef union
{
    U32 v;
    struct
    {
        U32 ana_gain_rssi_4                :8 ; /*7 :0 , ana gain rssi for agc_index=4 */
        U32 ana_gain_rssi_5                :8 ; /*15:8 , ana gain rssi for agc_index=5 */
        U32 ana_gain_rssi_6                :8 ; /*23:16, ana gain rssi for agc_index=6 */
        U32 ana_gain_rssi_7                :8 ; /*31:24, ana gain rssi for agc_index=7 */
    } b;
} T_FM_DSP_ana_gain_rssi_tb1;

//ana_gain_rssi_tb2
typedef union
{
    U32 v;
    struct
    {
        U32 ana_gain_rssi_8                :8 ; /*7 :0 , ana gain rssi for agc_index=8 */
        U32 ana_gain_rssi_9                :8 ; /*15:8 , ana gain rssi for agc_index=9 */
        U32 ana_gain_rssi_a                :8 ; /*23:16, ana gain rssi for agc_index=a */
        U32 ana_gain_rssi_b                :8 ; /*31:24, ana gain rssi for agc_index=b */
    } b;
} T_FM_DSP_ana_gain_rssi_tb2;

//ana_gain_rssi_tb3
typedef union
{
    U32 v;
    struct
    {
        U32 ana_gain_rssi_c                :8 ; /*7 :0 , ana gain rssi for agc_index=c */
        U32 ana_gain_rssi_d                :8 ; /*15:8 , ana gain rssi for agc_index=d */
        U32 ana_gain_rssi_e                :8 ; /*23:16, ana gain rssi for agc_index=e */
        U32 ana_gain_rssi_f                :8 ; /*31:24, ana gain rssi for agc_index=f */
    } b;
} T_FM_DSP_ana_gain_rssi_tb3;

//noise_cancel_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 fmdem_sel_grp                  :3 ; /*2 :0 , 1'd0: select cordic fmdem1'd1: select dpll fmdem.[2]: for seeing;[1]: for nosie&lt;th_min;[0]: for noise&gt;th_max.*/
        U32 plldem_th_max                  :6 ; /*8 :3 , threshold. Unit is 2db.       */
        U32 plldem_th_min                  :6 ; /*14:9 , threshold. Unit is 2db.       */
        U32 th_max_bw                      :7 ; /*21:15, fircut/gain38k change high threshold for RSSI. Unit is 1db.*/
        U32 th_min_bw                      :7 ; /*28:22, fircut/gain38k change low threshold for RSSI. Unit is 1db.*/
        U32 noise_cancel_ctrl0_reserved_0  :3 ; /*31:29, No description.               */
    } b;
} T_FM_DSP_noise_cancel_ctrl0;

//noise_cancel_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 th_min_hcc                     :6 ; /*5 :0 , bandwidth threshold. Unit is 2db*/
        U32 sel_fircut1                    :3 ; /*8 :6 , fircut bandwidth select during seek ready and good condition. [40KHz:20KHz:180KHz]*/
        U32 sel_fircut2                    :3 ; /*11:9 , fircut bandwidth select during seek ready and bad condition.  Offset is under th. [40KHz:20KHz:180KHz]*/
        U32 sel_fircut3                    :3 ; /*14:12, fircut bandwidth select during seeready and bad conditiong. Offset is over th. CENTER. [40KHz:20KHz:180KHz]*/
        U32 sel_fircut_sk                  :6 ; /*20:15, fircut bandwidth select during seeking UPPER/LOWER[2:0] and CENTER[5:3]. [40KHz:20KHz:180KHz]*/
        U32 fircut_sel_force_on            :1 ; /*21:21, fircut/gain38k change force on*/
        U32 noise_cancel_ctrl1_reserved_0  :10; /*31:22, No description.               */
    } b;
} T_FM_DSP_noise_cancel_ctrl1;

//noise_cancel_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 gain_38k1                      :15; /*14:0 , bandwidth threshold. Unit is 2db*/
        U32 gain_38k2                      :15; /*29:15, fircut bandwidth select during bad condition. [40KHz:20KHz:180KHz]*/
        U32 noise_cancel_ctrl2_reserved_0  :2 ; /*31:30, No description.               */
    } b;
} T_FM_DSP_noise_cancel_ctrl2;

//datapath_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 dc_ct                          :4 ; /*3 :0 , dc cancel control.[3]: dccancel mode. 1'd0: bypass; 1'd1: enable[2:0]: bandwidth.*/
        U32 limit_sel                      :1 ; /*4 :4 , 1'd0: *0.751'd1: *1for sinc_limit.*/
        U32 over_th_sel                    :3 ; /*7 :5 , [2]: enable over threshold detection.[1:0]: over threshold selection.2'd0:: 0 : 0.752'd1:: 1 : 0.92'd2:: 2 : 0.952'd3:: 3 : 1*/
        U32 sinc_limit_gain                :2 ; /*9 :8 , dig gain for signal before 125KHz mixer. 2'd0:: 0db2'd1:: 6db2'd2:: 12db3'd3:: 18db*/
        U32 dig_delayct                    :5 ; /*14:10, dig gain change delay setting. Unit is 0.375us*/
        U32 gainct                         :9 ; /*23:15, [8:6]: gain for mpx signal.[5:0]: gain for stereo. [5:3]:6db;[2:1]:2db;[0]:1db.*/
        U32 hbf2_bypass                    :1 ; /*24:24, No description.               */
        U32 hbf1_bypass                    :1 ; /*25:25, No description.               */
        U32 datapath_ctrl0_reserved_0      :6 ; /*31:26, No description.               */
    } b;
} T_FM_DSP_datapath_ctrl0;

//datapath_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 softmute_th3                   :6 ; /*5 :0 , softmute threshold for SNR.   */
        U32 softmute_th2                   :6 ; /*11:6 , softmute threshold for noise. */
        U32 softmute_th1                   :6 ; /*17:12, softmute threshold for RSSI.  */
        U32 nco_ct_u                       :3 ; /*20:18, nco dc bandwidth.             */
        U32 nco_ct_bw                      :4 ; /*24:21, nco 2ord bandwidth.           */
        U32 nco_ct_u_dt_sk                 :3 ; /*27:25, 19kHz tone detect bandwidth during seeking.*/
        U32 nco_ct_u_dt                    :3 ; /*30:28, 19kHz tone detect bandwidthduring normal.*/
        U32 datapath_ctrl1_reserved_0      :1 ; /*31:31, No description.               */
    } b;
} T_FM_DSP_datapath_ctrl1;

//datapath_ctrl2
typedef union
{
    U32 v;
    struct
    {
        U32 deemph_hcc                     :2 ; /*1 :0 , direct deemphasis hcc reg.    */
        U32 lpf_bw_reg                     :3 ; /*4 :2 , No description.               */
        U32 lpf_bw_dr                      :1 ; /*5 :5 , No description.               */
        U32 offset_flt_ct_u                :2 ; /*7 :6 , offset filter bandwidth.      */
        U32 th_softblend2                  :6 ; /*13:8 , softblend threshold for noise.*/
        U32 th_softblend1                  :6 ; /*19:14, softblend threshold for RSSI. */
        U32 sm_attenu                      :4 ; /*23:20, softmute attenu setting.      */
        U32 sm_rate                        :2 ; /*25:24, softmute rate. Fast-&gt;slow. */
        U32 softmute_th4                   :6 ; /*31:26, softmute threshold for offset.*/
    } b;
} T_FM_DSP_datapath_ctrl2;

//datapath_ctrl3
typedef union
{
    U32 v;
    struct
    {
        U32 interval_reg                   :16; /*15:0 , interval value                */
        U32 step19k_reg                    :15; /*30:16, step 19k value                */
        U32 datapath_ctrl3_reserved_0      :1 ; /*31:31, No description.               */
    } b;
} T_FM_DSP_datapath_ctrl3;

//datapath_ctrl4
typedef union
{
    U32 v;
    struct
    {
        U32 notch_flt_z0_i                 :20; /*19:0 , No description.               */
        U32 notch_flt_ka                   :2 ; /*21:20, No description.               */
        U32 notch_flt_bypass               :1 ; /*22:22, No description.               */
        U32 datapath_ctrl4_reserved_0      :9 ; /*31:23, No description.               */
    } b;
} T_FM_DSP_datapath_ctrl4;

//datapath_ctrl5
typedef union
{
    U32 v;
    struct
    {
        U32 notch_flt_z0_q                 :20; /*19:0 , No description.               */
        U32 datapath_ctrl5_reserved_0      :12; /*31:20, No description.               */
    } b;
} T_FM_DSP_datapath_ctrl5;

//log_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 ct_u_pilot                     :6 ; /*5 :0 , [5:3]: pilot_db1 bandwidth[2:0]: pilot_db2 bandwitdh*/
        U32 ct_u_rssi                      :6 ; /*11:6 , [5:3]: rssi_db1 bandwidth[2:0]: rssi_db2 bandwidth*/
        U32 ct_u_signal                    :6 ; /*17:12, [5:3]: signal_db1 bandwidth[2:0]: signal_db2 bandwidth*/
        U32 ct_u_noise                     :6 ; /*23:18, [5:3]: noise_db1 bandwidth[2:0]: noise_db2 bandwidth*/
        U32 log_ctrl0_reserved_0           :8 ; /*31:24, No description.               */
    } b;
} T_FM_DSP_log_ctrl0;

//status0
typedef union
{
    U32 v;
    struct
    {
        U32 offset2                        :8 ; /*7 :0 , frequency offset. Unit is db. */
        U32 signal_db2                     :7 ; /*14:8 , signal. Unit is db.           */
        U32 snr_out                        :6 ; /*20:15, snr. Unit is db.              */
        U32 rssi_db2                       :7 ; /*27:21, rssi. Unit is db.             */
        U32 seek_done                      :1 ; /*28:28, No description.               */
        U32 seek_ready                     :1 ; /*29:29, No description.               */
        U32 status0_reserved_0             :2 ; /*31:30, No description.               */
    } b;
} T_FM_DSP_status0;

//status1
typedef union
{
    U32 v;
    struct
    {
        U32 snr_cnt                        :4 ; /*3 :0 , No description.               */
        U32 sk_cmp_grp_flag                :5 ; /*8 :4 , [4]: 19k pilot flag[3]: offset flag[2]: snr flag[1]: rssi flag[0]: noise flag*/
        U32 pilot_db2                      :7 ; /*15:9 , No description.               */
        U32 snr19k_out                     :6 ; /*21:16, No description.               */
        U32 noise_db2                      :7 ; /*28:22, No description.               */
        U32 status1_reserved_0             :3 ; /*31:29, No description.               */
    } b;
} T_FM_DSP_status1;

//status2
typedef union
{
    U32 v;
    struct
    {
        U32 offset_cnt                     :8 ; /*7 :0 , No description.               */
        U32 pilot_cnt                      :8 ; /*15:8 , No description.               */
        U32 noise_cnt                      :8 ; /*23:16, No description.               */
        U32 rssi_cnt                       :8 ; /*31:24, No description.               */
    } b;
} T_FM_DSP_status2;

//status3
typedef union
{
    U32 v;
    struct
    {
        U32 agc_index                      :4 ; /*3 :0 , No description.               */
        U32 dig_gainct                     :8 ; /*11:4 , No description.               */
        U32 fmdem_sel                      :1 ; /*12:12, No description.               */
        U32 fircut_bw                      :3 ; /*15:13, No description.               */
        U32 ct_hcc                         :3 ; /*18:16, No description.               */
        U32 status3_reserved_0             :13; /*31:19, No description.               */
    } b;
} T_FM_DSP_status3;

//rsvd_reg
typedef union
{
    U32 v;
    struct
    {
        U32 rsvd_out                       :16; /*15:0 , No description.               */
        U32 rsvd_in                        :16; /*31:16, No description.               */
    } b;
} T_FM_DSP_rsvd_reg;

typedef struct
{
    volatile          T_FM_DSP_tx_fifo_rdata                  tx_fifo_rdata; /*0x0  , RO   , 0x00000000, No description.               */
    volatile               T_FM_DSP_revision                       revision; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                   T_FM_DSP_ctrl                           ctrl; /*0x8  , RW   , 0x00550003, No description.               */
    volatile                T_FM_DSP_tx_ctrl                        tx_ctrl; /*0xc  , RW   , 0x00000000, No description.               */
    volatile             T_FM_DSP_seek_ctrl0                     seek_ctrl0; /*0x10 , RW   , 0x14C4A91B, No description.               */
    volatile             T_FM_DSP_seek_ctrl1                     seek_ctrl1; /*0x14 , RW   , 0x32323232, No description.               */
    volatile             T_FM_DSP_seek_ctrl2                     seek_ctrl2; /*0x18 , RW   , 0x08D18794, No description.               */
    volatile             T_FM_DSP_seek_ctrl3                     seek_ctrl3; /*0x1c , RW   , 0x0206430A, No description.               */
    volatile             T_FM_DSP_seek_ctrl4                     seek_ctrl4; /*0x20 , RW   , 0x00281472, No description.               */
    volatile               T_FM_DSP_afc_ctrl                       afc_ctrl; /*0x24 , RW   , 0x0002530F, No description.               */
    volatile              T_FM_DSP_agc_ctrl0                      agc_ctrl0; /*0x28 , RW   , 0x4C9E087C, No description.               */
    volatile              T_FM_DSP_agc_ctrl1                      agc_ctrl1; /*0x2c , RW   , 0x00003179, No description.               */
    volatile      T_FM_DSP_ana_gain_rssi_tb0              ana_gain_rssi_tb0; /*0x30 , RW   , 0xEAF40000, No description.               */
    volatile      T_FM_DSP_ana_gain_rssi_tb1              ana_gain_rssi_tb1; /*0x34 , RW   , 0x120C0600, No description.               */
    volatile      T_FM_DSP_ana_gain_rssi_tb2              ana_gain_rssi_tb2; /*0x38 , RW   , 0x2A241E18, No description.               */
    volatile      T_FM_DSP_ana_gain_rssi_tb3              ana_gain_rssi_tb3; /*0x3c , RW   , 0x423C3630, No description.               */
    volatile     T_FM_DSP_noise_cancel_ctrl0             noise_cancel_ctrl0; /*0x40 , RW   , 0x040C3F10, No description.               */
    volatile     T_FM_DSP_noise_cancel_ctrl1             noise_cancel_ctrl1; /*0x44 , RW   , 0x0004C9E0, No description.               */
    volatile     T_FM_DSP_noise_cancel_ctrl2             noise_cancel_ctrl2; /*0x48 , RW   , 0x2726CE4D, No description.               */
    volatile         T_FM_DSP_datapath_ctrl0                 datapath_ctrl0; /*0x4c , RW   , 0x0043289B, No description.               */
    volatile         T_FM_DSP_datapath_ctrl1                 datapath_ctrl1; /*0x50 , RW   , 0x674A2505, No description.               */
    volatile         T_FM_DSP_datapath_ctrl2                 datapath_ctrl2; /*0x54 , RW   , 0xD1F52283, No description.               */
    volatile         T_FM_DSP_datapath_ctrl3                 datapath_ctrl3; /*0x58 , RW   , 0x3A5E3000, No description.               */
    volatile         T_FM_DSP_datapath_ctrl4                 datapath_ctrl4; /*0x5c , RW   , 0x00700000, No description.               */
    volatile         T_FM_DSP_datapath_ctrl5                 datapath_ctrl5; /*0x60 , RW   , 0x00000000, No description.               */
    volatile              T_FM_DSP_log_ctrl0                      log_ctrl0; /*0x64 , RW   , 0x0071C71C, No description.               */
    volatile                T_FM_DSP_status0                        status0; /*0x68 , RO   , 0x00000000, No description.               */
    volatile                T_FM_DSP_status1                        status1; /*0x6c , RO   , 0x00000000, No description.               */
    volatile                T_FM_DSP_status2                        status2; /*0x70 , RO   , 0x00000000, No description.               */
    volatile                T_FM_DSP_status3                        status3; /*0x74 , RO   , 0x00000000, No description.               */
    volatile               T_FM_DSP_rsvd_reg                       rsvd_reg; /*0x78 , RW   , 0x0000FF00, No description.               */
} T_HWP_FM_DSP_T;
#else
typedef struct
{
    volatile                             U32                  tx_fifo_rdata; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32                       revision; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                           ctrl; /*0x8  , RW   , 0x00550003, No description.               */
    volatile                             U32                        tx_ctrl; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                     seek_ctrl0; /*0x10 , RW   , 0x14C4A91B, No description.               */
    volatile                             U32                     seek_ctrl1; /*0x14 , RW   , 0x32323232, No description.               */
    volatile                             U32                     seek_ctrl2; /*0x18 , RW   , 0x08D18794, No description.               */
    volatile                             U32                     seek_ctrl3; /*0x1c , RW   , 0x0206430A, No description.               */
    volatile                             U32                     seek_ctrl4; /*0x20 , RW   , 0x00281472, No description.               */
    volatile                             U32                       afc_ctrl; /*0x24 , RW   , 0x0002530F, No description.               */
    volatile                             U32                      agc_ctrl0; /*0x28 , RW   , 0x4C9E087C, No description.               */
    volatile                             U32                      agc_ctrl1; /*0x2c , RW   , 0x00003179, No description.               */
    volatile                             U32              ana_gain_rssi_tb0; /*0x30 , RW   , 0xEAF40000, No description.               */
    volatile                             U32              ana_gain_rssi_tb1; /*0x34 , RW   , 0x120C0600, No description.               */
    volatile                             U32              ana_gain_rssi_tb2; /*0x38 , RW   , 0x2A241E18, No description.               */
    volatile                             U32              ana_gain_rssi_tb3; /*0x3c , RW   , 0x423C3630, No description.               */
    volatile                             U32             noise_cancel_ctrl0; /*0x40 , RW   , 0x040C3F10, No description.               */
    volatile                             U32             noise_cancel_ctrl1; /*0x44 , RW   , 0x0004C9E0, No description.               */
    volatile                             U32             noise_cancel_ctrl2; /*0x48 , RW   , 0x2726CE4D, No description.               */
    volatile                             U32                 datapath_ctrl0; /*0x4c , RW   , 0x0043289B, No description.               */
    volatile                             U32                 datapath_ctrl1; /*0x50 , RW   , 0x674A2505, No description.               */
    volatile                             U32                 datapath_ctrl2; /*0x54 , RW   , 0xD1F52283, No description.               */
    volatile                             U32                 datapath_ctrl3; /*0x58 , RW   , 0x3A5E3000, No description.               */
    volatile                             U32                 datapath_ctrl4; /*0x5c , RW   , 0x00700000, No description.               */
    volatile                             U32                 datapath_ctrl5; /*0x60 , RW   , 0x00000000, No description.               */
    volatile                             U32                      log_ctrl0; /*0x64 , RW   , 0x0071C71C, No description.               */
    volatile                             U32                        status0; /*0x68 , RO   , 0x00000000, No description.               */
    volatile                             U32                        status1; /*0x6c , RO   , 0x00000000, No description.               */
    volatile                             U32                        status2; /*0x70 , RO   , 0x00000000, No description.               */
    volatile                             U32                        status3; /*0x74 , RO   , 0x00000000, No description.               */
    volatile                             U32                       rsvd_reg; /*0x78 , RW   , 0x0000FF00, No description.               */
} T_HWP_FM_DSP_T;

#define FM_DSP_TX_DATA_Q(n)            (((n)&0xFFFF)<<0)   
#define FM_DSP_TX_DATA_Q_MASK          (0xFFFF<<0)         
#define FM_DSP_TX_DATA_Q_SHIFT         (0)                 

#define FM_DSP_TX_DATA_I(n)            (((n)&0xFFFF)<<16)  
#define FM_DSP_TX_DATA_I_MASK          (0xFFFF<<16)        
#define FM_DSP_TX_DATA_I_SHIFT         (16)                


#define FM_DSP_REV_ID(n)               (((n)&0xF)<<0)      
#define FM_DSP_REV_ID_MASK             (0xF<<0)            
#define FM_DSP_REV_ID_SHIFT            (0)                 


#define FM_DSP_MUTE                    (1<<0)              
#define FM_DSP_MUTE_NORMAL             (0<<0)              
#define FM_DSP_MUTE_MUTE               (1<<0)              

#define FM_DSP_STEREO_IN               (1<<1)              
#define FM_DSP_STEREO_IN_FORCE         (0<<1)              
#define FM_DSP_STEREO_IN_STEREO        (1<<1)              

#define FM_DSP_DEEMPH_MODE             (1<<2)              
#define FM_DSP_DEEMPH_MODE_75US        (0<<2)              
#define FM_DSP_DEEMPH_MODE_50US        (1<<2)              

#define FM_DSP_SOFTMUTE_EN             (1<<3)              
#define FM_DSP_SOFTMUTE_EN_DISABLE     (0<<3)              
#define FM_DSP_SOFTMUTE_EN_ENABLE      (1<<3)              

#define FM_DSP_SOFTBLEND_OFF           (1<<4)              
#define FM_DSP_SOFTBLEND_OFF_ENABLE    (0<<4)              
#define FM_DSP_SOFTBLEND_OFF_DISABLE   (1<<4)              

#define FM_DSP_AFC_DISABLE             (1<<5)              
#define FM_DSP_AFC_DISABLE_ENABLE      (0<<5)              
#define FM_DSP_AFC_DISABLE_DISABLE     (1<<5)              

#define FM_DSP_IMGREJ                  (1<<6)              
#define FM_DSP_IMGREJ_LOW              (0<<6)              
#define FM_DSP_IMGREJ_HIGH             (1<<6)              

#define FM_DSP_IMGREJ_DSP              (1<<7)              

#define FM_DSP_IQ_SWAP                 (1<<8)              

#define FM_DSP_PLLDEM_SWAP             (1<<9)              

#define FM_DSP_LR_SWAP                 (1<<10)             

#define FM_DSP_FIRCUT_BYPASS           (1<<11)             

#define FM_DSP_FIRCUT_BYPASS_SK        (1<<12)             

#define FM_DSP_LPF_BYPASS              (1<<13)             

#define FM_DSP_DEEMPH_BYPASS           (1<<14)             

#define FM_DSP_PILOT_COSINE            (1<<15)             
#define FM_DSP_PILOT_COSINE_SIN        (0<<15)             
#define FM_DSP_PILOT_COSINE_COS        (1<<15)             

#define FM_DSP_ADC_CLK_INVERT          (1<<16)             

#define FM_DSP_NOISE_SRC_SEL(n)        (((n)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_DANGLE0   (((0)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_DANGLE1   (((1)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_MASK      (0x3<<17)           
#define FM_DSP_NOISE_SRC_SEL_SHIFT     (17)                

#define FM_DSP_NOISE_CANCEL_SRC_SEL    (1<<19)             
#define FM_DSP_NOISE_CANCEL_SRC_SEL_NOISE_DB2 (0<<19)             
#define FM_DSP_NOISE_CANCEL_SRC_SEL_RSSI_DB2 (1<<19)             

#define FM_DSP_SEEK_RSSI_SRC_SEL       (1<<20)             
#define FM_DSP_SEEK_RSSI_SRC_SEL_RSSI_DB1 (0<<20)             
#define FM_DSP_SEEK_RSSI_SRC_SEL_SIGNAL_DB1 (1<<20)             

#define FM_DSP_SEEK_OFFSET_SRC_SEL     (1<<21)             
#define FM_DSP_SEEK_OFFSET_SRC_SEL_OUTPUT_AFC (0<<21)             
#define FM_DSP_SEEK_OFFSET_SRC_SEL_OUTPUT_OFFSET_FILTER (1<<21)             

#define FM_DSP_FMDEM_SRC_SEL           (1<<22)             
#define FM_DSP_FMDEM_SRC_SEL_OUTPUT_LPFIL (0<<22)             
#define FM_DSP_FMDEM_SRC_SEL_OUTPUT_DIG_GAIN (1<<22)             

#define FM_DSP_I2S_EN                  (1<<23)             
#define FM_DSP_I2S_EN_DISABLE          (0<<23)             
#define FM_DSP_I2S_EN_ENABLE           (1<<23)             

#define FM_DSP_I2S_DELAY_1T            (1<<24)             

#define FM_DSP_TX_EN                   (1<<25)             
#define FM_DSP_TX_EN_DISABLE           (0<<25)             
#define FM_DSP_TX_EN_ENABLE            (1<<25)             

#define FM_DSP_FORCE_CLK_ON            (1<<26)             

#define FM_DSP_ADC_DATA_FORMAT         (1<<27)             
#define FM_DSP_ADC_DATA_FORMAT_UNSIGNED (0<<27)             
#define FM_DSP_ADC_DATA_FORMAT_2S_COMPLEMENTARY (1<<27)             

#define FM_DSP_DBG_OUT_EN              (1<<28)             


#define FM_DSP_TX_SEL(n)               (((n)&0x1F)<<0)     
#define FM_DSP_TX_SEL_MASK             (0x1F<<0)           
#define FM_DSP_TX_SEL_SHIFT            (0)                 

#define FM_DSP_TX_FIFO_CLR             (1<<5)              

#define FM_DSP_TX_FIFO_OVERFLOW        (1<<6)              

#define FM_DSP_TX_FIFO_UNDERFLOW       (1<<7)              

#define FM_DSP_TX_FIFO_USEDW(n)        (((n)&0x7)<<8)      
#define FM_DSP_TX_FIFO_USEDW_MASK      (0x7<<8)            
#define FM_DSP_TX_FIFO_USEDW_SHIFT     (8)                 


#define FM_DSP_SK_CMP_GRP_EN(n)        (((n)&0x1F)<<0)     
#define FM_DSP_SK_CMP_GRP_EN_MASK      (0x1F<<0)           
#define FM_DSP_SK_CMP_GRP_EN_SHIFT     (0)                 

#define FM_DSP_SEEK_MODE(n)            (((n)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_ONLY (((0)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_OR_ADJACENT (((1)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_AND_ADJACENT (((2)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SNR_ST        (((3)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_CENTER_ST     (((4)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_UPPER_ST      (((5)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_LOWER_ST      (((6)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_BYPASS   (((7)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_MASK          (0x7<<5)            
#define FM_DSP_SEEK_MODE_SHIFT         (5)                 

#define FM_DSP_SEEK_AFC_ON             (1<<8)              
#define FM_DSP_SEEK_AFC_ON_DISABLE     (0<<8)              
#define FM_DSP_SEEK_AFC_ON_ENABLE      (1<<8)              

#define FM_DSP_SEEK_RANGE(n)           (((n)&0x1F)<<9)     
#define FM_DSP_SEEK_RANGE_MASK         (0x1F<<9)           
#define FM_DSP_SEEK_RANGE_SHIFT        (9)                 

#define FM_DSP_SNR_TH(n)               (((n)&0x3F)<<14)    
#define FM_DSP_SNR_TH_MASK             (0x3F<<14)          
#define FM_DSP_SNR_TH_SHIFT            (14)                

#define FM_DSP_DELTA_RSSI_TH(n)        (((n)&0x3F)<<20)    
#define FM_DSP_DELTA_RSSI_TH_MASK      (0x3F<<20)          
#define FM_DSP_DELTA_RSSI_TH_SHIFT     (20)                

#define FM_DSP_SNR_CNT_TH(n)           (((n)&0xF)<<26)     
#define FM_DSP_SNR_CNT_TH_MASK         (0xF<<26)           
#define FM_DSP_SNR_CNT_TH_SHIFT        (26)                


#define FM_DSP_OFFSET_CNT_TH(n)        (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET_CNT_TH_MASK      (0xFF<<0)           
#define FM_DSP_OFFSET_CNT_TH_SHIFT     (0)                 

#define FM_DSP_NOISE_CNT_TH(n)         (((n)&0xFF)<<8)     
#define FM_DSP_NOISE_CNT_TH_MASK       (0xFF<<8)           
#define FM_DSP_NOISE_CNT_TH_SHIFT      (8)                 

#define FM_DSP_RSSI_CNT_TH(n)          (((n)&0xFF)<<16)    
#define FM_DSP_RSSI_CNT_TH_MASK        (0xFF<<16)          
#define FM_DSP_RSSI_CNT_TH_SHIFT       (16)                

#define FM_DSP_PILOT_CNT_TH(n)         (((n)&0xFF)<<24)    
#define FM_DSP_PILOT_CNT_TH_MASK       (0xFF<<24)          
#define FM_DSP_PILOT_CNT_TH_SHIFT      (24)                


#define FM_DSP_RSSI_H_TH(n)            (((n)&0x7F)<<0)     
#define FM_DSP_RSSI_H_TH_MASK          (0x7F<<0)           
#define FM_DSP_RSSI_H_TH_SHIFT         (0)                 

#define FM_DSP_RSSI_L_TH(n)            (((n)&0x7F)<<7)     
#define FM_DSP_RSSI_L_TH_MASK          (0x7F<<7)           
#define FM_DSP_RSSI_L_TH_SHIFT         (7)                 

#define FM_DSP_NOISE_H_TH(n)           (((n)&0x7F)<<14)    
#define FM_DSP_NOISE_H_TH_MASK         (0x7F<<14)          
#define FM_DSP_NOISE_H_TH_SHIFT        (14)                

#define FM_DSP_NOISE_L_TH(n)           (((n)&0x7F)<<21)    
#define FM_DSP_NOISE_L_TH_MASK         (0x7F<<21)          
#define FM_DSP_NOISE_L_TH_SHIFT        (21)                


#define FM_DSP_PILOT_H_TH(n)           (((n)&0x7F)<<0)     
#define FM_DSP_PILOT_H_TH_MASK         (0x7F<<0)           
#define FM_DSP_PILOT_H_TH_SHIFT        (0)                 

#define FM_DSP_PILOT_L_TH(n)           (((n)&0x7F)<<7)     
#define FM_DSP_PILOT_L_TH_MASK         (0x7F<<7)           
#define FM_DSP_PILOT_L_TH_SHIFT        (7)                 

#define FM_DSP_OFFSET_H_TH(n)          (((n)&0x7F)<<14)    
#define FM_DSP_OFFSET_H_TH_MASK        (0x7F<<14)          
#define FM_DSP_OFFSET_H_TH_SHIFT       (14)                

#define FM_DSP_OFFSET_L_TH(n)          (((n)&0x7F)<<21)    
#define FM_DSP_OFFSET_L_TH_MASK        (0x7F<<21)          
#define FM_DSP_OFFSET_L_TH_SHIFT       (21)                


#define FM_DSP_SK_TIMER1(n)            (((n)&0xF)<<0)      
#define FM_DSP_SK_TIMER1_MASK          (0xF<<0)            
#define FM_DSP_SK_TIMER1_SHIFT         (0)                 

#define FM_DSP_SK_TIMER2(n)            (((n)&0x3F)<<4)     
#define FM_DSP_SK_TIMER2_MASK          (0x3F<<4)           
#define FM_DSP_SK_TIMER2_SHIFT         (4)                 

#define FM_DSP_SK_TIMER3(n)            (((n)&0x3F)<<10)    
#define FM_DSP_SK_TIMER3_MASK          (0x3F<<10)          
#define FM_DSP_SK_TIMER3_SHIFT         (10)                

#define FM_DSP_SK_TIMER4(n)            (((n)&0x3F)<<16)    
#define FM_DSP_SK_TIMER4_MASK          (0x3F<<16)          
#define FM_DSP_SK_TIMER4_SHIFT         (16)                


#define FM_DSP_AFC_INV                 (1<<0)              

#define FM_DSP_AFC_RANGE(n)            (((n)&0x3F)<<1)     
#define FM_DSP_AFC_RANGE_MASK          (0x3F<<1)           
#define FM_DSP_AFC_RANGE_SHIFT         (1)                 

#define FM_DSP_AFC_CT(n)               (((n)&0x3F)<<7)     
#define FM_DSP_AFC_CT_MASK             (0x3F<<7)           
#define FM_DSP_AFC_CT_SHIFT            (7)                 

#define FM_DSP_AFC_CT_SK(n)            (((n)&0x3F)<<13)    
#define FM_DSP_AFC_CT_SK_MASK          (0x3F<<13)          
#define FM_DSP_AFC_CT_SK_SHIFT         (13)                


#define FM_DSP_CT_ENDELAY(n)           (((n)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_0            (((0)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_1            (((1)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_2            (((2)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_3            (((3)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_4            (((4)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_5            (((5)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_6            (((6)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_MASK         (0x7<<0)            
#define FM_DSP_CT_ENDELAY_SHIFT        (0)                 

#define FM_DSP_AGC_INDEX_IN(n)         (((n)&0xF)<<3)      
#define FM_DSP_AGC_INDEX_IN_MASK       (0xF<<3)            
#define FM_DSP_AGC_INDEX_IN_SHIFT      (3)                 

#define FM_DSP_AGC_THD(n)              (((n)&0x1F)<<7)     
#define FM_DSP_AGC_THD_MASK            (0x1F<<7)           
#define FM_DSP_AGC_THD_SHIFT           (7)                 

#define FM_DSP_AGC_UPDATE0(n)          (((n)&0x7)<<12)     
#define FM_DSP_AGC_UPDATE0_MASK        (0x7<<12)           
#define FM_DSP_AGC_UPDATE0_SHIFT       (12)                

#define FM_DSP_AGC_UPDATE1(n)          (((n)&0x7)<<15)     
#define FM_DSP_AGC_UPDATE1_MASK        (0x7<<15)           
#define FM_DSP_AGC_UPDATE1_SHIFT       (15)                

#define FM_DSP_AGC_LOOP_GAIN0(n)       (((n)&0x7)<<18)     
#define FM_DSP_AGC_LOOP_GAIN0_MASK     (0x7<<18)           
#define FM_DSP_AGC_LOOP_GAIN0_SHIFT    (18)                

#define FM_DSP_AGC_LOOP_GAIN1(n)       (((n)&0x7)<<21)     
#define FM_DSP_AGC_LOOP_GAIN1_MASK     (0x7<<21)           
#define FM_DSP_AGC_LOOP_GAIN1_SHIFT    (21)                

#define FM_DSP_AGC_TEST                (1<<24)             

#define FM_DSP_AGC_TARGET_PWR(n)       (((n)&0x3F)<<25)    
#define FM_DSP_AGC_TARGET_PWR_MASK     (0x3F<<25)          
#define FM_DSP_AGC_TARGET_PWR_SHIFT    (25)                


#define FM_DSP_LOG_OVER_SEL            (1<<0)              

#define FM_DSP_LOG_AGC_TH(n)           (((n)&0x3F)<<1)     
#define FM_DSP_LOG_AGC_TH_MASK         (0x3F<<1)           
#define FM_DSP_LOG_AGC_TH_SHIFT        (1)                 

#define FM_DSP_OVER_STEP(n)            (((n)&0x3F)<<7)     
#define FM_DSP_OVER_STEP_MASK          (0x3F<<7)           
#define FM_DSP_OVER_STEP_SHIFT         (7)                 

#define FM_DSP_AGC_CT_U(n)             (((n)&0x3)<<13)     
#define FM_DSP_AGC_CT_U_MASK           (0x3<<13)           
#define FM_DSP_AGC_CT_U_SHIFT          (13)                

#define FM_DSP_DIG_GAIN_IN(n)          (((n)&0x1F)<<15)    
#define FM_DSP_DIG_GAIN_IN_MASK        (0x1F<<15)          
#define FM_DSP_DIG_GAIN_IN_SHIFT       (15)                


#define FM_DSP_ANA_GAIN_RSSI_0(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_0_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_0_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_1(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_1_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_1_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_2(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_2_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_2_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_3(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_3_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_3_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_4(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_4_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_4_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_5(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_5_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_5_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_6(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_6_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_6_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_7(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_7_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_7_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_8(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_8_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_8_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_9(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_9_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_9_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_A(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_A_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_A_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_B(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_B_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_B_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_C(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_C_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_C_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_D(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_D_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_D_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_E(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_E_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_E_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_F(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_F_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_F_SHIFT   (24)                


#define FM_DSP_FMDEM_SEL_GRP(n)        (((n)&0x7)<<0)      
#define FM_DSP_FMDEM_SEL_GRP_MASK      (0x7<<0)            
#define FM_DSP_FMDEM_SEL_GRP_SHIFT     (0)                 

#define FM_DSP_PLLDEM_TH_MAX(n)        (((n)&0x3F)<<3)     
#define FM_DSP_PLLDEM_TH_MAX_MASK      (0x3F<<3)           
#define FM_DSP_PLLDEM_TH_MAX_SHIFT     (3)                 

#define FM_DSP_PLLDEM_TH_MIN(n)        (((n)&0x3F)<<9)     
#define FM_DSP_PLLDEM_TH_MIN_MASK      (0x3F<<9)           
#define FM_DSP_PLLDEM_TH_MIN_SHIFT     (9)                 

#define FM_DSP_TH_MAX_BW(n)            (((n)&0x7F)<<15)    
#define FM_DSP_TH_MAX_BW_MASK          (0x7F<<15)          
#define FM_DSP_TH_MAX_BW_SHIFT         (15)                

#define FM_DSP_TH_MIN_BW(n)            (((n)&0x7F)<<22)    
#define FM_DSP_TH_MIN_BW_MASK          (0x7F<<22)          
#define FM_DSP_TH_MIN_BW_SHIFT         (22)                


#define FM_DSP_TH_MIN_HCC(n)           (((n)&0x3F)<<0)     
#define FM_DSP_TH_MIN_HCC_MASK         (0x3F<<0)           
#define FM_DSP_TH_MIN_HCC_SHIFT        (0)                 

#define FM_DSP_SEL_FIRCUT1(n)          (((n)&0x7)<<6)      
#define FM_DSP_SEL_FIRCUT1_MASK        (0x7<<6)            
#define FM_DSP_SEL_FIRCUT1_SHIFT       (6)                 

#define FM_DSP_SEL_FIRCUT2(n)          (((n)&0x7)<<9)      
#define FM_DSP_SEL_FIRCUT2_MASK        (0x7<<9)            
#define FM_DSP_SEL_FIRCUT2_SHIFT       (9)                 

#define FM_DSP_SEL_FIRCUT3(n)          (((n)&0x7)<<12)     
#define FM_DSP_SEL_FIRCUT3_MASK        (0x7<<12)           
#define FM_DSP_SEL_FIRCUT3_SHIFT       (12)                

#define FM_DSP_SEL_FIRCUT_SK(n)        (((n)&0x3F)<<15)    
#define FM_DSP_SEL_FIRCUT_SK_MASK      (0x3F<<15)          
#define FM_DSP_SEL_FIRCUT_SK_SHIFT     (15)                

#define FM_DSP_FIRCUT_SEL_FORCE_ON     (1<<21)             


#define FM_DSP_GAIN_38K1(n)            (((n)&0x7FFF)<<0)   
#define FM_DSP_GAIN_38K1_MASK          (0x7FFF<<0)         
#define FM_DSP_GAIN_38K1_SHIFT         (0)                 

#define FM_DSP_GAIN_38K2(n)            (((n)&0x7FFF)<<15)  
#define FM_DSP_GAIN_38K2_MASK          (0x7FFF<<15)        
#define FM_DSP_GAIN_38K2_SHIFT         (15)                


#define FM_DSP_DC_CT(n)                (((n)&0xF)<<0)      
#define FM_DSP_DC_CT_MASK              (0xF<<0)            
#define FM_DSP_DC_CT_SHIFT             (0)                 

#define FM_DSP_LIMIT_SEL               (1<<4)              

#define FM_DSP_OVER_TH_SEL(n)          (((n)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_0           (((0)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_1           (((1)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_2           (((2)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_3           (((3)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_MASK        (0x7<<5)            
#define FM_DSP_OVER_TH_SEL_SHIFT       (5)                 

#define FM_DSP_SINC_LIMIT_GAIN(n)      (((n)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_0DB     (((0)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_6DB     (((1)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_12DB    (((2)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_18DB    (((3)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_MASK    (0x3<<8)            
#define FM_DSP_SINC_LIMIT_GAIN_SHIFT   (8)                 

#define FM_DSP_DIG_DELAYCT(n)          (((n)&0x1F)<<10)    
#define FM_DSP_DIG_DELAYCT_MASK        (0x1F<<10)          
#define FM_DSP_DIG_DELAYCT_SHIFT       (10)                

#define FM_DSP_GAINCT(n)               (((n)&0x1FF)<<15)   
#define FM_DSP_GAINCT_MASK             (0x1FF<<15)         
#define FM_DSP_GAINCT_SHIFT            (15)                

#define FM_DSP_HBF2_BYPASS             (1<<24)             

#define FM_DSP_HBF1_BYPASS             (1<<25)             


#define FM_DSP_SOFTMUTE_TH3(n)         (((n)&0x3F)<<0)     
#define FM_DSP_SOFTMUTE_TH3_MASK       (0x3F<<0)           
#define FM_DSP_SOFTMUTE_TH3_SHIFT      (0)                 

#define FM_DSP_SOFTMUTE_TH2(n)         (((n)&0x3F)<<6)     
#define FM_DSP_SOFTMUTE_TH2_MASK       (0x3F<<6)           
#define FM_DSP_SOFTMUTE_TH2_SHIFT      (6)                 

#define FM_DSP_SOFTMUTE_TH1(n)         (((n)&0x3F)<<12)    
#define FM_DSP_SOFTMUTE_TH1_MASK       (0x3F<<12)          
#define FM_DSP_SOFTMUTE_TH1_SHIFT      (12)                

#define FM_DSP_NCO_CT_U(n)             (((n)&0x7)<<18)     
#define FM_DSP_NCO_CT_U_MASK           (0x7<<18)           
#define FM_DSP_NCO_CT_U_SHIFT          (18)                

#define FM_DSP_NCO_CT_BW(n)            (((n)&0xF)<<21)     
#define FM_DSP_NCO_CT_BW_MASK          (0xF<<21)           
#define FM_DSP_NCO_CT_BW_SHIFT         (21)                

#define FM_DSP_NCO_CT_U_DT_SK(n)       (((n)&0x7)<<25)     
#define FM_DSP_NCO_CT_U_DT_SK_MASK     (0x7<<25)           
#define FM_DSP_NCO_CT_U_DT_SK_SHIFT    (25)                

#define FM_DSP_NCO_CT_U_DT(n)          (((n)&0x7)<<28)     
#define FM_DSP_NCO_CT_U_DT_MASK        (0x7<<28)           
#define FM_DSP_NCO_CT_U_DT_SHIFT       (28)                


#define FM_DSP_DEEMPH_HCC(n)           (((n)&0x3)<<0)      
#define FM_DSP_DEEMPH_HCC_MASK         (0x3<<0)            
#define FM_DSP_DEEMPH_HCC_SHIFT        (0)                 

#define FM_DSP_LPF_BW_REG(n)           (((n)&0x7)<<2)      
#define FM_DSP_LPF_BW_REG_MASK         (0x7<<2)            
#define FM_DSP_LPF_BW_REG_SHIFT        (2)                 

#define FM_DSP_LPF_BW_DR               (1<<5)              

#define FM_DSP_OFFSET_FLT_CT_U(n)      (((n)&0x3)<<6)      
#define FM_DSP_OFFSET_FLT_CT_U_MASK    (0x3<<6)            
#define FM_DSP_OFFSET_FLT_CT_U_SHIFT   (6)                 

#define FM_DSP_TH_SOFTBLEND2(n)        (((n)&0x3F)<<8)     
#define FM_DSP_TH_SOFTBLEND2_MASK      (0x3F<<8)           
#define FM_DSP_TH_SOFTBLEND2_SHIFT     (8)                 

#define FM_DSP_TH_SOFTBLEND1(n)        (((n)&0x3F)<<14)    
#define FM_DSP_TH_SOFTBLEND1_MASK      (0x3F<<14)          
#define FM_DSP_TH_SOFTBLEND1_SHIFT     (14)                

#define FM_DSP_SM_ATTENU(n)            (((n)&0xF)<<20)     
#define FM_DSP_SM_ATTENU_MASK          (0xF<<20)           
#define FM_DSP_SM_ATTENU_SHIFT         (20)                

#define FM_DSP_SM_RATE(n)              (((n)&0x3)<<24)     
#define FM_DSP_SM_RATE_MASK            (0x3<<24)           
#define FM_DSP_SM_RATE_SHIFT           (24)                

#define FM_DSP_SOFTMUTE_TH4(n)         (((n)&0x3F)<<26)    
#define FM_DSP_SOFTMUTE_TH4_MASK       (0x3F<<26)          
#define FM_DSP_SOFTMUTE_TH4_SHIFT      (26)                


#define FM_DSP_INTERVAL_REG(n)         (((n)&0xFFFF)<<0)   
#define FM_DSP_INTERVAL_REG_MASK       (0xFFFF<<0)         
#define FM_DSP_INTERVAL_REG_SHIFT      (0)                 

#define FM_DSP_STEP19K_REG(n)          (((n)&0x7FFF)<<16)  
#define FM_DSP_STEP19K_REG_MASK        (0x7FFF<<16)        
#define FM_DSP_STEP19K_REG_SHIFT       (16)                


#define FM_DSP_NOTCH_FLT_Z0_I(n)       (((n)&0xFFFFF)<<0)  
#define FM_DSP_NOTCH_FLT_Z0_I_MASK     (0xFFFFF<<0)        
#define FM_DSP_NOTCH_FLT_Z0_I_SHIFT    (0)                 

#define FM_DSP_NOTCH_FLT_KA(n)         (((n)&0x3)<<20)     
#define FM_DSP_NOTCH_FLT_KA_MASK       (0x3<<20)           
#define FM_DSP_NOTCH_FLT_KA_SHIFT      (20)                

#define FM_DSP_NOTCH_FLT_BYPASS        (1<<22)             


#define FM_DSP_NOTCH_FLT_Z0_Q(n)       (((n)&0xFFFFF)<<0)  
#define FM_DSP_NOTCH_FLT_Z0_Q_MASK     (0xFFFFF<<0)        
#define FM_DSP_NOTCH_FLT_Z0_Q_SHIFT    (0)                 


#define FM_DSP_CT_U_PILOT(n)           (((n)&0x3F)<<0)     
#define FM_DSP_CT_U_PILOT_MASK         (0x3F<<0)           
#define FM_DSP_CT_U_PILOT_SHIFT        (0)                 

#define FM_DSP_CT_U_RSSI(n)            (((n)&0x3F)<<6)     
#define FM_DSP_CT_U_RSSI_MASK          (0x3F<<6)           
#define FM_DSP_CT_U_RSSI_SHIFT         (6)                 

#define FM_DSP_CT_U_SIGNAL(n)          (((n)&0x3F)<<12)    
#define FM_DSP_CT_U_SIGNAL_MASK        (0x3F<<12)          
#define FM_DSP_CT_U_SIGNAL_SHIFT       (12)                

#define FM_DSP_CT_U_NOISE(n)           (((n)&0x3F)<<18)    
#define FM_DSP_CT_U_NOISE_MASK         (0x3F<<18)          
#define FM_DSP_CT_U_NOISE_SHIFT        (18)                


#define FM_DSP_OFFSET2(n)              (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET2_MASK            (0xFF<<0)           
#define FM_DSP_OFFSET2_SHIFT           (0)                 

#define FM_DSP_SIGNAL_DB2(n)           (((n)&0x7F)<<8)     
#define FM_DSP_SIGNAL_DB2_MASK         (0x7F<<8)           
#define FM_DSP_SIGNAL_DB2_SHIFT        (8)                 

#define FM_DSP_SNR_OUT(n)              (((n)&0x3F)<<15)    
#define FM_DSP_SNR_OUT_MASK            (0x3F<<15)          
#define FM_DSP_SNR_OUT_SHIFT           (15)                

#define FM_DSP_RSSI_DB2(n)             (((n)&0x7F)<<21)    
#define FM_DSP_RSSI_DB2_MASK           (0x7F<<21)          
#define FM_DSP_RSSI_DB2_SHIFT          (21)                

#define FM_DSP_SEEK_DONE               (1<<28)             

#define FM_DSP_SEEK_READY              (1<<29)             


#define FM_DSP_SNR_CNT(n)              (((n)&0xF)<<0)      
#define FM_DSP_SNR_CNT_MASK            (0xF<<0)            
#define FM_DSP_SNR_CNT_SHIFT           (0)                 

#define FM_DSP_SK_CMP_GRP_FLAG(n)      (((n)&0x1F)<<4)     
#define FM_DSP_SK_CMP_GRP_FLAG_MASK    (0x1F<<4)           
#define FM_DSP_SK_CMP_GRP_FLAG_SHIFT   (4)                 

#define FM_DSP_PILOT_DB2(n)            (((n)&0x7F)<<9)     
#define FM_DSP_PILOT_DB2_MASK          (0x7F<<9)           
#define FM_DSP_PILOT_DB2_SHIFT         (9)                 

#define FM_DSP_SNR19K_OUT(n)           (((n)&0x3F)<<16)    
#define FM_DSP_SNR19K_OUT_MASK         (0x3F<<16)          
#define FM_DSP_SNR19K_OUT_SHIFT        (16)                

#define FM_DSP_NOISE_DB2(n)            (((n)&0x7F)<<22)    
#define FM_DSP_NOISE_DB2_MASK          (0x7F<<22)          
#define FM_DSP_NOISE_DB2_SHIFT         (22)                


#define FM_DSP_OFFSET_CNT(n)           (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET_CNT_MASK         (0xFF<<0)           
#define FM_DSP_OFFSET_CNT_SHIFT        (0)                 

#define FM_DSP_PILOT_CNT(n)            (((n)&0xFF)<<8)     
#define FM_DSP_PILOT_CNT_MASK          (0xFF<<8)           
#define FM_DSP_PILOT_CNT_SHIFT         (8)                 

#define FM_DSP_NOISE_CNT(n)            (((n)&0xFF)<<16)    
#define FM_DSP_NOISE_CNT_MASK          (0xFF<<16)          
#define FM_DSP_NOISE_CNT_SHIFT         (16)                

#define FM_DSP_RSSI_CNT(n)             (((n)&0xFF)<<24)    
#define FM_DSP_RSSI_CNT_MASK           (0xFF<<24)          
#define FM_DSP_RSSI_CNT_SHIFT          (24)                


#define FM_DSP_AGC_INDEX(n)            (((n)&0xF)<<0)      
#define FM_DSP_AGC_INDEX_MASK          (0xF<<0)            
#define FM_DSP_AGC_INDEX_SHIFT         (0)                 

#define FM_DSP_DIG_GAINCT(n)           (((n)&0xFF)<<4)     
#define FM_DSP_DIG_GAINCT_MASK         (0xFF<<4)           
#define FM_DSP_DIG_GAINCT_SHIFT        (4)                 

#define FM_DSP_FMDEM_SEL               (1<<12)             

#define FM_DSP_FIRCUT_BW(n)            (((n)&0x7)<<13)     
#define FM_DSP_FIRCUT_BW_MASK          (0x7<<13)           
#define FM_DSP_FIRCUT_BW_SHIFT         (13)                

#define FM_DSP_CT_HCC(n)               (((n)&0x7)<<16)     
#define FM_DSP_CT_HCC_MASK             (0x7<<16)           
#define FM_DSP_CT_HCC_SHIFT            (16)                


#define FM_DSP_RSVD_OUT(n)             (((n)&0xFFFF)<<0)   
#define FM_DSP_RSVD_OUT_MASK           (0xFFFF<<0)         
#define FM_DSP_RSVD_OUT_SHIFT          (0)                 

#define FM_DSP_RSVD_IN(n)              (((n)&0xFFFF)<<16)  
#define FM_DSP_RSVD_IN_MASK            (0xFFFF<<16)        
#define FM_DSP_RSVD_IN_SHIFT           (16)                


#endif

#endif


