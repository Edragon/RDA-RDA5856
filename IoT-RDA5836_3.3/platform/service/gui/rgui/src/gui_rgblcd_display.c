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
#include "gui_rgblcd_display.h"
#include "string.h"
#include "mci.h"

#define LCD_GET_OFFSET(x,y)             ((y)*lcd_full_rect.width+x)


#if APP_SUPPORT_RGBLCD==1

static UINT16 *rgblcd_buffer, *image_buffer;

static region_t lcd_full_rect = {0, 0, 0,0};
static UINT16 rgb_lcd_text_color, rgb_lcd_back_color;

extern   void lcdd_update_screen(UINT8* Data, const region_t *region);

void gui_rgblcd_update_screen(const region_t *region)
{
    if(!AP_Support_LCD())
        return;

    if(region == NULL)
        region = &lcd_full_rect;
	
     lcdd_update_screen(rgblcd_buffer, region);
}

void gui_rgblcd_clear_screen(const region_t *clrregion)
{
    INT32 x,y;

    if(clrregion == NULL)
        clrregion = &lcd_full_rect;
    
    for(y=clrregion->y;y<(clrregion->y+clrregion->height) && y<lcd_full_rect.height;y++)
        for(x=clrregion->x;x<(clrregion->x+clrregion->width) && x<lcd_full_rect.width;x++)
        {
            rgblcd_buffer[LCD_GET_OFFSET(x,y)] = rgb_lcd_back_color;
        }
}

void gui_rgblcd_invert_Region(const region_t *invregion)
{
    INT32 x,y;

    if(invregion == NULL)
        invregion = &lcd_full_rect;

    for(y=invregion->y;y<(invregion->y+invregion->height);y++)
        for(x=invregion->x;x<(invregion->x+invregion->width);x++)
        {
            rgblcd_buffer[LCD_GET_OFFSET(x,y)] ^= 0xffff;
        }
}

void gui_rgblcd_set_back_color(UINT16 color)
{
    rgb_lcd_back_color = color;
}


void gui_rgblcd_set_text_color(UINT16 color)
{
    rgb_lcd_text_color = color;
}


void gui_rgblcd_vertical_scroll(INT32 y)
{

}

void gui_rgblcd_horizontal_scroll(INT32 x)
{

}

void gui_rgblcd_draw_image(UINT16 img_id, region_t *region, UINT8 *data, UINT16 length, INT32 offset, region_t *rect)
{
    INT32 x,y,i;
    DEC_UNIT out_info;

    if(rect == NULL)
        rect = &lcd_full_rect;

    MCI_DecodePicture(data, length, image_buffer, lcd_full_rect.width*lcd_full_rect.height*2,
                        region->width, region->height, 0, offset, region->width-1, offset+region->height-1, &out_info, 0, img_id);
    
    for(i=0, y=region->y;y<(region->y+region->height);y++)
      for(x=region->x;x<(region->x+region->width);x++)
        {
            rgblcd_buffer[LCD_GET_OFFSET(x,y)] = image_buffer[i++];
        }
}

void gui_rgblcd_draw_text(region_t *region, UINT8 *data, region_t *rect)
{
    INT32 x,y,i,j;

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
            for(j=0;j<8;j++)
            {
                if(data[i]&(1<<j))
                    rgblcd_buffer[LCD_GET_OFFSET(x,y+j)] = rgb_lcd_text_color;
            }
            i++;
        }
}

void gui_rgblcd_display_text(INT32 x, INT32 y, const  UINT8 *text, region_t *region)
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

                gui_rgblcd_draw_text(&rect, pCharData, region);

            }
            pos += nWidth;  
            if(pos > region->x+region->width)
                break;
        }
    }
}

void gui_rgblcd_display_text_center(INT32 y, const UINT8 *text)
{
    if(text != NULL)
    gui_rgblcd_display_text(((INT32)lcd_full_rect.width-(INT32)GUI_GetTextWidth(text))/2,y,text, NULL);
}


void gui_rgblcd_display_string(INT32 x, INT32 y, U16 str_id)
{
    INT8* text = gui_get_string(str_id);
    if(text != NULL)
        gui_rgblcd_display_text(x,y,text, NULL);
}

UINT32 gui_rgblcd_get_text_width(const INT8 *text)
{
    if(text == NULL)
        return 0;
    return (UINT32)strlen(text)*8;
}

void gui_rgblcd_display_image(INT32 x, INT32 y, INT32 index, UINT16 img_id)
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
        gui_rgblcd_draw_image(img_id, &rect, data, len, index*height, NULL); // +index*width*height*2
    }
}

void gui_rgblcd_initialse(UINT8 width, UINT8 heigth)
{
    lcd_full_rect.width = width;
    lcd_full_rect.height = heigth;
    rgblcd_buffer = COS_Malloc(width*heigth*2);
    image_buffer = COS_Malloc(width*heigth*2);
    memset(rgblcd_buffer, 0, width*heigth*2);
}


#endif //#if APP_SUPPORT_LCD==1

