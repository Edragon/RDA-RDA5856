/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/* FILE NAME                                                                  */
/*      cos_win32.c                                                               */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      The COS API definitions on CSW.                                       */
/*      See CSW developement Manual for more.                                 */
/*                                                                            */
/******************************************************************************/


#include "stdafx.h"
#include "cos.h"

BOOL sxr_StartFunctionTimer(UINT32 Period, 
							void(*Function)(void *),
							void *callbackArg, 
							UINT8 flags
)
{
	COS_SetTimer(Period,Function,callbackArg,flags);

	return TRUE;
}


BOOL sxr_StopFunctionTimer (void(*Function)(void *)
						   )
{
	return COS_KillTimer_Ex(Function);
}


BOOL COS_SleepByTick (UINT32 Period) {}
