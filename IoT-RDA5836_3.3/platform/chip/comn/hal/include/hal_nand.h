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
/// @file hal_nand                                                            //
/// HAL NAND driver                                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_NAND_H_
#define _HAL_NAND_H_

/// @defgroup cipher HAL NAND Driver
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
// hal_NandOpen
// -----------------------------------------------------------------------------
/// Initialize NAND flash controller
/// @param None
/// @return None
// =============================================================================
PUBLIC VOID hal_NandOpen(VOID);


// =============================================================================
// hal_NandReadData
// -----------------------------------------------------------------------------
/// Read data from NAND flash
/// @param addr    The start address in NAND flash
/// @param pData   Pointing to a buffer to hold the data
/// @param len     The number of bytes to be read
/// @return None
// =============================================================================
PUBLIC VOID hal_NandReadData(UINT32 addr, VOID *pData, UINT32 len);



/// @} <-- End of the nand group

#endif // _HAL_NAND_H_

