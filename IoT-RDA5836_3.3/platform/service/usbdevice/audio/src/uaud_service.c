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
////////////////////////////////////////////////////////////////////////////////
//
/// @file uaud_service.c
///
/// Transport Bulk Only function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "csw.h"
#include "uaud_callback.h"
#include "hal_host.h"

//typedef HAL_USB_CALLBACK_EP_TYPE_T MsgBody_t;
//#include "itf_msg.h"

#include "hal_usb.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "uctls_m.h"

#include <string.h>

//#include "uaudp_debug.h"
#include "event.h"

#ifdef USB_AUDIO_SUPPORT

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
// =============================================================================
#define UAUD_WARN_TRC  0 // SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define UAUD_INFO_TRC   0 //SVC_UMSS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define UAUD_DBG_TRC    0 //SVC_DBG_TRC


#ifndef UAUD_NO_PRINTF
  #define UAUD_TRACE(level, tsmap, format, ...)   CSW_TRACE(level,format, ##__VA_ARGS__)
#else
  #define UAUD_TRACE(level, tsmap, fmt, ...)
#endif // UAUD_NO_PRINTF

// =============================================================================
// MACROS                                                                       
// =============================================================================

#define CBW_SIGNATURE 0x43425355
#define CSW_SIGNATURE 0x53425355

#define USB_AUD_SET_CUR 0x01
#define USB_AUD_GET_CUR 0x81
#define USB_AUD_GET_MIN 0x82
#define USB_AUD_GET_MAX 0x83
#define USB_AUD_GET_RES 0x84

#define USB_AUD_ISO_OUT_BUF_SIZE (576)
#define USB_AUD_CMD_DATA_BUF_SIZE (64)
extern PUBLIC HAL_USB_CS_INTERFACE_DESCRIPTOR_T*
uctls_NewCSInterfaceDescriptor(HAL_USB_CS_CONSTRUCTOR_T constructor,
                                          HAL_USB_CALLBACK_T       callback);

// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================
UINT8                        g_uaudIsoOutEp;
UINT8                        g_uaudIsoInEp;

PRIVATE UINT8                         *g_UsbAudIsoOutBuf;//[USB_AUD_ISO_OUT_BUF_SIZE];
PRIVATE UINT8                         *g_UsbAudCmdDataBuf;//[USB_AUD_CMD_DATA_BUF_SIZE];

PRIVATE UINT8                         g_UsbKeycode;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT16 bcdADC;
  UINT16 TotalLength;
  UINT8 InCollection;
  UINT8 InterfaceNr1;
  UINT8 InterfaceNr2;
  
}__attribute__((packed))AUD_CTL_IF_HEADER_DES;

typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 TerminalID;
  UINT16 TerminalType;
  UINT8 AssocTerminal;
  UINT8 NrChannals;
  UINT16 ChannalCofig;
  UINT8 ChannalName;
  UINT8 TermimalString;
  
}__attribute__((packed))AUD_CTL_IN_TRM_DES;

typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 TerminalID;
  UINT16 TerminalType;
  UINT8 AssocTerminal;
  UINT8 SourceID;
  UINT8 TerminalString;
  
}__attribute__((packed))AUD_CTL_OUT_TRM_DES;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 UnitID;
  UINT8 SourceID;
  UINT8 ControlSize;
  UINT8 Controls[3];
  UINT8 FeatrueString;
  
}__attribute__((packed))AUD_CTL_FTR_UNIT_DES_1;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 UnitID;
  UINT8 SourceID;
  UINT8 ControlSize;
  UINT8 Controls[2];
  UINT8 FeatrueString;
  
}__attribute__((packed))AUD_CTL_FTR_UNIT_DES_2;

typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 TerminalLink;
  UINT8 Delay;
  UINT16 FormatTag;
  
}__attribute__((packed))AUD_STR_IF_DES;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 FormatType;
  UINT8 NrOfChannel;
  UINT8 SubFrameSize;
  UINT8 BitsResolution;
  UINT8 SampleFreqType;
  UINT8 LowerSamFreq[3];
  UINT8 UpperSamFreq[3];
  
}__attribute__((packed))AUD_STR_FMT_TYPE_C_DES;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 FormatType;
  UINT8 NrOfChannel;
  UINT8 SubFrameSize;
  UINT8 BitsResolution;
  UINT8 SampleFreqType;
  UINT8 SamFreq[1][3];
  
}__attribute__((packed))AUD_STR_FMT_TYPE_D_DES;

typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 EndpointAddr;
  UINT8 bmAttributes;
  UINT16 MaxPackSize;
  UINT8 Intervel;
  UINT8 bRefreash;
  UINT8 bSynchAddr;
  
}__attribute__((packed))AUD_ISO_EP_DES;
typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDescriptorSubType;
  UINT8 Attributes;
  UINT8 LockDelayUnit;
  UINT16 LockDelay;
  
}__attribute__((packed))AUD_STR_ISO_AUD_DATA_EP_DES;

typedef struct 
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 EndpointAddr;
  UINT8 bmAttributes;
  UINT16 MaxPackSize;
  UINT8 Intervel;
  
}__attribute__((packed))AUD_INT_EP_DES;


PRIVATE UINT8 USBAudMinSpeaker[4] = {0xa0,0xe3};
PRIVATE UINT8 USBAudCurrentSpeaker[4] = {0x00,0x00};
PRIVATE UINT8 USBAudMaxSpeaker[4] = {0xf0,0xff};
PRIVATE UINT8 USBAudResolutionSpeaker[2] = {0x30,0x00};

PRIVATE UINT8 USBAudCurrentMic[4] = {0x00,0x00};
PRIVATE UINT8 USBAudMaxMic[4] = {0xd0,0x17};
PRIVATE UINT8 USBAudMinMic[4] = {0x00,0x00};
PRIVATE UINT8 USBAudResolutionMic[4] = {0x30,0x00};

PRIVATE UINT32 g_uAudSampleFreq = 0;
PRIVATE UINT32 g_uAudPacketSize = 0;
UINT32 g_uAudIsPlaying = 0;
PRIVATE UINT8  g_uAudMute = 0;
UINT32 g_uAudActualPacketSize = 0;
//PRIVATE INT16  g_uAudVol = 0;

PRIVATE UINT32 uAudSetSampleFreq(UINT8 freqLow,UINT8 freqMid,UINT8 freqHigh)
{
    g_uAudSampleFreq = (freqLow<<0)|(freqMid<<8)|(freqHigh<<16);
    g_uAudSampleFreq =48000;
    hal_HstSendEvent(USB_EVENT, g_uAudSampleFreq);
}
PRIVATE UINT32 uAudGetSampleFreq(void)
{
   //hal_HstSendEvent(USB_EVENT, g_uAudSampleFreq);
   return g_uAudSampleFreq;
}

PRIVATE UINT32 uAudSetPacketSize(UINT32 sampleFreq)
{
    g_uAudPacketSize = g_uAudSampleFreq/1000*4;
    if(sampleFreq == 0xac44)
    {
        g_uAudPacketSize = 192;
    }
    hal_HstSendEvent(USB_EVENT, 0xfcfcfcfc);
    hal_HstSendEvent(USB_EVENT, g_uAudPacketSize);
}
PRIVATE UINT32 uAudGetPacketSize(void)
{
   return g_uAudPacketSize;
}
PRIVATE UINT32 uAudDescConstructorAfterControl(UINT8* buffer)
{
	UINT8* p_desc;
	UINT32 if_desc_index = 0;
	
	p_desc = COS_Malloc(sizeof(AUD_CTL_IF_HEADER_DES));
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->bLength = sizeof(AUD_CTL_IF_HEADER_DES);
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->bDescriptorType = 0x24;;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->bDescriptorSubType = 0x01;;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->bcdADC = 0x0100;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->TotalLength = 0x0048;//0x47;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->InCollection  = 0x02;//0x02;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->InterfaceNr1= 0x01;
    ((AUD_CTL_IF_HEADER_DES*)p_desc)->InterfaceNr2 = 0x02;	
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_IF_HEADER_DES));
	if_desc_index += sizeof(AUD_CTL_IF_HEADER_DES);
	sxr_Free(p_desc);
	
	p_desc = COS_Malloc(sizeof(AUD_CTL_IN_TRM_DES));
	((AUD_CTL_IN_TRM_DES*)p_desc)->bLength = sizeof(AUD_CTL_IN_TRM_DES);
    ((AUD_CTL_IN_TRM_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->bDescriptorSubType = 0x02;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TerminalID = 0x01;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TerminalType = 0x0101;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->AssocTerminal = 0x00;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->NrChannals = 0x02;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->ChannalCofig = 0x0003;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->ChannalName = 0x00;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TermimalString = 0x00;
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_IN_TRM_DES));
	if_desc_index += sizeof(AUD_CTL_IN_TRM_DES);
	sxr_Free(p_desc);
#if 1	
	p_desc = COS_Malloc(sizeof(AUD_CTL_IN_TRM_DES));
	((AUD_CTL_IN_TRM_DES*)p_desc)->bLength = sizeof(AUD_CTL_IN_TRM_DES);
    ((AUD_CTL_IN_TRM_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->bDescriptorSubType = 0x02;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TerminalID = 0x02;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TerminalType = 0x0201;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->AssocTerminal = 0x00;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->NrChannals = 0x01;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->ChannalCofig = 0x0001;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->ChannalName = 0x00;
    ((AUD_CTL_IN_TRM_DES*)p_desc)->TermimalString = 0x00;
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_IN_TRM_DES));
	if_desc_index += sizeof(AUD_CTL_IN_TRM_DES);
	sxr_Free(p_desc);
#endif	
	p_desc = COS_Malloc(sizeof(AUD_CTL_OUT_TRM_DES));
	((AUD_CTL_OUT_TRM_DES*)p_desc)->bLength = sizeof(AUD_CTL_OUT_TRM_DES);
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->bDescriptorSubType = 0x03;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalID = 0x06;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalType= 0x0301;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->AssocTerminal = 0x00;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->SourceID = 0x09;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalString = 0x00;
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_OUT_TRM_DES));
	if_desc_index += sizeof(AUD_CTL_OUT_TRM_DES);
	sxr_Free(p_desc);
#if 1
    p_desc = COS_Malloc(sizeof(AUD_CTL_OUT_TRM_DES));
	((AUD_CTL_OUT_TRM_DES*)p_desc)->bLength = sizeof(AUD_CTL_OUT_TRM_DES);
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->bDescriptorSubType = 0x03;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalID = 0x07;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalType= 0x0101;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->AssocTerminal = 0x00;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->SourceID = 0x0a;
    ((AUD_CTL_OUT_TRM_DES*)p_desc)->TerminalString = 0x00;	
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_OUT_TRM_DES));
	if_desc_index += sizeof(AUD_CTL_OUT_TRM_DES);
	sxr_Free(p_desc);
#endif	
	p_desc = COS_Malloc(sizeof(AUD_CTL_FTR_UNIT_DES_1));
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->bLength = sizeof(AUD_CTL_FTR_UNIT_DES_1);
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->bDescriptorSubType = 0x06;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->UnitID = 0x09;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->SourceID = 0x01;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->ControlSize = 0x01;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->Controls[0] = 0x01;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->Controls[1]= 0x02;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->Controls[2] = 0x02;
    ((AUD_CTL_FTR_UNIT_DES_1*)p_desc)->FeatrueString = 0x00;
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_FTR_UNIT_DES_1));
	if_desc_index += sizeof(AUD_CTL_FTR_UNIT_DES_1);
	sxr_Free(p_desc);
#if 1
	p_desc = COS_Malloc(sizeof(AUD_CTL_FTR_UNIT_DES_2));
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->bLength = sizeof(AUD_CTL_FTR_UNIT_DES_2);
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->bDescriptorType = 0x24;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->bDescriptorSubType = 0x06;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->UnitID = 0x0a;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->SourceID = 0x02;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->ControlSize = 0x01;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->Controls[0] = 0x43;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->Controls[1] = 0x00;
    ((AUD_CTL_FTR_UNIT_DES_2*)p_desc)->FeatrueString = 0x00;
	memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_CTL_FTR_UNIT_DES_2));
	if_desc_index += sizeof(AUD_CTL_FTR_UNIT_DES_2);
	sxr_Free(p_desc);
#endif	
	return if_desc_index;

}
PRIVATE UINT32 uAudDescConstructorAfterStreamOut(UINT8* buffer)
{
    UINT8* p_desc;
    UINT32 if_desc_index = 0;
    
    p_desc = COS_Malloc(sizeof(AUD_STR_IF_DES));
    ((AUD_STR_IF_DES*)p_desc)->bLength = sizeof(AUD_STR_IF_DES);
    ((AUD_STR_IF_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_STR_IF_DES*)p_desc)->bDescriptorSubType = 0x01;
    ((AUD_STR_IF_DES*)p_desc)->TerminalLink = 0x01;
    ((AUD_STR_IF_DES*)p_desc)->Delay = 0x01;
    ((AUD_STR_IF_DES*)p_desc)->FormatTag = 0x0001;
    memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_IF_DES));
    if_desc_index += sizeof(AUD_STR_IF_DES);
    sxr_Free(p_desc);
    
    p_desc = COS_Malloc(sizeof(AUD_STR_FMT_TYPE_D_DES));
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bLength = sizeof(AUD_STR_FMT_TYPE_D_DES);
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bDescriptorType = 0x24;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bDescriptorSubType= 0x02;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->FormatType = 0x01;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->NrOfChannel = 0x02;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SubFrameSize = 0x02;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->BitsResolution  = 0x10;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SampleFreqType = 0x01;
    #if 0
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][0] = 0x40;//8k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][1] = 0x1f;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[1][0] = 0x11;//11.025k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[1][1] = 0x2b;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[1][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[2][0] = 0xe0;//12k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[2][1] = 0x2e;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[2][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[3][0] = 0x80;//16k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[3][1] = 0x3e;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[3][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[4][0] = 0x22;//22.05k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[4][1] = 0x56;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[4][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[5][0] = 0xc0;//24k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[5][1] = 0x5d;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[5][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[6][0] = 0x00;//32k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[6][1] = 0x7d;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[6][2] = 0x00;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[7][0] = 0x44;//44.1k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[7][1] = 0xac;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[7][2] = 0x00;
    #endif
#if 1
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][0] = 0x80;//48k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][1] = 0xbb;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][2] = 0x00;
#endif
#if 0
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][0] = 0x44;//44.1k sample rate
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][1] = 0xac;
    ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][2] = 0x00;
#endif
    memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_FMT_TYPE_D_DES));
    if_desc_index += sizeof(AUD_STR_FMT_TYPE_D_DES);
    sxr_Free(p_desc);
    
    p_desc = COS_Malloc(sizeof(AUD_STR_ISO_AUD_DATA_EP_DES));
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bLength = sizeof(AUD_STR_ISO_AUD_DATA_EP_DES);
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bDescriptorType = 0x25;
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bDescriptorSubType = 0x01;
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->Attributes = 0x01;
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->LockDelayUnit = 0x00;
    ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->LockDelay = 0x0000;
    memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_ISO_AUD_DATA_EP_DES));
    if_desc_index += sizeof(AUD_STR_ISO_AUD_DATA_EP_DES);
    sxr_Free(p_desc);
    
    return if_desc_index;
}
PRIVATE UINT32 uAudDescConstructorAfterStreamIn(UINT8* buffer)
{
   
   UINT8* p_desc;
   UINT32 if_desc_index = 0;
   
   p_desc = COS_Malloc(sizeof(AUD_STR_IF_DES));
   ((AUD_STR_IF_DES*)p_desc)->bLength = sizeof(AUD_STR_IF_DES);
   ((AUD_STR_IF_DES*)p_desc)->bDescriptorType = 0x24;
   ((AUD_STR_IF_DES*)p_desc)->bDescriptorSubType = 0x01;
   ((AUD_STR_IF_DES*)p_desc)->TerminalLink = 0x07;
   ((AUD_STR_IF_DES*)p_desc)->Delay = 0x01;
   ((AUD_STR_IF_DES*)p_desc)->FormatTag = 0x0001;
   memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_IF_DES));
   if_desc_index += sizeof(AUD_STR_IF_DES);
   sxr_Free(p_desc);

   p_desc = COS_Malloc(sizeof(AUD_STR_FMT_TYPE_D_DES));
   ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bLength = sizeof(AUD_STR_FMT_TYPE_D_DES);
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bDescriptorType = 0x24;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->bDescriptorSubType= 0x02;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->FormatType = 0x01;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->NrOfChannel = 0x02;// 2 channels
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SubFrameSize = 0x02;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->BitsResolution  = 0x10;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SampleFreqType = 0x01;
  #if 0
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][0] = 0x40;//8k sample rate
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][1] = 0x1f;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][2] = 0x00;
 #else
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][0] = 0x44;//44.1k sample rate
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][1] = 0xac;
  ((AUD_STR_FMT_TYPE_D_DES*)p_desc)->SamFreq[0][2] = 0x00;
 #endif
#if 0
   p_desc = COS_Malloc(sizeof(AUD_STR_FMT_TYPE_C_DES));
   ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->bLength = sizeof(AUD_STR_FMT_TYPE_C_DES);
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->bDescriptorType = 0x24;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->bDescriptorSubType= 0x02;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->FormatType = 0x01;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->NrOfChannel = 0x01;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->SubFrameSize = 0x02;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->BitsResolution  = 0x10;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->SampleFreqType = 0x02;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->LowerSamFreq[0] = 0x80;//44.1k sample rate
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->LowerSamFreq[1] = 0xbb;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->LowerSamFreq[2] = 0x00;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->UpperSamFreq[0] = 0x44;//48k sample rate
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->UpperSamFreq[1] = 0xac;
  ((AUD_STR_FMT_TYPE_C_DES*)p_desc)->UpperSamFreq[2] = 0x00;
#endif
  
  memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_FMT_TYPE_D_DES));
  if_desc_index += sizeof(AUD_STR_FMT_TYPE_D_DES);
  sxr_Free(p_desc);
 
  p_desc = COS_Malloc(sizeof(AUD_STR_ISO_AUD_DATA_EP_DES));
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bLength = sizeof(AUD_STR_ISO_AUD_DATA_EP_DES);
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bDescriptorType = 0x25;
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->bDescriptorSubType = 0x01;
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->Attributes = 0x00;
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->LockDelayUnit = 0x00;
  ((AUD_STR_ISO_AUD_DATA_EP_DES*)p_desc)->LockDelay = 0x0000;
  
  memcpy(&buffer[if_desc_index], p_desc, sizeof(AUD_STR_ISO_AUD_DATA_EP_DES));
  if_desc_index += sizeof(AUD_STR_ISO_AUD_DATA_EP_DES);
  sxr_Free(p_desc);
  return if_desc_index;
}


PRIVATE void uaudSendEvent(HAL_USB_CALLBACK_EP_TYPE_T type, UINT32 play_stop)
{
    COS_EVENT ev;
    ev.nEventId = EV_USB_AUD_EVENT_ID;
    ev.nParam1 = type;
 //   ev.nParam2 = play_stop;//for delete nParam2

    COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

PRIVATE HAL_USB_CALLBACK_RETURN_T
uaudControlInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T	 type,
			HAL_USB_SETUP_T*			 setup)
{
    static UINT8 counter = 0;
	hal_HstSendEvent(USB_EVENT,0xfcfc7000);
	switch(type)
	{
	case HAL_USB_CALLBACK_TYPE_CMD: 	
		hal_HstSendEvent(USB_EVENT,0xfcfc7001);
		hal_HstSendEvent(USB_EVENT,setup->lenght);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd\n");		
		if(USB_AUD_SET_CUR == setup->request)
		{
		   memset(g_UsbAudCmdDataBuf,0,sizeof(g_UsbAudCmdDataBuf));
		   hal_UsbRecv(0,g_UsbAudCmdDataBuf,setup->lenght,0);
		   return HAL_USB_CALLBACK_RETURN_OK;
		}
		if(setup->index == 0x0900)
		{
           if(USB_AUD_GET_CUR == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x81090000|USBAudCurrentSpeaker);
              hal_UsbSend(0,USBAudCurrentSpeaker,setup->lenght,0);
              return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_MIN == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x82090000|USBAudMinSpeaker);
			   hal_HstSendEvent(USB_EVENT,0x13030501);
           	hal_UsbSend(0,USBAudMinSpeaker,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_MAX == setup->request)
           {
			  // hal_HstSendEvent(USB_EVENT,0x83090000|USBAudMaxSpeaker);
			   hal_UsbSend(0,USBAudMaxSpeaker,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_RES == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x84090000|USBAudResolutionSpeaker);
           	hal_UsbSend(0,USBAudResolutionSpeaker,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
		}		
		else if(setup->index == 0x0a00)
		{
           if(USB_AUD_GET_CUR == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x810a0000|USBAudCurrentMic);
              hal_UsbSend(0,USBAudCurrentMic,setup->lenght,0);
              return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_MIN == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x820a0000|USBAudMinMic);
           	hal_UsbSend(0,USBAudMinMic,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_MAX == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x830a0000|USBAudMaxMic);
           	hal_UsbSend(0,USBAudMaxMic,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
           if(USB_AUD_GET_RES == setup->request)
           {
			   //hal_HstSendEvent(USB_EVENT,0x840a0000|USBAudResolutionMic);
           	hal_UsbSend(0,USBAudResolutionMic,setup->lenght,0);
           	return HAL_USB_CALLBACK_RETURN_OK;
           }
		}
		break;
	case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xfcfc7002);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd data\n");			
		if(USB_AUD_SET_CUR == setup->request)
		{
		//	hal_HstSendEvent(USB_EVENT,g_UsbAudCmdDataBuf[0]);
		//	hal_HstSendEvent(USB_EVENT,g_UsbAudCmdDataBuf[1]);
     		if(setup->lenght == 1)
     		{
     		   if(setup->index == 0x0900)
     		   	{
//				   USBAudCurrentSpeaker[0]= 0xc0;
//				   USBAudCurrentSpeaker[1]= 0xff;

                   g_uAudMute = g_UsbAudCmdDataBuf[0];
                   
				   COS_EVENT ev;
                   ev.nEventId = EV_FS_USB_VOL_CHANGE;
                   ev.nParam1 = 0;
                   
                   COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
     		   	}
     		   else if(setup->index == 0x0a00)
     		   	{
     		   	  USBAudCurrentMic[0] = 0x00;
     		   	  USBAudCurrentMic[1] = 0x00;
     		   	}
     		}
     		if(setup->lenght == 2)
     		{
     			if(setup->index == 0x0900)
     			{
     			   USBAudCurrentSpeaker[0]= g_UsbAudCmdDataBuf[0];
     			   USBAudCurrentSpeaker[1]= g_UsbAudCmdDataBuf[1];

//     			   g_uAudVol = (g_UsbAudCmdDataBuf[1]<<8)|g_UsbAudCmdDataBuf[0];
                   
				   COS_EVENT ev;
                   ev.nEventId = EV_FS_USB_VOL_CHANGE;
                   ev.nParam1 = 0;
                   
                   COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
     			}
     			else if(setup->index == 0x0a00)
     			{
     			   USBAudCurrentMic[0] = g_UsbAudCmdDataBuf[0];
     			   USBAudCurrentMic[1] = g_UsbAudCmdDataBuf[1];
     			}
     		}
		}
		break;
	case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc7003);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Receive end\n");
		break;
	case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc7004);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
		break;
	case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc7005);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Enable\n");		
		break;
	case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc7006);
		UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Disable\n");
		break;
	}
	return(HAL_USB_CALLBACK_RETURN_KO); 
}

void Usb_Audio_Input()
{
#if 0
static UINT32 virtual_audio[180] = {
0x00, 0x00, 0x5c, 0x2d,
0x26, 0x40, 0x5d, 0x2d, 0x00, 0x00, 0xa4, 0xd2, 0xd9, 0xbf, 0xa4, 0xd2, 0xff, 0xff, 0x5c, 0x2d, 
0x27, 0x40, 0x5d, 0x2d, 0x00, 0x00, 0xa4, 0xd2, 0xda, 0xbf, 0xa3, 0xd2, 0x00, 0x00, 0x5c, 0x2d, 
0x27, 0x40, 0x5c, 0x2d, 0xff, 0xff, 0xa4, 0xd2, 0xda, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5c, 0x2d, 
0x27, 0x40, 0x5d, 0x2d, 0x00, 0x00, 0xa4, 0xd2, 0xda, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5d, 0x2d, 
0x26, 0x40, 0x5c, 0x2d, 0x00, 0x00, 0xa3, 0xd2, 0xda, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5c, 0x2d, 
0x27, 0x40, 0x5c, 0x2d, 0xff, 0xff, 0xa3, 0xd2, 0xda, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5b, 0x2d, 
0x26, 0x40, 0x5c, 0x2d, 0x00, 0x00, 0xa4, 0xd2, 0xd9, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5c, 0x2d, 
0x26, 0x40, 0x5c, 0x2d, 0x01, 0x00, 0xa4, 0xd2, 0xd9, 0xbf, 0xa3, 0xd2, 0x00, 0x00, 0x5d, 0x2d, 
0x26, 0x40, 0x5c, 0x2d, 0xff, 0xff, 0xa4, 0xd2, 0xd9, 0xbf, 0xa4, 0xd2, 0x00, 0x00, 0x5c, 0x2d, 
0x26, 0x40, 0x5d, 0x2d, 0x01, 0x00, 0xa4, 0xd2, 0xd9, 0xbf, 0xa3, 0xd2, 0x01, 0x00, 0x5d, 0x2d, 
0x27, 0x40, 0x5d, 0x2d, 0x00, 0x00, 0xa4, 0xd2, 0xd9, 0xbf, 0xa4, 0xd2, 0xff, 0xff, 0x5d, 0x2d
};
    static UINT32 package_counter = 0;
    static UINT32 counter = 0;
    UINT8* p = virtual_audio;
    UINT8 i = 0;
    while(i < 176)
    {
        *p++ = i++;
    }

    if(counter == 9)//The tenth package should contain 4 more bytes
    {
        p[0] = i;
        p[1] = i + 1;
        p[2] = i + 2;
        p[3] = i + 3;
    }
//    hal_UsbSend(g_uaudIsoInEp, virtual_audio, 16, 0);
    hal_UsbSend(g_uaudIsoInEp, virtual_audio, counter == 9?180:176, 0);
    if(counter == 9)
    {
        counter = 0;
    }
    else
    {
        counter ++;
    }
    package_counter++;
//    hal_HstSendEvent(USB_EVENT, 0x12200001);
//    hal_HstSendEvent(USB_EVENT, package_counter);
#endif
}

PRIVATE HAL_USB_CALLBACK_RETURN_T
uaudStreamInInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
	//hal_HstSendEvent(USB_EVENT,0xfcfc6000);
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:		
		hal_HstSendEvent(USB_EVENT,0xfcfc6001);
                  UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd\n");
		if(setup->request == 0x0b)//Set interaface
		{
		   hal_HstSendEvent(USB_EVENT,setup->value);
		   if(setup->value == 0x0001)//Alternate Setting:1 (play)
		   {
			   hal_HstSendEvent(USB_EVENT,0xfcfc6011);
			   Usb_Audio_Input();
		     // g_uAudIsPlaying = 1;
		   }
		   else if(setup->value == 0x0000)//Alternate Setting: 0(stop)
		   {
			   hal_HstSendEvent(USB_EVENT,0xfcfc6021);	   
		      //g_uAudIsPlaying = 0;
		   }
		}
		//hal_UsbRecv(0,g_UsbAudCmdDataBuf,1,0);
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xfcfc6002);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc6003);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc6004);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc6005);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Enable\n");		
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc6006);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO); 
}

extern volatile UINT8 reset_flag_hid;
volatile UINT8 reset_flag_aud = 0;
//extern BOOL mediaSendCommand(UINT8 cmd, INT32 param);

PRIVATE HAL_USB_CALLBACK_RETURN_T
uaudStreamOutInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
	//hal_HstSendEvent(USB_EVENT,0xfcfc8000);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:		
		//hal_HstSendEvent(USB_EVENT,0xfcfc8001);		
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd\n");	
		if(setup->request == 0x0b)//Set interaface
		{
		   if(setup->value == 0x0001)//Alternate Setting:1 (play)
		   {
			   //hal_HstSendEvent(USB_EVENT,0xfcfc8011);	   
		      g_uAudIsPlaying = 1;
                        uaudSendEvent(HAL_USB_CALLBACK_TYPE_CMD, 1);
		   }
		   else if(setup->value == 0x0000)//Alternate Setting: 0(stop)
		   {
			   //hal_HstSendEvent(USB_EVENT,0xfcfc8021);	   
		      g_uAudIsPlaying = 0;
                        uaudSendEvent(HAL_USB_CALLBACK_TYPE_CMD, 0);
		      
		      if(reset_flag_hid == 1)
		      {
                             reset_flag_aud = 1;
		      }
		   }
		}
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xfcfc8002);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc8003);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xfcfc8004);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc8005);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xfcfc8006);
        UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
	}	
    return(HAL_USB_CALLBACK_RETURN_KO); 
}

volatile UINT32 last_music_tick;
extern UINT8 uaudCodecOpen;

HAL_USB_CALLBACK_RETURN_T uaudStreamOutCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
   //hal_HstSendEvent(USB_EVENT,0xb200000f);	
   UINT8* rxBuffer = NULL;
   switch(type)
   {
      case HAL_USB_CALLBACK_TYPE_DATA_CMD:
         //hal_HstSendEvent(USB_EVENT,0xb2000000);
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");	
         //uaudSendEvent(HAL_USB_CALLBACK_TYPE_DATA_CMD);
		 uAudSetSampleFreq(g_UsbAudCmdDataBuf[0],g_UsbAudCmdDataBuf[1],g_UsbAudCmdDataBuf[2]);
		 uAudSetPacketSize(uAudGetSampleFreq());
		 //MMC_USBSetFreq(uAudGetSampleFreq());
         hal_UsbRecv(g_uaudIsoOutEp,g_UsbAudIsoOutBuf,uAudGetPacketSize(),0);		 
         break;		 
      case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
         hal_UsbRecv(g_uaudIsoOutEp,g_UsbAudIsoOutBuf,g_uAudPacketSize,0);
         uaudSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END, 1);
         if(1 == uaudCodecOpen)
         {    
             rxBuffer = (UINT8 *)MMC_USBGetRXBuffer();
             if(rxBuffer)
             {
                 memcpy(rxBuffer, g_UsbAudIsoOutBuf, g_uAudActualPacketSize);
                 MMC_USBReceiveData(g_uAudActualPacketSize);	
             }
         }

         last_music_tick = hal_TimGetUpTime();
         break;
      case HAL_USB_CALLBACK_TYPE_CMD:       
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");	
         if((setup->request == 0x01)&&(setup->value == 0x0100)&&(setup->lenght == 0x03))
         {
            hal_HstSendEvent(0xff,0x20170214);
            //memset(g_UsbAudCmdDataBuf,0,sizeof(g_UsbAudCmdDataBuf));
            hal_UsbRecv(0,g_UsbAudCmdDataBuf,setup->lenght,0);      
            return HAL_USB_CALLBACK_RETURN_OK;
         }
      case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
         //hal_HstSendEvent(USB_EVENT,0xb2000003); 
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
         uaudSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END, 1);
         break;
      case HAL_USB_CALLBACK_TYPE_ENABLE:
         //hal_HstSendEvent(USB_EVENT,0xb2000004); 
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
         break;
      case HAL_USB_CALLBACK_TYPE_DISABLE:
         //hal_HstSendEvent(USB_EVENT,0xb2000005); 
         UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
         break;
   }
   return(HAL_USB_CALLBACK_RETURN_KO);
}

PRIVATE HAL_USB_CALLBACK_RETURN_T uaudStreamInCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
    //hal_HstSendEvent(USB_EVENT,0xb100000f);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
//    hal_HstSendEvent(USB_EVENT,0xb1000000);		
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");	
    Usb_Audio_Input();
    break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
    //hal_HstSendEvent(USB_EVENT,0xb1000001); 
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
    uaudSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END, 1);
    break;
    case HAL_USB_CALLBACK_TYPE_CMD:
//    hal_HstSendEvent(USB_EVENT,0xb1000002); 		
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");	
  //   if((setup->request == 0x01)&&(setup->value == 0x0100)&&(setup->index == 0x0001)&&(setup->lenght == 0x03))
    if((setup->request == 0x01)&&(setup->value == 0x0100)&&(setup->lenght == 0x03))
    {
       memset(g_UsbAudCmdDataBuf,0,sizeof(g_UsbAudCmdDataBuf));
       hal_UsbRecv(0,g_UsbAudCmdDataBuf,setup->lenght,0);      
       return HAL_USB_CALLBACK_RETURN_OK;
    }
    break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
    //hal_HstSendEvent(USB_EVENT,0xb1000003); 
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
//    uaudSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END);
    Usb_Audio_Input();
    break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
    //hal_HstSendEvent(USB_EVENT,0xb1000004); 
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
    break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
    //hal_HstSendEvent(USB_EVENT,0xb1000005); 
    UAUD_TRACE(UAUD_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
    break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}

PUBLIC VOID uaud_TransportEventProcess(UINT32 nParam1)
{
    UINT32                           i;
    COS_EVENT                        ev;
    UINT16                           size;
    UINT8* rxBuffer;

    switch(nParam1)
    {
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		#ifdef USB_AUDIO_SUPPORT 
        if(0 == uaudCodecOpen)
        {
            hal_HstSendEvent(0xff,0x2017aaa2);
            rxBuffer = (UINT8 *)MMC_USBGetRXBuffer();
            if(rxBuffer)
            {
                memcpy(rxBuffer, g_UsbAudIsoOutBuf, g_uAudActualPacketSize);
    	        MMC_USBReceiveData(g_uAudActualPacketSize);	
            }
        }
        #endif
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
              hal_HstSendEvent(0xff, 0x17030400);
                Usb_Audio_Input();
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
    case HAL_USB_CALLBACK_TYPE_CMD:
         hal_HstSendEvent(0xff, 0x17012801);
         uAudSetSampleFreq(g_UsbAudCmdDataBuf[0],g_UsbAudCmdDataBuf[1],g_UsbAudCmdDataBuf[2]);
		 uAudSetPacketSize(uAudGetSampleFreq());
         hal_UsbRecv(g_uaudIsoOutEp,g_UsbAudIsoOutBuf,uAudGetPacketSize(),0);	
         MMC_USBClearRXBuffer(); 
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
    case HAL_USB_CALLBACK_TYPE_DISABLE:
        break;
    }
}

PUBLIC void uaud_Open(void)
{
   
   HAL_USB_EP_DESCRIPTOR_T **epListOut;
   HAL_USB_EP_DESCRIPTOR_T **epListIn;
   UINT8                     epNum;
   extern uint32 *inputbuf_overlay;
   
   epListOut = (HAL_USB_EP_DESCRIPTOR_T**)
   COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);
   memset(epListOut, 0, sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);
   epListIn = (HAL_USB_EP_DESCRIPTOR_T**)
   COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);
   memset(epListIn, 0, sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);

 //   g_UsbAudIsoOutBuf = (UINT8*)(inputbuf_overlay);
 //   g_UsbAudCmdDataBuf = (UINT8*)(inputbuf_overlay+USB_AUD_ISO_OUT_BUF_SIZE/4);
  if(!g_UsbAudIsoOutBuf)
  {
    g_UsbAudIsoOutBuf = (UINT8*)COS_Malloc_NoCache(USB_AUD_ISO_OUT_BUF_SIZE);
  }
   if(!g_UsbAudCmdDataBuf)
   {
     g_UsbAudCmdDataBuf = (UINT8*)COS_Malloc_NoCache(USB_AUD_CMD_DATA_BUF_SIZE);
   }
   
 //  g_UsbAudIsoOutBuf = (UINT8*)COS_Malloc(USB_AUD_ISO_OUT_BUF_SIZE);
 //  g_UsbAudCmdDataBuf = (UINT8*)COS_Malloc(USB_AUD_CMD_DATA_BUF_SIZE);

    memset(g_UsbAudIsoOutBuf, 0, USB_AUD_ISO_OUT_BUF_SIZE);
    memset(g_UsbAudCmdDataBuf, 0, USB_AUD_CMD_DATA_BUF_SIZE);
   
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x01, 0x00, 0x00, 0, 0, uaudControlInterfaceCallback));
   
   uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uAudDescConstructorAfterControl,0));	//shaohuima 2016/07/08
#if 0
   //USB class :0x01 Audio class
   //USB Subclass : 0x02 Audio stream class
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0xF0, 0, 0, uaudStreamInInterfaceCallback));
   epNum = uctls_AllocateEndPoint();	
   //hal_HstSendEvent(USB_EVENT,epNum);
   g_uaudIsoInEp  = HAL_USB_EP_DIRECTION_IN (epNum);	
   //hal_HstSendEvent(USB_EVENT,g_uaudIsoInEp);
   epListIn[0] = uctls_NewEpDescriptor(g_uaudIsoInEp, 1, uaudStreamInCallbackEp, HAL_USB_EP_TYPE_ISO);
   epListIn[1] = 0;
   //hal_HstSendEvent(USB_EVENT,epListIn);
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0x0F, epListIn,0, uaudStreamInInterfaceCallback));
   uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uAudDescConstructorAfterStreamIn,0));
#endif   
#if 1
     //USB class :0x01 Audio class
     //USB Subclass : 0x02 Audio stream class
     uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0xF0, 0, 0, uaudStreamOutInterfaceCallback));
     epNum = uctls_AllocateEndPoint();	
     //epNum = uctls_AllocateEndPoint();
     //hal_HstSendEvent(USB_EVENT,0xfcfcaaaa);
     //hal_HstSendEvent(USB_EVENT,epNum);
     g_uaudIsoOutEp  = HAL_USB_EP_DIRECTION_OUT (epNum);	
     //hal_HstSendEvent(USB_EVENT,g_uaudIsoOutEp);
     epListOut[0] = uctls_NewEpDescriptor(g_uaudIsoOutEp, 1, uaudStreamOutCallbackEp,HAL_USB_EP_TYPE_ISO);
     epListOut[1] = 0;
     //hal_HstSendEvent(USB_EVENT,0xfcfcaaac);
     //hal_HstSendEvent(USB_EVENT,epListOut);
     uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0x0F, epListOut,0, uaudStreamOutInterfaceCallback));
     uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uAudDescConstructorAfterStreamOut,0));	//shaohuima 2016/07/08	
#endif
#if 1
   //USB class :0x01 Audio class
   //USB Subclass : 0x02 Audio stream class
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0xF0, 0, 0, uaudStreamInInterfaceCallback));
   epNum = uctls_AllocateEndPoint();	
   //hal_HstSendEvent(USB_EVENT,epNum);
   g_uaudIsoInEp  = HAL_USB_EP_DIRECTION_IN (epNum);	
   //hal_HstSendEvent(USB_EVENT,g_uaudIsoInEp);
   epListIn[0] = uctls_NewEpDescriptor(g_uaudIsoInEp, 1, uaudStreamInCallbackEp, HAL_USB_EP_TYPE_ISO);
   epListIn[1] = 0;
   //hal_HstSendEvent(USB_EVENT,epListIn);
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_AUDIO, 0x02, 0x00, 0x0F, epListIn,0, uaudStreamInInterfaceCallback));
   uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uAudDescConstructorAfterStreamIn,0));
#endif
}

PRIVATE VOID uaud_Close(VOID)
{

}

PUBLIC UINT32 uAudGetPlayStatus(void)
{
   return g_uAudIsPlaying;
}

PUBLIC VOID uAudGetVolFromCmd(UINT8* mute, UINT16* vol)
{
    *mute = g_uAudMute;
    *vol = (USBAudCurrentSpeaker[1]<<8)|USBAudCurrentSpeaker[0];
}

PUBLIC VOID uAudSetVolFromCmd(UINT8 mute, INT16 vol)
{
    g_uAudMute = mute;
    USBAudCurrentSpeaker[0] = vol&0x00ff;
    USBAudCurrentSpeaker[1] = (vol&0xff00)>>8;
}
// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC CONST UAUD_TRANSPORT_CALLBACK_T g_uaudServiceCallback =
{
    //.open  = uaud_TransportBoScsiOpen,
    .close = uaud_Close
};

#endif

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

