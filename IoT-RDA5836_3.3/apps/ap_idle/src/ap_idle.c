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

#if APP_SUPPORT_RGBLCD==1
//HH:MM
#define IDLE_SCREEN_TIME1_X	(24)
#define IDLE_SCREEN_TIME1_Y	(48)
//HH:MM:SS
#define IDLE_SCREEN_TIME2_X	(24)
#define IDLE_SCREEN_TIME2_Y	(48)

#define IDLE_SCREEN_IMG_X	(2)
#define IDLE_SCREEN_IMG_Y	(48)

#elif APP_SUPPORT_LCD==1
//HH:MM
#define IDLE_SCREEN_TIME1_X	(24)
#define IDLE_SCREEN_TIME1_Y	(16)
//HH:MM:SS
#define IDLE_SCREEN_TIME2_X	(4)
#define IDLE_SCREEN_TIME2_Y	(16)

#define IDLE_SCREEN_IMG_X	(2)
#define IDLE_SCREEN_IMG_Y	(70)

#endif


extern BOOL ui_auto_update;
extern INT8 curr_active_alarm;

void IdleRedraw(void)
{
    IDLE_SCREEN_PTR idle_ptr = (IDLE_SCREEN_PTR) GetHistoryGuiBuffer(SCR_ID_IDLE);
    TM_SYSTEMTIME systemTime;
    INT32 time;

#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    if(AP_Support_LCD())
    {
        TM_GetSystemTime(&systemTime);
        if(idle_ptr->sleep ==1)
            time = systemTime.uHour*60 + systemTime.uMinute;
        else
            time = systemTime.uHour*3600 + systemTime.uMinute * 60 + systemTime.uSecond;
        if(idle_ptr->last_time != time)
        {
            GUI_ClearScreen(NULL);//清全屏幕
            GUI_DisplayBattaryLever();
            if(idle_ptr->sleep ==1)
                GUI_DisplayTime(IDLE_SCREEN_TIME1_X, IDLE_SCREEN_TIME1_Y, GUI_TIME_DISPLAY_HOUR_MINUTE, GUI_IMG_BIG_NUM, time*60*1000);
            else
                GUI_DisplayTime(IDLE_SCREEN_TIME2_X, IDLE_SCREEN_TIME2_Y, GUI_TIME_DISPLAY_ALL, GUI_IMG_BIG_NUM, time*1000);
#if APP_SUPPORT_USBSTORAGE_BG==1
            if(GetUsbCableStatus())
                GUI_ResShowImage(GUI_IMG_DEVICE,IDLE_SCREEN_IMG_X,IDLE_SCREEN_IMG_Y,0);
#endif
            if(curr_active_alarm>=0)
                GUI_ResShowImage(GUI_IMG_MENU_SETUP,3,100,0);
            //BT_DisplayIcon(88, 0);
            GUI_UpdateScreen(NULL);
            idle_ptr->last_time = time;
        }
    }
#elif APP_SUPPORT_LED8S==1
    {
        TM_GetSystemTime(&systemTime);
        time = systemTime.uHour*3600 + systemTime.uMinute * 60 + systemTime.uSecond;
        GUI_ClearScreen(NULL);
        GUI_ResShowPic(GUI_ICON_COLON,0,0);
        GUI_DisplayTimeLEDs(time*1000);

        GUI_UpdateScreen(NULL);
    }
#endif
}

BOOL AppIdleMsgHandler(COS_EVENT *pEvent)
{
    IDLE_SCREEN_PTR idle_ptr = (IDLE_SCREEN_PTR) GetHistoryGuiBuffer(SCR_ID_IDLE);

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            ui_auto_update = FALSE;
            idle_ptr->last_time = 0;
            idle_ptr->sleep = 0;
            MESSAGE_Initial(g_comval);
#if APP_SUPPORT_LED==1
            LED_SetPattern(GUI_LED_IDLE, LED_LOOP_INFINITE);
#endif
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            IdleRedraw();
            break;

        case EV_UI_FW_ON_EXIT:
            break;
            
        case EV_UI_FW_ON_RESUME:
            idle_ptr->sleep = 2;
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_KEY_UP:
        	GobackScreen(0, 0);
        	break;
        	
        default:
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_WAIT_TIMEOUT:
                    if(!IsChargePlugIn())
                    {
                        APP_Sleep();
                        idle_ptr->sleep = 1;
                    }
                    else
                    {
                        app_trace(APP_MAIN_TRC, "Can't sleep for charging");
                        idle_ptr->sleep = 2;
                    }
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                    break;
                case AP_MSG_RTC:
                    if(IsChargePlugIn())
                    {
                        if(idle_ptr->sleep==1)
                        {
                            app_trace(APP_MAIN_TRC, "Wakeup for charging");
                            APP_Wakeup();
                            idle_ptr->sleep = 2;
                        }
                        else
                        {
                        	app_trace(APP_MAIN_TRC, "Can't sleep for charging");
                        }
                    }
                    else if(idle_ptr->sleep==2)
                    {
                        app_trace(APP_MAIN_TRC, "Sleep for charging end");

                        APP_Sleep();
                        idle_ptr->sleep = 1;
                    }

                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                    break;

                case AP_KEY_MODE|AP_KEY_HOLD:
                    if(MESSAGE_IsHold())
                    {
                        SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_STANDBY);
                    }
                    break;
                case AP_KEY_POWER|AP_KEY_PRESS:
                    SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_STANDBY);
                    break;

                default:
                	return FALSE;
            }
            break;
    }

    return TRUE;
}
/*********************************************************************************
* Description : IDLE 函数，在该模式下面会省电，但是希望能显示电子时钟的功能
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 IDLE_Entry(INT32 param)
{
    IDLE_SCREEN_PTR idle_ptr = NULL;

    idle_ptr = (IDLE_SCREEN_PTR) COS_MALLOC(sizeof(IDLE_SCREEN_T));

    EntryNewScreen(SCR_ID_IDLE, AppIdleMsgHandler, idle_ptr, 0, 0);

    return 0;
}







