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
// File name      : rf_interface                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:52                                                  
// Last modified  : 2016-08-29 14:59:52                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:52 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __RF_INTERFACE_H__
#define __RF_INTERFACE_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//revision
typedef union
{
    U32 v;
    struct
    {
        U32 revid                          :4 ; /*3 :0 , Revision ID.                  */
        U32 chipid                         :12; /*15:4 , Chip ID.Read only.            */
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
} T_RF_INTERFACE_revision;

//sys_control
typedef union
{
    U32 v;
    struct
    {
        U32 soft_resetn                    :1 ; /*0 :0 , If 0, then reset all the register of chip.*/
        U32 chip_self_cal_enable           :1 ; /*1 :1 , Chip self_cal enable. Self cal process will be triggered at posedge of chip_self_cal_enable.*/
        U32 pll_freq_mode                  :1 ; /*2 :2 , If 1, frequency decided by freq register;_x000D_If 0, frequency decided by channel number.*/
        U32 zf_nzf                         :1 ; /*3 :3 , 1: zero frequency;_x000D_0:nearly zero frequency.*/
        U32 pd_all_b_reg                   :1 ; /*4 :4 , If 0 ,then power down all blocks.*/
        U32 direct_reg                     :1 ; /*5 :5 , 1: pu use register values._x000D_0: use logic values.*/
        U32 tune_diff_en                   :1 ; /*6 :6 , ARFCN trigger mode enable.    */
        U32 clk_digital_enable_reg         :1 ; /*7 :7 , Register control of digital clk enable.*/
        U32 rf_mode                        :1 ; /*8 :8 , RF mode. 1'd0: BT; 1'd1: FM.  */
        U32 sys_control_reserved_0         :7 ; /*15:9 , No description.               */
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
} T_RF_INTERFACE_sys_control;

//bt_control
typedef union
{
    U32 v;
    struct
    {
        U32 arfcn                          :7 ; /*6 :0 , Channel number, to be used when pll_freq_mode=0. _x000D_7'h00 : Channel0 _x000D_7'h4E : Channel78*/
        U32 apc                            :4 ; /*10:7 , padrv_gain index              */
        U32 bt_control_reserved_0          :5 ; /*15:11, No description.               */
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
} T_RF_INTERFACE_bt_control;

//bt_status
typedef union
{
    U32 v;
    struct
    {
        U32 bt_self_cal_ready              :1 ; /*0 :0 , bt self calibration ready.    */
        U32 bt_status_reserved_0           :15; /*15:1 , No description.               */
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
} T_RF_INTERFACE_bt_status;

//fm_control0
typedef union
{
    U32 v;
    struct
    {
        U32 tune                           :1 ; /*0 :0 , start tune                    */
        U32 seek                           :1 ; /*1 :1 , start seek                    */
        U32 skmode                         :1 ; /*2 :2 , seek mode.1'd0: wrap at the upper or lower band limit and continue seeking1'd1: stop seeking at the upper or lower band limit*/
        U32 seekup                         :1 ; /*3 :3 , seek up.1'd0: seek down1'd1: seek up*/
        U32 band_sel                       :2 ; /*5 :4 , band select.2'd0: 87-108MHz (US/Europe)2'd1: 76-91MHz (Japan)2'd2: 76-108MHz (World Wide)2'd3: 65-76MHz (East Europe)*/
        U32 space_sel                      :2 ; /*7 :6 , channel space select.2'd0: 25KHz2'd1: 50KHz2'd2: 100KHz2'd3: 200KHz*/
        U32 fm_freq_mode                   :1 ; /*8 :8 , direct freq mode.If 1, frequency=bottom frequency+freq_direct*/
        U32 fm_control0_reserved_0         :7 ; /*15:9 , No description.               */
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
} T_RF_INTERFACE_fm_control0;

//fm_control1
typedef union
{
    U32 v;
    struct
    {
        U32 chan_reg                       :10; /*9 :0 , channel select.Frequency = channel space*chan + bottom fequency*/
        U32 fm_control1_reserved_0         :6 ; /*15:10, No description.               */
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
} T_RF_INTERFACE_fm_control1;

//fm_control2
typedef union
{
    U32 v;
    struct
    {
        U32 fm_freq_direct                 :16; /*15:0 , frequency direct. U6.10, unit is MHz*/
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
} T_RF_INTERFACE_fm_control2;

//fm_status
typedef union
{
    U32 v;
    struct
    {
        U32 fm_self_cal_ready              :1 ; /*0 :0 , fm self calibration is ready  */
        U32 stc                            :1 ; /*1 :1 , seek/tune complete            */
        U32 sf                             :1 ; /*2 :2 , seek fail                     */
        U32 fm_true                        :1 ; /*3 :3 , fm is a true statin           */
        U32 fm_ready                       :1 ; /*4 :4 , fm is ready                   */
        U32 readchan                       :10; /*14:5 , the processing channel        */
        U32 fm_status_reserved_0           :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_fm_status;

//pll_freq_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 pll_freq_synthesize_msb        :15; /*14:0 , To be used when pll_freq_mode=1._x000D_Fomula is freq*2^24/(mdll_div*crystal_clk)*/
        U32 pll_freq_setting0_reserved_0   :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_pll_freq_setting0;

//pll_freq_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 pll_freq_synthesize_lsb        :16; /*15:0 , To be used when pll_freq_mode=1._x000D_Fomula is freq*2^24/(mdll_div*crystal_clk)*/
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
} T_RF_INTERFACE_pll_freq_setting1;

//bt_lo_freq_setting
typedef union
{
    U32 v;
    struct
    {
        U32 bt_digital_lo_freq             :11; /*10:0 , Digital lo_freq, (1.10), unit 1Mhz. _x000D_Formula is freq(mhz)*1024. Defualt 625kHz.*/
        U32 bt_imgrej                      :1 ; /*11:11, If 0, use low if; if 1 use high if.*/
        U32 bt_lo_freq_setting_reserved_0  :4 ; /*15:12, No description.               */
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
} T_RF_INTERFACE_bt_lo_freq_setting;

//fm_lo_freq_setting
typedef union
{
    U32 v;
    struct
    {
        U32 fm_digital_lo_freq             :11; /*10:0 , Digital lo_freq, (1.10), unit 1Mhz. _x000D_Formula is freq(mhz)*1024. Defualt 125kHz.*/
        U32 fm_imgrej                      :1 ; /*11:11, If 0, use low if; if 1 use high if.*/
        U32 fm_lo_freq_setting_reserved_0  :4 ; /*15:12, No description.               */
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
} T_RF_INTERFACE_fm_lo_freq_setting;

//bt_lna_setting
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_cload_bit               :3 ; /*2 :0 , No description.               */
        U32 bt_lna_reg_bypass              :1 ; /*3 :3 , No description.               */
        U32 bt_lna_reg_bit                 :3 ; /*6 :4 , No description.               */
        U32 bt_balun_cbit_tx               :3 ; /*9 :7 , No description.               */
        U32 bt_balun_cbit_rx               :3 ; /*12:10, No description.               */
        U32 bt_lna_setting_reserved_0      :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_lna_setting;

//bt_rmx_setting
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rmx_reg_bypass              :1 ; /*0 :0 , No description.               */
        U32 bt_rmx_lo_vcom_bit             :2 ; /*2 :1 , Inner LO bias volatge setting.*/
        U32 bt_rmx_reg_bit                 :3 ; /*5 :3 , Rx mixer LO regulator voltage setting.*/
        U32 bt_rmx_ifvcm_sel               :1 ; /*6 :6 , 0: IF vcom not impacted._x000D_1: IF vcom is 1/2 avdd.*/
        U32 bt_rmx_iqswap                  :1 ; /*7 :7 , The I/Q signal after mixer will be swaped.*/
        U32 bt_rmx_lo_ifvcom_bit           :4 ; /*11:8 , No description.               */
        U32 bt_rmx_setting_reserved_0      :4 ; /*15:12, No description.               */
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
} T_RF_INTERFACE_bt_rmx_setting;

//bt_rxflt_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_bp_mode               :3 ; /*2 :0 , 111: rx filter in low pass mode._x000D_011: rx filter in band pass mode101: rx filter in band pass mode110: rx filter in band pass mode._x000D_Others: not defined*/
        U32 bt_rxflt_offset_range_bit      :3 ; /*5 :3 , No description.               */
        U32 bt_rxflt_cap_bit               :4 ; /*9 :6 , Rx filter capacitor setting.  */
        U32 bt_rxflt_cal_mode              :1 ; /*10:10, 0: rx filter offset caibration in low pass mode._x000D_1: rx filter offset calibration in bandpass mode.*/
        U32 bt_rxflt_rstn_dr               :1 ; /*11:11, Filter_rstn_reg direct reg enable.*/
        U32 bt_rxflt_rstn_reg              :1 ; /*12:12, If 0, rx filter opamps will be reset.*/
        U32 bt_rxflt_calclk_edge_sel       :1 ; /*13:13, No description.               */
        U32 bt_rxflt_setting0_reserved_0   :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_rxflt_setting0;

//bt_self_cal_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rx_cal_iqswap               :1 ; /*0 :0 , swap rx sys_cal result IQ     */
        U32 bt_rx_cal_polarity             :1 ; /*1 :1 , If 1, invert rx  filter calibration polarity.*/
        U32 bt_rxflt_offset_bit_dr         :1 ; /*2 :2 , If 1, rxfilter_sys_cal_bit from register value.*/
        U32 bt_dc_offset_init_delay        :2 ; /*4 :3 , dc_cal init delay_x000D_00: 2 intervals of 25KHz clk (80us)_x000D_01: 5 intervals of 25KHz clk (200us)_x000D_10: 7 intervals of 25KHz clk (280us)_x000D_11: 10 intervals of 25KHz clk (400us)*/
        U32 dc_table_full_mode             :1 ; /*5 :5 , 0: dc_cal use dc offset table item 0 and 151: dc_cal use dc offset table item 0~15*/
        U32 bt_rx_cal_cnt                  :4 ; /*9 :6 , Define dc calibration timer in slow mode.*/
        U32 bt_dc_cal_mode                 :2 ; /*11:10, 00: slow mode_x000D_01: medium mode (0x27)_x000D_10/11: fast mode*/
        U32 bt_rx_cal_clk_edge             :1 ; /*12:12, dc_cal_clk_invert_x000D_if 1, invert rx sys cal clock*/
        U32 bt_self_cal_setting0_reserved_0 :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_self_cal_setting0;

//bt_rx_cal_setting
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_offset_q_bit_reg      :8 ; /*7 :0 , while rx_cal_bit_dr=1,use this calibration value.*/
        U32 bt_rxflt_offset_i_bit_reg      :8 ; /*15:8 , while rx_cal_bit_dr=1,use this calibration value.*/
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
} T_RF_INTERFACE_bt_rx_cal_setting;

//bt_rx_cal_status
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_offset_q_bit          :8 ; /*7 :0 , Q channel offset calibration result*/
        U32 bt_rxflt_offset_i_bit          :8 ; /*15:8 , I channel offset calibration result*/
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
} T_RF_INTERFACE_bt_rx_cal_status;

//bt_adc_setting
typedef union
{
    U32 v;
    struct
    {
        U32 bt_adc_clk_edge_sel            :1 ; /*0 :0 , No description.               */
        U32 bt_adc_clk_div2                :1 ; /*1 :1 , No description.               */
        U32 bt_adc_delay_bit               :2 ; /*3 :2 , No description.               */
        U32 bt_adc_vref_bit                :2 ; /*5 :4 , No description.               */
        U32 bt_adc_setting_reserved_0      :10; /*15:6 , No description.               */
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
} T_RF_INTERFACE_bt_adc_setting;

//bt_padrv_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_ibit                  :6 ; /*5 :0 , No description.               */
        U32 bt_tmx_lobias                  :5 ; /*10:6 , No description.               */
        U32 bt_tmx_ibit                    :2 ; /*12:11, No description.               */
        U32 bt_padrv_setting0_reserved_0   :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_padrv_setting0;

//bt_padrv_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 bt_tmx_cap_bit                 :4 ; /*3 :0 , No description.               */
        U32 bt_tmx_plltest_en              :1 ; /*4 :4 , No description.               */
        U32 bt_tmx_bbtest_en               :1 ; /*5 :5 , No description.               */
        U32 bt_delay_padrv                 :8 ; /*13:6 , Padrv delay timer. _x000D_delay_padrv[7-0]*1us */
        U32 bt_padrv_setting1_reserved_0   :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_padrv_setting1;

//bt_bbdac_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_bbdac_vlow_bit              :3 ; /*2 :0 , Define vlow/vhigh voltage of digital signal.*/
        U32 bt_bbdac_dvdd_sel              :1 ; /*3 :3 , No description.               */
        U32 bt_bbdac_clk_edge_sel          :1 ; /*4 :4 , No description.               */
        U32 bt_bbdac_vtr_sel               :1 ; /*5 :5 , 0: bbdac Iref from bandgap._x000D_1: bbdac Iref from Vt/R.*/
        U32 bt_bbdac_out_en                :1 ; /*6 :6 , No description.               */
        U32 bt_bbdac_test                  :1 ; /*7 :7 , No description.               */
        U32 bt_bbdac_setting0_reserved_0   :8 ; /*15:8 , No description.               */
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
} T_RF_INTERFACE_bt_bbdac_setting0;

//fm_lna_mixer_setting
typedef union
{
    U32 v;
    struct
    {
        U32 fm_lna_ibit                    :3 ; /*2 :0 , No description.               */
        U32 fm_lna_cfg_bit                 :4 ; /*6 :3 , No description.               */
        U32 fm_mixer_lo_bias_bit           :2 ; /*8 :7 , No description.               */
        U32 fm_mixer_harm_rej_en           :1 ; /*9 :9 , No description.               */
        U32 fm_lna_mixer_setting_reserved_0 :6 ; /*15:10, No description.               */
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
} T_RF_INTERFACE_fm_lna_mixer_setting;

//fm_filter_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_iqswap               :1 ; /*0 :0 , No description.               */
        U32 fm_filter_bw_bit               :2 ; /*2 :1 , No description.               */
        U32 fm_filter_op_ibit              :2 ; /*4 :3 , No description.               */
        U32 fm_filter_cal_mode             :1 ; /*5 :5 , No description.               */
        U32 fm_filter_cal_range            :3 ; /*8 :6 , No description.               */
        U32 fm_filter_cal_clk_edge_inn     :1 ; /*9 :9 , No description.               */
        U32 fm_filter_cal_clk_edge         :1 ; /*10:10, No description.               */
        U32 fm_dc_cal_init_dealy           :2 ; /*12:11, No description.               */
        U32 fm_filter_cal_polarity         :1 ; /*13:13, No description.               */
        U32 fm_dcoffset_cal_onetime        :1 ; /*14:14, No description.               */
        U32 fm_filter_offset_cal_dr        :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_fm_filter_setting0;

//fm_filter_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_offset_cal_i_reg     :8 ; /*7 :0 , No description.               */
        U32 fm_filter_offset_cal_q_reg     :8 ; /*15:8 , No description.               */
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
} T_RF_INTERFACE_fm_filter_setting1;

//fm_filter_setting2
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_resetn_reg           :1 ; /*0 :0 , No description.               */
        U32 fm_filter_resetn_dr            :1 ; /*1 :1 , No description.               */
        U32 fm_dc_cal_cnt                  :4 ; /*5 :2 , No description.               */
        U32 fm_filter_setting2_reserved_0  :10; /*15:6 , No description.               */
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
} T_RF_INTERFACE_fm_filter_setting2;

//fm_filter_status
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_offset_cal_i         :8 ; /*7 :0 , No description.               */
        U32 fm_filter_offset_cal_q         :8 ; /*15:8 , No description.               */
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
} T_RF_INTERFACE_fm_filter_status;

//fm_adc_setting
typedef union
{
    U32 v;
    struct
    {
        U32 fm_adc_isel_bit                :2 ; /*1 :0 , No description.               */
        U32 fm_adc_iqswap                  :1 ; /*2 :2 , No description.               */
        U32 fm_adc_dac_bias_bit            :2 ; /*4 :3 , No description.               */
        U32 fm_adc_lp_mode_en              :1 ; /*5 :5 , No description.               */
        U32 fm_adc_test_enable             :1 ; /*6 :6 , No description.               */
        U32 fm_adc_cal_polarity            :1 ; /*7 :7 , No description.               */
        U32 fm_adc_cal_refi_bit_dr         :1 ; /*8 :8 , No description.               */
        U32 fm_adc_cal_refi_bit_reg        :3 ; /*11:9 , No description.               */
        U32 fm_adc_resetn_dr               :1 ; /*12:12, No description.               */
        U32 fm_adc_resetn_reg              :1 ; /*13:13, No description.               */
        U32 fm_adc_setting_reserved_0      :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_fm_adc_setting;

//fm_dsp_setting
typedef union
{
    U32 v;
    struct
    {
        U32 fm_dsp_resetn_reg              :1 ; /*0 :0 , No description.               */
        U32 fm_dsp_resetn_dr               :1 ; /*1 :1 , No description.               */
        U32 fm_dsp_resetn_delay            :2 ; /*3 :2 , No description.               */
        U32 fm_dsp_setting_reserved_0      :12; /*15:4 , No description.               */
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
} T_RF_INTERFACE_fm_dsp_setting;

//vco_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 vco_pkd_ref_bit                :3 ; /*2 :0 , No description.               */
        U32 vco_reg_bypass                 :1 ; /*3 :3 , No description.               */
        U32 vco_div2_bias_bit              :3 ; /*6 :4 , No description.               */
        U32 vco_buf_bias_ibit              :4 ; /*10:7 , No description.               */
        U32 vco_buf_ibit                   :4 ; /*14:11, No description.               */
        U32 vco_setting0_reserved_0        :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_vco_setting0;

//vco_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 vco_ibit_reg                   :4 ; /*3 :0 , No description.               */
        U32 vco_ibit_dr                    :1 ; /*4 :4 , if 1, vco_ibit is triggered by register;if 0, vco_ibit is triggered by logic.*/
        U32 vco_setting1_reserved_0        :11; /*15:5 , No description.               */
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
} T_RF_INTERFACE_vco_setting1;

//pll_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 pll_bypass_cp_na               :1 ; /*0 :0 , No description.               */
        U32 pll_bypass_notch               :1 ; /*1 :1 , If 1, bypass notch filter in PLL Loop.*/
        U32 pll_presc_reg_bit              :4 ; /*5 :2 , PLL prescalar regulator voltage setting.*/
        U32 pll_sdm_clk_sel_reg            :1 ; /*6 :6 , Select fbc or fref as  sdm clk*/
        U32 pll_sdm_clk_sel_dr             :1 ; /*7 :7 , pll_sdm_clk_sel_reg direct reg enable.*/
        U32 pll_refmulti2_en               :1 ; /*8 :8 , If 1, pll reference clk frequency will double.*/
        U32 pll_open_en                    :1 ; /*9 :9 , If 1, pll will be open loop.  */
        U32 pll_cal_clk_sel                :1 ; /*10:10, Select the positive or negative edge of cal clk*/
        U32 pll_cp_r_bit_rx                :2 ; /*12:11, No description.               */
        U32 pll_cp_r_bit_tx                :2 ; /*14:13, No description.               */
        U32 pll_setting0_reserved_0        :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_pll_setting0;

//pll_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 vco_band_reg                   :11; /*10:0 , VCO band  setting.            */
        U32 vco_band_dr                    :1 ; /*11:11, Pll_vco_band_reg direct reg enable.*/
        U32 pll_cal_bit                    :2 ; /*13:12, Control voltage when in calibration._x000D_00 : lowest voltage_x000D_01 : medial voltage_x000D_10 : medial voltage_x000D_11 : highest voltage*/
        U32 pll_setting1_reserved_0        :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_pll_setting1;

//pll_setting2
typedef union
{
    U32 v;
    struct
    {
        U32 pll_cp_bit_rx                  :6 ; /*5 :0 , Pll charge pump current setting when in RX mode.*/
        U32 pll_gain_bit_rx                :3 ; /*8 :6 , Pll vco gain control bit  when in RX mode.*/
        U32 pll_fbc_sel_rx                 :3 ; /*11:9 , Select one of the 3 retimed feedback clocks when in RX mode.*/
        U32 mdll_div_rx                    :4 ; /*15:12, Set this reg according  to pll reference colck in RX._x000D_26MHz: 0001_x000D_52MHz: 0010_x000D_208MHz: 1000*/
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
} T_RF_INTERFACE_pll_setting2;

//pll_setting3
typedef union
{
    U32 v;
    struct
    {
        U32 pll_cp_bit_tx                  :6 ; /*5 :0 , Pll charge pump current setting when in TX mode.*/
        U32 pll_gain_bit_tx                :3 ; /*8 :6 , Pll vco gain control bit  when in TX mode.*/
        U32 pll_fbc_sel_tx                 :3 ; /*11:9 , Select one of the 3 retimed feedback clocks when in TX mode.*/
        U32 mdll_div_tx                    :4 ; /*15:12, Set this reg according  to pll reference colck in TX._x000D_26MHz: 0001_x000D_52MHz: 0010_x000D_208MHz: 1000*/
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
} T_RF_INTERFACE_pll_setting3;

//pll_setting4
typedef union
{
    U32 v;
    struct
    {
        U32 pll_pfd_res_bit_rx             :6 ; /*5 :0 , Define vhigh/vlow of up/upb signal to chargepump when in RX mode.*/
        U32 pll_lpf_gain_bit_rx            :5 ; /*10:6 , Pll low pass gain setting when in RX mode.*/
        U32 pll_r_bit_rx                   :5 ; /*15:11, Pll loop filter resistor setting when in RX mode.*/
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
} T_RF_INTERFACE_pll_setting4;

//pll_setting5
typedef union
{
    U32 v;
    struct
    {
        U32 pll_pfd_res_bit_tx             :6 ; /*5 :0 , Define vhigh/vlow of up/upb signal to chargepump when in TX mode.*/
        U32 pll_lpf_gain_bit_tx            :5 ; /*10:6 , Pll low pass gain setting when in TX mode.*/
        U32 pll_r_bit_tx                   :5 ; /*15:11, Pll loop filter resistor setting when in TX mode.*/
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
} T_RF_INTERFACE_pll_setting5;

//pll_status
typedef union
{
    U32 v;
    struct
    {
        U32 pll_lock_det                   :1 ; /*0 :0 , Indicate if pll in lock state.*/
        U32 pll_status_reserved_0          :15; /*15:1 , No description.               */
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
} T_RF_INTERFACE_pll_status;

//vco_cal_setting
typedef union
{
    U32 v;
    struct
    {
        U32 vco_pkd_clk_edge_sel           :1 ; /*0 :0 , vco_pkd_clk edge selection    */
        U32 vco_cal_polarity               :1 ; /*1 :1 , vco_pkd_out polarity          */
        U32 vco_cal_init_delay             :2 ; /*3 :2 , vco calibration initial delay.2'd0:4us;2'd1:8us;2'd2:16us;2'd3:32us.*/
        U32 vco_cal_wait_cnt               :2 ; /*5 :4 , define the time that required for vco_pkd_out to be stable. (vco_cal_wait_cnt+1)*4us*/
        U32 vco_cal_cnt                    :2 ; /*7 :6 , if vco_cal_mode=0, define the calibration period.(vco_cal_cnt+1)*16*(vco_cal_wait_cnt+1)*4us*/
        U32 vco_cal_mode                   :1 ; /*8 :8 , vco calibration mode. 1'd1: fast;'1d0:normal*/
        U32 vco_cal_setting_reserved_0     :7 ; /*15:9 , vco calibration mode. 1'd1: fast;'1d0:normal*/
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
} T_RF_INTERFACE_vco_cal_setting;

//pll_cal_setting
typedef union
{
    U32 v;
    struct
    {
        U32 pll_init_delay                 :3 ; /*2 :0 , Define pll_cal initial delay, which is the  time between RXON(TXON) and rxpll_cal_enable._x000D_3'd0: init_delay_loop = 0us_x000D_3'd1: init_delay_loop = 1us_x000D_3'd2: init_delay_loop = 2us_x000D_3'd3: init_delay_loop = 3us_x000D_3'd4: init_delay_loop = 4us_x000D_3'd5: init_delay_loop = 5us_x000D_3'd6: init_delay_loop = 6us_x000D_3'd7: init_delay_loop = 7us*/
        U32 pll_cal_cnt_sel                :3 ; /*5 :3 , pll cal count time select_x000D_3'd0: each_cnt_time = 0.5us_x000D_3'd1: each_cnt_time = 1us_x000D_3'd2: each_cnt_time = 2us_x000D_3'd4: each_cnt_time = 4us_x000D_Else: each_cnt_time = 8us*/
        U32 pll_cal_opt                    :1 ; /*6 :6 , If 1, select the best vco band bit*/
        U32 pll_vco_bit_hold_time          :3 ; /*9 :7 , Vco bit hold time when vco bit changed during pll vco band calibration._x000D_3'd0: vco_bit_hold_time = 0.25us_x000D_3'd1: vco_bit_hold_time = 0.5us_x000D_3'd2: vco_bit_hold_time = 0.75us_x000D_3'd3: vco_bit_hold_time = 1us_x000D_3'd4: vco_bit_hold_time = 1.25us_x000D_3'd5: vco_bit_hold_time = 1.5us_x000D_3'd6: vco_bit_hold_time = 1.75us_x000D_3'd7: vco_bit_hold_time = 2us*/
        U32 pll_cnt_enable_polarity        :1 ; /*10:10, If 1, change pll_cnt_en edge mode(posedge or negedge of 26mhz clk)*/
        U32 reset_pll_lock_delay           :2 ; /*12:11, Pll lock detect time .0:5us; 1:10us;2:15us;3:20us.*/
        U32 pll_cal_setting_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_pll_cal_setting;

//sdm_setting
typedef union
{
    U32 v;
    struct
    {
        U32 sdm_delay_sel                  :4 ; /*3 :0 , SDM out will be delayed. Unit is sdm clock period.*/
        U32 sdm_dither_bypass              :1 ; /*4 :4 , SDM dither bypass enable.     */
        U32 sdm_int_dec_sel                :2 ; /*6 :5 , SDM prescaler divider scale setting_x000D_00: int divide_x000D_01: 1bit decimal divide_x000D_10: 2bits decimal divide_x000D_11: bypass the sdm (direct output int)*/
        U32 sdm_clk_edge                   :1 ; /*7 :7 , Invert SDM clock edge.        */
        U32 pll_sdm_resetn_reg             :1 ; /*8 :8 , Sdm modulator module reset register*/
        U32 pll_sdm_resetn_dr              :1 ; /*9 :9 , If 1, sdm resetn uses sdm_resetn_reg; _x000D_if 0, use logic value.*/
        U32 reset_pll_sdm_delay            :2 ; /*11:10, Delay time select after txon|rxon|self_cal_en after pll_cal_ready &amp; prescaler_div_ready_x000D_00:1us 01:2us 10:8us 11:16us*/
        U32 sdm_setting_reserved_0         :4 ; /*15:12, No description.               */
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
} T_RF_INTERFACE_sdm_setting;

//cal_dr_setting
typedef union
{
    U32 v;
    struct
    {
        U32 self_cal_ready_reg             :1 ; /*0 :0 , Self_cal_ready register.      */
        U32 self_cal_ready_dr              :1 ; /*1 :1 , If 1, self_cal_ready comes from register;_x000D_If 0, self_cal_ready comes from logic.*/
        U32 pll_cal_resetn_reg             :1 ; /*2 :2 , No description.               */
        U32 pll_cal_resetn_dr              :1 ; /*3 :3 , If 1, rxpll calibration triggered by register;_x000D_If 0, rxpll calibration triggered by logic.*/
        U32 bt_dc_cal_fsm_resetn_reg       :1 ; /*4 :4 , dc calibration control register.*/
        U32 bt_dc_cal_fsm_resetn_dr        :1 ; /*5 :5 , No description.               */
        U32 fm_dc_cal_fsm_resetn_reg       :1 ; /*6 :6 , dc calibration control register.*/
        U32 fm_dc_cal_fsm_resetn_dr        :1 ; /*7 :7 , No description.               */
        U32 fm_adc_cal_fsm_resetn_reg      :1 ; /*8 :8 , adc calibration control register.*/
        U32 fm_adc_cal_fsm_resetn_dr       :1 ; /*9 :9 , No description.               */
        U32 vco_cal_resetn_reg             :1 ; /*10:10, vco calibration control register.*/
        U32 vco_cal_resetn_dr              :1 ; /*11:11, if 1, vco calibraiton triggered by register.If 0, vco calibration triggered by logic.*/
        U32 reserved_1                     :1 ; /*12:12,                               */
        U32 reserved_2                     :1 ; /*13:13,                               */
        U32 cal_dr_setting_reserved_0      :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_cal_dr_setting;

//ldo_setting
typedef union
{
    U32 v;
    struct
    {
        U32 ivref_avdd_sel                 :1 ; /*0 :0 , 1: select bandgap as input reference_x000D_0: select  local vref as reference*/
        U32 pd_ivref_reg                   :1 ; /*1 :1 , pu_ivref control register.    */
        U32 pd_ivref_dr                    :1 ; /*2 :2 , Ldo_iref_pu direct reg enable.*/
        U32 ldo_setting_reserved_0         :13; /*15:3 , No description.               */
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
} T_RF_INTERFACE_ldo_setting;

//power_status0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_pd_bbdac                    :1 ; /*0 :0 , No description.               */
        U32 bt_pd_padrv                    :1 ; /*1 :1 , No description.               */
        U32 bt_pd_adc                      :1 ; /*2 :2 , No description.               */
        U32 bt_pd_rxflt                    :1 ; /*3 :3 , No description.               */
        U32 bt_pd_rmx                      :1 ; /*4 :4 , No description.               */
        U32 bt_pd_lna                      :1 ; /*5 :5 , No description.               */
        U32 pd_lo_rx                       :1 ; /*6 :6 , No description.               */
        U32 pd_lo_tx                       :1 ; /*7 :7 , No description.               */
        U32 pd_vco_pkd                     :1 ; /*8 :8 , No description.               */
        U32 pd_vco                         :1 ; /*9 :9 , No description.               */
        U32 pd_pll_fmclk                   :1 ; /*10:10, No description.               */
        U32 pd_pll_presc                   :1 ; /*11:11, No description.               */
        U32 pd_pll_digi                    :1 ; /*12:12, No description.               */
        U32 bt_pd_tmx                      :1 ; /*13:13, No description.               */
        U32 power_status0_reserved_0       :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_power_status0;

//power_status1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_pd_adc                      :1 ; /*0 :0 , No description.               */
        U32 fm_pd_filter                   :1 ; /*1 :1 , No description.               */
        U32 fm_pd_mixer                    :1 ; /*2 :2 , No description.               */
        U32 fm_pd_lna                      :1 ; /*3 :3 , No description.               */
        U32 power_status1_reserved_0       :12; /*15:4 , No description.               */
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
} T_RF_INTERFACE_power_status1;

//power_reg0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_pd_bbdac_reg                :1 ; /*0 :0 , No description.               */
        U32 bt_pd_padrv_reg                :1 ; /*1 :1 , No description.               */
        U32 bt_pd_adc_reg                  :1 ; /*2 :2 , No description.               */
        U32 bt_pd_rxflt_reg                :1 ; /*3 :3 , No description.               */
        U32 bt_pd_rmx_reg                  :1 ; /*4 :4 , No description.               */
        U32 bt_pd_lna_reg                  :1 ; /*5 :5 , No description.               */
        U32 pd_lo_rx_reg                   :1 ; /*6 :6 , No description.               */
        U32 pd_lo_tx_reg                   :1 ; /*7 :7 , No description.               */
        U32 pd_vco_pkd_reg                 :1 ; /*8 :8 , No description.               */
        U32 pd_vco_reg                     :1 ; /*9 :9 , No description.               */
        U32 pd_pll_fmclk_reg               :1 ; /*10:10, No description.               */
        U32 pd_pll_presc_reg               :1 ; /*11:11, No description.               */
        U32 pd_pll_digi_reg                :1 ; /*12:12, No description.               */
        U32 bt_pd_tmx_reg                  :1 ; /*13:13, No description.               */
        U32 power_reg0_reserved_0          :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_power_reg0;

//power_reg1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_pd_adc_reg                  :1 ; /*0 :0 , No description.               */
        U32 fm_pd_filter_reg               :1 ; /*1 :1 , No description.               */
        U32 fm_pd_mixer_reg                :1 ; /*2 :2 , No description.               */
        U32 fm_pd_lna_reg                  :1 ; /*3 :3 , No description.               */
        U32 power_reg1_reserved_0          :12; /*15:4 , No description.               */
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
} T_RF_INTERFACE_power_reg1;

//bt_gain_table_0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_0                :3 ; /*2 :0 , Filter_gain when pointer =0   */
        U32 bt_rmx_gain_0                  :4 ; /*6 :3 , Rmx_gain when pointer =0      */
        U32 bt_lna_gain2_0                 :2 ; /*8 :7 , Lna_gain2 when pointer =0     */
        U32 bt_lna_gain1_0                 :4 ; /*12:9 , Lna_gain1 when pointer =0     */
        U32 bt_gain_table_0_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_0;

//bt_gain_table_1
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_1                :3 ; /*2 :0 , Filter_gain when pointer =1   */
        U32 bt_rmx_gain_1                  :4 ; /*6 :3 , Rmx_gain when pointer =1      */
        U32 bt_lna_gain2_1                 :2 ; /*8 :7 , Lna_gain2 when pointer =1     */
        U32 bt_lna_gain1_1                 :4 ; /*12:9 , Lna_gain1 when pointer =1     */
        U32 bt_gain_table_1_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_1;

//bt_gain_table_2
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_2                :3 ; /*2 :0 , Filter_gain when pointer =2   */
        U32 bt_rmx_gain_2                  :4 ; /*6 :3 , Rmx_gain when pointer =2      */
        U32 bt_lna_gain2_2                 :2 ; /*8 :7 , Lna_gain2 when pointer =2     */
        U32 bt_lna_gain1_2                 :4 ; /*12:9 , Lna_gain1 when pointer =2     */
        U32 bt_gain_table_2_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_2;

//bt_gain_table_3
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_3                :3 ; /*2 :0 , Filter_gain when pointer =3   */
        U32 bt_rmx_gain_3                  :4 ; /*6 :3 , Rmx_gain when pointer =3      */
        U32 bt_lna_gain2_3                 :2 ; /*8 :7 , Lna_gain2 when pointer =3     */
        U32 bt_lna_gain1_3                 :4 ; /*12:9 , Lna_gain1 when pointer =3     */
        U32 bt_gain_table_3_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_3;

//bt_gain_table_4
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_4                :3 ; /*2 :0 , Filter_gain when pointer =4   */
        U32 bt_rmx_gain_4                  :4 ; /*6 :3 , Rmx_gain when pointer =4      */
        U32 bt_lna_gain2_4                 :2 ; /*8 :7 , Lna_gain2 when pointer =4     */
        U32 bt_lna_gain1_4                 :4 ; /*12:9 , Lna_gain1 when pointer =4     */
        U32 bt_gain_table_4_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_4;

//bt_gain_table_5
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_5                :3 ; /*2 :0 , Filter_gain when pointer =5   */
        U32 bt_rmx_gain_5                  :4 ; /*6 :3 , Rmx_gain when pointer =5      */
        U32 bt_lna_gain2_5                 :2 ; /*8 :7 , Lna_gain2 when pointer =5     */
        U32 bt_lna_gain1_5                 :4 ; /*12:9 , Lna_gain1 when pointer =5     */
        U32 bt_gain_table_5_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_5;

//bt_gain_table_6
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_6                :3 ; /*2 :0 , Filter_gain when pointer =6   */
        U32 bt_rmx_gain_6                  :4 ; /*6 :3 , Rmx_gain when pointer =6      */
        U32 bt_lna_gain2_6                 :2 ; /*8 :7 , Lna_gain2 when pointer =6     */
        U32 bt_lna_gain1_6                 :4 ; /*12:9 , Lna_gain1 when pointer =6     */
        U32 bt_gain_table_6_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_6;

//bt_gain_table_7
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_7                :3 ; /*2 :0 , Filter_gain when pointer =7   */
        U32 bt_rmx_gain_7                  :4 ; /*6 :3 , Rmx_gain when pointer =7      */
        U32 bt_lna_gain2_7                 :2 ; /*8 :7 , Lna_gain2 when pointer =7     */
        U32 bt_lna_gain1_7                 :4 ; /*12:9 , Lna_gain1 when pointer =7     */
        U32 bt_gain_table_7_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_7;

//bt_gain_table_8
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_8                :3 ; /*2 :0 , Filter_gain when pointer =8   */
        U32 bt_rmx_gain_8                  :4 ; /*6 :3 , Rmx_gain when pointer =8      */
        U32 bt_lna_gain2_8                 :2 ; /*8 :7 , Lna_gain2 when pointer =8     */
        U32 bt_lna_gain1_8                 :4 ; /*12:9 , Lna_gain1 when pointer =8     */
        U32 bt_gain_table_8_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_8;

//bt_gain_table_9
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_9                :3 ; /*2 :0 , Filter_gain when pointer =9   */
        U32 bt_rmx_gain_9                  :4 ; /*6 :3 , Rmx_gain when pointer =9      */
        U32 bt_lna_gain2_9                 :2 ; /*8 :7 , Lna_gain2 when pointer =9     */
        U32 bt_lna_gain1_9                 :4 ; /*12:9 , Lna_gain1 when pointer =9     */
        U32 bt_gain_table_9_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_9;

//bt_gain_table_a
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_a                :3 ; /*2 :0 , Filter_gain when pointer =a   */
        U32 bt_rmx_gain_a                  :4 ; /*6 :3 , Rmx_gain when pointer =a      */
        U32 bt_lna_gain2_a                 :2 ; /*8 :7 , Lna_gain2 when pointer =a     */
        U32 bt_lna_gain1_a                 :4 ; /*12:9 , Lna_gain1 when pointer =a     */
        U32 bt_gain_table_a_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_a;

//bt_gain_table_b
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_b                :3 ; /*2 :0 , Filter_gain when pointer =3   */
        U32 bt_rmx_gain_b                  :4 ; /*6 :3 , Rmx_gain when pointer =b      */
        U32 bt_lna_gain2_b                 :2 ; /*8 :7 , Lna_gain2 when pointer =b     */
        U32 bt_lna_gain1_b                 :4 ; /*12:9 , Lna_gain1 when pointer =b     */
        U32 bt_gain_table_b_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_b;

//bt_gain_table_c
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_c                :3 ; /*2 :0 , Filter_gain when pointer =4   */
        U32 bt_rmx_gain_c                  :4 ; /*6 :3 , Rmx_gain when pointer =c      */
        U32 bt_lna_gain2_c                 :2 ; /*8 :7 , Lna_gain2 when pointer =c     */
        U32 bt_lna_gain1_c                 :4 ; /*12:9 , Lna_gain1 when pointer =c     */
        U32 bt_gain_table_c_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_c;

//bt_gain_table_d
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_d                :3 ; /*2 :0 , Filter_gain when pointer =5   */
        U32 bt_rmx_gain_d                  :4 ; /*6 :3 , Rmx_gain when pointer =d      */
        U32 bt_lna_gain2_d                 :2 ; /*8 :7 , Lna_gain2 when pointer =d     */
        U32 bt_lna_gain1_d                 :4 ; /*12:9 , Lna_gain1 when pointer =d     */
        U32 bt_gain_table_d_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_d;

//bt_gain_table_e
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_e                :3 ; /*2 :0 , Filter_gain when pointer =6   */
        U32 bt_rmx_gain_e                  :4 ; /*6 :3 , Rmx_gain when pointer =e      */
        U32 bt_lna_gain2_e                 :2 ; /*8 :7 , Lna_gain2 when pointer =e     */
        U32 bt_lna_gain1_e                 :4 ; /*12:9 , Lna_gain1 when pointer =e     */
        U32 bt_gain_table_e_reserved_0     :3 ; /*15:13, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_e;

//bt_gain_table_f
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_gain_f                :3 ; /*2 :0 , Filter_gain when pointer =7   */
        U32 bt_rmx_gain_f                  :4 ; /*6 :3 , Rmx_gain when pointer =f      */
        U32 bt_lna_gain2_f                 :2 ; /*8 :7 , Lna_gain2 when pointer =f     */
        U32 bt_lna_gain1_f                 :4 ; /*12:9 , Lna_gain1 when pointer =f     */
        U32 bt_agc_gain_dr                 :1 ; /*13:13, if 1, use table F gain value  */
        U32 bt_gain_table_f_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_gain_table_f;

//bt_ibit_table_0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_1                 :4 ; /*3 :0 , lna_ibit2 when pointer=1      */
        U32 bt_lna_ibit1_1                 :3 ; /*6 :4 , lna_ibit1 when pointer=1      */
        U32 bt_lna_ibit2_0                 :4 ; /*10:7 , lna_ibit2 when pointer=0      */
        U32 bt_lna_ibit1_0                 :3 ; /*13:11, lna_ibit1 when pointer=0      */
        U32 bt_ibit_table_0_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_0;

//bt_ibit_table_1
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_3                 :4 ; /*3 :0 , lna_ibit2 when pointer=3      */
        U32 bt_lna_ibit1_3                 :3 ; /*6 :4 , lna_ibit1 when pointer=3      */
        U32 bt_lna_ibit2_2                 :4 ; /*10:7 , lna_ibit2 when pointer=2      */
        U32 bt_lna_ibit1_2                 :3 ; /*13:11, lna_ibit1 when pointer=2      */
        U32 bt_ibit_table_1_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_1;

//bt_ibit_table_2
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_5                 :4 ; /*3 :0 , lna_ibit2 when pointer=5      */
        U32 bt_lna_ibit1_5                 :3 ; /*6 :4 , lna_ibit1 when pointer=5      */
        U32 bt_lna_ibit2_4                 :4 ; /*10:7 , lna_ibit2 when pointer=4      */
        U32 bt_lna_ibit1_4                 :3 ; /*13:11, lna_ibit1 when pointer=4      */
        U32 bt_ibit_table_2_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_2;

//bt_ibit_table_3
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_7                 :4 ; /*3 :0 , lna_ibit2 when pointer=7      */
        U32 bt_lna_ibit1_7                 :3 ; /*6 :4 , lna_ibit1 when pointer=7      */
        U32 bt_lna_ibit2_6                 :4 ; /*10:7 , lna_ibit2 when pointer=6      */
        U32 bt_lna_ibit1_6                 :3 ; /*13:11, lna_ibit1 when pointer=6      */
        U32 bt_ibit_table_3_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_3;

//bt_ibit_table_4
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_9                 :4 ; /*3 :0 , lna_ibit2 when pointer=9      */
        U32 bt_lna_ibit1_9                 :3 ; /*6 :4 , lna_ibit1 when pointer=9      */
        U32 bt_lna_ibit2_8                 :4 ; /*10:7 , lna_ibit2 when pointer=8      */
        U32 bt_lna_ibit1_8                 :3 ; /*13:11, lna_ibit1 when pointer=8      */
        U32 bt_ibit_table_4_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_4;

//bt_ibit_table_5
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_b                 :4 ; /*3 :0 , lna_ibit2 when pointer=b      */
        U32 bt_lna_ibit1_b                 :3 ; /*6 :4 , lna_ibit1 when pointer=b      */
        U32 bt_lna_ibit2_a                 :4 ; /*10:7 , lna_ibit2 when pointer=a      */
        U32 bt_lna_ibit1_a                 :3 ; /*13:11, lna_ibit1 when pointer=a      */
        U32 bt_ibit_table_5_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_5;

//bt_ibit_table_6
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_d                 :4 ; /*3 :0 , lna_ibit2 when pointer=d      */
        U32 bt_lna_ibit1_d                 :3 ; /*6 :4 , lna_ibit1 when pointer=d      */
        U32 bt_lna_ibit2_c                 :4 ; /*10:7 , lna_ibit2 when pointer=c      */
        U32 bt_lna_ibit1_c                 :3 ; /*13:11, lna_ibit1 when pointer=c      */
        U32 bt_ibit_table_6_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_6;

//bt_ibit_table_7
typedef union
{
    U32 v;
    struct
    {
        U32 bt_lna_ibit2_f                 :4 ; /*3 :0 , lna_ibit2 when pointer=f      */
        U32 bt_lna_ibit1_f                 :3 ; /*6 :4 , lna_ibit1 when pointer=f      */
        U32 bt_lna_ibit2_e                 :4 ; /*10:7 , lna_ibit2 when pointer=e      */
        U32 bt_lna_ibit1_e                 :3 ; /*13:11, lna_ibit1 when pointer=e      */
        U32 bt_ibit_table_7_reserved_0     :2 ; /*15:14, No description.               */
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
} T_RF_INTERFACE_bt_ibit_table_7;

//bt_ibit_table_8
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_op_ibit_7             :2 ; /*1 :0 , rx filter op_ibit when pointer=7*/
        U32 bt_rxflt_op_ibit_6             :2 ; /*3 :2 , rx filter op_ibit when pointer=6*/
        U32 bt_rxflt_op_ibit_5             :2 ; /*5 :4 , rx filter op_ibit when pointer=5*/
        U32 bt_rxflt_op_ibit_4             :2 ; /*7 :6 , rx filter op_ibit when pointer=4*/
        U32 bt_rxflt_op_ibit_3             :2 ; /*9 :8 , rx filter op_ibit when pointer=3*/
        U32 bt_rxflt_op_ibit_2             :2 ; /*11:10, rx filter op_ibit when pointer=2*/
        U32 bt_rxflt_op_ibit_1             :2 ; /*13:12, rx filter op_ibit when pointer=1*/
        U32 bt_rxflt_op_ibit_0             :2 ; /*15:14, rx filter op_ibit when pointer=0*/
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
} T_RF_INTERFACE_bt_ibit_table_8;

//bt_ibit_table_9
typedef union
{
    U32 v;
    struct
    {
        U32 bt_rxflt_op_ibit_f             :2 ; /*1 :0 , rx filter op_ibit when pointer=f*/
        U32 bt_rxflt_op_ibit_e             :2 ; /*3 :2 , rx filter op_ibit when pointer=e*/
        U32 bt_rxflt_op_ibit_d             :2 ; /*5 :4 , rx filter op_ibit when pointer=d*/
        U32 bt_rxflt_op_ibit_c             :2 ; /*7 :6 , rx filter op_ibit when pointer=c*/
        U32 bt_rxflt_op_ibit_b             :2 ; /*9 :8 , rx filter op_ibit when pointer=b*/
        U32 bt_rxflt_op_ibit_a             :2 ; /*11:10, rx filter op_ibit when pointer=a*/
        U32 bt_rxflt_op_ibit_9             :2 ; /*13:12, rx filter op_ibit when pointer=9*/
        U32 bt_rxflt_op_ibit_8             :2 ; /*15:14, rx filter op_ibit when pointer=8*/
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
} T_RF_INTERFACE_bt_ibit_table_9;

//tx_padrv_gain_setting_0
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_1                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=1*/
        U32 bt_padrv_gain_0                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=0*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_0;

//tx_padrv_gain_setting_1
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_3                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=3*/
        U32 bt_padrv_gain_2                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=2*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_1;

//tx_padrv_gain_setting_2
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_5                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=5*/
        U32 bt_padrv_gain_4                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=4*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_2;

//tx_padrv_gain_setting_3
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_7                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=7*/
        U32 bt_padrv_gain_6                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=6*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_3;

//tx_padrv_gain_setting_4
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_9                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=9*/
        U32 bt_padrv_gain_8                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=8*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_4;

//tx_padrv_gain_setting_5
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_b                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=b*/
        U32 bt_padrv_gain_a                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=a*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_5;

//tx_padrv_gain_setting_6
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_d                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=d*/
        U32 bt_padrv_gain_c                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=c*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_6;

//tx_padrv_gain_setting_7
typedef union
{
    U32 v;
    struct
    {
        U32 bt_padrv_gain_f                :8 ; /*7 :0 , Padrv gain setting in psk mode when APC index=f*/
        U32 bt_padrv_gain_e                :8 ; /*15:8 , Padrv gain setting in fsk mode when APC index=e*/
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
} T_RF_INTERFACE_tx_padrv_gain_setting_7;

//fm_lna_gain_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 fm_lna_gain_bit_0              :2 ; /*1 :0 , fm lna gain bit when agc_index=0*/
        U32 fm_lna_gain_bit_1              :2 ; /*3 :2 , fm lna gain bit when agc_index=1*/
        U32 fm_lna_gain_bit_2              :2 ; /*5 :4 , fm lna gain bit when agc_index=2*/
        U32 fm_lna_gain_bit_3              :2 ; /*7 :6 , fm lna gain bit when agc_index=3*/
        U32 fm_lna_gain_bit_4              :2 ; /*9 :8 , fm lna gain bit when agc_index=4*/
        U32 fm_lna_gain_bit_5              :2 ; /*11:10, fm lna gain bit when agc_index=5*/
        U32 fm_lna_gain_bit_6              :2 ; /*13:12, fm lna gain bit when agc_index=6*/
        U32 fm_lna_gain_bit_7              :2 ; /*15:14, fm lna gain bit when agc_index=7*/
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
} T_RF_INTERFACE_fm_lna_gain_setting0;

//fm_lna_gain_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_lna_gain_bit_8              :2 ; /*1 :0 , fm lna gain bit when agc_index=8*/
        U32 fm_lna_gain_bit_9              :2 ; /*3 :2 , fm lna gain bit when agc_index=9*/
        U32 fm_lna_gain_bit_a              :2 ; /*5 :4 , fm lna gain bit when agc_index=a*/
        U32 fm_lna_gain_bit_b              :2 ; /*7 :6 , fm lna gain bit when agc_index=b*/
        U32 fm_lna_gain_bit_c              :2 ; /*9 :8 , fm lna gain bit when agc_index=c*/
        U32 fm_lna_gain_bit_d              :2 ; /*11:10, fm lna gain bit when agc_index=d*/
        U32 fm_lna_gain_bit_e              :2 ; /*13:12, fm lna gain bit when agc_index=e*/
        U32 fm_lna_gain_bit_f              :2 ; /*15:14, fm lna gain bit when agc_index=f*/
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
} T_RF_INTERFACE_fm_lna_gain_setting1;

//fm_mixer_gain_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 fm_mixer_gain_bit_0            :3 ; /*2 :0 , fm mixer gain bit when agc_index=0*/
        U32 fm_mixer_gain_bit_1            :3 ; /*5 :3 , fm mixer gain bit when agc_index=1*/
        U32 fm_mixer_gain_bit_2            :3 ; /*8 :6 , fm mixer gain bit when agc_index=2*/
        U32 fm_mixer_gain_bit_3            :3 ; /*11:9 , fm mixer gain bit when agc_index=3*/
        U32 fm_mixer_gain_bit_4            :3 ; /*14:12, fm mixer gain bit when agc_index=4*/
        U32 fm_mixer_gain_setting0_reserved_0 :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_fm_mixer_gain_setting0;

//fm_mixer_gain_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_mixer_gain_bit_5            :3 ; /*2 :0 , fm mixer gain bit when agc_index=5*/
        U32 fm_mixer_gain_bit_6            :3 ; /*5 :3 , fm mixer gain bit when agc_index=6*/
        U32 fm_mixer_gain_bit_7            :3 ; /*8 :6 , fm mixer gain bit when agc_index=7*/
        U32 fm_mixer_gain_bit_8            :3 ; /*11:9 , fm mixer gain bit when agc_index=8*/
        U32 fm_mixer_gain_bit_9            :3 ; /*14:12, fm mixer gain bit when agc_index=9*/
        U32 fm_mixer_gain_setting1_reserved_0 :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_fm_mixer_gain_setting1;

//fm_mixer_gain_setting2
typedef union
{
    U32 v;
    struct
    {
        U32 fm_mixer_gain_bit_a            :3 ; /*2 :0 , fm mixer gain bit when agc_index=a*/
        U32 fm_mixer_gain_bit_b            :3 ; /*5 :3 , fm mixer gain bit when agc_index=b*/
        U32 fm_mixer_gain_bit_c            :3 ; /*8 :6 , fm mixer gain bit when agc_index=c*/
        U32 fm_mixer_gain_bit_d            :3 ; /*11:9 , fm mixer gain bit when agc_index=d*/
        U32 fm_mixer_gain_bit_e            :3 ; /*14:12, fm mixer gain bit when agc_index=e*/
        U32 fm_mixer_gain_setting2_reserved_0 :1 ; /*15:15, No description.               */
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
} T_RF_INTERFACE_fm_mixer_gain_setting2;

//fm_mixer_gain_setting3
typedef union
{
    U32 v;
    struct
    {
        U32 fm_mixer_gain_bit_f            :3 ; /*2 :0 , fm mixer gain bit when agc_index=f*/
        U32 fm_mixer_gain_setting3_reserved_0 :13; /*15:3 , No description.               */
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
} T_RF_INTERFACE_fm_mixer_gain_setting3;

//fm_filter_gain_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_gain_bit_0           :2 ; /*1 :0 , fm filter gain bit when agc_index=0*/
        U32 fm_filter_gain_bit_1           :2 ; /*3 :2 , fm filter gain bit when agc_index=1*/
        U32 fm_filter_gain_bit_2           :2 ; /*5 :4 , fm filter gain bit when agc_index=2*/
        U32 fm_filter_gain_bit_3           :2 ; /*7 :6 , fm filter gain bit when agc_index=3*/
        U32 fm_filter_gain_bit_4           :2 ; /*9 :8 , fm filter gain bit when agc_index=4*/
        U32 fm_filter_gain_bit_5           :2 ; /*11:10, fm filter gain bit when agc_index=5*/
        U32 fm_filter_gain_bit_6           :2 ; /*13:12, fm filter gain bit when agc_index=6*/
        U32 fm_filter_gain_bit_7           :2 ; /*15:14, fm filter gain bit when agc_index=7*/
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
} T_RF_INTERFACE_fm_filter_gain_setting0;

//fm_filter_gain_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_filter_gain_bit_8           :2 ; /*1 :0 , fm filter gain bit when agc_index=8*/
        U32 fm_filter_gain_bit_9           :2 ; /*3 :2 , fm filter gain bit when agc_index=9*/
        U32 fm_filter_gain_bit_a           :2 ; /*5 :4 , fm filter gain bit when agc_index=a*/
        U32 fm_filter_gain_bit_b           :2 ; /*7 :6 , fm filter gain bit when agc_index=b*/
        U32 fm_filter_gain_bit_c           :2 ; /*9 :8 , fm filter gain bit when agc_index=c*/
        U32 fm_filter_gain_bit_d           :2 ; /*11:10, fm filter gain bit when agc_index=d*/
        U32 fm_filter_gain_bit_e           :2 ; /*13:12, fm filter gain bit when agc_index=e*/
        U32 fm_filter_gain_bit_f           :2 ; /*15:14, fm filter gain bit when agc_index=f*/
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
} T_RF_INTERFACE_fm_filter_gain_setting1;

//fm_adc_gain_setting0
typedef union
{
    U32 v;
    struct
    {
        U32 fm_adc_gain_bit_0              :2 ; /*1 :0 , fm adc gain bit when agc_index=0*/
        U32 fm_adc_gain_bit_1              :2 ; /*3 :2 , fm adc gain bit when agc_index=1*/
        U32 fm_adc_gain_bit_2              :2 ; /*5 :4 , fm adc gain bit when agc_index=2*/
        U32 fm_adc_gain_bit_3              :2 ; /*7 :6 , fm adc gain bit when agc_index=3*/
        U32 fm_adc_gain_bit_4              :2 ; /*9 :8 , fm adc gain bit when agc_index=4*/
        U32 fm_adc_gain_bit_5              :2 ; /*11:10, fm adc gain bit when agc_index=5*/
        U32 fm_adc_gain_bit_6              :2 ; /*13:12, fm adc gain bit when agc_index=6*/
        U32 fm_adc_gain_bit_7              :2 ; /*15:14, fm adc gain bit when agc_index=7*/
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
} T_RF_INTERFACE_fm_adc_gain_setting0;

//fm_adc_gain_setting1
typedef union
{
    U32 v;
    struct
    {
        U32 fm_adc_gain_bit_8              :2 ; /*1 :0 , fm adc gain bit when agc_index=8*/
        U32 fm_adc_gain_bit_9              :2 ; /*3 :2 , fm adc gain bit when agc_index=9*/
        U32 fm_adc_gain_bit_a              :2 ; /*5 :4 , fm adc gain bit when agc_index=a*/
        U32 fm_adc_gain_bit_b              :2 ; /*7 :6 , fm adc gain bit when agc_index=b*/
        U32 fm_adc_gain_bit_c              :2 ; /*9 :8 , fm adc gain bit when agc_index=c*/
        U32 fm_adc_gain_bit_d              :2 ; /*11:10, fm adc gain bit when agc_index=d*/
        U32 fm_adc_gain_bit_e              :2 ; /*13:12, fm adc gain bit when agc_index=e*/
        U32 fm_adc_gain_bit_f              :2 ; /*15:14, fm adc gain bit when agc_index=f*/
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
} T_RF_INTERFACE_fm_adc_gain_setting1;

typedef struct
{
    volatile         T_RF_INTERFACE_revision                       revision; /*0x0  , RO   , 0x00005430, No description.               */
    volatile      T_RF_INTERFACE_sys_control                    sys_control; /*0x1  , RW   , 0x00000051, No description.               */
    volatile       T_RF_INTERFACE_bt_control                     bt_control; /*0x2  , RW   , 0x00000780, No description.               */
    volatile        T_RF_INTERFACE_bt_status                      bt_status; /*0x3  , RO   , 0x00000000, No description.               */
    volatile      T_RF_INTERFACE_fm_control0                    fm_control0; /*0x4  , RW   , 0x00000080, No description.               */
    volatile      T_RF_INTERFACE_fm_control1                    fm_control1; /*0x5  , RW   , 0x00000000, No description.               */
    volatile      T_RF_INTERFACE_fm_control2                    fm_control2; /*0x6  , RW   , 0x00000000, No description.               */
    volatile        T_RF_INTERFACE_fm_status                      fm_status; /*0x7  , RO   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_pll_freq_setting0              pll_freq_setting0; /*0x8  , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_pll_freq_setting1              pll_freq_setting1; /*0x9  , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_bt_lo_freq_setting             bt_lo_freq_setting; /*0xa  , RW   , 0x00000280, No description.               */
    volatile T_RF_INTERFACE_fm_lo_freq_setting             fm_lo_freq_setting; /*0xb  , RW   , 0x00000080, No description.               */
    volatile   T_RF_INTERFACE_bt_lna_setting                 bt_lna_setting; /*0xc  , RW   , 0x00000000, No description.               */
    volatile   T_RF_INTERFACE_bt_rmx_setting                 bt_rmx_setting; /*0xd  , RW   , 0x00000034, No description.               */
    volatile T_RF_INTERFACE_bt_rxflt_setting0              bt_rxflt_setting0; /*0xe  , RW   , 0x00000406, No description.               */
    volatile T_RF_INTERFACE_bt_self_cal_setting0           bt_self_cal_setting0; /*0xf  , RW   , 0x00000220, No description.               */
    volatile T_RF_INTERFACE_bt_rx_cal_setting              bt_rx_cal_setting; /*0x10 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_bt_rx_cal_status               bt_rx_cal_status; /*0x11 , RO   , 0x00000000, No description.               */
    volatile   T_RF_INTERFACE_bt_adc_setting                 bt_adc_setting; /*0x12 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_bt_padrv_setting0              bt_padrv_setting0; /*0x13 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_bt_padrv_setting1              bt_padrv_setting1; /*0x14 , RW   , 0x00000780, No description.               */
    volatile T_RF_INTERFACE_bt_bbdac_setting0              bt_bbdac_setting0; /*0x15 , RW   , 0x00000004, No description.               */
    volatile T_RF_INTERFACE_fm_lna_mixer_setting           fm_lna_mixer_setting; /*0x16 , RW   , 0x00000004, No description.               */
    volatile T_RF_INTERFACE_fm_filter_setting0             fm_filter_setting0; /*0x17 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_filter_setting1             fm_filter_setting1; /*0x18 , RW   , 0x00008080, No description.               */
    volatile T_RF_INTERFACE_fm_filter_setting2             fm_filter_setting2; /*0x19 , RW   , 0x00000020, No description.               */
    volatile T_RF_INTERFACE_fm_filter_status               fm_filter_status; /*0x1a , RO   , 0x00008080, No description.               */
    volatile   T_RF_INTERFACE_fm_adc_setting                 fm_adc_setting; /*0x1b , RW   , 0x00000800, No description.               */
    volatile   T_RF_INTERFACE_fm_dsp_setting                 fm_dsp_setting; /*0x1c , RW   , 0x00000000, No description.               */
    volatile     T_RF_INTERFACE_vco_setting0                   vco_setting0; /*0x1d , RW   , 0x00000000, No description.               */
    volatile     T_RF_INTERFACE_vco_setting1                   vco_setting1; /*0x1e , RW   , 0x00000000, No description.               */
    volatile     T_RF_INTERFACE_pll_setting0                   pll_setting0; /*0x1f , RW   , 0x00000118, No description.               */
    volatile     T_RF_INTERFACE_pll_setting1                   pll_setting1; /*0x20 , RW   , 0x00001400, No description.               */
    volatile     T_RF_INTERFACE_pll_setting2                   pll_setting2; /*0x21 , RW   , 0x00002500, No description.               */
    volatile     T_RF_INTERFACE_pll_setting3                   pll_setting3; /*0x22 , RW   , 0x00002500, No description.               */
    volatile     T_RF_INTERFACE_pll_setting4                   pll_setting4; /*0x23 , RW   , 0x0000000F, No description.               */
    volatile     T_RF_INTERFACE_pll_setting5                   pll_setting5; /*0x24 , RW   , 0x00000024, No description.               */
    volatile       T_RF_INTERFACE_pll_status                     pll_status; /*0x25 , RO   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_vco_cal_setting                vco_cal_setting; /*0x26 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_pll_cal_setting                pll_cal_setting; /*0x27 , RW   , 0x00000048, No description.               */
    volatile      T_RF_INTERFACE_sdm_setting                    sdm_setting; /*0x28 , RW   , 0x00000020, No description.               */
    volatile   T_RF_INTERFACE_cal_dr_setting                 cal_dr_setting; /*0x29 , RW   , 0x00000000, No description.               */
    volatile      T_RF_INTERFACE_ldo_setting                    ldo_setting; /*0x2a , RW   , 0x00000001, No description.               */
    volatile    T_RF_INTERFACE_power_status0                  power_status0; /*0x2b , RO   , 0x00003FFF, No description.               */
    volatile    T_RF_INTERFACE_power_status1                  power_status1; /*0x2c , RO   , 0x0000000F, No description.               */
    volatile       T_RF_INTERFACE_power_reg0                     power_reg0; /*0x2d , RW   , 0x00003FFF, No description.               */
    volatile       T_RF_INTERFACE_power_reg1                     power_reg1; /*0x2e , RW   , 0x0000000F, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_0                bt_gain_table_0; /*0x2f , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_1                bt_gain_table_1; /*0x30 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_2                bt_gain_table_2; /*0x31 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_3                bt_gain_table_3; /*0x32 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_4                bt_gain_table_4; /*0x33 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_5                bt_gain_table_5; /*0x34 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_6                bt_gain_table_6; /*0x35 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_7                bt_gain_table_7; /*0x36 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_8                bt_gain_table_8; /*0x37 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_9                bt_gain_table_9; /*0x38 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_a                bt_gain_table_a; /*0x39 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_b                bt_gain_table_b; /*0x3a , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_c                bt_gain_table_c; /*0x3b , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_d                bt_gain_table_d; /*0x3c , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_e                bt_gain_table_e; /*0x3d , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_gain_table_f                bt_gain_table_f; /*0x3e , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_0                bt_ibit_table_0; /*0x3f , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_1                bt_ibit_table_1; /*0x40 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_2                bt_ibit_table_2; /*0x41 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_3                bt_ibit_table_3; /*0x42 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_4                bt_ibit_table_4; /*0x43 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_5                bt_ibit_table_5; /*0x44 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_6                bt_ibit_table_6; /*0x45 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_7                bt_ibit_table_7; /*0x46 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_8                bt_ibit_table_8; /*0x47 , RW   , 0x00000000, No description.               */
    volatile  T_RF_INTERFACE_bt_ibit_table_9                bt_ibit_table_9; /*0x48 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_0        tx_padrv_gain_setting_0; /*0x49 , RW   , 0x00006767, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_1        tx_padrv_gain_setting_1; /*0x4a , RW   , 0x00006767, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_2        tx_padrv_gain_setting_2; /*0x4b , RW   , 0x00007B7B, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_3        tx_padrv_gain_setting_3; /*0x4c , RW   , 0x00008585, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_4        tx_padrv_gain_setting_4; /*0x4d , RW   , 0x00008B8B, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_5        tx_padrv_gain_setting_5; /*0x4e , RW   , 0x00009B9B, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_6        tx_padrv_gain_setting_6; /*0x4f , RW   , 0x0000A7A7, No description.               */
    volatile T_RF_INTERFACE_tx_padrv_gain_setting_7        tx_padrv_gain_setting_7; /*0x50 , RW   , 0x0000A7A7, No description.               */
    volatile T_RF_INTERFACE_fm_lna_gain_setting0           fm_lna_gain_setting0; /*0x51 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_lna_gain_setting1           fm_lna_gain_setting1; /*0x52 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_mixer_gain_setting0         fm_mixer_gain_setting0; /*0x53 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_mixer_gain_setting1         fm_mixer_gain_setting1; /*0x54 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_mixer_gain_setting2         fm_mixer_gain_setting2; /*0x55 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_mixer_gain_setting3         fm_mixer_gain_setting3; /*0x56 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_filter_gain_setting0        fm_filter_gain_setting0; /*0x57 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_filter_gain_setting1        fm_filter_gain_setting1; /*0x58 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_adc_gain_setting0           fm_adc_gain_setting0; /*0x59 , RW   , 0x00000000, No description.               */
    volatile T_RF_INTERFACE_fm_adc_gain_setting1           fm_adc_gain_setting1; /*0x5a , RW   , 0x00000000, No description.               */
} T_HWP_RF_INTERFACE_T;
#else
typedef struct
{
    volatile                             U32                       revision; /*0x0  , RO   , 0x00005430, No description.               */
    volatile                             U32                    sys_control; /*0x1  , RW   , 0x00000051, No description.               */
    volatile                             U32                     bt_control; /*0x2  , RW   , 0x00000780, No description.               */
    volatile                             U32                      bt_status; /*0x3  , RO   , 0x00000000, No description.               */
    volatile                             U32                    fm_control0; /*0x4  , RW   , 0x00000080, No description.               */
    volatile                             U32                    fm_control1; /*0x5  , RW   , 0x00000000, No description.               */
    volatile                             U32                    fm_control2; /*0x6  , RW   , 0x00000000, No description.               */
    volatile                             U32                      fm_status; /*0x7  , RO   , 0x00000000, No description.               */
    volatile                             U32              pll_freq_setting0; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32              pll_freq_setting1; /*0x9  , RW   , 0x00000000, No description.               */
    volatile                             U32             bt_lo_freq_setting; /*0xa  , RW   , 0x00000280, No description.               */
    volatile                             U32             fm_lo_freq_setting; /*0xb  , RW   , 0x00000080, No description.               */
    volatile                             U32                 bt_lna_setting; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                 bt_rmx_setting; /*0xd  , RW   , 0x00000034, No description.               */
    volatile                             U32              bt_rxflt_setting0; /*0xe  , RW   , 0x00000406, No description.               */
    volatile                             U32           bt_self_cal_setting0; /*0xf  , RW   , 0x00000220, No description.               */
    volatile                             U32              bt_rx_cal_setting; /*0x10 , RW   , 0x00000000, No description.               */
    volatile                             U32               bt_rx_cal_status; /*0x11 , RO   , 0x00000000, No description.               */
    volatile                             U32                 bt_adc_setting; /*0x12 , RW   , 0x00000000, No description.               */
    volatile                             U32              bt_padrv_setting0; /*0x13 , RW   , 0x00000000, No description.               */
    volatile                             U32              bt_padrv_setting1; /*0x14 , RW   , 0x00000780, No description.               */
    volatile                             U32              bt_bbdac_setting0; /*0x15 , RW   , 0x00000004, No description.               */
    volatile                             U32           fm_lna_mixer_setting; /*0x16 , RW   , 0x00000004, No description.               */
    volatile                             U32             fm_filter_setting0; /*0x17 , RW   , 0x00000000, No description.               */
    volatile                             U32             fm_filter_setting1; /*0x18 , RW   , 0x00008080, No description.               */
    volatile                             U32             fm_filter_setting2; /*0x19 , RW   , 0x00000020, No description.               */
    volatile                             U32               fm_filter_status; /*0x1a , RO   , 0x00008080, No description.               */
    volatile                             U32                 fm_adc_setting; /*0x1b , RW   , 0x00000800, No description.               */
    volatile                             U32                 fm_dsp_setting; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                             U32                   vco_setting0; /*0x1d , RW   , 0x00000000, No description.               */
    volatile                             U32                   vco_setting1; /*0x1e , RW   , 0x00000000, No description.               */
    volatile                             U32                   pll_setting0; /*0x1f , RW   , 0x00000118, No description.               */
    volatile                             U32                   pll_setting1; /*0x20 , RW   , 0x00001400, No description.               */
    volatile                             U32                   pll_setting2; /*0x21 , RW   , 0x00002500, No description.               */
    volatile                             U32                   pll_setting3; /*0x22 , RW   , 0x00002500, No description.               */
    volatile                             U32                   pll_setting4; /*0x23 , RW   , 0x0000000F, No description.               */
    volatile                             U32                   pll_setting5; /*0x24 , RW   , 0x00000024, No description.               */
    volatile                             U32                     pll_status; /*0x25 , RO   , 0x00000000, No description.               */
    volatile                             U32                vco_cal_setting; /*0x26 , RW   , 0x00000000, No description.               */
    volatile                             U32                pll_cal_setting; /*0x27 , RW   , 0x00000048, No description.               */
    volatile                             U32                    sdm_setting; /*0x28 , RW   , 0x00000020, No description.               */
    volatile                             U32                 cal_dr_setting; /*0x29 , RW   , 0x00000000, No description.               */
    volatile                             U32                    ldo_setting; /*0x2a , RW   , 0x00000001, No description.               */
    volatile                             U32                  power_status0; /*0x2b , RO   , 0x00003FFF, No description.               */
    volatile                             U32                  power_status1; /*0x2c , RO   , 0x0000000F, No description.               */
    volatile                             U32                     power_reg0; /*0x2d , RW   , 0x00003FFF, No description.               */
    volatile                             U32                     power_reg1; /*0x2e , RW   , 0x0000000F, No description.               */
    volatile                             U32                bt_gain_table_0; /*0x2f , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_1; /*0x30 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_2; /*0x31 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_3; /*0x32 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_4; /*0x33 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_5; /*0x34 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_6; /*0x35 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_7; /*0x36 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_8; /*0x37 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_9; /*0x38 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_a; /*0x39 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_b; /*0x3a , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_c; /*0x3b , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_d; /*0x3c , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_e; /*0x3d , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_f; /*0x3e , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_0; /*0x3f , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_1; /*0x40 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_2; /*0x41 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_3; /*0x42 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_4; /*0x43 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_5; /*0x44 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_6; /*0x45 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_7; /*0x46 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_8; /*0x47 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_ibit_table_9; /*0x48 , RW   , 0x00000000, No description.               */
    volatile                             U32        tx_padrv_gain_setting_0; /*0x49 , RW   , 0x00006767, No description.               */
    volatile                             U32        tx_padrv_gain_setting_1; /*0x4a , RW   , 0x00006767, No description.               */
    volatile                             U32        tx_padrv_gain_setting_2; /*0x4b , RW   , 0x00007B7B, No description.               */
    volatile                             U32        tx_padrv_gain_setting_3; /*0x4c , RW   , 0x00008585, No description.               */
    volatile                             U32        tx_padrv_gain_setting_4; /*0x4d , RW   , 0x00008B8B, No description.               */
    volatile                             U32        tx_padrv_gain_setting_5; /*0x4e , RW   , 0x00009B9B, No description.               */
    volatile                             U32        tx_padrv_gain_setting_6; /*0x4f , RW   , 0x0000A7A7, No description.               */
    volatile                             U32        tx_padrv_gain_setting_7; /*0x50 , RW   , 0x0000A7A7, No description.               */
    volatile                             U32           fm_lna_gain_setting0; /*0x51 , RW   , 0x00000000, No description.               */
    volatile                             U32           fm_lna_gain_setting1; /*0x52 , RW   , 0x00000000, No description.               */
    volatile                             U32         fm_mixer_gain_setting0; /*0x53 , RW   , 0x00000000, No description.               */
    volatile                             U32         fm_mixer_gain_setting1; /*0x54 , RW   , 0x00000000, No description.               */
    volatile                             U32         fm_mixer_gain_setting2; /*0x55 , RW   , 0x00000000, No description.               */
    volatile                             U32         fm_mixer_gain_setting3; /*0x56 , RW   , 0x00000000, No description.               */
    volatile                             U32        fm_filter_gain_setting0; /*0x57 , RW   , 0x00000000, No description.               */
    volatile                             U32        fm_filter_gain_setting1; /*0x58 , RW   , 0x00000000, No description.               */
    volatile                             U32           fm_adc_gain_setting0; /*0x59 , RW   , 0x00000000, No description.               */
    volatile                             U32           fm_adc_gain_setting1; /*0x5a , RW   , 0x00000000, No description.               */
} T_HWP_RF_INTERFACE_T;

//revision
#define RF_INTERFACE_REVID(n)          (((n)&0xF)<<0)      
#define RF_INTERFACE_REVID_MASK        (0xF<<0)            
#define RF_INTERFACE_REVID_SHIFT       (0)                 

#define RF_INTERFACE_CHIPID(n)         (((n)&0xFFF)<<4)    
#define RF_INTERFACE_CHIPID_MASK       (0xFFF<<4)          
#define RF_INTERFACE_CHIPID_SHIFT      (4)                 


//sys_control
#define RF_INTERFACE_SOFT_RESETN       (1<<0)              

#define RF_INTERFACE_CHIP_SELF_CAL_ENABLE (1<<1)              

#define RF_INTERFACE_PLL_FREQ_MODE     (1<<2)              

#define RF_INTERFACE_ZF_NZF            (1<<3)              
#define RF_INTERFACE_ZF_NZF_ZERO       (0<<3)              
#define RF_INTERFACE_ZF_NZF_NEARLY     (1<<3)              

#define RF_INTERFACE_PD_ALL_B_REG      (1<<4)              

#define RF_INTERFACE_DIRECT_REG        (1<<5)              
#define RF_INTERFACE_DIRECT_REG_PU     (0<<5)              
#define RF_INTERFACE_DIRECT_REG_USE    (1<<5)              

#define RF_INTERFACE_TUNE_DIFF_EN      (1<<6)              

#define RF_INTERFACE_CLK_DIGITAL_ENABLE_REG (1<<7)              

#define RF_INTERFACE_RF_MODE           (1<<8)              


//bt_control
#define RF_INTERFACE_ARFCN(n)          (((n)&0x7F)<<0)     
#define RF_INTERFACE_ARFCN_MASK        (0x7F<<0)           
#define RF_INTERFACE_ARFCN_SHIFT       (0)                 

#define RF_INTERFACE_APC(n)            (((n)&0xF)<<7)      
#define RF_INTERFACE_APC_MASK          (0xF<<7)            
#define RF_INTERFACE_APC_SHIFT         (7)                 


//bt_status
#define RF_INTERFACE_BT_SELF_CAL_READY (1<<0)              


//fm_control0
#define RF_INTERFACE_TUNE              (1<<0)              

#define RF_INTERFACE_SEEK              (1<<1)              

#define RF_INTERFACE_SKMODE            (1<<2)              
#define RF_INTERFACE_SKMODE_WRAP       (0<<2)              
#define RF_INTERFACE_SKMODE_STOP       (1<<2)              

#define RF_INTERFACE_SEEKUP            (1<<3)              
#define RF_INTERFACE_SEEKUP_SEEK       (0<<3)              
#define RF_INTERFACE_SEEKUP_SEEK       (1<<3)              

#define RF_INTERFACE_BAND_SEL(n)       (((n)&0x3)<<4)      
#define RF_INTERFACE_BAND_SEL_87_108MHZ (((0)&0x3)<<4)      
#define RF_INTERFACE_BAND_SEL_76_91MHZ (((1)&0x3)<<4)      
#define RF_INTERFACE_BAND_SEL_76_108MHZ (((2)&0x3)<<4)      
#define RF_INTERFACE_BAND_SEL_65_76MHZ (((3)&0x3)<<4)      
#define RF_INTERFACE_BAND_SEL_MASK     (0x3<<4)            
#define RF_INTERFACE_BAND_SEL_SHIFT    (4)                 

#define RF_INTERFACE_SPACE_SEL(n)      (((n)&0x3)<<6)      
#define RF_INTERFACE_SPACE_SEL_25KHZ   (((0)&0x3)<<6)      
#define RF_INTERFACE_SPACE_SEL_50KHZ   (((1)&0x3)<<6)      
#define RF_INTERFACE_SPACE_SEL_100KHZ  (((2)&0x3)<<6)      
#define RF_INTERFACE_SPACE_SEL_200KHZ  (((3)&0x3)<<6)      
#define RF_INTERFACE_SPACE_SEL_MASK    (0x3<<6)            
#define RF_INTERFACE_SPACE_SEL_SHIFT   (6)                 

#define RF_INTERFACE_FM_FREQ_MODE      (1<<8)              


//fm_control1
#define RF_INTERFACE_CHAN_REG(n)       (((n)&0x3FF)<<0)    
#define RF_INTERFACE_CHAN_REG_MASK     (0x3FF<<0)          
#define RF_INTERFACE_CHAN_REG_SHIFT    (0)                 


//fm_control2
#define RF_INTERFACE_FM_FREQ_DIRECT(n) (((n)&0xFFFF)<<0)   
#define RF_INTERFACE_FM_FREQ_DIRECT_MASK (0xFFFF<<0)         
#define RF_INTERFACE_FM_FREQ_DIRECT_SHIFT (0)                 


//fm_status
#define RF_INTERFACE_FM_SELF_CAL_READY (1<<0)              

#define RF_INTERFACE_STC               (1<<1)              

#define RF_INTERFACE_SF                (1<<2)              

#define RF_INTERFACE_FM_TRUE           (1<<3)              

#define RF_INTERFACE_FM_READY          (1<<4)              

#define RF_INTERFACE_READCHAN(n)       (((n)&0x3FF)<<5)    
#define RF_INTERFACE_READCHAN_MASK     (0x3FF<<5)          
#define RF_INTERFACE_READCHAN_SHIFT    (5)                 


//pll_freq_setting0
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_MSB(n) (((n)&0x7FFF)<<0)   
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_MSB_MASK (0x7FFF<<0)         
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_MSB_SHIFT (0)                 


//pll_freq_setting1
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_LSB(n) (((n)&0xFFFF)<<0)   
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_LSB_MASK (0xFFFF<<0)         
#define RF_INTERFACE_PLL_FREQ_SYNTHESIZE_LSB_SHIFT (0)                 


//bt_lo_freq_setting
#define RF_INTERFACE_BT_DIGITAL_LO_FREQ(n) (((n)&0x7FF)<<0)    
#define RF_INTERFACE_BT_DIGITAL_LO_FREQ_MASK (0x7FF<<0)          
#define RF_INTERFACE_BT_DIGITAL_LO_FREQ_SHIFT (0)                 

#define RF_INTERFACE_BT_IMGREJ         (1<<11)             


//fm_lo_freq_setting
#define RF_INTERFACE_FM_DIGITAL_LO_FREQ(n) (((n)&0x7FF)<<0)    
#define RF_INTERFACE_FM_DIGITAL_LO_FREQ_MASK (0x7FF<<0)          
#define RF_INTERFACE_FM_DIGITAL_LO_FREQ_SHIFT (0)                 

#define RF_INTERFACE_FM_IMGREJ         (1<<11)             


//bt_lna_setting
#define RF_INTERFACE_BT_LNA_CLOAD_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_LNA_CLOAD_BIT_MASK (0x7<<0)            
#define RF_INTERFACE_BT_LNA_CLOAD_BIT_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_REG_BYPASS (1<<3)              

#define RF_INTERFACE_BT_LNA_REG_BIT(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_REG_BIT_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_REG_BIT_SHIFT (4)                 

#define RF_INTERFACE_BT_BALUN_CBIT_TX(n) (((n)&0x7)<<7)      
#define RF_INTERFACE_BT_BALUN_CBIT_TX_MASK (0x7<<7)            
#define RF_INTERFACE_BT_BALUN_CBIT_TX_SHIFT (7)                 

#define RF_INTERFACE_BT_BALUN_CBIT_RX(n) (((n)&0x7)<<10)     
#define RF_INTERFACE_BT_BALUN_CBIT_RX_MASK (0x7<<10)           
#define RF_INTERFACE_BT_BALUN_CBIT_RX_SHIFT (10)                


//bt_rmx_setting
#define RF_INTERFACE_BT_RMX_REG_BYPASS (1<<0)              

#define RF_INTERFACE_BT_RMX_LO_VCOM_BIT(n) (((n)&0x3)<<1)      
#define RF_INTERFACE_BT_RMX_LO_VCOM_BIT_MASK (0x3<<1)            
#define RF_INTERFACE_BT_RMX_LO_VCOM_BIT_SHIFT (1)                 

#define RF_INTERFACE_BT_RMX_REG_BIT(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_BT_RMX_REG_BIT_MASK (0x7<<3)            
#define RF_INTERFACE_BT_RMX_REG_BIT_SHIFT (3)                 

#define RF_INTERFACE_BT_RMX_IFVCM_SEL  (1<<6)              
#define RF_INTERFACE_BT_RMX_IFVCM_SEL_NO_IF (0<<6)              
#define RF_INTERFACE_BT_RMX_IFVCM_SEL_IF (1<<6)              

#define RF_INTERFACE_BT_RMX_IQSWAP     (1<<7)              

#define RF_INTERFACE_BT_RMX_LO_IFVCOM_BIT(n) (((n)&0xF)<<8)      
#define RF_INTERFACE_BT_RMX_LO_IFVCOM_BIT_MASK (0xF<<8)            
#define RF_INTERFACE_BT_RMX_LO_IFVCOM_BIT_SHIFT (8)                 


//bt_rxflt_setting0
#define RF_INTERFACE_BT_RXFLT_BP_MODE(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_BP_MODE_LP (((111)&0x7)<<0)    
#define RF_INTERFACE_BT_RXFLT_BP_MODE_BP1 (((011)&0x7)<<0)    
#define RF_INTERFACE_BT_RXFLT_BP_MODE_BP2 (((101)&0x7)<<0)    
#define RF_INTERFACE_BT_RXFLT_BP_MODE_BP3 (((110)&0x7)<<0)    
#define RF_INTERFACE_BT_RXFLT_BP_MODE_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_BP_MODE_SHIFT (0)                 

#define RF_INTERFACE_BT_RXFLT_OFFSET_RANGE_BIT(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_BT_RXFLT_OFFSET_RANGE_BIT_MASK (0x7<<3)            
#define RF_INTERFACE_BT_RXFLT_OFFSET_RANGE_BIT_SHIFT (3)                 

#define RF_INTERFACE_BT_RXFLT_CAP_BIT(n) (((n)&0xF)<<6)      
#define RF_INTERFACE_BT_RXFLT_CAP_BIT_MASK (0xF<<6)            
#define RF_INTERFACE_BT_RXFLT_CAP_BIT_SHIFT (6)                 

#define RF_INTERFACE_BT_RXFLT_CAL_MODE (1<<10)             
#define RF_INTERFACE_BT_RXFLT_CAL_MODE_LP (0<<10)             
#define RF_INTERFACE_BT_RXFLT_CAL_MODE_BP (1<<10)             

#define RF_INTERFACE_BT_RXFLT_RSTN_DR  (1<<11)             

#define RF_INTERFACE_BT_RXFLT_RSTN_REG (1<<12)             

#define RF_INTERFACE_BT_RXFLT_CALCLK_EDGE_SEL (1<<13)             


//bt_self_cal_setting0
#define RF_INTERFACE_BT_RX_CAL_IQSWAP  (1<<0)              

#define RF_INTERFACE_BT_RX_CAL_POLARITY (1<<1)              

#define RF_INTERFACE_BT_RXFLT_OFFSET_BIT_DR (1<<2)              

#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_2 (((00)&0x3)<<3)     
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_5 (((01)&0x3)<<3)     
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_7 (((10)&0x3)<<3)     
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_10 (((11)&0x3)<<3)     
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_MASK (0x3<<3)            
#define RF_INTERFACE_BT_DC_OFFSET_INIT_DELAY_SHIFT (3)                 

#define RF_INTERFACE_DC_TABLE_FULL_MODE (1<<5)              
#define RF_INTERFACE_DC_TABLE_FULL_MODE_0_AND_15 (1<<5)              
#define RF_INTERFACE_DC_TABLE_FULL_MODE_0_TO_15 (1<<5)              

#define RF_INTERFACE_BT_RX_CAL_CNT(n)  (((n)&0xF)<<6)      
#define RF_INTERFACE_BT_RX_CAL_CNT_MASK (0xF<<6)            
#define RF_INTERFACE_BT_RX_CAL_CNT_SHIFT (6)                 

#define RF_INTERFACE_BT_DC_CAL_MODE(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_DC_CAL_MODE_SLOW (((00)&0x3)<<10)    
#define RF_INTERFACE_BT_DC_CAL_MODE_MEDIUM (((01)&0x3)<<10)    
#define RF_INTERFACE_BT_DC_CAL_MODE_MASK (0x3<<10)           
#define RF_INTERFACE_BT_DC_CAL_MODE_SHIFT (10)                

#define RF_INTERFACE_BT_RX_CAL_CLK_EDGE (1<<12)             


//bt_rx_cal_setting
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_REG(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_REG_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_REG_SHIFT (0)                 

#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_REG(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_REG_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_REG_SHIFT (8)                 


//bt_rx_cal_status
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_RXFLT_OFFSET_Q_BIT_SHIFT (0)                 

#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_RXFLT_OFFSET_I_BIT_SHIFT (8)                 


//bt_adc_setting
#define RF_INTERFACE_BT_ADC_CLK_EDGE_SEL (1<<0)              

#define RF_INTERFACE_BT_ADC_CLK_DIV2   (1<<1)              

#define RF_INTERFACE_BT_ADC_DELAY_BIT(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_ADC_DELAY_BIT_MASK (0x3<<2)            
#define RF_INTERFACE_BT_ADC_DELAY_BIT_SHIFT (2)                 

#define RF_INTERFACE_BT_ADC_VREF_BIT(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_BT_ADC_VREF_BIT_MASK (0x3<<4)            
#define RF_INTERFACE_BT_ADC_VREF_BIT_SHIFT (4)                 


//bt_padrv_setting0
#define RF_INTERFACE_BT_PADRV_IBIT(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_BT_PADRV_IBIT_MASK (0x3F<<0)           
#define RF_INTERFACE_BT_PADRV_IBIT_SHIFT (0)                 

#define RF_INTERFACE_BT_TMX_LOBIAS(n)  (((n)&0x1F)<<6)     
#define RF_INTERFACE_BT_TMX_LOBIAS_MASK (0x1F<<6)           
#define RF_INTERFACE_BT_TMX_LOBIAS_SHIFT (6)                 

#define RF_INTERFACE_BT_TMX_IBIT(n)    (((n)&0x3)<<11)     
#define RF_INTERFACE_BT_TMX_IBIT_MASK  (0x3<<11)           
#define RF_INTERFACE_BT_TMX_IBIT_SHIFT (11)                


//bt_padrv_setting1
#define RF_INTERFACE_BT_TMX_CAP_BIT(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_TMX_CAP_BIT_MASK (0xF<<0)            
#define RF_INTERFACE_BT_TMX_CAP_BIT_SHIFT (0)                 

#define RF_INTERFACE_BT_TMX_PLLTEST_EN (1<<4)              

#define RF_INTERFACE_BT_TMX_BBTEST_EN  (1<<5)              

#define RF_INTERFACE_BT_DELAY_PADRV(n) (((n)&0xFF)<<6)     
#define RF_INTERFACE_BT_DELAY_PADRV_MASK (0xFF<<6)           
#define RF_INTERFACE_BT_DELAY_PADRV_SHIFT (6)                 


//bt_bbdac_setting0
#define RF_INTERFACE_BT_BBDAC_VLOW_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_BBDAC_VLOW_BIT_MASK (0x7<<0)            
#define RF_INTERFACE_BT_BBDAC_VLOW_BIT_SHIFT (0)                 

#define RF_INTERFACE_BT_BBDAC_DVDD_SEL (1<<3)              

#define RF_INTERFACE_BT_BBDAC_CLK_EDGE_SEL (1<<4)              

#define RF_INTERFACE_BT_BBDAC_VTR_SEL  (1<<5)              
#define RF_INTERFACE_BT_BBDAC_VTR_SEL_BANDGAP (0<<5)              
#define RF_INTERFACE_BT_BBDAC_VTR_SEL_VTR (1<<5)              

#define RF_INTERFACE_BT_BBDAC_OUT_EN   (1<<6)              

#define RF_INTERFACE_BT_BBDAC_TEST     (1<<7)              


//fm_lna_mixer_setting
#define RF_INTERFACE_FM_LNA_IBIT(n)    (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_LNA_IBIT_MASK  (0x7<<0)            
#define RF_INTERFACE_FM_LNA_IBIT_SHIFT (0)                 

#define RF_INTERFACE_FM_LNA_CFG_BIT(n) (((n)&0xF)<<3)      
#define RF_INTERFACE_FM_LNA_CFG_BIT_MASK (0xF<<3)            
#define RF_INTERFACE_FM_LNA_CFG_BIT_SHIFT (3)                 

#define RF_INTERFACE_FM_MIXER_LO_BIAS_BIT(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_FM_MIXER_LO_BIAS_BIT_MASK (0x3<<7)            
#define RF_INTERFACE_FM_MIXER_LO_BIAS_BIT_SHIFT (7)                 

#define RF_INTERFACE_FM_MIXER_HARM_REJ_EN (1<<9)              


//fm_filter_setting0
#define RF_INTERFACE_FM_FILTER_IQSWAP  (1<<0)              

#define RF_INTERFACE_FM_FILTER_BW_BIT(n) (((n)&0x3)<<1)      
#define RF_INTERFACE_FM_FILTER_BW_BIT_MASK (0x3<<1)            
#define RF_INTERFACE_FM_FILTER_BW_BIT_SHIFT (1)                 

#define RF_INTERFACE_FM_FILTER_OP_IBIT(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_FM_FILTER_OP_IBIT_MASK (0x3<<3)            
#define RF_INTERFACE_FM_FILTER_OP_IBIT_SHIFT (3)                 

#define RF_INTERFACE_FM_FILTER_CAL_MODE (1<<5)              

#define RF_INTERFACE_FM_FILTER_CAL_RANGE(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_FILTER_CAL_RANGE_MASK (0x7<<6)            
#define RF_INTERFACE_FM_FILTER_CAL_RANGE_SHIFT (6)                 

#define RF_INTERFACE_FM_FILTER_CAL_CLK_EDGE_INN (1<<9)              

#define RF_INTERFACE_FM_FILTER_CAL_CLK_EDGE (1<<10)             

#define RF_INTERFACE_FM_DC_CAL_INIT_DEALY(n) (((n)&0x3)<<11)     
#define RF_INTERFACE_FM_DC_CAL_INIT_DEALY_MASK (0x3<<11)           
#define RF_INTERFACE_FM_DC_CAL_INIT_DEALY_SHIFT (11)                

#define RF_INTERFACE_FM_FILTER_CAL_POLARITY (1<<13)             

#define RF_INTERFACE_FM_DCOFFSET_CAL_ONETIME (1<<14)             

#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_DR (1<<15)             


//fm_filter_setting1
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_REG(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_REG_MASK (0xFF<<0)           
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_REG_SHIFT (0)                 

#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_REG(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_REG_MASK (0xFF<<8)           
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_REG_SHIFT (8)                 


//fm_filter_setting2
#define RF_INTERFACE_FM_FILTER_RESETN_REG (1<<0)              

#define RF_INTERFACE_FM_FILTER_RESETN_DR (1<<1)              

#define RF_INTERFACE_FM_DC_CAL_CNT(n)  (((n)&0xF)<<2)      
#define RF_INTERFACE_FM_DC_CAL_CNT_MASK (0xF<<2)            
#define RF_INTERFACE_FM_DC_CAL_CNT_SHIFT (2)                 


//fm_filter_status
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_MASK (0xFF<<0)           
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_I_SHIFT (0)                 

#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_MASK (0xFF<<8)           
#define RF_INTERFACE_FM_FILTER_OFFSET_CAL_Q_SHIFT (8)                 


//fm_adc_setting
#define RF_INTERFACE_FM_ADC_ISEL_BIT(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_ISEL_BIT_MASK (0x3<<0)            
#define RF_INTERFACE_FM_ADC_ISEL_BIT_SHIFT (0)                 

#define RF_INTERFACE_FM_ADC_IQSWAP     (1<<2)              

#define RF_INTERFACE_FM_ADC_DAC_BIAS_BIT(n) (((n)&0x3)<<3)      
#define RF_INTERFACE_FM_ADC_DAC_BIAS_BIT_MASK (0x3<<3)            
#define RF_INTERFACE_FM_ADC_DAC_BIAS_BIT_SHIFT (3)                 

#define RF_INTERFACE_FM_ADC_LP_MODE_EN (1<<5)              

#define RF_INTERFACE_FM_ADC_TEST_ENABLE (1<<6)              

#define RF_INTERFACE_FM_ADC_CAL_POLARITY (1<<7)              

#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_DR (1<<8)              

#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_REG(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_REG_MASK (0x7<<9)            
#define RF_INTERFACE_FM_ADC_CAL_REFI_BIT_REG_SHIFT (9)                 

#define RF_INTERFACE_FM_ADC_RESETN_DR  (1<<12)             

#define RF_INTERFACE_FM_ADC_RESETN_REG (1<<13)             


//fm_dsp_setting
#define RF_INTERFACE_FM_DSP_RESETN_REG (1<<0)              

#define RF_INTERFACE_FM_DSP_RESETN_DR  (1<<1)              

#define RF_INTERFACE_FM_DSP_RESETN_DELAY(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_DSP_RESETN_DELAY_MASK (0x3<<2)            
#define RF_INTERFACE_FM_DSP_RESETN_DELAY_SHIFT (2)                 


//vco_setting0
#define RF_INTERFACE_VCO_PKD_REF_BIT(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_VCO_PKD_REF_BIT_MASK (0x7<<0)            
#define RF_INTERFACE_VCO_PKD_REF_BIT_SHIFT (0)                 

#define RF_INTERFACE_VCO_REG_BYPASS    (1<<3)              

#define RF_INTERFACE_VCO_DIV2_BIAS_BIT(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_VCO_DIV2_BIAS_BIT_MASK (0x7<<4)            
#define RF_INTERFACE_VCO_DIV2_BIAS_BIT_SHIFT (4)                 

#define RF_INTERFACE_VCO_BUF_BIAS_IBIT(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_VCO_BUF_BIAS_IBIT_MASK (0xF<<7)            
#define RF_INTERFACE_VCO_BUF_BIAS_IBIT_SHIFT (7)                 

#define RF_INTERFACE_VCO_BUF_IBIT(n)   (((n)&0xF)<<11)     
#define RF_INTERFACE_VCO_BUF_IBIT_MASK (0xF<<11)           
#define RF_INTERFACE_VCO_BUF_IBIT_SHIFT (11)                


//vco_setting1
#define RF_INTERFACE_VCO_IBIT_REG(n)   (((n)&0xF)<<0)      
#define RF_INTERFACE_VCO_IBIT_REG_MASK (0xF<<0)            
#define RF_INTERFACE_VCO_IBIT_REG_SHIFT (0)                 

#define RF_INTERFACE_VCO_IBIT_DR       (1<<4)              


//pll_setting0
#define RF_INTERFACE_PLL_BYPASS_CP_NA  (1<<0)              

#define RF_INTERFACE_PLL_BYPASS_NOTCH  (1<<1)              

#define RF_INTERFACE_PLL_PRESC_REG_BIT(n) (((n)&0xF)<<2)      
#define RF_INTERFACE_PLL_PRESC_REG_BIT_MASK (0xF<<2)            
#define RF_INTERFACE_PLL_PRESC_REG_BIT_SHIFT (2)                 

#define RF_INTERFACE_PLL_SDM_CLK_SEL_REG (1<<6)              

#define RF_INTERFACE_PLL_SDM_CLK_SEL_DR (1<<7)              

#define RF_INTERFACE_PLL_REFMULTI2_EN  (1<<8)              

#define RF_INTERFACE_PLL_OPEN_EN       (1<<9)              

#define RF_INTERFACE_PLL_CAL_CLK_SEL   (1<<10)             

#define RF_INTERFACE_PLL_CP_R_BIT_RX(n) (((n)&0x3)<<11)     
#define RF_INTERFACE_PLL_CP_R_BIT_RX_MASK (0x3<<11)           
#define RF_INTERFACE_PLL_CP_R_BIT_RX_SHIFT (11)                

#define RF_INTERFACE_PLL_CP_R_BIT_TX(n) (((n)&0x3)<<13)     
#define RF_INTERFACE_PLL_CP_R_BIT_TX_MASK (0x3<<13)           
#define RF_INTERFACE_PLL_CP_R_BIT_TX_SHIFT (13)                


//pll_setting1
#define RF_INTERFACE_VCO_BAND_REG(n)   (((n)&0x7FF)<<0)    
#define RF_INTERFACE_VCO_BAND_REG_MASK (0x7FF<<0)          
#define RF_INTERFACE_VCO_BAND_REG_SHIFT (0)                 

#define RF_INTERFACE_VCO_BAND_DR       (1<<11)             

#define RF_INTERFACE_PLL_CAL_BIT(n)    (((n)&0x3)<<12)     
#define RF_INTERFACE_PLL_CAL_BIT_LOWEST (((00)&0x3)<<12)    
#define RF_INTERFACE_PLL_CAL_BIT_MEDIAL (((01)&0x3)<<12)    
#define RF_INTERFACE_PLL_CAL_BIT_MEDIAL (((10)&0x3)<<12)    
#define RF_INTERFACE_PLL_CAL_BIT_HIGHEST (((11)&0x3)<<12)    
#define RF_INTERFACE_PLL_CAL_BIT_MASK  (0x3<<12)           
#define RF_INTERFACE_PLL_CAL_BIT_SHIFT (12)                


//pll_setting2
#define RF_INTERFACE_PLL_CP_BIT_RX(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_CP_BIT_RX_MASK (0x3F<<0)           
#define RF_INTERFACE_PLL_CP_BIT_RX_SHIFT (0)                 

#define RF_INTERFACE_PLL_GAIN_BIT_RX(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_PLL_GAIN_BIT_RX_MASK (0x7<<6)            
#define RF_INTERFACE_PLL_GAIN_BIT_RX_SHIFT (6)                 

#define RF_INTERFACE_PLL_FBC_SEL_RX(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_PLL_FBC_SEL_RX_MASK (0x7<<9)            
#define RF_INTERFACE_PLL_FBC_SEL_RX_SHIFT (9)                 

#define RF_INTERFACE_MDLL_DIV_RX(n)    (((n)&0xF)<<12)     
#define RF_INTERFACE_MDLL_DIV_RX_MASK  (0xF<<12)           
#define RF_INTERFACE_MDLL_DIV_RX_SHIFT (12)                


//pll_setting3
#define RF_INTERFACE_PLL_CP_BIT_TX(n)  (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_CP_BIT_TX_MASK (0x3F<<0)           
#define RF_INTERFACE_PLL_CP_BIT_TX_SHIFT (0)                 

#define RF_INTERFACE_PLL_GAIN_BIT_TX(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_PLL_GAIN_BIT_TX_MASK (0x7<<6)            
#define RF_INTERFACE_PLL_GAIN_BIT_TX_SHIFT (6)                 

#define RF_INTERFACE_PLL_FBC_SEL_TX(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_PLL_FBC_SEL_TX_MASK (0x7<<9)            
#define RF_INTERFACE_PLL_FBC_SEL_TX_SHIFT (9)                 

#define RF_INTERFACE_MDLL_DIV_TX(n)    (((n)&0xF)<<12)     
#define RF_INTERFACE_MDLL_DIV_TX_MASK  (0xF<<12)           
#define RF_INTERFACE_MDLL_DIV_TX_SHIFT (12)                


//pll_setting4
#define RF_INTERFACE_PLL_PFD_RES_BIT_RX(n) (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_PFD_RES_BIT_RX_MASK (0x3F<<0)           
#define RF_INTERFACE_PLL_PFD_RES_BIT_RX_SHIFT (0)                 

#define RF_INTERFACE_PLL_LPF_GAIN_BIT_RX(n) (((n)&0x1F)<<6)     
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_RX_MASK (0x1F<<6)           
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_RX_SHIFT (6)                 

#define RF_INTERFACE_PLL_R_BIT_RX(n)   (((n)&0x1F)<<11)    
#define RF_INTERFACE_PLL_R_BIT_RX_MASK (0x1F<<11)          
#define RF_INTERFACE_PLL_R_BIT_RX_SHIFT (11)                


//pll_setting5
#define RF_INTERFACE_PLL_PFD_RES_BIT_TX(n) (((n)&0x3F)<<0)     
#define RF_INTERFACE_PLL_PFD_RES_BIT_TX_MASK (0x3F<<0)           
#define RF_INTERFACE_PLL_PFD_RES_BIT_TX_SHIFT (0)                 

#define RF_INTERFACE_PLL_LPF_GAIN_BIT_TX(n) (((n)&0x1F)<<6)     
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_TX_MASK (0x1F<<6)           
#define RF_INTERFACE_PLL_LPF_GAIN_BIT_TX_SHIFT (6)                 

#define RF_INTERFACE_PLL_R_BIT_TX(n)   (((n)&0x1F)<<11)    
#define RF_INTERFACE_PLL_R_BIT_TX_MASK (0x1F<<11)          
#define RF_INTERFACE_PLL_R_BIT_TX_SHIFT (11)                


//pll_status
#define RF_INTERFACE_PLL_LOCK_DET      (1<<0)              


//vco_cal_setting
#define RF_INTERFACE_VCO_PKD_CLK_EDGE_SEL (1<<0)              

#define RF_INTERFACE_VCO_CAL_POLARITY  (1<<1)              

#define RF_INTERFACE_VCO_CAL_INIT_DELAY(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_VCO_CAL_INIT_DELAY_MASK (0x3<<2)            
#define RF_INTERFACE_VCO_CAL_INIT_DELAY_SHIFT (2)                 

#define RF_INTERFACE_VCO_CAL_WAIT_CNT(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_VCO_CAL_WAIT_CNT_MASK (0x3<<4)            
#define RF_INTERFACE_VCO_CAL_WAIT_CNT_SHIFT (4)                 

#define RF_INTERFACE_VCO_CAL_CNT(n)    (((n)&0x3)<<6)      
#define RF_INTERFACE_VCO_CAL_CNT_MASK  (0x3<<6)            
#define RF_INTERFACE_VCO_CAL_CNT_SHIFT (6)                 

#define RF_INTERFACE_VCO_CAL_MODE      (1<<8)              


//pll_cal_setting
#define RF_INTERFACE_PLL_INIT_DELAY(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((0)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((1)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((2)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((3)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((4)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((5)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((6)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_INIT_DELAY_LOOP (((7)&0x7)<<0)      
#define RF_INTERFACE_PLL_INIT_DELAY_MASK (0x7<<0)            
#define RF_INTERFACE_PLL_INIT_DELAY_SHIFT (0)                 

#define RF_INTERFACE_PLL_CAL_CNT_SEL(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL_EACH_CNT_TIME (((0)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL_EACH_CNT_TIME (((1)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL_EACH_CNT_TIME (((2)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL_EACH_CNT_TIME (((3)&0x7)<<3)      
#define RF_INTERFACE_PLL_CAL_CNT_SEL_MASK (0x7<<3)            
#define RF_INTERFACE_PLL_CAL_CNT_SEL_SHIFT (3)                 

#define RF_INTERFACE_PLL_CAL_OPT       (1<<6)              

#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME(n) (((n)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((0)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((1)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((2)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((3)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((4)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((5)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((6)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME (((7)&0x7)<<7)      
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_MASK (0x7<<7)            
#define RF_INTERFACE_PLL_VCO_BIT_HOLD_TIME_SHIFT (7)                 

#define RF_INTERFACE_PLL_CNT_ENABLE_POLARITY (1<<10)             

#define RF_INTERFACE_RESET_PLL_LOCK_DELAY(n) (((n)&0x3)<<11)     
#define RF_INTERFACE_RESET_PLL_LOCK_DELAY_MASK (0x3<<11)           
#define RF_INTERFACE_RESET_PLL_LOCK_DELAY_SHIFT (11)                


//sdm_setting
#define RF_INTERFACE_SDM_DELAY_SEL(n)  (((n)&0xF)<<0)      
#define RF_INTERFACE_SDM_DELAY_SEL_MASK (0xF<<0)            
#define RF_INTERFACE_SDM_DELAY_SEL_SHIFT (0)                 

#define RF_INTERFACE_SDM_DITHER_BYPASS (1<<4)              

#define RF_INTERFACE_SDM_INT_DEC_SEL(n) (((n)&0x3)<<5)      
#define RF_INTERFACE_SDM_INT_DEC_SEL_INT (((00)&0x3)<<5)     
#define RF_INTERFACE_SDM_INT_DEC_SEL_1BIT (((01)&0x3)<<5)     
#define RF_INTERFACE_SDM_INT_DEC_SEL_2BITS (((10)&0x3)<<5)     
#define RF_INTERFACE_SDM_INT_DEC_SEL_BYPASS (((11)&0x3)<<5)     
#define RF_INTERFACE_SDM_INT_DEC_SEL_MASK (0x3<<5)            
#define RF_INTERFACE_SDM_INT_DEC_SEL_SHIFT (5)                 

#define RF_INTERFACE_SDM_CLK_EDGE      (1<<7)              

#define RF_INTERFACE_PLL_SDM_RESETN_REG (1<<8)              

#define RF_INTERFACE_PLL_SDM_RESETN_DR (1<<9)              

#define RF_INTERFACE_RESET_PLL_SDM_DELAY(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_1US (((00)&0x3)<<10)    
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_2US (((01)&0x3)<<10)    
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_8US (((10)&0x3)<<10)    
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_16US (((11)&0x3)<<10)    
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_MASK (0x3<<10)           
#define RF_INTERFACE_RESET_PLL_SDM_DELAY_SHIFT (10)                


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
#define RF_INTERFACE_IVREF_AVDD_SEL_SELECT (1<<0)              
#define RF_INTERFACE_IVREF_AVDD_SEL_SELECT (1<<0)              

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
#define RF_INTERFACE_BT_RXFLT_GAIN_0_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_0_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_0(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_0_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_0_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_0(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_0_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_0_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_0(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_0_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_0_SHIFT (9)                 


//bt_gain_table_1
#define RF_INTERFACE_BT_RXFLT_GAIN_1(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_1_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_1_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_1(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_1_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_1_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_1(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_1_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_1_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_1(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_1_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_1_SHIFT (9)                 


//bt_gain_table_2
#define RF_INTERFACE_BT_RXFLT_GAIN_2(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_2_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_2_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_2(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_2_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_2_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_2(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_2_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_2_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_2(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_2_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_2_SHIFT (9)                 


//bt_gain_table_3
#define RF_INTERFACE_BT_RXFLT_GAIN_3(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_3_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_3_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_3(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_3_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_3_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_3(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_3_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_3_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_3(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_3_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_3_SHIFT (9)                 


//bt_gain_table_4
#define RF_INTERFACE_BT_RXFLT_GAIN_4(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_4_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_4_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_4(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_4_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_4_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_4(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_4_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_4_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_4(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_4_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_4_SHIFT (9)                 


//bt_gain_table_5
#define RF_INTERFACE_BT_RXFLT_GAIN_5(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_5_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_5_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_5(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_5_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_5_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_5(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_5_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_5_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_5(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_5_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_5_SHIFT (9)                 


//bt_gain_table_6
#define RF_INTERFACE_BT_RXFLT_GAIN_6(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_6_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_6_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_6(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_6_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_6_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_6(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_6_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_6_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_6(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_6_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_6_SHIFT (9)                 


//bt_gain_table_7
#define RF_INTERFACE_BT_RXFLT_GAIN_7(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_7_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_7_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_7(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_7_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_7_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_7(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_7_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_7_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_7(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_7_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_7_SHIFT (9)                 


//bt_gain_table_8
#define RF_INTERFACE_BT_RXFLT_GAIN_8(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_8_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_8_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_8(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_8_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_8_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_8(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_8_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_8_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_8(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_8_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_8_SHIFT (9)                 


//bt_gain_table_9
#define RF_INTERFACE_BT_RXFLT_GAIN_9(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_9_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_9_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_9(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_9_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_9_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_9(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_9_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_9_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_9(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_9_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_9_SHIFT (9)                 


//bt_gain_table_a
#define RF_INTERFACE_BT_RXFLT_GAIN_A(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_A_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_A_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_A(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_A_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_A_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_A(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_A_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_A_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_A(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_A_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_A_SHIFT (9)                 


//bt_gain_table_b
#define RF_INTERFACE_BT_RXFLT_GAIN_B(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_B_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_B_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_B(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_B_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_B_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_B(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_B_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_B_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_B(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_B_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_B_SHIFT (9)                 


//bt_gain_table_c
#define RF_INTERFACE_BT_RXFLT_GAIN_C(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_C_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_C_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_C(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_C_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_C_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_C(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_C_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_C_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_C(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_C_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_C_SHIFT (9)                 


//bt_gain_table_d
#define RF_INTERFACE_BT_RXFLT_GAIN_D(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_D_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_D_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_D(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_D_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_D_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_D(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_D_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_D_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_D(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_D_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_D_SHIFT (9)                 


//bt_gain_table_e
#define RF_INTERFACE_BT_RXFLT_GAIN_E(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_E_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_E_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_E(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_E_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_E_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_E(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_E_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_E_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_E(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_E_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_E_SHIFT (9)                 


//bt_gain_table_f
#define RF_INTERFACE_BT_RXFLT_GAIN_F(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_BT_RXFLT_GAIN_F_MASK (0x7<<0)            
#define RF_INTERFACE_BT_RXFLT_GAIN_F_SHIFT (0)                 

#define RF_INTERFACE_BT_RMX_GAIN_F(n)  (((n)&0xF)<<3)      
#define RF_INTERFACE_BT_RMX_GAIN_F_MASK (0xF<<3)            
#define RF_INTERFACE_BT_RMX_GAIN_F_SHIFT (3)                 

#define RF_INTERFACE_BT_LNA_GAIN2_F(n) (((n)&0x3)<<7)      
#define RF_INTERFACE_BT_LNA_GAIN2_F_MASK (0x3<<7)            
#define RF_INTERFACE_BT_LNA_GAIN2_F_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_GAIN1_F(n) (((n)&0xF)<<9)      
#define RF_INTERFACE_BT_LNA_GAIN1_F_MASK (0xF<<9)            
#define RF_INTERFACE_BT_LNA_GAIN1_F_SHIFT (9)                 

#define RF_INTERFACE_BT_AGC_GAIN_DR    (1<<13)             


//bt_ibit_table_0
#define RF_INTERFACE_BT_LNA_IBIT2_1(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_1_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_1_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_1(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_1_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_1_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_0(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_0_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_0_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_0(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_0_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_0_SHIFT (11)                


//bt_ibit_table_1
#define RF_INTERFACE_BT_LNA_IBIT2_3(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_3_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_3_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_3(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_3_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_3_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_2(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_2_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_2_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_2(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_2_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_2_SHIFT (11)                


//bt_ibit_table_2
#define RF_INTERFACE_BT_LNA_IBIT2_5(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_5_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_5_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_5(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_5_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_5_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_4(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_4_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_4_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_4(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_4_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_4_SHIFT (11)                


//bt_ibit_table_3
#define RF_INTERFACE_BT_LNA_IBIT2_7(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_7_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_7_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_7(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_7_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_7_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_6(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_6_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_6_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_6(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_6_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_6_SHIFT (11)                


//bt_ibit_table_4
#define RF_INTERFACE_BT_LNA_IBIT2_9(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_9_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_9_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_9(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_9_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_9_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_8(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_8_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_8_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_8(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_8_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_8_SHIFT (11)                


//bt_ibit_table_5
#define RF_INTERFACE_BT_LNA_IBIT2_B(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_B_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_B_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_B(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_B_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_B_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_A(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_A_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_A_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_A(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_A_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_A_SHIFT (11)                


//bt_ibit_table_6
#define RF_INTERFACE_BT_LNA_IBIT2_D(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_D_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_D_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_D(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_D_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_D_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_C(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_C_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_C_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_C(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_C_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_C_SHIFT (11)                


//bt_ibit_table_7
#define RF_INTERFACE_BT_LNA_IBIT2_F(n) (((n)&0xF)<<0)      
#define RF_INTERFACE_BT_LNA_IBIT2_F_MASK (0xF<<0)            
#define RF_INTERFACE_BT_LNA_IBIT2_F_SHIFT (0)                 

#define RF_INTERFACE_BT_LNA_IBIT1_F(n) (((n)&0x7)<<4)      
#define RF_INTERFACE_BT_LNA_IBIT1_F_MASK (0x7<<4)            
#define RF_INTERFACE_BT_LNA_IBIT1_F_SHIFT (4)                 

#define RF_INTERFACE_BT_LNA_IBIT2_E(n) (((n)&0xF)<<7)      
#define RF_INTERFACE_BT_LNA_IBIT2_E_MASK (0xF<<7)            
#define RF_INTERFACE_BT_LNA_IBIT2_E_SHIFT (7)                 

#define RF_INTERFACE_BT_LNA_IBIT1_E(n) (((n)&0x7)<<11)     
#define RF_INTERFACE_BT_LNA_IBIT1_E_MASK (0x7<<11)           
#define RF_INTERFACE_BT_LNA_IBIT1_E_SHIFT (11)                


//bt_ibit_table_8
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_7(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_7_MASK (0x3<<0)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_7_SHIFT (0)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_6(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_6_MASK (0x3<<2)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_6_SHIFT (2)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_5(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_5_MASK (0x3<<4)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_5_SHIFT (4)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_4(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_4_MASK (0x3<<6)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_4_SHIFT (6)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_3(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_3_MASK (0x3<<8)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_3_SHIFT (8)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_2(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_2_MASK (0x3<<10)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_2_SHIFT (10)                

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_1(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_1_MASK (0x3<<12)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_1_SHIFT (12)                

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_0(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_0_MASK (0x3<<14)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_0_SHIFT (14)                


//bt_ibit_table_9
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_F(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_F_MASK (0x3<<0)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_F_SHIFT (0)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_E(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_E_MASK (0x3<<2)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_E_SHIFT (2)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_D(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_D_MASK (0x3<<4)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_D_SHIFT (4)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_C(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_C_MASK (0x3<<6)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_C_SHIFT (6)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_B(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_B_MASK (0x3<<8)            
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_B_SHIFT (8)                 

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_A(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_A_MASK (0x3<<10)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_A_SHIFT (10)                

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_9(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_9_MASK (0x3<<12)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_9_SHIFT (12)                

#define RF_INTERFACE_BT_RXFLT_OP_IBIT_8(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_8_MASK (0x3<<14)           
#define RF_INTERFACE_BT_RXFLT_OP_IBIT_8_SHIFT (14)                


//tx_padrv_gain_setting_0
#define RF_INTERFACE_BT_PADRV_GAIN_1(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_1_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_1_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_0(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_0_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_0_SHIFT (8)                 


//tx_padrv_gain_setting_1
#define RF_INTERFACE_BT_PADRV_GAIN_3(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_3_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_3_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_2(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_2_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_2_SHIFT (8)                 


//tx_padrv_gain_setting_2
#define RF_INTERFACE_BT_PADRV_GAIN_5(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_5_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_5_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_4(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_4_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_4_SHIFT (8)                 


//tx_padrv_gain_setting_3
#define RF_INTERFACE_BT_PADRV_GAIN_7(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_7_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_7_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_6(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_6_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_6_SHIFT (8)                 


//tx_padrv_gain_setting_4
#define RF_INTERFACE_BT_PADRV_GAIN_9(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_9_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_9_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_8(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_8_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_8_SHIFT (8)                 


//tx_padrv_gain_setting_5
#define RF_INTERFACE_BT_PADRV_GAIN_B(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_B_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_B_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_A(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_A_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_A_SHIFT (8)                 


//tx_padrv_gain_setting_6
#define RF_INTERFACE_BT_PADRV_GAIN_D(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_D_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_D_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_C(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_C_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_C_SHIFT (8)                 


//tx_padrv_gain_setting_7
#define RF_INTERFACE_BT_PADRV_GAIN_F(n) (((n)&0xFF)<<0)     
#define RF_INTERFACE_BT_PADRV_GAIN_F_MASK (0xFF<<0)           
#define RF_INTERFACE_BT_PADRV_GAIN_F_SHIFT (0)                 

#define RF_INTERFACE_BT_PADRV_GAIN_E(n) (((n)&0xFF)<<8)     
#define RF_INTERFACE_BT_PADRV_GAIN_E_MASK (0xFF<<8)           
#define RF_INTERFACE_BT_PADRV_GAIN_E_SHIFT (8)                 


//fm_lna_gain_setting0
#define RF_INTERFACE_FM_LNA_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_0_MASK (0x3<<0)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_0_SHIFT (0)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_1_MASK (0x3<<2)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_1_SHIFT (2)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_2_MASK (0x3<<4)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_2_SHIFT (4)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_3_MASK (0x3<<6)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_3_SHIFT (6)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_4_MASK (0x3<<8)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_4_SHIFT (8)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_5_MASK (0x3<<10)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_5_SHIFT (10)                

#define RF_INTERFACE_FM_LNA_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_6_MASK (0x3<<12)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_6_SHIFT (12)                

#define RF_INTERFACE_FM_LNA_GAIN_BIT_7(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_7_MASK (0x3<<14)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_7_SHIFT (14)                


//fm_lna_gain_setting1
#define RF_INTERFACE_FM_LNA_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_8_MASK (0x3<<0)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_8_SHIFT (0)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_9_MASK (0x3<<2)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_9_SHIFT (2)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_A_MASK (0x3<<4)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_A_SHIFT (4)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_B_MASK (0x3<<6)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_B_SHIFT (6)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_LNA_GAIN_BIT_C_MASK (0x3<<8)            
#define RF_INTERFACE_FM_LNA_GAIN_BIT_C_SHIFT (8)                 

#define RF_INTERFACE_FM_LNA_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_D_MASK (0x3<<10)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_D_SHIFT (10)                

#define RF_INTERFACE_FM_LNA_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_E_MASK (0x3<<12)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_E_SHIFT (12)                

#define RF_INTERFACE_FM_LNA_GAIN_BIT_F(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_LNA_GAIN_BIT_F_MASK (0x3<<14)           
#define RF_INTERFACE_FM_LNA_GAIN_BIT_F_SHIFT (14)                


//fm_mixer_gain_setting0
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_0(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_0_MASK (0x7<<0)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_0_SHIFT (0)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_1(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_1_MASK (0x7<<3)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_1_SHIFT (3)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_2(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_2_MASK (0x7<<6)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_2_SHIFT (6)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_3(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_3_MASK (0x7<<9)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_3_SHIFT (9)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_4(n) (((n)&0x7)<<12)     
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_4_MASK (0x7<<12)           
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_4_SHIFT (12)                


//fm_mixer_gain_setting1
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_5(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_5_MASK (0x7<<0)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_5_SHIFT (0)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_6(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_6_MASK (0x7<<3)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_6_SHIFT (3)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_7(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_7_MASK (0x7<<6)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_7_SHIFT (6)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_8(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_8_MASK (0x7<<9)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_8_SHIFT (9)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_9(n) (((n)&0x7)<<12)     
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_9_MASK (0x7<<12)           
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_9_SHIFT (12)                


//fm_mixer_gain_setting2
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_A(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_A_MASK (0x7<<0)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_A_SHIFT (0)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_B(n) (((n)&0x7)<<3)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_B_MASK (0x7<<3)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_B_SHIFT (3)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_C(n) (((n)&0x7)<<6)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_C_MASK (0x7<<6)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_C_SHIFT (6)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_D(n) (((n)&0x7)<<9)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_D_MASK (0x7<<9)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_D_SHIFT (9)                 

#define RF_INTERFACE_FM_MIXER_GAIN_BIT_E(n) (((n)&0x7)<<12)     
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_E_MASK (0x7<<12)           
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_E_SHIFT (12)                


//fm_mixer_gain_setting3
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_F(n) (((n)&0x7)<<0)      
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_F_MASK (0x7<<0)            
#define RF_INTERFACE_FM_MIXER_GAIN_BIT_F_SHIFT (0)                 


//fm_filter_gain_setting0
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_0_MASK (0x3<<0)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_0_SHIFT (0)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_1_MASK (0x3<<2)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_1_SHIFT (2)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_2_MASK (0x3<<4)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_2_SHIFT (4)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_3_MASK (0x3<<6)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_3_SHIFT (6)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_4_MASK (0x3<<8)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_4_SHIFT (8)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_5_MASK (0x3<<10)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_5_SHIFT (10)                

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_6_MASK (0x3<<12)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_6_SHIFT (12)                

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_7(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_7_MASK (0x3<<14)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_7_SHIFT (14)                


//fm_filter_gain_setting1
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_8_MASK (0x3<<0)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_8_SHIFT (0)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_9_MASK (0x3<<2)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_9_SHIFT (2)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_A_MASK (0x3<<4)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_A_SHIFT (4)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_B_MASK (0x3<<6)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_B_SHIFT (6)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_C_MASK (0x3<<8)            
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_C_SHIFT (8)                 

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_D_MASK (0x3<<10)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_D_SHIFT (10)                

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_E_MASK (0x3<<12)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_E_SHIFT (12)                

#define RF_INTERFACE_FM_FILTER_GAIN_BIT_F(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_F_MASK (0x3<<14)           
#define RF_INTERFACE_FM_FILTER_GAIN_BIT_F_SHIFT (14)                


//fm_adc_gain_setting0
#define RF_INTERFACE_FM_ADC_GAIN_BIT_0(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_0_MASK (0x3<<0)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_0_SHIFT (0)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_1(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_1_MASK (0x3<<2)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_1_SHIFT (2)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_2(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_2_MASK (0x3<<4)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_2_SHIFT (4)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_3(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_3_MASK (0x3<<6)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_3_SHIFT (6)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_4(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_4_MASK (0x3<<8)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_4_SHIFT (8)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_5(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_5_MASK (0x3<<10)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_5_SHIFT (10)                

#define RF_INTERFACE_FM_ADC_GAIN_BIT_6(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_6_MASK (0x3<<12)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_6_SHIFT (12)                

#define RF_INTERFACE_FM_ADC_GAIN_BIT_7(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_7_MASK (0x3<<14)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_7_SHIFT (14)                


//fm_adc_gain_setting1
#define RF_INTERFACE_FM_ADC_GAIN_BIT_8(n) (((n)&0x3)<<0)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_8_MASK (0x3<<0)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_8_SHIFT (0)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_9(n) (((n)&0x3)<<2)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_9_MASK (0x3<<2)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_9_SHIFT (2)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_A(n) (((n)&0x3)<<4)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_A_MASK (0x3<<4)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_A_SHIFT (4)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_B(n) (((n)&0x3)<<6)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_B_MASK (0x3<<6)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_B_SHIFT (6)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_C(n) (((n)&0x3)<<8)      
#define RF_INTERFACE_FM_ADC_GAIN_BIT_C_MASK (0x3<<8)            
#define RF_INTERFACE_FM_ADC_GAIN_BIT_C_SHIFT (8)                 

#define RF_INTERFACE_FM_ADC_GAIN_BIT_D(n) (((n)&0x3)<<10)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_D_MASK (0x3<<10)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_D_SHIFT (10)                

#define RF_INTERFACE_FM_ADC_GAIN_BIT_E(n) (((n)&0x3)<<12)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_E_MASK (0x3<<12)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_E_SHIFT (12)                

#define RF_INTERFACE_FM_ADC_GAIN_BIT_F(n) (((n)&0x3)<<14)     
#define RF_INTERFACE_FM_ADC_GAIN_BIT_F_MASK (0x3<<14)           
#define RF_INTERFACE_FM_ADC_GAIN_BIT_F_SHIFT (14)                


#endif

#endif


