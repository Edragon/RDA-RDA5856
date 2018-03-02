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
// FILENAME: mmc_audioUSB.c
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
#include "csw_csp.h"

#include "audio_api.h"

#include "aud_m.h"
#include "fs_asyn.h"

#include "cos.h"
#include "mmc_audiousb.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "ars_m.h"
#include "event.h"

#include "vpp_audiosbc_enc.h"

#define MMC_USB_BUFF_SIZE               (480) // in pcm sample, total use 480*4 buffs, two for aif play, two for sco receive = 480*4*2bytes

static INT8 Mmc_USB_Pcm_Half;
static INT8 Mmc_USB_MIC_Half;

static INT8 g_continue_play_flag;
static INT32 g_sbc_frame;

USB_PLAY *USBPlayer;

extern AUD_LEVEL_T audio_cfg;
extern HAL_AIF_STREAM_T audioStream;
extern AUD_ITF_T audioItf;

extern HAL_AIF_STREAM_T micStream;

extern MPEG_PLAY *MpegPlayer;
INT32 *G_USBSBCConstY=NULL;
extern const INT32 G_VppSBCConstX[];
UINT8 uaudCodecOpen = 0;

INT32 add_byte;

extern PUBLIC UINT32 gpio_detect_earpiece(VOID);

/*
* NAME:	MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioUSBVocISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_USBSBC]VoC INT!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_USB_SBC_VOC);
} 

/*
* NAME:	MMC_AudioUSBHalfPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/

void MMC_AudioUSBHalfPcmISR(void)
{
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_USB]HALF!");
	Mmc_USB_Pcm_Half = 0;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_USB_PCM_INT);
}

/*
* NAME:	MMC_AudioUSBEndPcmISR()
* DESCRIPTION: ISR for pcm end buffer exhausted.
*/
void MMC_AudioUSBEndPcmISR(void)
{
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_USB]END!");
	Mmc_USB_Pcm_Half = 1;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_USB_PCM_INT);

}


/*
* NAME:	MMC_AudioUSBHalfPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/

void MMC_AudioUSBHalfMicISR(void)
{
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_USB]HALF!");
	Mmc_USB_MIC_Half = 0;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_USB_MIC_INT);
}

/*
* NAME:	MMC_AudioUSBEndPcmISR()
* DESCRIPTION: ISR for pcm end buffer exhausted.
*/
void MMC_AudioUSBEndMicISR(void)
{
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_USB]END!");
	Mmc_USB_MIC_Half = 1;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_USB_MIC_INT);

}

void MMC_USBSetFreq(UINT32 freq)
{
}

void MMC_USBPcmOutStart(USB_PLAY *pUSBPlayer)
{
        MCI_ERR_T result;
	
	// Stream out
	audioStream.startAddress  	= (UINT32 *)pUSBPlayer->USBOutput.Buffer;
	audioStream.length        	= pUSBPlayer->USBOutput.Size;
	audioStream.channelNb     	= HAL_AIF_STEREO;
    audioStream.sampleRate = HAL_AIF_FREQ_48000HZ;

	audioStream.voiceQuality   = !TRUE;    
	audioStream.halfHandler   	= MMC_AudioUSBHalfPcmISR;
	audioStream.endHandler    = MMC_AudioUSBEndPcmISR;
	
   	{
   	  	  // Initial cfg
   	  	  if(gpio_detect_earpiece())
    		audioItf = AUD_ITF_EAR_PIECE;
          else
            audioItf = AUD_ITF_LOUD_SPEAKER;
   	}

	//audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;
	
	result = aud_StreamStart(audioItf, &audioStream, &audio_cfg);

    // steam in
    micStream.startAddress  	= (UINT32 *)(pUSBPlayer->USBInput.data+MMC_USB_BUFF_SIZE);
	micStream.length        	= pUSBPlayer->USBInput.size;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
	micStream.channelNb     	= 1;//pUSBPlayer->USBOutput.Channel;//HAL_AIF_STEREO;//HAL_AIF_MONO;
	micStream.sampleRate    = HAL_AIF_FREQ_8000HZ;

	micStream.voiceQuality   = TRUE;    
	micStream.halfHandler    = MMC_AudioUSBHalfMicISR;
	micStream.endHandler    = MMC_AudioUSBEndMicISR;

    //result = aud_StreamRecord(audioItf, &micStream, &audio_cfg);
}



/*
* NAME:	MMC_AudioUSBDecOpen()
* DESCRIPTION: Initialize USB decoder.
*/
MCI_ERR_T MMC_AudioUSBDecOpen (USB_PLAY *pUSBPlayer, HANDLE sco_handle)
{
    if(pUSBPlayer == NULL)
        pUSBPlayer = USBPlayer = COS_Malloc(sizeof(USB_PLAY));
    USB_INPUT *input = &pUSBPlayer->USBInput;
    USB_OUTPUT *output = &pUSBPlayer->USBOutput;
    if(MpegPlayer == NULL)
        MpegPlayer = COS_Malloc(sizeof(MPEG_PLAY));
    SSHdl *my_ss_handle = MpegPlayer->MpegInput.ss_handle;
    bt_a2dp_audio_cap_struct * my_audio_config_handle=   MpegPlayer->MpegInput.audio_config_handle ;
     HAL_ERR_T voc_ret = 0;

    pUSBPlayer->in_flag = 0;
    input->sco_handle = sco_handle;

    output->Buffer= (uint16 *)pcmbuf_overlay;
    output->length = 0;
    output->Size = MMC_USB_BUFF_SIZE*4;
    memset(output->Buffer, 0, output->Size*2);

    input->data = (uint16*)(pcmbuf_overlay+MMC_USB_BUFF_SIZE*2);
    input->length = MMC_USB_BUFF_SIZE;
    input->size = MMC_USB_BUFF_SIZE*2;//(MCI_INPUT_BUF_SIZE/pUSBPlayer->packet_size-1)*pUSBPlayer->packet_size*2;
    memset(input->data, 0, input->size*2);

    g_continue_play_flag = 1;

    hal_HstSendEvent(USB_EVENT,0x10280020);

    hal_HstSendEvent(USB_EVENT,(UINT32)output->Buffer);
    hal_HstSendEvent(USB_EVENT,(UINT32)output->Size);
    hal_HstSendEvent(USB_EVENT,(UINT32)input->data);
    hal_HstSendEvent(USB_EVENT,(UINT32)input->size);

	 if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 1))      //BT_A2DP_sbc
	 {


	   pUSBPlayer->Voc_AudioDecStatus.consumedLen = 0;
	   pUSBPlayer->Voc_AudioDecStatus.nbChannel = 0;
	   pUSBPlayer->Voc_AudioDecStatus.output_len = 0;
	   pUSBPlayer->Voc_AudioDecStatus.streamStatus= 0;
			
	   pUSBPlayer->Voc_AudioDecIN.EQ_Type=-1;
	   pUSBPlayer->Voc_AudioDecIN.reset=1;

	G_USBSBCConstY = (INT32*)mmc_MemMalloc(468*4);
	if( G_USBSBCConstY==NULL)
	{
		mmc_MemFreeAll();
		return MCI_ERR_OUT_OF_MEMORY;		
	}

	/*open voc decoder*/
	voc_ret = vpp_AudioJpegDecOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioUSBVocISR);
	 
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
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_WAVSBC_ERROR]Fail to Call vpp_AudioJpegDecOpen()t: %d \n", voc_ret);
		return MCI_ERR_ERROR;
	default:
		break;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_WAVSBC]MMC_AudioDecOpen end!");
	}

    return MCI_ERR_NO;
}

int16 *MMC_USBGetRXBuffer(void)
{
	USB_OUTPUT *output = &USBPlayer->USBOutput;
    if(!g_continue_play_flag)
        return NULL;
    if(output->length >= MMC_USB_BUFF_SIZE*6)
    {
        hal_HstSendEvent(SYS_EVENT,0x84400002);
        output->length -= USBPlayer->packet_size/2;
    }
    return output->Buffer + output->length;
}

void MMC_USBClearRXBuffer(void)
{
    USB_OUTPUT *output = &USBPlayer->USBOutput;
    if(MMC_USBGetRXBuffer())
    {
        memset(output->Buffer, 0, MMC_USB_BUFF_SIZE*6);
        output->length = 0;
    }
}
BOOL MMC_USBPlugInVirtualWord(USB_PLAY *pUSBPlayer, int32 pcm_length)
{
    USB_OUTPUT *output = &pUSBPlayer->USBOutput;
    INT16 tmpWord, tmpNextWord;
    INT16 tmpLeftChlWord, tmpRightChlWord;
    INT32 zero_pcm_flag = -1;

    for(UINT32 i = 0; i < pcm_length; i++)
    {
        tmpWord =(INT16)(*(output->Buffer + output->length + i));
        tmpNextWord = (INT16)(*(output->Buffer + output->length + i + 2));

        if(tmpWord*tmpNextWord < 0)
        {
            zero_pcm_flag = i;
            hal_HstSendEvent(SYS_EVENT,0x13060401);
#if 0
            hal_HstSendEvent(SYS_EVENT,*(output->Buffer + output->length + zero_pcm_flag));
            hal_HstSendEvent(SYS_EVENT,*(output->Buffer + output->length + zero_pcm_flag + 2));
            hal_HstSendEvent(SYS_EVENT,zero_pcm_flag);
#endif
            break;
        }
    }

    if(zero_pcm_flag != -1)
    {
        if(!(zero_pcm_flag%2))//left channel
        {
            tmpLeftChlWord = ((INT16)(*(output->Buffer + output->length + zero_pcm_flag)) +
                                                    (INT16)(*(output->Buffer + output->length + zero_pcm_flag + 2)))/2;
            tmpRightChlWord = ((INT16)(*(output->Buffer + output->length + zero_pcm_flag + 1)) +
                                        (INT16)(*(output->Buffer + output->length + zero_pcm_flag + 3)))/2;
            memmove(output->Buffer + output->length + zero_pcm_flag + 4 , output->Buffer + output->length + zero_pcm_flag + 2, (pcm_length - zero_pcm_flag - 2)*2);
            *(output->Buffer + output->length + zero_pcm_flag + 2) = tmpLeftChlWord;
            *(output->Buffer + output->length + zero_pcm_flag + 3) = tmpRightChlWord;
        }
        else//right channel
        {
             tmpLeftChlWord = ((INT16)(*(output->Buffer + output->length + zero_pcm_flag -1)) +
                                                    (INT16)(*(output->Buffer + output->length + zero_pcm_flag + 1)))/2;
            tmpRightChlWord = ((INT16)(*(output->Buffer + output->length + zero_pcm_flag)) +
                                        (INT16)(*(output->Buffer + output->length + zero_pcm_flag + 2)))/2;
            memmove(output->Buffer + output->length + zero_pcm_flag + 3 , output->Buffer + output->length + zero_pcm_flag + 1, (pcm_length - zero_pcm_flag - 1)*2);
            *(output->Buffer + output->length + zero_pcm_flag + 1) = tmpLeftChlWord;
            *(output->Buffer + output->length + zero_pcm_flag + 2) = tmpRightChlWord;
        }
#if 0
        hal_HstSendEvent(SYS_EVENT,tmpLeftChlWord);
        hal_HstSendEvent(SYS_EVENT,tmpRightChlWord);
#endif
        return TRUE;
    }
    
    return FALSE;
}

void MMC_USBReceiveData(int32 length)
{
    USB_PLAY *pUSBPlayer = USBPlayer;
    USB_OUTPUT *output = &pUSBPlayer->USBOutput;
    USB_INPUT *input = &pUSBPlayer->USBInput;
    UINT32 status;
    BOOL plugIn = FALSE;

    if(!g_continue_play_flag)
        return;
   #if(CHIP_ASIC_ID!=CHIP_ASIC_ID_5856)
    SSHdl *my_ss_handle = MpegPlayer->MpegInput.ss_handle;
   #endif
    status = hal_EnterCriticalSection();

    if(add_byte != 0)
    {
        plugIn = MMC_USBPlugInVirtualWord(pUSBPlayer, length/2);
    }

    if(plugIn)
    {
        output->length += length/2 + 2;
        pUSBPlayer->packet_size = length + 4;
        add_byte -= 2;
    }
    else
    {
        output->length += length/2;
        pUSBPlayer->packet_size = length;
    }    
    
    hal_ExitCriticalSection(status);
    
    if(pUSBPlayer->in_flag == 1)
    {
      //  hal_HstSendEvent(SYS_EVENT,0x10280060);
      //  hal_HstSendEvent(SYS_EVENT,input->length);

        if(input->length+length/2 > MMC_USB_BUFF_SIZE)
        {
         //   hal_HstSendEvent(SYS_EVENT,0x84400004);
        }
        else
        {
            //rdabt_uart_tx_sco_data(input->data+input->length, input->sco_handle, length);
            input->length += length/2;
        }
    }
    else if(output->length >= MMC_USB_BUFF_SIZE*4)
    {
        hal_HstSendEvent(USB_EVENT,0x10270030);
        hal_HstSendEvent(USB_EVENT,output->length);
        pUSBPlayer->in_flag = 1;
		#if(CHIP_ASIC_ID!=CHIP_ASIC_ID_5856)
        if(!(is_SSHdl_valid(my_ss_handle)))
		#endif
        	MMC_USBPcmOutStart(pUSBPlayer);
    	uaudCodecOpen = 1;
    }
}

MCI_ERR_T  MMC_AudioUSBMicData(USB_PLAY *pUSBPlayer)
{
	USB_INPUT *input = &pUSBPlayer->USBInput;
    UINT32 sco_length;
    INT16 *PCM_q=(INT16 *)input->data;
    sco_length = pUSBPlayer->packet_size/2;

    while(input->length < MMC_USB_BUFF_SIZE/2)
    {
        //hal_HstSendEvent(SYS_EVENT,0x84400003);
        //rdabt_uart_tx_sco_data(input->data+input->length, input->sco_handle, sco_length);
        input->length += sco_length/2;
    }
    if(Mmc_USB_MIC_Half == 0) // half
    {
        memcpy(PCM_q+input->length-MMC_USB_BUFF_SIZE/2, PCM_q+input->length, MMC_USB_BUFF_SIZE*3-input->length*2);
    }
    else // end
    {
        memcpy(PCM_q+input->length-MMC_USB_BUFF_SIZE/2, PCM_q+input->length, (MMC_USB_BUFF_SIZE-input->length)*2);
        memcpy(PCM_q+MMC_USB_BUFF_SIZE/2, PCM_q+MMC_USB_BUFF_SIZE*3/2, MMC_USB_BUFF_SIZE);
    }
    input->length -= MMC_USB_BUFF_SIZE/2;
 //   hal_HstSendEvent(SYS_EVENT,0x10270040);
 //   hal_HstSendEvent(SYS_EVENT,input->length);
	return MCI_ERR_NO;    
}
/*
* NAME:	MMC_AudioUSBDecPcm()
* DESCRIPTION: Initialize USB decoder.
*/
#define PLAY_OR_NOT_TICK 160
extern volatile UINT32 last_music_tick;
volatile UINT32 now_tick;
//extern BOOL  g_usb_connected;
MCI_ERR_T MMC_AudioUSBDecPcm(USB_PLAY *pUSBPlayer)
{
	USB_OUTPUT *output = &pUSBPlayer->USBOutput;
    UINT32 status;
	 #if(CHIP_ASIC_ID!=CHIP_ASIC_ID_5856)
    //SSHdl *my_ss_handle = MpegPlayer->MpegInput.ss_handle;
    #endif
    if(audioStream.sampleRate == HAL_AIF_FREQ_44100HZ)
    {
        if(output->length < MMC_USB_BUFF_SIZE*4)
        {
            add_byte += 2;
        }
    }
    
    status = hal_EnterCriticalSection();
    if(output->length < MMC_USB_BUFF_SIZE*3)
    {
        now_tick = hal_TimGetUpTime();
        if(now_tick - last_music_tick > PLAY_OR_NOT_TICK)
        {
            if(Mmc_USB_Pcm_Half==0) // half
    	        memset(output->Buffer, 0, MMC_USB_BUFF_SIZE*2);
    	    else
    	        memset(output->Buffer+MMC_USB_BUFF_SIZE, 0, MMC_USB_BUFF_SIZE*2);
        }
    	
      //  hal_HstSendEvent(SYS_EVENT,0x84400001);
      //    hal_HstSendEvent(SYS_EVENT,now_tick - last_music_tick);
     //   hal_HstSendEvent(SYS_EVENT,output->length);
        return MCI_ERR_NO;
    }
  
	if(Mmc_USB_Pcm_Half==0) // half
	{
        memcpy(output->Buffer, output->Buffer+MMC_USB_BUFF_SIZE*2, MMC_USB_BUFF_SIZE*2);
        memcpy(output->Buffer+MMC_USB_BUFF_SIZE*2, output->Buffer+MMC_USB_BUFF_SIZE*3, output->length*2-MMC_USB_BUFF_SIZE*3);
	}
	else if(Mmc_USB_Pcm_Half==1) // end
	{
        memcpy(output->Buffer+MMC_USB_BUFF_SIZE, output->Buffer+MMC_USB_BUFF_SIZE*2, output->length*2-MMC_USB_BUFF_SIZE*2);
    }
    output->length -= MMC_USB_BUFF_SIZE;
    hal_ExitCriticalSection(status);

  //  hal_HstSendEvent(SYS_EVENT,0x10270035);
  //  hal_HstSendEvent(SYS_EVENT,output->length);

    return MCI_ERR_NO;
}

MCI_ERR_T MMC_AudioUSBDecVoC(USB_PLAY *pUSBPlayer)
{

	USB_INPUT *input = &pUSBPlayer->USBInput;
	USB_OUTPUT *output = &pUSBPlayer->USBOutput;
	
	vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus = &pUSBPlayer->Voc_AudioDecStatus;
	vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pUSBPlayer->Voc_AudioDecIN;


        SSHdl *my_ss_handle = MpegPlayer->MpegInput.ss_handle;
        bt_a2dp_audio_cap_struct * my_audio_config_handle=   MpegPlayer->MpegInput.audio_config_handle ;
        bt_a2dp_sbc_codec_cap_struct *sbc_config_handle =  &my_audio_config_handle->codec_cap.sbc;

         if(!(is_SSHdl_valid(my_ss_handle)))  
 	
               return MCI_ERR_NO;

			
	if(input->is_first_frame==0)
	{	
	
             vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);

	              //BT_A2DP_sbc
 	    my_ss_handle->PutData(my_ss_handle, (UINT8*) hal_VocGetPointer(SBC_Out_put_addr),pVoc_AudioDecStatus->ImgWidth);

	}

    	input->is_first_frame=0;

#if 0
	{
		int bt_music_gain,i;
		short *pcm_p=(short *)output->Buffer_head;

		switch(audio_cfg.spkLevel)
		{	
			case SND_SPK_MUTE:
				bt_music_gain=0;//mute
			break;
			case SND_SPK_VOL_1:
				bt_music_gain=0x80;//-18db
			break;
			case SND_SPK_VOL_2:
				bt_music_gain=0xb5;//-15db
			break;
			case SND_SPK_VOL_3:
				bt_music_gain=0x100;//-12db
			break;
			case SND_SPK_VOL_4:
				bt_music_gain=0x16a;//-9db
			break;
			case SND_SPK_VOL_5:
				bt_music_gain=0x200;//-6db
			break;
			case SND_SPK_VOL_6:
				bt_music_gain=0x2d4;//-3db
			break;
			case SND_SPK_VOL_7:
				bt_music_gain=0x400;//0db
			break;
			default:
				bt_music_gain=0x400;//0db	
			break;
			
		}

		for( i=0;i<(g_WavOutputPcmBufSize/4);i++)
		{
			pcm_p[i]=(pcm_p[i]*bt_music_gain)>>10;						
		}

	}
#endif
 	
   	memcpy( (UINT8*) hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR),  output->Buffer, output->length);


	pVoc_Voc_AudioDecIN->mode=MMC_SBC_ENC;
	pVoc_Voc_AudioDecIN->audioItf=audioItf;
       pVoc_Voc_AudioDecIN->BsbcEnable = 1;

 	  {
	 	
	 	int mode =SBC_MODE_JOINT_STEREO;
                  
			
	      if( sbc_config_handle->channel_mode == 0x01 )
	         mode = SBC_MODE_JOINT_STEREO; // joint stereo
	      else if( sbc_config_handle->channel_mode == 0x02 )
	        mode = SBC_MODE_STEREO; // stereo
	      else if( sbc_config_handle->channel_mode == 0x04 )
	        mode = SBC_MODE_DUAL_CHANNEL; // dual
	      else if( sbc_config_handle->channel_mode == 0x08 )
	        mode= SBC_MODE_MONO; // mono
	      else
	        MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC channel mode!\n"); 

		   pVoc_Voc_AudioDecIN->Source_width = mode;

	      if( sbc_config_handle->alloc_method  == 0x01 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_LOUDNESS;// 
	      else if( sbc_config_handle->alloc_method  == 0x02 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_SNR;//SBC_AM_LOUDNESS;//
	      else
	         MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC alloc_method!\n"); 
		  
		 
		   pVoc_Voc_AudioDecIN->Cut_width =    SBC_BIT_POOL;

		   pVoc_Voc_AudioDecIN->Zoomed_width =    HAL_AIF_STEREO;//output->Channel;         //used for Number of channels
		   pVoc_Voc_AudioDecIN->Zoomed_height =    HAL_AIF_FREQ_48000HZ;//output->SampleRate;  //used for SampleRate
		   
		   pVoc_Voc_AudioDecIN->inStreamUBufAddr = (INT32*)G_VppSBCConstX;
		   pVoc_Voc_AudioDecIN->inStreamVBufAddr = G_USBSBCConstY;
	 	}

         vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN);


         return MCI_ERR_NO;



}

/*
* NAME:	MMC_AudioUSBDecClose()
* DESCRIPTION: Close USB decoder.
*/
MCI_ERR_T MMC_AudioUSBDecClose (USB_PLAY *pUSBPlayer)
{
    uaudCodecOpen = 0;
        g_continue_play_flag = 0;
	aud_StreamStop(audioItf);
	mmc_MemFreeAll();
	
//tianwq		hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
    COS_Free(USBPlayer);
    USBPlayer = NULL;
	
	return  MCI_ERR_NO;
	
}


int32 Audio_USBPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
        MMC_AudioUSBDecOpen(USBPlayer, fhd);
        
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_USB]Audio_USBPlay");
	return  MCI_ERR_NO;
}

int32 Audio_USBStop (void) 	
{
	MMC_AudioUSBDecClose(USBPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_USBPause (void)	
{
        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_USB] Audio_USBPause!!!!");
	//MMC_AudioUSBDecClose(USBPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_USBResume ( HANDLE fhd) 
{
	MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_USB]Audio_USBResume!");
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_USB] Audio_USBResume!!!!");
	return  MCI_ERR_NO;
}


int32 Audio_USBGetID3 (char * pFileName)  
{
	return 0;	
}

int32 Audio_USBGetPlayInformation (MCI_PlayInf * MCI_PlayInfUSB)  
{
	MCI_PlayInfUSB->PlayProgress=0;
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_USB]PlayProgress:%d",MCI_PlayInfUSB->PlayProgress);
	
	return MCI_ERR_NO;
}

int32 Audio_USBUserMsg(COS_EVENT *ev)
{
	int32 nMsg = ev->nEventId;
	switch (nMsg)
	{
		case MSG_MMC_USB_PCM_INT:
			if(MMC_AudioUSBDecPcm(USBPlayer)==MCI_ERR_END_OF_FILE)
			{
				MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_USB]END OF FILE!");				
				g_continue_play_flag = 2;
			}
			break;
			
			
		case MSG_MMC_USB_PCM_END:
			
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_USB]MSG_MMC_USB_PCM_END\n");
			MMC_AudioUSBDecClose(USBPlayer);
			MCI_AudioFinished(MCI_ERR_END_OF_FILE);
			mmc_SetCurrMode(MMC_MODE_IDLE);
			
			break;

             case MSG_MMC_USB_MIC_INT:
                MMC_AudioUSBMicData(USBPlayer);
                break;

    		case MSG_MMC_USB_SBC_VOC:
			     MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_USB]MSG_MMC_USB_SBC_VOC! g_sbc_frame =%d",g_sbc_frame);	
                if(g_sbc_frame<1)
                {
                    MMC_AudioUSBDecVoC(USBPlayer);
                    g_sbc_frame++;
                }
						
			
			break;


		default:
			
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_USB_ERR]ERROR MESSAGE!");
			
			break;
	}
    return 0;
}


