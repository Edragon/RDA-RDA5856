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
// #include <hal.h>
#include "at_common.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_bt.h"
#include "at_cfg.h"

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



extern UINT16 CSW_ATCcGsmBcdToAscii(UINT8 *pBcd,  // input
                                    UINT8 nBcdLen, UINT8 *pNumber // output
                                   );

// extern UINT32 CFW_SimGetICCID(UINT16 nUTI);

VOID AT_BT_AsyncEventProcess(COS_EVENT *pEvent)
{
	CFW_EVENT CFWev;

	CFW_EVENT *pcfwEv = &CFWev;
	PAT_CMD_RESULT pResult = NULL;

	// UINT8 tLen;
	// UINT8 i = 0;
	UINT8 res[100];
	UINT8 *pSimID   = NULL;
	UINT8 uAsciiLen = 0;
	UINT8 SIMID[22];

#ifdef AT_DUAL_SIM
	UINT8 nSim;
#endif
	AT_CosEvent2CfwEvent(pEvent, pcfwEv);
#ifdef AT_DUAL_SIM
	nSim = pcfwEv->nFlag;
#endif
	switch (pcfwEv->nEventId)
	{
#if 1
	case EV_CFW_SIM_GET_ICCID_RSP:
		AT_TC(g_sw_SPEC, "%%%%%%%%%%%%%%%%%%%%%%%%");

		if (pcfwEv->nType == 0)
		{
			AT_MemZero(SIMID, 22);
			AT_MemZero(res, 100);
			AT_Sprintf(res, "+SCID: SIM Card ID: ");  // added by yangtt an 08-05-19 for bug 8507
			pSimID    = (UINT8 *)(pcfwEv->nParam1);
			uAsciiLen = CSW_ATCcGsmBcdToAscii(pSimID, 10, SIMID);

			/*
			   for(i=0;i<uAsciiLen;i++)
			   {
			   if(i%2)
			   AT_TC(g_sw_SPEC, "BCD:0x%x\r",pSimID[i/2]);
			   //(UINT8*)(pcfwEv->nParam1)++;
			   AT_TC(g_sw_SPEC, "ASCII:%d\r",SIMID[i]);
			   //pSimID++;

			   //AT_Sprintf(res+tLen,"%u ",*pSimID);
			   //tLen=AT_StrLen(res);
			   }
			 */
			AT_StrCat(res, SIMID);
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pcfwEv->nUTI);
		}
		else
		{
			/*
			   TODO: 该处改动是为了解决CFW的一个bug，即它不能识别SIM Not Inserted这种情况。等CFW解决了这个问题后
			   要去掉下面的补丁代码。
			   * */
#  if 1
			// PATCH CODE
			// AT_20071123_CAOW_B   for bug#7088, but whether it's reasonable? wt's situation about the rsp "ERR_CME_SIM_UNKNOW"?
			if (pcfwEv->nParam1 == ERR_CME_SIM_UNKNOW)
				pcfwEv->nParam1 = ERR_CME_SIM_NOT_INSERTED;

			// AT_20071123_CAOW_E
#endif

			AT_EX_FAIL_CreateRC(pResult,
			                    pcfwEv->nParam1 - ERR_CME_PHONE_FAILURE, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pcfwEv->nUTI);

		}

		break;
#endif
	default:
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pcfwEv->nUTI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}
VOID AT_BT_CmdFunc_bOpen(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult;
	//UINT32 nRet = ERR_SUCCESS;

	//INT32 iRet = ERR_SUCCESS;


#ifdef AT_DUAL_SIM
	//UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif


	if (pParam == NULL)
		goto bOpen_ERROR;
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "-------->>bOpen Param->iType %d", Param->iType);

		if (Param->pPara == NULL) //
		{
			AT_TC(g_sw_SPEC, "-------->>xx");
			goto bOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_TEST:
		{

			UINT8 Str[] = "bOpen ";

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (pResult != NULL)
				AT_Notify2ATM(pResult, pParam->nDLCI);

			if (pResult != NULL)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_EXE:
		{

			//rda_bt_poweron_for_test();
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

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
			goto bOpen_ERROR;
		}
	}

bOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (pResult != NULL)
		AT_Notify2ATM(pResult, pParam->nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}
VOID AT_BT_CmdFunc_bClose(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult;
	// UINT32 nRet = ERR_SUCCESS;

	// INT32 iRet = ERR_SUCCESS;


#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif


	if (pParam == NULL)
		goto bOpen_ERROR;
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "-------->>bClose Param->iType %d", Param->iType);

		if (Param->pPara == NULL) //
		{
			AT_TC(g_sw_SPEC, "-------->>xx");
			goto bOpen_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_TEST:
		{

			UINT8 Str[] = "bClose ";

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (pResult != NULL)
				AT_Notify2ATM(pResult, pParam->nDLCI);

			if (pResult != NULL)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_EXE:
		{

			//rda_bt_poweroff_for_test();
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

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
			goto bOpen_ERROR;
		}
	}

bOpen_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (pResult != NULL)
		AT_Notify2ATM(pResult, pParam->nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

VOID AT_BT_CmdFunc_bSet(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult;

	if (pParam == NULL)
		goto bSet_ERROR;
	else
	{
		AT_CMD_PARA *Param = pParam;

		AT_TC(g_sw_SPEC, "-------->>bClose Param->iType %d", Param->iType);

		if (Param->pPara == NULL) //
		{
			AT_TC(g_sw_SPEC, "-------->>xx");
			goto bSet_ERROR;
		}

		switch (Param->iType)
		{
		case AT_CMD_TEST:
		{

			UINT8 Str[] = "bClose ";

			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, Str, AT_StrLen(Str), pParam->nDLCI);

			if (pResult != NULL)
				AT_Notify2ATM(pResult, pParam->nDLCI);

			if (pResult != NULL)
			{
				AT_FREE(pResult);
				pResult = NULL;
			}

			return;
		}

		case AT_CMD_EXE:
		{

			//RDA5868_send_bbtest_singlehop_inquiry();
			pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK,
			                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

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
			goto bSet_ERROR;
		}
	}

bSet_ERROR:

	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR,
	                      CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	if (pResult != NULL)
		AT_Notify2ATM(pResult, pParam->nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}


