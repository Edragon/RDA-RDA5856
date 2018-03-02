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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_mcd_cfg.h                                                       //
/// That file provides the TGT API related to MCD configuration.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_MCD_CFG_H_
#define _TGT_MCD_CFG_H_

#include "cs_types.h"

#ifdef DUAL_TFLASH_SUPPORT
#include "mcd_config.h"

EXTERN_C_START

// =============================================================================
// tgt_GetMcdConfig
// -----------------------------------------------------------------------------
/// This function is used by MCD to get access to its configuration structure.
/// This is the only way MCD can get this information.
// =============================================================================
PUBLIC CONST MCD_CONFIG_STRUCT_T* tgt_GetMcdConfig(VOID);
EXTERN_C_END
#else
typedef struct MCD_CONFIG_STRUCT_T TGT_MCD_CONFIG_T;


// =============================================================================
// tgt_GetMcdConfig
// -----------------------------------------------------------------------------
/// This function is used by MCD to get access to its configuration structure.
/// This is the only way MCD can get this information.
// =============================================================================
PUBLIC CONST TGT_MCD_CONFIG_T* tgt_GetMcdConfig(VOID);
#endif

#endif // _TGT_MCD_CFG_H_

