/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_camd_cfg.h                                                      //
/// That file provides the TGT API related to CAMD configuration.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_CAMD_CFG_H_
#define _TGT_CAMD_CFG_H_

#include "cs_types.h"
#include "camd_config.h"


// =============================================================================
// tgt_GetCamdConfig
// -----------------------------------------------------------------------------
/// This function is used by CAMD to get access to its configuration structure.
/// This is the only way CAMD can get this information.
// =============================================================================
PUBLIC CONST CAMD_CONFIG_T* tgt_GetCamdConfig(VOID);


#endif // _TGT_CAMD_CFG_H_

