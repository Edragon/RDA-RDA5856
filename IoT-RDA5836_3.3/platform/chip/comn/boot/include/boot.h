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



#ifndef _BOOT_H_
#define _BOOT_H_




// =============================================================================
//  MACROS
// =============================================================================

/// Macro used to describe the time, in second, after which
/// the monitor is exited to continue on a valid flash code
/// (If any is present) since the latest command was received.
#define BOOT_IDLE_TIMEOUT    (30)



// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// BOOT_MONITOR_OP_STATUS_T
// -----------------------------------------------------------------------------
/// Describes the return status of a monitor (Host, UART, or USB).
// =============================================================================
typedef enum
{
    /// The monitor did no operation.
    BOOT_MONITOR_OP_STATUS_NONE = 0,
    
    /// The monitor executed an operation, and requires to stay
    /// in the monitoring loop.
    BOOT_MONITOR_OP_STATUS_CONTINUE = (1<<1),

    /// The monitor requested to exit the monitoring loop, and carryon
    /// execution.
    BOOT_MONITOR_OP_STATUS_EXIT = (1<<2),

    /// An error occured: Exit to.
    BOOT_MONITOR_OP_STATUS_EXIT_ON_ERROR = (1<<3)|(1<<2),

} BOOT_MONITOR_OP_STATUS_T;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// g_bootResetCause
// -----------------------------------------------------------------------------
/// Variable holding the reset cause value as read in the reset cause
/// register.
// =============================================================================
EXPORT UINT32 g_bootResetCause;

// =============================================================================
// g_bootBootMode
// -----------------------------------------------------------------------------
/// Variable holding the boot mode value as read in the boot mode
/// register.
// =============================================================================
EXPORT UINT32 g_bootBootMode;


// =============================================================================
// g_fastEntry
// -----------------------------------------------------------------------------
/// Variable holding the boot mode value as read in the boot mode
/// register.
// =============================================================================
EXPORT UINT32 g_fastEntry;

EXPORT UINT32 g_savecontextAddr;
// =============================================================================
// xcpu_error_code
// -----------------------------------------------------------------------------
/// Global variable holding the xcpu error code.
// =============================================================================
EXPORT volatile UINT32 xcpu_error_code;


// =============================================================================
// xcpu_error_status
// -----------------------------------------------------------------------------
/// Global variable holding the xcpu error status.
// =============================================================================
EXPORT volatile UINT32 xcpu_error_status;

// =============================================================================
// xcpu_sp_context
// -----------------------------------------------------------------------------
/// Hold a pointer to the GDB stub, when in GDB.
// =============================================================================
EXPORT volatile UINT32* xcpu_sp_context;

// =============================================================================
// xcpu_stack_base
// -----------------------------------------------------------------------------
/// unused.
// =============================================================================
EXPORT volatile UINT32 xcpu_stack_base;


// =============================================================================
// xcpu_main_entry
// -----------------------------------------------------------------------------
/// unused.
// =============================================================================
EXPORT volatile UINT32 xcpu_main_entry;


// =============================================================================
// sxr_IrqStack_pointer
// -----------------------------------------------------------------------------
/// Initial stack pointer for the XCPU
// =============================================================================
EXPORT volatile UINT32 sxr_IrqStack_pointer;

// =============================================================================
// bcpu_main_entry
// -----------------------------------------------------------------------------
/// Pointer to the first function lauched by the bcpu.
// =============================================================================
EXPORT UINT32* bcpu_main_entry;



// =============================================================================
// bcpu_sp_context
// -----------------------------------------------------------------------------
/// Variable holding the pointer where to save the BCPU context
// =============================================================================
EXPORT UINT32* bcpu_sp_context;

// =============================================================================
// bcpu_error_code
// -----------------------------------------------------------------------------
/// Global variable holding the bcpu error code.
// =============================================================================
EXPORT volatile UINT32 bcpu_error_code;


// =============================================================================
// bcpu_error_status
// -----------------------------------------------------------------------------
/// Global variable holding the bcpu error status.
// =============================================================================
EXPORT volatile UINT32 bcpu_error_status;

// =============================================================================
// bcpu_stack_base
// -----------------------------------------------------------------------------
/// Variable holding the stack pointer for the BCPU. This variable must
/// be set before the BCPU is started.
// =============================================================================
EXPORT UINT32 bcpu_stack_base;





// =============================================================================
//  FUNCTIONS
// =============================================================================



// =============================================================================
// boot_FlushCache
// -----------------------------------------------------------------------------
/// Flush both caches.
// =============================================================================
PUBLIC VOID boot_FlushCache(VOID);

// =============================================================================
// boot_DbgExlTrigFunction0
// -----------------------------------------------------------------------------
/// This function just jumps ra at a known address (0x80e00200).
// =============================================================================
PUBLIC VOID boot_DbgExlTrigFunction0(VOID);

// =============================================================================
// boot_GdbFlushCacheData
// -----------------------------------------------------------------------------
/// Flush the xcpu data cache (and only the data cache).
// =============================================================================
PUBLIC VOID boot_GdbFlushCacheData(VOID);



#endif // _BOOT_H_

