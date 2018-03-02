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


#include "ap_music.h"
#include "event.h"

#define ENCODING_LENGTH   2

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能

#define  AUDPLY_LYRIC_STR_BUFF_MAX_SIZE    (512)  /* pre-defined max lyric string buffer size : bytes */

#define  AUDPLY_LYRIC_MAX_SIZE    (128)  /* pre-defined max lyric string buffer size : bytes */
#define  AUDPLY_LYRIC_MAX_LINE_COUNT      16   /* pre-defined number of lyirc lines */

typedef enum
{
    NON_UCS2,
    UCS2_LITTLE_ENDIAN,
    UCS2_BIG_ENDIAN
} lyric_ucs2_encode_enum;

typedef enum
{
	CHSET_UCS2=8,
	CHSET_UTF8,
	CHSET_UNKNOW
}lyric_encode_type;

typedef enum
{
    TAG_NONE,
    TAG_START,
    TAG_MIN,
    TAG_SEC,
    TAG_MS,
    TAG_END,
}lyric_parse_status_enum;

typedef enum
{
    LOAD_STOP,
    LOAD_ALL,
    LOAD_NEXT,
    LOAD_PREV,
}lyric_load_emum;

typedef struct audply_lyric_line
{
    UINT32 time;
    UINT32 offset;
}lyric_line_struct;

typedef struct
{
    lyric_load_emum loading;       
    INT8        lrc_encoding_chset;
    INT8        lrc_encoding_ucs2;
    UINT8      lyric_count;
    UINT8      lyric_index;
    INT8      lyric_seprate_index;
    UINT32    file_offset;
    UINT32    time_base;
    lyric_line_struct lyric[AUDPLY_LYRIC_MAX_LINE_COUNT];
} lyric_struct_t;

static lyric_struct_t audply_lyric;

static INT32      g_lyric_file_handle = -1;

// buff for seprate long lyric
INT8 g_lyric_buff[AUDPLY_LYRIC_MAX_SIZE];
static INT32 lyricParse(UINT8 *buff, INT32 length);
static UINT8 *lyricGetLine(UINT8 *buff, UINT32 length);
INT32 mmi_chset_convert( UINT8 src_type, UINT8 dest_type, INT8 *src_buff,  INT8 *dest_buff, INT32 dest_size);

INT32 app_ucs2_strlen(const INT8 *arrOut)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    INT32 nCount = 0;
    INT32 nLength = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Check for NULL character only at the odd no. of bytes 
       assuming forst UINT8 start from zero */
    if (arrOut)
    {
        while (arrOut[nCount] != 0 || arrOut[nCount + 1] != 0)
        {
            ++nLength;
            nCount += 2;
        }
    }
    return nLength; /* One is added to count 0th UINT8 */
}

INT8 *app_ucs2_strcpy(INT8 *strDestination, const INT8 *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U16 count = 1;
    INT8 *temp = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strSource == NULL)
    {
        if (strDestination)
        {
            *(strDestination + count - 1) = '\0';
            *(strDestination + count) = '\0';
        }
        return temp;

    }

    if (strDestination == NULL || strSource == NULL)
    {
        return NULL;
    }
    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0)))
    {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return temp;
}

INT8 *app_ucs2_strcat(INT8 *strDestination, const INT8 *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    INT8 *dest = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest = dest + app_ucs2_strlen(strDestination) * ENCODING_LENGTH;

    app_ucs2_strcpy(dest, strSource);
    return strDestination;

}

INT8 *app_ucs2_strchr(const INT8 *string,  U16 ch)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT8 *chr = (INT8*) string;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    do
    {
        if ((*(chr+1)<<8|(INT8)(*chr)) == ch)
        {
            return chr;
        }
        chr += 2;
    } while (*chr || *(chr+1));

    return NULL;
}


/*****************************************************************************
 * FUNCTION
 *  lyricGetEncodingType
 * DESCRIPTION
 *  check if the lrc is an Unicode encoded file or the user specified encoded file.
 * PARAMETERS
 *  str     [IN]        Pointer to the text source.
 *  BL      [OUT]       A number to verify big or little indian
 * RETURNS
 *  a mmi_chset_enum that sprcifies UTF8/UCS2/User Specified  encoded type.
 *****************************************************************************/
static INT8 lyricGetEncodingType(const INT8 *str, lyric_ucs2_encode_enum *BL)
{
    if (str[0] == (INT8) 0xEF && str[1] == (INT8) 0xBB && str[2] == (INT8) 0xBF)
    {
        *BL = NON_UCS2;
        return CHSET_UTF8;
    }
    else if (str[0] == (INT8) 0xFE && str[1] == (INT8) 0xFF)
    {
        *BL = UCS2_BIG_ENDIAN;
        return CHSET_UCS2;
    }
    else if (str[0] == (INT8) 0xFF && str[1] == (INT8) 0xFE)
    {
        *BL = UCS2_LITTLE_ENDIAN;
        return CHSET_UCS2;
    }
    else/*unknow charset*/
    {
        *BL = NON_UCS2;
        return CHSET_UNKNOW;
    }
}


/*****************************************************************************
 * FUNCTION
 *  lyricUcs2Memcpy
 * DESCRIPTION
 *  If  the string buffer pool is not large enough, the string might ended in the middle of a
 *  lyrics line.which indicates that it is not ended in '\n' nor '\t'. the parser only takes the
 *  lyrics ended with '\n' or '\t'. this function try to take care of it.
 * PARAMETERS
 *  dest            [?]         
 *  src             [?]         
 *  total_size      [IN]        in bytes
 *  BL              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void lyricUcs2Memcpy(INT8 *dest, INT8 *src, UINT32 total_size, lyric_ucs2_encode_enum BL)
{
    if (BL == UCS2_LITTLE_ENDIAN)
    {
        memcpy(dest, src, total_size);
    }
    else if (BL == UCS2_BIG_ENDIAN)
    {
        UINT32 i;

        for (i = 0; i < total_size; i += 2)
        {
            *(dest + i)       = *(src + i + 1);
            *(dest + i + 1) = *(src + i);
        }
    }
}


/********************************************************************************
* Description : Load a lyric to lyric vram buffer.
*
* Arguments  : 
            strFile:  Dos 8.3 file name of music audio file.
            strID3:  ID3 information of the music audio file.
* Returns     :
            page mode of the lyric,  0 means fail.

* Notes       :  Only support LRC file now.
*
********************************************************************************/
BOOL  lyricLoad(const UINT32 file_entry)
{
    INT32 handle=-1;
	/*得到歌词文件的全文件名字，再打开*/
    INT8 *file_name_end=NULL,*temp_file_name_end=NULL;
  
    INT8  *temp_buf = COS_Malloc_BTOverlay(AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);


    if(g_lyric_file_handle >= 0)
    {
        FS_Close(g_lyric_file_handle);
        g_lyric_file_handle = -1;
    }
    memset(temp_buf,0,AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);
    
    FS_GetEntryName(file_entry, AUDPLY_LYRIC_STR_BUFF_MAX_SIZE, temp_buf);	

    temp_file_name_end = temp_buf;

    while( (temp_file_name_end = app_ucs2_strchr(temp_file_name_end, L'.')) != NULL )
    {
        file_name_end = temp_file_name_end;
        temp_file_name_end++;
    }
	
    if (file_name_end)
    {
        *file_name_end = 0;
    }

	/*Current Surporrt LRC Files*/
    app_ucs2_strcat(temp_buf, L".lrc");	

    hal_HstSendEvent(SYS_EVENT,0x09220031);
    hal_HstSendEvent(SYS_EVENT,temp_buf);
  
    if ((handle = FS_Open(temp_buf, FS_O_RDONLY,0)) >= 0)
    {
        INT32  file_size = 0;
        UINT32 read_size  = 0;

        lyric_ucs2_encode_enum BL = NON_UCS2;  /* when it is encoded in UCS2, use this variable to indicate Big or Little endian */

        if ((file_size=(UINT32)FS_GetFileSize(handle) )< 0)
        {
            COS_Free_BTOverlay(temp_buf);
            FS_Close(handle);
            return FALSE;
        }

        memset(&audply_lyric, 0, sizeof(lyric_struct_t));
        memset(temp_buf, 0, AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);

        /* to know what coding type it is */
        read_size=FS_Read(handle, (void *)temp_buf, 3);
	
        audply_lyric.lrc_encoding_chset = lyricGetEncodingType(temp_buf, &BL);
        audply_lyric.lrc_encoding_ucs2 = BL;
        audply_lyric.lyric_count = 0;
        g_lyric_file_handle = handle;

        COS_Free_BTOverlay(temp_buf);
        return TRUE;
    }
    else
    {
       COS_Free_BTOverlay(temp_buf);
      	return FALSE;
    }  
}

BOOL  lyricPreLoad(void)
{
    INT i, j;
    INT32 length;
    UINT8  *temp_buf;

    if(audply_lyric.loading == LOAD_STOP)
    {
        if(audply_lyric.lyric_index >= AUDPLY_LYRIC_MAX_LINE_COUNT/2 && audply_lyric.lyric_count == AUDPLY_LYRIC_MAX_LINE_COUNT)
        {
            app_trace(APP_MUSIC_TRC, "[lyricPreLoad]: read half of list(%d) [%d~%d], parse next part", audply_lyric.lyric_count, audply_lyric.lyric[0].time, audply_lyric.lyric[audply_lyric.lyric_count-1].time);
            for(i = audply_lyric.lyric_count -AUDPLY_LYRIC_MAX_LINE_COUNT/2, j=0; j < AUDPLY_LYRIC_MAX_LINE_COUNT/2; i++,j++)
            {
                audply_lyric.lyric[j] = audply_lyric.lyric[i];
            }
            audply_lyric.lyric_count -= AUDPLY_LYRIC_MAX_LINE_COUNT/2;
            audply_lyric.lyric_index -= AUDPLY_LYRIC_MAX_LINE_COUNT/2;
            audply_lyric.loading = LOAD_NEXT;
            audply_lyric.time_base = audply_lyric.lyric[AUDPLY_LYRIC_MAX_LINE_COUNT/2-1].time;
            audply_lyric.file_offset = 0;
            return TRUE;
        }
        return FALSE;
    }
    
    FS_Seek(g_lyric_file_handle, audply_lyric.file_offset, FS_SEEK_SET);

    temp_buf =  COS_Malloc_BTOverlay(AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);
    
    length = FS_Read(g_lyric_file_handle, temp_buf, AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);
    if(length > 0)
    {
        length = lyricParse(temp_buf, length);
        audply_lyric.file_offset += length;
    }

    if(length <= 0)
    {
        if(audply_lyric.loading != LOAD_ALL)
            audply_lyric.time_base = audply_lyric.lyric[0].time;
        app_trace(APP_MUSIC_TRC, "[lyricPreLoad]: parse end, list(%d) [%d~%d]", audply_lyric.lyric_count, audply_lyric.lyric[0].time, audply_lyric.lyric[audply_lyric.lyric_count-1].time);
        audply_lyric.loading = LOAD_STOP;
    }
    COS_Free_BTOverlay(temp_buf);
    
    return TRUE;
}



/********************************************************************************
* Description : check if the requst time is in current lyric list
*
* Arguments  : 
			lTime:	the time to queryed
* Returns     :
			>=0: index in the list
			-1: out of range
* Notes       :  the function would be called frequent, should fast.
*
********************************************************************************/
INT32  lyricCheckRange(UINT32 lyrcTime)
{
    INT32 index = audply_lyric.lyric_index;
    if(audply_lyric.lyric_count == 0)
        return -1;
    if(lyrcTime < audply_lyric.time_base && lyrcTime < audply_lyric.lyric[0].time)
        return -1;
    if(lyrcTime >= audply_lyric.lyric[index].time && lyrcTime < audply_lyric.lyric[index+1].time)
        return index;
    
    for(index=0; index< audply_lyric.lyric_count; index++)
    {
        if(audply_lyric.lyric[index].time > lyrcTime)
        {
            if(index > 0)
                return index - 1;
            else
                return 0;
            break;
        }
    }
    
    if(audply_lyric.lyric_count < AUDPLY_LYRIC_MAX_LINE_COUNT)
        return audply_lyric.lyric_count-1;

    return -1;
}


/********************************************************************************
* Description : Get the next lyric page 
*
* Arguments  : 
			strLyric:	lyric page string
			len:		the max length of lyric page
* Returns     :
			TRUE:   Successful.
			FALSE:  Fail,  next lyric page don't exist
* Notes       :  exist a problem that don't differ Flash R/W Error with exceed the tail.
*
********************************************************************************/
BOOL  lyricGetNext( INT8 *strLyric, UINT8 len, UINT32 *nextTime)
{
    INT i, length;
    INT8  *temp_buf = NULL, *lyric;
    INT parse_file = FALSE;
    INT32 index, count;

    if (len ==0)
        return FALSE;

    if(audply_lyric.loading == LOAD_ALL || (audply_lyric.loading == LOAD_NEXT && audply_lyric.lyric_index>=AUDPLY_LYRIC_STR_BUFF_MAX_SIZE/2))
    {
        strLyric[0] = 0;
        *nextTime += 500; // try to get lyric a half second later
        app_trace(APP_MUSIC_TRC, "[lyricGetNext]: parsing file, try later");
        return FALSE;
    }

    index = lyricCheckRange(*nextTime);
    if(index < 0)
    {
        app_trace(APP_MUSIC_TRC, "[lyricGetNext]: time %d out of current list(%d) [%d~%d]", *nextTime, audply_lyric.lyric_count, audply_lyric.lyric[0].time, audply_lyric.lyric[audply_lyric.lyric_count-1].time);
        audply_lyric.lyric[0].time = 0;
        audply_lyric.lyric[0].offset = 0;
        audply_lyric.loading = LOAD_ALL;
        audply_lyric.file_offset = 0;
        audply_lyric.lyric_count = 1;
        audply_lyric.time_base = *nextTime;
        strLyric[0] = 0;
        *nextTime += 500; // try to get lyric a half second later
        return FALSE;
    }        

    if(audply_lyric.lyric[index].offset == 0) // invalid lyric
    {
        strLyric[0] = 0;
        audply_lyric.lyric_index = index+1;
        *nextTime = audply_lyric.lyric[index+1].time;
        return TRUE;
    }
    
    if(audply_lyric.lyric_seprate_index == index) // seprate lyric
    {
        lyric = g_lyric_buff;
        length = strlen(g_lyric_buff);
    }
    else
    {
        // read the lyric
        FS_Seek(g_lyric_file_handle, audply_lyric.lyric[index].offset, FS_SEEK_SET);

        temp_buf = COS_Malloc_BTOverlay(AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);
        
        length = FS_Read(g_lyric_file_handle, temp_buf, AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);

        lyric = lyricGetLine(temp_buf, length);

        if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
        {
            length = app_ucs2_strlen(lyric)*ENCODING_LENGTH;
            gui_unicode2local(lyric, length);
        }
        else
            length = strlen(lyric);
    }

    app_trace(APP_MUSIC_TRC, "[lyricGetNext]: get lyric index %d,cur time %d, lyric time %d, len %d", index, *nextTime, audply_lyric.lyric[index].time, length);

    if(length > len) // too long for the buff, seprate the lyric
    {
        if(length > AUDPLY_LYRIC_MAX_SIZE+len-2)
            length = AUDPLY_LYRIC_MAX_SIZE + len-2;
        count = length/len + 1;
        len = length/count;
        for(i=0;i<len;)
        {
            if(lyric[i]&0x80)
                i+= 2;
            else
                i+=1;
        }
        len=i;
        strncpy(strLyric, lyric, len);
        strLyric[len] = 0;
        strncpy(g_lyric_buff, lyric+len, AUDPLY_LYRIC_MAX_SIZE);
        if(index<audply_lyric.lyric_count-1)
            i = audply_lyric.lyric[index+1].time-audply_lyric.lyric[index].time;
        else
            i = 3000*count;
        audply_lyric.lyric_seprate_index = index;
        *nextTime = audply_lyric.lyric[index].time = audply_lyric.lyric[index].time + i/count;
        audply_lyric.lyric_index = index;
        app_trace(APP_MUSIC_TRC, "[lyricGetNext]: lyric is too long, seprate count %d, next time %d", count, *nextTime);
    }
    else
    {
        strcpy(strLyric, lyric);
        audply_lyric.lyric_seprate_index = -1;
        audply_lyric.lyric_index = index+1;
        if(audply_lyric.lyric_index >= audply_lyric.lyric_count)
            *nextTime += 50000;// no more lyric
        else
            *nextTime = audply_lyric.lyric[audply_lyric.lyric_index].time;
    }
    if(temp_buf != NULL)
    {
        COS_Free_BTOverlay(temp_buf);
    }
    
    return TRUE;
}

/********************************************************************************
*   lyricFree
********************************************************************************/
BOOL  lyricFree(void)
{
    memset(&audply_lyric,0,sizeof(audply_lyric));
    return TRUE;
}

/********************************************************************************
* Description : Add one time tag to the list
*
* Arguments  : 
			offset:	offset in lyric file
			time:	the time to display the lyric
* Returns     :
			TRUE:   Successful.
			FALSE:  Fail
* Notes       :  
*
********************************************************************************/
BOOL  lyricAddTag(INT32 offset, UINT32 time)
{
    INT32 start, pos, index;

    switch(audply_lyric.loading)
    {
        case LOAD_STOP:
            return TRUE;
            break;

        case LOAD_ALL:
            start = 1;
            break;

         case LOAD_NEXT:
            start = AUDPLY_LYRIC_MAX_LINE_COUNT/2;
            break;

        default:
            break;
    }

    if(audply_lyric.lrc_encoding_chset == CHSET_UCS2 && audply_lyric.lrc_encoding_ucs2 == UCS2_LITTLE_ENDIAN)
        offset++;

    if(time <= audply_lyric.time_base)
    {
        if(audply_lyric.loading != LOAD_ALL)
            return TRUE;

        if(audply_lyric.lyric[0].time <= time)
        {
            audply_lyric.lyric[0].time = time;
            audply_lyric.lyric[0].offset = audply_lyric.file_offset + offset;
        }
        return TRUE;
        
    }

    index = audply_lyric.lyric_count;
    for(pos = start; pos<audply_lyric.lyric_count; pos++)
    {
        if(audply_lyric.lyric[pos].time > time)
        {
            index = pos;
            break;
        }
    }

    if(index < AUDPLY_LYRIC_MAX_LINE_COUNT)
    {
        for(pos=audply_lyric.lyric_count-1; pos>=index; pos--)
        {
            audply_lyric.lyric[pos+1] = audply_lyric.lyric[pos];
        }

        audply_lyric.lyric[index].offset = audply_lyric.file_offset + offset;
        audply_lyric.lyric[index].time = time;
        audply_lyric.lyric_count ++;
        if(audply_lyric.lyric_count > AUDPLY_LYRIC_MAX_LINE_COUNT)
            audply_lyric.lyric_count = AUDPLY_LYRIC_MAX_LINE_COUNT;
    }
    
    return TRUE;
}

 /*****************************************************************************
 * FUNCTION
 *  lyricParse
 * DESCRIPTION
 *  parse current lyric string
 * PARAMETERS
 *  cur_char        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static INT32 lyricParse(UINT8 *buff, INT32 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    lyric_parse_status_enum status = TAG_NONE;
    INT32 time_phase1, time_phase2, time_phase3;
    UINT32 temp_time, ms_index;
    INT32  i = 0, chr = 0;

    INT32 end_offset;

    if(audply_lyric.lrc_encoding_chset == CHSET_UCS2 && audply_lyric.lrc_encoding_ucs2 == UCS2_BIG_ENDIAN)
    {
        i = 1;
    }
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (; i<length; i++)
    {
        if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
        {
            if(audply_lyric.lrc_encoding_ucs2 == UCS2_BIG_ENDIAN)
                chr = buff[i-1];
            else
                chr = buff[i+1];
            if(chr != 0)
            {
                i++;        
                continue;
            }
        }
        
        switch (status)
        {
            case TAG_NONE:
                if(buff[i] == '[')
                {
                    status = TAG_START;
                    time_phase1 = -1;
                }
                break;
            case TAG_START:
                if(buff[i]>='0' && buff[i]<='9')
                {
                    if(time_phase1 < 0)
                        time_phase1 = 0;
                    else
                        time_phase1 *= 10;
                    time_phase1 += buff[i]-'0';
                }
                if(buff[i] == ':')
                {
                    buff[i] = 0;
                    if(time_phase1>=0)
                    {
                        time_phase2 = -1;
                        status = TAG_MIN;
                    }
                    else
                    {
                        status = TAG_NONE;
                    }
                }
                break;

            case TAG_MIN:
                if(buff[i]>='0' && buff[i]<='9')
                {
                    if(time_phase2 < 0)
                        time_phase2 = 0;
                    else
                        time_phase2 *= 10;
                    time_phase2 += buff[i]-'0';
                }
                if(buff[i] == '.')
                {
                    if(time_phase2>=0)
                    {
                        time_phase3 = 0;
                        ms_index = 100;
                        status = TAG_SEC;
                    }
                    else
                    {
                        status = TAG_NONE;
                    }
                }
                
                if(buff[i] == ']')
                {
                    if(time_phase2>=0)
                    {
                        temp_time = (time_phase1*60+time_phase2)*1000;
                        //app_trace(APP_MUSIC_TRC, "[lyricParse]: find time tag [%02d:%02d.%03d]", time_phase1, time_phase2, 0);
                        lyricAddTag(i+1, temp_time);
                    }
                    end_offset = i;
                    status = TAG_NONE;
                }
            case TAG_SEC:
                if(buff[i]>='0' && buff[i]<='9')
                {
                    time_phase3 += (buff[i]-'0')*ms_index;
                    ms_index/=10;
                }
                if(buff[i] == ']')
                {
                    temp_time = (time_phase1*60+time_phase2)*1000 + time_phase3;
                    //app_trace(APP_MUSIC_TRC, "[lyricParse]: find time tag [%02d:%02d.%03d]", time_phase1, time_phase2, time_phase3);
                    lyricAddTag(i+1, temp_time);
                    end_offset = i;
                    status = TAG_NONE;
                }                
                break;
            default:
                break;
        }   /* End switch */
        if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
            i++;        
    }   /* End fow */
    if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
    {
        if(audply_lyric.lrc_encoding_ucs2 == UCS2_BIG_ENDIAN)
            end_offset++;
        else
            end_offset+=2;
    }
    return end_offset;
}


UINT8 *lyricGetLine(UINT8 *buff, UINT32 length)
{
    UINT8 tmp, chr = 0;
    INT32 start = 0;
    UINT32 i;

    for(i=0; i<length; i++)
    {
        if(audply_lyric.lrc_encoding_ucs2 == UCS2_BIG_ENDIAN && (i&1)==0) // switch order
        {
            tmp = buff[i];
            buff[i] = buff[i+1];
            buff[i+1] = tmp;
        }
        if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
            chr = buff[i+1];

        if(chr == 0 && buff[i] == ']')
        {
            start = i+1;
            if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
                start ++;
        }
        if(chr == 0 && (buff[i] == '\r' || buff[i] == '\n'))
        {
            buff[i] = 0;
            break;
        }
        if(audply_lyric.lrc_encoding_chset == CHSET_UCS2)
            i++;
    }
    return buff+start;
}
 
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_convert
 * DESCRIPTION
 *  Convert string between 2 character sets. (will add the terminate character)
 * PARAMETERS
 *  src_type        [IN]        Charset type of source
 *  dest_type       [IN]        Charset type of destination
 *  src_buff        [IN]        Buffer stores source string
 *  dest_buff       [OUT]       Buffer stores destination string
 *  dest_size       [IN]        Size of destination buffer (bytes)
 * RETURNS
 *  Length of destination string, including null terminator. (bytes)
 *****************************************************************************/
INT32 mmi_chset_convert( UINT8 src_type, UINT8 dest_type, INT8 *src_buff,  INT8 *dest_buff, INT32 dest_size)
{
    INT32 count = 0;
    for (count = 0; src_buff[count] != 0 || src_buff[count + 1] != 0; count = count + 2)
    {
        if (count >= dest_size - 2)
        {
            count = (UINT16) dest_size - 2;
            break;
        }
        dest_buff[count] = src_buff[count];
        dest_buff[count + 1] = src_buff[count + 1];
    }
    dest_buff[count] = 0;
    dest_buff[++count] = 0;
    return count + 1;
}

#endif  /*_LRC_SUPPORT_支持歌词显示功能*/


