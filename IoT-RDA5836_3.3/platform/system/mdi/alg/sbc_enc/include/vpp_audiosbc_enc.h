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
/*************************************************************************/
/// 
///   @file vpp_AudioJpegDec_dec.h
///   This file defines VPP AUDIO_DEC API structures and functions.
/// 

#ifndef vpp_AudioSbc_ENC_H
#define vpp_AudioSbc_ENC_H

#include "cs_types.h"
#include "hal_voc.h"
#include "hal_error.h"
#include "hal_error.h"
#include "audio_voc_map.h"

/* sample rate */
#define SBC_FREQ_16000		  16000
#define SBC_FREQ_32000		  32000
#define SBC_FREQ_44100		  44100
#define SBC_FREQ_48000		  48000

/* channel mode */
#define SBC_MODE_MONO		  0x00
#define SBC_MODE_DUAL_CHANNEL 0x01
#define SBC_MODE_STEREO		  0x02
#define SBC_MODE_JOINT_STEREO 0x03

/* allocation method */
#define SBC_AM_LOUDNESS		  0x00
#define SBC_AM_SNR		      0x01

// ============================================================================
// vpp_AudioSbc_ENC_STREAM_STATUS
// ----------------------------------------------------------------------------
/// Status after the last frame processing
// ============================================================================
typedef enum 
{
    /// continue normally
    vpp_AudioSbc_ENC_STREAM_CONTINUE = 0x0000,
    /// stop decoding normally
    vpp_AudioSbc_ENC_STREAM_STOP     = 0x0010,
    /// stop decoding and signal an error
    vpp_AudioSbc_ENC_STREAM_BREAK    = 0x0011,
    /// ignore the current frame
    vpp_AudioSbc_ENC_STREAM_IGNORE   = 0x0020
          
} vpp_AudioSbc_ENC_STREAM_STATUS;
#if 0
typedef enum
{
	Rotate_with_0  = 0,
	Rotate_with_90 = 1,
	Rotate_with_270 = 2
} ROTATE_MODE;
#endif
// ============================================================================
// vpp_AudioSbc_ENC_IN_T
// ----------------------------------------------------------------------------
/// Defines the in/out stream buffers and the reset.
// ============================================================================

// struct of input parameters
typedef struct
{
	INT16 mode;              //Unused, for alignment to Word32.
	INT16 reset;				 //Global VPP AUDIO_DEC reset, active ¡®1¡¯. It should be automatically cleared by the VPP code.

	INT16 BsbcEnable;            //use to be as a flag of whether using bluetooth
	INT16 audioItf;               //unused

	INT32* inStreamBufAddr;         //Input stream buffer address
	INT32* outStreamBufAddr;        //Output stream buffer address

	INT16 EQ_Type;             //Input stream buffer end address
	INT16 zoom_mode;//ROTATE_MODE

	INT16 Source_width;
	INT16 Source_height;

	INT16 Cut_width;
       INT16 Cut_height;
	INT16 Zoomed_width;
	INT16 Zoomed_height;
	INT32* inStreamUBufAddr;         //Input stream U buffer address
	INT32* inStreamVBufAddr;         //Input stream V buffer address
	
} vpp_AudioSbc_ENC_IN_T;



// ============================================================================
// vpp_AudioSbc_ENC_OUT_T
// ----------------------------------------------------------------------------
/// Gives the status after the last frame processing and the mode of the last 
/// processed frame.

//struct of output parameters
typedef struct 
{	
	INT16 mode;     //used by AUDIO_DEC encode
	INT16 nbChannel;  //Number of channels
	INT32 output_len; //Size of the output PCM frame(bytes).
	INT32 SampleRate;
	INT16 BitRate;
	INT16 consumedLen; //Size of the consumed data since last frame
	INT16 streamStatus;			//Stream Status 
	INT16 ErrorStatus;
	/// Image width parameter for MJPEG 
	INT16  ImgWidth;
	/// Image Height parameter for MJPEG 			  
	INT16  ImgHeight;
} vpp_AudioSbc_ENC_OUT_T;

// ============================================================================
// vpp_AudioSbc_ENC_INIT_T
// ----------------------------------------------------------------------------
/// Set Sbc parameter before sbcenc start; 
/// processed frame.

//struct of intput parameters
typedef struct 
{	
	INT16 source_sample;  
	INT16 source_ch;  
	
	INT16 sbcenc_sample; 
	INT16 sbcenc_ch_mode; 
	INT16 sbcenc_bitpool;
	INT16 sbcenc_allocation;
} vpp_AudioSbc_ENC_INIT_T;

//=============================================================================
// vpp_AudioSbcEncOpen function
//-----------------------------------------------------------------------------
/// Initialize all VPP AUDIO_DEC code and environment variables. It also set the 
/// VoC wakeup masks necessary for the synchronization. This function is called
/// each time a AUDIO_DEC stream is started. Preload the input stream buffer.
/// @param vocIrqHandler user defined handler function for the VoC interrupt.
/// @param pStreamIn pointer to the start of the input stream
/// If the function is set to NULL, no interrupts are used (the interrupt mask 
/// is not set).
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T vpp_AudioSbcEncOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler); 

//=============================================================================
// vpp_AudioSbcEncClose function
//-----------------------------------------------------------------------------
/// Close VPP AUDIO_DEC Decoder, clear VoC wakeup masks. This function is 
/// called each time a AUDIO_DEC stream is stopped.
//=============================================================================
void vpp_AudioSbcEncClose(void);


//=============================================================================
// vpp_AudioSbcEncStatus function
//-----------------------------------------------------------------------------
/// This function will read the decoder status (valid at interrupt generation). 
/// @param pDecStatus pointer to the destination status structure of type 
/// vpp_AudioSbc_ENC_OUT_T.
//=============================================================================
void vpp_AudioSbcEncStatus(vpp_AudioSbc_ENC_OUT_T * pDecStatus);

//=============================================================================
// vpp_AudioSbcEncScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule decoding of one AUDIO_DEC coded frame.
/// @param pStreamIn pointer to the input stream
/// @param pStreamOut pointer to the output stream
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T  vpp_AudioSbcEncScheduleOneFrame(vpp_AudioSbc_ENC_IN_T *pVoc_Voc_AudioDecIN);

//=============================================================================
// vpp_AudioSbcEncSetCurMode function
//-----------------------------------------------------------------------------
/// Set Up VoC Video and AUDIO mode 
/// @return error of type HAL_ERR_T
//=============================================================================

HAL_ERR_T  vpp_AudioSbcEncSetCurMode(INT16 pVoc_Video_Mode, INT16 pVoc_Audio_Mode);


//=============================================================================
// vpp_AudioSbcEncSetInputMode function
//-----------------------------------------------------------------------------
/// Set Up VoC Video and AUDIO mode 
/// @return error of type HAL_ERR_T
//=============================================================================

HAL_ERR_T  vpp_AudioSbcEncSetInputMode(INT16 pVoc_Input_Mode);

//=============================================================================
// vpp_AVDecSetMode function
//-----------------------------------------------------------------------------
/// Set Up VoC Video and AUDIO mode 
/// @return error of type HAL_ERR_T
//=============================================================================

HAL_ERR_T  vpp_AVDecSetMode(INT16 DecMode, INT16 AVMode, INT16 Reset);


//=============================================================================
// vpp_AudioSbcEncGetMode function
//-----------------------------------------------------------------------------
/// Get VOC Video and AUDIO Input mode 
/// @return mode
//=============================================================================
INT16  vpp_AudioSbcEncGetMode(void);

//=============================================================================
// vpp_AudioJpegSetReuseCode function
//reuse with VPP_H263_Zoom_Code_ExternalAddr_addr
//code_id: 0=zoom, 1=rmvb, 2=264 ...
//set after vpp_AudioSbcEncOpen			
//-----------------------------------------------------------------------------
void vpp_AudioJpegSetReuseCode(INT16 code_id);

///  @} <- End of the vpp_AudioSbc_ENC group




#endif  // vpp_AudioSbc_ENC_H
