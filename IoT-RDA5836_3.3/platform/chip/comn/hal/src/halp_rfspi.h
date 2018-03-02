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
/// @file halp_rfspi.h                                                        //
/// That file provides the private interface for the RFSPI driver.            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_rfspi.h"
#include "hal_sys.h"


#ifndef _HALP_RFSPI_H_
#define _HALP_RFSPI_H_


// =============================================================================
// hal_RfspiIrqHandler
// -----------------------------------------------------------------------------
/// RF-SPI module IRQ handler.
/// This function is used by the private IRQ module to clear the IRQ and call 
/// the user handler with the proper status.
// =============================================================================
PROTECTED VOID hal_RfspiIrqHandler(UINT8 interruptId);

#endif // _HALP_RFSPI_H_




