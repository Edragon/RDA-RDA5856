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

#ifndef  _AP_IDLE_H_
#define  _AP_IDLE_H_

#include "ap_common.h"

typedef struct {
	INT32 last_time;
	INT32 sleep;
}IDLE_SCREEN_T;

typedef IDLE_SCREEN_T *  IDLE_SCREEN_PTR;


INT32  IDLE_Entry(INT32 param);
void TIMER_SetAlarm(BOOL maintask);

#endif/*_AP_IDLE_H_*/




