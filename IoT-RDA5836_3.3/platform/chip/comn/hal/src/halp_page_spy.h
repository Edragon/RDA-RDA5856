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
/// hal_page_spy.h
/// PAGE SPY driver private API
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




#ifndef  _HALP_PAGE_SPY_H_
#define  _HALP_PAGE_SPY_H_

#include "cs_types.h"
#include "hal_debug.h"


//Types and enums


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_DbgPageProtectGetHitMaster
// -----------------------------------------------------------------------------
/// Get the master that hit the page
/// @param page One of the four protected page
/// @return Master id of the one that hit the page
// =============================================================================
PROTECTED UINT8 hal_DbgPageProtectGetHitMaster(HAL_DBG_PAGE_NUM_T page);




// =============================================================================
// hal_DbgPageProtectGetHitAddress
// -----------------------------------------------------------------------------
/// Get address where \c page where hit
/// @param page Hit page whose address where it was hit is requested
/// @return The last address hit in a protected page
// =============================================================================
PROTECTED UINT32 hal_DbgPageProtectGetHitAddress(HAL_DBG_PAGE_NUM_T page);



// =============================================================================
// hal_DbgPageProtectGetHitMode
// -----------------------------------------------------------------------------
/// Get the mode that triggered the page
///
/// @param page A page that triggered the Page Spy
/// @return The access mode that triggered the Page Spy : \n
///     - If the bit 0 is set, a read access  happened
///     - If the bit 1 is set, a write access happened
///     .
// =============================================================================
PROTECTED HAL_DBG_PAGE_SPY_MODE_T hal_DbgPageProtectGetHitMode(HAL_DBG_PAGE_NUM_T page);


#endif // _HALP_PAGE_SPY_H_
