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
/// @file uaud_m.h
///
/// USB Mass Storage Service
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UHID_M_H_
#define _UHID_M_H_

#include "cs_types.h"

#include "hal_usb.h"

/// @file uhid_m.h
/// @mainpage USB HID service
///
/// This service provides Mass Storage service on usb
///
/// This document is composed of:
/// - @ref uaud
///
/// @addtogroup uaud USB Mass Storage Service (uaud)
/// @{
///

// =============================================================================
// MACROS                                                                       
// =============================================================================

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
// uHid_Open
// -----------------------------------------------------------------------------
/// Open USB HID service
PUBLIC VOID uhid_Open(UINT8 *desc, UINT16 length);

// =============================================================================
// uHid_TransportEventProcess
// -----------------------------------------------------------------------------
/// Process hid event
PUBLIC VOID uhid_TransportEventProcess(UINT32 nParam1);

// =============================================================================
// uHid_Close
// -----------------------------------------------------------------------------
/// Close USB HID service
PUBLIC VOID uhid_Close(void);

// =============================================================================
// uHid_SendKeycode
// -----------------------------------------------------------------------------
/// Send HID key code
PUBLIC VOID uhid_SendKeycode(UINT8 *keycode, UINT8 length);


///  @} <- End of the uaud group

#endif // _UHID_M_H_
