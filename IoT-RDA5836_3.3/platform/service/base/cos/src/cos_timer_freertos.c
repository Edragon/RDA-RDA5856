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
/* Filename:    cos_timer_freertos.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*   COS implementation.                                                      */
/******************************************************************************/

#include "cos.h"

#include <event.h>
#include "hal_timers.h"

#include "csw_csp.h"


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
}

void COS_Timer_Shutdown(void)
{

}


UINT8 COS_SetTimer(UINT32 nMillisecondes, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags)
{
    extern UINT8 hal_SetTimer(UINT32 p_funct, UINT32 data, UINT32 nMiliSec, UINT32 flag);
    UINT8 retVal = 0;
    UINT32 csStatus = hal_SysEnterCriticalSection();
    //#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    retVal = hal_SetTimer((UINT32)callbackFunc, (UINT32)callbackArg, nMillisecondes, flags);
    //#endif

    hal_SysExitCriticalSection(csStatus);
    return retVal;
}

BOOL COS_ChangeTimer(UINT8 timerHandle, UINT32 nMillisecondes)
{
    UINT32 nElapse;
}

BOOL COS_ChangeTimerUseSecond(UINT8 timerHandle, UINT32 secondes)
{
    UINT32 nElapse;
}

//
// Stop and Realse Timer
//
BOOL COS_KillTimer(UINT8 timerHandle)
{

//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    extern void hal_DeleteTimer(UINT32 timerId);
    hal_DeleteTimer(timerHandle);
//#endif
    return TRUE;

}

void COS_HandleExpiredTimers(UINT8 timerHandle)
{

//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    extern void hal_TimerEventProcess(UINT8 timerId);
    hal_TimerEventProcess(timerHandle);
//#endif

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

    extern UINT8 hal_SetTimer(UINT32 p_funct, UINT32 data, UINT32 nMiliSec, UINT32 flag);
    UINT8 retVal = 0;
    UINT32 csStatus = hal_SysEnterCriticalSection();
    
//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    retVal = hal_SetTimer((UINT32)0, (UINT32)(nTimerId|(pHTask->taskMod<<16)), nMillisecondes, nMode|0x80000000);
//#endif
    hal_SysExitCriticalSection(csStatus);
    if(retVal)
    {
        return TRUE;
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


    {
        extern void hal_DeleteTimer(UINT32 timerId);
        
//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
        hal_DeleteTimer(nTimerId | (pHTask->taskMod<<16) | 0x00008000);
//#endif
        return TRUE;
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
     vTaskDelay(nMillisecondes);   //1000 ticks per second
     return TRUE;
}

BOOL COS_SleepByTick(UINT32 ticks)
{
     vTaskDelay(ticks);   //1000 ticks per second
     return TRUE;
}

UINT8 COS_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d)
{
    return COS_SetTimer(Period, Function, (void*)((u16)(Id | COS_FUNCTION_TIMER)), I2d);
}

BOOL COS_StopFunctionTimer (UINT8 timer_handler)
{
    return COS_KillTimer(timer_handler);
}
