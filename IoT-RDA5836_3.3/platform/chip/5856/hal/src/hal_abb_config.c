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

#include "halp_abb_regs.h"
#include "halp_rda_abb.h"
#include "halp_debug.h"
#include "halp_sys.h"
#include "hal_sys.h"
#include "hal_rda_abb.h"
#include "hal_rda_audio.h"

#include "aud_m.h"
#include "sxr_tls.h"

#include "tgt_app_cfg.h"
#include "cos.h"
#include "boot_ispi.h"
#include "hal_host.h" // this is to send debug events
// =============================================================================
// EXTERNS                                                                       
// =============================================================================

extern VOID pmd_SpeakerPaCpLowOutput(BOOL low);
#ifdef ABB_HP_DETECT
extern VOID pmd_StartHpOutDetect(BOOL start);
#endif
extern VOID hal_AudSetAnaVolume(UINT32 vol);
#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__) && defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
extern BOOL hal_SysIsPllNeeded(VOID);
extern VOID boot_AbbIspiWrite(UINT32 regIdx, UINT32 value);
#endif
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
// HAL_AUD_DAC_DIV_REG_T
// -----------------------------------------------------------------------------
/// This type describes the register values to set in codec for a sample rate.
// =============================================================================
typedef struct
{
    UINT16 sel;
    UINT16 div;
} HAL_AUD_DAC_DIV_REG_T;

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

#if defined(AUD_3_IN_1_SPK) || defined(ABB_HP_DETECT)
// =============================================================================
// HP_DETECT_REG_VAL_T
// -----------------------------------------------------------------------------
/// This type describes the headphone detect register values to
// set in ABB when changing sample rate.
// =============================================================================
typedef struct
{
    UINT16 hpDet_3eh;
    UINT16 hpDet_3fh;
    UINT16 hpDet_44h;
    UINT16 hpDet_45h;
    UINT16 hpDet_46h;
    UINT16 hpDet_47h;
} HP_DETECT_REG_VAL_T;
#endif
 
//get I2S dataformat from wm8960
HAL_AIF_DATA_F0RMAT_T  I2s_DataFormat = HAL_AIF_LEFT_JUSTIFIED; 

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)

// MISC
const UINT16 g_halAbbCodecMisc =
    ABB_CODEC_EN_CODEC |
    ABB_RESERVER_ANALOG;

// AUX_PLL_SETTING1
const UINT16 g_halAbbCodecAuxPllSeting1 =
    ABB_PLL_AUX_CLKOUT_EN(3) |   //ABB_PLL_AUX_CLKOUT_EN(1)|
    ABB_PLL_AUX_CPAUX_BIT(4)|
    ABB_PLL_AUX_FILTER_IBIT(4)|
    ABB_PLL_AUX_CP_BIT(4) |
	ABB_PLL_AUX_SDM_CLK_SEL|
#if defined(__HAL_SYS_USE_AUX_PLL_ALWAYS__)
    0 //use aux pll always
#else
	ABB_PLL_AUX_SDM_CLK_TEST_EN //Set sys clock to bb pll
#endif /* __HAL_SYS_USE_AUX_PLL_ALWAYS__ */
    ;
    
// AUX_PLL_SETTING2
const UINT16 g_halAbbCodecAuxPllSeting2 =
    ABB_PLL_AUX_VREG_BIT(12);

//SDM2_SETTING1
const UINT16 g_halAbbCodecSdm2Seting1 =
    ABB_SDM2_DITHER_BYPASS|
    ABB_SDM2_RESETN |
    ABB_SDM2_RESERVED(0x09);

//DAC_SETTING1
const UINT16 g_halAbbCodecDacSeting1 =
    ABB_DAC_MODE_L_EN|
    ABB_DAC_MODE_R_EN|
    ABB_RESET_PA_RECI|
    ABB_RECI_GAIN_BIT(4);

//DAC_SETTING2
const UINT16 g_halAbbCodecDacSeting2 =
    ABB_PA_EN_HP|
    ABB_HP_GAIN_BIT(2);

//CLK_CTRL
const UINT16 g_halAbbCodecClkCtrl =
    AUDIO_S_DWA_EN|
    AUDIO_MASH_EN_ADC|
    AUDIO_S_DITHER_BYPASS|
    AUDIO_ANA_DAC_CLK_INV|
    AUDIO_ANA_ADC_CLK_INV|
   AUDIO_S_CODEC_EN; 

//adc
const UINT16 g_halAbbCodecAdc =
    ABB_MCPGA_GAIN_BIT(7)|
    ABB_MIC_STD_EN|
   ABB_BOOST_BP_MIC|
   ABB_MASH_EN_ADC; 

//MicBias
const UINT16 g_halAbbCodecMicBias =
    ABB_MICIN_IBPGA_EN|
    ABB_MICCAPLESS_MODE_EN|
    ABB_MICIN_CHOP_EN|
   ABB_MB_SEL_BIT(3); 

PRIVATE CONST HAL_AUD_SDM2_FREQ_REG_T
    gc_halAudSdm2FreqRegs[HAL_AUD_SAMPLE_RATE_INDEX_QTY] =
{
    {0xB57B, 0xF72}, // 8K
    {0xC287, 0x4B4}, // 11.025K
    {0xB57B, 0xF72}, // 12K
    {0xB57B, 0xF72}, // 16K
    {0xC287, 0x4B4}, // 22.05K
    {0xB57B, 0xF72}, // 24K
    {0xB57B, 0xF72}, // 32K
    {0xC287, 0x4B4},// 44.1K
    {0xB57B, 0xF72}, // 48K

 
    {0xC28A, 0xAE7}, //44103
    {0xC289, 0x8D6}, //44102
    {0xC288, 0x6C5}, //44101
    {0xC286, 0x2A3}, //44099 
    {0xC285, 0x092}, //44098
    {0xC283, 0xE80}, //44097
    {0xC282, 0xC6F}, //44096
    {0xC281, 0xA5E}, //44095
    {0xC280, 0x84D}, //44094
    {0xC27F, 0x63C}, //44093
    {0xC27E, 0x42B}, //44092
    {0xC27D, 0x21A}, //44091
    {0xC27C, 0x009}, //44090
    
    {0xB57f, 0xdac}, //48004
    {0xB57e, 0xe30}, //48003
    {0xB57d, 0xeb3}, //48002
    {0xB57c, 0xF72}, //48001
    {0xB57b, 0x040}, //47999
    {0xB57a, 0x0c1}, //47998
    {0xB579, 0x144}, //47997
    {0xB578, 0x1c8}, //47996
    {0xB577, 0x24b}, //47995
};

 PRIVATE CONST HAL_AUD_SDM2_FREQ_REG_T
	 gc_halAudSdm2FreqRegs_I2s[HAL_AUD_SAMPLE_RATE_INDEX_QTY] =
 {
	 {0xC0D3, 0xBB5}, // 8K
	 {0xCEAF, 0xBFF}, // 11.025K
	 {0xC0D3, 0xBB5}, // 12K
	 {0xC0D3, 0xBB5}, // 16K
	 {0xCEAF, 0xBFF}, // 22.05K
	 {0xC0D3, 0xBB5}, // 24K
	 {0xC0D3, 0xBB5}, // 32K
	 {0xCEAF, 0xBFF}, // 44.1K
	 {0xC0D3, 0xBB5}, // 48K
 
  
	 {0xC28A, 0xAE7}, //44103
	 {0xC289, 0x8D6}, //44102
	 {0xC288, 0x6C5}, //44101
	 {0xC286, 0x2A3}, //44099 
	 {0xC285, 0x092}, //44098
	 {0xC283, 0xE80}, //44097
	 {0xC282, 0xC6F}, //44096
	 {0xC281, 0xA5E}, //44095
	 {0xC280, 0x84D}, //44094
	 {0xC27F, 0x63C}, //44093
	 {0xC27E, 0x42B}, //44092
	 {0xC27D, 0x21A}, //44091
	 {0xC27C, 0x009}, //44090
	 
	 {0xC0B8, 0x0B5}, //48004
	 {0xC0D6, 0xEB5}, //48003
	 {0xC0D5, 0xD00}, //48002
	 {0xC0D4, 0xE00}, //48001
	 {0xC0D2, 0xDF0}, //47999
	 {0xC0D1, 0xDF0}, //47998
	 {0xC0D0, 0xDF0}, //47997
	 {0xC0CF, 0xBF0}, //47996
	 {0xC0CF, 0xBF0}, //47995
 };

 const uint8 gc_halAudDacSRDivCntInteger[HAL_AUD_SAMPLE_RATE_INDEX_QTY+1] =
{
    24, // 8K
    16, // 11.025K
    16, // 12K
    12, // 16K
    8, // 22.05K
    8, // 24K
    6, // 32K
    4,// 44.1K
    4, // 48K
    4 //fm
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


//When aux pll is on, system pll can be powerdown and use aux pll to save power
PRIVATE BOOL HAL_BOOT_DATA_INTERNAL g_halSysAuxPllIsOn = TRUE;
PRIVATE BOOL HAL_BOOT_DATA_INTERNAL g_halSysabbInitDone = FALSE;
VOID hal_abbEnablePa(BOOL on)
{
    UINT32 readdata;

    if(hal_AbbRegRead(CODEC_DAC_SETTING2,&readdata) == TRUE)
    {    
        hal_HstSendEvent(SYS_EVENT,readdata);
        if(on == TRUE)
        {
            readdata |= ABB_PA_EN_HP;
			hal_AbbRegWrite(CODEC_DAC_SETTING2, readdata);
	    }
	    else
	    {
	        readdata &= ~(ABB_PA_EN_HP);
		    hal_AbbRegWrite(CODEC_DAC_SETTING2, readdata);
	    }
    }
}

VOID hal_abbPdAll(BOOL on)
{
    UINT32 readdata;

    if(hal_AbbRegRead(CODEC_MISC,&readdata) == TRUE)
    {
        if(on == TRUE)
        {
            readdata |= ABB_PD_ALL;
			hal_AbbRegWrite(CODEC_MISC, readdata);

	    }
	    else
	    {
	        readdata &= ~(ABB_PD_ALL);
		    hal_AbbRegWrite(CODEC_MISC, readdata);
	    }
    }
}



// =============================================================================
//  FUNCTIONS
// =============================================================================
#ifndef APP_SUPPORT_I2S_CALI
UINT8 First_Pa_Set=0xaa;
#else
UINT8 First_Pa_Set=0x55;
#endif

VOID hal_abbFirstEnablePa(void)
{
   //UINT32 readdata;
   if(First_Pa_Set == 0x55)
   {
   	   //hal_TimDelay(3000 MS_WAITING);
	   hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(AUD_DEFAULT_ANALOG_GAIN) |ABB_PA_EN_HP | ABB_RESET_PA_HP );
	   hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(AUD_DEFAULT_ANALOG_GAIN) | ABB_PA_EN_HP);
	   First_Pa_Set = 0xff;
   }   
}
VOID hal_abbFirstEnablePaDelay(void)
{
   //UINT32 readdata;
   if(First_Pa_Set == 0xaa)
   {
	   hal_TimDelay(3000 MS_WAITING);
	   First_Pa_Set = 0x55;
   }   
}
UINT8 hal_getAbbFirstEnablePaFlag(void)
{
    return First_Pa_Set;
}
PUBLIC VOID hal_AudSetDacAnalogVolume(UINT32 vol)
{
    UINT32 old_vol;
	
    if(vol>7) vol = 7;
	
    hal_AbbRegRead(CODEC_DAC_SETTING2, &old_vol);

    old_vol >>= 5;
    old_vol &= 7;

    hal_HstSendEvent(SYS_EVENT,0x20171520);
    hal_HstSendEvent(SYS_EVENT,old_vol);
    hal_HstSendEvent(SYS_EVENT,vol);

    // hp_gain_bit[7:5]

    if(old_vol > vol)
    {
        for(int i=old_vol - vol - 1; i>=0; i--)
	{
#if 1
		if(vol + i == 0)
		{
		    hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(vol + i)|ABB_LINE_MODE_EN);
		}
		else
		{
		    hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_PA_EN_HP|ABB_HP_GAIN_BIT(vol + i)|ABB_LINE_MODE_EN);
		}
#else
		hal_AudSetAnaVolume(vol + i);
#endif
		COS_SleepByTick(8);
		hal_HstSendEvent(SYS_EVENT,0x20171521);
		hal_HstSendEvent(SYS_EVENT,vol + i);
	}
    }
    else if(old_vol < vol)
    {
        for(int i=vol - old_vol -1; i>=0; i--)
	{
#if 1
		if(vol - i == 0)
		{
		    hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(vol - i)|ABB_LINE_MODE_EN);
		}
		else
		{
		    hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_PA_EN_HP|ABB_HP_GAIN_BIT(vol - i)|ABB_LINE_MODE_EN);
		}
#else
		hal_AudSetAnaVolume(vol - i);
#endif
		COS_SleepByTick(8);
		hal_HstSendEvent(SYS_EVENT,0x20171522);
		hal_HstSendEvent(SYS_EVENT,vol - i);
	}
    }
    else
    {
		hal_AudSetAnaVolume(vol);
    }
}

VOID hal_abbEnableLine(BOOL on)
{
    UINT32 readdata;

    if(hal_AbbRegRead(CODEC_DAC_SETTING2,&readdata) == TRUE)
    {    
	if(on == TRUE)
	{
		readdata |= ABB_LINE_MODE_EN;
		hal_AbbRegWrite(CODEC_DAC_SETTING2, readdata);
	}
	else
	{
		readdata &= ~(ABB_LINE_MODE_EN);
		hal_AbbRegWrite(CODEC_DAC_SETTING2, readdata);
	}
    }
}

VOID hal_abbEnableDacMode(BOOL on)
{
    UINT32 readdata;

    if(hal_AbbRegRead(CODEC_DAC_SETTING1,&readdata) == TRUE)
    {    
	if(on == TRUE)
	{
		readdata |= ABB_DAC_MODE_L_EN;
		readdata |= ABB_DAC_MODE_R_EN;

		hal_AbbRegWrite(CODEC_DAC_SETTING1, readdata);
	}
	else
	{
		readdata &= ~(ABB_DAC_MODE_L_EN);
		readdata &= ~(ABB_DAC_MODE_R_EN);

		hal_AbbRegWrite(CODEC_DAC_SETTING1, readdata);
	}
    }
}
VOID hal_abbLineInMode(BOOL isLineInMode)
{
       COS_SleepByTick(128);

       if(isLineInMode == TRUE)
      	{
      	    //dac part
#ifndef SUPPORT_DIF_MIC
           hal_AbbRegWrite(CODEC_DAC_SETTING1, ABB_SYN_EN_DAC|ABB_RESET_PA_RECI|ABB_RECI_GAIN_BIT(4));
#else
           hal_AbbRegWrite(CODEC_DAC_SETTING1, ABB_RESET_PA_RECI|ABB_RECI_GAIN_BIT(4));
#endif
           hal_AbbRegWrite(CODEC_DAC_SETTING2,  ABB_HP_GAIN_BIT(AUD_DEFAULT_ANALOG_GAIN) | ABB_LINE_MODE_EN);
	     COS_SleepByTick(64);
           hal_AbbRegWrite(CODEC_DAC_SETTING2,  ABB_PA_EN_HP | ABB_HP_GAIN_BIT(AUD_DEFAULT_ANALOG_GAIN) | ABB_LINE_MODE_EN);
      	}
	else
	{

	     hal_abbEnablePa(FALSE);
            hal_abbEnableLine(FALSE);
#ifndef SUPPORT_DIF_MIC
		hal_AbbRegWrite(CODEC_DAC_SETTING1,  ABB_SYN_EN_DAC|ABB_DAC_MODE_L_EN | ABB_DAC_MODE_R_EN | ABB_RESET_PA_RECI | ABB_RECI_GAIN_BIT(4));
#else
		hal_AbbRegWrite(CODEC_DAC_SETTING1,  ABB_DAC_MODE_L_EN | ABB_DAC_MODE_R_EN | ABB_RESET_PA_RECI | ABB_RECI_GAIN_BIT(4));
#endif
	       COS_Sleep(800);
	}

}

PUBLIC VOID hal_AbbOpen(VOID)
{
    if (!g_halSysabbInitDone)
    {
        // reset software and register
        hal_AbbRegWrite(CODEC_RESET_CONTROL, ABB_REG_RESET_CLR|
                                          ABB_SOFT_RESET_CLR);
       
        hal_TimDelay(1);
        hal_AbbRegWrite(CODEC_RESET_CONTROL, ABB_REG_RESET_SET|
                                          ABB_SOFT_RESET_SET);

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)

     //clock init
     hal_AbbRegWrite(CODEC_MISC, g_halAbbCodecMisc);
     hal_AbbRegWrite(CODEC_AUX_PLL_SETTING1, g_halAbbCodecAuxPllSeting1);
     hal_AbbRegWrite(CODEC_AUX_PLL_SETTING2, g_halAbbCodecAuxPllSeting2);
     hal_AbbRegWrite(CODEC_SDM2_SETTING1, g_halAbbCodecSdm2Seting1);

     // Init codec sample rate
     hal_AudSetSampleRate(HAL_AUD_USER_CODEC, HAL_AIF_FREQ_8000HZ);

     hal_AbbRegWrite(CODEC_MISC, g_halAbbCodecMisc | ABB_DAC_EN_CODEC | ABB_ADC_EN_CODEC| ABB_RSTN_CODEC );

     //dac part
     //hal_AbbRegWrite(CODEC_DAC_SETTING1, g_halAbbCodecDacSeting1);
     //hal_AbbRegWrite(CODEC_DAC_SETTING2, g_halAbbCodecDacSeting2);

     //adc part
     hal_AbbRegWrite(CODEC_ADC, g_halAbbCodecAdc);
     hal_AbbRegWrite(CODEC_MIC_BIAS, g_halAbbCodecMicBias);

     //digital part
     hal_AbbRegWrite(CODEC_CLK_CTRL, g_halAbbCodecClkCtrl);
     hal_AbbRegWrite(CODEC_EP_DET_CTRL, 0xf8a7);
     hal_AbbRegWrite(CODEC_ADC_SR, 0x03a8);
     hal_AbbRegWrite(CODEC_VCOM_VREF_IBIAS, 0x1647);
     hal_AbbRegWrite(CODEC_DAC_VOLUME, AUDIO_S_MUTE_R | AUDIO_S_MUTE_L); 
     hal_AbbRegWrite(CODEC_ADC_CLK, AUDIO_CNT_ADC_CLK_INT(24) |AUDIO_S_ADC_OSR_SEL(1)); 
	 
     hal_AbbRegWrite(CODEC_DAC_SETTING1, ABB_DAC_MODE_R_EN | ABB_DAC_MODE_L_EN |ABB_RESET_DAC(3));
#ifndef SUPPORT_DIF_MIC
     hal_AbbRegWrite(CODEC_DAC_SETTING1, ABB_SYN_EN_DAC|ABB_DAC_MODE_R_EN | ABB_DAC_MODE_L_EN );
#else
     hal_AbbRegWrite(CODEC_DAC_SETTING1, ABB_DAC_MODE_R_EN | ABB_DAC_MODE_L_EN );
#endif
     hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(AUD_DEFAULT_ANALOG_GAIN) | ABB_RESET_MUX_EN | ABB_RESET_PA_HP);  
    // hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(1) | ABB_RESET_PA_HP);
     //hal_TimDelay(2500 MS_WAITING);
     //hal_AbbRegWrite(CODEC_DAC_SETTING2, ABB_HP_GAIN_BIT(2) | ABB_PA_EN_HP);

#ifdef SPI_REG_DEBUG
        extern VOID abb_RegisterSpiRegCheckFunc(VOID);
        abb_RegisterSpiRegCheckFunc();
#endif // SPI_REG_DEBUG

#if APP_SUPPORT_MENU == 1
        hal_abbFirstEnablePa();
#endif  //APP_SUPPORT_MENU == 1

#endif // !_FLASH_PROGRAMMER && !_T_UPGRADE_PROGRAMMER
        g_halSysabbInitDone = TRUE;
    }
}

PUBLIC VOID hal_AbbEnableUsbPhy(BOOL enable)
{
	return;
#if 0
    UINT32 usbCtrl = ABB_USB_CTRL(1)|
                     ABB_USB_VREG_BIT(7)|
                     ABB_SSI_VREFN_RBIT(3)|
                     ABB_SSI_VREFP_RBIT(3)|
                     ABB_PADET_IBIT(4);
    if (enable)
    {
        usbCtrl |= ABB_PD_USB;
    }
    hal_AbbRegWrite(USB_CONTROL, usbCtrl);
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
    UINT16 spiOutSel = hal_AbbIspiRead(CODEC_MISC);

    spiOutSel = (spiOutSel & ~ABB_SPI_OUT_MASK);
    if (addr < CODEC_DIG_MIN_REG_INDEX)
    {
        spiOutSel |= ABB_SPI_OUT_ABB;
    }
    else
    {
        spiOutSel |= ABB_SPI_OUT_CODEC;
    }
    hal_AbbIspiWrite(CODEC_MISC, spiOutSel);
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
    hal_HstSendEvent(SYS_EVENT, 0x20160617);
    hal_HstSendEvent(SYS_EVENT, g_halSysAuxPllIsOn);

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
	   hal_AbbRegWrite(CODEC_ADC_CLK, AUDIO_CNT_ADC_CLK_INT(24) | AUDIO_S_ADC_OSR_SEL(1) | AUDIO_FM_MODE);
	   hal_AbbRegWrite(CODEC_EP_DET_CTRL, 0x78a7);
	   /*
	   //hpf
	   hal_AbbIspiWrite(CODEC_RSVD9, 0x082b);
	   hal_AbbIspiWrite(CODEC_RSVD10, 0x4000);
	   hal_AbbIspiWrite(CODEC_RSVD11, 0x5938);
	   hal_AbbIspiWrite(CODEC_RSVD12, 0x533d);
	   hal_AbbIspiWrite(CODEC_RSVD13, 0x0001);
	   */
   }
   else
   {
         hal_AbbRegWrite(CODEC_ADC_CLK, AUDIO_CNT_ADC_CLK_INT(24) | AUDIO_S_ADC_OSR_SEL(1));
   }
   
}

PUBLIC VOID hal_AudSetVoiceMode(BOOL isVoiceMode)
{
    return;
}

VOID hal_abbSetPaOpenMode(BOOL on)
{
} 

#ifdef ABB_HP_DETECT
VOID hal_AudMuteHeadphoneDac(BOOL mute)
{
    g_halAudHeadphoneDacMuted = mute;
}
#endif

PUBLIC VOID hal_AudEnableDac(BOOL on)
{
	UINT32 readdata=0;

	hal_AbbRegRead(CODEC_MISC,&readdata);
    if (on)
    {
        // Not to direct-reg codec DAC
    	readdata &= ~ABB_DAC_EN_CODEC;
    }
    else
    {
        // Direct-reg to disable codec DAC
    	readdata |= ABB_DAC_EN_CODEC;
    }
    hal_AbbRegWrite(CODEC_MISC, readdata);
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
    //hal_HstSendEvent(SYS_EVENT, 0x2016061a);
    //hal_HstSendEvent(SYS_EVENT, on);

    if(g_halSysabbInitDone)
    {
        if(on || hal_SysIsPllNeeded())
        {
            if(!g_halSysAuxPllIsOn)
            {
                //Power on aux pll
                hal_AbbRegWrite(CODEC_AUX_PLL_SETTING2, (g_halAbbCodecAuxPllSeting2 & (~ABB_PD_PLL_AUX)));
                g_halSysAuxPllIsOn = TRUE;
            }
        }
        else
        {
            //Power off aux pll
            hal_AbbRegWrite(CODEC_AUX_PLL_SETTING2, (g_halAbbCodecAuxPllSeting2 | (ABB_PD_PLL_AUX)));
            g_halSysAuxPllIsOn = FALSE;
        }
    }
    else if(on)
    {
        HAL_ASSERT(g_halSysAuxPllIsOn, "Aux pll must be on default");
    }
    
    //hal_HstSendEvent(SYS_EVENT, 0x2016061b);

    hal_SysExitCriticalSection(status);

}

PUBLIC VOID Boot_AudEnableCodecAuxPll(VOID)
{
#if 0
        boot_AbbIspiWrite(CODEC_RESET_CONTROL, ABB_REG_RESET_CLR|
                                          ABB_SOFT_RESET_CLR);
       
        hal_TimDelay(1);
        boot_AbbIspiWrite(CODEC_RESET_CONTROL, ABB_REG_RESET_SET|
                                          ABB_SOFT_RESET_SET);
#endif

     boot_AbbIspiWrite(CODEC_AUX_PLL_SETTING1, g_halAbbCodecAuxPllSeting1);
     boot_AbbIspiWrite(CODEC_AUX_PLL_SETTING2, g_halAbbCodecAuxPllSeting2);
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
    UINT16 reg = g_halAbbCodecAuxPllSeting2;

    if(on)
    {
        reg &=~ABB_PD_PLL_AUX;
    }
    else
    {
        reg |= ABB_PD_PLL_AUX;
    }

	hal_AbbRegWrite(CODEC_AUX_PLL_SETTING2, reg);
    return;
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

PUBLIC VOID hal_AudSetSampleRate(HAL_AUD_USER_T user, HAL_AIF_FREQ_T sampleRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;

    index = hal_AudGetSampleRateIndex(sampleRate);

    if (user == HAL_AUD_USER_CODEC)
    {
        hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs[index].sdm2FreqHigh);
        hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs[index].sdm2FreqLow);
	 if((index == HAL_AUD_SAMPLE_RATE_INDEX_11_025K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_22_05K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_44_1K))
	 {
	          hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(6));
	  }
	  else
	  {
		   hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(5));
	  }
	 hal_AbbRegWrite(CODEC_DAC_SRO, AUDIO_S_DAC_OSR_SEL(1));
	 hal_AbbRegWrite(CODEC_DAC_SR1, AUDIO_FS_CNT_INT(gc_halAudDacSRDivCntInteger[index]));
	 
	 hal_AbbRegWrite(CODEC_ADC_CLK, AUDIO_FS_ADC_CNT_INT(gc_halAudDacSRDivCntInteger[index])|AUDIO_S_ADC_OSR_SEL(1));

    }
    else if (user == HAL_AUD_USER_FM)
    {
      hal_AbbRegWrite(CODEC_DAC_SRO, AUDIO_S_DAC_OSR_SEL(0));
	  hal_AbbRegWrite(CODEC_DAC_SR1, AUDIO_FS_CNT_INT(gc_halAudDacSRDivCntInteger[9]));
    }
}

PUBLIC VOID hal_AudCalibSampleRate(HAL_AUD_USER_T user, HAL_AIF_FREQ_T sampleRate1 ,HAL_AIF_FREQ_T sampleRate2)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;
    HAL_AUD_SAMPLE_RATE_INDEX_T index2;

    index = hal_AudGetSampleRateIndex(sampleRate1);
    index2 = hal_AudGetSampleRateIndex(sampleRate2);

    if (user == HAL_AUD_USER_CODEC)
    {
      if(I2s_DataFormat == HAL_AIF_LEFT_JUSTIFIED)
	  {
		    hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs[index2].sdm2FreqHigh);
		    hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs[index2].sdm2FreqLow);
	  }
	  else  //I2S_JUSTIFIED
	  {
	        hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs_I2s[index2].sdm2FreqHigh);
		    hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs_I2s[index2].sdm2FreqLow);
	  }
	  if((index == HAL_AUD_SAMPLE_RATE_INDEX_11_025K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_22_05K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_44_1K))
	  {
	       #ifndef APP_SUPPORT_I2S_CALI
	       hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(6));
		   #else
		   hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(13));
		   #endif
	  }
	  else
	  {
	       #ifndef APP_SUPPORT_I2S_CALI
		   hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(5));
		   #else
		   hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(11));
		   #endif
	  }
	  hal_AbbRegWrite(CODEC_DAC_SRO, AUDIO_S_DAC_OSR_SEL(1));
	  hal_AbbRegWrite(CODEC_DAC_SR1, AUDIO_FS_CNT_INT(gc_halAudDacSRDivCntInteger[index]));
    }
}

PUBLIC VOID hal_AudI2SSetSampleRate(HAL_AIF_FREQ_T sampleRate,HAL_AIF_DATA_F0RMAT_T  dataFormat)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;
	
    index = hal_AudGetSampleRateIndex(sampleRate);
	I2s_DataFormat = dataFormat;
	if(dataFormat == HAL_AIF_LEFT_JUSTIFIED)
	{
	    hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs[index].sdm2FreqHigh);
	    hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs[index].sdm2FreqLow);
	}
	else   //I2S_JUSTIFIED
    {
        hal_AbbRegWrite(CODEC_SDM2_FREQ1, gc_halAudSdm2FreqRegs_I2s[index].sdm2FreqHigh);
	    hal_AbbRegWrite(CODEC_SDM2_FREQ2, gc_halAudSdm2FreqRegs_I2s[index].sdm2FreqLow);
	}
    if((index == HAL_AUD_SAMPLE_RATE_INDEX_11_025K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_22_05K)||(index == HAL_AUD_SAMPLE_RATE_INDEX_44_1K))
    {
         hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(13));
    }
    else
    {
	     hal_AbbRegWrite(CODEC_DCDC3_SETTING, ABB_DCDC_CLKGEN3_DIV_BASE(11));
    }
}



PUBLIC VOID hal_AudSetAdcSampleRate(HAL_AIF_FREQ_T dacRate, HAL_AIF_FREQ_T adcRate)
{
    HAL_AUD_SAMPLE_RATE_INDEX_T index;

    index = hal_AudGetSampleRateIndex(adcRate);
    hal_AbbRegWrite(CODEC_ADC_CLK, AUDIO_CNT_ADC_CLK_INT(gc_halAudDacSRDivCntInteger[index]) |AUDIO_S_ADC_OSR_SEL(1) );
	
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
    // DacVolume[6:2] = 0 ... 31 = -26dB ... 5dB
    UINT32 data;
    data = AUDIO_S_ADC_DVST_CTL_3_0(15)|
    		AUDIO_S_DAC_VOLUME(vol);
    data |= AUDIO_DITHER_GAIN_4_3(1)|
    		AUDIO_SDM_GAIN(3);
    hal_AbbRegWrite(CODEC_DAC_VOLUME, data);
}

PUBLIC VOID hal_AudSetAdcVolume(UINT32 vol)
{
    // MIC AdcVolume[12:9] = 0 ... 15 = MUTE -12dB -10dB ... 14dB 16dB
    UINT32 data;
    data = AUDIO_S_ADC_VOLUME(vol)|
    		AUDIO_S_ADC_DVST_CTL_4;
    data |= AUDIO_DITHER_GAIN_2_0(4);
    hal_AbbRegWrite(CODEC_TEST_CTRL, data);
}

PUBLIC VOID hal_AudSetMicAnaVolume(UINT32 vol)
{
    // MIC AdcVolume[9:7] = 0 ... 7 
    UINT32 data;
    data = ABB_MCPGA_GAIN_BIT(vol)|
               ABB_MIC_STD_EN|
               ABB_BOOST_BP_MIC|
               ABB_MASH_EN_ADC; 
     hal_AbbRegWrite(CODEC_ADC, data);

}

PUBLIC VOID hal_AudSetAnaVolume(UINT32 vol)
{
    // MIC AdcVolume[9:7] = 0 ... 7 
    UINT32 data;
    hal_AbbRegRead(CODEC_DAC_SETTING2, &data);

    data &= ~(ABB_HP_GAIN_BIT(7));

    if(vol)
    {
    	data |= ABB_HP_GAIN_BIT(vol);
       data |= ABB_LINE_MODE_EN;
    }
    else
    {
    	data |= ABB_HP_GAIN_BIT(vol);
		
	data &= ~ABB_PA_EN_HP;
    }

    hal_AbbRegWrite(CODEC_DAC_SETTING2, data);

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

