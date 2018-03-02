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
/*      dm_win32.c                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    DM service declarations, constant definitions and macros                */
/*    See CSW developement Manual for more.                                   */
/*                                                                            */
/******************************************************************************/


#include "dm.h"

kal_uint32 lcd_frame_buffer_address;

BOOL DM_AudioInit (DM_SpeakerGain SpeakerGain,DM_MicGain MicGain,UINT8 Audio_mode){return TRUE;}

BOOL DM_SpeakerOpen(void){return TRUE;}
BOOL DM_MicOpen(void){return TRUE;}
BOOL DM_SpeakerClose(void){return TRUE;}
BOOL DM_MicClose(void){return TRUE;}

BOOL DM_SetMicGain ( DM_MicGain nGain){return TRUE;}
BOOL DM_SetSpeakerGain ( DM_SpeakerGain nGain){return TRUE;}
BOOL DM_SetSideGain ( DM_SideToneGain nGain){return TRUE;}


BOOL SRVAPI DM_SetAudioMode (UINT8 nAudioMode){return TRUE;}

BOOL SRVAPI DM_SetAudioMode_From_Calibration(UINT8 nAudioMode){return TRUE;}

BOOL SRVAPI DM_GetAudioMode (UINT8* pAudioMode){return TRUE;}
BOOL SRVAPI  DM_GetSpeakerVolume_From_Calibration(UINT8* pVolume){return TRUE;}


BOOL SRVAPI DM_SetAudioVolume (DM_SpeakerGain nVolume){return TRUE;}
BOOL SRVAPI  DM_GetSpeakerVolume (UINT8* pVolume){return TRUE;}

BOOL SRVAPI  DM_SetAudio_From_Calibration_Volume (DM_SpeakerGain nVolume){return TRUE;}

BOOL	DM_Audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode){return TRUE;}
BOOL	DM_Audio_StopStream(VOID){return TRUE;}
BOOL    DM_Audio_PauseStream(VOID){return TRUE;}
BOOL    DM_Audio_ResumeStream(VOID){return TRUE;}
VOID DM_CheckPowerOnCause(VOID){}
UINT32 gpio_initialise(VOID){return 0;}
INT32 DSM_DevInit( VOID ){return 0;}

/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words that can still be written in the record buffer.
Parameters : none
Return     : The number of words that can still be written in the record buffer.
================================================================================
*/

UINT32 DM_Audio_RemainingRecord(VOID){return 1;}
/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words yet to be played in the stream buffer.
Parameters : none
Return     : The number of words yet to be played in the stream buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingStream(VOID){return 1;}

BOOL SRVAPI DM_SetAudioControl( UINT8 nCmd,  UINT16 nValue){return TRUE;}




BOOL SRVAPI DM_PlayTone (
  UINT8 nToneType,
  UINT8 nToneAttenuation,
  UINT16 nDuration
){return TRUE;}

BOOL SRVAPI DM_StopTone (
  VOID
){return TRUE;}

VOID DM_KeyBackLightOpen(){}
VOID DM_KeyBackLightClose(){}
VOID DM_LpwMmiActive(){}
VOID DM_LpwMmiInactive(){}


BOOL SRVAPI DM_Reset(
  VOID
){return TRUE;}

BOOL SRVAPI DM_DeviceSwithOff (
  VOID
){return TRUE;}

BOOL DM_MicStartRecord(UINT32 *pBuffer, UINT32 nBufferSize,DM_PlayRingType codec,BOOL loop_mode){return TRUE;}


BOOL SRVAPI DM_MicStopRecord (
  VOID
){return TRUE;}


BOOL SRVAPI DM_RegisterHandler( 
  UINT8 nDeviceId,
  PFN_DEVICE_HANDLER pfnDevHandler 
){return TRUE;}


BOOL SRVAPI DM_StartGpadcDetect(UINT16 nPeriod){return TRUE;} // in ms

BOOL SRVAPI DM_StopGpadcDetect(VOID){return TRUE;}
//
// UART
//


//
// Others API to support.
//
BOOL DM_Wait(UINT32 nTick){return TRUE;}     // Wait in 1/256 seconds step.
BOOL DM_Delay(UINT32 nSeconds){return TRUE;} // Wait in 1 seconds step.
BOOL dm_DisableWatchDog(){return TRUE;}
BOOL dm_SetWathcDogTime(UINT32 time){return TRUE;}
BOOL dm_SetWathcDog_KeepAlive(){return TRUE;}
BOOL DM_SendDBGEvent(){return TRUE;}//for telling jade_debug_server the phone is ready.it is just for production line
void lcd_power_up(void)
{
}

void lcd_power_down(void)
{
}

VOID BlockWrite_DI(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
}
#ifdef USE_BUZZER

UINT8 DM_BuzzerVibratingSet(UINT8 nGpio){return 1;}

VOID DM_BuzzerDelayNmS(UINT32 nMillisecondes){}
UINT8 DM_BuzzerOpen(VOID){return 1;}
UINT8 DM_BuzzerStop(VOID){return 1;}
UINT8 DM_BuzzerPlay(DM_PWT_OCT Oct, DM_PWT_NOTE Note, UINT8 Vol){return 1;}

UINT8 DM_BuzzerVolumeSet(UINT8 Vol){return 1;}
UINT8 DM_BuzzerSongStart(UINT8* pBuffer, UINT32 nBufferLen, UINT32 duration, DM_BUZZER_SET Btemp, DM_BUZZER_VIBRATING Vtemp, DM_BUZZER_FLASH Ftemp, BUZZERCALLBACK pbuzzer_callback){return 1;}
UINT8 DM_BuzzerSongStop(VOID){return 1;}
#endif


UINT8 g_powerdown_cause = 0;
VOID DM_SetPowerDown_Cause(UINT8 cause)
{
    g_powerdown_cause = cause;
}
void bal_updateSoftware(void(*updateCb)(UINT32 percents))
{
}
void stopKeyPadLED(void)
{
}
//
//
//
void DestroyVideoHandle(void)
{
}


