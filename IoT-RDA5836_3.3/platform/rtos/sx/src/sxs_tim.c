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
  File         sxs_tim.c
--------------------------------------------------------------------------------

  Scope      : Time management.

  History    :
--------------------------------------------------------------------------------
  Jun 13 2003   |  ADA  | Creation
================================================================================
*/

#define __SXR_TIM_VAR__
#include "sxr_tim.hp"
#undef  __SXR_TIM_VAR__

#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxs_lib.h"

#include "tgt_m.h"

//u32 *CtxTmp;
/*
===============================================================================
   Function   : sxr_InitTimer
 -----------------------------------------------------------------------------
   Scope      : Timer initialization.
   Parameters : pointer onto delivery function to be called at timer due date,
                pointer onto next timer notification function (optional) to notify
                the period of the next timer to come.
                pointer onto a fuction which allows to get the physical value of
                the hardware timer when it exists (optional).
   Return     : none.
==============================================================================
*/
void sxr_InitTimer (void (*TimerDelivery)(u32 *),
                    void (*SetCntValue)(u32),
                    u32  (*GetCntValue)(void),
                    u8  I2d)
{
    static u8 InitDone = FALSE;

    if (I2d >= SXR_TIMER_NB_INSTANCE)
    {
        SXS_RAISE ((_SXR|TABORT|TDB,TSTR("Too many instances\n",0x06c20001)));
    }

    if (InitDone == FALSE)
    {
        u16 i;
        
        //CtxTmp = sxr_HMalloc(SXR_TIMER_CTX_SIZE*sizeof(u32));// To be used in sxr_tim.c
        //memset(CtxTmp, 0, SXR_TIMER_CTX_SIZE*sizeof(u32));

        sxr_TCtx = sxr_HMalloc(SXR_TIMER_NB_INSTANCE*sizeof(TimerCtx_t));
        memset(sxr_TCtx, 0, SXR_TIMER_NB_INSTANCE*sizeof(TimerCtx_t));

        sxr_TEnv.TimerEnv = sxr_HMalloc(SXR_NB_MAX_TIMER_ENVELOPPE*sizeof(Timer_t));

        for (i=0; i<SXR_NB_MAX_TIMER_ENVELOPPE; i++)
        {
            sxr_TEnv.TimerEnv [i].PeriodToNext = 0;
            sxr_TEnv.TimerEnv [i].IdxNext   = i+1;
            sxr_TEnv.TimerEnv [i].Id        = 0;
            //sxr_TEnv.TimerEnv [i].Ctx       = sxr_HMalloc(SXR_TIMER_CTX_SIZE*sizeof(u32));
            memset ((u8 *)sxr_TEnv.TimerEnv [i].Ctx, 0, SXR_TIMER_CTX_SIZE*sizeof(u32));
        }

        sxr_TEnv.TimerEnv [SXR_NB_MAX_TIMER_ENVELOPPE - 1].IdxNext = SXR_TIM_NIL_IDX;
        sxr_TEnv.IdxFree = 0;

        InitDone = TRUE;
    }

    sxr_TCtx [I2d].TimerDelivery = TimerDelivery;
    sxr_TCtx [I2d].SetCntValue   = SetCntValue;
    sxr_TCtx [I2d].GetCntValue = GetCntValue;

    if (sxr_TCtx [I2d].TimerDelivery == NIL)
    {
        SXS_RAISE ((_SXR|TABORT|TDB,TSTR("Timer Delivery function missing\n",0x06c20002)));
    }

    sxr_TCtx [I2d].IdxFirst = SXR_TIM_NIL_IDX;
    sxr_TCtx [I2d].NbTimer  = 0;
}

/*
===============================================================================
   Function   : sxr_CheckTimer
 -----------------------------------------------------------------------------
   Scope      : Print a status for all the existing timer.
   Parameters : none.
   Return     : none.
===============================================================================
*/
u8 sxr_CheckTimer (void)
{
    u16 Idx;
    u32 Period = 0;
    u32 NbTimer = 0;
    u32 I2d;

    for (I2d=0; I2d < SXR_TIMER_NB_INSTANCE; I2d++)
    {
        Idx = sxr_TCtx [I2d].IdxFirst;
        Period = 0;

        if (sxr_TCtx [I2d].TimerDelivery != NIL)
        {
            SXS_FPRINTF ((_SXR|TSTDOUT|TDB|TNB_ARG(1),TSTR("Instance %i\n",0x06c20003), I2d));

            while (Idx != SXR_TIM_NIL_IDX)
            {
                if (Idx != SXR_TIM_NIL_IDX)
                {
                    Period += sxr_TEnv.TimerEnv [Idx].PeriodToNext;
                }

                SXS_FPRINTF ((_SXR|TIDU|TSTDOUT|TDB|TNB_ARG(4),TSTR("Ctx 0x%lx 0x%lx Id %x period %i\n",0x06c20004), sxr_TEnv.TimerEnv [Idx].Ctx [0], sxr_TEnv.TimerEnv [Idx].Ctx [1], sxr_TEnv.TimerEnv [Idx].Id, Period));
                NbTimer++;

                Idx = sxr_TEnv.TimerEnv [Idx].IdxNext;
            }
        }
    }
    SXS_FPRINTF ((_SXR|TSTDOUT|TDB|TNB_ARG(2),TSTR("Nb timers %i / %i\n",0x06c20005), NbTimer, SXR_NB_MAX_TIMER_ENVELOPPE));

    return (u8)NbTimer;
}

/*
===============================================================================
   Function   : sxr_PrintTimer
 -----------------------------------------------------------------------------
   Scope      : Print status corresponding to identified timers.
   Parameters : Pointer onto sxr_Timer_t.
   Return     : none.
===============================================================================
*/
void sxr_PrintTimer (sxr_Timer_t *Timer, u8 I2d)
{
    u16 Idx = sxr_TCtx [I2d].IdxFirst;
    u32 Period = 0;

    while (Idx != SXR_TIM_NIL_IDX)
    {
        if (Idx != SXR_TIM_NIL_IDX)
        {
            Period += sxr_TEnv.TimerEnv [Idx].PeriodToNext;
        }

        if (((Timer -> Id != 0)
        &&  ((sxr_TEnv.TimerEnv [Idx].Id & Timer -> Msk) == Timer -> Id))
        || ((Timer -> Id == 0)
        &&  ((sxr_TEnv.TimerEnv [Idx].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx)))
        {
            SXS_FPRINTF ((_SXR|TIDU|TSTDOUT|TDB|TNB_ARG(4),TSTR("Ctx %lx %lx Id %x period %i\n",0x06c20006), sxr_TEnv.TimerEnv [Idx].Ctx [0], sxr_TEnv.TimerEnv [Idx].Ctx [1], sxr_TEnv.TimerEnv [Idx].Id, Period));
        }
        Idx = sxr_TEnv.TimerEnv [Idx].IdxNext;
    }
}


