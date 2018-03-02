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

#ifndef __AP_AUDIO_H__
#define __AP_AUDIO_H__

#include "ap_common.h"
#include "ap_media.h"

//Surporrt Decode Types
#define   DAF_DECODE
#define   WAV_CODEC
//#define  AU_DECODE
//#define  AIFF_DECODE
//
//#define  AMR_DECODE
//#define  AMRWB_DECODE
//#define  AAC_DECODE
//#define  WMA_DECODE
//#define  VM_CODEC


#define ID3_TAG_POOL_SIZE                      570 // number of bytes
#define ID3_TAG_GENRE_NUM                   148
#define ID3V2_HEADER_LEN                       10
#define PARSE_ID3_FRAME_NO_SPACE    -1
#define PARSE_ID3_FRAME_SUCCESS      0
#define PARSE_ID3_FRAME_ABORT           1

#define MAX_ID3_TITLE_LEN                 80
#define MAX_ID3_ARTIST_LEN               80
#define MAX_ID3_ALBUM_LEN               80
#define MAX_ID3_GENRE_LEN               30
#define MAX_ID3_YEAR_LEN                  5
#define MAX_ID3_COPYRIGHT_LEN      40
#define MAX_ID3_TRACKNUM_LEN       10
#define MAX_ID3_DURATION_LEN         10 /* max duration for UINT32 milliseconds type is "1194:59:59" */
#define MAX_DESCRIPTION                    80

#if 0
typedef struct tWMADescription
{
   UINT32  time; // duration in milli-second
   UINT32  bitRate;
   UINT32  sampleRate;
   UINT32  trackNum;
   UINT32  size; // file size
   BOOL    stereo;
   UINT8   textencode;
   INT8    title[MAX_ID3_TITLE_LEN+1];       /*TIT2, TT2*/
   INT8    artist[MAX_ID3_ARTIST_LEN+1];  /*TPE1, TP1*/
   INT8    album[MAX_ID3_ALBUM_LEN+1];    /*TALB,TAL*/
   INT8    author[MAX_ID3_ARTIST_LEN+1]; /*au,TAL*/
   INT8    genre[MAX_ID3_GENRE_LEN+1];
   INT8    copyright[MAX_ID3_COPYRIGHT_LEN+1];
   INT8    date[MAX_ID3_YEAR_LEN+1];  //year
   union
   {
      struct
      {
         UINT8 CRC;
         UINT8 channel_mode;
         UINT8 bit_rate_index;
      } DAF;
   } details;
} audInfoStruct;
#endif

typedef struct
{
	UINT8* buf; // one UINT8 reserved for tail reference to a valid address when it's full
	UINT8* ptr;
	UINT8* tail;
	UINT32 buf_size;
	UINT32 skip;
	UINT32 total_load;
	INT32  file;
} audio_input_stream_struct;

typedef enum
{
    ID3_FRAME_TITLE,
    ID3_FRAME_ARTIST,
    ID3_FRAME_ALBUM,
    ID3_FRAME_GENRE,
    ID3_FRAME_YEAR,
    ID3_FRAME_COPYRIGHT,
    NUM_OF_ID3_FRAME
}ID3_TAG_FRAME_ENUM;

typedef enum 
{
	ID3_FRAME_TEXT_ENCODING_ISO_8859_1,
	ID3_FRAME_TEXT_ENCODING_UTF16,
	ID3_FRAME_TEXT_ENCODING_UTF16_BE,
	ID3_FRAME_TEXT_ENCODING_UTF8
}ID3_TAG_FRAME_TEXT_ENCODING_ENUM;


typedef enum
{
	DAF_VERSION_1,
	DAF_VERSION_2,
	DAF_VERSION_NONE,
	DAF_VERSION_2_5
}DAF_VERSION_ID_ENUM;

typedef struct
{
	UINT8  id[4];
	UINT32 size; // include header & data
	UINT32 data_offset;
	UINT32 data_size; // data size after unsync
	BOOL   unsync;
}id3_tag_frame_header_struct;

typedef enum
{
	ID3_UTF16_BE,
	ID3_UTF16_LE,
	ID3_UTF16_UNKNOWN_BYTE_ORDER
}ID3_TAG_UTF16_BYTE_ORDER_ENUM;

/****************************************
* 此处定义ID3V1格式结构体
****************************************/
typedef struct
{
    INT8 Flag[3];
    INT8 TIT2_ID3V1[30];
    INT8 TPE1_ID3V1[30];
    INT8 TALB_ID3V1[30];
    INT8 TYER_ID3V1[4];
    INT8 COMM_ID3V1[29];
    UINT8 TRCK_ID3V1[1];
    UINT8 CON_ID3V1[1];
}ID3V1_t;


typedef struct 
{
	UINT32 v2_size;                // not include header & footer size (original size)
	UINT32 v2_reverse_unsync_size; // tag size after reverse unsync process, it's valid when v2_unsync is set
	UINT8  v2_major_ver;
	BOOL   v2_unsync;
	BOOL   v2_footer;
	BOOL   v2_exthdr;
	BOOL   v2_crc;         // it's valid when v2_exthdr flag is set
	UINT32 v2_exthdr_size; // it's valid when v2_exthdr flag is set
	audInfoStruct * audInfo_ptr;
}id3_tag_struct;

typedef struct
{
	UINT8 offset;
	UINT8 size;
}id3_tag_v1_field_struct;


/* ------------------------------------------------------------------------------ */
/*  Media File Playback/Recording Interface                                       */
/* ------------------------------------------------------------------------------ */
typedef enum {
   MEDIA_SUCCESS = 0,
   MEDIA_FAIL,
   MEDIA_BAD_FORMAT,             /* media format error                        */
   MEDIA_UNSUPPORTED_CHANNEL,    /* unsupported audio channel count           */
   MEDIA_UNSUPPORTED_FREQ,       /* unsupported audio freqency                */
   MEDIA_UNSUPPORTED_TYPE,       /* unsupported audio content                 */
   MEDIA_READ_FAIL              /* read fail indicator */
} Media_Status;


// Note : New defined media format should always be added at the last one,    
// otherwise it may induce unsync media format problem between MMI/MED and L1Audio.
typedef enum {
   MEDIA_FORMAT_AMR,
   MEDIA_FORMAT_AMR_WB,
   MEDIA_FORMAT_PCM_8K,
   MEDIA_FORMAT_PCM_16K,
   MEDIA_FORMAT_G711_ALAW,
   MEDIA_FORMAT_G711_ULAW,
   MEDIA_FORMAT_DVI_ADPCM,
   MEDIA_FORMAT_VRD,
   MEDIA_FORMAT_WAV,
   MEDIA_FORMAT_WAV_ALAW,
   MEDIA_FORMAT_WAV_ULAW,
   MEDIA_FORMAT_WAV_DVI_ADPCM,
   MEDIA_FORMAT_MMF,
   MEDIA_FORMAT_AU,
   MEDIA_FORMAT_AIFF,
   MEDIA_FORMAT_VRSI,
   MEDIA_FORMAT_WMA,
   MEDIA_FORMAT_M4A,
   MEDIA_FORMAT_WAV_DVI_ADPCM_16K,
   MEDIA_FORMAT_VOIPEVL,
   MEDIA_FORMAT_AAC_PLUS,
   MEDIA_FORMAT_AAC_PLUS_V2,
   MEDIA_FORMAT_MUSICAM = 32,
   MEDIA_FORMAT_AWB_PLUS,
   MEDIA_FORMAT_AWB_PLUS_EXTEND,
   MEDIA_FORMAT_UNKNOWN
} Media_Format;


// Audio Streaming Sender
typedef  struct mhdl MHdl;

struct mhdl {
//UINT32     data;
   Media_Format   mediaType;
   // Media ring buffer control variables
   UINT8          *rb_base;         // Pointer to the ring buffer
   INT32          rb_size;          // Size of the ring buffer
   INT32          write;
   INT32          read;
   BOOL         mono2stereo;
};

typedef struct 
{
   INT32    hFile;
   UINT32   uFileSize;
} STFSAL;


#endif // __AP_AUDIO_H__




