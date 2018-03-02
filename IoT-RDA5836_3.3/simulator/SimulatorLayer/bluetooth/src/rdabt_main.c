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



//#include "project_config.h"
#include "cswtype.h"
#include "cos.h"
#include "bt.h"
#include "bt_ctrl.h"
#include "papi.h"
#include "rdabt_main.h"
//#include "rdabt_hcit.h"
#include "hcit_win32_usb.h"
/*#include "rdabt_mgr.h"
#include "rdabt_hfp.h"
#include "rdabt_opp.h"
#include "rdabt_spp.h"
#include "rdabt_a2dp.h"
#include "rdabt_avrcp.h"*/
#include "manager.h"
#include "hfp.h"
#include "a2dp.h"
#include "rdabt_common.h"
#include "gatt.h"
#include "tgt_app_cfg.h"
#include "tgt_btd_cfg.h"
#include "hci.h"


rdabt_context_struct rdabt_cntx;
rdabt_context_struct* rdabt_p = &rdabt_cntx;

/* Set the device Address                       */
t_bdaddr local_addr;

#define NVRAM_EF_RDABT_SETTING_LID NVRAM_EF_BTRADIO_MT6612_LID
#define NVRAM_EF_RDABT_SETTING_SIZE  NVRAM_EF_BTRADIO_MT6612_SIZE
#define RDABT_WAKE_UP_TIME       					80 //00//5000       /* host wake up time */


extern int 	bonding_role; 	
extern char *  deviceName;
extern u_int8  nameLen;

u_int8  rdabt_send_notify_pending = 0;
#if 0
static const bt_config_t host_config =
{  
	//.host_ctrl_config
	{
		20,//.max_timers
		20,//.max_buff
		50,//.max_message
		10240,//.message_length
		0,//
		256,//host_mem_size
	},
	//.hci_config
		{
			1,//flow_control_support
			3, //flow_control_timeout
			5,  //flow_control_threshold
			6,//max_acl_number
			1021, //max_acl_size  
			10,//max_sco_number
			60, //   max_sco_size
			15360*2  ,//max_acl_queue_size
		},
	//.manager_config

		{
			3,//max_acl_links
			3,//max_device_list
			0,//multi_connect_support
			0x240404,//device_class // RDABT_CODSERVICEBIT_AUDIO|RDABT_CODSERVICEBIT_RENDERING|RDABT_CODMAJORDEVICE_AUDIO|RDABT_CODMINORDEVICE_AUDIO_HEADSETPROFILE
			1,//security_support
			0x04,//sco_packets
			0x8060,//voice_setting
			0,//sco_via_hci
			1,//ssp_support
			3,//io_capability
			30,//sniff_timeout
			0x320,//sniff_interval
		},
	//.l2cap_config
	{
		1,    //auto_config
		1,//fragment_acl_support
		0,   //config_to_core
		0x30,  //max_channel
		0x06, //   max_links    
		1021, //  max_in_mtu
		1021,  //  max_out_mtu
		0,//use_piconet_broadcast
		0x04ED,//max_broadcast_mtu
		0xef1, //broadcast_handle
		0x000000b8//0-------cch modify//RDABT_L2CAP_EXTENDED_FEATURE_FLOWCONTROL_MODE|RDABT_L2CAP_EXTENDED_FEATURE_RETRANSMISSION_MODE,
	},
	//sdp_config
	{
		0x80,//mtu,
	},
	// obex_config
	{   
		0x3800, // local_mtu
              3,      // server_transport
	},
	//.rfcomm_config
	{
		7,//init_credit
		255,//max_credit
		3,//min_credit
		7,//credit_issue
		128, // max_frame_size
	},
	//.hfp_config
	{
		HF_FEATURE_CLIP|HF_FEATURE_REMOTE_VOL,//.hf_feature
		0x1FF,//.ag_feature
		1,// .handle_at_cmd
	},
	//.a2dp_config
	{
		1021,//.media_mtu
		48,//.signal_mtu
		1,// .accetp_connect
	},
	// spp_config
    {
        6, // max_ports
        2, // server_num
        128, // max_data_len
    },      
};
#endif
extern void BT_Callback(rdabt_msg_t *message);
extern void BT_RegisterService(void);
extern void rdabt_handle_app_msg(COS_EVENT *event);

void BT_Register_Modules(void);

/* functions on rdabt driver */
void rdabt_poweron_init(void);
void RDA_bt_Power_Off(void);

void BtRadio_PowerOn(u_int8 polBT, u_int8 numBT);
void BtRadio_PowerOff(u_int8 polBT, u_int8 numBT);

void RDABT_Platform_Init(void);
void rdabt_timer_callback(void *param);
void rdabt_msg_callback(rdabt_msg_t *message);
const BtStackModuleList* tgt_GetBtStackModuleList(void);
const bt_config_t* tgt_GetBtStackConfig(void);

void rdabt_antenna_on()
{
       rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);

	if(USB_HCIt_Init(NULL) != 0)
    {
        rdabt_send_msg_up(EV_BT_READY_IND, 1);
        return;
    }   
    
	RDABT_Add_Transport(TRANSPORT_UART_ID, RDABT_CORE_TYPE_BR_EDR);

	rdabt_send_notify_pending = 0;
}

void rdabt_antenna_off(void)
{
	
    COS_KillTimer(rdabt_p->timer_id);
    rdabt_p->timer_id = 0;
    RDABT_Remove_Transport(TRANSPORT_UART_ID);

   /* reset global context */
   rdabt_p->host_wake_up = FALSE;
   rdabt_p->chip_knocked = FALSE;

   USB_HCIt_Shutdown(NULL);

	rdabt_p->state = RDABT_STATE_POWER_OFF;
}


/*****************************************************************************
* FUNCTION
*   rdabt_timer_callback
* DESCRIPTION
*   General callback function for event scheduler to execute when time out.
* PARAMETERS
*   param IN data from event scheduler
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_timer_callback(void *param)
{
    COS_EVENT ev = {0};
    ev.nEventId = EV_BT_TIMER_EXPIRY_IND;
    COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


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
    //kal_prompt_trace(0,"rdabt_init");

    RDABT_Platform_Init();
	
    /* init stack */
    RDABT_Initialise(tgt_GetBtStackConfig());

    BT_Register_Modules();
    rdabt_p->state = RDABT_STATE_POWER_OFF;
   return TRUE;
}

UINT8 get_rdabt_state()
{
	return rdabt_p->state;
}

void rdabt_main_msg_hdler(COS_EVENT *event)
{
   int status;
     do 
    {
      status=RDABT_Execute(2); 
     } while (0);	 
	rdabt_handle_app_msg(event);
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
    switch(message->msg_id)
    {
        case ADP_DATA_REQ:
        {
            adp_data_msg_t *data_msg = (adp_data_msg_t*)message->data;
            if(data_msg != NULL)
                USB_HCIt_PDU_Tx(NULL, data_msg->buff,data_msg->type,data_msg->flags,data_msg->hci_handle_flags);
        }
        break;
        
        case ADP_TRANSPORT_CNF:
        {
            u_int32 status = ((u_int32)(message->data));
            u_int8 transport_id;
            transport_id = status&0xf;
            status = status >> 8;

            if(transport_id != TRANSPORT_UART_ID || status != RDABT_NOERROR)
            {
                // error
                rdabt_send_msg_up(EV_BT_READY_IND, 1);
            }
            else
            {         
                //MGR_WriteLocalDeviceAddress(local_addr);
                if(0)//g_test_mode
                {
                    struct st_t_dataBuf * cmd = NULL;
                    HCI_Inquiry(&cmd, 0x9e8b33, 10, 0);
                    HCI_SendRawPDU(TRANSPORT_UART_ID, 1, cmd);
                }
                else
                {
                    RDABT_Start_Up(TRANSPORT_UART_ID);
                    //BT_RegisterHeadsetService();
                    //BT_RegisterObexService();
                    //BT_RegisterHIDService();
                    //BT_RegisterSPPService();
                }
            }
            break;
        }
        
        case HOST_INIT_CNF:		
        //case HOST_INIT_REQ:
        if(rdabt_p->original_boot_state)
        {
            t_MGR_HCVersion version;
            //u_int16 num_send;
            COS_EVENT ev = {0};
            ev.nEventId = RDABT_USER_BT_READY_MSG;
            COS_SendEvent(MOD_APP,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

            MGR_GetDeviceVersion(&version);
        }
        break;   
        
        case HOST_SWITCH_CNF:
        {
            COS_EVENT ev = {0};
            ev.nEventId = RDABT_USER_BT_MODE_SWITCH_MSG;
            COS_SendEvent(MOD_APP,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        }
        break;  
        
        case HOST_SHUTDOWN_CNF:
        {
            COS_EVENT ev = {0};
            //RDABT_Terminate(0);
            rdabt_antenna_off();
            //rdabt_send_msg_up(EV_BT_POWEROFF_CNF, NULL);	
            rdabt_send_notify_pending = 0;
            rdabt_send_notify();
            ev.nEventId = RDABT_USER_BT_CLOSE_MSG;
            COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            break;
        }
        
        case ADP_WAKE_IND:
        {
            // wake up bluetooth bt chip
            RDABT_Send_Message(ADP_WAKE_IND, RDABT_ADP, RDABT_HCI, 0, message->data);
        }
        break;
        
        case ADP_SLEEP_IND:
        {
            //app_trace(1, "Recieve ADP_SLEEP_IND, data=%d", message->data);
            if(message->data)  // host stack sleep
            {
                COS_KillTimer(rdabt_p->timer_id);
                rdabt_p->timer_id = 0;
            }
            else
            {
                if(rdabt_p->timer_id == 0)
                    rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);
            }
        }
        break;
        
        default:
            rdabt_msg_callback(message);
        break;
    }
    
    return RDABT_NOERROR;
}

s_int8 rdabt_read_rssi()
{
	return 0;
}

void rdabt_power_on_sched(void)
{
    COS_EVENT ev;
    int status;
    while (1)
    {
         /* proecess external queue */
         if(rdabt_cntx.tx_busy == 0)
        {
            //if(host_cmd_pending == 0)
            {
                 do 
                {
        	      status=RDABT_Execute(2); 
                 } while (0);//status==RDABT_RETRY);	 
                 //hal_HstSendEvent(SYS_EVENT,0x10140003);
				 //hal_HstSendEvent(SYS_EVENT,status);	
				 if(rdabt_p->state == RDABT_STATE_POWER_OFF)
                     break;
            }            
        }
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
        rdabt_main_msg_hdler(&ev);
        //hal_HstSendEvent(SYS_EVENT,0x10140005);

    }
}

void rdabt_power_off_sched(void)
{
    COS_EVENT ev;

   while(1) /* main root for message processing */
   {
        
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
        //hal_HstSendEvent(SYS_EVENT,0x10140001);
      switch(ev.nEventId)
      {
         
         case EV_BT_POWERON_REQ:
         {
            /* ====================== state change ====================== */
            /* send response back to MMI */
            //local_addr = *((t_bdaddr*)(ev.nParam1));
            //g_CalbClock = host_debug_buff.calib_flag;
            rdabt_antenna_on();
            
            rdabt_p->state = RDABT_STATE_POWER_ON;
            rdabt_p->original_boot_state = RDABT_STATE_POWER_ON;

            return;
         }   
		 break;
	     case RDABT_USER_BT_INIT_REQ:
			 rdabt_antenna_on();
		  rdabt_p->state = RDABT_STATE_POWER_ON;
            rdabt_p->original_boot_state = RDABT_STATE_POWER_ON;
			return;
	     break;
      } /* end of switch */
   }  /* end of while(1) */
}

void rdabt_main(void)
{
    memset(rdabt_p,0,sizeof(rdabt_cntx));

   /* main loop of bchs_main for all kinds of states */
   while(1)
   {
      switch(rdabt_p->state)
      {
         case RDABT_STATE_POWER_OFF:
            rdabt_power_off_sched();
            break;
         case RDABT_STATE_POWER_ON:
            rdabt_power_on_sched();
            break;
         default:
            ;//ASSERT(0);
      }
   }
}


TASK_ENTRY BAL_BTTaskEntry (void *pData)
{
#if 0
    COS_EVENT ev;
    static u_int8 bt_active = 0;
    int status;

    while(!bt_active)
    {
    		COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
		if(ev.nEventId == RDABT_USER_BT_INIT_REQ)	
		{
			rdabt_main_msg_hdler(&ev);
			//rdabt_entry();
		}
		
		if(ev.nEventId == EV_BT_POWERON_REQ)
		{
			rdabt_main_msg_hdler(&ev);
			//rdabt_antenna_on();
			bt_active = 1;
			break;
		}
    }
    while(1)
    {
	    COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);	
	    rdabt_main_msg_hdler(&ev);
    }
    //    rdabt_send_msg_to_bt(&ev);
/*
    if(rdabt_get_xcpu_msg(&ev))
    {
        // if med event
        if(0)
            // send to med task
            COS_SendEvent(MOD_MED,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        else
            COS_SendEvent(MOD_APP,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
	*/
#else
        rdabt_init();
        rdabt_main();
#endif		
	return 0;
}


// =============================================================================
// g_tgtBtStackCfg
// -----------------------------------------------------------------------------
/// BT Stack configuration.
// =============================================================================
const bt_config_t g_tgtBtStackCfg = 
	{  																		\
                                                                     \
	{																				\
		10,                                                           \
		20,                                                                 \
		30,                                                           \
		512,                                                          \
		0,                                                          \
		256                                                            \
	},                                                                                              \
	                                                                         \
		{                                                                                       \
			1,                                              \
			1,                                              \
			2,                                            \
			7,                                                    \
			508,                                                       \
			4,                                                    \
			60,                                                         \
			10240,                                            \
		},                                                                                          \
	                                                                         \
		{                                                                                           \
			3,                                                             \
			10,                                                          \
			1,                                                 \
			BT_DEVICE_OF_CLASS,											\
			1,                                                      \
			0x0c,                                                        \
			BT_VOICE_SETTING,                                                \
			0,                                                           \
			0,                                                           \
			3,                                                          \
			100,                                                       \
			0x320,                                                    \
			3,                                                       \
			0,                                                        \
			20000,                                                       \
			1															\
		},                                                                                          \
	                                                                            \
	{                                                                                               \
		1,                                                                   \
		1,                                                      \
		0,                                                                \
		10,                                                              \
		6,                                                                     \
		1021,                                                                  \
		1021,                                                                 \
		0,                                                     \
		0x04ED,                                                \
		0xef1,                                                      \
		0                                                              \
	},                                                                                              \
	                                                                               \
	{                                                                                               \
		0x80,                                                                            \
	},                                                                                                  \
	                                                                          \
	{                                                                                               \
		4096,                                                              \
        3,                                                        \
	},                                                                                              \
	                                                                    \
	{                                                                                           \
		7,                                                                   \
		255,                                                              \
		3,                                                                    \
		7,                                                                  \
		1021                                                       \
	},                                                                                          \
	                                                                          \
	{                                                                                           \
		HF_FEATURE_CLIP|HF_FEATURE_REMOTE_VOL|HF_FEATURE_NREC_FUNCTION, \
		AG_FEATURE_IN_BAND_RING,                          \
		1,                                                          \
		2                                                          \
	},                                                                                          \
	                                                                      \
	{                                                                                           \
		500,                                                           \
		48,                                                           \
		1,                                                        \
		2                                                          \
	},                                                                                          \
	                                                                       \
       {                                                                                        \
        6,                                                                     \
        2,                                                                    \
        1021                                                                 \
       },                                                                                           \
}      ;

// =============================================================================
// g_tgtBtStackModuleList
// -----------------------------------------------------------------------------
/// BT Stack module list.
// =============================================================================

#define MODULE_ITEM(id, fun)      \
{                                                                           \
    id,                                 \
    rdabt_##fun##_layer_ctrl,                 \
    rdabt_##fun##_msg_dispatch,         \
}                                                                         \

const BtStackModuleList g_tgtBtStackModuleList[] = 
{
    {
        RDABT_ADP,
        NULL,
        rdabt_adp_msg_dispatch,
    },   
#if CHIP_HAS_BTCPU==0
    MODULE_ITEM(RDABT_HCI, hci),
#endif    
    MODULE_ITEM(RDABT_MANAGER, manager),
    MODULE_ITEM(RDABT_L2CAP, l2cap),
    MODULE_ITEM(RDABT_SDP_CLI, sdp_cli),
    MODULE_ITEM(RDABT_SDP_SRV, sdp_srv),
    MODULE_ITEM(RDABT_RFCOMM, rfcomm),
#if RDABT_LE_SUPPORT==1
    MODULE_ITEM(RDABT_ATT, att),
#endif
#if APP_SUPPORT_PBAP==1 || APP_SUPPORT_MAP==1    
    MODULE_ITEM(RDABT_OBEX_CLI, OBEX_CLI), 
#endif    
#if APP_SUPPORT_OPP==1 || APP_SUPPORT_MAP==1    
    MODULE_ITEM(RDABT_OBEX_SRV, OBEX_SRV),
#endif
#if APP_SUPPORT_A2DP==1
    MODULE_ITEM(RDABT_A2DP, avdtp),
#endif    
#if APP_SUPPORT_AVRCP==1
	MODULE_ITEM(RDABT_AVRCP, avctp),
#endif
#if APP_SUPPORT_HFP==1   
    MODULE_ITEM(RDABT_HFP, hfp), 
#endif
#if APP_SUPPORT_HID==1
    MODULE_ITEM(RDABT_HID, hid), 
#endif
#if APP_SUPPORT_SPP==1
    MODULE_ITEM(RDABT_SPP, spp), 
#endif    
    {
        0,
        NULL,
        NULL,
    },   
};

// =============================================================================
// tgt_GetBtStackConfig
// -----------------------------------------------------------------------------
/// This function is used by BT Stack to get access to its configuration structure.
/// This is the only way BT Stack can get this information.
// =============================================================================
const bt_config_t* tgt_GetBtStackConfig(void)
{
    return &g_tgtBtStackCfg;
}

// =============================================================================
// tgt_GetBtStackModuleList
// -----------------------------------------------------------------------------
/// This function is used by BT Stack to get access to its configuration structure.
/// This is the only way BT Stack can get this information.
// =============================================================================
const BtStackModuleList* tgt_GetBtStackModuleList(void)
{
    return &g_tgtBtStackModuleList;
}

PUBLIC UINT8 tgt_GetBtMaxDeviceNum(VOID)
{
    return g_tgtBtStackCfg.manager_config.max_device_list;
}

PUBLIC UINT8 tgt_GetBtDefaultMode(VOID)
{
    return g_tgtBtStackCfg.manager_config.default_le_mode;
}

void SYS_SendEvent2(UINT32 event)
{
    hal_HstSendEvent(0,event);
}

