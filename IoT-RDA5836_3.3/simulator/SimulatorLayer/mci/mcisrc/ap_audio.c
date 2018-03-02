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

#include "ap_audio.h"
#include "ap_music.h"
#include "mci.h"

#define __CRC_SUPPORT__

/**********************************************************
 1) "RTENC"         :编码者      2) "TRCK"        :音轨
 3) "TPE1"          :演唱者      4) "TIT2"        :标题
 5) "TALB"          :专辑        6) "TYER"        :年代
 7) "TMED"          :媒体类型    8) "TCOM"        :作曲家
 9) "COMM"          :注释       10) "CON"         :流派
11) "TOPE"          :原创者     12) "TCOP"        :版权
13) "WXXX"          :URL 
**********************************************************/

extern Media_Status AU_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status AIFF_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status WAV_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status AAC_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status DAF_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status WMA_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );
extern Media_Status VM_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param );
extern Media_Status PCM_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param );
extern Media_Status AMR_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  );



UINT8 autGetBitsFromBuf(UINT8 *pBuf, UINT32 *puBitCnt, UINT32 uBitLen)
{
	static UINT8 bCurByte;
	UINT8 bMask;
	UINT32 uCurBitPos;
	UINT8 bValue;

    APP_ASSERT(puBitCnt!=NULL);
	APP_ASSERT(uBitLen>=1 && uBitLen<=8);

	uCurBitPos = *puBitCnt % 8;

	if(uCurBitPos==0) 
	{
	   bCurByte = pBuf[*puBitCnt/8];
	}

	*puBitCnt += uBitLen;

	if(uCurBitPos+uBitLen<=8) 
	{
		bMask  = (UINT8)((1 << (8-uCurBitPos)) - 1);
		bValue = (UINT8)((bCurByte&bMask) >> (8-(uCurBitPos+uBitLen)));
	} 
	else
	{
		bMask  = (UINT8)((1 << (8-uCurBitPos)) - 1);
		bValue = (UINT8)((bCurByte&bMask) << (uBitLen-(8-uCurBitPos)));
		bCurByte = pBuf[*puBitCnt/8];
		bValue |= bCurByte>>(16-uBitLen-uCurBitPos);
	}
	return bValue;
}

INT32 autGetBytesFromFile(MHdl *hdl, STFSAL *pstFSAL, UINT32 fileOffset, UINT32 num_bytes, UINT8 *ptr)
{   
   UINT32 data_size = pstFSAL->uFileSize- fileOffset;

   if(data_size <= num_bytes)   num_bytes = data_size;// EOF

   FS_Seek( pstFSAL->hFile, fileOffset,FS_SEEK_SET);

   data_size=FS_Read( pstFSAL->hFile, ptr, num_bytes);
   
   if(data_size != num_bytes)      
	   return -1; // Playback should be terminated
   else
	   return num_bytes;
}

INT32 autGetBytes(MHdl *hdl, STFSAL *pstFSAL, UINT32 uLen, UINT8 *pbBuf)
{
   UINT32 uDataSize,  uCurPos;

   if(uLen == 0) return 0;
   
   uCurPos    =FS_Seek(pstFSAL->hFile,0,FS_SEEK_CUR);
   
   uDataSize = pstFSAL->uFileSize - uCurPos;

   if(uDataSize <= uLen)      uLen = uDataSize;
   
   if(uLen == 0) return 0;

   if(FS_Read( pstFSAL->hFile, pbBuf, uLen) != ERR_SUCCESS)      return -1;

   return uLen;
}

void autSkipID3Tag( MHdl *hdl, STFSAL *pstFSAL, UINT32 *fileOffset, UINT8 *ptr )
{
   INT32 num_bytes,  tagsize;

   while(1)
   {
      tagsize = 0;
      num_bytes = autGetBytesFromFile(hdl, pstFSAL, *fileOffset, ID3V2_HEADER_LEN, ptr);
      
      if(num_bytes != ID3V2_HEADER_LEN)  return;
      
      if (memcmp(ptr, "ID3", 3) == 0)
      {
         tagsize = (ptr[6] << 21) | (ptr[7] << 14) | (ptr[8] <<  7) | (ptr[9] <<  0);
      
         if(pstFSAL->uFileSize >= (tagsize + 10))// Don't skip ID3 tag if tagsize is invalid
         { 
            *fileOffset += (tagsize+10);            
         }
         else
         {
             return;
         }         
      }
      else
      {
         return;
      }
   }
}



/*****************************************************************************
 * FUNCTION
 *  med_crc_calculate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [IN]        
 *  length      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
UINT32 med_crc_calculate(UINT8 const *data, UINT32 length)
{
#ifdef __CRC_SUPPORT__
    static UINT32 const crc_table[256] = 
    {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
        0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
        0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
        0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
        0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
        0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
        0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,

        0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
        0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
        0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
        0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
        0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
        0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,

        0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
        0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
        0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
        0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
        0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
        0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
        0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,

        0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
        0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
        0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
        0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
        0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
        0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,

        0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
        0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
        0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
        0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
        0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
        0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
        0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,

        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
        0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
        0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
        0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
        0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
        0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
        0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,

        0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
        0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
        0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
        0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
        0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
        0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,

        0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
        0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
        0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
        0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
        0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
        0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
        0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
    };
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    register UINT32 crc;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (crc = 0xffffffffL; length >= 8; length -= 8)
    {
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    }

    switch (length)
    {
        case 7:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 6:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 5:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 4:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 3:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 2:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 1:
            crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        case 0:
            break;
    }

    return crc ^ 0xffffffffL;

#else /*__RICH_AUDIO_PROFILE__*/
    return 0;
#endif /*__RICH_AUDIO_PROFILE__*/

}


/*****************************************************************************
 * FUNCTION
 *  med_util_utf8_to_ucs2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [?]         
 *  dest_size       [IN]        (>=4)
 *  src             [?]         
 *  src_size        [IN]        (>=1) it is used to prevent from infinte loop when src data are not utf8
 * RETURNS
 *  INT32
 *****************************************************************************/
INT32 med_util_utf8_to_ucs2(UINT8 *dest, UINT32 dest_size, UINT8 *src, UINT32 src_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 cnt, c;
    UINT16 ucs2;
    UINT32 dest_count = 0;
    UINT32 src_count = 0;
    UINT8 utf8_bytes_per_char[16] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 3, 4 };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest_size -= ((dest_size % 2) + 2); /* make it a even number + null terminator */

    while ((dest_count < dest_size) && (src_count < src_size) && ((c = *src) != 0))
    {
        cnt = utf8_bytes_per_char[c >> 4];
        switch (cnt)
        {
            case 1:
                dest[dest_count] = c;
                dest[dest_count + 1] = 0;
                dest_count += 2;
                src++;
                src_count++;
                break;
            case 2:
                ucs2 = ((UINT16) (c & 0x1F) << 6) | (UINT16) (src[1] & 0x3F);
                dest[dest_count] = ucs2 & 0xFF;
                dest[dest_count + 1] = ucs2 >> 8;
                dest_count += 2;
                src += 2;
                src_count += 2;
                break;
            case 3:
                ucs2 = ((UINT16) (c & 0x0F) << 12) | ((UINT16) (src[1] & 0x3F) << 6) | (UINT16) (src[2] & 0x3F);
                dest[dest_count] = ucs2 & 0xFF;
                dest[dest_count + 1] = ucs2 >> 8;
                dest_count += 2;
                src += 3;
                src_count += 3;
                break;
            case 4:
                src += 4;   /* skip it since we don't support UCS4 */
                src_count += 4;
                break;
            case 0:
                src++;      /* something wrong with the src, skip it */
                src_count++;
                break;
        }
    }
    dest[dest_count] = 0;
    dest[dest_count + 1] = 0;
    return dest_count + 2;
}


/*****************************************************************************
 * FUNCTION
 *  med_util_ucs2_to_utf8
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [?]         
 *  dest_size       [IN]        (>=1)
 *  src             [?]         
 *  src_size        [IN]        (>=2)
 * RETURNS
 *  INT32
 *****************************************************************************/
INT32 med_util_ucs2_to_utf8(UINT8 *dest, UINT32 dest_size, UINT8 *src, UINT32 src_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 lb, hb;
    UINT32 dest_count = 0;
    UINT32 src_count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest_size -= 1; /* one UINT8 space for null terminator */

    lb = src[0];
    hb = src[1];
    while (dest_count < dest_size && src_count < src_size &&   (lb != 0 || hb != 0))    /* src does not encounter null terminator */ /* not exceed limited size */
    {
        if (hb == 0 && (lb & 0x80) == 0)
        {
            dest[dest_count++] = lb;
        }
        else
        {
            if (hb & 0xF8 == 0)
            {
                dest[dest_count++] = 0xC0 | (hb << 2) | (lb >> 6);
            }
            else
            {
                dest[dest_count++] = 0xE0 | (hb >> 4);
                dest[dest_count++] = 0x80 | ((hb & 0x0F) << 2) | (lb >> 6);
            }
            dest[dest_count++] = 0x80 | (lb & 0x3F);
        }
        src_count += 2;
        lb = src[src_count];
        hb = src[src_count + 1];
    }
    dest[dest_count] = 0;
    return dest_count + 1;
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
/*INT32 mmi_chset_convert( UINT8 src_type, UINT8 dest_type, INT8 *src_buff,  INT8 *dest_buff, INT32 dest_size)
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
*/

/*****************************************************************************
* FUNCTION
*     Audio_GetDetailsInfo
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     WCHAR* filename
*     aud_info_struct* aud_info_p
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL Audio_GetDetailsInfo(CONST UINT32 file ,
											audInfoStruct  *aud_info,
											CONST Music_type_t format   )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/    
    STFSAL fsal_handle;
    INT8 Lan_id = g_comval->langid;
    extern UINT32 play_total_time;
    ID3Info_t ID3Info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Media_Status(*get_aud_info_func) (STFSAL * pstFSAL, audInfoStruct * contentInfo, void *param) = NULL;

    if(!aud_info)  	return FALSE;
    
    //aud_info->size=
    fsal_handle.uFileSize = FS_GetFileSize(file);

    fsal_handle.hFile    =file;    

    id3_tag_parse(file, aud_info);    
    
	switch (format)
    {
#ifdef DAF_DECODE
	case MCI_TYPE_DAF:
		get_aud_info_func = DAF_GetContentDescInfo;
		break;
#endif /* DAF_DECODE */ 
#ifdef WAV_CODEC
	case MCI_TYPE_WAV:
		get_aud_info_func = WAV_GetContentDescInfo;
		break;
#endif /* WAV_CODEC */
#ifdef AAC_DECODE
	case MCI_TYPE_AAC:
		get_aud_info_func = AAC_GetContentDescInfo;
		break;
#endif /* AAC_DECODE */ 
#if 1//def WMA_DECODE
	case MCI_TYPE_WMA:
		get_aud_info_func = WMA_GetContentDescInfo;
		break;
#endif /* WMA_DECODE */
#ifdef AU_DECODE
	case Music_AU:
		get_aud_info_func = AU_GetContentDescInfo;
		break;
#endif /* AU_DECODE */
#ifdef AIFF_DECODE
	case Music_AIF:
		get_aud_info_func = AIFF_GetContentDescInfo;
		break;
#endif /* AIFF_DECODE */
#ifdef AMR_DECODE
	case Music_AMR:
		get_aud_info_func = AMR_GetContentDescInfo;
		break;
#endif /* AMR_DECODE */
#ifdef VM_CODEC
	case Music_VM:
		get_aud_info_func = VM_GetContentDescInfo;
		break;
#endif /* VM_CODEC */
#ifdef PCM_CODEC
	case Music_PCM:
		get_aud_info_func = PCM_GetContentDescInfo;
		break;
#endif /* PCM_CODEC */		
	default:
		break;
    }

    hal_HstSendEvent(SYS_EVENT,get_aud_info_func);
    
    FS_Seek(file,0,FS_SEEK_SET);
    
    if (get_aud_info_func != NULL)
    {        
          
          if (get_aud_info_func(&fsal_handle, aud_info, (void*)format) == MEDIA_SUCCESS)
          { 
			  app_trace(1,"time==>%d\n",aud_info->time);
			  app_trace(1,"bitRate==>%d\n",aud_info->bitRate);
			  app_trace(1,"sampleRate==>%d\n",aud_info->sampleRate);
			  //app_trace(1,"stereo==>%d\n",aud_info->stereo);
			  //app_trace(1,"trackNum==>%d\n",aud_info->trackNum);

#if 0     
              channel_num = (aud_info->stereo ? 2 : 1);
              
              daf_set_data_info(  aud_info_p, channel_num, aud_info->time,  size,  aud_info->bitRate, aud_info->sampleRate);

              /* Fill track number string */
              if(aud_info->trackNum > 0)
              {
                  kal_wsprintf((WCHAR*) aud_info_p->track_num, "%d", aud_info->trackNum);
              }
#endif        
          }

    }
    // FS_Close(file);

    if(aud_info->title[0])
    {
      DealStringEnd( aud_info->title, Lan_id);
      DealStrEndSpace(  aud_info->title );
      app_trace(1,"title==>%s\r\n", aud_info->title);
    }

    if(aud_info->album[0])
    {
      DealStringEnd( aud_info->album, Lan_id);
      DealStrEndSpace( aud_info->album );
      app_trace(1,"album==>%s\r\n", aud_info->album);      
    }

    if(aud_info->artist[0])
    {
      DealStringEnd( aud_info->artist, Lan_id);
      DealStrEndSpace( aud_info->artist );
      app_trace(1,"artist==>%s\r\n",aud_info->artist);    
    }

    /*if(aud_info->genre[0])
    {
      DealStringEnd( aud_info->genre, Lan_id);
      DealStrEndSpace( aud_info->genre );
      app_trace(1,"genre==>%s\r\n",aud_info->genre);    
    }

    if(aud_info->copyright[0])
    {
      DealStringEnd( aud_info->copyright, Lan_id);
      DealStrEndSpace( aud_info->copyright );    
      app_trace(1,"copyright==>%s\r\n",aud_info->copyright);
    }

    if(aud_info->date[0])
    {
      DealStringEnd( aud_info->date, Lan_id);
      DealStrEndSpace( aud_info->date );    
      app_trace(1,"date==>%s\r\n",aud_info->date);
    }	*/

    if(aud_info->author[0])
    {
      DealStringEnd( aud_info->author, Lan_id);
      DealStrEndSpace( aud_info->author );    
      app_trace(1,"author==>%s\r\n",   aud_info->author);  
    }	

    if(aud_info->time == 0)
        aud_info->time = 100000;

#ifdef WIN32
    play_total_time = aud_info->time;
    {
        FILETIME create_time;
        SYSTEMTIME stUTC, stLocal;
        GetFileTime(file,&create_time, NULL, NULL);
        FileTimeToSystemTime(&create_time, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    
        aud_info->createDate = (stLocal.wYear) * 512 + stLocal.wMonth * 32 + stLocal.wDay;
        aud_info->createTime = stLocal.wHour *3600 + stLocal.wMinute * 60 + stLocal.wSecond;
    }
#endif
 
    return TRUE;
}






