#if (CHIP_HAS_USB == 1)

/*
 * MUSB OTG driver host support
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 * Copyright (C) 2008-2009 MontaVista Software, Inc. <source@mvista.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* MUSB HOST status 22-mar-2006
 *
 * - There's still lots of partial code duplication for fault paths, so
 *   they aren't handled as consistently as they need to be.
 *
 * - PIO mostly behaved when last tested.
 *     + including ep0, with all usbtest cases 9, 10
 *     + usbtest 14 (ep0out) doesn't seem to run at all
 *     + double buffered OUT/TX endpoints saw stalls(!) with certain usbtest
 *       configurations, but otherwise double buffering passes basic tests.
 *     + for 2.6.N, for N > ~10, needs API changes for hcd framework.
 *
 * - DMA (CPPI) ... partially behaves, not currently recommended
 *     + about 1/15 the speed of typical EHCI implementations (PCI)
 *     + RX, all too often reqpkt seems to misbehave after tx
 *     + TX, no known issues (other than evident silicon issue)
 *
 * - DMA (Mentor/OMAP) ...has at least toggle update problems
 *
 * - [23-feb-2009] minimal traffic scheduling to avoid bulk RX packet
 *   starvation ... nothing yet for TX, interrupt, or bulk.
 *
 * - Not tested with HNP, but some SRP paths seem to behave.
 *
 * NOTE 24-August-2006:
 *
 * - Bulk traffic finally uses both sides of hardware ep1, freeing up an
 *   extra endpoint for periodic use enabling hub + keybd + mouse.  That
 *   mostly works, except that with "usbnet" it's easy to trigger cases
 *   with "ping" where RX loses.  (a) ping to davinci, even "ping -f",
 *   fine; but (b) ping _from_ davinci, even "ping -c 1", ICMP RX loses
 *   although ARP RX wins.  (That test was done with a full speed link.)
 */


/*
 * NOTE on endpoint usage:
 *
 * CONTROL transfers all go through ep0.  BULK ones go through dedicated IN
 * and OUT endpoints ... hardware is dedicated for those "async" queue(s).
 * (Yes, bulk _could_ use more of the endpoints than that, and would even
 * benefit from it.)
 *
 * INTERUPPT and ISOCHRONOUS transfers are scheduled to the other endpoints.
 * So far that scheduling is both dumb and optimistic:  the endpoint will be
 * "claimed" until its software queue is no longer refilled.  No multiplexing
 * of transfers between endpoints, or anything clever.
 */


#include "pmd_m.h"

#include "sxr_ops.h"
#include "halp_sys.h"

#include "usb.h"
#include "halp_usb.h"
#include "halp_debug.h"
#include "halp_version.h"
#include "hal_usb.h"
#include "hal_host.h"
#include "usb_hcd.h"

#include "string.h"

#include "cfg_regs.h"
#include "global_macros.h"
#include "usbc.h"
#include "sys_ctrl.h"
#include "event.h"

#include "musb_core.h"


#define RDA_USB_MAX_END 	16
#define USB_END0_MAX_BUF	64

#define RXFIFOSIZE			64
#define TXFIFOSIZE			48

// endpoint 0 stage
#define USB_END0_STAGE_IDLE			0
#define USB_END0_STAGE_SETUP		1
#define USB_END0_STAGE_IN			2
#define USB_END0_STAGE_OUT			3
#define USB_END0_STAGE_STATUS_IN	4
#define USB_END0_STAGE_STATUS_OUT	5

// endpoint 0 dir
#define USB_END0_DIRIN			0x80

// endpoint type defined by synopsys
#define USB_END_CONTROL		0
#define USB_END_ISO			1
#define USB_END_BULK		2
#define USB_END_INTERRUPT	3
#define USB_STATE_DISCONNECTED 0
#define USB_STATE_CONNECTED    1 
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
 u16 count_MUSB_RXCOUNT=0;
extern UINT8 usb_music_udiskready;
#endif

typedef struct tagUSBENDPOINT
{
	uint8	Epaddress;
	uint8 	type;
	uint16 	nMaxPktSize;
	uint16	nCurTransferred;
	uint8 	DataToggle;
	uint8 	pktCnt;
	USBBULKREQUEST	*pBulkReq;   
}USBENDPOINT;

typedef struct tagUSBHOSTINST
{
	uint8 	dmaEnabled;
	uint8 	end0Stage;
	uint8 	endNum;		/*zero based, 0 for control channel*/
	uint8 	UsbState;
	uint8 	portEnabled;
	uint8	deviceSpeed;
	uint8 	DataToggle;
	uint8 	address;				/*device address set by host*/
	uint8 	dynamicFifoFlag;
	uint16	fifoOffset;
	USBCONTROLREQUEST * pCtrlReq;
	USBENDPOINT epArray[RDA_USB_MAX_END];
	void (*InitCompleteCb)(void *pData, uint8 status);
}USBHOSTINST;

static USBHOSTINST gUsbHostInst;

void musb_host_packet_rx(int chn);
extern int musb_dma_get_cur_epnum();
extern void hal_musb_start();

/*
 * Clear TX fifo. Needed to avoid BABBLE errors.
 */
static void musb_h_tx_flush_fifo(u8 epNum)
{
	u16		csr;
	u16		lastcsr = 0;
	int		retries = 1000;

	csr = musb_readw( MUSB_TXCSR);
	while (csr & MUSB_TXCSR_FIFONOTEMPTY) 
	{
		if (csr != lastcsr)
			lastcsr = csr;
		csr |= MUSB_TXCSR_FLUSHFIFO;
		musb_writew( MUSB_TXCSR, csr);
		csr = musb_readw( MUSB_TXCSR);
		
		if( retries-- < 1 )
		{
			return;
		}
		//mdelay(1);
		hal_TimDelay(16);
	}
}

static u16 musb_h_flush_rxfifo(u8 epnum, u16 csr)
{
	/* we don't want fifo to fill itself again;
	 * ignore dma (various models),
	 * leave toggle alone (may not have been saved yet)
	 */
	csr |= MUSB_RXCSR_FLUSHFIFO | MUSB_RXCSR_RXPKTRDY;
	csr &= ~(MUSB_RXCSR_H_REQPKT
		| MUSB_RXCSR_H_AUTOREQ
		| MUSB_RXCSR_AUTOCLEAR);

	/* write 2x to allow double buffering */
	musb_writew( MUSB_RXCSR, csr);
	musb_writew( MUSB_RXCSR, csr);

	/* flush writebuffer */
	return musb_readw( MUSB_RXCSR);
}


/*
 * Service an RX interrupt for the given IN endpoint; docs cover bulk, iso,
 * and high-bandwidth IN transfer cases.
 */
void musb_host_rx_int(u8 epnum)
{
	u16			rx_csr, val;
	bool			iso_err = FALSE;
	bool			done = FALSE;
	u32			status = 0;

	//hal_HstSendEvent(USB_EVENT, 0x14080730);

	musb_select_ep(epnum);

	rx_csr = musb_readw(MUSB_RXCSR);
	val = rx_csr;
	//hal_HstSendEvent(USB_EVENT, rx_csr);
	
	/* check for errors, concurrent stall & unlink is not really
	 * handled yet! */
	if (rx_csr & MUSB_RXCSR_H_RXSTALL) 
	{
		/* stall; record URB status */
		status = -1;
	}
	else if (rx_csr & MUSB_RXCSR_H_ERROR)
	{
		//status = -2;
		musb_writeb(MUSB_RXINTERVAL, 0);
	} 
	else if (rx_csr & MUSB_RXCSR_DATAERROR) 
	{
		status = -2;
		if (USB_END_BULK != gUsbHostInst.epArray[epnum].type) 
		{
			/* NOTE: NAKing is *NOT* an error, so we want to
			 * continue.  Except ... if there's a request for
			 * another QH, use that instead of starving it.
			 *
			 * Devices like Ethernet and serial adapters keep
			 * reads posted at all times, which will starve
			 * other devices without this logic.
			 */
			if (gUsbHostInst.epArray[epnum].type == USB_END_BULK)
 			{
				//musb_bulk_nak_timeout(epnum, 1);
				return;
			}
			musb_select_ep(epnum);
			rx_csr |= MUSB_RXCSR_H_WZC_BITS;
			rx_csr &= ~MUSB_RXCSR_DATAERROR;
			musb_writew(MUSB_RXCSR, rx_csr);
		}
		else 
		{
			/* packet error reported later */
			iso_err = TRUE;
		}
	} 
	else if (rx_csr & MUSB_RXCSR_INCOMPRX)
	{
		status = -2;
	}
     //mon_Event(0x20160322);
	/* faults abort the transfer */
	 
	u16	count = musb_readw(MUSB_RXCOUNT);
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[epnum];
     #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
     if(usb_music_udiskready==1)
        {
       
        if(count==0x12)
		{
		hal_HstSendEvent(USB_EVENT, 0x20170518);
		return ;
		}
        }
    if(count==0x00)count_MUSB_RXCOUNT++;
    
    if(count_MUSB_RXCOUNT==0x320)
        {
            hal_HstSendEvent(USB_EVENT, 0x20170519);
            return ;
        }
      #endif
	if (status) 
	{
		/* clean up dma and collect transfer count */
		musb_h_flush_rxfifo(epnum, MUSB_RXCSR_CLRDATATOG);
		musb_writeb(MUSB_RXINTERVAL, 0);
		done = TRUE;
		//mon_Event(0x20160525);
		pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USBHCD_ERROR_FAILED);
		pEnd->pBulkReq->completeCb=NULL;
	}
	else
	{
		//unload fifo and request rest data if any.
		pEnd = &gUsbHostInst.epArray[epnum];
		count = musb_readw(MUSB_RXCOUNT);

		if(0) //USB_DMA_ENABLED & pEnd->pktCnt>1)
		{
			//USBBULKREQUEST * pReq = pEnd->pBulkReq;
			//musb_dma_start_rx(pReq->pipe, pReq->nBufLen, pReq->pBuf, pEnd->nMaxPktSize);
		}
		else
		{
			rx_csr &= ~MUSB_RXCSR_RXPKTRDY;
			
			if(pEnd->pBulkReq->nBufLen >= pEnd->nCurTransferred + count)
			{
				musb_read_fifo_ram(epnum, pEnd->pBulkReq->pBuf+pEnd->nCurTransferred, count);
				musb_writew(MUSB_RXCSR, rx_csr);
				//mon_Event(0x20160328);
				//mon_Event(pEnd->pBulkReq->pBuf+pEnd->nCurTransferred);

				pEnd->nCurTransferred += count;
				if(count < pEnd->nMaxPktSize || pEnd->nCurTransferred >= pEnd->pBulkReq->nBufLen)
				{
					*pEnd->pBulkReq->pnByteTransferred = pEnd->nCurTransferred;
					if( pEnd->pBulkReq->completeCb)
					{
						//hal_HstSendEvent(USB_EVENT, 0xffddaacc);
						pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USB_NO_ERROR);
						pEnd->pBulkReq->completeCb=NULL;
					}
				}
				else
				{
					musb_host_packet_rx(epnum);
				}
			}
			else
			{
				// much more data than request , let's flush fifo and complete the request
				// SHOULD NOT HAPPEN
			}
		}
	}
}

int musb_bus_suspend()
{
	return 0;
}

int musb_bus_resume()
{
	/* resuming child port does the work */
	return 0;
}


/**/
static int musb_host_packet_tx(int chn)
{
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[chn];
	uint16 	pktSize = 0;
	uint8 	*pBuf;
	uint16	txCsr;
	
	if(pEnd->pktCnt == 0 || pEnd->nCurTransferred >= pEnd->pBulkReq->nBufLen)
	{
		return 1;
	}

	if(pEnd->pktCnt > 1)
	{
		pktSize = pEnd->nMaxPktSize;
	}
	else
	{
		pktSize = pEnd->pBulkReq->nBufLen - pEnd->nCurTransferred;
	}	
	
    	pBuf = pEnd->pBulkReq->pBuf + pEnd->nCurTransferred;
	musb_write_fifo_ram(chn, pBuf, pktSize);

	txCsr = musb_readw( MUSB_TXCSR);

	txCsr &= ~(MUSB_TXCSR_DMAMODE|MUSB_TXCSR_DMAENAB|MUSB_TXCSR_AUTOSET);
	musb_writew(MUSB_TXCSR, txCsr);
	txCsr |= MUSB_TXCSR_TXPKTRDY;
	musb_writew( MUSB_TXCSR, txCsr);
	
	return 0;
}



//request data from peripheral
void musb_host_packet_rx(int chn)
{
	unsigned short rxCsr = musb_readw( MUSB_RXCSR);

	rxCsr &= ~(MUSB_RXCSR_AUTOCLEAR|MUSB_RXCSR_DMAENAB|MUSB_RXCSR_H_AUTOREQ);
	musb_writew(MUSB_RXCSR, rxCsr);

	rxCsr &= ~MUSB_RXCSR_DMAMODE;
	musb_writew(MUSB_RXCSR, rxCsr);
	
	rxCsr |= MUSB_RXCSR_H_REQPKT;
	musb_writew( MUSB_RXCSR, rxCsr);
}



/* Service a Tx-Available or dma completion irq for the endpoint */
void musb_host_tx_int(u8 epnum)
{
	u16 tx_csr;
	s32 status = 0;

	USBENDPOINT *pEnd = &gUsbHostInst.epArray[epnum];

	//hal_HstSendEvent(USB_EVENT, 0x14081205);
	musb_select_ep(epnum);
	tx_csr = musb_readw(MUSB_TXCSR);

	/* check for errors */
	if (tx_csr & MUSB_TXCSR_H_RXSTALL)
	{
		/* stall; record URB status */
		status = -1;
	}
	else if (tx_csr & MUSB_TXCSR_H_ERROR)
	{
		status = -2;
	}
	else if (tx_csr & MUSB_TXCSR_H_NAKTIMEOUT)
	{
		if ( 1/*bulk type*/) 
		{
		   // hal_HstSendEvent(USB_EVENT, 0x14081205);
			tx_csr = musb_readw(MUSB_TXCSR);
			tx_csr |= MUSB_TXCSR_H_WZC_BITS;
			tx_csr &= ~MUSB_TXCSR_H_NAKTIMEOUT;
			musb_writew(MUSB_TXCSR, tx_csr);
		}
		else 
		{
			/* NOTE:  this code path would be a good place to PAUSE a
			 * transfer, if there's some other (nonperiodic) tx urb
			 * that could use this fifo.  (dma complicates it...)
			 * That's already done for bulk RX transfers.
			 *
			 * if (bulk && qh->ring.next != &musb->out_bulk), then
			 * we have a candidate... NAKing is *NOT* an error
			 */
			//hal_HstSendEvent(USB_EVENT, 0x14081206);
			musb_select_ep(epnum);
			musb_writew( MUSB_TXCSR, MUSB_TXCSR_H_WZC_BITS |MUSB_TXCSR_TXPKTRDY);
		}
		return;
	}

	if (status)
	{

		/* do the proper sequence to abort the transfer in the
		 * usb core; the dma engine should already be stopped.
		 */
		musb_h_tx_flush_fifo(epnum);
		tx_csr &= ~(MUSB_TXCSR_AUTOSET
				| MUSB_TXCSR_DMAENAB
				| MUSB_TXCSR_H_ERROR
				| MUSB_TXCSR_H_RXSTALL
				| MUSB_TXCSR_H_NAKTIMEOUT
				);
        //hal_HstSendEvent(USB_EVENT, 0x14081207);
		musb_select_ep(epnum);
		musb_writew( MUSB_TXCSR, tx_csr);
		/* REVISIT may need to clear FLUSHFIFO ... */
		musb_writew(MUSB_TXCSR, tx_csr);
		musb_writeb(MUSB_TXINTERVAL, 0);

	}
	else
	{
		if(pEnd->pktCnt > 1)
		{
			pEnd->nCurTransferred += pEnd->nMaxPktSize;
		}
		else
		{
			pEnd->nCurTransferred = pEnd->pBulkReq->nBufLen;
		}	
		pEnd->pktCnt--;
       //hal_HstSendEvent(USB_EVENT, 0x14081208);
		if(musb_host_packet_tx(epnum) > 0)
		{
			*pEnd->pBulkReq->pstatus = USB_NO_ERROR;
			*pEnd->pBulkReq->pnByteTransferred = pEnd->nCurTransferred;
			
			if(pEnd->pBulkReq->completeCb)
			{
				//hal_HstSendEvent(USB_EVENT, 0x14082301);
				pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USB_NO_ERROR);
				pEnd->pBulkReq->completeCb = NULL;
			}
		}
	}
}


void musb_host_dma_int(u32 dmaInt)
{
	u16 csr = 0;
	u8 	epnum = musb_dma_get_cur_epnum();
	USBENDPOINT * pEnd = &gUsbHostInst.epArray[epnum];
	
	musb_select_ep(epnum);

	if(pEnd->Epaddress&0x80)
	{
		csr = musb_readw(MUSB_RXCSR);

		hal_HstSendEvent(USB_EVENT, 0x14072510);
		
		if( pEnd->pBulkReq->nBufLen%pEnd->nMaxPktSize)
		{
			pEnd->pktCnt = 1;
			csr &= ~MUSB_RXCSR_RXPKTRDY;
			musb_writew(MUSB_RXCSR, csr);
		}
		else
		{	
			*pEnd->pBulkReq->pstatus = USB_NO_ERROR;
			*pEnd->pBulkReq->pnByteTransferred = pEnd->pBulkReq->nBufLen;
			if( pEnd->pBulkReq->completeCb)
			{
				hal_HstSendEvent(USB_EVENT, 0x14090101);
				pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USB_NO_ERROR);
				pEnd->pBulkReq->completeCb = NULL;
			}	
		}
	
	}
	else
	{
		//hal_HstSendEvent(USB_EVENT, 0x14081901);
		if(pEnd->pBulkReq->nBufLen%pEnd->nMaxPktSize)
		{
			pEnd->pktCnt = 1;
			csr |= MUSB_TXCSR_TXPKTRDY;
			musb_writew(MUSB_TXCSR, csr);
		}
		else
		{
			*pEnd->pBulkReq->pstatus = USB_NO_ERROR;
			*pEnd->pBulkReq->pnByteTransferred = pEnd->pBulkReq->nBufLen;
			if( pEnd->pBulkReq->completeCb)
			{
				 pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USB_NO_ERROR);
				 pEnd->pBulkReq->completeCb = NULL;
			}
		}
		
	}
}

void usb_host_end0_Interrupt()
{
	USBCONTROLREQUEST *pCtrlReq = gUsbHostInst.pCtrlReq;
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[0];
	unsigned short csr0 = 0;

	musb_select_ep(0);
	csr0 = musb_readw( MUSB_CSR0);
	hal_HstSendEvent(USB_EVENT, 0x14062705);
	//hal_HstSendEvent(USB_EVENT, csr0);
	hal_HstSendEvent(USB_EVENT, gUsbHostInst.end0Stage);
	
	if(csr0 & (MUSB_CSR0_H_ERROR | MUSB_CSR0_H_NAKTIMEOUT))
	{
		
	}
	else if(csr0 & MUSB_CSR0_H_RXSTALL)
	{
		
	}
	else
	{
		switch(gUsbHostInst.end0Stage)
		{
		case USB_END0_STAGE_SETUP:
			{
				unsigned short reg = 0;
				USBSTDREQUEST *pStdReq = &pCtrlReq->stdRequest;

				reg = musb_readw( MUSB_CSR0);
				if( pStdReq->wLength > 0 )
				{					
					if( pStdReq->bmRequestType & USB_END0_DIRIN)
					{	
						reg |= MUSB_CSR0_H_REQPKT;
						gUsbHostInst.end0Stage = USB_END0_STAGE_IN;
					}	
					else
					{
						uint8 size = pCtrlReq->nBufLen >= 0x40 ? 0x40: pCtrlReq->nBufLen;
						
						musb_write_fifo_ram(0, pCtrlReq->pBuf, size);
						reg |= MUSB_CSR0_TXPKTRDY;
						gUsbHostInst.end0Stage = USB_END0_STAGE_OUT;
						pEnd->nCurTransferred= size;
					}
					musb_writew( MUSB_CSR0, reg);
				}
				else
				{
					reg |= MUSB_CSR0_H_REQPKT|MUSB_CSR0_H_STATUSPKT;
					musb_writew(MUSB_CSR0, reg);
					gUsbHostInst.end0Stage = USB_END0_STAGE_STATUS_IN;
				}
			}
			break;
		case USB_END0_STAGE_IN:
			{
				UINT8 length = musb_readb( MUSB_COUNT0);
				//hal_HstSendEvent(USB_EVENT, 0x14062703);
				//hal_HstSendEvent(USB_EVENT, pCtrlReq->pBuf);
				//hal_HstSendEvent(USB_EVENT, pEnd->nCurTransferred);
				musb_read_fifo_ram(0, pCtrlReq->pBuf + pEnd->nCurTransferred, length);
				
				if(pCtrlReq->stdRequest.bRequest == USB_GET_DESCRIPTOR && (pCtrlReq->stdRequest.wValue & 1<<8))
				{
					pEnd->nMaxPktSize = pCtrlReq->pBuf[7];
				}
				pEnd->nCurTransferred += length;
				
				csr0 = musb_readw( MUSB_CSR0);

				if(length >= pEnd->nMaxPktSize && pEnd->nCurTransferred<pCtrlReq->nBufLen)
				{
					csr0 |= MUSB_CSR0_H_REQPKT;
					//pEnd->nCurTransferred = pEnd->nMaxPktSize;
					musb_writew( MUSB_CSR0, csr0);
				}
				else
				{
					csr0 |= MUSB_CSR0_TXPKTRDY|MUSB_CSR0_H_STATUSPKT;
					musb_writew(MUSB_CSR0, csr0);
					gUsbHostInst.end0Stage= USB_END0_STAGE_STATUS_OUT;	
				}
			}	
			break;
		case USB_END0_STAGE_OUT:
			{
				csr0 = musb_readw( MUSB_CSR0);
				if(pEnd->nCurTransferred < pCtrlReq->nBufLen)
				{
					uint8 size = pCtrlReq->nBufLen - pEnd->nCurTransferred >= 0x40 ? 0x40: pCtrlReq->nBufLen;

					musb_write_fifo_ram(0, pCtrlReq->pBuf+pEnd->nCurTransferred, size);
					csr0 |= MUSB_CSR0_TXPKTRDY;
					pEnd->nCurTransferred += size;
					musb_writew( MUSB_CSR0, csr0);
				}
				else
				{
					gUsbHostInst.end0Stage= USB_END0_STAGE_STATUS_IN;	
				}
			}
			break;
		case USB_END0_STAGE_STATUS_IN:
			gUsbHostInst.end0Stage = USB_END0_STAGE_IDLE;
			//hal_HstSendEvent(USB_EVENT, 0x14062706);
			musb_writew(MUSB_CSR0, 0);
			if(gUsbHostInst.pCtrlReq->stdRequest.bRequest == USB_SET_ADDRESS)
			{
				gUsbHostInst.address = gUsbHostInst.pCtrlReq->stdRequest.wValue;
				musb_writeb(MUSB_FADDR, gUsbHostInst.address);
			}

			if( gUsbHostInst.pCtrlReq->completeCb )
			{
				gUsbHostInst.pCtrlReq->completeCb(gUsbHostInst.pCtrlReq->pBuf, 0);
				gUsbHostInst.pCtrlReq->completeCb = NULL;
			}
			break;
		case USB_END0_STAGE_STATUS_OUT:
			//hal_HstSendEvent(USB_EVENT, 0x14062702);
			musb_writew(MUSB_CSR0, 0);
			gUsbHostInst.end0Stage = USB_END0_STAGE_IDLE;

			if( gUsbHostInst.pCtrlReq->completeCb )
			{
				gUsbHostInst.pCtrlReq->completeCb(gUsbHostInst.pCtrlReq->pBuf, 0);
				gUsbHostInst.pCtrlReq->completeCb = NULL;
			}
			break;
		case USB_END0_STAGE_IDLE:
			break;
		}
	}	

}
void hal_musb_host_connect(void)
{
	gUsbHostInst.UsbState = USB_STATE_CONNECTED;
}

void hal_musb_host_disconnect(void)
{
	int i = 1;
	for(; i<=gUsbHostInst.endNum; i++)
	{
		//if(gUsbHostInst.epArray[i].pBulkReq->completeCb)
		if(gUsbHostInst.epArray[i].pBulkReq && gUsbHostInst.epArray[i].pBulkReq->completeCb)
		{
			hal_HstSendEvent(USB_EVENT, 0x14090202);
			*gUsbHostInst.epArray[i].pBulkReq->pstatus = USBHCD_DEVICE_DISCONNECTED;
			gUsbHostInst.epArray[i].pBulkReq->completeCb(0, USBHCD_DEVICE_DISCONNECTED);
			gUsbHostInst.epArray[i].pBulkReq->completeCb=NULL;
		}
		
	}
	gUsbHostInst.UsbState = USB_STATE_DISCONNECTED;
	
}



int RDAUsb_BulkTransfer( USBBULKREQUEST * pReq)
{
	USBENDPOINT * pEnd;
	
	if( pReq == NULL )
	{
		return USBHCD_ERROR_FAILED;
	}
	if(gUsbHostInst.UsbState == USB_STATE_DISCONNECTED)
		return USBHCD_DEVICE_DISCONNECTED;
		
	musb_select_ep(pReq->pipe);
	
	pEnd =&gUsbHostInst.epArray[pReq->pipe];
	pEnd->pBulkReq = pReq;
	pEnd->nCurTransferred = 0;
	pEnd->pktCnt = pReq->nBufLen / pEnd->nMaxPktSize + 1; 

	//hal_HstSendEvent(SYS_EVENT, 0x14072601);
	//hal_HstSendEvent(USB_EVENT,  pEnd->Epaddress);
	
	if( pEnd->Epaddress&USB_END0_DIRIN)
	{
		musb_host_packet_rx(pReq->pipe);
	}
	else
	{
		if(USB_DMA_ENABLED && pEnd->pktCnt > 1)
		{
			//musb_dma_start_tx(pReq->pipe, pReq->nBufLen, pReq->pBuf, pEnd->nMaxPktSize);
		}
		else
		{
			musb_host_packet_tx(pReq->pipe);
		}
	}
	return USB_NO_ERROR;
}

USB_PIPE RDAUsb_OpenPipe(USBENDPOINTDESC *pEndDes)
{
	int i = 0;
	u8 reg = 0;
	u16 csr = 0;
	if( (gUsbHostInst.endNum+1 > 15) || pEndDes == NULL)
	{
		return NULL;
	}
	
	i = ++gUsbHostInst.endNum;

	gUsbHostInst.epArray[i].Epaddress = pEndDes->bEndpointAddress;
	gUsbHostInst.epArray[i].nMaxPktSize = pEndDes->wMaxPacketSize;
	gUsbHostInst.epArray[i].type = pEndDes->bmAttributes;

	musb_select_ep(i);

	//config fifo for the end
	
	reg = pEndDes->bEndpointAddress&0x0f;
	reg |= (pEndDes->bmAttributes & 0x03) << 4;		/*end point address(4 bits LSB) and protocol type*/ 
		
	if( pEndDes->bEndpointAddress & 0x80)
	{
		musb_writeb( MUSB_RXTYPE, reg);
		musb_writew( MUSB_RXMAXP, pEndDes->wMaxPacketSize);

		csr = musb_readw( MUSB_RXCSR);
		csr |= MUSB_TXCSR_CLRDATATOG;
		musb_writew( MUSB_RXCSR, csr);

		//enable interrupt
		csr = musb_readw( MUSB_INTRRXE);
		csr |= 1<<i;
		musb_writew( MUSB_INTRRXE, csr);
	}
	else
	{
		csr = musb_readw( MUSB_TXCSR);
		csr |= MUSB_TXCSR_CLRDATATOG | MUSB_TXCSR_MODE;
		musb_writew( MUSB_TXCSR, csr);
		
		musb_writeb( MUSB_TXTYPE, reg);
		musb_writew( MUSB_TXMAXP, pEndDes->wMaxPacketSize);
		musb_writeb(MUSB_TXINTERVAL, 0);
		
		csr = musb_readw( MUSB_INTRTXE);
		csr |= 1<<i;
		musb_writew( MUSB_INTRTXE, csr);
	}
	
	return (USB_PIPE)gUsbHostInst.endNum;
}
extern UINT8 usb_music_udiskready;
int RDAUsb_GetState(void)
{
	 U8 devctl = musb_readb( MUSB_DEVCTL);
	  if((devctl & MUSB_DEVCTL_BDEVICE)||!(devctl&MUSB_DEVCTL_HM))
	 {
		hal_HstSendEvent(USB_EVENT,0x88888885);
		musb_writeb(MUSB_INTRUSBE,0);
		hal_musb_host_disconnect();
		Mcd_UsbHostDeInit(0);
        hal_musb_start();
        usb_music_udiskready=0;
		musb_notify_conn_msg(MUSB_INTR_DISCONNECT);
		//AppGotoMainMenu();
		//hal_TimDelay(500 MS_WAITING);
		return 0;
	  }
	  else
	  {
		return gUsbHostInst.UsbState;
	  }
}

int RDAUsb_ControlTransfer(USBCONTROLREQUEST * pReq)
{
	//int i = 0;
	u8 *pSrc;
	unsigned short reg;
	if(pReq == NULL )
	{
		return USBHCD_ERROR_FAILED;
	}
	//mon_Event(0x20160524);
	//mon_Event(gUsbHostInst.UsbState);
   	if(gUsbHostInst.UsbState == USB_STATE_DISCONNECTED)
		return USBHCD_DEVICE_DISCONNECTED;
	
	musb_select_ep(0);

	gUsbHostInst.pCtrlReq = pReq;
	gUsbHostInst.end0Stage = USB_END0_STAGE_SETUP;
	gUsbHostInst.epArray[0].nCurTransferred = 0;
	
	pSrc = (u8*)(&pReq->stdRequest);
	
	musb_write_fifo_ram(0, pSrc, 8);

	reg = musb_readw( MUSB_CSR0);
	
	reg = MUSB_CSR0_TXPKTRDY|MUSB_CSR0_H_SETUPPKT;
	musb_writew( MUSB_CSR0, reg);

	return USB_NO_ERROR;
}


int RDA_UsbReset(void (*CompleteCb)(void* pData, uint8 status))
{
	int count = 0;
	unsigned char reg;
	
	reg = musb_readb( MUSB_POWER);
	musb_writeb(MUSB_POWER,  reg & ~MUSB_POWER_RESUME);
	hal_TimDelay(20);
	
	do
	{
		reg &=0xf0;  //musb_readb( MUSB_POWER);
		reg |= MUSB_POWER_RESET; 					/* start reset */
		musb_writeb( MUSB_POWER, reg);
		
		//sxr_Sleep(50 MS_WAITING);
		hal_TimDelay(920);
		reg = musb_readb( MUSB_POWER);
		
		reg &= ~MUSB_POWER_RESET; 					/* complete reset */
		musb_writeb( MUSB_POWER, reg);

		hal_TimDelay(1638);
		//sxr_Sleep(100 MS_WAITING);
		count++;
	}while(count < 2);  //count < 3 ); // && !gUsbHostInst.portEnabled);	

	//pmd_RDAWrite(0x00,0x0110);//usb tport
	if( CompleteCb )
	{
		CompleteCb(NULL, 0);
	}
	return 0;
}

void RDA_UsbCoreInit()
{
	unsigned char reg;

	hal_HstSendEvent(SYS_EVENT, 0x14060402);
	musb_select_ep(0);

	reg = musb_readb( MUSB_CONFIGDATA);
	
	if( reg & 0x04)
	{
		gUsbHostInst.dynamicFifoFlag = 1;
		gUsbHostInst.fifoOffset = 0x40;

		musb_writeb(MUSB_TXFIFOSZ, 0x06);		// 64 byte
		musb_writeb(MUSB_RXFIFOSZ, 0x06);		
		musb_writew(MUSB_TXFIFOADD, gUsbHostInst.fifoOffset);
		gUsbHostInst.fifoOffset += 0x200;
		musb_writew(MUSB_RXFIFOADD, gUsbHostInst.fifoOffset);
	}

	//enable the interrupt bit for end0
	musb_writew( MUSB_INTRTXE, 1);
}

int RDA_UsbHostInit(USBOPERATION *pUsbOps, void (*InitCb)(void*pData, uint8 status))
{

	if( pUsbOps != NULL)
	{
		pUsbOps->BulkTransfer = RDAUsb_BulkTransfer;
		pUsbOps->ControlTransfer = RDAUsb_ControlTransfer;
		pUsbOps->OpenPipe = RDAUsb_OpenPipe;
	}

	memset(&gUsbHostInst, 0, sizeof(gUsbHostInst));
	gUsbHostInst.UsbState = USB_STATE_CONNECTED;
	gUsbHostInst.end0Stage = USB_END0_STAGE_IDLE;
	gUsbHostInst.InitCompleteCb = InitCb;
	gUsbHostInst.epArray[0].nMaxPktSize = 0x40;	/*preset to 64 byte, update then we got device descriptor*/
	gUsbHostInst.fifoOffset = 0x40;
	
	RDA_UsbCoreInit();

	if(InitCb != NULL)
	{
		InitCb(NULL, 0);
	}

	return 0;
}


#endif


