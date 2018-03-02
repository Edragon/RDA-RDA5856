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
/// @file boot_dma.h                                                            //
/// BOOT DMA driver                                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _BOOT_DMA_H_
#define _BOOT_DMA_H_

/// @defgroup BOOT DMA Driver
///
/// @{
///

#include "cs_types.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// boot_DmaCopy
// -----------------------------------------------------------------------------
/// Copy data via DMA
/// @param dst  The destination address
/// @param src  The source address
/// @param len  The data length in bytes
/// @return None
// =============================================================================

PUBLIC VOID boot_DmaCopy(VOID *dst, CONST VOID *src, UINT32 len);


// =============================================================================
// boot_DmaSet
// -----------------------------------------------------------------------------
/// Set the memory to a pattern value via DMA
/// @param dst      The destination address (should be word-aligned)
/// @param pattern  The pattern to be written
/// @param len      The data length in bytes (should be mulitple of 4)
/// @return None
// =============================================================================

PUBLIC VOID boot_DmaSet(UINT32 *dst, UINT32 pattern, UINT32 len);


/// @} <-- End of the dma group

#endif // _BOOT_DMA_H_


