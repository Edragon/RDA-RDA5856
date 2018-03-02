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
 *  At_sa.c
 * Author
 *  Felix
 * Date
 *  2007/12/25
 * Descriptions:
 *  ...
 *
 *********************************************************/
#include "at_common.h"
#include "at_sa.h"
#include "at_module.h"
#include "cos.h"
#include "at_map.h"

/***************************************************************************************************
                                Command Table
****************************************************************************************************/
#define FSM_STATUS_MASK_POWER_DOWN      0x00000001
#define FSM_STATUS_MASK_NORMAL          0x00000002
#define FSM_STATUS_MASK_CHGR_NORMAL     0x00000004
#define FSM_STATUS_MASK_CHGR_ONLY       0x00000008
#define FSM_STATUS_MASK_ALARM_NORMAL    0x00000010
#define FSM_STATUS_MASK_ALARM_ONLY      0x00000020
#define FSM_STATUS_MASK_SLEEP           0x00000040

#define PROC_MODE_MASK_COMMAND          0x00000001
#define PROC_MODE_MASK_ONLINE_CMD       0x00000002
// [[hameina [MOD] 2008-4-22:according to the command table, online data should be PS, an other state is CS
#define PROC_MODE_MASK_CS                   0x00000004
#define PROC_MODE_MASK_PS                   0x00000008
// ]]Hameina[mod]2008-04-22 :add CS mode

// //////////////////////////////////////////////////
// System States
// //////////////////////////////////////////////////

typedef enum _sa_fsm_state_t
{
    SA_FSM_STATE_POWER_DOWN,
    SA_FSM_STATE_NORMAL,
    SA_FSM_STATE_CHGR_NORMAL,
    SA_FSM_STATE_CHGR_ONLY,
    SA_FSM_STATE_ALARM_NORMAL,
    SA_FSM_STATE_ALARM_ONLY,
    SA_FSM_STATE_SLEEP,
    SA_FSM_STATE_COUNT_,  // counter of this enum.
    SA_FSM_STATE_INVALID_,  // invalid value of this enum.
} sa_fsm_state_t;

// //////////////////////////////////////////////////
// GLOBAL VARIABLES DEFINITION
// //////////////////////////////////////////////////
extern AT_ModuleInfo g_AtModule;

// //////////////////////////////////////////////////
// PROTOTYPE DEFINITIONS OF INTERNAL FUNCTIONS
// //////////////////////////////////////////////////
static BOOL is_cmd_enabled(UINT32 index, UINT8 nDLCI)
{
    UINT32 sys_stat = 0;
    if (g_CommandTable[index].chief_enable_flag != 1)
        return FALSE;



    return  TRUE;

    AT_TC(g_sw_SA, "SA_INNER_FSM_STATUS_ = %d", SA_GetSysStatus(SA_INNER_FSM_STATUS_));
    sys_stat = SA_GetSysStatus(SA_INNER_FSM_STATUS_);
    switch (sys_stat)
    {
        case SA_FSM_STATE_POWER_DOWN:
            return FALSE;
        case SA_FSM_STATE_NORMAL:
            if ((g_CommandTable[index].enable_mask.fsm_status_mask & FSM_STATUS_MASK_NORMAL) != FSM_STATUS_MASK_NORMAL)
                return FALSE;
            break;
        case SA_FSM_STATE_CHGR_NORMAL:
            if ((g_CommandTable[index].enable_mask.fsm_status_mask & FSM_STATUS_MASK_CHGR_NORMAL) !=
                FSM_STATUS_MASK_CHGR_NORMAL)
                return FALSE;
            break;
        case SA_FSM_STATE_CHGR_ONLY:
            if ((g_CommandTable[index].enable_mask.fsm_status_mask & FSM_STATUS_MASK_CHGR_ONLY) != FSM_STATUS_MASK_CHGR_ONLY)
                return FALSE;
            break;
        case SA_FSM_STATE_ALARM_NORMAL:
            if ((g_CommandTable[index].enable_mask.fsm_status_mask & FSM_STATUS_MASK_ALARM_NORMAL) !=
                FSM_STATUS_MASK_ALARM_NORMAL)
                return FALSE;
            break;
        case SA_FSM_STATE_ALARM_ONLY:
            if ((g_CommandTable[index].enable_mask.fsm_status_mask & FSM_STATUS_MASK_ALARM_ONLY) != FSM_STATUS_MASK_ALARM_ONLY)
                return FALSE;
            break;
        case SA_FSM_STATE_SLEEP:
            return FALSE;
        default:
            return FALSE;
    }
    AT_TC(g_sw_SA, "SA_INNER_PROC_MODE_ = %d", SA_GetSysStatus(SA_INNER_PROC_MODE_));
    sys_stat = SA_GetSysStatus(SA_INNER_PROC_MODE_);
    switch (sys_stat)
    {
        case SA_PROC_MODE_COMMAND:
            if ((g_CommandTable[index].enable_mask.proc_mode_mask & PROC_MODE_MASK_COMMAND)!=PROC_MODE_MASK_COMMAND)
                return FALSE;
            break;
        case SA_PROC_MODE_ONLINE_CMD:
            if ((g_CommandTable[index].enable_mask.proc_mode_mask & PROC_MODE_MASK_ONLINE_CMD)!=PROC_MODE_MASK_ONLINE_CMD)
                return FALSE;
            break;
// [[hameina[mod] 2008-4-22 :Ps, cs are different modes, can't do the same check.
        case SA_PROC_MODE_PS:
            if ((g_CommandTable[index].enable_mask.proc_mode_mask & PROC_MODE_MASK_PS)!=PROC_MODE_MASK_PS)
                return FALSE;
            break;
        case SA_PROC_MODE_CS:
            if ((g_CommandTable[index].enable_mask.proc_mode_mask & PROC_MODE_MASK_CS)!=PROC_MODE_MASK_CS)
                return FALSE;
            break;
// ]]hameina[mod] 2008-4-22 :Ps, cs are different modes, can't do the same check.
        default:
            return FALSE;
    }
    return TRUE;
}

// TODO: need to check more status and module! now just check the initialization status for SMS and PBK.
static BOOL is_cmd_ready(UINT32 index, UINT8 nDLCI)
{
    SA_CMD_CLASS_T cmd_cls = g_CommandTable[index].cmd_class;
    UINT32 sys_stat        = 0;
    sys_stat = SA_GetSysStatus(SA_ATS_INIT_STATUS);

    switch (cmd_cls)
    {
        case SA_CMDCLS_SYS:
        case SA_CMDCLS_NW:
        case SA_CMDCLS_ID:
        case SA_CMDCLS_HW:
        case SA_CMDCLS_DS:
        case SA_CMDCLS_GC:
        case SA_CMDCLS_EX:
        default:
            break;
    }
    return TRUE;
}

pfn_AtCmdHandler SA_GetCmdHandler(UINT8* cmd_name, UINT8 nDLCI)
{
    int index = -1;
    pfn_AtCmdHandler pfn = SA_DefaultCmdHandler_NotSuppotted;
    while (g_CommandTable[++index].pName != NULL)
    {
        if (AT_StrCmp(cmd_name, g_CommandTable[index].pName) == 0)
        {
            if (is_cmd_enabled(index, nDLCI) == TRUE && is_cmd_ready(index, nDLCI) == TRUE)
                pfn = g_CommandTable[index].pFunc;
            else  // Command is diabled or not ready.
                pfn = SA_DefaultCmdHandler_Error;
        }
    }
    if (pfn == SA_DefaultCmdHandler_NotSuppotted)
        AT_TC(g_sw_ATE, "Command handler not found at command table.");
    return pfn;
}

/***************************************************************************************************
                                        SYSTEM AGENT
****************************************************************************************************/

//
// Global Vraiables
//

VOID SA_DefaultCmdHandler_NotSuppotted(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    AT_TC(g_sw_ATE, "Command Not Suppoted\n");
    pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_NOTSUPPORT, ERR_AT_CME_OPERATION_NOT_SUPPORTED,
                          CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
    AT_Notify2ATM(pResult, pParam->nDLCI);
    if (pResult != NULL)
    {
        AT_FREE(pResult);
        pResult = NULL;
    }
    return;
}

VOID SA_DefaultCmdHandler_CR(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    AT_TC(g_sw_ATE, "SA_DefaultCmdHandler_CR()\n");
    pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_CR, ERR_AT_OK, (UINT8)CMD_ERROR_CODE_OK, 0, NULL, 0, pParam->nDLCI);
    AT_Notify2ATM(pResult, pParam->nDLCI);
    if (pResult != NULL)
    {
        AT_FREE(pResult);
        pResult = NULL;
    }
    return;
}

VOID SA_DefaultCmdHandler_Error(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    AT_TC(g_sw_ATE, "Error Command\n");
    pResult =
        AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_ERROR, ERR_AT_UNKNOWN, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
    AT_Notify2ATM(pResult, pParam->nDLCI);
    if (pResult != NULL)
    {
        AT_FREE(pResult);
        pResult = NULL;
    }
    return;
}

VOID SA_DefaultCmdHandler_AT(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 msg[80];

    AT_TC(g_sw_ATE, "SA_DefaultCmdHandler_AT  %d",pParam->nDLCI);

    pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK, ERR_AT_OK, CMD_ERROR_CODE_OK, 0, (UINT8*)msg, 0, pParam->nDLCI);
    AT_Notify2ATM(pResult, pParam->nDLCI);
    if (pResult != NULL)
    {
        AT_FREE(pResult);
        pResult = NULL;
    }
    return;
}
/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EX_CmdFunc_RCMD(AT_CMD_PARA* pParam)
{

}

/******************************************************************************
 * SA_UartRecevDataInd: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void SA_UartRecevDataInd(COS_EVENT* pev)
{
    UINT8 nDLCI          = 0x00;
    //UINT8 nCtrlType      = 0x00;
    UINT8 uiStatus = 0;
    AT_CMD_BUF* pCmdBuf = NULL;
    UINT32 dce_err = CMD_RC_ERROR;
	PUART_DATA pUartData;
    
    AT_ASSERT(pev != NULL);
    AT_TC(g_sw_SA, "Get a message, id=0x%08x,=%d,nParam1 = %d",
          pev->nEventId, pev->nEventId, pev->nParam1);

    pUartData = (PUART_DATA)pev->nParam1;  // The pointer of the data

    if(NULL == pUartData) return;

    uiStatus = AT_GetStatus();
    pCmdBuf = AT_GetCmdBuf();

    AT_TC(g_sw_SA, "Status[%d],Rec:[%s],len:%d",uiStatus,pUartData->DataBuf,pUartData->DataLen);

    if (uiStatus == AT_STATUS_CMD_LINE)
    {
        if ((strcmp((pUartData->DataBuf), "A/") == 0) || (strcmp((pUartData->DataBuf), "a/") == 0))
        {
            if (g_AtModule.cmd_buf_bak.DataLen != 0)
                RestoreCmdBuffer();
            else
            {
                AT_FREE(pUartData);
                pUartData = NULL;
                return;
            }
        }
        else
        {
            AT_CleanCmdBuf(pCmdBuf);
            memcpy(pCmdBuf->DataBuf, pUartData->DataBuf, pUartData->DataLen);
            pCmdBuf->DataLen = pUartData->DataLen;
        }
    }
    else
    {
        memcpy(pCmdBuf->DataBuf, pUartData->DataBuf, pUartData->DataLen);
        pCmdBuf->DataLen = pUartData->DataLen;
    }

    AT_FREE(pUartData);
    pUartData = NULL;

    switch (uiStatus)
    {
        case AT_STATUS_CMD_LINE:
        {
            INT32 ret = 0;
        lab_reparse:
            ret = AT_CommandLineProcess(pCmdBuf->DataBuf, pCmdBuf->DataLen);
            if (ERR_SUCCESS == ret)
            {
                // "A/" command is coming.
                if (FirstCmdIsBSlash(nDLCI) == TRUE)
                {
                    // Clear "A/" command from command list first.
                    AT_ClearCommandList();
                    if (g_AtModule.cmd_buf_bak.DataLen != 0)
                    {
                        RestoreCmdBuffer();
                        goto lab_reparse;
                    }
                    else
                        return;
                }
                else
                {
                    BackupCmdBuffer();
                }
                AT_RunNextCommand();
            }
            else
            {
                dce_err = CMD_RC_INVCMDLINE;
                // I think it equals an exception case when a command line process failed.
                AT_TC(g_sw_ATE, "command line process failed, and enter exception process flow!");
                goto label_failed;
            }
        }
        break;
        default:
            AT_ASSERT(0);
        label_failed:
            ATE_HandleException(dce_err, nDLCI);
            AT_SetStatus(AT_STATUS_CMD_LINE); // ÖØ¸´²Ù×÷
            break;
    }
}

BOOL BAL_ATInit(VOID)
{
    AT_TC_INIT();
    AT_TC(g_sw_SA, "BAL_ApplicationInit Started...");

#ifndef WIN32
    at_RegisterYourself();
#endif
    //
    // Create a default MMI task by CSW automatically.
    // You only can change the task stack size.
    //
    AT_ModuleInit();  // Init UART1 and at module infomation

    AT_TC(g_sw_SA, "g_hAtTask creat, ...");
    //
    // Init Keyboard related HW.
    //
    return TRUE;
}


