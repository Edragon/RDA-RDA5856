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

#include "ap_common.h"
#include "ap_message.h"
#include "ap_music.h"
#include "ap_media.h"
#include "ap_mgr.h"
#if defined(CAMERA_SUPPORT)
extern  UINT32 cam_result;
static UINT32 cam_value;
#endif /* CAMERA_SUPPORT */
/*==== FUNCTIONS ===========*/

#ifdef MUSIC_BAND_INFO
#include"hal_gpio.h"

UINT8 g_TimerId = 0xFF;
UINT32 BandLevel1 = 1000;
UINT32 BandLevel2 = 3000;
UINT32 BandLevel3 = 6000;
UINT32 BandLevel4 = 12000;

UINT32 BandLevel_count = 0;
UINT32 BandLevel1_count = 0;
UINT32 BandLevel2_count = 0;
UINT32 BandLevel3_count = 0;
UINT32 BandLevel4_count = 0;
UINT32 BandLevel5_count = 0;

UINT32 BandMaxLevel = 0;
UINT32 BandAveLevel = 0;
UINT32 BandAveLevelCalu = 0;

void APP_DisplayFreqBand_TimeOutHandler(void * p)
{
	UINT16 freq[MUSIC_DEFAULT_BAND];
       static UINT16 count = 0;
	UINT32 BandValue = 0;
		
	memset(&freq[0], 0, MUSIC_DEFAULT_BAND * 2);
	mediaSendCommand(MC_GETFREQBAND, &freq[0]);

	HAL_APO_ID_T gpioled0, gpioled1, gpioled2, gpioled3;
	gpioled0.gpioId = HAL_GPIO_1_0;
	gpioled1.gpioId = HAL_GPIO_1_1;
	gpioled2.gpioId = HAL_GPIO_1_3;
	gpioled3.gpioId = HAL_GPIO_1_4;

       BandValue = freq[0] + freq[1] + freq[2] + freq[3] + freq[4] + freq[5] + freq[6] + freq[7];
       BandAveLevelCalu += BandValue;

       if(count++%256 == 0)
       {
           BandAveLevel = BandAveLevelCalu>> 8;
	    BandAveLevelCalu = 0;
	}
	if(BandMaxLevel < BandValue)
	{
		BandMaxLevel = BandValue;
	}
#if 1
	hal_HstSendEvent(SYS_EVENT, 0x44444444);
	//hal_HstSendEvent(SYS_EVENT, freq[0]);
	//hal_HstSendEvent(SYS_EVENT, freq[1]);
	//hal_HstSendEvent(SYS_EVENT, freq[2]);
	//hal_HstSendEvent(SYS_EVENT, freq[3]);
	//hal_HstSendEvent(SYS_EVENT, freq[4]);
	//hal_HstSendEvent(SYS_EVENT, freq[5]);
	//hal_HstSendEvent(SYS_EVENT, freq[6]);
	//hal_HstSendEvent(SYS_EVENT, freq[7]);
	
       hal_HstSendEvent(SYS_EVENT,BandValue);
	hal_HstSendEvent(SYS_EVENT,BandAveLevel);	
	hal_HstSendEvent(SYS_EVENT,BandMaxLevel);


#endif	

       if(BandLevel_count++ >= 600)
       {
		BandLevel_count = 0;
		BandLevel1_count = 0;
		BandLevel2_count = 0;
		BandLevel3_count = 0;
		BandLevel4_count = 0;
	}

	if(BandValue <= BandLevel1)
	{
		hal_GpioClr(gpioled0);
		hal_GpioClr(gpioled1);
		hal_GpioClr(gpioled2);
		hal_GpioClr(gpioled3);
		BandLevel1_count++;
	}
	else if(BandValue <= BandLevel2)
	{
		hal_GpioClr(gpioled0);
		hal_GpioClr(gpioled1);
		hal_GpioClr(gpioled2);
		hal_GpioSet(gpioled3);
		BandLevel2_count++;

	}
	else if(BandValue <= BandLevel3)
	{
		hal_GpioClr(gpioled0);
		hal_GpioClr(gpioled1);
		hal_GpioSet(gpioled2);
		hal_GpioSet(gpioled3);
		BandLevel3_count++;

	}	
	else if(BandValue <= BandLevel4)
	{
		hal_GpioClr(gpioled0);
		hal_GpioSet(gpioled1);
		hal_GpioSet(gpioled2);
		hal_GpioSet(gpioled3);
		BandLevel4_count++;

	}
	else
	{
		hal_GpioSet(gpioled0);
		hal_GpioSet(gpioled1);
		hal_GpioSet(gpioled2);
		hal_GpioSet(gpioled3);
		BandLevel5_count++;
	}
	if(BandLevel_count%32 == 0)
	{
		//hal_HstSendEvent(SYS_EVENT, 0x33333333);

		//hal_HstSendEvent(SYS_EVENT,BandLevel_count);
		//hal_HstSendEvent(SYS_EVENT,BandLevel1_count);	
		//hal_HstSendEvent(SYS_EVENT,BandLevel2_count);	
		//hal_HstSendEvent(SYS_EVENT,BandLevel3_count);	
		//hal_HstSendEvent(SYS_EVENT,BandLevel4_count);
		//hal_HstSendEvent(SYS_EVENT,BandLevel5_count);
	}
}
#endif

UINT32 AudSendCommandReq(AUD_REQ_MODE mode, UINT8 cmd, UINT32 param)
{
    AUD_CMD_REQ_MSG_PTR cmd_req_ptr = NULL;

    cmd_req_ptr = (AUD_CMD_REQ_MSG_PTR) COS_MALLOC(sizeof(AUD_CMD_REQ_MSG_T));
    memset(cmd_req_ptr, 0x0, sizeof(AUD_CMD_REQ_MSG_T));
    
    cmd_req_ptr->mode = mode;
    cmd_req_ptr->cmd = cmd;
    cmd_req_ptr->cmd_param = param;

    app_trace(APP_MEDIA_TRC, "AudSendCommandReq(%d) mode=%d, cmd=0x%x, param=0x%x", 
        COS_GetCurrentTaskModId(), mode, cmd, param);
        
    //Called by app task only!!
    //APP_ASSERT(MOD_APP == Cos_GetCurrentTaskId());

    aud_set_result(TRUE); 
    AUD_INIT_EVENT();
	ApMessageSendEvent(MOD_MULTI, EV_AUDIO_CMD_REQ, (UINT32) cmd_req_ptr);
    AUD_WAIT_EVENT();

    //Please get a better way to trace the result
    //app_trace(APP_MEDIA_TRC, "AudSendCommandReq result=0x%x, value=0x%x", aud_result, aud_value);
    return aud_get_result();

}

#if APP_SUPPORT_LINEIN
BOOL mediaSendLineInCommand(UINT8 cmd, UINT32 param)
{
#ifdef WIN32
       return TRUE;
#endif
	return (BOOL) AudSendCommandReq(AUD_LINEIN, cmd, param);
}
#endif



#if APP_SUPPORT_FM
BOOL mediaSendFMCommand(UINT8 cmd, UINT32 param)
{
#ifdef WIN32
       return TRUE;
#endif
	return (BOOL) AudSendCommandReq(AUD_FM, cmd, param);
}
#endif

#if APP_SUPPORT_BLUETOOTH
BOOL mediaSendBTCommand(UINT8 cmd, UINT32 param)
{
	BOOL ret = TRUE;
#ifdef WIN32
       return TRUE;
#endif
	ret = AudSendCommandReq(AUD_BT, cmd, param);

#ifdef MUSIC_BAND_INFO

	if(cmd == MC_PLAY_A2DP)
	{
		if(0xFF == g_TimerId )
		{
			//g_TimerId = mmi_timer_create(1, APP_DisplayFreqBand_TimeOutHandler,NULL,COS_TIMER_MODE_PERIODIC);
		       g_TimerId = COS_SetTimer(50, APP_DisplayFreqBand_TimeOutHandler, NULL, COS_TIMER_MODE_PERIODIC);
		}

	}else if(cmd == MC_STOP){

		if(0xFF != g_TimerId ){
			//mmi_cancel_timer(g_TimerId);
			COS_KillTimer(g_TimerId);
			g_TimerId = 0xFF;
		}
	}
#endif

	return ret;

}
#endif

BOOL mediaSendCommand(UINT8 cmd, INT32 param)
{
	BOOL ret = FALSE;
#ifdef WIN32
       return TRUE;
#endif

#if APP_SUPPORT_FADE_INOUT==1
	//Wait fade out finished cbk, and send MC_STOP command
	if(MC_STOP == cmd  && no_fade != param)
	{
        AudSendFadeInOutReq(FALSE, param, NULL);
	}
#endif

	ret = (BOOL) AudSendCommandReq(AUD_COMMON, cmd, param);

#ifdef MUSIC_BAND_INFO

	if(cmd == MC_PLAY)
	{
		if(0xFF == g_TimerId )
		{
			//g_TimerId = mmi_timer_create(1, APP_DisplayFreqBand_TimeOutHandler,NULL,COS_TIMER_MODE_PERIODIC);
		       g_TimerId = COS_SetTimer(50, APP_DisplayFreqBand_TimeOutHandler, NULL, COS_TIMER_MODE_PERIODIC);
		}

	}else if(cmd == MC_STOP){

		if(0xFF != g_TimerId ){
			//mmi_cancel_timer(g_TimerId);
			COS_KillTimer(g_TimerId);
			g_TimerId = 0xFF;
		}
	}
#endif

	return ret;
}

void media_StopInternalAudio(void)
{
#ifdef WIN32
    MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, 0);
    return 0;
#endif
	AudSendCommandReq(AUD_INTERNAL, MC_STOP, 0);
}

UINT32 media_PlayInternalAudio(UINT8 audio_id, UINT8 times, BOOL waitKey)
{
	UINT32 param = 0;
    
#ifdef WIN32
    MediaSendAppEvent(EV_AUDIO_INTERNAL_PLAY_FINISH_IND, 0);
    return 0;
#endif
    app_trace(APP_MEDIA_TRC, "media_PlayInternalAudio, audio_id=%d, times=%d, waitKey=%d",audio_id, times, waitKey);
	param = AUD_INTERNAL_PLAY_REQ_SET_PARAM(audio_id, times);
	
	return AudSendCommandReq(AUD_INTERNAL, MC_PLAY, param);

}



#if defined(CAMERA_SUPPORT)


void cam_set_pair_results(UINT32 result, UINT32 value)
{
    cam_result = result;
    cam_value = value;
}

BOOL media_CameraPowerOn(UINT32 mode)
{
    COS_EVENT ev;

    ev.nEventId = EV_CAMERA_POWER_UP_REQ;
    ev.nParam1 = mode;

    cam_result = TRUE;
    CAM_INIT_EVENT();
    COS_SendEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CAM_WAIT_EVENT(CAM_EVT_POWER_UP);
    return cam_result;
}

BOOL media_CameraPowerDown(void)
{
    COS_EVENT ev;

    ev.nEventId = EV_CAMERA_POWER_DOWN_REQ;

    cam_result = TRUE;
    CAM_INIT_EVENT();
    COS_SendEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CAM_WAIT_EVENT(CAM_EVT_POWER_DOWN);
    return cam_result;
}
#if defined(IP_CAMERA_SUPPORT)
BOOL media_IPCameraStart(CAM_CAPTURE_STRUCT *capture)
{
    COS_EVENT ev;

    ev.nEventId = EV_CAMERA_IPCAM_START_REQ;
    ev.nParam1 = (UINT32) capture;

    cam_result = TRUE;
    CAM_INIT_EVENT();
    COS_SendEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CAM_WAIT_EVENT(IPCAM_EVT_START);
    return cam_result;
}

BOOL media_IPCameraStop(void)
{
    COS_EVENT ev;

    ev.nEventId = EV_CAMERA_IPCAM_STOP_REQ;

    cam_result = TRUE;
    CAM_INIT_EVENT();
    COS_SendEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CAM_WAIT_EVENT(IPCAM_EVT_STOP);
    return cam_result;
}

UINT32 media_IPCameraGetFramBuffer(UINT8 *buffer, UINT32 max_len)
{
    COS_EVENT ev;

    ev.nEventId = EV_CAMERA_IPCAM_GETFRAME_REQ;
    ev.nParam1 = (UINT32) buffer;
    ev.nParam2 = (UINT32) max_len;

    cam_result = TRUE;
    CAM_INIT_EVENT();
    COS_SendEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CAM_WAIT_EVENT(IPCAM_EVT_GET_FRAME_DATA);
    return cam_result;
}

#endif /* IP_CAMERA_SUPPORT */
#endif /* CAMERA_SUPPORT */
