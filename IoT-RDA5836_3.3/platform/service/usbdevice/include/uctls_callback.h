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
/// @file uctls_callback.h
///
////////////////////////////////////////////////////////////////////////////////

#ifndef _UCTLS_CALLBACK_H_
#define _UCTLS_CALLBACK_H_

#include "cs_types.h"

#include "umss_m.h"
//#include "uvideos_m.h"

/// @file uctls_callback.h
///
/// @addtogroup uctls
/// @{
///

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// UCTLS_SERVICE_CFG_T
// -----------------------------------------------------------------------------
/// Service configuration data
// =============================================================================
typedef union {
    CONST UMSS_CFG_T mss;
//	CONST UVIDEOS_CFG_T videos;
} UCTLS_SERVICE_CFG_T;

// =============================================================================
// UCTLS_SERVICE_CALLBACK_OPEN_T
// -----------------------------------------------------------------------------
/// Open service callback
/// @param config service configure structure
/// @return USB list interface descriptor
// =============================================================================
typedef HAL_USB_INTERFACE_DESCRIPTOR_T**
(*UCTLS_SERVICE_CALLBACK_OPEN_T)(CONST UCTLS_SERVICE_CFG_T* config);

// =============================================================================
// UCTLS_SERVICE_CALLBACK_CLOSE_T
// -----------------------------------------------------------------------------
/// Close service callback
// =============================================================================
typedef VOID
(*UCTLS_SERVICE_CALLBACK_CLOSE_T)(VOID);

typedef VOID
(*UCTLS_SERVICE_CALLBACK_PROCESS_T)(UINT32);

// =============================================================================
// UCTLS_SERVICE_CALLBACK_T
// -----------------------------------------------------------------------------
/// Callback service structure
// =============================================================================
typedef struct {
    // Open
    CONST UCTLS_SERVICE_CALLBACK_OPEN_T  open;
    // Close
    CONST UCTLS_SERVICE_CALLBACK_CLOSE_T close;
    // Process event
    CONST UCTLS_SERVICE_CALLBACK_PROCESS_T process;
} UCTLS_SERVICE_CALLBACK_T;

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

/// @} <- End of the uctls group

#endif // _UCTLS_CALLBACK_H_

