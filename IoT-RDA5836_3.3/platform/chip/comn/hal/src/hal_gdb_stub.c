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

#include "cp0.h"
#undef Cause
// to fix conflict between cp0 Cause reg and Irq Cause reg !!

#include "boot.h"
#include "halp_gdb_stub.h"
#include "halp_page_spy.h"
#include "halp_timers.h"
#include "halp_debug.h"
#include "halp_irq_handler.h"
#include "hal_debugpaltrc.h"
#include "hal_ebc.h"
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

#include "memd_m.h"

extern void mon_Event(UINT32 event);

#ifndef CHIP_HAS_BCPU_ROM
#define CHIP_HAS_BCPU_ROM 0
#endif

// =============================================================================
//  MACROS
// =============================================================================

#define HAL_GDB_XCPU_EVENT  0x9db00000

#ifndef HAL_NO_PRINTF
//  Exception code names 
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
#endif

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
#define INC_STEP ((hal_GdbRegs[37] & 0x01) ? 2 : 4)
#define MASK_ONE(addr) ((addr)&(~1))

#define HAL_GDB_GET_OFFSET(op)   (((((op) & 0x00008000) == 0x00008000) ? \
        ((op) | 0xffff0000) : ((op) & 0x0000ffff)) << 2)

#define HAL_GDB_OPCODE_J(op)     (((op) & 0xfc000000) == 0x08000000)
#define HAL_GDB_OPADDR_J(op)     ((((op) & 0x03ffffff) << 2) | (hal_GdbRegs[37] & 0xf0000000))
#define HAL_GDB_OPCODE_JAL(op)   (((op) & 0xfc000000) == 0x0c000000)
#define HAL_GDB_OPADDR_JAL(op)   ((((op) & 0x03ffffff) << 2) | (hal_GdbRegs[37] & 0xf0000000))
#define HAL_GDB_OPCODE_JALX(op)  ((((op) & 0xfc000000) == 0x74000000))
#define HAL_GDB_OPADDR_JALX(op)  ((((op) & 0x03ffffff) << 2) | (hal_GdbRegs[37] & 0xf0000000) | 1)
#define HAL_GDB_OPCODE_JR(op)    (((op) & 0xfc1ff83f) == 0x00000008)
#define HAL_GDB_OPADDR_JR(op)    (hal_GdbRegs[((op) & 0x03e00000) >> 21])
#define HAL_GDB_OPCODE_JALR(op)  (((op) & 0xfc1f003f) == 0x00000009)
#define HAL_GDB_OPADDR_JALR(op)  (hal_GdbRegs[((op) & 0x03e00000) >> 21])
#define HAL_GDB_OPCODE_BEQ(op)   (((op) & 0xfc000000) == 0x10000000)
#define HAL_GDB_OPADDR_BEQ(op)   ((hal_GdbRegs[((op) & 0x03e00000) >> 21] == hal_GdbRegs[((op)   \
        & 0x001f0000) >> 16]) ? (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BNE(op)   (((op) & 0xfc000000) == 0x14000000)
#define HAL_GDB_OPADDR_BNE(op)   ((hal_GdbRegs[((op) & 0x03e00000) >> 21] != hal_GdbRegs[((op)   \
        & 0x001f0000) >> 16]) ? (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGEZ(op)  (((op) & 0xfc1f0000) == 0x04010000)
#define HAL_GDB_OPADDR_BGEZ(op)  (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] >= 0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGEZAL(op)    (((op) & 0xfc1f0000) == 0x04110000)
#define HAL_GDB_OPADDR_BGEZAL(op)    (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] >= 0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BGTZ(op)  (((op) & 0xfc1f0000) == 0x1c000000)
#define HAL_GDB_OPADDR_BGTZ(op)  (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] >  0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLEZ(op)  (((op) & 0xfc1f0000) == 0x18000000)
#define HAL_GDB_OPADDR_BLEZ(op)  (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] <= 0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLTZ(op)  (((op) & 0xfc1f0000) == 0x04000000)
#define HAL_GDB_OPADDR_BLTZ(op)  (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] <  0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))
#define HAL_GDB_OPCODE_BLTZAL(op)    (((op) & 0xfc1f0000) == 0x04100000)
#define HAL_GDB_OPADDR_BLTZAL(op)    (((INT32)hal_GdbRegs[((op) & 0x03e00000) >> 21] <  0) ?   \
        (hal_GdbRegs[37]+4+HAL_GDB_GET_OFFSET(op)) : (hal_GdbRegs[37]+4))


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
#define HAL_GDB_16_REGS(n)      hal_GdbRegs[HAL_GDB_16_MAP_REGS(n)]   
    
//  Helper for messy offsets on extended branches 
#define HAL_GDB_16_ASSEMBLE_EXT_OFFSET(op) \
                 ( (*((op)+1) & 0x001f) + ((*(op)&0x07e0))  + ((*(op) & 0x001f) << 11) ) 
//  GNU C only, but much cleaner... 
#define HAL_GDB_16_GET_EXT_OFFSET(op) \
        ({ UINT32 _off = HAL_GDB_16_ASSEMBLE_EXT_OFFSET((op)); (_off & 0x8000) ? ((_off | 0xffff0000)<<1) : ((_off & 0x0000ffff)<<1); })

#define HAL_GDB_16_OPCODE_JAL(op)    ((*(op) & 0xfc00) == 0x1800)
#define HAL_GDB_16_OPADDR_JAL(op)    ((0x80000001) | ( ((hal_GdbRegs[37] + 2)&0xe0000000) + ( ( *((op)+1) + ((*(op) & 0x001f) << 21) \
        + ((*(op) & 0x03e0) << 11) ) << 2 )) )
#define HAL_GDB_16_OPCODE_JALX(op)   ((*(op) & 0xfc00) == 0x1c00)
#define HAL_GDB_16_OPADDR_JALX(op)   ((0x80000000) | (( (hal_GdbRegs[37] + 2)&0xe0000000) + ( ( *((op)+1) + ((*(op) & 0x001f) << 21) \
        + ((*(op) & 0x03e0) << 11 ) ) << 2 )) )
#define HAL_GDB_16_OPCODE_JALR(op)   ((*(op) & 0xf85f) == 0xe840)
#define HAL_GDB_16_OPADDR_JALR(op)   (HAL_GDB_16_REGS((*(op) & 0x0700) >> 8))
#define HAL_GDB_16_OPCODE_JR(op)     ((*(op) & 0xf85f) == 0xe800)
#define HAL_GDB_16_OPADDR_JR(op)     ( (*(op) & 0x0020) ? \
        hal_GdbRegs[31] : HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) )
#define HAL_GDB_16_OPCODE_B(op)      ((*(op) & 0xf800) == 0x1000)
#define HAL_GDB_16_OPADDR_B(op)      (hal_GdbRegs[37]+2+HAL_GDB_16_GET_11_OFFSET(op))
#define HAL_GDB_16_OPCODE_BEQZ(op)   ((*(op) & 0xf800) == 0x2000)
#define HAL_GDB_16_OPADDR_BEQZ(op)   ((HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) == 0 ) ? \
        hal_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : hal_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BNEZ(op)   ((*(op) & 0xf800) == 0x2800)
#define HAL_GDB_16_OPADDR_BNEZ(op)   ((HAL_GDB_16_REGS((*(op) & 0x0700) >> 8) != 0 ) ? \
        hal_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : hal_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BTEQZ(op)  ((*(op) & 0xff00) == 0x6000)
#define HAL_GDB_16_OPADDR_BTEQZ(op)  ((hal_GdbRegs[24] == 0 ) ? \
        hal_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : hal_GdbRegs[37]+2)
#define HAL_GDB_16_OPCODE_BTNEZ(op)  ((*(op) & 0xff00) == 0x6100)
#define HAL_GDB_16_OPADDR_BTNEZ(op)  ((hal_GdbRegs[24] != 0 ) ? \
        hal_GdbRegs[37]+2+HAL_GDB_16_GET_8_OFFSET(op) : hal_GdbRegs[37]+2)

//  Separate test for extended insns 
#define HAL_GDB_16_OPCODE_EXTENDED(op)   ((*(op) & 0xf800) == 0xf000)

                                                            

//  Only valid if already tested as extended 
#define HAL_GDB_16_OPCODE_B_EXT(op)      ((*(&*(op)+1) & 0xf800) == 0x1000)
#define HAL_GDB_16_OPADDR_B_EXT(op)      ( hal_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) )
#define HAL_GDB_16_OPCODE_BEQZ_EXT(op)       ((*((op)+1) & 0xf800) == 0x2000)
#define HAL_GDB_16_OPADDR_BEQZ_EXT(op)       ((HAL_GDB_16_REGS((*((op)+1) & 0x0700) >> 8) == 0) ? \
        ( hal_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : hal_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BNEZ_EXT(op)       ((*((op)+1) & 0xf800) == 0x2800)
#define HAL_GDB_16_OPADDR_BNEZ_EXT(op)       ((HAL_GDB_16_REGS((*((op)+1) & 0x0700) >> 8) != 0) ? \
        ( hal_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : hal_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BTEQZ_EXT(op)      ((*((op)+1) & 0xff00) == 0x6000)
#define HAL_GDB_16_OPADDR_BTEQZ_EXT(op)      ((hal_GdbRegs[24] == 0) ? \
        ( hal_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : hal_GdbRegs[37] + 4 )
#define HAL_GDB_16_OPCODE_BTNEZ_EXT(op)      ((*((op)+1) & 0xff00) == 0x6100)
#define HAL_GDB_16_OPADDR_BTNEZ_EXT(op)      ((hal_GdbRegs[24] != 0) ? \
        ( hal_GdbRegs[37] + 4 + HAL_GDB_16_GET_EXT_OFFSET(op) ) : hal_GdbRegs[37] + 4 )



                                                                  

/// Flag to store a mips16 opcode in hal_GdbGData[cpuid].opCode
/// and make it different of 0. HAL_GDB_MIPS16_OPCODE_MASK is used
/// to restore the proper opcode, discarding the flag.
#define HAL_GDB_MIPS16_OPCODE_FLAG      0xdb900000
#define HAL_GDB_MIPS16_OPCODE_MASK      0x0000FFFF

/// Patch number used for the one step allowed in ROM
#define HAL_GDB_PATCH_NUMBER            (CHIP_NUMBER_OF_PATCHES-1)


// ****************************************************************************
//                          LOCAL FUNCTION PROTOTYPES                          
// ****************************************************************************
PRIVATE VOID hal_GdbHandle(UINT32 cpuid, VOLATILE UINT32 * regs);
PRIVATE VOID hal_GdbStep (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * hal_GdbRegs);
PRIVATE VOID hal_GdbUndoStep (UINT32 cpuid, VOLATILE UINT32 * hal_GdbRegs);
PRIVATE VOID hal_GdbSetBreakpoint(UINT32 cpuid,VOLATILE UINT32* hal_GdbRegs);
PRIVATE VOID hal_GdbUnsetBreakpoint(UINT32 cpuid,VOLATILE UINT32* hal_GdbRegs);
extern VOID hal_GdbFlushCache();
PRIVATE UINT32 hal_GdbStrLen(UINT8* pcStr);
PRIVATE VOID hal_GdbBackTrace(VOLATILE UINT32 *);
#ifndef HAL_GDB_SAFE_WITHOUT_SX
PRIVATE VOID hal_GdbDebugHook(VOID);
PRIVATE VOID hal_GdbCheckSwitchContextCmd(VOID);
#endif


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

#ifdef CT_NO_DEBUG
PROTECTED VOLATILE UINT32 HAL_UNCACHED_DATA_INTERNAL g_halGdbEnabled = 0;
#else
PROTECTED VOLATILE UINT32 HAL_UNCACHED_DATA_INTERNAL g_halGdbEnabled = 1;
#endif


#endif // HAL_GDB_CONFIG

//  Enable/disable page_spy to avoid SIGKILL on breakpt insertion
PRIVATE UINT32 page_spy_enable;
PRIVATE UINT32 hal_PageNoraise = 0;
PRIVATE HAL_GDB_DATA_T   hal_GdbGData[2]= {{0,0},{0,0}};
PRIVATE HAL_DBG_ERROR_HANDLER_T g_halErrorRegistry  
            = NULL;

VOLATILE HAL_GDB_SWITCH_CTX_T HAL_UNCACHED_BSS g_halGdbSwitchCtx;

#ifndef HAL_GDB_SAFE_WITHOUT_SX
PRIVATE VOLATILE UINT32 g_halGdbTempRegs[HAL_GDB_REGNUM_XTD];
PRIVATE VOLATILE UINT32 *g_halGdbRegsBackup = NULL;

#define HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM 4
PRIVATE VOID (*g_halGdbDebugHookFuncPtr[HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM])(VOID) =
    { &hal_GdbCheckSwitchContextCmd, &hal_SysProcessHostMonitor, };
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
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK; \

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

#define HAL_DBG_STEPFORBIDDEN_FUNCTION_COUNT 2
PRIVATE UINT32* HAL_DBG_STEPFORBIDDEN_FUNCTIONS [HAL_DBG_STEPFORBIDDEN_FUNCTION_COUNT] = 
    {(UINT32*)hal_DbgGdbHstSendEvent, (UINT32*)mon_Event};




// *****************************************************************************
//                         FUNCTION IMPLEMENTATION                              
// *****************************************************************************
// That is very nice to have a dedicated leaf function to send events, since
// a breakpoint (for conventional debugging) in a function called by that 
// #hal_DbgGdbHstSendEvent function, would call GDB into GDB and lead to 
// a system collapse.
PROTECTED VOID hal_DbgGdbHstSendEvent(UINT32 ch)
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





// =============================================================================
// hal_GdbIsAddressStepForbidden
// -----------------------------------------------------------------------------
// Helper function to tell if a function is step-into-able or not. If it's not,
// we will cheat and set the breakpoint after the func call.
// =============================================================================

PRIVATE BOOL hal_GdbIsAddressStepForbidden(UINT32 newAddr)
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

// =============================================================================
// hal_GdbBreaks
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbBreaks(UINT32 cpuid)
{
    VOLATILE UINT32 * hal_GdbRegs;

    HAL_GDB_ENABLE_PM_ACCESS;

    hal_GdbRegs = xcpu_sp_context;

    hal_GdbRegs[37]+=INC_STEP;
    return; 
}

// =============================================================================
// hal_GdbBreakPrint
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbBreakPrint(UINT32 cpuid)
{
    UINT32     nLen;
    UINT8 * str;
    VOLATILE UINT32 * hal_GdbRegs;
    VOLATILE UINT32 * v_regs;
    
    HAL_GDB_ENABLE_PM_ACCESS;

    hal_GdbRegs = xcpu_sp_context;
//  stop the watchdog so we can use gdb 
    hal_TimWatchDogStop();
    
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);
        
    str = (UINT8*)(hal_GdbRegs[4]); // a0 before break


    nLen  = hal_GdbStrLen(str);
//  prepare command 
    v_regs[HAL_GDB_CMD_OFF]=HAL_GDB_CMD_PRINT;
    v_regs[HAL_GDB_CMDPARAM_OFF]=nLen;

//  trigger command 
    hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
    
//  wait for the debug server to complete 
    while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    
    hal_GdbRegs[37]+=INC_STEP;
//  restart the watchdog 
    hal_TimWatchDogKeepAlive();
    return; 
}

// =============================================================================
// hal_GdbPrepare
// -----------------------------------------------------------------------------
/// @return GDB context (cached)
// =============================================================================
PROTECTED VOLATILE UINT32 *hal_GdbPrepare(UINT32 cpuid, UINT32 error_code, UINT32 error_status, BOOL fatal)
{
    VOLATILE UINT32 * hal_GdbRegs;

    // If the chip has a proxy memory, and the external access
    // is disabled, enable it.
    HAL_GDB_ENABLE_PM_ACCESS;

    if (fatal)
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
        hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
    }

    hal_GdbRegs          = xcpu_sp_context;

#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    hwp_sysCtrl->REG_DBG = error_code;
#endif

    xcpu_error_code      = error_code;
    xcpu_error_status    = error_status;
    
	// Make sure BCPU can see the new value
    *MEM_ACCESS_UNCACHED(&xcpu_error_code) = error_code;
    
    //  stop the watchdog so we can use gdb 
    hal_TimWatchDogStop();
    hal_GdbFlushCacheData();

    // Prevent frame reentrance by masking the fint.
    // This doesn't allow to "continue" the code's execution, the real time
    // aspect is broken for good.
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
    hwp_sysIrq->pulse_mask_clr = SYS_IRQ_SYS_IRQ_FRAME;
#endif
    return hal_GdbRegs;
}

// =============================================================================
// hal_GdbSendcmd
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbSendcmd(UINT32 cpuid, VOLATILE UINT32 * hal_GdbRegs, UINT32 signal, BOOL fatal)
{
#ifdef HAL_GDB_CONFIG
    if (g_halGdbEnabled == 0)
    {
        hal_DbgGdbHstSendEvent(0x10+HAL_GDB_XCPU_EVENT);
        return;
    }
#endif // HAL_GDB_CONFIG
//  prepare command 
    hal_GdbRegs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_BREAKPOINT;
    hal_GdbRegs[HAL_GDB_CMDPARAM_OFF] = signal;
//  trigger command 
    hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
}

// =============================================================================
// hal_GdbIsInGdbLoop
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED BOOL hal_GdbIsInGdbLoop(VOID)
{
    return g_halGdbIsInGdbLoop;
}

// =============================================================================
// hal_GdbLoop
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbLoop(UINT32 cpuid, VOLATILE UINT32 * v_regs, BOOL fatal)
{
#ifdef CT_NO_DEBUG
     hal_SysRestart();
#else
    HAL_PROFILE_PULSE(hal_Gdb);

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
            hal_GdbDebugHook();
            sxs_SerialFlush ();
#endif // HAL_GDB_SAFE_WITHOUT_SX
        }
    }
    else
    {
        // wait for the debug server to complete
        while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    }
    hal_GdbUndoStep(cpuid, v_regs);
    hal_GdbHandle(cpuid, v_regs);
    if(fatal)
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
    }
    // restart the watchdog 
    hal_TimWatchDogKeepAlive();

    g_halGdbIsInGdbLoop = FALSE;

    return;
#endif
}

// =============================================================================
// hal_GdbBreakDivBy0
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbBreakDivBy0(UINT32 cpuid)
{
#ifdef CT_NO_DEBUG
     hal_SysRestart();
#else
    VOLATILE UINT32 * hal_GdbRegs;
    VOLATILE UINT32 * v_regs;

    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_DIV0, 0, TRUE);

    hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT+0xd0);
    hal_GdbRegs[37]+=INC_STEP;

    // trigger GDB on /0 for both CPUs
    
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);
    
//  send command 
    hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGFPE, TRUE);
    
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
    hal_DbgPalFlushTrace ();
#endif
    HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, 
            "XCPU Division by 0 exception\n"
            "PC %lxy\n"
            "SP %lxy\n",
            hal_GdbRegs[37],
            hal_GdbRegs[29]);
    hal_GdbBackTrace(hal_GdbRegs);
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_DIV0, 0, hal_GdbRegs);
    hal_GdbLoop(cpuid, v_regs, TRUE);
#endif
}

// =============================================================================
// hal_GdbBreakBreakPoint
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbBreakBreakPoint(UINT32 cpuid)
{
    VOLATILE UINT32 * hal_GdbRegs;
    VOLATILE UINT32 * v_regs;
    
    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_GDB, 0, FALSE);
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);

//  send command 
    hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGTRAP, FALSE);
    
    hal_GdbLoop(cpuid, v_regs, FALSE);
}

// =============================================================================
// hal_GdbBreakSoftBreakPoint
// -----------------------------------------------------------------------------
/// same as breakpoint but skip the break instruction as it is placed in code not by gdb
// =============================================================================
PROTECTED VOID hal_GdbBreakSoftBreakPoint(UINT32 cpuid)
{
    VOLATILE UINT32* hal_GdbRegs;
    VOLATILE UINT32* v_regs;
    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_GDB, 0, FALSE);
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);
    hal_GdbRegs[37]+=INC_STEP;

//  send command 
    hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGTRAP, FALSE);
    
    hal_GdbLoop(cpuid, v_regs, FALSE);
}

// =============================================================================
// hal_GdbBreakRaise
// -----------------------------------------------------------------------------
/// same as breakpoint but skip the break instruction as it is placed in code not by gdb
/// used by stack for raises
// =============================================================================
PROTECTED VOID hal_GdbBreakRaise(UINT32 cpuid)
{
#ifdef CT_NO_DEBUG
     hal_SysRestart();
#else
    VOLATILE UINT32 * hal_GdbRegs;
    VOLATILE UINT32 * v_regs;
    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_RAISE, 0, TRUE);
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);
    hal_GdbRegs[37]+=INC_STEP;

//  send command 
    hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGKILL, TRUE);
    
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_RAISE, 0, hal_GdbRegs);

    hal_GdbLoop(cpuid, v_regs, TRUE);
#endif
}


// =============================================================================
// hal_GdbIrqDebug
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbIrqDebug(UINT32 cpuid, UINT32 cause)
{
    VOLATILE UINT32 * hal_GdbRegs;
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

    hal_GdbRegs = hal_GdbPrepare(cpuid, error_code, 0, fatal);
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);

    if(!fatal)
    {
//  don't actually send command if the BCPU triggered the IT
        hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGINT, FALSE);
    }

    // xcpu check for bcpu error, if any trace it
#ifndef NO_BCPU_IMAGE
    if ((cpuid == 0) && (bcpu_error_code != HAL_DBG_ERROR_NO))
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
        hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
        hal_DbgPalFlushTrace ();
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

        hal_GdbBackTrace(hal_GdbRegs);

        GDB_CSW_FUNC(1, bcpu_error_code, bcpu_error_status, bcpu_sp_context);
    }
    else
#else // NO_BCPU_IMAGE
    {
#ifndef PAL_NO_DBGTRC
        hal_DbgPalFlushTrace ();
#endif
        HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, "XCPU debug IRQ");
        hal_GdbBackTrace(hal_GdbRegs);
    }
#endif // NO_BCPU_IMAGE

    hal_GdbLoop(cpuid, v_regs, fatal);
}

// =============================================================================
// hal_GdbIrqPage
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbIrqPage(UINT32 cpuid)
{
#ifdef CT_NO_DEBUG
     hal_SysRestart();
#else
    VOLATILE UINT32 * hal_GdbRegs;
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
        master = hal_DbgPageProtectGetHitMaster(page);
        address = hal_DbgPageProtectGetHitAddress(page);
        pagecode = 
         (address &0x01FFFFFF) | ((master&0xF)<<28) | (page<<26);
    }
    
    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_PAGE, pagecode, !hal_PageNoraise);


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

    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);

    hal_GdbSendcmd(cpuid, hal_GdbRegs, HAL_GDB_SIGKILL, TRUE);
    
    {
        HAL_PROFILE_PULSE(XCPU_ERROR);
        hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
        hal_DbgPalFlushTrace ();
#endif
        HAL_TRACE(_SXR|TABORT|TSTDOUT, 0, 
                "Write Protect exception: Page %d, master: %d, addr: %x \n\n"
                "PC %lxy\n"
                "SP %lxy\n",
                page,
                master,
                address,
                hal_GdbRegs[37],
                hal_GdbRegs[29]);
        hal_GdbBackTrace(hal_GdbRegs);
        GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_PAGE, pagecode, hal_GdbRegs);
    }
    
//  clear and reenable pages done in handle
    hal_GdbLoop(cpuid, v_regs, TRUE);
#endif
}

// =============================================================================
// hal_GdbException
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbException(UINT32 cpuid, UINT32 cause)
{
#ifdef CT_NO_DEBUG
     hal_SysRestart();
#else
    VOLATILE UINT32 * hal_GdbRegs;
    VOLATILE UINT32 * v_regs;
    UINT32 signal;
    hal_GdbRegs = hal_GdbPrepare(cpuid, HAL_DBG_ERROR_IT, cause, TRUE);

    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);

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
    hal_GdbSendcmd(cpuid, hal_GdbRegs, signal, TRUE);

    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#ifndef PAL_NO_DBGTRC
    hal_DbgPalFlushTrace ();
#endif
    HAL_TRACE(_SXR|TABORT|TSTDOUT|TSMAP(0x2), 0, 
            "XCPU EXCEPTION CODE %d, %s\n\n"
            "PC %lxy\n"
            "SP %lxy\n",
            ((cause>>2)&0x1f),
            excCodeName[((cause>>2) & 0x1f)],
            hal_GdbRegs[37],
            hal_GdbRegs[29]);

    hal_GdbBackTrace(hal_GdbRegs);
    GDB_CSW_FUNC(cpuid, HAL_DBG_ERROR_IT, cause, hal_GdbRegs);

    hal_GdbLoop(cpuid, v_regs, TRUE);
#endif
}



// =============================================================================
// hal_GdbHandle
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbHandle(UINT32 cpuid, VOLATILE UINT32 * regs)
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
            hal_GdbFlushCache();
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
            break;
        case HAL_GDB_SET_BREAKPOINT :            
            hal_GdbSetBreakpoint(cpuid,regs);
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
            break;
        case HAL_GDB_UNSET_BREAKPOINT :
            hal_GdbUnsetBreakpoint(cpuid,regs);
            regs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_DONE;
            hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);        
            break;
        case HAL_GDB_CMD_STEP:
            hal_GdbStep(cpuid, 0, regs);
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
        hal_GdbDebugHook();
        sxs_SerialFlush ();
#endif // HAL_GDB_SAFE_WITHOUT_SX
    }
}


// =============================================================================
// hal_GdbSetBreakpoint
// -----------------------------------------------------------------------------
///  Manage the setting of a breakpoint when asked by the GDB server. This basi-
///  cally means to squash the current opcode by the break opcode. The opcode
///  is memorized by the GDB server for it to be restored.
// =============================================================================
PROTECTED VOID hal_GdbSetBreakpoint(UINT32 cpuid, VOLATILE UINT32* regs)
{
    UINT32 addr = regs[HAL_GDB_CMDPARAM_OFF];

    BOOL    flashWriteEnable = 0;
    // Take special action if the instruction to change
    // is in flash or in 
    // TODO : Rom
    if (addr & REG_CS0_BASE)
    {
        // This address is in flash
        flashWriteEnable = hal_EbcFlashWriteEnabled();
        hal_EbcFlashWriteEnable(TRUE);
        if (!memd_FlashRomulatorSetRamMode(TRUE))
        {
            HAL_TRACE(HAL_WARN_TRC, 0, "GDB: Cannot set a breakpoint in Flash !");
            // TODO Find a way to warn GDB about that !
            hal_EbcFlashWriteEnable(flashWriteEnable);
            // Nothing can be done, we cannot write the new opcode
            // in flash.
            return;
        }
    }    

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
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

        // Enable patch
        hwp_memBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_ENABLE
                                    | (addr & MEM_BRIDGE_BLOCK_ADDR_MASK);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
        hwp_bbMemBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_ENABLE
                                    | (addr & MEM_BRIDGE_BLOCK_ADDR_MASK);
#endif
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
    if (addr & REG_CS0_BASE)
    {
        // Reprotect flash
        // This address was in flash
        memd_FlashRomulatorSetRamMode(FALSE);
        hal_EbcFlashWriteEnable(flashWriteEnable);
    }    

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#else
    // We need to keep the patch enabled until the breakpoint is removed.
#endif
}

// =============================================================================
// hal_GdbUnsetBreakpoint
// -----------------------------------------------------------------------------
///  Manage the unsetting of a breakpoint when asked by the GDB server. This basi-
///  cally means to squash the current break opcode by the true opcode. The opcode
///  was memorized by the GDB server for it to be restored.
// =============================================================================
PROTECTED VOID hal_GdbUnsetBreakpoint(UINT32 cpuid, VOLATILE UINT32* regs)
{
    UINT32 addr         = regs[HAL_GDB_CMDPARAM_OFF];
    // The opcode to be restored (memorized by the GDB server).
    UINT32 theOpCode    = regs[HAL_GDB_CMDPARAM2_OFF]; //Because it's the one

    BOOL    flashWriteEnable = 0;
    // Take special action if the instruction to change
    // is in flash or in 
    if (addr & REG_CS0_BASE)
    {
        // This address is in flash
        flashWriteEnable = hal_EbcFlashWriteEnabled();
        hal_EbcFlashWriteEnable(TRUE);
        if (!memd_FlashRomulatorSetRamMode(TRUE))
        {
            HAL_TRACE(TABORT|HAL_WARN_TRC, 0, "GDB: Cannot unset a breakpoint in Flash !");    
            // TODO Find a way to warn GDB about that ! 
            hal_EbcFlashWriteEnable(flashWriteEnable);  
            // Nothing can be done, we cannot write the new opcode 
            // in flash. 
            return;  
        }
    }    

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
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
    if (addr & REG_CS0_BASE)
    {
        // Reprotect flash
        // This address was in flash
        memd_FlashRomulatorSetRamMode(FALSE);
        hal_EbcFlashWriteEnable(flashWriteEnable);
    }    

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#else
    // Disabling the patch would restore the opcode again.
    // It would also remove any step in progress, that 
    // would anyhow have changed the patched area, restoring
    // de facto the code before the set breakpoint.
#endif
}




// =============================================================================
// hal_Gdb16Step
// -----------------------------------------------------------------------------
///  Uses a TRAP to generate an exception after we run the next instruction.
// =============================================================================
PROTECTED UINT32 hal_Gdb16Step (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * hal_GdbRegs)
{
    UINT32     nNewAddr;
    UINT16     *nOp=(UINT16*)MASK_ONE(hal_GdbRegs[37]);

    if     (HAL_GDB_16_OPCODE_JALX(nOp))     nNewAddr=HAL_GDB_16_OPADDR_JALX(nOp);
    else if(HAL_GDB_16_OPCODE_JAL(nOp))      nNewAddr=HAL_GDB_16_OPADDR_JAL(nOp);
    else if(HAL_GDB_16_OPCODE_JALR(nOp))     nNewAddr=HAL_GDB_16_OPADDR_JALR(nOp);
    else if(HAL_GDB_16_OPCODE_JR(nOp))       nNewAddr=HAL_GDB_16_OPADDR_JR(nOp);
    else if(HAL_GDB_16_OPCODE_B(nOp))        nNewAddr=HAL_GDB_16_OPADDR_B(nOp);
    else if(HAL_GDB_16_OPCODE_BEQZ(nOp))     nNewAddr=HAL_GDB_16_OPADDR_BEQZ(nOp);
    else if(HAL_GDB_16_OPCODE_BNEZ(nOp))     nNewAddr=HAL_GDB_16_OPADDR_BNEZ(nOp);
    else if(HAL_GDB_16_OPCODE_BTEQZ(nOp))    nNewAddr=HAL_GDB_16_OPADDR_BTEQZ(nOp);
    else if(HAL_GDB_16_OPCODE_BTNEZ(nOp))    nNewAddr=HAL_GDB_16_OPADDR_BTNEZ(nOp);
    else if(HAL_GDB_16_OPCODE_EXTENDED(nOp))
    {
            if     (HAL_GDB_16_OPCODE_B_EXT(nOp))        nNewAddr=HAL_GDB_16_OPADDR_B_EXT(nOp);
            else if(HAL_GDB_16_OPCODE_BEQZ_EXT(nOp))
            {
                nNewAddr=HAL_GDB_16_OPADDR_BEQZ_EXT(nOp);
            }
            else if(HAL_GDB_16_OPCODE_BNEZ_EXT(nOp))     nNewAddr=HAL_GDB_16_OPADDR_BNEZ_EXT(nOp);
            else if(HAL_GDB_16_OPCODE_BTEQZ_EXT(nOp))    nNewAddr=HAL_GDB_16_OPADDR_BTEQZ_EXT(nOp);
            else if(HAL_GDB_16_OPCODE_BTNEZ_EXT(nOp))    nNewAddr=HAL_GDB_16_OPADDR_BTNEZ_EXT(nOp);
//- V: Uh, what are we doing in there ? 
//- J, M & B: Don't know but add 4 instead of 2 because the instruction is extended to four bytes.
            else                                        nNewAddr=hal_GdbRegs[37]+4;
    }
    else    
    {
        nNewAddr=hal_GdbRegs[37]+2;
    }
        
    
    if(hal_GdbIsAddressStepForbidden(nNewAddr))
    {          
        if(HAL_GDB_16_OPCODE_JAL(nOp) 
            || HAL_GDB_16_OPCODE_JALX(nOp))     // Bypass the step-into function (think about the delay slot).
        {
            nNewAddr = hal_GdbRegs[37]+6;
        }
        else if(HAL_GDB_16_OPCODE_EXTENDED(nOp) // Bypass the step-to branch (extended, no delay slot).
            || HAL_GDB_16_OPCODE_JALR(nOp)      // (think about the delay slot). And the kittens.
            || HAL_GDB_16_OPCODE_JR(nOp))       // (think about the delay slot). And the kittens.
        {
            nNewAddr = hal_GdbRegs[37]+4;
        }
        else                                    // Should never happen.
        {
             nNewAddr = hal_GdbRegs[37]+2;
        }
    }
    
    return nNewAddr;
}


// =============================================================================
// hal_Gdb32Step
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED UINT32 hal_Gdb32Step (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * hal_GdbRegs)
{
    UINT32     nNewAddr;
    UINT32     nOp=*(UINT32*)hal_GdbRegs[37];
  
    if     (HAL_GDB_OPCODE_J(nOp))       
    {
        nNewAddr=HAL_GDB_OPADDR_J(nOp);
    }
    else if(HAL_GDB_OPCODE_JAL(nOp))      
    {     
        nNewAddr=HAL_GDB_OPADDR_JAL(nOp);
    }
    else if(HAL_GDB_OPCODE_JALX(nOp))      
    {    
        nNewAddr=HAL_GDB_OPADDR_JALX(nOp);
    }
    else if(HAL_GDB_OPCODE_JR(nOp))       
    {     
        nNewAddr=HAL_GDB_OPADDR_JR(nOp);
    }
    else if(HAL_GDB_OPCODE_JALR(nOp))      
    {    
        nNewAddr=HAL_GDB_OPADDR_JALR(nOp);
    }
    else if(HAL_GDB_OPCODE_BEQ(nOp))      
    {     
        nNewAddr=HAL_GDB_OPADDR_BEQ(nOp);
    }
    else if(HAL_GDB_OPCODE_BNE(nOp))      
    {    
        nNewAddr=HAL_GDB_OPADDR_BNE(nOp);
    }
    else if(HAL_GDB_OPCODE_BGEZ(nOp))       
    {   
        nNewAddr=HAL_GDB_OPADDR_BGEZ(nOp);
    }
    else if(HAL_GDB_OPCODE_BGEZAL(nOp))      
    {  
        nNewAddr=HAL_GDB_OPADDR_BGEZAL(nOp);
    }
    else if(HAL_GDB_OPCODE_BGTZ(nOp))      
    {    
        nNewAddr=HAL_GDB_OPADDR_BGTZ(nOp);
    }
    else if(HAL_GDB_OPCODE_BLEZ(nOp))      
    {    
        nNewAddr=HAL_GDB_OPADDR_BLEZ(nOp);
    }
    else if(HAL_GDB_OPCODE_BLTZ(nOp))       
    {   
        nNewAddr=HAL_GDB_OPADDR_BLTZ(nOp);
    }
    else if(HAL_GDB_OPCODE_BLTZAL(nOp))      
    {  
        nNewAddr=HAL_GDB_OPADDR_BLTZAL(nOp);
    }
    else      
    {   
        nNewAddr=hal_GdbRegs[37]+4;
    }

    if(hal_GdbIsAddressStepForbidden(nNewAddr))
    {
        if(TRUE) 
        {
            nNewAddr = hal_GdbRegs[37]+8;
        }
        else    //This should not happened, 'cause we should always be delay-sloted in such cases.
        {
            nNewAddr = hal_GdbRegs[37]+4;
        }
        
    }   
    
    return nNewAddr;
}

// =============================================================================
// hal_Gdb16EndStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_Gdb16EndStep (UINT32 cpuid, UINT32 nAddr)
{
    // Flag to prevent mips16 opcode to look like 0
    // and be discared improperly by hal_GdbUndoStep, which
    // test opCode against 0 to check if there is a step to
    // undo.
    //  save the target opcode, replace with HAL_GDB_16_OPCODE_STEP 
    hal_GdbGData[cpuid].opCode = *(UINT16*)MASK_ONE(nAddr) | HAL_GDB_MIPS16_OPCODE_FLAG;
    hal_GdbGData[cpuid].stepAddr = nAddr;
    *(UINT16*)MASK_ONE(nAddr) = HAL_GDB_16_OPCODE_STEP;
}

// =============================================================================
// hal_Gdb32EndStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_Gdb32EndStep (UINT32 cpuid, UINT32 nAddr)
{
    while(*(UINT32*)nAddr == 0)
    {
        nAddr+=4;
    }

//  save the target opcode, replace with HAL_GDB_OPCODE_STEP 
    hal_GdbGData[cpuid].opCode = *(UINT32*)nAddr;
    hal_GdbGData[cpuid].stepAddr = nAddr;
    *(UINT32*)nAddr = HAL_GDB_OPCODE_STEP;
}

// =============================================================================
// hal_GdbStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbStep (UINT32 cpuid, UINT32 nAddr, VOLATILE UINT32 * hal_GdbRegs)
{
    UINT32 nNewAddr;
    if (hal_GdbRegs[37] & 0x01)
        nNewAddr=hal_Gdb16Step(cpuid, nAddr, hal_GdbRegs);
    else
        nNewAddr=hal_Gdb32Step(cpuid, nAddr, hal_GdbRegs);

    // Take special action if the instruction to change
    // is in flash or in Rom

    BOOL flashWriteEnable = 0;
    if (nNewAddr & REG_CS0_BASE)
    {
        // This address is in flash
        flashWriteEnable = hal_EbcFlashWriteEnabled();
        hal_EbcFlashWriteEnable(TRUE);
        if (!memd_FlashRomulatorSetRamMode(TRUE))
        {
            HAL_TRACE(TABORT|HAL_WARN_TRC, 0, "GDB: Cannot step in Flash !");    
            // TODO Find a way to warn GDB about that ! 
            hal_EbcFlashWriteEnable(flashWriteEnable);  
            // Nothing can be done, we cannot write the new opcode 
            // in flash. 
            return;  
        }
    }    

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#endif
#else
    if ((nNewAddr & 0x0FF00000) == REG_INT_ROM_BASE)
    { 
        // This instruction is in internal ROM
        // We need to copy the whole line to patch
        // it. The line being patched become writtable
        UINT32 wordIdx = 0;

        // Copy Rom line into patch memory
        for (wordIdx = 0 ; wordIdx<4; wordIdx++)
        {
            *((UINT32*)(KSEG0(REG_INT_SRAM_BASE)) + 4*HAL_GDB_PATCH_NUMBER + wordIdx) = 
                            *((UINT32*)(KSEG0(((nNewAddr & MEM_BRIDGE_BLOCK_ADDR_MASK)| REG_INT_ROM_BASE))) + wordIdx);
        }

        // Enable patch
        hwp_memBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_ENABLE
                                    | (nNewAddr & MEM_BRIDGE_BLOCK_ADDR_MASK);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
        hwp_bbMemBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_ENABLE
                                    | (nNewAddr & MEM_BRIDGE_BLOCK_ADDR_MASK);
#endif
    }
#endif

    if (nNewAddr & 0x01)
        hal_Gdb16EndStep(cpuid, nNewAddr);
    else
        hal_Gdb32EndStep(cpuid, nNewAddr);

    hal_GdbFlushCache();

    // Reprotect flash
    if (nNewAddr & REG_CS0_BASE)
    {
        // This address was in flash
        memd_FlashRomulatorSetRamMode(FALSE);
        hal_EbcFlashWriteEnable(flashWriteEnable);
    }    

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#endif

}

// =============================================================================
// hal_GdbUndoStep
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbUndoStep (UINT32 cpuid, VOLATILE UINT32 * hal_GdbRegs)
{
    UINT32     nNewAddr = hal_GdbGData[cpuid].stepAddr;
    UINT32     isMips16 = (hal_GdbRegs[37] & 0x01);

//  if we're stepping, then undo the step 
    if (hal_GdbGData[cpuid].opCode )
    {
        BOOL flashWriteEnable = 0;
        if (nNewAddr & REG_CS0_BASE)
        {
            // This address is in flash
            flashWriteEnable = hal_EbcFlashWriteEnabled();
            hal_EbcFlashWriteEnable(TRUE);
            if (!memd_FlashRomulatorSetRamMode(TRUE))
            {
                HAL_TRACE(HAL_WARN_TRC, 0, "GDB: Cannot undo a step in Flash !");    
                // TODO Find a way to warn GDB about that ! 
                hal_EbcFlashWriteEnable(flashWriteEnable);  
                // Nothing can be done, we cannot write the new opcode 
                // in flash. 
                return;  
            }
        }    

#ifdef FPGA
    // Special Rom
    // Unlock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;
#endif
#else
    // The corresponding instruction is patchable since
    // the previous passage through hal_GdbStep.
#endif

        if(nNewAddr == hal_GdbRegs[37])
        {           
            if(!isMips16)
                    *(UINT32*)hal_GdbRegs[37] = hal_GdbGData[cpuid].opCode;
            else
                    *(UINT16*)MASK_ONE(hal_GdbRegs[37]) = (UINT16)hal_GdbGData[cpuid].opCode  & HAL_GDB_MIPS16_OPCODE_MASK;
        }
        else
        {
            if(!isMips16)   
                    *(UINT32*)nNewAddr=hal_GdbGData[cpuid].opCode;
            else
                    *(UINT16*)MASK_ONE(nNewAddr)=(UINT16)hal_GdbGData[cpuid].opCode & HAL_GDB_MIPS16_OPCODE_MASK;
        }
        
        hal_GdbFlushCache();

        // Take special action if the instruction to change
        // is in flash or in Rom     
        if (nNewAddr & REG_CS0_BASE)
        {
            // Reprotect flash
            // This address was in flash
            memd_FlashRomulatorSetRamMode(FALSE);
            hal_EbcFlashWriteEnable(flashWriteEnable);
        }    

#ifdef FPGA
    // Special Rom
    // Lock
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
    hwp_bbMemBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif
#else
        // If the nNewAddr is in Rom, disabling the patch is equivalent to
        // the previous behaviour
        if ((nNewAddr & 0x0FF00000) == REG_INT_ROM_BASE)
        {
            // The patch should be enabled
            HAL_ASSERT((hwp_memBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER]& MEM_BRIDGE_PATCH_ENABLE) == (UINT32)MEM_BRIDGE_PATCH_ENABLE,
                    "hal_GdbUndoStep on a ROM address without the patch enabled - svn blame that line and ask me");
            // Disable the patch set in hal_GdbStep.
            hwp_memBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_DISABLE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) || (CHIP_HAS_BCPU_ROM)
            // The patch should be enabled
            HAL_ASSERT((hwp_bbMemBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER]& MEM_BRIDGE_PATCH_ENABLE) == (UINT32)MEM_BRIDGE_PATCH_ENABLE,
                    "hal_GdbUndoStep on a ROM address without the patch enabled - svn blame that line and ask me");
            // Disable the patch set in hal_GdbStep.
            hwp_bbMemBridge->Rom_Patch[HAL_GDB_PATCH_NUMBER] = MEM_BRIDGE_PATCH_DISABLE;
#endif
        }        
#endif
        
        hal_GdbGData[cpuid].opCode = 0;
    }
}


// =============================================================================
// hal_GdbStrLen
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED UINT32 hal_GdbStrLen(UINT8* pcStr)
{
    UINT32 i=0;
    while((*pcStr++) != '\0') i++;
    return i;
}

// =============================================================================
// hal_GdbBackTrace
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_GdbBackTrace(VOLATILE UINT32 *hal_GdbRegs)
{
#ifndef HAL_GDB_SAFE_WITHOUT_SX
    UINT32 *Sp = (UINT32*)hal_GdbRegs[29];

    sxs_BackTrace(hal_GdbRegs[37], Sp);
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

// commented hal_Gdb_WatchDog_handler:
// used to fill a gdb context from the WD context, however this is not really accurate
// kept code here if needed
#if 0
// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================
VOID hal_GdbWatchDogHandler(VOID)
{
    const UINT32 cpuid = 0;
    // only handled on xcpu
    UINT32 hal_GdbRegs[HAL_GDB_REGNUM_XTD];
    VOLATILE UINT32 * v_regs;
    INT32 i;
    HAL_WATCHDOG_CTX_T* g_halWatchdogCtx=(HAL_WATCHDOG_CTX_T*) KSEG0(INT_SRAM_BASE);
    
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);

    // build a gdb context from the WDog context
    hal_GdbRegs[0]=0;
    for(i=1;i<26;i++)
     hal_GdbRegs[i] = ((UINT32*)(g_halWatchdogCtx))[i-1]; // copy r1 to r25
    hal_GdbRegs[26]=0; // k0
    for(i=27;i<32;i++)
     hal_GdbRegs[i] = ((UINT32*)(g_halWatchdogCtx))[i-2]; // copy r27 to r31
    for(i=32;i<37;i++)
        hal_GdbRegs[i]=0; // clear unavailable registers
    hal_GdbRegs[37]=g_halWatchdogCtx->pc | 0x80000001; // EPC assuming caches mips16

    // set the context
    xcpu_sp_context = hal_GdbRegs;
    
    hwp_reg->reg_dbg = HAL_DBG_ERROR_GDB;
    xcpu_error_code = HAL_DBG_ERROR_WDOG;
    xcpu_error_status = 0;
    v_regs=(UINT32*)KSEG1((UINT32)hal_GdbRegs);

//  prepare command 
    hal_GdbRegs[HAL_GDB_CMD_OFF] = HAL_GDB_CMD_BREAKPOINT;
    hal_GdbRegs[HAL_GDB_CMDPARAM_OFF] = HAL_GDB_SIGKILL;
//  trigger command 
    hal_DbgGdbHstSendEvent(HAL_GDB_XCPU_EVENT);
    
//  wait for the debug server to complete 
    while(v_regs[HAL_GDB_CMD_OFF]!=HAL_GDB_CMD_DONE);
    
    hal_GdbUndoStep(cpuid, v_regs);
    hal_GdbHandle(cpuid, v_regs);
    HAL_PROFILE_PULSE(XCPU_ERROR);
    // FIXME hal_DbgExlClrTrigger(HAL_DBG_EXL_FATAL);
}
#endif

// =============================================================================
// hal_DbgErrorHandlerSet
// -----------------------------------------------------------------------------
/// Set the error handler from the upper layers.
/// @param handler Handler called when an error occurs on one of the CPU.
// =============================================================================
PUBLIC VOID hal_DbgErrorHandlerSet(HAL_DBG_ERROR_HANDLER_T handler)
{
    g_halErrorRegistry = handler;
}

#ifndef HAL_GDB_SAFE_WITHOUT_SX
// =============================================================================
// hal_GdbSwitchContext
// -----------------------------------------------------------------------------
/// Switch the task context after GDB occurs.
/// @param id The task or job ID that will be switched to.
/// @param isJob TRUE if switching to a job context.
// =============================================================================
PRIVATE VOID hal_GdbSwitchContext(UINT32 id, UINT32 isJob)
{
    if (g_halGdbRegsBackup == NULL)
    {
        g_halGdbRegsBackup = xcpu_sp_context;
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
        xcpu_sp_context = g_halGdbRegsBackup;
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
    xcpu_sp_context = g_halGdbTempRegs;
    g_halGdbSwitchCtx.cmd = HAL_GDB_SWITCH_CTX_CMD_OK;
}

PRIVATE VOID hal_GdbCheckSwitchContextCmd(VOID)
{
    if (g_halGdbSwitchCtx.cmd == HAL_GDB_SWITCH_CTX_CMD_START)
    {
        hal_GdbSwitchContext(g_halGdbSwitchCtx.id, g_halGdbSwitchCtx.isJob);
    }
}

PRIVATE VOID hal_GdbDebugHook(VOID)
{
    for (int i=0; i<HAL_GDB_MAX_DEBUG_HOOK_FUNC_NUM; i++)
    {
        if (g_halGdbDebugHookFuncPtr[i] != NULL)
        {
            (*g_halGdbDebugHookFuncPtr[i])();
        }
    }
}

PUBLIC BOOL hal_GdbRegisterDebugHookFunc(void (*funcPtr)(void))
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

