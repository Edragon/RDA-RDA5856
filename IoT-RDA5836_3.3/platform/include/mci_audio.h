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

////////////////////////////////////////////////////////////////////////////////
// FILENAME: mci_audio.h
//
// DESCRIPTION:
//   Extended CSW API AUDIO
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MCI_AUDIO_H_
#define _MCI_AUDIO_H_

////////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"
//#include "med_main.h"

// ============================================================================
// Defines 
// ============================================================================

// ============================================================================
// Types
// ============================================================================

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

// ============================================================================
// MCI_STATE_T
// ----------------------------------------------------------------------------
/// Enum describing the various states of the service
// ============================================================================
typedef enum
{
    MCI_STATE_PLAY,
    MCI_STATE_EOF,
    MCI_STATE_STOP,
    MCI_STATE_CLOSE,
    MCI_STATE_PAUSE,
    MCI_STATE_SEEK
} MCI_STATE_T;

// ============================================================================
// MCI_AUDIO_PLAY_CALLBACK_T
// ----------------------------------------------------------------------------
/// Callback function called when the service changes state
// ============================================================================
typedef VOID (*MCI_AUDIO_PLAY_CALLBACK_T)(MCI_STATE_T state);

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
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
UINT32 MCI_AudioPlay(INT32 OutputPath,UINT8* filename,MCI_AUDIO_PLAY_CALLBACK_T callback);
#else
// MCI_MEDIA_PLAY_REQ,
UINT32 MCI_AudioPlay(INT32 OutputPath,HANDLE fileHandle, med_type_enum fielType);
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

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

#endif // _MCI_AUDIO_H_


