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
///     @file hal_irq_handler.S
///     Low level Irq handler
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



//  HARDWARE DEFINITION 
#include "regdef.h"
#include "cp0.h"

#include "global_macros.h"
#include "sys_irq_asm.h"
#include "page_spy_asm.h"


//#include "halp_gdb_stub.h"
//#include "halp_irq_handler.h"

// Defines
/// @todo maybe find a better place for these definitions
#define HIP_FIRST_LINE  (0x400)
#define XCPU_HIP_MAIN_IRQ   (HIP_FIRST_LINE << XCPU_MAIN_IRQ_LINE)
#define XCPU_HIP_PAGE_SPY   (HIP_FIRST_LINE << XCPU_PAGE_SPY_IRQ_LINE)
#define XCPU_HIP_DEBUG      (HIP_FIRST_LINE << XCPU_DEBUG_IRQ_LINE)
#define XCPU_HIP_HOST       (HIP_FIRST_LINE << XCPU_HOST_IRQ_LINE)

#define CTX_TYPE_FROM_JOB   (0x10)


/// @todo move the IRQ_LOCATION into a configurable
/// piece of software
#define IRQ_LOCATION 0  // 0 = ram; Status_BEV = rom

#define IRQ_IN_SRAM 

//  -------------------------------------------------- 
// Special registers usage
// k0 lost register in context restoration
// k1 keeps track of the level of IRQ
// gp used to save stack addr when jumping to irq,
// it can be changed when calling task swap from inside irq
//  -------------------------------------------------- 
//-----------------------
// small context map
//-----------------------
#ifdef __REDUCED_REGS__
    #define TASK_SMALL_CTX_SIZE (20)
    #define SC_S0        0
    #define SC_S1        4
    #define SC_A0        8
    #define SC_PC        12
    #define    SC_CTX_TYPE    16
#else
    #define TASK_SMALL_CTX_SIZE    (48)
    #define SC_S0        0
    #define SC_S1        4
    #define SC_S2        8
    #define SC_S3        12
    #define SC_S4        16
    #define SC_S5        20
    #define SC_S6        24
    #define SC_S7        28
    #define SC_FP        32
    #define SC_A0        36
    #define SC_PC        40
    #define SC_CTX_TYPE    44
#endif //  __REDUCED_REGS__ 

// use sign bit to be able to use bltz/bgez
// also use 16b value to allow sign extended immediate loading with '| 1' for SC flag, as done in IRQ handler
#define CTX_TYPE_LONG 0xffff8000

//-----------------------
// extra context map
//-----------------------
#ifdef __REDUCED_REGS__
    #define TASK_EXTRA_CTX_SIZE (56)
    #define FC_LO    0
    #define FC_HI    4
    #define FC_V0    8
    #define FC_V1    12
    #define FC_A1    16
    #define FC_A2    20
    #define FC_A3    24
    #define FC_T0    28
    #define FC_T1    32
    #define FC_T2    36
    #define FC_T8    40
    #define FC_T9    44
    #define FC_RA    48
    #define FC_AT    52
#else
    #define TASK_EXTRA_CTX_SIZE (80)
    #define FC_LO    0
    #define FC_HI    4
    #define FC_V0    8
    #define FC_V1    12
    #define FC_A1    16
    #define FC_A2    20
    #define FC_A3    24
    #define FC_T0    28
    #define FC_T1    32
    #define FC_T2    36
    #define FC_T3    40
    #define FC_T4    44
    #define FC_T5    48
    #define FC_T6    52
    #define FC_T7    56
    #define FC_T8    60
    #define FC_T9    64
    #define FC_RA    68
    #define FC_AT    72
#endif //  __REDUCED_REGS__ 
// not saved
// R0
// k0 
// k1
// GP
// FP

// GP used inside irq, saved in irq stack

// K1 is used to record the level of IRQ we are in
//  0: means no IRQ : we are on a TASK
//  1 ... n : nomber of entered interrupt
//  the following bit is set when we are in a job's task
#define JOB_STACK_FLAG  0x01000000
//  the following bit is set when we are in the deep low power function 
//  (only called from idle task)
#define LPS_STACK_FLAG  0x00001000


//-----------------------
// job context map
//-----------------------
#ifdef __REDUCED_REGS__
    #define JOB_CTX_SIZE    (24)
    #define JC_S0            0
    #define JC_S1            4
    #define JC_A0            8
    #define JC_A1            12
    #define JC_PC            16
    #define JC_IS           20
#else
    #define JOB_CTX_SIZE    (52)
    #define JC_S0           0
    #define JC_S1           4
    #define JC_S2           8
    #define JC_S3           12
    #define JC_S4           16
    #define JC_S5           20
    #define JC_S6           24
    #define JC_S7           28
    #define JC_FP           32
    #define JC_A0           36
    #define JC_A1           40
    #define JC_PC           44
    #define JC_IS           48
#endif

#define VIC_BASE  0xa1688000

//.extern initial_stack_top_var

.extern bt_irq
.extern _bt_gdb

#
# 
#ifdef __mips16
//  This file contains 32 bit assembly code.  
    .set nomips16
#endif

    .text

    .set    reorder

/// *********************************************************
///  * Function name: bt_Irq
///  ***********************************************************
///  * Type     : API
///  * Purpose  : Irq handler
///  **********************************************************
//void _bt_Irq_handler(void)
//{
    //.section .text, "awx", @progbits
    .balign 0x10

    .globl    _bt_Irq_handler
    .ent    _bt_Irq_handler
    .frame    sp, 0, ra
_bt_Irq_handler:






// ------------------------------------------------
// save context on current stack
// ------------------------------------------------

    subu sp, sp, TASK_EXTRA_CTX_SIZE

//  Put 4 instructions in the "stub" to make better use of the cache 
    .set noat
    sw  AT,FC_AT(sp)
    .set at
    sw  t0,FC_T0(sp)

//  li  t0, 0xa1688000
//  sw  zero, 0x2c(t0)



    //----------------------------
    // Save extra context
    //----------------------------
//  make room on stack for extra context

//  save all registers 
    sw  v0,FC_V0(sp)        # save $2
    sw  v1,FC_V1(sp)        # save $3
    sw  a1,FC_A1(sp)        # save $5
    sw  a2,FC_A2(sp)        # save $6
    sw  a3,FC_A3(sp)        # save $7
    sw  t1,FC_T1(sp)        # save $9
    sw  t2,FC_T2(sp)        # save $10
#ifndef __REDUCED_REGS__
    sw  t3,FC_T3(sp)        # save $11
    sw  t4,FC_T4(sp)        # save $12
    sw  t5,FC_T5(sp)        # save $13
    sw  t6,FC_T6(sp)        # save $14
    sw  t7,FC_T7(sp)        # save $15
#endif
    sw  t8,FC_T8(sp)        # save $24
    sw  t9,FC_T9(sp)        # save $25
    sw  ra,FC_RA(sp)        # save $31

//  save HI and LO registers 
    mflo t0
    mfhi t1
    sw  t0, FC_LO(sp)
    sw  t1, FC_HI(sp)

    
    //----------------------------
    // Save small context
    //----------------------------
//  make room on stack for small context 
    subu sp, sp, TASK_SMALL_CTX_SIZE
    sw    s0,SC_S0(sp)           # save $16
    sw    s1,SC_S1(sp)           # save $17
#ifndef __REDUCED_REGS__
    sw    s2,SC_S2(sp)           # save $18
    sw    s3,SC_S3(sp)           # save $19
    sw    s4,SC_S4(sp)           # save $20
    sw    s5,SC_S5(sp)           # save $21
    sw    s6,SC_S6(sp)           # save $22
    sw    s7,SC_S7(sp)           # save $23
    sw    fp,SC_FP(sp)           # save $30
#endif
    sw    a0,SC_A0(sp)           # save $4

    mfc0    t0,EPC
    li     t1, CTX_TYPE_LONG | 1 // long context (<0) + 1 not in critical section
    sw    t0,SC_PC(sp)           # save Pc

#if 0
    blt k1, JOB_STACK_FLAG, $L_skip_save_job_flag
    or  t1, t1, CTX_TYPE_FROM_JOB
$L_skip_save_job_flag:
#endif

    sw    t1,SC_CTX_TYPE(sp)       # save CTX Type (>=0 is from function - <0 is From IT)

// ------------------------------------------------
//     End save context
// ------------------------------------------------


// ------------------------------------------------
// Go to Irq stack if needed
// ------------------------------------------------
    move  s0,sp                 # sp where the context has been saved stored in s0

#if 0
$L_comming_from_task:
        // coming from task
        lw t0, sxr_IrqStack_pointer
        nop
        move sp, t0

#endif

$L_comming_from_irq:

// ------------------------------------------------
// Get IRQ cause and select function to call
// ------------------------------------------------
    mfc0 t1,Cause

    addi k1,k1,1

//  read cause register 
#ifndef USE_GDB_PAGE_IRQ
//  extract exception code + debug irq + host irq 
    andi t0,t1,(XCPU_HIP_DEBUG|XCPU_HIP_HOST|Cause_ExcCode)
#else
//  extract exception code + debug irq + host irq + page irq 
    andi t0,t1,(XCPU_HIP_DEBUG|XCPU_HIP_HOST|XCPU_HIP_PAGE_SPY| Cause_ExcCode)
#endif

    mfc0    a0,EPC
	move 	a1, t1
	andi 	a2, t1, Cause_ExcCode
	srl     a2, a2, 2
	move	a3, k1

#if 1
    bnez t0,_go_to_gdb
#endif


//---------------------------------
// regular IT function
//---------------------------------
    // create stack frame for function call + gp restoration
    subu    sp, sp, 24
    sw        gp, 20(sp)
    // get the stack to restore in gp
    move    gp, s0

#if 0
    // disable it in ctrl
    li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    lw  zero,   REG_SYS_IRQ_SC(t0)
#endif

    // enable it in risc
//    li  t0, (Status_IntMASK | Status_IEc | Status_BEV)
    li  t0, (Status_IntMASK | Status_IEp | Status_BEV)

//add sc lock, by xzc

//	li  $8, VIC_BASE
//  	sw  $0,  0x2c($8)


    mfc0    a0,EPC
    mfc0    a1,Cause
.set noreorder
  //  jal bt_irq
  // jal TEST1_LSLCirq_BT_TIM2_Handler
  // jal SYSirq_Handle
  //jal bt_irq_point_func
    la t1, SYSirq_Handle
    jalr t1
    mtc0    t0, Status
.set reorder



    // disable it in risc - set prev irq bit to 1 so that rfe will have irq enabled
    li  t0, (Status_IntMASK | Status_IEp | Status_BEV)
    mtc0  t0, Status

//add sc un-lock, by xzc
//	li  $8, VIC_BASE
//  	sw  $9, 0x2c($8)



    // get the future stack from gp (this could have been updated in TaskSwap)
    move    v0, gp
    // restore stack frame
    lw        gp, 20(sp)
    // useless sp will be taken from v0 later
//    addu    sp, 24


// hopefully from here we are no longer interruptible, but exception can occur
// don't step bellow this
_srx_irq_handler_end_dont_step_in_here:

// restore context associated to scheduled task
//----------------------------
// restore context
//----------------------------

    // v0 is setup to the sp addr of the scheduled task by TaskSwitch
    // or is the value saved before

    lw    s0,SC_S0(v0)           # restore $16
    lw    s1,SC_S1(v0)           # restore $17
#ifndef __REDUCED_REGS__
    lw    s2,SC_S2(v0)           # restore $18
    lw    s3,SC_S3(v0)           # restore $19
    lw    s4,SC_S4(v0)           # restore $20
    lw    s5,SC_S5(v0)           # restore $21
    lw    s6,SC_S6(v0)           # restore $22
    lw    s7,SC_S7(v0)           # restore $23
    lw    fp,SC_FP(v0)           # restore $30
#endif
    lw    a0,SC_A0(v0)           # restore task param
    lw    k0,SC_PC(v0)           # restore PC
    lw    t1,SC_CTX_TYPE(v0)           # restore CTX type

#if 0

    and   t0,t1,CTX_TYPE_FROM_JOB
    beqz t0,$L_skip_restore_job_flag
    or    k1,k1,JOB_STACK_FLAG
$L_skip_restore_job_flag:

#endif

#if 0
    // Restore Critical Section Status
    li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    // store context direcly the hardware will check bit 0 only
    sw  t1, REG_SYS_IRQ_SC(t0)
#endif

// ------------------------------------------------
// Go to task stack if needed
// ------------------------------------------------
    addi k1,k1,-1

    addi    sp, v0, TASK_SMALL_CTX_SIZE

    // if context is small, we are done
    // if context is big, restore other registers
    bgez t1, $L_CTx_restored 

    //----------------------------------------------
    // Long restore - this task has been interrupted
    //----------------------------------------------
//  restore HI and LO registers 
    lw  t0, FC_LO(sp)
    lw  t1, FC_HI(sp)
    mtlo t0
    mthi t1
//  restore all registers 
    lw  v0,FC_V0(sp)
    lw  v1,FC_V1(sp)
    lw  a1,FC_A1(sp)
    lw  a2,FC_A2(sp)
    lw  a3,FC_A3(sp)
    lw  t2,FC_T2(sp)
#ifndef __REDUCED_REGS__
    lw  t3,FC_T3(sp)
    lw  t4,FC_T4(sp)
    lw  t5,FC_T5(sp)
    lw  t6,FC_T6(sp)
    lw  t7,FC_T7(sp)
#endif
    lw  ra,FC_RA(sp)
    lw  t8,FC_T8(sp)
    lw  t9,FC_T9(sp)
    .set noat
    lw  AT,FC_AT(sp)
    .set at

//  li  t0, 0xa1688000
 // li  t1,0x1
//  sw  t1, 0x2c(t0)

    
    lw  t0,FC_T0(sp)
    lw  t1,FC_T1(sp)

    // Free extra space on stack
    addu    sp, sp, TASK_EXTRA_CTX_SIZE

    //la    k0, bt_return
//    addu k0,k0,2
$L_CTx_restored:
    .set noreorder
    jr    k0
    rfe
    .set reorder

_go_to_gdb:
	 jal   _bt_gdb


    .type    _bt_Irq_handler,@function
    .size    _bt_Irq_handler,.-_bt_Irq_handler
    .end    _bt_Irq_handler

//}



//  ********************************************************** 
//               END INTERRUPT TREATMENT                       
//  ********************************************************** 


//  ********************************************************** 
//               INTERRUPT DRIVER                              
//  ********************************************************** 
// IRQ initialization is done by the bt_IrqInit function.

/// *********************************************************
///  * Function name: bt_IrqInit
///  ***********************************************************
///  * Type        : API
///  * Purpose    : Initialize the IRQ
///  **********************************************************
//void bt_IrqInit(void) 
//{
    .globl    bt_IrqInit
    .ent    bt_IrqInit
bt_IrqInit:    
    .frame    sp, 0, ra

    // configure Irq Stack
//    lw    t0, initial_stack_top_var
    // Disable debug uart IRQ. This IRQ is only
    // used to wake-up the system when in idle,
    // we don't want to handle it dynamically.
    li    t1, (SYS_IRQ_STATUS_MASK &~ SYS_IRQ_SYS_IRQ_DEBUG_UART)
//    sw    t0, sxr_IrqStack_pointer

    // irq enter SC, set mask
    li  t0, REG_SYS_IRQ_BASE_HI
    lw  zero, REG_SYS_IRQ_SC(t0)
    sw  t1, REG_SYS_IRQ_MASK_SET(t0)

    // xcpu enable irq
    mtc0  zero, Cause
    li  t8, (Status_IntMASK | Status_IEc | IRQ_LOCATION)
    mtc0  t8, Status

    li    t2,1

//  tell the irq handler that we are know in irq context (no task stack available)
    li    k1, 1

#if 0
    // exit SC
    // t0 still REG_SYS_IRQ_BASE_HI
    // store context direcly the hardware will check bit 0 only
    sw  t2, REG_SYS_IRQ_SC(t0)
#endif

    jr    ra

    .type    bt_IrqInit,@function
    .size    bt_IrqInit,.-bt_IrqInit
    .end bt_IrqInit
//}



