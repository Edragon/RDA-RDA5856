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


#ifndef _MMC_IMAGE_H
#define _MMC_IMAGE_H
#include "cs_types.h"
//#include "lily_lcd_api.h"
#include "fs.h"
// #include "../src/mmc_gif.h"
// #include "../src/mmc_gif_codec.h"

typedef enum
{
  MSG_MMC_GIF_PLAY_CONTINUE,  // =10,
  MSG_MMC_GIF_PLAY_STOP,
  MSG_MMC_UPDATE_LCD,
  MSG_MMC_WEBCAM_MJPEG_ENCODE,
} GIF_PLAY_USER_MSG;

typedef struct
{
  char *file_name;
  FS_FILE_ATTR file_attr;
}IMAGE_FILE_ATTR;

extern uint32 g_TotalMemAllocate;

// extern void mmc_gif_set_progress_callback(mmc_gif_callback_ptr ptr);
extern int32 Lily_BmpFileShow(uint8* filename, RECT* dispRegin);
extern int32 Lily_JpegFileShow(uint8* filename, RECT* dispRegin);
extern int32 Lily_JpegFileDecodeToBuff(uint8 *filename, uint8 *destBuff, uint16 *destWidth, uint16 *destHeight);
extern int32 Lily_AnimationPlayStart(uint8* filename, RECT* dispRegin);
extern int32 Lily_AnimationPlayStop(VOID);
extern int32 Lily_InvalidFileShow(uint8* filename, RECT* dispRegin);
extern VOID Lily_SetDecodeState(CHAR state);
extern CHAR Lily_GetDecodeState(VOID);
extern VOID Lily_SetAnimationResult(int32 value);
extern int32 Lily_GetAnimationResult(VOID);
extern int32 Lily_AddImageFrame(int16 startx,int16 starty,int16 endx, int16 endy);
extern boolean IsMemEnoughForAlloc(uint32 nNeedAllocMemSize);

#endif
 
