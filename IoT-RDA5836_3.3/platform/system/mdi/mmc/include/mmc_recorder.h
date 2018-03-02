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


#ifndef __RECORDMP3_H__
#define __RECORDMP3_H__

#include "cs_types.h"
#include "vpp_audiojpegenc.h"

#define FRAME_NUM 11
#define MP3ENC_INPUT_PCMSIZE        2*288*FRAME_NUM//6336 byte
#define MP3ENC_INPUT_BUFSIZE		2*1152*FRAME_NUM //25344 byte
#define MP3ENC_INPUT_FRAMESIZE		2*1152

#define MP3ENC_INPUT_ONEFRAME_SIZE  2*1152//2304 BYTE
#define MP3ENC_SAMPLE_RATE_N 32000
#define ENCODER_OUTPUT_FRAME_BUF_MAX_SIZE 1000//byte

//define local msg
#define MSG_MMC_AUDIOENC_VOC_INT 1

extern int32 RecordStart(int32 volume, int32 play_style, char * filename);

extern int32 RecordStop (void) ;

extern int32 RecordPause (void)	;

extern int32 RecordResume (void) ;

extern int32 RecordSample (char *pSample, int32 nSample);

extern int32 RecordUserMsg(int32 nMsg);

#endif//__RECORDMP3_H__


