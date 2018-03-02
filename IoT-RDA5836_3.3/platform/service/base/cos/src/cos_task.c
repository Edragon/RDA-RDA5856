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


//#include <csw.h>
//#include <stdkey.h>
#include <event.h>
#include "cos.h"
#include "ts.h"
#include <base_prv.h>
#include<sxr_tksd.h>
#include<sxr_tim.h>
#include "sxr_sbx.h"
#include "dbg.h"
#include "csw_csp.h"
#include "string.h"
#include "stdio.h"


#define __FUNCTION__    ("ads 1.2 does not support")


extern int sprintf(char *buf, const char *fmt, ...);
#define COS_ASSERT(expr) \
            do { \
                    char assertion_info[256] = {0}; \
                    sprintf(assertion_info, "[COS_ASSERT]FILE:%s FUN:%s LN:%d", __FILE__, __FUNCTION__, __LINE__); \
                } while (0)

//DBG_ASSERT(expr, assertion_info); 

#define IS_VALID_MAILBOX_ID(x)  TRUE 

#define  MAX_TASK_NAME_LEN        16
static TASK_HANDLE g_tskTaskList[TOTAL_MODULES];

extern BOOL hasFreeEnv(UINT8 nMbxId);
extern VOID sxr_Sleep (UINT32 Period);
extern BOOL COS_SleepByTick (UINT32 Period);
extern TASK_ENTRY BAL_MainTaskEntry(void *pData);
PRIVATE HANDLE g_hCosMmiTask = HNULL; // the default MMI task.

HANDLE COS_GetDefaultMmiTaskHandle(VOID)
{
    COS_ASSERT(g_hCosMmiTask != HNULL);
    return g_hCosMmiTask;
}

//
// Create task. Apply to upper layer MMI.
//
HANDLE COS_CreateTask(
  PTASK_ENTRY pTaskEntry,
  COS_MOD_ID mod,
  PVOID pParameter,
  PVOID pStackAddr,
  UINT16 nStackSize,
  UINT8 nPriority,
  UINT16 nCreationFlags,
  UINT16 nTimeSlice,
  const INT8* pTaskName
)
{
    TASK_HANDLE* pHTask = NULL;
    
//    if(nPriority < COS_PRI_BASE || nPriority > COS_PRI_BK_BASE)
//        return HNULL;
	if(MOD_CUSTOMER_AUTO == mod)
	{
		UINT16 i;
		
		for(i = MOD_CUSTOMER_START; i < MOD_CUSTOMER_END; i++)
		{
			 if(NULL == g_tskTaskList[i].sTaskDesc.TaskBody)
		 	{
		 		mod = i;
		 		break;
		 	}
		}

		if(MOD_CUSTOMER_END == i )
		{
			return (HANDLE)NULL;
		}
	}

    pHTask = &(g_tskTaskList[mod]);

    pHTask->sTaskDesc.Name = pTaskName;
    if(pTaskName == 0)
    {
        pHTask->sTaskDesc.Name = "noname";
    }

    pHTask->sTaskDesc.nPriority   = nPriority;
    pHTask->sTaskDesc.nStackSize  = nStackSize;
    pHTask->sTaskDesc.TaskBody    = pTaskEntry;
//    pHTask->sTaskDesc.TaskExit    = NULL;
    pHTask->nMailBoxId            = sxr_NewMailBox();
    pHTask->nTaskId               = sxr_NewTask ((sxr_TaskDesc_t*)&pHTask->sTaskDesc);
    
    if(nCreationFlags&COS_CREATE_SUSPENDED)
    {
        return (HANDLE)NULL; 
    }
    else if(nCreationFlags&COS_CREATE_PREEMPT)
    {
        return (HANDLE)NULL;  
    }
    else if(nCreationFlags&COS_CREATE_NO_PREEMPT)
    {
        return (HANDLE)NULL;     
    }
    else 
    {
        sxr_StartTask (pHTask->nTaskId, pParameter);
    }

    //if( (UINT32)pTaskEntry == (UINT32)BAL_MainTaskEntry )
    if ( MOD_APP ==  mod)
    {
        g_hCosMmiTask = (HANDLE)MOD_APP;//pHTask;;
    }
    
    return (HANDLE)pHTask; 
}

//
// Start task. Start a suspended task(never start over).
//
VOID COS_StartTask(
  TASK_HANDLE *pHTask,
  PVOID pParameter
)
{
  sxr_StartTask (pHTask->nTaskId, pParameter);
}


//
// Stop task. 
//
VOID COS_StopTask(
  TASK_HANDLE *pHTask
)
{
  sxr_StopTask (pHTask->nTaskId);
}

//
// Create task without MOD_ID.
//
HANDLE COS_CreateTask_Prv(
  PTASK_ENTRY pTaskEntry,
  PVOID pParameter,
  PVOID pStackAddr,
  UINT16 nStackSize,
  UINT8 nPriority,
  UINT16 nCreationFlags,
  UINT16 nTimeSlice,
  PCSTR pTaskName
)
{
	return COS_CreateTask(
			  pTaskEntry,
			  MOD_CUSTOMER_AUTO,
			  pParameter,
			  pStackAddr,
			  nStackSize,
			  nPriority,
			  nCreationFlags,
			  nTimeSlice,
			  pTaskName
			);
}

//
// Delete Task. Release Mailbox, task, and the handle.
//
BOOL COS_DeleteTask(
  COS_MOD_ID mod
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
    
    sxr_FreeTask(pHTask->nTaskId);
    sxr_FreeMailBox(pHTask->nMailBoxId);

	memset(pHTask, 0, sizeof(TASK_HANDLE));
    return FALSE;
}

//
// Suspsend Task.
//
UINT32 COS_SuspendTask(
  COS_MOD_ID mod
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);

    sxr_SuspendTask(pHTask->nMailBoxId);
    return TRUE;
}

//
// Resume task.
//
BOOL COS_ResumeTask (
  COS_MOD_ID mod
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);

    sxr_ResumeTask(pHTask->nMailBoxId);
    return TRUE;
}


BOOL COS_WaitEvent(
  COS_MOD_ID mod,
  COS_EVENT* pEvent,
  UINT32 nTimeOut
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
    UINT32 Evt [4] = {0, 0, 0, 0};
    BOOL ret = FALSE;
    extern UINT8 g_rtc_count;


    if(nTimeOut == COS_WAIT_FOREVER)
    {
        if( IS_VALID_MAILBOX_ID(pHTask->nMailBoxId) )
            sxr_Wait(Evt, pHTask->nMailBoxId
            #ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
            , COS_WAIT_FOREVER
            #endif
            );
        else
            return FALSE;
    }
    else
        return FALSE;

    if( Evt[0] )
    {
        if(Evt[0] >= HVY_TIMER_IN )
        {
            if(Evt[1]>>31)
                sxs_StartTimer ( Evt[1]&0x7FFFFFFF, Evt[0], (VOID*)Evt[1], FALSE, pHTask->nMailBoxId );
            Evt[1] = Evt[0]-HVY_TIMER_IN;
            Evt[0] = EV_TIMER;
        }
        ret = TRUE;
    }
    else
        ret = FALSE;

    if(Evt[0]==28/*AP_MSG_RTC*/)
    {
        if(g_rtc_count>0)
            g_rtc_count--;
    }

    pEvent->nEventId = Evt[0];
    pEvent->nParam1  = Evt[1];
#if defined(WIFI_SUPPORT)
    pEvent->nParam2  = Evt[2];
    pEvent->nParam3  = Evt[3];
#endif /* WIFI_SUPPORT */
    //CSW_TRACE(6, ("COS_WaitEvent event id = %d, param1 = %d. \n"), pEvent->nEventId, pEvent->nParam1);

    return ret;
}

VOID COS_SendMessageEvent(COS_MOD_ID mod_id, UINT32 event, UINT32 param)
{
    COS_EVENT ev;
    ev.nEventId = event;
    ev.nParam1 = param;

    COS_SendEvent(mod_id, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

//
// Send message to the specified task.
//
BOOL COS_SendEvent(
  COS_MOD_ID  mod, 
  COS_EVENT* pEvent,
  UINT32 nTimeOut,
  UINT16 nOption
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
    
    if( pHTask == NULL || pEvent== NULL )
    {
        printf("COS_SendEvent pHTask:%p, pEvent:%p\n\r", pHTask, pEvent);
        return FALSE;
    }

    if(nTimeOut == COS_WAIT_FOREVER)
    {
        UINT8 status = SXR_SEND_EVT;
        if( nOption&COS_EVENT_PRI_URGENT )
            status |= SXR_QUEUE_FIRST;
        
        if (!hasFreeEnv(pHTask->nMailBoxId))
        {
            CSW_TRACE(6, ("COS_SendEvent, mid=%d,mod=%d, Hs no Free Env, event id=%d. \n"), pHTask->nMailBoxId,mod,pEvent->nEventId);
            return FALSE;
        }
        sxr_Send(pEvent, pHTask->nMailBoxId, status);
//        CSW_TRACE(6, ("COS_SendEvent event id = %d, param1 = %d. \n"), pEvent->nEventId, pEvent->nParam1);
        
        return TRUE;
    }
    return FALSE;
}


//#define GET_SYSTASK_QUEUE_ID()    (((TASK_HANDLE*)BAL_TH(BAL_SYS_TASK_PRIORITY))->nMailBoxId)

//
// Send message to SYS_Task.
//
BOOL COS_SendSysEvent(
  COS_EVENT* pEvent,
  UINT16 nOption
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[MOD_SYS]);
    UINT8 status = SXR_SEND_EVT;
    
    if( nOption&COS_EVENT_PRI_URGENT )
        status |= SXR_QUEUE_FIRST;
    
    sxr_Send(pEvent,pHTask->nMailBoxId, status);
    return TRUE;
}

BOOL COS_ResetEventQueue(
  HANDLE hTask
)
{
    return FALSE;
}

BOOL COS_IsEventAvailable(
  COS_MOD_ID  mod
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
    //UINT32 Evt [4] = {0, 0, 0, 0};
    if(pHTask == NULL)
        return FALSE;
    if( sxr_SbxHot( pHTask->nMailBoxId ) )
         return TRUE;
    else
        return FALSE;
}


HANDLE COS_EnterCriticalSection (
  VOID
)
{
    return (HANDLE)hal_EnterCriticalSection();
}

BOOL COS_ExitCriticalSection (
  HANDLE hSection
)
{
    hal_ExitCriticalSection(hSection);
    
    return TRUE;
}

HANDLE SRVAPI COS_CreateSemaphore (
     UINT32  nInitCount  // Specify the initial count of the semaphore    sxr_NewSemaphore
)
{
    volatile UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
    UINT8 id = sxr_NewSemaphore((UINT8)nInitCount);
    CSW_TRACE(9, "COS_CreateSemaphore sbx: 0x%x; caller:0x%x \n", id, nCallerAdd);
    return (HANDLE) id;
}

BOOL SRVAPI COS_DeleteSemaphore(
    HANDLE hSem
)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
        return FALSE;
    }
    
    sxr_FreeSemaphore(hSem);
    return TRUE;
}

UINT8 test_cont =10;
BOOL SRVAPI COS_WaitForSemaphore(
    HANDLE hSem,     // Specify the handle to a counting semaphore 
    UINT32 nTimeOut  // the time-out value
)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
        return FALSE;
    }

    sxr_TakeSemaphore(hSem);
    test_cont--;
//    CSW_TRACE(9, "COS_WaitForSemaphore test_cont:0x%x \n",test_cont);
    return TRUE;
}

BOOL SRVAPI COS_ReleaseSemaphore(
    HANDLE hSem //Specify the counting semaphore
)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
        return FALSE;
    }

    volatile  UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
    sxr_ReleaseSemaphore(hSem); //sxr_SemaphoreHot
    test_cont++;
    if(test_cont>30)
    {
        CSW_TRACE(9, "COS_ReleaseSemaphore sbx: 0x%x; caller:0x%x ,test_cont:0x%x\n", hSem, nCallerAdd,test_cont);

        COS_ASSERT(FALSE);
    }
    //      CSW_TRACE(9, "COS_ReleaseSemaphore test_cont:0x%x \n",test_cont);
    return TRUE;
}

int SRVAPI COS_GetSemaphoreCount(HANDLE hSem)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
        return 0;
    }

    return sxr_SemaphoreGetValue(hSem);
}


PRIVATE UINT32 g_TaskLastError[20] = {0, };
PRIVATE UINT32 g_CurTaskId         = 0;     // Need to remember the task id,  TODO... 

UINT32 COS_GetLastError(VOID)
{
    return 0;//g_TaskLastError[g_CurTaskId];
}

VOID COS_SetLastError(
  UINT32 nErrCode   // per-thread error code
)
{
    g_TaskLastError[g_CurTaskId] = nErrCode;
}

UINT8 COS_GetSysTaskMbx(void)
{
    return g_tskTaskList[MOD_SYS].nMailBoxId;
}

UINT8 MultiGetSysTaskMbx(void)
{
    return g_tskTaskList[MOD_MULTI].nMailBoxId;
}


HANDLE Cos_GetTaskHandler(COS_MOD_ID task_id)
{
    return (HANDLE) &g_tskTaskList[task_id];
}

UINT8 COS_GetCurrentTaskId(VOID)
{
    UINT8 sxr_task_id = sxr_GetCurrentTaskId();

    return sxr_task_id; 
}

COS_MOD_ID COS_GetCurrentTaskModId(VOID)
{
    COS_MOD_ID i;

    UINT8 sxr_task_id = sxr_GetCurrentTaskId();

    if(0XFF != sxr_task_id)
    {
        for(i = MOD_NIL + 1; i < TOTAL_MODULES; i++)
        {
            if(g_tskTaskList[i].nTaskId == sxr_task_id)
                return i;
        }
    }

    return MOD_NIL;
}
