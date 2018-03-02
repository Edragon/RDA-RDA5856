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
#include "platform.h"
#include "ap_common.h"
#include "gui.h"
#include "gui_lcd_display.h"
#include "string.h"

#define LCD_GET_OFFSET(x,y)             ((y/8)*lcd_full_rect.width+x)
#define LCD_BIT_POS(x,y)                     (y&7)


#if APP_SUPPORT_LCD==1

static UINT8 *lcd_buffer;

static region_t lcd_full_rect = {0, 0, 0,0};

extern   void lcdd_update_screen(UINT8* Data, const region_t *region);

void gui_lcd_update_screen(const region_t *region)
{
    if(!AP_Support_LCD())
        return;

    if(region == NULL)
        region = &lcd_full_rect;
	
     lcdd_update_screen(lcd_buffer, region);
}

void gui_lcd_clear_screen(const region_t *clrregion, UINT16 value)
{
    INT32 x,y;

    if(clrregion == NULL)
        clrregion = &lcd_full_rect;
    
    for(y=clrregion->y;y<(clrregion->y+clrregion->height) && y<lcd_full_rect.height;y+=8)
        for(x=clrregion->x;x<(clrregion->x+clrregion->width) && x<lcd_full_rect.width;x++)
        {
            if(value)
                lcd_buffer[LCD_GET_OFFSET(x,y)] = 0xff;
            else
                lcd_buffer[LCD_GET_OFFSET(x,y)] = 0x00;
        }
}

void gui_lcd_invert_Region(const region_t *invregion)
{
    INT32 x,y;

    if(invregion == NULL)
        invregion = &lcd_full_rect;

    for(y=invregion->y;y<(invregion->y+invregion->height);y+=8)
        for(x=invregion->x;x<(invregion->x+invregion->width);x++)
        {
            lcd_buffer[LCD_GET_OFFSET(x,y)] ^= 0xff;
        }
}

void gui_lcd_vertical_scroll(INT32 y)
{

}

void gui_lcd_horizontal_scroll(INT32 x)
{

}

void gui_lcd_draw_image(region_t *region, UINT8 *data, region_t *rect)
{
    INT32 x,y,i;

    if(rect == NULL)
        rect = &lcd_full_rect;

    for(i=0, y=region->y;y<(region->y+region->height);y+=8)
      for(x=region->x;x<(region->x+region->width);x++)
        {
            if(x<rect->x|| x>=rect->x+rect->width || y<rect->y  || y>= rect->y+rect->height)
            {
                i++; // skip the unvisible line
                continue;
            }
            lcd_buffer[LCD_GET_OFFSET(x,y)] = data[i++];
        }
}

void gui_lcd_display_text(INT32 x, INT32 y, const  UINT8 *text, region_t *region)
{
    UINT8 *pCharData;
    U16 nWidth;
    U16 nHeight;
    INT32 pos;
    region_t rect;
    UINT32 ch;

    pos = x;

    if(text == NULL)
        return;

    if(region == NULL)
        region = &lcd_full_rect;
    
    while(*text)
    {        
        /*if((*text)&0x80)
        {
            ch =  (*(text+1))|((*(text))<<8);
            text +=2;
        }
        else*/
        {
            ch = *(text);
            text += 1;
        }
        gui_get_font_data(ch, GET_FONT_DATA_TYPE_DEFAULT, &pCharData, &nWidth, &nHeight);
        
        if(pCharData == NULL)
		{
            ch =  (*(text))|((*(text-1))<<8);
            text ++;
	        gui_get_font_data(ch, GET_FONT_DATA_TYPE_DEFAULT, &pCharData, &nWidth, &nHeight);
		}

        if(pCharData == NULL)
		{
            text --;
            ch =  (*(text-1));
            gui_get_font_data(ch, GET_FONT_DATA_TYPE_ASCII, &pCharData, &nWidth, &nHeight);
		}

        if(pCharData != NULL)
        {
            if(pos + nWidth >= region->x)
            {
                rect.x = pos;                     
                rect.y = y;
                rect.width = nWidth;          
                rect.height= nHeight;

                gui_lcd_draw_image(&rect, pCharData, region);

            }
            pos += nWidth;
            if(pos > region->x+region->width)
                break;
        }
    }
}

void gui_lcd_display_text_center(INT32 y, const UINT8 *text)
{
    if(text != NULL)
    gui_lcd_display_text(((INT32)lcd_full_rect.width-(INT32)GUI_GetTextWidth(text))/2,y,text, NULL);
}


void gui_lcd_display_string(INT32 x, INT32 y, U16 str_id)
{
    INT8* text = gui_get_string(str_id);
    if(text != NULL)
        gui_lcd_display_text(x,y,text, NULL);
}

UINT32 gui_lcd_get_text_width(const INT8 *text)
{
    if(text == NULL)
        return 0;
    return (UINT32)strlen(text)*8;
}

void gui_lcd_display_image(INT32 x, INT32 y, INT32 index, U16 img_id)
{
    UINT8 *data, width, height;
    region_t rect;
    INT32 i,j;
    UINT16 len;
    INT32 adjust = 0;

    data = gui_get_image(&width, &height, &len, img_id);
    if(data != NULL)
    {
        rect.x = x;
        rect.y = y;
        rect.width= width;
        rect.height= height;
        gui_lcd_draw_image(&rect, data+index*width*height/8, NULL);
    }
}

void gui_lcd_initialse(UINT8 width, UINT8 heigth)
{
    lcd_full_rect.width = width;
    lcd_full_rect.height = heigth;
    lcd_buffer = COS_Malloc_NoCache(width*heigth/8);
    memset(lcd_buffer, 0, width*heigth/8);
}


#endif //#if APP_SUPPORT_LCD==1

