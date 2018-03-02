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

#ifndef _AP_FM_H_
#define _AP_FM_H_

#include "ap_common.h"
#include "fmd_m.h"
#include "ap_gui.h"
#include "ap_message.h"
#include "resource_id.h"
#include "globalconstants.h"
#include "aud_main.h"

#define FM_TAB_NUMS		20

typedef struct
{
	UINT16 *param1;              //电台表地址指针，20个（US模式）/20个（Japan模式）
	UINT16 *param2;              //当前电台信息的地址指针
}FM_param_t;

typedef enum
{
  DOWN=1,              //[0]下降
  UP,                //[1]上升        
  STOP,
}FM_direct_t;

typedef enum
{
   STERE,               //[0]立体声
   MONO                 //[1]单声道        
}FM_Audio_t;

typedef enum
{
   releaseMUTE,        //解除静音
   SetMUTE             //静音        
}FM_MUTE_t;

typedef struct
{
   FM_Audio_t  FM_Stereo_Status;	//0，立体声；非0，单声道
   UINT16        FM_CurrentFreq;		//当前频率
}FMStatus_Tab_t;

typedef enum					//波段模式
{
	 Bank_US_Europe,
	 Bank_Japan
}FM_Station_bank_t;


typedef struct
{
  UINT8 status;           // (1:playing ; 2:pause ; 3(searching).
  UINT8 station;          //当前电台对应的电台表项号。
  UINT32  freq;             //当前电台的频率。单位为1k (最高位恒为1)
  FMD_BAND_T mode;             //0 (US-Europe) /1 (Japan) ;
  FMD_ANA_LEVEL_T volume;           //音量。
  BOOL stereo_status;
  UINT8 rssi;
  UINT8 tab_num;
  UINT16 tabs[FM_TAB_NUMS];
  UINT16 magic;
}FM_play_status_t;

typedef struct
{
	UINT8 search_type; //search all / search one
	UINT8 search_direct;
	UINT8 search_timer;
	
    UINT8 rssi[FM_TAB_NUMS]; //save vaild station rssi
}FM_Search_status_t;

//FM 搜索接口函数
//INT32  FM_SearchHandle(UINT8 Direct);                //自动搜索处理，Direct=10，NEXT，=20，LAST    
//INT32  FM_AutoSearchHandle(UINT8 Direct);        

void FMEntrySearchScreen(UINT8 type, UINT8 direct);

INT32  FM_MainFunc(INT32 test);                      //FM应用的处理
void FM_ShowFreqIn(void);                         //FM主界面显示
//BOOL FM_SearchDisplay(FM_direct_t Direct);          //自动搜台时的显示
INT32  FM_PlayingHandle(void);                      //播放处理函数
BOOL   AppFMDo(UINT8 cmd, UINT32 param);     //发送消息给其他task
void FM_Entry(void);
INT32  FM_OutPut(INT32 param, UINT32  *pFreq, BOOL *fm_ouput_on);/*进入FM Output 设置界面*/
BOOL FM_StationListOperator(UINT8 cmd, INT32 param);

#endif/*_AP_FM_H*/





