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
#include <cswtype.h>
#include <errorcode.h>
#include <ts.h>
#include "cs_types.h"
#include "chip_id.h"
#include "event.h"
#include "ts.h"
#include "sxr_tls.h"
#include "sxr_ops.h"

#include "usb.h"
#include "usb_hcd.h"
#include "usbmscp.h"

#include "bot.h"
#include "cbit.h"
#include "event.h"
#include "global_macros.h"
#include "hal_host.h"
#include "cos.h"

#define USB_ENDPOINT_DIRECTION_OUT(address)	(!(address&0x80))
#define USB_ENDPOINT_DIRECTION_IN(address)	(address&0x80)

#define USB_ENDPOINT_TYPE_MASK		0x03
#define USB_ENDPOINT_TYPE_BULK 		0x02
#define USB_ENDPOINT_TYPE_INTERRUPT	0x03


//*****************************************************************************
//
// F U N C T I O N    P R O T O T Y P E S
//
//*****************************************************************************
uint32 g_NumDevices = 0;
volatile int UsbCmdFinished = 0;

static USBMSC_DEVICE gUsbmscInst;
extern void * memset(void *,int,size_t);
USBINTERFACEDESC * ParseUsbDescriptors(void * configuration, uint8 *pnInterfaceNum);

uint32 GetMaxLUN(HANDLE hTransport, uint8 *pLun);

BOOL SetUsbInterface(PUSBMSC_DEVICE pUsbDevice, uint8 nInterfaceNum);
void RemoveDeviceContext(PUSBMSC_DEVICE pUsbDevice);

void UsbControlComplete(void *pData, uint8 status)
{
	UsbCmdFinished = 1;
}

HANDLE UsbDiskTransportInit(void * configuration, uint8 ulConfigIndex, uint8 *pLun)
{
	BOOL bRc;
	PUSBMSC_DEVICE   	pUsbDevice = NULL;
	USBINTERFACEDESC *pUsbInterface = NULL;
	uint8 	bInterfaceSubClass;
	//uint8 	bTempInterfaceSubClass = 0xFF; // invalid subclass
	uint8 	uMaxLun = 0;
	uint32  	uiIndex; // Working Unit.
	uint8 	uNumInterface = 0;

	do 
	{
		// Parse USB Descriptors
		pUsbInterface = ParseUsbDescriptors(configuration, &uNumInterface);

		hal_HstSendEvent(SYS_EVENT,0x08290001);
		hal_HstSendEvent(SYS_EVENT,(UINT32) pUsbInterface);
		if ( !pUsbInterface ) 
		{
			bRc = FALSE;
			break;
		}

		// we found a device, interface, & protocol we can control, so create our device context
		pUsbDevice = &gUsbmscInst;      //(PUSBMSC_DEVICE)sxr_Malloc( sizeof(USBMSC_DEVICE) );
		/*
		if ( !pUsbDevice )
		{
			bRc = FALSE;
			break;
		}
		*/
		memset(pUsbDevice, 0, sizeof(USBMSC_DEVICE));

		pUsbDevice->Sig = USBMSC_SIG;

		pUsbDevice->pUsbInterface = pUsbInterface;
		pUsbDevice->ConfigIndex   = (uint16)ulConfigIndex;

		pUsbDevice->Flags.AcceptIo      = TRUE;
		pUsbDevice->Flags.DeviceRemoved = FALSE;

		pUsbDevice->dwMaxLun=0;

		bRc = SetUsbInterface(pUsbDevice, uNumInterface);
		if( bRc == FALSE)
		{
			return bRc;
		}

		// use defaults
		pUsbDevice->Timeouts.Reset         = RESET_TIMEOUT;
		pUsbDevice->Timeouts.CommandBlock  = COMMAND_BLOCK_TIMEOUT;
		pUsbDevice->Timeouts.CommandStatus = COMMAND_STATUS_TIMEOUT;


		if (!pUsbDevice->Timeouts.Reset)
			pUsbDevice->Timeouts.Reset = RESET_TIMEOUT;

		if (!pUsbDevice->Timeouts.Reset)
			pUsbDevice->Timeouts.CommandBlock = COMMAND_BLOCK_TIMEOUT;

		if (!pUsbDevice->Timeouts.Reset)
			pUsbDevice->Timeouts.CommandStatus = COMMAND_STATUS_TIMEOUT;

		//
		// Load the USB Disk Driver based on the bInterfaceSubClass code.
		// The USB Disk Driver is named by convention USBDISKxx.DLL, 
		// where 'xx' is a valid bInterfaceSubClass code.
		//
		// To override the default disk driver stuff the replacement driver subkey in the registry.
		// If the named subkey does not exist then retry with SCSI as the default driver.
		//
		bInterfaceSubClass = pUsbDevice->pUsbInterface->bInterfaceSubClass;
		ASSERT( (bInterfaceSubClass >= USBMSC_SUBCLASS_RBC) && (bInterfaceSubClass <= USBMSC_SUBCLASS_SCSI) );


		// signal we can take I/O
		pUsbDevice->Flags.AcceptIo = TRUE;

		// Call the driver's DiskAttach.
		// DiskAttach returns non-null Context on success, else null.
		for (uiIndex=0; uiIndex<MAX_LUN; uiIndex++)
		{
			pUsbDevice->DiskContext[uiIndex]=NULL;
		}
		uMaxLun=0;
		// Get Max LUN.for Mass Storage Host no need Get Max LUN byshaohuima
		/*
		if (GetMaxLUN((HANDLE)pUsbDevice, &uMaxLun)!=ERROR_SUCCESS)
		{
			uMaxLun=1; // Using 1 as default;
		}*/

		pUsbDevice->dwMaxLun= uMaxLun;
		if( pLun ) 
		{
			*pLun = uMaxLun;
		}
			
		ASSERT(pUsbDevice->dwMaxLun>=1);


	} while(0);

	if (!bRc) 
	{
		// If not our device, or error, then clean up
		RemoveDeviceContext( pUsbDevice );
		return NULL;
	} 

	return (HANDLE)pUsbDevice;
	
}

void UsbDiskTransportDeinit(HANDLE hTransport)
{
	if(hTransport)
	{
		PUSBMSC_DEVICE pUsbDevice = (PUSBMSC_DEVICE)hTransport;
		RemoveDeviceContext(pUsbDevice);
		pUsbDevice->Flags.AcceptIo = 0;
		pUsbDevice->Flags.DeviceRemoved = 1;
	}
}


USBINTERFACEDESC *ParseUsbDescriptors(void *configuration, uint8 *pnInterfaceNum)
{
	uint32 dwNumInterfaces;
	uint32 dwIndex;
	USBINTERFACEDESC *pUsbInterface, *pDesiredInterface = NULL;
	USBCONFIGURATIONDESC *pConfigDes = (USBCONFIGURATIONDESC*)configuration;

	if (!configuration) 
	{
		return NULL;
	}

	pUsbInterface = (USBINTERFACEDESC*)(pConfigDes+1);
	 *pnInterfaceNum = dwNumInterfaces = pConfigDes->bNumInterfaces;

	// walk the interfaces searching for best fit
	for ( dwIndex = 0; dwIndex < dwNumInterfaces; pUsbInterface++, dwIndex++) 
	{
		if (  pUsbInterface->bInterfaceClass == USBMSC_INTERFACE_CLASS &&
		(pUsbInterface->bInterfaceSubClass >= USBMSC_SUBCLASS_RBC && pUsbInterface->bInterfaceSubClass <= USBMSC_SUBCLASS_SCSI) &&
		(pUsbInterface->bInterfaceProtocol == USBMSC_INTERFACE_PROTOCOL_CBIT || pUsbInterface->bInterfaceProtocol == USBMSC_INTERFACE_PROTOCOL_CBT || pUsbInterface->bInterfaceProtocol == USBMSC_INTERFACE_PROTOCOL_BOT) )
		{
			 // if we do not already have an interface, or the selected Protocol is not Bulk-Only
			// (I personally prefer Bulk-Only since it is well defined)  
			if ( !pDesiredInterface || pDesiredInterface->bInterfaceProtocol != USBMSC_INTERFACE_PROTOCOL_BOT)
			{
				pDesiredInterface = pUsbInterface;
			}
		}
	}

	return pDesiredInterface;
	
}


BOOL SetUsbInterface(PUSBMSC_DEVICE pUsbDevice, uint8 nInterfaceNum)
{
	BOOL bRc = FALSE;
	uint32 dwIndex = 0;
	uint32 dwErr = 0;
	USBENDPOINTDESC* pEndpoint = NULL;

	if ( !MSC_VALID_CONTEXT( pUsbDevice ) || !pUsbDevice->pUsbInterface ) 
	{
		return FALSE;
	}

	if( nInterfaceNum > 1)
	{
		dwErr = USB_SetInterface(pUsbDevice->pUsbInterface->bAlternateSetting, pUsbDevice->pUsbInterface->bInterfaceNumber, UsbControlComplete);

		while(1)
		{
			COS_Sleep(2);
			if( UsbCmdFinished > 0)
				break;
		}
		UsbCmdFinished = 0;
	}	
	
	hal_HstSendEvent(SYS_EVENT,0x08290009);
	hal_HstSendEvent(SYS_EVENT,(UINT32) pUsbDevice->pUsbInterface);
	pEndpoint =(USBENDPOINTDESC*)(pUsbDevice->pUsbInterface + 1);
	
	// now parse the endpoints
	for ( dwIndex = 0; dwIndex < pUsbDevice->pUsbInterface->bNumEndpoints; dwIndex++) 
	{
		hal_HstSendEvent(SYS_EVENT,(UINT32) pEndpoint);
		
		// Mass Storage Class supports 1 mandatory Bulk OUT, 1 mandatory Bulk IN, and 1 optional INTERRUPT
		if ( USB_ENDPOINT_DIRECTION_OUT(pEndpoint->bEndpointAddress ) )
		{ 
			if ( NULL == pUsbDevice->BulkOut.hPipe &&
			(pEndpoint->bmAttributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_TYPE_BULK) 
			{
				// create the Bulk OUT pipe
				pUsbDevice->BulkOut.hPipe = USB_OpenPipe(pEndpoint);
				if ( !pUsbDevice->BulkOut.hPipe )
				{
					bRc = FALSE;
					break;
				}
				pUsbDevice->BulkOut.bIndex         = pEndpoint->bEndpointAddress;
				pUsbDevice->BulkOut.wMaxPacketSize = pEndpoint->wMaxPacketSize;
			}
		}
		else if (USB_ENDPOINT_DIRECTION_IN(pEndpoint->bEndpointAddress ) ) 
		{ 		
			if ( NULL == pUsbDevice->BulkIn.hPipe && 
			(pEndpoint->bmAttributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_TYPE_BULK) 
			{
				// create the Bulk IN pipe
				pUsbDevice->BulkIn.hPipe = USB_OpenPipe(pEndpoint);
				if ( !pUsbDevice->BulkIn.hPipe )
				{
					bRc = FALSE;
					break;
				}
				pUsbDevice->BulkIn.bIndex         = pEndpoint->bEndpointAddress;
				pUsbDevice->BulkIn.wMaxPacketSize = pEndpoint->wMaxPacketSize;

			}
			else if ( NULL == pUsbDevice->Interrupt.hPipe &&
				(pEndpoint->bmAttributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_TYPE_INTERRUPT) 
			{
				// create the Interrupt pipe
				pUsbDevice->Interrupt.hPipe = USB_OpenPipe(pEndpoint);
				if ( !pUsbDevice->Interrupt.hPipe ) 
				{
					bRc = FALSE;
					break;
				}

				pUsbDevice->Interrupt.bIndex         = pEndpoint->bEndpointAddress;
				pUsbDevice->Interrupt.wMaxPacketSize = pEndpoint->wMaxPacketSize;
			}

		}
		else 
		{

		}
		pEndpoint = pEndpoint+1;
	}

	// did we find our endpoints?
	bRc = (pUsbDevice->BulkOut.hPipe && pUsbDevice->BulkIn.hPipe) ? TRUE : FALSE;
	switch (pUsbDevice->pUsbInterface->bInterfaceProtocol) 
	{
	case USBMSC_INTERFACE_PROTOCOL_CBIT:
		// CBI Transport 3.4.3: *shall* support the interrupt endpoint
		bRc &= (pUsbDevice->Interrupt.hPipe) ? TRUE : FALSE;
		break;
	case USBMSC_INTERFACE_PROTOCOL_CBT:
		break;

	case USBMSC_INTERFACE_PROTOCOL_BOT:
		break;

	default:
		bRc = FALSE;
		break;
	}

	// if we failed to find all of our endpoints then cleanup will occur later
	return (bRc);
	
}


void ClosePipe(PUSBMSC_DEVICE pUsbDevice, PPIPE pPipe)
{
    if ( !pUsbDevice || !pPipe) 
	{
        return;
    }

    return;
}


void RemoveDeviceContext(PUSBMSC_DEVICE pUsbDevice)
{

	if ( MSC_VALID_CONTEXT( pUsbDevice ) ) 
	{
		if ( pUsbDevice->Flags.AcceptIo ) 
		{
			return;
		}

		ClosePipe( pUsbDevice, &pUsbDevice->BulkOut );
		ClosePipe( pUsbDevice, &pUsbDevice->BulkIn );
		ClosePipe( pUsbDevice, &pUsbDevice->Interrupt );

		if (pUsbDevice->hEP0Event)
		{
		}

		if (&pUsbDevice->Lock) 
		{

		}

	} 
	else 
	{

	}

	return;
}


//*****************************************************************************
//
//  U S B M S C      I N T E R F A C E
//
//*****************************************************************************


/*++

UsbsDataTransfer:
   Called by the USB Disk driver to place the block command on the USB.
   Used for reads, writes, commands, etc.

 hTransport:
   The Transport handle passed to DiskAttach.

 pCommand:
    Pointer to Command Block for this transaction.

 pData:
    Pointer to Data Block for this transaction. May be NULL

--*/
uint32 UsbsDataTransfer(HANDLE hTransport, PTRANSPORT_COMMAND pCommand, PTRANSPORT_DATA pData)
{
	PUSBMSC_DEVICE pUsbDevice = (PUSBMSC_DEVICE)hTransport;
	uint32 dwErr = ERROR_SUCCESS;
			
	if ( hTransport && pCommand )
	{
		if ( MSC_ACCEPT_IO(pUsbDevice) ) 
		{
			if (pData)
			{
				// preset the number of bytes transferred
				pData->TransferLength = 0; 
			}

			// MUX the Transport Protocol
			switch ( pUsbDevice->pUsbInterface->bInterfaceProtocol ) 
			{
			case USBMSC_INTERFACE_PROTOCOL_CBIT:
				dwErr = CBIT_DataTransfer( pUsbDevice, pCommand, pData, (pCommand->Flags & DATA_IN));
				break;

			case USBMSC_INTERFACE_PROTOCOL_CBT:
				dwErr = CBIT_DataTransfer( pUsbDevice, pCommand, pData, (pCommand->Flags & DATA_IN));
				break;

			case USBMSC_INTERFACE_PROTOCOL_BOT:
				dwErr = BOT_DataTransfer( pUsbDevice, pCommand, pData, (pCommand->Flags & DATA_IN));
				break;

			default:
				dwErr = ERROR_INVALID_DRIVE;
				break;
			}

		} 
		else
		{
			dwErr = ERROR_ACCESS_DENIED;
		}

	} 
	else 
	{
		dwErr = ERROR_INVALID_PARAM;
	}

	return dwErr;
}

uint32 GetMaxLUN(HANDLE hTransport, uint8* pLun)
{
	PUSBMSC_DEVICE pUsbDevice = (PUSBMSC_DEVICE)hTransport;
	uint32 dwErr = ERROR_INVALID_PARAM;

	if ( hTransport  && pLun  && MSC_ACCEPT_IO(pUsbDevice)) 
	{
		switch ( pUsbDevice->pUsbInterface->bInterfaceProtocol)
		{
		case USBMSC_INTERFACE_PROTOCOL_BOT:
			dwErr = BOT_GetMaxLUN( pUsbDevice, pLun);
			//hal_HstSendEvent(SYS_EVENT,0x08290008);
			//hal_HstSendEvent(SYS_EVENT,*pLun);
			break;
		default:
			*pLun = 1;
			break;
		}
	}
	return dwErr;
}


#if 0

BOOL
UsbDeviceTest(
   PUSBMSC_DEVICE  pUsbDevice
   )
{
#if 1
    UNREFERENCED_PARAMETER(pUsbDevice);
#else
    DWORD dwErr;

    //
   // set stall on EP0
   //
   dwErr = ClearOrSetFeature( pUsbDevice->UsbFuncs,
                              pUsbDevice->hUsbDevice,
                              DefaultTransferComplete,
                              pUsbDevice->hEP0Event,
                              USB_SEND_TO_ENDPOINT,
                              USB_FEATURE_ENDPOINT_STALL,
                              0, // bIndex
                              1000,
                              TRUE );

   if ( ERROR_SUCCESS != dwErr ) {
      DEBUGMSG( ZONE_ERR, (TEXT("SetFeature error:%d\n"), dwErr ));
      TEST_TRAP();
   }

   //
   // clear EP0
   //
   ResetDefaultEndpoint(pUsbDevice->UsbFuncs, pUsbDevice->hUsbDevice);

   //
   // set stall on the BulkOut endpoint
   //
   dwErr = ClearOrSetFeature( pUsbDevice->UsbFuncs,
                              pUsbDevice->hUsbDevice,
                              DefaultTransferComplete,
                              pUsbDevice->hEP0Event,
                              USB_SEND_TO_ENDPOINT,
                              USB_FEATURE_ENDPOINT_STALL,
                              pUsbDevice->BulkOut.bIndex,
                              1000,
                              TRUE );
   
   if ( ERROR_SUCCESS != dwErr ) {
      DEBUGMSG( ZONE_ERR, (TEXT("SetFeature error:%d\n"), dwErr));
      TEST_TRAP();
   }
   
   //
   // clear it
   //
   dwErr = ClearOrSetFeature( pUsbDevice->UsbFuncs,
                              pUsbDevice->hUsbDevice,
                              DefaultTransferComplete,
                              pUsbDevice->hEP0Event,
                              USB_SEND_TO_ENDPOINT,
                              USB_FEATURE_ENDPOINT_STALL,
                              pUsbDevice->BulkOut.bIndex,
                              1000,
                              FALSE );
   
   if ( ERROR_SUCCESS != dwErr ) {
      DEBUGMSG( ZONE_ERR, (TEXT("ClearFeature error:%d\n"), dwErr));
      TEST_TRAP();
   }


   //
   // set stall on the BulkIn endpoint
   //
   dwErr = ClearOrSetFeature( pUsbDevice->UsbFuncs,
                              pUsbDevice->hUsbDevice,
                              DefaultTransferComplete,
                              pUsbDevice->hEP0Event,
                              USB_SEND_TO_ENDPOINT,
                              USB_FEATURE_ENDPOINT_STALL,
                              pUsbDevice->BulkIn.bIndex,
                              1000,
                              TRUE );

   if ( ERROR_SUCCESS != dwErr ) {
      DEBUGMSG( ZONE_ERR, (TEXT("SetFeature error:%d\n"), dwErr));
      TEST_TRAP();
   }

   //
   // clear it
   //
   dwErr = ClearOrSetFeature( pUsbDevice->UsbFuncs,
                              pUsbDevice->hUsbDevice,
                              DefaultTransferComplete,
                              pUsbDevice->hEP0Event,
                              USB_SEND_TO_ENDPOINT,
                              USB_FEATURE_ENDPOINT_STALL,
                              pUsbDevice->BulkIn.bIndex,
                              1000,
                              FALSE );

   if ( ERROR_SUCCESS != dwErr ) {
      DEBUGMSG( ZONE_ERR, (TEXT("ClearFeature error:%d\n"), dwErr ));
      TEST_TRAP();
   }

#endif // 0

   return TRUE;
}

#endif

// EOF
