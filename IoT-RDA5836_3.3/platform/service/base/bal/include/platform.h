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
/***********************************************************************
 *
 * MODULE NAME:    gui_display.h
 * DESCRIPTION:    gui display Interface
 * AUTHOR:         Tianwq
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2011-2011 RDA Microelectronics.
 *     All rights reserved.
 *
 * REMARKS:
 *
 ***********************************************************************/
 
#ifndef _PLATFORM__H_
#define _PLATFORM__H_

#include "cos.h"
#include "cswtype.h"
#include "tgt_app_cfg.h"

#define MIN_HISTORY  0
#define MAX_HISTORY  8

typedef enum {
	SCR_ID_START,
	SCR_ID_POWER_ON,
	SCR_ID_POWER_OFF,
	
	SCR_ID_MAIN_MENU,

	SCR_ID_FM_MAIN,
	SCR_ID_FM_SEARCH,

	SCR_ID_BT_MAIN,
	SCR_ID_BT_IDLE,
	SCR_ID_BT_CONNECTED,
	SCR_ID_BT_INCOMING_CALL,
	SCR_ID_BT_CALL,
	SCR_ID_BT_PLAY,

	SCR_ID_IDLE,
	SCR_ID_STANDBY,
	SCR_ID_CHARGING,
	SCR_ID_CALIB_KEY,

	SCR_ID_MUSIC_MAIN,
	SCR_ID_RECORD_MAIN,
#ifdef SPEAKER_SUPPORT
	SCR_ID_SPEAKER_MAIN,
#endif
	SCR_ID_UART_PLAY_MAIN,
	SCR_ID_UART_REC_MAIN,

	SCR_ID_LINEIN_MAIN,

	SCR_ID_USB_MAIN,
	SCR_ID_USB_AUDIO,
	SCR_ID_USB_STORAGE,

	SCR_ID_SETTING_MAIN,
	SCR_ID_SETTING_STORE_INFO,
	SCR_ID_SETTING_FIRMWARE,

	SCR_ID_ALARM,
	SCR_ID_ALARM_EDITALARM,
	SCR_ID_ALARM_EDITALARM_OPT,

	SCR_ID_CALENDAR,
	
	SCR_ID_GUI_MENU,
	SCR_ID_GUI_LIST,
	SCR_ID_GUI_SLIDER,

	SCR_ID_GUI_MESSAGE,
	SCR_ID_GUI_MULTI_TEXT,
	SCR_ID_GUI_INPUT_NUM,
	
	SCR_ID_GUI_DIRECTORY,
	SCR_ID_GUI_SET_DATETIME,

	
	SCR_ID_MAX
}APP_SCREEN_ID;

typedef enum{
	/*Exit by event, key, etc.*/
	SCREEN_EXIT_NORMAL,

	/*Exit slience*/
	SCREEN_EXIT_BACKGROUND
}APP_SCREEN_EXIT_FLAG;

typedef void (*FuncPtr)(void);

typedef BOOL (*App_Msg_Handler)(COS_EVENT *pEvent);


/* history information stuct */
typedef struct _historyNode
{
	U16 scrnID;
	App_Msg_Handler handler; 
	/*buffer like hightlit, input, etc.*/
	U8 *gui_buffer;
}historyNode;


typedef struct
{
    UINT8 loop_mode;
    UINT8 eq_mode;
    UINT8 repeat_count;
    UINT8 repeat_time;
} music_cfg_t;

#if APP_SUPPORT_FM==1           /*Surport fm*/
typedef struct
{
	UINT8   fm_band;             //if set the fm record
	UINT32 fm_sendfreq;           //if open the fm send
}fm_t;
#endif

//************************************
//系统公共变量定义
//************************************
typedef struct
{
	//对比度选择6~21
	INT8 DisplayContrast;

	//背景灯亮时间,默认值为0ah=5秒
	INT8 LightTime;

	//standby定时, in minutes
	INT8 StandbyTime;

	//界面语言
	INT8 langid;

	//背光灯颜色  7Color BackLight. 
	//INT8 BLightColor;

	//背光灯亮度. 
	INT8 BackLight;

	INT8 DateFormat; /*0: year-month-day; 1: day-month-year; 2: month-day-year.*/

    INT8 Volume; // system volume

    music_cfg_t music_cfg;

	//收音机选项，蓝牙选项
#if APP_SUPPORT_FM==1           /*Surport fm*/
	fm_t fm_value;
#endif

	UINT16 magic;
}comval_t;

extern void InitHistory(void);

extern App_Msg_Handler GetCurrentScreenMsgHandler(void);
extern void GobackScreen(UINT32 exit_param, UINT32 resume_param);
extern void GotoRootHistoryScreen(void);
extern void GobackAllHistory(void);
extern U8 EntryNewScreen(U16 newScreenID, 
	App_Msg_Handler newMsgHandler, 
	U8 *gui_buffer, 
	UINT32 start_param,
	UINT32 pause_param);
extern U8 DeleteScreenIfPresent(U16 ScrID);
UINT32 BAL_DisplayMessageWithCallback(
    INT16 title,
    INT16 message,
    UINT8 *string,
    UINT8 audio_id,
    void(*callbackFunc)(void),
    UINT8 flag);
U8 * GetHistoryGuiBuffer(U16 ScrID);

	
#define AppCurMsgHandler ((App_Msg_Handler) GetCurrentScreenMsgHandler())

#endif/*_PLATFORM__H_*/

