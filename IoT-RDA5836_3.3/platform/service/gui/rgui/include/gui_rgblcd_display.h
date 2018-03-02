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
 
#ifndef GUI_RGBLCD_DISPLAY_H
#define GUI_RGBLCD_DISPLAY_H

void gui_rgblcd_display_text(INT32 x, INT32 y, const UINT8 *text, region_t *region);
void gui_rgblcd_display_text_center(INT32 y, const UINT8 *text);
void gui_rgblcd_display_string(INT32 x, INT32 y, U16 str_id);
void gui_rgblcd_display_image(INT32 x, INT32 y, INT32 index, U16 img_id);
UINT32 gui_rgblcd_get_text_width(const INT8 *text);

void gui_rgblcd_update_screen(const region_t *region); 
void gui_rgblcd_clear_screen(const region_t *clrregion);
void gui_rgblcd_invert_Region(const region_t *invregion);

void gui_rgblcd_set_text_color(UINT16 color);

void gui_rgblcd_vertical_scroll(INT32 y);
void gui_rgblcd_horizontal_scroll(INT32 x);

void gui_rgblcd_draw_image(UINT16 img_id, region_t *region, UINT8 *data, UINT16 length, INT32 offset, region_t *rect);

void gui_rgblcd_initialse(UINT8 width, UINT8 heigth);

#endif/*GUI_RGBLCD_DISPLAY_H*/


