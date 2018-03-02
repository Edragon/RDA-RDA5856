/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
// FILENAME: vidrec_api.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   baoyh           20071130        create
//
////////////////////////////////////////////////////////////////////////////////
#ifndef VIDREC_API_H
#define VIDREC_API_H

#include "cpu_share.h"


int32 LILY_VidrecPreviewStartReq(MMC_VDOREC_SETTING_STRUCT *previewParam , void(*vid_rec_finish_ind)(uint16 msg_result) );


int32 LILY_VidrecAdjustSettingReq (int32 adjustItem, int32 value);

int32 LILY_VidrecPreviewStopReq(void);


int32 LILY_VidrecRecordStartReq (HANDLE filehandle) ;	//	MCI_MEDIA_PLAY_REQ,			


int32 LILY_VidrecRecordPauseReq(void);


int32 LILY_VidrecRecordResumeReq(void);

int32 LILY_VidrecRecordStopReq(void);

int32 LILY_VidrecAudioSampleReadyReq(uint32 nEvent);

int32 LILY_VidrecApplyAudioSampleStop(void);

#endif


