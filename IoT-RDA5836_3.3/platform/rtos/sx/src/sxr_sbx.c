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
  File         sxr_sbx.c
--------------------------------------------------------------------------------

  Scope      : Systeme executif synchronization boxes management.

  History    :
--------------------------------------------------------------------------------
  Mar 28 2006  |  ADA  | sxr_Send calls sxr_CheckSbx instead of sxr_CheckCluster
  Jun 13 2003  |  ADA  | Creation
================================================================================
*/

#include "sxs_io.h"
#include "sxs_lib.h"
#include "sxs_rmt.h"
#include "sxs_rmc.h"
#include "sxr_ops.h"
#include "sxr_tksd.hp"
#include "sxr_sbx.hp"

#include "sxp_debug.h"

#include "tgt_m.h"

#ifdef SX_PROFILING
u32 sxr_ProfileCfg = 
        SX_PRFOILE_ENV_ENABLE;
        //SX_PROFILE_QUEUE_ENABLE;
        //SX_PRFOILE_ENV_ENABLE|SX_PROFILE_QUEUE_ENABLE;
#endif // SX_PROFILING

//SemaInfo_t sxr_SemaInfo[SXR_NB_MAX_SEM];
SemaInfo_t *sxr_SemaInfo;
u32 toEvt[4] = {0xfefefefe,0,0,0};

// =============================================================================
// sxr_Send
// -----------------------------------------------------------------------------
/// Send a message or an event to a synchronization box.
/// @param Msg          Pointer onto message or event.
/// @param Sbx          Box number.
/// @param MsgStatus    Message status.
// =============================================================================
void sxr_Send (void *Msg, u8 Sbx, u8 MsgStatus)
{
    u16 IdxCur;
    u8  TaskId;

    if (sxr_Sbx.Queue [Sbx].TrcLev > 0)
    {
        SXS_TRACE (_SXR|TSMAP(1)|TSTDOUT|TNB_ARG(2)|TDB,TSTR("%s put a message into Sbx %i\n",0x06a90001), sxr_Task.Ctx [sxr_Task.Active].Desc -> Name, Sbx);
    }

    if (!(sxr_Sbx.RmtSbx [Sbx >> 5] & (1 << (Sbx & 31))))
    {
        u32 Status = sxr_EnterSc ();


        if (sxr_Sbx.EnvIdxFree == SXR_NO_ENV)
        {
            sxr_CheckSbx();
            SXS_RAISE ((_SXR|TABORT|TDB,TSTR("No more envelope\n",0x06a90002)));
        }

        if ((Sbx > SXR_NB_TOTAL_SBX)
            || (sxr_Sbx.Queue [Sbx].Id != SXR_SBX_ALLOCATED))
        {
    
            SXS_RAISE ((_SXR|TABORT|TNB_ARG(1)|TDB,TSTR("Unknown synchronization box %i\n",0x06a90003), Sbx));
        }

        IdxCur = sxr_Sbx.EnvIdxFree;
        sxr_Sbx.EnvIdxFree = sxr_Sbx.Env [IdxCur].Next;

        SX_PROFILE_ENVELOP_USE_START(IdxCur);

        if (!(MsgStatus & SXR_SEND_EVT))
        {
            sxr_Sbx.Env [IdxCur].EvtTag = FALSE;
            sxr_Sbx.Env [IdxCur].Evt [0] = (u32)Msg;
        }
        else
        {
            u8 i;

            sxr_Sbx.Env [IdxCur].EvtTag = TRUE;

            for (i=0; i<SXR_EVT_MBX_SIZE; i++)
            {
                sxr_Sbx.Env [IdxCur].Evt [i] = ((u32 *) Msg) [i];
            }
        }

        if (sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV)
        {
            sxr_Sbx.Queue [Sbx].Msg.First = IdxCur;
            sxr_Sbx.Queue [Sbx].Msg.Last  = IdxCur;
            sxr_Sbx.Env [IdxCur].Next     = SXR_NO_ENV;
        }
        else if (MsgStatus & SXR_QUEUE_FIRST)
        {
            sxr_Sbx.Env [IdxCur].Next     = sxr_Sbx.Queue [Sbx].Msg.First;
            sxr_Sbx.Queue [Sbx].Msg.First = IdxCur;
        }
        else
        {
            sxr_Sbx.Env [sxr_Sbx.Queue [Sbx].Msg.Last].Next = IdxCur;
            sxr_Sbx.Queue [Sbx].Msg.Last = IdxCur;
            sxr_Sbx.Env [IdxCur].Next    = SXR_NO_ENV;
        }

        if (sxr_Sbx.Queue [Sbx].Task.First != SXR_NO_TASK )
        {
            TaskId = sxr_Sbx.Queue [Sbx].Task.First;
            sxr_Sbx.Queue [Sbx].Task.First = sxr_Sbx.Queue [TaskId].Task.Next;
            sxr_Sbx.Queue [TaskId].Task.Next = SXR_NO_TASK;
            sxr_Task.Ctx [TaskId].State &= ~SXR_WAIT_MSG_TSK;

            sxr_WakeUpTask (TaskId);
        }

        sxr_ExitSc (Status);
    }
    else
    {
#if 0
        u16 Size;
        sxs_RmtMbx_t *RmtMbx;

        if (!(MsgStatus & SXR_SEND_EVT))
        {
            if (sxr_Sbx.TxMsg)
            {
                Msg = sxr_Sbx.TxMsg (Msg);
            }

            Size = sxr_GetMemSize (Msg);
        }
        else
        {
            Size = 4 * SXR_EVT_MBX_SIZE;
        }

        RmtMbx = (sxs_RmtMbx_t *)_sxs_SendToRemote (SXS_MBX_RMC, NIL,(u16)(Size + sizeof (sxs_RmtMbx_t)), SXS_RMT_SPL_CPU_IDX); // - 4));

        if (RmtMbx != NIL)
        {
            RmtMbx -> Mbx = Sbx;
            RmtMbx -> MsgStatus = MsgStatus;
            memcpy ((u8*)RmtMbx -> Data, Msg, Size);
        }

        if (!(MsgStatus & SXR_SEND_EVT))
        {
            sxr_Free (Msg);
        }

#ifndef __EMBEDDED__
        sxs_RmtBufferFlush ();
#endif
#ifdef __SXS_RMT_PROCESS_IDLE__
        while ( sxs_SerialFlush( ) );
#endif
#endif
    }
}



// =============================================================================
// sxr_Wait
// -----------------------------------------------------------------------------
/// Wait for a message or an event on a synchronization box.
/// @param Evt  Pointer onto Event to be received.
/// @param Sbx  Synchronization box number.
/// @return     Pointer onto message or NIL for event.
// =============================================================================
void *sxr_Wait (u32 *Evt, u8 Sbx
#ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
,u32 period
#endif
)
{
#ifndef CT_NO_DEBUG
    extern VOID hal_GetK1(UINT32* k1);
    UINT32 k1 = 1;
    hal_GetK1(&k1);
    if (k1 != 0
        // Dirty workaround: BAL_TaskInit() might take semaphore. At that time
        // SX has been init but scheduling is not started yet -- k1 is not zero.
        && sxr_Task.Ctx [sxr_Task.Active].State != SXR_PENDING_TSK
       )
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("sxr_Wait() must run in task context! (k1=0x%08x)\n",0x06a9000a),
                            k1));
    }
    if (sxr_Task.Active == sxr_Task.Idle)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(2), TSTR("sxr_Wait() cannot run in idle task! (active=%d, idle=%d)\n",0x06a9000b),
                            sxr_Task.Active, sxr_Task.Idle));
    }
#endif // ! CT_NO_DEBUG

    u8    i;
    u16   IdxCur;
    void *Msg = NIL;
    u32   Status = sxr_EnterSc ();

    SX_PROFILE_WAIT_QUEUE_START(Sbx);
    
    if ((Sbx > SXR_NB_TOTAL_SBX)
        || (sxr_Sbx.Queue [Sbx].Id != SXR_SBX_ALLOCATED))
    {
        SXS_RAISE ((_SXR|TABORT|TNB_ARG(1)|TDB,TSTR("Unknown synchronization box %i\n",0x06a90004), Sbx));
    }

    sxr_Sbx.Queue [sxr_Task.Active].Task.Next = SXR_NO_TASK;

    do {
        if (sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV)
        {
            if (sxr_Task.Active == sxr_Task.Idle)
            {
                SXS_RAISE ((_SXR|TABORT|TDB,TSTR("Idle Task cannot wait for a message.\n",0x06a90005)));
            }

            if (sxr_Sbx.Queue [Sbx].Task.First == SXR_NO_TASK)
            {
                sxr_Sbx.Queue [Sbx].Task.First = sxr_Task.Active;
            }
            
#ifdef SX_SBX_QUEUE_BY_PRIORITY
            else
            {
                u8 Index = sxr_Sbx.Queue [Sbx].Task.First;
                u8 NextIndex = sxr_Sbx.Queue [Index].Task.Next;        
                
                if((sxr_Task.Ctx [sxr_Task.Active].Desc -> Priority < sxr_Task.Ctx [Index].Desc -> Priority)){ 
                    sxr_Sbx.Queue [sxr_Task.Active].Task.Next = Index;
                    sxr_Sbx.Queue [Sbx].Task.First = sxr_Task.Active;
                }
                else{
                    while (( NextIndex != SXR_NO_TASK )
                            &&     (sxr_Task.Ctx [sxr_Task.Active].Desc -> Priority > sxr_Task.Ctx [NextIndex].Desc -> Priority))
                    {
                        Index = NextIndex;
                        NextIndex = sxr_Sbx.Queue [Index].Task.Next;
                    }
                    
                    sxr_Sbx.Queue [Index].Task.Next = sxr_Task.Active;
                        
                    sxr_Sbx.Queue [sxr_Task.Active].Task.Next = NextIndex;
                }
            }
#else
            else
            {
                u8 NextTsk = sxr_Sbx.Queue [Sbx].Task.First;

                while ( sxr_Sbx.Queue [NextTsk].Task.Next != SXR_NO_TASK )
                {
                    NextTsk = sxr_Sbx.Queue [NextTsk].Task.Next;
                }

                sxr_Sbx.Queue [NextTsk].Task.Next = sxr_Task.Active;
            }
#endif

            sxr_Task.Ctx [sxr_Task.Active].State |= SXR_WAIT_MSG_TSK;
            sxr_Task.Ctx [sxr_Task.Active].State &= ~SXR_ACTIVE_TSK;
            #ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
            /* start timer before switch to next task, the timer will send an fixed format
             * event to specified mailbox 
             */
            if (period != COS_WAIT_FOREVER) {                
                sxr_StopMbxTimer(toEvt, Sbx, Sbx); 
                sxr_StartMbxTimer(period MS_WAITING, toEvt, Sbx, Sbx);
            }
            #endif

            sxr_RunNextTask ();
        }

        //sxr_Sbx.Queue [Sbx].Task.First = sxr_Sbx.Queue [sxr_Task.Active].Task.Next;
        if ( sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV )
        {
            SXS_FPRINTF ((_SXR|TDB,TSTR("Message lost !\n",0x06a90006)));
        }

    } while ( sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV);


    IdxCur = sxr_Sbx.Queue [Sbx].Msg.First;
    sxr_Sbx.Queue [Sbx].Msg.First = sxr_Sbx.Env [IdxCur].Next;

    sxr_Sbx.Env [IdxCur].Next = sxr_Sbx.EnvIdxFree;
    sxr_Sbx.EnvIdxFree = IdxCur;

    SX_PROFILE_WAIT_QUEUE_END(Sbx);
    SX_PROFILE_ENVELOP_USE_END(IdxCur);

    if ( sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV )
    {
        sxr_Sbx.Queue [Sbx].Msg.Last = SXR_NO_ENV;
    }

    if (sxr_Sbx.Env [IdxCur].EvtTag)
    {

        if ( Evt == NIL )
        {
            SXS_RAISE ((_SXR|TABORT|TDB,TSTR("Unable to deliver event.\n",0x06a90007)));
        }

        for (i=0; i<SXR_EVT_MBX_SIZE; i++)
        {
            Evt [i] = sxr_Sbx.Env [IdxCur].Evt [i];
        }
    }
    else
    {
        Msg = (void *)sxr_Sbx.Env [IdxCur].Evt [0];
    }    

    /* In any case, stop the timer */
	#ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
    sxr_StopMbxTimer(toEvt, Sbx, Sbx);
    #endif

    sxr_ExitSc (Status);

    if (sxr_Sbx.Queue [Sbx].TrcLev > 0)
    {
        if (Msg != NIL)
        {
            SXS_TRACE (_SXR|TSMAP(1)|TSTDOUT|TNB_ARG(2)|TDB,TSTR("%s get a message from Sbx %i\n",0x06a90008), sxr_Task.Ctx [sxr_Task.Active].Desc -> Name, Sbx);
            SXS_DUMP (_SXR|TSTDOUT, 0, Msg, sxr_GetMemSize (Msg));
        }
        else if(Evt != NIL)
        {
            SXS_TRACE (_SXR|TSMAP(1)|TSTDOUT|TNB_ARG(4)|TDB,TSTR("%s get the Event 0x%08lx 0x%08lx from Sbx %i\n",0x06a90009), sxr_Task.Ctx [sxr_Task.Active].Desc -> Name, Evt [0], Evt [1], Sbx);
        }
    }


    return Msg;
}

// =============================================================================
// sxr_SbxHot
// -----------------------------------------------------------------------------
/// Check if at least a message or an event is in the synchronisation box.
/// @param Sbx  Synchronization box number.
/// @return     TRUE when box is hot, FALSE otherwise.
// =============================================================================
u8 sxr_SbxHot (u8 Sbx)
{
    return (sxr_Sbx.Queue [Sbx].Msg.First != SXR_NO_ENV);
}

// =============================================================================
// sxr_RemoveCustMsgFromMbox
// -----------------------------------------------------------------------------
/// Remove a/all message  from a mail box.add by licheng
/// @param Sbx          Mail box number.
/// @param msgid      message wan't to remove
/// @param isAny  	remove all message or only the customed msgid message 
/// @return             TRUE when message or event has been found, FALSE otherwise.
// =============================================================================

u8 sxr_RemoveCustMsgFromMbox (u8 Sbx,u8 msgid,u8 isAny)
{
    u32 Status   = sxr_EnterSc ();
    u16 IdxCur   = sxr_Sbx.Queue [Sbx].Msg.First;
    u16 IdxPrev  = sxr_Sbx.Queue [Sbx].Msg.First;
    u8  MsgFound = FALSE;
   typedef enum
   {
	   GCJ_MSG_COND_WAIT,
	   GCJ_MSG_COND_WAIT_TIMEOUT,
	   GCJ_MSG_PTHREAD_SIG,
	   GCJ_MSG_MUTEX,
	   GCJ_MSG_GCJ_KILL,
   } GCJ_BODY_MSG_T ;
   typedef GCJ_BODY_MSG_T MsgBody_t;
    typedef struct 
    {
       u32	  Id ;		  // Identifier Unique for whole stack
       u32	  SimId;	   
    }MsgHead_t;
    typedef struct 
    {
       MsgHead_t   H;
       MsgBody_t   B;
    
    }Msg_t;
    while (IdxCur != SXR_NO_ENV)
    {
    	MsgFound = FALSE;
	Msg_t * sig = (Msg_t *)sxr_Sbx.Env [IdxCur].Evt [0];
        if ( sig->B  ==  msgid || isAny)
        {
            MsgFound = TRUE;
	    sxr_Free(sig);		
        }

        if (MsgFound)
        {
            if (sxr_Sbx.Queue [Sbx].Msg.First == IdxCur)
            {
                sxr_Sbx.Queue [Sbx].Msg.First = sxr_Sbx.Env [IdxCur].Next;

                if ( sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV )
                {
                    sxr_Sbx.Queue [Sbx].Msg.Last = SXR_NO_ENV;
                }
            }
            else if (sxr_Sbx.Queue [Sbx].Msg.Last == IdxCur )
            {
                sxr_Sbx.Queue [Sbx].Msg.Last = IdxPrev;
                sxr_Sbx.Env [IdxPrev].Next  = SXR_NO_ENV;
            }
            else
            {
                sxr_Sbx.Env [IdxPrev].Next = sxr_Sbx.Env [IdxCur].Next;
            }

            sxr_Sbx.Env [IdxCur].Next = sxr_Sbx.EnvIdxFree;
            sxr_Sbx.EnvIdxFree = IdxCur;
	    IdxCur	 = sxr_Sbx.Queue [Sbx].Msg.First;
	    IdxPrev  = sxr_Sbx.Queue [Sbx].Msg.First;
        }
        else
        {
            IdxPrev = IdxCur;
            IdxCur  = sxr_Sbx.Env [IdxCur].Next;
        }
    }

    sxr_ExitSc (Status);

    return MsgFound;
}

// =============================================================================
// sxr_RmvTaskFromSbx
// -----------------------------------------------------------------------------
/// Remove a task waiting on a SBX.
/// @param TaskId   Task Id.
/// @return         TRUE when task has been removed, FALSE otherwise.
// =============================================================================
u8 sxr_RmvTaskFromSbx (u8 TaskId)
{
    u8 Sbx;

    for (Sbx=0; Sbx<SXR_NB_TOTAL_SBX; Sbx++)
    {
        if (sxr_Sbx.Queue [Sbx].Id == SXR_SBX_ALLOCATED)
        {
            u8 TaskPrev = sxr_Sbx.Queue [Sbx].Task.First;

            if (TaskPrev == TaskId)
            {
                sxr_Sbx.Queue [Sbx].Task.First = sxr_Sbx.Queue [TaskPrev].Task.Next;
                return TRUE;
            }
            else if (TaskPrev != SXR_NO_TASK)
            {
                u8 TaskCur = sxr_Sbx.Queue [TaskPrev].Task.Next;

                while ((TaskCur != TaskPrev)
                    && (TaskCur != SXR_NO_TASK))
                {
                    TaskPrev = TaskCur;
                    TaskCur = sxr_Sbx.Queue [TaskPrev].Task.Next;
                }

                if (TaskCur != SXR_NO_TASK)
                {
                    sxr_Sbx.Queue [TaskPrev].Task.Next = sxr_Sbx.Queue [TaskCur].Task.Next;
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

// =============================================================================
// sxr_RmvMsgFromMbx
// -----------------------------------------------------------------------------
/// Remove a message or an event from a mail box.
/// @param Msg          Pointer onto a message.
/// @param Evt          Event description.
/// @param Sbx          Mail box number.
/// @param EvtCtxSize   Event size(0 for message).
/// @return             TRUE when message or event has been found, FALSE otherwise.
// =============================================================================
u8 sxr_RmvMsgFromMbx ( void *Msg, u8 Evt, u8 Sbx, u8 EvtCtxSize )
{
    u32 Status   = sxr_EnterSc ();
    u16 IdxCur   = sxr_Sbx.Queue [Sbx].Msg.First;
    u16 IdxPrev  = sxr_Sbx.Queue [Sbx].Msg.First;
    u8  MsgFound = FALSE;
    u32 i;

    while ((IdxCur != SXR_NO_ENV)
        && (!MsgFound))
    {
        if (Evt)
        {
            if ( sxr_Sbx.Env [IdxCur].EvtTag)
            {
                MsgFound = TRUE;

                for (i=0; i < EvtCtxSize; i++)
                {
                    if (sxr_Sbx.Env [IdxCur].Evt [i] != ((u32 *) Msg) [i])
                    {
                        MsgFound = FALSE;
                        break;
                    }
                }
            }
        }
        else if ((void *)sxr_Sbx.Env [IdxCur].Evt [0] == Msg)
        {
            MsgFound = TRUE;
        }

        if (MsgFound)
        {
            if (sxr_Sbx.Queue [Sbx].Msg.First == IdxCur)
            {
                sxr_Sbx.Queue [Sbx].Msg.First = sxr_Sbx.Env [IdxCur].Next;

                if ( sxr_Sbx.Queue [Sbx].Msg.First == SXR_NO_ENV )
                {
                    sxr_Sbx.Queue [Sbx].Msg.Last = SXR_NO_ENV;
                }
            }
            else if (sxr_Sbx.Queue [Sbx].Msg.Last == IdxCur )
            {
                sxr_Sbx.Queue [Sbx].Msg.Last = IdxPrev;
                sxr_Sbx.Env [IdxPrev].Next  = SXR_NO_ENV;
            }
            else
            {
                sxr_Sbx.Env [IdxPrev].Next = sxr_Sbx.Env [IdxCur].Next;
            }

            sxr_Sbx.Env [IdxCur].Next = sxr_Sbx.EnvIdxFree;
            sxr_Sbx.EnvIdxFree = IdxCur;
        }
        else
        {
            IdxPrev = IdxCur;
            IdxCur  = sxr_Sbx.Env [IdxCur].Next;
        }
    }

    sxr_ExitSc (Status);

    return MsgFound;
}

// =============================================================================
// sxr_SetSemaCallerAddr
// -----------------------------------------------------------------------------
/// Set the caller address for the semaphore. For debug only.
/// @param Id   Semaphore Id.
/// @param CallerAddr   Caller Address.
// =============================================================================
void sxr_SetSemaCallerAddr (u8 Id, u32 CallerAddr)
{
    for (u32 i=0; i<SXR_NB_MAX_SEM; i++)
    {
        if (sxr_SemaInfo[i].InUse && sxr_SemaInfo[i].SemaId == Id)
        {
            sxr_SemaInfo[i].CallerAddr = CallerAddr;
            break;
        }
    }
}

// =============================================================================
// sxr_TakeSemaphore
// -----------------------------------------------------------------------------
/// Take the semaphore.
/// @param Id   Semaphore Id.
// =============================================================================
void sxr_TakeSemaphore (u8 Id)
{
    sxr_Wait (NIL, Id
    #ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
    , COS_WAIT_FOREVER
    #endif
    );
    
    for (u32 i=0; i<SXR_NB_MAX_SEM; i++)
    {
        if (sxr_SemaInfo[i].InUse && sxr_SemaInfo[i].SemaId == Id)
        {
            sxr_SemaInfo[i].CallerTask = sxr_Task.Active;
            SXS_GET_RA(&sxr_SemaInfo[i].CallerAddr);
            break;
        }
    }
}

// =============================================================================
// sxr_ReleaseSemamphore
// -----------------------------------------------------------------------------
/// Release a semaphore.
/// @param Id   Semaphore Id.
// =============================================================================
void sxr_ReleaseSemaphore (u8 Id)
{
    for (u32 i=0; i<SXR_NB_MAX_SEM; i++)
    {
        if (sxr_SemaInfo[i].InUse && sxr_SemaInfo[i].SemaId == Id)
        {
            sxr_SemaInfo[i].CallerTask = SXR_NO_TASK;
            sxr_SemaInfo[i].CallerAddr = NIL;
            break;
        }
    }

    sxr_Send (NIL, Id, SXR_SEND_MSG | SXR_QUEUE_LAST);
}

// =============================================================================
// sxr_SemaphoreHot
// -----------------------------------------------------------------------------
/// Check if a semaphore is free.
/// @param Id   Semaphore Id.
/// @return     TRUE when the semaphore is free, FALSE when it is hot.
// =============================================================================
u8 sxr_SemaphoreHot (u8 Id)
{
    return (sxr_Sbx.Queue [Id].Msg.First != SXR_NO_ENV);
}

// =============================================================================
// sxr_SemaphoreCount
// -----------------------------------------------------------------------------
/// Get current count of semaphore 
/// @param Id   Semaphore Id.
/// @return     
// =============================================================================
s8 sxr_SemaphoreGetValue(u8 id)
{
    s8 count = 0; 
    u32 Status = sxr_EnterSc ();
    
    u16 first = sxr_Sbx.Queue [id].Msg.First; 
    while (SXR_NO_ENV != first) {
        count ++; 
        first = sxr_Sbx.Env [first].Next; 
    } 
    /* No available env, count waiting task on the semaphore */
    if (0 == count) {
        u8 NextTsk = sxr_Sbx.Queue [id].Task.First;
        while (SXR_NO_TASK != NextTsk) {
            count --; 
            NextTsk = sxr_Sbx.Queue [NextTsk].Task.Next;
        }
    }

    sxr_ExitSc(Status);

    return count; 
}


// =============================================================================
// sxs_FuncName
// -----------------------------------------------------------------------------
/// Scope
/// @param 
/// @return
// =============================================================================

