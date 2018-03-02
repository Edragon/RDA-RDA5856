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
#include "mmc.h"
#include "vpp_sbc_dec.h"
#include "bt_types.h"

#ifndef MMC_AUDIOSBC_H
#define MMC_AUDIOSBC_H

#define MSG_MMC_SBC_VOC 10
#define MSG_MMC_SBC_PCM 20
#define MSG_MMC_SBC_END 30
#define MSG_MMC_SBC_A2DP    MSG_MMC_AUDIODEC_A2DP

/* SBC configuration bit fields */
#define SBC_SAMPLING_FREQ_16000          0   /* Octet 0 */
#define SBC_SAMPLING_FREQ_32000          1
#define SBC_SAMPLING_FREQ_44100          2
#define SBC_SAMPLING_FREQ_48000          3
#define SBC_CHANNEL_MODE_MONO            0
#define SBC_CHANNEL_MODE_DUAL_CHAN       1
#define SBC_CHANNEL_MODE_STEREO          2
#define SBC_CHANNEL_MODE_JOINT_STEREO    3
#define SBC_BLOCK_LENGTH_4               0   /* Octet 1 */
#define SBC_BLOCK_LENGTH_8               1
#define SBC_BLOCK_LENGTH_12              2
#define SBC_BLOCK_LENGTH_16              3
#define SBC_SUBBANDS_4                   0
#define SBC_SUBBANDS_8                   1
#define SBC_ALLOCATION_SNR               1
#define SBC_ALLOCATION_LOUDNESS          0


typedef struct SBC_INPUT 
{
	HANDLE fileHandle;
	uint8 *data; 
	uint32 length;
	uint8 inFlag;
} SBC_INPUT;

typedef struct SBC_OUTPUT 
{
	int32  buff_offset;
	PCMSETTING pcm;
} SBC_OUTPUT;

typedef struct SBC_PLAY 
{
	SBC_INPUT  SBCInput;
	SBC_OUTPUT  SBCOutput;
	vpp_SBC_DEC_OUT_T Voc_AudioDecStatus;	
	vpp_SBC_DEC_IN_T Voc_AudioDecIN;
} SBC_PLAY;

typedef struct SBC_BT_Packet
{
        UINT8 start_pos;
        UINT8 end_pos;
        t_DataBuf *BtPacketQueue[8];
} SBC_BT_Packet;

typedef VOID (*MMC_AUDIO_CHECK_SBC_CALLBACK_T)(BOOL flag);
int32 Audio_SBCPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_SBCStop (void);

int32 Audio_SBCPause (void);	

int32 Audio_SBCResume (HANDLE fhd); 

int32 Audio_SBCGetID3 (char * pFileName);

int32 Audio_SBCUserMsg(COS_EVENT *ev);

int32 Audio_SBCGetPlayInformation (MCI_PlayInf * MCI_PlayInfWAV);

int32 Audio_SBCGetFreqBandInfo(UINT16 *band);


#endif


