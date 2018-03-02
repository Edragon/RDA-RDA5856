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


#include "cs_types.h"
#include "mcip_debug.h"

#include "fs.h"
#include "mmc_audioamr.h"
#include "hal_error.h"

#include "cpu_share.h"
#include "assert.h"
#include"hal_voc.h"

#include "global.h"
#include "audio_api.h"

#include "aud_m.h"

#include "cos.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"

#include "string.h"
#include "hal_overlay.h"

#include "vpp_speech.h"


//extern UINT32 pcmbuf_overlay[4608];

volatile INT32 g_AMRConsumedLength=0;
volatile INT32 g_AMRFILESIZE=0;
static INT32 g_AMRFrameSize=0;

extern AUD_LEVEL_T audio_cfg;
extern AUD_ITF_T audioItf;
extern HAL_AIF_STREAM_T audioStream;
	
#define AudioAmr_INPUT_BUFSIZE		10*1024//20k byte


#define AMRDEC_OUTPUT_BUFSIZE (160*2)

#define AMRDEC_PCM_OUTPUT_BUFFER_SIZE (320*16) 


AMR_PLAY AmrPlayer;

static INT32 Mmc_Amr_Pcm_Half;
static INT32 Mmc_Amr_FrameCount;
static UINT32 Mmc_Amr_PcmDataCopyCount;
static UINT32 ErrorFrameCount=0;
PRIVATE BOOL g_mmcAmrVppOpened = FALSE;
PRIVATE BOOL g_mmcAmrAudOpened = FALSE;
PRIVATE BOOL g_mmcAmrOvlLoaded = FALSE;

/*
* NAME:	MMC_AmrVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AmrVocISR(void)
{  
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO_AMR]VoC!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AMRDEC_VOC);
} 

/*
* NAME:	MMC_AmrPcmISR()
* DESCRIPTION: ISR for pcm buffer exhausted..
*/
void MMC_AmrHalfPcmISR(void)
{
	Mmc_Amr_Pcm_Half = 0;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO_AMR]HALF!");
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);

}

/*
* NAME:	MMC_AudioPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/
void MMC_AmrEndPcmISR(void)
{
	Mmc_Amr_Pcm_Half = 1;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_AUDIO_AMR]END!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);

}


extern MPEG_PLAY MpegPlayer;

/*
* NAME:	MMC_AmrDecOpen()
* DESCRIPTION: Initialize AUDIO decoder.
*/
MCI_ERR_T MMC_AmrDecOpen (HANDLE fhd, AMR_PLAY *pAmrPlayer)
{
	AMR_INPUT *input = &pAmrPlayer->AmrInput;
	AMR_OUTPUT *output = &pAmrPlayer->AmrOutput;
       MPEG_INPUT    *my_input = &MpegPlayer.MpegInput;

	HAL_ERR_T voc_ret = 0;
	int32 readlen = 0;
	
	int32  result;
	int32  AMRMode;
	
	HAL_SPEECH_DEC_IN_T decStruct;
	VPP_SPEECH_AUDIO_CFG_T vppCfg;

	 AUD_ERR_T audErr;
	
	input->fileHandle = fhd;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AMR]fileHandle:%d",fhd);
	
	/*initialize I/O data struct*/
	input->bof = (uint8 *)mmc_MemMalloc(AudioAmr_INPUT_BUFSIZE);
	if(input->bof==NULL)
	{
		mmc_MemFreeAll();
		return MCI_ERR_OUT_OF_MEMORY;		
	}
	
	memset(input->bof, 0, AudioAmr_INPUT_BUFSIZE);
	input->length = AudioAmr_INPUT_BUFSIZE;
	input->data = input->bof;
	input->eof = input->bof + input->length;
	input->fileOffset = 0;
	input->readOffset = 0;
	input->inFlag = 0;
	input->is_first_frame=1;
	
	output->data = (int16 *)mmc_MemMalloc(AMRDEC_OUTPUT_BUFSIZE);
	if(input->data==NULL)
	{
		mmc_MemFreeAll();
		return MCI_ERR_OUT_OF_MEMORY;		
	}
	memset(output->data, 0, AMRDEC_OUTPUT_BUFSIZE);
	output->length = (AMRDEC_OUTPUT_BUFSIZE) >> 2;//80 int
	output->rPingpFlag = 0;
	output->wPingpFlag = 0;
	output->outFlag = 0;
	
	if (hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2) != HAL_ERR_NO)
	{
        return MCI_ERR_ERROR;
	}
    g_mmcAmrOvlLoaded = TRUE;

	output->pcm.Buffer = (uint32 *)pcmbuf_overlay;
	
	//output->pcm.Buffer = (uint32 *)mmc_MemMalloc(AMRDEC_PCM_OUTPUT_BUFFER_SIZE);
	memset(output->pcm.Buffer,0,AMRDEC_PCM_OUTPUT_BUFFER_SIZE);
	output->pcm.Buffer_head=output->pcm.Buffer;

	output->pcm.Size = AMRDEC_PCM_OUTPUT_BUFFER_SIZE>>2;

	
	Mmc_Amr_PcmDataCopyCount=0;
	Mmc_Amr_Pcm_Half=0;
	Mmc_Amr_FrameCount = 0;
	ErrorFrameCount=0;
	/*prepare input stream*/
	//diag_printf(" tick = %d\n",cyg_current_time());
	readlen = mmc_aud_FileRead(input->fileHandle, input->bof, 7);
	//diag_printf(" tick = %d\n",cyg_current_time());


	if(readlen<0)
	{
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	if (!((input->bof[0]==0x23)&&(input->bof[1]==0x21)&&(input->bof[2]==0x41)&&(input->bof[3]==0x4d)&&(input->bof[4]==0x52)&&(input->bof[5]==0x0a)))
		return MCI_ERR_INVALID_FORMAT;


	AMRMode=(input->bof[6]>>3)&0x7;
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_AMR]AMR Mode :%d",AMRMode);
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_AMR]g_AMRConsumedLength(before aligned) :%d",g_AMRConsumedLength);

	
	switch (AMRMode)
	{
		case MCI_PLAY_MODE_AMR475:
			g_AMRFrameSize=13;
			g_AMRConsumedLength=(g_AMRConsumedLength/13)*13;
			decStruct.codecMode = HAL_AMR475_DEC;
			break;
		case MCI_PLAY_MODE_AMR515:
			g_AMRFrameSize=14;
			g_AMRConsumedLength=(g_AMRConsumedLength/14)*14;
			decStruct.codecMode = HAL_AMR515_DEC;
			break;
		case MCI_PLAY_MODE_AMR59:
			g_AMRFrameSize=16;
			g_AMRConsumedLength=(g_AMRConsumedLength/16)*16;
			decStruct.codecMode = HAL_AMR59_DEC;
			break;
		case MCI_PLAY_MODE_AMR67:
			g_AMRFrameSize=18;
			g_AMRConsumedLength=(g_AMRConsumedLength/18)*18;
			decStruct.codecMode = HAL_AMR67_DEC;
			break;
		case MCI_PLAY_MODE_AMR74:			
			g_AMRFrameSize=20;
			g_AMRConsumedLength=(g_AMRConsumedLength/20)*20;
			decStruct.codecMode = HAL_AMR74_DEC;
			break;
		case MCI_PLAY_MODE_AMR795:			
			g_AMRFrameSize=21;
			g_AMRConsumedLength=(g_AMRConsumedLength/21)*21;
			decStruct.codecMode = HAL_AMR795_DEC;
			break;
		case MCI_PLAY_MODE_AMR102:			
			g_AMRFrameSize=27;
			g_AMRConsumedLength=(g_AMRConsumedLength/27)*27;
			decStruct.codecMode = HAL_AMR102_DEC;
			break;
		case MCI_PLAY_MODE_AMR122:			
			g_AMRFrameSize=32;
			g_AMRConsumedLength=(g_AMRConsumedLength/32)*32;
			decStruct.codecMode = HAL_AMR122_DEC;
			break;
		default:
			return MCI_ERR_INVALID_FORMAT;
			break;
	}	

	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AMR]g_AMRConsumedLength(after aligned) :%d",g_AMRConsumedLength);
	
	result=mmc_aud_FileSeek(input->fileHandle,g_AMRConsumedLength-1,FS_SEEK_CUR);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]FS_seek pos:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AMR]FS_seek error!");
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	readlen = mmc_aud_FileRead(input->fileHandle, input->bof, input->length);
	//diag_printf(" tick = %d\n",cyg_current_time());
	diag_printf("[MMC_AMR] first read len=%d ;need length=%d\n", readlen,input->length);
	
	
	if(readlen<input->length)
	{
		input->eof = input->bof + readlen;
	}

		input->fileOffset += readlen;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AMR]VoC open!");
	/*open voc decoder*/

	voc_ret =vpp_SpeechOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AmrVocISR, VPP_SPEECH_WAKEUP_SW_DEC);

	 if(HAL_ERR_RESOURCE_BUSY==voc_ret)
	 {
		diag_printf("[MMC_AMR_ERR]fail to call vpp_SpeechOpen(), result: %d \n", voc_ret);
		return MCI_ERR_ERROR;
	}

    g_mmcAmrVppOpened = TRUE;

        decStruct.dtxOn = 0;
        // Codec mode depends on the codec used by the stream
        // and will be set in the switch below
        decStruct.decFrameType = 0;
        decStruct.bfi = 0;
        decStruct.sid = 0;
        decStruct.taf = 0;
        decStruct.ufi = 0;

        // Size is a multiple of 4.
        for (INT32 i=0; i<HAL_SPEECH_FRAME_SIZE_COD_BUF/4; i++)
        {
            *((UINT32*)decStruct.decInBuf + i) = 0;
         }
		
        // VPP audio config
        vppCfg.echoEsOn = 0;
        vppCfg.echoEsVad = 0;
        vppCfg.echoEsDtd = 0;
        vppCfg.echoExpRel = 0;
        vppCfg.echoExpMu = 0;
        vppCfg.echoExpMin = 0;
        vppCfg.encMute = VPP_SPEECH_MUTE;
        vppCfg.decMute = VPP_SPEECH_UNMUTE;
        vppCfg.sdf = NULL;
        vppCfg.mdf = NULL;
        vppCfg.if1 = 1;//amrring.


        vpp_SpeechAudioCfg(&vppCfg);

        vpp_SpeechSetOutAlgGain(0);	


        // Init the Rx buffer
        *(vpp_SpeechGetRxCodBuffer()) = decStruct;


    // Stream out
//	   	audioStream.startAddress  = (UINT32*)vpp_SpeechGetRxPcmBuffer();
//    	audioStream.length        = sizeof(HAL_SPEECH_PCM_BUF_T);

   	audioStream.startAddress  = (UINT32*) pAmrPlayer->AmrOutput.pcm.Buffer;
    	audioStream.length        = pAmrPlayer->AmrOutput.pcm.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
    	
    	audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
    	audioStream.channelNb     = HAL_AIF_MONO;//HAL_AIF_STEREO;//HAL_AIF_MONO;
    	audioStream.voiceQuality         = TRUE;    // To adjust with a key
    	audioStream.playSyncWithRecord = FALSE;
    	audioStream.halfHandler   = MMC_AmrHalfPcmISR;
    	audioStream.endHandler    = MMC_AmrEndPcmISR;


      	aud_StreamStop(audioItf);


     	if(my_input->bt_sco==TRUE)
        {
        
    	   audioItf = AUD_ITF_BLUETOOTH;

          }


   	if(audioItf!=AUD_ITF_EAR_PIECE&&audioItf!=AUD_ITF_LOUD_SPEAKER&&audioItf!=AUD_ITF_BLUETOOTH)
   	{
   	  	  // Initial cfg
    		audioItf = AUD_ITF_EAR_PIECE;
   	}


	audErr =aud_StreamStart(audioItf, &audioStream, &audio_cfg);


       // profit
       switch (audErr)
       {
           case AUD_ERR_NO:
               g_mmcAmrAudOpened = TRUE;
		 MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_AMR]aud_StreamStart success!");		   
               return MCI_ERR_NO;			   
               break; 
   
           default:
		 MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_AMR]aud_StreamStart fail!");
               return MCI_ERR_BUSY;
               break; 
       }
	
}



/*
* NAME:	MMC_AmrDecVoc()
* DESCRIPTION: Decode AAC stream ......
*/
//static	HANDLE testfileHandle;

MCI_ERR_T MMC_AmrDecVoc (AMR_PLAY *pAmrPlayer)
{
	AMR_INPUT *input = &pAmrPlayer->AmrInput;
	AMR_OUTPUT *output = &pAmrPlayer->AmrOutput;
       MPEG_INPUT    *my_input = &MpegPlayer.MpegInput;

	HAL_SPEECH_DEC_IN_T* decStruct;

	//uint32 i;
	uint32 readlen = 0;


	
	//diag_printf("[MMC_AUDIO]frame: %d\n", Mmc_Amr_FrameCount);	
	Mmc_Amr_FrameCount++;
	
	if(input->is_first_frame==0)
	{	
		
		input->data += g_AMRFrameSize;
		
		g_AMRConsumedLength+=g_AMRFrameSize;


		memcpy(output->data, (UINT8 *)((UINT32)( vpp_SpeechGetRxPcmBuffer())|0x20000000)+160*2*(Mmc_Amr_FrameCount%2), 160*2);
		
		if((input->eof - input->data)<32)
		{
			if((input->eof-input->bof)==AudioAmr_INPUT_BUFSIZE)
			{		
				
				diag_printf("[MMC_AMR]READ FILE!");
				memcpy(input->bof, input->data, (input->eof - input->data));
				readlen=mmc_aud_FileRead(input->fileHandle, input->bof+(input->eof - input->data), input->length-(input->eof - input->data));
				
				if(readlen<0)
				{
					return MCI_ERR_ERROR;
				}
				
				if(readlen<(input->length-(input->eof - input->data)))
				{
					input->eof=input->bof+(input->eof - input->data)+readlen;				
					
				}
				input->data=input->bof;
			}
			else
			{			
				diag_printf("[MMC_AMR]MUSIC END!");
				return MCI_ERR_END_OF_FILE;				
			}	
		}
		

		ErrorFrameCount=0;
		if(Mmc_Amr_Pcm_Half==0)
		{
			output->pcm.Buffer_head=output->pcm.Buffer; 		
		}
		else if(Mmc_Amr_Pcm_Half==1)
		{
			output->pcm.Buffer_head=output->pcm.Buffer+(output->pcm.Size>>1);			
		}
		output->data=(int16 *)&output->pcm.Buffer_head[Mmc_Amr_PcmDataCopyCount];
		Mmc_Amr_PcmDataCopyCount+=80;

	     decStruct = vpp_SpeechGetRxCodBuffer();
	     	 
	     switch((input->data[0] >> 3) & 0x7)
            {
                // Real frame size, as there is no alignment
                case 0:
                    g_AMRFrameSize      = 13;
                    decStruct->codecMode = HAL_AMR475_DEC;
                    break;
                case 1:
                    g_AMRFrameSize      = 14;
                    decStruct->codecMode = HAL_AMR515_DEC;
                    break;
                case 2:
                    g_AMRFrameSize      = 16;
                    decStruct->codecMode = HAL_AMR59_DEC;
                    break;
                case 3:
                    g_AMRFrameSize      = 18;
                    decStruct->codecMode = HAL_AMR67_DEC;
                    break;
                case 4:
                    g_AMRFrameSize      = 20;
                    decStruct->codecMode = HAL_AMR74_DEC;
                    break;
                case 5:
                    g_AMRFrameSize      = 21;
                    decStruct->codecMode = HAL_AMR795_DEC;
                    break;
                case 6:
                    g_AMRFrameSize      = 27;
                    decStruct->codecMode = HAL_AMR102_DEC;
                    break;
                case 7:
                    g_AMRFrameSize      = 32;
                    decStruct->codecMode = HAL_AMR122_DEC;
                    break;
                default:
                    break;
            }
 

		 memcpy(decStruct->decInBuf, input->data, g_AMRFrameSize);

		
		if(Mmc_Amr_PcmDataCopyCount>=(output->pcm.Size>>1))
		{
			Mmc_Amr_PcmDataCopyCount=0;
		}
		else
		{
			vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC);
		}
	} 
	else
	{
		input->is_first_frame=0;
	}


             if( my_input->pcmflag)
              {
              
                  aud_StreamStop(audioItf);
                  
                 if(my_input->bt_sco)         audioItf = AUD_ITF_BLUETOOTH;
                  else                                   audioItf = AUD_ITF_LOUD_SPEAKER;


       	audioStream.startAddress  = (UINT32*) pAmrPlayer->AmrOutput.pcm.Buffer;
        	audioStream.length        = pAmrPlayer->AmrOutput.pcm.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
        	
        	audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
        	audioStream.channelNb     = HAL_AIF_MONO;//HAL_AIF_STEREO;//HAL_AIF_MONO;
        	audioStream.voiceQuality         = TRUE;    // To adjust with a key
        	audioStream.playSyncWithRecord = FALSE;
        	audioStream.halfHandler   = MMC_AmrHalfPcmISR;
        	audioStream.endHandler    = MMC_AmrEndPcmISR;


           	   aud_StreamStart(audioItf, &audioStream, &audio_cfg);
                   my_input->pcmflag = FALSE;


               }

     
	return MCI_ERR_NO;
}

MCI_ERR_T MMC_AmrDecPcm(AMR_PLAY *pAmrPlayer)
{
		
	
	if(Mmc_Amr_PcmDataCopyCount==0)
	{
		vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC);

	}
	else
	{
		diag_printf("[MMC_AMR]decoding!......");			
  		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AMRDEC_PCM);
		
	}
	return MCI_ERR_NO;
}


/*
* NAME:	MMC_AmrDecClose()
* DESCRIPTION: Close aac decoder.
*/
//extern uint32 *DRV_PCMOutStop(void);
MCI_ERR_T MMC_AmrDecClose (AMR_PLAY *pAmrPlayer)
{
//	AMR_INPUT *input = &pAmrPlayer->AmrInput;
//	AMR_OUTPUT*output = &pAmrPlayer->AmrOutput;

    if (g_mmcAmrAudOpened)
    {
	    aud_StreamStop(audioItf);
        g_mmcAmrAudOpened = FALSE;
    }

    if (g_mmcAmrVppOpened)
    {
	    vpp_SpeechClose();
        g_mmcAmrVppOpened = FALSE;
    }
	
	mmc_MemFreeAll();
	
/*	if (FS_Close(input->fileHandle) < 0)
	{
		diag_printf("[MMC_AUDIO]##WARNING##fail to close file!handle: %d \n", input->fileHandle);
		return MCI_ERR_ERROR;
	}	*/
	if (g_mmcAmrOvlLoaded)
	{
	    hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
        g_mmcAmrOvlLoaded = FALSE;
	}

	return  MCI_ERR_NO;
	
}


int32 Audio_AmrPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
	int32 result;

	AmrPlayer.mode = MMC_AMR_DECODE;
	
	AmrPlayer.AmrOutput.OutputPath = OutputPath;


	if (fhd< 0)
	{
		diag_printf("[MMC_AMR_ERR]ERROR file hander: %d \n", fhd);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	if ((g_AMRFILESIZE=mmc_aud_GetFileSize(fhd))<= 0)
	{
		diag_printf("[MMC_AMR_ERR]can not get file size!:%d",g_AMRFILESIZE);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]File Size: %d",g_AMRFILESIZE);

	g_AMRConsumedLength=(INT32)(((INT64)PlayProgress*(INT64)g_AMRFILESIZE)/10000);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]g_AMRConsumedLength: %d",g_AMRConsumedLength);

		
	if((result= MMC_AmrDecOpen(fhd, &AmrPlayer))!=MCI_ERR_NO)
	{
		diag_printf("[MMC_AMR_ERR]MMC_AmrDecOpen() return false!\n");
		MMC_AmrDecClose(&AmrPlayer);
		return result;
	}
	
	return  MCI_ERR_NO;
}

int32 Audio_AmrStop (void) 	
{
	MMC_AmrDecClose(&AmrPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_AmrPause (void)	
{

	g_AMRConsumedLength-=6;

	if(g_AMRConsumedLength<0)g_AMRConsumedLength=0;
	//aud_StreamPause(audioItf,TRUE);
	//memset(AmrPlayer.AmrOutput.pcm.Buffer,0,AMRDEC_PCM_OUTPUT_BUFFER_SIZE);


	MMC_AmrDecClose(&AmrPlayer);		
	return  MCI_ERR_NO;
}


int32 Audio_AmrResume ( HANDLE fhd) 
{
	//aud_StreamPause(audioItf,FALSE);
	int32 result;

	AmrPlayer.mode = MMC_AMR_DECODE;

	if (fhd< 0)
	{
		diag_printf("[MMC_AMR_ERROR]ERROR file hander: %d \n", fhd);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}
	
	result=mmc_aud_FileSeek(fhd,0,FS_SEEK_SET);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]FS_seek pos:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR_ERROR]FS_seek error!");
		return -1;
	}
	
	if((result= MMC_AmrDecOpen(fhd, &AmrPlayer))!=MCI_ERR_NO)
	{
		diag_printf("[MMC_AMR_ERROR]MMC_AmrDecOpen() return false!");
		MMC_AmrDecClose(&AmrPlayer);
		return result;
	}
	
	return  MCI_ERR_NO;
}


int32 Audio_AmrGetID3 (char * pFileName)  
{
	return MCI_ERR_NO;
}

int32 Audio_AmrGetPlayInformation (MCI_PlayInf * MCI_PlayInfAMR)  
{


       if(g_AMRFILESIZE>0)
       {
        	MCI_PlayInfAMR->PlayProgress=(INT32)((((INT64)g_AMRConsumedLength+6)*10000)/g_AMRFILESIZE);
       }
       else
       {
        	MCI_PlayInfAMR->PlayProgress=0;
       }

    
	
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AMR]PlayProgress:%d",MCI_PlayInfAMR->PlayProgress);

	return MCI_ERR_NO;
}

int32 Audio_AmrUserMsg(int32 nMsg)
{
    switch (nMsg)
    {
	case MSG_MMC_AMRDEC_VOC:
		//AmrPlayer.IrqType = AACIrqFlag;
		switch (MMC_AmrDecVoc(&AmrPlayer))
		{
		case MCI_ERR_END_OF_FILE:
			diag_printf("[MMC_AMR]MMC_AMRDecRun() return FINISH!\n");
			MMC_AmrDecClose(&AmrPlayer);
			MCI_AudioFinished(MCI_ERR_END_OF_FILE);
			//EVENT_Send2Jade(CHAN_MMC_ID, MCI_MEDIA_PLAY_FINISH_IND, 5, 0, 0);
			mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
			break;
		case MCI_ERR_ERROR:
			diag_printf("[MMC_AMR_ERR]MMC_AMRDecRun() return FALSE!\n");
			MMC_AmrDecClose(&AmrPlayer);
			MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
			//EVENT_Send2Jade(CHAN_MMC_ID, MCI_MEDIA_PLAY_FINISH_IND, 8, 0, 0);
			mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
			break;
		default:
			break;
		}
		
		break;
		case MSG_MMC_AMRDEC_PCM:
#ifdef __BT_PCM_OVER_UART__
                    aud_BtSendSCOData((Mmc_Amr_Pcm_Half)%2);
#endif
            
			MMC_AmrDecPcm(&AmrPlayer);
			break;
		default:
			diag_printf("[MMC_AMR_ERR]ERROR AMR MESSAGE!\n");
			break;
    }
    return 0;
}

