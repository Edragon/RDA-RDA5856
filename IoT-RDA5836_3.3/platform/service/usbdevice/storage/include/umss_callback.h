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
//
/// @file umss_callback.h
///
/// Callback defined in umss
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UMSS_CALLBACK_H_
#define _UMSS_CALLBACK_H_

#include "cs_types.h"

#include "uctls_callback.h"

/// @file umss_callback.h
///
/// @addtogroup umss
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
// g_umssCallback
// -----------------------------------------------------------------------------
/// Mass storage service callback
// =============================================================================
EXPORT PUBLIC CONST UCTLS_SERVICE_CALLBACK_T    g_umssCallback;

/// @} <- End of the umss group

#endif // _UMSS_CALLBACK_H_
