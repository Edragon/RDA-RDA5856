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
/// @file aud_codec_gallite.h                                                         //
/// That file is the interface for the CodecWM8960 implementation of the AUD        //
/// interface.                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef _AUD_CODEC_WM9860_H_
#define _AUD_CODEC_WM9860_H_

#include "cs_types.h"
#include "snd_types.h"
#include "aud_m.h"


// =============================================================================
//  MACROS
// =============================================================================
//update by wangxin 20150914

/*! @brief Define the register address of WM8960. */

#define WM8960_ADDR         0x1a

#define WM8960_LINVOL       0x0
#define WM8960_RINVOL       0x1
#define WM8960_LOUT1        0x2
#define WM8960_ROUT1        0x3
#define WM8960_CLOCK1       0x4
#define WM8960_DACCTL1      0x5
#define WM8960_DACCTL2      0x6
#define WM8960_IFACE1       0x7
#define WM8960_CLOCK2       0x8
#define WM8960_IFACE2       0x9
#define WM8960_LDAC         0xa
#define WM8960_RDAC         0xb

#define WM8960_RESET        0xf
#define WM8960_3D           0x10
#define WM8960_ALC1         0x11
#define WM8960_ALC2         0x12
#define WM8960_ALC3         0x13
#define WM8960_NOISEG       0x14
#define WM8960_LADC         0x15
#define WM8960_RADC         0x16
#define WM8960_ADDCTL1      0x17
#define WM8960_ADDCTL2      0x18
#define WM8960_POWER1       0x19
#define WM8960_POWER2       0x1a
#define WM8960_ADDCTL3      0x1b
#define WM8960_APOP1        0x1c
#define WM8960_APOP2        0x1d

#define WM8960_ADCINPUT     0x1f 

#define WM8960_LINPATH      0x20
#define WM8960_RINPATH      0x21
#define WM8960_LOUTMIX      0x22

#define WM8960_ROUTMIX      0x25
#define WM8960_MONOMIX1     0x26
#define WM8960_MONOMIX2     0x27
#define WM8960_LOUT2        0x28
#define WM8960_ROUT2        0x29
#define WM8960_MONO         0x2a
#define WM8960_INBMIX1      0x2b
#define WM8960_INBMIX2		0x2c
#define WM8960_BYPASS1		0x2d
#define WM8960_BYPASS2		0x2e
#define WM8960_POWER3		0x2f
#define WM8960_ADDCTL4		0x30
#define WM8960_CLASSD1		0x31

#define WM8960_CLASSD3		0x33
#define WM8960_PLL1		0x34
#define WM8960_PLL2		0x35
#define WM8960_PLL3		0x36
#define WM8960_PLL4		0x37

/* Cache register number */
#define WM8960_CACHEREGNUM   56

/*
 * Field Definitions.
 */

/* */
#define LEFT_INPUT_VOL_LINVOL

/*! @brief WM8960_IFACE1 FORMAT bits */
#define WM8960_IFACE1_FORMAT_MASK        0x03
#define WM8960_IFACE1_FORMAT_SHIFT       0x00
#define WM8960_IFACE1_FORMAT_RJ          0x00
#define WM8960_IFACE1_FORMAT_LJ          0x01
#define WM8960_IFACE1_FORMAT_I2S         0x02
#define WM8960_IFACE1_FORMAT_DSP         0x03
#define WM8960_IFACE1_FORMAT(x)          ((x << WM8960_IFACE1_FORMAT_SHIFT) & WM8960_IFACE1_FORMAT_MASK)

/*! @brief WM8960_IFACE1 WL bits */
#define WM8960_IFACE1_WL_MASK            0x0C
#define WM8960_IFACE1_WL_SHIFT           0x02
#define WM8960_IFACE1_WL_16BITS          0x00
#define WM8960_IFACE1_WL_20BITS          0x01
#define WM8960_IFACE1_WL_24BITS          0x02
#define WM8960_IFACE1_WL_32BITS          0x03
#define WM8960_IFACE1_WL(x)              ((x << WM8960_IFACE1_WL_SHIFT) & WM8960_IFACE1_WL_MASK)

/*! @brief WM8960_IFACE1 LRP bit */
#define WM8960_IFACE1_LRP_MASK           0x10
#define WM8960_IFACE1_LRP_SHIFT          0x04
#define WM8960_IFACE1_LRCLK_NORMAL_POL   0x00
#define WM8960_IFACE1_LRCLK_INVERT_POL   0x01
#define WM8960_IFACE1_DSP_MODEA          0x00
#define WM8960_IFACE1_DSP_MODEB          0x01
#define WM8960_IFACE1_LRP(x)             ((x << WM8960_IFACE1_LRP_SHIFT) & WM8960_IFACE1_LRP_MASK)

/*! @brief WM8960_IFACE1 DLRSWAP bit */
#define WM8960_IFACE1_DLRSWAP_MASK       0x20
#define WM8960_IFACE1_DLRSWAP_SHIFT      0x05
#define WM8960_IFACE1_DACCH_NORMAL       0x00
#define WM8960_IFACE1_DACCH_SWAP         0x01
#define WM8960_IFACE1_DLRSWAP(x)         ((x << WM8960_IFACE1_DLRSWAP_SHIFT) & WM8960_IFACE1_DLRSWAP_MASK)

/*! @brief WM8960_IFACE1 MS bit */
#define WM8960_IFACE1_MS_MASK            0x40
#define WM8960_IFACE1_MS_SHIFT           0x06
#define WM8960_IFACE1_SLAVE              0x00
#define WM8960_IFACE1_MASTER             0x01
#define WM8960_IFACE1_MS(x)              ((x << WM8960_IFACE1_MS_SHIFT) & WM8960_IFACE1_MS_MASK)

/*! @brief WM8960_IFACE1 BCLKINV bit */
#define WM8960_IFACE1_BCLKINV_MASK       0x80
#define WM8960_IFACE1_BCLKINV_SHIFT      0x07
#define WM8960_IFACE1_BCLK_NONINVERT     0x00
#define WM8960_IFACE1_BCLK_INVERT        0x01
#define WM8960_IFACE1_BCLKINV(x)         ((x << WM8960_IFACE1_BCLKINV_SHIFT) & WM8960_IFACE1_BCLKINV_MASK)

/*! @brief WM8960_IFACE1 ALRSWAP bit */
#define WM8960_IFACE1_ALRSWAP_MASK       0x100
#define WM8960_IFACE1_ALRSWAP_SHIFT      0x08
#define WM8960_IFACE1_ADCCH_NORMAL       0x00
#define WM8960_IFACE1_ADCCH_SWAP         0x01
#define WM8960_IFACE1_ALRSWAP(x)         ((x << WM8960_IFACE1_ALRSWAP_SHIFT) & WM8960_IFACE1_ALRSWAP_MASK)

/*! @brief WM8960_POWER1 */
#define WM8960_POWER1_VREF_MASK        0x40
#define WM8960_POWER1_VREF_SHIFT       0x06

#define WM8960_POWER1_AINL_MASK        0x20
#define WM8960_POWER1_AINL_SHIFT       0x05

#define WM8960_POWER1_AINR_MASK        0x10
#define WM8960_POWER1_AINR_SHIFT       0x04

#define WM8960_POWER1_ADCL_MASK        0x08
#define WM8960_POWER1_ADCL_SHIFT       0x03

#define WM8960_POWER1_ADCR_MASK        0x04
#define WM8960_POWER1_ADCR_SHIFT       0x02

/*! @brief WM8960_POWER2 */
#define WM8960_POWER2_DACL_MASK        0x100
#define WM8960_POWER2_DACL_SHIFT       0x08

#define WM8960_POWER2_DACR_MASK        0x80
#define WM8960_POWER2_DACR_SHIFT       0x07

#define WM8960_POWER2_LOUT1_MASK       0x40
#define WM8960_POWER2_LOUT1_SHIFT      0x06

#define WM8960_POWER2_ROUT1_MASK       0x20
#define WM8960_POWER2_ROUT1_SHIFT      0x05

#define WM8960_POWER2_SPKL_MASK        0x10
#define WM8960_POWER2_SPKL_SHIFT       0x04

#define WM8960_POWER2_SPKR_MASK        0x08
#define WM8960_POWER2_SPKR_SHIFT       0x03

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
// aud_CodecWM8960Init
// -----------------------------------------------------------------------------
/// Init  the audio. 
// =============================================================================

PUBLIC AUD_ERR_T aud_CodecWM8960Init(VOID);
// =============================================================================
// aud_CodecWM8960Setup
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
PUBLIC AUD_ERR_T aud_CodecWM8960Setup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg);



// =============================================================================
// aud_CodecWM8960StreamStart
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
PUBLIC AUD_ERR_T aud_CodecWM8960StreamStart(
                        SND_ITF_T itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg);



// =============================================================================
// aud_CodecWM8960StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960StreamStop(SND_ITF_T itf);




// =============================================================================
// aud_CodecWM8960StreamPause
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
PUBLIC AUD_ERR_T aud_CodecWM8960StreamPause(SND_ITF_T itf, BOOL pause);



// =============================================================================
// aud_CodecWM8960StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
/// 
///
/// The #aud_StreamRecord send a message to the driver which begins the 
/// dataflow from the audio and the compression. If the function returns \c 
/// FALSE, it means that the driver cannot handle the record. The value \c len 
/// should be the maximum size of the recording buffer and when the buffer is 
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
PUBLIC AUD_ERR_T aud_CodecWM8960StreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg);





// =============================================================================
// aud_CodecWM8960Tone
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
PUBLIC AUD_ERR_T aud_CodecWM8960Tone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
//                CONST SND_TONE_ATTENUATION_T attenuation,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start);


// =============================================================================
// aud_CodecWM8960TonePause
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
PUBLIC AUD_ERR_T aud_CodecWM8960TonePause(SND_ITF_T itf, BOOL pause);



// =============================================================================
// aud_CodecWM8960CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960CalibUpdateValues(SND_ITF_T itf);


#endif // _AUD_CODEC_WM9860_H_

