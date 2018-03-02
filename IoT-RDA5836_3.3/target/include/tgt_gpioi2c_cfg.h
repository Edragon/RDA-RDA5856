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
/// @file tgt_gpioi2c_cfg.h                                                      //
///              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_GPIOI2C_CFG_H_
#define _TGT_GPIOI2C_CFG_H_

#include "cs_types.h"

typedef struct GPIOI2C_CONFIG_STRUCT_T TGT_GPIOI2C_CONFIG_T;

// =============================================================================
// 
// -----------------------------------------------------------------------------
///  
/// 
// =============================================================================
PUBLIC CONST TGT_GPIOI2C_CONFIG_T* tgt_GetGpioi2cConfig(VOID);


#endif //  

