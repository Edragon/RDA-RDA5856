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



#include "cs_types.h"

#include "hal_voc.h"
#include "hal_aif.h"
#include "hal_sys.h"
#include "hal_error.h"

#include "aud_m.h"
#include "ars_m.h"
#include "arsp_cfg.h"
#include "hal_speech.h"
#include "mci.h"
#include "vpp_speech_bt.h"

#include "tgt_calib_m.h"

//extern uint32 pcmbuf_overlay[4608*2];


// svn propset svn:keywords "HeadURL Author Date Revision" dummy.h

/// TODO Does this part of the doc belong to the API ?
/// Workflow description
///   Init:
///     Init VPP, and load the first sample in its decoding buffer.
///     Mute mode is set.
///     Lauch play on the output decoded buffer of VPP.
///
///   Data flow:
///     Data are copied on VPP interrupts, meaning the previous set has been
///     decoded. Decoding is triggered by the IFC 'interrupt' directly telling
///     VPP what to do.
///     On IRQ from VPP, we copy the next buffer to be decoded.

// =============================================================================
//
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================
/// Number of trames before unmuting AUD
#define ARS_AUD_UNMUTE_TRAME_NB 2

/// Number of trames before unmuting VPP
#define ARS_VPP_UNMUTE_TRAME_NB 2



// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Start address of the stream buffer
PRIVATE UINT32* g_arsBufferStart = NULL;

/// End address of the stream buffer
PRIVATE UINT32 g_arsBufferLength= 0;

/// End address of the stream buffer
PRIVATE UINT32* g_arsBufferTop = NULL;

/// Current position in the buffer. Next read data will be
/// read from here.
PRIVATE UINT32* g_arsBufferCurPos= NULL;



/// Number of bytes of data to read from the buffer
PRIVATE UINT32 g_arsBufferAvailBytes= 0;

/// Pointer to the ouput (encoded) buffer of VPP part of VPP decoding buffer.
PRIVATE UINT32* g_arsEncOutput = NULL;

/// Number of played trames
PRIVATE UINT32 g_arsTramePlayedNb = 0;

/// Number of trames dropped
PRIVATE UINT32 g_arsTrameDroppedNb = 0;

/// User handler to call with play status is stored here.
PRIVATE ARS_USER_HANDLER_T g_arsUserHandler= NULL;


/// Audio configuration used to play the stream in normal state.
/// (ie not when started or stopped.
PRIVATE AUD_LEVEL_T g_arsAudioCfg;


/// VPP configuration used to play the stream in normal state.
/// (ie not when started or stopped.
//PRIVATE VPP_SPEECH_AUDIO_CFG_T g_arsVppCfg;

/// Interface used to play the current stream.
PRIVATE AUD_ITF_T g_arsItf= 0;

/// Index in the swap buffer (for PCM) which is the one where to copy
/// the new data
PRIVATE bool g_arsAlgEnable = 0;


/// Local buffer (TODO will become voc pcm's buffer)
/*PRIVATE*/ HAL_SPEECH_PCM_BUF_T g_arsPcmBuf;

/// To only pulse the mid buffer
PRIVATE BOOL g_arsMidBufDetected;


/// To stop at the end of the buffer if not in loop mode
PRIVATE BOOL g_arsLoopMode= FALSE;



/// Number of bytes composing one trame for the codec currently
/// used
PRIVATE UINT32 g_arsFrameSize  = 0;



PRIVATE INT16 g_arsMaxValue  = 0;

PRIVATE INT16 g_arsDigitalGain = 0;

// =============================================================================
//  FUNCTIONS
// =============================================================================
PUBLIC UINT32 vpp_SpeechInAlgGainDb2Val(INT32 db)
{
    CONST UINT32 micAlgVal[] =
    {
        0x40, 0x5a, 0x80, 0xb5,
        0x100, 0x16a, 0x200, 0x2d4,
        0x400, 0x5a8, 0x800, 0xb50,
        0x1000, 0x16a0, 0x2000, 0x2d40,
    };
    if (db == CALIB_AUDIO_GAIN_VALUE_MUTE)
        return 0;
    else if (db < 0)
        db = 0;
    else if (db > 45)
        db = 45;
    return micAlgVal[db/3];
}


// =============================================================================
// ars_VppSpeechHandler
// -----------------------------------------------------------------------------
/// Handler for the vpp module, for speech-kind encoded streams.
/// In charge of copying new datas, dealing with
/// underflow (no more samples), pause and resume etc ... It is called
/// every time a trame has been decoded by VPP.
// =============================================================================
#if 0
PRIVATE VOID ars_VppSpeechHandler(HAL_VOC_IRQ_STATUS_T* IRQStatus)
{
    ARS_PROFILE_FUNCTION_ENTER(ars_VppSpeechHandler);
    // Number of bytes to copy;
    UINT32 cpSz;
    UINT32 i;

    // Position in the decoding structure where to store
    UINT32* encOutBufPos;

    BOOL    endBufferReached = FALSE;
    BOOL    halfBufferReached = FALSE;

    g_arsTramePlayedNb++;
/*
    if (g_arsTramePlayedNb == ARS_AUD_UNMUTE_TRAME_NB)
    {
        // Unmute AUD
        ARS_TRACE(_SXR| TLEVEL (1), 0, "AUD unmuted");
        aud_Setup(g_arsItf, &g_arsAudioCfg);
    }
*/
    if (g_arsTramePlayedNb == ARS_VPP_UNMUTE_TRAME_NB)
    {
        // Unmute VPP
        ARS_TRACE(_SXR| TLEVEL (1), 0, "VPP unmuted");
        //g_arsVppCfg.encMute = VPP_SPEECH_UNMUTE;
        //vpp_SpeechAudioCfg(&g_arsVppCfg);

    }

    // Exit the handler immediatly if first interrupt
    if (g_arsTramePlayedNb <= 1)
    {
        ARS_PROFILE_FUNCTION_EXIT(ars_VppSpeechHandler);
        return;
    }

    // g_arsBufferAvailBytes is the number of bytes available
    // for the user to read. The room available in the buffer
    // is then length - g_arsBufferAvailBytes
    if (g_arsBufferLength - g_arsBufferAvailBytes < g_arsFrameSize)
    {
        // Drop the current frame
        g_arsTrameDroppedNb++;
        // This Frame was not played
        g_arsTramePlayedNb--;
        ARS_TRACE(TSTDOUT, 0, "Frame dropped: %d", g_arsTrameDroppedNb);

        // And does nothing else, hoping for better days
    }
    else
    {
        encOutBufPos = (UINT32*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)g_arsEncOutput);

        ///
        if (((UINT32)g_arsBufferCurPos + g_arsFrameSize) > (UINT32)g_arsBufferTop)
        {
            if (!g_arsLoopMode)
            {
                // As stop destroy the global vars ...
                ARS_USER_HANDLER_T handler = g_arsUserHandler;

                // Stop the play
                ars_Stop();

                // End of buffer reached
                if (handler)
                {
                    handler(ARS_STATUS_END_BUFFER_REACHED);
                }

                // Leave
                return;
            }
            // else wrap ...


            // Copy first part
            cpSz = (UINT32)(g_arsBufferTop - g_arsBufferCurPos);
            for (i = 0 ; i<(cpSz/4) ; i++)
            {
                *(g_arsBufferCurPos++) = *(encOutBufPos++);
            }

            g_arsBufferCurPos = g_arsBufferStart;
            cpSz = g_arsFrameSize - cpSz;

            // Buffer End Reached
            endBufferReached = TRUE;
            g_arsMidBufDetected = FALSE;
        }
        else
        {
            cpSz = g_arsFrameSize;
        }

        // Finish or full copy
        for (i = 0 ; i<(cpSz/4) ; i++)
        {
            *(g_arsBufferCurPos++) = *(encOutBufPos++);
        }


        // Half buffer reached
        if (((UINT32)g_arsBufferCurPos >= (UINT32)g_arsBufferStart + (g_arsBufferLength/2))
            && !g_arsMidBufDetected)
        {
            halfBufferReached = TRUE;
            g_arsMidBufDetected = TRUE;
        }

        // Report the new data
        UINT32 status = hal_SysEnterCriticalSection();
        // Decrease size in bytes
        g_arsBufferAvailBytes += g_arsFrameSize;
        hal_SysExitCriticalSection(status);
    }

    // Both of them shouldn't happen during the same iteration
    // of this function call.
    ARS_ASSERT(!(halfBufferReached & endBufferReached), "Half buffer and End of buffer reached in the same run");

    if (halfBufferReached)
    {
        if (g_arsUserHandler)
        {
            g_arsUserHandler(ARS_STATUS_MID_BUFFER_REACHED);
        }
    }

    if (endBufferReached)
    {
        if (g_arsUserHandler)
        {
            g_arsUserHandler(ARS_STATUS_END_BUFFER_REACHED);
        }
    }
    ARS_PROFILE_FUNCTION_EXIT(ars_VppSpeechHandler);
}

#endif

#define MULTS( x, y)  (INT64)(((INT64)x *(INT64)y)>>16)
#define ABS_AGC(X)  ((X<0)?-X:X)

//this value is what u want to control audio gain level;
//it should be set by user;
#define AGC_MAXVAL  30000; // 16384; // 32767;


static INT32 AGC_Coeff_l = 0x7fff*2*8;
static INT32 AGC_serror_l = 0;
static INT32  AGC_acc_error_l = 0;
PRIVATE void ars_PcmAlgGain(INT16 *in,INT16 *out,INT32 nLen)
{

    int pcm_in;
    int index;

    INT64 pcm_out;

    for(index=0;index<nLen;index++)
        {
            pcm_in = ABS_AGC(in[index]);

            pcm_out=MULTS(pcm_in,AGC_Coeff_l);


            if (pcm_out>32767)
            {
                pcm_out=32767;
            }

            AGC_serror_l = pcm_out - AGC_MAXVAL;
            if(AGC_serror_l <0)
            {

                if (AGC_serror_l<-32767/2)
                {
                    AGC_serror_l=-1;
                }
                else
                {
                    AGC_serror_l =0;
                }
            }

            AGC_acc_error_l = (AGC_acc_error_l >>17) + (AGC_serror_l <<1);



            AGC_Coeff_l = AGC_Coeff_l - AGC_acc_error_l ;

            if (AGC_Coeff_l> 0x7fff*32)
            {
                AGC_Coeff_l=0x7fff*32;
            }

            pcm_out=MULTS(in[index] ,AGC_Coeff_l);

            if (pcm_out<-32768)
            {
                pcm_out=-32768;
            }

            if (pcm_out>32767)
            {
                pcm_out=32767;
            }


            out[index] =pcm_out; //MULTS(in[index] ,AGC_Coeff_l);

        }

}


PRIVATE VOID ars_PcmAifIsr(VOID){

    UINT32 offset;
    BOOL    endBufferReached = FALSE;
    BOOL    halfBufferReached = FALSE;

    if ((((UINT32)g_arsBufferCurPos) + (g_arsFrameSize)) == (UINT32)g_arsBufferTop)
    {

        endBufferReached = TRUE;
        offset = 0;
    }
    else
    {
        halfBufferReached = TRUE;
        offset = g_arsFrameSize;
    }

    if (g_arsAlgEnable == TRUE)
        ars_PcmAlgGain((INT16 *)g_arsBufferCurPos, (INT16 *)g_arsBufferCurPos,g_arsFrameSize/2);

    g_arsBufferCurPos = g_arsBufferStart + offset/4;

    if (halfBufferReached)
    {
        if (g_arsUserHandler)
        {
            g_arsUserHandler(ARS_STATUS_MID_BUFFER_REACHED);
        }
    }

    if (endBufferReached)
    {
        if (g_arsUserHandler)
        {
            g_arsUserHandler(ARS_STATUS_END_BUFFER_REACHED);
        }
    }

}

// =============================================================================
// ars_Setup
// -----------------------------------------------------------------------------
/// Configure the ars service..
///
/// This functions configures the playing of a stream on a given audio interface:
/// gain for the side tone and
/// the speaker, input selection for the microphone and output selection for
/// the speaker... (TODO complete)
///
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See
/// #AUD_LEVEL_T for more details.
/// @return #ARS_ERR_NO it can execute the configuration.
// =============================================================================
PUBLIC ARS_ERR_T ars_Setup(AUD_ITF_T itf, CONST ARS_AUDIO_CFG_T* cfg)
{
    AUD_ERR_T audErr;

    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();

    // Set decoder config
    // ...

    // Set audio interface
    g_arsAudioCfg.spkLevel = AUD_SPK_MUTE;
    g_arsAudioCfg.micLevel = cfg->micLevel;
    g_arsAudioCfg.sideLevel = 0;
    g_arsAudioCfg.toneLevel = 0;

    g_arsDigitalGain=vpp_SpeechInAlgGainDb2Val(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall.alg)/0x60;

    if(g_arsDigitalGain!=0)
    {
        g_arsMaxValue=32700/g_arsDigitalGain;
    }
    else
    {
        g_arsMaxValue=0;
    }

    //Set Mic Gain in VoC
    vpp_SpeechBTSetInAlgGain(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall.alg);

    audErr = aud_Setup(itf, &g_arsAudioCfg);
    switch (audErr)
    {
        // TODO Add error as they comes

        case AUD_ERR_NO:
            return ARS_ERR_NO;
            break;

        default:
            return ARS_ERR_QTY; // ...unknown error ...
            break;
    }
}



// =============================================================================
// ars_Record
// -----------------------------------------------------------------------------
/// Start the playing of a stream.
///
/// This function  outputs the
/// audio on the audio interface specified as a parameter, on the output selected in
/// the #ars_Setup function. \n
/// In normal operation, when the buffer runs out, the playing will stop. It can
/// loop according to the caller choice.
/// ARS can call a user function
/// at the middle or at the end of the playback.
///
/// @param itf Interface on which play that stream
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @param loop If \c TRUE, the buffer is recorded in loop mode.
/// @return #ARS_ERR_RESOURCE_BUSY when the driver is busy with another audio
/// command, or if VPP is unavailable.
///         #ARS_ERR_NO it can execute the command.
// =============================================================================
PUBLIC ARS_ERR_T ars_Record(
                        CONST AUD_ITF_T      itf,
                        CONST ARS_ENC_STREAM_T* stream,
                        CONST ARS_AUDIO_CFG_T*  cfg,
                        BOOL loop)
{
    ARS_PROFILE_FUNCTION_ENTER(ars_Play);
    ARS_TRACE(_SXR| TLEVEL (1), 0, "ARS Start");
    ARS_TRACE(_SXR| TLEVEL (1), 0, "ARS Stream - buffer 0x%x  %d codec %d",
                                    stream->startAddress, stream->length,
                                    stream->mode);
    // TODO
    // BIG switch for PCM and/or other codec
    // Initial audio confguration
    AUD_LEVEL_T audioCfg;
    AUD_ERR_T audErr;
    HAL_AIF_STREAM_T pcmStream;

    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();

    // VPP encoding buffer pointer.
    CONST HAL_SPEECH_ENC_OUT_T* vppEncBuf;

    // The decoding buffer is used to configure VPP
    HAL_SPEECH_DEC_IN_T* vppDecBuf;

    g_arsDigitalGain=vpp_SpeechInAlgGainDb2Val(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall.alg)/0x60;

    if(g_arsDigitalGain!=0)
    {
        g_arsMaxValue=32700/g_arsDigitalGain;
    }
    else
    {
        g_arsDigitalGain=1;
        g_arsMaxValue=32767;
    }

    ARS_TRACE(_SXR| TLEVEL (1), 0, "ARS Stream - g_arsDigitalGain %d  g_arsMaxValue %d", g_arsDigitalGain,g_arsMaxValue);

    // Register global var
    g_arsItf = itf;
    g_arsBufferStart = (UINT32*) stream->startAddress;
    g_arsBufferTop = (UINT32*)(stream->startAddress + stream->length);
    g_arsBufferCurPos =(UINT32*) stream->startAddress;
    g_arsBufferAvailBytes = 0;
    g_arsBufferLength = stream->length;
    g_arsMidBufDetected = FALSE;

    g_arsTramePlayedNb = 0;
    g_arsTrameDroppedNb = 0;
    g_arsUserHandler = stream->handler;
    g_arsAlgEnable = 0;
    g_arsAudioCfg.spkLevel = cfg->spkLevel;
    g_arsAudioCfg.micLevel = cfg->micLevel;
    g_arsAudioCfg.sideLevel = 0;
    g_arsAudioCfg.toneLevel = 0;
    g_arsLoopMode = loop;

    // Set audio interface
    audioCfg.spkLevel = cfg->spkLevel;
    audioCfg.micLevel = cfg->micLevel;
    audioCfg.sideLevel = 0;
    audioCfg.toneLevel = 0;


    //hal_HstSendEvent(SYS_EVENT,0x52010000 + stream->mode);
    switch (stream->mode)
    {
        case ARS_REC_MODE_PCM:

            // Typical global vars
            g_arsEncOutput = (UINT32 *)&pcmbuf_overlay[0];//g_arsPcmBuf.pcmBuf[0][0];
            g_arsFrameSize = stream->length/2;
            g_arsAlgEnable = calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall.alg;

            // PCM Audio stream, output of VPP
            pcmStream.startAddress = (UINT32*)stream->startAddress;
            pcmStream.length = stream->length;
            pcmStream.sampleRate = stream->sampleRate;
            pcmStream.channelNb = stream->channelNb;
            pcmStream.voiceQuality = stream->voiceQuality;
            pcmStream.halfHandler = ars_PcmAifIsr;
            pcmStream.endHandler =  ars_PcmAifIsr;
            pcmStream.playSyncWithRecord = FALSE;
            break;

        default:

            // ---------------
            // open vpp speech
            // ---------------

            //if (HAL_ERR_RESOURCE_BUSY == vpp_SpeechOpen(ars_VppSpeechHandler, VPP_SPEECH_WAKEUP_HW_ENC))
            {
                return ARS_ERR_RESOURCE_BUSY;
            }

            // ---------------
            // dec (mode) buf
            // initialization
            // ---------------

            // init pointers to voc RAM
            //vppDecBuf = vpp_SpeechGetRxCodBuffer();

            vppDecBuf->dtxOn = 0;
            vppDecBuf->decFrameType = 0;
            vppDecBuf->bfi = 0;
            vppDecBuf->sid = 0;
            vppDecBuf->taf = 0;
            vppDecBuf->ufi = 0;

            // mode-specific config
            switch (stream->mode)
            {
                case ARS_REC_MODE_AMR475:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR475;
                    vppDecBuf->codecMode = HAL_AMR475_ENC;
                    break;

                case ARS_REC_MODE_AMR515:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR515;
                    vppDecBuf->codecMode = HAL_AMR515_ENC;
                    break;

                case ARS_REC_MODE_AMR59:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR59;
                    vppDecBuf->codecMode = HAL_AMR59_ENC;
                    break;

                case ARS_REC_MODE_AMR67:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR67 ;
                    vppDecBuf->codecMode = HAL_AMR67_ENC;
                    break;

                case ARS_REC_MODE_AMR74:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR74 ;
                    vppDecBuf->codecMode = HAL_AMR74_ENC;
                    break;

                case ARS_REC_MODE_AMR795:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR795;
                    vppDecBuf->codecMode = HAL_AMR795_ENC;
                    break;

                case ARS_REC_MODE_AMR102:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR102;
                    vppDecBuf->codecMode = HAL_AMR102_ENC;
                    break;

                case ARS_REC_MODE_AMR122:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR122;
                    vppDecBuf->codecMode = HAL_AMR122_ENC & HAL_AMR122_DEC;
                    break;

                case ARS_REC_MODE_AMR_RING:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_AMR_RING;
                    vppDecBuf->codecMode = HAL_AMR122_ENC & HAL_AMR122_DEC;
                    //g_arsVppCfg.if1 = 1;
                    break;

                case ARS_REC_MODE_EFR:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_EFR  ;
                    vppDecBuf->codecMode = HAL_EFR;
                    break;

                case ARS_REC_MODE_HR:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_HR ;
                    vppDecBuf->codecMode = HAL_HR;
                    break;

                case ARS_REC_MODE_FR:
                    g_arsFrameSize = HAL_SPEECH_FRAME_SIZE_FR;
                    vppDecBuf->codecMode = HAL_FR;
                    break;

                default:
                    ARS_ASSERT(FALSE, "Unsupported mode in ARS");
            }

            // ---------------
            // enc (rec) buf
            // initialization
            // ---------------

            //vppEncBuf = vpp_SpeechGetTxCodBuffer();

            // Pointer from which to get the encoded data.
            g_arsEncOutput = (UINT32*) vppEncBuf->encOutBuf;

            // FIXME
            UINT32 ij;
            for (ij=0; ij<8 ; ij++)
            {
                g_arsEncOutput[ij] = 0;
            }

            // ---------------
            // VPP audio config
            // ---------------
#if 0
            // TODO Improve ...
            g_arsVppCfg.echoEsOn = 0;
            g_arsVppCfg.echoEsVad = 0;
            g_arsVppCfg.echoEsDtd = 0;
            g_arsVppCfg.echoExpRel = 0;
            g_arsVppCfg.echoExpMu = 0;
            g_arsVppCfg.echoExpMin = 0;
            g_arsVppCfg.encMute = VPP_SPEECH_MUTE;
            g_arsVppCfg.decMute = VPP_SPEECH_MUTE;
            g_arsVppCfg.sdf = NULL;
            g_arsVppCfg.mdf = NULL;

            vpp_SpeechAudioCfg(&g_arsVppCfg);
                //Set Mic Gain in VoC
            vpp_SpeechSetInAlgGain(calibPtr->bb->audio[itf].audioGains.inGainsCall.alg);
#endif
            // ---------------
            // AUD audio config
            // ---------------

            // PCM Audio stream, input of VPP
//            pcmStream.startAddress = (UINT32*)vpp_SpeechGetTxPcmBuffer();
            pcmStream.length = sizeof(HAL_SPEECH_PCM_BUF_T);
            pcmStream.sampleRate = HAL_AIF_FREQ_8000HZ;
            pcmStream.channelNb = HAL_AIF_MONO;
            pcmStream.voiceQuality = TRUE;
            pcmStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
            pcmStream.endHandler = NULL; // Mechanical interaction with VPP's VOC
            pcmStream.playSyncWithRecord = FALSE;
            break;
    }



    // ...
    audErr = aud_StreamRecord(itf, &pcmStream, &audioCfg);

    // profit
    switch (audErr)
    {
        case AUD_ERR_NO:
            ARS_PROFILE_FUNCTION_EXIT(ars_Play);
            return ARS_ERR_NO;
            break; // :)

        default:
            ARS_TRACE(_SXR| TLEVEL (1), 0, "ARS Start failed !!!");
            ARS_PROFILE_FUNCTION_EXIT(ars_Play);
            return ARS_ERR_UNKNOWN;
            break; // :)
    }
}



// =============================================================================
// ars_Stop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback.
/// If the function returns
/// #ARS_ERR_RESOURCE_BUSY, it means that the driver is busy with another
/// audio command.
///
/// @return #ARS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #ARS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC ARS_ERR_T ars_Stop(VOID)
{
    ARS_PROFILE_FUNCTION_ENTER(ars_Stop);
    // stop stream ...
    aud_StreamStop(g_arsItf);
    //vpp_SpeechClose();

    // and reset global state
    g_arsItf = 0;
    g_arsBufferStart = NULL;
    g_arsBufferTop =  NULL;
    g_arsBufferCurPos = NULL;
    g_arsBufferAvailBytes = 0;
    g_arsBufferLength = 0;

    g_arsTramePlayedNb = 0;
    g_arsTrameDroppedNb = 0;
    g_arsUserHandler = NULL;

    g_arsAudioCfg.spkLevel = AUD_SPK_MUTE;
    g_arsAudioCfg.micLevel = AUD_MIC_MUTE;
    g_arsAudioCfg.sideLevel = 0;
    g_arsAudioCfg.toneLevel = 0;

    g_arsLoopMode = FALSE;

#if 0
    // TODO Improve ...
    g_arsVppCfg.echoEsOn = 0;
    g_arsVppCfg.echoEsVad = 0;
    g_arsVppCfg.echoEsDtd = 0;
    g_arsVppCfg.echoExpRel = 0;
    g_arsVppCfg.echoExpMu = 0;
    g_arsVppCfg.echoExpMin = 0;
    g_arsVppCfg.encMute = VPP_SPEECH_MUTE;
    g_arsVppCfg.decMute = VPP_SPEECH_MUTE;
    g_arsVppCfg.sdf = NULL;
    g_arsVppCfg.mdf = NULL;
    g_arsVppCfg.if1 = 0;
#endif

    g_arsFrameSize = 0;

    ARS_PROFILE_FUNCTION_EXIT(ars_Stop);
    return ARS_ERR_NO;
}




// =============================================================================
// ars_Pause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback.
/// If the function returns
/// #ARS_ERR_RESOURCE_BUSY, it means that the driver is busy with another
/// audio command.
///
/// @param pause If \c TRUE, pauses a playing stream. If \c FALSE (and a
/// stream is paused), resumes a paused stream.
/// @return #ARS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #ARS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC ARS_ERR_T ars_Pause(BOOL pause)
{
    aud_StreamPause(g_arsItf, pause);
    return ARS_ERR_NO;
}

// =============================================================================
// ars_GetBufPosition
// -----------------------------------------------------------------------------
/// This function returns the current position in the stream buffer.
/// This position points the next bytes to be played.
/// The bytes before this position might not yet have been played at the
/// time this position is read, but are in the process pipe to be played.
///
/// @return The pointer to the next sample to be fetched from the stream buffer.
// =============================================================================
PUBLIC UINT32* ars_GetBufPosition(VOID)
{
    return (g_arsBufferCurPos);
}



// =============================================================================
// ars_ReadData
// -----------------------------------------------------------------------------
/// When a stream buffer is recorded, already recorded data can be replaced by new
/// ones to record a long song seamlessly. The ARS service needs to be informed
/// about the number of new bytes available in the buffer. This is the role of the
/// #ars_ReadData function.
///
/// If the function returns
/// #ARS_ERR_RESOURCE_BUSY, it means that the driver is busy with another
/// audio command.
///
/// @param readDataBytes Number of bytes read from the buffer.
/// @return #ARS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #ARS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC ARS_ERR_T ars_ReadData(UINT32 readDataBytes)
{
    ARS_PROFILE_FUNCTION_ENTER(ars_ReadData);
    UINT32 status = hal_SysEnterCriticalSection();
    // nota : g_arsBufferAvailBytes is the number of bytes in the
    // buffer available for reading
    g_arsBufferAvailBytes -= readDataBytes;
    hal_SysExitCriticalSection(status);
    ARS_PROFILE_FUNCTION_EXIT(ars_ReadData);
    return ARS_ERR_NO;
}




// =============================================================================
// ars_AvailableData
// -----------------------------------------------------------------------------
/// @return The number of bytes ready to be read in the buffer.
///
// =============================================================================
PUBLIC UINT32 ars_AvailableData(VOID)
{
    return (g_arsBufferAvailBytes);
}
