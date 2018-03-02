/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file vois_m.h                                                            //
/// That file describes the audio VOIce Service (VOIS) interface. This        //
/// takes care of all sound related operation needed by a phone call.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _VOIS_M_H_
#define _VOIS_M_H_

#include "cs_types.h"

#include "hal_aif.h"
#include "aud_m.h"


/// @page vois_mainpage  VOIce Service API
/// @mainpage VOIce Service API
/// 
/// This service provides the sound management for the phone calls. It takes care of the Rx 
/// decoding, the Tx encoding, the configuration of the audio interfaces in a fully 
/// integrated way. The only things to do is to start this service when needed 
/// (#vois_Start), to stop it when it becomes unneeded (#vois_Stop), and configure on 
/// the run when needed (#vois_Setup).
/// 
/// @par Configuration requirements
/// HAL must be poperly configured in order to use the aps service.
/// 
/// The API is detailed in the following group: @ref vois
///
/// @par Organisation
/// @image latex vois.png
/// @image html vois.png
///
/// @defgroup vois VOIce Service (VOIS)
/// @{
///  

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================




// =============================================================================
// VOIS_ERR_T
// -----------------------------------------------------------------------------
/// Error types for the VOIS module.
// =============================================================================
typedef enum
{
    /// No error occured
    VOIS_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible)
    VOIS_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    VOIS_ERR_ALREADY_OPENED,
    
    /// Timeout while trying to access the resource
    VOIS_ERR_RESOURCE_TIMEOUT,
    
    /// Invalid parameter
    VOIS_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    VOIS_ERR_NO_ITF,

    /// What ?
    VOIS_ERR_UNKNOWN,


    VOIS_ERR_QTY
} VOIS_ERR_T;




// =============================================================================
// VOIS_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure.
/// 
/// A configuration structure allows to record a stream with the proper configuration
/// set as far as the audio interface and the decoding are concerned.
// =============================================================================
typedef struct
{
    /// Speaker level.
    AUD_SPK_LEVEL_T spkLevel;

    /// Mic level
    AUD_MIC_LEVEL_T micLevel;

    /// Side tone
    AUD_SIDE_LEVEL_T sideLevel;

    /// Tone level
    AUD_TONE_ATTENUATION_T toneLevel;
    
    /// encoder gain
    INT16 encMute;

    /// decoder gain
    INT16 decMute;
       
} VOIS_AUDIO_CFG_T;


// =============================================================================
// VOIS_STATUS_T
// -----------------------------------------------------------------------------
/// Status returned by the VOIS to the calling entity. The possible value
/// describes various information about the status of the play.
// =============================================================================
typedef enum
{
    VOIS_STATUS_MID_BUFFER_REACHED,
    VOIS_STATUS_END_BUFFER_REACHED,
    VOIS_STATUS_NO_MORE_DATA,
    VOIS_STATUS_ERR,

    VOIS_STATUS_QTY
} VOIS_STATUS_T;

typedef VOID (*VOIS_USER_HANDLER_T)(VOIS_STATUS_T);


// ============================================================================
// VOIS_MAP_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure. A configuration structure allows to record a stream
/// with the proper configuration set as far as the audio interface and the decoding
/// are concerned.
// =============================================================================
typedef struct
{
    /// Type of speaker
    UINT32                         spkSel;                       //0x00000000
    /// Type of speaker
    UINT32                         micSel;                       //0x00000004
    /// Speaker level.
    UINT32                         spkLevel;                     //0x00000008
    /// Mic level
    UINT32                         micLevel;                     //0x0000000C
    /// Side tone
    UINT32                         sideLevel;                    //0x00000010
    /// Tone level
    UINT32                         toneLevel;                    //0x00000014
    /// encoder gain
    INT16                          encMute;                      //0x00000018
    /// decoder gain
    INT16                          decMute;                      //0x0000001A
} VOIS_MAP_AUDIO_CFG_T; //Size : 0x1C



// ============================================================================
// AUD_MAP_LEVEL_T
// -----------------------------------------------------------------------------
/// Configuration structure. A configuration structure allows to start an AUD operation
/// (start stream, start record, or start tone) with the desired parameters.
// =============================================================================
typedef struct
{
    /// Speaker level,
    UINT32                         spkLevel;                     //0x00000000
    /// Microphone level: muted or enabled
    UINT32                         micLevel;                     //0x00000004
    /// Sidetone
    UINT32                         sideLevel;                    //0x00000008
    UINT32                         toneLevel;                    //0x0000000C
} AUD_MAP_LEVEL_T; //Size : 0x10



// ============================================================================
// VPP_MAP_SPEECH_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// VPP Speech audio configuration for echo cancellation, encoder gain and anti-distorsion
/// filter, decoder gain and anti-distorsion filter.
// =============================================================================
typedef struct
{
    /// enable the echo suppresor
    INT16                          echoEsOn;                     //0x00000000
    /// echo suppresor voice activity detection threshold
    INT16                          echoEsVad;                    //0x00000002
    /// echo suppresor double-talk detection threshold
    INT16                          echoEsDtd;                    //0x00000004
    /// echo canceller relative threshold
    INT16                          echoExpRel;                   //0x00000006
    /// echo canceller mu paramater (NULL to disable the echo canceller)
    INT16                          echoExpMu;                    //0x00000008
    /// echo canceller minimum threshold
    INT16                          echoExpMin;                   //0x0000000A
    /// encoder gain
    INT16                          encMute;                      //0x0000000C
    /// decoder gain
    INT16                          decMute;                      //0x0000000E
    /// pointer to the speaker anti-distorsion filter (SDF) coefficients
    INT32*                         sdf;                          //0x00000010
    /// pointer to the mic anti-distorsion filter (MDF) coefficients
    INT32*                         mdf;                          //0x00000014
    /// push-to-talk enable flag
    INT16                          if1;                          //0x00000018
} VPP_MAP_SPEECH_AUDIO_CFG_T; //Size : 0x1C



// ============================================================================
// VOIS_CONTEXT_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    /// Hold the 'started' or 'not started' state of VOIS
    BOOL                           voisStarted;                  //0x00000000
    /// Pointer to the output (encoded) buffer of VPP This field is to be used casted
    /// as a HAL_SPEECH_ENC_OUT_T.
    VOID*                          voisEncOutput;                //0x00000004
    /// Pointer to the input (decoded) buffer of VPP This field is to be used casted
    /// as a HAL_SPEECH_DEC_IN_T.
    VOID*                          voisDecInput;                 //0x00000008
    /// Number of played trames
    UINT32                         voisTramePlayedNb;            //0x0000000C
    /// Audio configuration used to play the stream in normal state. (ie not when
    /// started or stopped. This field is to be used casted as a AUD_LEVEL_T.
    AUD_MAP_LEVEL_T                voisAudioCfg;                 //0x00000010
    /// VPP configuration used to play the stream in normal state. (ie not when started
    /// or stopped.) This field should be used casted as a VPP_SPEECH_AUDIO_CFG_T.
    VPP_MAP_SPEECH_AUDIO_CFG_T     voisVppCfg;                   //0x00000020
    /// Interface used by vois to get and tell the words To be used casted as a AUD_ITF_ID_T.
    UINT32                         voisItf;                      //0x0000003C
    /// Latest config applied to Vois.
    VOIS_MAP_AUDIO_CFG_T           voisLatestCfg;                //0x00000040
    UINT32                         mode;                         //0x0000005C
    UINT32                         bufferSize;                   //0x00000060
    INT32                          fileHandle;                   //0x00000064
} VOIS_CONTEXT_T; //Size : 0x68


#endif // _VOIS_M_H_

