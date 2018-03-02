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


// File name      : audio_api                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-15 11:06:46                                                  
// Last modified  : 2016-08-15 11:06:46                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-15 11:06:46 --  Create by liqiu                                               
//------------------------------------------------------------------------

#ifndef AUDIO_API_H
#define AUDIO_API_H

#include "cs_types.h"
#include "mmc_audiompeg.h"
#include "mmc_audiowma.h"
#include "mmc_audiosbc.h"
#include "mci.h"


uint32 LILY_AudioPlay (int32 OutputPath, HANDLE fileHandle, int16 fielType,INT32 PlayProgress); 	//	MCI_MEDIA_PLAY_REQ,			

uint32 LILY_AudioPause(void);

uint32 LILY_AudioResume(HANDLE fileHandle);

uint32 LILY_AudioSeek(int32 time);

uint32 LILY_AudioStop(void);

//uint32 LILY_AudioGetID3(int8 *pfilename,ID3INFO *id3info) ;	//	MCI_MEDIA_PLAY_REQ,			

uint32 LILY_AudioSetEQ(AUDIO_EQ 	EQMode);

uint32 LILY_AudioGetPlayInformation(MCI_PlayInf* PlayInformation);

uint32 LILY_AudioPlayLeftData(void);


#endif

