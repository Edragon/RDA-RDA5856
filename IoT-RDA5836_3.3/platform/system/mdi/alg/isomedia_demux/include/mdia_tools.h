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


#ifndef SHEEN_VC_DEBUG
#include "global.h"
#endif

#ifdef MP4_3GP_SUPPORT

#ifndef _GF_MDIA_TOOLS_H_
#define _GF_MDIA_TOOLS_H_

#include "setup.h"
/*track dumper*/

enum
{
	/*track dumper types are formatted as flags for conveniency for 
	authoring tools, but never used as a OR'ed set*/
	/*native format (JPG, PNG, MP3, raw aac or raw cmp) if supported*/
	GF_EXPORT_NATIVE = 1,
	/*raw samples (all except OD)*/
	GF_EXPORT_RAW_SAMPLES = (1<<1),
	/*NHNT format (any MPEG-4 media)*/
	GF_EXPORT_NHNT = (1<<2),
	/*AVI (MPEG4 video and AVC tracks only)*/
	GF_EXPORT_AVI = (1<<3),
	/*MP4 (all except OD)*/
	GF_EXPORT_MP4 = (1<<4),
	/*AVI->RAW to dump video (trackID=1) or audio (trackID>=2)*/
	GF_EXPORT_AVI_NATIVE = (1<<5),

	/*following ones are real flags*/
	/*used bfor MP4 extraction, indicates track should be added to dest file if any*/
	GF_EXPORT_MERGE_TRACKS = (1<<10),
	/*indicates QCP file format possible as well as native (EVRC and SMV audio only)*/
	GF_EXPORT_USE_QCP = (1<<11),
	/*ony probes extraction format*/
	GF_EXPORT_PROBE_ONLY = (1<<30),
	/*when set by user during export, will abort*/
	GF_EXPORT_DO_ABORT = (1<<31),
};



#endif
#endif
