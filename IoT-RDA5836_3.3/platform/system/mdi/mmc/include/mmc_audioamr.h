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


#ifndef MMC_AUDIOAMR_H
#define MMC_AUDIOAMR_H


#include "mmc.h"


typedef enum 
{
	MSG_MMC_AMRDEC_VOC,
	MSG_MMC_AMRDEC_PCM,
}AMRDEC_USER_MSG;

typedef struct AMR_DECODER_INPUT 
{
	int32 fileHandle;
	uint32 fileOffset;
	uint8 *data; 
	uint8 *bof;
	uint8 *eof;
	uint32 length;
	uint32 readOffset;
	uint8 inFlag;
	uint8 is_first_frame;
} AMR_INPUT;

typedef struct AMR_DECODER_OUTPUT 
{
	int16 *data;
	uint32 length;
	uint8 channels;
	uint8 rPingpFlag;
	uint8 wPingpFlag;
	uint8 outFlag;
	PCMSETTING pcm;
	int32 OutputPath;
} AMR_OUTPUT;

typedef struct AMR_DECODER 
{
	AMR_INPUT  AmrInput;
	AMR_OUTPUT  AmrOutput;
	Codecmode mode;
	
} AMR_PLAY;






int32 Audio_AmrPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_AmrStop (void);

int32 Audio_AmrPause (void);	

int32 Audio_AmrResume (HANDLE fhd); 

int32 Audio_AmrGetID3 (char * pFileName);

int32 Audio_AmrUserMsg(int32 nMsg);

int32 Audio_AmrGetPlayInformation (MCI_PlayInf * MCI_PlayInfAMR);

#endif

