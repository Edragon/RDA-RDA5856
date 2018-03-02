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


#ifndef _MMC_JPEG_ENC_H_
#define _MMC_JPEG_ENC_H_
typedef enum
{
     NO_ENC_ERRO,
     ENC_FAILTOCREAT,
     ENC_FAILTOCLOSE,
     ENC_WRONGSIZE
}JPEGENC_ERROR;

extern  JPEGENC_ERROR  MMC_JpegEncode(char* filename,short quality,short ImgWidth,short ImgHeight,short* InBuff,unsigned long* outlen);
#endif
