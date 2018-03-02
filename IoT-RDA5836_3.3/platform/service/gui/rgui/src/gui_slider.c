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
#ifndef WIN32
#include "hal_debug.h"
#endif

#define SLIDER_TIMEOUT   (3000)


//垂直进度条位置x
#if APP_SUPPORT_RGBLCD==1
#define SLIDER_BAR_WIDTH      (130)
#elif APP_SUPPORT_LCD==1
#define SLIDER_BAR_WIDTH      (98)
#endif

#define SLIDER_BG_WIDTH       (4)
#define POSITION_SLIDERBAR    (15)

extern region_t res_region;
extern BOOL ui_auto_update;
extern BOOL ui_auto_select;
extern BOOL ui_timeout_exit;


static slider_t *Ptr_slider;

void GUI_SliderRedraw(slider_t *p)
{
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    const region_t bar_region= {0, 16, LCD_WIDTH, 16};   //控件所在区域
    INT32 start, end;        //x坐标

#if APP_SUPPORT_RGBLCD==1
	INT32 pic_y = 48;
    region_t num_region= {50, 80, 91, 24};   //数字区域
#elif APP_SUPPORT_LCD==1
	INT32 pic_y = 24;
    region_t num_region= {35, 40, 91, 24};   //数字区域
#endif
#endif
    INT8 buf[10];    //注意可能含符号 <<-0000>>

	APP_ASSERT(NULL != p);
	
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    GUI_ClearScreen(&bar_region);
    GUI_ResShowPic(GUI_IMG_SBAR, POSITION_SLIDERBAR, pic_y);
    start = POSITION_SLIDERBAR+1;
    end   = (POSITION_SLIDERBAR -1) + SLIDER_BAR_WIDTH * (p->value - p->min) / (p->max - p->min);
    APP_ASSERT(end < LCD_WIDTH);
    //分段显示背景
    while(start+SLIDER_BG_WIDTH < end)
    {
        GUI_ResShowPic(GUI_IMG_SBKGRD, start, pic_y);
        start += SLIDER_BG_WIDTH;
    }
    // 在背景的尽头处显示拖把
    GUI_ResShowPic(GUI_IMG_SSLIDER, start, pic_y);

    //format text <-xxxx>
    sprintf(buf, "<%d/%d>", p->value, p->max);

    //show text <-xxxx>
    GUI_ClearScreen(&num_region);
    GUI_DisplayText(num_region.x, num_region.y, buf);

    GUI_UpdateScreen(NULL);
    
#elif APP_SUPPORT_LED8S==1
    sprintf(buf,"%4d", p->value);
    GUI_DisplayText(0,0,buf);
    GUI_UpdateScreen(NULL);
#endif



}

BOOL GUI_SliderKeyHandler(UINT32 key)
{
    BOOL value_changed=FALSE;    //值是否改变, 需要callback
    UINT8 step = Ptr_slider->step;

    switch( key)
    {
        case AP_KEY_VOLADD|AP_KEY_PRESS:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
            step = 1;
        case AP_KEY_NEXT|AP_KEY_PRESS:
        case AP_KEY_NEXT | AP_KEY_HOLD:
            if((Ptr_slider->value) <= Ptr_slider->max)
            {
                Ptr_slider->value += step;
                if(Ptr_slider->value > Ptr_slider->max)
                    Ptr_slider->value = Ptr_slider->max;
                value_changed = TRUE;
            }
            break;


        case AP_KEY_VOLSUB|AP_KEY_PRESS:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
            step = 1;
        case AP_KEY_PREV|AP_KEY_PRESS:
        case AP_KEY_PREV | AP_KEY_HOLD:
            if((Ptr_slider->value) >= Ptr_slider->min)
            {
                Ptr_slider->value -= step;
                if(Ptr_slider->value < Ptr_slider->min)
                    Ptr_slider->value = Ptr_slider->min;
                value_changed = TRUE;
            }
            break;

        case RESULT_REDRAW://直接退出，因为控件不能重画背景
        case AP_KEY_MODE | AP_KEY_UP:
        case AP_KEY_PLAY | AP_KEY_UP:
            GobackScreen(0, 0); //ok and return
            break;

        case AP_MSG_WAIT_TIMEOUT:
            if(!ui_timeout_exit)
                break;

            GobackScreen(0, 0);
            break;

        default:
            return FALSE;
    }

    if(value_changed)
    {

        if(Ptr_slider->callback != NULL)
        {
            Ptr_slider->callback(Ptr_slider->value);
        }

        SendAppEvent(EV_UI_FW_REDRAW, 0);
        value_changed = FALSE;
    }

    return TRUE;
}

BOOL GUI_SliderMsgHandler(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            #if APP_SUPPORT_LED==1
            LED_SetPattern(GUI_LED_SLIDER, LED_LOOP_INFINITE);
            #endif 

        case EV_UI_FW_REDRAW:
            GUI_SliderRedraw(Ptr_slider);

            if(Ptr_slider->step == 0) // step 0 for progress display
                GobackScreen(0, 0);

            break;
            
		case EV_UI_FW_ON_PAUSE:
			//DeleteScreenIfPresent(SCR_ID_GUI_SLIDER);
			break;
			
        case EV_UI_FW_ON_EXIT:
            break;

        default:
            //key event
            if(GUI_SliderKeyHandler(MESSAGE_ConvertToKeyValue(pEvent)))
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
* Description : 滑动杆控件
*
* Arguments   : slider, 滑动杆结构
        callback, 滑动杆的回调函数,=NULL 表示没有回调函数
*
* Returns     : 结果消息, 或者0
*
* Notes       : 控件本身没有重画功能, 因为控件不能重画背景
*
*********************************************************************************/
INT32 GUI_Slider(slider_t *slider, const void (*callback)(INT32 res_id))
{
    APP_ASSERT(slider);

    MESSAGE_Initial(g_comval);

    Ptr_slider = slider;
    Ptr_slider->callback = callback;
    APP_ASSERT(Ptr_slider->value <= Ptr_slider->max);
    APP_ASSERT(Ptr_slider->value >= Ptr_slider->min);
    APP_ASSERT(Ptr_slider->step >= 0);

    EntryNewScreen(SCR_ID_GUI_SLIDER, GUI_SliderMsgHandler, NULL, 0, 0);

    return 0;
}


