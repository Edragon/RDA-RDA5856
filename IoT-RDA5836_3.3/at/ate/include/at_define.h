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

/*********************************************************
 *
 * File Name
 *	at_define.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/
// TODO: delete it or improvement!
#ifndef _AT_DEFINE_H
#define _AT_DEFINE_H

#include "cos.h"

// #define malloc               CSW_Malloc
// #define free               CSW_Free
// [[hameina[mod] 2008-6-30
#define AT_MALLOC                          COS_MALLOC
#define AT_FREE                            COS_FREE
// ]]hameina[mod] 2008-6-30

#ifdef MMI_WITH_AT
#define AT_NO_GPRS
#endif

#ifdef MCU_WIFI_PLAT
#    define AT_MemSet                          memset
#    define AT_MemCpy                          memcpy
#    define AT_MemCmp                          memcmp
#    define AT_MemZero(str,len)                memset((char*)(str),0,(len));
#    define SUL_StrNCaselessCompare            strnicmp
#else
#    define AT_MemSet                          SUL_MemSet8
#    define AT_MemCpy                          SUL_MemCopy8
#    define AT_MemCmp                          SUL_MemCompare
#    define AT_MemZero(str,len)                SUL_MemSet8((str), 0, (len))
#endif

#define AT_StrLen(str)                     strlen((char*)str)
#define AT_StrCpy(dst,src)                 strcpy((char*)dst,(char*)src)
#define AT_StrNCpy(dst,src,size)           do { \
		strncpy((char*)dst,(char*)src,size); \
		dst[size] = '\0'; \
	} while(0)
#define AT_StrCmp(str1,str2)               strcmp((char*)str1,(char*)str2)
#define AT_StrNCmp(str1,str2,size)         strncmp((char*)str1,(char*)str2, size)
//#define strnicmp SUL_StrNCaselessCompare
#define AT_StrNCaselessCmp(str1,str2,size) strnicmp((char*)str1,(char*)str2, size)
// CAOXH [+]2008-5-20
#define AT_StrCaselessCmp(str1,str2)  strnicmp((char*)str1,(char*)str2, strlen(str1) > strlen(str2) ? strlen(str1) : strlen(str2))
#define AT_StrChr(str,ch)                  strchr((char*)str,(int)ch)
#define AT_StrRChr(str,ch)                 strrchr((char*)str,(int)ch)
#define AT_StrCat(dst, src)                strcat((char*)dst,(char*)src)
#define AT_StrAToI                         atoi

extern char* strupr(char* str);

#define AT_StrUpr(str)                     strupr((char*)str)

#ifdef MCU_WIFI_PLAT
#define AT_Sprintf                         sprintf
#else
#define AT_Sprintf                         SUL_StrPrint
#endif

#endif
 
