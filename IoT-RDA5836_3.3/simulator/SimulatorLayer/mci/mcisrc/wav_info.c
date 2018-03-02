/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ap_audio.h"


#ifdef WAV_CODEC


#define  MSADPCM_EXTRA_COEFF        10
#define  WAV_MAX_DATA_REQUESTED  1024
#define  IS_AIFF                                         0
#define  IS_AIFF_C                                    1

#define _bytes_reverse_32( _before, _after )  \
{ _after =  ( _before << 24 ) | ( _before & 0x0000FF00 ) << 8 | ( _before & 0x00FF0000 ) >> 8 | ( _before >> 24 ); }
#define _bytes_reverse_16( _before, _after )  \
{ _after = ( _before << 8 ) | ( _before >> 8 ); }


typedef struct wavVar{
   UINT16         BlockCount;
   UINT16         data[16];
   UINT16         channel_num;
   UINT16         formatTag;
   UINT16         samplesPerBlock;
   INT32          sampleRate;
   INT32          AvgBytesPerSec;
   INT32          dataChunkOffset;
   UINT32         wDataLength;
   INT16          BitsPerSample;
   INT16           wBlockAlign;
   INT16           nNumCoef;
   INT8             sr_index;

// AIFF
   UINT8           aiff_type;          // the type of aiff file, either IS_AIFF or IS_AIFF_C
   UINT16         data_size;          // data size of a sample, in UINT8
   UINT32         ssnd_offset;        // offset in ssnd chunk
   UINT32         word_left_in_form;  // data count left in the form chunk, in UINT16
   UINT32         word_left_in_ck;    // data count left in the current chunk, in UINT16
   UINT32         ssnd_state;         // 0: not found yet
                                          // 1: ssnd is found but the actual sound data is not found
                                          // 2: sound data is found
// AU
   UINT32         data_offset;     // data offset in UINT8, found in AU file header
   UINT32         data_length;     // length of sound data in UINT8, found in AU file header
   UINT32         data_read;       // total data read so far, in UINT8
   UINT32         nextChunkOffset;    // the offset of next chunk, in UINT16
   BOOL           is_stereo;
   BOOL           mono2stereo;
   INT16          aCoeff[MSADPCM_EXTRA_COEFF][2];    // allow 10 more coefficient set
} wavInternal;

typedef struct wav_internal {
   MHdl             mh;
   wavInternal      wav;
   STFSAL           *pstFSAL;
   audInfoStruct    *description;
   UINT16       buf[WAV_MAX_DATA_REQUESTED/2];
   UINT32       fileOffset;
   UINT32       file_size;  
} wavMediaHdl;


static const UINT8 SampleRateTable[] = {
   0x00, /*8000*/   0x11,/* 11025 */ 0x12,/* 12000 */
   0x20, /*16000,*/ 0x21,/* 22050,*/ 0x22,/* 24000,*/
   0x40, /*32000,*/ 0x41,/* 44100,*/ 0x42 /* 48000 */
};


static BOOL wavGetByte(wavMediaHdl *ihdl, UINT8 *data)
{
   *data = ihdl->mh.rb_base[ihdl->mh.read];
   ihdl->mh.read++;
   if( ihdl->mh.read >= ihdl->mh.write)
      return FALSE;
   return TRUE;
}

static BOOL wavGetWord(wavMediaHdl *ihdl, UINT16 *data)
{
   UINT8 buf_8;

   if(!wavGetByte(ihdl, &buf_8))      return FALSE;
   *data = (UINT16)buf_8;

   if(!wavGetByte(ihdl, &buf_8))      return FALSE;

   *data |= (UINT16)buf_8 << 8;
   return TRUE;
}

static BOOL wavGetDword(wavMediaHdl *ihdl, UINT32 *data)
{
   UINT16 buf_16;
   if(!wavGetWord(ihdl, &buf_16))      return FALSE;
   
   *data = (UINT32)buf_16;

   if(!wavGetWord(ihdl, &buf_16))      return FALSE;

   *data |= (UINT32)buf_16 << 16;
   return TRUE;
}

static BOOL wavSkipByte(wavMediaHdl *ihdl)
{
   ihdl->mh.read++;
   if( ihdl->mh.read >= ihdl->mh.write)
      return FALSE;
   return TRUE;
}

static BOOL wavSkipNBytes(wavMediaHdl *ihdl, UINT32 NBytes)
{
   UINT32 cnt;

   for(cnt=NBytes; cnt > 0; cnt--)
   {
      if(!wavSkipByte(ihdl))         return FALSE;
   }
   return TRUE;
}

/* search "data" chunk descriptor */
static BOOL wavReachDataChunk( wavMediaHdl *ihdl )
{
   UINT32 data_32, uRead = 0;
   do 
   {
      if( (ihdl->mh.write - ihdl->wav.dataChunkOffset) < 8 || (ihdl->mh.read > ihdl->mh.write) )   return FALSE;

      // Get chunk id
      wavGetDword(ihdl, &data_32);
      uRead += 4;
      if( data_32 == 0x61746164 )   break;  // "data"
      wavGetDword(ihdl, &ihdl->wav.nextChunkOffset);
      uRead += 4;

      if(ihdl->wav.dataChunkOffset > ihdl->mh.write) return FALSE;
      wavSkipNBytes(ihdl, ihdl->wav.nextChunkOffset);
      uRead += ihdl->wav.nextChunkOffset;
   }while( 1 );

   wavGetDword(ihdl, &ihdl->wav.wDataLength);
   uRead += 4;
   ihdl->wav.dataChunkOffset += uRead;
   ihdl->fileOffset = ihdl->wav.dataChunkOffset;

   if(4 != ihdl->wav.BitsPerSample)
   {
      ihdl->wav.wDataLength *= (16/ihdl->wav.BitsPerSample);
   }
   else
   {
       INT64 len, samplePerBlock;
       len                      = (INT64)ihdl->wav.wDataLength;
       samplePerBlock = (INT64)ihdl->wav.samplesPerBlock;
       ihdl->wav.wDataLength = len * samplePerBlock * 2 * ihdl->wav.channel_num / ihdl->wav.wBlockAlign;
   }
   return TRUE;
}

static BOOL getSampleRateIdx( wavMediaHdl *ihdl, UINT32 SampleRate )
{
   switch( SampleRate / 1000 )
   {
      case 7: case 8:   ihdl->wav.sr_index = 0;
         break;
      case 10: case 11: ihdl->wav.sr_index = 1;
         break;
      case 12:                ihdl->wav.sr_index = 2;
         break;
      case 15: case 16: ihdl->wav.sr_index = 3;
         break;
      case 21: case 22: ihdl->wav.sr_index = 4;
         break;
      case 23: case 24: ihdl->wav.sr_index = 5;
         break;
      case 31: case 32: ihdl->wav.sr_index = 6;
         break;
      case 43: case 44: ihdl->wav.sr_index = 7;
         break;
      case 47: case 48: ihdl->wav.sr_index = 8;
         break;
      default:
         return FALSE;   // The sample rate is not supported by ASP.
   }
   return TRUE;
}

static Media_Status wavReadHeader( wavMediaHdl *ihdl )
{
   UINT32 dummy_32, ckSize, bytes_read;
   UINT16 FormatTag;

/* Check to see if header is invalid, "RIFF" chunk descriptor */
   wavGetDword(ihdl, &dummy_32);

   if( dummy_32 != 0x46464952 )      return MEDIA_BAD_FORMAT;
   
   wavSkipNBytes(ihdl, 4); // Skip 2 WORD
   wavGetDword(ihdl, &dummy_32);
   
   if( dummy_32 != 0x45564157 )      return MEDIA_BAD_FORMAT;
   
   do 
   {
      if(!wavGetDword(ihdl, &dummy_32))         return MEDIA_BAD_FORMAT;
      
      if( dummy_32 == 0x20746D66 )        // seek sub-chunk "fmt chunk"
         break;
      wavGetDword(ihdl, &dummy_32);
      wavSkipNBytes(ihdl, dummy_32);
   }while(1);
   
   // Get the fmt ckSize
   wavGetDword(ihdl, &ckSize);
   if( ckSize < 16 )
      return MEDIA_BAD_FORMAT;
   /* Check fmt chunk */
   wavGetWord(ihdl, &FormatTag);
   ihdl->wav.formatTag = FormatTag;
   wavGetWord(ihdl, &ihdl->wav.channel_num);
   if( ihdl->wav.channel_num == 1 ) 
   {
      ihdl->mh.mono2stereo = TRUE;
      ihdl->wav.is_stereo      = FALSE;
   }
   else if( ihdl->wav.channel_num == 2 ) 
   {
      ihdl->wav.is_stereo = TRUE;
   }
   else
   {
      return MEDIA_BAD_FORMAT;
   }

   // sample rate
   wavGetDword(ihdl, (UINT32 *)&ihdl->wav.sampleRate);

   if( FALSE == getSampleRateIdx( ihdl, ihdl->wav.sampleRate ) )
      return MEDIA_UNSUPPORTED_FREQ; // The sample rate is not supported by ASP.

   wavGetDword(ihdl,(UINT32 *)&ihdl->wav.AvgBytesPerSec);
   wavGetWord(ihdl, (UINT16 *)&ihdl->wav.wBlockAlign);
   wavGetWord(ihdl, (UINT16 *)&ihdl->wav.BitsPerSample); // 36

   if ( (ihdl->wav.wBlockAlign == 0) || (ihdl->wav.BitsPerSample == 0) ) 
   {
      return MEDIA_BAD_FORMAT;
   }

   bytes_read = 16;
   switch( FormatTag ) 
   {
      case 0x0001:
         if( ihdl->wav.BitsPerSample == 8 )
            ;
         else if( ihdl->wav.BitsPerSample != 16 )
            return MEDIA_UNSUPPORTED_TYPE;
         break;
      case 0x0006:
      case 0x0007:
         break;
      case 0x0011:
         if( ihdl->wav.BitsPerSample != 4 )   // support 4 bit dvi-adpcm only
            return MEDIA_UNSUPPORTED_TYPE;
         wavSkipNBytes(ihdl, 2);
         wavGetWord(ihdl, &ihdl->wav.samplesPerBlock);
         bytes_read += 4;
         break;
      case 0x0002:                           // MS ADPCM
         if( ihdl->wav.BitsPerSample != 4 )  // support 4 bit ms-adpcm only
            return MEDIA_UNSUPPORTED_TYPE;
         wavSkipNBytes(ihdl, 2);
         wavGetWord(ihdl, &ihdl->wav.samplesPerBlock);
         wavGetWord(ihdl, (UINT16 *)&ihdl->wav.nNumCoef);
         if( ihdl->wav.nNumCoef > MSADPCM_EXTRA_COEFF + 7 )
            return MEDIA_UNSUPPORTED_TYPE;

        wavSkipNBytes(ihdl, 28); // Skip 14 UINT16

         for( dummy_32=0; dummy_32 < ihdl->wav.nNumCoef - 7; dummy_32++ )
         {
            // These values are stored in 8.8 signed format !!!
            wavGetWord(ihdl, (UINT16 *)&ihdl->wav.aCoeff[dummy_32][0]);
            wavGetWord(ihdl, (UINT16 *)&ihdl->wav.aCoeff[dummy_32][1]);
         }
         bytes_read += 6 + ihdl->wav.nNumCoef * 4;
         break;
      default:
         return MEDIA_UNSUPPORTED_TYPE;
   }

   wavSkipNBytes(ihdl, ckSize - bytes_read);
   ihdl->wav.nextChunkOffset = 0;
   ihdl->wav.dataChunkOffset = ihdl->mh.read;

   return MEDIA_SUCCESS;
}

#if defined( AIFF_DECODE )
// Look for SSND chunk
// Bedofe calling this function, aiff.mb_count should be assigned correct value.
// Return value:
//    0: ssnd not found yet
//    1: ssnd found but the actual sound data is not found yet
//    2: sound data is found
static UINT32 aiffLookForSSND( wavMediaHdl *ihdl )
{
   MHdl *hdl = (MHdl *)ihdl;
   UINT32 data_32;

   while( 1 )
   {
      if( ihdl->wav.word_left_in_ck )
      {
         data_32 = (UINT32) hdl->GetDataCount(hdl);
         if(data_32 >= (ihdl->wav.word_left_in_ck << 1))
		 {
            wavSkipNBytes(ihdl, (ihdl->wav.word_left_in_ck << 1));
            ihdl->wav.word_left_in_ck = 0;
            ihdl->wav.word_left_in_form -= ihdl->wav.word_left_in_ck;
         }
         if( ihdl->wav.word_left_in_ck )
            return 0;
      }
      if( hdl->GetDataCount(hdl) < 16 )
         return 0;
      wavGetDword(ihdl, &data_32 );   // Get chunk id
      ihdl->wav.word_left_in_form -= 2;
      if( data_32 == 0x444e5353 )
         break;
      wavGetDword(ihdl, &data_32 );    // Get chunk size
      ihdl->wav.word_left_in_form -= 2;
      _bytes_reverse_32( data_32, ihdl->wav.word_left_in_ck )
      ihdl->wav.word_left_in_ck = ( ihdl->wav.word_left_in_ck + 1 ) >> 1;
   }
   // the ssnd is found
   wavGetDword(ihdl, &data_32 ); // chunk size
   _bytes_reverse_32( data_32, ihdl->wav.word_left_in_ck );
   ihdl->wav.word_left_in_ck = ( ihdl->wav.word_left_in_ck + 1 ) >> 1;
   wavGetDword(ihdl, &data_32 ); // SSND offset
   _bytes_reverse_32( data_32, ihdl->wav.ssnd_offset );
   wavSkipNBytes(ihdl, 4);  // blockSize
   ihdl->wav.word_left_in_ck -= 4;
   ihdl->wav.word_left_in_form -= 6;
   if( ihdl->wav.ssnd_offset == 0 )
   {
      ihdl->fileOffset += ihdl->mh.read;
      return 2;
   }
   else if( ihdl->wav.ssnd_offset & 0x01 )
   {
      return 0;   // We don't support odd offset.
   }
   else
   {
      ihdl->fileOffset += (ihdl->mh.read + ihdl->wav.ssnd_offset);
      ihdl->wav.ssnd_offset = 0;
      return 2;
   }
}

static BOOL aiffReadHeader( wavMediaHdl *ihdl )
{
   UINT16 data_16_a, data_16_b;
   UINT32 data_32;

   // file magic, 0x4d524f46 for ASCII "MROF"
   if( wavGetDword( ihdl, &data_32 ) == FALSE || data_32 != 0x4d524f46 )     return FALSE;

   // chunk size
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;
   _bytes_reverse_32( data_32, ihdl->wav.word_left_in_form );
   ihdl->wav.word_left_in_form = ( ihdl->wav.word_left_in_form + 1 ) >> 1;

   // aiff type
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;

   if( data_32 == 0x46464941 )   // 0x46464941 for ASCII "FFIA"
      ihdl->wav.aiff_type = IS_AIFF;
   else if( data_32 == 0x43464941 ) // 0x46464941 for ASCII "CFIA"
      ihdl->wav.aiff_type = IS_AIFF_C;
   else
      return FALSE;
   ihdl->wav.word_left_in_form -= 2;

   // Look for the COMM chunk
   while( 1 )
   {
      if( wavGetDword( ihdl, &data_32 ) == FALSE || data_32 == 0x444e5353 )
         return FALSE;  // Counld not find the COMM chunk or the SSND chunk is before COMM chunk, error!
      ihdl->wav.word_left_in_form -= 2;
      if( data_32 == 0x4d4d4f43 )
         break;

      if( wavGetDword( ihdl, &data_32 ) == FALSE )
         return FALSE;
      _bytes_reverse_32( data_32, ihdl->wav.word_left_in_ck );
      if( wavSkipNBytes( ihdl, ihdl->wav.word_left_in_ck ) == FALSE )
         return FALSE;
      ihdl->wav.word_left_in_ck = ( ihdl->wav.word_left_in_ck + 1 ) >> 1;
      ihdl->wav.word_left_in_form -= (ihdl->wav.word_left_in_ck + 2);
   }
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;
   ihdl->wav.word_left_in_form -= 2;
   _bytes_reverse_32( data_32, ihdl->wav.word_left_in_ck );
   if( ihdl->wav.aiff_type == IS_AIFF && ihdl->wav.word_left_in_ck != 18 )
      return FALSE;   // The file is AIFF and ckSize of COMM chunk is not equal to 18, error!
   ihdl->wav.word_left_in_ck = ( ihdl->wav.word_left_in_ck + 1 ) >> 1;

   // Check channel count
   if( wavGetWord( ihdl, &data_16_a) == FALSE )
      return FALSE;
   if( data_16_a == 0x0100 ) 
   {
      ihdl->wav.channel_num = 1;
      ihdl->wav.is_stereo = FALSE;
      ihdl->mh.mono2stereo = TRUE;
   }
   else if( data_16_a == 0x0200 )
   {
      ihdl->wav.channel_num = 2;
      ihdl->wav.is_stereo = TRUE;
      ihdl->mh.mono2stereo = FALSE;
   } 
   else
   {
      return FALSE;
   }

   // numSampleFrames
   if( wavSkipNBytes(ihdl, 4) == FALSE )
      return FALSE;

   if( wavGetWord( ihdl, &data_16_a) == FALSE )
      return FALSE;
   _bytes_reverse_16( data_16_a, ihdl->wav.data_size );
   ihdl->wav.data_size >>= 3;
   if( ihdl->wav.data_size != 1 && ihdl->wav.data_size != 2 )
      return FALSE;   // Support only 8/16 bit data

   if( wavGetWord( ihdl, &data_16_a ) == FALSE )
      return FALSE;
   if( wavGetWord( ihdl, &data_16_b ) == FALSE )
      return FALSE;
   _bytes_reverse_16( data_16_a, data_16_a );
   _bytes_reverse_16( data_16_b, data_16_b );
   data_16_a -= 16383;
   data_16_b >>= ( 15 - data_16_a );
   ihdl->wav.sampleRate = data_16_b;

   if( FALSE == getSampleRateIdx( ihdl, (UINT32)data_16_b ) )
      return 0; // The sample rate is not supported by ASP.

   if( wavSkipNBytes(ihdl, 6) == FALSE )
      return FALSE;
   ihdl->wav.word_left_in_form -= 9;
   ihdl->wav.word_left_in_ck -= 9;
   if( ihdl->wav.aiff_type == IS_AIFF)
   {
      if( ihdl->wav.data_size == 1 )
	  {
         ihdl->wav.BitsPerSample = 8;
      }
	  else
	  {
         ihdl->wav.BitsPerSample = 16;
      }
      ihdl->wav.formatTag = 0x0001;
   }
   else
   {
      if( wavGetDword( ihdl, &data_32 ) == FALSE )
         return FALSE;
      _bytes_reverse_32( data_32, data_32 );
      switch( data_32 )
      {
         case 0x4e4f4e45:  // "NONE"
            ihdl->wav.formatTag = 0x0001;
            if( ihdl->wav.data_size == 1 )
			{
               ihdl->wav.BitsPerSample = 8;
            }
			else
			{
               ihdl->wav.BitsPerSample = 16;
            }
            break;

         case 0x756c6177:  // "ulaw"
         case 0x554c4157:  // "ULAW"
            ihdl->wav.formatTag     = 0x0007;
            ihdl->wav.BitsPerSample = 8;
            ihdl->wav.data_size     = 1;
            break;
         case 0x616c6177:  // "alaw"
         case 0x414c4157:  // "ALAW"
            ihdl->wav.formatTag     = 0x0006;
            ihdl->wav.BitsPerSample = 8;
            ihdl->wav.data_size     = 1;
            break;
         default:
            return FALSE;
      }
      ihdl->wav.word_left_in_form -= 2;
      ihdl->wav.word_left_in_ck -= 2;
   }
   ihdl->wav.ssnd_state = 0;
   return TRUE;
}

#endif

static BOOL auReadHeader( wavMediaHdl *ihdl )
{
   UINT32 data_32;

   // file magic, 0x646e732e for ASCII "dns."
   if( wavGetDword( ihdl, &data_32 ) == FALSE || data_32 != 0x646e732e )
      return FALSE;

   // data offset
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;
   _bytes_reverse_32( data_32, ihdl->wav.data_offset );

   // data length
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;
   _bytes_reverse_32( data_32, ihdl->wav.data_length );

   // format id
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;

   switch( data_32 )
   {
      case 0x01000000:
         ihdl->wav.formatTag = 0x0007;
         ihdl->wav.BitsPerSample = 8;
         break;
      case 0x1b000000:
         ihdl->wav.formatTag = 0x0006;
         ihdl->wav.BitsPerSample = 8;
         break;
      case 0x02000000:
         ihdl->wav.formatTag = 0x0001;
         ihdl->wav.BitsPerSample = 8;
         break;
      case 0x03000000:
         ihdl->wav.formatTag = 0x0001;
         ihdl->wav.BitsPerSample = 16;
         break;
      default:
         return FALSE;
   }

   // sample rate
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;
   _bytes_reverse_32( data_32, data_32 );
   ihdl->wav.sampleRate = data_32;

   if( FALSE == getSampleRateIdx( ihdl, data_32 ) )
      return FALSE; // The sample rate is not supported by ASP.

   // channel count
   if( wavGetDword( ihdl, &data_32 ) == FALSE )
      return FALSE;

   if( data_32 == 0x01000000 ) 
   {
      ihdl->wav.channel_num = 1;
      ihdl->wav.is_stereo = FALSE;
      ihdl->mh.mono2stereo = TRUE;
   } 
   else if( data_32 == 0x02000000 ) 
   {
      ihdl->wav.channel_num = 2;
      ihdl->wav.is_stereo = TRUE;
      ihdl->mh.mono2stereo = FALSE;
   }
   else
   {
      return FALSE;
   }

   if( ihdl->wav.data_offset < 24 )      return FALSE;

   ihdl->fileOffset = ihdl->wav.data_offset;
   return TRUE;
}


Media_Status wavCommonInit(STFSAL *pstFSAL, audInfoStruct *contentInfo, MHdl **retHdl)
{
   MHdl *hdl;
   wavMediaHdl *ihdl= (wavMediaHdl*)COS_Malloc( sizeof(wavMediaHdl) );
   if(ihdl)
        memset(ihdl, 0, sizeof(wavMediaHdl));
   else
        return MEDIA_FAIL;
   
   hdl = (MHdl *) ihdl;

   ihdl->pstFSAL = pstFSAL;
   hdl->rb_base  = (UINT8 *)&ihdl->buf[0];
   hdl->rb_size    = WAV_MAX_DATA_REQUESTED;

   {  /* Fill Media Buffer to parse file header */
      INT32 num_bytes= autGetBytesFromFile(hdl, ihdl->pstFSAL, ihdl->fileOffset, WAV_MAX_DATA_REQUESTED, (UINT8 *)&ihdl->buf[0]);

	  if(num_bytes < 0)
	  {
         COS_Free( ihdl );
         return MEDIA_READ_FAIL;
      }
      hdl->write += num_bytes;
   }
   *retHdl = (MHdl *) ihdl;
   return MEDIA_SUCCESS;
}

Media_Status WAV_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param )
{
   MHdl *hdl;
   wavMediaHdl *ihdl;
   Media_Status mResult;

   if( MEDIA_SUCCESS != (mResult = wavCommonInit(pstFSAL, contentInfo, &hdl)))     return mResult;

   ihdl = (wavMediaHdl *)hdl;
   /* parse file header */
   mResult = wavReadHeader(ihdl);
   if (mResult != MEDIA_SUCCESS)
   {
      COS_Free( ihdl );
      return mResult;
   }

   if ( wavReachDataChunk(ihdl) == FALSE )
   { // reach data offset, "ihdl->fileOffset"
      COS_Free( ihdl );
      return MEDIA_BAD_FORMAT;
   }

   contentInfo->bitRate         = ihdl->wav.AvgBytesPerSec * 8;
   contentInfo->stereo          = ihdl->wav.is_stereo;
   contentInfo->sampleRate = ihdl->wav.sampleRate;

   {
      UINT32 timeData = 0, timeFile = 0;
      
      timeData = (UINT64)ihdl->wav.wDataLength * 1000 / (2*ihdl->wav.channel_num * ihdl->wav.sampleRate);
            
      if (pstFSAL->uFileSize>= ihdl->fileOffset) 
	  {
         pstFSAL->uFileSize -= ihdl->fileOffset;
      }
	  else
	  {
         pstFSAL->uFileSize = 0;
      }
      if(contentInfo->bitRate)     timeFile = (UINT64)pstFSAL->uFileSize * 8000 / (UINT64)contentInfo->bitRate; // in ms
      
      if (timeData < timeFile)
         contentInfo->time = timeData;
      else
         contentInfo->time = timeFile;
   }

   COS_Free( ihdl );
   return MEDIA_SUCCESS;
}

static void getDataCommon(wavMediaHdl *ihdl, audInfoStruct *contentInfo)
{
    contentInfo->stereo         = ihdl->wav.is_stereo;
    contentInfo->sampleRate = ihdl->wav.sampleRate;
    contentInfo->bitRate        = ihdl->wav.sampleRate * ( ihdl->wav.is_stereo + 1 ) * ihdl->wav.BitsPerSample;
    
    ihdl->pstFSAL->uFileSize-= ihdl->wav.data_offset;
    
    if (contentInfo->bitRate)   contentInfo->time = (UINT64)ihdl->pstFSAL->uFileSize * 8000 / (UINT64)contentInfo->bitRate; // in ms

    COS_Free( ihdl );
}


#if defined( AIFF_DECODE )
Media_Status AIFF_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param )
{
   MHdl *hdl;
   wavMediaHdl *ihdl;
   Media_Status mResult;

   if( MEDIA_SUCCESS != (mResult = wavCommonInit(pstFSAL, contentInfo, &hdl)))     return mResult;

   ihdl = (wavMediaHdl *)hdl;

   if( !aiffReadHeader(ihdl) )
   {   /* parse file header */
      COS_Free( ihdl );
      return MEDIA_BAD_FORMAT;
   }

   if ( aiffLookForSSND(ihdl) != 2 )
   { // reach data offset, "ihdl->fileOffset"
      COS_Free( ihdl );
      return MEDIA_BAD_FORMAT;
   }

   getDataCommon(ihdl, contentInfo);

   return MEDIA_SUCCESS;
}
#endif

#if defined( AU_DECODE )
Media_Status AU_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param )
{
   MHdl *hdl;
   wavMediaHdl *ihdl;
   Media_Status mResult;

   if( MEDIA_SUCCESS != (mResult = wavCommonInit(pstFSAL, contentInfo, &hdl)))      return mResult;

   ihdl = (wavMediaHdl *)hdl;

   if( !auReadHeader(ihdl) )
   {
      COS_Free( ihdl );
      return MEDIA_BAD_FORMAT;
   }

   getDataCommon(ihdl, contentInfo);

   return MEDIA_SUCCESS;
}
#endif

Media_Status PCM_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param )
{
   Media_Format eFormat;
   contentInfo->sampleRate = 8000;

   eFormat = (Media_Format) ((UINT32)param);

   switch (eFormat) 
   {
      case MEDIA_FORMAT_DVI_ADPCM:
         contentInfo->time      = (pstFSAL->uFileSize>> 2);
         contentInfo->bitRate = 32000;
         break;
      case MEDIA_FORMAT_G711_ALAW:
      case MEDIA_FORMAT_G711_ULAW:
         contentInfo->time      = (pstFSAL->uFileSize >> 3);
         contentInfo->bitRate = 64000;
         break;
      case MEDIA_FORMAT_PCM_8K:
         contentInfo->time      = (pstFSAL->uFileSize >> 4);
         contentInfo->bitRate = 128000;
         break;
      case MEDIA_FORMAT_PCM_16K:
         contentInfo->sampleRate = 16000;
         contentInfo->time             = (pstFSAL->uFileSize >> 5);
         contentInfo->bitRate        = 256000;
      default:
         APP_ASSERT(0);
   }

   return MEDIA_SUCCESS;
}

#endif //#if defined(WAV_CODEC)



