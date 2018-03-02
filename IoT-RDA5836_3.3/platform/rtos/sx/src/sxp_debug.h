/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file sxp_debug.h
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _SXP_DEBUG_H_
#define _SXP_DEBUG_H_

#include "cs_types.h"
#include "hal_debug.h"

#include "sx_profile_codes.h"




#ifdef SX_PRINTF
#define SX_TRACE(format, ...)                                    \
    hal_DbgTrace(TSTDOUT, 0, format, ##__VA_ARGS__)
#else
#define SX_TRACE(fmt, ...)
#endif


#ifdef SX_NO_ASSERT
#define SX_ASSERT(BOOL, format, ...)
#else
#define SX_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif






#ifdef SX_PROFILING


// =============================================================================
//  SX_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define SX_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SX, (CP_ ## eventName))

// =============================================================================
//  SX_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define SX_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SX, (CP_ ## eventName))

// =============================================================================
//  SX_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define SX_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SX, (CP_ ## pulseName))


// decimal value
#define CPJOBSPACEMASK 0x1f


#define SX_PROFILE_JOB_START_ENTER(id) \
    { \
        UINT16 prfJobId = sxr_Job -> Ctx [(id)].Desc -> Id; \
        if (prfJobId > CPJOBSPACEMASK) prfJobId = CPJOBSPACEMASK; \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SX, CP_sxr_StartJob_0 + prfJobId); \
    }

#define SX_PROFILE_JOB_START_EXIT(id) \
    { \
        UINT16 prfJobId = sxr_Job -> Ctx [(id)].Desc -> Id; \
        if (prfJobId > CPJOBSPACEMASK) prfJobId = CPJOBSPACEMASK; \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SX, CP_sxr_StartJob_0 + prfJobId); \
    }

#define SX_PROFILE_JOB_STOP_ENTER(id) \
    { \
        UINT16 prfJobId = sxr_Job -> Ctx [(id)].Desc -> Id; \
        if (prfJobId > CPJOBSPACEMASK) prfJobId = CPJOBSPACEMASK; \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SX, CP_sxr_StopJob_0 + prfJobId); \
    }

#define SX_PROFILE_JOB_STOP_EXIT(id) \
    { \
        UINT16 prfJobId = sxr_Job -> Ctx [(id)].Desc -> Id; \
        if (prfJobId > CPJOBSPACEMASK) prfJobId = CPJOBSPACEMASK; \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SX, CP_sxr_StopJob_0 + prfJobId); \
    }

#define SX_PROFILE_JOB_ENTER(id) \
    if ((id) != SXR_NIL_JOB)\
    { \
        hal_DbgPxtsProfileJobEnter(HAL_DBG_PXTS_SX, sxr_Job -> Ctx [id].Desc -> Id);\
    }

#define SX_PROFILE_JOB_EXIT(id) \
    if ((id) != SXR_NIL_JOB)\
    { \
        hal_DbgPxtsProfileJobExit(HAL_DBG_PXTS_SX, sxr_Job -> Ctx [id].Desc -> Id);\
    }

#define SX_PRFOILE_ENV_ENABLE       (1<<0)
#define SX_PROFILE_QUEUE_ENABLE     (1<<1)
extern u32 sxr_ProfileCfg;

// Profile the usage of an envelop
#define SX_PROFILE_ENVELOP_USE_START(id)\
    if (sxr_ProfileCfg & SX_PRFOILE_ENV_ENABLE) \
    {\
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SX, CP_SX_ENVELOP_USE_0 + id);\
    }

// Profile the usage of an envelop
#define SX_PROFILE_ENVELOP_USE_END(id)\
    if (sxr_ProfileCfg & SX_PRFOILE_ENV_ENABLE) \
    {\
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SX, CP_SX_ENVELOP_USE_0 + id);\
    }

// Profile the queue when waiting a msg
#define SX_PROFILE_WAIT_QUEUE_START(id)\
    if (sxr_ProfileCfg & SX_PROFILE_QUEUE_ENABLE) \
    {\
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SX, CP_SX_WAIT_QUEUE_0 + id);\
    }

// Profile the queue when waiting a msg
#define SX_PROFILE_WAIT_QUEUE_END(id)\
    if (sxr_ProfileCfg & SX_PROFILE_QUEUE_ENABLE) \
    {\
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SX, CP_SX_WAIT_QUEUE_0 + id);\
    }

#else // NO SX_PROFILING

#define SX_PROFILE_FUNCTION_ENTER(eventName)
#define SX_PROFILE_FUNCTION_EXIT(eventName)
#define SX_PROFILE_PULSE(pulseName)
#define SX_PROFILE_JOB_START_ENTER(id)
#define SX_PROFILE_JOB_START_EXIT(id)
#define SX_PROFILE_JOB_STOP_ENTER(id)
#define SX_PROFILE_JOB_STOP_EXIT(id)
#define SX_PROFILE_JOB_ENTER(id)
#define SX_PROFILE_JOB_EXIT(id)
#define SX_PROFILE_ENVELOP_USE_START(id)
#define SX_PROFILE_ENVELOP_USE_END(id)
#define SX_PROFILE_WAIT_QUEUE_START(id)
#define SX_PROFILE_WAIT_QUEUE_END(id)

#endif // SX_PROFILING

typedef struct
{
    u8 InUse;
    u8 SemaId;
    u8 Pad;
    u8 CallerTask;
    u32 CallerAddr;
} SemaInfo_t;

extern SemaInfo_t *sxr_SemaInfo;

#endif // _SXP_DEBUG_H_

