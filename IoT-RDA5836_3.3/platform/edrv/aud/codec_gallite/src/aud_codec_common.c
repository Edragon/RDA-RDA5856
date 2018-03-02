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
/// @file aud_codec_common.c                                                        //
/// That file provides the implementation of the AUD common interface for the Codec //
//  chip.                                                                     //
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#include "cs_types.h"

#include "hal_rda_abb.h"
#include "aud_codec_common.h"
#include "audp_debug.h"

#include "hal_aif.h"
#include "hal_error.h"
#include "hal_sys.h"
#include "hal_host.h"

#include "hal_rda_audio.h"
#include "hal_ispi.h"
#include "pmd_m.h"
#include "sxr_tls.h"
#include "tgt_calib_m.h"
//#include "rfd_xcv.h"
#include "global_macros.h"
#include "sys_ctrl.h"

#include "tgt_aud_cfg.h"


// aud_StreamStart() and aud_StreamStop(), aud_Setup() are of mutually exclusive access,
// with the help of semaphore. Then here we do not need mutex any more.

#ifdef SWITCH_AUDIO_CODEC_FREQ
extern VOID rfd_XcvSwitchAudioCodecFreq(UINT32 freqVal_09h, UINT32 freqVal_0ah);
#endif

#ifdef ABB_HP_DETECT
extern VOID hal_AudSaveHeadphoneState(BOOL on);
#endif

extern BOOL g_audSetHeadMaxDigitalGain;
extern BOOL g_audSetMicMaxGain;


// =============================================================================
//  MACROS
// =============================================================================

#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef SPI_REG_DEBUG
#endif

// Define VCOM_BAT_LARGE_CAP to 1 if there is no external loud speaker and
// the capacitor connected to VCOM_BAT is larger than 1uF
#define VCOM_BAT_LARGE_CAP 0




//#define AUDIO_CLOSE_DEALY

#define IS_APP_SND_ITF(itf) (itf == SND_ITF_FM || itf == SND_ITF_LINEIN)

extern BOOL COS_Sleep ( UINT32 nMillisecondes);
// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// AUD_SAMPLE_RATE_INDEX_T
// -----------------------------------------------------------------------------
/// This type list the index of supported sample rates.
// =============================================================================
typedef enum
{
    AUD_SAMPLE_RATE_INDEX_8K,
    AUD_SAMPLE_RATE_INDEX_11_025K,
    AUD_SAMPLE_RATE_INDEX_12K,
    AUD_SAMPLE_RATE_INDEX_16K,
    AUD_SAMPLE_RATE_INDEX_22_05K,
    AUD_SAMPLE_RATE_INDEX_24K,
    AUD_SAMPLE_RATE_INDEX_32K,
    AUD_SAMPLE_RATE_INDEX_44_1K,
    AUD_SAMPLE_RATE_INDEX_48K,

    AUD_SAMPLE_RATE_INDEX_QTY,
} AUD_SAMPLE_RATE_INDEX_T;

// =============================================================================
// AUD_SAMPLE_RATE_REG_VAL_T
// -----------------------------------------------------------------------------
/// This type describes the register values to set in codec for a sample rate.
// =============================================================================
typedef struct
{
    UINT16 sel;
    UINT16 div;
} AUD_SAMPLE_RATE_REG_VAL_T;

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE) // 8808 or later
// =============================================================================
// AUD_CODEC_FREQ_REG_VAL_T
// -----------------------------------------------------------------------------
/// This type describes the register values to set in XCV when 
/// changing audio codec frequency for a sample rate.
// =============================================================================
typedef struct
{
    UINT16 sdm2FreqHigh;
    UINT16 sdm2FreqLow;
} AUD_CODEC_FREQ_REG_VAL_T;
#endif

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
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

extern UINT16 g_halHpDetect_3eh_codecMode;
extern UINT16 g_halHpDetect_3fh_codecMode;
#endif
#endif


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Playing status
PRIVATE BOOL  g_audCodecPlaying  = FALSE;

/// Recording status 
PRIVATE BOOL  g_audCodecRecording  = FALSE;

/// Openness status 
PRIVATE BOOL  g_audCodecOpened  = FALSE;

/// Remember the last settings applied
/// They will be set again to take into account any potential
/// calibration change.
PRIVATE AUD_LEVEL_T g_audCodecLatestLevel  = 
{       
        .spkLevel   = SND_SPK_MUTE,
        .micLevel   = SND_MIC_MUTE,
        .sideLevel  = SND_SIDE_MUTE,
        .toneLevel  = SND_TONE_0DB,
};

PRIVATE AUD_DEVICE_CFG_T g_audCodecLatestCfg   = 
{
        .spkSel = AUD_SPK_DISABLE,
        .spkType = AUD_SPEAKER_STEREO_NA,
        .micSel = AUD_MIC_DISABLE,
        .level  = &g_audCodecLatestLevel
};

#ifdef __FORCE_WORK_IN_26MHZ__
PRIVATE CONST AUD_SAMPLE_RATE_REG_VAL_T
    g_audNormalSampleRateRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
    {0x0080, 0x0659}, // 8K
    {0x5087, 0x049a}, // 11.025K
    {0x5087, 0x043b}, // 12K
    {0x8084, 0x032c}, // 16K
    {0x5085, 0x024d}, // 22.05K
    {0x6086, 0x021d}, // 24K
    {0x4081, 0x0196}, // 32K
    {0x6082, 0x0126}, // 44.1K
    {0xd083, 0x010e}, // 48K
#else // 8809 or later
    {0x0080, 0x0600}, // 8K
    {0x0087, 0x0400}, // 11.025K
    {0x0087, 0x0400}, // 12K
    {0x0084, 0x0300}, // 16K
    {0x0085, 0x0200}, // 22.05K
    {0x0086, 0x0200}, // 24K
    {0x0081, 0x0180}, // 32K
    {0x0082, 0x0100}, // 44.1K
    {0x0083, 0x0100}, // 48K
#endif
};

#ifdef SWITCH_AUDIO_CODEC_FREQ
PRIVATE CONST AUD_SAMPLE_RATE_REG_VAL_T
    g_audSwFreqModeSampleRateRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0x0080, 0x0600}, // 8K
    {0x0080, 0x0400}, // 11.025K
    {0x0080, 0x0400}, // 12K
    {0x0080, 0x0300}, // 16K
    {0x0080, 0x0200}, // 22.05K
    {0x0080, 0x0200}, // 24K
    {0x0080, 0x0180}, // 32K
    {0x0080, 0x0100}, // 44.1K
    {0x0080, 0x0100}, // 48K
};

#if 0
PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB57B, 0xFB9F}, // 8K
    {0xA6C4, 0x615F}, // 11.025K
    {0xB57B, 0xFB9F}, // 12K
    {0xB57B, 0xFB9F}, // 16K
    {0xA6CE, 0x30AF}, // 22.05K
    {0xB57B, 0xFB9F}, // 24K
    {0xB57B, 0xFB9F}, // 32K
    {0xA6CE, 0x30AF}, // 44.1K
    {0xB57B, 0xFB9F}, // 48K
};
#endif
//pll change
  PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T  g_audCodecFreqRegVals8808[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB851, 0xEB8F, 0x0081, 0x8021}, // 8K
    {0xB4A2, 0x33AF, 0x0001, 0x801d}, // 11.025K
    {0xB851, 0xEB8F, 0x0081, 0x8015}, // 12K
    {0xC49B, 0xA5EF, 0x0001, 0x8015}, // 16K
    {0xB4A2, 0x33AF, 0x0001, 0x800d}, // 22.05K
    {0xB851, 0xEB8F, 0x0081, 0x8009}, // 24K
    {0xC49B, 0xA5EF, 0x0001, 0x8009}, // 32K
    {0xB4A2, 0x33AF, 0x0001, 0x8005}, // 44.1K
    {0xC49B, 0xA5EF, 0x0001, 0x8005}, // 48K
};

PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals8808s[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB851, 0xEB8F, 0x0001, 0x80b1}, // 8K
    {0xB4A2, 0x33AF, 0x0001, 0x807d}, // 11.025K
    {0xB851, 0xEB8F, 0x0001, 0x8075}, // 12K
    {0xC49B, 0xA5EF, 0x0001, 0x805d}, // 16K
    {0xB4A2, 0x33AF, 0x0001, 0x803d}, // 22.05K
    {0xB851, 0xEB8F, 0x0001, 0x8039}, // 24K
    {0xC49B, 0xA5EF, 0x0001, 0x802d}, // 32K
    {0xB4A2, 0x33AF, 0x0001, 0x801d}, // 44.1K
    {0xC49B, 0xA5EF, 0x0001, 0x801d}, // 48K
};

PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T *g_audCodecFreqRegVals = NULL;
//pll change end


PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqDefaultRegValues = {0xC000, 0x000F};
#endif // SWITCH_AUDIO_CODEC_FREQ

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
     
#if 0     
PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB57B, 0x0FB9}, // 8K
    {0xA6C4, 0x0615}, // 11.025K
    {0xB57B, 0x0FB9}, // 12K
    {0xB57B, 0x0FB9}, // 16K
    {0xA6CE, 0x030A}, // 22.05K
    {0xB57B, 0x0FB9}, // 24K
    {0xB57B, 0x0FB9}, // 32K
    {0xA6CE, 0x030A}, // 44.1K
    {0xB57B, 0x0FB9}, // 48K
};
#endif

PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB851, 0x0EB8, 0x0001, 0xc0b1}, // 8K
    {0xB4A2, 0x033A, 0x0001, 0xc07d}, // 11.025K
    {0xB851, 0x0EB8, 0x0001, 0xc075}, // 12K
    {0xC49B, 0x0A5E, 0x0001, 0xc05d}, // 16K
    {0xB4A2, 0x033A, 0x0001, 0xc03d}, // 22.05K
    {0xB851, 0x0EB8, 0x0001, 0xc039}, // 24K
    {0xC49B, 0x0A5E, 0x0001, 0xc02d}, // 32K
    {0xB4A2, 0x033A, 0x0001, 0xc01d}, // 44.1K
    {0xC49B, 0x0A5E, 0x0001, 0xc01d}, // 48K
};

#if defined(AUD_3_IN_1_SPK) || defined(ABB_HP_DETECT)
#if 1
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectFmRegVals = 
    {0x7df4, 0x0000, 0xc808, 0x3fba, 0x7580, 0xa93f};
#else
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectFmRegVals = 
    {0x7df4, 0x0000, 0x080e, 0x3f8d, 0x1a00, 0x613f};
#endif
#endif

#ifdef AUD_3_IN_1_SPK
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_aud3In1SpkMp3RegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xf5c3, 0x0028, 0x2962, 0x3567, 0xce30, 0x522a}, // 8K
    {0xb76b, 0x001d, 0xb988, 0x33f8, 0xf170, 0xb627}, // 11.025K
    {0x4e82, 0x001b, 0x996c, 0x34cd, 0x9b20, 0xfe29}, // 12K
    {0x7ae1, 0x0014, 0x791b, 0x3744, 0x88e0, 0x632e}, // 16K
    {0xdad6, 0x000e, 0xa8d4, 0x3970, 0xe150, 0x1332}, // 22.05K
    {0xa741, 0x000d, 0x28c4, 0x39ed, 0xda30, 0x3333}, // 24K
    {0x3d71, 0x000a, 0x4896, 0x3b57, 0xae80, 0x2736}, // 32K
    {0x6d6b, 0x0007, 0xb86e, 0x3c8d, 0x1b60, 0xdb39}, // 44.1K
    {0xd3a0, 0x0006, 0xf866, 0x3cd1, 0xa3e0, 0x3d39}, // 48K
};
#endif // AUD_3_IN_1_SPK

#ifdef ABB_HP_DETECT
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectMp3RegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xf5c3, 0x0028, 0xe828, 0x3ebe, 0x7dc0, 0x643d}, // 8K
    {0xb76b, 0x001d, 0xd81d, 0x3f15, 0x2ba0, 0x683e}, // 11.025K
    {0x4e82, 0x001b, 0x980d, 0x3f93, 0x2730, 0xaa3f}, // 12K
    {0x7ae1, 0x0014, 0xe814, 0x3f5d, 0xbbd0, 0x533e}, // 16K
    {0xdad6, 0x000e, 0x280e, 0x3f8a, 0x1440, 0xc53f}, // 22.05K
    {0xa741, 0x000d, 0x980d, 0x3f93, 0x2730, 0x943f}, // 24K
    {0x3d71, 0x000a, 0x980a, 0x3fae, 0x5d20, 0x323f}, // 32K
    {0x6d6b, 0x0007, 0x280e, 0x3f8a, 0x1440, 0xbf3f}, // 44.1K
    {0xd3a0, 0x0006, 0xa806, 0x3fc9, 0x9340, 0xce3f}, // 48K
};
#endif // ABB_HP_DETECT
#endif // 8809 or later

#else
PRIVATE CONST AUD_SAMPLE_RATE_REG_VAL_T
    g_audNormalSampleRateRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
    {0x0080, 0x0659}, // 8K
    {0x5087, 0x049a}, // 11.025K
    {0x5087, 0x043b}, // 12K
    {0x8084, 0x032c}, // 16K
    {0x5085, 0x024d}, // 22.05K
    {0x6086, 0x021d}, // 24K
    {0x4081, 0x0196}, // 32K
    {0x6082, 0x0126}, // 44.1K
    {0xd083, 0x010e}, // 48K
#else // 8809 or later
    {0x0000, 0x0600}, // 8K
    {0x0007, 0x0400}, // 11.025K
    {0x0007, 0x0400}, // 12K
    {0x0004, 0x0300}, // 16K
    {0x0005, 0x0200}, // 22.05K
    {0x0006, 0x0200}, // 24K
    {0x0001, 0x0180}, // 32K
    {0x0002, 0x0100}, // 44.1K
    {0x0003, 0x0100}, // 48K
#endif
};

#ifdef SWITCH_AUDIO_CODEC_FREQ
PRIVATE CONST AUD_SAMPLE_RATE_REG_VAL_T
    g_audSwFreqModeSampleRateRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0x0080, 0x0600}, // 8K
    {0x0080, 0x0400}, // 11.025K
    {0x0080, 0x0400}, // 12K
    {0x0080, 0x0300}, // 16K
    {0x0080, 0x0200}, // 22.05K
    {0x0080, 0x0200}, // 24K
    {0x0080, 0x0180}, // 32K
    {0x0080, 0x0100}, // 44.1K
    {0x0080, 0x0100}, // 48K
};

PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB57B, 0xFB9F}, // 8K
    {0xA6C4, 0x615F}, // 11.025K
    {0xB57B, 0xFB9F}, // 12K
    {0xB57B, 0xFB9F}, // 16K
    {0xA6CE, 0x30AF}, // 22.05K
    {0xB57B, 0xFB9F}, // 24K
    {0xB57B, 0xFB9F}, // 32K
    {0xA6CE, 0x30AF}, // 44.1K
    {0xB57B, 0xFB9F}, // 48K
};

PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqDefaultRegValues = {0xC000, 0x000F};
#endif // SWITCH_AUDIO_CODEC_FREQ

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
PRIVATE CONST AUD_CODEC_FREQ_REG_VAL_T
    g_audCodecFreqRegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xB57B, 0x0FB9}, // 8K
    {0xA6C4, 0x0615}, // 11.025K
    {0xB57B, 0x0FB9}, // 12K
    {0xB57B, 0x0FB9}, // 16K
    {0xA6CE, 0x030A}, // 22.05K
    {0xB57B, 0x0FB9}, // 24K
    {0xB57B, 0x0FB9}, // 32K
    {0xA6CE, 0x030A}, // 44.1K
    {0xB57B, 0x0FB9}, // 48K
};

#if defined(AUD_3_IN_1_SPK) || defined(ABB_HP_DETECT)
#if 1
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectFmRegVals = 
    {0x7df4, 0x0000, 0xc808, 0x3fba, 0x7580, 0xa93f};
#else
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectFmRegVals = 
    {0x7df4, 0x0000, 0x080e, 0x3f8d, 0x1a00, 0x613f};
#endif
#endif

#ifdef AUD_3_IN_1_SPK
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_aud3In1SpkMp3RegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
#if 1 // notch = xxx hz , @20130603
    {0xf5c3, 0x0028, 0x298B, 0x33A2, 0x4440, 0xEB27}, // 8K
    {0xb76b, 0x001d, 0x992D, 0x3698, 0x3120, 0x082D}, // 11.025K
    {0x4e82, 0x001b, 0x7917, 0x3744, 0x88E0, 0x892E}, // 12K
    {0x7ae1, 0x0014, 0xC8D8, 0x393D, 0x7B70, 0x5632}, // 16K
    {0xdad6, 0x000e, 0xA8A1, 0x3AF5, 0xEB40, 0x5335}, // 22.05K
    {0xa741, 0x000d, 0x4895, 0x3B57, 0xAE80, 0x1E36}, // 24K
    {0x3d71, 0x000a, 0xA871, 0x3C71, 0xE340, 0xCF38}, // 32K
    {0x6d6b, 0x0007, 0x9853, 0x3D61, 0xC310, 0xD03A}, // 44.1K
    {0xd3a0, 0x0006, 0x084D, 0x3D96, 0x2C10, 0x413B}, // 48K
#elif 0 // notch = 250hz
	{0xf5c3, 0x0028, 0x7939, 0x3744, 0x88e0, 0x6e2e}, // 8K
	{0xb76b, 0x001d, 0xa8e4, 0x3970, 0xe150, 0x8B32}, // 11.025K
	{0x4e82, 0x001b, 0x28D2, 0x39ED, 0xDA30, 0x3933}, // 12K
	{0x7ae1, 0x0014, 0x489E, 0x3B57, 0xAE80, 0x3D36}, // 16K
	{0xdad6, 0x000e, 0xB873, 0x3C8D, 0x1B60, 0x3439}, // 22.05K
	{0xa741, 0x000d, 0xF869, 0x3CD1, 0xA3E0, 0xEC39}, // 24K
	{0x3d71, 0x000a, 0x084F, 0x3D96, 0x2C10, 0x9F3B}, // 32K
	{0x6d6b, 0x0007, 0x0839, 0x3E3B, 0x7600, 0xE33C}, // 44.1K
	{0xd3a0, 0x0006, 0xE835, 0x3E5E, 0xBDC0, 0x353C}, // 48K
#elif 0 // notch = 170 hz
    {0xf5c3, 0x0028, 0xB9C6, 0x3241, 0x8370, 0x1A24}, // 8K
    {0xb76b, 0x001d, 0x195A, 0x3570, 0xE030, 0x022A}, // 11.025K
    {0x4e82, 0x001b, 0x7941, 0x362B, 0x56F0, 0x6E2C}, // 12K
    {0x7ae1, 0x0014, 0x08F9, 0x3858, 0xB000, 0x0430}, // 16K
    {0xdad6, 0x000e, 0xD8B9, 0x3A42, 0x85A0, 0xD634}, // 22.05K
    {0xa741, 0x000d, 0x88AB, 0x3AB0, 0x6100, 0xCC35}, // 24K
    {0x3d71, 0x000a, 0x3883, 0x3BEF, 0xDE50, 0x2C37}, // 32K
    {0x6d6b, 0x0007, 0x8860, 0x3CFF, 0xFF10, 0xA239}, // 44.1K
    {0xd3a0, 0x0006, 0x5859, 0x3D3B, 0x7690, 0x133A}, // 48K
#elif 1 // notch = 150 hz
    {0xf5c3, 0x0028, 0x9A16, 0x2FA3, 0x4730, 0x1F1F}, // 8K
    {0xb76b, 0x001d, 0xA99C, 0x334B, 0x9740, 0x8A26}, // 11.025K
    {0x4e82, 0x001b, 0x9980, 0x3425, 0x4B10, 0x7428}, // 12K
    {0x7ae1, 0x0014, 0xD92C, 0x36B2, 0x6590, 0xB02D}, // 16K
    {0xdad6, 0x000e, 0xD8E2, 0x38FA, 0xF5A0, 0x5031}, // 22.05K
    {0xa741, 0x000d, 0x68D1, 0x397E, 0xFCC0, 0x9F32}, // 24K
    {0x3d71, 0x000a, 0xF8A0, 0x3AFE, 0xFDE0, 0xF335}, // 32K
    {0x6d6b, 0x0007, 0x6877, 0x3C4A, 0x94D0, 0x2438}, // 44.1K
    {0xd3a0, 0x0006, 0x886D, 0x3C93, 0x26F0, 0xF039}, // 48K
#endif};
#endif // AUD_3_IN_1_SPK

#ifdef ABB_HP_DETECT
PRIVATE CONST HP_DETECT_REG_VAL_T
    g_audHpDetectMp3RegVals[AUD_SAMPLE_RATE_INDEX_QTY] = 
{
    {0xf5c3, 0x0028, 0xe828, 0x3ebe, 0x7dc0, 0x643d}, // 8K
    {0xb76b, 0x001d, 0xd81d, 0x3f15, 0x2ba0, 0x683e}, // 11.025K
    {0x4e82, 0x001b, 0x980d, 0x3f93, 0x2730, 0xaa3f}, // 12K
    {0x7ae1, 0x0014, 0xe814, 0x3f5d, 0xbbd0, 0x533e}, // 16K
    {0xdad6, 0x000e, 0x280e, 0x3f8a, 0x1440, 0xc53f}, // 22.05K
    {0xa741, 0x000d, 0x980d, 0x3f93, 0x2730, 0x943f}, // 24K
    {0x3d71, 0x000a, 0x980a, 0x3fae, 0x5d20, 0x323f}, // 32K
    {0x6d6b, 0x0007, 0x280e, 0x3f8a, 0x1440, 0xbf3f}, // 44.1K
    {0xd3a0, 0x0006, 0xa806, 0x3fc9, 0x9340, 0xce3f}, // 48K
};
#endif // ABB_HP_DETECT
#endif // 8809 or later

#endif
PRIVATE HAL_AIF_FREQ_T g_audCodecSampleRate = HAL_AIF_FREQ_8000HZ;

PRIVATE BOOL g_audCodecVoiceQuality = FALSE;

PRIVATE HAL_AUD_USER_T g_audCodecCurrentUser = HAL_AUD_USER_NULL;

#ifdef AUDIO_CLOSE_DEALY
PRIVATE HAL_AUD_USER_T g_audHalCurrentUser = HAL_AUD_USER_NULL;
#endif

CONST HAL_AIF_CONFIG_T *g_audCodecCurrentAifCfgPtr = NULL;

PRIVATE CONST HAL_AIF_CONFIG_T g_audCodecToneAifCfg =
{
    .interface = HAL_AIF_IF_PARALLEL_STEREO,
    .sampleRate = HAL_AIF_FREQ_8000HZ,
    .channelNb = HAL_AIF_MONO,
    .serialCfg = NULL,
};

PRIVATE BOOL g_audCodecMicInput = FALSE;

PUBLIC UINT8 g_audFirstOpen = AUD_FIRST_OPEN_OFF;

#if (AUDIO_CALIB_VER == 1)
PRIVATE INT16 g_audCodecOutAlgGainDb2Val = 0;
#endif


// =============================================================================
//  FUNCTIONS
// =============================================================================
PRIVATE VOID aud_CodecCommonOutAlgGainDb2Val(INT8 alg)
{
    switch(alg)
    {
        case -45: //mute
            g_audCodecOutAlgGainDb2Val = 0;
            break;
        case -42: //level1 -42db
            g_audCodecOutAlgGainDb2Val = 130;
            break;
        case -39: //level2 -39db
            g_audCodecOutAlgGainDb2Val = 184;
            break;
        case -36: //level3 -36db
            g_audCodecOutAlgGainDb2Val = 260;
            break;
        case -33: //level4 -33db
            g_audCodecOutAlgGainDb2Val = 367;
            break;
        case -30: //level5 -30db
            g_audCodecOutAlgGainDb2Val = 518;
            break;
        case -27: //level6 -27db
            g_audCodecOutAlgGainDb2Val = 732;
            break;
        case -24: //level7 -24db
            g_audCodecOutAlgGainDb2Val = 1034;
            break;
        case -21: //level8 -21db
            g_audCodecOutAlgGainDb2Val = 1460;
            break;
        case -18: //level9 -18db
            g_audCodecOutAlgGainDb2Val = 2063;
            break;
        case -15: //level10 -15db
            g_audCodecOutAlgGainDb2Val = 2914;
            break;
        case -12: //level11 -12db
            g_audCodecOutAlgGainDb2Val = 4115;
            break;
        case -9: //level12 -9db
            g_audCodecOutAlgGainDb2Val = 5813;
            break;
        case -6: //level13 -6db
            g_audCodecOutAlgGainDb2Val = 8211;
            break;
        case -3: //level14 -3db
            g_audCodecOutAlgGainDb2Val = 11599;
            break;
        case 0: //level15 0db
            g_audCodecOutAlgGainDb2Val = 16384;
            break;
        default:
            g_audCodecOutAlgGainDb2Val = 16384;
            break;
    }

    return;
}
PRIVATE VOID aud_CodecCommonSetRcvGain(INT32 ana, INT32 dac)
{
      UINT32 anaGain = 0;
      UINT32 dacVol  = 0;
      UINT32 oldAnaGain = 0;
      INT32 target  = 0;
      INT32 origin = 0;
	  
    #if defined(GALLITE_IS_8806)
    UINT32 xcvChipId = rfd_XcvGetChipId();
    if (! (xcvChipId == GALLITE_CHIP_ID_8806_U03 ||
        xcvChipId == GALLITE_CHIP_ID_8806_U05) )
    {
         if(ana>=6)
         {
            ana-=6;
          }
         else if(ana >= 3)
         {
            ana =ana -3;
            dac =dac - 3;
          }
         else
         {
            ana = 0;
            dac =dac - 6;
         }
     }
    #endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809) 

#ifndef AUD_DONT_ADPATER_8809U06_RCVGAIN
        // U06 or later
    if (hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) >= HAL_SYS_CHIP_METAL_ID_U06)
    {
       if(ana>=6)
       {
	   	ana-=6;
       }
	 else if(ana >= 3)
	 {
	       ana =ana -3;
	       dac =dac - 3;
	 
	 }
	 else
	 {
	       ana = 0;
	       dac =dac - 6;
	 }
	 
    }
#endif

#endif

    anaGain = hal_AudRcvGainDb2Val(ana);
    dacVol = hal_AudDacGainDb2Val(dac);

    oldAnaGain = (INT32)hal_AudGetRcvGain();
    target = (INT32)(anaGain >> 1);
    origin = (INT32)(oldAnaGain >> 1);

    AUD_TRACE(AUD_INFO_TRC, 0,
            "aud_CodecCommonSetRcvGain, ana=%d(%d), dac=%d(%d), oldAna=%d",  
             							anaGain, ana, dacVol,dac, oldAnaGain);
	
    if (origin > target)
    {
        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
        COS_Sleep(1);

        for (origin--; origin>=target; origin--)
        {
            hal_AudSetRcvGain((UINT32)(origin<<1));
            COS_Sleep(1 );
        }
    }
    else
    {
        for (origin++; origin<=target; origin++)
        {
            hal_AudSetRcvGain((UINT32)(origin<<1));
            COS_Sleep(1);
        }

        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
    }
}

PRIVATE VOID aud_CodecCommonSetHeadGain(INT32 ana, INT32 dac)
{
    UINT32 anaGain = 0;
    UINT32 dacVol  = 0;
    UINT32 oldAnaGain = 0;
    INT32 target  = 0;
    INT32 origin = 0;

    #if defined(GALLITE_IS_8806)
    UINT32 xcvChipId = rfd_XcvGetChipId();
    if (! (xcvChipId == GALLITE_CHIP_ID_8806_U03 ||
        xcvChipId == GALLITE_CHIP_ID_8806_U05) )
    {
         if(ana>=6)
         {
            ana-=6;
          }
         else if(ana >= 3)
         {
            ana =ana -3;
            dac =dac - 3;
          }
         else
         {
            ana = 0;
            dac =dac - 6;
         }
     }
    #endif

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
    //mute
    if(!(ana == 0 && dac == 0))
#endif
	{
	    anaGain = hal_AudHeadGainDb2Val(ana);
	    dacVol = hal_AudDacGainDb2Val(dac);
	}
    oldAnaGain = hal_AudGetHeadGain();
    target = (INT32)(anaGain >> 1);
    origin = (INT32)(oldAnaGain >> 1);

    AUD_TRACE(AUD_INFO_TRC, 0,
            "aud_CodecCommonSetHeadGain, ana=%d(%d), dac=%d(%d), oldAna=%d",  
             							anaGain, ana, dacVol,dac, oldAnaGain);


    if (origin > target)
    {
        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
        COS_Sleep(1);

        for (origin--; origin>=target; origin--)
        {
            hal_AudSetHeadGain((UINT32)(origin<<1));
            COS_Sleep(1);
        }
    }
    else
    {
        for (origin++; origin<=target; origin++)
        {
            hal_AudSetHeadGain((UINT32)(origin<<1));
            COS_Sleep(1);
        }

        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
    }
}

PRIVATE VOID aud_CodecCommonSetSpkCfg(BOOL mute, INT32 ana, INT32 dac)
{
    #if defined(GALLITE_IS_8806)
    UINT32 xcvChipId = rfd_XcvGetChipId();
    if (! (xcvChipId == GALLITE_CHIP_ID_8806_U03 ||
        xcvChipId == GALLITE_CHIP_ID_8806_U05) )
    {
         if(ana>=6)
         {
            ana-=6;
          }
         else if(ana >= 3)
         {
            ana =ana -3;
            dac =dac - 3;
          }
         else
         {
            ana = 0;
            dac =dac - 6;
         }
     }
    #endif
    HAL_AUD_SPK_CFG_T spkCfg;
    spkCfg.noSpkMute = !mute;
    spkCfg.spkGain = hal_AudSpkGainDb2Val(ana);
    UINT32 dacVol = hal_AudDacGainDb2Val(dac);

    if (!spkCfg.noSpkMute)
    {
        spkCfg.spkGain = 0;
    }
    HAL_AUD_SPK_CFG_T oldCfg = hal_AudGetSpkCfg();
    if (!oldCfg.noSpkMute)
    {
        oldCfg.spkGain = 0;
    }
    INT32 target = (INT32)(spkCfg.spkGain >> 1);
    INT32 origin = (INT32)(oldCfg.spkGain >> 1);


    AUD_TRACE(AUD_INFO_TRC, 0,
            "aud_CodecCommonSetSpkCfg, ana=%d(%d), dac=%d(%d), oldAna=%d",  
             							spkCfg.spkGain, ana, dacVol,dac, oldCfg.spkGain);

    if (origin > target)
    {
        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
        COS_Sleep(1);

        for (origin--; origin>=target; origin--)
        {
            oldCfg.spkGain = (UINT32)(origin<<1);
            hal_AudSetSpkCfg(oldCfg);
            COS_Sleep(1);
        }
        if (!spkCfg.noSpkMute && oldCfg.noSpkMute)
        {
            // Mute at last
            oldCfg.noSpkMute = FALSE;
            hal_AudSetSpkCfg(oldCfg);
        }
    }
    else
    {
        if (spkCfg.noSpkMute && !oldCfg.noSpkMute)
        {
            // Unmute at first
            oldCfg.noSpkMute = TRUE;
            hal_AudSetSpkCfg(oldCfg);
            COS_Sleep(1);
        }
        for (origin++; origin<=target; origin++)
        {
            oldCfg.spkGain = (UINT32)(origin<<1);
            hal_AudSetSpkCfg(oldCfg);
            COS_Sleep(1);
        }
        if (!spkCfg.noSpkMute && oldCfg.noSpkMute)
        {
            // In this case, origin = target = 0
            // Mute at last
            oldCfg.noSpkMute = FALSE;
            hal_AudSetSpkCfg(oldCfg);
        }

        // Set ABB DAC digital gain
        hal_AudSetDacVolume(dacVol);
    }
}

PRIVATE VOID aud_CodecCommonSetMicCfg(BOOL mute, INT32 ana, INT32 adc)
{
    HAL_AUD_MIC_CFG_T micCfg;
    micCfg.noMicMute = !mute;
    micCfg.micGain = hal_AudMicGainDb2Val(ana);
    UINT32 adcVol = hal_AudAdcGainDb2Val(adc);

    AUD_TRACE(AUD_INFO_TRC, 0,
            "aud_CodecCommonSetMicCfg, ana=%d(%d), adc=%d(%d)", micCfg.micGain, ana, adcVol,adc);


    // No need to change MIC gain in a sequential manner
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
    hal_AudSetMicAnaVolume(micCfg.micGain);
#else
    hal_AudSetMicCfg(micCfg);
#endif
    // Set ABB ADC digital gain
    hal_AudSetAdcVolume(adcVol);
}

// =============================================================================
// aud_CodecCommonSetConfig
// -----------------------------------------------------------------------------
/// Set the audio configuration.
/// @param isRecording Whether it is recording stream or not
/// @param user The audio user
/// @param pAifCfg Pointing to the AIF configuration to be set
/// @return TRUE if success, FALSE otherwise
// =============================================================================
PUBLIC BOOL aud_CodecCommonSetConfig(BOOL isRecording, HAL_AUD_USER_T user, CONST HAL_AIF_CONFIG_T *pAifCfg)
{
    // It is required to call StreamStart before StreamRecord
    if (isRecording && g_audCodecCurrentUser == user)
    {
        // The audio mode was set when starting stream
        return TRUE;
    }

    if (g_audCodecCurrentUser != HAL_AUD_USER_NULL)
    {
       hal_HstSendEvent(SYS_EVENT, 0x22223332);
        return FALSE;
    }

    if (user == HAL_AUD_USER_CODEC || user == HAL_AUD_USER_AIF_ONLY)
    {
        AUD_ASSERT(pAifCfg != NULL, "Missing AIF cfg");
    }

    g_audCodecCurrentAifCfgPtr = pAifCfg;
    g_audCodecMicInput = FALSE;

    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        HAL_AIF_IF_T aifIf = g_audCodecCurrentAifCfgPtr->interface;
        // Check if there is MIC input
        if (aifIf == HAL_AIF_IF_PARALLEL ||
            aifIf == HAL_AIF_IF_PARALLEL_STEREO)
        {
            g_audCodecMicInput = TRUE;
        }
    }

    // g_audCodecCurrentUser will be cleared in aud_CodecCommonClose()
    g_audCodecCurrentUser = user;

    return TRUE;
}

// =============================================================================
// aud_CodecCommonGetRateIndex
// -----------------------------------------------------------------------------
/// get index of audio sample rate according to aif sampleRate
/// @param  sampleRate The AIF sample rate
/// @return index of audio sample rate
// =============================================================================
PUBLIC AUD_SAMPLE_RATE_INDEX_T aud_CodecCommonGetRateIndex(HAL_AIF_FREQ_T sampleRate)
{
    AUD_SAMPLE_RATE_INDEX_T index = AUD_SAMPLE_RATE_INDEX_8K;

    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ :
        {
            index = AUD_SAMPLE_RATE_INDEX_8K;
            break;
        }
        case HAL_AIF_FREQ_11025HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_11_025K;
            break;
        }
        case HAL_AIF_FREQ_12000HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_12K;
            break;
        }
        case HAL_AIF_FREQ_16000HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_16K;
            break;
        }
        case HAL_AIF_FREQ_22050HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_22_05K;
            break;
        }
        case HAL_AIF_FREQ_24000HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_24K;
            break;
        }
        case HAL_AIF_FREQ_32000HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_32K;
            break;
        }
        case HAL_AIF_FREQ_44100HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_44_1K;
            break;
        }
        case HAL_AIF_FREQ_48000HZ:
        {
            index = AUD_SAMPLE_RATE_INDEX_48K;
            break;
        }
        default:
        {
            AUD_ASSERT(FALSE, "Unsupported sample rate in AUD: %s, %d", __FILE__, __LINE__);
            break;
        }
    }

    return index;
}

// =============================================================================
// aud_CodecCommonSetRateStg
// -----------------------------------------------------------------------------
/// write the correct CodecGallite register to set the sample rate using HAL types.
/// @param  sampleRate The AIF sample rate
/// @return
// =============================================================================
#if defined(SOUND_RECORDER_SUPPORT_16K)	
extern PUBLIC VOID hal_AudSetSampleRateDiv(HAL_AIF_FREQ_T sampleRate);
#endif
PUBLIC VOID aud_CodecCommonSetRateStg(HAL_AIF_FREQ_T sampleRate)
{
    AUD_SAMPLE_RATE_INDEX_T index = AUD_SAMPLE_RATE_INDEX_8K;

    index = aud_CodecCommonGetRateIndex(sampleRate);

    CONST AUD_SAMPLE_RATE_REG_VAL_T* regVals;
#ifdef SWITCH_AUDIO_CODEC_FREQ
    if (! (rfd_XcvGetChipId() == XCV_CHIP_ID_8808_V2 &&
           sampleRate == HAL_AIF_FREQ_8000HZ) )
    {
        regVals = g_audSwFreqModeSampleRateRegVals;

        if (sampleRate != HAL_AIF_FREQ_8000HZ)
        {
            UINT32 sdm2FreqHigh = g_audCodecFreqRegVals[index].sdm2FreqHigh;
            UINT32 sdm2FreqLow = g_audCodecFreqRegVals[index].sdm2FreqLow;
            if (rfd_XcvGetChipId() == XCV_CHIP_ID_8808_V2)
            {
                sdm2FreqHigh += 0x10;
            }
             rfd_XcvSwitchAudioCodecFreq(sdm2FreqHigh, sdm2FreqLow);
        }
    }
    else
#endif
    {
        regVals = g_audNormalSampleRateRegVals;
    }

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808)
    UINT16 adc_sample_sel = regVals[index].sel;
    if(g_audCodecCurrentUser == HAL_AUD_USER_FM)
    {
        // fm mode, set osr reg_2c<7:8> to 00, for fm record    
        //adc_sample_sel &= (~ABB_DIG_DAC_OSR_SEL_MASK);
        //adc_sample_sel |= ABB_DIG_DAC_OSR_SEL_64;
#ifdef CHIP_DIE_8809E
    if (hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) == HAL_SYS_CHIP_METAL_ID_8809E)
    {
        index = aud_CodecCommonGetRateIndex(HAL_AIF_FREQ_48000HZ);
    }
#endif
    }
    hal_AudSetSampleRate(adc_sample_sel, regVals[index].div);
#else
    hal_AudSetSampleRate(regVals[index].sel, regVals[index].div);
#endif
}


// =============================================================================
// aud_CodecCommonCodecPrePowerOnConfig
// -----------------------------------------------------------------------------
/// Set registers before powering on codec.
/// @return
// =============================================================================
PRIVATE VOID aud_CodecCommonCodecPrePowerOnConfig(VOID)
{
    // Set voice mode
    hal_AudSetVoiceMode(g_audCodecVoiceQuality);
    // Set sample rate
    hal_AudSetSampleRate(g_audCodecCurrentUser, g_audCodecSampleRate);
}


// =============================================================================
// aud_CodecCommonChipConfig
// -----------------------------------------------------------------------------
/// Configure the CodecGallite chip, through I2C.
/// The stream parameter is needed to know the format of the PCM 
/// thing to play. The cfg one is to know which mic, spk etc use
/// Interfaces matching table:
/// -
/// -
/// .
/// 
/// 
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return 
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonChipConfig(
                                        HAL_AIF_FREQ_T sampleRate,
                                        BOOL voice,
                                        BOOL mono)
{
    AUD_PROFILE_FUNCTION_ENTRY(aud_ChipConfig);

    if (voice)
    {
#ifndef LINEIN_RECORDER_SUPPORT
        AUD_ASSERT(sampleRate == HAL_AIF_FREQ_8000HZ,
                "AUD: CodecGallite: Voice mode only supports 8kHz frequency.");
#endif
/*
        AUD_ASSERT(mono == TRUE,
                "AUD: CodecGallite: Voice mode is mono only.");
                */
        // TODO add a bunch of assert/check
    }

    HAL_AIF_FREQ_T oldSampleRate = g_audCodecSampleRate;
    BOOL oldVoiceQuality = g_audCodecVoiceQuality;

    g_audCodecSampleRate = sampleRate;
    g_audCodecVoiceQuality = voice;

    HAL_AUD_CALLBACK_T callback = NULL;

    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        callback = &aud_CodecCommonCodecPrePowerOnConfig;
    }
    else if (g_audCodecCurrentUser == HAL_AUD_USER_FM)
    {
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
        callback = &aud_CodecCommonCodecPrePowerOnConfig;
#endif
    }
    else if (g_audCodecCurrentUser == HAL_AUD_USER_LINEIN ||
             g_audCodecCurrentUser == HAL_AUD_USER_AIF_ONLY)
    {
    }
    else
    {
        AUD_ASSERT(FALSE, "Invalid audio mode: %d", g_audCodecCurrentUser);
    }

#if defined(SOUND_RECORDER_SUPPORT_16K)	
     hal_AudSetSampleRateDiv(sampleRate);
#endif

#ifdef AUDIO_CLOSE_DEALY
        BOOL ret ;
        if(g_audHalCurrentUser != g_audCodecCurrentUser)
        {
            if(g_audHalCurrentUser != HAL_AUD_USER_NULL)
                hal_AudClose(g_audHalCurrentUser);
            ret = hal_AudOpen(g_audCodecCurrentUser, NULL);
            g_audHalCurrentUser = g_audCodecCurrentUser;
        }
#else    
    // Configure audio codec
    BOOL ret = hal_AudOpen(g_audCodecCurrentUser, callback);
    if (!ret)
    {
        g_audCodecSampleRate = oldSampleRate;
        g_audCodecVoiceQuality = oldVoiceQuality;
        AUD_PROFILE_FUNCTION_EXIT(aud_ChipConfig);
        return AUD_ERR_RESOURCE_BUSY;
    }
#endif

    AUD_PROFILE_FUNCTION_EXIT(aud_ChipConfig);
    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonAifConfig
// -----------------------------------------------------------------------------
/// Configure the AIF interface for use with the CodecGallite chip.
/// @return AUD_ERR_NO
// =============================================================================
INLINE AUD_ERR_T aud_CodecCommonAifConfig(VOID)
{
    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        if (hal_AifOpen(g_audCodecCurrentAifCfgPtr) != HAL_ERR_NO)
        {
            return AUD_ERR_RESOURCE_BUSY;
        }
    }

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonClose
// -----------------------------------------------------------------------------
/// Close the driver and configure the CodecGallite and AIF.
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonClose(VOID)
{
    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        hal_AifClose();
        g_audCodecCurrentAifCfgPtr = NULL;
    }

    if (g_audCodecCurrentUser != HAL_AUD_USER_NULL)
    {
        // Except for earpiece detection, HAL AUD is always
        // used in EDRV AUD, and is protected by EDRV AUD mutex.
        #ifndef AUDIO_CLOSE_DEALY
        while(!hal_AudClose(g_audCodecCurrentUser))
        {
            COS_Sleep(AUD_TIME_RETRY);
        }
        #endif
        g_audCodecCurrentUser = HAL_AUD_USER_NULL;
    }

#ifdef SWITCH_AUDIO_CODEC_FREQ
#ifdef __FORCE_WORK_IN_26MHZ__
    if (rfd_XcvGetChipId() == XCV_CHIP_ID_8808_V2)
    {
        // Back to normal mode
        rfd_XcvSwitchAudioCodecFreq(g_audCodecFreqDefaultRegValues.sdm2FreqHigh,
                                    g_audCodecFreqDefaultRegValues.sdm2FreqLow,
				g_audCodecFreqDefaultRegValues.reg0b,
				g_audCodecFreqDefaultRegValues.reg0f);
    }
    else
    {
        // Still in switching freq mode, but set to 8K
        rfd_XcvSwitchAudioCodecFreq(g_audCodecFreqRegVals[0].sdm2FreqHigh,
                                    g_audCodecFreqRegVals[0].sdm2FreqLow,
				g_audCodecFreqRegVals[0].reg0b,
				g_audCodecFreqRegVals[0].reg0f);
    }
#else
    if (g_audCodecSampleRate != HAL_AIF_FREQ_8000HZ)
    {
        if (rfd_XcvGetChipId() == XCV_CHIP_ID_8808_V2)
        {
            // Back to normal mode
            rfd_XcvSwitchAudioCodecFreq(g_audCodecFreqDefaultRegValues.sdm2FreqHigh,
                                        g_audCodecFreqDefaultRegValues.sdm2FreqLow);
        }
        else
        {
            // Still in switching freq mode, but set to 8K
            rfd_XcvSwitchAudioCodecFreq(g_audCodecFreqRegVals[0].sdm2FreqHigh,
                                        g_audCodecFreqRegVals[0].sdm2FreqLow);
        }
    }
#endif
#endif

    g_audCodecSampleRate = HAL_AIF_FREQ_8000HZ;
    g_audCodecVoiceQuality = FALSE;

    g_audCodecLatestCfg.spkSel = AUD_SPK_DISABLE;
    g_audCodecLatestCfg.spkType = AUD_SPEAKER_STEREO_NA;
    g_audCodecLatestCfg.micSel = AUD_MIC_DISABLE;

    g_audCodecLatestLevel.spkLevel = SND_SPK_MUTE;
    g_audCodecLatestLevel.micLevel = SND_MIC_MUTE;
    g_audCodecLatestLevel.sideLevel = SND_SIDE_MUTE;
    g_audCodecLatestLevel.toneLevel = SND_TONE_0DB;

    g_audCodecOpened = FALSE;

    return AUD_ERR_NO;
}

// =============================================================================
// aud_CodecCommonOpen
// -----------------------------------------------------------------------------
/// Open the driver and configure the CodecGallite and AIF.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonOpen(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono)
{
    AUD_ERR_T errStatus;
        
    errStatus = aud_CodecCommonChipConfig(sampleRate, voice, mono);
    if (errStatus == AUD_ERR_NO)
    {
        errStatus = aud_CodecCommonAifConfig();
        if (errStatus == AUD_ERR_NO)
        {
            g_audCodecOpened = TRUE;

            if (g_audFirstOpen == AUD_FIRST_OPEN_OFF)
            {
                g_audFirstOpen = AUD_FIRST_OPEN_ON;
            }
            else
            {
                g_audFirstOpen = AUD_FIRST_OPEN_DONE;
            }
        }
    }

    if (errStatus != AUD_ERR_NO)
    {
        aud_CodecCommonClose();
    }
    return errStatus;
}


// =============================================================================
// aud_CodecCommonSetup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker.
/// 
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_DEVICE_CFG_T for more details.
/// @return #AUD_ERR_NO if it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
{
    // Check parameters
    AUD_ASSERT(cfg->spkSel<AUD_SPK_QTY || cfg->spkSel == AUD_SPK_DISABLE,
            "AUD: Improper Spk Id:%d", cfg->spkSel);
    AUD_ASSERT(cfg->micSel<AUD_MIC_QTY || cfg->micSel == AUD_MIC_DISABLE,
            "AUD: Improper Mic Id:%d", cfg->micSel);
    AUD_ASSERT(cfg->level->spkLevel<AUD_SPK_VOL_QTY, "AUD: Improper Spk Level");
    AUD_ASSERT(cfg->level->micLevel<AUD_MIC_VOL_QTY, "AUD: Improper Mic Level");
    AUD_ASSERT(cfg->level->sideLevel<AUD_SIDE_VOL_QTY, "AUD: Improper Side Level");
    AUD_ASSERT(cfg->level->toneLevel<AUD_TONE_VOL_QTY, "AUD: Improper Tone Level: %d",cfg->level->toneLevel );

    INT8 regAnaLvl, regDacLvl, regAlgLvl;
    CONST CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
    
    AUD_ASSERT(calibPtr->aud.audioGainItfMap[itf]>=0, "AUD: No Gain Calibration for itf: %d", itf);
    CONST CALIB_AUDIO_OUT_GAINS_T *pCalibOutGains = 
        &calibPtr->aud.audioGains[(int)calibPtr->aud.audioGainItfMap[itf]].outGains[cfg->level->spkLevel];

    if (IS_APP_SND_ITF(itf))
    {
        if (aud_GetCurOutputDevice() == SND_ITF_EAR_PIECE)
        {
            regAnaLvl = pCalibOutGains->voiceOrEpAna;
            regDacLvl = pCalibOutGains->voiceOrEpDac;
			regAlgLvl = pCalibOutGains->voiceOrEpAlg;
        }
        else
        {
            regAnaLvl = pCalibOutGains->musicOrLsAna;
            regDacLvl = pCalibOutGains->musicOrLsDac;
			regAlgLvl = pCalibOutGains->musicOrLsAlg;
        }
    }
    else
    {
        if (g_audCodecVoiceQuality)
        {
            regAnaLvl = pCalibOutGains->voiceOrEpAna;
            regDacLvl = pCalibOutGains->voiceOrEpDac;
			regAlgLvl = pCalibOutGains->voiceOrEpAlg;
        }
        else
        {
            regAnaLvl = pCalibOutGains->musicOrLsAna;
            regDacLvl = pCalibOutGains->musicOrLsDac;
			regAlgLvl = pCalibOutGains->musicOrLsAlg;
        }
    }

    //hal_HstSendEvent(SYS_EVENT, 0x20161143);
    //hal_HstSendEvent(SYS_EVENT, itf);

    //hal_HstSendEvent(SYS_EVENT, cfg->level->spkLevel);
    //hal_HstSendEvent(SYS_EVENT, g_audCodecVoiceQuality);

    //hal_HstSendEvent(SYS_EVENT, regAnaLvl);
    //hal_HstSendEvent(SYS_EVENT, regDacLvl);
    //hal_HstSendEvent(SYS_EVENT, regAlgLvl);

    // For test mode AUD_TEST_RECVMIC_IN_EARPIECE_OUT
    if (g_audSetHeadMaxDigitalGain)
    {
        regDacLvl = MAX_ABB_DAC_GAIN_HEAD;
    }
	//calc alg gain
    aud_CodecCommonOutAlgGainDb2Val(regAlgLvl);

    // Check if we need to mute the original audio output channel gradually
    if ( cfg->spkSel != g_audCodecLatestCfg.spkSel ||
         (cfg->level->spkLevel == SND_SPK_MUTE &&
          g_audCodecLatestLevel.spkLevel != SND_SPK_MUTE) )
    {
#if 0    
        switch (g_audCodecLatestCfg.spkSel)
        {
            case AUD_SPK_RECEIVER:
            {
                aud_CodecCommonSetRcvGain(0, 0);
                break;
            }
            case AUD_SPK_EAR_PIECE:
            {
                aud_CodecCommonSetHeadGain(0, 0);
                break;
            }
            case AUD_SPK_LOUD_SPEAKER:
            {
                aud_CodecCommonSetSpkCfg(TRUE, 0, 0);
                break;
            }
            case AUD_SPK_LOUD_SPEAKER_EAR_PIECE:
            {
                // Loudspeaker
                aud_CodecCommonSetSpkCfg(TRUE, 0, 0);
                // Earpiece
                aud_CodecCommonSetHeadGain(0, 0);
                break;
            }

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_8809)
            case AUD_SPK_I2S:
            {
                hal_HstSendEvent(SYS_EVENT, 0x10250201);
                pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, FALSE);
                hwp_sysCtrl->Cfg_Reserved &= ~0x02;
                
                break;
            }
#endif
            default:
            {
                break;
            }
        }
#else
        HAL_AUD_SPK_CFG_T spkCfg;
        spkCfg.reg = 0;
        hal_AudSetSpkCfg(spkCfg);
#endif
    }

    // Check if we need to mute the original MIC channel gradually
    if ( cfg->micSel != g_audCodecLatestCfg.micSel ||
         (cfg->level->micLevel == SND_MIC_MUTE &&
          g_audCodecLatestLevel.micLevel != SND_MIC_MUTE) )
    {
        aud_CodecCommonSetMicCfg(TRUE, 0, 0);
    }

    // Enable/disable corresponding audio PAs
    HAL_AUD_SEL_T selCfg;
    selCfg.reg = 0;

    if (g_audCodecCurrentUser == HAL_AUD_USER_FM)
    {
        selCfg.fmSel = TRUE;
    }

    switch (cfg->spkSel)
    {
        case AUD_SPK_RECEIVER:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.rcvSel = FALSE;
            }
            else
            {
                selCfg.rcvSel = TRUE;
            }
            break;
        }
        case AUD_SPK_EAR_PIECE:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.headSel = FALSE;
            }
            else
            {
                selCfg.headSel = TRUE;
            }
            break;
        }
        case AUD_SPK_LOUD_SPEAKER:
        {
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.spkSel = FALSE;
            }
            else
            {
                if (cfg->spkType == AUD_SPEAKER_STEREO)
                {
                    selCfg.spkSel = TRUE;
                }
                else
                {
                    selCfg.spkMonoSel = TRUE;
                }
            }
            break;
        }
        case AUD_SPK_LOUD_SPEAKER_EAR_PIECE:
        {
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                // Loudspeaker
                selCfg.spkSel = FALSE;
                // Earpiece
                selCfg.headSel = FALSE;
            }
            else
            {
                // Loudspeaker
                if (cfg->spkType == AUD_SPEAKER_STEREO)
                {
                    selCfg.spkSel = TRUE;
                }
                else
                {
                    selCfg.spkMonoSel = TRUE;
                }
                // Earpiece
                selCfg.headSel = TRUE;
            }
            break;
        }
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_8809)
            case AUD_SPK_I2S:
            {
                hal_HstSendEvent(SYS_EVENT, 0x10250200);
                pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, TRUE);
		#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
                hwp_sysCtrl->Cfg_Reserved |= 0x02;
		#endif
            }
            break;
#endif        
        default:
        {
            break;
        }
    }

    if ( cfg->micSel == AUD_MIC_RECEIVER ||
          cfg->micSel == AUD_MIC_EAR_PIECE ||
          cfg->micSel == AUD_MIC_LOUD_SPEAKER )
    {
        switch (cfg->micSel)
        {
            case AUD_MIC_EAR_PIECE:
            {
                selCfg.auxMicSel = TRUE;
                break;
            }
            case AUD_MIC_RECEIVER:
            case AUD_MIC_LOUD_SPEAKER:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if ( cfg->spkSel != g_audCodecLatestCfg.spkSel ||
         cfg->micSel != g_audCodecLatestCfg.micSel ||
         ( cfg->spkSel == AUD_SPK_LOUD_SPEAKER &&
           cfg->spkType != g_audCodecLatestCfg.spkType ) ||
         ( (cfg->level->spkLevel == SND_SPK_MUTE || 
             g_audCodecLatestLevel.spkLevel == SND_SPK_MUTE) &&
           cfg->level->spkLevel != g_audCodecLatestLevel.spkLevel ) )
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
	    // Set audio gains mute for clean noise;
	    switch (cfg->spkSel)
	    {
	        case AUD_SPK_RECEIVER:
	        {
	                aud_CodecCommonSetRcvGain(0, 0);
	        }
	        break;

	        case AUD_SPK_EAR_PIECE:
	        {
	                aud_CodecCommonSetHeadGain(0, 0);
	        }
	        break;
	        
	        case AUD_SPK_LOUD_SPEAKER:
	        {
	            BOOL mute;

	            mute = TRUE;
	            regDacLvl = 0;

	            aud_CodecCommonSetSpkCfg(mute, regAnaLvl, regDacLvl);

	        }
	        break;
	        
	        case AUD_SPK_LOUD_SPEAKER_EAR_PIECE:
	        {
	                // Loudspeaker
	                aud_CodecCommonSetSpkCfg(TRUE, 0, 0);
	                // Earpiece
	                aud_CodecCommonSetHeadGain(0, 0);
	           	 
	        }
	        break;
	         
	        default:
	        {
	            break;
	        }
	    }
#endif
        hal_AudSetSelCfg(selCfg);
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
#ifdef ABB_HP_DETECT
        // Save the headphone state here. It will be restored when the
        // headphone key detection is disabled due to entering LP mode
        hal_AudSaveHeadphoneState(selCfg.headSel);
#endif
#endif
    }

    // Set audio gains
    switch (cfg->spkSel)
    {
        case AUD_SPK_RECEIVER:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                aud_CodecCommonSetRcvGain(0, 0);
            }
            else
            {
                aud_CodecCommonSetRcvGain(regAnaLvl, regDacLvl);
            }
            break;
        }
        case AUD_SPK_EAR_PIECE:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                aud_CodecCommonSetHeadGain(0, 0);
            }
            else
            {
                aud_CodecCommonSetHeadGain(regAnaLvl, regDacLvl);
            }
            break;
        }
        case AUD_SPK_LOUD_SPEAKER:
        {
            BOOL mute;
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                mute = TRUE;
                regDacLvl = 0;
            }
            else
            {
                mute = FALSE;
            }
            aud_CodecCommonSetSpkCfg(mute, regAnaLvl, regDacLvl);
            break;
        }
        case AUD_SPK_LOUD_SPEAKER_EAR_PIECE:
        {
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                // Loudspeaker
                aud_CodecCommonSetSpkCfg(TRUE, 0, 0);
                // Earpiece
                aud_CodecCommonSetHeadGain(0, 0);
            }
            else
            {
                // Loudspeaker
                aud_CodecCommonSetSpkCfg(FALSE, regAnaLvl, regDacLvl);
                // Earpiece
                INT8 epAnaLevel, epDacLevel;
                if (IS_APP_SND_ITF(itf))
                {
                    epAnaLevel = pCalibOutGains->voiceOrEpAna;
                    epDacLevel = pCalibOutGains->voiceOrEpDac;
                }
                else
                {
                    CONST CALIB_AUDIO_OUT_GAINS_T *pEpGains;
                    pEpGains = &calibPtr->aud.audioGains[(int)calibPtr->aud.audioGainItfMap[SND_ITF_EAR_PIECE]].outGains[cfg->level->spkLevel];
                    if (g_audCodecVoiceQuality)
                    {
                        epAnaLevel = pEpGains->voiceOrEpAna;
                        epDacLevel = pEpGains->voiceOrEpDac;
                    }
                    else
                    {
                        epAnaLevel = pEpGains->musicOrLsAna;
                        epDacLevel = pEpGains->musicOrLsDac;
                    }
                }
                aud_CodecCommonSetHeadGain(epAnaLevel, epDacLevel);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    if ( cfg->micSel == AUD_MIC_RECEIVER ||
          cfg->micSel == AUD_MIC_EAR_PIECE ||
          cfg->micSel == AUD_MIC_LOUD_SPEAKER )
    {
        BOOL mute = TRUE;
        INT32 anaVol = 0;
        INT32 adcVol = 0;
        if (cfg->level->micLevel != SND_MIC_MUTE)
        {
            mute = FALSE;

            CALIB_AUDIO_IN_GAINS_T inGainsCall;
            inGainsCall = calibPtr->aud.audioGains[(int)calibPtr->aud.audioGainItfMap[itf]].inGainsCall;
            anaVol = inGainsCall.ana;
            adcVol = inGainsCall.adc;

            // For test mode AUD_TEST_RECVMIC_IN_EARPIECE_OUT
            if (g_audSetMicMaxGain)
            {
                anaVol = 21;
                adcVol = MAX_ABB_ADC_GAIN;
            }
        }
        aud_CodecCommonSetMicCfg(mute, anaVol, adcVol);
    }

    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        // Side tone volume controlled in the AIF. Normal use or loop test use.
        if (AUD_SIDE_VOL_TEST == cfg->level->sideLevel)
        {
            // If in loop test mode, the side tone and mic are set to
            // large values (Note: the max values might cause echo or
            // squeaking noise)
            aud_CodecCommonSetMicCfg(FALSE, 18, MAX_ABB_ADC_GAIN - 3);
            hal_AifSetSideTone(10);
        }
        else
        {
            hal_AifSetSideTone(0);//hal_AifSideToneGainDb2Val(calibPtr->aud.audioGains[calibPtr->aud.gainItfMap[itf]].sideTone[cfg->level->spkLevel]));
        }
    }

    // Record last config
    g_audCodecLatestCfg.spkSel = cfg->spkSel;
    g_audCodecLatestCfg.spkType = cfg->spkType;
    g_audCodecLatestCfg.micSel = cfg->micSel;
    g_audCodecLatestLevel = *(cfg->level);

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonStreamStart
// -----------------------------------------------------------------------------
/// The common codes to start the playing of a stream. 
/// 
/// This function  outputs the 
/// audio on the audio interface specified as a parameter, on the output selected in 
/// the #aud_Setup function. \n
/// In normal operation, when 
/// the buffer runs out, the playing will wrap at the beginning. Here, there are two ways 
/// you can handle your buffer: HAL can call a user function at the middle or 
/// at the end of the playback or, depending 
/// on your application, you can simply poll the playing state using the "reached half"
/// and "reached end functions" TODO Ask if needed, and then implement !
/// 
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamStart(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audCodecPlaying)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audCodecOpened)
    {
        errStatus = aud_CodecCommonOpen(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }

    if (g_audFirstOpen == AUD_FIRST_OPEN_ON && cfg->spkSel == AUD_SPK_EAR_PIECE)
    {
        COS_Sleep(AUD_EARPIECE_PA_POWER_STABLE_TIME);
        g_audFirstOpen = AUD_FIRST_OPEN_DONE;
    }

    errStatus = aud_CodecCommonSetup(itf, cfg);

    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        if (errStatus == AUD_ERR_NO)
        {
#if (VCOM_BAT_LARGE_CAP)
            // If the capacitor connected to the loud speaker is large (larger
            // than 1uF), it might take an extra long time to be charged,
            // and then the audio PA can work. If the sound being played is
            // very short (e.g., camera shutter sound), it might NOT be
            // output via the loud speaker.
            // Workaround: Wait for a while before playing the sound.
            if (itf == SND_ITF_LOUD_SPEAKER)
            {
                COS_Sleep(300);
            }
#endif // VCOM_BAT_LARGE_CAP
            // Send the stream through the IFC
            if (hal_AifPlayStream(stream) != HAL_ERR_NO)
            {
                AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                errStatus = AUD_ERR_RESOURCE_BUSY;
            }
        }
    } // (g_audCodecCurrentAifCfgPtr != NULL)

    if (errStatus == AUD_ERR_NO)
    {
        needCloseAud = FALSE;
        g_audCodecPlaying = TRUE;
    }

    if (needCloseAud)
    {
        aud_CodecCommonClose();
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonStreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamStop(SND_ITF_T itf)
{
    if (g_audCodecPlaying)
    {
        if (g_audCodecCurrentAifCfgPtr != NULL)
        {
            hal_AifStopPlay();
        }
        g_audCodecPlaying = FALSE;
    }

    if (g_audCodecRecording)
    {
        if (g_audCodecCurrentAifCfgPtr != NULL)
        {
            hal_AifStopRecord();
        }
        if (g_audCodecMicInput)
        {
            pmd_EnablePower(PMD_POWER_MIC, FALSE);
        }
        g_audCodecRecording = FALSE;
    }

    if (g_audCodecOpened)
    {
        // Gradually mute audio channels
        g_audCodecLatestLevel.spkLevel = SND_SPK_MUTE;
        g_audCodecLatestLevel.micLevel = SND_MIC_MUTE;
        g_audCodecLatestLevel.sideLevel = SND_SIDE_MUTE;
        g_audCodecLatestLevel.toneLevel = SND_TONE_0DB;
        aud_CodecCommonSetup(itf, &g_audCodecLatestCfg);
        // Close audio codec
        return aud_CodecCommonClose();
    }
    else
    {
        return AUD_ERR_NO;
    }
}


// =============================================================================
// aud_CodecCommonStreamPause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamPause(SND_ITF_T itf, BOOL pause)
{
    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        if (hal_AifPause(pause) == HAL_ERR_NO)
        {
            return AUD_ERR_NO;
        }
    }

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonStreamRecord
// -----------------------------------------------------------------------------
/// The common codes to manage the recording of a stream. 
/// 
///
/// The #aud_StreamRecord send a message to the driver which begins the 
/// dataflow from the audio and the compression. If the function returns \c 
/// FALSE, it means that the driver cannot handle the record. The value \c len 
/// should be the maximum size of the recording buffer and when the buffer is 
/// full, the recording will automatically wrap. A 
/// user function can be called, either when the middle or the end of the 
/// buffer is reached. (TODO add iatus about polling if available)
/// 
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audCodecRecording)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audCodecOpened)
    {
        errStatus = aud_CodecCommonOpen(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }

    errStatus = aud_CodecCommonSetup(itf, cfg);

    if (g_audCodecCurrentAifCfgPtr != NULL)
    {
        if (errStatus == AUD_ERR_NO)
        {
            // Send the stream through the IFC
            if (hal_AifRecordStream(stream) != HAL_ERR_NO)
            {
                AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                errStatus = AUD_ERR_RESOURCE_BUSY;
            }
        }
    }

    if (errStatus == AUD_ERR_NO)
    {
        if (g_audCodecMicInput)
        {
            pmd_EnablePower(PMD_POWER_MIC, TRUE);
        }
        needCloseAud = FALSE;
        g_audCodecRecording = TRUE;
    }

    if (needCloseAud)
    {
        aud_CodecCommonClose();
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonTone
// -----------------------------------------------------------------------------
//  Manage the playing of a tone: DTMF or Comfort Tone. 
/// 
/// Outputs a DTMF or comfort tone
///
/// When the audio output is enabled, a DTMF or a comfort tones can be output 
/// as well. This function starts the output of a tone generated in the audio 
/// module. \n
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param tone The tone to generate
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audCodecCurrentUser != HAL_AUD_USER_NULL   &&
        g_audCodecCurrentUser != HAL_AUD_USER_CODEC&&
        (itf == SND_ITF_BLUETOOTH                                       &&
          g_audCodecCurrentUser != HAL_AUD_USER_AIF_ONLY))
    {
         // The tone cannot be played unless the user is null (for tone in idle screen) or codec.
        return AUD_ERR_NO;
    }

    if (!g_audCodecOpened && start)
    {
        // Open the codec
        // FIXME Use the stream phantom !
        if (!aud_CodecCommonSetConfig(FALSE, HAL_AUD_USER_CODEC, &g_audCodecToneAifCfg))
        {
            return AUD_ERR_RESOURCE_BUSY;
        }
        errStatus = aud_CodecCommonOpen(HAL_AIF_FREQ_8000HZ, TRUE, TRUE);
        needCloseAud = TRUE;
    }

    if (start)
    {
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_CodecCommonSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                hal_AifTone(tone, cfg->level->toneLevel, TRUE);
                needCloseAud = FALSE;
            }
        }

        if (needCloseAud)
        {
            aud_CodecCommonClose();
        }
    }
    else
    {
        // Stop tone anyway
        hal_AifTone(0, 0, FALSE);
        if(!g_audCodecPlaying && !g_audCodecRecording)
        {
            // Gradually mute audio channels
            g_audCodecLatestLevel.spkLevel = SND_SPK_MUTE;
            g_audCodecLatestLevel.micLevel = SND_MIC_MUTE;
            g_audCodecLatestLevel.sideLevel = SND_SIDE_MUTE;
            g_audCodecLatestLevel.toneLevel = SND_TONE_0DB;
            aud_CodecCommonSetup(itf, &g_audCodecLatestCfg);
            // Close audio codec
            aud_CodecCommonClose();
        }
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonTonePause
// -----------------------------------------------------------------------------
/// This function pauses the audio tone.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonTonePause(SND_ITF_T itf, BOOL pause)
{
    switch (hal_AifTonePause(pause))
    {
        case HAL_ERR_NO:
        default:
            return AUD_ERR_NO;
            break;
    }
}


// =============================================================================
// aud_CodecCommonCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonCalibUpdateValues(SND_ITF_T itf)
{
    // FIXME --> Open the i2c before ...
//    return aud_CodecCommonSetup(itf, &g_audCodecLatestCfg);
    return HAL_ERR_NO;
}

#if (AUDIO_CALIB_VER == 1)

// =============================================================================
// aud_CodecCommonGetLatestLevel
// -----------------------------------------------------------------------------
/// get the audio codec Latest level.
// =============================================================================
PUBLIC AUD_LEVEL_T* aud_CodecCommonGetLatestLevel(VOID)
{
    return (&g_audCodecLatestLevel);
}

// =============================================================================
// aud_CodecCommonGetOutAlgGainDb2Val
// -----------------------------------------------------------------------------
/// get the audio codec AlgGainDb2Val.
// =============================================================================
PUBLIC INT16 aud_CodecCommonGetOutAlgGainDb2Val(VOID)
{
    return g_audCodecOutAlgGainDb2Val;
}
#endif


