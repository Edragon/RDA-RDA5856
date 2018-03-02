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
 *	at_cmd_special_emod.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_EMOD_H_
#define _AT_CMD_EX_EMOD_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**************************************************************************************************
	 *                                          INCLUDES
	 **************************************************************************************************/
#include "at_common.h"

	/**************************************************************************************************
	 *                                         CONSTANTS
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                          MACROS
	 **************************************************************************************************/

	//
	// ERROR
	//
#define AT_EX_FAIL_CreateRC(pResult,errCode,errorType,delayTime,pBuff,dataSize ,nDLCI)\
	do{                                                                                            \
		pResult = AT_CreateRC(CMD_FUNC_FAIL,CMD_RC_ERROR,errCode,errorType,delayTime,pBuff,dataSize,nDLCI);\
	} while(0)

	//
	// SUCCESS
	//
#define AT_EX_SUCC_CreateRC(pResult,delayTime,pBuff,dataSize,nDLCI)\
	do{\
		pResult = AT_CreateRC(CMD_FUNC_SUCC,CMD_RC_OK,CMD_ERROR_CODE_OK,CMD_ERROR_CODE_TYPE_CME,delayTime,pBuff,dataSize,nDLCI);\
	}while(0)

	//
	// success,need waite asyn
	//
#define AT_EX_SUCC_ASYN_CreateRC(pResult,delayTime,pBuff,dataSize,nDLCI)\
	do{\
		pResult = AT_CreateRC(CMD_FUNC_SUCC_ASYN,CMD_RC_OK,CMD_ERROR_CODE_OK,CMD_ERROR_CODE_TYPE_CME,delayTime,pBuff,dataSize,nDLCI);\
	}while(0)

#define AT_EX_MONI_TIM_ID         20
#define AT_EX_MONP_TIM_ID         21

#ifdef CODEC_SUPPORT
#define AT_EMOD_BUFFER_SIZE       550*2
#else
#define AT_EMOD_BUFFER_SIZE       AT_CMD_LINE_BUFF_LEN*2  //1024 
#endif

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
	* asyn event process
	*/
	VOID AT_Emod_AsyncEventProcess(COS_EVENT *pEvent);

	/*
	* AT^MONI
	*/
	VOID AT_EMOD_CmdFunc_MONI(AT_CMD_PARA *pParam);

	/*
	* AT^MONI
	*/
	VOID AT_EX_TimerFunc_MONI(AT_CMD_PARA *pParam);

	/*
	* AT^MONIEND
	*/
	VOID AT_EMOD_CmdFunc_MONIEND(AT_CMD_PARA *pParam);

	/*
	* AT^MONIEND
	*/
	VOID AT_EX_TimerFunc_MONIEND(AT_CMD_PARA *pParam);

	/*
	* AT^MONP
	*/
	VOID AT_EMOD_CmdFunc_MONP(AT_CMD_PARA *pParam);

	/*
	* AT^MONP
	*/
	VOID AT_EX_TimerFunc_MONP(AT_CMD_PARA *pParam);

	/*
	* AT^MONPEND
	*/
	VOID AT_EMOD_CmdFunc_MONPEND(AT_CMD_PARA *pParam);

	/*
	* AT^MONPEND
	*/
	VOID AT_EX_TimerFunc_MONPEND(AT_CMD_PARA *pParam);

	/*
	* AT^SCID
	*/
	VOID AT_EMOD_CmdFunc_SCID(AT_CMD_PARA *pParam);

	/*
	* AT^SBBI
	*/
	VOID AT_EMOD_CmdFunc_SBBI(AT_CMD_PARA *pParam);

	/*
	* AT^SBBR
	*/
	VOID AT_EMOD_CmdFunc_SBBR(AT_CMD_PARA *pParam);

	/*
	* AT^NBBR
	*/
	VOID AT_EMOD_CmdFunc_NBBR(AT_CMD_PARA *pParam);

	/*
	* AT^SBBW
	*/
	VOID AT_EMOD_CmdFunc_SBBW(AT_CMD_PARA *pParam);

	/*
	* AT^NBBW
	*/
	VOID AT_EMOD_CmdFunc_NBBW(AT_CMD_PARA *pParam);

	/*
	* AT^DREG
	*/
	VOID AT_EMOD_CmdFunc_DREG(AT_CMD_PARA *pParam);

	/*
	* AT^DPBK
	*/
	VOID AT_EMOD_CmdFunc_DPBK(AT_CMD_PARA *pParam);

	/*
	* AT^DFFS
	*/
	VOID AT_EMOD_CmdFunc_DFFS(AT_CMD_PARA *pParam);

	/*
	* AT^DSMS
	*/
	VOID AT_EMOD_CmdFunc_DSMS(AT_CMD_PARA *pParam);

	VOID AT_EMOD_CmdFunc_SLCD(AT_CMD_PARA *pParam);

	VOID AT_EMOD_CmdFunc_STCD(AT_CMD_PARA *pParam);

	/*
	*  AT CFGINIT and CFGGET
	*/
	VOID AT_EMOD_CmdFunc_CFGINIT(AT_CMD_PARA *pParam);
	VOID AT_EMOD_CmdFunc_CFGGET(AT_CMD_PARA *pParam);
	VOID AT_EMOD_CmdFunc_BTSET(AT_CMD_PARA *pParam);

	/*
	*
	*/
	VOID AT_EX_AsyncEventProcess(COS_EVENT *pEvent);

	/**************************************************************************************************
	**************************************************************************************************/
	VOID AT_EMOD_CmdFunc_TSTK(AT_CMD_PARA *pParam);
#if (AUDIO_CALIB_VER == 1)
VOID AT_EMOD_CmdFunc_CAVCP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_CACCP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_CAMCP(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_CAWTF(AT_CMD_PARA *pParam);
VOID AT_EMOD_CmdFunc_CAIET(AT_CMD_PARA *pParam);
#endif
#ifdef __cplusplus
}
#endif
#endif

