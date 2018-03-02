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

/*********************************************************
 *
 * File Name
 *	at_cmd_tcpip.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 * 	
 *********************************************************/

#ifndef __AT_CMD_FM_H__
#define __AT_CMD_FM_H__

#include "event.h"
#include "at_common.h"

extern VOID AT_FM_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam);
extern VOID AT_FM_CmdFunc_AutoSearch(AT_CMD_PARA* pParam);
extern VOID AT_FM_CmdFunc_HoldMode(AT_CMD_PARA* pParam);
extern VOID AT_FM_CmdFunc_StopSearch(AT_CMD_PARA* pParam);
extern VOID AT_FM_CmdFunc_Frequence(AT_CMD_PARA* pParam);
extern VOID AT_FM_CmdFunc_GetState(AT_CMD_PARA* pParam);

#endif
 
