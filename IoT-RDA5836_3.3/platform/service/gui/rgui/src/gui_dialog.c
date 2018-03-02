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


extern BOOL   ui_auto_update ; 

/*===================================================================
接口: INT32 GUI_Dialog(dialog_t *dialog, INT32 string_id)
input:
    typedef struct
    {
    UINT16 HeadIcon;//头部图标的ID值
    UINT16 HeadStr;//头部信息的ID值
    INT8 *string;//信息显示区的显示的字串首址
    }dialog_t;

    dialog_t dialog: 要显示的对话框的信息
    INT32 string_id: 显示用的语言

output:
    return INT32:  NULL: 选中了OK
                 RESULT_IGNORE: 时间到了未按确定,按了PLAY键,或选中了NO
                 其他非0的值: 当热键返回    
//===================================================================*/
/*
INT32 GUI_Dialog(dialog_t *dialog,INT32 string_id)
{
    UINT32 key;
    BOOL Selectflag = FALSE; //FALSE(NO); TRUE(YES)
    BOOL need_draw  = TRUE;
    
    GUI_ClearScreen(NULL);
    ui_auto_update = FALSE;  


    //显示要求显示的信息
    if (dialog->string != NULL)
    {
		GUI_DisplayText(0, 16, dialog->string);
    }
    
    //显示头部图标    
    if (dialog->HeadIcon)
    {
        GUI_ResShowPic(dialog->HeadIcon, 0, 0);
    }

    //显示头部的信息
    if (dialog->HeadStr)
    {
        GUI_ResShowMultiString(dialog->HeadStr, string_id, 16, 0);            
    }

    //显示选择按钮（默认N0）
    GUI_ResShowPic(GUI_IMG_CDEL_NO,100,0);

    while(1)
    {
        GUI_UpdateScreen(NULL);
        key = MESSAGE_Wait();                          //获取消息
        
        switch (key)
        {
        case AP_KEY_NEXT:                            //YES和NO之间切换
        case AP_KEY_PREV:
        case AP_KEY_NEXT | AP_KEY_HOLD:
        case AP_KEY_PREV | AP_KEY_HOLD:
            if(Selectflag)
            {
                Selectflag = FALSE;
                GUI_ResShowPic(GUI_IMG_CDEL_NO,100,0);
            }
            else
            {
                Selectflag = TRUE;
                GUI_ResShowPic(GUI_IMG_CDEL_YES,100,0);
            }
            break;
        
        case AP_KEY_MODE | AP_KEY_UP:
            if(Selectflag)            
            {//YES时按下MODE键的处理
            }
            else
            {    
                return RESULT_IGNORE;//NO时按下MODE键的处理
            }
        
        case AP_MSG_WAIT_TIMEOUT://8秒退出    
        case AP_KEY_PLAY | AP_KEY_UP:
             return RESULT_IGNORE;
        
        
        default:
            key=MESSAGE_HandleHotkey(key);//热键处理
            if(key == RESULT_REDRAW)
            {
                need_draw=TRUE;
            }
            else if(key!=0)
            {
                return key;                                              
            }
        }//switch(key)            
    }//while(1)
}
*/
