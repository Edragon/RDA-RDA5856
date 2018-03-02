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
/// @file bp_ap_sync_config.h                                                      //
/// That file provides the TGT API related to ApBpSync configuration.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_AP_BP_SYNC_CFG_H_
#define _TGT_AP_BP_SYNC_CFG_H_
#include "cs_types.h"
#include "gpsd_config.h"


// =============================================================================
// TGT_AP_BP_SYNC_CONFIG_T
// -----------------------------------------------------------------------------
/// This function is used by AP_BP_SYNC to get access to its configuration structure.
/// This is the only way AP_BP_SYNC can get this information.
// =============================================================================
PUBLIC CONST GPSD_CONFIG_T* tgt_GetGpsdConfig(VOID);


#endif // _TGT_CAMS_CFG_H_

