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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_rfd_cfg.h                                                       //
/// That file provides the TGT API related to RFD configuration.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_RFD_CFG_H
#define _TGT_RFD_CFG_H

#include "cs_types.h"
#include "rfd_config.h"


// =============================================================================
// tgt_GetXcvConfig
// -----------------------------------------------------------------------------
/// This function is used by XCV to get access to its configuration structure.
/// This is the only way XCV can get this information.
// =============================================================================
PUBLIC CONST XCV_CONFIG_T* tgt_GetXcvConfig(VOID);


// =============================================================================
// tgt_GetPaConfig
// -----------------------------------------------------------------------------
/// This function is used by PA to get access to its configuration structure.
/// This is the only way PA can get this information.
// =============================================================================
PUBLIC CONST PA_CONFIG_T*  tgt_GetPaConfig(VOID);


// =============================================================================
// tgt_GetSwConfig
// -----------------------------------------------------------------------------
/// This function is used by SW to get access to its configuration structure.
/// This is the only way SW can get this information.
// =============================================================================
PUBLIC CONST SW_CONFIG_T*  tgt_GetSwConfig(VOID);

#endif // _TGT_RFD_CFG_H

