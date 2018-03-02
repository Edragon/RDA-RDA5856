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

#include "ap_fm.h"
#include "ap_gui.h"
#include "tm.h"
#include "hal_overlay.h"
#include "ap_common.h"
#include "hal_debug.h"
#include "hal_host.h"

#if APP_SUPPORT_FM

#define FM_TX_TIMEOUT               (3000)

#define FM_AUTO_SEARCH              AP_MSG_USER_FM+1
#define FM_SAVE_FREQ                AP_MSG_USER_FM+2
#define FM_DELETE_FREQ              AP_MSG_USER_FM+3
#define FM_DELALL_FREQ              AP_MSG_USER_FM+4
#define FM_SET_BAND                 AP_MSG_USER_FM+5
#define FM_FREQ_LIST                AP_MSG_USER_FM+6
#define FM_ADJUST_CLOCK             AP_MSG_USER_FM+7
#define FM_ADD_TIMER                AP_MSG_USER_FM+8

extern slider_t            slider;

extern BOOL ui_auto_update;
extern INT8 g_timer_setting_flag;


FM_play_status_t    *FMStatus;                //当前电台信息
FM_Search_status_t  *FmSearchPtr = NULL;

BOOL edit_station=FALSE;
BOOL output_flag=FALSE;


INT8 fm_item_buf[12];
static UINT32 current_time;

//当前状态
enum
{
    fm_close,
    fm_play,
    fm_search_one,
    fm_search_all,
    fm_output
}   g_fm_status = fm_close;

//标志：need_draw，重画 BandSwitch，频段切换 FirstWait，刚进AP
static BOOL need_draw=TRUE;

//电台频率对照表(注:频率范围87M-108M（US）或76M-90M（Japan）,以K为精度要三位数).
static const UINT32 freq_scope [FMD_BAND_QTY][2] = {{87000,108000},{76000,90000},{76000,108000},{65000,76000}};


#define FM_MIN_FREQ  (freq_scope[FMStatus->mode][0])
#define FM_MAX_FREQ  (freq_scope[FMStatus->mode][1])

#define SEARCH_TIME 5

#define FM_BACKUPDATA() NVRAMWriteData()
extern void GUI_InputNumber(UINT32 input_num, UINT16 max_len, void(*callbackFunc)(UINT32));
extern U16 GetCurrentScreenId(void);
extern BOOL mediaSendFMCommand(UINT8 cmd, UINT32 param);
/*********************************************************************************
* Description : 音量设置回调函数
*
* Arguments   :
        param:
            0, 开始时关标停在第0项
*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
void  FM_VolumeCb(INT32 Vol)
{
    FMStatus->volume= (FMD_ANA_LEVEL_T)Vol;
    AppFMDo(MC_SETVOLUME, FMStatus->volume);
}


INT32 fm_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
        return 0;

    switch(value)
    {
#if APP_SUPPORT_LCD
        case RESULT_FMREC_START:
            return g_timer_setting_flag==0;
            break;
#endif
        case FM_SAVE_FREQ:
            return FMStatus->station == 0;
            break;
        case FM_DELETE_FREQ:
            return FMStatus->station != 0;
            break;
        case FM_SET_BAND:
            return FMStatus->mode != param;
            break;
        case FM_FREQ_LIST:
            return FMStatus->tab_num > 0;
            break;
    }
    return 0;
}

INT32 fm_list_callback(INT32 type, INT32 value, INT8 **string)
{
    INT freq;

    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = fm_item_buf;
        freq = (FMStatus->tabs[value]+0x10000+49)/100;

        sprintf(fm_item_buf, "FM%d.%dMHz", freq/10, freq%10);
        return value+1 == FMStatus->station;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        FM_StationListOperator(MC_SETSTATION, value+1);
        return 0;
    }
    return 0;
}
#if APP_SUPPORT_REMOTECONTROL==1
/******************************************************************************
 * FMInputNumberCallback:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void FMInputNumberCallback(UINT32 num_input)
{
    if(num_input>0 && num_input <= FMStatus->tab_num)
    {
        FM_StationListOperator(MC_SETSTATION, num_input);
        AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
    }
    else if(num_input*100>=FM_MIN_FREQ && num_input*100 <= FM_MAX_FREQ)
    {
        FMStatus->freq = num_input*100;
        AppFMDo(MC_PLAY, 0);
        AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
    }
    SendAppEvent(EV_UI_FW_REDRAW, 0);
}
#endif

void FM_CheckParam(void)
{
    static BOOL FirstFlag = TRUE;

    INT32 k;

    if(FirstFlag)
    {
        FirstFlag = FALSE;        //FM BUFFER标志赋初值

        if(FMStatus->magic != MAGIC_FMRADIO)
        {
            for(k=0; k<20; k++)
            {
                FMStatus->tabs[k]=0;
            }

            FMStatus->magic =MAGIC_FMRADIO;
            FMStatus->mode  =g_comval->fm_value.fm_band;
            FMStatus->freq  =FM_MIN_FREQ;
            FMStatus->volume= g_comval->Volume;
            FMStatus->station=0;
            FMStatus->stereo_status=0;
            FMStatus->tab_num = 0;
        }
        
        if( (FMStatus->mode != FMD_BAND_US_EUROPE)
            && (FMStatus->mode != FMD_BAND_JAPAN)
            && (FMStatus->mode != FMD_BAND_WORLD)
            && (FMStatus->mode != FMD_BAND_EAST_EUROPE))
        {
            FMStatus->mode  = FMD_BAND_US_EUROPE; 
            g_comval->fm_value.fm_band =  FMStatus->mode;         
        }

        
    }
}

/******************************************************************************
 * FM_OnStart:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void FM_OnStart(void)
{
    app_trace(APP_FM_TRC, "FM_OnStart");

    if(FMStatus->tab_num == 0)
    {
        FMEntrySearchScreen(fm_search_all, UP);
    }
    else
    {
        AppFMDo(MC_PLAY,0);
        COS_Sleep(100);
        AppFMDo(MC_SETVOLUME,FMStatus->volume);
        AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
        SendAppEvent(EV_UI_FW_REDRAW, 0);
    }
}

void FM_SearchTimeout(void *param)
{
    UINT32 search_interval_time;
    UINT32 new_freq = FMStatus->freq;
    BOOL search_a_round = FALSE;

    if(NULL == FmSearchPtr)
        return;

    if(FmSearchPtr->search_direct==UP)
    {
        new_freq+=100;
    }
    else
    {
        new_freq-=100;
    }


    if(new_freq < FM_MIN_FREQ)
    {
        search_a_round = TRUE;
        new_freq = FM_MAX_FREQ;
    }
    else if(new_freq > FM_MAX_FREQ)
    {
        search_a_round = TRUE;
        new_freq = FM_MIN_FREQ;
    }


    if(search_a_round && (fm_search_all == FmSearchPtr->search_type))
    {
        if(GetCurrentScreenId() == SCR_ID_FM_SEARCH)
            GobackScreen(0, 0);
        else
            DeleteScreenIfPresent(SCR_ID_FM_SEARCH);

        return;
    }


    //new vaild freq
    FMStatus->freq = new_freq;

    if(mediaSendFMCommand(MC_SEARCH, FMStatus->freq))
    { 
        AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus); 
        AppFMDo(MC_SETVOLUME,FMStatus->volume);
        if(fm_search_all == FmSearchPtr->search_type)
        {
            if(FMStatus->tab_num < FM_TAB_NUMS)
            {
                FMStatus->tabs[FMStatus->tab_num] = FMStatus->freq - 0x10000;
                FmSearchPtr->rssi[FMStatus->tab_num] = FMStatus->rssi;
            }
            else
            {
                UINT16 i,temp = 0;
                UINT16 min_rssi = 0xff;
                for(i = 0; i < FM_TAB_NUMS; i++)
                {
                    if(FmSearchPtr->rssi[i] < min_rssi)
                    {
                        min_rssi = FmSearchPtr->rssi[i];
                        temp = i;
                    }
                }
                if(FMStatus->rssi > min_rssi)
                {
                    FMStatus->tabs[temp] = FMStatus->freq - 0x10000;
                    FmSearchPtr->rssi[temp] = FMStatus->rssi;
                }
            }
            FMStatus->tab_num ++;
            FMStatus->station = FMStatus->tab_num;
        }
        else
        {
            //search one available, exit
            if(GetCurrentScreenId() == SCR_ID_FM_SEARCH)
                GobackScreen(0, 0);
            else
                DeleteScreenIfPresent(SCR_ID_FM_SEARCH);

            return;
        }

        search_interval_time = SEARCH_TIME;
    }
    else
    {
        AppFMDo(MC_MUTE,0);
        search_interval_time = 1;
    }

    FmSearchPtr->search_timer = mmi_timer_create(search_interval_time, 
                                        FM_SearchTimeout,NULL,COS_TIMER_MODE_SINGLE);

    if(GetCurrentScreenId() == SCR_ID_FM_SEARCH)
        SendAppEvent(EV_UI_FW_REDRAW, 0);
}

/******************************************************************************
 * FMSearchScreenMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL FMSearchScreenMsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            if(fm_search_all == FmSearchPtr->search_type)
            {
                FMStatus->tab_num = 0;                 // 全自动搜索,从极值开始.
                FMStatus->freq = FM_MIN_FREQ;
            }
            else
            {
            }
            AppFMDo(MC_CLOSE,0);
            GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_FM_SEARCH, GUI_MSG_FLAG_WAIT);
            break;

        case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
        case EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND:
        case EV_UI_FW_ON_RESUME:
        	if(0 == FmSearchPtr->search_timer)
        	{
	            AppFMDo(MC_OPEN,0);
	            AppFMDo(MC_MUTE,0);
	            FmSearchPtr->search_timer = mmi_timer_create(SEARCH_TIME, FM_SearchTimeout,NULL,COS_TIMER_MODE_SINGLE);
			}
            break;

        case EV_UI_FW_REDRAW:
            FM_ShowFreqIn();
            break;

        case EV_UI_FW_ON_PAUSE:
            if(FmSearchPtr->search_timer != 0)
            {
        	    mmi_cancel_timer(FmSearchPtr->search_timer);
            	FmSearchPtr->search_timer = 0;
        	}
            AppFMDo(MC_MUTE,0);
            AppFMDo(MC_CLOSE,0);
            break;

        case EV_UI_FW_ON_EXIT:
            if(FmSearchPtr->search_timer != 0)
            {
        	    mmi_cancel_timer(FmSearchPtr->search_timer);
            	FmSearchPtr->search_timer = 0;
        	}
            AppFMDo(MC_CLOSE,0);


            if(fm_search_all == FmSearchPtr->search_type)
            {
                UINT16    temp, i,j;

                if(FMStatus->tab_num > 0)
                {
                    if(FMStatus->tab_num > FM_TAB_NUMS)
                    {
                        FMStatus->tab_num = FM_TAB_NUMS;

                        // sort tabs
                        for(i=0; i<FM_TAB_NUMS-1; i++)
                            for(j=i+1; j<FM_TAB_NUMS; j++)
                            {
                                if(FMStatus->tabs[i] > FMStatus->tabs[j])
                                {
                                    temp = FMStatus->tabs[i];
                                    FMStatus->tabs[i] = FMStatus->tabs[j];
                                    FMStatus->tabs[j] = temp;
                                }
                            }
                    }

                    FMStatus->station = 1;
                    FMStatus->freq=(FMStatus->tabs[0]) + 0x10000;

                }
                app_trace(APP_FM_TRC, "FM search complete, tab_num=%d, set freq=%d",FMStatus->tab_num, FMStatus->freq);
                FM_BACKUPDATA();
            }
            else
            {
            }



            COS_FREE(FmSearchPtr);
            FmSearchPtr = NULL;

            break;

#ifdef __AT_MOD_FM__
		case AT_FM_STOP_SEARCH:
			GobackScreen(0, 0);
			break;
#endif /* __AT_MOD_FM__ */

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_STANDBY:
                    //do nothing
                    break;
                case AP_KEY_PLAY | AP_KEY_PRESS:
                case AP_KEY_MODE | AP_KEY_PRESS:
                    GobackScreen(0, 0);
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
 * FMEntrySearchScreen:
 * DESCRIPTION: -
 *
 * Input:
 * type - search_all/search_one
 * direct - UP /DOWN
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void FMEntrySearchScreen(UINT8 type, UINT8 direct)
{
    app_trace(APP_FM_TRC, "====FMEntrySearchScreen====");

    APP_ASSERT(NULL == FmSearchPtr);

    g_fm_status = type;

    FmSearchPtr = (FM_Search_status_t *) COS_MALLOC(sizeof(FM_Search_status_t));
    memset(FmSearchPtr, 0, sizeof(FM_Search_status_t));

    FmSearchPtr->search_type = type;
    FmSearchPtr->search_direct = direct;


    EntryNewScreen(SCR_ID_FM_SEARCH, FMSearchScreenMsgHandler, NULL, 0, 0);
}


/******************************************************************************
 * AppFMMainKeyHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppFMMainKeyHandler(UINT32 key)
{
    BOOL play_tone = FALSE;
    TM_SYSTEMTIME systemTime;

    switch(key)
    {
        case AP_MSG_STANDBY:
            //do nothing
            break;
        case AP_MSG_WAIT_TIMEOUT:
            APP_Sleep();
            break;
        case AP_MSG_RTC:
#if APP_SUPPORT_LCD==1
        {
            UINT32 result=0;

            TM_GetSystemTime(&systemTime);
            result = systemTime.uHour*3600 + systemTime.uMinute * 60;
            if(current_time != result)
            {
                current_time = result;
                SendAppEvent(EV_UI_FW_REDRAW, 0);
            }
        }
#endif
        break;

        #if(1)
        {
            case AP_KEY_NEXT|AP_KEY_PRESS:                //跳到电台表的下一个
            FM_StationListOperator(MC_STEP,UP);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
            case AP_KEY_PREV|AP_KEY_PRESS:                //跳到电台表的上一个
            FM_StationListOperator(MC_STEP,DOWN);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
         }
         #else
         {
         
        case AP_KEY_HOLD|AP_KEY_NEXT:
#if defined(__AT_MOD_COMMON__)				
	case AT_COMMON_FORE_PLAY:
#endif
            FMStatus->freq+=100;
            FMEntrySearchScreen(fm_search_one, UP);
            break;
        case AP_KEY_HOLD|AP_KEY_PREV:
#if defined(__AT_MOD_COMMON__)				
	case AT_COMMON_BACK_PLAY:
#endif			
            FMStatus->freq-=100;
            FMEntrySearchScreen(fm_search_one, DOWN);
            break;
        case AP_KEY_NEXT|AP_KEY_PRESS:
#if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_NEXT:
#endif
            AppFMDo(MC_STEPGRID,UP);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
        case AP_KEY_PREV|AP_KEY_PRESS:
#if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_PREV:
#endif			
            AppFMDo(MC_STEPGRID,DOWN);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
            
        case AP_KEY_PLAY|AP_KEY_PRESS:                //跳到电台表的下一个
            FM_StationListOperator(MC_STEP,UP);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
          }
          #endif

        case AP_KEY_REC | AP_KEY_PRESS:
            //    return RESULT_FMREC_START;
            break;

        case AP_KEY_MODE|AP_KEY_PRESS:
#if defined(__AT_MOD_FM__)
	case AT_FM_MODE:
#endif
#if APP_SUPPORT_MENU==1
            if(AP_Support_MENU())
            {
                GUI_Display_Menu(GUI_MENU_FM, fm_menu_callback);
            }
            else
#endif
            {
                AppGotoMainMenu();
            }
            break;


        case FM_AUTO_SEARCH:
#if defined(__AT_MOD_FM__)
	case AT_FM_AUTO_SEARCH:
#endif
            FMEntrySearchScreen(fm_search_all, UP);
            break;
        case AP_KEY_LOOP|AP_KEY_PRESS:    //跳到电台表的下一个
            FM_StationListOperator(MC_STEP,DOWN);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case FM_SAVE_FREQ:
        case FM_DELETE_FREQ:

        #if (1) 
         case AP_KEY_PLAY|AP_KEY_HOLD: 
         hal_HstSendEvent(APP_EVENT,0x44444444);
         FMEntrySearchScreen(fm_search_all, UP);
         break;
        #else
        case AP_KEY_PLAY|AP_KEY_HOLD:                 //insert or delete a station
            if(edit_station)
                break;
            edit_station = TRUE;
            if(FMStatus->station == 0) // add a station
            {
                FM_StationListOperator(MC_SAVESTATION,0);
            }
            else // remove a station
            {
                FM_StationListOperator(MC_DELONE,(INT32)&FMStatus);
                FM_StationListOperator(MC_STEP,UP);
            }
            FM_BACKUPDATA();
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
         #endif
        case FM_DELALL_FREQ:
            FM_StationListOperator(MC_DELALL,0);
            FM_BACKUPDATA();
            break;
        case AP_KEY_PLAY|AP_KEY_UP:               //insert or delete a station
            edit_station = FALSE;
            break;
#if APP_SUPPORT_MENU==1
        case FM_SET_BAND:
            GUI_GetMenuResult(&(FMStatus->mode));
            FMStatus->tab_num = 0;
            AppFMDo(MC_CLOSE, 0);

            if(!AppFMDo(MC_OPEN,0))            //硬件错误提示
            {
                AppGotoMainMenu();
                break;
            }

            FMEntrySearchScreen(fm_search_all, UP);

            break;
        case FM_FREQ_LIST:
            GUI_Display_List(LIST_TYPE_SINGLE_SELECT, 0, FMStatus->tab_num, FMStatus->station-1, fm_list_callback, FALSE);
            break;
#endif
        case AP_KEY_VOLADD|AP_KEY_DOWN:
        case AP_KEY_VOLSUB|AP_KEY_DOWN:
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);
                GUI_ResShowPic(GUI_IMG_VOLUMEBK,0,0);
                slider.max=AUD_MAX_LEVEL;
                slider.min=0;
                slider.step=1;
                slider.value=FMStatus->volume;
                GUI_Slider(&slider,FM_VolumeCb);        //音量滚动条显示
                break;
            }
#endif
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
#if defined(__AT_MOD_COMMON__)
	case AT_COMMON_VOLADD:
	case AT_COMMON_VOLSUB:
#endif
        {
            if((key&0xffff00) == AP_KEY_VOLADD||(key == AT_COMMON_VOLADD))
            {
                FMStatus->volume++;
                if(FMStatus->volume > AUD_MAX_LEVEL)
                {
                    play_tone = TRUE;
                    FMStatus->volume = AUD_MAX_LEVEL;
                }
            }
            if((key&0xffff00) == AP_KEY_VOLSUB||(key == AT_COMMON_VOLSUB))
            {
                if(FMStatus->volume > 0)
                {
                    FMStatus->volume--;
                }
                else
                {
                    play_tone = TRUE; 
                }
            }
            AppFMDo(MC_SETVOLUME,FMStatus->volume);
			if(play_tone)
			GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_VOL_MAXORMIN, GUI_MSG_FLAG_WAIT);
        }
        break;
#if APP_SUPPORT_USBSTORAGE_BG==1
        case AP_MSG_USB_DISCONNECT:
            uctls_Close();
            need_draw=TRUE;
            break;
#endif
#if APP_SUPPORT_MENU==1
        case FM_ADJUST_CLOCK:
        {
            TM_GetSystemTime(&systemTime);

            systemTime.uSecond = 0;
            systemTime.uMilliseconds = 0;

            if(systemTime.uMinute < 10)
                systemTime.uMinute = 0;
            else if(systemTime.uMinute >= 50)
            {
                systemTime.uHour ++;
                if(systemTime.uHour == 24)
                {
                    systemTime.uHour = 23;
                    systemTime.uMinute = 59;
                    systemTime.uSecond = 59;
                }
            }
            else
                break;

            TM_SetSystemTime(&systemTime);
        }
        break;
#endif
        default:                                        //热键处理
#if APP_SUPPORT_REMOTECONTROL==1
            if(MESSAGE_IsNumberKey(key))
            {
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
                GUI_InputNumber(key, 3, FMInputNumberCallback);
#endif
                break;
            }
            else
#endif
            {
                return FALSE;
            }
    }

    return TRUE;
}


/******************************************************************************
 * AppFMMainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppFMMainMsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            FM_OnStart();
            break;

        case EV_UI_FW_REDRAW:
            FM_ShowFreqIn();
            break;

        case EV_UI_FW_ON_PAUSE:
            AppFMDo(MC_CLOSE,0);
            break;

        case EV_UI_FW_ON_RESUME:
            AppFMDo(MC_OPEN,0);
            AppFMDo(MC_PLAY, 0);
            COS_Sleep(100);
            AppFMDo(MC_SETVOLUME,FMStatus->volume);

#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);//From select freq list
#endif
            FM_ShowFreqIn();
            APP_Sleep();
            break;


        case EV_UI_FW_ON_EXIT:
            //非电台录音退出！for FM record
            if ((pEvent->nParam1 != RESULT_FMREC_NOSTART)&&(pEvent->nParam1 != RESULT_FMREC_START))
            {
                AppFMDo(MC_CLOSE,0);
            }
            
            NVRAMWriteData();
            break;

#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
		case FM_ADD_TIMER:
			TIMER_API_AddAlarm(RESULT_RADIO);
			break;
#endif

#ifdef __AT_MOD_COMMON__
        case AT_COMMON_VOLUME:
        	FMStatus->volume = MIN(pEvent->nParam1, AUD_MAX_LEVEL);
        	AppFMDo(MC_SETVOLUME,FMStatus->volume);
        	break;
#endif /* __AT_MOD_COMMON__ */

#ifdef __AT_MOD_FM__
		case AT_FM_SETFREQ:
			if(pEvent->nParam1 < FM_MIN_FREQ)
			{
			    FMStatus->freq = FM_MIN_FREQ;
			}
			else if(pEvent->nParam1 > FM_MAX_FREQ)
			{
			    FMStatus->freq = FM_MAX_FREQ;
			}
			else
			{			
				FMStatus->freq = pEvent->nParam1;
			}
            AppFMDo(MC_PLAY,0);
            AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
			break;
#endif /* __AT_MOD_FM__ */


        default:
            //key event
            if(AppFMMainKeyHandler(MESSAGE_ConvertToKeyValue(pEvent)))
            {
                break;
            }
            else
            {
                //Common events, DONOT deal it.
                return FALSE;
            }
    }

    return TRUE;

}


/*********************************************************************************
* Description : 主函数
*
* Arguments   :
        param:
            0,  rx mode
            1,  tx mode
*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
void FM_Entry(void)
{
    TM_SYSTEMTIME systemTime;

    app_trace(APP_FM_TRC, "====FM_Entry====");
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
	hal_BTOverlayLoad(HAL_BTOVERLAY_FM);
#endif	
    //初始化
    FMStatus = (FM_play_status_t*)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));
#if APP_SUPPORT_INDEPEND_VOLUMNE==0
    FMStatus->volume = g_comval->Volume;
#endif
    TM_GetSystemTime(&systemTime);
    current_time = systemTime.uHour*3600 + systemTime.uMinute * 60;
    output_flag=FALSE;

    FM_CheckParam();

    if(!AppFMDo(MC_OPEN,0))            //硬件错误提示
    {
        GUI_DisplayMessageWithCB(0, GUI_STR_FMERROR, NULL, AppGotoMainMenu, GUI_MSG_FLAG_WAIT);
    }
    else
    {
        EntryNewScreen(SCR_ID_FM_MAIN, AppFMMainMsgHandler, NULL, 0, 0);
    }

}


#if APP_SUPPORT_FM_OUT
INT32  FM_OutPut(INT32 param, UINT32  *pFreq, BOOL *fm_ouput_on)/*进入FM Output 设置界面*/
{
    UINT32 key;
    INT32 result = 0;

    if(param == 0xfe)
    {
        //hal_AifSelectI2SPath(FALSE);
        AppFMDo(MC_MUTE,0);
        AppFMDo(MC_CLOSE,0);
        if(fm_ouput_on)
            *fm_ouput_on = FALSE;

        return 0;
    }

    output_flag=TRUE;
    need_draw=TRUE;

    if(param == 0xff)// open fm output, no display
    {
        need_draw = FALSE;
        g_fm_status = fm_close;
        MESSAGE_SetEvent(AP_MSG_WAIT_TIMEOUT);
    }

    //NVRAMRead(&FMStatus,VM_AP_RADIO,sizeof(FMStatus));
    FMStatus = (FM_play_status_t*)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));
    FM_CheckParam();

    FMStatus->freq = *pFreq;
    if(FMStatus->freq == 0)
        FMStatus->freq = FM_MIN_FREQ;

    while(1)
    {

        if(fm_close == g_fm_status)        //初始化
        {
            //AppFMDo(MC_MUTE,SetMUTE);
            if(!AppFMDo(MC_OPEN,1))            //硬件错误提示
            {
                GUI_DisplayMessageWithCB(0, GUI_STR_FMERROR, NULL, AppGobackScreen, GUI_MSG_FLAG_WAIT);
                return 0;
            }

            COS_Sleep(200);
            AppFMDo(MC_PLAY,0);

            //hal_AifSelectI2SPath(TRUE);


        }

        if(need_draw)            //屏幕重画
        {
            FM_ShowFreqIn();
            need_draw=FALSE;
        }

        key = MESSAGE_Wait();

#if APP_SUPPORT_REMOTECONTROL==1
        if(MESSAGE_IsNumberKey(key))
        {
            UINT32 num_input = GUI_KeyNumberHandle(key);

            if(num_input*100>=FM_MIN_FREQ && num_input*100 <= FM_MAX_FREQ)
            {
                FMStatus->freq = num_input*100;
                AppFMDo(MC_PLAY, 0);
                AppFMDo(MC_GETSTATUS,(UINT32)&FMStatus);
                FM_BACKUPDATA();
            }
            need_draw=TRUE;
        }
#endif

        switch(key)
        {
            case AP_MSG_STANDBY:
                //do nothing
                break;

            case AP_KEY_MODE | AP_KEY_UP:
            case AP_KEY_PLAY | AP_KEY_UP:
                goto end;      //ok and return

            case AP_KEY_NEXT|AP_KEY_PRESS:
                FMStatus->freq+=900;
            case AP_KEY_VOLADD|AP_KEY_PRESS:
                AppFMDo(MC_STEPGRID,UP);
                need_draw=TRUE;
                break;
            case AP_KEY_PREV|AP_KEY_PRESS:
                FMStatus->freq-=900;
            case AP_KEY_VOLSUB |AP_KEY_PRESS:
                AppFMDo(MC_STEPGRID,DOWN);
                need_draw=TRUE;
                break;
            case AP_MSG_WAIT_TIMEOUT:
                goto end;
                break;

            default:                                        //热键处理
                key=MESSAGE_HandleHotkey(key);

                if(key == RESULT_REDRAW)
                    need_draw=TRUE;
                else if((key!=0))
                {
                    result = key;
                    goto end;
                }
        }

    }

end:
    *pFreq = FMStatus->freq;
    *fm_ouput_on = TRUE;
    return result;
}
#endif


/******************************************************************************
 * FM_StationListOperator:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL FM_StationListOperator(UINT8 cmd, INT32 param)
{
    INT32 i;
    UINT8 k;

    app_trace(APP_FM_TRC, "FM_StationListOperator cmd=0x%x, param=0x%x", cmd, param);

    switch(cmd)
    {
        case MC_STEP:
            if(FMStatus->tab_num==0)
                return FALSE;
            switch(param)
            {
                case UP:                   //找到下一电台频率
                   
					
					#if 0
                    if(FMStatus->station == 0)
                    {
                        hal_HstSendEvent(SYS_EVENT,0x66666669);
                        for(k=0; k< FMStatus->tab_num; k++)
                        {
                            if((UINT32)FMStatus->tabs[k]+0x10000 > FMStatus->freq)
                                break;
                        }
                        FMStatus->station = k;
                    }
					#endif
                   

                    FMStatus->station++;
					 if(FMStatus->station>FMStatus->tab_num)
                        FMStatus->station=1;
                    
                    break;
                case DOWN:                    //找到上一电台频率
                #if 0
                    if(FMStatus->station == 0)
                    {
                        for(k=FMStatus->tab_num-1; k>=0 ; k--)
                        {
                            if((UINT32)FMStatus->tabs[k]+0x10000 < FMStatus->freq)
                                break;
                        }
                        FMStatus->station = k + 2;
                    }
					#endif

					 FMStatus->station--;
                   	 if(FMStatus->station == 0)
                     {
                         FMStatus->station= FMStatus->tab_num;
					 }
                   
                    break;
                default:
                    break;
            }
            FMStatus->freq=(FMStatus->tabs[FMStatus->station-1]) + 0x10000;
            AppFMDo(MC_PLAY, FMStatus->freq);
			hal_HstSendEvent(SYS_EVENT,0x6666666a);
			hal_HstSendEvent(SYS_EVENT,FMStatus->station);
			hal_HstSendEvent(SYS_EVENT,FMStatus->freq);
            FM_BACKUPDATA();
            break;

        case MC_SETSTATION:
            FMStatus->station = param;
            FMStatus->freq=(FMStatus->tabs[FMStatus->station-1]) + 0x10000;
            AppFMDo(MC_PLAY, FMStatus->freq);
            FM_BACKUPDATA();
            return TRUE;

        case    MC_SAVESTATION:
            if(FMStatus->station!=0)            //当前电台在电台表中有保存的情况
            {
                k=0;
                for(i=0; i<FM_TAB_NUMS; i++)
                {
                    if(FMStatus->tabs[i]!=0)k++;

                    if(k==FMStatus->station)
                    {
                        FMStatus->tabs[i]=FMStatus->freq;
                        break;
                    }
                }
            }
            else if(FMStatus->tab_num>19)
                return    FALSE;
            else                                    //新的电台频率保存到电台表
            {
                for(i=0; i<FMStatus->tab_num; i++)
                    if(FMStatus->tabs[i] >= FMStatus->freq - 0x10000)
                        break;

                FMStatus->station=i;
                for(i=FMStatus->tab_num; i>FMStatus->station; i--)
                {
                    FMStatus->tabs[i] = FMStatus->tabs[i-1];
                }
                FMStatus->tabs[i]=FMStatus->freq - 0x10000;
                FMStatus->station++;
                FMStatus->tab_num++;
            }
            return TRUE;

        case MC_DELONE:
            if(FMStatus->station==0)
                return    FALSE;

            for(k=FMStatus->station; k<FMStatus->tab_num+1; k++)
            {
                FMStatus->tabs[k-1]=FMStatus->tabs[k];
            }

            FMStatus->tabs[FMStatus->tab_num-1]=0;
            FMStatus->tab_num--;
            FMStatus->station=0;
            return     TRUE;

        case MC_DELALL:
            for(i=0; i<FM_TAB_NUMS; i++)
            {
                FMStatus->tabs[i]=0;
            }
            FMStatus->station=0;
            FMStatus->tab_num=0;
            break;

        default:
            break;
    }

    return    TRUE;
}
/******************************************************************************
 * AppFMDo:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppFMDo(UINT8 cmd, UINT32 param)
{
    INT32 i;
    //UINT8 k;
    //FMD_ERR_T ret;

    app_trace(APP_FM_TRC, "AppFMDo cmd=0x%x, param=0x%x", cmd, param);
    //hal_HstSendEvent(SYS_EVENT,0xFF000300 + cmd);

    switch(cmd)
    {
        case MC_OPEN:
        {
            return mediaSendFMCommand(cmd, FMStatus->mode);
        }
        case MC_PLAY:
        {
            app_trace(APP_FM_TRC, "FM tune, freq=%d",FMStatus->freq);
            mediaSendFMCommand(cmd, FMStatus->freq);
            g_fm_status = fm_play;
        }
        break;


        case MC_STEPGRID:
            switch(param)
            {
                case UP:
                    if(((FMStatus->freq+100)>FM_MAX_FREQ+20))
                        FMStatus->freq=FM_MIN_FREQ;
                    else
                        FMStatus->freq+=100;
                    break;
                case DOWN:
                    if(((FMStatus->freq-100)<FM_MIN_FREQ-20))
                        FMStatus->freq=FM_MAX_FREQ;
                    else
                        FMStatus->freq-=100;
                    break;
                default:
                    break;
            }
            mediaSendFMCommand(MC_PLAY, FMStatus->freq);
            FM_BACKUPDATA();
            return TRUE;


        case MC_GETSTATUS:
        {
            AUD_FM_REQ_STATUS_MSG_T req_msg;

            memset(&req_msg, 0x0, sizeof(AUD_FM_REQ_STATUS_MSG_T));

            mediaSendFMCommand(MC_GETSTATUS, (UINT32)&req_msg);

            FMStatus->freq = req_msg.freq;
            FMStatus->stereo_status = req_msg.stereo_status;
            FMStatus->rssi = req_msg.rssi;

      
            //当前电台频率值和电台表中某一频率差距小于150KHZ，此电台和此电台表对应
            //FMStatus->station=0;
            for(i=0; i<FM_TAB_NUMS; i++)
            {
                if((FMStatus->tabs[i]!=0) && ((FMStatus->freq-0x10000) != FMStatus->tabs[i]))
                {
                    if(ABS(FMStatus->freq-0x10000-FMStatus->tabs[i])<100)
                    {
                        FMStatus->station=i+1;
                        break;
                    }
                }
            }
            // no save data here NVRAMWrite(&FMStatus,VM_AP_RADIO,sizeof(FMStatus));
        }
        break;

        case MC_MUTE:
            mediaSendFMCommand(MC_MUTE, param);
            break;

        case MC_SETVOLUME:
            mediaSendFMCommand(MC_SETVOLUME, param);
            FMStatus->volume = param;
            g_comval->Volume = param;
            break;

        case MC_CLOSE:
            mediaSendFMCommand(MC_CLOSE, param);
            g_fm_status = fm_close;
            break;

        default:
            break;
    }

    return    TRUE;
}

/******************************************************************************
 * AppFMExternalDo:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppFMExternalDo(UINT8 cmd, UINT32 param)
{
    BOOL ret = TRUE;

    app_trace(APP_FM_TRC, "AppFMExternalDo cmd=0x%x, param=0x%x", cmd, param);

    switch(cmd)
    {
        case MC_OPEN:
            FMStatus = (FM_play_status_t*)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));

#if APP_SUPPORT_INDEPEND_VOLUMNE==0
            FMStatus->volume = g_comval->Volume;
#endif
            if(fm_close != g_fm_status)
            {
                ret = FALSE;
                break;
            }

            if(!AppFMDo(MC_OPEN,0))            //硬件错误提示
            {
                ret = FALSE;
                break;
            }

            AppFMDo(MC_PLAY,0);
            AppFMDo(MC_SETVOLUME,FMStatus->volume);
            break;

        case MC_CLOSE:
            AppFMDo(MC_MUTE,0);
            AppFMDo(MC_CLOSE,0);
            break;

        default:
            break;
    }

    return ret;
}

void FM_ShowFreqIn(void)
{
#if APP_SUPPORT_RGBLCD==1
    INT8 buf[20]= {'\0'};
    INT32 i,k;

    if(AP_Support_LCD())
    {
        ui_auto_update=FALSE;
        GUI_ClearScreen(NULL);

        GUI_ResShowPic(GUI_IMG_RA_FM,0,0);         //fm 标志显示
        //BT_DisplayIcon(31, 0);
#if APP_SUPPORT_USBSTORAGE_BG==1
        if(GetUsbCableStatus())
            GUI_ResShowImage(GUI_IMG_DEVICE,2,48,0);
#endif
        if(output_flag)
        {
            GUI_ResShowPic(GUI_IMG_MMUSIC,15,0);         //music 标志显示
        }
        else
        {
            if((FMStatus->tab_num>0)&&(FMStatus->station>0))        //显示电台号
            {
                sprintf(buf, "[CH %d]", FMStatus->station);
                GUI_DisplayTextCenter(20, buf);
                //if(FMStatus->station >= 10)
                //    GUI_DisplayText(28, 0, buf);
                //else
                //    GUI_DisplayText(32, 0, buf);
            }

            GUI_DisplayTimeText(64, 0, GUI_TIME_DISPLAY_HOUR_MINUTE, current_time*1000);


            if(FMStatus->stereo_status)
            {
                GUI_ResShowPic(GUI_IMG_RA_STERE,15,0);        //显示立体声
            }
        }

        GUI_DisplayBattaryLever();

        //频率换算及转换成ASCII码
        i = (FMStatus->freq+49)/100;

        sprintf(buf, "<FM %d.%d MHz>", i/10, i%10);

        GUI_DisplayTextCenter(48, buf);                        //显示电台频率

        k=(FMStatus->freq-FM_MIN_FREQ)/200;
        if(FMStatus->mode == 0)
        {
            GUI_ResShowPic(GUI_IMG_DWNARROW,24+k,80);            //显示箭头
            GUI_ResShowPic(GUI_IMG_FQBAR,0,92);                    //显示US频率条
        }
        else
        {
            GUI_ResShowPic(GUI_IMG_DWNARROW,40+k,80);            //显示箭头
            GUI_ResShowPic(GUI_IMG_FQBAR_JP,0,92);                    //显示US频率条
        }

        GUI_UpdateScreen(NULL);                             //刷新
        ui_auto_update=TRUE;
    }
#elif APP_SUPPORT_LCD==1
    INT8 buf[20]= {'\0'};
    INT32 i,k;

    if(AP_Support_LCD())
    {
        ui_auto_update=FALSE;
        GUI_ClearScreen(NULL);

        GUI_ResShowPic(GUI_IMG_RA_FM,0,0);         //fm 标志显示
        //BT_DisplayIcon(31, 0);
#if APP_SUPPORT_USBSTORAGE_BG==1
        if(GetUsbCableStatus())
            GUI_ResShowImage(GUI_IMG_DEVICE,2,48,0);
#endif
        if(output_flag)
        {
            GUI_ResShowPic(GUI_IMG_MMUSIC,15,0);         //music 标志显示
        }
        else
        {
            if((FMStatus->tab_num>0)&&(FMStatus->station>0))        //显示电台号
            {
                sprintf(buf, "[CH %d]", FMStatus->station);
                GUI_DisplayTextCenter(16, buf);
                //if(FMStatus->station >= 10)
                //    GUI_DisplayText(28, 0, buf);
                //else
                //    GUI_DisplayText(32, 0, buf);
            }

            GUI_DisplayTimeText(70, 0, GUI_TIME_DISPLAY_HOUR_MINUTE, current_time*1000);


            if(FMStatus->stereo_status)
            {
                GUI_ResShowPic(GUI_IMG_RA_STERE,15,0);        //显示立体声
            }
        }

        GUI_DisplayBattaryLever();

        //频率换算及转换成ASCII码
        i = (FMStatus->freq+49)/100;

        sprintf(buf, "<FM %d.%d MHz>", i/10, i%10);

        GUI_DisplayTextCenter(32, buf);                        //显示电台频率

        k=(FMStatus->freq-FM_MIN_FREQ)/200;
        if(FMStatus->mode == 0)
        {
            GUI_ResShowPic(GUI_IMG_DWNARROW,6+k,48);            //显示箭头
            GUI_ResShowPic(GUI_IMG_FQBAR,0,56);                    //显示US频率条
        }
        else
        {
            GUI_ResShowPic(GUI_IMG_DWNARROW,24+k,48);            //显示箭头
            GUI_ResShowPic(GUI_IMG_FQBAR_JP,0,56);                    //显示US频率条
        }

        GUI_UpdateScreen(NULL);                             //刷新
        ui_auto_update=TRUE;
    }
#elif APP_SUPPORT_LED8S==1
    INT8 buf[10]= {'\0'};
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_ICON_FM|GUI_ICON_DECPOINT,0,0);
    sprintf(buf, "%4d", (FMStatus->freq+49)/100);
    GUI_DisplayText(0, 0, buf);
    GUI_UpdateScreen(NULL);
#endif

#if (APP_SUPPORT_LED == 1)
    if((g_fm_status == fm_search_one) || (g_fm_status == fm_search_all))
        LED_SetPattern(GUI_LED_FM_SEEK, LED_LOOP_INFINITE);
    else if(FMStatus->station == 0)
        LED_SetPattern(GUI_LED_FM_PLAY, LED_LOOP_INFINITE);
    else
        LED_SetPattern(GUI_LED_FM_NOSTATION, LED_LOOP_INFINITE);
#endif /* APP_SUPPORT_LED */

}
#endif // #if APP_SUPPORT_FM
