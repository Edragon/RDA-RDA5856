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

// FILENAME: mmc_audiolineinRecord.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "mci.h"
#include "mmc.h"
#include "vpp_audiosbc_enc.h"
#include "mmc_adpcm.h"

#ifndef MMC_AUDIOMICROPHONE_H
#define MMC_AUDIOMICROPHONE_H

#define SOURCE_LINE_IN 1
#define SOURCE_MIC_IN  2

#define MSG_MMC_MICROPHONE_PCM_INT  1
#define MSG_MMC_MICROPHONE_PCM_END  2
#define MSG_MMC_MICROPHONE_SBC_VOC  3
#define MSG_MMC_MICROPHONE_REC_HALF 4
#define MSG_MMC_MICROPHONE_REC_END  5

typedef struct MICROPHONE_INPUT 
{
	uint8 *data; 	
	uint32 length;
         uint8 is_first_frame;
} MICROPHONE_INPUT;

typedef struct MICROPHONE_OUTPUT 
{	
	uint32 *Buffer;		//Pointer for PCM buffer
	uint32 *Buffer_head;
} MICROPHONE_OUTPUT;

typedef struct MICROPHONE_PLAY 
{
	MICROPHONE_INPUT  MicroPhoneInput;
	MICROPHONE_OUTPUT  MicroPhoneOutput;

	vpp_AudioSbc_ENC_OUT_T Voc_AudioEncStatus;	
	vpp_AudioSbc_ENC_IN_T Voc_AudioEncIN;
} MICROPHONE_PLAY;

int32 Audio_MicroPhonePlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_MicroPhoneStop (void);

int32 Audio_MicroPhonePause (void);	

int32 Audio_MicroPhoneResume (HANDLE fhd); 

int32 Audio_MicroPhoneGetID3 (char * pFileName);

int32 Audio_MicroPhoneUserMsg(COS_EVENT *ev);

int32 Audio_MicroPhoneGetPlayInformation (MCI_PlayInf * MCI_PlayInfWAV);

#endif


