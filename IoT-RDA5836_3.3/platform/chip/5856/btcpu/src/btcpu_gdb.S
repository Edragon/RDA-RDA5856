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
/// @file btcpu_Gdb.S
/// HAL GDB Assembly functions and variables
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "regdef.h"
#include "cp0.h"
#include "btcpu_gdb_stub.h"


//  HARDWARE DEFINITION 
#include "regdef.h"
#include "cp0.h"

#include "global_macros.h"
#include "sys_irq_asm.h"
#include "page_spy_asm.h"


//#include "halp_gdb_stub.h"
//#include "halp_irq_handler.h"

/// @todo maybe find a better place for these definitions
#define HIP_FIRST_LINE  (0x400)
#define XCPU_HIP_MAIN_IRQ   (HIP_FIRST_LINE << XCPU_MAIN_IRQ_LINE)
#define XCPU_HIP_PAGE_SPY   (HIP_FIRST_LINE << XCPU_PAGE_SPY_IRQ_LINE)
#define XCPU_HIP_DEBUG      (HIP_FIRST_LINE << XCPU_DEBUG_IRQ_LINE)
#define XCPU_HIP_HOST       (HIP_FIRST_LINE << XCPU_HOST_IRQ_LINE)

#define CTX_TYPE_FROM_JOB   (0x10)


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


#define btcpu_Gdb_REGNUM           38
//  all register + command and parameter for command 
#define btcpu_Gdb_REGNUM_XTD       (btcpu_Gdb_REGNUM+3)
#define btcpu_Gdb_CONTEXT_SIZE     (btcpu_Gdb_REGNUM_XTD*4)

#define btcpu_Gdb_CMD_OFF          btcpu_Gdb_REGNUM
#define btcpu_Gdb_CMDPARAM_OFF     (btcpu_Gdb_REGNUM + 1)
#define btcpu_Gdb_CMDPARAM2_OFF    (btcpu_Gdb_REGNUM + 2)

//  command between jade cpus and debug server 
#define btcpu_Gdb_CMD_DONE         0x00

#define btcpu_Gdb_CMD_FLUSH_CACHE     0x01
#define btcpu_Gdb_CMD_STEP            0x02
#define btcpu_Gdb_CMD_CONTINUE        0x04
#define btcpu_Gdb_SET_BREAKPOINT      0x05
#define btcpu_Gdb_UNSET_BREAKPOINT    0x06



#define btcpu_Gdb_CMD_BREAKPOINT   0x10
#define btcpu_Gdb_CMD_PRINT        0x20




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

    .globl  btcpu_GdbGBreak
btcpu_GdbGBreak:
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreakSoftBreakPoint
    .word   btcpu_GdbBreakRaise
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreakBreakPoint
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreakDivBy0
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreaks
    .word   btcpu_GdbBreakPrint

    /*
//  GDB DATA for bb 
    
.align 2
    .globl    __bb_gdb_context
__bb_gdb_context:
    .space btcpu_Gdb_CONTEXT_SIZE

    */

		.text
		.set nomips16
    .globl    _bt_gdb
    .ent    _bt_gdb
_bt_gdb:
    .frame    sp, 0, ra

    // MISSING BREAK , DBG_IT, EXCEPTION, HOST_IT
    // s0 is context_sp
    // t0, at are invalid
    // t1 is Cause
#if UNIT_TEST==0

//  make room on stack 
    subu sp, sp, btcpu_Gdb_CONTEXT_SIZE

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
    la        t0, btcpu_sp_context
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
    la      t8,    btcpu_GdbGBreak        //  get break table address 
    andi    t0,    t0,    0x3c
    addu    t0, t0,    t8                //  add break number 
    lw        t0,    0(t0)
    jalr     t0                        //  call the implementation function 
    j        $L_gdb_skip_irq
$L_gdb_skip_break:
    beqz    t0, $L_gdb_skip_exception        //  an exception? 
    move    a1, t1                            //  cause 
//    la        t0, btcpu_GdbException
//    jalr    t0
    jal        btcpu_GdbException
    j        $L_gdb_skip_irq
$L_gdb_skip_exception:
#ifdef USE_GDB_PAGE_IRQ
//    here whe have one of the upper exceptions
    andi    t0, t1, XCPU_HIP_PAGE_SPY
    beqz    t0, $L_skip_page
    jal        btcpu_GdbIrqPage
    j        $L_gdb_skip_irq
$L_skip_page:
#endif // USE_GDB_PAGE_IRQ

// Here we have the call to the host command handler in case
// of host exception (ie host interrrupt)
    andi    t0, t1, XCPU_HIP_HOST
    beqz    t0, $L_skip_host
//    jal     hal_HstRamClientIrq //hal_HstCmdBasicHandler
    j        $L_gdb_skip_irq
$L_skip_host:

//    la        t0, btcpu_GdbIrqDebug
//    jalr    t0
    jal        btcpu_GdbIrqDebug
$L_gdb_skip_irq:

//  frame for parameters 
    addu    sp, sp, 16


//  restore gdb context 

//  Tag btcpu_sp_context as "There is no saved GDB context, ie, 
//  we are not in GDB"
    la        t0, btcpu_sp_context
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
    //addu sp, sp, btcpu_Gdb_CONTEXT_SIZE
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

    .type    _bt_gdb,@function
    .size    _bt_gdb,.-_bt_gdb
    .end _bt_gdb


