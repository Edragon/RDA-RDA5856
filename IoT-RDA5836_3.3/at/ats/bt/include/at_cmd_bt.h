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
 *	at_cmd_bt.h
 * Author
 * 	WuXiang
 * Date
 * 	2016/1/29
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_BT_H_
#define _AT_CMD_BT_H_

#include "tgt_app_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**************************************************************************************************
	 *                                          INCLUDES
	 **************************************************************************************************/
#include "at_common.h"



#ifdef __cplusplus
}
#endif

extern long strtol(const char *nptr, char **endptr, int base);
extern void hal_BtEnableSleep(BOOL sleep);

VOID AT_EMOD_CmdFunc_BTOPEN(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTPLAY(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTPAUSE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTINQUIRY(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPDISCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_A2DPCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_A2DPDISCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTCLOSE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSWITCH(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTFORWARD(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTBACKWARD(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTGETTRACKINFO(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTVOLUP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTVOLDOWN(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTVISIBLE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTINVISIBLE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSETNAME(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTGETRSSI(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPACCEPTCALL(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPREJECTCALL(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPTERMINALCALL(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPCALL(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPSENDAT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPGETPB(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPCDN(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HIDTP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_GETPB(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_GETCR(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_GETMAP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_SETMNS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_CONHID(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HIDTP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTAVRCPCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTAVRCPDISCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSWITCH(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTINQUIRY(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTINQUIRECANCEL(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSPPCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSPPDISCONNECT(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSPPSENDDATA(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPCONSCO(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_HFPDISCSCO(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTDUTTEST(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_FCC(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_FCCSTOP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTVCOTEST(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_START_BAS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_START_IAS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_START_WIFISCS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETCONFIGSTATE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSETADDR(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTSETLNAME(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTGETSTATUS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_BTGETA2DPSTATUS(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETAPINFO(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_SETSTATE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_SETFEATURE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETINDSTATE(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_LE_WIFISCS_CLEARAPINFO(AT_CMD_PARA *pParam);

#endif

