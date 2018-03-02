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

#include <cswtype.h>
#include <errorcode.h>
#include <ts.h>
#include <cos.h>
#include "drv_usb.h"
#include "chip_id.h"
#include "event.h"
#include "ts.h"
#include "sxr_tls.h"
#include "hal_debug.h"
#include "sxr_tim.h"
#include "chip_id.h"
#include "csw_mem_prv.h"
#include "pmd_m.h"
#include "mcd_m.h"
#include "mcd_usb.h"
#include "scsi2.h"
#include "hal_host.h"

#define MASS_INTERFACE_CLASS		0x08	// storage class
#define MASS_INTERFACE_SUBCLASS		0x06	// SCSI command set
#define USB_BLOCK_CACHE				0x200
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856||CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
UINT8 usb_music_udiskready=0;
extern UINT32 usb_poweron_det;
#endif
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
extern UINT32 play_Stop_Key_Status;
#endif
typedef struct tagUsbDiskInst
{
	uint8 			udiskReady;
	PSCSI_DEVICE	pScsiDevice;
}USBDISKINST;

static volatile USBDISKINST  gUsbDiskInst;

INT32 DRV_ReadUSBDisk(HANDLE dev_handle, UINT64 blk_nr, BYTE* pBuffer);
INT32 DRV_WriteUSBDisk (HANDLE dev_handle, UINT64 blk_nr, CONST BYTE* pBuffer);

void  UsbDiskAttach(void *confiuration, uint8 index)
{
	
	PSCSI_DEVICE  pScsiDevice = ScsiUsbDiskAttach(confiuration, index);

	if( pScsiDevice == NULL)
	{
		return ;
	}

	gUsbDiskInst.pScsiDevice = pScsiDevice;
	gUsbDiskInst.udiskReady = 1;
	hal_HstSendEvent(USB_EVENT,0x09060007);
      //MUSIC_Entry(0);
	//g_USB_DEVICE=1;
      //mon_Event(0x09060007);
      //usb_music = 1;
	//UINT8 temp[512];
	//for(; i<1000; i++)
      //DRV_ReadUSBDisk(0, 512,  temp);

	/* notify filesystem, usb disk is ready */
	#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
       usb_music_udiskready=1;
	COS_EVENT ev;
	ev.nEventId = EV_FS_UDISK_READY;
	ev.nParam1 = 0;
	COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	#elif(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
         usb_music_udiskready=1;
       #else
       COS_EVENT ev;
	ev.nEventId = EV_FS_UDISK_READY;
	ev.nParam1 = 0;
	COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
       #endif
	
}


void UsbDiskDetach(void *param)
{
	hal_HstSendEvent(SYS_EVENT,0x10140006);
	gUsbDiskInst.udiskReady = 0;
    #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856||CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
	usb_music_udiskready=0;
    #endif
	hal_HstSendEvent(SYS_EVENT,gUsbDiskInst.udiskReady);
	ScsiUsbDiskDetach(gUsbDiskInst.pScsiDevice);
}
static int USBDisk_ReadWrite(uint8* pBuf, uint64 blk_nr, uint32* pdwBytesTransferred, uint8 bRead)
{
	int dwErr = ERROR_SUCCESS;
	
	PSCSI_DEVICE pDevice = gUsbDiskInst.pScsiDevice; 

	if( gUsbDiskInst.udiskReady == 0)
	{
		return 	ERROR_FAILED;
	}
    // mon_Event(0x09089001);
     
     
	if (!bRead && pDevice->Flags.WriteProtect) 
	{
		dwErr = ERROR_WRITE_PROTECT;
	}
	else 
	{      
		//dwErr = ScsiUnitAttention(pDevice, pDevice->Lun);
		//hal_HstSendEvent(SYS_EVENT,0x09090031);
		//hal_HstSendEvent(SYS_EVENT,dwErr);
		if(ERROR_SUCCESS == dwErr) 
		{
			pDevice->Flags.MediumChanged=FALSE;
			{
				uint32 dwBytesTransferred= ScsiRWSG(pDevice, pBuf, blk_nr, pDevice->Lun, bRead, &dwErr);
				//hal_HstSendEvent(SYS_EVENT,0x09100001);
				//hal_HstSendEvent(SYS_EVENT,pBuf);
				if (pdwBytesTransferred) 
					*pdwBytesTransferred=dwBytesTransferred;
			}
		}
		else if (pDevice->Flags.MediumPresent==FALSE)
		{
			dwErr = ERROR_INVALID_PARAM;
		}
	}
	return dwErr;
}


// returns number of bytes written in the pDiskInfo
static uint32 GetDiskInfo(PSCSI_DEVICE pDevice, DISK_INFO *pDiskInfo,  uint8 Lun)
{
	uint32 dwErr;
	uint32 dwSize = sizeof(DISK_INFO);
	uint32 dwRepeat = 1;

	hal_HstSendEvent(SYS_EVENT,0x09089014);
	dwErr = ScsiUnitAttention(pDevice, Lun);
	if (dwErr == ERROR_GEN_FAILURE) 
	{
		return 0;
	}

	if ( ERROR_SUCCESS == dwErr ) 
	{
		dwRepeat = pDevice->Timeouts.UnitAttnRepeat;

		do 
		{
			dwErr = ScsiReadCapacity(pDevice, pDiskInfo, Lun);
			dwRepeat--;

		} while (ERROR_SUCCESS != dwErr && dwRepeat != 0 );
	}

	if ( ERROR_SUCCESS != dwErr ||0 == pDiskInfo->di_total_sectors ||0 == pDiskInfo->di_bytes_per_sect ) 
	{
		dwSize = 0;
	}

	return dwSize;
}


//-----------------------------------------------------------------------------------------
// DRV_OpenUSBDisk
// 	Open the device.
// @dev_name[in]
//  USBDisk device name.
// @pdev_handle[out]
//   Output device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//    else return error code.
//-----------------------------------------------------------------------------------------	
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
VOID DRV_Mcd_RegisterClientDriver()
{

  USBCLIENTDRIVER	drvInst;
  drvInst.InterfaceClass = MASS_INTERFACE_CLASS;
  drvInst.InterfaceSubClass = MASS_INTERFACE_SUBCLASS;
  drvInst.UsbDeviceAttach = UsbDiskAttach;
  drvInst.UsbDeviceDetach = UsbDiskDetach;
  Mcd_RegisterClientDriver(&drvInst);
  hal_HstSendEvent(USB_EVENT,0x08090009);
}
#endif

INT32 DRV_OpenUSBDisk(UINT8* dev_name,HANDLE* pdev_handle)
{

	USBCLIENTDRIVER	drvInst;
	drvInst.InterfaceClass = MASS_INTERFACE_CLASS;
	drvInst.InterfaceSubClass = MASS_INTERFACE_SUBCLASS;
	drvInst.UsbDeviceAttach = UsbDiskAttach;
	drvInst.UsbDeviceDetach = UsbDiskDetach;

	hal_HstSendEvent(SYS_EVENT,0x08150002);
	
      Mcd_RegisterClientDriver(&drvInst);
	
	return ERR_SUCCESS;
}

//-----------------------------------------------------------------------------------------
// DRV_CloseUSBDisk
// 	Close the device.
// @pdev_handle[out]
//   Device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//    else return error code.
//-----------------------------------------------------------------------------------------	
INT32 DRV_CloseUSBDisk(HANDLE dev_handle)
{    
    return ERR_SUCCESS;    
}


//-----------------------------------------------------------------------------------------
// DRV_FlushUSBDisk
//	 Flush the cache.
// @pdev_handle[out]
//   Device handle.
// Return 
//	 Upon successful completion, return ERR_SUCCESS.
//	 If flush failed, shall return err code.
//-----------------------------------------------------------------------------------------
INT32 DRV_FlushUSBDisk(HANDLE dev_handle)
{
    return ERR_SUCCESS;
}



//-----------------------------------------------------------------------------------------
// DRV_ReadUSBDisk
//	Read a specifically block.
// @dev_handle[in]
// 	Device handle.
// @blk_nr[in]
// 	Block number.it range 0 -  max block number.
// @pBuffer[out] 
// 	Output the  readed content.
// Return 
// 	Upon successful completion, return ERR_SUCCESS.
//	 If read block failed, shall return err code.
//-----------------------------------------------------------------------------------------

INT32 DRV_ReadUSBDisk(HANDLE dev_handle, UINT64 blk_nr, BYTE* pBuffer)
{    
	uint32 nBytesRead = 0;
	INT32 status;

	status = USBDisk_ReadWrite(pBuffer, blk_nr, &nBytesRead, 1);
	//hal_HstSendEvent(SYS_EVENT, 0x55667788);
	//hal_HstSendEvent(SYS_EVENT, status);
	return status;	
	//memcpy(pBuffer, HAL_SYS_GET_UNCACHED_ADDR(pBuffer), USB_BLOCK_CACHE);
}


//-----------------------------------------------------------------------------------------
// DRV_WriteUSBDisk
// 	Write a specifically block.
// @dev_handle[in]
// 	Device handle.
// @blk_nr[in]
//	 Block number.it range 0 -  max block number
// @pBuffer[in] 
// 	Points to the buffer in which the data to be written is stored. 
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//	 If write block failed, shall return err code.
//-----------------------------------------------------------------------------------------
INT32 DRV_WriteUSBDisk (HANDLE dev_handle, UINT64 blk_nr, CONST BYTE* pBuffer)
{
	uint32 nBytesWrite = 0;

	return USBDisk_ReadWrite((UINT8*)pBuffer, blk_nr, &nBytesWrite, 0);
}

//-----------------------------------------------------------------------------------------
// DRV_GetDevInfoUSBDisk
// Get device information.
// @dev_handle[in]
// Device handle.
// @pNrBlock[out]
// output put the max block number.
// @pBlockSize[out] 
// output block size.
// Return 
// Upon successful completion, return ERR_SUCCESS,
// If get device informatioin failed, shall return err code.
//-----------------------------------------------------------------------------------------
INT32 DRV_GetDevInfoUSBDisk (HANDLE dev_handle, UINT64 *pBlockNr, UINT32 *pBlockSize)
{
	uint32 dwSize = sizeof(DISK_INFO);
	uint32 dwErr = ERR_SUCCESS;
	PSCSI_DEVICE pDevice = gUsbDiskInst.pScsiDevice;//((USBDISKINST*)dev_handle)->pScsiDevice;

	hal_HstSendEvent(SYS_EVENT,0x09089002);
	if ( !pBlockNr || !pBlockSize ) 
	{
	    dwErr = ERROR_INVALID_PARAM;
	} 
	else
	{
		// return cached disk geometry, if possible, otherwise,
		// query device
		if (pDevice->Flags.MediumPresent &&
		0 != pDevice->DiskInfo.di_total_sectors &&
		0 != pDevice->DiskInfo.di_bytes_per_sect)
		{
			*pBlockNr = pDevice->DiskInfo.di_total_sectors;
			*pBlockSize = pDevice->DiskInfo.di_bytes_per_sect;
		}
		else
		{
			uint32 dwBytesTransferred;
			DISK_INFO diskInfo;
			
			dwBytesTransferred = GetDiskInfo(pDevice, &diskInfo, pDevice->Lun);
			if (dwSize == dwBytesTransferred) 
			{
				// the device returned geometry data, thus, a medium
				// is present
				pDevice->Flags.FSDMounted = TRUE;
				pDevice->Flags.MediumChanged = FALSE;
				*pBlockNr = diskInfo.di_total_sectors;
				*pBlockSize = diskInfo.di_bytes_per_sect;
			}
			else
			{
				if (pDevice->Flags.MediumPresent == FALSE) 
				{
					//DEBUGMSG(ZONE_ERR, (TEXT("Usbdisk6!DSK_IOControl> IOCTL_GET_INFO; media not present\r\n")));
					*pBlockNr = 0;
					*pBlockSize = 0;
					dwErr = ERROR_FAILED;
				}
			}
			
		}
	}
    	return dwErr;
}

    
//-----------------------------------------------------------------------------------------
//DRV_DeactiveUSBDisk
//   Deactive the device.
// @dev_handle[in]
// 	Device handle.
//Return
//	VOID
//-----------------------------------------------------------------------------------------

VOID DRV_DeactiveUSBDisk(HANDLE dev_handle)
{      
    hal_HstSendEvent(SYS_EVENT,0x09089003); 
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
    if(play_Stop_Key_Status== 1)
    {
    //hal_SysRequestFreq(16, HAL_SYS_FREQ_32K, 0);
    hal_HstSendEvent(0xff, 0x20170627);
    play_Stop_Key_Status=0;
    }
#endif
    return;
}


//-----------------------------------------------------------------------------------------
//DRV_ActiveUSBDisk
// Active the device.
// @dev_handle[in]
// 	Device handle.
// Return 
// See the description abount of drv_OpenDeviceUSBDisk().
//-----------------------------------------------------------------------------------------
extern PUBLIC VOID hal_UsbUpdateDivider(HAL_SYS_FREQ_T freq);
extern void UsbDeviceAttachDriver();

INT32 DRV_ActiveUSBDisk(HANDLE dev_handle)
{      
    hal_HstSendEvent(SYS_EVENT,0x09089004);
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    if(1 == usb_poweron_det)
    {
        UsbDeviceAttachDriver();
    }
    else if(0 == usb_poweron_det)
    {
        return ERROR_FAILED;
    }
 #elif(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
    {
     //hal_usbinitclockenable();
    return ERR_SUCCESS; 
    }
 #else
 return ERR_SUCCESS;
 #endif

 return ERR_SUCCESS;    
}


//-----------------------------------------------------------------------------------------
//DRV_SetStatusSleepIn
// Set LCD status.
// @dev_handle[in]
// 	Device handle.
// Return 
// VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetStatusSleepInUSBDisk(BOOL dev_status)
{   
    hal_HstSendEvent(SYS_EVENT,0x09089005);
}


//-----------------------------------------------------------------------------------------
//DRV_GetRCacheSizeUSBDisk
//Get the size of rcache buffer.
//@dev_handle[in]
// 	Device handle.
//Return
//   return size of rcache buffer.
//-----------------------------------------------------------------------------------------
UINT32 DRV_GetRCacheSizeUSBDisk (HANDLE dev_handle)
{
    hal_HstSendEvent(SYS_EVENT,0x09089006);
    return 0;
}


//-----------------------------------------------------------------------------------------
//DRV_SetRCacheSizeUSBDisk
//Set the size of rcache buffer.
//@dev_handle[in]
// 	Device handle.
//@cache_size[in]
//   Size of rcache buffer.
//Return Value:
//   VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetRCacheSizeUSBDisk(HANDLE dev_handle,UINT32 cache_size)
{
    hal_HstSendEvent(SYS_EVENT,0x09089007);
}


//-----------------------------------------------------------------------------------------
//DRV_GetWCacheSizeUSBDisk
//Get the size of wcache buffer.
//@dev_handle[in]
// 	Device handle.
//Return
//   return size of wcache buffer.
//-----------------------------------------------------------------------------------------
UINT32 DRV_GetWCacheSizeUSBDisk(HANDLE dev_handle)
{
	hal_HstSendEvent(SYS_EVENT,0x09089008);
    return 0;
}


//-----------------------------------------------------------------------------------------
//DRV_SetWCacheSizeUSBDisk
//Set the size of wcache buffer.
//@dev_handle[in]
// 	Device handle.
//@cache_size[in]
//   Size of wcache buffer.
//Return Value:
//   VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetWCacheSizeUSBDisk(HANDLE dev_handle,UINT32 cache_size)
{
	hal_HstSendEvent(SYS_EVENT,0x09089009);
}

/*
extern VOID pm_USBDiskDetect(BOOL cardPlugged);

VOID USBPlugHandler(BOOL cardPlugged)
{
#ifdef _T_UPGRADE_PROGRAMMER
      cardPlugged = cardPlugged;
#else // _T_UPGRADE_PROGRAMMER
#ifndef _MS_VC_VER_
    pm_USBDiskDetect(cardPlugged);
#endif
#endif // _T_UPGRADE_PROGRAMMER
}

void McdSetUSBDetectHandler(void)
{
    mcd_SetUSBDiskDetectHandler((MCD_CARD_DETECT_HANDLER_T)USBPlugHandler );
}
*/

