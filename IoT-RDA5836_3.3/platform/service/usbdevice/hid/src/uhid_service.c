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
/// @file uhid_service.c
///
/// Transport Bulk Only function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "csw.h"
#include "uhid_m.h"
#include "hal_host.h"

//typedef HAL_USB_CALLBACK_EP_TYPE_T MsgBody_t;
//#include "itf_msg.h"

#include "hal_usb.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "uctls_m.h"

#include <string.h>

#include "event.h"

#ifndef _UHID_MOUSE_
//#define _UHID_MOUSE_
#endif

#ifndef _UHID_KEYBOARD_
//#define _UHID_KEYBOARD_
#endif
extern PUBLIC HAL_USB_CS_INTERFACE_DESCRIPTOR_T*
uctls_NewCSInterfaceDescriptor(HAL_USB_CS_CONSTRUCTOR_T constructor,
                                          HAL_USB_CALLBACK_T       callback);
// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
// =============================================================================
#define UHID_WARN_TRC  0 // SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define UHID_INFO_TRC   0 //SVC_UMSS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define UHID_DBG_TRC    0 //SVC_DBG_TRC


#ifndef UHID_NO_PRINTF
  #define UHID_TRACE(level, tsmap, format, ...)   CSW_TRACE(level,format, ##__VA_ARGS__)
#else
  #define UHID_TRACE(level, tsmap, fmt, ...)
#endif // UHID_NO_PRINTF

// =============================================================================
// MACROS                                                                       
// =============================================================================

//#define CBW_SIGNATURE 0x43425355
//#define CSW_SIGNATURE 0x53425355


//USB HID report descriptor constants
#define USB_HID_USAGE_PAGE 0x05
#define USB_HID_USAGE 0x09
#define USB_HID_COLL_START 0xa1//collection start
#define USB_HID_COLL_END 0xc0//collection end
#define USB_HID_LOGICAL_MIN 0x15
#define USB_HID_LOGICAL_MAX 0x25
#define USB_HID_USAGE_MIN 0x19
#define USB_HID_USAGE_MAX 0x29
#define USB_HID_REPORT_COUNT 0x95
#define USB_HID_REPORT_SIZE 0x75
#define USB_HID_INPUT_DES 0x81//input descriptor
#define USB_HID_OUTPUT_DES 0x91//output descriptor


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================

// =============================================================================
// HID MOUSE Variable and function Definition                                                            
// =============================================================================
#ifdef _UHID_MOUSE_
#define MOUSE_INPUT_SIZE 4
PRIVATE UINT8                        g_uhid_MouseEp;
PRIVATE UINT8 USBHIDMousecode[MOUSE_INPUT_SIZE] = {0x00,0x00,0x00,0x00};
PRIVATE UINT8 Mouse_ReportDescriptor[] = {
                                        USB_HID_USAGE_PAGE,0x01,//Usage page:Generic Desktop
                                        USB_HID_USAGE,0x02,//Usage:Mouse
                                        USB_HID_COLL_START,0x01,//Collection:Application
                                          USB_HID_USAGE,0x01,//Usage:Pointer
                                          USB_HID_COLL_START,0x00,//Collection:Physical
                                            0x85,0x01,//Report ID
                                            USB_HID_USAGE_PAGE,0x09,//Usage page:Buttons
                                            USB_HID_USAGE_MIN,0x01,//Usage Minimum:1
                                            USB_HID_USAGE_MAX,0x03,//Usage Maximum:3
                                            USB_HID_LOGICAL_MIN,0x00,//Logical Minimum:0
                                            USB_HID_LOGICAL_MAX,0x01,//Logical Maximum:1
                                            USB_HID_REPORT_SIZE,0x01,//Report size:1
                                            USB_HID_REPORT_COUNT,0x03,//Report Count:3
                                            USB_HID_INPUT_DES,0x02,//Input:Data,Variable,Absolute
                                            USB_HID_REPORT_SIZE,0x05,//Report Size:5
                                            USB_HID_REPORT_COUNT,0x01,//Report Count:1
                                            USB_HID_INPUT_DES,0x01,//Input:Constant
                                            USB_HID_USAGE_PAGE,0x01,//Usage page:Generic Desktop
                                            USB_HID_USAGE,0x30,//Usage:Coordinate X
                                            USB_HID_USAGE,0x31,//Usage:Coordinate Y
                                            USB_HID_USAGE,0x38,//Usage:Wheel
                                            USB_HID_LOGICAL_MIN,0x81,//Logical Minimum:-127
                                            USB_HID_LOGICAL_MAX,0x7f,//Logical Maximum:127
                                            USB_HID_REPORT_SIZE,0x08,//Report Size:8
                                            USB_HID_REPORT_COUNT,0x03,//Report Size:3
                                            USB_HID_INPUT_DES,0x06,//Input:Data,Variable,Relative
                                          USB_HID_COLL_END,//End Collection
                                        USB_HID_COLL_END//End Collection
                                    };

PRIVATE UINT32 uHid_MouseDescConstructorAfterInterrupt(UINT8* buffer)
{
	UINT8* p_desc;
	UINT32 if_desc_index = 0;
	
	p_desc = COS_Malloc(sizeof(HAL_USB_HID_DES));
	((HAL_USB_HID_DES*)p_desc)->bLength = sizeof(HAL_USB_HID_DES);
	((HAL_USB_HID_DES*)p_desc)->bDescriptorType = 0x21;
	((HAL_USB_HID_DES*)p_desc)->bcdHID = 0x0100;
	((HAL_USB_HID_DES*)p_desc)->bCountryCode = 0x00;
	((HAL_USB_HID_DES*)p_desc)->bNumberDescriptor = 0x01;
	((HAL_USB_HID_DES*)p_desc)->bDescType = 0x22;
	((HAL_USB_HID_DES*)p_desc)->DescriptorLength = sizeof(Mouse_ReportDescriptor);
	  
    memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_HID_DES));
    if_desc_index += sizeof(HAL_USB_HID_DES);
    sxr_Free(p_desc);
	
	 p_desc = COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->size = sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->type = 0x05;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->ep = g_uhid_MouseEp;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->attributes = 0x03;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->mps = 0x0004;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->interval = 0x0A;
	 memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	if_desc_index += sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	sxr_Free(p_desc);

    return if_desc_index;

}

PRIVATE HAL_USB_CALLBACK_RETURN_T
uhid_MouseInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
    //hal_HstSendEvent(USB_EVENT,0xfcfc9000);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:		
	    //hal_HstSendEvent(USB_EVENT,0xfcfc9001);		
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd\n");
		if(setup->request == 0x06)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e706);
		   //hal_HstSendEvent(USB_EVENT,setup->lenght);
		   //hal_HstSendEvent(USB_EVENT,sizeof(ReportDescriptor));
		   hal_UsbSend(0,Mouse_ReportDescriptor,sizeof(Mouse_ReportDescriptor),0);
           return (HAL_USB_CALLBACK_RETURN_OK);
		}
		if(setup->request == 0x0a)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e70a);
		}
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
	 //hal_HstSendEvent(USB_EVENT,0xfcfc9002);
	 UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9003);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9004);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9005);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9006);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
	}	
    return(HAL_USB_CALLBACK_RETURN_KO); 
}


PRIVATE HAL_USB_CALLBACK_RETURN_T uhid_MouseCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
    UINT8 end = 0;
	static BOOL endflag = FALSE;
	//hal_HstSendEvent(USB_EVENT,0xb300000f);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000000); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000001); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
     //   uhidSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END);
        break;
    case HAL_USB_CALLBACK_TYPE_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000002); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000003); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
		//uhidSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END);
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000004); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000005); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}


PUBLIC void uhid_Mouse_Open(void)
{
   HAL_USB_EP_DESCRIPTOR_T **epListHID;
   UINT8                     epNum;
   
   epListHID = (HAL_USB_EP_DESCRIPTOR_T**)
   COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);
   
   
   epNum = uctls_AllocateEndPoint();
   g_uhid_MouseEp = HAL_USB_EP_DIRECTION_IN(epNum);
   epListHID[0] = uctls_NewEpDescriptor(g_uhid_MouseEp,0x0a, uhid_MouseCallbackEp,HAL_USB_EP_TYPE_INTERRUPT);
   epListHID[1] = 0;
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_HID, 0x00, 0x02, 0x00,epListHID,0, uhid_MouseInterfaceCallback));
   uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uHid_MouseDescConstructorAfterInterrupt,0));
   
}

PUBLIC VOID uhid_Mouse_SendKeycode(UINT8* keycode_pointer)
{ 
   hal_UsbSend(g_uhid_MouseEp,keycode_pointer,MOUSE_INPUT_SIZE+1,0);  
}
#endif

// =============================================================================
// HID KEYBOARD Variable and function Definition                                                            
// =============================================================================
#ifdef _UHID_KEYBOARD_
#define KEYBOARD_INPUT_SIZE 8
PRIVATE UINT8                        g_uhid_KeyBoardEp;
PRIVATE UINT8 USBHIDKeyBoardcode[KEYBOARD_INPUT_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
PUBLIC INT8 USBHIDKeyBoard_OUTPUT[2] = {-1};
PRIVATE UINT8 KeyBoard_ReportDescriptor[] = {
                                           USB_HID_USAGE_PAGE,0x01,//Usage Page:Generic Desktop
                                           USB_HID_USAGE,0x06,//Usage:Keyboard
                                           USB_HID_COLL_START,0x01,//Collection:Application
                                               0x85,0x02,//Report ID
                                               USB_HID_USAGE,0x07,//Usage:Key Codes
                                               USB_HID_USAGE_MIN,0xe0,//Usage Minimum:224
                                               USB_HID_USAGE_MAX,0xe7,//Usage Maximum:231
                                               USB_HID_LOGICAL_MIN,0x00,//Logical Minimum:0
                                               USB_HID_LOGICAL_MAX,0x01,//Logical Maximum:1
                                               USB_HID_REPORT_SIZE,0x01,//Report Size:1
                                               USB_HID_REPORT_COUNT,0x08,//Report Count:8
                                               USB_HID_INPUT_DES,0x02,//Input:Data,Variable,Absolute
                                               USB_HID_REPORT_COUNT,0x01,//Report Count:1
                                               USB_HID_REPORT_SIZE,0x08,//Report Size:8
                                               USB_HID_INPUT_DES,0x01,//Input:Constant
/*begin of the output report*/                 USB_HID_REPORT_COUNT,0x05,//Report Count:5
                                               USB_HID_REPORT_SIZE,0x01,//Report Size:1
                                               USB_HID_USAGE_PAGE,0x08,//Usage page:LEDs
                                               USB_HID_USAGE_MIN,0x01,//Usage Minimum:0x01
                                               USB_HID_USAGE_MAX,0x05,//Usage Maximum:0x05
                                               USB_HID_OUTPUT_DES,0x02,//Output:Data,Variable,Absolute
                                               USB_HID_REPORT_COUNT,0x01,//Report Count:1
                                               USB_HID_REPORT_SIZE,0x03,//Report Size:3
/*end of the output report*/                   USB_HID_OUTPUT_DES,0x01,//Output:Constant
                                               USB_HID_REPORT_COUNT,0x06,//Report Count:6
                                               USB_HID_REPORT_SIZE,0x08,//Report Size:8
                                               USB_HID_LOGICAL_MIN,0x00,//Logical Minimum:0
                                               USB_HID_LOGICAL_MAX,0x65,//Logical Maximum:101
                                               USB_HID_USAGE_PAGE,0x07,//Usage Page:Key Codes
                                               USB_HID_USAGE_MIN,0x00,//Usage Minimum:1
                                               USB_HID_USAGE_MAX,0x65,//Usage Maximum:101
                                               USB_HID_INPUT_DES,0x00,//Input:Data,Array
                                           USB_HID_COLL_END//End Collection
                                    };

PRIVATE UINT32 uHid_KeyBoardDescConstructorAfterInterrupt(UINT8* buffer)
{
	UINT8* p_desc;
	UINT32 if_desc_index = 0;
	
	p_desc = COS_Malloc(sizeof(HAL_USB_HID_DES));
	((HAL_USB_HID_DES*)p_desc)->bLength = sizeof(HAL_USB_HID_DES);
	((HAL_USB_HID_DES*)p_desc)->bDescriptorType = 0x21;
	((HAL_USB_HID_DES*)p_desc)->bcdHID = 0x0100;
	((HAL_USB_HID_DES*)p_desc)->bCountryCode = 0x00;
	((HAL_USB_HID_DES*)p_desc)->bNumberDescriptor = 0x01;
	((HAL_USB_HID_DES*)p_desc)->bDescType = 0x22;
	((HAL_USB_HID_DES*)p_desc)->DescriptorLength = sizeof(KeyBoard_ReportDescriptor);
	  
    memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_HID_DES));
    if_desc_index += sizeof(HAL_USB_HID_DES);
    sxr_Free(p_desc);
	
	 p_desc = COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->size = sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->type = 0x05;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->ep = g_uhid_KeyBoardEp;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->attributes = 0x03;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->mps = 0x0009;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->interval = 0x0A;
	 memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	if_desc_index += sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	sxr_Free(p_desc);

    return if_desc_index;

}

PRIVATE HAL_USB_CALLBACK_RETURN_T
uhid_KeyboardInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
    //hal_HstSendEvent(USB_EVENT,0xfcfc9000);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:		
	    //hal_HstSendEvent(USB_EVENT,0xfcfc9001);		
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd\n");
		if(setup->request == 0x06)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e706);
		   //hal_HstSendEvent(USB_EVENT,setup->lenght);
		   //hal_HstSendEvent(USB_EVENT,sizeof(ReportDescriptor));
		   hal_UsbSend(0,KeyBoard_ReportDescriptor,sizeof(KeyBoard_ReportDescriptor),0);
           return (HAL_USB_CALLBACK_RETURN_OK);
		}
		if(setup->request == 0x0a)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e70a);
		}
		if(*((UINT8*)(setup)) == 0x21)
		{
		    hal_HstSendEvent(USB_EVENT,0x11220001);
            if(setup->request == 0x09)//Set_Report cmd
            {
//                hal_HstSendEvent(USB_EVENT,0x11220002);
                hal_HstSendEvent(USB_EVENT, ((setup->value)>>8)&0xff);
                //if((((setup->value)>>8)&0xff) == 0x02)
                if(((setup->value)>>8)&0x02)//Output
                {
//                    hal_HstSendEvent(USB_EVENT,0x11220003);
                    if(hal_UsbRecv(0, USBHIDKeyBoard_OUTPUT, setup->lenght, 0))
                    {
                        hal_HstSendEvent(USB_EVENT, 0x11220004);
                        hal_HstSendEvent(USB_EVENT, USBHIDKeyBoard_OUTPUT[0]);
                        hal_HstSendEvent(USB_EVENT, USBHIDKeyBoard_OUTPUT[1]);
                    }
                }
            }
		}
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
	 //hal_HstSendEvent(USB_EVENT,0xfcfc9002);
	 UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9003);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9004);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9005);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9006);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
	}	
    return(HAL_USB_CALLBACK_RETURN_KO); 
}



PRIVATE HAL_USB_CALLBACK_RETURN_T uhid_KeyboardCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
    UINT8 end = 0;
	static BOOL endflag = FALSE;
	//hal_HstSendEvent(USB_EVENT,0xb300000f);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000000); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000001); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
     //   uhidSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END);
        break;
    case HAL_USB_CALLBACK_TYPE_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000002); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000003); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
		//uhidSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END);
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000004); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000005); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}


PUBLIC void uhid_Keyboard_Open(void)
{
   HAL_USB_EP_DESCRIPTOR_T **epListHID;
   UINT8                     epNum;
   
   epListHID = (HAL_USB_EP_DESCRIPTOR_T**)
   COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);
   
   //USB class :0x03 HID class
   epNum = uctls_AllocateEndPoint();
   g_uhid_KeyBoardEp = HAL_USB_EP_DIRECTION_IN(epNum);
   epListHID[0] = uctls_NewEpDescriptor(g_uhid_KeyBoardEp,0x0a, uhid_KeyboardCallbackEp,HAL_USB_EP_TYPE_INTERRUPT);
   epListHID[1] = 0;
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_HID, 0x00, 0x01, 0x00,epListHID,0, uhid_KeyboardInterfaceCallback));
   uctls_AddInterface(uctls_NewCSInterfaceDescriptor(uHid_KeyBoardDescConstructorAfterInterrupt,0));
   
}

PUBLIC VOID uhid_Keyboard_SendKeycode(UINT8* keycode_pointer)
{   
   hal_UsbSend(g_uhid_KeyBoardEp,keycode_pointer,KEYBOARD_INPUT_SIZE+1,0);  
}
#endif


UINT8                        g_uhidEp;

/*USB HID report descriptor for audio control*/
#if 0
PRIVATE UINT8 ReportDescriptor[] = {
	                                USB_HID_USAGE_PAGE,0x0c,//Usage page:Consumer Device
									USB_HID_USAGE,0x01,//Usage:Consumer Contorl
									USB_HID_COLL_START,0x01,//Collection start
									  USB_HID_LOGICAL_MIN,0x00,//Logical Minimum:0
									  USB_HID_LOGICAL_MAX,0x01,//Logical Maximun:1
									  USB_HID_USAGE_PAGE,0x0c,//Usage page:Comsumer Device
									  USB_HID_USAGE,0xe9,//Usage:Volume Up 0x02
									  USB_HID_USAGE,0xea,//Usage:Volume Down 0x04
									  USB_HID_USAGE,0xe2,//Usage:Mute  0x01
									  USB_HID_USAGE,0xcd,//Usage:Play/Pause 0x08
                                      //USB_HID_USAGE,0xb1,//Usage:Pause 0x10
									  //USB_HID_USAGE,0xb5,//Usage:Next 0x20
									  //USB_HID_USAGE,0xb6,//Usage:Previous 0x40
									  USB_HID_REPORT_COUNT,0x04,//Report Count:7
									  USB_HID_REPORT_SIZE,0x01,//Report Size :1
									  USB_HID_INPUT_DES,0x42,//Input:data,var,abs,null
									  USB_HID_REPORT_COUNT,0x04,//Report Count:1
									  USB_HID_REPORT_SIZE,0x01,//Report Size:1
									  USB_HID_INPUT_DES,0x01,//Input:cnst,ary,abs
									USB_HID_COLL_END//End Collection
	                               };
#endif
UINT8 *ReportDescriptor;
UINT16 Desc_Length;

PRIVATE UINT32 uHidDescConstructorAfterInterrupt(UINT8* buffer)
{
	UINT8* p_desc;
	UINT32 if_desc_index = 0;
	
	p_desc = COS_Malloc(sizeof(HAL_USB_HID_DES));
	((HAL_USB_HID_DES*)p_desc)->bLength = sizeof(HAL_USB_HID_DES);
	((HAL_USB_HID_DES*)p_desc)->bDescriptorType = 0x21;
	((HAL_USB_HID_DES*)p_desc)->bcdHID = 0x0100;
	((HAL_USB_HID_DES*)p_desc)->bCountryCode = 0x00;
	((HAL_USB_HID_DES*)p_desc)->bNumberDescriptor = 0x01;
	((HAL_USB_HID_DES*)p_desc)->bDescType = 0x22;
	((HAL_USB_HID_DES*)p_desc)->DescriptorLength = Desc_Length;//sizeof(ReportDescriptor);
	  
    memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_HID_DES));
    if_desc_index += sizeof(HAL_USB_HID_DES);
    sxr_Free(p_desc);
	
	 p_desc = COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->size = sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->type = 0x05;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->ep = g_uhidEp;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->attributes = 0x03;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->mps = 0x0009;
	((HAL_USB_EP_DESCRIPTOR_REAL_T*)p_desc)->interval = 0x01;
	 memcpy(&buffer[if_desc_index], p_desc, sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T));
	if_desc_index += sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
	sxr_Free(p_desc);

    return if_desc_index;

}

volatile UINT8 reset_flag_hid= 0;
PRIVATE HAL_USB_CALLBACK_RETURN_T
uhidInterfaceCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
    //hal_HstSendEvent(USB_EVENT,0xfcfc9000);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:		
	    //hal_HstSendEvent(USB_EVENT,0xfcfc9001);		
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd\n");
		if(setup->request == 0x06)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e706);
		   //hal_HstSendEvent(USB_EVENT,setup->lenght);
		   //hal_HstSendEvent(USB_EVENT,sizeof(ReportDescriptor));
		   hal_UsbSend(0,ReportDescriptor,Desc_Length,0);
           reset_flag_hid= 1;
           return (HAL_USB_CALLBACK_RETURN_OK);
		}
		if(setup->request == 0x0a)
		{
		   //hal_HstSendEvent(USB_EVENT,0x7e90e70a);
		}
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
	 //hal_HstSendEvent(USB_EVENT,0xfcfc9002);
	 UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9003);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9004);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9005);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
	//hal_HstSendEvent(USB_EVENT,0xfcfc9006);
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
	}	
    return(HAL_USB_CALLBACK_RETURN_KO); 
}

PRIVATE void uhidSendEvent(HAL_USB_CALLBACK_EP_TYPE_T type)
{
    hal_HstSendEvent(USB_EVENT, 0x11210001);
    COS_EVENT ev;
    ev.nEventId = EV_USB_AUD_EVENT_ID;
    ev.nParam1 = type;
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

PRIVATE HAL_USB_CALLBACK_RETURN_T uhidCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
      //UINT8 end = 0;
	//static BOOL endflag = FALSE;
	//hal_HstSendEvent(USB_EVENT,0xb300000f);	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000000); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000001); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
        uhidSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END);
        break;
    case HAL_USB_CALLBACK_TYPE_CMD:
		//hal_HstSendEvent(USB_EVENT,0xb3000002); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
		//hal_HstSendEvent(USB_EVENT,0xb3000003); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
		//uhidSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END);
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000004); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
		//hal_HstSendEvent(USB_EVENT,0xb3000005); 
        UHID_TRACE(UHID_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}

PUBLIC VOID uhid_TransportEventProcess(UINT32 nParam1)
{
}

PUBLIC void uhid_Open(UINT8 *desc, UINT16 length)
{
   HAL_USB_EP_DESCRIPTOR_T **epListHID;
   UINT8                     epNum;
   
   epListHID = (HAL_USB_EP_DESCRIPTOR_T**)
   COS_Malloc(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*2);

    ReportDescriptor = desc;
    Desc_Length = length;
   
   //USB class :0x03 HID class
   epNum = uctls_AllocateEndPoint();
   g_uhidEp = HAL_USB_EP_DIRECTION_IN(epNum);
   epListHID[0] = uctls_NewEpDescriptor(g_uhidEp,0x0a, uhidCallbackEp,HAL_USB_EP_TYPE_INTERRUPT);
   epListHID[1] = 0;
   uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_HID, 0x00, 0x00, 0x00,epListHID,0, uhidInterfaceCallback));
   uctls_AddInterface((HAL_USB_INTERFACE_DESCRIPTOR_T *)uctls_NewCSInterfaceDescriptor(uHidDescConstructorAfterInterrupt,0));//shaohuima 2016/07/08
}

PUBLIC VOID uhid_Close(VOID)
{

}

volatile UINT32 keycode_input;
PUBLIC VOID uhid_SendKeycode(UINT8 *keycode, UINT8 length)
{
   hal_UsbSend(g_uhidEp,keycode,length,0);  
    keycode_input = *keycode;
  //  *((UINT8*)(0x01a80000+0x4000)) = *keycode;
}


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

