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

// //////////////////////////////////////////////////////////////////////////////
#include "at_common.h"
#include "MainTask.h"
#if defined(__AT_MOD_FM__)	
#include "at_cmd_fm.h"
#include "ap_fm.h"

extern FM_play_status_t *FMStatus;

VOID AT_FM_Result_OK(UINT32 uReturnValue,
                       UINT32 uResultCode, UINT8 nDelayTime, UINT8* pBuffer, UINT16 nDataSize, UINT8 nDLCI)

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

VOID AT_FM_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
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

VOID AT_FM_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam)
{
    return;
}


VOID AT_FM_CmdFunc_AutoSearch(AT_CMD_PARA* pParam)
{
	COS_EVENT ev;
	ev.nEventId = (UINT16)AT_FM_AUTO_SEARCH;
	ev.nParam1 = (UINT16)(AT_FM_AUTO_SEARCH>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}

VOID AT_FM_CmdFunc_HoldMode(AT_CMD_PARA* pParam)
{
	COS_EVENT ev;
	ev.nEventId = (UINT16)AT_FM_MODE;
	ev.nParam1 = (UINT16)(AT_FM_MODE>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}

VOID AT_FM_CmdFunc_StopSearch(AT_CMD_PARA* pParam)
{
    SendAppEvent(AT_FM_STOP_SEARCH, 0);    
	AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
}

VOID AT_FM_CmdFunc_Frequence(AT_CMD_PARA* pParam)
{

    PAT_CMD_RESULT pResult;

    if (NULL == pParam)
    {
        goto iOpen_ERROR;
    }
    else
    {
        if(NULL == pParam->pPara)
        {
            goto iOpen_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
			    UINT16 uStrLen    = 4;
			    INT32 iRetValue  = 0;
			    UINT32 value;

                UINT8 uNumOfParam = 0;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (1!= uNumOfParam)
                {
                    goto iOpen_ERROR;
                }
                
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT32, &value, &uStrLen);
		        AT_TC(g_sw_ATE, "AT FM Set Freq %d",value);
		        
                SendAppEvent(AT_FM_SETFREQ, value);
                AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }

            case AT_CMD_READ:
            {
            	if(FMStatus)
            	{
	            	AT_WriteUartString("FM FREQ=%d\r\n",FMStatus->freq);
            	}
                AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }
            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:

	AT_FM_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}


VOID AT_FM_CmdFunc_GetState(AT_CMD_PARA* pParam)
{
    UINT8 i;

    if(FMStatus)
    {
        for(i = 0; i < FMStatus->tab_num; i++)
        {
            AT_WriteUartString("%d. %d\r\n", i, FMStatus->tabs[i] + 0x10000);
        }
    }

	AT_FM_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
}

#endif
