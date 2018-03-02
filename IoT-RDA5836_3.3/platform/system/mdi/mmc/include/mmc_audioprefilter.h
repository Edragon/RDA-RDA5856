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
#ifndef MMC_AUDIOPREFILTER_H
#define MMC_AUDIOPREFILTER_H

#include "cswtype.h"
#include "string.h"
#include "sxr_mutx.h"
#include "sxr_mutx.h"
#include "hal_host.h"

#define MPEG2TSPACKETSIZE (188)


INT32 MMC_Mpeg2tsDemux(UINT8 * buf, UINT8 * end, UINT8 * outbuf,UINT32 *outlen,INT32 flags,INT32 demux_pid);
INT32 mpeg_stream_check(INT32 *filetype,UINT8 *buffer,UINT32 checklength);


#endif
