
/********************************************************************************
*                RDA API FOR MP3HOST
*
*        (c) Copyright, RDA Co,Ld.
*             All Right Reserved
*
********************************************************************************/
#include "ap_audio.h"


#if defined(AAC_DECODE)


#define AAC_MAX_DATA_REQUESTED  1024
#define MAX_FRAME_CHECK_LEN        (2*1024)
#define MIN_DATA_LEN                           256
#define SAMPLES_PER_FRAME             1024


typedef struct aacVar{
   BOOL     bIsTypeADTS;
   BOOL     bIsTypeADIF;
   BOOL     stereo;
   UINT32   rawDataBlockOffset;
   UINT32   bitRate;
   UINT8    bADTSFixedHeader[4];
   UINT8    sr_idx;
} aacInternal;

typedef struct mh_internal {
   MHdl                mh;
   aacInternal      aac;
   STFSAL          *pstFSAL;
   audInfoStruct   *description;
   UINT8         buf[AAC_MAX_DATA_REQUESTED];
   UINT32       fileOffset;
   UINT64       frameDur; // in ms * 10E-6
} aacMediaHdl;



static const INT32 sampleRateTab[]          ={96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000};
static const UINT32 maxByterateFromIndex[12] ={ 0, 0, 0, 36000, 33076, 24000, 18000, 16538, 16000, 12000, 9000, 6000 };


/* Refer to ISO/IEC 14496-3:2001(E) Annex 1.A */
static Media_Status aacParseHeader(aacMediaHdl *ihdl, UINT8 *pBuf)
{
   UINT8  protection_absent, no_raw_data_blocks_in_frame, num_program_config_elements;
   UINT32 uBitCnt=0;

   if ((pBuf[0] == 0xFF) && ((pBuf[1] & 0xF0) == 0xF0)) /* ADTS */
   { 
      /* fixed header */
      autGetBitsFromBuf(pBuf, &uBitCnt, 8); /* syncword(12) + ID(1) */
      autGetBitsFromBuf(pBuf, &uBitCnt, 5);
      if (autGetBitsFromBuf(pBuf, &uBitCnt, 2) != 0x00) /* layer */
         return MEDIA_BAD_FORMAT;
      protection_absent = autGetBitsFromBuf(pBuf, &uBitCnt, 1); /* protection absent */

      if ( autGetBitsFromBuf(pBuf, &uBitCnt, 2) != 0x01) /* profile object type */
         return MEDIA_UNSUPPORTED_TYPE;

      ihdl->aac.sr_idx = autGetBitsFromBuf(pBuf, &uBitCnt, 4); /* sampling frequency index */
      
      if(ihdl->aac.sr_idx < 3 || ihdl->aac.sr_idx > 11)
         return MEDIA_UNSUPPORTED_FREQ;

      autGetBitsFromBuf(pBuf, &uBitCnt, 1); /* private bit */
      {
         UINT8 channel = autGetBitsFromBuf(pBuf, &uBitCnt, 3);
         if ( channel > 0x02) /* channel configuration */
            return MEDIA_UNSUPPORTED_CHANNEL;
         if ( channel == 2 )
            ihdl->aac.stereo = TRUE;
      }
      /* original/copy(1), home(1) */ /* copyright bit(1), copyright start(1) */
      autGetBitsFromBuf(pBuf, &uBitCnt, 4);

      /* variable header */
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);

      no_raw_data_blocks_in_frame = autGetBitsFromBuf(pBuf, &uBitCnt, 2);

      if (protection_absent == 0)
      {
         if(no_raw_data_blocks_in_frame)
         { /* no_raw_data_blocks_in_frame */
            autGetBitsFromBuf(pBuf, &uBitCnt, 8);
            autGetBitsFromBuf(pBuf, &uBitCnt, 8);
         }
         autGetBitsFromBuf(pBuf, &uBitCnt, 8);
         autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      }
      /* cache ADTS Fixed Header */
      memcpy(ihdl->aac.bADTSFixedHeader, pBuf, 4);
      ihdl->aac.bADTSFixedHeader[3] &= 0xF0;
      ihdl->aac.bIsTypeADTS = TRUE;
    } 
    else if ((pBuf[0] == 'A') && (pBuf[1] == 'D') && (pBuf[2] == 'I') && (pBuf[3] == 'F')) /* ADIF */
    { 
      UINT8 uBitstreamType;
      autGetBitsFromBuf(pBuf, &uBitCnt, 8); /* read out 4 bytes */
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      if (autGetBitsFromBuf(pBuf, &uBitCnt, 1)) 
      { /* copyright_id_present */
         UINT32 uCount;
         for (uCount=0; uCount<9; uCount++)
            autGetBitsFromBuf(pBuf, &uBitCnt, 8);
      }
      autGetBitsFromBuf(pBuf, &uBitCnt, 2); /* origianl copy(1), home(1) */
      uBitstreamType = autGetBitsFromBuf(pBuf, &uBitCnt, 1);

      ihdl->aac.bitRate = autGetBitsFromBuf(pBuf, &uBitCnt, 8) |
      ((UINT32)autGetBitsFromBuf(pBuf, &uBitCnt, 8) << 8) |
      ((UINT32)autGetBitsFromBuf(pBuf, &uBitCnt, 7) << 16);

      num_program_config_elements = autGetBitsFromBuf(pBuf, &uBitCnt, 4);

      if (0 == uBitstreamType)
      {
         autGetBitsFromBuf(pBuf, &uBitCnt, 8);
         autGetBitsFromBuf(pBuf, &uBitCnt, 8);
         autGetBitsFromBuf(pBuf, &uBitCnt, 4);
      }
      {
         UINT32 uCount,i;
         UINT8  num_front_channel_elements, num_side_channel_elements
                  , num_back_channel_elements, num_lfe_channel_elements
                  , num_assoc_data_elements, num_valid_cc_elements, comment_field_bytes;
               /*--- PCE ---*/
         for (uCount = 0; uCount < num_program_config_elements + 1; uCount++)
         {
            autGetBitsFromBuf(pBuf, &uBitCnt, 4); /* element_instance_tag */
            if ( autGetBitsFromBuf(pBuf, &uBitCnt, 2) != 0x01) /* object type */
               return MEDIA_UNSUPPORTED_TYPE;

            ihdl->aac.sr_idx = autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            if(ihdl->aac.sr_idx < 3 || ihdl->aac.sr_idx > 11)
               return MEDIA_UNSUPPORTED_FREQ;
            num_front_channel_elements = autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            num_side_channel_elements = autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            num_back_channel_elements = autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            num_lfe_channel_elements     = autGetBitsFromBuf(pBuf, &uBitCnt, 2);
            num_assoc_data_elements  = autGetBitsFromBuf(pBuf, &uBitCnt, 3);
            num_valid_cc_elements       = autGetBitsFromBuf(pBuf, &uBitCnt, 4);

            if (autGetBitsFromBuf(pBuf, &uBitCnt, 1) == 1)
               autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            if (autGetBitsFromBuf(pBuf, &uBitCnt, 1) == 1)
               autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            if (autGetBitsFromBuf(pBuf, &uBitCnt, 1) == 1)
               autGetBitsFromBuf(pBuf, &uBitCnt, 3);
            for (i = 0; i < num_front_channel_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 5);
            for (i = 0; i < num_side_channel_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 5);
            for (i = 0; i < num_back_channel_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 5);
            for (i = 0; i < num_lfe_channel_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            for (i = 0; i < num_assoc_data_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 4);
            for (i = 0; i < num_valid_cc_elements; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 5);

            uBitCnt += 8-(uBitCnt%8); // UINT8 alignment

            comment_field_bytes = autGetBitsFromBuf(pBuf, &uBitCnt, 8);
            for (i = 0; i < comment_field_bytes; i++)
               autGetBitsFromBuf(pBuf, &uBitCnt, 8);
         }
      }
      ihdl->aac.bIsTypeADIF = TRUE;
   } 
   else 
   {
      return MEDIA_FAIL;
   }
   ihdl->aac.rawDataBlockOffset = uBitCnt/8;
   ihdl->frameDur = (UINT64)SAMPLES_PER_FRAME * (UINT64)1000000000 / (UINT64) sampleRateTab[ihdl->aac.sr_idx];
   /* Currently, DSP decoder only support 1024 samples per frame */
   return MEDIA_SUCCESS;
}

static Media_Status aacSyncFirstFrame( aacMediaHdl *ihdl )
{  /* bit-stream should be more than 512 Bytes */
    Media_Status ret;
	UINT8  *ptr, *end;
	UINT32 num_bytes;
	UINT32 firstFrameOffset = 0;
	BOOL   found = FALSE;

	while( !found )
	{  // check if it is out of searching range
		if( firstFrameOffset > MAX_FRAME_CHECK_LEN)		return MEDIA_BAD_FORMAT;

		num_bytes = autGetBytesFromFile((MHdl*)ihdl, ihdl->pstFSAL, ihdl->fileOffset, AAC_MAX_DATA_REQUESTED, &ihdl->buf[0]);
    
		if(num_bytes < (MIN_DATA_LEN *2)) return MEDIA_BAD_FORMAT;

		for( ptr = &ihdl->buf[0], end = (ptr + num_bytes - MIN_DATA_LEN) ; ptr < end; ptr++ )
		{
		    ret=aacParseHeader( ihdl, ptr );
			if( ret == MEDIA_SUCCESS )
			{
				ihdl->fileOffset += (UINT32)(ptr - &ihdl->buf[0]);
				found = TRUE;
				break;
			}
		    if( ret != MEDIA_FAIL )     return ret;
		}
		if( !found )
		{
			ihdl->fileOffset += (num_bytes - MIN_DATA_LEN);
			firstFrameOffset += num_bytes - MIN_DATA_LEN;
        }
	}
   return MEDIA_SUCCESS;
}




Media_Status AAC_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  )
{
    Media_Status ret;
    aacMediaHdl *ihdl = (aacMediaHdl*)COS_Malloc( sizeof(aacMediaHdl) );
	MHdl *hdl = (MHdl *) ihdl;  
    if(ihdl)
          memset(ihdl, 0, sizeof(aacMediaHdl));
    else
          return MEDIA_FAIL;    
      
    ihdl->pstFSAL    = pstFSAL;
    ihdl->description= contentInfo;    

    autSkipID3Tag( hdl, ihdl->pstFSAL, &ihdl->fileOffset, &ihdl->buf[0] );

    if ((ret=aacSyncFirstFrame( ihdl ))!=MEDIA_SUCCESS)
    {
       COS_Free( ihdl );
       return ret;
    }

   if(ihdl->aac.bIsTypeADTS)
   {
      contentInfo->stereo = ihdl->aac.stereo;
   }
   else
   {
      contentInfo->stereo = TRUE;
   }
   
   {
      UINT64 bitRate = maxByterateFromIndex[ihdl->aac.sr_idx];
      if(bitRate)   contentInfo->time = (UINT64)pstFSAL->uFileSize* 8000 / bitRate;
   }
   
   contentInfo->sampleRate = sampleRateTab[ihdl->aac.sr_idx];

   COS_Free( ihdl );

   return MEDIA_SUCCESS;
}


#endif /* AAC_DECODE */



