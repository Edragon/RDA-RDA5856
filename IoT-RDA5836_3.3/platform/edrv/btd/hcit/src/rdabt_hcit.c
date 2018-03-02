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

#include "rdabt_main.h"
#include "bt.h"
#include "rdabt_hcit.h"
#include "hal_sharemem.h"
#include "hal_sys.h"

#ifdef BT_SHAREMEM_TRANSPORT
extern rdabt_context_struct rdabt_cntx;
UINT8 event_rx_page=0, cmd_tx_page=0, cmd_rx_page=0;

t_api rdabt_hcit_sharemem_init(void)
{
    hal_ShareMemInit();
    event_rx_page = 0;
    cmd_tx_page = 0;
    cmd_rx_page = 0;
    return 0;
}

t_api rdabt_hcit_sharemem_shutdown(void) 
{

    return 0;
}


void rdabt_hcit_sharemem_rx_data(void)
{
    UINT16 *head;
    UINT8 *buff;
    UINT16 rx_type, rx_len, head_len;
    UINT32 *rx_buff;
UINT16 rx_opcode;
    struct st_t_dataBuf *rx_buf;
    adp_data_msg_t data_msg;
    UINT16 header[2];
    UINT8 page;
    UINT8 wakeup = 0;

    do{
        head = hal_ShareMem_GetHeader();
        
        rx_type = head[0];
        rx_len = head[1];

        if(rx_type == 6)
        {
            buff = hal_ShareMem_GetData();
            cmd_rx_page = buff[0];
            rdabt_cntx.tx_busy = 0;
            hal_ShareMem_NextData(1);
            wakeup = 1;
            continue;
        }
		if(rx_type ==0)
			break;
		if(rx_type ==0xffff)
			break;
        if(rx_type < 2 || rx_type > 4)
        {
        	hal_DbgAssert("Wrong rx type");
            break;
        }
#ifdef UART2_HCI
		rx_buff = hal_ShareMem_GetData();
		if(rx_buff[0]==0x6231ff04)
		{
			{
		//		RDABT_Send_Message(MGR_PEERKEY_IND, RDABT_MANAGER, RDABT_ADP, 48, rx_buff+1);// uart
			}
		}
		else 
		{
			uart2_send_data(&rx_type, 1);
			uart2_send_data(rx_buff, rx_len);
		}
#else
		if(rx_type == 3) // sco data
		{
			buff = MMC_SCOGetRXBuffer();
			if(buff != NULL)
			{
				pMemcpy(buff, hal_ShareMem_GetData()+3, rx_len-3);
				MMC_SCOReceiveData(rx_len-3);
			}
		}
		else
		{
	        //hal_HstSendEvent(SYS_EVENT,0x10190000+rx_type);
	        //hal_HstSendEvent(SYS_EVENT,rx_len);

	        if (RDABT_NOERROR!=HCI_GetReceiveBuffer(&rx_buf, rx_len))
	        {
	        	hal_DbgAssert("No receive buff");
	            break;
	        }
			
	        pMemcpy(rx_buf->buf, hal_ShareMem_GetData(), rx_len); /* copy header bytes into PDU buffer*/
	        rx_buf->dataLen = rx_len;
	        
	        //hal_HstSendEvent(SYS_EVENT,rx_buf->buf[0]+(rx_buf->buf[3]<<16)+(rx_buf->buf[4]<<24));

	        data_msg.buff = rx_buf;
	        data_msg.type = rx_type;
	        rx_buf->transport = TRANSPORT_UART_ID;
	        RDABT_Send_Message(ADP_DATA_IND, RDABT_ADP, RDABT_HCI, sizeof(adp_data_msg_t), &data_msg);
		}
#endif		
        hal_ShareMem_NextData(rx_len);
        page = hal_ShareMem_GetRxPage();
        if(page != event_rx_page)
        {
            hal_ShareMem_SendData(&page, 1);
            header[0] = 0x6;
            header[1] = 1;
            hal_ShareMem_SendHeader(header);
            event_rx_page = page;
            break;
        }
        //hal_HstSendEvent(SYS_EVENT,page);
        wakeup = 1;
    }while(1);

    if(wakeup)
    {
#if CHIP_HAS_BTCPU==1
        hal_BtNotifyDataSent();
#else
        pWakeUpScheduler();
#endif
    }
}

t_api rdabt_hcit_sharemem_tx_pdu(t_DataBuf *pdu, UINT8 type, UINT16 flags, UINT16 hci_handle_flags)
{
	UINT16 bytesWritten;
	UINT8 *pkt_head;
    UINT16 pkt_head_len;
    UINT16 header[2];
    UINT32 critical;
    UINT8 page;

    if (flags & RDABT_HOST_BUF_HCI_HEADER_PRESENT)
		pkt_head_len=1;
	else
		pkt_head_len=5;
	pkt_head = pdu->buf-pkt_head_len;
    
	pkt_head[0]=type;
	
	if (pkt_head_len==5)
	{
		pkt_head[1]=hci_handle_flags & 0xFF;
		pkt_head[2]=hci_handle_flags >> 8;
		pkt_head[3]=pdu->dataLen& 0xFF;
		pkt_head[4]=pdu->dataLen>> 8;
	}

    header[0] = type;
    header[1] = pdu->dataLen+pkt_head_len-1;

    //hal_HstSendEvent(SYS_EVENT,0x10170000 + type);
    //hal_HstSendEvent(SYS_EVENT,pdu->buf[0]+(pdu->buf[1]<<8));
	
    if(type < 1 || type > 3)
    {
    	hal_DbgAssert("Wrong tx type");
    }

    critical = hal_SysEnterCriticalSection();

    hal_ShareMem_SendData(pkt_head+1,pdu->dataLen+pkt_head_len-1);
    hal_ShareMem_SendHeader(header);
    hal_BtNotifyDataSent();
    
    hal_SysExitCriticalSection(critical);
    
    HCI_FreeWriteBuffer(pdu, type);

    page = hal_ShareMem_GetTxPage();
    if(page != cmd_tx_page)
    {
        if(cmd_tx_page != cmd_rx_page
            &&(page+1 == cmd_rx_page || (cmd_rx_page == 0 && page == 7)))
            rdabt_cntx.tx_busy = 1;
        cmd_tx_page = page;
    }

    return 0;
}

t_api rdabt_hcit_sharemem_tx_data(UINT8 port, UINT8 *pdu, UINT16 len)
{
    UINT16 header[2];
    UINT32 critical;

    hal_HstSendEvent(SYS_EVENT,0x10170000 + pdu[0]);
    hal_HstSendEvent(SYS_EVENT,len);

    header[0] = pdu[0];
    header[1] = len-1;

    critical = hal_SysEnterCriticalSection();
    hal_ShareMem_SendData(pdu+1, len-1);
    hal_ShareMem_SendHeader(header);
	hal_BtNotifyDataSent();
    hal_SysExitCriticalSection(critical);

    return 0;
    
}
#endif

