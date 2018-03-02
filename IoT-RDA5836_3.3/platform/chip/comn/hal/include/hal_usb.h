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
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_usb.h
///
/// Hal USB public function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_USB_H_
#define _HAL_USB_H_

#include "cs_types.h"

#include "chip_id.h"

#ifndef DONGLE_HOST
//#define DONGLE_HOST
#endif
/// @defgroup usb HAL USB Driver
/// This driver provide an interface to use the USB bus. You can use this
/// driver on interrupt mode (default) or polling mode with the function
/// #hal_UsbEpEnableInterrupt. The driver manage the EP0 on interrupt only
///
/// @par
/// The API allows to write or read several bytes using
/// #hal_UsbSend or #hal_UsbRecv.
///
/// @par
/// You transmit a callback function when you open the driver with #hal_UsbOpen.
/// This callback is call when the usb enum phase is finish and this function
/// must return a description of usb device
///
/// @{

// =============================================================================
// MACROS                                                                       
// =============================================================================

#define HAL_USB_EP_DIRECTION  0x80

// =============================================================================
// HAL_USB_EP_DIRECTION_IN
// -----------------------------------------------------------------------------
/// This macro set the direction IN on an endpoint
// =============================================================================
#define HAL_USB_EP_DIRECTION_IN(X)    ((X)|HAL_USB_EP_DIRECTION)

// =============================================================================
// HAL_USB_EP_DIRECTION_OUT
// -----------------------------------------------------------------------------
/// This macro set the direction OUT on an endpoint
// =============================================================================
#define HAL_USB_EP_DIRECTION_OUT(X)   ((X)&~HAL_USB_EP_DIRECTION)

// =============================================================================
// HAL_USB_IS_EP_DIRECTION_IN
// -----------------------------------------------------------------------------
/// This macro define if the direction is IN
// =============================================================================
#define HAL_USB_IS_EP_DIRECTION_IN(X) ((X)&HAL_USB_EP_DIRECTION)

// =============================================================================
// HAL_USB_EP_NUM
// -----------------------------------------------------------------------------
/// This macro return the endpoint index
// =============================================================================
#define HAL_USB_EP_NUM(X)             ((X)&(~HAL_USB_EP_DIRECTION))

// =============================================================================
// HAL_USB_MPS_T
// -----------------------------------------------------------------------------
/// Mas Packet Size
// =============================================================================
#define HAL_USB_MPS        64
#define HAL_USB_ISO_IN_MPS 180
#define HAL_USB_ISO_OUT_MPS 192
#define HAL_USB_MPS_HS		512

// =============================================================================
// USB classes
// =============================================================================
#define HAL_USB_CLASS_AUDIO 0x01
#define HAL_USB_CLASS_HID   0x03
#define HAL_USB_CLASS_MSS   0x08

// =============================================================================
// TYPES                                                                        
// =============================================================================

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
#define USB_UCBSS_INTERNAL __attribute__((aligned(4), section(".sramucbss")))
#else
#define USB_UCBSS_INTERNAL __attribute__((aligned(4), section(".ucbss")))
#endif

typedef enum
{
   HAL_USB_MODE_MASS = 0x01,    // mass storage device.
   HAL_USB_MODE_AUDIO = 0x02    // usb audio device.
} HAL_USB_MODE_T;
// =============================================================================
// HAL_USB_CALLBACK_EP_TYPE_T
// -----------------------------------------------------------------------------
/// Type of callback
// =============================================================================
typedef enum
{
    /// Event send when a command is receive
    HAL_USB_CALLBACK_TYPE_CMD,
    /// Event send when a data command transfert is finish
    HAL_USB_CALLBACK_TYPE_DATA_CMD,
    /// Event send when a receive transfert is finish
    HAL_USB_CALLBACK_TYPE_RECEIVE_END,
    /// Event send when a transmit transfert is finish
    HAL_USB_CALLBACK_TYPE_TRANSMIT_END,
    /// Event send when interface is enable
    HAL_USB_CALLBACK_TYPE_ENABLE,
    /// Event send when interface is disable
    HAL_USB_CALLBACK_TYPE_DISABLE
} HAL_USB_CALLBACK_EP_TYPE_T;

// =============================================================================
// HAL_USB_REQUEST_EP_T
// -----------------------------------------------------------------------------
/// List of EndPoint request
// =============================================================================
typedef enum
{
    HAL_USB_REQUEST_EP_GET_STATUS    = 0x00,
    HAL_USB_REQUEST_EP_CLEAR_FEATURE = 0x01,
    HAL_USB_REQUEST_EP_SET_FEATURE   = 0x03
} HAL_USB_REQUEST_EP_T ;

// =============================================================================
// HAL_USB_SETUP_REQUEST_DESC_T
// -----------------------------------------------------------------------------
/// Description of request
// =============================================================================
typedef struct
{
    UINT8  requestDest     :5;
    UINT8  requestType     :2;
    UINT8  requestDirection:1;
} PACKED HAL_USB_SETUP_REQUEST_DESC_T;

// =============================================================================
// HAL_USB_SETUP_T
// -----------------------------------------------------------------------------
/// Description of USB command
// =============================================================================

typedef struct
{
    HAL_USB_SETUP_REQUEST_DESC_T  requestDesc;
    UINT8                         request;
    UINT16                        value;
    UINT16                        index;
    UINT16                        lenght;
} PACKED HAL_USB_SETUP_T;

// =============================================================================
// HAL_USB_EP_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Ep descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;//this structure's length in byte
    UINT8  type;//Endpoint type:command,interrupt ISO or bulk
    UINT8  ep;//the number of endpoint 
    UINT8  attributes;//endpoint's special atrributes
    UINT16 mps;//max packet size in byte
    UINT8  interval;//the interval of request
} PACKED HAL_USB_EP_DESCRIPTOR_REAL_T;
// =============================================================================
// HAL_USB_HID_DES
// -----------------------------------------------------------------------------
/// Human Input Device Descriptor
// =============================================================================
typedef struct 
{
    UINT8 bLength;//Numeric expression that is the total size of the HID descriptor
    UINT8 bDescriptorType;//Constant name specifying type of HID descriptor
    UINT16 bcdHID;//Numeric expression identifying the HID Class Specification release
    UINT8 bCountryCode;//Numeric expression identifying country code of the localized hardware
    UINT8 bNumberDescriptor;//Numeric expresssion specifying the number of class descriptor
    UINT8 bDescType;//Constant name identifying type of class descriptor.
    UINT16 DescriptorLength;//Numeric expression that is the total size of the report descriptor
} PACKED HAL_USB_HID_DES;

// =============================================================================
// HAL_USB_CALLBACK_RETURN_T
// -----------------------------------------------------------------------------
/// Return value of callback
// =============================================================================
typedef enum {
    HAL_USB_CALLBACK_RETURN_OK,
    HAL_USB_CALLBACK_RETURN_RUNNING,
    HAL_USB_CALLBACK_RETURN_KO
} HAL_USB_CALLBACK_RETURN_T ;


// =============================================================================
// HAL_USB_CALLBACK_T
// -----------------------------------------------------------------------------
/// Type use to define endpoint/interface callback. It's call when an endpoint
/// commande  are receive on EP0.
/// @param type kind of action
/// @param setup Setup is the USB command data
// =============================================================================
typedef HAL_USB_CALLBACK_RETURN_T
(*HAL_USB_CALLBACK_T)(HAL_USB_CALLBACK_EP_TYPE_T   type,
                      HAL_USB_SETUP_T*             setup);

typedef UINT32 (*HAL_USB_CS_CONSTRUCTOR_T)(UINT8* buffer);

#include "hal_usb_descriptor_types.h"

// =============================================================================
// HAL_USB_GETDESCRIPTOR_CALLBACK_T
// -----------------------------------------------------------------------------
/// This function is call when USB must send a device descriptor request
/// @return Return a device descriptor
// =============================================================================
typedef HAL_USB_DEVICE_DESCRIPTOR_T* (*HAL_USB_GETDESCRIPTOR_CALLBACK_T)(BOOL);

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

// =============================================================================
// hal_UsbEpStall
// -----------------------------------------------------------------------------
/// Stall an End point
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param stall \c TRUE Set EndPoint Stall
///              \c FALSE Clear EndPoint Stall
// =============================================================================
PUBLIC VOID hal_UsbEpStall(UINT8 ep, BOOL stall);

// =============================================================================
// hal_UsbOpen
// -----------------------------------------------------------------------------
/// This function enable USB
///
/// This function requests a resource of #HAL_SYS_FREQ_39M.
///
/// @param callback See #HAL_USB_GETDESCRIPTOR_CALLBACK_T
// =============================================================================
PUBLIC VOID hal_UsbOpen(HAL_USB_GETDESCRIPTOR_CALLBACK_T callback);

PUBLIC VOID hal_UsbCloseSoftware(VOID);

// =============================================================================
// hal_UsbClose
// -----------------------------------------------------------------------------
/// This function disable USB
// =============================================================================
PUBLIC VOID hal_UsbClose(VOID);

// =============================================================================
// hal_UsbSend
// -----------------------------------------------------------------------------
/// Send an usb packet
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param buffer Pointer on the date buffer to be send.
/// @param size Number of bytes to be send.
/// @param flag For partial send set the flag to 1
/// @return Number of sent bytes or -1 if error
// =============================================================================
PUBLIC INT32 hal_UsbSend(UINT8  ep, UINT8* buffer, UINT16 size, UINT32 flag);

// =============================================================================
// hal_UsbRecv
// -----------------------------------------------------------------------------
/// Recv an usb packet
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param buffer Pointer on the data buffer to be send.
/// @param size Number of bytes to be send.
/// @param flag No flag you must set this parameter to zero
/// @return Number of sent bytes or -1 if error
// =============================================================================
PUBLIC INT32 hal_UsbRecv(UINT8  ep, UINT8* buffer, UINT16 size, UINT32 flag);

// =============================================================================
// hal_UsbCompletedCommand
// -----------------------------------------------------------------------------
/// This function must be call for completed an delayed EP0 command (when you
/// return #HAL_USB_CALLBACK_RETURN_RUNNING on the interface or EP callback)
// =============================================================================
PUBLIC VOID hal_UsbCompletedCommand(VOID);

// =============================================================================
// hal_UsbEnableInterrupt
// -----------------------------------------------------------------------------
/// Enable or disable interrupt for an specific EndPoint. The default value
/// after initialisation is true
/// @param ep Number of Endpoint
/// @param enable \c TRUE Enable the interrupt
///               \c FALSE Disable the interrupt
// =============================================================================
PUBLIC VOID hal_UsbEpEnableInterrupt(UINT8 ep, BOOL enable);

// =============================================================================
// hal_UsbEpRxTransferedSize
// -----------------------------------------------------------------------------
/// This function return number of bytes send on the last transfert
/// @param ep Number of Endpoint
/// @return Size transfered
// =============================================================================
PUBLIC UINT16 hal_UsbEpRxTransferedSize(UINT8 ep);

// =============================================================================
// hal_UsbEpTransfertDone
// -----------------------------------------------------------------------------
/// This function indicate if an transfert is finish.
/// @param ep Number of Endpoint
/// @return return the end transfert status
// =============================================================================
PUBLIC BOOL hal_UsbEpTransfertDone(UINT8 ep);
PUBLIC void hal_UsbSleepMode(void);
PUBLIC void hal_UsbWakeUpMode(void);

// =============================================================================
// hal_UsbReset
// -----------------------------------------------------------------------------
/// This function is use to reset USB and/or change configuration of usb
/// For reset you must do hal_UsbReset(TRUE); sleep 0.5s; hal_UsbReset(FALSE);
/// @param reset If is true, the function is clear usb and power down usb else
/// the function power up usb
// =============================================================================
PUBLIC VOID hal_UsbReset(BOOL reset);

///  @} <- End of USB group
//#ifdef USB_AUDIO_SUPPORT
PUBLIC HAL_USB_EP_TYPE_T hal_UsbGetEpAttr(UINT8 epNum);
PUBLIC VOID hal_UsbSetMode(HAL_USB_MODE_T Mode);
PUBLIC HAL_USB_MODE_T hal_UsbGetMode(void);
PUBLIC void hal_UsbSetDisconnCb(void (*Cb)(BOOL isHost));
PUBLIC BOOL hal_UsbIsWorkAsHost();
PUBLIC VOID hal_musb_start_synops();
PUBLIC int RDAUsb_GetState(void);
PUBLIC void musb_notify_conn_msg(UINT8 msg);
PUBLIC int Mcd_UsbHostDeInit(uint8 port);
PUBLIC void hal_musb_host_disconnect(void);

//#endif
#endif // _HAL_USB_H_
