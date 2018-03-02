
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
// FILENAME: mmc_audiompeg.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   xuml
//
////////////////////////////////////////////////////////////////////////////////

#include "cswtype.h"
#include "mcip_debug.h"
#include "fs.h"
#include "mmc_audiompeg.h"
#include "hal_error.h"

#include "cpu_share.h"
#include "assert.h"
#include "hal_voc.h"
#include "hal_rda_abb.h"

#include "global.h"
#include "audio_api.h"

#include "aud_m.h"

#include "fs_asyn.h"

#include "mci.h"
#include "cos.h"

#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "sxr_tls.h"

#include "resample.h"
#include "event.h"
#include "mmc_audioprefilter.h"

#ifdef KALAOK_SUPPORT
#include "Effectecho.h"
#endif

#define AudioJpeg_INPUT_BUFSIZE     (7*1024)

#define  MIN_INPUT_REMAIN_SIZE   (2*1024)

#define AUDIO_MAX_OUTPUT_BUFSIZE (1152*4)
#define AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE (1152*4*2)


#define AACDEC_OUTPUT_BUFSIZE (1024*4)
#define AACDEC_PCM_OUTPUT_BUFFER_SIZE (1024*4*2)

#define MP3DEC_OUTPUT_BUFSIZE (1152*4)
#define MP3DEC_PCM_OUTPUT_BUFFER_SIZE (1152*4*2)
#define MP3_MAX_ERROR_CNT    30
extern AUD_LEVEL_T audio_cfg;
extern HAL_AIF_STREAM_T audioStream;
extern AUD_ITF_T audioItf;

MPEG_PLAY *MpegPlayer;

static INT8 Mmc_Audio_Pcm_Half;
static UINT8 ContinuePlayFlag=0;//this flag controls to continue play the left data in the pcm buffer
static UINT8 LeftDataPlayFlag=0;//this flag controls to continue play the left data in the input buffer

static UINT8 g_ReadFileFlag=0;
static UINT8 g_PCMPassFlag=0;
static UINT8 g_VocBusyFlag=0;

static UINT8 g_GetInfoFlag=0;
static UINT8 g_DecOpenFlag=0;
static UINT8 FrameCount=0;
static UINT8 ErrorFrameCount = 0;
static UINT8 Frame_exit= 0;
static INT32 g_ConsumedLength=0;
static UINT8 g_AudioMode=0;
static UINT8 g_AudioSeekFlag=0;
static uint32 g_id3_tagsize_flag=0;
static unsigned char g_audMutex = 0xFF;
static INT32 g_MPEGFILESIZE=0;
//static INT32 g_MPEGConsumeSIZE=0;
//static INT32 g_MPEGOutputSIZE=0;

static INT32 g_MpegResumeDataLen = 0;

//#define AUDIO_DUMP_FT_DATA_ENABLE  // dump factory test data.

static UINT8 g_AudioTestMode = 0;
static UINT8 g_AudioFTDetectCount = 0;
static UINT8 g_AudioFTDetectPassCount = 0;

#ifndef CODEC_SUPPOT
extern BOOL g_audioIsReadingFIle;
#endif

#ifdef AUDIO_DUMP_FT_DATA_ENABLE
static UINT8 *g_AudioTestSaveAddr = 0;
static UINT16 g_AudioTestSaveBytes = 0;
extern UINT8 *g_codec_buffer;
#endif

extern HAL_AIF_STREAM_T micStream;
AUDIO_FACTORY_TEST_HANDLER_T g_mmcAudioFactoryTestHandler = NULL;


extern UINT8 MCI_Play_Stream_Buffer;
UINT16 *g_last_buffer_addr;
UINT32  g_PCM_SMAPLE_RATE_ERROR;
INT32 AACFRAME;
INT32 AAClearBuffer;
#ifdef BT_SUPPORT
extern UINT8 MCI_Play_BTStream;
extern bt_a2dp_audio_cap_struct *MCI_BTAudioCap;
#endif

#ifdef MUSIC_BAND_INFO
static uint16 MpegBandInfo[8]={0};
#endif

#ifdef KALAOK_SUPPORT
///dsp

extern HAL_AIF_STREAM_T audioInStream;
extern effectecho_param echo_param;// 1:delay times ms; 2:16384 -> 0.5;
extern UINT16 * echo_delay_buff;
#endif

#ifndef CODEC_SUPPORT
#define JUMP_WRONG_MP3_DATA
#endif

#ifdef JUMP_WRONG_MP3_DATA
INT32 find_head_length = 0;
UINT32 mp3_head = 0x00;
#endif
#ifdef CODEC_SUPPORT

static UINT8 g_VocTimerID = 0;
#define  MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN) \
            if (HAL_ERR_RESOURCE_NOT_ENABLED == vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN)) {\
                g_VocBusyFlag = 0; \
            } else {                   \
                g_VocBusyFlag = 1;   \
                if (g_VocTimerID) {\
                    COS_KillTimer(g_VocTimerID);\
                    g_VocTimerID = 0;\
                }                       \
                g_VocTimerID = COS_SetTimer(35,  \
                               MMC_AudioVocISRTimeOut, NULL, COS_TIMER_MODE_SINGLE);\
            }

#else

#define  MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN) \
            if (HAL_ERR_RESOURCE_NOT_ENABLED == vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN)) {\
                g_VocBusyFlag = 0; \
            } else {                   \
               g_VocBusyFlag = 1; \
            }



#endif


extern PUBLIC void hal_SysSetVocClockDefault(void);
extern PUBLIC UINT32 gpio_detect_earpiece(VOID);
extern void mmc_BuffTagSize(UINT32 tagsize);

MCI_ERR_T  MMC_MpegReceiveA2dpData(MPEG_PLAY *pAudioPlayer);
UINT8 flag_PCM = 0;

// =============================================================================
// aud_MutexTake
// -----------------------------------------------------------------------------
/// Take the mutex protecting AUD shared variables.
///
/// @return The user ID to take this mutex.
// =============================================================================
static UINT8 aud_MutexTake(VOID)
{
    // Create the semaphore if it doesn't exist.
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_audMutex == 0xFF)
    {
        g_audMutex = sxr_NewMutex();
    }
    hal_SysExitCriticalSection(status);

    return sxr_TakeMutex(g_audMutex);
}


// =============================================================================
// aud_MutexRelease
// -----------------------------------------------------------------------------
/// Release the mutex protecting AUD shared variables.
///
/// @param userId The user ID to release this mutex
// =============================================================================
static VOID aud_MutexRelease(UINT8 userId)
{
    sxr_ReleaseMutex(g_audMutex, userId);
}

MCI_ERR_T MMC_AUDIODecClose (MPEG_PLAY *pAudioPlayer)
{

#ifdef CODEC_SUPPORT
    if (g_VocTimerID) {
        COS_KillTimer(g_VocTimerID);
        g_VocTimerID = 0;
    }
#endif
    hal_HstSendEvent(SYS_EVENT,0xabc20000);
    MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MCI_MP3AAC]MMC_AUDIODecClose!");
    vpp_AudioJpegDecClose();
    aud_StreamStop(audioItf);
    hal_SysSetVocClockDefault();
    mmc_MemFreeAll();
    flag_PCM =0;
    g_DecOpenFlag = 0;
    MCI_Play_Stream_Buffer = 0;

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 1);
        //Avdtp_Stop_Data();
#endif
    COS_Free(MpegPlayer);
    MpegPlayer = NULL;

#ifdef KALAOK_SUPPORT
    if(echo_delay_buff != NULL)
    {
    	   hal_HstSendEvent(SYS_EVENT,0xa12301a01);
         COS_Free(echo_delay_buff);
	  echo_delay_buff = NULL;
    }
#endif

    return  MCI_ERR_NO;

}
#ifdef CODEC_SUPPORT

static void MMC_AudioVocISRTimeOut(void)
{
    if (MpegPlayer == NULL)
        return ;
    hal_HstSendEvent(SYS_EVENT,0xabc21404);
    MMC_AUDIODecClose(MpegPlayer);
    MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
    mmc_SetCurrMode(MMC_MODE_IDLE);
    ContinuePlayFlag = 0;

}
#endif
/*
* NAME: MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioVocISR(void)
{
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_VOC);

#ifdef CODEC_SUPPORT

    if (g_VocTimerID) {
        COS_KillTimer(g_VocTimerID);
        g_VocTimerID = 0;
    }
#endif
}



static int AudioZCPDetect(unsigned char *buffer, int size, int range, int freq, int sample_rate)
{
    int zsp_target_value = 0;
    int zsp_count = 0;
    int i, ret = 0;
    short *buf = (short *)buffer;

    //zsp_target_value = 2*size*freq/sample_rate;  // double size.
    zsp_target_value = size*freq/sample_rate;

    // count zero crossing point
    for (i = 0; i < size/2; i++)
    {
    #if 0
        if ((*buf > -range) && (*buf < range))
        {
            zsp_count++;
        }
    #else
        if (*buf > 0)
        {
            if(*(buf+1) < 0)
            {
                zsp_count++;
            }
        }
        else
        {
            if(*(buf+1) > 0)
            {
                zsp_count++;
            }
        }
    #endif
        buf++;
    }

    if ((zsp_count > (zsp_target_value - 10)) && (zsp_count < (zsp_target_value + 10)))
    {
        g_AudioFTDetectPassCount++;
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    if (++g_AudioFTDetectCount > 20)
    {
        if (g_mmcAudioFactoryTestHandler != NULL)
            g_mmcAudioFactoryTestHandler(g_AudioFTDetectPassCount > 10 ? 1 : 0);

        g_AudioFTDetectCount = 0;
        g_AudioFTDetectPassCount = 0;
    }

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]FT: zsp_target_value:%d, zsp_count: %d, success: %d, pass/total: %d/%d",
        zsp_target_value, zsp_count, ret, g_AudioFTDetectPassCount, g_AudioFTDetectCount);

    return ret;
}


/*
* NAME: MMC_AudioPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/



void AudioHalfPcmISR(void)
{
    Mmc_Audio_Pcm_Half = 0;
    // hal_HstSendEvent(SYS_EVENT,0x96600001);

#if 1
#ifndef CODEC_SUPPOT
    if(g_audioIsReadingFIle == TRUE)
    {
        memset(MpegPlayer->MpegOutput.pcm.Buffer, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
    }
#endif
#endif

    if(flag_PCM < 6)
    {
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);
        flag_PCM++;
    }

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 0)
#endif

#if 0
#ifndef CODEC_SUPPOT
        if(!g_ReadFileFlag)
        {
            memset(pcmbuf_overlay, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
        }
#endif
#endif
    if (g_AudioTestMode == 1)
    {
        AudioZCPDetect((UINT8 *)micStream.startAddress, micStream.length/2, 1000, 400, micStream.sampleRate);

#ifdef AUDIO_DUMP_FT_DATA_ENABLE
        memcpy(g_AudioTestSaveAddr+g_AudioTestSaveBytes, (UINT8 *)micStream.startAddress, micStream.length/2);
        g_AudioTestSaveBytes += micStream.length/2;
        if (g_AudioTestSaveBytes >= 1024*8)
        {
            g_AudioTestSaveBytes = 0;  // loop save record data.
        }
#endif
    }
}

/*
* NAME: MMC_AudioPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/
void AudioEndPcmISR(void)
{
    Mmc_Audio_Pcm_Half = 1;
    //hal_HstSendEvent(SYS_EVENT,0x96600002);
    
#if 1
#ifndef CODEC_SUPPOT
    if(g_audioIsReadingFIle == TRUE)
    {
        memset(MpegPlayer->MpegOutput.pcm.Buffer, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
    }
#endif
#endif

    if(flag_PCM < 6)
    {
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);
        flag_PCM++;
    }

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 0)
#endif

#if 0
#ifndef CODEC_SUPPOT
        if(!g_ReadFileFlag)
        {
            memset(pcmbuf_overlay, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
        }
#endif
#endif

    if (g_AudioTestMode == 1)
    {
        AudioZCPDetect((UINT8 *)micStream.startAddress + micStream.length/2, micStream.length/2, 1000, 400, micStream.sampleRate);

#ifdef AUDIO_DUMP_FT_DATA_ENABLE
        memcpy(g_AudioTestSaveAddr+g_AudioTestSaveBytes, (UINT8 *)micStream.startAddress + micStream.length/2, micStream.length/2);
        g_AudioTestSaveBytes += micStream.length/2;
        if (g_AudioTestSaveBytes >= 1024*8)
        {
            g_AudioTestSaveBytes = 0;  // loop save record data.
        }
#endif
    }
}
/// Audio interface used in the test
extern VOID hal_abbSetPaOpenMode(BOOL on);
void MMC_MpegPcmOutStart(MPEG_PLAY *pAudioPlayer)
{

    hal_HstSendEvent(SYS_EVENT,0x88503000);
    hal_HstSendEvent(SYS_EVENT,(UINT32)pAudioPlayer->MpegOutput.pcm.Buffer);

    vpp_Mp3DecSetAudioParamReloadFlag();
    vpp_AudioJpegDecCalcEqCoef();

#ifdef KALAOK_SUPPORT
	if(pAudioPlayer->Voc_AudioDecStatus.nbChannel==2)
	{
		hal_HstSendEvent(SYS_EVENT,0xa12301a00);

		//output->pcm.Size alread have been changed in voc;
		pAudioPlayer->Voc_AudioDecStatus.nbChannel = pAudioPlayer->Voc_AudioDecStatus.nbChannel >> 1;

		hal_HstSendEvent(SYS_EVENT,pAudioPlayer->MpegOutput.pcm.Size);
		hal_HstSendEvent(SYS_EVENT,pAudioPlayer->Voc_AudioDecStatus.nbChannel);
	}
// Stream out
    // Stream out
    audioStream.startAddress  = (UINT32*) pAudioPlayer->MpegOutput.pcm.Buffer;
    audioStream.length        = pAudioPlayer->MpegOutput.pcm.Size*4;
    audioStream.channelNb     = pAudioPlayer->Voc_AudioDecStatus.nbChannel;

    audioStream.voiceQuality         = !TRUE;
    audioStream.halfHandler   = NULL;
    audioStream.endHandler    = NULL;
#else

    // Stream out
    audioStream.startAddress  = (UINT32*) pAudioPlayer->MpegOutput.pcm.Buffer;
    audioStream.length        = pAudioPlayer->MpegOutput.pcm.Size*4;
    audioStream.channelNb     = pAudioPlayer->Voc_AudioDecStatus.nbChannel;

    audioStream.voiceQuality         = !TRUE;
    audioStream.halfHandler   = AudioHalfPcmISR;
    audioStream.endHandler    = AudioEndPcmISR;

    if (g_AudioTestMode == 1)
    {
#ifdef AUDIO_DUMP_FT_DATA_ENABLE
        g_AudioTestSaveAddr = (g_codec_buffer + 1024*8); // base_addr + 0x2000
        g_AudioTestSaveBytes = 0;
#endif
        audioStream.playSyncWithRecord = TRUE;
        micStream.startAddress =  (UINT32*)mmc_MemMalloc(pAudioPlayer->MpegOutput.pcm.Size*4);
        micStream.length = pAudioPlayer->MpegOutput.pcm.Size*4;
        micStream.sampleRate = HAL_AIF_FREQ_8000HZ;
        micStream.channelNb = HAL_AIF_MONO;
        micStream.voiceQuality = TRUE;
        micStream.playSyncWithRecord = TRUE;

        micStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
        micStream.endHandler = NULL; // Mechanical interaction with VPP's VOC_AHB_BCPU_DEBUG_IRQ_CAUSE
        MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]factory test rec cfg: addr=0x%x, size=%d",
            micStream.startAddress, micStream.length);
    }
#endif

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 1)
    {
        if(gpio_detect_earpiece())
            audioItf = AUD_ITF_BT_EP;
        else
            audioItf = AUD_ITF_BT_SP;
    }
    else
#endif
        if(gpio_detect_earpiece())
            audioItf = AUD_ITF_EAR_PIECE;
        else
            audioItf = AUD_ITF_LOUD_SPEAKER;


    hal_HstSendEvent(SYS_EVENT,0x88900000 + pAudioPlayer->Voc_AudioDecStatus.SampleRate);

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]pAudioPlayer->Voc_AudioDecStatus.SampleRate:%d",pAudioPlayer->Voc_AudioDecStatus.SampleRate);
    g_PCM_SMAPLE_RATE_ERROR=pAudioPlayer->Voc_AudioDecStatus.SampleRate;
    g_PCM_SMAPLE_RATE_ERROR|=0x80000000;
    switch (pAudioPlayer->Voc_AudioDecStatus.SampleRate)
    {
        case 48000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_48K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_48000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 44100:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_44_1K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 32000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_32K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_32000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 24000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_24K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_24000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 22050:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_22_05K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_22050HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 16000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_16K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_16000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 12000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_12K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_12000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 11025:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_11_025K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_11025HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        case 8000:
            diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_8K\n");
            audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
            break;
        default:
            diag_printf("[MMC_AUDIO]##WARNING## Sample rate error:%d\n",pAudioPlayer->Voc_AudioDecStatus.SampleRate);
            break;
    }


       hal_abbSetPaOpenMode(TRUE);

#ifdef KALAOK_SUPPORT
       UINT32 errStatus = AUD_ERR_NO;

	audioInStream.startAddress = audioStream.startAddress+pAudioPlayer->MpegOutput.pcm.Size;
	audioInStream.length       = audioStream.length;
	audioInStream.sampleRate   = audioStream.sampleRate;
	audioInStream.channelNb    = audioStream.channelNb;
	audioInStream.voiceQuality = !TRUE;
	audioInStream.playSyncWithRecord = FALSE;
	audioInStream.halfHandler  = AudioHalfPcmISR;
	audioInStream.endHandler   = AudioEndPcmISR;

	errStatus = aud_StreamStart(audioItf, &audioStream, &audio_cfg);
	if (errStatus == AUD_ERR_NO)
	{
		aud_StreamRecord(audioItf, &audioInStream, &audio_cfg);
	}
#else
    UINT32 errStatus = AUD_ERR_NO;
    errStatus = aud_StreamStart(audioItf, &audioStream, &audio_cfg);

    if (g_AudioTestMode == 1)
    {
        if (errStatus == AUD_ERR_NO)
        {
            MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]factory test: record start.");
            // Start the other flux only if the Rx one is started
            errStatus = aud_StreamRecord(audioItf, &micStream, &audio_cfg);
        }
    }
#endif
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_MpegPcmOutStart end; errStatus = %d", errStatus);
}

/*
* NAME: MMC_AudioDecOpen()
* DESCRIPTION: Initialize AUDIO decoder.
*/
MCI_ERR_T MMC_AudioDecOpen (HANDLE fhd, MPEG_PLAY *pAudioPlayer)
{
    if(MpegPlayer == NULL)
        pAudioPlayer = MpegPlayer = COS_Malloc(sizeof(MPEG_PLAY));
    MPEG_INPUT *input = &pAudioPlayer->MpegInput;
    MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
    vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;
    HAL_ERR_T voc_ret = 0;
    int32 readlen = 0;
    FrameCount=0;
    AAClearBuffer=0;
    AACFRAME=0;
    flag_PCM =0;

#ifdef JUMP_WRONG_MP3_DATA
    mp3_head = 0x00;
#endif

    hal_HstSendEvent(SYS_EVENT,(UINT32)0x88100011);
    hal_HstSendEvent(SYS_EVENT,(UINT32)input);
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_104M);
    /*open AAC file*/
    input->fileHandle =fhd;

    /*initialize I/O data struct*/
    input->bof = (uint8 *)inputbuf_overlay;
#ifdef CODEC_SUPPORT
    hal_HstSendEvent(SYS_EVENT,g_MpegResumeDataLen);

    memset(input->bof + g_MpegResumeDataLen, 0, AudioJpeg_INPUT_BUFSIZE - g_MpegResumeDataLen);
    input->length = g_MpegResumeDataLen;
    input->inFlag = 0;
#else
    memset(input->bof, 0, AudioJpeg_INPUT_BUFSIZE);
    input->length = AudioJpeg_INPUT_BUFSIZE;
    input->inFlag = 0;
#endif

    //output->length = (AUDIO_MAX_OUTPUT_BUFSIZE) >> 2;//80 int

    output->pcm.Buffer = (uint32 *)pcmbuf_overlay;
    //output->data =  (int16 *)pcmbuf_overlay;
    memset(output->pcm.Buffer,0,AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
    output->pcm.Buffer_head=output->pcm.Buffer;

    hal_HstSendEvent(SYS_EVENT,0x88100020);
    hal_HstSendEvent(SYS_EVENT,(UINT32)input->bof);
    hal_HstSendEvent(SYS_EVENT,pAudioPlayer->mode);
#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 1)
    {
        //Avdtp_Clean_Data();
        input->length = 0;
    }
#endif

    switch(pAudioPlayer->mode)
    {
        case MMC_MPEG_NONE:
        case MMC_AAC_DECODE:
            pAudioPlayer->Voc_AudioDecStatus.consumedLen = 0;
            pAudioPlayer->Voc_AudioDecStatus.nbChannel = 0;
            pAudioPlayer->Voc_AudioDecStatus.output_len = 0;
            pAudioPlayer->Voc_AudioDecStatus.streamStatus= 0;

            pAudioPlayer->Voc_AudioDecIN.EQ_Type=-1;
            pAudioPlayer->Voc_AudioDecIN.reset=1;
            break;

        case MMC_MP3_DECODE:
            pAudioPlayer->Voc_AudioDecStatus.consumedLen = 0;
            pAudioPlayer->Voc_AudioDecStatus.nbChannel = 0;
            pAudioPlayer->Voc_AudioDecStatus.output_len = 0;
            pAudioPlayer->Voc_AudioDecStatus.streamStatus= 0;

            pAudioPlayer->Voc_AudioDecIN.EQ_Type=-1;
            pAudioPlayer->Voc_AudioDecIN.reset=1;
            break;

        default:
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_MP3AAC_ERROR]Error Mode!");
            break;
    }



    Mmc_Audio_Pcm_Half=1;
    ErrorFrameCount=0;
    ContinuePlayFlag = 0;
    LeftDataPlayFlag = 0;
    g_ReadFileFlag=0;
    g_PCMPassFlag = 0;
    g_VocBusyFlag = 0;
    //g_MPEGConsumeSIZE = 0;
    //g_MPEGOutputSIZE = 0;


#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 0)
#endif
    {
#ifdef CODEC_SUPPORT
        int32 skiplen = 0;

        /*prepare input stream*/
        readlen = mmc_aud_FileRead(input->fileHandle, input->bof, AudioJpeg_INPUT_BUFSIZE - g_MpegResumeDataLen);

        skiplen = mpeg_stream_check(&pAudioPlayer->mode,input->bof,readlen);
        if (skiplen >0 ) {

            if (skiplen >= readlen)
            {
                readlen = mmc_aud_FileRead(input->fileHandle, input->bof, AudioJpeg_INPUT_BUFSIZE - g_MpegResumeDataLen);

            }
            else
            {
                memcpy(input->bof, (input->bof + skiplen), (readlen - skiplen));
                readlen = readlen - skiplen;
                readlen += mmc_aud_FileRead(input->fileHandle, input->bof+(readlen - skiplen),skiplen);
            }

        }

        hal_HstSendEvent(SYS_EVENT,skiplen);
        hal_HstSendEvent(SYS_EVENT,*(int*)input->bof);
        hal_HstSendEvent(SYS_EVENT,pAudioPlayer->mode);

        input->length += readlen;
        g_VocTimerID = 0;
#else
        /*prepare input stream*/
        readlen = mmc_aud_FileRead(input->fileHandle, input->bof, input->length);
#endif

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_MP3AAC] First Read Len=%d ;Need Length=%d\n", input->length,AudioJpeg_INPUT_BUFSIZE - g_MpegResumeDataLen);

        if(readlen<0)
        {
            return MCI_ERR_CANNOT_OPEN_FILE;
        }
        g_AudioMode = pAudioPlayer->mode;

        if(g_ConsumedLength>0)
        {
            g_AudioSeekFlag=1;
        }
        else
        {
            g_AudioSeekFlag=0;
        }

        if ((input->bof[0]=='I')&&(input->bof[1]=='D')&&(input->bof[2]=='3'))
        {
            hal_HstSendEvent(SYS_EVENT,0x1D300000);
            /* high bit is not used */
            g_id3_tagsize_flag = (input->bof[6] << 21) | (input->bof[7] << 14) |
                      (input->bof[8] <<  7) | (input->bof[9] <<  0);

            g_id3_tagsize_flag += 10;

            diag_printf("[MMC_AUDIO] g_id3_tagsize: %d\n", g_id3_tagsize_flag);

#if defined(__MMC_PLAY_BUFFER__)
            //Buffer play need Loop N times(N>=1),we need know position to do loop
            mmc_BuffTagSize(g_id3_tagsize_flag);
#endif /* __MMC_PLAY_BUFFER__ */

#ifndef CODEC_SUPPORT
            if(mmc_aud_FileSeek(input->fileHandle, g_id3_tagsize_flag, FS_SEEK_SET)<0)
            {
                hal_HstSendEvent(SYS_EVENT,0x1D300001);
                return MCI_ERR_CANNOT_OPEN_FILE;;
            }
#else
            if(g_id3_tagsize_flag > input->length)
            {
                hal_HstSendEvent(SYS_EVENT,0x1D300002);
                COS_Sleep(100);
                readlen = mmc_aud_FileRead(input->fileHandle, input->bof, g_id3_tagsize_flag - input->length);
                if(readlen < g_id3_tagsize_flag - input->length)
                {
                    hal_HstSendEvent(SYS_EVENT,0x1D300003);
                    hal_HstSendEvent(SYS_EVENT,g_id3_tagsize_flag);
                    hal_HstSendEvent(SYS_EVENT,input->length);
                    hal_HstSendEvent(SYS_EVENT,readlen);
                }
                input->length = 0;
            }
            else
            {
                input->length -= g_id3_tagsize_flag;

                memcpy(input->bof, input->bof+g_id3_tagsize_flag, input->length);
                memset(input->bof+input->length, 0x00, AudioJpeg_INPUT_BUFSIZE -input->length);

                hal_HstSendEvent(SYS_EVENT,0x1D300004);
                hal_HstSendEvent(SYS_EVENT,input->length);

            }
 #endif
            g_ConsumedLength += g_id3_tagsize_flag;
#ifdef CODEC_SUPPORT
            readlen = mmc_aud_FileRead(input->fileHandle, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE-input->length);
            input->length += readlen;
#else
            readlen = mmc_aud_FileRead(input->fileHandle, input->bof, input->length);
#endif

            diag_printf("[MMC_AUDIO] after ID3first read len=%d ;need length=%d\n", readlen,input->length);

            if(readlen<0)
            {
                return MCI_ERR_CANNOT_OPEN_FILE;
            }
            if(readlen<input->length)
            {
            }
        }
    }

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen vpp_AudioJpegDecOpen!");
    /*open voc decoder*/

    voc_ret = vpp_AudioJpegDecOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioVocISR);

    hal_HstSendEvent(SYS_EVENT,0x88110000+voc_ret);

    switch (voc_ret)
    {
        case HAL_ERR_NO:
            break;

        case HAL_ERR_RESOURCE_RESET:
        case HAL_ERR_RESOURCE_BUSY:
        case HAL_ERR_RESOURCE_TIMEOUT:
        case HAL_ERR_RESOURCE_NOT_ENABLED:
        case HAL_ERR_BAD_PARAMETER:
        case HAL_ERR_UART_RX_OVERFLOW:
        case HAL_ERR_UART_TX_OVERFLOW:
        case HAL_ERR_UART_PARITY:
        case HAL_ERR_UART_FRAMING:
        case HAL_ERR_UART_BREAK_INT:
        case HAL_ERR_TIM_RTC_NOT_VALID:
        case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
        case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
        case HAL_ERR_COMMUNICATION_FAILED:
        case HAL_ERR_QTY:
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_MP3AAC_ERROR]Fail to Call vpp_AudioJpegDecOpen()t: %d \n", voc_ret);
            return MCI_ERR_ERROR;
        default:
            break;
    }
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen end!");

    // setup first frame config
    pVoc_Voc_AudioDecIN->inStreamBufAddr=(INT32 *)input->bof;
    pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer;

    pVoc_Voc_AudioDecIN->mode=pAudioPlayer->mode;
    pVoc_Voc_AudioDecIN->BsbcEnable = 0;
    if(AUD_ITF_BT_SP==audioItf)
        pVoc_Voc_AudioDecIN->audioItf=AUD_ITF_LOUD_SPEAKER;
    else
        pVoc_Voc_AudioDecIN->audioItf=audioItf;

#ifdef KALAOK_SUPPORT
    if(echo_delay_buff == NULL)
    {
        hal_HstSendEvent(SYS_EVENT,0xa12301a02);
        echo_delay_buff = COS_Malloc(ECHO_DELAY_MAX * sizeof(INT16));
    }
    Effectecho_init(ECHO_DELAY_MAX, echo_delay_buff);
#endif

    g_DecOpenFlag = 1;

    return MCI_ERR_NO;
}


void Audio_MpegPrepareNextFrame(MPEG_PLAY *pAudioPlayer)
{
    MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
    vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;

    MCI_TRACE (MCI_AUDIO_TRC,0, "[Audio_MpegPrepareNextFrame]Mmc_Audio_Pcm_Half:%d",Mmc_Audio_Pcm_Half);

    // prepare for nect frame decode
    if(Mmc_Audio_Pcm_Half==1) // end
    {
        output->pcm.Buffer_head=output->pcm.Buffer;
    }
    else if(Mmc_Audio_Pcm_Half==0) // half
    {
        output->pcm.Buffer_head=output->pcm.Buffer+(output->pcm.Size>>1);
    }
    pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer_head;
}


/*
* NAME: MMC_AUDIODecVoc()
* DESCRIPTION: Decode AAC stream ......
*/
MCI_ERR_T MMC_AUDIODecVoc (MPEG_PLAY *pAudioPlayer)
{
    MPEG_INPUT *input = &pAudioPlayer->MpegInput;
    MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
    vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus = &pAudioPlayer->Voc_AudioDecStatus;
    vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;
    INT32 length;

    vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);
    //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->SampleRate+0x18500000);


    //hal_HstSendEvent(SYS_EVENT,input->inFlag);

    g_VocBusyFlag = 0;

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 1)
    {
        uint32 mpeg_length = 0;//, frames;
        if(input->inFlag==0)
        {
            input->inFlag = 1;
            g_PCMPassFlag = 1;
            return MCI_ERR_NO;
        }
        vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);
        diag_printf("MMC_AUDIODecVoc: inFlag = %d, consumedLen=%d, output_len=%d, ErrorStatus=%d", input->inFlag,pVoc_AudioDecStatus->consumedLen, pVoc_AudioDecStatus->output_len, pVoc_AudioDecStatus->ErrorStatus);
        //     hal_HstSendEvent(SYS_EVENT,AAClearBuffer);
        //     hal_HstSendEvent(SYS_EVENT,AACFRAME);
        //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->output_len);
        //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);

        if(AAClearBuffer>=1)
        {
            AAClearBuffer++;
            if(AAClearBuffer<5)
                memset(pcmbuf_overlay,0,AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
            else
                AAClearBuffer=0;
        }



        if(pVoc_AudioDecStatus->ErrorStatus!=0 && (pVoc_AudioDecStatus->consumedLen==0||pVoc_AudioDecStatus->output_len>AUDIO_MAX_OUTPUT_BUFSIZE))
        {
            hal_HstSendEvent(SYS_EVENT,0x88540000);
            return MCI_ERR_ERROR;
        }
        MCI_MergeSideTone(pVoc_AudioDecStatus->output_len, g_last_buffer_addr);

        if(input->inFlag==1)
        {
            // setup ouput buffer size as dec frame sise
            output->pcm.Size = pVoc_AudioDecStatus->output_len >> 1;
            hal_HstSendEvent(SYS_EVENT,output->pcm.Size);
            if(pVoc_AudioDecStatus->output_len&0x1f) // the length should be 32byte aligned
                return MCI_ERR_ERROR;

            MMC_MpegPcmOutStart(pAudioPlayer);
            input->inFlag = 2;
        }

        length = input->length-pVoc_AudioDecStatus->consumedLen;
        if(length > 0)
        {
            //while(1);
            memcpy(input->bof, input->bof+pVoc_AudioDecStatus->consumedLen, length);
        }
        else
        {
            length = 0;
        }
        input->length = length;
        if(length < AudioJpeg_INPUT_BUFSIZE)
        {
            //Avdtp_Get_Data(&mpeg_length, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE-input->length);
            input->length += mpeg_length;
            diag_printf("MMC_AUDIODecVoc: Avdtp_Get_Data(), mpeg_length=%d, input->length=%d", mpeg_length, input->length);
        }

        if(input->length >= MIN_INPUT_REMAIN_SIZE)
        {
            if(g_PCMPassFlag)
            {
                MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);
                g_ReadFileFlag = 0;
                g_PCMPassFlag = 0;

                // prepare for nect frame decode
                Audio_MpegPrepareNextFrame(pAudioPlayer);
            }
            else
                g_ReadFileFlag = 1;
        }
    }
    else
#endif
    {
        if(input->inFlag==0)
        {
            input->inFlag = 1;

            hal_HstSendEvent(SYS_EVENT,0xabcd0000);

           // decode first frame
#ifdef CODEC_SUPPORT
           if(!(input->length >= MIN_INPUT_REMAIN_SIZE || LeftDataPlayFlag))
           {
                hal_HstSendEvent(SYS_EVENT,0xabcd0001);
                hal_HstSendEvent(SYS_EVENT,input->length);

                COS_Sleep(100);
                length = mmc_aud_FileRead(input->fileHandle, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE - input->length);
                input->length += length;

                hal_HstSendEvent(SYS_EVENT,0xabcd0002);
                hal_HstSendEvent(SYS_EVENT,input->length);
           }
#endif
           MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);


           return MCI_ERR_NO;
        }

        vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);

        diag_printf("MMC_AUDIODecVoc: inFlag = %d, consumedLen=%d, output_len=%d, ErrorStatus=%d,ErrorFrameCount=%d", input->inFlag,pVoc_AudioDecStatus->consumedLen, pVoc_AudioDecStatus->output_len, pVoc_AudioDecStatus->ErrorStatus,ErrorFrameCount);
        //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->consumedLen);
        //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->output_len);
        //hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);
        //g_MPEGConsumeSIZE += pVoc_AudioDecStatus->consumedLen;
        //g_MPEGOutputSIZE += pVoc_AudioDecStatus->output_len;

        if(input->inFlag==1)
        {
            hal_HstSendEvent(SYS_EVENT,0xabcd0003);
            // if the error data can skip, don't return
            if(pVoc_AudioDecStatus->ErrorStatus!=0 && pVoc_AudioDecStatus->consumedLen<=0)
            {
                hal_HstSendEvent(SYS_EVENT,0xabcdfff0);
                Frame_exit=1;
                ErrorFrameCount++;
                return MCI_ERR_ERROR;
            }
            // the data length cannot play one frame.
            g_ConsumedLength += pVoc_AudioDecStatus->consumedLen;
            if(g_ConsumedLength>=g_MPEGFILESIZE)
            {
                hal_HstSendEvent(SYS_EVENT,0xabcdfff1);
                Frame_exit=1;
                ErrorFrameCount++;
                return MCI_ERR_ERROR;
            }
            // setup ouput buffer size as dec frame sise
            if(pVoc_AudioDecStatus->output_len>0&&pVoc_AudioDecStatus->output_len<=AUDIO_MAX_OUTPUT_BUFSIZE)
            {
                output->pcm.Size = pVoc_AudioDecStatus->output_len >> 1;
                //     hal_HstSendEvent(SYS_EVENT,output->pcm.Size);
            }
            else
                output->pcm.Size=0;

#ifdef JUMP_WRONG_MP3_DATA
            if(input->bof[0] == 0xff && ((input->bof[1]&0xf0) == 0xf0) && (MpegPlayer->mode == MMC_MP3_DECODE))
            {
                mp3_head = (UINT8)input->bof[0]<<16 | (UINT8)input->bof[1]<<8 | (UINT8)input->bof[2];
                hal_HstSendEvent(SYS_EVENT,0xabc40000);
                hal_HstSendEvent(SYS_EVENT,input->bof[0]);
                hal_HstSendEvent(SYS_EVENT,input->bof[1]);
                hal_HstSendEvent(SYS_EVENT,input->bof[2]);

                hal_HstSendEvent(SYS_EVENT,mp3_head);
            }
#endif
            // move data from pcmbuf_overlay to inputbuf_overlay;
            length = input->length-pVoc_AudioDecStatus->consumedLen;
            if(length > 0)
            {
                if(length < AudioJpeg_INPUT_BUFSIZE)
                {
#ifdef CODEC_SUPPORT
                    memcpy(input->bof, input->bof+pVoc_AudioDecStatus->consumedLen, length);
                    memset(input->bof+length, 0x00, AudioJpeg_INPUT_BUFSIZE -length);
#else
                    memcpy(inputbuf_overlay, input->bof+pVoc_AudioDecStatus->consumedLen, length);
#endif
                }
            }


            if(length < 0)
            {
                hal_HstSendEvent(SYS_EVENT,0xabcd0004);
                mmc_aud_FileSeek(input->fileHandle,-length,FS_SEEK_CUR);
                length = 0;
            }
#ifdef CODEC_SUPPORT
            input->length = length;
            length = mmc_aud_FileRead(input->fileHandle, input->bof+length, AudioJpeg_INPUT_BUFSIZE -length);
            input->length += length;
#else
            length = mmc_aud_FileRead(input->fileHandle, input->bof+length, input->length-length);
#endif
            //hal_HstSendEvent(SYS_EVENT,length);
            diag_printf("MMC_AUDIODecVoc: Read data length=%d ", length);
            if(length<0)
            {
                hal_HstSendEvent(SYS_EVENT,0xabcdfff2);
                Frame_exit=1;
                ErrorFrameCount++;
                return MCI_ERR_ERROR;
            }
            //output->data=(int16 *)(output->pcm.Buffer+(output->pcm.Size>>1));

            pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)(output->pcm.Buffer+(output->pcm.Size>>1));
#ifdef CODEC_SUPPORT
            if(!(input->length >= MIN_INPUT_REMAIN_SIZE || LeftDataPlayFlag))
            {
                hal_HstSendEvent(SYS_EVENT,0xabcd0005);
                hal_HstSendEvent(SYS_EVENT,input->length);

                COS_Sleep(200);
                length = mmc_aud_FileRead(input->fileHandle, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE - input->length);
                input->length += length;

                hal_HstSendEvent(SYS_EVENT,0xabcd0006);
                hal_HstSendEvent(SYS_EVENT,input->length);

                if(!(input->length >= MIN_INPUT_REMAIN_SIZE || LeftDataPlayFlag))
                {
                    hal_HstSendEvent(SYS_EVENT,0xabcdfff3);

                    Frame_exit=1;
                    ErrorFrameCount++;
                    return MCI_ERR_ERROR;
                }
            }

#endif
            MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);


            //output->data=(int16 *)output->pcm.Buffer;
            pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer;
#ifdef CODEC_SUPPORT
            FrameCount+=2;
#else
            FrameCount++;
#endif
            if( g_GetInfoFlag == 0)
            {
                //    hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->output_len);
                if(pVoc_AudioDecStatus->ErrorStatus==0&&pVoc_AudioDecStatus->output_len>0&&
                   (pVoc_AudioDecStatus->output_len&0x1f)==0&&pVoc_AudioDecStatus->output_len<=AUDIO_MAX_OUTPUT_BUFSIZE&&
                   g_GetInfoFlag== 0&&FrameCount>2)
                {
                    MMC_MpegPcmOutStart(pAudioPlayer);
                    input->inFlag = 2;
                }
                else
                {
                    input->inFlag =1;

                    if(pVoc_AudioDecStatus->ErrorStatus!=0)
                    {
                        ErrorFrameCount++;
                        FrameCount=0;
                    }
                }
            }
            else
            {
                if(pVoc_AudioDecStatus->ErrorStatus!=0||pVoc_AudioDecStatus->output_len<0||
                   (pVoc_AudioDecStatus->output_len&0x1f)!=0||pVoc_AudioDecStatus->output_len>AUDIO_MAX_OUTPUT_BUFSIZE)
                {
                    ErrorFrameCount++;
                    Frame_exit=0;
                }
                else
                {
                    Frame_exit=2;
                }

            }
            if(ErrorFrameCount>=5)
            {
                MCI_TRACE(MCI_AUDIO_TRC, 0,"%s(%d):ErrorFrameCount:%d,FrameCount:%d",__func__,__LINE__,ErrorFrameCount,FrameCount);
                hal_HstSendEvent(SYS_EVENT,0xabc50502);
                return MCI_ERR_ERROR;
#if 0
                MMC_AUDIODecClose(MpegPlayer);
                MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
                mmc_SetCurrMode(MMC_MODE_IDLE);
                ContinuePlayFlag = 0;
#endif
            }
            return MCI_ERR_NO;

        }

#ifndef KALAOK_SUPPORT
        MCI_MergeSideTone(pVoc_AudioDecStatus->output_len, g_last_buffer_addr);
#else
#if 1
	if(input->inFlag == 2)
	{
		uint16 i = 0;
		INT16 *pMicData, *pAudioData;
		INT32 temp_value;
		if(Mmc_Audio_Pcm_Half)
		{
			pMicData = (INT16 *)audioInStream.startAddress + (audioInStream.length >> 2);
		}
		else
		{
			pMicData = (INT16 *)audioInStream.startAddress ;
		}

		pAudioData = (INT16 *)g_last_buffer_addr;

		Effectecho_config(&echo_param, audioInStream.sampleRate, ECHO_DELAY_MAX);

		Effectecho_process(pMicData, pMicData, audioInStream.length >> 2);

		for(i = 0; i<pVoc_AudioDecStatus->output_len/2; i++)
		{
			//*(pAudioData+i) = *(pAudioData+i) >> 1 + *(pMicData+i);
			*(pAudioData+i) = *(pAudioData+i) >> 1;
			//*(pAudioData+i) = *(pMicData+i) << 1;
			temp_value = *(pAudioData+i) + *(pMicData+i);
			if(temp_value > 32767)
			{
				hal_HstSendEvent(SYS_EVENT,0x19890420);

				*(pAudioData+i) = 32767;
			}
			else if(temp_value < -32767)
			{
				hal_HstSendEvent(SYS_EVENT,0x19890421);

				*(pAudioData+i) = -32767;
			}
			else
			{
				*(pAudioData+i) = (INT16)temp_value;
			}
		}
	}
#endif
#endif

        // fulfill inputbuf_overlay
        if(pVoc_AudioDecStatus->consumedLen > 0)
        {
            g_ConsumedLength += pVoc_AudioDecStatus->consumedLen;
            if(g_ConsumedLength>=g_MPEGFILESIZE)
            {
                hal_HstSendEvent(SYS_EVENT,0xabc50000);
                hal_HstSendEvent(SYS_EVENT,g_ConsumedLength);
                hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);
                hal_HstSendEvent(SYS_EVENT,input->length);

                return MCI_ERR_END_OF_FILE;
            }
            length = input->length-pVoc_AudioDecStatus->consumedLen;
            if(length > 0) {
                memcpy(input->bof, input->bof+pVoc_AudioDecStatus->consumedLen, length);
                memset(input->bof+length, 0x00, AudioJpeg_INPUT_BUFSIZE -length);
            }

            if(length < 0)
            {
                mmc_aud_FileSeek(input->fileHandle,-length,FS_SEEK_CUR);
                length = 0;
            }
#ifdef CODEC_SUPPORT
            input->length = length;
            length = mmc_aud_FileRead(input->fileHandle, input->bof+length, AudioJpeg_INPUT_BUFSIZE -length);
            input->length += length;
#else
            length = mmc_aud_FileRead(input->fileHandle, input->bof+length, input->length-length);
#endif

            //hal_HstSendEvent(SYS_EVENT,length);
            if(length<0)
            {
                hal_HstSendEvent(SYS_EVENT,0xabc50001);
                hal_HstSendEvent(SYS_EVENT,input->length);

                return MCI_ERR_END_OF_FILE;
            }

        }

#if 1
        //hal_HstSendEvent(SYS_EVENT,0x88510000);

        if(pVoc_AudioDecStatus->ErrorStatus==0)
        {
            if(g_PCM_SMAPLE_RATE_ERROR&0x80000000)
            {
                if((g_PCM_SMAPLE_RATE_ERROR&0x7fffffff)!=pVoc_AudioDecStatus->SampleRate)
                {
                    hal_HstSendEvent(SYS_EVENT,0x88500000);
                    memset(pAudioPlayer->MpegOutput.pcm.Buffer, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
                    ErrorFrameCount+=4;
                }
                else
                {
                    ErrorFrameCount=0;
                }
                //hal_HstSendEvent(SYS_EVENT,0x19910000+pVoc_AudioDecStatus->SampleRate);
                //hal_HstSendEvent(SYS_EVENT,0x19920000+g_PCM_SMAPLE_RATE_ERROR);
                //hal_HstSendEvent(SYS_EVENT,0x19900000+ErrorFrameCount);


                if(ErrorFrameCount>MP3_MAX_ERROR_CNT)
                {
                    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_MP3AAC_ERROR]ErrorFrameCount:%d",ErrorFrameCount);
                    g_PCM_SMAPLE_RATE_ERROR=0;
                    return MCI_ERR_ERROR;
                }

            }
            else
            {
                ErrorFrameCount=0;
            }
#ifdef MUSIC_BAND_INFO
        vpp_Mp3GetBandInfo(MpegBandInfo);
#endif
        }
        else
        {

            ErrorFrameCount++;
            hal_HstSendEvent(SYS_EVENT,0x88500001);
            hal_HstSendEvent(SYS_EVENT,ErrorFrameCount);
            memset(pAudioPlayer->MpegOutput.pcm.Buffer, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);

            if (LeftDataPlayFlag)
            {
                hal_HstSendEvent(SYS_EVENT,0xabc50002);
                hal_HstSendEvent(SYS_EVENT,input->length);

                return MCI_ERR_END_OF_FILE;
            }

            if(ErrorFrameCount>MP3_MAX_ERROR_CNT)
            {
                hal_HstSendEvent(SYS_EVENT,0xabce0000);
                hal_HstSendEvent(SYS_EVENT,ErrorFrameCount);

                MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_MP3AAC_ERROR]ErrorFrameCount:%d",ErrorFrameCount);
                g_PCM_SMAPLE_RATE_ERROR=0;
                return MCI_ERR_ERROR;
            }
#ifdef JUMP_WRONG_MP3_DATA
            //find next mp3 head 0xFFFX;
            if(pVoc_AudioDecStatus->consumedLen == 0)
            {
                hal_HstSendEvent(SYS_EVENT,0xabc3000a);
                find_head_length = 1;       //avoid loop for one error frame;
                g_ConsumedLength++;
            }
            else
            {
                find_head_length = 0;
            }
            hal_HstSendEvent(SYS_EVENT,0xabc30000);
            hal_HstSendEvent(SYS_EVENT,ErrorFrameCount);

            if(ErrorFrameCount >= 10 && (MpegPlayer->mode == MMC_MP3_DECODE))
            {
                #if 0//only for mike;
                hal_HstSendEvent(SYS_EVENT,0xabc30001);
                hal_HstSendEvent(SYS_EVENT,input->fileHandle);
                if(mmc_aud_FileSeek(input->fileHandle, 0x364610, FS_SEEK_SET)<0)
                {
                    return MCI_ERR_END_OF_FILE;
                }
                else
                {
                    g_ConsumedLength = 0x364610;
                }
                hal_HstSendEvent(SYS_EVENT,input->fileHandle);
                //only for mike;
                #endif

                while(g_ConsumedLength < g_MPEGFILESIZE && mp3_head != 0)
                {
                    UINT16 find_head_flag = 0;
                    INT32 iRet = 0;
                    UINT16 head_low, head_mid, head_high;

                    head_low = input->bof[find_head_length];
                    head_mid = input->bof[find_head_length + 1];
                    head_high = input->bof[find_head_length + 2];

                    if(head_low == 0xff)
                    {
                        if(head_mid == ((mp3_head>>8) & 0xff))
                        {
                            if((head_high & 0xfc) == (mp3_head & 0xfc))
                            {

                                {
                                    hal_HstSendEvent(SYS_EVENT,0xabc30002);
                                    hal_HstSendEvent(SYS_EVENT,find_head_length);
                                    hal_HstSendEvent(SYS_EVENT,mp3_head);
                                    hal_HstSendEvent(SYS_EVENT,head_high);

                                    find_head_flag = 1;
                                }
                            }
                        }

                    }

                    if(find_head_length >= (input->length - 512) || (find_head_flag == 1  && find_head_length > 0))
                    {
                        memcpy(input->bof, input->bof + find_head_length, input->length - find_head_length);
                        iRet = mmc_aud_FileRead(input->fileHandle, input->bof + input->length - find_head_length, find_head_length);

                        COS_Sleep(5);

                        find_head_length = 0;
                    }
                    else
                    {
                        find_head_length++;
                    }

                    if(iRet < 0 || g_ConsumedLength >= g_MPEGFILESIZE)
                    {
                        hal_HstSendEvent(SYS_EVENT,0xabc30003);
                        hal_HstSendEvent(SYS_EVENT,iRet);
                        hal_HstSendEvent(SYS_EVENT,g_ConsumedLength);
                        hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);

                        return MCI_ERR_END_OF_FILE;
                    }

                    if(find_head_flag == 1)
                    {
                        hal_HstSendEvent(SYS_EVENT,0xabc30004);
                        hal_HstSendEvent(SYS_EVENT,g_ConsumedLength);
                        hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);

                        ErrorFrameCount = 0;

                        hal_HstSendEvent(SYS_EVENT,*(UINT32 *)input->bof);

                        break;
                    }
                    g_ConsumedLength++;
                }
            }
#endif

        }

#endif

#if 0
        if(pVoc_AudioDecStatus->ErrorStatus!=0)
        {
            if(MCI_Play_Stream_Buffer)
                return MCI_ERR_ERROR;
            ErrorFrameCount++;

            memset(pAudioPlayer->MpegOutput.pcm.Buffer, 0, 0x2400);
            if(ErrorFrameCount>=MP3_MAX_ERROR_CNT)
            {
                MCI_TRACE (MCI_AUDIO_TRC,0, "%s(%d)ErrorFrameCount:%d",__func__,__LINE__,ErrorFrameCount);
                //  g_PCM_SMAPLE_RATE_ERROR=0;
                return MCI_ERR_ERROR;
            }
        }

#endif

#ifdef CODEC_SUPPORT
        if (g_PCMPassFlag)
        {
           if(input->length >= MIN_INPUT_REMAIN_SIZE  || LeftDataPlayFlag)
           {
                hal_HstSendEvent(SYS_EVENT,0xabcd0007);
                MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);
                g_ReadFileFlag = 0;
                g_PCMPassFlag = 0;


                // prepare for nect frame decode
                Audio_MpegPrepareNextFrame(pAudioPlayer);
           }
           else
           {
                hal_HstSendEvent(SYS_EVENT,0xabcd0008);
                hal_HstSendEvent(SYS_EVENT,input->length);
                hal_HstSendEvent(SYS_EVENT,LeftDataPlayFlag);
                g_ReadFileFlag = 0;
           }
        }
        else
        {
            if(input->length >= MIN_INPUT_REMAIN_SIZE || LeftDataPlayFlag)
            {
                //hal_HstSendEvent(SYS_EVENT,0xabcd0009);
                //hal_HstSendEvent(SYS_EVENT,input->length);
                //hal_HstSendEvent(SYS_EVENT,LeftDataPlayFlag);

                g_ReadFileFlag = 1;
            }
            else
            {
                hal_HstSendEvent(SYS_EVENT,0xabcd000a);
                hal_HstSendEvent(SYS_EVENT,input->length);
                hal_HstSendEvent(SYS_EVENT,LeftDataPlayFlag);
                g_ReadFileFlag = 0;
            }
        }
    }
#else
        if(g_PCMPassFlag)
        {


            MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);


            g_ReadFileFlag = 0;
            g_PCMPassFlag = 0;


            // prepare for nect frame decode
            Audio_MpegPrepareNextFrame(pAudioPlayer);
        }
        else
            g_ReadFileFlag = 1;
    }
#endif


    return MCI_ERR_NO;
}

MCI_ERR_T MMC_AUDIODecPcm(MPEG_PLAY *pAudioPlayer)
{
    //MPEG_INPUT *input = &pAudioPlayer->MpegInput;
    MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
    vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;


    if(ContinuePlayFlag)
    {
        //hal_HstSendEvent(SYS_EVENT,0x65900033);
        diag_printf("!!!!!play all left data in the pcm buffer\n");
        MMC_AUDIODecClose(MpegPlayer);
        MCI_AudioFinished(MCI_ERR_END_OF_FILE);

        mmc_SetCurrMode(MMC_MODE_IDLE);
        ContinuePlayFlag = 0;
        return MCI_ERR_NO;
    }
#ifdef BT_SUPPORT
    if((!g_ReadFileFlag) && MCI_Play_BTStream == 1)
    {
        g_PCMPassFlag = 0;
        MMC_MpegReceiveA2dpData(pAudioPlayer);
    }
#endif

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIODecPcm]g_ReadFileFlag:%d,%d",g_ReadFileFlag,g_VocBusyFlag);
    if(g_VocBusyFlag == 0)
    {
        if(g_ReadFileFlag)
        {
            // file data read, decode one frame

            MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);

            g_PCMPassFlag = 0;
            g_ReadFileFlag = 0;
            // prepare for nect frame decode
            Audio_MpegPrepareNextFrame(pAudioPlayer);
            AACFRAME=0;
        }
        else
        {
            // set flag
            g_PCMPassFlag = 1;
#ifdef CODEC_SUPPORT
            hal_HstSendEvent(SYS_EVENT,0x99400001);
            hal_HstSendEvent(SYS_EVENT,input->length);
            hal_HstSendEvent(SYS_EVENT,LeftDataPlayFlag);

            extern mci_type_enum  g_audio_format;
            INT32 length = 0,remaininglen = 0;

            length = mmc_aud_FileRead(input->fileHandle, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE -input->length);

            input->length += length;

            remaininglen = mmc_GetRemainingBytes() ;

            if ((g_audio_format == MCI_TYPE_TS) && (remaininglen < MPEG2TSPACKETSIZE)) {

                remaininglen = 0;
            }

            if(input->length <= 0)
            {
                hal_HstSendEvent(SYS_EVENT,0x99400002);
                return MCI_ERR_ERROR;
            }
            else if((remaininglen == 0 && input->length > 0) || (input->length >= MIN_INPUT_REMAIN_SIZE))
            {
                // file data read, decode one frame
                MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);

                g_PCMPassFlag = 0;
                g_ReadFileFlag = 0;
                // prepare for next frame decode
                Audio_MpegPrepareNextFrame(pAudioPlayer);
                hal_HstSendEvent(SYS_EVENT,0x99400003);
                hal_HstSendEvent(SYS_EVENT,input->length);
                return MCI_ERR_NO;
            }

#endif
            hal_HstSendEvent(SYS_EVENT,0x88500002);
            memset(output->pcm.Buffer_head, 0, output->pcm.Size<<1);
            Audio_MpegPrepareNextFrame(pAudioPlayer);

            AACFRAME++;
            if(AACFRAME>4)
            {
                AACFRAME=0;
                AAClearBuffer=1;
            }

        }
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT,0x88500003);
        memset(output->pcm.Buffer_head, 0, output->pcm.Size<<1);
        Audio_MpegPrepareNextFrame(pAudioPlayer);
    }
    return MCI_ERR_NO;
}



#ifdef BT_SUPPORT

MCI_ERR_T  MMC_MpegReceiveA2dpData(MPEG_PLAY *pAudioPlayer)
{
    MPEG_INPUT *input = &pAudioPlayer->MpegInput;
    //MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
    //int32 i,readlen;
    vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;
    //uint32 mpeg_length;//, frame_len, frames, length;

    //hal_HstSendEvent(SYS_EVENT,0x10260032);
    if(g_ReadFileFlag)
        return 0;

    if(input->length < AudioJpeg_INPUT_BUFSIZE)
    {
        //Avdtp_Get_Data(&mpeg_length, input->bof+input->length, AudioJpeg_INPUT_BUFSIZE-input->length);
        //input->length += mpeg_length;
    }
    //diag_printf("MMC_MpegReceiveA2dpData: inFlag = %d, mpeg_length=%d, input->length=%d", input->inFlag,mpeg_length, input->length);
    if(input->inFlag!=2)
    {
        if(input->length < MIN_INPUT_REMAIN_SIZE*2)
            return MCI_ERR_NO;
    }

    if(input->length < MIN_INPUT_REMAIN_SIZE)
        return MCI_ERR_NO;

    if(g_PCMPassFlag)
    {
        MMC_DRAININPUTBUFFER(pVoc_Voc_AudioDecIN);
        g_ReadFileFlag = 0;
        g_PCMPassFlag = 0;

        // prepare for nect frame decode
        Audio_MpegPrepareNextFrame(pAudioPlayer);
#if 0
        hal_HstSendEvent(SYS_EVENT,0x10260035);
        hal_HstSendEvent(SYS_EVENT,Mmc_Audio_Pcm_Half);
        hal_HstSendEvent(SYS_EVENT,pVoc_Voc_AudioDecIN->outStreamBufAddr);
        hal_HstSendEvent(SYS_EVENT,output->pcm.Size);
#endif
    }
    else
        g_ReadFileFlag = 1;

    return MCI_ERR_NO;

}
#endif

int32 Audio_MpegPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
    int32 result;
    MCI_TRACE (MCI_AUDIO_TRC,0,"Audio_MpegPlay,fhd is %d",fhd);

    hal_HstSendEvent(SYS_EVENT,0x88000000);

    UINT8 userId = aud_MutexTake();


    if(MpegPlayer == NULL)
        MpegPlayer = COS_Malloc(sizeof(MPEG_PLAY));

    g_MpegResumeDataLen = 0;

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 0)
#endif
    {
        //g_FileHandle=fhd;
        if (fhd< 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]ERROR file hander: %d \n", fhd);

            aud_MutexRelease(userId);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        if ((g_MPEGFILESIZE=mmc_aud_GetFileSize(fhd))<= 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]can not get file size!:%d",g_MPEGFILESIZE);

            aud_MutexRelease(userId);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]File Size: %d",g_MPEGFILESIZE);

        g_ConsumedLength=(INT32)(((INT64)PlayProgress*(INT64)g_MPEGFILESIZE)/10000);

        hal_HstSendEvent(SYS_EVENT,0x88000010);
        hal_HstSendEvent(SYS_EVENT,PlayProgress);
        if ((result=mmc_aud_FileSeek(fhd,g_ConsumedLength,FS_SEEK_SET))< 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]can not seek file!:%d",result);
            aud_MutexRelease(userId);

            return MCI_ERR_CANNOT_OPEN_FILE;
        }
    }

    MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]g_AMRConsumedLength: %d",g_ConsumedLength);
    switch(filetype)
    {
        case MCI_TYPE_TS:
            MpegPlayer->mode = MMC_MPEG_NONE;
            break;
        case MCI_TYPE_DAF:
            MpegPlayer->mode = MMC_MP3_DECODE;
            break;

        case MCI_TYPE_AAC:
            MpegPlayer->mode = MMC_AAC_DECODE;
            break;

        default:
            hal_HstSendEvent(SYS_EVENT,0x8800852);
            diag_printf("Audio_MpegPlay()->unsupport record media type");
            while(1);
            break;
    }

    if((result= MMC_AudioDecOpen(fhd, MpegPlayer))!=MCI_ERR_NO)
    {
        diag_printf("[MMC_AUDIO]##WARNING##MMC_AudioDecOpen() return false!\n");
        MMC_AUDIODecClose(MpegPlayer);

        aud_MutexRelease(userId);
        return result;
    }

    aud_MutexRelease(userId);

    return  MCI_ERR_NO;
}

int32 Audio_MpegStop (void)
{
    //clear the flag used to play left data in the input buffer and pcm buffer
    UINT8 userId = aud_MutexTake();

    ContinuePlayFlag=0;
    g_MpegResumeDataLen = 0;

    MMC_AUDIODecClose(MpegPlayer);

    aud_MutexRelease(userId);
    return  MCI_ERR_NO;
}

int32 Audio_MpegPlayLeftData (void)
{
    //clear the flag used to play left data in the input buffer and pcm buffer
    LeftDataPlayFlag=1;

    return  MCI_ERR_NO;
}

int32 Audio_MpegSetTestMode (uint8 mode, AUDIO_FACTORY_TEST_HANDLER_T hld)
{
    MCI_TRACE (MCI_AUDIO_TRC,0,"Audio_MpegSetTestMode: %d, 0x%x", mode, hld);

    g_AudioTestMode = mode;
    g_mmcAudioFactoryTestHandler = hld;
    g_AudioFTDetectPassCount = 0;
    g_AudioFTDetectCount = 0;

    return  MCI_ERR_NO;
}

int32 Audio_MpegPause (void)
{
    //MPEG_INPUT *input = &MpegPlayer->MpegInput;
    //Drv_PcmOutStop();
    UINT8 userId = aud_MutexTake();
    if (MpegPlayer == NULL)
    {
        aud_MutexRelease(userId);
        return MCI_ERR_NO;
    }
    g_AudioMode=MpegPlayer->mode;
    MCI_TRACE (MCI_AUDIO_TRC,0, "******************* Audio_MpegPause**********************");


    //clear the flag used to play left data in the input buffer and pcm buffer
    ContinuePlayFlag=0;
#ifndef CODEC_SUPPORT
    MMC_AUDIODecClose(MpegPlayer);
#else
    aud_StreamPause(audioItf,TRUE);
#endif
    aud_MutexRelease(userId);

    return  MCI_ERR_NO;
}


int32 Audio_MpegResume ( HANDLE fhd)
{
    int32 result;
    UINT8 userId = aud_MutexTake();

    if (MpegPlayer == NULL)
        MpegPlayer = COS_Malloc(sizeof(MPEG_PLAY));

    MpegPlayer->mode=g_AudioMode;


#ifndef CODEC_SUPPORT
    g_ConsumedLength-=2048;//VoC buffer length

    if(g_ConsumedLength<0)g_ConsumedLength=0;

    result=mmc_aud_FileSeek(fhd,g_ConsumedLength,FS_SEEK_SET);

    MCI_TRACE (MCI_AUDIO_TRC,0,"FS_seek pos:%d",result);

    if(result<0 )
    {
        MCI_TRACE (MCI_AUDIO_TRC,0,"FS_seek error!");

        aud_MutexRelease(userId);
        return -1;
    }
    if((result= MMC_AudioDecOpen(fhd, MpegPlayer))!=MCI_ERR_NO)
    {
        diag_printf("[MMC_AUDIO]##WARNING##MMC_AudioDecOpen() return false!\n");
        MMC_AUDIODecClose(MpegPlayer);

        aud_MutexRelease(userId);
        return result;
    }
#else
    g_ConsumedLength = 0;
    aud_StreamPause(audioItf,FALSE);
#endif

    aud_MutexRelease(userId);
    return  MCI_ERR_NO;
}


int32 Audio_MpegGetID3 (char * pFileName)
{
    return MCI_ERR_NO;
}
#ifdef MUSIC_BAND_INFO
int32 Audio_MpegGetFreqBandInfo(UINT16 *band)
{
       UINT16 i;

       for(i = 0; i < 8; i++)
       {
        band[i] = MpegBandInfo[i];
    }

    return MCI_ERR_NO;
}
#endif

int32 Audio_MpegGetPlayInformation (MCI_PlayInf * MCI_PlayInfMPEG)
{
    if(g_MPEGFILESIZE==0 || g_ConsumedLength < g_id3_tagsize_flag || g_MPEGFILESIZE <= g_id3_tagsize_flag )
        MCI_PlayInfMPEG->PlayProgress = 0;
    else
        MCI_PlayInfMPEG->PlayProgress=(INT32)((((INT64)g_ConsumedLength - g_id3_tagsize_flag)*10000)/(g_MPEGFILESIZE - g_id3_tagsize_flag));

    MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_MPEG]PlayProgress:%d,g_MPEGFILESIZE=%d,g_DecOpenFlag=%d",MCI_PlayInfMPEG->PlayProgress,g_MPEGFILESIZE,g_DecOpenFlag);

    //hal_HstSendEvent(SYS_EVENT,0x89300000);
    //hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);
    //hal_HstSendEvent(SYS_EVENT,g_ConsumedLength);
    //hal_HstSendEvent(SYS_EVENT,g_id3_tagsize_flag);
        
    return MCI_ERR_NO;
}

int32 Audio_MpegGetFileInformation(HANDLE fhd, AudDesInfoStruct  * CONST DecInfo, int16 filetype)
{
    int32 result, wait;
    //COS_EVENT ev;

    MCI_TRACE (MCI_AUDIO_TRC,0,"Audio_MpegPlay,fhd is %d",fhd);
    if(MpegPlayer == NULL)
        MpegPlayer = COS_Malloc(sizeof(MPEG_PLAY));

    vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus = &MpegPlayer->Voc_AudioDecStatus;

    //hal_HstSendEvent(SYS_EVENT,0x88000000);
    g_GetInfoFlag = 0;
    if(g_DecOpenFlag)
    {
        // hal_HstSendEvent(SYS_EVENT,0x12150010);
        wait = 0;
        while( ErrorFrameCount==0&&MpegPlayer->MpegInput.inFlag <2)
        {
            hal_HstSendEvent(SYS_EVENT,0x88000024);
            COS_Sleep(10);
            hal_HstSendEvent(SYS_EVENT,0x88000018);
            hal_HstSendEvent(SYS_EVENT,wait);
            wait ++;
            if(wait>10)
                break;
        }
        DecInfo->sampleRate = pVoc_AudioDecStatus->SampleRate;
        DecInfo->stereo = pVoc_AudioDecStatus->nbChannel==2;
        if(pVoc_AudioDecStatus->BitRate != 0)
        {
            DecInfo->bitRate = pVoc_AudioDecStatus->BitRate*1000;
            DecInfo->time = g_MPEGFILESIZE*8/pVoc_AudioDecStatus->BitRate+100;
        }
        else
        {
            DecInfo->time = DecInfo->bitRate = 0;
        }

        hal_HstSendEvent(SYS_EVENT,0x89200000);
        hal_HstSendEvent(SYS_EVENT,DecInfo->sampleRate);
        hal_HstSendEvent(SYS_EVENT,DecInfo->bitRate);
        hal_HstSendEvent(SYS_EVENT,DecInfo->time);
        hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);
        hal_HstSendEvent(SYS_EVENT,g_id3_tagsize_flag);

        return MCI_ERR_NO;
    }


#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 0)
#endif
    {
        //g_FileHandle=fhd;
        if (fhd< 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]ERROR file hander: %d \n", fhd);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        if ((g_MPEGFILESIZE=mmc_aud_GetFileSize(fhd))< 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]can not get file size!:%d",g_MPEGFILESIZE);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]File Size: %d",g_MPEGFILESIZE);
        //get id3 size.
       { 
               UINT8 id3_buf[10];
               INT32 readlen;

	        readlen = mmc_aud_FileRead(fhd, id3_buf, 10);

	        if ((id3_buf[0]=='I')&&(id3_buf[1]=='D')&&(id3_buf[2]=='3'))
	        {
	            hal_HstSendEvent(SYS_EVENT,0x1D300001);
	            /* high bit is not used */
	            g_id3_tagsize_flag = (id3_buf[6] << 21) | (id3_buf[7] << 14) |
	                      (id3_buf[8] <<  7) | (id3_buf[9] <<  0);

	            g_id3_tagsize_flag += 10;
	        }
	        else
	        {
	            g_id3_tagsize_flag = 0;
	        }
	        
		hal_HstSendEvent(SYS_EVENT,0x89400000);
		hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);
		hal_HstSendEvent(SYS_EVENT,g_id3_tagsize_flag);
		
              if(g_id3_tagsize_flag >= g_MPEGFILESIZE)
              {
              	return MCI_ERR_CANNOT_OPEN_FILE;
              }
        }
        hal_HstSendEvent(SYS_EVENT,0x88000010);
        if ((result=mmc_aud_FileSeek(fhd,g_MPEGFILESIZE/2,FS_SEEK_SET))< 0) // from middle of file to get information
        {
            diag_printf("[MMC_AUDIO_ERROR]can not seek file!:%d",result);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        g_ConsumedLength = 0;

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]g_AMRConsumedLength: %d",g_ConsumedLength);
    }

    switch(filetype)
    {
        case MCI_TYPE_DAF:
            MpegPlayer->mode = MMC_MP3_DECODE;
            break;

        case MCI_TYPE_AAC:
            MpegPlayer->mode = MMC_AAC_DECODE;
            break;

        default:
            hal_HstSendEvent(SYS_EVENT,0x88000963);
            diag_printf("Audio_MpegPlay()->unsupport record media type");
            while(1);
            break;
    }

    ErrorFrameCount = 0;
    g_GetInfoFlag = 1;
    Frame_exit=0;
    if((result= MMC_AudioDecOpen(fhd, MpegPlayer))!=MCI_ERR_NO)
    {
        diag_printf("[MMC_AUDIO]##WARNING##MMC_AudioDecOpen() return false!\n");
        MMC_AUDIODecClose(MpegPlayer);
        g_GetInfoFlag = 0;
        return result;
    }

    wait = 0;
    while(Frame_exit==0&& ErrorFrameCount<5)
    {
        COS_Sleep(10);
        hal_HstSendEvent(SYS_EVENT,0x88000018);
        hal_HstSendEvent(SYS_EVENT,wait);
        wait ++;
        if(wait>10)
            break;
    }

    if( pVoc_AudioDecStatus->BitRate != 0&& ErrorFrameCount<5)
    {
        DecInfo->sampleRate = pVoc_AudioDecStatus->SampleRate;
        DecInfo->stereo = pVoc_AudioDecStatus->nbChannel==2;
        DecInfo->bitRate = pVoc_AudioDecStatus->BitRate*1000;
        DecInfo->time = (g_MPEGFILESIZE - g_id3_tagsize_flag)*8/pVoc_AudioDecStatus->BitRate+100;
        //DecInfo->time = ((INT64)g_MPEGFILESIZE)*g_MPEGOutputSIZE/g_MPEGConsumeSIZE/2/pVoc_AudioDecStatus->nbChannel/(pVoc_AudioDecStatus->SampleRate/1000);

        hal_HstSendEvent(SYS_EVENT,0x89100000);
        hal_HstSendEvent(SYS_EVENT,DecInfo->sampleRate);
        hal_HstSendEvent(SYS_EVENT,DecInfo->bitRate);
        hal_HstSendEvent(SYS_EVENT,DecInfo->time);
        hal_HstSendEvent(SYS_EVENT,g_MPEGFILESIZE);
        hal_HstSendEvent(SYS_EVENT,g_id3_tagsize_flag);
    }
    else
        result = MCI_ERR_ERROR;

    MMC_AUDIODecClose(MpegPlayer);
    mmc_ExitMode();

    g_GetInfoFlag = 0;

    return  result;

}

UINT16 PA_Count=0;
int32 Audio_MpegUserMsg(COS_EVENT *ev)
{
    int32 nMsg = ev->nEventId;

    UINT8 userId = aud_MutexTake();
    //ignore messages when Decode is not open
    //Fix bug 33063: recvice previous MSG_MMC_AUDIODEC_VOC when exeutting Audio_MpegPlay
    if (0 == g_DecOpenFlag)
    {
        hal_HstSendEvent(SYS_EVENT,0xabc20001);
        aud_MutexRelease(userId);
        return 1;
    }

    switch (nMsg)
    {
        case MSG_MMC_AUDIODEC_VOC:

            switch (MMC_AUDIODecVoc(MpegPlayer))
            {
                case MCI_ERR_END_OF_FILE:
                    diag_printf("[MMC_AUDIO]MMC_AACDecRun() return FINISH!\n");

                    ContinuePlayFlag = 1;
                    ErrorFrameCount ++;
                    break;
                case MCI_ERR_ERROR:
#if 0
                    hal_HstSendEvent(SYS_EVENT,0x65900022);
                    diag_printf("[MMC_AUDIO]##WARNING##MMC_AACDecRun() return FALSE!\n");
                    ErrorFrameCount ++;
#ifdef BT_SUPPORT
                    if(MCI_Play_BTStream)
                    {
                        ErrorFrameCount ++;
                       // Avdtp_Clean_Data();
                        break;
                    }
#endif
                    if(g_GetInfoFlag == 0)
                    {
                        MMC_AUDIODecClose(MpegPlayer);
                        if(g_AudioSeekFlag==1)
                        {
                            MCI_AudioFinished(MCI_ERR_END_OF_FILE);
                        }
                        else
                        {
                            MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
                        }
                    }

#endif
#ifdef BT_SUPPORT
                    if(MCI_Play_BTStream)
                    {
                        ErrorFrameCount ++;
                        //Avdtp_Clean_Data();
                        break;
                    }
#endif

                    MMC_AUDIODecClose(MpegPlayer);
                    MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
                    mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
                    ContinuePlayFlag = 0;
                    break;
                default:
                    break;
            }
            break;
#ifdef BT_SUPPORT
        case MSG_MMC_AUDIODEC_A2DP:
            MMC_MpegReceiveA2dpData(MpegPlayer);
            break;
#endif

        case MSG_MMC_AUDIODEC_PCM:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
            if(PA_Count<= 8)
            {
                PA_Count++;
            }
            else
            {
               hal_abbFirstEnablePa();
            }
#endif
            flag_PCM--;
            switch (MMC_AUDIODecPcm(MpegPlayer))
            {
                case MCI_ERR_ERROR:
                    MMC_AUDIODecClose(MpegPlayer);
              hal_HstSendEvent(SYS_EVENT, 0x2018f004);
                    MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
                    mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
                    ContinuePlayFlag = 0;
            default:
                break;
         }

            break;
        default:
            break;
    }
    aud_MutexRelease(userId);
    return 0;
}


