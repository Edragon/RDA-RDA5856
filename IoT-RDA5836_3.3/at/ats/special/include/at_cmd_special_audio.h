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
 *	at_cmd_special_audio.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_AUDIO_H_
#define _AT_CMD_EX_AUDIO_H_

#  include "at_common.h"

#define AT_EX_MONI_TIM_ID         20
#define AT_EX_MONP_TIM_ID         21

#  if 0
#    define AT_AUDIO_TEST_HANDSET 	0
// ¶ú»ú
#    define AT_AUDIO_TEST_EARPIECE 	1
// ÌýÍ²
#    define AT_AUDIO_TEST_LOUDSPEAKER	2
// ÃâÌá
#endif

#define AT_AUDIO_TEST_EARPIECE_CFW 	0
// ÌýÍ²
#define AT_AUDIO_TEST_HANDSET_CFW 	1
// ¶ú»ú
#define AT_AUDIO_TEST_LOUDSPEAKER_CFW	2
// ÃâÌá

/*
 * AT^AUST
 */
extern VOID AT_AUDIO_CmdFunc_AUST(AT_CMD_PARA *pParam);

/*
 * AT^AUEND
 */
extern VOID AT_AUDIO_CmdFunc_AUEND(AT_CMD_PARA *pParam);

/*
 * AT^SAIC
 */
extern VOID AT_AUDIO_CmdFunc_SAIC(AT_CMD_PARA *pParam);

/*
 * AT+CLVL
 */
extern VOID AT_AUDIO_CmdFunc_CLVL(AT_CMD_PARA *pParam);

/*
 * AT+VGR
 */
extern VOID AT_AUDIO_CmdFunc_VGR(AT_CMD_PARA *pParam);

/*
 * AT+VGT
 */
extern VOID AT_AUDIO_CmdFunc_VGT(AT_CMD_PARA *pParam);

/*
 * AT+CMUT
 */
extern VOID AT_AUDIO_CmdFunc_CMUT(AT_CMD_PARA *pParam);
extern VOID AT_AUDIO_CmdFunc_CAUDIO(AT_CMD_PARA *pParam);
extern VOID AT_AUDIO_CmdFunc_SNFS(AT_CMD_PARA *pParam);
extern VOID AT_AUDIO_CmdFunc_CRSL(AT_CMD_PARA *pParam);

/*
 *  AT+CRMP
 */
extern VOID AT_AUDIO_CmdFunc_CRMP(AT_CMD_PARA *pParam);

typedef enum
{
    AT_RECORD_ERR_NO,
    AT_RECORD_ERR_UNKNOWN_FORMAT,
    AT_RECORD_ERR_BUSY,
    AT_RECORD_ERR_INVALID_PARAMETER,
    AT_RECORD_ERR_ACTION_NOT_ALLOWED,
    AT_RECORD_ERR_OUT_OF_MEMORY,
    AT_RECORD_ERR_CANNOT_OPEN_FILE,
    AT_RECORD_ERR_END_OF_FILE,
    AT_RECORD_ERR_TERMINATED,
    AT_RECORD_ERR_BAD_FORMAT,
    AT_RECORD_ERR_INVALID_FORMAT,
    AT_RECORD_ERR_ERROR,
} AT_RECORD_ERR_T;


typedef enum
{
    AT_RECORD_TYPE_NONE = -1,
    AT_RECORD_TYPE_GSM_FR,             /* 0 */
    AT_RECORD_TYPE_GSM_HR,              /* 1 */
    AT_RECORD_TYPE_GSM_EFR,             /* 2 */
    AT_RECORD_TYPE_AMR,                 /* 3 */
    AT_RECORD_TYPE_AMR_WB,              /* 4 */
    AT_RECORD_TYPE_DAF,                 /* 5 */
    AT_RECORD_TYPE_AAC,                 /* 6 */
    AT_RECORD_TYPE_PCM_8K,              /* 7 */
    AT_RECORD_TYPE_PCM_16K,             /* 8 */
    AT_RECORD_TYPE_G711_ALAW,           /* 9 */
    AT_RECORD_TYPE_G711_ULAW,           /* 10 */
    AT_RECORD_TYPE_DVI_ADPCM,           /* 11 */
    AT_RECORD_TYPE_VR,                  /* 12 */
    AT_RECORD_TYPE_WAV,                 /* 13 */
    AT_RECORD_TYPE_WAV_ALAW,            /* 14 */
    AT_RECORD_TYPE_WAV_ULAW,            /* 15 */
    AT_RECORD_TYPE_WAV_DVI_ADPCM,       /* 16 */
    AT_RECORD_TYPE_SMF,                    /* 17 */
    AT_RECORD_TYPE_IMELODY,                /* 18 */
    AT_RECORD_TYPE_SMF_SND,             /* 19 */
    AT_RECORD_TYPE_MMF,                /* 20 */
    AT_RECORD_TYPE_AU,                 /*  21 */
    AT_RECORD_TYPE_AIFF,                 /*  22 */
    AT_RECORD_TYPE_M4A,                  /* 23  */
    AT_RECORD_TYPE_3GP,                  /*  24 */
    AT_RECORD_TYPE_MP4,	                  /* 25 */
    AT_RECORD_TYPE_JPG,	                  /* 26 */
    AT_RECORD_TYPE_GIF,	                  /* 27 */
    AT_RECORD_TYPE_MJPG,	                  /* 28 */
    AT_RECORD_TYPE_WMA,                  /* 29 */
    AT_RECORD_TYPE_MIDI,                  /* 30 */
    AT_RECORD_TYPE_RM,			     /*31*/
    AT_RECORD_TYPE_AVSTRM,			/*32*/
    AT_RECORD_TYPE_SCO,			     /*33*/

    AT_RECORD_NO_OF_TYPE
}
AT_RECORD_type_enum;

typedef enum
{
    AT_RECORD_PLAY_MODE_AMR475,
    AT_RECORD_PLAY_MODE_AMR515,
    AT_RECORD_PLAY_MODE_AMR59,
    AT_RECORD_PLAY_MODE_AMR67,
    AT_RECORD_PLAY_MODE_AMR74,
    AT_RECORD_PLAY_MODE_AMR795,
    AT_RECORD_PLAY_MODE_AMR102,
    AT_RECORD_PLAY_MODE_AMR122,
    AT_RECORD_PLAY_MODE_FR,
    AT_RECORD_PLAY_MODE_HR,
    AT_RECORD_PLAY_MODE_EFR,
    AT_RECORD_PLAY_MODE_PCM,
    // TODO Implement that mode
    AT_RECORD_PLAY_MODE_AMR_RING,
    AT_RECORD_PLAY_MODE_MP3,
    AT_RECORD_PLAY_MODE_AAC,
    AT_RECORD_PLAY_MODE_WAV,
    AT_RECORD_PLAY_MODE_MID,
    AT_RECORD_PLAY_MODE_STREAM_PCM, //for TTS stream play
    AT_RECORD_PLAY_MODE_QTY

} AT_RECORD_PLAY_MODE_T;
#endif
