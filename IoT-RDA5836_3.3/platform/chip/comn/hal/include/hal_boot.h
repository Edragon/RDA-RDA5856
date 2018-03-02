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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_host.h                                                          //
/// That file contains types and defines used to exchange with the            //
/// host for monitoring and remote execution.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_BOOT_H_
#define _HAL_BOOT_H_

/// @defgroup boot HAL Boot and Monitoring Driver
///
/// Tha group describes the API allowing for monitoring stuff to be done through
/// and by the HAL layer.
///
/// @{
///


#include "cs_types.h"



// =============================================================================
// hal_BootEnterMonitor
// -----------------------------------------------------------------------------
/// Call this function to enter the boot monitor.
// =============================================================================
PUBLIC VOID hal_BootEnterMonitor(VOID);


// @} // <-- End of the boot group.

#endif // _HAL_BOOT_H_




