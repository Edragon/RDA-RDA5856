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

#if defined(__AT_MOD_SPECIAL__)
#include "cs_types.h"
#include "chip_id.h"
#include "string.h"

//#include "calib_m.h"
//#include "calibp_defaults.h"
#include "calibp_debug.h"
//#include "calibp_calibration.h"

#include "tgt_calib_m.h"

//#include "pal_gsm.h"
#include "hal_mem_map.h"
#include "hal_dma.h"
#include "hal_host.h"

#ifdef CHIP_HAS_AP
#include "hal_ap_comm.h"
#include "syscmds_m.h"
#endif

#include "sxr_mem.h"
#include "sxr_tls.h"
#include "sxs_io.h"

#include "memd_m.h"

//#include "rfd_pa.h"
//#include "rfd_sw.h"
//#include "rfd_xcv.h"

#include "aud_snd_types.h"
#include "calibaud_m.h"
#include "aud_m.h"

extern UINT8 *SUL_Itoa(INT32 value, UINT8 *string, INT32 radix);
extern int atox(const char *s, int len);
UINT8 *pBuffer = NULL;

#define _PAL  0  

PRIVATE VOID AudioParamToATParam(UINT8 *pbuf, UINT8 *param, INT16 len)
{
    int i;
    UINT8 buf[3] = {0}, deslen;

    for(i = 0; i < len; i++)
    {
        memset(buf, 0, 3);
        SUL_Itoa(pbuf[i], buf, 16);
        deslen = strlen(buf);
        memcpy(param + (2 - deslen), buf, deslen);
        param = param + 2;
    }
}

PRIVATE VOID ATParamToAudioParam(UINT8 *param, UINT8 *pbuf, INT16 len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        *pbuf++ = atox(param, 2);
        param += 2;
    }
}
#if 0
PRIVATE VOID calib_SetAudioCodecOutGainsSame(AUD_ITF_T itf, INT8 ana, INT8 dac, BOOL isMusic)
{
    int i;

    CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

    if(isMusic)
    {
        for(i = CALIB_AUDIO_GAIN_QTY-2; i >= 0; i--)
        {
            //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->audio[itf].audioGains.outGains[i]);
			pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[i]);

            pOutGains->musicOrLsAna = ana;
            pOutGains->musicOrLsDac = dac;

            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music outGain: ana : %d, dac : %d\n",
                    pOutGains->musicOrLsAna, pOutGains->musicOrLsDac);          
        }
    }
    else
    {       
        for(i = CALIB_AUDIO_GAIN_QTY-2; i >= 0; i--)
        {
            //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[i]);
			pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[i]);

            pOutGains->voiceOrEpAna = ana;
            pOutGains->voiceOrEpDac = dac;
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set voice outGain: ana : %d, dac : %d\n",
                    pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);
        }
    }
}
#endif

PRIVATE VOID calib_SetAudioCodecOutGainsdecreased(AUD_ITF_T itf, INT8 ana, INT8 dac, BOOL isMusic)
{
    int i;

    CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
    INT16 audio_level;
	INT16 ana_count, dac_count;
    float ana_steps, dac_steps, ana_value, dac_value;
	
    ana_count = ana;
	dac_count = (dac + 26);

	audio_level = CALIB_AUDIO_GAIN_QTY;
	
	if(audio_level <= 7)
	{
		audio_level = 7;
	}
	
	ana_steps = (float)ana_count / (audio_level - 1);
	dac_steps = (float)dac_count / (audio_level - 1);

	ana_value = ana;
	dac_value = dac;

    //hal_HstSendEvent(SYS_EVENT, 0x20161410);
    //hal_HstSendEvent(SYS_EVENT, audio_level);

    if(isMusic)
    {
    	//hal_HstSendEvent(SYS_EVENT, 0x20161411);
        for(i = audio_level-1; i >= 0; i--)
        {
            //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[i]);
            pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[i]);

		    pOutGains->musicOrLsAna = (INT8)ana_value;
		    pOutGains->musicOrLsDac = (INT8)dac_value;

			//hal_HstSendEvent(SYS_EVENT, pOutGains->musicOrLsAna);
			//hal_HstSendEvent(SYS_EVENT, pOutGains->musicOrLsDac);
			
			if(ana_value - ana_steps >=0)
	              {
	            	        ana_value -= ana_steps; 
			}
			else if(i == 1)
			{
				ana_value = 0;
			}

			if(dac_value - dac_steps >= -26)
			{
				dac_value -= dac_steps;
			}
			else if(i == 1)
			{
				dac_value = -26;
			}
			
            //CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music outGain: ana : %d, dac : %d\n",
            //        pOutGains->musicOrLsAna, pOutGains->musicOrLsDac);

        }
    }
    else
    {
        //hal_HstSendEvent(SYS_EVENT, 0x20161422);
        for(i = audio_level-1; i >= 0; i--)
        {
		    //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[i]);
            pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[i]);

		    pOutGains->voiceOrEpAna = (INT8)ana_value;
		    pOutGains->voiceOrEpDac = (INT8)dac_value;

			//hal_HstSendEvent(SYS_EVENT, pOutGains->voiceOrEpAna);
			//hal_HstSendEvent(SYS_EVENT, pOutGains->voiceOrEpDac);
			
			if(ana_value - ana_steps >=0)
	              {
	            	        ana_value -= ana_steps; 
			}
			else if(i == 1)
			{
				ana_value = 0;
			}

			if(dac_value - dac_steps >= -26)
			{
				dac_value -= dac_steps;
			}
			else if(i == 1)
			{
				dac_value = -26;
			}
			


            //CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music outGain: ana : %d, dac : %d\n",
            //        pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);
        }
    }
}

// =============================================================================
// calib_SetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC VOID calib_SetAudioCodecIngain(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_IN_GAINS_T *pInGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
	CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

    //pInGains = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.inGainsCall);
	pInGains = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall);

    CALIB_ASSERT(4 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)pInGains, 2);

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set InGain: ana : %d, adc : %d\n",
                pInGains->ana, pInGains->adc);
}

// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC UINT32  calib_GetAudioCodecIngain(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_IN_GAINS_T *pInGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//pInGains = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.inGainsCall);
	pInGains = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsCall);

    memset(param, '0', 4);
    AudioParamToATParam((UINT8 *)pInGains, param, 2);

    return 4;
}


// =============================================================================
// calib_SetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC VOID calib_SetAudioCodecOutGains(AUD_ITF_T itf, UINT8 *param, UINT16 param_len, BOOL isMusic)
{
    CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
    #if 1
    //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[CALIB_AUDIO_GAIN_QTY-1]);
	pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[CALIB_AUDIO_GAIN_QTY-1]);

    CALIB_ASSERT(4 == param_len, "error param");

    if(itf == AUD_ITF_FM || itf == AUD_ITF_TV)
    {
        if (aud_GetCurOutputDevice() == AUD_ITF_EAR_PIECE)
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set voice outGain: ana : %d, dac : %d\n",
                    pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);
            calib_SetAudioCodecOutGainsdecreased(itf, pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac, FALSE);            
        }
        else
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains + 4, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music outGain: ana : %d, dac : %d\n",
                    pOutGains->musicOrLsAna, pOutGains->musicOrLsDac);
            calib_SetAudioCodecOutGainsdecreased(itf, pOutGains->musicOrLsAna, pOutGains->musicOrLsDac, TRUE);            
        }
    }
    else
    {
        if(isMusic)
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains + 4, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music outGain: ana : %d, dac : %d\n",
                    pOutGains->musicOrLsAna, pOutGains->musicOrLsDac);
            calib_SetAudioCodecOutGainsdecreased(itf, pOutGains->musicOrLsAna, pOutGains->musicOrLsDac, TRUE);
        }
        else
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set voice outGain: ana : %d, dac : %d\n",
                    pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);      
            calib_SetAudioCodecOutGainsdecreased(itf, pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac, FALSE);
        }
    }

    #else
    CALIB_ASSERT(4 * CALIB_AUDIO_GAIN_QTY == param_len, "error param");
    for(int i=0; i<CALIB_AUDIO_GAIN_QTY-1; i++)
    {
        pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[i]);

        if(isMusic)
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains + 4, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set voice outGain: ana : %d, dac : %d\n",
                pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);
        }
        else
        {
            ATParamToAudioParam(param, (UINT8 *)pOutGains, 2);
            CALIB_TRACE(_PAL | TSTDOUT, 0, "Set voice outGain: ana : %d, dac : %d\n",
                pOutGains->voiceOrEpAna, pOutGains->voiceOrEpDac);
        }
    }
    #endif
}

// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC UINT32 calib_GetAudioCodecOutgains(AUD_ITF_T itf, UINT8 *param, BOOL isMusic)
{
    CALIB_AUDIO_OUT_GAINS_T *pOutGains;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
    #if 1
    //pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)(&(calibPtr->bb->audio[itf].audioGains.outGains[CALIB_AUDIO_GAIN_QTY-1]));
    pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].outGains[CALIB_AUDIO_GAIN_QTY-1]);

    if(itf == AUD_ITF_FM || itf == AUD_ITF_TV)
    {
        if (aud_GetCurOutputDevice() == AUD_ITF_EAR_PIECE)
        {
            memset(param, '0', 4);
            AudioParamToATParam((UINT8 *)pOutGains, param, 2);

            return 4;
        }
        else
        {
            memset(param, '0', 4);
            AudioParamToATParam((UINT8 *)pOutGains + 4, param, 2);

            return 4;
        }
    }
    else
    {
        if(isMusic)
        {
            memset(param, '0', 4);
            AudioParamToATParam((UINT8 *)pOutGains + 4, param, 2);

            return 4;
        }
        else
        {
            memset(param, '0', 4);
            AudioParamToATParam((UINT8 *)pOutGains, param, 2);

            return 4;
        }
    }
    #else
    for(int i=0; i<CALIB_AUDIO_GAIN_QTY-1; i++)
    {
        pOutGains = (CALIB_AUDIO_OUT_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.outGains[i]);

        if(isMusic)
        {
            memset(param, '0',  4); 
            AudioParamToATParam((UINT8 *)pOutGains + 4, param, 2);
            }
        else
        {
            memset(param, '0', 4);
            AudioParamToATParam((UINT8 *)pOutGains, param, 2);
        }
    }

    return 4 * CALIB_AUDIO_GAIN_QTY;
    
    #endif
}

// =============================================================================
// calib_SetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC VOID calib_SetAudioCodecSidetone(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    INT8 *pSidetone;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
	//pSidetone = (INT8 *)(&(calibPtr->bb->audio[itf].audioGains.sideTone[CALIB_AUDIO_GAIN_QTY-1]));
	pSidetone = (INT8 *)(&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].sideTone[CALIB_AUDIO_GAIN_QTY-1]));


    CALIB_ASSERT(2 == param_len, "error param");
    ATParamToAudioParam(param,  (UINT8 *)pSidetone, 1);

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set sidetone: sidetone : %d\n", *pSidetone);
}

// =============================================================================
// calib_GetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC UINT32 calib_GetAudioCodecSidtone(AUD_ITF_T itf, UINT8 *param)
{
    UINT8 *pSidetone;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
	//pSidetone = (UINT8 *)(&(calibPtr->bb->audio[itf].audioGains.sideTone[CALIB_AUDIO_GAIN_QTY-1]));
	pSidetone = (UINT8 *)(&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].sideTone[CALIB_AUDIO_GAIN_QTY-1]));

    memset(param, '0', 2);
    AudioParamToATParam(pSidetone, param, 1);

    return 2;
}

// =============================================================================
// calib_SetAudioCodecParam
// -----------------------------------------------------------------------------
// for all the codec gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC VOID calib_SetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_IN_GAINS_T *pInGainsRecord;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//pInGainsRecord = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.inGainsRecord);
	pInGainsRecord = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsRecord);

    CALIB_ASSERT(4 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)pInGainsRecord, 2);

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set inGainsRecord: ana : %d, adc : %d\n",
                pInGainsRecord->ana, pInGainsRecord->adc);
}

// =============================================================================
// calib_GetAudioMusicInGainsRecord
// -----------------------------------------------------------------------------
// for all the  gains, we use INT8, but we use it as INT16
// =============================================================================
PUBLIC UINT32  calib_GetAudioMusicInGainsRecord(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_IN_GAINS_T *pInGainsRecord;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//pInGainsRecord = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->bb->audio[itf].audioGains.inGainsRecord);
	pInGainsRecord = (CALIB_AUDIO_IN_GAINS_T *)&(calibPtr->aud.audioGains[(UINT8)calibPtr->aud.audioGainItfMap[itf]].inGainsRecord);

    memset(param, '0', 4);
    AudioParamToATParam((UINT8 *)pInGainsRecord, param, 2);

    return 4;
}

// [num[3]:den[3]]
static INT16 HPFParamCoeffs[5][6] =
{
    {15979, -31959, 15979, 16384, 31949, -15585}, // 45Hz
    {15585, -31170, 15585, 16384, 31131, -14825}, // 90Hz
    {14825, -29650, 14825, 16384, 29501, -13415}, // 180Hz
    {14354, -28709, 14354, 16384, 28457, -12578}, // 238Hz
    {13868, -27736, 13868, 16384, 27348, -11741}, // 300Hz
};

PRIVATE int HPFFreqToIndex(INT16 freq)
{
    int index = -1;
    
    switch (freq)
    {
        case 45:
            index = 0;
            break;
        case 90:
            index = 1;
            break;
        case 180:
            index = 2;
            break;
        case 238:
            index = 3;
            break;
        case 300:
            index = 4;
            break;
        default:
            CALIB_TRACE(_PAL | TSTDOUT, 0, "INVALID FREQUENCY\n");
            break;
    }

    return index;
}

// =============================================================================
// calib_SetAudioVocHPFParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocHPFParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    int j, index;

    CALIB_AUDIO_IIR_HPF_T *hpf;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//hpf= &(calibPtr->audio_voc->voc[itf].hpf);
	hpf= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].hpf);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_IIR_HPF_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)hpf, sizeof( CALIB_AUDIO_IIR_HPF_T));

    // we use lut here to get hpf coeffs
    index = HPFFreqToIndex(hpf->band.freq);
    CALIB_ASSERT(index != -1, "invalid frequency");
    for (j = 0; j < 3; j++)
    {
        hpf->band.num[j] = HPFParamCoeffs[index][j];
        hpf->band.den[j] = HPFParamCoeffs[index][j + 3];
    }

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set HPF: flag : %d\n num : 0x%x 0x%x 0x%x, den : 0x%x 0x%x 0x%x\n",
                hpf->flag,
                hpf->band.num[0], hpf->band.num[1], hpf->band.num[2],
                hpf->band.den[0], hpf->band.den[1], hpf->band.den[2]);
}


// =============================================================================
// calib_GetAudioVocHPFParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocHPFParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_IIR_HPF_T *hpf;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//hpf = &(calibPtr->audio_voc->voc[itf].hpf);
	hpf= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].hpf);

    memset(param, '0', sizeof(CALIB_AUDIO_IIR_HPF_T) * 2);
    AudioParamToATParam((UINT8 *)hpf, param, sizeof(CALIB_AUDIO_IIR_HPF_T));

    return sizeof(CALIB_AUDIO_IIR_HPF_T) * 2;
}

// =============================================================================
// calib_SetAudioVocFIRTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocFIRTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    INT16 *filters;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
    CALIB_ASSERT((CALIB_VOC_MDF_QTY + 1) * 4  == param_len, "error param");
    //filters = &(calibPtr->audio_voc->voc[itf].fir.mdfflag);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.mdfflag);

    ATParamToAudioParam(param, (UINT8 *)filters, 2);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.mdfFilter[0]);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.mdfFilter[0]);

    ATParamToAudioParam(param + 4, (UINT8 *)filters, CALIB_VOC_MDF_QTY * 2);

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set FIR tx: mdf[0] : %x, mdf[1] : %x\n",
                filters[0], filters[CALIB_VOC_MDF_QTY - 1]);
}

// =============================================================================
// calib_GetAudioVocFIRTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32  calib_GetAudioVocFIRTXParam(AUD_ITF_T itf, UINT8 *param)
{
    INT16 *filters;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
    memset(param, '0', (CALIB_VOC_MDF_QTY + 1) * 4);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.mdfflag);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.mdfflag);

    AudioParamToATParam((UINT8 *)filters, param, 2);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.mdfFilter[0]);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.mdfFilter[0]);

    AudioParamToATParam((UINT8 *)filters, param + 4, CALIB_VOC_MDF_QTY * 2);

    return (CALIB_VOC_MDF_QTY + 1) * 4;
}

// =============================================================================
// calib_SetAudioVocFIRRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocFIRRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    UINT16 *filters;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
    CALIB_ASSERT((CALIB_VOC_SDF_QTY + 1) * 4 == param_len, "error param");
    //filters = &(calibPtr->audio_voc->voc[itf].fir.sdfflag);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.sdfflag);

    ATParamToAudioParam(param, (UINT8 *)filters, 2);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.sdfFilter[0]);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.sdfFilter[0]);

    ATParamToAudioParam(param + 4, (UINT8 *)filters, CALIB_VOC_SDF_QTY * 2);

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set FIR rx: sdf[0] : %x, sdf[1] : %x\n",
                filters[0], filters[CALIB_VOC_MDF_QTY - 1]);
}

// =============================================================================
// calib_GetAudioVocFIRRxParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32  calib_GetAudioVocFIRRXParam(AUD_ITF_T itf, UINT8 *param)
{
    UINT16 *filters;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
    memset(param, '0', (CALIB_VOC_SDF_QTY + 1) * 4);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.sdfflag);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.sdfflag);

    AudioParamToATParam((UINT8 *)filters, param, 2);
    //filters = &(calibPtr->audio_voc->voc[itf].fir.sdfFilter[0]);
	filters= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].fir.sdfFilter[0]);

    AudioParamToATParam((UINT8 *)filters, param + 4, CALIB_VOC_SDF_QTY * 2);

    return (CALIB_VOC_SDF_QTY + 1 ) * 4;
}

// =============================================================================
// calib_SetAudioVocAECParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAECParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_AEC_T *aec;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//aec = &(calibPtr->audio_voc->voc[itf].aec);
	aec= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].aec);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_VOC_AEC_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)aec, sizeof(CALIB_AUDIO_VOC_AEC_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set AEC: itf : %d, AecEnableFlag : %d,"
                "PFCLDTDThrRatio : %d, DelaySampleNum : %d\n",
                itf, aec->AecEnableFlag,
                aec->PFCLDTDThrRatio, aec->DelaySampleNum);
}

// =============================================================================
// calib_GetAudioVocAECParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocAECParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_AEC_T *aec;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//aec =&(calibPtr->audio_voc->voc[itf].aec);
	aec= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].aec);

    memset(param, '0', sizeof(CALIB_AUDIO_VOC_AEC_T) * 2);
    AudioParamToATParam((UINT8 *)aec, param, sizeof(CALIB_AUDIO_VOC_AEC_T));

    return sizeof(CALIB_AUDIO_VOC_AEC_T) * 2;
}

// =============================================================================
// =============================================================================
// calib_SetAudioVocAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAGCTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_AGC_PATH_T *agc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//agc = &(calibPtr->audio_voc->voc[itf].agc.tx);
	agc= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].agc.tx);

    CALIB_ASSERT(sizeof( CALIB_AUDIO_VOC_AGC_PATH_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)agc, sizeof(CALIB_AUDIO_VOC_AGC_PATH_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set agc tx: itf : %d\n flag : %d, smooth_threshold : %d,"
                "noise_threshold : %d, gain : %d\n",
                itf, agc->flag, agc->smooth_threshold, agc->noise_threshold, agc->gain);
}

// =============================================================================
// calib_GetAudioVocAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32 calib_GetAudioVocAGCTXParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_AGC_PATH_T *agc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//agc = &(calibPtr->audio_voc->voc[itf].agc.tx);
	agc= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].agc.tx);

    memset(param, '0', sizeof( CALIB_AUDIO_VOC_AGC_PATH_T) * 2);
    AudioParamToATParam((UINT8 *)agc, param, sizeof(CALIB_AUDIO_VOC_AGC_PATH_T));

    return sizeof(CALIB_AUDIO_VOC_AGC_PATH_T) * 2;
}

// =============================================================================
// calib_SetAudioVocAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocAGCRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_AGC_PATH_T *agc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//agc = &(calibPtr->audio_voc->voc[itf].agc.rx);
	agc= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].agc.rx);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_VOC_AGC_PATH_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)agc, sizeof( CALIB_AUDIO_VOC_AGC_PATH_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set agc rx: itf : %d\n flag : %d, smooth_threshold : %d,"
                "noise_threshold : %d, gain : %d\n",
                itf, agc->flag, agc->smooth_threshold, agc->noise_threshold, agc->gain);
}

// =============================================================================
// calib_GetAudioVocAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocAGCRXParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_AGC_PATH_T *agc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//agc = &(calibPtr->audio_voc->voc[itf].agc.rx);
	agc= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].agc.rx);

    memset(param, '0', sizeof( CALIB_AUDIO_VOC_AGC_PATH_T) * 2);
    AudioParamToATParam((UINT8 *)agc, param, sizeof(CALIB_AUDIO_VOC_AGC_PATH_T));

    return sizeof(CALIB_AUDIO_VOC_AGC_PATH_T) * 2;
}


// =============================================================================
// =============================================================================
// calib_SetAudioVocWebRtcAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocWebRtcAGCTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
	CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *webrtcagc;
	CALIB_CALIBRATION_T *calibPtr = tgt_GetCalibConfig();

	webrtcagc = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].webrtcagc.tx);

	CALIB_ASSERT(sizeof( CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2 == param_len, "error param");
	ATParamToAudioParam(param, (UINT8 *)webrtcagc, sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T));

	CALIB_TRACE(_PAL | TSTDOUT, 0, "Set webrtcagc tx: itf : %d\n flag : %d, upper_thr : %d,"
				"lower_thr : %d, decay_value : %d\n",
				itf, webrtcagc->enable, webrtcagc->upper_thr, webrtcagc->lower_thr, webrtcagc->decay_value);
}

// =============================================================================
// calib_GetAudioVocWebRtcAGCTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32 calib_GetAudioVocWebRtcAGCTXParam(AUD_ITF_T itf, UINT8 *param)
{
	CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *webrtcagc;
	CALIB_CALIBRATION_T *calibPtr = tgt_GetCalibConfig();

	webrtcagc = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].webrtcagc.tx);

	memset(param, '0', sizeof( CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2);
	AudioParamToATParam((UINT8 *)webrtcagc, param, sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T));

	return sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2;
}

// =============================================================================
// =============================================================================
// calib_SetAudioVocWebRtcAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocWebRtcAGCRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
	CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *webrtcagc;
	CALIB_CALIBRATION_T *calibPtr = tgt_GetCalibConfig();

	webrtcagc = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].webrtcagc.rx);

	CALIB_ASSERT(sizeof( CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2 == param_len, "error param");
	ATParamToAudioParam(param, (UINT8 *)webrtcagc, sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T));

	CALIB_TRACE(_PAL | TSTDOUT, 0, "Set webrtcagc rx: itf : %d\n flag : %d, upper_thr : %d,"
				"lower_thr : %d, decay_value : %d\n",
				itf, webrtcagc->enable, webrtcagc->upper_thr, webrtcagc->lower_thr, webrtcagc->decay_value);
}

// =============================================================================
// calib_GetAudioVocWebRtcAGCRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC  UINT32 calib_GetAudioVocWebRtcAGCRXParam(AUD_ITF_T itf, UINT8 *param)
{
	CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T *webrtcagc;
	CALIB_CALIBRATION_T *calibPtr = tgt_GetCalibConfig();

	webrtcagc = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].webrtcagc.rx);

	memset(param, '0', sizeof( CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2);
	AudioParamToATParam((UINT8 *)webrtcagc, param, sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T));

	return sizeof(CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T) * 2;
}

// =============================================================================
// calib_SetAudioVocNSTXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocNSTXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_SPEEX_PATH_T *speex;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//speex = &(calibPtr->audio_voc->voc[itf].speex.tx);
	speex= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].speex.tx);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)speex, sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set ns tx: itf : %d\n"
                "flag : %d, noise_suppress_default : %d,"
                "noise_probe_range : %d %d %d %d",
                itf, speex->flag, speex->noise_suppress_default,
                speex->noise_probe_range[0], speex->noise_probe_range[1],
                speex->noise_probe_range[2], speex->noise_probe_range[3]);
}
// =============================================================================
// calib_GetAudioVocNSTXParam
// ------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocNSTXParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_SPEEX_PATH_T *speex;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//speex = &(calibPtr->audio_voc->voc[itf].speex.tx);
	speex= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].speex.tx);

    memset(param, '0', sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2);
    AudioParamToATParam((UINT8 *)speex, param, sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T));

    return sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2;
}

// =============================================================================
// calib_SetAudioVocNSRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocNSRXParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_SPEEX_PATH_T *speex;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//speex = &(calibPtr->audio_voc->voc[itf].speex.rx);
	speex= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].speex.rx);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2  == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)speex, sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set ns rx: itf : %d\n"
                "flag : %d, noise_suppress_default : %d,"
                "noise_probe_range : %d %d %d %d",
                itf, speex->flag, speex->noise_suppress_default,
                speex->noise_probe_range[0], speex->noise_probe_range[1],
                speex->noise_probe_range[2], speex->noise_probe_range[3]);
}

// =============================================================================
// calib_GetAudioVocNSRXParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocNSRXParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_SPEEX_PATH_T *speex;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//speex = &(calibPtr->audio_voc->voc[itf].speex.rx);
	speex= &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].speex.rx);

    memset(param, '0', sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2);
    AudioParamToATParam((UINT8 *)speex, param, sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T));

    return sizeof(CALIB_AUDIO_VOC_SPEEX_PATH_T) * 2;
}


// =============================================================================
// calib_GetAudioVocEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocEQParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_CALL_IIR_EQ_T *eq;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//eq = &(calibPtr->audio_voc->voc[itf].eq);
	eq = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].eq);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_CALL_IIR_EQ_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)eq, sizeof(CALIB_AUDIO_CALL_IIR_EQ_T));
}

// =============================================================================
// calib_GetAudioVocEQParam
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocEQParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_CALL_IIR_EQ_T *eq;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//eq = &(calibPtr->audio_voc->voc[itf].eq);
	eq = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].eq);

    memset(param, '0', sizeof(CALIB_AUDIO_CALL_IIR_EQ_T) * 2);
    AudioParamToATParam((UINT8 *)eq, param, sizeof(CALIB_AUDIO_CALL_IIR_EQ_T));

    return sizeof(CALIB_AUDIO_CALL_IIR_EQ_T) * 2;
}


// =============================================================================
// calib_SetAudioVocDigGain
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioVocDigGain(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_VOC_DigGain_T *diggain;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//diggain = &(calibPtr->audio_voc->voc[itf].gain);
	diggain = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].gain);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_VOC_DigGain_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)diggain, sizeof(CALIB_AUDIO_VOC_DigGain_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "in_enable : %d, out_enable : %d,"
                "scal_in:0x%x, scal_out : 0x%x\n",
                diggain->in_enable, diggain->out_enable,
                diggain->scal_in, diggain->scal_out);
}


// =============================================================================
// calib_GetAudioVocDigGain
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioVocDigGain(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_VOC_DigGain_T *diggain;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//diggain = &(calibPtr->audio_voc->voc[itf].gain);
	diggain = &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[itf]].gain);

    memset(param, '0', sizeof(CALIB_AUDIO_VOC_DigGain_T) * 2);
    AudioParamToATParam((UINT8 *)diggain, param, sizeof(CALIB_AUDIO_VOC_DigGain_T));

    return sizeof(CALIB_AUDIO_VOC_DigGain_T) * 2;
}


// =============================================================================
// calib_SetAudioMusicEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicEQParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_IIR_EQ_T *eq;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
	
	//eq =  &(calibPtr->audio_music->music[itf].eq);
	eq = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[itf]].eq);

	hal_HstSendEvent(SYS_EVENT, 0x20161472);

	hal_HstSendEvent(SYS_EVENT, itf);
	hal_HstSendEvent(SYS_EVENT, (UINT32)eq);
	
	hal_HstSendEvent(SYS_EVENT, sizeof(CALIB_AUDIO_IIR_EQ_T));
	hal_HstSendEvent(SYS_EVENT, param_len);
    //CALIB_ASSERT(sizeof(CALIB_AUDIO_IIR_EQ_T) == param_len, "error param");


       ATParamToAudioParam(param, (UINT8 *)eq, sizeof(CALIB_AUDIO_IIR_EQ_T));
#if 0
	hal_HstSendEvent(SYS_EVENT, eq->flag);

	hal_HstSendEvent(SYS_EVENT, eq->band[0].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[0].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[0].type);

	hal_HstSendEvent(SYS_EVENT, eq->band[1].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[1].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[1].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[2].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[2].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[2].type);

	hal_HstSendEvent(SYS_EVENT, eq->band[3].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[3].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[3].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[4].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[4].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[4].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[5].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[5].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[5].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[6].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[6].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[6].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[7].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[7].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[7].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[8].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[8].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[8].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[9].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[9].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[9].type);
#endif
    //CALIB_TRACE(_PAL | TSTDOUT, 0, "Set music eq: itf : %d, flag : %d\n"
    //            "band[0].num : 0x%x 0x%x 0x%x, band[0].den : 0x%x 0x%x 0x%x\n",
    //            itf, eq->flag,
    //            eq->band[0].num[0], eq->band[0].num[1], eq->band[0].num[2],
    //            eq->band[0].den[0], eq->band[0].den[1], eq->band[0].den[2]);
}

// =============================================================================
// calib_GetAudioMusicEQParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioMusicEQParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_IIR_EQ_T *eq;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//eq = &(calibPtr->audio_music->music[itf].eq);
	eq = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[itf]].eq);

	hal_HstSendEvent(SYS_EVENT, 0x20161700);
	hal_HstSendEvent(SYS_EVENT, sizeof(CALIB_AUDIO_IIR_EQ_T));

	hal_HstSendEvent(SYS_EVENT, eq->flag);

	hal_HstSendEvent(SYS_EVENT, eq->band[0].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[0].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[0].type);

	hal_HstSendEvent(SYS_EVENT, eq->band[1].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[1].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[1].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[2].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[2].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[2].type);

	hal_HstSendEvent(SYS_EVENT, eq->band[3].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[3].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[3].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[4].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[4].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[4].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[5].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[5].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[5].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[6].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[6].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[6].type);

	hal_HstSendEvent(SYS_EVENT, eq->band[7].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[7].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[7].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[8].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[8].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[8].type);
	
	hal_HstSendEvent(SYS_EVENT, eq->band[9].freq);
	hal_HstSendEvent(SYS_EVENT, eq->band[9].gain);
	hal_HstSendEvent(SYS_EVENT, eq->band[9].type);
	
    memset(param, '0', sizeof(CALIB_AUDIO_IIR_EQ_T) * 2);
    AudioParamToATParam((UINT8 *)eq, param, sizeof(CALIB_AUDIO_IIR_EQ_T));

    return sizeof(CALIB_AUDIO_IIR_EQ_T) * 2;
}

// =============================================================================
// calib_SetAudioMusicALCParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID calib_SetAudioMusicALCParam(AUD_ITF_T itf, UINT8 *param, UINT16 param_len)
{
    CALIB_AUDIO_MUSIC_DRC_T *drc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//drc =  &(calibPtr->audio_music->music[itf].drc);
	drc = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[itf]].drc);

    CALIB_ASSERT(sizeof(CALIB_AUDIO_MUSIC_DRC_T) * 2 == param_len, "error param");
    ATParamToAudioParam(param, (UINT8 *)drc, sizeof(CALIB_AUDIO_MUSIC_DRC_T));

    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set Music DRC: itf : %d\n"
                "drc->alc_enable = %d, drc->target = %d\n",
                itf, drc->alc_enable, drc->Thr_dB);
}

// =============================================================================
// calib_GetAudioMusicALCParam
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetAudioMusicALCParam(AUD_ITF_T itf, UINT8 *param)
{
    CALIB_AUDIO_MUSIC_DRC_T *drc;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

	//drc = &(calibPtr->audio_music->music[itf].drc);
	drc = &(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[itf]].drc);

    memset(param, '0', sizeof(CALIB_AUDIO_MUSIC_DRC_T) * 2);
    AudioParamToATParam((UINT8 *)drc, param, sizeof(CALIB_AUDIO_MUSIC_DRC_T));

    return sizeof(CALIB_AUDIO_MUSIC_DRC_T) * 2;
}

// =============================================================================
// calib_SetBufferData
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC BOOL calib_SetBufferData(UINT16 offset, UINT16 length, UINT8 *param)
{
    extern  CALIB_CALIBRATION_T  g_tgtCalibCfg; 

    if(offset == 0)
    {
	 hal_HstSendEvent(SYS_EVENT, 0x2017ca1b);
	 hal_HstSendEvent(SYS_EVENT, sizeof(CALIB_BUFFER_T));

        memset(&g_tgtCalibCfg.aud, 0x00, sizeof(CALIB_BUFFER_T) );
    }

    ATParamToAudioParam(param, (UINT8*)&g_tgtCalibCfg.aud + offset, length);

    if((offset + length) == sizeof(CALIB_BUFFER_T))
    {
        //calib_UpdateBufferData((CALIB_BUFFER_T*)&g_tgtCalibCfg.aud);
        hal_HstSendEvent(SYS_EVENT, 0x2017ca2b);

        if(!calib_BurnFlash())
        {
            hal_HstSendEvent(SYS_EVENT, 0x2017ca3b);
            return FALSE;
        }
    }

    return TRUE;
}

// =============================================================================
// calib_GetBufferData
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT32 calib_GetBufferData(UINT16 offset, UINT16 length, UINT8 *param)
{
    CONST CALIB_BUFFER_T *buffer;
    //CALIB_CALIBRATION_T *calibPtr = calib_GetPointers();
    CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

#if 0
    CALIB_TRACE(_PAL | TSTDOUT, 0, "Set Music DRC: itf : %d\n"
                "drc->alc_enable = %d, drc->target = %d\n",
                itf, drc->alc_enable, drc->Thr_dB);
#endif

    buffer = (CONST CALIB_BUFFER_T *)calibPtr->flash;

    hal_HstSendEvent(SYS_EVENT, 0x2017ca1c);
    hal_HstSendEvent(SYS_EVENT, length);
    hal_HstSendEvent(SYS_EVENT, offset);
    
    hal_HstSendEvent(SYS_EVENT, (UINT32)buffer);

    memset(param, '0', length * 2);
    AudioParamToATParam((UINT8 *)buffer + offset, param, length);
	
    hal_HstSendEvent(SYS_EVENT, 0x2017ca1d);

    return length * 2;
}

// =============================================================================
// calib_GetBufferPointers
//---------------------------------------------------------------------------
// =============================================================================
PUBLIC UINT8* calib_GetBufferPointers(VOID)
{
    return (pBuffer);
}

#endif

