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

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define LILY_DEBUG
#include "cs_types.h"
//Unicode usage
//#define USE_UNICODE_CP1252
//#define USE_UNICODE_CP932
//#define USE_UNICODE_CP950
//#define USE_UNICODE_CP949
#define USE_UNICODE_CP936
/*
// dingjian 20071217 for lcd szie is 176*220
// TYPE1 128*160
// TYPE2 176*220
// TYPE3 240*320
#define TYPE_128_160    1
#define TYPE_176_220    2
#define TYPE_240_320    3

#ifdef LCDSIZE_128_160
#define LCD_SIZE    TYPE_128_160
#endif

#ifdef LCDSIZE_176_220
#define LCD_SIZE    TYPE_176_220
#endif

#ifdef LCDSIZE_240_320
#define LCD_SIZE    TYPE_240_320
#endif

#if (TYPE_128_160 ==  LCD_SIZE)
#define LCD_WIDTH	128
#define LCD_HEIGHT	160
#elif (TYPE_176_220 ==  LCD_SIZE)
#define LCD_WIDTH	176
#define LCD_HEIGHT	220
#elif (TYPE_240_320 ==  LCD_SIZE)
#define LCD_WIDTH	240
#define LCD_HEIGHT	320
#else
#error "no lcd config info"
#endif
*/
//ENABLED OR DISABLED FUNCTION
//#define ID3_ENABLED
#define LILY_USE_LOCAL_UNICODETAB
extern INT32 MMC_LcdWidth;
extern INT32 MMC_LcdHeight;
#ifdef DUAL_LCD
extern INT32 MMC_SubLcdWidth;
extern INT32 MMC_SubLcdHeight;
#endif


// dingjian add 20080218
//#if (TYPE_240_320 ==  LCD_SIZE)
//#define CONST_MMC_MEM_SIZE 960 * 1024
//#else
#define CONST_MMC_MEM_SIZE 960 * 1024
//#endif

#define DEVICE_SUPPORT_USB
//#define DEVICE_SUPPORT_NANDFLASH
#define MP4_3GP_SUPPORT
#endif//__GLOBAL_H__


