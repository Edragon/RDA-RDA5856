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

extern BOOL ui_auto_update;
extern BOOL ui_auto_select;

#define  POSITION_LISTDERBG    (121)
#define  POSITION_MENUL        (2)
#define  POSITION_MENUR        (115)

#define MENU_TIMEOUT            (5000)

#define MENU_MAX_ITEMS          16
#define MENU_MAX_LEVEL          4

Menu_t *g_pMenu;
MenuItem_t *g_pMenuItems;
menu_callback g_menu_callback;
INT8 g_menu_map[MENU_MAX_ITEMS];
INT8 g_menu_active; // active menu item
INT8 g_menu_count; // total visiable item number
INT8 g_menu_level; // level in the tree
INT8 g_menu_tree[MENU_MAX_LEVEL][2]; // first is menu_id, second is active index

UINT8 GUI_MapIndex(UINT8 index)
{
    if(index < MENU_MAX_ITEMS)
        return g_menu_map[index];
    else
        return index;
}

UINT16 GUI_GetMenuResult(UINT16 *param)
{
    if(param)
        *param = g_pMenuItems[GUI_MapIndex(g_menu_active)].param;
    return g_pMenuItems[GUI_MapIndex(g_menu_active)].result;
}

BOOL GUI_LoadMenu(UINT8 menu_id, menu_callback callback)
{
    INT32 i,j,value;

    g_pMenu = gui_get_menu(menu_id, &g_pMenuItems);

    if(!g_pMenu)
        return FALSE;
    g_menu_active = 0;
    g_menu_count  = 0;

    MESSAGE_Initial(g_comval);

    if(g_pMenu->flags & MENU_FLAG_QUERY_ACTIVE)
    {
        value = callback(MENU_CALLBACK_QUERY_ACTIVE, menu_id, 0, NULL) ;
    }
	else
	{
		value = 0;
	}

    for(i=0,j=0; i<g_pMenu->count; i++)
    {
        if(g_pMenuItems[i].flags&MENU_ITEM_FLAG_INVISABLE)
            continue;
        if(g_pMenuItems[i].flags&MENU_ITEM_FLAG_CHECK)
        {
            if(callback(MENU_CALLBACK_QUERY_VISIABLE, g_pMenuItems[i].result, g_pMenuItems[i].param, NULL) == 0)
                continue;
        }

        if(g_pMenuItems[i].action == 1) //action == result
        {
            if(g_pMenuItems[i].result == value)
            {
                g_menu_active = j;
            }
        }
        else if(g_pMenuItems[i].param == value)
        {
            g_menu_active = j;
        }

        if(j < MENU_MAX_ITEMS)
            g_menu_map[j++] = i;
        g_menu_count ++;
    }
    if((g_pMenu->flags & MENU_FLAG_QUERY_ACTIVE)==0)
        g_menu_active = 0;

    return TRUE;
}


void GUI_ShowMenu(menu_callback callback)
{
    int i, n, cur_group;
    INT8 *string;
//clear screen
    GUI_ClearScreen(NULL);

    //what page?
    cur_group = g_menu_active /4;
    n = cur_group*4;

    if(g_pMenu->style==0)//水平
    {
        //show icons & text
        for(i=0; i<4; i++)
        {
            if(n+i >= g_menu_count) break;   //处理不足5个的情况
            GUI_ResShowImage(g_pMenu->image_id, GUI_MapIndex(n+i),   8+29*i, 24);
        }

        //show active, 2 pixels left
        //GUI_ResShowPic(menu->pic_active[menu->active], menu_pos[menu->active % 5], 24);

        GUI_ResShowImage(g_pMenu->active_image, GUI_MapIndex(g_menu_active),   8+29*(g_menu_active&3), 24);

        //check if show left arrows
        if(cur_group > 0)
            GUI_ResShowPic(GUI_IMG_CARROWL, POSITION_MENUL, 0);
        //else
        // GUI_ResShowPic(GUI_IMG_CARROWN, POSITION_MENUL, 0);

        //check if show right arrows
        if(cur_group+1 < g_menu_count/4)
            GUI_ResShowPic(GUI_IMG_CARROWR, POSITION_MENUR, 0);
        // else
        //   GUI_ResShowPic(GUI_IMG_CARROWN, POSITION_MENUR, 0);
        string = GUI_GetString(g_pMenuItems[ GUI_MapIndex(g_menu_active)].string_id);
        GUI_DisplayTextCenter(0, string);

    }
    else/*列表模式*/
    {
        region_t    active_region= {0};

        //show icons & text
        for(i=0; i<4; i++)
        {
            if(n+i >= g_menu_count) break;   //处理不足4个的情况
            GUI_ResShowImage(g_pMenu->image_id, GUI_MapIndex(n+i),  0,  16*i);
            if(g_pMenuItems[GUI_MapIndex(n+i)].flags&MENU_ITEM_FLAG_RAWTEXT)
            {
                callback(MENU_CALLBACK_QUERY_TEXT, g_pMenuItems[GUI_MapIndex(n+i)].result, g_pMenuItems[GUI_MapIndex(n+i)].param, (UINT8 **)&string);
                GUI_DisplayText(16, 16*i, string);
            }
            else
                GUI_ResShowString(g_pMenuItems[ GUI_MapIndex(n+i)].string_id, 16, 16*i);
        }

        active_region.x=  16;
        if(16*(g_menu_active-n-1)<0)
            active_region.y=0;
        else
            active_region.y=  16*(g_menu_active-n);
        active_region.width  =104;
        active_region.height=16;

        /*Dispaly Active Items*/
        GUI_ResShowImage(g_pMenu->active_image, GUI_MapIndex(g_menu_active),  0, active_region.y);

        /*Invert Active Item*/
        GUI_InvertRegion(&active_region);

        GUI_ResShowPic(GUI_IMG_LISTDERBG, POSITION_LISTDERBG, 0);
        /*Display Indication Pic In The Bg Pic*/
        GUI_ResShowPic(GUI_IMG_LISTDER, POSITION_LISTDERBG+1, ((g_menu_active*(LCD_HEIGHT/8-2))/g_menu_count+1)*8);
    }

    GUI_UpdateScreen(NULL);
}

INT32 menu_list_callback(INT32 type, INT32 value, INT8 **string)
{
//    UINT32 LEntry;
//    INT32 result;
    INT32 index;
//    INT i;
    index = GUI_MapIndex(value);

    if(LIST_CALLBACK_GET_ITEM == type)
    {
        if(g_menu_callback && (g_pMenuItems[index].flags&MENU_ITEM_FLAG_RAWTEXT))
        {
            g_menu_callback(MENU_CALLBACK_QUERY_TEXT, g_pMenuItems[index].result, g_pMenuItems[index].param, (UINT8 **)string);
        }
        else
            // GUI_ResShowString(g_pMenuItems[ GUI_MapIndex(n+i)].string_id, 16, 16*i);
            *string = GUI_GetString(g_pMenuItems[index].string_id);
        return index;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        g_menu_active = value;
        return 0;
    }

    return 0;
}

BOOL GUI_Display_Menu_MsgHandler(COS_EVENT *pEvent)
{
    MenuItem_t *item;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        case EV_UI_FW_REDRAW:
            GUI_Display_List(LIST_TYPE_CUSTOM_IMAGE+(g_pMenu->style==0?4:0), g_pMenu->image_id, g_menu_count, g_menu_active, menu_list_callback, FALSE);
            break;

        case EV_UI_FW_ON_PAUSE:
            //save cur menu state to hisotry buffer
            break;


		//We always consider that it's returned from GUI_LIST screen.
        case EV_UI_FW_ON_RESUME:
            if(pEvent->nParam1 == RESULT_IGNORE)
            {
                if(g_menu_level > 0)
                {
                    g_menu_level --;
                    GUI_LoadMenu(g_menu_tree[(UINT8)g_menu_level][0], g_menu_callback);
                    g_menu_active = g_menu_tree[(UINT8)g_menu_level][1];
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                    break;
                }
            }
            if(pEvent->nParam1 != 0)
            {
                GobackScreen(0, pEvent->nParam1);
                break;
            }

            item = g_pMenuItems+GUI_MapIndex(g_menu_active);
            if(item->action == 0) // submenu
            {
                if(item->result > 0) // enter submenu
                {
                    g_menu_tree[(UINT8)g_menu_level][1] = g_menu_active;
                    if(GUI_LoadMenu(item->result, g_menu_callback))
                    {
                        g_menu_level ++;
                        if(g_menu_level >= MENU_MAX_LEVEL)
                        {
                            memcpy(&g_menu_tree[0][0], &g_menu_tree[1][0], (MENU_MAX_LEVEL-1)*sizeof(INT8)*2);
                            g_menu_level --;
                        }
                        g_menu_tree[(UINT8)g_menu_level][0] = item->result;
                    }
                }
                else if(g_menu_level>0)// return parent menu
                {
                    g_menu_level --;

                    GUI_LoadMenu(g_menu_tree[(UINT8)g_menu_level][0], g_menu_callback);
                    g_menu_active = g_menu_tree[(UINT8)g_menu_level][1];
                }
                SendAppEvent(EV_UI_FW_REDRAW, 0);

                break;
            }
            else if(item->action == 1) // result
            {
                //GobackScreen(0, 0);
                SendAppEvent(EV_UI_FW_SWITCH_MOD, item->result);
                break;
            }
            else if(item->action == 2) // key
            {
                SendAppEvent(item->result, item->param);
            }
            else if(item->action == 3) // message
            {
                SendAppEvent(item->result, 0);
            }
            GobackScreen(0, 0);
            break;

        case EV_UI_FW_ON_EXIT:
            break;

        default:
            return FALSE;
    }

    return TRUE;
}


INT32 GUI_Display_Menu(UINT8 menu_id, menu_callback callback)
{

    g_menu_level = 0;
    g_menu_tree[0][0] = menu_id;
    g_menu_callback = callback;

    if(!GUI_LoadMenu(menu_id, callback))
        return 0;


    EntryNewScreen(SCR_ID_GUI_MENU, GUI_Display_Menu_MsgHandler, NULL, 0, 0);

    return 0;
}



