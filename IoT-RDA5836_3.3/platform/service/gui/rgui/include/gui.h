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
 *
 * REMARKS:
 *
 ***********************************************************************/
 
#ifndef _GUI__H_
#define _GUI__H_


typedef struct
{
  INT16 x;         //列位置
  INT16 y;         //行位置
  INT16 width;     //宽度
  INT16 height;    //高度
}region_t;

#include "tgt_app_cfg.h"
#include "resource_id.h"
#include "bal_resource.h"

#if APP_SUPPORT_RGBLCD==1
#include "gui_rgblcd_display.h"

#define GUI_Initialise(width,height)           gui_rgblcd_initialse(width,height)
#define GUI_UpdateScreen(region)               gui_rgblcd_update_screen(region)
#define GUI_ClearScreen(region)                gui_rgblcd_clear_screen(region)
#define GUI_InvertRegion(region)               gui_rgblcd_invert_Region(region)
#define GUI_VerticalScroll(y)                  gui_rgblcd_vertical_scroll(y)
#define GUI_HorizontalScroll(x)                gui_rgblcd_horizontal_scroll(x)
#define GUI_SetBackColor(color)                gui_rgblcd_set_back_color(color)
#define GUI_SetTextColor(color)                gui_rgblcd_set_text_color(color)
#define GUI_ResShowPic(id, x, y)               gui_rgblcd_display_image(x,y,0,id)
#define GUI_ResShowImage(id,index,x,y)         gui_rgblcd_display_image(x,y,index,id)
#define GUI_ResShowString(id,x,y)              gui_rgblcd_display_string(x, y, id)
#define GUI_DisplayText(x,y,str)               gui_rgblcd_display_text(x, y, str, NULL)
#define GUI_DisplayTextCenter(y,str)           gui_rgblcd_display_text_center(y, str)
#define GUI_DisplayTextRect(region,x,y,str)    gui_rgblcd_display_text(x, y, str, region)
#define GUI_GetTextWidth(str)                  gui_rgblcd_get_text_width(str)
#define GUI_GetString(str_id)                  gui_get_string(str_id)
#define GUI_DisplayMessage(title, message, string, flag)   GUI_DisplayMessageWithCallback(title, message, string,0, NULL, flag)
#define GUI_DisplayMessageWithCB(title, message, string, callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,0,callback, flag)
#define GUI_DisplayMessageTone(title, message, string, tone,flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, NULL, flag)
#define GUI_DisplayMessageToneWithCB(title, message, string, tone,callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, callback, flag)

#elif APP_SUPPORT_LCD==1
#include "gui_lcd_display.h"

#define GUI_Initialise(width,height)           gui_lcd_initialse(width,height)
#define GUI_UpdateScreen(region)               gui_lcd_update_screen(region)
#define GUI_ClearScreen(region)                gui_lcd_clear_screen(region, 0)
#define GUI_InvertRegion(region)               gui_lcd_invert_Region(region)
#define GUI_VerticalScroll(y)                  gui_lcd_vertical_scroll(y)
#define GUI_HorizontalScroll(x)                gui_lcd_horizontal_scroll(x)
#define GUI_ResShowPic(id, x, y)               gui_lcd_display_image(x,y,0,id)
#define GUI_ResShowImage(id,index,x,y)         gui_lcd_display_image(x,y,index,id)
#define GUI_ResShowString(id,x,y)              gui_lcd_display_string(x, y, id)
#define GUI_DisplayText(x,y,str)               gui_lcd_display_text(x, y, str, NULL)
#define GUI_DisplayTextCenter(y,str)           gui_lcd_display_text_center(y, str)
#define GUI_DisplayTextRect(region,x,y,str)    gui_lcd_display_text(x, y, str, region)
#define GUI_GetTextWidth(str)                  gui_lcd_get_text_width(str)
#define GUI_GetString(str_id)                  gui_get_string(str_id)
#define GUI_SetTextColor(color)                
#define GUI_SetBackColor(color)                
#define GUI_DisplayMessage(title, message, string, flag)   GUI_DisplayMessageWithCallback(title, message, string,0,NULL, flag)
#define GUI_DisplayMessageWithCB(title, message, string, callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,0,callback, flag)
#define GUI_DisplayMessageTone(title, message, string, tone,flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, NULL, flag)
#define GUI_DisplayMessageToneWithCB(title, message, string, tone,callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, callback, flag)

#elif APP_SUPPORT_LED8S==1   // 8 段数码管显示
#include "gui_led8s_display.h"

#define GUI_Initialise(width,height)        gui_led8s_clear_screen()
#define GUI_UpdateScreen(region)            gui_led8s_update_screen()
#define GUI_ClearScreen(region)             gui_led8s_clear_screen()
#define GUI_InvertRegion(region)            
#define GUI_VerticalScroll(y)               
#define GUI_HorizontalScroll(x)             
#define GUI_ResShowPic(id, x, y)            gui_led8s_display_icon(id)
#define GUI_ResShowImage(id,index,x,y)      
#define GUI_ResShowString(id,x,y)           
#define GUI_DisplayText(x,y,str)            gui_led8s_display_text(x,str)
#define GUI_DisplayTextCenter(y,str)        
#define GUI_DisplayTextRect(region,x,y,str) 
#define GUI_GetTextWidth(str)               (0)
#define GUI_GetString(str_id)               NULL
#define GUI_SetTextColor(color)             
#define GUI_SetBackColor(color)             
#define GUI_DisplayMessage(title, message, string, flag)	
#define GUI_DisplayMessageWithCB(title, message, string, callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,0,callback, GUI_MSG_FLAG_DISPLAY)
#define GUI_DisplayMessageTone(title, message, string, tone,flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, NULL, flag)
#define GUI_DisplayMessageToneWithCB(title, message, string, tone,callback, flag)   GUI_DisplayMessageWithCallback(title, message, string,tone, callback, flag)

#else // no gui

#define GUI_Initialise(width,height)        
#define GUI_UpdateScreen(region)            
#define GUI_ClearScreen(region)             
#define GUI_InvertRegion(region)            
#define GUI_VerticalScroll(y)               
#define GUI_HorizontalScroll(x)             
#define GUI_ResShowPic(id, x, y)            
#define GUI_ResShowImage(id,index,x,y)      
#define GUI_ResShowString(id,x,y)           
#define GUI_DisplayText(x,y,str)            
#define GUI_DisplayTextCenter(y,str)        
#define GUI_DisplayTextRect(region,x,y,str) 
#define GUI_GetTextWidth(str)               (0)
#define GUI_GetString(str_id)               NULL
#define GUI_SetTextColor(color)             
#define GUI_SetBackColor(color)             
#define GUI_DisplayMessage(title, message, string, flag)	
#define GUI_DisplayMessageWithCB(title, message, string, callback, flag)   BAL_DisplayMessageWithCallback(title, message, string,0,callback, GUI_MSG_FLAG_DISPLAY)
#define GUI_DisplayMessageTone(title, message, string, tone,flag)   BAL_DisplayMessageWithCallback(title, message, string,tone, NULL, flag)
#define GUI_DisplayMessageToneWithCB(title, message, string, tone,callback, flag)   BAL_DisplayMessageWithCallback(title, message, string,tone, callback, flag)

#endif

#endif/*_GUI__H_*/

