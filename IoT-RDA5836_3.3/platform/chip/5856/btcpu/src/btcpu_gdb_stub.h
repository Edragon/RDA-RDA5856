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
/// @file hal_gdb_stub.h
/// Defines used by GDB stub. WARNING: this file is included by an assembly
/// file.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_GDB_STUB_H_
#define  _HALP_GDB_STUB_H_


#define HAL_GDB_REGNUM           38
//  all register + command and parameter for command 
#define HAL_GDB_REGNUM_XTD       (HAL_GDB_REGNUM+3)
#define HAL_GDB_CONTEXT_SIZE     (HAL_GDB_REGNUM_XTD*4)

#define HAL_GDB_CMD_OFF          HAL_GDB_REGNUM
#define HAL_GDB_CMDPARAM_OFF     (HAL_GDB_REGNUM + 1)
#define HAL_GDB_CMDPARAM2_OFF    (HAL_GDB_REGNUM + 2)

//  command between jade cpus and debug server 
#define HAL_GDB_CMD_DONE         0x00

#define HAL_GDB_CMD_FLUSH_CACHE     0x01
#define HAL_GDB_CMD_STEP            0x02
#define HAL_GDB_CMD_CONTINUE        0x04
#define HAL_GDB_SET_BREAKPOINT      0x05
#define HAL_GDB_UNSET_BREAKPOINT    0x06



#define HAL_GDB_CMD_BREAKPOINT   0x10
#define HAL_GDB_CMD_PRINT        0x20


#ifndef CT_ASM

/// Function used to flush both caches.
PUBLIC VOID btcpu_GdbFlushCache(VOID);

/// Function used to flush the data cache.
PUBLIC VOID btcpu_GdbFlushCacheData(VOID);
PROTECTED VOID btcpu_DbgGdbHstSendEvent(UINT32 ch);

PROTECTED BOOL btcpu_GdbIsInGdbLoop(VOID);
#endif // !CT_ASM


#endif //  _HALP_GDB_STUB_H_ 
