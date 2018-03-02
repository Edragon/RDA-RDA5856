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
//
/// @file dbg_assert.c
/// Implement the assert.
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "dbg.h"
#include "stdarg.h"
#include "hal_debug.h"

// =============================================================================
// dbg_AssertUtil
// -----------------------------------------------------------------------------
/// Utilitary function to define the assert.
/// This function is used in the public macro that implements the minimal side-
/// effect assert.
/// @param format String format a-la printf, displayed in the trace.
// =============================================================================
PUBLIC VOID dbg_AssertUtil(CONST CHAR* format, ...)
{
    va_list args;
    va_start(args, format);
    // Just a HAL wrapper.
    hal_DbgAssert(format, args);
    va_end(args);
}
