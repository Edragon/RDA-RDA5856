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
//
/// @file bootp_usb.h
///
/// Protected function for usb
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_USB_H_
#define _BOOTP_USB_H_

#include "cs_types.h"
#include "hal_usb.h"

#include "global_macros.h"
#include "usbc.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================

#define EPIN       0x01
#define EPOUT      0x02
#define EPINOUT    0x00

#define EPDIR(n)   (((hwp_usbc->GHWCFG1)>>(2*n))&0x03)

#define SINGLE     0
#define INCR       1
#define INCR4      3
#define BOOT_USB_AHB_MODE    INCR4


#define RXFIFOSIZE 64
#define TXFIFOSIZE 48

#ifndef USB_MAX_STRING
#define USB_MAX_STRING 8
#endif /* USB_MAX_STRING */



// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// BOOT_USB_REQUEST_DESTINATION_T
// -----------------------------------------------------------------------------
/// Destination of USB command
// =============================================================================
typedef enum
{
    BOOT_USB_REQUEST_DESTINATION_DEVICE    = 0,
    BOOT_USB_REQUEST_DESTINATION_INTERFACE = 1,
    BOOT_USB_REQUEST_DESTINATION_EP        = 2
} BOOT_USB_REQUEST_DESTINATION_T ;

// =============================================================================
// BOOT_USB_REQUEST_DEVICE_T
// -----------------------------------------------------------------------------
/// List of device request
// =============================================================================
typedef enum
{
    BOOT_USB_REQUEST_DEVICE_SETADDR        = 0x05,
    BOOT_USB_REQUEST_DEVICE_GETDESC        = 0x06,
    BOOT_USB_REQUEST_DEVICE_SETCONF        = 0x09
} BOOT_USB_REQUEST_DEVICE_T ;

// =============================================================================
// BOOT_USB_REQUEST_EP_T
// -----------------------------------------------------------------------------
/// List of EndPoint request
// =============================================================================
typedef enum
{
    BOOT_USB_REQUEST_EP_GET_STATUS         = 0x00,
    BOOT_USB_REQUEST_EP_CLEAR_FEATURE      = 0x01,
    BOOT_USB_REQUEST_EP_SET_FEATURE        = 0x03
} BOOT_USB_REQUEST_EP_T ;

// =============================================================================
// BOOT_USB_DEVICE_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Device descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT16 bcdUsb;
    UINT8  usbClass;
    UINT8  usbSubClass;
    UINT8  usbProto;
    UINT8  ep0Mps;
    UINT16 vendor;
    UINT16 product;
    UINT16 release;
    UINT8  iManufacturer;
    UINT8  iProduct;
    UINT8  iSerial;
    UINT8  nbConfig;
} PACKED BOOT_USB_DEVICE_DESCRIPTOR_REAL_T;

// =============================================================================
// BOOT_USB_CONFIG_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Config descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT16 totalLength;
    UINT8  nbInterface;
    UINT8  configIndex;
    UINT8  iDescription;
    UINT8  attrib;
    UINT8  maxPower;
}  PACKED BOOT_USB_CONFIG_DESCRIPTOR_REAL_T;

// =============================================================================
// BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Interface descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT8  interfaceIndex;
    UINT8  alternateSetting;
    UINT8  nbEp;
    UINT8  usbClass;
    UINT8  usbSubClass;
    UINT8  usbProto;
    UINT8  iDescription;
} PACKED BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T;

// =============================================================================
// BOOT_USB_EP_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Ep descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT8  ep;
    UINT8  attributes;
    UINT16 mps;
    UINT8  interval;
} PACKED BOOT_USB_EP_DESCRIPTOR_REAL_T;

// =============================================================================
// EP0_STATE_T
// -----------------------------------------------------------------------------
/// State of ep0
// =============================================================================
typedef enum
{
    EP0_STATE_IDLE,
    EP0_STATE_IN,
    EP0_STATE_OUT,
    EP0_STATE_STATUS_IN,
    EP0_STATE_STATUS_OUT
} EP0_STATE_T ;

// =============================================================================
// BOOT_USB_TRANSFERT_T
// -----------------------------------------------------------------------------
/// Structure containt the transfert parameter
// =============================================================================
typedef struct
{
	BOOL  transferDone;
    	INT32  sizeRemaining;
    	INT32  sizeTransfered;
	UINT8 *buffer;
} BOOT_USB_TRANSFERT_T;

// =============================================================================
// BOOT_USB_GLOBAL_VAR_T
// -----------------------------------------------------------------------------
/// Structure with all global var
// =============================================================================
typedef struct
{
    HAL_USB_GETDESCRIPTOR_CALLBACK_T DeviceCallback;
    HAL_USB_DEVICE_DESCRIPTOR_T*     Desc;
    HAL_USB_CALLBACK_T               EpInCallback [DIEP_NUM];
    HAL_USB_CALLBACK_T               EpOutCallback[DOEP_NUM];
    UINT32                           EpFlag;
    UINT32                           EpInterruptMask;
    UINT8*                           String       [USB_MAX_STRING];

    // Transfert data
    BOOT_USB_TRANSFERT_T             Transfert [DIEP_NUM+DOEP_NUM+1];

    // Ep0 State
    HAL_USB_SETUP_REQUEST_DESC_T     RequestDesc;
    UINT8                            Ep0State;
    UINT8                            Ep0Index;

    UINT8                            Config;
    UINT8                            NbEp;
    UINT8                            NbInterface;
    UINT8                            NbString;
    UINT8					FifoAddr;
    UINT8					configData;
    UINT8 					vbuserr_retry;
    UINT8					Varvbuserr_retry;
    UINT8					port1_status;
    UINT8 					usbAddr;
} BOOT_USB_GLOBAL_VAR_T;

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

EXPORT PROTECTED BOOT_USB_GLOBAL_VAR_T g_bootUsbVar;

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

// =============================================================================
// boot_UsbInitVar
// -----------------------------------------------------------------------------
/// Init global variable
// =============================================================================
PROTECTED VOID boot_UsbInitVar(VOID);

/// FIXME TODO Comment and document functions.
PROTECTED UINT8 boot_UsbStartTransfert(UINT8 ep, VOID *data, UINT16 size,
                                     UINT32 flag);
PROTECTED VOID boot_UsbCancelTransfert(UINT8 ep);
PROTECTED UINT8 boot_UsbContinueTransfert(UINT8 ep);
PROTECTED VOID boot_UsbResetTransfert(VOID);

#endif // _BOOTP_USB_H_
