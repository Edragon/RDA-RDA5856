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


#ifndef ID3_PARSER_H
#define ID3_PARSER_H


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
	AudDesInfoStruct * audInfo_ptr;
}id3_tag_struct;

typedef struct
{
	UINT8 offset;
	UINT8 size;
}id3_tag_v1_field_struct;

BOOL id3_tag_parse(CONST UINT32 file , AudDesInfoStruct *aud_info);
#endif

