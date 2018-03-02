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
/*                                                                            */
/* FILE NAME                                                                  */
/*      dm.h                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    DM service declarations, constant definitions and macros                */
/*    See CSW developement Manual for more.                                   */
/*                                                                            */
/******************************************************************************/

#ifndef __DM_H__
#define __DM_H__

#include <cos.h>

#define USE_BUZZER	1

//
// nPlayCount
//
#define  DM_SND_REPEAT_ONE       1         //DM use this value by default if you use a invalid count. 
#define  DM_SND_REPEAT_INFINITE  0x7FFFFFFF //DM use this value as loop for ever. 

//
// nOption
//

#define DM_SND_OPTION_INTERRUPT  0x1  //Interrupt any currently playing sound and play a new one. By default, this option will be applied for all sound player. 
#define DM_SND_OPTION_PRIORITY   0x2  //Interrupts another, currently playing, sound if the new sound has a higher priority than the currently playing sound. 
#define DM_SND_OPTION_ASYNC      0x4  //Play sound asynchronously. 
#define DM_SND_OPTION_SYNC       0x8  //Play sound synchronously. Not supported currently 
#define DM_SND_OPTION_PRI_HIGH   0x10 //Play sound in high priority. 
#define DM_SND_OPTION_PRI_LOW    0x20 //Play sound in low priority. 

enum LCDType{ 
    Coolsand_MAIN_LCD,
    Coolsand_SUB_LCD
};


// =============================================================================
// CSW_LP_RESOURCE_T
// -----------------------------------------------------------------------------
/// Describes the possible users for resource management. For those users, 
/// and only through that type, can a system frequency be chosen.
// =============================================================================
typedef enum {
    CSW_LP_RESOURCE_LCDBACKLIGHT = 0,
    CSW_LP_RESOURCE_KEYBACKLIGHT,
    CSW_LP_RESOURCE_MMIACTIVE,
    CSW_LP_RESOURCE_CAMERA,
    CSW_LP_RESOURCE_AUDIO_PLAYER,
    CSW_LP_RESOURCE_VIDEO_PLAYER,
    CSW_LP_RESOURCE_UNUSED_0,
    CSW_LP_RESOURCE_UNUSED_1,
    CSW_LP_RESOURCE_QTY
} CSW_LP_RESOURCE_T;


// =============================================================================
// CSW_SYS_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the system clock.
/// This type is used to define the minimum system frequency required 
/// by a resource selected among #CSW_LP_RESOURCE_T.
// =============================================================================
typedef enum
{
    CSW_SYS_FREQ_32K    = 32768,
    CSW_SYS_FREQ_13M    = 13000000,
    CSW_SYS_FREQ_26M    = 26000000,
    CSW_SYS_FREQ_39M    = 39000000,
    CSW_SYS_FREQ_52M    = 52000000,
    CSW_SYS_FREQ_78M    = 78000000,
    CSW_SYS_FREQ_104M   = 104000000,
    CSW_SYS_FREQ_156M   = 156000000,
} CSW_SYS_FREQ_T;


// =============================================================================
// csw_SetResourceActivity
// -----------------------------------------------------------------------------
/// Require a minimum system frequency for a given resource.
/// To release the resource, allowing for lowpower, you have to require a
/// frequenct of 32kHz, ie: #CSW_SYS_FREQ_32K.
/// Any other requirement will take the resource.
///
/// @param resource Resource that requests a minimum system frequency.
/// @param freq Minimum system frequency required. The actual system fequency
/// may be faster.
// =============================================================================
PUBLIC VOID csw_SetResourceActivity(CSW_LP_RESOURCE_T resource, CSW_SYS_FREQ_T freq);



/* Codec type */
typedef enum DM_CodecMode{
    DM_DICTAPHONE_MR475,
    DM_DICTAPHONE_MR515,
    DM_DICTAPHONE_MR59,
    DM_DICTAPHONE_MR67,
    DM_DICTAPHONE_MR74,
    DM_DICTAPHONE_MR795,
    DM_DICTAPHONE_MR102,
    DM_DICTAPHONE_MR122,
    DM_DICTAPHONE_FR,
    DM_DICTAPHONE_HR,
    DM_DICTAPHONE_EFR,
    DM_DICTAPHONE_PCM,
    DM_DICTAPHONE_AMR_RING=13
}DM_PlayRingType;




#define DM_TONE_DTMF_0                           0
#define DM_TONE_DTMF_1                           1
#define DM_TONE_DTMF_2                           2
#define DM_TONE_DTMF_3                           3
#define DM_TONE_DTMF_4                           4
#define DM_TONE_DTMF_5                           5
#define DM_TONE_DTMF_6                           6
#define DM_TONE_DTMF_7                           7
#define DM_TONE_DTMF_8                           8
#define DM_TONE_DTMF_9                           9
#define DM_TONE_DTMF_A                           10
#define DM_TONE_DTMF_B                           11
#define DM_TONE_DTMF_C                           12
#define DM_TONE_DTMF_D                           13
#define DM_TONE_DTMF_S                           14
#define DM_TONE_DTMF_P                           15
#define DM_TONE_DIAL                             16
#define DM_TONE_SUBSCRIBER_BUSY                  17
#define DM_TONE_CONGESTION                       18
#define DM_TONE_RADIO_PATHACKNOWLEDGEMENT        19
#define DM_TONE_CALL_DROPPED                     20
#define DM_TONE_SPECIAL_INFORMATION              21
#define DM_TONE_CALL_WAITING                     22
#define DM_TONE_RINGING                          23

#define DM_TONE_0dB     0 
#define DM_TONE_m3dB    1
#define DM_TONE_m9d     2
#define DM_TONE_m15dB   3
#define DM_TONE_DEFAULT 4
//


/* Microphone volume. */
typedef enum    {
    DM_AUDIO_MIC_0dB = 0,     //  min
    DM_AUDIO_MIC_3dB,
    DM_AUDIO_MIC_6dB,
    DM_AUDIO_MIC_9dB,
    DM_AUDIO_MIC_12dB,
    DM_AUDIO_MIC_15dB,
    DM_AUDIO_MIC_18dB,
    DM_AUDIO_MIC_21dB,
    DM_AUDIO_MIC_24dB,
    DM_AUDIO_MIC_27dB,
    DM_AUDIO_MIC_30dB,
    DM_AUDIO_MIC_33dB,
    DM_AUDIO_MIC_36dB,
    DM_AUDIO_MIC_39dB,
    DM_AUDIO_MIC_42dB,
    DM_AUDIO_MIC_45dB,       //max
    DM_AUDIO_MIC_MUTE       // mute
}DM_MicGain;


/* Side tone gain. */
typedef enum  {
    DM_AUDIO_SIDE_MUTE = 0,    //mute
    DM_AUDIO_SIDE_m36dB,       //  min
    DM_AUDIO_SIDE_m33dB,
    DM_AUDIO_SIDE_m30dB,
    DM_AUDIO_SIDE_m27dB,
    DM_AUDIO_SIDE_m24dB,
    DM_AUDIO_SIDE_m21dB,
    DM_AUDIO_SIDE_m18dB,
    DM_AUDIO_SIDE_m15dB,
    DM_AUDIO_SIDE_m12dB,
    DM_AUDIO_SIDE_m9dB,
    DM_AUDIO_SIDE_m6dB,
    DM_AUDIO_SIDE_m3dB,
    DM_AUDIO_SIDE_0dB,
    DM_AUDIO_SIDE_3dB,
    DM_AUDIO_SIDE_6dB             //  max
}DM_SideToneGain;


/* Speaker volume */
typedef enum  {
    /// -15dB
    DM_AUDIO_SPK_m15dB = 0,
    /// -12dB
    DM_AUDIO_SPK_m12dB,
    /// -9dB
    DM_AUDIO_SPK_m9dB,
    /// -6dB
    DM_AUDIO_SPK_m6dB,
    /// -3dB
    DM_AUDIO_SPK_m3dB,
    /// 0dB
    DM_AUDIO_SPK_0dB,
    /// 3dB
    DM_AUDIO_SPK_3dB,
    /// 6dB
    DM_AUDIO_SPK_6dB,
    /// 9dB
    DM_AUDIO_SPK_9dB,
    /// 12dB
    DM_AUDIO_SPK_12dB,
    /// 15dB
    DM_AUDIO_SPK_15dB,
    /// Mute
    DM_AUDIO_SPK_MUTE
}DM_SpeakerGain;


#define DM_AUDIO_MODE_HANDSET      0
#define DM_AUDIO_MODE_EARPIECE     1
#define DM_AUDIO_MODE_LOUDSPEAKER  2
//guoh add new bluetooth audio path
#define DM_AUDIO_MODE_BLUETOOTH 			3
//Yangtt delete this function on 20070622
//BOOL DM_AudioInit (DM_SpeakerGain SpeakerGain,DM_MicGain MicGain,UINT8 Audio_mode) ;

BOOL DM_SpeakerOpen(void);
BOOL DM_MicOpen(void);
BOOL DM_SpeakerClose(void);
BOOL DM_MicClose(void);

BOOL DM_SetMicGain ( DM_MicGain nGain);
BOOL DM_SetSpeakerGain ( DM_SpeakerGain nGain);
BOOL DM_SetSideGain ( DM_SideToneGain nGain);


BOOL SRVAPI DM_SetAudioMode (UINT8 nAudioMode);

BOOL  SRVAPI DM_SetAudioMode_From_Calibration(UINT8 nAudioMode);

BOOL SRVAPI DM_GetAudioMode (UINT8* pAudioMode);
BOOL SRVAPI  DM_GetSpeakerVolume_From_Calibration(UINT8* pVolume);


BOOL SRVAPI DM_SetAudioVolume (DM_SpeakerGain nVolume);
BOOL SRVAPI  DM_GetSpeakerVolume (UINT8* pVolume);

BOOL SRVAPI  DM_SetAudio_From_Calibration_Volume (DM_SpeakerGain nVolume);

BOOL	DM_Audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode);
BOOL	DM_Audio_StopStream(VOID);
BOOL      DM_Audio_PauseStream(VOID);
BOOL      DM_Audio_ResumeStream(VOID);
VOID DM_Set_SpeechingFlag(BOOL flag) ; 
BOOL DM_Close_All_Audio(VOID);


/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words that can still be written in the record buffer.
Parameters : none
Return     : The number of words that can still be written in the record buffer.
================================================================================
*/

UINT32 DM_Audio_RemainingRecord(VOID) ;
/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words yet to be played in the stream buffer.
Parameters : none
Return     : The number of words yet to be played in the stream buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingStream(VOID) ;

BOOL SRVAPI DM_SetAudioControl( UINT8 nCmd,  UINT16 nValue);




BOOL SRVAPI DM_PlayTone (
  UINT8 nToneType,
  UINT8 nToneAttenuation,
  UINT16 nDuration
);

BOOL SRVAPI DM_StopTone (
  VOID
);

VOID DM_KeyBackLightOpen();
VOID DM_KeyBackLightClose();
VOID DM_LpwMmiActive();
VOID DM_LpwMmiInactive();
VOID DM_LpwLCDActive();
VOID DM_LpwLCDInactive();
VOID DM_LpwDeviceActive();
VOID DM_LpwDeviceInactive();

VOID DM_ClkOut_Enable(BOOL bEnable);


typedef struct _SFW_VB_DATA {
  UINT16 nDuration;
  UINT8 nAmplitude;
  UINT8 nFrequency;
} DM_VB_DATA;

typedef struct _DM_VB_SONG {
  DM_VB_DATA* pVibrateDataArray;
  UINT8 nArraySize;
  BOOL bRepeat;
} DM_VB_SONG;



BOOL SRVAPI DM_Reset(
  VOID
);




typedef struct _DM_AUD_OUT_PARAM { 
  UINT8 outBbcGain;
  UINT8 outStep;
  UINT16 sideTone;
  UINT16 outCalibrate[4];
} DM_AUD_OUT_PARAM;


#define DM_AUD_IOCTL_MUTE_MIC     0
#define DM_AUD_IOCTL_UNMUTE_MIC   1
#define DM_AUD_IOCTL_MUTE_SPK     4
#define DM_AUD_IOCTL_UNMUTE_SPK   8

#define DM_DEVICE_SPEAKER            0 /* Tone, Keypad sound to play on audio device. */
#define DM_DEVICE_MICROPHONE          1 /* Microphone sound to send on audio device. */



BOOL SRVAPI DM_DeviceSwithOff (
  VOID
);

BOOL DM_MicStartRecord(UINT32 *pBuffer, UINT32 nBufferSize,DM_PlayRingType codec,BOOL loop_mode);


BOOL SRVAPI DM_MicStopRecord (
  VOID
);




#define DM_POWRN_ON_CAUSE_KEY         0x01
#define DM_POWRN_ON_CAUSE_CHARGE      0x02
#define DM_POWRN_ON_CAUSE_ALARM       0x04
#define DM_POWRN_ON_CAUSE_EXCEPTION   0x08
#define DM_POWRN_ON_CAUSE_RESET       0x10
#define DM_POWRN_ON_CAUSE_UNKOWN      0x80

#define DM_STATUS_FLASH_INIT          0x01

typedef VOID (*PFN_DEVICE_HANDLER)(COS_EVENT*);

#define DM_GPIO(n)			   (1<<(n))

#define DM_DEVID_GPIO_0        DRV_GPIO_0
#define DM_DEVID_GPIO_1        DRV_GPIO_1
#define DM_DEVID_GPIO_2        DRV_GPIO_2
#define DM_DEVID_GPIO_6        DRV_GPIO_6
#define DM_DEVID_GPIO_7        DRV_GPIO_7
#define DM_DEVID_GPIO_8        DRV_GPIO_8
#define DM_DEVID_GPIO_9        DRV_GPIO_9 //reserved for power off/on key
#define DM_DEVID_GPIO_10       DRV_GPIO_10
#define DM_DEVID_GPIO_12       DRV_GPIO_12
#define DM_DEVID_GPIO_13       DRV_GPIO_13
#define DM_DEVID_GPIO_14       DRV_GPIO_14
#define DM_DEVID_GPIO_15       DRV_GPIO_15
#define DM_DEVID_GPIO_16       DRV_GPIO_16
#define DM_DEVID_GPIO_17       DRV_GPIO_17
#define DM_DEVID_GPIO_20       DRV_GPIO_20
#define DM_DEVID_GPIO_21       DRV_GPIO_21
#define DM_DEVID_GPIO_22       DRV_GPIO_22
#define DM_DEVID_GPIO_23       DRV_GPIO_23
#define DM_DEVID_GPIO_24       DRV_GPIO_24
#define DM_DEVID_GPIO_25       DRV_GPIO_25
#define DM_DEVID_GPIO_26       DRV_GPIO_26
#define DM_DEVID_GPIO_27       DRV_GPIO_27
#define DM_DEVID_GPIO_28       DRV_GPIO_28
#define DM_DEVID_GPIO_29       DRV_GPIO_29
#define DM_DEVID_GPIO_30       DRV_GPIO_30
#define DM_DEVID_GPIO_31       DRV_GPIO_31

#define DM_DEVID_EXTI          (32)
#define DM_DEVID_POWER_ON      (33)
#define DM_DEVID_KEY           (34)
#define DM_DEVID_ALARM         (35)
#define DM_DEVID_PM            (36)
#define DM_DEVID_DM_STATUS     (37)
#define DM_DEVID_GPADC         (38)
#define DM_DEVID_RECORD_END (39)
#define DM_DEVID_STREAM_END (40)
/*zj mod*/
#define DM_DEVID_SPI	  (41)
#define DM_DEVID_END_          (41)

BOOL SRVAPI DM_RegisterHandler( 
  UINT8 nDeviceId,
  PFN_DEVICE_HANDLER pfnDevHandler 
);


BOOL SRVAPI DM_StartGpadcDetect(UINT16 nPeriod); // in ms

BOOL SRVAPI DM_StopGpadcDetect(VOID);
//
// UART
//
typedef BOOL (*PFN_DM_UART_GET_DATA)(UINT8* pUartBuffer, UINT8 nLen);

//
// Others API to support.
//
BOOL DM_Wait(UINT32 nTick);     // Wait in 1/256 seconds step.
BOOL DM_Delay(UINT32 nSeconds); // Wait in 1 seconds step.
BOOL dm_DisableWatchDog();
BOOL dm_SetWathcDogTime(UINT32 time);
BOOL dm_SetWathcDog_KeepAlive();
BOOL DM_SendDBGEvent();//for telling jade_debug_server the phone is ready.it is just for production line
#ifdef USE_BUZZER
//
// PWT Buzzer @Longman
//

// octave
typedef enum {
	DM_PWT_OCT_0, //no use
	DM_PWT_OCT_1,
	DM_PWT_OCT_2,
	DM_PWT_OCT_3,
	DM_PWT_OCTS
}DM_PWT_OCT;

// note
typedef enum {
	DM_PWT_NOTE_C,		// 0 
	DM_PWT_NOTE_CS,		// 1 
	DM_PWT_NOTE_D,		// 2 
	DM_PWT_NOTE_DS,		// 3 
	DM_PWT_NOTE_E,		// 4 
	DM_PWT_NOTE_F,		// 5 
	DM_PWT_NOTE_FS,		// 6 
	DM_PWT_NOTE_G,		// 7 
	DM_PWT_NOTE_GS,		// 8 
	DM_PWT_NOTE_A,		// 9 
	DM_PWT_NOTE_AS,		// 10
	DM_PWT_NOTE_B,		// 11
	DM_PWT_NOTES
}DM_PWT_NOTE;

typedef enum {
	DM_BUZZER_SET_IDLE,
	DM_BUZZER_SET_EN
}DM_BUZZER_SET;


typedef enum {
	DM_BUZZER_VIBRATING_IDLE,
	DM_BUZZER_VIBRATING_EN
}DM_BUZZER_VIBRATING;

typedef enum {
	DM_BUZZER_FLASH_IDLE,
	DM_BUZZER_FLASH_KEY,
	DM_BUZZER_FLASH_LCD,
	DM_BUZZER_FLASH_BOTH
}DM_BUZZER_FLASH;

UINT8 DM_BuzzerVibratingSet(UINT8 nGpio);

// callback part added @11.22 by Longman
typedef void (*BUZZERCALLBACK)(void);

VOID DM_BuzzerDelayNmS(UINT32 nMillisecondes);
UINT8 DM_BuzzerOpen(VOID);
UINT8 DM_BuzzerStop(VOID);
UINT8 DM_BuzzerPlay(DM_PWT_OCT Oct, DM_PWT_NOTE Note, UINT8 Vol);

UINT8 DM_BuzzerVolumeSet(UINT8 Vol);
UINT8 DM_BuzzerSongStart(UINT8* pBuffer, UINT32 nBufferLen, UINT32 duration, DM_BUZZER_SET Btemp, DM_BUZZER_VIBRATING Vtemp, DM_BUZZER_FLASH Ftemp, BUZZERCALLBACK pbuzzer_callback);
UINT8 DM_BuzzerSongStop(VOID);
#endif
//
//
//

#endif // _H_
