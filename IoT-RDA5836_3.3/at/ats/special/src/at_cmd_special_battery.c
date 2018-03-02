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
 *	?
 * Author
 * 	?
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

/**************************************************************************************************
 *                                           INCLUDES
 **************************************************************************************************/
#include "at_common.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_battery.h"
//#include "at_cmd_gc.h"
//#include "at_cfg.h"

/**************************************************************************************************
 *                                          CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 *                                           MACROS
 **************************************************************************************************/

/**************************************************************************************************
 *                                          TYPEDEFS
 **************************************************************************************************/

/**************************************************************************************************
 *                                       GLOBAL VARIABLES
 **************************************************************************************************/

// flag of whether the URC ^CBCI:<bcs>,<bcl>,<Bat_state>,<Bat_volt> is reported
PRIVATE UINT8 g_nPMIndMark = AT_BATTERY_IND_DISABLE;

// set default urc ^cbci display mode: disable
VOID AT_SPECIAL_Battery_Init()
{
	g_nPMIndMark = AT_BATTERY_IND_DISABLE;

	if (!PM_SetBatteryChargeMode(PM_BATTERY_IND_DISABLE))
	{
		COS_Sleep(10);

		if (!PM_SetBatteryChargeMode(PM_BATTERY_IND_DISABLE))
		{
			AT_TC(g_sw_SPEC, "AT_SPECIAL_Battery_Init error\n");
		}
	}
}

VOID AT_SPECIAL_Battery_AsyncEventProcess(COS_EVENT *pEvent)
{
	/*
		UINT8 arrRes[40];
		UINT8 nACStatus = 0;

		if (pEvent == NULL)
		{
			return;
		}

		AT_TC(g_sw_SPEC, "EV_PM_BC_IND come\n");

		//AT_CosEvent2CfwEvent(pEvent, pEv);

		if (pEvent->nEventId == EV_PM_BC_IND)
		{
			UINT16 nBatteryChargeState = HIUINT16(pEvent->nParam1);
			UINT16 nBatteryChargeLevel = LOUINT16(pEvent->nParam1);
			UINT16 nBatteryState = LOUINT16(pEvent->nParam2);
			UINT16 nBatteryVoltage = LOUINT16(pEvent->nParam3);

			AT_TC(g_sw_SPEC, "nBatteryChargeState = %d,nBatteryChargeLevel = %d,nBatteryState = %d,nBatteryVoltage = %d",
				  nBatteryChargeState, nBatteryChargeLevel, nBatteryState, nBatteryVoltage);

			AT_MemZero(arrRes, sizeof(arrRes));

			//CIEV:"BATTCHG",battery charge

			if (gATCurrentuCmer_ind != 0)
			{
				AT_MemZero(arrRes, sizeof(arrRes));
				AT_Sprintf(arrRes, "+CIEV: \"BATTCHG\",%u", nBatteryChargeLevel / 20);
				AT_Result_OK(CSW_IND_NOTIFICATION, CMD_RC_CR, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
			}

			//^sbc: under voltage
			if ((nBatteryState == PM_BATTERY_STATE_CRITICAL)
					|| (nBatteryState == PM_BATTERY_STATE_SHUTDOWN)
					|| (nBatteryState == PM_BATTERY_STATE_LOW))
			{
				AT_MemZero(arrRes, sizeof(arrRes));
				//AT_20071224_CAOW_B for bug#7158
	#if 0
				AT_Sprintf(arrRes, "+SBC: UNDER VOLTAGE");
	#else
				AT_Sprintf(arrRes, "+SBC: UNDER VOLTAGE"); // added by yangtt for bug 8895 at 20080701
	#endif
				//AT_20071224_CAOW_E
				AT_Result_OK(CSW_IND_NOTIFICATION, CMD_RC_CR, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
			}

			if ((nBatteryState == PM_BATTERY_STATE_CRITICAL)
					|| (nBatteryState == PM_BATTERY_STATE_SHUTDOWN)
					|| (nBatteryChargeState != 0)
					|| (g_nPMIndMark == AT_BATTERY_IND_ENABLE))
			{
				AT_MemZero(arrRes, sizeof(arrRes));
				//AT_20071221_CAOW_B
	#if 1

				if (PM_GetACPowerStatus(&nACStatus))
				{
					if (nACStatus == PM_ACSTATUS_OFFLINE)
					{
						AT_TC(g_sw_SPEC, "PM_ACSTATUS_OFFLINE\n");
						AT_Sprintf(arrRes, "^CBCI: %u,%u,%u,%u",
								   nBatteryChargeState,
								   nBatteryChargeLevel,
								   nBatteryState,
								   nBatteryVoltage);
					}
					else if (nACStatus == PM_ACSTATUS_ONLINE)
					{
						AT_TC(g_sw_SPEC, "PM_ACSTATUS_ONLINE\n");
						AT_Sprintf(arrRes, "+CBCI: %u,0,%u,%u", // added by yangtt for bug 8895 at 20080701
								   nBatteryChargeState,
								   nBatteryState,
								   nBatteryVoltage);
					}
				}

	#else
				AT_Sprintf(arrRes, "^CBCI: %u,%u,%u,%u",
						   nBatteryChargeState,
						   nBatteryChargeLevel,
						   nBatteryState,
						   nBatteryVoltage);

	#endif
				//AT_20071221_CAOW_E
				AT_Result_OK(CSW_IND_NOTIFICATION, CMD_RC_CR, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
			}

			return;

		}
		else
		{
			//do nothing
		}
	*/
	return;
}

VOID AT_BATTERY_Timer_Handle(UINT8 nDLCI)
{
	UINT8 battery_info[50];
	CFW_EMOD_BATTERY_INFO pBatInfo;

	CFW_EmodGetBatteryInfo(&pBatInfo );
	sprintf(battery_info, "+CBCE: %d, %d\r\n", pBatInfo.nChargeInfo.nBcs, pBatInfo.nChargeInfo.nBcl);

	if (bMuxActiveStatus)
		SendUIH(nDLCI << 2, battery_info, strlen(battery_info), CTRL_FIELD_UIH);
	else
		AT_WriteUart(battery_info, strlen(battery_info));
	START_BATTERY_TIMER(nDLCI);
}

BOOL AT_BATTERY_GetBatterInfo(UINT8 *pBcs, UINT8 *pBcl)
{
	CFW_EMOD_BATTERY_INFO pBatInfo;

	CFW_EmodGetBatteryInfo(&pBatInfo );
	*pBcs = pBatInfo.nChargeInfo.nBcs;
	*pBcl = pBatInfo.nChargeInfo.nBcl;
	return TRUE;
}

BOOL AT_CIND_GetBattery(UINT8 *pBattchg)
{
	if (pBattchg == NULL)
	{
		return FALSE;
	}

	UINT8 nBcs = 0;

	UINT8 nBcl = 0;

	if (!AT_BATTERY_GetBatterInfo(&nBcs, &nBcl))
	{
		return FALSE;
	}

	*pBattchg = nBcl / 20;

	return TRUE;
}

VOID AT_BATTERY_CmdFunc_CBCE(AT_CMD_PARA *pParam)
{
	INT32 nResult;
	UINT8 iParaCount;
	UINT8 nParaLen = 0;
	UINT8 nEcho = 0;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	switch (pParam->iType)
	{
	case AT_CMD_SET:
		nResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);
		if ((nResult == ERR_SUCCESS) && (iParaCount == 1))
		{
			nParaLen = 1;
			nResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nEcho, &nParaLen);
			if ((nResult != ERR_SUCCESS) || (nEcho > 1))
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
			else
			{
				if (nEcho == 1)
					START_BATTERY_TIMER(pParam->nDLCI);
				else
					STOP_BATTERY_TIMER(pParam->nDLCI);

#ifdef AT_DUAL_SIM
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
			}
		}
		break;

	default:
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
		break;
	}
}
// ^sbc: get charger status and battery charge level
VOID AT_BATTERY_CmdFunc_CBC(AT_CMD_PARA *pParam)
{
	UINT8 res[40];

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	switch (pParam->iType)
	{

	case AT_CMD_TEST:

		AT_MemZero(res, sizeof(res));

		// AT_20071220_CAOW_B
#if 0
		AT_StrCpy(res, "^SBC: (0-5),(0,20,40,60,80,100)");
#else
		AT_StrCpy(res, "+CBC: (0-5),(0,10,20,30,40,50,60,70,80,90,100)"); // yangtt for 8220 at 2008-04-25
#endif
		// AT_20071220_CAOW_E
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI);
#endif
		return;
		break;

	case AT_CMD_READ:
	{
		UINT8 nBcs = 0;
		UINT8 nBcl = 0;

		if (AT_BATTERY_GetBatterInfo(&nBcs, &nBcl))
		{
			AT_MemZero(res, sizeof(res));
			AT_Sprintf(res, "+CBC: %u, %u", nBcs, nBcl);  // yangtt for 8220 at 2008-04-25
			// AT_Sprintf(  res,  "^SBC: 0, %u", BatChargeInfo.nChargeInfo.nBcl);
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI);
#endif
			return;
		}
		else
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		break;
	}

	default:
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
		break;
	}
}

// ^cbcm: control urc ^cbci display  and get battery charge level
VOID AT_BATTERY_CmdFunc_CBCM(AT_CMD_PARA *pParam)
{
	BOOL ret = FALSE;
	UINT8 iParaCount;
	UINT8 nParaLen = 0;
	INT32 eResult;

	// CFW_EMOD_BATTERY_INFO                             BatInfo;
	UINT8 res[20];
	UINT8 nMode = 0;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	switch (pParam->iType)
	{

	case AT_CMD_SET:
		eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

		if ((eResult == ERR_SUCCESS) && (iParaCount == 1))
		{

			nParaLen = 1;
			eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nMode, &nParaLen);

			if ((eResult != ERR_SUCCESS) || (nMode > 1))  // || (nPMIndMark<0) )
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
			else
			{
				g_nPMIndMark = nMode;

				if (g_nPMIndMark == AT_BATTERY_IND_ENABLE)
				{
					ret = PM_SetBatteryChargeMode(PM_BATTERY_IND_ENABLE);
				}
				else if (g_nPMIndMark == AT_BATTERY_IND_DISABLE)
				{
					ret = PM_SetBatteryChargeMode(PM_BATTERY_IND_DISABLE);
				}

				// if (ret) //change by wulc just for testing
				if (!ret)
				{
#ifdef AT_DUAL_SIM
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
					return;
				}
				else
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

			}

		}
		else

		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		break;

	case AT_CMD_READ:
		AT_MemZero(res, sizeof(res));
		AT_Sprintf(res, "+CBCM:%u", g_nPMIndMark);  // added by yangtt for bug 8895 at 20080701
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI);
#endif
		return;
		break;

		/*
		   case AT_CMD_EXE:

		   if ( CFW_EmodGetBatteryInfo(&BatInfo  )  )
		   {
		   AT_MemZero (  res, sizeof(res));
		   AT_Sprintf(  res,  "^CBCM:%u",  BatInfo.nChargeInfo.nBcl);
		   AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI);
		   return;
		   }
		   else
		   {
		   AT_Result_Err(ERR_AT_CME_EXE_FAIL,CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
		   return;
		   }
		   break;
		 */

	case AT_CMD_TEST:
		AT_MemZero(res, sizeof(res));
		AT_Sprintf(res, "+CBCM: (0-1)");  // added by yangtt for bug 8895 at 20080701
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, res, AT_StrLen(res), pParam->nDLCI);
#endif
		return;
		break;

	default:
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
		break;
	}
}

