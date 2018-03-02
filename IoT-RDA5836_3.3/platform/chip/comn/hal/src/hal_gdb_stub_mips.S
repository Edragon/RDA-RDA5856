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
///     @file hal_gbd_stub_mips.S
///     GDB assembly functions for the GDB stub, 
///     about cache flushing
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "regdef.h"
.text

.globl hal_GdbFlushCache
.ent   hal_GdbFlushCache
.frame sp,0,ra
hal_GdbFlushCache:
    .set noreorder
    .align 4
    cache 0,0
    nop
    nop
    nop
    jr ra
    nop
    .set reorder
.end hal_GdbFlushCache

.globl hal_GdbFlushCacheData
.ent   hal_GdbFlushCacheData
.frame sp,0,ra
hal_GdbFlushCacheData:
    .set noreorder
    .align 4
    cache 2,0
    nop
    nop
    nop
    jr ra
    nop
    .set reorder
.end hal_GdbFlushCacheData
