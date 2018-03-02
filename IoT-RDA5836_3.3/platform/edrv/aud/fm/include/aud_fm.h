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
//                                                                            //
/// @file aud_null.h                                                         //
/// That file is the interface for the null implementation of the AUD        //
/// interface.                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"
#include "aud_m.h"
#include "snd_types.h"


// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================



// =============================================================================
// aud_FmSetup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker.
/// 
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_DEVICE_CFG_T for more details.
/// @return #AUD_ERR_NO it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg);



// =============================================================================
// aud_FmStreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
/// 
/// This function  outputs the 
/// audio on the audio interface specified as a parameter, on the output selected in 
/// the #aud_Setup function. \n
/// In normal operation, when 
/// the buffer runs out, the playing will wrap at the beginning. Here, there are two ways 
/// you can handle your buffer: HAL can call a user function at the middle or 
/// at the end of the playback or, depending 
/// on your application, you can simply poll the playing state using the "reached half"
/// and "reached end functions" TODO Ask if needed, and then implement !
/// 
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmStreamStart(
                        SND_ITF_T itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg);



// =============================================================================
// aud_FmStreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmStreamStop(SND_ITF_T itf);




// =============================================================================
// aud_FmStreamPause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmStreamPause(SND_ITF_T itf, BOOL pause);



// =============================================================================
// aud_FmStreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
/// 
///
/// The #aud_StreamRecord send a message to the driver which begins the 
/// dataflow from the audio and the compression. If the function returns \c 
/// FALSE, it means that the driver cannot handle the record. The value \c len 
/// should be the nullum size of the recording buffer and when the buffer is 
/// full, the recording will automatically wrap. A 
/// user function can be called, either when the middle or the end of the 
/// buffer is reached. (TODO add iatus about polling if available)
/// 
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg);





// =============================================================================
// aud_FmTone
// -----------------------------------------------------------------------------
//  Manage the playing of a tone: DTMF or Comfort Tone. 
/// 
/// Outputs a DTMF or comfort tone
///
/// When the audio output is enabled, a DTMF or a comfort tones can be output 
/// as well. This function starts the output of a tone generated in the audio 
/// module. \n
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param tone The tone to generate
/// @param attenuation The attenuation level of the tone generator
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_FmTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start);


// =============================================================================
// aud_FmTonePause
// -----------------------------------------------------------------------------
/// This function pauses the audio tone.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_FmTonePause(SND_ITF_T itf, BOOL pause);


// =============================================================================
// aud_FmCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_FmCalibUpdateValues(SND_ITF_T itf);

