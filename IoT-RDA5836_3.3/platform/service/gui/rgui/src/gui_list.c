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
#include "hal_host.h"
#ifndef WIN32
#include "hal_debug.h"
#endif

extern BOOL ui_auto_update;
extern BOOL ui_auto_select;
extern BOOL ui_timeout_exit;

#define  POSITION_LISTDERBG    (LCD_WIDTH-7)
#define  POSITION_MENUL        (2)
#define  POSITION_MENUR        (LCD_WIDTH-13)

#if APP_SUPPORT_RGBLCD==1
#define LIST_ITEM_PAGE      6
#elif APP_SUPPORT_LCD==1
#define LIST_ITEM_PAGE      3
#else
#define LIST_ITEM_PAGE      1
#endif

typedef struct
{
    UINT16 count;
    UINT16 num;
    UINT16 active;
    UINT16 image_id;

    region_t rect;
    list_callback callback;

    UINT8 type;
    UINT8 scroll_timer_id;

    BOOL direct;
    BOOL update;
} GUI_LIST_T;

typedef GUI_LIST_T* GUI_LIST_PTR;

//Prevent re-entry gui_list screen in some case!
//static GUI_LIST_PTR s_lis_ptr = NULL; 
#define g_list_count        (s_lis_ptr->count)
#define g_list_active       (s_lis_ptr->active)
#define g_list_image_id     (s_lis_ptr->image_id)
#define g_list_callback     (s_lis_ptr->callback)
#define g_list_type         (s_lis_ptr->type)
#define scroll_timer_id     (s_lis_ptr->scroll_timer_id)
#define list_rect           (s_lis_ptr->rect)
#define list_direct         (s_lis_ptr->direct)
#define list_update         (s_lis_ptr->update)
#define list_num            (s_lis_ptr->num)


static GUI_LIST_PTR NewGUIList(void)
{
    GUI_LIST_PTR p;

    p=(GUI_LIST_PTR) COS_MALLOC(sizeof(GUI_LIST_T));
    APP_ASSERT(p);

    memset(p, 0, sizeof(GUI_LIST_T));

    p->direct = FALSE;
    p->update = TRUE;
    p->callback = NULL;

    return p;
}

VOID GUIListScrollTimerHandler(VOID *param)
{
    SendAppEvent(EV_UI_FW_REDRAW, 0x2/*just redraw scroll text*/);
}

VOID GUIListStartScrollTimer(VOID)
{
    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    if(scroll_timer_id)
    {
        mmi_cancel_timer(scroll_timer_id);
    }    
    scroll_timer_id = mmi_timer_create(5, GUIListScrollTimerHandler, NULL, COS_TIMER_MODE_PERIODIC);
}

VOID GUIListStopScrollTimer(VOID)
{
    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    if(scroll_timer_id)
    {
        mmi_cancel_timer(scroll_timer_id);
        scroll_timer_id = 0;
    }
}


void GUI_ShowList(UINT32 param)
{
    int i, n, cur_group;
    INT8 *string;
    INT32 select;
    region_t    active_region= {0};

    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    if(g_list_count == 5)
    {
        while(0);
    }
#if APP_SUPPORT_LCD==1
    if(g_list_type&LIST_TYPE_HORIZONTAL)
    {
#if APP_SUPPORT_RGBLCD==1
        if(!list_update)
#endif
        {
            //what page?
            cur_group = g_list_active /LIST_ITEM_PAGE;
            n = cur_group*LIST_ITEM_PAGE;
            GUI_ClearScreen(NULL);
            GUI_ResShowImage(GUI_IMG_BG_MAIN, 0, 0, 0);
            //show icons & text
            for(i=0; i<LIST_ITEM_PAGE; i++)
            {
                if(n+i >= g_list_count) break;   //处理不足4个的情况
                select = g_list_callback(LIST_CALLBACK_GET_ITEM, i+n, &string);
                if(i+n == g_list_active)
                {
#if APP_SUPPORT_RGBLCD==1
                    GUI_DisplayTextCenter(10, string);
                    if(i < 3)
                        GUI_ResShowImage(g_list_image_id+1, select,   12+55*i, 36);
                    else GUI_ResShowImage(g_list_image_id+1, select,   12+(55*(i-3)), 80);
#else
                    GUI_DisplayTextCenter(0, string);
                    GUI_ResShowImage(g_list_image_id+1, select,   12+40*i, 24);
#endif
                }
                else
#if APP_SUPPORT_RGBLCD==1
                    if(i < 3)
                        GUI_ResShowImage(g_list_image_id, select,   12+55*i, 36);
                    else GUI_ResShowImage(g_list_image_id, select,   12+(55*(i-3)), 80);
#else
                    GUI_ResShowImage(g_list_image_id, select,   12+40*i, 24);
#endif
            }

            //show active, 2 pixels left
            //GUI_ResShowPic(menu->pic_active[menu->active], menu_pos[menu->active % 5], 24);
            //GUI_ResShowImage(g_pMenu->active_image, GUI_MapIndex(g_menu_active),   8+29*(g_menu_active&3), 24);

            //check if show left arrows
            if(cur_group > 0)
                GUI_ResShowPic(GUI_IMG_CARROWL, POSITION_MENUL, 0);
            else if(g_list_count > 6)
                GUI_ResShowPic(GUI_IMG_CARROWN, POSITION_MENUL, 0);
#if APP_SUPPORT_RGBLCD==1
            //check if show right arrows
            if(cur_group + 1 < g_list_count/6)
#else
            if(cur_group + 1 < g_list_count/3)
#endif
                GUI_ResShowPic(GUI_IMG_CARROWR, POSITION_MENUR, 0);
            GUI_UpdateScreen(NULL);
        }
        // else
        //   GUI_ResShowPic(GUI_IMG_CARROWN, POSITION_MENUR, 0);
        //string = GUI_GetString(g_pMenuItems[ GUI_MapIndex(g_menu_active)].string_id);
    }
    else
#endif //APP_SUPPORT_LCD==1    
    {
#if APP_SUPPORT_RGBLCD==1
        if(!list_update)
#endif
        {
            //what page?
            cur_group = g_list_active /(LCD_HEIGHT>>4);
            n = cur_group*(LCD_HEIGHT>>4);

            active_region.x=  16;
            if(16*(g_list_active-n-1)<0)
                active_region.y=0;
            else
                active_region.y=  16*(g_list_active-n);
            active_region.width  =(LCD_WIDTH-23);
            active_region.height=16;

            if(param == 2)
            {
                // scroll active text
                g_list_callback(LIST_CALLBACK_GET_ITEM, g_list_active, &string);
                GUI_Scroll(string, FALSE);
                /*Invert Active Item*/
                //GUI_InvertRegion(&active_region);
                GUI_UpdateScreen(NULL);
                return ;
            }

            GUI_ClearScreen(NULL);
            GUI_Reset_Scroll();
            //show icons & text
            for(i=0; i<(LCD_HEIGHT>>4); i++)
            {
                if(n+i >= g_list_count) break;   //处理不足一页的情况
                select = g_list_callback(LIST_CALLBACK_GET_ITEM, i+n, &string);
                //if(g_list_active==i+n)
                {
                    if(GUI_GetTextWidth(string)>LCD_WIDTH-16-8)
                    {
                        GUI_SetScrollRegin(&active_region, 5);
                        GUI_Scroll(string, TRUE);
                        GUIListStartScrollTimer();
                    }
                    else
                        GUIListStopScrollTimer();
                }
                if(g_list_type<=LIST_TYPE_SINGLE_SELECT)
                    GUI_ResShowImage(GUI_IMG_SELECTICONS, g_list_type*2+select,  2,  16*i);
                else if(i+n == g_list_active)
                    GUI_ResShowImage(g_list_image_id+1, select,  0,  16*i);
                else
                    GUI_ResShowImage(g_list_image_id, select,  0,  16*i);
                GUI_DisplayText(16, 16*i, string);
                //   GUI_ResShowMultiString(g_pMenuItems[ g_menu_map[n+i]].string_id, 0, 16, 16*i);
            }


            /*Dispaly Active Items*/
            //GUI_ResShowImage(g_pMenu->active_image, g_menu_map[g_menu_active],  0, active_region.y);

            /*Invert Active Item*/
            //GUI_InvertRegion(&active_region);

            /*Display Indication Pic In The Bg Pic*/
            GUI_ResShowPic(GUI_IMG_LISTDERBG, POSITION_LISTDERBG, 0);

#if APP_SUPPORT_RGBLCD==1
            GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_LISTDERBG+1, ((g_list_active*(LCD_HEIGHT-18))/(g_list_count-1)+5));
#else
            GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_LISTDERBG+1, ((g_list_active*(LCD_HEIGHT/8-2))/g_list_count+1)*8);
#endif
            GUI_UpdateScreen(NULL);
        }
#if APP_SUPPORT_RGBLCD==1
        else
        {
            //what page?
            cur_group = g_list_active /(LCD_HEIGHT>>4);
            n = cur_group*(LCD_HEIGHT>>4);
            active_region.x=  16;
            if(16*(g_list_active-n-1)<0)
                active_region.y=0;
            else
                active_region.y=  16*(g_list_active-n);
            active_region.width  =(LCD_WIDTH-23);
            active_region.height=16;

            if(param == 2)
            {
                // scroll active text
                g_list_callback(LIST_CALLBACK_GET_ITEM, g_list_active, &string);
                GUI_Scroll(string, FALSE);
                /*Invert Active Item*/
                GUI_InvertRegion(&active_region);
                GUI_UpdateScreen(NULL);
                return ;
            }
            GUI_ClearScreen(&list_rect);
            //show icons & text
            if(list_direct == TRUE)
            {
                for(i=g_list_active-list_num; i<=g_list_active; i++)
                {
                    select = g_list_callback(LIST_CALLBACK_GET_ITEM, i, &string);
                    if(g_list_active==i)
                    {
                        if(GUI_GetTextWidth(string)>LCD_WIDTH-16-8)
                        {
                            GUI_SetScrollRegin(&active_region, 5);
                            GUI_Scroll(string, TRUE);
                            GUIListStartScrollTimer();
                        }
                        else
                            GUIListStopScrollTimer();
                    }
                    if(g_list_type<=LIST_TYPE_SINGLE_SELECT)
                        GUI_ResShowImage(GUI_IMG_SELECTICONS, g_list_type*2+select,  2,  16*(i%8));
                    else if(i == g_list_active)
                        GUI_ResShowImage(g_list_image_id+1, select,  0,  16*(i%8));
                    else
                        GUI_ResShowImage(g_list_image_id, select,  0,  16*(i%8));
                    GUI_DisplayText(16, 16*(i%8), string);
                    //   GUI_ResShowMultiString(g_pMenuItems[ g_menu_map[n+i]].string_id, 0, 16, 16*i);
                }
            }
            else
            {
                for(i=g_list_active+list_num; i>=g_list_active; i--)
                {
                    select = g_list_callback(LIST_CALLBACK_GET_ITEM, i, &string);
                    if(g_list_active==i)
                    {
                        if(GUI_GetTextWidth(string)>LCD_WIDTH-16-8)
                        {
                            GUI_SetScrollRegin(&active_region, 5);
                            GUI_Scroll(string, TRUE);
                            GUIListStartScrollTimer();
                        }
                        else
                            GUIListStopScrollTimer();
                    }
                    if(g_list_type<=LIST_TYPE_SINGLE_SELECT)
                        GUI_ResShowImage(GUI_IMG_SELECTICONS, g_list_type*2+select,  2,  16*(i%8));
                    else if(i == g_list_active)
                        GUI_ResShowImage(g_list_image_id+1, select,  0,  16*(i%8));
                    else
                        GUI_ResShowImage(g_list_image_id, select,  0,  16*(i%8));
                    GUI_DisplayText(16, 16*(i%8), string);
                    //   GUI_ResShowMultiString(g_pMenuItems[ g_menu_map[n+i]].string_id, 0, 16, 16*i);
                }
            }


            /*Dispaly Active Items*/
            //GUI_ResShowImage(g_pMenu->active_image, g_menu_map[g_menu_active],  0, active_region.y);

            /*Invert Active Item*/
            GUI_InvertRegion(&active_region);

            /*Display Indication Pic In The Bg Pic*/
            GUI_ResShowPic(GUI_IMG_LISTDERBG, POSITION_LISTDERBG, 0);

            GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_LISTDERBG+1, ((g_list_active*(LCD_HEIGHT-18))/(g_list_count-1)+5));
            list_update = FALSE;
            GUI_UpdateScreen(&list_rect);

            active_region.x = POSITION_LISTDERBG;
            active_region.y = 0;
            active_region.width = 6;
            active_region.height = LCD_HEIGHT-1;
            GUI_UpdateScreen(&active_region);
        }
#endif
    }
}

void GUI_Add_item()
{
    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    g_list_count++;
    SendAppEvent(EV_UI_FW_REDRAW, 0);
}

UINT16 GUI_GetListAcive(void)
{
    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    return g_list_active;
}

BOOL GUIListKeyHandler(UINT32 key)
{
    GUI_LIST_PTR s_lis_ptr = (GUI_LIST_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_LIST);

    switch(key)
    {
#if 1
        case AP_KEY_VOLSUB| AP_KEY_PRESS:
            if(g_list_type&LIST_TYPE_HORIZONTAL)
            {
                if(g_list_active < g_list_count-3)
                {
                    g_list_active += 3;
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                    break;
                }
            }
        case AP_KEY_NEXT | AP_KEY_PRESS:
            if(g_list_active < g_list_count-1)
            {
                list_direct = TRUE;
                list_num = 1;
                if(!(g_list_type&LIST_TYPE_HORIZONTAL))
                {
                    list_rect.width = LCD_WIDTH-7;
                    list_rect.height = 32;
                    list_rect.x = 0;
                    list_rect.y = (g_list_active%8)*16;
                    list_update = TRUE;
                }
                g_list_active++;
                if(!(g_list_active % 8) && g_list_active!=0
                   && !(g_list_type&LIST_TYPE_HORIZONTAL))
                {
                    list_update = FALSE;
                }
            }
            else
            {
                //  list_update = FALSE;
                g_list_active = 0;
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case AP_KEY_VOLSUB | AP_KEY_HOLD:
        case AP_KEY_NEXT | AP_KEY_HOLD:
            if(g_list_active < g_list_count-4)
            {
                list_direct = TRUE;
                list_num = 4;
                list_rect.width = LCD_WIDTH;
                list_rect.height = 64;
                list_rect.x = 0;
                list_rect.y = (g_list_active%8)*16;
                if(!(g_list_type&LIST_TYPE_HORIZONTAL))
                    list_update = TRUE;
                g_list_active+=4;
                if(!(g_list_active % 8) && g_list_active!=0)
                    list_update = FALSE;
            }
            else
            {
                //  list_update = TRUE;
                g_list_active = g_list_count-1;
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case AP_KEY_VOLADD | AP_KEY_PRESS:
            if(g_list_type&LIST_TYPE_HORIZONTAL)
            {
                if(g_list_active >= 3)
                {
                    g_list_active -= 3;
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                    break;
                }
            }
        case AP_KEY_PREV | AP_KEY_PRESS:
            if(g_list_active > 0)
            {
                g_list_active--;
                list_direct = FALSE;
                list_num = 1;
                list_rect.width = LCD_WIDTH;
                list_rect.height = 32;
                list_rect.x = 0;
                list_rect.y = (g_list_active%8)*16;
                if(!(g_list_type&LIST_TYPE_HORIZONTAL))
                    list_update = TRUE;

                if(!(g_list_active % 7) && g_list_active!=0)
                    list_update = FALSE;
            }
            else
            {
                //  list_update = FALSE;
                g_list_active = g_list_count-1;
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case AP_KEY_VOLADD | AP_KEY_HOLD:
        case AP_KEY_PREV | AP_KEY_HOLD:
            if(g_list_active > 4)
            {
                g_list_active-=4;
                list_direct = FALSE;
                list_num = 4;
                list_rect.width = LCD_WIDTH;
                list_rect.height = 64;
                list_rect.x = 0;
                list_rect.y = (g_list_active%8)*16;
                if(!(g_list_type&LIST_TYPE_HORIZONTAL))
                    list_update = TRUE;

                if(!(g_list_active % 7) && g_list_active!=0)
                    list_update = FALSE;
            }
            else
            {
                //  list_update = FALSE;
                g_list_active = 0;
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
        case AP_KEY_MODE | AP_KEY_PRESS:   //取消退出
            g_list_callback(LIST_CALLBACK_CANCEL, g_list_active, NULL);
            ui_auto_update = TRUE;
            GobackScreen(0, RESULT_IGNORE);
            break;

        case AP_MSG_WAIT_TIMEOUT:
            if(!ui_timeout_exit)
                break;
            if(!ui_auto_select)
            {
                g_list_callback(LIST_CALLBACK_CANCEL, g_list_active, NULL);
                ui_auto_update = TRUE;
                GobackScreen(0, RESULT_IGNORE);
				break;
            }

        case AP_KEY_PLAY | AP_KEY_PRESS:   //确认选择
			
            key = g_list_callback(LIST_CALLBACK_SELECT_ITEM, g_list_active, NULL);
            if(key == 0)
            {
                ui_auto_update = TRUE;
                GobackScreen(0, 0);
				break;
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        default:
            return FALSE;
#endif
    }

    return TRUE;
}


BOOL GUI_Display_List_MsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        case EV_UI_FW_ON_RESUME:
        	SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;
            
        case EV_UI_FW_REDRAW:
            GUI_ShowList(pEvent->nParam1);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
            GUIListStopScrollTimer();
            break;

        default:
            //key event
            if(GUIListKeyHandler(MESSAGE_ConvertToKeyValue(pEvent)))
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

GUI_LIST_PTR list_debug = 0;
    
INT32 GUI_Display_List(UINT8 style, UINT16 image_id, UINT16 count,
                       UINT16 active, list_callback callback, BOOL is_dynamic)
{

	GUI_LIST_PTR s_lis_ptr = NULL;
	
    if(count == 0)
        return 0;

    s_lis_ptr = NewGUIList();
    list_debug = s_lis_ptr;
    //need_draw_list  = TRUE;
    g_list_count    = count;
    g_list_type     = style;
    g_list_image_id = image_id;
    g_list_callback = callback;
    if(active + 1 > count)
    	active = 0;
    	
    g_list_active   = active;


    ui_auto_update = FALSE;
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_LIST, LED_LOOP_INFINITE);
    #endif 


    EntryNewScreen(SCR_ID_GUI_LIST, GUI_Display_List_MsgHandler, (U8 *)s_lis_ptr, 0, 0);

    return 0;
}




