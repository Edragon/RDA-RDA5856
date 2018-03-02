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

#if defined(__AT_MOD_RECORD__)
#include "at_cmd_record.h"
#include "mci.h"
#include "tgt_app_cfg.h"

VOID AT_RECORD_Result_OK(UINT32 uReturnValue,
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

VOID AT_RECORD_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
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

/*****************************************************************************
* Name:        AT_RECORD_CmdFunc_Start
* Description: MUSIC module execute error process.
* Parameter:   UINT32 uErrorCode, UINT8 nErrorType
* Return:      VOID
* Remark:      n/a
* Author:      YangYang
* Data:        2008-5-15
******************************************************************************/
VOID AT_RECORD_CmdFunc_Start(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;
    ev.nEventId = (UINT16)AT_RECORD_START;
     ev.nParam1 = (UINT16)(AT_RECORD_START>>16);
     COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_RECORD_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

/*****************************************************************************
* Name:        AT_RECORD_CmdFunc_PAUSE
* Description: MUSIC module execute error process.
* Parameter:   UINT32 uErrorCode, UINT8 nErrorType
* Return:      VOID
* Remark:      n/a
* Author:      YangYang
* Data:        2008-5-15
******************************************************************************/
VOID AT_RECORD_CmdFunc_Pause(AT_CMD_PARA* pParam)
{
	COS_EVENT ev;
	ev.nEventId = (UINT16)AT_RECORD_PAUSE;
	ev.nParam1 = (UINT16)(AT_RECORD_PAUSE>>16);
      COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	AT_RECORD_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

#if (APP_SUPPORT_UART_REC == 1) || (defined WIN32)
VOID AT_RECORD_Uart_Record_Callback(unsigned char* buf_p,unsigned int len, unsigned int *record_len)
{
    AT_WriteUart(buf_p, len);

    *record_len = len;
}


VOID AT_RECORD_Uart_Record_Error_Callback(VOID)
{
    AT_TC(g_sw_SA, "AT_RECORD_Uart_Record_Error_Callback");
}

VOID AT_RECORD_CmdFunc_UartStart(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS;

    if(NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {

            case AT_CMD_SET:
            {
                UINT8 param_cnt ;
                UINT16 record_type;
                UINT32 buffer_size,samplerate = 8000;
                UINT16 Len;
                INT32 ret;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if((ERR_SUCCESS != iRet)||(param_cnt < 2))
                {
                    goto Param_ERROR;
                }


                Len = 2;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT16, &record_type, &Len);  //MCI_TYPE_WAV_DVI_ADPCM,

                Len = 4;
                AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT32, &buffer_size, &Len);

                if (param_cnt == 3) {

                    Len = 4;
                    AT_Util_GetParaWithRule(pParam->pPara, 2, AT_UTIL_PARA_TYPE_UINT32,&samplerate,&Len);
                }



                AT_TC(g_sw_SA, "AT_RECORD_CmdFunc_UartStart record_type=%d,buffer_size = %d,SampleRate = %d",
                              record_type, buffer_size,samplerate);

                ret = MCI_AudioRecordStart(buffer_size, record_type, 7, samplerate,AT_RECORD_Uart_Record_Error_Callback,
                                                                    AT_RECORD_Uart_Record_Callback);

				if(ret != MCI_ERR_NO)
				{
					AT_RECORD_Result_Err(ret, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
				}
				else
				{
					AT_RECORD_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
				}

                return;
            }
            default:
            {
                goto Param_ERROR;
            }

        }
    }

Param_ERROR:

    AT_RECORD_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;
}


VOID AT_RECORD_CmdFunc_UartStop(AT_CMD_PARA* pParam)
{
    MCI_AudioRecordStop();

    AT_RECORD_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}
#endif /* end #if (APP_SUPPORT_UART_REC == 1) */

#endif /* end __AT_MOD_RECORD__ */
