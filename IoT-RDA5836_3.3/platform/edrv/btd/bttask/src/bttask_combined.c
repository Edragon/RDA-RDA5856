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
#include "rdabt_drv.h"

#include "bt.h"
#include "bt_ctrl.h"
#include "bt_config.h"
#include "hfp.h"
#include "hci.h"
#include "papi.h"
#include "rdabt_main.h"
#include "rdabt_hcit.h"
#include "rdabt_uart.h"

#include "rdabt_common.h"
#include "manager.h"
#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "pmd_m.h"
#include "hal_gpio.h"


CONST TGT_BTD_CONFIG_T* g_btdConfig = NULL;
u_int8 bt_pcmin_mode = 0; // for bt_common_cfg.c used

rdabt_context_struct rdabt_cntx = {0};
rdabt_context_struct* rdabt_p = &rdabt_cntx;
extern const unsigned char bt_gpio_setting[16]; 
u_int8  rdabt_send_notify_pending = 0;
extern u_int8  rdabt_send_data_ready;
extern u_int8  rdabt_send_timer_expiry_pending;
extern void* (*TRAhcit_Register)(void);
extern void* TRAhcit_Register_COMBINED(void);

//static u_int8 rdabt_wake_chip_data[] = {0xff};
//static u_int8 warm_reset[] = {0x01,0x00,0xfd,0x00}; // for fpga test
//static u_int8 sniff_window[] = {0x01, 0x43, 0xfc, 0x01, 0x0b};
//BOOL  reset_in_process=0;

/* Set the device Address                       */
//t_bdaddr local_addr;

#define RDABT_WAKE_UP_TIME       					5000      /* host wake up time 2s */

extern int 	bonding_role; 	
extern char *  deviceName;
extern u_int8  nameLen;
extern BOOL g_test_mode;
BOOL  g_CalbClock = 0;
BOOL  IsFlowCtrlStart = 0;/*added by laixf for Lps to remember flow control*/

void BT_Register_Modules(void);

/* functions on rdabt driver */
void rdabt_poweron_init(void);
void RDA_bt_Power_Off(void);

void BtRadio_PowerOn(u_int8 polBT, u_int8 numBT);
void BtRadio_PowerOff(u_int8 polBT, u_int8 numBT);

void RDABT_Platform_Init(void);
void rdabt_timer_callback(void *param);
void rdabt_uart_eint_hdlr(void);
const bt_config_t* tgt_GetBtStackConfig(void);

extern void SYSconfig_Initialise(void);
extern void BT_Initialise(void);
extern void rdabt_poweronphone_init(void);
extern void rdabt_handle_app_msg(COS_EVENT *event);
extern u_int8 BTms_Sched(u_int32 num_interations);
extern void rdabt_send_msg_to_xcpu(COS_EVENT *event);
extern INT16 *MMC_SCOGetRXBuffer(UINT8 flag);
extern VOID MMC_SCOReceiveData(INT32 length);

void rdabt_antenna_on()
{
	//u_int16 num_send;
	//u_int32 ii=0;

#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(FALSE);
#endif
      rdabt_poweron_init();
     //  rdabt_p->timer_id = COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);
	rdabt_cntx.tx_busy = 0;
	rdabt_send_notify_pending = 0;
}

extern INT32 h_Log_File;
void rdabt_antenna_off(void)
{
	
   // COS_EVENT ev;
   //u_int32 msg_count;

   *(volatile UINT32*)(0xa1a14080) = 0;

   RDABT_Remove_Transport(TRANSPORT_UART_ID);

   RDA_bt_Power_Off();

   /* remove all external messages */
//   while(1)
   {      
//      COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
   }
   // COS_KillTimer(rdabt_p->timer_id);
   // rdabt_p->timer_id = 0;

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
    
    //BT_Send_Msg_Up(EV_BT_POWEROFF_CNF, NULL);	
#if 0//pDEBUG
    if(h_Log_File>=0)
    {
        //FS_Close(h_Log_File);
        h_Log_File = -1;
    }
#endif
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
   
    if(rdabt_send_notify_pending < 5)
    {
        rdabt_send_notify_pending ++;
        ev.nEventId = EV_BT_NOTIFY_IND;
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
#ifdef USE_VOC_CVSD
u_int16 sco_packet_length = 60;
#else
u_int16 sco_packet_length = 120;
#endif

u_int8 get_rdabt_state()
{
	return rdabt_p->state;
}


boolean rdabt_init(void)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#if 0//(FPGA_MODE_ENABLE)
    RDA_WRITE_REG(0xa1a0F000, 0x02);
    RDA_WRITE_REG(0xa1a0F018, 0x30003000);
#endif	

    //enable bt clk before BT_Initialise while BT_Initialise need BT IRQ, by yj
    RDA_WRITE_REG(0xa1a00010,RDA_READ_REG(0xa1a00010)| (1<<5)); //	SYS_CTRL_BT_EN;
    hal_SysWriteRFUsingSPI();
    hal_SysPMUIntfXtalCtrl(1);     //BT ADC en
#endif

    /* init stack */
    SYS_SendEvent2(0x2015a00a);
    RDABT_Initialise(tgt_GetBtStackConfig());
    SYS_SendEvent2(0x2015a00b);
    BT_Register_Modules();

    SYSconfig_Initialise();
    TRAhcit_Register =TRAhcit_Register_COMBINED;
    BT_Initialise();
    RDABT_Start_Up(TRANSPORT_UART_ID);
    RDABT_Send_Message(HOST_INIT_REQ, RDABT_HCI, RDABT_HOST_CORE, 0, NULL);
    rdabt_send_notify();
   return TRUE;
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
TASK_ENTRY BAL_BTTaskEntry (void *pData)
{
    COS_EVENT ev;
    //static u_int8 bt_active = 0;
    UINT32 status;
    extern UINT32 test_pka;

    while (1)
    {
        COS_WaitEvent (MOD_BT, &ev, COS_WAIT_FOREVER);

        if (ev.nEventId == EV_BT_NOTIFY_IND)
        {
            status = hal_SysEnterCriticalSection();
            if (rdabt_send_notify_pending)
                rdabt_send_notify_pending--;
            hal_SysExitCriticalSection(status);
        }
		
        {
            if (ev.nEventId == RDABT_USER_BT_INIT_REQ)
            {
                //rdabt_main_init();
                g_btdConfig = tgt_GetBtdConfig ();

                rdabt_poweronphone_init ();
                rdabt_antenna_on ();
                hal_HstSendEvent (SYS_EVENT, 0x20152000);
                rdabt_init ();
	         rdabt_p->state = RDABT_STATE_POWER_ON;
            }
            if (rdabt_p->state == RDABT_STATE_POWER_ON)
            {
                rdabt_handle_app_msg (&ev);
                do
                {
                    status = RDABT_Execute(1);
                    status |= BTms_Sched (1);
                    status |= RDABT_Execute(1);
                } while(0);
                if(status>1 && rdabt_send_notify_pending<3)
                {
                    status = hal_SysEnterCriticalSection();
                    rdabt_send_notify_pending ++;
                    hal_SysExitCriticalSection(status);
                    ev.nEventId = EV_BT_NOTIFY_IND;
                    COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
                }
                //rdabt_test_temp ();
            	test_pka++;
            	if(test_pka>100000)
            	    test_pka =1;
            }
        }
    }
}

t_api rdabt_adp_msg_dispatch(rdabt_msg_t *message)
{
        //pDebugPrintfEX((pLOGNOTICE,pLOGADP,"rdabt_adp_msg_dispatch message:0x%x", message));
        switch(message->msg_id)
        {
                case HOST_INIT_CNF:
                {
                    COS_EVENT ev = {0};
                    //app_trace(1, "HOST_INIT_CNF");
                    
                    //t_MGR_HCVersion version;
                    //u_int16 num_send;
#ifdef BT_UART_BREAK_INT_WAKEUP
                    //u_int32 value = 0x00007fff; // set hostwake  gpio to	input
                    //MGR_WritePHYRegister(0, 0x40200010, 1, &value);
#endif
                    //rdabt_active_init();
                    //rdabt_active_success();
                    rdabt_p->state = RDABT_STATE_POWER_ON;
			ev.nEventId = RDABT_USER_BT_READY_MSG;
			rdabt_send_msg_to_xcpu(&ev);
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
                    RDABT_Terminate(0);
                    rdabt_antenna_off();
                    rdabt_send_notify_pending = 0;
                    rdabt_send_notify();
                    ev.nEventId = RDABT_USER_BT_CLOSE_MSG;
                    rdabt_send_msg_to_xcpu(&ev);
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
                break;
            case MGR_RSSI_IND:
                break;
            case MGR_SCO_DATA_IND:
                {
                    hci_sco_data_msg_t *packet = (hci_sco_data_msg_t*)message->data;
                    pMemcpy((void*)MMC_SCOGetRXBuffer(packet->flag), packet->data, packet->length);
                    MMC_SCOReceiveData(packet->length);
                }
                break;
            default:
                rdabt_msg_callback(message);
                break;
        }
        return RDABT_NOERROR;        
}


