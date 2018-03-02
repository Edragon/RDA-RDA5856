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
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_speech_asm.c                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for the Speech.               */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_speech.h
///   This contains the VoC assembly code for the Speech.
///

#include "cs_types.h"
//#include "chip_id.h"

#define MOVE_AEC_CODE_TO_RAM 0
#ifndef MICRON_ALIGN
#define MICRON_ALIGN(a)
#endif


 

PROTECTED CONST INT32   g_vppBTSpeechMainCode[] MICRON_ALIGN(32)= {
#ifdef USE_OLD_AEC_NS
#include "vpp_speech_asm_main_old.tab"
#else
#include "vpp_speech_asm_main.tab"
#endif
};

PROTECTED CONST INT32   g_vppBTAecConstantX[] MICRON_ALIGN(32)= {
#ifdef USE_OLD_AEC_NS
#include "vpp_aec_const_old.tab"
#else
#include "vpp_aec_const.tab"
#endif
};

PROTECTED CONST INT32   g_vppBTAecCode[] MICRON_ALIGN(32)=
{
#ifdef USE_OLD_AEC_NS
#include "vpp_speech_asm_aec_old.tab"
#else
#include "vpp_speech_asm_aec.tab"
#endif
};

PROTECTED CONST INT32   g_vppSpeex_aec_const[] MICRON_ALIGN(32)=
{
#ifdef USE_OLD_AEC_NS
#include "speex_aec_const_old.tab"
#else
#include "speex_aec_const.tab"
#endif
};

PROTECTED CONST INT32   g_vppNoiseSuppressCode[] MICRON_ALIGN(32)=
{
#ifdef USE_OLD_AEC_NS
#include "vpp_speech_asm_noisesuppress_old.tab"
#else
#include "vpp_speech_asm_noisesuppress.tab"
#endif
};
PROTECTED CONST INT32   g_vppNoiseSuppressConstantX[] MICRON_ALIGN(32)=
{
#ifdef USE_OLD_AEC_NS
#include "vpp_noisesuppress_const_x_old.tab"
#else
#include "vpp_noisesuppress_const_x.tab"
#endif
};

PROTECTED CONST INT32  g_vppAgcCode[] MICRON_ALIGN(32)=
{
#include "vpp_speech_asm_agc.tab"
};

PROTECTED CONST INT32 g_vppAgcConstantX[] MICRON_ALIGN(32)=
{
#include "vpp_agc_const_x.tab"
};

PROTECTED CONST INT32  g_vppCvsdCode[] MICRON_ALIGN(32)=
{
#include "vpp_speech_asm_cvsd.tab"
};

PROTECTED CONST INT32 g_vppBTSpeechMainCodeSize MICRON_ALIGN(32)= sizeof(g_vppBTSpeechMainCode);


