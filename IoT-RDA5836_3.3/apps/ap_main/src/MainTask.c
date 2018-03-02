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
#include "ap_common.h"
#include "ap_calendar.h"
#include "ap_idle.h"
#include "ap_linein.h"
#include "ap_record.h"
#include "ap_setting.h"
#include "ap_fm.h"
#include "ap_music.h"
#include "ap_usbdisk.h"
#include "ap_charging.h"
#include "tm.h"
#include "MainTask.h"
#include "event.h"
#include "gpio_edrv.h"
#include "bt_msg.h"
#include "ap_mgr.h"
#include "gui.h"
#include "iot_bt.h"
#ifdef __AT_SUPPORT__
#include "at_sa.h"
#endif
#if defined(__ZBAR_SUPPORT__)
#include "ap_camera.h"
#endif /* __ZBAR_SUPPORT__ */
#include "dm.h"
#include "gpio_edrv.h"
#ifndef WIN32
#include "tsd_m.h"
#endif

//#include "hal_timers.h"
//#include "hal_gpio.h"

#define READ_BUF_MAX          128
#define MAIN_ITEMCOUNT        5  /*Main Menu Nums*/
#define MAX_BOOT_LOGO_PICS    6  /*BootLogo Nums Used by User*/

#define UPDATE_TFLASH_CFG_NAME  L"update.cfg"


static const UINT16 boot_logo_pic[MAX_BOOT_LOGO_PICS]= {GUI_IMG_LOGO,GUI_IMG_LOGO,GUI_IMG_LOGO,GUI_IMG_LOGO,GUI_IMG_LOGO,GUI_IMG_LOGO};

///////////////////////////////////////////////////////////////////////////////////
extern comval_t         *g_comval; //系统总的环境变量
slider_t         slider = {0};
//function_menu_t  menu;
//progress_t       progress;
sysconfig_t      g_sysconfig = {0};
displayconfig_t  g_displayconfig = {0};
#if 1
#if APP_SUPPORT_BLUETOOTH==1
btconfig_t       g_btconfig = {{0},{0},0,{0}};
#endif
#endif
INT32           g_last_reslt = -1;
INT32           g_current_module = 0;
INT8           g_play_disk = FS_DEV_TYPE_INVALID;
INT8           g_next_play_disk = FS_DEV_TYPE_INVALID;

#if APP_SUPPORT_HID_MODE==1
UINT8           g_current_BT_module=BT_MODE_HEADSET;
#endif
static UINT32 g_mmi_poweron_cause = 0;
//////////////////////////////////////////////////////////////////////////////////

//add end
extern BOOL ui_auto_update;
extern BOOL ui_auto_select;
extern BOOL g_test_mode;
UINT32 event_detected_displaying_log;
extern UINT8 g_light_time;
extern UINT16 g_standby_time;
#if (APP_SUPPORT_POWERKEY == 1)
extern volatile UINT8 tsd_PowerOff;
#endif
void APP_Test_Mode(void);
int Mcd_USBHostInit();
extern BOOL boot_get_testmode(VOID); 
extern void boot_set_testmode(BOOL testmode); 

#if defined(__AT_SUPPORT__)
extern BOOL BAL_ATInit(VOID);
extern BOOL AT_AsyncEventProcess(COS_EVENT* pEvent);
extern VOID SA_UartRecevDataInd(COS_EVENT* pEvent);
#endif /* __AT_SUPPORT__ */

BOOL AppDefaultMsgHandler(COS_EVENT *ev);
BOOL ExecuteCurrProtocolHandler(COS_EVENT *ev);
void InitProtocolEvent(void);
BOOL AppMsgPreHandler(COS_EVENT *ev);
extern BOOL DSM_UpgradeStructGetValid(VOID);
extern VOID DSM_TFlash_Upgrade(INT32 disk, INT32 file);

void DSM_EnqNvram(void)
{
	UINT16 *p_UserData = NULL;
	p_UserData =(UINT16 *)NVRAMGetData(0x80, 6);
	p_UserData =(UINT16 *)NVRAMGetData(VM_SYSTEM,sizeof(comval_t));
    #if APP_SUPPORT_BLUETOOTH==1
	p_UserData =(UINT16 *)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
    #endif
	p_UserData =(UINT16 *)NVRAMGetData(VM_AP_MUSIC,sizeof(music_vars_t));
	p_UserData =(UINT16 *)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));
}
/*********************************************************************************
* Description : 初始化系统设置变量
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
void APP_ReadComPara(void)
{
    extern const UINT8 g_LanguageList[];
 
    //DSM_ReadUserData();

    //NVRAMRead(g_comval, VM_SYSTEM, sizeof(g_comval));
    g_comval = (comval_t*)NVRAMGetData(VM_SYSTEM,sizeof(comval_t));
    hal_HstSendEvent(SYS_EVENT, 0x19880900);
    if(g_comval->magic != MAGIC_COMVAL)
    {
        hal_HstSendEvent(SYS_EVENT, 0x19880901);
        if(!LoadFactorySetting(g_comval, sizeof(g_comval)))
        {
            g_comval->DisplayContrast = 5;
            g_comval->langid    = g_LanguageList[0];
            g_comval->LightTime = 3;
            //g_comval->ReplayMode = 0;
            //g_comval->SleepTime = 0;
            g_comval->StandbyTime = 30;
            //g_comval->BatteryType = BATT_TYPE_ALKALINE;
            //g_comval->FMBuildInFlag = 0;
            //g_comval->RecordType = 0;
            //g_comval->BLightColor = 3;
            g_comval->BackLight = 5;
            g_comval->Volume = MIN(8, AUD_MAX_LEVEL); // default volume

            g_comval->music_cfg.loop_mode = 0;
            g_comval->music_cfg.eq_mode = 0;
            g_comval->music_cfg.repeat_count = 3;
            g_comval->music_cfg.repeat_time = 30;

#if APP_SUPPORT_FM==1           /*Surport fm*/
            g_comval->fm_value.fm_band = 0;
            g_comval->fm_value.fm_sendfreq = 1000;
#endif
        }
        hal_HstSendEvent(SYS_EVENT, 0x19880902);
        g_comval->magic     = MAGIC_COMVAL;
        // no need save data
        //NVRAMWrite(g_comval, VM_SYSTEM,sizeof(g_comval));

        //g_test_mode = 1;
    }

#ifdef ROTARY_SWITCH_USED
    g_comval->Volume = 1; // default volume
#endif

    if(!ReadConfigData(&g_sysconfig, CFG_SYSTEM, sizeof(g_sysconfig)))
    {
        g_sysconfig.AppSupportFlag = AP_SUPPORT_RECORD|(AP_SUPPORT_FM*APP_SUPPORT_FM)|(AP_SUPPORT_FMREC*APP_SUPPORT_FM_RECORD)|(AP_SUPPORT_LCD*APP_SUPPORT_LCD)|(AP_SUPPORT_MENU*APP_SUPPORT_MENU)|(AP_SUPPORT_LED*APP_SUPPORT_LED);
    }
#if APP_SUPPORT_RGBLCD==1
    if(AP_Support_LCD() && !ReadConfigData(&g_displayconfig, CFG_DISPLAY, sizeof(g_displayconfig)))
    {
        g_displayconfig.lcd_heigth = 128;
        g_displayconfig.lcd_width = 160;
        //g_displayconfig.log_x = 0;
        //g_displayconfig.log_y = 8;
        //g_displayconfig.usb_x = 0;
        //g_displayconfig.usb_y = 16;

        g_displayconfig.big_num_width = 16;
        g_displayconfig.big_colon_width = 9;
        g_displayconfig.small_num_width = 6;
        g_displayconfig.small_colon_width = 4;

        //g_displayconfig.num_key_img = GUI_IMG_BIG_NUM;
        //g_displayconfig.num_key_x = 32;
        //g_displayconfig.num_key_y = 16;

        g_displayconfig.position_batt = POSITION_BATT;
        g_displayconfig.message_y = 48;
    }
#elif APP_SUPPORT_LCD==1
    if(AP_Support_LCD() && !ReadConfigData(&g_displayconfig, CFG_DISPLAY, sizeof(g_displayconfig)))
    {
        g_displayconfig.lcd_heigth = 64;
        g_displayconfig.lcd_width = 128;
        //g_displayconfig.log_x = 0;
        //g_displayconfig.log_y = 8;
        //g_displayconfig.usb_x = 0;
        //g_displayconfig.usb_y = 16;

        g_displayconfig.big_num_width = 16;
        g_displayconfig.big_colon_width = 9;
        g_displayconfig.small_num_width = 6;
        g_displayconfig.small_colon_width = 4;

        //g_displayconfig.num_key_img = GUI_IMG_BIG_NUM;
        //g_displayconfig.num_key_x = 32;
        //g_displayconfig.num_key_y = 16;

        g_displayconfig.position_batt = POSITION_BATT;
        g_displayconfig.message_y = 24;
    }
#endif
#if 1
#if APP_SUPPORT_BLUETOOTH==1     /*Surport bt*/
    if(!ReadConfigData(&g_btconfig, CFG_BLUETOOTH, sizeof(g_btconfig)))
    {
        strcpy(g_btconfig.local_name, "RDA MP3 BT"); // bluetooth name
        strcpy(g_btconfig.password, "0000");         // bluetooth password
    }
#endif
#endif
    //SetBackLightColor(g_comval->BLightColor);        //设置背光颜色。

    SetBackLight(g_comval->BackLight);

    SetContrast(g_comval->DisplayContrast);           //设置屏幕的对比度

    g_light_time   = g_comval->LightTime * 2;
    g_standby_time = g_comval->StandbyTime * 60;//comval->StandbyTime * 60 * 2;

}

BOOL APP_StandBy_MsgHandler(COS_EVENT *pEvent)
{
    static BOOL is_play_finished = FALSE;
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            g_light_time = 0;
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            GUI_DisplayMessage(0, GUI_STR_STANDBY, NULL, GUI_MSG_FLAG_WAIT);

            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);//清全屏幕
                GUI_UpdateScreen(NULL);
                CloseBacklight();
            }
            ui_auto_update=FALSE;
#endif
            is_play_finished = FALSE;
            media_PlayInternalAudio(GUI_AUDIO_POWEROFF, 1, 0);
	    #if APP_SUPPORT_LED==1
           LED_SetPattern(GUI_LED_POWEROFF, 1);
           #endif 

            break;
			
		case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
			  hal_HstSendEvent(BOOT_EVENT, 0X66666668);
			  #if (APP_SUPPORT_POWERKEY ==1)
			  if(tsd_PowerOff == 0xaa)
			  {
			          hal_HstSendEvent(BOOT_EVENT, 0X66666669);
				      pmd_SetPowerMode(4);
				      hal_XCpuEnterSleep();
			         
			  }
			  #endif
			  if(MESSAGE_IsHold())
			  {
				  hal_HstSendEvent(APP_EVENT,0x55555550);
				  app_trace(APP_MAIN_TRC, "APP_StandBy call DM_DeviceSwithOff");
				  DM_DeviceSwithOff();
			  }
			  else
			  {
				  hal_HstSendEvent(APP_EVENT,0x55555551);
				  is_play_finished = TRUE;//Wait key up
			  }
			  break;

        case EV_UI_FW_ON_EXIT:
            APP_Wakeup();
            if(g_comval->LightTime != 0)
            {
                g_light_time   = g_comval->LightTime * 2;
                OpenBacklight();
            }
            break;
/*
        case AP_MSG_RTC:
            if(GetBattery()>=0)
            {
                app_trace(APP_MAIN_TRC, "APP_StandBy call DM_DeviceSwithOff");
                DM_DeviceSwithOff();
            }
            break;
*/
        case AP_KEY_UP:
           if((tgt_GetKeyCode(0) == pEvent->nParam1)&& is_play_finished)
            {
                hal_HstSendEvent(APP_EVENT,0x55555552);
                app_trace(APP_MAIN_TRC, "APP_StandBy call DM_DeviceSwithOff");
                DM_DeviceSwithOff();
            }
            break;
/*
        case AP_KEY_LONG:
            if(KEY_PLAY== pEvent->nParam1)
            {
                AppGobackScreen();
                SendAppEvent(EV_DM_POWER_ON_IND, 0);
            }
            break;
*/
        case AP_MSG_ALARM:
            AppGobackScreen();
            SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_ALARM);
            break;

        default:
            break;
    }

    return TRUE;
}
INT32 APP_StandBy(void)
{
    app_trace(APP_MAIN_TRC, "====APP_StandBy====");

    if(IsChargePlugIn())
    {
    #if APP_SUPPORT_CHARGING==1
        Charging_Entry();
    #endif 
    }
    else
    {
        EntryNewScreen(SCR_ID_STANDBY, APP_StandBy_MsgHandler, NULL, 0, 0);
    }

    return 0;
}

#if APP_SUPPORT_CALIB_KEY==1
/******************************************************************************
 * APP_CalibKey_MsgHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL APP_CalibKey_MsgHandler(COS_EVENT *pEvent)
{
	static UINT8 calib_key_step;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        	calib_key_step = 0;
        	SendAppEvent(EV_UI_FW_REDRAW, 0);
        	break;

        case EV_UI_FW_REDRAW:
#if defined(_USED_TSC_KEY_)
        	tsd_Key_Calib(calib_key_step); // start
#endif /* _USED_TSC_KEY_ */
        	GUI_DisplayMessage(0, GUI_STR_CALIBKEY + calib_key_step, 
        			NULL, GUI_MSG_FLAG_DISPLAY);
            break;

        case EV_UI_FW_ON_EXIT:
            break;

        case AP_KEY_UP:
	        calib_key_step++;

	        if(calib_key_step < 2)
	        {
#if APP_SUPPORT_LED==1
			    LED_SetPattern(GUI_LED_CALIB_KEY1 + calib_key_step, LED_LOOP_INFINITE);
#endif 
				SendAppEvent(EV_UI_FW_REDRAW, 0);
			}
			else
			{
#if defined(_USED_TSC_KEY_)
			    if(tsd_Key_Calib(2) == 0)// last key press
#else
				if(TRUE)
#endif /* _USED_TSC_KEY_ */
			    {
#if APP_SUPPORT_LED==1
			        LED_SetPattern(GUI_LED_NONE, 0);
#endif
			        GUI_DisplayMessageWithCB(0, GUI_STR_CALIBKEY_SUCC, NULL, AppGobackScreen, GUI_MSG_FLAG_WAIT);
			    }
			    else
			    {
			        GUI_DisplayMessageWithCB(0, GUI_STR_CALIBKEY_FAIL, NULL, AppGobackScreen, GUI_MSG_FLAG_WAIT);
			    }
			}
    
            break;

        default:
            return TRUE;//return TRUE to distory any other events here.
    }

    return TRUE;
}
/******************************************************************************
 * APP_CalibKey: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
INT32 APP_CalibKey(void)
{
    app_trace(APP_MAIN_TRC, "====APP_CalibKey====");
	
    EntryNewScreen(SCR_ID_CALIB_KEY, APP_CalibKey_MsgHandler, NULL, 0, 0);
    return 0;
}
#endif

/*********************************************************************************
* Description : 显示系统开机动画
*
* Arguments   :
*
*
* Notes       :
*
*********************************************************************************/
void APP_DisplayLogo_TimeOutHandler(void *param)
{
    static UINT16 index = 0;

    app_trace(APP_MAIN_TRC, "APP_DisplaySysLogo,index=%d,time=%d,count=%d",
              index, g_displayconfig.log_image_time,g_displayconfig.log_image_count);

    if(index >= g_displayconfig.log_image_count)
    {
        GobackAllHistory();
        index = 0;
        return;
    }
    else
    {
        mmi_timer_create(g_displayconfig.log_image_time*5, APP_DisplayLogo_TimeOutHandler,NULL,COS_TIMER_MODE_SINGLE);
    }

#if APP_SUPPORT_RGBLCD==1
    if(AP_Support_LCD())
    {
        GUI_ClearScreen(NULL);//清全屏幕
        GUI_ResShowImage(GUI_IMG_LOGO,index,0,40);//g_displayconfig.log_x, g_displayconfig.log_y);/*BootLogo*/
        GUI_UpdateScreen(NULL);
    }
#elif APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        GUI_ClearScreen(NULL);//清全屏幕
        GUI_ResShowImage(GUI_IMG_LOGO,index,0,8);//g_displayconfig.log_x, g_displayconfig.log_y);/*BootLogo*/
        GUI_UpdateScreen(NULL);
    }
#elif APP_SUPPORT_LED8S==1
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(0xffff,0,0);
    GUI_DisplayText(0,0,"8888");
    GUI_UpdateScreen(NULL);
#endif

    index++;

}
/******************************************************************************
 * AppExitPowerOnScreen:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
UINT8  tflash_poweron_flag1=0;
void AppExitPowerOnScreen(void)
{
    INT32  result = 0;

    if (g_comval->LightTime == 0)            //每次开机判断一次！解决Bug Report:27
        CloseBacklight();
    else
        OpenBacklight();

    //Go to first app after power on
    if(gpio_detect_linein() == GPIO_DETECTED)
    {
        result = RESULT_LINE_IN;
    }
#if APP_SUPPORT_USB==1
    else if(GetUsbCableStatus())
    {
        result = RESULT_UDISK;
    }
#endif
    else if(g_last_reslt > 0)
    {
        result = g_last_reslt;
    }
    else
    {
#ifndef BT_SCO_RECORD_SUPPORT
        if(gpio_CardDetectPowerOn() == 1)
        {
            tflash_poweron_flag1 = 1;
            result = RESULT_MUSIC;
            g_next_play_disk = FS_DEV_TYPE_TFLASH;
        }
        else
#endif
        {
            result = RESULT_MAIN;
        }
    }
#if APP_SUPPORT_TEST_BOX == 1
    result = RESULT_BT;
#endif
    SendAppEvent(EV_UI_FW_SWITCH_MOD, result);

}

/******************************************************************************
 * AppPowerOnMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppPowerOnMsgHandler(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);
                GUI_UpdateScreen(NULL);
            }
#if APP_SUPPORT_LED==1
            LED_SetPattern(GUI_LED_POWERON, 1);
#endif 
            media_PlayInternalAudio(GUI_AUDIO_POWERON, 1, 0);  
            break;

        case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
        case EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND:
        case EV_UI_FW_ON_RESUME:
            if((g_displayconfig.log_image_time | g_displayconfig.log_image_count) == 0)
            {
                //mmi_timer_create(1, GobackAllHistory,NULL,COS_TIMER_MODE_SINGLE);
                GobackAllHistory();
            }
            else
            {
                APP_DisplayLogo_TimeOutHandler(NULL);
            }
            break;

        case EV_UI_FW_ON_EXIT:
            AppExitPowerOnScreen();
            break;
            
#if APP_SUPPORT_TEST_BOX==0
#if APP_SUPPORT_MUSIC==1
        case EV_FS_TFLASH_DETECT:
            if(pEvent->nParam1)
            {
                g_next_play_disk = FS_DEV_TYPE_TFLASH;
                g_last_reslt = RESULT_MUSIC;
            }
            break;

#if APP_SUPPORT_USB==1
        case EV_FS_UDISK_READY:
            g_next_play_disk = FS_DEV_TYPE_USBDISK;
            g_last_reslt = RESULT_MUSIC;
            break;
#endif
#endif
#endif
#if (APP_SUPPORT_POWERKEY ==1)	
       case AP_MSG_POWER_KEY:        
            hal_HstSendEvent(BOOT_EVENT, 0X66666661);
            g_last_reslt = RESULT_STANDBY;
            tsd_PowerOff =0xaa;
            break;
#endif

        default:
            //Stop hot key handler when power on screen
            break;
    }

    return TRUE;
}

void AppPowerOnEntry(void)
{
    app_trace(APP_MAIN_TRC, "====AppPowerOnEntry====");
    EntryNewScreen(SCR_ID_POWER_ON, AppPowerOnMsgHandler, NULL, 0, 0);
}

#if (APP_SUPPORT_MENU == 1)
INT32 main_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
        return g_last_reslt;
    return FALSE;
}

/******************************************************************************
 * AppMainMenuMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppMainMenuMsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            if(AP_Support_MENU())
            {
                ui_auto_select = FALSE;
                GUI_Display_Menu(GUI_MENU_MAIN, main_menu_callback);
                //g_last_reslt        = RESULT_NULL;
            }
            break;

        case EV_UI_FW_ON_RESUME:
            if(pEvent->nParam1 == RESULT_TIMEOUT || pEvent->nParam1 == RESULT_IGNORE)
            {
                //SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_IDLE);// RESULT_CALENDAR;
                IDLE_Entry(0);
            }
            else
            {
                SendAppEvent(EV_UI_FW_REDRAW, 0);
            }
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
            break;

        default:
            return FALSE;
    }

    return TRUE;
}


/******************************************************************************
 * MainMenu_Entry:
 * DESCRIPTION: -
 * Main menu should be ROOT screen always
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void MainMenu_Entry(void)
{
    app_trace(APP_MAIN_TRC, "====MainMenu_Entry====");

	DeleteScreenIfPresent(SCR_ID_MAIN_MENU);
	
    EntryNewScreen(SCR_ID_MAIN_MENU, AppMainMenuMsgHandler, NULL, 0, 0);
}
#endif

/******************************************************************************
 * BAL_MainTaskEntry:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
extern void RecordCurrKeyHandler(U16 keyCode, U16 keyType);
#endif
u_int8 *spp_buf = "spp data test";
TASK_ENTRY BAL_MainTaskEntry(void *pData)
{
#if 0
        u_int8 iot_ret = 0;
        COS_EVENT ev;
        
        iot_ret = bt_Open(6000);
        bt_SppAccept();
        while(1)
        {
                if( 0 == bt_SppRecv(6000))
                        bt_SppSend(spp_buf, strlen(spp_buf), 6000);
        }
#else
    TM_SYSTEMTIME systime;
    COS_EVENT ev;

    // dump version and date
    hal_HstSendEvent(SYS_EVENT,0x11223344);
    //hal_HstSendEvent(SYS_EVENT,GetPlatformVersion());
    //hal_HstSendEvent(SYS_EVENT,GetPlatformRevision());
    //hal_HstSendEvent(SYS_EVENT,GetPlatformBuildDate());
    //hal_HstSendEvent(SYS_EVENT,AP_GetVersion());
    hal_HstSendEvent(SYS_EVENT,AP_GetBuildDate());
    hal_HstSendEvent(SYS_EVENT,AP_GetBuildTime());

    APP_ReadComPara();  //读取系统的全局变量。可以放在os初始化的过程中做
    InitHistory();
    InitProtocolEvent();
#if !defined(__DISABLE__MMI_TIMER__)
    mmi_timer_initialise();
#endif /* __DISABLE__MMI_TIMER__ */


    gui_load_resource(g_comval->langid);

#if APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        lcdd_Open();
        GUI_Initialise(g_displayconfig.lcd_width, g_displayconfig.lcd_heigth);
        GUI_SetTextColor(g_displayconfig.font_color);
        GUI_SetBackColor(g_displayconfig.back_color);
    }
#endif

#if OS_KERNEL == SXR
    if(boot_get_testmode())
    {
        NVRAMWriteData();
        APP_Test_Mode();
    }
#endif
    AP_GetBuildDate(); // for link build date time info

    // check systemtime
    TM_GetSystemTime(&systime);
    if(systime.uHour>=24 || systime.uDay ==0 || systime.uYear >2050) // invalid date time
    {
        systime.uYear = 2012;
        systime.uMonth = 8;
        systime.uDay = 15;
        systime.uHour = 12;
        systime.uMinute = 30;
        TM_SetSystemTime(&systime);
    }
#if APP_SUPPORT_LCD==1
    TIMER_SetAlarm(1);
#endif

    hal_HstSendEvent(SYS_EVENT, 0x09250001);
#if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_NONE, 1);
#endif

    hal_HstSendEvent(APP_EVENT, 0x09558000);
#ifndef WIN32
#ifdef SERVICE_BASE_FS_SUPPORT
    if(DSM_UpgradeStructGetValid())
        DSM_TFlash_Upgrade(0,0);
#endif
#endif
    //SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_BT);
    while(1)
    {
        COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
	 
        //Ignore some ev
        //if(!(ev.nEventId == AP_MSG_RTC || ev.nEventId == EV_UI_FW_REDRAW || ev.nEventId == EV_UI_FW_TIMER_EXPIRED))
        //    app_trace(APP_MAIN_TRC, "MainTask ev.id=%d, param=%d", ev.nEventId, ev.nParam1);
        if(AppMsgPreHandler(&ev))
            continue;
		
        switch(ev.nEventId)
        {
            case EV_DM_POWER_ON_IND:
            {
                //Display power on screen
                g_mmi_poweron_cause = ev.nParam1;
                app_trace(APP_MAIN_TRC, "MainTask PWRON cause=%d", ev.nParam1);
#if APP_SUPPORT_CHARGING==1 
                if(g_mmi_poweron_cause & DM_POWRN_ON_CAUSE_CHARGE)
                {
                    Charging_Entry();
                }
                else
#endif
                {
                    AppPowerOnEntry();
                }

#ifdef GSM_SUPPORT
                {
                    UINT32 ret;
                    ret = CFW_ShellControl(0);
                    app_trace(APP_MAIN_TRC, "CFW_ShellControl ret= %d", ret);
                }
#endif
            }
                break;

#if APP_SUPPORT_CALIB_KEY==1
            case AP_MSG_KEY_CALIB:
                APP_CalibKey();
                break;
#endif

#if defined(__AT_SUPPORT__)
            case EV_DM_UART_RECEIVE_DATA_IND:
                SA_UartRecevDataInd(&ev);
                break;
#endif /* __AT_SUPPORT__ */

#if defined(__ZBAR_SUPPORT__)
            case EV_CAMERA_GET_QRCODE_DATA_IND:
                {
                    UINT8 *buffer = (UINT8 *) ev.nParam1;
                    UINT16 len = ev.nParam2;

                    app_trace(APP_MAIN_TRC, "QRD_IND=%s,len=%d", buffer, len);
                    IPCamera_API_Qrdec_Handler(buffer, len);
                    
                    COS_FREE(buffer);
                }
                break;
#endif /* __ZBAR_SUPPORT__ */

            case EV_TIMER:
            case EV_UI_FW_TIMER_EXPIRED://mmi clock
                mmi_handle_expired_timers(ev.nParam1);
                break;

            default:
#if 0 // move T_card update to music module
//#if APP_SUPPORT_LCD==0  //added for T_card update without LCD. Search TF card, if there is a file with the name "update.cfg", then do the update.
                if(aud_isIdle())
                {
                    if((ev.nEventId == AP_MSG_USB_PLUGIN) || (ev.nEventId == EV_FS_TFLASH_DETECT&&ev.nParam1 == 1))
                    {
                        BOOL card_ok = FALSE;
                        UINT8 disk;
                        INT32 file = -1;
                        if(ev.nEventId == AP_MSG_USB_PLUGIN)
                        {
                            card_ok=MountDisk(FS_DEV_TYPE_USBDISK);
                            disk = FS_DEV_TYPE_USBDISK;
                        }
                        if(ev.nEventId == EV_FS_TFLASH_DETECT)
                        {
                            card_ok=MountDisk(FS_DEV_TYPE_TFLASH);
                            disk = FS_DEV_TYPE_TFLASH;
                        }
                        hal_HstSendEvent(APP_EVENT, 0x92ade000);
                        hal_HstSendEvent(APP_EVENT, card_ok);
                        hal_HstSendEvent(APP_EVENT, disk);
                        if(card_ok)
                        {
                            file=FS_Open((PCSTR)UPDATE_TFLASH_CFG_NAME, FS_O_RDONLY, 0);
                            hal_HstSendEvent(APP_EVENT, file);
                            if(file >=0)
                            {
#ifndef WIN32
                                DSM_TFlash_Upgrade(disk, file);
#endif
                            }
                        }
                    }
                }
#endif
#if APP_SUPPORT_BLUETOOTH==1
                app_bt_msg_handle(&ev);
#endif
		
                if(AppCurMsgHandler)
                {
                    if(AppCurMsgHandler(&ev))
                    {
                        break;
                    }
                }
                else
                {
                    ;
                }

                if(0)
                {
                }
#if 0//defined(__SLIMMMI_TCPIP__)
                else if(app_SocketEventcb(&ev))
                {
                }
#endif /* __SLIMMMI_TCPIP__ */
#if defined(__AT_SUPPORT__)
                else if(AT_AsyncEventProcess(&ev))
                {
                }
#endif /* __AT_SUPPORT__ */
                else if(ExecuteCurrProtocolHandler(&ev))
                {
                }
                else if(AppDefaultMsgHandler(&ev))
                {
                }
                else
                {
                    //Ignore some ev
                    //if(!(ev.nEventId == AP_MSG_RTC || ev.nEventId == EV_UI_FW_REDRAW))
                    //    app_trace(APP_MAIN_TRC, "Ignore event:%d, nparam=%d", ev.nEventId, ev.nParam1);
                }

                break;
        }

//Monkey only in debug version
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)&& (!defined(CT_NO_DEBUG))
		RecordCurrKeyHandler(ev.nParam1,ev.nEventId);
#endif
	}
#endif
}







