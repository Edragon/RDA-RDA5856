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
#ifndef _AP_CODEC_H_
#define _AP_CODEC_H_

#define AP_CODEC_USE_BT_SRAM

#ifdef AP_CODEC_USE_BT_SRAM
#define AP_CODEC_BUFFER_SIZE_MAX    (1024*8)
#define AP_CODEC_BT_SRAM_BASE       (0xa1c80000)
#else
#define AP_CODEC_BUFFER_SIZE_MAX    (1024*6)
#endif

extern INT32 UartPlayEntry(INT32 param);
extern INT32 UartRecEntry(INT32 param);

#endif     /*_AP_CODEC_H_*/

