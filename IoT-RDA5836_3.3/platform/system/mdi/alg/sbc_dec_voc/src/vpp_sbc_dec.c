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

#include "mcip_debug.h"
#include "vpp_sbc_dec.h"

#include "tgt_calib_m.h"
#include "tgt_aud_cfg.h"
#include "aud_m.h"

#include "protos.h"

#include "analog_audio_iir.h"
#include "mci.h"
#include "audio_voc_map.h"

#define vpp_SBC_DEC_IN_STRUCT         (Input_mode_addr)//(0   + 0x4000) 
#define vpp_SBC_DEC_OUT_STRUCT        (SBC_DEC_FRAME_FREQUENCY)//(82  + 0x4000)
#define vpp_SBC_DEC_OUT_STATUS        (Output_ERR_Status_addr)//(29  + 0x4000)
#define vpp_SBC_DEC_IN_LENGTH         (SBCDEC_INPUTSTREAM_LENGTH_ADDR)//(74  + 0x4000)

#define vpp_SBC_DEC_OUT_LENGTH        (SBCDEC_OUTPUTSTREAM_LENGTH_ADDR)//(78  + 0x4000)

#define vpp_SBC_DEC_INIT              (SBC_DEC_FRAME_INIT)//(95+0x4000)

#define VPP_SBC_Code_ExternalAddr_addr  (H263_Zoom_Code_ExternalAddr_addr)//(68 + 0x4000)
#define VPP_SBC_MIXING_AUDIO_FLAGE      (sbc_mixing_audio_flag)//(4158+74+0X4000)


/*
//below defined in audio_voc_map.h
#define SBC_DEC_PCM_OUT_ADDR_begain                (4248+74+0X4000)
#define SBC_DEC_PCM_OUT_ADDR_end                   (4250+74+0X4000)

#define sbc_detect_pval_addr                       (4244+74+0x4000)
#define sbc_detect_nval_addr                       (4246+74+0x4000)
#define sbc_detect_mute_addr                       (4260+74+0x4000)

#define SBC_48K_SET_DMA_OUT_LENGTH_FLAG            (4254+74+0x4000)
#define SBC_44_1K_TO_48K_ENABLE_FLAG               (4262+74+0x4000)
#define SBC_44_1K_TO_48K_InterPhase                (4263+74+0x4000)
#define SBC_44_1K_TO_48K_REMAIN_LENGTH             (4585+74+0x4000)


#define SBCDEC_IIR_ENABLE_FLAG_ADDR                (5332+74+0x4000)
#define m_SBCDEC_IIRProcPara_ADDR                  (5334+74+0x4000)
#define GLOBAL_DIGITAL_GAIN_SBCDEC_ADDR            (4166+74+0x4000)

#define SBC_DEC_FRAME_BLOCKS                       (10+74+0x4000)
#define SBC_DEC_FRAME_CHANNELS                     (12+74+0x4000)
#define SBC_DEC_FRAME_SUBBANDS                     (15+74+0x4000)
#define SBC_DEC_FRAME_SB_SAMPLE                    (94+74+0x4000)
*/

#define VPP_SBC_DRC_MODE_Code_ExternalAddr_addr			JPEG_Dec_Code_ExternalAddr_addr

#define vpp_AudioJpeg_DEC_CODE_SIZE       (280*4)
#define vpp_AudioJpeg_DEC_CODE_ENTRY 0
#define vpp_AudioJpeg_DEC_CRITICAL_SECTION_MIN 0
#define vpp_AudioJpeg_DEC_CRITICAL_SECTION_MAX 0


//#define VPP_SBC_SAMPLING_FREQ_16000          0   
//#define VPP_SBC_SAMPLING_FREQ_32000          1
//#define VPP_SBC_SAMPLING_FREQ_44100          2
//#define VPP_SBC_SAMPLING_FREQ_48000          3



//common code
extern const INT32 G_VppCommonDecCode[];
extern const INT32 G_VppSBCDecCode[];
extern const INT32 G_VppSBCDecConstX[];

extern const INT32 G_VppDRCCode[];
extern const INT32 G_VppDRCConst[];

static bool reCalcSbcdecEQ = 0;
static INT16 SbcdecFirstFlag = 1;

extern AUD_ITF_T audioItf;

extern void vpp_DecSetDrcmodeParam(CALIB_AUDIO_MUSIC_DRC_T drc_control,INT16 alg_gain);

void vpp_SBCSetBandInfoEnable(uint16 flag)
{
	*((UINT16 *)hal_VocGetPointer(SBCDEC_BAND_INFO_ENABLE_FLAG_ADDR)) = flag;
}
#ifdef MUSIC_BAND_INFO

void vpp_SBCGetBandInfo(uint16 * band_buf)
{
   UINT32 band[8];
   int i;
   
   UINT32 channels = *((UINT16 *)hal_VocGetPointer(SBC_DEC_FRAME_CHANNELS));
   UINT32 blocks = *((UINT16 *)hal_VocGetPointer(SBC_DEC_FRAME_BLOCKS));

   UINT32 *pBandVoCInfo = (UINT32 *)hal_VocGetPointer(m_SBCDEC_BAND_INFO_ADDR);

   //hal_HstSendEvent(SYS_EVENT,0x20170223);

   for(i=0; i<8;i++)
   {
        band[i] = *(pBandVoCInfo+i);
	 if(channels == 2)
	 {
            band[i] >>= (1+3); //8 frame;
	 }
	 else
	 {
            band[i] >>= 3; //8 frame;
	 }
	 
	 if(blocks == 4)
	 {
            band[i] >>= 2;
	 }
	 else
	 {
            band[i] >>= 3;
	 }
	 band_buf[i] = (UINT16) band[i];

	//hal_HstSendEvent(SYS_EVENT,band_buf[i]); 
	//hal_HstSendEvent(SYS_EVENT,band[i]);

   }
}

#endif
//=============================================================================
// vpp_SBCSetKalaokMode function
//-----------------------------------------------------------------------------
void vpp_SBCSetKalaokMode(uint16 flag)
{
	*((UINT16 *)hal_VocGetPointer(sbc_support_kalaok_flag)) = flag;
}
//=============================================================================
// vpp_SBCGetNoUsedAddr function
//-----------------------------------------------------------------------------
UINT16 * vpp_SBCGetVoCNoUsedAddr(void)
{
	return ((UINT16 *)hal_VocGetPointer(SBCDEC_NO_USED_X_ADDR));// size of noused memory is 20480 - SBCDEC_NO_USED_X_ADDR;
}
//=============================================================================
// vpp_AudioJpegDecOpen function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_SBCDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) 
{
    HAL_VOC_CFG_T cfg;
    vpp_SBC_DEC_IN_T * pDecIn;
    vpp_SBC_DEC_OUT_T *pDecStatus;
    INT32 * *pDMA;
	//INT32 * pDataInit;
	//INT16 *pVocEq; 
	
	diag_printf("[vpp_AudioJpeg_DEC]Opening VPP SBC_DEC\n"); 

    hal_HstSendEvent(SYS_EVENT,0x8810000a);
      
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
   
	//sbc dec
	pDMA = hal_VocGetPointer(VPP_SBC_Code_ExternalAddr_addr);

	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppSBCDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppSBCDecConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);	
	*((INT16 *)hal_VocGetPointer(Drc_Code_Load_Enable_addr)) = 1;
	pDMA = hal_VocGetPointer(VPP_SBC_DRC_MODE_Code_ExternalAddr_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppDRCCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppDRCConst,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	pDecIn = hal_VocGetPointer(vpp_SBC_DEC_IN_STRUCT);
	pDecIn->reset=1;
    pDecIn->mode=15;
    pDecIn->EQ_Type=0;
	 pDecStatus = (vpp_SBC_DEC_OUT_T *)hal_VocGetPointer(vpp_SBC_DEC_OUT_STRUCT);
	 pDecStatus->ErrorStatus=0;
	 
   *((INT16*)hal_VocGetPointer(VPP_SBC_MIXING_AUDIO_FLAGE))=0;
   *((INT16 *)hal_VocGetPointer(vpp_SBC_DEC_INIT)) = 0; 

   //init pcm upsample from 44_1K TO 48K
   *((INT16*)hal_VocGetPointer(SBC_44_1K_TO_48K_InterPhase))=0;
   *((INT16*)hal_VocGetPointer(SBC_44_1K_TO_48K_REMAIN_LENGTH))=0;
   *((INT16*)hal_VocGetPointer(SBC_48K_SET_DMA_OUT_LENGTH_FLAG))=0;

   //set eq param
   *((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = 0;
   *((INT16 *)hal_VocGetPointer(IIR_PCM_BAND_ACTIVE_NUM)) = 0;
	
   vpp_SBCDecSetAudioParamReloadFlag(3);
   SbcdecFirstFlag = 1;
   
   vpp_SBCDecSetDetectMuteThd(1);
#ifdef MUSIC_BAND_INFO
   vpp_SBCSetBandInfoEnable(1);
#else
   vpp_SBCSetBandInfoEnable(0);
#endif
#ifdef A2DP_SAMPLE_44_1K_TO_48K
	vpp_SBCDecSampleCoverEnable();
#else
    vpp_SBCDecSampleCoverDisable();

#endif
#ifdef KALAOK_SUPPORT
    vpp_SBCSetKalaokMode(1);
#else
    vpp_SBCSetKalaokMode(0);
#endif
    // move to STALL location (VoC irq generated)
    vpp_SBCSetMixMode(0);

    return  hal_VocWakeup(HAL_VOC_START);
            
}


//=============================================================================
// vpp_AudioJpegDecClose function
//-----------------------------------------------------------------------------
void vpp_SBCDecClose(void)
{
    hal_VocClose();   
    diag_printf("[vpp_AudioSbc_DEC]Closing VPP\n");
    
}
//=============================================================================
// vpp_SBCDecStatus function
//-----------------------------------------------------------------------------
void vpp_SBCDecStatus(vpp_SBC_DEC_OUT_T * pDecStatus)
{  
    // copy status structure from VoC RAM to the destination.
    *pDecStatus = *((vpp_SBC_DEC_OUT_T *)hal_VocGetPointer(vpp_SBC_DEC_OUT_STRUCT));
    pDecStatus->ErrorStatus = *((INT16 *)hal_VocGetPointer(vpp_SBC_DEC_OUT_STATUS));
    
    pDecStatus->sample_length = *((INT32 *)hal_VocGetPointer(vpp_SBC_DEC_OUT_LENGTH));
    
     
}

//=============================================================================
// vpp_SBCDecSampleCoverStart function
//-----------------------------------------------------------------------------
void vpp_SBCDecSampleCoverEnable(void)
{  
	*((INT16*)hal_VocGetPointer(SBC_44_1K_TO_48K_ENABLE_FLAG))=1;
}
//=============================================================================
// vpp_SBCDecSampleCoverStart function
//-----------------------------------------------------------------------------
void vpp_SBCDecSampleCoverDisable(void)
{  
	*((INT16*)hal_VocGetPointer(SBC_44_1K_TO_48K_ENABLE_FLAG))=0;
}

//=============================================================================
// vpp_SBCDecSetDetectMuteThd
//-----------------------------------------------------------------------------
///Set Detect Mute Thd;
//=============================================================================

UINT16 vpp_SBCDecSetDetectMuteThd(short Thd)
{  
	if(Thd < 0)
	{
		Thd = -Thd;
	}
	*((INT16*)hal_VocGetPointer(sbc_detect_pval_addr)) = Thd;
	*((INT16*)hal_VocGetPointer(sbc_detect_nval_addr)) = 0-Thd;

	return 0;
}

//=============================================================================
// vpp_SBCDecDetectMute function
//-----------------------------------------------------------------------------
/// return 1:mute;
//=============================================================================

UINT16 vpp_SBCDecDetectMute(void)
{  
	return *((INT16*)hal_VocGetPointer(sbc_detect_mute_addr));
}

//=============================================================================
// vpp_SBCSetMixMode function
//-----------------------------------------------------------------------------
/// mdoe 0&1:use target set; 2:use left; 3:use right
//=============================================================================

void vpp_SBCSetMixMode(UINT16 mode)
{  
	CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();

	if(mode == 2 || mode == 3)
	{
		// set mix
		*((INT16 *)hal_VocGetPointer(VPP_SBC_MIXING_AUDIO_FLAGE)) = mode;
	}
	else
	{

		// set mix
		*((INT16 *)hal_VocGetPointer(VPP_SBC_MIXING_AUDIO_FLAGE)) = audioCfg[audioItf].mixAudio;
	}
}

//=============================================================================
// vpp_SBCDecCheckOutPcmAddr function
//-----------------------------------------------------------------------------

void vpp_SBCDecCheckOutPcmAddr(UINT32 *pAddrBegain, UINT32 *pAddrEnd)
{  
    UINT32 *pSbcDecOutPcmAddrBegain = (UINT32 *)hal_VocGetPointer(SBC_DEC_PCM_OUT_ADDR_begain);
    UINT32 *pSbcDecOutPcmAddrEnd    = (UINT32 *)hal_VocGetPointer(SBC_DEC_PCM_OUT_ADDR_end);

	*pSbcDecOutPcmAddrBegain = (UINT32)hal_VocGetDmaiPointer(pAddrBegain,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	*pSbcDecOutPcmAddrEnd    = (UINT32)hal_VocGetDmaiPointer(pAddrEnd,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
}
void vpp_SBCDecSetAudioParamReloadFlag(UINT8 flag)
{
	reCalcSbcdecEQ = flag;
}
#if 0
short iir_eq_coeffs[7][6] =
{
	{0x3eb9, 0xffff828f, 0x3eb9, 0x4000, 0x7d6b, 0xffffc287},
	{0x4000, 0xffff8529, 0x3b0b, 0x4000, 0x7ad7, 0xffffc4f5},
	{0x4000, 0xffff8a4d, 0x3679, 0x4000, 0x75b3, 0xffffc987},
	{0x4000, 0xffff947f, 0x2e5d, 0x4000, 0x6b81, 0xffffd1a3},
	{0x4000, 0xffffa85d, 0x219c, 0x4000, 0x57a3, 0xffffde64},
	{0x4000, 0xffffcdbc, 0x121a, 0x4000, 0x3244, 0xffffede6},
	{0x1ecb, 0x3d97, 0x1ecb, 0x4000, 0xffffd4fd, 0xffffefd6},
};

short iir_eq_coeffs[7][6] =
{
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
	{0x4000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000},
};
#endif
void vpp_SBCDecSetEqParam(CALIB_AUDIO_IIR_EQ_T *eq, UINT8 num)
{
	int i;
	INT32 *IIR_Init = (INT32 *)hal_VocGetPointer(m_IIRProcPara_ADDR);
       sIIRcoefficients *iir_coef = (sIIRcoefficients *)get_coeffs_point();

	*((INT16 *)hal_VocGetPointer(IIR_ENABLE_FLAG_ADDR)) = eq->flag;
	*((INT16 *)hal_VocGetPointer(IIR_PCM_BAND_ACTIVE_NUM)) = num;

	hal_HstSendEvent(SYS_EVENT,0x20170010);
	hal_HstSendEvent(SYS_EVENT,(UINT32)eq); 
	hal_HstSendEvent(SYS_EVENT,(UINT32)IIR_Init); 
	hal_HstSendEvent(SYS_EVENT,(UINT32)num); 

#if 1
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
//#else
	//memcpy(IIR_Init, iir_eq_coeffs, 6*7*2);
#endif

    return;
	
}


//typedef enum
//{
//    SKIP = 0,
//    LPF,
//    HPF,
//   peakingEQ,
//    lowShelf,
//    highShelf,
//} FILTER_TYPE;
void vpp_SBCDecSetConstEqParam(CALIB_AUDIO_IIR_EQ_T *eq, AUDIO_EQ eq_type)
{
	hal_HstSendEvent(SYS_EVENT,0x88700001); 
	hal_HstSendEvent(SYS_EVENT,eq_type); 
	
	eq->band[0].type = peakingEQ;
	eq->band[0].freq = 100;
	eq->band[0].param.qual = 7;

	eq->band[1].type = peakingEQ;
	eq->band[1].freq = 200;
	eq->band[1].param.qual = 7;

	eq->band[2].type = peakingEQ;
	eq->band[2].freq = 400;	
	eq->band[2].param.qual = 7;

	eq->band[3].type = peakingEQ;
	eq->band[3].freq = 600;	
	eq->band[3].param.qual = 7;

	eq->band[4].type = peakingEQ;
	eq->band[4].freq = 1000;
	eq->band[4].param.qual = 7;

	eq->band[5].type = peakingEQ;
	eq->band[5].freq = 2000;
	eq->band[5].param.qual = 7;

	eq->band[6].freq = peakingEQ;
	eq->band[6].type = 4000;
	eq->band[6].param.qual = 7;

	eq->band[7].freq = peakingEQ;
	eq->band[7].type = 8000;
	eq->band[7].param.qual = 7;
	
	eq->band[8].freq = peakingEQ;
	eq->band[8].type = 12000;
	eq->band[8].param.qual = 7;
	
	eq->band[9].freq = peakingEQ;
	eq->band[9].type = 16000;
	eq->band[9].param.qual = 7;
	switch(eq_type)
	{
		case BASS :
			      eq->band[0].gain = 5;
			      eq->band[1].gain = 4;
			      eq->band[2].gain = 3;
			      eq->band[3].gain = 2;
			      eq->band[4].gain = 0;
			      eq->band[5].gain = 0;
			      eq->band[6].gain = -1;
			      eq->band[7].gain = -1;
			      eq->band[8].gain = -2;
			      eq->band[9].gain = -2;
		             break;
		case DANCE :
			      eq->band[0].gain = 0;
			      eq->band[1].gain = 0;
			      eq->band[2].gain = 2;
			      eq->band[3].gain = 3;
			      eq->band[4].gain = 2;
			      eq->band[5].gain = 0;
			      eq->band[6].gain = 1;
			      eq->band[7].gain = 2;
			      eq->band[8].gain = 3;
			      eq->band[9].gain = 2;
		             break;	
		case CLASSICAL : 
			      eq->band[0].gain = 0;
			      eq->band[1].gain = 2;
			      eq->band[2].gain = 3;
			      eq->band[3].gain = 3;
			      eq->band[4].gain = 2;
			      eq->band[5].gain = 0;
			      eq->band[6].gain = 0;
			      eq->band[7].gain = 0;
			      eq->band[8].gain = 1;			      
			      eq->band[9].gain = 0;
		             break;		
		case TREBLE : 
			      eq->band[0].gain = 0;
			      eq->band[1].gain = -2;
			      eq->band[2].gain = -4;
			      eq->band[3].gain = -1;
			      eq->band[4].gain = -1;
			      eq->band[5].gain = 0;
			      eq->band[6].gain = 2;
			      eq->band[7].gain = 4;
			      eq->band[8].gain = 3;			      
			      eq->band[9].gain = 0;
		             break;		
		case PARTY : 
			      eq->band[0].gain = 0;
			      eq->band[1].gain = 0;
			      eq->band[2].gain = 0;
			      eq->band[3].gain = 0;
			      eq->band[4].gain = 2;
			      eq->band[5].gain = 4;
			      eq->band[6].gain = 4;
			      eq->band[7].gain = 0;
			      eq->band[8].gain = 0;			      
			      eq->band[9].gain = 0;
		             break;
		case POP : 
			      eq->band[0].gain = 3;
			      eq->band[1].gain = 1;
			      eq->band[2].gain = 0;
			      eq->band[3].gain = -2;
			      eq->band[4].gain = -4;
			      eq->band[5].gain = -4;
			      eq->band[6].gain = -2;
			      eq->band[7].gain = 0;
			      eq->band[8].gain = 1;			      
			      eq->band[9].gain = 2;
		             break;
		case ROCK : 
			      eq->band[0].gain = -2;
			      eq->band[1].gain = 0;
			      eq->band[2].gain = 2;
			      eq->band[3].gain = 4;
			      eq->band[4].gain = -2;
			      eq->band[5].gain = -2;
			      eq->band[6].gain = 0;
			      eq->band[7].gain = 0;
			      eq->band[8].gain = 2;			      
			      eq->band[9].gain = 3;
		             break;
	       case NORMAL :
              default : 
			      eq->band[0].gain = 0;
			      eq->band[1].gain = 0;
			      eq->band[2].gain = 0;
			      eq->band[3].gain = 0;
			      eq->band[4].gain = 0;
			      eq->band[5].gain = 0;
			      eq->band[6].gain = 0;
			      eq->band[7].gain = 0;
			      eq->band[8].gain = 0;			      
			      eq->band[9].gain = 0;
			      eq->flag = 0;
		             break;		 
	}
	return;
	
}
//=============================================================================
// vpp_AudioJpegDecScheduleOneFrame function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_SBCDecScheduleOneFrame(vpp_SBC_DEC_IN_T *pDecIn)
{ 
	//hal_HstSendEvent(SYS_EVENT,0x88200000); 

	vpp_SBC_DEC_IN_T  *pDecInVoC = (vpp_SBC_DEC_IN_T *)hal_VocGetPointer(vpp_SBC_DEC_IN_STRUCT);

	INT16 init,audiomode,algGain;
	extern UINT16 *g_last_buffer_addr;
	CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
	CALIB_AUDIO_IIR_EQ_T EQ_COEF;
	HAL_ERR_T voc_state;
	static INT32 SbcdecSampleRate = 0;
       static UINT8 sbc_eq_active_num = 0;
	*pDecInVoC=*pDecIn;
	g_last_buffer_addr = (UINT16*)(pDecInVoC->outStreamBufAddr);
	pDecInVoC->inStreamBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecInVoC->outStreamBufAddr = hal_VocGetDmaiPointer(pDecIn->outStreamBufAddr,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDecIn->reset = 0;
	pDecInVoC->EQ_Type = pDecIn->EQ_Type;

	// set alg gain
	algGain = aud_GetOutAlgGainDb2Val();
	*((INT32*)hal_VocGetPointer(GLOBAL_DIGITAL_GAIN_SBCDEC_ADDR)) = algGain;

	// set mp3eq
	CALIB_AUDIO_IIR_EQ_T *eq = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[audioItf]].eq);

   if(reCalcSbcdecEQ == 1)
   {
	if(pDecIn->EQ_Type)
	{
		eq = &EQ_COEF;
		eq->flag = 1;
	}
	vpp_SBCDecSetEqParam(eq, sbc_eq_active_num);

	hal_HstSendEvent(SYS_EVENT,0x2017000c);

	reCalcSbcdecEQ = 0;
	}


	if((SbcdecFirstFlag==1)||(pDecIn->audioItf != audioItf))
	{
		//set drc
		CALIB_AUDIO_MUSIC_DRC_T drc_in = (CALIB_AUDIO_MUSIC_DRC_T)(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[audioItf]].drc);
		INT16 drc_alg_gain = 0;
		if(drc_in.alc_enable==1)
		{
			hal_HstSendEvent(SYS_EVENT,0x2017ff01);
			hal_HstSendEvent(SYS_EVENT,drc_in.thres);
			hal_HstSendEvent(SYS_EVENT,drc_in.width);
			hal_HstSendEvent(SYS_EVENT,drc_in.Thr_dB);
			vpp_DecSetDrcmodeParam(drc_in,drc_alg_gain);
		}
		else
		{
			hal_HstSendEvent(SYS_EVENT,0x2017ff00);
			*((INT16*)(hal_VocGetPointer(DRC_G_Y_alc_enable))) = 0;
		}
		pDecIn->audioItf = audioItf;
	}



	*((INT16 *)hal_VocGetPointer(vpp_SBC_DEC_IN_LENGTH)) = 1024;
	init = *((INT16 *)hal_VocGetPointer(vpp_SBC_DEC_INIT));
	audiomode = *((INT16 *)hal_VocGetPointer(vpp_SBC_DEC_IN_STRUCT+36));


	voc_state = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);

	if (reCalcSbcdecEQ > 1 && SbcdecFirstFlag == 0)
	{
	       if(reCalcSbcdecEQ == 3)
	       {
			pDecIn->EQ_Type = 0;
		}
		if(pDecIn->EQ_Type)
		{
			vpp_SBCDecSetConstEqParam(&EQ_COEF, pDecIn->EQ_Type);
			eq = &EQ_COEF;
		}
		
		vpp_SBC_DEC_OUT_T *pDecOutVoC = (vpp_SBC_DEC_OUT_T *)hal_VocGetPointer(vpp_SBC_DEC_OUT_STRUCT);
		switch(pDecOutVoC->freq)
		{
			case 0 : SbcdecSampleRate = 16000;
				break;
			case 1 : SbcdecSampleRate = 32000;
				break;
			case 2 : SbcdecSampleRate = 44100;
				break;
			case 3 : SbcdecSampleRate = 48000;
				break;

			default : SbcdecSampleRate = 44100;
				break;
		}

		sbc_eq_active_num = calc_coeffs_for_voc(eq, SbcdecSampleRate, audioItf);

		hal_HstSendEvent(SYS_EVENT,0x2017000b);
		hal_HstSendEvent(SYS_EVENT,(UINT32)SbcdecSampleRate); 
		hal_HstSendEvent(SYS_EVENT,(UINT32)eq->flag); 
		hal_HstSendEvent(SYS_EVENT,(UINT32)algGain); 

		hal_HstSendEvent(SYS_EVENT,(UINT32)audioItf); 
		hal_HstSendEvent(SYS_EVENT,(UINT32)eq); 


		reCalcSbcdecEQ = 1;
	}

	if(SbcdecFirstFlag)
	{
		SbcdecFirstFlag = 0;
	}
	return voc_state;
}

