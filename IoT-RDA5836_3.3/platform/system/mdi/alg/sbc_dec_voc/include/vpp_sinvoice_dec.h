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

#ifndef vpp_SINVOICE_DEC_VOC_H
#define vpp_SINVOICE_DEC_VOC_H

#include "cs_types.h"
#include "string.h"

#define SIN_VOICE_PCM_VOC_HANDLER_NUM   2


typedef enum 
{
	SinVoiceRegStateInit,
	SinVoiceRegStateStart,
	
	SinVoiceRegStateErro,
	SinVoiceRegStateOk,
	SinVoiceRegStateOverCount
}SinVoiceRegState;

typedef enum 
{
	ProcessSinVoiceRegStateNeedInit,
	ProcessSinVoiceRegStateInitOk,

	ProcessSinVoiceRegStateCheckSpaceOk,

	ProcessSinVoiceRegStateErro,
	ProcessSinVoiceRegStateHeadOk,
	ProcessSinVoiceRegStateEndOk

}ProcessSinVoiceRegState;

typedef struct 
{
	INT32 SinVoiceResultLen;
	INT8 *pSinVoiceResult;
	SinVoiceRegState processSinVoiceState;
}ProcessSinVoiceResult;

typedef VOID (*VPP_SINVOICE_DEC_CALLBACK_T)(ProcessSinVoiceResult SinVoiceResult);

//=============================================================================
// Vpp_SinVoiceStreamDetectSet function
//-----------------------------------------------------------------------------
/// flag : 1, enable SinVoiceDec; flag : 0, disable SinVoiceDec; 
//=============================================================================
void Vpp_SinVoiceStreamDetectSet(UINT8 flag);

//=============================================================================
// Vpp_SinVoiceStreamDetectSet function
//-----------------------------------------------------------------------------
/// return : 1, enable SinVoiceDec; return : 0, disable SinVoiceDec; 
//=============================================================================
UINT8 Vpp_SinVoiceDecDetect(void);

void Vpp_SinVoiceDecInit(void);
ProcessSinVoiceResult Vpp_SinVoiceDec(INT16 *pInputPcm, INT16 ch, INT32 inputPcmSampleLen, INT16 *pCollatorBuffer);
///  @} <- End of the vpp_SINVOICE_DEC group


#endif  // vpp_SINVOICE_DEC_VOC_H

