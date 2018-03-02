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
/// @file hal_usb.c
///
/// Hal USB public function
//
////////////////////////////////////////////////////////////////////////////////

#if (CHIP_HAS_USB == 1)&& !defined(DISABLE_USB_SUPPORT)

#if defined(HAL_USB_PRINTF) && defined(HAL_TRACE_FORCE_USB)
  #error "You can t use HAL_USB_PRINTF with HAL_TRACE_FORCE_USB"
#endif

#ifdef HAL_USB_PRINTF
  #define HAL_USB_TRACE HAL_TRACE
#else
  #define HAL_USB_TRACE(level, tsmap, fmt, ...)
#endif /* HAL_USB_PRINTF */

#include "pmd_m.h"

#include "sxr_ops.h"
#include "halp_sys.h"

#include "halp_usb.h"
#include "halp_debug.h"
//#include "halp_version.h"
#include "hal_usb.h"
#include "hal_host.h"
#include "hal_ispi.h"

#include "boot_usb.h"

#include "string.h"
#include "boot_ispi.h"
#include "cfg_regs.h"
#include "global_macros.h"
#include "usbc.h"
#include "sys_ctrl.h"
#include "event.h"
#include "cos.h"
#include "iomux.h"
#include "musb_core.h"
#include "usb_role_det.h"
#include "hal_gpio.h"
#include "usbc.h"
#include "tgt_app_cfg.h"
#include "musb_core.h"
// =============================================================================
// MACRO
// =============================================================================

#define EPIN       0x01
#define EPOUT      0x02
#define EPINOUT    0x00

#define EPDIR(n)   (((hwp_usbc->GHWCFG1)>>(2*n))&0x03)

#define SINGLE     0
#define INCR       1
#define INCR4      3
#define HAL_USB_AHB_MODE    INCR4


#define RXFIFOSIZE 64
#define TXFIFOSIZE 48

#ifndef USB_MAX_STRING
#define USB_MAX_STRING 8
#endif /* USB_MAX_STRING */

#define HAL_USB_OPERATE_TIME 1638

// =============================================================================
// TYPES
// =============================================================================

// =============================================================================
// HAL_USB_REQUEST_DESTINATION_T
// -----------------------------------------------------------------------------
/// Destination of USB command
// =============================================================================
typedef enum
{
    HAL_USB_REQUEST_DESTINATION_DEVICE    = 0,
    HAL_USB_REQUEST_DESTINATION_INTERFACE = 1,
    HAL_USB_REQUEST_DESTINATION_EP        = 2
} HAL_USB_REQUEST_DESTINATION_T ;

// =============================================================================
// HAL_USB_REQUEST_DEVICE_T
// -----------------------------------------------------------------------------
/// List of device request
// =============================================================================
typedef enum
{
    HAL_USB_REQUEST_DEVICE_SETADDR = 0x05,
    HAL_USB_REQUEST_DEVICE_GETDESC = 0x06,
    HAL_USB_REQUEST_DEVICE_SETCONF = 0x09
    //HAL_USB_REQUEST_DEVICE_TEST_MODE=0x10
    
} HAL_USB_REQUEST_DEVICE_T ;

// =============================================================================
// HAL_USB_DEVICE_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Device descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT16 bcdUsb;
    UINT8  usbClass;
    UINT8  usbSubClass;
    UINT8  usbProto;
    UINT8  ep0Mps;
    UINT16 vendor;
    UINT16 product;
    UINT16 release;
    UINT8  iManufacturer;
    UINT8  iProduct;
    UINT8  iSerial;
    UINT8  nbConfig;
} PACKED HAL_USB_DEVICE_DESCRIPTOR_REAL_T;

// =============================================================================
// HAL_USB_CONFIG_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Config descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT16 totalLength;
    UINT8  nbInterface;
    UINT8  configIndex;
    UINT8 iDescription;
    UINT8  attrib;
    UINT8  maxPower;
}  PACKED HAL_USB_CONFIG_DESCRIPTOR_REAL_T;
// =============================================================================
// HAL_USB_CONFIG_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Config descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT16 totalLength;
    UINT8  nbInterface;	
	UINT8  bConfigurationValue;
    UINT8  configIndex;
    UINT8  attrib;
    UINT8  maxPower;
}  PACKED HAL_USBA_CONFIG_DESCRIPTOR_REAL_T;
// =============================================================================
// HAL_USB_INTERFACE_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// Interface descriptor structure
// =============================================================================
typedef struct
{
    UINT8  size;
    UINT8  type;
    UINT8  interfaceIndex;
    UINT8  alternateSetting;
    UINT8  nbEp;
    UINT8  usbClass;
    UINT8  usbSubClass;
    UINT8  usbProto;
    UINT8  iDescription;
} PACKED HAL_USB_INTERFACE_DESCRIPTOR_REAL_T;
// =============================================================================
// HAL_USB_ISO_EP_DESCRIPTOR_REAL_T
// -----------------------------------------------------------------------------
/// ISO Ep descriptor structure
// =============================================================================
typedef struct
{
   UINT8  size;
   UINT8  type;
   UINT8  ep;
   UINT8  attributes;
   UINT16 mps;
   UINT8  interval;
   UINT8 bRefreash;
   UINT8 bSynchAddr;
} PACKED HAL_USB_ISO_EP_DESCRIPTOR_REAL_T;
// =============================================================================
// EP0_STATE_T
// -----------------------------------------------------------------------------
/// State of ep0
// =============================================================================
typedef enum
{
    EP0_STATE_IDLE,
    EP0_STATE_IN,
    EP0_STATE_OUT,
    EP0_STATE_STATUS_IN,
    EP0_STATE_STATUS_OUT
} EP0_STATE_T ;

// =============================================================================
// HAL_USB_TRANSFERT_T
// -----------------------------------------------------------------------------
/// Structure containt the transfert parameter
// =============================================================================
typedef struct
{
    BOOL  transferDone;
	INT32  sizeRemaining;
	INT32  sizeTransfered;
	UINT8 *buffer;
} HAL_USB_MGC_TRANSFER_T;

// =============================================================================
// HAL_USB_GLOBAL_VAR_T
// -----------------------------------------------------------------------------
/// Structure with all global var
// =============================================================================
typedef struct
{
    HAL_USB_GETDESCRIPTOR_CALLBACK_T DeviceCallback;
    HAL_USB_DEVICE_DESCRIPTOR_T*     Desc;
    HAL_USB_CALLBACK_T               EpInCallback [DIEP_NUM];
    HAL_USB_CALLBACK_T               EpOutCallback[DOEP_NUM];
    HAL_USB_SETUP_T                  lastSetup;
    UINT32                           EpFlag;
    UINT32                           EpInterruptMask;
    UINT8*                           String       [USB_MAX_STRING];

	// Transfert data
	HAL_USB_MGC_TRANSFER_T             Transfert [1+DIEP_NUM+1+DOEP_NUM];

	// Ep0 State
	HAL_USB_SETUP_REQUEST_DESC_T     RequestDesc;
	UINT8                            Ep0State;
	UINT8                            Ep0Index;

	UINT8                            Config;
	UINT8                            NbEp;
	UINT8                            NbInterface;
	UINT8                            NbString;
	UINT8					FifoAddr;
	UINT8					configData;
	UINT8 					vbuserr_retry;
	UINT8					Varvbuserr_retry;
	UINT8					port1_status;
	UINT8 					usbAddr;
} HAL_USB_GLOBAL_VAR_T;

// =============================================================================
// PRIVATE VARIABLES
// =============================================================================

PRIVATE HAL_USB_GLOBAL_VAR_T g_HalUsbVar =
{
    .DeviceCallback = 0             ,
    .Desc           = 0             ,
    .NbEp           = 0             ,
    .NbInterface    = 0             ,
    .NbString       = 0             ,
    .Ep0State       = EP0_STATE_IDLE,
    .Config         = 0xFF
};


static UINT8 Varvbuserr_retry = 0;
UINT32 g_USB_DEVICE=0;

UINT32 usb_poweron_det=0;
UINT8 g_sys_timer_id_usb_detect;
extern UINT8 usb_music_udiskready;
extern BYTE g_usb_connected;
extern u16 count_MUSB_RXCOUNT;


// Ep0 Buffer
PRIVATE UINT8 USB_UCBSS_INTERNAL 	g_HalUsbBufferEp0Out[HAL_USB_MPS];
PRIVATE UINT8 USB_UCBSS_INTERNAL 	g_HalUsbBufferEp0In [HAL_USB_MPS*4];
PRIVATE UINT8 g_UsbMode = 0;
void(*g_pfDisconnCb)(BOOL isHost)  = NULL;
// =============================================================================
// PRIVATE FUNCTION PROTOTYPE
// =============================================================================

PRIVATE        VOID hal_UsbConfigureEp(HAL_USB_EP_DESCRIPTOR_T* ep);
PRIVATE        VOID hal_UsbDecodeEp0Packet(VOID);
PRIVATE        VOID hal_UsbEnableEp(UINT8 ep, HAL_USB_EP_TYPE_T type);
PRIVATE        VOID hal_UsbDisableEp(UINT8 ep);
PRIVATE        VOID hal_UsbClrConfig(VOID);
INLINE         VOID hal_UsbStatusIn(VOID);
INLINE         VOID hal_UsbStatusOut(VOID);
INLINE         HAL_USB_CALLBACK_RETURN_T
hal_UsbCallbackEp(UINT8                      ep,
                  HAL_USB_CALLBACK_EP_TYPE_T type,
                  HAL_USB_SETUP_T*           setup);
INLINE         HAL_USB_CALLBACK_RETURN_T
hal_UsbCallbackInterface(UINT8                       ep,
                         HAL_USB_CALLBACK_EP_TYPE_T  type,
                         HAL_USB_SETUP_T*            setup);
PRIVATE UINT16 hal_generateDescConfig(HAL_USB_CONFIG_DESCRIPTOR_T* cfg,
                                      UINT8*                       buffer,
                                      UINT8                        num);
PRIVATE UINT16
hal_generateDescEp(HAL_USB_EP_DESCRIPTOR_T* ep,
                   UINT8*                   buffer);
PRIVATE UINT16
hal_generateDescInterface(HAL_USB_INTERFACE_DESCRIPTOR_T* interface,
                          UINT8*                          buffer,
                          UINT8                           num);

PRIVATE VOID hal_UsbCancelTransfert(UINT8 ep);

// =============================================================================
// hal_UsbFlushRxFifo
// -----------------------------------------------------------------------------
/// Flux reception USB fifo
// =============================================================================
PRIVATE VOID hal_UsbFlushRxFifo(VOID);

// =============================================================================
// hal_UsbFlushTxFifo
// -----------------------------------------------------------------------------
/// Flush transmition USB FIFO
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
// =============================================================================
PRIVATE VOID hal_UsbFlushTxFifo(UINT8 ep);

// =============================================================================
// hal_UsbFlushAllTxFifos
// -----------------------------------------------------------------------------
/// Flush all transmition USB FIFOS
// =============================================================================
PRIVATE VOID hal_UsbFlushAllTxFifos(VOID);

// =============================================================================
// hal_usbAsciiToUtf8
// -----------------------------------------------------------------------------
/// Convert string to utf8
/// @param utf8 destination string
/// @param acsii source string
// =============================================================================
PRIVATE VOID hal_usbAsciiToUtf8(UINT8 *utf8, UINT8 *acsii);


void musb_g_rx(UINT8 epnum);
void musb_g_tx(UINT8 epnum);
void hal_musb_start();


extern VOID umss_Close(void);
extern void musb_host_rx_int(u8 epnum);
extern void musb_host_tx_int(u8 epnum);
extern void usb_host_end0_Interrupt();
extern void hal_musb_host_connect(void);


static bool hal_mgc_usb_highSpeed()
{
	UINT8 reg = musb_readb( MUSB_POWER);
	return reg & MUSB_POWER_HSMODE;
}

// =============================================================================
// PRIVATE FUNCTION
// =============================================================================

PRIVATE VOID hal_usbAsciiToUtf8(UINT8 *utf8, UINT8 *acsii)
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

PRIVATE VOID hal_UsbFlushRxFifo(VOID)
{

}

PRIVATE VOID hal_UsbFlushTxFifo(UINT8 ep)
{
    UINT8 epNum;

    epNum              = HAL_USB_EP_NUM(ep);

}

PRIVATE VOID hal_UsbFlushAllTxFifos(VOID)
{
    //UINT32 second_time,first_time;
}

#if 0
#ifdef CFG_USB_TEST_MODE
/**
 * usb_test_mode2usbhw_test_mode - Set usb test mode
 */
 //shaohuima 2016/3/8
static UINT8 usb_test_mode2usbhw_test_mode(UINT8 usb_test_mode)
{
    switch(usb_test_mode)
    {
        case 1:     return MUSB_TEST_J;
        case 2:     return MUSB_TEST_K;
        case 3:     return MUSB_TEST_SE0_NAK;
        case 4:     return MUSB_TEST_PACKET;
        /* Following test mode is vendor specific */
        case 0xC0:  return MUSB_TEST_FORCE_HS;
        case 0xC1:  return MUSB_TEST_FORCE_FS;
        case 0xC2:  return MUSB_TEST_FIFO_ACCESS;
        case 0xC3:  return MUSB_TEST_FORCE_HOST
			
        default:    return 0;
    }
}
#endif 
#endif
PRIVATE VOID hal_UsbResetTransfert(VOID)
{
	UINT8 i;

	for(i = 0; i < DIEP_NUM+DOEP_NUM+1; ++i)
	{
        g_HalUsbVar.Transfert[i].transferDone = TRUE;
		g_HalUsbVar.Transfert[i].sizeRemaining   = -1;
		g_HalUsbVar.Transfert[i].sizeTransfered  =  0;
		g_HalUsbVar.Transfert[i].buffer =  NULL;
	}
}

PRIVATE VOID hal_UsbCancelTransfert(UINT8 ep)
{
    UINT8               epNum;

    HAL_PROFILE_FUNCTION_ENTER(hal_UsbCancelTransfert);

    epNum = HAL_USB_EP_NUM(ep);
	if(HAL_USB_IS_EP_DIRECTION_IN(ep))
	{
	    g_HalUsbVar.Transfert[epNum].transferDone = TRUE;
        g_HalUsbVar.Transfert[epNum].sizeRemaining = 0;
    	g_HalUsbVar.Transfert[epNum].sizeTransfered  =  0;
    	g_HalUsbVar.Transfert[epNum].buffer =  NULL;
	}
	else
	{        
        g_HalUsbVar.Transfert[DIEP_NUM+epNum+1].transferDone = TRUE;
        g_HalUsbVar.Transfert[DIEP_NUM+epNum+1].sizeRemaining = 0;
    	g_HalUsbVar.Transfert[DIEP_NUM+epNum+1].sizeTransfered  =  0;
    	g_HalUsbVar.Transfert[DIEP_NUM+epNum+1].buffer =  NULL;
	}

    HAL_PROFILE_FUNCTION_EXIT(hal_UsbCancelTransfert);
}

PRIVATE BOOL hal_UsbContinueTransferDone(UINT8 ep,UINT8 size)
{
    UINT8               epNum;
    HAL_USB_MGC_TRANSFER_T *pTransfer;
    UINT16 csr0;
    BOOL ret = FALSE;
    epNum = HAL_USB_EP_NUM(ep);

    if(HAL_USB_IS_EP_DIRECTION_IN(ep))
    {
        INT16 length = 0;
        if(epNum==0)
        {
            pTransfer = &g_HalUsbVar.Transfert[epNum];
            pTransfer->sizeTransfered += 0x40;
            pTransfer->sizeRemaining -= 0x40;
            csr0 = musb_readw( MUSB_CSR0);
            if(pTransfer->sizeRemaining >= 0x40)
            {
                length = 0x40;
                csr0 |= MUSB_CSR0_TXPKTRDY;
            }
            else
            {
                length = pTransfer->sizeRemaining;
                csr0 |= MUSB_CSR0_TXPKTRDY|MUSB_CSR0_P_DATAEND;
                ret = TRUE;
            }
            //mon_Event(0x17022802);
            //mon_Event(length);
            if(length > 0)
            {
                musb_write_fifo_ram(0, pTransfer->buffer+pTransfer->sizeTransfered, length);
            }
            else
            {
                ret = TRUE;
            }
            musb_writew(MUSB_CSR0, csr0);
         }
         return ret;
     }
     else
     {
        if(epNum==0)
        {
            pTransfer = &g_HalUsbVar.Transfert[epNum];
        }
        else
        {
            pTransfer = &g_HalUsbVar.Transfert[epNum+DIEP_NUM+1];
        }

        pTransfer->transferDone = FALSE;
        if(size>pTransfer->sizeRemaining) 
            return FALSE;

        musb_read_fifo_ram(0, pTransfer->buffer + pTransfer->sizeTransfered,  size);
        musb_writew(MUSB_CSR0, MUSB_CSR0_P_SVDRXPKTRDY); //MUSB_CSR0_P_DATAEND
        pTransfer->sizeRemaining -= size;
        pTransfer->sizeTransfered+= size;

        if(pTransfer->sizeRemaining==0)
            return TRUE;
 
     }
     return ret;
}
PRIVATE VOID hal_UsbConfigureEp(HAL_USB_EP_DESCRIPTOR_T* ep)
{
    UINT8 epNum;

    epNum = HAL_USB_EP_NUM(ep->ep);

    if(HAL_USB_IS_EP_DIRECTION_IN(ep->ep))
    {
        g_HalUsbVar.EpInCallback[epNum-1]  = ep->callback;
        hal_UsbEnableEp(HAL_USB_EP_DIRECTION_IN(epNum), ep->type);
    }
    else
    {
        g_HalUsbVar.EpOutCallback[epNum-1] = ep->callback;
        hal_UsbEnableEp(HAL_USB_EP_DIRECTION_OUT(epNum), ep->type);
    }
}

// =============================================================================
// boot_UsbInit
// -----------------------------------------------------------------------------
/// (Re)Initializes the USB state machine, clear the IRQ and stop the transfers.
// =============================================================================
PRIVATE VOID hal_UsbInit(VOID)
{
    // Flush fifo
    hal_UsbFlushAllTxFifos();
    hal_UsbFlushRxFifo();

    // Reset EP0 state
    g_HalUsbVar.Ep0State        = EP0_STATE_IDLE;
    g_HalUsbVar.NbString        = 0;
    g_HalUsbVar.usbAddr = 0xff;

    hal_UsbClrConfig();
    g_HalUsbVar.Desc            = 0;

    hal_UsbResetTransfert();
    //*(volatile UINT32*)(0xa1a80084) = 0x3;
}

PRIVATE VOID hal_UsbConfig(VOID)
{
	UINT8 power;
	UINT8 epinfo;

	// Nb EP
	hal_HstSendEvent(USB_EVENT, 0x14063003);
	epinfo = musb_readb( MUSB_EPINFO);
	g_HalUsbVar.NbEp = (epinfo&0x0f) + ((epinfo>>4)&0x0f);
	g_HalUsbVar.NbEp++;

/*
	power = musb_readb( MUSB_POWER);
	power &= ~MUSB_POWER_SOFTCONN;
	musb_writeb(MUSB_POWER, power);

	musb_writew(MUSB_INTRTXE, 0x01);
	
	hal_TimDelay(16384);
*/	
	//enable HS negotiable shaohuima 
	power = musb_readb( MUSB_POWER);
	power |=  MUSB_POWER_SOFTCONN;      // | MUSB_POWER_HSENAB shaohuima;
	//power &= ~MUSB_POWER_SOFTCONN;
	musb_writeb( MUSB_POWER, power);

	//tx, rx fifo if dynamic
	musb_select_ep(0);
	g_HalUsbVar.configData = musb_readb( MUSB_CONFIGDATA);
	if(g_HalUsbVar.configData & MUSB_CONFIGDATA_DYNFIFO)
	{
		g_HalUsbVar.FifoAddr = 64;
		musb_writew( MUSB_TXFIFOADD, g_HalUsbVar.FifoAddr>>3);
		musb_writeb( MUSB_TXFIFOSZ, 0x06);		//  512 bytes
		g_HalUsbVar.FifoAddr += 512;
		musb_writew( MUSB_RXFIFOADD, g_HalUsbVar.FifoAddr>>3);
		musb_writew( MUSB_RXFIFOSZ, 0x06);   //  512 bytes
		g_HalUsbVar.FifoAddr += 512;
	}
		
	hal_UsbFlushRxFifo();
	hal_UsbFlushAllTxFifos();
	
}

// =============================================================================
// hal_UsbDisableEp
// -----------------------------------------------------------------------------
/// Disable an endpoint.
///
/// Please note that there is a particular behaviour for endpoint 0.
/// @param ep Endpoint to disable.
// ============================================================================= 
PRIVATE VOID hal_UsbDisableEp(UINT8 ep)
{
    UINT8   epNum;
    UINT8   offset = 0;
    /*REG32*  diepctl;
    REG32*  doepctl;
    REG32*  ctl;
    REG32*  diepint;
    REG32*  doepint;
    REG32*  epint;
    UINT32 second_time,first_time;
    first_time = hal_TimGetUpTime();*/


    HAL_PROFILE_FUNCTION_ENTER(hal_UsbDisableEp);

    epNum = HAL_USB_EP_NUM(ep);
    HAL_ASSERT(epNum < 16, "hal_UsbDisableEp: Ep number invalid");

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

    hal_UsbFlushTxFifo(epNum);

    hal_UsbCancelTransfert(ep);

    // We don't disable EP0 interrupt as it is driving the USB
    // stuff. Closing the USB will disable them though.
    if (epNum != 0)
    {
        // Disable interrupt mask for this EP
        g_HalUsbVar.EpInterruptMask &= ~(1<<(epNum+offset));
    }

    HAL_PROFILE_FUNCTION_EXIT(hal_UsbDisableEp);
}

PRIVATE VOID hal_UsbEnableEp(UINT8 ep, HAL_USB_EP_TYPE_T type)
{
	UINT8   epNum;
	UINT8   offset = 0;

	UINT16  maxP, csr;

	UINT32 csStatus = hal_SysEnterCriticalSection();

	epNum = HAL_USB_EP_NUM(ep);

	//hal_HstSendEvent(USB_EVENT, 0x14070801);
	//hal_HstSendEvent(USB_EVENT, ep);

	musb_select_ep(epNum);
	if( hal_mgc_usb_highSpeed() )
	{
		maxP  = HAL_USB_MPS_HS;
	}
	else
    {
        if(type == HAL_USB_EP_TYPE_ISO)
            maxP = HAL_USB_ISO_OUT_MPS;
        else 
            maxP = HAL_USB_MPS;	
	}
	
	// Select direction
	if(HAL_USB_IS_EP_DIRECTION_IN(ep))
	{
	    if(type == HAL_USB_EP_TYPE_ISO)
            maxP = HAL_USB_ISO_IN_MPS;
		musb_writew( MUSB_TXMAXP, maxP);
		csr = musb_readw( MUSB_TXCSR);
		if(type == HAL_USB_EP_TYPE_ISO)
		{
			csr |= MUSB_TXCSR_P_ISO;
		}

		csr |= MUSB_TXCSR_MODE;
		//musb_writew(MUSB_TXCSR, csr);
		csr |=  MUSB_TXCSR_CLRDATATOG;// | MUSB_TXCSR_FLUSHFIFO;// | MUSB_TXCSR_TXPKTRDY;
		musb_writew( MUSB_TXCSR, csr);

		csr = musb_readw( MUSB_INTRTXE);
        csr |= 1<<epNum;
		musb_writew( MUSB_INTRTXE, csr);
	}
	else
	{
		musb_writew( MUSB_RXMAXP, maxP);
		csr = musb_readw( MUSB_RXCSR);
		if(type == HAL_USB_EP_TYPE_ISO)
		{
			csr |= MUSB_RXCSR_P_ISO;
		}
		csr |=  MUSB_RXCSR_CLRDATATOG | MUSB_RXCSR_FLUSHFIFO;// | MUSB_RXCSR_RXPKTRDY;
		musb_writew( MUSB_RXCSR, csr);

		csr = musb_readw( MUSB_INTRRXE);
		csr |= 1<<epNum;
		musb_writew( MUSB_INTRRXE, csr);
	}

	g_HalUsbVar.EpInterruptMask |= (1<<(epNum+offset));

	HAL_PROFILE_FUNCTION_EXIT(hal_UsbEnableEp);

	hal_SysExitCriticalSection(csStatus);

}

INLINE         UINT8 hal_UsbAddString(UINT8 *str)
{
    UINT8 i;

    if(str == 0)
    {
        return(0);
    }
    for(i = 0; i < g_HalUsbVar.NbString; ++i)
    {
        if(g_HalUsbVar.String[i] == str)
        {
            return(i+1);
        }
    }
    g_HalUsbVar.String[g_HalUsbVar.NbString] = str;
    g_HalUsbVar.NbString++;
    return(g_HalUsbVar.NbString);
}

PRIVATE VOID hal_generateDescDevice(HAL_USB_DEVICE_DESCRIPTOR_T* dev,
                                    UINT8*                       buffer)
{
    HAL_USB_DEVICE_DESCRIPTOR_REAL_T* desc;
    UINT8                             i;

    desc = (HAL_USB_DEVICE_DESCRIPTOR_REAL_T*) buffer;
    desc->size          = sizeof(HAL_USB_DEVICE_DESCRIPTOR_REAL_T);
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
        desc->product   =0x0900;// BOOT_USB_PRODUCT_ID(dev->usbMode);
    }
    else
    {
        desc->product   = dev->product;
    }

    if(dev->version == 0)
    {
        desc->release   = BOOT_USB_REVISION_ID;
    }
    else
    {
        desc->release   = dev->version;
    }
    desc->iManufacturer = hal_UsbAddString((UINT8*)"RDA Microelectronics");
    desc->iProduct      = hal_UsbAddString(dev->description);
    desc->iSerial       = hal_UsbAddString(dev->serialNumber);
    i = 0;
    if(dev->configList)
    {
        for(i = 0; dev->configList[i]; ++i);
    }
    desc->nbConfig             = i;
}

PRIVATE UINT16 hal_generateDescConfig(HAL_USB_CONFIG_DESCRIPTOR_T* cfg,
                                      UINT8*                       buffer,
                                      UINT8                        num)
{

    UINT32                            i;
    HAL_USB_CONFIG_DESCRIPTOR_REAL_T* desc;
    UINT16                            size;	

#ifdef USB_AUDIO_SUPPORT
    HAL_USBA_CONFIG_DESCRIPTOR_REAL_T* descA;

	if(HAL_USB_MODE_AUDIO == hal_UsbGetMode())
	{
       descA = (HAL_USBA_CONFIG_DESCRIPTOR_REAL_T*) buffer;
       descA->size         = sizeof(HAL_USBA_CONFIG_DESCRIPTOR_REAL_T);
       size               = sizeof(HAL_USBA_CONFIG_DESCRIPTOR_REAL_T);
       descA->type         = 2;
       descA->nbInterface  = 0;
    	if(cfg->iad != NULL)
    	{
    		memcpy(&buffer[size],cfg->iad,sizeof(HAL_USB_INTERFACE_ASSOCIATION_DESCRIPTOR_REAL_T));
    		size += sizeof(HAL_USB_INTERFACE_ASSOCIATION_DESCRIPTOR_REAL_T);
    	}
       if(cfg->interfaceList)
       {
           for(i = 0; cfg->interfaceList[i]; ++i)
           {
               size += hal_generateDescInterface(cfg->interfaceList[i],
                                                 &buffer[size], descA->nbInterface);
            if((cfg->interfaceList[i]->interfaceIdx != 0xFF)&&
            	((cfg->interfaceList[i]->interfaceIdx != 0xF0))
              )
            {	 
                  ++descA->nbInterface;
               }
           }
       }
   	   descA->bConfigurationValue = 0x01;
       descA->configIndex  = cfg->configIdx;
       descA->attrib       = cfg->attrib;
       descA->maxPower     = cfg->maxPower;
       descA->totalLength  = size;
       HAL_USB_TRACE(HAL_USB_TRC, 0, "Configuration descriptor size %i\n", size);
       HAL_ASSERT(size <= sizeof(g_HalUsbBufferEp0In), "Configuration descriptor too big %i", size);

       hal_HstSendEvent(USB_EVENT, 0x11060001);
       return(size);
	}
	else
#endif
	{
		desc = (HAL_USB_CONFIG_DESCRIPTOR_REAL_T*) buffer;
		desc->size		   = sizeof(HAL_USB_CONFIG_DESCRIPTOR_REAL_T);
		size			   = sizeof(HAL_USB_CONFIG_DESCRIPTOR_REAL_T);
		desc->type		   = 2;
		desc->nbInterface  = 0;
		if(cfg->iad != NULL)
		{
			memcpy(&buffer[size],cfg->iad,sizeof(HAL_USB_INTERFACE_ASSOCIATION_DESCRIPTOR_REAL_T));
			size += sizeof(HAL_USB_INTERFACE_ASSOCIATION_DESCRIPTOR_REAL_T);
		}
		if(cfg->interfaceList)
		{
			for(i = 0; cfg->interfaceList[i]; ++i)
			{
				size += hal_generateDescInterface(cfg->interfaceList[i],
												  &buffer[size], desc->nbInterface);
				if(cfg->interfaceList[i]->interfaceIdx != 0xFF)
				{
                    ++desc->nbInterface;
                }
            }
        }
        desc->configIndex= cfg->configIdx;
        desc->iDescription = hal_UsbAddString(cfg->description);
        desc->attrib       = cfg->attrib;
        desc->maxPower     = cfg->maxPower;
        desc->totalLength  = size;

        HAL_ASSERT(size <= sizeof(g_HalUsbBufferEp0In), "Configuration descriptor too big %i", size);

		return(size);
	}
}

PRIVATE UINT16
hal_generateDescInterface(HAL_USB_INTERFACE_DESCRIPTOR_T* interface,
                          UINT8*                          buffer,
                          UINT8                           num)
{
    UINT8                                j;
    HAL_USB_INTERFACE_DESCRIPTOR_REAL_T* desc;
    UINT16                               size;
    HAL_USB_CS_INTERFACE_DESCRIPTOR_T*   csDesc;

    csDesc = (HAL_USB_CS_INTERFACE_DESCRIPTOR_T*)   interface;
    desc   = (HAL_USB_INTERFACE_DESCRIPTOR_REAL_T*) buffer;
//    static UINT8 alternateSetting = 0;
    if(interface->interfaceIdx == 0xFF)
    { // CS_INTERFACE,and other descriptor
        size                    = csDesc->constructor(buffer);
    }
    else
    {
        desc->size              = sizeof(HAL_USB_INTERFACE_DESCRIPTOR_REAL_T);
        size                    = sizeof(HAL_USB_INTERFACE_DESCRIPTOR_REAL_T);
        // Interface type = 4
        desc->type              = 4;
        desc->interfaceIndex    = num;

#ifndef USB_AUDIO_SUPPORT      
      desc->alternateSetting = 0;      
#else
      //for Audio Class
      if(interface->interfaceIdx == 0x0F)
      {
         desc->alternateSetting = 1;
      }
      else
      {
         desc->alternateSetting = 0;
      }
#endif
      //for HID interface     
      if(interface->usbClass == 0x03)
      {
    	  desc->nbEp			  = 1;
      }
      else
      {
         j = 0;
    	     //hal_HstSendEvent(0x19920708);
         if(interface->epList)
         {
            for(j = 0; interface->epList[j]; ++j)
            {
               size += hal_generateDescEp(interface->epList[j],&buffer[size]);
               //hal_HstSendEvent(USB_EVENT, 0x11121002);
            }
         }
    	 desc->nbEp 			 = j;
    	 //hal_HstSendEvent(USB_EVENT, 0x11121003);
    	 //hal_HstSendEvent(USB_EVENT, j);
      }
        desc->usbClass          = interface->usbClass   ;
        desc->usbSubClass       = interface->usbSubClass;
        desc->usbProto          = interface->usbProto   ;
        desc->iDescription      = hal_UsbAddString(interface->description);
    }

    return(size);
}

PRIVATE UINT16
hal_generateDescEp(HAL_USB_EP_DESCRIPTOR_T* ep,
                   UINT8*                   buffer)
{
	HAL_USB_EP_DESCRIPTOR_REAL_T* desc;
	HAL_USB_ISO_EP_DESCRIPTOR_REAL_T* descIso;
	if(ep->type == HAL_USB_EP_TYPE_ISO)
	{
		descIso = (HAL_USB_ISO_EP_DESCRIPTOR_REAL_T*) buffer;
		descIso->size       = sizeof(HAL_USB_ISO_EP_DESCRIPTOR_REAL_T);
		descIso->attributes = ep->type|0x00;	  
		descIso->type       = 5;
		descIso->ep         = ep->ep;
		//hal_HstSendEvent(USB_EVENT, 0x11130003);
		//hal_HstSendEvent(USB_EVENT, ep->ep);

		if(HAL_USB_IS_EP_DIRECTION_IN(ep->ep))
		{
			descIso->mps = HAL_USB_ISO_IN_MPS;
		}
		else
		{
			descIso->mps = HAL_USB_ISO_OUT_MPS;
		}
		descIso->interval = ep->interval;
		descIso->bRefreash = 0x00;
		descIso->bSynchAddr = 0x00;
		return (descIso->size);

	}
	else
	{
		  desc = (HAL_USB_EP_DESCRIPTOR_REAL_T*) buffer;     
		  desc->size       = sizeof(HAL_USB_EP_DESCRIPTOR_REAL_T);
		  // EP type = 5
		  desc->type       = 5;
		  desc->ep         = ep->ep;
		  //hal_HstSendEvent(USB_EVENT, 0x11130004);
		  //hal_HstSendEvent(USB_EVENT, ep->ep);
		  desc->attributes = ep->type;
		  
		  desc->mps        = hal_mgc_usb_highSpeed() ? HAL_USB_MPS_HS:HAL_USB_MPS;
		  desc->interval   = ep->interval;
		  // EP Type (Cmd, Bulk, Iso, Int)
		  return(desc->size);
	}
}

INLINE	VOID hal_UsbStatusIn(VOID)
{
    	g_HalUsbVar.Ep0State = EP0_STATE_STATUS_IN;
}

INLINE	VOID hal_UsbStatusOut(VOID)
{
	g_HalUsbVar.Ep0State = EP0_STATE_STATUS_OUT;
}

// =============================================================================
// hal_UsbClrConfig
// -----------------------------------------------------------------------------
/// Clear the USB config. Stop the transfer on the endpoint (but ep0) and clear
/// the interrupts (but ep0).
// =============================================================================
PRIVATE VOID hal_UsbClrConfig(VOID)
{
    UINT8 i;

    if(g_HalUsbVar.Desc)
    {
        if(g_HalUsbVar.Config != 0xFF &&
           g_HalUsbVar.Desc->configList[g_HalUsbVar.Config] &&
           g_HalUsbVar.Desc->configList[g_HalUsbVar.Config]->interfaceList)
        {
            for(i = 0;
                g_HalUsbVar.Desc->configList[g_HalUsbVar.Config]
                    ->interfaceList[i];
                ++i)
            {
                if(g_HalUsbVar.Desc->configList[g_HalUsbVar.Config]
                   ->interfaceList[i]->callback)
                {
                    g_HalUsbVar.Desc->configList[g_HalUsbVar.Config]
                        ->interfaceList[i]->callback
                        (HAL_USB_CALLBACK_TYPE_DISABLE, 0);
                }
            }
        }
        g_HalUsbVar.Config = 0xFF;
    }

    // Disable all EP
    for(i = 0; i < DIEP_NUM; ++i)
    {
        g_HalUsbVar.EpInCallback[i]  = 0;
        hal_UsbDisableEp(HAL_USB_EP_DIRECTION_IN (i+1));
    }
    for(i = 0; i < DOEP_NUM; ++i)
    {
        g_HalUsbVar.EpOutCallback[i] = 0;
        hal_UsbDisableEp(HAL_USB_EP_DIRECTION_OUT(i+1));
    }

    // Endpoint 0 doesn't have endpoint callbacks other
    // than the IRQ handlers...
    hal_UsbDisableEp(HAL_USB_EP_DIRECTION_IN(0));
    hal_UsbDisableEp(HAL_USB_EP_DIRECTION_OUT(0));
}

INLINE         HAL_USB_CALLBACK_RETURN_T
hal_UsbCallbackEp(UINT8                      ep,
                  HAL_USB_CALLBACK_EP_TYPE_T type,
                  HAL_USB_SETUP_T*           setup)
{
    UINT8                     epNum;

    epNum = HAL_USB_EP_NUM(ep);

    if(epNum < 16 && epNum != 0)
    {
        if(HAL_USB_IS_EP_DIRECTION_IN(ep))
        {
            if(g_HalUsbVar.EpInCallback[epNum-1])
            {
                return(g_HalUsbVar.EpInCallback[epNum-1](type, setup));
            }
        }
        else
        {
            if(g_HalUsbVar.EpOutCallback[epNum-1])
            {
                return(g_HalUsbVar.EpOutCallback[epNum-1](type, setup));
            }
        }
    }
    
    return(HAL_USB_CALLBACK_RETURN_KO);
}

INLINE         HAL_USB_CALLBACK_RETURN_T
hal_UsbCallbackInterface(UINT8                      interface,
                         HAL_USB_CALLBACK_EP_TYPE_T type,
                         HAL_USB_SETUP_T*           setup)
{
	HAL_USB_INTERFACE_DESCRIPTOR_T* interfaceDesc = 0;
	UINT32                          i;
	UINT32                          nonCsItfIdx = 0;

	if(g_HalUsbVar.Desc == 0)
	{
		return HAL_USB_CALLBACK_RETURN_KO;
	}
	
	if(g_HalUsbVar.Config == 0xFF)
	{
		return HAL_USB_CALLBACK_RETURN_KO;
	}
	
	if(g_HalUsbVar.Desc->configList[g_HalUsbVar.Config] == 0)
	{
		return HAL_USB_CALLBACK_RETURN_KO;
	}
	for(i = 0;
	(interfaceDesc = g_HalUsbVar.Desc->configList[g_HalUsbVar.Config]->interfaceList[i]);
	++i)
	{
		if((interfaceDesc->interfaceIdx != 0xFF)&&(interfaceDesc->interfaceIdx != 0xF0))
		{
			if(nonCsItfIdx == interface)
			{
			    break;
			}
			nonCsItfIdx++;
		}
	}
	
	if(interfaceDesc == 0)
	{
		return HAL_USB_CALLBACK_RETURN_KO;
	}
	
	if(interfaceDesc->callback == 0)
	{
		return HAL_USB_CALLBACK_RETURN_KO;
	}
	return interfaceDesc->callback(type, setup);
}

PRIVATE VOID hal_UsbSetConfig(UINT8 num)
{
    UINT8 i = 0;
    UINT8 j;

    // Disable old config
    hal_UsbClrConfig();

    g_HalUsbVar.Config      = 0xFF;
    g_HalUsbVar.NbInterface = 0;

    // Search config index
	//hal_HstSendEvent(USB_EVENT,0x67676767);
	//hal_HstSendEvent(USB_EVENT,num);
#ifdef USB_AUDIO_SUPPORT
	if(HAL_USB_MODE_AUDIO == hal_UsbGetMode())
	{
	    for(i = 0; g_HalUsbVar.Desc->configList[i]; i++) 
	    {
           if(g_HalUsbVar.Desc->configList[i]->configIdx == (num-1))
           {            
			   hal_HstSendEvent(USB_EVENT,0xfafa0006);
               break;
           }
        }
	}
	else
#endif
	{
	    for(i = 0; g_HalUsbVar.Desc->configList[i]; ++i) 
	    {
           if(g_HalUsbVar.Desc->configList[i]->configIdx == num)
           {            
			   hal_HstSendEvent(USB_EVENT,0xfafa0006);
			   hal_HstSendEvent(USB_EVENT,i);
			   hal_HstSendEvent(USB_EVENT,num);
               break;
           }
        }
	}
    //hal_HstSendEvent(USB_EVENT,0xfafa0001);
    //hal_HstSendEvent(USB_EVENT,i);
    if(g_HalUsbVar.Desc->configList[i] == 0)
    {
        return;
    }
    // Save config index
    g_HalUsbVar.Config = i;
    num                = i;

	// Configure all interface end EP of this config
	if(g_HalUsbVar.Desc->configList[num]->interfaceList)
	{
		for(i = 0; g_HalUsbVar.Desc->configList[num]->interfaceList[i]; ++i)
		{
			if(g_HalUsbVar.Desc->configList[num]->interfaceList[i]->interfaceIdx != 0xFF)
			{
				if(g_HalUsbVar.Desc->configList[num]->interfaceList[i]->epList)
				{
					for(j = 0; g_HalUsbVar.Desc->configList[num]->interfaceList[i]->epList[j]; ++j)
					{                       
						hal_UsbConfigureEp(g_HalUsbVar.Desc->configList[num]->interfaceList[i]->epList[j]);
					}
				}
			}
			if(g_HalUsbVar.Desc->configList[num]->interfaceList[i]->callback)
			{
				g_HalUsbVar.Desc->configList[num]->interfaceList[i]->callback(HAL_USB_CALLBACK_TYPE_ENABLE, 0);
			}
		}
		g_HalUsbVar.NbInterface = i;
	}
}

PRIVATE VOID hal_UsbDecodeEp0Packet(VOID)
{
	HAL_USB_SETUP_T* setup;
	UINT16           size;
	UINT8            setup_completed = 0;
	UINT16		csr0;

LOOP:	
    musb_select_ep(0);
    csr0 = musb_readw(MUSB_CSR0);
    if(g_HalUsbVar.Ep0State==EP0_STATE_IDLE)
    {
        if(!(csr0 & MUSB_CSR0_RXPKTRDY))
            return ;
    }

    if(csr0 & MUSB_CSR0_P_SETUPEND)
    {
        musb_writew(MUSB_CSR0, MUSB_CSR0_P_SVDSETUPEND);
        /* Transition into the early status phase */
        switch (g_HalUsbVar.Ep0State) {
        case EP0_STATE_IN:
            g_HalUsbVar.Ep0State = EP0_STATE_STATUS_OUT;
            break;
        case EP0_STATE_OUT:
            g_HalUsbVar.Ep0State = EP0_STATE_STATUS_IN;
            break;
        default:
            break;
        }
        csr0 = musb_readw(MUSB_CSR0);
    }

	switch(g_HalUsbVar.Ep0State)
	{
	case EP0_STATE_IDLE  :
        size = musb_readw( MUSB_COUNT0);
        //unload fifo.
        musb_read_fifo_ram(0, g_HalUsbBufferEp0Out, size);
		//mon_Event(0x2015111e);
		//mon_Event(*(UINT32*)g_HalUsbBufferEp0Out);
		#if 0
		for(i=0;i<8;i++)
		{
          mon_Event(g_HalUsbBufferEp0Out[i]);

		}
		#endif
		setup = (HAL_USB_SETUP_T*) (g_HalUsbBufferEp0Out);
		// Change endian less
		//mon_Event((UINT32*)setup);
		setup->value     = HAL_ENDIAN_LITTLE_16(setup->value );
		setup->index     = HAL_ENDIAN_LITTLE_16(setup->index );
		setup->lenght    = HAL_ENDIAN_LITTLE_16(setup->lenght);
		
        g_HalUsbVar.lastSetup = *setup;
        g_HalUsbVar.RequestDesc = setup->requestDesc;
        g_HalUsbVar.Ep0Index    = setup->index&0xff;
        size = 0;

        musb_writew(MUSB_CSR0, MUSB_CSR0_P_SVDRXPKTRDY);

        if(setup->lenght != 0)
        {
            if(setup->requestDesc.requestDirection)
            {
                g_HalUsbVar.Ep0State = EP0_STATE_IN;
            }
            else
            {
                g_HalUsbVar.Ep0State = EP0_STATE_OUT;
            }
        }
        
		switch(setup->requestDesc.requestDest)
		{
		case HAL_USB_REQUEST_DESTINATION_DEVICE:
			switch(setup->request)
			{
			case HAL_USB_REQUEST_DEVICE_SETADDR:
				hal_HstSendEvent(USB_EVENT, 0x14082511);
				g_HalUsbVar.usbAddr = setup->value & 0xff;
				setup_completed = 1;
				break;
			case HAL_USB_REQUEST_DEVICE_SETCONF:
				hal_HstSendEvent(USB_EVENT, 0x14082512);
				if(g_HalUsbVar.Desc == 0)
				{
					break;
				}
				setup_completed = 1;
				hal_UsbSetConfig(setup->value&0xFF);
				break;
			case HAL_USB_REQUEST_DEVICE_GETDESC:
				// Fetch device descriptor   
				hal_HstSendEvent(USB_EVENT, 0x20151301);
				hal_HstSendEvent(USB_EVENT, (UINT32)g_HalUsbVar.Desc);
				g_USB_DEVICE=1;
				if(g_HalUsbVar.Desc == 0)
				{
				 
					break;
				}

				
				switch(setup->value>>8)
				{
				case 1: /* Device */
					
					hal_HstSendEvent(USB_EVENT, 0x20151302);
					hal_generateDescDevice(g_HalUsbVar.Desc, g_HalUsbBufferEp0In);
					size = sizeof(HAL_USB_DEVICE_DESCRIPTOR_REAL_T);
					break;
				case 2: /* Config */
					size = hal_generateDescConfig(
					g_HalUsbVar.Desc->configList[(setup->value&0xFF)],
					g_HalUsbBufferEp0In, setup->value&0xFF);
					hal_HstSendEvent(USB_EVENT, 0x11130006);
					break;
				case 3: /* String */
					if((setup->value&0xFF) == 0)
					{
						// Set caracter table to 0x0409
						size                   = 0x04;
						g_HalUsbBufferEp0In[0] = 0x04;
						g_HalUsbBufferEp0In[1] = 0x03;
						g_HalUsbBufferEp0In[2] = 0x09;
						g_HalUsbBufferEp0In[3] = 0x04;
					}
					else
					{
						// Select the string and send it
						if((setup->value&0xFF) <= g_HalUsbVar.NbString)
						{
							//hal_HstSendEvent(USB_EVENT,0xfcfc1337);
							size = strlen((INT8 *)g_HalUsbVar.String[(setup->value&0xFF)-1])*2 + 2;
							g_HalUsbBufferEp0In[0] = size;
							g_HalUsbBufferEp0In[1] = 0x03;
							hal_usbAsciiToUtf8(&g_HalUsbBufferEp0In[2],g_HalUsbVar.String[(setup->value&0xFF)-1]);
						}
					}
					hal_HstSendEvent(USB_EVENT, 0x14070810);
					break;
				}

                setup_completed = 1;
                if(setup->lenght < size)
                {
                    size = setup->lenght;
                }

                hal_HstSendEvent(USB_EVENT, 0x20151303);
                hal_UsbSend(0, g_HalUsbBufferEp0In, size, 0);
                break;
#if 0

                case HAL_USB_REQUEST_DEVICE_TEST_MODE:
                static const UINT8 _usb_test_packet[53] =
                {
                    /** - Implicit SYNC({KJ * 15}, KK) and DATA0 PID(KKJKJKKK) */

                    /* JKJKJKJK x9 */
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    /** - JJKKJJKK x8 */
                    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                    /** - JJJJKKKK x8 */
                    0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
                    /** - JJJJJJJKKKKKKK x8 */
                    0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF,
                    /** - JJJJJJJK x8 */
                    0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD,
                    /* JKKKKKKK x10, JK */
                    0xFC, 0x7E, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0x7E

                    /** - Implicit CRC16(JJJKKKJJKKKKJKKK) then EOP(JJJJJJJJ) */
                };

                UINT8 usbhw_test_mode= usb_test_mode2usbhw_test_mode(setup->request);
			
            if(usbhw_test_mode=MUSB_TEST_PACKET)
           {
            /*load test_packet into fifo */

            //musb_write_fifo_ram(0, _usb_test_packet, sizeof(_usb_test_packet));
           
			}

#endif
            }
			break;
		case HAL_USB_REQUEST_DESTINATION_INTERFACE:
			switch(hal_UsbCallbackInterface(setup->index&0xFF,HAL_USB_CALLBACK_TYPE_CMD, setup))
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
		case HAL_USB_REQUEST_DESTINATION_EP:
			switch(setup->request)
			{
			case HAL_USB_REQUEST_EP_GET_STATUS   :
				break;
			case HAL_USB_REQUEST_EP_CLEAR_FEATURE:
				if(setup->value == 0 || setup->value & 0x01)
				{
				//the implementation of hal_UsbEpStall has bad effect on CLEAR_FEATURE command
				//remove this function will not effect current umss and uvideos
				//TODO:implement hal_UsbEpStall in right way
				//hal_UsbEpStall(setup->index&0xFF, FALSE);
				}
				setup_completed = 1;
				break;
			case HAL_USB_REQUEST_EP_SET_FEATURE  :
				break;
			default:
				break;
			}
			
			switch(hal_UsbCallbackEp(setup->index&0xFF, HAL_USB_CALLBACK_TYPE_CMD, setup))
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
		
        if(setup->lenght == 0)
        {
            //
            ///musb_writew(MUSB_CSR0, MUSB_CSR0_P_DATAEND);
            hal_UsbStatusIn();
        }
        else
        {
            if(setup_completed != 1)
            {
                if(setup->requestDesc.requestDirection)
                {
                    hal_UsbSend(0, g_HalUsbBufferEp0In, 0, 0);
                }
            }
        }
		break;
	case EP0_STATE_IN    :
        setup = &g_HalUsbVar.lastSetup;
        if(hal_UsbContinueTransferDone(0x80, 0))
        {
			switch(g_HalUsbVar.RequestDesc.requestDest)
			{
			case HAL_USB_REQUEST_DESTINATION_DEVICE:
				break;
			case HAL_USB_REQUEST_DESTINATION_INTERFACE:
				hal_UsbCallbackInterface(g_HalUsbVar.Ep0Index&0xFF, HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
				break;
			case HAL_USB_REQUEST_DESTINATION_EP:
				hal_UsbCallbackEp(g_HalUsbVar.Ep0Index&0xFF,
				HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
				break;
			}
			hal_UsbStatusOut();
		}
		break;
	case EP0_STATE_OUT:
        setup = &g_HalUsbVar.lastSetup;
        size = musb_readw( MUSB_COUNT0);
        if(hal_UsbContinueTransferDone(0x00,size))
        {
            switch(g_HalUsbVar.RequestDesc.requestDest)
            {
            case HAL_USB_REQUEST_DESTINATION_DEVICE:
                break;
            case HAL_USB_REQUEST_DESTINATION_INTERFACE:
                hal_UsbCallbackInterface(g_HalUsbVar.Ep0Index&0xFF, HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
                break;
            case HAL_USB_REQUEST_DESTINATION_EP:
                hal_UsbCallbackEp(g_HalUsbVar.Ep0Index&0xFF, HAL_USB_CALLBACK_TYPE_DATA_CMD, setup);
                break;
            }
            hal_UsbStatusIn();
        }
        break;
	case EP0_STATE_STATUS_IN:
		hal_HstSendEvent(USB_EVENT, 0x14082601);
		if(musb_readb(MUSB_FADDR) == 0 && g_HalUsbVar.usbAddr != 0xff)
		{
			musb_writeb( MUSB_FADDR, g_HalUsbVar.usbAddr & 0x7f);
		}
//		g_HalUsbVar.Ep0State = EP0_STATE_IDLE;
//		break;
	case EP0_STATE_STATUS_OUT:
		hal_HstSendEvent(USB_EVENT, 0x14082602);
		g_HalUsbVar.Ep0State = EP0_STATE_IDLE;
        csr0 = musb_readw( MUSB_CSR0);
        if(csr0 != 0)
        {
            hal_HstSendEvent(USB_EVENT, 0x17000302);
            goto LOOP;
        }
        break;
    }

	//hal_SysExitCriticalSection(csStatus);
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

void musb_g_resume()
{
	// if () callback.... 
	{
		//notify device driver
	}
}
void musb_notify_conn_msg(UINT8 msg)
{
	COS_EVENT ev;
	
	if(msg == MUSB_INTR_CONNECT)
		{
		ev.nEventId = EV_FS_USB_PLUG_DETECT;
		}
	else
		{
		ev.nEventId = EV_FS_USB_PLUG_OUT;
        }
	COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


static void musb_stage0_irq(UINT8 int_usb, UINT8 devctl)
{
	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */
	static int workAsHost = 0; 
	if (int_usb & MUSB_INTR_RESUME) 
	{
		if (devctl & MUSB_DEVCTL_HM) 
		{
			UINT8 power;

			/* remote wakeup?  later, GetPortStatus
			 * will stop RESUME signaling
			 */
			power = musb_readb( MUSB_POWER);
			if (power & MUSB_POWER_SUSPENDM) 
			{
				/* spurious */
				int_usb &= ~MUSB_INTR_SUSPEND;
			}
			else
			{	
				power &= ~MUSB_POWER_SUSPENDM;
				musb_writeb(MUSB_POWER, power | MUSB_POWER_RESUME);
			}
		} 
		else
		{
			if ((devctl & MUSB_DEVCTL_VBUS) != (3 << MUSB_DEVCTL_VBUS_SHIFT) ) 
			{
				int_usb |= MUSB_INTR_DISCONNECT;
				int_usb &= ~MUSB_INTR_SUSPEND;
			}
			else
			{
				musb_g_resume();
			}
		}
	}

	if (int_usb & MUSB_INTR_VBUSERROR) 
	//if (int_usb & MUSB_INTR_SESSREQ) 
	{
		int	ignore = 0;

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
		if (devctl & MUSB_DEVCTL_HM)
		{
			/* recovery is dicey once we've gotten past the
			 * initial stages of enumeration, but if VBUS
			 * stayed ok at the other end of the link, and
			 * another reset is due (at least for high speed,
			 * to redo the chirp etc), it might work OK...
			 */

			if (Varvbuserr_retry) 
			{
              
				Varvbuserr_retry--;
				ignore = 1;
				devctl |= MUSB_DEVCTL_SESSION;
				musb_writeb(MUSB_DEVCTL, devctl);
			} 
			else 
			{
				//g_HalUsbVar->port1_status |= USB_PORT_STAT_OVERCURRENT | (USB_PORT_STAT_C_OVERCURRENT << 16);
			}

		}

		/* go through A_WAIT_VFALL then start a new session */
		//if (!ignore)
			//musb_platform_set_vbus(musb, 0);
	}

	if (int_usb & MUSB_INTR_SUSPEND) 
      {

        hal_HstSendEvent(USB_EVENT, 0x00004574); 
		if ((devctl & MUSB_DEVCTL_HM) == 0) 
		{
			//musb_g_suspend(musb);
		}
		else
		{
			
		}
		
	  }
	
	if (int_usb & MUSB_INTR_CONNECT) 
	{

		/* high vs full speed is just a guess until after reset */
		//if (devctl & MUSB_DEVCTL_LSDEV)
			//g_HalUsbVar->port1_status |= USB_PORT_STAT_LOW_SPEED;

		/* indicate new connection to OTG machine */
		hal_HstSendEvent(USB_EVENT, 0x00004572);
		if ((devctl & MUSB_DEVCTL_HM) == 0) 
		{
			if (int_usb & MUSB_INTR_SUSPEND) 
			{
				int_usb &= ~MUSB_INTR_SUSPEND;
			}
		}
		else
		{
			//notify HCD to reset the
			g_USB_DEVICE=1;
			workAsHost = 1;
			hal_musb_host_connect();
			musb_notify_conn_msg(MUSB_INTR_CONNECT);
		}
	}
	
	if (int_usb & MUSB_INTR_DISCONNECT)
    {
        if(workAsHost)
        {
            /*hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;
            //hwp_sysCtrl->usb_cfg    &=~SYS_CTRL_USB_PLL_PU;// usb_pll_pu
            //hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_USBPHY;//disbale clk_usbphy
            //hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;
            //COS_KillTimer(g_sys_timer_id_usb_detect);*/
            hal_HstSendEvent(USB_EVENT, 0x00004573);
            musb_writeb(MUSB_INTRUSBE,0);
            hal_musb_host_disconnect();
            musb_writeb(MUSB_FADDR,0);
            Mcd_UsbHostDeInit(0);
            hal_musb_start();
            usb_music_udiskready=0;
            usb_poweron_det=0;
            count_MUSB_RXCOUNT=0;
            musb_notify_conn_msg(MUSB_INTR_DISCONNECT);
        }
        else
        {
        //musb_g_disconnect(musb);
        }
        workAsHost = 0;
        }

    /* mentor saves a bit: bus reset and babble share the same irq.
      * only host sees babble; only peripheral sees bus reset.
    	 */
	if (int_usb & MUSB_INTR_RESET) 
	{

	    if(usb_poweron_det>=1)
	    {
    	    hal_musb_host_disconnect();
            Mcd_UsbHostDeInit(0);
            hal_musb_start();
            return;
	    }
	   
		hal_HstSendEvent(USB_EVENT, 0x2015111c);
		if ((devctl & MUSB_DEVCTL_HM) != 0)
	     {             


		    //mon_Event(0x2015111g);
			/*
			 * Looks like non-HS BABBLE can be ignored, but
			 * HS BABBLE is an error condition. For HS the solution
			 * is to avoid babble in the first place and fix what
			 * caused BABBLE. When HS BABBLE happens we can only
			 * stop the session.
			 */
			if (devctl & (MUSB_DEVCTL_FSDEV | MUSB_DEVCTL_LSDEV))
			{
				;
			}
			else 
			{
				musb_writeb( MUSB_DEVCTL, 0);
			}
		} 
		else 
		{
			static int count = 0;
			count++;
			if( count == 1)
			{
				
				hal_HstSendEvent(USB_EVENT, 0x20151201);
				          #if APP_SUPPORT_USBAUDIO==1 
                          musb_writeb( MUSB_DEVCTL, 0);
                          hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;
                          hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_USBC;
                          hwp_sysCtrl->usb_cfg    &=~SYS_CTRL_USB_PLL_PU;
                          hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_LOCK;
                           USB_Entry(0);
                          #else
						  musb_notify_conn_msg(MUSB_INTR_CONNECT);
						  #endif
				          g_USB_DEVICE=0;
			} 
           
			if(g_HalUsbVar.DeviceCallback != 0)
			{
				hal_HstSendEvent(USB_EVENT, 0x20151202);
				g_HalUsbVar.Desc = g_HalUsbVar.DeviceCallback(TRUE);
			}

		    // Reset EP0 state
			g_HalUsbVar.Ep0State        = EP0_STATE_IDLE;
			g_HalUsbVar.NbString        = 0;
			g_HalUsbVar.usbAddr = 0xff;

		}
	}

	return ;
}

PUBLIC UINT16 hal_usbSpiRead(UINT32 regIdx)
{
    UINT32 wrData, rdData;
    UINT32 count;
    UINT16 value;
    wrData = (1<<25) | ((regIdx & 0x1ff) << 16) | 0;
    while(hal_IspiTxFifoAvail(2) < 1 ||
            hal_IspiSendData(2, wrData, TRUE) == 0);
    while(!hal_IspiTxFinished(2));
    count = hal_IspiGetData(2, &rdData);
    value = (UINT16)(rdData & 0xffff);
    return value;
}

VOID BAL_SysUSBDeviceDetect()
{
#if APP_SUPPORT_USB == 1
    if(g_USB_DEVICE==1)
    {
        g_USB_DEVICE=0;
        hal_HstSendEvent(USB_EVENT, 0x00004567);
        COS_KillTimer(g_sys_timer_id_usb_detect);
    } 
    else
    {
        hal_HstSendEvent(USB_EVENT, 0x00004568);
        usb_poweron_det=0;
        hal_musb_host_disconnect();
        Mcd_UsbHostDeInit(0);
        hal_musb_start();
    }
#endif
}


PROTECTED VOID hal_UsbRoleDet(UINT8 interruptId)
{
  UINT8		devctl;
 /* 
   After USB detection interrupt (No. 29) rises up,
   SW checks USB_ROLE_DET module register bits timeout_int,
   and det_int to see the detection result, and device_mode,
   host_mode to see device/host mode if det_int set. If
   det_int set, SW switches DP/DM to normal mode. If a timeout
   happens,SW should re-start USB_ROLE_DET module by first 
   setting P00 or P01 to a function except 7,and then setting 
   P00 and P01 back to function 7 again.*/
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
	hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;
    hal_HstSendEvent(USB_EVENT, 0x00004569);
    hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_USBPHY;//disbale clk_usbphy
    // USB need 48M clk from pll so we request clk >52M to open pll
    hal_SysRequestFreq(HAL_SYS_FREQ_USB, HAL_SYS_FREQ_78M, 0);
    hal_TimDelay(1 MS_WAITING);
    hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_USBPHY;//enable clk_usbphy
    hal_TimDelay(1 MS_WAITING);
#if 1
	while(!(hal_usbSpiRead(0x84)==(0xe000))) //usb phy pll lock bit
	{
		hwp_sysCtrl->usb_cfg    &= ~SYS_CTRL_USB_PLL_PU;
		hal_TimDelay(1);
		hal_HstSendEvent(0xff, 0x00009999);
		hwp_sysCtrl->usb_cfg |= SYS_CTRL_USB_PLL_PU;
		hal_TimDelay(1);
	}
#endif
    hwp_usb_role_det->INT_CLR|=USB_ROLE_DET_INT_CLEAR;

//Disbale usb pins when AT in uart in 5856							/*add for hci target*/
#if !defined(__AT_SUPPORT__) || defined(__AT_IN_DEBUG_HOST__) || !defined (UART2_SUPPORT)
#ifndef CODEC_SUPPORT
	//GPIO cfg FUNCTION 6 iomux p00 
	hal_GpioSetPinMux(HAL_GPIO_0_0, HAL_GPIO_ALTFUNC_CFG_6);

	//GPIO cfg FUNCTION 6 iomax p01 
	hal_GpioSetPinMux(HAL_GPIO_0_1, HAL_GPIO_ALTFUNC_CFG_6);
#endif
#else
	//4 Set GPIO_0_0 and GPIO_0_1 to 2(UART) in TGT_GPIO_PINMUX_CFG
#endif /* __AT_SUPPORT__ && !__AT_IN_DEBUG_HOST__*/

	boot_UsbIspiWrite(0x83, 0x4448);
	boot_UsbIspiWrite(0x89, 0xee8d);
	boot_UsbIspiWrite(0x8d, 0x0029);    
	hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_SET_RST_USBC;
    musb_writeb(MUSB_INTRUSBE, 0xf7);
	musb_writeb(MUSB_TESTMODE, 0x0);//00
#ifndef USB_AUDIO_SUPPORT
	musb_writeb(MUSB_POWER, MUSB_POWER_ISOUPDATE
						    | MUSB_POWER_HSENAB
						/* ENSUSPEND wedges tusb */
						 //| MUSB_POWER_ENSUSPEND
						);
#else
	musb_writeb(MUSB_POWER, MUSB_POWER_ISOUPDATE
						    //| MUSB_POWER_HSENAB
						/* ENSUSPEND wedges tusb */
						 //| MUSB_POWER_ENSUSPEND
						);
#endif
	devctl = musb_readb(MUSB_DEVCTL);
	devctl |= MUSB_DEVCTL_SESSION;
	musb_writeb(MUSB_DEVCTL, devctl);   //81
	devctl = musb_readb(MUSB_DEVCTL);
	hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_LOCK;		
	hal_HstSendEvent(USB_EVENT, 0x00004570);
	g_sys_timer_id_usb_detect = COS_SetTimer(1000, BAL_SysUSBDeviceDetect, NULL, COS_TIMER_MODE_SINGLE);


#endif
}
PROTECTED VOID hal_UsbIrqHandler(UINT8 interruptId)
{
    UINT8		devctl;
    int		    ep_num;
    UINT32		reg;
    UINT8 		int_usb;
    UINT16		int_tx;
    UINT16		int_rx;

#if APP_SUPPORT_USB==1
    devctl = musb_readb( MUSB_DEVCTL);
    //if (devctl & MUSB_DEVCTL_HM)
        hal_TimDelay(3);

    int_usb = musb_readb( MUSB_INTRUSB);
    int_tx = musb_readw( MUSB_INTRTX);
    int_rx = musb_readw( MUSB_INTRRX);
    #if 0
     u32  dmaInt = musb_readl(MUSB_DMA_INT);
   if(dmaInt)
	{
		if (devctl & MUSB_DEVCTL_HM)
		{
			musb_host_dma_int(dmaInt);
		}
		else
		{
		musb_device_dma_int(dmaInt);
		}
	}
   #endif
    int_usb &= ~MUSB_INTR_SOF;
    if ( !( int_usb || int_tx || int_rx) )
    {
    	return ;
    }

      //hal_HstSendEvent(USB_EVENT, int_tx<<16|int_rx);
      /* the core can interrupt us for multiple reasons; docs have
      a generic interrupt flowchart to follow
      */

    if (int_usb)
    {
    	hal_HstSendEvent(USB_EVENT, 0x00004571);
    	hal_HstSendEvent(USB_EVENT, int_usb);
    	musb_stage0_irq(int_usb, devctl);
    }

    /* "stage 1" is handling endpoint irqs */

    /* handle endpoint 0 first */

    if (int_tx & 1)
    {
    	if (devctl & MUSB_DEVCTL_HM)
    		usb_host_end0_Interrupt();
    	else
    		hal_UsbDecodeEp0Packet();
    }

    /* RX on endpoints 1-15 */
    reg = int_rx >> 1;
    ep_num = 1;
    while (reg) 
    {
    	if (reg & 1)
    	{
    		/* musb_ep_select(musb->mregs, ep_num); */
    		/* REVISIT just retval = ep->rx_irq(...) */
    		if (devctl & MUSB_DEVCTL_HM)
    			musb_host_rx_int(ep_num);
    		else
    			musb_g_rx(ep_num);//devicejinru shaohuima
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
    		/* musb_ep_select(musb->mregs, ep_num); */
    		/* REVISIT just retval |= ep->tx_irq(...) */
    		if (devctl & MUSB_DEVCTL_HM)
    			musb_host_tx_int(ep_num);
    		else
    			musb_g_tx(ep_num);//shaohuima
    	}
    	reg >>= 1;
    	ep_num++;
    }
#endif // #if APP_SUPPORT_USB==1
}

PUBLIC VOID hal_UsbOpen(HAL_USB_GETDESCRIPTOR_CALLBACK_T callback)
{
#if APP_SUPPORT_USB==1
    hal_UsbInit();
    g_HalUsbVar.DeviceCallback = callback;
    hal_UsbConfig();
#endif
}

PUBLIC VOID hal_UsbEpStall(UINT8 ep, BOOL stall)
{
	UINT8   epNum;
	//REG32*  diepctl;
	//REG32*  doepctl;

	hal_HstSendEvent(USB_EVENT, 0x1112eeee);

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
// hal_UsbRecv
// -----------------------------------------------------------------------------
/// Recv an usb packet
/// @param ep Define the endpoint index for the direction
/// use #HAL_USB_EP_DIRECTION_IN and use #HAL_USB_EP_DIRECTION_OUT
/// @param buffer Pointer on the data buffer to be send.
/// @param size Number of bytes to be send.
/// @param flag No flag you must set this parameter to zero
/// @return Number of sent bytes or -1 if error
// =============================================================================
PUBLIC INT32 hal_UsbRecv(UINT8 ep, UINT8* buffer, UINT16 size, UINT32 flag)
{
	UINT8   epNum;
    UINT16     csr;
	HAL_USB_MGC_TRANSFER_T *pTransfer;

	//UINT32 csStatus = hal_SysEnterCriticalSection();
	
	epNum = HAL_USB_EP_NUM(ep);

	// recieve data in interrupt, unload fifo. 
	if(epNum == 0)
    {   
        //musb_writew(MUSB_CSR0, MUSB_CSR0_P_SVDRXPKTRDY);
	    pTransfer = &g_HalUsbVar.Transfert[epNum];
    }
    else
    {
        pTransfer = &g_HalUsbVar.Transfert[epNum+DIEP_NUM+1];
    }
	pTransfer->buffer = buffer;
	pTransfer->sizeTransfered = 0;
	pTransfer->sizeRemaining = size;
	pTransfer->transferDone = FALSE;

    if(epNum != 0)
	{
	    musb_select_ep(epNum);

    	csr = musb_readw(MUSB_RXCSR);
    	csr &=~(MUSB_RXCSR_RXPKTRDY);// Clear RXPKTRDY
    	musb_writew(MUSB_RXCSR, csr);

    	csr = musb_readw(MUSB_RXCSR);
    }

    //hal_SysExitCriticalSection(csStatus);
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
PUBLIC INT32 hal_UsbSend(UINT8 ep, UINT8* buffer, UINT16 size, UINT32 flag)
{
	UINT8   epNum;
	UINT16	txcsr = 0;
	UINT16	length = 0;	
	
	epNum = HAL_USB_EP_NUM(ep);
	musb_select_ep(epNum);
	
	if(epNum == 0)
	{
		UINT16 csr0; 
        HAL_USB_MGC_TRANSFER_T *pTransfer = &g_HalUsbVar.Transfert[0];
		pTransfer->sizeRemaining = size;
		pTransfer->sizeTransfered = 0;
		pTransfer->buffer = buffer;
		pTransfer->transferDone = FALSE;

		//hal_HstSendEvent(USB_EVENT, 0x14082710);
		if( pTransfer->sizeRemaining > 0x40 )
		{
			length = 0x40;
		}
        else
		{
			length = pTransfer->sizeRemaining;
		}

	    //hal_HstSendEvent(SYS_EVENT, 0x00002015);
        musb_write_fifo_ram(0, pTransfer->buffer+pTransfer->sizeTransfered, length);
		csr0 = musb_readw( MUSB_CSR0);
		if(length < 0x40)
		{
		    csr0 |= MUSB_CSR0_TXPKTRDY|MUSB_CSR0_P_DATAEND;
            hal_UsbStatusOut();
        }
		else
		    csr0 |= MUSB_CSR0_TXPKTRDY;
		musb_writew(MUSB_CSR0, csr0);

		return size;	

	}
	else
	{
		HAL_USB_MGC_TRANSFER_T *pTransfer = &g_HalUsbVar.Transfert[epNum];
		pTransfer->sizeRemaining = size;
		pTransfer->sizeTransfered = 0;
		pTransfer->buffer = buffer;
		pTransfer->transferDone = FALSE;

		//hal_HstSendEvent(USB_EVENT, 0x14082201);
		//hal_HstSendEvent(USB_EVENT, size);

		length = musb_readw( MUSB_TXMAXP);
		if( size < length )
		{
			length = size;
		}

		txcsr = musb_readw( MUSB_TXCSR);
		txcsr |= MUSB_TXCSR_MODE;
		musb_writew( MUSB_TXCSR, txcsr);
		musb_write_fifo_ram(epNum, buffer, length);

		txcsr = musb_readw( MUSB_TXCSR);
		txcsr |= MUSB_TXCSR_TXPKTRDY;	
		//hal_HstSendEvent(USB_EVENT, csr);		
		musb_writew( MUSB_TXCSR, txcsr);
	}
	return length;
}

void musb_g_rx(UINT8 epnum)
{
    UINT16			csr;
    UINT16			rxcount;
    UINT16         rxMaxP;
    HAL_USB_MGC_TRANSFER_T *pTransfer;
    musb_select_ep(epnum);
    csr = musb_readw(MUSB_RXCSR);

	//hal_HstSendEvent(USB_EVENT, 0x1408201a);
	//hal_HstSendEvent(USB_EVENT, csr);
    
	if (csr & MUSB_RXCSR_P_SENTSTALL) 
	{
		csr |= MUSB_RXCSR_P_WZC_BITS;
		csr &= ~MUSB_RXCSR_P_SENTSTALL;
		musb_writew(MUSB_RXCSR, csr);
		return;
	}

	if (csr & MUSB_RXCSR_P_OVERRUN) 
	{
		/* csr |= MUSB_RXCSR_P_WZC_BITS; */
		csr &= ~MUSB_RXCSR_P_OVERRUN;
		musb_writew(MUSB_RXCSR, csr);
		return ;
	}

	if (csr & MUSB_RXCSR_INCOMPRX)
	{
		/* REVISIT not necessarily an error */
		return ;
	}

	rxcount = musb_readw( MUSB_RXCOUNT);
	//hal_HstSendEvent(USB_EVENT, (csr<<16)|rxcount);
	if(rxcount == 0)
	{
	    //hal_HstSendEvent(USB_EVENT, 0x14082033);
		return ;
	}
	
	//hal_HstSendEvent(USB_EVENT, 0x14082010);	
	//hal_HstSendEvent(USB_EVENT, epnum);	
    
	pTransfer = &g_HalUsbVar.Transfert[epnum+DIEP_NUM+1];
	if(pTransfer->buffer==NULL)
		{
		  //mon_Event(0x2016aaaa);
		  return;
		 }
	musb_read_fifo_ram(epnum, pTransfer->buffer + pTransfer->sizeTransfered,  rxcount);
	//csr &=~(MUSB_RXCSR_RXPKTRDY);
	//musb_writew(MUSB_RXCSR, csr);
	
	pTransfer->sizeTransfered += rxcount;
	pTransfer->sizeRemaining -= rxcount;

    rxMaxP = musb_readw( MUSB_RXMAXP);
#ifdef USB_AUDIO_SUPPORT
    //if( rxcount <= rxMaxP || pTransfer->sizeRemaining == 0)
    {
        //hal_HstSendEvent(USB_EVENT, 0x14082011);
        extern int g_uAudActualPacketSize;
        g_uAudActualPacketSize = rxcount;
        pTransfer->transferDone = TRUE ;
        //pTransfer->sizeTransfered = 0;
        hal_UsbCallbackEp( HAL_USB_EP_DIRECTION_OUT(epnum), HAL_USB_CALLBACK_TYPE_RECEIVE_END, 0);
        //pTransfer->buffer=NULL;
    }
#else
    if( rxcount < rxMaxP || pTransfer->sizeRemaining == 0)
    {
        //hal_HstSendEvent(USB_EVENT, 0x14082011);
        //hal_HstSendEvent(USB_EVENT, rxcount);
        //hal_HstSendEvent(USB_EVENT, pTransfer->sizeRemaining);

        pTransfer->transferDone= TRUE;
        hal_UsbCallbackEp( HAL_USB_EP_DIRECTION_OUT(epnum), HAL_USB_CALLBACK_TYPE_RECEIVE_END, 0);
        pTransfer->buffer=NULL;
    }
    else
    {
        csr &=~(MUSB_RXCSR_RXPKTRDY);
    }
#endif
    musb_writew(MUSB_RXCSR, csr);

}

void musb_g_tx(UINT8 epnum)
{
	UINT16		csr;
	UINT16 		txmaxp;
	HAL_USB_MGC_TRANSFER_T *pTransfer = &g_HalUsbVar.Transfert[epnum];
    //mon_Event(0x00000145);
	//hal_HstSendEvent(USB_EVENT, 0x14802640);
	musb_select_ep(epnum);
	
	csr = musb_readw( MUSB_TXCSR);

	/*
	 * REVISIT: for high bandwidth, MUSB_TXCSR_P_INCOMPTX
	 * probably rates reporting as a host error.
	 */
	if (csr & MUSB_TXCSR_P_SENTSTALL) 
	{
		csr |=	MUSB_TXCSR_P_WZC_BITS;
		csr &= ~MUSB_TXCSR_P_SENTSTALL;
		musb_writew(MUSB_TXCSR, csr);
		return;
	}

	if (csr & MUSB_TXCSR_P_UNDERRUN) 
	{
		/* We NAKed, no big deal... little reason to care. */
		csr |=	 MUSB_TXCSR_P_WZC_BITS;
		csr &= ~(MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_TXPKTRDY);
		musb_writew(MUSB_TXCSR, csr);
	}

	if(pTransfer->buffer == 0 || pTransfer->sizeRemaining == 0)
	{
		return ;
	}

	// continue transfer....
	txmaxp = musb_readw( MUSB_TXMAXP);
	if(txmaxp >= pTransfer->sizeRemaining )
	{
	    // mon_Event(0x00000147);
	    //hal_HstSendEvent(USB_EVENT, 0x14802641);
	    pTransfer->sizeRemaining = 0;
		hal_UsbCallbackEp(HAL_USB_EP_DIRECTION_IN(epnum), HAL_USB_CALLBACK_TYPE_TRANSMIT_END, 0);
	}
	else
	{
		UINT16 size = txmaxp;
		pTransfer->sizeRemaining -= txmaxp;
		pTransfer->sizeTransfered += txmaxp;

		if(txmaxp > pTransfer->sizeRemaining)
		{
			size = pTransfer->sizeRemaining;
		}
		musb_write_fifo_ram(epnum, pTransfer->buffer+pTransfer->sizeTransfered, size);
        //mon_Event(0x00000146);
		csr = musb_readw( MUSB_TXCSR);
		csr |= MUSB_TXCSR_TXPKTRDY;	
		musb_writew( MUSB_TXCSR, csr);
	}
	
}
PUBLIC VOID hal_UsbCompletedCommand(VOID)
{
    switch(g_HalUsbVar.Ep0State)
    {
    case EP0_STATE_STATUS_OUT:
        hal_UsbStatusOut();
        break;
    case EP0_STATE_STATUS_IN:
        hal_UsbStatusIn();
        break;
    default:
        break;

    }
}

PUBLIC VOID hal_UsbEpEnableInterrupt(UINT8 ep, BOOL enable)
{
	UINT8   epNum;
	epNum = HAL_USB_EP_NUM(ep);

	if(enable == TRUE)
	{
		if(HAL_USB_IS_EP_DIRECTION_IN(ep))
		{
			g_HalUsbVar.EpInterruptMask    |= (1<<(epNum+0));
		}
		else
		{
			g_HalUsbVar.EpInterruptMask    |= (1<<(epNum+16));
		}
	}
	else
	{
		if(HAL_USB_IS_EP_DIRECTION_IN(ep))
		{
			g_HalUsbVar.EpInterruptMask    &= ~(1<<(epNum+0));
		}
		else
		{
			g_HalUsbVar.EpInterruptMask    &= ~(1<<(epNum+16));
		}
	}
}

PUBLIC UINT16 hal_UsbEpRxTransferedSize(UINT8 ep)
{
    UINT8   epNum;

    epNum = HAL_USB_EP_NUM(ep);

    // When the transfer is done, the size value is
    // inverted. Consequently, when this function
    // is called after a transfer is done, size
    // must be inverted.
    return g_HalUsbVar.Transfert[epNum+DIEP_NUM+1].sizeTransfered;
}



PUBLIC VOID hal_UsbCloseSoftware(VOID)
{
	hal_UsbClrConfig();
	hal_UsbSetMode(0);

	g_HalUsbVar.Ep0State        = EP0_STATE_IDLE;
	g_HalUsbVar.NbString        = 0;

	g_HalUsbVar.DeviceCallback  = 0;
	g_HalUsbVar.Desc            = 0;
	hal_UsbResetTransfert();
}

PUBLIC VOID hal_UsbClose(VOID)
{
	hal_UsbClrConfig();

	g_HalUsbVar.Ep0State        = EP0_STATE_IDLE;
	g_HalUsbVar.NbString        = 0;
	g_HalUsbVar.DeviceCallback  = 0;
	hal_UsbResetTransfert();

	g_HalUsbVar.Desc            = 0;
}


// =============================================================================
// hal_UsbReset
// -----------------------------------------------------------------------------
/// This function is use to reset USB and/or change configuration of usb
/// For reset you must do hal_UsbReset(TRUE); sleep 0.5s; hal_UsbReset(FALSE);
/// @param reset If is true, the function is clear usb and power down usb else
/// the function power up usb
// =============================================================================
PUBLIC VOID hal_UsbReset(BOOL reset)
{
	if(reset == TRUE)
	{
		hal_UsbInit();
		hal_UsbConfig();
		//pmd_EnablePower(PMD_POWER_USB, FALSE);
	}
	else
	{
		//pmd_EnablePower(PMD_POWER_USB, TRUE);
	}
}

void hal_musb_start()
{
    hal_HstSendEvent(USB_EVENT, 0x20160315);
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    // reset USBC shaohuima 
    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_USBC;
    hwp_sysCtrl->usb_cfg    &=~SYS_CTRL_USB_PLL_PU;
    //disable phy, enable gpio det
    boot_UsbIspiWrite(0x83, 0x4448);
    boot_UsbIspiWrite(0x89, 0x2C10);
    boot_UsbIspiWrite(0x8d, 0x0029);

    //Disbale usb pins when AT in uart in 5856
#if !defined(__AT_SUPPORT__) || defined(__AT_IN_DEBUG_HOST__)
    //GPIO cfg FUNCTION 7 iomux p00 
    hal_GpioSetPinMux(HAL_GPIO_0_0, HAL_GPIO_ALTFUNC_CFG_7);

    //GPIO cfg FUNCTION 7 iomax p01 
    hal_GpioSetPinMux(HAL_GPIO_0_1, HAL_GPIO_ALTFUNC_CFG_7);
#else
    //4 Set GPIO_0_0 and GPIO_0_1 to 2(UART) in TGT_GPIO_PINMUX_CFG
#endif /* __AT_SUPPORT__ && !__AT_IN_DEBUG_HOST__*/

    //open clock
    hwp_sysCtrl->Clk_Per_Enable = SYS_CTRL_ENABLE_PER_USB_ROLE_DET ;   

    //det_mask 0xa1a2e000
    hwp_usb_role_det->IRQ_MASK |=USB_ROLE_DET_INT_MASK_Det;
    //timeout_mask 0xa1a2e000              
    hwp_usb_role_det->IRQ_MASK &=~USB_ROLE_DET_INT_MASK_Timeout;
    //loop time = 0         
    hwp_usb_role_det->IRQ_MASK &= ~USB_ROLE_DET_LOOP_TIME_MASK;          
    hwp_usb_role_det->INTERVAL |=USB_ROLE_DET_LOOP_INTERVAL(0);
    hwp_usb_role_det->INTERVAL |=USB_ROLE_DET_DET_INTERVAL(0);
    hwp_usb_role_det->INTERVAL |=USB_ROLE_DET_wait_INTERVAL(0);
    //boot_UsbIspiWrite(0x87, 0x90);
#ifdef SERVICE_BASE_FS_SUPPORT
if(g_usb_connected==1)umss_Close();   
#endif
    hal_HstSendEvent(USB_EVENT, 0x2016022a);
    	
    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_LOCK;
#endif

    hal_SysRequestFreq(HAL_SYS_FREQ_USB, HAL_SYS_FREQ_32K, 0);

}

BOOL hal_UsbIsWorkAsHost()
{
    UINT8 reg;
#if (CHIP_ASIC_ID !=  CHIP_ASIC_ID_5857)
    hwp_sysCtrl->REG_DBG        = SYS_CTRL_PROTECT_UNLOCK;
#endif
    reg = musb_readb( MUSB_DEVCTL);

    if(reg & MUSB_DEVCTL_HM)
        return TRUE;
    else
        return FALSE;
    #if (CHIP_ASIC_ID !=  CHIP_ASIC_ID_5857)
    hwp_sysCtrl->REG_DBG        = SYS_CTRL_PROTECT_LOCK;
    #endif
}

void hal_UsbSetDisconnCb(void (*Cb)(BOOL isHost))
{
	g_pfDisconnCb = Cb;
}

PUBLIC HAL_USB_EP_TYPE_T hal_UsbGetEpAttr(UINT8 ep)
{    
	UINT32 epCtrl = 0;
	UINT8 epNum = HAL_USB_EP_NUM(ep);

	if (epNum == 0)
	{
		return HAL_USB_EP_TYPE_CONTROL;
	} 

	if (HAL_USB_IS_EP_DIRECTION_IN(ep))
	{

	}
	else
	{

	}
	return (HAL_USB_EP_TYPE_T)((epCtrl >> 18)&0x3);
}

PUBLIC void hal_UsbSleepMode(void)
{
    hal_SysRequestFreq(HAL_SYS_FREQ_USB, HAL_SYS_FREQ_32K, 0);
    #if (CHIP_ASIC_ID !=  CHIP_ASIC_ID_5857)
    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;//shaohuima  
    hwp_sysCtrl->Clk_Other_Enable &=~SYS_CTRL_ENABLE_OC_USBPHY;//disenable clk_usbphy
    hwp_sysCtrl->usb_cfg    &=~SYS_CTRL_USB_PLL_PU;// usb_pll_pu
    //hwp_usb_role_det->INT_CLR|=USB_ROLE_DET_INT_CLEAR;
    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_LOCK;//shaohuima
    #endif
}
PUBLIC void hal_UsbWakeUpMode(void)
{
    hal_SysRequestFreq(HAL_SYS_FREQ_USB, HAL_SYS_FREQ_78M, 0);
    #if (CHIP_ASIC_ID !=  CHIP_ASIC_ID_5857)
    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_UNLOCK;//shaohuima
    hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_USBPHY;//enable clk_usbphy
    hwp_sysCtrl->usb_cfg    |= SYS_CTRL_USB_PLL_PU;// usb_pll_pu
  //hwp_usb_role_det->INT_CLR|=USB_ROLE_DET_INT_CLEAR;

    hwp_sysCtrl->REG_DBG   = SYS_CTRL_PROTECT_LOCK;//shaohuima
    #endif
}
PUBLIC void hal_UsbSetMode(HAL_USB_MODE_T Mode)
{
	g_UsbMode = Mode;
}
PUBLIC HAL_USB_MODE_T hal_UsbGetMode(void)
{
	return g_UsbMode;
}
#endif // CHIP_HAS_USB


