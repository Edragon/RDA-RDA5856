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
 *	at_cmd_special_battery.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_BATTERY_H_
#define _AT_CMD_EX_BATTERY_H_

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
#define PM_ACSTATUS_OFFLINE 	0
#define PM_ACSTATUS_ONLINE	1
#define PM_ACSTATUS_UNKNOWN 	255

#define AT_BATTERY_IND_ENABLE 1
#define AT_BATTERY_IND_DISABLE 0
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

#define START_BATTERY_TIMER(nDLC) \
    do { \
        COS_KillTimer(g_hAtTask, ATE_BATTERY_TIMER + nDLC); \
        COS_SetTimer(g_hAtTask, ATE_BATTERY_TIMER + nDLC, COS_TIMER_MODE_SINGLE, 10000 * 16); \
    } while (0)

#define STOP_BATTERY_TIMER(nDLC) \
    do { \
        COS_KillTimer(g_hAtTask, ATE_BATTERY_TIMER + nDLC); \
    } while (0)

#define AT_EX_MONI_TIM_ID         20
#define AT_EX_MONP_TIM_ID         21

	/**************************************************************************************************
	 *                                         TYPEDEFS
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                     GLOBAL VARIABLES
	 **************************************************************************************************/

	/**************************************************************************************************
	 *                                     FUNCTIONS - API
	 **************************************************************************************************/
#ifdef AT_DUAL_SIM
	extern VOID AT_Result_OK(UINT32 uReturnValue, UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize,
	                         UINT8 nDLCI, UINT8 nSim);
	extern VOID AT_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI, UINT8 nSim);
#else
	extern VOID AT_Result_OK(UINT32 uReturnValue, UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize,
	                         UINT8 nDLCI);
	extern VOID AT_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI);
#endif
	/*
	 * AT^CBC
	 */
	VOID AT_BATTERY_CmdFunc_CBC(AT_CMD_PARA *pParam);
	VOID AT_BATTERY_CmdFunc_CBCE(AT_CMD_PARA *pParam);

	/*
	 * AT^CBCM
	 */
	VOID AT_BATTERY_CmdFunc_CBCM(AT_CMD_PARA *pParam);

	VOID AT_SPECIAL_Battery_AsyncEventProcess(COS_EVENT *pEvent);
	extern VOID AT_SPECIAL_Battery_Init();

	/**************************************************************************************************
	**************************************************************************************************/

#ifdef __cplusplus
}
#endif
#endif

