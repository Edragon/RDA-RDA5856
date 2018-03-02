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
// File name      : audio_codec                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:32                                                  
// Last modified  : 2016-12-21 13:55:32                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:32 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AUDIO_CODEC_H__
#define __AUDIO_CODEC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//audio_function_id
typedef union
{
    U32 v;
    struct
    {
        U32 fid                            :16; /*15:0 , Function ID                   */
        U32 audio_function_id_reserved_0   :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_audio_function_id;

//codec_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 s_isphone                      :1 ; /*0 :0 , Phone Mode1: phone in/out    */
        U32 s_phone_sel                    :1 ; /*1 :1 , Phone Selection               */
        U32 s_loop                         :1 ; /*2 :2 , Loop Test                     */
        U32 s_dac_1k                       :1 ; /*3 :3 , DAC 1K Sine Wave Test         */
        U32 fm_mode                        :1 ; /*4 :4 , Audio FM Mode0:: codec mode1:: FM mode;*/
        U32 sleep_mode                     :1 ; /*5 :5 , Sleep Mode1:: Sleep mode 0:: Normal mode*/
        U32 s_codec_en                     :1 ; /*6 :6 , Codec Enable1:: enable codec dsp*/
        U32 codec_cfg_reserved_0           :25; /*31:7 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_codec_cfg;

//adc_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 adc_clk_ld                     :1 ; /*0 :0 , load adc clock divider value  */
        U32 cnt_adc_clk_int                :7 ; /*7 :1 , ADC Clock Divider Integerdivider number for Audio CODEC clock div to ADC clock; Integer part*/
        U32 adc_clk_cfg_reserved_0         :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_adc_clk_cfg;

//adc_pathcfg
typedef union
{
    U32 v;
    struct
    {
        U32 adc_clkgen_mode                :1 ; /*0 :0 , ADC Clock Generation Mode     */
        U32 ana_adc_clk_inv                :1 ; /*1 :1 , Analog ADC Clock Inverter     */
        U32 s_adc_osr_sel                  :3 ; /*4 :2 , ADC OSR Select                */
        U32 adc_lpf_bypass                 :1 ; /*5 :5 , ADC LPF Bypass                */
        U32 s_hpf_bypass                   :1 ; /*6 :6 , MIC HPF Bypass                */
        U32 s_adc_volume                   :4 ; /*10:7 , ADC Volume                    */
        U32 adc_dig_sel                    :1 ; /*11:11, select dig mic input for adc path*/
        U32 dig_mic_switch                 :1 ; /*12:12, switch dig mic L/R channel signal*/
        U32 adc_path_reset                 :1 ; /*13:13, ADC Path Reset1:: Reset      */
        U32 s_adc_en                       :1 ; /*14:14, enable adc                    */
        U32 adc_out_format                 :1 ; /*15:15, 0:: mono output(left channel)1:: stereo output */
        U32 adc_pathcfg_reserved_0         :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_adc_pathcfg;

//dac_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 dac_clk_ld                     :1 ; /*0 :0 , load dac clock divider value  */
        U32 fs_cnt_int                     :10; /*10:1 , Sample Rate Divider Counter integer part*/
        U32 dac_clk_cfg_reserved_0         :21; /*31:11, RESERVED                      */
    } b;
} T_AUDIO_CODEC_dac_clk_cfg;

//dac_path_cfg1
typedef union
{
    U32 v;
    struct
    {
        U32 s_dac_volume_mute              :1 ; /*0 :0 , DAC Volume Mute1: mute both DAC Left &amp; Right channels*/
        U32 s_mute_r                       :1 ; /*1 :1 , DAC Mute Right                */
        U32 s_mute_l                       :1 ; /*2 :2 , DAC Mute Left                 */
        U32 s_dac_volume                   :6 ; /*8 :3 , DAC Volume                    */
        U32 s_dac_gain                     :2 ; /*10:9 , DAC Gainunit is 6dB          */
        U32 s_dac_osr_sel                  :2 ; /*12:11, DAC OSR Select2'b00:: 128 OSR2'b01:: 256 OSR2'b10:: 512 OSR2'b11:: 512 OSR*/
        U32 dac_path_reset                 :1 ; /*13:13, DAC Path Reset                */
        U32 s_dac_en                       :1 ; /*14:14, enable dac                    */
        U32 dac_path_cfg1_reserved_0       :17; /*31:15, RESERVED                      */
    } b;
} T_AUDIO_CODEC_dac_path_cfg1;

//dac_path_cfg2
typedef union
{
    U32 v;
    struct
    {
        U32 s_fs_sel                       :3 ; /*2 :0 , DAC Ramp Sample Rate Select   */
        U32 sdm_gain                       :2 ; /*4 :3 , SDM Gain                      */
        U32 dither_gain_2_0                :3 ; /*7 :5 , SDM Dither Gain [2:0]         */
        U32 dither_gain_4_3                :2 ; /*9 :8 , SDM Dither Gain [4:3]         */
        U32 s_dither_bypass                :1 ; /*10:10, Dither Bypass                 */
        U32 s_dwa_en                       :1 ; /*11:11, DWA Enable                    */
        U32 dac_clkgen_mode                :1 ; /*12:12, DAC Clock Generation Mode     */
        U32 ana_dac_clk_inv                :1 ; /*13:13, Analog DAC Clock Inverter     */
        U32 dac_path_cfg2_reserved_0       :18; /*31:14, RESERVED                      */
    } b;
} T_AUDIO_CODEC_dac_path_cfg2;

//dac_ramp
typedef union
{
    U32 v;
    struct
    {
        U32 rampdn_step                    :7 ; /*6 :0 , DAC Ramp Down Step            */
        U32 rampup_step                    :7 ; /*13:7 , DAC Ramp Up Step              */
        U32 ramp_bypass                    :1 ; /*14:14, DAC Ramp Bypass               */
        U32 ramp_ind                       :1 ; /*15:15, DAC Ramp Indicator            */
        U32 dac_ramp_reserved_0            :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_dac_ramp;

//side_tone_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 s_adc_dvst_ctl_3_0             :4 ; /*3 :0 , Side Tone Gain [3:0]          */
        U32 s_adc_dvst_ctl_4               :1 ; /*4 :4 , Side Tone Gain [4]            */
        U32 side_tone_cfg_reserved_0       :27; /*31:5 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_side_tone_cfg;

//test_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 tports_sel                     :5 ; /*4 :0 , Test Ports Selection          */
        U32 tports_en                      :1 ; /*5 :5 , Test Ports Enable             */
        U32 tports_out_sel                 :1 ; /*6 :6 , 1: output high 16bit of tports0: output low 16bit of tports*/
        U32 test_ctrl_reserved_0           :25; /*31:7 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_test_ctrl;

//test_port_lo
typedef union
{
    U32 v;
    struct
    {
        U32 tports_lo                      :16; /*15:0 , Test Ports low 16 bit         */
        U32 test_port_lo_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_test_port_lo;

//test_port_hi
typedef union
{
    U32 v;
    struct
    {
        U32 tports_hi                      :16; /*15:0 , Test Ports high 16 bit        */
        U32 test_port_hi_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_test_port_hi;

//eq_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 eq_reset                       :1 ; /*0 :0 , reset eq module               */
        U32 eq_bypass                      :1 ; /*1 :1 , eq bypass                     */
        U32 eq_ctrl_reserved_0             :30; /*31:2 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_ctrl;

//eq_coef0_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef0_lo                    :16; /*15:0 , 1st stage EQ B0 coef lo       */
        U32 eq_coef0_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef0_low;

//eq_coef0_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef0_hi                    :8 ; /*7 :0 , 1st stage EQ B0 coef hi       */
        U32 eq_coef0_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef0_high;

//eq_coef1_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef1_lo                    :16; /*15:0 , 1st stage EQ B1 coef lo       */
        U32 eq_coef1_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef1_low;

//eq_coef1_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef1_hi                    :8 ; /*7 :0 , 1st stage EQ B1 coef hi       */
        U32 eq_coef1_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef1_high;

//eq_coef2_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef2_lo                    :16; /*15:0 , 1st stage EQ A1 coef lo       */
        U32 eq_coef2_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef2_low;

//eq_coef2_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef2_hi                    :8 ; /*7 :0 , 1st stage EQ A1 coef hi       */
        U32 eq_coef2_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef2_high;

//eq_coef3_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef3_lo                    :16; /*15:0 , 1st stage EQ B2 coef lo       */
        U32 eq_coef3_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef3_low;

//eq_coef3_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef3_hi                    :8 ; /*7 :0 , 1st stage EQ B2 coef hi       */
        U32 eq_coef3_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef3_high;

//eq_coef4_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef4_lo                    :16; /*15:0 , 1st stage EQ A2 coef lo       */
        U32 eq_coef4_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef4_low;

//eq_coef4_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef4_hi                    :8 ; /*7 :0 , 1st stage EQ A2 coef hi       */
        U32 eq_coef4_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef4_high;

//eq_coef5_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef5_lo                    :16; /*15:0 , 2nd stage EQ B0 coef lo       */
        U32 eq_coef5_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef5_low;

//eq_coef5_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef5_hi                    :8 ; /*7 :0 , 2nd stage EQ B0 coef hi       */
        U32 eq_coef5_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef5_high;

//eq_coef6_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef6_lo                    :16; /*15:0 , 2nd stage EQ B1 coef lo       */
        U32 eq_coef6_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef6_low;

//eq_coef6_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef6_hi                    :8 ; /*7 :0 , 2nd stage EQ B1 coef hi       */
        U32 eq_coef6_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef6_high;

//eq_coef7_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef7_lo                    :16; /*15:0 , 2nd stage EQ A1 coef lo       */
        U32 eq_coef7_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef7_low;

//eq_coef7_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef7_hi                    :8 ; /*7 :0 , 2nd stage EQ A1 coef hi       */
        U32 eq_coef7_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef7_high;

//eq_coef8_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef8_lo                    :16; /*15:0 , 2nd stage EQ B2 coef lo       */
        U32 eq_coef8_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef8_low;

//eq_coef8_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef8_hi                    :8 ; /*7 :0 , 2nd stage EQ B2 coef hi       */
        U32 eq_coef8_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef8_high;

//eq_coef9_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef9_lo                    :16; /*15:0 , 2nd stage EQ A2 coef lo       */
        U32 eq_coef9_low_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef9_low;

//eq_coef9_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef9_hi                    :8 ; /*7 :0 , 2nd stage EQ A2 coef hi       */
        U32 eq_coef9_high_reserved_0       :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef9_high;

//eq_coef10_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef10_lo                   :16; /*15:0 , 3rd stage EQ B0 coef lo       */
        U32 eq_coef10_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef10_low;

//eq_coef10_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef10_hi                   :8 ; /*7 :0 , 3rd stage EQ B0 coef hi       */
        U32 eq_coef10_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef10_high;

//eq_coef11_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef11_lo                   :16; /*15:0 , 3rd stage EQ B1 coef lo       */
        U32 eq_coef11_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef11_low;

//eq_coef11_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef11_hi                   :8 ; /*7 :0 , 3rd stage EQ B1 coef hi       */
        U32 eq_coef11_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef11_high;

//eq_coef12_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef12_lo                   :16; /*15:0 , 3rd stage EQ A1 coef lo       */
        U32 eq_coef12_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef12_low;

//eq_coef12_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef12_hi                   :8 ; /*7 :0 , 3rd stage EQ A1 coef hi       */
        U32 eq_coef12_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef12_high;

//eq_coef13_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef13_lo                   :16; /*15:0 , 3rd stage EQ B2 coef lo       */
        U32 eq_coef13_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef13_low;

//eq_coef13_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef13_hi                   :8 ; /*7 :0 , 3rd stage EQ B2 coef hi       */
        U32 eq_coef13_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef13_high;

//eq_coef14_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef14_lo                   :16; /*15:0 , 3rd stage EQ A2 coef lo       */
        U32 eq_coef14_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef14_low;

//eq_coef14_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef14_hi                   :8 ; /*7 :0 , 3rd stage EQ A2 coef hi       */
        U32 eq_coef14_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef14_high;

//eq_coef15_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef15_lo                   :16; /*15:0 , 4th stage EQ B0 coef lo       */
        U32 eq_coef15_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef15_low;

//eq_coef15_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef15_hi                   :8 ; /*7 :0 , 4th stage EQ B0 coef hi       */
        U32 eq_coef15_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef15_high;

//eq_coef16_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef16_lo                   :16; /*15:0 , 4th stage EQ B1 coef lo       */
        U32 eq_coef16_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef16_low;

//eq_coef16_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef16_hi                   :8 ; /*7 :0 , 4th stage EQ B1 coef hi       */
        U32 eq_coef16_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef16_high;

//eq_coef17_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef17_lo                   :16; /*15:0 , 4th stage EQ A1 coef lo       */
        U32 eq_coef17_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef17_low;

//eq_coef17_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef17_hi                   :8 ; /*7 :0 , 4th stage EQ A1 coef hi       */
        U32 eq_coef17_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef17_high;

//eq_coef18_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef18_lo                   :16; /*15:0 , 4th stage EQ B2 coef lo       */
        U32 eq_coef18_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef18_low;

//eq_coef18_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef18_hi                   :8 ; /*7 :0 , 4th stage EQ B2 coef hi       */
        U32 eq_coef18_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef18_high;

//eq_coef19_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef19_lo                   :16; /*15:0 , 4th stage EQ A2 coef lo       */
        U32 eq_coef19_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef19_low;

//eq_coef19_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef19_hi                   :8 ; /*7 :0 , 4th stage EQ A2 coef hi       */
        U32 eq_coef19_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef19_high;

//eq_coef20_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef20_lo                   :16; /*15:0 , 5th stage EQ B0 coef lo       */
        U32 eq_coef20_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef20_low;

//eq_coef20_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef20_hi                   :8 ; /*7 :0 , 5th stage EQ B0 coef hi       */
        U32 eq_coef20_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef20_high;

//eq_coef21_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef21_lo                   :16; /*15:0 , 5th stage EQ B1 coef lo       */
        U32 eq_coef21_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef21_low;

//eq_coef21_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef21_hi                   :8 ; /*7 :0 , 5th stage EQ B1 coef hi       */
        U32 eq_coef21_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef21_high;

//eq_coef22_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef22_lo                   :16; /*15:0 , 5th stage EQ A1 coef lo       */
        U32 eq_coef22_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef22_low;

//eq_coef22_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef22_hi                   :8 ; /*7 :0 , 5th stage EQ A1 coef hi       */
        U32 eq_coef22_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef22_high;

//eq_coef23_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef23_lo                   :16; /*15:0 , 5th stage EQ B2 coef lo       */
        U32 eq_coef23_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef23_low;

//eq_coef23_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef23_hi                   :8 ; /*7 :0 , 5th stage EQ B2 coef hi       */
        U32 eq_coef23_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef23_high;

//eq_coef24_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef24_lo                   :16; /*15:0 , 5th stage EQ A2 coef lo       */
        U32 eq_coef24_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef24_low;

//eq_coef24_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef24_hi                   :8 ; /*7 :0 , 5th stage EQ A2 coef hi       */
        U32 eq_coef24_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef24_high;

//eq_coef25_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef25_lo                   :16; /*15:0 , 6th stage EQ B0 coef lo       */
        U32 eq_coef25_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef25_low;

//eq_coef25_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef25_hi                   :8 ; /*7 :0 , 6th stage EQ B0 coef hi       */
        U32 eq_coef25_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef25_high;

//eq_coef26_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef26_lo                   :16; /*15:0 , 6th stage EQ B1 coef lo       */
        U32 eq_coef26_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef26_low;

//eq_coef26_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef26_hi                   :8 ; /*7 :0 , 6th stage EQ B1 coef hi       */
        U32 eq_coef26_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef26_high;

//eq_coef27_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef27_lo                   :16; /*15:0 , 6th stage EQ A1 coef lo       */
        U32 eq_coef27_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef27_low;

//eq_coef27_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef27_hi                   :8 ; /*7 :0 , 6th stage EQ A1 coef hi       */
        U32 eq_coef27_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef27_high;

//eq_coef28_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef28_lo                   :16; /*15:0 , 6th stage EQ B2 coef lo       */
        U32 eq_coef28_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef28_low;

//eq_coef28_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef28_hi                   :8 ; /*7 :0 , 6th stage EQ B2 coef hi       */
        U32 eq_coef28_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef28_high;

//eq_coef29_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef29_lo                   :16; /*15:0 , 6th stage EQ A2 coef lo       */
        U32 eq_coef29_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef29_low;

//eq_coef29_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef29_hi                   :8 ; /*7 :0 , 6th stage EQ A2 coef hi       */
        U32 eq_coef29_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef29_high;

//eq_coef30_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef30_lo                   :16; /*15:0 , 7th stage EQ B0 coef lo       */
        U32 eq_coef30_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef30_low;

//eq_coef30_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef30_hi                   :8 ; /*7 :0 , 7th stage EQ B0 coef hi       */
        U32 eq_coef30_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef30_high;

//eq_coef31_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef31_lo                   :16; /*15:0 , 7th stage EQ B1 coef lo       */
        U32 eq_coef31_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef31_low;

//eq_coef31_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef31_hi                   :8 ; /*7 :0 , 7th stage EQ B1 coef hi       */
        U32 eq_coef31_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef31_high;

//eq_coef32_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef32_lo                   :16; /*15:0 , 7th stage EQ A1 coef lo       */
        U32 eq_coef32_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef32_low;

//eq_coef32_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef32_hi                   :8 ; /*7 :0 , 7th stage EQ A1 coef hi       */
        U32 eq_coef32_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef32_high;

//eq_coef33_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef33_lo                   :16; /*15:0 , 7th stage EQ B2 coef lo       */
        U32 eq_coef33_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef33_low;

//eq_coef33_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef33_hi                   :8 ; /*7 :0 , 7th stage EQ B2 coef hi       */
        U32 eq_coef33_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef33_high;

//eq_coef34_low
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef34_lo                   :16; /*15:0 , 7th stage EQ A2 coef lo       */
        U32 eq_coef34_low_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef34_low;

//eq_coef34_high
typedef union
{
    U32 v;
    struct
    {
        U32 eq_coef34_hi                   :8 ; /*7 :0 , 7th stage EQ A2 coef hi       */
        U32 eq_coef34_high_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_eq_coef34_high;

//coef_a1_lo
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_a1_lo                 :16; /*15:0 , adc hpf a1 coef lo            */
        U32 coef_a1_lo_reserved_0          :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_a1_lo;

//coef_a1_hi
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_a1_hi                 :4 ; /*3 :0 , adc hpf a1 coef hi            */
        U32 coef_a1_hi_reserved_0          :28; /*31:4 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_a1_hi;

//coef_a2_lo
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_a2_lo                 :16; /*15:0 , adc hpf a2 coef lo            */
        U32 coef_a2_lo_reserved_0          :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_a2_lo;

//coef_a2_hi
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_a2_hi                 :4 ; /*3 :0 , adc hpf a2 coef hi            */
        U32 coef_a2_hi_reserved_0          :28; /*31:4 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_a2_hi;

//coef_b1_lo
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_b1_lo                 :16; /*15:0 , adc hpf b1 coef lo            */
        U32 coef_b1_lo_reserved_0          :16; /*31:16, RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_b1_lo;

//coef_b1_hi
typedef union
{
    U32 v;
    struct
    {
        U32 hpf_coef_b1_hi                 :4 ; /*3 :0 , adc hpf b1 coef hi            */
        U32 coef_b1_hi_reserved_0          :28; /*31:4 , RESERVED                      */
    } b;
} T_AUDIO_CODEC_coef_b1_hi;

typedef struct
{
    volatile T_AUDIO_CODEC_audio_function_id              audio_function_id; /*0x0  , RO   , 0x0000CDEC, Audio Function ID Register    */
    volatile         T_AUDIO_CODEC_codec_cfg                      codec_cfg; /*0x4  , RW   , 0x00000000, Audio ADC clock divider config*/
    volatile       T_AUDIO_CODEC_adc_clk_cfg                    adc_clk_cfg; /*0x8  , RW   , 0x00000030, Audio ADC clock divider config*/
    volatile       T_AUDIO_CODEC_adc_pathcfg                    adc_pathcfg; /*0xc  , RW   , 0x00000380, Audio ADC path config         */
    volatile       T_AUDIO_CODEC_dac_clk_cfg                    dac_clk_cfg; /*0x10 , RW   , 0x00000030, Audio DAC clock divider config*/
    volatile     T_AUDIO_CODEC_dac_path_cfg1                  dac_path_cfg1; /*0x14 , RW   , 0x00000188, Audio DAC path config1        */
    volatile     T_AUDIO_CODEC_dac_path_cfg2                  dac_path_cfg2; /*0x18 , RW   , 0x00000910, Audio DAC path config2        */
    volatile          T_AUDIO_CODEC_dac_ramp                       dac_ramp; /*0x1c , RW   , 0x00004503, Audio DAC Ramp Register       */
    volatile     T_AUDIO_CODEC_side_tone_cfg                  side_tone_cfg; /*0x20 , RW   , 0x0000001F, side tone config              */
    volatile         T_AUDIO_CODEC_test_ctrl                      test_ctrl; /*0x24 , RW   , 0x00000000, Audio Test Control Register   */
    volatile      T_AUDIO_CODEC_test_port_lo                   test_port_lo; /*0x28 , RO   , 0x00000000, Audio Test Ports Register     */
    volatile      T_AUDIO_CODEC_test_port_hi                   test_port_hi; /*0x2c , RO   , 0x00000000, Audio Test Ports Register     */
    volatile           T_AUDIO_CODEC_eq_ctrl                        eq_ctrl; /*0x30 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef0_low                   eq_coef0_low; /*0x34 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef0_high                  eq_coef0_high; /*0x38 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef1_low                   eq_coef1_low; /*0x3c , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef1_high                  eq_coef1_high; /*0x40 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef2_low                   eq_coef2_low; /*0x44 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef2_high                  eq_coef2_high; /*0x48 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef3_low                   eq_coef3_low; /*0x4c , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef3_high                  eq_coef3_high; /*0x50 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef4_low                   eq_coef4_low; /*0x54 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef4_high                  eq_coef4_high; /*0x58 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef5_low                   eq_coef5_low; /*0x5c , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef5_high                  eq_coef5_high; /*0x60 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef6_low                   eq_coef6_low; /*0x64 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef6_high                  eq_coef6_high; /*0x68 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef7_low                   eq_coef7_low; /*0x6c , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef7_high                  eq_coef7_high; /*0x70 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef8_low                   eq_coef8_low; /*0x74 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef8_high                  eq_coef8_high; /*0x78 , RW   , 0x00000000, EQ control Register           */
    volatile      T_AUDIO_CODEC_eq_coef9_low                   eq_coef9_low; /*0x7c , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef9_high                  eq_coef9_high; /*0x80 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef10_low                  eq_coef10_low; /*0x84 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef10_high                 eq_coef10_high; /*0x88 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef11_low                  eq_coef11_low; /*0x8c , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef11_high                 eq_coef11_high; /*0x90 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef12_low                  eq_coef12_low; /*0x94 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef12_high                 eq_coef12_high; /*0x98 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef13_low                  eq_coef13_low; /*0x9c , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef13_high                 eq_coef13_high; /*0xa0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef14_low                  eq_coef14_low; /*0xa4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef14_high                 eq_coef14_high; /*0xa8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef15_low                  eq_coef15_low; /*0xac , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef15_high                 eq_coef15_high; /*0xb0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef16_low                  eq_coef16_low; /*0xb4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef16_high                 eq_coef16_high; /*0xb8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef17_low                  eq_coef17_low; /*0xbc , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef17_high                 eq_coef17_high; /*0xc0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef18_low                  eq_coef18_low; /*0xc4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef18_high                 eq_coef18_high; /*0xc8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef19_low                  eq_coef19_low; /*0xcc , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef19_high                 eq_coef19_high; /*0xd0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef20_low                  eq_coef20_low; /*0xd4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef20_high                 eq_coef20_high; /*0xd8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef21_low                  eq_coef21_low; /*0xdc , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef21_high                 eq_coef21_high; /*0xe0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef22_low                  eq_coef22_low; /*0xe4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef22_high                 eq_coef22_high; /*0xe8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef23_low                  eq_coef23_low; /*0xec , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef23_high                 eq_coef23_high; /*0xf0 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef24_low                  eq_coef24_low; /*0xf4 , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef24_high                 eq_coef24_high; /*0xf8 , RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef25_low                  eq_coef25_low; /*0xfc , RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef25_high                 eq_coef25_high; /*0x100, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef26_low                  eq_coef26_low; /*0x104, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef26_high                 eq_coef26_high; /*0x108, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef27_low                  eq_coef27_low; /*0x10c, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef27_high                 eq_coef27_high; /*0x110, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef28_low                  eq_coef28_low; /*0x114, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef28_high                 eq_coef28_high; /*0x118, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef29_low                  eq_coef29_low; /*0x11c, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef29_high                 eq_coef29_high; /*0x120, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef30_low                  eq_coef30_low; /*0x124, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef30_high                 eq_coef30_high; /*0x128, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef31_low                  eq_coef31_low; /*0x12c, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef31_high                 eq_coef31_high; /*0x130, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef32_low                  eq_coef32_low; /*0x134, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef32_high                 eq_coef32_high; /*0x138, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef33_low                  eq_coef33_low; /*0x13c, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef33_high                 eq_coef33_high; /*0x140, RW   , 0x00000000, EQ control Register           */
    volatile     T_AUDIO_CODEC_eq_coef34_low                  eq_coef34_low; /*0x144, RW   , 0x00000000, EQ control Register           */
    volatile    T_AUDIO_CODEC_eq_coef34_high                 eq_coef34_high; /*0x148, RW   , 0x00000000, EQ control Register           */
    volatile        T_AUDIO_CODEC_coef_a1_lo                     coef_a1_lo; /*0x14c, RW   , 0x00000000, hpf_coef                      */
    volatile        T_AUDIO_CODEC_coef_a1_hi                     coef_a1_hi; /*0x150, RW   , 0x00000000, EQ control Register           */
    volatile        T_AUDIO_CODEC_coef_a2_lo                     coef_a2_lo; /*0x154, RW   , 0x00000000, EQ control Register           */
    volatile        T_AUDIO_CODEC_coef_a2_hi                     coef_a2_hi; /*0x158, RW   , 0x00000000, EQ control Register           */
    volatile        T_AUDIO_CODEC_coef_b1_lo                     coef_b1_lo; /*0x15c, RW   , 0x00000000, EQ control Register           */
    volatile        T_AUDIO_CODEC_coef_b1_hi                     coef_b1_hi; /*0x160, RW   , 0x00000000, EQ control Register           */
} T_HWP_AUDIO_CODEC_T;
#else
typedef struct
{
    volatile                             U32              audio_function_id; /*0x0  , RO   , 0x0000CDEC, Audio Function ID Register    */
    volatile                             U32                      codec_cfg; /*0x4  , RW   , 0x00000000, Audio ADC clock divider config*/
    volatile                             U32                    adc_clk_cfg; /*0x8  , RW   , 0x00000030, Audio ADC clock divider config*/
    volatile                             U32                    adc_pathcfg; /*0xc  , RW   , 0x00000380, Audio ADC path config         */
    volatile                             U32                    dac_clk_cfg; /*0x10 , RW   , 0x00000030, Audio DAC clock divider config*/
    volatile                             U32                  dac_path_cfg1; /*0x14 , RW   , 0x00000188, Audio DAC path config1        */
    volatile                             U32                  dac_path_cfg2; /*0x18 , RW   , 0x00000910, Audio DAC path config2        */
    volatile                             U32                       dac_ramp; /*0x1c , RW   , 0x00004503, Audio DAC Ramp Register       */
    volatile                             U32                  side_tone_cfg; /*0x20 , RW   , 0x0000001F, side tone config              */
    volatile                             U32                      test_ctrl; /*0x24 , RW   , 0x00000000, Audio Test Control Register   */
    volatile                             U32                   test_port_lo; /*0x28 , RO   , 0x00000000, Audio Test Ports Register     */
    volatile                             U32                   test_port_hi; /*0x2c , RO   , 0x00000000, Audio Test Ports Register     */
    volatile                             U32                        eq_ctrl; /*0x30 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef0_low; /*0x34 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef0_high; /*0x38 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef1_low; /*0x3c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef1_high; /*0x40 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef2_low; /*0x44 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef2_high; /*0x48 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef3_low; /*0x4c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef3_high; /*0x50 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef4_low; /*0x54 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef4_high; /*0x58 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef5_low; /*0x5c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef5_high; /*0x60 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef6_low; /*0x64 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef6_high; /*0x68 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef7_low; /*0x6c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef7_high; /*0x70 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef8_low; /*0x74 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef8_high; /*0x78 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                   eq_coef9_low; /*0x7c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef9_high; /*0x80 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef10_low; /*0x84 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef10_high; /*0x88 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef11_low; /*0x8c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef11_high; /*0x90 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef12_low; /*0x94 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef12_high; /*0x98 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef13_low; /*0x9c , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef13_high; /*0xa0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef14_low; /*0xa4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef14_high; /*0xa8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef15_low; /*0xac , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef15_high; /*0xb0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef16_low; /*0xb4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef16_high; /*0xb8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef17_low; /*0xbc , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef17_high; /*0xc0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef18_low; /*0xc4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef18_high; /*0xc8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef19_low; /*0xcc , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef19_high; /*0xd0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef20_low; /*0xd4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef20_high; /*0xd8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef21_low; /*0xdc , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef21_high; /*0xe0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef22_low; /*0xe4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef22_high; /*0xe8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef23_low; /*0xec , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef23_high; /*0xf0 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef24_low; /*0xf4 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef24_high; /*0xf8 , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef25_low; /*0xfc , RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef25_high; /*0x100, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef26_low; /*0x104, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef26_high; /*0x108, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef27_low; /*0x10c, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef27_high; /*0x110, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef28_low; /*0x114, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef28_high; /*0x118, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef29_low; /*0x11c, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef29_high; /*0x120, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef30_low; /*0x124, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef30_high; /*0x128, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef31_low; /*0x12c, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef31_high; /*0x130, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef32_low; /*0x134, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef32_high; /*0x138, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef33_low; /*0x13c, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef33_high; /*0x140, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                  eq_coef34_low; /*0x144, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                 eq_coef34_high; /*0x148, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                     coef_a1_lo; /*0x14c, RW   , 0x00000000, hpf_coef                      */
    volatile                             U32                     coef_a1_hi; /*0x150, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                     coef_a2_lo; /*0x154, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                     coef_a2_hi; /*0x158, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                     coef_b1_lo; /*0x15c, RW   , 0x00000000, EQ control Register           */
    volatile                             U32                     coef_b1_hi; /*0x160, RW   , 0x00000000, EQ control Register           */
} T_HWP_AUDIO_CODEC_T;

#define AUDIO_CODEC_FID(n)             (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_FID_MASK           (0xFFFF<<0)         
#define AUDIO_CODEC_FID_SHIFT          (0)                 


#define AUDIO_CODEC_S_ISPHONE          (1<<0)              

#define AUDIO_CODEC_S_PHONE_SEL        (1<<1)              

#define AUDIO_CODEC_S_LOOP             (1<<2)              

#define AUDIO_CODEC_S_DAC_1K           (1<<3)              

#define AUDIO_CODEC_FM_MODE            (1<<4)              

#define AUDIO_CODEC_SLEEP_MODE         (1<<5)              

#define AUDIO_CODEC_S_CODEC_EN         (1<<6)              


#define AUDIO_CODEC_ADC_CLK_LD         (1<<0)              

#define AUDIO_CODEC_CNT_ADC_CLK_INT(n) (((n)&0x7F)<<1)     
#define AUDIO_CODEC_CNT_ADC_CLK_INT_MASK (0x7F<<1)           
#define AUDIO_CODEC_CNT_ADC_CLK_INT_SHIFT (1)                 


#define AUDIO_CODEC_ADC_CLKGEN_MODE    (1<<0)              

#define AUDIO_CODEC_ANA_ADC_CLK_INV    (1<<1)              

#define AUDIO_CODEC_S_ADC_OSR_SEL(n)   (((n)&0x7)<<2)      
#define AUDIO_CODEC_S_ADC_OSR_SEL_MASK (0x7<<2)            
#define AUDIO_CODEC_S_ADC_OSR_SEL_SHIFT (2)                 

#define AUDIO_CODEC_ADC_LPF_BYPASS     (1<<5)              

#define AUDIO_CODEC_S_HPF_BYPASS       (1<<6)              

#define AUDIO_CODEC_S_ADC_VOLUME(n)    (((n)&0xF)<<7)      
#define AUDIO_CODEC_S_ADC_VOLUME_MASK  (0xF<<7)            
#define AUDIO_CODEC_S_ADC_VOLUME_SHIFT (7)                 

#define AUDIO_CODEC_ADC_DIG_SEL        (1<<11)             

#define AUDIO_CODEC_DIG_MIC_SWITCH     (1<<12)             

#define AUDIO_CODEC_ADC_PATH_RESET     (1<<13)             

#define AUDIO_CODEC_S_ADC_EN           (1<<14)             

#define AUDIO_CODEC_ADC_OUT_FORMAT     (1<<15)             


#define AUDIO_CODEC_DAC_CLK_LD         (1<<0)              

#define AUDIO_CODEC_FS_CNT_INT(n)      (((n)&0x3FF)<<1)    
#define AUDIO_CODEC_FS_CNT_INT_MASK    (0x3FF<<1)          
#define AUDIO_CODEC_FS_CNT_INT_SHIFT   (1)                 


#define AUDIO_CODEC_S_DAC_VOLUME_MUTE  (1<<0)              

#define AUDIO_CODEC_S_MUTE_R           (1<<1)              

#define AUDIO_CODEC_S_MUTE_L           (1<<2)              

#define AUDIO_CODEC_S_DAC_VOLUME(n)    (((n)&0x3F)<<3)     
#define AUDIO_CODEC_S_DAC_VOLUME_MASK  (0x3F<<3)           
#define AUDIO_CODEC_S_DAC_VOLUME_SHIFT (3)                 

#define AUDIO_CODEC_S_DAC_GAIN(n)      (((n)&0x3)<<9)      
#define AUDIO_CODEC_S_DAC_GAIN_MASK    (0x3<<9)            
#define AUDIO_CODEC_S_DAC_GAIN_SHIFT   (9)                 

#define AUDIO_CODEC_S_DAC_OSR_SEL(n)   (((n)&0x3)<<11)     
#define AUDIO_CODEC_S_DAC_OSR_SEL_MASK (0x3<<11)           
#define AUDIO_CODEC_S_DAC_OSR_SEL_SHIFT (11)                

#define AUDIO_CODEC_DAC_PATH_RESET     (1<<13)             

#define AUDIO_CODEC_S_DAC_EN           (1<<14)             


#define AUDIO_CODEC_S_FS_SEL(n)        (((n)&0x7)<<0)      
#define AUDIO_CODEC_S_FS_SEL_MASK      (0x7<<0)            
#define AUDIO_CODEC_S_FS_SEL_SHIFT     (0)                 

#define AUDIO_CODEC_SDM_GAIN(n)        (((n)&0x3)<<3)      
#define AUDIO_CODEC_SDM_GAIN_MASK      (0x3<<3)            
#define AUDIO_CODEC_SDM_GAIN_SHIFT     (3)                 

#define AUDIO_CODEC_DITHER_GAIN_2_0(n) (((n)&0x7)<<5)      
#define AUDIO_CODEC_DITHER_GAIN_2_0_MASK (0x7<<5)            
#define AUDIO_CODEC_DITHER_GAIN_2_0_SHIFT (5)                 

#define AUDIO_CODEC_DITHER_GAIN_4_3(n) (((n)&0x3)<<8)      
#define AUDIO_CODEC_DITHER_GAIN_4_3_MASK (0x3<<8)            
#define AUDIO_CODEC_DITHER_GAIN_4_3_SHIFT (8)                 

#define AUDIO_CODEC_S_DITHER_BYPASS    (1<<10)             

#define AUDIO_CODEC_S_DWA_EN           (1<<11)             

#define AUDIO_CODEC_DAC_CLKGEN_MODE    (1<<12)             

#define AUDIO_CODEC_ANA_DAC_CLK_INV    (1<<13)             


#define AUDIO_CODEC_RAMPDN_STEP(n)     (((n)&0x7F)<<0)     
#define AUDIO_CODEC_RAMPDN_STEP_MASK   (0x7F<<0)           
#define AUDIO_CODEC_RAMPDN_STEP_SHIFT  (0)                 

#define AUDIO_CODEC_RAMPUP_STEP(n)     (((n)&0x7F)<<7)     
#define AUDIO_CODEC_RAMPUP_STEP_MASK   (0x7F<<7)           
#define AUDIO_CODEC_RAMPUP_STEP_SHIFT  (7)                 

#define AUDIO_CODEC_RAMP_BYPASS        (1<<14)             

#define AUDIO_CODEC_RAMP_IND           (1<<15)             


#define AUDIO_CODEC_S_ADC_DVST_CTL_3_0(n) (((n)&0xF)<<0)      
#define AUDIO_CODEC_S_ADC_DVST_CTL_3_0_MASK (0xF<<0)            
#define AUDIO_CODEC_S_ADC_DVST_CTL_3_0_SHIFT (0)                 

#define AUDIO_CODEC_S_ADC_DVST_CTL_4   (1<<4)              


#define AUDIO_CODEC_TPORTS_SEL(n)      (((n)&0x1F)<<0)     
#define AUDIO_CODEC_TPORTS_SEL_MASK    (0x1F<<0)           
#define AUDIO_CODEC_TPORTS_SEL_SHIFT   (0)                 

#define AUDIO_CODEC_TPORTS_EN          (1<<5)              

#define AUDIO_CODEC_TPORTS_OUT_SEL     (1<<6)              


#define AUDIO_CODEC_TPORTS_LO(n)       (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_TPORTS_LO_MASK     (0xFFFF<<0)         
#define AUDIO_CODEC_TPORTS_LO_SHIFT    (0)                 


#define AUDIO_CODEC_TPORTS_HI(n)       (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_TPORTS_HI_MASK     (0xFFFF<<0)         
#define AUDIO_CODEC_TPORTS_HI_SHIFT    (0)                 


#define AUDIO_CODEC_EQ_RESET           (1<<0)              

#define AUDIO_CODEC_EQ_BYPASS          (1<<1)              


#define AUDIO_CODEC_EQ_COEF0_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF0_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF0_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF0_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF0_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF0_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF1_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF1_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF1_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF1_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF1_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF1_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF2_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF2_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF2_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF2_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF2_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF2_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF3_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF3_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF3_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF3_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF3_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF3_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF4_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF4_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF4_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF4_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF4_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF4_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF5_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF5_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF5_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF5_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF5_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF5_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF6_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF6_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF6_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF6_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF6_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF6_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF7_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF7_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF7_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF7_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF7_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF7_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF8_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF8_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF8_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF8_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF8_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF8_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF9_LO(n)     (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF9_LO_MASK   (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF9_LO_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF9_HI(n)     (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF9_HI_MASK   (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF9_HI_SHIFT  (0)                 


#define AUDIO_CODEC_EQ_COEF10_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF10_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF10_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF10_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF10_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF10_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF11_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF11_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF11_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF11_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF11_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF11_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF12_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF12_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF12_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF12_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF12_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF12_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF13_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF13_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF13_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF13_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF13_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF13_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF14_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF14_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF14_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF14_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF14_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF14_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF15_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF15_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF15_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF15_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF15_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF15_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF16_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF16_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF16_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF16_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF16_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF16_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF17_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF17_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF17_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF17_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF17_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF17_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF18_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF18_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF18_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF18_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF18_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF18_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF19_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF19_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF19_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF19_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF19_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF19_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF20_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF20_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF20_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF20_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF20_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF20_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF21_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF21_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF21_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF21_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF21_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF21_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF22_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF22_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF22_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF22_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF22_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF22_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF23_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF23_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF23_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF23_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF23_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF23_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF24_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF24_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF24_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF24_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF24_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF24_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF25_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF25_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF25_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF25_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF25_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF25_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF26_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF26_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF26_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF26_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF26_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF26_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF27_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF27_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF27_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF27_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF27_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF27_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF28_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF28_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF28_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF28_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF28_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF28_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF29_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF29_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF29_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF29_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF29_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF29_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF30_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF30_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF30_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF30_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF30_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF30_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF31_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF31_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF31_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF31_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF31_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF31_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF32_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF32_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF32_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF32_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF32_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF32_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF33_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF33_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF33_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF33_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF33_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF33_HI_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF34_LO(n)    (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_EQ_COEF34_LO_MASK  (0xFFFF<<0)         
#define AUDIO_CODEC_EQ_COEF34_LO_SHIFT (0)                 


#define AUDIO_CODEC_EQ_COEF34_HI(n)    (((n)&0xFF)<<0)     
#define AUDIO_CODEC_EQ_COEF34_HI_MASK  (0xFF<<0)           
#define AUDIO_CODEC_EQ_COEF34_HI_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_A1_LO(n)  (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_HPF_COEF_A1_LO_MASK (0xFFFF<<0)         
#define AUDIO_CODEC_HPF_COEF_A1_LO_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_A1_HI(n)  (((n)&0xF)<<0)      
#define AUDIO_CODEC_HPF_COEF_A1_HI_MASK (0xF<<0)            
#define AUDIO_CODEC_HPF_COEF_A1_HI_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_A2_LO(n)  (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_HPF_COEF_A2_LO_MASK (0xFFFF<<0)         
#define AUDIO_CODEC_HPF_COEF_A2_LO_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_A2_HI(n)  (((n)&0xF)<<0)      
#define AUDIO_CODEC_HPF_COEF_A2_HI_MASK (0xF<<0)            
#define AUDIO_CODEC_HPF_COEF_A2_HI_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_B1_LO(n)  (((n)&0xFFFF)<<0)   
#define AUDIO_CODEC_HPF_COEF_B1_LO_MASK (0xFFFF<<0)         
#define AUDIO_CODEC_HPF_COEF_B1_LO_SHIFT (0)                 


#define AUDIO_CODEC_HPF_COEF_B1_HI(n)  (((n)&0xF)<<0)      
#define AUDIO_CODEC_HPF_COEF_B1_HI_MASK (0xF<<0)            
#define AUDIO_CODEC_HPF_COEF_B1_HI_SHIFT (0)                 


#endif

#endif


