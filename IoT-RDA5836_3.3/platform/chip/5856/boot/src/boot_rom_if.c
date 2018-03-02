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
/// @file                                                                     //
/// That file contains the function called by rom but implement in flash
/// a fixed location in ROM.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "hal_debug.h"
#include "hal_sys.h"
#include <stdarg.h>
#include <string.h>


void (*g_pPrintf_ROM) (u32 Id, const ascii *Fmt, va_list argPtr);

#if CHIP_HAS_BTCPU==1
void sxs_vprintf (u32 Id, const ascii *Fmt, va_list argPtr)
{
    if(g_pPrintf_ROM)
    {
        g_pPrintf_ROM(Id, Fmt, argPtr);
    }
}
#endif
VOID hal_GetK1(UINT32* k1)
{
    asm volatile("move $2, $27\n\tsw $2, 0(%0)"::"r"((k1)):"$2");
}



