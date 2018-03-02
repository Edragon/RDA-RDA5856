
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
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_aec_asm.h                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP AEC VoC assembly      */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_hello_asm.h
///   This is private header of VPP Audio Motion JPEG VoC assembly.
///

#ifndef VPPP_AEC_ASM_H
#define VPPP_AEC_ASM_H

#include "cs_types.h"

#ifndef CS_PLATFORM_SIMU

 // avoid double declaration in simulation
 #define RAM_X_BEGIN_ADDR               0x0000
 #define RAM_Y_BEGIN_ADDR               0x4000

#endif

#define VPP_AEC_CFG_STRUCT                    	(0 + RAM_Y_BEGIN_ADDR)
#define VPP_AEC_STATUS_STRUCT                 	(2 + RAM_Y_BEGIN_ADDR)
#define VPP_AEC_CODE_CFG_STRUCT               	(6 + RAM_Y_BEGIN_ADDR)
#define m_AECProcPara_ADDR                      	(12 + RAM_Y_BEGIN_ADDR)

#define VPP_AEC_IN_STRUCT              			(4 + RAM_X_BEGIN_ADDR)

#define VPP_AEC_MAIN_SIZE               g_vppAECCommonCodeSize
#define VPP_AEC_MAIN_ENTRY              0
#define VPP_AEC_CRITICAL_SECTION_MIN    0
#define VPP_AEC_CRITICAL_SECTION_MAX    0

typedef struct
{
	//ProcessPara
	INT16 ProcessMode;
	INT16 InitRegularFactor;
	INT16 AFUpdateFactor;
	INT16 AFCtrlSpeedUpFactor;
	INT16 AFFilterAmp;

	INT16 EchoEstMethod;
	INT16 NoiseUpdateFactor;
	INT16 SNRUpdateFactor;
	INT16 SNRLimit;
	INT16 NoiseOverEstFactor;
	INT16 NoiseGainLimit;
	INT16 EchoUpdateFactor;
	INT16 SERUpdateFactor;
	INT16 SERLimit;
	INT16 EchoOverEstFactor;
	INT16 EchoGainLimit;
	INT16 CNGFactor;
	INT16 AmpGain;	
	INT32 NoiseMin;


	INT32 PFPrevEchoEstLimit;
	INT16 PFDTEchoOverEstFactor;

	INT16 pPFEchoGainLimitAmp[9];

	
	
	//soft limiter

//	INT16 SoftLimiter_Beta_order;
//	INT16 SoftLimiter_Scale;
//	INT32 SoftLimiter_T;


	//NonLinear filter

//	INT16 NonLinear_Beta_order;
//	INT16 NonLinear_Scale;
//	INT32 NonLinear_T;


	//pre-filter

//	INT16 g_PE_PPFCoef[5]; //g_PE_PPFilterLen 5
//	INT16 NoiseWeight_threshold;

	INT16 NoiseGainLimitStep;
	INT16 AmpThr;
	
	INT16 PFCLDataSmoothFactor;
	INT16 PFCLNoiseUpdateFactor;
	INT16 PFCLThrSigDet;
	INT16 PFCLThrDT;

	INT16 DataSmoothFactor;
	INT16 PFCLChanlGain;

	INT16 PFCLThrNT;
	INT16 PFCLThrDTDiff;
	INT16 PFCLDTHoldTime;
	INT16 PFCLDTStartTime;

	INT16 PFCLDTDUpdateFactor;
	INT16 PFCLDTDThreshold;
	INT16 PFCLDTD2ndThreshold;
	INT16 StrongEchoFlag;

	INT16 PFCLDTDThrRatio;

} VPP_SPEECH_AEC_INIT_T;
// ******************************
//            COMMON
// ******************************

CONST INT32 g_vppAECCommonCode[] = {
#include "vpp_aec_asm_common.tab"
};

CONST INT32 g_vppAECCommonCodeSize = sizeof(g_vppAECCommonCode);


// ******************************
//         OTHER CODE
// ******************************

CONST INT32 g_vppAECHiCode[] = {
#include "vpp_aec_asm_hi.tab"
};

// ******************************
//            CONST
// ******************************

CONST INT32 g_vppAECConst[] =
{
#include "vpp_aec_const.tab"
};


#endif  // VPPP_AEC_ASM_H

