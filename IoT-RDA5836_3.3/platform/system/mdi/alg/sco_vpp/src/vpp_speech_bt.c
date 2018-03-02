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
/*      vpp_speech.c                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file contains the VPP API  functions.                        */
/*                                                                       */
/*************************************************************************/
#define VPP_PRINTF

#include "cs_types.h"
#include "string.h"
#include "hal_host.h"

#include "hal_error.h"
#include "hal_voc.h"
#include "hal_speech_bt.h"
#include "hal_dma.h"

#include "vppp_speech_bt.h"
#include "vppp_speech_asm_bt.h"
#include "vppp_speech_map_bt.h"

#include "vpp_speech_bt.h"
#include "tgt_calib_m.h"
#include "aud_m.h"

#if (AUDIO_CALIB_VER == 1)
#include "analog_audio_iir.h"
#endif

#define ECHO_COHF                               			(0) //alias
#define ECHO_TF                                 			(1) //alias

#define PassThrough                             			(0) 
#define AdaptiveFilter                          			(1) 
#define AdaptiveFilterPostFilter                			(2) 
#define NoiseSuppression                        			(3) 
#define LowDelayOpenLoopAF                      			(4) 
#define TwoStepNoiseSuppression                 			(5) 
#define AdaptiveFilterPostFilterEnhanced        			(6) 


/// The audio gain value standing for mute.
#define CALIB_AUDIO_GAIN_VALUE_MUTE              (-128)

// Global variable storing the voc irq handler
PRIVATE VPP_SPEECH_WAKEUP_MODE_T g_vppSpeechWakeupMode   = VPP_SPEECH_WAKEUP_NO; 

//<1472
#define GLOBAL_NOISESUPPRESS_RAMX_BACKUP_SIZE (1350)

INT32* g_BTRxBuf= NULL;
INT32* g_BTTxBuf= NULL;
INT32* g_Bluetooth_RxBuf= NULL;
INT32* g_Bluetooth_TxBuf= NULL;

INT32* g_vppMicNoiseSuppressRAMX= NULL;
INT32* g_vppReceiverNoiseSuppressRAMX= NULL;

//The audio micgain value
INT16 gMicGain = 8;
INT16 gEchoGainLimit = 255;

PUBLIC HAL_ERR_T vpp_SpeechInitBuffer(VOID) 
{
    extern uint32 *pBtSpeechVocOverlay;
    extern uint32 *pBtSpeechNSPcmOverlay;
    g_BTRxBuf = (INT32 *)pBtSpeechVocOverlay;
    g_BTTxBuf = (INT32 *)(pBtSpeechVocOverlay+160);
    g_Bluetooth_RxBuf = (INT32 *)(pBtSpeechVocOverlay+320);
    g_Bluetooth_TxBuf = (INT32 *)(pBtSpeechVocOverlay+480);
	
    g_vppMicNoiseSuppressRAMX = (INT32 *)pBtSpeechNSPcmOverlay;
    g_vppReceiverNoiseSuppressRAMX = (INT32 *)(pBtSpeechNSPcmOverlay + GLOBAL_NOISESUPPRESS_RAMX_BACKUP_SIZE);

    memset((UINT8 *)g_BTRxBuf,0x00,640);
    memset((UINT8 *)g_BTTxBuf,0x00,640);
    memset((UINT8 *)g_Bluetooth_RxBuf,0x00,640);
    memset((UINT8 *)g_Bluetooth_TxBuf,0x00,640);
#ifdef USE_VOC_CVSD
#if (USE_XCPU_CVSD == 0)
    memset((UINT8 *)g_Bluetooth_RxBuf,0x55,640);
    memset((UINT8 *)g_Bluetooth_TxBuf,0x55,640);
#endif
#endif
    memset((UINT8 *)g_vppMicNoiseSuppressRAMX,0x0,GLOBAL_NOISESUPPRESS_RAMX_BACKUP_SIZE*4*2);

    return HAL_ERR_NO;
}

//=============================================================================
// vpp_SpeechBTOpen function
//-----------------------------------------------------------------------------
PUBLIC HAL_ERR_T vpp_SpeechBTOpen(    HAL_VOC_IRQ_HANDLER_T       vocIrqHandler, 
                                    VPP_SPEECH_WAKEUP_MODE_T    wakeupMode)
{
    HAL_ERR_T status;
    HAL_VOC_CFG_T cfg;
    VPP_SPEECH_AUDIO_CFG_T audioCfg;
    VPP_SPEECH_INIT_T *pInit = hal_VocGetPointer(VPP_SPEECH_INIT_STRUCT); 
	

    
    cfg.vocCode              = g_vppBTSpeechMainCode;
    cfg.vocCodeSize          = g_vppBTSpeechMainCodeSize;
    cfg.pcVal                = VPP_SPEECH_MAIN_ENTRY;
    cfg.pcValCriticalSecMin  = VPP_SPEECH_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = VPP_SPEECH_CRITICAL_SECTION_MAX;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc         = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc      = 0;
    cfg.vocIrqHandler       = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 1;
    cfg.eventMask.wakeupSof0 = 0;
    cfg.eventMask.wakeupSof1 = 0;

    switch (wakeupMode)
    {
            case VPP_SPEECH_WAKEUP_SW_DECENC : 
            case VPP_SPEECH_WAKEUP_SW_ENC    : cfg.eventMask.wakeupSof0 = 1; break;
            case VPP_SPEECH_WAKEUP_SW_DEC    : cfg.eventMask.wakeupSof1 = 1; break;
            case VPP_SPEECH_WAKEUP_HW_DECENC :  
            case VPP_SPEECH_WAKEUP_HW_ENC    : cfg.eventMask.wakeupIfc0 = 1; break;
            case VPP_SPEECH_WAKEUP_HW_DEC    : cfg.eventMask.wakeupIfc1 = 1; break;
            default                          : break;
    }
              
    g_vppSpeechWakeupMode = wakeupMode;
    
    status = hal_VocOpen(&cfg);

    if (HAL_ERR_RESOURCE_BUSY == status) 
    {
        return status;
    }
    else if (HAL_ERR_RESOURCE_RESET == status) 
    {
        UINT16 i;

        // Init code state structure
        for (i = 0; i < 4; i++)
        {
            pInit->codeStateTab[i] = -1;
        }

        //pInit->codePtrTab[VPP_SPEECH_EFR_AMR_IDX]          = hal_VocGetDmaiPointer(g_vppEfrAmrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_FR_IDX]               = hal_VocGetDmaiPointer(g_vppFrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_HR_IDX]               = hal_VocGetDmaiPointer(g_vppHrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_IDX]              = hal_VocGetDmaiPointer(g_vppAmrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
       // pInit->codePtrTab[VPP_SPEECH_EFR_IDX]              = hal_VocGetDmaiPointer(g_vppEfrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_475_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode475,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_515_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode515,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_59_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode59,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_67_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode67,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_74_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode74,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_795_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode795,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_102_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode102,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_ENC_122_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode122,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_475_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode475,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_515_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode515,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_59_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode59,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_67_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode67,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_74_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode74,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_795_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode795,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_102_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode102,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->codePtrTab[VPP_SPEECH_AMR_DEC_122_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode122,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

        //pInit->constPtrTab[VPP_SPEECH_EFR_AMR_CONST_X_IDX] = hal_VocGetDmaiPointer(g_vppEfrAmrConstX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->constPtrTab[VPP_SPEECH_EFR_AMR_CONST_Y_IDX] = hal_VocGetDmaiPointer(g_vppEfrAmrConstY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->constPtrTab[VPP_SPEECH_HR_CONST_X_IDX]      = hal_VocGetDmaiPointer(g_vppHrConstX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        //pInit->constPtrTab[VPP_SPEECH_HR_CONST_Y_IDX]      = hal_VocGetDmaiPointer(g_vppHrConstY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
		
	

    }

   //  *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTSpeechRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
   //  *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_CODE_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTSpeechCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
  //   *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTAecRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTAecCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_CONSTX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTAecConstantX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

 //    *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_RAMY_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppBTSpeechRAMY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     //*((INT32 *)hal_VocGetPointer(GLOBAL_MORPH_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppMorphCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     //*((INT32 *)hal_VocGetPointer(GLOBAL_MORPH_CONSTY_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppMorphConstantY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);



     *((INT32 *)hal_VocGetPointer(GLOBAL_RX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_BTRxBuf,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_TX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_BTTxBuf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	 *((INT32 *)hal_VocGetPointer(GLOBAL_BLUETOOTH_RX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_Bluetooth_RxBuf,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_BLUETOOTH_TX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_Bluetooth_TxBuf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

 	 *((INT32 *)hal_VocGetPointer(GLOBAL_MIC_NOISESUPPRESS_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppMicNoiseSuppressRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
 	 *((INT32 *)hal_VocGetPointer(GLOBAL_RECEIVER_NOISESUPPRESS_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppReceiverNoiseSuppressRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
   
     *((INT32 *)hal_VocGetPointer(GLOBAL_NOISESUPPRESS_CONSTX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppNoiseSuppressConstantX ,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	 *((INT32 *)hal_VocGetPointer(GLOBAL_NOISESUPPRESS_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppNoiseSuppressCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#ifndef USE_OLD_AEC_NS
     *((INT32 *)hal_VocGetPointer(GLOBAL_AGC_CONSTX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppAgcConstantX ,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AGC_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppAgcCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_CVSD_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppCvsdCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif
    #ifdef USE_VOC_CVSD
    #if (USE_XCPU_CVSD == 0)

/**********************************************CVSD******************************************************/
{
	int i;
	for(i=0;i<74;i++)
	{
	    *((INT32 *)hal_VocGetPointer(CVSD_LOWPASS_FILTER_STRUCT)) = 0;
	}
	
	*((INT32 *)hal_VocGetPointer(CVSD_LOWPASS_FILTER_ROUND)) = 0x00000020;//32 ,0.5
	*((INT32 *)hal_VocGetPointer(CVSD_DEC_OUTPUT_ROUND)) = 0x00000200;//;512:0.5; 266:0.6

	*((INT32 *)hal_VocGetPointer(CVSD_enc_delta)) = 0x00000000;
	*((INT32 *)hal_VocGetPointer(CVSD_dec_delta)) = 0x00000000;
	*((INT32 *)hal_VocGetPointer(CVSD_delta_min)) = 0x00002800;
	*((INT32 *)hal_VocGetPointer(CVSD_delta_max)) = 0x00140000;//extend 10 bits;
	*((INT32 *)hal_VocGetPointer(CVSD_y_min)) = 0-0x01fffc00;
	*((INT32 *)hal_VocGetPointer(CVSD_y_max)) = 0x01fffc00;
	*((INT32 *)hal_VocGetPointer(CVSD_enc_xest_l)) = 0x00000000;
	*((INT32 *)hal_VocGetPointer(CVSD_dec_xest_l)) = 0x00000000;
	*((INT32 *)hal_VocGetPointer(CVSD_ENC_JUDGE_alpha_buf)) = 0x000a000a;//dec and enc
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag)) = 0x00000000;//dec and enc


	
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+2))=0x00edff9f;//CVSD_LOWPASS_CONST_FILTER3_Coef_TAB[5]
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+4))=0x030b04c8; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+6))=0x0037ffc0;//CVSD_LOWPASS_CONST_FILTER2_Coef_TAB[7] 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+8))=0x007fff60; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+10))=0x032d0574; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+12))=0xfff8ff31; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+14))=0x006f0012;//CVSD_LOWPASS_CONST_FILTER1_Coef_TAB[21] 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+16))=0x003affc1; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+18))=0x0031ffc7; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+20))=0xffffffdf; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+22))=0xff0e0044; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+24))=0x02c806c1; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+26))=0x00e2fea9; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+28))=0x0072ff5f; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+30))=0x0036ffaf; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+32))=0x001effda; 
	*((INT32 *)hal_VocGetPointer(CVSD_enc_first_frame_flag+34))=0x000f0066;	


}
#endif
#endif

/**********************************************AEC******************************************************/
{
	
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

    	Aec_Init->AecEnbleFlag=FALSE;
	Aec_Init->AgcEnbleFlag=FALSE;

	Aec_Init->ProcessMode=AdaptiveFilterPostFilterEnhanced;
	Aec_Init->InitRegularFactor=8;
#ifdef USE_OLD_AEC_NS
	Aec_Init->AFUpdateFactor=8;              

	Aec_Init->AFCtrlSpeedUpFactor=32;        
#else
	Aec_Init->AFUpdateFactor=4;              //new:4 old:8

	Aec_Init->AFCtrlSpeedUpFactor=10;        //new:10 old:32
#endif
	Aec_Init->AFFilterAmp=32;


	Aec_Init->EchoEstMethod=ECHO_COHF;

	Aec_Init->NoiseUpdateFactor=64;
	Aec_Init->SNRUpdateFactor=4;
	Aec_Init->SNRLimit=13;
	Aec_Init->NoiseOverEstFactor=1;

	Aec_Init->NoiseGainLimit=40;
	Aec_Init->EchoUpdateFactor=32;
	Aec_Init->SERUpdateFactor=16;
	Aec_Init->SERLimit=3;

	Aec_Init->EchoOverEstFactor=2;
#ifdef USE_OLD_AEC_NS
	Aec_Init->EchoGainLimit=gEchoGainLimit;
#else
	Aec_Init->EchoGainLimit=51;         //new:51 old:255
#endif
	Aec_Init->CNGFactor=2;
	Aec_Init->AmpGain=16;
	Aec_Init->NoiseMin=0;

	Aec_Init->PFPrevEchoEstLimit=1023;

	Aec_Init->PFDTEchoOverEstFactor=20;
#ifdef USE_OLD_AEC_NS
	Aec_Init->pPFEchoGainLimitAmp[0]=20;
	Aec_Init->pPFEchoGainLimitAmp[1]=100;
	Aec_Init->pPFEchoGainLimitAmp[2]=10;
	Aec_Init->pPFEchoGainLimitAmp[3]=8;
	Aec_Init->pPFEchoGainLimitAmp[4]=6;
	Aec_Init->pPFEchoGainLimitAmp[5]=4;
	Aec_Init->pPFEchoGainLimitAmp[6]=2;
	Aec_Init->pPFEchoGainLimitAmp[7]=1;
	Aec_Init->pPFEchoGainLimitAmp[8]=1;
#else
	Aec_Init->pPFEchoGainLimitAmp[0]=3;
	Aec_Init->pPFEchoGainLimitAmp[1]=8;
	Aec_Init->pPFEchoGainLimitAmp[2]=5;
	Aec_Init->pPFEchoGainLimitAmp[3]=3;
	Aec_Init->pPFEchoGainLimitAmp[4]=1;
	Aec_Init->pPFEchoGainLimitAmp[5]=1;
	Aec_Init->pPFEchoGainLimitAmp[6]=1;
	Aec_Init->pPFEchoGainLimitAmp[7]=1;
	Aec_Init->pPFEchoGainLimitAmp[8]=1;
#endif
	

	

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


#ifdef USE_OLD_AEC_NS
	Aec_Init->PFCLDTDUpdateFactor=2;
#else
	Aec_Init->PFCLDTDUpdateFactor=16;  //new:16 old:2
#endif
	Aec_Init->PFCLDTDThreshold=115;
	Aec_Init->PFCLDTD2ndThreshold=64;		
	Aec_Init->StrongEchoFlag=0;
	Aec_Init->PFCLDTDThrRatio=71;

#ifndef USE_OLD_AEC_NS
	/* below is not in struct VPP_SPEECH_AEC_INIT_T */
	VPP_SPEECH_AEC_EXTRA_T *AecExtra_Init = hal_VocGetPointer(m_AECExtraPara_ADDR);
	AecExtra_Init->DelaySampleNum = 35;

	AecExtra_Init->FrameCntLimitNum[0] = 500;
	AecExtra_Init->FrameCntLimitNum[1] = 2000;
	AecExtra_Init->FrameCntLimitNum[2] = 5000;
	
	AecExtra_Init->NoiseCntLimitNum[0] = 200;
	AecExtra_Init->NoiseCntLimitNum[1] = 300;
	AecExtra_Init->NoiseCntLimitNum[2] = 400;
	AecExtra_Init->NoiseCntLimitNum[3] = 600;

	AecExtra_Init->NoiseWeightSNRLimitMax = 256;
	AecExtra_Init->NoiseWeightSNRLimitMin = 128;
	AecExtra_Init->NoiseWeightSNRTh1 = 128;
	AecExtra_Init->NoiseWeightSNRTh2 = 256;
	AecExtra_Init->NoiseWeightSNRLimitStep = 16;
	AecExtra_Init->NoiseWeightSub1DecayFactor = 9;

	AecExtra_Init->ResidualEchoEnable = 0;
	AecExtra_Init->AGCRrrSpkRelation = 250;
	AecExtra_Init->AddAttDB = 0;
	AecExtra_Init->MinAttDB = 0;
	AecExtra_Init->MaxAttDB = 491;
	AecExtra_Init->NPowRiseFactor = 8;
	AecExtra_Init->WorkFactFactor = 4;
	AecExtra_Init->AttExtFactor = 5;
	AecExtra_Init->AttRiseFactor = 0;
	AecExtra_Init->AttDecayFactor = 5;
	AecExtra_Init->Log2LinFactor = 8;
	
#endif

	INT16 *NotchFIlterCoef =hal_VocGetPointer(GLOBAL_NOTCH_FILTER_COEF_ADDR);

	NotchFIlterCoef[0]=31919;
	NotchFIlterCoef[1]=30529;
	NotchFIlterCoef[2]=28258;
	NotchFIlterCoef[3]=25169;
	
	NotchFIlterCoef[4]=21354;
	NotchFIlterCoef[5]=16922;
	NotchFIlterCoef[6]=12002;
	NotchFIlterCoef[7]=6734;

	NotchFIlterCoef[8]=1272;
	NotchFIlterCoef[9]=-4227;
	NotchFIlterCoef[10]=-9603;
	NotchFIlterCoef[11]=-14703;

	NotchFIlterCoef[12]=-19377;
	NotchFIlterCoef[13]=-23492;
	NotchFIlterCoef[14]=-26928;
	NotchFIlterCoef[15]=0;


}
/**********************************************AEC******************************************************/

/**********************************************Morph Voice******************************************************/

#ifdef USE_OLD_AEC_NS
     *((INT16 *)hal_VocGetPointer(MORPHVOICE_ENABLE_FLAG_ADDR)) =FALSE;

     *((INT16 *)hal_VocGetPointer(PITCH_SHIFT_ADDR)) =0x4000;
#endif

	*((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) =TRUE;
	*((INT16 *)hal_VocGetPointer(NOTCH_FILTER_ENABLE_FLAG_ADDR)) =FALSE;//
#if  !defined(SPEECH_NS_MIC_VOC_CLOSE)
	
	*((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =TRUE;
#else

	*((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =FALSE;
#endif

#if  !defined(SPEECH_NS_RECEIVER_VOC_CLOSE)
	*((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =TRUE;
#else

	*((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =FALSE;
#endif

/**********************************************Morph Voice******************************************************/



/*************************record speech*********************/
#ifdef USE_OLD_AEC_NS
     *((INT16 *)hal_VocGetPointer(GLOBAL_SPEECH_RECORD_FLAG)) =FALSE;
#endif

/*************************record speech*********************/


/*************************noise*********************/

     *((INT16 *)hal_VocGetPointer(GLOBAL_EXTERN_PROCESS_MIC_HANDLE_FLAG)) =FALSE;

/*************************extern process mic handle flag*********************/

/*************************mic noise suppress flag*********************/

     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_SPEAKER_GAIN_ADDR)) =1;
     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_LEAK_ESTIMATE_ADDR)) =8192;

/*************************mic noise suppress flag*********************/


/*************************Ext process mic flag*********************/

vpp_SpeechBTSetProcessMicEnableFlag(0);

/*************************Ext process mic flag*********************/


/*************************CVSD VOC process flag*********************/
#ifdef USE_VOC_CVSD
#if (USE_XCPU_CVSD == 0)
     *((INT16 *)hal_VocGetPointer(BT_SPEECH_VOC_CVSD_enable)) =1;
#else
     *((INT16 *)hal_VocGetPointer(BT_SPEECH_VOC_CVSD_enable)) =0;
#endif
#else
     *((INT16 *)hal_VocGetPointer(BT_SPEECH_VOC_CVSD_enable)) =0;
#endif
/*************************CVSD VOC process flag*********************/



    // irq generation
    switch (wakeupMode)
    {
        case VPP_SPEECH_WAKEUP_SW_ENC: 
        case VPP_SPEECH_WAKEUP_HW_ENC    : pInit->irqgen = VPP_SPEECH_IRQGEN_ENC; break;
        case VPP_SPEECH_WAKEUP_SW_DECENC :
        case VPP_SPEECH_WAKEUP_HW_DECENC :  
        case VPP_SPEECH_WAKEUP_SW_DEC    : 
        case VPP_SPEECH_WAKEUP_HW_DEC    : pInit->irqgen = VPP_SPEECH_IRQGEN_DEC; break;
        default                          : pInit->irqgen = VPP_SPEECH_IRQGEN_NONE; break;
    }
    
    // reset VPP speech
    vpp_SpeechBTReset();

    // default audio configuration
    audioCfg.echoEsOn   = 0;
    audioCfg.echoEsVad  = 0;
    audioCfg.echoEsDtd  = 0;
    audioCfg.echoExpRel = 0;
    audioCfg.echoExpMu  = 0;
    audioCfg.echoExpMin = 0;
    audioCfg.encMute    = VPP_SPEECH_UNMUTE;
    audioCfg.decMute    = VPP_SPEECH_UNMUTE;
    audioCfg.sdf        = NULL;
    audioCfg.mdf        = NULL;
    
    vpp_SpeechBTAudioCfg(&audioCfg);

    vpp_SpeechBTSetOutAlgGain(0);
    vpp_SpeechBTSetInAlgGain(0);
#ifndef USE_OLD_AEC_NS
	CALIB_AUDIO_VOC_DigGain_T digGain;
	digGain.in_enable = 1;
	digGain.out_enable = 1;
	digGain.scal_in = 0x400;
	digGain.scal_out = 0x400;
    vpp_SpeechBTSetScaleDigitalGainPara(&digGain, SND_SPK_MUTE);
#endif
	vpp_SpeechBTSetWebRtcAgcReloadFlag(1, 1);

    // invalidate the encoder out buffer
    ((HAL_SPEECH_ENC_OUT_T *)hal_VocGetPointer(VPP_SPEECH_ENC_OUT_STRUCT))->encMode = HAL_INVALID;
    
    // move to STALL location (VoC irq generated)
    if (HAL_ERR_RESOURCE_BUSY == hal_VocWakeup(HAL_VOC_START))
    {
            return HAL_ERR_RESOURCE_BUSY;
    }
 
    return status;
            
}

//=============================================================================
// vpp_SpeechBTClose function
//-----------------------------------------------------------------------------
/// Close VPP Speech, clear VoC wakeup masks. This function is 
/// called each time a voice call is stopped.
//=============================================================================
PUBLIC VOID vpp_SpeechBTClose(VOID)
{

    hal_VocClose();
#if 0
#ifdef SMALL_BSS_RAM_SIZE
    if(g_vppSpeechRAMX_alloc)
    {
        mmc_MemFreeAll_BssRam(g_vppSpeechRAMX_alloc);
        g_vppSpeechRAMX_alloc=NULL;
    }
    if(g_vppSpeechRAMY_alloc)
    {
        mmc_MemFreeAll_BssRam(g_vppSpeechRAMY_alloc);
        g_vppSpeechRAMY_alloc=NULL;
    }
    if(g_vppSpeechCode_alloc)
    {
        mmc_MemFreeAll_BssRam(g_vppSpeechCode_alloc);
        g_vppSpeechCode_alloc=NULL;
    }
    if(g_vppAecRAMX_alloc)
    {
        mmc_MemFreeAll_BssRam(g_vppAecRAMX_alloc);
        g_vppAecRAMX_alloc=NULL;
    }	
#endif
#endif

    g_vppSpeechWakeupMode = VPP_SPEECH_WAKEUP_NO;
    
}

//=============================================================================
// vpp_SpeechBTReset function
//-----------------------------------------------------------------------------
/// Reset VPP (including swap buffer initialization).
//=============================================================================
PUBLIC VOID vpp_SpeechBTReset(VOID)
{
    ((VPP_SPEECH_INIT_T *)hal_VocGetPointer(VPP_SPEECH_INIT_STRUCT))->reset = 1; 
}

//=============================================================================
// vpp_SpeechBTAudioCfg function
//-----------------------------------------------------------------------------
/// Configure the audio parameters of VPP Speech.
/// @param pAudioCfg : pointer to the structure containing the audio configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechBTAudioCfg(VPP_SPEECH_AUDIO_CFG_T *pAudioCfg)
{
        
    VPP_SPEECH_AUDIO_CFG_T *pDest = hal_VocGetPointer(VPP_SPEECH_AUDIO_CFG_STRUCT);
    
    // copy structure
    *pDest = *pAudioCfg;

    // convert to DMAI pointers
    pDest->sdf = hal_VocGetDmaiPointer(pDest->sdf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDest->mdf = hal_VocGetDmaiPointer(pDest->mdf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    
}

//============================================================================
// vpp_SpeechBTCodecCfg function
//-----------------------------------------------------------------------------
/// Configure the codec parameters in the decoder structure of VPP Speech.
/// @param pCodecCfg : pointer to the structure containing the decoder configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechBTCodecCfg(HAL_SPEECH_DEC_IN_T *pCodecCfg)
{
    *(HAL_SPEECH_DEC_IN_T*) vpp_SpeechBTGetRxCodBuffer() = *pCodecCfg;
}


//=============================================================================
// vpp_SpeechBTScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule one decoding and/or one encoding frame. Convert to Vpp codec mode. 
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_SpeechBTScheduleOneFrame(VPP_SPEECH_WAKEUP_MODE_T    wakeupMode)
{

    HAL_ERR_T status = HAL_ERR_NO;
	
    if (wakeupMode == VPP_SPEECH_WAKEUP_SW_DEC)
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_1);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

    if (wakeupMode == VPP_SPEECH_WAKEUP_SW_ENC)
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

	
/*
    if ((g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DECENC) || (g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DEC))
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_1);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

    if ((g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DECENC) || (g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_ENC))
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);        
    }
*/
    return status;
 
}




//=============================================================================
// vpp_SpeechBTGetRxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Rx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetRxPcmBuffer(VOID)
{
    //return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_DEC_OUT_STRUCT);

    return (HAL_SPEECH_PCM_BUF_T *)g_BTRxBuf;
}

//=============================================================================
// vpp_SpeechBTGetTxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetTxPcmBuffer(VOID)
{
//    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);

    return (HAL_SPEECH_PCM_BUF_T *)g_BTTxBuf;
}
//=============================================================================
// vpp_SpeechBTGetTxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetBluetoothRxPcmBuffer(VOID)
{
//    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);

    return (HAL_SPEECH_PCM_BUF_T *)g_Bluetooth_RxBuf;
}
//=============================================================================
// vpp_SpeechBTGetTxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetBluetoothTxPcmBuffer(VOID)
{
//    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);

    return (HAL_SPEECH_PCM_BUF_T *)g_Bluetooth_TxBuf;
}

//=============================================================================
// vpp_SpeechBTGetRxPcmBuffer_test function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Rx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetRxPcmBuffer_test(VOID)
{
//    return (HAL_SPEECH_PCM_BUF_T *)g_BTRxBuf;
    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_DEC_OUT_STRUCT);
}

//=============================================================================
// vpp_SpeechBTGetTxPcmBuffer_test function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechBTGetTxPcmBuffer_test(VOID)
{
//    return (HAL_SPEECH_PCM_BUF_T *)g_BTTxBuf;

    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);
}

//=============================================================================
// vpp_SpeechBTGetRxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Rx) coded buffer
//=============================================================================
PUBLIC HAL_SPEECH_DEC_IN_T * vpp_SpeechBTGetRxCodBuffer(VOID)
{
    return (HAL_SPEECH_DEC_IN_T *) hal_VocGetPointer(VPP_SPEECH_DEC_IN_STRUCT);
}

//=============================================================================
// vpp_SpeechBTGetTxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Tx) coded buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_ENC_OUT_T * vpp_SpeechBTGetTxCodBuffer(VOID)
{
    return (CONST HAL_SPEECH_ENC_OUT_T *) hal_VocGetPointer(VPP_SPEECH_ENC_OUT_STRUCT);
}



//=============================================================================
// vpp_SpeechBTSetOutAlgGain function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetOutAlgGain(INT32 alg)
{
    UINT32 algGain = vpp_SpeechBTOutAlgGainDb2Val(alg);
	UINT16 Aec_DigitalGain;

	switch (algGain)
	{
		case 0:			
			algGain=0x10;
			break;
		case 1:			
			algGain=0x16;           ///1.3
			break;
		case 2:			
			algGain=0x20;       ///1.3^2
			break;
		case 3:			
			algGain=0x2d;     ///1.3^3
			break;
		default:
            algGain=0x10;
			break;	
	}

    * ( UINT16 *) hal_VocGetPointer(GLOBAL_RECEIVER_DIGITAL_GAIN_ADDR)=algGain;
#ifdef USE_OLD_AEC_NS
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(32767*16))/algGain);

   	Aec_DigitalGain=((0xa*0x10*0x10)/algGain);
#else
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(30000*16))/algGain);

   	Aec_DigitalGain=0x10;
#endif
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_AEC_SPK_DIGITAL_GAIN_ADDR)=Aec_DigitalGain;
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_AEC_SPK_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(32767*16))/Aec_DigitalGain);
}

//=============================================================================
// vpp_SpeechBTSetAecEnableFlag function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetAecEnableFlag(INT8 AecEnbleFlag,INT8 AgcEnbleFlag)
{
    VPP_SPEECH_AEC_INIT_T *Aec_Init = hal_VocGetPointer(m_AECProcPara_ADDR);
    Aec_Init->AecEnbleFlag=AecEnbleFlag;
    Aec_Init->AgcEnbleFlag=AgcEnbleFlag;
}


//=============================================================================
// vpp_SpeechBTSetInAlgGain function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetInAlgGain(INT32 alg)
{
    UINT32 algGain = vpp_SpeechBTInAlgGainDb2Val(alg);
    if (algGain != 0)
	{
        algGain=algGain>>2;
        * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_GAIN_ADDR)=algGain;
#ifdef USE_OLD_AEC_NS
        * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(32767*16))/algGain);
#else
        * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(30000*16))/algGain);
#endif
	}
	else
	{
	    * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_GAIN_ADDR)=0;
	    * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR)=32767;
	}
}



//=============================================================================
// vpp_SpeechBTSetMorphVoice function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetMorphVoice(INT16 EnableFlag, INT16 MorphPara)
{
#ifdef USE_OLD_AEC_NS
    *((INT16 *)hal_VocGetPointer(MORPHVOICE_ENABLE_FLAG_ADDR)) =EnableFlag;
    *((INT16 *)hal_VocGetPointer(PITCH_SHIFT_ADDR)) =MorphPara;
#endif
}



//=============================================================================
// vpp_SpeechBTSetAecPara function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetAecPara(INT8 NoiseGainLimit,INT8 NoiseMin,INT8 NoiseGainLimitStep,INT8 AmpThr,INT8 StrongEchoFlag)
{
        
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

#if  !defined(SPEECH_NS_MIC_VOC_CLOSE)

	Aec_Init->NoiseGainLimit=127;
#else
	Aec_Init->NoiseGainLimit=NoiseGainLimit;
#endif
	Aec_Init->NoiseMin=NoiseMin;
	Aec_Init->NoiseGainLimitStep=NoiseGainLimitStep;
	Aec_Init->AmpThr=AmpThr;
#ifdef USE_OLD_AEC_NS
	Aec_Init->StrongEchoFlag =0;
#else
	Aec_Init->StrongEchoFlag =StrongEchoFlag;
#endif
	Aec_Init->AFFilterAmp=32;

	Aec_Init->PFCLThrDT =4;
	Aec_Init->PFCLChanlGain = 128;

	Aec_Init->PFCLDTStartTime = 16;

	Aec_Init->PFCLDTDThreshold = 115;
	Aec_Init->PFCLDTD2ndThreshold = 64;	

	Aec_Init->PFCLDTDThrRatio=71;	


   	if (StrongEchoFlag==0)
	{
	      *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_SPEAKER_GAIN_ADDR)) =1;
	     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_LEAK_ESTIMATE_ADDR)) =8192;
		
	}
	else
	{
#ifdef USE_OLD_AEC_NS	
	     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_SPEAKER_GAIN_ADDR)) =8;
	     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_LEAK_ESTIMATE_ADDR)) =32767;
#else
	     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_SPEAKER_GAIN_ADDR)) =16;
	     *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_NOISE_SUPPRESS_LEAK_ESTIMATE_ADDR)) =32767;
#endif
/*		Aec_Init->AFFilterAmp=128;

		Aec_Init->PFCLThrDT =64;
		Aec_Init->PFCLChanlGain = 128;

		Aec_Init->PFCLDTStartTime = 80;

		Aec_Init->PFCLDTDThreshold = 102;
		Aec_Init->PFCLDTD2ndThreshold = 64;		
		Aec_Init->PFCLDTDThrRatio=80;*/

	} 
 
	 return;
    
}

PUBLIC VOID vpp_SpeechBTSetAecAllPara(CALIB_AUDIO_VOC_AEC_T *AecParam)
{
#ifndef USE_OLD_AEC_NS
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

	Aec_Init->AecEnbleFlag = AecParam->AecEnableFlag;

	Aec_Init->AgcEnbleFlag=AecParam->AgcEnableFlag;

	Aec_Init->ProcessMode=AecParam->ProcessMode;
	Aec_Init->InitRegularFactor=AecParam->InitRegularFactor;
	Aec_Init->AFUpdateFactor=AecParam->AFUpdateFactor;              //new:4 old:8

	Aec_Init->AFCtrlSpeedUpFactor=AecParam->AFCtrlSpeedUpFactor;        //new:10 old:32
	Aec_Init->AFFilterAmp=AecParam->AFFilterAmp;


	Aec_Init->EchoEstMethod=AecParam->EchoEstMethod;

	Aec_Init->NoiseUpdateFactor=AecParam->NoiseUpdateFactor;
	Aec_Init->SNRUpdateFactor=AecParam->SNRUpdateFactor;
	Aec_Init->SNRLimit=AecParam->SNRLimit;
	Aec_Init->NoiseOverEstFactor=AecParam->NoiseOverEstFactor;

	Aec_Init->NoiseGainLimit=AecParam->NoiseGainLimit;
	Aec_Init->EchoUpdateFactor=AecParam->EchoUpdateFactor;
	Aec_Init->SERUpdateFactor=AecParam->SERUpdateFactor;
	Aec_Init->SERLimit=AecParam->SERLimit;

	Aec_Init->EchoOverEstFactor=AecParam->EchoOverEstFactor;
	Aec_Init->EchoGainLimit=AecParam->EchoGainLimit;         //new:51 old:255
	Aec_Init->CNGFactor=AecParam->CNGFactor;
	Aec_Init->NoiseMin=AecParam->NoiseMin;

	Aec_Init->PFPrevEchoEstLimit=AecParam->PFPrevEchoEstLimit;

	Aec_Init->PFDTEchoOverEstFactor=AecParam->PFDTEchoOverEstFactor;

	Aec_Init->pPFEchoGainLimitAmp[0]=AecParam->pPFEchoGainLimitAmp[0];
	Aec_Init->pPFEchoGainLimitAmp[1]=AecParam->pPFEchoGainLimitAmp[1];
	Aec_Init->pPFEchoGainLimitAmp[2]=AecParam->pPFEchoGainLimitAmp[2];
	Aec_Init->pPFEchoGainLimitAmp[3]=AecParam->pPFEchoGainLimitAmp[3];
	Aec_Init->pPFEchoGainLimitAmp[4]=AecParam->pPFEchoGainLimitAmp[4];
	Aec_Init->pPFEchoGainLimitAmp[5]=AecParam->pPFEchoGainLimitAmp[5];
	Aec_Init->pPFEchoGainLimitAmp[6]=AecParam->pPFEchoGainLimitAmp[6];
	Aec_Init->pPFEchoGainLimitAmp[7]=AecParam->pPFEchoGainLimitAmp[7];
	Aec_Init->pPFEchoGainLimitAmp[8]=AecParam->pPFEchoGainLimitAmp[8];

	

	Aec_Init->AmpGain=AecParam->AmpGain;

	Aec_Init->NoiseGainLimitStep=AecParam->NoiseGainLimitStep;
	Aec_Init->AmpThr=AecParam->AmpThr;

	Aec_Init->PFCLDataSmoothFactor=AecParam->PFCLDataSmoothFactor;
	Aec_Init->PFCLNoiseUpdateFactor=AecParam->PFCLNoiseUpdateFactor;
	Aec_Init->PFCLThrSigDet=AecParam->PFCLThrSigDet;
	Aec_Init->PFCLThrDT=AecParam->PFCLThrDT;

	Aec_Init->DataSmoothFactor=AecParam->DataSmoothFactor;
	Aec_Init->PFCLChanlGain=AecParam->PFCLChanlGain;

	Aec_Init->PFCLThrNT=AecParam->PFCLThrNT;
	Aec_Init->PFCLThrDTDiff=AecParam->PFCLThrDTDiff;
	Aec_Init->PFCLDTHoldTime=AecParam->PFCLDTHoldTime;
	Aec_Init->PFCLDTStartTime=AecParam->PFCLDTStartTime;



	Aec_Init->PFCLDTDUpdateFactor=AecParam->PFCLDTDUpdateFactor;  //new:16 old:2
	Aec_Init->PFCLDTDThreshold=AecParam->PFCLDTDThreshold;
	Aec_Init->PFCLDTD2ndThreshold=AecParam->PFCLDTD2ndThreshold;
	Aec_Init->StrongEchoFlag=AecParam->StrongEchoFlag;
	Aec_Init->PFCLDTDThrRatio=AecParam->PFCLDTDThrRatio;

	/* below is not in struct VPP_SPEECH_AEC_INIT_T */
	VPP_SPEECH_AEC_EXTRA_T *AecExtra_Init = hal_VocGetPointer(m_AECExtraPara_ADDR);
	AecExtra_Init->DelaySampleNum = AecParam->DelaySampleNum;
	
	AecExtra_Init->FrameCntLimitNum[0] = AecParam->FrameCntLimitNum[0];
	AecExtra_Init->FrameCntLimitNum[1] = AecParam->FrameCntLimitNum[1];
	AecExtra_Init->FrameCntLimitNum[2] = AecParam->FrameCntLimitNum[2];
	
	AecExtra_Init->NoiseCntLimitNum[0] = AecParam->NoiseCntLimitNum[0];
	AecExtra_Init->NoiseCntLimitNum[1] = AecParam->NoiseCntLimitNum[1];
	AecExtra_Init->NoiseCntLimitNum[2] = AecParam->NoiseCntLimitNum[2];
	AecExtra_Init->NoiseCntLimitNum[3] = AecParam->NoiseCntLimitNum[3];

	AecExtra_Init->NoiseWeightSNRLimitMax = AecParam->NoiseWeightSNRLimitMax;
	AecExtra_Init->NoiseWeightSNRLimitMin = AecParam->NoiseWeightSNRLimitMin;
	AecExtra_Init->NoiseWeightSNRTh1 = AecParam->NoiseWeightSNRTh1;
	AecExtra_Init->NoiseWeightSNRTh2 = AecParam->NoiseWeightSNRTh2;
	AecExtra_Init->NoiseWeightSNRLimitStep = AecParam->NoiseWeightSNRLimitStep;
	AecExtra_Init->NoiseWeightSub1DecayFactor = AecParam->NoiseWeightSub1DecayFactor;

	AecExtra_Init->ResidualEchoEnable = AecParam->ResidualEchoEnable;
	AecExtra_Init->AGCRrrSpkRelation = AecParam->AGCRrrSpkRelation;
	AecExtra_Init->AddAttDB = AecParam->AddAttDB;
	AecExtra_Init->MinAttDB = AecParam->MinAttDB;
	AecExtra_Init->MaxAttDB = AecParam->MaxAttDB;
	AecExtra_Init->NPowRiseFactor = AecParam->NPowRiseFactor;
	AecExtra_Init->WorkFactFactor = AecParam->WorkFactFactor;
	AecExtra_Init->AttExtFactor = AecParam->AttExtFactor;
	AecExtra_Init->AttRiseFactor = AecParam->AttRiseFactor;
	AecExtra_Init->AttDecayFactor = AecParam->AttDecayFactor;
	AecExtra_Init->Log2LinFactor = AecParam->Log2LinFactor;
#endif
	return;
}

//=============================================================================
// vpp_SpeechSetFilterPara function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetFilterPara(INT8 HighPassFilterFlag,INT8 NotchFilterFlag,INT8 MicNoiseSuppresserFlag,INT8 ReceiverNoiseSuppresserFlag)
{
        
	*((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) =HighPassFilterFlag;
	*((INT16 *)hal_VocGetPointer(NOTCH_FILTER_ENABLE_FLAG_ADDR)) =NotchFilterFlag;

#if  !defined(SPEECH_NS_MIC_VOC_CLOSE)
	
	*((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =MicNoiseSuppresserFlag;
#else

	*((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =FALSE;
#endif

#if  !defined(SPEECH_NS_RECEIVER_VOC_CLOSE)
	*((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =ReceiverNoiseSuppresserFlag;
#else

	*((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =FALSE;
#endif
	
	 return;
    
}

//=============================================================================
// vpp_SpeechSetMorphVoice function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetReocrdSpeech(INT16 EnableFlag)
{
#ifdef USE_OLD_AEC_NS        
     *((INT16 *)hal_VocGetPointer(GLOBAL_SPEECH_RECORD_FLAG)) =EnableFlag;
#endif
}


//=============================================================================
// vpp_SpeechGetType function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC INT16 vpp_SpeechBTGetType(VOID)
{
        
   return  *((INT16 *)hal_VocGetPointer(GLOBAL_EXTERN_PROCESS_MIC_HANDLE_FLAG));

}

//=============================================================================
// vpp_SpeechSetMorphVoice function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetProcessMicEnableFlag(INT16 EnableFlag )
{

     *((INT16 *)hal_VocGetPointer(GLOBAL_EXTERN_PROCESS_MIC_ENABLE_FLAG)) =EnableFlag;

   return ;

}

PUBLIC INT16 vpp_SpeechBTGetProcessMicEnableFlag(VOID)
{
    return *((INT16 *)hal_VocGetPointer(GLOBAL_EXTERN_PROCESS_MIC_ENABLE_FLAG));
}

/*
PUBLIC VOID vpp_SpeechBTSetNoiseSuppressParaTX(INT16 NoiseSuppressDefault, INT16 MinRange1, INT16 MinRange2,
					   INT16 MinRange3, INT16 MinRange4)
{
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_SUPPRESS_DEFAULT_TX)) = NoiseSuppressDefault;

    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE1)) = MinRange1;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE2)) = MinRange2;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE3)) = MinRange3;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE4)) = MinRange4;

    return;
}

PUBLIC VOID vpp_SpeechBTSetNoiseSuppressParaRX(INT16 NoiseSuppressDefault, INT16 MinRange1, INT16 MinRange2,
					   INT16 MinRange3, INT16 MinRange4)
{
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_SUPPRESS_DEFAULT_RX)) = NoiseSuppressDefault;

    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE1)) = MinRange1;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE2)) = MinRange2;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE3)) = MinRange3;
    *((UINT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE4)) = MinRange4;

    return;
}

PUBLIC VOID vpp_SpeechBTSetNonClipParaTX(INT16 SmoothThreshold, INT16 NoiseThreshold)
{
    *((UINT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_SMOOTH_THRESHOLD_TX)) = SmoothThreshold;
    *((UINT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_NOISE_THRESHOLD_TX)) = NoiseThreshold;

    return;
}

PUBLIC VOID vpp_SpeechBTSetNonClipParaRX(INT16 SmoothThreshold, INT16 NoiseThreshold)
{
    *((UINT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_SMOOTH_THRESHOLD_RX)) = SmoothThreshold;
    *((UINT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_NOISE_THRESHOLD_RX)) = NoiseThreshold;

    return;
}
*/

PUBLIC VOID vpp_SpeechBTSetHpfPara(CALIB_AUDIO_IIR_HPF_T *hpf)
{
    *((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) = hpf->flag;
#ifndef USE_OLD_AEC_NS
	*((INT16 *)hal_VocGetPointer(GLOBAL_HIGH_PASS_FILTER3_NUM1_ADDR)) = hpf->band.num[1];
	*((INT16 *)hal_VocGetPointer(GLOBAL_HIGH_PASS_FILTER3_NUM2_ADDR)) = hpf->band.num[2];
	*((INT16 *)hal_VocGetPointer(GLOBAL_HIGH_PASS_FILTER3_DEN1_ADDR)) = hpf->band.den[1];
	*((INT16 *)hal_VocGetPointer(GLOBAL_HIGH_PASS_FILTER3_DEN2_ADDR)) = hpf->band.den[2];
#endif
}

PUBLIC VOID vpp_SpeechBTSetFIRPara(VPP_SPEECH_AUDIO_CFG_T *pAudioCfg, CALIB_AUDIO_VOC_FILTERS_T *fir)
{
    memcpy(pAudioCfg->sdf, fir->sdfFilter, sizeof(UINT16) * CALIB_VOC_SDF_QTY);
    memcpy(pAudioCfg->mdf, fir->mdfFilter, sizeof(UINT16) * CALIB_VOC_MDF_QTY);
     
    vpp_SpeechBTAudioCfg(pAudioCfg);

    return;
}

PUBLIC VOID vpp_SpeechBTSetNoiseSuppressParaTX(CALIB_AUDIO_VOC_SPEEX_PATH_T *tx)
{
    *((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) = tx->flag;
#ifndef USE_OLD_AEC_NS
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_SUPPRESS_DEFAULT_TX)) = tx->noise_suppress_default;

    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE1)) = tx->noise_probe_range[0];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE2)) = tx->noise_probe_range[1];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE3)) = tx->noise_probe_range[2];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_TX_RANGE4)) = tx->noise_probe_range[3];
#endif
    return;
}

PUBLIC VOID vpp_SpeechBTSetNoiseSuppressParaRX(CALIB_AUDIO_VOC_SPEEX_PATH_T *rx)
{
    *((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) = rx->flag;
#ifndef USE_OLD_AEC_NS
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_SUPPRESS_DEFAULT_RX)) = rx->noise_suppress_default;

    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE1)) = rx->noise_probe_range[0];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE2)) = rx->noise_probe_range[1];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE3)) = rx->noise_probe_range[2];
    *((INT16 *)hal_VocGetPointer(SPEEX_NOISE_PROB_MIN_RX_RANGE4)) = rx->noise_probe_range[3];
#endif
    return;
}

PUBLIC VOID vpp_SpeechBTSetNoiseSuppressParaAll(CALIB_AUDIO_VOC_SPEEX_T *speex)
{
    vpp_SpeechBTSetNoiseSuppressParaTX(&speex->tx);
    vpp_SpeechBTSetNoiseSuppressParaRX(&speex->rx);

    return;
}

PUBLIC VOID vpp_SpeechBTSetNonClipParaTX(CALIB_AUDIO_VOC_AGC_PATH_T *tx)
{
#ifndef USE_OLD_AEC_NS
    *((INT16 *)hal_VocGetPointer(MIC_NONCLIP_ENABLE_FLAG_ADDR)) = tx->flag;
    *((INT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_SMOOTH_THRESHOLD_TX)) = tx->smooth_threshold;
    *((INT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_NOISE_THRESHOLD_TX)) = tx->noise_threshold;
	vpp_SpeechBTSetInAlgGain(tx->gain);
#endif
    return;
}

PUBLIC VOID vpp_SpeechBTSetNonClipParaRX(CALIB_AUDIO_VOC_AGC_PATH_T *rx)
{
#ifndef USE_OLD_AEC_NS
    *((INT16 *)hal_VocGetPointer(RECEIVER_NONCLIP_ENABLE_FLAG_ADDR)) = rx->flag;
    *((INT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_SMOOTH_THRESHOLD_RX)) = rx->smooth_threshold;
    *((INT16 *)hal_VocGetPointer(GLOBAL_NON_CLIP_NOISE_THRESHOLD_RX)) = rx->noise_threshold;
	vpp_SpeechBTSetOutAlgGain(rx->gain);
#endif
    return;
}

PUBLIC VOID vpp_SpeechBTSetNonClipParaAll(CALIB_AUDIO_VOC_AGC_T *agc)
{
    vpp_SpeechBTSetNonClipParaTX(&agc->tx);
    vpp_SpeechBTSetNonClipParaRX(&agc->rx);

    return;
}
static bool reCalcBtCallEQ = 0;
PUBLIC void vpp_BtCallSetAudioParamReloadFlag(bool value)
{
	reCalcBtCallEQ = value;
}

PUBLIC bool vpp_BtCallGetAudioParamReloadFlag(void)
{
	return reCalcBtCallEQ;
}
PUBLIC VOID vpp_SpeechBTSetEQPara(CALIB_AUDIO_CALL_IIR_EQ_T *eq)
{
#ifndef USE_OLD_AEC_NS
	int i;
	INT16 *IIR_Init = (INT16 *)hal_VocGetPointer(m_IIRProcPara_ADDR);
    sIIRcoefficients *iir_coef = get_coeffs_point();

	*((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = eq->flag;
	
    hal_HstSendEvent(SYS_EVENT,0x88b00000); 
    hal_HstSendEvent(SYS_EVENT,eq->flag); 
    if(eq->flag)
    {
        calc_coeffs_for_call_voc((CALIB_AUDIO_IIR_EQ_T *)eq, 8000, 0);
    }
	// each band has 6 coeffs(num[3], den[3]) call use q14 music use q19.
	for (i = 0; i < CALIB_AUDIO_CALL_IIR_BANDS; i++)
	{
		IIR_Init[0] = (INT16)(iir_coef->num[0]>>5);
		IIR_Init[1] = (INT16)(iir_coef->num[1]>>5);
		IIR_Init[2] = (INT16)(iir_coef->num[2]>>5);
		IIR_Init[3] = (INT16)(iir_coef->den[0]>>5);
		IIR_Init[4] = (INT16)(iir_coef->den[1]>>5);
		IIR_Init[5] = (INT16)(iir_coef->den[2]>>5);

#if 0
		hal_HstSendEvent(SYS_EVENT,0x88b00001); 
		hal_HstSendEvent(SYS_EVENT,i); 
		
		hal_HstSendEvent(SYS_EVENT,iir_coef->num[0]); 
		hal_HstSendEvent(SYS_EVENT,iir_coef->num[1]); 
		hal_HstSendEvent(SYS_EVENT,iir_coef->num[2]); 

		hal_HstSendEvent(SYS_EVENT,iir_coef->den[3]); 
		hal_HstSendEvent(SYS_EVENT,iir_coef->den[4]); 
		hal_HstSendEvent(SYS_EVENT,iir_coef->den[5]); 
		
		hal_HstSendEvent(SYS_EVENT,IIR_Init[0]); 
		hal_HstSendEvent(SYS_EVENT,IIR_Init[1]); 
		hal_HstSendEvent(SYS_EVENT,IIR_Init[2]); 

		hal_HstSendEvent(SYS_EVENT,IIR_Init[3]); 
		hal_HstSendEvent(SYS_EVENT,IIR_Init[4]); 
		hal_HstSendEvent(SYS_EVENT,IIR_Init[5]); 
#endif
		IIR_Init += 6;
        iir_coef += 1;
	}
#endif
    return;
}
#ifdef CHIP_HAS_AP
// calc as power(10, dB/20) * 16384
static INT16 db_to_multiple_table[8] = {
	1024, // 0dB
	724, // -3dB
	513, // -6dB
	363, // -9dB
	257, // -12dB
	182, // -15dB
	128, // -18dB
	0, // mute
};
#endif

PUBLIC VOID vpp_SpeechBTSetScaleDigitalGainPara(CALIB_AUDIO_VOC_DigGain_T *scale, SND_SPK_LEVEL_T outLevel)
{
#ifdef USE_OLD_AEC_NS
    gMicGain = scale->scal_in >> 10;
#else
#ifdef CHIP_HAS_AP
	INT16 multiply = db_to_multiple_table[7 - outLevel];
#else
	INT16 multiply = aud_GetOutAlgGainDb2Val();
#endif

    *((INT16 *)hal_VocGetPointer(SCALE_MIC_ENABLE_FLAG_ADDR)) = scale->in_enable;
    *((INT16 *)hal_VocGetPointer(SCALE_RECEIVER_ENABLE_FLAG_ADDR)) = scale->out_enable;

    *((INT16 *)hal_VocGetPointer(GLOBAL_MIC_SCALE_IN_ADDR)) = scale->scal_in;
    *((INT16 *)hal_VocGetPointer(GLOBAL_RECEIVER_SCALE_OUT_ADDR)) = ((INT32)scale->scal_out * multiply) >> 14;//multiply is Q14.
#endif
    return;
}

// TODO: AGC functions should be rewrite when these params are added in calibration sector
typedef struct
{
	INT16 enable;
	INT16 target_level_dbfs;
	INT16 compression_gain_dB;
	INT16 limiter_enable;
	INT16 upper_thr;
	INT16 lower_thr;
	INT16 decay_value;
	INT16 capacitor_slow_factor;
	INT16 capacitor_fast_factor;
	INT16 noise_compress_factor;
	INT16 noise_gate_thr;
	INT16 noise_std_short_term_factor;
} WebRtcAgc;

PUBLIC VOID vpp_SpeechBTSetWebRtcAgcParaTx(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *tx)
{
	WebRtcAgc *config = (WebRtcAgc *)hal_VocGetPointer(STRUCT_MicAgcConfig_addr);

	config->enable = tx->enable;
	config->target_level_dbfs = tx->target_level_dbfs;
	config->compression_gain_dB = tx->compression_gain_dB;
	config->limiter_enable = tx->limiter_enable;
	config->upper_thr = tx->upper_thr;
	config->lower_thr = tx->lower_thr;
	config->decay_value = tx->decay_value;
	config->capacitor_slow_factor = tx->capacitor_slow_factor;
	config->capacitor_fast_factor = tx->capacitor_fast_factor;
	config->noise_compress_factor = tx->noise_compress_factor;
	config->noise_gate_thr = tx->noise_gate_thr;
	config->noise_std_short_term_factor = tx->noise_std_short_term_factor;
}

PUBLIC VOID vpp_SpeechBTSetWebRtcAgcParaRx(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *rx)
{
	WebRtcAgc *config = (WebRtcAgc *)hal_VocGetPointer(STRUCT_ReceiverAgcConfig_addr);

	config->enable = rx->enable;
	config->target_level_dbfs = rx->target_level_dbfs;
	config->compression_gain_dB = rx->compression_gain_dB;
	config->limiter_enable = rx->limiter_enable;
	config->upper_thr = rx->upper_thr;
	config->lower_thr = rx->lower_thr;
	config->decay_value = rx->decay_value;
	config->capacitor_slow_factor = rx->capacitor_slow_factor;
	config->capacitor_fast_factor = rx->capacitor_fast_factor;
	config->noise_compress_factor = rx->noise_compress_factor;
	config->noise_gate_thr = rx->noise_gate_thr;
	config->noise_std_short_term_factor = rx->noise_std_short_term_factor;
}

PUBLIC VOID vpp_SpeechBTSetWebRtcAgcParaAll(CALIB_AUDIO_VOC_WEBRTC_AGC_T *webrtc_agc)
{
	vpp_SpeechBTSetWebRtcAgcParaTx(&(webrtc_agc->tx));
	vpp_SpeechBTSetWebRtcAgcParaRx(&(webrtc_agc->rx));
}

PUBLIC VOID vpp_SpeechBTSetWebRtcAgcReloadFlag(INT16 tx, INT16 rx)
{
	*((INT16 *)hal_VocGetPointer(GLOBAL_MICAGC_RELOAD)) = tx;
	*((INT16 *)hal_VocGetPointer(GLOBAL_RECEIVERAGC_RELOAD)) = rx;
}

PUBLIC VOID vpp_SpeechBTMSDFReload(VOID)
{
#ifndef USE_OLD_AEC_NS
	*((INT16 *)hal_VocGetPointer(GLOBAL_MSDF_RELOAD_FLAG)) = 1;
#endif
	return;
}

PUBLIC VOID vpp_SpeechBTSetMSDFPara(CALIB_AUDIO_VOC_FILTERS_T *fir)
{
	*((INT16 *)hal_VocGetPointer(GLOBAL_MICFIR_ENABLE_FLAG_ADDR)) = fir->mdfflag;
	*((INT16 *)hal_VocGetPointer(GLOBAL_RECEIVERFIR_ENABLE_FLAG_ADDR)) = fir->sdfflag;
}

PUBLIC UINT32 vpp_SpeechBTInAlgGainDb2Val(INT32 db)
{
    CONST UINT32 micAlgVal[] =
    {
        0x40, 0x5a, 0x80, 0xb5,
        0x100, 0x16a, 0x200, 0x2d4,
        0x400, 0x5a8, 0x800, 0xb50,
        0x1000, 0x16a0, 0x2000, 0x2d40,
    };
    if (db == CALIB_AUDIO_GAIN_VALUE_MUTE)
        return 0;
    else if (db < 0)
        db = 0;
    else if (db > 45)
        db = 45;
    return micAlgVal[db/3];
}


PUBLIC UINT32 vpp_SpeechBTOutAlgGainDb2Val(INT32 db)
{
    if (db < 0)
        db = 0;
    else if (db > 9)
        db = 9;
    return db/3;
}

//=============================================================================
// vpp_SpeechAecSetDelayNum function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC VOID vpp_SpeechBTAecSetDelayNum(INT16 delaySampleNum)
{
#ifndef USE_OLD_AEC_NS
	VPP_SPEECH_AEC_EXTRA_T *AecExtra_Init = hal_VocGetPointer(m_AECExtraPara_ADDR);
	AecExtra_Init->DelaySampleNum = delaySampleNum;
#endif
	return;
}


//=============================================================================
// vpp_SpeechSetEncDecPocessingParams
//
//=============================================================================
PUBLIC VOID vpp_SpeechBTSetEncDecPocessingParams(INT16 itf, INT16 level, INT16 enabled)
{
	//CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
#ifndef USE_OLD_AEC_NS
    vpp_SpeechBTSetNoiseSuppressParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].speex));

    vpp_SpeechBTSetNonClipParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].agc));

    vpp_SpeechBTSetAecAllPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].aec));

	vpp_SpeechBTSetScaleDigitalGainPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].gain), level);

	vpp_SpeechBTSetHpfPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].hpf));

	vpp_SpeechBTSetEQPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].eq));

	vpp_SpeechBTSetWebRtcAgcParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].webrtcagc));

    //vpp_SpeechMSDFReload();

	// flag is true, we set audio effects params according to calibration,
	// else we disable all effects
	if (enabled == 0)
	{
		// disable ns
		*((INT16 *)hal_VocGetPointer(MIC_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) = 0;
		*((INT16 *)hal_VocGetPointer(RECEIVER_NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) = 0;

		// disable nonclip
		*((INT16 *)hal_VocGetPointer(MIC_NONCLIP_ENABLE_FLAG_ADDR)) = 0;
		*((INT16 *)hal_VocGetPointer(RECEIVER_NONCLIP_ENABLE_FLAG_ADDR)) = 0;

		// disable aec
		VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);
		Aec_Init->AecEnbleFlag = 0;

		// disable hpf
		*((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) = 0;

		// disable eq
		*((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = 0;
		
		// disable agc
		WebRtcAgc *AgcConfig = (WebRtcAgc *)hal_VocGetPointer(STRUCT_MicAgcConfig_addr);
		AgcConfig->enable = 0;

		AgcConfig = (WebRtcAgc *)hal_VocGetPointer(STRUCT_ReceiverAgcConfig_addr);
		AgcConfig->enable = 0;

		vpp_SpeechBTSetWebRtcAgcReloadFlag(0, 0);
	}
#endif
}

#if 1
//=============================================================================
// vpp_AudioJpegDecScheduleOneFrame function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_SpeechScheduleOneFrame()
{ 
    if(gMicGain<=1)
	{
        gMicGain = 1;
	}
#ifdef USE_OLD_AEC_NS
	*((INT16 *)hal_VocGetPointer(NS_MIC_GAIN_ADDR)) = gMicGain;
#endif
    return hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
}
#endif
