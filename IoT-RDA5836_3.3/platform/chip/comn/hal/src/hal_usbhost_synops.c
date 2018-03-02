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

/************************************************************************************
*
*  hal_usbhost.c, 
*
*
*
*************************************************************************************/


#if (CHIP_HAS_USB == 1)

#include "pmd_m.h"

#include "sxr_ops.h"
#include "halp_sys.h"

#include "usb.h"
#include "halp_usb.h"
#include "halp_debug.h"
#include "halp_version.h"
#include "hal_usb.h"
#include "usb_hcd.h"

#include "string.h"

#include "cfg_regs.h"
#include "global_macros.h"
#include "usbc.h"
#include "sys_ctrl.h"
#include "event.h"
#include "hal_host.h"
#include "tgt_app_cfg.h"
#ifdef MCD_USB_HOST_SUPPORT
#include "sys_ctrl.h"

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


// usb internal dma mode , AHB Master burst type:
#define USB_IDMA_SINGLE		0
#define USB_IDMA_INCR		1
#define USB_IDMA_INCR4		3
#define USB_IDMA_INCR8		5
#define USB_IDMA_INCR16		7

//host channel n interrupt mask 
#define HCNI_XFERCOMPL_MSK		(1<<0)
#define HCNI_XFERCOMPL			(1<<0)
#define HCNI_CHHLTD_MSK			(1<<1)
#define HCNI_CHHLTD				(1<<1)
#define HCNI_AHBERR_MSK			(1<<2)
#define HCNI_AHBERR			    (1<<2)
#define HCNI_STALL_MSK			(1<<3)
#define HCNI_STALL				(1<<3)
#define HCNI_NAK_MSK			(1<<4)
#define HCNI_NAK				(1<<4)
#define HCNI_ACK_MSK			(1<<5)
#define HCNI_ACK 				(1<<5)
#define HCNI_NYET_MSK			(1<<6)
#define HCNI_NYET				(1<<6)
#define HCNI_XACTERR_MSK		(1<<7)
#define HCNI_XACTERR			(1<<7)
#define HCNI_BBLERR_MSK			(1<<8)
#define HCNI_BBLERR				(1<<8)
#define	HCNI_FRMOVRUN_MSK		(1<<9)
#define	HCNI_FRMOVRUN			(1<<9)
#define HCNI_DTGLERR_MSK		(1<<10)
#define HCNI_DTGLERR 			(1<<10)

//HPRT bitmap 
#define USBH_PRTCONNSTS		(1<<0)
#define USBH_PRTCONNDET		(1<<1)
#define USBH_PRTENA			(1<<2)
#define USBH_PRTENCHNG		(1<<3)
#define USBH_PRTOVRCURRACT	(1<<4)
#define USBH_PRTOVRCURRCHNG	(1<<5)
#define USBH_PRTRESUME		(1<<6)
#define USBH_PRTSUSP		(1<<7)
#define USBH_PRTRST			(1<<8)
#define USBH_PRTLNSTS_DN    (1<<10)
#define USBH_PRTLNSTS_DP    (1<<11)
#define USBH_PRTPWR			(1<<12)

//HCCHARn bitmap
#define USBH_CHENA		(1<<31)
#define USBH_CHDIS		(1<<30)
#define USBH_ODDFRM		(1<<29)
#define USBH_DEVADDR(n)	((n&0x3f)<<22)
#define USBH_IN			(1<<15)
#define USBH_EPTYPE(n)	((n&0x03)<<18)
#define USBH_LSPDDEV	(1<<17)
#define USBH_EPDIRIN	(1<<15)
#define USBH_EPDIROUT	()
#define USBH_EPNUM(n)	((n&0x0f)<<11)
#define USBH_MPS(n)		(n&0x07ff)

//HCTSIZn bitmap 
#define USBH_PKTCNT(n)		((n&0x03ff)<<19)
#define USBH_XFERSIZE(n)	((n&0x7ffff))
#define USBH_PID(n)			((n&0x03)<<29)


//according to synopsys's datasheet
#define USB_DATA0		0
#define USB_DATA1		2
#define USB_DATA2		1
#define USB_MDATA		3




typedef struct tagUSBHostReg
{
    REG32	GOTGCTL;                      //0x00000000
    REG32   GOTGINT;                      //0x00000004
    REG32   GAHBCFG;                      //0x00000008
    REG32   GUSBCFG;                      //0x0000000C
    REG32   GRSTCTL;                      //0x00000010
    REG32   GINTSTS;                      //0x00000014
    REG32   GINTMSK;                      //0x00000018
    REG32   GRXSTSR;                      //0x0000001C
    REG32   GRXSTSP;                      //0x00000020
    REG32   GRXFSIZ;                      //0x00000024
    REG32   GNPTXFSIZ;                    //0x00000028
    REG32   GNPTXSTS;                     //0x0000002C
    REG32   GI2CCTL;                      //0x00000030
    REG32   GPVNDCTL;                     //0x00000034
    REG32   GGPIO;                        //0x00000038
    REG32   GUID;                         //0x0000003C
    REG32   GSNPSID;                      //0x00000040
    REG32   GHWCFG1;                      //0x00000044
    REG32   GHWCFG2;                      //0x00000048
    REG32   GHWCFG3;                      //0x0000004C
    REG32   GHWCFG4;                      //0x00000050
    REG32 	Reserved_00000054[43];        //0x00000054
    REG32   HPTXFSIZ;                     //0x00000100
    REG32   Reserved_00000110[191];       //0x00000104 - 3FF
    //host part 
    REG32 	HCFG;						  /*0x00000400*/
	REG32	HFIR;						  /*0x00000404*/
	REG32	HFNUM;						  /*0x00000408*/
	REG32	Reserved_0000040c;			  /*0x0000040c*/
	REG32	HPTXSTS;					  /*0x00000410*/
	REG32	HAINT;						  /*0x00000414*/
	REG32	HAINTMSK;					  /*0x00000418*/
	REG32	Reserved_0000041c[9];		  /*0x0000041C*/	
	REG32	HPRT;						  /*0x00000440*/
	REG32	Reserved_00000444[47];		  /*0x00000444*/
	struct 	
	{
		REG32	HCCHARn;				/*0x00000500+n*0x20*/
		REG32	HCSPLTn;				/*0x00000504+n*0x20*/
		REG32	HCINTn;					/*0x00000508+n*0x20*/
		REG32	HCINTMSKn;				/*0x0000050c+n*0x20*/
		REG32	HCTSIZn;				/*0x00000510+n*0x20*/
		REG32	HCDMAn;					/*0x00000504+n*0x20*/
		REG32	Reserved_0000000n[2];
	}HCSR[16];
}USBHOSTREG;

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
	USBCONTROLREQUEST * pCtrlReq;
	USBENDPOINT epArray[RDA_USB_MAX_END];
	void (*InitCompleteCb)(void *pData, uint8 status);
}USBHOSTINST;

static USBHOSTINST gUsbHostInst;
static volatile USBHOSTREG* gUsbRegs = (USBHOSTREG*)0xa1a40000;

BOOL RDA_USBEnable(void)
{
    if(gUsbHostInst.portEnabled)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
uint8 pktArry[10];
/**/
int RDAUsb_PacketTransfer(int chn)
{
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[chn];
	uint8 	pktSize = 0;
	uint8 	*pBuf;
	
	if(pEnd->pktCnt == 0 || pEnd->nCurTransferred == pEnd->pBulkReq->nBufLen)
	{
		return 1;
	}

	if(pEnd->pktCnt > 1)
	{
		pktSize = pEnd->nMaxPktSize;
		//pBuf = pEnd->pBulkReq->pBuf + pEnd->nCurTransferred;
	}
	else
	{
		pktSize = pEnd->pBulkReq->nBufLen - pEnd->nCurTransferred;
		//pBuf = pEnd->pBulkReq->pBuf + pEnd->nCurTransferred;
	}
    	pBuf = pEnd->pBulkReq->pBuf + pEnd->nCurTransferred;
#if 0
    UINT32 firstUINT32 = 0;
    uint8 *tempBuf = pBuf-64;
    firstUINT32 = (*tempBuf)|(*(tempBuf+1)<<8)|(*(tempBuf+2)<<16)|(*(tempBuf+3)<<24);
    hal_HstSendEvent(SYS_EVENT, 0x15061101);
    hal_HstSendEvent(SYS_EVENT, firstUINT32);
#endif
	if( pEnd->Epaddress&USB_END0_DIRIN)
	{	
		gUsbRegs->HCSR[chn].HCINTMSKn = 0x7ef;
	}
	else
	{
		gUsbRegs->HCSR[chn].HCINTMSKn = 0x7ff;
	}
	pktArry[9] = pEnd->pktCnt;
	pktArry[pEnd->pktCnt] = pktSize;
	//hal_HstSendEvent(SYS_EVENT,0x09150002);
	//hal_HstSendEvent(SYS_EVENT,pBuf);
	gUsbRegs->HCSR[chn].HCDMAn = (uint32)pBuf;
	gUsbRegs->HCSR[chn].HCTSIZn = USBH_XFERSIZE(pktSize) | USBH_PKTCNT(1) | USBH_PID(pEnd->DataToggle);
	//hal_HstSendEvent(gUsbRegs->HCSR[chn].HCTSIZn);
	gUsbRegs->HCSR[chn].HCCHARn |= USBH_CHENA;
	return 0;
	
}

/*host part init*/
void rda_UsbHostInitPart2()
{
	uint32 addr = 0;

	gUsbRegs->GINTMSK |= USBC_PRTINT_MASK;
	hal_HstSendEvent(USB_EVENT, 0x14090502);

	gUsbRegs->HCFG |= 0x05;								/* full/low speed, 48 MHZ */
	gUsbRegs->HPRT |= USBH_PRTPWR;						/* power on */
	//COS_Sleep(20 MS_WAITING);							/* wait for HPRT connDet interrupt */
			
	gUsbHostInst.deviceSpeed = (gUsbRegs->HPRT>>17)&0x03;

	gUsbRegs->HFIR = 48000;

	if(gUsbRegs->GHWCFG2 & USBC_DYNFIFOSIZING)
	{	
		gUsbRegs->GRXFSIZ  = 64;
		addr  = 64;
	
		gUsbRegs->GNPTXFSIZ  = USBC_NPTXFSTADDR(addr) | USBC_NPTXFDEPS(64); //RXFIFOSIZE
		addr += 64;	//RXFIFOSIZE;

		hal_HstSendEvent(USB_EVENT, 0x14090522);
		hal_HstSendEvent(USB_EVENT, gUsbRegs->GNPTXFSIZ);
		//gUsbRegs->HPTXFSIZ 	 = USBC_NPTXFSTADDR(addr) | USBC_NPTXFDEPS(0x100);  //HPTXFSIZ
		//addr += 0x100;
	}	

	/*flush fifo*/
	gUsbRegs->GRSTCTL |= USBC_TXFFLSH | USBC_RXFFLSH; 
	hal_HstSendEvent(USB_EVENT, 0x14090533);
	hal_HstSendEvent(USB_EVENT, gUsbRegs->GNPTXFSIZ);
	hal_HstSendEvent(USB_EVENT, gUsbRegs->GAHBCFG);
	hal_HstSendEvent(USB_EVENT, gUsbRegs->GINTMSK);
}


int RDAUsb_BulkTransfer( USBBULKREQUEST * pReq)
{
	USBENDPOINT * pEnd;
	
	if( pReq == NULL || gUsbHostInst.portEnabled==0 )
	{
	    hal_HstSendEvent(SYS_EVENT, 0x15062501);
	    hal_HstSendEvent(SYS_EVENT, gUsbHostInst.portEnabled);
		return USBHCD_ERROR_FAILED;
	}

	if(pReq->pipe > 2)
	{
        hal_HstSendEvent(SYS_EVENT, 0x15062544);
        hal_DbgAssert("3 or bigger endpoint will not be supported.");
	}
	pEnd =&gUsbHostInst.epArray[pReq->pipe];
	pEnd->pBulkReq = pReq;
	pEnd->nCurTransferred = 0;

	if(pEnd->nMaxPktSize == 0)
	{
        hal_HstSendEvent(SYS_EVENT, 0x15072501);
        hal_HstSendEvent(SYS_EVENT, (UINT32)pReq->pipe);
        hal_HstSendEvent(SYS_EVENT, (UINT32)pEnd);
        return USBHCD_ERROR_FAILED;
	}
	pEnd->pktCnt = pReq->nBufLen / pEnd->nMaxPktSize + 1; 
	
	RDAUsb_PacketTransfer(pReq->pipe);
	
	return USB_NO_ERROR;
}

USB_PIPE RDAUsb_OpenPipe(USBENDPOINTDESC *pEndDes)
{
	int i = 0;
	if( ((gUsbHostInst.endNum+1) > ((gUsbRegs->GHWCFG2>>14)&0x0F)) || pEndDes == NULL)
	{
		return NULL;
	}
	
	i = ++gUsbHostInst.endNum;

	gUsbHostInst.epArray[i].Epaddress = pEndDes->bEndpointAddress;
	gUsbHostInst.epArray[i].nMaxPktSize = pEndDes->wMaxPacketSize;
	gUsbHostInst.epArray[i].type = pEndDes->bmAttributes;
	gUsbHostInst.epArray[i].DataToggle = USB_DATA0;	/*first packect should be USB_DATA0*/

	gUsbRegs->HAINTMSK |= (1<<i);			
	//gUsbRegs->HCSR[i].HCINTMSKn = 0x7ff;	/*unmask all kind of channel's interrupt*/
	gUsbRegs->HCSR[i].HCCHARn = USBH_MPS(pEndDes->wMaxPacketSize) | USBH_EPNUM(pEndDes->bEndpointAddress) \
		| USBH_EPTYPE(pEndDes->bmAttributes) | USBH_DEVADDR(gUsbHostInst.address); 
	if( pEndDes->bEndpointAddress & 0x80)
	{
		gUsbRegs->HCSR[i].HCCHARn |= USBH_EPDIRIN;
	}
	
	return (USB_PIPE)gUsbHostInst.endNum;
}

int RDAUsb_ControlTransfer(USBCONTROLREQUEST * pReq)
{
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[0];
	if(pReq == NULL || gUsbHostInst.portEnabled == 0)
	{
		return USBHCD_ERROR_FAILED;
	}

	gUsbHostInst.pCtrlReq = pReq;
	gUsbHostInst.end0Stage = USB_END0_STAGE_SETUP;

	hal_HstSendEvent(SYS_EVENT,0x09060005);
	hal_HstSendEvent(USB_EVENT, gUsbRegs->GNPTXFSIZ);

	gUsbRegs->HAINTMSK |= 1;  /*bit 0 for channel 0*/
	gUsbRegs->GINTMSK |= USBC_HCHINT_MASK;	/*unmask host channel interrupt*/

	/* unmask interrupt, transfer Complete, AHB error, stall response, halted channel, transaction error */
	gUsbRegs->HCSR[0].HCINTMSKn = 0x7ef; //HCNI_XFERCOMPL_MSK | HCNI_CHHLTD_MSK | HCNI_AHBERR_MSK | HCNI_STALL_MSK | HCNI_XACTERR_MSK; 
	gUsbRegs->HCSR[0].HCTSIZn = USBH_PID(USB_MDATA); /**/
	gUsbRegs->HCSR[0].HCCHARn = USBH_MPS(gUsbHostInst.epArray[0].nMaxPktSize);	/*max packet size*/
	
	gUsbRegs->HCSR[0].HCDMAn = (uint32)&pReq->stdRequest;
	gUsbRegs->HCSR[0].HCTSIZn |=  USBH_XFERSIZE(sizeof(USBSTDREQUEST)) | USBH_PKTCNT(1);  
	gUsbRegs->HCSR[0].HCCHARn |= USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
	gUsbHostInst.DataToggle = USB_DATA0;

	pEnd->nCurTransferred = 0;
	hal_HstSendEvent(SYS_EVENT,0x09060015);
	
	return USB_NO_ERROR;
}


int RDA_UsbReset(void (*CompleteCb)(void* pData, uint8 status))
{
	int count = 0;
	hal_HstSendEvent(USB_EVENT,0x17040606);
	do
	{
		gUsbRegs->HPRT |= USBH_PRTRST;							/* start reset */
		//COS_Sleep(50 MS_WAITING);
		hal_TimDelay(920);

		gUsbRegs->HPRT &= ~USBH_PRTRST;							/* complete reset */
		//COS_Sleep(200 MS_WAITING);
		hal_TimDelay(1638);
		count++;
		
	}while(0); // && !gUsbHostInst.portEnabled);	
    hal_HstSendEvent(USB_EVENT,0x17040607);
	if( CompleteCb )
	{
		CompleteCb(NULL, !gUsbHostInst.portEnabled);
	}
	hal_HstSendEvent(USB_EVENT,0x09170003);
	hal_HstSendEvent(USB_EVENT,gUsbHostInst.portEnabled);
	hal_HstSendEvent(USB_EVENT,0x14090555);
	hal_HstSendEvent(USB_EVENT, gUsbRegs->GNPTXFSIZ);
	return !gUsbHostInst.portEnabled;
}

/*usb core init*/
void RDA_UsbCoreInit()
{
	uint32 	AHBCfg = 0;
       //uint32 csStatus;    

	//flush fifo
	gUsbRegs->GRSTCTL  =  USBC_TXFNUM(0x10) | USBC_TXFFLSH | USBC_RXFFLSH; 

	/*core initialization*/
	if(gUsbRegs->GHWCFG2&(3<<3)) 	// external DMA or Internal DMA impl
	{
		AHBCfg |= USBC_DMAEN | USB_IDMA_INCR4<<1;
		gUsbHostInst.dmaEnabled = 1;
	}

	AHBCfg |= USBC_GLBLINTRMSK;

	gUsbRegs->GAHBCFG |= AHBCfg;
	gUsbRegs->GUSBCFG |= USBC_PHYIF | USBC_USBTRDTIM(5); //USBC_USBTRDTIM(9)

	gUsbRegs->GINTMSK =  USBC_MODEMIS_MASK | USBC_DISCONNINT_MASK; // | USBC_SOF_MASK;	/*unmask port status interrupt bit*/

	/*host initialization*/
	rda_UsbHostInitPart2();
	
}


PUBLIC VOID hal_Usb_host_Open(void)
{
    hal_HstSendEvent(USB_EVENT, 0x17041400);
    // resource will automatically enable the PLL on greenstone
    //hal_SysRequestFreq(HAL_SYS_FREQ_USB, HAL_SYS_FREQ_39M, 0);
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
    hwp_sysCtrl->REG_DBG          = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_USBPHY;
    hwp_sysCtrl->REG_DBG          = SYS_CTRL_PROTECT_LOCK;
#endif    
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_8808)
    pmd_EnablePower(PMD_POWER_USB, TRUE);
#endif
}

int RDA_UsbHostInit(USBOPERATION *pUsbOps, void (*InitCb)(void*pData, uint8 status))
{
	hal_Usb_host_Open();
	if( pUsbOps != NULL)
	{
		pUsbOps->BulkTransfer = RDAUsb_BulkTransfer;
		pUsbOps->ControlTransfer = RDAUsb_ControlTransfer;
		pUsbOps->OpenPipe = RDAUsb_OpenPipe;
		//pUsbOps->ResetDefaultEndpoint = RDA_UsbReset;
	}

	memset(&gUsbHostInst, 0, sizeof(gUsbHostInst));

	gUsbHostInst.end0Stage = USB_END0_STAGE_IDLE;
	gUsbHostInst.InitCompleteCb = InitCb;
	gUsbHostInst.epArray[0].nMaxPktSize = 0x40;	/*preset to 64 byte, update then we got device descriptor*/

	RDA_UsbCoreInit();
	RDA_UsbCoreInit();

	if( InitCb )
	{
		//InitCb(NULL, 0);
	}
	return 0;
}

void rda_ProcessControlInterrupt()
{
	USBCONTROLREQUEST *pCtrlReq = gUsbHostInst.pCtrlReq;
	USBENDPOINT *pEnd = &gUsbHostInst.epArray[0];
	int ints = gUsbRegs->HCSR[0].HCINTn;

	hal_HstSendEvent(SYS_EVENT,0x09163333);
	hal_HstSendEvent(SYS_EVENT,ints);
	hal_HstSendEvent(SYS_EVENT,gUsbHostInst.end0Stage);
	hal_HstSendEvent(SYS_EVENT,(UINT32)gUsbHostInst.pCtrlReq->pBuf);
	gUsbRegs->HCSR[0].HCINTn = 0x7ff;
	ints &= HCNI_XFERCOMPL | HCNI_CHHLTD | HCNI_AHBERR | HCNI_STALL | HCNI_XACTERR_MSK ;

	//hal_HstSendEvent(SYS_EVENT,0x09163333);
	//hal_HstSendEvent(SYS_EVENT,ints);
	
	if(ints&HCNI_CHHLTD)
	{
		if(( ints&HCNI_XFERCOMPL)||(ints == HCNI_CHHLTD))
		{
			switch(gUsbHostInst.end0Stage)
			{
			case USB_END0_STAGE_SETUP:
				{
				    hal_HstSendEvent(SYS_EVENT,0x09160033);
					USBSTDREQUEST *pStdReq = &pCtrlReq->stdRequest;
					//hal_TimDelay(2 MS_WAITING);
					if( pStdReq->wLength > 0 )
					{
						int size = pCtrlReq->nBufLen > 0x40?0x40:pCtrlReq->nBufLen;
						
						gUsbRegs->HCSR[0].HCDMAn = (UINT)pCtrlReq->pBuf;
						gUsbHostInst.DataToggle = gUsbHostInst.DataToggle?USB_DATA0:USB_DATA1;
						gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(size) | USBH_PKTCNT(1)| USBH_PID(gUsbHostInst.DataToggle); 

						if( pStdReq->bmRequestType & USB_END0_DIRIN)
						{	
							gUsbRegs->HCSR[0].HCCHARn |= USBH_IN;
							gUsbHostInst.end0Stage = USB_END0_STAGE_IN;
						}	
						else
						{
							gUsbRegs->HCSR[0].HCCHARn &= ~USBH_IN;
							gUsbHostInst.end0Stage = USB_END0_STAGE_OUT;
						}
						pEnd->nCurTransferred = size;
						gUsbRegs->HCSR[0].HCCHARn |= USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
					}
					else
					{
						gUsbRegs->HCSR[0].HCDMAn = 0;
						gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(0) | USBH_PKTCNT(1)| USBH_PID(USB_DATA1); 
						gUsbRegs->HCSR[0].HCCHARn = USBH_MPS(pEnd->nMaxPktSize) | USBH_CHENA | USBH_IN | USBH_DEVADDR(gUsbHostInst.address);
						gUsbHostInst.end0Stage = USB_END0_STAGE_STATUS_IN;
					}
				}
				break;
			case USB_END0_STAGE_IN:
				{
					hal_HstSendEvent(SYS_EVENT,0x09160001);
					//hal_TimDelay(1 MS_WAITING);
					uint8 *Buf = &pCtrlReq->pBuf[0];
					hal_HstSendEvent(SYS_EVENT,(UINT32)Buf);
					hal_HstSendEvent(SYS_EVENT,(Buf[0]<<24)|(Buf[1]<<16)|(Buf[2]<<8)|(Buf[3]<<0));
					hal_HstSendEvent(SYS_EVENT,(Buf[4]<<24)|(Buf[5]<<16)|(Buf[6]<<8)|(Buf[7]<<0));
					hal_HstSendEvent(SYS_EVENT,(Buf[8]<<24)|(Buf[9]<<16)|(Buf[10]<<8)|(Buf[11]<<0));
					hal_HstSendEvent(SYS_EVENT,(Buf[12]<<24)|(Buf[13]<<16)|(Buf[14]<<8)|(Buf[15]<<0));
					if(pCtrlReq->stdRequest.bRequest == USB_GET_DESCRIPTOR && (pCtrlReq->stdRequest.wValue & 1<<8))
					{
						pEnd->nMaxPktSize = pCtrlReq->pBuf[7];
					}
					
					if(pEnd->nCurTransferred < pCtrlReq->nBufLen)
					{
						int size = (pCtrlReq->nBufLen - pEnd->nCurTransferred);
						size = size > 0x40 ? 0x40:size;
						
						gUsbRegs->HCSR[0].HCDMAn = (UINT)pCtrlReq->pBuf+pEnd->nCurTransferred;
						gUsbHostInst.DataToggle = gUsbHostInst.DataToggle?USB_DATA0:USB_DATA1;
						gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(size) | USBH_PKTCNT(1)| USBH_PID(gUsbHostInst.DataToggle); 
						gUsbRegs->HCSR[0].HCCHARn |= USBH_IN|USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
						pEnd->nCurTransferred +=size;
					}
					else
					{
						/*status out*/
						gUsbHostInst.end0Stage= USB_END0_STAGE_STATUS_OUT;
						gUsbRegs->HCSR[0].HCDMAn = 0;
						gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(0) | USBH_PKTCNT(1)| USBH_PID(USB_DATA1); 
						gUsbRegs->HCSR[0].HCCHARn = USBH_MPS(pEnd->nMaxPktSize) | USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
					}
				}		
				break;
			case USB_END0_STAGE_OUT:
				{
					if(pEnd->nCurTransferred < pCtrlReq->nBufLen)
					{
						int size = (pCtrlReq->nBufLen - pEnd->nCurTransferred);
						size = size > 0x40 ? 0x40:size;

						gUsbRegs->HCSR[0].HCDMAn = (UINT)pCtrlReq->pBuf+pEnd->nCurTransferred;
						gUsbHostInst.DataToggle = gUsbHostInst.DataToggle?USB_DATA0:USB_DATA1;
						gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(size) | USBH_PKTCNT(1)| USBH_PID(gUsbHostInst.DataToggle); 
						gUsbRegs->HCSR[0].HCCHARn &= ~USBH_IN;
						gUsbRegs->HCSR[0].HCCHARn |= USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
						pEnd->nCurTransferred += size;

					}
					else
					{
						gUsbHostInst.end0Stage= USB_END0_STAGE_STATUS_IN;	
					}
				}
				break;
			case USB_END0_STAGE_STATUS_IN:
			    hal_HstSendEvent(SYS_EVENT,0x09160003);
				gUsbHostInst.end0Stage = USB_END0_STAGE_IDLE;

				if(gUsbHostInst.pCtrlReq->stdRequest.bRequest == USB_SET_ADDRESS)
				{
					gUsbHostInst.address = gUsbHostInst.pCtrlReq->stdRequest.wValue;
				}
				
				gUsbRegs->HCSR[0].HCDMAn = 0;
				gUsbRegs->HCSR[0].HCTSIZn = 0;
				gUsbRegs->HCSR[0].HCCHARn = 0;

				if( gUsbHostInst.pCtrlReq->completeCb )
				{
					gUsbHostInst.pCtrlReq->completeCb(gUsbHostInst.pCtrlReq->pBuf, 0);
					gUsbHostInst.pCtrlReq->completeCb = NULL;
				}
				break;
			case USB_END0_STAGE_STATUS_OUT:
				hal_HstSendEvent(SYS_EVENT,0x09160002);
				hal_HstSendEvent(SYS_EVENT,(UINT32)gUsbHostInst.pCtrlReq->pBuf);
				gUsbRegs->HCSR[0].HCDMAn = 0;
				gUsbRegs->HCSR[0].HCTSIZn = 0;
				gUsbRegs->HCSR[0].HCCHARn = 0;
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
		else  if (!(ints & 0x7fd))
		{
			hal_HstSendEvent(SYS_EVENT,0x09280001);
			hal_HstSendEvent(SYS_EVENT,ints);
			if(gUsbHostInst.end0Stage== USB_END0_STAGE_STATUS_OUT)
			{
				//COS_Sleep(1 );

				/*status out*/
				gUsbRegs->HCSR[0].HCDMAn = 0;
				gUsbRegs->HCSR[0].HCTSIZn = USBH_XFERSIZE(0) | USBH_PKTCNT(1)| USBH_PID(USB_DATA1); 
				gUsbRegs->HCSR[0].HCCHARn = USBH_MPS(gUsbHostInst.epArray[0].nMaxPktSize) | USBH_CHENA | USBH_DEVADDR(gUsbHostInst.address);
			}
		}
		else if((ints&HCNI_XACTERR) || (ints&HCNI_NAK))// For fast plug-in/out in enumeration
		{
		    hal_HstSendEvent(SYS_EVENT,0x09280004);
			#if APP_SUPPORT_USB==1
		    extern void hal_UsbDisconnCb(BOOL);
		    hal_UsbDisconnCb(1);
			#endif
		}
		else
		{
		    hal_HstSendEvent(SYS_EVENT,0x09280002);
			if( gUsbHostInst.pCtrlReq->completeCb )
			{
				gUsbHostInst.pCtrlReq->completeCb(gUsbHostInst.pCtrlReq->pBuf, 1);
				gUsbHostInst.pCtrlReq->completeCb = NULL;
			}
		}
	}
	else 
	{
		//hal_HstSendEvent(SYS_EVENT,0x09160004);
		//hal_HstSendEvent(SYS_EVENT,ints);	
	}
}


 void hal_UsbIRQServiceHost(UINT8 interruptId)
{
	uint32 reg;
	uint32 intsts;
	BOOL plugOutException = FALSE;
	interruptId = interruptId;

	//hal_HstSendEvent(USB_EVENT, 0x09090001);

	intsts = gUsbRegs->GINTSTS;
	
	intsts &= USBC_PRTINT | USBC_HCHINT | USBC_DISCONNINT | USBC_MODEMIS | USBC_CONLDSTSCHNG | USBC_SOF;
	gUsbRegs->GINTSTS |= intsts ;
	if(intsts & USBC_DISCONNINT)
	{
	  extern void(*g_pfDisconnCb)(BOOL isHost);
        hal_HstSendEvent(USB_EVENT, 0x079d15c0);
         if(g_pfDisconnCb != NULL)
	  g_pfDisconnCb(1);
	}
	
	if( intsts & USBC_PRTINT)
	{
		reg = gUsbRegs->HPRT;
		if( reg & USBH_PRTENCHNG )
		{
			hal_HstSendEvent(SYS_EVENT,0x09170001);
			hal_HstSendEvent(SYS_EVENT,reg);
            if(reg == 0x21008)// DN=0, DP=1, PortRst=0, PortEna=0, PortConnStatus=1
            {
                plugOutException = TRUE;
            }
			gUsbHostInst.portEnabled = (reg&0x04) >> 2;
			reg &= (~USBH_PRTENA);		/* can't clear USBH_PRTENA*/
			reg |= USBH_PRTENCHNG;
			gUsbRegs->HPRT = reg; //USBH_PRTENCHNG;	/*clear this bit */ 	

			hal_HstSendEvent(SYS_EVENT,0x0917000a);
			hal_HstSendEvent(SYS_EVENT,(gUsbRegs->HPRT | (plugOutException<<31)));
                   hal_HstSendEvent(SYS_EVENT,(gUsbRegs->HPRT & 1));
                   hal_HstSendEvent(SYS_EVENT,plugOutException);
			//let's wait a moment here.
			//hal_TimDelay(1 MS_WAITING);
			if(( !(gUsbRegs->HPRT & 1))|| plugOutException)
			{
				extern void(*g_pfDisconnCb)(BOOL isHost);
				int i = 1;
				
				gUsbRegs->GAHBCFG = 0;
				gUsbRegs->GINTMSK = 0;
				gUsbHostInst.portEnabled = 0;
				
				for(; i<gUsbHostInst.endNum+1; i++)
				{
					if(gUsbHostInst.epArray[i].pBulkReq->completeCb)
					{
						hal_HstSendEvent(SYS_EVENT,0x10140003);
						*gUsbHostInst.epArray[i].pBulkReq->pstatus = USBHCD_DEVICE_DISCONNECTED;
						gUsbHostInst.epArray[i].pBulkReq->completeCb(NULL, 0);
						break;
					}
				}
				if(g_pfDisconnCb != NULL)
				g_pfDisconnCb(1);
			}
			hal_HstSendEvent(SYS_EVENT,0x0917000b);
			hal_HstSendEvent(SYS_EVENT,intsts);
		}
		else if(reg & USBH_PRTCONNDET)
		{
			hal_HstSendEvent(SYS_EVENT,0x09170002);
			hal_HstSendEvent(USB_EVENT, gUsbRegs->HPRT);
			gUsbRegs->HPRT |= USBH_PRTCONNDET;
#if 1
			if(gUsbHostInst.InitCompleteCb)
			{
				gUsbHostInst.InitCompleteCb(NULL, 0);
				gUsbHostInst.InitCompleteCb = NULL;
				hal_HstSendEvent(0xff,0x09170004);
			}
#endif
		}
	}

	/*host channels interrupt*/
	if( intsts & USBC_HCHINT)
	{
		int i = 1;
		uint32 haint = gUsbRegs->HAINT;
		uint32 haintMsk = gUsbRegs->HAINTMSK;
		reg = 1;
		
		//endpoint 0 interrupt.
		if((reg&haint)& haintMsk)
		{	
			rda_ProcessControlInterrupt();
		}

		// data endpoints' interrupt
		for(; i<gUsbHostInst.endNum+1; i++)
		{
			reg <<= 1;
			if(reg&haint)
			{
				int hcint = gUsbRegs->HCSR[i].HCINTn;
				USBENDPOINT *pEnd = &gUsbHostInst.epArray[i];

				gUsbRegs->HCSR[i].HCINTn = 0x07ff;
				
				if( (hcint&HCNI_CHHLTD) || (hcint & HCNI_ACK) )
				{
					if((hcint & HCNI_XFERCOMPL) || (hcint & HCNI_ACK))
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
						pEnd->DataToggle = pEnd->DataToggle==USB_DATA0?USB_DATA1:USB_DATA0;
		
						if( RDAUsb_PacketTransfer(i) > 0)
						{
							//hal_HstSendEvent(SYS_EVENT,0x55000000+hcint+(reg<<16));
							//hal_HstSendEvent(SYS_EVENT,pEnd->nCurTransferred);
							*pEnd->pBulkReq->pstatus = USB_NO_ERROR;
							*pEnd->pBulkReq->pnByteTransferred = pEnd->nCurTransferred;
							
							if(pEnd->pBulkReq->completeCb)
							{
								pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, USB_NO_ERROR);
							}
						}
					}
					else 
					{
					    if((hcint & HCNI_XACTERR) || (hcint & HCNI_STALL))// USB will powder down
						{
						    gUsbRegs->GAHBCFG = 0;
            				gUsbRegs->GINTMSK = 0;
            				gUsbHostInst.portEnabled = 0;
						    extern void(*g_pfDisconnCb)(BOOL);
						    hal_HstSendEvent(USB_EVENT, 0xc1234567);
						    hal_HstSendEvent(USB_EVENT, hcint);
						    hal_HstSendEvent(USB_EVENT, (UINT32)g_pfDisconnCb);                            
                            if(g_pfDisconnCb != NULL)
                            {
                                g_pfDisconnCb(1);
                            }
						}
						else if(hcint & HCNI_NAK)
						{	
							//try again
							*pEnd->pBulkReq->pstatus = USB_RECEIVED_NAK;
							*pEnd->pBulkReq->pnByteTransferred = 0;
							hal_HstSendEvent(USB_EVENT, 0xc1234566);
						}
						else if(hcint & HCNI_ACK)
						{
							*pEnd->pBulkReq->pstatus = USB_RECEIVED_ACK;
							*pEnd->pBulkReq->pnByteTransferred = pEnd->nCurTransferred;
							hal_HstSendEvent(USB_EVENT, 0xc1234565);
						}/*
						else if(hcint & HCNI_STALL)
						{
							*pEnd->pBulkReq->pstatus = USB_STALL_ERROR;
						}*/

						if( pEnd->pBulkReq->completeCb )
						{
    						hal_HstSendEvent(USB_EVENT, 0xc1234563);
    						hal_HstSendEvent(USB_EVENT, hcint);
    						hal_HstSendEvent(USB_EVENT, i);
							pEnd->pBulkReq->completeCb(pEnd->pBulkReq->handle, 0);
						}
					}
					
				}
				else if( hcint & HCNI_ACK )
				{
					hal_HstSendEvent(SYS_EVENT, 0x15550001);
				}
				else if( (hcint & HCNI_NAK) && (pEnd->Epaddress&USB_END0_DIRIN))
				{
					//agony experience, there are too many IN NAK interrupt result for system dead. 
					gUsbRegs->HCSR[i].HCINTMSKn &= ((~HCNI_NAK_MSK)&0x7ff);
				}

			}
		}
		
	}
	
}

#endif

#endif


