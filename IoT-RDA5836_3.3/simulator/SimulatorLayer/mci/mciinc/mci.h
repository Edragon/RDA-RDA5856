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

#ifndef _MCI_H__
#define _MCI_H__
////////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"
#include "cpu_share.h"
#ifndef WIN32
#include "BTSco.h"
#endif

#define __BT_A2DP_PROFILE__

 
// ============================================================================
// Defines 
// ============================================================================

// ============================================================================
// Types
// ============================================================================

#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)

#else

typedef enum {
   MCI_TYPE_NONE=-1,
   MCI_TYPE_GSM_FR,             /* 0 */
   MCI_TYPE_GSM_HR,              /* 1 */
   MCI_TYPE_GSM_EFR,             /* 2 */
   MCI_TYPE_AMR,                 /* 3 */
   MCI_TYPE_AMR_WB,              /* 4 */
   MCI_TYPE_DAF,                 /* 5 */
   MCI_TYPE_AAC,                 /* 6 */
   MCI_TYPE_PCM_8K,              /* 7 */
   MCI_TYPE_PCM_16K,             /* 8 */
   MCI_TYPE_G711_ALAW,           /* 9 */
   MCI_TYPE_G711_ULAW,           /* 10 */
   MCI_TYPE_DVI_ADPCM,           /* 11 */
   MCI_TYPE_VR,                  /* 12 */
   MCI_TYPE_WAV,                 /* 13 */
   MCI_TYPE_WAV_ALAW,            /* 14 */
   MCI_TYPE_WAV_ULAW,            /* 15 */
   MCI_TYPE_WAV_DVI_ADPCM,       /* 16 */
   MCI_TYPE_SMF,                    /* 17 */
   MCI_TYPE_IMELODY,                /* 18 */
   MCI_TYPE_SMF_SND,             /* 19 */
   MCI_TYPE_MMF,                /* 20 */
   MCI_TYPE_AU,                 /*  21 */
   MCI_TYPE_AIFF,                 /*  22 */
   MCI_TYPE_M4A,                  /* 23  */
   MCI_TYPE_3GP,                  /*  24 */
   MCI_TYPE_MP4,	                  /* 25 */ 
   MCI_TYPE_JPG,	                  /* 26 */
   MCI_TYPE_GIF,	                  /* 27 */
   MCI_TYPE_MJPG,	                  /* 28 */
   MCI_TYPE_WMA,                  /* 29 */
   MCI_TYPE_MIDI,                  /* 30 */
   MCI_TYPE_RM,			     /*31*/
   MCI_TYPE_SBC,			     /*32*/
   MCI_TYPE_SCO,			     /*33*/
   MCI_TYPE_TONE,			     /*34*/
   MCI_TYPE_USB,			     /*35*/
    MCI_TYPE_LINEIN,                    /*36*/
   MCI_TYPE_TS,
   MCI_NO_OF_TYPE
}
mci_type_enum;
// ============================================================================
// AUDIO_EQ
// ----------------------------------------------------------------------------
/// Enum describing the various equalizer modes
// ============================================================================
typedef enum
{
    NORMAL,   /*EQ=0*/
    BASS,     /*EQ=1*/
    DANCE,    /*EQ=2*/
    CLASSICAL,/*EQ=3*/
    TREBLE,   /*EQ=4*/
    PARTY,    /*EQ=5*/
    POP,      /*EQ=6*/
    ROCK,     /*EQ=7*/
    AUDIO_EQ_NUM
} AUDIO_EQ;

// ============================================================================
// MCI_AUDIO_PATH_T
// ----------------------------------------------------------------------------
/// Enum describing the various audio paths
// ============================================================================
typedef enum
{
    MCI_PATH_NORMAL    = 0,      /* normal  */
    MCI_PATH_HP        = 1,      /* earphone, carkit */
    MCI_PATH_LSP       = 2,      /* loudspeaker  */
    MCI_PATH_FM_HP     = 3,
    MCI_PATH_FM_LSP_HP = 4
} MCI_AUDIO_PATH_T;


typedef enum 
{
    MCI_ERR_NO,
    MCI_ERR_UNKNOWN_FORMAT,
    MCI_ERR_BUSY,
    MCI_ERR_INVALID_PARAMETER,
    MCI_ERR_ACTION_NOT_ALLOWED,
    MCI_ERR_OUT_OF_MEMORY,
    MCI_ERR_CANNOT_OPEN_FILE,         		           
    MCI_ERR_END_OF_FILE,	     
    MCI_ERR_TERMINATED,		   
    MCI_ERR_BAD_FORMAT,	      
    MCI_ERR_INVALID_FORMAT,   
    MCI_ERR_ERROR,	               

	
} MCI_ERR_T;


typedef struct _PlayInfromation 
{
	INT32 PlayProgress;
	INT32 reserve;
} MCI_PlayInf;

typedef struct _ProgressInfromation 
{
	INT32 DurationTime;//ms
	INT32 reserve;
} MCI_ProgressInf;


// =============================================================================
// APBS_PLAY_MODE_T
// -----------------------------------------------------------------------------
/// This type describes the encoding mode used in a stream to play.
// =============================================================================

typedef enum
{
    MCI_PLAY_MODE_AMR475,
    MCI_PLAY_MODE_AMR515,
    MCI_PLAY_MODE_AMR59,
    MCI_PLAY_MODE_AMR67,
    MCI_PLAY_MODE_AMR74,
    MCI_PLAY_MODE_AMR795,
    MCI_PLAY_MODE_AMR102,
    MCI_PLAY_MODE_AMR122,
    MCI_PLAY_MODE_FR,
    MCI_PLAY_MODE_HR,
    MCI_PLAY_MODE_EFR,
    MCI_PLAY_MODE_PCM,
    // TODO Implement that mode
    MCI_PLAY_MODE_AMR_RING,
    MCI_PLAY_MODE_MP3,
    MCI_PLAY_MODE_AAC,
    MCI_PLAY_MODE_WAV,
    MCI_PLAY_MODE_MID,
    
    MCI_PLAY_MODE_QTY
    
} MCI_PLAY_MODE_T;

typedef enum
{
    STREAM_STATUS_REQUEST_DATA,
     STREAM_STATUS_NO_MORE_DATA,
       STREAM_STATUS_END,
    STREAM_STATUS_ERR,

} MCI_STREAM_STATUS_T;

#define AVDTP_MEDIA_CODEC_SBC				    (0)		
#define AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO		(1)		
#define AVDTP_MEDIA_CODEC_MPEG2_4_AAC		(2)		

#define MSG_MMC_AUDIODEC_A2DP       3
#define MSG_MMC_AUDIODEC_SCO       4

// ============================================================================
// MCI_AUDIO_PLAY_CALLBACK_T
// ----------------------------------------------------------------------------
/// Callback function called when the service changes state
// ============================================================================
typedef VOID (*MCI_STREAM_USER_HANDLER_T)(MCI_STREAM_STATUS_T);

 typedef VOID (*MCI_AUDIO_PLAY_CALLBACK_T)(MCI_ERR_T result);
// ============================================================================
// Functions
// ============================================================================

// ============================================================================
// MCI_AudioPlay
// ----------------------------------------------------------------------------
/// Play the selected file on selected output
/// @param OutputPath: Selects the audio path to use
/// @param filename: Unix absolute filename
/// @param callback: Callback function called when the state changes (e.g EOF)
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_UNKNOWN_FORMAT, MCI_ERR_NO
// ============================================================================

// MCI_MEDIA_PLAY_REQ,
UINT32 MCI_AudioPlay(INT32 OutputPath,HANDLE fileHandle,mci_type_enum fielType,MCI_AUDIO_PLAY_CALLBACK_T callback,INT32 PlayProgress);
// ============================================================================
// MCI_AudioPause
// ----------------------------------------------------------------------------
/// Pause the current stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioPause(VOID);

// ============================================================================
// MCI_AudioResume
// ----------------------------------------------------------------------------
/// Resume previoulsy paused stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
// MCI_MEDIA_PLAY_REQ,
UINT32 MCI_AudioResume (HANDLE fileHandle);

// ============================================================================
// MCI_AudioStop
// ----------------------------------------------------------------------------
/// Stop current stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioStop (VOID);

// ============================================================================
// MCI_AudioSetEQ
// ----------------------------------------------------------------------------
/// select equalizer mode
/// @param EQMode : Equalizer mode
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioSetEQ(AUDIO_EQ EQMode);

// ============================================================================
// MCI_AudioSetOutputPath
// ----------------------------------------------------------------------------
/// Allows to change the audio path while playing
/// @param OutputPath : Selects the audio path to use
/// @param Mute : If Mute=1 the sound will be muted
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioSetOutputPath(UINT16 OutputPath,UINT16 Mute);

// ============================================================================
// MCI_AudioSetVolume
// ----------------------------------------------------------------------------
/// Allows to change the audio volume
/// If no audio is currently played, the value will be saved and used for 
/// the next stream played
/// @param volume : Selects the audio volume
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioSetVolume(UINT16 volume);

UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* PlayInformation);


#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */


#define MAX_AUD_DESCRIPTION     30
typedef struct tAudInfoDescription
{
   UINT32 time; // duration in millisecond
   UINT32 bitRate;
   UINT32 sampleRate;
   BOOL stereo;
   char title[MAX_AUD_DESCRIPTION+1];
   char artist[MAX_AUD_DESCRIPTION+1];
   char album[MAX_AUD_DESCRIPTION+1];
   char author[MAX_AUD_DESCRIPTION+1];
//   INT16 copyright[MAX_AUD_DESCRIPTION+1];
//   INT16 date[MAX_AUD_DESCRIPTION+1];
}AudDesInfoStruct;


typedef VOID (*MCI_AUDIO_BUFFER_PLAY_CALLBACK_T)(MCI_ERR_T result);
typedef VOID (*MCI_AUDIO_FILE_RECORD_CALLBACK_T)(MCI_ERR_T result);

UINT32 MCI_AudioRecordStart (HANDLE fhd,mci_type_enum format,U8 quality, MCI_AUDIO_FILE_RECORD_CALLBACK_T callback);
UINT32 MCI_AudioRecordPause(VOID);
UINT32 MCI_AudioRecordResume(VOID);
UINT32 MCI_AudioRecordStop(VOID);
UINT32 MCI_AudioRecordGetTime(VOID);



#ifndef __BT_A2DP_CODEC_TYPES__
#define __BT_A2DP_CODEC_TYPES__

typedef struct
{
	UINT8 min_bit_pool;
	UINT8 max_bit_pool;
	UINT8 block_len; // b0: 16, b1: 12, b2: 8, b3: 4
	UINT8 subband_num; // b0: 8, b1: 4
	UINT8 alloc_method; // b0: loudness, b1: SNR
	UINT8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 16000
	UINT8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
} bt_a2dp_sbc_codec_cap_struct;



typedef struct
{
	UINT8 layer; // b0: layerIII, b1: layerII, b0: layerI
	BOOL CRC;
	UINT8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
	BOOL MPF; // is support MPF-2
	UINT8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 24000, b4: 22050, b5: 16000
	BOOL VBR; // is support VBR
	UINT16 bit_rate; // bit-rate index in ISO 11172-3 , b0:0000 ~ b14: 1110
} bt_a2dp_mp3_codec_cap_struct; /* all MPEG-1,2 Audio */




typedef struct
{
	UINT8 object_type; // b4: M4-scalable, b5: M4-LTP, b6: M4-LC, b7: M2-LC
	UINT16 sample_rate; // b0~b11: 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000
	UINT8 channels; // b0: 2, b1: 1
	BOOL VBR; // is supported VBR
	UINT32 bit_rate; // constant/peak bits per second in 23 bit UiMsbf, 0:unknown
} bt_a2dp_aac_codec_cap_struct; /* all MPEG-2,4 AAC */

typedef struct
{
	UINT8 version; // 1:ATRAC, 2:ATRAC2, 3:ATRAC3
	UINT8 channel_mode; // b0: joint stereo, b1: dual, b2: single
	UINT8 sample_rate; // b0: 48000, b1: 44100
	BOOL VBR; // is supported VBR
	UINT32 bit_rate; // bit-rate index in ATRAC, b0: 0x0012 ~ b18: 0x0000
	UINT16 max_sul; // sound unit length in 16 bits UiMsbf
} bt_a2dp_atrac_codec_cap_struct; /* all ATRAC family */

typedef union
{
	bt_a2dp_sbc_codec_cap_struct sbc;
	bt_a2dp_mp3_codec_cap_struct mp3;
//	bt_a2dp_aac_codec_cap_struct aac;
//	bt_a2dp_atrac_codec_cap_struct atrac;
} bt_a2dp_audio_codec_cap_struct;

typedef struct
{
	UINT8 codec_type; // SBC, MP3
	bt_a2dp_audio_codec_cap_struct codec_cap;
} bt_a2dp_audio_cap_struct;



#endif /* _MED_STRUCT_H */


 MCI_ERR_T MCI_AudioGetFileInformation (CONST HANDLE FileHander,
											AudDesInfoStruct  * CONST DecInfo,
											CONST mci_type_enum FileType   );


 UINT32 MCI_AudioGetDurationTime(HANDLE fileHandle, 
 								mci_type_enum fielType,
 								INT32 BeginPlayProgress,
 								INT32 OffsetPlayProgress,
 								MCI_ProgressInf* PlayInformation);

UINT32 MCI_PlayBTStream (bt_a2dp_audio_cap_struct *audio_cap);


UINT32 MCI_StartBTSCO (UINT16 handle);

UINT32 MCI_StopBTSCO (void);


UINT32 MCI_PlayInternalAudio(UINT8 type, UINT32 length, UINT8 *data);
UINT32 MCI_StopInternalAudio(void);

UINT32 MCI_GetRemainingBytes(void);
UINT32 MCI_GetWriteBuffer(UINT8 **buffer);
typedef struct 
{
	short  width;
	short  height;
	int    len;
	MCI_ERR_T state;
}DEC_UNIT;

/***************************************************************************************
sheen  2011.6.13

data:		    set input buffer address
outbuf:			set output buffer address
outbuf_len:		output buffer size, not less than display area.
width:			set scaled width of the full output image.
height:			set scaled height of the full output image.
start_x:		set x start position of the display area in scaled image.(0==first)
end_x:			set x end position of the display area in scaled image.
start_y:		set y start position of the display area in scaled image.(0==first)
end_y:			set y end position of the display area in scaled image.
kind:			set kind of the input&output format.(default=0 input jpeg in buf & output RGB565)
decUnit->width:	output display image width
decUnit->height: output display image height
decUnit->len:	output display image data size.

!!! NOTE:
	1. input width/height will be adjust inside with aspect ratio also limit within original width/height.
	2. start_x/end_x/start_y/end_y sign the real output area also be adjust with width/height.
	3. set output area within LCD width/height.

****************************************************************************************/

MCI_ERR_T MCI_DecodePicture(UINT8 *data, UINT16 length, char* outbuf,long outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit, short kind);

#define __MMC_PLAY_STREAM__

#if defined(__MMC_PLAY_STREAM__)
extern void mmc_InitStream(UINT8 *buffer, UINT32 buf_len);
extern UINT32 mmc_AddedData(const UINT8 *addedData, UINT32 addedDataBytes);
extern UINT32 mmc_GetRemainingBytes(void);
extern UINT32 mmc_GetWriteBuffer(UINT8 **buffer);
#endif /* __MMC_PLAY_STREAM__ */

extern INT32 mmc_aud_FileSeek(INT32 fd, INT32 iOffset, UINT8 iOrigin);
extern INT32 mmc_aud_GetFileSize(INT32 fd);
extern INT32 mmc_aud_FileRead(INT32 fd, UINT8 *pBuf, UINT32 iLen);

extern void mmc_aud_SetupFileHandle(
	int32 (*GetFileSize) (int32),
	int32 (*FileSeek)(int32, int32, UINT8),
	int32 (*FileRead)(int32, UINT8 *, UINT32)
);


#if 0//def CAMERA_SUPPORT
typedef struct
{
	unsigned short start_x;
	unsigned short start_y;
	unsigned short end_x;
	unsigned short end_y;
	unsigned int image_width;
	unsigned int image_height;
	unsigned short preview_rotate;	//xiaoyifeng add  10/11/1
	//effects
	unsigned short nightmode;
	unsigned short imageQuality;
	unsigned short factor;
	unsigned short contrast;
	unsigned short specialEffect;
	unsigned short brightNess;
	unsigned short whiteBlance;
	unsigned short exposure;
	unsigned short addFrame;
	// flash
	unsigned short flashenable;
	
	
}CAM_PREVIEW_STRUCT;
typedef struct
{
	unsigned short image_width;
	unsigned short image_height;
	unsigned char media_mode;
	
}CAM_CAPTURE_STRUCT;

#endif /* CAMERA_SUPPORT */

#endif // __JADE_LILY_MCI_H__


