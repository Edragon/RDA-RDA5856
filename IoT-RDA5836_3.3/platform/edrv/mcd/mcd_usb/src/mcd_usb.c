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
/// @file mcd_sdmmc.c
///
/// Implementation of the MCD external driver for the sdmmc protocol.
///
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "cos.h"
//#include "mcd_config.h"
//#include "tgt_mcd_cfg.h"

#include "hal_gpio.h"
#include "mcd_m.h"
#include "mcdp_debug.h"
//#include "mcdp_sdmmc.h"
#include "usb.h"
#include "hal_usb.h"
#include "mcd_usb.h"

#include "hal_sdmmc.h"
#include "hal_sys.h"

#include "stdlib.h"
#include "stdio.h"
#include "sxr_tls.h"
#include "event.h"
#include "pmd_m.h"
#include "sxs_io.h"
#include "sxr_sbx.h"
#include "usb_hcd.h"
#include "hal_host.h"


#define MAX_USB_CLIENT		2
extern UINT32 usb_poweron_det;
enum 
{
	USB_HOST_SCH_INIT,
	USB_HOST_SCH_RESET,
	USB_HOST_SCH_GET_DEVICE_DES,	
	USB_HOST_SCH_SET_ADDRESS,
	USB_HOST_SCH_GET_CONFIGURATION1,	/*get configuration descriptor itself */
	USB_HOST_SCH_GET_CONFIGURATION2,	/*get a whole configuration info*/
	USB_HOST_SCH_SET_CONFIGURATION,
	USB_HOST_SCH_INIT_END
};


//PRIVATE MCD_STATUS_T      g_mcdUSBStatus = MCD_STATUS_NOTOPEN_PRESENT;
PRIVATE MCD_CARD_DETECT_HANDLER_T g_mcdUSBDiskDetectHandler;

typedef struct tagMCDUSBHOST
{
	USBDEVICEDESC *pDeviceDesc;
	USBCONFIGURATIONDESC *pConfigDes;
	uint32	UsbSchOp;
	uint8	nClientNum;
	int 		nCurrentDev;
	USBCLIENTDRIVER ClientDriver[MAX_USB_CLIENT];
	
}MCDUSBHOST;

MCDUSBHOST g_McdUsbInst;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
void UsbDeviceAttachDriver()
{
   static uint8 index = 0;
   UINT32 statuscs=0;
   usb_poweron_det++;
   statuscs =hal_SysEnterCriticalSection();
  if(g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach&&(usb_poweron_det==2))
    {
    hal_HstSendEvent(USB_EVENT, 0x20131000);
    g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach(g_McdUsbInst.pConfigDes, index);
    hal_HstSendEvent(USB_EVENT, 0x20131001);
    }
   hal_SysExitCriticalSection(statuscs);
}
#endif
//extern volatile int gUsbParam; 
//void *gUsbData = NULL;
//uint8 gUsbStatus = 0;

UINT8 mcd_usbHostInitOpCompleteCb_status = 0;//added for delete nParam2
// this callback fuction runs in ISR
static void mcd_usbHostInitOpCompleteCb(void *pData, uint8 status)
{
	
	COS_EVENT ev;
	
	ev.nEventId = EV_FS_USB_INIT;
	ev.nParam1  = (uint32)pData;
         mcd_usbHostInitOpCompleteCb_status = status;
//	ev.nParam2  = status;//for delete nParam2
//	ev.nParam3  = 0;//for delete nParam3
	COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	
    //gUsbParam  = 1;
    //gUsbData = pData;
    //gUsbStatus = status;
	
}

int RDA_UsbReset(void (*CompleteCb)(void* pData, uint8 status));

extern UINT32 g_USB_DEVICE;
int Mcd_USBHostSchOperation(void *pData, uint8 status)
{
	static uint8 index = 0;
	static uint8 count = 0;
	int 	handled = 1;

    //pData = gUsbData;
    //status = gUsbStatus;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    if(RDAUsb_GetState()==0)
    {  
        // hal_musb_start();
         g_USB_DEVICE=0;
		 return 0;
	}
#endif
	if( status == 0)
	{
		count = 0;
		g_McdUsbInst.UsbSchOp++;
	}
	else
	{
		count ++;
		if( count < 3)
		{
			// let's try again... 
			
			if( g_McdUsbInst.UsbSchOp > USB_HOST_SCH_GET_DEVICE_DES)
			{
				//g_McdUsbInst.UsbSchOp = USB_HOST_SCH_RESET;
				//RDA_UsbReset(mcd_usbHostInitOpCompleteCb);
				return 0;	
			}
			else
			{
				RDA_UsbReset(NULL);	
			}	
		}
		else
		{
			return 0;
		}
	}

	switch( g_McdUsbInst.UsbSchOp)
	{
	case USB_HOST_SCH_RESET:
		hal_HstSendEvent(USB_EVENT,0x09170010);
		RDA_UsbReset(mcd_usbHostInitOpCompleteCb);
		break;
	case USB_HOST_SCH_GET_DEVICE_DES:
		hal_HstSendEvent(USB_EVENT,0x09170011);
		USB_GetDescriptor(USB_GET_DESCRIPTOR, USB_DES_DEVICE<<8, 0, mcd_usbHostInitOpCompleteCb);
		break;
	case USB_HOST_SCH_SET_ADDRESS:
		hal_HstSendEvent(USB_EVENT,0x09170012);
		if( pData != NULL)
		{
			g_McdUsbInst.pDeviceDesc = (USBDEVICEDESC*)pData;
			index = 0;
		}
		USB_SetAddress(mcd_usbHostInitOpCompleteCb);
		break;
	case USB_HOST_SCH_GET_CONFIGURATION1:
	case USB_HOST_SCH_GET_CONFIGURATION2:
		hal_HstSendEvent(USB_EVENT,0x08260003);
		USB_GetDescriptor(USB_GET_DESCRIPTOR, USB_DES_CONFIGURATION<<8|index, 0, mcd_usbHostInitOpCompleteCb);
		break;
	case USB_HOST_SCH_SET_CONFIGURATION:
		{
			hal_HstSendEvent(USB_EVENT,0x08260005);
			hal_HstSendEvent(USB_EVENT,(UINT32)pData);
			if( pData != NULL)
			{
				USBCONFIGURATIONDESC * pConfig  = (USBCONFIGURATIONDESC*)pData;
				uint8 * ptemp = (uint8*)pData;
				USBINTERFACEDESC *pInterface = (USBINTERFACEDESC*)(ptemp+sizeof(USBCONFIGURATIONDESC));
				int j = 0;

				index = pConfig->bConfigurationValue;
			      hal_HstSendEvent(USB_EVENT,0x08260006);
				hal_HstSendEvent(USB_EVENT,g_McdUsbInst.nClientNum);
				for(j = 0; j<g_McdUsbInst.nClientNum; j++)
				{
					if( pInterface->bInterfaceClass == g_McdUsbInst.ClientDriver[j].InterfaceClass 
						&& pInterface->bInterfaceSubClass == g_McdUsbInst.ClientDriver[j].InterfaceSubClass)
					{
						g_McdUsbInst.nCurrentDev = j;
						g_McdUsbInst.pConfigDes = pConfig;
						USB_SetConfiguration(index, mcd_usbHostInitOpCompleteCb);
						break;
					}
				}
			}
			else
			{
				/* try again? */
			}
		}
		break;
	case USB_HOST_SCH_INIT_END:
		{  
			#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
		        hal_HstSendEvent(0xff,0x08260007);
		        COS_EVENT ev;
		        usb_poweron_det = 1;
                     ev.nEventId = EV_FS_UDISK_READY;
                     ev.nParam1 = 0;
                     COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
			#elif(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
                        hal_HstSendEvent(USB_EVENT,0x08090003);
			    if(g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach)
        		    {
        			g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach(g_McdUsbInst.pConfigDes, index);
        		    }
			 #else
                       hal_HstSendEvent(USB_EVENT,0x08090003);
			    if(g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach)
        		    {
        			g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceAttach(g_McdUsbInst.pConfigDes, index);
        		    }
			#endif
		}
		break;
	default:
		handled = 0;
		break;
	}
	return handled;
}

int Mcd_USBHostInit()
{
	USBOPERATION *pOps = USB_HcdInit();

	g_McdUsbInst.UsbSchOp = USB_HOST_SCH_INIT;
	g_McdUsbInst.nCurrentDev = -1;
	
	if( pOps != NULL)
	{
		hal_HstSendEvent(USB_EVENT,0x08150004);
		RDA_UsbHostInit(pOps, mcd_usbHostInitOpCompleteCb);
	}
    return 0;
}

int Mcd_UsbHostDeInit(uint8 port)
{
	USB_HcdDeinit();
	
	//since that we only register client driver when power on, don't delete driver info
	if( g_McdUsbInst.nCurrentDev < 0)
	{
		return 0;
	}
	
	if( g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceDetach)
	{
		g_McdUsbInst.ClientDriver[g_McdUsbInst.nCurrentDev].UsbDeviceDetach(NULL);
	}
	g_McdUsbInst.nCurrentDev = -1;
	hal_HstSendEvent(USB_EVENT,0x09260002);
	hal_AbbEnableUsbPhy(FALSE);
	pmd_EnablePower(PMD_POWER_USB, FALSE);
	return 0;
}

int Mcd_RegisterClientDriver(USBCLIENTDRIVER *pdriverInst)
{
	if( g_McdUsbInst.nClientNum >= MAX_USB_CLIENT  || pdriverInst == NULL)
	{
		return MCDUSB_FAILED;
	}

	g_McdUsbInst.ClientDriver[g_McdUsbInst.nClientNum++] = *pdriverInst;

	return MCDUSB_SUCCESS;
}


/// Update g_mcdStatus
/// @return TRUE is card present (only exact when GPIO is used for card detect.)
PRIVATE BOOL mcd_USBDetectUpdateStatus(VOID)
{
   return TRUE;
}

PRIVATE VOID mcd_USBDiskDetectHandler(VOID)
{
    BOOL CardPresent = mcd_USBDetectUpdateStatus();

    g_mcdUSBDiskDetectHandler(CardPresent);
}

// =============================================================================
// mcd_SetCardDetectHandler
// -----------------------------------------------------------------------------
/// Register a handler for card detection
///
/// @param handler function called when insertion/removal is detected.
// =============================================================================
PUBLIC MCD_ERR_T mcd_SetUSBDiskDetectHandler(MCD_CARD_DETECT_HANDLER_T handler)
{
/*    if(NULL == g_mcdConfig)
    {
        g_mcdConfig = tgt_GetMcdConfig();
    }

    if(g_mcdConfig->cardDetectGpio == HAL_GPIO_NONE)
    {
        return MCD_ERR_NO_HOTPLUG;
    }

    if(NULL != handler)*/
    {
        HAL_GPIO_CFG_T cfg  = 
            {
            .direction      = HAL_GPIO_DIRECTION_INPUT,
            .irqMask        = 
                {
                .rising     = TRUE,
                .falling    = TRUE,
                .debounce   = TRUE,
                .level      = FALSE
                },
            .irqHandler     = mcd_USBDiskDetectHandler
            };

    HAL_GPIO_GPIO_ID_T gpio_0;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    gpio_0 = HAL_GPIO_6;
#else
    gpio_0 = HAL_GPIO_0_6;
#endif	

        hal_GpioOpen(gpio_0, &cfg);
        g_mcdUSBDiskDetectHandler = handler;
    }
/*    else
    {
        hal_GpioClose(g_mcdConfig->cardDetectGpio);
        g_mcdCardDetectHandler = NULL;
    }*/
    
    return MCD_ERR_NO;
}


