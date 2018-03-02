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

#include "hal_sys.h"
#include "tgt_app_cfg.h"

#ifndef WIN32
#include "pmd_m.h"

#include "hal_uart.h"
#include "hal_host.h"
#include "hal_mem_map.h"
#include "hal_timers.h"
#endif

#ifndef WIN32
#include "rdabt_drv.h"
#endif

#include "bt.h"
#include "bt_ctrl.h"
#include "bt_config.h"
#include "hfp.h"
#include "hci.h"
#include "papi.h"
#include "rdabt_main.h"
#include "rdabt_common.h"
#include "rdabt_spp.h"
#include "rdabt_avrcp.h"
#include "rdabt_a2dp.h"
#include "rdabt_hfp.h"
#include "rdabt_manager.h"
#include "rdabt_gatt.h"
#include "manager.h"
#include "gatt.h"
#include "rdabt_hid.h"
#include "rdabt_pbap.h"


#define HOST_BR_MODE	0
#define HOST_LE_MODE	1

#if APP_SUPPORT_FCC==1

typedef struct
{
    u_int32 param;
    u_int8  hop_frq;
    u_int8 power_level;
    u_int8 tx_rx;
}FCC_TEST_PARAM;

extern FCC_TEST_PARAM fcc_param;

#endif

#if APP_SUPPORT_TEST_BOX==1
typedef struct
{
    t_bdaddr addr;
    char device_name[MAX_BT_DEVICE_NAME];
}APP_CHANGE_DEVICE_NAME_INFO;
#endif

#if 0//APP_SUPPORT_A2DP_SOURCE==1

typedef struct
{
    t_bdaddr a2dp_addr;
    u_int32 length;
    u_int8 *p_data;
    u_int32 timestamp;
    u_int16 seq_num;
    u_int8 payload_type;
    u_int8 marker;
    u_int8 frames;
}RDABT_A2DP_DATA_INFO;

#endif
typedef struct
{
    u_int32 key;
    u_int32 param;
}KEY_MSG_INFO;

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

u_int8  rdabt_send_timer_expiry_pending = 0;

#ifdef WIN32
extern u_int8  rdabt_send_notify_pending;
#endif


#if BLUETOOTH_USE_MARLIN==1
extern u_int8  rdabt_send_notify_pending;
extern u_int8  rdabt_send_data_ready ;
extern u_int8  rdabt_send_data_ready2 ;

static u_int8 rdabt_wake_chip_data[] = {0xff};
#endif
const BtStackModuleList* tgt_GetBtStackModuleList(void);
const bt_config_t* tgt_GetBtStackConfig(void);
void BT_Register_Modules(void);

/**********************************************************************************************************************************************************************/
extern APIDECL1 t_api APIDECL2 RDABT_GetHostMode(void);
extern APIDECL1 t_api APIDECL2 MGR_BaseBand_RXTest(u_int8 hop_frq,u_int8 EDR_mode,u_int8 pattern,u_int8 is_whiten, u_int8 whiten_init,u_int8 am_addr,u_int32 total_packet);
extern void _TRAhcit_Set_Init_Fccsleep_Flag(u_int8 init_flag);
extern APIDECL1 t_api APIDECL2 RDABT_GetHostState(void);
extern void TRAhcit_Set_Init_Timer(void);
extern void Rdabt_vco_test_Write_r21(void);
extern int HF_Send_Set_Link_Timeout(t_bdaddr *address);
#if CHIP_HAS_BTCPU==1
extern void rdabt_send_msg_to_xcpu(COS_EVENT *event);
#endif
#if APP_SUPPORT_HID==1
extern void rdabt_hid_register_sdp(t_hidOpcode type, t_hid_attributes *p_hid_attribute);
extern void rdabt_hid_send_data(RDABT_Hid_Data_Info *sd);
extern void rdabt_hid_msg_handle(u_int16 msg_id, void* msg_data);
#endif
/**********************************************************************************************************************************************************************/

//#ifndef WIN32
#if CHIP_HAS_BTCPU==0
void rdabt_send_msg_to_bt(COS_EVENT *event)
{
    COS_SendEvent(MOD_BT, event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void rdabt_send_msg_to_xcpu(COS_EVENT *event)
{
    COS_SendEvent(MOD_APP, event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
#endif
//#endif

void rdabt_register_sdp(u_int8 is_headset)
{
#if APP_SUPPORT_HFP_AG==1
    rdabt_hfp_register_ag_sdp();
#elif APP_SUPPORT_HFP==1 
#if APP_SUPPORT_HID_MODE==1
    if(is_headset)
#endif    
        rdabt_hfp_register_hf_sdp();
        
#endif

#if APP_SUPPORT_A2DP==1 
#if APP_SUPPORT_HID_MODE==1
    if(is_headset)
#endif  
        rdabt_a2dp_register_sink_sdp();
#endif

#if APP_SUPPORT_A2DP_SOURCE==1
    rdabt_a2dp_register_source_sdp();
#endif

#if APP_SUPPORT_AVRCP==1
#if APP_SUPPORT_HID_MODE==1
    if(is_headset)
#endif
        rdabt_avrcp_register_controller_sdp();
        
#if APP_SUPPORT_AVRCP_SYNC_VOL==1
#if APP_SUPPORT_HID_MODE==1
    if(is_headset)
#endif
        rdabt_avrcp_register_target_sdp();
#endif
#endif

#if APP_SUPPORT_PBAP==1
#endif
#if APP_SUPPORT_MAP==1
        rdabt_map_register_sdp();
#endif	
#if APP_SUPPORT_HID==1
        //rdabt_hid_register_sdp();   
#endif
#if APP_SUPPORT_SPP==1
        rdabt_spp_register_services();    
#endif
}

INT8 rdabt_active_success(u_int8 is_headset)
{
    if(RDABT_GetHostMode() == HOST_BR_MODE)
    {
#if APP_SUPPORT_HFP==1 
        rdabt_hfp_init();
#endif
#if APP_SUPPORT_A2DP==1 
        rdabt_a2dp_init();
#endif
#if APP_SUPPORT_AVRCP==1
        rdabt_avrcp_init();
#endif
#if APP_SUPPORT_PBAP==1
        rdabt_pbap_init();
#endif
#if APP_SUPPORT_MAP==1
        rdabt_map_init();
#endif	
#if APP_SUPPORT_SPP==1
        rdabt_spp_adp_init(); 
#endif
        rdabt_register_sdp(is_headset);
     }
     else if(RDABT_GetHostMode() == HOST_LE_MODE)
     {
#if APP_SUPPORT_LE==1
        rdabt_gatt_init();
#endif
     }   
     return 0;
}

void rdabt_adp_free_br(void)
{
#if APP_SUPPORT_HFP==1 
        rdabt_hfp_free();
#endif
#if APP_SUPPORT_A2DP==1 
        rdabt_a2dp_free();
#endif
#if APP_SUPPORT_AVRCP==1
        rdabt_avrcp_free();
#endif
#if APP_SUPPORT_PBAP==1
        rdabt_pbap_free();
#endif
#if APP_SUPPORT_MAP==1
        rdabt_map_free();
#endif	
#if APP_SUPPORT_SPP==1
        rdabt_spp_free(); 
#endif
    rdabt_mgr_free();
}

void rdabt_adp_free_le(void)
{
#if APP_SUPPORT_LE==1
    rdabt_gatt_free();
#endif
}

void BT_Register_Modules(void)
{
    const BtStackModuleList *bt_stack_module_list = tgt_GetBtStackModuleList();
    while(bt_stack_module_list->module_id != 0)
    {
        RDABT_AddModule(bt_stack_module_list->module_id,
                                       bt_stack_module_list->ctrl_fun,
                                       bt_stack_module_list->msg_dispatch);
        bt_stack_module_list++;
    }
}

#if APP_SUPPORT_FCC==1
#define RDA_WRITE_UINT32( _register_, _value_ )    (*((volatile unsigned int *)(_register_)) = (_value_))

void rdabt_fcc_test(u_int32 fcc_params, u_int8 hop_frq, u_int8 power_level, u_int8 tx_rx)
{
    u_int8 hop_enable, br_edr;
    u_int32 tx_len, packet_type;
    u_int8 offset = 0;
    u_int32 packet_len[] = {0x19, 0xb7, 0x153, 0x36, 0x16f, 0x2a7, 0x53, 0x228, 0x3fd};
    u_int32 packet_type_array[] = {0x4, 0xb, 0xf, 0x4, 0xa, 0xe, 0x8, 0xb, 0xf};
	u_int16 fcc_ctrl;

	br_edr = fcc_params /10;
	while(br_edr > 10)
    {
        br_edr /= 10;
    }
    if(tx_rx)
    {
        packet_type = fcc_params % 10;
        
        hop_enable = fcc_params / 100;
        while(packet_type > 10)
        {
            packet_type %= 10;
        }


        if(packet_type == 1)
            offset = 0;
        else if(packet_type == 3)
            offset = 1;
        else if(packet_type == 5)
            offset = 2;

		if(power_level == 0)
			power_level = 0xd;
		else if(power_level == 1)
			power_level = 0xe;
		else
			power_level = 0xf;
        tx_len = packet_len[((br_edr-1)*3) + offset];
        packet_type = packet_type_array[((br_edr-1)*3) + offset];
		if(tx_rx == 1)
                    MGR_BaseBand_TXTest(hop_frq, 2, power_level, packet_type, br_edr-1,  tx_len, 0, 0xe23a1a33, 0xce2c7a4e, hop_enable, 0, 0, 0);
		else
		{
			RDA_WRITE_UINT32(0xa1600020,0X00000000);// tx off
			RDA_WRITE_UINT32(0xa16082fc,0X00000000);
            RDA_WRITE_UINT32(0xa16082ac,0X000000b2);// 2b
			
			fcc_ctrl = power_level & 0x0F;
			fcc_ctrl = (fcc_ctrl << 5) + (0x03 & 0x1F);
			fcc_ctrl = fcc_ctrl << 7;
			RDA_WRITE_UINT32(0xa1608208,fcc_ctrl + hop_frq);
			RDA_WRITE_UINT32(0xa1600020,0x00000041);
    }
	}
    else
    {
    	MGR_BaseBand_RXTest( hop_frq,0 /*br_edr-1*/, 0x1,0,0,0,0x3e8/*total_pkts*/);
    }
}

void rdabt_fcc_test_stop(void)
{
    MGR_BaseBand_TestEnd();
}
#endif

void rdabt_handle_app_msg(COS_EVENT *event)
{
    switch (event->nEventId) 
    {
#if (defined WIN32  || BLUETOOTH_USE_MARLIN==1)
        case EV_BT_TIMER_EXPIRY_IND:   /* timer routine */
        {
            rdabt_send_timer_expiry_pending = 0;
            RDABT_Send_Message(ADP_TIMER_IND, RDABT_ADP, RDABT_HOST_CORE, 0, NULL);
            break;
        }
        
        case EV_BT_NOTIFY_IND:
        {
            UINT32 status = hal_SysEnterCriticalSection();
            if(rdabt_send_notify_pending)
                rdabt_send_notify_pending--;
            
            hal_SysExitCriticalSection(status);
            break;  
        }
#endif

#if BLUETOOTH_USE_MARLIN==1
        case EV_BT_WAKE_UP_REQ:
        {
            //hal_HstSendEvent(SYS_EVENT,0x04070189);

            rdabt_marlin_bt_wakeup();
            RDABT_Send_Message(ADP_WAKE_IND, RDABT_ADP, RDABT_HCI, 0, BT_UART_PORT2);
            break;
        }


        case EV_BT_READY_TO_READ_IND:
        {
            UINT32 status;
            //hal_HstSendEvent(SYS_EVENT,0x04070120);
            //status= hal_SysEnterCriticalSection();
            if(rdabt_send_data_ready)
                rdabt_send_data_ready--;

            if(rdabt_send_data_ready2)
                rdabt_send_data_ready2--;

            rdabt_adp_uart_rx(event->nParam1);
            //host_cmd_time = hal_TimGetUpTime();
            //hal_SysExitCriticalSection(status);
        }
        break;
#endif        

#ifndef UART2_HCI
        case EV_BT_POWEROFF_REQ: 
        {
            RDABT_Shut_Down();
            rdabt_send_notify();
        }
        break;
        
        case RDABT_USER_BT_MODE_SWITCH_REQ: 
        {
            if(RDABT_GetHostMode() == HOST_BR_MODE)
            {
                rdabt_adp_free_br();
            }
            else if(RDABT_GetHostMode() == HOST_LE_MODE)
            {
                rdabt_adp_free_le();
            }
            rdabt_mode_switch(event->nParam1);
        }
        break;
        
        case RDABT_USER_BT_CLOSE_REQ:
        {
            if(RDABT_GetHostMode() == HOST_BR_MODE)
            {
                rdabt_adp_free_br();
            }
            else if(RDABT_GetHostMode() == HOST_LE_MODE)
            {
                rdabt_adp_free_le();
            }
            
#ifndef WIN32        
            _TRAhcit_Set_Init_Fccsleep_Flag(0);
#endif

            RDABT_Shut_Down();
        }
        break;
        
        case RDABT_USER_BT_INIT_REQ:
        {
            if(5 == RDABT_GetHostState())
            {
                event->nEventId = RDABT_USER_BT_READY_MSG;
                rdabt_send_msg_to_xcpu(event);
            }
        }
        break;
        
        case RDABT_STACK_INIT_REQ: 
        {
            RDABT_MGR_ADP_INFO *tmp = (RDABT_MGR_ADP_INFO*)event->nParam1;
            u_int8 is_headset=1;
            if(RDABT_GetHostMode() == HOST_BR_MODE)
            {
                SYS_SendEvent2(0x99880002);
                rdabt_common_init(event);
                rdabt_active_init();
            }
            else
            {
                SYS_SendEvent2(0x99880001);
                rdabt_le_addr_init(event);
            }

            if((tmp->device_class & 0x500) == 0x500)
                is_headset = 0;

            rdabt_active_success(is_headset);
            // enable btcpu sleep
#if COMBINED_HOST!=0
            TRAhcit_Set_Init_Timer();
#endif
            event->nEventId = RDABT_STACK_INIT_MSG;
            event->nParam1 = RDABT_GetHostMode();
            rdabt_send_msg_to_xcpu(event);
        }
        break;
        
        case RDABT_MGR_ADP_INFO_REQ:
            rdabt_mgr_return_adp_info();
        break;
        
        case RDABT_MODIFY_PINCODE_REQ:
        if(RDABT_GetHostMode() == HOST_BR_MODE)
        {
            SYS_SendEvent2(0x9988000a);
            rdabt_change_local_pincode(event);
        }
        break;
        
        case RDABT_SCO_CONNECT_REQ:
        if (event->nParam1 != NULL) 
        {
            rdabt_mgr_sco_connect(*(t_bdaddr *) (event->nParam1));
        }
        break;
        
        case RDABT_SCO_DISCONNECT_REQ:
        {
            rdabt_mgr_sco_disconnect((event->nParam1));
        }
        break;
        
        case RDABT_REMOVE_DEVICE_REQ:
        if (event->nParam1 != NULL) 
        {
            rdabt_removedevice_by_addr(*(t_bdaddr *) (event->nParam1));
        }
        break; 
        
        case RDABT_GET_REMOTE_NAME_REQ:
        if (event->nParam1 != NULL) 
        {
            rdabt_get_remote_name_req(*(t_bdaddr *) (event->nParam1));
        }
        break;   
        
        case RDABT_GET_RSSI_REQ:
        if (event->nParam1 != NULL) 
        {
            rdabt_get_rssi_by_addr(*(t_bdaddr *) (event->nParam1));
        }
        break;
        
        case RDABT_USER_BT_DUT_TEST_REQ:
        {
            rdabt_mgr_test();
        }
        break;
        
        case RDABT_VCO_TEST_REQ: 
        {
#ifndef WIN32		
#if  APP_SUPPORT_VCO == 1           
            Rdabt_vco_test_Write_r21();
#endif		
#endif
        }
        break;
        
        case RDABT_USER_SEND_KEY_REQ: 
        {

        }
        break;

        case RDABT_AVRCP_SEND_KEY_ADDR_REQ: 
        {

        }
        break;
        
#if APP_SUPPORT_TEST_BOX == 1        
        case RDABT_HFP_CHANGE_DEVICE_NAME_REQ:
        {
            APP_CHANGE_DEVICE_NAME_INFO *info = (APP_CHANGE_DEVICE_NAME_INFO *)event->nParam1;
            HF_Change_Device_Name(&info->addr, info->device_name);
        }
        break;
#endif        

#if APP_SUPPORT_HFP == 1   
        case RDABT_HFP_SEND_NREC_MSG:
        if(event->nParam1 != NULL)
        {
            rdabt_hfp_send_NREC(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_HFP_SET_LINK_TIMEOUT_REQ:
        if(event->nParam1 != NULL)    
        {
            HF_Send_Set_Link_Timeout((t_bdaddr *)(event->nParam1));
        }
        break;  
        
        case RDABT_HFG_CONNECT_REQ:
        if(event->nParam1 != NULL)
        {
#if APP_SUPPORT_HFP_AG==1
            rdabt_connect_hfp_hs(*(t_bdaddr *)(event->nParam1));
#else
            rdabt_connect_hfp(*(t_bdaddr *)(event->nParam1));
#endif
        }
        break;
        
        case RDABT_HFG_DISCONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_disconnect_hfp(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_HFG_DISCONNECT_ALL_REQ:
        {
            rdabt_hfp_disconnect_all();
        }
        break;
        
        case RDABT_HFP_GET_ADP_INFO_REQ:
        {
            rdabt_hfp_return_adp_info();
        }
        break;
        
        case RDABT_HFG_CALL_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_hfp_call_req((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HFG_CALL_ANSWER_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_hfp_call_answer_req((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HFG_CALL_TERMINAL_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_hfp_terminal_call_req((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HFG_SEND_DATA_REQ:
        {
            rdabt_hfp_send_raw_data((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HF_GET_PBAP_REQ:
        {
            rdabt_hfp_get_pbap_req((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HF_GET_PBAP_DATA_REQ:
        {
            rdabt_hfp_get_pb_data_req((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HFP_SEND_SPEAKER_GAIN_REQ:
        {
            rdabt_hfp_send_speaker_gain((RDABT_HFP_MSG_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_HFP_SEND_VOICE_RECOGNITION_REQ:
        {
            HF_Activate_Voice_Recognition((t_bdaddr *)event->nParam1);
        }
        break;
        
        case RDABT_HFP_FREE_MSG_REQ:
        {
            rdabt_free((void*)event->nParam1);
        }
        break;   
        case RDABT_HFP_ENABLE_MULTI_CALL_REQ:
        {
            HF_Enable_Called_Waiting((t_bdaddr *)event->nParam1);
        }
        break;
#endif         

#if APP_SUPPORT_SPP == 1              
        case RDABT_SPP_SUPPORT_SOUNDBAR:   
        {
            rdabt_set_support_soundbar();
        }   
        break;
        
        case RDABT_SPP_SEND_SOUNDBAR_VOLUME:
        {
            rdabt_set_local_soundbar_volume(event->nParam1);
        }
        break;
        
        case RDABT_SPP_CONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_spp_connect(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_SPP_DISCONNECT_REQ:
        //if(event->nParam1 != NULL)
        {
            rdabt_spp_disconnect((u_int16)event->nParam1);
        }
        break;
        
        case RDABT_SPP_GET_ADP_INFO_REQ:
        {
            rdabt_spp_return_adp_info();
        }
        break;
        
        case RDABT_SPP_SEND_DATA_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_adp_spp_send_data((RDABT_SPP_SEND_DATA_STRUCT *)event->nParam1);
        }
        break;
        
        case RDABT_SPP_FREE_DATA_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_spp_free_buf((t_DataBuf*)event->nParam1);
        }
        break;
#endif   

#if APP_SUPPORT_A2DP == 1              
        case RDABT_A2DP_DATA_COMPLETED_MSG:
        if(event->nParam1 != NULL)
        {
            rdabt_free_buf((t_DataBuf *)event->nParam1, 2);
        }
        break;
        
        case RDABT_A2DP_CONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_a2dp_connect(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_A2DP_CONNECT_MEDIA_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_a2dp_connect_media(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_A2DP_DISCONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_a2dp_disconnect(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_A2DP_DISCONNECT_ALL_REQ:
        {
            rdabt_a2dp_disconnect_all();
        }
        break;   
        
        case RDABT_A2DP_GET_ADP_INFO_REQ:
        {
            rdabt_a2dp_return_adp_info();
        }
        break;
        
        case RDABT_A2DP_SEND_SUSPEND_REQ:
        {
            rdabt_a2dp_send_suspend(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_A2DP_SEND_AUDIO_SYNC_CLK_REQ:
        if(event->nParam1 != NULL)
        {
            SOUNBAR_AUDIO_SYNC_STRUCT *temp_audio_sync_data = (SOUNBAR_AUDIO_SYNC_STRUCT *)event->nParam1;
            rdabt_a2dp_set_audio_sync_clk(temp_audio_sync_data->current_clk,temp_audio_sync_data->counterNum);
        }
        break;
        
        case RDABT_A2DP_DROP_SBC_REQ:
        {
            if(event->nParam1 != NULL)
            {
                rdabt_a2dp_drop_sbc(*(t_bdaddr *)(event->nParam1));
            }
        }
        break;
        
        case RDABT_A2DP_RESUME_SBC_REQ:
        {
            if(event->nParam1 != NULL)
            {
                rdabt_a2dp_resume_sbc(*(t_bdaddr *)(event->nParam1));
            }
        }
        break;
        
#if APP_SUPPORT_A2DP_SOURCE==1
        case RDABT_A2DP_SEND_DATA_REQ:
        {   
            rdabt_a2dp_send_data((RDABT_A2DP_DATA_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_A2DP_SEND_START_REQ:
        {
            rdabt_a2dp_send_start(*(t_bdaddr *)(event->nParam1));
        }
        break;    
#endif
#endif 

#if APP_SUPPORT_AVRCP==1
        case RDABT_AVRCP_CONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_avrcp_connect(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_AVRCP_DISCONNECT_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_avrcp_disconnect(*(t_bdaddr *)(event->nParam1));
        }
        break;
        
        case RDABT_AVRCP_DISCONNECT_ALL_REQ:
        {
            rdabt_avrcp_disconnect_all();
        }
        break;
        
        case RDABT_AVRCP_SEND_KEY_REQ:
        {
            RDABT_AVRCP_KEY_INFO *key_info = (RDABT_AVRCP_KEY_INFO *)event->nParam1;
            rdabt_avrcp_send_key(key_info->opid, key_info->press, key_info->param, key_info->addr);
            key_info->pending = 0;
        }
        break;
        
        case RDABT_AVRCP_GET_ADP_INFO_REQ:
            rdabt_avrcp_return_adp_info();
        break;
        
        case RDABT_AVRCP_VENDOR_DATA_FINISH_REQ:
            rdabt_avrcp_finish_vendor_data((u_int8 *)event->nParam1);
        break;
        
        case RDABT_AVRCP_VOLUME_SYNC_REQ:
            rdabt_avrcp_sync_volume(event->nParam1);
        break;
#endif

        case RDABT_USER_BT_CHANGE_NAME_REQ:
            rdabt_change_local_name((char *)event->nParam1);
        break;
        
        case RDABT_USER_BT_SET_VISIBLE_REQ:
            rdabt_set_visiable(event->nParam1);
        break;
        
        case RDABT_USER_BT_SET_CONNECTABLE_REQ:
            MGR_SetPairingMode(event->nParam1);
            MGR_SetConnectableMode(event->nParam1);
        break;
        
        case RDABT_USER_BT_INQUIRY_REQ:
            rdabt_mgr_inquiry_device((RDABT_ADP_INQUIRY_DATA *)event->nParam1);
        break;
        
        case RDABT_USER_BT_INQUIRT_CANCEL_REQ:
            rdabt_mgr_inquiry_cancel();
        break;
        
        case RDABT_USER_BT_BONDING_REQ:
            MGR_InitBonding(*(t_bdaddr *) (event->nParam1));
        break;
        
#if APP_SUPPORT_PBAP==1             
        case RDABT_PBAP_GET_SIZE_REQ:
        if(event->nParam1 != NULL)
        {
            rdabt_pbap_download((Rdabt_pbap_msg *)event->nParam1);
        }
        break;
        
        case RDABT_PBAP_DOWNLOAD_REQ:
        {
            rdabt_pbap_get_data((Rdabt_pbap_msg *)event->nParam1);
        }
        break;
        
        case RDABT_PBAP_DISCONNECT_REQ:
        {
            rdabt_pbap_disconnect();
        }
        break;
#endif

#if APP_SUPPORT_MAP==1
        case RDABT_MAP_GET_SIZE_REQ: 
        {
            rdabt_map_download((Rdabt_map_msg *)event->nParam1);
        }
        break;
        
        case RDABT_MAP_SET_MNS_REQ: 
        {
            rdabt_msg_notify((Rdabt_map_msg *)event->nParam1);
        }
        break;
        
        case RDABT_MAP_DOWNLOAD_REQ: 
        {
        rdabt_map_get_data((Rdabt_map_msg *)event->nParam1);
        }
        break;
        
        case RDABT_MAP_GET_DATA_REQ: 
        {
            rdabt_msg_get_data(event->nParam1);
        }
        break;
        
        case RDABT_MAS_DISCONNECT_REQ: 
        {
            rdabt_mas_disconnect();
        }
        break;
        
        case RDABT_MNS_DISCONNECT_REQ: 
        {
            rdabt_mns_disconnect();
        }
        break;
#endif     

#if APP_SUPPORT_HID==1
        case RDABT_HID_REGISTER_SDP_REQ:
        {
            RDABT_Hid_Sdp_Info *sdp_info = (RDABT_Hid_Sdp_Info *)event->nParam1;
            rdabt_hid_register_sdp(sdp_info->type, sdp_info->p_hid_attributes);
        }
        break; 
        
        case RDABT_HID_TXDATA_REQ:
        {
            rdabt_hid_send_data((RDABT_Hid_Data_Info *)event->nParam1);
        }
        break;  
        
        case RDABT_HID_CONNECT_REQ:
        {
            rdabt_hid_connect_req((t_bdaddr *)event->nParam1);
        }
        break;  
        
        case RDABT_HID_DISCONNECT_REQ:
        {
            rdabt_hid_disconnect_req();
        }
        break;
        
#endif
#if COMBINED_HOST!=0
        case RDABT_SCO_DATA_RES:
        {
            hci_sco_data_msg_t *sco_data = (hci_sco_data_msg_t*)(event->nParam1);
            HCI_SendSCOData(sco_data->handle, sco_data->length, (u_int8*)sco_data->data);
        }
        break;
#endif

#if APP_SUPPORT_LE==1
        case RDABT_GATT_ADD_SERVICE_REQ: 
        {
            rdabt_add_ble_service((adp_gatt_data_msg_t*) (event->nParam1));
        }
        break;
        
        case RDABT_GATT_SEND_NOTIFY_REQ:
        {
            rdabt_send_notification((RDABT_LE_DATA_INFO *)event->nParam1);
        }
        break;
        
        case RDABT_GATT_SET_VISIBLE_REQ:
        {
            rdabt_ble_set_visible((adp_gatt_data_msg_t*) (event->nParam1));
        }
        break;
#endif

#if APP_SUPPORT_FCC==1
        case RDABT_FCC_TEST_REQ:
        {
            FCC_TEST_PARAM *fcc_test_param = (FCC_TEST_PARAM *)event->nParam1;
            MGR_SetDiscoverableMode(FALSE);
            MGR_SetConnectableMode(FALSE);
            rdabt_fcc_test(fcc_test_param->param, fcc_test_param->hop_frq, fcc_test_param->power_level, fcc_test_param->tx_rx);
        }
        break;   
        
        case RDABT_FCC_TEST_STOP_REQ:
        {
            rdabt_fcc_test_stop();    
        }
        break;
#endif

        case RDABT_MGR_FREE_DATA_REQ:
        {
            if(event->nParam1 != NULL)
            {
                t_EventBuf *event_buf = (t_EventBuf *)event->nParam1;
                SYS_SendEvent2(0x20160121);
                SYS_SendEvent2((u_int32)event_buf);

                if(event_buf)
                {
                    rdabt_free(event_buf); 
                    if(event_buf->buf)
                        rdabt_free(event_buf->buf); 
                }
            }

        }
        break;

#endif // UART2_HCI
        }
}

void rdabt_msg_callback(rdabt_msg_t *message)
{
	u_int16 msg_id = message->msg_id;

    if(RDABT_GetHostState() != 5)
    {
        return ;
    }
       //pDebugPrintfEX((pLOGNOTICE,pLOGADP,"rdabt_msg_callback message:0x%x", message));
	if((msg_id>=MGR_CHANGE_LOCAL_NAME && msg_id <=SDP_SERVICE_SEARCH_CNF))
	{
		rdabt_manager_msg_handle(msg_id, message->data);
	}
#if APP_SUPPORT_HFP==1
	else if((msg_id>=HFG_ACTIVATE_CNF && msg_id <=HFG_BATTERY_OK_IND))
	{
		rdabt_hfp_msg_handle(msg_id, message->data);
	}
#endif
#if APP_SUPPORT_A2DP==1
	else if((msg_id>=A2DP_SIGNAL_CONNECT_CNF && msg_id <=A2DP_SEND_SEP_DELAY_REPORT_IND))
	{
		rdabt_a2dp_msg_handle(msg_id, message->data);
	}
#endif
#if APP_SUPPORT_AVRCP==1
	else if((msg_id>=AVRCP_ACTIVATE_CNF && msg_id <=AVRCP_BROWSING_DATA_IND))
	{
		rdabt_avrcp_msg_handle(msg_id, message->data);
	}
#endif
	else if(message->msg_id >=OBEX_CLI_PUSH_SERVER_CNF &&  message->msg_id <=OBEX_SERVER_AUTHORIZE_IND)
	{
#if APP_SUPPORT_PBAP==1
		rdabt_pbap_msg_handle(msg_id, message->data);
#endif
#if APP_SUPPORT_MAP==1
		rdabt_map_msg_handle(msg_id, message->data);
#endif
	}
#if APP_SUPPORT_HID==1
	else if(message->msg_id >=HID_CONNECT_IND &&  message->msg_id <=HID_DISCONNECT_CNF)
	{
		rdabt_hid_msg_handle(msg_id, message->data);
	}
#endif
#if APP_SUPPORT_SPP == 1
	else if(message->msg_id >= SPP_CONNECT_IND && message->msg_id <= SPP_DISCONNECT_CNF)
	{
		rdabt_spp_msg_handle(msg_id, message->data);
	}
#endif	
	//BT_Execute();

}

