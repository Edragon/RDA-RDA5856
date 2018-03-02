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
#include "hal_sys.h"
#include "uart2_transport.h"
#include "chip_id.h"

enum
{
    HCI_UART_STATE_RX_TYPE,
    HCI_UART_STATE_RX_HEADER,      
    HCI_UART_STATE_RX_DATA    
};

#if 1
static char *transportTypes[] = {        
    "HCI Command",
    "ACL Data",
    "SCO Data",
    "HCI Event",
    "RS232 Error",
    "RS232 Negotiation"
};
#endif


static UINT8 uart2_hci_rx_state;
static UINT16 uart2_hci_pend_length;                  /* the length of data have received */
static UINT16 uart2_hci_rx_length;                    /* the length of data need receive */
static UINT16 uart2_hci_rx_buf_length;                /* the length of buf */
static UINT8* uart2_hci_rx_buf;
static UINT8  uart2_hci_rx_head_buf[1021+4];
static UINT8  uart2_hci_rx_type; 
static const UINT8 uart2_hci_head_length[] = {3,4,3,2};


int uart2_hcit_init(void) 
{
    uart2_hci_rx_state = HCI_UART_STATE_RX_TYPE;
    uart2_hci_pend_length = 0;
    uart2_hci_rx_buf_length = 0;
    uart2_hci_rx_length = 1;
    uart2_hci_rx_buf = uart2_hci_rx_head_buf;
    return 0;
}

int uart2_hcit_shutdown(void) 
{
    return 0;
}

void uart2_hcit_rx_data(UINT8 *pdu,UINT32 length)
{
    extern BOOL g_test_mode;
    extern BOOL  g_CalbClock;
     //hal_HstSendEvent(SYS_EVENT,0x04060200);
    if(uart2_hci_pend_length + length < uart2_hci_rx_length)
    {
        if(uart2_hci_rx_buf != NULL)
            memcpy(uart2_hci_rx_buf + uart2_hci_pend_length, pdu, length);
        uart2_hci_pend_length += length;
     //hal_HstSendEvent(SYS_EVENT,0x04060300);
        return;
    }

    while(uart2_hci_pend_length + length >=  uart2_hci_rx_length)    
    {
        if(uart2_hci_rx_buf != NULL)
            memcpy(uart2_hci_rx_buf + uart2_hci_pend_length, pdu, uart2_hci_rx_length - uart2_hci_pend_length);
        pdu += uart2_hci_rx_length - uart2_hci_pend_length;
        length -= uart2_hci_rx_length - uart2_hci_pend_length;

#if 1
        hal_HstSendEvent(SYS_EVENT,0x04060210);
        hal_HstSendEvent(SYS_EVENT,uart2_hci_rx_state);
        hal_HstSendEvent(SYS_EVENT,pdu);
        hal_HstSendEvent(SYS_EVENT,uart2_hci_pend_length);
        hal_HstSendEvent(SYS_EVENT,uart2_hci_rx_length);
#endif    
        switch(uart2_hci_rx_state)
        {
        case HCI_UART_STATE_RX_TYPE:
            uart2_hci_rx_type = uart2_hci_rx_buf[0];
            /* rx type error */
            if(uart2_hci_rx_type < 1 || uart2_hci_rx_type > 3)
            {
                hal_HstSendEvent(SYS_EVENT,0x04060010);
                hal_HstSendEvent(SYS_EVENT,uart2_hci_rx_type);
                break;
            }
            uart2_hci_rx_length = uart2_hci_head_length[uart2_hci_rx_type - 1];
            uart2_hci_rx_buf_length = uart2_hci_rx_length+1;
            uart2_hci_rx_buf = uart2_hci_rx_head_buf+1;
            uart2_hci_rx_state = HCI_UART_STATE_RX_HEADER;
            break;
        case HCI_UART_STATE_RX_HEADER:

            uart2_hci_pend_length = uart2_hci_rx_length;
            
            if(uart2_hci_rx_type == 2) /* ACL data */
            {
                uart2_hci_rx_length = ((UINT16)uart2_hci_rx_buf[3]<<8) |  uart2_hci_rx_buf[2];
            }
            else //if(uart2_hci_rx_type == 3) /* SCO data */
            {
                uart2_hci_rx_length =  uart2_hci_rx_buf[2];
            }
            //else  /* Event */
            {
                //uart2_hci_rx_length =  uart2_hci_rx_buf[1];
            }

            uart2_hci_rx_buf_length += uart2_hci_rx_length;
			uart2_hci_rx_buf = uart2_hci_rx_head_buf+uart2_hci_pend_length+1;

            uart2_hci_rx_state = HCI_UART_STATE_RX_DATA;
            
            if(uart2_hci_rx_length != 0)
            {
                break;
            }
            /* for no payload, direct finish the packet */
        case HCI_UART_STATE_RX_DATA:
            hal_HstSendEvent(SYS_EVENT,0x04060020);
            hal_HstSendEvent(SYS_EVENT,uart2_hci_rx_type);
            hal_HstSendEvent(SYS_EVENT,uart2_hci_rx_buf_length);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E) 
	UINT8 *rx_buff;
	rx_buff = TRAhcit_Generic_Get_Rx_Buf(uart2_hci_rx_type, uart2_hci_rx_length , uart2_hci_rx_head_buf+1);
	if(rx_buff != NULL)
	{
		hw_memcpy8(rx_buff, uart2_hci_rx_buf , uart2_hci_rx_length);	
		TRAhcit_Generic_Commit_Rx_Buf(uart2_hci_rx_type);
	}
#else
#ifdef BT_SHAREMEM_TRANSPORT
        	rdabt_hcit_sharemem_tx_data(uart2_hci_rx_head_buf, uart2_hci_rx_buf_length);
#else
			rdabt_uart_tx_data(uart2_hci_rx_head_buf, uart2_hci_rx_buf_length);
#endif
#endif
            uart2_hci_rx_state = HCI_UART_STATE_RX_TYPE;
            
            break;
        }

        if(uart2_hci_rx_state == HCI_UART_STATE_RX_TYPE)
        {
            uart2_hci_rx_buf = uart2_hci_rx_head_buf;
            uart2_hci_rx_length = 1;
        }
        uart2_hci_pend_length = 0;
    }
    /** save the last data **/
    if(length > 0 && uart2_hci_rx_buf != NULL)
        memcpy(uart2_hci_rx_buf, pdu, length);
    uart2_hci_pend_length = length;
//     hal_HstSendEvent(SYS_EVENT,0x04060300);
}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
void TRAhcit_UART_Transmit(UINT8 *data, UINT32 pdu_len, UINT8 *header, 
		UINT8 head_len, UINT8 tra_source, UINT8 device_index)
{ 
	uart2_send_data(header,head_len);
	uart2_send_data(data,pdu_len);	
	TRAhcit_Generic_Acknowledge_Complete_Tx( tra_source, pdu_len + head_len); 
 }


void TRAhcit_UART_Initialise(void)
{}

void TRAhcit_UART_Shutdown(void)
{}

void TRAhcit_UART_Convert_Baudrate(void)
{}
#endif
