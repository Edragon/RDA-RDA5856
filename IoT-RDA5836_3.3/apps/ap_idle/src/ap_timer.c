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

#include "ap_gui.h"
#include "ap_idle.h"
#include "tm.h"
#include "ap_fm.h"
#include "ap_music.h"
#include "ap_media.h"

#define AP_MAX_TIMER                5

#define AP_TIMER_ACTIVE             1

#define AP_TIMER_MAX_NAME           64

#define TIMER_ALARM_OPEN            AP_MSG_USER_TIMER+1
#define TIMER_ALARM_CLOSE           AP_MSG_USER_TIMER+2
#define TIMER_ALARM_TIME            AP_MSG_USER_TIMER+3
#define TIMER_ALARM_TYPE            AP_MSG_USER_TIMER+4
#define TIMER_ALARM_DAYS            AP_MSG_USER_TIMER+5
#define TIMER_ALARM_SELECT          AP_MSG_USER_TIMER+6
#define TIMER_ALARM_EDIT            AP_MSG_USER_TIMER+7

enum alarm_type
{
    MUSIC_ALARM,
    FM_TIMER,
    FM_REC_TIMER,
    AUDIO_ALARM,  // internal audio
};

typedef struct
{
    UINT8 type;
    UINT8 flag; // bit 7~4 disk
    UINT8 days; // bit 0 ~ 6, sunday to saturday
    UINT8 volume;
    UINT16 time; // time in 24 hour
    UINT32 file_or_freq; // music file for alarm or freq for fm timer
} alarm_t;

typedef struct
{

    alarm_t timers[AP_MAX_TIMER];
    UINT16 magic;
} timer_vars_t;


typedef struct
{
	alarm_t alarm;
	//UINT8 draw_timer;
	UINT8 stop_timer;
	UINT8 fade_in_timer;
} TIMER_ALARM_GUI_T,*TIMER_ALARM_GUI_PTR;


extern BOOL     ui_auto_update;
extern INT32           g_last_reslt;

extern FM_play_status_t    *FMStatus;                //当前电台信息
extern music_vars_t *g_music_vars;//本AP的公共变量


#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)

timer_vars_t *ap_timers;
INT8 g_timer_setting_flag;
INT8 curr_active_alarm;

static INT8 alarm_item_buf[AP_TIMER_MAX_NAME];
static INT8 curr_setting_alarm;
static INT32 menu_active;

static const UINT16 alarm_days_text[7]     = {GUI_STR_MONDAY,   GUI_STR_TUESDAY,  GUI_STR_WEDNESDAY,  GUI_STR_THURSDAY, GUI_STR_FRIDAY,  GUI_STR_SATURDAY, GUI_STR_SUNDAY};
extern const INT8 month_days[];
extern INT8 g_play_disk;

BOOL TIMER_Init(void)
{
    INT i;

    MESSAGE_Initial(g_comval);

#if APP_SUPPORT_FM
    FMStatus = (FM_play_status_t*)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));
#endif
    g_music_vars = (music_vars_t*)NVRAMGetData(VM_AP_MUSIC,sizeof(music_vars_t));

    ap_timers = (timer_vars_t*)NVRAMGetData(VM_AP_TIMER,sizeof(timer_vars_t));

    if(ap_timers->magic != MAGIC_TIMER)
    {
        memset(ap_timers, 0, sizeof(ap_timers));
        for(i=0; i<AP_MAX_TIMER; i++)
        {
            ap_timers->timers[i].time = 360; // set default clock to 6:00
            ap_timers->timers[i].file_or_freq = 1;
        }
        ap_timers->magic = MAGIC_TIMER;
    }

}

void Timer_AlarmVolumeFadeInTimeout(void *param)
{
    TIMER_ALARM_GUI_PTR p = (TIMER_ALARM_GUI_PTR) GetHistoryGuiBuffer(SCR_ID_ALARM);

	if(p)
	{
	    app_trace(APP_SETTING_TRC,"Timer_AlarmVolumeFadeInTimeout=%d",p->alarm.volume);
	    if(p->alarm.volume < AUD_MAX_LEVEL)
	    {
			p->alarm.volume++;
	        mediaSendCommand(MC_SETVOLUME, p->alarm.volume);
        }
	}
}

void Timer_AlarmTimeout(void *param)
{
    TIMER_ALARM_GUI_PTR p = (TIMER_ALARM_GUI_PTR) GetHistoryGuiBuffer(SCR_ID_ALARM);

	if(p)
	{
	    app_trace(APP_SETTING_TRC,"Timer_AlarmTimeout=%d",p->alarm.type);
        if(p->alarm.type == MUSIC_ALARM)
        {
            mediaSendCommand(MC_STOP, 0);
            mediaSendCommand(MC_CLOSE, 0);
        }
        else
        {
            media_StopInternalAudio();
        }
	}
}

void Timer_AlarmDrawTimeout(void *param)
{

}

void Timer_AlarmPlayMusicCallback(UINT32 result )
{
    BOOL no_error = TRUE;

    app_trace(APP_SETTING_TRC,"Timer_AlarmPlayMusicCallback=%d",result);

    if (result == PLAYING_ERROR)
    {

    }
    else if (result == PLAYING_REACH_END)
    {
        mediaSendCommand(MC_STOP, 0);
        mediaSendCommand(MC_PLAY, 0);
    }
}

/******************************************************************************
 * TIMER_AlarmMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL TIMER_AlarmMsgHandler(COS_EVENT *pEvent)
{
    UINT16 param;
    TIMER_ALARM_GUI_PTR p = (TIMER_ALARM_GUI_PTR) GetHistoryGuiBuffer(SCR_ID_ALARM);

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            if(p->alarm.type == MUSIC_ALARM)
            {
                SetProtocolEventHandler(Timer_AlarmPlayMusicCallback, EV_AUDIO_PLAY_FINISH_IND);
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);

			// stop in one minute
            p->stop_timer = mmi_timer_create(600,Timer_AlarmTimeout,NULL,COS_TIMER_MODE_SINGLE);
            //fade in timer
            p->fade_in_timer = mmi_timer_create(100,Timer_AlarmVolumeFadeInTimeout,NULL,COS_TIMER_MODE_PERIODIC);
            break;

        case EV_UI_FW_REDRAW:
        {
            TM_SYSTEMTIME systemTime;
            UINT32 time;

            TM_GetSystemTime(&systemTime);
            time = systemTime.uHour*3600 + systemTime.uMinute * 60 + systemTime.uSecond;

            GUI_ClearScreen(NULL);//清全屏幕
            GUI_DisplayBattaryLever();
            GUI_ResShowImage(GUI_IMG_ALARM_PICS, (systemTime.uMilliseconds/333)%3, 48,0);
            GUI_DisplayTime(4, 32, GUI_TIME_DISPLAY_ALL, GUI_IMG_BIG_NUM, time*1000);
            GUI_UpdateScreen(NULL);
        }
        break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_ON_EXIT:
			mmi_cancel_timer(p->stop_timer);
			mmi_cancel_timer(p->fade_in_timer);
            ClearProtocolEventHandler(EV_AUDIO_PLAY_FINISH_IND);
            if(p->alarm.type == MUSIC_ALARM)
            {
                mediaSendCommand(MC_STOP, 0);
                mediaSendCommand(MC_CLOSE, 0);
            }
            else
            {
                media_StopInternalAudio();
            }
            NVRAMWriteData();
            break;

        case AP_KEY_PRESS:
        	AppGobackScreen();
        	break;
        	

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
            	case AP_MSG_RTC:
            	   SendAppEvent(EV_UI_FW_REDRAW, 0);
            	   break;

	            case AP_MSG_WAIT_TIMEOUT:
	            case AP_MSG_LOW_POWER:
	            case AP_MSG_STANDBY:
	                break;

	            case AP_KEY_PLAY|AP_KEY_HOLD:
	                if(!MESSAGE_IsHold())
	                    break;
	            case AP_KEY_POWER|AP_KEY_PRESS:
	                SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_STANDBY);
	                break;

                default:
                {
                    //Common events, DONOT deal it.
                    return FALSE;
                }
            }
    }

    return TRUE;
}

/******************************************************************************
 * TIMER_AlarmCallAPP:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
INT32 TIMER_AlarmCallAPP(UINT8 alarm_index)
{
    UINT8 disk, volume;
    INT32 result=RESULT_MAIN;//POWERON;
    Music_type_t music_type;
    Open_param_t param;

    if(alarm_index < 0)
        return RESULT_IDLE;

#if APP_SUPPORT_FM
    if(ap_timers->timers[alarm_index].type == FM_TIMER || ap_timers->timers[alarm_index].type == FM_REC_TIMER)
    {
        FMStatus->freq = ap_timers->timers[alarm_index].file_or_freq;

        if(ap_timers->timers[alarm_index].type == FM_TIMER)
            return RESULT_RADIO;
        else
            return RESULT_FMREC_START;
    }
#endif

    if(ap_timers->timers[alarm_index].type == AUDIO_ALARM)
    {
        media_PlayInternalAudio(GUI_AUDIO_ALARM1+ap_timers->timers[alarm_index].file_or_freq-1, 0, FALSE);
    }
    else
    {
        disk = ap_timers->timers[alarm_index].flag>>4;
        if (!MountDisk(disk))
        {
            GUI_DisplayMessage(0, 0, "Alarm", GUI_MSG_FLAG_KEY);
            return RESULT_RADIO;
        }

        result = FS_GetShortName(ap_timers->timers[alarm_index].file_or_freq, alarm_item_buf);
        music_type = MUSIC_CheckType(alarm_item_buf);
        if(result != ERR_SUCCESS || music_type == Music_NON)
        {
            GUI_DisplayMessage(0, 0, "Alarm", GUI_MSG_FLAG_KEY);
            return RESULT_RADIO;
        }

        param.type      = music_type;
        param.mode      = MEDIA_PLAY;
        param.file_entry = ap_timers->timers[alarm_index].file_or_freq;

        result = mediaSendCommand(MC_OPEN,(INT32)&param);
        if(!result)
        {
            GUI_DisplayMessage(0, 0, "Alarm", GUI_MSG_FLAG_KEY);
            return RESULT_RADIO;
        }

        volume = ap_timers->timers[alarm_index].volume;
        mediaSendCommand(MC_PLAY, 0);
        mediaSendCommand(MC_SETVOLUME, volume);
    }


    return RESULT_NULL;
}


INT32 TIMER_Alarm(void)
{
    UINT8 alarm_index;
    INT32 result=RESULT_MAIN;//POWERON;
    TIMER_ALARM_GUI_PTR p = NULL;

    TIMER_Init();

    app_trace(APP_SETTING_TRC,"====TIMER_Alarm====%d",curr_active_alarm);

    alarm_index = curr_active_alarm;
    if(alarm_index >= 0)
    {
        if(ap_timers->timers[alarm_index].days == 0)
        {
            ap_timers->timers[curr_setting_alarm].flag &= ~AP_TIMER_ACTIVE;
            curr_active_alarm--;
        }
    }

    TIMER_SetAlarm(0);

    result = TIMER_AlarmCallAPP(alarm_index);
    app_trace(APP_SETTING_TRC,"TIMER_AlarmCallAPP=%d",result);

    if(RESULT_NULL != result)
    {
        SendAppEvent(EV_UI_FW_SWITCH_MOD, result);
        return 0;
    }

	DeleteScreenIfPresent(SCR_ID_ALARM);
	
    p = (TIMER_ALARM_GUI_PTR) COS_MALLOC(sizeof(TIMER_ALARM_GUI_T));
    memset(p, 0x0, sizeof(TIMER_ALARM_GUI_T));
    memcpy(&(p->alarm), &(ap_timers->timers[alarm_index]), sizeof(alarm_t));

    EntryNewScreen(SCR_ID_ALARM, TIMER_AlarmMsgHandler, p, 0, 0);

}

void SET_ALARMClockSettingCallback(date_t *date,ap_time_t *time)
{
    /*Save Clock*/
    ap_timers->timers[curr_setting_alarm].time = time->hour*60 + time->minute;

    TIMER_SetAlarm(0);

}
/*********************************************************************************
* Description : 闹钟设置:设置闹钟时间
*
* Arguments   :
*
* Returns     :
*
*
*********************************************************************************/
INT32 SET_ALARMClockSetting(void)
{
    ap_time_t time;

    time.hour   = ap_timers->timers[curr_setting_alarm].time/60;
    time.minute = ap_timers->timers[curr_setting_alarm].time%60;

    GUI_ClockSetting(NULL,&time,GUI_IMG_CLOCKICON,GUI_STR_ALARMCLOCK, SET_ALARMClockSettingCallback);

    return 0;
}

INT32 days_list_callback(INT32 type, INT32 value, INT8 **string)
{
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = GUI_GetString(alarm_days_text[value]);
        return (ap_timers->timers[curr_setting_alarm].days & (1<<value)) != 0;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        if(ap_timers->timers[curr_setting_alarm].days & (1<<value))
            ap_timers->timers[curr_setting_alarm].days &= ~(1<<value);
        else
            ap_timers->timers[curr_setting_alarm].days |= (1<<value);
        return 1;
    }
}


/*********************************************************************************
* Description : 闹钟设置:设置闹钟周期
*
* Arguments   :
*
* Returns     :
*
*
*********************************************************************************/
INT32 SET_ALARMTimesSetting(void)
{
    UINT8 result;

    result = GUI_Display_List(LIST_TYPE_MULTI_SELECT, 0, 7, 0, days_list_callback, FALSE);

    return 0;
}

INT32 timer_list_callback(INT32 type, INT32 value, INT8 **string)
{
    INT time, i;
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = alarm_item_buf;
        time = ap_timers->timers[value].time;
        if(ap_timers->timers[value].type == AUDIO_ALARM)
            sprintf(alarm_item_buf,"%02d:%02d%s", time/60, time%60, GUI_GetString(GUI_STR_ALARM_AUDIO1+ap_timers->timers[value].file_or_freq-1));
        else
            sprintf(alarm_item_buf,"%02d:%02d%s", time/60, time%60, GUI_GetString(GUI_STR_ALARMMUSIC+ap_timers->timers[value].type));
        return ap_timers->timers[value].flag & AP_TIMER_ACTIVE;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        curr_setting_alarm = value;
        SendAppEvent(TIMER_ALARM_EDIT, value);
        return 0;
    }
}

INT32 timer_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    INT time, i;
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
    {
        if(value == GUI_MENU_TIMERTYPE)
        {
            if(AUDIO_ALARM == ap_timers->timers[curr_setting_alarm].type)
                return MUSIC_ALARM;
            else
                return ap_timers->timers[curr_setting_alarm].type;
        }
        else if(value == GUI_MENU_TIMERAUDIO)
        {
            if(AUDIO_ALARM == ap_timers->timers[curr_setting_alarm].type)
                return ap_timers->timers[curr_setting_alarm].file_or_freq;
            else
                return RESULT_MUSIC;
        }
        else if(value == GUI_MENU_TIMER)
            return menu_active;
        else
            return 0;
    }

    if(MENU_CALLBACK_QUERY_TEXT == type)
    {
        *string = alarm_item_buf;
        if(value == TIMER_ALARM_TIME)
        {
            time = ap_timers->timers[curr_setting_alarm].time;
            sprintf(alarm_item_buf,"%02d:%02d", time/60, time%60);
        }
        else if(value == TIMER_ALARM_DAYS)
        {
            if(ap_timers->timers[curr_setting_alarm].days == 0)
                *string = GUI_GetString(GUI_STR_ALARMNOREPEAT);
            else if(ap_timers->timers[curr_setting_alarm].days == 0x7f)
                *string = GUI_GetString(GUI_STR_ALARMEVERYDAY);
            else if(ap_timers->timers[curr_setting_alarm].days == 0x1f)
                *string = GUI_GetString(GUI_STR_ALARMWORKDAY);
            else
            {
                time = -1;
                for(i=0; i<7; i++)
                {
                    if(ap_timers->timers[curr_setting_alarm].days & (1<<i))
                    {
                        time = (time+1)*8+i;
                    }
                }
                if(time < 7)
                    *string = GUI_GetString(alarm_days_text[time]);
                else
                    *string = GUI_GetString(GUI_STR_ALARMDAYS);
            }
        }
        else if(value == GUI_MENU_TIMERAUDIO)
        {
            UINT32 len=0;
            *string = GUI_GetString(GUI_STR_ALARMFILE);
            if(AUDIO_ALARM == ap_timers->timers[curr_setting_alarm].type)
            {
                *string = GUI_GetString(GUI_STR_ALARM_AUDIO1+ap_timers->timers[curr_setting_alarm].file_or_freq-1);
            }
            else if(MUSIC_ALARM == ap_timers->timers[curr_setting_alarm].type)
            {
                UINT8 disk = ap_timers->timers[curr_setting_alarm].flag>>4;
                if (MountDisk(disk) && (FS_GetEntryName(ap_timers->timers[curr_setting_alarm].file_or_freq,AP_TIMER_MAX_NAME,alarm_item_buf)==ERR_SUCCESS) )
                {
                    gui_unicode2local(alarm_item_buf,AP_TIMER_MAX_NAME-2);
                    *string = alarm_item_buf;
                }
            }
        }
        else if(value == RESULT_RADIO)
        {
            time = ap_timers->timers[curr_setting_alarm].file_or_freq/100;

            sprintf(alarm_item_buf, "FM%d.%dMHz", time/10, time%10);
        }
        return 0;
    }

    switch(value)
    {
        case TIMER_ALARM_CLOSE:
            return ap_timers->timers[curr_setting_alarm].flag & AP_TIMER_ACTIVE;
            break;
        case TIMER_ALARM_OPEN:
            return (ap_timers->timers[curr_setting_alarm].flag & AP_TIMER_ACTIVE) == 0;
            break;
        case GUI_MENU_TIMERTYPE:
            if(ap_timers->timers[curr_setting_alarm].type == AUDIO_ALARM)
                return param == MUSIC_ALARM;
            else
                return ap_timers->timers[curr_setting_alarm].type == param;
            break;
        case RESULT_MUSIC:
            return ap_timers->timers[curr_setting_alarm].type == MUSIC_ALARM || ap_timers->timers[curr_setting_alarm].type == AUDIO_ALARM;
            break;
#if APP_SUPPORT_FM
        case RESULT_RADIO:
            return ap_timers->timers[curr_setting_alarm].type == FM_TIMER || ap_timers->timers[curr_setting_alarm].type == FM_REC_TIMER;
            break;
#endif
        default:
            break;
    }
    return 1;
}

/******************************************************************************
 * TIMER_EditAlarmOptMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL TIMER_EditAlarmOptMsgHandler(COS_EVENT *pEvent)
{
    UINT16 param;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            GUI_Display_Menu(GUI_MENU_TIMER, timer_menu_callback);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            if(RESULT_IGNORE == pEvent->nParam1)
                AppGobackScreen();
            else
                SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_ON_EXIT:
            NVRAMWriteData();
            break;

        default:
            //key event from timer menu
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case TIMER_ALARM_OPEN:
                    ap_timers->timers[curr_setting_alarm].flag |= AP_TIMER_ACTIVE;
                    TIMER_SetAlarm(0);
                    menu_active = 3;
                    break;
                case TIMER_ALARM_CLOSE:
                    ap_timers->timers[curr_setting_alarm].flag &= ~AP_TIMER_ACTIVE;
                    TIMER_SetAlarm(0);
                    menu_active = 4;
                    break;
                case TIMER_ALARM_TIME:
                    SET_ALARMClockSetting();
                    menu_active = 8;
                    break;
                case TIMER_ALARM_TYPE:
                    GUI_GetMenuResult(&param);
                    ap_timers->timers[curr_setting_alarm].type = param;
                    menu_active = param;
                    break;
                case TIMER_ALARM_DAYS:
                    SET_ALARMTimesSetting();
                    menu_active = 9;
                    break;
                case TIMER_ALARM_SELECT:
                    //menu_active = 9;
                    ap_timers->timers[curr_setting_alarm].type = AUDIO_ALARM;
                    GUI_GetMenuResult(&param);
                    ap_timers->timers[curr_setting_alarm].file_or_freq = param;
                    break;
                case AP_KEY_MODE|AP_KEY_PRESS:
                case AP_KEY_PLAY|AP_KEY_PRESS:
                    //AppGobackScreen();
                    break;
                default:
                {
                    //Common events, DONOT deal it.
                    return FALSE;
                }
            }
    }

    return TRUE;
}

INT32 TIMER_EditAlarmOption(void)
{
    EntryNewScreen(SCR_ID_ALARM_EDITALARM_OPT, TIMER_EditAlarmOptMsgHandler, NULL, 0, 0);
}

/******************************************************************************
 * TIMER_EditAlarmMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL TIMER_EditAlarmMsgHandler(COS_EVENT *pEvent)
{
    UINT16 param;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            GUI_Display_List(LIST_TYPE_MULTI_SELECT, 0, AP_MAX_TIMER, curr_setting_alarm, timer_list_callback, FALSE);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            if(RESULT_IGNORE == pEvent->nParam1)
                AppGobackScreen();
            else
                SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_ON_EXIT:
            NVRAMWriteData();
            break;

        //Entry timer edit scr
        case TIMER_ALARM_EDIT:
        {
            alarm_t *p = (alarm_t *) GetHistoryGuiBuffer(SCR_ID_ALARM_EDITALARM);
            if(curr_active_alarm == -1)
                curr_active_alarm = 0;

            //Save new alarm to selected item
            if(p != NULL)
            {
                ap_timers->timers[curr_setting_alarm].type = p->type;
                ap_timers->timers[curr_setting_alarm].flag = p->flag;
                ap_timers->timers[curr_setting_alarm].file_or_freq = p->file_or_freq;
                ap_timers->timers[curr_setting_alarm].volume = p->volume;
                ClearGuiBuffer(SCR_ID_ALARM_EDITALARM);
            }

            TIMER_EditAlarmOption();
        }
        break;

        default:
            //key event from timer menu
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_KEY_MODE|AP_KEY_PRESS:
                case AP_KEY_PLAY|AP_KEY_PRESS:
                    //AppGobackScreen();
                    break;
                default:
                {
                    //Common events, DONOT deal it.
                    return FALSE;
                }
            }
    }

    return TRUE;
}

/******************************************************************************
 * TIMER_EditAlarm:
 * DESCRIPTION: -
 *
 * Input: new_alarm - if NOT empty save to selected alarm before entry edit option scr, or just edit alarm
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
INT32 TIMER_EditAlarm(alarm_t *new_alarm)
{
    alarm_t *p = NULL;

    TIMER_Init();

    if(NULL != new_alarm)
    {
        p = (alarm_t *) COS_MALLOC(sizeof(alarm_t));
        memcpy(p, new_alarm, sizeof(alarm_t));
    }

    EntryNewScreen(SCR_ID_ALARM_EDITALARM, TIMER_EditAlarmMsgHandler, p, 0, 0);
}


void TIMER_SetAlarm(BOOL maintask)
{
    INT i, first = -1, days;
    UINT32 time, cur_time;
    TM_SYSTEMTIME systime;
    //if(maintask)
    //    NVRAMRead(&ap_timers,VM_AP_TIMER,sizeof(ap_timers));
    ap_timers = (timer_vars_t*)NVRAMGetData(VM_AP_TIMER,sizeof(timer_vars_t));

    TM_GetSystemTime(&systime);
    cur_time = systime.uHour * 60 +systime.uMinute;
    time = cur_time;

    for(i=0; i<AP_MAX_TIMER; i++)
    {
        if(ap_timers->timers[i].flag & AP_TIMER_ACTIVE)
        {
            if(ap_timers->timers[i].days != 0)
            {
                if(ap_timers->timers[i].time > cur_time)
                {
                    if((ap_timers->timers[i].days & (1<<systime.uDayOfWeek))==0)
                        continue;
                }
                else
                {
                    if((ap_timers->timers[i].days & (1<<(systime.uDayOfWeek+1)%7))==0)
                        continue;
                }
            }
            if(ap_timers->timers[i].time > cur_time)
            {
                if(time <= cur_time || ap_timers->timers[i].time < time)
                {
                    time = ap_timers->timers[i].time;
                    first = i;
                }
            }
            else
            {
                if(time<=cur_time && ap_timers->timers[i].time < time)
                {
                    time = ap_timers->timers[i].time;
                    first = i;
                }
            }
        }
    }
    if(first < 0)
    {
        TM_DisableAlarm();
        curr_active_alarm = -1;
        return;
    }

    if(time < cur_time)
    {
        days = month_days[systime.uMonth-1];
        if(IsLeapYear(systime.uYear) && (systime.uMonth == 2))
        {
            days++;
        }
        if(systime.uDay == days)
        {
            systime.uDay = 1;
            if(systime.uMonth == 12)
            {
                systime.uMonth = 1;
                systime.uYear ++;
            }
            else
                systime.uMonth ++;
        }
        else
            systime.uDay ++;
    }
    systime.uHour = time/60;
    systime.uMinute = time%60;
    systime.uSecond = 0;

    curr_active_alarm = first;
    TM_SetAlarm(&systime);
}



void TIMER_API_AddAlarm(INT32 mode)
{
    INT8 curr_setting_type;
    alarm_t  new_alarm;

    memset(&new_alarm, 0x0, sizeof(alarm_t));

    if(mode == RESULT_MUSIC)
    {
        new_alarm.type = MUSIC_ALARM;
        new_alarm.flag |= AP_TIMER_ACTIVE;

        //hal_HstSendEvent(SYS_EVENT, 0x16091201);
        //hal_HstSendEvent(SYS_EVENT, g_music_vars->location[g_play_disk].file_entry);
        if(g_music_vars->location[g_play_disk].file_entry > 0)
            new_alarm.file_or_freq = g_music_vars->location[g_play_disk].file_entry;
        new_alarm.flag |= (g_play_disk<<4);
        new_alarm.volume = g_music_vars->volume;
    }
#if APP_SUPPORT_FM
    else if(mode == RESULT_RADIO)
    {
        new_alarm.type = FM_TIMER;
        new_alarm.flag |= AP_TIMER_ACTIVE;

        if(FMStatus->freq > 0)
            new_alarm.file_or_freq = FMStatus->freq;
        new_alarm.volume = FMStatus->volume;
    }
#endif

    TIMER_EditAlarm(&new_alarm);

}

#endif // APP_SUPPORT_LCD

