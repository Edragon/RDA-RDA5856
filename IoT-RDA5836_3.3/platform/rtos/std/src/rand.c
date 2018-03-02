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


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file rand.c                                                              //
/// Standard pseudo random generator                                          //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 

#include "stdlib.h"
#include "hal_mem_map.h"

PRIVATE unsigned long int HAL_DATA_BACKUP g_currentRandValue = 1;

// ============================================================================
// srand
// ----------------------------------------------------------------------------
/// Initializes the generator
// ============================================================================
void srand(unsigned int init)
{
    g_currentRandValue = init;
}

// ============================================================================
// rand
// ----------------------------------------------------------------------------
/// Generates a new value
// ============================================================================
#ifdef CHIP_CPU_IS_ARM
__weak int rand()
#else
int rand()
#endif
{
    //Based on Knuth "The Art of Computer Programming"
    g_currentRandValue = g_currentRandValue * 1103515245 + 12345;
    return ( (unsigned int) (g_currentRandValue / 65536) % (RAND_MAX+1) );
}
