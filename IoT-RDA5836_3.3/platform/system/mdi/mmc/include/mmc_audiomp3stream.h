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
// FILENAME: mmc_audiomp3stream.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#ifndef MMC_AUDIO_STREAM_H
#define MMC_AUDIO_STREAM_H
//#include "lily_lcd_api.h"
#include "vpp_audiojpeg_dec.h"
#include "mmc.h"

typedef enum 
{
	MSG_MMC_MP3STREAMDEC_VOC,
	MSG_MMC_MP3STREAMDEC_PCM,
}MP3STREAMDEC_USER_MSG;

typedef struct MP3_DECODER_INPUT 
{
	uint8 *data; 
	uint8 *bof;
	uint32 length;
	uint8 is_first_fill_pcm;
	uint8 is_open_intr;
} MP3_STREAM_INPUT;

typedef struct MP3_DECODER_OUTPUT 
{
	int16 *data;
	uint32 length;
	uint8 channels;
	uint8 rPingpFlag;
	uint8 wPingpFlag;
	uint8 outFlag;
	PCMSETTING pcm;
	int32 OutputPath;
} MP3_STREAM_OUTPUT;



typedef struct MP3_STREAM_DECODER 
{
	MP3_STREAM_INPUT  Mp3StreamInput;
	MP3_STREAM_OUTPUT  Mp3StreamOutput;
	Codecmode mode;
	uint8 loop;
	vpp_AudioJpeg_DEC_OUT_T Voc_Mp3StreamDecStatus;	
	vpp_AudioJpeg_DEC_IN_T Voc_Mp3StreamDecIN;
} MP3_STREAM_PLAY;

int32 AudioMp3StartStream(uint32 *pBuffer, uint32 len, uint8 loop);
int32 AudioMp3StopStream(void);
int32 Audio_Mp3StreamUserMsg(int32 nMsg);

#endif

