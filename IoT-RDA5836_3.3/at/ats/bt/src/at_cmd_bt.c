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
 *  at_cmd_bt.c
 * Author
 *  WuXiang
 * Date
 *  2016-1-29
 * Descriptions:
 *  at command for bt operation
 *
 *********************************************************/
#include "tgt_app_cfg.h"

#if APP_SUPPORT_BLUETOOTH==1
#include "at_common.h"
#include "at_module.h"
#include "at_cmd_bt.h"
#include "event.h"
#include "bt_types.h"
#include "bt_msg.h"
#include "ap_mgr.h"
#include "ap_pbap.h"
#include "ap_map.h"
#include "ap_hfp.h"
#include "ap_spp.h"
#include "ap_avrcp.h"
//#include "hal_bt.h"

static t_bdaddr cmd_addr = {{0}};
static UINT8 g_at_cmd_bt_timeout = 0;

void AT_End(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult;
    pResult = AT_CreateRC(CMD_FUNC_SUCC, CMD_RC_OK, CMD_ERROR_CODE_OK,
		CMD_ERROR_CODE_TYPE_CME, 0, pParam->pExData, pParam->iExDataLen, pParam->nDLCI);

    if (NULL != pResult)
    {
        AT_Notify2ATM(pResult, pParam->nDLCI);
        AT_FREE(pResult);
        pResult = NULL;
    }
}

u_int8 AT_Check_Param(u_int8 *param, u_int8 param_len, u_int8 check_eq)
{
    if(param == NULL)
    {
        return 0xff;
    }

    if(check_eq)
    {
        if(strlen(param) != param_len)
        {
            return 0xff;
        }
    }
    else
    {
        if(strlen(param) > param_len || strlen(param) == 0)
        {
            return 0xff;
        }
    }
    
    return 0;
}

/*------------------------------BT FUNC---------------------------------*/
VOID AT_EMOD_CmdFunc_BTOPEN(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_bt_active_req();
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTSWITCH(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 1, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    app_bt_mode_switch((unsigned char)atoi(pParam->pPara));
    AT_End(pParam);
}

#if APP_SUPPORT_LE == 1
VOID AT_EMOD_CmdFunc_LE_START_BAS(AT_CMD_PARA *pParam)
{
    if (pParam != NULL) {
        app_bt_start_bas_server_req();
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_LE_START_IAS(AT_CMD_PARA *pParam)
{
    if (pParam != NULL) {
        app_bt_start_ias_server_req();
    }
    AT_End(pParam);
}

#ifdef CODEC_SUPPORT
VOID AT_EMOD_CmdFunc_LE_START_WIFISCS(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 62, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    app_bt_start_wifiscs_server_req(pParam->pPara);

    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETCONFIGSTATE(AT_CMD_PARA *pParam)
{
    u_int8 values[4] ={0};
    int state = 0;

    state = app_bt_wifiscs_get_configState();
    sprintf(values, "%d", state);
    
	AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, values, strlen(values), pParam->nDLCI);
}


VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETAPINFO(AT_CMD_PARA *pParam)
{
    u_int8 values[256] = {0};

    hal_HstSendEvent(SYS_EVENT, 0x20171E9B);

    app_bt_wifiscs_get_apinfo(values);

	AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, values, strlen(values), pParam->nDLCI);
}

VOID AT_EMOD_CmdFunc_LE_WIFISCS_SETSTATE(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 40, FALSE))
    {
        AT_End(pParam);
        return ;
    }
    app_bt_wifiscs_set_state(pParam->pPara);

    AT_End(pParam);
}


VOID AT_EMOD_CmdFunc_LE_WIFISCS_CLEARAPINFO(AT_CMD_PARA *pParam)
{
    app_bt_wifiscs_clear_apinfo();
    AT_End(pParam);
}


VOID AT_EMOD_CmdFunc_LE_WIFISCS_SETFEATURE(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 60, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    app_bt_wifiscs_set_feature(pParam->pPara);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_LE_WIFISCS_GETINDSTATE(AT_CMD_PARA *pParam)
{
    u_int8 values[4] ={0};
    int state = 0;

    state = app_bt_wifiscs_get_ind_state();
    sprintf(values, "%d", state);
    
	AT_COMMON_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, values, strlen(values), pParam->nDLCI);
}

#endif
#endif

VOID AT_EMOD_CmdFunc_BTINQUIRY(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 1, FALSE))
    {
        AT_End(pParam);
        return ;
    }
    app_bt_inquiry_req((unsigned char)atoi(pParam->pPara), FALSE);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTINQUIRECANCEL(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_bt_inquiry_cancel_req();
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTBOND(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_bt_bonding_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTACTIVATE(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_bt_active_req();
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTCLOSE(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_bt_close_req();
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTVISIBLE(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTINVISIBLE(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTSETNAME(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 32, FALSE))
    {
        AT_End(pParam);
        return ;
    }
    
    app_bt_change_name_req(pParam->pPara);
    AT_End(pParam);
}


VOID AT_EMOD_CmdFunc_BTGETRSSI_CB(signed int rssi)
{
	INT8 values[8];
	AT_CMD_PARA sParam = {0};

	hal_HstSendEvent(SYS_EVENT, 0xb7833101);
	hal_HstSendEvent(SYS_EVENT, rssi);

	if (g_at_cmd_bt_timeout) {
		COS_KillTimer(g_at_cmd_bt_timeout);
		g_at_cmd_bt_timeout = 0;
	}

	memset(values, 0, sizeof(values));
	sprintf(values, "%d", rssi);

	sParam.pExData = values;
	sParam.iExDataLen = strlen(values);

	AT_End(&sParam);
}

VOID AT_EMOD_CmdFunc_BTGETRSSI_Timeout(VOID* pParam)
{
	INT8 values[8];
	AT_CMD_PARA sParam = {0};

	hal_HstSendEvent(SYS_EVENT, 0xb7833102);

	if (g_at_cmd_bt_timeout) {
		COS_KillTimer(g_at_cmd_bt_timeout);
		g_at_cmd_bt_timeout = 0;
	}

#if 1
	memset(values, 0, sizeof(values));
	sprintf(values, "%d", 0);

	sParam.pExData = values;
	sParam.iExDataLen = strlen(values);

	AT_End(&sParam);
#else
	AT_WriteUartString("\r\nTIMEOUT\r\n");
#endif
}

VOID AT_EMOD_CmdFunc_BTGETRSSI(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
	hal_HstSendEvent(SYS_EVENT, 0xb7833100);

	app_bt_get_rssi_req2(AT_EMOD_CmdFunc_BTGETRSSI_CB, &cmd_addr);

	if (g_at_cmd_bt_timeout) {
		COS_KillTimer(g_at_cmd_bt_timeout);
		g_at_cmd_bt_timeout = 0;
	}

	g_at_cmd_bt_timeout = COS_SetTimer(600,
		AT_EMOD_CmdFunc_BTGETRSSI_Timeout, NULL, COS_TIMER_MODE_SINGLE);

	//AT_End(pParam);
}


/*------------------------BT FUNC END-------------------------------------*/
#if APP_SUPPORT_A2DP == 1
/*------------------------A2DP FUNC---------------------------------------*/
VOID AT_EMOD_CmdFunc_A2DPCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_connect_a2dp(&cmd_addr);
  
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_A2DPDISCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_disconnect_a2dp(&cmd_addr);
    AT_End(pParam);
}

/*------------------------A2DP FUNC END-----------------------------------*/
#endif
#if APP_SUPPORT_AVRCP == 1
/*------------------------AVRCP FUNC---------------------------------------*/

static APP_AVRCP_DEVICE_INFO *at_get_active_avrcp_device(void)
{
    APP_AVRCP_DEVICE_INFO *avrcp_device = app_find_active_avrcp_device();
    APP_AVRCP_ADP_INFO *avrcp_adp_info = NULL;

    if (avrcp_device == NULL) {
        avrcp_adp_info = app_get_avrcp_adp_info();
        if (avrcp_adp_info != NULL) {
            avrcp_device = app_find_avrcp_device(avrcp_adp_info->last_addr);
        }
    }

    return avrcp_device;
}

VOID AT_EMOD_CmdFunc_BTAVRCPCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_connect_avrcp(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTAVRCPDISCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }

    app_disconnect_avrcp(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTPLAY(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};
    APP_AVRCP_DEVICE_INFO *avrcp_device = NULL;

    if (0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        avrcp_device = at_get_active_avrcp_device();
        if (avrcp_device != NULL)
            app_avrcp_send_key(OPID_PLAY, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
    }
    else
    {
        for(i = 0; i < SIZE_OF_BDADDR; i++)
        {
            tmp_unit[0] = '0';
            tmp_unit[1] = 'x';
            tmp_unit[2] = pParam->pPara[i*2];
            tmp_unit[3] = pParam->pPara[i*2+1];

            cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
        }
        app_avrcp_send_key(OPID_PLAY, AVRCP_NO_PRESS, 0, cmd_addr);
    }

    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTPAUSE(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};
    APP_AVRCP_DEVICE_INFO *avrcp_device = NULL;

    if (0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        avrcp_device = at_get_active_avrcp_device();
        if (avrcp_device != NULL)
            app_avrcp_send_key(OPID_PAUSE, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
    }
    else
    {
        for(i = 0; i < SIZE_OF_BDADDR; i++)
        {
            tmp_unit[0] = '0';
            tmp_unit[1] = 'x';
            tmp_unit[2] = pParam->pPara[i*2];
            tmp_unit[3] = pParam->pPara[i*2+1];

            cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
        }

        app_avrcp_send_key(OPID_PAUSE, AVRCP_NO_PRESS, 0, cmd_addr);
    }

    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTFORWARD(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};
    APP_AVRCP_DEVICE_INFO *avrcp_device = NULL;

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        avrcp_device = at_get_active_avrcp_device();
        if (avrcp_device != NULL)
            app_avrcp_send_key(OPID_FORWARD, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
    }
    else
    {
        for(i = 0; i < SIZE_OF_BDADDR; i++)
        {
            tmp_unit[0] = '0';
            tmp_unit[1] = 'x';
            tmp_unit[2] = pParam->pPara[i*2];
            tmp_unit[3] = pParam->pPara[i*2+1];

            cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
        }

        app_avrcp_send_key(OPID_FORWARD, AVRCP_NO_PRESS, 0, cmd_addr);
    }

    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTBACKWARD(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};
    APP_AVRCP_DEVICE_INFO *avrcp_device = NULL;

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        avrcp_device = at_get_active_avrcp_device();
        if (avrcp_device != NULL)
            app_avrcp_send_key(OPID_BACKWARD, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
    }
    else
    {
        for(i = 0; i < SIZE_OF_BDADDR; i++)
        {
            tmp_unit[0] = '0';
            tmp_unit[1] = 'x';
            tmp_unit[2] = pParam->pPara[i*2];
            tmp_unit[3] = pParam->pPara[i*2+1];

            cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
        }

        app_avrcp_send_key(OPID_BACKWARD, AVRCP_NO_PRESS, 0, cmd_addr);
    }

    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTGETTRACKINFO(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTVOLUP(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        SendAppEvent((UINT16)AT_COMMON_VOLADD, (UINT16)(AT_COMMON_VOLADD>>16));
    }
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTVOLDOWN(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        SendAppEvent((UINT16)AT_COMMON_VOLSUB, (UINT16)(AT_COMMON_VOLSUB>>16));
    }
    AT_End(pParam);
}
/*------------------------AVRCP FUNC END-----------------------------------*/
#endif
#if APP_SUPPORT_HFP == 1
/*------------------------HFP FUNC----------------------------------------*/
VOID AT_EMOD_CmdFunc_HFPCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;  
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }

    app_connect_hfp(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPDISCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {   
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_disconnect_hfp(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPACCEPTCALL(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_hfp_call_answer_req(&cmd_addr, 8);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPREJECTCALL(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_hfp_call_answer_req(&cmd_addr, 9);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPTERMINALCALL(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_hfp_call_terminal_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPCALL(AT_CMD_PARA *pParam)
{
    u_int8 call_type;
    u_int8 phone_number[16] = {0};
    u_int8 i = 0;
    u_int8 len = 0;

    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 44, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    do
    {
        if(strlen(&pParam->pPara[12]) < 11)
            break;

        call_type = pParam->pPara[12];
        call_type -= 0x30;
        len = strlen(&pParam->pPara[14]);
        if(len >=  sizeof(phone_number))
            len = sizeof(phone_number);

        strncpy(phone_number, &pParam->pPara[14], len);
        app_hfp_call_req(&cmd_addr,call_type , phone_number);
    }
    while(0);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPSENDAT(AT_CMD_PARA *pParam)
{
    u_int8 at_buf[32] = {0};
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 42, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    do
    {
        if(strlen(&pParam->pPara[12]) > sizeof(at_buf)-3)
            break;
        memcpy(at_buf, &pParam->pPara[12], strlen(&pParam->pPara[12]));
        at_buf[strlen(&pParam->pPara[12])] = '\r';
        at_buf[strlen(&pParam->pPara[12])+1] = '\n';
        app_hfp_send_raw_data(&cmd_addr, at_buf, strlen(at_buf));
    }while(0);        
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPGETPB(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }

    app_hfp_get_pbap_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPCONSCO(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_sco_connect_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HFPDISCSCO(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 1, FALSE))
    {
        AT_End(pParam);
        return ;
    }
    app_sco_disconnect_req((u_int16)atoi(pParam->pPara));
    AT_End(pParam);
/*
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = strtol(tmp_unit, NULL, 16);
    }
    
    app_sco_disconnect_req(&cmd_addr);
    AT_End(pParam);*/
}

/*------------------------HFP FUNC END------------------------------------*/
#endif

#if APP_SUPPORT_TEST_BOX == 1
extern void app_hfp_change_device_name(char *name);
VOID AT_EMOD_CmdFunc_HFPCDN(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, MAX_BT_DEVICE_NAME, FALSE))
    {
        AT_End(pParam);
        return ;
    }    

    app_hfp_change_device_name(pParam->pPara);
    AT_End(pParam);
}
#endif

#if APP_SUPPORT_SPP == 1
VOID AT_EMOD_CmdFunc_BTSPPCONNECT(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    
    app_spp_connect_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTSPPDISCONNECT(AT_CMD_PARA *pParam)
{
/*
    unsigned char i = 0;  
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = strtol(tmp_unit, NULL, 16);
    }
    */
    app_spp_disconnect_req((unsigned char)atoi(pParam->pPara));
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTSPPSENDDATA(AT_CMD_PARA *pParam)
{
    if(0 != AT_Check_Param(pParam->pPara, 128, FALSE))
    {
        AT_End(pParam);
        return ;
    }
    
    strcpy(spp_sd.data_buf->buf, pParam->pPara);
    app_spp_send_data_req(&spp_sd);
    AT_End(pParam);
}

#endif

#if APP_SUPPORT_PBAP==1
VOID AT_EMOD_CmdFunc_GETPB(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 42, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    app_get_pbap_size(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_GETCR(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 42, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    app_get_call_recends_size(&cmd_addr);
    AT_End(pParam);
}
#endif

#if APP_SUPPORT_MAP==1
VOID AT_EMOD_CmdFunc_GETMAP(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 42, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    app_get_map_size(&cmd_addr);    
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_SETMNS(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 42, FALSE))
    {
        AT_End(pParam);
        return ;
    }

    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }
    app_set_mns(&cmd_addr); 
    AT_End(pParam);
}
#endif

#if APP_SUPPORT_HID==1
VOID AT_EMOD_CmdFunc_CONHID(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;  
    unsigned char tmp_unit[5] = {0};

    if(0 != AT_Check_Param(pParam->pPara, 12, TRUE))
    {
        AT_End(pParam);
        return ;
    }
    
    for(i = 0; i < SIZE_OF_BDADDR; i++)
    {
        tmp_unit[0] = '0';
        tmp_unit[1] = 'x';
        tmp_unit[2] = pParam->pPara[i*2];
        tmp_unit[3] = pParam->pPara[i*2+1];

        cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
    }

    app_hid_connect_req(&cmd_addr);
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_HIDTP(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_hid_tp_req(NULL);
    }
    AT_End(pParam);
}
#endif

VOID AT_EMOD_CmdFunc_BTSETADDR(AT_CMD_PARA *pParam)
{

    if (pParam == NULL)
    {
        AT_End(pParam);
        return ;
    }

    switch (pParam->iType)
    {
        case AT_CMD_SET:
        {
            unsigned char i = 0;
            unsigned char tmp_unit[5] = {0};

            if (0 != AT_Check_Param(pParam->pPara, 12, TRUE))
            {
                AT_End(pParam);
                return ;
            }

            for (i = 0; i < SIZE_OF_BDADDR; i++)
            {
                tmp_unit[0] = '0';
                tmp_unit[1] = 'x';
                tmp_unit[2] = pParam->pPara[i*2];
                tmp_unit[3] = pParam->pPara[i*2+1];

                cmd_addr.bytes[5-i] = (u_int8)strtol(tmp_unit, NULL, 16);
            }
            hal_HstSendEvent(SYS_EVENT, 0x2017B7a1);
            app_bt_set_address(&cmd_addr);
            AT_End(pParam);
        }

        case AT_CMD_READ:
        {
            u_int8 *paddr;
            u_int8 value[12];
            paddr = app_bt_get_address();
            sprintf(value, "%02x%02x%02x%02x%02x%02x", paddr[5], paddr[4], paddr[3], paddr[2], paddr[1],paddr[0]);
            pParam->pExData = value;
            pParam->iExDataLen = 12;
            hal_HstSendEvent(SYS_EVENT, 0x2017B7a2);
            AT_End(pParam);
            return;
        }
    }
}

VOID AT_EMOD_CmdFunc_BTSETLNAME(AT_CMD_PARA *pParam)
{
    hal_HstSendEvent(SYS_EVENT, 0x2017B720);

    if (pParam == NULL)
    {
        AT_End(pParam);
        return ;
    }

    switch (pParam->iType)
    {
        case AT_CMD_SET:
        {
            UINT16 uStrLen = 1;
            INT32 iRetValue = 0;
            UINT8 btname[MAX_BT_DEVICE_NAME] = {0};

            if (0 != AT_Check_Param(pParam->pPara, 32, FALSE))
            {
                AT_End(pParam);
                return ;
            }

            uStrLen  = SIZEOF(btname);
            iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, &btname, &uStrLen);
            hal_HstSendEvent(SYS_EVENT, 0x2017B721);
            app_bt_set_local_name(btname);

            AT_End(pParam);
            return;
        }

        case AT_CMD_READ:
        {
            u_int8 *pname;

            pname = app_bt_get_local_name();

            pParam->pExData = pname;
            pParam->iExDataLen = strlen(pname);
            hal_HstSendEvent(SYS_EVENT, 0x2017B722);
            AT_End(pParam);
            return;
        }
    }
}

VOID AT_EMOD_CmdFunc_BTGETSTATUS(AT_CMD_PARA *pParam)
{
    u_int8 values[8];
    u_int8 state = 0;

    hal_HstSendEvent(SYS_EVENT, 0x2017B730);

    if (pParam != NULL)
    {
        state = app_get_mgr_state();

        memset(values, 0, sizeof(values));
        sprintf(values, "%d", state);

        pParam->pExData = values;
        pParam->iExDataLen = strlen(values);
    }
	AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_BTGETA2DPSTATUS(AT_CMD_PARA *pParam)
{
    u_int8 values[8];
    u_int8 state = 0;

    hal_HstSendEvent(SYS_EVENT, 0x2017B7A3);

    if (pParam != NULL)
    {
        state = app_get_a2dp_state();

        memset(values, 0, sizeof(values));
        sprintf(values, "%d", state);

        pParam->pExData = values;
        pParam->iExDataLen = strlen(values);
    }
	AT_End(pParam);
}

#if APP_SUPPORT_FCC==1
VOID AT_EMOD_CmdFunc_FCC(AT_CMD_PARA *pParam)
{
    unsigned char i = 0;
    //unsigned char hop_enable = 0;
    //unsigned char br_edr = 0;
    //unsigned char packet_type = 0;
    unsigned char param[4] = {0};
    unsigned char hop_frq[2] = {0};
    unsigned char power_level[2] = {0};
    unsigned char tx_rx;

    if(0 != AT_Check_Param(pParam->pPara, 10, FALSE))
    {
        return ;
    }
    
    param[0] = pParam->pPara[i++];
    param[1] = pParam->pPara[i++];
    param[2] = pParam->pPara[i++]; // 3
    
    if(pParam->pPara[i+2] == ',')
    {
        hop_frq[0]='0';
        hop_frq[1]=pParam->pPara[++i];
        i++; //5
    }
    else
    {
        hop_frq[0]=pParam->pPara[++i];
        hop_frq[1]=pParam->pPara[++i];
        i++;  //6
    }
    if(pParam->pPara[i+2] == ',')
    {
        power_level[0]='0';
        power_level[1]=pParam->pPara[++i];
        i++;
    }
    else
    {
        power_level[0]=pParam->pPara[++i];
        power_level[1]=pParam->pPara[++i];
        i++;
    }
    tx_rx = pParam->pPara[++i];

    app_fcc_test(atoi(param), atoi(hop_frq), atoi(power_level), atoi(&tx_rx));
    AT_End(pParam);
}

VOID AT_EMOD_CmdFunc_FCCSTOP(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_fcc_test_stop();
    }
    AT_End(pParam);
}

#endif

#if APP_SUPPORT_DUT==1
VOID AT_EMOD_CmdFunc_BTDUTTEST(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
#ifndef WIN32    
        hal_BtEnableSleep(0);
#endif
        app_bt_dut_test_req();
    }
    AT_End(pParam);
}
#endif

#if APP_SUPPORT_VCO==1
VOID AT_EMOD_CmdFunc_BTVCOTEST(AT_CMD_PARA *pParam)
{
    if(pParam != NULL)
    {
        app_bt_vco_test_req();
    }
    AT_End(pParam);
}
#endif

#endif
