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
/// @file halp_dma.h                                                          //
/// That file provides the private interface of the DMA driver.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef  _HALP_DMA_H_
#define  _HALP_DMA_H_


#include "cs_types.h"

// =============================================================================
//  FUNCTIONS
// =============================================================================



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

// =============================================================================
// hal_DmaIrqHandler
// -----------------------------------------------------------------------------
/// DMA module IRQ handler
/// Clear IRQ and call the IRQ handler user function.
// =============================================================================
PROTECTED VOID hal_DmaIrqHandler(UINT8 dmaIrqId);




#endif //  _HAL_DMA_H_ 

