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
/// @file uctls_m.h
///                                                                             
/// USB Controler description
//                                                                              
////////////////////////////////////////////////////////////////////////////////

#ifndef _UCTLS_M_H_
#define _UCTLS_M_H_

#include "cs_types.h"

#include "hal_usb.h"

#include "uctls_callback.h"

/// @file uctls_m.h
/// @mainpage USB Controller service
///
/// This service provides very high level usb interface for select the usb
/// service
///
/// This document is composed of:
/// - @ref uctls
///
/// @defgroup uctls USB Controller service (uctls)
/// @{
///

// =============================================================================
// MACROS                                                                       
// =============================================================================

#ifndef UCTLS_SERVICE_LIST_SIZE
#define UCTLS_SERVICE_LIST_SIZE 4
#endif /* UCTLS_SERVICE_LIST_SIZE */

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// UCTLS_ID_T
// -----------------------------------------------------------------------------
/// USB Mode ID
// =============================================================================
typedef enum
{
    /// Mode Trace
    UCTLS_ID_TRACE,
    /// Mode 1
    UCTLS_ID_1,//mass storage
    /// Mode 2
    UCTLS_ID_2,//video
    /// Mode 3
    UCTLS_ID_3
} UCTLS_ID_T;

// =============================================================================
// UCTLS_SERVICE_T
// -----------------------------------------------------------------------------
/// Usb Service
// =============================================================================
typedef struct
{
    // Service Callback
    CONST UCTLS_SERVICE_CALLBACK_T* serviceCallback;
    // Service configuration
    CONST UCTLS_SERVICE_CFG_T       serviceCfg;
} UCTLS_SERVICE_T;

// =============================================================================
// UCTLS_SERVICE_LIST_T
// -----------------------------------------------------------------------------
/// List of services
// =============================================================================
typedef struct
{
    CONST UCTLS_SERVICE_T servicesList[UCTLS_SERVICE_LIST_SIZE];
} UCTLS_SERVICE_LIST_T;

// =============================================================================
// UCTLS_CHARGER_STATUS_T
// -----------------------------------------------------------------------------
/// Charger status
// =============================================================================
typedef enum
{
    UCTLS_CHARGER_STATUS_CONNECTED,
    UCTLS_CHARGER_STATUS_DISCONNECTED,
} UCTLS_CHARGER_STATUS_T;

// =============================================================================
// UCTLS_BODY_MSG_T
// -----------------------------------------------------------------------------
/// Describe body of uctls message
// =============================================================================
typedef enum
{
    UCTLS_MSG_USB_DEVICE_CONNECTED,
    UCTLS_MSG_USB_DEVICE_DISCONNECTED
} UCTLS_BODY_MSG_T ;

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

// =============================================================================
// uctls_Open
// -----------------------------------------------------------------------------
/// Open USB controler
/// @param mbx Mailbox use to transmit the usb event. Message header is
/// #UCTLS_HEADER_ID and body is describe #UCTLS_BODY_MSG_T.
/// @param usbVendor Id vendor defined by usb.org. If the value is 0, the value
/// is 0x1E04 (Coolsand vendor Id) also you can rewrite th1is value with this
/// parameter.
/// @param usbProduct Id product. If the value is different of 0 you rewrite the
/// default value. This feature is use for specific case.
/// @param usbVersion Version of device. If the value is different of 0 you
/// rewrite the default value. This feature is use for specific case.
/// @param usbDescription Description of device. Can be NULL
// =============================================================================
PUBLIC VOID uctls_Open(UINT16                   usbVendor,
                       UINT16                   usbProduct,
                       UINT16                   usbVersion,
                       UINT8*                   usbSerialNumber);


PUBLIC VOID uctls_AddInterface(HAL_USB_INTERFACE_DESCRIPTOR_T *interface);

PUBLIC UINT8 uctls_AllocateEndPoint(VOID);

// =============================================================================
// uctls_Close
// -----------------------------------------------------------------------------
/// Close USB controler
// =============================================================================
PUBLIC VOID uctls_Close(VOID);

// =============================================================================
// uctls_ChargerStatus
// -----------------------------------------------------------------------------
/// Use by MMI to change power status. Is used to enable or disable USB
/// @param status Define the current charger status
// =============================================================================
PUBLIC VOID uctls_ChargerStatus(UCTLS_CHARGER_STATUS_T status);

// =============================================================================
// uctls_NewEpDescriptor
// -----------------------------------------------------------------------------
/// Create a endpoint descriptor
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param interval Polling intervale
/// @param callback Endpoint callback. See #HAL_USB_CALLBACK_EP_CMD_T.
/// @param type Endpoint type. See #HAL_USB_EP_TYPE_T.
/// @return Endpoint descriptor
// =============================================================================
PUBLIC HAL_USB_EP_DESCRIPTOR_T*
uctls_NewEpDescriptor(UINT8 ep, UINT8 interval, HAL_USB_CALLBACK_T callback, HAL_USB_EP_TYPE_T type);

// =============================================================================
// uctls_NewCmdEpDescriptor
// -----------------------------------------------------------------------------
/// Create a command endpoint descriptor
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param callback Endpoint callback. See #HAL_USB_CALLBACK_EP_CMD_T.
/// @return Endpoint descriptor
// =============================================================================
//PUBLIC HAL_USB_EP_DESCRIPTOR_T*
//uctls_NewCmdEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback);

// =============================================================================
// uctls_NewBulkEpDescriptor
// -----------------------------------------------------------------------------
/// Create a bulk endpoint descriptor
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param callback Endpoint callback. See #HAL_USB_CALLBACK_EP_CMD_T.
/// @param type Endpoint type. See #HAL_USB_EP_TYPE_T.
/// @return Endpoint descriptor
// =============================================================================
//PUBLIC HAL_USB_EP_DESCRIPTOR_T*
//uctls_NewBulkEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback, HAL_USB_EP_TYPE_T type);

// =============================================================================
// uctls_NewInterruptEpDescriptor
// -----------------------------------------------------------------------------
/// Create a Interrupt endpoint descriptor
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param interval Polling intervale
/// @param callback Endpoint callback. See #HAL_USB_CALLBACK_EP_CMD_T.
/// @return Endpoint descriptor
// =============================================================================
//PUBLIC HAL_USB_EP_DESCRIPTOR_T*
//uctls_NewInterruptEpDescriptor(UINT8 ep, UINT8 interval,
//                               HAL_USB_CALLBACK_T callback);

// =============================================================================
// uctls_NewIsoEpDescriptor
// -----------------------------------------------------------------------------
/// Create a ISO endpoint descriptor
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param callback Endpoint callback. See #HAL_USB_CALLBACK_EP_CMD_T.
/// @return Endpoint descriptor
// =============================================================================
//PUBLIC HAL_USB_EP_DESCRIPTOR_T*
//uctls_NewIsoEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback);

// =============================================================================
// uctls_NewInterfaceDescriptor
// -----------------------------------------------------------------------------
/// Create a interface descriptor
/// @param usbClass Interface class defined by usb.org
/// @param usbSubClass Interface subclass defined by usb.org
/// @param usbProto Interface protocol
/// @param interfaceIdx Interface index. If 0 the interface index is generate 
/// by HAL
/// @param epList List of endpoint linked of interface. The list must finish
/// with NULL pointer
/// @param description Interface description
/// @param callback interface callback. See #HAL_USB_CALLBACK_INTERFACE_CMD_T.
/// @return Interface descriptor
// =============================================================================
PUBLIC HAL_USB_INTERFACE_DESCRIPTOR_T*
uctls_NewInterfaceDescriptor(UINT8                            usbClass,
                             UINT8                            usbSubClass,
                             UINT8                            usbProto,
                             UINT8                            interfaceIdx,
                             HAL_USB_EP_DESCRIPTOR_T**        epList,
                             UINT8*                           description,
                             HAL_USB_CALLBACK_T               callback);

// =============================================================================
// uctls_NewConfigDescriptor
// -----------------------------------------------------------------------------
/// Create a configuration descriptor
/// @param maxPower Power use in configuration 1 unit = 2mA
/// @param attributes Describe on #HAL_USB_CONFIG_ATTRIB_T
/// @param interfaceList List of interface linked of interface. The list must
/// finish with NULL pointer see #uctls_NewInterfaceDescriptor
/// @param description Configuration description
/// @return Configuration descriptor
// =============================================================================
PUBLIC HAL_USB_CONFIG_DESCRIPTOR_T*
uctls_NewConfigDescriptor(UINT8 maxPower, UINT8 attributes,
                          HAL_USB_INTERFACE_DESCRIPTOR_T**    interfaceList,
                          UINT8*                              description);

// =============================================================================
// uctls_NewDeviceDescriptor
// -----------------------------------------------------------------------------
/// Create a device descriptor
/// @param usbClass Device usb class. If equal 0 the class is describe in
/// interface
/// @param usbSubClass Device usb subclass
/// @param usbProto Device protocol
/// @param vendor VendorID
/// @param product ProductID
/// @param version Version of product (format BCD)
/// @param configList All config include in device
/// #see uctls_NewConfigDescriptor
/// @param serialNumber Serial number
/// @param manufacturer Manufacturer name
/// @return Device descriptor
// =============================================================================
PUBLIC HAL_USB_DEVICE_DESCRIPTOR_T*
uctls_NewDeviceDescriptor(UINT8  usbClass, UINT8  usbSubClass,
                          UINT8  usbProto, UINT16 vendor,
                          UINT16 product,  UINT16 version,
                          HAL_USB_CONFIG_DESCRIPTOR_T* configList[],
                          UINT8*                       serialnumber,
                          UINT8*                       manufacturer);

// =============================================================================
// uctls_CleanDeviceDescritor
// -----------------------------------------------------------------------------
/// Free all memory allocated for device and sub structure
/// @param dev Descriptor of the usb device
// =============================================================================
PUBLIC VOID
uctls_CleanDeviceDescriptor(HAL_USB_DEVICE_DESCRIPTOR_T* dev);

// =============================================================================
// uctls_CleanInterfaceListDescritor
// -----------------------------------------------------------------------------
/// Free all memory allocated for interface list and sub structure
/// @param dev Descriptor of the usb interface list
// =============================================================================
PUBLIC VOID
uctls_CleanInterfaceListDescriptor(HAL_USB_INTERFACE_DESCRIPTOR_T** interfaces);

// =============================================================================
// uctls_CopyInterfaceDescritor
// -----------------------------------------------------------------------------
/// Copy a interface descriptor
/// @param interface interface must be copy
/// @return copy of the interface
// =============================================================================
PUBLIC HAL_USB_INTERFACE_DESCRIPTOR_T*
uctls_CopyInterfaceDescriptor(CONST HAL_USB_INTERFACE_DESCRIPTOR_T* interface);

///  @} <- End of the uctls group


#endif // _UCTLS_M_H_
