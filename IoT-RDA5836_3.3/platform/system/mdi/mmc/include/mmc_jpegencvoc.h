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


#ifndef  MMC_JPEGENCVOC
#define  MMC_JPEGENCVOC

void JPEGENCIsr(void);
INT32 initVocJpegEncode(void);
void quitVocJpegEncode(void);
uint32 getJpegEncodeLen(void);
void  MMC_jpgEn (UINT16 srcw,  // width of source
                UINT16 srch,        // height of source
                UINT16 imgw,      // width of the jpeg image
                UINT16 imgh,       // height of the jpeg image 
                UINT16 quality,    // quality of the jpeg image
                char *bufin,         // buffer of the source
                char *bufout,       // buffer of the jpeg image
                UINT16 yuvmode  // 1 - YUV, 0 - RGB565
        );

#endif

