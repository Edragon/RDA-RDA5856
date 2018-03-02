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


#ifndef _AP_GUI_H_
#define _AP_GUI_H_

#include "ap_common.h"
#include "ap_message.h"
#include "gui.h"

//主界面各个控件的x坐标
#if APP_SUPPORT_RGBLCD==1
#define POSITION_A            (0)
#define POSITION_B            (20)
#define POSITION_C            (44)
#define POSITION_D            (68)
#define POSITION_E            (90)
#define POSITION_F            (110)
#define POSITION_BATT         (141)
#define POSITION_G            (0) 
#elif APP_SUPPORT_LCD==1
#define POSITION_A            (0)
#define POSITION_B            (15)
#define POSITION_C            (38)
#define POSITION_D            (51)
#define POSITION_E            (68)
#define POSITION_F            (86)
#define POSITION_BATT         (114)
#define POSITION_G            (0)

#endif
#define GUI_MSG_FLAG_DISPLAY           1  // display and return
#define GUI_MSG_FLAG_CONFIRM          2  // display and wait user select yes or no
#define GUI_MSG_FLAG_KEY                  4  // display and wait user press any key
#define GUI_MSG_FLAG_WAIT                  8  // display and wait 2 seconds

typedef struct
{
  INT8 min;
  INT8 max;
  INT8 value;
  INT8 step;        //步长,每次+/- 的值 // step 0 for progress display
  const void (*callback)(INT32 res_id);
}slider_t;

typedef struct
{
  INT32 min;
  INT32 max;
  INT32 value;
}progress_t;

typedef struct
{                          
	INT8    mode;
	INT8    Language;
}Scroll_t;


typedef struct
{
	UINT16 HeadIcon;//头部图标的ID值
	UINT16 HeadStr;//头部信息的ID值
	INT8 *string;//信息显示区的显示的字串首址
}dialog_t;

#define  AP_MAX_FROMAT      9

extern const char *date_formats[AP_MAX_FROMAT];

#define GUI_DisplayLock(islock)                     {if(islock) GUI_ResShowPic(GUI_IMG_LOCK,0,0);}
#define GUI_DisplayNumberD(number, part)        GUI_DisplayNumbers(POSITION_D,part==0?0:8,3,GUI_IMG_SMALL_NUM, number)
#define GUI_DisplayNumberCD(number, part)       GUI_DisplayNumbers(POSITION_C,part==0?0:8,3,GUI_IMG_SMALL_NUM, number)
#define GUI_DisplayTimeF(time, part)        GUI_DisplayTime(POSITION_F,part==0?0:8,0,GUI_IMG_SMALL_NUM, time)
#define GUI_DisplayTimeEF(time, part)       GUI_DisplayTime(POSITION_E,part==0?0:8,GUI_TIME_DISPLAY_ALL,GUI_IMG_SMALL_NUM, time)
#define GUI_DisplayStringG(string)              GUI_DisplayText(POSITION_G,32,string)

/*Display String*/
#define GUI_DisplayIconA(id)                GUI_ResShowPic(id, POSITION_A, 0)
#define GUI_DisplayIconB(id)                GUI_ResShowPic(id, POSITION_B, 0)
#define GUI_DisplayIconC(id)                GUI_ResShowPic(id, POSITION_C, 0)
#define GUI_DisplayIconD(id)                GUI_ResShowPic(id, POSITION_D, 0)
#define GUI_DisplayIconE(id)                GUI_ResShowPic(id, POSITION_E, 0)

#define MENU_CALLBACK_QUERY_ACTIVE       1
#define MENU_CALLBACK_QUERY_VISIABLE     2
#define MENU_CALLBACK_QUERY_TEXT         3

typedef INT32 (*menu_callback)(INT32 type, INT32 value, INT32 param, UINT8 **string);

#define LIST_TYPE_MULTI_SELECT           0
#define LIST_TYPE_SINGLE_SELECT          1
#define LIST_TYPE_CUSTOM_IMAGE           2
#define LIST_TYPE_HORIZONTAL             4

#define LIST_CALLBACK_GET_ITEM           1   // return item select status and string
#define LIST_CALLBACK_SELECT_ITEM        2   // an item selected, retun 0 to end list
#define LIST_CALLBACK_CANCEL             3

typedef INT32 (*list_callback)(INT32 type, INT32 value, INT8 **string);

#define GUI_TIME_DISPLAY_AUTO               0
#define GUI_TIME_DISPLAY_HOUR_MINUTE        1
#define GUI_TIME_DISPLAY_MINUTE_SECOND      2
#define GUI_TIME_DISPLAY_ALL                3

INT32  GUI_Delete(file_location_t *location, UINT8 type,UINT8 string_id);
INT32  GUI_DeletePlayList(file_location_t *location, UINT8 type,UINT8 string_id);
INT32  GUI_Directory(file_location_t *location, UINT8 type,UINT8 string_id);
INT32  GUI_PlayList(file_location_t *location, UINT8 type,UINT8 string_id);
INT32 GUI_Display_Menu(UINT8 menu_Id, menu_callback callback);
INT32  GUI_Slider(slider_t *slider, const void (*callback)(INT32 res_id));
INT32  GUI_Display_Progress(const progress_t *progress);
void GUI_DisplayBattaryLever(void);
INT32 GUI_Display_List(UINT8 style, UINT16 image_id, UINT16 count, UINT16 active, list_callback callback, BOOL is_dynamic);

INT32  GUI_Dialog(dialog_t *dialog,INT32 string_id);
void   GUI_SetScrollRegin( const region_t *region, INT8 step);
void   GUI_Scroll( const INT8 *scrollstr, BOOL mode); //(字符串首址，是否初始化true为要初始化)
void GUI_Reset_Scroll(void);
//UINT32 GUI_DisplayMessage(INT16 title, INT16 message, INT8 *string,UINT8 flag);
UINT32 GUI_DisplayMessageWithCallback(    INT16 title,INT16 message,UINT8 *string,UINT8 audio_id,void(*callbackFunc)(void),UINT8 flag);
void GUI_DisplayNumbers(INT32 x, INT32 y, UINT8 size, UINT16 img_id, UINT32 number);
void GUI_DisplayTime(INT32 x, INT32 y, UINT8 type, UINT16 img_id, UINT32 time);
void GUI_DisplayTimeText(INT32 x, INT32 y, UINT8 type, UINT32 time);
void GUI_DisplayDateText(INT32 x, INT32 y, UINT8 islong, UINT32 date); // long = 0, year in two numbers, long = 1, year in four numbers
void GUI_DisplayTimeLEDs(UINT32 time);
void GUI_InputNumber(UINT32 input_num, UINT16 max_len, void(*callbackFunc)(UINT32));

#define KEY_MAP_VALUE_MAXLEN               5
typedef struct
{
    UINT32 key;
    UINT8 len;
    char value[KEY_MAP_VALUE_MAXLEN];
}tKeyMapTable;/*kye map table*/

extern const tKeyMapTable _gKeyMapTable[];
extern const UINT8 _gKeyCount;
#define KEYMAP_COUNT _gKeyCount
typedef INT32 (*searchlist_callback)(INT32 type, INT32 value, INT8 **string, UINT16 *count, INT8 *searchtext);
#define SEARCHLIST_CALLBACK_GET_ITEM           1   // return item select status and string
#define SEARCHLIST_CALLBACK_SELECT_ITEM        2   // an item selected, retun 0 to end list
#define SEARCHLIST_CALLBACK_TEXT               3   // return text
#define SEARCHLIST_CALLBACK_CANCEL             4
INT32 GUI_DisplaySearchList(char* string, INT32 maxlen, searchlist_callback callback);

#define GUI_INPUT_TYPE_NUMBER              0
#define GUI_INPUT_TYPE_TEXT                1

#define GUI_INPUT_MAXLNE            48

typedef INT32 (*input_callback)(INT8* string);
// max len must < 48
INT32 GUI_InputText(INT8* string, INT32 strlen, INT8 type, BOOL ispwd, INT32 maxlen, input_callback callback);



#endif/*_AP_GUI_H_*/

