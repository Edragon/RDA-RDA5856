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


#include "platform.h"
#include "gui.h"
#include "bal_resource.h"
#include "string.h"

/* defines for the struct of resource binary file, must be same as defines in IDList.h of resedit project */
enum {
    LIST_ID_LANG = 1,
    LIST_ID_FONT = 2,
    LIST_ID_IMAGE = 3,
    LIST_ID_STRING = 4,
    LIST_ID_MENU = 5,
    LIST_ID_FESTIVAL = 6,
    LIST_ID_SETTING = 7,
    LIST_ID_FACTORY = 8,
    LIST_ID_CALENDAR = 9,
    LIST_ID_MENULIST = 10,
    LIST_ID_AUDIO = 11,
    LIST_ID_PATTERN = 12
};

typedef struct {
    U16 ID;  // resource id
    U16 count;  // count of resource
    UINT32 total_size;  // size include resource header, header list and data
} ResourceHeader_t;

typedef struct {
    U16 font_id; // font id
    UINT32 size;
    UINT32 offset;
} FontHeader_t;

typedef struct {
    U8 audio_id; // audio id
    U8 type;
    U8 lang;
    DWORD size;
    DWORD offset;
} AudioHeader_t;

typedef struct {
    U16 img_id; // image id
    U16 height; // height
    U16 width; // width
    U16 size; // size of image data
    UINT32 offset; // offset from the base of data
} ImageHeader_t;


typedef struct {
    U8 menu_id; // menu id
    U8 count; // count of menu item
    U8 style;  // horizontal or vertical
    U8 flags; // 1, query active item before display menu
    U16 image_id; // image id
    U16 active_image; // active image id
    UINT32 offset; // offset from the base of data
} MenuHeader_t;

typedef struct {
    U8 language;
    U8 font;
    U16 count;
    UINT32 total_size;     // size of total string list, include string list header, string headers and string data
    UINT32 offset;
} StringListHeader_t;

typedef struct{
    U16 string_id;
    U16 length;
    UINT32 offset;  // offset from current list data base
} StringHeader_t;

typedef struct{
    UINT16 language;
    UINT16 count;
    UINT32 total_size;     // size of total string list, include string headers and string data
    UINT32 offset;
} FestivalListHeader_t;

typedef struct{
    UINT8 month;  // bit7~4, style
    UINT8 day;    // bit7~5, week
    UINT16 length;
    UINT32 offset;  // offset from current list data base
} FestivalHeader_t;

typedef struct {
    UINT8 config_id;
    UINT8 size;
    UINT16 offset;
} ConfigHeader_t;

/* struct for font data, must be same as Font.cpp of resedit project */
typedef struct Range
{
	U16	nMin;
	U16	nOffset;
}RangeData_t;

typedef struct _FontData
{
UINT8	 nHeight;
UINT8	 nWidth;
UINT8	 nCharBytes;
UINT8	 nUniType; // 0-no conv, 1-word to byte, 2, table 2->2, 3 table 2->1
UINT16   nMaxChar;
UINT16   nMaxUnicode;
UINT16   nNoOfUniRanges;
UINT16   nNoOfRanges;
UINT32   pUnicodeRanges;
UINT32   pUnicodeData;
UINT32   pDataArray;
RangeData_t rangeList[1];
}fontData_t;


fontData_t *gpCurrentFont = NULL;  // currect font used to get character data
fontData_t *gpDefaultFont = NULL;  // default font of current language;
fontData_t *gpAsciiFont = NULL;        // ascii font

UINT8 gCurrentLang = 0;
UINT8 *gpResourceBase = NULL;   // base address of resource data, will be set by boot monitor


ImageHeader_t *gpImageList = NULL;
U16 gImageCount = 0;
UINT8 *gpImageDataBase = NULL;

AudioHeader_t *gpAudioList = NULL;
U16 gAudioCount = 0;
UINT8 *gpAudioDataBase = NULL;

MenuHeader_t *gpMenuList = NULL;
U16 gMenuCount = 0;
UINT8 *gpMenuDataBase = NULL;

StringHeader_t *gpStringList = NULL;
U16 gStringCount = 0;
UINT8 *gpStringDataBase = NULL;

StringHeader_t *gpLedPatternList = NULL;
U16 gLedPatternCount = 0;
UINT8 *gpLedPatternDataBase = NULL;

FestivalHeader_t *gpFestivalList = NULL;
U16 gFestivalCount = 0;
UINT8 *gpFestivalDataBase = NULL;

UINT8 g_current_language;

void gui_load_resource(UINT8 lang)
{
    UINT8 *res_addr = gpResourceBase;
    ResourceHeader_t *pRes = (ResourceHeader_t*)res_addr;
#if APP_SUPPORT_LCD == 1
    INT32 i;
    UINT8 font_id = 0;
    UINT8 *data_addr;
#endif
    if(res_addr == NULL)
        return;

    while(pRes->ID != 0)
    {
        // load string list
#if APP_SUPPORT_LCD == 1
        if(pRes->ID == LIST_ID_STRING)
        {
            StringListHeader_t *pStrList = (StringListHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            for(i=0; i<pRes->count; i++)
            {
                if(pStrList[i].language == lang)
                {
                    font_id = pStrList[i].font;
                    gpStringList = (StringHeader_t*)(res_addr +sizeof(ResourceHeader_t) + pStrList[i].offset + sizeof(StringListHeader_t) * pRes->count);
                    gStringCount = pStrList[i].count;
                    gpStringDataBase = res_addr +sizeof(ResourceHeader_t) + pStrList[i].offset + sizeof(StringHeader_t) * gStringCount + sizeof(StringListHeader_t) * pRes->count;
                    break;
                }
             }
        }
        // load string list
        if(pRes->ID == LIST_ID_FESTIVAL)
        {
            FestivalListHeader_t *pDayList = (FestivalListHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            for(i=0; i<pRes->count; i++)
            {
                if(pDayList[i].language == lang)
                {
                    gpFestivalList = (FestivalHeader_t*)(res_addr +sizeof(ResourceHeader_t) + pDayList[i].offset + sizeof(FestivalListHeader_t) * pRes->count);
                    gFestivalCount = pDayList[i].count;
                    gpFestivalDataBase = res_addr +sizeof(ResourceHeader_t) + pDayList[i].offset + sizeof(FestivalHeader_t) * gFestivalCount + sizeof(FestivalListHeader_t) * pRes->count;
                    break;
                }
             }
        }
        // load image list
        else if(pRes->ID == LIST_ID_IMAGE)
        {
            gpImageList = (ImageHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            gImageCount = pRes->count;
            gpImageDataBase = res_addr+sizeof(ResourceHeader_t) + sizeof(ImageHeader_t) * pRes->count;            
        }
        // load menu data
        else if(pRes->ID == LIST_ID_MENU)
        {
            gpMenuList = (MenuHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            gMenuCount = pRes->count;
            gpMenuDataBase = res_addr+sizeof(ResourceHeader_t) + sizeof(MenuHeader_t) * pRes->count;            
        }
        // load font data
        else if(pRes->ID == LIST_ID_FONT)
        {
            FontHeader_t *pFontHeader = (FontHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            data_addr = res_addr+sizeof(ResourceHeader_t) + sizeof(FontHeader_t) * pRes->count;

            gpDefaultFont = NULL;
            for(i=0; i<pRes->count; i++)
            {
                switch(pFontHeader[i].font_id)
                {
                case GUI_LANG_ENGLISH:
                    gpAsciiFont = (fontData_t*)(data_addr + pFontHeader[i].offset);
                    break;
                default:
                    if(pFontHeader[i].font_id == font_id)
                        gpDefaultFont = (fontData_t*)(data_addr + pFontHeader[i].offset);
                    break;
                }
            }
            if(gpDefaultFont == NULL)
                gpDefaultFont = gpAsciiFont;
        }
        else
#endif
        // load audio list
        if(pRes->ID == LIST_ID_AUDIO)
        {
            gpAudioList = (AudioHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            gAudioCount = pRes->count;
            gpAudioDataBase = res_addr+sizeof(ResourceHeader_t) + sizeof(AudioHeader_t) * pRes->count;            
        }
        // load led pattern data
        else if(pRes->ID == LIST_ID_PATTERN)
        {
            gpLedPatternList = (StringHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            gLedPatternCount = pRes->count;
            gpLedPatternDataBase = res_addr+sizeof(ResourceHeader_t) + sizeof(StringHeader_t) * pRes->count;            
        }

        // goto next resource
        res_addr += pRes->total_size;
        pRes = (ResourceHeader_t*)(res_addr);
    }
    g_current_language = lang;
}

INT8* gui_get_string(UINT16 StringId)
{
	INT16	nFirst	= 	0;
	INT16 nLast =  gStringCount -1;
	INT16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if(gpStringList[nMid].string_id == StringId)
		{
	        return gpStringDataBase + gpStringList[nMid].offset;
		}
		else if(gpStringList[nMid].string_id < StringId)
		{
		    nLast = nMid-1;
		}
		else
		{
		    nFirst = nMid +1;
		}
   }

   return NULL;
}

INT8* gui_get_image(UINT8 *width, UINT8 *height, UINT16 *size, UINT16 img_id)
{
	INT16 nFirst	= 	0;
	INT16 nLast =  gImageCount -1;
	INT16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;
		if(gpImageList[nMid].img_id == img_id)
		{
			*height = gpImageList[nMid].height;
			*width = gpImageList[nMid].width;
            *size = gpImageList[nMid].size;
			return gpImageDataBase + gpImageList[nMid].offset;
		}
		else if(gpImageList[nMid].img_id < img_id)
		{
		    nLast = nMid-1;
		}
		else
		{
		    nFirst = nMid +1;
		}
	}
	return NULL;
}

UINT8 *gui_get_audio(UINT8 audio_id, UINT8 *type, UINT32 *length)
{
	INT16 nFirst	= 	0;
	INT16 nLast =  gAudioCount -1;
	INT16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;
		if(gpAudioList[nMid].audio_id == audio_id)
		{
		    if(gpAudioList[nMid].lang == 0 || gpAudioList[nMid].lang == g_current_language)
            {      
    			*type = gpAudioList[nMid].type;
    			*length = gpAudioList[nMid].size;
    			return gpAudioDataBase + gpAudioList[nMid].offset;
            }
            else if(gpAudioList[nMid].lang < g_current_language)
            {
                nLast = nMid-1;
            }
            else
            {
                nFirst = nMid +1;
            }
		}
		else if(gpAudioList[nMid].audio_id < audio_id)
		{
		    nLast = nMid-1;
		}
		else
		{
		    nFirst = nMid +1;
		}
	}
	return NULL;
}

UINT8 *gui_get_led_pattern(UINT8 pattern_id, UINT32 *length)
{
	INT16 nFirst	= 	0;
	INT16 nLast =  gLedPatternCount -1;
	INT16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;
		if(gpLedPatternList[nMid].string_id == pattern_id)
		{
			*length = gpLedPatternList[nMid].length;
			return gpLedPatternDataBase + gpLedPatternList[nMid].offset;
		}
		else if(gpLedPatternList[nMid].string_id < pattern_id)
		{
		    nLast = nMid-1;
		}
		else
		{
		    nFirst = nMid +1;
		}
	}
	return NULL;
}

Menu_t *gui_get_menu(UINT32 menu_id, MenuItem_t **items)
{
	INT16 nFirst	= 	0;
	INT16 nLast =  gMenuCount -1;
	INT16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;
		if(gpMenuList[nMid].menu_id == menu_id)
		{
		    *items = (MenuItem_t*)(gpMenuDataBase + gpMenuList[nMid].offset);
			return (Menu_t*)(gpMenuList+nMid);
		}
		else if(gpMenuList[nMid].menu_id < menu_id)
		{
		    nLast = nMid-1;
		}
		else
		{
		    nFirst = nMid +1;
		}
	}
	return NULL;
}

BOOL gui_get_festival(UINT8 index, Festival_t *pFestival)
{
    if(index >= gFestivalCount || pFestival==NULL)
        return FALSE;

    pFestival->name = gpFestivalDataBase + gpFestivalList[index].offset;
    pFestival->style = (gpFestivalList[index].month)>>4;
    pFestival->month = (gpFestivalList[index].month) & 0x0f;
    pFestival->day= (gpFestivalList[index].day);
    if(pFestival->style == FESTIVAL_TYPE_WEEK)
    {
        pFestival->week = (gpFestivalList[index].day)>>4;
        pFestival->day= (gpFestivalList[index].day)&0x0f;
    }
    return TRUE;
}


INT32 SearchIndexinList(UINT32 nCh, UINT32 nRangeNum, const RangeData_t* pRangeData)
{
	INT16	nFirst	= 	0;
	INT16 nLast = nRangeNum-1;
	INT16 nMid;

    if(nCh >= pRangeData[nLast].nMin)
        return nLast;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if((nCh >=  pRangeData[nMid].nMin) && (nCh <  pRangeData[nMid+1].nMin))
		{
			return nMid;
		}
		if(nCh>pRangeData[nMid].nMin)
			nFirst = nMid +1;
		else
			nLast = nMid-1;

	}

	return -1;
}

UINT32 gui_get_font_data(UINT32 nCh, UINT8 font, UINT8 **ppCharData,UINT16* pnWidth, UINT16* pnHeight)
{
	INT32 NumChar=0;
	//UINT8   count;
	INT32   index	=	-1;
	UINT32	nIndexInList;
	//UINT32	nBase;
	//INT32	nFontIndex	=	-1;
    const RangeData_t*	pRangeData;

    switch(font)
    {
        case GET_FONT_DATA_TYPE_DEFAULT:
            gpCurrentFont = gpDefaultFont;
            break;
        case GET_FONT_DATA_TYPE_ASCII:
            gpCurrentFont = gpAsciiFont;
            break;
    }


	pRangeData	=	gpCurrentFont->rangeList;

	*pnWidth = gpCurrentFont->nWidth;
	*pnHeight = gpCurrentFont->nHeight;

	if(nCh>=gpCurrentFont->rangeList[0].nMin &&
		nCh<=gpCurrentFont->nMaxChar)
	{
		index = SearchIndexinList(nCh,gpCurrentFont->nNoOfRanges,pRangeData);
		if(index >= 0)
		{
            UINT8 *pBaseData = (UINT8*)gpCurrentFont;
			NumChar = gpCurrentFont->nCharBytes;
			nIndexInList = (NumChar*(nCh-pRangeData[index].nMin+pRangeData[index].nOffset));
			*ppCharData = (pBaseData + gpCurrentFont->pDataArray+nIndexInList);
            return NumChar;
		}
	}

    *ppCharData = NULL;
    return 0;
}

UINT32 gui_convert_unicode_data(UINT32 nCh, UINT8 font, UINT8 *buff)
{
    //INT i;
    RangeData_t *pRangeData;
    UINT8 *pBaseData;
    UINT8 *pData;
	INT32   index	=	-1;
    UINT32 len = 0;
    UINT32 offset;

    switch(font)
    {
        case GET_FONT_DATA_TYPE_DEFAULT:
            gpCurrentFont = gpDefaultFont;
            break;
        case GET_FONT_DATA_TYPE_ASCII:
            gpCurrentFont = gpAsciiFont;
            break;
    }

    if(gpDefaultFont == NULL && gpAsciiFont == NULL)
    {
        if(nCh > 0xff)
        {
            buff[0] = nCh&0xff;
            buff[1] = (nCh>>8)&0xff;
            return 2;
        }
        else
        {
            buff[0] = nCh;
            return 1;
        }
    }

    if(gpCurrentFont->nUniType == 0)
    {
        buff[0] = nCh&0xff;
        buff[1] = (nCh>>8)&0xff;
        return 2;
    }
    if(gpCurrentFont->nUniType == 1)
    {
        buff[0] = nCh;
        return 1;
    }
    
    pBaseData = (UINT8*)gpCurrentFont;
    pRangeData = (RangeData_t*)(pBaseData + gpCurrentFont->pUnicodeRanges);
    pData = pBaseData + gpCurrentFont->pUnicodeData;

    if(nCh >= pRangeData[0].nMin && nCh < gpCurrentFont->nMaxUnicode)
    {
        index = SearchIndexinList(nCh,gpCurrentFont->nNoOfUniRanges,(RangeData_t*)pRangeData);
        if(index >=0)
        {
            offset = pRangeData[index].nOffset + nCh - pRangeData[index].nMin;
            if(gpCurrentFont->nUniType == 2)
            {
                buff[0] = pData[offset*2];
                buff[1] = pData[offset*2+1];
                if(buff[0]!=0)
                {
                    len++;
                }
                if(buff[1]!=0)
                {
                    len++;
                }
            }
            else if(gpCurrentFont->nUniType == 3)
            {
                buff[0] = pData[offset];
                if(buff[0]!=0)
                    len ++;
            }
        }
    }
    return len;
}


UINT32 gui_unicode2local(UINT8 *buff, UINT32 length)
{
    INT i;
    UINT32 nCh;
    UINT32 result, len = 0;
    //UINT32 offset;
    
    for(i=0;i<length;i+=2)
    {
        nCh = buff[i]|(buff[i+1]<<8);
        if(nCh == 0)
            break;
        result = gui_convert_unicode_data(nCh, GET_FONT_DATA_TYPE_DEFAULT, buff+len);
        if(result == 0)
            result = gui_convert_unicode_data(nCh, GET_FONT_DATA_TYPE_ASCII, buff+len);

        if(result == 0)
            buff[len++] = 0x20;
        else
            len+= result;
    }
    buff[len] = 0;
    
    return len+1;
}


BOOL ReadConfigData(void *pbuf, UINT8 id, UINT32 len)             //len: 字节为单位
{
    INT32 i;
    UINT8 *res_addr = gpResourceBase;
    ResourceHeader_t *pRes = (ResourceHeader_t*)res_addr;

    if(res_addr == NULL)
        return FALSE;
    while(pRes->ID != 0)
    {
        // load string list
        if(pRes->ID == LIST_ID_SETTING)
        {
            ConfigHeader_t *pConfigList = (ConfigHeader_t*)(res_addr+sizeof(ResourceHeader_t));
            for(i=0; i<pRes->count; i++)
            {
                if(pConfigList[i].config_id == id)
                {
                    if(pConfigList[i].size < len)
                        len = pConfigList[i].size;
                    
                    memcpy(pbuf, res_addr+sizeof(ResourceHeader_t) + sizeof(ConfigHeader_t) * pRes->count + pConfigList[i].offset, len);
                    return TRUE;
                }
             }
        }
        // goto next resource
        res_addr += pRes->total_size;
        pRes = (ResourceHeader_t*)(res_addr);
    }
    return FALSE;
}

BOOL LoadFactorySetting(void *pbuf, UINT32 len)
{
    //INT32 i;
    UINT8 *res_addr = gpResourceBase;
    ResourceHeader_t *pRes = (ResourceHeader_t*)res_addr;

    if(res_addr == NULL)
        return FALSE;
    while(pRes->ID != 0)
    {
        // load string list
        if(pRes->ID == LIST_ID_FACTORY)
        {
            if(len<pRes->total_size-sizeof(ResourceHeader_t))
                len = pRes->total_size-sizeof(ResourceHeader_t);
            memcpy(pbuf, res_addr+sizeof(ResourceHeader_t), len);
            return TRUE;
        }
        // goto next resource
        res_addr += pRes->total_size;
        pRes = (ResourceHeader_t*)(res_addr);
    }
    return FALSE;
}

void *GetCalendarData(void)
{
    //INT32 i;
    UINT8 *res_addr = gpResourceBase;
    ResourceHeader_t *pRes = (ResourceHeader_t*)res_addr;

    if(res_addr == NULL)
        return FALSE;
    while(pRes->ID != 0)
    {
        if(pRes->ID == LIST_ID_CALENDAR)
        {
            return res_addr+sizeof(ResourceHeader_t);
        }
        // goto next resource
        res_addr += pRes->total_size;
        pRes = (ResourceHeader_t*)(res_addr);
    }
    return NULL;
}
