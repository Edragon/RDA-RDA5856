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


#define VM_LOG_DEBUG
#define CACHE_ENABLE


#define LoadDWORD( dw, p )   (dw) = (*(p+3)<<24) | (*(p+2)<<16) | (*(p+1)<<8) |(*p);  (p) += sizeof(UINT32);
#define MAX_DATA_REQUESTED    1024


/* ------------------------------------------------------------------------------ */

typedef struct vm_internal{
   MHdl           mh;
   STFSAL      *pstFSAL;
   UINT8          buf[MAX_DATA_REQUESTED];
   UINT16        control;    /* control UINT16 for FR/HR/EFR                         */
                              /* bit  0-1:   Channel usage(bit0:UL,bit1:DL)         */
                              /* bit  2-5:   UL Speech mode(0->FR, 1->HR, 2->EFR)   */
                              /* bit  6-9:   DL Speech mode(0->FR, 1->HR, 2->EFR)   */
                              /* bit  10:    DP_TX_TCH_S0_0 data bit 1              */
                              /* bit 11-15:  DP_RX_TCH_S0_0 data bit 1-5            */

                              /* control UINT16 for AMR                               */
                              /* bit  0-1:   Channel usage(bit0:UL,bit1:DL)         */
                              /* bit  2-5:   UL Speech mode(0->FR, 1->HR, 2->EFR)   */
                              /* bit  6-9:   DL Speech mode(0->FR, 1->HR, 2->EFR)   */
                              /* bit 10-11:  DP_TX_TCH_S0_0 data bit 0-1            */
                              /* bit 12-15:  DP_RX_TCH_S0_0 data bit 1-3,5          */
   UINT32         fileOffset;
   UINT32         headerOffset;
} vmMediaHdl;


static Media_Status vmGetFirstFrameContent(vmMediaHdl *ihdl, audInfoStruct *contentInfo, Media_Format eFormat)
{   
  MHdl *hdl = (MHdl *) ihdl;
  UINT32 file_size=ihdl->pstFSAL->uFileSize;  // in UINT8

   // VRD parser
   if (eFormat == MEDIA_FORMAT_VRD)
   {
      UINT32 dummy_32;
      INT32  get_len;
      UINT8 *pData;

      ihdl->headerOffset = 18;
      pData = &ihdl->buf[0];
      get_len = autGetBytes(hdl, ihdl->pstFSAL, 18, pData);
      ihdl->fileOffset += 18;
      if (get_len < 18) 
	  {
         return MEDIA_BAD_FORMAT;
      }

      LoadDWORD(dummy_32, pData);
      if ( (dummy_32 != 0x564B544D) && (dummy_32 != 0x56425943) )
         return MEDIA_BAD_FORMAT;

      pData += 6;
      LoadDWORD(dummy_32, pData);
      if (dummy_32 != 0x45434F56)
         return MEDIA_BAD_FORMAT;

      LoadDWORD(file_size, pData);
   }

   // Parse the first frame
   {
      UINT16 control, sc_mode, sd_mode, data_len, debug_data_len = 0;
      UINT8  *pBuf;

      data_len = 0;
      pBuf = &ihdl->buf[0];

      while( 1 ) 
	  {
         if( eFormat != MEDIA_FORMAT_VRD && eFormat != MEDIA_FORMAT_VRSI) 
		 {
#if defined( VM_LOG_DEBUG )
            data_len = 3;
            do 
			{
               if ( autGetBytes(hdl, ihdl->pstFSAL, 2, pBuf) != 2)
                  return MEDIA_BAD_FORMAT;

               control = *(UINT16 *)pBuf;
            } while( control != 0xAA55 );
            
            if ( autGetBytes(hdl, ihdl->pstFSAL, 2, pBuf) != 2)//format
               return MEDIA_BAD_FORMAT;
            debug_data_len = *(UINT16 *)pBuf >> 6;
            
            if ( autGetBytes(hdl, ihdl->pstFSAL, 4, pBuf) != 4)
               return MEDIA_BAD_FORMAT;
#endif
            if ( autGetBytes(hdl, ihdl->pstFSAL, 2, pBuf) != 2)
               return MEDIA_BAD_FORMAT;

            control = *(UINT16 *)pBuf;

            if( (control&3) == 0 )
               continue;
         }
         else
         {
            control = 0x401;
         }
         
#define VM_CTRL_SC_START   2
#define VM_CTRL_SD_START   6       

         sc_mode = (control >> VM_CTRL_SC_START) & 0x0F;
         sd_mode = (control >> VM_CTRL_SD_START) & 0x0F;
         
         if( sc_mode != sd_mode && (sc_mode <= 2 || sd_mode <= 2) )            continue;

         break;
      }

#if defined( VM_LOG_DEBUG )
      if( debug_data_len > 0 )
         data_len += ( debug_data_len << 1 );
#endif

      contentInfo->bitRate = data_len * 800;

      if (data_len)
         contentInfo->time = 20 * (file_size / data_len);
   }
   return MEDIA_SUCCESS;
}

Media_Status VM_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param )
{
   vmMediaHdl *ihdl;
   MHdl *hdl;
   Media_Format eFormat;
   Media_Status ret = MEDIA_SUCCESS;

   ihdl = (vmMediaHdl*)COS_Malloc( sizeof(vmMediaHdl) );
   if(ihdl)
         memset(ihdl, 0, sizeof(vmMediaHdl));
   else
         return MEDIA_FAIL;
   
   hdl = (MHdl *) ihdl;

   ihdl->pstFSAL = pstFSAL;
   
   eFormat = *(Media_Format *) param;

   contentInfo->sampleRate = 8000;

   ret = vmGetFirstFrameContent(ihdl, contentInfo, eFormat);

   COS_Free( ihdl );

   return ret;
}



