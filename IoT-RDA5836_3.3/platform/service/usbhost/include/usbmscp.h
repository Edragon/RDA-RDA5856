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
#ifndef _USBMSCP_ 
#define _USBMSCP_

//*****************************************************************************
// I N C L U D E S
//*****************************************************************************
#include "usb.h"
#include "usbmsc.h"


//*****************************************************************************
// D E F I N E S
//*****************************************************************************

#define USBMSC_SIG 0x43534d55   //'UMSC'

#define MAX_USB_BULK_LENGTH 0x2000L // 8k

// default timeout values   ,  in ms
#define RESET_TIMEOUT           4000
#define COMMAND_BLOCK_TIMEOUT   4000
#define COMMAND_STATUS_TIMEOUT  4000


#define ONE_FRAME_PERIOD		(16384)

// TBD: may want this in the registry
#define MAX_INT_RETRIES 3


// Is the context pointer valid
//&& USBMSC_SIG == p->Sig
#define MSC_VALID_CONTEXT( p ) \
   ( (p != NULL) )

// Can the device accept any I/O requests
#define MSC_ACCEPT_IO( p ) \
   ( MSC_VALID_CONTEXT( p ) && \
     p->Flags.AcceptIo && \
    !p->Flags.DeviceRemoved )

//*****************************************************************************
// T Y P E D E F S
//*****************************************************************************
extern uint32 g_NumDevices;

//
// Device Flags
//
typedef struct _MSCDEVICE_FLAGS {
    //
    // The device is initialized and ready to accept I/O
    //
    uint8   AcceptIo : 1;

    //
    // The device has been removed
    //
    uint8   DeviceRemoved : 1;

    //
    // The device claims CBIT protocol,
    // but does not use the Interrupt pipe
    //
    uint8   IgnoreInterrupt : 1;

} MSCDEVICE_FLAGS, *PMSCDEVICE_FLAGS;


// Our notion of a Pipe
typedef struct _PIPE
{
    // USB Pipe handle received from the stack
    USB_PIPE hPipe;

    // Endpoint's Address
    uint8    bIndex;

    // Endpoint's wMaxPacketSize
    uint16 wMaxPacketSize;
} PIPE, *PPIPE;


// Reg configurable Timeouts
typedef struct _MSCTIMEOUTS {

    uint32 Reset;

    uint32 CommandBlock;

    uint32 CommandStatus;

} MSCTIMEOUTS, *PMSCTIMEOUTS;


typedef struct _USBMSC_DEVICE 
{
    // device context Signature
    uint32 Sig;

    HANDLE  Lock; 

    // USB handle to our device
    HANDLE   hUsbDevice;
    uint32    Index;

    // Fields from USB_INTERFACE that we need
    USBINTERFACEDESC * pUsbInterface;

    // USB Configutation Index
    uint16 ConfigIndex;

    // Bulk OUT Pipe
    PIPE BulkOut;

    // Bulk IN Pipe
    PIPE BulkIn;

    // Interrupt Pipe
    PIPE  Interrupt;
    uint8 InterruptErrors;

    // completion signal used for endpoint 0
    HANDLE hEP0Event;

    // Multi-Lun Support.
    uint32 dwMaxLun;
    void* DiskContext[MAX_LUN];
	
    // FLAGS
    MSCDEVICE_FLAGS Flags;

    MSCTIMEOUTS Timeouts;

    uint32 dwCurTag;

} USBMSC_DEVICE, *PUSBMSC_DEVICE;


#endif // _USBMSCP_

// EOF
