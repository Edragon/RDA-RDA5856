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


/******************************************************************************/
/* Filename:    cos_task.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*   COS implementation.                                                      */
/******************************************************************************/
//#include <csw.h>
//#include <stdkey.h>
#include <event.h>
#include "cos.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

#include "csw_csp.h"

extern int sprintf(char *buf, const char *fmt, ...);
#define COS_ASSERT(expr) \
				do { \
					char assertion_info[256] = {0}; \
					sprintf(assertion_info, "[COS_ASSERT]FILE:%s FUN:%s LN:%d", __FILE__, __FUNCTION__, __LINE__); \
				} while (0)

//DBG_ASSERT(expr, assertion_info); \

#define IS_VALID_MAILBOX_ID(x)  TRUE 

#define  MAX_TASK_NAME_LEN        16
static TASK_HANDLE g_tskTaskList[TOTAL_MODULES];

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

    pHTask->queue                 = xQueueCreate(50, ( UBaseType_t ) sizeof(COS_EVENT));
    xTaskCreate(pTaskEntry, pHTask->sTaskDesc.Name, nStackSize, ( void * ) pParameter, nPriority,  &pHTask->taskHandle);
    pHTask->taskMod = mod;

    
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

}


//
// Stop task. 
//
VOID COS_StopTask(
  TASK_HANDLE *pHTask
)
{

}

//
// Create task, only apply to CSW internal.
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
    TASK_HANDLE* pHTask = NULL;


    return (HANDLE)0;

}

//
// Delete Task. Release Mailbox, task, and the handle.
//
BOOL COS_DeleteTask(
  COS_MOD_ID mod
)
{
    TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
    

    vTaskDelete((void*)pHTask->taskHandle);
    if(pHTask->queue)
    {
    	vQueueDelete(pHTask->queue);
    	pHTask->queue = NULL;
    }
    pHTask->taskHandle = NULL;

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

    return TRUE;
}

BOOL COS_SetExclusiveRunningTask(COS_MOD_ID mod, BOOL enable)
{
	TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
	pHTask->exclusive = enable;
}

BOOL COS_IsExclusiveRunningTask(COS_MOD_ID mod)
{
	TASK_HANDLE* pHTask  = &(g_tskTaskList[mod]);
	return pHTask->exclusive;
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

	if(nTimeOut == COS_WAIT_FOREVER)
	{

		if(!pHTask->exclusive)
		{
			xQueueReceive(pHTask->queue, &Evt, nTimeOut);
		}
		else
		{
			if(pEvent)
			{
				memset(pEvent, 0, sizeof(COS_EVENT));
				pEvent->nEventId = EV_OS_EXCLUSIVE;
			}
			return TRUE;
		}

	}
	else
		return FALSE;

	if( Evt[0] )
	{
		ret = TRUE;
	}
	else
		ret = FALSE;

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

    if(pHTask->queue)
    {
    	if(nOption == COS_EVENT_PRI_URGENT)
    	{
    		//xQueueGenericSend(pHTask->queue, ( void * )pEvent,  nTimeOut, queueSEND_TO_FRONT);
    		if(xQueueSendToFrontFromISR(pHTask->queue, ( void * )pEvent, NULL) == FALSE)
    		{
    			printf("xQueueSendToFrontFromISR failed %d\n\r", pHTask->taskMod);
    		}
    	}
    	else
    	{
    		if(xQueueSend(pHTask->queue, ( void * )pEvent,  nTimeOut) == FALSE)
    		{
    			printf("xQueueSend failed %d\n\r", pHTask->taskMod);
    		}
    	}
    }
    else
    {
    	return FALSE;
    }
    return TRUE;

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
	UINT32 Evt [4] = {0, 0, 0, 0};
    if(pHTask == NULL)
        return FALSE;

    return xQueuePeek(pHTask->queue, Evt, 0);

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
	SemaphoreHandle_t handle = xSemaphoreCreateCounting(countMAX_COUNT_VALUE, nInitCount);
	if(handle)
	{
		vQueueAddToRegistry( ( QueueHandle_t )handle, NULL);
	}
	return (HANDLE) handle;

}

BOOL SRVAPI COS_DeleteSemaphore(
    HANDLE hSem
)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
    	return FALSE;
    }
    
    vSemaphoreDelete((SemaphoreHandle_t)hSem);
    return TRUE;
}

BOOL SRVAPI COS_WaitForSemaphore(
    HANDLE hSem,     // Specify the handle to a counting semaphore 
    UINT32 nTimeOut  // the time-out value
)
{
    if(hSem == 0)//NULL)modified by fengwei 20080917 for warning reduce
    {
    	return FALSE;
    }


    if(xSemaphoreTake((SemaphoreHandle_t)hSem, nTimeOut) != TRUE)
    {
    	return FALSE;
    }

//	  CSW_TRACE(9, "COS_WaitForSemaphore test_cont:0x%x \n",test_cont);
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


	if(xSemaphoreGive((SemaphoreHandle_t)hSem) != TRUE)
	{
		return FALSE;
	}

    return TRUE;
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

UINT8 COS_GetSysTaskMbx(VOID)
{

}

UINT8 MultiGetSysTaskMbx(VOID)
{

}


HANDLE Cos_GetTaskHandler(COS_MOD_ID task_id)
{
    return (HANDLE) &g_tskTaskList[task_id];
}

UINT8 COS_GetCurrentTaskId(VOID)
{
    COS_MOD_ID i;


    return MOD_NIL;
}

COS_MOD_ID COS_GetCurrentTaskModId(VOID)
{
   return MOD_NIL; 
}

HANDLE COS_CreateMutex(VOID)
{
	return  (HANDLE)xQueueCreateMutex(0);
}

UINT8 COS_GetMutex(HANDLE handle, UINT32 nTimeOut)
{
	return xQueueTakeMutexRecursive((VOID*)handle, nTimeOut);
}

UINT8 COS_ReleaseMutex(HANDLE handle)
{
	return xQueueGiveMutexRecursive((VOID*)handle);
}

VOID COS_DeleteMutex(HANDLE handle)
{
	printf("COS_DeleteMutex\n");
    vSemaphoreDelete(handle);
}
