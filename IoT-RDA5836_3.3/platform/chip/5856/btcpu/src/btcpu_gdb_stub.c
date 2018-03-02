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
/// @file hal_gdb_stub.c
/// GDB stub implementation
///
/// This file needs #CSW_ErrorEntry. If this function is
/// not defined, there will be linking problem when some
/// functions are used.
///
/// In this file are also defined a *lot* of defines ...
/// one for each gdb opcode.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"
#include "global_macros.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "debug_host.h"
#include "page_spy.h"
#include "tester_chip.h"
#include "mem_bridge.h"
#include "timer.h"

#include "cp0.h"
#undef Cause
// to fix conflict between cp0 Cause reg and Irq Cause reg !!

//#include "boot.h"
#include "btcpu_gdb_stub.h"
//#include "halp_page_spy.h"
//#include "halp_timers.h"
//#include "halp_debug.h"
//#include "halp_irq_handler.h"
#include "hal_debugpaltrc.h"
//#include "hal_ebc.h"
#include "hal_map.h"
#include "hal_timers.h"
#include "hal_host.h"
#include "hal_sys.h"
#ifndef PAL_NO_DBGTRC
#include "hal_debugpaltrc.h"
#endif

// if gdb triggers an exception in some code called by sx (backtrace)
//#define HAL_GDB_SAFE_WITHOUT_SX
#ifndef HAL_GDB_SAFE_WITHOUT_SX
#include "sxs_io.h"
#include "sxs_srl.h"
#include "sxs_rmt.h"
#include "sxr_jbsd.h"
#include "sxr_tksd.h"
#endif // HAL_GDB_SAFE_WITHOUT_SX

//#include "memd_m.h"


// =============================================================================
//  MACROS
// =============================================================================

#define HAL_GDB_XCPU_EVENT  0x9dbb0000

//  Exception code names 
#ifndef  HAL_NO_PRINTF
#define HAL_EXCEPTION_QTY    32
static CONST CHAR *excCodeName[HAL_EXCEPTION_QTY] = {
  "Interrupt [Int]",
  "TLB modif. exc. [Mod]",
  "TLB exc. (load or instr.) [TLBL]",
  "TLB exc. (store) [TLBS]",
  "Addr. error exc. (load or instr.) [AdEL]",
  "Addr. error exc. (store) [AdES]",
  "Bus error exc. (instr.) [IBE]",
  "Bus error exc. (load/store) [DBE]",
  "Syscall exc. [Sys]",
  "Breakpoint exc. [Bp]",
  "Reserved Instr. exc. [RI]",
  "Coproc. unusable exc. [CpU]",
  "Arithmetic exc. [Ov]",
  "** Trap exc. [Tr] **",           //  Not on R3000 
  "** Virtual Coh. exc. [VCEI] **", //  Not on R3000 
  "** Floating point exc. [FPE] **",    //  Not on R3000 
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "-- Reserved --",
  "** Virtual coher. exc. [VCED] **"        //  Not on R3000 
};
#endif // #ifdef  HAL_NO_PRINTF

#define HAL_PROFILE_FUNCTION_ENTER(eventName)
#define HAL_PROFILE_FUNCTION_EXIT(eventName)
#define HAL_PROFILE_WINDOW_ENTER(eventName)
#define HAL_PROFILE_WINDOW_EXIT(eventName)
#define HAL_PROFILE_PULSE(pulseName)
#define HAL_PROFILE_SPI_ACTIVATION(id, csNum)
#define HAL_PROFILE_SPI_DEACTIVATION(id, csNum)

#define HAL_TRACE(level, tsmap, fmt, ...)


// ****************************************************************************
//                             LOCAL CONSTANTS                                 
// ****************************************************************************

#define HAL_GDB_OPCODE_STEP      0x0005000d
#define HAL_GDB_16_OPCODE_STEP   0xe8a5
#define HAL_GDB_SIGTRAP          5
#define HAL_GDB_SIGINT           2
#define HAL_GDB_SIGSEGV          11
#define HAL_GDB_SIGILL           4
#define HAL_GDB_SIGBUS           10
#define HAL_GDB_SIGKILL          9
#define HAL_GDB_SIGFPE           8

#define HAL_GDB_PAGE_DISABLE     do \
                                    { if (hwp_pageSpy->enable != 0) \
                                        page_spy_enable = hwp_pageSpy->enable; \
                                    hwp_pageSpy->disable = page_spy_enable;} \
                                while (0)
#define HAL_GDB_PAGE_ENABLE      (hwp_pageSpy->enable = page_spy_enable)



#define GDB_CSW_FUNC(c,e,s,r) do \
                                { \
                                    if(g_halErrorRegistry) \
                                    { \
                                        g_halErrorRegistry(c,e,s,(HAL_DBG_ERROR_CTX_T*)r); \
                                    } \
                                }\
                                while (0);


//  Manage 16/32 bits addressing modes 
#define INC_STEP ((btcpu_GdbRegs[37] & 0x01) ? 2 : 4)
#define MASK_ONE(addr) ((addr)&(~1))

#define HAL_GDB_GET_OFFSET(op)   (((((op) & 0x00008000) == 0x00008000) ? \
        ((op) | 0xffff0000) : ((op) & 0x0000ffff)) << 2)

#define HAL_GDB_OPCODE_J(op)     (((op) & 0xfc000000) == 0x08000000)
#define HAL_GDB_OPADDR_J(op)     ((((op) & 0x03ffffff) << 2) | (btcpu_GdbRegs[37] & 0xf0000000))
#define HAL_GDB_OPCODE_JAL(op)   (((op) & 0xfc000000) == 0x0c000000)
#define HAL_GDB_OPADDR_JAL(op)   ((((op) & 0x03ffffff) << 2) | (btcpu_GdbRegs[37] & 0xf0000000))
#define HAL_GDB_OPCODE_JALX(op)  ((((op) & 0xfc000000) == 0x74000000))
#define HAL_GDB_OPADDR_JALX(op)  ((((op) & 0x03ffffff) << 2) | (btcpu_GdbRegs[37] & 0xf0000000) | 1)
#define HAL_GDB_OPCODE_JR(op)    (((op) & 0xfc1ff83f) == 0x00000008)
#define HAL_GDB_OPADDR_JR(op)    (btcpu_GdbRegs[((op) & 0x03e00000) >> 21])
#define HAL_GDB_OPCODE_JALR(op)  (((op) & 0xfc1f003f) == 0x00000009)
#define HAL_GDB_OPADDR_JALR(op)  (btcpu_GdbRegs[((op) & 0x03e00000) >> 21])
#define HAL_GDB_OPCODE_BEQ(op)   (((op) & 0xfc000000) == 0x10000000)
#define HAL_GDB_OPADDR_BEQ(op)   ((btcpu_GdbRegs[((op) & 0x03e00000) >> 21] == btcpu_GdbRegs[((op)   \
        & 0x001f0000) >> 16]) ? (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BNE(op)   (((op) & 0xfc000000) == 0x14000000)
#define HAL_GDB_OPADDR_BNE(op)   ((btcpu_GdbRegs[((op) & 0x03e00000) >> 21] != btcpu_GdbRegs[((op)   \
        & 0x001f0000) >> 16]) ? (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGEZ(op)  (((op) & 0xfc1f0000) == 0x04010000)
#define HAL_GDB_OPADDR_BGEZ(op)  (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] >= 0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGEZAL(op)    (((op) & 0xfc1f0000) == 0x04110000)
#define HAL_GDB_OPADDR_BGEZAL(op)    (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] >= 0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGTZ(op)  (((op) & 0xfc1f0000) == 0x1c000000)
#define HAL_GDB_OPADDR_BGTZ(op)  (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] >  0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLEZ(op)  (((op) & 0xfc1f0000) == 0x18000000)
#define HAL_GDB_OPADDR_BLEZ(op)  (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] <= 0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLTZ(op)  (((op) & 0xfc1f0000) == 0x04000000)
#define HAL_GDB_OPADDR_BLTZ(op)  (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] <  0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLTZAL(op)    (((op) & 0xfc1f0000) == 0x04100000)
#define HAL_GDB_OPADDR_BLTZAL(op)    (((INT32)btcpu_GdbRegs[((op) & 0x03e00000) >> 21] <  0) ?   \
        (btcpu_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (btcpu_GdbRegs[37]+4))


//  We pass op as a pointer for mips16 macros, because we may need to access the next 16bits chunk 

//  Perform 32bits wide sign extension and left shift for various offsets 
#define HAL_GDB_16_GET_11_OFFSET(op)     ( ( (*(op) & 0x0400) ? \
                ((*(op)) | 0xfffff800) : ((*(op)) & 0x7ff) ) << 1 )
#define HAL_GDB_16_GET_8_OFFSET(op)      ( ( (*(op) & 0x0080) ? \
                ((*(op)) | 0xffffff00) : ((*(op)) & 0x00ff) ) << 1 )

/// Cf mips16 doc: register encoding in mips16 is a fucking mess.
/// Please refer to the spec or trust the fact that:
/// 0 --> r16
/// 1 --> r17
/// 2 --> r2
/// ..
/// 7 --> r7
#define HAL_GDB_16_MAP_REGS(n)  (((n)<2)?((n)+16):(n))
#define HAL_GDB_16_REGS(n)      btcpu_GdbRegs[HAL_GDB_16_MAP_REGS(n)]
    
//  Helper for messy offsets on extended branches 
#define HAL_GDB_16_ASSEMBLE_EXT_OFFSET(op) \
                 ( (*((op)+1) & 0x001f) + ((*(op)&0x07e0))  + ((*(op) & 0x001f) << 11) ) 
//  GNU C only, but much cleaner... 
#define HAL_GDB_16_GET_EXT_OFFSET(op) \
        ({ UINT32 _off = HAL_GDB_16_ASSEMBLE_EXT_OFFSET((op)); (_off & 0x8000) ? ((_off | 0xffff0000)<<1) : ((_off & 0x0000ffff)<<1); })

#define HAL_GDB_16_OPCODE_JAL(op)    ((*(op) & 0xfc00) == 0x1800)
#define HAL_GDB_16_OPADDR_JAL(op)    ((0x80000001) | ( ((btcpu_GdbRegs[37] + 2)&0xe0000000) + ( ( *((op)+1) + ((*(op) & 0x001f) << 21) \
        + ((*(op) & 0x03e0) << 11) ) << 2 )) )
#define HAL_GDB_16_OPCODE_JALX(op)   ((*(op) & 0xfc00) == 0x1c00)
#define HAL_GDB_16_OPADDR_JALX(op)   ((0x80000000) | (( (btcpu_GdbRegs[37] + 2)&0xe0000000) + ( ( *((op)+1) + ((*(op) & 0x001f) << 21) \
        + ((*(op) & 0x03e0) << 11 ) ) << 2 )) )
#define HAL_GDB_16_OPCODE_JALR(op)   ((*(op) & 0xf85f) == 0xe840)
#define HAL_GDB_16_OPADDR_JALR(op)   (HAL_GDB_16_REGS((*(op) & 0x0700) >> 8))
#define HAL_GDB_16_OPCODE_JR(op)     ((*(op) & 0xf85f) == 0xe800)
#define HAL_GDB_16_OPADDR_JR(op)     ( (*(op) & 0x0020) ? \
        btcpu_GdbRegs[31] : HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) )
#define HAL_GDB_16_OPCODE_B(op)      ((*(op) & 0xf800) == 0x1000)
#define HAL_GDB_16_OPADDR_B(op)      (btcpu_GdbRegs[37]+2+HAL_GDB_16_GET_11_OFFSET(op))
#define HAL_GDB_16_OPCODE_BEQZ(op)   ((*(op) & 0xf800) == 0x2000)
#define HAL_GDB_16_OPADDR_BEQZ(op)   ((HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) == 0 ) ? \
        btcpu_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : btcpu_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BNEZ(op)   ((*(op) & 0xf800) == 0x2800)
#define HAL_GDB_16_OPADDR_BNEZ(op)   ((HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) != 0 ) ? \
        btcpu_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : btcpu_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BTEQZ(op)  ((*(op) & 0xff00) == 0x6000)
#define HAL_GDB_16_OPADDR_BTEQZ(op)  ((btcpu_GdbRegs[24] == 0 ) ? \
        btcpu_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : btcpu_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BTNEZ(op)  ((*(op) & 0xff00) == 0x6100)
#define HAL_GDB_16_OPADDR_BTNEZ(op)  ((btcpu_GdbRegs[24] != 0 ) ? \
        btcpu_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : btcpu_GdbRegs[37]+2)

//  Separate test for extended insns 
#define HAL_GDB_16_OPCODE_EXTENDED(op)   ((*(op) & 0xf800) == 0xf000)

                                                            

//  Only valid if already tested as extended 
#define HAL_GDB_16_OPCODE_B_EXT(op)      ((*(&*(op)+1) & 0xf800) == 0x1000)
#define HAL_GDB_16_OPADDR_B_EXT(op)      ( btcpu_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) )
#define HAL_GDB_16_OPCODE_BEQZ_EXT(op)       ((*((op)+1) & 0xf800) == 0x2000)
#define HAL_GDB_16_OPADDR_BEQZ_EXT(op)       ((HAL_GDB_16_REGS((*((op)+1) & 0x0700) >> 8) == 0) ? \
        ( btcpu_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : btcpu_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BNEZ_EXT(op)       ((*((op)+1) & 0xf800) == 0x2800)
#define HAL_GDB_16_OPADDR_BNEZ_EXT(op)       ((HAL_GDB_16_REGS((*((op)+1) & 0x0700) >> 8) != 0) ? \
        ( btcpu_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : btcpu_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BTEQZ_EXT(op)      ((*((op)+1) & 0xff00) == 0x6000)
#define HAL_GDB_16_OPADDR_BTEQZ_EXT(op)      ((btcpu_GdbRegs[24] == 0) ? \
        ( btcpu_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : btcpu_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BTNEZ_EXT(op)      ((*((op)+1) & 0xff00) == 0x6100)
#define HAL_GDB_16_OPADDR_BTNEZ_EXT(op)      ((btcpu_GdbRegs[24] != 0) ? \
        ( btcpu_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : btcpu_GdbRegs[37] + 4 )



                                                                  

/// Flag to store a mips16 opcode in btcpu_GdbGData[cpuid].opCode
/// and make it different of 0. HAL_GDB_MIPS16_OPCODE_MASK is used
/// to restore the proper opcode, discarding the flag.
#define HAL_GDB_MIPS16_OPCODE_FLAG      0xdb900000
#define HAL_GDB_MIPS16_OPCODE_MASK      0x0000FFFF

/// Patch number used for the one step allowed in ROM
#define HAL_GDB_PATCH_NUMBER            (CHIP_NUMBER_OF_PATCHES-1)


// ****************************************************************************
//                          LOCAL FUNCTION PROTOTYPES                          
// ****************************************************************************
PRIVATE VOID btcpu_GdbHandle(UINT32 cpuid, VOLATILE UINT32 * regs);
PRIVATE VOID btcpu_GdbStep (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * btcpu_GdbRegs);
PRIVATE VOID btcpu_GdbUndoStep (UINT32 cpuid, VOLATILE UINT32 * btcpu_GdbRegs);
PRIVATE VOID btcpu_GdbSetBreakpoint(UINT32 cpuid,VOLATILE UINT32* btcpu_GdbRegs);
PRIVATE VOID btcpu_GdbUnsetBreakpoint(UINT32 cpuid,VOLATILE UINT32* btcpu_GdbRegs);
extern VOID btcpu_GdbFlushCache();
PRIVATE UINT32 btcpu_GdbStrLen(UINT8* pcStr);
PRIVATE VOID btcpu_GdbBackTrace(VOLATILE UINT32 *);
#ifndef HAL_GDB_SAFE_WITHOUT_SX
PRIVATE VOID btcpu_GdbDebugHook(VOID);
PRIVATE VOID btcpu_GdbCheckSwitchContextCmd(VOID);
#endif

extern void SYS_SendEvent2(UINT32 event);

// =============================================================================
//  TYPES
// =============================================================================
/// GDB data type
typedef struct{
    UINT32 stepAddr;
    UINT32 opCode;
} HAL_GDB_DATA_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#ifdef HAL_GDB_CONFIG
EXPORT UINT32 g_halGdbEnabled;
#endif // HAL_GDB_CONFIG

//  Enable/disable page_spy to avoid SIGKILL on breakpt insertion
PRIVATE UINT32 page_spy_enable;
//PRIVATE UINT32 hal_PageNoraise = 0;
PRIVATE HAL_GDB_DATA_T   btcpu_GdbGData[2]= {{0,0},{0,0}};
PRIVATE HAL_DBG_ERROR_HANDLER_T g_halErrorRegistry  
            = NULL;

//VOLATILE HAL_GDB_SWITCH_CTX_T HAL_UNCACHED_BSS g_halGdbSwitchCtx;

#ifndef HAL_GDB_SAFE_WITHOUT_SX
PRIVATE VOLATILE UINT32 g_halGdbTempRegs[HAL_GDB_REGNUM_XTD];
PRIVATE VOLATILE UINT32 *g_halGdbRegsBackup = NULL;

#define HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM 4
PRIVATE VOID (*g_halGdbDebugHookFuncPtr[HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM])(VOID) =
    { &btcpu_GdbCheckSwitchContextCmd, &hal_SysProcessHostMonitor, };
#endif

PRIVATE BOOL g_halGdbIsInGdbLoop = FALSE;

#if (CHIP_HAS_PROXY_MEMORY == 1)
PRIVATE UINT32 g_halGdbProxyEnable = 0;
    // If the chip has a proxy memory, and the external access
    // is disabled, enable it.
#define HAL_GDB_ENABLE_PM_ACCESS \
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK; \
    g_halGdbProxyEnable = (hwp_sysCtrl->Cfg_AHB & SYS_CTRL_XCPU_PMEM_ACCESS_ENABLE); \
    hwp_sysCtrl->Cfg_AHB |= SYS_CTRL_XCPU_PMEM_ACCESS_ENABLE; \
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;

    // If the chip has a proxy memory, and the external access was 
    // was disabled, disable it before leaving GDB.
#define HAL_GDB_DISABLE_PM_ACCESS \
    if (g_halGdbProxyEnable == 0) \
    { \
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK; \
        hwp_sysCtrl->Cfg_AHB &= ~SYS_CTRL_XCPU_PMEM_ACCESS_ENABLE; \
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK; \
    } \
    g_halGdbProxyEnable = 0;

#else 
#define HAL_GDB_ENABLE_PM_ACCESS
#define HAL_GDB_DISABLE_PM_ACCESS
#endif

//#define HAL_DBG_STEPFORBIDDEN_FUNCTION_COUNT 2
//PRIVATE UINT32* HAL_DBG_STEPFORBIDDEN_FUNCTIONS [HAL_DBG_STEPFORBIDDEN_FUNCTION_COUNT] = 
//    {(UINT32*)btcpu_DbgGdbHstSendEvent, (UINT32*)hal_HstSendEvent};

// =============================================================================
// btcpu_sp_context
// -----------------------------------------------------------------------------
/// Variable holding the pointer where to save the btcpu context
// =============================================================================
EXPORT UINT32* btcpu_sp_context;

// =============================================================================
// btcpu_error_code
// -----------------------------------------------------------------------------
/// Global variable holding the btcpu error code.
// =============================================================================
EXPORT volatile UINT32 btcpu_error_code;

// =============================================================================
// btcpu_error_status
// -----------------------------------------------------------------------------
/// Global variable holding the btcpu error status.
// =============================================================================
EXPORT volatile UINT32 btcpu_error_status;


// *****************************************************************************
//                         FUNCTION IMPLEMENTATION                              
// *****************************************************************************
// That is very nice to have a dedicated leaf function to send events, since
// a breakpoint (for conventional debugging) in a function called by that 
// #btcpu_DbgGdbHstSendEvent function, would call GDB into GDB and lead to
// a system collapse.
PROTECTED VOID btcpu_DbgGdbHstSendEvent(UINT32 ch)
{
    // Enter critical section
    UINT32 status = hwp_sysIrq->sc;

    // Wait for semaphore to be free. We don't care
    // being blocking here: there is a host plugged
    // to the target when we are using gdb
    while (hwp_debugHost->event == 0);
    hwp_debugHost->event = ch;

    // Exit critical section
    hwp_sysIrq->sc = status;
}


PROTECTED VOID btcpu_DbgWatchDogStop(VOID)
{
    hwp_timer->wdtimer_ctrl = TIMER_STOP;    
}


PUBLIC VOID SYS_ASSERT(VOID)
{
	SYS_SendEvent2(0xb7a55e47);
	asm volatile("break 2");
}

#if 0
// =============================================================================
// btcpu_GdbIsAddressStepForbidden
// -----------------------------------------------------------------------------
// Helper function to tell if a function is step-into-able or not. If it's not,
// we will cheat and set the breakpoint after the func call.
// =============================================================================

PRIVATE BOOL btcpu_GdbIsAddressStepForbidden(UINT32 newAddr)
{
    int i = 0;
 
    for(i = 0; i<HAL_DBG_STEPFORBIDDEN_FUNCTION_COUNT; i++)
    {    
        if((UINT32)HAL_DBG_STEPFORBIDDEN_FUNCTIONS[i] == newAddr)
        {
            return TRUE;
        }  
    }
    
    return FALSE;
}
#endif

// =============================================================================
// btcpu_GdbBreaks
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbBreaks(UINT32 cpuid)
{
    VOLATILE UINT32 * btcpu_GdbRegs;

    HAL_GDB_ENABLE_PM_ACCESS;

    btcpu_GdbRegs = btcpu_sp_context /* xcpu_sp_context */;

    btcpu_GdbRegs[37]+=INC_STEP;
    return; 
}

// =============================================================================
// btcpu_GdbBreakPrint
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbBreakPrint(UINT32 cpuid)
{
    UINT32     nLen;
    UINT8 * str;
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    
    HAL_GDB_ENABLE_PM_ACCESS;

    btcpu_GdbRegs = btcpu_sp_context /* xcpu_sp_context */;
//  stop the watchdog so we can use gdb 
    btcpu_DbgWatchDogStop();
    
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);
        
    str = (UINT8*)(btcpu_GdbRegs[4]); // a0 before break


    nLen  = btcpu_GdbStrLen(str);
//  prepare command 
    v_regs[HAL_GDB_CMD_OFF]=HAL_GDB_CMD_PRINT;
    v_regs[HAL_GDB_CMDPARAM_OFF]=nLen;

//  trigger command 
    btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
    
//  wait for the debug server to complete 
    while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    
    btcpu_GdbRegs[37]+=INC_STEP;
//  restart the watchdog 
    //hal_TimWatchDogKeepAlive();
    return; 
}

// =============================================================================
// btcpu_GdbPrepare
// -----------------------------------------------------------------------------
/// @return GDB context (cached)
// =============================================================================
PROTECTED VOLATILE UINT32 *btcpu_GdbPrepare(UINT32 cpuid, UINT32 error_code, UINT32 error_status, BOOL fatal)
{
    VOLATILE UINT32 * btcpu_GdbRegs;

    // If the chip has a proxy memory, and the external access
    // is disabled, enable it.
    HAL_GDB_ENABLE_PM_ACCESS;

    if (fatal)
    {
        //HAL_PROFILE_PULSE(XCPU_ERROR);
        //btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
    }

    btcpu_GdbRegs          = btcpu_sp_context /* xcpu_sp_context */;
    hwp_sysCtrl->REG_DBG = error_code;
    btcpu_error_code      = error_code;
    btcpu_error_status    = error_status;
    
	// Make sure BCPU can see the new value
    *MEM_ACCESS_UNCACHED(&btcpu_error_code) = error_code;
    
    //  stop the watchdog so we can use gdb 
    btcpu_DbgWatchDogStop();

    // Prevent frame reentrance by masking the fint.
    // This doesn't allow to "continue" the code's execution, the real time
    // aspect is broken for good.
    hwp_sysIrq->pulse_mask_clr = SYS_IRQ_SYS_IRQ_FRAME;

    return btcpu_GdbRegs;
}

// =============================================================================
// btcpu_GdbSendcmd
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbSendcmd(UINT32 cpuid, VOLATILE UINT32 * btcpu_GdbRegs, UINT32 signal, BOOL fatal)
{
#ifdef HAL_GDB_CONFIG
    if (g_halGdbEnabled == 0)
    {
        btcpu_DbgGdbHstSendEvent(0x10+HAL_GDB_XCPU_EVENT);
        return;
    }
#endif // HAL_GDB_CONFIG
//  prepare command 
    btcpu_GdbRegs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_BREAKPOINT;
    btcpu_GdbRegs[HAL_GDB_CMDPARAM_OFF] = signal;
//  trigger command 
    btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
}

// =============================================================================
// btcpu_GdbIsInGdbLoop
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED BOOL btcpu_GdbIsInGdbLoop(VOID)
{
    return g_halGdbIsInGdbLoop;
}

// =============================================================================
// btcpu_GdbLoop
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbLoop(UINT32 cpuid, VOLATILE UINT32 * v_regs, BOOL fatal)
{
    HAL_PROFILE_PULSE(btcpu_Gdb);

    g_halGdbIsInGdbLoop = TRUE;

    if((cpuid == 0) && fatal)
    {
#ifdef HAL_GDB_CONFIG
        if (g_halGdbEnabled == 0)
        {
            hal_SysRestart();
            // in this case we will not go any longer
        }
#endif // HAL_GDB_CONFIG
        // try to flush the trace only on xcpu and when fatal
        //  wait for the debug server to complete 
        while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE)
        {
#ifndef HAL_GDB_SAFE_WITHOUT_SX
            btcpu_GdbDebugHook();
            sxs_SerialFlush ();
#endif // HAL_GDB_SAFE_WITHOUT_SX
        }
    }
    else
    {
        // wait for the debug server to complete
        while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    }
    btcpu_GdbUndoStep(cpuid, v_regs);
    btcpu_GdbHandle(cpuid, v_regs);
    if(fatal)
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
    }
    // restart the watchdog 
    //hal_TimWatchDogKeepAlive();

    g_halGdbIsInGdbLoop = FALSE;

    return;
}

// =============================================================================
// btcpu_GdbBreakDivBy0
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbBreakDivBy0(UINT32 cpuid)
{
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;

    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_DIV0, 0, TRUE);

    btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT+0xd0);
    btcpu_GdbRegs[37]+=INC_STEP;

    // trigger GDB on /0 for both CPUs
    
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);
    
//  send command 
    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGFPE, TRUE);
    
    HAL_PROFILE_PULSE(XCPU_ERROR);
    //btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
    //btcpu_DbgPalFlushTrace ();
#endif
    HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, 
            "BTCPU Division by 0 exception\n"
            "PC %lxy\n"
            "SP %lxy\n",
            btcpu_GdbRegs[37],
            btcpu_GdbRegs[29]);
    btcpu_GdbBackTrace(btcpu_GdbRegs);
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_DIV0, 0, btcpu_GdbRegs);
    btcpu_GdbLoop(cpuid, v_regs, TRUE);
}

// =============================================================================
// btcpu_GdbBreakBreakPoint
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbBreakBreakPoint(UINT32 cpuid)
{
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    
    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_GDB, 0, FALSE);
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);

//  send command 
    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGTRAP, FALSE);
    
    btcpu_GdbLoop(cpuid, v_regs, FALSE);
}

// =============================================================================
// btcpu_GdbBreakSoftBreakPoint
// -----------------------------------------------------------------------------
/// same as breakpoint but skip the break instruction as it is placed in code not by gdb
// =============================================================================
PROTECTED VOID btcpu_GdbBreakSoftBreakPoint(UINT32 cpuid)
{
    VOLATILE UINT32* btcpu_GdbRegs;
    VOLATILE UINT32* v_regs;
    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_GDB, 0, FALSE);
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);
    btcpu_GdbRegs[37]+=INC_STEP;

//  send command 
    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGTRAP, FALSE);
    
    btcpu_GdbLoop(cpuid, v_regs, FALSE);
}

// =============================================================================
// btcpu_GdbBreakRaise
// -----------------------------------------------------------------------------
/// same as breakpoint but skip the break instruction as it is placed in code not by gdb
/// used by stack for raises
// =============================================================================
PROTECTED VOID btcpu_GdbBreakRaise(UINT32 cpuid)
{
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_RAISE, 0, TRUE);
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);
    btcpu_GdbRegs[37]+=INC_STEP;

//  send command 
    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGKILL, TRUE);
    
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_RAISE, 0, btcpu_GdbRegs);

    btcpu_GdbLoop(cpuid, v_regs, TRUE);
}


// =============================================================================
// btcpu_GdbIrqDebug
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbIrqDebug(UINT32 cpuid, UINT32 cause)
{
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    UINT32 error_code = HAL_DBG_ERROR_GDB;
    BOOL fatal = FALSE;

    if (0) // should be (cause & SYS_IRQ_HOST_IRQ) ??
    {
        // hal_SetWatch(cpuid);
        hwp_debugHost->irq = (0x1 << cpuid);
    }

    // Only DbgIrq is writtable in the register.
    hwp_sysIrq->nonmaskable = 0;

#ifndef NO_BCPU_IMAGE
    if (bcpu_error_code != HAL_DBG_ERROR_NO)
    {
        error_code = HAL_DBG_ERROR_BCPU;
        fatal = TRUE;
    }
#endif // !NO_BCPU_IMAGE

    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, error_code, 0, fatal);
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);

    if(!fatal)
    {
//  don't actually send command if the BCPU triggered the IT
        btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGINT, FALSE);
    }

    // xcpu check for bcpu error, if any trace it
#ifndef NO_BCPU_IMAGE
    if ((cpuid == 0) && (bcpu_error_code != HAL_DBG_ERROR_NO))
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
        //btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
        //btcpu_DbgPalFlushTrace ();
#endif
        switch(bcpu_error_code) {
        case HAL_DBG_ERROR_IT:
            HAL_TRACE(_SXR|TABORT|TSTDOUT|TSMAP(0x2), 0,
                "BCPU EXCEPTION CODE %d, %s\n\n"
                "PC %lxy\n"
                "SP %lxy\n",
                ((bcpu_error_status>>2)&0x1f),
                excCodeName[((bcpu_error_status>>2) & 0x1f)],
                ((VOLATILE UINT32*)bcpu_sp_context)[37],
                ((VOLATILE UINT32*)bcpu_sp_context)[29]);
            break;
        case HAL_DBG_ERROR_GDB:
            HAL_TRACE(_SXR|TABORT|TSTDOUT, 0,
                "BCPU Stopped on breakpoint\n"
                "PC %lxy\n"
                "SP %lxy\n",
                ((VOLATILE UINT32*)bcpu_sp_context)[37],
                ((VOLATILE UINT32*)bcpu_sp_context)[29]);
            break;
        case HAL_DBG_ERROR_DIV0:
            HAL_TRACE(_SXR|TABORT|TSTDOUT, 0,
                "BCPU Division by 0 exception\n"
                "PC %lxy\n"
                "SP %lxy\n",
                ((VOLATILE UINT32*)bcpu_sp_context)[37],
                ((VOLATILE UINT32*)bcpu_sp_context)[29]);
            break;
        default:
            HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, 
                "BCPU ERROR UNKNOWN"
                ", (or memory corruption @0x20)");
            break;
        }

        btcpu_GdbBackTrace(btcpu_GdbRegs);

        GDB_CSW_FUNC(1, bcpu_error_code, bcpu_error_status, bcpu_sp_context);
    }
    else
#else // NO_BCPU_IMAGE
    {
#ifndef PAL_NO_DBGTRC
        //btcpu_DbgPalFlushTrace ();
#endif
        HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, "BTCPU debug IRQ");
        btcpu_GdbBackTrace(btcpu_GdbRegs);
    }
#endif // NO_BCPU_IMAGE

    btcpu_GdbLoop(cpuid, v_regs, fatal);
}

// =============================================================================
// btcpu_GdbIrqPage
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbIrqPage(UINT32 cpuid)
{
#if 0
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    UINT8 page;
    UINT8 master;
    UINT32 address;
    UINT32 en;
    UINT32 pagecode;

    // NOTE: only XCPU has page spy:
    //  - cpuid will always be 0 here, keep code generic anyway

    {
        UINT32 status = hwp_pageSpy->status;
        for(page=PAGE_SPY_NB_PAGE-1;(status & (1<<page))==0;page--);
        master = btcpu_DbgPageProtectGetHitMaster(page);
        address = btcpu_DbgPageProtectGetHitAddress(page);
        pagecode = 
         (address &0x01FFFFFF) | ((master&0xF)<<28) | (page<<26);
    }
    
    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_PAGE, pagecode, !hal_PageNoraise);


    if (hal_PageNoraise) 
    {
#ifndef HAL_NO_WATCH_REGS
        hwp_testerChip->W0 = (cpuid << 16) | HAL_DBG_ERROR_PAGE;
        {
            hwp_testerChip->W1 = 0xfa9e0000 + page;
            hwp_testerChip->W1 = pagecode;
        }
#endif
        en = hwp_pageSpy->enable;
        hwp_pageSpy->disable = en;
        hwp_pageSpy->enable  = en;
        return;
    }

    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);

    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, HAL_GDB_SIGKILL, TRUE);
    
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
        //btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
        //btcpu_DbgPalFlushTrace ();
#endif
        HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, 
                "Write Protect exception: Page %d, master: %d, addr: %x \n\n"
                "PC %lxy\n"
                "SP %lxy\n",
                page,
                master,
                address,
                btcpu_GdbRegs[37],
                btcpu_GdbRegs[29]);
        btcpu_GdbBackTrace(btcpu_GdbRegs);
        GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_PAGE, pagecode, btcpu_GdbRegs);
    }
//  clear and reenable pages done in handle
    btcpu_GdbLoop(cpuid, v_regs, TRUE);
#endif    
}

// =============================================================================
// btcpu_GdbException
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbException(UINT32 cpuid, UINT32 cause)
{
    VOLATILE UINT32 * btcpu_GdbRegs;
    VOLATILE UINT32 * v_regs;
    UINT32 signal;
    btcpu_GdbRegs = btcpu_GdbPrepare(cpuid, HAL_DBG_ERROR_IT, cause, TRUE);

    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);

//  send command 
    switch(cause){
    case ExcCode_IBE:
    case ExcCode_DBE:
        signal = HAL_GDB_SIGBUS;
        break;
    case ExcCode_RI:
        signal = HAL_GDB_SIGILL;
        break;
    default:
        signal = HAL_GDB_SIGSEGV;
        break;
    }
    btcpu_GdbSendcmd(cpuid, btcpu_GdbRegs, signal, TRUE);

    HAL_PROFILE_PULSE(XCPU_ERROR);
    //btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
    //btcpu_DbgPalFlushTrace ();
#endif
    HAL_TRACE(_SXR|TABORT|TSTDOUT|TSMAP(0x2), 0, 
            "BTCPU EXCEPTION CODE %d, %s\n\n"
            "PC %lxy\n"
            "SP %lxy\n",
            ((cause>>2)&0x1f),
            excCodeName[((cause>>2) & 0x1f)],
            btcpu_GdbRegs[37],
            btcpu_GdbRegs[29]);

    btcpu_GdbBackTrace(btcpu_GdbRegs);
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_IT, cause, btcpu_GdbRegs);

    btcpu_GdbLoop(cpuid, v_regs, TRUE);
}



// =============================================================================
// btcpu_GdbHandle
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbHandle(UINT32 cpuid, VOLATILE UINT32 * regs)
{
    //  Disable page_spy now, or it will SIGKILL us. *
//      * Re-enable on exit from the critical section. 
    HAL_GDB_PAGE_DISABLE;
    
    HAL_GDB_ENABLE_PM_ACCESS;
    
    while (1)
    {
        switch(regs[HAL_GDB_CMD_OFF])
        {
        case HAL_GDB_CMD_FLUSH_CACHE:            
            //btcpu_GdbFlushCache();
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
            break;
        case HAL_GDB_SET_BREAKPOINT :            
            btcpu_GdbSetBreakpoint(cpuid,regs);
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
            break;
        case HAL_GDB_UNSET_BREAKPOINT :
            btcpu_GdbUnsetBreakpoint(cpuid,regs);
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
            break;
        case HAL_GDB_CMD_STEP:
            btcpu_GdbStep(cpuid, 0, regs);
            HAL_GDB_PAGE_ENABLE;
            // Restore proxy memory access to its before
            // GDB state.
//            HAL_GDB_DISABLE_PM_ACCESS;
            return;
        case HAL_GDB_CMD_CONTINUE:
            HAL_GDB_PAGE_ENABLE;
            // Restore proxy memory access to its before
            // GDB state.
//            HAL_GDB_DISABLE_PM_ACCESS;
            return ;
        default:
            break;
        }
#ifndef HAL_GDB_SAFE_WITHOUT_SX
        btcpu_GdbDebugHook();
        sxs_SerialFlush ();
#endif // HAL_GDB_SAFE_WITHOUT_SX
    }
}


// =============================================================================
// btcpu_GdbSetBreakpoint
// -----------------------------------------------------------------------------
///  Manage the setting of a breakpoint when asked by the GDB server. This basi-
///  cally means to squash the current opcode by the break opcode. The opcode
///  is memorized by the GDB server for it to be restored.
// =============================================================================
PROTECTED VOID btcpu_GdbSetBreakpoint(UINT32 cpuid, VOLATILE UINT32* regs)
{
    UINT32 addr = regs[HAL_GDB_CMDPARAM_OFF];

    //BOOL    flashWriteEnable = 0;
    // Take special action if the instruction to change
    // is in flash or in 
    // TODO : Rom
        

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#endif
#else
    if ((addr & 0x0FF00000) == REG_INT_ROM_BASE)
    { 
        // This instruction is in internal ROM
        // We need to copy the whole line to patch
        // it. The line being patched become writtable
        UINT32 wordIdx = 0;
        // Copy Rom line into patch memory
        for (wordIdx = 0 ; wordIdx<4; wordIdx++)
        {
            *((UINT32*)(KSEG0(REG_INT_SRAM_BASE)) + 4*HAL_GDB_PATCH_NUMBER + wordIdx) = 
                            *((UINT32*)(KSEG0(((addr & MEM_BRIDGE_BLOCK_ADDR_MASK)| REG_INT_ROM_BASE))) + wordIdx);
        }

    }
#endif

    if(addr & 1) //MIPS16
    {
        *(UINT16*)(addr&~1) = HAL_GDB_16_OPCODE_STEP;
    }
    else         //MIPS32
    {
        *(UINT32*)(addr) = HAL_GDB_OPCODE_STEP;        
    }
 
    // Take special action if the instruction to change
    // is in flash or in

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#else
    // We need to keep the patch enabled until the breakpoint is removed.
#endif
}

// =============================================================================
// btcpu_GdbUnsetBreakpoint
// -----------------------------------------------------------------------------
///  Manage the unsetting of a breakpoint when asked by the GDB server. This basi-
///  cally means to squash the current break opcode by the true opcode. The opcode
///  was memorized by the GDB server for it to be restored.
// =============================================================================
PROTECTED VOID btcpu_GdbUnsetBreakpoint(UINT32 cpuid, VOLATILE UINT32* regs)
{
    UINT32 addr         = regs[HAL_GDB_CMDPARAM_OFF];
    // The opcode to be restored (memorized by the GDB server).
    UINT32 theOpCode    = regs[HAL_GDB_CMDPARAM2_OFF]; //Because it's the one

    //BOOL    flashWriteEnable = 0;
    // Take special action if the instruction to change
    // is in flash or in 

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#endif
#else
    // We only use one patch page to handle GDB in ROM 
    // code. So we can safely assume that the patch 
    // was enabled to set the breakpoint we are to remove.
#endif

    if(addr & 1) //MIPS16
    {
        *(UINT16*)(addr&~1) = (UINT16)theOpCode;
    }
    else         //MIPS32
    {
        *(UINT32*)(addr) = theOpCode;        
    }
 
    // Take special action if the instruction to change
    // is in flash or in

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_memBridge_BB->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#else
    // Disabling the patch would restore the opcode again.
    // It would also remove any step in progress, that 
    // would anyhow have changed the patched area, restoring
    // de facto the code before the set breakpoint.
#endif
}




// =============================================================================
// btcpu_Gdb16Step
// -----------------------------------------------------------------------------
///  Uses a TRAP to generate an exception after we run the next instruction.
// =============================================================================
PROTECTED UINT32 btcpu_Gdb16Step (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * btcpu_GdbRegs)
{
    //UINT32     nNewAddr;
    //UINT16     *nOp=(UINT16*)MASK_ONE(btcpu_GdbRegs[37]);


    
    return nAddr+2;
}


// =============================================================================
// btcpu_Gdb32Step
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED UINT32 btcpu_Gdb32Step (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * btcpu_GdbRegs)
{
    //UINT32     nNewAddr;
    //UINT32     nOp=*(UINT32*)btcpu_GdbRegs[37];
  

    
    return nAddr+4;
}

// =============================================================================
// btcpu_Gdb16EndStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_Gdb16EndStep (UINT32 cpuid, UINT32 nAddr)
{
    // Flag to prevent mips16 opcode to look like 0
    // and be discared improperly by btcpu_GdbUndoStep, which
    // test opCode against 0 to check if there is a step to
    // undo.
    //  save the target opcode, replace with HAL_GDB_16_OPCODE_STEP 
    btcpu_GdbGData[cpuid].opCode = *(UINT16*)MASK_ONE(nAddr) | HAL_GDB_MIPS16_OPCODE_FLAG;
    btcpu_GdbGData[cpuid].stepAddr = nAddr;
    *(UINT16*)MASK_ONE(nAddr) = HAL_GDB_16_OPCODE_STEP;
}

// =============================================================================
// btcpu_Gdb32EndStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_Gdb32EndStep (UINT32 cpuid, UINT32 nAddr)
{
    while(*(UINT32*)nAddr == 0)
    {
        nAddr+=4;
    }

//  save the target opcode, replace with HAL_GDB_OPCODE_STEP 
    btcpu_GdbGData[cpuid].opCode = *(UINT32*)nAddr;
    btcpu_GdbGData[cpuid].stepAddr = nAddr;
    *(UINT32*)nAddr = HAL_GDB_OPCODE_STEP;
}

// =============================================================================
// btcpu_GdbStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbStep (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * btcpu_GdbRegs)
{
    //UINT32 nNewAddr;

    // Take special action if the instruction to change
    // is in flash or in Rom

    //BOOL flashWriteEnable = 0;
        


}

// =============================================================================
// btcpu_GdbUndoStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbUndoStep (UINT32 cpuid, VOLATILE UINT32 * btcpu_GdbRegs)
{
    //UINT32     nNewAddr = btcpu_GdbGData[cpuid].stepAddr;
    //UINT32     isMips16 = (btcpu_GdbRegs[37] & 0x01);

    
}


// =============================================================================
// btcpu_GdbStrLen
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED UINT32 btcpu_GdbStrLen(UINT8* pcStr)
{
    UINT32 i=0;
    while((*pcStr++) != '\0') i++;
    return i;
}

// =============================================================================
// btcpu_GdbBackTrace
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID btcpu_GdbBackTrace(VOLATILE UINT32 *btcpu_GdbRegs)
{
#ifndef HAL_GDB_SAFE_WITHOUT_SX
    UINT32 *Sp = (UINT32*)btcpu_GdbRegs[29];

    sxs_BackTrace(btcpu_GdbRegs[37], Sp);
#ifdef HAL_GDB_CONFIG
    if (g_halGdbEnabled != 0)
#endif // HAL_GDB_CONFIG
    {
        HAL_TRACE(_SXR|TSTDOUT, 0, "#####################");
        HAL_TRACE(_SXR|TSTDOUT, 0, "To Get a complete context connect GDB\n");
    }
    sxs_Boot -> ExceptionExpected = TRUE;
    sxs_Boot -> ExceptionCount++;
    sxs_SaveRmtBuffer ();
#endif // HAL_GDB_SAFE_WITHOUT_SX
}

// commented btcpu_Gdb_WatchDog_handler:
// used to fill a gdb context from the WD context, however this is not really accurate
// kept code here if needed
#if 0
// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================
VOID btcpu_GdbWatchDogHandler(VOID)
{
    const UINT32 cpuid = 0;
    // only handled on xcpu
    UINT32 btcpu_GdbRegs[HAL_GDB_REGNUM_XTD];
    VOLATILE UINT32 * v_regs;
    INT32 i;
    HAL_WATCHDOG_CTX_T* g_halWatchdogCtx=(HAL_WATCHDOG_CTX_T*) KSEG0(INT_SRAM_BASE);
    
    HAL_PROFILE_PULSE(XCPU_ERROR);
    btcpu_DbgFatalTrigger(HAL_DBG_EXL_FATAL);

    // build a gdb context from the WDog context
    btcpu_GdbRegs[0]=0;
    for(i=1;i<26;i++)
     btcpu_GdbRegs[i] = ((UINT32*)(g_halWatchdogCtx))[i-1]; // copy r1 to r25
    btcpu_GdbRegs[26]=0; // k0
    for(i=27;i<32;i++)
     btcpu_GdbRegs[i] = ((UINT32*)(g_halWatchdogCtx))[i-2]; // copy r27 to r31
    for(i=32;i<37;i++)
        btcpu_GdbRegs[i]=0; // clear unavailable registers
    btcpu_GdbRegs[37]=g_halWatchdogCtx->pc | 0x80000001; // EPC assuming caches mips16

    // set the context
    xcpu_sp_context = btcpu_GdbRegs;
    
    hwp_reg->reg_dbg = HAL_DBG_ERROR_GDB;
    xcpu_error_code = HAL_DBG_ERROR_WDOG;
    xcpu_error_status = 0;
    v_regs=(UINT32*)KSEG1((UINT32)btcpu_GdbRegs);

//  prepare command 
    btcpu_GdbRegs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_BREAKPOINT;
    btcpu_GdbRegs[HAL_GDB_CMDPARAM_OFF] = HAL_GDB_SIGKILL;
//  trigger command 
    btcpu_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
    
//  wait for the debug server to complete 
    while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    
    btcpu_GdbUndoStep(cpuid, v_regs);
    btcpu_GdbHandle(cpuid, v_regs);
    HAL_PROFILE_PULSE(XCPU_ERROR);
    // FIXME btcpu_DbgExlClrTrigger(HAL_DBG_EXL_FATAL);
}
#endif

// =============================================================================
// btcpu_DbgErrorHandlerSet
// -----------------------------------------------------------------------------
/// Set the error handler from the upper layers.
/// @param handler Handler called when an error occurs on one of the CPU.
// =============================================================================
PUBLIC VOID btcpu_DbgErrorHandlerSet(HAL_DBG_ERROR_HANDLER_T handler)
{
    g_halErrorRegistry = handler;
}

#ifndef HAL_GDB_SAFE_WITHOUT_SX
// =============================================================================
// btcpu_GdbSwitchContext
// -----------------------------------------------------------------------------
/// Switch the task context after GDB occurs.
/// @param id The task or job ID that will be switched to.
/// @param isJob TRUE if switching to a job context.
// =============================================================================
PRIVATE VOID btcpu_GdbSwitchContext(UINT32 id, UINT32 isJob)
{
    if (g_halGdbRegsBackup == NULL)
    {
        g_halGdbRegsBackup = btcpu_sp_context /* xcpu_sp_context */;
    }

    if (isJob != 0)
    {
        id = (UINT16)id;
    }
    else
    {
        id = (UINT8)id;
    }

    #define SXR_NO_TASK (0xFF)
    #define INVALID_JOB_ID (0xFFFF)

    if (
         (isJob == 0 && (id == SXR_NO_TASK || id == sxr_GetCurrentTaskId())) ||
         (isJob !=0 && ( id == INVALID_JOB_ID ||(sxr_GetActiveJob() !=SXR_NIL_JOB && id == sxr_GetJobId(sxr_GetActiveJob()))))
       )
    {
        btcpu_sp_context /* xcpu_sp_context */ = g_halGdbRegsBackup;
        g_halGdbSwitchCtx.cmd = HAL_GDB_SWITCH_CTX_CMD_RESTORED;
        return;
    }

    UINT32 *sp;
    if (isJob != 0)
    {
        sp = sxr_GetJobSpById(id);
    }
    else
    {
        sp = sxr_GetTaskSp(id);
    }

    if (sp == NULL)
    {
        g_halGdbSwitchCtx.cmd = HAL_GDB_SWITCH_CTX_CMD_ERR;
        return;
    }

    g_halGdbTempRegs[0] = (id<<16|isJob<<8);
    for (UINT32 i=1; i<HAL_GDB_REGNUM_XTD; i++)
    {
        g_halGdbTempRegs[i] = 0;
    }

    if (isJob != 0)
    {
        g_halGdbTempRegs[16] = sp[JC_S0/4];
        g_halGdbTempRegs[17] = sp[JC_S1/4];
        g_halGdbTempRegs[4] = sp[JC_A0/4];
        //g_halGdbTempRegs[5] = sp[JC_A1/4]; // use as jobNum instead
        g_halGdbTempRegs[37] = sp[JC_PC/4]; // EPC
        g_halGdbTempRegs[31] = sp[JC_PC/4]; // RA
#ifndef __REDUCED_REGS__
        g_halGdbTempRegs[18] = sp[JC_S2/4];
        g_halGdbTempRegs[19] = sp[JC_S3/4];
        g_halGdbTempRegs[20] = sp[JC_S4/4];
        g_halGdbTempRegs[21] = sp[JC_S5/4];
        g_halGdbTempRegs[22] = sp[JC_S6/4];
        g_halGdbTempRegs[23] = sp[JC_S7/4];
        g_halGdbTempRegs[30] = sp[JC_FP/4];
#endif
        sp += (JOB_CTX_SIZE/4);
    }
    else
    {
        INT32 scCtxType = (INT32)sp[SC_CTX_TYPE/4];
        g_halGdbTempRegs[16] = sp[SC_S0/4];
        g_halGdbTempRegs[17] = sp[SC_S1/4];
        g_halGdbTempRegs[4] = sp[SC_A0/4];
        g_halGdbTempRegs[37] = sp[SC_PC/4]; // EPC
        g_halGdbTempRegs[31] = sp[SC_PC/4]; // RA
#ifndef __REDUCED_REGS__
        g_halGdbTempRegs[18] = sp[SC_S2/4];
        g_halGdbTempRegs[19] = sp[SC_S3/4];
        g_halGdbTempRegs[20] = sp[SC_S4/4];
        g_halGdbTempRegs[21] = sp[SC_S5/4];
        g_halGdbTempRegs[22] = sp[SC_S6/4];
        g_halGdbTempRegs[23] = sp[SC_S7/4];
#endif
        sp += (TASK_SMALL_CTX_SIZE/4);

        if (scCtxType < 0)
        {
            g_halGdbTempRegs[33] = sp[FC_LO/4];
            g_halGdbTempRegs[34] = sp[FC_HI/4];
            g_halGdbTempRegs[2] = sp[FC_V0/4];
            g_halGdbTempRegs[3] = sp[FC_V1/4];
            g_halGdbTempRegs[5] = sp[FC_A1/4];
            g_halGdbTempRegs[6] = sp[FC_A2/4];
            g_halGdbTempRegs[7] = sp[FC_A3/4];
            g_halGdbTempRegs[8] = sp[FC_T0/4];
            g_halGdbTempRegs[9] = sp[FC_T1/4];
            g_halGdbTempRegs[10] = sp[FC_T2/4];
            g_halGdbTempRegs[24] = sp[FC_T8/4];
            g_halGdbTempRegs[25] = sp[FC_T9/4];
            g_halGdbTempRegs[31] = sp[FC_RA/4];
            g_halGdbTempRegs[1] = sp[FC_AT/4];
#ifndef __REDUCED_REGS__
            g_halGdbTempRegs[11] = sp[FC_T3/4];
            g_halGdbTempRegs[12] = sp[FC_T4/4];
            g_halGdbTempRegs[13] = sp[FC_T5/4];
            g_halGdbTempRegs[14] = sp[FC_T6/4];
            g_halGdbTempRegs[15] = sp[FC_T7/4];
#endif
            sp += (TASK_EXTRA_CTX_SIZE/4);
        }
    }
    g_halGdbTempRegs[29] = (UINT32)sp; // SP
    btcpu_sp_context /* xcpu_sp_context */ = g_halGdbTempRegs;
    g_halGdbSwitchCtx.cmd = HAL_GDB_SWITCH_CTX_CMD_OK;
}

#ifndef HAL_GDB_SAFE_WITHOUT_SX
PRIVATE VOID btcpu_GdbCheckSwitchContextCmd(VOID)
{
    if (g_halGdbSwitchCtx.cmd == HAL_GDB_SWITCH_CTX_CMD_START)
    {
        btcpu_GdbSwitchContext(g_halGdbSwitchCtx.id, g_halGdbSwitchCtx.isJob);
    }
}

PRIVATE VOID btcpu_GdbDebugHook(VOID)
{
    for (int i=0; i<HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM; i++)
    {
        if (g_halGdbDebugHookFuncPtr[i] != NULL)
        {
            (*g_halGdbDebugHookFuncPtr[i])();
        }
    }
}
#endif

PUBLIC BOOL btcpu_GdbRegisterDebugHookFunc(void (*funcPtr)(void))
{
    int i;
    bool result = FALSE;

    u32 status = hal_SysEnterCriticalSection();
    for (i=0; i<HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM; i++)
    {
        if (g_halGdbDebugHookFuncPtr[i] == NULL)
            break;
    }

    if (i < HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM)
    {
        g_halGdbDebugHookFuncPtr[i] = funcPtr;
        result = TRUE;
    }
    hal_SysExitCriticalSection(status);

    return result;
}

#endif // !HAL_GDB_SAFE_WITHOUT_SX

