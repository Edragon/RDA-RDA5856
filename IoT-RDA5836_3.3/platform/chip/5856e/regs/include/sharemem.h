//==============================================================================
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
// ==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _BB_AHB_SHAREMEM_H_
#define _BB_AHB_SHAREMEM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bb_ahb_monitor'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BB_AHB_MONITOR_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================

#define SHAREMEM_SIZE       (256)

typedef struct
{
    REG32                          Rx_Buff[SHAREMEM_SIZE*3];                      //0x00000000
    REG32                          Tx_Buff[SHAREMEM_SIZE];                      //0x00000400
} HWP_AHB_SHAREMEM_T;



#endif

