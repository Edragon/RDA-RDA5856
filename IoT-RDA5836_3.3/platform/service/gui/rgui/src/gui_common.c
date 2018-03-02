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
#include "string.h"
#include "gui_led8s_display.h"
#include "ap_media.h"
#ifndef WIN32
#include "hal_debug.h"
#endif

extern displayconfig_t  g_displayconfig;

typedef struct
{
    INT16 title;
    INT16 StringID;
    INT8 *string;
    UINT8 audio_id;
    BOOL audio_play_finished;
    UINT32 msg_timer_count;
    void(*callbackFunc)(void);
} GUI_DISP_MSG_T, *GUI_DISP_MSG_PTR;

#if APP_SUPPORT_REMOTECONTROL==1
typedef struct
{
    UINT8 timer_id;
    UINT16 num_len;
    UINT16 max_len;
    UINT32 numvalue;
    void(*callbackFunc)(UINT32);
} GUI_INPUT_NUMBER_T, *GUI_INPUT_NUMBER_PTR;

#endif

//请不要把这个变量定义在非IDATA0 段,否则不能初始化
BOOL ui_auto_update=TRUE;   //是否自动刷新屏幕
BOOL ui_auto_select=TRUE;   //是否自动确认
BOOL ui_timeout_exit=TRUE;  //超时是否退出


const char *date_formats[AP_MAX_FROMAT]=
{
    "YY-MM-DD","MM-DD-YY","DD-MM-YY",
    "YY/MM/DD","MM/DD/YY","DD/MM/YY",
    "YY.MM.DD","MM.DD.YY","DD.MM.YY"
};

/**************************************************************************************
 * One level is 5.86mv//0~15, -1 表示正在充电
****************************************************************************************/
void GUI_DisplayBattaryLever(void)
{
#if APP_SUPPORT_LCD==1
    static UINT32 cur_value=0;    //当前电量值
    UINT32 value = GetBattery();
    INT8 batt_type = 0;

    if(AP_Support_LCD())
    {
        if(IsChargePlugIn())
        {
            cur_value++;
#if APP_SUPPORT_RGBLCD==1
            if(cur_value > 5)
#else
            if(cur_value > 9)
#endif
                cur_value = 1;
        }
        else
        {
#if APP_SUPPORT_RGBLCD==1
            value = (value+3) /6;
            if(value > 5)
                value = 5;
#else
            value = (value+5) /10;
            if(value > 9)
                value = 9;
#endif
            if(value == cur_value && ui_auto_update)
                return;
            cur_value = value;
        }

        //GUI_ResShowPic(batt_id[cur_value], POSITION_BATT, 0);
        GUI_ResShowImage(GUI_IMG_BATTLVLS, cur_value, g_displayconfig.position_batt, 0);

        if(ui_auto_update)
            GUI_UpdateScreen(NULL);                             //刷新
    }
#endif
}

#if 1
extern BOOL BAL_DisplayMessageWithCallback_MsgHandler(COS_EVENT *pEvent);

#else
/******************************************************************************
 * GUI_DisplayMessageWithCallback_MsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL GUI_DisplayMessageWithCallback_MsgHandler(COS_EVENT *pEvent)
{
    GUI_DISP_MSG_PTR gui_disp_msg_ptr = (GUI_DISP_MSG_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_MESSAGE);
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        case EV_UI_FW_REDRAW:
            break;

        case EV_UI_FW_ON_RESUME:
            APP_ASSERT(0);  //Should NOT receive resume message!!
            break;

        case EV_UI_FW_ON_PAUSE://Not push history
            DeleteScreenIfPresent(SCR_ID_GUI_MESSAGE);
        case EV_UI_FW_ON_EXIT:
            //Stop play tone
            media_StopInternalAudio();
            mmi_timer_create(0, gui_disp_msg_ptr->callbackFunc,NULL,COS_TIMER_MODE_SINGLE);
            gui_disp_msg_ptr->callbackFunc = NULL;
        	break;

        case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
            gui_disp_msg_ptr->audio_play_finished = TRUE;
            break;

        case EV_KEY_PRESS:
            GobackScreen(0, 0);
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_RTC:
                    if(gui_disp_msg_ptr->msg_timer_count++< 4)
                    {
                        break;
                    }
                case AP_MSG_WAIT_TIMEOUT:
                    if(gui_disp_msg_ptr->audio_play_finished)
                    {
                        GobackScreen(0, 0);
                    }
                case AP_MSG_STANDBY:
                    //do nothing
                    break;

                default:
                    //Common events, DONOT deal it.
                    return FALSE;
            }
            break;
    }

    return TRUE;
}
#endif

/******************************************************************************
 * GUI_DisplayMessageWithCallback:
 * DESCRIPTION: - NOTE: This screen will NEVER push in history!
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
UINT32 GUI_DisplayMessageWithCallback(
    INT16 title,
    INT16 message,
    UINT8 *string,
    UINT8 audio_id,
    void(*callbackFunc)(void),
    UINT8 flag)
{
    //UINT32 key;
    INT8 *str;
    //INT32 time=0;
    GUI_DISP_MSG_PTR gui_disp_msg_ptr;

    app_trace(APP_GUI_TRC, "GUI_DisplayMessageWithCallback,msg=%d,aud=%d,cbk=0x%x,f=%d",
    						message, audio_id, callbackFunc, flag);

    //Draw message
    ui_auto_update=FALSE;
    GUI_ClearScreen(NULL);

    GUI_DisplayBattaryLever();

    str = gui_get_string(message);
    if(str)
        GUI_DisplayTextCenter(g_displayconfig.message_y, str);
    else if(string)
        GUI_DisplayTextCenter(g_displayconfig.message_y, string);

    GUI_UpdateScreen(NULL);                             //刷新
    ui_auto_update=TRUE;


    //Play tone
    if(audio_id != 0)
    {
        media_PlayInternalAudio(audio_id, 1, 0);
    }

    if(flag == GUI_MSG_FLAG_DISPLAY)
    {
        if(callbackFunc)
            callbackFunc();
        return 0;
    }

    //Make sure there is no history
    DeleteScreenIfPresent(SCR_ID_GUI_MESSAGE);

    gui_disp_msg_ptr = (GUI_DISP_MSG_PTR) COS_MALLOC(sizeof(GUI_DISP_MSG_T));
    memset(gui_disp_msg_ptr, 0x0, sizeof(GUI_DISP_MSG_T));
    gui_disp_msg_ptr->title = title;
    gui_disp_msg_ptr->StringID= message;
    gui_disp_msg_ptr->string= string;
    gui_disp_msg_ptr->audio_id = audio_id;
    gui_disp_msg_ptr->callbackFunc= callbackFunc;
    gui_disp_msg_ptr->audio_play_finished = (0 == audio_id) ? TRUE:FALSE;

    EntryNewScreen(SCR_ID_GUI_MESSAGE, BAL_DisplayMessageWithCallback_MsgHandler, (UINT8 *)gui_disp_msg_ptr, 0, 0);

    return 0;
}

/******************************************************************************
 * GUI_DisplayNumbers:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void GUI_DisplayNumbers(INT32 x, INT32 y, UINT8 size, UINT16 img_id, UINT32 number)
{
    INT8 i;
    INT8 width;
    UINT16 len;

    gui_get_image(&width, &i, &len, img_id);
    for(i=size-1; i>=0; i--)
    {
        GUI_ResShowImage(img_id, number%10, x+i*width, y);
        number /= 10;
    }
}

void GUI_DisplayTime(INT32 x, INT32 y, UINT8 type, UINT16 img_id, UINT32 time)
{
    UINT32 sec = time/1000;
    UINT32 part1,part2,part3;
    INT width;

    if(img_id == GUI_IMG_BIG_NUM)
        width = g_displayconfig.big_num_width;
    else if(img_id == GUI_IMG_SMALL_NUM)
        width = g_displayconfig.small_num_width;

    part1 = sec/3600;
    part2 = (sec/60)%60;
    part3 = sec%60;

    if((type==GUI_TIME_DISPLAY_ALL)||(type==GUI_TIME_DISPLAY_HOUR_MINUTE)||(type==GUI_TIME_DISPLAY_AUTO&&sec>=3600))
    {
        GUI_ResShowImage(img_id, part1/10, x, y);
        x += width;
        GUI_ResShowImage(img_id, part1%10, x, y);
        x += width;
        GUI_ResShowImage(img_id, 10, x, y);
        if(img_id == GUI_IMG_BIG_NUM)
            x += g_displayconfig.big_colon_width;
        else if(img_id == GUI_IMG_SMALL_NUM)
            x += g_displayconfig.small_colon_width;
    }
    GUI_ResShowImage(img_id, part2/10, x, y);
    x += width;
    GUI_ResShowImage(img_id, part2%10, x, y);
    x += width;
    if((type==GUI_TIME_DISPLAY_ALL)||(type==GUI_TIME_DISPLAY_MINUTE_SECOND)||(type==GUI_TIME_DISPLAY_AUTO&&sec<3600))
    {
        GUI_ResShowImage(img_id, 10, x, y);
        if(img_id == GUI_IMG_BIG_NUM)
            x += g_displayconfig.big_colon_width;
        else if(img_id == GUI_IMG_SMALL_NUM)
            x += g_displayconfig.small_colon_width;
        GUI_ResShowImage(img_id, part3/10, x, y);
        x += width;
        GUI_ResShowImage(img_id, part3%10, x, y);
    }
}


void GUI_DisplayTimeText(INT32 x, INT32 y, UINT8 type, UINT32 time)
{
    UINT32 sec = time/1000;
    UINT32 part1,part2,part3;
    INT8 buff[10];

    part1 = sec/3600;
    part2 = (sec/60)%60;
    part3 = sec%60;

    if(type == GUI_TIME_DISPLAY_ALL)
        sprintf(buff,"%02d:%02d:%02d", part1, part2, part3);
    else if((type==GUI_TIME_DISPLAY_HOUR_MINUTE)||(type==GUI_TIME_DISPLAY_AUTO&&sec>=3600))
        sprintf(buff,"%02d:%02d", part1, part2);
    else
        sprintf(buff,"%02d:%02d", part2, part3);

    GUI_DisplayText(x, y, buff);
}

void GUI_DisplayDateText(INT32 x, INT32 y, UINT8 islong, UINT32 date)
{
    INT8 buff[12], format[15];
    INT i;
    INT param[3];
    extern const char *date_formats[];
    const char *format_str;

    format_str = date_formats[(UINT8)g_comval->DateFormat];
    for(i=0; i<3; i++)
    {
        format[i*5] = '%';
        format[i*5+1] = '0';
        format[i*5+2] = '2';
        format[i*5+3] = 'd';
        format[i*5+4] = format_str[i*3+2];
        switch(format_str[i*3])
        {
            case 'Y':
                param[i] = ((date>>9));
                if(islong)
                    format[i*5+2] = '4';
                else
                    param[i] = param[i]%100;
                break;
            case 'M':
                param[i] = (date>>5)&0xf;
                break;
            case 'D':
                param[i] = date&0x1f;
                break;
        }
    }

    sprintf(buff, format, param[0], param[1], param[2]);
    GUI_DisplayText(x, y, buff);
}

void GUI_DisplayTimeLEDs(UINT32 time)
{
    UINT32 sec = time/1000;
    UINT32 part1,part2,part3;
    INT8 buff[10];

    GUI_ResShowPic(GUI_ICON_MUSIC|GUI_ICON_COLON,0,0);

    part1 = sec/3600;
    part2 = (sec/60)%60;
    part3 = sec%60;

    if(sec>=3600)
        sprintf(buff,"%02d%02d", part1, part2);
    else
        sprintf(buff,"%02d%02d", part2, part3);

    GUI_DisplayText(0, 0, buff);
}

void GUI_DisplayMultiText(UINT8 *str)
{
#if (0)

    int i = 0;
    BOOL need_draw = FALSE;
    BOOL is_line = FALSE;
    int cpy_len = 0;
    int index = 0;
    int curr_index = 0;
    UINT32 key = 0;
    int line_num = 0;
    int result = 0;
    char *str_arr[60] = {0};
    int page = 0;
    int page_num = 0;
    int j = 0;
    int str_len = strlen(str);
    int deal_len = 0;

    line_num = (str_len >> 4);
    if(str_len % 16)
    {
        line_num++;
        is_line = TRUE;
    }

    page_num = (line_num>>2);
    if(line_num % 4)
        page_num++;

    if(str_len >= 16)
        deal_len = 16;
    else deal_len = str_len;

    while(i < line_num)
    {
        while(cpy_len < deal_len)
        {
            if(str[curr_index] & 0x80)
            {
                cpy_len += 2;
                curr_index += 2;
            }
            else
            {
                cpy_len += 1;
                curr_index += 1;
            }
        }
        index += cpy_len;
        str_arr[i] = (char *)pMalloc(cpy_len+2);
        memset(str_arr[i], 0, cpy_len+2);
        if((i == line_num - 1) && is_line)
        {
            if(str_len >= 16)
                strncpy(str_arr[i], &str[index - cpy_len], str_len & 0x10);
            else strncpy(str_arr[i], &str[index - cpy_len], str_len);
        }
        else
        {
            strncpy(str_arr[i], &str[index - cpy_len], cpy_len);
        }
        cpy_len = 0;
        i++;
    }

    GUI_ClearScreen(NULL);
    need_draw = TRUE;
    if(line_num <= 4)
    {
        for(i = 0; i < line_num; i++)
        {
            GUI_DisplayText(0, i * 16, str_arr[i]);
        }
    }
    else
    {
        for(i = 0; i < 4; i++)
        {
            GUI_DisplayText(0, i * 16, str_arr[i]);
        }
    }

    GUI_UpdateScreen(NULL);
    while(1)
    {
        if(line_num <= 4)
        {
            key = MESSAGE_Wait();
            if((key&0xffff) == AP_KEY_PRESS || (key&0xffff) == AP_KEY_HOLD)
            {
                break;
            }
        }
        else
        {
            if(need_draw)
            {
                GUI_ClearScreen(NULL);
                for(i = (page << 2), j= 0; i < ((page+1) << 2) - 1, j < 4; i++, j++)
                {
                    GUI_DisplayText(0, j * 16, str_arr[i]);
                }
                need_draw = FALSE;
                GUI_UpdateScreen(NULL);
            }

            key = MESSAGE_Wait();
            switch(key)
            {
                case AP_KEY_MODE|AP_KEY_PRESS:
                    goto end;
                    break;
                case AP_KEY_NEXT|AP_KEY_DOWN:
                case AP_KEY_VOLSUB|AP_KEY_DOWN:
                    if(page < page_num-1)
                    {
                        need_draw = TRUE;
                        page++;
                    }
                    break;
                case AP_KEY_PREV|AP_KEY_DOWN:
                case AP_KEY_VOLADD|AP_KEY_DOWN:
                    if(page > 0)
                    {
                        need_draw = TRUE;
                        page--;
                    }
                    break;
                default:
                    result = MESSAGE_HandleHotkey(key);
                    if(result != 0)
                        return result;
            }
        }
    }
end:
    i = 0;
    while(i < line_num)
    {
        if(str_arr[i])
        {
            pFree(str_arr[i]);
            str_arr[i] = NULL;
        }
        i++;
    }
#endif /* 0 */
}

#if APP_SUPPORT_REMOTECONTROL==1

#define INPUT_NUMBER_TIMEOUT            15

void Number_Input_Timeout(void *param)
{
    MESSAGE_SetEvent(AP_MSG_WAIT_TIMEOUT);
}

BOOL GUI_InputNumberMsgHandler(COS_EVENT *pEvent)
{
    GUI_INPUT_NUMBER_PTR number_input_ptr = (GUI_INPUT_NUMBER_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_INPUT_NUM);

    APP_ASSERT(pEvent);
    APP_ASSERT(number_input_ptr);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        case EV_UI_FW_ON_RESUME:
            APP_ASSERT(0 == number_input_ptr->timer_id);
            number_input_ptr->timer_id = mmi_timer_create(INPUT_NUMBER_TIMEOUT, Number_Input_Timeout,NULL,COS_TIMER_MODE_SINGLE);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            ui_auto_update = FALSE;
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);
                GUI_DisplayNumbers(32, 16, number_input_ptr->num_len, GUI_IMG_BIG_NUM, number_input_ptr->numvalue);
                GUI_UpdateScreen(NULL);
            }
#elif APP_SUPPORT_LED8S==1
            {
                INT8 buff[10];

                sprintf(buff,"%4d", number_input_ptr->numvalue);
                GUI_DisplayText(0,0,buff);
                GUI_UpdateScreen(NULL);
            }
#endif
            ui_auto_update = TRUE;
            break;

        case EV_UI_FW_ON_PAUSE:
        case EV_UI_FW_ON_EXIT:
            mmi_cancel_timer(number_input_ptr->timer_id);
            number_input_ptr->timer_id = 0;
            break;

        case AP_MSG_WAIT_TIMEOUT:
            if(number_input_ptr->callbackFunc)
            {
                number_input_ptr->callbackFunc(number_input_ptr->numvalue);
            }
            mmi_cancel_timer(number_input_ptr->timer_id);
            number_input_ptr->timer_id = 0;
            AppGobackScreen();
            break;

        case EV_KEY_PRESS:
            if((pEvent->nParam1 == AP_KEY_MODE) || (pEvent->nParam1 == AP_KEY_PLAY))
            {
                Number_Input_Timeout(NULL);
                break;
            }
        case EV_KEY_LONG_PRESS:
            if((pEvent->nParam1>= KEY_1) && (pEvent->nParam1<= KEY_0))
            {
                if(number_input_ptr->num_len >= number_input_ptr->max_len)
                {
                    Number_Input_Timeout(NULL);
                    break;
                }
                else
                {
                    number_input_ptr->numvalue*= 10;
                    if(pEvent->nParam1<KEY_0)
                    {
                        number_input_ptr->numvalue += pEvent->nParam1;
                    }
                    number_input_ptr->num_len++;
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                }
            }
            mmi_change_timer(number_input_ptr->timer_id, INPUT_NUMBER_TIMEOUT);
            break;

        default:
            //Common events, DONOT deal it.
            return FALSE;
    }

    return TRUE;
}

/******************************************************************************
 * GUI_InputNumber:
 * DESCRIPTION: - 处理输入的数字按键消息
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void GUI_InputNumber(UINT32 input_num, UINT16 max_len, void(*callbackFunc)(UINT32))
{
    GUI_INPUT_NUMBER_PTR  number_input_ptr = NULL;

    //Make sure there is no history
    DeleteScreenIfPresent(SCR_ID_GUI_INPUT_NUM);

    number_input_ptr = (GUI_INPUT_NUMBER_PTR) COS_MALLOC(sizeof(GUI_INPUT_NUMBER_T));
    memset(number_input_ptr, 0x0, sizeof(GUI_INPUT_NUMBER_T));
    number_input_ptr->max_len = max_len;
    number_input_ptr->callbackFunc= callbackFunc;

    EntryNewScreen(SCR_ID_GUI_INPUT_NUM, GUI_InputNumberMsgHandler, (UINT8 *)number_input_ptr, 0, 0);
}
#endif




