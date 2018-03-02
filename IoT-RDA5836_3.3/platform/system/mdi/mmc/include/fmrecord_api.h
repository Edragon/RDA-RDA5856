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
//
// FILENAME: fmrecord_api.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"



uint32 LILY_FmrRecordStart (int8 *file_name_p) ;	//	MCI_MEDIA_PLAY_REQ,			

uint32 LILY_FmrRecordPause(void);

uint32 LILY_FmrRecordResume(void);

uint32 LILY_FmrRecordStop(void);
int32 FmrRecordStart(int32 volume, int32 play_style, char * filename);

