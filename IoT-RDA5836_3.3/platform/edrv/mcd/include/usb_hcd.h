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
#ifndef USB_HCD_H
#define USB_HCD_H

#define USB_HCD_DEVICE_DETACHED			0x00000001

#define USB_IN_TRANSFER			0x00000001
#define USB_NO_WAIT				0x00000002
#define USB_SHORT_TRANSFER_OK 	0x00000004
#define USB_OUT_TRANSFER			0x00000008



#define USB_NO_ERROR			0x00000000
#define USB_STALL_ERROR			0x00000001
#define USBHCD_INVALID_PARAM	0x00000002
#define USBHCD_NO_RESOURCE		0x00000003
#define USBHCD_ERROR_FAILED		0x00000004
#define USB_RECEIVED_ACK		0x00000005
#define USB_RECEIVED_NAK		0x00000006

#define USBHCD_DEVICE_DISCONNECTED	0x00000007

typedef int USB_PIPE;

typedef struct tagUsbControlRequest
{
	USBSTDREQUEST 	stdRequest;
	uint8* 	pBuf;
	uint32	nBufLen;
	void 	(*completeCb)(void*pData, uint8 status);
}USBCONTROLREQUEST;

typedef struct tagUsbBulkRequest
{
	USB_PIPE 	pipe;
	uint8*  	pBuf;
	uint32* 	pnByteTransferred;
	uint32		nBufLen;
	uint32 		*pstatus;
	HANDLE		handle;
	void		(*completeCb)(HANDLE handle, uint8 status);
}USBBULKREQUEST;

typedef struct tagUsbOperation
{
	USB_PIPE (*OpenPipe)(USBENDPOINTDESC *pEndDes);
	int	(*ControlTransfer)(USBCONTROLREQUEST* ctrlReq);
	int (*BulkTransfer)(USBBULKREQUEST *bulkReq);
	int (*ResetDefaultEndpoint)();
	int (*InterruptTransfer)();
}USBOPERATION;

typedef struct tagUSBHCD
{
	uint32 flag;
	uint8  address;
	USBOPERATION usbops;
	USBCONTROLREQUEST	ctrlReq;
	USBBULKREQUEST		bulkReq;
	USBDEVICEDESC	*pdeviceDesc;
	USBCONFIGURATIONDESC *pconfig;			/*configuration itself*/
	USBCONFIGURATIONDESC *pConfigDesc[3];  /* this include configuration descriptor, interfaces's descriptor, endpoints's descriptor */
}USBHCD;


USBOPERATION *USB_HcdInit();
 
int USB_SetAddress(void (*Cb)(void*pData, uint8 status));

int USB_GetDescriptor(uint8 type, uint16 wValue, uint16 wIndex, void(*Cb)(void *pData, uint8 status));

int USB_SetConfiguration(uint8 bValue, void (*Cb)(void*pData, uint8 status));

int USB_BulkOrInterruptTransfer(USB_PIPE pipe, void* pBuf, uint32 BufLen, uint32* pnBytesTransferred, uint32 timeout);

int USB_VendorTransfer(USBSTDREQUEST *pStdRequest, void *pBuf, uint32* pnDataTransferred, uint32 timeout);

int USB_ResetEndpoint();

int USB_SetInterface(uint16 bAlternateSetting, uint16 Interface, void (*Cb)(void*pData, uint8 status));

USB_PIPE USB_OpenPipe(USBENDPOINTDESC *pEndDes);

int USB_ResetDefaultEndpoint();
int USB_HcdDeinit();
int RDA_UsbHostInit(USBOPERATION *pUsbOps, void (*InitCb)(void*pData, uint8 status));



#endif



