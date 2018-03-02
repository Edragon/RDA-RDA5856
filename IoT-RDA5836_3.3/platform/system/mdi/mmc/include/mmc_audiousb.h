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
// FILENAME: mmc_audioUSB.h
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
#include "vpp_audiojpeg_dec.h"
#include "mmc_adpcm.h"
#include "cos.h"

#ifndef MMC_AUDIOUSB_H
#define MMC_AUDIOUSB_H

#define MSG_MMC_USB_PCM_INT 10
#define MSG_MMC_USB_PCM_END 20
#define MSG_MMC_USB_MIC_INT 30
#define MSG_MMC_USB_SBC_VOC 40
#define MSG_MMC_USB_DATA_IND MSG_MMC_AUDIODEC_USB

typedef struct USB_INPUT  // struct for record data to send by sco
{
    uint8 is_first_frame;
    uint16 sco_handle;
    uint16 *data;          // pcm record buff
	uint16 length;       // valid data in pcm buff, in pcm sample
    uint16 size;          // size of data, in pcm sample
} USB_INPUT;

typedef struct USB_OUTPUT  // struct for play received sco data
{
	uint16 *Buffer;		//Pointer for PCM buffer
	uint16 length;       // valid data in pcm buff, in pcm sample
	uint16 Size;		//Size of PCM buffer, in pcm sample
} USB_OUTPUT;

typedef struct USB_PLAY 
{
	uint8 in_flag;
    uint8 packet_size;
	USB_INPUT  USBInput;
	USB_OUTPUT  USBOutput;
	vpp_AudioJpeg_DEC_OUT_T Voc_AudioDecStatus;	
	vpp_AudioJpeg_DEC_IN_T Voc_AudioDecIN;
} USB_PLAY;

int32 Audio_USBPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress);

int32 Audio_USBStop (void);

int32 Audio_USBPause (void);	

int32 Audio_USBResume (HANDLE fhd); 

int32 Audio_USBGetID3 (char * pFileName);

int32 Audio_USBUserMsg(COS_EVENT *ev);

int32 Audio_USBGetPlayInformation (MCI_PlayInf * MCI_PlayInfUSB);


#endif


