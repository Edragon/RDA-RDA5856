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
/// @file bcpu_rom.S                                                          //
/// That file provides the boot code                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "regdef.h"
#include "cp0.h"

#include "global_macros.h"

    
        
///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

    

.extern btcpu_stack_base
.extern btcpu_main_entry
.extern btcpu_int_entry

.text
.set at
//############################################
//# BTCPU BOOT at 0x4B00
//############################################
.globl  __btcpu_start
__btcpu_start:
    nop
    // ------------------------------------
    // Initialize caches
    // Flush both caches
    // ------------------------------------
    cache 0,0
    nop

	li  v0,0
    li  v1,0
    li  a2,0
    li  t2,0
    li  t4,0
    li  t5,0
    li  t6,0
    li  t7,0
    li  s0,0
    li  s1,0
    li  s2,0
    li  s3,0
    li  s4,0
    li  s5,0
    li  s6,0
    li  s7,0
    li  t8,0
    li  t9,0
    //li  k0,0 // k0 is not saved - put 0
    li  k1,0
    li  gp,0
    li  sp,0
    li  fp,0
    li  ra,0
	
    // ------------------------------------
    //  initialize stack pointer
    // ------------------------------------
    lw    sp, __btcpu_stack_base


    la    v0, __btcpu_init
    jr    v0



// ##################################################################
// # Initialization function - Must be put after Uhandler definition
// ##################################################################
.globl  __btcpu_init
__btcpu_init:
    // barely nothing to do the xcpu does the setup of the system

//  Enable ITs with handler in rom
    li      t0,    Status_BEV | Status_IntMASK | Status_IEc
    mtc0    zero, Cause
    nop
    mtc0    t0, Status

    lw      ra, __btcpu_int_handler // if return from main... error
    lw      v0, __btcpu_main_entry    // jump to the btcpu_main
    j       v0


__btcpu_stack_base: .word _bt_sram_non_romed_top
__btcpu_main_entry: .word _btflash_start
__btcpu_int_entry: .word _btrom_irq_start

//UINT32 btcpu_GetSp(void)
//{
    .globl  btcpu_GetSp
    .globl  btcpu_GetSp
    .ent    btcpu_GetSp
btcpu_GetSp:
btcpu_GetSp:    
    .frame sp, 0, ra

    move    v0,sp

    j    ra

    .type   btcpu_GetSp,@function
    .size   btcpu_GetSp,.-btcpu_GetSp

    .end btcpu_GetSp
//}



. = (__btcpu_start | 0x180)

// #####################################
// # rom interrupt handler is at 0x4180 (0x4000 | 0x180)
// #####################################
.globl __btcpu_int_handler
__btcpu_int_handler:
    // ------------------------------------
    //  Get the irq handler
    // ------------------------------------
    lw      k0, __btcpu_int_entry
    j       k0


