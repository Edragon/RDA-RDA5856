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


#include "halp_gdb_stub.h"
#include "halp_irq_handler.h"
#ifdef CHIP_5856E
#ifdef CT_NO_DEBUG
#include "wdt_asm.h"
#endif
#endif
// Defines
/// @todo maybe find a better place for these definitions
#define HIP_FIRST_LINE  (0x400)
#define XCPU_HIP_MAIN_IRQ   (HIP_FIRST_LINE << XCPU_MAIN_IRQ_LINE)
#define XCPU_HIP_PAGE_SPY   (HIP_FIRST_LINE << XCPU_PAGE_SPY_IRQ_LINE)
#define XCPU_HIP_DEBUG      (HIP_FIRST_LINE << XCPU_DEBUG_IRQ_LINE)
#define XCPU_HIP_HOST       (HIP_FIRST_LINE << XCPU_HOST_IRQ_LINE)

#define CTX_TYPE_FROM_JOB   (0x10)


.extern initial_stack_top_var

#ifdef CHIP_5856E
#ifdef CT_NO_DEBUG
.globl  g_wdtStatus
#endif
#endif

#
# 
#ifdef __mips16
//  This file contains 32 bit assembly code.  
    .set nomips16
#endif

    .text

    .set    reorder

/// *********************************************************
///  * Function name: sxr_Irq
///  ***********************************************************
///  * Type     : API
///  * Purpose  : Irq handler
///  **********************************************************
//void _sxr_Irq_handler(void)
//{
    .section .irqtext, "awx", @progbits
    .balign 0x10

    .globl    _sxr_Irq_handler
    .ent    _sxr_Irq_handler
    .frame    sp, 0, ra
_sxr_Irq_handler:






#ifdef PAGE_SPY_LOG
    mfc0 k0,Cause
    andi k0, k0, XCPU_HIP_PAGE_SPY
    bnez k0, __trace_write_with_page_spy
    nop
#endif






// ------------------------------------------------
// save context on current stack
// ------------------------------------------------

    subu sp, sp, TASK_EXTRA_CTX_SIZE

//  Put 4 instructions in the "stub" to make better use of the cache 
    .set noat
    sw  AT,FC_AT(sp)
    .set at
    sw  t0,FC_T0(sp)

#ifdef CHIP_5856E
#ifdef CT_NO_DEBUG
	lw	t0, 	REG_wdogintrawa
	sw  t0,	    g_wdtStatus
    li	t0, 	1
    sw  t0,	    REG_wdogcleara
#endif
#endif


#ifdef PAGE_SPY_LOG
    mfc0    t0,EPC
    sw    t0,(SC_PC - TASK_SMALL_CTX_SIZE)(sp)           # save Pc 

    // This context saving on stack will trig the 
    // page spy if the IRQ occurs during a job
    // We need to reenable exception asap to catch
    // those writings before GDB below
    // We will only enable the page spy
    // (well, not really, but that's the only one that 
    // will occur. The writings caused by the saving of those
    // two regs will not be saved in the PS_buffer ... :(

    // disable it in ctrl
#ifndef CHIP_5856E
    li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    lw  zero,   REG_SYS_IRQ_SC(t0)
#else
    //li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    lw  zero,   REG_sc
#endif
    // enable it in risc
    li  t0, (Status_IntMASK | Status_IEc | IRQ_LOCATION)
    mtc0    t0, Status
#endif  






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

#ifndef PAGE_SPY_LOG
    mfc0    t0,EPC
#endif
    li     t1, CTX_TYPE_LONG | 1 // long context (<0) + 1 not in critical section
#ifndef PAGE_SPY_LOG
    sw    t0,SC_PC(sp)           # save Pc 
#endif

    blt k1, JOB_STACK_FLAG, $L_skip_save_job_flag
    or  t1, t1, CTX_TYPE_FROM_JOB
$L_skip_save_job_flag:

    sw    t1,SC_CTX_TYPE(sp)       # save CTX Type (>=0 is from function - <0 is From IT)

// ------------------------------------------------
//     End save context
// ------------------------------------------------


// ------------------------------------------------
// Go to Irq stack if needed
// ------------------------------------------------
    move  s0,sp                 # sp where the context has been saved stored in s0

    beqz k1,$L_comming_from_task
        blt k1, JOB_STACK_FLAG, $L_comming_from_irq
            .set noat
            li      AT, ~JOB_STACK_FLAG
            and     k1, k1, AT
            .set at
$L_comming_from_task:
        // coming from task
        lw t0, sxr_IrqStack_pointer
        nop
        move sp, t0

$L_comming_from_irq:

// ------------------------------------------------
// Get IRQ cause and select function to call
// ------------------------------------------------
    mfc0 t1,Cause
#if 0
    li t0, 0xa1a2c008
    li t2, 0x16
    sw t2, 0(t0)
    

$L_comming_from_irq_eeee:
    bnez t2,$L_comming_from_irq_eeee
    
    nop

#endif   
    addi k1,k1,1

//  read cause register 
#ifndef USE_GDB_PAGE_IRQ
//  extract exception code + debug irq + host irq 
    andi t0,t1,(XCPU_HIP_DEBUG|XCPU_HIP_HOST|Cause_ExcCode)
#else
//  extract exception code + debug irq + host irq + page irq 
    andi t0,t1,(XCPU_HIP_DEBUG|XCPU_HIP_HOST|XCPU_HIP_PAGE_SPY| Cause_ExcCode)
#endif
    
    bnez t0,_go_to_gdb 

//---------------------------------
// regular IT function
//---------------------------------
#ifndef CT_C_USE_GP
    // create stack frame for function call + gp restoration
    subu    sp, sp, 24
    sw        gp, 20(sp)
    // get the stack to restore in gp
    move    gp, s0
#else
    // create stack frame for function call + task stack restoration
    subu    sp, sp, 24
    lw        t0, sxr_Irq_TaskStack_pointer
    sw        t0, 20(sp)
    // get the stack to restore in sxr_Irq_TaskStack_pointer
    sw        s0, sxr_Irq_TaskStack_pointer
#endif
    
        

    // disable it in ctrl
#ifndef CHIP_5856E
    li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    lw  zero,   REG_SYS_IRQ_SC(t0)
#else
    //li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    lw  zero,   REG_sc
#endif
    // enable it in risc
    li  t0, (Status_IntMASK | Status_IEc | IRQ_LOCATION)

.set noreorder
    jal hal_IrqDispatch
    mtc0    t0, Status
.set reorder



    // disable it in risc - set prev irq bit to 1 so that rfe will have irq enabled
    li  t0, (Status_IntMASK | Status_IEp | IRQ_LOCATION)
    mtc0  t0, Status

#ifndef CT_C_USE_GP
    // get the future stack from gp (this could have been updated in TaskSwap)
    move    v0, gp
    // restore stack frame
    lw        gp, 20(sp)
    // useless sp will be taken from v0 later
//    addu    sp, 24
#else
    // get the future stack from sxr_Irq_TaskStack_pointer
    lw        v0, sxr_Irq_TaskStack_pointer
    // restore stack frame
    lw        t0, 20(sp)
    sw        t0, sxr_Irq_TaskStack_pointer
    // useless sp will be taken from v0 later
//    addu    sp, 24
#endif


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

    and   t0,t1,CTX_TYPE_FROM_JOB
    beqz t0,$L_skip_restore_job_flag
    or    k1,k1,JOB_STACK_FLAG
$L_skip_restore_job_flag:

#ifndef CHIP_5856E
    // Restore Critical Section Status
    li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    // store context direcly the hardware will check bit 0 only
    sw  t1, REG_SYS_IRQ_SC(t0)
#else
    // Restore Critical Section Status
    //li  t0, KSEG1(REG_SYS_IRQ_BASE_HI)
    // store context direcly the hardware will check bit 0 only
    sw  t1, REG_sc
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
    lw  t0,FC_T0(sp)
    lw  t1,FC_T1(sp)

    // Free extra space on stack
    addu    sp, sp, TASK_EXTRA_CTX_SIZE

$L_CTx_restored :
    .set noreorder
    jr    k0
    rfe
    .set reorder

//  ***** 
//   GDB  
//  ***** 
#define GDB_REG00 0x00
#define GDB_REG01 0x04
#define GDB_REG02 0x08
#define GDB_REG03 0x0c
#define GDB_REG04 0x10
#define GDB_REG05 0x14
#define GDB_REG06 0x18
#define GDB_REG07 0x1c
#define GDB_REG08 0x20
#define GDB_REG09 0x24
#define GDB_REG10 0x28
#define GDB_REG11 0x2c
#define GDB_REG12 0x30
#define GDB_REG13 0x34
#define GDB_REG14 0x38
#define GDB_REG15 0x3c
#define GDB_REG16 0x40
#define GDB_REG17 0x44
#define GDB_REG18 0x48
#define GDB_REG19 0x4c
#define GDB_REG20 0x50
#define GDB_REG21 0x54
#define GDB_REG22 0x58
#define GDB_REG23 0x5c
#define GDB_REG24 0x60
#define GDB_REG25 0x64
#define GDB_REG26 0x68
#define GDB_REG27 0x6c
#define GDB_REG28 0x70
#define GDB_REG29 0x74
#define GDB_REG30 0x78
#define GDB_REG31 0x7c
#define GDB_REGSR 0x80
#define GDB_REGLO 0x84
#define GDB_REGHI 0x88
#define GDB_REGBAD 0x8c
#define GDB_REGCAUSE 0x90
#define GDB_REGEPC 0x94



_go_to_gdb:
    // MISSING BREAK , DBG_IT, EXCEPTION, HOST_IT
    // s0 is context_sp
    // t0, at are invalid
    // t1 is Cause
#if UNIT_TEST==0

//  make room on stack 
    subu sp, sp, HAL_GDB_CONTEXT_SIZE

//  store Cause and EPC registers 
    mfc0    t8, EPC
    sw      t1, GDB_REGCAUSE(sp)
    sw      t8, GDB_REGEPC(sp)
    // t8 must hold EPC until later

//  r1 = at from context 
    lw        t0,  TASK_SMALL_CTX_SIZE+FC_AT(s0)
    sw        r0,  GDB_REG00(sp) // this is always 0 but we want it for gdb
    .set noat
    sw        t0,  GDB_REG01(sp) 
    .set at
    sw        r2,  GDB_REG02(sp)
    sw        r3,  GDB_REG03(sp)
    sw        r4,  GDB_REG04(sp)
    sw        r5,  GDB_REG05(sp)
    sw        r6,  GDB_REG06(sp)
    sw        r7,  GDB_REG07(sp)
//  r8-r9 = t0-t1 from context 
    lw        t0,  TASK_SMALL_CTX_SIZE+FC_T0(s0)
    sw        t0,  GDB_REG08(sp)
    lw        t0,  TASK_SMALL_CTX_SIZE+FC_T1(s0)
    sw        t0,  GDB_REG09(sp)
//  
    sw        r10, GDB_REG10(sp)
    sw        r11, GDB_REG11(sp)
    sw        r12, GDB_REG12(sp)
    sw        r13, GDB_REG13(sp)
    sw        r14, GDB_REG14(sp)
    sw        r15, GDB_REG15(sp)
//  r16 = s0 from context 
    lw        t0, SC_S0(s0)
    sw        r17, GDB_REG17(sp)
    sw        t0, GDB_REG16(sp)
    sw        r18, GDB_REG18(sp)
    sw        r19, GDB_REG19(sp)
    sw        r20, GDB_REG20(sp)
    sw        r21, GDB_REG21(sp)
    sw        r22, GDB_REG22(sp)
    sw        r23, GDB_REG23(sp)
//  r24 = t8 from context 
    lw        t0,  TASK_SMALL_CTX_SIZE+FC_T8(s0)
    sw        t0,  GDB_REG24(sp)
//  
    sw        r25, GDB_REG25(sp)
//  k0 is destroyed saved 0 for gdb 
    sw        r0, GDB_REG26(sp)
    addi    t0,k1,-1
    sw        t0, GDB_REG27(sp)
    sw        r28, GDB_REG28(sp)
//  r29 original sp calculated here 
    addu    s0, s0, TASK_SMALL_CTX_SIZE+TASK_EXTRA_CTX_SIZE
    sw        s0, GDB_REG29(sp)
    sw        r30, GDB_REG30(sp)
    sw        r31, GDB_REG31(sp)
//  store HI and LO registers 
    .set noat
    mflo    t0
    mfhi    AT
    sw        t0, GDB_REGLO(sp)
    sw        AT, GDB_REGHI(sp)
    .set at
//  store Status, BadVAddr 
    .set noat
    mfc0    t0, Status
    mfc0    AT, BadVAddr
    sw      t0, GDB_REGSR(sp)
    sw      AT, GDB_REGBAD(sp)
    .set at
    
//  save stack pointer position when context saved 
    la        t0, xcpu_sp_context
    sw        sp, 0(t0) 

//  frame for paramters 
    subu    sp, sp, 16


// Use the internal rom handler for GDB exceptions
// as this will put the PC in a while one in the ROM irq Handler.
// As the previous status was stored in GDB_REGSR the normal restoration
// will put back Status.
#if 1
    li  t0, (Status_IntMASK | Status_IEp | Status_BEV)
    mtc0  t0, Status
#endif

    li        a0, 0                    //  break parameter is cpu id
    
    andi    t0, t1, Cause_ExcCode     //  extract exception code 
    bne        t0, ExcCode_BREAK, $L_gdb_skip_break

    // t8 still holds EPC
    andi    t0, t8, 1                //  check for mips16 ISA 
    beqz    t0, $L_isa32_bp
$L_isa16_bp:
    lhu        t0, -1(t8)                //  t8 holds &EPC+1 in mips16 
    srl        t0, 3                    
    j         $L_isa_common
$L_isa32_bp:
    // branch to breakpoint
    lw      t0,    0(t8)                //  t8 is still epc, get instruction 
    srl     t0,    14
$L_isa_common:
    la      t8,    hal_GdbGBreak        //  get break table address 
    andi    t0,    t0,    0x3c
    addu    t0, t0,    t8                //  add break number 
    lw        t0,    0(t0)
    jalr     t0                        //  call the implementation function 
    j        $L_gdb_skip_irq
$L_gdb_skip_break:
    beqz    t0, $L_gdb_skip_exception        //  an exception? 
    move    a1, t1                            //  cause 
//    la        t0, hal_GdbException
//    jalr    t0
    jal        hal_GdbException
    j        $L_gdb_skip_irq
$L_gdb_skip_exception:
#ifdef USE_GDB_PAGE_IRQ
//    here whe have one of the upper exceptions
    andi    t0, t1, XCPU_HIP_PAGE_SPY
    beqz    t0, $L_skip_page
    jal        hal_GdbIrqPage
    j        $L_gdb_skip_irq
$L_skip_page:
#endif // USE_GDB_PAGE_IRQ

// Here we have the call to the host command handler in case
// of host exception (ie host interrrupt)
    andi    t0, t1, XCPU_HIP_HOST
    beqz    t0, $L_skip_host
    jal     hal_HstRamClientIrq //hal_HstCmdBasicHandler
    j        $L_gdb_skip_irq
$L_skip_host:

//    la        t0, hal_GdbIrqDebug
//    jalr    t0
    jal        hal_GdbIrqDebug
$L_gdb_skip_irq:

//  frame for parameters 
    addu    sp, sp, 16


//  restore gdb context 

//  Tag xcpu_sp_context as "There is no saved GDB context, ie, 
//  we are not in GDB"
    la        t0, xcpu_sp_context
    sw        r0, 0(t0) 



    move k0, sp
//  free stack 
    move sp, s0
//  restore HI and LO registers - we don't need to hold the cause in t1 
    lw  t0, GDB_REGLO(k0)
    lw  t1, GDB_REGHI(k0)
    mtlo t0
    mthi t1
//  restore status 
    lw    t0, GDB_REGSR(k0)
    mtc0    t0, Status
//  restore all registers 
    lw    r31, GDB_REG31(k0)
    lw    r30, GDB_REG30(k0)
    // sp is restored later
    // lw    r29, GDB_REG29(k0)
    // gp is not restored
    // lw    r28, GDB_REG28(k0)
    // k0 and k1 are not restored
    // lw    r27, GDB_REG27(k0)
    // lw    r26, GDB_REG26(k0)
    lw    r25, GDB_REG25(k0)
    lw    r24, GDB_REG24(k0)
    lw    r23, GDB_REG23(k0)
    lw    r22, GDB_REG22(k0)
    lw    r21, GDB_REG21(k0)
    lw    r20, GDB_REG20(k0)
    lw    r19, GDB_REG19(k0)
    lw    r18, GDB_REG18(k0)
    lw    r17, GDB_REG17(k0)
    lw    r16, GDB_REG16(k0)
    lw    r15, GDB_REG15(k0)
    lw    r14, GDB_REG14(k0)
    lw    r13, GDB_REG13(k0)
    lw    r12, GDB_REG12(k0)
    lw    r11, GDB_REG11(k0)
    lw    r10, GDB_REG10(k0)
    lw    r9,  GDB_REG09(k0)
    lw    r8,  GDB_REG08(k0)
    lw    r7,  GDB_REG07(k0)
    lw    r6,  GDB_REG06(k0)
    lw    r5,  GDB_REG05(k0)
    lw    r4,  GDB_REG04(k0)
    lw    r3,  GDB_REG03(k0)
    lw    r2,  GDB_REG02(k0)
    .set noat
    lw    r1,  GDB_REG01(k0) 
    .set at

//  free stack 
    //addu sp, sp, HAL_GDB_CONTEXT_SIZE
    //lw    sp, GDB_REG29(k0)
//  get return address 
    lw  k0, GDB_REGEPC(k0)
    
//  0 when tack stack, else It 
    addi k1,k1,-1

    .set noreorder
    j   k0                 // go back
    rfe
    .set reorder


#endif // UNIT_TEST==1













#define PS_T0   0
#define PS_T1   4
#define PS_A0   8
#define PS_A1   12

#define PS_BUFFER_STAMP     0
#define PS_BUFFER_EPC       4
#define PS_BUFFER_CONTENT    8
#define PS_BUFFER_ADDR      12
#define PS_BUFFER_SAMPLE_SIZE   16


#ifdef PAGE_SPY_LOG
__trace_write_with_page_spy:
    .set noat
    // Use k0 as the temp stack pointer
    //  g_SpBufferPtr and g_SpBufferTop are inited in
    // hal_Open
    la k0, _sys_sram_sp_stack_start

    // Save ctx
    sw      t0, PS_T0(k0)
    sw      t1, PS_T1(k0)
    sw      a0, PS_A0(k0)
    sw      a1, PS_A1(k0)

    lw  a0, g_SpBufferPtr

    mfc0    t0,EPC
    sw      t0, PS_BUFFER_EPC(a0)
    
    li      a1, REG_PAGE_SPY_BASE_HI
    lw      t0, REG_PAGE_SPY_PAGE_4_ADDR(a1)
    sw      t0, PS_BUFFER_ADDR(a0)
    
    // get rid of lowest 2 bits
    li      t1, 0x80000000
    not     t0
    ori     t0, 0x3
    not     t0
    or      t0,t0, t1

    lw      t0, 0(t0)
    sw      t0, PS_BUFFER_CONTENT(a0)

    lw      t0, g_SpBufferStamp
    sw      t0, PS_BUFFER_STAMP(a0)

    //  update pointer and Test overflow
    addi    a0, a0, PS_BUFFER_SAMPLE_SIZE
    addi    t0, t0, 1
    la      t1, g_SpBufferStamp
    sw      t0, 0(t1)
    // FIXME 
    andi    t0, t0, 0x7ff
    bnez    t0, __ps_exit
        // Otherwise, we stored the 128 samples
        // possible in the buffer so we wrap
        lw a0, g_SpBufferTop

__ps_exit:
    la t1, g_SpBufferPtr
    sw a0, 0(t1)
    // Clear PS interrupt
    // a1 still holds the page spy base address
    li t0, PAGE_SPY_ENABLE_PAGE_4
    // Disable
    sw t0, REG_PAGE_SPY_DISABLE(a1)
    // Re-enable
    sw t0, REG_PAGE_SPY_ENABLE(a1)



    // Restore ctx
    lw      t0, PS_T0(k0)
    lw      t1, PS_T1(k0)
    lw      a0, PS_A0(k0)
    lw      a1, PS_A1(k0)

    // Return from interrupt
    mfc0    k0,EPC
    .set noreorder
    j   k0                 // go back
    rfe
    .set reorder
    .set at
#endif




























    .type    _sxr_Irq_handler,@function
    .size    _sxr_Irq_handler,.-_sxr_Irq_handler
    .end    _sxr_Irq_handler

//}



//  ********************************************************** 
//               END INTERRUPT TREATMENT                       
//  ********************************************************** 


//  ********************************************************** 
//               INTERRUPT DRIVER                              
//  ********************************************************** 
// IRQ initialization is done by the sxr_TaskFirst function.



//  ********************************************************* 
//                     Global Vars                            
//  ********************************************************* 

    .section .sramdata, "aw", @progbits

.globl sxr_IrqStack_pointer
.globl g_SpBufferPtr
.globl g_SpBufferTop
.globl g_SpBufferStamp

g_SpBufferPtr: .word 0
g_SpBufferTop: .word 0
g_SpBufferStamp: .word 0

#ifdef CT_C_USE_GP
.globl sxr_Irq_TaskStack_pointer
sxr_Irq_TaskStack_pointer:    .word 0
#endif




