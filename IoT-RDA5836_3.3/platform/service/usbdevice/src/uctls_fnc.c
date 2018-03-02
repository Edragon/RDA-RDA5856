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
////////////////////////////////////////////////////////////////////////////////
//
/// @file uctls_fnc.c
///
/// USB Controler service
//
////////////////////////////////////////////////////////////////////////////////


#include "csw.h"
#include "uctls_m.h"

#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxr_tim.h"
#include "hal_host.h"
//#include "tgt_uctls_cfg.h"

//typedef UCTLS_BODY_MSG_T MsgBody_t;
//#include "itf_msg.h"

#include "string.h"
#include "uctlsp_debug.h"
#include "event.h"
//#include "tgt_uctls_cfg.h"
// FIXME TODO Add comments.
#if OS_KERNEL == FREERTOS
typedef UCTLS_SERVICE_LIST_T UCTLS_CONFIG_T; //shaohuima

// =============================================================================
// tgt_GetUctlsConfig
// -----------------------------------------------------------------------------
/// This function is used by UCTL to get access to its configuration structure.
/// This is the only way UCTLS can get this information.
// =============================================================================
PUBLIC CONST UCTLS_CONFIG_T* tgt_GetUctlsConfig(VOID);// shaohuima 
#endif
// =============================================================================
// UCTLS_INTERNAL_STATUS_T
// -----------------------------------------------------------------------------
/// The status of uctls service
// =============================================================================
typedef enum
{
    UCTLS_STATUS_CLOSE = 0,
    UCTLS_STATUS_OPEN,
    UCTLS_STATUS_CHARGER,
    UCTLS_STATUS_CONNECTED
} UCTLS_STATUS_T ;


PRIVATE UCTLS_ID_T                   g_UctlsId              = -1;
PRIVATE HAL_USB_DEVICE_DESCRIPTOR_T* g_UctlsDeviceDesc      =  0;
PRIVATE UCTLS_CHARGER_STATUS_T       g_UctlsChargerStatus = 1;

UCTLS_STATUS_T                       g_UctlsStatus          =UCTLS_STATUS_CLOSE;

//PRIVATE VOID uctls_UsbDeviceNotDetected(VOID*);
extern void hal_musb_start();

PRIVATE HAL_USB_DEVICE_DESCRIPTOR_T* uctls_GetDescriptor(VOID)
{
    COS_EVENT ev;
//    Msg_t*                           msg;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_GetDescriptor");


    if(g_UctlsStatus != UCTLS_STATUS_CONNECTED)
    {
        /*msg              = (Msg_t*) COS_Malloc(sizeof(Msg_t));
        msg->H.Id        = UCTLS_HEADER_ID;
        msg->B           = UCTLS_MSG_USB_DEVICE_CONNECTED;
        sxr_Send(msg, g_UctlsMbx, SXR_SEND_MSG);*/
        ev.nEventId = EV_USB_UTLS_EVENT_ID;
        ev.nParam1 = UCTLS_MSG_USB_DEVICE_CONNECTED;
        COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }

    g_UctlsStatus = UCTLS_STATUS_CONNECTED;

    //sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);

    return(g_UctlsDeviceDesc);
}

PUBLIC VOID uctls_Open(UINT16                   usbVendor,
                       UINT16                   usbProduct,
                       UINT16                   usbVersion,
                       UINT8*                   usbSerialNumber)
{
		
    HAL_USB_CONFIG_DESCRIPTOR_T** usbConfigs;


    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_Open");

    if(g_UctlsStatus == UCTLS_STATUS_CLOSE)
    {
    	
        usbConfigs        = (HAL_USB_CONFIG_DESCRIPTOR_T**)
            COS_Malloc(sizeof(HAL_USB_CONFIG_DESCRIPTOR_T*)*2);
		
        // Set config MaxPower 100mA
        usbConfigs[0]     = uctls_NewConfigDescriptor(50, 0, 0, 0);
        usbConfigs[1]     = 0;
		
		
        // Set device Class = Not defined
        
        g_UctlsDeviceDesc =  uctls_NewDeviceDescriptor(0, 0, 0, usbVendor,
                                                       usbProduct, usbVersion,
                                                       usbConfigs,
                                                       usbSerialNumber,
                                                       "RDA");

        usbConfigs[0]->interfaceList = (HAL_USB_INTERFACE_DESCRIPTOR_T**)
        COS_Malloc(sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*) * 16); // max 16 interface
        hal_HstSendEvent(USB_EVENT,0x09040001);
		hal_HstSendEvent(USB_EVENT,(UINT32)usbConfigs[0]->interfaceList);

        //uctls_SetMode(UCTLS_ID_1);
    }

    g_UctlsStatus        = UCTLS_STATUS_OPEN;
    g_UctlsChargerStatus = UCTLS_CHARGER_STATUS_DISCONNECTED;

}
#if 0
PRIVATE VOID uctls_UsbDeviceNotDetected(VOID* data)
{
   /* UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_UsbDeviceNotDetected: No device detected");

    data = data;*/

    //hal_UsbClose();

    /// @todo add link with pmd/pms
}
#endif

PUBLIC VOID uctls_ChargerStatus(UCTLS_CHARGER_STATUS_T status)
{
    COS_EVENT ev;
//    Msg_t*                           msg;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_ChargerStatus %i", status);

    if(status != g_UctlsChargerStatus)
    {
        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_ChargerStatus: Change %i", status);
        switch(status)
        {
        case UCTLS_CHARGER_STATUS_CONNECTED:
        	hal_UsbReset(TRUE);
        	sxr_Sleep(800);
        	hal_UsbReset(FALSE);
        
	      hal_HstSendEvent(SYS_EVENT,0xa1a80001);
            hal_UsbOpen((HAL_USB_GETDESCRIPTOR_CALLBACK_T)uctls_GetDescriptor);
            g_UctlsStatus = UCTLS_STATUS_CHARGER;
#ifdef  MCD_MGC_USB	
            hal_musb_start();
#endif

            //sxr_StartFunctionTimer(10*(1<<14) , uctls_UsbDeviceNotDetected,
                                   //(VOID*)NULL, 0x02);
            break;
        case UCTLS_CHARGER_STATUS_DISCONNECTED:
            hal_HstSendEvent(SYS_EVENT,0xa1a80002);
            hal_UsbClose();
            ev.nEventId = EV_USB_UTLS_EVENT_ID;
            ev.nParam1 = UCTLS_MSG_USB_DEVICE_DISCONNECTED;
            COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            
            g_UctlsStatus = UCTLS_STATUS_OPEN;
            //sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);
            break;
        }

        g_UctlsChargerStatus = status;
    }
}

PUBLIC VOID uctls_Close(VOID)
{
//    Msg_t*                           msg;
    COS_EVENT ev;

   // UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_Close");//shaohuima 2016/07/08

    hal_HstSendEvent(SYS_EVENT,0x09080033);
    hal_HstSendEvent(SYS_EVENT,g_UctlsStatus);

    if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
       g_UctlsStatus == UCTLS_STATUS_CHARGER)
    {
    	hal_HstSendEvent(SYS_EVENT,0x09080003);
        hal_UsbClose();
 //       UINT32 ticks = hal_TimGetUpTime();
//        hal_UsbCloseSoftware();//close the usb software simply
//        hal_HstSendEvent(SYS_EVENT,hal_TimGetUpTime() - ticks);
        if(g_UctlsDeviceDesc)
        {
            uctls_CleanDeviceDescriptor(g_UctlsDeviceDesc);
            g_UctlsDeviceDesc = 0;
        }

         umss_ResetUsbVirtualStatus();
    }
    if(g_UctlsStatus == UCTLS_STATUS_CONNECTED)
    {
        ev.nEventId = EV_USB_UTLS_EVENT_ID;
        ev.nParam1 = UCTLS_MSG_USB_DEVICE_DISCONNECTED;
        COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        
    }

    //sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);
    g_UctlsId  	  = 0;
    g_UctlsStatus = UCTLS_STATUS_CLOSE;
}

PUBLIC VOID uctls_SetMode(UCTLS_ID_T mode)
{
    CONST UCTLS_SERVICE_LIST_T*      modes=NULL;
    UINT8                            nbInterface;
    UINT8                            i;
    UINT8                            j;
	UINT8                            indexInterfaceList;
    HAL_USB_INTERFACE_DESCRIPTOR_T** interfaceList;
    HAL_USB_INTERFACE_DESCRIPTOR_T** interfaceTmpList;
	
    if(g_UctlsId == mode)
    {
        return;
    }

	hal_HstSendEvent(SYS_EVENT,0xa1a80031);
	//shaohuima
     // modes = tgt_GetUctlsConfig();// 20160708
      hal_HstSendEvent(SYS_EVENT,(UINT32)modes);
	//close previous service
	if(g_UctlsId != -1)
	{
	    for(i = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i) {
	        if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
	        {
	            continue;
	        }
	        modes[g_UctlsId].servicesList[i].serviceCallback->close();
	    }
	}

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_SetMode: Change %i", mode);

    g_UctlsDeviceDesc->usbMode = mode;

	hal_HstSendEvent(SYS_EVENT,g_UctlsDeviceDesc->usbMode);
    if(g_UctlsDeviceDesc->configList[0]->interfaceList)
	{         
		uctls_CleanInterfaceListDescriptor(             
			g_UctlsDeviceDesc->configList[0]->interfaceList);        
		g_UctlsDeviceDesc->configList[0]->interfaceList = 0;    
	}

    g_UctlsId                                   = mode;
    g_UctlsDeviceDesc->configList[0]->configIdx = mode + 1;

	
    for(i = 0, nbInterface = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i)
    {
        if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
        {
            continue;
        }
        nbInterface++;
        hal_HstSendEvent(SYS_EVENT,0x09030001);
        hal_HstSendEvent(SYS_EVENT,(UINT32)modes[g_UctlsId].servicesList[i].serviceCallback);
    }

    if(nbInterface == 0)
    {
        if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
           g_UctlsStatus == UCTLS_STATUS_CHARGER)
        {
            //hal_UsbReset(TRUE);
            //sxr_Sleep(8000); // Wait 0.5s
            //hal_UsbReset(FALSE);
        }
        return;
    }
	
	hal_HstSendEvent(SYS_EVENT,nbInterface);
    if(nbInterface != 0)
    {
        interfaceList = (HAL_USB_INTERFACE_DESCRIPTOR_T**)
            COS_Malloc(sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*) * 16);
		hal_HstSendEvent(SYS_EVENT,(UINT32)interfaceList);

        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: interface list %#x\n", interfaceList);

        memset(interfaceList, 0,
               sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*) * 16);

        for(i = 0, indexInterfaceList = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i)
        {
            if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
            {
                continue;
            }
			hal_HstSendEvent(SYS_EVENT,0xa1a80034);
            interfaceTmpList = modes[g_UctlsId].servicesList[i].serviceCallback->open(
                    &modes[g_UctlsId].servicesList[i].serviceCfg);
            if(interfaceTmpList)
            {
                for(j = 0; interfaceTmpList[j]; ++j)
                {
                    interfaceList[indexInterfaceList] = interfaceTmpList[j];
                    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: interface %i %#x\n",
                                indexInterfaceList, interfaceTmpList[j]);
                    ++indexInterfaceList;
                }
                sxr_Free(interfaceTmpList);
            }
        }


        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: end of descriptor generation"
                    " nb interfaces %i\n", indexInterfaceList);

        // Set interfaces list
        g_UctlsDeviceDesc->configList[0]->interfaceList = interfaceList;
		
		//video iad
//		if(g_UctlsId == 2)
//			g_UctlsDeviceDesc->configList[0]->iad = modes[g_UctlsId].servicesList[0].serviceCfg.videos.iad;
//		else
			g_UctlsDeviceDesc->configList[0]->iad = NULL;
    }


    hal_HstSendEvent(SYS_EVENT,0x04170020);
	
    //if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
       //g_UctlsStatus == UCTLS_STATUS_CHARGER)
    {
        //hal_UsbReset(TRUE);
        //sxr_Sleep(8000); // Wait 0.5s
        //hal_UsbReset(FALSE);
    }
    
}

PUBLIC VOID uctls_AddInterface(HAL_USB_INTERFACE_DESCRIPTOR_T *interface)
{
    HAL_USB_INTERFACE_LIST_DESCRIPTOR_T *interfaceList = g_UctlsDeviceDesc->configList[0]->interfaceList;

    while(*interfaceList)
        interfaceList++;
    *interfaceList = interface;
}

PUBLIC UINT8 uctls_AllocateEndPoint(VOID)
{
    INT i;
    INT16 epMask = g_UctlsDeviceDesc->configList[0]->epMask;
    for(i=1; i<16; i++)
    {
        if((epMask&(1<<i))==0)
        {
            g_UctlsDeviceDesc->configList[0]->epMask |= (1<<i);
            return i;
        }
    }
    return i ;
}

