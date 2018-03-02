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
#include "usb.h"
#include "event.h"

#include "sxr_mem.h"

#include "usb_hcd.h"
#include "hal_usb.h"
#include "global_macros.h"
#include "hal_sys.h"
#include "musb_core.h"
#include "hal_host.h"
static USBHCD gHcdInst;
volatile HAL_UNCACHED_BSS int VendorCmdFinish = 0;
volatile HAL_UNCACHED_BSS int BulkTransFinish;
extern void * memset(void *,int,size_t);
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856||CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
extern UINT8 usb_music_udiskready;
#endif
void VendorCmdCallBack(void* pData, uint8 status)
{
      /*UINT32 statuscs=0;
	
	//statuscs =hal_SysEnterCriticalSection();
	
	//hal_HstSendEvent(USB_EVENT,  0x14081244);*/
	VendorCmdFinish = 1;

      // hal_SysExitCriticalSection(statuscs);
	/*
	COS_EVENT ev;
	
	ev.nEventId = EV_FS_USB_INIT;
	ev.nParam1  = (uint32)pData;
	ev.nParam2  = status;
	ev.nParam3  = 0;
	COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	*/
}

void BulkTransferCompleteCallBack(HANDLE handle, uint8 status)
{
	/*UINT32 statuscs=0;
	
	statuscs =hal_SysEnterCriticalSection();

	hal_HstSendEvent(USB_EVENT,  0x14081212);*/
	BulkTransFinish = 1;

	//hal_SysExitCriticalSection(statuscs);
	/*
	COS_EVENT ev;
	
	ev.nEventId = EV_FS_USB_TRANSFERRED;
	ev.nParam1  = handle;
	ev.nParam2  = status;
	ev.nParam3  = 0;
	COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	*/
}

USBOPERATION *USB_HcdInit()
{	
	VendorCmdFinish = 0;
	//BulkTransFinish = 0;
	memset(&gHcdInst, 0, sizeof(USBHCD));
	return &gHcdInst.usbops;
}

int USB_HcdDeinit()
{
	int i = 0;
	//u32 temp;
    #if 0
    if(gHcdInst.ctrlReq != NULL)
    {
        COS_Free(gHcdInst.ctrlReq);
        gHcdInst.ctrlReq = NULL;
    }
    #endif

	if( gHcdInst.pdeviceDesc != NULL )
	{
	    COS_Free(gHcdInst.pdeviceDesc);
	    gHcdInst.pdeviceDesc = NULL;
	}	

	if( gHcdInst.pconfig != NULL)
	{
	    COS_Free(gHcdInst.pconfig);
	    gHcdInst.pconfig = NULL;
	}

	for(; i<3; i++)
	{
		if( gHcdInst.pConfigDesc[i]!= NULL )
		{
		    COS_Free(gHcdInst.pConfigDesc[i]);
		    gHcdInst.pConfigDesc[i] = NULL;
		}
	}
	VendorCmdFinish = 1;
	BulkTransFinish = 1;
	gHcdInst.flag = USB_HCD_DEVICE_DETACHED;
	return 0;
}

int USB_SetAddress(void (*Cb)(void*pData, uint8 status))
{
	USBCONTROLREQUEST  *pReq = &gHcdInst.ctrlReq;
	memset(pReq, 0, sizeof(USBCONTROLREQUEST));

	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
		return USBHCD_DEVICE_DISCONNECTED;
	}
	
	gHcdInst.address ++;
	if( gHcdInst.address == 0)
	{
		gHcdInst.address  = 1;
	}

	pReq->completeCb =  Cb;
	pReq->stdRequest.bRequest = USB_SET_ADDRESS;
	pReq->stdRequest.wValue = gHcdInst.address;
	
	if(gHcdInst.usbops.ControlTransfer)
	{
		return gHcdInst.usbops.ControlTransfer(pReq);
	}
	else
	{
		return  -1;
	}
}

int USB_SetConfiguration(uint8 bValue, void (*Cb)(void*pData, uint8 status))
{
	USBCONTROLREQUEST  *pReq = &gHcdInst.ctrlReq;
	memset(pReq, 0, sizeof(USBCONTROLREQUEST));

	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
		return -1;
	}
		
	pReq->completeCb =  Cb;
	pReq->stdRequest.wValue = bValue;
	pReq->stdRequest.bRequest = USB_SET_CONFIGURATION;

	if( gHcdInst.usbops.ControlTransfer)
	{
		return gHcdInst.usbops.ControlTransfer( pReq );
	}
	else
	{
		return -1;
	}
}

int USB_GetDescriptor(uint8 type, uint16 wValue, uint16 wIndex, void (*Cb)(void*pData, uint8 status))
{
	uint8 desType = wValue>>8;
	uint8 desIndex = wValue;
	USBCONTROLREQUEST  *pReq = &gHcdInst.ctrlReq;
	memset(pReq, 0, sizeof(USBCONTROLREQUEST));

	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
		return USBHCD_DEVICE_DISCONNECTED;
	}
		
	pReq->stdRequest.bmRequestType = 0x80;
	pReq->stdRequest.bRequest = USB_GET_DESCRIPTOR;
	pReq->stdRequest.wValue = wValue;

	if( desType == USB_DES_DEVICE )
	{
	     if(gHcdInst.pdeviceDesc==NULL)
            {   
		gHcdInst.pdeviceDesc = (USBDEVICEDESC*)COS_Malloc_NoCache(2*sizeof(USBDEVICEDESC));
            }
		memset(gHcdInst.pdeviceDesc, 0xaa, 2*sizeof(USBDEVICEDESC)-1);
		pReq->nBufLen = pReq->stdRequest.wLength = sizeof(USBDEVICEDESC); 
		pReq->pBuf = (uint8*)gHcdInst.pdeviceDesc;

	}
	else if( desType == USB_DES_CONFIGURATION )
	{
		//the first time we read a specific configuration
		hal_HstSendEvent(USB_EVENT, 0x77770003);
		if( gHcdInst.pconfig == NULL)
		{
			gHcdInst.pconfig = (USBCONFIGURATIONDESC*)COS_Malloc_NoCache(sizeof(USBCONFIGURATIONDESC) + 3);
			memset(gHcdInst.pconfig, 0, sizeof(USBCONFIGURATIONDESC) + 3);
		}

		hal_HstSendEvent(USB_EVENT, (UINT32)gHcdInst.pconfig);
		if(gHcdInst.pconfig->wTotalLength == 0)
		{
			hal_HstSendEvent(SYS_EVENT,0x09060003);
		 	pReq->nBufLen = 	pReq->stdRequest.wLength = sizeof(USBCONFIGURATIONDESC); 
			pReq->pBuf = (uint8*)gHcdInst.pconfig;
		}
		else
		{
			hal_HstSendEvent(SYS_EVENT,0x09060004);
                if(gHcdInst.pConfigDesc[desIndex]==NULL)
                    {
			gHcdInst.pConfigDesc[desIndex]=(USBCONFIGURATIONDESC*)COS_Malloc_NoCache(16+ gHcdInst.pconfig->wTotalLength);
                    }
			memset(gHcdInst.pConfigDesc[desIndex], 0, 16+gHcdInst.pconfig->wTotalLength);
			if( gHcdInst.pConfigDesc[desIndex] == NULL)
			{
				return USBHCD_NO_RESOURCE;
			}
			
			pReq->nBufLen =               pReq->stdRequest.wLength = gHcdInst.pconfig->wTotalLength;
			pReq->pBuf = (uint8*)gHcdInst.pConfigDesc[desIndex];
		}
	}
	else 
	{
		return USBHCD_INVALID_PARAM;
	}
	
	pReq->stdRequest.wIndex = wIndex;
	pReq->completeCb =  Cb;
	if(gHcdInst.usbops.ControlTransfer)
	{
		return  gHcdInst.usbops.ControlTransfer(pReq);
	}
	else
	{
		return USBHCD_ERROR_FAILED;
	}
	
}

int USB_SetInterface(uint16 bAlternateSetting, uint16 Interface, void (*Cb)(void*pData, uint8 status))
{
	USBCONTROLREQUEST  *pReq = &gHcdInst.ctrlReq;
	memset(pReq, 0, sizeof(USBCONTROLREQUEST));

	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
		return USBHCD_DEVICE_DISCONNECTED;
	}
		
	pReq->stdRequest.bmRequestType = 0x01;	/*dest: interface */ 
	pReq->stdRequest.bRequest = USB_SET_INTERFACE;
	pReq->stdRequest.wIndex = Interface;
	pReq->stdRequest.wValue = bAlternateSetting;

	pReq->completeCb = Cb;

	if(gHcdInst.usbops.ControlTransfer)
	{
		return  gHcdInst.usbops.ControlTransfer(pReq);
	}
	else
	{
		return USBHCD_ERROR_FAILED;
	}
	
}

USB_PIPE USB_OpenPipe(USBENDPOINTDESC *pEndDes)
{
	if(gHcdInst.usbops.OpenPipe)
	{
		return gHcdInst.usbops.OpenPipe(pEndDes);
	}
	return NULL;
}

int USB_VendorTransfer(USBSTDREQUEST *pStdRequest, void *pBuf, uint32* pnDataTransferred, uint32 timeout)
{

    UINT32 status = 0;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
    UINT32 startTick = 0;
#endif
    USBCONTROLREQUEST  *pReq = &gHcdInst.ctrlReq;
    memset(pReq, 0, sizeof(USBCONTROLREQUEST));
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    UINT32 statuscs=0;
    statuscs =hal_SysEnterCriticalSection();
#endif
	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
        hal_SysExitCriticalSection(statuscs);
#endif
        return USBHCD_DEVICE_DISCONNECTED;
	}
		
	pReq->stdRequest = *pStdRequest;
	pReq->completeCb = VendorCmdCallBack;
	pReq->pBuf = pBuf;
	pReq->nBufLen = pStdRequest->wLength;

    if(gHcdInst.usbops.ControlTransfer)
    {
        if( gHcdInst.usbops.ControlTransfer(pReq) != USB_NO_ERROR )
        {
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
            hal_SysExitCriticalSection(statuscs);
#endif
            return USBHCD_ERROR_FAILED;
        }
    }
    else
    {
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
        hal_SysExitCriticalSection(statuscs);
#endif
        return USBHCD_ERROR_FAILED;
    }
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    hal_SysExitCriticalSection(statuscs);
#endif
            
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
	while(RDAUsb_GetState()!=0) //timeout
	{
        //timeout--;
        COS_Sleep(1);
        if( VendorCmdFinish > 0)
            break;
	}
	VendorCmdFinish = 0;
#elif(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
    startTick = hal_TimGetUpTime();
	while(1) //timeout
	{
        timeout--;
        //sxr_Sleep(1 MS_WAITING);
        if( VendorCmdFinish > 0)
        break;
#if 0
        if(hal_TimGetUpTime() - startTick > timeout)// 1s timeout
        {
            hal_HstSendEvent(USB_EVENT, 0x20170608);
            hal_HstSendEvent(USB_EVENT, timeout);
            return USBHCD_ERROR_FAILED;
        }
#endif
	}
	VendorCmdFinish = 0;
#else
    while(1) //timeout
	{
		timeout--;
		//sxr_Sleep(1 MS_WAITING);
		if( VendorCmdFinish > 0)
			break;
	}
	VendorCmdFinish = 0;
#endif

	return status;
}

int USB_BulkOrInterruptTransfer(USB_PIPE pipe, void* pBuf, uint32 BufLen, uint32* pnBytesTransferred, uint32 timeout)
{
    uint32 status = 0;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
    UINT32 startTick = 0;
#endif
    USBBULKREQUEST *pReq = &gHcdInst.bulkReq;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    UINT32 statuscs=0;
    UINT8 counter_test=0;
    statuscs =hal_SysEnterCriticalSection();
#endif
    memset(pReq, 0, sizeof(USBBULKREQUEST));
    //hal_HstSendEvent(USB_EVENT, 0x14080555);

	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
	{
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
        hal_SysExitCriticalSection(statuscs);
#endif
        return USBHCD_DEVICE_DISCONNECTED;
	}
	
	BulkTransFinish = 0;
	pReq->nBufLen = BufLen;
	pReq->pBuf = pBuf;
	pReq->pipe = pipe;
	pReq->pnByteTransferred = pnBytesTransferred;
	pReq->completeCb = BulkTransferCompleteCallBack;
	pReq->pstatus = &status;
    //hal_HstSendEvent(USB_EVENT, 0x14089999);
	if( gHcdInst.usbops.BulkTransfer )
	{
		if( gHcdInst.usbops.BulkTransfer(pReq) != USB_NO_ERROR )
		{
			hal_HstSendEvent(USB_EVENT, 0x14080801);
                  #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
			hal_SysExitCriticalSection(statuscs);
			#endif
			return USBHCD_ERROR_FAILED;
		}	
	}
	else
	{
		hal_HstSendEvent(USB_EVENT, 0x14080802);
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
        hal_SysExitCriticalSection(statuscs);
#endif
		return USBHCD_ERROR_FAILED;
	}
    //hal_HstSendEvent(USB_EVENT, 0x14089999);
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    hal_SysExitCriticalSection(statuscs);
#endif
        
WAIT_FRAME_PERIOD:
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
     hal_TimDelay((1 MS_WAITING)/5);
	while(RDAUsb_GetState()!=0)
	{
		//timeout --;
		
		if( BulkTransFinish > 0 )   //BulkTransFinish > 0  ||
		{
			break;
		}
        #if 1
        if(usb_music_udiskready==1)
        {
        counter_test++;
        if(counter_test==250)
        {
            hal_musb_host_disconnect();
            Mcd_UsbHostDeInit(0);
            musb_notify_conn_msg(MUSB_INTR_DISCONNECT); 
            break;
        }
        }
        COS_Sleep(1);
        #endif

	}
#elif(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
       startTick = hal_TimGetUpTime();
	while(1)
	{
		//timeout --;
		if( BulkTransFinish > 0 )   //BulkTransFinish > 0  ||
		{
			break;
		}
           if(usb_music_udiskready==1)
            {
            if(hal_TimGetUpTime() - startTick > timeout)// 1s timeout
		{
		    hal_HstSendEvent(USB_EVENT, 0x20170608);
		    hal_HstSendEvent(USB_EVENT, timeout);
                 Mcd_UsbHostDeInit(0);
                 hal_musb_start_synops();
		    return USBHCD_ERROR_FAILED;
		}
            }
	}
  
#else 
     while(1)
	{
		//timeout --;
		if( BulkTransFinish > 0 )   //BulkTransFinish > 0  ||
		{
			break;
		}
	}
   
#endif

	//BulkTransFinish = 0;

	//hal_HstSendEvent(USB_EVENT, 0x14080501);
	if( status == USB_STALL_ERROR)
	{
		//clear feature
		return USB_STALL_ERROR;
	}
	else if(status == USB_RECEIVED_NAK)
	{
		// try to send again;
		goto WAIT_FRAME_PERIOD;
	}
	else if( status == USBHCD_DEVICE_DISCONNECTED )
	{
		hal_HstSendEvent(SYS_EVENT,0x88888888);
		gHcdInst.flag = USB_HCD_DEVICE_DETACHED;
		return USBHCD_DEVICE_DISCONNECTED;
	}

	return USB_NO_ERROR;
	
}

int USB_ResetDefaultEndpoint()
{
	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
		return USBHCD_DEVICE_DISCONNECTED;
	else
		return 0;
}

int USB_ResetEndpoint(void *pipe, uint32 timeout)
{
	if( gHcdInst.flag & USB_HCD_DEVICE_DETACHED)
		return USBHCD_DEVICE_DISCONNECTED;
	else
		return 0;
}




