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

#ifdef MCU_WIFI_PLAT
#include "cs_types.h"

#include "hal_sys.h"
#include "hal_debug.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_uart.h"
#include "hal_usb.h"

#include "stdarg.h"

#define DBG_TRACE_MAX_LEVEL  16

static volatile int dl_pktLen;
static int dl_msgId;
static unsigned char dl_pkt[1600];
static int send_ok;


void printf (const char *fmt, ...)
{
    UINT32 i;
    char printbuffer[256];

    va_list args;

    va_start (args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = vsprintf (printbuffer, fmt, args);
    va_end (args);

    /* Print the string */
    hal_UartPuts(printbuffer);
}


static void usb_if_tx_comp(urb_t *urb)
{
	send_ok = 1;
	printf("usb_if_tx_comp\n\r");
}

static void usb_if_rx_comp(urb_t *urb)
{
    if(urb->status==USB_XFER_SUCCESS)
    {
        UINT8 fmt[2];
        fmt[0] = urb->data[0];
        fmt[1] = urb->data[1];

        urb->actual_len = (fmt[0]) | (fmt[1]<<8);
        urb->data = NULL;
        urb->length = 0;

        dl_pktLen = urb->actual_len;
        printf("usb get one pkt:%d\n\r", dl_pktLen);
        urb->actual_len = 0;
    }
}

void hal_dump_buf(const UINT8 *buf, UINT16 len)
{
    int i;

    for (i=0;i<len;i++) {
        if(i%8 == 0)
            printf("  ");
        printf("%02x ", buf[i]);
        if((i+1)%16 == 0)
            printf("\n");
    }
    printf("\n");
}


unsigned char snd_back[] ={"ready to receive another pkt"};


// This xcpu_main replaces the regular xcpu_main present in the platform library
PROTECTED void main(void)
{
	UINT32 buffer[] ={0xffffffff, 0xeeeeeeee, 0xbbbbbbbb, 0x88888888};
	UINT32 buffer2[4];
	UINT32 bytes;
	UINT32 len = 0;
	UINT32 addr = 0;

	memd_FlashOpen(NULL);

#if 0
	memd_FlashErase(0, 4096);
	memd_FlashWrite(0, sizeof(buffer), &bytes, buffer);
	memd_FlashRead(0, sizeof(buffer2), &bytes, buffer2);
	printf("read back %x, %x, %x, %x", buffer2[0], buffer2[1], buffer2[2], buffer2[3]);
	while(1);

#else

	hal_usb_start(usb_if_tx_comp, usb_if_rx_comp);

	dl_pktLen = 0;
	dl_msgId = 0;
	memset(dl_pkt, 0, sizeof(dl_pkt));

	printf("ready to download\n\r");
	send_ok = 1;
	while(1)
	{
		while(send_ok == 0);
		usb_hif_receive(dl_pkt, 1540);
		while(dl_pktLen == 0);

		len = dl_pkt[0] | (dl_pkt[1]<<8);
		dl_msgId = dl_pkt[2] | (dl_pkt[3]<<8);
		printf("get one pkt len:%d, msgid:%d\n\r", len, dl_msgId);

		if(len > 0)
		{
			if((addr & (4096-1)) == 0)
			{
				//memd_FlashErase(addr, addr+4096);
				printf("need to erase a sector\n");
			}
			memd_FlashWrite(addr, len, &len, &dl_pkt[4]);

			addr += len;
			if(len < 1024)
			{
				void(*boot_fun)(void) = (void(*)(void))0x80000000;
				printf("ready to boot\n");
				hal_SysDisableIrq(&len);
				hal_IrqInitialise();
				boot_fun();
			}

			send_ok = 0;
			dl_pktLen = 0;
			printf("send\n");
			usb_hif_send(snd_back, sizeof(snd_back));
		}
		else
		{

		}
	}
#endif
}

#endif
