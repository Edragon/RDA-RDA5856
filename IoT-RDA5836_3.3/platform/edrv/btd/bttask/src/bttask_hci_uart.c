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

/***************************************************************************** 
* Include
*****************************************************************************/
#include "tgt_app_cfg.h"
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
#include "rdabt_drv.h"
#include "rdabt_common.h"
#include "manager.h"
#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "pmd_m.h"
#include "hal_gpio.h"
#include "rdabt_hcit.h"
#include "rdabt_uart.h"
#include "hci.h"
#if BLUETOOTH_USE_MARLIN==1
#include "gpio_adp.h"
#endif

CONST TGT_BTD_CONFIG_T* g_btdConfig = NULL;
u_int8 bt_pcmin_mode = 0; // for bt_common_cfg.c used

rdabt_context_struct rdabt_cntx = {0};
rdabt_context_struct* rdabt_p = &rdabt_cntx;
extern const unsigned char bt_gpio_setting[16]; 
u_int8  rdabt_send_notify_pending = 0;
u_int8  rdabt_send_data_ready = 0;
u_int8  rdabt_send_data_ready2 = 0;
extern u_int8  rdabt_send_timer_expiry_pending;

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


static u_int8 host_cmd_pending = 0;
static u_int32 host_cmd_time = 0;

static u_int8 rdabt_wakeup_data[] = {0x01,0xc0,0xfc,0x00};
static u_int8 rdabt_change_baudrate[] = 
	#if 0
	{0x01,0x02,0xfd,0x0a,0x00,0x01,0x60,0x00,0x00,0x80,0x00,0x10,0x0e,0x00,
	0x01,0x02 ,0xfd ,0x0a ,0x00 ,0x01 ,0x40 ,0x00 ,0x00 ,0x80 ,0x00 ,0x01 ,0x00 ,0x00};
	#else
{0x01, 0x34, 0xfc, 0x04, 0x00, 0x10, 0x0e, 0x00};//{0x01, 0x34, 0xfc, 0x04, 0x00, 0x10, 0x0e, 0x00};//{0x01,0x77,0xfc,0x01,0x00};
#endif

// host debug command/result
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


static u_int8 rdabt_wake_chip_data[] = {0xff};
static u_int8 warm_reset[] = {0x01,0x00,0xfd,0x00}; // for fpga test
static u_int8 sniff_window[] = {0x01, 0x43, 0xfc, 0x01, 0x0b};
BOOL  reset_in_process=0;

/* Set the device Address                       */
//t_bdaddr local_addr;

#define RDABT_WAKE_UP_TIME       					5000      /* host wake up time 2s */
extern int 	bonding_role; 	
extern char *  deviceName;
extern u_int8  nameLen;
extern BOOL g_test_mode;
BOOL  g_CalbClock = 0;
BOOL  IsFlowCtrlStart = 0;/*added by laixf for Lps to remember flow control*/
VOLATILE host_debug_t HAL_UNCACHED_BSS host_debug_buff;

/* functions on rdabt driver */
void rdabt_poweron_init(void);
void RDA_bt_Power_Off(void);

void BtRadio_PowerOn(u_int8 polBT, u_int8 numBT);
void BtRadio_PowerOff(u_int8 polBT, u_int8 numBT);

void RDABT_Platform_Init(void);
void rdabt_timer_callback(void *param);
void rdabt_uart_eint_hdlr(void);

#if BLUETOOTH_USE_MARLIN==1
void rdabt_marlin_bt_init(void)
{
    struct st_t_dataBuf * cmd = NULL;
    u_int8 result=0;

       t_MarlinPskey marlin_pskey={
        .dbg_source_sink_syn_test_data = 0,
        .sys_sleep_in_standby_supported = 1,
        .sys_sleep_in_master_supported = 1,
        .sys_sleep_slave_supported = 1,

        .default_ahb_clk = 26000000,
        .device_class = 0x001F00,
        .win_ext = 30,

        .aGainValue = {0x0000F600, 0x0000D000, 0x0000AA00, 0x00008400, 0x00004400, 0x00000A00},
        .aPowerValue = {0x0FC80000, 0x0FF80000, 0x0FDA0000, 0x0FCC0000, 0x0FFC0000},

        .feature_set = {0xFF, 0xFF, 0x8D, 0xFE, 0xDB, 0x7D, 0x7B, 0x83, 0xFF, 0xA7, 0xFF, 0x7F, 0x00, 0xE0, 0xF7, 0x3E},  //0xFF, 0xFF, 0x8D, 0xFE-->FA, disable 3M
        .device_addr = {0x00, 0x15, 0x83, 0x4c, 0x4a, 0x7E},

        .sys_sco_transmit_mode = 0, //true tramsmit by uart, otherwise by share memory
        .sys_uart0_communication_supported = 1, //true use uart0, otherwise use uart1 for debug
        .edr_tx_edr_delay = 5,
        .edr_rx_edr_delay = 14,

        .wbs_nv_117 = 0x007c,

        .is_wdg_supported = 0,

        .share_memo_rx_base_addr = 0,
        .wbs_nv_118 = 0x0066,
        .nbv_nv_117 = 0x10F9,

        .share_memo_tx_packet_num_addr = 1,
        .share_memo_tx_data_base_addr = 0,

        .PrintLevel = 0xFFFFFFFF,

        .share_memo_tx_block_length = 0,
        .share_memo_rx_block_length = 0,
        .share_memo_tx_water_mark = 0,

        .nbv_nv_118 = 0x0A45,

        .uart_rx_watermark = 48,
        .uart_flow_control_thld = 0x3F,
        .comp_id = 0x01EC,
        .pcm_clk_divd = 0x26,

        .reserved = {0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0 },
    };

    result=HCI_Write_Marlin_Pskey(&cmd, &marlin_pskey);

    if(result==RDABT_NOERROR)
        result=HCI_SendRawPDU(TRANSPORT_UART2_ID, 1, cmd);
    
    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI_Write_Marlin_Pskey result=%d",result));

}


void rdabt_marlin_bt_wakeup(void)
{
#if 0

    SCI_GPIO_CONFIG config = {GPIO_INT_EDGES_RISING, GPIO_OUTPUT_PIN};

    SCI_GPIOInitilize(MARLIN_GPIO_2, &config);
    SCI_Sleep(100);

    SCI_GPIOWrite(MARLIN_GPIO_2, 1u);
    SCI_Sleep(20);
    SCI_GPIODeInit(MARLIN_GPIO_2);
#else
    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"rdabt_marlin_bt_wakeup_req!!! "));

    hal_GpioSet(g_btdConfig->pinWakeUp);
    COS_Sleep(18);
    hal_GpioClr(g_btdConfig->pinWakeUp);
#endif
}
#endif


void rdabt_antenna_on()
{
	u_int16 num_send;
	u_int32 ii=0;

#if BLUETOOTH_USE_MARLIN==1
        SCI_GPIO_CONFIG config = {GPIO_INT_EDGES_RISING, GPIO_OUTPUT_PIN};
        u_int8 uart_port=BT_UART_PORT2;
#else
        u_int8 uart_port=BT_UART_PORT;
#endif 

#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(FALSE);
#endif

#ifdef BT_SHAREMEM_TRANSPORT
	RDABT_Tranport_Init();
    rdabt_poweron_init();
    COS_SetTimer(10, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);//uart
    return;
#else //BT_SHAREMEM_TRANSPORT

#if BLUETOOTH_USE_MARLIN==1
    SCI_GPIOInitilize(g_btdConfig->pinWakeUp.gpioId/*MARLIN_GPIO_1*/, &config);
    SCI_Sleep(100);
    rdabt_marlin_bt_wakeup();
    
    rdabt_adp_uart_start(uart_port);

    //I2C_Open();
    rdabt_adp_uart_configure(uart_port, 3250000 /*921600*/, TRUE);
	rdabt_uart_register_cb(uart_port);
#else
    rdabt_adp_uart_start(BT_UART_PORT);
    //I2C_Open();
    rdabt_adp_uart_configure(BT_UART_PORT, 115200, FALSE);
#endif	
    //rdabt_uart_host_wake_up();
#ifndef BT_UART_BREAK_INT_WAKEUP
    GPIO_INT_Registration(g_btdConfig->pinSleep,0,rdabt_uart_eint_hdlr);
#endif

#if BLUETOOTH_USE_MARLIN == 0
    rdabt_poweron_init();
    rdabt_uart_register_cb(uart_port);
    rdabt_baudrate_ctrl();
#endif	
    RDABT_Tranport_Init();
#endif  // BT_SHAREMEM_TRANSPORT

       rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);

	//RDABT_Tranport_Tx_Data(rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
        if(RDABT_PENDING != RDABT_Add_Transport(uart_port, RDABT_CORE_TYPE_BR_EDR))
    {
        hal_HstSendEvent(SYS_EVENT, 0x13062508);
        //BT_Send_Msg_Up(EV_BT_READY_IND, 1);
        rdabt_antenna_off();
        return;
    }   
    //I2C_Close();
    // Disable 26M AUX clock

//	L1SM_SleepDisable(rdabt_cntx.l1_handle);
    rdabt_cntx.tx_busy = 0;
	rdabt_send_notify_pending = 0;
}

extern INT32 h_Log_File;
void rdabt_antenna_off(void)
{
	
    COS_EVENT ev;
   u_int32 msg_count;

#if BLUETOOTH_USE_MARLIN==1
    RDABT_Remove_Transport(TRANSPORT_UART2_ID);
	SCI_GPIODeInit(MARLIN_GPIO_1);
#else
   RDABT_Remove_Transport(TRANSPORT_UART_ID);
#endif

   RDA_bt_Power_Off();

   /* remove all external messages */
//   while(1)
   {      
//      COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
   }
    //COS_KillTimer(rdabt_p->timer_id);
    rdabt_p->timer_id = 0;

   /* reset global context */
//   L1SM_SleepEnable(rdabt_p->l1_handle);
   rdabt_p->host_wake_up = FALSE;
   rdabt_p->chip_knocked = FALSE;

   //RDABT_Tranport_Tx_Data(warm_reset, sizeof(warm_reset));
   //RDABT_Tranport_Shutdown();

	rdabt_p->state = RDABT_STATE_POWER_OFF;
    //rdabt_adp_uart_stop();
    
#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(TRUE);
#endif
}

t_api rdabt_adp_msg_dispatch(rdabt_msg_t *message)
{
	COS_EVENT ev = {0};
	//hal_HstSendEvent(SYS_EVENT, 0x20151044);
	//hal_HstSendEvent(SYS_EVENT, message->msg_id);
        switch(message->msg_id)
        {
            case ADP_DATA_REQ:
                {
                    adp_data_msg_t *data_msg = (adp_data_msg_t*)message->data;
                    //COS_Sleep(10);
                    RDABT_Tranport_Tx_PDU(data_msg->buff,data_msg->type,data_msg->flags,data_msg->hci_handle_flags);
                    host_cmd_time = hal_TimGetUpTime();
                }
                break;
            case ADP_TRANSPORT_CNF:
                {
                    u_int32 status = ((u_int32)(message->data));
                    u_int8 transport_id;
                    transport_id = status&0xf;
                    status = status >> 8;
			hal_HstSendEvent(SYS_EVENT, 0x20151043);
			hal_HstSendEvent(SYS_EVENT, transport_id);
			hal_HstSendEvent(SYS_EVENT, status);
			hal_HstSendEvent(SYS_EVENT, g_test_mode);
                    //RDAbt_enable_dut_write();
                    //break;

                    if((transport_id != TRANSPORT_UART_ID && transport_id != TRANSPORT_UART2_ID) || status != RDABT_NOERROR)
                     {
                        // error
                        ev.nEventId = EV_BT_READY_IND;
			   ev.nParam1 = 1;
                        //BT_Send_Msg_Up(EV_BT_READY_IND, 1);
			COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
			hal_HstSendEvent(SYS_EVENT, 0x1306250a);
                        rdabt_antenna_off();
                    }
        			else if(g_test_mode)
                    {
                        struct st_t_dataBuf * cmd = NULL;
                        if(g_CalbClock)
                        {
                            // notify host to start clock calib
                            hal_HstSendEvent(BOOT_EVENT, 0x7e57ca1b); 
                        }
                        else
                        {
                            HCI_Inquiry(&cmd, 0x9e8b33, 10, 0);
                            HCI_SendRawPDU(TRANSPORT_UART_ID, 1, cmd);
                        }
                    }
                    else
                    {
//#ifndef FCC_TEST_OPEN                   
                    
                        RDABT_Start_Up(transport_id);
						//BT_RegisterHeadsetService();
						//BT_RegisterObexService();
						//BT_RegisterHIDService();
						//BT_RegisterSPPService();
//#endif 
                    }
                }
                break;
				
#if BLUETOOTH_USE_MARLIN==1
                case ADP_RESET_CNF:
                {
                     struct st_t_dataBuf * cmd = NULL;
                     u_int8 status=0;
                         
                    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"ADP_RESET_CNF!!!! "));    
                    rdabt_marlin_bt_wakeup();
                    rdabt_marlin_bt_init();

                    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"ADP_RESET_CNF status=%d ",status));   
                }
                break;
#endif
                
            case HOST_INIT_CNF:
               if(rdabt_p->original_boot_state)
               {
                    t_MGR_HCVersion version;
                    u_int16 num_send;
			COS_EVENT ev = {0};		
#ifdef BT_UART_BREAK_INT_WAKEUP
                    u_int32 value = 0x00007fff; // set hostwake  gpio to	input
                    //MGR_WritePHYRegister(0, 0x40200010, 1, &value);
#endif
                     /* send to MMI */
                    //BT_Send_Msg_Up(EV_BT_READY_IND, NULL);
                    hal_HstSendEvent(SYS_EVENT, 0x1306250b);
                    // MGR_ChangeLocalName("RDA MP3 BT");
                    MGR_GetDeviceVersion(&version);
#if BLUETOOTH_USE_MARLIN==1
					RDABT_Tranport_Tx_Data(BT_UART_PORT2, sniff_window, sizeof(sniff_window));	
#else
					RDABT_Tranport_Tx_Data(BT_UART_PORT,  sniff_window, sizeof(sniff_window));
#endif
                    //rdabt_active_init();
                    //rdabt_active_success();
			ev.nEventId = RDABT_USER_BT_READY_MSG;
			rdabt_send_msg_to_xcpu(&ev);		
               }
                break;
            case HOST_SHUTDOWN_CNF:
                {
                    //RDABT_Terminate(0);
                    rdabt_antenna_off();
                    //BT_Send_Msg_Up(EV_BT_POWEROFF_CNF, NULL);	
			rdabt_send_notify_pending = 0;
			rdabt_send_notify();
			break;
                }
            case ADP_WAKE_IND: // wake up bluetooth bt chip
                {
			//L1SM_SleepDisable(rdabt_cntx.l1_handle);
                    //rdabt_hcit_sharemem_tx_data(rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                    //RDABT_Send_Message(ADP_WAKE_IND, RDABT_ADP, RDABT_HCI, 0, message->data);
                    hal_HstSendEvent(SYS_EVENT, 0x20161018);


#if BLUETOOTH_USE_MARLIN==1
                    rdabt_marlin_bt_wakeup();
                    //RDABT_Send_Message(ADP_WAKE_IND, RDABT_ADP, RDABT_HCI, 0, message->data);
                    //RDABT_Tranport_Tx_Data(BT_UART_PORT2, rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                    hal_UartAllowSleep(TRANSPORT_UART2_ID,FALSE);
#else
                    RDABT_Tranport_Tx_Data(BT_UART_PORT, rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                    hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
#endif
                }
                break;
            case ADP_SLEEP_IND:
                {
                    hal_HstSendEvent(SYS_EVENT, 0x88889999);    
                    app_trace(1, "Recieve ADP_SLEEP_IND, data=%d", message->data);
                    if(message->data)  // host stack sleep
                    {
                        COS_KillTimer(rdabt_p->timer_id);
                        rdabt_p->timer_id = 0;
#if BLUETOOTH_USE_MARLIN==1
                        hal_GpioClr(g_btdConfig->pinWakeUp);
#endif
                    }
                    else
                    {
                        if(rdabt_p->timer_id == 0)
                            rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);
                    }
                }
                break;
            case MGR_RSSI_IND:
                host_debug_buff.data.data = (u_int32)(message->data);
                host_debug_buff.result = HOST_RESULT_DATA;
                break;
            case MGR_READ_REGISTER_CNF:
                host_debug_buff.data.data = (u_int32)(message->data);
                host_debug_buff.result = HOST_RESULT_DATA;
                break;
            default:
                rdabt_msg_callback(message);
                break;
        }
    return RDABT_NOERROR;
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
    UINT32 status = hal_SysEnterCriticalSection();
	
#ifdef BT_SHAREMEM_TRANSPORT    
	rdabt_hcit_sharemem_rx_data();  //uart
#endif
   
    if(rdabt_send_timer_expiry_pending == 0)
    {
        rdabt_send_timer_expiry_pending = 1;
        ev.nEventId = EV_BT_TIMER_EXPIRY_IND;
        COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
    hal_SysExitCriticalSection(status);

}

#ifdef FCC_TEST_OPEN
extern void rda_bt_test_enable_dut(void);
extern void Rdabt_manual_tx_test(UINT32 type,UINT32 package,UINT32 freq,UINT32 pwrLevel,UINT32 msgPattern);

UINT32* BT_Test_Mast_Ptr = 0;
UINT32 volatile BT_Test_Mask=0x1;
static uint32 pre_BT_Test_Mask=0;

UINT32 type, package, freq, pwrLevel, msgPattern;
UINT32 bt_test_timer_id = 0;
UINT32 volatile mask_tmp = 1;
#endif


u_int8 get_rdabt_state()
{
	return rdabt_p->state;
}

boolean rdabt_init(void)
{
    //EXPORT HAL_HOST_CONFIG_t g_halHostConfig;
    
    //g_halHostConfig.bt_debug = &host_debug_buff;
	
    /* init stack */
    RDABT_Initialise(tgt_GetBtStackConfig());

    BT_Register_Modules();

   rdabt_p->state = RDABT_STATE_POWER_OFF;

   return TRUE;
}

void rdabt_power_off_sched(void)
{
    COS_EVENT ev;

   while(1) /* main root for message processing */
   {
        if(host_debug_buff.result == HOST_RESULT_PENDING)
            host_debug_buff.result = HOST_RESULT_CLOSED;
        
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
        //hal_HstSendEvent(SYS_EVENT,0x10140001);
	// hal_HstSendEvent(SYS_EVENT,ev.nEventId);	
      switch(ev.nEventId)
      {
         case RDABT_USER_BT_INIT_REQ:
         {
            /* ====================== state change ====================== */
            /* send response back to MMI */
            //local_addr = *((t_bdaddr*)(ev.nParam1));
            g_CalbClock = host_debug_buff.calib_flag;
            rdabt_antenna_on();
            //rdabt_common_init(&ev);
            rdabt_p->state = RDABT_STATE_POWER_ON;
            rdabt_p->original_boot_state = RDABT_STATE_POWER_ON;
		hal_HstSendEvent(SYS_EVENT, 0x20151041);
            return;
         }   
      } /* end of switch */
   }  /* end of while(1) */
}

void rdabt_power_on_sched(void)
{
    COS_EVENT ev;
   int status;
#if BLUETOOTH_USE_MARLIN==1
   u_int8 port=BT_UART_PORT2;
#else
    u_int8 port=BT_UART_PORT;
#endif

    while (1)
    {
         /* proecess external queue */
         if(rdabt_cntx.tx_busy == 0)
        {
            if(host_cmd_pending == 0)
            {
                 do 
                {
        	      status=RDABT_Execute(2); 
                 } while(0); //(status==RDABT_RETRY);	 
                 //hal_HstSendEvent(SYS_EVENT,0x10140003);
                 if(rdabt_p->state == RDABT_STATE_POWER_OFF)
                     break;
		   if(status == RDABT_RETRY)
		   {
		   	rdabt_send_notify();
		   }
            }

            if(host_debug_buff.result == HOST_RESULT_PENDING)
            {
#if 0
                if(host_debug_buff.calib_flag == 2) // calib end
                {
                    // write calib to flash
                    UINT32 system_flag;
                    UINT32 s;
                    EXPORT HAL_HOST_CONFIG_t g_halHostConfig;
                    EXPORT INT32 g_CDAC;
                    UINT32 csStatus = hal_SysEnterCriticalSection();

                    system_flag = *(((U32 *)(&_flash_start))+1);
                    system_flag = system_flag&~4;
                    memd_FlashWrite((UINT8*)0x04,sizeof(UINT32), &s,(CHAR*)(&system_flag));
                    system_flag = (g_halHostConfig.spi_data>>10) - g_CDAC;
                    memd_FlashWrite((UINT8*)0x08,sizeof(UINT32), &s,(CHAR*)(&system_flag));
                    
                    host_debug_buff.result = HOST_RESULT_OK;
                    hal_SysExitCriticalSection(csStatus);
                    //BT_Send_Msg_Up(EV_BT_READY_IND, 0);
                }
                else
#endif
                {
                    if(hal_TimGetUpTime() > host_cmd_time+ 1 SECOND)
                    {
                        RDABT_Tranport_Tx_Data(port, rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
                        COS_Sleep(500);
                    }
                    hal_HstSendEvent(SYS_EVENT, 0x14012601);
                    hal_HstSendEvent(SYS_EVENT, host_debug_buff.length);
                    RDABT_Tranport_Tx_Data(port, host_debug_buff.data.buff,host_debug_buff.length);
                    host_cmd_time = hal_TimGetUpTime();
                    host_debug_buff.result = HOST_RESULT_OK;
                    if(host_debug_buff.cmd == HOST_CMD_END)
                        host_cmd_pending = 0;
                    else
                        host_cmd_pending = 1;
                }
            }
        }
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
        rdabt_handle_app_msg(&ev);
//        hal_HstSendEvent(SYS_EVENT,0x10140005);

    }
}

void rdabt_main(void)
{
    memset(rdabt_p,0,sizeof(rdabt_cntx));

    g_btdConfig = tgt_GetBtdConfig();

	if (g_btdConfig->pinReset.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetOut(g_btdConfig->pinReset.gpioId);
	}
	if (g_btdConfig->pinWakeUp.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetOut(g_btdConfig->pinWakeUp.gpioId);
	}
	if (g_btdConfig->pinSleep.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetIn(g_btdConfig->pinSleep.gpioId);
	}

 	rdabt_poweronphone_init();
	hal_HstSendEvent(SYS_EVENT, 0x20151040);
	hal_HstSendEvent(SYS_EVENT, rdabt_p->state);
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

/*****************************************************************************
* FUNCTION
*   bt_create
* DESCRIPTION
*   This function creats bt task
* PARAMETERS
*   handle  OUT   pointer to the table of bt task structure.
* RETURNS
*   Always TRUE since the creation shall not be failed.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
extern void SYS_Main(void);
TASK_ENTRY BAL_BTTaskEntry (void *pData)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
        SYS_Main();
#else
        rdabt_init();
        rdabt_main();
#endif
}


/*****************************************************************************
* FUNCTION
*   rdabt_host_wake_up_timeout
* DESCRIPTION
*   Timeout function for host wake up -- enable sleep mode again
* PARAMETERS
*   mi   IN  no use
*   mv  IN  no use
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_host_wake_up_timeout(void *dummy)
{
   if(rdabt_cntx.chip_knocked == FALSE)
   {
      /* disable sleep mode */
//      L1SM_SleepEnable(rdabt_cntx.l1_handle);
#if BLUETOOTH_USE_MARLIN==1
    hal_UartAllowSleep(TRANSPORT_UART2_ID,TRUE);
#else
      hal_UartAllowSleep(TRANSPORT_UART_ID,TRUE);
#endif
      //hal_BtSleep();
      rdabt_cntx.host_wake_up = FALSE;
   }
   else
   {
      rdabt_cntx.chip_knocked = FALSE;
      COS_SetTimer(RDABT_WAKE_UP_TIME, rdabt_host_wake_up_timeout, 0, COS_TIMER_MODE_SINGLE);
   }
    rdabt_send_notify();
}

/*****************************************************************************
* FUNCTION
*   rdabt_uart_host_wake_up
* DESCRIPTION
*   Handle of host wake up routine
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_uart_host_wake_up(void)
{
   if(!rdabt_cntx.host_wake_up)
   {
      /* disable sleep mode */
//      L1SM_SleepDisable(rdabt_cntx.l1_handle);
#if BLUETOOTH_USE_MARLIN==1
    hal_UartAllowSleep(TRANSPORT_UART2_ID,FALSE);
#else
      hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
#endif
      //hal_BtWakeUp();
      rdabt_cntx.host_wake_up = TRUE;
      COS_SetTimer(RDABT_WAKE_UP_TIME, rdabt_host_wake_up_timeout, 0, COS_TIMER_MODE_SINGLE);
   }
   else
   {
      rdabt_cntx.chip_knocked = TRUE;
   }
}

/*****************************************************************************
* FUNCTION
*   rdabt_uart_unsleep
* DESCRIPTION
*   Disable UART sleep mode
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_uart_unsleep(void)
{
   if(!rdabt_cntx.host_wake_up)
   {
      /* disable sleep mode */
//      L1SM_SleepDisable(rdabt_cntx.l1_handle);
#if BLUETOOTH_USE_MARLIN==1
    hal_UartAllowSleep(TRANSPORT_UART2_ID,FALSE);
#else
      hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
#endif
      //hal_BtWakeUp();
      rdabt_cntx.host_wake_up = TRUE;
      COS_SetTimer(RDABT_WAKE_UP_TIME, rdabt_host_wake_up_timeout, 0, COS_TIMER_MODE_SINGLE);
   }
   else
    {
        rdabt_cntx.chip_knocked = TRUE;
   }
}

void rdabt_uart_eint_hdlr(void)
{
    COS_EVENT event = {0};
    event.nEventId = EV_BT_WAKE_UP_REQ;
    hal_HstSendEvent(SYS_EVENT,0x77778888);
#if BLUETOOTH_USE_MARLIN==1
    hal_UartAllowSleep(TRANSPORT_UART2_ID,FALSE);
#else
    hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
#endif
    COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}



