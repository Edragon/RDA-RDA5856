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


#include "aud_main.h"
#include "bt.h"
#include "a2dp.h"
#include "event.h"
#include "mci.h"
#include "bal_resource.h"
#ifndef WIN32
#include "hal_voc.h"
#include "dm_audio.h"
#include "hal_rda_abb.h"
#include "hal_rda_audio.h"
#endif

#include "hal_debug.h"

#if APP_SUPPORT_FM
#include "fmd_m.h"
#endif

#define AUD_FULL_PROGRESS_CNT (9999)
HANDLE g_audioSem = 0;

static UINT32 aud_result;
static UINT32 aud_value;

INT32  g_current_fd = -1;

static media_status_t g_media_status = {0};
#ifdef SERVICE_BASE_FS_SUPPORT
static UINT32 g_play_start_pos = 0;
static UINT8  g_media_mode = 0;
static mci_type_enum g_currentType = 0;
#endif

#if APP_SUPPORT_FADE_INOUT==1
#define MEDIA_FADE_SLOW_STEPS        5
#define MEDIA_FADE_FAST_STEPS        3

static UINT8 g_fade_timer = 0;
static INT8 g_fade_volume = 0;
static INT8 g_fade_step = 0;
//The cbk function always will be call
static FuncPtr g_fade_cbk_func_ptr = NULL;
#endif
extern UINT8 g_cur_volume;

void AudStopInternalAudio(void);
UINT32 AudPlayInternalAudio(UINT8 audio_id, UINT8 times);
UINT32 MCI_AudioRecordGetTime(VOID);

//#define MediaSendAppEvent(e, p)     ApMessageSendEvent(MOD_APP, e, p)
//#define MediaSendMediaEvent(e, p)   ApMessageSendEvent(MOD_MULTI, e, p)

/*************************************************************************
* FUNCTION
*   aud_set_result
*
* DESCRIPTION
*   This function is to set the result of audio request.
*
* PARAMETERS
*   kal_int32 result
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID aud_set_result(UINT32 result)
{
    aud_result=result;
}

UINT32 aud_get_result(VOID)
{
  return aud_result; 
}

BOOL media_is_bt_playing()
{
	if(g_media_status.status == PLAYING_STOP || g_media_status.status == PLAYING_REACH_END || g_media_status.status == PLAYING_ERROR)
		return 0;
	else 
		return 1;
}

BOOL no_play_inter()
{
	return (!(g_media_status.status == PLAYING_INTERNAL));
}

/*************************************************************************
* FUNCTION
*   aud_set_pair_results
*
* DESCRIPTION
*   This function is to set the result and the value of audio request.
*
* PARAMETERS
*   kal_int32 result
*   kal_uint32 value
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID aud_set_pair_results(UINT32 result, UINT32 value)
{
    aud_result = result;
    aud_value = value;
}

void MediaSendAppEvent(UINT32 event, UINT32 param)
{
    COS_EVENT ev = {event, param};
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#ifdef SERVICE_BASE_FS_SUPPORT
static void file_play_finished( MCI_ERR_T result )
{
    MULTI_TRACE(MULTI_AUD_TRC,"file_play_finished:g_current_fd=%d,g_media_status.status=%d,result;%d\r\n",g_current_fd,g_media_status.status,result);
    hal_HstSendEvent(SYS_EVENT,0x11111111);
    if((g_current_fd >= 0)&&(PLAYING_PLAYING == g_media_status.status))
    {
        MULTI_TRACE(MULTI_AUD_TRC,"file_play_finished!\r\n");
        FS_Close(g_current_fd);
        g_current_fd = -1;

        if((MCI_ERR_INVALID_FORMAT==result) || (PLAYING_ERROR == result))
        {
            g_media_status.status= PLAYING_ERROR;
        }
        else
        {
            g_media_status.status = PLAYING_REACH_END;
        }

        MediaSendAppEvent(EV_AUDIO_PLAY_FINISH_IND, g_media_status.status);
    }
}
#endif

#if APP_SUPPORT_RECORD==1
static void record_callback(MCI_ERR_T result)
{
    g_media_status.status = RECORDING_DISK_ERROR;

    MediaSendAppEvent(EV_AUDIO_RECORD_FINISH_IND, g_media_status.status);

    hal_HstSendEvent(SYS_EVENT,0x22222222);
}
#endif

#if APP_SUPPORT_FADE_INOUT==1
extern BOOL AudSendFadeInOutReq(BOOL fade_in, fade_t type, FuncPtr cbk);
#if APP_SUPPORT_FADE_INOUT==1
static void mediaFadeoutFinishCbk(void)
{
    AUD_SET_EVENT();
}

/******************************************************************************
 * AudSendFadeInOutReq: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL AudSendFadeInOutReq(BOOL fade_in, fade_t type, FuncPtr cbk)
{
	AUD_FADE_INOUT_REQ_MSG_PTR msg = (AUD_FADE_INOUT_REQ_MSG_PTR) COS_MALLOC(sizeof(AUD_FADE_INOUT_REQ_MSG_T));

	memset(msg, 0x0, sizeof(AUD_FADE_INOUT_REQ_MSG_T));

	msg->fade_in = fade_in;
	msg ->type = type;

	if(fade_in)
	{
		msg ->cbk = cbk;
		COS_SendMessageEvent(MOD_MULTI, EV_AUDIO_FADE_INOUT_REQ, msg);
        
	}
	else
	{
		//Fade out will block Caller task, so callback is useless
		msg ->cbk = mediaFadeoutFinishCbk;
	    AUD_INIT_EVENT();
		COS_SendMessageEvent(MOD_MULTI, EV_AUDIO_FADE_INOUT_REQ, msg);
	    AUD_WAIT_EVENT();//Wait until fade out finished
	}
}

#endif

/******************************************************************************
 * AudMediaFadeRest:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void AudMediaFadeRest(void)
{
    MULTI_TRACE(MULTI_AUD_TRC, "AudMediaFadeRest");

    if(0 != g_fade_timer)
    {
        Multi_KillTimer(g_fade_timer);
        g_fade_timer = 0;
    }

    g_fade_volume = 0;
    g_fade_step = 0;

    if(NULL != g_fade_cbk_func_ptr)
    {
        MULTI_TRACE(MULTI_AUD_TRC, "AudMediaFadeRest cbk=0x%x", g_fade_cbk_func_ptr);
        g_fade_cbk_func_ptr();
        g_fade_cbk_func_ptr = NULL;
    }
}

/******************************************************************************
 * AudMediaFadeFinished:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void AudMediaFadeFinished(void)
{
    AudMediaFadeRest();
    MediaSendAppEvent(EV_AUDIO_FADE_INOUT_FINISH_IND, 0);
}

/******************************************************************************
 * AudMediaFadeTimeout:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void AudMediaFadeTimeout(void *param)
{
    MULTI_TRACE(MULTI_AUD_TRC, "AudMediaFadeTimeout volume=%d,step=%d",
                g_fade_volume, g_fade_step);

    Multi_ChangeTimer(g_fade_timer, 30);
    g_fade_volume += g_fade_step;

    if(g_fade_step>0)
    {
        SetPAVolume(MIN(g_fade_volume , g_media_status.volume));
        if(g_fade_volume >= g_media_status.volume)
        {
            AudMediaFadeFinished();
        }
    }
    else
    {
        SetPAVolume(MAX(g_fade_volume , 0));
        if(g_fade_volume <= 0)
        {
            AudMediaFadeFinished();
        }
    }

}

/******************************************************************************
 * AudFadeInOutHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void AudFadeInOutHandler(AUD_FADE_INOUT_REQ_MSG_PTR msg)
{
    UINT8 start_vol, end_vol;

    APP_ASSERT(msg);

    MULTI_TRACE(MULTI_AUD_TRC, "AudFadeInOutHandler fade_in=%d, type=%d, cbk=0x%x",
                msg->fade_in, msg->type, msg->cbk);

    //1.Stop previous fade schedule if it's present
    AudMediaFadeRest();

    //2.Prepare param
    if(msg->fade_in)
    {
#if APP_SUPPORT_DELAY_VOLUME==1
        start_vol = 0;  //start_vol
#else
        start_vol = 1;  //start_vol
#endif
        end_vol = g_media_status.volume;
    }
    else
    {
        start_vol = g_cur_volume;   //start_vol
        end_vol = 0;
    }

    g_fade_cbk_func_ptr = msg->cbk;

    //3. If no fade, call finished directly
    if(no_fade == msg->type)
    {
        SetPAVolume(end_vol);
        AudMediaFadeFinished();
        return;
    }
    else if(msg->type == fast_fade)
    {
        g_fade_step = abs(start_vol - end_vol)/MEDIA_FADE_FAST_STEPS;
    }
    else /*slow_fade*/
    {
        g_fade_step = abs(start_vol - end_vol)/MEDIA_FADE_SLOW_STEPS;
    }


    if(g_fade_step == 0)
    {
        g_fade_step = 1;
    }
    else
    {
        APP_ASSERT(g_fade_step > 0); //Should NOT less than 0
    }

    //4.Start fade in/out timer
    MULTI_TRACE(MULTI_AUD_TRC, "AudFadeInOut StartTimer start_vol=%d, end_vol=%d, g_fade_step=%d, cbk=0x%x",
                start_vol, end_vol, g_fade_step, g_fade_cbk_func_ptr);

    g_fade_volume = start_vol;
    if(msg->fade_in)
    {
        SetPAVolume(start_vol);
        g_fade_timer = Multi_SetTimer(1000, AudMediaFadeTimeout, NULL, MULTI_TIMER_MODE_PERIODIC);
    }
    else
    {
        g_fade_step *= (-1);

        if(start_vol>2)
        {
            g_fade_timer = Multi_SetTimer(100, AudMediaFadeTimeout, NULL, MULTI_TIMER_MODE_PERIODIC);
        }
        else
        {
            SetPAVolume(end_vol);
            AudMediaFadeFinished();
        }
    }
}


#endif

#if APP_SUPPORT_BLUETOOTH==1
/******************************************************************************
 * AudFMCommandHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL AudBTCommandHandler(UINT8 cmd, UINT32 param)
{
    BOOL ret = TRUE;
    MULTI_TRACE(MULTI_AUD_TRC, "AudBTCommandHandler cmd=0x%x, param=0x%x", cmd, param);

    switch(cmd)
    {
        case MC_PLAY_A2DP:
            AudStopInternalAudio();

            ret= MCI_PlayBTStream((bt_a2dp_audio_cap_struct*)param);
            if(MCI_ERR_NO == ret)
            {
                g_media_status.status=PLAYING_PLAYING;
            }
            break;
#if 1            
        case MC_PLAY_SCO:
            AudStopInternalAudio();

            ret= MCI_StartBTSCO((UINT16)param);
            if(MCI_ERR_NO == ret)
            {
                g_media_status.status=PLAYING_PLAYING;
            }
            break;
        case MC_STOP:
            hal_HstSendEvent(1, 0x20161452);
            AudStopInternalAudio();
            ret= MCI_AudioStop();
            g_media_status.status=PLAYING_STOP;
       		break;   
		
        case MC_SETVOLUME:
            if(g_media_status.status != PLAYING_INTERNAL)
            {
                ret= SetPAVolume(param);
            }
            else
            {
                g_cur_volume = param;
            }
		    break;   
	     case MC_SETEQ:
	    {
		    g_media_status.eq = param;
		    MCI_AudioSetEQ(param);
	     }
		    break;
#endif            
    }

    return ret;
}

#endif // APP_SUPPORT_BLUETOOTH==1


#if (1 == APP_SUPPORT_FM)

/******************************************************************************
 * AudFMCommandHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL AudFMCommandHandler(UINT8 cmd, UINT32 param)
{
    BOOL ret = TRUE;
    FMD_ERR_T error;

    MULTI_TRACE(MULTI_AUD_TRC, "AudFMCommandHandler cmd=0x%x, param=0x%x", cmd, param);

    switch(cmd)
    {
        case MC_OPEN:
            AudStopInternalAudio();
            error = fmd_Open((FMD_BAND_T) param, NULL, NULL);

            MULTI_TRACE(MULTI_AUD_TRC, "AudFMCommandHandler MC_OPEN error=%d",   error);

            if(error != FMD_ERR_NO)
            {
                aud_set_pair_results(FALSE, error);
                break;
            }
            if(FMD_ERR_NO != MCI_FmPlay())
            {
                fmd_Close();
                aud_set_pair_results(FALSE, error);
                break;
            }
            g_media_status.status = PLAYING_FM;

            break;

        case MC_PLAY:
            fmd_Tune(param);
            break;

        case MC_SEARCH:
            ret = fmd_ValidStop(param);

            aud_set_result(ret);
            break;

        case MC_GETSTATUS:
        {
            AUD_FM_REQ_STATUS_MSG_PTR fm_status_ptr = (AUD_FM_REQ_STATUS_MSG_PTR) param;

            APP_ASSERT(fm_status_ptr);

            fmd_GetFreq(&fm_status_ptr->freq);
            fmd_GetStereoStatus(&fm_status_ptr->stereo_status);
            fmd_GetRssi(&fm_status_ptr->rssi);
        }
        break;

        case MC_MUTE:            //¾²Òô
            SetPAVolume(0);
            fmd_SetVolume(FMD_ANA_MUTE, FALSE, TRUE);
            break;

        case MC_SETVOLUME:
            fmd_SetVolume(ap_convert_volume(AUD_MAX_LEVEL, FMD_ANA_VOL_QTY - 1, (UINT16) param), FALSE, FALSE);
            SetPAVolume(param);
            break;

        case MC_CLOSE:
            if(!param)
            {
                if(PLAYING_FM == g_media_status.status)
                    MCI_FmStop();
            }
            fmd_Close();
            g_media_status.status = PLAYING_STOP;
            break;

        default:
            return FALSE;
    }

    return TRUE;
}
#endif

#if (1 == APP_SUPPORT_LINEIN)

/******************************************************************************
 * AudLineInCommandHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL AudLineInCommandHandler(UINT8 cmd, UINT32 param)
{
    MULTI_TRACE(MULTI_AUD_TRC, "AudLineInCommandHandler cmd=0x%x, param=0x%x", cmd, param);

    switch(cmd)
    {
        case MC_OPEN:
            AudStopInternalAudio();
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
#ifndef WIN32
            hal_abbLineInMode(TRUE);
#endif
#else
		    MCI_LineinPlay(0);
#endif
            g_media_status.status = PLAYING_LINEIN;
            break;


        case MC_MUTE:
            break;

        case MC_SETVOLUME:
#ifndef WIN32            
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#ifndef WIN32
			hal_AudSetDacAnalogVolume(param);
#endif
#else
            SetPAVolume(param);
#endif
#endif
            break;
            
        case MC_CLOSE:
 #if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856) 
#ifndef WIN32
            hal_abbLineInMode(FALSE);
#endif 
#else
        	MCI_LineStop();
 #endif
            g_media_status.status = PLAYING_STOP;
            break;

        default:
            return FALSE;
    }

    return TRUE;
}
#endif

/******************************************************************************
 * AudCommandHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
#ifdef SERVICE_BASE_FS_SUPPORT
static BOOL AudCommandHandler(UINT8 cmd, UINT32 param)
{
    UINT32 res;
    if(cmd!=0x80 && cmd!=0x83)
        hal_HstSendEvent(SYS_EVENT,0x82CED000 + cmd);

    switch (cmd)
    {
        case MC_OPEN:
        {
            Open_param_t *pParam = (Open_param_t*)param;
            if(g_current_fd >= 0)
            {
                FS_Close(g_current_fd);
            }
            if(pParam->mode == MEDIA_PLAY)
            {
                g_current_fd = FS_OpenDirect(pParam->file_entry, FS_O_RDONLY, 0);
            }
            else
            {
#if APP_SUPPORT_RECORD==1
                WCHAR filename[12];
                filename[0] = pParam->file_name[0];
                filename[1] = pParam->file_name[1];
                filename[2] = pParam->file_name[2];
                filename[3] = pParam->file_name[3] ;
                filename[4] = pParam->file_name[4] ;
                filename[5] = pParam->file_name[5] ;
                filename[6] = '.';
                filename[7] = pParam->file_name[8];
                filename[8] = pParam->file_name[9];
                filename[9] = pParam->file_name[10];
                filename[10] = 0;
                if(pParam->mode == MEDIA_RECORD)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x19880203);
                    g_current_fd = FS_Open((PCSTR)filename, FS_O_RDWR|FS_O_TRUNC|FS_O_CREAT, 0);
                    #ifdef APP_RECORD_VOICE_INRAM
                    /* Any value greater than 0 is ok, just bypass error check */
                    g_current_fd = 1; 
                    #endif
                }
                else if(pParam->mode == MEDIA_PLAYREC)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x19880204);
                    g_current_fd = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
                }
                else
#endif
                {
                    hal_HstSendEvent(SYS_EVENT, 0x19880205);
                    aud_set_result(FALSE);
                    break;
                }
            }
            g_media_mode = pParam->mode;

            hal_HstSendEvent(SYS_EVENT, 0x19880210 + pParam->type);
            switch(pParam->type)
            {
                case Music_MP3:
                    g_currentType = MCI_TYPE_DAF;
                    break;
                case Music_AAC:
                    g_currentType = MCI_TYPE_AAC;
                    break;
                case Music_WAV:
                    g_currentType = MCI_TYPE_WAV;
                    break;
                case Music_SBC:
                    g_currentType = MCI_TYPE_SBC;
                    break;
#ifndef TARGET_WMA_SUPPORT_OFF
                case Music_WMA:
                    g_currentType = MCI_TYPE_WMA;
                    break;
#endif
                
                default:
                    g_currentType = 0;
                    aud_set_result(FALSE); // unsupport file type
                    return TRUE;
            }
            if(g_media_status.status==PLAYING_INTERNAL)
                AudStopInternalAudio();

            if(g_media_mode == MEDIA_PLAY)
            {
                g_media_status.status = PLAYING_STOP;
            }
            else
            {
                g_media_status.status = RECORDING_STOP;
            }
            if(g_current_fd < 0)
            {
                hal_HstSendEvent(SYS_EVENT, 0x19880220);
                aud_set_result(FALSE);
                break;
            }
            break;
        }

        case MC_GETFILEINFO:
            if(g_current_fd < 0)
            {
                aud_set_result(FALSE);
            }
            else
            {
                AudDesInfoStruct *pInfo = (AudDesInfoStruct*)param;

                APP_ASSERT(pInfo);

                memset(pInfo, 0, sizeof(AudDesInfoStruct));
                MCI_AudioGetFileInformation(g_current_fd, pInfo,g_currentType);
                //Audio_GetDetailsInfo(g_current_fd, pInfo,g_currentType);
                g_media_status.total_time =  pInfo->time;
            }
            break;

        case MC_GETTOTALTIME:
            if(g_current_fd < 0)
            {
                aud_set_result(FALSE);
            }
            else
            {
                MCI_ProgressInf time;
                //INT32 sec;
                UINT32 *pTime = (UINT32*)param;
                MCI_AudioGetDurationTime(g_current_fd, g_currentType, 0, 10000, &time);
                *pTime = g_media_status.total_time =  time.DurationTime;
            }
            break;

        case MC_PLAY:
        {
            //UINT32 totaltime=0,progress =0;
#ifdef LINEIN_RECORDER_SUPPORT
            g_currentType = MCI_TYPE_LINEIN_BT;
            g_current_fd = 1;
#endif

            if(g_current_fd < 0)
            {
                aud_set_result(FALSE);
                break;
            }
            hal_HstSendEvent(SYS_EVENT,g_media_status.status);

            MULTI_TRACE(MULTI_AUD_TRC,"[MC_PLAY]status=%d,param=%d\r\n",g_media_status.status,param);
            AudStopInternalAudio();

            if (g_media_status.status == PLAYING_ERROR)
            {
                aud_set_result(FALSE);
                break;
            }

            if(g_media_status.status == PLAYING_PAUSE)
            {
                MCI_AudioResume(g_current_fd);
                g_media_status.status = PLAYING_PLAYING;
                aud_set_result(TRUE);
                break;
            }
#if APP_SUPPORT_FADE_INOUT==1
            AudMediaFadeRest();
#if APP_SUPPORT_DELAY_VOLUME==1
            SetPAVolume(0);
#else
            SetPAVolume(1);
#endif
            AudSendFadeInOutReq(TRUE, slow_fade, NULL);

#endif

            if(g_currentType)
            {
                g_play_start_pos = param;
#ifdef LINEIN_RECORDER_SUPPORT
                res = MCI_AudioPlay(0,1,MCI_TYPE_LINEIN,NULL, param);
#else
                res = MCI_AudioPlay(0,g_current_fd,g_currentType,file_play_finished, param);
#endif
            }
            else
                res = MCI_ERR_BAD_FORMAT;

            hal_HstSendEvent(SYS_EVENT,res);
            hal_HstSendEvent(SYS_EVENT, 0x19880300);
            g_media_status.status = PLAYING_PLAYING;
            if(res != MCI_ERR_NO)
            {
                hal_HstSendEvent(SYS_EVENT, 0x19880301);
                hal_HstSendEvent(SYS_EVENT, 0x19880400+res);
                file_play_finished(PLAYING_ERROR);
                aud_set_pair_results(FALSE, res);
                break;
            }

            MCI_AudioSetEQ(g_media_status.eq);
            

            break;
        }


#if APP_SUPPORT_RECORD==1
        case MC_RECORD:
        {
            if(g_current_fd < 0)
            {
                aud_set_result(FALSE);
                break;
            }
            AudStopInternalAudio();

            if(g_media_status.status == RECORDING_PAUSE)
            {
                res = MCI_AudioRecordResume();
            }
            else
            {
                res = MCI_AudioRecordStart(g_current_fd, MCI_TYPE_WAV_DVI_ADPCM, 7, 8000,record_callback, NULL);//g_file_type
            }
            if(res == MCI_ERR_NO)
            {
                g_media_status.status = RECORDING_RECORDING;
                aud_set_result(TRUE);
            }
            else
            {
                g_media_status.status = RECORDING_EXCEPTION;
                aud_set_pair_results(FALSE, res);
            }
            break;
        }
#endif


#if APP_SUPPORT_USBAUDIO==1
        case MC_PLAY_USB:
            res= MCI_AudioPlay(0, 0, MCI_TYPE_USB, NULL, 0);
            if(res == MCI_ERR_NO)
            {
                g_media_status.status = PLAYING_PLAYING;
                aud_set_result(TRUE);
            }
            else
            {
                g_media_status.status = PLAYING_ERROR;
                aud_set_pair_results(FALSE, res);
            }
            break;
#endif
        case MC_SETEQ:
        {
            //if(g_media_status.status == PLAYING_PLAYING)
            hal_HstSendEvent(SYS_EVENT, 0x20161454);
            hal_HstSendEvent(SYS_EVENT, param);
            g_media_status.eq = param;
            MCI_AudioSetEQ(param);
            break;
        }

        case MC_PAUSE:
        {
            if(g_media_status.status == PLAYING_PLAYING)
            {
                MCI_PlayInf audiofile_lastPlayInfo ;
                MCI_AudioPause();
                MCI_AudioGetPlayInformation(&audiofile_lastPlayInfo);
                g_media_status.progress=audiofile_lastPlayInfo.PlayProgress;
                g_media_status.status = PLAYING_PAUSE;
            }
#if APP_SUPPORT_RECORD==1
            else  if(g_media_status.status == RECORDING_RECORDING)
            {
                MCI_AudioRecordPause();
                g_media_status.status = RECORDING_PAUSE;
            }
#endif
            break;
        }

        case MC_STOP:
        {
            hal_HstSendEvent(SYS_EVENT,g_media_status.status);
            if(g_media_status.status == PLAYING_PLAYING||g_media_status.status == PLAYING_REACH_END)
            {
                hal_HstSendEvent(SYS_EVENT, 0x19880033);
                AudStopInternalAudio();
                MCI_AudioStop();
                g_media_status.status = PLAYING_STOP;
            }
            else if(g_media_status.status == RECORDING_RECORDING||g_media_status.status ==RECORDING_PAUSE)
            {
                MCI_AudioRecordStop();
                g_media_status.status = RECORDING_STOP;
            }
            break;
        }


        case MC_CLOSE:
        {
            if(g_current_fd>=0)
            {
                FS_Close(g_current_fd);
                g_current_fd = -1;
            }
            break;
        }

        case MC_SETVOLUME:
        {
            g_media_status.volume = param;
            SetPAVolume(param);
            break;
        }

        case MC_GETTIME:
        {
            UINT32 *pTime = (UINT32*)param;
            if(pTime == NULL)
            {
                aud_set_result(FALSE);
                break;
            }

            if(PLAYING_PLAYING==g_media_status.status)
            {
                MCI_PlayInf playInfo;

                MCI_AudioGetPlayInformation(&playInfo);
                g_media_status.progress = playInfo.PlayProgress;
                *pTime = ((g_media_status.total_time / 100 *  g_media_status.progress) / 100) + 100;
            }
#if APP_SUPPORT_RECORD==1
            else if(g_media_status.status == RECORDING_RECORDING||g_media_status.status ==RECORDING_PAUSE)
            {
                *pTime = MCI_AudioRecordGetTime()+100;
            }
#endif
            break;
        }

        case MC_GETSTATUS:
        {
            if(param)
            {
                MCI_PlayInf playInfo;
                media_status_t *pStatus = (media_status_t*) param;

                if(g_media_mode==MEDIA_PLAY)
                {
                    MCI_AudioGetPlayInformation(&playInfo);
                    g_media_status.progress = playInfo.PlayProgress;
                }
                *pStatus = g_media_status;
            }
            break;
        }

        case MC_GETSTARTPOS:
        {
            UINT32 *pPos = (UINT32*)param;
            *pPos = g_play_start_pos;
        }
        break;
		
#ifdef MUSIC_BAND_INFO
		case MC_GETFREQBAND:
		{
			if(param)
			{
				MCI_AudioGetFreqBandInfo((uint16 *)param);
			}

		}
		break;	
#endif
        default:
        {
            MULTI_TRACE(MULTI_AUD_TRC,"unknow command:%d,%d.\r\n",cmd,param);
            return FALSE;
        }
    }
    return TRUE;
}
#endif

/******************************************************************************
 * AudCmdReqDispatcher:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL AudCmdReqDispatcher(AUD_CMD_REQ_MSG_PTR msg_ptr)
{
    MULTI_TRACE(MULTI_AUD_TRC, "AudCmdReqDispatcher  mode=%d, cmd=0x%x, param=0x%x",
                msg_ptr->mode, msg_ptr->cmd, msg_ptr->cmd_param);
        hal_HstSendEvent(1, 0x20161450);
        hal_HstSendEvent(1, msg_ptr->mode);
        hal_HstSendEvent(1, msg_ptr->cmd);
    if(AUD_INTERNAL == msg_ptr->mode)
    {
        switch(msg_ptr->cmd)
        {
            case MC_PLAY:
                AudPlayInternalAudio(AUD_INTERNAL_PLAY_REQ_GET_AUDIOID(msg_ptr->cmd_param),
                                     AUD_INTERNAL_PLAY_REQ_GET_TIMES(msg_ptr->cmd_param));
                break;
            case MC_STOP:
                AudStopInternalAudio();
                break;
        }
    }
#if (1 == APP_SUPPORT_FM)
    else if(AUD_FM == msg_ptr->mode)
    {
        AudFMCommandHandler(msg_ptr->cmd, msg_ptr->cmd_param);
    }
#endif
#if APP_SUPPORT_BLUETOOTH==1
    else if(AUD_BT == msg_ptr->mode)
    {
        hal_HstSendEvent(1, 0x20161451);
        AudBTCommandHandler(msg_ptr->cmd, msg_ptr->cmd_param);
    }
#endif
#if (1 == APP_SUPPORT_LINEIN)
    else if(AUD_LINEIN == msg_ptr->mode)
    {
        AudLineInCommandHandler(msg_ptr->cmd, msg_ptr->cmd_param);
    }
#endif
#ifdef SERVICE_BASE_FS_SUPPORT
    else
    {
        AudCommandHandler(msg_ptr->cmd, msg_ptr->cmd_param);
    }
#endif
    return TRUE;
}

static void Aud_InternalAudioPlayFinished(MCI_ERR_T result)
{
    hal_HstSendEvent(SYS_EVENT,0x22221111);
    hal_HstSendEvent(SYS_EVENT,g_media_status.status);
    if(PLAYING_INTERNAL == g_media_status.status )
    {
        g_media_status.status = PLAYING_STOP;
        //MCI_StopInternalAudio();
    	DM_SetAudioVolume(g_cur_volume);
        MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, result);
    }
}


void AudStopInternalAudio(void)
{
    if(PLAYING_INTERNAL == g_media_status.status )
    {
        g_media_status.status = PLAYING_STOP;
        MCI_StopInternalAudio();
    	DM_SetAudioVolume(g_cur_volume);
        MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND, g_media_status.interaudioid);
    }
}

UINT32 AudPlayInternalAudio(UINT8 audio_id, UINT8 times)
{
    UINT8 *data = NULL;
    UINT8 type;
    UINT32 length;
    UINT32 ret;

    data = gui_get_audio(audio_id, &type, &length);

    MULTI_TRACE(MULTI_AUD_TRC, "Enter AudPlayInternalAudio, audio_id = %d, g_media_status.status = %d, type = %d, data = %x",
                audio_id, g_media_status.status,type,data);

    AudStopInternalAudio();

    if(data && (times > 0))
    {
        hal_HstSendEvent(SYS_EVENT, 0x19885001);
        hal_HstSendEvent(SYS_EVENT, g_media_status.status);
        if((g_media_status.status == PLAYING_STOP) || (g_media_status.status == PLAYING_ERROR) || (g_media_status.status == RECORDING_STOP))
        {
#ifdef EXTERNAL_PA_SUPPORT
            gpio_SetMute(FALSE);
            COS_Sleep(50);
#endif

            // TODO:  Fix me! What's the value of volume
            //SetPAVolume(3);
            DM_SetAudioVolume(6);
            COS_Sleep(50);

            g_media_status.status = PLAYING_INTERNAL;
            ret = MCI_PlayInternalAudio(type, length, data, Aud_InternalAudioPlayFinished, times);

            if(ret != MCI_ERR_NO)
            {
                Aud_InternalAudioPlayFinished(ret);
            }
            else
            {
                    g_media_status.interaudioid = audio_id;
            }
        }
        else if(g_media_status.status == PLAYING_PLAYING && type==1)
        {
            hal_HstSendEvent(SYS_EVENT, 0x19885002);
            hal_HstSendEvent(SYS_EVENT, length);
            hal_HstSendEvent(SYS_EVENT, (UINT32)data);
            hal_HstSendEvent(SYS_EVENT, times);
            MCI_PlaySideTone(length, data, times);
            MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, MCI_ERR_NO); 
        }
        else
        {
            MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, MCI_ERR_BUSY); 
        }
    }
    else
    {
        MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, MCI_ERR_INVALID_PARAMETER); 
    }
    return 0;
}

#ifdef KALAOK_SUPPORT

void AudStopMicroPhoneAudio(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x19895003);
    hal_HstSendEvent(SYS_EVENT, g_media_status.status);

    if(PLAYING_MICROPHONE== g_media_status.status )
    {
        g_media_status.status = PLAYING_STOP;
        MCI_StopMicroPhoneAudio();
    	DM_SetAudioVolume(g_cur_volume);
        //MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND, g_media_status.interaudioid);
    }
}

void AudPlayMicroPhoneAudio(void)
{
	UINT8 *data = NULL;
	UINT32 length;
	UINT32 ret;


	MULTI_TRACE(MULTI_AUD_TRC, "Enter AudPlayMicroPhoneAudio,  g_media_status.status = %d",
	g_media_status.status);

	AudStopMicroPhoneAudio();

	hal_HstSendEvent(SYS_EVENT, 0x19895001);
	hal_HstSendEvent(SYS_EVENT, g_media_status.status);
	if((g_media_status.status == PLAYING_STOP) || (g_media_status.status == PLAYING_ERROR) || (g_media_status.status == RECORDING_STOP) || (g_media_status.status == RECORDING_PAUSE))
	{
#ifdef EXTERNAL_PA_SUPPORT
		gpio_SetMute(FALSE);
		COS_Sleep(50);
#endif

		// TODO:  Fix me! What's the value of volume
		//SetPAVolume(3);
		DM_SetAudioVolume(g_cur_volume);
		COS_Sleep(50);

		ret = MCI_PlayMicroPhoneAudio(0, NULL, 0);

		if(ret != MCI_ERR_NO)
		{
	            hal_HstSendEvent(SYS_EVENT, 0x19895002);
		}

	}
	else
	{
	            hal_HstSendEvent(SYS_EVENT, 0x19895007);
	}
}


#endif

#if APP_SUPPORT_BLUETOOTH==1
/******************************************************************************
 * aud_bt_main:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL aud_bt_main(MULTI_MSG_PTR ev)
{
	return FALSE;
}
#endif
/******************************************************************
 * FUNCTION
 *    aud_init
 * DESCRIPTION
 *    This function is used to init audio task.
 * PARAMETERS
 *
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *
 *
 ******************************************************************/
BOOL aud_init(void)
{

    return TRUE;
}

/******************************************************************
 * FUNCTION
 *    aud_isIdle
 * DESCRIPTION
 *    This function is used to get the status of the voc.
 * PARAMETERS
 *
 *
 * RETURNS
 *
 * GLOBALS AFFECTED
 *
 *
 ******************************************************************/
BOOL aud_isIdle(void)
{
#ifndef WIN32
#if (CHIP_ASIC_ID !=  CHIP_ASIC_ID_5857)
    return hal_VocIsClose();
#else
    return hal_ZspIsClose();
#endif
#else
    return TRUE;
#endif
}

/******************************************************************
* FUNCTION
*    aud_main
* DESCRIPTION
*    This function is main message dispatching function of audio module.
* PARAMETERS
*
*
* RETURNS
*
* GLOBALS AFFECTED
*
*
******************************************************************/
BOOL aud_main(MULTI_MSG_PTR ev)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if APP_SUPPORT_BLUETOOTH==1
    if (aud_bt_main(ev))
    {
        return TRUE;
    }
#endif

    switch( ev->nEventId)
    {
        case EV_AUDIO_CMD_REQ: /*7001*/
        {
            AUD_CMD_REQ_MSG_PTR ptr = (AUD_CMD_REQ_MSG_PTR) ev->nParam1;
            AudCmdReqDispatcher(ptr);
            AUD_SET_EVENT();
            COS_FREE(ptr);
        }
        break;

#if APP_SUPPORT_FADE_INOUT==1
        case EV_AUDIO_FADE_INOUT_REQ: /*7020*/
        {
            AUD_FADE_INOUT_REQ_MSG_PTR ptr = (AUD_FADE_INOUT_REQ_MSG_PTR) ev->nParam1;
            AudFadeInOutHandler(ptr);
            COS_FREE(ptr);
        }
        break;
#endif
		default:
			return FALSE;
    }

    return TRUE;

}

