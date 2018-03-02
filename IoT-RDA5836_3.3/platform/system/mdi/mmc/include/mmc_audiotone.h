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
// FILENAME: mmc_audiowav.h
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
#include "cos.h"

#ifndef MMC_AUDIOTONE_H
#define MMC_AUDIOTONE_H

#define MSG_MMC_TONE_PCM_INT 1
#define MSG_MMC_TONE_PCM_END 2


typedef struct TONE_INPUT 
{
	uint8 *buff;
	uint8 *data; 
    uint8 loop;
    uint8 times;
	uint32 index;
	uint32 length;
} TONE_INPUT;

typedef struct TONE_OUTPUT 
{
	UINT32 start_time;
	int32 OutputPath;
} TONE_OUTPUT;

typedef struct TONE_PLAY 
{
	TONE_INPUT  ToneInput;
	TONE_OUTPUT  ToneOutput;
} TONE_PLAY;

int32 Audio_TonePlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_ToneStop (void);

int32 Audio_TonePause (void);	

int32 Audio_ToneResume (HANDLE fhd); 

int32 Audio_ToneGetID3 (char * pFileName);

int32 Audio_ToneUserMsg(COS_EVENT *ev);

int32 Audio_ToneGetPlayInformation (MCI_PlayInf * MCI_PlayInfWAV);

PUBLIC MCI_ERR_T MMC_ToneGetFileInformation ( CONST INT32 FileHander,  AudDesInfoStruct  * CONST DecInfo  );

#endif


