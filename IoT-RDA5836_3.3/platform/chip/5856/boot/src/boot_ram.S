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
/// @file boot_ram.S                                                          //
/// That file defines the global variables in memory used by the romed boot   //
/// code.                                                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "regdef.h"
#include "cp0.h"

//#################################################################################
// Global variables in RAM
//#################################################################################
.data
// Xcpu variables
.globl xcpu_error_code
.globl xcpu_error_status
.globl xcpu_sp_context
.globl sxr_IrqStack_pointer

// Reset cause
.globl g_bootResetCause
// Boot mode
.globl g_bootBootMode

xcpu_error_code:         .word 0xc0010000
xcpu_error_status:       .word 0xc001c001
xcpu_sp_context:         .word 0x0
sxr_IrqStack_pointer:    .word _sys_sram_non_romed_top
g_bootResetCause:        .word 0x0
g_bootBootMode:          .word 0x0


