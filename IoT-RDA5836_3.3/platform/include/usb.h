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
#ifndef USB_H
#define USB_H

#ifdef __cplusplus
extern "C"
{
#endif

// bitmap of field bmRequestType in USBSTDREQUEST
//directtion bit 7
#define USB_REQUEST_HOST_TO_DEVICE	0x00
#define USB_REQUEST_DEVICE_TO_HOST	0x80		

//type bit[6-5]
#define USB_REQUEST_STANDARD		0x00
#define USB_REQUEST_CLASS			0x20		
#define USB_REQUEST_VENDOR			0x60

//recipient, bit[4-0]
#define USB_REQUEST_FOR_DEVICE		0x00
#define USB_REQUEST_FOR_INTERFACE	0x01
#define USB_REQUEST_FOR_ENDPOINT	0x02
#define USB_REQUEST_FOR_OTHER		0x03

//Standard USB Request Codes
enum 
{
	USB_GET_STATUS = 0,
	USB_CLEAR_FEATURE,
	RESERVED1,
	USB_SET_FEATURE,
	RESERVED2,
	USB_SET_ADDRESS,
	USB_GET_DESCRIPTOR,
	USB_SET_DESCRIPTOR,
	USB_GET_CONFIGURATION,
	USB_SET_CONFIGURATION,
	USB_GET_INTERFACE,
	USB_SET_INTERFACE,
	USB_SYNCH_FRAME,
};

//USB descriptor type
enum 
{
	USB_DES_DEVICE = 1,
	USB_DES_CONFIGURATION,
	USB_DES_STRING,
	USB_DES_INTERFACE,
	USB_DES_ENDPOINT,
	USB_DES_DEVICE_QUALIFIER,
	USB_DES_OTHER_SPEED_CONFIGURATION,
	USB_DES_INTERFACE_POWER
};

typedef struct tagUsbStdRequest
{
	uint8	bmRequestType;
	uint8  	bRequest;
	uint16 	wValue;
	uint16 	wIndex;
	uint16 	wLength;
}__attribute__((packed)) USBSTDREQUEST;

typedef struct tagUsbDeviceDesc
{
	uint8	bLength;
	uint8	bDescriptorType;
	uint16	bcdUSB;
	uint8	bDeviceClass;
	uint8	bDeviceSubClass;
	uint8	bDeviceProtocol;
	uint8	bMaxPacketSize0;
	uint16 	idVendor;
	uint16 	idProduct;
	uint16 	bcdDevice;
	uint8	iManufacturer;
	uint8	iProduct;
	uint8	iSerialNumber;
	uint8	bNumConfigurations;
}__attribute__((packed)) USBDEVICEDESC;


typedef struct tagUsbConfigurationDesc
{
	uint8	bLength;
	uint8	bDescriptorType;
	uint16 	wTotalLength;
	uint8	bNumInterfaces;
	uint8	bConfigurationValue;
	uint8	iConfiguration;
	uint8	bmAttributes;
	uint8	bMaxPower;
}__attribute__((packed)) USBCONFIGURATIONDESC;

typedef struct tagUsbInterfaceDesc
{
	uint8	bLength;
	uint8	bDescriptorType;
	uint8	bInterfaceNumber;
	uint8	bAlternateSetting;
	uint8	bNumEndpoints;
	uint8	bInterfaceClass;
	uint8  	bInterfaceSubClass;
	uint8  	bInterfaceProtocol;
	uint8 	iInterface;
}__attribute__((packed)) USBINTERFACEDESC;

typedef struct tagUsbEndpointDesc
{
	uint8	bLength;
	uint8	bDescriptorType;
	uint8	bEndpointAddress;
	uint8	bmAttributes;
	uint16 	wMaxPacketSize;
	uint8	bInterval;
}__attribute__((packed)) USBENDPOINTDESC;


#ifdef __cplusplus
};
#endif


#endif





