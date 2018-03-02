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

#ifdef AMR_CODEC


static const UINT16 AMR_BitRate[]={4750, 5150, 5900, 6700, 7400, 7950, 10200, 12200};
#if defined(AMRWB_DECODE)
static const UINT16 AMRWB_BitRate[]={6600, 8850, 12650, 14250, 15850, 18250, 19850, 23050, 23850};
#endif

const UINT8 AWB_frame_length[10] = { 17, 23, 32, 36, 40, 46, 50, 58, 60, 5 };
const UINT8 AMR_frame_length[9]  = { 12, 13, 15, 17, 19, 20, 26, 31, 5 };


#define MAX_DATA_REQUESTED    1024


typedef struct mh_internal {
   MHdl              mh;
   STFSAL            *pstFSAL;
   audInfoStruct     *description;
   UINT32        fileOffset;
   UINT32        uFileSize;
   UINT8         buf[MAX_DATA_REQUESTED];
} amrMediaHdl;


static BOOL amrChkFileHeader( amrMediaHdl *ihdl )
{
   UINT8 AMR_Header[9];
   /* check amr-wb file header */
   if(autGetBytesFromFile((MHdl*)ihdl, ihdl->pstFSAL, ihdl->fileOffset, 9, AMR_Header) != 9)
   {
      return FALSE;
   }
#if defined(AMRWB_DECODE)
   if (memcmp(AMR_Header, "#!AMR-WB\n", 9)==0)
   {
      ihdl->mh.mediaType = MEDIA_FORMAT_AMR_WB;
      ihdl->fileOffset += 9;
   }
   else
#endif
   {
      if(memcmp(AMR_Header, "#!AMR\n", 6)!=0)
      {
         return FALSE;
  	  }
      ihdl->mh.mediaType = MEDIA_FORMAT_AMR;
      ihdl->fileOffset += 6;
   }
   return TRUE;
}


Media_Status AMR_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  )
{
   UINT8   AMR_Header = 0x7C;
   amrMediaHdl * ihdl = (amrMediaHdl*)COS_Malloc( sizeof(amrMediaHdl) );
   if(ihdl)
        memset(ihdl, 0, sizeof(amrMediaHdl));
   else
        return MEDIA_FAIL;
  
   ihdl->pstFSAL   = pstFSAL;
   ihdl->description=contentInfo;   

   if(amrChkFileHeader( ihdl ) == FALSE)
   {
      COS_Free(ihdl);
      return MEDIA_BAD_FORMAT;
   }
   /*Read AMR Header*/
   {
      INT32 cnt = 0;
      while(AMR_Header == 0x7C)
	  { // No Data Frame
         if(autGetBytesFromFile((MHdl*)ihdl, ihdl->pstFSAL, ihdl->fileOffset+cnt, 1, &AMR_Header) != 1)
		 {
            COS_Free(ihdl);
            return MEDIA_BAD_FORMAT;
         }
         cnt++;
      }
   }
   /*Get AMR File Info*/
   {
      UINT8  FT = (AMR_Header >> 3) & 0x0F;
#if defined(AMRWB_DECODE)
      if (MEDIA_FORMAT_AMR_WB == ihdl->mh.mediaType ) 
	  { /* AMR-WB */
         contentInfo->sampleRate = 16000;
         if(FT <= 8)
		 {
            contentInfo->bitRate = AMRWB_BitRate[FT];
            contentInfo->time      = (UINT64)(pstFSAL->uFileSize- 9) / (AWB_frame_length[FT]+1) * 20;
         }
      }
	  else 
#endif
       if (MEDIA_FORMAT_AMR == ihdl->mh.mediaType ) 
	   { /* AMR */
          contentInfo->sampleRate = 8000;
          if(FT <= 7)
		  {
             contentInfo->bitRate = AMR_BitRate[FT];
             contentInfo->time = (UINT64)(pstFSAL->uFileSize- 6) / (AMR_frame_length[FT]+1) * 20;
          }
       }
   }
   COS_Free( ihdl );

   return MEDIA_SUCCESS;
}

#endif   // #if defined(AMR_CODEC)



