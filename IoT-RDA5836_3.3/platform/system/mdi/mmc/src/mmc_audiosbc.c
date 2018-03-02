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
// FILENAME: mmc_audioSBC.c
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
#include "sbc.h"

#include "mmc_audiosbc.h"

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

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "vpp_sbc_dec.h"
#include "event.h"
#include "bt_types.h"
#include "bt_msg.h"
#include "hal_rda_abb.h"
#include "hal_rda_audio.h"
#include "hal_bt.h"
#include "ap_spp.h"
#include "ap_a2dp.h"

#include "hal_host.h"

#ifdef KALAOK_SUPPORT
#include "Effectecho.h"
#endif

static INT8 Mmc_SBC_Pcm_Half = 0;

static INT32 g_SBCHeaderLength=0;

SBC_PLAY *SBCPlayer;

extern AUD_LEVEL_T audio_cfg;
extern HAL_AIF_STREAM_T audioStream;
extern AUD_ITF_T audioItf;

volatile INT32 g_SBCConsumedLength=0;

static INT32 g_SBCFILESIZE=-1;
static UINT8 ContinuePlayFlag=0;//this flag controls to continue play the left data in the pcm buffer
static UINT8 ErrorFrameCount = 0;
static UINT8 g_VocBusy=0;
static UINT8 g_SampleRateChangeFlag=0;

//static sbc_t g_sbc_dec;

#define SBC_INPUT_BUFSIZE		              (7*1024)
#define SBC_MAX_OUTPUT_BUFSIZE		(9*1024)

#define SBC_MIN_INPUT_SIZE		              (3*1024)
#define SBC_PLAY_MIN_INPUT_SIZE		              (1024)

#define SBCDEC_PROTECT_PCMOUT
//#define SBC_DEBUG_TICKS
//#define SBC_SUPPORT_MUTE_DIS_PA
//#define SBC_DUMP_DATA_DEBUG

#ifdef MUSIC_BAND_INFO
static uint16 SbcBandInfo[8]={0};
#endif

#ifdef KALAOK_SUPPORT
///dsp 

HAL_AIF_STREAM_T audioInStream;
effectecho_param echo_param = {200, 16384};// 1:delay times ms; 2:16384 -> 0.5;
UINT16 * echo_delay_buff = NULL;

#endif

#ifdef BT_SUPPORT

#define LOW_SPEED    0
#define NORMAL_SPEED 1
#define HIGH_SPEED   2

INT16 SpeedMode = HIGH_SPEED;

extern UINT8 MCI_Play_BTStream;
extern bt_a2dp_audio_cap_struct *MCI_BTAudioCap;
SBC_BT_Packet SbcBtPacket;
#endif

#ifdef SBC_DEBUG_TICKS
u_int32	g_SbcPacketTicks = 0;
u_int32	g_SbcVocTicks = 0;
u_int32	g_SbcPcmTicks = 0;
#endif

#ifdef SBC_DUMP_DATA_DEBUG
INT32  gSbcDebugCurFd = -1;
#define SBC_DUMP_NAME L"dump_sbc.txt"
#endif

#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_5856) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)  || (CHIP_ASIC_ID == CHIP_ASIC_ID_8809))
UINT8 g_MuteFlag = 0;
UINT32 g_MuteBeginTick = 0;
#endif

extern PUBLIC UINT32 gpio_detect_earpiece(VOID);
extern PUBLIC VOID hal_AudCalibSampleRate(HAL_AUD_USER_T user, HAL_AIF_FREQ_T sampleRate1 ,HAL_AIF_FREQ_T sampleRate2);
extern PUBLIC void hal_SysSetVocClockDefault(void);

#if APP_SUPPORT_SOUNDBAR==1

/************************************************************/
UINT32 soundbar_sbc_counter = 0;
#define SOUNDBAR_SYNC_CHECK_TIME  2500  //counter
#define SOUNDBAR_CHECK_DELAY_TIME 5  //ms


typedef enum
{
    BT_CLOCK,
    NATIVE_CLOCK
};

UINT8 MMC_Soundbar_Get_Soundbar_Clk_Role(void)
{
    if( Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)//NATIVE_CLOCK
    {
        return NATIVE_CLOCK; 
    }
    else if(Soundbar_App_Get_Role_Status() == SOUNDBAR_SLAVE)//BT_CLOCK
    {
        return BT_CLOCK;
    }
}

VOID MMC_Soundbar_Get_Current_Clock(UINT32* clock)
{
    UINT8 i=0;
    //u_int32 native_bt_clk =0x7000001c;
    //u_int32 bt_clk =       0x70000018;
    if(MMC_Soundbar_Get_Soundbar_Clk_Role() == NATIVE_CLOCK)
    {
        for(;i<2;i++)
        {
            *clock = hal_BtReadNativeClock();
            if(*clock != 0)
                break;
        }
    }else if(MMC_Soundbar_Get_Soundbar_Clk_Role() == BT_CLOCK)
    {
        for(;i<2;i++)
        {
            *clock = hal_BtReadBTClock();
            if(*clock != 0)
                break;
        }
    }
}

SOUNBAR_AUDIO_SYNC_STRUCT mmc_audio_sync_data = {0};
void MMC_Soundbar_Check_Counter_And_Send_Clk(void)
{
    static UINT32 i = 0x01;
    UINT32 mmc_current_clk = 0;
    soundbar_sbc_counter++;
    if(soundbar_sbc_counter > SOUNDBAR_SYNC_CHECK_TIME * i)
    {
        hal_HstSendEvent(1, 0x20170306);
        hal_HstSendEvent(1, soundbar_sbc_counter);
        MMC_Soundbar_Get_Current_Clock(&mmc_current_clk);
        hal_HstSendEvent(1, mmc_current_clk);
        if(mmc_current_clk == 0)
            return;
        mmc_audio_sync_data.current_clk = mmc_current_clk;
        mmc_audio_sync_data.counterNum  = SOUNDBAR_SYNC_CHECK_TIME * i;
        if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
            app_send_audio_sync_data(&mmc_audio_sync_data);
        i++;
    }
}

UINT32 MMC_Check_SyncClock_DelayMilliseconds(u_int32 clk1,u_int32 clk2)
{
    return (((clk1 - clk2)*625)/2000);
}

void MMC_Soundbar_Control_Audio_Sync(SOUNBAR_AUDIO_SYNC_STRUCT *temp_sync_data)
{
    UINT32 delayMilliseconds = 0;
    if(temp_sync_data->counterNum == mmc_audio_sync_data.counterNum)
    {
        if(temp_sync_data->current_clk > mmc_audio_sync_data.current_clk)
        {
            delayMilliseconds = MMC_Check_SyncClock_DelayMilliseconds(temp_sync_data->current_clk , mmc_audio_sync_data.current_clk);
            hal_HstSendEvent(SYS_EVENT, 0x20180000);
            hal_HstSendEvent(SYS_EVENT, delayMilliseconds);
            if(delayMilliseconds > SOUNDBAR_CHECK_DELAY_TIME)
            {
                Audio_Change_SampleRate(LOW_SPEED);
                hal_HstSendEvent(SYS_EVENT, 0x20170303);
            }

        }
        else if(temp_sync_data->current_clk < mmc_audio_sync_data.current_clk)
        {
            delayMilliseconds = MMC_Check_SyncClock_DelayMilliseconds(mmc_audio_sync_data.current_clk , temp_sync_data->current_clk);
            hal_HstSendEvent(SYS_EVENT, 0x20180001);
            hal_HstSendEvent(SYS_EVENT, delayMilliseconds);
            if(delayMilliseconds > SOUNDBAR_CHECK_DELAY_TIME)
            {
                Audio_Change_SampleRate(HIGH_SPEED);
                hal_HstSendEvent(SYS_EVENT, 0x20170304);
            }
        }
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeeeaaaa);
    }

}

void MMC_AudioPlayA2dpData(SBC_PLAY *pSBCPlayer)
{
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pSBCPlayer->Voc_AudioDecIN;
	
    if(input->inFlag!=2)
    {
        //if(input->length < SBC_MIN_INPUT_SIZE)
            //return MCI_ERR_NO;
        hal_HstSendEvent(SYS_EVENT, input->length);
		
        if(g_VocBusy==0)
        {
            hal_HstSendEvent(SYS_EVENT, 0x88400044);
            vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
            g_VocBusy = 1;

            // prepare for nect frame decode
            Audio_SBCPrepareNextFrame(pSBCPlayer);
        }
        else
        {
			hal_HstSendEvent(SYS_EVENT, 0x88400003);
        }
    }
}

void MMC_SyncAudioPlay(void)
{
    u_int32 sbc_tick;
    MMC_AudioPlayA2dpData(SBCPlayer);
	sbc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0xee000000 + sbc_tick);
}

/*************************************************************/
#endif
MCI_ERR_T MMC_AudioSBCDecClose (SBC_PLAY *pSBCPlayer);

/*
* NAME:	MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioSBCVocISR(void)
{
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_SBC_VOC);
} 


/*
* NAME:	MMC_AudioSBCHalfPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/

void MMC_AudioSBCHalfPcmISR(void)
{
    //MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC]HALF!");
    Mmc_SBC_Pcm_Half = 0;
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_SBC_PCM);
}

/*
* NAME:	MMC_AudioSBCEndPcmISR()
* DESCRIPTION: ISR for pcm end buffer exhausted.
*/
void MMC_AudioSBCEndPcmISR(void)
{
    //MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC]END!");
    Mmc_SBC_Pcm_Half = 1;
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_SBC_PCM);
#if APP_SUPPORT_SOUNDBAR == 1
    if(Soundbar_App_Get_Role_Status() < SOUNDBAR_NOROLE)
        MMC_Soundbar_Check_Counter_And_Send_Clk();
#endif
}


MCI_ERR_T MMC_SBCHeaderParse(SBC_PLAY *pSBCPlayer)
{
    //SBC_INPUT *input = &pSBCPlayer->SBCInput;
    //SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;

    return MCI_ERR_NO;

}

extern VOID hal_abbSetPaOpenMode(BOOL on);

void MMC_SBCPcmOutStart(SBC_PLAY *pSBCPlayer)
{
    hal_HstSendEvent(SYS_EVENT,0x10250020);
	
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC_DEC]MMC_SBCPcmOutStart BEGIN!");
#ifdef KALAOK_SUPPORT
    if(pSBCPlayer->SBCOutput.pcm.Channel==2)
    {
        hal_HstSendEvent(SYS_EVENT,0x5bc01a00);

        //output->pcm.Size alread have been changed in voc;
        pSBCPlayer->SBCOutput.pcm.Channel = pSBCPlayer->SBCOutput.pcm.Channel >> 1;

        hal_HstSendEvent(SYS_EVENT,pSBCPlayer->SBCOutput.pcm.Size);
        hal_HstSendEvent(SYS_EVENT,pSBCPlayer->SBCOutput.pcm.Channel);
    }
    // Stream out
    audioStream.startAddress  	= (UINT32 *)pSBCPlayer->SBCOutput.pcm.Buffer;
    audioStream.length        	= pSBCPlayer->SBCOutput.pcm.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
    audioStream.channelNb     	= pSBCPlayer->SBCOutput.pcm.Channel;//HAL_AIF_STEREO;//HAL_AIF_MONO;

    audioStream.voiceQuality   = !TRUE;    
    audioStream.halfHandler   	= NULL;
    audioStream.endHandler    = NULL;
#else
    // Stream out
    audioStream.startAddress  	= (UINT32 *)pSBCPlayer->SBCOutput.pcm.Buffer;
    audioStream.length        	= pSBCPlayer->SBCOutput.pcm.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
    audioStream.channelNb     	= pSBCPlayer->SBCOutput.pcm.Channel;//HAL_AIF_STEREO;//HAL_AIF_MONO;

    audioStream.voiceQuality   = !TRUE;    
    audioStream.halfHandler   	= MMC_AudioSBCHalfPcmISR;
    audioStream.endHandler    = MMC_AudioSBCEndPcmISR;
#endif

    // Initial cfg
    if(gpio_detect_earpiece())
        audioItf = AUD_ITF_BT_EP;
    else
        audioItf = AUD_ITF_BT_SP;

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC_DEC]SAMPLERATE:%d",pSBCPlayer->SBCOutput.pcm.SampleRate);

    switch (pSBCPlayer->SBCOutput.pcm.SampleRate)
    {
        case SBC_SAMPLING_FREQ_48000:
            audioStream.sampleRate    = HAL_AIF_FREQ_48000HZ;
        break;

        case SBC_SAMPLING_FREQ_44100:
            audioStream.sampleRate    = HAL_AIF_FREQ_44100HZ;
        break;

        case SBC_SAMPLING_FREQ_32000:
            audioStream.sampleRate    = HAL_AIF_FREQ_32000HZ;
        break;

        case SBC_SAMPLING_FREQ_16000:
            audioStream.sampleRate    = HAL_AIF_FREQ_16000HZ;
        break;

        default:
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SBC_DEC_ERROR]ERROR SAMPLERATE:%d",pSBCPlayer->SBCOutput.pcm.SampleRate);
        break;
    }
    
    hal_HstSendEvent(SYS_EVENT,0x10250021);
    hal_HstSendEvent(SYS_EVENT,pSBCPlayer->SBCOutput.pcm.SampleRate);
    hal_HstSendEvent(SYS_EVENT,audioStream.sampleRate);
    hal_HstSendEvent(SYS_EVENT,audioStream.channelNb);
    //COS_EnterCriticalSection();
	//while(1);
    hal_abbSetPaOpenMode(FALSE);
#ifdef KALAOK_SUPPORT
    audioInStream.startAddress = audioStream.startAddress+pSBCPlayer->SBCOutput.pcm.Size;
    audioInStream.length       = audioStream.length;
    audioInStream.sampleRate   = audioStream.sampleRate;
    audioInStream.channelNb    = audioStream.channelNb;
    audioInStream.voiceQuality = !TRUE;
    audioInStream.playSyncWithRecord = FALSE;
    audioInStream.halfHandler  = MMC_AudioSBCHalfPcmISR;
    audioInStream.endHandler   = MMC_AudioSBCEndPcmISR;

    aud_StreamStart(audioItf, &audioStream, &audio_cfg);

    aud_StreamRecord(audioItf, &audioInStream, &audio_cfg);
#else
    aud_StreamStart(audioItf, &audioStream, &audio_cfg);
#endif
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC_DEC] MMC_SBCPcmOutStart!");

}


PUBLIC MCI_ERR_T MMC_SBCGetFileInformation (
                        CONST INT32 FileHander,
                         AudDesInfoStruct  * CONST DecInfo  )
{

    SBC_PLAY SBCPlay;
    SBC_INPUT *input = &SBCPlay.SBCInput;
    int32 result;

    //judge the file handler
    input->fileHandle = FileHander;
    if (input->fileHandle<0)
    {
        diag_printf("[MMC_SBC_DEC]MMC_SBCGetFileInformation FILE HANDLER ERROR!: %d \n", FileHander);
        return MCI_ERR_CANNOT_OPEN_FILE;
    }
    else
    {
        diag_printf("[MMC_SBC_DEC] MMC_SBCGetFileInformation FILE HANDLER: %d\n", FileHander);
    }

    g_SBCHeaderLength=0;

    //parse SBC file header
    if((result= MMC_SBCHeaderParse(&SBCPlay))!=MCI_ERR_NO)
    {
        return result;
    }

    // get the infromation.
    DecInfo->sampleRate             = SBCPlay.SBCOutput.pcm.SampleRate;
    DecInfo->stereo                     = SBCPlay.SBCOutput.pcm.Channel;

    diag_printf("[MMC_SBC_DEC]MMC_SBCGetFileInformation DecInfo->sampleRate : %d , DecInfo->stereo : %d\n", DecInfo->sampleRate,DecInfo->stereo );

    return MCI_ERR_NO;

}




/*
* NAME:	MMC_AudioSBCDecOpen()
* DESCRIPTION: Initialize SBC decoder.
*/
MCI_ERR_T MMC_AudioSBCDecOpen (HANDLE fhd, SBC_PLAY *pSBCPlayer)
{
    if(pSBCPlayer == NULL)
        pSBCPlayer = SBCPlayer = COS_Malloc(sizeof(SBC_PLAY));
    
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pSBCPlayer->Voc_AudioDecIN;

    HAL_ERR_T voc_ret = 0;
	
    int32 readlen = 0;
    int32 result;
    
#ifdef BT_SUPPORT
    SpeedMode = NORMAL_SPEED;
#endif

#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_5856) || (CHIP_ASIC_ID == CHIP_ASIC_ID_8809))
      g_MuteFlag = 0;
      g_MuteBeginTick = 0;
#endif
	
    hal_HstSendEvent(SYS_EVENT,0x10250002);
    //#ifdef TARGET_A2DPDISK_SUPPORT
    hal_SysSetVocClock(HAL_SYS_VOC_FREQ_104M);
    //#endif
    ContinuePlayFlag=0;
    ErrorFrameCount = 0;

    output->buff_offset = 0;
    output->pcm.Buffer= pcmbuf_overlay;
    memset(output->pcm.Buffer, 0, SBC_MAX_OUTPUT_BUFSIZE);
    output->pcm.Buffer_head=output->pcm.Buffer;
    output->pcm.Channel=2;
    g_SampleRateChangeFlag = 0;
    input->data = (uint8 *)inputbuf_overlay;
    memset(input->data, 0, SBC_INPUT_BUFSIZE);
    input->length = SBC_INPUT_BUFSIZE;
    input->inFlag=0;

    //hal_HstSendEvent(SYS_EVENT,0x10250011);
    //hal_HstSendEvent(SYS_EVENT,output->pcm.Buffer);
    //hal_HstSendEvent(SYS_EVENT,input->data);
    //sbc_init(&g_sbc_dec, 0);
    //g_sbc_dec.endian = SBC_LE;
    //hal_HstSendEvent(SYS_EVENT,0x10250012);
	
#ifdef BT_SUPPORT
    //hal_HstSendEvent(SYS_EVENT,MCI_Play_BTStream);
    //hal_HstSendEvent(SYS_EVENT,MCI_BTAudioCap);
    if(MCI_Play_BTStream == 1)
    {
        output->pcm.Bits = 16;
        output->pcm.SampleRate = MCI_BTAudioCap->codec_cap.sbc.sample_rate;
        output->pcm.Channel = MCI_BTAudioCap->codec_cap.sbc.channel_mode==SBC_CHANNEL_MODE_MONO?1:2;
        output->pcm.Size = (SBC_MAX_OUTPUT_BUFSIZE) >> 2;

        //Avdtp_Clean_Data(0);

        input->length = 0;
        //SbcBtPacket.BtPacketQueueNum = 0;
        SbcBtPacket.start_pos = 0;
        SbcBtPacket.end_pos = 0;
    }
    else
#endif   
    if(0)
    {// disable sbc file play
        //judge the file handler
        input->fileHandle = fhd;
        if (input->fileHandle<0)
        {
            diag_printf("[MMC_SBC_DEC]FILE HANDLER ERROR!: %d \n", fhd);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }
        else
        {
            diag_printf("[MMC_SBC_DEC] FILE HANDLER: %d\n", fhd);
        }

        g_SBCHeaderLength=0;

        //parse SBC file header
        if((result= MMC_SBCHeaderParse(pSBCPlayer))!=MCI_ERR_NO)
        {
            return result;
        }

        g_SBCConsumedLength=g_SBCConsumedLength-g_SBCHeaderLength;

        if(g_SBCConsumedLength<0)
            g_SBCConsumedLength=0;

        g_SBCConsumedLength=g_SBCConsumedLength&0xfffffffC;

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC_DEC]g_SBCConsumedLength :%d",g_SBCConsumedLength);

        result=mmc_aud_FileSeek(input->fileHandle,g_SBCConsumedLength,FS_SEEK_CUR);

        /*prepare input stream*/
        readlen = mmc_aud_FileRead(input->fileHandle, input->data, input->length);

        hal_HstSendEvent(SYS_EVENT,(UINT32)0x12130080);
        hal_HstSendEvent(SYS_EVENT,(UINT32)g_SBCConsumedLength);
        hal_HstSendEvent(SYS_EVENT,(UINT32)input->data);
        hal_HstSendEvent(SYS_EVENT,(UINT32)input->length);
        hal_HstSendEvent(SYS_EVENT,(UINT32)readlen);
        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC_DEC] First Read Len=%d ;Need Length=%d\n", readlen,input->length);

        if(readlen<0)
        {
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        //input->fileOffset=g_SBCConsumedLength+g_SBCHeaderLength;

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC_DEC]FILE SEEK POSITION:%d",result);

        if(result<0 )
        {
            MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC_DEC_ERR]FS_seek ERROR!");
            return MCI_ERR_BAD_FORMAT;
        }

        //MMC_SBCPcmOutStart(pSBCPlayer);
    }
	   
	Mmc_SBC_Pcm_Half = 0;   
    voc_ret = vpp_SBCDecOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioSBCVocISR);

    hal_HstSendEvent(SYS_EVENT,0x88100000+voc_ret);
	 
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
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC_DEC_ERROR]Fail to Call vpp_AudioJpegDecOpen()t: %d \n", voc_ret);
            return MCI_ERR_ERROR;
            
        default:
            break;
    }
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_SBC_DEC]MMC_AudioDecOpen end!");

        // setup first frame config
        
        
    pVoc_Voc_AudioDecIN->inStreamBufAddr=(INT32 *)input->data;
    pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer;
    pVoc_Voc_AudioDecIN->mode=MMC_SBC_DEC;
    pVoc_Voc_AudioDecIN->audioItf=audioItf;
    pVoc_Voc_AudioDecIN->reset = 1;


#ifdef SBC_DUMP_DATA_DEBUG
    gSbcDebugCurFd = FS_Open(SBC_DUMP_NAME, FS_O_RDWR|FS_O_CREAT|FS_O_TRUNC, 0);
    hal_HstSendEvent(SYS_EVENT,gSbcDebugCurFd);
    if(gSbcDebugCurFd < 0)
    {
        hal_HstSendEvent(SYS_EVENT,0x88100004);
        hal_HstSendEvent(SYS_EVENT,gSbcDebugCurFd);
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT,0x88100003);
        hal_HstSendEvent(SYS_EVENT,gSbcDebugCurFd);
    }
#endif	

#ifdef KALAOK_SUPPORT
    
    Effectecho_init(ECHO_DELAY_MAX, vpp_SBCGetVoCNoUsedAddr());
#endif
	return MCI_ERR_NO;
	
}

void Audio_SBCPrepareNextFrame(SBC_PLAY *pAudioPlayer)
{
    SBC_OUTPUT *output = &pAudioPlayer->SBCOutput;
    vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;

    // prepare for nect frame decode
    if(Mmc_SBC_Pcm_Half==1) // end
    {
        output->pcm.Buffer_head=output->pcm.Buffer;
    }
    else if(Mmc_SBC_Pcm_Half==0) // half
    {
        output->pcm.Buffer_head=output->pcm.Buffer+(output->pcm.Size>>1);           
    }
    pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer_head;
#ifdef SBCDEC_PROTECT_PCMOUT
    if((UINT32)output->pcm.Buffer_head < (UINT32)pcmbuf_overlay || (UINT32)output->pcm.Buffer_head > ((UINT32)pcmbuf_overlay + SBC_MAX_OUTPUT_BUFSIZE))
    {
        hal_HstSendEvent(SYS_EVENT,0x12010062);
        hal_DbgAssert("SBCDecPcmOutAddr is error!");  
    }
#endif
}



#ifdef BT_SUPPORT

int8 Audio_Get_Sbc_Buf_Num(void)
{
    if(SbcBtPacket.end_pos >= SbcBtPacket.start_pos)
        return (SbcBtPacket.end_pos - SbcBtPacket.start_pos);
    else
        return (SbcBtPacket.end_pos + 8 - SbcBtPacket.start_pos);
}

void Audio_Change_SampleRate(u_int8 speedflag)
{
    HAL_AIF_FREQ_T change_sampleRate = 0;

    switch(speedflag)
    {
        case HIGH_SPEED   : 
        if(audioStream.sampleRate  == HAL_AIF_FREQ_48000HZ)
        {
            change_sampleRate = HAL_AIF_FREQ_48003HZ;
        }
        else
        {
#if APP_SUPPORT_SOUNDBAR == 1
            if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
            {
                change_sampleRate = HAL_AIF_FREQ_44102HZ;
            }
            else
            {
                change_sampleRate = HAL_AIF_FREQ_44103HZ;
            }
#else
            change_sampleRate = HAL_AIF_FREQ_44102HZ;
#endif		
        }
        break;
        
        case LOW_SPEED    : 
        if(audioStream.sampleRate  == HAL_AIF_FREQ_48000HZ)
        {
            change_sampleRate = HAL_AIF_FREQ_47999HZ;
        }
        else
        {
#if APP_SUPPORT_SOUNDBAR == 1
            if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
            {
                change_sampleRate = HAL_AIF_FREQ_44099HZ;
            }
            else
            {
                change_sampleRate = HAL_AIF_FREQ_44098HZ;
            }
#else
            change_sampleRate = HAL_AIF_FREQ_44099HZ;
#endif	
        }
        break;
        
        case NORMAL_SPEED : 
            change_sampleRate = audioStream.sampleRate;
        break;
        
        default :           
            change_sampleRate = audioStream.sampleRate;
        break;
    }
    
    hal_AudCalibSampleRate(HAL_AUD_USER_CODEC, audioStream.sampleRate, change_sampleRate);
}

void MMC_AudioAutoCalibSameple(SBC_PLAY *pSBCPlayer ,u_int8 high_flag)
{
//#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
  //  return;
//#endif
	SBC_INPUT *input = &pSBCPlayer->SBCInput;
	//INT16 sbcPacketNum = 0;
 
    //sbcPacketNum = Audio_Get_Sbc_Buf_Num();
	if(high_flag && SpeedMode != HIGH_SPEED )
	{
	    SpeedMode = HIGH_SPEED;
		Audio_Change_SampleRate(HIGH_SPEED);
		hal_HstSendEvent(SYS_EVENT, 0x88400006);
		hal_HstSendEvent(SYS_EVENT, input->length);
	}
	else if((input->length > SBC_INPUT_BUFSIZE/7*5 ) && (SpeedMode != NORMAL_SPEED && SpeedMode != HIGH_SPEED))
	{
	    SpeedMode = NORMAL_SPEED;
		Audio_Change_SampleRate(NORMAL_SPEED);
		hal_HstSendEvent(SYS_EVENT, 0x88400004);
		hal_HstSendEvent(SYS_EVENT, input->length);
		//hal_HstSendEvent(SYS_EVENT, sbcPacketNum);

	}
	else if((input->length < SBC_PLAY_MIN_INPUT_SIZE*2) && (SpeedMode != LOW_SPEED))
	{
		SpeedMode = LOW_SPEED;
		Audio_Change_SampleRate(LOW_SPEED);
		hal_HstSendEvent(SYS_EVENT, 0x88400005);
		hal_HstSendEvent(SYS_EVENT, input->length);
		//hal_HstSendEvent(SYS_EVENT, sbcPacketNum);
	}
}
MCI_ERR_T  MMC_AudioReceiveA2dpData(SBC_PLAY *pSBCPlayer, t_DataBuf *p_buf)
{
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    //SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pSBCPlayer->Voc_AudioDecIN;
    //int32 i,readlen;
    COS_EVENT event={0};
    //t_DataBuf *p_bufbak;
    //int packets = 0; 
    //hal_HstSendEvent(SYS_EVENT,0x11000000+csw_TMGetTick());

#ifdef SBC_DEBUG_TICKS
    u_int32 sbc_tick;
    sbc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x11100000 + sbc_tick - g_SbcPacketTicks);
    g_SbcPacketTicks = sbc_tick;
#endif

#if 0
    hal_HstSendEvent(SYS_EVENT,0x10250030);
    hal_HstSendEvent(SYS_EVENT,input->data);
    hal_HstSendEvent(SYS_EVENT,input->length);
    hal_HstSendEvent(SYS_EVENT,input->inFlag);
#endif     

    event.nEventId = RDABT_A2DP_DATA_COMPLETED_MSG;

    SbcBtPacket.BtPacketQueue[SbcBtPacket.end_pos++] = p_buf;
    if(SbcBtPacket.end_pos == 8)
        SbcBtPacket.end_pos = 0;

    p_buf = SbcBtPacket.BtPacketQueue[SbcBtPacket.start_pos];
    event.nParam1 = (UINT32)p_buf;
    if(p_buf->dataLen <= SBC_INPUT_BUFSIZE-input->length)
    {	
        UINT16 data_len = 0;

        data_len = p_buf->dataLen;
        memcpy(input->data+input->length, p_buf->buf, p_buf->dataLen);
        COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        input->length += data_len;
#ifdef SBC_DUMP_DATA_DEBUG
        if(gSbcDebugCurFd >= 0)
        {
            int iret;
            iret = FS_Write(gSbcDebugCurFd, input->data+input->length-data_len, data_len);
            hal_HstSendEvent(SYS_EVENT,0x88100005);
            hal_HstSendEvent(SYS_EVENT,data_len);
            hal_HstSendEvent(SYS_EVENT,iret);
        }
#endif
        SbcBtPacket.BtPacketQueue[SbcBtPacket.start_pos] = NULL;
        SbcBtPacket.start_pos++;
        if(SbcBtPacket.start_pos == 8)
            SbcBtPacket.start_pos = 0;
    }
    else
    {
        // hal_HstSendEvent(SYS_EVENT,0x20161114);
        // hal_HstSendEvent(SYS_EVENT, input->length);
        // hal_HstSendEvent(SYS_EVENT, p_buf->dataLen);

        MMC_AudioAutoCalibSameple(pSBCPlayer,1);
    }
    
#if APP_SUPPORT_SOUNDBAR == 1
    if(Soundbar_App_Get_Role_Status() == SOUNDBAR_NOROLE)
    {
#endif
        if(input->inFlag!=2)
        {
            if(input->length < SBC_MIN_INPUT_SIZE)
            {
                hal_HstSendEvent(SYS_EVENT, 0x88400022);
                return MCI_ERR_NO;
            }

            if(g_VocBusy==0)
            {
                hal_HstSendEvent(SYS_EVENT, 0x88400033);
                vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
                g_VocBusy = 1;
                Mmc_SBC_Pcm_Half = 0; // put next data to end buf;
                // prepare for nect frame decode
                Audio_SBCPrepareNextFrame(pSBCPlayer);
            }
            else
            {
                hal_HstSendEvent(SYS_EVENT, 0x88400003);
            }
        }
#if APP_SUPPORT_SOUNDBAR == 1
    }
#endif

    return MCI_ERR_NO;
}


#endif

#ifdef SINDEBUG
short sintone[] = {0x0000,0x0663,0x0CB6,0x12E8,0x18EB,0x1EAD,0x2424,0x293C,0x2DEC,	
				0x3228,	0x35E3,	0x3916,	0x3BB8,	0x3DC0,	0x3F2D,	0x3FF8,	0x4022,	0x3FA7,	0x3E8B,	
				0x3CCF,	0x3A7A,	0x378E,	0x3416,	0x301A,	0x2BA2,	0x26BC,	0x2173,	0x1BD6,	0x15F0,	
				0x0FD5,	0x0990,	0x0333,	0xFCCD,	0xF670,	0xF02C,	0xEA0F,	0xE42B,	0xDE8D,	0xD944,	
				0xD45F,	0xCFE7,	0xCBEB,	0xC871,	0xC587,	0xC331,	0xC175,	0xC059,	0xBFDF,	0xC007,	
				0xC0D3,	0xC23F,	0xC449,	0xC6EA,	0xCA1D,	0xCDD8,	0xD214,	0xD6C4,	0xDBDD,	0xE152,	
				0xE715,	0xED18,	0xF34A,	0xF99C,	0x0000	};
#endif

void MMC_AudioCleanA2dpData(void)
{
#ifdef BT_SUPPORT
    COS_EVENT event={0};
    event.nEventId = RDABT_A2DP_DATA_COMPLETED_MSG;

    u_int8 i = 0;

    for(;i < 8; i++)
    {
        if(SbcBtPacket.BtPacketQueue[i] != NULL)
        {
            event.nParam1 = (UINT32)SbcBtPacket.BtPacketQueue[i];
            SbcBtPacket.BtPacketQueue[i] = NULL;
            COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        }
    }
    SbcBtPacket.start_pos = 0;
    SbcBtPacket.end_pos = 0;
#endif
}
MMC_AUDIO_CHECK_SBC_CALLBACK_T g_mmcCheckSbcMuteCallback = NULL;
UINT32 sbc_mute_times = 0;
UINT32 sbc_check_mute_begain = 0;

void MMC_AudioSBCCheckIsMute(UINT32 times, MMC_AUDIO_CHECK_SBC_CALLBACK_T callbak)
{
    sbc_mute_times = times;
    g_mmcCheckSbcMuteCallback = callbak;
    sbc_check_mute_begain = 1;
}
void AudioSBCCheckIsMute(UINT32 times, MMC_AUDIO_CHECK_SBC_CALLBACK_T callbak)
{
#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_5856) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)  || (CHIP_ASIC_ID == CHIP_ASIC_ID_8809))
    if(sbc_check_mute_begain)
    {
        BOOL Mute = (vpp_SBCDecDetectMute() == 1);
        if(Mute)
        {
            if(g_MuteFlag == 0)
            {
                g_MuteFlag = 1;
                g_MuteBeginTick = hal_TimGetUpTime();
            }
        }
        else
        {
            if(g_MuteFlag == 2)
            {
#ifdef SBC_SUPPORT_MUTE_DIS_PA
                hal_HstSendEvent(SYS_EVENT, 0x103c0dec);
                hal_abbEnablePa(TRUE);
#endif
            }
            g_MuteFlag = 0;

            if(callbak != NULL)
            {
                (*callbak)(FALSE);
            }
            sbc_check_mute_begain = 0;
        }

        if(g_MuteFlag == 1)
        {
            extern VOID hal_AudResetCodec(VOID);
            if(Mute && (hal_TimGetUpTime() - g_MuteBeginTick >= times))
            {// Reset codec
#ifdef SBC_SUPPORT_MUTE_DIS_PA
                hal_HstSendEvent(SYS_EVENT, 0x102c0dec);
                hal_abbEnablePa(FALSE);
#endif
                if(callbak != NULL)
                {
                    (*callbak)(TRUE);
                }
                sbc_check_mute_begain = 0;
                //hal_AudResetCodec(); not need;
                g_MuteFlag = 2;
            }
        }
    }
#endif
}
MCI_ERR_T MMC_AudioSBCDecVoc(SBC_PLAY *pSBCPlayer)
{
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    vpp_SBC_DEC_OUT_T *pVoc_AudioDecStatus = &pSBCPlayer->Voc_AudioDecStatus;
	vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pSBCPlayer->Voc_AudioDecIN;
    //int32 i,readlen;
    //uint32 length;
    extern UINT16 *g_last_buffer_addr;


#ifdef SBC_DEBUG_TICKS
    u_int32 sbc_tick;
    sbc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x33300000 + sbc_tick - g_SbcVocTicks);
    hal_HstSendEvent(SYS_EVENT,0x44400000 + sbc_tick - g_SbcPcmTicks);

    g_SbcVocTicks = sbc_tick;
#endif
    //hal_HstSendEvent(SYS_EVENT,0x12010010);
    //hal_HstSendEvent(SYS_EVENT,MCI_Play_BTStream);
    g_VocBusy = 0;

#ifdef BT_SUPPORT
    if(MCI_Play_BTStream == 1)
    {
        //uint32 sbc_length, frame_len, frames, length;
        int32 length;

        if(input->inFlag==0)
        {
            input->inFlag = 1;
            vpp_SBCDecCheckOutPcmAddr((UINT32 *)output->pcm.Buffer,(UINT32 *)(output->pcm.Buffer+(SBC_MAX_OUTPUT_BUFSIZE/4)));


#if APP_SUPPORT_SOUNDBAR == 1
            switch(Soundbar_App_Get_Role_Status())
            {
                case SOUNDBAR_SLAVE     : 
                    vpp_SBCSetMixMode(3); 
                    break;
                    
                case SOUNDBAR_MASTER   : 
                    vpp_SBCSetMixMode(2); 
                    break;
                    
                case SOUNDBAR_NOROLE   : 
                    vpp_SBCSetMixMode(0); 
                    break;

                default: 
                    vpp_SBCSetMixMode(0); break;
            }
#endif

            return MCI_ERR_NO;
        }
        
        vpp_SBCDecStatus(pVoc_AudioDecStatus);
        AudioSBCCheckIsMute(sbc_mute_times, g_mmcCheckSbcMuteCallback);
#ifndef KALAOK_SUPPORT
        MCI_MergeSideTone(pVoc_AudioDecStatus->sample_length, g_last_buffer_addr);
#else
#if 1
        if(input->inFlag == 2)
        {
            uint16 i = 0;
            INT16 *pMicData, *pAudioData;
            INT32 temp_value;
            if(Mmc_SBC_Pcm_Half)
            {
                pMicData = (INT16 *)audioInStream.startAddress + (audioInStream.length >> 2);
            }
            else
            {
                pMicData = (INT16 *)audioInStream.startAddress ;
            }

            pAudioData = (INT16 *)g_last_buffer_addr;

            Effectecho_config(&echo_param, audioInStream.sampleRate, ECHO_DELAY_MAX);

            Effectecho_process(pMicData, pMicData, audioInStream.length >> 2);

            for(i = 0; i<pVoc_AudioDecStatus->sample_length/2; i++)
            {
                //*(pAudioData+i) = *(pAudioData+i) >> 1 + *(pMicData+i);
                *(pAudioData+i) = *(pAudioData+i) >> 1;
                //*(pAudioData+i) = *(pMicData+i) << 1;
                temp_value = *(pAudioData+i) + *(pMicData+i);
                if(temp_value > 32767)
                {
                    hal_HstSendEvent(SYS_EVENT,0x19890420);

                    *(pAudioData+i) = 32767;
                }
                else if(temp_value < -32767)
                {
                    hal_HstSendEvent(SYS_EVENT,0x19890421);

                    *(pAudioData+i) = -32767;
                }
                else
                {
                    *(pAudioData+i) = (INT16)temp_value;
                }
            }
        }
#endif
#endif

#ifdef SINDEBUG
        {
            uint16 i = 0;
            for(i = 0; i<pVoc_AudioDecStatus->sample_length/128; i++)
            {
                memcpy((UINT8 *)g_last_buffer_addr + i*128 , sintone, 128);
            }
        }
#endif
        /*
        hal_HstSendEvent(SYS_EVENT,0x12010030);
        hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->consumedLen);
        hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->sample_length);
        hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);*/
        //MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AudioSBCDecVoc] consumedLen=%d bytes;sample_length=%d bytes,ErrorStatus=%d bytes\n", pVoc_AudioDecStatus->consumedLen,pVoc_AudioDecStatus->sample_length,pVoc_AudioDecStatus->ErrorStatus);        
        if(input->inFlag==1)
        {
            if(pVoc_AudioDecStatus->ErrorStatus==0
                && !(pVoc_AudioDecStatus->sample_length%4)
                &&pVoc_AudioDecStatus->consumedLen>0)//11251
            {
                // setup ouput buffer size as dec frame sise
                if(output->pcm.SampleRate != pVoc_AudioDecStatus->freq)
                {
                    hal_HstSendEvent(SYS_EVENT,0x11140030);
                    hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->freq);
                    g_SampleRateChangeFlag = 1;
                }
                output->pcm.Size = pVoc_AudioDecStatus->sample_length >> 1;
                output->pcm.SampleRate = pVoc_AudioDecStatus->freq;
                output->pcm.Channel = pVoc_AudioDecStatus->channels;

#ifdef A2DP_SAMPLE_44_1K_TO_48K
                if(pVoc_AudioDecStatus->freq==SBC_SAMPLING_FREQ_44100 && pVoc_AudioDecStatus->channels==2)
                {
                    output->pcm.SampleRate=SBC_SAMPLING_FREQ_48000;
                    hal_HstSendEvent(SYS_EVENT,0x19891001);
                }
#endif

				if(g_VocBusy == 0)
		        {
		            hal_HstSendEvent(SYS_EVENT, 0x88400022);
		            vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
		            g_VocBusy = 1;
                    Mmc_SBC_Pcm_Half = 1; // put next data to half buf;
		            // prepare for nect frame decode
		            Audio_SBCPrepareNextFrame(pSBCPlayer);
		        }
		        else
		        {
		            hal_HstSendEvent(SYS_EVENT, 0x8840000a);
		        }
#ifndef TARGET_A2DPDISK_SUPPORT
                MMC_SBCPcmOutStart(pSBCPlayer); 
#endif
                input->inFlag = 2;
                ErrorFrameCount = 0;

                hal_HstSendEvent(SYS_EVENT,0x20161711);
                hal_HstSendEvent(SYS_EVENT,output->pcm.Size);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->freq);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->blocks);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->channels);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->subbands);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->bitpool);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->blocks);

            }
            else
            {
                MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AudioSBCDecVoc] error!");

                hal_HstSendEvent(SYS_EVENT,0x12010070);
                hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);

                ErrorFrameCount++;
                if(ErrorFrameCount>50)
                {
                    MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SBCDEC_ERROR]ErrorFrameCount:%d ",ErrorFrameCount);
                    return MCI_ERR_ERROR;				
                }

            }

        }

        if(pVoc_AudioDecStatus->ErrorStatus != 0 || pVoc_AudioDecStatus->consumedLen<=0 || pVoc_AudioDecStatus->sample_length%4)
        {
            hal_HstSendEvent(SYS_EVENT,0x12010060);
            hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);

#ifdef SBCDEC_PROTECT_PCMOUT
            if(pVoc_AudioDecStatus->ErrorStatus == 9)
            {
                hal_HstSendEvent(SYS_EVENT,0x12010061);
                hal_DbgAssert("SBCDecPcmOutAddr is error!");  
            }
#endif
            // find next packet
            length = (pVoc_AudioDecStatus->consumedLen >0)? pVoc_AudioDecStatus->consumedLen + 1 : 1;
            while(length < input->length)
            {
                if(input->data[length] == 0x9c)
                    break;
                length++;
            };
            
            if(length < input->length)
            {
                memcpy(input->data, input->data+length, input->length-length);
                input->length -= length;
            }
            else
            {
                input->length = 0;
            }

            hal_HstSendEvent(SYS_EVENT,length);
            hal_HstSendEvent(SYS_EVENT,input->length);
        }
        else
        {
            length = input->length-pVoc_AudioDecStatus->consumedLen;
            if(length > 0)
            {
                memcpy(input->data, input->data+pVoc_AudioDecStatus->consumedLen, length);
            }
            else
                length = 0;
            
            input->length = length;
            
#ifdef MUSIC_BAND_INFO
            vpp_SBCGetBandInfo(SbcBandInfo);
#endif
        }

        if(input->length < SBC_INPUT_BUFSIZE)
        {
            COS_EVENT event={0};
            t_DataBuf *p_bufbak;
            //INT32 i,j;

            event.nEventId = RDABT_A2DP_DATA_COMPLETED_MSG;

            while(SbcBtPacket.start_pos != SbcBtPacket.end_pos)  
            {
                p_bufbak = SbcBtPacket.BtPacketQueue[SbcBtPacket.start_pos];
                event.nParam1 = (UINT32)p_bufbak;

                if(p_bufbak->dataLen <= SBC_INPUT_BUFSIZE-input->length)
                {
                    UINT16 data_len = 0;

                    data_len = p_bufbak->dataLen;
                    memcpy(input->data+input->length, p_bufbak->buf, p_bufbak->dataLen);
                    COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
                    input->length += data_len;

#ifdef SBC_DUMP_DATA_DEBUG
                    if(gSbcDebugCurFd >= 0)
                    {
                        int iret;
                        iret = FS_Write(gSbcDebugCurFd, input->data+input->length-data_len, data_len);
                        hal_HstSendEvent(SYS_EVENT,0x88100006);
                        hal_HstSendEvent(SYS_EVENT,data_len);
                        hal_HstSendEvent(SYS_EVENT,iret);
                    }
#endif
                    SbcBtPacket.BtPacketQueue[SbcBtPacket.start_pos] = NULL;
                    SbcBtPacket.start_pos++;
                    if(SbcBtPacket.start_pos == 8)
                        SbcBtPacket.start_pos = 0;
                }
                else
                {
                break;
                }
            }
        }
    }        
    else
#endif	
        if(0)
        { // disable sbc file play
#if 0
            if(input->inFlag==0)
            {
                input->inFlag = 1;

                // decode first frame
                vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
                return MCI_ERR_NO;
            }

            vpp_SBCDecStatus(pVoc_AudioDecStatus);

#if 0
            hal_HstSendEvent(SYS_EVENT,0x12010030);
            hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->consumedLen);
            hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->sample_length);
            hal_HstSendEvent(SYS_EVENT,pVoc_AudioDecStatus->ErrorStatus);
#endif

            if(input->inFlag==1)
            {
                // return MCI_ERR_NO;

                if(pVoc_AudioDecStatus->ErrorStatus!=0)
                    return MCI_ERR_ERROR;

                g_SBCConsumedLength += pVoc_AudioDecStatus->consumedLen;
                if(g_SBCConsumedLength>=g_SBCFILESIZE)
                    return MCI_ERR_END_OF_FILE;

                // setup ouput buffer size as dec frame sise
                output->pcm.Size = pVoc_AudioDecStatus->sample_length >> 1;
                output->pcm.SampleRate = pVoc_AudioDecStatus->freq;
                output->pcm.Channel = pVoc_AudioDecStatus->channels==SBC_CHANNEL_MODE_MONO?1:2;
                hal_HstSendEvent(SYS_EVENT,output->pcm.Size);
                hal_HstSendEvent(SYS_EVENT,output->pcm.SampleRate);
                hal_HstSendEvent(SYS_EVENT,output->pcm.Channel);

                length = input->length-pVoc_AudioDecStatus->consumedLen;
                if(length > 0)
                {
                    if(length <= SBC_INPUT_BUFSIZE)
                    {
                        memcpy(input->data, input->data+pVoc_AudioDecStatus->consumedLen, length);
                    }
                    else
                    {
                        memcpy(input->data, input->data+pVoc_AudioDecStatus->consumedLen, SBC_INPUT_BUFSIZE);
                        mmc_aud_FileSeek(input->fileHandle,length-SBC_INPUT_BUFSIZE,FS_SEEK_CUR);
                        length = 0;
                    }
                }
                else
                {
                    mmc_aud_FileSeek(input->fileHandle,-length,FS_SEEK_CUR);
                    length = 0;
                }
                length = mmc_aud_FileRead(input->fileHandle, input->data+length, input->length-length);
                //hal_HstSendEvent(SYS_EVENT,length);
                
                if(length<0)
                {
                    return MCI_ERR_END_OF_FILE;
                }
                pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)(output->pcm.Buffer+(output->pcm.Size>>1));

                vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);

                pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->pcm.Buffer;

                MMC_SBCPcmOutStart(pSBCPlayer);    
                input->inFlag = 2;

                return MCI_ERR_NO;
            }
            // return MCI_ERR_NO;
            //    pcm_count++;
            //  if(pcm_count>1)
            //    return MCI_ERR_NO;    
            if(pVoc_AudioDecStatus->consumedLen > 0)
            {
                g_SBCConsumedLength += pVoc_AudioDecStatus->consumedLen;
                //if(g_SBCConsumedLength>=g_SBCFILESIZE)
                //    return MCI_ERR_END_OF_FILE;
                length = input->length-pVoc_AudioDecStatus->consumedLen;
                if(length > 0)
                {   
                    uint8 temp;
                    memcpy(input->data, input->data+pVoc_AudioDecStatus->consumedLen, length);
                    //hal_HstSendEvent(SYS_EVENT,0x12010040);
                    //hal_HstSendEvent(SYS_EVENT,length);

                }
                else if(length < 0)
                {
                    mmc_aud_FileSeek(input->fileHandle,-length,FS_SEEK_CUR);
                    length = 0;
                }

                length = mmc_aud_FileRead(input->fileHandle, input->data+length, input->length-length);
                //hal_HstSendEvent(SYS_EVENT,length);
                if(length<0)
                {
                    return MCI_ERR_END_OF_FILE;
                }

            }  

            if(pVoc_AudioDecStatus->ErrorStatus==0)
            {

                ErrorFrameCount=0;
            }
            else
            {
                ErrorFrameCount++;
                if(ErrorFrameCount>100)
                {
                    MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SBCDEC_ERROR]ErrorFrameCount:%d ",ErrorFrameCount);
                    return MCI_ERR_ERROR;				
                }
            }

            if(g_PCMPassFlag)
            {
                vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
                g_VocBusy = 1;
                g_ReadFileFlag = 0;
                g_PCMPassFlag = 0;

                // prepare for nect frame decode
                Audio_SBCPrepareNextFrame(pSBCPlayer);
            }
            else
                g_ReadFileFlag = 1;
#endif
    }
    return MCI_ERR_NO;

}


/*
* NAME:	MMC_AudioSBCDecPcm()
* DESCRIPTION: Initialize SBC decoder.
*/
MCI_ERR_T MMC_AudioSBCDecPcm(SBC_PLAY *pSBCPlayer)
{
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    vpp_SBC_DEC_IN_T *pVoc_Voc_AudioDecIN = &pSBCPlayer->Voc_AudioDecIN;
    //int32 i,readlen;
    //uint32 length;

#ifdef SBC_DEBUG_TICKS
    u_int32 sbc_tick;
    sbc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x22200000 + sbc_tick - g_SbcPcmTicks);
    g_SbcPcmTicks = sbc_tick;
#endif

    if(ContinuePlayFlag)
    {
        diag_printf("!!!!!play all left data in the pcm buffer\n");
        MMC_AudioSBCDecClose(pSBCPlayer);
        MCI_AudioFinished(MCI_ERR_END_OF_FILE);

        mmc_SetCurrMode(MMC_MODE_IDLE);
        ContinuePlayFlag = 0;
    }

    if(input->length >= SBC_PLAY_MIN_INPUT_SIZE)
    {
        // file data read, decode one frame
        if(g_VocBusy == 0)
        {
            vpp_SBCDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
            g_VocBusy = 1;

            // prepare for nect frame decode
            Audio_SBCPrepareNextFrame(pSBCPlayer);
        }
        else
        {
            hal_HstSendEvent(SYS_EVENT, 0x88400002);
        }
    }
    else
    {
        // set flag
        INT16 sbcPacketNum = 0;

        sbcPacketNum = Audio_Get_Sbc_Buf_Num();

        hal_HstSendEvent(SYS_EVENT,0x88400001);
        hal_HstSendEvent(SYS_EVENT,input->length);
        //hal_HstSendEvent(SYS_EVENT,sbcPacketNum);

        MMC_AudioCleanA2dpData();


        memset(output->pcm.Buffer_head, 0, output->pcm.Size<<1);
        Audio_SBCPrepareNextFrame(pSBCPlayer);
    }

    MMC_AudioAutoCalibSameple(pSBCPlayer,0);

    return MCI_ERR_NO;

}

/*
* NAME:	MMC_AudioSBCDecClose()
* DESCRIPTION: Close SBC decoder.
*/
MCI_ERR_T MMC_AudioSBCDecClose (SBC_PLAY *pSBCPlayer)
{
    hal_HstSendEvent(SYS_EVENT,0x10250101);

    if(pSBCPlayer == NULL)
        return MCI_ERR_NO;
    
    pSBCPlayer->SBCInput.inFlag = 0;

    vpp_SBCDecClose();

    hal_SysSetVocClockDefault();

    aud_StreamStop(audioItf);
	Mmc_SBC_Pcm_Half = 0;   

    hal_HstSendEvent(SYS_EVENT, 0x08220001);
    mmc_MemFreeAll();

    hal_HstSendEvent(SYS_EVENT, 0x08220002);

    COS_Free(SBCPlayer);
    SBCPlayer = NULL;

#ifdef SBC_DUMP_DATA_DEBUG
    if(gSbcDebugCurFd >= 0)
    {
        FS_Close(gSbcDebugCurFd);     
    }
#endif    

#ifdef KALAOK_SUPPORT
    Effectecho_destory();
#endif

    return  MCI_ERR_NO;
}


#ifdef TARGET_A2DPDISK_SUPPORT

MCI_ERR_T Audio_SBCReadData(INT16 *buff, UINT32 length)
{
    SBC_PLAY *pSBCPlayer = SBCPlayer;
    SBC_OUTPUT *output = &pSBCPlayer->SBCOutput;
    SBC_INPUT *input = &pSBCPlayer->SBCInput;
    INT32 next_frame = 0;

    //hal_HstSendEvent(SYS_EVENT,0x11090010);
    //hal_HstSendEvent(SYS_EVENT,length);    
    if(g_SampleRateChangeFlag)
    {
        g_SampleRateChangeFlag = 0;
        hal_HstSendEvent(SYS_EVENT,0x11140020);
        return MCI_ERR_END_OF_FILE;
    }
    
    if(input->inFlag != 2)
    {
        memset(buff, 0, length);
        return MCI_ERR_NO;
    }

    if(Mmc_SBC_Pcm_Half == 1 && output->buff_offset + length >= SBC_MAX_OUTPUT_BUFSIZE/2)
    {
        next_frame = 1;
        //Mmc_SBC_Pcm_Half = 0;
    }
    else if(Mmc_SBC_Pcm_Half == 0 && output->buff_offset + length >= SBC_MAX_OUTPUT_BUFSIZE)
    {
        //Mmc_SBC_Pcm_Half = 1;
        next_frame = 1;
    }

    if(next_frame)
    {
        if(g_VocBusy==1)
        {
            hal_HstSendEvent(SYS_EVENT,0x11120010);
        }
    }

    if(output->buff_offset + length >= SBC_MAX_OUTPUT_BUFSIZE)
    {
        memcpy(buff, output->pcm.Buffer + output->buff_offset/4, SBC_MAX_OUTPUT_BUFSIZE-output->buff_offset);
        output->buff_offset = length+output->buff_offset-SBC_MAX_OUTPUT_BUFSIZE;
        if(output->buff_offset > 0)
            memcpy(buff, output->pcm.Buffer, length - output->buff_offset);
    }
    else
    {
        memcpy(buff, output->pcm.Buffer + output->buff_offset/4, length);
        output->buff_offset += length;
    }
    //hal_HstSendEvent(SYS_EVENT,next_frame);    
    //hal_HstSendEvent(SYS_EVENT,Mmc_SBC_Pcm_Half);    
    //hal_HstSendEvent(SYS_EVENT,output->buff_offset);    
    

    if(next_frame)
    {
        Mmc_SBC_Pcm_Half = !Mmc_SBC_Pcm_Half;
        MMC_AudioSBCDecPcm(pSBCPlayer);
    }
    
	return  MCI_ERR_NO;
}

int32 Audio_SBCGetSampleRate(void)
{
	SBC_OUTPUT *output = &SBCPlayer->SBCOutput;
	SBC_INPUT *input = &SBCPlayer->SBCInput;
    int32 sample_rate;
	switch (output->pcm.SampleRate)
	{
	case SBC_SAMPLING_FREQ_48000:
  		sample_rate   = HAL_AIF_FREQ_48000HZ;
		break;
	case SBC_SAMPLING_FREQ_44100:
		sample_rate   = HAL_AIF_FREQ_44100HZ;
		break;
	case SBC_SAMPLING_FREQ_32000:
		sample_rate   = HAL_AIF_FREQ_32000HZ;
		break;
	case SBC_SAMPLING_FREQ_16000:
		sample_rate   = HAL_AIF_FREQ_16000HZ;
		break;
	default:
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_SBC_DEC_ERROR]ERROR SAMPLERATE:%d",output->pcm.SampleRate);
		sample_rate   = HAL_AIF_FREQ_44100HZ;
		break;
	}
    return sample_rate;
}

#endif

int32 Audio_SBCPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
    int32 result;

    hal_HstSendEvent(SYS_EVENT,0x10250001);
    if(SBCPlayer == NULL)
        SBCPlayer = COS_Malloc(sizeof(SBC_PLAY));

    g_SBCConsumedLength=0;

#ifdef BT_SUPPORT
    // Initial cfg
    if(gpio_detect_earpiece())
        audioItf = AUD_ITF_BT_EP;
    else
        audioItf = AUD_ITF_BT_SP;

    if(0) // disable sbc file play MCI_Play_BTStream == 0)
#endif   
    {
        if (fhd< 0)
        {
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_ERROR]ERROR FILE HANDLER: %d \n", fhd);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        if ((g_SBCFILESIZE=mmc_aud_GetFileSize(fhd))< 0)
        {
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_AUDIO_ERROR]CAN NOT GET FILE SIZE!:%d",g_SBCFILESIZE);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_SBC]FILE SIZE: %d",g_SBCFILESIZE);

        g_SBCConsumedLength=(INT32)(((INT64)PlayProgress*(INT64)g_SBCFILESIZE)/10000);

        MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_SBC]g_AMRConsumedLength: %d",g_SBCConsumedLength);

        if ((result=mmc_aud_FileSeek(fhd,g_SBCConsumedLength,FS_SEEK_SET))< 0)
        {
            diag_printf("[MMC_AUDIO_ERROR]can not seek file!:%d",result);
            return MCI_ERR_CANNOT_OPEN_FILE;
        }
    }

    if((result= MMC_AudioSBCDecOpen(fhd, SBCPlayer))!=MCI_ERR_NO)
    {
        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC_ERR]MMC_AudioDecOpen() RETURN FALSE!");
        MMC_AudioSBCDecClose(SBCPlayer);
        return result;
    }

    return  MCI_ERR_NO;
}

int32 Audio_SBCStop (void) 	
{
	MMC_AudioSBCDecClose(SBCPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_SBCPause (void)	
{
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC] Audio_SBCPause!!!!");

	MMC_AudioSBCDecClose(SBCPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_SBCResume ( HANDLE fhd) 
{
	//int32 result;
	
	MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC]Audio_SBCResume!");
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC] Audio_SBCResume!!!!");
#if 0
	g_SBCConsumedLength-=(SBC_INPUT_BUFSIZE);

	if(g_SBCConsumedLength<0)g_SBCConsumedLength=0;

	result=mmc_aud_FileSeek(fhd,0,FS_SEEK_SET);

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_SBC]FILE SEEK POSITON:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC_ERR]FS_seek error!");
		return MCI_ERR_BAD_FORMAT;
	}


	if((result= MMC_AudioSBCDecOpen(fhd, SBCPlayer))!=MCI_ERR_NO)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC_ERR]MMC_AudioDecOpen RETURN FALSE!");
		MMC_AudioSBCDecClose(SBCPlayer);
		return result;
	}
#endif
	return  MCI_ERR_NO;
}


int32 Audio_SBCGetID3 (char * pFileName)  
{
    return 0;	
}

#ifdef MUSIC_BAND_INFO
int32 Audio_SBCGetFreqBandInfo(UINT16 *band)
{    
    UINT16 i;

    for(i = 0; i < 8; i++)
    {
        band[i] = SbcBandInfo[i];
    }

    return MCI_ERR_NO;
}
#endif

int32 Audio_SBCGetPlayInformation (MCI_PlayInf * MCI_PlayInfSBC)  
{
    if(g_SBCFILESIZE == 0)
        MCI_PlayInfSBC->PlayProgress = 0 ;
    else
        MCI_PlayInfSBC->PlayProgress=(INT32)((((INT64)g_SBCConsumedLength+g_SBCHeaderLength)*10000)/g_SBCFILESIZE);

    MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_SBC]PlayProgress:%d",MCI_PlayInfSBC->PlayProgress);

    return MCI_ERR_NO;
}

int32 Audio_SBCUserMsg(COS_EVENT *ev)
{
    //hal_HstSendEvent(SYS_EVENT,0x10250020);
    //hal_HstSendEvent(SYS_EVENT,nMsg);
    int32 nMsg = ev->nEventId;  
    switch (nMsg)
    {
        case MSG_MMC_SBC_VOC:
            switch (MMC_AudioSBCDecVoc(SBCPlayer))
            {
                case MCI_ERR_END_OF_FILE:
                    diag_printf("[MMC_AUDIO]MMC_SBCDecRun() return FINISH!\n");

                    ContinuePlayFlag = 1;
                    break;
                    
                case MCI_ERR_ERROR:
                    hal_HstSendEvent(SYS_EVENT,0x65900022);
                    diag_printf("[MMC_AUDIO]##WARNING##MMC_SBCDecRun() return FALSE!\n");

                    MMC_AudioSBCDecClose(SBCPlayer);
                    MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
                    mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
                    ContinuePlayFlag = 0;
                    break;
                    
                default:
                    break;
            }
            break;


#ifndef TARGET_A2DPDISK_SUPPORT
        case MSG_MMC_SBC_PCM:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
            hal_abbFirstEnablePa();
#endif
            if(MMC_AudioSBCDecPcm(SBCPlayer)==MCI_ERR_END_OF_FILE)
            {

                MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC]END OF FILE!");				
            }
            break;
#endif

        case MSG_MMC_SBC_END:
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC]MSG_MMC_SBC_END\n");
            MMC_AudioSBCDecClose(SBCPlayer);
            MCI_AudioFinished(MCI_ERR_END_OF_FILE);
            mmc_SetCurrMode(MMC_MODE_IDLE);
            break;

#ifdef BT_SUPPORT
        case MSG_MMC_AUDIODEC_A2DP:
            MMC_AudioReceiveA2dpData(SBCPlayer, (t_DataBuf *)ev->nParam1);
            break;
            
#if APP_SUPPORT_SOUNDBAR == 1
        case MSG_MMC_AUDIODEC_CONTROL:
            MMC_Soundbar_Control_Audio_Sync((SOUNBAR_AUDIO_SYNC_STRUCT *)ev->nParam1);
            break;
#endif
#endif

        default:
            MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_SBC_ERR]ERROR MESSAGE!");
            break;
    }
    return 0;
}


