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


#ifndef _MULTIMADIA_TASK_H__
#define _MULTIMADIA_TASK_H__

#include "ap_common.h"

typedef  COS_EVENT *  MULTI_MSG_PTR;

#define MULTI_MAIN_TRC   10
#define MULTI_AUD_TRC  11
#define MULTI_CAM_TRC  12

#ifndef APP_NO_TRACE
#define MULTI_TRACE app_trace
#else
#define MULTI_TRACE(level, format, ...)  
#endif //APP_NO_TRACE


#if 0//ndef __DISABLE_MULTI_TASK_TIMER__   // multi timer
#define MULTI_TIMER_MODE_SINGLE   		COS_TIMER_MODE_SINGLE
#define MULTI_TIMER_MODE_PERIODIC   	COS_TIMER_MODE_PERIODIC

#define MULTI_TIMER_UNUSED		0
#define MULTI_MAX_TIMERS              10


UINT8 Multi_SetTimer(UINT32 nMillisecondes, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags);
BOOL Multi_ChangeTimer(UINT8 timerHandle, UINT32 nMillisecondes);
BOOL Multi_KillTimer(UINT8 timerHandle);
#else
#define MULTI_TIMER_MODE_SINGLE   		COS_TIMER_MODE_SINGLE
#define MULTI_TIMER_MODE_PERIODIC   	COS_TIMER_MODE_PERIODIC

#define Multi_SetTimer  	COS_SetTimer
#define Multi_ChangeTimer   COS_ChangeTimer
#define Multi_KillTimer  	COS_KillTimer
#endif /*__DISABLE_MULTI_TASK_TIMER__*/

#endif /* _MULTIMADIA_TASK_H__ */

