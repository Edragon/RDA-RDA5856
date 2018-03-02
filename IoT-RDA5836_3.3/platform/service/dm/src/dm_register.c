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

#include <cswtype.h>
#include <base.h>
#include "chip_id.h"
#include "hal_timers.h"
#include <cos.h>
#include <base_prv.h>
#include <dm_prv.h>

#define DM_DEVID_KEYPAD 0
#define DM_DEVID_ALRAM  1
#define DM_DEVID_XX

extern BOOL COS_SendSysEvent(
    COS_EVENT* pEvent,
    UINT16 nOption
);
extern VOID DM_AudInit(VOID);

BOOL g_KeyBoard_Init_Status = FALSE ;

BOOL DM_KeyboardInit()
{
    if (g_KeyBoard_Init_Status)
    {
        return TRUE ;
    }

    g_KeyBoard_Init_Status = TRUE ;
    dm_PowerdownInit();
    // Key Init
    dm_KeyInit();
    return TRUE ;
}

PRIVATE VOID alarm_isr(VOID)
{
    COS_EVENT ev;
    ev.nEventId = EV_TIM_ALRAM_IND;
    ev.nParam1  = 0;
//    ev.nParam2  = 0;//for delete nParam2
//    ev.nParam3  = 0;//for delete nParam3
    COS_SendSysEvent(&ev, (UINT16)0);
}

BOOL dm_Init()
{
    // Alarm Init
    hal_TimRtcIrqSetMask(TRUE);
    hal_TimRtcAlarmIrqSetHandler(alarm_isr);
    // Audio Init
    DM_AudInit();
    // Keyboard Init
    DM_KeyboardInit();
    return TRUE;
}

BOOL dm_SendToDefaultMMITask(COS_EVENT* pev)
{
    return COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

