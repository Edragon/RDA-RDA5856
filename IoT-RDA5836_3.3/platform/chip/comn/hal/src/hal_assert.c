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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_assert.c
///     Provide services for driver internal tests
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "string.h"
#include <stdarg.h>
#include "stdio.h"

#include "hal_host.h"
#include "hal_sys.h"

#include "halp_debug.h"
#include "boot.h"

#include "sxs_io.h"


#ifdef ASICSIMU
#include "asicsimu_m.h"
#endif
#include "cos.h"
#if(OS_KERNEL==FREERTOS)
#include "freertos_io.h"
#endif

#define HAL_ASSERT_ERROR_FIFO_SIZE       32
#define HAL_ASSERT_CODE              0xa55E
#define HAL_ASSERT_DELAY             100000
#define HAL_ASSERT_LOOP_QTY          20


// =============================================================================
// hal_DbgAssert
// -----------------------------------------------------------------------------
/// Function to call when 'building' an assert, if the condition is false.
/// @param format String format a-la printf, displayed in the trace.
// =============================================================================
PUBLIC VOID hal_DbgAssert(CONST CHAR* format, ...)
{
#if defined(TFLASH_UPGRADE_SUPPORT) || defined(CHIP_CPU_IS_ARM)
    hal_HstSendEvent(0xff,0xA55E47);
    while (1);
#else
    va_list args;
    CHAR msg[256];

    hal_TimWatchDogClose();    
    va_start(args, format);
    vsprintf(msg, format, args);

#ifdef ASICSIMU
    asicsimu_Fatal((format==NULL)? "FATAL assert":(CHAR *)msg);
#endif

    hal_HstSendEvent(0xff,0xA55E47);
    // We never go out of this function, so we can safely assume
    // it is possible to read it from coolwatcher.
    hal_HstSendEvent(0xff,(UINT32)msg);
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);
#if OS_KERNEL == SXR
    xcpu_error_code = HAL_ASSERT_CODE;
    xcpu_error_status = 0;
#endif
    
    //  Print string to display 
    // TSMAP is the bitfield of parameters which are string
    // sxs_Raise is a no-exit function    
    UINT32 raiseId = _SXR|TSMAP(0x1)|TABORT;
#if OS_KERNEL==SXR
    sxs_Raise(raiseId,
          "\n##############################################\n"
          "# FATAL assert (Use GDB to debug!)\n"
          "#    --> %s\n"
          "##############################################",
          (format==NULL? "/no string/":(CHAR *)msg));
#elif (OS_KERNEL == FREERTOS)

#ifndef COMPILE_ERROR_API
    raise(raiseId,
    "\n##############################################\n"
    "# FATAL assert (Use GDB to debug!)\n"
    "#    --> %s\n"
    "##############################################",
    (format==NULL? "/no string/":(CHAR *)msg));
#endif

#endif

//  ...and at last end loop
    while (1);

    va_end(args);
#endif
}


