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
/// @file mcd_config.h
/// That file describes the configuration of the structure used to set up
/// the SDMMC version of the MCD driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "hal_gpio.h"


#ifndef _MCD_CONFIG_H_
#define _MCD_CONFIG_H_



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================


// =============================================================================
//  MACROS
// =============================================================================

#define MCD_CONFIG_STRUCT_T MCD_CONFIG_SDMMC_STRUCT_T

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// MCD_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for the MCD driver on SDMMC Interface
// =============================================================================
struct MCD_CONFIG_SDMMC_STRUCT_T
{
    /// Wether or not the DAT3/CD line has a pull-down and can be used for 
    /// minimalist new card detection (no insertion/removal interrupt.)
    BOOL                dat3HasPullDown;
};




// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

#endif // _MCD_CONFIG_H_


