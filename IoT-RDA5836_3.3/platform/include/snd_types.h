//==============================================================================
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
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//  $HeadURL$                                                                   
//  $Author$                                                                    
//  $Date$                                                                      
//  $Revision$                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _SND_TYPES_H_
#define _SND_TYPES_H_


#include "cs_types.h"

// =============================================================================
//  MACROS
// =============================================================================

// ============================================================================
// SND_SPK_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the speaker volume.
// =============================================================================
typedef enum
{
    SND_SPK_MUTE                                = 0x00000000,
    SND_SPK_VOL_1                               = 0x00000001,
    SND_SPK_VOL_2                               = 0x00000002,
    SND_SPK_VOL_3                               = 0x00000003,
    SND_SPK_VOL_4                               = 0x00000004,
    SND_SPK_VOL_5                               = 0x00000005,
    SND_SPK_VOL_6                               = 0x00000006,
    SND_SPK_VOL_7                               = 0x00000007,
    SND_SPK_VOL_8                               = 0x00000008,
    SND_SPK_VOL_9                               = 0x00000009,
    SND_SPK_VOL_10                              = 0x0000000a,
    SND_SPK_VOL_11                              = 0x0000000b,
    SND_SPK_VOL_12                              = 0x0000000c,
    SND_SPK_VOL_13                              = 0x0000000d,
    SND_SPK_VOL_14                              = 0x0000000e,
    SND_SPK_VOL_15                              = 0x0000000f,
    SND_SPK_VOL_16                              = 0x00000010,
    SND_SPK_VOL_17                              = 0x00000011,
    SND_SPK_VOL_18                              = 0x00000012,
    SND_SPK_VOL_19                              = 0x00000013,
    SND_SPK_VOL_20                              = 0x00000014,
    SND_SPK_VOL_21                              = 0x00000015,
    SND_SPK_VOL_22                              = 0x00000016,
    SND_SPK_VOL_23                              = 0x00000017,
    SND_SPK_VOL_24                              = 0x00000018,
    SND_SPK_VOL_25                              = 0x00000019,
    SND_SPK_VOL_26                              = 0x0000001a,
    SND_SPK_VOL_27                              = 0x0000001b,
    SND_SPK_VOL_28                              = 0x0000001c,
    SND_SPK_VOL_29                              = 0x0000001d,
    SND_SPK_VOL_30                              = 0x0000001e,
    SND_SPK_VOL_31                              = 0x0000001f,

    SND_SPK_VOL_QTY                             = 0x00000020
} SND_SPK_LEVEL_T;


// ============================================================================
// SND_MIC_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the mic: mute or enabled.
// =============================================================================
typedef enum
{
    SND_MIC_MUTE                                = 0x00000000,
    SND_MIC_ENABLE                              = 0x00000001,
    SND_MIC_VOL_QTY                             = 0x00000002
} SND_MIC_LEVEL_T;


// ============================================================================
// SND_SIDE_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the side tone volume. The value
/// SND_SIDE_VOL_TEST is used for mic to spk simple loop back test.
// =============================================================================
typedef enum
{
    SND_SIDE_MUTE                               = 0x00000000,
    SND_SIDE_VOL_1                              = 0x00000001,
    SND_SIDE_VOL_2                              = 0x00000002,
    SND_SIDE_VOL_3                              = 0x00000003,
    SND_SIDE_VOL_4                              = 0x00000004,
    SND_SIDE_VOL_5                              = 0x00000005,
    SND_SIDE_VOL_6                              = 0x00000006,
    SND_SIDE_VOL_7                              = 0x00000007,
    SND_SIDE_VOL_TEST                           = 0x00000008,
    SND_SIDE_VOL_QTY                            = 0x00000009
} SND_SIDE_LEVEL_T;


// ============================================================================
// SND_TONE_ATTENUATION_T
// -----------------------------------------------------------------------------
/// Attenuation of the tone. The attenuation can be set to 0 dB, -3 dB, -9 dB and
/// -15dB.
// =============================================================================
typedef enum
{
/// No attenuation
    SND_TONE_0DB                                = 0x00000000,
/// -3dB
    SND_TONE_M3DB                               = 0x00000001,
/// -9db
    SND_TONE_M9DB                               = 0x00000002,
/// -15dB
    SND_TONE_M15DB                              = 0x00000003,
    SND_TONE_VOL_QTY                            = 0x00000004
} SND_TONE_ATTENUATION_T;


// ============================================================================
// SND_TONE_TYPE_T
// -----------------------------------------------------------------------------
/// Tone types. The DTMF Tones are used to inform the user that the number is being
/// composed. All the standard DTMF are available: 0 to 9, A to D, pound and star.
/// \n The Comfort Tones are used to inform the user on the current state of the
/// call: Ringing, Busy, Unavailable... All frequencies needed to do the standard
/// Comfort Tones are available: 425 Hz, 950 Hz, 1400 Hz and 1800 Hz. \n
// =============================================================================
typedef enum
{
/// Tone when the '0' key
    SND_DTMF_0                                  = 0x00000000,
/// Tone when the '1' key
    SND_DTMF_1                                  = 0x00000001,
/// Tone when the '2' key
    SND_DTMF_2                                  = 0x00000002,
/// Tone when the '3' key
    SND_DTMF_3                                  = 0x00000003,
/// Tone when the '4' key
    SND_DTMF_4                                  = 0x00000004,
/// Tone when the '5' key
    SND_DTMF_5                                  = 0x00000005,
/// Tone when the '6' key
    SND_DTMF_6                                  = 0x00000006,
/// Tone when the '7' key
    SND_DTMF_7                                  = 0x00000007,
/// Tone when the '8' key
    SND_DTMF_8                                  = 0x00000008,
/// Tone when the '9' key
    SND_DTMF_9                                  = 0x00000009,
    SND_DTMF_A                                  = 0x0000000A,
    SND_DTMF_B                                  = 0x0000000B,
    SND_DTMF_C                                  = 0x0000000C,
    SND_DTMF_D                                  = 0x0000000D,
/// Tone when the * key
    SND_DTMF_S                                  = 0x0000000E,
/// Tone when the # key
    SND_DTMF_P                                  = 0x0000000F,
/// Comfort tone at 425 Hz
    SND_COMFORT_425                             = 0x00000010,
/// Comfort tone at 950 Hz
    SND_COMFORT_950                             = 0x00000011,
/// Comfort tone at 1400 Hz
    SND_COMFORT_1400                            = 0x00000012,
/// Confort tone at 1800 Hz
    SND_COMFORT_1800                            = 0x00000013,
/// No tone is emitted
    SND_NO_TONE                                 = 0x00000014
} SND_TONE_TYPE_T;


// ============================================================================
// SND_ITF_T
// -----------------------------------------------------------------------------
/// That type provide a way to identify the different audio interface.
// =============================================================================
typedef enum
{
    SND_ITF_RECEIVER                            = 0x00000000,
    SND_ITF_EAR_PIECE                           = 0x00000001,
    SND_ITF_LOUD_SPEAKER                        = 0x00000002,
    SND_ITF_BLUETOOTH                           = 0x00000003,
    SND_ITF_BT_EP                               = 0x00000004,
    SND_ITF_BT_SP                               = 0x00000005,
    SND_ITF_FM                                  = 0x00000006,
    SND_ITF_LINEIN                              = 0x00000007,
/// Number (max) of available interface to the SND driver
    SND_ITF_QTY                                 = 0x00000008,
    SND_ITF_NONE                                = 0x000000FF
} SND_ITF_T;


// ============================================================================
// SND_EQUALIZER_MODE_T
// -----------------------------------------------------------------------------
/// SND equalizer modes enumerator
// =============================================================================
typedef enum
{
    SND_EQUALIZER_OFF                           = 0xFFFFFFFF,
    SND_EQUALIZER_NORMAL                        = 0x00000000,
    SND_EQUALIZER_BASS                          = 0x00000001,
    SND_EQUALIZER_DANCE                         = 0x00000002,
    SND_EQUALIZER_CLASSICAL                     = 0x00000003,
    SND_EQUALIZER_TREBLE                        = 0x00000004,
    SND_EQUALIZER_PARTY                         = 0x00000005,
    SND_EQUALIZER_POP                           = 0x00000006,
    SND_EQUALIZER_ROCK                          = 0x00000007,
    SND_EQUALIZER_CUSTOM                        = 0x00000008,
    SND_EQUALIZER_QTY                           = 0x00000009
} SND_EQUALIZER_MODE_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SND_FREQUENCY_BARS_T
// -----------------------------------------------------------------------------
/// Audio track frequency bars
// =============================================================================
typedef UINT8 SND_FREQUENCY_BARS_T[10];





#endif

