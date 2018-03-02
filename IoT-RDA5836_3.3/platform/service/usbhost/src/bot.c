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

#include <cswtype.h>
#include <errorcode.h>
#include <ts.h>
#include "cs_types.h"
#include "ts.h"
#include "sxr_tls.h"
#include "sxr_ops.h"

#include "usb.h"
#include "usb_hcd.h"
#include "usbmscp.h"
#include "bot.h"
#include "event.h"
#include "hal_host.h"
#define min(a,b)	((a)<(b)?(a):(b))
extern BOOL COS_SleepByTick (UINT32 Period);
extern void * memcpy(void *, const void *, size_t);
extern  void * memset(void *,int,size_t);
BOOL BOT_ResetAllPipes(PUSBMSC_DEVICE pUsbDevice);

// 3.1 Bulk-Only Mass Storage Reset.
// This class-specific command shall ready the device for the next CBW.
BOOL BOT_MassStorageReset(PUSBMSC_DEVICE pUsbDevice)
{
	USBSTDREQUEST ControlHeader;
	uint32 dwBytesTransferred = 0;
	uint32 dwErr = 0, dwUsbErr = 0;
	BOOL bRc = FALSE;
	uint8 count = 0;

    if ( MSC_ACCEPT_IO(pUsbDevice) ) 
	{
        ControlHeader.bmRequestType = USB_REQUEST_HOST_TO_DEVICE | USB_REQUEST_CLASS | USB_REQUEST_FOR_INTERFACE;
        ControlHeader.bRequest = 255;
        ControlHeader.wValue = 0;
        ControlHeader.wIndex = pUsbDevice->pUsbInterface->bInterfaceNumber;
        ControlHeader.wLength = 0;
_retry:
        dwErr = USB_VendorTransfer(&ControlHeader, NULL, &dwBytesTransferred, pUsbDevice->Timeouts.Reset);

		if ( ERROR_SUCCESS == dwErr && USB_NO_ERROR == dwUsbErr ) 
		{
			bRc = TRUE;
		}
		else if( dwErr == USBHCD_DEVICE_DISCONNECTED )
		{
			//hal_HstSendEvent(SYS_EVENT,0x10140001);
			pUsbDevice->Flags.DeviceRemoved = 1;
			pUsbDevice->Flags.AcceptIo = 0;
		}
		else if (ERROR_SUCCESS == USB_ResetDefaultEndpoint() ) 
		{
			if (++count < MAX_BOT_STALL_COUNT) 
			{
				COS_SleepByTick(ONE_FRAME_PERIOD);
				goto _retry;
			}
		}
	}

	return bRc;
}


// 3.2 Get Max LUN
uint32 BOT_GetMaxLUN(PUSBMSC_DEVICE pUsbDevice, uint8* pLun)
{
	USBSTDREQUEST ControlHeader;
	uint32 	dwBytesTransferred = 0;
	uint8   bMaxLun = 0xff;
	uint32 	dwErr = 0, dwUsbErr = 0;
	//BOOL 	bRc = FALSE;

	if ( MSC_ACCEPT_IO(pUsbDevice) &&  pLun )
	{
		ControlHeader.bmRequestType = USB_REQUEST_DEVICE_TO_HOST | USB_REQUEST_CLASS | USB_REQUEST_FOR_INTERFACE;
		ControlHeader.bRequest = 254;
		ControlHeader.wValue = 0;
		ControlHeader.wIndex = pUsbDevice->pUsbInterface->bInterfaceNumber;
		ControlHeader.wLength = 1;

		dwErr = USB_VendorTransfer(&ControlHeader, &bMaxLun, &dwBytesTransferred, pUsbDevice->Timeouts.CommandBlock);
		
		hal_HstSendEvent(SYS_EVENT,0x08290007);
		if( USB_NO_ERROR != dwUsbErr )
		{     
		 	USB_ResetDefaultEndpoint();           
		}
		else
		{
		    if (0 <= bMaxLun && bMaxLun < MAX_LUN) 
			{
		        *pLun = bMaxLun + 1;
		    }
		    else 
			{
		       	*pLun = 1;
		    }
		}

	} 

	return dwErr;
}


//
// 5.3.4 Reset Recovery
//
BOOL BOT_ResetRecovery(PUSBMSC_DEVICE pUsbDevice)
{
	BOOL bRc = FALSE;
	uint32 dwErr;

	// 0. reset endpoint 0
	dwErr = USB_ResetDefaultEndpoint();

	if ( ERROR_SUCCESS == dwErr ) 
	{
		// 1. send class command
		BOT_MassStorageReset(pUsbDevice);

		// 2. reset BulkIn endpoint
		dwErr = USB_ResetEndpoint(pUsbDevice->BulkIn.hPipe, pUsbDevice->Timeouts.Reset);

		if ( ERROR_SUCCESS == dwErr) 
		{
			// 3. reset BulkOut endpoint
			dwErr = USB_ResetEndpoint(pUsbDevice->BulkOut.hPipe, pUsbDevice->Timeouts.Reset);

			if ( ERROR_SUCCESS == dwErr ) 
			{
				bRc = TRUE;
			}
		}
	} 
	return bRc;
}



//
// Reset All Pipes
//
BOOL BOT_ResetAllPipes( PUSBMSC_DEVICE pUsbDevice )
{
	BOOL bRc = FALSE;
	uint32 dwErr;

	// 0. reset endpoint 0
	dwErr = USB_ResetDefaultEndpoint();

	if ( USB_NO_ERROR == dwErr ) 
	{
		// 1. reset BulkIn endpoint
		if ( USB_ResetEndpoint(pUsbDevice->BulkIn.hPipe, USB_NO_WAIT) == USB_NO_ERROR) 
		{
			// 2. reset BulkOut endpoint
			if ( USB_ResetEndpoint(pUsbDevice->BulkOut.hPipe, USB_NO_WAIT) == USB_NO_ERROR) 
			{               
				bRc = TRUE;
			}
		}
	}

	return bRc;
}


//
// See 5.3 Data Transfer Conditions
//
//static HAL_UNCACHED_BSS uint8 csw[16];

uint32 BOT_DataTransfer(PUSBMSC_DEVICE pUsbDevice, PTRANSPORT_COMMAND pCommand, PTRANSPORT_DATA pData, BOOL Direction )
{
	// Command Block Wrapper
	CBW *Cbw = (CBW*)COS_Malloc(sizeof(CBW));
	// Command Status Wrapper
	CSW *Csw = (CSW*)COS_Malloc_NoCache(sizeof(CSW));//csw
	
	uint32 dwCbwSize = sizeof(CBW);
	uint32 dwCswSize = sizeof(CSW);

	uint32 dwBytesTransferred = 0;   
	uint32 dwErr = USB_NO_ERROR;
	uint32 dwResetErr, dwCswErr;

	BOOL  bRc = FALSE;
	uint8 ucStallCount;

	// parameter check
	if ( !pUsbDevice || !pCommand || !pCommand->CommandBlock || pCommand->Length > MAX_CBWCB_SIZE )
	{
		dwErr = ERROR_INVALID_PARAM;
              COS_Free(Csw);
              COS_Free(Cbw);
		return dwErr;
	}

	if ( !MSC_ACCEPT_IO(pUsbDevice) )
	{
		dwErr = ERROR_ACCESS_DENIED;
              COS_Free(Csw);
              COS_Free(Cbw);
		return dwErr;
	}

	// We require exclusive entry into the transport.
	// we could implement command queuing.
	if ( !BOT_ResetAllPipes(pUsbDevice) )
	{
		dwErr = ERROR_GEN_FAILURE;
		goto BOT_SendCommandDone;
	}

	ucStallCount = 0;

	pUsbDevice->dwCurTag++;

	memset( Cbw, 0, dwCbwSize );
	memset( Csw, 0, dwCswSize );

	// build the active CBW  
	Cbw->dCBWSignature = CBW_SIGNATURE;
	Cbw->dCBWTag = pUsbDevice->dwCurTag;    
	Cbw->dCBWDataTransferLength = pData ? pData->RequestLength : 0;

	if (Direction) 
	{
		Cbw->bmCBWFlags |= 0x80;
	}

	Cbw->bCBWLUN = (uint8)(pCommand->dwLun & 0xf); // TBD
	Cbw->bCBWCBLength = (uint8)pCommand->Length;
	memcpy( Cbw->CBWCB, pCommand->CommandBlock, pCommand->Length);

	// 5.3.1 Command Block Transport
	dwErr = USB_BulkOrInterruptTransfer(pUsbDevice->BulkOut.hPipe, Cbw, dwCbwSize, &dwBytesTransferred, pUsbDevice->Timeouts.CommandBlock);
	
	if ( USB_NO_ERROR != dwErr || dwBytesTransferred != dwCbwSize ) 
	{
		hal_HstSendEvent(SYS_EVENT,0x09090061);
		bRc = BOT_ResetRecovery(pUsbDevice);
		goto BOT_SendCommandDone;
	}
	//hal_HstSendEvent(SYS_EVENT,0x09150010);
	
	// 5.3.2 Data Transport
	if (pData && pData->DataBlock && pData->RequestLength) 
	{
		//const uint16 wMaxPacketSize = Direction ? pUsbDevice->BulkIn.wMaxPacketSize : pUsbDevice->BulkOut.wMaxPacketSize;
		// we don't provide our own phys buffer so are limited by USBD's max buffer size
		const uint32  dwMaxBlock = MAX_USB_BULK_LENGTH;
		void* pDataBlock = pData->DataBlock;
		uint32 dwTransferLength = 0;
		uint32 dwRequestLength = 0;
		uint32 dwDataLength = pData->RequestLength;
		uint32 dwTimeout = pCommand->Timeout;
		//uint32 dwStartTime=0;
		
		while(dwDataLength) 
		{
			dwRequestLength = min(dwMaxBlock, dwDataLength);
			dwTransferLength = 0;
			
			ASSERT( dwRequestLength <= MAX_USB_BULK_LENGTH );

			dwErr = USB_BulkOrInterruptTransfer(
			       Direction ? pUsbDevice->BulkIn.hPipe : pUsbDevice->BulkOut.hPipe,
			       pDataBlock,
			       dwRequestLength,
			      &dwTransferLength,
			       dwTimeout);

			// 6.7.2.3: the host shall accept the data received
			if ( (USB_NO_ERROR == dwErr || USB_STALL_ERROR == dwErr) ) 
			{
				// adjust transfer length & buffer
				ASSERT(dwTransferLength <= dwRequestLength);
				pData->TransferLength += dwTransferLength;
				dwDataLength -= dwTransferLength;
				pDataBlock = (void*)((uint8*)pDataBlock + dwTransferLength);
			}

			if ( USB_NO_ERROR != dwErr )
			{

				//uint8 bIndex = Direction ? pUsbDevice->BulkIn.bIndex : pUsbDevice->BulkOut.bIndex;

				// 6.7.x.3: the host shall clear the Bulk pipe

				dwDataLength = 0;

				// test/reset Bulk endpoint
				dwResetErr = USB_ResetEndpoint((Direction ? pUsbDevice->BulkIn.hPipe : pUsbDevice->BulkOut.hPipe), pUsbDevice->Timeouts.Reset);

				if (ERROR_SUCCESS != dwResetErr) 
				{
					bRc = BOT_ResetRecovery(pUsbDevice);

					if (ERROR_SUCCESS == dwErr)
						dwErr = ERROR_GEN_FAILURE;
					goto BOT_SendCommandDone;
				}
			}
		}
	}

	// 5.3.3 Command Status Transport (CSW)
	ucStallCount = 0; // reset Stall count

_RetryCSW:

	//hal_HstSendEvent(SYS_EVENT,0x09150001);
	dwCswErr = USB_BulkOrInterruptTransfer(pUsbDevice->BulkIn.hPipe, Csw, dwCswSize, &dwBytesTransferred, pUsbDevice->Timeouts.CommandStatus);

	// Figure 2 - Status Transport Flow
	if ( USB_NO_ERROR != dwCswErr ) 
	{
		if( dwCswErr == USBHCD_DEVICE_DISCONNECTED )
		{
			goto BOT_SendCommandDone;
		}
		else
		{
			// reset BulkIn endpoint
			dwResetErr = USB_ResetEndpoint(pUsbDevice->BulkIn.hPipe, pUsbDevice->Timeouts.Reset );

			if ( ERROR_SUCCESS == dwResetErr && ++ucStallCount < MAX_BOT_STALL_COUNT ) 
			{
				COS_SleepByTick(ONE_FRAME_PERIOD);

				goto _RetryCSW;

			}
			else 
			{
				bRc = BOT_ResetRecovery(pUsbDevice);

				if (ERROR_SUCCESS == dwErr)
					dwErr = ERROR_GEN_FAILURE;

				goto BOT_SendCommandDone;
			}
		}
	}

	// Validate CSW...
	// 6.3.1 Valid CSW: size
	if ( dwBytesTransferred != dwCswSize )
	{ 
		hal_HstSendEvent(SYS_EVENT,0x09090025);
		// 6.5 Host shall perform ResetRecovery for invalid CSW
		bRc = BOT_ResetRecovery(pUsbDevice);

		if (ERROR_SUCCESS == dwErr)
			dwErr = ERROR_GEN_FAILURE;

		goto BOT_SendCommandDone;
	}

//	memcpy(&Csw, HAL_SYS_GET_UNCACHED_ADDR(&Csw), sizeof(CSW));

	// 6.3.1 Valid CSW: Signature
	if ( CSW_SIGNATURE != Csw->dCSWSignature) 
	{ 
		hal_HstSendEvent(SYS_EVENT,0x09090026);

		// 6.5 Host shall perform ResetRecovery for invalid CSW
		bRc = BOT_ResetRecovery(pUsbDevice);

		if (ERROR_SUCCESS == dwErr)
			dwErr = ERROR_GEN_FAILURE;

		//goto BOT_SendCommandDone;
	}

	// 6.3.1 Valid CSW: Tags
	if ( Cbw->dCBWTag != Csw->dCSWTag ) 
	{
		hal_HstSendEvent(SYS_EVENT,0x09090027);
		// 6.5 Host shall perform ResetRecovery for invalid CSW
		bRc = BOT_ResetRecovery(pUsbDevice);

		if (ERROR_SUCCESS == dwErr)
			dwErr = ERROR_GEN_FAILURE;

		goto BOT_SendCommandDone;
	}

	// Command Status?
	if ( 0 == Csw->bCSWStatus || 1 == Csw->bCSWStatus )
	{
		//hal_HstSendEvent(SYS_EVENT,0x09090028);
		if (pData && pData->TransferLength) 
		{
			pData->TransferLength = Cbw->dCBWDataTransferLength - Csw->dCSWDataResidue;
		}

		if (1 == Csw->bCSWStatus )
		{
			dwErr = ERROR_GEN_FAILURE;
		}

		goto BOT_SendCommandDone;
	}

	// Phase Error?
	if ( 2 == Csw->bCSWStatus )
	{
		hal_HstSendEvent(SYS_EVENT,0x09090029);
		// ignore the dCSWDataResidue
		bRc = BOT_ResetRecovery(pUsbDevice);

		if (ERROR_SUCCESS == dwErr)
			dwErr = ERROR_GEN_FAILURE;
	}
	

BOT_SendCommandDone:
	 COS_Free(Csw);
       COS_Free(Cbw);
       //hal_HstSendEvent(USB_EVENT, 0x20170804);
	return dwErr;
}


//
// Set a uint32 Value into an array of chars pointed to by pbArray
// (Little Endian)
//
void SetDWORD(uint8* pbArray, uint32 dwValue) 
{
	if(pbArray)
	{
		pbArray[3] = (uint8)dwValue;
		dwValue >>= 8;
		pbArray[2] = (uint8)dwValue;
		dwValue >>= 8;
		pbArray[1] = (uint8)dwValue;
		dwValue >>= 8;
		pbArray[0] = (uint8)dwValue;
	}
	return;
}

void SetWORD(uint8*  pBytes, uint16 wValue )
{
	if (pBytes)
	{
		pBytes[1] = (uint8)wValue;
		wValue >>= 8;
		pBytes[0] = (uint8)wValue;
	}
	return;
}


//
// Gets a uint16 Value from an array of chars pointed to by pbArray.
// The return value is promoted to uint32.
// (Little Endian)
//
uint32 GetDWORD(uint8*  pbArray)
{
	uint32 dwReturn=0;
	if (pbArray) 
	{
		dwReturn=*(pbArray++);
		dwReturn=dwReturn*0x100 + *(pbArray++);
		dwReturn=dwReturn*0x100 + *(pbArray++);
		dwReturn=dwReturn*0x100 + *(pbArray++);
	}
	return dwReturn;
}

// EOF
