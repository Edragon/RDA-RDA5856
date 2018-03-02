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
/* Filename:    cos_timer.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*   COS implementation.                                                      */
/******************************************************************************/

#include "cos.h"
#include "string.h"
#include <csw.h>
#include <stdkey.h>
#include <event.h>
#include <base_prv.h>
#include <ts.h>
#include<sxr_tksd.h>
#include<sxr_tim.h>
#include <sxr_csp.h>
#include <sxr_cnf.h>
#include "hal_host.h"

#include "csw_csp.h"

#define COS_TIMER_UNUSED        0

#define COS_MAX_TIMERS              20

#if (COS_TIMER_ID_BASE + COS_MAX_TIMERS >= (COS_TIMER_ID_END_))
#error 'Error: invalid COS_MAX_TIMERS definition!'
#endif

static struct t_cosTimerInfo {  
    UINT8 flags;
    void (*callbackFunc)(void *);
    void *callbackParam;
} cos_timers[COS_MAX_TIMERS];

UINT8 COS_GetSysTaskMbx(void);

#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)&& (!defined(CT_NO_DEBUG))
extern UINT8 monkey_timer_id;
#endif /* AUTO_TEST_KEYPRESS_RECORD_SUPPORT */

/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void COS_TimerDelivery (u32 *Id)
{
    //SXS_RAISE ((_SXR|TNB_ARG(1)|TDB, TSTR("Timer not handled %x\n", 0x070b0001), Id));
    hal_DbgAssert("Timer not handled %x\n", Id);
}


void COS_Timer_Initialise(void)
{
    sxr_InitTimer (COS_TimerDelivery, pal_TimerSetCnt, 
            pal_TimerGetCnt, SXR_REGULAR_TIMER);

    memset(cos_timers, 0, sizeof(cos_timers));
}

void COS_Timer_Shutdown(void)
{
    //COS_DeleteSemaphore( timer_info.timer_sem);
}


UINT8 COS_SetTimer(UINT32 nMillisecondes, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags)
{
    UINT8 retVal=0;

    /* Timer handles must start at 1 - timer handle 0 is unused */
    struct t_cosTimerInfo *newEntry;

    UINT8 i;
    UINT32 csStatus;
    UINT32 nElapse;

    if(flags == 0)
        return 0;

    for(i=1; i<COS_MAX_TIMERS; i++)
    {
        if(cos_timers[i].flags == COS_TIMER_UNUSED)
        {
            //COS_WaitForSemaphore( timer_info.timer_sem, COS_WAIT_FOREVER);
            csStatus = hal_SysEnterCriticalSection();
            if(cos_timers[i].flags == COS_TIMER_UNUSED)
            {      
                newEntry = &(cos_timers[i]);
                newEntry->flags=flags;
                retVal = i;
                hal_SysExitCriticalSection(csStatus);
                break;
            }
            //COS_ReleaseSemaphore( timer_info.timer_sem);
            hal_SysExitCriticalSection(csStatus);
        }
    }
    if (i==COS_MAX_TIMERS) {
        hal_HstSendEvent(SYS_EVENT, 0x12240100);
        return 0;
    }

    newEntry->callbackFunc=callbackFunc;
    newEntry->callbackParam=callbackArg;
    //nElapse = nMillisecondes MS_WAITING;
	nElapse = (nMillisecondes / 1000) SECOND;
	nElapse += (nMillisecondes % 1000) MS_WAITING;
	
    csStatus = hal_SysEnterCriticalSection();

    sxs_StartTimer ( nElapse, HVY_TIMER_IN+retVal, (VOID*)(((flags==COS_TIMER_MODE_SINGLE)?FALSE:TRUE)<<31|nElapse), FALSE, COS_GetSysTaskMbx());
    hal_SysExitCriticalSection(csStatus);
    return retVal;
}

BOOL COS_ChangeTimer(UINT8 timerHandle, UINT32 nMillisecondes)
{
    UINT32 nElapse;
    if(timerHandle !=0 && timerHandle < COS_MAX_TIMERS)
    {
        UINT8 timer_index = timerHandle;
        struct t_cosTimerInfo *p_timer = &cos_timers[timer_index];

        //nElapse = nMillisecondes MS_WAITING;
		nElapse = (nMillisecondes / 1000) SECOND;
		nElapse += (nMillisecondes % 1000) MS_WAITING;

        sxs_StartTimer ( nElapse, HVY_TIMER_IN+timer_index, (VOID*)(((p_timer->flags==COS_TIMER_MODE_SINGLE)?FALSE:TRUE)<<31|nElapse), TRUE, COS_GetSysTaskMbx());
        
        return TRUE;
    }
    else
        return FALSE;
}

BOOL COS_ChangeTimerUseSecond(UINT8 timerHandle, UINT32 secondes)
{
    UINT32 nElapse;
    if(timerHandle !=0 && timerHandle < COS_MAX_TIMERS)
    {
        UINT8 timer_index = timerHandle;
        struct t_cosTimerInfo *p_timer = &cos_timers[timer_index];

        nElapse = secondes SECOND;
        sxs_StartTimer ( nElapse, HVY_TIMER_IN+timer_index, (VOID*)(((p_timer->flags==COS_TIMER_MODE_SINGLE)?FALSE:TRUE)<<31|nElapse), TRUE, COS_GetSysTaskMbx());
        
        return TRUE;
    }
    else
        return FALSE;
}

//
// Stop and Realse Timer
//
BOOL COS_KillTimer(UINT8 timerHandle)
{
    if(timerHandle !=0 && timerHandle < COS_MAX_TIMERS && cos_timers[timerHandle].flags != COS_TIMER_UNUSED)
    {
        UINT32 csStatus = hal_SysEnterCriticalSection();
        UINT8 timer_index = timerHandle;
        if(cos_timers[timer_index].flags != COS_TIMER_UNUSED)
        {
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)&& (!defined(CT_NO_DEBUG))
            if(monkey_timer_id == timerHandle)
            {
                hal_DbgAssert("monkey killed!");
            }
#endif /* AUTO_TEST_KEYPRESS_RECORD_SUPPORT */

            cos_timers[timer_index].flags = COS_TIMER_UNUSED;

            sxs_StopTimer(HVY_TIMER_IN+timer_index, NULL, COS_GetSysTaskMbx());

        }
        hal_SysExitCriticalSection(csStatus);

        return TRUE;
    }
    else
        return FALSE;
}

void COS_HandleExpiredTimers(UINT8 timerHandle)
{
    //int curr_index;
    struct t_cosTimerInfo *current;

    void (*callbackFunc)(void *);
    void *callbackParam;
    
    //COS_WaitForSemaphore( timer_info.timer_sem, COS_WAIT_FOREVER);
    UINT32 csStatus = hal_SysEnterCriticalSection();
    current = &(cos_timers[timerHandle]);

    if(current->flags == COS_TIMER_UNUSED)
    {
        sxs_StopTimer(HVY_TIMER_IN+timerHandle, NULL, COS_GetSysTaskMbx());
        hal_SysExitCriticalSection(csStatus);
        return;
    }   
    callbackFunc=current->callbackFunc;
    callbackParam = current->callbackParam;
    if (current->flags == COS_TIMER_MODE_SINGLE)
    {
        sxs_StopTimer(HVY_TIMER_IN+timerHandle, NULL, COS_GetSysTaskMbx());
        current->flags = COS_TIMER_UNUSED; // disable the timer
    } 
    //COS_ReleaseSemaphore( timer_info.timer_sem);
    hal_SysExitCriticalSection(csStatus);

    // call callback function
    if(callbackFunc)
        callbackFunc(callbackParam);
}

UINT32 COS_GetTickCount(VOID)
{
    return ((UINT64)hal_TimGetUpTime())*1000/HAL_TICK1S;
}



BOOL COS_SetTimerEX (
  HANDLE hTask,
  UINT16 nTimerId,//nTimerId should small than 0x1000,
  UINT8 nMode,
  UINT32 nMillisecondes
)
{
    TASK_HANDLE* pHTask = (TASK_HANDLE*)hTask;

    UINT8 nMailBoxId    = 0;
    UINT32 nElapse;
    
    //nElapse = nMillisecondes MS_WAITING;
	nElapse = (nMillisecondes / 1000) SECOND;
	nElapse += (nMillisecondes % 1000) MS_WAITING;

    if(nTimerId>0x1000)
    {
        return FALSE;
    }

    if(nElapse > 0x7FFFFFFF)
        return FALSE;
    
    if((UINT32)pHTask == (HANDLE)0xFFFFFFFF)
    {
        nMailBoxId = 11;
    }
    else
    {
        nMailBoxId = pHTask->nMailBoxId;
    }
    
    if(pHTask)
    {
        if( nMailBoxId )
        {
            BOOL bMode = (nMode==COS_TIMER_MODE_SINGLE)?FALSE:TRUE;            

            sxs_StartTimer ( nElapse, HVY_TIMER_IN+nTimerId, (VOID*)(bMode<<31|nElapse), FALSE, nMailBoxId );
            return TRUE;
        }
    }
    return FALSE;
}

BOOL COS_KillTimerEX (
  HANDLE hTask,
  UINT16 nTimerId 
)
{
    TASK_HANDLE* pHTask = (TASK_HANDLE*)hTask;
    UINT8 nMailBoxId = 0;
    
    if(nTimerId>0x1000)
    {
        return FALSE;
    }

    if((UINT32)pHTask == (HANDLE)0xFFFFFFFF)
    {
        nMailBoxId = 11;
    }
    else
    {
        nMailBoxId = pHTask->nMailBoxId;
    }
    
    if(pHTask)
    {
        if( nMailBoxId )
        {
            sxs_StopTimer(HVY_TIMER_IN+nTimerId, NULL, nMailBoxId);
            return TRUE;
        }
    }
    return FALSE;
}


//
// Sleep current task in nMillisecondes
//
BOOL COS_Sleep (
  UINT32 nMillisecondes
)
{
     extern VOID sxr_Sleep (UINT32 Period);
	UINT32 nElapse;

	nElapse = (nMillisecondes / 1000) SECOND;
	nElapse += (nMillisecondes % 1000) MS_WAITING;

     sxr_Sleep(nElapse);
     return TRUE;
}

BOOL COS_SleepByTick (UINT32 ticks)
{
    extern VOID sxr_Sleep (UINT32 Period);

     sxr_Sleep(ticks);
     return TRUE;
}
