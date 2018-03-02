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

////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_usb.c
///
/// Usb Function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "cs_types.h"

#include "global_macros.h"
#include "usbc.h"
#include "cfg_regs.h"
#include "sys_ctrl.h"

#include "bootp_version.h"
#include "bootp_debug.h"
#include "bootp_usb.h"
#include "boot_usb.h"
#include "bootp_mode.h"
#include "boot_map.h"
//#include "bootp_host.h"
#include "hal_usb.h"
#include "hal_sys.h"
#include <string.h>
#include "boot_ispi.h"
#include "iomux.h"
#include "hal_timers.h"
#define MUSB_EP0_FIFOSIZE	64	/* This is non-configurable */
extern void mon_Event(UINT32 event);
// =============================================================================
// MACROS                                                                       
// =============================================================================
/*
 * MUSB Register bits
 */
/* POWER */
#define MUSB_POWER_ISOUPDATE	0x80
#define MUSB_POWER_SOFTCONN	0x40
#define MUSB_POWER_HSENAB	0x20
#define MUSB_POWER_HSMODE	0x10
#define MUSB_POWER_RESET	0x08
#define MUSB_POWER_RESUME	0x04
#define MUSB_POWER_SUSPENDM	0x02
#define MUSB_POWER_ENSUSPEND	0x01

/* INTRUSB */
#define MUSB_INTR_SUSPEND	0x01
#define MUSB_INTR_RESUME	0x02
#define MUSB_INTR_RESET		0x04
#define MUSB_INTR_BABBLE	0x04
#define MUSB_INTR_SOF		0x08
#define MUSB_INTR_CONNECT	0x10
#define MUSB_INTR_DISCONNECT	0x20
#define MUSB_INTR_SESSREQ	0x40
#define MUSB_INTR_VBUSERROR	0x80	/* For SESSION end */

/* DEVCTL */
#define MUSB_DEVCTL_BDEVICE	0x80
#define MUSB_DEVCTL_FSDEV	0x40
#define MUSB_DEVCTL_LSDEV	0x20
#define MUSB_DEVCTL_VBUS	0x18
#define MUSB_DEVCTL_VBUS_SHIFT	3
#define MUSB_DEVCTL_HM		0x04
#define MUSB_DEVCTL_HR		0x02
#define MUSB_DEVCTL_SESSION	0x01

/* MUSB ULPI VBUSCONTROL */
#define MUSB_ULPI_USE_EXTVBUS	0x01
#define MUSB_ULPI_USE_EXTVBUSIND 0x02
/* ULPI_REG_CONTROL */
#define MUSB_ULPI_REG_REQ	(1 << 0)
#define MUSB_ULPI_REG_CMPLT	(1 << 1)
#define MUSB_ULPI_RDN_WR	(1 << 2)

/* TESTMODE */
#define MUSB_TEST_FORCE_HOST	0x80
#define MUSB_TEST_FIFO_ACCESS	0x40
#define MUSB_TEST_FORCE_FS	0x20
#define MUSB_TEST_FORCE_HS	0x10
#define MUSB_TEST_PACKET	0x08
#define MUSB_TEST_K		0x04
#define MUSB_TEST_J		0x02
#define MUSB_TEST_SE0_NAK	0x01
#define MUSB_TXCSR_MODE			0x2000
/* Allocate for double-packet buffering (effectively doubles assigned _SIZE) */
#define MUSB_FIFOSZ_DPB	0x10
/* Allocation size (8, 16, 32, ... 4096) */
#define MUSB_FIFOSZ_SIZE	0x0f

/* CSR0 */
#define MUSB_CSR0_FLUSHFIFO	0x0100
#define MUSB_CSR0_TXPKTRDY	0x0002
#define MUSB_CSR0_RXPKTRDY	0x0001

/* CSR0 in Peripheral mode */
#define MUSB_CSR0_P_SVDSETUPEND	0x0080
#define MUSB_CSR0_P_SVDRXPKTRDY	0x0040
#define MUSB_CSR0_P_SENDSTALL	0x0020
#define MUSB_CSR0_P_SETUPEND	0x0010
#define MUSB_CSR0_P_DATAEND	0x0008
#define MUSB_CSR0_P_SENTSTALL	0x0004

/* CSR0 in Host mode */
#define MUSB_CSR0_H_DIS_PING		0x0800
#define MUSB_CSR0_H_WR_DATATOGGLE	0x0400	/* Set to allow setting: */
#define MUSB_CSR0_H_DATATOGGLE		0x0200	/* Data toggle control */
#define MUSB_CSR0_H_NAKTIMEOUT		0x0080
#define MUSB_CSR0_H_STATUSPKT		0x0040
#define MUSB_CSR0_H_REQPKT		0x0020
#define MUSB_CSR0_H_ERROR		0x0010
#define MUSB_CSR0_H_SETUPPKT		0x0008
#define MUSB_CSR0_H_RXSTALL		0x0004

/* CSR0 bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_CSR0_P_WZC_BITS	\
	(MUSB_CSR0_P_SENTSTALL)
#define MUSB_CSR0_H_WZC_BITS	\
	(MUSB_CSR0_H_NAKTIMEOUT | MUSB_CSR0_H_RXSTALL \
	| MUSB_CSR0_RXPKTRDY)

/* TxType/RxType */
#define MUSB_TYPE_SPEED		0xc0
#define MUSB_TYPE_SPEED_SHIFT	6
#define MUSB_TYPE_PROTO		0x30	/* Implicitly zero for ep0 */
#define MUSB_TYPE_PROTO_SHIFT	4
#define MUSB_TYPE_REMOTE_END	0xf	/* Implicitly zero for ep0 */

/* CONFIGDATA */
#define MUSB_CONFIGDATA_MPRXE		0x80	/* Auto bulk pkt combining */
#define MUSB_CONFIGDATA_MPTXE		0x40	/* Auto bulk pkt splitting */
#define MUSB_CONFIGDATA_BIGENDIAN	0x20
#define MUSB_CONFIGDATA_HBRXE		0x10	/* HB-ISO for RX */
#define MUSB_CONFIGDATA_HBTXE		0x08	/* HB-ISO for TX */
#define MUSB_CONFIGDATA_DYNFIFO		0x04	/* Dynamic FIFO sizing */
#define MUSB_CONFIGDATA_SOFTCONE	0x02	/* SoftConnect */
#define MUSB_CONFIGDATA_UTMIDW		0x01	/* Data width 0/1 => 8/16bits */

/* TXCSR in Peripheral and Host mode */
#define MUSB_TXCSR_AUTOSET		0x8000
#define MUSB_TXCSR_DMAENAB		0x1000
#define MUSB_TXCSR_FRCDATATOG		0x0800
#define MUSB_TXCSR_DMAMODE		0x0400
#define MUSB_TXCSR_CLRDATATOG		0x0040
#define MUSB_TXCSR_FLUSHFIFO		0x0008
#define MUSB_TXCSR_FIFONOTEMPTY		0x0002
#define MUSB_TXCSR_TXPKTRDY		0x0001

/* TXCSR in Peripheral mode */
#define MUSB_TXCSR_P_ISO		0x4000
#define MUSB_TXCSR_P_INCOMPTX		0x0080
#define MUSB_TXCSR_P_SENTSTALL		0x0020
#define MUSB_TXCSR_P_SENDSTALL		0x0010
#define MUSB_TXCSR_P_UNDERRUN		0x0004

/* TXCSR in Host mode */
#define MUSB_TXCSR_H_WR_DATATOGGLE	0x0200
#define MUSB_TXCSR_H_DATATOGGLE		0x0100
#define MUSB_TXCSR_H_NAKTIMEOUT		0x0080
#define MUSB_TXCSR_H_RXSTALL		0x0020
#define MUSB_TXCSR_H_ERROR		0x0004

/* TXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_TXCSR_P_WZC_BITS	\
	(MUSB_TXCSR_P_INCOMPTX | MUSB_TXCSR_P_SENTSTALL \
	| MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_FIFONOTEMPTY)
#define MUSB_TXCSR_H_WZC_BITS	\
	(MUSB_TXCSR_H_NAKTIMEOUT | MUSB_TXCSR_H_RXSTALL \
	| MUSB_TXCSR_H_ERROR | MUSB_TXCSR_FIFONOTEMPTY)

/* RXCSR in Peripheral and Host mode */
#define MUSB_RXCSR_AUTOCLEAR		0x8000
#define MUSB_RXCSR_DMAENAB		0x2000
#define MUSB_RXCSR_DISNYET		0x1000
#define MUSB_RXCSR_PID_ERR		0x1000
#define MUSB_RXCSR_DMAMODE		0x0800
#define MUSB_RXCSR_INCOMPRX		0x0100
#define MUSB_RXCSR_CLRDATATOG		0x0080
#define MUSB_RXCSR_FLUSHFIFO		0x0010
#define MUSB_RXCSR_DATAERROR		0x0008
#define MUSB_RXCSR_FIFOFULL		0x0002
#define MUSB_RXCSR_RXPKTRDY		0x0001

/* RXCSR in Peripheral mode */
#define MUSB_RXCSR_P_ISO		0x4000
#define MUSB_RXCSR_P_SENTSTALL		0x0040
#define MUSB_RXCSR_P_SENDSTALL		0x0020
#define MUSB_RXCSR_P_OVERRUN		0x0004

/* RXCSR in Host mode */
#define MUSB_RXCSR_H_AUTOREQ		0x4000
#define MUSB_RXCSR_H_WR_DATATOGGLE	0x0400
#define MUSB_RXCSR_H_DATATOGGLE		0x0200
#define MUSB_RXCSR_H_RXSTALL		0x0040
#define MUSB_RXCSR_H_REQPKT		0x0020
#define MUSB_RXCSR_H_ERROR		0x0004

/* RXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_RXCSR_P_WZC_BITS	\
	(MUSB_RXCSR_P_SENTSTALL | MUSB_RXCSR_P_OVERRUN \
	| MUSB_RXCSR_RXPKTRDY)
#define MUSB_RXCSR_H_WZC_BITS	\
	(MUSB_RXCSR_H_RXSTALL | MUSB_RXCSR_H_ERROR \
	| MUSB_RXCSR_DATAERROR | MUSB_RXCSR_RXPKTRDY)

/* HUBADDR */
#define MUSB_HUBADDR_MULTI_TT		0x80

#define MUSB_C_NUM_EPS		15

/*
 * Common USB registers
 */

#define MUSB_FADDR		0x00	/* 8-bit */
#define MUSB_POWER		0x01	/* 8-bit */

#define MUSB_INTRTX		0x02	/* 16-bit */
#define MUSB_INTRRX		0x04
#define MUSB_INTRTXE		0x06
#define MUSB_INTRRXE		0x08
#define MUSB_INTRUSB		0x0A	/* 8 bit */
#define MUSB_INTRUSBE		0x0B	/* 8 bit */
#define MUSB_FRAME		0x0C
#define MUSB_INDEX		0x0E	/* 8 bit */
#define MUSB_TESTMODE		0x0F	/* 8 bit */

#define MUSB_FIFO_OFFSET(epnum)	(0x20 + ((epnum) * 4))


/*
 * Additional Control Registers
 */

#define MUSB_DEVCTL		0x60	/* 8 bit */

/* These are always controlled through the INDEX register */
#define MUSB_TXFIFOSZ		0x62	/* 8-bit (see masks) */
#define MUSB_RXFIFOSZ		0x63	/* 8-bit (see masks) */
#define MUSB_TXFIFOADD		0x64	/* 16-bit offset shifted right 3 */
#define MUSB_RXFIFOADD		0x66	/* 16-bit offset shifted right 3 */

/* REVISIT: vctrl/vstatus: optional vendor utmi+phy register at 0x68 */
#define MUSB_HWVERS		0x6C	/* 8 bit */
#define MUSB_ULPI_BUSCONTROL	0x70	/* 8 bit */
#define MUSB_ULPI_INT_MASK	0x72	/* 8 bit */
#define MUSB_ULPI_INT_SRC	0x73	/* 8 bit */
#define MUSB_ULPI_REG_DATA	0x74	/* 8 bit */
#define MUSB_ULPI_REG_ADDR	0x75	/* 8 bit */
#define MUSB_ULPI_REG_CONTROL	0x76	/* 8 bit */
#define MUSB_ULPI_RAW_DATA	0x77	/* 8 bit */

#define MUSB_EPINFO		0x78	/* 8 bit */
#define MUSB_RAMINFO		0x79	/* 8 bit */
#define MUSB_LINKINFO		0x7a	/* 8 bit */
#define MUSB_VPLEN		0x7b	/* 8 bit */
#define MUSB_HS_EOF1		0x7c	/* 8 bit */
#define MUSB_FS_EOF1		0x7d	/* 8 bit */
#define MUSB_LS_EOF1		0x7e	/* 8 bit */

/* Offsets to endpoint registers */
#define MUSB_TXMAXP		0x10
#define MUSB_TXCSR		0x12
#define MUSB_CSR0		MUSB_TXCSR	/* Re-used for EP0 */
#define MUSB_RXMAXP		0x14
#define MUSB_RXCSR		0x16
#define MUSB_RXCOUNT		0x18
#define MUSB_COUNT0		MUSB_RXCOUNT	/* Re-used for EP0 */
#define MUSB_TXTYPE		0x1A
#define MUSB_TYPE0		MUSB_TXTYPE	/* Re-used for EP0 */
#define MUSB_TXINTERVAL		0x1B
#define MUSB_NAKLIMIT0		MUSB_TXINTERVAL	/* Re-used for EP0 */
#define MUSB_RXTYPE		0x1C
#define MUSB_RXINTERVAL		0x1D
#define MUSB_FIFOSIZE		0x1F
#define MUSB_CONFIGDATA		MUSB_FIFOSIZE	/* Re-used for EP0 */


// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================

PROTECTED BOOT_USB_GLOBAL_VAR_T g_BootUsbVar;

// Ep0 Buffer
PRIVATE UINT8 ALIGNED(4) g_BootUsbBufferEp0Out[HAL_USB_MPS];
PRIVATE UINT8 ALIGNED(4) g_BootUsbBufferEp0In [HAL_USB_MPS];

PRIVATE CONST BOOT_USB_GLOBAL_VAR_T CONST *
__attribute__((section (".usbFixptr"),used))
    g_BootUsbFixPtr = &g_BootUsbVar;

 //extern PUBLIC void hal_SysRequestFreq
#define BOOT_MUSB_ADDR	0xa1a80000
static inline u16 musb_readw_rom(UINT16 offset)
	{ return * ((volatile UINT16*)((UINT8*)BOOT_MUSB_ADDR + offset)); }

static inline u32 musb_readl_rom(UINT16 offset)
	{ return *((volatile UINT32*)((UINT8*)BOOT_MUSB_ADDR + offset)); }

static inline void musb_writew_rom(UINT16 offset, UINT16 data)
	{ *((volatile UINT16*)((UINT8*)BOOT_MUSB_ADDR + offset)) = data; }

static inline void musb_writel_rom(UINT16 offset, UINT32 data)
	{ *((volatile UINT32*)((UINT8*)BOOT_MUSB_ADDR + offset)) = data; }

static inline u8 musb_readb_rom(UINT16 offset)
{
	return *( (volatile UINT8*)((UINT8*)BOOT_MUSB_ADDR + offset)); 
}

static inline void musb_writeb_rom(UINT16 offset, UINT8 data)
	{ *((volatile UINT8*)((UINT8*)BOOT_MUSB_ADDR + offset))= data; }


void musb_select_ep_rom(unsigned char endnum)
{
	if(endnum < 15)
	{
		musb_writeb_rom( MUSB_INDEX, endnum);
	} 
}

void musb_write_fifo_rom(unsigned char endnum, unsigned char* pSrc, unsigned short len)
{
	/* byte aligned */
	int i = 0;
	void * fifo  = (unsigned char*)BOOT_MUSB_ADDR+(endnum*4+0x20);
	
	if ((0x01 & (unsigned long)pSrc) == 0)
	{
		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) pSrc) == 0) 
		{
			if (len >= 4) 
			{
				i = len >>2;
				for( ; i > 0; i--)
				{
					*(unsigned int*)fifo	 = *(unsigned int*)pSrc;
					pSrc+=4;
					len -= 4;
				}
			}
			if (len & 0x02) 
			{
				*(unsigned short*)fifo = *(unsigned short*)pSrc;
				pSrc += 2;
				len-=2;
			}
		}
	} 

	for( i = 0; i<len; i++)
	{
		*(unsigned char*)fifo = *pSrc++;
	}
	
}

void musb_read_fifo_rom(unsigned char endnum, unsigned char *dst,  unsigned short len)
{	
	int i = 0;
	void * fifo  = (unsigned char*)BOOT_MUSB_ADDR+(endnum*4+0x20);
	
	if ((0x01 & (unsigned long)dst) == 0)
	{
		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) dst) == 0) 
		{
			if (len >= 4) 
			{
				i = len >>2;
				for( ; i > 0; i--)
				{
					*(unsigned int*)dst = *(unsigned int*)fifo;
					dst+=4;
					len -= 4;
				}
			}
			if (len & 0x02) 
			{
				*(unsigned short*)dst = *(unsigned short*)fifo ;
				dst += 2;
				len-=2;
			}
		}
	} 

	for( i = 0; i<len; i++)
	{
		*dst++ = *(unsigned char*)fifo;
	}
}


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// PRIVATE FUNCTIONS PROTOTYPE                                                  
// =============================================================================
static UINT8 Varvbuserr_retry = 0;
PRIVATE        VOID boot_UsbConfigureEp(HAL_USB_EP_DESCRIPTOR_T* ep);
PRIVATE        VOID boot_UsbDecodeEp0Packet(VOID);
//PRIVATE        VOID boot_getSetupPacket(VOID);
PRIVATE        VOID boot_UsbEnableEp(UINT8 ep, HAL_USB_EP_TYPE_T type);
PRIVATE        VOID boot_UsbDisableEp(UINT8 ep);
PRIVATE        VOID boot_UsbClrConfig(VOID);
INLINE         VOID boot_UsbStatusIn(VOID);
INLINE         VOID boot_UsbStatusOut(VOID);
INLINE         HAL_USB_CALLBACK_RETURN_T
boot_UsbCallbackEp(UINT8                       ep,
                   HAL_USB_CALLBACK_EP_TYPE_T  type,
                   HAL_USB_SETUP_T*            setup);
INLINE         HAL_USB_CALLBACK_RETURN_T
boot_UsbCallbackInterface(UINT8                       ep,
                          HAL_USB_CALLBACK_EP_TYPE_T  type,
                          HAL_USB_SETUP_T*            setup);
PRIVATE UINT16 boot_UsbGenerateDescConfig(HAL_USB_CONFIG_DESCRIPTOR_T* cfg,
                                       UINT8*                       buffer,
                                       UINT8                        num);
PRIVATE UINT16
boot_UsbGenerateDescEp(HAL_USB_EP_DESCRIPTOR_T* ep,
                    UINT8*                   buffer);
PRIVATE UINT16
boot_UsbGenerateDescInterface(HAL_USB_INTERFACE_DESCRIPTOR_T* interface,
                           UINT8*                          buffer,
                           UINT8                           num);

// =============================================================================
// boot_UsbFlushRxFifo
// -----------------------------------------------------------------------------
/// Flux reception USB fifo
// =============================================================================
PRIVATE VOID boot_UsbFlushRxFifo(VOID);

// =============================================================================
// boot_UsbFlushTxFifo
// -----------------------------------------------------------------------------
/// Flush transmition USB FIFO
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
// =============================================================================
PRIVATE VOID boot_UsbFlushTxFifo(UINT8 ep);

// =============================================================================
// boot_UsbFlushAllTxFifos
// -----------------------------------------------------------------------------
/// Flush all transmition USB FIFOS
// =============================================================================
PRIVATE VOID boot_UsbFlushAllTxFifos(VOID);

// =============================================================================
// boot_UsbAsciiToUtf8
// -----------------------------------------------------------------------------
/// Convert string to utf8
/// @param utf8 destination string
/// @param acsii source string
// =============================================================================
PRIVATE VOID boot_UsbAsciiToUtf8(UINT8 *utf8, UINT8 *acsii);

BOOL boot_mgc_usb_highSpeed()
{
    if(musb_readb_rom(MUSB_POWER)&MUSB_POWER_HSMODE)
        return TRUE;
    else
        return FALSE;
}
// =============================================================================
// PRIVATE FUNCTION
// =============================================================================

PRIVATE VOID boot_UsbAsciiToUtf8(UINT8 *utf8, UINT8 *acsii)
{
    while(*acsii)
    {
        *utf8 = *acsii;
        utf8++;
        *utf8 = 0;
        utf8++;
        acsii++;
    }
}

PRIVATE VOID boot_UsbFlushRxFifo(VOID)
{
	UINT32 i;
	u16 csr;
	
	for(i=0; i<15; i++)
	{
		musb_writew_rom(MUSB_INDEX, i);
		
		if(i==0)
		{
			csr = musb_readw_rom(MUSB_CSR0);
			if(csr|MUSB_CSR0_RXPKTRDY)
			{
				csr |= MUSB_CSR0_FLUSHFIFO;
				musb_writew_rom(MUSB_CSR0, csr);
			}
		}
		else
		{
			csr = musb_readw_rom(MUSB_RXCSR);
			if(csr|MUSB_RXCSR_RXPKTRDY)
			{
				csr |= (MUSB_RXCSR_FLUSHFIFO|MUSB_RXCSR_P_WZC_BITS);
				csr |= (MUSB_RXCSR_FLUSHFIFO|MUSB_RXCSR_P_WZC_BITS);
				musb_writew_rom(MUSB_RXCSR, csr);
			}
		}
	}

}

PRIVATE VOID boot_UsbFlushTxFifo(UINT8 ep)
{
    UINT8 epNum;
	u16	txcsr;
    epNum = HAL_USB_EP_NUM(ep);
	musb_writew_rom(MUSB_INDEX, epNum);
	
    if(epNum == 0)
	{
		txcsr = musb_readw_rom(MUSB_CSR0);
        if(txcsr|MUSB_CSR0_TXPKTRDY)
        {
            txcsr |= MUSB_CSR0_FLUSHFIFO;
			musb_writew_rom(MUSB_CSR0, txcsr);
        }
	}
	else
	{
		txcsr = musb_readw_rom(MUSB_TXCSR);
        if(txcsr|MUSB_TXCSR_TXPKTRDY)
        {
            txcsr |= (MUSB_TXCSR_FLUSHFIFO|MUSB_TXCSR_P_WZC_BITS);
			musb_writew_rom(MUSB_TXCSR, txcsr);
        }
	}

}

PRIVATE VOID boot_UsbFlushAllTxFifos(VOID)
{
	UINT8 i;

	for(i=0; i<15; i++)
	{
	    boot_UsbFlushTxFifo(i);
	}

}

PROTECTED VOID boot_UsbResetTransfert(VOID)
{
    	UINT8 i;

	for(i = 0; i < DIEP_NUM+DOEP_NUM+1; ++i)
	{
        g_BootUsbVar.Transfert[i].transferDone = TRUE;
		g_BootUsbVar.Transfert[i].sizeRemaining   = -1;
		g_BootUsbVar.Transfert[i].sizeTransfered  =  0;
		g_BootUsbVar.Transfert[i].buffer =  NULL;
	}


}


// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
// =============================================================================
PROTECTED VOID boot_UsbCancelTransfert(UINT8 ep)
{
	UINT8               epNum;

	epNum = HAL_USB_EP_NUM(ep);
	if(HAL_USB_IS_EP_DIRECTION_IN(ep))
	{
	    g_BootUsbVar.Transfert[epNum].transferDone = TRUE;
        g_BootUsbVar.Transfert[epNum].sizeRemaining = 0;
    	g_BootUsbVar.Transfert[epNum].sizeTransfered  =  0;
    	g_BootUsbVar.Transfert[epNum].buffer =  NULL;
	}
	else
	{        
        g_BootUsbVar.Transfert[DIEP_NUM+epNum].transferDone = TRUE;
        g_BootUsbVar.Transfert[DIEP_NUM+epNum].sizeRemaining = 0;
    	g_BootUsbVar.Transfert[DIEP_NUM+epNum].sizeTransfered  =  0;
    	g_BootUsbVar.Transfert[DIEP_NUM+epNum].buffer =  NULL;
	}

}

PROTECTED UINT8 boot_UsbContinueTransfert(UINT8 ep)
{
    UINT8               epNum;

    epNum = HAL_USB_EP_NUM(ep);

    return(1);
}


PRIVATE VOID boot_UsbConfigureEp(HAL_USB_EP_DESCRIPTOR_T* ep)
{
    UINT8 epNum;

    epNum = HAL_USB_EP_NUM(ep->ep);

    if(HAL_USB_IS_EP_DIRECTION_IN(ep->ep))
    {
        g_BootUsbVar.EpInCallback[epNum-1]  = ep->callback;
        boot_UsbEnableEp(HAL_USB_EP_DIRECTION_IN(epNum), ep->type);
    }
    else
    {
        g_BootUsbVar.EpOutCallback[epNum-1] = ep->callback;
        boot_UsbEnableEp(HAL_USB_EP_DIRECTION_OUT(epNum), ep->type);
    }
}

// =============================================================================
// boot_UsbInit
// -----------------------------------------------------------------------------
/// (Re)Initializes the USB state machine, clear the IRQ and stop the transfers.
// =============================================================================
PRIVATE VOID boot_UsbInit(VOID)
{
    // Flush fifo
    boot_UsbFlushAllTxFifos();
    boot_UsbFlushRxFifo();

    g_BootUsbVar.Ep0State        = EP0_STATE_IDLE;
    g_BootUsbVar.NbString        = 0;
    g_BootUsbVar.usbAddr = 0xff;

    boot_UsbClrConfig();
    g_BootUsbVar.Desc            = 0;

    boot_UsbResetTransfert();
   //*(volatile UINT32*)(0xa1a80084) = 0x3;
}

PRIVATE VOID boot_UsbConfig(VOID)
{
	UINT8 power;
	UINT8 epinfo;

	// Nb EP
	epinfo = musb_readb_rom( MUSB_EPINFO);
	g_BootUsbVar.NbEp = (epinfo&0x0f) + ((epinfo>>4)&0x0f);
	g_BootUsbVar.NbEp++;
	
	//enable HS negotiable 
	power = musb_readb_rom( MUSB_POWER);
	power |=  MUSB_POWER_SOFTCONN; // |  MUSB_POWER_HSENAB;
	musb_writeb_rom( MUSB_POWER, power);

	//tx, rx fifo if dynamic
	musb_select_ep_rom(0);
	g_BootUsbVar.configData = musb_readb_rom( MUSB_CONFIGDATA);
	if(g_BootUsbVar.configData & MUSB_CONFIGDATA_DYNFIFO)
	{
		g_BootUsbVar.FifoAddr = 64;
		musb_writew_rom( MUSB_TXFIFOADD, g_BootUsbVar.FifoAddr>>3);
		musb_writeb_rom( MUSB_TXFIFOSZ, 0x06);		//  512 bytes
		g_BootUsbVar.FifoAddr += 512;
		musb_writew_rom( MUSB_RXFIFOADD, g_BootUsbVar.FifoAddr>>3);
		musb_writew_rom( MUSB_RXFIFOSZ, 0x06);   //  512 bytes
		g_BootUsbVar.FifoAddr += 512;
	}
		
	boot_UsbFlushRxFifo();
	boot_UsbFlushAllTxFifos();
	
}



// =============================================================================
// boot_UsbDisableEp
// -----------------------------------------------------------------------------
/// Disable an endpoint.
///
/// Please note that there is a particular behaviour for endpoint 0.
/// @param ep Endpoint to disable.
// =============================================================================
PRIVATE VOID boot_UsbDisableEp(UINT8 ep)
{
    UINT8   epNum;
    UINT8   offset=0;

    epNum = HAL_USB_EP_NUM(ep);

    // Select ctl register
    if(epNum == 0)
    {

    }
    else
    {

    }

    // Select direction
    if(HAL_USB_IS_EP_DIRECTION_IN(ep))
    {

    }
    else
    {

    }

    // Clear the interruption cause, as we don't care
    // about the past anymore.
    if(epNum == 0)
    {
        // EP0 must always be enabled, so we don't stop
        // its transfer.
    }
    else
    {

    }

    boot_UsbFlushTxFifo(epNum);

    boot_UsbCancelTransfert(ep);

    // We don't disable EP0 interrupt as it is driving the USB
    // stuff. Closing the USB will disable them though.
    if (epNum != 0)
    {
        // Disable interrupt mask for this EP
        g_BootUsbVar.EpInterruptMask &= ~(1<<(epNum+offset));
    }

   // HAL_PROFILE_FUNCTION_EXIT(hal_UsbDisableEp);
}




PRIVATE VOID boot_UsbEnableEp(UINT8 ep, HAL_USB_EP_TYPE_T type)
{
	UINT8   epNum;
	UINT8   offset=0;

	UINT16  maxP, csr;

	epNum = HAL_USB_EP_NUM(ep);
	musb_select_ep_rom(epNum);
	if( boot_mgc_usb_highSpeed() )
	{
		maxP  = HAL_USB_MPS_HS;
	}
	else
	{
		maxP = HAL_USB_MPS;
	}
	
	// Select direction
	if(HAL_USB_IS_EP_DIRECTION_IN(ep))
	{
		musb_writew_rom( MUSB_TXMAXP, maxP);
		csr = musb_readw_rom( MUSB_TXCSR);
		if(type == HAL_USB_EP_TYPE_ISO)
		{
			csr |= MUSB_TXCSR_P_ISO;
		}

		csr |= MUSB_TXCSR_MODE;
		//musb_writew(MUSB_TXCSR, csr);
		csr |=  MUSB_TXCSR_CLRDATATOG;// | MUSB_TXCSR_FLUSHFIFO;// | MUSB_TXCSR_TXPKTRDY;
		musb_writew_rom( MUSB_TXCSR, csr);

		csr = musb_readw_rom( MUSB_INTRTXE);
        csr |= 1<<epNum;
		musb_writew_rom( MUSB_INTRTXE, csr);
	}
	else
	{
		musb_writew_rom( MUSB_RXMAXP, maxP);
		csr = musb_readw_rom( MUSB_RXCSR);
		if(type == HAL_USB_EP_TYPE_ISO)
		{
			csr |= MUSB_RXCSR_P_ISO;
		}
		csr |=  MUSB_RXCSR_CLRDATATOG | MUSB_RXCSR_FLUSHFIFO;// | MUSB_RXCSR_RXPKTRDY;
		musb_writew_rom( MUSB_RXCSR, csr);

		csr = musb_readw_rom( MUSB_INTRRXE);
		csr |= 1<<epNum;
		musb_writew_rom( MUSB_INTRRXE, csr);
	}

	g_BootUsbVar.EpInterruptMask |= (1<<(epNum+offset));

}

INLINE         UINT8 boot_UsbAddString(UINT8 *str)
{
    UINT8 i;

    if(str == 0)
    {
        return(0);
    }
    for(i = 0; i < g_BootUsbVar.NbString; ++i)
    {
        if(g_BootUsbVar.String[i] == str)
        {
            return(i+1);
        }
    }
    g_BootUsbVar.String[g_BootUsbVar.NbString] = str;
    g_BootUsbVar.NbString++;
    return(g_BootUsbVar.NbString);
}


PRIVATE VOID boot_UsbGenerateDescDevice(HAL_USB_DEVICE_DESCRIPTOR_T* dev,
                                     UINT8*                       buffer)
{
    BOOT_USB_DEVICE_DESCRIPTOR_REAL_T* desc;
    UINT8                             i;

    desc = (BOOT_USB_DEVICE_DESCRIPTOR_REAL_T*) buffer;
    desc->size          = sizeof(BOOT_USB_DEVICE_DESCRIPTOR_REAL_T);
    // Device type = 1
    desc->type          = 1;
    desc->bcdUsb        = 0x0110;
    desc->usbClass      = dev->usbClass;
    desc->usbSubClass   = dev->usbSubClass;
    desc->usbProto      = dev->usbProto;
    desc->ep0Mps        = HAL_USB_MPS;
    if(dev->vendor == 0)
    {
        desc->vendor    = BOOT_USB_VENDOR_ID;
    }
    else
    {
        desc->vendor    = dev->vendor;
    }
    if(dev->product == 0)
    {
        desc->product   =   0x0900; //BOOT_USB_PRODUCT_ID(dev->usbMode);
    }
    else
    {
        desc->product   =  0x0900; //dev->product;
    }
    if(dev->version == 0)
    {
        desc->release   = 0x3410; //BOOT_USB_REVISION_ID;
    }
    else
    {
        desc->release   = 0x3410; // dev->version;
    }
    desc->iManufacturer = boot_UsbAddString((UINT8*)"rdamicro inc.");
    desc->iProduct      = boot_UsbAddString(dev->description);
    desc->iSerial       = boot_UsbAddString(dev->serialNumber);
    i = 0;
    if(dev->configList)
    {
        for(i = 0; dev->configList[i]; ++i);
    }
    desc->nbConfig          = i;
}

PRIVATE UINT16 boot_UsbGenerateDescConfig(HAL_USB_CONFIG_DESCRIPTOR_T* cfg,
                                          UINT8*                       buffer,
                                          UINT8                        num)
{
    BOOT_USB_CONFIG_DESCRIPTOR_REAL_T* desc;
    UINT16                             size;
    UINT8                              i;

    desc = (BOOT_USB_CONFIG_DESCRIPTOR_REAL_T*) buffer;
    desc->size         = sizeof(BOOT_USB_CONFIG_DESCRIPTOR_REAL_T);
    size               = sizeof(BOOT_USB_CONFIG_DESCRIPTOR_REAL_T);
    // Config type = 2
    desc->type         = 2;

    i = 0;
    if(cfg->interfaceList)
    {
        for(i = 0; cfg->interfaceList[i]; ++i)
        {
            size += boot_UsbGenerateDescInterface(cfg->interfaceList[i],
                                               &buffer[size], desc->nbInterface );
				if(cfg->interfaceList[i]->interfaceIdx != 0xFF)
				{
                    ++desc->nbInterface;
                }
        }
    }
    desc->nbInterface  = i;
    desc->configIndex  = cfg->configIdx;
    desc->iDescription = boot_UsbAddString(cfg->description);
    desc->attrib       = cfg->attrib;
    desc->maxPower     = cfg->maxPower;
    desc->totalLength  = size;
    return(size);
}

PRIVATE UINT16
boot_UsbGenerateDescInterface(HAL_USB_INTERFACE_DESCRIPTOR_T* interface,
                           UINT8*                          buffer,
                           UINT8                           num)
{
    UINT8                                 i;
    BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T* desc;
    UINT16                                size;

    desc = (BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T*) buffer;

    desc->size              = sizeof(BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T);
    size                    = sizeof(BOOT_USB_INTERFACE_DESCRIPTOR_REAL_T);
    // Interface type = 4
    desc->type              = 4;
    desc->interfaceIndex    = 0;
    if(interface->interfaceIdx)
    {
        desc->interfaceIndex = interface->interfaceIdx;
    }
    desc->alternateSetting  = 0;
    i = 0;
    if(interface->epList)
    {
        for(i = 0; interface->epList[i]; ++i)
        {
            size           += boot_UsbGenerateDescEp(interface->epList[i],
                                                  &buffer[size]);
        }
    }
    desc->nbEp              = i;
    desc->usbClass          = interface->usbClass   ;
    desc->usbSubClass       = interface->usbSubClass;
    desc->usbProto          = interface->usbProto   ;
    desc->iDescription      = boot_UsbAddString(interface->description);

    return(size);
}

PRIVATE UINT16
boot_UsbGenerateDescEp(HAL_USB_EP_DESCRIPTOR_T* ep,
                    UINT8*                   buffer)
{
    BOOT_USB_EP_DESCRIPTOR_REAL_T* desc;

    desc = (BOOT_USB_EP_DESCRIPTOR_REAL_T*) buffer;

    desc->size       = sizeof(BOOT_USB_EP_DESCRIPTOR_REAL_T);
    // EP type = 5
    desc->type       = 5;
    desc->ep         = ep->ep;
    // EP Type (Cmd, Bulk, Iso, Int)
    desc->attributes = ep->type;
    desc->mps        = HAL_USB_MPS;
    desc->interval   = ep->interval;

    return(desc->size);
}


INLINE         VOID boot_UsbStatusIn(VOID)
{
	g_BootUsbVar.Ep0State = EP0_STATE_STATUS_IN;
}

INLINE         VOID boot_UsbStatusOut(VOID)
{
	g_BootUsbVar.Ep0State = EP0_STATE_STATUS_OUT;
}


// =============================================================================
// boot_UsbClrConfig
// -----------------------------------------------------------------------------
/// Clear the USB config. Stop the transfer on the endpoint (but ep0) and clear
/// the interrupts (but ep0).
// =============================================================================
PRIVATE VOID boot_UsbClrConfig(VOID)
{
    UINT8 i;

    if(g_BootUsbVar.Desc)
    {
        if(g_BootUsbVar.Config != 0xFF  &&
           g_BootUsbVar.Desc->configList[g_BootUsbVar.Config] &&
           g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]->interfaceList)
        {
            for(i = 0;
                g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]
                    ->interfaceList[i];
                ++i)
            {
                if(g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]
                   ->interfaceList[i]->callback)
                {
                    g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]
                        ->interfaceList[i]->callback
                        (HAL_USB_CALLBACK_TYPE_DISABLE, 0);
                }
            }
        }

        // Ask lilian why he did that at revision 15516 in hal_usb.c.
        g_BootUsbVar.Config = 0xFF;
    }

    // Disable all EP
    for(i = 0; i < DIEP_NUM; ++i)
    {
        g_BootUsbVar.EpInCallback[i]  = 0;
        boot_UsbDisableEp(HAL_USB_EP_DIRECTION_IN(i+1));
    }
    for(i = 0; i < DOEP_NUM; ++i)
    {
        g_BootUsbVar.EpOutCallback[i] = 0;
        boot_UsbDisableEp(HAL_USB_EP_DIRECTION_OUT(i+1));
    }
 
    // Endpoint 0 doesn't have endpoint callbacks other 
    // than the IRQ handlers...
    boot_UsbDisableEp(HAL_USB_EP_DIRECTION_IN(0));
    boot_UsbDisableEp(HAL_USB_EP_DIRECTION_OUT(0));
}

INLINE         HAL_USB_CALLBACK_RETURN_T
boot_UsbCallbackEp(UINT8                      ep,
                   HAL_USB_CALLBACK_EP_TYPE_T type,
                   HAL_USB_SETUP_T*           setup)
{
    UINT8                     epNum;

    epNum = HAL_USB_EP_NUM(ep);

    if(epNum < 16 &&
       epNum != 0)
    {
        if(HAL_USB_IS_EP_DIRECTION_IN(ep))
        {
            if(g_BootUsbVar.EpInCallback[epNum-1])
            {
                return(g_BootUsbVar.EpInCallback[epNum-1](type, setup));
            }
        }
        else
        {
            if(g_BootUsbVar.EpOutCallback[epNum-1])
            {
                return(g_BootUsbVar.EpOutCallback[epNum-1](type, setup));
            }
        }
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}

INLINE         HAL_USB_CALLBACK_RETURN_T
boot_UsbCallbackInterface(UINT8 interface,
                          HAL_USB_CALLBACK_EP_TYPE_T type,
                          HAL_USB_SETUP_T*           setup)
{
    if(g_BootUsbVar.Desc == 0)
    {
        return(HAL_USB_CALLBACK_RETURN_KO);
    }
    if(g_BootUsbVar.Config == 0xFF)
    {
        return(HAL_USB_CALLBACK_RETURN_KO);
    }
    if(g_BootUsbVar.Desc->configList[g_BootUsbVar.Config] == 0)
    {
        return(HAL_USB_CALLBACK_RETURN_KO);
    }
    if(interface >= g_BootUsbVar.NbInterface)
    {
        return(HAL_USB_CALLBACK_RETURN_KO);
    }
    if(g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]->
       interfaceList[interface]->callback == 0)
    {
        return(HAL_USB_CALLBACK_RETURN_KO);
    }
    return(g_BootUsbVar.Desc->configList[g_BootUsbVar.Config]->
           interfaceList[interface]->
           callback(type, setup));
}

PRIVATE VOID boot_UsbSetConfig(UINT8 num)
{
    UINT8 i = 0;
    UINT8 j;

	//return ;

    // Disable old config
    boot_UsbClrConfig();

    g_BootUsbVar.Config      = 0xFF;
    g_BootUsbVar.NbInterface = 0;

    // Search config index
    for(i = 0; g_BootUsbVar.Desc->configList[i]; ++i) 
	{
        if(g_BootUsbVar.Desc->configList[i]->configIdx == num)
        {
            break;
        }
    }
    if(g_BootUsbVar.Desc->configList[i] == 0)
    {
        return;
    }
    // Save config index
    g_BootUsbVar.Config = i;
    num                = i;

    // Configure all interface end EP of this config
    if(g_BootUsbVar.Desc->configList[num]->interfaceList)
    {
        for(i = 0; g_BootUsbVar.Desc->configList[num]->interfaceList[i]; ++i)
        {
            for(j = 0;
                g_BootUsbVar.Desc->configList[num]->interfaceList[i]->epList[j];
                ++j)
            {
                boot_UsbConfigureEp(g_BootUsbVar.Desc->configList[num]->
                                    interfaceList[i]->epList[j]);
            }
            if(g_BootUsbVar.Desc->configList[num]->interfaceList[i]->callback)
            {
                g_BootUsbVar.Desc->configList[num]->
                    interfaceList[i]->callback(HAL_USB_CALLBACK_TYPE_ENABLE, 0);
            }
        }
        g_BootUsbVar.NbInterface = i;
    }
}

PRIVATE VOID boot_UsbDecodeEp0Packet(VOID)
{
    HAL_USB_SETUP_T* setup;
    UINT16           size;
    UINT8            setup_completed = 0;
	UINT16		csr0;
   // UINT32 i=0;
	musb_select_ep_rom(0);
	size = musb_readw_rom( MUSB_COUNT0);
	csr0 = musb_readw_rom( MUSB_CSR0);
	if(g_BootUsbVar.Ep0State==EP0_STATE_IDLE)
	{
     if(musb_readw_rom(MUSB_CSR0) & MUSB_CSR0_RXPKTRDY)
     {}
     else
     {
       return;
     }
	}

	if(csr0 & MUSB_CSR0_P_SETUPEND)
	{
		musb_writew_rom(MUSB_CSR0, MUSB_CSR0_P_SVDSETUPEND);
		/* Transition into the early status phase */
		switch (g_BootUsbVar.Ep0State) {
		case EP0_STATE_IN:
			g_BootUsbVar.Ep0State = EP0_STATE_STATUS_OUT;
			break;
		case EP0_STATE_OUT:
			g_BootUsbVar.Ep0State = EP0_STATE_STATUS_IN;
			break;
		default:
			break;
		}
		csr0 = musb_readw_rom(MUSB_CSR0);
	}

    switch(g_BootUsbVar.Ep0State)
    {
    case EP0_STATE_IDLE  :
        // New setup packet
		musb_read_fifo_rom(0, g_BootUsbBufferEp0Out, size);
		setup = (HAL_USB_SETUP_T*) (g_BootUsbBufferEp0Out);
        // Change endian less
        setup->value     = HAL_ENDIAN_LITTLE_16(setup->value );
        setup->index     = HAL_ENDIAN_LITTLE_16(setup->index );
        setup->lenght    = HAL_ENDIAN_LITTLE_16(setup->lenght);

        g_BootUsbVar.RequestDesc = setup->requestDesc;
        g_BootUsbVar.Ep0Index    = setup->index&0xff;
      // mon_Event((UINT32*) setup);
		musb_writew_rom(MUSB_CSR0, MUSB_CSR0_P_SVDRXPKTRDY);
		while(musb_readw_rom(MUSB_CSR0) & MUSB_CSR0_RXPKTRDY);
        if(setup->lenght == 0)
        {
            // No data
            if(setup->requestDesc.requestDirection)
            {
                
                g_BootUsbVar.Ep0State = EP0_STATE_STATUS_OUT;
            }
            else
            {
                 
                g_BootUsbVar.Ep0State = EP0_STATE_STATUS_IN;
            }
        }
        else
        {
            // Command with data
            if(setup->requestDesc.requestDirection)
            {
                g_BootUsbVar.Ep0State = EP0_STATE_IN;
            }
            else
            {
                g_BootUsbVar.Ep0State = EP0_STATE_OUT;
            }
        }

        switch(setup->requestDesc.requestDest)
        {
        case BOOT_USB_REQUEST_DESTINATION_DEVICE:
            switch(setup->request)
            {
            case BOOT_USB_REQUEST_DEVICE_SETADDR:
			g_BootUsbVar.usbAddr = setup->value & 0xff;
			setup_completed = 1;
                break;
            case BOOT_USB_REQUEST_DEVICE_SETCONF:
                // Select one config
                if(g_BootUsbVar.Desc == 0)
                {
                    break;
                }
                    setup_completed = 1;
                    boot_UsbSetConfig(setup->value&0xFF);
                break;
            case BOOT_USB_REQUEST_DEVICE_GETDESC:
                // Fetch device descriptor
                if(g_BootUsbVar.Desc == 0)
                {
                    break;
                }
                size = 0;
                switch(setup->value>>8)
                {
                case 1: /* Device */
                    boot_UsbGenerateDescDevice(g_BootUsbVar.Desc,g_BootUsbBufferEp0In);
                    size = sizeof(BOOT_USB_DEVICE_DESCRIPTOR_REAL_T);
                    break;
                case 2: /* Config */
                    size = boot_UsbGenerateDescConfig(
                        g_BootUsbVar.Desc->configList[(setup->value&0xFF)],
                        g_BootUsbBufferEp0In, setup->value&0xFF);
                    break;
                case 3: /* String */
                    if((setup->value&0xFF) == 0)
                    {
                        // Set caracter table to 0x0409
                        size                    = 0x04;
                        g_BootUsbBufferEp0In[0] = 0x04;
                        g_BootUsbBufferEp0In[1] = 0x03;
                        g_BootUsbBufferEp0In[2] = 0x09;
                        g_BootUsbBufferEp0In[3] = 0x04;
                    }
                    else
                    {
                        // Select the string and send it
                        size = 0;
                        if((setup->value&0xFF) <= g_BootUsbVar.NbString)
                        {
                            size = strlen((INT8 *)g_BootUsbVar.String[(setup->value&0xFF)-1])*2 + 2;
                            g_BootUsbBufferEp0In[0] = size;
                            g_BootUsbBufferEp0In[1] = 0x03;
                            boot_UsbAsciiToUtf8(&g_BootUsbBufferEp0In[2],g_BootUsbVar.String[(setup->value&0xFF)-1]);
						}
                    }
                    break;
                }
                if(setup->lenght < size)
                {
                    size = setup->lenght;
                }
                /* Data in */
                boot_UsbSend(0, g_BootUsbBufferEp0In, size, 0);
                setup_completed = 1;
                break;
            }
            break;
        case BOOT_USB_REQUEST_DESTINATION_INTERFACE:
            switch(boot_UsbCallbackInterface(setup->index&0xFF,HAL_USB_CALLBACK_TYPE_CMD, setup))
            {
            case HAL_USB_CALLBACK_RETURN_OK:
                setup_completed = 1;
                break;
            case HAL_USB_CALLBACK_RETURN_RUNNING:
                setup_completed = 2;
                break;
            case HAL_USB_CALLBACK_RETURN_KO:
                break;
            }
            break;
        case BOOT_USB_REQUEST_DESTINATION_EP:
            switch(setup->request)
            {
            case BOOT_USB_REQUEST_EP_GET_STATUS   :
                break;
            case BOOT_USB_REQUEST_EP_CLEAR_FEATURE:
                if(setup->value == 0 || setup->value & 0x01)
                {
                    //boot_UsbEpStall(setup->index&0xFF, FALSE);
                }
                setup_completed = 1;
                break;
            case BOOT_USB_REQUEST_EP_SET_FEATURE  :
                break;
            default:
                break;
            }
            switch(boot_UsbCallbackEp(setup->index&0xFF,HAL_USB_CALLBACK_TYPE_CMD, setup))
            {
            case HAL_USB_CALLBACK_RETURN_OK:
                setup_completed = 1;
                break;
            case HAL_USB_CALLBACK_RETURN_RUNNING:
                setup_completed = 2;
                break;
            case HAL_USB_CALLBACK_RETURN_KO:
                break;
            }
            break;
        default:
            break;
        }
        if(setup_completed == 0 || setup_completed == 1)
        {
            
			// Command finish. Ready to receive status.
		if(setup->lenght == 0)
		{
		    
			csr0 = musb_readw_rom(MUSB_CSR0);
			csr0 |=  MUSB_CSR0_P_DATAEND;   //MUSB_CSR0_P_SVDRXPKTRDY
			musb_writew_rom( MUSB_CSR0, csr0);
			boot_UsbStatusIn();
		}
		else
		{	
			if(setup->requestDesc.requestDirection)
			{
				boot_UsbStatusOut();
			}
			else
			{
			    
				boot_UsbStatusIn();
			}
		}
        }
        if(setup_completed == 0)
        {
            if(setup->lenght != 0)
            {
                // Command success and data must be sent

                if(setup->requestDesc.requestDirection)
                {
                    boot_UsbSend(0, g_BootUsbBufferEp0In, 0, 0);
                }
                else
                {
			//waiting for data int
                }
            }
        }
        break;
    case EP0_STATE_IN    :
        /* Transfert finish */
        if(0)
        {
            switch(g_BootUsbVar.RequestDesc.requestDest)
            {
            case BOOT_USB_REQUEST_DESTINATION_DEVICE:
                break;
            case BOOT_USB_REQUEST_DESTINATION_INTERFACE:
                boot_UsbCallbackInterface(g_BootUsbVar.Ep0Index&0xFF,
                                          HAL_USB_CALLBACK_TYPE_DATA_CMD,
                                          setup);
                break;
            case BOOT_USB_REQUEST_DESTINATION_EP:
                boot_UsbCallbackEp(g_BootUsbVar.Ep0Index&0xFF,
                                   HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
                break;
            }
            boot_UsbStatusOut();
        }
        break;
    case EP0_STATE_OUT   :
		/* Transfert finish */
		if(0)
		{
			switch(g_BootUsbVar.RequestDesc.requestDest)
			{
			case BOOT_USB_REQUEST_DESTINATION_DEVICE:
				break;
			case BOOT_USB_REQUEST_DESTINATION_INTERFACE:
				boot_UsbCallbackInterface(g_BootUsbVar.Ep0Index&0xFF,
				                      HAL_USB_CALLBACK_TYPE_DATA_CMD,
				                      setup);
				break;
			case BOOT_USB_REQUEST_DESTINATION_EP:
				boot_UsbCallbackEp(g_BootUsbVar.Ep0Index&0xFF,
				               HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
				break;
			}
			boot_UsbStatusIn();
		}
		break;
    case EP0_STATE_STATUS_IN:		
		if(musb_readb_rom(MUSB_FADDR) == 0 && g_BootUsbVar.usbAddr != 0xff)
		{
			musb_writeb_rom( MUSB_FADDR, g_BootUsbVar.usbAddr & 0x7f);
		}
		//g_BootUsbVar.Ep0State = EP0_STATE_IDLE;
           
        	//break;
    case EP0_STATE_STATUS_OUT:		
		g_BootUsbVar.Ep0State = EP0_STATE_IDLE;
		if(csr0 != 0)
		{
            boot_UsbDecodeEp0Packet();
		}
         
        	break;
    }
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

PUBLIC VOID boot_SetIOMux(UINT32 bootMode)
{
	UINT32 *pIOMux = (UINT32*)g_BootUsbBufferEp0Out;
	if(pIOMux[0] != BOOT_SECTOR_IOMUX_VALID_TAG)
		Varvbuserr_retry = 0;
	
	{
		for(int i=0;i<Varvbuserr_retry;i+=2)
			*((UINT32*)pIOMux[i+1]) = pIOMux[i+2];
	}
}

PUBLIC VOID boot_usbinitclockenable(VOID)
{
   
hwp_sysCtrl->REG_DBG        = SYS_CTRL_PROTECT_UNLOCK;
  
  hwp_sysCtrl->Sel_Clock  |= SYS_CTRL_DIGEN_H_ENABLE ;         //Enable digital 52M clock.
  hwp_sysCtrl->Cfg_Clk_Sys = (SYS_CTRL_FREQ_52M|SYS_CTRL_FORCE_DIV_UPDATE);               //following siwtch 52M 
  hwp_sysCtrl->Pll_Ctrl  &= ~(SYS_CTRL_PLL_ENABLE|SYS_CTRL_PLL_LOCK_RESET) ;
  hwp_sysCtrl->Pll_Ctrl  |= SYS_CTRL_PLL_ENABLE ;           //pll_enable
  hwp_sysCtrl->Pll_Ctrl  |= SYS_CTRL_PLL_CLK_FAST_ENABLE ; //pll_clock_fast_enable
  hwp_sysCtrl->Pll_Ctrl  &=~SYS_CTRL_BB_PLL_BYPASS ;      //pll_bypass
  hwp_sysCtrl->Pll_Ctrl  |= SYS_CTRL_PLL_LOCK_RESET ;    //pll_lock_reset
  
  while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));
  
  hwp_sysCtrl->Sel_Clock  &=~SYS_CTRL_SYS_SEL_FAST_SLOW ;  //Sys_sel_Fast 0
  hwp_sysCtrl->Sel_Clock  |= SYS_CTRL_PLL_BYPASS_LOCK ;   //pll bypass lock 1
 
  hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_USBPHY;//enable clk_usbphy
  hwp_sysCtrl->usb_cfg    |= SYS_CTRL_USB_PLL_PU;// usb_pll_pu

if(Varvbuserr_retry == 0)
	{
	//hwp_iomux->IOMUX_PXX_CTRL[0][0] &= 0xfffffff0;//GPIO cfg FUNCTION 6 iomux p00 
	hwp_iomux->IOMUX_PXX_CTRL[0][0] = 6;

	//hwp_iomux->IOMUX_PXX_CTRL[0][1] &= 0xfffffff0;//GPIO cfg FUNCTION 6 iomax p01 
	hwp_iomux->IOMUX_PXX_CTRL[0][1] = 6;
}
  u8  devcth = musb_readb_rom(MUSB_POWER);//open POWER_HSMODE shaohuima 2015/10/29 
  devcth |= MUSB_POWER_HSMODE;
  musb_writeb_rom(MUSB_POWER, devcth);	
  // 
  #if 0
	hwp_iomux->IOMUX_PXX_CTRL[0][0] |= IOMUX_PXX_ENABLE_FORCE;//P00 force enable
    hwp_iomux->IOMUX_PXX_CTRL[0][0] |= IOMUX_PXX_DIR_FORCE;//Combine with p00_enable_force, P00 force direction: 0-out, 1-in
	hwp_iomux->IOMUX_PXX_CTRL[0][0] &= ~IOMUX_PXX_UP_ACTIVE_FORCE;//Combine with p00_enable_force, P00 force pull-up active:  0-inactive, 1-active
	hwp_iomux->IOMUX_PXX_CTRL[0][0] &= ~IOMUX_PXX_DN_ACTIVE_FORCE;//Combine with p00_enable_force, P00 force  pull-down active: 0-inactive, 1-active 

	
    hwp_iomux->IOMUX_PXX_CTRL[0][1] |= IOMUX_PXX_ENABLE_FORCE;//P01 force enable
    hwp_iomux->IOMUX_PXX_CTRL[0][1] |= IOMUX_PXX_DIR_FORCE;//Combine with p01_enable_force, P01 force direction: 0-out, 1-in
	hwp_iomux->IOMUX_PXX_CTRL[0][1] &= ~IOMUX_PXX_UP_ACTIVE_FORCE;//Combine with p01_enable_force, P01 force pull-up active:  0-inactive, 1-active
	hwp_iomux->IOMUX_PXX_CTRL[0][1] &= ~IOMUX_PXX_DN_ACTIVE_FORCE;//Combine with p01_enable_force, P01 force  pull-down active: 0-inactive, 1-active 
#endif
	/*UINT32  drreg=0xffff;                                       //pu_pll_dr,pu_pll_reg
	        drreg|=((1<<16)|(1<<17));  
    boot_IspiSendData(BOOT_ISPI_CS_USB,drreg, FALSE);
    */
 hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;
}




PUBLIC VOID boot_UsbInitVar(VOID)
{
    g_BootUsbVar.DeviceCallback = 0;
    g_BootUsbVar.Desc           = 0;
    g_BootUsbVar.NbEp           = 0;
    g_BootUsbVar.NbInterface    = 0;
    g_BootUsbVar.NbString       = 0;
    g_BootUsbVar.Ep0State       = EP0_STATE_IDLE;
    g_BootUsbVar.Config         = 0xFF;
}


void boot_musb_g_resume()
{
	// if () callback.... 
	{
		//notify device driver
	}
}

static void boot_musb_stage0_irq(u8 int_usb, u8 devctl)
{
	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */
	if (int_usb & MUSB_INTR_RESUME) 
	{
		if ((devctl & MUSB_DEVCTL_VBUS) != (3 << MUSB_DEVCTL_VBUS_SHIFT) ) 
		{
			int_usb |= MUSB_INTR_DISCONNECT;
			int_usb &= ~MUSB_INTR_SUSPEND;
		}
		else
		{
			boot_musb_g_resume();
		}
	}

	if (int_usb & MUSB_INTR_VBUSERROR) 
	{
		//int	ignore = 0;

		/* During connection as an A-Device, we may see a short
		 * current spikes causing voltage drop, because of cable
		 * and peripheral capacitance combined with vbus draw.
		 * (So: less common with truly self-powered devices, where
		 * vbus doesn't act like a power supply.)
		 *
		 * Such spikes are short; usually less than ~500 usec, max
		 * of ~2 msec.  That is, they're not sustained overcurrent
		 * errors, though they're reported using VBUSERROR irqs.
		 *
		 * Workarounds:  (a) hardware: use self powered devices.
		 * (b) software:  ignore non-repeated VBUS errors.
		 *
		 * REVISIT:  do delays from lots of DEBUG_KERNEL checks
		 * make trouble here, keeping VBUS < 4.4V ?
		 */

		/* go through A_WAIT_VFALL then start a new session */
		//if (!ignore)
			//musb_platform_set_vbus(musb, 0);
	}

	if (int_usb & MUSB_INTR_SUSPEND) 
	{
		//musb_g_suspend(musb);
	}

	if (int_usb & MUSB_INTR_CONNECT) 
	{

		/* high vs full speed is just a guess until after reset */
		//if (devctl & MUSB_DEVCTL_LSDEV)
			//g_HalUsbVar->port1_status |= USB_PORT_STAT_LOW_SPEED;

		/* indicate new connection to OTG machine */
		if (int_usb & MUSB_INTR_SUSPEND) 
		{
			int_usb &= ~MUSB_INTR_SUSPEND;
		}
	}

	if (int_usb & MUSB_INTR_DISCONNECT)
	{
		//musb_g_disconnect(musb);
	}

	/* mentor saves a bit: bus reset and babble share the same irq.
	 * only host sees babble; only peripheral sees bus reset.
	 */
	if (int_usb & MUSB_INTR_RESET) 
			{
		if(g_BootUsbVar.DeviceCallback != 0)
		{
			g_BootUsbVar.Desc = g_BootUsbVar.DeviceCallback(TRUE);
			}
			g_BootUsbVar.Ep0State        = EP0_STATE_IDLE;
			g_BootUsbVar.NbString        = 0;
			g_BootUsbVar.usbAddr = 0xff;
	//	}
	}

	return ;
}

void boot_musb_g_rx(u8 epnum)
{
	u16			csr;
//	u32 			status;
	u16			rxcount;
	u16         rxMaxP;
	BOOT_USB_TRANSFERT_T *pTransfer;
	musb_select_ep_rom(epnum);

	csr = musb_readw_rom(MUSB_RXCSR);

	if (csr & MUSB_RXCSR_P_SENTSTALL) 
	{
		csr |= MUSB_RXCSR_P_WZC_BITS;
		csr &= ~MUSB_RXCSR_P_SENTSTALL;
		musb_writew_rom(MUSB_RXCSR, csr);
		return;
	}

	if (csr & MUSB_RXCSR_P_OVERRUN) 
	{
		csr &= ~MUSB_RXCSR_P_OVERRUN;
		musb_writew_rom(MUSB_RXCSR, csr);
		return ;
	}

	if (csr & MUSB_RXCSR_INCOMPRX)
	{
		/* REVISIT not necessarily an error */
		return ;
	}

	
	rxcount = musb_readw_rom( MUSB_RXCOUNT);
	if(rxcount == 0)
	{
		return ;
	}
	
	pTransfer = &g_BootUsbVar.Transfert[epnum+DIEP_NUM];

	musb_read_fifo_rom(epnum, pTransfer->buffer + pTransfer->sizeTransfered,  rxcount);
	pTransfer->sizeTransfered += rxcount;
	pTransfer->sizeRemaining -= rxcount;

	rxMaxP = musb_readw_rom( MUSB_RXMAXP);

	if( rxcount < rxMaxP || pTransfer->sizeRemaining == 0)
	{
		pTransfer->transferDone= TRUE;
		boot_UsbCallbackEp( HAL_USB_EP_DIRECTION_OUT(epnum), HAL_USB_CALLBACK_TYPE_RECEIVE_END, 0);
	}
	else
	{
        csr &=~(MUSB_RXCSR_RXPKTRDY);
	}

	musb_writew_rom(MUSB_RXCSR, csr);
	
}

void boot_musb_g_tx(u8 epnum)
{
	u16		csr;
	u16 		txmaxp;
	BOOT_USB_TRANSFERT_T *pTransfer = &g_BootUsbVar.Transfert[epnum];
	musb_select_ep_rom(epnum);
	
	csr = musb_readw_rom( MUSB_TXCSR);

	/*
	 * REVISIT: for high bandwidth, MUSB_TXCSR_P_INCOMPTX
	 * probably rates reporting as a host error.
	 */
	if (csr & MUSB_TXCSR_P_SENTSTALL) 
	{
		csr |=	MUSB_TXCSR_P_WZC_BITS;
		csr &= ~MUSB_TXCSR_P_SENTSTALL;
		musb_writew_rom(MUSB_TXCSR, csr);
		return;
	}

	if (csr & MUSB_TXCSR_P_UNDERRUN) 
	{
		/* We NAKed, no big deal... little reason to care. */
		csr |=	 MUSB_TXCSR_P_WZC_BITS;
		csr &= ~(MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_TXPKTRDY);
		musb_writew_rom(MUSB_TXCSR, csr);
	}

	if(pTransfer->buffer == 0 || pTransfer->sizeRemaining == 0)
	{
		return ;
	}

	// continue transfer....
	txmaxp = musb_readw_rom( MUSB_TXMAXP);
	if(txmaxp >= pTransfer->sizeRemaining )
	{
	    pTransfer->sizeRemaining = 0;
		pTransfer->transferDone = TRUE;// Set transferDone here. Be different from hal_usb_mgc
		boot_UsbCallbackEp(HAL_USB_EP_DIRECTION_IN(epnum), HAL_USB_CALLBACK_TYPE_TRANSMIT_END, 0);
	}
	else
	{
		u16 size = txmaxp;
		pTransfer->sizeRemaining -= txmaxp;
		pTransfer->sizeTransfered += txmaxp;

		if(txmaxp > pTransfer->sizeRemaining)
		{
			size = pTransfer->sizeRemaining;
		}
		musb_write_fifo_rom(epnum, pTransfer->buffer+pTransfer->sizeTransfered, size);

		csr = musb_readw_rom( MUSB_TXCSR);
		csr |= MUSB_TXCSR_TXPKTRDY;	
		musb_writew_rom( MUSB_TXCSR, csr);
	}
	
}

PUBLIC VOID boot_UsbIrqHandler(UINT8 interruptId)
{
	u8		devctl;
	int		ep_num;
	u32		reg;

	u8 		int_usb = musb_readb_rom( MUSB_INTRUSB);
	u16		int_tx = musb_readw_rom( MUSB_INTRTX);
	u16		int_rx = musb_readw_rom( MUSB_INTRRX);
	
	int_usb &= ~MUSB_INTR_SOF;
	if ( !( int_usb || int_tx || int_rx) )
	{
		return ;
	}

	devctl = musb_readb_rom( MUSB_DEVCTL);

	/* the core can interrupt us for multiple reasons; docs have
	 * a generic interrupt flowchart to follow
	 */
	if (int_usb)
	{
		boot_musb_stage0_irq(int_usb, devctl);
	}
	
	/* "stage 1" is handling endpoint irqs */

	/* handle endpoint 0 first */
	if (int_tx & 1)
	{
	     mon_Event(0x00001473);
		boot_UsbDecodeEp0Packet();
	}
	/* RX on endpoints 1-15 */
	reg = int_rx >> 1;
	ep_num = 1;
	while (reg) 
	{
		if (reg & 1)
		{
			boot_musb_g_rx(ep_num);
		}

		reg >>= 1;
		ep_num++;
	}

	/* TX on endpoints 1-15 */
	reg = int_tx >> 1;
	ep_num = 1;
	while (reg) 
	{
		if (reg & 1)
		{
			boot_musb_g_tx(ep_num);
		}
		reg >>= 1;
		ep_num++;
	}

}

void boot_musb_start()
{
	u8		devctl = musb_readb_rom(MUSB_DEVCTL);
    
	/* enable USB common interrupts  INTUSB */
	musb_writeb_rom(MUSB_INTRUSBE, 0xf7);
	musb_writeb_rom(MUSB_TESTMODE, 0);

	/* put into basic highspeed mode and start session */
	musb_writeb_rom(MUSB_POWER, MUSB_POWER_ISOUPDATE
						//| MUSB_POWER_HSENAB
						/* ENSUSPEND wedges tusb */
						 //| MUSB_POWER_ENSUSPEND
						);

	devctl = musb_readb_rom(MUSB_DEVCTL);
	devctl |= MUSB_DEVCTL_SESSION;
	musb_writeb_rom(MUSB_DEVCTL, devctl);//81
	Varvbuserr_retry = 3;
 #if 0
 	    devctl = musb_readb_rom(MUSB_DEVCTL);
	    devctl &= ~MUSB_DEVCTL_SESSION;

	/* session started after:
	 * (a) ID-grounded irq, host mode;
	 * (b) vbus present/connect IRQ, peripheral mode;
	 * (c) peripheral initiates, using SRP
	 */
	if (!((devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS))
		;
	else
		devctl |= MUSB_DEVCTL_SESSION;

     musb_writeb_rom(MUSB_DEVCTL, devctl);
 #endif
}


PUBLIC VOID boot_UsbOpen(HAL_USB_GETDESCRIPTOR_CALLBACK_T callback)
{
    // USB reset.
    boot_UsbInit();
    // Register callback
    g_BootUsbVar.DeviceCallback = callback;
    boot_musb_start();
    boot_UsbConfig();
}

PUBLIC VOID boot_UsbEpStall(UINT8 ep, BOOL stall)
{
	UINT8   epNum;

	epNum = HAL_USB_EP_NUM(ep);

	// Select ctl register
	if(epNum == 0)
	{

	}
	else
	{

	}
}



// =============================================================================
// boot_UsbRecv
// -----------------------------------------------------------------------------
/// Recv an usb packet
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param buffer Pointer on the data buffer to be send.
/// @param size Number of bytes to be send.
/// @param flag No flag you must set this parameter to zero
/// @return Number of sent bytes or -1 if error
// =============================================================================
PUBLIC INT32 boot_UsbRecv(UINT8 ep, UINT8* buffer, UINT16 size, UINT32 flag)
{
    UINT8   epNum;
    u16     csr;
    BOOT_USB_TRANSFERT_T *pTransfer;
	
    epNum = HAL_USB_EP_NUM(ep);

	// recieve data in interrupt, unload fifo. 
	pTransfer = &g_BootUsbVar.Transfert[epNum+DIEP_NUM];
	pTransfer->buffer = buffer;
	pTransfer->sizeTransfered = 0;
	pTransfer->sizeRemaining = size;
	pTransfer->transferDone = FALSE;

	
	musb_select_ep_rom(epNum);

	csr = musb_readw_rom(MUSB_RXCSR);
	csr &=~(MUSB_RXCSR_RXPKTRDY);// Clear RXPKTRDY
	musb_writew_rom(MUSB_RXCSR, csr);
#if 0
	csr = musb_readw_rom(MUSB_RXCSR);
	csr &=~(MUSB_RXCSR_RXPKTRDY);// Clear RXPKTRDY
	musb_writew_rom(MUSB_RXCSR, csr);
#endif
	csr = musb_readw_rom(MUSB_RXCSR);

	return(size);
}


// =============================================================================
// boot_UsbSend
// -----------------------------------------------------------------------------
/// Send an usb packet
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param buffer Pointer on the date buffer to be send.
/// @param size Number of bytes to be send.
/// @param flag For partial send set the flag to 1
/// @return Number of sent bytes or -1 if error
// =============================================================================
PUBLIC INT32 boot_UsbSend(UINT8 ep, UINT8* buffer, UINT16 size, UINT32 flag)
{
	UINT8   epNum;
	UINT16	txcsr = 0;
	UINT16	length = 0;	
	
	epNum = HAL_USB_EP_NUM(ep);
	musb_select_ep_rom(epNum);
	
	if(epNum == 0)
	{
		u16 csr0  ;
		u16 csr1  ;
		musb_write_fifo_rom(0, buffer, size);
		csr0 = musb_readw_rom( MUSB_CSR0);
		csr0  |= MUSB_CSR0_TXPKTRDY|MUSB_CSR0_P_DATAEND;
		musb_writew_rom(MUSB_CSR0, csr0);
		csr1 = musb_readw_rom( MUSB_INTRTX);
		return size;
	}
	else
	{
		BOOT_USB_TRANSFERT_T *pTransfer = &g_BootUsbVar.Transfert[epNum];
		pTransfer->sizeRemaining = size;
		pTransfer->sizeTransfered = 0;
		pTransfer->buffer = buffer;
		pTransfer->transferDone = FALSE;

		//write to fifo
		length = musb_readw_rom( MUSB_TXMAXP);
		if( size < length )
		{
			length = size;
		}

		txcsr = musb_readw_rom( MUSB_TXCSR);
		txcsr |= MUSB_TXCSR_MODE;
		musb_writew_rom( MUSB_TXCSR, txcsr);
		musb_write_fifo_rom(epNum, buffer, length);

		txcsr = musb_readw_rom( MUSB_TXCSR);
		txcsr |= MUSB_TXCSR_TXPKTRDY;	
		musb_writew_rom( MUSB_TXCSR, txcsr);
	}
	return length;
}

PUBLIC VOID boot_UsbCompletedCommand(VOID)
{
    switch(g_BootUsbVar.Ep0State)
    {
    case EP0_STATE_STATUS_OUT:
        boot_UsbStatusOut();
        break;
    case EP0_STATE_STATUS_IN:
        boot_UsbStatusIn();
        break;
    default:
        break;

    }
}

PUBLIC VOID boot_UsbEpEnableInterrupt(UINT8 ep, BOOL enable)
{
	UINT8   epNum;
	epNum = HAL_USB_EP_NUM(ep);

	if(enable == TRUE)
	{
		if(HAL_USB_IS_EP_DIRECTION_IN(ep))
		{
			g_BootUsbVar.EpInterruptMask    |= (1<<(epNum+0));
		}
		else
		{
			g_BootUsbVar.EpInterruptMask    |= (1<<(epNum+16));
		}
	}
	else
	{
		if(HAL_USB_IS_EP_DIRECTION_IN(ep))
		{
			g_BootUsbVar.EpInterruptMask    &= ~(1<<(epNum+0));
		}
		else
		{
			g_BootUsbVar.EpInterruptMask    &= ~(1<<(epNum+16));
		}
	}
}

PUBLIC UINT16 boot_UsbEpRxTransferedSize(UINT8 ep)
{
    UINT8   epNum;

    epNum = HAL_USB_EP_NUM(ep);

    // When the transfer is done, the size value is 
    // inverted. Consequently, when this function 
    // is called after a transfer is done, size 
    // must be inverted.
    return g_BootUsbVar.Transfert[epNum+DIEP_NUM].sizeTransfered;
}

PUBLIC BOOL boot_UsbEpTransfertDone(UINT8 ep)
{
    UINT8   epNum = HAL_USB_EP_NUM(ep);
    BOOT_USB_TRANSFERT_T* pTransfer = NULL;

	if(HAL_USB_IS_EP_DIRECTION_IN(ep))
	{
		pTransfer = &g_BootUsbVar.Transfert[epNum];
	}
	else
	{      
		pTransfer = &g_BootUsbVar.Transfert[DIEP_NUM+epNum];  
	}
    return pTransfer->transferDone;
}

PUBLIC VOID boot_UsbClose(VOID)
{
    // Clear the configuration, and call the callbacks.
    boot_UsbClrConfig();
    g_BootUsbVar.Ep0State        = EP0_STATE_IDLE;
    g_BootUsbVar.NbString        = 0;
    g_BootUsbVar.DeviceCallback  = 0;
    boot_UsbResetTransfert();
    g_BootUsbVar.Desc            = 0;
}


// =============================================================================
// hal_UsbReset
// -----------------------------------------------------------------------------
/// This function is use to reset USB and/or change configuration of usb
/// For reset you must do hal_UsbReset(TRUE); sleep 0.5s; hal_UsbReset(FALSE);
/// @param reset If is true, the function is clear usb and power down usb else
/// the function power up usb
// =============================================================================
PUBLIC VOID boot_UsbReset(VOID)
{
	boot_UsbInit();
	boot_UsbConfig();
}



