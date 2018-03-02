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
#include "rdabt_drv.h"
#include "hci.h"


CONST TGT_BTD_CONFIG_T* g_btdConfig = NULL;
u_int8 bt_pcmin_mode = 0; // for bt_common_cfg.c used

rdabt_context_struct rdabt_cntx = {0};
rdabt_context_struct* rdabt_p = &rdabt_cntx;
extern const unsigned char bt_gpio_setting[16]; 
extern u_int8  rdabt_send_notify_pending;
extern u_int8  rdabt_send_data_ready;
extern u_int8  rdabt_send_timer_expiry_pending;

//static u_int8 host_cmd_pending = 0;
static u_int32 host_cmd_time = 0;

//static u_int8 rdabt_wake_chip_data[] = {0xff};
//static u_int8 warm_reset[] = {0x01,0x00,0xfd,0x00}; // for fpga test
//static u_int8 sniff_window[] = {0x01, 0x43, 0xfc, 0x01, 0x0b};
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

void BT_Register_Modules(void);

/* functions on rdabt driver */
void rdabt_poweron_init(void);
void RDA_bt_Power_Off(void);

void BtRadio_PowerOn(u_int8 polBT, u_int8 numBT);
void BtRadio_PowerOff(u_int8 polBT, u_int8 numBT);

void RDABT_Platform_Init(void);
void rdabt_timer_callback(void *param);
void rdabt_uart_eint_hdlr(void);

/************************************************************************************/
extern void rdabt_poweronphone_init(void);
extern void rdabt_send_msg_to_bt(COS_EVENT *event);
extern BOOL rdabt_get_xcpu_msg(COS_EVENT *event);
extern INT16 *MMC_SCOGetRXBuffer(UINT8 flag);
#if APP_SUPPORT_TEST_BOX==1
extern VOID MMC_SCOSendData(INT16 *pdu, UINT16 length);
#else
extern VOID MMC_SCOReceiveData(INT32 length);
#endif

/************************************************************************************/

void rdabt_antenna_on()
{
#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(FALSE);
#endif
#if COMBINED_HOST==2
    //rdabt_sharemem_init();
    rdabt_poweron_init();
       rdabt_p->timer_id = 0; // COS_SetTimer(100, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);
#else
#ifdef BT_SHAREMEM_TRANSPORT
	RDABT_Tranport_Init();
    rdabt_poweron_init();
    COS_SetTimer(10, rdabt_timer_callback, NULL, COS_TIMER_MODE_PERIODIC);//uart
    return;
#endif	
#endif
	rdabt_cntx.tx_busy = 0;
	rdabt_send_notify_pending = 0;
}

extern INT32 h_Log_File;
void rdabt_antenna_off(void)
{
   RDABT_Remove_Transport(TRANSPORT_UART_ID);

   RDA_bt_Power_Off();

   /* remove all external messages */
//   while(1)
   {      
//      COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
   }
    COS_KillTimer(rdabt_p->timer_id);
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
   
    if(rdabt_send_notify_pending == 0)
    {
        rdabt_send_notify_pending = 1;
        ev.nEventId = EV_BT_NOTIFY_IND;
        COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
    hal_SysExitCriticalSection(status);

}

void rdabt_main_msg_hdler(COS_EVENT *event)
{
    switch(event->nEventId)
    {
        case EV_BT_TIMER_EXPIRY_IND:   /* timer routine */
        {
            rdabt_send_timer_expiry_pending = 0;
      		//RDABT_Send_Message(ADP_TIMER_IND, RDABT_ADP, RDABT_HOST_CORE, 0, NULL);
            break;
        }
        case EV_BT_NOTIFY_IND:
		if(rdabt_send_notify_pending)
			rdabt_send_notify_pending--;
            break;
        
        case EV_BT_READY_TO_READ_IND:
            {
                //UINT32 status;
                //hal_HstSendEvent(SYS_EVENT,0x04070120);
                //status= hal_SysEnterCriticalSection();
                if(rdabt_send_data_ready)
                    rdabt_send_data_ready--;
                //rdabt_adp_uart_rx();
                host_cmd_time = hal_TimGetUpTime();
                //hal_SysExitCriticalSection(status);
           }
        break;
        
        
        case EV_BT_WAKE_UP_REQ:
        {
            //RDABT_Tranport_Tx_Data(rdabt_wake_chip_data,sizeof(rdabt_wake_chip_data));
            break;
        }
        
        case EV_BT_POWEROFF_REQ:
            RDABT_Shut_Down();
        break;
        
        default:
            break;				
    }
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
u_int16 sco_packet_length = 30;
#else
u_int16 sco_packet_length = 60;
#endif

u_int8 get_rdabt_state()
{
	return rdabt_p->state;
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
#ifdef BT_SHAREMEM_TRANSPORT
    int status=0;
#endif

    g_btdConfig = tgt_GetBtdConfig();

#ifdef BT_SHAREMEM_TRANSPORT

    rdabt_antenna_on(); //uart
    rdabt_hcit_sharemem_init();
    while(1)
    {
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);
        status = hal_SysEnterCriticalSection();
        rdabt_hcit_sharemem_rx_data();
        hal_SysExitCriticalSection(status);
    }

#else
    while(1)
    {
        COS_WaitEvent(MOD_BT, &ev, COS_WAIT_FOREVER);	
        //hal_HstSendEvent(SYS_EVENT, 0x20151fff);
        //hal_HstSendEvent(SYS_EVENT, ev.nEventId);
        if(ev.nEventId != EV_BT_NOTIFY_IND)
        {
            //hal_HstSendEvent(SYS_EVENT, 0x20151000);
            //rdabt_get_xcpu_msg(&ev);	
            //hal_HstSendEvent(SYS_EVENT, 0x20151001);
            //hal_HstSendEvent(SYS_EVENT, ev.nEventId);
        }

        if(ev.nEventId == EV_BT_NOTIFY_IND)
        {
            if(rdabt_send_notify_pending)
                rdabt_send_notify_pending--;
        }
        else
        {
            if(ev.nEventId == RDABT_USER_BT_INIT_REQ)	
            {
                //rdabt_main_init();
                rdabt_poweronphone_init();
                rdabt_antenna_on();
                rdabt_p->state = RDABT_STATE_POWER_ON;
                hal_HstSendEvent(SYS_EVENT, 0x20152000);
            }
            
            if(rdabt_p->state == RDABT_STATE_POWER_ON)
                rdabt_send_msg_to_bt(&ev);
        }
        while(rdabt_get_xcpu_msg(&ev))
        {
            /*
            if(ev.nEventId == RDABT_A2DP_MEDIA_DATA_IND_MSG)
            {
            ev.nEventId = 3;
            // send to med task
            COS_SendEvent(MOD_MED,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }
            else */if(ev.nEventId == RDABT_SCO_DATA_IND)
            {
                hci_sco_data_msg_t *packet = (hci_sco_data_msg_t*)ev.nParam1;
#if APP_SUPPORT_TEST_BOX==1
                //hal_HstSendEvent(0xff, 0x20160300);
                //hal_HstSendEvent(0xff, ev.nParam1);
                MMC_SCOSendData(packet->data, packet->length);
#else
                {
                    COS_EVENT event = {0};
                    event.nEventId = RDABT_SCO_DATA_RES;
                    event.nParam1 = (u_int32)packet->data;
                    //rdabt_send_msg_to_bt(&event);
                    pMemcpy((void*)MMC_SCOGetRXBuffer(packet->flag), packet->data, packet->length);
                    MMC_SCOReceiveData(packet->length);
                }
#endif
            }
            else
            {
                if(ev.nEventId == RDABT_USER_BT_CLOSE_MSG)
                {
                    hal_HstSendEvent(1, 0x04191515);
                    rdabt_antenna_off();
                }
                COS_SendEvent(MOD_APP,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }
        }
    }
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
      hal_UartAllowSleep(TRANSPORT_UART_ID,TRUE);
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
      hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
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
      hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
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
    hal_UartAllowSleep(TRANSPORT_UART_ID,FALSE);
    COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


