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
/// @file halp_sdio.h
/// SDIO private API. It holds the IRQ handling functions, types and mechanisms
/// to enable the SDIO interrupt on the System Side. If they appear to be useful,
/// put them in the public interface.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef _HALP_SDIO_H_
#define _HALP_SDIO_H_



// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// ============================================================================
// hal_SdioIrqHandler
// ----------------------------------------------------------------------------
/// SDIO IRQ handler, clearing the IRQ cause regiter and calling the user handler
/// defined by hal_SdioIrqSetHandler.
/// @param interruptId Id with which the handler is entered
// ============================================================================
PROTECTED VOID hal_SdioIrqHandler(UINT8 interruptId);



#endif
