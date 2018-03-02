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
 *  at_common.c
 * Author
 *  Felix
 * Date
 *  2008/01/07
 * Descriptions:
 *  Public functions definition
 *
 *********************************************************/

#include "at_common.h"
#include "at_sa.h"
#include "at_module.h"
#include "at_parser.h"
#include "at_utility.h"
#include "sul.h"

#if defined(GSM_SUPPORT)
#include "cfw.h"
#endif /*GSM_SUPPORT*/



extern AT_ModuleInfo g_AtModule;
extern UINT8 DlciGetCid(UINT8 nDLCI);
// //////////////////////////////////////////////////////////////////////////////
// UTI: user transaction id table defines
// add by wangqunyang 2008.06.02
// //////////////////////////////////////////////////////////////////////////////
UINT16 nATUserTransIdSpareHeader = 0;
UINT16 nATUserTransIdSpareTail   = 0;


static UINT8 CmdModeOutputUrcLink(UINT8 nDLCI)
{
    // -----------------------------------------------//
    // Command mode ATM will write the urc's into uart.
    // Otherwise reserve the urc's in the buf link.
    // The current urc event will drive the pre-urc be
    // wrote into uart before current urc.
    // Need to consider the buf link size and the status
    // of the phone [call control]-->look at the code://if(0 == uiAtMode && AT_ACTIVE_STATUS_CALL_IN_PROGRESS != AT_GetPhoneActiveStatus())
    //
    // -----------------------------------------------//
    UINT8 uiCounter = 0;
    UINT8 uiAtMode  = 0;
    AT_IND** ppIndList = NULL;
    // if(0 == uiAtMode && AT_ACTIVE_STATUS_CALL_IN_PROGRESS != AT_GetPhoneActiveStatus())
    uiAtMode  = AT_GetMode();
    ppIndList = AT_GetIndicationList();
    if (0 == uiAtMode)
    {
        while (*ppIndList != NULL)
        {
            AT_IND* pInd = *ppIndList;
            // CmdResultNotification(&(pInd->result));
            if (NULL != pInd)
            {
                if (pInd->result.pData != NULL && pInd->result.nDataSize > 0)
                {
                    unsigned char s[] = { 0, 0 };
                    s[0] = CHAR_LF;
                    AT_WriteUart(s, strlen(s));
                    AT_WriteUart(pInd->result.pData, pInd->result.nDataSize);
                }
                if (uRCPresentMode == 0)
                {
                    UINT8 msg[128] = { 0 };
                    AT_MemSet(msg, '\0', 128);
                    AT_GetError_DCE(uRCFormatMode, pInd->result.uResultCode, msg);
                    if (AT_StrLen(msg))
                    {
                        AT_TC(g_sw_ATE, "%s", msg);
                        AT_WriteUart(msg, AT_StrLen(msg));
                    }
                }
            }
            AT_FreeCurInd(ppIndList, pInd);
            uiCounter++;
            ppIndList = AT_GetIndicationList();
        }
    }
    return uiCounter;
}

static UINT8 OutputUrcNode(PAT_CMD_RESULT pCmdResult)
{
    if (pCmdResult->pData != NULL && pCmdResult->nDataSize > 0)
    {
        UINT8 s[3] = { 0 };
        s[0] = CHAR_CR; //AT_GC_CfgGetS3Value();
        s[1] = CHAR_LF; //AT_GC_CfgGetS4Value();
        // Output prefix <cr><lf> when ATV setting is 1.
        if (uRCFormatMode == 1)
            AT_WriteUart(s, strlen(s));
        AT_WriteUart(pCmdResult->pData, pCmdResult->nDataSize);
        // Output suffix <cr><lf>.
        AT_WriteUart(s, strlen(s));
    }
    if (uRCPresentMode == 0)
    {
        UINT8 msg[128] = { 0 };
        // Do not output anything (includes the suffix "<cr><lf>" and/or prefix "<cr><lf>")
        // if result code equals to [CMD_RC_CR].
        if (pCmdResult->uResultCode != CMD_RC_CR)
        {
            AT_GetError_DCE(uRCFormatMode, pCmdResult->uResultCode, msg);
            if (strlen(msg) > 0)
                AT_WriteUart(msg, strlen(msg));
        }
    }
    return 0;
}

static INT32 CmdResultFail(PAT_CMD_RESULT pCmdResult)
{
    UINT8 szCR[] = { 0, 0 };
    UINT8 szLF[] = { 0, 0 };
    UINT32 uErrRet      = ERR_AT_UTIL_FAILED;
    UINT8 szErrStr[128] = { 0 };

	UINT8 uCmeeSet = 1; //gATCurrentuCmee; // hameina replaced 080326

    szCR[0] = CHAR_CR;
    szLF[0] = CHAR_LF;
    AT_KillAsyncTimer();
    // ////////////////////////////////////////////////
    // Output detailed error information.
    if (pCmdResult->pData != NULL && pCmdResult->nDataSize > 0)
    {
        UINT8 s[4] = { 0 };
        AT_Sprintf(s, "%s%s", szCR, szLF);
        // //////////////////////////////////////////////
        // When ATV is set to 0, response information's format: <text><cr><lf>, set to 1, format: <cr><lf><text><cr><lf>.
        if (uRCFormatMode == 0)
        {
            AT_WriteUart(pCmdResult->pData, pCmdResult->nDataSize);
            AT_WriteUart(s, strlen(s));
        }
        else if (uRCFormatMode == 1)
        {
            AT_WriteUart(s, strlen(s));
            AT_WriteUart(pCmdResult->pData, pCmdResult->nDataSize);
            AT_WriteUart(s, strlen(s));
        }
    }
    // Exit when ATQ set to 1.
    if (uRCPresentMode == 1)
        goto lab_clean;


	if (pCmdResult->nErrorType == CMD_ERROR_CODE_TYPE_CME)
		uErrRet = AT_GetError_CME(pCmdResult->uErrorCode, szErrStr, uCmeeSet);
	else if (pCmdResult->nErrorType == CMD_ERROR_CODE_TYPE_CMS)
		uErrRet = AT_GetError_CMS(pCmdResult->uErrorCode, szErrStr, uCmeeSet);

    //uErrRet = AT_GetError_DCE(uRCFormatMode, pCmdResult->uResultCode, szErrStr);

    if (uErrRet == ERR_AT_UTIL_FAILED)
    {
        memset(szErrStr, 0, sizeof(szErrStr));
        AT_GetError_DCE(uRCFormatMode, pCmdResult->uResultCode, szErrStr);
    }
    if (strlen(szErrStr) > 0)
    {
        AT_WriteUart(szErrStr, strlen(szErrStr));
    }
lab_clean:
    // parpare to accept new command line
    uart_ClearBuffer();
    AT_ClearCommandList();
    AT_SetStatus(AT_STATUS_CMD_LINE);
    return 0;
}

// Create this sub routine just for decreasing the writing times to UART device.
static void write_succ_result2uart(PAT_CMD_RESULT p_result)
{
    UINT8* p        = NULL;
    UINT8 szCR[]    = { 0, 0 };
    UINT8 szLF[]    = { 0, 0 };
    UINT8 szCR_LF[] = { 0, 0, 0 };
    UINT8 uAtvSet   = 1;
    AT_ASSERT(p_result->pData != NULL && p_result->nDataSize != 0);
    szCR[0] = AT_CMD_CR;
    szLF[0] = AT_CMD_LF;
    szCR_LF[0] = szCR[0];
    szCR_LF[1] = szLF[0];
    /*
       @RUBY STYLE PSEUDOCODE@
       AT_WriteUart("\r\n" + p_result->pData + "\r\n");
       * */
    p = AT_MALLOC(p_result->nDataSize + 4 + 1); // Couple of CR+LF characters.
    AT_ASSERT(p != NULL);
    AT_MemSet(p, 0, p_result->nDataSize + 4 + 1);
    if (uAtvSet == 1)
        strcpy(p, szCR_LF);
    memcpy(p + strlen(p), p_result->pData, p_result->nDataSize);
    memcpy(p + strlen(p), szCR_LF, strlen(szCR_LF));
    AT_WriteUart(p, p_result->nDataSize + 4); // Not include the null-terminated character.
    AT_FREE(p);
}
static INT32 ProcessSuccessResultNoClearCmd(PAT_CMD_RESULT p_result)
{
    AT_CMD** ppCmdList;
    // if async timer exist, kill it.
    ppCmdList = AT_GetCommandList();
    if (AT_IsAsyncTimer())
        AT_KillAsyncTimer();
    AT_SetStatus(AT_STATUS_CMD_LINE);
    if (p_result->pData != NULL && p_result->nDataSize > 0)
        write_succ_result2uart(p_result);
    if (*ppCmdList == NULL)
    {
        UINT8 s[30] = { 0 };
        SA_SetSysStatus(SA_INNER_RUNCOMMAND_STATE_, 0);
        if (uRCPresentMode == 0)
        {
            AT_GetError_DCE(uRCFormatMode, p_result->uResultCode, s);
            if (AT_StrLen(s))
                AT_WriteUart(s, AT_StrLen(s));
        }
    }
    else
    {
        SA_SetSysStatus(SA_INNER_RUNCOMMAND_STATE_, 1);
    }
    return 0;
}

static INT32 ProcessSuccessResult(PAT_CMD_RESULT p_result)
{
    AT_CMD** ppCmdList = NULL;
    ppCmdList = AT_GetCommandList();
    if (AT_IsAsyncTimer())
        AT_KillAsyncTimer();
    // if async timer exist, kill it.
    // release the current command and flush the result
    if (*ppCmdList != NULL)
    {
        AT_FreeCurCmd(ppCmdList, *ppCmdList);
        ppCmdList = AT_GetCommandList();
    }
    AT_SetStatus(AT_STATUS_CMD_LINE);
    if (p_result->pData != NULL && p_result->nDataSize > 0)
        write_succ_result2uart(p_result);
    if (*ppCmdList == NULL)
    {
        UINT8 s[30] = { 0 };
        SA_SetSysStatus(SA_INNER_RUNCOMMAND_STATE_, 0);
        if (uRCPresentMode == 0)
        {
            AT_GetError_DCE(uRCFormatMode, p_result->uResultCode, s);
            if (AT_StrLen(s))
                AT_WriteUart(s, AT_StrLen(s));
        }
        // add by wulc for ATQ1 bug  begin
        else if (uRCPresentMode == 1)
        {
            AT_GetError_DCE(uRCFormatMode, p_result->uResultCode, s);
            if (AT_StrLen(s))
                AT_WriteUart(s, AT_StrLen(s));
        }
        // add by wulc for ATQ1 bug end
    }
    else
    {
        AT_TC(g_sw_ATE, "ppCmdList->name = %s", (*ppCmdList)->pName);
        SA_SetSysStatus(SA_INNER_RUNCOMMAND_STATE_, 1);
    }
    return 0;
}

static INT32 CmdResultAsynSuccess(PAT_CMD_RESULT pCmdResult)
{
    // Start asynchronous timer.
    if (pCmdResult->pData != NULL && pCmdResult->nDataSize > 0)
        AT_WriteUart(pCmdResult->pData, pCmdResult->nDataSize);
    AT_SetAsyncTimer(pCmdResult->nDelayTime == 0 ? AT_DEFAULT_ELAPSE : pCmdResult->nDelayTime);
    return 0;
}
static INT32 CmdResultWaitIpData(PAT_CMD_RESULT pCmdResult)
{
    AT_SetStatus(AT_STATUS_IP_DATA);
    if (pCmdResult->pData != NULL && pCmdResult->nDataSize > 0)
        AT_WriteUart(pCmdResult->pData, pCmdResult->nDataSize);
    if (pCmdResult->nDelayTime != 0)
    {
        AT_SetAsyncTimer(pCmdResult->nDelayTime);
    }
    /* else
       {
       AT_SetAsyncTimer(AT_DEFAULT_ELAPSE);
       } */
    return ERR_SUCCESS;
}

// Create this sub routine just for decreasing the writing times to UART device.
static void write_conti_result2uart(PAT_CMD_RESULT p_result)
{
    UINT8* p        = NULL;
    UINT8 szCR_LF[] = { 0, 0, 0 };
    AT_ASSERT(p_result->pData != NULL && p_result->nDataSize != 0);
    szCR_LF[0] = CHAR_CR;
    szCR_LF[1] = CHAR_LF;
    /*
       @RUBY STYLE PSEUDOCODE@
       if add_suffix == true then
       AT_WriteUart("\r\n" + p_result->pData + "\r\n")
       else
       AT_WriteUart("\r\n" + p_result->pData)
       end
       * */
    p = AT_MALLOC(p_result->nDataSize + 4 + 1); // Maybe, couple of CR+LF characters.
    AT_MemSet(p, 0, p_result->nDataSize + 4 + 1);
    if (g_AtModule.uSuccRcCounter == 0 && uRCFormatMode == 1)
        strcpy(p, szCR_LF);
    memcpy(p + strlen(p), p_result->pData, p_result->nDataSize);
    strcat(p, szCR_LF);
    AT_WriteUart(p, strlen(p));
    AT_FREE(p);
}

static INT32 CmdResultContinue(PAT_CMD_RESULT pCmdResult)
{
    if (pCmdResult->pData == NULL || pCmdResult->nDataSize == 0)
        return 0;
    write_conti_result2uart(pCmdResult);
    return 0;
}

static INT32 CmdResultNotification(PAT_CMD_RESULT pCmdResult)
{
    UINT8 uiAtMode;
    UINT8 uMode;
    AT_IND** ppIndList;
    uiAtMode  = AT_GetMode();
    ppIndList = AT_GetIndicationList();
    uMode     = AT_GetCmerMode();
    switch (uMode)
    {
        case AT_CMER_ONLY_BUF_MODE:
        {
            // temporary saving the CSW indication
            // and show them when the list is empty
            AT_IND* p = AT_MALLOC(sizeof(AT_IND));
            AT_MemZero(p, sizeof(AT_IND));
            AT_MemCpy(&p->result, pCmdResult, sizeof(AT_CMD_RESULT));
            p->pNext = NULL;
            AT_AddIndToList(ppIndList, p);
        }
        break;
        case AT_CMER_DATA_DIS_MODE:
            if (uiAtMode == 0)
                OutputUrcNode(pCmdResult);
            else; // AT_CMER_DATA_DIS_MODE discards the URC according to 27007
            break;
        case AT_CMER_DATA_BUF_MODE:
            if (uiAtMode == 0)
                OutputUrcNode(pCmdResult);
            else
            {
                // temporary saving the CSW indication
                // and show them when the list is empty
                AT_IND* p = AT_MALLOC(sizeof(AT_IND));
                AT_MemZero(p, sizeof(AT_IND));
                AT_MemCpy(&(p->result), pCmdResult, sizeof(AT_CMD_RESULT));
                p->pNext = NULL;
                AT_AddIndToList(ppIndList, p);
            }
            break;
        case AT_CMER_DMA_MODE:
        default:
            OutputUrcNode(pCmdResult);
            break;
    }
    return 0;
}

// <uErrCode> must be a DCE error code.
VOID ATE_HandleException(UINT32 uErrCode, UINT8 nDLCI)
{
    UINT8 v_set          = 1;
    UINT8 rc_str[128]    = { 0 };
    // Output a debug trace when an exception raised.
    AT_TC(g_sw_ATE, "An exception raised by ATE, error code is: %u", uErrCode);
    // Kill timer first.
    if (AT_IsAsyncTimer())
        AT_KillAsyncTimer();
    // Output RC to TE.
    AT_GetError_DCE(v_set, uErrCode, rc_str);
    if (uRCPresentMode == 0)
    {
        AT_WriteUart(rc_str, strlen(rc_str));
        // Empty command list.
        AT_ClearCommandList();
        // Need more work to verify IND List
        uart_ClearBuffer();
        // Conver to command line state.
        AT_SetStatus(AT_STATUS_CMD_LINE);
    }
}

#define ERR_STR "COMMAND NO RESPONSE!"
VOID ATE_HandleTimeout(VOID* noPar)
{
    
    char s3                            = CHAR_CR, s4 = CHAR_LF;
    char buff[sizeof(ERR_STR) + 1 + 4] = { 0 };

	AT_TC(g_sw_UTIL, "ATE_HandleTimeout");
    if (AT_IsAsyncTimer())
        AT_KillAsyncTimer();
    // It's a ATE bug, delete this global flag after fixing bug by ATE.
    sprintf(buff, "%c%c COMMAND NO RESPONSE!%c%c", s3, s4, s3, s4);
    AT_WriteUart((UINT8*)buff, strlen(buff));
    // Need more work to Verify Ind List
    AT_ClearCommandList();
    AT_SetStatus(AT_STATUS_CMD_LINE);
}

BOOL AT_IsAsynCmdAvailable(UINT8* pName, UINT32 uCmdStamp, UINT8 nDLCI)
{
    AT_CMD** ppCmdList = AT_GetCommandList();
    if (AT_IsAsyncTimer()
        && (*ppCmdList != NULL)
        && (AT_StrCmp(pName, (*ppCmdList)->pName) == 0) && ((*ppCmdList)->uCmdStamp == uCmdStamp))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

INT32 AT_NotifyResult2ATM(PAT_CMD_RESULT pCmdResult, UINT8 nDLCI)
{
    AT_ASSERT(pCmdResult != NULL);
    // why to output URC here?
    CmdModeOutputUrcLink(nDLCI);
    {
        switch (pCmdResult->uReturnValue)
        {
            case CMD_FUNC_FAIL:
                g_AtModule.uSuccRcCounter = 0;
                return CmdResultFail(pCmdResult);
            case CMD_FUNC_SUCC:
                g_AtModule.uSuccRcCounter = 0;
                return ProcessSuccessResult(pCmdResult);
            case CMD_FUNC_SUCC_ASYN:
                g_AtModule.uSuccRcCounter = 0;
                return CmdResultAsynSuccess(pCmdResult);
            case CMD_FUNC_WAIT_IP_DATA:
                g_AtModule.uSuccRcCounter = 0;
                return CmdResultWaitIpData(pCmdResult);
            case CMD_FUNC_CONTINUE:
            {
                INT32 r = 0;
                r = CmdResultContinue(pCmdResult);
                if (g_AtModule.uSuccRcCounter >= MAX_UINT8)
                    g_AtModule.uSuccRcCounter = 1;
                else if (pCmdResult->nDataSize > 0) // Just increase the counter when has RC text.
                    g_AtModule.uSuccRcCounter++;
                return r;
            }
            case CSW_IND_NOTIFICATION:
                return CmdResultNotification(pCmdResult);
            case CMD_FUNC_SUCC_ASYN_CLEAR_CMD:
            {
                AT_CMD** ppCmdList;
                ppCmdList = AT_GetCommandList();
                // release the current command and flush the result
                if (*ppCmdList != NULL)
                {
                    AT_FreeCurCmd(ppCmdList, *ppCmdList);
                    ppCmdList = AT_GetCommandList();
                }
                g_AtModule.uSuccRcCounter = 0;
                return CmdResultAsynSuccess(pCmdResult);
            }
            case CMD_FUNC_SUCC_NO_CLEAR_CMD:
                g_AtModule.uSuccRcCounter = 0;
                return ProcessSuccessResultNoClearCmd(pCmdResult);
            default:
                AT_ASSERT(0);
                return 0;
        }
    }
}

PAT_CMD_RESULT AT_CreateRC(UINT32 uReturnValue,
                           UINT32 uResultCode,
                           UINT32 uErrorCode,
                           UINT8 nErrorType, UINT8 nDelayTime, UINT8* pBuffer, UINT16 nDataSize, UINT8 nDLCI)
{
    PAT_CMD_RESULT pResult = (PAT_CMD_RESULT)AT_MALLOC(sizeof(AT_CMD_RESULT) + nDataSize);
    if (pResult == NULL)
    {
        return NULL;
    }
    AT_MemZero(pResult, sizeof(AT_CMD_RESULT) + nDataSize);
    pResult->uReturnValue = uReturnValue;
    pResult->uResultCode = uResultCode;
    pResult->uErrorCode  = uErrorCode;
    pResult->nErrorType  = nErrorType;
    pResult->nDataSize   = nDataSize;
    pResult->nDelayTime  = nDelayTime;
    pResult->nDLCI       = nDLCI;
    if (pBuffer != NULL)
    {
        AT_MemCpy(pResult->pData, pBuffer, nDataSize);
        // Make a protection on RSP format.
        AT_Util_TrimRspStringSuffixCrLf(pResult->pData, &pResult->nDataSize);
        pResult->pData[pResult->nDataSize] = '\0';
    }
    return pResult;
}
#ifdef GSM_SUPPORT

/*

// AT_20071123_CAOW_B
UINT32 AT_GetPhoneActiveStatus(UINT8 nSim)
{

	UINT32 ret = AT_ACTIVE_STATUS_UNKNOWN;

#if 0 // ZXB

	if (AT_GetCommandTable() != NULL)
	{
		ret = AT_ACTIVE_STATUS_READY;
	}
	else
	{
		ret = AT_ACTIVE_STATUS_UNAVAILABLE;
	}

#else
	ret = AT_ACTIVE_STATUS_READY;

#endif

	switch (CFW_CcGetCallStatus(nSim))
	{

	case 0x01: // case CC_STATE_ACTIVE:

	case 0x02: // case CC_STATE_HOLD:

	case 0x40: // case CC_STATE_RELEASE:
	{
		ret = AT_ACTIVE_STATUS_CALL_IN_PROGRESS;
		break;
	}

	case 0x04: // case CC_STATE_WAITING:

	case 0x08: // case CC_STATE_INCOMING:

	case 0x10: // case CC_STATE_DIALING:

	case 0x20: // case CC_STATE_ALERTLING:
	{
		ret = AT_ACTIVE_STATUS_RINGING;
		break;
	}

	default:
		break;
	}

	return ret;
}
*/
/******************************************************************************************
Function			:	AT_SetCmeErrorCode
Description		:  	set cme error code
Called By			:	ATS moudle
Data Accessed  	:
Data Updated  	:
Input			:      UINT32 nCfwErrorCode, BOOL bSetParamValid
Output			:
Return			:	UINT32
Others			:	build by author unknown
					modify and add comment by wangqunyang 2008.04.05
*******************************************************************************************/
UINT32 AT_SetCmeErrorCode(UINT32 nCfwErrorCode, BOOL bSetParamValid)
{

	switch (nCfwErrorCode)
	{

	case ERR_CFW_INVALID_PARAMETER:

		if (bSetParamValid)
		{
			return ERR_AT_CME_PARAM_INVALID;
		}
		else
		{
			return ERR_AT_CME_EXE_FAIL;
		}

		break;

	case ERR_CME_SIM_NOT_INSERTED:
		return ERR_AT_CME_SIM_NOT_INSERTED;
		break;

	case ERR_CME_SIM_PIN_REQUIRED:
		return ERR_AT_CME_SIM_PIN_REQUIRED;
		break;

	case ERR_CME_SIM_PUK_REQUIRED:
		return ERR_AT_CME_SIM_PUK_REQUIRED;
		break;

	case ERR_CME_SIM_PIN2_REQUIRED:
		return ERR_AT_CME_SIM_PIN2_REQUIRED;
		break;

	case ERR_CME_SIM_PUK2_REQUIRED:
	case ERR_CME_SIM_CHV_NEED_UNBLOCK:
		return ERR_AT_CME_SIM_PUK2_REQUIRED;
		break;

	case ERR_NO_MORE_MEMORY:
		return ERR_AT_CME_MEMORY_FULL;
		break;

	case ERR_CME_MEMORY_FULL:
		return ERR_AT_CME_MEMORY_FULL;
		break;

	case ERR_CME_INCORRECT_PASSWORD:
	case ERR_CME_SIM_COMMAND_SUCCESS_VERIFICATION_FAIL:
		return ERR_AT_CME_INCORRECT_PASSWORD;
		break;

	case ERR_CFW_NOT_SUPPORT:
		return ERR_AT_CME_OPERATION_NOT_SUPPORTED;
		break;

	case ERR_CME_INVALID_INDEX:
		return ERR_AT_CME_INVALID_INDEX;
		break;

	case ERR_CME_MEMORY_FAILURE:
		return ERR_AT_CME_MEMORY_FAILURE;
		break;

	case ERR_CME_SIM_CHV_UNINIT:

	case ERR_CME_SIM_UNBLOCK_FAIL:

	case ERR_CME_SIM_UNBLOCK_FAIL_NO_LEFT:

	case ERR_CME_SIM_VERIFY_FAIL_NO_LEFT:
		return ERR_AT_CME_SIM_FAILURE;
		break;
	case ERR_CME_SIM_VERIFY_FAIL:
		return ERR_AT_CME_INCORRECT_PASSWORD;
		break;

	case ERR_CME_SIM_BLOCKED:

	case ERR_CME_SIM_PROFILE_ERROR:

	case ERR_CME_SIM_FILEID_NOT_FOUND:

	case ERR_CME_SIM_PATTERN_NOT_FOUND:

	case ERR_CME_SIM_MAXVALUE_REACHED:

	case ERR_CME_SIM_CONTRADICTION_INVALIDATION:

	case ERR_CME_SIM_CONTRADICTION_CHV:

	case ERR_CME_SIM_FILE_UNMATCH_COMMAND:

	case ERR_CME_SIM_NOEF_SELECTED:

	case ERR_CME_SIM_TECHNICAL_PROBLEM:

	case ERR_CME_SIM_WRONG_CLASS:

	case ERR_CME_SIM_UNKNOW_COMMAND:

	case ERR_CME_SIM_CONDITION_NO_FULLFILLED:
		return ERR_AT_CME_SIM_WRONG;
		break;
	case ERR_CME_SIM_UNKNOW:
		return ERR_AT_CME_SIM_NOT_INSERTED;
		break;
	case ERR_CME_SIM_STK_BUSY:
		return ERR_AT_CME_SIM_BUSY;
		break;

	case ERR_CME_PHONE_FAILURE:
		return ERR_AT_CME_PHONE_FAILURE;
		break;

	case ERR_CME_NO_CONNECTION_TO_PHONE:
		return ERR_AT_CME_NO_CONNECT_PHONE;
		break;

	case ERR_CME_PHONE_ADAPTER_LINK_RESERVED:
		return ERR_AT_CME_PHONE_ADAPTER_LINK_RESERVED;
		break;

	case ERR_CME_OPERATION_NOT_ALLOWED:
		return ERR_AT_CME_OPERATION_NOT_ALLOWED;
		break;

	case ERR_CME_OPERATION_NOT_SUPPORTED:
		return ERR_AT_CME_OPERATION_NOT_SUPPORTED;
		break;

	case ERR_CME_PH_SIM_PIN_REQUIRED:
		return ERR_AT_CME_PHSIM_PIN_REQUIRED;
		break;

	case ERR_CME_PH_FSIM_PIN_REQUIRED:
		return ERR_AT_CME_PHFSIM_PIN_REQUIRED;
		break;

	case ERR_CME_PH_FSIM_PUK_REQUIRED:
		return ERR_AT_CME_PHFSIM_PUK_REQUIRED;
		break;

	case ERR_CME_SIM_FAILURE:
		return ERR_AT_CME_SIM_FAILURE;
		break;

	case ERR_CME_SIM_BUSY:
		return ERR_AT_CME_SIM_BUSY;
		break;

	case ERR_CME_NOT_FOUND:
		return ERR_AT_CME_NOT_FOUND;
		break;

	case ERR_CME_TEXT_STRING_TOO_LONG:
		return ERR_AT_CME_TEXT_LONG;
		break;

	case ERR_CME_INVALID_CHARACTERS_IN_TEXT_STRING:
		return ERR_AT_CME_INVALID_CHAR_INTEXT;
		break;

	case ERR_CME_DIAL_STRING_TOO_LONG:
		return ERR_AT_CME_DAIL_STR_LONG;
		break;

	case ERR_CME_INVALID_CHARACTERS_IN_DIAL_STRING:
		return ERR_AT_CME_INVALID_CHAR_INDIAL;
		break;

	case ERR_CME_NO_NETWORK_SERVICE:
		return ERR_AT_CME_NO_NET_SERVICE;
		break;

	case ERR_CME_NETWORK_TIMEOUT:
		return ERR_AT_CME_NETWORK_TIMOUT;
		break;

	case ERR_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY:
		return ERR_AT_CME_NOT_ALLOW_EMERGENCY;
		break;

	case ERR_CME_NETWORK_PERSONALIZATION_PIN_REQUIRED:
		return ERR_AT_CME_NET_PER_PIN_REQUIRED;
		break;

	case ERR_CME_NETWORK_PERSONALIZATION_PUK_REQUIRED:
		return ERR_AT_CME_NET_PER_PUK_REQUIRED;
		break;

	case ERR_CME_NETWORK_SUBSET_PS_PIN_REQUIRED:
		return ERR_AT_CME_NET_SUB_PER_PIN_REQ;
		break;

	case ERR_CME_NETWORK_SUBSET_PS_PUK_REQUIRED:
		return ERR_AT_CME_NET_SUB_PER_PUK_REQ;
		break;

	case ERR_CME_SERVICE_PROVIDER_PS_PIN_REQUIRED:
		return ERR_AT_CME_SERVICE_PROV_PER_PIN_REQ;
		break;

	case ERR_CME_SERVICE_PROVIDER_PS_PUK_REQUIRED:
		return ERR_AT_CME_SERVICE_PROV_PER_PUK_REQ;
		break;

	case ERR_CME_CORPORATE_PERSONALIZATION_PIN_REQUIRED:
		return ERR_AT_CME_CORPORATE_PER_PIN_REQ;
		break;

	case ERR_CME_CORPORATE_PERSONALIZATION_PUK_REQUIRED:
		return ERR_AT_CME_CORPORATE_PER_PUK_REQ;
		break;

	case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
		return ERR_AT_CME_OPERATION_NOT_ALLOWED;
		break;

	default:
		return ERR_AT_CME_EXE_FAIL;
		break;

	}
}
#endif
/******************************************************************************************
Function			:	AT_Get_RetureValue_Name
Description		:      transfer reture value to name string
Called By			:	 AT_SMS_RESULT_OK
Data Accessed   	:
Data Updated    	:
Input			:      uReturnValue, int type
Output			:
Return			:	char *
Others			:	build by wangqunyang 08/04/2008
Mark                     :
*******************************************************************************************/
INT8* AT_Get_RetureValue_Name(UINT32 uReturnValue)
{
    switch (uReturnValue)
    {
        case CMD_FUNC_SUCC:
            return "CMD_FUNC_SUCC";
        case CMD_FUNC_SUCC_ASYN:
            return "CMD_FUNC_SUCC_ASYN";
        case CMD_FUNC_FAIL:
            return "CMD_FUNC_FAIL";
        case CMD_FUNC_CONTINUE:
            return "CMD_FUNC_CONTINUE";
        case CMD_FUNC_WAIT_SMS:
            return "CMD_FUNC_WAIT_SMS";
        case CMD_FUNC_WAIT_IP_DATA:
            return "CMD_FUNC_WAIT_IP_DATA";
        case CSW_IND_NOTIFICATION:
            return "CSW_IND_NOTIFICATION";
        default:
            return "Error uReturnValue";
    }
}

/******************************************************************************************
Function            :   AT_Get_ResultCode_Name
Description     :      transfer result code to name string
Called By           :    AT_SMS_RESULT_OK
Data Accessed       :
Data Updated    :
Input           :      uResultCode, int type
Output          :
Return          :   char *
Others          :   build by wangqunyang 08/04/2008
Mark                     :
*******************************************************************************************/
INT8* AT_Get_ResultCode_Name(UINT32 uResultCode)
{
    switch (uResultCode)
    {
        case CMD_RC_OK:
            return "CMD_RC_OK";
        case CMD_RC_CONNECT:
            return "CMD_RC_CONNECT";
        case CMD_RC_RING:
            return "CMD_RC_RING";
        case CMD_RC_NOCARRIER:
            return "CMD_RC_NOCARRIER";
        case CMD_RC_ERROR:
            return "CMD_RC_ERROR";
        case CMD_RC_NODIALTONE:
            return "CMD_RC_NODIALTONE";
        case CMD_RC_BUSY:
            return "CMD_RC_BUSY";
        case CMD_RC_NOANSWER:
            return "CMD_RC_NOANSWER";
        case CMD_RC_NOTSUPPORT:
            return "CMD_RC_NOTSUPPORT";
        case CMD_RC_INVCMDLINE:
            return "CMD_RC_INVCMDLINE";
        case CMD_RC_CR:
            return "CMD_RC_CR";
        case CMD_RC_SIMDROP:
            return "CMD_RC_SIM_DROP";
        default:
            return "Error uResultCode";
    }
}

/******************************************************************************************
Function            :   SMS_UnicodeBigend2Unicode
Description     :       unicode BigEnd to unicode normal(LittlteEnd)
Called By           :
Data Accessed       :
Data Updated        :
Input           :       UINT8 * pUniBigData, UINT8 *pUniData, UINT16 nDataLen
Output          :
Return          :   BOOL
Others          :       build by wangqunyang 30/04/2008
*******************************************************************************************/
BOOL AT_UnicodeBigEnd2Unicode(UINT8* pUniBigData, UINT8* pUniData, UINT16 nDataLen)
{
    /* check the pointer is NULL or not */
    if ((NULL == pUniData) || (NULL == pUniBigData))
    {
        return FALSE;
    }
    /* there is odd chars and we make
       ** it even, discard the last char */
    if (nDataLen % 2)
    {
        nDataLen = nDataLen - 1;
    }
    while (nDataLen > 0)
    {
        pUniData[nDataLen - 2] = pUniBigData[nDataLen - 1];
        pUniData[nDataLen - 1] = pUniBigData[nDataLen - 2];
        nDataLen -= 2;
    }
    return TRUE;
}

// caoxh [+]2008-05-9
char* strupr(char* pStr)
{
    char* p = pStr;
    if (p == NULL)
        return NULL;
    while (*p != '\0')
    {
        if (*p >= 'a' && *p <= 'z')
            *p = *p - 32;
        p++;
    }
    return pStr;
}

// caoxh [+]2008-05-9

/******************************************************************************************
Function            :   AT_Bytes2String
Description     :       transfer bytes to ascii string
Called By           :   ATS moudle
Data Accessed       :
Data Updated    :
Input           :   UINT8 * pSource, UINT8 * pDest, UINT8 nSourceLen
Output          :
Return          :   INT8
Others          :   build by wangqunyang 2008.05.22
*******************************************************************************************/
BOOL AT_Bytes2String(UINT8* pDest, UINT8 * pSource, UINT8 * nSourceLen)
{
    UINT8 nTemp    = 0;
    UINT8 nDestLen = 0;
    if ((NULL == pSource) || (NULL == pDest))
    {
        AT_TC(g_sw_UTIL, "AT_Bytes2String: pointer is NULL");
        return FALSE;
    }
    AT_TC(g_sw_UTIL, "AT_Bytes2String: nSourceLen = %u",* nSourceLen);
    while (nTemp < *nSourceLen)
    {
        /* get high byte */
        pDest[nDestLen] = (pSource[nTemp] >> 4) & 0x0f;
        if (pDest[nDestLen] < 10)
        {
            pDest[nDestLen] |= '0';
        }
        else
        {
            pDest[nDestLen] += 'A' - 10;
        }
        nDestLen++;
        /* get low byte */
        pDest[nDestLen] = pSource[nTemp] & 0x0f;
        if (pDest[nDestLen] < 10)
        {
            pDest[nDestLen] |= '0';
        }
        else
        {
            pDest[nDestLen] += 'A' - 10;
        }
        nDestLen++;
        nTemp++;
    }
    pDest[nDestLen] = '\0';
    *nSourceLen = nDestLen;
    /* string char counter must be even */
    if (*nSourceLen % 2)
    {
        AT_TC(g_sw_UTIL, "AT_Bytes2String: source len is not even, nSourceLen = %u", nSourceLen);
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************************
Function            :   AT_String2Bytes
Description     :       This functions can transfer ascii string to bytes
Called By           :   ATS moudle
Data Accessed       :
Data Updated        :
Input           :       UINT8 * pDest, UINT8 * pSource, UINT8* pLen
Output          :
Return          :   INT8
Others          :   build by wangqunyang 2008.05.22
*******************************************************************************************/
BOOL AT_String2Bytes(UINT8* pDest, UINT8 * pSource, UINT8 * pLen)
{
    UINT8 nSourceLen  = *pLen;
    UINT8 nTemp       = 0;
    UINT8 nByteNumber = 0;
    if ((NULL == pSource) || (NULL == pDest))
    {
        AT_TC(g_sw_UTIL, "AT_String2Bytes: pointer is NULL");
        return FALSE;
    }
    AT_TC(g_sw_UTIL, "AT_String2Bytes: nSourceLen = %u", nSourceLen);
    /* string char counter must be even */
    if (nSourceLen % 2)
    {
        AT_TC(g_sw_UTIL, "AT_String2Bytes: source len is not even");
        return FALSE;
    }
    while (nTemp < nSourceLen)
    {
        /* get high half byte */
        if ((pSource[nTemp] > 0x2f) && (pSource[nTemp] < 0x3a))
        {
            pDest[nByteNumber] = (pSource[nTemp] - '0') << 4;
        }
        else if ((pSource[nTemp] > 0x40) && (pSource[nTemp] < 0x47))
        {
            pDest[nByteNumber] = (pSource[nTemp] - 'A' + 10) << 4;
        }
        else if ((pSource[nTemp] > 0x60) && (pSource[nTemp] < 0x67))
        {
            pDest[nByteNumber] = (pSource[nTemp] - 'a' + 10) << 4;
        }
        else
        {
            AT_TC(g_sw_UTIL, "high half byte more than 'F' of HEX: pSource[nTemp]: = %u ", pSource[nTemp]);
            return FALSE;
        }
        /* get low half byte */
        nTemp++;
        if ((pSource[nTemp] > 0x2f) && (pSource[nTemp] < 0x3a))
        {
            pDest[nByteNumber] += (pSource[nTemp] - '0');
        }
        else if ((pSource[nTemp] > 0x40) && (pSource[nTemp] < 0x47))
        {
            pDest[nByteNumber] += (pSource[nTemp] - 'A' + 10);
        }
        else if ((pSource[nTemp] > 0x60) && (pSource[nTemp] < 0x67))
        {
            pDest[nByteNumber] += (pSource[nTemp] - 'a' + 10);
        }
        else
        {
            AT_TC(g_sw_UTIL, "low half byte more than 'F' of HEX: pSource[nTemp]: = %u ", pSource[nTemp]);
            return FALSE;
        }
        nTemp++;
        nByteNumber++;
    }
    pDest[nByteNumber] = '\0';
    *pLen = nByteNumber;
    return TRUE;
}

#ifdef GSM_SUPPORT
#define  AT_USER_TRANS_ID_SIZE  199 // 0-199 for GSM; 200-255 is for GPRS
typedef struct _AT_UTI_TABLE
{
	UINT8 nUTI;
	INT32 nNextUTISpareTable;
} AT_UTI_TABLE;

AT_UTI_TABLE ATUserTransIdTable[AT_USER_TRANS_ID_SIZE];

UINT32 AT_GetFreeUTI(UINT32 nServiceId, UINT8 *pUTI)
{
    return CFW_GetFreeUTI(nServiceId, pUTI);
}

/******************************************************************************************
Function			:	AT_InitUtiTable
Description		:   	Initial the UTI table when AT Module beiginning
Called By			:	AT moudle
Data Accessed   	:
Data Updated    	:
Input			:   	VOID
Output			:
Return			:	VOID
Others			:	build by wangqunyang 2008.06.02
*******************************************************************************************/
VOID AT_InitUtiTable()
{
	UINT16 nIndex = 0;

	AT_MemZero(&ATUserTransIdTable[0], AT_USER_TRANS_ID_SIZE * sizeof(AT_UTI_TABLE));

	nATUserTransIdSpareHeader = 1;
	nATUserTransIdSpareTail   = 0;

	for (nIndex = 1; nIndex < AT_USER_TRANS_ID_SIZE - 1; nIndex++)
	{
		ATUserTransIdTable[nIndex].nNextUTISpareTable = nIndex + 1;
	}

	nATUserTransIdSpareTail                                        = nIndex;
	ATUserTransIdTable[nATUserTransIdSpareTail].nNextUTISpareTable = 0;

}

/******************************************************************************************
Function			:	AT_AllocUserTransID
Description		:   	malloc one user transaction id
Called By			:	AT moudle
Data Accessed   	:
Data Updated    	:
Input			:   	VOID
Output			:
Return			:	UINT8
Others			:	build by wangqunyang 2008.06.02
*******************************************************************************************/
UINT16 AT_AllocUserTransID()
{
	UINT16 TempIndex = 0;

	// verify the parameters
	if ((nATUserTransIdSpareHeader <= 0) || (nATUserTransIdSpareTail > AT_USER_TRANS_ID_SIZE - 1) || (nATUserTransIdSpareHeader == nATUserTransIdSpareTail))  // leave at least one node
	{
		AT_TC(g_sw_UTIL, "AT alloc UTI exception");
		return TempIndex;

	}

	// alloc the array element
	TempIndex                 = nATUserTransIdSpareHeader;
	nATUserTransIdSpareHeader = ATUserTransIdTable[nATUserTransIdSpareHeader].nNextUTISpareTable;
	AT_MemZero(&ATUserTransIdTable[TempIndex], sizeof(AT_UTI_TABLE));

	return TempIndex;

}

/******************************************************************************************
Function			:	AT_FreeUserTransID
Description		:   	free one user transaction id
Called By			:	AT moudle
Data Accessed   	:
Data Updated    	:
Input			:   	UINT8 nUTI
Output			:
Return			:	VOID
Others			:	build by wangqunyang 2008.06.02
*******************************************************************************************/
VOID AT_FreeUserTransID(UINT16 nUTI)
{

	// verify the parameters
	if ((nUTI >= AT_USER_TRANS_ID_SIZE) || (ATUserTransIdTable[nUTI].nNextUTISpareTable != 0))
	{
		AT_TC(g_sw_UTIL, "AT free UTI exception");
		return;
	}

	// Free the array element
	ATUserTransIdTable[nATUserTransIdSpareTail].nNextUTISpareTable = nUTI;
	nATUserTransIdSpareTail                                        = nUTI;
	AT_MemZero(&(ATUserTransIdTable[nUTI]), sizeof(AT_UTI_TABLE));
}

VOID AT_SendToSIMID(UINT8 nSim)
{
	UINT8 arrResTest[40] = { 0, };
	UINT8 arrResOk[10]   = { AT_CMD_CR, AT_CMD_LF, 'O', 'K', AT_CMD_CR, AT_CMD_LF, 0, };  // {0,};
	UINT8 arrCRLF[4]     = { AT_CMD_CR, AT_CMD_LF, 0, };

	AT_StrCpy(arrResTest, arrCRLF);
	AT_Sprintf(&arrResTest[2], "SIM:%d", nSim);

	AT_TC(g_sw_UTIL, "To SIM:%d", nSim);

	AT_StrCat(arrResTest, arrCRLF);
	AT_StrCat(arrResTest, arrResOk);
	AT_WriteUart(arrResTest, AT_StrLen(arrResTest));

	return;
}


#define AT_WAITING_EVENT_MAX 20
struct ATWaitingEvent {
    UINT32 nEvent;
    UINT8 nSim;
    UINT8 nDLCI;
    UINT8 enable;
};


struct ATWaitingEvent gAT_WaitingEvent[AT_WAITING_EVENT_MAX] = {{0,0,0,0},};

VOID AT_AddWaitingEvent(UINT32 event, UINT8 sim, UINT8 dlci)
{
    int i;
    for (i = 0; i < AT_WAITING_EVENT_MAX; i++)
    {
        if (gAT_WaitingEvent[i].enable == 0)
        {
            gAT_WaitingEvent[i].nEvent = event;
            gAT_WaitingEvent[i].nSim = sim;
            gAT_WaitingEvent[i].nDLCI = dlci;
            gAT_WaitingEvent[i].enable = 1;
            AT_TC(g_sw_UTIL, "Add Waiting Event:%x", event);
            return;
        }
    }
    AT_TC(g_sw_UTIL, "-----Warning no enough gAT_WaitingEvent");
    return;
}

BOOL AT_GetWaitingEvent(UINT32 event, UINT8 sim, UINT8 dlci)
{
    int i;

    for (i = 0; i < AT_WAITING_EVENT_MAX; i++)
    {
       if ((gAT_WaitingEvent[i].enable)
               && (gAT_WaitingEvent[i].nEvent == event) 
               && (gAT_WaitingEvent[i].nSim == sim)) 
       {
            AT_TC(g_sw_UTIL, "Get Waiting Event:%x", event);
            return TRUE;
       }
    }

    AT_TC(g_sw_UTIL, "Not found Waiting Event:%x", event);
    return FALSE;
}

VOID AT_DelWaitingEvent(UINT32 event, UINT8 sim, UINT8 dlci)
{
    int i;

    for (i = 0; i < AT_WAITING_EVENT_MAX; i++)
    {
       if ((gAT_WaitingEvent[i].enable)
               && (gAT_WaitingEvent[i].nEvent == event) 
               && (gAT_WaitingEvent[i].nSim == sim))
       {
            gAT_WaitingEvent[i].nEvent = 0;
            gAT_WaitingEvent[i].nSim = 0;
            gAT_WaitingEvent[i].nDLCI = 0;
            gAT_WaitingEvent[i].enable = 0;
            AT_TC(g_sw_UTIL, "Delet Waiting Event:%x", event);
            return;
       }
    }

    AT_TC(g_sw_UTIL, "Delete Not found Waiting Event:%x", event);
    return;
}

UINT8 AT_GetWaitingEvent_DLCI(UINT32 event, UINT8 sim)
{
    int i;

    for (i = 0; i < AT_WAITING_EVENT_MAX; i++)
    {
        if ((gAT_WaitingEvent[i].enable)
                && (gAT_WaitingEvent[i].nEvent == event) 
                && (gAT_WaitingEvent[i].nSim == sim))
        {
            AT_TC(g_sw_UTIL, "get DLCI:%d in Waiting Event:%x", gAT_WaitingEvent[i].nDLCI, event);
            return gAT_WaitingEvent[i].nDLCI;
        }
    }
    AT_TC(g_sw_UTIL, "Not found DLCIin Waiting Event:%d", event);
    return 0;
}


void AT_CosEvent2CfwEvent(COS_EVENT *pCosEvent, CFW_EVENT *pCfwEvent)
{
	pCfwEvent->nEventId = pCosEvent->nEventId;
	pCfwEvent->nParam1  = pCosEvent->nParam1;
	pCfwEvent->nParam2  = pCosEvent->nParam2;
	pCfwEvent->nUTI     = HIUINT16(pCosEvent->nParam3);
	pCfwEvent->nType    = HIUINT8(pCosEvent->nParam3);
	pCfwEvent->nFlag    = LOUINT8(pCosEvent->nParam3);
}

#endif /*GSM_SUPPORT*/
