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
 *	at_cmd_special_wifi.c
 * Author
 * 	?
 * Date
 *
 * Descriptions:
 *	at command for wifi operation
 *
 *********************************************************/

// #include <hal.h>
#include "at_common.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_wifi.h"
#include "at_cfg.h"

#ifdef WIFI_SUPPORT
#include "wifi_config.h"

extern void wifi_PowerOff(void);
extern void wifi_test_mode_close(void);
extern VOID wifi_PowerOn(UINT8 *local_mac);
extern VOID Wifi_ipAddrChangeReq(void *req);
extern VOID wifi_ScanAps(UINT8 dur, UINT8 *ssid);
extern void wifi_test_mode_set_rfregw(UINT16 reg_addr, UINT16 val);
extern void wifi_test_mode_set(UINT16 dbga, UINT16 rate, UINT16 channel, UINT16 len);
extern VOID wifi_ConnectApEx(RDA5890_BSS_DESC *tmp_bss, UINT8 len, wifi_conn_t *conn, UINT16 dur);

typedef struct
{
	UINT8 ref_count;
	UINT16 msg_len;
	BOOL use_dhcp;
	UINT8 ip_addr[4];
	UINT8 gateway[4];
	UINT8 netmask[4];
	UINT8 pri_dns_addr[4];
	UINT8 sec_dns_addr[4];
} ipaddr_change_req_struct;

UINT8 wifi_nDLCI = 0xFF;
UINT8 g_wifiState = 0x00;  //0: closed, 1: opened, 0x0f : openning, 0xff: closeing
UINT8 g_wifiMacTest[6] = {0x00,};
ipaddr_change_req_struct g_ipReq;
RDA5890_BSS_DESC *g_ssidList;

#define CFG_SENDDATALEN 	64
#define	 ERROR_RETURN(pResult,reason,nDLCI) do{ pResult = AT_CreateRC(CMD_FUNC_FAIL,\
        CMD_RC_ERROR, \
        reason,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        NULL,\
        0,\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult	= NULL;	\
    }\
    return;\
}while(0)
#define	 OK_RETURN(pResult,pString,nDLCI) do{	pResult	= AT_CreateRC(	CMD_FUNC_SUCC,\
        CMD_RC_OK, \
        CMD_ERROR_CODE_OK,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        pString,\
        strlen(pString),\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult	= NULL;	\
    }\
    return;\
}while(0)


VOID AT_WIFI_Result_OK(UINT32 uReturnValue,
                       UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize, UINT8 nDLCI)

{
	PAT_CMD_RESULT pResult = NULL;

	pResult = AT_CreateRC(uReturnValue,
	                      uResultCode, CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, nDelayTime, pBuffer, nDataSize, nDLCI);

	AT_Notify2ATM(pResult, nDLCI);
	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

VOID AT_WIFI_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
{
	PAT_CMD_RESULT pResult = NULL;

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR, uErrorCode, nErrorType, 0, 0, 0, nDLCI);
	AT_Notify2ATM(pResult, nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

VOID AT_WIFI_Open_Rsp(COS_EVENT *pEvent)
{
	if(0x00 == pEvent->nParam1)
	{
		AT_WIFI_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, wifi_nDLCI);
	}
	else
	{
		AT_WIFI_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, wifi_nDLCI);
	}
}

//========================================================
//VOID AT_WIFI_AsyncEventProcess(COS_EVENT* pEvent)
//Handle asynchronous msg, called by AT_AsyncEventProcess.
//========================================================

VOID AT_WIFI_AsyncEventProcess(COS_EVENT *pEvent)
{
	UINT8 OutStr[50];
	memset(OutStr, 0, 50);

	switch (pEvent->nEventId)
	{
	case EV_CFW_WIFI_INIT_RSP:
	{
		if(0x00 != pEvent->nParam1)
		{
			AT_WIFI_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, wifi_nDLCI);
			if(0x0f == g_wifiState)
			{
				g_wifiState = 0x01;
			}
		}
		else
		{
			AT_WIFI_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, wifi_nDLCI);
			if(0x0f == g_wifiState)
			{
				g_wifiState = 0x00;
			}
		}
		break;
	}
	case EV_CFW_WIFI_DEINIT_RSP:
	{
		AT_WIFI_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, wifi_nDLCI);
		if(0xff == g_wifiState)
		{
			g_wifiState = 0x00;
		}
		break;
	}
	case EV_CFW_WIFI_SCAN_RSP:
	{
		UINT32 scan_cnt = pEvent->nParam2;
		UINT32 scan_index = 0;

		AT_Sprintf(OutStr, "+WSCAN: %x, cnt=%d(%d)", pEvent->nParam1, pEvent->nParam2, scan_cnt);
		AT_WriteUart(OutStr, AT_StrLen(OutStr));
		AT_WriteUart("\r\n", AT_StrLen("\r\n"));


		g_ssidList = (RDA5890_BSS_DESC *) pEvent->nParam1;

		for (scan_index = 0; scan_index < scan_cnt; scan_index++)
		{
			AT_Sprintf(OutStr, "%d: %s", scan_index, (g_ssidList + scan_index)->ssid);
			AT_WriteUart(OutStr, AT_StrLen(OutStr));
			AT_WriteUart("\r\n", AT_StrLen("\r\n"));
		}

		AT_WIFI_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, wifi_nDLCI);

		break;
	}
	case EV_CFW_WIFI_CONN_RSP:
	case EV_CFW_WIFI_DISCONN_RSP:
	case EV_CFW_WIFI_DISCONNECT_IND:
	case EV_CFW_WIFI_IPADDR_UPDATE_IND:
	{
		AT_WIFI_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, wifi_nDLCI);
		break;
	}

	default :
		break;

	}

}

//========================================================
//VOID AT_WIFI_CmdFunc_iOpen(AT_CMD_PARA* pParam)
//
//========================================================
VOID AT_WIFI_CmdFunc_iOpen(AT_CMD_PARA *pParam)
{

	INT32 iRet = ERR_SUCCESS;
	PAT_CMD_RESULT pResult;
	static WIFI_TEST_MODE siOpenCmd = NO_TEST;

	if (pParam == NULL)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "iOpen Param->iType %d", Param->iType);

		if (Param->pPara == NULL)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_SET:
		{
			UINT8 uCmd;
			UINT8 uLen 		 	 = 0;
			UINT8 uNumOfParam 	 = 0;
			UINT8 arrBuf[5] 	 = {0x00,};

			if (ERR_SUCCESS != AT_Util_GetParaCount(Param->pPara, &uNumOfParam))
			{
				goto iOpen_ERROR;
			}

			AT_TC(g_sw_SPEC, "NumOfParam %d", uNumOfParam);

			if (uNumOfParam != 1)
			{
				goto iOpen_ERROR;
			}

			// Get first parameter
			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 0, arrBuf, &uLen))) // Get first parameter.
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uCmd = AT_StrAToI(arrBuf);
			siOpenCmd = uCmd;

			//#ifdef USB_WIFI_SUPPORT
			if(1 == siOpenCmd)
			{
				wifi_PowerOn(g_wifiMacTest);
				g_wifiState = 0x0f;
				//uctls_Open_wifi(0x0000, 0x0000, 0x0000, "USB Controller 1.1");
			}
			else
			{
				wifi_PowerOff();
				g_wifiState = 0xff;
			}
			wifi_nDLCI = pParam->nDLCI;
			//#endif

			AT_WIFI_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
			//wifi_test_mode_open(Cmd);
			/*
			UINT8 Str[] = "iOpen: OK";

			pResult = AT_CreateRC(CMD_FUNC_SUCC_ASYN, CMD_RC_OK,
			CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 10, Str, AT_StrLen(Str), pParam->nDLCI);


			AT_Notify2ATM(pResult, pParam->nDLCI);

			if (pResult != NULL)
			{
			AT_FREE(pResult);
			pResult = NULL;
			}
			*/
			return;
		}
		case AT_CMD_TEST:
		{

			UINT8 Str[] = "iOpen: NO_TEST, RX_TEST, TX_TEST, OTHER ";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (pResult != NULL)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_READ:
		{

			UINT8 *p[] = {"NO_TEST", "RX_TEST", "TX_TEST", "OTHER"};
			UINT8 Str[30] = {0x00,};
			AT_Sprintf(Str, "iOpen: %s", p[siOpenCmd]);

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
			}

			if (NULL != pResult)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}
		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}

//========================================================
//VOID AT_WIFI_CmdFunc_iTOpen(AT_CMD_PARA* pParam)
//
//========================================================

VOID AT_WIFI_CmdFunc_iTOpen(AT_CMD_PARA *pParam)
{

	INT32 iRet = ERR_SUCCESS;
	PAT_CMD_RESULT pResult;
	static WIFI_TEST_MODE siOpenCmd = NO_TEST;

	if(NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "iTOpen Param->iType %d", Param->iType);

		if(NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch(Param->iType)
		{
		case AT_CMD_SET:
		{
			UINT8 uCmd;
			UINT8 uLen 		  = 0;
			UINT8 uNumOfParam = 0;
			UINT8 arrBuf[5]      = {0x00,};

			if (ERR_SUCCESS != AT_Util_GetParaCount(Param->pPara, &uNumOfParam))
			{
				goto iOpen_ERROR;
			}

			AT_TC(g_sw_SPEC, "NumOfParam %d", uNumOfParam);

			if (uNumOfParam != 1)
			{
				goto iOpen_ERROR;
			}

			// Get first parameter
			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 0, arrBuf, &uLen))) // Get first parameter.
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uCmd = AT_StrAToI(arrBuf);
			siOpenCmd = uCmd;

			wifi_test_mode_open(uCmd);
			UINT8 Str[] = "iTOpen: OK";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			AT_Notify2ATM(pResult, pParam->nDLCI);
			if (pResult != NULL)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}
			return;
		}
		case AT_CMD_TEST:
		{

			UINT8 Str[] = "iTOpen: NO_TEST, RX_TEST, TX_TEST, OTHER ";

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if(NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_READ:
		{

			UINT8 *p[] = {"NO_TEST", "RX_TEST", "TX_TEST", "OTHER"};
			UINT8 Str[30] = {0x00,};
			AT_Sprintf(Str, "iTOpen: %s", p[siOpenCmd]);

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if(NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}


//========================================================
//VOID AT_WIFI_CmdFunc_iTClose(AT_CMD_PARA* pParam)
//
//========================================================

VOID AT_WIFI_CmdFunc_iTClose(AT_CMD_PARA *pParam)
{

	PAT_CMD_RESULT pResult;

	if (NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "iClose Param->iType %d", Param->iType);

		if (NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_TEST:
		{
			UINT8 Str[] = "iTClose ";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if(NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_EXE:
		{

			wifi_test_mode_close();
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}

//========================================================
//VOID AT_WIFI_CmdFunc_iTSetReg(AT_CMD_PARA* pParam)
//
//========================================================

VOID AT_WIFI_CmdFunc_iTSetReg(AT_CMD_PARA *pParam)
{

	PAT_CMD_RESULT pResult;
	INT32 iRet = ERR_SUCCESS;

	if(NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "iSetReg Param->iType %d", Param->iType);

		if(NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_SET:
		{
			UINT8 uNumOfParam = 0;
			UINT8 uLen 		  = 0;
			UINT8 arrBuf[5]   = {0x00,};
			UINT16 uRegAddr   = 0;
			UINT16 uVal       = 0;

			if (ERR_SUCCESS != AT_Util_GetParaCount(Param->pPara, &uNumOfParam))
			{
				goto iOpen_ERROR;
			}

			AT_TC(g_sw_SPEC, "NumOfParam %d", uNumOfParam);

			if (uNumOfParam != 2)
			{
				goto iOpen_ERROR;
			}

			// Get first parameter
			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 0, arrBuf, &uLen))) // Get first parameter.
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uRegAddr = AT_StrAToI(arrBuf);

			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 1, arrBuf, &uLen)))
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}

			I_AM_HERE();

			uVal = AT_StrAToI(arrBuf);
			wifi_test_mode_set_rfregw(uRegAddr, uVal);
			UINT8 Str[] = "iTSetReg: OK";

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			AT_Notify2ATM(pResult, pParam->nDLCI);

			if(NULL != pResult)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}
			return;
		}

		case AT_CMD_TEST:
		{

			UINT8 Str[] = "iTSetReg: void wifi_test_mode_set_rfregw(UINT16 reg_addr,UINT16 val)";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}
		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

//========================================================
//VOID AT_WIFI_CmdFunc_iTSet(AT_CMD_PARA* pParam)
//
//========================================================

VOID AT_WIFI_CmdFunc_iTSet(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult;
	INT32 iRet = ERR_SUCCESS;

	if(NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "iOpen Param->iType %d", Param->iType);

		if (NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_SET:
		{
			UINT8 uNumOfParam 		= 0;
			UINT8 uLen 				= 0;
			UINT8 arrBuf[5] 		= {0x00,};
			UINT16 uDbga 			= 0;
			UINT16 uRate			= 0;
			UINT16 uChannel 		= 0;

			if (ERR_SUCCESS != AT_Util_GetParaCount(Param->pPara, &uNumOfParam))
			{
				goto iOpen_ERROR;
			}
			AT_TC(g_sw_SPEC, "NumOfParam %d", uNumOfParam);

			if (uNumOfParam != 4)
			{
				goto iOpen_ERROR;
			}

			// Get first parameter
			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 0, arrBuf, &uLen))) // Get first parameter.
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uDbga = AT_StrAToI(arrBuf);

			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 1, arrBuf, &uLen)))
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uRate = AT_StrAToI(arrBuf);

			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 2, arrBuf, &uLen)))
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}

			I_AM_HERE();

			uChannel = AT_StrAToI(arrBuf);

			uLen = 5;
			if (ERR_SUCCESS != (iRet = AT_Util_GetPara(Param->pPara, 3, arrBuf, &uLen)))
			{
				AT_TC(g_sw_SPEC, "AT_Util_GetPara iRet Err 0x%x", iRet);
				goto iOpen_ERROR;
			}


			I_AM_HERE();

			uLen = AT_StrAToI(arrBuf);
			wifi_test_mode_set(uDbga, uRate, uChannel, uLen);

			UINT8 arrStr[] = "iTSet: OK";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, arrStr, AT_StrLen(arrStr), pParam->nDLCI);

			if(NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_TEST:
		{

			UINT8 arrStr[] = "iTSet: void wifi_test_mode_set(UINT16 dbga,UINT16 rate,UINT16 channel,UINT16 len)";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, arrStr, AT_StrLen(arrStr), pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}


		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if(NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}



//========================================================
// VOID AT_WIFI_CmdFunc_WPOWER(AT_CMD_PARA* pParam)
// Power on/off wifimodule, have one parameter, 1 mean power on,0 mean power off
//========================================================
VOID AT_WIFI_CmdFunc_WPOWER(AT_CMD_PARA *pParam)
{

	UINT8 uStrLen   				= 1;
	INT32 iRetValue 				= 0;
	static WIFI_TEST_MODE siOpenCmd = NO_TEST;
	PAT_CMD_RESULT pResult;

	if (NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{

		AT_TC(g_sw_SPEC, "WPOWER Param->iType %d", pParam->iType);

		if(NULL == pParam->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (pParam->iType)
		{

		case AT_CMD_SET:
		{
			UINT8 uNumOfParam = 0;
			if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
			{
				goto iOpen_ERROR;
			}
			if (1 != uNumOfParam)
			{
				goto iOpen_ERROR;
			}

			iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &siOpenCmd, &uStrLen);

			if (ERR_SUCCESS != iRetValue)
			{

				AT_WIFI_Result_Err(ERR_AT_CME_INVALID_CHAR_INTEXT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
				return;
			}

			I_AM_HERE();

			if(siOpenCmd == 1)
			{
				wifi_PowerOn(g_wifiMacTest);
				g_wifiState = 0x0f;
			}
			else
			{
				wifi_PowerOff();
				g_wifiState = 0xff;
			}

			wifi_nDLCI = pParam->nDLCI;
			AT_WIFI_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

			return;
		}

		case AT_CMD_TEST:
		{

			UINT8 arrStr[] = "iOpen: NO_TEST, RX_TEST, TX_TEST, OTHER ";
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, arrStr, AT_StrLen(arrStr), pParam->nDLCI);

			if (NULL != pResult)
			{
				AT_Notify2ATM(pResult, pParam->nDLCI);
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_READ:
		{
			UINT8 *p[] = {"NO_TEST", "RX_TEST", "TX_TEST", "OTHER"};
			UINT8 arrStr[30] = {0x00,};
			AT_Sprintf(arrStr, "WPOWER: %s", p[siOpenCmd]);

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, arrStr, AT_StrLen(arrStr), pParam->nDLCI);

			if (pResult != NULL)
				AT_Notify2ATM(pResult, pParam->nDLCI);

			if (pResult != NULL)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}
		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}


//========================================================
//VOID AT_WIFI_CmdFunc_WSCAN(AT_CMD_PARA* pParam)
//Scan hotspot list,without parmeter
//========================================================
VOID AT_WIFI_CmdFunc_WSCAN(AT_CMD_PARA *pParam)
{

	PAT_CMD_RESULT pResult;

	if (NULL == pParam )
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "WSCAN Param->iType %d", Param->iType);

		if (NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_EXE:
		{
			wifi_nDLCI = pParam->nDLCI;
			#ifdef RDA_WIFI_SUPPORT
			wifi_ScanAps(5, NULL);
			#endif

			AT_WIFI_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

			return;
		}
		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}


//========================================================
//VOID AT_WIFI_CmdFunc_WJOIN(AT_CMD_PARA* pParam)
//Connect with hotspot, two parameters: num, ssid
//========================================================
VOID AT_WIFI_CmdFunc_WJOIN(AT_CMD_PARA *pParam)
{
	INT32 iRet = ERR_SUCCESS;
	wifi_conn_t tWifiConn;
	PAT_CMD_RESULT pResult;

	if (NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		AT_CMD_PARA *Param = pParam;
		if (NULL == Param->pPara)
		{
			goto iOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_SET:
		{
			UINT8 uPara1;
			UINT8 uPara1Len = 1;
			UINT8 uParaCount;
			UINT8 uSize ;
			UINT8 arrCharacterSet[20] = {0,};

			iRet = AT_Util_GetParaCount(pParam->pPara, &uParaCount);

			if(ERR_SUCCESS != iRet)
			{
				goto iOpen_ERROR;
			}

			AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uPara1, &uPara1Len);

			if(2 == uParaCount)
			{
				uSize  = SIZEOF(arrCharacterSet);
				AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, &arrCharacterSet, &uSize);

				tWifiConn.password = arrCharacterSet;
				tWifiConn.pwd_len = uSize;
			}
			else
			{
				tWifiConn.password = NULL;
				tWifiConn.pwd_len = 0;
			}

			tWifiConn.ssid = (g_ssidList + uPara1)->ssid;
			tWifiConn.ssid_len = strlen(tWifiConn.ssid);
			wifi_nDLCI = pParam->nDLCI;

			wifi_ConnectApEx(g_ssidList + uPara1, sizeof(RDA5890_BSS_DESC), &tWifiConn, 15);
			AT_WIFI_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 30, NULL, 0, pParam->nDLCI);

			return;
		}
		break;

		default:
		{
			goto iOpen_ERROR;
		}
		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (pResult != NULL)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}

//========================================================
//VOID AT_WIFI_CmdFunc_WADDR(AT_CMD_PARA* pParam)
//Get ip address,without parameter
//========================================================
VOID AT_WIFI_CmdFunc_WADDR(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult;
	g_ipReq.use_dhcp = 1;

	if(NULL == pParam)
	{
		goto iOpen_ERROR;
	}
	else
	{
		if (pParam->pPara == NULL)
		{
			goto iOpen_ERROR;
		}

		switch (pParam->iType)
		{

		case AT_CMD_EXE:
		{
			#ifdef RDA_WIFI_SUPPORT
			Wifi_ipAddrChangeReq(&g_ipReq);
			#endif
			wifi_nDLCI = pParam->nDLCI;
			AT_WIFI_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 30, NULL, 0, pParam->nDLCI);

			return;
		}
		default:
		{
			goto iOpen_ERROR;
		}

		}
	}

iOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (pResult != NULL)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;

}

#endif


