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

#include "ap_common.h"
#include "ap_gui.h"

#define FirstSectionCode         0xa1
#define SCROLL_SPACE    20

//设置滚屏的区域
//modify by wuxiang
//static region_t scrollregion={0,32,LCD_WIDTH,16};
region_t scrollregion={0,32,LCD_WIDTH,16};
region_t cur_scroll_region = {0};
INT8 scroll_str_index = 0;
UINT8 scroll_str[4][SCROLL_SPACE] = {0};

//modify end
static INT32 scroll_pos;
static INT8 scroll_step;

void GUI_Reset_Scroll(void)
{
	scroll_str_index = 0;
}

void GUI_SetScrollStr( const INT8 * str, UINT8 line) 
{
    strncpy(&scroll_str[line], str, SCROLL_SPACE);
}

/* (字符串首址，是否初始化true为要初始化) */
void GUI_Scroll( const INT8 * scrollstr, BOOL mode) 
{
    INT32 length         =GUI_GetTextWidth(scrollstr);
    int i = 0;
    
    
    if(mode)//第一次进入滚屏处理
    {
        scroll_pos = scrollregion.x;
    
    /*
    if(scroll_str_index == 4)
    {
    memset(scroll_str, 0, sizeof(scroll_str));
    scroll_str_index = 0;
    }
    strcpy(scroll_str[(UINT8)scroll_str_index], scrollstr);
    scroll_str_index++;
    */
    }
    else
    {
        scroll_pos -= scroll_step;
        if(length + scroll_pos < scrollregion.x)
        scroll_pos += length + SCROLL_SPACE;
    }

	for(i = scrollregion.y/16; i < (scrollregion.y+scrollregion.height)/16; i++)
    {
        cur_scroll_region = scrollregion;
        cur_scroll_region.y = i*16;

        GUI_ClearScreen(&cur_scroll_region);
        GUI_DisplayTextRect(&cur_scroll_region, scroll_pos, i*16, scroll_str[i]);
        if(length + scroll_pos + SCROLL_SPACE< scrollregion.width)
        GUI_DisplayTextRect(&cur_scroll_region, scroll_pos + length + SCROLL_SPACE, i*16, scroll_str[i]);
    }
}


void GUI_SetScrollRegin( const region_t *region, INT8 step)
{
    scroll_step = step;
    if(region)
  	{
	    scrollregion.x     =region->x;
		scrollregion.y     =region->y;
		scrollregion.width =region->width;
		scrollregion.height=region->height;
  	}
}

