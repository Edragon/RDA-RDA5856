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
/*      vpp_aec.c                                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file contains the VPP Multimedai API functions.              */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"

#include "hal_error.h"
#include "hal_voc.h"

#include "vpp_aec.h"
//#include "vppp_aec.h"
#include "vpp_aec_asm.h"
//#include "vppp_aec_map.h"

// ============================================================================
// VPP_AEC_CODE_CFG_T
// ----------------------------------------------------------------------------
/// Internal VPP AEC configuration structure
// ============================================================================

typedef struct
{
    /// pointer to the hi code
    INT32* hiPtr;
    /// pointer to the bye code
    INT32* byePtr;
    /// pointer to the data constants
    INT32* constPtr;
    
} VPP_AEC_CODE_CFG_T;


//=============================================================================
// vpp_AECOpen function
//-----------------------------------------------------------------------------

PUBLIC HAL_ERR_T vpp_AECOpen(HAL_VOC_IRQ_HANDLER_T  vocIrqHandler)
{
    HAL_ERR_T status;
    HAL_VOC_CFG_T cfg;
    
    VPP_AEC_STATUS_T* pStatus = hal_VocGetPointer(VPP_AEC_STATUS_STRUCT);
    
    cfg.vocCode              = g_vppAECCommonCode;
    cfg.vocCodeSize          = VPP_AEC_MAIN_SIZE;
    cfg.pcVal                = VPP_AEC_MAIN_ENTRY;
    cfg.pcValCriticalSecMin  = VPP_AEC_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = VPP_AEC_CRITICAL_SECTION_MAX;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc          = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc       = 0;
    cfg.vocIrqHandler        = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 0;
    cfg.eventMask.wakeupSof0 = 0;//1;
    cfg.eventMask.wakeupSof1 = 0;

    status = hal_VocOpen(&cfg);

    if (HAL_ERR_RESOURCE_BUSY == status) 
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
    else if (HAL_ERR_RESOURCE_RESET == status) 
    {
        VPP_AEC_CODE_CFG_T *pIn  = hal_VocGetPointer(VPP_AEC_CODE_CFG_STRUCT);

        pIn->hiPtr    = hal_VocGetDmaiPointer((INT32*)g_vppAECHiCode,       HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pIn->byePtr   = hal_VocGetDmaiPointer((INT32*)g_vppAECByeCode,      HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pIn->constPtr = hal_VocGetDmaiPointer((INT32*)g_vppAECConst,        HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    }

	pStatus->mode         = VPP_AEC_MODE_NO;
	pStatus->errorStatus  = VPP_AEC_ERROR_NO;

/*****************************************************AEC*****************************************************/

	{
		VPP_SPEECH_AEC_INIT_T *Aec_Init = (VPP_SPEECH_AEC_INIT_T *)hal_VocGetPointer(m_AECProcPara_ADDR);// &RAM_Y[(m_AECProcPara_ADDR - RAM_Y_BEGIN_ADDR)/2];

		Aec_Init->ProcessMode=6;//AdaptiveFilterPostFilterEnhanced;
		Aec_Init->InitRegularFactor=8;
		Aec_Init->AFUpdateFactor=8;

		Aec_Init->AFCtrlSpeedUpFactor=32;
		Aec_Init->AFFilterAmp=32;


		Aec_Init->EchoEstMethod=0;//ECHO_COHF;

		Aec_Init->NoiseUpdateFactor=64;
		Aec_Init->SNRUpdateFactor=4;
		Aec_Init->SNRLimit=13;

		
		Aec_Init->NoiseOverEstFactor=1;

		Aec_Init->NoiseGainLimit=40;
		Aec_Init->EchoUpdateFactor=32;
		Aec_Init->SERUpdateFactor=16;
		Aec_Init->SERLimit=3;

		Aec_Init->EchoOverEstFactor=2;
		Aec_Init->EchoGainLimit=5;
		Aec_Init->CNGFactor=2;
		Aec_Init->NoiseMin=0;

		Aec_Init->PFPrevEchoEstLimit=1023;

		Aec_Init->PFDTEchoOverEstFactor=20;

		Aec_Init->pPFEchoGainLimitAmp[0]=20;
		Aec_Init->pPFEchoGainLimitAmp[1]=100;
		Aec_Init->pPFEchoGainLimitAmp[2]=10;
		Aec_Init->pPFEchoGainLimitAmp[3]=8;
		Aec_Init->pPFEchoGainLimitAmp[4]=6;
		Aec_Init->pPFEchoGainLimitAmp[5]=4;
		Aec_Init->pPFEchoGainLimitAmp[6]=2;
		Aec_Init->pPFEchoGainLimitAmp[7]=1;
		Aec_Init->pPFEchoGainLimitAmp[8]=1;

		

		Aec_Init->AmpGain=16;

		Aec_Init->NoiseGainLimitStep=0;
		Aec_Init->AmpThr=0x7f;

		Aec_Init->PFCLDataSmoothFactor=128;
		Aec_Init->PFCLNoiseUpdateFactor=64;
		Aec_Init->PFCLThrSigDet=160;
		Aec_Init->PFCLThrDT=4;

		Aec_Init->DataSmoothFactor=256;
		Aec_Init->PFCLChanlGain=128;

		Aec_Init->PFCLThrNT=64;
		Aec_Init->PFCLThrDTDiff=6;
		Aec_Init->PFCLDTHoldTime=160;
		Aec_Init->PFCLDTStartTime=16;



		Aec_Init->PFCLDTDUpdateFactor=2;
		Aec_Init->PFCLDTDThreshold=115;
		Aec_Init->PFCLDTD2ndThreshold=64;		
		Aec_Init->StrongEchoFlag=0;

		Aec_Init->PFCLDTDThrRatio=71;



		//loud-speaker mode
/*
		Aec_Init->StrongEchoFlag=1;
		Aec_Init->AFFilterAmp=128;

		Aec_Init->PFCLThrDT =64;
		Aec_Init->PFCLChanlGain = 128;

		Aec_Init->PFCLDTStartTime = 80;

		Aec_Init->PFCLDTDThreshold = 102;
		Aec_Init->PFCLDTD2ndThreshold = 64;		

		Aec_Init->PFCLDTDThrRatio=80;
*/

	}
    
    // move to STALL location (VoC irq generated)
    if (HAL_ERR_RESOURCE_BUSY == hal_VocWakeup(HAL_VOC_START))
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
 
    return status;          
}

//=============================================================================
// vpp_AECClose function
//-----------------------------------------------------------------------------
/// Close VPP AEC, clear VoC wakeup masks. This function is 
/// called each time a stream is stopped.
//=============================================================================
PUBLIC VOID vpp_AECClose(VOID)
{
    hal_VocClose();
}


//=============================================================================
// vpp_AECStatus function
//-----------------------------------------------------------------------------
/// Return the VPP AEC status structure.
/// @param pStatus : result status structure of type VPP_AEC_STATUS_T
//=============================================================================
PUBLIC VOID vpp_AECGetStatus(VPP_AEC_STATUS_T * pStatus)
{  
    // copy status structure from VoC RAM to the destination.
    *pStatus = *((VPP_AEC_STATUS_T *)hal_VocGetPointer(VPP_AEC_STATUS_STRUCT));
}



//=============================================================================
// vpp_AECSchedule function
//-----------------------------------------------------------------------------
/// @return error of type HAL_ERR_T
//=============================================================================
HAL_ERR_T vpp_AECScheduleOneFrame(VPP_AEC_CFG_T *pVoc_ACE_CFG, vpp_AEC_IN_T *pVoc_ACEIN)
{
    vpp_AEC_IN_T * pDecInVoC = (vpp_AEC_IN_T *)hal_VocGetPointer(VPP_AEC_IN_STRUCT);
    VPP_AEC_CFG_T *pAEC_CFG = (VPP_AEC_CFG_T*)hal_VocGetPointer(VPP_AEC_CFG_STRUCT);
	
    pDecInVoC->inStreamMICBufAddr   = hal_VocGetDmaiPointer(pVoc_ACEIN->inStreamMICBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->inStreamSPKBufAddr   = hal_VocGetDmaiPointer(pVoc_ACEIN->inStreamSPKBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->outStreamMICBufAddr   = hal_VocGetDmaiPointer(pVoc_ACEIN->outStreamMICBufAddr,  HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->length = pVoc_ACEIN->length;
    
    *pAEC_CFG = *pVoc_ACE_CFG;
    pVoc_ACE_CFG->reset=0;

    return hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
}

