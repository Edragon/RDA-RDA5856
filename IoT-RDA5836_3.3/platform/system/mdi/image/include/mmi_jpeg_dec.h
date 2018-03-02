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


#ifndef _MMC_JPEG_DECODER_H
#define _MMC_JPEG_DECODER_H
//#define SUPPORT_NONSTANDARD_HUFFMANTABLE
//#include "stdio.h"
//#include "ap_filemanager.h"
////////////jpeg header marker
//#include "cswtype.h"
#include "setjmp.h"

typedef enum
{
	    FUNC_OK=0,
        FUNC_FORMAT_ERROR ,
        VLC_ERROR ,
        WRONG_DECOMPOSE , 
		ERROR_OPENFIAL,
		ERROR_WRONG_SIZE,
		ERROR_BIG_SIZE,
		ERROR_WRONG_CASE,
		ERROR_WRONG_HEAD
}ERROR_TYPE;

typedef struct 
{
	short  width;
	short  height;
	int    len;
	ERROR_TYPE state;
}DEC_UNIT;

extern jmp_buf Jpeg_jmpbuf;
extern BOOL Jpeg_jmpbuf_is_used;

#define JPEG_RAISE(ret_code)   longjmp (Jpeg_jmpbuf, ret_code)
#define JPEG_TRY                                \
   {                                           \
      int jmpret;                              \
      MMI_ASSERT(!Jpeg_jmpbuf_is_used);         \
      Jpeg_jmpbuf_is_used = TRUE;               \
      jmpret = setjmp (Jpeg_jmpbuf);            \
      if(jmpret == 0 )                         \
      {

#define JPEG_CATCH_BEGIN                     \
      }                                     \
      else                                  \
      {                                     \
         {

#define JPEG_CATCH_END            \
         }                       \
      }                          \
      Jpeg_jmpbuf_is_used = FALSE;\
   }

ERROR_TYPE MMF_JpegDecodePicture(long handle ,char* outbuf,unsigned int outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit,short kind,void (*callBackFun)(void),
								 UINT16 img_id);

#endif
