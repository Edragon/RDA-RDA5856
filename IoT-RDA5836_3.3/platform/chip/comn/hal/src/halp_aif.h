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
/// @file halp_aif.h                                                          //
/// That file presents the HAL AIF driver private API.                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_AIF_H_
#define _HALP_AIF_H_

#include "cs_types.h"
#include "hal_aif.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Indices for the user handler array.
#define RECORD  0
#define PLAY    1

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_AIF_XFER_HANDLER_T
// -----------------------------------------------------------------------------
/// Type privately used to store both handler for each direction
// =============================================================================
typedef struct
{
    HAL_AIF_HANDLER_T halfHandler;
    HAL_AIF_HANDLER_T endHandler;
} HAL_AIF_XFER_HANDLER_T;






// =============================================================================
// hal_AifIrqHandler
// -----------------------------------------------------------------------------
/// Handler called by the IRQ module when a BB-IFC interrupt occurs.
/// 
// =============================================================================
PROTECTED VOID hal_AifIrqHandler(UINT8 interruptId);


#endif //_HALP_AIF_H_

