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



#include "MultimediaTask.h"
#if !defined(WIN32)
#include "hal_timers.h"
#include "hal_sys.h"
#endif /* WIN32 */
#include "aud_main.h"
#if defined(CAMERA_SUPPORT)
#include "cam_main.h"
#endif /* CAMERA_SUPPORT */


#if 0//ndef __DISABLE_MULTI_TASK_TIMER__   // multi timer

static struct t_MultiTimerInfo {	
    UINT8 flags;
    void (*callbackFunc)(void *);
    void *callbackParam;
} multi_timers[MULTI_MAX_TIMERS];

extern UINT8 MultiGetSysTaskMbx(void);

static void Multi_Timer_Initialise(void)
{
    memset(multi_timers, 0, sizeof(multi_timers));
}

UINT8 Multi_SetTimer(UINT32 nMillisecondes, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags)
{
    /* Timer handles must start at 1 - timer handle 0 is unused */
    struct t_MultiTimerInfo *newEntry;
    UINT8 retVal=0;
	UINT8 i;
	UINT32 csStatus;
    UINT32 nElapse;

	if(flags == 0)
        return 0;

	for(i=1; i<MULTI_MAX_TIMERS; i++)
	{
		if(multi_timers[i].flags == MULTI_TIMER_UNUSED)
		{
		    csStatus = hal_SysEnterCriticalSection();
		    if(multi_timers[i].flags == MULTI_TIMER_UNUSED)
            {      
				newEntry = &(multi_timers[i]);
                newEntry->flags=flags;
				retVal = i;
            	hal_SysExitCriticalSection(csStatus);
				break;
            }
            hal_SysExitCriticalSection(csStatus);
		}
	}
    if (i==MULTI_MAX_TIMERS) {
        return 0;
    }

    newEntry->callbackFunc=callbackFunc;
    newEntry->callbackParam=callbackArg;
    nElapse = nMillisecondes MS_WAITING;
    csStatus = hal_SysEnterCriticalSection();
    sxs_StartTimer ( nElapse, HVY_TIMER_IN+COS_MULTI_TIMER_ID_BASE+retVal, (VOID*)(((flags==MULTI_TIMER_MODE_SINGLE)?FALSE:TRUE)<<31|nElapse), FALSE, MultiGetSysTaskMbx());
    hal_SysExitCriticalSection(csStatus);
    return retVal;
}

BOOL Multi_ChangeTimer(UINT8 timerHandle, UINT32 nMillisecondes)
{
    UINT32 nElapse;

    if(timerHandle !=0 && timerHandle < MULTI_MAX_TIMERS)
    {
        UINT8 timer_index = timerHandle;
        struct t_MultiTimerInfo *p_timer = &multi_timers[timer_index];

        nElapse = nMillisecondes MS_WAITING;
        sxs_StartTimer ( nElapse, HVY_TIMER_IN+COS_MULTI_TIMER_ID_BASE+timer_index, (VOID*)(((p_timer->flags==MULTI_TIMER_MODE_SINGLE)?FALSE:TRUE)<<31|nElapse), TRUE, MultiGetSysTaskMbx());
        
        return TRUE;
    }
    else
	    return FALSE;
}

//
// Stop and Realse Timer
//
BOOL Multi_KillTimer(UINT8 timerHandle)
{
    if(timerHandle !=0 && timerHandle < MULTI_MAX_TIMERS && multi_timers[timerHandle].flags != MULTI_TIMER_UNUSED)
    {
        UINT32 csStatus = hal_SysEnterCriticalSection();
        UINT8 timer_index = timerHandle;
        if(multi_timers[timer_index].flags != MULTI_TIMER_UNUSED)
        {
        	multi_timers[timer_index].flags = MULTI_TIMER_UNUSED;
            sxs_StopTimer(HVY_TIMER_IN+COS_MULTI_TIMER_ID_BASE+timer_index, NULL, MultiGetSysTaskMbx());
        }
    	hal_SysExitCriticalSection(csStatus);

        return TRUE;
    }
    else
	    return FALSE;
}

static void Multi_HandleExpiredTimers(UINT8 timerHandle)
{
    int curr_index;
	struct t_MultiTimerInfo *current;

    void (*callbackFunc)(void *);
    void *callbackParam;
    
    UINT32 csStatus = hal_SysEnterCriticalSection();
	current = &(multi_timers[timerHandle]);

	if(current->flags == MULTI_TIMER_UNUSED)
	{
	    sxs_StopTimer(HVY_TIMER_IN+COS_MULTI_TIMER_ID_BASE+timerHandle, NULL, MultiGetSysTaskMbx());
	    hal_SysExitCriticalSection(csStatus);
		return;
	}	
    callbackFunc=current->callbackFunc;
    callbackParam = current->callbackParam;
    if (current->flags == MULTI_TIMER_MODE_SINGLE)
    {
        sxs_StopTimer(HVY_TIMER_IN+COS_MULTI_TIMER_ID_BASE+timerHandle, NULL, MultiGetSysTaskMbx());
    	current->flags = MULTI_TIMER_UNUSED; // disable the timer
    } 
	hal_SysExitCriticalSection(csStatus);

    // call callback function
	if(callbackFunc)
	    callbackFunc(callbackParam);
}
#endif
/******************************************************************************
 * MultimediaTaskInit: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
static void MultimediaTaskInit(void)
{
#if 0//ndef __DISABLE_MULTI_TASK_TIMER__   // multi timer
	Multi_Timer_Initialise();
#endif

	aud_init();
#if defined(CAMERA_SUPPORT)
	cam_init();
#endif /* CAMERA_SUPPORT */
}

/******************************************************************************
 * BAL_MultimediaTaskEntry: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
TASK_ENTRY BAL_MultimediaTaskEntry(void *pData)
{
	COS_EVENT ev;
	memset(&ev, 0, sizeof(ev));
	
	/* Multimedia task module init. */
	MultimediaTaskInit();

	while (1)
	{
        COS_WaitEvent(MOD_MULTI, &ev, COS_WAIT_FOREVER);

#if 0//ndef __DISABLE_MULTI_TASK_TIMER__   // multi timer
		if(ev.nEventId == EV_TIMER)
		{
			MULTI_TRACE(MULTI_MAIN_TRC, "MultimediaTask timer event coming! timer_id = %d", ev.nParam1);
			Multi_HandleExpiredTimers(ev.nParam1);
			continue;
		}
#endif
		
        MULTI_TRACE(MULTI_MAIN_TRC, "MultimediaTask ev.id=%d, param=%x", ev.nEventId, ev.nParam1);

		if(aud_main(&ev))
		{
		}
#if defined(CAMERA_SUPPORT)
		else if(cam_main(&ev))
		{
		}
#endif /* CAMERA_SUPPORT */
	}
}

