/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "edrvp_debug.h"
#include "cs_types.h"
#include "cos.h"
#include "hal_uart.h"
#include "hal_sys.h"
#include "uart2.h"
#include "event.h"

#if 0

#define UART2_WAKE_UP_TIME       					1000       /* uart2 wake up time, default 1second */

typedef struct
{
	char customer_at_cmd[2];
	UINT16 rda_at_cmd;
}AT_TABLE;

static UINT8 *uart2_rx_buff = NULL;
static UINT8 *uart2_tx_buff = NULL;
HAL_UART_IRQ_STATUS_T  uart2_mask = {0};
HAL_UART_CFG_T uart2_cfg = {0};

static HAL_SYS_CLOCK_OUT_ID_T g_RdaBtClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
static UINT8 rx_index = 0;
static UINT8 uart2_sleep_timer;

const AT_TABLE at_list[] = 
{
{"CA", /*BT_VISIBLE*/UART_BT_VISIBLE},                 //蓝牙可见
{"CB", /*BT_INVISIBLE*/UART_BT_INVISIBLE},               //蓝牙不可见
//{"CC", HFG_ACL_CONNECT_REQ},        //HFP连接
{"CD", /*BT_DISCONNECT*/110},              //HFP断开
//{"CE", }                          //接听电话
//{"CF", }                          //拒接电话
//{"CG", }                          //通话结束
//{"CH", }                          //重拨
//{"CI", }							//语音拨号
//{"CJ", }							//语音拨号结束
//{"CM", }							//静音或取消静音
//{"CO", HFG_RING_IND}                //传输活动呼叫
//{"CW", }							//开始呼叫
//{"CX", }							//send DTMF
//{"CY", }							//查询HFP状态
//{"CZ", }							//reset
//{"MA", }							//play/pause music
//{"MC", }							//stop music
//{"MD", }							//音乐快进
//{"ME", }							//音乐快退
//{"MF", }							//Query auto answer and poweron auto connection configration
//{"MG", }							//Enable poweron auto connection
//{"MH", }							//Disable poweron auto connection
{"MI", /*BT_CONNECT_A2DP*/108},				//Connect to AV source
{"MJ", /*BT_DISCONNECT*/110},				//Disconnect from AV source
//{"MM", }							//Change Local Device Name Casually
//{"MN", }							//Change Local Device Pin Code
//{"MO", }							//Query AVRCP Status
//{"MP", }							//Enable Auto Answer
//{"MQ", }							//Disable Auto Answer
//{"MR", }							//Start Fast Forward
//{"MS", }							//Start Rewind
//{"MT", }							//Stop Fast Forward / Rewind
//{"MV", }							//Query A2DP Status
//{"MY", }							//Query Module Software Version
//{"PA", }							//Synchronize Phonebook Stored by SIM(via AT Command)
{"PB", /*BT_PHONE_BOOK*/114},				//Synchronize Phonebook Stored by Phone(via AT Command)
//{"PC", }							//Read Next One Phonebook Item
//{"PD", }							//Read Previous One Phonebook Item
//{"PH", }							//Synchronize Dialed Calls List (via AT Command)
//{"PI", }							//Synchronize Received Calls List (via AT Command)
//{"PJ", }							//Synchronize Missed Calls List (via AT Command)
//{"PK", }							//Synchronize Last Call List (via AT Command)
//{"PL", }							//Get Recent Dialed Call History (record by module)
//{"PM", }							//Get Recent Received Call History (record by module)
//{"PN", }							//Get Recent Missed Call History (record by module)
//{"PO", }							//Dial Last Received Phone Number
//{"PR", }							//Clear Call History (record by module)
//{"VC", }							//set clock debug mode
//{"VD", AP_KEY_VOLSUB | AP_KEY_DOWN}	//Speaker Volume Down
//{"VE", }							//Enter Test Mode
//{"VF", }							//Set to Fixed Frequency
{"VI", /*BT_INQUIRY_DEVICE*/119},			//Start Inquiry
//{"VU", AP_KEY_VOLSUB | AP_KEY_DOWN}	//Speaker Volume Up
};

//收到的命令格式
//| AT# 3bytes | CMD 2byte | CMD Param |
static int find_str(
			 const char *src_str, 
			 const char *spec_str, 
			 int str_len 
			)
{
	int index = 0;
	int length = strlen(src_str);
	
	if(src_str == NULL || spec_str == NULL)
		return -1;
	if(str_len == 0)
		return -1;

	while(index < length)
	{
		if(!strncmp(&src_str[index], &spec_str[0], str_len))
			return index;
		else index++;
	}
	return -2;
}

static int query_at_table(char *cmd)
{
	int i = 0;

	for(i = 0; i < (sizeof(at_list)/4); i++)
	{
		if(!strncmp(cmd, at_list[i].customer_at_cmd, 2))
			return i;
	}
	return -1;
}

void uart2_deal_data(UINT32 param)
{
	int ret = 0;
	int tmp_index;
	char cmd_data[255] = {0};
	char cmd[3] = {0};
	COS_EVENT ev = {0};
	char divide[2] = {0x0d, 0x0a};
	char *p_data = (char *)param;
	//解析uart数据内的命令
	tmp_index = 3;//命令头占了3bytes
	ret = find_str(&p_data[3], divide, 2);
    hal_HstSendEvent(SYS_EVENT, 0x00275855);
    hal_HstSendEvent(SYS_EVENT, ret);
	if(ret > 0)
	{
		strncpy(cmd_data, &p_data[3], ret);
		cmd[0] = cmd_data[0];
		cmd[1] = cmd_data[1];
		hal_HstSendEvent(SYS_EVENT, 0x00275856);
        hal_HstSendEvent(SYS_EVENT, cmd[0]);
        hal_HstSendEvent(SYS_EVENT, cmd[1]);
		if((ret = query_at_table(cmd)) >= 0)
		{
		    hal_HstSendEvent(SYS_EVENT, 0x00275857);
            hal_HstSendEvent(SYS_EVENT, ret);
			ev.nEventId = at_list[ret].rda_at_cmd;
			COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		}
	}
	if(p_data)
    {   
		COS_Free(p_data);
        p_data = NULL;
    }
}

static void  set_uart2_parameter(HAL_UART_IRQ_STATUS_T * pMask, HAL_UART_CFG_T *pCfg)
{
    pMask->rxDataAvailable=1;
	pMask->rxTimeout       =1;        
	pMask->rxLineErr       =  0;       
	pMask->rxDmaDone       = 0;        
	pMask->rxDmaTimeout    = 0;  	
	pMask->txModemStatus=0;
	pMask->txDataNeeded= 0;
	pMask->txDmaDone       = 0;   

	pCfg->rx_trigger = HAL_UART_RX_TRIG_QUARTER; 	
	pCfg->rx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;	 
	pCfg->tx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;		
	pCfg->data = HAL_UART_8_DATA_BITS;	
	pCfg->stop = HAL_UART_1_STOP_BIT;
	pCfg->parity = HAL_UART_NO_PARITY;
	pCfg->tx_trigger = HAL_UART_TX_TRIG_EMPTY;	
	pCfg->afc = HAL_UART_AFC_MODE_DISABLE;	
	pCfg->irda = HAL_UART_IRDA_MODE_DISABLE;			
	pCfg->rate  =   HAL_UART_BAUD_RATE_115200;
}

static void uart2_sleep_timeout(void *param)
{
    EDRV_TRACE(EDRV_UART_TRC, 0, "Uart2 sleep!");
    hal_UartAllowSleep(HAL_UART_2, 1);
    uart2_sleep_timer = 0;
}

static BOOL uart2_open(void)
{		      
    set_uart2_parameter(&uart2_mask, &uart2_cfg);
    
	g_RdaBtClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_32K);
    if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == g_RdaBtClockOutId)
    {
        return FALSE;
    }
    
 	return TRUE ;
}


void uart2_start(void)
{
   uart2_open();
   if(uart2_rx_buff == NULL)
       uart2_rx_buff = COS_Malloc_NoCache(UART_RX_BUFFER);
   memset(uart2_rx_buff, 0, UART_RX_BUFFER);
   uart2_configure(115200, FALSE);
   uart2_sleep_timer = COS_SetTimer(UART2_WAKE_UP_TIME, uart2_sleep_timeout, NULL, COS_TIMER_MODE_SINGLE);
}

void uart2_stop(void)
{
    hal_UartClose(HAL_UART_2);
    if(uart2_rx_buff != NULL)
    {
        COS_Free(uart2_rx_buff);
        uart2_rx_buff = NULL;
    }
}

UINT32 uart2_send_data(const UINT8* data, UINT32 length)
{
    int len, Tx_fifo_avail;
    uart2_wakeup();
    EDRV_TRACE(EDRV_UART_TRC, 0, "Send data to uart2, length=%d!", length);
    while(length>0)
    {
        UINT32 suspend_time = 0;
        UINT32  send_timer_out=0;

        send_timer_out = hal_TimGetUpTime();
        while(!hal_UartTxFinished(UART2_PORT))
        {
            suspend_time = hal_TimGetUpTime();
            if (suspend_time - send_timer_out > 2*16384)
                return 0;
        }
        Tx_fifo_avail=hal_UartTxFifoAvailable(UART2_PORT);
        len = hal_UartSendData(UART2_PORT, data, length>Tx_fifo_avail?Tx_fifo_avail:length);
        data += len;
        length -= len;
    };
    return length;
}

UINT32 uart2_recv_data(void)
{
    UINT8 check_crc = 0;
    UINT8 i = 0;
	char *cmd_buf = NULL;
    char divide[2] = {0x0d, 0x0a};
    EDRV_TRACE(EDRV_UART_TRC,0,"uart2_rx_buff:%s", uart2_rx_buff);  	
	UINT8 recv_cnt = hal_UartGetData(UART2_PORT, uart2_rx_buff + rx_index, UART_RX_BUFFER);
    rx_index += recv_cnt;
    hal_HstSendEvent(SYS_EVENT, 0x00275850);
    hal_HstSendEvent(SYS_EVENT, rx_index);
    hal_HstSendEvent(SYS_EVENT, recv_cnt);
    EDRV_TRACE(EDRV_UART_TRC,0,"uart2_rx_buff:%s", uart2_rx_buff);  
    uart2_wakeup();

    if(rx_index >= 3)// 接收到了三个字符，命令头是AT#
    {
        hal_HstSendEvent(SYS_EVENT, 0x00275851);
        if(strncmp(uart2_rx_buff, "AT#", 3))//判断是否是命令
        {
            hal_HstSendEvent(SYS_EVENT, 0x00275852);
            memset(uart2_rx_buff, 0, UART_RX_BUFFER);
            rx_index = 0;
        }
		else if(find_str(uart2_rx_buff, divide, 2) >= 0)//收到了完整命令，解析命令内容，然后发消息给system task
		{
		    hal_HstSendEvent(SYS_EVENT, 0x00275853);
			cmd_buf = (UINT8 *)COS_Malloc(UART_RX_BUFFER);
			memcpy(cmd_buf, uart2_rx_buff, UART_RX_BUFFER);
            //memset(uart2_rx_buff, 0, UART_RX_BUFFER);
			COS_EVENT ev = {0};
			ev.nEventId = EV_UART2_DATA;
			ev.nParam1 = cmd_buf;
			COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            memset(uart2_rx_buff, 0, UART_RX_BUFFER);
		}
    }
	return rx_index;
}

#if 0
//收到的命令格式
//| 0x01 1byte | event 4byte | crc 1byte |

UINT32 uart2_recv_data(void)
{
    UINT8 check_crc = 0;
    UINT8 i = 0;
    UINT8 recv_cnt = hal_UartGetData(UART2_PORT, uart2_rx_buff + rx_index, UART_RX_BUFFER);
    rx_index += recv_cnt;
    
    if(rx_index == 1)// 接收到了第一个字符
    {
        if(uart2_rx_buff[0] != 0x01)//判断第一个字符是否是命令
        {
            rx_index = 0;
        }
    }

    if(rx_index == 6)
    {
        for(i = 0; i < rx_index; i++)
        {
            check_crc ^= uart2_rx_buff[i];
        }
        if(check_crc == 0xff)//data right
        {
            COS_EVENT ev = {0};
            ev.nParam1 = (uart2_rx_buff[1]+(uart2_rx_buff[2]<<8)) & 0xffff;
            ev.nEventId = (uart2_rx_buff[3]+(uart2_rx_buff[4]<<8)) & 0xffff;
//            hal_HstSendEvent(SYS_EVENT, 0x00275850);
//            hal_HstSendEvent(SYS_EVENT, ev.nEventId);
//            hal_HstSendEvent(SYS_EVENT, ev.nParam1);
            COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        }
        memset(uart2_rx_buff, 0, UART_RX_BUFFER);
        rx_index = 0;
    }
    
    return rx_index;
}
#endif

void uart2_irq_handler(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error) 
{
    UINT32 uDataSize = 0;  
    
    if(status.rxDataAvailable)
    {
        uDataSize =  uart2_recv_data();
        if(uDataSize>0) 
        {
            hal_UartSetRts(UART2_PORT, FALSE);
        }  
        //if have callback function, call it at here
    }
    else if(status.rxTimeout)
    {
        uDataSize  = 1;
        while(uDataSize>0)
        {
            uDataSize = uart2_recv_data(); 
            if(uDataSize>0) 
            {
                hal_UartSetRts(UART2_PORT, FALSE);
            }
            uDataSize  = hal_UartRxFifoLevel(UART2_PORT);
        }  
    }
    if(uart2_rx_buff[0] == 0)
    {
        rx_index = 0;
    }
}

void uart2_configure(UINT32 band, char flow_control)
{
   	void (*uart2_isr)(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error);
	hal_UartClose(UART2_PORT);
	uart2_isr = uart2_irq_handler;
	uart2_cfg.rate = band; 	
    if(flow_control)
    {
    	uart2_cfg.afc = HAL_UART_AFC_MODE_RX_TRIG_1;
    }
    else
    {
    	uart2_cfg.afc = HAL_UART_AFC_MODE_DISABLE;
    }
	hal_UartOpen(UART2_PORT, &uart2_cfg);	
	hal_UartFifoFlush(UART2_PORT);	
	hal_UartClearErrorStatus(UART2_PORT);    	
	hal_UartSetRts(UART2_PORT, TRUE); 

	hal_UartIrqSetMask(UART2_PORT, uart2_mask);
	hal_UartIrqSetHandler(UART2_PORT, uart2_isr);

	return;	 

}

void uart2_wakeup()
{
    if(uart2_sleep_timer != 0)
        COS_ChangeTimer(uart2_sleep_timer, UART2_WAKE_UP_TIME);
    else
    {
       EDRV_TRACE(EDRV_UART_TRC, 0, "Wakeup uart2!");
       hal_UartAllowSleep(HAL_UART_2, 0);
       uart2_sleep_timer = COS_SetTimer(UART2_WAKE_UP_TIME, uart2_sleep_timeout, NULL, COS_TIMER_MODE_SINGLE);
    }
}


#endif //

