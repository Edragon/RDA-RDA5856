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
#define _WIN32_WINNT 0x0500
#define _UNICODE

#include <windows.h>
#include "stdafx.h"
#include "mci.h"
#include "cos.h"

extern TCHAR open_filename[MAX_PATH];

UINT32 play_time;
UINT32 play_total_time;
UINT8 paused;
UINT8 play_timer;
UINT8 play_flag;
UINT8 play_a2dp;
MCI_AUDIO_PLAY_CALLBACK_T play_callback;
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
DWORD WINAPI A2DP_Play_Thread_USB(LPVOID Arg) 
{
	UINT8 buffer[1024];
    UINT32 length = 0;
	do{
		//Avdtp_Get_Data(&length, buffer, 1024);
		Avdtp_Clean_Data();
		//if(length == 0)
			//Sleep(1);
	}while(play_a2dp);
	return 0;
}

void timeout(void *param)
{
    if(!paused)
    {
        if(play_a2dp)
        {
        }
        else
        {
            play_time += 100;

            if(play_flag && play_time >= play_total_time)
            {
                if(play_callback)
                    play_callback(0);
                COS_KillTimer(play_timer);
             	 play_timer = 0;
            }
        }
    }
}

// MCI_MEDIA_PLAY_REQ,
UINT32 MCI_AudioPlay(INT32 OutputPath,HANDLE fileHandle,mci_type_enum fielType,MCI_AUDIO_PLAY_CALLBACK_T callback,INT32 PlayProgress)
{
#if 0
    MCI_OPEN_PARMS open = {0};
    MCI_PLAY_PARMS play;
    DWORD err;
    
    open.lpstrElementName = open_filename;
    open.lpstrDeviceType = L"mpegvideo";
    
    err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_ELEMENT|MCI_WAIT, &open);
    play.dwCallback = NULL;
    play.dwTo = play.dwFrom = 0;

    if(err == 0)
        err = mciSendCommand(open.wDeviceID, MCI_PLAY, 0, &play);
#endif
    play_flag = 1;
	play_a2dp = 0;
    play_time = PlayProgress * play_total_time / 10000;
    play_callback = callback;
	if(!play_timer)
        play_timer = COS_SetTimer(100, timeout, NULL, COS_TIMER_MODE_PERIODIC);
	else
		COS_ChangeTimer(play_timer,100);
    paused = FALSE;
    return MCI_ERR_NO;
}

 MCI_ERR_T MCI_AudioGetFileInformation (CONST HANDLE FileHander,
											AudDesInfoStruct  * CONST DecInfo,
											CONST mci_type_enum FileType   )
{
    Audio_GetDetailsInfo(FileHander, DecInfo, FileType);
    return MCI_ERR_NO;
}

 UINT32 MCI_AudioGetDurationTime(HANDLE fileHandle, 
 								mci_type_enum fielType,
 								INT32 BeginPlayProgress,
 								INT32 OffsetPlayProgress,
 								MCI_ProgressInf* PlayInformation)
{
    AudDesInfoStruct decinfo;
    Audio_GetDetailsInfo(fileHandle, &decinfo, fielType);
    PlayInformation->DurationTime = play_total_time;
    return MCI_ERR_NO;
}


// ============================================================================
// MCI_AudioPause
// ----------------------------------------------------------------------------
/// Pause the current stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioPause(VOID)
{
    paused = TRUE;
    return 0;
}

// ============================================================================
// MCI_AudioResume
// ----------------------------------------------------------------------------
/// Resume previoulsy paused stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
// MCI_MEDIA_PLAY_REQ,
UINT32 MCI_AudioResume (HANDLE fileHandle)
{
    paused = FALSE;
    return 0;
}

// ============================================================================
// MCI_AudioStop
// ----------------------------------------------------------------------------
/// Stop current stream
/// @param none
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioStop (VOID)
{
    paused = FALSE;
    Avdtp_Stop_Data();
    COS_KillTimer(play_timer);
    play_timer = 0;
    play_a2dp = 0;
    return 0;
}

// ============================================================================
// MCI_AudioSetEQ
// ----------------------------------------------------------------------------
/// select equalizer mode
/// @param EQMode : Equalizer mode
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioSetEQ(AUDIO_EQ EQMode)
{
    return 0;
}

// ============================================================================
// MCI_AudioSetOutputPath
// ----------------------------------------------------------------------------
/// Allows to change the audio path while playing
/// @param OutputPath : Selects the audio path to use
/// @param Mute : If Mute=1 the sound will be muted
/// @return error code MCI_ERR_ACTION_NOT_ALLOWED, MCI_ERR_INVALID_PARAMETER,
/// MCI_ERR_NO
// ============================================================================
UINT32 MCI_AudioSetOutputPath(UINT16 OutputPath,UINT16 Mute)
{
    return 0;
}

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
UINT32 MCI_AudioSetVolume(UINT16 volume)
{
    return 0;
}

UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* PlayInformation)
{
    PlayInformation->PlayProgress = play_time * 10000 / play_total_time;
    return 0;
}


UINT32 MCI_AudioRecordStart (HANDLE fhd,mci_type_enum format,U8 quality, MCI_AUDIO_FILE_RECORD_CALLBACK_T callback)
{
    play_flag = 0;
    play_time = 0;
    //play_total_time = 0xffffffff;
    play_callback = callback;
	if(!play_timer)
        play_timer = COS_SetTimer(100, timeout, NULL, COS_TIMER_MODE_PERIODIC);
	else
		COS_ChangeTimer(play_timer,100);
    paused = FALSE;
    return MCI_ERR_NO;
}

UINT32 MCI_AudioRecordPause(VOID)
{
    paused = TRUE;
    return 0;
}

UINT32 MCI_AudioRecordResume(VOID)
{
    paused = FALSE;
    return 0;
}

UINT32 MCI_AudioRecordStop(VOID)
{
    paused = FALSE;
    COS_KillTimer(play_timer);
    play_timer = 0;
    return 0;
}

UINT32 MCI_AudioRecordGetTime(VOID)
{
    return play_time;
}

void hal_SpiCodecVolume(UINT8 vol)
{
}

UINT32 MCI_FmPlay(UINT8 volume)
{
    return 0;
}

UINT32 MCI_FmStop(void)
{
    return 0;
}

UINT32 MCI_LineinPlay(UINT8 volume)
{
    return 0;
}

UINT32 MCI_LineStop(void)
{
    return 0;
}

UINT32 MCI_PlayBTStream (bt_a2dp_audio_cap_struct *audio_cap)
{
	DWORD Ind_Thread_ID;
    play_a2dp = TRUE;
	CreateThread(NULL,0,A2DP_Play_Thread_USB,NULL,0,&Ind_Thread_ID);
    paused = FALSE;
    return 0;
}


UINT32 MCI_StartBTSCO (UINT16 handle)
{
    return 0;
}

UINT32 MCI_StopBTSCO (void)
{
    return 0;
}


UINT32 MCI_PlayInternalAudio(UINT8 type, UINT32 length, UINT8 *data)
{
    return 0;
}

UINT32 MCI_StopInternalAudio(void)
{
    return 0;
}

UINT32 aud_CodecStop(void)
{
    return 0;
}

PUBLIC VOID gpio_SetMute(BOOL mute_flag)
{

}

UINT32 MCI_PlaySideTone(UINT32 length, UINT8 *data, UINT32 times)
{
	return 0;
}
