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



/* FILE NAME                                                                  */
/*      cos_win32.c                                                               */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      The COS API definitions on CSW.                                       */
/*      See CSW developement Manual for more.                                 */
/*                                                                            */
/******************************************************************************/


#include "stdafx.h"
#include "cos.h"
#include "event.h"
#include "ts.h"
#include <time.h>

//Enable task debug
//#define  COS_WIN32_TASK_DEBUG

static const UINT8 *cos_task_name[TOTAL_MODULES] =
{
    "NIL",      "SYS",      "APP",      "MED",
#if defined(BT_SUPPORT)
    "BT",
#endif /* BT_SUPPORT */
    "MULT",            // Multimedia Task
#ifdef CFW_TCPIP_SUPPORT
    "APS",
#endif
#if defined(WIFI_SUPPORT)
    "WIFI",
    "W_REC",
#endif /* WIFI_SUPPORT */
#ifdef CFW_TCPIP_SUPPORT
    "S_TST", //For test socket performance
#endif /* CFW_TCPIP_SUPPORT */
#ifdef __WIFI_RADIO_BOX__
    "R_BOX",
#endif /* __WIFI_RADIO_BOX__ */
#ifdef IP_CAMERA_SUPPORT
    "CAM",
#endif /* IP_CAMERA_SUPPORT */

};

#define COS_MAX_EVENT           20
#define COS_MAX_TIMERS              100
#define COS_MAX_SEM             10
#define COS_TIMER_UNUSED        0

//base on Zero
#define COS_IS_VAIL_SEM(s)     ((s) > 0 && (s) < COS_MAX_SEM)

typedef struct
{
    UINT8 InUse;
    HANDLE_WIN32 SemaId;
    UINT8 CallerTask;
    UINT8 count;
    UINT32 CallerAddr;
} COS_SemaInfo_t;

static struct t_cosTimerInfo
{
    UINT8 flags;
    void (*callbackFunc)(void *);
    void *callbackParam;
} cos_timers[COS_MAX_TIMERS];

static COS_MOD_ID active_mod = MOD_APP; //Start app task first in simulator
static COS_EVENT cos_events[TOTAL_MODULES][COS_MAX_EVENT];
static int cos_event_count[TOTAL_MODULES] = {0};
static HANDLE_WIN32 hTaskEvent[TOTAL_MODULES] = {0};
static HANDLE_WIN32 g_tskTaskList[TOTAL_MODULES] = { 0};
static COS_SemaInfo_t cos_SemaInfo[COS_MAX_SEM] = {0};//base on Zero


BOOL COS_FreeEvent(COS_EVENT* pEvent);
static void COS_HandleTimerOutEX(UINT16 timer_id);

HANDLE_WIN32 SRVAPI COS_CreateTask(
    PTASK_ENTRY pTaskEntry,
    UINT8 mod,
    PVOID pParameter,
    PVOID pStackAddr,
    UINT16 nStackSize,
    UINT8 nPriority,
    UINT16 nCreationFlags,
    UINT16 nTimeSlice,
    PCSTR pTaskName
)
{
    DWORD ThreadID;

    hTaskEvent[mod]  =CreateEvent(NULL,FALSE,FALSE,NULL);
    g_tskTaskList[mod] =  (HANDLE_WIN32)CreateThread(NULL,0,pTaskEntry,NULL,0,&ThreadID);

    return g_tskTaskList[mod];
}

/******************************************************************************
 * COS_TaskSwap:
 * DESCRIPTION: - First ActiveNext task, then Block cur_mod with wait sem
 *
 * Input: wait_sem - which current task wait for
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL COS_TaskSwap(const COS_MOD_ID cur_mod, const HANDLE_WIN32 wait_sem)
{
    UINT8 i;
    COS_MOD_ID block_mod;
    HANDLE_WIN32 send_sem;
    static BOOL lock = FALSE;
    static HANDLE_WIN32 cos_task_wait_sem[TOTAL_MODULES] = {0};

#if defined(COS_WIN32_TASK_DEBUG)
    UINT8 buffer1[20] = {0};
    UINT8 buffer2[100] = {0};
#endif /* COS_WIN32_TASK_DEBUG */

    if(!lock)
    {
        lock = TRUE;

    Check_sem_again:
        //Check sem release
        for(i = 1; i < COS_MAX_SEM; i++)
        {
            if((0 != cos_SemaInfo[i].count) && (1 == cos_SemaInfo[i].InUse)
               && MOD_NIL != cos_SemaInfo[i].CallerTask)
            {
                block_mod = cos_SemaInfo[i].CallerTask;
                ASSERT(g_tskTaskList[block_mod] != 0);//CallerTask Must be setup
                cos_SemaInfo[i].CallerTask = MOD_NIL;
                cos_SemaInfo[i].count = 0;
                send_sem = cos_SemaInfo[i].SemaId;
#if defined(COS_WIN32_TASK_DEBUG)
                app_trace(1, "Sem %d release",send_sem);
#endif /* COS_WIN32_TASK_DEBUG */
                goto TaskSwap;
            }
        }

        //Check events
        block_mod = cur_mod;
        while(block_mod++)
        {
            if((block_mod <= MOD_NIL) || (block_mod >= TOTAL_MODULES))
            {
                block_mod = MOD_NIL + 1;
            }

            //Turn to current task again
            if(block_mod == cur_mod)
            {
                if(cos_event_count[block_mod] != 0)
                {
                    lock = FALSE;
                    return TRUE;
                }
                else
                    goto Check_sem_again;
            }
            //If task is setup and wait for event, and NOT block by other semaphone
            else if((0 != g_tskTaskList[block_mod]) &&(cos_event_count[block_mod] != 0)
                    &&(cos_task_wait_sem[block_mod] == hTaskEvent[block_mod]))
            {
                break;
            }

        }

        send_sem = hTaskEvent[block_mod];

    TaskSwap:

#if defined(COS_WIN32_TASK_DEBUG)

        for(i = 0; i < TOTAL_MODULES; i++)
        {
            if(i == block_mod)
                sprintf(buffer1, "\t-%s", cos_task_name[i]);
            else if(i == cur_mod)
                sprintf(buffer1, "\t+%s", cos_task_name[i]);
            else
                sprintf(buffer1, "\t%s", cos_task_name[i]);

            strcat(buffer2,buffer1);
        }
        app_trace(1, "%s", buffer2);

        memset(buffer2, 0, sizeof(buffer2));
        for(i = 0; i < TOTAL_MODULES; i++)
        {
            if(0 != g_tskTaskList[i])
            {
                if(i == block_mod)
                    sprintf(buffer1, "\t%d-%d", cos_task_wait_sem[i], send_sem);
                else if(i == cur_mod)
                    sprintf(buffer1, "\t%d+%d", cos_task_wait_sem[i], wait_sem);
                else
                    sprintf(buffer1, "\t%d", cos_task_wait_sem[i]);
            }
            else //Task is NOT create
            {
                sprintf(buffer1, "\tx(%d)",cos_task_wait_sem[i]);
            }
            strcat(buffer2,buffer1);
        }
        app_trace(1, "%s", buffer2);

        app_trace(1, "Sem %d active task %s \t Sem %d block task %s\n",
                  send_sem, cos_task_name[block_mod],
                  wait_sem, cos_task_name[cur_mod]);

#endif /* COS_WIN32_TASK_DEBUG */

        ASSERT(cos_task_wait_sem[cur_mod] == 0);
        ASSERT(cos_task_wait_sem[block_mod] == send_sem);

        cos_task_wait_sem[cur_mod] = wait_sem;
        cos_task_wait_sem[block_mod] = 0;


        active_mod = block_mod;

        SetEvent(send_sem);
    }
    else    //Other task is runing, wait for wait_sem directly
    {
#if defined(COS_WIN32_TASK_DEBUG)
        app_trace(1, "COS_TaskSwap LOCKED, Block task %s, sem %d\n", cos_task_name[cur_mod], wait_sem);
#endif /* COS_WIN32_TASK_DEBUG */

        ASSERT(cos_task_wait_sem[cur_mod] == 0);
        cos_task_wait_sem[cur_mod] = wait_sem;
    }

    if(WaitForSingleObject(wait_sem,INFINITE) != WAIT_OBJECT_0)
    {
        ASSERT(0);
        return FALSE;
    }

    //When wake up current task, unlock swap task
    lock = FALSE;

    return TRUE;
}

BOOL SRVAPI COS_WaitEvent(
    COS_MOD_ID mod,
    COS_EVENT* pEvent,
    UINT32 nTimeOut
)
{
    int i;
    memset(pEvent, 0 ,sizeof(COS_EVENT));

    //No event or runing other task currently
    while((cos_event_count[mod]<=0) || (active_mod != mod))
    {
        COS_TaskSwap(mod, hTaskEvent[mod]);
    }

    if(cos_event_count[mod] > 0)
    {
        *pEvent = cos_events[mod][0];
        cos_event_count[mod] --;

        for(i=0; i<cos_event_count[mod]; i++)
            cos_events[mod][i] = cos_events[mod][i+1];
    }

    return TRUE;
}

BOOL SRVAPI COS_SendEvent(
    COS_MOD_ID mod,
    COS_EVENT* pEvent,
    UINT32 nTimeOut,
    UINT16 nOption
)
{
    if(cos_event_count[mod] +1 >= COS_MAX_EVENT)
    {
#if defined(COS_WIN32_TASK_DEBUG)
        app_trace(1, "COS_SendEvent to %s FAIL,event_id=%d\n", cos_task_name[mod], pEvent->nEventId);
#endif /* COS_WIN32_TASK_DEBUG */
        return FALSE;
    }

    cos_events[mod][cos_event_count[mod]++] = *pEvent ;
    return TRUE;
}

VOID COS_SendMessageEvent(COS_MOD_ID mod_id, UINT32 event, UINT32 param)
{
    COS_EVENT ev;
    ev.nEventId = event;
    ev.nParam1 = param;

    COS_SendEvent(mod_id, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

BOOL SRVAPI COS_DeleteTask(
#ifndef MMI_ON_WIN32
    HANDLE hTask
#else
    HANDLE_WIN32 hTask
#endif
)
{
    return FALSE;
}

UINT32 COS_SuspendTask(
#ifndef MMI_ON_WIN32
    HANDLE hTask
#else
    HANDLE_WIN32 hTask
#endif
)
{
    return 0;
}

BOOL COS_ResumeTask (
    COS_MOD_ID mod
)
{
    return FALSE;
}

HANDLE_WIN32 Cos_GetTaskHandler(COS_MOD_ID task_id)
{
    return (HANDLE_WIN32) g_tskTaskList[task_id];
}


BOOL SRVAPI COS_ResetEventQueue(
#ifndef MMI_ON_WIN32
    HANDLE hTask
#else
    HANDLE_WIN32 hTask
#endif
)
{
    return FALSE;
}

BOOL SRVAPI COS_IsEventAvailable(
#ifndef MMI_ON_WIN32
    HANDLE hTask
#else
    HANDLE_WIN32 hTask
#endif
)
{
    return TRUE;
}

HANDLE SRVAPI COS_CreateSemaphore (
    UINT32  nInitCount  // Specify the initial count of the semaphore
)
{
    UINT8 i;

    for(i = 1; i < COS_MAX_SEM; i++)
    {
        if(0 == cos_SemaInfo[i].InUse)
        {
            cos_SemaInfo[i].InUse = 1;
            cos_SemaInfo[i].SemaId = CreateEvent(NULL,FALSE,FALSE,NULL);
            cos_SemaInfo[i].CallerTask = 0;
            cos_SemaInfo[i].count = 0;
            cos_SemaInfo[i].CallerAddr= 0;
            return (HANDLE) i;
        }
    }

    return 0;
}

BOOL SRVAPI COS_DeleteSemaphore(
#ifndef MMI_ON_WIN32
    HANDLE hSem
#else
    HANDLE_WIN32 hSem
#endif
)
{
    UINT8 i = (UINT8) hSem;

    if(!COS_IS_VAIL_SEM(i))
        ASSERT(0);


    if(1 == cos_SemaInfo[i].InUse)
    {
        cos_SemaInfo[i].InUse = 0;
        cos_SemaInfo[i].SemaId = 0;
        cos_SemaInfo[i].CallerTask = 0;
        cos_SemaInfo[i].count = 0;
        cos_SemaInfo[i].CallerAddr= 0;
        return TRUE;
    }

    return FALSE;
}

BOOL SRVAPI COS_WaitForSemaphore(
#ifndef MMI_ON_WIN32
    HANDLE hSem, // Specify the handle to a counting semaphore
#else
    HANDLE_WIN32 hSem,
#endif
    UINT32 nTimeOut  // the time-out value
)
{
    HANDLE wait_sem;
    COS_MOD_ID block_mod = active_mod;

    if(!COS_IS_VAIL_SEM(hSem))
        ASSERT(0);

    cos_SemaInfo[hSem].CallerTask = block_mod;
    cos_SemaInfo[hSem].count = 0;
    wait_sem = cos_SemaInfo[hSem].SemaId;

    return COS_TaskSwap(block_mod, wait_sem);
}

BOOL SRVAPI COS_ReleaseSemaphore(
#ifndef MMI_ON_WIN32
    HANDLE hSem //Specify the counting semaphore
#else
    HANDLE_WIN32 hSem
#endif
)
{
    if(!COS_IS_VAIL_SEM(hSem))
        ASSERT(0);

    cos_SemaInfo[hSem].count++;
    return TRUE;
}


void SetSimTimer(int id, int nMillisecondes);
void KillSimTimer(int id);

UINT8 SRVAPI COS_SetTimer (
    UINT32 nMillisecondes,
    void(*callbackFunc)(void *),
    void *callbackArg,
    UINT8 flags
)
{
    int i;

    struct t_cosTimerInfo *newEntry;

    for(i=1; i<COS_MAX_TIMERS; i++)
    {
        if(cos_timers[i].flags == COS_TIMER_UNUSED)
        {
            newEntry = &(cos_timers[i]);
            break;
        }
    }

    if (i==COS_MAX_TIMERS)
    {
        ASSERT(0);
        return 0;
    }

    newEntry->callbackFunc=callbackFunc;
    newEntry->callbackParam=callbackArg;
    newEntry->flags=flags;

    SetSimTimer(i, nMillisecondes);
    return i;
}


BOOL SRVAPI COS_ChangeTimer(UINT8 timerHandle, UINT32 nMillisecondes)
{
    if(timerHandle !=0 && timerHandle <= COS_MAX_TIMERS)
    {
        struct t_cosTimerInfo *p_timer = &cos_timers[timerHandle];

        SetSimTimer(timerHandle, nMillisecondes);
        return TRUE;
    }
    else
        return FALSE;
}

BOOL SRVAPI COS_ChangeTimerUseSecond(UINT8 timerHandle, UINT32 secondes)
{
    if(timerHandle !=0 && timerHandle <= COS_MAX_TIMERS)
    {
        struct t_cosTimerInfo *p_timer = &cos_timers[timerHandle];

        SetSimTimer(timerHandle, secondes*1000);
        return TRUE;
    }
    else
        return FALSE;
}

BOOL SRVAPI COS_KillTimer (
    UINT8 nTimerId
)
{
    if(nTimerId !=0 && nTimerId <= COS_MAX_TIMERS && cos_timers[nTimerId].flags != COS_TIMER_UNUSED)
    {
        cos_timers[nTimerId].flags = COS_TIMER_UNUSED;
        KillSimTimer(nTimerId);
        return TRUE;
    }
    else
        return FALSE;

}

BOOL COS_KillTimer_Ex(void(*callbackFunc)(void *))
{
    int i;

    for(i=1; i<COS_MAX_TIMERS; i++)
    {
        if(cos_timers[i].callbackFunc == callbackFunc)
        {
            break;
        }
    }
    if (i==COS_MAX_TIMERS)
    {
        ASSERT(0);
    }

    return COS_KillTimer(i);
}


void COS_HandleTimeout(UINT8 nTimerId)
{
    if(nTimerId >0 && nTimerId <= COS_MAX_TIMERS && cos_timers[nTimerId].flags != COS_TIMER_UNUSED)
    {
        if(cos_timers[nTimerId].flags == COS_TIMER_MODE_SINGLE)
        {
            cos_timers[nTimerId].flags = COS_TIMER_UNUSED;
            KillSimTimer(nTimerId);
        }

        if(cos_timers[nTimerId].callbackFunc)
        {
            cos_timers[nTimerId].callbackFunc(cos_timers[nTimerId].callbackParam);
        }
    }
    else
    {
    	COS_HandleTimerOutEX(nTimerId);
    }
}


BOOL SRVAPI COS_Sleep (
    UINT32 nMillisecondes
)
{
    Sleep(nMillisecondes);
    return TRUE;
}

UINT32 SRVAPI COS_GetTickCount( VOID)
{
    DWORD t;

    t = GetTickCount();

    return t;
}

BOOL SRVAPI COS_SetPriority (
#ifndef MMI_ON_WIN32
    HANDLE hTask,
#else
    HANDLE_WIN32 hTask,
#endif
    UINT8 nPriority
);

HANDLE SRVAPI COS_EnterCriticalSection (
    VOID
)
{
    return 0;
}

BOOL SRVAPI COS_ExitCriticalSection (
#ifndef MMI_ON_WIN32
    HANDLE hSection
#else
    HANDLE_WIN32 hSection
#endif
)
{
    return FALSE;
}

UINT32 SRVAPI COS_GetLastError(VOID);

VOID SRVAPI COS_SetLastError(
    UINT32 nErrCode   // per-thread error code
);

#ifdef _DEBUG
PVOID COS_MallocDbg (UINT32 nSize, CONST UINT8* pszFileName, UINT32 nLine)
{
    /*app_trace(TRUE,_T("%s,%d\n"),pszFileName,nLine);*/
    return malloc(nSize);
}

BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine)
{
    /*app_trace(TRUE,_T("%s,%d\n"),pszFileName,nLine);*/
    if (pMemAddr)
    {
        free(pMemAddr);
    }
    return TRUE;
}

#define COS_MALLOC(size) COS_MallocDbg((UINT32)size, __FILE__, __LINE__)
#define COS_FREE(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)
#else
#define COS_MALLOC(size) COS_Malloc((UINT32)size)
#define COS_FREE(p)      COS_Free((PVOID)p)
#endif

PVOID SRVAPI COS_Malloc (
    UINT32 nSize
)
{
    return malloc(nSize);
}

PVOID SRVAPI COS_Malloc_NoCache (
    UINT32 nSize
)
{
    return malloc(nSize);
}

BOOL SRVAPI COS_Free (
    PVOID pMemBlock
)
{
    if (pMemBlock)
    {
        free(pMemBlock);
    }
    return TRUE;
}

#define COS_EX_TIMER_BASE (COS_MAX_TIMERS + 1)
#define COS_EX_TIMER_MAX 100
typedef struct
{
    UINT8 flags;
    COS_MOD_ID  dst_mod_id;
    UINT16      timer_id;
} COS_EX_TIMER_T,*COS_EX_TIMER_PTR;

static COS_EX_TIMER_T cos_ex_timers[COS_EX_TIMER_MAX]={0};

static void COS_HandleTimerOutEX(UINT16 timer_id)
{
	if((timer_id > COS_EX_TIMER_BASE) && (timer_id < COS_EX_TIMER_BASE + COS_EX_TIMER_MAX))
	{
	    COS_EVENT ev;
		UINT16 ex_timer_id = timer_id - COS_EX_TIMER_BASE;

		if(cos_ex_timers[ex_timer_id].flags!= COS_TIMER_UNUSED)
		{
	        if(cos_ex_timers[ex_timer_id].flags == COS_TIMER_MODE_SINGLE)
	        {
	            cos_ex_timers[ex_timer_id].flags = COS_TIMER_UNUSED;
	            KillSimTimer(timer_id);
	        }

#if defined(COS_WIN32_TASK_DEBUG)
		   	app_trace(1, "COS_HandleTimerOutEX MOD=%s, id=%d ", 
		   			cos_task_name[cos_ex_timers[ex_timer_id].dst_mod_id], 
		   			cos_ex_timers[ex_timer_id].timer_id);
#endif /* COS_WIN32_TASK_DEBUG */


		    ev.nEventId = EV_TIMER;
		    ev.nParam1 = cos_ex_timers[ex_timer_id].timer_id;

		    COS_SendEvent(cos_ex_timers[ex_timer_id].dst_mod_id, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	    }
	}
}

BOOL  SRVAPI COS_KillTimerEX (
    HANDLE hTask,
    UINT16 nTimerId
)
{
	if((nTimerId > 0) && (nTimerId < COS_EX_TIMER_MAX))
	{
	    cos_ex_timers[nTimerId].flags = COS_TIMER_UNUSED;
        KillSimTimer(nTimerId + COS_EX_TIMER_BASE);
	    return TRUE;
	}
    return FALSE;
}

BOOL SRVAPI COS_SetTimerEX (
    HANDLE hTask,
    UINT16 nTimerId,
    UINT8 nMode,
    UINT32 nMillisecondes
)
{
    COS_MOD_ID i;
    UINT16 id;

    for(i = MOD_NIL + 1; i < TOTAL_MODULES; i++)
    {
        if(g_tskTaskList[i] == hTask)
        {
		    for(id=1; id<COS_EX_TIMER_MAX; id++)
		    {
		        if(COS_TIMER_UNUSED == cos_ex_timers[id].flags)
		        {
#if defined(COS_WIN32_TASK_DEBUG)
	            	app_trace(1, "COS_SetTimerEX =%d ",id);
#endif /* COS_WIN32_TASK_DEBUG */

	            	cos_ex_timers[id].timer_id = nTimerId;
	            	cos_ex_timers[id].dst_mod_id= i;
				    cos_ex_timers[id].flags = nMode;

			        SetSimTimer(id + COS_EX_TIMER_BASE, nMillisecondes);
				    return TRUE;
		        }
		    }
            break;
        }
    }

    return FALSE;
}

VOID  COS_UpdateMemCallAdd( VOID *pMemeAddr, UINT32 nCallAdd)
{
    return;
}

INT32 SUL_Wcsicmp(const UINT16* cs, const UINT16* ct)
{

    return (*cs) - (*ct);

}

UINT32 COS_GetLastError(VOID)
{
    return 0;
}

UINT32 hal_SysEnterCriticalSection(VOID)
{
    return 0;
}
VOID hal_SysExitCriticalSection(UINT32 status)
{
    return;
}

UINT8 COS_GetCurrentTaskId(VOID)
{
	return MOD_APP;
}

COS_MOD_ID COS_GetCurrentTaskModId(VOID)
 {
	return MOD_APP;
}

#if 0
UINT32 COS_PageProtectDisable( COS_PAGE_NUM nPageNum )
{
    return 0;
}
UINT32 COS_PageProtectEnable( COS_PAGE_NUM nPageNum )
{
    return 0;
}
UINT32 COS_PageProtectSetup( COS_PAGE_NUM nPageNum, COS_PAGE_SPY_MODE nMode, UINT32 nStartAddr, UINT32 nEndAddr )
{
    return 0;
}
#endif

BOOL SRVAPI COS_Free_BTOverlay(PVOID ptr)
{
    return TRUE;
}

PVOID SRVAPI COS_Malloc_BTOverlay( UINT32 size)
{
    return TRUE;
}
