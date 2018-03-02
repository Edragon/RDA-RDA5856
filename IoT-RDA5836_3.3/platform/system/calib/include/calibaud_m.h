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
/// @file
/// <!-- TODO Add a complete comment header, with @mainpage, etc --> @defgroup calib
/// Calibration Driver
///  @{
/// 
//
//==============================================================================

#ifndef _CALIBAUD_M_H_
#define _CALIBAUD_M_H_

//#include "calib_m.h"
#include "tgt_calib_m.h"
#include "aud_snd_types.h"

PUBLIC VOID calib_SetAudioCodecIngain(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);
PUBLIC UINT32  calib_GetAudioCodecIngain(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioCodecParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioCodecOutGains(AUD_ITF_T itf, UINT8 *param, UINT16 param_len, BOOL isMusic);
// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioCodecOutgains(AUD_ITF_T itf, UINT8 *param, BOOL isMusic);

// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// =============================================================================

PUBLIC VOID calib_SetAudioCodecSidetone(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);
// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioCodecSidtone(AUD_ITF_T itf, UINT8 *param);


// =============================================================================
// calib_SetAudioMusicInGainsRecord
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
//calib_GetAudioMusicInGainsRecord
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32  calib_GetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param);


// =============================================================================
// calib_SetAudioVocHPFParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocHPFParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocHPFParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocHPFParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioVocFIRTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocFIRTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocFIRTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32  calib_GetAudioVocFIRTXParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioVocFIRRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocFIRRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocFIRRxParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32  calib_GetAudioVocFIRRXParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioVocAECParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAECParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocAECParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32 calib_GetAudioVocAECParam(AUD_ITF_T itf, UINT8 *param);


// =============================================================================
// calib_SetAudioVocAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAGCTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocAGCTXParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioVocAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAGCRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocAGCRXParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioVocNSTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocNSTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocNSTXParam
// ------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocNSTXParam(AUD_ITF_T itf, UINT8 *param);
// =============================================================================
// calib_GetAudioVocNSRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocNSRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_SetAudioVocNSRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocNSRXParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_GetAudioVocEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocEQParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocEQParam
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocEQParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_GetAudioVocDigGain
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocDigGain(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocDigGain
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocDigGain(AUD_ITF_T itf, UINT8 *param);


// =============================================================================
// calib_SetAudioMusicEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicEQParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);
// =============================================================================
// calib_GetAudioMusicEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioMusicEQParam(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetAudioMusicALCParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicALCParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioMusicALCParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioMusicALCParam(AUD_ITF_T itf, UINT8 *param);


// =============================================================================
// calib_SetAudioMusicInGainsRecord
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
//calib_GetAudioMusicInGainsRecord
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32  calib_GetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_GetAudioVocMp3FIRParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocMp3Param(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);

// =============================================================================
// calib_GetAudioVocMp3FIRParam
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocMp3Param(AUD_ITF_T itf, UINT8 *param);

// =============================================================================
// calib_SetBufferData
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC BOOL calib_SetBufferData(UINT16 offset, UINT16 length, UINT8 *param);

// =============================================================================
// calib_GetBufferData
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetBufferData(UINT16 offset, UINT16 length, UINT8 *param);

// =============================================================================
// calib_GetBufferPointers
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT8* calib_GetBufferPointers(VOID);

PUBLIC BOOL calib_BurnFlash(VOID);

PUBLIC VOID calib_SetAudioVocWebRtcAGCTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);
PUBLIC VOID calib_SetAudioVocWebRtcAGCRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len);
PUBLIC  UINT32 calib_GetAudioVocWebRtcAGCTXParam(AUD_ITF_T itf, UINT8 *param);
PUBLIC  UINT32 calib_GetAudioVocWebRtcAGCRXParam(AUD_ITF_T itf, UINT8 *param);
#endif
