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
#ifndef VPPP_AUDIOJPEGENC_ASM_H
#define VPPP_AUDIOJPEGENC_ASM_H


#include "cs_types.h"

#ifndef CS_PLATFORM_SIMU

 #define RAM_X_BEGIN_ADDR 0x0
 #define RAM_Y_BEGIN_ADDR 0x4000

#endif

#define VPP_AUDIOJPEGENC_MAIN_ENTRY           0
#define VPP_AUDIOJPEGENC_CRITICAL_SECTION_MIN 0
#define VPP_AUDIOJPEGENC_CRITICAL_SECTION_MAX 0


//common
EXPORT const INT32 G_VppCommonEncCode[];

//mp3
EXPORT const INT32 G_VppMp3EncCode[];
EXPORT const INT32 G_VppMp3EncConstX[];
EXPORT const INT32 G_VppMp3EncConstY[];
EXPORT const INT32 G_VppMp3EncConst_rqmy[];
EXPORT const INT32 G_VppMp3EncConst_Zig[];

//amrjpeg
EXPORT const INT32 G_VppAmrJpegEncCode[];
EXPORT const INT32 G_VppAmrJpegEncConstX[];
EXPORT const INT32 G_VppAmrJpegEncConstY[];

EXPORT CONST INT32 G_VppAudJpegMainCodeSize;

#endif  // VPPP_AUDIOJPEGENC_ASM_H
