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
//                                                                            /
/// @file boot_rom.S                                                          //
/// That file provides the boot code                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "regdef.h"
#include "cp0.h"
#include "global_macros.h"
#include "mem_bridge_asm.h"
#include "sys_ctrl_asm.h"
#include "debug_port_asm.h"
#include "debug_uart_asm.h"
#include "bb_sram_asm.h"
#include "debug_host_asm.h"
#include "tester_chip_asm.h"
#include "spiflash_asm.h"
#include "pmuc_asm.h"
#include "chip_id.h"
#include "boot_host.h"
#include "boot_sector_chip.h"
#include "boot_profile_codes.h"
#include "bootp_mode.h"
#include "pmuc_asm.h"    

///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////


/// Extern variable
.globl  boot_HstMonitor 
.globl  g_bootResetCause
.globl  g_bootBootMode
.globl  g_fastEntry
.globl  g_savecontextAddr

.text
.set at
//############################################
//# BOOT at 0x0
//############################################
.globl    __xcpu_start
__xcpu_start:


    // Historical reason: when the boot code was in flash, it was
    // fixing the "four nop" issue. As the flash was not ready on
    // time, we added some nops. This might not be useful with the ROM.
    nop
    nop
    nop
    nop
    b __jump_1
__jump_1:
    // ------------------------------------
    // Save context for Watchdog in Dualport SRAM
    // ------------------------------------
    la     k0, KSEG1(REG_DUALPORT_SRAM_BASE)
    .set noat
    sw  AT, 0x4(k0)
    .set at
    sw  a0, 0x10(k0)
    sw  a1, 0x14(k0)
    sw  t0, 0x20(k0)
    sw  t1, 0x24(k0)

    // We save those registers because they are used for PXTS code sending.
    sw  a3, 0x1c(k0)
    sw  t3, 0x2c(k0)

    
    //------------------------------------------------------
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // ONLY USE AT, a0, a1, t0 and t1 until the full WD context is saved
    // a1 is used to store the boot mode pins value.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //------------------------------------------------------

    
    // ------------------------------------------------------
    // Testmode for the production test 
    // On the production line, the functional test mode is used
    // to tell the chip it is going to undergo the production 
    // test. The Boot mode has then a particular meaning dedicated
    // to the functional test:
    //  - A boot_mode = 0 ==> configure the EBC and jump a the beginning
    // of the flash, where further instruction (ie the functional 
    // test code) is waiting
    //  - A boot_mode != 0 ==> The boot mode is used by the romed 
    // functional test to do things, we jump at that romed function.
    // (This code is provided by the HW ASIC )
    // ------------------------------------------------------
    lw      t0, REG_mcu_warm_reset_cause
    li      a1, SYS_CTRL_FUNCTION_TEST_MODE
    and     t1, t0, a1
    
    bnez    t1, __functional_test_boot
   
    // ------------------------------------
    // Whether enter the sleep recover mode
    // ------------------------------------
    lw      t3, REG_pwr_sys_ctrl
    li      a1, PMUC_PM_REG(3)
    and     t1, t3, a1
    beqz    t1, __normal_boot 

__fastentry:

    li  a0, BOOT_FASTENTRY_VALID_TAG
    lw  t1, g_fastEntry
    lw  sp, g_savecontextAddr
    beq  a0,t1,_boot_irq_restore_context
   
    

    // ------------------------------------------------------
    // Normal boot, we are not in functional test.
    // ------------------------------------------------------
    
__normal_boot:
    
    // ------------------------------------
    // Get the status of the BOOT MODE pins.
    // This will also save the status of the software boot modes.
    // ------------------------------------
    //lw  t0, REG_mcu_warm_reset_cause
    // t0's value not defined now (nop automatically added)
    srl a1, t0, SYS_CTRL_BOOT_MODE_SHIFT
    sw  a1, g_bootBootMode

    // ------------------------------------
    // Save reset cause register
    // ------------------------------------
    //lw  t0, REG_mcu_warm_reset_cause
    li  t1, SYS_CTRL_WDT_RST \
          | SYS_CTRL_GLOBAL_SOFT_RST \
          | SYS_CTRL_DBG_HST_RST \
          | SYS_CTRL_ALARM_CAUSE
    and t0, t0, t1
    sw  t0, g_bootResetCause

    // spiflash_en 
    li  t0, SYS_CTRL_SPIFLASH_EN
    sw  t0, REG_per_module_en
    
    // set spiflash clock to 26M  
    li t0, (SYS_CTRL_SPIFLASH_CLK_SEL_FAST|SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M)
    sw t0, REG_spiflash_clk_cfg

    //spiflash clock divider 1; din_delay 1; single mode
    li t0, SPIFLASH_CLK_DIVIDER(1)|SPIFLASH_SAMPLE_DELAY(1);
    sw t0, REG_flash_config

    // this jump allows to go from uncached to cached
    la  t0, _xcpu_end_boot
    jr     t0


// ################################################################################
// Int handler at its fixed place, of needed
// ################################################################################


.align 7
. = (__xcpu_start | 0x180)
// #####################################
// # rom interrupt handler is at 0x180
// #####################################

    .globl  _boot_IrqHandler
_boot_IrqHandler:


// ------------------------------------------------
// save context on current stack
// ------------------------------------------------

//  make room on stack for extra context
    subu sp, sp, 120

    .set noat
    sw  AT, 0(sp)
    .set at
    sw  t0, 4(sp)

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
    sw  s6,100(sp)           # save $22
    sw  s7,104(sp)           # save $23
    sw  fp,108(sp)           # save $30
    sw  a0,112(sp)           # save $4

    mfc0    t0,EPC
    sw  t0,116(sp)           # save Pc

// ------------------------------------------------
//  End save context
// ------------------------------------------------

// ------------------------------------------------
// Get IRQ cause and select function to call
// ------------------------------------------------
    mfc0 t1,Cause

    //  While loop if is not a interruption
    // For example, we jump or directly execute _boot_IrqHandler
_boot_not_interrupt:
    beqz t1, _boot_not_interrupt

    
    // read cause register.
    // If this is not a regular interrupt, we are handling
    // an exception. In the rom code, we while(1).
    andi    t1, t1, (0x8000 | Cause_ExcCode)    // extract exception code + debug irq
_boot_romed_exception:
    bnez    t1, _boot_romed_exception 

//---------------------------------
// regular IT function
//---------------------------------
    // create stack frame for function call + gp restoration
    subu    sp, sp, 24

    jal boot_IrqDispatch

    addu    sp, 24

    
_boot_irq_restore_context:

// restore context associated to scheduled task
//----------------------------
// restore context
//----------------------------

    // v0 is setup to the sp addr of the scheduled task by TaskSwitch
    // or is the value saved before

    lw  k0,116(sp)           # restore PC
    lw  a0,112(sp)           # restore task param
    lw  fp,108(sp)           # restore $30
    lw  s7,104(sp)           # restore $23
    lw  s6,100(sp)           # restore $22
    lw  s5,96(sp)           # restore $21
    lw  s4,92(sp)           # restore $20
    lw  s3,88(sp)           # restore $19
    lw  s2,84(sp)           # restore $18
    lw  s1,80(sp)           # restore $17
    lw  s0,76(sp)           # restore $16

//  restore HI and LO registers 
    lw  t0, 68(sp)
    lw  t1, 72(sp)
    mtlo t0
    mthi t1

//  restore all registers 
    lw  ra,64(sp)
    lw  t9,60(sp)
    lw  t8,56(sp)
    lw  t7,52(sp)
    lw  t6,48(sp)
    lw  t5,44(sp)
    lw  t4,40(sp)
    lw  t3,36(sp)
    lw  t2,32(sp)
    lw  t1,28(sp)
    lw  a3,24(sp)
    lw  a2,20(sp)
    lw  a1,16(sp)
    lw  v1,12(sp)
    lw  v0, 8(sp)
    .set noat
    lw  AT,0(sp)
    .set at
    lw  t0,4(sp)

    // Free extra space on stack
    addu    sp, sp, 120

    .set noreorder
    jr  k0
    rfe
    .set reorder






// ################################################################################
// Other functions. 
// ################################################################################
// ################################################################################
// _hstMonitorXCtx_start is the base address of the executing structure
// defined by the linker script

.align 4
.globl boot_HstMonitorX
.ent boot_HstMonitorX
.frame sp, 0, ra
boot_HstMonitorX:
    .set noreorder
    // Save sp,ra, s0 on the stack
    addiu sp, -24
    sw  ra, 16(sp)
    sw  s0, 20(sp)

    // sp saved in s0 in a delay slot later

    // read in monitor host execution context the 
    // value for pc and sp, and parameter.
    lw  t2, _hstMonitorXCtx_start+BOOT_HST_MONITOR_X_CTX_SP;
    
    // Pc loaded in t1 here because of undetermination
    // of the target of a lw a the time immediately 
    // following
    lw    t1, _hstMonitorXCtx_start+ BOOT_HST_MONITOR_X_CTX_PC;

    // if BOOT_HST_MONITOR_X_CTX_SP is 0, keep current sp
    beqz t2, $L_KEEP_SP
    move s0, sp             // sp saved in delay slot

    move    sp,    t2
$L_KEEP_SP:

    lw  a0, _hstMonitorXCtx_start + BOOT_HST_MONITOR_X_CTX_PARAM; 

    jalr t1
    nop
    // parameter loaded in delay slot

    sw  v0, _hstMonitorXCtx_start + BOOT_HST_MONITOR_X_CTX_RETURNED_VALUE;

    // Recover previous state
    move sp, s0
    lw   ra, 16(sp)
    lw   s0, 20(sp)

    jr ra
    addiu sp, 24
    
    .set reorder
.end   boot_HstMonitorX



//.align 8 
// ============================================
// Function to trig EXL by jumping at a known 
// address (here)
// ============================================
.globl boot_DbgExlTrigFunction0
.ent   boot_DbgExlTrigFunction0
.frame sp,0,ra
boot_DbgExlTrigFunction0:
    .align 4
    jr ra
.end boot_DbgExlTrigFunction0




// ################################################################################
// ################################################################################
// ################################################################################
// ################################################################################








//------------------------------------------
// End of boot
//------------------------------------------
_xcpu_end_boot:

     // ------------------------------------
    // Save reset cause register
    // ------------------------------------
    lw  t0, REG_mcu_warm_reset_cause
    li  t1, SYS_CTRL_WDT_RST   
    li  t3, SYS_CTRL_RST_CAUSE_CLR
    sw  t3, REG_mcu_warm_reset_cause
    and t0, t0, t1
    beqz t0, _xcpu_init_stack
    // -------------------------------------
    // Finish saving WD context
    // -------------------------------------
    sw  zero, 0x0(k0)
    // at already saved in 0x04
    sw  v0, 0x8(k0)
    sw  v1, 0xc(k0)
    // a0 already saved in 0x10
    // a1 already saved in 0x14
    sw  a2, 0x18(k0)
//    sw  a3, 0x1c(k0) // FIXME
    // t0 already saved in 0x20
    // t1 already saved in 0x24
    sw  t2, 0x28(k0)
//    sw  t3, 0x2c(k0)
    sw  t4, 0x30(k0)
    sw  t5, 0x34(k0)
    sw  t6, 0x38(k0)
    sw  t7, 0x3c(k0)
    sw  s0, 0x40(k0)
    sw  s1, 0x44(k0)
    sw  s2, 0x48(k0)
    sw  s3, 0x4c(k0)
    sw  s4, 0x50(k0)
    sw  s5, 0x54(k0)
    sw  s6, 0x58(k0)
    sw  s7, 0x5c(k0)
    sw  t8, 0x60(k0)
    sw  t9, 0x64(k0)
    sw  zero, 0x68(k0) // k0 is not saved - put 0
    sw  k1, 0x6c(k0)
    sw  gp, 0x70(k0)
    sw  sp, 0x74(k0)
    sw  fp, 0x78(k0)
    sw  ra, 0x7c(k0)
    sw  zero, 0x80(k0) // status is not saved     - put 0
    sw  zero, 0x84(k0) // lo is not saved         - put 0
    sw  zero, 0x88(k0) // hi is not saved         - put 0
    sw  zero, 0x8c(k0) // bad is not saved         - put 0
    sw  zero, 0x90(k0) // cause is not saved     - put 0
    // save PC
    //li    a0, REG_DEBUG_PORT_BASE_HI
    //lw    t0, REG_DEBUG_PORT_WD_ADDR_CFG(a0)
    //sw    t0, 0x94(k0)
    //---------------------------------------
    // WD context saved
    // no more limitation on register usage
    //---------------------------------------
    
_xcpu_init_stack:

    // ------------------------------------
    //  initialize stack pointer 
    // ------------------------------------
    lw    sp, initial_stack_top_var
    sw  sp, sxr_IrqStack_pointer
    
     // ######################################
    // Raise wakeup line
    // ######################################
    li  t0, SYS_CTRL_FORCE_WAKEUP
    sw  t0, REG_wakeup_ctrl
    
    // enable it in risc
    li  t0, (Status_IntMASK | Status_IEc | 0x400000)
    mtc0    t0, Status

    // #######################################
    // Jump to monitor function - IF RETURNS FROM MONITOR ERROR
    // #######################################

    lw    t0, monitor_entry
    jr t0    

idle_loop:
    nop
    j   idle_loop
    nop


    // ------------------------------
    // Functional Test Boot
    // ------------------------------

__functional_test_boot:


    // NOTE: BIST module has been removed from chip since gallite
boot_FunctionalTest:
    nop
    j       boot_FunctionalTest



//.align 11
// #####################################
// # global variables accessible from outside  are at 0x800
// #####################################
.globl monitor_entry
.globl initial_stack_top_var


// ################################################################################
// Some of these functions might move if the previous code expands
// ################################################################################


// ============================================
// Some variables (in fact constants, since in ROM ...)
// ============================================
monitor_entry: .word    boot_Monitor
initial_stack_top_var: .word _sys_sram_non_romed_top



// ============================================
// Function to flush caches
// ============================================
.globl boot_FlushCache
.ent   boot_FlushCache
.frame sp,0,ra
boot_FlushCache:
    .set noreorder
    .align 4
    cache 0,0
    nop
    nop
    nop
    jr ra
    nop
    .set reorder
.end boot_FlushCache



// ============================================
// Disable interruption
// ============================================
.globl boot_DisableInterrupt
.ent   boot_DisableInterrupt
.frame sp,0,ra
boot_DisableInterrupt:
        li  t0, (Status_IntMASK | Status_IEp)
        mtc0  t0, Status
.end boot_DisableInterrupt



