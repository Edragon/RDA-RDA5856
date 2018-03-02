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
///     @file   hal_lps_utils.S 
///     This file defines the low level functions for lps Deep Sleep.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



//  HARDWARE DEFINITION 
#include "regdef.h"
#include "cp0.h"
#include "global_macros.h"
#include "sys_irq_asm.h"


#include "halp_gdb_stub.h"
#include "halp_irq_handler.h"
#include "chip_id.h"

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#include "pmuc_asm.h"
#endif




.extern sxr_IrqStack_pointer
.extern hal_LpsDeepSleep

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
.extern hal_SysXcpuSleep
#endif

#ifdef __mips16
//  This file contains 32 bit assembly code.  
    .set nomips16
#endif



// =============================================================================
// hal_LpsDeepSleepWrapper
// -----------------------------------------------------------------------------
/// Have the XCPU entered in critical section.
/// Set the stack to the IRQ Stack (in internal ram)
/// call hal_LpsDeepSleep() with unchanged parameters
///   hal_LpsDeepSleep() does the low power sleep sequence
/// Resore Stack & critical section
/// basically the prototype should be the one of hal_LpsDeepSleep() as this
/// wrapper is only some piece of assembly code.
///
/// Note: The critical section used here is the CPU's one not the IRQ Controller
/// to prevent all IRQ source (debug interrupts, page interrupt...) during low
/// power. So that the power up sequence is executed before executing the
/// interrupt code.
///
/// @param die : If TRUE, the sytem will "die" in a low power state
/// This is for Idle power test purpose one. In regular usage always use FALSE
/// @return the irq cause register value, 0 is a normal LPS wake up
/// =============================================================================
// UINT32 hal_LpsDeepSleepWrapper (BOOL die)
// {
    .globl    hal_LpsDeepSleepWrapper
    .ent    hal_LpsDeepSleepWrapper
hal_LpsDeepSleepWrapper:    
    .frame    sp, 32, ra
//    .frame    t0, 32, ra
    // Have the XCPU entered in critical section.
    // use XCPU critical section (avoid debug irqs and page irq while in lp)
    mfc0    t1, Status
    li      t0, ~1
    and     t0, t1
    mtc0    t0, Status
    .set noat
    li      AT, LPS_STACK_FLAG
    or      k1, k1, AT
    .set at
    // reusing IRQ Stack Pointer
    lw      t0, sxr_IrqStack_pointer
    add     t0, t0, -32
    sw      t1, 24(t0)
    sw      sp, 20(t0)
    sw      ra, 16(t0)
    move    sp, t0
    // call the LPS function
    jal     hal_LpsDeepSleep
    // restore
    .set noat
    li      AT, ~LPS_STACK_FLAG
    and     k1, k1, AT
    .set at
    lw      t1, 24(sp)
    lw      ra, 16(sp)
    lw      sp, 20(sp)
    mtc0    t1, Status
    jr  ra

    .type    hal_LpsDeepSleepWrapper,@function
    .size    hal_LpsDeepSleepWrapper,.-hal_LpsDeepSleepWrapper
    .end    hal_LpsDeepSleepWrapper
// }

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
.globl g_savecontextAddr
.globl boot_save_context
.ent boot_save_context
boot_save_context:
// ------------------------------------------------
// save context on current stack
// ------------------------------------------------

//  make room on stack for extra context
    subu sp, sp, 120

    .set noat
    sw  AT, 0(sp)
    .set at
    sw  t0, 4(sp)
    sw sp, g_savecontextAddr
    //----------------------------
    // Save extra context
    //----------------------------

//  save all registers
    sw  v0, 8(sp)        # save $2
    sw  v1,12(sp)        # save $3
    sw  a1,16(sp)        # save $5
    sw  a2,20(sp)        # save $6
    sw  a3,24(sp)        # save $7
    sw  t1,28(sp)        # save $9
    sw  t2,32(sp)        # save $10
    sw  t3,36(sp)        # save $11
    sw  t4,40(sp)        # save $12
    sw  t5,44(sp)        # save $13
    sw  t6,48(sp)        # save $14
    sw  t7,52(sp)        # save $15
    sw  t8,56(sp)        # save $24
    sw  t9,60(sp)        # save $25
    sw  ra,64(sp)        # save $31

//  save HI and LO registers
    mflo t0
    mfhi t1
    sw  t0,68(sp)
    sw  t1,72(sp)

    sw  s0,76(sp)           # save $16
    sw  s1,80(sp)           # save $17
    sw  s2,84(sp)           # save $18
    sw  s3,88(sp)           # save $19
    sw  s4,92(sp)           # save $20
    sw  s5,96(sp)           # save $21
    sw  s6,100(sp)          # save $22
    sw  s7,104(sp)          # save $23
    sw  fp,108(sp)          # save $30
    sw  a0,112(sp)          # save $4
    sw  ra,116(sp)          # save Pc
    

    jal hal_SysXcpuSleep

    nop
    nop
    nop
    nop
.end   boot_save_context
#endif
