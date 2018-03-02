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


#ifndef _AP_SETTING_H_
#define _AP_SETTING_H_


typedef void (*GUI_DATETIME_SET_CBK) (date_t *, ap_time_t *);

typedef struct
{
    UINT16 img_id;
    UINT16 str_id;
    date_t  *date;
    ap_time_t *time;
    GUI_DATETIME_SET_CBK cbk_func;

    BOOL   show_what;    //TRUE, 显示, FALSE, 不显示
    BOOL     adjusting;        //正在设置,不要闪烁
    INT8   set_what;       //0, 非设定状态, 1,year, 2,month...
    INT8   item_start;
    INT8   item_end;
} GUI_DATETIME_SET_T, *GUI_DATETIME_SET_PTR;

typedef enum
{
    GUI_DATETIME_DRAW_BACKGROUND = 0x1,
    GUI_DATETIME_DRAW_DATE = 0x2,
    GUI_DATETIME_DRAW_TIME = 0x4,

	GUI_DATETIME_DRAW_DATETIME = 0x6,
    GUI_DATETIME_DRAW_ALL = 0x7,
} GUI_DATETIME_DRAW_ENUM;


//系统设置栏内的接口函数
//INT32 SET_ClockSetting(void);
INT32 SET_LanguageSetting(void);
INT32 SET_PowerDownSetting(void);
INT32 SET_BackLightTimeSetting(void);
INT32 SET_BackLightSetting(void);
INT32 SET_BackLightColorSetting(void);
INT32 SET_ContrastSetting(void);
INT32 SET_StorageSetting(void);
INT32 SET_FirmwareSetting(void);
INT32 SET_SystemReset(void);
INT32 SET_UpdateSetting(void);
INT32 SET_BTSetting(void);
INT32 SET_DataFormatSetting(void);

/*Alarm Sub Menu items*/
INT32 SET_ALARMClockSetting(void);
INT32 SET_ALARMTimesSetting(void);

INT32 GUI_ClockSetting(date_t *date,ap_time_t *time,UINT16 img_id,UINT16 str_id, GUI_DATETIME_SET_CBK cbk_func);


INT32 SET_Entry(INT32 param);/*外部接口函数*/

#endif  /* _AP_SETTING_H_*/



