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
#ifndef WMATYPES_H
#define WMATYPES_H


/****************************************************************************/

#define UNALIGNED
#include "cswtype.h"
#include "cs_types.h"
/****************************************************************************/

//#ifndef _BOOL_DEFINED
//#define _BOOL_DEFINED
//typedef long boolean;
//#endif /* _BOOL_DEFINED */

//#ifndef _BYTE_DEFINED
//#define _BYTE_DEFINED
//typedef unsigned char   BYTE;
//#endif /* _BYTE_DEFINED */



//#ifndef _WORD_DEFINED
//#define _WORD_DEFINED
//typedef unsigned short  WORD;
//#endif /* _WORD_DEFINED */

//#ifndef _DWORD_DEFINED
//#define _DWORD_DEFINED
//typedef unsigned long    DWORD;	/* actually long, but this should be okay */
//#endif /* _DWORD_DEFINED */

#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
typedef  struct tQWORD
{
    DWORD   dwLo;
    DWORD   dwHi;

}  QWORD;

#endif /* _QWORD_DEFINED */


#pragma pack(1)

/****************************************************************************/
#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct  
{
    DWORD   Data1;
    WORD    Data2;
    WORD    Data3;
    BYTE    Data4[ 8 ];

}tGUID, GUID;

#endif


/****************************************************************************/
#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef  struct tWAVEFORMATEX
{
    WORD    wFormatTag;         /* format type */
    WORD    nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD   nSamplesPerSec;     /* sample rate */
    DWORD   nAvgBytesPerSec;    /* for buffer estimation */
    WORD    nBlockAlign;        /* block size of data */
    WORD    wBitsPerSample;     /* number of bits per sample of mono data */
    WORD    cbSize;             /* the count in bytes of the size of */

}  WAVEFORMATEX;
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;

#endif

#define WAVE_FORMAT_MSAUDIO1  0x0160
typedef  struct 
{
    WAVEFORMATEX wfx;
    WORD         wSamplesPerBlock;
    WORD         wEncodeOptions;

}tMSAUDIO1WAVEFORMAT , MSAUDIO1WAVEFORMAT;

#define WAVE_FORMAT_WMAUDIO2  0x0161
typedef  struct 
{
    WAVEFORMATEX wfx;
    DWORD        dwSamplesPerBlock;
    WORD         wEncodeOptions;
    DWORD        dwSuperBlockAlign;

} tWMAUDIO2WAVEFORMAT , WMAUDIO2WAVEFORMAT;


/****************************************************************************/
typedef  struct 
{
    BYTE    span;
    WORD    virtualPacketLen;
    WORD    virtualChunkLen;
    WORD    silenceLen;
    WORD    silence[1];

} AsfXAcmAudioErrorMaskingData;


/****************************************************************************/
typedef  struct 
{
    DWORD maxObjectSize;
    WORD  chunkLen;
    WORD  signatureLen;
    BYTE  signature[1];////should not be modified!!!!

}tAsfXSignatureAudioErrorMaskingData ,  AsfXSignatureAudioErrorMaskingData;

#pragma pack()

#endif  /* WMATYPES_H */

