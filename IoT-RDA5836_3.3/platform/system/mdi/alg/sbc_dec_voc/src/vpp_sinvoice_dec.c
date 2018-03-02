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


#include "vpp_sinvoice_dec.h"
#include "mcip_debug.h"

//#define SINVOICE_DEBUG_TRACE

#define SAMPLE_48k      48000
#define SAMPLE_44_1k    44100

#define CHAR_BUFFER_SIZE     200

#define IndexToHex(n) (((n)-6)/3) 
//#define ABS(n)        ((n)<0 ? (-n):(n))

#define STATE_REG_STEP1  1
#define STATE_REG_STEP2  2

#define SPACE_SIN_THRESHOID    2000
#define SPACE_SIN_DURATION     1*SAMPLE_48k/1000
#define SIN_VOICE_DURATION_NUM 10

#define SIN_BEGAIN_FRE    600
#define SIN_END_FRE       800

#define SIN_BEGAIN_HEX    IndexToHex(SAMPLE_48k/SIN_BEGAIN_FRE)
#define SIN_END_HEX       IndexToHex(SAMPLE_48k/SIN_END_FRE)

#define SIN_VOICE_PCM_BUFFER_LEN   (SIN_VOICE_PCM_VOC_HANDLER_NUM*1112)
#define MIN_SIN_VOICE_REG_BEGAIN_LEN   (SIN_VOICE_PCM_VOC_HANDLER_NUM*1024)


UINT8 gPlaySinVoiceStream = 0;

INT32 gSampleRate = SAMPLE_48k;

static INT32 CODE_BEGAIN = SIN_BEGAIN_FRE;
//static INT32 CODE_END = SIN_END_FRE;



INT32 gSamplingPointCount = 0;
BOOL gIsStartCounting = FALSE;
INT32 gStep;
BOOL gIsBeginning = FALSE;
BOOL gStartingDet = FALSE;
INT32 gStartingDetCount;

INT32 mRegValue;
INT32 mRegIndex;
INT32 mRegCount;
INT32 mPreRegCircle;
BOOL gIsRegStart = FALSE;
 
static INT32 gMax_Sample_Point_Count;
static INT32 gMin_Reg_Circle_Count;
static INT32 gMin_Reg_Pcm_Sample_Len;

INT32 gCollatorBufferLen = 0;

ProcessSinVoiceResult processSinVoiceResult = {0, NULL, SinVoiceRegStateInit};
INT8 gCharOutputBuffer[3][CHAR_BUFFER_SIZE] = {{0,},{0,},{0,}};
ProcessSinVoiceRegState gProcessSinVoiceRegState = ProcessSinVoiceRegStateNeedInit;
SinVoiceRegState gSinVoiceRegState = SinVoiceRegStateInit;

INT8 gSinVoiceRegBegainFlag = 0; 


void HexToAscii(INT8* buffer,INT8* data,INT32 len)
{
	INT32 i = 0;
	for(i = 0;i<len/2; i++)
	{
		*(buffer+i) = *(data + 2*i)*0x10 + *(data + 2*i + 1);	
#ifdef SINVOICE_DEBUG_TRACE
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SIN_VOICE] HexToAscii: %c",*(buffer+i));
#endif
	}
	
}

INT32 checkSinSpace(INT32 sinValue, INT32 *pSinSpaceCount)
{
    INT32 ret;
	if( ABS(sinValue) < SPACE_SIN_THRESHOID)
	{
		sinValue = 0;
		(*pSinSpaceCount)++;
		if(*pSinSpaceCount >= (SPACE_SIN_DURATION/2))
		{
			*pSinSpaceCount = 0;
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		*pSinSpaceCount = 0;
		ret = 0;
	}
	
	return ret;
}
INT32 preReg(INT32 samplingPointCount) 
{
	if((samplingPointCount <= 4) || (samplingPointCount >= 82) || (samplingPointCount >= 53 && samplingPointCount <= 58))
	{
		samplingPointCount = 0;
	}
	return samplingPointCount;
}
INT32 reg(INT32 samplingPointCount, INT8 *pDecDuffer) 
{
   static INT32 samplingPointCountAll = 0;
   INT8 ret = 0;
   if (!gIsBeginning) 
   {
	   if (!gStartingDet) 
	   {
		   if ((samplingPointCount <= gMax_Sample_Point_Count + 1) && (samplingPointCount >= gMax_Sample_Point_Count - 2)) //check begain
		   {
			   gStartingDet = TRUE;
			   gStartingDetCount = 0;
		   }
	   }
	   else 
	   {
		   if ((samplingPointCount <= gMax_Sample_Point_Count + 1) && (samplingPointCount >= gMax_Sample_Point_Count - 2)) //check begain
		   {
			   ++gStartingDetCount;
			   if (gStartingDetCount >= gMin_Reg_Circle_Count) 
			   {
				   gIsBeginning = TRUE;
				   gIsRegStart = FALSE;
				   mRegCount = 0;
				   ret = 2;
			   }
		   } 
		   else 
		   {
			   gStartingDet = FALSE;
		   }
	   }
   } 
   else 
   {
	   if (!gIsRegStart) 
	   {
		   if (samplingPointCount > 0) 
		   {
			   mRegValue = samplingPointCount;
			   samplingPointCountAll = samplingPointCount;
			   gIsRegStart = TRUE;
			   mRegCount = 1;
		   }
	   } 
	   else 
	   	{
		   if ((samplingPointCount >= mRegValue - 2) && (samplingPointCount <= mRegValue + 2)) 
		   {
			   ++mRegCount;
			   samplingPointCountAll += samplingPointCount;
			   mRegValue = samplingPointCountAll / mRegCount;
			   if (mRegCount >= gMin_Reg_Circle_Count) 
			   	{
				   // ok
				   if ((mRegValue <= mPreRegCircle - 1) || (mRegValue >= mPreRegCircle + 1)) 
				   {
					   if (NULL != pDecDuffer) 
					   {

						   *pDecDuffer = IndexToHex((samplingPointCountAll + mRegCount) / mRegCount);
						   ret = 1;
					   }
					   mPreRegCircle = mRegValue;
				   }
				   gIsRegStart = FALSE;
			   }
		   } 
		   else 
		   {
			   gIsRegStart = FALSE;
		   }
	   }
   }
   return ret;
}

static INT32 PcmConsumedCount = 0;
INT32 SinVoiceProcess(INT16 *decBuffer, INT32 decBufferLen, INT8 *decOutIndex, INT32 *consumed) 
{
	INT32 size = 0;
	INT16 sh = 0;
	INT32 outPutLen = 0;
	INT32 waitSpaceFlag = 0;
    INT32 sinSpaceCount = 0;

	size = decBufferLen;
	for (INT32 i = 0; i < size; i++) 
	{
	  PcmConsumedCount++;
	  sh = decBuffer[i];
      *consumed = i + 1;

	  if(waitSpaceFlag)
	  {
		if(checkSinSpace(sh, &sinSpaceCount))
		{
			waitSpaceFlag = 0;
			mPreRegCircle = 0;
			gIsStartCounting = FALSE;
			gProcessSinVoiceRegState = ProcessSinVoiceRegStateCheckSpaceOk;
#ifdef SINVOICE_DEBUG_TRACE
		       MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SIN_VOICE] check space ok!");
#endif
			return outPutLen;
		
		}
	  }
	  if (!gIsStartCounting) 
	  {
		  if (STATE_REG_STEP1 == gStep)
		  {
			  if (sh < 0) 
			  {
				  gStep = STATE_REG_STEP2;
			  }
		  }
		  else if (STATE_REG_STEP2 == gStep) 
		  {
			  if (sh > 0) 
			  {
				  gIsStartCounting = TRUE;
				  gSamplingPointCount = 0;
				  gStep = STATE_REG_STEP1;
			  }
		  }
	  } 
	  else if(waitSpaceFlag == 0)
	  {
		  ++gSamplingPointCount;
		  if (STATE_REG_STEP1 == gStep) 
		  {
			  if (sh < 0) 
			  {
				  gStep = STATE_REG_STEP2;
			  }
		  } 
		  else if (STATE_REG_STEP2 == gStep) 
		  {
			  if (sh > 0) 
			  {			  
				  // preprocess the circle
				  INT32 samplingPointCount = preReg(gSamplingPointCount);
				  
#ifdef  SINVOICE_DEBUG_TRACE
				  //hal_HstSendEvent(SYS_EVENT, 0x11110000 + gSamplingPointCount);
#endif

				  // recognise voice
				  waitSpaceFlag = reg(samplingPointCount,decOutIndex+outPutLen);
				  if(waitSpaceFlag == 1)
				  {
						//hal_HstSendEvent(SYS_EVENT, 0x22220000 + *(decOutIndex+outPutLen));
						outPutLen += waitSpaceFlag;
				  }
				  else if(waitSpaceFlag == 2)
				  {
					gProcessSinVoiceRegState = ProcessSinVoiceRegStateHeadOk;	  
					MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SIN_VOICE] The head of SinVoice is finded!");
				  }
				  gSamplingPointCount = 0;
				  gStep = STATE_REG_STEP1;
				  if(*(decOutIndex+outPutLen - 1) == SIN_END_HEX)
				  {
				      gProcessSinVoiceRegState = ProcessSinVoiceRegStateEndOk;
					MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SIN_VOICE] The end of SinVoice is finded!");
				      return outPutLen - 1;
				  }
			  }
		  }
	  }
	}
	return outPutLen;
}

void SinVoiceDecReset()
{
	gSamplingPointCount = 0;
	gIsStartCounting = FALSE;
	gStep = STATE_REG_STEP1;
	gIsBeginning = FALSE;
	gStartingDet = FALSE;
	gStartingDetCount = 0;
	mPreRegCircle = -1;

	gMax_Sample_Point_Count = ((gSampleRate/CODE_BEGAIN+1)/3)*3;
	gMin_Reg_Circle_Count = SIN_VOICE_DURATION_NUM - 3;
    gMin_Reg_Pcm_Sample_Len = gSampleRate/SIN_BEGAIN_FRE*SIN_VOICE_DURATION_NUM+SPACE_SIN_DURATION;

	MCI_TRACE (MCI_AUDIO_TRC,0,"[SinVoiceDecInit]  gMin_Reg_Pcm_Sample_Len=%d", gMin_Reg_Pcm_Sample_Len);

}

INT32 SinVoiceBufferDec( INT16 *pInputPcm, INT32 inputPcmSampleLen, INT32 *pInputPcmSampleConsumedLen, INT8  *pCharOutPut)
{
    INT32 outCharLen = 0;
	INT32 InputPcmSampleConsumedLen = 0;
     INT16 *pPcm = NULL;

	 pPcm = pInputPcm;
	*pInputPcmSampleConsumedLen = InputPcmSampleConsumedLen;
	
	//dbg_TraceOutputText(0,"[SinVoiceBufferDec] SinVoiceBufferDec begain");
	if(pPcm == NULL || pCharOutPut == NULL)
	{
	
		//dbg_TraceOutputText(0,"[SinVoiceBufferDec] SinVoiceBufferDec error:pPcm == NULL || pCharOutPut == NULL");
		gProcessSinVoiceRegState = ProcessSinVoiceRegStateErro;
		return 0;
	}
	
	if(inputPcmSampleLen <= gMin_Reg_Pcm_Sample_Len)
	{
	
		//dbg_TraceOutputText(0,"[SinVoiceBufferDec] SinVoiceBufferDec error:inputPcmSampleLen <= gMin_Reg_Pcm_Sample_Len");
		return 0;
	}
	//MCI_TRACE (MCI_AUDIO_TRC,0,"[SinVoiceBufferDec] gProcessSinVoiceRegState=%d", gProcessSinVoiceRegState);

	if(gProcessSinVoiceRegState != ProcessSinVoiceRegStateNeedInit)
	{
		while((inputPcmSampleLen - *pInputPcmSampleConsumedLen >= gMin_Reg_Pcm_Sample_Len) && (gProcessSinVoiceRegState != ProcessSinVoiceRegStateEndOk) && (gProcessSinVoiceRegState != ProcessSinVoiceRegStateErro))
		{
		
			//MCI_TRACE (MCI_AUDIO_TRC,0,"[SinVoiceBufferDec] inputPcmSampleLen -  *pInputPcmSampleConsumedLen=%d", inputPcmSampleLen -  *pInputPcmSampleConsumedLen);
			outCharLen += SinVoiceProcess(pPcm + *pInputPcmSampleConsumedLen, inputPcmSampleLen -  *pInputPcmSampleConsumedLen, pCharOutPut + outCharLen, &InputPcmSampleConsumedLen);
			*pInputPcmSampleConsumedLen += InputPcmSampleConsumedLen;
			//MCI_TRACE (MCI_AUDIO_TRC,0,"[SinVoiceBufferDec] InputPcmSampleConsumedLen=%d", InputPcmSampleConsumedLen);
		}
	}
	return outCharLen;
}

//=============================================================================
// Vpp_SinVoiceStreamDetectSet function
//-----------------------------------------------------------------------------
/// flag : 1, enable SinVoiceDec; flag : 0, disable SinVoiceDec; 
//=============================================================================
void Vpp_SinVoiceStreamDetectSet(UINT8 flag)
{
	 gPlaySinVoiceStream = flag;
}

//=============================================================================
// Vpp_SinVoiceStreamDetectSet function
//-----------------------------------------------------------------------------
/// return : 1, enable SinVoiceDec; return : 0, disable SinVoiceDec; 
//=============================================================================
UINT8 Vpp_SinVoiceDecDetect(void)
{
	return gPlaySinVoiceStream;
}
void Vpp_SinVoiceDecInit(void)
{
	gSinVoiceRegBegainFlag = 0;
	gProcessSinVoiceRegState = ProcessSinVoiceRegStateNeedInit;
	gSinVoiceRegState = SinVoiceRegStateInit;
	processSinVoiceResult.SinVoiceResultLen = 0;
	processSinVoiceResult.pSinVoiceResult = NULL;
	processSinVoiceResult.processSinVoiceState = SinVoiceRegStateInit;
}

ProcessSinVoiceResult Vpp_SinVoiceDec(INT16 *pInputPcm, INT16 ch, INT32 inputPcmSampleLen, INT16 *pCollatorBuffer)
{
	INT16 i;
	INT32 inputPcmConsumedLen = 0;
	static INT32 remainInputDataLen = 0;
	static INT16 decIndexLen = 0, decTimes = 0;
	static INT8 *pDecIndex = NULL;
	
	if(gProcessSinVoiceRegState == ProcessSinVoiceRegStateNeedInit)
	{
		remainInputDataLen = 0;
		decIndexLen = 0;
		decTimes = 0;
		pDecIndex = &gCharOutputBuffer[decTimes][0];
		
		//MCI_TRACE (MCI_AUDIO_TRC,0,"[Vpp_SinVoiceDec] ProcessSinVoiceRegStateNeedInit! remainInputDataLen=%d", remainInputDataLen);
	}

	if(gProcessSinVoiceRegState == ProcessSinVoiceRegStateNeedInit || gProcessSinVoiceRegState == ProcessSinVoiceRegStateEndOk)
	{
		SinVoiceDecReset();
		gProcessSinVoiceRegState = ProcessSinVoiceRegStateInitOk;
	}
	
	if(remainInputDataLen + inputPcmSampleLen/2 <= SIN_VOICE_PCM_BUFFER_LEN)
	{
		for(i=0; i< inputPcmSampleLen; i+=ch)
		{
			*(pCollatorBuffer + remainInputDataLen) = *(pInputPcm + i);
			remainInputDataLen++;
		}
	}
	
	//MCI_TRACE (MCI_AUDIO_TRC,0,"[Vpp_SinVoiceDec] Befor SinVoiceBufferDec remainInputDataLen=%d", remainInputDataLen);
	
	if(gSinVoiceRegBegainFlag == 0)
	{
		if(remainInputDataLen >= MIN_SIN_VOICE_REG_BEGAIN_LEN)
		{
			gSinVoiceRegBegainFlag = 1;
		}
		else
		{
			return processSinVoiceResult;
		}
	}
	//MCI_TRACE (MCI_AUDIO_TRC,0,"[Vpp_SinVoiceDec] gProcessSinVoiceRegState=%d", gProcessSinVoiceRegState);
	decIndexLen +=SinVoiceBufferDec(pCollatorBuffer, remainInputDataLen, &inputPcmConsumedLen, pDecIndex + decIndexLen);
	remainInputDataLen -= inputPcmConsumedLen;
	if(remainInputDataLen > 0)
	{
		memcpy(pCollatorBuffer,pCollatorBuffer + inputPcmConsumedLen, remainInputDataLen*2);
	}
	
	//MCI_TRACE (MCI_AUDIO_TRC,0,"[Vpp_SinVoiceDec] End SinVoiceBufferDec remainInputDataLen=%d", remainInputDataLen);
	switch(gProcessSinVoiceRegState)
	{
		case ProcessSinVoiceRegStateHeadOk	       :
		case ProcessSinVoiceRegStateInitOk		   :
		case ProcessSinVoiceRegStateCheckSpaceOk   : 
			gSinVoiceRegState = SinVoiceRegStateStart;
			break;
        case ProcessSinVoiceRegStateNeedInit       :
		case ProcessSinVoiceRegStateErro		   : 
			gSinVoiceRegState = SinVoiceRegStateErro;
			break;

		case ProcessSinVoiceRegStateEndOk 		   :
			
			HexToAscii(pDecIndex, pDecIndex, decIndexLen);
			if(decTimes == 1)
			{
			 if(!strcmp(&gCharOutputBuffer[0][0],&gCharOutputBuffer[1][0]))
			 {
				gSinVoiceRegState = SinVoiceRegStateOk; 
				processSinVoiceResult.pSinVoiceResult = pDecIndex;
				processSinVoiceResult.SinVoiceResultLen = decIndexLen/2;
				gProcessSinVoiceRegState = ProcessSinVoiceRegStateNeedInit;
			 }
			}
			else if(decTimes == 2)
			{
			 if(!strcmp(&gCharOutputBuffer[0][0],&gCharOutputBuffer[2][0]) || !strcmp(&gCharOutputBuffer[1][0],&gCharOutputBuffer[2][0]))
			 {
				gSinVoiceRegState = SinVoiceRegStateOk; 
				processSinVoiceResult.pSinVoiceResult = pDecIndex;
				processSinVoiceResult.SinVoiceResultLen = decIndexLen/2;
			 }
			 else
			 {
				gSinVoiceRegState = SinVoiceRegStateErro; 
			 }
			 gProcessSinVoiceRegState = ProcessSinVoiceRegStateNeedInit;
			}
			if(gSinVoiceRegState != SinVoiceRegStateOk)
			{
				decIndexLen = 0;
				decTimes++;
				pDecIndex = &gCharOutputBuffer[decTimes][0];
			}
			break;		
		default :				
		gSinVoiceRegState = SinVoiceRegStateErro;
		gProcessSinVoiceRegState = ProcessSinVoiceRegStateNeedInit;
		break;

	}
	processSinVoiceResult.processSinVoiceState = gSinVoiceRegState;

	return processSinVoiceResult;
}

VPP_SINVOICE_DEC_CALLBACK_T g_vppSinVoiceDecFinishedCallback = NULL;

VOID Vpp_SinVoiceFinished(ProcessSinVoiceResult result)
{
    if (g_vppSinVoiceDecFinishedCallback)
    {
        (*g_vppSinVoiceDecFinishedCallback)(result);
    }
}

