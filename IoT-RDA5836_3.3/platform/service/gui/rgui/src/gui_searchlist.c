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

#if 0
#define SEATCHLIST_SWTICH_TIME                (1*10)
#define SEATCHLIST_TEXT_MAXLEN               5

#define  POSITION_SEARCHLISTDERBG    (121)


static UINT8 timer_started = 0;/*0: time not start,  1: timer started*/
static UINT8 first_entry = 1;/*0:,  1: first entry or char is null*/
static UINT32 last_press_key = 0;/*record last press key*/
static UINT32 press_key_counter = 0;/*counter for press key*/
static UINT8 press_key_timer = 0;/*counter timer*/
static UINT8 text_cursor = 0;/*cursor of input text*/
static UINT8 text_len = SEATCHLIST_TEXT_MAXLEN;/*input text upper limit*/
static char text[SEATCHLIST_TEXT_MAXLEN+1];/*input text*/


static UINT16 g_searchlist_count;
static UINT16 g_searchlist_active;
static UINT8 need_draw_searchlist;
static UINT8 g_searchlist_scroll_flag;

static searchlist_callback g_searchlist_callback;

const tKeyMapTable _gKeyMapTable[] =
{
    {AP_KEY_NUM1, 1,  {'1', 0,   0,   0,   0}},
    {AP_KEY_NUM2, 4,  {'2', 'A', 'B', 'C', 0}},
    {AP_KEY_NUM3, 4,  {'3', 'D', 'E', 'F', 0}},
    {AP_KEY_NUM4, 4,  {'4', 'G', 'H', 'I', 0}},
    {AP_KEY_NUM5, 4,  {'5', 'J', 'K', 'L', 0}},
    {AP_KEY_NUM6, 4,  {'6', 'M', 'N', 'O', 0}},
    {AP_KEY_NUM7, 5,  {'7', 'P', 'Q', 'R', 'S'}},
    {AP_KEY_NUM8, 4,  {'8', 'T', 'U', 'V', 0}},
    {AP_KEY_NUM9, 5,  {'9', 'W', 'X', 'Y', 'Z'}},
    {AP_KEY_STAR, 2,  {'*', '+', 0,   0,   0}},
    {AP_KEY_NUM0, 1,  {'0', 0,   0,   0,   0}},
    {AP_KEY_HASH, 1,  {'#', ' ', 0,   0,   0}}
};

const UINT8 _gKeyCount = sizeof(_gKeyMapTable)/sizeof(tKeyMapTable);
static UINT16 _AnsiiToUnicode(S8 *pOutBuffer, S8 *pInBuffer)
{
	
    INT16 count = -1;
    UINT16 unicode = 0;
    UINT8 arrOut[2];

	while(*pInBuffer != '\0')
	{
        unicode = ((UINT16)*pInBuffer)&0x00ff;
        if(unicode < 256)
        {
			arrOut[0] = *((UINT8*)(&unicode));
			arrOut[1] = 0;
        }
        else
        {
			arrOut[0] = *((UINT8*)(&unicode));
			arrOut[1] = *(((UINT8*)(&unicode)) + 1);
        }

		pOutBuffer[++count] = arrOut[0];
		pOutBuffer[++count] = arrOut[1];
		pInBuffer++;
	}
	pOutBuffer[++count] = '\0';
    pOutBuffer[++count] = '\0';
	return count + 1;
}

static void _StopSearchCounterTimer(void)
{
    mmi_cancel_timer(press_key_timer);
    press_key_timer = 0;
}
static void _PressKeyCounter_Timeout(void *param)
{
    timer_started = 0;
    if(press_key_counter > 0)
    {
        press_key_counter = 0;
    }
    g_searchlist_callback(SEARCHLIST_CALLBACK_TEXT, -1, NULL, &g_searchlist_count, text);
    g_searchlist_active = 0;
    need_draw_searchlist = 1;
}
static void _StartSearchCounterTimer(void)
{
    timer_started = 1;
    press_key_timer = mmi_timer_create(SEATCHLIST_SWTICH_TIME, _PressKeyCounter_Timeout, NULL, COS_TIMER_MODE_SINGLE);
}
static void _ShowSearch(void)
{
    UINT16 len = 0;
    UINT8 string[(SEATCHLIST_TEXT_MAXLEN+1)*2];// = 0;
    UINT32 y0 = 0;
    region_t rect = {0};

    y0 = 16*3;
    rect.x = 0;
    rect.y = y0;
    rect.width = LCD_WIDTH;
    rect.height = 16;

    GUI_ClearScreen(&rect);

    len = _AnsiiToUnicode(string,text);

    gui_unicode2local(string, (UINT8)len);
    
    GUI_DisplayTextCenter(y0,string);

    GUI_UpdateScreen(&rect);
}
static void _ShowList(void)
{
    UINT32 y0 = 0;
    region_t rect = {0}, active_region = {0};
    UINT32 i = 0, n = 0, cur_group = 0;
    INT8 *string = NULL;


    //what page
    cur_group = g_searchlist_active/3;
    n = cur_group*3;

    y0 = 0;
    rect.x = 0;
    rect.y = y0;
    rect.width = LCD_WIDTH;
    rect.height = 16*3;

    GUI_ClearScreen(&rect);

    active_region.x = 0;
    if(16*(g_searchlist_active-n-1) < 0)  
      active_region.y = 0;
    else
      active_region.y = 16*(g_searchlist_active-n);
    active_region.width = LCD_WIDTH;
    active_region.height = 16;                
    
    if(need_draw_searchlist == 2)
    {
        g_searchlist_callback(LIST_CALLBACK_GET_ITEM, g_searchlist_active, &string, NULL, NULL);
        GUI_Scroll(string, FALSE);
        GUI_InvertRegion(&active_region);                 
        GUI_UpdateScreen(&rect);
        return;
    }
    
    if(g_searchlist_count == 0)
    {
        //GUI_DisplayTextCenter(16,string);
        GUI_UpdateScreen(&rect);
        return;
    }

    //show text
    for(i = 0; i < 3; i++)
    {
        if(n+i >= g_searchlist_count)
            break;   //处理不足3个的情况
            
        g_searchlist_callback(SEARCHLIST_CALLBACK_GET_ITEM, i+n, &string, NULL, NULL);
        if(g_searchlist_active == i+n)
        {
            if(GUI_GetTextWidth(string) > LCD_WIDTH-16-8)
            {
                GUI_SetScrollRegin(&active_region, 5);
                GUI_Scroll(string, TRUE);
                g_searchlist_scroll_flag = 1;
            }
            else
                g_searchlist_scroll_flag = 0; 
        }
        GUI_DisplayText(2, 16*i, string);
    }
    
    /*Invert Active Item*/
    GUI_InvertRegion(&active_region);                 
    
    /*Display Indication Pic In The Bg Pic*/
    GUI_ResShowPic(GUI_IMG_LISTDERBG, POSITION_SEARCHLISTDERBG, 0);
	if(g_searchlist_count == 1)
		GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_SEARCHLISTDERBG+1, 0);
	else
		GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_SEARCHLISTDERBG+1, g_searchlist_active*(16*3-8)/(g_searchlist_count-1));
    GUI_UpdateScreen(&rect);
}
#endif

INT32 GUI_DisplaySearchList(char* string, INT32 maxlen, searchlist_callback callback)
{

#if 0
    BOOL need_draw = TRUE, need_notify = TRUE;
    UINT32 key, key_value, key_type;
    UINT8 i;

    /*init callback*/
    if(callback == NULL)
        return 0;
    g_searchlist_callback = callback;
    
    /*init text value*/    
    last_press_key = 0;
    press_key_counter = 0;
    text_cursor = 0;
    first_entry = 1;
    if(maxlen == 0 || maxlen > SEATCHLIST_TEXT_MAXLEN)
    {
        text_len = SEATCHLIST_TEXT_MAXLEN;
    }
    else
    {
        text_len = maxlen;
    }
    memset(text, 0, SEATCHLIST_TEXT_MAXLEN+1);

    /*init list value*/
    need_draw_searchlist = 1;
    g_searchlist_count = 0;
    g_searchlist_active = 0;
    if(0 == callback(SEARCHLIST_CALLBACK_TEXT, -1, NULL, &g_searchlist_count, text))
        return 0;
    
    while(1)
    {
        if(need_draw == TRUE)
        {
            _ShowSearch();
            need_draw = FALSE;
        }
        if(need_draw_searchlist == 1)
        {
            _ShowList();
            need_draw_searchlist = 0;
        }
        key = MESSAGE_Wait();
        /*process key*/
        if(key != 0 && key != AP_MSG_RTC)
        {
            key_type =  key&0xFFFF;
            if(key_type == AP_KEY_UP)
            {
                key_value = key&0xFFFF0000;
                switch(key_value)
                {
                case AP_KEY_NUM0:
                case AP_KEY_NUM1:
                case AP_KEY_NUM2:
                case AP_KEY_NUM3:
                case AP_KEY_NUM4:
                case AP_KEY_NUM5:
                case AP_KEY_NUM6:
                case AP_KEY_NUM7:
                case AP_KEY_NUM8:
                case AP_KEY_NUM9:
                case AP_KEY_STAR:
                case AP_KEY_HASH:
                    /*1. stop counter timer*/
                    _StopSearchCounterTimer();
                    
                    /*2. map key*/
                    i = 0;
                    for(i = 0; i < KEYMAP_COUNT; i++)
                    {
                        if(_gKeyMapTable[i].key == key_value)
                        {
                            break;
                        }
                    }
                    /*3. process current input char*/
                    if(first_entry == 1)
                    {
                        /*first entyr, key counter ==  0,  input cursor == 0, don't change cursor*/
                        if(i < KEYMAP_COUNT)
                        {
                            text[text_cursor] = _gKeyMapTable[i].value[press_key_counter%_gKeyMapTable[i].len];
                        }
						/*key counter ++*/
                        press_key_counter++;
                        first_entry = 0;
                        need_notify = FALSE;
                    }
                    else if(last_press_key == key_value && timer_started == 1)
                    {
                        /* last key  == current key and counter timer not time out, so change current input char*/
                        if(i < KEYMAP_COUNT)
                        {
                            text[text_cursor] = _gKeyMapTable[i].value[press_key_counter%_gKeyMapTable[i].len];
                        }
                        /*key counter ++*/
                        press_key_counter++;
                        need_notify = FALSE;
                    }
                    else
                    {
                        /*last key != current key or counter timer time out, so insert new char*/
                        /*key counter = 0*/
                        press_key_counter = 0;
                        /*input cursor ++, and check input upper limit*/
                        text_cursor++;
                        if(text_cursor >= text_len)
                        {
                            text_cursor = text_len-1;
                        }
                        if(i < KEYMAP_COUNT)
                        {
                            text[text_cursor] = _gKeyMapTable[i].value[press_key_counter%_gKeyMapTable[i].len];
                        }
                        /*key counter ++*/
                        press_key_counter++;
                    }
                    /*4. record current key*/
                    last_press_key = key_value;
                    /*5. start counter timer*/
                    _StartSearchCounterTimer();
                    if(need_notify)
                    {
                        callback(SEARCHLIST_CALLBACK_TEXT, -1, NULL, &g_searchlist_count, text);
                        g_searchlist_active = 0;
                        need_draw_searchlist = 1;			
                    }
                    need_draw = TRUE;
                    break;
                default:
                    _StopSearchCounterTimer();
                    last_press_key = 0;
                    press_key_counter = 0;
                    need_draw = TRUE;	
                    break;
                }
            }
        }	
        switch(key)
        {
        case AP_KEY_NULL:
            if(g_searchlist_scroll_flag && g_searchlist_scroll_flag < 10)
                g_searchlist_scroll_flag++;

            if(g_searchlist_scroll_flag > 3)
                need_draw_searchlist |= 2;

		if(g_searchlist_count > 0)
		 	need_draw_searchlist = 1;
            break;
        case AP_KEY_VOLSUB| AP_KEY_PRESS:
        case AP_KEY_NEXT | AP_KEY_PRESS:
            if(g_searchlist_active < g_searchlist_count-1)
                g_searchlist_active++;
            else
                g_searchlist_active = 0;
            need_draw_searchlist = 1;
            break;
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
        case AP_KEY_NEXT | AP_KEY_HOLD:
            if(g_searchlist_active < g_searchlist_count-3)
                g_searchlist_active += 3;
            else
                g_searchlist_active = g_searchlist_count-1;
                need_draw_searchlist = 1;
            break;
        case AP_KEY_VOLADD | AP_KEY_PRESS:
        case AP_KEY_PREV | AP_KEY_PRESS:
            if(g_searchlist_active > 0)
                g_searchlist_active--;
            else
                g_searchlist_active = g_searchlist_count-1;
            need_draw_searchlist = 1;
            break;
        case AP_KEY_VOLADD | AP_KEY_HOLD:
        case AP_KEY_PREV | AP_KEY_HOLD:
            if(g_searchlist_active > 3)
                g_searchlist_active -= 3;
            else
                g_searchlist_active = 0;
            need_draw_searchlist = 1;
            break;
        case AP_KEY_MODE | AP_KEY_PRESS:
            /*1. stop counter timer*/
            _StopSearchCounterTimer();
            /*2. process back key, clear or back*/
            if(text[text_cursor] != 0)
            {
                /*has char, so clear current char*/
                text[text_cursor] = 0;
                if(text_cursor > 0)
                {
                    text_cursor--;
                }
                else
                {
                    /*reset firset entry = 1, for change current char*/
                    first_entry = 1;
                }
			    callback(SEARCHLIST_CALLBACK_TEXT, -1, NULL, &g_searchlist_count, text);
                g_searchlist_active = 0;
				need_draw = TRUE;
				need_draw_searchlist = 1;
            }
            else
            {
                /*no char, notify app canceled*/
                callback(SEARCHLIST_CALLBACK_CANCEL, g_searchlist_active, NULL, NULL, NULL);
                return 0;
            }
            break;
        case AP_MSG_WAIT_TIMEOUT:
            //if(!ui_auto_select) 
            {
                callback(SEARCHLIST_CALLBACK_CANCEL, g_searchlist_active, NULL, NULL, NULL);
                //ui_auto_update = TRUE;
                return 0;
            }
            break;
        case AP_KEY_PLAY | AP_KEY_PRESS:   //确认选择
            key = callback(SEARCHLIST_CALLBACK_SELECT_ITEM, g_searchlist_active, NULL, NULL, NULL);
            if(key == 0)
            {
                //ui_auto_update = TRUE;
                return 0;
            }
            need_draw_searchlist = 1;
            break;
        default:
            key = MESSAGE_HandleHotkey(key);
            if(key == RESULT_REDRAW)
            {
            need_draw_searchlist = 1;
            }
            else if(key != 0) 
            {
            return key;
            }
        }
    }


#endif


    return 0;
}


