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
#include "string.h"

#include "hal_host.h"
#include "global_macros.h"
#include "musb_core.h"

#define MUSB_DMA_CHN_NUM		1

typedef struct tagMusbDma{
	u8 dmaChn;
	u8 endnum;
}MUSBDMA;

static MUSBDMA g_MusbDma;

int musb_dma_start_tx(u8 pipe,  u32 count,  u32 addr, u16 maxPktSize)
{
	uint16 txcsr = musb_readw(MUSB_TXCSR);

	u32 ctrl = 0;

	//hal_HstSendEvent(USB_EVENT, 0x14072501);

	ctrl |= MUSB_DMA_MODE_1 | MUSB_DMA_ENABLE | MUSB_DMA_INT_ENABLE | MUSB_DMA_ENDNUM(pipe) | MUSB_DMA_BURSTMODE(0x01)|MUSB_DMA_READ;

	g_MusbDma.dmaChn = 1;
	g_MusbDma.endnum = pipe;

	txcsr |= MUSB_TXCSR_AUTOSET | MUSB_TXCSR_DMAENAB | MUSB_TXCSR_DMAMODE;

	musb_writew(MUSB_TXCSR, txcsr);
	musb_config_dma(1, addr, count, ctrl);	

	return 0;
	
}

int musb_dma_start_rx(u8 pipe, u32 count, u32 addr, u32 maxPktSize)
{
	uint16 csr = musb_readw(MUSB_RXCSR);
	u32 ctrl = 0;
	u16 pktCount;

	hal_HstSendEvent(USB_EVENT, 0x14072502);

	ctrl |= MUSB_DMA_MODE_1|MUSB_DMA_ENABLE | MUSB_DMA_INT_ENABLE | MUSB_DMA_ENDNUM(pipe) | MUSB_DMA_BURSTMODE(0x01);

	g_MusbDma.dmaChn = 1;
	g_MusbDma.endnum = pipe;	

	csr &= ~(MUSB_RXCSR_H_REQPKT | MUSB_RXCSR_DMAMODE);
	csr |= MUSB_RXCSR_AUTOCLEAR | MUSB_RXCSR_DMAENAB | MUSB_RXCSR_H_AUTOREQ;	

	pktCount = count/maxPktSize;
	if((count%maxPktSize))
	{
		pktCount++; 
	}

	musb_writew((0x300+pipe*2), pktCount);
	musb_writew(MUSB_RXCSR, csr);
    //  add &=0x0fffFFFF;
	musb_config_dma(1, addr, count, ctrl);

	return 0;
}

VOID musb_device_dma_int(u32 dmaInt)
{

}
int musb_dma_get_cur_epnum()
{
	return g_MusbDma.endnum; 
}

void musb_config_dma(u32 chan, u32 addr, u32 count, u32 ctrl)
{
	u32 *reg =(u32*) ( (u8*)gUsbRegs+ (chan-1)*0x10+0x20c);

	*reg-- = count;
	*reg--=addr;
	*reg=ctrl;
}



