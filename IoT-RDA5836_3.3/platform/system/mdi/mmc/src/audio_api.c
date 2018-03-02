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

#include "mcip_debug.h"

#include "audio_api.h"
#include "mmc.h"
#include "lily_statemachine.h"


//#include "med_main.h" // to be removed 

extern SBC_PLAY *SBCPlayer;
extern Audio_Selector gAudio;
extern MPEG_PLAY *MpegPlayer;
extern WMA_PLAY *WMA_PLAYER;

static int32 gAudioType;


uint32 LILY_AudioPlay (int32 OutputPath, HANDLE fileHandle, int16 fielType,INT32 PlayProgress) 	//	MCI_MEDIA_PLAY_REQ,			
{
	int32 result;
    Mmc_Api_Msg_Msg api_msg;

	if(fielType == MCI_TYPE_NONE)
	{
		return MCI_ERR_BAD_FORMAT;
	}
	
	gAudioType = fielType;
	lily_changeToState(STM_MOD_AUD_PLAY);

    api_msg.msg_id = MMC_APIMSG_AUDIO_PLAY;
    api_msg.p.a_play.OutputPath = OutputPath;
    api_msg.p.a_play.fileHandle = fileHandle;
    api_msg.p.a_play.fielType = fielType;
    api_msg.p.a_play.PlayProgress = PlayProgress;
    result = mmc_SendApiMsgReq(&api_msg);

	if (result != MCI_ERR_NO)
	{
  		lily_exitModule(STM_MOD_AUD_PLAY);
	}

	return result;
}

uint32 LILY_AudioPause(void)
{
	int32 result;
    Mmc_Api_Msg_Msg api_msg;

  	lily_exitModule(STM_MOD_AUD_PLAY);
  	lily_changeToState(STM_MOD_AUD_PAUSE);

    api_msg.msg_id = MMC_APIMSG_AUDIO_PAUSE;
    result = mmc_SendApiMsgReq(&api_msg);

	return result;
}

uint32 LILY_AudioResume(HANDLE fileHandle)
{
	int32 result;
    Mmc_Api_Msg_Msg api_msg;

  	lily_exitModule(STM_MOD_AUD_PAUSE);
 	lily_changeToState(STM_MOD_AUD_PLAY);

    api_msg.msg_id = MMC_APIMSG_AUDIO_RESUME;
    api_msg.p.a_resume.fileHandle = fileHandle;
    result = mmc_SendApiMsgReq(&api_msg);
	
	return result;
}

uint32 LILY_AudioSeek(int32 time)
{
   return 0;
}

uint32 LILY_AudioStop(void)
{
	int32 result = MCI_ERR_NO;
    Mmc_Api_Msg_Msg api_msg;

    api_msg.msg_id = MMC_APIMSG_AUDIO_STOP;
    result = mmc_SendApiMsgReq(&api_msg);

  	lily_exitModule(STM_MOD_AUD_PLAY);
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO]media stop\n");
	return result;
}

uint32 LILY_AudioPlayLeftData(void)
{
	int32 result = MCI_ERR_NO;
	Mmc_Api_Msg_Msg api_msg;

	api_msg.msg_id = MMC_APIMSG_AUDIO_PLAY_LEFT_DATA;
	result = mmc_SendApiMsgReq(&api_msg);

	lily_exitModule(STM_MOD_AUD_PLAY);
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO]media stop play all left data\n");
	return result;
}
/*uint32 LILY_AudioGetID3(int8 *pfilename,ID3INFO *id3info) 			
{
	int32 result;
	int32 audiotype;

	audiotype = med_get_media_type((kal_wchar*)pfilename);
	mmc_EntryMode(MMC_MODE_AUDIO, audiotype);
	ass(gAudio.GetID3 == 0);        
	result = gAudio.GetID3(pfilename);
	mmc_ExitMode();

	return result;
}*/

uint32 LILY_AudioGetPlayInformation(MCI_PlayInf* PlayInformation) 			
{
	uint32 result;
    Mmc_Api_Msg_Msg api_msg;
	
	ass(PlayInformation == NULL);

	api_msg.msg_id = MMC_APIMSG_AUDIO_GETPLAYINFO;
	api_msg.p.a_getinfo.PlayInformation = PlayInformation;
	result = mmc_SendApiMsgReq(&api_msg);
	return result;
}
	

uint32 LILY_AudioSetEQ(AUDIO_EQ EQMode)
{
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO]LILY_AudioSetEQ()->EQMode:%d\n",EQMode);
	
	switch(gAudioType)
	{
		case MCI_TYPE_DAF:
		case MCI_TYPE_AAC:
            if(MpegPlayer != NULL)
    			MpegPlayer->Voc_AudioDecIN.EQ_Type = EQMode-1;
			break;
#ifndef TARGET_WMA_SUPPORT_OFF
		case MCI_TYPE_WMA:
            if(WMA_PLAYER != NULL)
	    		WMA_PLAYER->EQMode = EQMode;
		    break;
#endif
		case MCI_TYPE_SBC:
            if(SBCPlayer != NULL)
                SBCPlayer->Voc_AudioDecIN.EQ_Type = EQMode;
		  vpp_SBCDecSetAudioParamReloadFlag(2);// 2:key set eq; 3:calib set eq;
            break;
		default:
			MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO]audio type unsupport set eq\n");
			break;
	}
	return 1;
}

#ifdef MUSIC_BAND_INFO
uint32 LILY_AudioGetFreqBandInfo(uint16 * FreqBand) 			
{
	uint32 result;
    Mmc_Api_Msg_Msg api_msg;
	
	ass(FreqBand == NULL);

	api_msg.msg_id = MMC_APIMSG_AUDIO_GETFREQINFO;
	api_msg.p.a_freqinfo.FreqBand = FreqBand;
	result = mmc_SendApiMsgReq(&api_msg);
	return result;
}
#endif
