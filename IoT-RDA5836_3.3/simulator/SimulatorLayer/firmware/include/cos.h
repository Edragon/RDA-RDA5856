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
/*      cos.h                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      The COS API definitions on CSW.                                       */
/*      See CSW developement Manual for more.                                 */
/*                                                                            */
/******************************************************************************/

#ifndef _COS_H_
#define _COS_H_

#include <cswtype.h>
#include <windows.h>
#include "sxs_io.h"

#define COS_MAX_PRIORITY       20 
#define COS_CREATE_DEFAULT     0
#define COS_CREATE_SUSPENDED   1
#define COS_CREATE_PREEMPT     2
#define COS_CREATE_NO_PREEMPT  3


#define	FREERTOS               1
#define	SXR                    2

#if OS_KERNEL == FREERTOS
#define tskMIN_PRIORITY			0
#define tskMAX_PRIORITY			5
#define mainBLOCK_Q_PRIORITY	( tskMIN_PRIORITY + 2 )
#define countMAX_COUNT_VALUE	( 10 )
#endif

typedef struct _TaskDesc
{
 void (*TaskBody)(void *);
 void (*TaskExit)(void *);
 const char *Name;
 UINT16 nStackSize;
 UINT8 nPriority;
}TaskDesc;

typedef struct _TASK_HANDLE
{
 TaskDesc sTaskDesc;
 UINT8 nTaskId;
 UINT8 nMailBoxId;
 UINT8 padding[4];
}TASK_HANDLE;

/*************************************************************************
 * 3. Component task's module id (Please add before system service)
 *************************************************************************/
typedef enum {
   MOD_NIL = 0,
   MOD_SYS,                 // system task
   MOD_APP ,                // application task
   MOD_MED,                 // media play task
#if defined(BT_SUPPORT)
   MOD_BT,                  // bluetooth task
#endif /* BT_SUPPORT */
   MOD_MULTI,            // Multimedia Task
#ifdef CFW_TCPIP_SUPPORT
   MOD_APS,
#endif
#if defined(WIFI_SUPPORT)
   MOD_WIFI_SERVICE,
   MOD_WIFI_REC,
#endif /* WIFI_SUPPORT */
#ifdef CFW_TCPIP_SUPPORT
   MOD_SOC_TEST, //For test socket performance
#endif /* CFW_TCPIP_SUPPORT */
#ifdef __WIFI_RADIO_BOX__
   MOD_RADIO_BOX,
#endif /* __WIFI_RADIO_BOX__ */
#ifdef IP_CAMERA_SUPPORT
   MOD_IP_CAMERA,
#endif /* IP_CAMERA_SUPPORT */
   TOTAL_MODULES
} COS_MOD_ID;

#ifdef MMI_ON_WIN32
#define TASK_ENTRY      DWORD WINAPI
typedef DWORD(WINAPI *PTASK_ENTRY)(PVOID);
#else
#define TASK_ENTRY      VOID
#endif


#define COS_MMI_TASKS_PRIORITY_BASE 220 // 220-250 is assigned to MMI part.
#define COS_PRIORITY_NUM            30

#ifndef MMI_ON_WIN32
HANDLE SRVAPI COS_CreateTask(
#else	
HANDLE_WIN32 SRVAPI COS_CreateTask(
#endif
  PTASK_ENTRY pTaskEntry,
  UINT8 mod,
  PVOID pParameter,
  PVOID pStackAddr,
  UINT16 nStackSize,
  UINT8 nPriority,
  UINT16 nCreationFlags,
  UINT16 nTimeSlice,
  PCSTR pTaskName
);

BOOL SRVAPI COS_DeleteTask(
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
);

UINT32 COS_SuspendTask(
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
);

BOOL COS_ResumeTask (
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
);

typedef struct _COS_EVENT {
  UINT32 nEventId;
  UINT32 nParam1;
#if defined(WIFI_SUPPORT)||defined(CFW_TCPIP_SUPPORT)
  UINT32 nParam2;
  UINT32 nParam3;
#endif /* WIFI_SUPPORT */
} COS_EVENT;

BOOL SRVAPI COS_WaitEvent(
#ifndef MMI_ON_WIN32
  HANDLE hTask,
#else
  HANDLE_WIN32 hTask,
#endif
  COS_EVENT* pEvent,
  UINT32 nTimeOut
);

#define COS_WAIT_FOREVER  0xFFFFFFFF
#define COS_NO_WAIT       0x0

#define COS_EVENT_PRI_NORMAL   0
#define COS_EVENT_PRI_URGENT   1

BOOL SRVAPI COS_SendEvent(
#ifndef MMI_ON_WIN32
  HANDLE hTask, 
#else
  HANDLE_WIN32 hTask,
#endif
  COS_EVENT* pEvent,
  UINT32 nTimeOut,
  UINT16 nOption
);

BOOL SRVAPI COS_IsEventAvailable(
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
);

#define COS_TIMER_MODE_SINGLE       1
#define COS_TIMER_MODE_PERIODIC     2

UINT8 SRVAPI COS_SetTimer (
  UINT32 nMillisecondes, 
  void(*callbackFunc)(void *), 
  void *callbackArg, 
  UINT8 flags
);

/// @todo this function cannot work (timer data never match)
BOOL SRVAPI COS_KillTimer (
  UINT8 nTimerId 
);

BOOL SRVAPI COS_ChangeTimer(
    UINT8 timerHandle, 
    UINT32 nMillisecondes);


BOOL SRVAPI COS_Sleep (
  UINT32 nMillisecondes
);

UINT32 SRVAPI COS_GetTickCount( VOID); 

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
);

BOOL SRVAPI COS_ExitCriticalSection (
#ifndef MMI_ON_WIN32
  HANDLE hSection
#else
  HANDLE_WIN32 hSection
#endif
);

UINT32 SRVAPI COS_GetLastError(VOID);

VOID SRVAPI COS_SetLastError(
  UINT32 nErrCode   // per-thread error code
);

#ifdef _DEBUG
PVOID  COS_MallocDbg (UINT32 nSize, CONST UINT8* pszFileName, UINT32 nLine);
BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine);

#define COS_MALLOC(size) COS_MallocDbg((UINT32)size, __FILE__, __LINE__)
#define COS_FREE(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)
#else
#define COS_MALLOC(size) COS_Malloc((UINT32)size)
#define COS_FREE(p)      COS_Free((PVOID)p)
#endif



PVOID SRVAPI COS_Malloc (
  UINT32 nSize
);

BOOL SRVAPI COS_Free (
  PVOID pMemBlock
);


#define COS_SHMEMMALLOC(size) COS_Malloc((UINT32)size)
#define COS_SHMEMFREE(p)      COS_Free((PVOID)p)


HANDLE SRVAPI COS_CreateSemaphore (
    UINT32  nInitCount  // Specify the initial count of the semaphore
);

BOOL SRVAPI COS_DeleteSemaphore(
#ifndef MMI_ON_WIN32
  HANDLE hSem
#else
  HANDLE_WIN32 hSem
#endif
);

BOOL SRVAPI COS_WaitForSemaphore(
#ifndef MMI_ON_WIN32
	HANDLE hSem, // Specify the handle to a counting semaphore 
#else
	HANDLE_WIN32 hSem,
#endif
    UINT32 nTimeOut  // the time-out value
);

BOOL SRVAPI COS_ReleaseSemaphore(
#ifndef MMI_ON_WIN32
  HANDLE hSem //Specify the counting semaphore
#else
  HANDLE_WIN32 hSem
#endif
);

#endif // H 

