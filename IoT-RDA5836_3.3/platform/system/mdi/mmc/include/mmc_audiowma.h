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
// FILENAME: mmc_wmadec.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   guosh
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _mmc_audiowma_h_
#define _mmc_audiowma_h_
#include "cs_types.h"
#include "vpp_wma_dec.h"
#include "mmc.h"

//#define WRITE_PCM_FILE
#if 0
#define WMADEC_INPUT_BUFSIZE		60*1024
#ifdef PCM_OUT_24BIT_PER_SAMPLE
#define WMADEC_OUTPUT_BUFSIZE	8/*frame*/*2/*channel*/*4/*byte*/*2/*half buffer*/*2048/*max frame pcm*/
#else
#define WMADEC_OUTPUT_BUFSIZE	8/*frame*/*2/*channel*/*2/*byte*/*2/*half buffer*/*2048/*max frame pcm*/
#endif
#define WMADEC_MAXFRAME_SIZE		2048*2*2
#endif
#define WMADEC_INPUT_BUFSIZE	        2*1024
#define WMADEC_OUTPUT_BUFSIZE	2/*channel*/*2*/*byte*/2048/*max frame pcm*/
#define WMADEC_MAXFRAME_SIZE		2048*2*2

typedef enum
{
	MMC_WMADEC_VOC_IRQ = 0, 
	MMC_WMADEC_PCM_IRQ	
	
} MMC_WMADEC_IRQ_TYPE;

typedef enum
{
	MSG_MMC_WMADEC_VOC_IRQ,
	MSG_MMC_WMADEC_PCM_IRQ,
	
} WMADEC_USER_MSG;



typedef enum 
{
	WMADEC_PLAY_REQ = 0,
	WMADEC_STOP_REQ,
	WMADEC_PAUSE_REQ,
	WMADEC_RESUME_REQ,
	WMADEC_SEEK_REQ,
	WMADEC_CONTINUE_REQ,

	WMADEC_PLAY_IND,
	WMADEC_STOP_IND,
	WMADEC_PAUSE_IND,
	WMADEC_FINISH_IND,
	WMADEC_ERR_IND,

} WMADEC_MSG;

typedef enum 
{
	WMADEC_STOP = 0,
	WMADEC_PLAY,
	WMADEC_PAUSE,
	WMADEC_FINISH

} WMADEC_STATE;

typedef struct WMA_DECODER_INPUT 
{
	HANDLE fileHandle;
	uint32 fileOffset;
	uint8 *data; 
	uint8 *bof;
	uint8 *eof;
	uint32 length;
	uint32 readOffset;
	uint8 inFlag;
	uint32 maxConsumedLen;
	
} WMA_INPUT;

typedef struct WMA_DECODER_OUTPUT 
{
	int32 *data[2];
	int32 *bof;
	int32 *eof;
	uint32 wOffset;
	uint32 rOffset;
	uint32 length;
	uint32 resdLen;
	uint8 channels;
	uint8 rPingpFlag;
	uint8 wPingpFlag;
	uint8 outFlag;
	//DI_pcm_para_t pcm;
	PCMSETTING pcm;
	int16 *pbuf;
	int16 *pTransBuf;
	int32 OutputPath;
} WMA_OUTPUT;

typedef struct WMA_DECODER 
{
	WMA_INPUT  WmaInput;
	WMA_OUTPUT  WmaOutput;
	uint32 EQMode;
	int16 *pHistoryData;
	void *handle;
	MMC_WMADEC_IRQ_TYPE IrqType;
	WMADEC_STATE UserState;
	VPP_WMA_DEC_OUT_T Voc_WmaDecStatus;
	VPP_WMA_DEC_CONTENT_T wmaHdrInfo;
	uint32 *AudPausePos;

} WMA_PLAY;


int32 Audio_WmaPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);
int32 Audio_WmaStop (void) 	;
int32 Audio_WmaPause (void)	;
int32 Audio_WmaResume(HANDLE fhd);
int32 Audio_WmaGetID3 (char * filename)  ;
int32 Audio_WmaGetPlayInformation (MCI_PlayInf * MCI_PlayInfWMA);
int32 Audio_WmaGetFreqBandInfo(UINT16 *band);
int32 Audio_WmaUserMsg(COS_EVENT *ev);
int32 Audio_WMAGetFileInformation(HANDLE fhd, AudDesInfoStruct  * CONST DecInfo);

#endif
