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

#include "cswtype.h"
#include "cos.h"
#include "pmd_m.h"
#include "hal_sys.h"
#include "hal_uart.h"
#include "hal_host.h"
#include "hal_mem_map.h"
#include "hal_timers.h"

#include "bt.h"
#include "bt_ctrl.h"
#include "bt_config.h"
#include "hfp.h"
#include "papi.h"
#include "rdabt_main.h"
#include "rdabt_common.h"
#include "rdabt_spp.h"
#include "rdabt_gatt.h"
#include "manager.h"
#include "gatt.h"
#include "rdabt_a2dp.h"
#include "rdabt_drv.h"
#include "hci.h"


#define HOST_BR_MODE	0
#define HOST_LE_MODE	1

u_int8 rdabt_send_notify_pending = 0;
typedef struct{
    UINT8 cmd;
    UINT8 result;
    UINT8 length;
    UINT8 calib_flag; // 0: no calib, 1: calib clock, 2: calib finish
    union{
        UINT32 data;
        UINT8 buff[32];
    }data;
} host_debug_t;

VOLATILE host_debug_t HAL_UNCACHED_BSS host_debug_buff;

enum {
    HOST_CMD_NONE=0,
    HOST_CMD_START=1,
    HOST_CMD_END=2,
    HOST_RESULT_OK=8,
    HOST_RESULT_DATA=9,
    HOST_RESULT_CLOSED=10,
    HOST_RESULT_PENDING=11,
    HOST_RESULT_ERROR=12,
};

const BtStackModuleList* tgt_GetBtStackModuleList(void);
const bt_config_t* tgt_GetBtStackConfig(void);
void BT_Register_Modules(void);

/**********************************************************************************************************************************************************************/
extern void rdabt_send_msg_to_xcpu(COS_EVENT *event);
extern void SYSpwr_Halt_System2(void);
extern BOOL rdabt_get_bt_msg(COS_EVENT *event);
extern void rdabt_handle_app_msg(COS_EVENT *event);
/**********************************************************************************************************************************************************************/

/*
static void _Host_Timeout(void *p_link)
{
    //SYS_SendEvent2(0x21212121);
    
    RDABT_Send_Message(ADP_TIMER_IND, RDABT_ADP, RDABT_HOST_CORE, 0, NULL);
    LMtmr_Set_Timer(0xa0, _Host_Timeout, NULL, 1);
}
*/
/*****************************************************************************
* FUNCTION
*   rdabt_init
* DESCRIPTION
*   Init function if bt task
* PARAMETERS
*   task_index  IN   index of the taks
* RETURNS
*   TRUE if reset successfully
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
boolean rdabt_init(void)
{
    /* init stack */
	SYS_SendEvent2(0x2015a00a);
    RDABT_Initialise(tgt_GetBtStackConfig());
SYS_SendEvent2(0x2015a00b);
    BT_Register_Modules();

    //LMtmr_Set_Timer(0xa0, _Host_Timeout, NULL, 1);
   //rdabt_p->state = RDABT_STATE_POWER_OFF;

   return TRUE;
}

/*****************************************************************************
* FUNCTION
*   rdabt_adp_msg_dispatch
* DESCRIPTION
*   This function handle of internal message
* PARAMETERS
*   Message
* RETURNS
*   None
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
t_api rdabt_adp_msg_dispatch(rdabt_msg_t *message)
{
        //pDebugPrintfEX((pLOGNOTICE,pLOGADP,"rdabt_adp_msg_dispatch message:0x%x", message));
        switch(message->msg_id)
        {
#if COMBINED_HOST==0
            case ADP_DATA_REQ:
                {
                    adp_data_msg_t *data_msg = (adp_data_msg_t*)message->data;
                    //COS_Sleep(10);
                    RDABT_Tranport_Tx_PDU(data_msg->buff,data_msg->type,data_msg->flags,data_msg->hci_handle_flags);
                    //host_cmd_time = hal_TimGetUpTime();  
                }
                break;
            case ADP_TRANSPORT_CNF:
                {
                    u_int32 status = ((u_int32)(message->data));
                    u_int8 transport_id;
                    transport_id = status&0xf;
                    status = status >> 8;
#ifdef UART2_HCI
					RDABT_Send_Message(MGR_LOCALKEY_IND, RDABT_MANAGER, RDABT_ADP, 0, NULL);
                    break;
#endif
					
                    //RDAbt_enable_dut_write();

                    if(transport_id != TRANSPORT_UART_ID || status != RDABT_NOERROR)
                     {
                        // error
                        //BT_Send_Msg_Up(EV_BT_READY_IND, 1);
                        //hal_HstSendEvent(SYS_EVENT, 0x1306250a);
                        //rdabt_antenna_off();
                    }
        			else if(g_test_mode)
                    {
                        struct st_t_dataBuf * cmd = NULL;
                       // if(g_CalbClock)
                        {
                            // notify host to start clock calib
                            //hal_HstSendEvent(BOOT_EVENT, 0x7e57ca1b); 
                        }
                        //else
                        {
                            HCI_Inquiry(&cmd, 0x9e8b33, 10, 0);
                            HCI_SendRawPDU(TRANSPORT_UART_ID, 1, cmd);
                        }
                    }
                    else
                    {
#ifndef FCC_TEST_OPEN                   
                            RDABT_Start_Up(TRANSPORT_UART_ID);
//                            BT_RegisterHeadsetService();
   //                         BT_RegisterObexService();
      //                      BT_RegisterHIDService();
							//BT_RegisterSPPService();
#endif 
                    }
                }
                break;
#endif
#if 0
            case HOST_INIT_REQ:
               //if(rdabt_p->original_boot_state)
               {
               COS_EVENT ev = {0};
                    //app_trace(1, "HOST_INIT_CNF");
                    
                    //t_MGR_HCVersion version;
                    //u_int16 num_send;
#ifdef BT_UART_BREAK_INT_WAKEUP
                    //u_int32 value = 0x00007fff; // set hostwake  gpio to	input
                    //MGR_WritePHYRegister(0, 0x40200010, 1, &value);
#endif
			SYS_SendEvent2(0x2015a002);
                    //rdabt_active_success();
                    rdabt_a2dp_init();
                    rdabt_hfp_init();
                    rdabt_register_sdp();
                    rdabt_set_visiable();
                    MGR_ChangeLocalName("sb250");
			//rdabt_main_init();
			//rdabt_active_success();
			ev.nEventId = RDABT_USER_BT_READY_MSG;
			rdabt_send_msg_to_xcpu(&ev);
            }
               
                break;
#endif
                case HOST_INIT_CNF:
                {
                    COS_EVENT ev = {0};
			SYS_SendEvent2(0x2015a002);
			ev.nEventId = RDABT_USER_BT_READY_MSG;
			rdabt_send_msg_to_xcpu(&ev);
                    SYS_SendEvent2(0x2015a003);
               }
               
                break;
                case HOST_SWITCH_CNF:
                {
                        COS_EVENT ev = {0};
                        ev.nEventId = RDABT_USER_BT_MODE_SWITCH_MSG;
	          rdabt_send_msg_to_xcpu(&ev);
                }
                break;        
            case HOST_SHUTDOWN_CNF:
                {
                  COS_EVENT ev = {0};
                        ev.nEventId = RDABT_USER_BT_CLOSE_MSG;
	          rdabt_send_msg_to_xcpu(&ev);
			rdabt_send_notify_pending = 0;
			//rdabt_send_notify();
			SYSpwr_Halt_System2();
                }
            break;
            case ADP_WAKE_IND: // wake up bluetooth bt chip
                {
			//L1SM_SleepDisable(rdabt_cntx.l1_handle);
                    //rdabt_hcit_sharemem_tx_data(rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                    RDABT_Send_Message(ADP_WAKE_IND, RDABT_ADP, RDABT_HCI, 0, message->data);
                    //RDABT_Tranport_Tx_Data(rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                    //hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
                }
                break;
            case ADP_SLEEP_IND:
#if COMBINED_HOST!=2
                {
                    if(message->data)  // host stack sleep
                    {
#ifndef UART2_HCI
                        //hal_BtSleep();
	                    //app_trace(1, "Recieve ADP_SLEEP_IND, data=%d", message->data);
                     //   COS_KillTimer(rdabt_p->timer_id);
                     //   rdabt_p->timer_id = 0;
#endif
                    }
                    else
                    {
                        //hal_BtWakeUp();
                       // if(rdabt_p->timer_id == 0)
                         //   rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);
                    }
                }
#endif
                break;
            case MGR_RSSI_IND:
                host_debug_buff.data.data = (u_int32)(message->data);
                host_debug_buff.result = HOST_RESULT_DATA;
                break;
            case MGR_READ_REGISTER_CNF:
                host_debug_buff.data.data = (u_int32)(message->data);
                host_debug_buff.result = HOST_RESULT_DATA;
                break;
            case MGR_SCO_DATA_IND:
                {
                    COS_EVENT ev = {0};
                    ev.nEventId = RDABT_SCO_DATA_IND;
                    ev.nParam1 = (u_int32)message->data;
                    //SYS_SendEvent2(0x201505d0);
                    //SYS_SendEvent2(message->data);
                    rdabt_send_msg_to_xcpu(&ev);
                }
                break;

               case MGR_COMMON_EVENT_IND:
               {
                   COS_EVENT ev = {0};
                   ev.nEventId = RDABT_USER_BT_COMMON_EVENT_MSG;
                   ev.nParam1 = (u_int32)message->data;
                   SYS_SendEvent2(0x20150117);
                   SYS_SendEvent2((u_int32)message->data);
                   rdabt_send_msg_to_xcpu(&ev);
               }
               break;
                
            default:
                rdabt_msg_callback(message);
                break;
        }
    return RDABT_NOERROR;
}

void host_init_start(void)
{
	int status = 0;
	struct st_t_dataBuf *cmd;
	
	status = HCI_Reset(&cmd);
    	if(status == RDABT_NOERROR)
        status = HCI_SendRawPDU(0, 1, cmd);
}

u_int8 BThost_Initialise()
{
    rdabt_init();

    //host_init_start();
    RDABT_Start_Up(TRANSPORT_UART_ID);
    RDABT_Send_Message(HOST_INIT_REQ, RDABT_HCI, RDABT_HOST_CORE, 0, NULL);
    return RDABT_NOERROR;
}
/* host schedule function */
u_int8  BThost_Sched(u_int32 num_interations)
{
        u_int8 status = 0;
        COS_EVENT ev;	
        //SYS_SendEvent2(0x2015a000);    	
        //EDRV_TRACE(EDRV_BTD_TRC, 0, "BThost_Sched");	

#ifndef UART2_HCI
        do{
                status = RDABT_Execute(1);
        }while(0);
#endif   
        if(rdabt_get_bt_msg(&ev))
        {
			rdabt_handle_app_msg(&ev);
			status |= 1;
        }
        rdabt_a2dp_read_syncclk_and_start();
        return status;
}

