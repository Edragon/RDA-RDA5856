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
///
/// @file patch_m.h
/// That file describes the patch interface.
///
/// The PATCH module gives the interface to setup a patch and also provides
/// as submodules the current set of patch applyed to a particular chip's ROM
///
////////////////////////////////////////////////////////////////////////////////


#ifndef _PATCH_M_H_
#define _PATCH_M_H_

#include "cs_types.h"

/// @defgroup PATCH
/// @{


// =============================================================================
//  MACROS
// =============================================================================

#define PATCH_FAILED    (-1)

// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// patch_Open
// -----------------------------------------------------------------------------
/// This function install all the hardware patch available for the current chip
// =============================================================================
PUBLIC VOID patch_Open(VOID);


// =============================================================================
// patch_ReplaceFunction
// -----------------------------------------------------------------------------
/// This function install and enable a patch to replace a function.
/// The function in ROM must be aligned on 16 bytes boundaries.
/// The two functions must have the exact same prototype !
/// The patch will actually replace \p oldFunction by a direct jump 
/// to \p newFunction.
/// @param oldFunction  the address/symbol of the function in ROM to replace.
/// @param newFunction  the address/symbol of the new implementation of the 
///                     function.
/// @return             the number of the allocated hardware patch 
///                     or \c PATCH_FAILED
// =============================================================================
PUBLIC INT32 patch_ReplaceFunction(CONST VOID (*oldFunction)(VOID),
                                   CONST VOID (*newFunction)(VOID));


// =============================================================================
// patch_ReplaceData
// -----------------------------------------------------------------------------
/// This function install and enable a patch to replace some rom data.
/// The data in ROM must be aligned on 16 bytes boundaries.
/// @param oldDataAddr  the address of the data in ROM to replace.
/// @param newData      pointing to the new data.
/// @param len          the length of the new data in bytes and must be 16.
/// @return             the number of the allocated hardware patch 
///                     or \c PATCH_FAILED
// =============================================================================
PUBLIC INT32 patch_ReplaceData(CONST VOID *oldDataAddr,
                               CONST VOID *newData,
                               UINT32 len);


// =============================================================================
// patch_GetLocation
// -----------------------------------------------------------------------------
/// This function allocate a hardware patch, it then need to be enabled with
/// #patch_Enable.
/// Use this function to get a patch location when #patch_ReplaceFunction cannot
/// be used for your patch (patch data or patch assembly code)
/// @param patchArea    pointer to recieve the address of the patch area
///                     It can be null.
/// @return             the number of the hardware patch or \c PATCH_FAILED
// =============================================================================
PUBLIC INT32 patch_GetLocation(UINT32** patchArea);


// =============================================================================
// patch_Enable
// -----------------------------------------------------------------------------
/// This function enables a hardware patch.
/// Once the replacement code is placed in internal sram, use this function to
/// enable the hardware patch.
/// @param patchNumber  the number of the patch as returned by #patch_GetLocation
/// @param patchAddress the address in the rom to patch (will be 16 bytes aligned)
// =============================================================================
PUBLIC VOID patch_Enable(UINT32 patchNumber, UINT32 patchAddress);


///  @} <- End of the PATCH group 

#endif // _PATCH_M_H_

