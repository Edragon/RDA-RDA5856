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
#include "uart2_hci.h"
#include "event.h"
#include "uart2_transport.h"

#ifdef UART2_HCI

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
    hal_UartAllowSleep(UART2_PORT, 1);
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
	uart2_hcit_init();
   uart2_open();
   if(uart2_rx_buff == NULL)
       uart2_rx_buff = COS_Malloc_NoCache(UART_RX_BUFFER);
   memset(uart2_rx_buff, 0, UART_RX_BUFFER);
   uart2_configure(115200, FALSE);
   uart2_sleep_timer = COS_SetTimer(UART2_WAKE_UP_TIME, uart2_sleep_timeout, NULL, COS_TIMER_MODE_SINGLE);
}

void uart2_stop(void)
{
    hal_UartClose(UART2_PORT);
    if(uart2_rx_buff != NULL)
    {
        COS_Free(uart2_rx_buff);
        uart2_rx_buff = NULL;
    }
    uart2_hcit_shutdown();
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
    //EDRV_TRACE(EDRV_UART_TRC,0,"uart2_rx_buff:%s", uart2_rx_buff);  	
    UINT8 recv_cnt = hal_UartGetData(UART2_PORT, uart2_rx_buff, UART_RX_BUFFER);
    hal_HstSendEvent(SYS_EVENT, 0x00275850);
    hal_HstSendEvent(SYS_EVENT, rx_index);
    hal_HstSendEvent(SYS_EVENT, recv_cnt);
    EDRV_TRACE(EDRV_UART_TRC,0,"uart2_rx_buff:%02x %02x %02x %02x", uart2_rx_buff[0],uart2_rx_buff[1],uart2_rx_buff[2],uart2_rx_buff[3]);  
    uart2_wakeup();

	uart2_hcit_rx_data(uart2_rx_buff,recv_cnt);

	return rx_index;
}

void uart2_deal_data(UINT32 param)
{
}

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
    else if(status.rxLineErr)
    {
        hal_DbgAssert("UART ERROR!");
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
       hal_UartAllowSleep(UART2_PORT, 0);
       uart2_sleep_timer = COS_SetTimer(UART2_WAKE_UP_TIME, uart2_sleep_timeout, NULL, COS_TIMER_MODE_SINGLE);
    }
}


#endif //def UART2_HCI

