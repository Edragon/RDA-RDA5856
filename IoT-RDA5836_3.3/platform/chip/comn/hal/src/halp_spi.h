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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file  hal_spi.h                                                          //
/// That file provides the SPI driver private API.                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef  _HALP_SPI_H_
#define  _HALP_SPI_H_

#include "hal_spi.h"




// svn propset svn:keywords "HeadURL Author Date Revision" dummy.h



// =============================================================================
//  MACROS
// =============================================================================

/// Those defines are directly related to the ways the macros representing
/// the transferts are defined : IRQ needing ones are odd. DMA needing ones
/// are 3 or 4 (second bit is then one ...)
#define NEED_IRQ(val) (val & 0x1)
#define IS_DMA(val) (val & 0x2)



// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// hal_SpiIrqHandler
// -----------------------------------------------------------------------------
/// Handler called in case of SPI interrupt
/// 
/// @param interruptId  Will be spi Hw module interrupt 
/// id when used by the IRQ driver.
// =============================================================================
PROTECTED VOID hal_SpiIrqHandler(UINT8 interruptId);

// =============================================================================
// hal_SpiUpdateDivider
// -----------------------------------------------------------------------------
/// Update the SPI divider so that the maximum frequency defined for the currently
/// active CS is not overpassed.
/// @param id Identifier of the SPI for which the function is called.
/// @param sysFreq Newly applied system frequency
// =============================================================================
PROTECTED VOID hal_SpiUpdateDivider(HAL_SPI_ID_T id, HAL_SYS_FREQ_T sysFreq);

#endif //_HALP_SPI_H_


