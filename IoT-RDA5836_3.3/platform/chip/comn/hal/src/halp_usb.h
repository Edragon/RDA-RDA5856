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
/// @file halp_usb.h
///
/// This document describes the HAL USB private function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_USB_H_
#define _HALP_USB_H_

#include "cs_types.h"

#if CHIP_HAS_USB == 1
#include "boot_usb.h"
#endif

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

#if CHIP_HAS_USB == 1
#ifdef HAL_USB_DRIVER_DONT_USE_ROMED_CODE
PROTECTED VOID hal_UsbIrqHandler(UINT8 interruptId);
PROTECTED VOID hal_UsbRoleDet(UINT8 interruptId);
#else
#define hal_UsbIrqHandler boot_UsbIrqHandler
#endif // HAL_USB_DRIVER_DONT_USE_ROMED_CODE
PROTECTED VOID hal_UsbHandleIrqInGdb(VOID);
#endif /* CHIP_HAS_USB */

#endif // _HALP_USB_H_

