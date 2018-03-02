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

#ifndef _IOT_BASE_H_
#define _IOT_BASE_H_

#include "cs_types.h"
#include "cos.h"


// ============================================================================
// sys_Malloc
// ----------------------------------------------------------------------------
/// Malloc specified size memory  
/// 
/// @param size memory size 
// ============================================================================
PVOID iot_Malloc(UINT32 size);

// ============================================================================
// sys_Free 
// ----------------------------------------------------------------------------
/// Free memory  
///
/// @param ptr Memory pointer 
// ============================================================================
BOOL iot_Free(PVOID ptr);

// ============================================================================
// app_Trace
// ----------------------------------------------------------------------------
/// Print trace to serial console,  for simple iot application, the levle is 
/// always APP_MAIN_TRC
/// 
/// @param levle always equal to APP_MAIN_TRC  
/// @param fmt like C language printf format 
// ============================================================================
VOID iot_Printf(INT8* fmt,...);

// ============================================================================
// sys_delayMs
// ----------------------------------------------------------------------------
/// Make the application sleep ms millionseconds. 
/// 
/// @param ms The unit is millionseconds  
// ============================================================================
VOID iot_DelayMs(UINT32 ms);

// ============================================================================
// hal_EbcCsClose
// ----------------------------------------------------------------------------
/// Make the application sleep seconds 
/// 
/// @param seconds The unit is second 
// ============================================================================
VOID iot_DelaySeconds( UINT32 seconds);

#endif

