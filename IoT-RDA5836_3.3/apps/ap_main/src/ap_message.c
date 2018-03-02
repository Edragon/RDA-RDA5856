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

#include "tgt_app_cfg.h"
#include "ap_gui.h"
#include "event.h"
//#include "ap_bluetooth.h"
#include "MainTask.h"//20130515
#include "gpio_edrv.h"
#ifdef SUPPORT_KEY_FILTER
#include "hal_sys.h"
#endif
#if APP_SUPPORT_BLUETOOTH
//#include "rdabt_common.h"
#include "bt_msg.h"
#endif
#include "ap_fm.h"
#include "ap_mgr.h"
#include "ap_hfp.h"
#if APP_SUPPORT_MUSIC==1
#include "ap_music.h"
#endif
#include "ap_usbdisk.h"
#include "ap_linein.h"
#include "ap_charging.h"
#include "ap_media.h"

#if APP_SUPPORT_RECORD==1
#include "ap_record.h"
#endif
#include "aud_main.h"
#ifndef WIN32
#include "uctls_m.h"
#endif

#include "pm.h"
#include "dm.h"

typedef UINT32 (*Key_func)(UINT32);

static PseventInfo protocolEventHandler[MAX_PROTOCOL_EVENT];

UINT8 g_shutdown_tm_handle = 0;

UINT8 g_light_time=0;     //关背光时间,0.5秒为单位
UINT16 g_standby_time=0;   //自动关机时间,0.5秒为单位
UINT16 g_rtc_counter=0;    //rtc 消息计数
UINT16 g_bat_time = 10;

UINT8 g_key_hold_flag = 0;
UINT8 g_mute_flag = 0;

extern INT8 g_play_disk;
extern INT8 g_next_play_disk;

#if APP_SUPPORT_HID_MODE==1
extern UINT8 g_current_BT_module;
#endif
void shutdown_timeout(void *param);
void AppModSwitchHandler(UINT32 result);
void AppSwitchLanguage(void);
VOID AppMsgResetRtcCounter(VOID);


/*********************************************************************************
* Description : 处理系统消息, 返回按键消息, 同时负责开/关背光
*
* Arguments   : key, 系统消息
*
* Returns     : 返回ap能够识别的消息, 如果没有系统消息, 返回 AP_KEY_NULL
*
* Notes       :

* 按键时序:
 *  0.....1.2....1.5........... (s)
 *  key    long   hold    up    (>1.2s)
 *  key  up            (<1.2s)
 *
 * key rate: 0.3s
*
*********************************************************************************/
BOOL is_first_batt = TRUE;
UINT8 batt_timer = 0;

void Batt_Timeout(void *param)
{
    //media_PlayInternalAudio(GUI_AUDIO_LOWPOWER, 1, FALSE);
    //GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_LOWPOWER, GUI_MSG_FLAG_WAIT);
    app_mgr_PlayInternalAudio(GUI_AUDIO_LOWPOWER);
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_LOWPOWER, 1);
    #endif 
	
}

void shutdown_timeout(void *param)
{
    g_shutdown_tm_handle = 0;
    hal_HstSendEvent(SYS_EVENT, 0x03281047);
#if APP_SUPPORT_TEST_BOX==0    
    MESSAGE_SetEvent(AP_MSG_STANDBY);
#endif
    AppMsgResetRtcCounter();
}

void ApMessageSendEvent(COS_MOD_ID mod_id, UINT32 event, UINT32 param)
{
    COS_EVENT ev = {event, param};
    COS_SendEvent(mod_id, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#ifdef SUPPORT_KEY_FILTER
extern volatile UINT8 g_key_filter;
#endif


BOOL MESSAGE_Hotkey(UINT32 key)
{
	UINT32 module_id = 0;

    switch(key)
    {
        case AP_KEY_POWER  | AP_KEY_HOLD:
#ifndef MMI_ON_WIN32
        {
            // enter usb download mode
            hal_SysSetBootMode((1<<1)); // BOOT_MODE_FORCE_MONITOR
            hal_SysRestart();
        }
#endif
        break;
        
        case AP_MSG_STANDBY:      //自动关机
        case AP_KEY_POWER  | AP_KEY_PRESS:
        //case AP_MSG_SLEEP:        //睡眠
        //case AP_MSG_LOW_POWER:    //低电自动关机
        case AP_MSG_FORCE_STANDBY:    //使用上两个会报label duplicate
            hal_HstSendEvent(SYS_EVENT, 0x03281048);
            module_id = RESULT_STANDBY;
            break;

        case AP_KEY_MODE|AP_KEY_PRESS:// press mode return to main | AP_KEY_LONG:    //长按menu进入main
            if(!AP_Support_MENU())
            {
	            module_id = RESULT_MAIN;
            }
            break;
            
        case AP_KEY_MUTE|AP_KEY_PRESS:
            g_mute_flag = !g_mute_flag;
            SetPAMute(g_mute_flag);
            break;

#if (APP_SUPPORT_MENU == 0)
        case AP_KEY_PLAY|AP_KEY_DOUBLE_CLICK:
            AppSwitchLanguage();
            break;
#endif
        case AP_MSG_LINE_IN:
            module_id = RESULT_LINE_IN;
            break;

        case AP_MSG_ALARM:
            module_id = RESULT_ALARM;
            break;

#if APP_SUPPORT_USBDEVICE
        case AP_MSG_USB_CONNECT:        //进入udisk 模式
            if(!IS_USB_MODE())
            {
                hal_HstSendEvent(SYS_EVENT,0x09070002);
                module_id = RESULT_UDISK;                
            }
            else
            {
                hal_HstSendEvent(SYS_EVENT,0x09070003);
            }
            break;

        case AP_MSG_USB_DISCONNECT:        //退出udisk模式
            uctls_Close();
            if(IS_USB_MODE())//20130515
            {
                module_id = RESULT_MAIN;
            }
			break;
#endif

#if APP_SUPPORT_USB
        case AP_MSG_USB_PLUGIN:
            g_next_play_disk = FS_DEV_TYPE_USBDISK;
            module_id = RESULT_MUSIC;
            break;
#endif

#if APP_SUPPORT_MUSIC==1
        case AP_MSG_SD_IN:
            g_next_play_disk = FS_DEV_TYPE_TFLASH;
            module_id = RESULT_MUSIC;
            break;
#endif
            
        case AP_MSG_BT_ACTIVE:
            module_id = RESULT_BT_ACTIVE;
            break;
            
        case AP_MSG_LOCK:        //locked
            GUI_DisplayLock(1);    //这里不处理热键,防止嵌套
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

#if (APP_SUPPORT_POWERKEY == 1)
        case AP_MSG_POWER_KEY: 
	     hal_SysRequestFreq(9, HAL_SYS_FREQ_39M, NULL);
            SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_STANDBY);
	     extern volatile uint8 tsd_PowerOff;
            tsd_PowerOff =0xaa;
            break;
#endif

        default:
            if(((tgt_GetKeyCode(0) <<16) |AP_KEY_HOLD) == key && MESSAGE_IsHold())
            {
                module_id = RESULT_STANDBY;
            }
            else
            {
                //Just drop the key here
                return FALSE;
            }
            break;
    }

	if(key != AP_MSG_RTC)
	{
		hal_HstSendEvent(1, 0x05122101);
		hal_HstSendEvent(1, key);
		hal_HstSendEvent(1, module_id);
	}
    if(0 != module_id)
    {
        app_trace(APP_MAIN_TRC, "MESSAGE_Hotkey module_id=%d", module_id);
        SendAppEvent(EV_UI_FW_SWITCH_MOD, module_id);
    }

    return TRUE;
}


/*********************************************************************************
* Description : 处理热键消息, 返回结果消息
        ap_handle_hotkey_core() 的入口
*
* Arguments   : key, 按键消息
*
* Returns     : 如果有可识别的热键消息,  返回结果消息
        如果没有可识别的热键消息,  返回0
*
* Notes       :
*
*********************************************************************************/
UINT32 MESSAGE_HandleHotkey(UINT32 key)
{
// TODO: This function is useless now, Remove me!
    return 0;
}


UINT32 MESSAGE_IsNumberKey(UINT32 key)
{
    if( ((key&0xffff) == AP_KEY_DOWN) && ((key&0xffff0000) >= AP_KEY_NUM1) && (key&0xffff0000) <= (AP_KEY_DOWN|AP_KEY_NUM0))
        return 1;
    else
        return 0;
}


/******************************************************************************
 * MESSAGE_IsHold: 
 * DESCRIPTION: - 
 * 防止hold 事件重复处理
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
UINT32 MESSAGE_IsHold(void)
{
    if(g_key_hold_flag == 0)
    {
        g_key_hold_flag = 1;
        return 1;
    }
    else
        return 0;
}

void MESSAGE_HoldFlagClr(void)
{
    g_key_hold_flag=0;
}
/******************************************************************************
 * AppGotoMainMenu: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void AppGotoMainMenu(void)
{
	SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_MAIN);
}

/******************************************************************************
 * AppGobackScreen: 
 * DESCRIPTION: - 
 * Goback screen with no param
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void AppGobackScreen(void)
{
	GobackScreen(0, 0);
}

/******************************************************************************
 * InitProtocolEvent: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void InitProtocolEvent(void)
{
	UINT16 count = 0;

	for(count = 0; count < MAX_PROTOCOL_EVENT; count++)
	{
		protocolEventHandler[count].eventID = 0;
		protocolEventHandler[count].entryFuncPtr = NULL;
	}

}
/******************************************************************************
 * SetProtocolEventHandler: 
 * DESCRIPTION: - sets protocol event handler
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void SetProtocolEventHandler(PsFuncPtr funcPtr, UINT32 eventID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	UINT16 count;
	UINT16 writeIndex = MAX_PROTOCOL_EVENT;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	APP_ASSERT(eventID != 0);
	
	for(count = 0; count < MAX_PROTOCOL_EVENT; count++)
	{
		if(protocolEventHandler[count].eventID == eventID)
		{
			//Has same event id, overwrite it.
			writeIndex = count;
			break;
		}
		else if(0 == protocolEventHandler[count].eventID)
		{
			//Save free index, and CONTINUE search if there has same event id.
			writeIndex = count;
		}
	}

	APP_ASSERT(writeIndex < MAX_PROTOCOL_EVENT);

	protocolEventHandler[writeIndex].eventID = eventID;
	protocolEventHandler[writeIndex].entryFuncPtr = funcPtr;
}

/******************************************************************************
 * ClearProtocolEventHandler: 
 * DESCRIPTION: - clears protocol event handler
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void ClearProtocolEventHandler(UINT32 eventID)
{
	SetProtocolEventHandler(NULL, eventID);
}
/******************************************************************************
 * ExecuteCurrProtocolHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL ExecuteCurrProtocolHandler(COS_EVENT *ev)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
	PsFuncPtr currFuncPtr = NULL;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	for (count = 0; count < MAX_PROTOCOL_EVENT; count++)
	{
		if (protocolEventHandler[count].eventID == ev->nEventId)
		{
			currFuncPtr = protocolEventHandler[count].entryFuncPtr;
			break;
		}
	}

	if(currFuncPtr)
	{
		app_trace(APP_MAIN_TRC, "ExecuteCurrProtocolHandler,id=%d,ptr=%x,param=%x",ev->nEventId,currFuncPtr,ev->nParam1);
		(*currFuncPtr)(ev->nParam1);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
/******************************************************************************
 * AppMsgResetRtcCounter: 
 * DESCRIPTION: - Reset g_rtc_counter , shutdown timer, and AP_MSG_WAIT_TIMEOUT
 * 
 * Input: void
 * Output: void
 * Returns: void
 * 
 * 
 ******************************************************************************/
VOID AppMsgResetRtcCounter(VOID)
{
    g_rtc_counter = 0;

    if(g_shutdown_tm_handle)
    {
        mmi_cancel_timer(g_shutdown_tm_handle);
        g_shutdown_tm_handle = 0;
    }
}
/******************************************************************************
 * AppMsgPreHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: TRUE - Stop post msg to others
 * 
 * 
 ******************************************************************************/
BOOL AppMsgPreHandler(COS_EVENT *ev)
{
	BOOL ret = FALSE; //TRUE - Stop post msg to others

#ifdef SUPPORT_KEY_FILTER
    UINT32 csStatus;
    csStatus = hal_SysEnterCriticalSection();
    switch(ev->nEventId)
    {
        case EV_KEY_DOWN:
            g_key_filter &= ~(0x1);
            break;
        case EV_KEY_PRESS:
            g_key_filter &= ~(0x2);
            break;
        case EV_KEY_UP:
            g_key_filter &= ~(0x4);
            break;
        case EV_KEY_LONG_PRESS:
            g_key_filter &= ~(0x8);
            break;
		case EV_KEY_DOUBLE_CLICK:
            g_key_filter &= ~(0x10);
            break;
        default:
            break;
    }
    hal_SysExitCriticalSection(csStatus);
#endif

    switch(ev->nEventId)
    {
        case AP_KEY_UP:
            g_key_hold_flag = 0;
        case AP_KEY_DOWN:
        case AP_KEY_HOLD:
        case AP_MSG_USB_PLUGIN:        //usb plugin, 等同按键
        case AP_MSG_USB_PULLOUT:
        case AP_MSG_SD_IN:
        case AP_MSG_SD_OUT:
        case AP_MSG_WAKE_REQ:
            //开背光
            APP_Wakeup();
            break;

        case AP_MSG_RTC:           //rtc
            g_rtc_counter++;
            //hal_HstSendEvent(BOOT_EVENT, 0x20170818);
            //hal_HstSendEvent(BOOT_EVENT, g_rtc_counter);
/*
#if APP_SUPPORT_BLUETOOTH==1
            BT_UpdateLed();
#endif*/
            if(IsChargePlugIn())
            {
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            	GUI_DisplayBattaryLever();
#endif
            }
            else if(g_bat_time !=0 && (g_rtc_counter%g_bat_time) == 0)
            {
                UINT32 batt = GetBattery();
				/*
#if APP_SUPPORT_BTBATTERY==1
                BT_HFPSendBattery();
#endif*/
                if(batt == 1)
                {
                    //key = AP_MSG_LOW_POWER;
                    MESSAGE_SetEvent(AP_MSG_LOW_POWER);
                    //media_PlayInternalAudio(GUI_AUDIO_LOWPOWER, 1, FALSE);
                    //GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_LOWPOWER, GUI_MSG_FLAG_WAIT);
                    app_mgr_PlayInternalAudio(GUI_AUDIO_LOWPOWER);
                }
                else if(batt < 15 && is_first_batt)
                {
                    is_first_batt = FALSE;
                    Batt_Timeout(NULL);
                    batt_timer = mmi_timer_create(100, Batt_Timeout, NULL, COS_TIMER_MODE_PERIODIC);
                }
                else if(batt >= 15)
                {
                    if(batt_timer)
                    {
                        mmi_cancel_timer(batt_timer);
                        batt_timer = 0;
                    }
                    is_first_batt = TRUE;
                }
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
                GUI_DisplayBattaryLever();
#endif
            }

            if(g_light_time != 0 && g_rtc_counter == g_light_time)    //自动关背光
            {
                //关背光
                CloseBacklight();
            }

            if(g_rtc_counter == 12)
            {
                //hal_HstSendEvent(SYS_EVENT, 0x03281036);
                if(!g_shutdown_tm_handle)
                    g_shutdown_tm_handle = mmi_timer_create(g_standby_time*10, shutdown_timeout, NULL, COS_TIMER_MODE_SINGLE);
                mmi_change_timer(g_shutdown_tm_handle, g_standby_time * 10);
                MESSAGE_SetEvent(AP_MSG_WAIT_TIMEOUT);    // 6 sec timeout
				//hal_HstSendEvent(1, 0x05231651);
			}

            break;

        case MSG_LOW_POWER:    //low power, nmi 0.98v
            //APP_DUMP("low power", 0);
            break;
            
#if (0)//For test
        case AP_KEY_PRESS:
        	if(ev->nParam1 == KEY_MUTE)
        	{
        		static UINT8 temp = 0;

        		switch(temp)
        		{
        			case 0:
	        			media_PlayInternalAudio(GUI_AUDIO_BT_POWERON, 1, FALSE);
	        			break;
	        		case 1:
	        		   	media_PlayInternalAudio(GUI_AUDIO_BT_ANSWERCALL, 1, FALSE);
	        		   	break;
	        	}

	        	temp = (++temp) % 2;
	        	
	            GUI_DisplayMessage(0, 0, "Just For test!", GUI_MSG_FLAG_WAIT);
	            //SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_STANDBY);
			    app_trace(APP_MAIN_TRC, "Mute key press For test!");
				ret = TRUE;
			}
			break;
#endif /* 1 */

		default:
			break;
    }
    
	return ret;
}

const UINT8 g_LanguageList[] = {
#if APP_SUPPORT_LANG_ENGLISH==1
    GUI_LANG_ENGLISH,
#endif
#if APP_SUPPORT_LANG_SM_CHINESE==1
    GUI_LANG_SM_CHINESE,
#endif
#if APP_SUPPORT_LANG_TR_CHINESE==1
    GUI_LANG_TR_CHINESE,
#endif
#if APP_SUPPORT_LANG_JAPANESE==1
    GUI_LANG_JAPANESE  ,
#endif
#if APP_SUPPORT_LANG_KOREAN==1
    GUI_LANG_KOREAN    ,
#endif
#if APP_SUPPORT_LANG_SPANISH==1
    GUI_LANG_SPANISH	,
#endif
#if APP_SUPPORT_LANG_FRENCH==1    
    GUI_LANG_FRENCH		,
#endif
#if APP_SUPPORT_LANG_GERMAN==1
    GUI_LANG_GERMAN		,
#endif
#if APP_SUPPORT_LANG_ITALIAN==1    
    GUI_LANG_ITALIAN	,
#endif
#if APP_SUPPORT_LANG_RUSSIAN==1
    GUI_LANG_RUSSIAN	,
#endif
#if APP_SUPPORT_LANG_THAI==1
    GUI_LANG_THAI		,
#endif
#if APP_SUPPORT_LANG_TURKISH==1
    GUI_LANG_TURKISH	,
#endif
#if APP_SUPPORT_LANG_MALAY==1
    GUI_LANG_MALAY		,
#endif
#if APP_SUPPORT_LANG_VIETNAMESE==1    
    GUI_LANG_VIETNAMESE,
#endif
#if APP_SUPPORT_LANG_DANISH==1
    GUI_LANG_DANISH		,
#endif
#if APP_SUPPORT_LANG_PORTUGUESE==1
    GUI_LANG_PORTUGUESE,
#endif
#if APP_SUPPORT_LANG_INDONESIAN==1
    GUI_LANG_INDONESIAN,
#endif
#if APP_SUPPORT_LANG_ARABIC==1
    GUI_LANG_ARABIC	   ,
#endif
};

#if (APP_SUPPORT_MENU == 1)
#else
/******************************************************************************
 * AppGetNextMode:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
INT32 AppGetNextMode(INT32 cur_mode)
{
    INT32 result = cur_mode;

    switch(cur_mode)
    {
        case RESULT_BT:
#if APP_SUPPORT_HID_MODE==1  
             if(BT_MODE_HEADSET == g_current_BT_module)
             {
                result = RESULT_BT;
                g_current_BT_module = BT_MODE_HID; 
                break;
             }
            else if(BT_MODE_HID == g_current_BT_module)
            {
                g_current_BT_module = BT_MODE_HEADSET;
#if APP_SUPPORT_MUSIC==1
                result = RESULT_MUSIC;
                g_play_disk = FS_DEV_TYPE_TFLASH;
#else
                result = RESULT_BT;
#endif
                break;
            }
#endif
    
#if APP_SUPPORT_MUSIC==1
            result = RESULT_MUSIC;
            g_play_disk = FS_DEV_TYPE_TFLASH;
            break;
            
        case RESULT_MUSIC_TF:
        case RESULT_MUSIC_USB:
        case RESULT_MUSIC:
            if(FS_DEV_TYPE_INVALID== g_play_disk)
            {
                result = RESULT_MUSIC;
                g_play_disk = FS_DEV_TYPE_TFLASH;
                break;
            }
            else if(FS_DEV_TYPE_TFLASH == g_play_disk)
            {
                result = RESULT_MUSIC;
                g_play_disk = FS_DEV_TYPE_USBDISK;
                break;
            }
            else
            {
                g_play_disk = FS_DEV_TYPE_INVALID;
            }
#endif
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856) 
    #if APP_SUPPORT_LINEIN
    result = RESULT_LINE_IN;
    break;
    case RESULT_LINE_IN:
    #endif
    #if APP_SUPPORT_FM
    result = RESULT_RADIO;
    break;
    case RESULT_RADIO:
    #endif
	
#else

#if APP_SUPPORT_FM || APP_SUPPORT_LINEIN
            //没有Line in时才进入FM 2012-9-27
            if(gpio_detect_linein() == GPIO_DETECTED)
                result = RESULT_LINE_IN;
            else
                result = RESULT_RADIO;
            break;
        case RESULT_LINE_IN:
        case RESULT_RADIO:
#endif

#endif

#if 0//APP_SUPPORT_RECORD
            if(AP_Support_RECORD())
            {
                result = RESULT_RECORD_NOSTART;
                break;
            }
        case RESULT_RECORD_NOSTART:
#endif
#if APP_SUPPORT_BLUETOOTH
            result = RESULT_BT;
#if APP_SUPPORT_HID_MODE==1  
            g_current_BT_module = BT_MODE_HEADSET; 
#endif
            break;
#endif
        default:
            result = APP_DEFAULT_RESULT;
            break;
    }
    
    app_trace(APP_MAIN_TRC, "AppGetNextMode, result=%d", result);

    return result;
}



void AppSwitchLanguage(void)
{
    int i;

#if APP_SUPPORT_BLUETOOTH==1
    if(app_get_mgr_state()!=MGR_SCAN)
        return;
#endif

    if(media_is_bt_playing())
        return;
    
    for(i=0;i<sizeof(g_LanguageList); i++)
    {
        if(g_comval->langid==g_LanguageList[i])
            break;
    }
    i++;
    if(i>=sizeof(g_LanguageList))
        i = 0;
    g_comval->langid = g_LanguageList[i];
    gui_load_resource(g_comval->langid);
    GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_LANGUAGE, GUI_MSG_FLAG_WAIT);
    hal_HstSendEvent(SYS_EVENT, 0x1a1a0000 + g_comval->langid);
}
#endif
/******************************************************************************
 * AppGetModeRing: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
UINT8 AppGetModeRing(INT32 mode)
{
	UINT8 aud_id;

	switch (mode)
	{
		case RESULT_BT:
			aud_id = GUI_AUDIO_BT_POWERON;
			break;

		case RESULT_RADIO:
			aud_id = GUI_AUDIO_FM_PLAY;
			break;

		case RESULT_LINE_IN:
			aud_id = GUI_AUDIO_LINEIN_PLAY;
			break;

#ifdef CODEC_SUPPORT
		case RESULT_UART_PLAY:
			aud_id = GUI_AUDIO_LINEIN_PLAY;
			break;
#endif

		default:
			aud_id = 0;
			break;
	}

	return aud_id;
}

void AppPlayModeRingFinishHandler(UINT32 param)
{
    app_trace(APP_MAIN_TRC, "AppPlayModeRingFinishHandler %d", g_current_module);

	ClearProtocolEventHandler(EV_AUDIO_INTERNAL_PLAY_FINISH_IND);
    AppModSwitchHandler(g_current_module);
}
/******************************************************************************
 * AppModSwitchHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void AppModSwitchHandler(UINT32 result)
{	
    app_trace(APP_MAIN_TRC, "AppModSwitchHandler, result=%d, current_module=%d, last_result=%d", result, g_current_module, g_last_reslt);

    switch(result)
    {
/********Special module******/
#if (APP_SUPPORT_MENU == 1)
    	case RESULT_MAIN:
			MainMenu_Entry();
            break;
#endif

        case RESULT_STANDBY:
			APP_StandBy();
	        break;
#if APP_SUPPORT_CHARGING==1 
		case RESULT_CHARGING:
			Charging_Entry();
			break;
#endif 

/*********Normal module**********/
#if APP_SUPPORT_MUSIC==1            
        case RESULT_MUSIC:
			if(g_play_disk == FS_DEV_TYPE_INVALID)
            {
                g_play_disk = FS_DEV_TYPE_TFLASH;
            }
        	MUSIC_Entry(0);
            break;
        case RESULT_MUSIC_TF:
            g_play_disk = FS_DEV_TYPE_TFLASH;
            MUSIC_Entry((UINT32)FS_DEV_TYPE_TFLASH);
            break;
        case RESULT_MUSIC_USB:
            g_play_disk = FS_DEV_TYPE_USBDISK;
            MUSIC_Entry((UINT32)FS_DEV_TYPE_USBDISK);
            break;
#endif                

#if APP_SUPPORT_FM==1
        case RESULT_RADIO:
        	FM_Entry();
            break;
#endif

#if APP_SUPPORT_RECORD==1
        case RESULT_RECORD_START:
        case RESULT_RECORD_NOSTART:
        case RESULT_FMREC_START:
        case RESULT_FMREC_NOSTART:
            RECORD_Entry(result);
            break;
#endif

#if APP_SUPPORT_LINEIN==1
        case RESULT_LINE_IN:
            LINEIN_Entry(0);
            break;
#endif

#if APP_SUPPORT_USBDEVICE==1
        case RESULT_UDISK:
            USB_Entry(0);
            break;
#endif
#if APP_SUPPORT_LCD==1
        case RESULT_SYSTEM:
        	SET_Entry(0);
            break;
            
        case RESULT_ALARM:
        	TIMER_Alarm();
            break;
            
        case RESULT_CALENDAR:
        	CALENDAR_Entry(0);
        	break;
#endif
#if APP_SUPPORT_BLUETOOTH==1
	 case RESULT_BT:
	 case RESULT_BT_ACTIVE:
	 case RESULT_BT_PENDING:
        app_bt_entry();
		break;
#endif

#ifdef APP_SUPPORT_UART_PLAY
	case RESULT_UART_PLAY:
		UartPlayEntry(0);
		break;
#endif

#ifdef APP_SUPPORT_UART_REC
	case RESULT_UART_REC:
		UartRecEntry(0);
		break;
#endif
        default:
        	//Invaild module
        	APP_ASSERT(0);
            break;
    }

    g_last_reslt = result;
}

/******************************************************************************
 * AppDefaultMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppDefaultMsgHandler(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case EV_UI_FW_ON_RESUME:
        	app_trace(0, "Warning unhandled EV_UI_FW_ON_RESUME, param=%d", ev->nParam1);
        	break;

        case EV_UI_FW_SWITCH_MOD:
        	GobackAllHistory();

#if APP_SUPPORT_LED==1
            LED_SetPattern(GUI_LED_NONE, 1);
#endif
#if (APP_SUPPORT_MENU == 0)
			//It's needs to play ring before entry some mode while lcd is NOT present

        	if(ev->nParam1 == RESULT_MAIN) //Switch to next mode
        	{
        	    g_current_module = AppGetNextMode(g_current_module);
        	}
        	else
        	{
        	    g_current_module = ev->nParam1;
                g_play_disk = g_next_play_disk;
        	}

			ClearProtocolEventHandler(EV_AUDIO_INTERNAL_PLAY_FINISH_IND);
			media_StopInternalAudio();
            if(AppGetModeRing(g_current_module) != 0)
            {
            	SetProtocolEventHandler(AppPlayModeRingFinishHandler, EV_AUDIO_INTERNAL_PLAY_FINISH_IND);
                media_PlayInternalAudio(AppGetModeRing(g_current_module), 1, 0);
            }
            else
#else
            g_current_module = ev->nParam1;
#endif
            {
                AppModSwitchHandler(g_current_module);
            }
            break;

#ifdef __AT_MOD_COMMON__
		case AT_COMMON_PLAYTONE:
			GUI_DisplayMessageTone(0, 0, 0, ev->nParam1, GUI_MSG_FLAG_WAIT);
			break;
#endif /* __AT_MOD_COMMON__ */


        case EV_PM_BC_IND:
            app_trace(0, "EV_PM_BC_IND bcl=%d,bcs=%d", LOUINT16(ev->nParam1), HIUINT16(ev->nParam1));

            if(HIUINT16(ev->nParam1) > PM_CHARGER_DISCONNECTED)
            {
#if APP_SUPPORT_CHARGING==1 
                //Switch off to enter Charge plugin
                DM_DeviceSwithOff();
#endif
            }
            break;
        
        default:
			if(!MESSAGE_Hotkey(MESSAGE_ConvertToKeyValue(ev)))
        	{
        		return FALSE;
        	}
        	break;
    }

    return TRUE;
}

