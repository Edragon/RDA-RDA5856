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
#include "mci.h"
#include "hal_host.h"

#include "mcip_debug.h"

#ifdef VIDEO_PLAYER_SUPPORT
#include "video_api.h"
#endif
//#include "med_main.h"
#include "fs_asyn.h"

#include "audio_api.h"
//#include "mmc_camera.h"

#include "mmc_audiomp3stream.h"
#include "vidrec_api.h"

#include "cos.h"
#include "dm.h"
#include "mmc_sndrec.h"
#include "apfs.h"
#include "mmc_dafrtpss.h"

#include "apbs_m.h"
#include "mmc_midi.h"
#include "aud_fm.h"
#include "fmd_m.h"
#include "mcip_debug.h"

#include "hal_overlay.h"

#include "mmc_audiosco.h"
#include "ap_media.h"

//#include "mmc_analogtv.h"
//#include "cam_motion_detect.h"
#ifdef CHIP_DIE_8809E
#define MCI_DEFAULT_XCPU_FREQ   CSW_SYS_FREQ_78M
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#define MCI_DEFAULT_XCPU_FREQ   CSW_SYS_FREQ_52M
#else
#define MCI_DEFAULT_XCPU_FREQ   CSW_SYS_FREQ_104M
#endif

static HANDLE g_ServerTaskHandle = HNULL;

static HANDLE g_AppTaskHandle = HNULL;

static HANDLE g_CodecTaskHandle = HNULL;

static HANDLE g_AgentTaskHandle = HNULL;

static HANDLE g_FsappTaskHandle = HNULL;


static HANDLE g_MMCTaskHandle = HNULL;

static HANDLE g_DITaskHandle = HNULL;

static HANDLE g_ImageTaskHandle = HNULL;

 mci_type_enum  g_audio_format;

#define COS_MMC_TASKS_PRIORITY_BASE 180

#define MMC_TASK_PRIORITY       (COS_MMC_TASKS_PRIORITY_BASE+0)
#define IMAGE_TASK_PRIORITY (COS_MMC_TASKS_PRIORITY_BASE+1)
#define USB_TASK_PRIORITY       (COS_MMC_TASKS_PRIORITY_BASE+2)

// for asyn fs task.
#define ASYNFS_TASK_PRIORITY (COS_MMC_TASKS_PRIORITY_BASE+3)

#define MMC_TASK_STACK_SIZE     2048      // 2k
#define IMAGE_TASK_STACK_SIZE       2048    // 2k
#define USB_TASK_STACK_SIZE     2048      // 2k

// for asyn fs task.
#define ASYNFS_TASK_STACK_SIZE 2048   // 2k



extern PUBLIC UINT32 gpio_detect_earpiece(VOID);
extern void mmc_EndStream(void);


/**************************************************************

    FUNCTION NAME       : GetMCITaskHandle

    PURPOSE             : Get    Task   Handle

    INPUT PARAMETERS    : module_type mod

    OUTPUT PARAMETERS   : VOID

    RETURNS             : HANDLE

    REMARKS             :

**************************************************************/
HANDLE GetMCITaskHandle(COSMBOXID mod)
{
    switch (mod)
    {
        case MBOX_ID_SERVER:
            return g_ServerTaskHandle;
        case MBOX_ID_APP:
            return g_AppTaskHandle;
        case MBOX_ID_COCEC:
            return g_CodecTaskHandle;
        case MBOX_ID_AGENT:
            return g_AgentTaskHandle;
        case MBOX_ID_FSAPP:
            return g_FsappTaskHandle;
                case MBOX_ID_MMC:
                        return g_MMCTaskHandle;
        case MBOX_ID_DI:
            return g_DITaskHandle;
          case MBOX_ID_IMAGE:
                        return g_ImageTaskHandle;
        default:
                //ASSERT(0);
            return g_ServerTaskHandle;
    }
}
/**************************************************************

    FUNCTION NAME       : SetMCITaskHandle

    PURPOSE             : Set  MmiTask  Handle

    INPUT PARAMETERS    : module_type mod, HANDLE hTask

    OUTPUT PARAMETERS   : VOID

    RETURNS             : BOOL

    REMARKS             :

**************************************************************/
BOOL SetMCITaskHandle(COSMBOXID mod, HANDLE hTask)
{
    switch (mod)
    {
        case MBOX_ID_SERVER:
            g_ServerTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_APP:
            g_AppTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_COCEC:
            g_CodecTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_AGENT:
            g_AgentTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_FSAPP:
            g_FsappTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_MMC:
            g_MMCTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_DI:
            g_DITaskHandle = hTask;
            return TRUE;
        case MBOX_ID_IMAGE:
            g_ImageTaskHandle = hTask;
            return TRUE;
        default:
            //ASSERT(0);
            return FALSE;
    }
}


extern VOID MMCTask(UINT32 data);
extern VOID ImageTask(UINT32 data);

extern void mmi_ebook_usb_mode_off(void);
extern void mmi_ebook_usb_mode_on(void);
extern BOOL mmc_BuffInit(UINT8 *Pdata, UINT32 len, UINT8 LoopTimes);
/*
static INT32 MMC_GetLcdWidth()
{
     return LCD_WIDTH;
}

static INT32 MMC_GetLcdHeight()
{
    return LCD_HEIGHT;
}
*/
extern AUD_ITF_T audioItf;

extern HANDLE g_hAsynFsTask;

INT32 MMC_LcdWidth = 0;
INT32 MMC_LcdHeight = 0;
#ifdef DUAL_LCD
INT32 MMC_SubLcdWidth = 0;
INT32 MMC_SubLcdHeight = 0;
#endif

/*
init lcd size for mci.
*/
BOOL MCI_LcdSizeSet(INT32 LcdWidth, INT32 LcdHeight)
{
    if(LcdWidth<=0 || LcdHeight<=0)
        return FALSE;
    MMC_LcdWidth = LcdWidth;
    MMC_LcdHeight = LcdHeight;
    return TRUE;
}

#ifdef DUAL_LCD
BOOL MCI_SubLcdSizeSet(INT32 LcdWidth, INT32 LcdHeight)
{
    if(LcdWidth <= 0 || LcdHeight <= 0)
        return FALSE;
    MMC_SubLcdWidth = LcdWidth;
    MMC_SubLcdHeight = LcdHeight;
    return TRUE;
}
#endif

INT32  MMC_GetBVGA(INT32 bvga)
{
          return  bvga;
}

BOOL MCI_TaskInit(VOID)  //  MCI_MEDIA_PLAY_REQ,
{

diag_printf( "*******************MCI_TaskInit**********************");
 mmc_MemInit();
     	//MMC_LcdWidth=MMC_GetLcdWidth();
	//MMC_LcdHeight=MMC_GetLcdHeight();
/*
 HANDLE hTask;
hTask = COS_CreateTask((PTASK_ENTRY)MMCTask,
                NULL, NULL,
                MMC_TASK_STACK_SIZE,
                MMC_TASK_PRIORITY,
                COS_CREATE_DEFAULT, 0, "MMC Task");
    //ASSERT(hTask != HNULL);
    SetMCITaskHandle(MBOX_ID_MMC, hTask);

 // creat asyn fs task.
 g_hAsynFsTask = COS_CreateTask(BAL_AsynFsTask,
                NULL, NULL,
                ASYNFS_TASK_STACK_SIZE,
                ASYNFS_TASK_PRIORITY,
                COS_CREATE_DEFAULT, 0, "ASYNFS Task");
 */


/*
hTask = COS_CreateTask(MMCTask,
                NULL, NULL,
                USB_TASK_STACK_SIZE,
                USB_TASK_PRIORITY,
                COS_CREATE_DEFAULT, 0, "USB Task");
    ASSERT(hTask != HNULL);
    SetMCITaskHandle(MBOX_ID_MMC, hTask);*/

    return TRUE;

}






UINT32 MCI_Speach(boolean on) //chenhe for jasperII,for  can't speach on when calling
{
    return 0;
}

MCI_AUDIO_PLAY_CALLBACK_T g_mciAudioFinishedCallback = NULL;
MCI_STREAM_USER_HANDLER_T g_mmcStreamUserHandler = NULL;

PRIVATE MCI_AUDIO_BUFFER_PLAY_CALLBACK_T g_mciRingFinishedCallback = NULL;
MCI_AUDIO_FILE_RECORD_CALLBACK_T g_mciAudioRecordFinishedCallback = NULL;

UINT8 MCI_Play_Stream_Buffer = 0;
#ifdef BT_SUPPORT

UINT8 MCI_Play_BTStream;
bt_a2dp_audio_cap_struct *MCI_BTAudioCap;

UINT32 MCI_PlayBTStream (bt_a2dp_audio_cap_struct *audio_cap)
{
	INT32 result=0;
       mci_type_enum mediaType;

       hal_HstSendEvent(SYS_EVENT,0x10250010);
       hal_HstSendEvent(SYS_EVENT,(UINT32)audio_cap);
       hal_HstSendEvent(SYS_EVENT,audio_cap->codec_type);

       csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);
       if(audio_cap->codec_type == AVDTP_MEDIA_CODEC_SBC)
            mediaType = MCI_TYPE_SBC;
       else if(audio_cap->codec_type == AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO )
            mediaType = MCI_TYPE_DAF;
       else if(audio_cap->codec_type == AVDTP_MEDIA_CODEC_MPEG2_4_AAC)
	   	    mediaType = MCI_TYPE_AAC;
       else
            return MCI_ERR_BAD_FORMAT;

	g_mciAudioFinishedCallback=NULL;
       g_mmcStreamUserHandler = NULL;
       MCI_BTAudioCap = audio_cap;
       MCI_Play_BTStream = 1;
       MCI_Play_Stream_Buffer = 0;
#ifdef LINEIN_RECORDER_SUPPORT
    mediaType = MCI_TYPE_LINEIN_BT;
#endif
	result     = LILY_AudioPlay(0, 0,mediaType, 0);
    hal_HstSendEvent(SYS_EVENT,0x10250100);
    hal_HstSendEvent(SYS_EVENT,result);
    return result;
}


UINT32 MCI_StartBTSCO (UINT16 handle)
{
	INT32 result=0;
       mci_type_enum mediaType = MCI_TYPE_SCO;

       MCI_Play_BTStream = 1;
       MCI_Play_Stream_Buffer = 0;
	result     = LILY_AudioPlay(0, handle,mediaType, 0);

    hal_HstSendEvent(SYS_EVENT,0x10270100);
    hal_HstSendEvent(SYS_EVENT,result);
    return result;
}

UINT32 MCI_RecordBTSCO (HANDLE fileHandle)
{
    MCI_Play_BTStream = 1;
    return Aduio_SCORecord(fileHandle);
}

#endif

VOID MCI_AudioFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);

    mmc_aud_ClearFileHandle();

    if (g_mciAudioFinishedCallback)
    {
        (*g_mciAudioFinishedCallback)(result);
    }

    if (g_mmcStreamUserHandler)
    {
       (*g_mmcStreamUserHandler)(STREAM_STATUS_END);
    }
}

VOID MCI_RingFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    if (g_mciRingFinishedCallback)
    {
        (*g_mciRingFinishedCallback)(result);
    }
}

VOID MCI_AudioRecordFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    if (g_mciAudioRecordFinishedCallback)
    {
        (*g_mciAudioRecordFinishedCallback)(result);
    }
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

mci_type_enum audio_mode;
void MCI_vid_play_finish_ind(int32 result)
{
     if (g_mciAudioFinishedCallback)
    {
        (*g_mciAudioFinishedCallback)(MCI_ERR_END_OF_FILE);
    }
}

UINT32 MCI_AudioPlay (INT32 OutputPath,HANDLE fileHandle, mci_type_enum fielType,MCI_AUDIO_PLAY_CALLBACK_T callback,INT32 PlayProgress)   //  MCI_MEDIA_PLAY_REQ,
{
	INT32 result=MCI_ERR_NO;
	diag_printf( "***********OutputPath:%d*****file_name_p:%d, PlayProgress :%d ,fielType :%d ",OutputPath,fileHandle,PlayProgress,fielType);
	g_mciAudioFinishedCallback=callback;
    g_mmcStreamUserHandler = NULL;

    MCI_Play_Stream_Buffer = 0;
#ifdef BT_SUPPORT
        MCI_Play_BTStream = 0;
#endif

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);

     audio_mode = fielType;

	//Read file from FS
	mmc_aud_ClearFileHandle();

#ifdef VIDEO_PLAYER_SUPPORT
      if((audio_mode >=MCI_TYPE_3GP)&&(audio_mode <=MCI_TYPE_MJPG))
        {

          if(PlayProgress ==0)
            {
            result=  MCI_VideoOpenFile(2,2, fileHandle, 0, 0, fielType, MCI_vid_play_finish_ind,NULL);

	    if (result !=ISOM_ERR_OK)
	    	{
	        result = MCI_VideoClose();
                 return MCI_ERR_ERROR;
	    	}
                  MCI_VideoPlay (0,0)  ;
            }
          else
            {
        	 INT16 img_width = 0;
        	 INT16 img_height = 0;
        	 INT32 totaltime = 0;

                UINT16  aud_channel;
                UINT16  aud_sample_rate;
                UINT16  track;
               UINT16  audio_type;
              INT64  current_Progress;

              result=MCI_VideoOpenFile(2,2, fileHandle, 0, 0, fielType, MCI_vid_play_finish_ind,NULL);
               audio_type = MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);

              current_Progress= (((INT64)PlayProgress*(INT64)totaltime)/10000);

             MCI_VideoSeek (current_Progress, SEEK_TIME_MODE_ABSOLUTE, 0, 0) ;
              MCI_VideoPlay (0,0)  ;
            }

        }
    else
#endif
	result = LILY_AudioPlay(OutputPath, fileHandle,fielType, PlayProgress);
    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}


UINT32 MCI_AudioPause(VOID)
{
    INT32 result;
    result = LILY_AudioPause();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}

extern INT MMI_FS_Seek(HANDLE FileHandle, INT32 Offset, INT32 Whence);

UINT32 MCI_AudioGetDurationTime(HANDLE fileHandle, mci_type_enum fielType,INT32 BeginPlayProgress,INT32 OffsetPlayProgress,MCI_ProgressInf* PlayInformation)
{

	return apfs_GetDurationTime( fileHandle,  fielType, BeginPlayProgress, OffsetPlayProgress, PlayInformation);

#if 0
   INT32 result =MCI_ERR_NO;

      if((audio_mode >=MCI_TYPE_3GP)&&(audio_mode <=MCI_TYPE_MJPG))
        {

     	      INT16 img_width = 0;
        	 INT16 img_height = 0;
        	 INT32 totaltime = 0;

                UINT16  aud_channel;
                UINT16  aud_sample_rate;
                UINT16  track;
               UINT16  audio_type;
              INT64  current_Progress;

	      MMI_FS_Seek(fileHandle, 0, 0);
            result=MCI_VideoOpenFile(2,2, fileHandle, 0, 0, fielType, MCI_vid_play_finish_ind,NULL);

	     MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);

             current_Progress= (((INT64)OffsetPlayProgress*(INT64)totaltime)/10000);

             MCI_VideoSeek (current_Progress, SEEK_TIME_MODE_ABSOLUTE, 0, 0) ;

              PlayInformation->DurationTime= MCI_VideoGetPlayTime();//*10000/ totaltime;

	   result = MCI_VideoStop();
          result = MCI_VideoClose();

        }
      else
       result = apfs_GetDurationTime( fileHandle,  fielType, BeginPlayProgress, OffsetPlayProgress, PlayInformation);

     return  result;
#endif
}


UINT32 MCI_AudioResume(HANDLE fileHandle)
{
    INT32 result;
    diag_printf( "##################MCI_AudioResume############################");
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);
    result = LILY_AudioResume( fileHandle );
    return result;
}


UINT32 MCI_AudioStop(VOID)
{
    INT32 result;
    hal_HstSendEvent(1, 0x20161453);
    hal_HstSendEvent(1, audio_mode);
       if((audio_mode >=MCI_TYPE_3GP)&&(audio_mode <=MCI_TYPE_MJPG))
        {
          result = MCI_VideoStop();
          result = MCI_VideoClose();
        }
      else
        result = LILY_AudioStop();
    MCI_Play_Stream_Buffer = 0;
#ifdef BT_SUPPORT
    MCI_Play_BTStream = 0;
#endif

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}

INT32 MCI_AudioPlayLeftData(VOID)
{
    INT32 result;
    hal_HstSendEvent(1, 0x20161454);
    hal_HstSendEvent(1, audio_mode);
       if((audio_mode >=MCI_TYPE_3GP)&&(audio_mode <=MCI_TYPE_MJPG))
        {
          result = MCI_VideoStop();
          result = MCI_VideoClose();
        }
      else
        result = LILY_AudioPlayLeftData();
    MCI_Play_Stream_Buffer = 0;
#ifdef BT_SUPPORT
    MCI_Play_BTStream = 0;
#endif

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}

UINT32 MCI_AudioGetID3(char *pfilename)     //  MCI_MEDIA_PLAY_REQ,
{
    return 0;
}

UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* PlayInformation)     //  MCI_MEDIA_PLAY_REQ,
{

      INT32 result =MCI_ERR_NO;
	 if((audio_mode >=MCI_TYPE_3GP)&&(audio_mode <=MCI_TYPE_MJPG))
        {

	 INT16 img_width = 0;
	 INT16 img_height = 0;
	 INT32 totaltime = 0;

        UINT16  aud_channel;
        UINT16  aud_sample_rate;
        UINT16  track;

	  MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);
	  if(totaltime>0)
         	PlayInformation->PlayProgress= (INT32)((INT64)MCI_VideoGetPlayTime()*10000  / totaltime);
	  else
	  	PlayInformation->PlayProgress=0;

        }
      else
            result = LILY_AudioGetPlayInformation(PlayInformation);

    	return result;
}

#ifdef MUSIC_BAND_INFO

UINT32 MCI_AudioGetFreqBandInfo(uint16 * FreqBand)
{
	return LILY_AudioGetFreqBandInfo(FreqBand);
}
#endif

UINT32 MCI_AudioSetVolume(UINT16 volume)
{
    return 0;
}


UINT32 MCI_AudioSetOutputPath(UINT16 OutputPath,UINT16 Onoff)
{
    return 0;
}


UINT32 MCI_AudioSetEQ(AUDIO_EQ EQMode)
{
    INT32 result=0;

    result = LILY_AudioSetEQ(EQMode);

    return result;
}


 MCI_ERR_T MCI_AudioGetFileInformation (CONST HANDLE FileHander,
											AudDesInfoStruct  * CONST DecInfo,
											CONST mci_type_enum FileType   )
 {
    INT32 result=0;

      if((FileType >=MCI_TYPE_3GP)&&(FileType <=MCI_TYPE_MJPG))
        {

	 INT16 img_width = 0;
	 INT16 img_height = 0;
	 INT32 totaltime = 0;

        UINT16  aud_channel = 0;
        UINT16  aud_sample_rate = 0;
        UINT16  track = 0;
        UINT16  audio_type = 0;

       result=  MCI_VideoOpenFile(2,2, FileHander, 0, 0, FileType, MCI_vid_play_finish_ind,NULL);
	 audio_type= MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);

       if(audio_type ==MCI_TYPE_AMR)
        {
         DecInfo->sampleRate=0;
         DecInfo->stereo    = 0;
        }
       else
        {
         DecInfo->sampleRate=aud_sample_rate;
         DecInfo->stereo    = aud_channel;
        }

          result = MCI_VideoClose();
           return MCI_ERR_NO;
        }
      else
	return apfs_GetFileInformation(FileHander, DecInfo, FileType);
 }


#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

extern MPEG_PLAY *MpegPlayer;

void mmc_audio_bt_sco(bool flag)
{

MpegPlayer->MpegInput.bt_sco = flag;
MpegPlayer->MpegInput.pcmflag = TRUE;
}


#ifdef __BT_A2DP_PROFILE__
A2DP_codec_struct *MCI_A2dpDafOpen( A2DP_Callback pHandler,
									    bt_a2dp_audio_cap_struct *daf_config_data,
									    UINT8 *buf, UINT32 buf_len)
{

    MPEG_INPUT *mpeg_input_p = &(MpegPlayer->MpegInput);
    mpeg_input_p->audio_config_handle = daf_config_data;

    return A2DP_DAF_Open( &(mpeg_input_p->ss_handle),  pHandler, &(mpeg_input_p->audio_config_handle)->codec_cap.mp3, buf,  buf_len);
}



void MCI_A2dpDafClose(void)
{
    MPEG_INPUT *mpeg_input_p = &(MpegPlayer->MpegInput);
     A2DP_DAF_Close(&(mpeg_input_p->ss_handle));
     return;
}

extern A2DP_codec_struct *SBC_Open( SSHdl **ss_handle,  A2DP_Callback pHandler,
							bt_a2dp_sbc_codec_cap_struct *sbc_config_data,
   							UINT8 *buf,   UINT32 buf_len );

extern VOID SBC_Close( SSHdl **ss_handle );

A2DP_codec_struct *MCI_SBCOpen(    A2DP_Callback pHandler,
									    bt_a2dp_audio_cap_struct *daf_config_data,
									    UINT8 *buf, UINT32 buf_len)
{

    MPEG_INPUT *mpeg_input_p = &(MpegPlayer->MpegInput);
    mpeg_input_p->audio_config_handle = daf_config_data;
 return SBC_Open( &(mpeg_input_p->ss_handle),  pHandler, &(mpeg_input_p->audio_config_handle)->codec_cap.sbc, buf,  buf_len);

 return 0;
}


void MCI_SBCClose(void )
{
    MPEG_INPUT *mpeg_input_p = &(MpegPlayer->MpegInput);
     SBC_Close(&(mpeg_input_p->ss_handle));
     return;
}




#endif
#if 1//defined(APBS)


#if 0
static VOID MCI_AudioPlayStreamFinishCbk(MCI_ERR_T state)
{
	if(g_mmcStreamUserHandler)
	{
		g_mmcStreamUserHandler(STREAM_STATUS_END);
	}
}
#endif

INT32 MCI_AudioSetupStream(UINT8 *pBuffer, UINT32 len, UINT32 vaildLen, UINT32 threshold,mci_type_enum  format)
{
    g_audio_format = format;
    mmc_InitStream(pBuffer,len, vaildLen, threshold);

	return MCI_ERR_NO;
}

INT32  MCI_AudioPlayStream(UINT8 loop,
	MCI_STREAM_USER_HANDLER_T callback,
	INT32 startPosition)
{
    INT32 result;

    if(1)
    {

#ifdef BT_SUPPORT
	    MCI_Play_BTStream = 0;
#endif
    	g_mmcStreamUserHandler = callback;
	    g_mciAudioFinishedCallback = NULL;
	    result = LILY_AudioPlay(0, 0,g_audio_format, startPosition);
    }
    else
        return MCI_ERR_BAD_FORMAT;

    return result;
}

INT32  MCI_AudioStopStream(void)
{
    INT32 ret = 0;
    ret = MCI_AudioStop();
    mmc_EndStream();

    return ret;
}

INT32  MCI_AudioPlayAllLeftData(void)
{
	return MCI_AudioPlayLeftData();
}

UINT32 MCI_AudioPlayPause(VOID)
{
    return MCI_AudioPause();
}

UINT32 MCI_AudioPlayResume(VOID)
{
    return MCI_AudioResume(0);
}
UINT32 MCI_AddedData(UINT8* addedData, UINT32 addedDataBytes)
{
	return mmc_AddedData(addedData, addedDataBytes);
}

UINT32 MCI_GetRemainingBytes(void)
{
	return mmc_GetRemainingBytes();
}
UINT32 MCI_GetWriteBuffer(UINT8 **buffer)
{
	return mmc_GetWriteBuffer(buffer);
}
#endif /* APBS */

#ifdef SOUND_RECORDER_SUPPORT
UINT32 MCI_AudioRecordStart (HANDLE fhd,mci_type_enum format,U8 quality,UINT32 samplerate, MCI_AUDIO_FILE_RECORD_CALLBACK_T callback,  MCI_AUDIO_RECORD_BUFFER_PLAY_CALLBACK_T usercallback)
{
    g_mciAudioRecordFinishedCallback=callback;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);

    INT32 result = Mmc_sndRecStart(fhd,quality,format,samplerate,callback, usercallback);
    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}

UINT32 MCI_AudioRecordPause(VOID)
{
    INT32 result = Mmc_sndRecPause();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}
UINT32 MCI_AudioRecordResume(VOID)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);
    INT32 result = Mmc_sndRecResume();
    return result;
}
UINT32 MCI_AudioRecordStop(VOID)
{
    INT32 result = Mmc_sndRecStop();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}
#endif

//FMRADIO START
UINT32 MCI_FM_RDA5800_init(uint8 scl,uint8 sda)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_close(VOID)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_open(UINT32 bUseLastFreq)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_tune(UINT32 iFrequency,UINT32 isStereo)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_mute(UINT32 isMute)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_setVolume(UINT32 iVol)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_seek(UINT32 isSeekUp)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_I2S(VOID)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_STOPI2S(VOID)
{
    return 0;
}


VOID MCI_FmSetOutputDevice(MCI_AUDIO_PATH_T device)
{
    AUD_SPK_T out = AUD_SPK_EAR_PIECE;
    if (device == MCI_PATH_HP || device == MCI_PATH_FM_HP)
    {
        out = AUD_SPK_EAR_PIECE;
    }
    else if (device == MCI_PATH_LSP || device == MCI_PATH_FM_LSP_HP)
    {
        out = AUD_SPK_LOUD_SPEAKER;
    }

    aud_SetCurOutputDevice(out);
}


//extern uint32 pcmbuf_overlay[4608];

static BOOL g_FmBT8KEnbleFlag=0;

UINT32 MCI_FmPlay(void)
{
	AUD_ERR_T audError;
	HAL_AIF_STREAM_T stream;
	AUD_LEVEL_T level;

	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_FmPlay!");

    level.spkLevel=AUD_SPK_MUTE; // MMI will set the volumn later
    level.sideLevel=0;
    level.micLevel=0;
    level.toneLevel=0;
    // Set audio interface
    stream.startAddress = (UINT32*)pcmbuf_overlay;
    stream.length = 6400;
    stream.playSyncWithRecord = FALSE;

    // initialize the interrupt function.
    stream.halfHandler = NULL;
    stream.endHandler = NULL;

     if(g_FmBT8KEnbleFlag==1)
     	{
	        stream.sampleRate = HAL_AIF_FREQ_8000HZ;
	        stream.channelNb = HAL_AIF_MONO;
	        stream.voiceQuality = TRUE;

		//hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);

		FMD_ERR_T fmdError = FMD_ERR_NO;
		FMD_I2S_CFG_T fmdI2sCfg={0};

		fmdI2sCfg.freq=FMD_FREQ_8000HZ;

	       fmdError= fmd_I2sOpen( fmdI2sCfg);

		if (fmdError != FMD_ERR_NO)
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen FAILED");
			return AUD_ERR_RESOURCE_BUSY;
		}
		else
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen SUCCESS");
		}
     	}
	 else
	 {
	        stream.sampleRate = HAL_AIF_FREQ_32000HZ;
	        stream.channelNb = HAL_AIF_STEREO;
	        stream.voiceQuality = FALSE;

		//hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);

		FMD_ERR_T fmdError = FMD_ERR_NO;
		FMD_I2S_CFG_T fmdI2sCfg={0};

		switch (stream.sampleRate)
		{
		case HAL_AIF_FREQ_8000HZ:
	  		fmdI2sCfg.freq=FMD_FREQ_8000HZ;
			break;

		case HAL_AIF_FREQ_11025HZ:
			fmdI2sCfg.freq=FMD_FREQ_11025HZ;
			break;

		case HAL_AIF_FREQ_12000HZ:
			fmdI2sCfg.freq=FMD_FREQ_12000HZ;
			break;

		case HAL_AIF_FREQ_16000HZ:
			fmdI2sCfg.freq=FMD_FREQ_16000HZ;
			break;

		case HAL_AIF_FREQ_22050HZ:
			fmdI2sCfg.freq=FMD_FREQ_22050HZ;
			break;

		case HAL_AIF_FREQ_24000HZ:
			fmdI2sCfg.freq=FMD_FREQ_24000HZ;
			break;

		case HAL_AIF_FREQ_32000HZ:
			fmdI2sCfg.freq=FMD_FREQ_32000HZ;
			break;

		case HAL_AIF_FREQ_44100HZ:
			fmdI2sCfg.freq=FMD_FREQ_44100HZ;
			break;

		case HAL_AIF_FREQ_48000HZ:
			fmdI2sCfg.freq=FMD_FREQ_48000HZ;
			break;
		default:
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]Sample rate erro:%d",stream.sampleRate);
			break;
		}

		fmdI2sCfg.slave=TRUE;

	       fmdError= fmd_I2sOpen( fmdI2sCfg);

		if (fmdError != FMD_ERR_NO)
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen FAILED");
			return AUD_ERR_RESOURCE_BUSY;
		}
		else
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen SUCCESS");
		}


	 }

	audioItf = AUD_ITF_FM;

	audError= aud_StreamStart(audioItf, &stream, &level);

	if (audError != AUD_ERR_NO)
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]aud_FmStreamStart FAILED");
	}
	else
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]aud_FmStreamStart SUCCESS");
	}

	return audError;
}

//extern U8 GetHandsetInPhone(VOID);

UINT32 MCI_FmStop(VOID)
{
    AUD_ERR_T audError = AUD_ERR_NO;

    MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_FmStop!audioItf = %d",audioItf);


    while((audError=aud_StreamStop(audioItf)) != AUD_ERR_NO)
    {
        COS_Sleep(100);
    }

    // Directly call MMI API GetHandsetInPhone() here.
    // In the future audioItf will be managed only in CSW.
    //if (1 == GetHandsetInPhone())
	if(gpio_detect_earpiece())
    {
	    audioItf = AUD_ITF_EAR_PIECE;
    }
    else
    {
        audioItf = AUD_ITF_LOUD_SPEAKER;
    }

    fmd_I2sClose();
    //hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);

    return audError;
}


UINT32 MCI_FmSetup(UINT8 volume )
{
	AUD_ERR_T audError = AUD_ERR_NO;
	AUD_LEVEL_T cfg;

	if(volume>7)volume=7;



	cfg.spkLevel=volume;
	cfg.micLevel=0;
	cfg.sideLevel=0;
	cfg.toneLevel=0;



	audError=aud_Setup(audioItf,  & cfg);

	return audError;
}


//fmradio record
UINT32 MCI_FmrRecordStart (char *file_name_p)   //  MCI_MEDIA_PLAY_REQ,
{
    return 0;
}
UINT32 MCI_FmrRecordPause(VOID)
{
    return 0;
}
UINT32 MCI_FmrRecordResume(VOID)
{
    return 0;
}
UINT32 MCI_FmrRecordStop(VOID)
{
    return 0;
}

UINT32 MCI_LineinPlay(void)
{
	AUD_ERR_T audError;
	HAL_AIF_STREAM_T stream;
	AUD_LEVEL_T level;

	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_FmPlay!");

       level.spkLevel=AUD_SPK_MUTE; // MMI will set the volumn later
	level.sideLevel=0;
	level.micLevel=0;
	level.toneLevel=0;
       // Set audio interface
	stream.startAddress = (UINT32*)pcmbuf_overlay;
       stream.length = 6400;

       // initialize the interrupt function.
       stream.halfHandler = NULL;
       stream.endHandler = NULL;


        stream.sampleRate = HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;HAL_AIF_FREQ_8000HZ;//
        stream.channelNb = HAL_AIF_STEREO;
        stream.voiceQuality = FALSE;

	audioItf = AUD_ITF_LINEIN;

	audError= aud_StreamStart(audioItf, &stream, &level);

	if (audError != AUD_ERR_NO)
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]MCI_LineinPlay FAILED");
	}
	else
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]MCI_LineinPlay SUCCESS");
	}

	return audError;

}

UINT32 MCI_LineStop(void)
{
    AUD_ERR_T audError = AUD_ERR_NO;

    MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_LineStop!audioItf = %d",audioItf);

    while((audError=aud_StreamStop(audioItf)) != AUD_ERR_NO)
    {
        hal_HstSendEvent(SYS_EVENT,audError);
        if(audError == AUD_ERR_RESOURCE_BUSY) // play music with fm output
            break;
        COS_Sleep(100);
    }
    if(audError == AUD_ERR_NO)
    {
        if(gpio_detect_earpiece())
            audioItf = AUD_ITF_EAR_PIECE;
        else
            audioItf = AUD_ITF_LOUD_SPEAKER;
    }
    return audError;
}

//Buffer len (bytes)
UINT32 MCI_Play_Stream_Buffer_Length = 0;
UINT8 *MCI_Play_Stream_Buffer_Address = 0;

UINT32 MCI_PlayInternalAudio(UINT8 type, UINT32 length, UINT8 *data, MCI_AUDIO_PLAY_CALLBACK_T callback,UINT8 times)
{
    INT32 result=0;
    mci_type_enum mediaType;

    hal_HstSendEvent(SYS_EVENT,0x06110010);
    hal_HstSendEvent(SYS_EVENT,type);

    if(type == 3)
        mediaType = MCI_TYPE_DAF;
    else if(type == 1)
        mediaType = MCI_TYPE_TONE;
    else
        return MCI_ERR_BAD_FORMAT;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, MCI_DEFAULT_XCPU_FREQ);
    g_mciAudioFinishedCallback=callback;
    MCI_Play_Stream_Buffer = 0; // TODO: Delete this flag
#ifdef BT_SUPPORT
    MCI_Play_BTStream = 0;
#endif
    g_audio_format = mediaType;
    if(mediaType == MCI_TYPE_TONE)
    {
        MCI_Play_Stream_Buffer_Address = data;
        MCI_Play_Stream_Buffer_Length = length;
        result = LILY_AudioPlay(0, 0,mediaType, times);
    }
    else
    {
        mmc_BuffInit(data,length,times);
        result = LILY_AudioPlay(0, 0,mediaType, 0);
    }

    hal_HstSendEvent(SYS_EVENT,0x06110011);
    hal_HstSendEvent(SYS_EVENT,result);
    return result;
}

UINT32 MCI_StopInternalAudio(void)
{
    return MCI_AudioStop();
}


UINT32 MCI_PlayNotificationSnd (UINT8 audio_id,MCI_STREAM_USER_HANDLER_T callback)
{
    UINT32 ret = 0;
    media_PlayInternalAudio(audio_id, 1, FALSE);
    g_mmcStreamUserHandler = callback;
    return ret;
}

#ifdef KALAOK_SUPPORT

UINT32 MCI_PlayMicroPhoneAudio(UINT8 type, MCI_AUDIO_PLAY_CALLBACK_T callback,UINT8 times)
{
	INT32 result=0;
	mci_type_enum mediaType;

	hal_HstSendEvent(SYS_EVENT,0x06111010);
	hal_HstSendEvent(SYS_EVENT,type);

	if(type == 0)
	mediaType = MCI_TYPE_MICRO_PHONE;
	else
	return MCI_ERR_BAD_FORMAT;

	g_mciAudioFinishedCallback=callback;
    g_mmcStreamUserHandler = NULL;
	MCI_Play_Stream_Buffer = 0; // TODO: Delete this flag


	result = LILY_AudioPlay(0, 0,MCI_TYPE_MICRO_PHONE, times);

	hal_HstSendEvent(SYS_EVENT,0x06111011);
	hal_HstSendEvent(SYS_EVENT,result);
	return result;
}

UINT32 MCI_StopMicroPhoneAudio(void)
{
    return MCI_AudioStop();
}

#endif

/**************************************************************************/
/**************************************************************************/
/******************Video function API   ***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_VID_CLOSE_REQ,          // 7
        MCI_VID_OPEN_FILE_REQ,      // 8
        MCI_VID_FILE_READY_IND,     // 9
        MCI_VID_PLAY_REQ,               // 10
        MCI_VID_PLAY_FINISH_IND,        // 11
        MCI_VID_STOP_REQ,               // 12
        MCI_VID_PAUSE_REQ,          // 13
        MCI_VID_RESUME_REQ,         // 14
        MCI_VID_SEEK_REQ,               // 15
        MCI_VID_SEEK_DONE_IND,
*/


UINT32  MCI_DisplayVideoInterface (VOID)
{
#ifdef VIDEO_PLAYER_SUPPORT
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_DISPLAY_INTERFACE_REQ);
#endif
    return 0;
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

UINT32  MCI_VideoClose (VOID)   //MCI_VID_CLOSE_REQ
{
    INT32 result=0;
#ifdef VIDEO_PLAYER_SUPPORT
    result = LILY_VideoClose();
#endif
    return result;

}

 UINT32 MCI_VideoOpenFile (INT32 OutputPath, uint8 open_audio, HANDLE fhd, UINT8 file_mode, UINT32 data_Len, mci_type_enum type, void(*vid_play_finish_callback)(int32), void (*vid_draw_panel_callback)(uint32))   //MCI_VID_OPEN_FILE_REQ
 {

    INT32 result=0;
    //INT32 i;

#ifdef VIDEO_PLAYER_SUPPORT
    result     = LILY_VideoOpenFile( fhd, file_mode, data_Len, OutputPath,open_audio, type, vid_play_finish_callback, vid_draw_panel_callback);
 #endif
#if 0
    if (result == 0)
    {
        mci_vid_file_ready_ind_struct msg;
        media_vid_file_ready_ind_struct *ind_p=NULL ;
        ilm_struct       *local_ilm_ptr = NULL;
        LILY_VideoGetInfo(&msg);
           ind_p = (media_vid_file_ready_ind_struct*)
                    construct_local_para(sizeof(media_vid_file_ready_ind_struct), TD_CTRL);
           if(!ind_p)return 1;
           ind_p->result = (int16)result;
           ind_p->image_width=msg.image_width;
           ind_p->image_height=msg.image_height;
           ind_p->total_frame_num=msg.total_frame_num;
           ind_p->total_time=msg.total_time;
           ind_p->seq_num=msg.seq_num;
           ind_p->media_type =msg.media_type;

           local_ilm_ptr = allocate_ilm(MOD_MED);
           if(!local_ilm_ptr)return 1;
           local_ilm_ptr->src_mod_id = MOD_MED;
           local_ilm_ptr->dest_mod_id = MOD_MMI;
           local_ilm_ptr->sap_id = MED_SAP;

           local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_READY_IND;
           local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
           local_ilm_ptr->peer_buff_ptr = NULL;

           i=0;
           while(!mmc_SendMsg(GetMmiTaskHandle(local_ilm_ptr->dest_mod_id),local_ilm_ptr->msg_id, (uint32)local_ilm_ptr, 0, 0))
           {
            i++;
            diag_printf( "COS_SendEvent 2 MMI fail i=%d",i);
            COS_Sleep(20);//ms
            if(i>=3)
            {
                result=1;
                break;
            }
           }

    }
#endif

    return result;
}

UINT32 MCI_VideoPlayFinishInd (mci_vid_play_finish_ind_struct *plyFinish)
{
    return 0;
}


UINT32  MCI_VideoGetPlayTime (VOID)
{
#ifdef VIDEO_PLAYER_SUPPORT
    mci_vid_file_ready_ind_struct fileInfo;
    LILY_VideoGetInfo(&fileInfo);
    return fileInfo.current_time;
#else
	return 0;
#endif
}

UINT16  MCI_VideoGetInfo ( UINT16  *image_width, UINT16  *image_height, UINT32  *total_time, UINT16  *aud_channel, UINT16  *aud_sample_rate,UINT16  *track)
{
#ifdef VIDEO_PLAYER_SUPPORT
    mci_vid_file_ready_ind_struct fileInfo;
    LILY_VideoGetInfo(&fileInfo);
    *image_width=fileInfo.image_width;
    *image_height=fileInfo.image_height;
    *total_time=fileInfo.total_time;
   *aud_channel = fileInfo.aud_channel;
   *aud_sample_rate = fileInfo.aud_sample_rate;
   *track = fileInfo.track;
      return fileInfo.audio_type;
#else
       return -1;
#endif

}


UINT32 MCI_VideoRecordFinishInd (INT32 finishCause)
{
    return 0;
}

UINT32 MCI_VideoSeekDoneInd (mci_vid_seek_done_ind_struct *pSeekDone)
{
    return 0;
}

 VOID MCI_VideoOpenFileInd (mci_vid_file_ready_ind_struct *vdoOpen)    //MCI_VID_FILE_READY_IND
 {
    return;
}

 UINT32 MCI_VideoPlay (UINT16 startX, UINT16 startY)   //MCI_VID_PLAY_REQ
{
    INT32 result=0;
#ifdef VIDEO_PLAYER_SUPPORT
    result = LILY_VideoPlay(startX,startY);
#endif
    return result;
}

VOID  MCI_VideoPlayInd (VOID)   // MCI_VID_PLAY_FINISH_IND
{
    return;
}


 UINT32 MCI_VideoStop (VOID)    // MCI_VID_STOP_REQ
{
    INT32 result=0;
#ifdef VIDEO_PLAYER_SUPPORT
    result = LILY_VideoStop();
#endif
    return result;
}

UINT32  MCI_VideoPause (VOID)  //  MCI_VID_PAUSE_REQ
{
    INT32 result=0;
#ifdef VIDEO_PLAYER_SUPPORT
    result = LILY_VideoPause();
#endif
    return result;
}

UINT32  MCI_VideoResume (VOID)  // MCI_VID_RESUME_REQ
{
    INT32 result=0;
#ifdef VIDEO_PLAYER_SUPPORT
    result = LILY_VideoResume();
#endif
    return result;

}

 UINT32 MCI_VideoSeek (long long playtime, INT32 time_mode, INT16 startX, INT16 startY)   // MCI_VID_SEEK_REQ
  {
    INT32 result=0;
    //media_vid_seek_done_ind_struct *ind_p=NULL;
    //ilm_struct       *local_ilm_ptr = NULL;
    //INT32 i;
#ifdef VIDEO_PLAYER_SUPPORT
    result    = LILY_VideoSeek(playtime, time_mode, startX, startY);
#endif
#if 0
    ind_p = (media_vid_seek_done_ind_struct*)
        construct_local_para(sizeof(media_vid_seek_done_ind_struct), TD_CTRL);
    if(!ind_p)return 1;
    ind_p->result = (int16)result;
    local_ilm_ptr = allocate_ilm(MOD_MED);
    if(!local_ilm_ptr)return 1;
    local_ilm_ptr->src_mod_id = MOD_MED;
    local_ilm_ptr->dest_mod_id = MOD_MMI;
    ind_p->seq_num = 0;
    local_ilm_ptr->sap_id = MED_SAP;

    local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_DONE_IND;
    local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
    local_ilm_ptr->peer_buff_ptr = NULL;

       i=0;
   while(!mmc_SendMsg(GetMmiTaskHandle(local_ilm_ptr->dest_mod_id),local_ilm_ptr->msg_id, (uint32)local_ilm_ptr, 0, 0))
     {
        i++;
        diag_printf( "COS_SendEvent 2 MMI fail i=%d",i);
        COS_Sleep(20);//ms
        if(i>=3)
        {
            result=1;
            break;
        }
       }
#endif
    return result;
}

VOID  MCI_VideoSeekInd (VOID)  // MCI_VID_SEEK_DONE_IND
{
    return;
}

UINT32 MCI_VideoSet (UINT16 ZoomWidth, UINT16 ZoomHeight, INT16 startX, INT16 startY, INT16 cutX, INT16 cutY, INT16 cutW, INT16 cutH, UINT16 Rotate)  // MCI_VID_SET_MODE_REQ
{
	UINT32 ret=0;
#ifdef VIDEO_PLAYER_SUPPORT
	mci_vid_set_mode_ind_struct SetMode;

	SetMode.lcd_start_x = startX;
	SetMode.lcd_start_y = startY;
	SetMode.zoom_height = ZoomHeight;
	SetMode.zoom_width = ZoomWidth;
	SetMode.cutX= cutX;
	SetMode.cutY= cutY;
	SetMode.cutW= cutW;
	SetMode.cutH= cutH;
	SetMode.rotate=Rotate;
    ret = LILY_VideoSetInfo(&SetMode);
#endif
	return ret;
}

#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */


/**************************************************************************/
/**************************************************************************/
/******************Camera function API***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_CAM_POWER_UP_REQ,       // 17
        MCI_CAM_POWER_DOWN_REQ, // 18
        MCI_CAM_PREVIEW_REQ,            // 19
        MCI_CAM_STOP_REQ,               // 20
        MCI_CAM_CAPTURE_REQ,            // 21
        MCI_CAM_SET_PARAM_REQ,      // 22
*/
#ifdef CAMERA_SUPPORT
#if (CSW_EXTENDED_API_CAMERA == 0)
#if defined(CAM_MD_SUPPORT)
INT32 cam_md_state=0;//0=close,1=open,2=need reopen
#endif

UINT32 MCI_CamPowerUp (INT32  vediomode,void (*cb)(int32))   // MCI_CAM_POWER_UP_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPowerUp.vediomode%d",vediomode);

#if defined(CAM_MD_SUPPORT)
	if(cam_md_state)
	{
		MCI_CamMdClose();
		//only close and not reopen again.
		cam_md_state=0;//reopen need keep 2 parameters.
	}
#endif

	return Lily_Camera_PowerOn(vediomode,cb);
}

UINT32 MCI_CamPowerDown (VOID)  //MCI_CAM_POWER_DOWN_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPowerDown");
	return Lily_Camera_PowerOff();
}

UINT32 MCI_CamPlayBack ()  //MCI_CAM_PLAY_BACK
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPlayBack");

	return Lily_Capture_Preview();
}

INT32 MCI_CamSavePhoto (INT32 filehandle)  //MCI_CAM_SAVE_PHOTO
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamSavePhoto.filehandle:%d",filehandle);
	return Lily_camera_savePhoto(filehandle);
}



UINT32 MCI_CamPreviewOpen(CAM_PREVIEW_STRUCT* data)  // MCI_CAM_PREVIEW_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPreviewOpen");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]start_x:%d",data->start_x);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]start_y:%d",data->start_y);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]end_x:%d",data->end_x);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]end_y:%d",data->end_y);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_width:%d",data->image_width);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_height:%d",data->image_height);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]nightmode:%d",data->nightmode);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]imageQuality:%d",data->imageQuality);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]factor:%d",data->factor);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]contrast:%d",data->contrast);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]specialEffect:%d",data->specialEffect);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]brightNess:%d",data->brightNess);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]whiteBlance:%d",data->whiteBlance);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]exposure:%d",data->exposure);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]addFrame:%d",data->addFrame);

	return Lily_Camera_Preview(data);
}

UINT32 MCI_CamPreviewClose(VOID)  // MCI_CAM_STOP_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPreviewClose");
	return Lily_Camera_PreviewOff();
}

 UINT32 MCI_CamCapture (CAM_CAPTURE_STRUCT *data)    //MCI_CAM_CAPTURE_REQ
 {
 	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamCapture");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_width:%d",data->image_width);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_height:%d",data->image_height);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]media_mode:%d",data->media_mode);

    return Lily_Camera_Capture(data);
 }

 UINT32 MCI_CamSetPara(INT32 effectCode,INT32 value)  // MCI_CAM_SET_PARAM_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamSetPara ");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]effectCode:%d",effectCode);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]value:%d",value);

	return Lily_Camera_SetEffect(effectCode,value);
}

UINT32 MCI_CamCancelLastSave(VOID)  // MCI_CAM_PREVIEW_REQ
{
    return 0;
}

#if defined(IP_CAMERA_SUPPORT)
UINT32 MCI_IPCamStart(CAM_CAPTURE_STRUCT* data)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"MCI_IPCamStart");

    Lily_cameraStartCapture(data);

    return 0;

}

UINT32 MCI_IPCamStop(VOID)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"MCI_IPCamStop");
	return Lily_cameraStopCapture();
}

UINT32 MCI_IPCamGetFrame(UINT8 *buffer, UINT32 buffersize)
{
	return Lily_cameraGetEncodeData(buffer, buffersize);
}

#endif /* IP_CAMERA_SUPPORT */
#if defined(CAM_MD_SUPPORT)
//open camera motion detect
INT32 MCI_CamMdOpen(INT32 sensLevel, VOID(*md_callback)(int32))
{
	if( motion_detect_open( sensLevel, md_callback)==0)
	{
		cam_md_state=1;
		return 0;
	}
	else
	{
		cam_md_state=0;
		return -1;
	}
}
//close camera motion detect
INT32 MCI_CamMdClose( VOID)
{
	cam_md_state=0;
	return motion_detect_close();
}

VOID MCI_CamDoMd( VOID)
{
	do_motion_detect();
}
#endif // CAM_MD_SUPPORT


#endif /* CSW_EXTENDED_API_CAMERA */
#endif

/**************************************************************************/
/**************************************************************************/
/******************Image  function API***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_IMG_DECODE_REQ,         // 23
    MCI_IMG_DECODE_EVENT_IND,   // 24
    MCI_IMG_DECODE_FINISH_IND,  // 25
    MCI_IMG_RESIZE_REQ,         // 26
    MCI_IMG_STOP_REQ,               // 27
*/


UINT32 MCI_ImgCheckDecodeState(VOID)
{
    return 0;
}

UINT32 MCI_ImgResize (img_decode_req_struct *imgPara)  // MCI_IMG_RESIZE_REQ
{
    return 0;
}
UINT32 MCI_ImgStop (VOID)  //MCI_IMG_STOP_REQ
{
    return 0;
}


INT32 MCI_ImgDisplay(uint8 startx,uint8 starty, uint8 width,uint8 height,char * filename)  // MCI_DI_IMAGE_DISPLAYSTILL_IND
{
    return 0;
}


UINT32 MCI_getLilyStatus(UINT32 cmd_arg)
{
    return 0;
}

#ifdef VIDEO_RECORDER_SUPPORT
UINT32 MCI_VideoRecordPreviewStart (MMC_VDOREC_SETTING_STRUCT *previewPara, VOID(*vid_rec_finish_ind)(uint16 msg_result))
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPreviewStart!");
	return LILY_VidrecPreviewStartReq(previewPara,vid_rec_finish_ind);
}

UINT32 MCI_VideoRecordAdjustSetting (INT32 adjustItem, INT32 value)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordAdjustSetting!");
	return LILY_VidrecAdjustSettingReq(adjustItem, value);
}

UINT32 MCI_VideoRecordPreviewStop(VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPreviewStop!");
	return LILY_VidrecPreviewStopReq();
}

UINT32 MCI_VideoRecordStart(HANDLE filehandle)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordStart!");
	return LILY_VidrecRecordStartReq(filehandle);
}

UINT32 MCI_VideoRecordStop( VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordStop!");
	return LILY_VidrecRecordStopReq();
}

UINT32 MCI_VideoRecordPause(VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPause!");
	return LILY_VidrecRecordPauseReq();
}

UINT32 MCI_VideoRecordResume( VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordResume!");
	return LILY_VidrecRecordResumeReq();
}

UINT32 MCI_VideoRecordSound(INT32 *PCMdata, UINT32 length)
{
    return 0;
}
#endif




#ifdef __PNG_DECODE_WITH_VOC__




#include "vpp_pngdec.h"




PRIVATE PNGDEC_USER_HANDLER_T g_PngDecUserHandler = NULL;

void PngDecVocISR(INT32 value)
{

   if(g_PngDecUserHandler!=NULL)
   {
   	g_PngDecUserHandler(value);
   }


   return;
}




INT32 MCI_PngdecStart(UINT32 *FileContent,
			  UINT32 FileSize,
			  UINT32 *PNGOutBuffer,
			  UINT32 OutBufferSize,
			  UINT32 *AlphaOutBuffer,
			  UINT32 AlphaBufferSize,
			  UINT32 *BkgrdBuffer,
			  UINT16 BkgrdClipX1,
			  UINT16 BkgrdClipX2,
			  UINT16 BkgrdClipY1,
			  UINT16 BkgrdClipY2,
			  INT16 BkgrdOffsetX,
			  INT16 BkgrdOffsetY,
			  UINT16 BkgrdWidth,
			  UINT16 BkgrdHeight,
			  UINT16 ZoomWidth,
			  UINT16 ZoomHeight,
			  UINT16 DecMode,
			  UINT16 alpha_blending_to_transparentColor,
			  UINT32 *ImgWidth,
			  UINT32 *ImgHeigh ,
			  UINT32*WindowBuf,
       		 UINT32 *rgb_temp_ptr,
       		 UINT32 PngFileHandle,
			  PNGDEC_USER_HANDLER_T handle

			  )
{




	VPP_PNGDEC_CFG_T g_vppPngDecCfg;



	hal_HstSendEvent(SYS_EVENT,0xabcd1233);
	hal_HstSendEvent(SYS_EVENT,FileContent);

/*		hal_HstSendEvent(SYS_EVENT,FileSize);
		hal_HstSendEvent(SYS_EVENT,PNGOutBuffer);
		hal_HstSendEvent(SYS_EVENT,OutBufferSize);
		hal_HstSendEvent(SYS_EVENT,AlphaOutBuffer);
		hal_HstSendEvent(SYS_EVENT,AlphaBufferSize);
	hal_HstSendEvent(SYS_EVENT,0xabcd1240);
		hal_HstSendEvent(SYS_EVENT,BkgrdBuffer);
		hal_HstSendEvent(SYS_EVENT,BkgrdClipX1);
		hal_HstSendEvent(SYS_EVENT,BkgrdClipX2);
		hal_HstSendEvent(SYS_EVENT,BkgrdClipY1);
		hal_HstSendEvent(SYS_EVENT,BkgrdClipY2);
	hal_HstSendEvent(SYS_EVENT,0xabcd1241);
		hal_HstSendEvent(SYS_EVENT,BkgrdOffsetX);
		hal_HstSendEvent(SYS_EVENT,BkgrdOffsetY);
		hal_HstSendEvent(SYS_EVENT,BkgrdWidth);
		hal_HstSendEvent(SYS_EVENT,BkgrdHeight);
		hal_HstSendEvent(SYS_EVENT,ZoomWidth);
		hal_HstSendEvent(SYS_EVENT,ZoomHeight);
	hal_HstSendEvent(SYS_EVENT,0xabcd1242);
		hal_HstSendEvent(SYS_EVENT,DecMode);
		hal_HstSendEvent(SYS_EVENT,ImgWidth);
		hal_HstSendEvent(SYS_EVENT,ImgHeigh);

	hal_HstSendEvent(SYS_EVENT,0xabcd1243);
*/



	g_PngDecUserHandler=handle;

	HAL_ERR_T re_value=vpp_PngDecOpen((HAL_VOC_IRQ_HANDLER_T )PngDecVocISR,VPP_PNGDEC_WAKEUP_SW0);

    if (re_value != HAL_ERR_RESOURCE_BUSY)
    {

   		// ----------------
		// config
		// ----------------
		g_vppPngDecCfg.mode         =  0;
		g_vppPngDecCfg.reset        =  1;

		g_vppPngDecCfg.FileContent=FileContent;
		g_vppPngDecCfg.FileSize=FileSize;
		g_vppPngDecCfg.PNGOutBuffer=PNGOutBuffer;
		g_vppPngDecCfg.OutBufferSize=OutBufferSize;
		g_vppPngDecCfg.AlphaOutBuffer=AlphaOutBuffer;
		g_vppPngDecCfg.AlphaBufferSize=AlphaBufferSize;
		g_vppPngDecCfg.BkgrdBuffer=BkgrdBuffer;
		g_vppPngDecCfg.BkgrdClipX1=BkgrdClipX1;
		g_vppPngDecCfg.BkgrdClipX2=BkgrdClipX2;
		g_vppPngDecCfg.BkgrdClipY1=BkgrdClipY1;
		g_vppPngDecCfg.BkgrdClipY2=BkgrdClipY2;
		g_vppPngDecCfg.BkgrdOffsetX=BkgrdOffsetX;
		g_vppPngDecCfg.BkgrdOffsetY=BkgrdOffsetY;
		g_vppPngDecCfg.BkgrdWidth=BkgrdWidth;
		g_vppPngDecCfg.BkgrdHeight=BkgrdHeight;
		g_vppPngDecCfg.ZoomWidth=ZoomWidth;
		g_vppPngDecCfg.ZoomHeight=ZoomHeight;
		g_vppPngDecCfg.DecMode=DecMode;
		g_vppPngDecCfg.alpha_blending_to_transparentColor=alpha_blending_to_transparentColor;

		g_vppPngDecCfg.ImgWidth=ImgWidth;
		g_vppPngDecCfg.ImgHeigh=ImgHeigh;

		g_vppPngDecCfg.Window=WindowBuf;

		g_vppPngDecCfg.PngFileHandle=PngFileHandle;

		vpp_PngDecCfg(&g_vppPngDecCfg);

		vpp_PngDecSchedule();

		hal_GdbFlushCacheData();

		return HAL_ERR_NO;

    	}

	return re_value;

}


//0 is decoding .1 is decoded
BOOL MCI_PngdecGetDecodingStatus(MCI_PNGDEC_STATUS_T *status_png)
{

	VPP_PNGDEC_STATUS_T  vpp_Status;

	vpp_PngDecStatus(&vpp_Status);


	status_png->mode=vpp_Status.mode;
	status_png->errorStatus=vpp_Status.errorStatus;
	status_png->PngdecFinished=vpp_Status.PngdecFinished;


	if(status_png->PngdecFinished==1)
	{
        	vpp_PngDecClose();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif


