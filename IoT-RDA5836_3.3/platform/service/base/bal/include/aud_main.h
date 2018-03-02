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

#ifndef _AUD_MAIN_H__
#define _AUD_MAIN_H__

#include "MultimediaTask.h"
#include "ap_message.h"
#if 0//(1 == APP_SUPPORT_FM)
#include "ap_fm.h"
#endif

#define MEDIA_PLAY			1
#define MEDIA_RECORD		2
#define MEDIA_PLAYREC		3

typedef enum
{
    Music_NON,
    Music_MP3,
    Music_WAV,
    Music_WMA,
    Music_ASF,
    Music_AAC,
    Music_OGG,
    Music_SBC,
    Music_USB,
    
}Music_type_t;

typedef enum
{
    normal,   /*eq=0*/
    bass,     /*eq=1*/
    dance,    /*eq=2*/
    classical,/*eq=3*/
    treble,   /*eq=4*/
    party,    /*eq=5*/
    pop,      /*eq=6*/
    rock,     /*eq=7*/
    max_eq
}eq_t;

typedef enum
{
    no_fade,
    fast_fade,
    slow_fade,
}fade_t;

typedef struct
{
	BOOL 	fade_in;
	fade_t 	type;
	FuncPtr  cbk;//The cbk function always will be call
}AUD_FADE_INOUT_REQ_MSG_T, *AUD_FADE_INOUT_REQ_MSG_PTR;


//把一些地址参数集中在MC_OPEN命令中传送
typedef struct
{
	UINT64  file_entry;//文件名地址
	UINT8   mode;
	UINT8   type;
    INT8 *file_name;
}Open_param_t;

#if (1 == APP_SUPPORT_FM)
typedef struct
{
  UINT32  freq;
  BOOL stereo_status;
  UINT8 rssi;
}AUD_FM_REQ_STATUS_MSG_T, *AUD_FM_REQ_STATUS_MSG_PTR;
#endif

/*for compiler compatible */
typedef struct
{
    UINT8 status;
    UINT8 signal;  
    UINT8 volume;
    UINT32 progress;    // 0~10000
    UINT32 total_time;  // in millisecond
    eq_t eq;
    INT8 speed;
    UINT8 interaudioid; // internal audio id
}media_status_t;


#define MAX_AUD_DESCRIPTION     30
typedef struct tAppAudInfoDescription
{
   UINT32 time; // duration in sec
   UINT32 bitRate;
   UINT32 sampleRate;
   UINT32 createDate;
   UINT32 createTime;
   BOOL stereo;
   INT8 title[MAX_AUD_DESCRIPTION+1];
   INT8 artist[MAX_AUD_DESCRIPTION+1];
   INT8 album[MAX_AUD_DESCRIPTION+1];
   INT8 author[MAX_AUD_DESCRIPTION+1];
//   INT16 copyright[MAX_AUD_DESCRIPTION+1];
//   INT16 date[MAX_AUD_DESCRIPTION+1];
}audInfoStruct;

typedef void (* API_MEDIA_CBK_FUN)(UINT8);

typedef enum{
	AUD_COMMON,
	AUD_INTERNAL,
	AUD_MUSIC,
	AUD_RECORD,
	AUD_FM,
	AUD_BT,
	AUD_LINEIN,
	AUD_MAX
}AUD_REQ_MODE;


typedef struct{
	AUD_REQ_MODE   mode; //mp3, record,bt,fm...
	UINT8  cmd;
	UINT32 cmd_param;
}AUD_CMD_REQ_MSG_T, *AUD_CMD_REQ_MSG_PTR;

#define AUD_INTERNAL_PLAY_REQ_SET_PARAM(audio_id, times)	((((times) &0XFF) << 8) | ((audio_id) &0XFF))
#define AUD_INTERNAL_PLAY_REQ_GET_AUDIOID(param)			((param)&0XFF)
#define AUD_INTERNAL_PLAY_REQ_GET_TIMES(param)				((((param)>>8)&0XFF))


#define AUD_INIT_EVENT()     do{if(!g_audioSem) g_audioSem = COS_CreateSemaphore(0);}while(0)
#define AUD_WAIT_EVENT()  COS_WaitForSemaphore(g_audioSem, COS_WAIT_FOREVER)
#define AUD_SET_EVENT()   COS_ReleaseSemaphore(g_audioSem)


extern HANDLE g_audioSem;
BOOL aud_init(void);
VOID aud_set_result(UINT32 result); 
UINT32 aud_get_result(VOID); 
VOID aud_set_pair_results(UINT32 result, UINT32 value); 
BOOL aud_main(MULTI_MSG_PTR ev);
BOOL media_is_bt_playing();

#endif /* _AUD_MAIN_H__ */
