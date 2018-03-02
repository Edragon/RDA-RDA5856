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
/// @file halp_ebc.h                                                          //
/// Prototypes for private EBC functions                                      //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 


#ifndef  _HALP_EBC_H_
#define  _HALP_EBC_H_

#include "cs_types.h"
#include "global_macros.h"
#include "mem_bridge.h"
#include "hal_sys.h"
#include "hal_ebc.h"


#ifdef CHIP_HAS_EBC_CS2_BUG
/// Use to remember the CS0 timing configuration, as the register
/// cannot be read again.
extern PROTECTED UINT32 g_halCs0TimeReg ;
/// Use to remember the CS0 mode configuration, as the register
/// cannot be read again.
extern PROTECTED UINT32 g_halCs0ModeReg ;
#endif

/// 
///     Array of pointers to the different
///     CS configuration.
///     
///     \c NULL in the i-th place means
///     the CS i is not enabled/used @todo Write that properly.
/// 
extern HAL_EBC_CS_CFG_T* g_halEbcCsConfigArray [EBC_NB_CS];

#endif // HAL_EBC_H
