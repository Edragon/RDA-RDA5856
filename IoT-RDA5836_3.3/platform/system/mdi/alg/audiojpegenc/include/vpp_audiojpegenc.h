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
///   @file vpp_AudioJpegEnc_enc.h
///   This file defines VPP AUDIO_ENC API structures and functions.
/// 

#ifndef vpp_AudioJpeg_ENC_H
#define vpp_AudioJpeg_ENC_H

#include "cs_types.h"
#include "hal_voc.h"
#include "hal_error.h"


// ============================================================================
// vpp_AudioJpeg_ENC_STREAM_STATUS
// ----------------------------------------------------------------------------
/// Status after the last frame processing
// ============================================================================
typedef enum 
{
    /// continue normally
    vpp_AudioJpeg_ENC_STREAM_CONTINUE = 0x0000,
    /// stop encoding normally
    vpp_AudioJpeg_ENC_STREAM_STOP     = 0x0010,
    /// stop encoding and signal an error
    vpp_AudioJpeg_ENC_STREAM_BREAK    = 0x0011,
    /// ignore the current frame
    vpp_AudioJpeg_ENC_STREAM_IGNORE   = 0x0020
          
} vpp_AudioJpeg_ENC_STREAM_STATUS;

// ============================================================================
// vpp_AudioJpeg_ENC_IN_T
// ----------------------------------------------------------------------------
/// Defines the in/out stream buffers and the reset.
// ============================================================================

// struct of input parameters
typedef struct
{
    INT16 mode; //Unused, for alignment to Word32.
    INT16 reset;  //Global VPP AUDIO_ENC reset, active ??1?бе. It should be automatically cleared by the VPP code.
    INT32* inStreamBufAddr; //Input stream buffer address
    INT32* outStreamBufAddr;  //Output stream buffer address

    INT32 SampleRate;
    INT16 BitRate;
    INT16 imag_quality;
    INT16 imag_width;
    INT16 imag_height;
    INT16 channel_num;
    INT16 reserved;
} vpp_AudioJpeg_ENC_IN_T;



// ============================================================================
// vpp_AudioJpeg_ENC_OUT_T
// ----------------------------------------------------------------------------
/// Gives the status after the last frame processing and the mode of the last 
/// processed frame.

//struct of output parameters
typedef struct 
{	
    INT16 mode;     //used by AUDIO_ENC encode
    INT16 streamStatus;			//Stream Status 
    INT32 output_len; //Size of the output PCM frame(bytes).
    INT32 consumedLen;
    INT16 ErrorStatus;
    INT16 reserve0;
} vpp_AudioJpeg_ENC_OUT_T;



//=============================================================================
// vpp_AudioJpegEncOpen function
//-----------------------------------------------------------------------------
/// Initialize all VPP AUDIO_ENC code and environment variables. It also set the 
/// VoC wakeup masks necessary for the synchronization. This function is called
/// each time a AUDIO_ENC stream is started. Preload the input stream buffer.
/// @param vocIrqHandler user defined handler function for the VoC interrupt.
/// @param pStreamIn pointer to the start of the input stream
/// If the function is set to NULL, no interrupts are used (the interrupt mask 
/// is not set).
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T vpp_AudioJpegEncOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler); 

//=============================================================================
// vpp_AudioJpegEncClose function
//-----------------------------------------------------------------------------
/// Close VPP AUDIO_ENC Encoder, clear VoC wakeup masks. This function is 
/// called each time a AUDIO_ENC stream is stopped.
//=============================================================================
void vpp_AudioJpegEncClose(void);


//=============================================================================
// vpp_AudioJpegEncStatus function
//-----------------------------------------------------------------------------
/// This function will read the encoder status (valid at interrupt generation). 
/// @param pEncStatus pointer to the destination status structure of type 
/// vpp_AudioJpeg_ENC_OUT_T.
//=============================================================================
void vpp_AudioJpegEncStatus(vpp_AudioJpeg_ENC_OUT_T * pEncStatus);

//=============================================================================
// vpp_AudioJpegEncScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule encoding of one AUDIO_ENC coded frame.
/// @param pStreamIn pointer to the input stream
/// @param pStreamOut pointer to the output stream
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T  vpp_AudioJpegEncScheduleOneFrame(vpp_AudioJpeg_ENC_IN_T *pVoc_Voc_AudioEncIN);

///  @} <- End of the vpp_AudioJpeg_ENC group
void  VPP_WRITE_JPEGHeadr(short quality_select,
                        short width,
                        short height,
                        unsigned char *JPEG_buffer);

unsigned char * vpp_AudioJpegGetOutBufferAddr(void);


VOID vpp_AudioJpegEncSetInAlgGain(INT32 alg);

#endif  // vpp_AudioJpeg_ENC_H









