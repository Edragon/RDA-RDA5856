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
 *	at_cmd_common.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 * 	
 *********************************************************/

#ifndef __AT_CMD_COMMON_H__
#define __AT_CMD_COMMON_H__

#include "event.h"
#include "at_common.h"

extern VOID AT_COMMON_CmdFunc_VolAdd(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_VolSub(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Volume(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_PlayTone(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Next(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Prev(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Fore(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Back(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam);

extern VOID AT_COMMON_CmdFunc_Sleep(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Reset(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Shutdown(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_ReadPeripheralReg(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_WritePeripheralReg(AT_CMD_PARA* pParam);
extern VOID AT_COMMON_CmdFunc_Version(AT_CMD_PARA* pParam);

#endif
 
