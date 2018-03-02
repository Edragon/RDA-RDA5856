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
/// @file hal_gdb.S
/// HAL GDB Assembly functions and variables
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "regdef.h"
#include "cp0.h"
#include "halp_gdb_stub.h"

// =============================================================================
// hal_DbgGdbPrintf
// -----------------------------------------------------------------------------
/// Print to the gdb console
/// @param string Pointer to a string to print to a gdb console
// =============================================================================
///PUBLIC VOID hal_DbgGdbPrintf(CHAR *string);

//  Either mips16 or 32, no change needed 
    SET_P_MIPS16
    .globl    hal_DbgGdbPrintf
    .globl    dbg_GdbPrint
    .ent    hal_DbgGdbPrintf
    .frame    sp, 0, ra
hal_DbgGdbPrintf:
dbg_GdbPrint:
    break    15
    jr        ra
    nop
    .type    hal_DbgGdbPrintf,@function
    .size    hal_DbgGdbPrintf,.-hal_DbgGdbPrintf
    .end    hal_DbgGdbPrintf



//  ********************************************************** 
//               GDB STUB                                      
//  ********************************************************** 
    .data

    .globl  hal_GdbGBreak
hal_GdbGBreak:
    .word   hal_GdbBreaks
    .word   hal_GdbBreakSoftBreakPoint
    .word   hal_GdbBreakRaise
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreakBreakPoint
    .word   hal_GdbBreaks
    .word   hal_GdbBreakDivBy0
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreaks
    .word   hal_GdbBreakPrint

    /*
//  GDB DATA for bb 
    
.align 2
    .globl    __bb_gdb_context
__bb_gdb_context:
    .space HAL_GDB_CONTEXT_SIZE

    */

