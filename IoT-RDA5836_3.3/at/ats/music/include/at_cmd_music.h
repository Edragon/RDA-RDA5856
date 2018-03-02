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
 *  at_cmd_tcpip.h
 * Author
 *  Felix
 * Date
 *  2007/11/25
 * Descriptions:
 *  ...
 *
 *********************************************************/

#ifndef __AT_CMD_MUSIC_H__
#define __AT_CMD_MUSIC_H__

#include "event.h"
#include "at_common.h"
typedef enum {

    AT_MUSIC_STREAM_NONE,
    AT_MUSIC_STREAM_UART,
    AT_MUSIC_STREAM_LOCAL
}AT_STREAM_TYPE;

extern VOID AT_MUSIC_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_CmdFunc_Play(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_CmdFunc_Pause(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_CmdFunc_State(AT_CMD_PARA* pParam);

extern VOID AT_MUSIC_UartPlay_CmdFunc_Play(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_Stop(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_Pause(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_Resume(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_RawData(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_FTCodec(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_SetMicGain(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_GetStatus(AT_CMD_PARA* pParam);
extern VOID AT_MUSIC_UartPlay_CmdFunc_PlayInterAud(AT_CMD_PARA* pParam);

#endif

