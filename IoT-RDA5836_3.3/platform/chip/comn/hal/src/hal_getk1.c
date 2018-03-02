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
//==============================================================================
//                                                                            //
/// @file                                                                     //
/// That file provides a dummy header  ...                                    //
//                                                                            //
//==============================================================================





// svn propset svn:keywords "HeadURL Author Date Revision" dummy.h



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================
#include "cs_types.h"
#include "hal_mem_map.h"

// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

VOID HAL_FUNC_INTERNAL hal_GetK1(UINT32* k1)
{
#ifdef CHIP_CPU_IS_MIPS
    asm volatile("move $2, $27\n\tsw $2, 0(%0)"::"r"((k1)):"$2");
#endif
}

