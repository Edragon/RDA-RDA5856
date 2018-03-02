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

#ifdef DAF_DECODE

#define MUSICAM_DECODE
#define XING_SUPPORT

#define MIN_FRAME_SIZE          24
#define MAX_FRAME_SIZE         1442
#define SEARCH_LEN             (MAX_FRAME_SIZE-MIN_FRAME_SIZE)

#define MAX_FRAME_CHECK_LEN    (8*1024) // MAX search length of Frame Header Sync.

#define DAF_MAX_DATA_REQUESTED 1024


#ifdef XING_SUPPORT  

#define FRAMES_FLAG              0x0001
#define BYTES_FLAG               0x0002
#define TOC_FLAG                 0x0004
#define VBR_SCALE_FLAG           0x0008

#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

// structure to receive extracted header, toc may be NULL
typedef struct {
    INT32   h_id;       // from MPEG header, 0=MPEG2, 1=MPEG1
    INT32   samprate;   // determined from MPEG header
    INT32   flags;      // from Xing header data
    INT32   frames;     // total bit stream frames from Xing header data
    INT32   bytes;      // total bit stream bytes from Xing header data
    INT32   vbr_scale;  // encoded vbr scale from Xing header data
    BOOL   toc[100];  // pointer to unsigned INT8 toc_buffer[100]  // may be NULL if toc not desired
}   XHEADDATA;
#endif // #if defined XING_SUPPORT

typedef struct mh_internal {
   MHdl                mh;
   STFSAL          *pstFSAL;
   audInfoStruct   *description;
   UINT8         buf[DAF_MAX_DATA_REQUESTED];
   UINT32       fileOffset;
   UINT32       uSampleRate;
   UINT32       uMLayer;
   UINT32       uFirstFrameSize;
   UINT32       uFirstBR;
   UINT16       wHeader1;
   UINT16       wHeader2;
   UINT64       uSamplesPerFrame;
   UINT64       frameDur; // in ms * 10E-6
#ifdef XING_SUPPORT  

   XHEADDATA        stXing;
   BOOL                   fXing; // bit0: checked ? bit1: Xing Header exist ?
#endif // #if defined XING_SUPPORT
} dafMediaHdl;

#ifdef MUSICAM_DECODE
const UINT16 DAF_BIT_RATE_TABLE_L2_V1[15]={1, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384};
#endif // #ifdef MUSICAM_DECODE

const UINT16 DAF_BIT_RATE_TABLE_V1[15]={1, 32, 40, 48, 56, 64, 80, 96, 112,128, 160, 192, 224, 256, 320};
const UINT16 DAF_BIT_RATE_TABLE_V2[15]={1, 8, 16, 24, 32, 40, 48, 56, 64, 80,96, 112, 128, 144, 160};


static void dafUpdateInfo(dafMediaHdl *ihdl, UINT16 wFHdrW1, UINT16 wFHdrW2)
{
#ifdef MUSICAM_DECODE
   UINT16 layer = (wFHdrW1>>9) & 3;
#endif 
   UINT16  MPEGver      = (wFHdrW1>>11) & 3;         /* check MPEG audio version ID */
   UINT32  sample_rate  = (wFHdrW2>> 2) & 3;    /* sampling rate frequency index */

   /* check sampling rate */
   if( MPEGver == 3 && sample_rate == 0 )            /* MPEG1, 44100 */
      sample_rate = 44100;
   else if( MPEGver == 3 && sample_rate == 1 )    /* MPEG1, 48000 */
      sample_rate = 48000;
   else if( MPEGver == 3 && sample_rate == 2 )    /* MPEG1, 32000 */
      sample_rate = 32000;
   else if( MPEGver == 2 && sample_rate == 0 )    /* MPEG2, 22050 */
      sample_rate = 22050;
   else if( MPEGver == 2 && sample_rate == 1 )    /* MPEG2, 24000 */
      sample_rate = 24000;
   else if( MPEGver == 2 && sample_rate == 2 )    /* MPEG2, 16000 */
      sample_rate = 16000;
   else if( MPEGver == 0 && sample_rate == 0 )    /* MPEG2.5, 11025 */
      sample_rate = 11025;
   else if( MPEGver == 0 && sample_rate == 1 )    /* MPEG2.5, 12000 */
      sample_rate = 12000;
   else if( MPEGver == 0 && sample_rate == 2 )    /* MPEG2.5, 8000 */
      sample_rate = 8000;

   ihdl->uSampleRate = sample_rate;

#ifdef MUSICAM_DECODE
   if(layer == 1)// Layer III
   { 
#endif 
      /* check bit rate */
      if( MPEGver == 3 ) /* MPEG1 */           
         ihdl->uSamplesPerFrame = 1152;
      else /* MPEG2, MPEG2.5 */                
         ihdl->uSamplesPerFrame = 576;
      ihdl->uMLayer = 3;
#ifdef MUSICAM_DECODE
   }
   else if(layer == 2)// Layer II
   { 
       ihdl->uSamplesPerFrame = 1152;
       ihdl->uMLayer = 2;
   }
#endif 

   ihdl->frameDur = ihdl->uSamplesPerFrame * 1000000000 / (UINT64) ihdl->uSampleRate;
}

static UINT8   g_mci_buff[8*1024];

static BOOL dafReachNextFrame(dafMediaHdl *ihdl, UINT16 *wFHdrW1, UINT16 *wFHdrW2, UINT32 *uFrameSize)
{
   UINT32 uFrameOffset, uBR1, uBR2, uBR3, uBR4;
   INT32   uMaxCheckLen;
   UINT32 nextFrameOffset = 0;
   UINT8   *pbBuf;
   UINT8   *pbHeader;

// read bit stream
   pbBuf              = g_mci_buff;
   pbHeader           = pbBuf;

   uMaxCheckLen = autGetBytesFromFile((MHdl *)ihdl,ihdl->pstFSAL, ihdl->fileOffset, MAX_FRAME_CHECK_LEN,  pbBuf);

   if( uMaxCheckLen <= 4 ) 
   {
        return FALSE;
   }

   while(1) 
   {   // search the first MPEG header
      UINT16 w1, w2;
      UINT8  *pbHeaderTmp;

      if( nextFrameOffset > uMaxCheckLen ) 
      {
         return FALSE;
      }

/// (Step 1) Check to see whether the foremost 4-UINT8 is a valid MPEG header
      w1 = (UINT16)pbHeader[0] | ((UINT16)pbHeader[1] << 8);
      w2 = (UINT16)pbHeader[2] | ((UINT16)pbHeader[3] << 8);

#ifdef MUSICAM_DECODE
      if(((w1 & 0x1800) != 0x0800) && (((w1 & 0xE6FF) == 0xE2FF) || ((w1 & 0xE6FF) == 0xE4FF)) && ((w2 & 0x00F0) != 0x00F0) && ((w2 & 0x00F0) != 0x0) && ((w2 & 0x000C) != 0x000C)) 
#else
      if( ((w1 & 0x1800) != 0x0800) && ((w1 & 0xE6FF) == 0xE2FF) && ((w2 & 0x00F0) != 0x00F0) && ((w2 & 0x00F0) != 0x0) && ((w2 & 0x000C) != 0x000C)) 
#endif 
      {
         // 1st MPEG header is valid
         UINT16 MPEGver_1;
         INT32   sample_rate_1;
#ifdef MUSICAM_DECODE
         UINT16 layer = (w1>>9) & 3;
#endif 
         *wFHdrW1 = w1;
         *wFHdrW2  = w2;
         ihdl->wHeader1 = w1;
         ihdl->wHeader2 = w2;

         MPEGver_1      = (w1>>11) & 3;  /* check MPEG audio version ID */
         sample_rate_1 = (w2 >> 2) & 3;

         /* bit rate */
         if( MPEGver_1 == 3 )
         {                          /* MPEG1 */
            uBR1 = DAF_BIT_RATE_TABLE_V1[(w2 >> 4) & 0x0F] * 1000;
#ifdef MUSICAM_DECODE
         }
        else 
         {                                    /* MPEG2, MPEG2.5 */
             uBR1 = DAF_BIT_RATE_TABLE_V2[(w2 >> 4) & 0x0F] * 1000;
         }

         if( MPEGver_1 == 3 )
         {                          /* MPEG1 */
            if( layer == 1 )
            {// Layer III
                uBR1 = DAF_BIT_RATE_TABLE_V1[(w2 >> 4) & 0x0F] * 1000;
            }
            else 
            {
                uBR1 = DAF_BIT_RATE_TABLE_L2_V1[(w2 >> 4) & 0x0F] * 1000;
            }
#endif 
        }
        else 
        {                                    /* MPEG2, MPEG2.5 */
             uBR1 = DAF_BIT_RATE_TABLE_V2[(w2 >> 4) & 0x0F] * 1000;
        }

		pbHeaderTmp = pbHeader + MIN_FRAME_SIZE;
		uFrameOffset  = 0;

/// (Step 2) Skip this 4-UINT8 valid MPEG header, find the 2nd valid header by linear search method
		while( uFrameOffset <= SEARCH_LEN ) 
		{
			w1 = (UINT16)pbHeaderTmp[0] | ((UINT16)pbHeaderTmp[1] << 8);
			w2 = (UINT16)pbHeaderTmp[2] | ((UINT16)pbHeaderTmp[3] << 8);
#ifdef MUSICAM_DECODE
			if( ((w1 & 0x1800) != 0x0800) && (((w1 & 0xE6FF) == 0xE2FF) || ((w1 & 0xE6FF) == 0xE4FF)) && ((w2 & 0x00F0) != 0x00F0) && ((w2 & 0x00F0) != 0x0) && ((w2 & 0x000C) != 0x000C))
#else
			if( ((w1 & 0x1800) != 0x0800) && ((w1 & 0xE6FF) == 0xE2FF) && ((w2 & 0x00F0) != 0x00F0) && ((w2 & 0x00F0) != 0x0) && ((w2 & 0x000C) != 0x000C))
#endif 
			{
				// 2nd MPEG header is valid
				UINT32 uFsize1, uFsize2, uFsize3, uFsize4;
				UINT16 MPEGver;
				INT32  sample_rate;
				UINT32 uSearchFL;
#ifdef MUSICAM_DECODE
				UINT16 layer = (w1>>9) & 3;
#endif 

/// (Step 3) Count the frame size of the 1st frame in UINT8, named ¡§Fsize1¡¨

				uFsize1         = MIN_FRAME_SIZE + uFrameOffset;
				*uFrameSize     = uFsize1;
				ihdl->uFirstFrameSize = uFsize1;
				ihdl->uFirstBR        = uBR1/1000;

				/// (Step 4) Calculate frame size of the 2nd frame using the following formula
				MPEGver       =  (w1>>11) & 3;  /* check MPEG audio version ID */
				sample_rate   =  (w2 >> 2) & 3;

				// check sync. UINT16, layer, version and sample rate
				if( (MPEGver != MPEGver_1 ) || ( sample_rate != sample_rate_1 )  || ((ihdl->wHeader1&0xFEFF) != (w1&0xFEFF)) || ((ihdl->wHeader2 & 0x000C) != (w2 & 0x000C))) 
				{
					uFrameOffset++;
					pbHeaderTmp++;
					continue;
				}
				/* bit rate */
				if( MPEGver == 3 )
				{                          /* MPEG1 */
                  uBR2 = DAF_BIT_RATE_TABLE_V1[(w2 >> 4) & 0x0F] * 1000;
#ifdef MUSICAM_DECODE
				}
				else
				{                                    /* MPEG2, MPEG2.5 */
                   uBR2 = DAF_BIT_RATE_TABLE_V2[(w2 >> 4) & 0x0F] * 1000;
				}
				if( MPEGver == 3 ) 
				{                          /* MPEG1 */
                  if( layer == 1 ) 
                  {  // Layer III
                     uBR2 = DAF_BIT_RATE_TABLE_V1[(w2 >> 4) & 0x0F] * 1000;
                  }
                  else 
                  {
                      uBR2 = DAF_BIT_RATE_TABLE_L2_V1[(w2 >> 4) & 0x0F] * 1000;
                  }
#endif 
				}
				else
				{                                    /* MPEG2, MPEG2.5 */
                   uBR2 = DAF_BIT_RATE_TABLE_V2[(w2 >> 4) & 0x0F] * 1000;
				}

                uFsize2      = uFsize1 * uBR2 / uBR1;
                uSearchFL    = (uBR2 / uBR1) * 1 + 2;
                uFsize2      -= uSearchFL;

/// (Step 5) Check the 3rd MPEG header validation with "Fsize2"
               {  // Brace 1
				UINT16     w3rd1, w3rd2;
				UINT8      *pbHeader3rdTmp;
				UINT32    uOffset3rd = 0;
				UINT16    MPEGver3rd;
				INT32      sample_rate3rd;
				pbHeader3rdTmp = pbHeader + uFsize1 + uFsize2;

				while( uOffset3rd <= (uSearchFL * 2) ) 
				{
					w3rd1 = (UINT16)pbHeader3rdTmp[0] | ((UINT16)pbHeader3rdTmp[1] << 8);
					w3rd2 = (UINT16)pbHeader3rdTmp[2] | ((UINT16)pbHeader3rdTmp[3] << 8);
#ifdef MUSICAM_DECODE
					if( ((w3rd1 & 0x1800) != 0x0800) && (((w3rd1 & 0xE6FF) == 0xE2FF) || ((w3rd1 & 0xE6FF) == 0xE4FF))&& ((w3rd2 & 0x00F0) != 0x00F0) && ((w3rd2 & 0x00F0) != 0x0) && ((w3rd2 & 0x000C) != 0x000C))
#else
					if( ((w3rd1 & 0x1800) != 0x0800) && ((w3rd1 & 0xE6FF) == 0xE2FF)&& ((w3rd2 & 0x00F0) != 0x00F0) && ((w3rd2 & 0x00F0) != 0x0) && ((w3rd2 & 0x000C) != 0x000C)) 
#endif 
					{
                        // 3rd frame is valid
                        MPEGver3rd      = (w3rd1>>11) & 3;  /* check MPEG audio version ID */
                        sample_rate3rd = (w3rd2 >> 2) & 3;
                        if( (MPEGver3rd != MPEGver_1) || (sample_rate3rd != sample_rate_1)  || ((ihdl->wHeader1&0xFEFF) != (w3rd1&0xFEFF)) || ((ihdl->wHeader2 & 0x000C) != (w3rd2 & 0x000C))) 
                        {
                           uOffset3rd++;
                           pbHeaderTmp++;
                           continue;
                        }
                        /* bit rate */
                        if( MPEGver3rd == 3 ) 
                        {                          /* MPEG1 */
                           uBR3 = DAF_BIT_RATE_TABLE_V1[(w3rd2 >> 4) & 0x0F] * 1000;
#ifdef MUSICAM_DECODE
                        }
                        else
                        {                                    /* MPEG2, MPEG2.5 */
                            uBR3 = DAF_BIT_RATE_TABLE_V2[(w3rd2 >> 4) & 0x0F] * 1000;
                        }
                        if( MPEGver3rd == 3 ) 
                        {                          /* MPEG1 */
                           if( layer == 1 ) 
                           { // Layer III
                              uBR3 = DAF_BIT_RATE_TABLE_V1[(w3rd2 >> 4) & 0x0F] * 1000;
                           }
                           else
                           {
                               uBR3 = DAF_BIT_RATE_TABLE_L2_V1[(w3rd2 >> 4) & 0x0F] * 1000;
                           }
#endif 
                        }
                        else
                        {                                    /* MPEG2, MPEG2.5 */
                            uBR3 = DAF_BIT_RATE_TABLE_V2[(w3rd2 >> 4) & 0x0F] * 1000;
                        }

                        uFsize3      = (uFsize2+uOffset3rd) * uBR3 / uBR2;
                        uSearchFL = (uBR3 / uBR2) * 1 + 2;
                        uFsize3    -= uSearchFL;

                        {   // Brace 2
                           UINT16 w4rd1, w4rd2;
                           UINT8  *pbHeader4rdTmp;
                           UINT32 uOffset4rd = 0;
                           UINT16 MPEGver4rd;
                           INT32  sample_rate4rd;
                           pbHeader4rdTmp = pbHeader+uFsize1+uFsize2+uOffset3rd+uFsize3;

                           while( uOffset4rd <= (uSearchFL * 2) ) 
                           {
                              w4rd1 = (UINT16)pbHeader4rdTmp[0] | ((UINT16)pbHeader4rdTmp[1] << 8);
                              w4rd2 = (UINT16)pbHeader4rdTmp[2] | ((UINT16)pbHeader4rdTmp[3] << 8);
#ifdef MUSICAM_DECODE
                              if( ((w4rd1 & 0x1800) != 0x0800) && (((w4rd1 & 0xE6FF) == 0xE2FF) || ((w4rd1 & 0xE6FF) == 0xE4FF)) && ((w4rd2 & 0x00F0) != 0x00F0) && ((w4rd2 & 0x00F0) != 0x0) && ((w4rd2 & 0x000C) != 0x000C))
#else
                              if( ((w4rd1 & 0x1800) != 0x0800) && ((w4rd1 & 0xE6FF) == 0xE2FF) && ((w4rd2 & 0x00F0) != 0x00F0) && ((w4rd2 & 0x00F0) != 0x0) && ((w4rd2 & 0x000C) != 0x000C)) 
#endif 
                              {
                                 MPEGver4rd = (w4rd1>>11) & 3;  /* check MPEG audio version ID */
                                 sample_rate4rd = (w4rd2 >> 2) & 3;
                                 if( (MPEGver4rd != MPEGver_1) || (sample_rate4rd != sample_rate_1)  || ((ihdl->wHeader1&0xFEFF) != (w4rd1&0xFEFF)) || ((ihdl->wHeader2 & 0x000C) != (w4rd2 & 0x000C)))
                                 {
                                    uOffset4rd++;
                                    pbHeaderTmp++;
                                    continue;
                                 }

                                 /* bit rate */
                                 if( MPEGver4rd == 3 ) 
                                 {                          /* MPEG1 */
                                    uBR4 = DAF_BIT_RATE_TABLE_V1[(w4rd2 >> 4) & 0x0F] * 1000;
#ifdef MUSICAM_DECODE
                                 }
                                 else
                                 {                                    /* MPEG2, MPEG2.5 */
                                     uBR4 = DAF_BIT_RATE_TABLE_V2[(w4rd2 >> 4) & 0x0F] * 1000;
                                 }
                                 if( MPEGver4rd == 3 ) 
                                 {                          /* MPEG1 */
                                    if( layer == 1 ) 
                                    { // Layer III
                                       uBR4 = DAF_BIT_RATE_TABLE_V1[(w4rd2 >> 4) & 0x0F] * 1000;
                                    }
                                    else
                                    {
                                        uBR4 = DAF_BIT_RATE_TABLE_L2_V1[(w4rd2 >> 4) & 0x0F] * 1000;
                                    }
#endif 
                                 }
                                 else
                                 {                                    /* MPEG2, MPEG2.5 */
                                     uBR4 = DAF_BIT_RATE_TABLE_V2[(w4rd2 >> 4) & 0x0F] * 1000;
                                 }

                                 uFsize4      = (uFsize3+uOffset4rd) * uBR4 / uBR3;
                                 uSearchFL = (uBR4 / uBR3) * 1 + 2;
                                 uFsize4    -= uSearchFL;
                                 { // Brace 3
                                    UINT16  w5rd1, w5rd2;
                                    UINT8    *pbHeader5rdTmp;
                                    UINT32  uOffset5rd = 0;
                                    UINT16  MPEGver5rd;
                                    INT32    sample_rate5rd;
                                    pbHeader5rdTmp = pbHeader+uFsize1+uFsize2+uOffset3rd+uFsize3+uOffset4rd+uFsize4;

                                    while( uOffset5rd <= (uSearchFL * 2) ) 
                                    {
                                       w5rd1 = (UINT16)pbHeader5rdTmp[0] | ((UINT16)pbHeader5rdTmp[1] << 8);
                                       w5rd2 = (UINT16)pbHeader5rdTmp[2] | ((UINT16)pbHeader5rdTmp[3] << 8);
#ifdef MUSICAM_DECODE
                                       if( ((w5rd1 & 0x1800) != 0x0800) && (((w5rd1 & 0xE6FF) == 0xE2FF) || ((w5rd1 & 0xE6FF) == 0xE4FF)) && ((w5rd2 & 0x00F0) != 0x00F0) && ((w5rd2 & 0x00F0) != 0x0) && ((w5rd2 & 0x000C) != 0x000C))
#else
                                       if( ((w5rd1 & 0x1800) != 0x0800) && ((w5rd1 & 0xE6FF) == 0xE2FF) && ((w5rd2 & 0x00F0) != 0x00F0) && ((w5rd2 & 0x00F0) != 0x0) && ((w5rd2 & 0x000C) != 0x000C)) 
#endif 
                                       {
                                          MPEGver5rd      = (w5rd1>>11) & 3;  /* check MPEG audio version ID */
                                          sample_rate5rd = (w5rd2 >> 2) & 3;
                                          if( (MPEGver5rd != MPEGver_1) || (sample_rate5rd != sample_rate_1) || ((ihdl->wHeader1&0xFEFF) != (w5rd1&0xFEFF)) || ((ihdl->wHeader2 & 0x000C) != (w5rd2 & 0x000C))) 
                                          {
                                             uOffset5rd++;
                                             pbHeaderTmp++;
                                             continue;
                                          }
                                          return TRUE;
		                                } // if
                                        uOffset5rd++;
                                        pbHeader5rdTmp++;
                                    }   // while( uOffset4rd <= (uSearchFL * 2) ) {
                                 } // Brace 3
	                          }
	                          uOffset4rd++;
	                          pbHeader4rdTmp++;
        	                }   // while( uOffset4rd <= (uSearchFL * 2) ) {
        	            }   // Brace 2
                     }   // 3rd frame is valid
	                 uOffset3rd++;
	                 pbHeader3rdTmp++;
                  }   // while( uOffset3rd <= (uSearchFL * 2) ) {
               }   // Brace 1
            }   // 2nd MPEG header is valid
            uFrameOffset++;
            pbHeaderTmp++;
         }   // (Step 2) Skip this 4-UINT8 valid MPEG header, find the 2nd valid header by linear search method
      }   // Check to see whether the foremost 4-UINT8 is a valid MPEG header
      ihdl->fileOffset++;
      nextFrameOffset++;
      pbHeader++;
   }   // search the first MPEG header
   
}
            

static void dafGetInfo(UINT16 wFHdrW1, UINT16 wFHdrW2, audInfoStruct *contentInfo)
{
#ifdef MUSICAM_DECODE
   UINT16 layer        = (wFHdrW1>>9) & 3;
#endif 

   UINT16 MPEGver = (wFHdrW1>>11) & 3;         /* check MPEG audio version ID */
   INT32   srIdx         = (wFHdrW2 >> 2) & 3;          /* sampling rate frequency index */

   /* sampling rate */
   if( MPEGver == 3 && srIdx == 0 )         /* MPEG1, 44100 */
      contentInfo->sampleRate = 44100;
   else if( MPEGver == 3 && srIdx == 1 )    /* MPEG1, 48000 */
      contentInfo->sampleRate = 48000;
   else if( MPEGver == 3 && srIdx == 2 )    /* MPEG1, 32000 */
      contentInfo->sampleRate = 32000;
   else if( MPEGver == 2 && srIdx == 0 )    /* MPEG2, 22050 */
      contentInfo->sampleRate = 22050;
   else if( MPEGver == 2 && srIdx == 1 )    /* MPEG2, 24000 */
      contentInfo->sampleRate = 24000;
   else if( MPEGver == 2 && srIdx == 2 )    /* MPEG2, 16000 */
      contentInfo->sampleRate = 16000;
   else if( MPEGver == 0 && srIdx == 0 )    /* MPEG2.5, 11025 */
      contentInfo->sampleRate = 11025;
   else if( MPEGver == 0 && srIdx == 1 )    /* MPEG2.5, 12000 */
      contentInfo->sampleRate = 12000;
   else if( MPEGver == 0 && srIdx== 2 )    /* MPEG2.5, 8000 */
      contentInfo->sampleRate = 8000;

   /* bit rate */
   if( MPEGver == 3 )
   {                          /* MPEG1 */
      contentInfo->bitRate = DAF_BIT_RATE_TABLE_V1[(wFHdrW2 >> 4) & 0x0F] * 1000;
#ifdef MUSICAM_DECODE
   }
   else
   {                                    /* MPEG2, MPEG2.5 */
      contentInfo->bitRate = DAF_BIT_RATE_TABLE_V2[(wFHdrW2 >> 4) & 0x0F] * 1000;
   }

   /* bit rate */
   if( MPEGver == 3 )
   {                          /* MPEG1 */
      if(layer == 1) // Layer III
         contentInfo->bitRate = DAF_BIT_RATE_TABLE_V1[(wFHdrW2 >> 4) & 0x0F] * 1000;
      else
         contentInfo->bitRate = DAF_BIT_RATE_TABLE_L2_V1[(wFHdrW2 >> 4) & 0x0F] * 1000;
#endif 
   }
   else
   {                                    /* MPEG2, MPEG2.5 */
      contentInfo->bitRate = DAF_BIT_RATE_TABLE_V2[(wFHdrW2 >> 4) & 0x0F] * 1000;
   }

   //contentInfo->details.DAF.bit_rate_index = (UINT8)((wFHdrW2 >> 4) & 0x0F);
   //contentInfo->details.DAF.CRC                = (UINT8)((wFHdrW1>>8) & 1);
   //contentInfo->details.DAF.channel_mode = (UINT8)((wFHdrW2 >> 14) & 0x3);
   if(((wFHdrW2 >> 14) & 0x3) == 0x03) // stereo indicator
      contentInfo->stereo = FALSE;
   else
      contentInfo->stereo = TRUE;
}


#ifdef XING_SUPPORT  

static INT32 ExtractI4(UINT8 *buf)
{
   INT32 x;
   // big endian extract
   x = buf[0];
   x <<= 8;
   x |= buf[1];
   x <<= 8;
   x |= buf[2];
   x <<= 8;
   x |= buf[3];

   return x;
}

static INT32 GetXingHeader(XHEADDATA *X,  UINT8 *buf)
{
   INT32 i, head_flags;
   INT32 h_id, h_mode, h_sr_index;
   static INT32 sr_table[4] = { 44100, 48000, 32000, 99999 };

   // get Xing header data

   X->flags = 0;     // clear to null incase fail

   // get selected MPEG header data
   h_id           = (buf[1] >> 3) & 1;
   h_sr_index = (buf[2] >> 2) & 3;
   h_mode     = (buf[3] >> 6) & 3;

   // determine offset of header
   if( h_id ) 
   {        // mpeg1
       if( h_mode != 3 ) buf+=(32+4);
       else              buf+=(17+4);
   }
   else 
   {      // mpeg2
       if( h_mode != 3 ) buf+=(17+4);
       else              buf+=(9+4);
   }

   if( buf[0] != 'X' ) return 0;    // fail
   if( buf[1] != 'i' ) return 0;    // header not found
   if( buf[2] != 'n' ) return 0;
   if( buf[3] != 'g' ) return 0;
   buf+=4;

   X->h_id = h_id;
   X->samprate = sr_table[h_sr_index];
   if( h_id == 0 ) X->samprate >>= 1;

   head_flags = X->flags = ExtractI4(buf); buf+=4;      // get flags

   if( head_flags & FRAMES_FLAG ) {X->frames   = ExtractI4(buf); buf+=4;}
   if( head_flags & BYTES_FLAG )  {X->bytes = ExtractI4(buf); buf+=4;}

   if( head_flags & TOC_FLAG )
   {
       if( X->toc != NULL ) 
       {
           for(i=0;i<100;i++) X->toc[i] = buf[i];
       }
       buf+=100;
   }
   
   X->vbr_scale = -1;
   if( head_flags & VBR_SCALE_FLAG )  {X->vbr_scale = ExtractI4(buf); buf+=4;}
   
   return 1;       // success
}
#endif // #if defined XING_SUPPORT


Media_Status DAF_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  )
{
    UINT16 headerFstWd, headerSndWd;
    UINT32 uFrameSize;

    dafMediaHdl *ihdl = (dafMediaHdl*)COS_Malloc( sizeof(dafMediaHdl) );
    
	if(ihdl)
		memset(ihdl, 0, sizeof(dafMediaHdl));
	else
		return MEDIA_FAIL;

    if(!(pstFSAL&&contentInfo))  return MEDIA_FAIL;
    
    ihdl->pstFSAL    =pstFSAL;
    ihdl->description=contentInfo;

    autSkipID3Tag((MHdl *) ihdl, ihdl->pstFSAL, &ihdl->fileOffset, &ihdl->buf[0] );

    if ( dafReachNextFrame(ihdl, &headerFstWd, &headerSndWd, &uFrameSize) == FALSE )
    {
        COS_Free( ihdl );
        return MEDIA_BAD_FORMAT;
    }

    dafUpdateInfo(ihdl, headerFstWd, headerSndWd);
#ifdef XING_SUPPORT   
    if(autGetBytesFromFile((MHdl *)ihdl,ihdl->pstFSAL, ihdl->fileOffset, 512, &ihdl->buf[0]) == 512)
    {
        ihdl->fXing = GetXingHeader(&ihdl->stXing,  &ihdl->buf[0]);
    }
#endif // #if defined XING_SUPPORT
   dafGetInfo(headerFstWd, headerSndWd, contentInfo);
#ifdef XING_SUPPORT 
   if(ihdl->fXing && ihdl->stXing.frames)
   {
      if((contentInfo->sampleRate == 44100) || (contentInfo->sampleRate == 48000) || (contentInfo->sampleRate == 32000))
         contentInfo->time = (INT64)ihdl->stXing.frames * 1152000 / contentInfo->sampleRate;
      else
         contentInfo->time = (INT64)ihdl->stXing.frames * 576000 / contentInfo->sampleRate;
   }
   else
#endif // #if defined XING_SUPPORT       
    {
         if(contentInfo->bitRate)     contentInfo->time = (UINT64)pstFSAL->uFileSize* 8000 / (UINT64)contentInfo->bitRate;
    }

   COS_Free( ihdl );

   return MEDIA_SUCCESS;
}

#endif /* DAF_DECODE */

