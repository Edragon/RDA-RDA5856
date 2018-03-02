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

#if 1//defined(WMA_DECODE)

#define WMA_RB_REQ_THR                200

#define WAVE_FORMAT_MSAUDIO1   0x0160
#define WAVE_FORMAT_WMAUDIO2  0x0161

#define MIN_OBJECT_SIZE                       24
#define DATA_OBJECT_SIZE                     50
#define WMA_MAX_DATA_REQUESTED  1024

typedef INT32 WMAERR;

#define WMAERR_OK                          0
#define WMAERR_FAIL                        1
#define WMAERR_INVALIDHEADER    2
#define WMAERR_CORRUPTDATA      4
#define WMAERR_UNSUPPORT           5
#define WMA_PARSE_ENCRYPTION    0

typedef struct tqword
{
   UINT32   dwLo;
   UINT32   dwHi;
}  qword;

typedef struct tPayloadParseInfo
{
   UINT32 msObjectPres;
   UINT32 cbParseOffset;
   UINT16 cbPayloadSize;
   UINT16 cbTotalSize;
   UINT16 wBytesRead;
   UINT16 wSubpayloadLeft;

   UINT8 bStreamId;
   UINT8 cbRepData;
   
   UINT8 bSubPayloadState;
   UINT8 bIsCompressedPayload;
   UINT8 bNextSubPayloadSize;
   BOOL  fInvalidFrame;
}  payloadParseInfo;

typedef struct tPacketParseInfo
{
   UINT32 cbParseOffset;
   UINT32 cbExplicitPacketLength; 
   UINT32 cbPadding;
   UINT32 dwSCR;
   UINT32 cPayloads;
   
   BOOL fParityPacket;
   BOOL fMultiPayloads;
   BOOL fEccPresent;
   UINT8 bECLen;

   UINT8 bPacketLenType;
   UINT8 bPadLenType;
   UINT8 bSequenceLenType;
   UINT8 bOffsetLenType;
   UINT8 bPayLenType;
}  packetParseInfo;

// internal structure for the ASF header parsing
typedef struct tFileHeaderParseInfo
{
   UINT32 currPacketOffset;     // not just for packets
   UINT32 nextPacketOffset;

   // ASF header
   UINT32   cbHeader;
   UINT32   cbPacketSize;
   UINT32   cPackets;
   UINT32   msDuration;
   UINT32   cbLastPacketOffset;

   // audio prop
   UINT32   nSamplesPerSec;
   UINT32   nAvgBytesPerSec;
   UINT32   nBlockAlign;
   UINT16   wAudioStreamId;
   UINT16   nVersion;
   UINT16   nChannels;
   UINT16   nEncodeOpt;

#if WMA_PARSE_ENCRYPTION
   // DRM
   UINT32   cbSecretData;
   UINT8    pbSecretData[32];
   UINT8    pbType[16];
   UINT8    pbKeyID[32];
   UINT8    pbURL[256];
#endif
} fileHeaderParseInfo;

/* parser state  */
typedef enum
{
   csWMA_NewAsfPacket = 0,
   csWMA_DecodePayloadStart,
   csWMA_DecodePayload,
   csWMA_DecodePayloadHeader,
   csWMA_DecodeLoopStart,
   csWMA_DecodePayloadEnd,
   csWMA_DecodeCompressedPayload,
   csWMA_End
} asfParseState;

typedef struct tagAsfFileStateInternal
{
   fileHeaderParseInfo hdr_parse;

   // parse_state   
   asfParseState parse_state;

   // payload parsing
   UINT32 cbPayloadOffset;
   UINT32 cbPayloadLeft;
   UINT32 cbBlockLeft;
   UINT32 iPayload;
   BOOL  bBlockStart;

   // packet parsing
   packetParseInfo ppex;
   payloadParseInfo payload;
} asfFileStateInternal;


typedef struct media_handle_internal {
   MHdl                       mh;      /* this should be the first line in this structure */
   STFSAL                  *pstFSAL;
   UINT32                     fileSize;
   asfFileStateInternal       sState;
   audInfoStruct              *description;
   UINT8                         pDataBuffer[WMA_MAX_DATA_REQUESTED];
} wmaMediaHandleInternal;

/*typedef struct tGUID
{
   UINT32   Data1;
   UINT16   Data2;
   UINT16   Data3;
   UINT8     Data4[ 8 ];
} GUID;*/

#define GetUnalignedWord( pb, w )    (w) = *(UINT16*)(pb); 
#define GetUnalignedDword( pb, dw ) (dw) = *(UINT32*)(pb);
#define GetUnalignedQword( pb, qw ) (qw) = *(qword*)(pb);

#define GetUnalignedWordEx( pb, w )     GetUnalignedWord( pb, w );   (pb) += sizeof(UINT16);
#define GetUnalignedDwordEx( pb, dw )   GetUnalignedDword( pb, dw ); (pb) += sizeof(UINT32);
#define GetUnalignedQwordEx( pb, qw )   GetUnalignedQword( pb, qw ); (pb) += sizeof(qword);

#define LoadBYTE( b, p )        b = *(UINT8 *)(p);  (p) += sizeof(UINT8);
#define LoadWORD( w, p )      (w) = *(UINT16*)(p);  (p) += sizeof(UINT16);
#define LoadDWORD( dw, p )   (dw) = *(UINT32*)(p);  (p) += sizeof(UINT32);
#define LoadQWORD( qw, p )   (qw) = *(qword *)(p);  (p) += sizeof(qword);

#define LoadGUID( g, p ) \
        { \
            LoadDWORD( (g).Data1, p ); \
            LoadWORD( (g).Data2, p ); \
            LoadWORD( (g).Data3, p ); \
            LoadBYTE( (g).Data4[0], p ); \
            LoadBYTE( (g).Data4[1], p ); \
            LoadBYTE( (g).Data4[2], p ); \
            LoadBYTE( (g).Data4[3], p ); \
            LoadBYTE( (g).Data4[4], p ); \
            LoadBYTE( (g).Data4[5], p ); \
            LoadBYTE( (g).Data4[6], p ); \
            LoadBYTE( (g).Data4[7], p ); \
        }

#define WMA_IsEqualGUID(rguid1, rguid2)   (!memcmp((void *)rguid1, (void *)rguid2, sizeof(GUID)))

/***************************************************************************/


typedef struct tAsfXAcmAudioErrorMaskingData
{
   UINT8     span;
   UINT16    virtualPacketLen;
   UINT16    virtualChunkLen;
   UINT16    silenceLen;
   UINT8     silence[1];
} AsfXAcmAudioErrorMaskingData;

typedef struct tAsfXSignatureAudioErrorMaskingData
{
   UINT32  maxObjectSize;
   UINT16  chunkLen;
   UINT16  signatureLen;
   UINT8   signature[1];
} AsfXSignatureAudioErrorMaskingData;


static const GUID CLSID_CAsfHeaderObjectV0 = {0x75b22630,0x668e,0x11cf, {0xa6,0xd9,0x00,0xaa,0x00,0x62,0xce,0x6c}};
static const GUID CLSID_CAsfPropertiesObjectV2 = {0x8cabdca1, 0xa947, 0x11cf, {0x8e, 0xe4, 0x0, 0xc0, 0xc, 0x20, 0x53, 0x65}};
static const GUID CLSID_CAsfStreamPropertiesObjectV1 = {0xb7dc0791, 0xa9b7, 0x11cf, {0x8e, 0xe6, 0x0, 0xc0, 0xc, 0x20, 0x53, 0x65}};
static const GUID CLSID_CAsfContentDescriptionObjectV0 = {0x75b22633,0x668e,0x11cf,{0xa6,0xd9,0x00,0xaa,0x00,0x62,0xce,0x6c}};
static const GUID CLSID_CAsfExtendedContentDescObject = {0xd2d0a440, 0xe307, 0x11d2, {0x97, 0xf0, 0x0, 0xa0, 0xc9, 0x5e, 0xa8, 0x50}};
static const GUID CLSID_AsfXStreamTypeAcmAudio = {0xf8699e40,0x5b4d,0x11cf,{0xa8,0xfd,0x00,0x80,0x5f,0x5c,0x44,0x2b}};
static const GUID CLSID_AsfXSignatureAudioErrorMaskingStrategy = {0x49f1a440, 0x4ece, 0x11d0, {0xa3, 0xac, 0x0, 0xa0, 0xc9, 0x3, 0x48, 0xf6}};
static const GUID CLSID_AsfXAcmAudioErrorMaskingStrategy = {0xbfc3cd50,0x618f,0x11cf,{0x8b,0xb2,0x00,0xaa,0x00,0xb4,0xe2,0x20}};
static const GUID CLSID_CAsfStreamPropertiesObjectV2 = {0x818d0d47, 0x43, 0x4451, {0x92, 0x37, 0xda, 0xef, 0xbc, 0x48, 0x4f, 0x50}};
static const GUID CLSID_CAsfContentEncryptionObject = {0x2211b3fb, 0xbd23, 0x11d2, {0xb4, 0xb7, 0x00, 0xa0, 0xc9, 0x55, 0xfc, 0x6e}};

static const UINT8 Asf_AlbumTitle[28] = {
   0x57, 0x00, 0x4D, 0x00, 0x2F, 0x00, 0x41, 0x00, 0x6C, 0x00,
   0x62, 0x00, 0x75, 0x00, 0x6D, 0x00, 0x54, 0x00, 0x69, 0x00,
   0x74, 0x00, 0x6C, 0x00, 0x65, 0x00};


static const UINT8 SampleRateTable[] = {  // WMA L2 support
   0x00, /*8000*/   0x11,/* 11025 */ 0x12,/* 12000 */
   0x20, /*16000,*/ 0x21,/* 22050,*/ 0x22,/* 24000,*/
   0x40, /*32000,*/ 0x41,/* 44100,*/ 0x42 /* 48000 */
};

static UINT32 asf_HnsQWORDtoMsDWORD (qword qw)
{
   UINT32 msLo, msHi;

   msLo = (qw.dwHi*7296 + qw.dwLo)/10000;
   msHi =  qw.dwHi*429496;

   return msLo + msHi;
}

static WMAERR asf_LoadHeaderObject(wmaMediaHandleInternal *ihdl)
{
   GUID  objectId;
   qword qwSize;
   UINT32 cHeaders;
   UINT8  align, arch;

   UINT32 cbWanted;
   UINT8 *pData;
   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;

   pInt = &ihdl->sState.hdr_parse;
   cbWanted = MIN_OBJECT_SIZE + sizeof(UINT32) + 2*sizeof(UINT8);

   if (ihdl->fileSize < cbWanted)
      return WMAERR_INVALIDHEADER;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   pInt->currPacketOffset += cbWanted;

   LoadGUID(objectId, pData);
   LoadQWORD(qwSize, pData);
   LoadDWORD(cHeaders, pData);
   LoadBYTE(align, pData);
   LoadBYTE(arch, pData);

   if (!WMA_IsEqualGUID(&CLSID_CAsfHeaderObjectV0, &objectId) || align != 1 || arch != 2)
      return WMAERR_INVALIDHEADER;

   pInt->cbHeader = qwSize.dwLo;

   return WMAERR_OK;
}

static WMAERR asf_LoadObjectHeader(wmaMediaHandleInternal *ihdl, GUID *pObjectId, qword *pqwSize)
{
   UINT8 *pData;
   UINT32 cbWanted;
   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;

   pInt = &ihdl->sState.hdr_parse;

   cbWanted = MIN_OBJECT_SIZE;
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   pInt->currPacketOffset += cbWanted;

   LoadGUID(*pObjectId, pData);
   LoadQWORD(*pqwSize, pData);

   return WMAERR_OK;
}

static WMAERR asf_LoadPropertiesObject(wmaMediaHandleInternal *ihdl, UINT32 cbSize)
{
   qword qwPackets, qwPlayDuration, qwPreroll;
   UINT32 dwMinPacketSize, dwMaxPacketSize;

   UINT8  *pData;
   UINT32 cbWanted;
   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;

   pInt = &ihdl->sState.hdr_parse;
   cbSize -= MIN_OBJECT_SIZE;

   cbWanted = sizeof(GUID) + 6*sizeof(qword) + 4*sizeof(UINT32);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];

   // skip File ID, File Size and Creation Date
   pData += sizeof(GUID) + 2*sizeof(qword);
   LoadQWORD(qwPackets, pData);
   // Play Duration
   LoadQWORD(qwPlayDuration, pData);
   // skip send duration
   pData += sizeof(qword);
   // preroll
   LoadQWORD(qwPreroll, pData);
   // skip Flags
   pData += sizeof(UINT32);
   LoadDWORD(dwMinPacketSize, pData);
   LoadDWORD(dwMaxPacketSize, pData);
   // skip Max Bitrate

   if ( (dwMinPacketSize != dwMaxPacketSize) ||
        (dwMinPacketSize == 0)               ||
        (qwPackets.dwLo == 0 && qwPackets.dwHi == 0) )
       return WMAERR_FAIL;

   pInt->cbPacketSize = dwMaxPacketSize;
   pInt->cPackets     = qwPackets.dwLo;
   pInt->msDuration   = asf_HnsQWORDtoMsDWORD(qwPlayDuration) - qwPreroll.dwLo;

   pInt->currPacketOffset += cbSize;

   return WMAERR_OK;
}

static WMAERR asf_LoadAudioObject(wmaMediaHandleInternal *ihdl, UINT32 cbSize)
{
   GUID streamType, ecStrategy;
   UINT32 cbTypeSpecific, cbErrConcealment;
   UINT16 wStreamNum, wFormat;

   UINT8  *pData;
   UINT32 cbWanted;
   fileHeaderParseInfo *pInt;
   AsfXAcmAudioErrorMaskingData *pScramblingData;
   UINT32 cbObjectOffset = 0;
   MHdl *hdl = (MHdl *) ihdl;

   pInt = &ihdl->sState.hdr_parse;
   cbSize -= MIN_OBJECT_SIZE;

   cbWanted = 2*sizeof(GUID) + sizeof(qword) + 3*sizeof(UINT32) + sizeof(UINT16);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadGUID(streamType, pData);
   LoadGUID(ecStrategy, pData);
   // skip Time Offset
   pData += sizeof(qword);
   LoadDWORD(cbTypeSpecific, pData);
   LoadDWORD(cbErrConcealment, pData);
   LoadWORD(wStreamNum, pData);

   wStreamNum &= 0x7F;

   if ( !WMA_IsEqualGUID( &CLSID_AsfXStreamTypeAcmAudio, &streamType ) )
   {
      pInt->currPacketOffset += cbSize;   // Skip over the rest
      return WMAERR_OK;
   }

   if (wStreamNum==0)                     // Type specific
      return WMAERR_FAIL;

   pInt->wAudioStreamId = wStreamNum;     // Record the first audio stream

   if (cbTypeSpecific > 0)
   {
      cbWanted = cbTypeSpecific;
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      cbObjectOffset += cbWanted;

      LoadWORD (wFormat ,pData);
      switch (wFormat)
      {
         case WAVE_FORMAT_WMAUDIO2:
            if (cbTypeSpecific < 28)  //sizeof(WMAUDIO2WAVEFORMAT)
                return WMAERR_FAIL;

            pInt->nVersion         = 2;
            LoadWORD (pInt->nChannels, pData);
            LoadDWORD(pInt->nSamplesPerSec, pData);
            LoadDWORD(pInt->nAvgBytesPerSec, pData);
            LoadWORD (pInt->nBlockAlign, pData);
            // skip Bit Per Sample, Codec Specific Data Size, and Sample pre block
            pData += 2*sizeof(UINT16) + sizeof(UINT32);
            LoadWORD (pInt->nEncodeOpt, pData);
            break;

         case WAVE_FORMAT_MSAUDIO1:
            if (cbTypeSpecific < 22)  //sizeof(MSAUDIO1WAVEFORMAT)
                return WMAERR_FAIL;

            pInt->nVersion         = 1;
            LoadWORD (pInt->nChannels, pData);
            LoadDWORD(pInt->nSamplesPerSec, pData);
            LoadDWORD(pInt->nAvgBytesPerSec, pData);
            LoadWORD (pInt->nBlockAlign, pData);
            // skip Bit Per Sample, Codec Specific Data Size, and Sample pre block
            pData += 2*sizeof(UINT16) + sizeof(UINT32);
            LoadWORD (pInt->nEncodeOpt, pData);
            break;

         default:  // unknown...
            return WMAERR_UNSUPPORT;
      }
   }

   if (pInt->nBlockAlign == 0)
      return WMAERR_FAIL;

   if (cbErrConcealment > 0)
   {
      if (WMA_IsEqualGUID(&CLSID_AsfXSignatureAudioErrorMaskingStrategy, &ecStrategy)) 
      {
         cbWanted = sizeof(AsfXSignatureAudioErrorMaskingData);
      } 
      else if (WMA_IsEqualGUID(&CLSID_AsfXAcmAudioErrorMaskingStrategy, &ecStrategy)) 
      {
         cbWanted = 8;
      }
      else 
      {
         return WMAERR_FAIL;
      }

      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      cbObjectOffset += cbErrConcealment;

      if (WMA_IsEqualGUID(&CLSID_AsfXAcmAudioErrorMaskingStrategy, &ecStrategy)) 
      {
         pScramblingData = (AsfXAcmAudioErrorMaskingData *)pData;

         if (pScramblingData->span >1)
            return WMAERR_FAIL;
      } 
      else if ( !WMA_IsEqualGUID(&CLSID_AsfXSignatureAudioErrorMaskingStrategy, &ecStrategy)) 
      {
         return WMAERR_FAIL;
      }
   }

   pInt->currPacketOffset += cbSize;

   return WMAERR_OK;
}

static void asf_LoadContentDescriptionObject(wmaMediaHandleInternal *ihdl, UINT32 cbSize)
{
   UINT16 cbCDTitle, cbCDAuthor, cbCDCopyright, cbCDDescription, cbCDRating;
   UINT8  *pData;
   UINT32 cbWanted;
   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;
   UINT32 cbObjectOffset = 0;

   pInt = &ihdl->sState.hdr_parse;
   cbSize -= MIN_OBJECT_SIZE;

   cbWanted = 5*sizeof(UINT16);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadWORD(cbCDTitle, pData);
   LoadWORD(cbCDAuthor, pData);
   LoadWORD(cbCDCopyright, pData);
   LoadWORD(cbCDDescription, pData);
   LoadWORD(cbCDRating, pData);

   if (cbCDTitle > MAX_DESCRIPTION)
      cbWanted = MAX_DESCRIPTION;
   else
      cbWanted = cbCDTitle;
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, (UINT8 *)&ihdl->description->title[0]);
   cbObjectOffset += cbCDTitle;

   if (cbCDAuthor > MAX_DESCRIPTION)
      cbWanted = MAX_DESCRIPTION;
   else
      cbWanted = cbCDAuthor;
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, (UINT8 *)&ihdl->description->author[0]);
   cbObjectOffset += cbCDAuthor;

/*   if (cbCDCopyright > MAX_DESCRIPTION)
      cbWanted = MAX_DESCRIPTION;
   else
      cbWanted = cbCDAuthor;
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, (UINT8 *)&ihdl->description->copyright[0]);
   cbObjectOffset += cbCDCopyright;*/

   pInt->currPacketOffset += cbSize;
}

static void asf_LoadExtendedContentDescriptionObject(wmaMediaHandleInternal *ihdl, UINT32 cbSize)
{
   UINT16 numDescriptor;
   UINT8  *pData;
   UINT32 cbWanted;
   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;
   UINT32 cbObjectOffset = 0;
   INT32 i;
   BOOL isAlbumTitle = FALSE;

   pInt = &ihdl->sState.hdr_parse;
   cbSize -= MIN_OBJECT_SIZE;

   // Obtain the Contene Description Count
   cbWanted = 1*sizeof(UINT16);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;
   LoadWORD(numDescriptor, pData);

   // Parse each Content Descriptor
   for (i=numDescriptor-1; i>=0; i--) 
   {
      UINT16 nameLen, dataType, valueLen;;
      
      // Get Descriptor Name Length
      cbWanted = 1*sizeof(UINT16);
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset,  cbWanted, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      cbObjectOffset += cbWanted;
      LoadWORD(nameLen, pData);
      
      // Get Descriptor Name
      if (nameLen > WMA_MAX_DATA_REQUESTED)
         cbWanted = WMA_MAX_DATA_REQUESTED;
      else
         cbWanted = nameLen;
      
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
      cbObjectOffset += nameLen;
      
      // Check whether it is VM/AlbumTitle
      if (nameLen == 28) 
      {
         if (0 == memcmp(ihdl->pDataBuffer, Asf_AlbumTitle, 28)) 
         {
            isAlbumTitle = TRUE;
         }
      }
      
      // Get Descriptor Value Data Type, and Descriptor Value Length
      cbWanted = 2*sizeof(UINT16);
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      cbObjectOffset += cbWanted;
      LoadWORD(dataType, pData);
      LoadWORD(valueLen, pData);
      
      if (isAlbumTitle) 
      {
         if (valueLen > MAX_DESCRIPTION)
            cbWanted = MAX_DESCRIPTION;
         else
            cbWanted = valueLen;
         
         autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, (UINT8 *)&ihdl->description->album[0]);
         ihdl->description->album[cbWanted] = '\0';
         break;
      }
      cbObjectOffset += valueLen;
   }

   pInt->currPacketOffset += cbSize;
}

#if WMA_PARSE_ENCRYPTION
static WMAERR asf_LoadEncryptionObject(wmaMediaHandleInternal *ihdl, UINT32 cbSize)
{
   UINT32 cbWanted, cbLength;
   UINT8 *pData;

   fileHeaderParseInfo *pInt;
   MHdl *hdl = (MHdl *) ihdl;
   UINT32 cbObjectOffset = 0;

   pInt = &ihdl->sState.hdr_parse;
   cbSize -= MIN_OBJECT_SIZE;

   pInt->cbSecretData = 0;

   // Secret Data
   cbWanted = sizeof(UINT32);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadDWORD(cbWanted, pData);
   pInt->cbSecretData = cbWanted;
   if (cbWanted) 
   {
      if (cbObjectOffset + cbWanted > cbSize)
         return WMAERR_FAIL;
      
      cbLength = cbWanted;
      if (cbWanted > 32)
         cbWanted = 32;
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &pInt->pbSecretData[0]);
      cbObjectOffset += cbLength;
      pInt->pbSecretData[cbWanted] = '\0';
   }

   // Type string
   cbWanted = sizeof(UINT32);
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadDWORD(cbWanted, pData);
   if (cbWanted) 
   {
      if (cbObjectOffset + cbWanted > cbSize)
         return WMAERR_FAIL;

      cbLength = cbWanted;
      if (cbWanted > 16)
         cbWanted = 16;
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &pInt->pbType[0]);
      cbObjectOffset += cbLength;
      pInt->pbType[cbWanted] = '\0';
   }

   // Key ID
   cbWanted = sizeof(UINT32);
   if (cbObjectOffset + cbWanted > cbSize)
      return WMAERR_FAIL;;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadDWORD(cbWanted, pData);
   if (cbWanted) 
   {
      if (cbObjectOffset + cbWanted > cbSize)
         return WMAERR_FAIL;

      cbLength = cbWanted;
      if (cbWanted > 32)
         cbWanted = 32;
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &pInt->pbKeyID[0]);
      cbObjectOffset += cbLength;
      pInt->pbKeyID[cbWanted] = '\0';
   }

   // License URL
   cbWanted = sizeof(UINT32);
   if (cbObjectOffset + cbWanted > cbSize)
      return WMAERR_FAIL;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   cbObjectOffset += cbWanted;

   LoadDWORD(cbWanted, pData);

   if (cbWanted) 
   {
      if (cbObjectOffset + cbWanted > cbSize)
         return WMAERR_FAIL;
      
      cbLength = cbWanted;
      if (cbWanted > 256)
         cbWanted = 256;
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->currPacketOffset + cbObjectOffset, cbWanted, &pInt->pbURL[0]);
      cbObjectOffset += cbLength;
      pInt->pbURL[cbWanted] = '\0';

      // Ignore others
   }

   pInt->currPacketOffset += cbSize;

   return WMAERR_OK;
}
#endif

static WMAERR asf_ParseAsfHeader(wmaMediaHandleInternal *ihdl)
{
   WMAERR wmarc;
   GUID   objId;
   qword  qwSize;
   fileHeaderParseInfo *pInt;
   UINT8  fObject = 0;

   pInt = &ihdl->sState.hdr_parse;

   // Initialize current packet offset and Audio Stream ID
   pInt->currPacketOffset = 0;
   pInt->wAudioStreamId = 0;

   // ASF Header Object
   wmarc = asf_LoadHeaderObject(ihdl);
   if (wmarc != WMAERR_OK)
      return wmarc;

   pInt->cbHeader += DATA_OBJECT_SIZE;

   // Error: File too small
   if (ihdl->fileSize < pInt->cbHeader)
      return WMAERR_INVALIDHEADER;

   // Scan Header Objects
   while (pInt->currPacketOffset < pInt->cbHeader)
   {
      wmarc = asf_LoadObjectHeader(ihdl, &objId, &qwSize);
      if (wmarc != WMAERR_OK)
         return wmarc;

      if (WMA_IsEqualGUID(&CLSID_CAsfPropertiesObjectV2, &objId)) 
      {
         wmarc = asf_LoadPropertiesObject(ihdl, qwSize.dwLo);
         if (wmarc != WMAERR_OK)
            return wmarc;
         fObject |= 0x01;
      } 
      else if ( WMA_IsEqualGUID( &CLSID_CAsfStreamPropertiesObjectV1, &objId ) || WMA_IsEqualGUID( &CLSID_CAsfStreamPropertiesObjectV2, &objId ) ) 
      {
          if ( pInt->wAudioStreamId == 0  )
          {
             wmarc = asf_LoadAudioObject(ihdl, qwSize.dwLo);
             if (wmarc != WMAERR_OK)
                return wmarc;
             fObject |= 0x02;
          } 
          else 
          {
              pInt->currPacketOffset += qwSize.dwLo - MIN_OBJECT_SIZE;
          }
      } 
      else if ( WMA_IsEqualGUID(&CLSID_CAsfContentDescriptionObjectV0, &objId)) 
      {
      	 if (ihdl->description)
      	    asf_LoadContentDescriptionObject(ihdl, qwSize.dwLo);
      	 else
      	    pInt->currPacketOffset += qwSize.dwLo - MIN_OBJECT_SIZE;
      } 
      else if ( WMA_IsEqualGUID(&CLSID_CAsfExtendedContentDescObject, &objId)) 
      {
      	 if (ihdl->description)
      	    asf_LoadExtendedContentDescriptionObject(ihdl, qwSize.dwLo);
      	 else
      	    pInt->currPacketOffset += qwSize.dwLo - MIN_OBJECT_SIZE;
      } 
      else if ( WMA_IsEqualGUID(&CLSID_CAsfContentEncryptionObject, &objId)) 
      {
#if WMA_PARSE_ENCRYPTION
         wmarc = asf_LoadEncryptionObject(ihdl, qwSize.dwLo);
         if (wmarc != WMAERR_OK)
            return wmarc;
#else
         return WMAERR_UNSUPPORT;
#endif
      } 
      else 
      {
          // skip over this object
          pInt->currPacketOffset += qwSize.dwLo - MIN_OBJECT_SIZE;
      }
   }

   pInt->cbLastPacketOffset = pInt->currPacketOffset;

   if (fObject != 0x03)
      return WMAERR_FAIL;

   return WMAERR_OK;
}

static UINT32 asf_GetDataByType(wmaMediaHandleInternal *ihdl, UINT8 LenType, UINT32 Position, UINT32 *Len) 
{
   UINT8  *pData;
   UINT16 w;
   UINT32 dwValue, dw;
   MHdl *hdl = (MHdl *) ihdl;

   switch(LenType)
   {
      case 0x01:
         *Len =1;
         autGetBytesFromFile(hdl, ihdl->pstFSAL, Position, 1, &ihdl->pDataBuffer[0]);
         pData = &ihdl->pDataBuffer[0];
         dwValue =  (UINT32)pData[0];
         break;
      case 0x02:
         *Len =2;
         autGetBytesFromFile(hdl, ihdl->pstFSAL, Position, 2, &ihdl->pDataBuffer[0]);
         pData = &ihdl->pDataBuffer[0];
         GetUnalignedWord(pData, w);
         dwValue =  (UINT32)(w);
         break;
      case 0x03:
         *Len =4;
         autGetBytesFromFile(hdl, ihdl->pstFSAL, Position, 4, &ihdl->pDataBuffer[0]);
         pData = &ihdl->pDataBuffer[0];
         GetUnalignedDword(pData, dw);
         dwValue =  dw;
         break;
      default:
         dwValue = 0;
         *Len =0;
   }

   return dwValue;
}

static WMAERR asf_ParsePacketHeader(wmaMediaHandleInternal *ihdl)
{
   UINT8 b;
   UINT8  *pData;
   UINT32 cbWanted;
   packetParseInfo *pParseInfoEx;
   asfFileStateInternal *pInt;
   MHdl *hdl = (MHdl *) ihdl;

   pInt = &ihdl->sState;
   pParseInfoEx = &pInt->ppex;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset, 1, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];

   // ParsePacket begins
   pParseInfoEx->fParityPacket = FALSE;
   pParseInfoEx->cbParseOffset = 1;
   b = pData[0];

   pParseInfoEx->fEccPresent = (BOOL)( (b&0x80) == 0x80 );
   pParseInfoEx->bECLen = 0;

   //To check if error correction present, 1 means exist
   if (pParseInfoEx->fEccPresent)
   {
      // Opaque Data Present
      if (b&0x10)
      {
         pParseInfoEx->fParityPacket = TRUE;
         return WMAERR_OK;
      }

      // Error Correction Length Type (00, other for future)
      if (b&0x60)
         return WMAERR_FAIL;

      // Error Correction Data Length (should 0010)
      pParseInfoEx->bECLen = (b&0x0f);
      if (pParseInfoEx->bECLen != 2)
         return WMAERR_FAIL;
      pParseInfoEx->cbParseOffset += (UINT32) pParseInfoEx->bECLen;

      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, 1, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      b = pData[0];
      pParseInfoEx->cbParseOffset++;
   }

   // KH: should loose the constraint
   pParseInfoEx->bPacketLenType = (b&0x60)>>5;
   if (pParseInfoEx->bPacketLenType != 0 && pParseInfoEx->bPacketLenType != 2)
      return WMAERR_FAIL;

   // KH: should loose the constraint
   pParseInfoEx->bPadLenType = (b&0x18)>>3;
   if (pParseInfoEx->bPadLenType == 3)
      return WMAERR_FAIL;

   pParseInfoEx->bSequenceLenType = (b&0x06)>>1;
   pParseInfoEx->fMultiPayloads = (BOOL)(b&0x01);

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, 1, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   b = pData[0];
   pParseInfoEx->cbParseOffset++;

   pParseInfoEx->bOffsetLenType = 3;
   if (b != 0x5d)
   {
      // Stream Number Length Type shall be 01
      if ((b&0xc0) != 0x40)
         return WMAERR_FAIL;

      // Media Object Number Length Type shall be 01
      if ((b&0x30) != 0x10)
         return WMAERR_FAIL;

      // KH: Offset should be 11 (should loose the constraint)
      pParseInfoEx->bOffsetLenType = (b&0x0c)>>2;
      if (pParseInfoEx->bOffsetLenType == 0)
         return WMAERR_FAIL;

      if ((b&0x03) != 0x01)
         return WMAERR_FAIL;
   }

   pParseInfoEx->cbExplicitPacketLength = asf_GetDataByType(ihdl, pParseInfoEx->bPacketLenType, pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, &cbWanted);
   pParseInfoEx->cbParseOffset += cbWanted;

   asf_GetDataByType(ihdl, pParseInfoEx->bSequenceLenType,  pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, &cbWanted);
   pParseInfoEx->cbParseOffset += cbWanted;

   pParseInfoEx->cbPadding = asf_GetDataByType(ihdl, pParseInfoEx->bPadLenType,   pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, &cbWanted);
   pParseInfoEx->cbParseOffset += cbWanted;

   // Send time(4 bytes) and duration(2 bytes)
   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, 6, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   pParseInfoEx->cbParseOffset += 6;

   GetUnalignedDword(&pData[0], pParseInfoEx->dwSCR);

   // skip duration

   // ParsePacketEx begins
   pParseInfoEx->bPayLenType = 0;
   pParseInfoEx->cPayloads = 1;

   if (pParseInfoEx->fMultiPayloads)
   {
      autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pParseInfoEx->cbParseOffset, 1, &ihdl->pDataBuffer[0]);
      pData = &ihdl->pDataBuffer[0];
      pParseInfoEx->cbParseOffset ++;

      b = pData[0];

      //KH: must not be 0 (we should accept 11)
      pParseInfoEx->bPayLenType = (b&0xc0)>>6;
      if (pParseInfoEx->bPayLenType != 2 && pParseInfoEx->bPayLenType != 1)
         return WMAERR_FAIL;

      pParseInfoEx->cPayloads = (UINT32)(b&0x3f);
      if (pParseInfoEx->cPayloads == 0)
         return WMAERR_FAIL;
   }

   return WMAERR_OK;
}

static WMAERR asf_ParsePayloadHeader(wmaMediaHandleInternal *ihdl)
{
   UINT32 dwPayloadSize;
   asfFileStateInternal *pInt;
   packetParseInfo *pParseInfoEx;
   payloadParseInfo *pPayload;
   MHdl *hdl = (MHdl *) ihdl;

   UINT8  *pData;
   UINT32 cbWanted;

   pInt = &ihdl->sState;
   pParseInfoEx = &pInt->ppex;
   pPayload = &pInt->payload;

   pPayload->cbParseOffset = pParseInfoEx->cbParseOffset;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pPayload->cbParseOffset, 2, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   pPayload->cbParseOffset += 2;

   pPayload->bStreamId = (pData[0])&0x7f;

   asf_GetDataByType(ihdl, pParseInfoEx->bOffsetLenType, pInt->hdr_parse.currPacketOffset + pPayload->cbParseOffset, &cbWanted);
   pPayload->cbParseOffset += cbWanted;

   autGetBytesFromFile(hdl, ihdl->pstFSAL, pInt->hdr_parse.currPacketOffset + pPayload->cbParseOffset,1, &ihdl->pDataBuffer[0]);
   pData = &ihdl->pDataBuffer[0];
   pPayload->cbParseOffset++;

   pPayload->cbRepData = pData[0];
   pPayload->cbParseOffset += pPayload->cbRepData;

   if (pPayload->cbRepData == 1)
      pPayload->bIsCompressedPayload = 1;
   else
      pPayload->bIsCompressedPayload =0;

   if (pParseInfoEx->fMultiPayloads)
   {
      dwPayloadSize = asf_GetDataByType(ihdl, pParseInfoEx->bPayLenType,  pInt->hdr_parse.currPacketOffset + pPayload->cbParseOffset, &cbWanted);
      pPayload->cbParseOffset += cbWanted;
   } 
   else if (pParseInfoEx->cbExplicitPacketLength > 0) 
   {
      dwPayloadSize = pParseInfoEx->cbExplicitPacketLength - pPayload->cbParseOffset  - pParseInfoEx->cbPadding;
   } 
   else 
   {
      dwPayloadSize = pInt->hdr_parse.cbPacketSize - pPayload->cbParseOffset  - pParseInfoEx->cbPadding;
   }

   pPayload->cbPayloadSize = (UINT16)dwPayloadSize;
   pParseInfoEx->cbParseOffset = pPayload->cbParseOffset + pPayload->cbPayloadSize;

   if (pParseInfoEx->cbParseOffset > pInt->hdr_parse.cbPacketSize || (pParseInfoEx->cbParseOffset == pInt->hdr_parse.cbPacketSize
          && pInt->iPayload < pParseInfoEx->cPayloads - 1))
   {
      return WMAERR_CORRUPTDATA;
   }
   return WMAERR_OK;
}



/*
 * Description
 * -----------
 *  Retrieves the content description for the WMA file.
 *  Content descriptions are such as the song title, the
 *  author, the copyright info, the song description,
 *  and the rating info. All these are part of the
 *  standard ASF description.
 *  Not all songs have content descriptions.
 *  It can be called at any time.
 *
 * Syntax
 * ------------
 *   Media_Status WMA_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo);
 *
 * where:
 *
 *   pstFSAL           Pointer to the FSAL structure.
 *   contentInfo       Pointer to audio information structure.
 *
 * Return Value
 * ------------
 * Media_Status    Return MEDIA_SUCCESS if the operation is complete. Return MEDIA_FAIL otherwise.
 */

Media_Status WMA_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo, void *param  )
{
   wmaMediaHandleInternal *ihdl = (wmaMediaHandleInternal*)COS_Malloc( sizeof(wmaMediaHandleInternal) );
   if(ihdl)
        memset(ihdl, 0, sizeof(wmaMediaHandleInternal));
   else
        return MEDIA_FAIL;

   ihdl->pstFSAL     = pstFSAL;
   ihdl->description = contentInfo;
   ihdl->fileSize       =ihdl->pstFSAL->uFileSize;
   
   asf_ParseAsfHeader(ihdl);
   
   ihdl->description->time             = ihdl->sState.hdr_parse.msDuration;   // duration in milli-sec
   ihdl->description->bitRate        = ihdl->sState.hdr_parse.nAvgBytesPerSec << 3;
   ihdl->description->sampleRate = ihdl->sState.hdr_parse.nSamplesPerSec;
   ihdl->description->stereo          = (ihdl->sState.hdr_parse.nChannels ==  2)?TRUE:FALSE;

   COS_Free( ihdl );
   return MEDIA_SUCCESS;
}

#endif  /* WMA_DECODE */

