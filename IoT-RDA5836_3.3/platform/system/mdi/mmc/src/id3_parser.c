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

#include "cs_types.h"
#include "string.h"
#include "mmc.h"
#include "mci.h"
#include "fs.h"
#include "id3_parser.h"
#include "mcip_debug.h"
#include "assert.h"

/**********************************************************
 1) "RTENC"         :编码者      2) "TRCK"        :音轨
 3) "TPE1"          :演唱者      4) "TIT2"        :标题
 5) "TALB"          :专辑        6) "TYER"        :年代
 7) "TMED"          :媒体类型    8) "TCOM"        :作曲家
 9) "COMM"          :注释       10) "CON"         :流派
11) "TOPE"          :原创者     12) "TCOP"        :版权
13) "WXXX"          :URL 
**********************************************************/


// id3 tag v2 frame id -- title, artist, album, genre, year
const INT8 *const id3_tag_v2_2_frame_id_list[NUM_OF_ID3_FRAME] = { "TT2",  "TP1",   "TAL", "TCO", "TYE", "WCP" };
const INT8 *const id3_tag_v2_3_frame_id_list[NUM_OF_ID3_FRAME] = { "TIT2", "TPE1", "TALB", "TCON", "TYER", "TCOP" };
const INT8 *const id3_tag_v2_4_frame_id_list[NUM_OF_ID3_FRAME] = { "TIT2", "TPE1", "TALB", "TCON", "TDRC", "TCOP" };


#if 0    
/*********************************************************************************
*             SearchString
*
* Description :在一个BIN中找出一个字符数组的位置
*
* Arguments   :
*
* Returns     :返回子串地址，找不到，要出了，返回空
*
* Notes       :
*
*********************************************************************************/
INT32 SearchString(const UINT8 *buf,const INT8 *str,const INT32 buflen,const INT32 strlen)
{
    INT32 i,j=0;

    for(i=0;i<buflen;i++)
    {
        if(buf[i]!=str[j])
            j=0;
        else
            if(j++>=strlen)  return (INT32)(i);
    }
    return 0;
}


//-------[处理定符串尾的不完整汉字问题]-------
//
//---------------------------------------------
void DealStringEnd( INT8 *str, INT32 string_id)
{
    UINT8 i;
    BOOL half;

    if ( (string_id != GUI_LANG_SM_CHINESE) && (string_id != GUI_LANG_TR_CHINESE) )
    {
        return;
    }

    //如是空串不用处理
    if ( str[0] == 0) 
    {
        return;
    }        

    //
    for (i=0; i<255; i++)
    {
        if(str[i] >= 0x80)
        {
            half = TRUE; 
        }
        else
        {
            half = FALSE; 
        }
    
        //是否将结束
        if(str[i+1] == 0)
        {
            if(half)
            {
                str[i] = ' ';
            }
            return;            
        }

        if (half)
        {
            i++;                            
            if ( str[i+1] == 0 )
            {
                //str[i] = ' ';            
                return;
            }

            half = FALSE;
        }
    }//end for

}
//---------------------------------------

//---------------------------------------
//-------[处理字符串后的空格的问题]-------
//
//----------------------------------------

void DealStrEndSpace(INT8 *str)
{
	UINT8 i,j;
	
	for(i=0,j=0; i<255; i++)
	{
		if( str[i] == 0 )
		{
			break;
		}
		if( str[i] != ' ' )
		{
			j = i;
		}
		
	}	
    if(j == 0)
    {
        str[j] = 0;
    }
    else   
    {     
	    str[j+1] = 0;
    }
}



/*********************************************************************************
*             mp3GetTiltle
*
* Description :
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
BOOL mp3GetID3Info(const UINT32 fileEntry, ID3Info_t *ID3Info, INT32 string_id)
{
    //定义空标志
    BOOL kongFlag = FALSE;	

    //下面处理得出的 信息的一些特殊情况
    if (ID3Info->TIT2_length != 0)
    {
        DealStringEnd( ID3Info->TIT2_buffer, string_id);
        DealStrEndSpace( ID3Info->TIT2_buffer );
        if (ID3Info->TIT2_buffer[0] != 0)
        {
            kongFlag = TRUE;
        }
    }    

    if (ID3Info->TPE1_length != 0)
    {
        DealStringEnd( ID3Info->TPE1_buffer, string_id);      
        DealStrEndSpace( ID3Info->TPE1_buffer );      
        if (ID3Info->TPE1_buffer[0] != 0)
        {
            kongFlag = TRUE;            
        }
    }

    if (ID3Info->TALB_length != 0)
    {        
        DealStringEnd( ID3Info->TALB_buffer, string_id);   
        DealStrEndSpace( ID3Info->TALB_buffer );   
        if (ID3Info->TALB_buffer[0] != 0)
        {
            kongFlag = TRUE;            
        }
    }

    if (ID3Info->TENC_length != 0)
    {        
        DealStringEnd( ID3Info->TENC_buffer, string_id);
        DealStrEndSpace( ID3Info->TENC_buffer );
        if (ID3Info->TENC_buffer[0] != 0)
        {
            kongFlag = TRUE;            
        }
    }

    return kongFlag;
    
}

#endif

/*****************************************************************************
* FUNCTION
*     audio_file_input_stream_alloc
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     HANDLE file
* RETURNS
*     success
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 audio_file_input_stream_alloc( INT32 file, audio_input_stream_struct* input_stream )
{
	if(input_stream)
	{
		memset(input_stream,0,sizeof(audio_input_stream_struct));        
	    input_stream->buf = (UINT8*)pcmbuf_overlay;
		input_stream->ptr = input_stream->tail = input_stream->buf;
		input_stream->buf_size     = MCI_PCM_BUF_SIZE*4;
		input_stream->total_load   = 0;
		input_stream->file         = file;
		return MCI_ERR_NO;
	}
	else
	{
		return MCI_ERR_INVALID_PARAMETER;
	}
}


/*****************************************************************************
* FUNCTION
*     audio_input_stream_seek
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct *input_stream
*     INT32 offset
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void audio_input_stream_seek( audio_input_stream_struct* input_stream, UINT32 offset)
{
	input_stream->ptr = input_stream->tail = input_stream->buf;
	FS_Seek(input_stream->file,offset,FS_SEEK_SET);
	input_stream->total_load = offset;
}

/*****************************************************************************
* FUNCTION
*     audio_input_stream_get_data_size
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct *input_stream
* RETURNS
*     INT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 audio_input_stream_get_data_size( audio_input_stream_struct* input_stream )
{
	 return	 FS_GetFileSize(input_stream->file);
}

/*****************************************************************************
* FUNCTION
*     audio_input_stream_read
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct *input_stream
*     INT32 size : 0 means read as many as possible
* RETURNS
*     INT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 audio_input_stream_read( audio_input_stream_struct *input_stream, UINT32 size )
{
	INT32 prev_data_size, free_size, read_data_size, i;
	UINT8 *ptr, *src;
	
	ptr = input_stream->buf;
	// init free data size
	free_size = (INT32)input_stream->buf_size;
	
	if( size > 0 )
	{
		assert( size <= free_size );
		free_size = size;
	}
	else
	{
		// check if it's full, we must move ptr forward to free space before call stream load
		if( (prev_data_size = input_stream->tail - input_stream->ptr) == free_size )
        {
			return 0;
        }

		if( prev_data_size > 0 )
		{
			// we move data by ourself instead using memcpy because src block maybe overlap with dst block
			// and we don't know how memcpy actually copy data
			// if we always start copying data from src front side, it should be ok
			src = input_stream->ptr;
			for( i = 0; i < prev_data_size; i++ )
            {
                *ptr++ = src[i];
            }
			free_size -= prev_data_size;
		}
	}
	
    read_data_size  = FS_Read(input_stream->file, ptr, free_size); /* read data from file */
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"audio_input_stream_read,read_data_size:%d,file_handle:0x%x,free_size:%d",read_data_size,input_stream->file,free_size);
	// update ptr, tail, and total_load
	input_stream->ptr  = input_stream->buf;
	input_stream->tail = ptr + read_data_size;
	input_stream->total_load += read_data_size;
	
	return read_data_size;
}


/*****************************************************************************
* FUNCTION
*     id3_tag_utf16_to_ucs2
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8 byte_order
*     UINT8 *dest
*     UINT32 dest_size (>=4)
*     UINT8* src
*     UINT32 src_size (>=1)
* RETURNS
*     INT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
INT32 id3_tag_utf16_to_ucs2( UINT8 byte_order, UINT8 *dest, UINT32 dest_size, UINT8* src, UINT32 src_size )
{
	UINT16 utf16;
	UINT32 dest_count = 0;
	UINT32 src_count = 0; // src_size is used to prevent from infinte loop when src data are not utf16
	
	if( byte_order == ID3_UTF16_UNKNOWN_BYTE_ORDER )
	{
		// check BOM
		if( src[0] == 0xFF && src[1] == 0xFE )
        {
			byte_order = ID3_UTF16_LE;
        }
		else if( src[0] == 0xFE && src[1] == 0xFF )
        {
			byte_order = ID3_UTF16_BE;
        }
		src += 2;
		src_count += 2;
	}
	
	if( byte_order != ID3_UTF16_UNKNOWN_BYTE_ORDER )
	{
		dest_size -= ((dest_size % 2) + 2); // make it a even number + null terminator
		// ID3_UTF16_BE = 0, ID3_UTF16_LE = 1
		while( dest_count < dest_size && src_count < src_size &&(utf16 = ((UINT16)src[byte_order] << 8) | ((UINT16)src[1-byte_order])) )
		{
			if( utf16 < 0xD800 || utf16 > 0xDFFF )
			{
				dest[dest_count]   = (UINT8)(utf16 & 0xFF);
				dest[dest_count+1] = (UINT8)(utf16 >> 8);
				dest_count += 2;
			}
			src += 2;
			src_count += 2;
		}
	}
	dest[dest_count] = 0;
	dest[dest_count + 1] = 0;
	return dest_count + 2;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse_number
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     WCHAR *text: UCS2 and its format is *((0-9)+)*, i.e. (12) or (123), ....etc.
*     UINT32 text_size
* RETURNS
*     INT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
INT32 id3_tag_parse_number( WCHAR *text, UINT32 text_size )
{
	INT32 val;
    for (val = -1; text_size != 0; text_size--, text++)
	{
        if (*text >= L'0' && *text <= L'9')
		{
			if( val < 0 ) // check if it's the first time
            {
				val = 0;
            }
			val = val * 10 + (INT32)(*text - L'0');
		}
        else if (*text == L'(' || *text == L')')
		{
			if( val >= 0 )
            {
				break; // finish
            }
        }
    }
	return val;	
}

/*****************************************************************************
* FUNCTION
*     id3_tag_get_unsync_uint32
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     UINT8 size
* RETURNS
*     UINT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 id3_tag_get_unsync_uint32( UINT8* data, UINT8 size )
{
	UINT32 num = 0;
	if( size > 4 ) // size should never exceed 5
	{
		num = *data++;
		size--;
	}
	for( ; size > 0; size-- )
    {
		num = (num << 7) | (*data++);
    }
	return num;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_get_uint32
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     UINT8 size
* RETURNS
*     UINT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 id3_tag_get_uint32( UINT8* data, UINT8 size )
{
	UINT32 num;
	for( num = 0; size > 0; size-- ) // size should never exceed 4
    {
		num = (num << 8) | (*data++);
    }
	return num;
}


/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v2_tag_header
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     id3_tag_struct* id3_tag_para
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL id3_tag_parse_v2_tag_header( UINT8* data, id3_tag_struct*id3_tag)
{
	UINT8 flags, version;
  
	// check id3v2 magic Word -- "ID3"
	if( !(data[0] == 'I' && data[1] == 'D' && data[2] == '3') )
    {
		return FALSE;
    }
	version = data[3];
	if( data[4] != 0 )
    {
		return FALSE;
    }
	flags = data[5];
	
	// init flags
	id3_tag->v2_major_ver = version;
	id3_tag->v2_unsync     = FALSE;
	id3_tag->v2_exthdr      = FALSE;
	id3_tag->v2_footer       = FALSE;

	switch( version )
	{
        case 4:
            if (flags & 0x0F)
            {
			   return FALSE;
            }
            if (flags & 0x10)   /* check footer flag */
            {
                id3_tag->v2_footer = TRUE;
            }
        case 3:
            if (flags & 0x1F)
            {
                return FALSE;
            }
            if (flags & 0x20)   /* check extended header flag */
            {
                id3_tag->v2_exthdr = TRUE;
            }
            break;
        case 2:
            if (flags & 0x3F)
            {
                return FALSE;
            }
            if (flags & 0x40)   /* check compression flag, if it is set, ignore this tag */
            {
                return FALSE;
            }
            break;
        default:
            return FALSE;
    }
  
    if (flags & 0x80)   /* check unsynchronisation flag */
    {
        id3_tag->v2_unsync = TRUE;
    }

	// get tag size
	id3_tag->v2_size = id3_tag_get_unsync_uint32( data + 6, 4 ); // 6, 7, 8, 9
 
  	MCI_TRACE (MCI_AUDIO_TRC,0,"[v2_tag_header]v2_size:%d,version:%d,flags: %d,data[0-2]:%c,%c,%c",id3_tag->v2_size,version,flags,data[0],data[1],data[2]);

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_reverse_unsync_process
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     UINT32 size
* RETURNS
*     UINT32
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT32 id3_tag_reverse_unsync_process( UINT8* data, UINT32 size )
{
	UINT8 *output = data + 1;
	UINT8 *ptr = data;
	UINT8 prev_value;
	
	if( size == 0 ) return 0;
	for( prev_value = *ptr++, size--; size != 0; size-- )
	{
		if( !(prev_value == 0xFF && *ptr == 0x00) )
        {
            *output++ = *ptr;
        }
		prev_value = *ptr++;
	}

	return (UINT32)output - (UINT32)data;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v2_tag_extended_header
* DESCRIPTION
*     
* IMPACT 
*     we assume id3 tag read buffer >= 14 bytes
* PARAMETERS
*     UINT8* data
*     id3_tag_struct* id3_tag_para
*     BOOL data_full_load;
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL id3_tag_parse_v2_tag_extended_header( UINT8* data, id3_tag_struct*id3_tag, BOOL data_full_load )
{
	UINT32 extended_header_size;
	UINT32 tag_size;
	UINT32 padding_size;
	UINT32 crc_data_size;
	UINT32 crc_data;
	UINT8  *crc_data_start;
	UINT8  flags;
	BOOL   crc_data_present;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (id3_tag->v2_unsync)
    {
        tag_size = id3_tag->v2_reverse_unsync_size;
    }
    else
    {
        tag_size = id3_tag->v2_size;
    }
    switch (id3_tag->v2_major_ver)
    {
        case 4:
            extended_header_size = id3_tag_get_unsync_uint32(data, 4);  /* 0, 1, 2, 3 */
            /* check size */
            if (extended_header_size >= tag_size)
            {
                return FALSE;
            }
            if (data[4] != 0x01)    /* flag bytes must be 1 */
            {
                return FALSE;
            }
            flags = data[5];
            if (flags & 0x8F)   /* check unset flags */
            {
                return FALSE;
            }
            if (flags & 0x20)   /* check CRC data present flag */
            {
                crc_data_present = TRUE;
                if (data[6] != 0x05)    /* flag data length must be 5 */
                {
                    return FALSE;
                }
                crc_data = id3_tag_get_unsync_uint32(data + 7, 5);      /* 7, 8, 9, 10, 11 */
                crc_data_start = data + 12;
                crc_data_size = tag_size - extended_header_size;
            }
            else
            {
                crc_data_present = FALSE;
            }
            break;
        case 3:
            extended_header_size = id3_tag_get_uint32(data, 4); /* 0, 1, 2, 3 */
            if (data[5] != 0)   /* 2nd flag must be all unset */
            {
                return FALSE;
            }
            padding_size = id3_tag_get_uint32(data + 6, 4); /* 6, 7, 8, 9 */
            /* check size */
            if (extended_header_size + padding_size >= tag_size)
            {
                return FALSE;
            }
            flags = data[4];
            if (flags & 0x80)   /* check CRC data present flag */
            {
                crc_data_present = TRUE;
                crc_data          = id3_tag_get_uint32(data + 10, 4);    /* 10, 11, 12, 13 */
                crc_data_start = data + 14;
                crc_data_size = tag_size - extended_header_size - padding_size;
            }
            else
            {
                crc_data_present = FALSE;
            }
            break;
        default:
            return FALSE;
    }
	
	if( crc_data_present && data_full_load ) // if tag is larger than our buffer size, we skip CRC check process
	{
		//if( med_crc_calculate( (UINT8 const*)crc_data_start, crc_data_size ) != crc_data )
        {
		//	return FALSE;
        }
    }

    /* get extended header size */
    id3_tag->v2_exthdr_size = extended_header_size;
    id3_tag->v2_crc         = crc_data_present;

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v2_frame_header
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     INT32 size
*     id3_tag_frame_header_struct* frame_header
*     id3_tag_struct* id3_tag_para
* RETURNS
*     UINT8: PARSE_ID3_FRAME_NO_SPACE (-1) -> no space
*               PARSE_ID3_FRAME_SUCCESS (0) -> success
*               PARSE_ID3_FRAME_ABORT (1) -> abort
* GLOBALS AFFECTED
*     
*****************************************************************************/
UINT8 id3_tag_parse_v2_frame_header( UINT8* data, INT32 size, id3_tag_frame_header_struct* frame_header, id3_tag_struct* id3_tag)
{
	UINT8   flags;
	UINT32 data_offset = 0;
	
	switch( id3_tag->v2_major_ver )
    {
        case 4:
            if (size < 10)
            {
                return PARSE_ID3_FRAME_NO_SPACE;
            }
            memcpy(frame_header->id, data, 4);  /* 0, 1, 2, 3 */
            frame_header->size = id3_tag_get_unsync_uint32(data + 4, 4);        /* 4, 5, 6, 7 */
            frame_header->size += 10;
            data_offset = 10;

            /* get status flags */
            flags = data[8];
            if (flags & 0x80)   /* check unset flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }
            if (flags & 0x60)   /* check file and tag alter perservation flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            /* get format flags */
            flags = data[9];

            if (flags & 0xB0)   /* check unset flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            if (flags & 0x0C)   /* check compress and encrypt flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            if (flags & 0x40)   /* check group id flag */
            {
                data_offset++;
            }

            if (flags & 0x20)   /* check unsyncrhonisation flag */
            {
                frame_header->unsync = TRUE;
            }
            else
            {
                frame_header->unsync = FALSE;
            }

            if (flags & 0x01)   /* check data length indicator flag */
            {
                id3_tag_get_unsync_uint32(data + 10, 4);    /* 10, 11, 12, 13 */
                data_offset += 4;
            }
            break;

        case 3:
            if (size < 10)
            {
                return PARSE_ID3_FRAME_NO_SPACE;
            }
            memcpy(frame_header->id, data, 4);  /* 0, 1, 2, 3 */
            frame_header->size = id3_tag_get_uint32(data + 4, 4);       /* 4, 5, 6, 7 */
            frame_header->size += 10;
            data_offset = 10;

            /* get status flags */
            flags = data[8];

            if (flags & 0x1F)   /* check unset flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            if (flags & 0xC0)   /* check file and tag alter perservation flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            /* get format flags */
            flags = data[9];

            if (flags & 0x1F)   /* check unset flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            if (flags & 0xC0)   /* check compress and encrypt flags */
            {
                return PARSE_ID3_FRAME_ABORT;
            }

            if (flags & 0x20)   /* check group id flag */
            {
                data_offset++;
            }
            break;

        case 2:
            if (size < 6)
            {
                return PARSE_ID3_FRAME_NO_SPACE;
            }
            memcpy(frame_header->id, data, 3);  /* 0, 1, 2 */
            frame_header->size = id3_tag_get_uint32(data + 3, 3);       /* 3, 4, 5 */
            frame_header->size += 6;
            data_offset = 6;
            break;

        default:
            data_offset = 0;
            assert(0);
    }

	if( size <= (INT32)data_offset ) // check buffer size
    {
		return PARSE_ID3_FRAME_NO_SPACE;
    }
	else if( frame_header->size <= data_offset ) // frame size
    {
		return PARSE_ID3_FRAME_ABORT;
    }

	frame_header->data_offset = data_offset;
	frame_header->data_size   = frame_header->size - data_offset;

	return PARSE_ID3_FRAME_SUCCESS;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_match_v2_frame_id_from_list
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* id
*     const INT8* id_list[]
*     UINT8 size
* RETURNS
*     INT8
* GLOBALS AFFECTED
*     
*****************************************************************************/
INT8 id3_tag_match_v2_frame_id_from_list( UINT8* id, const INT8* const id_list[], UINT8 size )
{
	UINT32 i;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < NUM_OF_ID3_FRAME; i++)
    {
        if (memcmp(id, id_list[i], size) == 0)
        {
            return i;
        }
    }
	return -1;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v2_frame_data
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UINT8* data
*     id3_tag_frame_header_struct* frame_header
*     id3_tag_struct* id3_tag_para
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL id3_tag_parse_v2_frame_data( UINT8* data, id3_tag_frame_header_struct* frame_header, id3_tag_struct* id3_tag)
{
	INT8    frame_id_index = 0;
	UINT8  *src;
    UINT8  text_encoding = data[0];
	UINT32 text_size; // number of bytes

    switch (id3_tag->v2_major_ver)
	{
	case 4:
		if( text_encoding > 3 )
			return FALSE;
		frame_id_index = id3_tag_match_v2_frame_id_from_list( (UINT8*)frame_header->id, id3_tag_v2_4_frame_id_list, 4 );
		break;
	case 3:
		if( text_encoding > 1 )
			return FALSE;
		frame_id_index = id3_tag_match_v2_frame_id_from_list( (UINT8*)frame_header->id, id3_tag_v2_3_frame_id_list, 4 );
		break;
	case 2:
		if( text_encoding > 1 )
			return FALSE;
		frame_id_index = id3_tag_match_v2_frame_id_from_list( (UINT8*)frame_header->id, id3_tag_v2_2_frame_id_list, 3 );
		break;
	default:
		assert(0);
	}

	if( frame_id_index < 0 ) // quit if it's not our wanted frame
	{
        MCI_TRACE (MCI_AUDIO_TRC,0,"Failed!,error tag!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return FALSE;
	}     

	text_size    = frame_header->data_size - 1; // src text size
	src          = data + 1;
  
    if(id3_tag->audInfo_ptr)
    {
        //id3_tag->audInfo_ptr->textencode=text_encoding; 
		switch(frame_id_index)
		{
		case 0:
			strncpy(id3_tag->audInfo_ptr->title,src,MAX_AUD_DESCRIPTION);
			break;
		case 1:
			strncpy(id3_tag->audInfo_ptr->artist,src,MAX_AUD_DESCRIPTION);
			break;
		case 2:
			strncpy(id3_tag->audInfo_ptr->album,src,MAX_AUD_DESCRIPTION);
			break;
		case 3:
			//memcpy(id3_tag->audInfo_ptr->genre,src,text_size);
			break;
		case 4:
			//memcpy(id3_tag->audInfo_ptr->date,src,text_size);
			break;
		case 5:
			//memcpy(id3_tag->audInfo_ptr->copyright,src,text_size);
			break;
		}	
    }
    
    MCI_TRACE (MCI_AUDIO_TRC,0,"[id3_tag_parse_v2_frame_data,String]:%s,text_size:%d,frame_id_index:%d.",src,text_size,frame_id_index);

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v2_tag
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct* input_stream
*     id3_tag_struct           * id3_tag
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL id3_tag_parse_v2_tag( audio_input_stream_struct* input_stream, id3_tag_struct* id3_tag)
{
	BOOL    data_full_load;
	UINT8   parse_result;
	UINT8   *frame_data_ptr, *ptr;
	UINT32  frame_size, frame_data_size;
	INT32   target_size, buf_size;
	INT32   v2_tag_size, offset;
	id3_tag_frame_header_struct frame_header;
	
	if( audio_input_stream_read( input_stream, 10 ) != 10 )		return FALSE;

	offset      = (INT32)input_stream->total_load;
	buf_size    = (INT32)input_stream->buf_size;
	ptr         = input_stream->buf;
	
	/*
	 * check id3v2 magic UINT16 -- "ID3"
	 * and parse header and set the following 4 fields in id3_tag_para:
	 * (1) v2_size // not include header & footer size
	 * (2) v2_unsync
	 * (3) v2_exthdr
	 * (4) v2_footer
	 */
	if( id3_tag_parse_v2_tag_header( ptr, id3_tag) )
	{
		// check if tag size <= our read buffer size
		if( (v2_tag_size = (INT32)id3_tag->v2_size) > buf_size )
		{
			MCI_TRACE (MCI_AUDIO_TRC,0,"id3_tag_parse_v2_tag,v2_tag_size: %d,buf_size:%d",v2_tag_size,buf_size);
	
			data_full_load = FALSE;
			// read partial data of the tag to read buffer
			if( audio_input_stream_read( input_stream, buf_size ) != buf_size )		return FALSE;
		}
		else
		{
			data_full_load = TRUE;
			
			MCI_TRACE (MCI_AUDIO_TRC,0,"id3_tag_parse_v2_tag,else,v2_tag_size:%d,line is %d.",id3_tag->v2_size,__LINE__);

			// read the whole tag header to read buffer (not include footer)
			if( audio_input_stream_read( input_stream, v2_tag_size ) != v2_tag_size )		return FALSE;
		}
		
		// reverse unsynchronisation process
		if( id3_tag->v2_unsync )
		{
			if( !data_full_load ) // if the tag is larger than our buffer size, we can't do reverse unsync process
				return TRUE;  // but we still return TRUE
			v2_tag_size = id3_tag->v2_reverse_unsync_size = (INT32)id3_tag_reverse_unsync_process( ptr, v2_tag_size );
		}
		
		// target size (v2_tag_size) not including 10 bytes v2 tag header
		target_size = v2_tag_size;
    
		if( id3_tag->v2_exthdr )
		{
			/*
			 * parse extended header and set the following 2 fields in id3_tag_para:
			 * (1) v2_exthdr_size
			 * (2) v2_crc
			 */
			if( !id3_tag_parse_v2_tag_extended_header( ptr, id3_tag, data_full_load ) )			return FALSE;

            /* point to frame start position */
            offset             += id3_tag->v2_exthdr_size;
            ptr                 += id3_tag->v2_exthdr_size;
            target_size     -= id3_tag->v2_exthdr_size;
        }

        /* parse frames */
        for (  frame_size = 0; 
               target_size > 0 && ptr + 15 <= input_stream->tail &&       /* 15 is the maximal frame header size */
               (parse_result = id3_tag_parse_v2_frame_header(ptr, target_size, &frame_header, id3_tag)) != PARSE_ID3_FRAME_NO_SPACE;
               ptr += frame_size, target_size -= frame_size)
        {
            /* check if frame id is invalid or frame size too large */
            if (frame_header.id[0] == 0 || frame_header.size > (UINT32) target_size)
            {
                break;
            }
            
            offset        += frame_size;   /* offset always point to frame start position */
            frame_size = frame_header.size;
            
            if (parse_result != PARSE_ID3_FRAME_ABORT)
            {
                // we need to check if the frame boundary is within buffer
                // otherwise, we need to skip processing the frame data & move to next frame
                
                if (ptr + frame_size <= input_stream->tail)
                {
                    frame_data_ptr = ptr + frame_header.data_offset;

                    /* check unsync flag & reverse unsynchronisation process in a frame */
                    if (!id3_tag->v2_unsync && frame_header.unsync)
                    {
                        frame_header.data_size = id3_tag_reverse_unsync_process(frame_data_ptr, frame_header.data_size);
                    }

                    /* parse frame data */
                    if (frame_header.data_size > 1)
                    {
                        id3_tag_parse_v2_frame_data(frame_data_ptr, &frame_header, id3_tag);
                    }
                }
            }
        }

        // when data_full_load is FALSE, we can't finish parsing it in buffer in last step
        // so we keep loading data & parse frames
        if (!data_full_load)
        {
            target_size += frame_size;
            ptr = input_stream->buf;
            do
            {
                /* seek to the frame header start position */
                audio_input_stream_seek(input_stream, offset);

                /* we assume buf is >= 15 bytes, and read 15 bytes for parsing header */
                if (audio_input_stream_read(input_stream, 15) != 15)    /* 15 is the maximal frame header size */
                {
                    break;
                }

                parse_result = id3_tag_parse_v2_frame_header(ptr, target_size, &frame_header, id3_tag);
                
                if (parse_result == PARSE_ID3_FRAME_NO_SPACE)
                {
                    break;
                }
                
                /* check if frame id is invalid or frame size too large */
                if (frame_header.id[0] == 0 || frame_header.size > (UINT32) target_size)
                {
                    break;
                }

                frame_data_size = frame_header.data_size;

                // if it's our wanted frame, we need to check if the frame data size <= buf_size
                // otherwise, we need to skip processing the frame data & seek to next frame
                if (parse_result != PARSE_ID3_FRAME_ABORT && (INT32) frame_data_size <= buf_size)
                {
                    /* seek to the frame data start position */
                    audio_input_stream_seek(input_stream, offset + frame_header.data_offset);

                    /* read data bytes */
                    if ((UINT32) audio_input_stream_read(input_stream, frame_data_size) != frame_data_size)
                    {
                        break;
                    }

                    /* check unsync flag & reverse unsynchronisation process in a frame */
                    if (!id3_tag->v2_unsync && frame_header.unsync)
                    {
                        frame_data_size = frame_header.data_size = id3_tag_reverse_unsync_process(ptr, frame_data_size);
                    }

                    /* parse frame data */
                    if (frame_header.data_size > 1)
                    {
                        id3_tag_parse_v2_frame_data(ptr, &frame_header, id3_tag);
                    }
                }

                frame_size = frame_header.size;
                target_size -= frame_size;
                offset += frame_size;
            } while (target_size > 0);
        }
		return TRUE;
	}	
	return FALSE;
}


/*****************************************************************************
* FUNCTION
*     id3_tag_parse_v1_tag
* DESCRIPTION
*     parse id3 version info  
*     
* PARAMETERS
*     audio_input_stream_struct* input_stream
*     id3_tag_struct           * id3_tag
* RETURNS
*     BOOL
*     
*****************************************************************************/
BOOL id3_tag_parse_v1_tag( audio_input_stream_struct* input_stream, id3_tag_struct* id3_tag )
{	
	if( audio_input_stream_read( input_stream, 128 ) != 128 )		
	{
		return FALSE;
	} 	
	/*
	 * check id3v1 magic WORD -- "TAG"
	 * and parse header and set the following 5 frames in id3_tag:
	 * (1) title
	 * (2) artist
	 * (3) album
	 * (4) year
	 * (5) genre
	 */
	if( !(input_stream->buf[0] == 'T' && input_stream->buf[1] == 'A' && input_stream->buf[2] == 'G') )	
	{
		return FALSE;
	}

    if(id3_tag->audInfo_ptr)
    {
        strncpy(id3_tag->audInfo_ptr->title,     ((ID3V1_t *)input_stream->buf)->TIT2_ID3V1,MAX_AUD_DESCRIPTION);
        strncpy(id3_tag->audInfo_ptr->artist,    ((ID3V1_t *)input_stream->buf)->TPE1_ID3V1,MAX_AUD_DESCRIPTION);
        strncpy(id3_tag->audInfo_ptr->album,     ((ID3V1_t *)input_stream->buf)->TALB_ID3V1,MAX_AUD_DESCRIPTION);
        //memcpy(id3_tag->audInfo_ptr->date,      ((ID3V1_t *)input_stream->buf)->TYER_ID3V1,4);
        //memcpy(id3_tag->audInfo_ptr->genre,     ((ID3V1_t *)input_stream->buf)->COMM_ID3V1,29);        
        //id3_tag->audInfo_ptr->trackNum=         ((ID3V1_t *)input_stream->buf)->TRCK_ID3V1;
    }  
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     id3_tag_parse
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct* input_stream
*     id3_tag_struct* id3_tag_para
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL id3_tag_parse(CONST UINT32 file , AudDesInfoStruct *aud_info)
{
	INT32 data_size;
	BOOL v2_result = FALSE;
	BOOL v1_result = FALSE;

    id3_tag_struct id3_tag;

    audio_input_stream_struct input_stream;

    if(!aud_info)  return FALSE;	

    id3_tag.audInfo_ptr=aud_info;

    audio_file_input_stream_alloc(file, &input_stream);
    
	audio_input_stream_seek(&input_stream, 0 );
	/*
	 * check id3v2 magic UINT16 -- "ID3" and parse & save data to frames
	 */
    v2_result = id3_tag_parse_v2_tag(&input_stream, &id3_tag);
	
	if( (data_size = audio_input_stream_get_data_size(&input_stream )) >= 128 )
	{
		// move to last 128 bytes before file end for checking id3v1 tag
		audio_input_stream_seek(&input_stream, data_size - 128 );
		
		/*
		 * check id3v1 magic word -- "TAG" and parse & save data to frames
         */
        v1_result = id3_tag_parse_v1_tag(&input_stream, &id3_tag);
    }
    else
    {
        v1_result = FALSE;
    }

	return v1_result || v2_result;
}



