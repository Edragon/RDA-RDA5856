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
///   @file vpp_sbc_dec.h
///   This file defines VPP AUDIO_DEC API structures and functions.
/// 

#ifndef vpp_SBC_DEC_VOC_H
#define vpp_SBC_DEC_VOC_H

#include "cs_types.h"
#include "hal_voc.h"
#include "hal_error.h"
#include "string.h"


// ============================================================================
// vpp_AudioJpeg_DEC_IN_T
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
    INT16 EQ_Type;
    INT16 mixing_audio;
} vpp_SBC_DEC_IN_T;



// ============================================================================
// vpp_AudioJpeg_DEC_OUT_T
// ----------------------------------------------------------------------------
/// Gives the status after the last frame processing and the mode of the last 
/// processed frame.

//struct of output parameters
typedef struct 
{	
	INT16 freq;
	INT16 block_mode;
	INT16 blocks;
	INT16 channel_mode;
	INT16 channels;
	INT16 alloc_mode;
	INT16 subband_mode;
	INT16 subbands;
	INT16 bitpool;
	INT16 joint;
	INT16 CRC;
	INT16 consumedLen;
	INT16 sample_length;
	INT16 ErrorStatus;
} vpp_SBC_DEC_OUT_T;



//=============================================================================
// vpp_AudioJpegDecOpen function
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
HAL_ERR_T vpp_SBCDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler); 

//=============================================================================
// vpp_AudioJpegDecClose function
//-----------------------------------------------------------------------------
/// Close VPP AUDIO_DEC Decoder, clear VoC wakeup masks. This function is 
/// called each time a AUDIO_DEC stream is stopped.
//=============================================================================
void vpp_SBCDecClose(void);


//=============================================================================
// vpp_AudioJpegDecStatus function
//-----------------------------------------------------------------------------
/// This function will read the decoder status (valid at interrupt generation). 
/// @param pDecStatus pointer to the destination status structure of type 
/// vpp_AudioJpeg_DEC_OUT_T.
//=============================================================================
void vpp_SBCDecStatus(vpp_SBC_DEC_OUT_T * pDecStatus);

//=============================================================================
// vpp_AudioJpegDecStatus function
//-----------------------------------------------------------------------------
/// This function will enable voc conver the sample of pcm from 44.1k to 48k. 
//=============================================================================

void vpp_SBCDecSampleCoverEnable(void);
//=============================================================================
// vpp_AudioJpegDecStatus function
//-----------------------------------------------------------------------------
/// This function will Disable voc conver the sample of pcm from 44.1k to 48k. 
//=============================================================================

void vpp_SBCDecSampleCoverDisable(void);

//=============================================================================
// vpp_SBCDecSetDetectMuteThd
//-----------------------------------------------------------------------------
///Set Detect Mute Thd;
//=============================================================================
UINT16 vpp_SBCDecSetDetectMuteThd(short Thd);

//=============================================================================
// vpp_SBCDecDetectMute function
//-----------------------------------------------------------------------------
/// return 1:mute;
//=============================================================================
UINT16 vpp_SBCDecDetectMute(void);

//=============================================================================
// vpp_SBCSetMixMode function
//-----------------------------------------------------------------------------
/// mdoe 0&1:use target set; 2:use left; 3:use right
//=============================================================================
void vpp_SBCSetMixMode(UINT16 mode);

//=============================================================================
// vpp_SBCGetVoCNoUsedAddr function
//-----------------------------------------------------------------------------
UINT16 * vpp_SBCGetVoCNoUsedAddr(void);

//=============================================================================
// vpp_SBCDecCheckOutPcmAddr function
//-----------------------------------------------------------------------------
void vpp_SBCDecCheckOutPcmAddr(UINT32 *pAddrBegain, UINT32 *pAddrEnd);


//=============================================================================
// vpp_AudioJpegDecScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule decoding of one AUDIO_DEC coded frame.
/// @param pStreamIn pointer to the input stream
/// @param pStreamOut pointer to the output stream
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T  vpp_SBCDecScheduleOneFrame(vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN);

//=============================================================================
// vpp_SBCGetBandInfo function
//-----------------------------------------------------------------------------
void vpp_SBCGetBandInfo(uint16 * band_buf);

//=============================================================================
// vpp_SBCDecSetAudioParamReloadFlag function
//-----------------------------------------------------------------------------
void vpp_SBCDecSetAudioParamReloadFlag(UINT8 flag);

///  @} <- End of the vpp_AudioJpeg_DEC group



#endif  // vpp_SBC_DEC_VOC_H

