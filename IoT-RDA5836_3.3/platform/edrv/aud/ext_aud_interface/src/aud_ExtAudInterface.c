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
/// @file aud_codec_gallite.c                                                        //
/// That file provides the implementation of the AUD interface for the CodecGallite //
//  chip.                                                                     //
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#include "cs_types.h"

#include "Aud_ExtAudInterface.h"
#include "audp_debug.h"

#include "hal_aif.h"
#include "hal_error.h"
#include "hal_sys.h"

#include "hal_rda_abb.h"
#include "hal_rda_audio.h"
#include "hal_ispi.h"
#include "pmd_m.h"
#include "sxr_tls.h"
#include "calib_m.h"

#include "tgt_aud_cfg.h"


// aud_StreamStart() and aud_StreamStop(), aud_Setup() are of mutually exclusive access,
// with the help of semaphore. Then here we do not need mutex any more.


// =============================================================================
//  MACROS
// =============================================================================

#define FREQ_SAMPLE_SEL_8K (0x0080)
#define FREQ_SAMPLE_DIV_8K (0x0659)

#define FREQ_SAMPLE_SEL_11_025K (0x5087)
#define FREQ_SAMPLE_DIV_11_025K (0x049a)

#define FREQ_SAMPLE_SEL_12K (0x5087)
#define FREQ_SAMPLE_DIV_12K (0x043b)

#define FREQ_SAMPLE_SEL_16K (0x8084)
#define FREQ_SAMPLE_DIV_16K (0x032c)

#define FREQ_SAMPLE_SEL_22_05K (0x5085)
#define FREQ_SAMPLE_DIV_22_05K (0x024d)

#define FREQ_SAMPLE_SEL_24K (0x6086)
#define FREQ_SAMPLE_DIV_24K (0x021d)

#define FREQ_SAMPLE_SEL_32K (0x4081)
#define FREQ_SAMPLE_DIV_32K (0x0196)

#define FREQ_SAMPLE_SEL_44_1K (0x6082)
#define FREQ_SAMPLE_DIV_44_1K (0x0126)

#define FREQ_SAMPLE_SEL_48K (0xd083)
#define FREQ_SAMPLE_DIV_48K (0x010e)


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// AUD_ANALOG_CALIB_OUT_GAINS_T
// -----------------------------------------------------------------------------
/// This type describes how the gains are recorded in the audio calibration
/// structure for the Analog AUD driver.
// =============================================================================
typedef union
{
    /// The value read from the calibration structure.
    UINT32  calibValue;
        
    /// Calibrated gains.
    struct 
    {
        /// Calibrated gains for a voice flow.
        /// Right gain on the stereo macro for the voice flow. (Only for LS or HS).
        UINT32  voiceRight:5;
        /// Left gain on the stereo macro for the voice flow (Only for LS or HS)..
        UINT32  voiceLeft:5;
        /// Preamplifier of the voice flow before the stereo macro (Only for LS or HS)..
        UINT32  voicePreamp:3;
        /// Analog gain of the voice flow.
        UINT32  voiceAnalog:3;
        /// Digital gain of the voice flow.
        UINT32  voiceDigital:2;

        /// Calibrated gains for a music flow.
        /// Right gain on the stereo macro for the music flow
        UINT32  musicRight:5;
        /// Left gain on the stereo macro for the music flow
        UINT32  musicLeft:5;
        /// Digital gain on the stereo macro for the music flow
        UINT32  musicDigital:3;
        /// Stuff bit.
        UINT32  :1;
    } gains;
} AUD_CODEC_CALIB_OUT_GAINS_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Playing status
PRIVATE BOOL  g_audExtAudInterfacePlaying  = FALSE;

/// Recording status 
PRIVATE BOOL  g_audExtAudInterfaceRecording   = FALSE;

/// Openness status 
PRIVATE BOOL  g_audExtAudInterfaceOpened  = FALSE;

/// Remember the last settings applied
/// They will be set again to take into account any potential
/// calibration change.
PRIVATE AUD_LEVEL_T g_audCodecGalliteLatestLevel  = 
{       
        .spkLevel   = SND_SPK_MUTE,
        .micLevel   = SND_MIC_MUTE,
        .sideLevel  = SND_SIDE_MUTE,
        .toneLevel  = SND_TONE_0DB,
};

PRIVATE AUD_DEVICE_CFG_T g_audCodecGalliteLatestCfg   = 
{
        .spkSel = AUD_SPK_DISABLE,
        .spkType = AUD_SPEAKER_STEREO_NA,
        .micSel = AUD_MIC_DISABLE,
        .level  = &g_audCodecGalliteLatestLevel
};

PRIVATE HAL_AIF_FREQ_T g_audCodecGalliteSampleRate;

// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// aud_CodecGalliteeGetRateStg
// -----------------------------------------------------------------------------
/// write the correct CodecGallite register to set the sample rate using HAL types.
/// @return
// =============================================================================
PRIVATE VOID aud_CodecGalliteSetRateStg(HAL_AIF_FREQ_T sampleRate)
{
    UINT32 sel = 0;
    UINT32 div = 0;
    
    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ :
        {
		sel = FREQ_SAMPLE_SEL_8K;
              div = FREQ_SAMPLE_DIV_8K;
		break;
        }
        case HAL_AIF_FREQ_11025HZ:
        {
		sel = FREQ_SAMPLE_SEL_11_025K;
              div = FREQ_SAMPLE_DIV_11_025K;
		break;
        }
        case HAL_AIF_FREQ_12000HZ:
        {
		sel = FREQ_SAMPLE_SEL_12K;
              div = FREQ_SAMPLE_DIV_12K;
		break;
        }
        case HAL_AIF_FREQ_16000HZ:
        {
		sel = FREQ_SAMPLE_SEL_16K;
              div = FREQ_SAMPLE_DIV_16K;
		break;
        }
        case HAL_AIF_FREQ_22050HZ:
        {
		sel = FREQ_SAMPLE_SEL_22_05K;
              div = FREQ_SAMPLE_DIV_22_05K;
		break;
        }
        case HAL_AIF_FREQ_24000HZ:
        {
		sel = FREQ_SAMPLE_SEL_24K;
              div = FREQ_SAMPLE_DIV_24K;
		break;
        }
        case HAL_AIF_FREQ_32000HZ:
        {
		sel = FREQ_SAMPLE_SEL_32K;
              div = FREQ_SAMPLE_DIV_32K;
		break;
        }
        case HAL_AIF_FREQ_44100HZ:
        {
		sel = FREQ_SAMPLE_SEL_44_1K;
              div = FREQ_SAMPLE_DIV_44_1K;
		break;
        }
        case HAL_AIF_FREQ_48000HZ:
        {
		sel = FREQ_SAMPLE_SEL_48K;
              div = FREQ_SAMPLE_DIV_48K;
		break;
        }
        default:
        {
            AUD_ASSERT(FALSE, "Unsupported sample rate in AUD: %s, %d", __FILE__, __LINE__);
            break;
        }
    }

    //hal_AbbRegWrite(CODEC_FUNC_FREQ_SAMPLE_SEL, sel);
    //hal_AbbRegWrite(CODEC_FUNC_FREQ_SAMPLE_DIV, div);
    
    return;
}

// =============================================================================
// aud_CodeGallitePrePowerOnConfig
// -----------------------------------------------------------------------------
/// Set registers before powering on codec.
/// @return
// =============================================================================
PRIVATE VOID aud_CodeGallitePrePowerOnConfig(VOID)
{
    // Set sample rate
    aud_CodecGalliteSetRateStg(g_audCodecGalliteSampleRate);
}

// =============================================================================
// aud_ChipConfig
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
PRIVATE AUD_ERR_T aud_CodecGalliteChipConfig(
                                        HAL_AIF_FREQ_T sampleRate,
                                        BOOL voice,
                                        BOOL mono)
{
    AUD_PROFILE_FUNCTION_ENTRY(aud_ChipConfig);

    if (voice)
    {
        AUD_ASSERT(sampleRate == HAL_AIF_FREQ_8000HZ,
                "AUD: CodecGallite: Voice mode only supports 8kHz frequency.");
        AUD_ASSERT(mono == TRUE,
                "AUD: CodecGallite: Voice mode is mono only.");
        // TODO add a bunch of assert/check
    }

#ifdef AIF_DIV_8K_WORKAROUND
if (sampleRate == HAL_AIF_FREQ_8000HZ) sampleRate = HAL_AIF_FREQ_16000HZ;
#endif // AIF_DIV_8K_WORKAROUND

    g_audCodecGalliteSampleRate = sampleRate;
     
    // Configure audio codec
    if (!hal_AudOpen(HAL_AUD_USER_CODEC, aud_CodeGallitePrePowerOnConfig))
    {
        return AUD_ERR_RESOURCE_BUSY;
    }

    AUD_PROFILE_FUNCTION_EXIT(aud_ChipConfig);
    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecGalliteAifConfig
// -----------------------------------------------------------------------------
/// Configure the AIF interface for use with the CodecGallite chip.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return AUD_ERR_NO
// =============================================================================
INLINE AUD_ERR_T aud_CodecGalliteAifConfig(HAL_AIF_FREQ_T sampleRate, BOOL voice,
                                            BOOL mono)
{
#ifdef AIF_DIV_8K_WORKAROUND
if (sampleRate == HAL_AIF_FREQ_8000HZ) sampleRate = HAL_AIF_FREQ_16000HZ;
#endif // AIF_DIV_8K_WORKAROUND
	
	HAL_AIF_SERIAL_CFG_T aifSerialConfig;
    HAL_AIF_CONFIG_T     aifConfig;

    // AIF Serial Interface configuration
    aifSerialConfig.mode=HAL_SERIAL_MODE_I2S;
    aifSerialConfig.aifIsMaster=TRUE;
    aifSerialConfig.lsbFirst=FALSE;
    aifSerialConfig.polarity=FALSE;
    aifSerialConfig.rxDelay=HAL_AIF_RX_DELAY_2;
    aifSerialConfig.txDelay=HAL_AIF_TX_DELAY_ALIGN;
    aifSerialConfig.rxMode=HAL_AIF_TX_MODE_STEREO_STEREO; //HAL_AIF_RX_MODE_STEREO_STEREO;
    aifSerialConfig.txMode=HAL_AIF_TX_MODE_STEREO_STEREO;
    aifSerialConfig.fs=sampleRate;
    aifSerialConfig.bckLrckRatio=32; //64; // 2x32
    aifSerialConfig.invertBck=TRUE;
    aifSerialConfig.outputHalfCycleDelay=FALSE;
    aifSerialConfig.inputHalfCycleDelay=FALSE;
    aifSerialConfig.enableBckOutGating=FALSE;

	// AIF configuration
	aifConfig.interface=HAL_AIF_IF_SERIAL_IN_PARALLEL_OUT; // I2S input 1  
	aifConfig.sampleRate=sampleRate;
    aifConfig.channelNb    = mono ? HAL_AIF_MONO : HAL_AIF_STEREO;
	aifConfig.channelNb=HAL_AIF_STEREO; //HAL_AIF_STEREO;
	aifConfig.serialCfg=&aifSerialConfig;

    hal_AifOpen(&aifConfig);

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecGalliteClose
// -----------------------------------------------------------------------------
/// Close the driver and configure the CodecGallite and AIF.
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecGalliteClose(VOID)
{
    hal_AifClose();

    while(!hal_AudClose(HAL_AUD_USER_CODEC))
    {
        COS_Sleep(AUD_TIME_RETRY);
    }

    g_audCodecGalliteLatestCfg.spkSel = AUD_SPK_DISABLE;
    g_audCodecGalliteLatestCfg.spkType = AUD_SPEAKER_STEREO_NA;
    g_audCodecGalliteLatestCfg.micSel = AUD_MIC_DISABLE;
    
    g_audExtAudInterfaceOpened = FALSE;

    return AUD_ERR_NO;
}



// =============================================================================
// aud_CodecGalliteOpen
// -----------------------------------------------------------------------------
/// Open the driver and configure the CodecGallite and AIF.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecGalliteOpen(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono)
{

    AUD_ERR_T errStatus;
    errStatus = aud_CodecGalliteChipConfig(sampleRate, voice, mono);
    if (errStatus == AUD_ERR_NO)
    {
        errStatus = aud_CodecGalliteAifConfig(sampleRate, voice, mono);
        if (errStatus == AUD_ERR_NO)
        {
            g_audExtAudInterfaceOpened = TRUE;
        }
    }

    if (!g_audExtAudInterfaceOpened)
    {
        aud_CodecGalliteClose();
    }
    
    return errStatus;
}


// =============================================================================
// aud_ExtAudInterfaceSetup
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
PUBLIC AUD_ERR_T aud_ExtAudInterfaceSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
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

    UINT8 regSpkLvl;
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    AUD_CODEC_CALIB_OUT_GAINS_T calibOutGains;

    calibOutGains.calibValue = calibPtr->bb->audio[calibPtr->aud.audioGainItfMap[itf]].audioGains.outGains[cfg->level->spkLevel];
	regSpkLvl = calibOutGains.gains.musicLeft;

    // TODO Implement all the missing features
    
    // TODO Use calibration stuff to know the actual value to write in
    // the registers.

    // TODO Add if statement to discrimate about the Speaker, the recevier or other spkSel

    // TODO Mic interface to add in mic mode writing for volume

    HAL_AUD_SEL_T selCfg;
    selCfg.reg = 0;
    
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
                hal_AudSetRcvGain(regSpkLvl);
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
                hal_AudSetHeadGain(regSpkLvl);
            }
            break;
        }
        case AUD_SPK_LOUD_SPEAKER:
        {
            HAL_AUD_SPK_CFG_T spkCfg;
            spkCfg.reg = 0;
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.spkSel = FALSE;
                spkCfg.noSpkMute = FALSE;
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

                spkCfg.noSpkMute = TRUE;
                spkCfg.spkGain = regSpkLvl;
            }
            hal_AudSetSpkCfg(spkCfg);
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

        HAL_AUD_MIC_CFG_T micCfg;
        micCfg.reg = 0;
        if (cfg->level->micLevel != SND_MIC_MUTE)
        {
            micCfg.noMicMute = TRUE;
            micCfg.micGain = calibPtr->bb->audio[calibPtr->aud.audioGainItfMap[itf]].audioGains.inGain;
        }
        hal_AudSetMicCfg(micCfg);
    }

    if ( cfg->spkSel != g_audCodecGalliteLatestCfg.spkSel ||
         cfg->micSel != g_audCodecGalliteLatestCfg.micSel ||
         ( cfg->spkSel == AUD_SPK_LOUD_SPEAKER &&
           cfg->spkType != g_audCodecGalliteLatestCfg.spkType ) ||
         ( (cfg->level->spkLevel == SND_SPK_MUTE || 
             g_audCodecGalliteLatestLevel.spkLevel == SND_SPK_MUTE) &&
           cfg->level->spkLevel != g_audCodecGalliteLatestLevel.spkLevel ) )
    {
        hal_AudSetSelCfg(selCfg);
    }

    // Side tone volume controlled in the AIF. Normal use or loop test use.
    if (AUD_SIDE_VOL_TEST == cfg->level->sideLevel)
    {
        // If in loop test mode, the side tone and mic are set to their max values.
        HAL_AUD_MIC_CFG_T micCfg;
        micCfg.noMicMute = TRUE;
        micCfg.micGain = 15;
        hal_AudSetMicCfg(micCfg);
        hal_AifSetSideTone(15);
    }
    else
    {
        hal_AifSetSideTone(calibPtr->bb->audio[calibPtr->aud.audioGainItfMap[itf]].audioGains.sideTone[cfg->level->spkLevel]);
    }

    // Record last config
    g_audCodecGalliteLatestCfg.spkSel = cfg->spkSel;
    g_audCodecGalliteLatestCfg.spkType = cfg->spkType;
    g_audCodecGalliteLatestCfg.micSel = cfg->micSel;
    g_audCodecGalliteLatestLevel = *(cfg->level);

    return AUD_ERR_NO;
}



// =============================================================================
// aud_ExtAudInterfaceStreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
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
PUBLIC AUD_ERR_T aud_ExtAudInterfaceStreamStart(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audExtAudInterfacePlaying)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (!g_audExtAudInterfaceOpened)
        {
            errStatus = aud_CodecGalliteOpen(stream->sampleRate, stream->voiceQuality,
                (stream->channelNb == HAL_AIF_MONO));
            needCloseAud = TRUE;
        }

        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_ExtAudInterfaceSetup(itf, cfg);

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
				
            if (errStatus == AUD_ERR_NO)
            {
                // Send the stream through the IFC
                if (hal_AifPlayStream(stream) != HAL_ERR_NO)
                {
                    AUD_TRACE(AUD_INFO_TRC, 0,
                        "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                        errStatus = AUD_ERR_RESOURCE_BUSY;
                }
                else
                {
                    needCloseAud = FALSE;
                    g_audExtAudInterfacePlaying = TRUE;
                }
            }
            
            if (needCloseAud)
            {
                aud_CodecGalliteClose();
            }
        }
        return errStatus;
    }
}



// =============================================================================
// aud_StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_ExtAudInterfaceStreamStop(SND_ITF_T itf)
{
    if (g_audExtAudInterfacePlaying)
    {
    
		SXS_TRACE(TSTDOUT, "aud_ExtAudInterfaceStreamStop hal_AifStopPlay");
        hal_AifStopPlay();
		hal_AifStopRecord();
        g_audExtAudInterfacePlaying = FALSE;
    }

    if (g_audExtAudInterfaceRecording )
    {

	
	SXS_TRACE(TSTDOUT, "aud_ExtAudInterfaceStreamStop hal_AifStopRecord");

        hal_AifStopRecord();
        pmd_EnablePower(PMD_POWER_MIC, FALSE);
        g_audExtAudInterfaceRecording  = FALSE;
    }

    if (g_audExtAudInterfaceOpened)
    {
        return aud_CodecGalliteClose();
    }
    else
    {
        return AUD_ERR_NO;
    }
}

// =============================================================================
// aud_StreamPause
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
PUBLIC AUD_ERR_T aud_ExtAudInterfaceStreamPause(SND_ITF_T itf, BOOL pause)
{
    switch (hal_AifPause(pause))
    {
        case HAL_ERR_NO:
        default:
            return AUD_ERR_NO;
            break;
    }
}


// =============================================================================
// aud_StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
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
PUBLIC AUD_ERR_T aud_ExtAudInterfaceStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audExtAudInterfaceRecording )
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (!g_audExtAudInterfaceOpened)
        {
            errStatus = aud_CodecGalliteOpen(stream->sampleRate, stream->voiceQuality,
                (stream->channelNb == HAL_AIF_MONO));
            needCloseAud = TRUE;
        }
        
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_ExtAudInterfaceSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                // Send the stream through the IFC
                if (hal_AifRecordStream(stream) != HAL_ERR_NO)
                {
                    AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                    errStatus = AUD_ERR_RESOURCE_BUSY;
                }
                else
                {
                    pmd_EnablePower(PMD_POWER_MIC, TRUE);
                    needCloseAud = FALSE;
                    g_audExtAudInterfaceRecording  = TRUE;
                }
            }
            
            if (needCloseAud)
            {
                aud_CodecGalliteClose();
            }
        }
        return errStatus;
    }
    return AUD_ERR_NO;
}



// =============================================================================
// aud_ExtAudInterfaceTone
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
/// @param attenuation The attenuation level of the tone generator
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ExtAudInterfaceTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
//                CONST SND_TONE_ATTENUATION_T attenuation,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (!g_audExtAudInterfaceOpened && start)
    {
        // Open the codec
        // FIXME Use the stream phantom !
        errStatus = aud_CodecGalliteOpen(HAL_AIF_FREQ_8000HZ, FALSE, TRUE);
        needCloseAud = TRUE;
    }
    
    if (start)
    {
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_ExtAudInterfaceSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                hal_AifTone(tone, cfg->level->toneLevel, TRUE);
                needCloseAud = FALSE;
            }
        }
        
        if (needCloseAud)
        {
            aud_CodecGalliteClose();
        }
    }
    else
    {
        // Stop tone anyway
        hal_AifTone(0, 0, FALSE);
        if(!g_audExtAudInterfacePlaying && !g_audExtAudInterfaceRecording )
        {
            aud_CodecGalliteClose();
        }
    }

    return AUD_ERR_NO;

}



// =============================================================================
// aud_ExtAudInterfaceTonePause
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
PUBLIC AUD_ERR_T aud_ExtAudInterfaceTonePause(SND_ITF_T itf, BOOL pause)
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
// aud_ExtAudInterfaceCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ExtAudInterfaceCalibUpdateValues(SND_ITF_T itf)
{
    // FIXME --> Open the i2c before ...
//    return aud_ExtAudInterfaceSetup(itf, &g_audCodecGalliteLatestCfg);
    return HAL_ERR_NO;
}


