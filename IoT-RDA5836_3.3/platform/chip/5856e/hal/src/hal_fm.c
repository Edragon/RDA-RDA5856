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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_fm.c                                                    //
/// Implementation of HAL initialization related with the particular instance
/// of the IP.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "cfg_regs.h"
#include "halp_debug.h"
#include "halp_sys.h"
#include "hal_sys.h"
#include "rf_if.h"
#include "sxr_tls.h"

// DATAPATH_CTRL3 interval
const UINT16 g_halFMDsp_Datapath_Ctrl3_Interval = 0x2eff;

// DATAPATH_CTRL3 step19
const UINT16 g_halFMDsp_Datapath_Ctrl3_Step19 = 0x3A5E;

// NOISE_CANCEL_CTRL1
const UINT8 g_halFMDsp_Noise_Cancel_Ctrl3_Fircut = 4;

// DATAPATH_CTRL0 gainct
const UINT8 g_halFMDsp_Datapath_Ctrl0_Gainct = 0x80;

//AFC_CTRL afc_ct_sk
const UINT8 g_halFMDsp_Afc_Ctrl_Afc_Ct_Sk = 0x3F;

//AFC_CTRL afc_range
const UINT8 g_halFMDsp_Afc_Ctrl_Afc_Range = 0x32;

//AFC_CTRL noise_cancel_ctrl2
const UINT16 g_halFMDsp_Noise_Cancel_Ctrl2 = 0x7FFF;

//DATAPATH_CTRL1  SOFTMUTE RSSI
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th1 = 0x7;

//DATAPATH_CTRL1  SOFTMUTE NIOSE
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th2 = 0x5;

//DATAPATH_CTRL1  SOFTMUTE SNR
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th3 = 0xd;

//DATAPATH_CTRL2  SOFTMUTE offset
const UINT8 g_halFMDsp_Datapath_Ctrl2_SoftMute_Th4 = 0x32;

//DATAPATH_CTRL2  SmRate
const UINT8 g_halFMDsp_Datapath_Ctrl2_SmRate = 0;

//DATAPATH_CTRL2  SmAttenu
const UINT8 g_halFMDsp_Datapath_Ctrl2_SmAttenu = 0xf;

//DATAPATH_CTRL2  Thsoftblend1
const UINT8 g_halFMDsp_Datapath_Ctrl2_ThSoftBlend1 = 0;

//DATAPATH_CTRL2  Thsoftblend2
const UINT8 g_halFMDsp_Datapath_Ctrl2_ThSoftBlend2 = 0x32;

//DATAPATH_CTRL2  deemph hcc
const UINT8 g_halFMDsp_Datapath_Ctrl2_Deemph_Hcc = 0;

//DATAPATH_CTRL2  Lpf_bw
const UINT8 g_halFMDsp_Datapath_Ctrl2_Lpf_Bw = 0x5;

//CTRL  DeemphMode
const UINT8 g_halFMDsp_Ctrl_DeemphMode = FM_DSP_DEEMPH_MODE_75US;

void hal_Fm_Dsp_Start(void) 
{
    uint8 i=0;
	
   
    hwp_rf_if->fm_dsp_setting  |= RF_IF_FM_DSP_RESETN_DR;
    hwp_rf_if->fm_dsp_setting  &= ~RF_IF_FM_DSP_RESETN_REG;
//    delay(10);
    for(i=0;i<20;i++);
    hwp_rf_if->fm_dsp_setting |= RF_IF_FM_DSP_RESETN_REG;
}

void hal_Fm_Dsp_Enable_IQSwitchBeforeMixer(BOOL on) 
{

    if(on)
        hwp_fm_dsp->ctrl |= FM_DSP_IMGREJ_DSP;
    else
	hwp_fm_dsp->ctrl &= ~FM_DSP_IMGREJ_DSP;
}


void hal_Fm_Dsp_Disable_AFC(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |=  FM_DSP_AFC_DISABLE_DISABLE;	
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_AFC_DISABLE;
	 hwp_fm_dsp->ctrl |=  FM_DSP_AFC_DISABLE_ENABLE;
    }
}

void hal_Fm_Dsp_Enable_SoftMute(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTMUTE_EN_ENABLE;
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_SOFTMUTE_EN;
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTMUTE_EN_DISABLE;
    }
}

void hal_Fm_Dsp_Enable_Softblend(bool on)
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_SOFTBLEND_OFF;
	 hwp_fm_dsp->ctrl |= FM_DSP_SOFTBLEND_OFF_ENABLE;			
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTBLEND_OFF_DISABLE;
    }
}

void hal_Fm_Dsp_Enable_Mute(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_MUTE_MUTE;
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_MUTE;
        hwp_fm_dsp->ctrl |= FM_DSP_MUTE_NORMAL;
    }
}

void hal_FM_DSP_Deemph_Bypass(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_DEEMPH_BYPASS;
        
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_DEEMPH_BYPASS;
    }
}

void hal_FM_DSP_LPF_Bypass(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_LPF_BYPASS;
        
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_LPF_BYPASS;
    }
}

void hal_FM_DSP_Enable_I2s(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_I2S_EN_ENABLE;
        
    }
    else
    {
        hwp_fm_dsp->ctrl &= FM_DSP_I2S_EN;
	 hwp_fm_dsp->ctrl |= FM_DSP_I2S_EN_DISABLE;
    }
}

void hal_FM_DSP_Fmdem_Sel(bool sel) 
{
    if(sel)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_FMDEM_SRC_SEL_OUTPUT_DIG_GAIN;
        
    }
    else
    {
        hwp_fm_dsp->ctrl &= FM_DSP_FMDEM_SRC_SEL;
	 hwp_fm_dsp->ctrl |= FM_DSP_FMDEM_SRC_SEL_OUTPUT_LPFIL;
    }
}

void hal_FM_DSP_Set_All_SelFircut(uint8_t val) 
{
    hwp_fm_dsp->noise_cancel_ctrl1 &= ~(FM_DSP_SEL_FIRCUT_SK_MASK|FM_DSP_SEL_FIRCUT3_MASK|FM_DSP_SEL_FIRCUT2_MASK|FM_DSP_SEL_FIRCUT1_MASK);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT_SK(val*0x9);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT3(val);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT2(val);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT1(val);
}

#if 0
void hal_Fm_Dsp_Seek_Ctrl_Init(void)
{
    hwp_fm_dsp->seek_ctrl0 &= ~FM_DSP_SK_CMP_GRP_EN_MASK;
    hwp_fm_dsp->seek_ctrl0 |= FM_DSP_SK_CMP_GRP_EN(26);
    hwp_fm_dsp->seek_ctrl2 &= ~(FM_DSP_RSSI_H_TH_MASK|FM_DSP_RSSI_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl2 |= FM_DSP_RSSI_H_TH(57)|FM_DSP_RSSI_L_TH(52) ;
    hwp_fm_dsp->seek_ctrl3 &= ~(FM_DSP_OFFSET_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl3 |= FM_DSP_OFFSET_L_TH(20);
}
#else
void hal_Fm_Dsp_Seek_Ctrl_Init(void)
{
    hwp_fm_dsp->seek_ctrl0 &= ~FM_DSP_SK_CMP_GRP_EN_MASK;
    hwp_fm_dsp->seek_ctrl0 |= FM_DSP_SK_CMP_GRP_EN(10);
    hwp_fm_dsp->seek_ctrl1 &= ~FM_DSP_SNR_CNT_TH_MASK;
    hwp_fm_dsp->seek_ctrl1 |= FM_DSP_SNR_CNT_TH(30);
    hwp_fm_dsp->seek_ctrl2 &= ~(FM_DSP_RSSI_H_TH_MASK|FM_DSP_RSSI_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl2 |= FM_DSP_RSSI_H_TH(75)|FM_DSP_RSSI_L_TH(72) ;
    hwp_fm_dsp->seek_ctrl3 &= ~(FM_DSP_OFFSET_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl3 |= FM_DSP_OFFSET_L_TH(8) | FM_DSP_OFFSET_H_TH(13);
    hwp_fm_dsp->seek_ctrl4 &= ~(FM_DSP_SK_TIMER1_MASK|FM_DSP_SK_TIMER2_MASK|FM_DSP_SK_TIMER3_MASK);
    hwp_fm_dsp->seek_ctrl4 |= FM_DSP_SK_TIMER1(15)|FM_DSP_SK_TIMER2(32);//|FM_DSP_SK_TIMER3(32);
}
#endif

void hal_Fm_Dsp_Datapath_Init(void)
{
    //notchfit flt bypass
    hwp_fm_dsp->datapath_ctrl4 |= FM_DSP_NOTCH_FLT_BYPASS;

    //diable irq switch before mixer 
    hal_Fm_Dsp_Enable_IQSwitchBeforeMixer(FALSE);

    //disable mute
    hal_Fm_Dsp_Enable_Mute(FALSE);

    //disable softmute
    hal_Fm_Dsp_Enable_SoftMute(FALSE);

    //enable afc
    hal_Fm_Dsp_Disable_AFC(FALSE);

    //deemph bypass
    hal_FM_DSP_Deemph_Bypass(TRUE);

    //disable blend
    hal_Fm_Dsp_Enable_Softblend(TRUE);

   //lpf bypass
    hal_FM_DSP_LPF_Bypass(FALSE);
   
    //noise src sel
    hwp_fm_dsp->ctrl &= ~FM_DSP_NOISE_SRC_SEL_MASK;
    hwp_fm_dsp->ctrl |= FM_DSP_NOISE_SRC_SEL_DANGLE0;
		
    //dc cancel control
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_DC_CT_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_DC_CT(0xc);

    //sin  limit gain 
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_SINC_LIMIT_GAIN_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_SINC_LIMIT_GAIN_6DB;

    //limit sel =1
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_LIMIT_SEL;

    //over threshold selection
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_OVER_TH_SEL_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_OVER_TH_SEL_3;

    //gainct
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_GAINCT_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_GAINCT(g_halFMDsp_Datapath_Ctrl0_Gainct);

    //noise_cancel_ctrl1 fircut	
     hwp_fm_dsp->noise_cancel_ctrl1 = FM_DSP_SEL_FIRCUT_SK(9) |FM_DSP_SEL_FIRCUT3(5) |FM_DSP_SEL_FIRCUT2(5) | FM_DSP_SEL_FIRCUT1(5)|FM_DSP_TH_MIN_HCC(32);
    
    //output dig gain
    hal_FM_DSP_Fmdem_Sel(1);

    //noise_cancel_ctrl0 fmdem sel grp
    hwp_fm_dsp->noise_cancel_ctrl0 &= ~FM_DSP_FMDEM_SEL_GRP_MASK;
    hwp_fm_dsp->noise_cancel_ctrl0 |= FM_DSP_FMDEM_SEL_GRP(0);

    //afc_ctrl afc_ct
    hwp_fm_dsp->afc_ctrl &= ~(FM_DSP_AFC_CT_MASK|FM_DSP_AFC_CT_SK_MASK|FM_DSP_AFC_INV|FM_DSP_AFC_RANGE_MASK);
    hwp_fm_dsp->afc_ctrl |= FM_DSP_AFC_CT(0) |FM_DSP_AFC_CT_SK(g_halFMDsp_Afc_Ctrl_Afc_Ct_Sk) | FM_DSP_AFC_RANGE(g_halFMDsp_Afc_Ctrl_Afc_Range);

    //noise_cancel_ctrl2
    hwp_fm_dsp->noise_cancel_ctrl2 &= ~(FM_DSP_GAIN_38K1_MASK|FM_DSP_GAIN_38K2_MASK);
    hwp_fm_dsp->noise_cancel_ctrl2 |= FM_DSP_GAIN_38K1(g_halFMDsp_Noise_Cancel_Ctrl2) | FM_DSP_GAIN_38K2(g_halFMDsp_Noise_Cancel_Ctrl2);

    //datapath_ctrl1           nco_ct, limit gain, softmute rssi,noise,snr
    hwp_fm_dsp->datapath_ctrl1 = 0;
    hwp_fm_dsp->datapath_ctrl1 |= FM_DSP_NCO_CT_U_DT(0x4) | FM_DSP_SOFTMUTE_TH1(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th1) |FM_DSP_SOFTMUTE_TH2(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th2) | FM_DSP_SOFTMUTE_TH3(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th3);

    //datapath_ctrl2  softmute_th4
    hwp_fm_dsp->datapath_ctrl2 &= ~(FM_DSP_SOFTMUTE_TH4_MASK|FM_DSP_SM_RATE_MASK|FM_DSP_SM_ATTENU_MASK|FM_DSP_TH_SOFTBLEND1_MASK|FM_DSP_TH_SOFTBLEND2_MASK|FM_DSP_DEEMPH_HCC_MASK|FM_DSP_LPF_BW_REG_MASK);
    hwp_fm_dsp->datapath_ctrl2 |= FM_DSP_SOFTMUTE_TH4(g_halFMDsp_Datapath_Ctrl2_SoftMute_Th4) | FM_DSP_SM_RATE(g_halFMDsp_Datapath_Ctrl2_SmRate) | FM_DSP_SM_ATTENU(g_halFMDsp_Datapath_Ctrl2_SmAttenu) | FM_DSP_TH_SOFTBLEND1(g_halFMDsp_Datapath_Ctrl2_ThSoftBlend1) | FM_DSP_TH_SOFTBLEND2(g_halFMDsp_Datapath_Ctrl2_ThSoftBlend2)|FM_DSP_DEEMPH_HCC(0)|FM_DSP_LPF_BW_DR|FM_DSP_LPF_BW_REG(g_halFMDsp_Datapath_Ctrl2_Lpf_Bw);

    //datapath0    limit_sel  sin_limit gain 
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_SINC_LIMIT_GAIN_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_SINC_LIMIT_GAIN_6DB|FM_DSP_LIMIT_SEL;

    //over threshold selection
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_OVER_TH_SEL_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_OVER_TH_SEL_3;

    //ctrl  DeemphMode
    hwp_fm_dsp->ctrl &= ~(FM_DSP_DEEMPH_MODE | FM_DSP_NOISE_SRC_SEL_MASK);
    hwp_fm_dsp->ctrl |= g_halFMDsp_Ctrl_DeemphMode | FM_DSP_NOISE_SRC_SEL(2);

    hwp_fm_dsp->ana_gain_rssi_tb0  = 0xfaf4f4f4;

    hal_Fm_Dsp_Seek_Ctrl_Init();
    
}

