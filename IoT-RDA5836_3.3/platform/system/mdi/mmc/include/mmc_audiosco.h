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
// FILENAME: mmc_audioSCO.h
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

#ifndef MMC_AUDIOSCO_H
#define MMC_AUDIOSCO_H

#define MSG_MMC_SCO_PCM_INT 10
#define MSG_MMC_SCO_PCM_END 20
#define MSG_MMC_SCO_MIC_INT 30
#define MSG_MMC_SCO_DATA_IND MSG_MMC_AUDIODEC_SCO

typedef struct SCO_INPUT  // struct for record data to send by sco
{
    uint16 sco_handle;
    uint16 *data;          // pcm record buff
	uint16 length;       // valid data in pcm buff, in pcm sample
    uint16 size;          // size of data, in pcm sample
} SCO_INPUT;

typedef struct SCO_OUTPUT  // struct for play received sco data
{
	uint16 *Buffer;		//Pointer for PCM buffer
	uint16 length;       // valid data in pcm buff, in pcm sample
	uint16 Size;		//Size of PCM buffer, in pcm sample
} SCO_OUTPUT;

typedef struct SCO_PLAY 
{
	uint8 in_flag;
    uint8 packet_size;
	HANDLE fileHandle;
	SCO_INPUT  SCOInput;
	SCO_OUTPUT  SCOOutput;
} SCO_PLAY;

int32 Aduio_SCORecord(HANDLE fhd);

int32 Audio_SCOPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_SCOStop (void);

int32 Audio_SCOPause (void);	

int32 Audio_SCOResume (HANDLE fhd); 

int32 Audio_SCOGetID3 (char * pFileName);

int32 Audio_SCOUserMsg(COS_EVENT *ev);

int32 Audio_SCOGetPlayInformation (MCI_PlayInf * MCI_PlayInfSCO);


#endif


