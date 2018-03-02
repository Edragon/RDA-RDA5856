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
 *  at_main.c
 * Author
 *  Lixp
 * Date
 *  2007/11/25
 * Descriptions:
 *  ...
 *
 *********************************************************/
#include "at_common.h"
#include "at_module.h"
#include "at_parser.h"
#include "at_sa.h"
#include "cos.h"
#include "event.h"
#include "ap_common.h"
#include "sul.h"

#ifdef WIFI_SUPPORT
#include "at_cmd_wifi.h"
#include "at_cmd_tcpip.h"
#endif

#if COMPILER==2
#define BUILD_DATE 	"2016.02.26"
#endif

#if defined(GSM_SUPPORT)
#include "at_cmd_gc.h"
#include "cfw.h"
#endif /*GSM_SUPPORT*/



#define MMI_VERSION_STRING  "MMIv1.0.1"

/* BUILD_DATE is provided by the environment */
#ifdef BUILD_DATE
#define MMI_BUILD_DATE  BUILD_DATE
#else
#error BUILD_DATE is not defined ! Check your environment...
#endif

/* Leave the section attribute untouched, or this will not work */
/*lib_version_t MMI_version __attribute__((section(".mmi_version"))) = {
    .version = MMI_VERSION_STRING,
    .date = MMI_BUILD_DATE
};*/

extern AT_ModuleInfo g_AtModule;

//extern VOID AT_Result_OK(UINT32 uReturnValue, UINT32 uResultCode, UINT8 nDelayTime, UINT8* pBuffer, UINT16 nDataSize,
//                         UINT8 nDLCI);


static VOID AT_RunCommand(VOID);
VOID AT_RunNextCommand(VOID)
{
    // maybe here it need to add some codes for URC that like +RING
    AT_RunCommand();
    return;
}
/*
Backup the data of g_AtModule.cmd_buf to .cmd_buf_bak field.
**/
void BackupCmdBuffer(void)
{
    if (g_AtModule.cmd_buf.DataLen != 0)
        memcpy(&g_AtModule.cmd_buf_bak, &g_AtModule.cmd_buf, sizeof(AT_CMD_BUF));
}
void RestoreCmdBuffer(void)
{
    memcpy(&g_AtModule.cmd_buf, &g_AtModule.cmd_buf_bak, sizeof(AT_CMD_BUF));
}
BOOL FirstCmdIsBSlash(UINT8 nDLCI)
{
    AT_CMD* pCmdList = NULL;
    pCmdList = *AT_GetCommandList();
    if (pCmdList == NULL)
        return FALSE;
    while (pCmdList != NULL)
    {
        if (strcmp(pCmdList->pName, "A/") == 0)
            return TRUE;
        pCmdList = pCmdList->pNext;
    }
    return FALSE;
}

INT32 AT_CommandLineProcess(UINT8* pBuf, UINT32 len)
{
    UINT32 uResult;
    INT32 iRet = ERR_SUCCESS;
    UINT32 i;
    AT_PARSE_RESULT sParseResult;
    SA_COMMAND_TABLE_T* pSaCmdTable = (SA_COMMAND_TABLE_T*)g_CommandTable;
    AT_CMD_NODE* pTmp;
    BOOL bFine = FALSE;
    if (pBuf == NULL && len == 0)
    {
        return- 1;
    }
    // Create command list via AT_CmdParser(), if the command list is not empty,
    // AT_CmdParser() will append the new commands at the end of the command list.
    AT_TC(g_sw_SA, "Call syntax parser, <pBuf>=%x,<len>=%u", pBuf, len);
    AT_MemSet(&sParseResult, 0, SIZEOF(AT_PARSE_RESULT));
    uResult = AT_CmdLineSyntaxParser(pBuf, len, &sParseResult);
    if (uResult == ERR_SUCCESS)
    {
        AT_TC(g_sw_ATE, "AT_CmdParser() success!!!\n");
        while (sParseResult.pNodeHead != NULL)
        {
            bFine = FALSE;
            i     = 0;
            while (NULL != pSaCmdTable[i].pName)
            {
                if (AT_StrCaselessCmp(pSaCmdTable[i].pName, sParseResult.pNodeHead->szName) == 0)
                {
                    AT_CMD* pAtCmd;
                    pAtCmd = (AT_CMD*)AT_MALLOC(sizeof(AT_CMD));
                    if (pAtCmd == NULL)
                    {
                        return- 1;
                    }
                    AT_MemSet(pAtCmd, 0x00, sizeof(AT_CMD));
                    pAtCmd->iType = sParseResult.pNodeHead->uType;
                    AT_StrCpy(pAtCmd->pName, sParseResult.pNodeHead->szName);
                    AT_StrCpy(pAtCmd->pPara, sParseResult.pNodeHead->szParam);
                    AT_StrUpr(pAtCmd->pName);
                    // AT_TC(1, "AT Syntax Parser: CMD: %s, PARAM: %s\n", pAtCmd->pName, pAtCmd->pPara);
                    AT_AddCmdToList(pAtCmd);
                    bFine = TRUE;
                    break;
                }
                i++;
            }
            if (bFine != TRUE)
            {
                iRet = -1;
                break;
            }
            pTmp = sParseResult.pNodeHead;
            sParseResult.pNodeHead = sParseResult.pNodeHead->pNext;
            AT_FREE(pTmp);
            pTmp = NULL;
        }
        // Free the spare node in the parse result node chain.
        while (sParseResult.pNodeHead != NULL)
        {
            pTmp                   = sParseResult.pNodeHead;
            sParseResult.pNodeHead = sParseResult.pNodeHead->pNext;
            AT_FREE(pTmp);
            pTmp = NULL;
        }
    }
    else
    {
        AT_TC(g_sw_ATE, "Call AT_CmdLineSyntaxParser() failed!error code = %d.", uResult);
        iRet = uResult;
    }
    // clean the command list when paser fail
    if (ERR_SUCCESS != iRet)
    {
        // if parser fail, clean command list
        AT_ClearCommandList();
        // need more work to show error information
    }
    return iRet;
}
VOID AT_RunCommand(VOID)
{
    AT_CMD** ppCmdList = NULL;
	AT_CMD* pCmd = NULL;

    ppCmdList = AT_GetCommandList();
    pCmd = (AT_CMD*)*ppCmdList;
    if (ppCmdList != NULL)
    {
        // AT_TC(g_sw_ATE, "-------------The addr of the pCmd is: 0x%x-------------", (AT_CMD *)*ppCmdList);
        if (pCmd != NULL) // carry out the command in the list
        {
            AT_TC(g_sw_ATE, "pCmd->pName:%s,func=%x,para=%s", pCmd->pName,pCmd->pCmdFunc,pCmd->pPara);
            if (pCmd->pCmdFunc == NULL)
            {
                AT_TC(g_sw_ATE, "%s routine is not avaiable!!!\n", pCmd->pName);
                // *ppCmdList = NULL;
                AT_ClearCommandList();
                // error handler
                // show error infomation
            }
            else  // carry out command
            {
                AT_CMD_PARA para;
                AT_CMD_BUF* pCmdBuf = AT_GetCmdBuf();
                AT_MemSet(&para, 0, sizeof(AT_CMD_PARA));
                // prepare the parameters
                para.iType     = pCmd->iType;
                para.pPara     = pCmd->pPara;
                para.uCmdStamp = pCmd->uCmdStamp;
                if (pCmdBuf->DataLen > 0 && (AT_GetStatus() == AT_STATUS_IP_DATA))
                {
                    para.pExData    = pCmdBuf->DataBuf;
                    para.iExDataLen = pCmdBuf->DataLen;
                }
                else
                {
                    para.pExData    = NULL;
                    para.iExDataLen = 0;
                }
                // run command
                if (NULL != pCmd->pCmdFunc)
                {
                    pCmd->pCmdFunc(&para);
                }
                else
                {
                    // ......Need to add some codes for dynamic adding at command
                    AT_TC(g_sw_ATE, "NULL == pCmd->pCmdFunc: %s", pCmd->pName);
                }
            }
        }
        else
        {
            AT_TC(g_sw_ATE, "NULL == pCmd");
        }
    }
    else
    {
        AT_TC(g_sw_ATE, "Command List Not Available!!!\n");
        return;
    }
    return;
}

VOID AT_Result_OK(UINT32 uReturnValue,
                  UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize, UINT8 nDLCI, UINT8 nSim)
{

	PAT_CMD_RESULT pResult = NULL;

//	AT_BACKSIMID(nSim);
	// 填充结果码
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

VOID AT_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI, UINT8 nSim)
{
	PAT_CMD_RESULT pResult = NULL;

//	AT_BACKSIMID(nSim);
	// 填充结果码
	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR, uErrorCode, nErrorType, 0, 0, 0, nDLCI);

	AT_Notify2ATM(pResult, nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

#if defined(GSM_SUPPORT)
BOOL AT_GsmAsyncEventProcess(COS_EVENT* pEvent);
#endif /*GSM_SUPPORT*/

BOOL AT_AsyncEventProcess(COS_EVENT* pEvent)
{
	if(0)
	{
	}
#if defined(WIFI_SUPPORT) && !defined(MCU_WIFI_PLAT)
	else if(AT_WIFI_AsyncEventProcess(pEvent))
	{
	}
#endif /* WIFI_SUPPORT */
#ifdef __AT_CONTROL_BT__
	else if(AT_ATControlBT_AsyncEventProcess(pEvent))
	{
	}
#endif
#if defined(GSM_SUPPORT)
	else if(AT_GsmAsyncEventProcess(pEvent))
	{
	}
#endif /*GSM_SUPPORT*/
#if defined(APP_SUPPORT_UART_PLAY) && (APP_SUPPORT_UART_PLAY == 1)
	else if(AT_MUSIC_UartPlay_AsyncEventProcess(pEvent))
	{
	}
#endif /* APP_SUPPORT_UART_PLAY */
	else
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef __BT_RDABT__
bool BT_IsInquiring(void)
{
    return TRUE;
}
UINT32 rdabt_adp_uart_rx()
{
    return ERR_SUCCESS;
}
#endif

#if defined(GSM_SUPPORT)
BOOL gbPowerCommFlag[CFW_SIM_COUNT] = {FALSE, FALSE};
BOOL gbPowerCommMsgFlag[CFW_SIM_COUNT] = {FALSE, FALSE};

BOOL CFW_CheckPowerCommStatus(CFW_SIM_ID nSimID)
{
    return gbPowerCommFlag[nSimID];
}
extern CFW_INIT_INFO cfwInitInfo;

static BOOL AT_CFW_InitProcess(COS_EVENT *pEvent)
{
    CFW_EVENT cfw_event;
    UINT32 ats_stat = 0;
    UINT8 nSim;

    AT_MemSet(&cfw_event, 0, SIZEOF(cfw_event));
    AT_CosEvent2CfwEvent(pEvent, &cfw_event);
//    AT_FREE((PVOID)pEvent);
    nSim = cfw_event.nFlag;

    if (cfw_event.nEventId == EV_CFW_INIT_IND)
    {
        // //////////////////////////////////////////
        // Report a URC (just for MAL)
#if (MAL_EXTENDED == 1)
        PAT_CMD_RESULT pResult = NULL;
        UINT8 urc_str[100]     = { 0 };

        AT_Sprintf(urc_str, "^CINIT: %u, %u, %u", cfw_event.nType, cfw_event.nParam1, cfw_event.nParam2);
        pResult = AT_CreateRC(CSW_IND_NOTIFICATION,
                CMD_RC_CR,
                CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0, urc_str, (UINT16)AT_StrLen(urc_str), 0);
        AT_TC(g_sw_SA, "send an URC to ATM, URC string=%s", urc_str);
        AT_Notify2ATM(pResult, cfw_event.nUTI);
        if (pResult != NULL)
        {
            AT_FREE(pResult);
            pResult = NULL;
        }
#endif
        if (g_gc_pwroff)
        {
            g_gc_pwroff = 0;  // hameina[+]2007.11.15 support CFUN
            AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, cfw_event.nUTI, nSim);
            return TRUE;
        }

        // g_cfg_cfun = 5;//switch on was called
        AT_TC(g_sw_SA, "EV_CFW_INIT_IND %d %d", cfw_event.nType, cfw_event.nParam1);

        switch (cfw_event.nType)
        {
            case CFW_INIT_STATUS_NO_SIM: //
                {
#if defined(USER_DATA_CACHE_SUPPORT)
                    VDS_CacheFlushAllowed();
#endif
                    AT_ModuleSetInitResult(AT_MODULE_INIT_NO_SIM);
                    AT_MemCpy(&(cfwInitInfo.noSim[nSim]), &cfw_event, sizeof(cfw_event));

                    static BOOL bSetComFlag = FALSE;
                    UINT8 nSimStatus = 0;//CFW_SIM_ABSENT
                    UINT8 nOtherSimID = 0;
                    UINT16 nUti = 0;

                    if(FALSE == bSetComFlag)
                    {
                        if(CFW_SIM_0 == nSim)
                        {
                            nOtherSimID = CFW_SIM_1;
                        }
                        else
                        {
                            nOtherSimID = CFW_SIM_0;
                        }

                        nSimStatus = (UINT8)CFW_GetSimStatus(nOtherSimID);
                        if(0 == nSimStatus)
                        {
                            bSetComFlag = TRUE;
                            CFW_GetFreeUTI(0, (UINT8 *)&nUti);
                            CFW_SetComm(CFW_DISABLE_COMM, 1, nUti, CFW_SIM_1);
                            gbPowerCommFlag[CFW_SIM_1] = TRUE;
                            gbPowerCommMsgFlag[CFW_SIM_1] = TRUE;
                        }
                    }
                    break;
                }
            case CFW_INIT_STATUS_SIM_DROP:
                {
                    AT_CC_Result_OK(CSW_IND_NOTIFICATION, CMD_RC_SIMDROP, 0, 0, 0, cfw_event.nUTI, nSim);	// hameina[+] ring info should be set out
                }
            case CFW_INIT_STATUS_SIM:
                if (cfw_event.nParam1 & CFW_INIT_SIM_PIN1_READY)
                {
                    AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_AUTH_READY);
                }

                if (cfw_event.nParam1 & CFW_INIT_SIM_WAIT_PIN1)
                {
                    AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_AUTH_WAIT_PIN1);
                }

                if (cfw_event.nParam1 & CFW_INIT_SIM_WAIT_PUK1)
                {
                    AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_AUTH_WAIT_PUK1);
                }

                AT_MemCpy(&(cfwInitInfo.sim), &cfw_event, sizeof(cfw_event));
                break;

            case CFW_INIT_STATUS_SAT:
                {
                    if (!(cfw_event.nParam1 & CFW_INIT_SIM_SAT))
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_SAT_OK);
                    }
                    else//	if (cfw_event.nParam1 & CFW_INIT_SIM_SAT)
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_SAT_ERROR);
                    }

                    AT_MemCpy(&(cfwInitInfo.sat), &cfw_event, sizeof(cfw_event));

                    break;
                }

            case CFW_INIT_STATUS_SIMCARD:
                {
                    if ((cfw_event.nParam1 & CFW_INIT_SIM_CARD_CHANGED) || (cfw_event.nParam1 & CFW_INIT_SIM_NORMAL_CARD))
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_NORMAL_CHANGED_SIM_CARD);
#ifdef AT_FTP_SUPPORT
                        TCpIPInit();
#endif
                    }
                    else if (cfw_event.nParam1 & CFW_INIT_SIM_NORMAL_CARD)
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_NORMAL_SIM_CARD);
                    }
                    else if (cfw_event.nParam1 & CFW_INIT_SIM_CARD_CHANGED)
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_CHANGED_SIM_CARD);
                    }
                    else if (cfw_event.nParam1 & CFW_INIT_SIM_TEST_CARD)
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_TEST_SIM_CARD);
                    }

                    AT_MemCpy(&(cfwInitInfo.simCard), &cfw_event, sizeof(cfw_event));

                    break;
                }

            case CFW_INIT_STATUS_SMS:  // SMS初始化完毕
                {
                    UINT32 nUTI = 0x00;

                    /* get the UTI and free it after finished calling */
                    if (0 == (nUTI = AT_AllocUserTransID()))
                    {
                        AT_SMS_Result_Err(ERR_AT_CMS_ME_FAIL, NULL, 0, "cmgr: malloc UTI error", cfw_event.nUTI);
                        return FALSE;
                    }

                    CFW_SMS_STORAGE_INFO nStorageInfo;

                    SUL_ZeroMemory8(&nStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                    nStorageInfo.totalSlot = pEvent->nParam2 & 0xFF;

                    CFW_CfgSetSmsStorageInfo(&nStorageInfo, CFW_SMS_STORAGE_SM, nSim);
                    AT_TC(g_sw_SA, , TSTXT("Sim SMS GetTotalNum Success Total sms %d\n"), nStorageInfo.totalSlot);

                    nStorageInfo.totalSlot = PHONE_SMS_ENTRY_COUNT / NUMBER_OF_SIM;
                    AT_TC(g_sw_SA, , TSTXT("ME SMS GetTotalNum Success Total sms %d\n"), nStorageInfo.totalSlot);
                    CFW_CfgSetSmsStorageInfo(&nStorageInfo, CFW_SMS_STORAGE_ME, nSim);

                    //Remove SMS Init for Android issue.
                    //Add by XP at 20130129 begin;
#ifdef CHIP_HAS_AP
                    AT_SmsMoInit(cfw_event.nUTI, nSim);
#else
                    CFW_SmsMoInit(cfw_event.nUTI, nSim);

#endif
                    static BOOL bSetComFlag = FALSE;
                    UINT8 nSimStatus = 0;
                    UINT8 nOtherSimID = 0;
                    UINT16 nUti = 0;

                    if(FALSE == bSetComFlag)
                    {
                        if(CFW_SIM_0 == nSim)
                        {
                            nOtherSimID = CFW_SIM_1;
                        }
                        else
                        {
                            nOtherSimID = CFW_SIM_0;
                        }
                        nSimStatus = (UINT8)CFW_GetSimStatus(nOtherSimID);
                        if(0 == nSimStatus)//0 means CFW_SIM_ABSENT
                        {
                            gbPowerCommFlag[nOtherSimID] = TRUE;
                            gbPowerCommMsgFlag[nOtherSimID] = TRUE;
                            bSetComFlag = TRUE;
                            CFW_GetFreeUTI(0, (UINT8 *)&nUti);
                            CFW_SetComm(CFW_DISABLE_COMM, 1, nUti, nOtherSimID);
                        }
                    }
                    break;
                }

            case CFW_INIT_STATUS_PBK:
                {
                    if (0 == (cfw_event.nParam1))
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_PBK_OK);
                    }

                    if (1 == (cfw_event.nParam1))
                    {
                        AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_PBK_ERROR);
                    }
//                    AT_PBK_Init(nSim);
                    AT_MemCpy(&(cfwInitInfo.pbk), &cfw_event, sizeof(cfw_event));
                    ats_stat = SA_GetSysStatus(SA_ATS_INIT_STATUS);
                    ats_stat |= SA_ATS_PBK_READY;
                    SA_SetSysStatus(SA_ATS_INIT_STATUS, ats_stat);
#if defined(USER_DATA_CACHE_SUPPORT)
                    VDS_CacheFlushAllowed();
#endif
                    break;
                }
            case CFW_INIT_STATUS_AUTODAIL:
                {
#ifndef __NGUX_PLATFORM__

                    UINT32 ret = 0x00;
                    UINT8 DestNumber[2];

                    CSW_TRACE(CFW_SHELL_TS_ID, "CFW_INIT_STATUS_AUTODAIL\n");
                    DestNumber[0] = 0x11;
                    DestNumber[1] = 0xF2;
                    if (ERR_SUCCESS == (ret = CFW_CcEmcDial(DestNumber, 2, nSim)))
                    {
                        CSW_TRACE(CFW_SHELL_TS_ID, "Auto dial 112 Successl!!!\n");
                    }
                    else
                    {
                        CSW_TRACE(CFW_SHELL_TS_ID, "Auto dial 112 failed,The errorcode is 0x%x!!!\n", ret);
                        while (ERR_SUCCESS != ret)
                        {
                            COS_Sleep(500);
                            ret = CFW_CcEmcDial(DestNumber, 2, nSim);
                        }
                    }
#endif
                    break;
                }
            case CFW_INIT_STATUS_NET:
                {
                    AT_TC(g_sw_ATE, "CFW_INIT_STATUS_NET\n");
#ifdef CFW_GPRS_SUPPORT
                    CFW_GprsAtt(CFW_GPRS_ATTACHED, 2,  nSim);
#endif
                    break;
                }
            default:
                break;
        }
    }
    else if (cfw_event.nEventId == EV_CFW_SRV_STATUS_IND)
    {
        switch (cfw_event.nType)
        {
            case 0x10: //
                AT_TC(g_sw_ATE, "CFW SMS INIT success\n");
                AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_SMS_OK);

                break;

            case 0x11:
                AT_ModuleSetInitResult(AT_MODULE_INIT_SIM_SMS_ERROR);
                AT_TC(g_sw_ATE, "CFW SMS INIT Error\n");

                break;

            default:
                break;

        }
        UINT32 nUTI = 0x00;

        /* get the UTI and free it after finished calling */
        if (0 == (nUTI = AT_AllocUserTransID()))
        {
            AT_SMS_Result_Err(ERR_AT_CMS_ME_FAIL, NULL, 0, "cmgr: malloc UTI error", cfw_event.nUTI);
            return FALSE;
        }

#ifdef  __NGUX_PLATFORM__
        CFW_SmsInitComplete(TRUE, nUTI, nSim);
#else
        CFW_SmsInitComplete(nUTI, nSim);
#endif

        AT_SMS_INIT(nSim);
        AT_MemCpy(&(cfwInitInfo.sms[nSim]), &cfw_event, sizeof(cfw_event));

        // Notify SA that the SMS (ATS) module is ready for use.
        ats_stat = SA_GetSysStatus(SA_ATS_INIT_STATUS);
        ats_stat |= SA_ATS_SMS_READY;
        SA_SetSysStatus(SA_ATS_INIT_STATUS, ats_stat);
        AT_TC(g_sw_SA, TSTXT("set gATCurrentuCmer_ind nSim[%d] to 2\n"), nSim);
//        gATCurrentuCmer_ind[nSim]           = 2;
//        gATCurrentAtSMSInfo[nSim].sCnmi.nMt = 2;
    }
    else if(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP == cfw_event.nEventId )
    {
        static BOOL bSetComFlag = FALSE;
        UINT8 nSimStatus = 0;
        UINT8 nOtherSimID = 0;
        UINT16 nUti = 0;

        if(FALSE == bSetComFlag)
        {
            bSetComFlag = TRUE;
            if(CFW_SIM_0 == nSim)
            {
                nOtherSimID = CFW_SIM_1;
            }
            else
            {
                nOtherSimID = CFW_SIM_0;
            }
            nSimStatus = (UINT8)CFW_GetSimStatus(nOtherSimID);
            if(0 == nSimStatus || 4 == nSimStatus)
            {
                CFW_GetFreeUTI(0, (UINT8 *)&nUti);
                CFW_SetComm(CFW_DISABLE_COMM, 1, nUti, nOtherSimID);
            }
        }
    }
    return FALSE;
}

BOOL AT_GsmAsyncEventProcess(COS_EVENT* pEvent)
{
    switch(pEvent->nEventId)
      {
	case EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP:
	case EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP:
	case EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP:
	case EV_CFW_NW_SIGNAL_QUALITY_IND:
	case EV_CFW_NW_NETWORKINFO_IND:
	case EV_CFW_NW_REG_STATUS_IND:
	case EV_CFW_NW_STORELIST_IND:
	case EV_CFW_NW_SET_BAND_RSP:
	case EV_CFW_NW_GET_IMEI_RSP:
	case EV_CFW_NW_GET_SIGNAL_QUALITY_RSP:
	case EV_CFW_NW_SET_REGISTRATION_RSP:
	case EV_CFW_NW_GET_AVAIL_OPERATOR_RSP:
	case EV_CFW_NW_ABORT_LIST_OPERATORS_RSP:
	case EV_CFW_SET_COMM_RSP:
	  {
	    AT_NW_AsyncEventProcess(pEvent);
	    break;
	  }

	case EV_CFW_NW_DEREGISTER_RSP:
	  {
	    if (!g_gc_pwroff)
	      {
		AT_NW_AsyncEventProcess(pEvent);
	      }

	    break;
	  }


    case EV_CFW_CC_SPEECH_CALL_IND:
//        if (bMuxActiveStatus)
//            SA_SetSysStatusMux(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND, HIUINT16(pEvent->nParam3));
//        else
            SA_SetSysStatus(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND);
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }
        //wait platform  submit  reg simulator support
    case EV_CFW_REG_UPDATE_IND:
        {
//            AT_EMOD_CFGUPDATE_Indictation(pEvent);
        }
        break;
    case EV_CFW_CC_ERROR_IND:
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_RELEASE_CALL_IND:
//        if (bMuxActiveStatus)
//            SA_SetSysStatusMux(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND, HIUINT16(pEvent->nParam3));
//        else
            SA_SetSysStatus(AT_EVENT, EV_CFW_CC_RELEASE_CALL_IND);
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_CALL_INFO_IND:
    case EV_CFW_CC_PROGRESS_IND:
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_CRSSINFO_IND:
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP:
//        if (bMuxActiveStatus)
//            SA_SetSysStatusMux(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND, HIUINT16(pEvent->nParam3));
//        else
            SA_SetSysStatus(AT_EVENT, EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP);
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP:
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_INITIATE_SPEECH_CALL_RSP:
//        if (bMuxActiveStatus)
//            SA_SetSysStatusMux(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND, HIUINT16(pEvent->nParam3));
//        else
            SA_SetSysStatus(AT_EVENT, EV_CFW_CC_INITIATE_SPEECH_CALL_RSP);
        {
            AT_CC_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_CC_RELEASE_CALL_RSP:
//        if (bMuxActiveStatus)
//            SA_SetSysStatusMux(AT_EVENT, EV_CFW_CC_SPEECH_CALL_IND, HIUINT16(pEvent->nParam3));
//        else
            SA_SetSysStatus(AT_EVENT, EV_CFW_CC_RELEASE_CALL_RSP);

        AT_CC_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_SS_SET_CALL_WAITING_RSP:
    case EV_CFW_SS_QUERY_CALL_WAITING_RSP:
    case EV_CFW_SS_SET_CALL_FORWARDING_RSP:
    case EV_CFW_SS_QUERY_CALL_FORWARDING_RSP:
    case EV_CFW_SS_QUERY_CLIP_RSP:
    case EV_CFW_SS_QUERY_CLIR_RSP:
    case EV_CFW_SS_QUERY_COLR_RSP:
    case EV_CFW_SS_QUERY_COLP_RSP:
    case EV_CFW_SS_SEND_USSD_RSP:
    case EV_CFW_SIM_GET_PUCT_RSP:
    case EV_CFW_SIM_GET_ACM_RSP: // [+Changyg moved from SMS
    case EV_CFW_SIM_SET_ACM_RSP: // [+Changyg moved from SMS
    case EV_CFW_SIM_SET_PUCT_RSP:
    case EV_CFW_SS_USSD_IND:
    case EV_CFW_SS_QUERY_FACILITY_LOCK_RSP:
    case EV_CFW_SS_SET_FACILITY_LOCK_RSP:
        AT_SS_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_SIM_CHANGE_PWD_RSP:
    case EV_CFW_SIM_GET_AUTH_STATUS_RSP:
    case EV_CFW_SIM_ENTER_AUTH_RSP:
    case EV_CFW_SIM_SET_FACILITY_LOCK_RSP:
    case EV_CFW_SIM_GET_FACILITY_LOCK_RSP:
    case EV_CFW_SIM_DELETE_MESSAGE_RSP:
    case EV_CFW_SIM_WRITE_MESSAGE_RSP:
    case EV_CFW_SIM_READ_MESSAGE_RSP:
    case EV_CFW_SIM_LIST_MESSAGE_RSP:
    case EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP:
    case EV_CFW_SIM_READ_BINARY_RSP:
    case EV_CFW_SIM_UPDATE_BINARY_RSP:
    case EV_CFW_SIM_GET_SMS_PARAMETERS_RSP:
    case EV_CFW_SIM_SET_SMS_PARAMETERS_RSP:
    case EV_CFW_SIM_GET_MR_RSP:
    case EV_CFW_SIM_SET_MR_RSP:
    case EV_CFW_SIM_COMPOSE_PDU_RSP:
    case EV_CFW_SIM_SET_PBKMODE_RSP:
    case EV_CFW_SIM_GET_PBKMODE_RSP:
    case EV_CFW_SIM_INIT_SMS_RSP:
    case EV_CFW_SIM_RESET_RSP:
    case EV_CFW_SIM_GET_ACMMAX_RSP:
    case EV_CFW_SIM_SET_ACMMAX_RSP:
    case EV_CFW_SIM_GET_FILE_STATUS_RSP:
    case EV_CFW_SIM_READ_RECORD_RSP:
    case EV_CFW_SIM_UPDATE_RECORD_RSP:
        AT_SIM_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_SIM_GET_ICCID_RSP:
//        AT_Emod_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_SIM_GET_PROVIDER_ID_RSP:
        AT_ID_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_SAT_CMDTYPE_IND:
    case EV_CFW_SAT_ACTIVATION_RSP:
    case EV_CFW_SAT_GET_STATUS_RSP:
    case EV_CFW_SAT_RESPONSE_RSP:
    case EV_CFW_SAT_GET_INFORMATION_RSP:
        AT_SAT_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_NEW_SMS_IND:
    case EV_CFW_CB_MSG_IND:
    case EV_CFW_CB_PAGE_IND:
    case EV_CFW_SMS_INFO_IND:
    case EV_CFW_SMS_DELETE_MESSAGE_RSP:
    case EV_CFW_SMS_LIST_MESSAGE_RSP:
    case EV_CFW_SMS_READ_MESSAGE_RSP:
    case EV_CFW_SMS_SEND_MESSAGE_RSP:
    case EV_CFW_SMS_WRITE_MESSAGE_RSP:
    case EV_CFW_SMS_GET_STORAGE_RSP:
    case EV_CFW_SMS_SET_STORAGE_RSP:
    case EV_CFW_SMS_SET_CENTER_ADDR_RSP:
    case EV_CFW_SMS_GET_CENTER_ADDR_RSP:
    case EV_CFW_SMS_SET_TEXT_MODE_PARAM_RSP:
    case EV_CFW_SMS_GET_TEXT_MODE_PARAM_RSP:
    case EV_CFW_SMS_LIST_MESSAGE_HRD_RSP:
    case EV_CFW_SMS_COPY_MESSAGE_RSP:
    case EV_CFW_SMS_SET_UNREAD2READ_RSP:
    case EV_CFW_SMS_SET_UNSENT2SENT_RSP:
    case EV_CFW_SMS_SET_READ2UNREAD_RSP:
        AT_SMS_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_PBK_ADD_RSP:
    case EV_CFW_PBK_UPDATE_ENTRY_RSP:
    case EV_CFW_PBK_DELETE_RSP:
    case EV_CFW_PBK_DELETE_BATCH_ENTRIES_RSP:
    case EV_CFW_PBK_FIND_ENTRY_RSP:
    case EV_CFW_PBK_LIST_ENTRIES_RSP:
    case EV_CFW_PBK_GET_ENTRY_RSP:
    case EV_CFW_PBK_COPY_ENTRIES_RSP:
    case EV_CFW_PBK_ADD_ENTRY_ITEM_RSP:
    case EV_CFW_PBK_GET_ENTRY_ITEMS_RSP:
    case EV_CFW_PBK_DELETE_ENTRY_ITEM_RSP:
    case EV_CFW_PBK_ADD_CALLLOG_ENTTRY_RSP:
    case EV_CFW_PBK_GET_CALLLOG_ENTRY_RSP:
    case EV_CFW_PBK_LIST_CALLLOG_ENTRIES_RSP:
    case EV_CFW_PBK_DELETE_CALLLOG_ALL_ENTRIES_RSP:
    case EV_CFW_PBK_CANCEL_RSP:
    case EV_CFW_PBK_DELETE_CALLLOG_OLDEST_ENTRY_RSP:
    case EV_CFW_SIM_ADD_PBK_RSP:
    case EV_CFW_SIM_DELETE_PBK_ENTRY_RSP:
    case EV_CFW_SIM_GET_PBK_ENTRY_RSP:
    case EV_CFW_SIM_LIST_PBK_ENTRY_RSP:
    case EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP:
    case EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP:
    case EV_CFW_SIM_GET_PBK_STRORAGE_RSP:
//        AT_PBK_AsyncEventProcess(pEvent);
        break;

#ifdef CFW_GPRS_SUPPORT
    case EV_CFW_GPRS_CXT_ACTIVE_IND:
    case EV_CFW_GPRS_CXT_DEACTIVE_IND:
    case EV_CFW_GPRS_MOD_IND:
    case EV_CFW_GPRS_STATUS_IND:
        // GPRS Response.
    case EV_CFW_GPRS_CTX_MODIFY_ACC_RSP:
    case EV_CFW_GPRS_ATT_RSP:
    case EV_CFW_GPRS_ACT_RSP:
    case EV_CFW_GPRS_CXT_ACTIVE_RSP:
    case EV_CFW_GPRS_MOD_RSP:
        AT_GPRS_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_GPRS_DATA_IND:
//        PsDownlinkDataHandler((CFW_GPRS_DATA *)pEvent->nParam1, LOUINT8(pEvent->nParam2));
        break;

    case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
    case EV_CFW_TCPIP_SOCKET_SEND_RSP:
    case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
    case EV_CFW_TCPIP_REV_DATA_IND:
    case EV_CFW_DNS_RESOLV_ERR_IND:
    case EV_CFW_DNS_RESOLV_SUC_IND:
//        AT_TCPIP_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_PPP_DATA_IND:
//        PPPDownlinkDataHandler((UINT8 *)pEvent->nParam1, (UINT16)pEvent->nParam2);
        break;
#endif
    case EV_CFW_TSM_INFO_IND:
        AT_NW_AsyncEventProcess(pEvent);
        break;

    case EV_CFW_TSM_INFO_END_RSP:
        break;

    case EV_PM_BC_IND:
        {
            // BATTERY LLP[ADD]
//            AT_SPECIAL_Battery_AsyncEventProcess(pEvent);
            break;
        }

    case EV_CFW_EXIT_IND:
    case EV_TIM_ALRAM_IND: // hameina[+]07-10-29 for alarm URC
        {
            AT_GC_AsyncEventProcess(pEvent);
        }
        break;
#ifdef CFW_GPRS_SUPPORT
    case EV_CFW_PPP_TERMINATE_IND:
//        if (bMuxActiveStatus)
//            AT_GPRS_PPP_Terminate_Ind(pEvent->nParam3);
//        else
    	at_PppEndProcess();
        break;
#endif



#ifdef AT_FTP_SUPPORT

        case EV_CFW_FTP_LONGIN_RSP:
        case EV_CFW_FTP_FILE_DOWNLOAD_RSP:
        case EV_CFW_FTP_LIST_RSP:
        case EV_CFW_FTP_CHANGEDIR_RSP:
        case EV_CFW_FTP_REC_DATA_IND:
            AT_FTP_AsyncEventProcess(pEvent);
            break;

#endif

	default:
		if(AT_CFW_InitProcess(pEvent))
		{
			break;
		}
	return FALSE;
      }

    return TRUE;

}
#endif /*GSM_SUPPORT*/


