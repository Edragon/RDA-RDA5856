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
// FILENAME: mmc_audiowav.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cswtype.h"
#include "mcip_debug.h"
#include "fs.h"


#include "mmc.h"
#include "cpu_share.h"

#include "audio_api.h"

#include "aud_m.h"
#include "fs_asyn.h"

#include "cos.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"
#include "tgt_aud_cfg.h"

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "event.h"
#include "mmc_audiowav.h"

#ifndef BT_SOURCE_SPEAKER
#define ADPCM_SUPPORT   1
#endif

static INT8 Mmc_Wav_Pcm_Half;
static INT32 g_copyed_frame;
static INT32 g_readed_frame;
static INT8 g_stop_read;
//patch for short file
static INT8 g_read_end;
static INT8 g_continue_play_flag;
static INT32 g_sbc_frame;

static INT32 g_WAVHeaderLength=0;

WAV_PLAY *WavPlayer;

extern AUD_LEVEL_T audio_cfg;
extern HAL_AIF_STREAM_T audioStream;
extern AUD_ITF_T audioItf;

//extern UINT32 pcmbuf_overlay[4608];

volatile INT32 g_WAVConsumedLength=0;

volatile INT32 g_WAVFILESIZE=-1;

static int32 data_size =0;

//static UINT16 g_WavNout;
static UINT32 g_Wavcounterframe;

//static INT16 g_WavNumOfOutSample=0;

//static INT16 g_WavTotalFrame=0;


static UINT8 g_WavResampleInitFlag=0;


//static INT16 *g_WavBT8kPcmBuffer=NULL;


static  INT16  *g_WavBTDataAddress=0;



static INT32 g_Wav16BitInputBufSize =0;

static INT32 g_WavOutputPcmBufSize =0;

static INT32 g_nFrame               =0;

static INT32 g_WavInputBufSize	  =0;

static INT8 g_WavPlayFlag = 0;

static INT8 g_wav_read_flag = 0;

static UINT8 wav_pcm_irq_num = 0;

static UINT8 LeftWavDataPlayFlag=0;//this flag controls to continue play the left data in the input buffer

extern PUBLIC UINT32 gpio_detect_earpiece(VOID);

#ifdef BT_SOURCE_SPEAKER
//#define WAV_TO_SBC_DEBUG
//#define WAV_TO_SBC_TICKS_DEBUG

#define SBC_DATA_BUF_LEN 1024

#ifdef WAV_TO_SBC_TICKS_DEBUG
    UINT32 g_WavSbcTicks = 0;
#endif

//#if APP_SUPPORT_A2DP_SOURCE==1
extern void app_a2dp_send_data_handle(void *pArgs);
//#endif
static UINT8 bt_start_send_wav_sbc_flag = 1;
static UINT16 source_mtu = 800;
A2DP_SOURCE_MSG *frame_sbc_msg = NULL;

static INT8 *g_WavToSbcBuffer=NULL;
static INT8 *g_SbcToBtBuffer=NULL;

static INT16 g_WavDataLen=0;
static INT16 g_SbcDataLen=0;
static INT16 sbcenc_voc_busy = 0;
vpp_AudioSbc_ENC_INIT_T SbcParse;

extern const INT32 G_VppSBCConstX[];
static INT32 *G_VppSBCEncBakConstY=NULL;


/*
* NAME:	MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioWavVocISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_WAVSBC]VoC INT!");
    sbcenc_voc_busy = 0;
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_SBC_VOC);
} 

#endif

 
/*
* NAME:	MMC_AudioWavHalfPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/

void MMC_AudioWavHalfPcmISR(void)
{

#ifdef WAV_TO_SBC_TICKS_DEBUG
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x11100000 + sbcenc_tick - g_WavSbcTicks);
	g_WavSbcTicks = sbcenc_tick;
#endif

	if(wav_pcm_irq_num > 20)
	{
		return;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAV]HALF!");
	Mmc_Wav_Pcm_Half = 0;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_PCM_INT);
    if(g_wav_read_flag)
    {
    	wav_pcm_irq_num++;
        hal_HstSendEvent(SYS_EVENT,0x08440001);
    	memset(pcmbuf_overlay, 0, g_WavOutputPcmBufSize);
    }
	else
	{
		wav_pcm_irq_num = 0;
	}
}

/*
* NAME:	MMC_AudioWavEndPcmISR()
* DESCRIPTION: ISR for pcm end buffer exhausted.
*/
void MMC_AudioWavEndPcmISR(void)
{
#ifdef WAV_TO_SBC_TICKS_DEBUG
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x22200000 + sbcenc_tick - g_WavSbcTicks);
	g_WavSbcTicks = sbcenc_tick;
#endif

	if(wav_pcm_irq_num > 20)
	{
		return;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAV]END!");
	Mmc_Wav_Pcm_Half = 1;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_PCM_INT);
    if(g_wav_read_flag)
    {
	    wav_pcm_irq_num++;
        hal_HstSendEvent(SYS_EVENT,0x08440002);
    	memset(pcmbuf_overlay, 0, g_WavOutputPcmBufSize);
    }
	else
	{
		wav_pcm_irq_num = 0;
	}
    
}


static const uint16 PCMSamplesRate[]={48000,44100,32000,24000,22050,16000,12000,11025,8000};

MCI_ERR_T MMC_WavHeaderParse(WAV_PLAY *pWavPlayer, INT16 resumeFlag)
{
	WAV_INPUT *input = &pWavPlayer->WavInput;
	WAV_OUTPUT *output = &pWavPlayer->WavOutput;

	static WAVE_DATA_HEADER WaveDataHeader;
	static WAVE_HEADER WaveHeaderParse;

	INT32 readlen=0,i;
	WAVE_HEADER *pWaveHdr=&pWavPlayer->WaveHeader;
	WAVE_DATA_HEADER *pWaveDataHdr=&WaveDataHeader;

	UINT8 ChunkInfoID[4];
	UINT32 ChunkInfoSize;
	UINT32 ChunkInfoData;
	UINT32 ChunkInfoLen;

	if(resumeFlag == 0)
	{
        mmc_aud_FileSeek(input->fileHandle, 0, FS_SEEK_SET);
	readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)pWaveHdr,sizeof(WAVE_HEADER));
	if(readlen!=sizeof(WAVE_HEADER)){
		return MCI_ERR_ERROR;
	}

	input->fileOffset+=36;
	g_WAVHeaderLength+=36;


	if(pWaveHdr->wFormatTag==1){  //PCM format!!
	if((memcmp(pWaveHdr->szRiff,"RIFF",4)!=0)
		||(memcmp(pWaveHdr->szWaveFmt,"WAVEfmt ",8)!=0)
		||((pWaveHdr->nChannels!=1)&&(pWaveHdr->nChannels!=2))
		||((pWaveHdr->wBitsPerSample!=8)&&(pWaveHdr->wBitsPerSample!=16))
		||(pWaveHdr->nAvgBytesPerSec!=pWaveHdr->nChannels*pWaveHdr->wBitsPerSample*pWaveHdr->nSamplesPerSec/8)
		||(pWaveHdr->nBlockAlign!=pWaveHdr->nChannels*pWaveHdr->wBitsPerSample/8))
	{
	        hal_HstSendEvent(SYS_EVENT,0x11140020);
	    	return MCI_ERR_BAD_FORMAT;
	}
	}
      else{
#if ADPCM_SUPPORT        
          if((memcmp(pWaveHdr->szRiff,"RIFF",4)!=0)
		||(memcmp(pWaveHdr->szWaveFmt,"WAVEfmt ",8)!=0)
		||((pWaveHdr->nChannels!=1)&&(pWaveHdr->nChannels!=2)))
#endif
		{
		    hal_HstSendEvent(SYS_EVENT,0x11140030);
		   MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_WAV]parse ADPCM header error\n");
		 	return MCI_ERR_BAD_FORMAT;	
		}

       }
	
	if(pWaveHdr->dwFmtSize>=18)     /* We're obviously dealing with WAVEFORMATEX */
	{
             UINT32 cbSize,size;

		size = pWaveHdr->dwFmtSize -18;
 
		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8*)&cbSize,2);
	
		if(readlen!=2)
		{
			diag_printf("[MMC_AUDIO_WAV]Optional infomation read error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}		

              cbSize = MIN(size, cbSize);

              hal_HstSendEvent(SYS_EVENT,0x11140050);
              hal_HstSendEvent(SYS_EVENT,cbSize);
       
	       if(cbSize > 0) {
		if( (cbSize >=22)&&( pWaveHdr->wFormatTag==0xfffe )){ /* WAVEFORMATEXTENSIBLE */
                    diag_printf("[MMC_AUDIO_WAV] ERROR!!!:  Don't support  WAVEFORMATEXTENSIBLE mode!!\n");
			return MCI_ERR_BAD_FORMAT;
			}

              input->extradata_size = cbSize; 			  
		input->extradata = (uint8 *)mmc_MemMalloc((input->extradata_size + WAV_INPUT_BUFFER_PADDING_SIZE));

		if(input->extradata==NULL)
		{
//tianwq				hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
			mmc_MemFreeAll();
			return MCI_ERR_OUT_OF_MEMORY;
			
		}	 
		
              readlen=mmc_aud_FileRead(input->fileHandle,input->extradata,input->extradata_size);
			  
		if(readlen!=input->extradata_size)
		{
			diag_printf("[MMC_AUDIO_WAV]Optional infomation read error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}		
	       }   
       

             size -= cbSize;
			 
            /* It is possible for the chunk to contain garbage at the end */
             if (size > 0){
			 	
		    INT32 result;
                  result=mmc_aud_FileSeek(input->fileHandle,size,FS_SEEK_CUR);

		if(result<0 ){
			MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV_ERR]FS_seek ERROR!");
			return MCI_ERR_BAD_FORMAT;
		 }
            }
			 
             input->fileOffset = input->fileOffset + 2 + cbSize + size;
             g_WAVHeaderLength = g_WAVHeaderLength + 2 + cbSize + size;
	
			
	}
	

	//check samplerate
	for(i=0;i<sizeof(PCMSamplesRate)/sizeof(PCMSamplesRate[0]);i++)
	{
		if(pWaveHdr->nSamplesPerSec==PCMSamplesRate[i])
		{
			break;
		}
	}
	
	if(i>=sizeof(PCMSamplesRate)/sizeof(PCMSamplesRate[0]))
	{
	    hal_HstSendEvent(SYS_EVENT,0x11140060);
		return MCI_ERR_INVALID_FORMAT;
	}

	readlen=mmc_aud_FileRead(input->fileHandle,ChunkInfoID,4);

	if(readlen!=4)
	{
		diag_printf("[MMC_AUDIO_WAV]data header read len:%d\n",readlen);
		return MCI_ERR_ERROR;
	}

	input->fileOffset+=4;
	g_WAVHeaderLength+=4;
	
    if(memcmp(ChunkInfoID,"LIST",4)==0)
    {
        readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)&ChunkInfoLen,4);

        if((ChunkInfoLen < 0) || (readlen != 4))
        {
        	hal_HstSendEvent(SYS_EVENT,0x11140090);
        	hal_HstSendEvent(SYS_EVENT,ChunkInfoLen);
        	hal_HstSendEvent(SYS_EVENT,readlen);

    		return MCI_ERR_BAD_FORMAT;
        }
        input->fileOffset+=4;
	    g_WAVHeaderLength+=4;
	    
        if(mmc_aud_FileSeek(input->fileHandle, ChunkInfoLen, FS_SEEK_CUR)<0)
        {
            hal_HstSendEvent(SYS_EVENT,0x111400a0);
    		return MCI_ERR_BAD_FORMAT;
        }

        input->fileOffset+=readlen;
	    g_WAVHeaderLength+=readlen;
	    
        readlen=mmc_aud_FileRead(input->fileHandle,ChunkInfoID,4);

        if(readlen != 4)
        {
            hal_HstSendEvent(SYS_EVENT,0x111400b0);
    		return MCI_ERR_BAD_FORMAT;
        }
        else
        {
            hal_HstSendEvent(SYS_EVENT,0x111400c0);
            hal_HstSendEvent(SYS_EVENT,*(UINT32 *)ChunkInfoID);
        }
    }
    
	if(memcmp(ChunkInfoID,"fact",4)==0)
	{
		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)&ChunkInfoSize,4);
		
		if(readlen!=4||ChunkInfoSize!=4)
		{
			diag_printf("[MMC_WAV]Fact Chunk Size Read Error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}

		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)&ChunkInfoData,4);
		
		if(readlen!=4)
		{
			diag_printf("[MMC_WAV]Fact Chunk Data Read Error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}


		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)pWaveDataHdr,sizeof(WAVE_DATA_HEADER));
		
		if(readlen!=sizeof(WAVE_DATA_HEADER))
		{
			diag_printf("[MMC_WAV]Data Chunk Read Error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}
		input->fileOffset+=16;
		g_WAVHeaderLength+=16;
				
	}	
	else if(memcmp(ChunkInfoID,"data",4)==0)
	{
		pWaveDataHdr->szData[0]=ChunkInfoID[0];
		pWaveDataHdr->szData[1]=ChunkInfoID[1];
		pWaveDataHdr->szData[2]=ChunkInfoID[2];
		pWaveDataHdr->szData[3]=ChunkInfoID[3];

		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)&pWaveDataHdr->dwDataSize,4);
		
		if(readlen!=4)
		{
			diag_printf("[MMC_WAV]Data Chunk DataSize Read Error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}
		
		input->fileOffset+=4;
		g_WAVHeaderLength+=4;		

	}
    else if(memcmp(ChunkInfoID,"APx5",4)==0)
	{
		pWaveDataHdr->szData[0]=ChunkInfoID[0];
		pWaveDataHdr->szData[1]=ChunkInfoID[1];
		pWaveDataHdr->szData[2]=ChunkInfoID[2];
		pWaveDataHdr->szData[3]=ChunkInfoID[3];

		readlen=mmc_aud_FileRead(input->fileHandle,(UINT8 *)&pWaveDataHdr->dwDataSize,4);
		
		if(readlen!=4)
		{
			diag_printf("[MMC_WAV]Data Chunk DataSize Read Error!:%d\n",readlen);
			return MCI_ERR_ERROR;
		}
		
		input->fileOffset+=4;
		g_WAVHeaderLength+=4;		

	}
	else
	{
    	hal_HstSendEvent(SYS_EVENT,0x11140080);
		diag_printf("[MMC_WAV]data header error [%d][%d][%d][%d]\n",pWaveDataHdr->szData[0],pWaveDataHdr->szData[1],pWaveDataHdr->szData[2],pWaveDataHdr->szData[3]);
		return MCI_ERR_BAD_FORMAT;	
	}
		hal_HstSendEvent(SYS_EVENT,0x11140081);

		WaveHeaderParse = pWavPlayer->WaveHeader;

		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.dwFmtSize);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.nBlockAlign);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.nSamplesPerSec);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.wFormatTag);

		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.dwFmtSize);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.nBlockAlign);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.nSamplesPerSec);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.wFormatTag);
	}
	else
	{
		hal_HstSendEvent(SYS_EVENT,0x11140082);
		pWavPlayer->WaveHeader = WaveHeaderParse;

		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.dwFmtSize);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.nBlockAlign);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.nSamplesPerSec);
		hal_HstSendEvent(SYS_EVENT,pWavPlayer->WaveHeader.wFormatTag);

		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.dwFmtSize);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.nBlockAlign);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.nSamplesPerSec);
		hal_HstSendEvent(SYS_EVENT,WaveHeaderParse.wFormatTag);
	}

	if(pWaveDataHdr->dwDataSize == 0)
	{
		return MCI_ERR_END_OF_FILE;
	}
       input->codec        =   pWaveHdr->wFormatTag;
	input->codec_id    =  wav_codec_get_id(pWaveHdr->wFormatTag, pWaveHdr->wBitsPerSample);
	input->block_align = pWaveHdr->nBlockAlign;
	input->channels = (uint8)pWaveHdr->nChannels;	
	output->SampleRate=(uint16)pWaveHdr->nSamplesPerSec;
	output->Bits=(uint8)pWaveHdr->wBitsPerSample;
	
	return MCI_ERR_NO;

}

/*
* NAME:	MMC_AudioWavFileReadFinish()
*/
void MMC_AudioWavFileReadFinish(void *pParam)
{
	FS_ASYN_READ_RESULT *pRsp = ( FS_ASYN_READ_RESULT*)pParam;
	
	if(pRsp->iResult!=0)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_WAV_ERR] READING FILE ERROR!");  
		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_PCM_END);
		return;
	}


	if(pRsp->uSize < (g_WavOutputPcmBufSize>>1))
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]READ LENGTHE:%d",pRsp->uSize);
		g_read_end = 1;
		return;
	}
//	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]MMC_AudioWavFileReadFinish g_copyed_frame:%d,g_readed_frame:%d,g_stop_read:%d",g_copyed_frame,g_readed_frame,g_stop_read);

	WavPlayer->WavInput.fileOffset = WavPlayer->WavInput.fileOffset + pRsp->uSize;

	g_readed_frame++;
	
	if(g_readed_frame == g_nFrame)
	{
		g_readed_frame = 0;
	}
	
	if(g_readed_frame != g_copyed_frame)
	{
		FS_AsynReadReq(WavPlayer->WavInput.fileHandle, WavPlayer->WavInput.bof + g_readed_frame * (g_WavOutputPcmBufSize>>1), g_WavOutputPcmBufSize>>1, MMC_AudioWavFileReadFinish);
	}
	else if(g_readed_frame == g_copyed_frame)
	{
		g_stop_read = 1;
	}
    return;
} 

extern VOID hal_abbSetPaOpenMode(BOOL on);

void MMC_WavPcmOutStart(WAV_PLAY *pWavPlayer)
{

	hal_HstSendEvent(SYS_EVENT,0x88883000);    	
	hal_HstSendEvent(SYS_EVENT,(UINT32)pWavPlayer->WavOutput.Buffer);
	hal_HstSendEvent(SYS_EVENT,pWavPlayer->WavOutput.Size);    	
	hal_HstSendEvent(SYS_EVENT,pWavPlayer->WavOutput.Channel);    	
	hal_HstSendEvent(SYS_EVENT,pWavPlayer->WavOutput.Bits);    	

	
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAV]MMC_WavPcmOutStart BEGIN!");
	{
	// Stream out
		audioStream.startAddress  	= (UINT32 *)pWavPlayer->WavOutput.Buffer;
		audioStream.length        	= pWavPlayer->WavOutput.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
		audioStream.channelNb     	= pWavPlayer->WavOutput.Channel;//HAL_AIF_STEREO;//HAL_AIF_MONO;

	}
	audioStream.voiceQuality   = !TRUE;    
	audioStream.halfHandler   	= MMC_AudioWavHalfPcmISR;
	audioStream.endHandler    = MMC_AudioWavEndPcmISR;

    // Initial cfg
   	{
   	  	  // Initial cfg
   	  	  if(gpio_detect_earpiece())
    		audioItf = AUD_ITF_EAR_PIECE;
          else
            audioItf = AUD_ITF_LOUD_SPEAKER;
   	}

	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAV]SAMPLERATE:%d",pWavPlayer->WavOutput.SampleRate);

	switch (pWavPlayer->WavOutput.SampleRate)
	{
	case 48000:
  		audioStream.sampleRate    = HAL_AIF_FREQ_48000HZ;
		break;
	case 44100:
		audioStream.sampleRate    = HAL_AIF_FREQ_44100HZ;
		break;
	case 32000:
		audioStream.sampleRate    = HAL_AIF_FREQ_32000HZ;
		break;
	case 24000:
		audioStream.sampleRate    = HAL_AIF_FREQ_24000HZ;
		break;
	case 22050:
		audioStream.sampleRate    = HAL_AIF_FREQ_22050HZ;
		break;
	case 16000:
		audioStream.sampleRate    = HAL_AIF_FREQ_16000HZ;
		break;
	case 12000:
		audioStream.sampleRate    = HAL_AIF_FREQ_12000HZ;
		break;
	case 11025:
		audioStream.sampleRate    = HAL_AIF_FREQ_11025HZ;
		break;
	case 8000:
		audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;
		break;			
	default:
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_WAV_ERROR]ERROR SAMPLERATE:%d",pWavPlayer->WavOutput.SampleRate);

		break;
	}
	hal_abbSetPaOpenMode(TRUE);
	aud_StreamStart(audioItf, &audioStream, &audio_cfg);
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAV] END!");

}


PUBLIC MCI_ERR_T MMC_WavGetFileInformation (
                        CONST INT32 FileHander,
                         AudDesInfoStruct  * CONST DecInfo  )
{

  WAV_PLAY WavPlay;
 WAV_INPUT *input = &WavPlay.WavInput;
  int32 result;

   hal_HstSendEvent(SYS_EVENT,0x11140010);
    hal_HstSendEvent(SYS_EVENT,g_WavPlayFlag);
    hal_HstSendEvent(SYS_EVENT,FileHander);
   if(g_WavPlayFlag == 0)
   {
        input->fileOffset=0;
         //judge the file handler
    	input->fileHandle = FileHander;
    	if (input->fileHandle<0)
    	{
    		diag_printf("[MMC_WAV]MMC_WavGetFileInformation FILE HANDLER ERROR!: %d \n", FileHander);
    		return MCI_ERR_CANNOT_OPEN_FILE;
    	}
    	else
    	{
    		diag_printf("[MMC_WAV] MMC_WavGetFileInformation FILE HANDLER: %d\n", FileHander);
    	}

       if ((g_WAVFILESIZE=mmc_aud_GetFileSize(FileHander))< 0)
       {
           MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_ERROR]CAN NOT GET FILE SIZE!:%d",g_WAVFILESIZE);
           return MCI_ERR_CANNOT_OPEN_FILE;
       }
	
       g_WAVHeaderLength=0;
 
      	//parse wav file header
    	if((result= MMC_WavHeaderParse(&WavPlay, 0))!=MCI_ERR_NO)
    	{
    		return result;
    	}
    }
									// get the infromation.
	DecInfo->sampleRate             = WavPlay.WavOutput.SampleRate;
	DecInfo->stereo                 = WavPlay.WavOutput.Channel!=1;
    DecInfo->bitRate = WavPlay.WaveHeader.nAvgBytesPerSec*8;
    if(DecInfo->bitRate == 0)
        DecInfo->time = 0;
    else
        DecInfo->time = g_WAVFILESIZE*8/DecInfo->bitRate*1000+500;

    hal_HstSendEvent(SYS_EVENT,0x89200000);
    hal_HstSendEvent(SYS_EVENT,DecInfo->sampleRate);
    hal_HstSendEvent(SYS_EVENT,DecInfo->bitRate);
    hal_HstSendEvent(SYS_EVENT,DecInfo->time);
    
    diag_printf("[MMC_WAV]MMC_WavGetFileInformation DecInfo->sampleRate : %d , DecInfo->stereo : %d\n", DecInfo->sampleRate,DecInfo->stereo );
					
      return MCI_ERR_NO;

}




/*
* NAME:	MMC_AudioWavDecOpen()
* DESCRIPTION: Initialize wav decoder.
*/

MCI_ERR_T MMC_AudioWavDecOpen (HANDLE fhd, WAV_PLAY *pWavPlayer, INT16 resumeFlag)
{
    if(pWavPlayer == NULL)
        pWavPlayer = WavPlayer = COS_Malloc(sizeof(WAV_PLAY));
	WAV_INPUT *input = &pWavPlayer->WavInput;
	WAV_OUTPUT *output = &pWavPlayer->WavOutput;

	
	//int32 readlen = 0;
	int32 result;

	LeftWavDataPlayFlag = 0;

	hal_HstSendEvent(SYS_EVENT,0xad020000);

#ifdef BT_SOURCE_SPEAKER
    g_WavOutputPcmBufSize =(8*1024); // in bytes
#else
    g_WavOutputPcmBufSize =(16*1024); // in bytes
#endif
    g_nFrame = (g_Wav16BitInputBufSize/(g_WavOutputPcmBufSize>>1));

    g_WavInputBufSize =(g_WavOutputPcmBufSize);

    data_size = (g_WavOutputPcmBufSize>>1);

    g_Wavcounterframe=0;
    g_WavResampleInitFlag=1;

    input->fileOffset=0;
    input->is_first_frame=1;
    g_sbc_frame =2;  
	//judge the file handler
	input->fileHandle = fhd;
	if(input->fileHandle<0)
	{
		diag_printf("[MMC_WAV]FILE HANDLER ERROR!: %d \n", fhd);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}
	else
	{
		diag_printf("[MMC_WAV] FILE HANDLER: %d\n", fhd);
	}
	

	input->fileTotalLen=mmc_aud_GetFileSize(fhd);
	
	diag_printf("[MMC_WAV]FILE TOTAL LENGTH:%d\n",input->fileTotalLen);

	g_WAVHeaderLength=0;
	
	//parse wav file header
	if((result= MMC_WavHeaderParse(pWavPlayer, resumeFlag))!=MCI_ERR_NO)
	{
		return result;
	}

	g_WAVConsumedLength=g_WAVConsumedLength-g_WAVHeaderLength;

	if(g_WAVConsumedLength<0)g_WAVConsumedLength=0;

	g_WAVConsumedLength=g_WAVConsumedLength&0xfffffffC;

 	if((g_WAVConsumedLength>0)&& ( input->codec != 1 ) ){ //ADPCM
 	
        int times;
     MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]g_WAVConsumedLength :%d",g_WAVConsumedLength);

	   times = g_WAVConsumedLength / input->block_align;
	   
          g_WAVConsumedLength = (times+1)*input->block_align;
	  MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]g_WAVConsumedLength :%d ,times: %d",g_WAVConsumedLength,times);	  

 	}

//tianwq		hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);
	output->Buffer= (uint32 *)pcmbuf_overlay;
	memset(output->Buffer, 0, g_WavOutputPcmBufSize);
	//output->Size= (g_WavOutputPcmBufSize) >> 2;//int 
	output->Buffer_head=output->Buffer;
    output->Channel=2;
	//output->OnPCM = MMC_AudioWavPcmISR;
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]g_WAVConsumedLength :%d",g_WAVConsumedLength);
	
	result=mmc_aud_FileSeek(input->fileHandle,g_WAVConsumedLength,FS_SEEK_CUR);

	input->fileOffset=g_WAVConsumedLength+g_WAVHeaderLength;

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]FILE SEEK POSITION:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV_ERR]FS_seek ERROR!");
		return MCI_ERR_BAD_FORMAT;
	}
	
	//fill the pcm out buffer first,only consider that the pcm out module is 16bit situation
	if(WAV_DATA_DEPTH_16BIT != output->Bits)
	{
	    //g_WavInputBufSize = g_WavInputBufSize*output->Bits/WAV_DATA_DEPTH_16BIT;
    }

    if(1 == input->channels)
    {
        g_WavInputBufSize /= 2;
    }
    if(WAV_DATA_DEPTH_8BIT == output->Bits)
    {
        g_WavInputBufSize /= 2;
    }
    
	output->Size = (g_WavOutputPcmBufSize) >> 2;

#if ADPCM_SUPPORT==1
    if(input->codec != 1  ) // adpcm
    {
        if(pWavPlayer->WavInput.codec_id == AdpcmCODEC_ID_NONE)
            return MCI_ERR_BAD_FORMAT;

		g_WavOutputPcmBufSize = 8*1024; // in bytes
		g_WavInputBufSize = input->block_align;
		if((g_WavOutputPcmBufSize + g_WavInputBufSize + g_WavInputBufSize*4 + 32) > 16*1024)
			return MCI_ERR_OUT_OF_MEMORY;
		
		output->Size = (g_WavOutputPcmBufSize) >> 2; //in int
		memset(output->Buffer, 0, g_WavOutputPcmBufSize);
		
        input->bof = ((uint8 *)pcmbuf_overlay)+g_WavOutputPcmBufSize  ;//mmc_MemMalloc(g_WavInputBufSize);
        memset(input->bof, 0, g_WavInputBufSize);
        input->length = g_WavInputBufSize;
        input->data = input->bof;
        input->eof = input->bof + input->length;

        input->priv_data   = (uint8 *)mmc_MemMalloc(sizeof(ADPCMContext));
	    output->tmpBuffer = ((uint8 *)pcmbuf_overlay)+g_WavOutputPcmBufSize+g_WavInputBufSize ;// (uint8 *) mmc_MemMalloc(WAV_ADPCM_TMP_BUFSIZE);
	    output->tmpBuf_read = NULL;
	    output->Remain_Size = 0;
		data_size = 16*1024 - g_WavOutputPcmBufSize - g_WavInputBufSize; //in byte, for output->tmpBuffer length
	  
        if((input->priv_data == NULL)|(output->tmpBuffer == NULL))
	    {
            //tianwq hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
    		mmc_MemFreeAll();
    		return MCI_ERR_OUT_OF_MEMORY;
			
	    }

        adpcm_decode_init(input );

        output->Bits = WAV_DATA_DEPTH_16BIT;

	    diag_printf("[MMC_ADPCM]  Finished adpcm_decode_init:  codec_id is  0x%x    \n", input->codec_id);
     }

#endif

#if 0
          hal_HstSendEvent(SYS_EVENT,0x09190001);
          hal_HstSendEvent(SYS_EVENT,output->Bits);
          hal_HstSendEvent(SYS_EVENT,output->SampleRate);
          hal_HstSendEvent(SYS_EVENT,output->Channel);
          hal_HstSendEvent(SYS_EVENT,input->codec_id);
          hal_HstSendEvent(SYS_EVENT,input->channels);
          hal_HstSendEvent(SYS_EVENT,input->block_align);
#endif
		
/*	else if(WAV_DATA_DEPTH_16BIT == output->Bits)
	{
		input->bof = ((uint8 *)pcmbuf_overlay)+g_WavOutputPcmBufSize;//mmc_MemMalloc(g_Wav16BitInputBufSize);


		if(input->bof==NULL)
		{
//tianwq				hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
			mmc_MemFreeAll();
			return MCI_ERR_OUT_OF_MEMORY;
			
		}
		
		memset(input->bof, 0, g_Wav16BitInputBufSize);
		input->length = g_Wav16BitInputBufSize;
		input->data = input->bof;
	
		input->eof = input->bof + input->length;
		
		output->Size = (g_WavOutputPcmBufSize) >> 2;

         if(input->codec == 1  ){ //PCM
		 	
		readlen = mmc_aud_FileRead(input->fileHandle,input->bof,g_Wav16BitInputBufSize);
		
		if(readlen < g_Wav16BitInputBufSize)
		{
			MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]BUFFER IS NOT FULL!");
			
			g_input_buf_not_full = 1;
			g_buffered_frame = readlen/(g_WavOutputPcmBufSize>>1)+1;
			g_copyed_frame = 0;
			g_stop_read = 0;
		}
		else
		{
			input->fileOffset+=readlen;
			g_copyed_frame = 0;
			g_readed_frame = 0;
			g_stop_read = 1;
			g_input_buf_not_full = 0;
		}

         	}

		
		g_read_end = 0;
	}
    */
	g_continue_play_flag = 0;


#ifdef BT_SOURCE_SPEAKER
    INT16 ch;
    HAL_ERR_T voc_ret = 0;

    g_SbcDataLen = 0;
    
	g_WavToSbcBuffer=(INT8 *)pcmbuf_overlay+g_WavOutputPcmBufSize;//(SBC_DATA_BUF_LEN);   

	memset(g_WavToSbcBuffer, 0, SBC_DATA_BUF_LEN);
	
	/*initialize I/O data struct*/
	//G_VppSBCEncBakConstY = (INT32*)mmc_MemMalloc(468*4);

    hal_HstSendEvent(SYS_EVENT,0xad020001);
    hal_HstSendEvent(SYS_EVENT,G_VppSBCEncBakConstY);

	//if(G_VppSBCEncBakConstY==NULL)
	//{
	//	mmc_MemFreeAll();
	//	return MCI_ERR_OUT_OF_MEMORY;		
	//}

	g_WavDataLen = 0;
	
	pWavPlayer->Voc_AudioDecStatus.consumedLen = 0;
	pWavPlayer->Voc_AudioDecStatus.nbChannel = 0;
	pWavPlayer->Voc_AudioDecStatus.output_len = 0;
	pWavPlayer->Voc_AudioDecStatus.streamStatus= 0;
		
	pWavPlayer->Voc_AudioDecIN.EQ_Type=-1;
	pWavPlayer->Voc_AudioDecIN.reset=1;

	/*set sbc parameter*/
	//check sample, sbcenc support 8k->32k & 16k->32k;

	hal_HstSendEvent(SYS_EVENT, 0xad020002);
    hal_HstSendEvent(SYS_EVENT, output->SampleRate);
    
    switch(output->SampleRate)
    {
        case 8000  : ch = SBC_FREQ_32000; break;
        case 16000 : ch = SBC_FREQ_32000; break;
        case 32000 : ch = SBC_FREQ_32000; break;      
        case 44100 : ch = SBC_FREQ_44100; break;
        case 48000 : ch = SBC_FREQ_48000; break;

        default    : return MCI_ERR_ERROR; 
    }
    SbcParse.source_ch = output->Channel;
    SbcParse.source_sample = output->SampleRate;
    
    SbcParse.sbcenc_sample = ch;
    SbcParse.sbcenc_ch_mode = SBC_MODE_JOINT_STEREO;
    SbcParse.sbcenc_bitpool = 32;
    SbcParse.sbcenc_allocation = SBC_AM_LOUDNESS;

	/*open voc decoder*/

	voc_ret = vpp_AudioSbcEncOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioWavVocISR);

	hal_HstSendEvent(SYS_EVENT, 0xad020003);
	hal_HstSendEvent(SYS_EVENT, voc_ret);
	
	switch (voc_ret)
	{
	case HAL_ERR_NO:
		break;
		
	case HAL_ERR_RESOURCE_RESET:
	case HAL_ERR_RESOURCE_BUSY:  
	case HAL_ERR_RESOURCE_TIMEOUT:
	case HAL_ERR_RESOURCE_NOT_ENABLED:	  
	case HAL_ERR_BAD_PARAMETER:
	case HAL_ERR_UART_RX_OVERFLOW:
	case HAL_ERR_UART_TX_OVERFLOW:
	case HAL_ERR_UART_PARITY:
	case HAL_ERR_UART_FRAMING:
	case HAL_ERR_UART_BREAK_INT:
	case HAL_ERR_TIM_RTC_NOT_VALID:
	case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
	case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
	case HAL_ERR_COMMUNICATION_FAILED:
	case HAL_ERR_QTY:
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_WAVSBC_ERROR]Fail to Call vpp_AudioSbcEncOpen(): %d \n", voc_ret);
		return MCI_ERR_ERROR;
	default:
		break;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_WAVSBC]vpp_AudioSbcEncOpen end!");

#endif
    
	MMC_WavPcmOutStart(pWavPlayer);
    g_WavPlayFlag= 1;
	
	return MCI_ERR_NO;
	
}

#ifdef BT_SOURCE_SPEAKER

MCI_ERR_T MMC_AudioWavDecVoC(WAV_PLAY *pWavPlayer)
{

#ifdef WAV_TO_SBC_TICKS_DEBUG
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x33300000 + sbcenc_tick - g_WavSbcTicks);
	g_WavSbcTicks = sbcenc_tick;
#endif

	WAV_INPUT *input = &pWavPlayer->WavInput;
	WAV_OUTPUT *output = &pWavPlayer->WavOutput;
	
	vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus = &pWavPlayer->Voc_AudioDecStatus;
	vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pWavPlayer->Voc_AudioDecIN;

    static int sbclen, sbcaddr;

	UINT32 sbc_length = 0;
	UINT8* sbc_output_addr =  (UINT8*) hal_VocGetPointer(SBC_Out_put_addr);
	
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_WAVSBC]MMC_AudioSbcEncVoc! WavDataLen = %d", g_WavDataLen);
    
    if(input->is_first_frame==1)
    {
        input->is_first_frame=0;		  
        return MCI_ERR_NO;
	}  
	
    vpp_AudioSbcEncStatus(pVoc_AudioDecStatus);
	sbc_length = pVoc_AudioDecStatus->ImgWidth;

    if(sbclen != sbc_length || sbcaddr != sbc_output_addr)
    {
		sbclen = sbc_length;
		sbcaddr = sbc_output_addr;
		
		hal_HstSendEvent(SYS_EVENT, 0xad010020);
		hal_HstSendEvent(SYS_EVENT, pVoc_AudioDecStatus->ErrorStatus);
		hal_HstSendEvent(SYS_EVENT, sbc_length);
		hal_HstSendEvent(SYS_EVENT, sbc_output_addr);
	}

    if(g_SbcDataLen + sbc_length < SBC_DATA_BUF_LEN)
    {
	    memcpy((UINT8*)g_WavToSbcBuffer+g_SbcDataLen, sbc_output_addr, sbc_length);
	    g_SbcDataLen += sbc_length;
    }
    else
    {
#ifdef WAV_TO_SBC_DEBUG
	    hal_HstSendEvent(SYS_EVENT, 0xad010021);
	    hal_HstSendEvent(SYS_EVENT, g_SbcDataLen);
	    hal_HstSendEvent(SYS_EVENT, g_WavToSbcBuffer);
        hal_DbgAssert("You can dump wav to sbc data from 0x%x, length = %d...", g_WavToSbcBuffer, g_SbcDataLen);
#endif
    }

#if 1 //send to bt
	if(bt_start_send_wav_sbc_flag)
	{
		u_int8 btsend_frame_num = 0;
		u_int8 sbcenc_frame_num = 0;
        u_int8 sbcenc_one_frame_len = 0;
        
        if(g_SbcToBtBuffer == NULL)
		{
			g_SbcToBtBuffer =(INT8 *)pcmbuf_overlay+g_WavOutputPcmBufSize+SBC_DATA_BUF_LEN;  
		}
		
		if(frame_sbc_msg == NULL)
		{
			frame_sbc_msg = (A2DP_SOURCE_MSG *)COS_Malloc(sizeof(A2DP_SOURCE_MSG));
			
			frame_sbc_msg->sbc_data = (UINT8 *)(g_SbcToBtBuffer);


			// mono: 1024 byte pcm -> 4 frame sbc; dual: 1024 byte pcm -> 2 frame sbc;
    		sbcenc_frame_num = 4>>(SbcParse.source_ch-1);
    		sbcenc_one_frame_len = sbc_length/sbcenc_frame_num;
    		btsend_frame_num = source_mtu/sbcenc_one_frame_len;

    		if(btsend_frame_num > 8)
    		{
                btsend_frame_num = 8;
    		}

    		frame_sbc_msg->frame = btsend_frame_num;
    		frame_sbc_msg->sbc_data_len = frame_sbc_msg->frame*sbcenc_one_frame_len;

            hal_HstSendEvent(SYS_EVENT, 0xad010022);	
            hal_HstSendEvent(SYS_EVENT, sbcenc_frame_num);
            hal_HstSendEvent(SYS_EVENT, sbcenc_one_frame_len);
            hal_HstSendEvent(SYS_EVENT, btsend_frame_num);
            
            hal_HstSendEvent(SYS_EVENT, frame_sbc_msg->sbc_data);
            hal_HstSendEvent(SYS_EVENT, frame_sbc_msg->sbc_data_len);
            hal_HstSendEvent(SYS_EVENT, frame_sbc_msg->frame);

		}
	
		if(g_SbcDataLen >= frame_sbc_msg->sbc_data_len)
		{
			//static u_int8 sendcount = 0;
			//sendcount++;
			//if(sendcount > 200)
			//{
			//	sendcount = 0;
            //    hal_HstSendEvent(SYS_EVENT, 0xad010023);	
				//hal_HstSendEvent(SYS_EVENT, sbc_length*2);	
				//hal_HstSendEvent(SYS_EVENT, sbc_remain_len);	
			//}			

			memcpy(frame_sbc_msg->sbc_data ,g_WavToSbcBuffer,frame_sbc_msg->sbc_data_len);
			
			app_a2dp_send_data_handle(frame_sbc_msg);
			
			g_SbcDataLen -= frame_sbc_msg->sbc_data_len;

			memcpy(g_WavToSbcBuffer ,((UINT8 *)g_WavToSbcBuffer)+frame_sbc_msg->sbc_data_len,g_SbcDataLen);
		}

	  }
#endif

	if(g_WavDataLen >= 1024)//feed pcm data to voc
	{
        memcpy((UINT8*)hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR),\
               (INT8*)output->Buffer_head+(g_WavOutputPcmBufSize/2-g_WavDataLen), 1024);
        g_WavDataLen -= 1024;       

		pVoc_Voc_AudioDecIN->mode=0xd;                               //sbcenc
		pVoc_Voc_AudioDecIN->BsbcEnable=1;
		pVoc_Voc_AudioDecIN->audioItf=0;
		
		pVoc_Voc_AudioDecIN->Source_width=SbcParse.sbcenc_ch_mode;
		pVoc_Voc_AudioDecIN->Source_height=SbcParse.sbcenc_allocation;
		
		pVoc_Voc_AudioDecIN->Cut_width=SbcParse.sbcenc_bitpool;
		
		pVoc_Voc_AudioDecIN->Zoomed_width=SbcParse.source_ch;     // inputch
		pVoc_Voc_AudioDecIN->Zoomed_height=SbcParse.source_sample;
		
		pVoc_Voc_AudioDecIN->inStreamUBufAddr=(INT32*)G_VppSBCConstX;
		pVoc_Voc_AudioDecIN->inStreamVBufAddr=(INT32*)G_VppSBCEncBakConstY;
		  
		vpp_AudioSbcEncScheduleOneFrame(pVoc_Voc_AudioDecIN);
		sbcenc_voc_busy = 1;
	}
   return MCI_ERR_NO;

}

#endif
/*
* NAME:	MMC_AudioWavDecPcm()
* DESCRIPTION: Initialize wav decoder.
*/
MCI_ERR_T MMC_AudioWavDecPcm(WAV_PLAY *pWavPlayer)
{
	WAV_INPUT *input = &pWavPlayer->WavInput;
	WAV_OUTPUT *output = &pWavPlayer->WavOutput;
	int32 i,readlen;
   int32 length;
   CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
  
    g_wav_read_flag=1;
    g_sbc_frame=0;

	if(g_continue_play_flag==1)
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]END PLAYING !");
		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_PCM_END);
	}
	else if(g_continue_play_flag==2)
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]CONTINUE PLAY!");
		g_continue_play_flag--;

		if(WAV_DATA_DEPTH_8BIT == pWavPlayer->WavOutput.Bits)
		{
			MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]8 BIT WAV END PLAYING !");
			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_WAV_PCM_END);
		}
		
	}
	else
	{
		//check the remain data 
//		if(WAV_DATA_DEPTH_8BIT == pWavPlayer->WavOutput.Bits)
		{
			if((input->fileTotalLen-input->fileOffset) < (g_WavInputBufSize))
			{
				return MCI_ERR_END_OF_FILE;
			}
		}
	/*	else if(WAV_DATA_DEPTH_16BIT == pWavPlayer->WavOutput.Bits)
		{
			if((input->fileTotalLen-input->fileOffset) < g_WavInputBufSize)
			{
				//return MCI_ERR_END_OF_FILE;
				g_read_end = 1;
			}
		}*/

	
		if(Mmc_Wav_Pcm_Half==0)
		{
			output->Buffer_head=output->Buffer; 		
		}
		else if(Mmc_Wav_Pcm_Half==1)
		{
			output->Buffer_head=output->Buffer+(output->Size>>1);			
		}
		
		g_WavBTDataAddress=(INT16 *)output->Buffer_head;
        if(input->codec == 1  ) //PCM
		{
			readlen = mmc_aud_FileRead(input->fileHandle, (UINT8*)output->Buffer_head, g_WavInputBufSize >> 1);

			if(readlen<0)
			{
				return MCI_ERR_ERROR;
			}
			else if(readlen < (g_WavInputBufSize >> 1))
			{
			    memset(((UINT8*)output->Buffer_head) + readlen, 0, (g_WavInputBufSize >> 1) - readlen);
			    if(LeftWavDataPlayFlag)
			    {
                            return MCI_ERR_END_OF_FILE;
			    }
			}

			input->fileOffset+=readlen;

			
			g_WAVConsumedLength+=readlen;//(g_WavOutputPcmBufSize>>2);

            //if(WAV_DATA_DEPTH_16BIT == pWavPlayer->WavOutput.Bits)
            {
             //memcpy(output->Buffer_head, input->data, readlen);
            }
            if(WAV_DATA_DEPTH_8BIT == output->Bits)
            {
        		INT16 *PCM_p=(INT16 *)output->Buffer_head;
                INT8 *data = (INT8 *)output->Buffer_head;
                    
        		for(i=readlen-1;i>=0;i--)
        		{
                    PCM_p[i]=((INT16)((INT8)(data[i])-0x80))<<8;
        		}
            }
		}
#if ADPCM_SUPPORT==1
        else
        {
        
                 
		   uint32 remain_pcm = 0 ;
		   uint32 tempbuf_len=0;
		   
		   uint8 *Buffer_end;
           if((2==output->Channel)&&(1==input->channels))
                Buffer_end = (uint8 *)  ( output->Buffer_head +(output->Size>>2)) ;
           else
               Buffer_end = (uint8 *)  ( output->Buffer_head +(output->Size>>1)) ;

            output->write =(uint8 *) output->Buffer_head;
            
#if 0
                 hal_HstSendEvent(SYS_EVENT,0x09170000);
                 hal_HstSendEvent(SYS_EVENT,Buffer_end);
                 hal_HstSendEvent(SYS_EVENT,output->write);
                 hal_HstSendEvent(SYS_EVENT,output->Remain_Size);
                 hal_HstSendEvent(SYS_EVENT,output->tmpBuf_read);
#endif		  
                remain_pcm =  Buffer_end -output->write;

                if(output->Remain_Size >= remain_pcm)
                {
	                memcpy(output->write, output->tmpBuf_read, remain_pcm);
	                output->tmpBuf_read += remain_pcm;
	                output->Remain_Size -= remain_pcm;
                }
                else
                {
                   if(output->Remain_Size > 0)
                   {
                     memcpy(output->write,   output->tmpBuf_read ,output->Remain_Size);
                     output->write += output->Remain_Size;
                     output->Remain_Size = 0;
                   }

                   remain_pcm =  Buffer_end -output->write;
                   do{
                       readlen = mmc_aud_FileRead(input->fileHandle,input->bof,input->block_align);

           	           if(readlen  < input->block_align){
                             g_read_end = 1;
                            return MCI_ERR_END_OF_FILE;
                       }
                         
             	       tempbuf_len =  data_size;
        	           length =  adpcm_decode_frame(input,output->tmpBuffer,(INT32 *)&tempbuf_len, input->bof,readlen);

                     if(length<=0) // decode error
                        {
                            return MCI_ERR_UNKNOWN_FORMAT;
                        }
                     
        	           g_WAVConsumedLength+=readlen;// length;

#if 0
                       hal_HstSendEvent(SYS_EVENT,0x09170003);
                       hal_HstSendEvent(SYS_EVENT,readlen);
                       hal_HstSendEvent(SYS_EVENT,length);
                       hal_HstSendEvent(SYS_EVENT,data_size);
                       hal_HstSendEvent(SYS_EVENT,remain_pcm);
#endif
                       if(remain_pcm > tempbuf_len)
                       {
                            memcpy(output->write, output->tmpBuffer, tempbuf_len );
                            remain_pcm -= tempbuf_len;
                            output->write += tempbuf_len;
                        }
                       else
                        {
                           memcpy(output->write, output->tmpBuffer, remain_pcm );
            				
                           output->tmpBuf_read =  output->tmpBuffer + remain_pcm ;
                           output->Remain_Size =  tempbuf_len - remain_pcm;
                           remain_pcm = 0;
                        }
                   } while(remain_pcm>0);
                 }
           }
#endif
            if((2==output->Channel)&&(1==input->channels))
            {
                INT16 *PCM_p=(INT16 *)output->Buffer_head;
                for(i=(output->Size)-2 ;i>=0;i=i-2)
                {
                    PCM_p[i]=PCM_p[i+1]=PCM_p[i>>1];
                }
                
            }
		    else  if((2==output->Channel)&&audioCfg[audioItf].mixAudio)//(AUD_ITF_LOUD_SPEAKER==audioItf||AUD_ITF_BT_SP==audioItf))
			{

				INT16 *PCM_p=(INT16 *)output->Buffer_head;
				
				for(i=0;i<(g_WavOutputPcmBufSize>>2);i=i+2)
				{
					PCM_p[i]=PCM_p[i+1]=(PCM_p[i]>>1)+(PCM_p[i+1]>>1);					
				}
				
			}			


            MCI_MergeSideTone(g_WavOutputPcmBufSize/2, (INT16*)output->Buffer_head);
            
#ifdef BT_SOURCE_SPEAKER   
	        vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pWavPlayer->Voc_AudioDecIN;
	        if(g_WavDataLen > 0)  //check last pcm data is left or not;
	        {
            	hal_HstSendEvent(SYS_EVENT, 0xad010003);
                hal_HstSendEvent(SYS_EVENT, g_WavDataLen);
	        }
            g_WavDataLen = g_WavOutputPcmBufSize/2;
        	if(sbcenc_voc_busy == 0)//feed pcm data to voc
        	{
                memcpy((UINT8*)hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR),\
                       (INT8*)output->Buffer_head+(g_WavOutputPcmBufSize/2-g_WavDataLen), 1024);
                g_WavDataLen -= 1024;       

        		pVoc_Voc_AudioDecIN->mode=0xd;                               //sbcenc
        		pVoc_Voc_AudioDecIN->BsbcEnable=1;
        		pVoc_Voc_AudioDecIN->audioItf=0;
        		
        		pVoc_Voc_AudioDecIN->Source_width=SbcParse.sbcenc_ch_mode;
        		pVoc_Voc_AudioDecIN->Source_height=SbcParse.sbcenc_allocation;
        		
        		pVoc_Voc_AudioDecIN->Cut_width=SbcParse.sbcenc_bitpool;
        		
        		pVoc_Voc_AudioDecIN->Zoomed_width=SbcParse.source_ch;     // inputch
        		pVoc_Voc_AudioDecIN->Zoomed_height=SbcParse.source_sample;
        		
        		pVoc_Voc_AudioDecIN->inStreamUBufAddr=(INT32*)G_VppSBCConstX;
        		pVoc_Voc_AudioDecIN->inStreamVBufAddr=(INT32*)G_VppSBCEncBakConstY;
        		  
        		vpp_AudioSbcEncScheduleOneFrame(pVoc_Voc_AudioDecIN);
        		sbcenc_voc_busy = 1;
        	}
#endif

#if 0		

			 if(g_WavBT8KEnbleFlag==TRUE)
			{

				if(output->Channel==2)
				{
					INT16 *PCM_p=(INT16 *)g_WavBTDataAddress;
					
					for(i=0;i<(g_WavOutputPcmBufSize>>2);i=i+2)
					{
						PCM_p[i/2]=(PCM_p[i]>>1)+(PCM_p[i+1]>>1);					
					}
				}

				/* Resample stuff in input buffer */        

				g_WavNout=ResampleOneFrame(g_WavBTDataAddress,g_WavBT8kPcmBuffer+g_Wavcounterframe*g_WavNumOfOutSample);

				g_Wavcounterframe++;

				if(g_Wavcounterframe>=g_WavTotalFrame)
				{
					g_Wavcounterframe=0;
				}

	          		MCI_TRACE (TSTDOUT,0,"g_WavNout = %d",g_WavNout);  

			}
		    if(input->codec == 1  ) //PCM
		    	{
		    	
			g_copyed_frame++;
			
			if(g_copyed_frame == g_nFrame)
			{
				MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]COPYED FRAME IS RESET!");
				g_copyed_frame = 0;
			}
			
			if(g_read_end)
			{
				if(g_copyed_frame == g_readed_frame)
				{
					MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]END OF FILE!");
					return MCI_ERR_END_OF_FILE;
				}
			}
			
		    	}
			//patch for short file
			if(g_input_buf_not_full)
			{
				if(0 == g_buffered_frame)
				{
					return MCI_ERR_END_OF_FILE;
				}		

				if(g_copyed_frame == g_buffered_frame)
				{
					return MCI_ERR_END_OF_FILE;
				}
			}
			

	            if(input->codec == 1  ) //PCM
		    	{
		    	
			if(g_stop_read)
			{
				FS_AsynReadReq(input->fileHandle, input->bof + g_readed_frame * (g_WavOutputPcmBufSize>>1), g_WavOutputPcmBufSize>>1, MMC_AudioWavFileReadFinish);
				g_stop_read = 0;
			}
	            	}
				
		}
#endif
	}
g_wav_read_flag=0;
//wav_pcm_irq_num--;
	return MCI_ERR_NO;
}

/*
* NAME:	MMC_AudioWavDecClose()
* DESCRIPTION: Close wav decoder.
*/
MCI_ERR_T MMC_AudioWavDecClose (WAV_PLAY *pWavPlayer)
{

	aud_StreamStop(audioItf);
	mmc_MemFreeAll();
	g_WavPlayFlag = 0;
//tianwq		hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);

    if(WavPlayer != NULL)
    {
        COS_Free(WavPlayer);
        WavPlayer = NULL;
    }
    
#ifdef BT_SOURCE_SPEAKER   
    vpp_AudioSbcEncClose();

    if(frame_sbc_msg != NULL)
    {
        COS_Free(frame_sbc_msg);
        frame_sbc_msg = NULL;
    }

    if(g_SbcToBtBuffer != NULL)
    {
        g_SbcToBtBuffer = NULL;
    }
#endif

	wav_pcm_irq_num = 0;
	LeftWavDataPlayFlag = 0;
	return  MCI_ERR_NO;
	
}


int32 Audio_WavPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
	int32 result;

    if(WavPlayer == NULL)
        WavPlayer = COS_Malloc(sizeof(WAV_PLAY));

	WavPlayer->WavOutput.OutputPath = OutputPath;
	g_WAVConsumedLength=0;


	if (fhd< 0)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_ERROR]ERROR FILE HANDLER: %d \n", fhd);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	if ((g_WAVFILESIZE=mmc_aud_GetFileSize(fhd))< 0)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_ERROR]CAN NOT GET FILE SIZE!:%d",g_WAVFILESIZE);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_WAV]FILE SIZE: %d",g_WAVFILESIZE);

	g_WAVConsumedLength=(INT32)(((INT64)PlayProgress*(INT64)g_WAVFILESIZE)/10000);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_WAV]g_AMRConsumedLength: %d",g_WAVConsumedLength);

    result=mmc_aud_FileSeek(fhd,0,FS_SEEK_SET);
	if((result= MMC_AudioWavDecOpen(fhd, WavPlayer, 0))!=MCI_ERR_NO)
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV_ERR]MMC_AudioDecOpen() RETURN FALSE!");
		MMC_AudioWavDecClose(WavPlayer);
		return result;
	}
    
	return  MCI_ERR_NO;
}

int32 Audio_WavStop (void) 	
{
	MMC_AudioWavDecClose(WavPlayer);	
	return  MCI_ERR_NO;
}

int32 Audio_WavPlayLeftData (void)
{
    //clear the flag used to play left data in the input buffer and pcm buffer
    LeftWavDataPlayFlag=1;

    return  MCI_ERR_NO;
}

int32 Audio_WavPause (void)	
{
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV] Audio_WavPause!!!!");
	if (WavPlayer != NULL)
	{
		MMC_AudioWavDecClose(WavPlayer);
	}
	return  MCI_ERR_NO;
}


int32 Audio_WavResume ( HANDLE fhd) 
{
	int32 result;
	
	MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV]Audio_WavResume!");
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV] Audio_WavResume!!!!");

#ifndef CODEC_SUPPORT

	g_WAVConsumedLength-=(g_WavOutputPcmBufSize);

	if(g_WAVConsumedLength<0)g_WAVConsumedLength=0;



	result=mmc_aud_FileSeek(fhd,0,FS_SEEK_SET);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_WAV]FILE SEEK POSITON:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV_ERR]FS_seek error!");
		return MCI_ERR_BAD_FORMAT;
	}
#endif

	if((result= MMC_AudioWavDecOpen(fhd, WavPlayer, 1))!=MCI_ERR_NO)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV_ERR]MMC_AudioDecOpen RETURN FALSE!");
		MMC_AudioWavDecClose(WavPlayer);
		return result;
	}
	return  MCI_ERR_NO;
}


int32 Audio_WavGetID3 (char * pFileName)  
{
	return 0;	
}

int32 Audio_WavGetPlayInformation (MCI_PlayInf * MCI_PlayInfWAV)  
{
    if(g_WAVFILESIZE == 0)
        MCI_PlayInfWAV->PlayProgress = 0;
    else
    	MCI_PlayInfWAV->PlayProgress=(INT32)((((INT64)g_WAVConsumedLength+g_WAVHeaderLength)*10000)/g_WAVFILESIZE);
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_WAV]PlayProgress:%d",MCI_PlayInfWAV->PlayProgress);
	
	return MCI_ERR_NO;
}

int32 Audio_WavUserMsg(COS_EVENT *ev)
{
	int32 nMsg = ev->nEventId;
    MCI_ERR_T result;
	switch (nMsg)
	{
		case MSG_MMC_WAV_PCM_INT:
			result = MMC_AudioWavDecPcm(WavPlayer);
            if(result==MCI_ERR_END_OF_FILE)
			{
				
				MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV]END OF FILE!");				
				g_continue_play_flag = 2;
			}
            else if(result != MCI_ERR_NO)
            {
    			MMC_AudioWavDecClose(WavPlayer);
				MCI_AudioFinished(result);
				mmc_SetCurrMode(MMC_MODE_IDLE);
            }
			break;

#ifdef BT_SOURCE_SPEAKER
		case MSG_MMC_WAV_SBC_VOC:

			MMC_AudioWavDecVoC(WavPlayer);
			break;

		case MSG_MMC_WAV_START_SEND_SBC:
		
            bt_start_send_wav_sbc_flag = 1;
            source_mtu = (u_int16)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0xad010000+0x30);
            hal_HstSendEvent(SYS_EVENT, source_mtu);
	        break;
#endif			
			
		case MSG_MMC_WAV_PCM_END:
			
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV]MSG_MMC_WAV_PCM_END\n");
			MMC_AudioWavDecClose(WavPlayer);
			MCI_AudioFinished(MCI_ERR_END_OF_FILE);
			mmc_SetCurrMode(MMC_MODE_IDLE);
			
			break;
			
		default:
			
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_WAV_ERR]ERROR MESSAGE!");
			
			break;
	}
    return 0;
}


