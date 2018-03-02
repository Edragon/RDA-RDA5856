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
/// @file hal_abb_config.c                                                    //
/// Implementation of HAL initialization related with the particular instance
/// of the IP.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "cfg_regs.h"

#include "halp_rda_abb.h"
#include "halp_debug.h"
#include "halp_sys.h"
#include "hal_sys.h"
#include "hal_rda_audio.h"

#include "aud_m.h"
#include "sxr_tls.h"
#include "abb.h"
#include "audio_codec.h"
#include "tgt_app_cfg.h"
#include "cos.h"
#include "hal_host.h"
// =============================================================================
// EXTERNS                                                                       
// =============================================================================

extern VOID pmd_SpeakerPaCpLowOutput(BOOL low);
#ifdef ABB_HP_DETECT
extern VOID pmd_StartHpOutDetect(BOOL start);
#endif

extern PUBLIC VOID hal_AudEnableAdc(BOOL on);
// =============================================================================
//  MACROS
// =============================================================================

// Whether to enable DSSI to improve sound quality during silence
// 0: disable DSSI all the time
// 1: only enable DSSI in music mode
// 2: enable DSSI both voice and music mode
#define AUD_SPK_DSSI_DETECT 0

#define AUD_DEFAULT_ANALOG_GAIN     2

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_AUD_SAMPLE_RATE_INDEX_T
// -----------------------------------------------------------------------------
/// This type list the index of supported sample rates.
// =============================================================================
typedef enum
{
    HAL_AUD_SAMPLE_RATE_INDEX_8K,
    HAL_AUD_SAMPLE_RATE_INDEX_11_025K,
    HAL_AUD_SAMPLE_RATE_INDEX_12K,
    HAL_AUD_SAMPLE_RATE_INDEX_16K,
    HAL_AUD_SAMPLE_RATE_INDEX_22_05K,
    HAL_AUD_SAMPLE_RATE_INDEX_24K,
    HAL_AUD_SAMPLE_RATE_INDEX_32K,
    HAL_AUD_SAMPLE_RATE_INDEX_44_1K,
    HAL_AUD_SAMPLE_RATE_INDEX_48K,

    HAL_AUD_SAMPLE_RATE_INDEX_44103,
    HAL_AUD_SAMPLE_RATE_INDEX_44102,
    HAL_AUD_SAMPLE_RATE_INDEX_44101,
    HAL_AUD_SAMPLE_RATE_INDEX_44099,
    HAL_AUD_SAMPLE_RATE_INDEX_44098,
    HAL_AUD_SAMPLE_RATE_INDEX_44097,
    HAL_AUD_SAMPLE_RATE_INDEX_44096,
    HAL_AUD_SAMPLE_RATE_INDEX_44095,
    HAL_AUD_SAMPLE_RATE_INDEX_44094,
    HAL_AUD_SAMPLE_RATE_INDEX_44093,
    HAL_AUD_SAMPLE_RATE_INDEX_44092,
    HAL_AUD_SAMPLE_RATE_INDEX_44091,
    HAL_AUD_SAMPLE_RATE_INDEX_44090,
    
    HAL_AUD_SAMPLE_RATE_INDEX_48004,
    HAL_AUD_SAMPLE_RATE_INDEX_48003,
    HAL_AUD_SAMPLE_RATE_INDEX_48002,
    HAL_AUD_SAMPLE_RATE_INDEX_48001,
    HAL_AUD_SAMPLE_RATE_INDEX_47999,
    HAL_AUD_SAMPLE_RATE_INDEX_47998,
    HAL_AUD_SAMPLE_RATE_INDEX_47997,
    HAL_AUD_SAMPLE_RATE_INDEX_47996,
    HAL_AUD_SAMPLE_RATE_INDEX_47995,

    HAL_AUD_SAMPLE_RATE_INDEX_QTY,
} HAL_AUD_SAMPLE_RATE_INDEX_T;

// =============================================================================
// HAL_AUD_SDM2_FREQ_REG_T
// -----------------------------------------------------------------------------
/// This type describes the register values to set in SDM when 
/// changing audio codec frequency for a sample rate.
// =============================================================================
typedef struct
{
    UINT16 sdm2FreqHigh;
    UINT16 sdm2FreqLow;
} HAL_AUD_SDM2_FREQ_REG_T;



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)


PRIVATE CONST HAL_AUD_SDM2_FREQ_REG_T
    gc_halAudSdm2FreqRegs[HAL_AUD_SAMPLE_RATE_INDEX_QTY] =
{
    {0xF1FA, 0xA4C9}, // 8K
    {0xDE51, 0x7A98}, // 11.025K
    {0xF1FA, 0xA4C9}, // 12K
    {0xF1FA, 0xA4C9}, // 16K
    {0xDE51, 0x7A98}, // 22.05K
    {0xF1FA, 0xA4C9}, // 24K
    {0xF1FA, 0xA4C9}, // 32K
    {0xDE51, 0x7A98},// 44.1K
    {0xF1FA, 0xA4C9}, // 48K
    {0xDE55, 0x59BE}, //44103
    {0xDE54, 0xF5C}, //44102
    {0xDE52, 0xC4FA}, //44101
    {0xDE50, 0x3036}, //44099 
    {0xDE4E, 0xE5D5}, //44098
    {0xDE4D, 0x9B73}, //44097
    {0xDE4C, 0x5111}, //44096
    {0xDE4B, 0x6AF}, //44095
    {0xDE49, 0xBC4D}, //44094
    {0xDE48, 0x71EB}, //44093
    {0xDE47, 0x278A}, //44092
    {0xDE45, 0xDD28}, //44091
    {0xDE44, 0x92C6}, //44090
    
    {0xF1FF, 0xCE50}, //48004
    {0xF1FE, 0x83EE}, //48003
    {0xF1FD, 0x398C}, //48002
    {0xF1FB, 0xEF2A}, //48001
    {0xF1F9, 0x5A67}, //47999
    {0xF1F8, 0x1005}, //47998
    {0xF1F6, 0xC5A3}, //47997
    {0xF1F5, 0x7B41}, //47996
    {0xF1F4, 0x20DF}, //47995
};

const uint8 gc_halAudAdcSRDivCntInteger[HAL_AUD_SAMPLE_RATE_INDEX_QTY+1] =
{
    12, // 8K
    8, // 11.025K
    8, // 12K
    6, // 16K
    4, // 22.05K
    4, // 24K
    3, // 32K
    1,// 44.1K
    1, // 48K
    1 //fm
};

 const uint8 gc_halAudDacSRDivCntInteger[HAL_AUD_SAMPLE_RATE_INDEX_QTY+1] =
{
    3, // 8K
    4, // 11.025K
    4, // 12K
    3, // 16K
    2, // 22.05K
    2, // 24K
    3, // 32K
    1,// 44.1K
    1, // 48K
    1 //fm
};

PRIVATE CONST UINT16
    gc_halAudAdcDiv1[HAL_AUD_SAMPLE_RATE_INDEX_QTY] =
{
    0x0582, // 8K
    0x02C2, // 11.025K
    0xC382, // 12K
    0x02C2, // 16K
    0x0A6C, // 22.05K
    0x61C2, // 24K
    0x8142, // 32K
    0x0102, // 44.1K
    0xA0C2, // 48K
};

PRIVATE CONST UINT16
    gc_halAudAdcDiv2[HAL_AUD_SAMPLE_RATE_INDEX_QTY] =
{
    0x0602, // 8K
    0x6442, // 11.025K
    0x0402, // 12K
    0x0302, // 16K
    0xC202, // 22.05K
    0x0202, // 24K
    0x0182, // 32K
    0x6102, // 44.1K
    0x0102, // 48K
};

#endif // !_FLASH_PROGRAMMER && !_T_UPGRADE_PROGRAMMER

const uint16 gc_halAudDcOffsetCount[HAL_AUD_SAMPLE_RATE_INDEX_QTY+1] =
{
    80, // 8K
    110, // 11.025K
    120, // 12K
    160, // 16K
    220, // 22.05K
    240, // 24K
    320, // 32K
    440,// 44.1K
    480, // 48K
    1 //fm
};
  
//When aux pll is on, system pll can be powerdown and use aux pll to save power
PRIVATE BOOL HAL_BOOT_DATA_INTERNAL g_halSysAuxPllIsOn = TRUE;
PRIVATE BOOL g_halSysabbInitDone = FALSE;
PRIVATE BOOL g_halSysabbOpenWaitMode = TRUE;

VOID hal_abbSetPaOpenMode(BOOL on)
{
	g_halSysabbOpenWaitMode = on;
} 
VOID hal_abbPdAll(BOOL on)
{
} 

static volatile UINT8 g_abb_pu_timer,g_abb_pustep=0;
static volatile UINT8 g_abb_pd_timer,g_abb_pdstep=0;

void hal_abb_pd_timeout(void* param)
{
    
     if(g_abb_pdstep == 0)
     {
         if(g_abb_pd_timer)
	 {
	   	COS_KillTimer(g_abb_pd_timer);
	       g_abb_pd_timer = 0;
        }
        if(g_abb_pu_timer)
	 {
		COS_KillTimer(g_abb_pu_timer);  
		g_abb_pu_timer = 0;
        }

        hwp_abb->denoise_other_cfg = ABB_HP_DISCHG_RES_BIT(3);
         hwp_abb->abb_rsvd_cfg = 0;
		   g_abb_pdstep =1;
	   g_abb_pd_timer = COS_SetTimer(20, hal_abb_pd_timeout, NULL, COS_TIMER_MODE_PERIODIC);
     }
     else if(g_abb_pdstep ==1)
     {
	    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(1);
	    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(1);
	    hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
	    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
           g_abb_pdstep = 2;
           COS_ChangeTimer(g_abb_pd_timer, 100);
     }
    else if(g_abb_pdstep ==2) 
    {
           hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1);
	    hwp_abb->denoise_elimination_dac_clk_cfg = ABB_DENOISE_DAC_CLK_DIV(54) | ABB_DENOISE_DAC_CLK_LD;
	    hwp_abb->denoise_adc_clk_cfg = ABB_DENOISE_ADC_CLK_DIV(100) | ABB_DENOISE_ADC_CLK_LD;
	    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(2048);
	    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(2048);
	    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
	    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS | ABB_HP_DAC_CHG_IBIT(15);
#else
	    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS | ABB_HP_DAC_CHG_IBIT(15);
#endif
        g_abb_pdstep = 3;
	  COS_ChangeTimer(g_abb_pd_timer,20);
    }
    else if(g_abb_pdstep ==3) 
    {
#if defined(__CT_CHIP_PKG_RDA5836TE__)
	    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(15)|ABB_PU_HP_L_DAC_CHG|ABB_PU_ADC_DET_L;
#else
	    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(15)|ABB_PU_HP_L_DAC_CHG|ABB_PU_HP_R_DAC_CHG|ABB_PU_ADC_DET_L|ABB_PU_ADC_DET_R;
#endif
           g_abb_pdstep = 4;
            COS_ChangeTimer(g_abb_pd_timer,20);
    }
    else if (g_abb_pdstep == 4)
   {
    hwp_abb->abb_rsvd_status = 0XA00;
    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
	 g_abb_pdstep = 5;
            COS_ChangeTimer(g_abb_pd_timer,200);
    }
    else if(g_abb_pdstep == 5)
    {
	    hwp_abb->abb_rsvd_status = 0;
	    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
	    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(0);
	    hwp_abb->denoise_other_cfg = 0x3c;
    COS_KillTimer(g_abb_pd_timer);
	g_abb_pd_timer = 0;
    g_abb_pdstep =0;
    }
}

void hal_abb_pu_timeout(void* param)
{
     if(g_abb_pustep == 0)
     {
        if(g_abb_pd_timer)
	 {
	   	COS_KillTimer(g_abb_pd_timer);
	       g_abb_pd_timer = 0;
        }
        if(g_abb_pu_timer)
	 {
		COS_KillTimer(g_abb_pu_timer);  
		g_abb_pu_timer = 0;
        }

       hwp_abb->denoise_other_cfg = 0x3c;
	   hwp_abb->denoise_other_cfg = ABB_HP_DISCHG_RES_BIT(3);
	   hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(1);
	   hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(1);
	   hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
	   hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
	   g_abb_pustep = 1;
	   g_abb_pu_timer = COS_SetTimer(100, hal_abb_pu_timeout, NULL, COS_TIMER_MODE_PERIODIC);
     }
     else if(g_abb_pustep ==1)
     {
           hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1);
	    hwp_abb->denoise_elimination_dac_clk_cfg = ABB_DENOISE_DAC_CLK_DIV(54) | ABB_DENOISE_DAC_CLK_LD;
	    hwp_abb->denoise_adc_clk_cfg = ABB_DENOISE_ADC_CLK_DIV(100) | ABB_DENOISE_ADC_CLK_LD;
	    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(2713);
	    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(2713);
	    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
	    hwp_abb->abb_rsvd_cfg |= HP_R_SOFT_START_EN;
	    hwp_abb->denoise_pwr_cfg = ABB_HP_L_SOFT_START_EN | ABB_HP_DAC_CHG_IBIT(1);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_BIAS;
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_ADC_DET_L;
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_DAC_CHG;
#else
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS;
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_ADC_DET_R|ABB_PU_ADC_DET_L;
	    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_R_DAC_CHG|ABB_PU_HP_L_DAC_CHG;
#endif
           g_abb_pustep = 2;
           COS_ChangeTimer(g_abb_pu_timer, 20);
     }
    else if(g_abb_pustep ==2) 
    {
         hwp_abb->abb_rsvd_status = 0X1400;
         hwp_abb->denoise_ramp_cfg_l =  ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
         hwp_abb->denoise_ramp_cfg_r =  ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
         g_abb_pustep = 3;
	  COS_ChangeTimer(g_abb_pu_timer,200);
    }
    else if(g_abb_pustep ==3) 
    {
#if defined(__CT_CHIP_PKG_RDA5836TE__)
	    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(1) |ABB_HP_L_SOFT_START_EN | ABB_PU_HP_L_BIAS |ABB_PU_HP_L_MPATH;	
#else
	    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(1) |ABB_HP_L_SOFT_START_EN | ABB_PU_HP_L_BIAS | ABB_PU_HP_R_BIAS |ABB_PU_HP_L_MPATH |ABB_PU_HP_R_MPATH;
#endif
	    hwp_abb->abb_rsvd_status = 0;
	    hwp_abb->denoise_ramp_cfg_l =  ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
	    hwp_abb->denoise_ramp_cfg_r =  ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);

#if ( APP_SUPPORT_DIF_DAC == 0)
	    hwp_abb->abb_rsvd_cfg = 0;
#else
	    hwp_abb->abb_rsvd_cfg &= HP_R_SOFT_START_EN;
#endif
	    hwp_abb->denoise_pwr_cfg &= ~(ABB_HP_L_SOFT_START_EN |ABB_HP_DAC_CHG_IBIT_MASK);
           COS_KillTimer(g_abb_pu_timer);
	    g_abb_pu_timer = 0;
	    g_abb_pustep =0;
    }
}

VOID hal_abbPdHp(void)
{
    g_abb_pdstep = 0;
    hal_abb_pd_timeout(NULL);
}

VOID hal_abbPuHp(void)
{  
    g_abb_pustep = 0;
    hal_abb_pu_timeout(NULL);
}

VOID hal_abbWaitPuHp(void)
{  
   hwp_abb->denoise_other_cfg = 0x3c;
   hwp_abb->denoise_other_cfg = ABB_HP_DISCHG_RES_BIT(3);
    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(1);
    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(1);
    hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
    hal_TimDelay(100 MS_WAITING);
    hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1);
    hwp_abb->denoise_elimination_dac_clk_cfg = ABB_DENOISE_DAC_CLK_DIV(54) | ABB_DENOISE_DAC_CLK_LD;
    hwp_abb->denoise_adc_clk_cfg = ABB_DENOISE_ADC_CLK_DIV(100) | ABB_DENOISE_ADC_CLK_LD;
    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(2713);
    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(2713);
    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
    hwp_abb->abb_rsvd_cfg |= HP_R_SOFT_START_EN;
    hwp_abb->denoise_pwr_cfg = ABB_HP_L_SOFT_START_EN | ABB_HP_DAC_CHG_IBIT(1);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_BIAS;
    hwp_abb->denoise_pwr_cfg |= ABB_PU_ADC_DET_L;
    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_DAC_CHG;
#else
    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS;
    hwp_abb->denoise_pwr_cfg |= ABB_PU_ADC_DET_R|ABB_PU_ADC_DET_L;
    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_R_DAC_CHG|ABB_PU_HP_L_DAC_CHG;
#endif
    hal_TimDelay(20 MS_WAITING);
    hwp_abb->abb_rsvd_status = 0X1400;
    hwp_abb->denoise_ramp_cfg_l =  ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
    hwp_abb->denoise_ramp_cfg_r =  ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
    hal_TimDelay(150 MS_WAITING); 
#if defined(__CT_CHIP_PKG_RDA5836TE__)
    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(1) |ABB_HP_L_SOFT_START_EN | ABB_PU_HP_L_BIAS |ABB_PU_HP_L_MPATH; 
#else
    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(1) |ABB_HP_L_SOFT_START_EN | ABB_PU_HP_L_BIAS | ABB_PU_HP_R_BIAS |ABB_PU_HP_L_MPATH |ABB_PU_HP_R_MPATH;
#endif
    hwp_abb->abb_rsvd_status = 0;
    hwp_abb->denoise_ramp_cfg_l =  ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r =  ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
 
#if ( APP_SUPPORT_DIF_DAC == 0)
    hwp_abb->abb_rsvd_cfg = 0;
#else
    hwp_abb->abb_rsvd_cfg &= HP_R_SOFT_START_EN;
#endif
    hwp_abb->denoise_pwr_cfg &= ~(ABB_HP_L_SOFT_START_EN |ABB_HP_DAC_CHG_IBIT_MASK);
}

VOID hal_abbWaitPdHp(void)
{
    hwp_abb->denoise_other_cfg = ABB_HP_DISCHG_RES_BIT(3);
    hwp_abb->abb_rsvd_cfg = 0;
    hal_TimDelay(20 MS_WAITING);
    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(1);
    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(1);
    hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
    hal_TimDelay(100 MS_WAITING);
    hwp_abb->denoise_ramp_cfg_l = ABB_RAMP_DONE_L|ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r = ABB_RAMP_DONE_R|ABB_STABLE_TIME_R(1);
    hwp_abb->denoise_elimination_dac_clk_cfg = ABB_DENOISE_DAC_CLK_DIV(54) | ABB_DENOISE_DAC_CLK_LD;
    hwp_abb->denoise_adc_clk_cfg = ABB_DENOISE_ADC_CLK_DIV(100) | ABB_DENOISE_ADC_CLK_LD;
    hwp_abb->denoise_rampup_cfg_l = ABB_RAMPUP_TH_VALUE_L(2048);
    hwp_abb->denoise_rampup_cfg_r = ABB_RAMPUP_TH_VALUE_R(2048);
    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS | ABB_HP_DAC_CHG_IBIT(15);
#else
    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS | ABB_HP_DAC_CHG_IBIT(15);
#endif
 
    hal_TimDelay(20 MS_WAITING);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(15)|ABB_PU_HP_L_DAC_CHG|ABB_PU_ADC_DET_L;
#else
    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(15)|ABB_PU_HP_L_DAC_CHG|ABB_PU_HP_R_DAC_CHG|ABB_PU_ADC_DET_L|ABB_PU_ADC_DET_R;
#endif
    
    hal_TimDelay(20 MS_WAITING);
    hwp_abb->abb_rsvd_status = 0XA00;
    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1)|ABB_START_DENOISE_L;
    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1)|ABB_START_DENOISE_R;
    hal_TimDelay(200 MS_WAITING);
    hwp_abb->abb_rsvd_status = 0;
    hwp_abb->denoise_ramp_cfg_l = ABB_ADC_DET_TH_L(4) |ABB_RAMPUP_CYCLE_L(7) |ABB_STABLE_TIME_L(1);
    hwp_abb->denoise_ramp_cfg_r = ABB_ADC_DET_TH_R(4) |ABB_RAMPUP_CYCLE_R(7) |ABB_STABLE_TIME_R(1);
    hwp_abb->denoise_pwr_cfg = ABB_HP_DAC_CHG_IBIT(0);
    hwp_abb->denoise_other_cfg = 0x3c;
 
}

#if defined(APP_SUPPORT_DCOFFSET) && (APP_SUPPORT_DCOFFSET == 1)
VOID hal_abbEnablePa(BOOL on)
{

 if(on == TRUE)
 {
#if ( APP_SUPPORT_DIF_DAC == 0)
     {
         if(g_halSysabbOpenWaitMode == TRUE)
         {
         	hal_abbWaitPuHp();
         }
         else
         {
              hal_abbPuHp();
         }
     }
#else
    {
                  hwp_abb->hp_chg_cfg &= ~ABB_HP_L_CHG_RES_BIT_MASK;
#if defined(__CT_CHIP_PKG_RDA5836TE__)
		    hwp_abb->hp_chg_cfg |= ABB_HP_CHG_RES_EN|ABB_HP_L_CHG_RES_BIT(3)|HP_L_CHG_RES_NPART_EN;
#else
                  hwp_abb->hp_chg_cfg |= ABB_HP_CHG_RES_EN|ABB_HP_L_CHG_RES_BIT(3)|HP_L_CHG_RES_NPART_EN|HP_R_CHG_RES_NAPRT_EN;
#endif

                 hal_TimDelay(2);

#if defined(__CT_CHIP_PKG_RDA5836TE__)
		    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS ;
#else
		    hwp_abb->denoise_pwr_cfg = ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS ;
#endif

#if defined(__CT_CHIP_PKG_RDA5836TE__)
		    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_MPATH;
#else
		    hwp_abb->denoise_pwr_cfg |= ABB_PU_HP_L_MPATH|ABB_PU_HP_R_MPATH;
#endif

#if defined(__CT_CHIP_PKG_RDA5836TE__)
		    hwp_abb->hp_chg_cfg &= ~(ABB_HP_CHG_RES_EN|HP_L_CHG_RES_NPART_EN);
#else
                  hwp_abb->hp_chg_cfg &= ~(ABB_HP_CHG_RES_EN|HP_L_CHG_RES_NPART_EN|HP_R_CHG_RES_NAPRT_EN);
#endif
    }
#endif
 }
 else
 {
     #if ( APP_SUPPORT_DIF_DAC == 0)
     {
         if(g_halSysabbOpenWaitMode == TRUE)
         {
         	hal_abbWaitPdHp();
         }
         else
         {
         	hal_abbPdHp();
         }
     }
     #else
     {
#if defined(__CT_CHIP_PKG_RDA5836TE__)
		   hwp_abb->denoise_pwr_cfg &= ~ABB_PU_HP_L_MPATH;
#else
		   hwp_abb->denoise_pwr_cfg &= ~(ABB_PU_HP_L_MPATH|ABB_PU_HP_R_MPATH);
#endif

#if defined(__CT_CHIP_PKG_RDA5836TE__)
		   hwp_abb->denoise_pwr_cfg &= ~ABB_PU_HP_L_BIAS ;
#else
		   hwp_abb->denoise_pwr_cfg &= ~(ABB_PU_HP_L_BIAS|ABB_PU_HP_R_BIAS) ;
#endif

                 hwp_abb->denoise_other_cfg &= ~(ABB_HP_DISCHG_RES_BIT_MASK);
#if defined(__CT_CHIP_PKG_RDA5836TE__)
		   hwp_abb->denoise_other_cfg |= ABB_HP_L_DISCHG_RES_EN |HP_L_DISCHG_RES_NPART_EN|ABB_HP_DISCHG_RES_BIT(3);
#else
		    hwp_abb->denoise_other_cfg |= ABB_HP_L_DISCHG_RES_EN | ABB_HP_R_DISCHG_RES_EN |HP_L_DISCHG_RES_NPART_EN|ABB_HP_DISCHG_RES_BIT(3);
#endif

                 hal_TimDelay(2);

#if defined(__CT_CHIP_PKG_RDA5836TE__)
		   hwp_abb->denoise_other_cfg &= ~(ABB_HP_L_DISCHG_RES_EN|HP_L_DISCHG_RES_NPART_EN) ;
#else
		    hwp_abb->denoise_other_cfg &= ~( ABB_HP_L_DISCHG_RES_EN | ABB_HP_R_DISCHG_RES_EN |HP_L_DISCHG_RES_NPART_EN );
#endif
     }
	 #endif
 }
}
 
#else

VOID hal_abbEnablePa(BOOL on)
{
 if(on == TRUE)
 {
#if ( APP_SUPPORT_DIF_DAC == 0)
     {
         if(g_halSysabbOpenWaitMode == TRUE)
         {
         	hal_abbWaitPuHp();
         }
         else
         {
              hal_abbPuHp();
         }
     }
#else
    {
        hwp_abb->denoise_pwr_cfg =0x300;
        hwp_abb->denoise_pwr_cfg  =0xf00;
    }
#endif
 }
 else
 {
     #if ( APP_SUPPORT_DIF_DAC == 0)
     {
         if(g_halSysabbOpenWaitMode == TRUE)
         {
         	hal_abbWaitPdHp();
         }
         else
         {
         	hal_abbPdHp();
         }
     }
     #else
     {
         hwp_abb->denoise_pwr_cfg =0x300;
         hwp_abb->denoise_pwr_cfg  =0x0;
     }
     #endif
 }
}
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================

UINT8 First_Pa_Set=0xaa;
VOID hal_abbFirstEnablePa(BOOL on)
{
   if(First_Pa_Set == 0xaa)
   {
	   First_Pa_Set = 0x55;
   }   
}

VOID hal_abbLineInMode(BOOL isLineInMode)
{
       if(isLineInMode == TRUE)
      	{
      	    hal_abbEnablePa(TRUE);
	    hwp_abb->Codec_Mode0 &= ~(ABB_MICINPUT_R_SEL_MASK| ABB_MICINPUT_L_SEL_MASK);
	    hwp_abb->Codec_Mode0 |= ABB_MICINPUT_R_SEL(12)| ABB_MICINPUT_L_SEL(9);
      	    hal_AudSetSampleRate(HAL_AUD_USER_CODEC,HAL_AIF_FREQ_48000HZ);
	    hwp_sysCtrl->audio_clk_cfg &= ~SYS_CTRL_AIF_TX_STB_DIV_MASK;
           hwp_sysCtrl->audio_clk_cfg |= SYS_CTRL_AIF_TX_STB_DIV_EN | SYS_CTRL_AIF_TX_STB_DIV_LD_CFG|SYS_CTRL_AIF_TX_STB_DIV(1024)|SYS_CTRL_CODEC_CLK_SEL_PLL_AUDIO_PLL;
           hwp_audio_codec ->codec_cfg |= AUDIO_CODEC_S_LOOP;
      	}
	else
	{
	    hwp_audio_codec ->codec_cfg &= ~AUDIO_CODEC_S_LOOP;
	    hwp_abb->Codec_Mode0 &= ~(ABB_MICINPUT_R_SEL_MASK| ABB_MICINPUT_L_SEL_MASK);
	    hwp_abb->Codec_Mode0 |= ABB_MICINPUT_R_SEL(3);
	    hal_abbEnablePa(FALSE);
	}
}


PUBLIC VOID hal_AbbOpen(VOID)
{
    g_halSysAuxPllIsOn = TRUE;
    g_halSysabbInitDone = FALSE;
    if (!g_halSysabbInitDone)
    {
        //abb 
        hwp_audio_codec ->dac_path_cfg1 |= AUDIO_CODEC_S_DAC_VOLUME_MUTE;
#if ((APP_SUPPORT_DIF_DAC == 1) && ( APP_SUPPORT_DIF_ADC == 1))	 
#if defined(__CT_CHIP_PKG_RDA5836TE__)
        hwp_abb->codec_cfg0 = ABB_SEL_BG  | ABB_DAC_VOS_BIT(2) | ABB_IBIAS_BIT(2) |ABB_VCOMBUF_SEL(5) |ABB_VCOM_MODE(2); 
#else
        hwp_abb->codec_cfg0 = ABB_SEL_BG  | ABB_DAC_VOS_BIT(2) | ABB_IBIAS_BIT(3) |ABB_VCOMBUF_SEL(5) |ABB_VCOM_MODE(2); 
#endif
#else
        hwp_abb->codec_cfg0 = ABB_SEL_BG  | ABB_DAC_VOS_BIT(2) | ABB_IBIAS_BIT(2) |ABB_VCOMBUF_SEL(2) |ABB_VCOM_MODE(0); 
#endif

        hwp_abb->codec_cfg1 = ABB_DAC_VREG_BIT(5);  

        hwp_abb->ADC_Gain = 0 ; 
		
#if ((APP_SUPPORT_DIF_DAC == 1) && ( APP_SUPPORT_DIF_ADC == 1))	 
#if defined(__CT_CHIP_PKG_RDA5836TE__)
	    hwp_abb->DAC_Gain = ABB_HP_GAIN_BIT(5) |  ABB_MUX_GAIN_BIT(3) | ABB_DAC_GAIN_BIT(1);   
#else
		hwp_abb->DAC_Gain = ABB_HP_GAIN_BIT(7) |  ABB_MUX_GAIN_BIT(2) | ABB_DAC_GAIN_BIT(5);  
#endif
#else
		hwp_abb->DAC_Gain = ABB_HP_GAIN_BIT(5) |  ABB_MUX_GAIN_BIT(3) | ABB_DAC_GAIN_BIT(1);  
#endif

        
#if ((APP_SUPPORT_DIF_DAC == 1) && ( APP_SUPPORT_DIF_ADC == 1))	 
#if defined(__CT_CHIP_PKG_RDA5836TE__)
		hwp_abb->Codec_Mode0 = ABB_HP_INPUTSEL(1) | ABB_MUX_IBIT(3) | ABB_HP_IBIT(1);  
#else
		hwp_abb->Codec_Mode0 = ABB_HP_INPUTSEL(1) | ABB_MUX_IBIT(2) | ABB_HP_IBIT(3); 
#endif
#else
	    hwp_abb->Codec_Mode0 = ABB_HP_INPUTSEL(1) | ABB_MUX_IBIT(3) | ABB_HP_IBIT(1);
#endif

        hwp_abb->Codec_Mode1 = ABB_DAC_R_MODE | ABB_DAC_L_MODE ;
        
#if ( APP_SUPPORT_DIF_DAC == 0)
        {
            hwp_abb->Codec_Mode1 &= ~ABB_HP_DIFF_MODE;  
            hwp_abb->abb_rsvd_cfg &= ~ HP_R_DIFF_MODE;
        }
#else
        {
            hwp_abb->Codec_Mode1 |=  ABB_HP_DIFF_MODE;
            hwp_abb->abb_rsvd_cfg |=  HP_R_DIFF_MODE;
        }
#endif

#if ((APP_SUPPORT_DIF_DAC == 1) && ( APP_SUPPORT_DIF_ADC == 1))	 

        hwp_abb->Power_Ctrl = ABB_PU_ALL|ABB_PU_IVREF |ABB_PU_VCOMBUF_B ;	
#else
        hwp_abb->Power_Ctrl = ABB_PU_ALL|ABB_PU_IVREF |ABB_PU_VCOMBUF_C ;

#endif
         
        hwp_abb->Power_Ctrl  |= ABB_PU_DAC_L |ABB_PU_MUX_L  ;

#if defined(__CT_CHIP_PKG_RDA5836TE__)
#else
        hwp_abb->Power_Ctrl  |= ABB_PU_DAC_R | ABB_PU_MUX_R;  
#endif                                                        
		
        hwp_abb->aupll_setting2 &= ~ ABB_AUPLL_CLKOUT_EN(7);
        hwp_abb->aupll_setting2 |= ABB_PU_AUPLL | ABB_AUPLL_RSTB | ABB_AUPLL_CLKOUT_EN(7);

        hwp_abb->sdmsetting = ABB_SDM_INT_DEC_SEL(3) | ABB_SDM_RESETN;

        hwp_abb->hp_chg_cfg = 0;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               hwp_audio_codec ->codec_cfg = AUDIO_CODEC_S_CODEC_EN;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        hwp_audio_codec ->dac_path_cfg1 = AUDIO_CODEC_S_DAC_EN | AUDIO_CODEC_S_DAC_VOLUME(49);
        
        hwp_audio_codec ->dac_path_cfg2 = AUDIO_CODEC_S_DWA_EN | AUDIO_CODEC_S_DITHER_BYPASS |AUDIO_CODEC_DITHER_GAIN_4_3(1) |AUDIO_CODEC_SDM_GAIN(3) ;

        hwp_audio_codec ->eq_ctrl = AUDIO_CODEC_EQ_BYPASS;
		
        hwp_abb->vrefc_cal = 0x14c;
        hwp_abb->vadc_cal = 0x298;
        hwp_abb->vdac_cal = 0x14c;

#ifndef CHIP_HAS_NO_ISPI
#ifdef SPI_REG_DEBUG
        extern VOID abb_RegisterSpiRegCheckFunc(VOID);
        abb_RegisterSpiRegCheckFunc();
#endif // SPI_REG_DEBUG
#endif
        g_halSysabbInitDone = TRUE;
    }
}
#ifdef SUPPORT_ASIC_EQ
PUBLIC VOID hal_AbbSetEq(UINT16 eq_en, UINT32 *coef)
{
        UINT8 i;
        UINT32 *abb_coef;

        abb_coef = (UINT32 *)&hwp_audio_codec ->eq_coef0_low;

        hwp_audio_codec ->eq_ctrl = AUDIO_CODEC_EQ_BYPASS;

	hal_HstSendEvent(SYS_EVENT, 0x2017e900);
	hal_HstSendEvent(SYS_EVENT, eq_en);
       hal_HstSendEvent(SYS_EVENT, coef);

	hal_HstSendEvent(SYS_EVENT, abb_coef);
       hal_HstSendEvent(SYS_EVENT, hwp_audio_codec ->eq_ctrl);
	hal_HstSendEvent(SYS_EVENT, hwp_audio_codec ->eq_coef0_low);


        if(eq_en)
        {
            hwp_audio_codec ->eq_ctrl = AUDIO_CODEC_EQ_RESET;
            for(i = 0; i<7; i++)
            {
                *abb_coef= (*coef<<2)&0xffff;                   //low
                *(abb_coef+1)= (*coef>>14)&0xffff;     //high

                *(abb_coef+2)= ((*(coef+1))<<2)&0xffff;                   //low
                *(abb_coef+3)= ((*(coef+1))>>14)&0xffff;     //high

                *(abb_coef+4)= ((*(coef+2))<<2)&0xffff;                   //low
                *(abb_coef+5)= ((*(coef+2))>>14)&0xffff;     //high
                
                *(abb_coef+6)= (((*(coef+4))<<2))&0xffff;                   //low
                *(abb_coef+7)= (((*(coef+4))>>14))&0xffff;     //high
                
                *(abb_coef+8)= (((*(coef+5))<<2))&0xffff;                   //low
                *(abb_coef+9)= (((*(coef+5))>>14))&0xffff;     //high

                abb_coef += 10;
                coef += 6;
#if 0
	hal_HstSendEvent(SYS_EVENT, 0x2017e901);
	hal_HstSendEvent(SYS_EVENT, *abb_coef);
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+1));
	hal_HstSendEvent(SYS_EVENT, *coef);

       hal_HstSendEvent(SYS_EVENT, *(abb_coef+2));
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+3));
	hal_HstSendEvent(SYS_EVENT, *(coef+1));

	hal_HstSendEvent(SYS_EVENT, *(abb_coef+4));
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+5));
	hal_HstSendEvent(SYS_EVENT, *(coef+2));
	
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+6));
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+7));
       hal_HstSendEvent(SYS_EVENT, *(coef+3));
	hal_HstSendEvent(SYS_EVENT, *(coef+4));
	
	hal_HstSendEvent(SYS_EVENT, *(abb_coef+8));
       hal_HstSendEvent(SYS_EVENT, *(abb_coef+9));
	hal_HstSendEvent(SYS_EVENT, *(coef+5));
#endif
            }
            //enable abb eq;
            hwp_audio_codec ->eq_ctrl = 0;

        }

}
#endif
PUBLIC VOID hal_AbbEnableUsbPhy(BOOL enable)
{
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
UINT32 usbCtrl=hwp_abb->usb_phy_ctrl ;
hal_HstSendEvent(SYS_EVENT, 0x20170815);
hal_HstSendEvent(SYS_EVENT, usbCtrl);
if (enable)
{
    
    usbCtrl |= ABB_USB_CTRL(4);
}
else
 {
   usbCtrl  &= ~ABB_USB_CTRL_MASK;
 }
  hwp_abb->usb_phy_ctrl = usbCtrl;
  hal_HstSendEvent(SYS_EVENT, usbCtrl);
#endif
}

PUBLIC VOID hal_AbbEnableSim(UINT32 simMask)
{
#if 0
    UINT16 value = ABB_UNSEL_RST_VAL_1|
                   ABB_UNSEL_RST_VAL_2|
                   ABB_UNSEL_RST_VAL_3|
                   ABB_UNSEL_RST_VAL_4;

    if (simMask & HAL_ABB_SIM_SELECT_SIM0)
    {
        value |= ABB_SIM_SELECT_SIM1;
    }
    if (simMask & HAL_ABB_SIM_SELECT_SIM1)
    {
        value |= ABB_SIM_SELECT_SIM2;
    }
    if (simMask & HAL_ABB_SIM_SELECT_SIM2)
    {
        value |= ABB_SIM_SELECT_SIM3;
    }
    if (simMask & HAL_ABB_SIM_SELECT_SIM3)
    {
        value |= ABB_SIM_SELECT_SIM4;
    }

    hal_AbbRegWrite(CODEC_SIM_INTERFACE, value);
#endif
}

PROTECTED VOID hal_AbbIspiReadOutputSelect(UINT32 addr)
{
}

#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)
/******************************************************************************
 * hal_AudAuxPllIsOn: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC BOOL hal_AudAuxPllIsOn(VOID)
{
    hal_HstSendEvent(BOOT_EVENT, 0x20160617);
    hal_HstSendEvent(BOOT_EVENT, g_halSysAuxPllIsOn);

    //Check if bss is initialed
    return g_halSysAuxPllIsOn;
}
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)

#ifdef AUD_3_IN_1_SPK
PUBLIC VOID hal_AudBypassNotchFilterReq(BOOL bypass)
{
    // 3-in-1 speaker enables the notch filter to filter out 170 Hz signal
    // to avoid vibration, and disables it to vibrate.
    UINT32 status = hal_SysEnterCriticalSection();
    g_halAudBypassNotchFilterApp = bypass;

    if (g_halAudBypassNotchFilterApp)
    {
        g_halAudHpDetect_3dh |= ABB_NOTCH20_BYPASS;
    }
    else
    {
        g_halAudHpDetect_3dh &= ~ABB_NOTCH20_BYPASS;
    }
    hal_AbbRegWrite(CODEC_HP_DETECT_3DH, g_halAudHpDetect_3dh);
    hal_SysExitCriticalSection(status);
}
#endif // AUD_3_IN_1_SPK

/// Must wait for AUD_TIME_SET_FM_MODE if setting to non-FM mode
PUBLIC VOID hal_AudSetFmMode(BOOL isFmMode)
{
   if(isFmMode == TRUE)
   {
	  hwp_audio_codec ->codec_cfg |= AUDIO_CODEC_FM_MODE;
         hal_AudSetSampleRate(HAL_AUD_USER_FM,HAL_AIF_FREQ_48000HZ);
   }
   else
   {
         hwp_audio_codec ->codec_cfg &= ~AUDIO_CODEC_FM_MODE;
   }
}

PUBLIC VOID hal_AudSetVoiceMode(BOOL isVoiceMode)
{
    return;
}

#ifdef ABB_HP_DETECT
VOID hal_AudMuteHeadphoneDac(BOOL mute)
{
    g_halAudHeadphoneDacMuted = mute;
}
#endif

PUBLIC VOID hal_AudEnableDac(BOOL on)
{
	
}

PUBLIC VOID hal_AudEnableAdc(BOOL on)
{
    if(on == TRUE)
    {
        hwp_abb->Power_Ctrl |= ABB_PU_VMIC|ABB_PU_VREFLN;
        hwp_abb->codec_cfg0 |= ABB_VMIC_BIT(1);
        hwp_abb->codec_cfg1 &= ~ (ABB_MICPGA_R_IBIT_MASK | ABB_MICPGA_L_IBIT_MASK | ABB_ADC_IBIT_MASK);
        hwp_abb->codec_cfg1 |=ABB_ADC_CLK_INV_SEL ;          
        hwp_abb->ADC_Gain = ABB_ADC_R_GAIN_BIT(6) | ABB_ADC_L_GAIN_BIT(6) | ABB_MICPGA_R_GAIN_BIT(3) |ABB_MICPGA_L_GAIN_BIT(3);
#if defined(APP_SUPPORT_DIF_ADC) && (APP_SUPPORT_DIF_ADC == 1)
        hwp_abb->Codec_Mode0 |=  ABB_MICINPUT_L_SEL(3);
#else
#if defined(APP_SUPPORT_MIC_P) && (APP_SUPPORT_MIC_P == 1)
        hwp_abb->Codec_Mode0 |=  ABB_MICINPUT_L_SEL(9);
#else
        hwp_abb->Codec_Mode0 |=  ABB_MICINPUT_L_SEL(12);
#endif
#endif
        hwp_abb->Power_Ctrl  |=ABB_PU_MICPGA_L|ABB_PU_ADC_L;
#if defined(__CT_CHIP_PKG_RDA5836TE__)
#else
        hwp_abb->Codec_Mode0 |= ABB_MICINPUT_R_SEL(3);
        hwp_abb->Power_Ctrl  |=ABB_PU_ADC_R |ABB_PU_MICPGA_R;
#endif

        hwp_audio_codec ->adc_pathcfg = AUDIO_CODEC_S_ADC_EN |AUDIO_CODEC_S_ADC_VOLUME(10) | AUDIO_CODEC_ADC_LPF_BYPASS | AUDIO_CODEC_ANA_ADC_CLK_INV|AUDIO_CODEC_S_ADC_OSR_SEL(3);

    }
    else
    {
        hwp_abb->codec_cfg0 &= ~(ABB_VMIC_BIT(3));
        hwp_abb->Codec_Mode1 &= ~ABB_ADC_SMPLRATE_SEL_MASK;
        hwp_abb->Power_Ctrl &=  ~(ABB_PU_VMIC|ABB_PU_VREFLN|ABB_PU_ADC_R |ABB_PU_MICPGA_R|ABB_PU_MICPGA_L|ABB_PU_ADC_L);
    }
}
PUBLIC VOID hal_AudEnableCodecDsp(BOOL on)
{
    return;
}


#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__) && defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
/******************************************************************************
 * hal_AudEnableCodecAuxPll: 
 * DESCRIPTION: - sys pll is always off and use aux pll when needed
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_AudEnableCodecAuxPll(BOOL on)
{
    UINT32 status = hal_SysEnterCriticalSection();
    hal_HstSendEvent(BOOT_EVENT, 0x2016061a);
    hal_HstSendEvent(BOOT_EVENT, on);

    if(g_halSysabbInitDone)
    {
        if(on)
        {
            if(!g_halSysAuxPllIsOn)
            {
                //Power on aux pll

                hwp_pmu_intf->xtal_ctrl |=PMU_INTF_XTAL_CLK_AUPLL_EN_LV; 

                hwp_abb->Power_Ctrl |= ABB_PU_ALL;
                hwp_abb->aupll_setting2 |= ABB_PU_AUPLL;
                hwp_abb->aupll_setting2 |= ABB_AUPLL_CLKOUT_EN(7);
                hwp_abb->aupll_setting2 &= ~ABB_AUPLL_RSTB;
                hwp_abb->aupll_setting2 |= ABB_AUPLL_RSTB ;

                g_halSysAuxPllIsOn = TRUE;
            }
        }
        else
        {
            //Power off aux pll
            hwp_abb->aupll_setting2 &= ~ABB_PU_AUPLL;
            hwp_abb->aupll_setting2 &= ~ ABB_AUPLL_CLKOUT_EN(7);
            hwp_pmu_intf->xtal_ctrl &=(~PMU_INTF_XTAL_CLK_AUPLL_EN_LV); 
            g_halSysAuxPllIsOn = FALSE;
        }
    }
    else if(on)
    {
        HAL_ASSERT(g_halSysAuxPllIsOn, "Aux pll must be on default");
    }
    
    hal_HstSendEvent(BOOT_EVENT, 0x2016061b);

    hal_SysExitCriticalSection(status);

}

#else /*__HAL_SYS_SWITCH_PLL_DYNAMIC__*/
/******************************************************************************
 * hal_AudEnableCodecAuxPll: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_AudEnableCodecAuxPll(BOOL on)
{
    
}
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */


// =============================================================================
// hal_AudGetSampleRateIndex
// -----------------------------------------------------------------------------
/// get index of HAL audio sample rate according to aif sampleRate
/// @param  sampleRate The AIF sample rate
/// @return index of HAL audio sample rate
// =============================================================================
PRIVATE HAL_AUD_SAMPLE_RATE_INDEX_T hal_AudGetSampleRateIndex(HAL_AIF_FREQ_T sampleRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index = HAL_AUD_SAMPLE_RATE_INDEX_8K;

    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ :
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_8K;
            break;
        }
        case HAL_AIF_FREQ_11025HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_11_025K;
            break;
        }
        case HAL_AIF_FREQ_12000HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_12K;
            break;
        }
        case HAL_AIF_FREQ_16000HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_16K;
            break;
        }
        case HAL_AIF_FREQ_22050HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_22_05K;
            break;
        }
        case HAL_AIF_FREQ_24000HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_24K;
            break;
        }
        case HAL_AIF_FREQ_32000HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_32K;
            break;
        }
        case HAL_AIF_FREQ_44100HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44_1K;
            break;
        }
        case HAL_AIF_FREQ_48000HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_48K;
            break;
        }
        case HAL_AIF_FREQ_44103HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44103;
            break;
        }
        case HAL_AIF_FREQ_44102HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44102;
            break;
        }
        case HAL_AIF_FREQ_44101HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44101;
            break;
        }
        case HAL_AIF_FREQ_44099HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44099;
            break;
        }
        case HAL_AIF_FREQ_44098HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44098;
            break;
        }
        case HAL_AIF_FREQ_44097HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44097;
            break;
        }
        case HAL_AIF_FREQ_44096HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44096;
            break;
        }
        case HAL_AIF_FREQ_44095HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44095;
            break;
        }
        case HAL_AIF_FREQ_44094HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44094;
            break;
        }
        case HAL_AIF_FREQ_44093HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44093;
            break;
        }
        case HAL_AIF_FREQ_44092HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44092;
            break;
        }
		case HAL_AIF_FREQ_44091HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44091;
            break;
        }
	    case HAL_AIF_FREQ_44090HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_44090;
            break;
        }
        case HAL_AIF_FREQ_48004HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_48004;
            break;
        }
        case HAL_AIF_FREQ_48003HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_48003;
            break;
        }
        case HAL_AIF_FREQ_48002HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_48002;
            break;
        }
        case HAL_AIF_FREQ_48001HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_48001;
            break;
        }
        case HAL_AIF_FREQ_47999HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_47999;
            break;
        }
        case HAL_AIF_FREQ_47998HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_47998;
            break;
        }
        case HAL_AIF_FREQ_47997HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_47997;
            break;
        }
        case HAL_AIF_FREQ_47996HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_47996;
            break;
        }
        case HAL_AIF_FREQ_47995HZ:
        {
            index = HAL_AUD_SAMPLE_RATE_INDEX_47995;
            break;
        }
        default:
        {
            HAL_ASSERT(FALSE, "Unsupported sample rate in HAL ABB: %d", sampleRate);
            break;
        }
    }

    return index;
}

#if defined(APP_SUPPORT_DCOFFSET) && (APP_SUPPORT_DCOFFSET == 1)
PUBLIC uint16 hal_AudGetDcOffsetCount(HAL_AIF_FREQ_T sampleRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;
	
    index = hal_AudGetSampleRateIndex(sampleRate);
    return (gc_halAudDcOffsetCount[index]);
}

PUBLIC uint16 hal_AudGetDcOffsetValue(void)
{
    uint8 efuse_data=0,i;
    uint16 dc_offset_data=0;
	
    hwp_pmu_intf->efuse_ctrl1 =0x70045200;
    for(i=0;i<10;i++);
    hwp_pmu_intf->efuse_ctrl1 =0x70045700;
    for(i=0;i<10;i++);
    hwp_pmu_intf->efuse_ctrl1 =0x70047f00;
    for(i=0;i<10;i++);
	
    efuse_data = hwp_pmu_intf->efuse_ctrl1;
    hal_HstSendEvent(0xff, 0x88778875);
    hal_HstSendEvent(0xff, efuse_data);
    if(efuse_data & 0x80)
    {
        dc_offset_data = (efuse_data&0x7f)<<1;
	 dc_offset_data = ~dc_offset_data+1;
    }
    else
    {
        dc_offset_data = efuse_data<<1;
    }

    return dc_offset_data;
}
#endif

PUBLIC VOID hal_AudSetSampleRate(HAL_AUD_USER_T user, HAL_AIF_FREQ_T sampleRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;

    index = hal_AudGetSampleRateIndex(sampleRate);
    hwp_abb->sdmfreq1 = gc_halAudSdm2FreqRegs[index].sdm2FreqHigh;
    hwp_abb->sdmfreq2 = gc_halAudSdm2FreqRegs[index].sdm2FreqLow;
    if (user == HAL_AUD_USER_CODEC)
    {
	 hwp_audio_codec ->dac_clk_cfg = AUDIO_CODEC_DAC_CLK_LD | AUDIO_CODEC_FS_CNT_INT(gc_halAudDacSRDivCntInteger[index]);
       
    }
    else if (user == HAL_AUD_USER_FM)
    {
        hwp_audio_codec ->dac_clk_cfg = AUDIO_CODEC_DAC_CLK_LD | AUDIO_CODEC_FS_CNT_INT(gc_halAudDacSRDivCntInteger[9]);
    }
    
    if((sampleRate == HAL_AIF_FREQ_8000HZ))
    {
        hwp_audio_codec ->dac_path_cfg1 &= ~AUDIO_CODEC_S_DAC_OSR_SEL_MASK;
        hwp_audio_codec ->dac_path_cfg1 |= AUDIO_CODEC_S_DAC_OSR_SEL(2);//512
    }
    else if(sampleRate == HAL_AIF_FREQ_32000HZ)
    {
        hwp_audio_codec ->dac_path_cfg1 &= ~AUDIO_CODEC_S_DAC_OSR_SEL_MASK;//128
    }
    else
    {
       hwp_audio_codec ->dac_path_cfg1 &= ~AUDIO_CODEC_S_DAC_OSR_SEL_MASK;
       hwp_audio_codec ->dac_path_cfg1 |= AUDIO_CODEC_S_DAC_OSR_SEL(1);//256
    }
		
    hwp_audio_codec ->dac_path_cfg1 |= AUDIO_CODEC_DAC_PATH_RESET;
    hwp_audio_codec ->dac_path_cfg1 &= ~(AUDIO_CODEC_DAC_PATH_RESET);
}

PUBLIC VOID hal_AudCalibSampleRate(HAL_AUD_USER_T user, HAL_AIF_FREQ_T sampleRate1 ,HAL_AIF_FREQ_T sampleRate2)
{
	HAL_AUD_SAMPLE_RATE_INDEX_T index1, index2;

	index1 = hal_AudGetSampleRateIndex(sampleRate1);
	index2 = hal_AudGetSampleRateIndex(sampleRate2);

	hal_HstSendEvent(SYS_EVENT,0x07150001);
	hal_HstSendEvent(SYS_EVENT,user);

	if (user == HAL_AUD_USER_CODEC)
	{
		hal_HstSendEvent(SYS_EVENT,0x07150002);
		hal_HstSendEvent(SYS_EVENT,gc_halAudSdm2FreqRegs[index2].sdm2FreqHigh);
		hal_HstSendEvent(SYS_EVENT,gc_halAudSdm2FreqRegs[index2].sdm2FreqLow);

		hwp_abb->sdmfreq1 = gc_halAudSdm2FreqRegs[index2].sdm2FreqHigh;
		hwp_abb->sdmfreq2 = gc_halAudSdm2FreqRegs[index2].sdm2FreqLow;
	}
}


PUBLIC VOID hal_AudI2SSetSampleRate(HAL_AIF_FREQ_T sampleRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;
	
    index = hal_AudGetSampleRateIndex(sampleRate);
    //hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs[index].sdm2FreqHigh);
    //hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs[index].sdm2FreqLow);
    if((index == HAL_AUD_SAMPLE_RATE_INDEX_11_025K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_22_05K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_44_1K))
    {
            //hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(12));
    }
    else
    {
	     //hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(11));
    }
}

PUBLIC VOID hal_AudSetDacAnalogVolume(UINT32 vol)
{
    
}

PUBLIC VOID hal_AudSetAdcSampleRate(HAL_AIF_FREQ_T dacRate, HAL_AIF_FREQ_T adcRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;

    index = hal_AudGetSampleRateIndex(adcRate);
    if(adcRate == HAL_AIF_FREQ_8000HZ)
        hwp_audio_codec ->adc_clk_cfg = (AUDIO_CODEC_CNT_ADC_CLK_INT(gc_halAudAdcSRDivCntInteger[index]) | AUDIO_CODEC_ADC_CLK_LD);
    else
	 hwp_audio_codec ->adc_clk_cfg = (AUDIO_CODEC_CNT_ADC_CLK_INT(gc_halAudAdcSRDivCntInteger[index]) | AUDIO_CODEC_ADC_CLK_LD);
	
    hwp_abb->Codec_Mode1 &= ~ABB_ADC_SMPLRATE_SEL_MASK;
    
    if(adcRate == HAL_AIF_FREQ_48000HZ)
    {
        hwp_abb->Codec_Mode1 |= ABB_ADC_SMPLRATE_SEL(2);
    }
    else if(adcRate == HAL_AIF_FREQ_44100HZ)
    {
        hwp_abb->Codec_Mode1 |= ABB_ADC_SMPLRATE_SEL(3);
    }
    else if(adcRate == HAL_AIF_FREQ_16000HZ)
    {
        hwp_abb->Codec_Mode1 |= ABB_ADC_SMPLRATE_SEL(1);
    }
    else
    {
        hwp_abb->Codec_Mode1 |= ABB_ADC_SMPLRATE_SEL(0);
    }
    
}

// Force to enable charge pump clock.
// Called by PMD when LCD backlight is on if LCD_BACKLIGHT_CHARGE_PUMP enabled.
PUBLIC VOID hal_AudForceDcdc4ClockOn(BOOL on)
{
   /*
    UINT32 status = hal_SysEnterCriticalSection();
    if (on)
    {
        g_halAudCodecClockClassKPa |= (ABB_DCDC4_CLKGEN4_RESETN_DR|
                                       ABB_DCDC4_CLKGEN4_RESETN_REG);
    }
    else
    {
        g_halAudCodecClockClassKPa &= ~(ABB_DCDC4_CLKGEN4_RESETN_DR|
                                        ABB_DCDC4_CLKGEN4_RESETN_REG);
    }
    hal_AbbRegWrite(CODEC_CLOCK_CLASS_K_PA, g_halAudCodecClockClassKPa);
    hal_SysExitCriticalSection(status);
    */
}

PUBLIC VOID hal_AudEnableDSSI(BOOL on)
{
    return;
}

PUBLIC VOID hal_AudSetDacVolume(UINT32 vol)
{
    hwp_audio_codec ->dac_path_cfg1 &= ~ AUDIO_CODEC_S_DAC_VOLUME_MASK;
    hwp_audio_codec ->dac_path_cfg1 |= AUDIO_CODEC_S_DAC_VOLUME(vol);
}

PUBLIC VOID hal_AudSetAdcVolume(UINT32 vol)
{
        hwp_audio_codec ->adc_pathcfg = AUDIO_CODEC_S_ADC_EN |AUDIO_CODEC_S_ADC_VOLUME(vol) | AUDIO_CODEC_ADC_LPF_BYPASS | AUDIO_CODEC_ANA_ADC_CLK_INV|AUDIO_CODEC_S_ADC_OSR_SEL(3);
}

PUBLIC VOID hal_AudSetMicAnaVolume(UINT32 vol)
{
	// mute,-3 ~ +15db 
	hwp_abb->ADC_Gain = ABB_ADC_R_GAIN_BIT(vol) | ABB_ADC_L_GAIN_BIT(vol) | ABB_MICPGA_R_GAIN_BIT(3) |ABB_MICPGA_L_GAIN_BIT(3);

}

PROTECTED VOID hal_AudSetCodecMode(HAL_AUD_CODEC_MODE_PARAM_T *modeParam)
{
    return;
	/*
    HAL_AUD_SEL_T orgSelCfg = modeParam->orgSelCfg;
    // Line-in & MIC config
    UINT32 lineInSetting = 0;
    UINT32 micSetting = gc_halAbbCodecMicSetting;

    if (modeParam->curUser == HAL_AUD_USER_LINEIN ||
        modeParam->curUser == HAL_AUD_USER_FM)
    {
        if (orgSelCfg.headSel || orgSelCfg.rcvSel ||
            orgSelCfg.spkSel || orgSelCfg.spkMonoSel)
        {
            if(modeParam->curUser == HAL_AUD_USER_LINEIN)
            {
                lineInSetting |= ABB_LINEIN_TO_PA_MODE;
            }

            // A. muxin enable
            micSetting |= ABB_MUXIN_LEFT_EN_DR|ABB_MUXIN_LEFT_EN_REG|
                          ABB_MUXIN_RIGHT_EN_DR|ABB_MUXIN_RIGHT_EN_REG;

            // B. normal/mux mic disable
            micSetting |= ABB_MICIN_IBPGA_EN_DR;
            micSetting &= ~ABB_MICIN_IBPGA_EN_REG;
        }
        else // line-in mute
        {
            // TODO: Need to mute the speaker DAC line too?
        }
    }

    hal_AbbRegWrite(CODEC_LINEIN_SETTING, lineInSetting);
    hal_AbbRegWrite(CODEC_MIC_SETTING, micSetting);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    if((modeParam->curUser == HAL_AUD_USER_CODEC)||(modeParam->curUser == HAL_AUD_USER_FM))
    {// add by xzc, for codec tone

        //hal_AbbRegWrite(2, 0xe404);
        //hal_AbbRegWrite(3, 0x3888);

        hal_AbbRegWrite(0x06, 0x4f19);// vneg bit max zhusn
        //hal_AbbRegWrite(CODEC_CLOCK_CLASS_K_PA, 0x5d);
        //hal_AbbRegWrite(CODEC_HP_DETECT_3DH,0x98a7);

        //hwp_configRegs->audio_sel_cfg = 0x2;
        //hwp_configRegs->audio_spk_cfg = 0x13;
    }
#endif
*/
}

PUBLIC UINT32 hal_AudDacGainDb2Val(INT32 db)
{
    UINT32 value;
    // DacVolume[6:2] = 0 ... 31 = -26dB ... 5dB
    if (db < -26)
        db = -26;
    else if (db > 5)
        db = 5;
    value = db + 26;

    value = value*2;
    
    return value;
}

PUBLIC UINT32 hal_AudAdcGainDb2Val(INT32 db)
{
    UINT32 value;
    // MIC AdcVolume[12:9] = 0 ... 15 = MUTE -12dB -10dB ... 14dB 16dB
    if (db > 16)
        db = 16;
    if (db < -12)
        value = 0; // MIC ADC MUTE
    else
        value = (db + 12) / 2 + 1;
    return value;
}

#endif // !_FLASH_PROGRAMMER && !_T_UPGRADE_PROGRAMMER

