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
/// @file halp_config.h                                                       //
/// That file is the private header for the configuration of HAL in its aspect
/// that are the more likely to be implementation dependent...                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_CONFIG_H_
#define _HALP_CONFIG_H_

#include "cs_types.h"
#include "hal_config.h"


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

EXPORT CONST HAL_CFG_CONFIG_T* g_halCfg;


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_BoardSetup
// -----------------------------------------------------------------------------
/// Apply board dependent configuration to HAL
/// @param halCfg Pointer to HAL configuration structure (from the target
/// module).
// =============================================================================
PROTECTED VOID hal_BoardSetup(CONST HAL_CFG_CONFIG_T* halCfg);


#endif // _HALP_CONFIG_H_

