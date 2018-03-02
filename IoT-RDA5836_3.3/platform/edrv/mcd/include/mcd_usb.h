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


////////////////////////////////////////////////////////////////////////////////
/// @file mcd_m.h                                                             //
////////////////////////////////////////////////////////////////////////////////


#ifndef _MCD_USB_H_
#define _MCD_USB_H_

#include "cs_types.h"

/// @file mcd_usb.h

/// This document describes the characteristics of the  USB Disk 
/// Driver and how to use it via its API. 

#ifdef __cplusplus
extern "C"
{
#endif

#define MCDUSB_SUCCESS	0x00000000
#define MCDUSB_FAILED	0x00000001


// =============================================================================
//  Functions
// =============================================================================

// =============================================================================
// mcd_SetCardDetectHandler
// -----------------------------------------------------------------------------
/// Register a handler for card detection
///
/// @param handler function called when insertion/removal is detected.
// =============================================================================
PUBLIC MCD_ERR_T mcd_SetUSBDiskDetectHandler(MCD_CARD_DETECT_HANDLER_T handler);


typedef struct tagUSBClientDriver
{
	int	InterfaceClass;
	int InterfaceSubClass;
	void (*UsbDeviceAttach)(void *configuration, uint8 index);
	void (*UsbDeviceDetach)(void*);
}USBCLIENTDRIVER;

int Mcd_RegisterClientDriver(USBCLIENTDRIVER *pdriverInst);


#ifdef __cplusplus
};
#endif



/// @} <- End of the mmc group

#endif // _MCD_USB_H_

