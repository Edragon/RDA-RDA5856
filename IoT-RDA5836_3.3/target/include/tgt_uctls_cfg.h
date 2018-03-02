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
/// @file tgt_uctl_cfg.h                                                      //
/// That file provides the TGT API related to Usb Controller configuration.   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_UCTLS_CFG_H_
#define _TGT_UCTLS_CFG_H_

#include "cs_types.h"
#include "uctls_m.h"
#include "umss_m.h"
//#include "utraces_m.h"
#include "umss_callback.h"
#include "umss_config.h"
//#include "uvideos_callback.h"
//#include "uvideos_config.h"

// =============================================================================
// UCTLS_CONFIG_T
// -----------------------------------------------------------------------------
/// Rename type for uctls config
// =============================================================================
typedef UCTLS_SERVICE_LIST_T UCTLS_CONFIG_T;

// =============================================================================
// tgt_GetUctlsConfig
// -----------------------------------------------------------------------------
/// This function is used by UCTL to get access to its configuration structure.
/// This is the only way UCTLS can get this information.
// =============================================================================
PUBLIC CONST UCTLS_CONFIG_T* tgt_GetUctlsConfig(VOID);

#endif // _TGT_UCTLS_CFG_H_
