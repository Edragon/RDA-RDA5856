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


 
#ifndef _GUI_RES_H_
#define _GUI_RES_H_

#define GET_FONT_DATA_TYPE_DEFAULT              1           // default font of current language
#define GET_FONT_DATA_TYPE_ASCII                    2

#define MENU_FLAG_QUERY_ACTIVE          1
#define MENU_ITEM_FLAG_INVISABLE          1
#define MENU_ITEM_FLAG_CHECK          2
#define MENU_ITEM_FLAG_RAWTEXT         4

#define FESTIVAL_TYPE_SOLAR     0
#define FESTIVAL_TYPE_LUNAR     1
#define FESTIVAL_TYPE_WEEK      2

typedef struct {
    U16 string_id; // string id for the item
    U16 result; // menu result
    U16 param; // result param
    U8 action; // menu action
    U8 flags; // 0x01, invisable,  0x02, check if invisable runtime, 0x04, raw text
} MenuItem_t;

typedef struct {
    U8 menu_id; // menu id
    U8 count; // count of menu item
    U8 style;  // horizontal or vertical
    U8 flags; // 1, query active item before display menu
    U16 image_id; // image id
    U16 active_image; // active image id
} Menu_t;

typedef struct {
    U8 style;
    U8 month;
    U8 week;
    U8 day;
    U8 *name;
} Festival_t;

void gui_load_resource(UINT8 lang);
INT8* gui_get_string(UINT16 StringId);
INT8* gui_get_image(UINT8 *width, UINT8 *height, UINT16 *size, UINT16 img_id);
UINT8 *gui_get_audio(UINT8 audio_id, UINT8 *type, UINT32 *length);
UINT8 *gui_get_led_pattern(UINT8 pattern_id, UINT32 *length);
UINT32 gui_get_font_data(UINT32 Ch, UINT8 font, UINT8 **ppCharData,UINT16* pnWidth, UINT16* pnHeight);
UINT32 gui_unicode2local(UINT8 *buff, UINT32 length);
BOOL ReadConfigData(void *pbuf, UINT8 id, UINT32 len);
BOOL LoadFactorySetting(void *pbuf, UINT32 len);
Menu_t *gui_get_menu(UINT32 menu_id, MenuItem_t **items);
BOOL gui_get_festival(UINT8 index, Festival_t *pFestival);
void *GetCalendarData(void);
    
#endif/*_GUI_RES_H_*/

