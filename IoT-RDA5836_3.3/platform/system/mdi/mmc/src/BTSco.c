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


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file vois.c                                                               //
/// That file implementes the VOIS service.                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "string.h"
#include "fs.h"

#include "hal_timers.h"
#include "hal_aif.h"
#include "hal_sys.h"
#include "hal_speech_bt.h"
#include "hal_overlay.h"

#include "vois_m.h"
//#include "vois_map.h"
//#include "voisp_map.h"
//#include "voisp_cfg.h"

#include "aud_m.h"
#include "vpp_speech_bt.h"
#include "tgt_calib_m.h"
#include "BTSco.h"
#include "cvsd.h"

#include "hal_map_engine.h"

#include "sxr_sbx.h"
#include "mcip_debug.h"
#include "mci.h"
#include "cos.h"
#include "event.h"
#include "dm.h"
#include "speex/speex_preprocess.h"
#include "plc.h"
#include "bt_msg.h"
#include "hci.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Number of trames before unmuting VPP
#define BT_VPP_UNMUTE_TRAME_NB 2

#define VOC_INTR_STEP


// =============================================================================
//  DEBUGE
// =============================================================================
//#define DEBUG_TICKS
#ifdef DEBUG_TICKS
	INT32 g_VoCHandleTicks = 0;
	INT32 g_ScoTicks = 0;
#endif
	INT32 g_PlcTicks = 0;


#ifdef BT_SCO_RECORD_SUPPORT
#if 0
	#define DUMP_SCO_NAME L"dump.pcm"
#endif
#define CHECK_SCO_DELAY 1
INT32  gDumpScoCurFd = -1;
INT32  gScoRecordFlag = 0;
INT32  gScoWriteFlag = 0;
UINT8  *dump_data_buf = NULL;
extern VOID ScoMcdOpen();
extern VOID ScoMcdClose();
extern VOID test_mcd_Write(UINT32 blockStartAddr, CONST UINT8* blockWr, UINT32 blockLength);

#else
       #define CHECK_SCO_DELAY 0
#endif

#ifndef __AT_MOD_SPECIAL__
	#define SUPPORT_SCO_PLC
	UINT8 gPlcInitFlag = 0;
#endif
// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#define MMC_SCO_PACKET_MAX_SIZE           60
#define MMC_SCO_SEND_BUFF_SIZE      (160*2)
#define MMC_SCO_BUFF_SIZE           (160*2)
#define MMC_SCO_MIC_BUFF_SIZE		(640)

#ifdef BTSCO_MALLOC_MEM
UINT16 *ReceivedScoDataCache = NULL;
UINT32 *MicData = NULL;
INT16  *ScoSendData = NULL;
#else
UINT16 ReceivedScoDataCache[MMC_SCO_BUFF_SIZE];
UINT32 MicData[MMC_SCO_MIC_BUFF_SIZE/4];
INT16  ScoSendData[MMC_SCO_PACKET_MAX_SIZE];
INT16  ScoRcvData[MMC_SCO_PACKET_MAX_SIZE];
#endif

#if (USE_XCPU_CVSD == 1)
#ifdef USE_HFP_CVSD
static short CvsdDecHighPassHistoryX[2] = {0};
static short CvsdDecHighPassHistoryY[2] = {0};
static short CvsdEncHighPassHistoryX[2] = {0};
static short CvsdEncHighPassHistoryY[2] = {0};
#endif
#endif

hci_sco_data_msg_t ScoSendMsg;

#define DEBUG_SCO 0

#if (DEBUG_SCO == 1 || CHECK_SCO_DELAY == 1)
INT16 ScoPlayTestData[320] = 
{
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
};//800hz
#endif
UINT16 SendDataLen = 0;
UINT16 ReceiveDataLen = 0;
BOOL  MMC_ScoMicDataReadySend = TRUE;
BOOL  MMC_ScoDataReadyPlay = FALSE;

UINT8 ScoReceiveDataFlag=0;

#ifdef USE_VOC_CVSD
UINT8 HistoryScoReceiveDataLen=30;
#define SPEECH_BT_FILL_INVALID_DATA   (0x55)
#else
UINT8 HistoryScoReceiveDataLen=60;
#define SPEECH_BT_FILL_INVALID_DATA   (0x00)
#endif
extern int32 MMC_AudioSCO_AsynSendReq(int16* pdu,uint16 handle, uint16 length);
extern void rdabt_send_msg_to_bt(COS_EVENT *event);
extern BOOL BTSco_AudCallStop(void);
extern UINT16 BTSco_AudFileHandle(void);
extern PUBLIC void hal_SysSetVocClockDefault(void);
extern PUBLIC UINT32 csw_TMGetTick(VOID);


// =============================================================================
// g_BtCtx
// -----------------------------------------------------------------------------
/// VoiS context, holding global variables, to be watched through CoolWatcher.
// =============================================================================
PROTECTED VOIS_CONTEXT_T g_BtCtx =
{
            .voisStarted = FALSE,
            .voisEncOutput = NULL,
            .voisDecInput = NULL, 
            .voisTramePlayedNb = 0,
            .voisAudioCfg = {0,0,0,0},
            .voisVppCfg = {0,0,0,0,0,0,0,0,NULL, NULL,0},
    .voisItf = AUD_ITF_NONE,
    .voisLatestCfg =
    {
                                .spkSel     = AUD_SPK_RECEIVER,
                                .micSel     = AUD_MIC_RECEIVER,
                                .spkLevel   = AUD_SPK_MUTE,
                                .micLevel   = AUD_MIC_MUTE,
                                .sideLevel  = AUD_SIDE_MUTE,
                                .toneLevel  = AUD_TONE_0dB,
                                .encMute    = VPP_SPEECH_MUTE, 
        .decMute    = VPP_SPEECH_MUTE
    },
            .mode = 0,
            .bufferSize = MMC_SCO_BUFF_SIZE,
            .fileHandle = -1,
};
#if 0
///  Audio params,MDF and SDF profile.
const UINT16 default_sdfFilter[] = {
                0x4000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000, \
				0x0000, 0x0000, 0x0000, 0x0000};
#endif                
/// Stream to encode.
PUBLIC HAL_AIF_STREAM_T micStream;

/// Decoded stream.
extern HAL_AIF_STREAM_T audioStream;

/// User handler to call with play status is stored here.
PRIVATE VOIS_USER_HANDLER_T g_BtUserHandler= NULL;

/// SRAM overlay usage flag
//PRIVATE BOOL g_BtOverlayLoaded = FALSE;

/// Record flag
PRIVATE VOLATILE BOOL g_BtRecordFlag = FALSE;
       
/// End address of the stream buffer    
//PRIVATE UINT32 g_BtBufferLength= 0;

/// End address of the stream buffer    
//PRIVATE UINT32* g_BtBufferTop = NULL;

/// PCM buffer address
UINT32* g_BtRecordMicPcmBuffer = NULL;

PRIVATE  UINT8 *g_BtAgcPcmBufferReceiver=NULL;

CALIB_AUDIO_VOC_SPEEX_T speex_config =
{
    .rx =
    {
        .flag = 0,
        .noise_suppress_default = -30,
        .noise_probe_range = {30, 30, 30, 30},
    },
    .tx =
    {
        .flag = 0,
        .noise_suppress_default = -30,
        .noise_probe_range = {30, 30, 30, 30},
    },
};

CALIB_AUDIO_VOC_AGC_T agc_config =
{
    .rx =
    {
        .flag = 0,
        .smooth_threshold = 700,
        .noise_threshold = 1000,
        .gain = 0x40,
    },
    .tx =
    {
        .flag = 0,
        .smooth_threshold = 700,
        .noise_threshold = 1000,
        .gain = 0x40,
    },
};


PROTECTED BOOL g_msdfReload = 0;

// =============================================================================
//  FUNCTIONS
// =============================================================================

VOLATILE AUD_ITF_T BT_AudInterface = 0;

VOLATILE INT16 BT_VoCRunFlag = 0;

VOLATILE UINT8 BT_AudPath = 0; // 0 for ear-piece, 1 for louder speaker


INT8* Get_BtvoisEncOutput(void)
{
    return (INT8*)g_BtCtx.voisEncOutput;
}

INT8* Get_BtAgcPcmBufferReceiver(void)
{
    return (INT8*)g_BtAgcPcmBufferReceiver;
}

INT8* Get_BtRecordMicPcmBuffer(void)
{
    return (INT8*)g_BtRecordMicPcmBuffer;
}

// =============================================================================
// BTSco_SetCalibratedVppCfg
// -----------------------------------------------------------------------------
/// Using a VOIS configuration structure, this function gets the calibrated data
/// from the calibration structure to configure properly a VPP config
/// structure.
/// @param itf Audio Interface used.
/// @param vppCfg VPP configuration structure to set with calibrated data.
// =============================================================================
PRIVATE VOID BTSco_SetCalibratedVppCfg(AUD_ITF_T itf,
                                      VPP_SPEECH_AUDIO_CFG_T* vppCfg)
{
    // FIXME : All calibration to be checked
    //CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
#ifdef USE_OLD_AEC_NS
    // Set the configuration (in the global variables)
    // Get VPP config From Calib
    
    //#define DEFAULT_CALIB_AUDIO_PARAMS_CONFIG0  {
	//	1, 0, 0, 127, 0, 8, 127, 28}
    //#define DEFAULT_CALIB_ECHO_CANCEL_CONFIG0  {
	//	0, 0, 0, 0, 0, 0, 0}
	
    vppCfg->echoEsOn     = 0;//calibPtr->aud.echo[BT_AudPath].vocEc.esOn;
    vppCfg->echoEsVad    = 0;//calibPtr->aud.echo[BT_AudPath].vocEc.esVad;
    vppCfg->echoEsDtd    = 0;//calibPtr->aud.echo[BT_AudPath].vocEc.esDtd;
    vppCfg->echoExpRel   = 0;//calibPtr->aud.echo[BT_AudPath].vocEc.ecRel;
    vppCfg->echoExpMu    = 0;//((calibPtr->aud.echo[BT_AudPath].vocEc.ecMu == 0) || (g_BtCtx.voisAudioCfg.spkLevel < calibPtr->aud.echo[BT_AudPath].vocEc.ecMu)) ? 0 : 1;
    vppCfg->echoExpMin   = 0;//calibPtr->aud.echo[BT_AudPath].vocEc.ecMin;
#endif
    vppCfg->sdf          = (INT32*)calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.sdfFilter;
    vppCfg->mdf          = (INT32*)calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.mdfFilter;
}


// =============================================================================
// BTSco_VppSpeechHandler
// -----------------------------------------------------------------------------
/// Handler for the vpp module, for speech-kind encoded streams. 
/// In charge of copying new datas, dealing with
/// underflow (no more samples), pause and resume etc ... It is called 
/// every time a trame has been decoded by VPP.
// =============================================================================
PRIVATE VOID BTSco_VppSpeechHandler(HAL_VOC_IRQ_STATUS_T* status)
{
	      // FIXME : All calibration to be checked
  //  CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    //CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
    COS_EVENT ev;
    ev.nEventId = EV_BT_SCO_REC;
   //MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_VppSpeechHandler");
   UINT32 voc_ticks;
   voc_ticks = csw_TMGetTick();
   
 #ifdef DEBUG_TICKS  
   hal_HstSendEvent(SYS_EVENT,0x77700000 + voc_ticks - g_VoCHandleTicks);
 #endif
 
    BT_VoCRunFlag=0;
    if (g_BtCtx.voisTramePlayedNb == BT_VPP_UNMUTE_TRAME_NB)
    {
        // Unmute VPP
        vpp_SpeechBTAudioCfg((VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg);

    }

#ifdef BT_SCO_RECORD_SUPPORT
    if (g_BtCtx.voisTramePlayedNb == 10)
    {
		// Unmute VPP
		gScoRecordFlag = 1;
    }
#endif
    // FIXME: trick to only copy data after a encode finished interrupt
    {    
        // Copy the buffer to decode (Rx) and fetch from VPP's memory
        // the freshly encoded buffer (Tx).
        


		if(g_BtCtx.voisTramePlayedNb%2==0)
		{	   
		        hal_Speech_BtPopRxPushTx((UINT8*)g_BtCtx.voisDecInput+320, (UINT8*)g_BtCtx.voisEncOutput+320);

#ifdef BT_SCO_RECORD_SUPPORT
#if CHECK_SCO_DELAY == 1
			if(g_BtCtx.voisTramePlayedNb == 10||g_BtCtx.voisTramePlayedNb == 20||g_BtCtx.voisTramePlayedNb == 30)
			{
				memcpy((UINT8*)g_BtCtx.voisDecInput+320, ScoPlayTestData, 320);
			}
#endif
	            if(gScoRecordFlag >= 1)
	            {
	              ev.nParam1 = 0;
			      COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		      }    
#endif					
		}
		else
		{
		        hal_Speech_BtPopRxPushTx((UINT8*)g_BtCtx.voisDecInput, (UINT8*)g_BtCtx.voisEncOutput);		

#ifdef BT_SCO_RECORD_SUPPORT
#if CHECK_SCO_DELAY == 1
			if(g_BtCtx.voisTramePlayedNb == 11||g_BtCtx.voisTramePlayedNb == 13||g_BtCtx.voisTramePlayedNb == 31)
			{
				memcpy((UINT8*)g_BtCtx.voisDecInput, ScoPlayTestData, 320);
			}
#endif
	            if(gScoRecordFlag >= 1)
	            {
	               ev.nParam1 = 1;
			       COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		     }
#endif
		}

   //     memset(g_BtAgcPcmBufferReceiver, 0x91, 640);

		if (g_msdfReload)
		{
			vpp_SpeechBTMSDFReload();
			g_msdfReload = 0;
		}

		g_BtCtx.voisTramePlayedNb++;	
	}
    
}
// =============================================================================
// BTSco_Setup
// -----------------------------------------------------------------------------
/// Configure the vois service.. 
/// 
/// This functions configures the audio interface for the speech stream:
/// gain for the side tone and the speaker, input selection for the microphone
/// and output selection for the speaker... It also configures the decoding
/// parameters like audio cancellation. (cf #VOIS_AUDIO_CFG_T for exhaustive
/// details).
/// 
/// DO NOT CALL THAT FUNCTION BEFORE #vois_start.
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #VOIS_AUDIO_CFG_T for more details.
/// @return #VOIS_ERR_NO it can execute the configuration.
// =============================================================================

PUBLIC VOIS_ERR_T BTSco_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg)
{
    AUD_ERR_T audErr = AUD_ERR_NO;
    AUD_LEVEL_T audioCfg;
    VPP_SPEECH_AUDIO_CFG_T vppCfg;
    UINT32 btScoTimeOut = 0;
    MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_Setup itf=%d", itf);

    hal_HstSendEvent(SYS_EVENT, 0x19890100);

      // FIXME : All calibration to be checked
 //   CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
    //dbg_TraceOutputText(0 , "BTSco_Setup");

    // Check that VOIS is running
    if (g_BtCtx.voisItf == AUD_ITF_NONE || !g_BtCtx.voisStarted)
    {
        // Ignore this call
	//dbg_TraceOutputText(0 , "Ignore this call");
	    hal_HstSendEvent(SYS_EVENT, 0x19890101);
	    hal_HstSendEvent(SYS_EVENT, g_BtCtx.voisItf);
	    hal_HstSendEvent(SYS_EVENT, g_BtCtx.voisStarted);

        return VOIS_ERR_NO;
    }
	btScoTimeOut = csw_TMGetTick();
    while (BT_VoCRunFlag==0);
    {   
        //COS_Sleep(1);
        if(csw_TMGetTick() - btScoTimeOut > 5000)//about 300 ms
        {
            hal_HstSendEvent(SYS_EVENT, 0x19890103);
            return VOIS_ERR_RESOURCE_TIMEOUT;
        }
    }
    hal_HstSendEvent(SYS_EVENT, 0x19890105);
    hal_HstSendEvent(SYS_EVENT, csw_TMGetTick() - btScoTimeOut);

    btScoTimeOut = csw_TMGetTick();
    while (BT_VoCRunFlag==1 && g_BtCtx.voisStarted==TRUE)
    {
        //COS_Sleep(1);
        if(csw_TMGetTick() - btScoTimeOut > 500)//about 30 ms
        {
            hal_HstSendEvent(SYS_EVENT, 0x19890104);
            return VOIS_ERR_RESOURCE_TIMEOUT;
        }    
    }
    hal_HstSendEvent(SYS_EVENT, 0x19890106);
    hal_HstSendEvent(SYS_EVENT, csw_TMGetTick() - btScoTimeOut);
  //  BT_SpkLevel=cfg->spkLevel;
	
    BT_AudInterface=itf;

    BT_AudPath = itf; //- AUD_ITF_BT_EP;
    
    // Record the last applied configuration
    *(VOIS_MAP_AUDIO_CFG_T*)&g_BtCtx.voisLatestCfg =*(VOIS_MAP_AUDIO_CFG_T*)cfg;
    
    // Those parameters are not calibration dependent.
    g_BtCtx.voisVppCfg.encMute = cfg->encMute;
    g_BtCtx.voisVppCfg.decMute = cfg->decMute;

    // disable push-to-talk
    g_BtCtx.voisVppCfg.if1 = 0; 

    // Set audio interface 
    g_BtCtx.voisAudioCfg.spkLevel = cfg->spkLevel;
    g_BtCtx.voisAudioCfg.micLevel = cfg->micLevel;
    g_BtCtx.voisAudioCfg.sideLevel = cfg->sideLevel;
    g_BtCtx.voisAudioCfg.toneLevel = cfg->toneLevel;
   
    // get calibration params for Vpp Speech. Need to be called after setting g_BtCtx 
    BTSco_SetCalibratedVppCfg(itf, (VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg);

    // If the interface is different, we stop and resume the streams
    // to apply the interface change.
    if (itf != g_BtCtx.voisItf)
    {
        // Stop the stream.
        audErr = aud_StreamStop(g_BtCtx.voisItf);
        g_BtCtx.voisItf = itf;
		

        // Set the DAC to use 
        audioStream.voiceQuality = 
#ifdef VOIS_USE_STEREO_DAC_FOR_LOUDSPEAKER
                        // use stereo DAC if the mode is loudspeaker 
                        (itf == AUD_ITF_LOUD_SPEAKER) ? FALSE :
#endif
#ifdef VOIS_USE_STEREO_DAC_FOR_EARPIECE
                        // use stereo DAC if the mode is earpiece 
                        (itf == AUD_ITF_EAR_PIECE) ?   FALSE :
#endif
                        // default 
                        TRUE;

        // Enforce temporary muting and progressive level resume by the 
        // speech handler.
        g_BtCtx.voisTramePlayedNb = 1;
        
        // Set audio interface, initially muted.
        *(AUD_LEVEL_T*)&audioCfg = *(AUD_LEVEL_T*)&g_BtCtx.voisAudioCfg;
        audioCfg.spkLevel   = cfg->spkLevel;
        audioCfg.micLevel   = cfg->micLevel;
        audioCfg.toneLevel  = cfg->sideLevel;
        audioCfg.sideLevel  = cfg->toneLevel;




        // Set the VPP config, initially muted.
        *(VPP_SPEECH_AUDIO_CFG_T*)&vppCfg = *(VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg;
        vppCfg.encMute = VPP_SPEECH_MUTE;
        vppCfg.decMute = VPP_SPEECH_MUTE;
	
        vpp_SpeechBTAudioCfg(&vppCfg);

	vpp_SpeechBTReset();


        if (audErr == AUD_ERR_NO)
        {
            // configure AUD and VPP buffers
            audErr = aud_StreamStart(itf, &audioStream, &audioCfg);
        }
        
        if (audErr == AUD_ERR_NO)
        {
            // Start the other flux only if the Rx one is started
            audErr = aud_StreamRecord(itf, &micStream, &audioCfg);
        }
    }
    else
    {
        // Just apply the configuration change on the same
        // interface.
        vpp_SpeechBTAudioCfg((VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg);
        audErr = aud_Setup(itf, (AUD_LEVEL_T*)&g_BtCtx.voisAudioCfg);
    }
	//Set Mic Gain in VoC

    if (cfg->encMute == VPP_SPEECH_MUTE)
        vpp_SpeechBTSetInAlgGain(CALIB_AUDIO_GAIN_VALUE_MUTE);
    else
	    vpp_SpeechBTSetInAlgGain(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc.tx.gain);
#ifdef USE_OLD_AEC_NS

    vpp_SpeechBTSetAecEnableFlag(calibPtr->aud.audioVoc[calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AecEnableFlag,
                           calibPtr->aud.audioVoc[calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AgcEnableFlag);

	//typedef struct
	//{
	//    INT8                           AecEnbleFlag;                 //0x00000000
	//    INT8                           AgcEnbleFlag;                 //0x00000001
	//    INT8                           StrongEchoFlag;               //0x00000002
	//    INT8                           NoiseGainLimit;               //0x00000003
	//    INT8                           NoiseMin;                     //0x00000004
	//    INT8                           NoiseGainLimitStep;           //0x00000005
	//    INT8                           AmpThr;                       //0x00000006
	//    UINT8                          Flags;                        //0x00000007
	//} CALIB_AUDIO_PARAMS_T; //Size : 0x08
	
    //#define DEFAULT_CALIB_AUDIO_PARAMS_CONFIG0  {
	//	1, 0, 0, 127, 0, 8, 127, 28}
    //#define DEFAULT_CALIB_ECHO_CANCEL_CONFIG0  {
	//	0, 0, 0, 0, 0, 0, 0}
    vpp_SpeechBTSetAecPara(127, 0, 8, 127, 0);	

	vpp_SpeechBTSetFilterPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf.flag,
                           FALSE,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.tx.flag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.rx.flag);
    //CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //pOutGains = &calibPtr->aud.audioGains[calibPtr->aud.audioGainItfMap[itf]].outGains[cfg->spkLevel];
    //vpp_SpeechBTSetOutAlgGain(pOutGains->voiceOrEpAlg);
#else
    vpp_SpeechBTSetAecEnableFlag(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AecEnableFlag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AgcEnableFlag);

    vpp_SpeechBTSetAecPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseGainLimit,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseMin,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseGainLimitStep,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AmpThr,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.StrongEchoFlag);

    vpp_SpeechBTSetFilterPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf.flag,
                           FALSE,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.tx.flag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.rx.flag);
                           
    vpp_SpeechBTSetNoiseSuppressParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex));
    vpp_SpeechBTSetNonClipParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc));
    vpp_SpeechBTSetAecAllPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec));
	vpp_SpeechBTSetScaleDigitalGainPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].gain), 0);
	vpp_SpeechBTSetHpfPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf));
	vpp_SpeechBTSetEQPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].eq));
	vpp_SpeechBTSetMSDFPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].fir));
	vpp_SpeechBTSetWebRtcAgcParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].webrtcagc));
	vpp_SpeechBTSetInAlgGain(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc.tx.gain);
	vpp_SpeechBTSetOutAlgGain(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc.rx.gain);	
    vpp_SpeechBTSetProcessMicEnableFlag(FALSE);
	

#endif
    //dbg_TraceOutputText(0 , "SETUP Gain=%d", pOutGains->voiceOrEpAlg);

//    //dbg_TraceOutputText(0 , "BT_VoCRunFlag=%d", BT_VoCRunFlag);

#ifndef VOC_INTR_STEP    
    hal_VocIntrOpen(Wakeup_Mask_Status);
#endif
	//dbg_TraceOutputText(0 , "BTSco_Setup RET=%dl" ,audErr);

    switch (audErr)
    {
        // TODO Add error as they come

        case AUD_ERR_NO:

            return VOIS_ERR_NO;
            break;

        default:

            return audErr; // ...unknown error ...
            break;
    }
}





VOID BTSco_txhandle_half(VOID)
{
#ifdef DEBUG_TICKS
    int ticks = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT, 0x55500000 +  ticks - g_VoCHandleTicks);
    g_VoCHandleTicks = ticks;
#endif

#if DEBUG_SCO == 1
	memcpy(g_BtRecordMicPcmBuffer, ScoPlayTestData, 320);
#endif
   
	vpp_SpeechScheduleOneFrame();
	BT_VoCRunFlag=1;
	return;
}

VOID BTSco_txhandle_end(VOID)
{
#ifdef DEBUG_TICKS
    int ticks = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT, 0x66600000 +  ticks - g_VoCHandleTicks);
    g_VoCHandleTicks = ticks;
#endif

#if DEBUG_SCO == 1
    memcpy((u_int8 *)g_BtRecordMicPcmBuffer+320, ScoPlayTestData, 320);
#endif

   vpp_SpeechScheduleOneFrame();
   BT_VoCRunFlag=1;
   return;
}

extern VOID hal_abbSetPaOpenMode(BOOL on);


// =============================================================================
// BTSco_Start
// -----------------------------------------------------------------------------
/// Start the VOIS service. 
/// 
/// This function records from the mic and outputs sound on the speaker
/// on the audio interface specified as a parameter, using the input and output
/// set by the \c parameter. \n
///
/// @param itf Interface providing the audion input and output.
/// @param cfg The configuration set applied to the audio interface
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command, or if VPP is unavailable.
///         #VOIS_ERR_NO it can execute the command.
// =============================================================================

PUBLIC VOIS_ERR_T BTSco_Start(
                        CONST AUD_ITF_T      itf,
                        CONST VOIS_AUDIO_CFG_T* cfg)
{
    //VOIS_PROFILE_FUNCTION_ENTRY(BTSco_Start);
    //dbg_TraceOutputText(0, "BTSco_Start");
    MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_Start itf=%d", itf);
    hal_HstSendEvent(SYS_EVENT, 0x19820103);
    // Initial audio confguration
    AUD_LEVEL_T audioCfg;
    VPP_SPEECH_AUDIO_CFG_T vppCfg;
    AUD_ERR_T audErr;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#if (USE_XCPU_CVSD == 1)
    hal_SysRequestFreq(HAL_SYS_FREQ_VOIS, HAL_SYS_FREQ_144M, NULL);
#else
    hal_SysRequestFreq(HAL_SYS_FREQ_VOIS, HAL_SYS_FREQ_78M, NULL);
#endif
#else
    hal_SysRequestFreq(HAL_SYS_FREQ_VOIS, HAL_SYS_FREQ_104M, NULL);
#endif
	//set cpu freqence. use HAL_SYS_FREQ_AVPS temporary
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_104M);
 //   CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
    
    //ReceivedScoDataCache = (UINT16*)pcmbuf_overlay;//[MMC_SCO_BUFF_SIZE/2];
    //MicData = (UINT16*)(pcmbuf_overlay+MMC_SCO_BUFF_SIZE/4);//[MMC_SCO_BUFF_SIZE/2];
#ifdef HAL_OVERLAY
    if (calib_bb_audio[itf].audioParams.AecEnbleFlag)
    {
        if (hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_5) != HAL_ERR_NO)
        {
            //VOIS_PROFILE_FUNCTION_EXIT(BTSco_Start);
          //  dbg_TraceOutputText(0, "VOIS_ERR_RESOURCE_BUSY");

            return VOIS_ERR_RESOURCE_BUSY;
        }
        g_BtOverlayLoaded = TRUE;
    }
#endif	

#ifdef BT_SCO_RECORD_SUPPORT
#if 0
	gDumpScoCurFd = FS_Open(DUMP_SCO_NAME, FS_O_RDWR|FS_O_CREAT|FS_O_TRUNC, 0);
#endif
    gDumpScoCurFd = 0x2000;   //init block;
	hal_HstSendEvent(SYS_EVENT,gDumpScoCurFd);

	dump_data_buf = COS_Malloc_NoCache(1024);
	
	if(gDumpScoCurFd < 0 || dump_data_buf == NULL)
	{
	    gScoRecordFlag = 0;
		hal_HstSendEvent(SYS_EVENT,0x89000003);
	}
	else
	{		
		hal_HstSendEvent(SYS_EVENT,0x89000004);
	}
    ScoMcdOpen();
#endif	

  //  BT_AGCEnbleFlag = FALSE;
   // BT_SpkLevel=cfg->spkLevel;
    BT_AudInterface=itf;
    BT_AudPath = itf; //- AUD_ITF_BT_EP;

    // Record the last applied configuration
    *(VOIS_MAP_AUDIO_CFG_T*)&g_BtCtx.voisLatestCfg =*(VOIS_MAP_AUDIO_CFG_T*)cfg;
    hal_HstSendEvent(SYS_EVENT, 0x19820107);
    // Configure the Speech FIFO
    //hal_Speech_BtFifoReset(pcmbuf_overlay+MMC_SCO_BUFF_SIZE/2);
    hal_Speech_BtFifoReset(pBtSpeechFifoOverlay);
    // initialise the vpp buffer
    vpp_SpeechInitBuffer();
    
    g_BtCtx.voisVppCfg.encMute = cfg->encMute;
    g_BtCtx.voisVppCfg.decMute = cfg->decMute;
    g_BtCtx.fileHandle = -1;
    
    // disable push-to-talk
    g_BtCtx.voisVppCfg.if1 = 0; 

    // Set audio interface 
    g_BtCtx.voisAudioCfg.spkLevel = cfg->spkLevel;
    g_BtCtx.voisAudioCfg.micLevel = cfg->micLevel;
    g_BtCtx.voisAudioCfg.sideLevel = cfg->sideLevel;
    g_BtCtx.voisAudioCfg.toneLevel = cfg->toneLevel;
    hal_HstSendEvent(SYS_EVENT, 0x19820108);
    // get calibration params for Vpp Speech. Need to be called after setting g_BtCtx 
    BTSco_SetCalibratedVppCfg(itf, (VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg);
    // Register global var
    g_BtCtx.voisItf = itf;

    g_BtCtx.voisTramePlayedNb = 0;
    
    // Set audio interface, initially muted
    *(AUD_LEVEL_T*)&audioCfg = *(AUD_LEVEL_T*)&g_BtCtx.voisAudioCfg;
    audioCfg.spkLevel   =  cfg->spkLevel;
    audioCfg.micLevel   =  cfg->micLevel;
    audioCfg.toneLevel  =  cfg->sideLevel;
    audioCfg.sideLevel  =  cfg->toneLevel;

    // Set the VPP config
    *(VPP_SPEECH_AUDIO_CFG_T*)&vppCfg = *(VPP_SPEECH_AUDIO_CFG_T*)&g_BtCtx.voisVppCfg;
    vppCfg.encMute = VPP_SPEECH_MUTE;
    vppCfg.decMute = VPP_SPEECH_MUTE;
    hal_HstSendEvent(SYS_EVENT, 0x19820109);

    g_BtAgcPcmBufferReceiver=(UINT8 *)((UINT32)( vpp_SpeechBTGetRxPcmBuffer())|0x20000000);//(UINT8 *)vpp_SpeechBTGetRxPcmBuffer();
    g_BtRecordMicPcmBuffer=(UINT32 *)((UINT32)(vpp_SpeechBTGetTxPcmBuffer())|0x20000000);//(UINT8 *)vpp_SpeechBTGetTxPcmBuffer();



    //g_BtBufferStart=(UINT32 *)COS_SHMEMMALLOC(50*160*2);
    //BTSco_RecordStart(g_BtBufferStart,50*160*2,Vois_test);

    // Typical global vars
    // Those buffer are written by VoC, thus the need to access
    // them the uncached way.
    g_BtCtx.voisEncOutput = (HAL_SPEECH_BT_ENC_OUT_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)vpp_SpeechBTGetBluetoothRxPcmBuffer());
    g_BtCtx.voisDecInput  = (HAL_SPEECH_BT_DEC_IN_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)vpp_SpeechBTGetBluetoothTxPcmBuffer());
    hal_HstSendEvent(SYS_EVENT, 0x1982010a);

    // PCM Audio Tx stream, output of VPP

    audioStream.startAddress = (UINT32*)g_BtAgcPcmBufferReceiver;
    audioStream.length = MMC_SCO_BUFF_SIZE*2;
    audioStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    audioStream.channelNb = HAL_AIF_MONO;
    audioStream.voiceQuality = TRUE;
    audioStream.playSyncWithRecord = TRUE;

    audioStream.halfHandler =  NULL ; // Mechanical interaction with VPP's VOC
    audioStream.endHandler =  NULL ; // Mechanical interaction with VPP's VOC


    micStream.startAddress = (UINT32*)g_BtRecordMicPcmBuffer;
    micStream.length = MMC_SCO_BUFF_SIZE*2;
    micStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    micStream.channelNb = HAL_AIF_MONO;
    micStream.voiceQuality = TRUE;
    micStream.playSyncWithRecord = TRUE;

    micStream.halfHandler =  BTSco_txhandle_half ; // Mechanical interaction with VPP's VOC
    micStream.endHandler =  BTSco_txhandle_end ; // Mechanical interaction with VPP's VOC_AHB_BCPU_DEBUG_IRQ_CAUSE
    hal_HstSendEvent(SYS_EVENT, 0x1982010b);

    // set vpp
    BT_VoCRunFlag = 0;
    UINT32 spStatus =    vpp_SpeechBTOpen(BTSco_VppSpeechHandler, ( VPP_SPEECH_WAKEUP_SW_DECENC ));

    hal_HstSendEvent(SYS_EVENT, 0x19820104);
    hal_HstSendEvent(SYS_EVENT, spStatus);

    //dbg_TraceOutputText(0 , "VPP_SPEECH_OPEN STATUS = %08X", spStatus);
#ifdef HAL_OVERLAY
    if (spStatus == HAL_ERR_RESOURCE_BUSY)
    {
        if (g_BtOverlayLoaded)
        {
            hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_5);
            g_BtOverlayLoaded = FALSE;
        }
        //VOIS_PROFILE_FUNCTION_EXIT(BTSco_Start);
        return VOIS_ERR_RESOURCE_BUSY;
    }
#endif	
    vpp_SpeechBTAudioCfg(&vppCfg);
    
#ifdef USE_OLD_AEC_NS
    vpp_SpeechBTSetInAlgGain(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall.alg);
	
    vpp_SpeechBTSetAecEnableFlag(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AecEnableFlag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AgcEnableFlag);

    vpp_SpeechBTSetAecPara(127, 0, 8, 127, 0);	
	
	vpp_SpeechBTSetFilterPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf.flag,
                           FALSE,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.tx.flag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.rx.flag);
	//CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //pOutGains = &calibPtr->aud.audioGains[calibPtr->aud.audioGainItfMap[itf]].outGains[cfg->spkLevel];
    //vpp_SpeechBTSetOutAlgGain(pOutGains->voiceOrEpAlg);
#else

    vpp_SpeechBTSetAecEnableFlag((UINT8)calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AecEnableFlag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AgcEnableFlag);

    vpp_SpeechBTSetAecPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseGainLimit,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseMin,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.NoiseGainLimitStep,
		                   calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.AmpThr,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec.StrongEchoFlag);

    vpp_SpeechBTSetFilterPara(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf.flag,
                           FALSE,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.tx.flag,
                           calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex.rx.flag);
                           
    vpp_SpeechBTSetNoiseSuppressParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].speex));
    vpp_SpeechBTSetNonClipParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc));
    vpp_SpeechBTSetAecAllPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].aec));
	vpp_SpeechBTSetScaleDigitalGainPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].gain), 0);
	vpp_SpeechBTSetHpfPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].hpf));
	vpp_SpeechBTSetEQPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].eq));
	vpp_SpeechBTSetMSDFPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].fir));
	vpp_SpeechBTSetWebRtcAgcParaAll(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].webrtcagc));
	vpp_SpeechBTSetInAlgGain(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc.tx.gain);
	vpp_SpeechBTSetOutAlgGain(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[BT_AudPath]].agc.rx.gain);	
    vpp_SpeechBTSetProcessMicEnableFlag(FALSE);
#endif	

    hal_HstSendEvent(SYS_EVENT, 0x19820105);

    //dbg_TraceOutputText(0 , "Gain=%d", pOutGains->voiceOrEpAlg);

  //  vpp_SpeechBTSetMorphVoice(g_MorphVoiceEnableFlag,g_MorphVoicePara);

    hal_abbSetPaOpenMode(FALSE);
    // configure AUD and VPP buffers
    audErr = aud_StreamStart(itf, &audioStream, &audioCfg);

    if (audErr == AUD_ERR_NO)
    {
        // Start the other flux only if the Rx one is started
        audErr = aud_StreamRecord(itf, &micStream, &audioCfg);
    }
            //dbg_TraceOutputText(0, "BTSco_Start ret=%d",audErr);

    switch (audErr)
    {
        case AUD_ERR_NO:
            break; 

        default:
            vpp_SpeechBTClose();
#ifdef HAL_OVERLAY
            if (g_BtOverlayLoaded)
            {
                hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_5);
                g_BtOverlayLoaded = FALSE;
            }
#endif
            //VOIS_PROFILE_FUNCTION_EXIT(BTSco_Start);
            //dbg_TraceOutputText(0 , "VOIS Start Failed !!!");
            return VOIS_ERR_UNKNOWN;
            break; // :)
    }
    hal_HstSendEvent(SYS_EVENT, 0x19820106);
#ifdef SUPPORT_SCO_PLC
    gPlcInitFlag = 0;
#endif
#if (USE_XCPU_CVSD == 1)
    CvsdInit();
#endif
#ifdef BTSCO_MALLOC_MEM
	ReceivedScoDataCache = (UINT16 *)mmc_MemMalloc_BssRam(MMC_SCO_BUFF_SIZE*2);
	MicData = (UINT32 *)mmc_MemMalloc_BssRam(MMC_SCO_MIC_BUFF_SIZE);
	ScoSendData = (INT16 *)mmc_MemMalloc_BssRam(MMC_SCO_PACKET_MAX_SIZE*2);
#endif

    //VOIS_PROFILE_FUNCTION_EXIT(BTSco_Start);
    g_BtCtx.voisStarted = TRUE;
    return VOIS_ERR_NO;
}

BOOL BTSco_GetStatus(void)
{
	return g_BtCtx.voisStarted;
}

// =============================================================================
// BTSco_Stop
// -----------------------------------------------------------------------------
/// This function stops the VOIS service.
/// If the function returns 
/// #VOIS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #VOIS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC VOIS_ERR_T BTSco_Stop(VOID)
{
    //VOIS_PROFILE_FUNCTION_ENTRY(BTSco_Stop);
    hal_HstSendEvent(SYS_EVENT,0x1982010f);
    hal_HstSendEvent(SYS_EVENT,g_BtCtx.voisStarted);

    MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_Stop");
    hal_SysRequestFreq(HAL_SYS_FREQ_VOIS, HAL_SYS_FREQ_32K, NULL);
    //csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    hal_SysSetVocClockDefault();
    if (g_BtCtx.voisStarted)
    {
        hal_HstSendEvent(SYS_EVENT,0x1982013f);

        g_BtCtx.voisStarted = FALSE;
        
        // stop stream ...
        aud_StreamStop(g_BtCtx.voisItf);
        vpp_SpeechBTClose();
#ifdef HAL_OVERLAY
        if (g_BtOverlayLoaded)
        {
            hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_5);
            g_BtOverlayLoaded = FALSE;
        }
#endif
    	BT_VoCRunFlag=0;

        // and reset global state and cl
        g_BtCtx.voisItf = AUD_ITF_NONE;

        g_BtCtx.voisTramePlayedNb = 0;

        g_BtCtx.voisAudioCfg.spkLevel = AUD_SPK_MUTE;
        g_BtCtx.voisAudioCfg.micLevel = AUD_MIC_MUTE;
        g_BtCtx.voisAudioCfg.sideLevel = 0;
        g_BtCtx.voisAudioCfg.toneLevel = 0;

        g_BtCtx.voisVppCfg.echoEsOn = 0;
        g_BtCtx.voisVppCfg.echoEsVad = 0;
        g_BtCtx.voisVppCfg.echoEsDtd = 0;
        g_BtCtx.voisVppCfg.echoExpRel = 0;
        g_BtCtx.voisVppCfg.echoExpMu = 0;
        g_BtCtx.voisVppCfg.echoExpMin = 0;
        g_BtCtx.voisVppCfg.encMute = VPP_SPEECH_MUTE;
        g_BtCtx.voisVppCfg.decMute = VPP_SPEECH_MUTE;
        g_BtCtx.voisVppCfg.sdf = NULL;
        g_BtCtx.voisVppCfg.mdf = NULL;
        g_BtCtx.voisVppCfg.if1 = 0;
        hal_Speech_BtFifoReset(pBtSpeechFifoOverlay);

        // PCM Audio Tx stream, output of VPP
        audioStream.startAddress = NULL;
        audioStream.length = 0;
        audioStream.sampleRate = HAL_AIF_FREQ_8000HZ;
        audioStream.channelNb = HAL_AIF_MONO;
        audioStream.voiceQuality = TRUE;
        audioStream.playSyncWithRecord = FALSE;//TRUE;
        audioStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
        audioStream.endHandler = NULL; // Mechanical interaction with VPP's VOC

        // PCM Audio Rx stream, input of VPP
        micStream.startAddress = NULL;
        micStream.length = 0;
        micStream.sampleRate = HAL_AIF_FREQ_8000HZ;
        micStream.channelNb = HAL_AIF_MONO;
        micStream.voiceQuality = TRUE;
        micStream.playSyncWithRecord = FALSE;//TRUE;
        micStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
        micStream.endHandler = NULL; // Mechanical interaction with VPP's VOC
    }
	
	MMC_ScoDataReadyPlay = FALSE;
	ReceiveDataLen=0;
	
    if(g_BtRecordFlag == TRUE)
    {
        if (g_BtUserHandler)
        {
            g_BtUserHandler(VOIS_STATUS_NO_MORE_DATA);
        }               
    }
	
#ifdef BTSCO_MALLOC_MEM
	if(ReceivedScoDataCache)
	{
		mmc_MemFreeAll_BssRam(ReceivedScoDataCache);
		ReceivedScoDataCache = NULL;
	}
	if(MicData)
	{
		mmc_MemFreeAll_BssRam(MicData);
		MicData = NULL;
	}
	if(ScoSendData)
	{
		mmc_MemFreeAll_BssRam(ScoSendData);
		ScoSendData = NULL;
	}
#endif
#ifdef SUPPORT_SCO_PLC
        PlcDeInit();
        gPlcInitFlag = 0;
#endif
  //  COS_SHMEMFREE(g_BtBufferStart);
  //  BTSco_RecordStop();

  #ifdef BT_SCO_RECORD_SUPPORT
	hal_HstSendEvent(SYS_EVENT,0x89000005);
	hal_HstSendEvent(SYS_EVENT,gDumpScoCurFd);
    gScoRecordFlag = 0;
	while(gScoWriteFlag ==1)
	{
		COS_Sleep( 10);
	}
	if(gDumpScoCurFd >= 0)
	{
  		//FS_Close(gDumpScoCurFd);
	}
	if(dump_data_buf != NULL)
	{
		COS_Free(dump_data_buf);
	}
	ScoMcdClose();
#endif	

	ReceiveDataLen = 0;
	SendDataLen = 0;
	MMC_ScoDataReadyPlay = FALSE;
    //VOIS_PROFILE_FUNCTION_EXIT(BTSco_Stop);
    return VOIS_ERR_NO;
}



// =============================================================================
// BTSco_CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update Vois related values depending on calibration parameters.
/// 
/// @return #VOIS_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #VOIS_ERR_NO otherwise
// =============================================================================
PUBLIC VOIS_ERR_T BTSco_CalibUpdateValues(VOID)
{
    // Only relevant would VoiS be started
        //dbg_TraceOutputText(0 , "BTSco_CalibUpdateValues g_BtCtx.voisStarted = %d",g_BtCtx.voisStarted);

    if (g_BtCtx.voisStarted)
    {
        return BTSco_Setup(g_BtCtx.voisItf, (VOIS_AUDIO_CFG_T*)&g_BtCtx.voisLatestCfg);
        // As BTSco_Setup does a aud_Setup, all settings 
        // will be updated from the possibly new calibration
        // values.
    }
    else
    {
        return VOIS_ERR_NO;
    }
}

// =============================================================================
// BTSco_RecordStart
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOIS_ERR_T BTSco_RecordStart( INT32 file)
{
    MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_RecordStart, file=%d", file);

    g_BtCtx.fileHandle = file;
  
    return VOIS_ERR_NO;
}

// =============================================================================
// BTSco_RecordStop
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOIS_ERR_T BTSco_RecordStop( VOID)
{
    MCI_TRACE(MCI_AUDIO_TRC, 0, "BTSco_RecordStop");

    //dbg_TraceOutputText(0 , "BTSco_RecordStop");
    return VOIS_ERR_NO;
}

#ifdef BT_SCO_RECORD_SUPPORT

UINT32	g_WriteTicks = 0;
VOID BTSco_WriteData(INT16 *pMicData, INT16 *pSpkData, INT16 *pResData)
{
       UINT32 write_tick1, write_tick2;
       UINT32 i;
	//write_tick1 = csw_TMGetTick();
	//hal_HstSendEvent(SYS_EVENT, 0x11100000 +  write_tick1 - g_WriteTicks);
	//g_WriteTicks = write_tick1;
		
       if(gScoRecordFlag == 1)
       {

	       memcpy(dump_data_buf, pMicData, 320);
		memcpy(dump_data_buf+320, pSpkData, 320);
		memcpy(dump_data_buf+640, pResData, 320);

		//pcm_data = (INT8*)pSpkData;
		//mic_data = (INT8*)pMicData;

		//for(i=0;i<512;i++)
		// {
		//     *(UINT16*)(dump_data_buf+2*i) = gDumpScoCurFd - 0x2000;
		// }
        gScoWriteFlag = 1;
		//FS_Write(gDumpScoCurFd, dump_data_buf, 960);
		test_mcd_Write(gDumpScoCurFd++, dump_data_buf, 512);
		test_mcd_Write(gDumpScoCurFd++, dump_data_buf+512, 512);
	    gScoWriteFlag = 0;
	}
	//write_tick2 = csw_TMGetTick();
	//hal_HstSendEvent(SYS_EVENT, 0x22200000 +  write_tick2 - write_tick1);
}
#endif

VOID MMC_SCOSendData(INT16 *pdu, UINT16 length)
{
#if COMBINED_HOST==0
    rdabt_uart_tx_sco_data(pdu, BTSco_AudFileHandle(), length);
#else
    COS_EVENT event = {0};
    memcpy(ScoSendData, pdu, length);
    ScoSendMsg.data = ScoSendData;
    ScoSendMsg.length = length;
    ScoSendMsg.handle =  BTSco_AudFileHandle();
    event.nEventId = RDABT_SCO_DATA_RES;
    event.nParam1 = (UINT32)&ScoSendMsg;
    rdabt_send_msg_to_bt(&event);
#endif
}

INT16 *MMC_SCOGetRXBuffer(UINT8 flag)
{
     ScoReceiveDataFlag = flag;
     return (int16 *)(ScoRcvData);
}

VOID MMC_SCOReceiveData(INT32 length)
{
    uint32 sco_tick;//, plc_tick;
    static uint32 plc_count;
		
    if(!g_BtCtx.voisStarted)
    {
        hal_HstSendEvent(SYS_EVENT,0x20161336);
#if (USE_XCPU_CVSD == 0)
		memset((UINT8*)MicData, SPEECH_BT_FILL_INVALID_DATA,sizeof(MicData));
        MMC_SCOSendData((int16*)MicData, length);
#else
        memset((UINT8*)MicData, 0x00,sizeof(MicData));
		memset((UINT8*)ScoSendData, SPEECH_BT_FILL_INVALID_DATA,sizeof(ScoSendData));
		MMC_SCOSendData((int16*)ScoSendData, length);
#endif
        SendDataLen = 0;
        return;

    }
	if (length>120) hal_DbgAssert("BTSco_ReceiveData length overflow");
	sco_tick = csw_TMGetTick();
#ifdef DEBUG_TICKS
    hal_HstSendEvent(SYS_EVENT,0x99900000 + sco_tick - g_ScoTicks);
    g_ScoTicks = sco_tick;
#endif

	//MCI_TRACE (MCI_AUDIO_TRC,0, "[BTSCO]BTSco_ReceiveData_lenght = %d,paket_flag = %d",length,ScoReceiveDataFlag);
	if (length!=30 && length!=60 && length!=120)
	{
		hal_HstSendEvent(SYS_EVENT,0x20161337);
		hal_HstSendEvent(SYS_EVENT,HistoryScoReceiveDataLen);
		hal_HstSendEvent(SYS_EVENT,length);

	    if(length ==0)
		{
            length = HistoryScoReceiveDataLen;
            memset(ScoRcvData,SPEECH_BT_FILL_INVALID_DATA,length);
		}
		else
		{
		    length = HistoryScoReceiveDataLen;
		    memset((UINT8*)MicData, SPEECH_BT_FILL_INVALID_DATA,sizeof(MicData));
			memset(ReceivedScoDataCache,SPEECH_BT_FILL_INVALID_DATA,sizeof(ReceivedScoDataCache));
			MMC_SCOSendData((int16*)MicData, length);

			return;
		}
	}
	else if(HistoryScoReceiveDataLen != length)
	{
           HistoryScoReceiveDataLen = length;
	}
#ifndef USE_VOC_CVSD
#if DEBUG_SCO == 1
    memcpy(ScoRcvData, ScoPlayTestData, length);
#endif
#endif
#if 0 //gen lost packet num.
    {
        static UINT8 lost_gen_num = 0;
        static UINT8 lost_packet_num = 10;
        static UINT8 lost_packet_count = 0;

        lost_gen_num++;
        if(lost_gen_num >= 50)
        {
            if(lost_packet_count <= lost_packet_num)
            {
                lost_packet_count++;
            }
            else
            {
               lost_packet_num--;
               lost_gen_num = 0;
               lost_packet_count = 0;
               if(lost_packet_num <= 0)
               {
                    lost_packet_num = 10;
               }
            }
            
            ScoReceiveDataFlag = 0x10;
        }
        else
        {
            ScoReceiveDataFlag = 0x00;
        }

    }
#endif
	switch(ScoReceiveDataFlag)
	{
		case 0x00 : 
		   break;
		case 0x10 : 
		case 0x20 :
		case 0x01 : 
		case 0x02 :  
		    plc_count++;
		    length = HistoryScoReceiveDataLen;
		    memset(ScoRcvData,SPEECH_BT_FILL_INVALID_DATA,length);
		    MCI_TRACE (MCI_AUDIO_TRC,0, "[BTSCO]BTSco_ReceiveData_lenght = %d,paket_flag = %d",length,ScoReceiveDataFlag);

		    //hal_HstSendEvent(SYS_EVENT,0x89000000);
		    break;

		default : break;
	}

//caculate PLC rate
#if 1
	if((sco_tick - g_PlcTicks) > 16384)
	{
		 float plc_rate;
#ifdef USE_VOC_CVSD
               plc_rate = plc_count*100/(8000.0/length);
	        MCI_TRACE (MCI_AUDIO_TRC,0, "[BTSCO]PLC count = %d  PLC rate = %d%",plc_count,(INT32)plc_rate);
#else
               plc_rate = plc_count*100/(16000.0/length);
	        MCI_TRACE (MCI_AUDIO_TRC,0, "[BTSCO]PLC count = %d  PLC rate = %d%",plc_count,(INT32)plc_rate);
#endif


		 hal_HstSendEvent(SYS_EVENT,0x8900000a);
		 hal_HstSendEvent(SYS_EVENT,plc_count);

		 plc_count = 0;
	   	 g_PlcTicks = sco_tick;
	}
#endif	

    
#ifdef USE_VOC_CVSD
//process lost data;
	{
		int i = 0;
		for(i = 0;i<length/2;i++)
		{
			if(0x00 != *(ScoRcvData+i))
			{
				break;
			}
		}
		if(i == length/2)
		{
			memset(ScoRcvData,0x55,length);
			//hal_HstSendEvent(SYS_EVENT,0x89000001);
		}
	}

#if (USE_XCPU_CVSD == 1)
#if 0
    if(!ScoReceiveDataFlag)
	{
	    UINT32 dec_tick1, dec_tick2;
	    dec_tick1 = csw_TMGetTick();
		CVSDDecode(ScoRcvData, ReceivedScoDataCache+ReceiveDataLen/2, length);
	    dec_tick2 = csw_TMGetTick();
        hal_HstSendEvent(SYS_EVENT,0xddd00000 + dec_tick2 - dec_tick1);

#ifdef USE_HFP_CVSD
		CvsdHighPassFilter(ReceivedScoDataCache+ReceiveDataLen/2, length, CvsdDecHighPassHistoryX, CvsdDecHighPassHistoryY);
#endif
	}
	else
	{
		UINT32 dec_tick1, dec_tick2;
	    dec_tick1 = csw_TMGetTick();
		CVSDDecode(ScoRcvData, ReceivedScoDataCache+ReceiveDataLen/2, 10);
	    dec_tick2 = csw_TMGetTick();
        hal_HstSendEvent(SYS_EVENT,0xddd00000 + dec_tick2 - dec_tick1);

#ifdef USE_HFP_CVSD
		CvsdHighPassFilter(ReceivedScoDataCache+ReceiveDataLen/2, length, CvsdDecHighPassHistoryX, CvsdDecHighPassHistoryY);
#endif
		memset(ReceivedScoDataCache+ReceiveDataLen/2, 0x00, length*2);
		hal_HstSendEvent(SYS_EVENT,0xfff00000);
	}
#else
    {
	    //UINT32 dec_tick1, dec_tick2;
	    //dec_tick1 = csw_TMGetTick();
#ifdef SUPPORT_SCO_PLC
        if(!ScoReceiveDataFlag)
        {
            CVSDDecode(ScoRcvData, ReceivedScoDataCache+ReceiveDataLen/2, length);
        }
        else
        {
            CvsdInit();
        }
#else
		CVSDDecode(ScoRcvData, ReceivedScoDataCache+ReceiveDataLen/2, length);
#endif
	    //dec_tick2 = csw_TMGetTick();
        //hal_HstSendEvent(SYS_EVENT,0xddd00000 + dec_tick2 - dec_tick1);

#ifdef USE_HFP_CVSD
		CvsdHighPassFilter(ReceivedScoDataCache+ReceiveDataLen/2, length, CvsdDecHighPassHistoryX, CvsdDecHighPassHistoryY);
#endif
    }
#endif
#ifdef SUPPORT_SCO_PLC	

    if(gPlcInitFlag == 0)
    {
    	PlcInit(length);  // length is cvsd, plc length is sample.
    	gPlcInitFlag = 1;
    	hal_HstSendEvent(SYS_EVENT,0x89000006);
    }
    
    if(!ScoReceiveDataFlag)
    {
        addtohistory(ReceivedScoDataCache+ReceiveDataLen/2);
    }
    else
    {	
        //UINT32 plc_tick1, plc_tick2;
	    //plc_tick1 = csw_TMGetTick();
		dofe(ReceivedScoDataCache+ReceiveDataLen/2);
	    //plc_tick2 = csw_TMGetTick();

	    //hal_HstSendEvent(SYS_EVENT,0xccc00000 + plc_tick2 - plc_tick1);
    }
#endif
    ReceiveDataLen += length*2;//yh
	if(ReceiveDataLen >=MMC_SCO_BUFF_SIZE)
	{
		hal_Speech_BtPushRxPopTx((CONST UINT8* )ReceivedScoDataCache,(UINT8*)MicData+SendDataLen, MMC_SCO_BUFF_SIZE);
		SendDataLen+=MMC_SCO_BUFF_SIZE;


		memset(ReceivedScoDataCache,0x0,MMC_SCO_BUFF_SIZE);
		memcpy(ReceivedScoDataCache,ReceivedScoDataCache+MMC_SCO_BUFF_SIZE/2,ReceiveDataLen - MMC_SCO_BUFF_SIZE);
		memset(ReceivedScoDataCache+MMC_SCO_BUFF_SIZE/2,0x0,ReceiveDataLen - MMC_SCO_BUFF_SIZE);
		ReceiveDataLen-=MMC_SCO_BUFF_SIZE;

		//  MMC_AudioSCO_AsynSendReq((int16*)MicData, BTSco_AudFileHandle(), MMC_SCO_BUFF_SIZE);

	}

    if(MMC_ScoMicDataReadySend == FALSE)// just for 5851s;
	{
		if(SendDataLen>=MMC_SCO_SEND_BUFF_SIZE)
		{
			int i;
			for(i=0;i<3;i++)
			{
				MMC_SCOSendData((int16*)MicData, length);
				memcpy((UINT8*)MicData,((UINT8*)MicData)+length,SendDataLen-length);
				memset(((UINT8*)MicData)+SendDataLen-length,0x00,length);
				SendDataLen-=length;
			}
			MMC_ScoMicDataReadySend = TRUE;
		}
	}else if(SendDataLen>length*2){
	
#ifdef USE_HFP_CVSD
	    CvsdHighPassFilter(MicData,60,CvsdEncHighPassHistoryX, CvsdEncHighPassHistoryY);
#endif
        {
        	//UINT32 enc_tick1, enc_tick2;
	        //enc_tick1 = csw_TMGetTick();
            CVSDEncode(MicData, ScoSendData,length);
            //enc_tick2 = csw_TMGetTick();
            //hal_HstSendEvent(SYS_EVENT,0xeee00000 + enc_tick2 - enc_tick1);
        }
        if(g_BtCtx.voisTramePlayedNb < 8)
        {
            memset((UINT8*)ScoSendData, SPEECH_BT_FILL_INVALID_DATA,sizeof(ScoSendData));
        }
		MMC_SCOSendData((int16*)ScoSendData, length);
		memcpy((UINT8*)MicData,((UINT8*)MicData)+length*2,SendDataLen-length*2);
		memset(((UINT8*)MicData)+SendDataLen-length*2,0x00,length*2);
		SendDataLen-=length*2;


	}
	else//data not enough
	{
	    hal_HstSendEvent(SYS_EVENT, 0x20161339);
	    hal_HstSendEvent(SYS_EVENT, SendDataLen);
	    hal_HstSendEvent(SYS_EVENT, g_BtCtx.voisTramePlayedNb);
        hal_HstSendEvent(SYS_EVENT, length);
        memset((UINT8*)ScoSendData, SPEECH_BT_FILL_INVALID_DATA,sizeof(ScoSendData));
		MMC_SCOSendData((int16*)ScoSendData, length);
	}


	if(SendDataLen>= MMC_SCO_MIC_BUFF_SIZE)
	{
		//dbg_TraceOutputText(0,"    SendDataLen  error unknow");
		memset(MicData,0x55,MMC_SCO_MIC_BUFF_SIZE);
		SendDataLen=0;
	}
#if 0
	if(!MMC_ScoDataReadyPlay )
	{
		//dbg_TraceOutputText(0,"BTSco_ReceiveData    ready?=%d,len?%d",MMC_ScoDataReadyPlay,ReceiveDataLen);
		memset(ReceivedScoDataCache,0x55,MMC_SCO_BUFF_SIZE*2);
		MMC_ScoDataReadyPlay = TRUE;
	}
#endif
#else
	

	memcpy(ReceivedScoDataCache+ReceiveDataLen/2, ScoRcvData, length);
    ReceiveDataLen+=length;//yh
	if(ReceiveDataLen >= MMC_SCO_BUFF_SIZE/2)
	{
		hal_Speech_BtPushRxPopTx((CONST UINT8* )ReceivedScoDataCache,(UINT8*)MicData+SendDataLen, MMC_SCO_BUFF_SIZE);
		SendDataLen+=MMC_SCO_BUFF_SIZE/2;


		memset(ReceivedScoDataCache,0x55,MMC_SCO_BUFF_SIZE/2);
		memcpy(ReceivedScoDataCache,(UINT8*)ReceivedScoDataCache+MMC_SCO_BUFF_SIZE/2,ReceiveDataLen - MMC_SCO_BUFF_SIZE/2);
		memset((UINT8*)ReceivedScoDataCache+ReceiveDataLen - MMC_SCO_BUFF_SIZE/2,0x55,ReceiveDataLen - MMC_SCO_BUFF_SIZE/2);
		ReceiveDataLen-=MMC_SCO_BUFF_SIZE/2;
	}

	if(MMC_ScoMicDataReadySend == FALSE)// just for 5851s;
	{
		if(SendDataLen>=MMC_SCO_SEND_BUFF_SIZE)
		{
			int i;
			for(i=0;i<3;i++)
			{
				MMC_SCOSendData((int16*)MicData, length);
				memcpy((UINT8*)MicData,((UINT8*)MicData)+length,SendDataLen-length);
				memset(((UINT8*)MicData)+SendDataLen-length,0x55,length);
				SendDataLen-=length;
			}
			MMC_ScoMicDataReadySend = TRUE;
		}
	}else if(SendDataLen>length){
        if(g_BtCtx.voisTramePlayedNb < 8)
        {
            memset((UINT8*)MicData, SPEECH_BT_FILL_INVALID_DATA,length);
        }
		MMC_SCOSendData((int16*)MicData, length);
		memcpy((UINT8*)MicData,((UINT8*)MicData)+length,SendDataLen-length);
		memset(((UINT8*)MicData)+SendDataLen-length,0x55,length);
		SendDataLen-=length;


	}
	else//data not enough
	{
	    hal_HstSendEvent(SYS_EVENT, 0x20161339);
	    hal_HstSendEvent(SYS_EVENT, SendDataLen);
        hal_HstSendEvent(SYS_EVENT, length);
	    memset(((UINT8*)MicData), 0x55,length);
		MMC_SCOSendData((int16*)MicData, length);
	}


	if(SendDataLen>= MMC_SCO_MIC_BUFF_SIZE)
	{
		//dbg_TraceOutputText(0,"    SendDataLen  error unknow");
		memset(MicData,0x55,MMC_SCO_MIC_BUFF_SIZE);
		SendDataLen=0;
	}

	if(!MMC_ScoDataReadyPlay )
	{
		//dbg_TraceOutputText(0,"BTSco_ReceiveData    ready?=%d,len?%d",MMC_ScoDataReadyPlay,ReceiveDataLen);
		memset(ReceivedScoDataCache,0x55,MMC_SCO_BUFF_SIZE*2);
		MMC_ScoDataReadyPlay = TRUE;
	}

#endif



#else

//process plc;
#ifdef SUPPORT_SCO_PLC
if(gPlcInitFlag == 0)
{
	PlcInit(length/2);
	gPlcInitFlag = 1;
	hal_HstSendEvent(SYS_EVENT,0x89000006);
}
	switch(ScoReceiveDataFlag)
	{
		case 0x00 : 
		   addtohistory(ScoRcvData);
		   break;
		case 0x10 : 
		case 0x20 : 
		case 0x01 : 
		case 0x02 : 
		    dofe(ScoRcvData);
		    //hal_HstSendEvent(SYS_EVENT,0x89000002);
		    break;

		default : break;
	}
#endif	
	memcpy(ReceivedScoDataCache+ReceiveDataLen/2, ScoRcvData, length);
    ReceiveDataLen+=length;//yh


	if(ReceiveDataLen >=MMC_SCO_BUFF_SIZE)
	{
		hal_Speech_BtPushRxPopTx((CONST UINT8* )ReceivedScoDataCache,(UINT8*)MicData+SendDataLen, MMC_SCO_BUFF_SIZE);
		SendDataLen+=MMC_SCO_BUFF_SIZE;


		memset(ReceivedScoDataCache,0x0,MMC_SCO_BUFF_SIZE);
		memcpy(ReceivedScoDataCache,ReceivedScoDataCache+MMC_SCO_BUFF_SIZE/2,ReceiveDataLen - MMC_SCO_BUFF_SIZE);
		memset(ReceivedScoDataCache+MMC_SCO_BUFF_SIZE/2,0x0,ReceiveDataLen - MMC_SCO_BUFF_SIZE);
		ReceiveDataLen-=MMC_SCO_BUFF_SIZE;

		//  MMC_AudioSCO_AsynSendReq((int16*)MicData, BTSco_AudFileHandle(), MMC_SCO_BUFF_SIZE);

	}
	//hal_Speech_BtPushRxPopTx((CONST UINT8* )ReceivedScoDataCache,(UINT8*)MicData, length);

    if(SendDataLen >= length )
	{
	   //need wait for micdata buff	  &move data forward 
	   //MMC_AudioSCO_AsynSendReq((int16*)MicDataToBT, BTSco_AudFileHandle(), length);
       if(g_BtCtx.voisTramePlayedNb < 8)
       {
           memset((UINT8*)MicData, SPEECH_BT_FILL_INVALID_DATA,length);
       }
	   MMC_SCOSendData((int16*)MicData, length);
	   memset(MicData,0x0,length);
	   memcpy(MicData,MicData+length/4,SendDataLen-length);
	   memset(MicData+SendDataLen/4-length/4,0x0,length);
	   SendDataLen-=length;
	   
	}
	else//data not enough
	{
	    hal_HstSendEvent(SYS_EVENT, 0x20161340);
	    hal_HstSendEvent(SYS_EVENT, SendDataLen);
	    memset(((UINT8*)MicData)+SendDataLen, 0x00,length);
		MMC_SCOSendData((int16*)MicData, length);
	}


	if(SendDataLen>= MMC_SCO_MIC_BUFF_SIZE)
	{
		//dbg_TraceOutputText(0,"    SendDataLen  error unknow");
		memset(MicData,0x0,MMC_SCO_MIC_BUFF_SIZE);
		SendDataLen=0;
	}

	if(!MMC_ScoDataReadyPlay )
	{
		//dbg_TraceOutputText(0,"BTSco_ReceiveData    ready?=%d,len?%d",MMC_ScoDataReadyPlay,ReceiveDataLen);
		memset(ReceivedScoDataCache,0x0,MMC_SCO_BUFF_SIZE*2);
		MMC_ScoDataReadyPlay = TRUE;
	}
#endif
}

PUBLIC VOIS_AUDIO_CFG_T* BTSco_GetAudioCfg(VOID)
{
	return (VOIS_AUDIO_CFG_T*)&g_BtCtx.voisLatestCfg;
}

PUBLIC VOID BTSco_SetMsdfReloadFlag(VOID)
{
    g_msdfReload = 1;
}


