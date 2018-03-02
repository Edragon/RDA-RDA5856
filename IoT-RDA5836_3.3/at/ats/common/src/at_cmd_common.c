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
#include "at_cmd_common.h"
#include "ap_common.h"
#include "ap_message.h"
#include "at_module.h"

#include "hal_sys.h"

#include "string.h"


#if !defined(WIN32)
#include "hal_map.h"
#endif /* !WIN32 */

#if defined(__AT_MOD_COMMON__)
extern UINT8 g_cur_volume;

/******************************************************************************
 * AT_COMMON_Result_OK:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_Result_OK(UINT32 uReturnValue,
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

/******************************************************************************
 * AT_COMMON_Result_Err:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
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

/******************************************************************************
 * AT_COMMON_CmdFunc_VolAdd:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_VolAdd(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_VOLADD;
    ev.nParam1 = (UINT16)(AT_COMMON_VOLADD>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

/******************************************************************************
 * AT_COMMON_CmdFunc_VolSub:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_VolSub(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_VOLSUB;
    ev.nParam1 = (UINT16)(AT_COMMON_VOLSUB>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

/******************************************************************************
 * AT_COMMON_CmdFunc_Volume:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Volume(AT_CMD_PARA* pParam)
{

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
			    UINT16 value, uStrLen    = 1;
			    INT32 iRetValue  = 0;

                UINT8 uNumOfParam = 0;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (1!= uNumOfParam)
                {
                    goto iOpen_ERROR;
                }

                uStrLen = 2;
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT16, &value, &uStrLen);
		        AT_TC(g_sw_ATE, "AT Set Volume %d",value);
		        
                SendAppEvent(AT_COMMON_VOLUME, value);
                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }

            case AT_CMD_READ:
            {
            	AT_WriteUartString("VOLUME=%d\r\n",g_cur_volume);
                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }
            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:

	AT_COMMON_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}


/******************************************************************************
 * AT_COMMON_CmdFunc_PlayTone:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_PlayTone(AT_CMD_PARA* pParam)
{

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
			    UINT16 value, uStrLen    = 2;
			    INT32 iRetValue  = 0;
                UINT8 uNumOfParam = 0;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (1!= uNumOfParam)
                {
                    goto iOpen_ERROR;
                }
                
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT16, &value, &uStrLen);
		        AT_TC(g_sw_ATE, "AT Playtone %d",value);
		        
                SendAppEvent(AT_COMMON_PLAYTONE, value);
                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }

            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:

	AT_COMMON_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}


/******************************************************************************
 * AT_COMMON_CmdFunc_Next:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Next(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_NEXT;
    ev.nParam1 = (UINT16)(AT_COMMON_NEXT>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

/******************************************************************************
 * AT_COMMON_CmdFunc_Prev:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Prev(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_PREV;
    ev.nParam1 = (UINT16)(AT_COMMON_PREV>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}


/******************************************************************************
 * AT_COMMON_CmdFunc_Fore:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Fore(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_FORE;
    ev.nParam1 = (UINT16)(AT_COMMON_FORE>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
    ev.nEventId = (UINT16)AT_COMMON_FORE_PLAY;
    ev.nParam1 = (UINT16)(AT_COMMON_FORE_PLAY>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}

/******************************************************************************
 * AT_COMMON_CmdFunc_Back:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Back(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = (UINT16)AT_COMMON_BACK;
    ev.nParam1 = (UINT16)(AT_COMMON_BACK>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
    ev.nEventId = (UINT16)AT_COMMON_BACK_PLAY;
    ev.nParam1 = (UINT16)(AT_COMMON_BACK_PLAY>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;

}


/******************************************************************************
 * AT_COMMON_CmdFunc_ModeSwitch:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam)
{

    UINT16 uStrLen                   = 1;
    INT32 iRetValue                 = 0;
    UINT8 arrCharacterSet[20] = {0,};
    UINT8 arrStr[30];

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
                UINT8 uNumOfParam = 0;
                UINT32 result = 0;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (1!= uNumOfParam)
                {
                    goto iOpen_ERROR;
                }
                uStrLen  = SIZEOF(arrCharacterSet);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, &arrCharacterSet, &uStrLen);

                if (0==AT_StrCaselessCmp(arrCharacterSet,"MUSIC"))
                    result = RESULT_MUSIC;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"RECORD"))
                    result = RESULT_RECORD_START;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"FM"))
                    result = RESULT_RADIO;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"BT"))
                    result = RESULT_BT;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"LINEIN"))
                    result = RESULT_LINE_IN;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"MAIN"))
                    result = RESULT_MAIN;
				else if (0==AT_StrCaselessCmp(arrCharacterSet,"MUSIC_TF"))
                    result = RESULT_MUSIC_TF;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"MUSIC_USB"))
                    result = RESULT_MUSIC_USB;
#ifdef SPEAKER_SUPPORT
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"SPEAKER"))
                    result = RESULT_SPEAKER;
#endif
#if defined(APP_SUPPORT_UART_PLAY) && (APP_SUPPORT_UART_PLAY == 1)
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"UART_PLAY"))
                    result = RESULT_UART_PLAY;
#endif
#if defined(APP_SUPPORT_UART_REC) && (APP_SUPPORT_UART_REC == 1)
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"UART_REC"))
                    result = RESULT_UART_REC;
#endif
                if(result > 0)
                {
                    SendAppEvent(EV_UI_FW_SWITCH_MOD, result);
                }

                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }

            case AT_CMD_READ:
            {
                if(IS_MUSIC_MODE())
                {
                    memcpy(arrStr,"MUSIC",sizeof("MUSIC"));
                }
                else if(IS_RECORD_MODE())
                {
                    memcpy(arrStr,"RECORD",sizeof("RECORD"));
                }
                else if(IS_FM_MODE())
                {
                    memcpy(arrStr,"FM",sizeof("FM"));
                }
#ifdef SPEAKER_SUPPORT
                else if(IS_SPEAKER_MODE())
                {
                    memcpy(arrStr,"SPEAKER",sizeof("SPEAKER"));
                }
#endif
                else if(IS_BT_MODE())
                {
                    memcpy(arrStr,"BT",sizeof("BT"));
                }
                else if(IS_USB_MODE())
                {
                    memcpy(arrStr,"USB",sizeof("USB"));
                }
                else if(IS_LINEIN_MODE())
                {
                    memcpy(arrStr,"LINEIN",sizeof("LINEIN"));
                }

                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, arrStr, AT_StrLen(arrStr), pParam->nDLCI);
                return;
            }
            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:

	AT_COMMON_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}

/******************************************************************************
 * AT_COMMON_CmdFunc_Sleep: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Sleep(AT_CMD_PARA* pParam)
{
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
    APP_Sleep();
}

/******************************************************************************
 * AT_COMMON_CmdFunc_Reset: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Reset(AT_CMD_PARA* pParam)
{
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);   
#if !defined(WIN32)
    hal_SysRestart();
#endif /* !WIN32 */
}


/******************************************************************************
 * AT_COMMON_CmdFunc_Shutdown: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Shutdown(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;

    ev.nEventId = AP_MSG_FORCE_STANDBY;
    ev.nParam1 = 0;

    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
}


/******************************************************************************
 * AT_COMMON_CmdFunc_Version:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_COMMON_CmdFunc_Version(AT_CMD_PARA* pParam)
{
	UINT8 version_string[128];

	//100,170314,162201
	//AT_WriteUartString("%x,%x,%x",AP_GetVersion(),AP_GetBuildDate(),AP_GetBuildTime());

	memset(version_string, 0, sizeof(version_string));
#ifndef WIN32    
	sprintf(version_string,"%x,%x,%x%x", hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID),
		AP_GetVersion(), AP_GetBuildDate(), AP_GetBuildTime());
#endif

	AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, version_string, strlen(version_string), pParam->nDLCI);
}

#if !defined(WIN32)
/******************************************************************************
 * AT_COMMON_CmdFunc_PeripheralReg:
 * DESCRIPTION: -
 *  Write   AT+CPREG=<ABB/PMU/FMD/XCV>,<REGADDR in hex>,<REGVAL in hex>
 *  Read    AT+CPREG?<ABB/PMU/FMD/XCV>,<REGADDR in hex>
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
extern HAL_ISPI_DEBUG_T g_halIspiDebug;
VOID AT_COMMON_CmdFunc_ReadPeripheralReg(AT_CMD_PARA* pParam)
{

    UINT16 uStrLen                   = 1;
    INT32 iRetValue                 = 0;
    UINT8 arrCharacterSet[20] = {0};
    UINT8 arrStr[30];

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
                UINT8 uNumOfParam = 0;
                UINT32 data;
                UINT32 timeout = 1000;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (uNumOfParam < 2)
                {
                    goto iOpen_ERROR;
                }


                uStrLen  = SIZEOF(arrCharacterSet);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, &arrCharacterSet, &uStrLen);

                if (0==AT_StrCaselessCmp(arrCharacterSet,"ABB"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_ABB;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"PMU"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_PMD;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"FMD"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_FMD;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"XCV"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_XCV;

                
                memset(arrStr, 0, SIZEOF(arrStr));
                uStrLen  = SIZEOF(arrStr);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, &arrStr, &uStrLen);
                //sscanf(arrStr, "0x%x", &data);
                data = strtol(arrStr, NULL, 16);
                g_halIspiDebug.debug.address = (UINT16) data;

                g_halIspiDebug.debug.read = 1;
                AT_TC(g_sw_ATE, "AT ReadPReg %s[0x%x]", arrCharacterSet, data);

                
                do{
                    COS_Sleep(1);
                    if(0 == timeout--) break;
                }while(g_halIspiDebug.debug.read || g_halIspiDebug.debug.write);

                AT_TC(g_sw_ATE, "AT ReadPReg %s[0x%x] = 0x%x", arrCharacterSet, 
                                                         g_halIspiDebug.debug.address,
                                                        g_halIspiDebug.data);
                if(0 == timeout)
                {
                    AT_WriteUartString("ReadPReg Timeout\r\n");
                	AT_COMMON_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
                }
                else
                {
                    AT_WriteUartString("0x%x\r\n", g_halIspiDebug.data);
                    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
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

	AT_COMMON_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}


VOID AT_COMMON_CmdFunc_WritePeripheralReg(AT_CMD_PARA* pParam)
{

    UINT16 uStrLen                   = 1;
    INT32 iRetValue                 = 0;
    UINT8 arrCharacterSet[20] = {0};
    UINT8 arrStr[30];

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
                UINT8 uNumOfParam = 0;
                UINT32 data;
                UINT32 timeout = 1000;
                
                if (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uNumOfParam))
                {
                    goto iOpen_ERROR;
                }
                if (uNumOfParam < 3)
                {
                    goto iOpen_ERROR;
                }

                uStrLen  = SIZEOF(arrCharacterSet);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, &arrCharacterSet, &uStrLen);

                if (0==AT_StrCaselessCmp(arrCharacterSet,"ABB"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_ABB;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"PMU"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_PMD;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"FMD"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_FMD;
                else if (0==AT_StrCaselessCmp(arrCharacterSet,"XCV"))
                    g_halIspiDebug.debug.type = HAL_ISPI_REG_TYPE_XCV;

                
                memset(arrStr, 0, SIZEOF(arrStr));
                uStrLen  = SIZEOF(arrStr);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, &arrStr, &uStrLen);
                data = strtol(arrStr, NULL, 16);
                g_halIspiDebug.debug.address = (UINT16) data;

                memset(arrStr, 0, SIZEOF(arrStr));
                uStrLen  = SIZEOF(arrStr);
                iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 2, AT_UTIL_PARA_TYPE_STRING, &arrStr, &uStrLen);
                data = strtol(arrStr, NULL, 16);
                g_halIspiDebug.data = (UINT16) data;
                g_halIspiDebug.debug.write = 1;


                AT_TC(g_sw_ATE, "AT WritePReg %s[0x%x] = 0x%x", arrCharacterSet, 
                                                              g_halIspiDebug.debug.address,
                                                              g_halIspiDebug.data);

                
                do{
                    COS_Sleep(1);
                    if(0 == timeout--) break;
                }while(g_halIspiDebug.debug.read || g_halIspiDebug.debug.write);

                if(0 == timeout)
                {
                    AT_WriteUartString("WritePReg Timeout\r\n");
                	AT_COMMON_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
                    return;
                }

                AT_TC(g_sw_ATE, "AT WritePReg Done");

                AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                return;
            }

            
            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:

	AT_COMMON_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}

#else /* WIN32 */

VOID AT_COMMON_CmdFunc_ReadPeripheralReg(AT_CMD_PARA* pParam)
{
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
}

VOID AT_COMMON_CmdFunc_WritePeripheralReg(AT_CMD_PARA* pParam)
{
    AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
}

#endif /* !WIN32 */

#endif
