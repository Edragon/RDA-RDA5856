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
#ifndef _USBMSC_
#define _USBMSC_


//*****************************************************************************
// D E F I N E S
//*****************************************************************************

#define USBMSC_INTERFACE_CLASS      0x08

#define USBMSC_SUBCLASS_RBC         0x01
#define USBMSC_SUBCLASS_SFF8020i    0x02
#define USBMSC_SUBCLASS_QIC157      0x03
#define USBMSC_SUBCLASS_UFI         0x04
#define USBMSC_SUBCLASS_SFF8070i    0x05
#define USBMSC_SUBCLASS_SCSI        0x06

#define USBMSC_INTERFACE_PROTOCOL_CBIT  0x00
#define USBMSC_INTERFACE_PROTOCOL_CBT   0x01
#define USBMSC_INTERFACE_PROTOCOL_BOT   0x50




// Event types
#define MANUAL_RESET_EVENT TRUE
#define AUTO_RESET_EVENT   FALSE

#define MAX_DLL_LEN     64  // Max length of device driver DLL name

// Error codes
#define ERROR_SUCCESS			0x00
#define ERROR_PERSISTANT   		0x01
#define ERROR_INVALID_DRIVE		0x02
#define ERROR_ACCESS_DENIED		0x03
#define ERROR_INVALID_PARAM     0x04
#define ERROR_FAILED			0x05
#define ERROR_NO_RESOURCE		0x06
#define ERROR_TIMEOUT			0x07

#if 0
#define ERROR_GEN_FAILURE				-50
#define ERROR_INVALID_HANDLE			-51
#define ERROR_NOT_READY					-52
#define DISK_REMOVED_ERROR				-53
#define ERROR_UNRECOGNIZED_MEDIA		-54
#define ERROR_DISK_OPERATION_FAILED		-55
#define ERROR_FLOPPY_UNKNOWN_ERROR		-56
#define ERROR_INSUFFICIENT_BUFFER		-57
#define ERROR_WRITE_PROTECT				-58
#else
#define ERROR_GEN_FAILURE				0x09
#define ERROR_INVALID_HANDLE			0x0a
#define ERROR_NOT_READY					0x0b
#define DISK_REMOVED_ERROR				0x0c
#define ERROR_UNRECOGNIZED_MEDIA		0x0d
#define ERROR_DISK_OPERATION_FAILED		0x0e
#define ERROR_FLOPPY_UNKNOWN_ERROR		0x0f
#define ERROR_INSUFFICIENT_BUFFER		0x10
#define ERROR_WRITE_PROTECT				0x11

#endif

// Max LUN index
#define MAX_LUN     0x7


//*****************************************************************************
// T Y P E D E F S
//*****************************************************************************

// Direction Flags
#define DATA_OUT 0x00000000
#define DATA_IN  0x00000001

// Command Block
typedef struct _TRANSPORT_COMMAND 
{
    uint32 Flags;        // IN - DATA_IN or DATA_OUT
    uint32 Timeout;      // IN - Timeout for this command Block
    uint32 Length;       // IN - Length of the command block buffer
    uint32 dwLun;        // IN - Logical Number for Logic Device.
    void* CommandBlock; // IN - Pointer to the command block buffer.
} TRANSPORT_COMMAND, *PTRANSPORT_COMMAND;

//
// Data Block
//
typedef struct _TRANSPORT_DATA_BUFFER {
    uint32 RequestLength;  // IN  - Requested Length
    uint32 TransferLength; // OUT - Returns number of bytes actually transferred 
    void*  DataBlock;      // IN  - Pointer to the data buffer. May be NULL.
} TRANSPORT_DATA, *PTRANSPORT_DATA;


//*****************************************************************************
//
// F U N C T I O N    P R O T O T Y P E S
//
//*****************************************************************************

/*++

UsbDiskTransportInit:
 
    This routine is called by the USB Mass Storage Class driver (i.e., Transport)
    when a USB Disk is enumerated according to the device's bInterfaceSubClass.

 hTransport:
    An opaque handle to the USB Transport.

 pHardwareKey:
    Pointer to the hardware registry key.

 bInterfaceSubClass: 
    USB Disk SubClass as reported in the USB descriptor.

 Returns:
    If successful the USB Disk driver returns a pointer to it's
    context used in subsequent UsbDiskXxx calls. Else, NULL

--*/
 
HANDLE UsbDiskTransportInit(void * configuration, uint8 ulConfigIndex, uint8 *pLun);
void UsbDiskTransportDeinit(HANDLE hTransport);


/*++

UsbsDataTransfer:
   Called by the USB Disk driver to place the block command on the USB.
   Used for reads, writes, commands, etc.

 hTransport:
   The Transport handle passed to DiskAttach.

 pCommand:
    Pointer to Command Block for this transaction.

 pData:
    Pointer to Data Block for this transaction. May be NULL.

--*/
uint32 UsbsDataTransfer(HANDLE hTransport, PTRANSPORT_COMMAND pCommand, PTRANSPORT_DATA pData);


//
// Get a DWORD Value from an array of chars pointed to by pbArray
// (Little Endian)
//
uint32 GetDWORD(uint8 * pbArray);

//
// Set a DWORD Value into an array of chars pointed to by pbArray
// (Little Endian)
//
void SetDWORD(uint8*  pbArray, uint32  dwValue);

void SetWORD(uint8* pBytes,  uint16 wValue );

#endif // _USBMSC_

// EOF
