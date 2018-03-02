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
/// @file hal_usb_monitor.h
///
/// Global variable for usb monitor
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_USB_MONITOR_H_
#define _HAL_USB_MONITOR_H_

#include "cs_types.h"

#include "hal_usb.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================

#define HAL_HOST_USB_FIFO_READ_SIZE              (32)
#define HAL_HOST_USB_BUFFER_OUT_SIZE             (2048)
#define HAL_HOST_USB_BUFFER_IN_SIZE              (1032)

// =============================================================================
// TYPES                                                                        
// =============================================================================

typedef enum {
    HAL_HOST_USB_MODE_BOOT                      = 0x00000001,
    HAL_HOST_USB_MODE_SX                        = 0x00000002
} HAL_HOST_USB_MODE_T;

// =============================================================================
// HAL_HOST_USB_CALLBACK_T
// -----------------------------------------------------------------------------
/// Callback for decoding trame
// =============================================================================
typedef VOID (*HAL_HOST_USB_CALLBACK_T)(VOID *buffer, UINT16 size);

// ============================================================================
// HAL_HOST_USB_MONITOR_FIFO_ELEM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================


typedef struct
{
    UINT32                         Addr;                         //0x00000000
    UINT8                          Rid;                          //0x00000004
    UINT16                         Size;                         //0x00000006
} HAL_HOST_USB_MONITOR_FIFO_ELEM_T; //Size : 0x8



// ============================================================================
// HAL_HOST_USB_MONITOR_BUFFER_IN_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================


typedef struct
{
    UINT8                          Buffer[HAL_HOST_USB_BUFFER_IN_SIZE]; //0x00000000
} HAL_HOST_USB_MONITOR_BUFFER_IN_T; //Size : 0x408



// ============================================================================
// HAL_HOST_USB_MONITOR_CTX_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================


typedef struct
{
    HAL_HOST_USB_MODE_T            MonitorMode;                  //0x00000000
    UINT8                          BufferOut[HAL_HOST_USB_BUFFER_OUT_SIZE]; //0x00000004
    HAL_HOST_USB_MONITOR_BUFFER_IN_T BufferIn[2];                //0x0x00000804
    HAL_HOST_USB_MONITOR_FIFO_ELEM_T Fifo[HAL_HOST_USB_FIFO_READ_SIZE]; //0x0x00001014
} HAL_HOST_USB_MONITOR_CTX_T; //Size : 0x1114

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

// =============================================================================
// hal_HostUsbOpen
// -----------------------------------------------------------------------------
/// Configure USB host
// =============================================================================
PUBLIC HAL_USB_INTERFACE_DESCRIPTOR_T* hal_HostUsbOpen(VOID);

// =============================================================================
// hal_HostUsbClose
// -----------------------------------------------------------------------------
/// Close host usb
// =============================================================================
PUBLIC VOID hal_HostUsbClose(VOID);

#endif // _HAL_USB_MONITOR_H_
