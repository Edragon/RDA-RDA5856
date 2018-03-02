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
/// @file sxp_mutx.h
/// Systeme executif task mutual exclusion private header file.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __SXP_MUTX_H__
#define __SXP_MUTX_H__

#include "sxs_type.h"
#include "sxr_cnf.h"


#define SXR_NO_MUTEX        0xFF
#define SXR_MUTEX_ALLOCATED 0xFE

typedef struct
{
    u8 SemaId;
    u8 TaskId;
    u8 Count;
    /// chaining for allocation
    u8 Next;
    /// debug info
    u32 CallerAdd;
} Mutex_t;


#ifdef __SXR_MUTX_VAR__
#define DefExtern
#else
#define DefExtern extern
#endif
DefExtern struct
{
    //Mutex_t    Queue    [SXR_NB_MAX_MUTEX];
    Mutex_t    *Queue;
    u8         IdxFree;
    u8         Nb;
} sxr_Mutex;

#undef DefExtern

#ifdef SX_NO_ASSERT
#define SX_MUTX_ASSERT(BOOL, format, ...)
#else
#define SX_MUTX_ASSERT(BOOL, format, ...) \
    if (!(BOOL)) { \
        sxr_CheckMutex(); \
        hal_DbgAssert(format, ##__VA_ARGS__); \
    }
#endif

#endif

