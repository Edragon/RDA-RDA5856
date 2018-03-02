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
 *	at_cmd_special.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_H_
#define _AT_CMD_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**************************************************************************************************
	 *                                          INCLUDES
	 **************************************************************************************************/
#  include "at_common.h"

	/**************************************************************************************************
	 *                                         CONSTANTS
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                          MACROS
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                         TYPEDEFS
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                     GLOBAL VARIABLES
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                     FUNCTIONS - API
	 **************************************************************************************************/

	/*
	 * AT^MONI
	 */
	VOID AT_EX_CmdFunc_MONI(AT_CMD_PARA *pParam);

	/*
	 * AT^MONI
	 */
	VOID AT_EX_TimerFunc_MONI(AT_CMD_PARA *pParam);

	/*
	 * AT^MONIEND
	 */
	VOID AT_EX_CmdFunc_MONIEND(AT_CMD_PARA *pParam);

	/*
	 * AT^MONIEND
	 */
	VOID AT_EX_TimerFunc_MONIEND(AT_CMD_PARA *pParam);

	/*
	 * AT^MONP
	 */
	VOID AT_EX_CmdFunc_MONP(AT_CMD_PARA *pParam);

	/*
	 * AT^MONP
	 */
	VOID AT_EX_TimerFunc_MONP(AT_CMD_PARA *pParam);

	/*
	 * AT^MONPEND
	 */
	VOID AT_EX_CmdFunc_MONPEND(AT_CMD_PARA *pParam);

	/*
	 * AT^MONPEND
	 */
	VOID AT_EX_TimerFunc_MONPEND(AT_CMD_PARA *pParam);

	/*
	 * AT^SCID
	 */
	VOID AT_EX_CmdFunc_SCID(AT_CMD_PARA *pParam);

	/*
	 * AT^SBBI
	 */
	VOID AT_EX_CmdFunc_SBBI(AT_CMD_PARA *pParam);

	/*
	 * AT^SBBR
	 */
	VOID AT_EX_CmdFunc_SBBR(AT_CMD_PARA *pParam);

	/*
	 * AT^NBBR
	 */
	VOID AT_EX_CmdFunc_NBBR(AT_CMD_PARA *pParam);

	/*
	 * AT^SBBW
	 */
	VOID AT_EX_CmdFunc_SBBW(AT_CMD_PARA *pParam);

	/*
	 * AT^NBBW
	 */
	VOID AT_EX_CmdFunc_NBBW(AT_CMD_PARA *pParam);

	/*
	 * AT^DREG
	 */
	VOID AT_EX_CmdFunc_DREG(AT_CMD_PARA *pParam);

	/*
	 * AT^DPBK
	 */
	VOID AT_EX_CmdFunc_DPBK(AT_CMD_PARA *pParam);

	/*
	 * AT^DFFS
	 */
	VOID AT_EX_CmdFunc_DFFS(AT_CMD_PARA *pParam);

	/*
	 * AT^DSMS
	 */
	VOID AT_EX_CmdFunc_DSMS(AT_CMD_PARA *pParam);

	/*
	 * AT^PRDU
	 */
	VOID AT_EX_CmdFunc_PRDU(AT_CMD_PARA *pParam);

	/*
	 * AT^AUST
	 */
	VOID AT_EX_CmdFunc_AUST(AT_CMD_PARA *pParam);

	/*
	 * AT^AUEND
	 */
	VOID AT_EX_CmdFunc_AUEND(AT_CMD_PARA *pParam);

	/*
	 * AT^SAIC
	 */
	VOID AT_EX_CmdFunc_SAIC(AT_CMD_PARA *pParam);

	/*
	 * AT^SBC
	 */
	VOID AT_EX_CmdFunc_SBC(AT_CMD_PARA *pParam);

	/*
	 * AT^CBCM
	 */
	VOID AT_EX_CmdFunc_CBCM(AT_CMD_PARA *pParam);

	/*
	 * AT^SCTM
	 */
	VOID AT_EX_CmdFunc_SCTM(AT_CMD_PARA *pParam);

	/*
	 * A/
	 */
	VOID AT_EX_CmdFunc_RCMD(AT_CMD_PARA *pParam);

	/*
	 * AT+CMUT
	 */
	VOID AT_CC_CmdFunc_CLVL(AT_CMD_PARA *pParam);
	VOID AT_CC_CmdFunc_VGR(AT_CMD_PARA *pParam);
	VOID AT_CC_CmdFunc_CMUT(AT_CMD_PARA *pParam);

	/*
	 *
	 */
	VOID AT_EX_AsyncEventProcess(COS_EVENT *pEvent);

	/**************************************************************************************************
	**************************************************************************************************/

#ifdef __cplusplus
}
#endif
#endif
