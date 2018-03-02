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
  File         sxr_tim.c
--------------------------------------------------------------------------------

  Scope      : Time management.

  History    :
--------------------------------------------------------------------------------
 Jun 13 2003 |  ADA  | Creation
 Apr 07 2005 |  ADA  | DeleteComplexTimer: SXR_DELETED_TIMER in Ctx 0 only !
 Oct 27 2005 |  ADA  | sxr_Tampax: propagate and decrease past due date.
 Nov 10 2005 |  ADA  | sxr_SetTimer: do not decrease first timer with GetCnt
             |       | result.
 Dec 15 2005 |  ADA  | sxr_Tampax: check period consistency only for FRAMED_TIMER.
 Jan 13 2006 |  ADA  | SetTimer: "Period %i > BasePeriod %i forbidden for Framed timer"
             |       | becomes a warning.
 Feb 06 2006 |  JFN  | sxr_DeleteComplexTimer : when the first timer is not
             |       | deleted, it must be marked after the call to the Free
             |       | function.
             |       | Raise the threshold to mark the first timer from 1 to 10.
================================================================================
*/

#include "sxr_tim.hp"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxs_lib.h"
#include "sxr_cpu.h"

// Framed timer check might be violated when L1 jobs enter sync domain while leaving async
// domain (task context) -- Some jobs might be started or reset via L1 events (not always via messages).
// So the framed timer strict check is disabled here.

//#define SXR_FRAMED_TIMER_STRICT_CHECK

/*
==============================================================================
   Function   : sxs_StartTimer
 -----------------------------------------------------------------------------
   Scope      : Start a timer. At timer due date, an event built with parameters
                Id and Data is send in the required mail box.
   Parameters : - Timer Period,
                - Id, user data,
                - Boolean restart is set to TRUE when timer must be stopped first.
                - mail box number.
   Return     : none
==============================================================================
*/
void sxs_StartTimer (u32 Period, u32 Id, void *Data, u8 ReStart, u8 Mbx)
{
    u32 Ctx [SXR_TIMER_CTX_SIZE];

    if ((Mbx & (SXR_TIMER_MBX_MSK >> SXR_TIMER_MBX_POS)) != Mbx)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("Mail box number overflow: %d\n",0x06aa0006), Mbx));
    }

    Ctx [0] = Id;
    Ctx [1] = (u32)Data;

    if (ReStart)
    {
        sxs_StopTimer (Id, Data, Mbx);
    }

    sxr_SetTimer (Period, Ctx, (u16)SXR_GEN_MBX_TIMER_ID (0, Mbx), SXR_REGULAR_TIMER);
}

/*
==============================================================================
   Function   : sxs_StopTimer
 -----------------------------------------------------------------------------
   Scope      : Stop the identified timer.
   Parameters : Id, user Data, mail box number.
   Return     : TRUE when timer found, FALSE otherwise.
==============================================================================
*/
u8 sxs_StopTimer (u32 Id, void *Data, u8 Mbx)
{
    u32 Ctx [SXR_TIMER_CTX_SIZE];
    sxr_Timer_t Timer = { 0, 0xFFFFFFFF, 0, 0 };

    if ((Mbx & (SXR_TIMER_MBX_MSK >> SXR_TIMER_MBX_POS)) != Mbx)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("Mail box number overflow: %d\n",0x06aa0006), Mbx));
    }

    Timer.Ctx = Id;
    Ctx [0] = Id;
    Ctx [1] = (u32)Data;

    if (!sxr_DeleteTimer (&Timer, NIL, FALSE, SXR_REGULAR_TIMER))
    {
        return sxr_RmvMsgFromMbx (Ctx, TRUE, Mbx,SXR_TIMER_CTX_SIZE);
    }
    else
    {
        return TRUE;
    }
}

/*
==============================================================================
   Function   : sxs_StopAllTimer
 -----------------------------------------------------------------------------
   Scope      : Stop all instances of identified timer.
   Parameters : Id, user Data, mail box number.
   Return     : TRUE when timer found, FALSE otherwise.
==============================================================================
*/
u8 sxs_StopAllTimer (u32 Id, void *Data, u8 Mbx)
{
    u32 Ctx [SXR_TIMER_CTX_SIZE];
    sxr_Timer_t Timer = { 0, 0xFFFFFFFF, 0, 0 };

    if ((Mbx & (SXR_TIMER_MBX_MSK >> SXR_TIMER_MBX_POS)) != Mbx)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("Mail box number overflow: %d\n",0x06aa0006), Mbx));
    }

    Timer.Ctx = Id;
    Ctx [0] = Id;
    Ctx [1] = (u32)Data;

    if (!sxr_DeleteTimer (&Timer, NIL, TRUE, SXR_REGULAR_TIMER))
    {
        return sxr_RmvMsgFromMbx (Ctx, TRUE, Mbx,SXR_TIMER_CTX_SIZE);
    }
    else
    {
        return TRUE;
    }
}

/*
==============================================================================
   Function   : sxs_ExistTimer
 -----------------------------------------------------------------------------
   Scope      : Check if the timer identified thanks to its Id exists and if
                so returns its period or SXR_NO_DUE_DATE when no timer found.
   Parameters : Timer Id.
   Return     : Period or SXR_NO_DUE_DATE.
==============================================================================
*/
u32 sxs_ExistTimer (u32 Id)
{
    sxr_Timer_t Timer = { 0, 0xFFFFFFFF, 0, 0 };
    Timer.Ctx = Id;

    return sxr_TimerQuery (&Timer, SXR_REGULAR_TIMER);
}

/*
==============================================================================
   Function   : _sxr_StartFunctionTimer
 -----------------------------------------------------------------------------
   Scope      : Start a timer such that after required period, the function
                given in parameter is called with the parameter given.
   Parameters : Period to wait,
                Function to be called
                Parameter to be given to the function called.
                Timer Id,
                Timer instance.
   return     : none.
==============================================================================
*/
void _sxr_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d)
{
    u32 Ctx [SXR_TIMER_CTX_SIZE];

    if (Id & SXR_MBX_TIMER)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1),TSTR("Bit %x reserved.\n",0x06aa0001), SXR_MBX_TIMER));
    }

    SXR_GEN_FUNCTION_TIMER_CTX (Ctx, Function, Param);

    sxr_SetTimer (Period, Ctx, (u16)(Id | SXR_FUNCTION_TIMER), I2d);
}


u32 _sxr_StopFunctionTimer (void (*Function)(void *), u8 I2d)
{
    u32 Ctx [SXR_TIMER_CTX_SIZE];
    sxr_Timer_t Timer = { 0, 0xFFFFFFFF, 0, SXR_TIMER_FUNCTION_IDX };

    Timer.Ctx = (u32)Function;

    return sxr_DeleteTimer (&Timer, Ctx, TRUE, SXR_REGULAR_TIMER);
}

/*
==============================================================================
   Function   : _sxr_StopFunctionTimer2
 -----------------------------------------------------------------------------
   Scope      : stop a function timer which related to param, add by licheng
   Parameters :
                Function to be stoped
                Parameter is related to function
                Timer instance.
   return     : none.
==============================================================================
*/


u32 _sxr_StopFunctionTimer2 (void (*Function)(void *),void *Param, u8 I2d)
{
	//u32 Ctx [SXR_TIMER_CTX_SIZE];
	sxr_Timer_t Timer = { 0, 0xFFFFFFFF, 0, SXR_TIMER_FUNCTION_IDX };
	u8 DelAll = TRUE;
	void (*Free)(u16, u32 *)=NULL;
	 u32 Status 	= sxr_EnterSc ();
	 u16 IdxCur 	= sxr_TCtx [I2d].IdxFirst;
	 u16 IdxPrev	= SXR_TIM_NIL_IDX;
	 u16 IdxMem 	= SXR_TIM_NIL_IDX;
	 u8  NbDeletion = 0;
	 u8  DeletedNotRemoved = FALSE;

	 while (IdxCur != SXR_TIM_NIL_IDX)
	 {
	  if (((sxr_TEnv.TimerEnv [IdxCur].Ctx [0] & Timer.Msk) == (u32)Param ) && ((sxr_TEnv.TimerEnv [IdxCur].Ctx [1] & Timer.Msk) == (u32)Function )) 
	  {

	   if (IdxCur != sxr_TCtx [I2d].IdxFirst)
	   {
		IdxMem = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

		sxr_TEnv.TimerEnv [IdxPrev].IdxNext = IdxMem;

		if (IdxMem != SXR_TIM_NIL_IDX)
		 sxr_TEnv.TimerEnv [IdxMem].PeriodToNext += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;

		sxr_TEnv.TimerEnv [IdxCur].IdxNext = sxr_TEnv.IdxFree;
		sxr_TEnv.IdxFree = IdxCur;

		IdxMem = IdxCur;
		IdxCur = sxr_TEnv.TimerEnv [IdxPrev].IdxNext;
		sxr_TCtx [I2d].NbTimer--;
	   }
	   else
	   {
		u32 NextPeriod = sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext;

		if ((sxr_TCtx [I2d].GetCntValue != NIL)
		&&	((NextPeriod = (*sxr_TCtx [I2d].GetCntValue) ()) <= 10))
		{
	//	   sxr_TEnv.TimerEnv [IdxCur].Ctx [0] = SXR_DELETED_TIMER;
		 DeletedNotRemoved = TRUE;
		 IdxMem  = IdxCur;
		 IdxPrev = IdxCur;
		 IdxCur  = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
		}
		else
		{
		 sxr_TCtx [I2d].IdxFirst = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

		 IdxMem = IdxCur;
		 IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

		 sxr_TEnv.TimerEnv [IdxMem].IdxNext = sxr_TEnv.IdxFree;
		 sxr_TEnv.IdxFree = IdxMem;

		 if (sxr_TCtx [I2d].IdxFirst != SXR_TIM_NIL_IDX)
		 {
		  sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext += NextPeriod;

		  if (sxr_TCtx [I2d].SetCntValue != NIL)
		   (*sxr_TCtx [I2d].SetCntValue) (sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext);
		 }
		 sxr_TCtx [I2d].NbTimer--;
		}
	   }

	   NbDeletion++;

	   if (Free != NIL)
		Free (sxr_TEnv.TimerEnv [IdxMem].Id, sxr_TEnv.TimerEnv [IdxMem].Ctx);

	   if (DeletedNotRemoved)
	   {
		sxr_TEnv.TimerEnv [IdxMem].Ctx [0] = SXR_DELETED_TIMER;
		DeletedNotRemoved = FALSE;
	   }

	   if (!DelAll)
		break;
	  }
	  else
	  {
	   IdxPrev = IdxCur;
	   IdxCur  = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
	  }
	 }
/*
	 if ((Ctx)
	 &&  (NbDeletion))
	  memcpy ((u8* )Ctx, (u8 *)sxr_TEnv.TimerEnv [IdxMem].Ctx, 4 * SXR_TIMER_CTX_SIZE);
*/
	 sxr_ExitSc (Status);

	 return NbDeletion;
}

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
void _sxr_StartMbxTimer (u32 period, u32 *evt, u8 mbx, u16 id, u8 i2d)
{
    if (id & (SXR_TIMER_MBX_MSK | SXR_FUNCTION_TIMER))
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1),TSTR("Bit %x reserved.\n",0x06aa0002), SXR_TIMER_MBX_MSK | SXR_FUNCTION_TIMER));
    }

    if ((mbx & (SXR_TIMER_MBX_MSK >> SXR_TIMER_MBX_POS)) != mbx)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("Mail box number overflow: %d\n",0x06aa0006), mbx));
    }

    sxr_SetTimer (period, evt, (u16)SXR_GEN_MBX_TIMER_ID(id, mbx), i2d);
}


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
u8 _sxr_StopMbxTimer (u32 *evt, u8 mbx, u16 id, u8 i2d)
{
    sxr_Timer_t timer = {0, 0xFFFFFFFF, 0, 0};

    if (id & (SXR_TIMER_MBX_MSK | SXR_FUNCTION_TIMER))
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1),TSTR("Bit %x reserved.\n",0x06aa0002), SXR_TIMER_MBX_MSK | SXR_FUNCTION_TIMER));
    }

    if ((mbx & (SXR_TIMER_MBX_MSK >> SXR_TIMER_MBX_POS)) != mbx)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("Mail box number overflow: %d\n",0x06aa0006), mbx));
    }

    // timer.Id being different from 0, we are looking for the timer with the 
    // same Id. timer.Ctx and timer.Idx are thus irrelvant here.
    timer.Ctx   = 0;
    timer.Msk   = 0xFFFFFFFF;
    timer.Id    = (u16)SXR_GEN_MBX_TIMER_ID(id, mbx);
    timer.Idx   = 0;

    if (!sxr_DeleteTimer (&timer, NIL, FALSE, i2d))
    {
        return sxr_RmvMsgFromMbx (evt, TRUE, mbx, SXR_TIMER_CTX_SIZE);
    }
    else
    {
        return TRUE;
    }
}

/*
==============================================================================
   Function   : sxr_SetTimer
 -----------------------------------------------------------------------------
   Scope      : Set a timer.
   Parameters : relative Period, Ctx, Id and instance number.
   Return     : none
==============================================================================
*/
void sxr_SetTimer (u32 PeriodParam, u32 *Ctx, u16 Id, u8 I2d)
{
    u16 IdxFree;
    s32 Period = (s32)PeriodParam;
    u32 Status = sxr_EnterSc();

    if (Period <= 0)
    {
        SXS_RAISE ((_SXR|TABORT|TDB,TSTR("Period must be > 0\n",0x06aa0003)));
    }

    if (sxr_TEnv.IdxFree == SXR_TIM_NIL_IDX)
    {
        SXS_RAISE ((_SXR|TABORT|TDB,TSTR("No more timer enveloppe\n",0x06aa0004), sxr_CheckTimer ()));
    }

    IdxFree = sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].IdxNext;

    if (sxr_TCtx [I2d].IdxFirst != SXR_TIM_NIL_IDX)
    {
        u16 IdxCur  = sxr_TCtx [I2d].IdxFirst;
        u16 IdxPrev = SXR_TIM_NIL_IDX;
        s32 TotalPeriod;

        if (sxr_TCtx [I2d].GetCntValue != NIL)
        {
            TotalPeriod = (s32)(*sxr_TCtx [I2d].GetCntValue) ();
//   sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext = TotalPeriod;
        }
        else
        {
            TotalPeriod = (s32)sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext;
        }

        while ((sxr_TEnv.TimerEnv [IdxCur].IdxNext != SXR_TIM_NIL_IDX )
        &&     (TotalPeriod < Period))
        {
            IdxPrev = IdxCur;
            IdxCur  = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

            TotalPeriod += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
        }

        if ((IdxPrev == SXR_TIM_NIL_IDX)
        &&  (TotalPeriod >= Period))
        {
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].PeriodToNext = Period;
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].IdxNext = sxr_TCtx [I2d].IdxFirst;

//   sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext -= Period;
            sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext = TotalPeriod - Period;
            sxr_TCtx [I2d].IdxFirst = sxr_TEnv.IdxFree;

            if ((sxr_TCtx [I2d].SetCntValue != NIL)
            &&  (sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext != 0))
            {
                (*sxr_TCtx [I2d].SetCntValue) (sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext);
            }
        }
        else if ((sxr_TEnv.TimerEnv [IdxCur].IdxNext == SXR_TIM_NIL_IDX)
             &&  (TotalPeriod < Period))
        {
            sxr_TEnv.TimerEnv [IdxCur].IdxNext = sxr_TEnv.IdxFree;
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].PeriodToNext = Period - TotalPeriod;
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].IdxNext = SXR_TIM_NIL_IDX;
        }
        else
        {
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].PeriodToNext = Period - (TotalPeriod - sxr_TEnv.TimerEnv [IdxCur].PeriodToNext);

            sxr_TEnv.TimerEnv [IdxPrev ].IdxNext = sxr_TEnv.IdxFree;
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].IdxNext = IdxCur;

            sxr_TEnv.TimerEnv [IdxCur ].PeriodToNext -= sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].PeriodToNext;
        }
    }
    else
    {
        if ( sxr_TCtx [I2d].SetCntValue != NIL )
        {
            (*sxr_TCtx [I2d].SetCntValue) (Period);
        }

        sxr_TCtx [I2d].IdxFirst = sxr_TEnv.IdxFree;

        sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].IdxNext = SXR_TIM_NIL_IDX;
        sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext = Period;
    }

    sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].Id = Id;
    memcpy ((u8* )sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].Ctx, Ctx, 4 * SXR_TIMER_CTX_SIZE);

    sxr_TEnv.IdxFree = IdxFree;
    sxr_ExitSc (Status);

    sxr_TCtx [I2d].NbTimer++;
}

/*
==============================================================================
   Function   : sxr_DeleteTimer
 -----------------------------------------------------------------------------
   Scope      : When Id = 0, delete all timer / Ctx  = Ctx [Idx] & Msk.
                When Id != 0 delete all timer / Id = Id & Msk.
                DelAll indicates if all the timers must be deleted or only the
                first timer found.
   Parameters : - Pointer onto sxr_Timer_t.
                - Pointer on Ctx. When not null, filled with last deleted timer
                context.
                - DelAll : When TRUE, delete all the timer that match criteria.
                - Timer instance.
   Return     : Number of deleted timers.
==============================================================================
*/
u32 sxr_DeleteTimer (sxr_Timer_t *Timer, u32 *Ctx, u8 DelAll, u8 I2d)
{
    return sxr_DeleteComplexTimer (Timer, Ctx, DelAll, NIL, I2d);
}

/*
==============================================================================
   Function   : sxr_DeleteTimer
 -----------------------------------------------------------------------------
   Scope      : When Id = 0, delete all timer / Ctx  = Ctx [Idx] & Msk.
                When Id != 0 delete all timer / Id = Id & Msk.
                DelAll indicates if all the timers must be deleted or only the
                first timer found.
   Parameters : - Pointer onto sxr_Timer_t.
                - Pointer on Ctx. When not null, filled with last deleted timer
                context.
                - DelAll : When TRUE, delete all the timer that match criteria.
                - Free : function called with the contexte of the deleted timer
                as parameter.
                - Timer instance.
   Return     : Number of deleted timers.
==============================================================================
*/
u32 sxr_DeleteComplexTimer (sxr_Timer_t *Timer, u32 *Ctx, u8 DelAll, void (*Free)(u16, u32 *), u8 I2d)
{
    u32 Status     = sxr_EnterSc ();
    u16 IdxCur     = sxr_TCtx [I2d].IdxFirst;
    u16 IdxPrev    = SXR_TIM_NIL_IDX;
    u16 IdxMem     = SXR_TIM_NIL_IDX;
    u8  NbDeletion = 0;
    u8  DeletedNotRemoved = FALSE;

    while (IdxCur != SXR_TIM_NIL_IDX)
    {
        if (((Timer -> Id == 0) && ((sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx ))
        ||  ((((Timer -> Id != 0) && ((sxr_TEnv.TimerEnv [IdxCur].Id & Timer -> Msk) == Timer -> Id)) && (sxr_TEnv.TimerEnv [IdxCur].Id != 0))))
        {

            if (IdxCur != sxr_TCtx [I2d].IdxFirst)
            {
                IdxMem = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

                sxr_TEnv.TimerEnv [IdxPrev].IdxNext = IdxMem;

                if (IdxMem != SXR_TIM_NIL_IDX)
                {
                    sxr_TEnv.TimerEnv [IdxMem].PeriodToNext += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
                }

                sxr_TEnv.TimerEnv [IdxCur].IdxNext = sxr_TEnv.IdxFree;
                sxr_TEnv.IdxFree = IdxCur;

                IdxMem = IdxCur;
                IdxCur = sxr_TEnv.TimerEnv [IdxPrev].IdxNext;
                sxr_TCtx [I2d].NbTimer--;
            }
            else
            {
                s32 NextPeriod = (s32)sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext;

                if ((sxr_TCtx [I2d].GetCntValue != NIL)
                &&  ((NextPeriod = (s32)(*sxr_TCtx [I2d].GetCntValue) ()) <= 10))
                {
//     sxr_TEnv.TimerEnv [IdxCur].Ctx [0] = SXR_DELETED_TIMER;
                    DeletedNotRemoved = TRUE;
                    IdxMem  = IdxCur;
                    IdxPrev = IdxCur;
                    IdxCur  = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
                }
                else
                {
                    sxr_TCtx [I2d].IdxFirst = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

                    IdxMem = IdxCur;
                    IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

                    sxr_TEnv.TimerEnv [IdxMem].IdxNext = sxr_TEnv.IdxFree;
                    sxr_TEnv.IdxFree = IdxMem;

                    if (sxr_TCtx [I2d].IdxFirst != SXR_TIM_NIL_IDX)
                    {
                        sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext += NextPeriod;

                        if (sxr_TCtx [I2d].SetCntValue != NIL)
                        {
                            (*sxr_TCtx [I2d].SetCntValue) (sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext);
                        }
                    }
                    sxr_TCtx [I2d].NbTimer--;
                }
            }

            NbDeletion++;

            if (Free != NIL)
            {
                Free (sxr_TEnv.TimerEnv [IdxMem].Id, sxr_TEnv.TimerEnv [IdxMem].Ctx);
            }

            if (DeletedNotRemoved)
            {
                sxr_TEnv.TimerEnv [IdxMem].Ctx [0] = SXR_DELETED_TIMER;
                DeletedNotRemoved = FALSE;
            }

            if (!DelAll)
            {
                break;
            }
        }
        else
        {
            IdxPrev = IdxCur;
            IdxCur  = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
        }
    }

    if ((Ctx)
    &&  (NbDeletion))
    {
        memcpy ((u8* )Ctx, (u8 *)sxr_TEnv.TimerEnv [IdxMem].Ctx, 4 * SXR_TIMER_CTX_SIZE);
    }

    sxr_ExitSc (Status);

    return NbDeletion;
}

/*
==============================================================================
   Function   : sxr_Tampax
 -----------------------------------------------------------------------------
   Scope      : Take into account the number of time period ellapsed for
                scheduling. Timer at due date delivery.
   Parameters : Number of time period elapsed since last call.
   Return     : none
==============================================================================
*/
void sxr_Tampax (u32 Period, u8 I2d)
{
    u16 Idx, IdxFirst, IdxPrev = 0;
    u32 Status;

    if (sxr_TCtx [I2d].IdxFirst == SXR_TIM_NIL_IDX)
    {
        return;
    }

    Status = sxr_EnterSc ();

    Idx = sxr_TCtx [I2d].IdxFirst;

    if (Period < sxr_TEnv.TimerEnv [Idx].PeriodToNext)
    {
        sxr_TEnv.TimerEnv [Idx].PeriodToNext -= Period;

        if (sxr_TCtx [I2d].SetCntValue != NIL)
        {
            (*sxr_TCtx [I2d].SetCntValue) (sxr_TEnv.TimerEnv [Idx].PeriodToNext);
        }
    }
    else
    {
        u32 BasePeriod = 0;

        while ((Idx != SXR_TIM_NIL_IDX)
        &&     ((sxr_TEnv.TimerEnv [Idx].PeriodToNext + BasePeriod) <= Period))
        {
            BasePeriod += sxr_TEnv.TimerEnv [Idx].PeriodToNext;

            if (I2d == SXR_FRAMED_TIMER && BasePeriod < Period)
            {
#ifdef SXR_FRAMED_TIMER_STRICT_CHECK
                SXS_RAISE ((_SXR|TNB_ARG(2)|TDB|TABORT,TSTR("Period %i > BasePeriod %i forbidden for Framed timer\n",0x06aa0005), Period, BasePeriod));
#else
                SXS_TRACE (_SXR|TNB_ARG(2)|TDB,TSTR("Period %i > BasePeriod %i forbidden for Framed timer\n",0x06aa0005), Period, BasePeriod);
#endif
            }

            IdxPrev = Idx;
            Idx = sxr_TEnv.TimerEnv [Idx].IdxNext;
        }

        if (BasePeriod < Period)
        {
            if (I2d == SXR_FRAMED_TIMER)
            {
#ifdef SXR_FRAMED_TIMER_STRICT_CHECK
                SXS_RAISE ((_SXR|TNB_ARG(2)|TDB|TABORT,TSTR("Period %i > BasePeriod %i forbidden for Framed timer\n",0x06aa0005), Period, BasePeriod));
#else
                SXS_TRACE (_SXR|TNB_ARG(2)|TDB,TSTR("Period %i > BasePeriod %i forbidden for Framed timer\n",0x06aa0005), Period, BasePeriod);
#endif
            }

            if (Idx != SXR_TIM_NIL_IDX)
            {
                sxr_TEnv.TimerEnv [Idx].PeriodToNext -= (Period - BasePeriod);
            }
        }

        sxr_TEnv.TimerEnv [IdxPrev].IdxNext = SXR_TIM_NIL_IDX;

        IdxFirst = sxr_TCtx [I2d].IdxFirst;
        sxr_TCtx [I2d].IdxFirst = Idx;

        if ((sxr_TCtx [I2d].SetCntValue != NIL)
        &&  (Idx != SXR_TIM_NIL_IDX ))
        {
            (*sxr_TCtx [I2d].SetCntValue) (sxr_TEnv.TimerEnv [Idx].PeriodToNext);
        }

        while (IdxFirst != SXR_TIM_NIL_IDX)
        {
            sxr_ExitSc (Status);

            if (sxr_TEnv.TimerEnv [IdxFirst].Ctx [0] != SXR_DELETED_TIMER)
            {
                if (sxr_TEnv.TimerEnv [IdxFirst].Id & SXR_FUNCTION_TIMER)
                {
                    ((void (*)(void*, u16))sxr_TEnv.TimerEnv [IdxFirst].Ctx [SXR_TIMER_FUNCTION_IDX]) ((void *) sxr_TEnv.TimerEnv [IdxFirst].Ctx [SXR_TIMER_PARAM_IDX], sxr_TEnv.TimerEnv [IdxFirst].Id);
                }
                else if (sxr_TEnv.TimerEnv [IdxFirst].Id & SXR_MBX_TIMER)
                {
                    sxr_Send (sxr_TEnv.TimerEnv [IdxFirst].Ctx, SXR_GET_TIMER_MBX(sxr_TEnv.TimerEnv [IdxFirst].Id), SXR_SEND_EVT);
                }
                else
                {
                    (*sxr_TCtx [I2d].TimerDelivery) (sxr_TEnv.TimerEnv [IdxFirst].Ctx);
                }
            }

            Status = sxr_EnterSc();

            Idx = sxr_TEnv.IdxFree;
            sxr_TEnv.IdxFree = IdxFirst;
            IdxFirst = sxr_TEnv.TimerEnv [IdxFirst].IdxNext;
            sxr_TEnv.TimerEnv [sxr_TEnv.IdxFree].IdxNext = Idx;

            sxr_TCtx [I2d].NbTimer--;
        }
    }
    sxr_ExitSc (Status);
}

/*
==============================================================================
   Function   : sxr_IsTimerSet
 -----------------------------------------------------------------------------
   Scope      : Check if a Timer Unit is up and running
   Parameters : Pointer onto sxr_Timer_t.
   Return     : boolean
==============================================================================
*/
u8 sxr_IsTimerSet (sxr_Timer_t *Timer, u8 I2d)
{
    u32 Status = sxr_EnterSc ();
    u16 IdxCur = sxr_TCtx [I2d].IdxFirst;

    while (IdxCur != SXR_TIM_NIL_IDX)
    {
        if ((sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx)
        {
            sxr_ExitSc (Status);
            return TRUE;
        }

        IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
    }

    sxr_ExitSc (Status);
    return FALSE;
}

/*
==============================================================================
   Function   : sxr_TimerList
 -----------------------------------------------------------------------------
   Scope      : Retreive a list of active timers.
   Parameters : Pointer onto TimerList_t, instance number.
   Return     : Number of timer found.
==============================================================================
*/
u8 sxr_TimerList (sxr_Timer_t *Timer, u32 *Tab, u8 Max, u8 I2d)
{
    u32 Status  = sxr_EnterSc ();
    u16 IdxCur  = sxr_TCtx [I2d].IdxFirst;
    u8  NbTimer = 0;

    while (IdxCur != SXR_TIM_NIL_IDX)
    {
        if (((Timer -> Id == 0) && (sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx)
        ||  ((Timer -> Id != 0) && (sxr_TEnv.TimerEnv [IdxCur].Id & Timer -> Msk) == Timer -> Id))
        {
            Tab [NbTimer] = sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx];

            if (++NbTimer >= Max)
            {
                break;
            }
        }
        IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
    }
    sxr_ExitSc (Status);

    return NbTimer;
}


/*
==============================================================================
   Function   : sxr_NextTimerQuery
 -----------------------------------------------------------------------------
   Scope      : Return the next timer remaining period for selected timer type.
   Parameters : Mask to determine the kind of timer to be considered.
                Instance number.
   Return     : Period or SXR_NO_DUE_DATE when no timer found.
==============================================================================
*/
u32 sxr_NextTimerQuery (u16 Mask, u8 I2d)
{
    u32 Status = sxr_EnterSc ();
    s32 Period;
    u16 IdxCur = sxr_TCtx [I2d].IdxFirst;

    if (IdxCur == SXR_TIM_NIL_IDX)
    {
        sxr_ExitSc (Status);
        return SXR_NO_DUE_DATE;
    }

    Period = sxr_TCtx [I2d].GetCntValue != NIL ?
             (s32)(*sxr_TCtx [I2d].GetCntValue) () :
             (s32)sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;

    while ((sxr_TEnv.TimerEnv [IdxCur].Id & Mask) != sxr_TEnv.TimerEnv [IdxCur].Id)
    {
        IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

        if (IdxCur != SXR_TIM_NIL_IDX)
        {
            Period += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
        }
        else
        {
            break;
        }
    }

    if (Period < 0)
    {
        Period = 0;
    }

    if (IdxCur == SXR_TIM_NIL_IDX)
    {
        Period = SXR_NO_DUE_DATE;
    }

    sxr_ExitSc (Status);
    return (u32)Period;
}

/*
==============================================================================
   Function   : sxr_TimerQuery
 -----------------------------------------------------------------------------
   Scope      : Return remaining period of the identified timer.
   Parameters : Pointer onto TimerQuery_t, instance number.
   Return     : Period or SXR_NO_DUE_DATE when no timer found.
==============================================================================
*/
u32 sxr_TimerQuery (sxr_Timer_t *Timer, u8 I2d)
{
    u32 Status = sxr_EnterSc ();
    s32 TotalPeriod;
    u16 IdxCur = sxr_TCtx [I2d].IdxFirst;

    if (IdxCur != SXR_TIM_NIL_IDX)
    {

        if (sxr_TCtx [I2d].GetCntValue != NIL)
        {
            TotalPeriod = (s32)(*sxr_TCtx [I2d].GetCntValue) ();
        }
        else
        {
            TotalPeriod = (s32)sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
        }

        while (IdxCur != SXR_TIM_NIL_IDX)
        {
            if (((Timer -> Id == 0)
            &&   ((sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx))
            ||  ((Timer -> Id != 0)
            &&   ((sxr_TEnv.TimerEnv [IdxCur].Id) != 0 )
            &&   ((sxr_TEnv.TimerEnv [IdxCur].Id & Timer -> Msk) == Timer -> Id)))
            {
                sxr_ExitSc (Status);
                return (TotalPeriod<0) ? 0 : (u32)TotalPeriod;
            }

            IdxCur = sxr_TEnv.TimerEnv [IdxCur].IdxNext;

            if (IdxCur != SXR_TIM_NIL_IDX)
            {
                TotalPeriod += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
            }
        }
    }

    sxr_ExitSc (Status);
    return SXR_NO_DUE_DATE;
}

/*
==============================================================================
   Function   : sxr_PeriodToTimer
 -----------------------------------------------------------------------------
   Scope      : Return identifier corresponding to the specified Period
                and generic Ctx or Id.
   Parameters : Pointer onto sxr_Timer_t, Period, instance number.
   return     : Updated sxr_Timer_t (Ctx and Id) or NIL.
==============================================================================
*/
sxr_Timer_t *sxr_PeriodToTimer (sxr_Timer_t *Timer, u32 Period, u8 I2d)
{
    u32 Status = sxr_EnterSc ();
    s32 TotalPeriod;
    u16 IdxCur = sxr_TCtx [I2d].IdxFirst;

    if (sxr_TCtx [I2d].GetCntValue != NIL)
    {
        TotalPeriod = (s32)(*sxr_TCtx [I2d].GetCntValue) ();
    }
    else
    {
        TotalPeriod = (s32)sxr_TEnv.TimerEnv [sxr_TCtx [I2d].IdxFirst].PeriodToNext;
    }

    while ((IdxCur != SXR_TIM_NIL_IDX)
    &&     (TotalPeriod < Period))
    {
        IdxCur     = sxr_TEnv.TimerEnv [IdxCur].IdxNext;
        TotalPeriod += sxr_TEnv.TimerEnv [IdxCur].PeriodToNext;
    }

    if (TotalPeriod < 0)
    {
        TotalPeriod = 0;
    }

    if (TotalPeriod == Period)
    {
        if (((Timer -> Id == 0)
        &&  ((sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx] & Timer -> Msk) == Timer -> Ctx))
        ||  ((Timer -> Id != 0)
        &&  ((sxr_TEnv.TimerEnv [IdxCur].Id) != 0 )
        &&  ((sxr_TEnv.TimerEnv [IdxCur].Id & Timer -> Msk) == Timer -> Id)))
        {
            Timer -> Ctx = sxr_TEnv.TimerEnv [IdxCur].Ctx [Timer -> Idx];
            Timer -> Id = sxr_TEnv.TimerEnv [IdxCur].Id;
            sxr_ExitSc (Status);
            return Timer;
        }
    }

    sxr_ExitSc (Status);
    return NIL;
}




