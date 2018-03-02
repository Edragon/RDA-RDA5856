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

#ifndef _UAUD_M_H_
#define _UAUD_M_H_

#include "cs_types.h"

#include "hal_usb.h"

#include "uaud_config.h"

/// @file uaud_m.h
/// @mainpage USB Mass Storage service
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
// uaud_Open
// -----------------------------------------------------------------------------
/// Open USB audio play service
PUBLIC VOID uaud_Open(void);

// =============================================================================
// uaud_TransportEventProcess
// -----------------------------------------------------------------------------
/// Process audio event
PUBLIC VOID uaud_TransportEventProcess(UINT32 nParam1);

// =============================================================================
// uaud_Close
// -----------------------------------------------------------------------------
/// Close USB audio play service
PUBLIC VOID uaud_Close(void);

PUBLIC UINT32 uAudGetPlayStatus(void);

///  @} <- End of the uaud group

#endif // _UAUD_M_H_
