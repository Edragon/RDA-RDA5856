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
/// @file boot_nand.h                                                         //
/// BOOT NAND driver                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _BOOT_NAND_H_
#define _BOOT_NAND_H_

/// @defgroup BOOT NAND Driver
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
// boot_NandOpen
// -----------------------------------------------------------------------------
/// Initialize NAND flash controller
/// @param None
/// @return None
// =============================================================================

PUBLIC VOID boot_NandOpen(VOID);


// =============================================================================
// boot_NandGetPageSize
// -----------------------------------------------------------------------------
/// Get the page size of the NAND flash
/// @param None
/// @return The page size
// =============================================================================

PUBLIC UINT32 boot_NandGetPageSize(VOID);


// =============================================================================
// boot_NandReadByPageNum
// -----------------------------------------------------------------------------
/// Read one page of data from NAND flash by page number
/// @param page    The page number in NAND flash
/// @param pData   Pointing to a buffer to hold the data (should be word-aligned)
/// @param len     The number of bytes to be read (should be mulitple of 4)
/// @return None
// =============================================================================

PUBLIC VOID boot_NandReadByPageNum(UINT32 page, UINT32 *pData, UINT32 len);


// =============================================================================
// boot_NandReadPages
// -----------------------------------------------------------------------------
/// Read multiple page data from NAND flash
/// @param addr    The start address in NAND flash (should be page-aligned)
/// @param pData   Pointing to a buffer to hold the data (should be word-aligned)
/// @param len     The number of bytes to be read (should be mulitple of 4)
/// @return None
// =============================================================================

PUBLIC VOID boot_NandReadPages(UINT32 addr, UINT32 *pData, UINT32 len);


/// @} <-- End of the nand group

#endif // _BOOT_NAND_H_

