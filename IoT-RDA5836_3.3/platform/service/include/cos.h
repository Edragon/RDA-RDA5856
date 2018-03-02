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



/*                                                                            */
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

#ifndef PUBLIC
#define PUBLIC
#endif
#define COS_MAX_PRIORITY       20 
#define COS_CREATE_DEFAULT     0
#define COS_CREATE_SUSPENDED   1
#define COS_CREATE_PREEMPT     2
#define COS_CREATE_NO_PREEMPT  3

//#define COS_EVENT_TIMER        0x01 
//#define COS_FUNCITON_TIMER     0x02
#define DEV_INVALID_SEM_HANDLE   0xff


#define COS_FUNCTION_TIMER_POS  14
#define COS_FUNCTION_TIMER      ((u16)1 << COS_FUNCTION_TIMER_POS)
#define COS_MBX_TIMER_POS       15
#define COS_MBX_TIMER           ((u16)1 << COS_MBX_TIMER_POS)


#if OS_KERNEL == FREERTOS
#define tskMIN_PRIORITY			0
#define tskMAX_PRIORITY			5
#define mainBLOCK_Q_PRIORITY	( tskMIN_PRIORITY + 2 )
#define countMAX_COUNT_VALUE	( 10 )
#endif

typedef struct _TaskDesc
{
 void (*TaskBody)(void *);
// void (*TaskExit)(void *);
 const char *Name;
 UINT16 nStackSize;
 UINT8 nPriority;
}TaskDesc;


typedef enum {
   MOD_NIL = 0,
   MOD_SYS,                 // system task
   MOD_APP ,                // application task
   MOD_MED,                 // media play task
#if defined(BT_SUPPORT)
   MOD_BT,                  // bluetooth task
#endif /* BT_SUPPORT */
#if defined(GSM_SUPPORT)
   MOD_CFW,
#endif
   MOD_MULTI,            // Multimedia Task
#ifdef CFW_TCPIP_SUPPORT
   MOD_APS,
#endif
#if defined(WIFI_SUPPORT)
   MOD_WIFI_SERVICE,
   MOD_WIFI_REC,
   MOD_WIFI_MGR,
#endif /* WIFI_SUPPORT */
#ifdef CFW_TCPIP_SUPPORT
   MOD_SOC_TEST, //For test socket performance
#endif /* CFW_TCPIP_SUPPORT */

#ifdef SPEAKER_SUPPORT
	MOD_SPEAKER_SOCKET,
#endif

#ifdef RDA_DLNA
   MOD_HTTP_SERVER,
   MOD_HTTP_SERVER_SHIFT,
   MOD_SSDP_SERVER,
   MOD_SSDP_ADVERTISE,
#endif

#ifdef __WIFI_RADIO_BOX__
   MOD_RADIO_BOX,
#endif /* __WIFI_RADIO_BOX__ */
#ifdef IP_CAMERA_SUPPORT
   MOD_IP_CAMERA,
#endif /* IP_CAMERA_SUPPORT */

#ifdef MCU_WIFI_PLAT
   MOD_WIFI_CORE,
#endif

#ifdef YOC_SUPPORT
   MOD_YOC_SERVICE,
   MOD_YOC_MAIN_LOOP,
#endif


/********  AUTO MOD ID  START***************/
   MOD_CUSTOMER_AUTO,
   MOD_CUSTOMER_START = MOD_CUSTOMER_AUTO,
   MOD_CUSTOMER_END = MOD_CUSTOMER_START + 10 /**/,
/********  AUTO MOD ID  END***************/

   TOTAL_MODULES
} COS_MOD_ID;


typedef struct _TASK_HANDLE
{
 TaskDesc sTaskDesc;
#if OS_KERNEL == SXR
 UINT8 nTaskId;
 UINT8 nMailBoxId;
#else
 COS_MOD_ID    taskMod;
 void *        queue;
 void *        taskHandle;
 BOOL 	       exclusive;
#endif
// UINT8 padding[4];
}TASK_HANDLE;



#ifdef MMI_ON_WIN32
#define TASK_ENTRY      DWORD WINAPI
typedef DWORD(WINAPI *PTASK_ENTRY)(PVOID);
#else
#define TASK_ENTRY      VOID
#endif

typedef enum  {
      COS_MMI_HEAP = 0x00,

     	COS_HEAP_COUNT 
}COS_HEAP_ID;

#define COS_MMI_TASKS_PRIORITY_BASE 220 // 220-250 is assigned to MMI part.
#define COS_PRIORITY_NUM            30

HANDLE SRVAPI COS_CreateTask(
  PTASK_ENTRY pTaskEntry,
  COS_MOD_ID mod,
  PVOID pParameter,
  PVOID pStackAddr,
  UINT16 nStackSize,
  UINT8 nPriority,
  UINT16 nCreationFlags,
  UINT16 nTimeSlice,
  CONST INT8* pTaskName
);

VOID SRVAPI COS_StartTask(
  TASK_HANDLE *pHTask,
  PVOID pParameter
);

VOID SRVAPI COS_StopTask(
  TASK_HANDLE *pHTask
);

BOOL SRVAPI COS_DeleteTask(
  COS_MOD_ID mod
);

UINT32 COS_SuspendTask(
  COS_MOD_ID mod
);

BOOL COS_ResumeTask (
  COS_MOD_ID mod
);

typedef struct _COS_EVENT {
  UINT32 nEventId;
  UINT32 nParam1;
#if defined(WIFI_SUPPORT)
  UINT32 nParam2;
  UINT32 nParam3;
#endif /* WIFI_SUPPORT */
} COS_EVENT;

BOOL SRVAPI COS_WaitEvent(
  COS_MOD_ID mod,
  COS_EVENT* pEvent,
  UINT32 nTimeOut
);

BOOL COS_FreeEvent(COS_EVENT* pEvent);

#define COS_WAIT_FOREVER  0xFFFFFFFF
#define COS_NO_WAIT       0x0

#define COS_EVENT_PRI_NORMAL   0
#define COS_EVENT_PRI_URGENT   1


VOID COS_SendMessageEvent(COS_MOD_ID mod_id, UINT32 event, UINT32 param); 

BOOL SRVAPI COS_SendEvent(
  COS_MOD_ID mod, 
  COS_EVENT* pEvent,
  UINT32 nTimeOut,
  UINT16 nOption
);

BOOL SRVAPI COS_ResetEventQueue(
  HANDLE hTask
);

BOOL SRVAPI COS_IsEventAvailable(
  COS_MOD_ID mod
);

UINT8 COS_GetCurrentTaskId(VOID);
COS_MOD_ID COS_GetCurrentTaskModId(VOID); 

#if OS_KERNEL == FREERTOS
BOOL COS_SetExclusiveRunningTask(COS_MOD_ID mod, BOOL enable);
BOOL COS_IsExclusiveRunningTask(COS_MOD_ID mod);
#endif

#define COS_TIMER_MODE_SINGLE     1
#define COS_TIMER_MODE_PERIODIC   2

#if 0
BOOL SRVAPI COS_SetTimer (
  HANDLE hTask,
  UINT8 nTimerId,
  UINT8 nMode,
  UINT32 nElapse
#endif

UINT8 SRVAPI COS_SetTimer (
  UINT32 nMillisecondes, 
  void(*callbackFunc)(void *), 
  void *callbackArg, 
  UINT8 flags
);

void COS_Timer_Initialise(void);

// Temporarily  Change the define TimerID range for MMI and CSW

//
// COS TimerID
// 
#define COS_TIMER_ID_BASE  0x000
#define COS_TIMER_ID_END_  0x0FF


//
// MMI TimerID
// 
#define COS_MMI_TIMER_ID_BASE  0x100
#define COS_MMI_TIMER_ID_END_  0x3FF

//
// CSW TimerID
// 
#define COS_CSW_TIMER_ID_BASE  0x400
#define COS_CSW_TIMER_ID_END_  0x4FF


//
// BT TimerID
// 
#define COS_BT_TIMER_ID_BASE    0x500
#define COS_BT_TIMER_ID_END_    0x5FF


//
// MDI TimerID
// 
#define COS_MDI_TIMER_ID_BASE   0x600
#define COS_MDI_TIMER_ID_END_   0x6FF

//
//TODO...
//
#define COS_XXX_TIMER_ID_BASE   0x700
#define COS_XXX_TIMER_ID_END_   0xFFE

BOOL SRVAPI COS_SetTimerEX (
  HANDLE hTask,
  UINT16 nTimerId,
  UINT8 nMode,
  UINT32 nMillisecondes
);

BOOL  SRVAPI COS_KillTimerEX (
  HANDLE hTask,
  UINT16 nTimerId 
);

BOOL SRVAPI COS_KillTimer (
  UINT8 nTimerId 
);

BOOL COS_ChangeTimer(
    UINT8 timerHandle, 
    UINT32 nMillisecondes);

BOOL COS_ChangeTimerUseSecond(
    UINT8 timerHandle, 
    UINT32 secondes);

BOOL SRVAPI COS_Sleep (
  UINT32 nMillisecondes
);

BOOL COS_SleepByTick(UINT32 ticks); 

UINT8 COS_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d);
BOOL COS_StopFunctionTimer (UINT8 timer_handler);

UINT32 SRVAPI COS_GetTickCount( VOID); 

BOOL SRVAPI COS_SetPriority (
  COS_MOD_ID mod,
  UINT8 nPriority
);

HANDLE SRVAPI COS_EnterCriticalSection (
  VOID
);

BOOL SRVAPI COS_ExitCriticalSection (
  HANDLE hSection
);

UINT32 SRVAPI COS_GetLastError(VOID);

VOID SRVAPI COS_SetLastError(
  UINT32 nErrCode   // per-thread error code
);

VOID COS_MemInit(); 

#ifdef _DEBUG
extern PVOID  COS_MallocDbg (UINT32 nSize,  CONST UINT8* pszFileName, UINT32 nLine);
extern BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine);

#define COS_MALLOC(size) COS_MallocDbg((UINT32)size, __FILE__, __LINE__)
#define COS_FREE(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)

#define COS_Malloc(size) COS_MallocDbg((UINT32)size, __FILE__, __LINE__)
#define COS_Free(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)

extern void  _COS_MEMChkEntry (CONST UINT8* pszFileName, UINT32 nLine);
extern void _COS_MEMChkExit (CONST UINT8* pszFileName, UINT32 nLine);

#define COS_MEMChkEntry()   _COS_MEMChkEntry(__FILE__, __LINE__)
#define COS_MEMChkExit()      _COS_MEMChkExit(__FILE__, __LINE__)

#else
#define COS_MALLOC(size) COS_Malloc((UINT32)size/*,COS_MMI_HEAP*/)
#define COS_FREE(p)      COS_Free((PVOID)p)
#define COS_REALLOC(ptr, size) COS_Realloc(ptr, size)
#endif

#ifdef CHIP_CPU_IS_ARM
#define COS_GET_RA(store)
#else
#ifdef CHIP_CPU_IS_RISCV
#define COS_GET_RA(store)
#else 
#ifdef __mips16
#define COS_GET_RA(store) asm volatile("move $2, $31\n\tsw $2, 0(%0)"::"d"((store)):"$2")
#else
#define COS_GET_RA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))
#endif
#endif
#endif

void  COS_CleanDCache(void) ;//asm volatile(".set noreorder\n\t .align 4\n\tcache 2,0\n\tnop\n\tnop\n\tnop\n\tjr ra\n\tnop\n\t.set reorder ")
void  COS_CleanICache(void) ;//asm volatile(".set noreorder\n\t .align 4\n\tcache 1,0\n\tnop\n\tnop\n\tnop\n\tjr ra\n\tnop\n\t.set reorder ")
void  COS_CleanALLCache(void);// asm volatile(".set noreorder\n\t .align 4\n\tcache 0,0\n\tnop\n\tnop\n\tnop\n\tjr ra\n\tnop\n\t.set reorder ")

VOID  COS_UpdateMemCallAdd( VOID *pMemeAddr, UINT32 nCallAdd);
typedef struct _COS_HEAP_STATUS {
  UINT32 nUsedSize;
  UINT32 nTotalSize;
} COS_HEAP_STATUS;

UINT32 COS_GetHeapUsageStatus( COS_HEAP_STATUS *pHeapStatus, UINT8 nIdx );

PVOID SRVAPI COS_Malloc (UINT32 nSize);
PVOID SRVAPI COS_Calloc(UINT32 n, UINT32 size);
PVOID SRVAPI COS_Malloc_NoCache (UINT32 nSize);
PVOID COS_Realloc(VOID *ptr,UINT32 nSize);
BOOL SRVAPI COS_Free (PVOID pMemBlock);

#if (CHIP_BTOVERLAY_SUPPORTED==0)
#define COS_Malloc_BTOverlay(size)   COS_Malloc(size)
#define COS_Free_BTOverlay(size)   COS_Free(size)
#else
VOID COS_BTOverlay_Free_All(void);
PVOID SRVAPI COS_Malloc_BTOverlay( UINT32 size);
BOOL SRVAPI COS_Free_BTOverlay(PVOID pMemBlock);
#endif

#define COS_SHMEMMALLOC(size) COS_Malloc((UINT32)size)
#define COS_SHMEMFREE(p)      COS_Free((PVOID)p)


HANDLE SRVAPI COS_CreateSemaphore (
    UINT32  nInitCount  // Specify the initial count of the semaphore
);

BOOL SRVAPI COS_DeleteSemaphore(
    HANDLE hSem
);

BOOL SRVAPI COS_WaitForSemaphore(
    HANDLE hSem,     // Specify the handle to a counting semaphore 
    UINT32 nTimeOut  // the time-out value
);

BOOL SRVAPI COS_ReleaseSemaphore(
    HANDLE hSem //Specify the counting semaphore
);

int SRVAPI COS_GetSemaphoreCount(HANDLE hSem);


UINT32 COS_SendRMCEvent(COS_EVENT* pEV);


// to allow not using cos.h but sx direclty for timers
#define HVY_TIMER_IN             ( 0xFFFFF000          )


// =============================================================================
// COS_PAGE_NUM
// -----------------------------------------------------------------------------
/// Those are the 2 pages which can be protected by app
// =============================================================================
typedef enum{
    COS_PAGE_NUM_4      = 0x04,
    COS_PAGE_NUM_5      = 0x05,
}COS_PAGE_NUM;


// =============================================================================
// COS_PAGE_SPY_MODE
// -----------------------------------------------------------------------------
/// Type defining the possible Page Spy Mode 
/// triggering an interrupt
// =============================================================================
typedef enum{
    COS_PAGE_NO_TRIGGER        =0,      /// No interrupt is triggered
	  COS_PAGE_READ_TRIGGER      =1,      /// Triggered on reading of the protected page
	  COS_PAGE_WRITE_TRIGGER     =2,      /// Triggered on writing of the protected page
	  COS_PAGE_READWRITE_TRIGGER =3       /// Triggered on reading or writing of the protected page
}COS_PAGE_SPY_MODE;


// =============================================================================
// COS_PageProtectSetup
// -----------------------------------------------------------------------------
/// This function setups a protection page
///
/// @param nPageNum Name of the page we want to configure  
/// @param nMode Protection mode
/// @param nStartAddr Address of the beginning of the page
/// @param nEndAddr  End address of the page. This address is not included in 
/// the page
// =============================================================================  
PUBLIC UINT32 COS_PageProtectSetup( COS_PAGE_NUM nPageNum, COS_PAGE_SPY_MODE nMode, UINT32 nStartAddr, UINT32 nEndAddr );


// =============================================================================
// COS_PageProtectEnable
// -----------------------------------------------------------------------------
/// Enable the protection of a given page.
///
/// @param nPageNum Page to enable
// =============================================================================
PUBLIC UINT32 COS_PageProtectEnable( COS_PAGE_NUM nPageNum );


// =============================================================================
// COS_PageProtectDisable
// -----------------------------------------------------------------------------
/// Disable the protection of a given page
///
/// @param nPageNum Page to disable
// =============================================================================
PUBLIC UINT32 COS_PageProtectDisable( COS_PAGE_NUM nPageNum );

HANDLE COS_CreateMutex(VOID);

UINT8 COS_GetMutex(HANDLE handle, UINT32 nTimeOut);

UINT8 COS_ReleaseMutex(HANDLE handle);

VOID COS_DeleteMutex(HANDLE handle);

HANDLE Cos_GetTaskHandler(COS_MOD_ID task_id);

#endif // H 

