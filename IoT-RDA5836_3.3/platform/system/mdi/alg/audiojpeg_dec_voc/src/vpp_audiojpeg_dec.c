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
/*      vpp.c                                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file contains the VPP API  functions.                        */
/*                                                                       */
/*************************************************************************/
//#include "stdio.h"

#include "cs_types.h"

#include "hal_error.h"
#include "hal_voc.h"
#include "hal_host.h"

#include "vpp_audiojpeg_dec_asm.h"
#include "mcip_debug.h"
#include "vpp_audiojpeg_dec.h"

#include "tgt_calib_m.h"
#include "tgt_aud_cfg.h"

#include "aud_m.h"
#include "audp_debug.h"

//#include "audio_voc_map.h"

#if (AUDIO_CALIB_VER == 1)
#include "analog_audio_iir.h"
#endif

//#define SUPPORT_MUSIC_DRC

CONST INT16 f_sbc_persition[30]= 
{
	2, 2, 2, 4, 4, 
	4, 4, 4, 4, 8, 8, 8, 8, 
	8, 8, 16, 16, 16, 16, 16, 32, 
	32, 32, 32, 32, 64, 64, 64, 63,
	0
};

BOOL vpp_AudioJpegDecVocOpen_status=FALSE;

//   extern AUD_LEVEL_T audio_cfg;
//   extern HAL_AIF_STREAM_T audioStream;
   extern AUD_ITF_T audioItf;

#if (AUDIO_CALIB_VER == 1)
static bool reCalcMp3EQ = 0;
extern SND_ITF_T musicItf;
//static AUD_ITF_T lastAudioItf = AUD_ITF_NONE;
static bool audecResetDrcFlag = 0;
static UINT8 eq_active_num = 0;
#endif

#ifdef KALAOK_SUPPORT
UINT16 kalaok_support_flag = 1;
#else
UINT16 kalaok_support_flag = 0;
#endif

#ifdef MUSIC_BAND_INFO
void vpp_Mp3SetBandInfoEnable(uint16 flag)
{
	*((UINT16 *)hal_VocGetPointer(MP3DEC_BAND_INFO_ENABLE_FLAG_ADDR)) = flag;
}

void vpp_Mp3GetBandInfo(UINT16 * band_buf)
{
    vpp_AudioJpeg_DEC_IN_T * pDecIn;
    vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus;
		
    UINT32 band[8];
    UINT16 shift, i;
		
    UINT32 *pBandVoCInfo;
		
    pDecIn = hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
    pVoc_AudioDecStatus = (vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT);


	hal_HstSendEvent(SYS_EVENT,0x20170303); 
	hal_HstSendEvent(SYS_EVENT,pDecIn->mode);
	hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->nbChannel);

	switch(pDecIn->mode)
	{
	    case 9 :                //MMC_AAC_DECODE
	        pBandVoCInfo = (UINT32 *)hal_VocGetPointer(m_AACDEC_BAND_INFO_ADDR);
		 *(INT16 *)hal_VocGetPointer(AACDEC_BAND_INFO_ENABLE_FLAG_ADDR) = 1;
		 shift = 1;
	        break;

	    case 1 :               //MMC_MP3_DECODE
	        pBandVoCInfo = (UINT32 *)hal_VocGetPointer(m_MP3DEC_BAND_INFO_ADDR);
		 *(INT16 *)hal_VocGetPointer(MP3DEC_BAND_INFO_ENABLE_FLAG_ADDR) = 1;
		 shift = 0;
	        break;

	    default:
	        pBandVoCInfo = NULL;
	        MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_MP3AAC_BANDINFO_ERROR]Error Mode!");
	        break;
	}

	if(pBandVoCInfo == NULL)
	{
	    return;
	}

	if(pVoc_AudioDecStatus->nbChannel == 2)
	{
	    shift += 1; 
	}

	 for(i=0; i<8;i++)
	{
		band[i] = *(pBandVoCInfo+i);

		band[i] >>= shift;
		band_buf[i] = (UINT16) band[i];

	}

}
#endif

//=============================================================================
// vpp_AudioJpegDecOpen function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_AudioJpegDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) 
{
    HAL_VOC_CFG_T cfg;
    vpp_AudioJpeg_DEC_IN_T * pDecIn;
    vpp_AudioJpeg_DEC_OUT_T *pDecStatus;
    INT32 * *pDMA;
//    INT16 *pFilter;

//    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
	
   diag_printf("[vpp_AudioJpeg_DEC]Opening VPP AUDIO_DEC\n"); 

//#ifdef AUDIO_CALIB_OLD
//    if(calibPtr->aud.vocItfMap[audioItf]>=0)
//        pFilter = &(calibPtr->aud.vocFilters[calibPtr->aud.vocItfMap[audioItf]].sdfFilter[0]);
//    else
//    {
//        pFilter = (INT16*)mmc_MemMalloc(sizeof(CALIB_AUDIO_VOC_FILTERS_T));
//        memset(pFilter, 0, sizeof(CALIB_AUDIO_VOC_FILTERS_T));
//    } 
//#endif
    cfg.vocCode              = G_VppCommonDecCode;
    cfg.vocCodeSize        = vpp_AudioJpeg_DEC_CODE_SIZE;
    cfg.pcVal                   = vpp_AudioJpeg_DEC_CODE_ENTRY;
    cfg.pcValCriticalSecMin  = vpp_AudioJpeg_DEC_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = vpp_AudioJpeg_DEC_CRITICAL_SECTION_MAX;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc          = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc       = 0;
    cfg.vocIrqHandler        = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 0;
    cfg.eventMask.wakeupSof0 = 0;
    cfg.eventMask.wakeupSof1 = 0;
    
    // load the VPP AUDIO_DEC  code and configure the VoC resource        
    hal_HstSendEvent(SYS_EVENT,0x201750c0);
    switch (hal_VocOpen(&cfg))
    {
        // error opening the resource
        case HAL_ERR_RESOURCE_BUSY:                 
            diag_printf("[vpp_AudioJpeg_DEC]##WARNING##Error opening VoC resource\n"); 
            return HAL_ERR_RESOURCE_BUSY;
                                        
        // first open, load the constant tables                            
        case HAL_ERR_RESOURCE_RESET:              
            diag_printf("[vpp_AudioJpeg_DEC]First open.\n");			
            break;

        default:           
            diag_printf("[vpp_AudioJpeg_DEC]No first open.\n"); 
            break;
    }                          

   diag_printf("[vpp_AudioJpeg_DEC]Initializing the DMA addr.\n");
      //mp3
   	pDMA = hal_VocGetPointer(VPP_MP3_Code_ExternalAddr_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
   *((INT32*)hal_VocGetPointer(AUDIO_MIXING_MP3_ADDR))=FALSE;
	//aac
	pDMA = hal_VocGetPointer(VPP_AAC_Code_ExternalAddr_addr);

#ifndef TARGET_AAC_SUPPORT_OFF
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3Layer12Dec_DMA_ConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);	
#ifndef TARGET_AAC_SUPPORT_OFF
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif
	//amr
	pDMA = hal_VocGetPointer(VPP_AMR_Code_ExternalAddr_addr);

	//*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAmrDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3Dec_EQ_DMA_ConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	//pDMA=pDMA+1;
	//*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAmrDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

#ifndef TARGET_AAC_SUPPORT_OFF
    pDMA= hal_VocGetPointer(VPP_AAC_Tab_huffTabSpec_START_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecHuffTabSpec,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecCos4sin4tab,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecTwidTabOdd,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecSinWindow,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif

 //*((INT16*)hal_VocGetPointer( VPP_GLOBAL_NON_CLIP_BUFFER_MP3_CLEAR_FLAG))=1;

#ifdef VIDEO_PLAYER_SUPPORT
	//jpeg
	pDMA = hal_VocGetPointer(VPP_JPEG_Code_ExternalAddr_addr);

	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJpegDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJpegDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);


	
	pDMA = hal_VocGetPointer(VPP_AAC_Tab_huffTabSpec_START_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecHuffTabSpec,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecCos4sin4tab,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecTwidTabOdd,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecSinWindow,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

      //h263 zoom
	pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	 pDMA=pDMA+1;
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif

#ifdef bt_support
       //sbc encode
	pDMA = hal_VocGetPointer(VPP_SBC_ENC_Code_ExternalAddr_addr);
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppSBCCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif

	//pDMA = hal_VocGetPointer(VPP_Speak_AntiDistortion_Filter_Coef_addr);
   //  *pDMA=hal_VocGetDmaiPointer((INT32*)(&(calibPtr->bb->audio[4].vocFilters.sdfFilter[0])),  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
   // *pDMA=hal_VocGetDmaiPointer((INT32*)pFilter,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     
	//pDMA = hal_VocGetPointer(VPP_GLOBAL_NON_CLIP_HISTORYDATA_L_MP3_ADDR);

#ifdef SMALL_BSS_RAM_SIZE

	//G_Mp3NonCliPLBuf_alloc                     = (UINT8*)mmc_MemMalloc_BssRam(G_Mp3NonCliPLBuf_Len+3);
	//G_Mp3NonCliPRBuf_alloc                     = (UINT8*)mmc_MemMalloc_BssRam(G_Mp3NonCliPRBuf_Len+3);
	//G_Mp3NonCliPLBuf =(UINT32*) ((UINT32)(G_Mp3NonCliPLBuf_alloc +3) & ~0x3);
	//G_Mp3NonCliPRBuf =(UINT32*) ((UINT32)(G_Mp3NonCliPRBuf_alloc+3) & ~0x3);

	//hal_HstSendEvent(SYS_EVENT,0xaccccccc);
	//hal_HstSendEvent(SYS_EVENT,G_Mp3NonCliPLBuf_alloc);
	//hal_HstSendEvent(SYS_EVENT,G_Mp3NonCliPLBuf);
	//hal_HstSendEvent(SYS_EVENT,G_Mp3NonCliPRBuf_alloc);
	//hal_HstSendEvent(SYS_EVENT,G_Mp3NonCliPRBuf);

#endif


    // *pDMA=hal_VocGetDmaiPointer((INT32*)G_Mp3NonCliPLBuf,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     
	//pDMA = hal_VocGetPointer(VPP_GLOBAL_NON_CLIP_HISTORYDATA_R_MP3_ADDR);
    // *pDMA=hal_VocGetDmaiPointer((INT32*)G_Mp3NonCliPRBuf,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     
	//pDMA = hal_VocGetPointer(VPP_GLOBAL_DIGITAL_GAIN_MP3_ADDR);

    // *pDMA=(INT32 *)(((32767)<<16)|(0x10));

     //for(INT32 i=0;i<30;i++)
    // {
        //calibPtr->bb->audio[4].vocFilters.sdfFilter[32+i]=f_sbc_persition[i];
    //    pFilter[32+i]=f_sbc_persition[i];
    // }

     //for(INT32 i=0;i<288;i++)
    // {
    //    G_Mp3NonCliPLBuf[i]=0;
           
     //}

	//NB: reuse jpeg Code and Const addr
	*((INT16 *)hal_VocGetPointer(Drc_Code_Load_Enable_addr)) = 1;

	pDMA = hal_VocGetPointer(VPP_DRC_MODE_Code_ExternalAddr_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppDRCCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA = pDMA + 1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppDRCConst,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	pDecIn = hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	pDecIn->reset=1;
	pDecIn->EQ_Type = -1;  	

	*((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = 0;
	*((INT16 *)hal_VocGetPointer(IIR_PCM_BAND_ACTIVE_NUM)) = 0;
    
	 pDecStatus = (vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT);
	 pDecStatus->ErrorStatus=0;
	 pDecStatus->mode=-1;//not 0~9;
 
	audecResetDrcFlag = 1;
    // move to STALL location (VoC irq generated)

      vpp_AudioJpegDecVocOpen_status=TRUE;

    return  hal_VocWakeup(HAL_VOC_START);
            
}


//=============================================================================
// vpp_AudioJpegDecClose function
//-----------------------------------------------------------------------------
void vpp_AudioJpegDecClose(void)
{
    if(vpp_AudioJpegDecVocOpen_status==TRUE)
    {
        hal_HstSendEvent(SYS_EVENT,0x201750c1);
	    hal_VocClose();   
    }

#ifdef SMALL_BSS_RAM_SIZE
   /*if(G_Mp3NonCliPRBuf_alloc)
	{
		mmc_MemFreeAll_BssRam(G_Mp3NonCliPRBuf_alloc);
		G_Mp3NonCliPRBuf_alloc=0;
    	}*/
     //if(G_Mp3NonCliPLBuf_alloc)
    //	{
	 //   mmc_MemFreeAll_BssRam(G_Mp3NonCliPLBuf_alloc);
	 //   G_Mp3NonCliPLBuf_alloc=0;
    //	}
#endif
    vpp_AudioJpegDecVocOpen_status=FALSE;    
    diag_printf("[vpp_AudioJpeg_DEC]Closing VPP\n");
    
}
//=============================================================================
// vpp_AudioJpegDecStatus function
//-----------------------------------------------------------------------------
void vpp_AudioJpegDecStatus(vpp_AudioJpeg_DEC_OUT_T * pDecStatus)
{  
    // copy status structure from VoC RAM to the destination.
    *pDecStatus = *((vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT));
}

void vpp_Mp3DecSetAudioParamReloadFlag(void)
{
	reCalcMp3EQ = 1;
}
void  vpp_AudioJpegDecCalcEqCoef(void)
{
	// calc mp3eq
	CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
	CALIB_AUDIO_IIR_EQ_T *eq = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[audioItf]].eq);
	vpp_AudioJpeg_DEC_OUT_T *pDecOutVoC = (vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT);

	if(reCalcMp3EQ == 1)
	{
		eq_active_num = calc_coeffs_for_voc(eq, pDecOutVoC->SampleRate, audioItf);
		reCalcMp3EQ = 2;
	}

	hal_HstSendEvent(SYS_EVENT,0x88300000); 
	hal_HstSendEvent(SYS_EVENT,eq->flag);
}
void vpp_Mp3DecSetEqParam(CALIB_AUDIO_IIR_EQ_T *eq, UINT8 num)
{
	int i;
	INT32 *IIR_Init = (INT32 *)hal_VocGetPointer(m_IIRProcPara_ADDR);
    sIIRcoefficients *iir_coef = get_coeffs_point();

	*((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = eq->flag;
	*((INT16 *)hal_VocGetPointer(IIR_PCM_BAND_ACTIVE_NUM)) = num;

	hal_HstSendEvent(SYS_EVENT,0x20170020);
	hal_HstSendEvent(SYS_EVENT,(UINT32)eq); 
	hal_HstSendEvent(SYS_EVENT,(UINT32)IIR_Init); 
	hal_HstSendEvent(SYS_EVENT,num); 
	hal_HstSendEvent(SYS_EVENT,(UINT32)iir_coef); 

	// each band has 6 coeffs(num[3], den[3])
	for (i = 0; i < CALIB_AUDIO_IIR_BANDS; i++)
	{
		IIR_Init[0] = iir_coef->num[0];
		IIR_Init[1] = iir_coef->num[1];
		IIR_Init[2] = iir_coef->num[2];
		IIR_Init[3] = iir_coef->den[0];
		IIR_Init[4] = iir_coef->den[1];
		IIR_Init[5] = iir_coef->den[2];

		IIR_Init += 6;
		iir_coef += 1;
	}
#ifdef SUPPORT_ASIC_EQ
       hal_AbbSetEq(eq->flag, (UINT32 *)get_coeffs_point());

#endif
    return;
	
}

void vpp_DecSetDrcmodeParam(CALIB_AUDIO_MUSIC_DRC_T drc_control,INT16 alg_gain)
{
	drc_control.thres = (drc_control.thres + alg_gain + drc_control.mm_gain)<<8;
	drc_control.width = drc_control.width<<8;
	drc_control.limit = (drc_control.limit + alg_gain + drc_control.mm_gain)<<8;
	drc_control.M	= drc_control.M<<8;
	drc_control.noise_gate = (drc_control.noise_gate + alg_gain + drc_control.mm_gain)<<8;

	*((CALIB_AUDIO_MUSIC_DRC_T*)hal_VocGetPointer(DRC_GLOBAL_STRUCT_DRC_PARAM)) = drc_control;
	*((INT32*)(hal_VocGetPointer(DRC_G_Y_alg_gain)))	= (INT32)((512<<16)|alg_gain); //crosszero_window=512,alg_gain
}
//=============================================================================
// vpp_MpegSetKalaokMode function
//-----------------------------------------------------------------------------
void vpp_MpegSetKalaokMode(uint16 flag, INT16 mode)
{
       if(mode == VPP_MP3_DEC_MODE)
       {
		*((UINT16 *)hal_VocGetPointer(mp123_support_kalaok_flag)) = flag;
	}
	else
	{
		*((UINT16 *)hal_VocGetPointer(aac_support_kalaok_flag)) = flag;
	}
}
//=============================================================================
// vpp_AudioJpegDecScheduleOneFrame function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_AudioJpegDecScheduleOneFrame(vpp_AudioJpeg_DEC_IN_T *pDecIn)
{
    vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
    extern UINT16 *g_last_buffer_addr;
//    INT32 * *pDMA;
    //CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    INT16  algGain;
    
    HAL_ERR_T voc_state;

    //diag_printf("vpp_AudioJpegDecScheduleOneFrame: input->data[0~3]=[%02x,%02x,%02x,%02x]", pDecIn->inStreamBufAddr[0], pDecIn->inStreamBufAddr[1], pDecIn->inStreamBufAddr[2], pDecIn->inStreamBufAddr[3]);
    //if(audioItf==SND_ITF_BT_SP) // for loud speaker, mix audio

	if(pDecIn->audioItf != audioItf)
	{
		 pDecIn->audioItf = audioItf;
		 audecResetDrcFlag = 1;
	}
	
	*pDecInVoC=*pDecIn;
	g_last_buffer_addr = (UINT16*)(pDecIn->outStreamBufAddr);
	pDecInVoC->inStreamBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecInVoC->inStreamUBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamUBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecInVoC->inStreamVBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamVBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecInVoC->outStreamBufAddr = hal_VocGetDmaiPointer(pDecIn->outStreamBufAddr,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecIn->reset=0;

	//hal_HstSendEvent(SYS_EVENT,0x88200000);    
	//hal_HstSendEvent(SYS_EVENT,pDecIn->inStreamBufAddr);
	//hal_HstSendEvent(SYS_EVENT,pDecIn->outStreamBufAddr);
	
    // diag_printf("[vpp_AudioJpeg_DEC]Codecmode:%d\n",pDecInVoC->mode); 	

    //set alg gain
       algGain = aud_GetOutAlgGainDb2Val();
       CALIB_AUDIO_IIR_EQ_T *eq = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[audioItf]].eq);


	//NB: just mp3 decoder may support drc mode
	if(pDecInVoC->mode == VPP_MP3_DEC_MODE)
	{
		*((INT32*)hal_VocGetPointer(AUDIO_MIXING_MP3_ADDR))=audioCfg[audioItf].mixAudio;

	       *((INT32*)hal_VocGetPointer(GLOBAL_DIGITAL_GAIN_MP3_ADDR)) = algGain;

		if(reCalcMp3EQ == 2)
		{
			vpp_Mp3DecSetEqParam(eq, eq_active_num);

			reCalcMp3EQ = 0;
		}
		
		if(audecResetDrcFlag==1)
		{
			CALIB_AUDIO_MUSIC_DRC_T drc_in = calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[audioItf]].drc;
			INT16 drc_alg_gain = 0;
			if(drc_in.alc_enable==1)
			{
				hal_HstSendEvent(SYS_EVENT,0x2017ee01);
				hal_HstSendEvent(SYS_EVENT,drc_in.thres);
				hal_HstSendEvent(SYS_EVENT,drc_in.width);
				hal_HstSendEvent(SYS_EVENT,drc_in.Thr_dB);
				vpp_DecSetDrcmodeParam(drc_in,drc_alg_gain);
			}
			else
			{
				hal_HstSendEvent(SYS_EVENT,0x2017ee00);
				*((INT16 *)hal_VocGetPointer(DRC_G_Y_alc_enable)) = 0;
			}
			audecResetDrcFlag = 0;
		}
	}
	else if(pDecInVoC->mode == VPP_AAC_DEC_MODE)
      {
      		*((INT16*)hal_VocGetPointer(AUDIO_MIXING_AAC_ADDR))=audioCfg[audioItf].mixAudio;
	       *((INT16*)hal_VocGetPointer(GLOBAL_DIGITAL_GAIN_AAC_ADDR)) = algGain;
      }

      vpp_MpegSetKalaokMode(kalaok_support_flag, pDecInVoC->mode);

/*
    if(audio_cfg.spkLevel%2==0)
    {
        *pDMA=(INT32 *)(((32767*0x10/0x20)<<16)|0x20);

    }
    else
    {
        *pDMA=(INT32 *)(((32767)<<16)|(0x10));
    }

*/  
    // schedule next frame on sof1 event
    voc_state = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);


	if (reCalcMp3EQ == 1)
	{
		// calc mp3eq
		vpp_AudioJpeg_DEC_OUT_T *pDecOutVoC = (vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT);
		
		eq_active_num = calc_coeffs_for_voc(eq, pDecOutVoC->SampleRate, audioItf);

		hal_HstSendEvent(SYS_EVENT,0x88300001); 
		hal_HstSendEvent(SYS_EVENT,eq->flag);
		hal_HstSendEvent(SYS_EVENT,algGain); 

		reCalcMp3EQ = 2;
	}


    return voc_state;
}

#ifdef VIDEO_PLAYER_SUPPORT
HAL_ERR_T  vpp_AudioJpegDecSetCurMode(INT16 pVoc_Video_Mode, INT16 pVoc_Audio_Mode)
{
	INT16 * pointVoC = hal_VocGetPointer(VPP_Current_Audio_Mode_addr);
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	
	*pointVoC=pVoc_Audio_Mode;
	pointVoC=pointVoC+1;
	*pointVoC=pVoc_Video_Mode;

	pDecInVoC->reset=0;//not reload code
	pDecInVoC->BsbcEnable=0;//bluetooth
	
	return HAL_ERR_NO;
}


HAL_ERR_T  vpp_AudioJpegDecSetInputMode(INT16 pVoc_Input_Mode)
{
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	
   	pDecInVoC->mode=pVoc_Input_Mode;
	pDecInVoC->BsbcEnable=0;
    
    	return HAL_ERR_NO;
}
/*
set voc audio/video decode mode
DecMode:	Codecmode enum
AVMode:		0= video, 1= audio.
Reset:		0=keep current mode,1=reset current mode in voc.
NOTE:		set Reset=1 for the first time.
sheen
2011.09.11
*/
HAL_ERR_T  vpp_AVDecSetMode(INT16 DecMode, INT16 AVMode, INT16 Reset)
{
	INT16 * pCurInVoC = hal_VocGetPointer(VPP_Current_Audio_Mode_addr);
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);

	pDecInVoC->mode=DecMode;
	pDecInVoC->reset=Reset;// 1 = set both current audio & video mode in valide and reload code
	pDecInVoC->BsbcEnable=0;//bluetooth
	
	if(AVMode==0)
	{//set next audio mode invalide in voc
		*pCurInVoC= -1;
	}
	else
	{//set next video mode invalide in voc
		pCurInVoC=pCurInVoC+1;
		*pCurInVoC= -1;
	}

	return HAL_ERR_NO;
}

INT16  vpp_AudioJpegDecGetMode(void)
{
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	return pDecInVoC->mode;
}

//=============================================================================
// vpp_AudioJpegSetReuseCode function
//reuse with VPP_H263_Zoom_Code_ExternalAddr_addr
//code_id: 0=zoom, 1=rmvb, 2=264, 3=mpeg4 ...
//set after vpp_AudioJpegDecOpen			
//-----------------------------------------------------------------------------
void vpp_AudioJpegSetReuseCode(INT16 code_id)
{
	INT32 * *pDMA;

	switch(code_id)
	{
		case 0:
			//h263 zoom
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 1:
			//rmvb
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppRMDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppRMDecConsty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 2:
			//h264
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH264DecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH264DecConsty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 3:
			//mpeg4
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMpeg4DecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMpeg4DecConsty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 4:
			//h263
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263DecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263DecConsty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 5:
			//jpeg
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJpegDec2Code,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			//pDMA=pDMA+1;
			//*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJPEGDec2Consty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;

		default:
			break;
	}
}
#endif

