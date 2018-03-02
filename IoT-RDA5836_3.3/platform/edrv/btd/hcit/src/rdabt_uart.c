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


#include "cs_types.h"
#include "cos.h"
#include "rdabt_drv.h"

#include "rdabt_uart_drv.h"               /* header file for UART */
#include "rdabt_main.h"
#include "bt.h"
#include "rdabt_uart.h"
#include "rdabt_hcit.h"
#include "edrvp_debug.h"

extern rdabt_context_struct rdabt_cntx;
static UINT32 *rx_buff;

#if BLUETOOTH_USE_MARLIN==1
static UINT32 *rx_buff2; //for port 2

static   uint8  uart2_rx_buf[RDABT_RX_BUFF_SIZE];
volatile uint32 uart2_rx_length=0;

extern u_int8  rdabt_send_data_ready2;

#endif
extern u_int8  rdabt_send_data_ready;

void rdabt_uart_unsleep(void);
#ifdef BT_UART_BREAK_INT_WAKEUP
extern VOID UART_Register_Wakeup_Cb(void (*cb)());
#if BLUETOOTH_USE_MARLIN==1
extern VOID UART2_Register_Wakeup_Cb(void (*cb)());
#endif
#endif

/*****************************************************************************
* FUNCTION
*   rdabt_adp_uart_stop
* DESCRIPTION
*   Stop uart
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_adp_uart_stop(u_int8 port)
{
   UART_Close(/*BT_UART_PORT*/port);
    if(port==BT_UART_PORT)
    {
       if(rx_buff != NULL)
        {
            COS_Free(rx_buff);
            rx_buff = NULL;
        }
    }
    else
    {
#if BLUETOOTH_USE_MARLIN==1
       if(rx_buff2 != NULL)
        {
            COS_Free(rx_buff2);
            rx_buff2 = NULL;
        }
       uart2_rx_length=0;
       memset(uart2_rx_buf, 0, RDABT_RX_BUFF_SIZE);
#endif
    }
}


/*****************************************************************************
* FUNCTION
*   rdabt_adp_uart_start
* DESCRIPTION
*   Start UART for 5868
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_adp_uart_start(u_int8 port)
{
   if (!UART_Open (/*BT_UART_PORT*/port, MOD_BT))
    {
        hal_HstSendEvent(SYS_EVENT,0x03230404);
    }
   else
    {
        if(port==BT_UART_PORT)
        {
            if(rx_buff == NULL)
                rx_buff = COS_Malloc(RDABT_RX_BUFF_SIZE);
        }
#if BLUETOOTH_USE_MARLIN==1
        else  if(port==BT_UART_PORT2)
        {
            uart2_rx_length = 0;
            //if(rx_buff2 == NULL)
            //    rx_buff2 = COS_Malloc(RDABT_RX_BUFF_SIZE);
        } 
#endif		   
	}
}

/*****************************************************************************
* FUNCTION
*   rdabt_adp_uart_configure
* DESCRIPTION
*   Configure baud rate of UART
* PARAMETERS
*   baud    IN    baud rate of UART port
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_adp_uart_configure(UINT8 port, UINT32 baud,char flow_control)
{
    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"rdabt_adp_uart_configure port=%d",port));
   UART_SetDCBConfig (/*BT_UART_PORT*/port, baud, flow_control);
}


/*****************************************************************************
* FUNCTION
*   rdabt_adp_uart_rx
* DESCRIPTION
*   Read Rx buffer in UART when receive READY_TO_READ_IND or DATA_TO_READ_IND
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
int stop_flag=0;
void rdabt_adp_uart_rx(u_int8 port)
{
    UINT16 rx_length;
    UINT32 pos = 0;
    UINT8 status;
    rdabt_uart_unsleep();

     //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"rdabt_adp_uart_rx port=%d",port));
    if(port==BT_UART_PORT)
    {
        memset(rx_buff, 0, RDABT_RX_BUFF_SIZE);
        rx_length = UART_GetBytes(BT_UART_PORT, (UINT8*)(rx_buff+pos), RDABT_RX_BUFF_SIZE-pos*4, &status, MOD_BT);
        //if(rx_length>0)
        //kal_prompt_trace(MOD_BT, "rx_length: %d, rx_buff: %02x %02x %02x %02x %02x %02x %02x", rx_length, rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4], rx_buff[5], rx_buff[6]);
        //hal_HstSendEvent(SYS_EVENT,(pos<<16)|(rx_length&0xffff));
        if(rx_length >0)
        {
#if 0
        hal_HstSendEvent(SYS_EVENT,0x03230100);
        //hal_HstSendEvent(SYS_EVENT,rx_buff);
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos));
        if(rx_length>4)
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos+1));
        if(rx_length>8)
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos+2));
        if(rx_length>12)
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos+3));
        if(rx_length>16)
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos+4));
        if(rx_length>20)
        hal_HstSendEvent(SYS_EVENT,*(rx_buff+pos+5));
#endif
#ifdef UART2_HCI
	   EDRV_TRACE(EDRV_BTD_TRC,0,"rdabt_adp_uart_rx:%02x %02x %02x %02x", rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);

		if(rx_buff[0]==0x6231ff04)
		{
			{
				RDABT_Send_Message(MGR_PEERKEY_IND, RDABT_MANAGER, RDABT_ADP, 48, rx_buff+1);
			}
		}
		else 
			uart2_send_data(rx_buff, rx_length);
#else
        //if(stop_flag==0)
            rdabt_uart_rx_data((u_int8*)rx_buff,rx_length);
        //pos += (rx_length+3)/4;
        //if(pos*4 >= RDABT_RX_BUFF_SIZE*4/5)
        //    pos = 0;
#endif
        }

    }
	
#if BLUETOOTH_USE_MARLIN==1
    if(port==BT_UART_PORT2)
    {
#if 1
        if(uart2_rx_length)
        {
            uint32 cnt=0;

            //hal_HstSendEvent(SYS_EVENT,0x40170009);
            //hal_HstSendEvent(SYS_EVENT,uart2_rx_length);
            //hal_HstSendEvent(SYS_EVENT,COS_GetTickCount());

            cnt = uart2_rx_length;
            
            rdabt_uart2_rx_data(uart2_rx_buf,cnt);

            hal_UartIrqEnableBreakInt(BT_UART_PORT2, FALSE);
            uart2_rx_length -= cnt;
            if(uart2_rx_length)
            {
                pMemcpy(uart2_rx_buf, uart2_rx_buf+cnt, uart2_rx_length);
            }
            hal_UartIrqEnableBreakInt(BT_UART_PORT2, TRUE);
        }  

    #else
        memset(rx_buff2, 0, RDABT_RX_BUFF_SIZE);

        rx_length = UART_GetBytes(BT_UART_PORT2, &uart2_rx_buf[uart2_rx_length],, RDABT_RX_BUFF_SIZE-pos*4, &status, MOD_BT);
         uart_rx_length+=len_read;

        //hal_HstSendEvent(SYS_EVENT,0x40170009);
        //hal_HstSendEvent(SYS_EVENT,rx_length);
        //(SYS_EVENT,COS_GetTickCount());
       /* hal_HstSendEvent(SYS_EVENT,rx_buff2[1]);
        hal_HstSendEvent(SYS_EVENT,rx_buff2[3]);
        hal_HstSendEvent(SYS_EVENT,rx_buff2[4]);
        hal_HstSendEvent(SYS_EVENT,rx_buff2[5]);*/
        if(rx_length >0)
        {
            rdabt_uart2_rx_data(rx_buff2,rx_length);

        }
        #endif
    }
#endif	
    // trace_mem();
}


/*****************************************************************************
* FUNCTION
*   rdabt_adp_uart_tx
* DESCRIPTION
*   Send data to Tx buffer
* PARAMETERS
*   buf            IN      buffer of data to be sent
*   num_to_send   IN       number of bytes to be sent
*   num_send      OUT     buffer to store actual size of data sent.
* RETURNS
*   TRUE if all bytes sent; otherwise, FALSE.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
unsigned char rdabt_adp_uart_tx(u_int8 port, CONST UINT8 *buf, unsigned short num_to_send, unsigned short *num_send)
{
	//EDRV_TRACE(EDRV_BTD_TRC,0,"rdabt_adp_uart_tx:%02x %02x %02x %02x", buf[0],buf[1],buf[2],buf[3]);
    //hal_HstSendEvent(SYS_EVENT, 0x20161043);
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"rdabt_adp_uart_tx port=%d,num_to_send=%d, buf[0-3]=0x%x, 0x%x, 0x%x, 0x%x",port, num_to_send, buf[0], buf[1], buf[2], buf[3] ));

   rdabt_uart_unsleep();   
   *num_send = (UINT16)UART_PutBytes(/*BT_UART_PORT*/port, (UINT8*)buf, (UINT16)num_to_send, MOD_BT);

#if 0
    hal_HstSendEvent(SYS_EVENT,0x03230200);
    hal_HstSendEvent(SYS_EVENT,num_to_send);
    //hal_HstSendEvent(SYS_EVENT,*num_send);
    //hal_HstSendEvent(SYS_EVENT,buf);
    hal_HstSendEvent(SYS_EVENT,buf[0]|(buf[1]<<8)|(buf[2]<<16)|(buf[3]<<24));
    if(num_to_send>4)
    hal_HstSendEvent(SYS_EVENT,buf[4]|(buf[5]<<8)|(buf[6]<<16)|(buf[7]<<24));
    if(num_to_send>8)
    hal_HstSendEvent(SYS_EVENT,buf[8]|(buf[9]<<8)|(buf[10]<<16)|(buf[11]<<24));
#endif

   //kal_prompt_trace(0, "num_to_send: %d, num_send: %d", num_to_send, *num_send);
   if(*num_send == num_to_send)
      return TRUE;
   else
   {
      return FALSE;
   }
}


/*****************************************************************************
* FUNCTION
*   RDA5868_uart_rx_cb
* DESCRIPTION
*   Callback function when RX interrupt comes
* PARAMETERS
*   port	IN UART power number
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
static void rdabt_uart_rx_cb(UINT8 port)
{
    COS_EVENT event = {0};
    event.nEventId = EV_BT_READY_TO_READ_IND;
    event.nParam1 = BT_UART_PORT;
    if(rdabt_send_data_ready<3)
    {
        //hal_HstSendEvent(SYS_EVENT,0x04070121);
        //hal_HstSendEvent(SYS_EVENT,rdabt_send_data_ready);
        COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        rdabt_send_data_ready ++;
    }
}

#if BLUETOOTH_USE_MARLIN==1
static void rdabt_uart2_rx_cb(UINT8 port)
{
    COS_EVENT event = {0};
    UINT16 len_read=0;
    UINT8 status=0;
    u_int32 num=0;
    event.nEventId = EV_BT_READY_TO_READ_IND;
    event.nParam1 = BT_UART_PORT2;

#if 1
            for(num=3;num>0;num--)
            {
                len_read = UART_GetBytes(BT_UART_PORT2, &uart2_rx_buf[uart2_rx_length],RDABT_RX_BUFF_SIZE, &status, MOD_BT);
                uart2_rx_length+=len_read;
                    if(len_read==0)
                        break;
            }
#endif    
     
    if(rdabt_send_data_ready2<10)
    {
        //hal_HstSendEvent(SYS_EVENT,0x04070121);
        //hal_HstSendEvent(SYS_EVENT,rdabt_send_data_ready);
        COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        rdabt_send_data_ready2 ++;
    }
}
#endif

#if 0
static void rdabt_uart_tx_cb(UART_PORT port)
{
   ilm_struct *msg;
   ASSERT(port == BT_UART_PORT);

   msg = allocate_ilm(MOD_UART3_HISR);
   msg->src_mod_id = MOD_UART3_HISR;
   msg->dest_mod_id = MOD_BT;
   msg->sap_id = INVALID_SAP;
   msg->msg_id = MSG_ID_UART_READY_TO_WRITE_IND;
   msg->local_para_ptr = NULL;
   msg->peer_buff_ptr = NULL;
   msg_send_ext_queue_to_head(msg);
}
#endif

void rdabt_uart_register_cb(u_int8 port)
{
    UINT8 status=0;
    char temp_buff[16];
   // clean the data in rx buff
  // UART_ClrRxBuffer   (BT_UART_PORT, MOD_BT);
   while(UART_GetBytes(/*BT_UART_PORT*/port, temp_buff, 16, &status, MOD_BT));

    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"rdabt_uart_register_cb port=%d,status=%d",port,status));

  //  kal_prompt_trace(0,"rdabt_uart_register_cb");
  
  if(port==BT_UART_PORT)
        UART_Register_RX_cb(BT_UART_PORT, MOD_BT, rdabt_uart_rx_cb);
#if BLUETOOTH_USE_MARLIN==1			
	else
    {
        UART_Register_RX_cb(BT_UART_PORT2, MOD_BT, rdabt_uart2_rx_cb);
    }
#endif  
  // UART_Register_TX_cb(BT_UART_PORT, MOD_BT, rdabt_uart_tx_cb);

#ifdef BT_UART_BREAK_INT_WAKEUP
    UART_Register_Wakeup_Cb(rdabt_uart_eint_hdlr);
#if BLUETOOTH_USE_MARLIN==1	
    UART2_Register_Wakeup_Cb(rdabt_uart_eint_hdlr);
#endif	
#endif
}

#define __RDABT_INITALL_BY_IIC__ 1

void rdabt_wirte_memory(unsigned int addr,const unsigned int *data,unsigned char len,unsigned char memory_type)
{
#if BLUETOOTH_USE_MARLIN==1
    u_int8 port =BT_UART_PORT2;
#else
    u_int8 port =BT_UART_PORT;
#endif

#ifdef __RDABT_INITALL_BY_IIC__
   if(memory_type == 0x00)
   {
     rdabt_iic_core_write_data(addr,data,len);  
   }
   else
#endif
   {
   unsigned short num_send,num_to_send;	
   unsigned char i;
   unsigned char data_to_send[256];
   unsigned int address_convert;
   data_to_send[0] = 0x01;
   data_to_send[1] = 0x02;
   data_to_send[2] = 0xfd;
   data_to_send[3] = len*4+6;
   data_to_send[4] = memory_type;
   if(memory_type == 0x10)
	       data_to_send[4] = 0x00;
//       else 
//        data_to_send[4] = 0x81;
   data_to_send[5] = len;
   if(memory_type == 0x01)
   {
      address_convert = addr*4+0x200;
      data_to_send[6] = address_convert;
      data_to_send[7] = address_convert>>8;
      data_to_send[8] = address_convert>>16;
      data_to_send[9] = address_convert>>24;	  
   }
   else
   {
      data_to_send[6] = addr;
      data_to_send[7] = addr>>8;
      data_to_send[8] = addr>>16;
      data_to_send[9] = addr>>24;
   }
   for(i=0;i<len;i++,data++)
   {
       unsigned char j=10+i*4;
       data_to_send[j] = *data;
       data_to_send[j+1] = *data>>8;
       data_to_send[j+2] = *data>>16;
       data_to_send[j+3] = *data>>24;
   }
   num_to_send = 4+data_to_send[3];
   
   rdabt_adp_uart_tx(port, data_to_send,num_to_send,&num_send);
   }	
}

void rdabt_write_pskey(unsigned char id, const unsigned int *data,unsigned char len)
{
   unsigned short num_to_send;	
   unsigned char data_to_send[128];
    EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_write_pskey id=%x, len=%d",id,len);
	
   data_to_send[0] = 0x01;
   data_to_send[1] = 0x05;
   data_to_send[2] = 0xfd;
   data_to_send[3] = len+1;
   data_to_send[4] = id;
   if(len)
   	memcpy(data_to_send+5, data, len);
   num_to_send = 5+len;

   RDABT_Tranport_Tx_Data(BT_UART_PORT, data_to_send, num_to_send);

}

