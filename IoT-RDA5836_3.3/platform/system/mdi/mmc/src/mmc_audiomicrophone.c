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

// FILENAME: mmc_audiolineinRecord.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cswtype.h"
#include "mcip_debug.h"
#include "fs.h"

#include "mmc_audiomicrophone.h"

#include "mmc.h"
#include "cpu_share.h"

#include "audio_api.h"

#include "aud_m.h"
#include "fs_asyn.h"

#include "cos.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "event.h"

#ifdef KALAOK_SUPPORT
#include "Effectecho.h"
#endif

#ifdef KALAOK_SUPPORT

//#define SBCENC_DEBUG
#define MICROPHONE_DEBUG_TICKS

#define MICROPHONE_REC_PCM_BUF_LEN (4*1024)
#define MICROPHONE_PLAY_PCM_BUF_LEN (4*1024)

#define MicroPhone_rec_pcm_buffer pcmbuf_overlay
#define MicroPhone_play_pcm_buffer (pcmbuf_overlay+MICROPHONE_REC_PCM_BUF_LEN/4)

#define MicroPhone_Echo_buffer (MicroPhone_play_pcm_buffer+MICROPHONE_PLAY_PCM_BUF_LEN/4)

MICROPHONE_PLAY *MicroPhonePlayer;

//INT32 *G_VppSBCEncBakConstY=NULL;
extern const INT32 G_VppSBCConstX[];

UINT8* sbc_buffer;

static UINT8 sbc_will_encoding = 0;

#ifdef MICROPHONE_DEBUG_TICKS
    UINT32 g_MicroPhoneHalfEndTicks = 0;
#endif

extern HAL_AIF_STREAM_T audioInStream;
extern effectecho_param echo_param;// 1:delay times ms; 2:16384 -> 0.5;
extern UINT16 * echo_delay_buff;

UINT8 sbc_test_timer_id = 0;
UINT8 sbc_test_flag = 0;
UINT8* test_buffer = NULL;
UINT8* sbc_test_buffer = NULL;
UINT8* pcm_test_buffer = NULL;
/**/
INT32 pcm_cnt = -1;
UINT32 sbc_cnt = 0;
UINT8 g_MicroPhoneRecHalfEnd = 0;


extern AUD_LEVEL_T audio_cfg;
extern AUD_ITF_T audioItf;
/*
* NAME:	MMC_AudioMicroPhoneVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioMicroPhoneVocISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]VoC INT!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_MICROPHONE_SBC_VOC);
} 

/*
* NAME:	MMC_AudioMicroPhoneVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioMicroPhoneRecHalfISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]REC HALF INT!");
#ifdef MICROPHONE_DEBUG_TICKS
    UINT32 microphone_tick;
	microphone_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x11100000 + microphone_tick - g_MicroPhoneHalfEndTicks);
	g_MicroPhoneHalfEndTicks = microphone_tick;
#endif
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_MICROPHONE_REC_HALF);
} 

/*
* NAME:	MMC_AudioMicroPhoneVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioMicroPhoneRecEndISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]REC END INT!");
#ifdef MICROPHONE_DEBUG_TICKS
    UINT32 microphone_tick;
	microphone_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x22200000 + microphone_tick - g_MicroPhoneHalfEndTicks);
	g_MicroPhoneHalfEndTicks = microphone_tick;
#endif
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_MICROPHONE_REC_END);
} 

/*
* NAME:	MMC_AudioMicroPhoneDecOpen()
* DESCRIPTION: Initialize linein decoder.
*/
MCI_ERR_T MMC_AudioMicroPhoneDecOpen (HANDLE fhd, MICROPHONE_PLAY *pMicroPhonePlayer)
{
    hal_HstSendEvent(SYS_EVENT, 0xad0a0000);
        
    if(pMicroPhonePlayer == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xad0a0001);
    }      
    memset(pMicroPhonePlayer, 0, sizeof(MICROPHONE_PLAY));
    pcm_cnt = 0;
    
    MICROPHONE_INPUT *input = &pMicroPhonePlayer->MicroPhoneInput;
    MICROPHONE_OUTPUT *output = &pMicroPhonePlayer->MicroPhoneOutput;        

    input->data = NULL;
    output->Buffer = NULL;

    memset(MicroPhone_rec_pcm_buffer, 0x00, MICROPHONE_REC_PCM_BUF_LEN);
    memset(MicroPhone_play_pcm_buffer, 0x00, MICROPHONE_PLAY_PCM_BUF_LEN);

    echo_delay_buff = (UINT16 *)MicroPhone_Echo_buffer;
    memset(echo_delay_buff, 0x00, ECHO_DELAY_MAX);

    Effectecho_init(ECHO_DELAY_MAX, echo_delay_buff);

    hal_HstSendEvent(SYS_EVENT, 0xad0a060b);

    MMC_MicroPhonePcmOutStart(audioItf, pMicroPhonePlayer);
    return MCI_ERR_NO;
	
}


VOID MMC_MicroPhonePcmOutStart(MICROPHONE_PLAY *pMicroPhonePlayer)
{
	// Local variable passed to a function, but copied in the function.
	HAL_AIF_STREAM_T inAudioStream;
	HAL_AIF_STREAM_T outAudioStream;

	// Initial cfg
	//if(gpio_detect_earpiece())
	//    *itf = AUD_ITF_EAR_PIECE;
	//else
	//    *itf = AUD_ITF_LOUD_SPEAKER;
	hal_HstSendEvent(SYS_EVENT, 0xad0a1628);

	// Initial cfg
	if(gpio_detect_earpiece())
		audioItf = AUD_ITF_BT_EP;
	else
		audioItf = AUD_ITF_BT_SP;

	audioInStream.startAddress = MicroPhone_rec_pcm_buffer;
	audioInStream.length       = MICROPHONE_REC_PCM_BUF_LEN;
	audioInStream.sampleRate   = HAL_AIF_FREQ_44100HZ;
	audioInStream.channelNb    = HAL_AIF_MONO;//HAL_AIF_STEREO;
	audioInStream.voiceQuality = !TRUE;
	audioInStream.playSyncWithRecord = FALSE;
	audioInStream.halfHandler  = MMC_AudioMicroPhoneRecHalfISR;
	audioInStream.endHandler   = MMC_AudioMicroPhoneRecEndISR;

	outAudioStream.startAddress = MicroPhone_play_pcm_buffer;
	outAudioStream.length       = MICROPHONE_PLAY_PCM_BUF_LEN;
	outAudioStream.sampleRate   = HAL_AIF_FREQ_44100HZ;
	outAudioStream.channelNb    = HAL_AIF_MONO;//HAL_AIF_STEREO;
	outAudioStream.voiceQuality = !TRUE;
	outAudioStream.playSyncWithRecord = FALSE;
	outAudioStream.halfHandler  = NULL;//MMC_AudioMicroPhoneRecHalfISR;
	outAudioStream.endHandler   = NULL;//MMC_AudioMicroPhoneRecEndISR;
	
	while (aud_StreamStart(audioItf, &outAudioStream, &audio_cfg) != AUD_ERR_NO)
	{
		sxr_Sleep(1 MS_WAITING);
		hal_HstSendEvent(SYS_EVENT, 0xad0a1629);
	}

	while (aud_StreamRecord(audioItf, &audioInStream, &audio_cfg) != AUD_ERR_NO)
	{
		sxr_Sleep(1 MS_WAITING);
		hal_HstSendEvent(SYS_EVENT, 0xad0a1632);
	}
	//aud_TestModeSetup(audioItf, &sideAudioStream, &audio_cfg, AUD_TEST_SIDE_TEST);
}


MCI_ERR_T MMC_AudioMicroPhonePcm(UINT32 rec_half_end)
{
    INT16 i;

    if(MicroPhone_rec_pcm_buffer == NULL)
    {
        hal_DbgAssert("Rec_pcm_buffer is NULL in MMC_AudioMicroPhonePcm...");
    }

    INT16* pMicData = (INT16* )(rec_half_end?MicroPhone_rec_pcm_buffer+MICROPHONE_REC_PCM_BUF_LEN/8: MicroPhone_rec_pcm_buffer);
    INT16* pSpkData = (INT16* )(rec_half_end?MicroPhone_play_pcm_buffer+MICROPHONE_PLAY_PCM_BUF_LEN/8: MicroPhone_play_pcm_buffer);


    Effectecho_config(&echo_param, audioInStream.sampleRate, ECHO_DELAY_MAX);

    Effectecho_process(pMicData, pMicData, audioInStream.length >> 2);
	    
    for(i = 0; i<MICROPHONE_REC_PCM_BUF_LEN/4; i++)
    {
        *(pSpkData+i) = *(pMicData+i);
    }

    return MCI_ERR_NO;
}
/*
* NAME:	MMC_AudioMicroPhoneDecClose()
* DESCRIPTION: Close linein decoder.
*/
MCI_ERR_T MMC_AudioMicroPhoneDecClose (MICROPHONE_PLAY *pMicroPhonePlayer)
{
    hal_HstSendEvent(SYS_EVENT,0xad0a0101);

    if(pMicroPhonePlayer != NULL)
    {
        COS_Free(pMicroPhonePlayer);
    }
    else
    {
        return  MCI_ERR_NO;
    }

    aud_StreamStop(audioItf);

    mmc_MemFreeAll();

    return  MCI_ERR_NO;
	
}


int32 Audio_MicroPhonePlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
    int32 result;

    if(MicroPhonePlayer == NULL)
        MicroPhonePlayer = COS_Malloc(sizeof(MICROPHONE_PLAY));
    if((result= MMC_AudioMicroPhoneDecOpen(fhd, MicroPhonePlayer))!=MCI_ERR_NO)
    {
    	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_MICROPHONE]MMC_AudioMicroPhoneDecOpen() RETURN FALSE!");
    	MMC_AudioMicroPhoneDecClose(MicroPhonePlayer);
    	return result;
    }
    
    return  MCI_ERR_NO;
}

int32 Audio_MicroPhoneStop (void) 	
{
	MMC_AudioMicroPhoneDecClose(MicroPhonePlayer);
	return  MCI_ERR_NO;
}


int32 Audio_MicroPhonePause (void)	
{
        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_MICROPHONE] Audio_MicroPhonePause!!!!");
	
	return  MCI_ERR_NO;
}


int32 Audio_MicroPhoneResume ( HANDLE fhd) 
{	
	return  MCI_ERR_NO;
}


int32 Audio_MicroPhoneGetID3 (char * pFileName)  
{
	return 0;	
}

int32 Audio_MicroPhoneGetPlayInformation (MCI_PlayInf * MCI_PlayInfMicroPhone)  
{
	return MCI_ERR_NO;
}

int32 Audio_MicroPhoneUserMsg(COS_EVENT *ev)
{

    int32 nMsg = ev->nEventId;  

	switch (nMsg)
	{
		case MSG_MMC_MICROPHONE_REC_HALF:
			 MMC_AudioMicroPhonePcm(0);
		break;
		
		case MSG_MMC_MICROPHONE_REC_END:
			 MMC_AudioMicroPhonePcm(1);
		break;



		default:
		        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_MICROPHONE]Audio_MicroPhoneUserMsg receives unexpected event!!!! nMsg = %d", nMsg);
		         break;

	}
    return 0;
}

#endif

