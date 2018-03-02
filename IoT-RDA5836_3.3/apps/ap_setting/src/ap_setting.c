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
#include "ap_setting.h"
#include "ap_calendar.h"

#define  MAX_SYSMENUNUM          7
#define  MAX_LIST_NODE           15

typedef INT32 (*setting_func)(void);

extern slider_t         slider;
//extern progress_t       progress;
extern BOOL             ui_auto_select;
extern BOOL             ui_auto_update;
extern INT32            g_last_reslt;


#define CONFIG_SET_SHUTTIME             AP_MSG_USER_SETTING+1
#define CONFIG_SET_BACKTIME             AP_MSG_USER_SETTING+2
#define CONFIG_SET_BACKCOLOR            AP_MSG_USER_SETTING+3
#define CONFIG_SET_BACKLIGHT            AP_MSG_USER_SETTING+4
#define CONFIG_SET_LANGUAGE             AP_MSG_USER_SETTING+5
#define CONFIG_SET_MEMORY_INFO          AP_MSG_USER_SETTING+6
#define CONFIG_SET_HW_VERSION           AP_MSG_USER_SETTING+7
#define CONFIG_SET_HW_UPGRADE           AP_MSG_USER_SETTING+8
#define CONFIG_SYSTEM_CLOCK             AP_MSG_USER_SETTING+9
#define CONFIG_SET_DATE_FORMAT          AP_MSG_USER_SETTING+10
#define CONFIG_SET_HW_SYSTEMRESET       AP_MSG_USER_SETTING+11
#define CONFIG_SET_TIMER       			AP_MSG_USER_SETTING+12


#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)

static INT32 menu_active;    //main menu's active number,

//days of different month
const INT8 month_days[]= {31,28,31,30,31,30,31,31,30,31,30,31 };

extern clndr_table_struct *g_pCalendarInfo;

static const INT8 digitalchar[]="0123456789";

/*********************************************************************************
* Description :
*
* Arguments   :
*
* Returns     :
*
*
*********************************************************************************/
INT32 config_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
    {
        if(value == GUI_MENU_CONFIG)
            return menu_active;
        else if(value == GUI_MENU_CFG_LANG)
            return g_comval->langid;
        else
            return 0;
    }

    return 0;
}

/******************************************************************************
 * SET_ClockSettingCallback:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void SET_ClockSettingCallback(date_t *date,ap_time_t *time)
{
    //save date & time
    TM_SetDateAndTime(&date, &time);
}
/******************************************************************************
 * SET_MainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL SET_MainMsgHandler(COS_EVENT *pEvent)
{
    UINT16 param;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
		    menu_active = 0;
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            GUI_Display_Menu(GUI_MENU_CONFIG, config_menu_callback);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_ON_EXIT:
            NVRAMWriteData();
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case CONFIG_SYSTEM_CLOCK:
                {
                    date_t    date= {2012, 1, 15};
                    ap_time_t time= {12, 30, 00};

                    TM_GetDateAndTime(&date, &time);

                    GUI_ClockSetting(&date,&time,GUI_IMG_CLOCKICON,GUI_STR_SYSTEMCLOCK, SET_ClockSettingCallback);

                    menu_active = 1;
                }
                break;

                case CONFIG_SET_TIMER:
					TIMER_EditAlarm(NULL);
                    menu_active = 3;
					break;

                case CONFIG_SET_DATE_FORMAT:
                    SET_DataFormatSetting();
                    menu_active = 7;
                    break;

                case CONFIG_SET_SHUTTIME:
                    SET_PowerDownSetting();
                    menu_active = 4;
                    break;
                case CONFIG_SET_BACKTIME:
                    SET_BackLightTimeSetting();
                    menu_active = 5;
                    break;
                case CONFIG_SET_BACKCOLOR:
                    //SET_BackLightColorSetting();
                    menu_active = 5;
                    break;
                case CONFIG_SET_BACKLIGHT:
                    SET_BackLightSetting();
                    menu_active = 5;
                    break;
                case CONFIG_SET_LANGUAGE:
                    GUI_GetMenuResult(&param);
                    g_comval->langid = param;
                    gui_load_resource(param);
                    menu_active = 2;
                    break;
                case CONFIG_SET_MEMORY_INFO:
                    SET_StorageSetting();
                    menu_active = 6;
                    break;
                case CONFIG_SET_HW_VERSION:
                    SET_FirmwareSetting();
                    menu_active = 6;
                    break;
                case CONFIG_SET_HW_UPGRADE:
                    SET_UpdateSetting();
                    menu_active = 6;
                    break;
                case CONFIG_SET_HW_SYSTEMRESET:
                    SET_SystemReset();
                    menu_active = 6;
                    AppGotoMainMenu();
                    break;
                case AP_KEY_MODE|AP_KEY_PRESS:
                case AP_KEY_PLAY|AP_KEY_PRESS:
                    AppGobackScreen();
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

/*********************************************************************************
* Description : 主函数
*
* Arguments   :
        param:
            0, 开始时关标停在第0项
            1, 开始时关标停在ADFU
*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
INT32 SET_Entry(INT32 argument)
{
    EntryNewScreen(SCR_ID_SETTING_MAIN, SET_MainMsgHandler, NULL, 0, 0);

    return 0;
}



/*********************************************************************************
* Description : 背光时间设置回调函数
*
* Arguments   : value, 背光时间,单位秒
*
* Returns     :
*
* Notes       : =0时关闭背光
*
*********************************************************************************/
void SET_BackLightTimeCb(INT32 value)
{
    g_comval->LightTime = (UINT8)value;

    //及时关闭背光
    if(g_comval->LightTime == 0)
        CloseBacklight();
    else
        OpenBacklight();

    MESSAGE_Initial(g_comval);
}

/*********************************************************************************
* Description : 背光时间设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 SET_BackLightTimeSetting(void)
{
    INT32  result=0;

    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_IMG_BKLIGHT, 0, 0);
    GUI_DisplayBattaryLever();
    GUI_DisplayTextCenter(0, GUI_GetString(GUI_STR_BACKLIGHTTIME));

    slider.max  = 30;
    slider.min  = 0;
    slider.step = 3;
    slider.value = g_comval->LightTime;

    result=GUI_Slider(&slider, SET_BackLightTimeCb);

    return result;
}
/******************************************************************************
 * SET_PowerDownCb: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void SET_PowerDownCb(INT32 value)
{
    g_comval->StandbyTime = (UINT8)value;
    //重新设置关机时间
    MESSAGE_Initial(g_comval);
}

/*********************************************************************************
* Description : 关机设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       : 标准功能菜单控件
*
*********************************************************************************/
INT32 SET_PowerDownSetting(void)
{
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_IMG_POWER, 0, 0);
    GUI_DisplayTextCenter(0, GUI_GetString(GUI_STR_POWEOFFTIME));
    GUI_DisplayBattaryLever();

    slider.min   = 0;
    slider.max   = 60;
    slider.step  = 10;
    slider.value = g_comval->StandbyTime;
    GUI_Slider(&slider, SET_PowerDownCb);
    
    return 0;
}

/*********************************************************************************
* Description : 对比度设定回调函数
*
* Arguments   : value, 1~16, 对应硬件 6~21
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
void SET_ContrastCb(INT32 value)
{
    g_comval->DisplayContrast = (UINT8)value;
    //设置到硬件
    SetContrast(g_comval->DisplayContrast);
}


/*********************************************************************************
* Description : 对比度设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       : 注意背景先画
*
*********************************************************************************/
INT32 SET_ContrastSetting(void)
{
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_IMG_CONTRSTW, 10, 0);
    GUI_ResShowPic(GUI_IMG_CONTRSTB, 98, 0);

    GUI_DisplayBattaryLever();

    slider.min  = 1;
    slider.max  = 7;
    slider.step = 1;
    slider.value = g_comval->DisplayContrast;

    return GUI_Slider(&slider, SET_ContrastCb);
}

/*********************************************************************************
* Description : 亮度设定界面
*
* Arguments   :
*
* Returns     :
*
* Notes       : 注意背景先画
*
*********************************************************************************/
void SET_BackLightCb(INT32 value)
{
    g_comval->BackLight = (UINT8)value;
    //设置到硬件
    SetBackLight(g_comval->BackLight);
}
INT32 SET_BackLightSetting(void)
{
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_IMG_BKLIGHT, 10, 0);
    GUI_ResShowPic(GUI_IMG_BKLIGHTA, 98, 0);

    GUI_DisplayBattaryLever();

    slider.min  = 0;
    slider.max  = 7;
    slider.step = 1;
    slider.value = g_comval->BackLight;

    return GUI_Slider(&slider, SET_BackLightCb);
}

/*********************************************************************************
* Description : 格式化储存容量的显示字符串
*
* Arguments   : buf, 输出字符串buffer
        mega，总容量
        percent，使用百分比
*
* Returns     :
*
* Notes       : 1024M<100%>
*
*********************************************************************************/
void SET_FmtStorage(INT8 *buf, UINT16 mega, UINT8 percent)
{
    sprintf(buf, "%dM<%d%>", mega, percent);
}

typedef enum
{
    STORE_DEV_TFLASH,
#if APP_SUPPORT_USB
    STORE_DEV_USB,
#endif
    STORE_DEV_TOTAL
} SET_STORE_DEV_ENUM;

typedef struct
{
    BOOL mount_ok;
    UINT64 total;
    UINT64 free;        //内存
} SET_STORE_INFO_T, *SET_STORE_INFO_PTR;

typedef struct
{
    UINT8 show_id;
    SET_STORE_INFO_T dev[STORE_DEV_TOTAL];
} SET_STOREAGE_SET_T, *SET_STOREAGE_SET_PTR;


void SET_StorageSettingRedraw(SET_STORE_DEV_ENUM param)
{
    INT32 img_index[STORE_DEV_TOTAL] =
    {
        FS_DEV_TYPE_TFLASH,
#if APP_SUPPORT_USB
        FS_DEV_TYPE_USBDISK
#endif
    };


    SET_STOREAGE_SET_PTR ssp = (SET_STOREAGE_SET_PTR) GetHistoryGuiBuffer(SCR_ID_SETTING_STORE_INFO);

    SET_STORE_DEV_ENUM index = (param >= STORE_DEV_TOTAL) ? (ssp->show_id) : param;


    UINT64 total=ssp->dev[index].total;
    UINT64 free=ssp->dev[index].free;        //插卡
    UINT8  percent=0L;

    INT8 buf[20];    // 1024M<100%>


    //处理<1%
    if(total == free)
        percent = 0;
    else
    {
        percent = (total - free)*100 / total;
        if(percent == 0) percent = 1;
    }

    GUI_ClearScreen(NULL);

    //show progress bar & text//percentage
    //control
    slider.min = 0;
    slider.max = 100;
    slider.value = percent;//percentage

    //显示卡图标
    GUI_ResShowImage(GUI_IMG_DEVICE, img_index[index], 0, 0);

    //显示箭头
    if(STORE_DEV_TOTAL - 1 == index)
        GUI_ResShowPic(GUI_IMG_CARROWL, 115, 0);
    else
        GUI_ResShowPic(GUI_IMG_CARROWR, 115, 0);

    //显示文字
    SET_FmtStorage(buf, total / 1024 / 1024, percent);
    GUI_DisplayTextCenter(0, buf);

   // GUI_Slider(&slider, NULL);
	GUI_SliderRedraw(&slider);

}

BOOL SET_StorageSettingMsgHandler(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, STORE_DEV_TOTAL);
            break;

        case EV_UI_FW_REDRAW:
            SET_StorageSettingRedraw(pEvent->nParam1);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, STORE_DEV_TOTAL);
            break;

        case EV_UI_FW_ON_EXIT:
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_WAIT_TIMEOUT:           //子菜单界面在8秒后返回系统功能菜单
                case AP_KEY_MODE | AP_KEY_PRESS:
                case AP_KEY_PLAY | AP_KEY_PRESS:
                    AppGobackScreen();                       //退出
                    break;

                case AP_KEY_NEXT | AP_KEY_PRESS:
                case AP_KEY_PREV | AP_KEY_PRESS:
                {
                    SET_STORE_DEV_ENUM i;
                    SET_STOREAGE_SET_PTR ssp =
                        (SET_STOREAGE_SET_PTR) GetHistoryGuiBuffer(SCR_ID_SETTING_STORE_INFO);

                    ssp->show_id ++;

                    if(ssp->show_id >= STORE_DEV_TOTAL)
                        ssp->show_id = STORE_DEV_TFLASH;

                    SendAppEvent(EV_UI_FW_REDRAW, ssp->show_id);
                }
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
/*********************************************************************************
* Description : 储存状况界面
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 SET_StorageSetting(void)
{
    SET_STORE_DEV_ENUM i;
    SET_STOREAGE_SET_PTR ssp = NULL;
    FS_DEV_TYPE dev_type[STORE_DEV_TOTAL] =
    {
        FS_DEV_TYPE_TFLASH,
#if APP_SUPPORT_USB
        FS_DEV_TYPE_USBDISK
#endif
    };


    ssp = (SET_STOREAGE_SET_PTR) COS_MALLOC(sizeof(SET_STOREAGE_SET_T));
    memset(ssp, 0x0, sizeof(SET_STOREAGE_SET_T));


    for( i = STORE_DEV_TFLASH; i < STORE_DEV_TOTAL; i++)
    {
        if(MountDisk(dev_type[i]))
        {
            ssp->dev[i].total = FS_GetSpace(dev_type[i],FS_GET_SPACE_TOTAL);
            ssp->dev[i].free = FS_GetSpace(dev_type[i],FS_GET_SPACE_FREE);
            ssp->dev[i].mount_ok = TRUE;

            ssp->show_id = i;
        }
        else
        {
            ssp->dev[i].mount_ok = FALSE;
        }
    }


    EntryNewScreen(SCR_ID_SETTING_STORE_INFO, SET_StorageSettingMsgHandler, ssp, 0, 0);
}

/******************************************************************************
 * SET_FirmwareSettingMsgHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL SET_FirmwareSettingMsgHandler(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
        {
            INT8 buf[20];

            GUI_ClearScreen(NULL);

            //show barcode
            GUI_ResShowPic(GUI_IMG_FWBCODE, 10, 0);
            sprintf(buf, "APP:%d.%d", AP_GetVersion()>>8, AP_GetVersion()&0xff);
            GUI_DisplayText(0, 16, buf);
            sprintf(buf, "Date: %08d", AP_GetBuildDate());
            GUI_DisplayText(0, 32, buf);
            sprintf(buf, "HW:%d.%d r%d", GetPlatformVersion()>>8, GetPlatformVersion()&0xff, GetPlatformRevision());
            GUI_DisplayText(0, 48, buf);

            GUI_UpdateScreen(NULL);

        }
        break;

        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_ON_PAUSE:
        case EV_UI_FW_ON_EXIT:
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_WAIT_TIMEOUT:           //子菜单界面在8秒后返回系统功能菜单
                case AP_KEY_MODE | AP_KEY_PRESS:
                case AP_KEY_PLAY | AP_KEY_PRESS:
                    AppGobackScreen();                       //退出
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

/*********************************************************************************
* Description : 固件版本界面
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 SET_FirmwareSetting(void)
{
    EntryNewScreen(SCR_ID_SETTING_FIRMWARE, SET_FirmwareSettingMsgHandler, NULL, 0, 0);
}

void SET_UpdateSettingProcess(void *p)
{
    INT32 file = (INT32) p;
    INT8 *string;

    GUI_ClearScreen(NULL);

    string = GUI_GetString(GUI_STR_UPGRADING);
    GUI_DisplayTextCenter(16, string);
    GUI_UpdateScreen(NULL);
    /*for(i=0;i<100;i++)
    {
        slider.value = i;
        GUI_Slider(&slider, NULL);
        MESSAGE_Sleep(1);
    }*/
    //DSM_TFlash_Upgrade(file);

    GUI_DisplayMessage(0, GUI_STR_UPGRADE_COMPLETE, NULL, GUI_MSG_FLAG_WAIT);
    RestartSystem();

}

void SET_UpdateSettingSelectFileCb(file_location_t *f, UINT16 result)
{
    INT32 file;

    if(LIST_CALLBACK_SELECT_ITEM == result)
    {
    	if(f)
    	{
    	    file = FS_OpenDirect(f->file_entry, /*FS_O_RDONLY*/FS_O_RDWR, 0);
		    if(file < 0)
		    {
		        GUI_DisplayMessage(0, GUI_STR_MREADERR, NULL, GUI_MSG_FLAG_WAIT);
		    }
		    else
		    {
	    		mmi_timer_create(1, SET_UpdateSettingProcess, file, COS_TIMER_MODE_SINGLE);
		    }
    	}
    }
}

/*********************************************************************************
* Description : 固件升级
*
* Arguments   :
*
* Returns     : 结果消息
*
* Notes       :
*
*********************************************************************************/
INT32 SET_UpdateSetting(void)
{
    file_location_t location;

    GUI_EntryFileBrowser(&location, FSEL_TYPE_ALLFILE, 0, SET_UpdateSettingSelectFileCb);
    return 0;
}

#if 0
/*********************************************************************************
* Description : 背光颜色选择界面,
*
* Arguments   : 0:White,1:Red,2:Green,3Yellow,4:Blue,5:Purple,6:Cyan
*
* Returns     :
*
*
*********************************************************************************/
void SET_BackLightColorCb(INT32 value)
{
    CloseBacklight();
    SetBackLightColor((INT8)value);   //将颜色值传递给键盘驱动的BLightColor全局变量.
    OpenBacklight();               //根据上面设置的BLightColor，点亮对应颜色的背光.
    g_comval->BLightColor = (INT8)value;//保存当前背光颜色值.
}

INT32 bkcolor_list_callback(INT32 type, INT32 value, INT8 **string)
{
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = GUI_GetString(GUI_STR_REDLIGHT + value);
        return value+1 == g_comval->BLightColor;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        CloseBacklight();
        SetBackLightColor((INT8)value+1);   //将颜色值传递给键盘驱动的BLightColor全局变量.
        OpenBacklight();               //根据上面设置的BLightColor，点亮对应颜色的背光.
        g_comval->BLightColor = (INT8)value+1;//保存当前背光颜色值.
        return 1;
    }
}

/*********************************************************************************
* Description : 设置背光灯颜色
*
* Arguments   :
*
* Returns     :
*
* Notes       : 多色彩支持
*
*********************************************************************************/
INT32 SET_BackLightColorSetting(void)
{
    return  GUI_Display_List(LIST_TYPE_SINGLE_SELECT, 0, 7, g_comval->BLightColor-1, bkcolor_list_callback);
}
#endif

#define CLOCK_SET_YEAR       1
#define CLOCK_SET_MONTH      2
#define CLOCK_SET_DAY        3
#define CLOCK_SET_HOUR       4
#define CLOCK_SET_MINUTE     5

UINT8 SET_DateAndTime(INT8 set_what,INT8 *days,date_t *date,ap_time_t *time,BOOL flag)
{

    // check date format
    if(date && set_what <= CLOCK_SET_DAY)
    {
        const char *format = date_formats[g_comval->DateFormat];
        switch(format[(set_what-1)*3])
        {
            case 'Y':
                set_what = CLOCK_SET_YEAR;
                break;
            case 'M':
                set_what = CLOCK_SET_MONTH;
                break;
            case 'D':
                set_what = CLOCK_SET_DAY;
                break;
        }
    }
    {
        switch(set_what)
        {
            case CLOCK_SET_YEAR:    //year
                if(date)
                {
                    if(flag)
                    {
                        if(date->year < g_pCalendarInfo->last_year)
                            date->year++;
                        else
                            date->year = g_pCalendarInfo->first_year;
                    }
                    else
                    {
                        if(date->year > g_pCalendarInfo->first_year)
                            date->year--;
                        else
                            date->year = g_pCalendarInfo->last_year;
                    }
                }
                break;
            case CLOCK_SET_MONTH:    //month
                if(date)
                {
                    if(flag)
                    {
                        if(date->month < 12)
                            date->month++;
                        else
                            date->month = 1;
                    }
                    else
                    {
                        if(date->month > 1)
                            date->month--;
                        else
                            date->month = 12;
                    }
                }
                break;
            case CLOCK_SET_DAY:    //day
                if(date)
                {
                    *days = month_days[date->month-1];   //本月天数
                    if(IsLeapYear(date->year) && (date->month == 2))
                        *days=*days+1;    //闰年+1

                    if(flag)
                    {
                        if(date->day < *days )
                            date->day++;
                        else
                            date->day = 1;
                    }
                    else
                    {
                        if(date->day > 1 )
                            date->day--;
                        else
                            date->day = *days;
                    }
                }
                break;
            case CLOCK_SET_HOUR:    //hour
                if(time)
                {
                    if(flag)
                    {
                        if(time->hour < 23)
                            time->hour++;
                        else
                            time->hour = 0;
                    }
                    else
                    {
                        if(time->hour > 0)
                            time->hour--;
                        else
                            time->hour = 23;
                    }
                }
                break;
            case CLOCK_SET_MINUTE:    //min
                if(time)
                {
                    if(flag)
                    {
                        if(time->minute < 59)
                            time->minute++;
                        else
                            time->minute = 0;
                    }
                    else
                    {
                        if(time->minute > 0)
                            time->minute--;
                        else
                            time->minute = 59;
                    }
                }
                break;
            default:    //no in setting
                return 0;
        }    //set_what
    }

    return 0xff;
}



#if 1
void GUI_DatetimeSettingRedraw(GUI_DATETIME_DRAW_ENUM param)
{
    region_t clock_region= {32, 40, 64, 16};
    region_t setting_region = {0, 0, 0, 0};
    const char *format = date_formats[g_comval->DateFormat];

    GUI_DATETIME_SET_PTR dtp = (GUI_DATETIME_SET_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_SET_DATETIME);

    //显示背景
    if(param & GUI_DATETIME_DRAW_BACKGROUND)
    {
        GUI_ClearScreen(NULL);
        ui_auto_update = FALSE;
        if(dtp->img_id) GUI_ResShowPic(dtp->img_id, 0, 0);    //show icon
        GUI_DisplayTextCenter(0, GUI_GetString(dtp->str_id));
        GUI_DisplayBattaryLever();
        ui_auto_update = TRUE;
    }

    //显示时钟
    //if(need_draw)
    {
        setting_region.width = 16;
        setting_region.height = 16;

        if(dtp->date) //(param & GUI_DATETIME_DRAW_DATE)
        {
            GUI_DisplayDateText(24, 24, TRUE, ((dtp->date->year)<<9)+(dtp->date->month<<5)+dtp->date->day);
            if(dtp->set_what<CLOCK_SET_HOUR)
            {
                setting_region.x = 24+(dtp->set_what-1)*24;
                setting_region.y = 24;
                if(format[(dtp->set_what-1)*3] == 'Y')
                {
                    setting_region.width = 32;
                }
                else
                {
                    if((dtp->set_what==2 && format[0] == 'Y') || (dtp->set_what==3 && (format[0] == 'Y'||format[3] == 'Y')))
                        setting_region.x += 16;
                }
            }

        }

        if(dtp->time) //(param & GUI_DATETIME_DRAW_TIME)
        {
            UINT32 time_value = ((dtp->time->hour*60+dtp->time->minute)*60+dtp->time->second)*1000;

            if(dtp->set_what>=CLOCK_SET_HOUR)
                setting_region.x = 44+(dtp->set_what-CLOCK_SET_HOUR)*24;

            if(dtp->date)
            {
                GUI_DisplayTimeText(44, 40, GUI_TIME_DISPLAY_HOUR_MINUTE, time_value);
                if(dtp->set_what>=CLOCK_SET_HOUR)
                    setting_region.y = 40;
            }
            else
            {
                GUI_DisplayTimeText(44, 24, GUI_TIME_DISPLAY_HOUR_MINUTE, time_value);
                if(dtp->set_what>=CLOCK_SET_HOUR)
                    setting_region.y = 24;
            }
        }
        if(dtp->set_what != 0)
        {
            if(dtp->show_what == 0)
            {
                GUI_ClearScreen(&setting_region);
            }
            //GUI_InvertRegion(&setting_region);
        }

        //to reduce scr flash
        GUI_UpdateScreen(NULL);

    }
}


BOOL GUI_ClockSettingMsgHandler(COS_EVENT *pEvent)
{
    UINT8  bSettime=0;
    INT8   days;        //本月的天数

    GUI_DATETIME_SET_PTR dtp = (GUI_DATETIME_SET_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_SET_DATETIME);

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            dtp->set_what=0;       //0, 非设定状态, 1,year, 2,month...
            dtp->show_what=TRUE;    //TRUE, 显示, FALSE, 不显示
            dtp->adjusting=FALSE;        //正在设置,不要闪烁
        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, GUI_DATETIME_DRAW_ALL);
            break;

        case EV_UI_FW_REDRAW:
            GUI_DatetimeSettingRedraw(pEvent->nParam1);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
            if(pEvent->nParam1)/*save data*/
            {
                if(dtp->date)
                {
                    //re-check the date/time format
                    days = month_days[dtp->date->month - 1];

                    if((dtp->date->year % 4 == 0) && (dtp->date->month == 2))
                        days++;    //润年
                    if(dtp->date->day > days)
                        dtp->date->day =  days;
                }

                if(dtp->time)
                    dtp->time->second = 0;

                if(dtp->cbk_func)
                    dtp->cbk_func(dtp->date, dtp->time);
            }

            if(dtp->date) COS_FREE(dtp->date);
            if(dtp->time) COS_FREE(dtp->time);
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_RTC:
                    if(dtp->adjusting)
                        dtp->show_what = TRUE;
                    else
                        dtp->show_what = !dtp->show_what;

                    SendAppEvent(EV_UI_FW_REDRAW, GUI_DATETIME_DRAW_DATETIME);
                    break;
                case AP_KEY_NEXT | AP_KEY_PRESS:
                    if(dtp->set_what >= dtp->item_end || dtp->set_what < dtp->item_start)
                        dtp->set_what = dtp->item_start;
                    else
                        dtp->set_what++;
                    break;
                case AP_KEY_PREV | AP_KEY_PRESS:
                    if(dtp->set_what <= dtp->item_start)
                        dtp->set_what = dtp->item_end;
                    else
                        dtp->set_what--;
                    break;
                case AP_KEY_VOLADD | AP_KEY_HOLD:
                    dtp->adjusting = TRUE;
                case AP_KEY_VOLADD | AP_KEY_PRESS:    //in setting clock
                    bSettime  = SET_DateAndTime(dtp->set_what,&days,dtp->date,dtp->time,1);
                    SendAppEvent(EV_UI_FW_REDRAW, GUI_DATETIME_DRAW_DATETIME);
                    break;

                case AP_KEY_VOLSUB | AP_KEY_HOLD:
                    dtp->adjusting = TRUE;
                case AP_KEY_VOLSUB | AP_KEY_PRESS:    //in setting clock
                    bSettime  = SET_DateAndTime(dtp->set_what,&days,dtp->date,dtp->time,0);
                    SendAppEvent(EV_UI_FW_REDRAW, GUI_DATETIME_DRAW_DATETIME);
                    break;

                case AP_KEY_VOLADD| AP_KEY_UP:
                case AP_KEY_VOLSUB| AP_KEY_UP:
                    dtp->adjusting = FALSE;
                    break;

                case AP_KEY_MODE | AP_KEY_PRESS:
                case AP_MSG_WAIT_TIMEOUT:        //子菜单界面在8秒后返回系统功能菜单
                    GobackScreen(0 /*NOT save data*/, 0);
                    break;

                case AP_KEY_PLAY | AP_KEY_PRESS:
                    GobackScreen(1 /*save data*/, 0);
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
 * GUI_ClockSetting:
 * DESCRIPTION: -时间设定界面
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
INT32 GUI_ClockSetting(date_t *date,ap_time_t *time,UINT16 img_id,UINT16 str_id, GUI_DATETIME_SET_CBK cbk_func)
{
    GUI_DATETIME_SET_PTR dtp = NULL;

    if(date==NULL && time == NULL)
        return 0;

    if(cbk_func == NULL)
        return 0;

    g_pCalendarInfo = (clndr_table_struct*)GetCalendarData();
    dtp = (GUI_DATETIME_SET_PTR) COS_MALLOC(sizeof(GUI_DATETIME_SET_T));
    memset(dtp, 0x0, sizeof(GUI_DATETIME_SET_T));

    if(date)
    {
        if(date->year > g_pCalendarInfo->last_year)
            date->year = g_pCalendarInfo->last_year;
        else if(date->year < g_pCalendarInfo->first_year)
            date->year = g_pCalendarInfo->first_year;

        dtp->item_start=CLOCK_SET_YEAR;
        dtp->date = (date_t *) COS_MALLOC(sizeof(date_t));

        memcpy(dtp->date, date, sizeof(date_t));
    }
    else
    {
        dtp->item_start=CLOCK_SET_HOUR;
    }


    if(time)
    {
        time->second = 0;
        dtp->item_end = CLOCK_SET_MINUTE;

        dtp->time= (ap_time_t *) COS_MALLOC(sizeof(ap_time_t));
        memcpy(dtp->time, time, sizeof(ap_time_t));
    }
    else
    {
        dtp->item_end = CLOCK_SET_DAY;
    }

    dtp->cbk_func = cbk_func;

    EntryNewScreen(SCR_ID_GUI_SET_DATETIME, GUI_ClockSettingMsgHandler, dtp, 0, 0);

}

#endif



INT32 format_list_callback(INT32 type, INT32 value, INT8 **string)
{
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = date_formats[value];
        return value==g_comval->DateFormat;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        g_comval->DateFormat = value;
        return 0;
    }
}



/*********************************************************************************
* Description : 设置日期格式
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 SET_DataFormatSetting(void)
{
    INT32   result;
    result = GUI_Display_List(LIST_TYPE_SINGLE_SELECT, 0, AP_MAX_FROMAT, g_comval->DateFormat, format_list_callback, FALSE);

    return result;
}



/*********************************************************************************
* Description : 系统复位
*
* Arguments   :
*
* Returns     : 结果消息
*
* Notes       :
*
*********************************************************************************/
INT32 SET_SystemReset(void)
{

    DSM_UserDataClear();
    g_comval = (comval_t*)NVRAMGetData(VM_SYSTEM,sizeof(comval_t));

    LoadFactorySetting(g_comval, sizeof(g_comval));
//    NVRAMWrite(g_comval, VM_SYSTEM,sizeof(g_comval));
    NVRAMWriteData();
    GUI_DisplayMessage(0, GUI_STR_RESET_COMPLETE, NULL, GUI_MSG_FLAG_WAIT);
    RestartSystem();
    return RESULT_MAIN;
}



#endif // APP_SUPPORT_LCD==1    


