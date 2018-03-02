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
/*
================================================================================
  File         sxr_tim.h
--------------------------------------------------------------------------------

  Scope      : Time management header file.

  History    :
--------------------------------------------------------------------------------
 Jun 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __SXR_TIM_H__
#define __SXR_TIM_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "sxs_type.h"
#include "sxr_cnf.h"

#define SXR_NO_DUE_DATE         0xFFFFFFFF
#define SXR_NO_MSK              0xFFFFFFFF

#define SXR_TIMER_USR_ID_POS    0
#define SXR_TIMER_MBX_USR_ID_MSK ((u16)0xFFL << SXR_TIMER_USR_ID_POS) /* Up to 256 user ids   */
#define SXR_TIMER_MBX_POS       8
#ifdef GSM_SUPPORT
#define SXR_TIMER_MBX_MSK       ((u16)0x3FL << SXR_TIMER_MBX_POS)    /* Up to 64 mail boxes. */
#else
#define SXR_TIMER_MBX_MSK       ((u16)0x07L << SXR_TIMER_MBX_POS)    /* Up to 8 mail boxes. */
#endif
#define SXR_FUNCTION_TIMER_POS  14
#define SXR_FUNCTION_TIMER      ((u16)1 << SXR_FUNCTION_TIMER_POS)
#define SXR_MBX_TIMER_POS       15
#define SXR_MBX_TIMER           ((u16)1 << SXR_MBX_TIMER_POS)

#define SXR_TIMER_USR_ID_MSK    ~((u16)(SXR_FUNCTION_TIMER | SXR_MBX_TIMER))

#define SXR_GEN_MBX_TIMER_ID(UsrId, Mbx)                   \
        (u16)( SXR_MBX_TIMER                               \
        | ((Mbx << SXR_TIMER_MBX_POS) & SXR_TIMER_MBX_MSK) \
        | ((UsrId  << SXR_TIMER_USR_ID_POS) & SXR_TIMER_MBX_USR_ID_MSK))

#define SXR_GET_TIMER_MBX(Ctx) (u8)((Ctx & SXR_TIMER_MBX_MSK) >> SXR_TIMER_MBX_POS)

#define SXR_TIMER_PARAM_IDX     0
#define SXR_TIMER_FUNCTION_IDX  1

#define SXR_GEN_FUNCTION_TIMER_CTX(Ctx, Function, Param)  \
        Ctx [SXR_TIMER_PARAM_IDX]    = (u32)Param;        \
        Ctx [SXR_TIMER_FUNCTION_IDX] = (u32)Function;

typedef struct
{
    u32 Ctx;
    u32 Msk;
    u16 Id;
    u8  Idx;
} sxr_Timer_t;

typedef struct
{
    u32 Ctx;
    u32 Msk;
    u8  Idx;
    u16 Id;
    u32 *Tab [SXR_TIMER_LIST_SIZE];
    u16 NbTimerMax;
} sxr_TimerList_t;

void sxr_InitTimer (void (*TimerDelivery)(u32 *),
                    void (*SetCntValue)(u32),
                    u32  (*GetCntValue)(void),
                    u8   I2d);

void sxr_SetTimer    (u32 Period, u32 *Ctx, u16 Id, u8 I2d);
u32  sxr_DeleteTimer (sxr_Timer_t *Timer, u32 *Ctx, u8 DelAll, u8 I2d);
u32  sxr_DeleteComplexTimer (sxr_Timer_t *Timer, u32 *Ctx, u8 DelAll, void (*Free)(u16, u32 *), u8 I2d);
void sxr_Tampax      (u32 NbTimePeriod, u8 I2d);
u8   sxr_IsTimerSet  (sxr_Timer_t *Timer, u8 I2d);
u8   sxr_TimerList   (sxr_Timer_t *Timer, u32 *Tab, u8 Max, u8 I2d);
u32  sxr_NextTimerQuery (u16 Mask, u8 I2d);
u32  sxr_TimerQuery  (sxr_Timer_t *Timer, u8 I2d);
sxr_Timer_t *sxr_PeriodToTimer (sxr_Timer_t *Timer, u32 Period, u8 I2d);

// =============================================================================
// _sxr_StartMbxTimer
// -----------------------------------------------------------------------------
/// Start a timer such that after required period, the event
/// given in parameter is sent in the required mail box.
///
/// @param period   Period to wait,
/// @param evt      Event to be sent.
/// @param mbx      Mail box number
/// @param id       Timer Id,
/// @param i2d      Timer instance. (SXR_REGULAR_TIMER or SXR_FRAMED_TIMER)
// =============================================================================
void _sxr_StartMbxTimer (u32 Period, u32 *Evt, u8 Mbx, u16 Id, u8 I2d);

// =============================================================================
// _sxr_StopMbxTimer
// -----------------------------------------------------------------------------
/// Stop the identified timer, identified by the mailbox id and the timer id.
///
/// @param evt      Event to be sent.
/// @param mbx      Mail box number
/// @param id       Timer Id,
/// @param i2d      Timer instance. (SXR_REGULAR_TIMER or SXR_FRAMED_TIMER)
/// @return         \c TRUE when timer found, \c FALSE otherwise.
// =============================================================================
u8 _sxr_StopMbxTimer (u32 *evt, u8 mbx, u16 id, u8 i2d);


void _sxr_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d);
u32  _sxr_StopFunctionTimer (void (*Function)(void *), u8 I2d);
u32 _sxr_StopFunctionTimer2 (void (*Function)(void *),void *Param, u8 I2d);

u8   sxr_CheckTimer (void);
void sxr_PrintTimer (sxr_Timer_t *Timer, u8 I2d);


/* -----------------  User friendly Timer interface  --------------------
| Reminder:
| - Function timer: the given function is called with specified parameter
|   after required period.
| - Mbx timer: the given event is sent in the given mail box after required
|   period.
| - User timer: Timer delivery function is called with the given context
|   as parameter after required period.
|
| - Framed timer correspond to a high priority timer instance and are reserved
|   for lower layer (NOT RR !).
|
*/

#define sxr_StartFunctionTimer(Period, Function, Param, Id) \
 _sxr_StartFunctionTimer ((u32)Period, (void (*)(void *))Function, (void *)Param, (u16)Id, SXR_REGULAR_TIMER)

#define sxr_StopFunctionTimer(Function) \
 _sxr_StopFunctionTimer ((void (*)(void *))Function, SXR_REGULAR_TIMER)

#define sxr_StopFunctionTimer2(Function,Param) \
 _sxr_StopFunctionTimer2 ((void (*)(void *))Function, Param,SXR_REGULAR_TIMER)

// =============================================================================
// sxr_StartMbxTimer
// -----------------------------------------------------------------------------
/// Start a timer such that after required period, the event
/// given in parameter is sent in the required mail box.
///
/// @param period   Period to wait,
/// @param evt      Event to be sent.
/// @param mbx      Mail box number
/// @param id       Timer Id,
// =============================================================================
#define sxr_StartMbxTimer(Period, Evt, Mbx, Id) \
 _sxr_StartMbxTimer ((u32)Period, (u32 *)Evt, (u8)Mbx, (u16)Id, SXR_REGULAR_TIMER)

// =============================================================================
// sxr_StopMbxTimer
// -----------------------------------------------------------------------------
/// Stop the identified timer, identified by the mailbox id and the timer id.
///
/// @param evt      Event to be sent.
/// @param mbx      Mail box number
/// @param id       Timer Id,
/// @return         \c TRUE when timer found, \c FALSE otherwise.
// =============================================================================
#define sxr_StopMbxTimer(Evt, Mbx, Id) \
 _sxr_StopMbxTimer ((u32 *)Evt, (u8)Mbx, (u16)Id, SXR_REGULAR_TIMER)

#define sxr_StartUserTimer(Period, Ctx, Id) \
 sxr_SetTimer ((u32)Period, (u32 *)Ctx, (u16)Id & SXR_TIMER_USR_ID_MSK, SXR_REGULAR_TIMER)


#define sxr_StartFunctionFramedTimer(Period, Function, Param, Id) \
 _sxr_StartFunctionTimer ((u32)Period, (void (*)(void *))Function, (void *)Param, (u16)Id, SXR_FRAMED_TIMER)

#define sxr_StopFunctionFramedTimer(Function) \
 _sxr_StopFunctionTimer (Function, SXR_REGULAR_TIMER)


#define sxr_StartUserFramedTimer(Period, Ctx, Id) \
 sxr_SetTimer ((u32)Period, (u32 *)Ctx, (u16)Id & SXR_TIMER_USR_ID_MSK, SXR_FRAMED_TIMER)


void sxs_StartTimer   (u32 Period, u32 Id, void *Data, u8 ReStart, u8 Mbx);
u8   sxs_StopTimer    (u32 Id, void *Data, u8 Mbx);
u8   sxs_StopAllTimer (u32 Id, void *Data, u8 Mbx);
u32  sxs_ExistTimer   (u32 Id);

#ifdef __cplusplus
}
#endif

#endif

