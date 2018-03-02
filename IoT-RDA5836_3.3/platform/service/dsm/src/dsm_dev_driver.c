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

#ifdef _FS_SIMULATOR_

#else
#include <base_prv.h>
#include <drv_flash.h>
#include <drv_usb.h>

#ifndef _T_UPGRADE_PROGRAMMER
//tianwq #include "umss_m.h"
#endif //_T_UPGRADE_PROGRAMMER
#endif //_MS_VC_VER_


#include "dsm_cf.h"
#include "dsm_dbg.h"
#include "chip_id.h"
#include "vds_api.h"
#include "hal_mem_map.h"
#include "hal_host.h"
#include "hal_voc.h"
#include "hal_bt.h"
#include "hal_spi_flash.h"

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
#include "vds_cache.h"
#endif
#include "event.h"
#include "cos.h"
#include "memd_m.h"

//extern VDS_PARTITION g_VDS_partition_table[];
extern UINT32 g_vds_partition_count;

// Device description table.this table include name,module id,handle,state,device type and point of option function.
BTOVERLAY_BSS DSM_DEV_DESCRIP* g_pDevDescrip= NULL;

// Device description table recodes number.The arry 0 is reserved,so the value equal to (recode number + 1)
BTOVERLAY_BSS UINT32 g_iDevDescripCount = 0;

// Device simple information table.
BTOVERLAY_BSS DSM_DEV_INFO* g_pDsmDevInfo = NULL;

// Device simple information table record number.
BTOVERLAY_BSS UINT32 g_iDsmDevCount = 0;

// The semaphore of  DSM device  access.
HANDLE g_hDevSem = (HANDLE)DEV_INVALID_SEM_HANDLE;


VOID DSM_SemInit(VOID);
VOID DSM_SemFree(VOID);
VOID DSM_SemDown(VOID);
VOID DSM_SemUp(VOID);
extern void COS_CleanALLCach(VOID);

/**************************************************************************************/
// Function: This function init the device table.
//   Firstly, get the partition config, get the flash partition count and t-flash partition count throught scan the partition config table;
//   Secondly, add the flash partiton into the device talbe, add get the device handle for every flash partition;
//   Thirdly,  add the t-flash partiton into the device talbe, add get the device handle for every t-flash partition;
// Parameter:
// None
// Return value:
// TODO
// Node: the device table uint 0 is reserved. when access the device with dev_no, the zero value is invalid device No.
/***************************************************************************************/
INT32 DSM_DevInit( VOID )
{
    UINT32 iPartCount = 0;
    UINT32 iDeviceCount = 0;
    DSM_PARTITION_CONFIG* pPartConfigInfo = NULL;
    CONST DSM_CONFIG_T* pDsmConfig = NULL;
    UINT32 i,n;
    INT32 dev_handle;
    INT32 iRet;
    //extern void (*g_pGetFileDateTime)(UINT16 *date, UINT16 *time);

    //g_pGetFileDateTime = DSM_GetFileDateTime;

    if( (g_pDevDescrip != NULL) && (g_pDsmDevInfo != NULL) )
        return ERR_SUCCESS;

    DSM_SemDown();

    // Get the partition config information.
    pDsmConfig = tgt_GetDsmCfg();

    pPartConfigInfo = (DSM_PARTITION_CONFIG*)pDsmConfig->dsmPartitionInfo;
    iPartCount = pDsmConfig->dsmPartitionNumber;

    // Malloc for device table.
    iDeviceCount = iPartCount + 1;
    if(g_pDevDescrip == NULL)
        g_pDevDescrip = DSM_MAlloc(SIZEOF(DSM_DEV_DESCRIP)*iDeviceCount);
    
    if(NULL == g_pDevDescrip)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "In DSM_DevInit,malloc failed.\n");
        DSM_ASSERT(0,"DSM_DevInit:1. malloc(0x%x) failed.",(SIZEOF(DSM_DEV_DESCRIP)*iDeviceCount));
        DSM_SemUp();
        return ERR_DRV_NO_MORE_MEM;
    }
    DSM_MemSet(g_pDevDescrip,0,SIZEOF(DSM_DEV_DESCRIP)*iDeviceCount);

    // step1:Initialize the device descrip table.
    // The array 0 is reseved,when dev_no is zero, we consider it is invalid device no,so set the n = 1,
    n = 1;
    for ( i = 0; i < iPartCount; i++ )
    {
        DSM_StrCpy(g_pDevDescrip[n].dev_name,pPartConfigInfo[i].szPartName);
        g_pDevDescrip[n].dev_handle = -1;
        g_pDevDescrip[n].module_id = pPartConfigInfo[i].eModuleId;
        g_pDevDescrip[n].dev_state = DEV_STATE_CLOSE;
        g_pDevDescrip[n].dev_type = pPartConfigInfo[i].eDevType;

        // flash partition. When open successed,this partition will been add to the device descrip table.
        if(pPartConfigInfo[i].eDevType == DSM_MEM_DEV_FLASH)
        {
#if defined(VDS_SUPPORT) // tianwq
            g_pDevDescrip[n].open = (PF_DEV_OPEN)VDS_Open;
            g_pDevDescrip[n].close = (PF_DEV_ClOSE)VDS_Close;
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
            g_pDevDescrip[n].bread = (PF_DEV_BLOCKREAD)VDS_CacheReadBlock;
            g_pDevDescrip[n].bwrite = (PF_DEV_BLOCKWRITE)VDS_CacheWriteBlock;
            g_pDevDescrip[n].brevert = (PF_DEV_BLOCKREVERT)VDS_CacheRevertBlock;
#else
            g_pDevDescrip[n].bread = (PF_DEV_BLOCKREAD)VDS_ReadBlock;
            g_pDevDescrip[n].bwrite = (PF_DEV_BLOCKWRITE)VDS_WriteBlock;
            g_pDevDescrip[n].brevert = (PF_DEV_BLOCKREVERT)VDS_RevertBlock;
#endif
            g_pDevDescrip[n].get_dev_info = (PF_DEV_GETDEVINFO)VDS_GetPartitionInfo;
            g_pDevDescrip[n].set_rcache_size = (PF_DEV_SET_CACHE_SIZE)VDS_SetRCacheSize;
            g_pDevDescrip[n].get_rcache_size = (PF_DEV_GET_CACHE_SIZE)VDS_GetRCacheSize;
            g_pDevDescrip[n].set_wcache_size = (PF_DEV_SET_CACHE_SIZE)VDS_SetWCacheSize;
            g_pDevDescrip[n].get_wcache_size = (PF_DEV_GET_CACHE_SIZE)VDS_GetWCacheSize;
            g_pDevDescrip[n].active = (PF_DEV_ACTIVE)VDS_Active;
            g_pDevDescrip[n].deactive = (PF_DEV_ACTIVE)VDS_Deactive;
            g_pDevDescrip[n].flush = (PF_DEV_ACTIVE)VDS_Flush;
            /*
            // Open the device, get the device handle.
            iRet = (*g_pDevDescrip[n].open)( ((UINT8*)g_pDevDescrip[n].dev_name), &dev_handle );
            if(ERR_SUCCESS == iRet)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "Open device[%s] successfully with handle[%d] returned.",
                g_pDevDescrip[n].dev_name, dev_handle );
                g_pDevDescrip[n].dev_state = DEV_STATE_OPEN;
            }
            else
            {
                CSW_TRACE(BASE_FFS_TS_ID, "Open device[%s] failed with errcode[%d].",
                g_pDevDescrip[n].dev_name, iRet );
                continue;
            }
            g_pDevDescrip[n].dev_handle = dev_handle;
            */
#ifndef _T_UPGRADE_PROGRAMMER
#if 0 // tianwq(CHIP_HAS_USB == 1) 
            if(DSM_StrCaselessCmp(pPartConfigInfo[i].szPartName,"VDS1") == 0)
            {
                UINT64 iBlkNr = 0;
                UINT32 iBlkSz = 0;
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
                init_uFlash_Func (VDS_CacheWriteBlock, VDS_CacheReadBlock);
#else
                init_uFlash_Func (VDS_WriteBlock, VDS_ReadBlock);
#endif // USER_DATA_CACHE_SUPPORT               
                VDS_GetPartitionInfo(i,&iBlkNr,&iBlkSz);
                init_uFlash_Param(i,iBlkNr,iBlkSz);
            }
#endif // CHIP_HAS_USB
#endif  //_T_UPGRADE_PROGRAMMER
#endif
        }
        // t-flash partiton. Don't check the option of opening,and have no use for device handle.
#ifdef MCD_TFCARD_SUPPORT
        else if(pPartConfigInfo[i].eDevType == DSM_MEM_DEV_TFLASH)
        {
            g_pDevDescrip[n].open = (PF_DEV_OPEN)DRV_OpenTFlash;
            g_pDevDescrip[n].close = (PF_DEV_ClOSE)DRV_CloseTFlash;
            g_pDevDescrip[n].bread = (PF_DEV_BLOCKREAD)DRV_ReadTFlash;
            g_pDevDescrip[n].bwrite = (PF_DEV_BLOCKWRITE)DRV_WriteTFlash;
            g_pDevDescrip[n].get_dev_info = (PF_DEV_GETDEVINFO)DRV_GetDevInfoTFlash;
//            g_pDevDescrip[n].set_rcache_size = (PF_DEV_SET_CACHE_SIZE)DRV_SetRCacheSizeTFlash;
//            g_pDevDescrip[n].get_rcache_size = (PF_DEV_GET_CACHE_SIZE)DRV_GetRCacheSizeTFlash;
//            g_pDevDescrip[n].set_wcache_size = (PF_DEV_SET_CACHE_SIZE)DRV_SetWCacheSizeTFlash;
//            g_pDevDescrip[n].get_wcache_size = (PF_DEV_GET_CACHE_SIZE)DRV_GetWCacheSizeTFlash;
            g_pDevDescrip[n].active = (PF_DEV_ACTIVE)DRV_ActiveTFlash;
            g_pDevDescrip[n].deactive = (PF_DEV_ACTIVE)DRV_DeactiveTFlash;
            g_pDevDescrip[n].flush = (PF_DEV_ACTIVE)DRV_FlushTFlash;
        }
#endif
#ifdef MCD_USB_HOST_SUPPORT
        else if(pPartConfigInfo[i].eDevType == DSM_MEM_DEV_USB)
        {
            g_pDevDescrip[n].open = (PF_DEV_OPEN)DRV_OpenUSBDisk;
            g_pDevDescrip[n].close = (PF_DEV_ClOSE)DRV_CloseUSBDisk;
            g_pDevDescrip[n].bread = (PF_DEV_BLOCKREAD)DRV_ReadUSBDisk;
            g_pDevDescrip[n].bwrite = (PF_DEV_BLOCKWRITE)DRV_WriteUSBDisk;
            g_pDevDescrip[n].get_dev_info = (PF_DEV_GETDEVINFO)DRV_GetDevInfoUSBDisk;
//            g_pDevDescrip[n].set_rcache_size = (PF_DEV_SET_CACHE_SIZE)DRV_SetRCacheSizeUSBDisk;
//            g_pDevDescrip[n].get_rcache_size = (PF_DEV_GET_CACHE_SIZE)DRV_GetRCacheSizeUSBDisk;
//            g_pDevDescrip[n].set_wcache_size = (PF_DEV_SET_CACHE_SIZE)DRV_SetWCacheSizeUSBDisk;
//            g_pDevDescrip[n].get_wcache_size = (PF_DEV_GET_CACHE_SIZE)DRV_GetWCacheSizeUSBDisk;
            g_pDevDescrip[n].active = (PF_DEV_ACTIVE)DRV_ActiveUSBDisk;
            g_pDevDescrip[n].deactive = (PF_DEV_ACTIVE)DRV_DeactiveUSBDisk;
            g_pDevDescrip[n].flush = (PF_DEV_ACTIVE)DRV_FlushUSBDisk;
        }       
#endif
        // Open the device, get the device handle.
        if(g_pDevDescrip[n].open == NULL) // unsupport device
        {
            CSW_TRACE(BASE_FFS_TS_ID, "Device[%s] NOT support by this target.",
                      g_pDevDescrip[n].dev_name);
            continue;
        }

        iRet = (*g_pDevDescrip[n].open)( ((UINT8*)g_pDevDescrip[n].dev_name), (UINT32*)&dev_handle );
        if(ERR_SUCCESS == iRet)
        {
            CSW_TRACE(BASE_FFS_TS_ID, "Open device[%s] successfully with handle[%d] returned.",
                      g_pDevDescrip[n].dev_name, dev_handle );
            g_pDevDescrip[n].dev_state = DEV_STATE_OPEN;
        }
        else
        {
            CSW_TRACE(BASE_FFS_TS_ID, "Open device[%s] failed with errcode[%d].",
                      g_pDevDescrip[n].dev_name, iRet );
            continue;
        }
        g_pDevDescrip[n].dev_handle = dev_handle;
        CSW_TRACE(BASE_FFS_TS_ID, "Device[%s] is added to Device dscription,DevNo = %d.\n", g_pDevDescrip[n].dev_name, n);
        n ++;

    }

    // Todo step2 initialize the table for others partition.

    // Step3: init dsm device information table.
    g_iDevDescripCount = n;
    g_iDsmDevCount = n - 1;
    if(g_iDsmDevCount > 0)
    {
        if(g_pDsmDevInfo == NULL)
            g_pDsmDevInfo = DSM_MAlloc(SIZEOF(DSM_DEV_INFO)*g_iDsmDevCount);
        
        if(NULL == g_pDsmDevInfo)
        {
            CSW_TRACE(BASE_FFS_TS_ID, "In DSM_DevInit,malloc failed.");
            DSM_ASSERT(0,"DSM_DevInit: 2.malloc(0x%x) failed.",(SIZEOF(DSM_DEV_INFO)*g_iDsmDevCount));
            DSM_SemUp();
            return ERR_DRV_NO_MORE_MEM;
        }
        DSM_MemSet(g_pDsmDevInfo,0,SIZEOF(DSM_DEV_INFO)*g_iDsmDevCount);
        for(i = 1; i < g_iDevDescripCount; i++)
        {
            g_pDsmDevInfo[i - 1].dev_no  = i;
            DSM_StrCpy( g_pDsmDevInfo[i - 1].dev_name,g_pDevDescrip[i].dev_name);
            g_pDsmDevInfo[i - 1].dev_type = g_pDevDescrip[i].dev_type;
            g_pDsmDevInfo[i - 1].module_id = g_pDevDescrip[i].module_id;
        }
    }
    DSM_SemUp();
    return ERR_SUCCESS;
}



UINT32 DSM_DevName2DevNo( PCSTR pszDevName )
{
    UINT32 i;

    if ( !pszDevName )
    {
        return INVALID_DEVICE_NUMBER;
    }

    for ( i = 1; i < g_iDevDescripCount; i++ )
    {
        //CSW_TRACE(BASE_FFS_TS_ID, "g_pDevDescrip[%d].dev_name = %s,input device name= %s\n", i,g_pDevDescrip[i].dev_name,pszDevName );
        if ( g_pDevDescrip[i].dev_name && 0 == DSM_StrCaselessCmp( pszDevName, g_pDevDescrip[i].dev_name ) )
        {
            break;
        }
    }

    if ( i == g_iDevDescripCount )
    {
        return INVALID_DEVICE_NUMBER;
    }

    return i;
}


INT32 DSM_DevNr2Name( UINT32 uDevNo, PSTR pszDevName )
{
    if ( INVALID_DEVICE_NUMBER == uDevNo || uDevNo >= g_iDevDescripCount || !pszDevName )
    {
        return ERR_DRV_INVALID_DEVNO;
    }

    DSM_StrCpy( pszDevName, g_pDevDescrip[uDevNo].dev_name );

    return ERR_SUCCESS;
}

INT32 DSM_GetDevSpaceSize(PSTR pszDevName,UINT32* pBlkCount,UINT32* pBlkSize)
{
    UINT32 i;
    INT32 iResult;
    UINT32 uBlkCount = 0;
    UINT32 uBlkSize = 0;

    if ( !pszDevName )
    {
        return ERR_DRV_INVALID_DEVNAME;
    }

    for ( i = 1; i < g_iDevDescripCount; i++ )
    {
        // CSW_TRACE(BASE_FFS_TS_ID, "g_pDevDescrip[%d].dev_name = %s,input device name= %s\n", i,g_pDevDescrip[i].dev_name,pszDevName );
        if ( g_pDevDescrip[i].dev_name && 0 == DSM_StrCaselessCmp( pszDevName, g_pDevDescrip[i].dev_name ) )
        {
            iResult = g_pDevDescrip[i].get_dev_info(g_pDevDescrip[i].dev_handle,&uBlkCount,&uBlkSize);
            if(ERR_SUCCESS != iResult)
            {
                return ERR_DRV_DEV_NOT_INIT;
            }
            else
            {
                *pBlkCount = uBlkCount;
                *pBlkSize = uBlkSize;
                break;
            }
        }
    }

    if ( i == g_iDevDescripCount )
    {
        return ERR_DRV_INVALID_DEVNAME;
    }

    return ERR_SUCCESS;
}


UINT32 DSM_GetDevHandle( PCSTR pszDevName )
{
    UINT32 i;

    if ( !pszDevName )
    {
        return ERR_DRV_INVALID_DEVNAME;
    }

    for ( i = 1; i < g_iDevDescripCount; i++ )
    {
        // CSW_TRACE(BASE_FFS_TS_ID, "g_pDevDescrip[%d].dev_name = %s,input device name= %s\n", i,g_pDevDescrip[i].dev_name,pszDevName );
        if ( g_pDevDescrip[i].dev_name && 0 == DSM_StrCaselessCmp( pszDevName, g_pDevDescrip[i].dev_name ) )
        {
            break;
        }
    }

    if ( i == g_iDevDescripCount )
    {
        return ERR_DRV_INVALID_DEVNAME;
    }
    else
    {
        return g_pDevDescrip[i].dev_handle;
    }
}



INT32 DSM_GetDevType(UINT32 uDevNo)
{
    if(uDevNo == INVALID_DEVICE_NUMBER || (UINT32)uDevNo >= g_iDevDescripCount)
    {
        return ERR_DRV_INVALID_DEVNO;
    }

    return  g_pDevDescrip[uDevNo].dev_type;


}

BOOL DSM_IsRemoveableDev(UINT32 uDevNo)
{
    return (DSM_GetDevType(uDevNo) == DSM_MEM_DEV_TFLASH) || (DSM_GetDevType(uDevNo) == DSM_MEM_DEV_USB);
}

INT32 DSM_GetFsRootDevName(PSTR pszDevName )
{
    UINT32 i;

    for ( i = 1; i < g_iDevDescripCount; i++ )
    {
        if ( g_pDevDescrip[i].module_id == DSM_MODULE_FS_ROOT)
        {
            break;
        }
    }
    if ( i == g_iDevDescripCount )
    {
        return ERR_DRV_GET_DEV_FAILED;
    }

    DSM_StrCpy(pszDevName,g_pDevDescrip[i].dev_name);
    return ERR_SUCCESS;
}


INT32 DSM_GetDevNrOnModuleId(DSM_MODULE_ID eModuleId)
{
    UINT32 i;

    for ( i = 1; i < g_iDevDescripCount; i++ )
    {
        if ( g_pDevDescrip[i].module_id == eModuleId)
        {
            break;
        }
    }

    if ( i == g_iDevDescripCount )
    {
        return INVALID_DEVICE_NUMBER;
    }
    return i;

}

INT32 DSM_GetDevCount(VOID)
{
    return g_iDsmDevCount;
}


DSM_MODULE_ID DSM_GetDevModuleId(UINT32 uDevNo)
{
    if(uDevNo == INVALID_DEVICE_NUMBER || (UINT32)uDevNo >= g_iDevDescripCount)
    {
        return ERR_DRV_INVALID_DEVNO;
    }
    return g_pDevDescrip[uDevNo].module_id;
}

//Function
//This function to get the DSM device information.
//    INT32 DSM_GetDeviceInfo(UINT32* pDeviceCount,DSM_DEV_INFO** ppDevInfo);
//Parameter
//   pDeviceCount [out]: Output the count of device.
//   ppDevInfo[out]: Output a array for DSM_DEV_INFO structure.
//Return value
//   Upon successful completion, 0 shall be returned. Otherwise, Error Code is returned. The following error codes may be returned.
//   ERR_DRV_INVALID_PARAMETER: The point of parameter is NULL.
INT32 DSM_GetDeviceInfo(UINT32* pDeviceCount,DSM_DEV_INFO** ppDevInfo)
{
    // Check input parameters.
    if(NULL == pDeviceCount ||
       NULL == ppDevInfo)
    {
        return ERR_DRV_INVALID_PARA;
    }

    // Device not initialized.
    if(NULL == g_pDsmDevInfo ||
       0 == g_iDsmDevCount)
    {
        return ERR_DRV_DEV_NOT_INIT;
    }

    // output the dsm device information
    *ppDevInfo = g_pDsmDevInfo;
    *pDeviceCount  = g_iDsmDevCount;
    return ERR_SUCCESS;

}


///////////////////////////////////////////////////////////////////////
//  Read the partition on byte.
// If read to the end of partition, output the real reading size.
/////////////////////////////////////////////////////////////////////
INT32 DSM_Read( UINT32 uDevNr, UINT32 ulAddrOffset, UINT8* pBuffer,
                UINT32 ulBytesToRead, UINT32* pBytesRead )
{
    INT32 iResult;
    UINT32 ulModuleSize;
    UINT32 ulBlockOffset;
    UINT32 uBlockNO = 0;
    UINT64 uNrBlock = 0;
    UINT32 uBlockSize = 0;

    UINT8 pBlockBuffer[DSM_DEFAULT_SECSIZE] = {0,};
    UINT32 ulPos;
    UINT32 ulLenRead = 0;


    // Check the input parameter.
    if ( INVALID_DEVICE_NUMBER == uDevNr ||
         uDevNr >= g_iDevDescripCount ||
         NULL == pBuffer ||
         NULL == pBytesRead)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "Invalid parameter,dev_no = %d,pbuffer = 0x%x,pbytesread = 0x%x.\n",
                  uDevNr,
                  pBuffer,
                  pBytesRead
                 );
        return ERR_DRV_INVALID_PARA;
    }

    DSM_SemDown();

    // Get the block count and block size.
    iResult = DRV_GET_DEV_TFLASH_INFO( uDevNr, &uNrBlock, &uBlockSize);
    if(ERR_SUCCESS != iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "get dev info failed,dev_nr = %d,err code = %d.\n",
                  uDevNr,
                  iResult
                 );
        DSM_SemUp();
        return ERR_DRV_GET_DEV_FAILED;
    }

    // Judge the address offset if exceed the moudule size.
    ulModuleSize = uBlockSize* uNrBlock;
    if ( ulAddrOffset >= ulModuleSize )
    {
        CSW_TRACE(BASE_FFS_TS_ID, "block number to read overflow total block nuber. to read = 0x%x,total = 0x%x.\n",
                  ulAddrOffset,
                  ulModuleSize
                 );
        DSM_SemUp();
        return ERR_DRV_INVALID_PARA;
    }

    // Figure out the spare size.
    if ( ulAddrOffset + ulBytesToRead > ulModuleSize )
    {
        ulBytesToRead = ulModuleSize - ulAddrOffset;
    }

    ulPos = ulAddrOffset;
    ulBlockOffset = ulPos % uBlockSize;
    uBlockNO = ulPos / uBlockSize;

    // Read the first block.
    if ( ulBlockOffset != 0 )
    {

        iResult = DRV_BLOCK_READ(uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_readblock(first) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      g_pDevDescrip[uDevNr].dev_handle,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_READ_FAILED;
        }

        if ( ulBlockOffset + ulBytesToRead <= uBlockSize )
        {
            DSM_MemCpy( pBuffer + ulLenRead, pBlockBuffer + ulBlockOffset, ulBytesToRead );
            ulPos += ulBytesToRead;
            ulLenRead += ulBytesToRead;
        }
        else
        {
            DSM_MemCpy( pBuffer + ulLenRead, pBlockBuffer + ulBlockOffset, uBlockSize - ulBlockOffset );
            ulPos += ( uBlockSize - ulBlockOffset );
            ulLenRead += ( uBlockSize - ulBlockOffset );
        }
    }

    // Read middle block.
    while ( ulLenRead + uBlockSize <= ulBytesToRead )
    {
        uBlockNO = ulPos / uBlockSize;
        iResult = DRV_BLOCK_READ(uDevNr, uBlockNO, pBuffer + ulLenRead );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_readblock(next) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_READ_FAILED;
        }

        ulPos += uBlockSize;
        ulLenRead += uBlockSize;
    }

    // Read the last block.
    if ( ulLenRead < ulBytesToRead )
    {
        uBlockNO =  ulPos / uBlockSize;
        iResult = DRV_BLOCK_READ(uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_readblock(last) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_READ_FAILED;
        }

        DSM_MemCpy( pBuffer + ulLenRead, pBlockBuffer, ulBytesToRead - ulLenRead );
        ulLenRead += ( ulBytesToRead - ulLenRead );
    }

    *pBytesRead = ulLenRead;
    DSM_SemUp();
    return ERR_SUCCESS;

}



///////////////////////////////////////////////////////////////////////
//  Write the partition on byte.
// If write to the end of partition, output the real writing size.
/////////////////////////////////////////////////////////////////////
INT32 DSM_Write( UINT32 uDevNr, UINT32 ulAddrOffset, CONST UINT8* pBuffer,
                 UINT32 ulBytesToWrite, UINT32* pBytesWritten )
{
    INT32 iResult;
    UINT32 ulModuleSize;
    UINT32 ulBlockOffset;
    UINT32 uBlockNO;
    UINT64 uNrBlock = 0;
    UINT32 uBlockSize = 0;
    UINT8 pBlockBuffer[DSM_DEFAULT_SECSIZE] = {0,};
    UINT32 ulPos;
    UINT32 ulLenWriten = 0;

    // Check the input parameter.
    if ( INVALID_DEVICE_NUMBER == uDevNr ||
         uDevNr >= g_iDevDescripCount ||
         NULL == pBuffer ||
         NULL == pBytesWritten)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "Invalid parameter,dev_no = %d,pbuffer = 0x%x,pbyteswritten = 0x%x.\n",
                  uDevNr,
                  pBuffer,
                  pBytesWritten
                 );
        return ERR_DRV_INVALID_PARA;
    }

    DSM_SemDown();

    // Get the block count and block size.
    iResult = DRV_GET_DEV_TFLASH_INFO( uDevNr, &uNrBlock, &uBlockSize);
    if(ERR_SUCCESS != iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "get dev info failed,dev_nr = %d,err code = %d.\n",
                  uDevNr,
                  iResult
                 );
        DSM_SemUp();
        return ERR_DRV_GET_DEV_FAILED;
    }

    // Judge the address offset if exceed the moudule size.
    ulModuleSize = uBlockSize* uNrBlock;
    if ( ulAddrOffset >= ulModuleSize )
    {
        CSW_TRACE(BASE_FFS_TS_ID, "block number to write overflow total block nuber. to write = 0x%x,total = 0x%x.\n",
                  ulAddrOffset,
                  ulModuleSize
                 );
        DSM_SemUp();
        return ERR_DRV_INVALID_PARA;
    }

    // Figure out the spare size.
    if ( ulAddrOffset + ulBytesToWrite > ulModuleSize )
    {
        ulBytesToWrite = ulModuleSize - ulAddrOffset;
    }

    ulPos = ulAddrOffset;
    ulBlockOffset = ulPos % uBlockSize;
    uBlockNO = ulPos / uBlockSize ;

    // Write the first block.
    if ( ulBlockOffset != 0 )
    {
        iResult = DRV_BLOCK_READ(uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_readblock(write before) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_READ_FAILED;
        }

        if ( ulBlockOffset + ulBytesToWrite <= uBlockSize )
        {
            DSM_MemCpy( pBlockBuffer + ulBlockOffset, pBuffer + ulLenWriten, ulBytesToWrite );
            ulPos += ulBytesToWrite;
            ulLenWriten += ulBytesToWrite;
        }
        else
        {
            DSM_MemCpy( pBlockBuffer + ulBlockOffset, pBuffer + ulLenWriten, uBlockSize - ulBlockOffset );
            ulPos += ( uBlockSize - ulBlockOffset );
            ulLenWriten += ( uBlockSize - ulBlockOffset );
        }

        iResult = DRV_BLOCK_WRITE(uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_writeblock(first) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_WRITE_FAILED;
        }
    }

    // Write middle block.
    while ( ulLenWriten + uBlockSize <= ulBytesToWrite )
    {
        uBlockNO =  ulPos / uBlockSize;
        DSM_MemCpy(pBlockBuffer,pBuffer + ulLenWriten,DSM_DEFAULT_SECSIZE);
        iResult = DRV_BLOCK_WRITE(uDevNr, uBlockNO, ( UINT8* )  pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_writeblock(next) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_WRITE_FAILED;
        }

        ulPos += uBlockSize;
        ulLenWriten += uBlockSize;
    }

    // Write the last block.
    if ( ulLenWriten < ulBytesToWrite )
    {
        uBlockNO = ulPos / uBlockSize ;
        iResult = DRV_BLOCK_READ( uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_readblock(last befor) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_READ_FAILED;
        }

        DSM_MemCpy( pBlockBuffer, pBuffer + ulLenWriten, ulBytesToWrite - ulLenWriten );
        iResult = DRV_BLOCK_WRITE(uDevNr, uBlockNO, pBlockBuffer );
        if ( ERR_SUCCESS != iResult )
        {
            CSW_TRACE(BASE_FFS_TS_ID, "vds_writeblock(last) failed,dev_handle = %d,block_no= 0x%x,err code = %d.\n",
                      uDevNr,
                      uBlockNO,
                      iResult
                     );
            DSM_SemUp();
            return ERR_DRV_WRITE_FAILED;
        }
        ulLenWriten += ( ulBytesToWrite - ulLenWriten );
    }

    *pBytesWritten = ulLenWriten;
    DSM_SemUp();
    return ERR_SUCCESS;

}

#define USER_DATA_FLAG          (0xa5e8f5dc)
#define USER_DATA_BLOCK_NUM     (USER_DATA_SIZE/USER_DATA_BLOCK_SIZE)    // Total 4KB in flash
#define USER_DATA_BUFFER_SIZE          (512*3) // 2KB in RAM

struct UserData_Header_t
{
    UINT32 flag;
    UINT16 length;
    UINT16 map;
} *p_Data_Header;

struct UserData_Modify_t
{
    UINT16 index;
    UINT16 offset;
    UINT16 length;
};
static UINT16 *user_data_buff = NULL;
//static UINT32 *&_user_data_start = NULL;
UINT32 g_curModifyPos = 0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#define memd_FlashCacheFlush COS_CleanALLCach
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define memd_FlashCacheFlush hal_FlashCacheFlush
#elif  (CHIP_ASIC_ID == CHIP_ASIC_ID_5857) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E2)
#define memd_FlashCacheFlush()
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
volatile UINT8 HAL_SHARED_BSS g_flash_write_req;
#else
volatile UINT8 HAL_UNCACHED_BSS g_flash_write_req; // TODO: Remove me
#endif

INT32 DSM_FlashWrite(UINT8 *flashAddress,
                     UINT32 byteSize,
                     UINT32 * pWrittenByteSize,
                     CONST UINT8* buffer)
{
    return memd_FlashWrite((UINT8*)((UINT32)flashAddress-(UINT32)&_flash_start),
                           byteSize,pWrittenByteSize,buffer);

}

INT32 DSM_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    return memd_FlashErase((UINT8*)((UINT32)startFlashAddress-(UINT32)&_flash_start),
                           (UINT8*)((UINT32)endFlashAddress-(UINT32)&_flash_start));
}
//-------------------------------------------------------------------------------------------------
//  Clear the data on flash user region.
//  Must reset the system after called this function.
//  Return value:
//     ERR_SUCCESS:                      Clear data succed.
//     ERR_DRV_GET_DEV_FAILED:   Get the device information failed.
//     ERR_DRV_ERASE_FAILED:       Erase flash sector failed.
//
//------------------------------------------------------------------------------------------------
#if 0
INT32 DSM_UserDataClear(VOID)
{
    UINT32 *flag = (UINT32*)user_data_buff;
    memset(user_data_buff, 0, USER_DATA_BUFFER_SIZE);
    p_Data_Header->flag = USER_DATA_FLAG;
    p_Data_Header->length = sizeof(struct UserData_Header_t)/2+1;
    p_Data_Header->map = 1;
    return ERR_SUCCESS;
}

void *DSM_GetUserData(UINT8 index, UINT32 length)
{
    int pos = sizeof(struct UserData_Header_t)/2;

    if(p_Data_Header==NULL) //(user_data_buff == NULL)
    {
        //user_data_buff = COS_Malloc(USER_DATA_BUFFER_SIZE);
        p_Data_Header = (struct UserData_Header_t*)user_data_buff;
        DSM_UserDataClear();
    }

    if(p_Data_Header->flag != USER_DATA_FLAG)
    {
        CSW_TRACE(BASE_FFS_TS_ID,  "Get UserData flag error:%x",p_Data_Header->flag);
        DSM_UserDataClear();
    }

    while(pos < USER_DATA_BUFFER_SIZE/2)
    {
        if(user_data_buff[pos] == index || user_data_buff[pos] == 0)
        {
            if(user_data_buff[pos] == 0)
            {
                user_data_buff[pos] = index;
                user_data_buff[pos+1] = length;
                //user_data_buff[pos+((length+3)&~3)/2 + 2] = 0;
                p_Data_Header->length += ((length+3)&~3)/2 + 2;
                user_data_buff[p_Data_Header->length-1] = 0;
            }
            return &user_data_buff[pos+2];
        }
        pos += ((user_data_buff[pos+1]+3)&~3)/2 + 2;
    }
    hal_HstSendEvent(SYS_EVENT,0xfffffffd); // user data overflow
}

INT32 DSM_ReadUserData(void)
{
    UINT32* src;
    UINT32* dst;
    UINT32 memorymap,i,j;

    //if(user_data_buff == NULL)
    {
        //user_data_buff = COS_Malloc(USER_DATA_BUFFER_SIZE);
        p_Data_Header = (struct UserData_Header_t*)user_data_buff;
        DSM_UserDataClear();
    }

    for(i=0; i<USER_DATA_BLOCK_NUM; i++)
    {
        memcpy(p_Data_Header,&_user_data_start+i*USER_DATA_BLOCK_SIZE/4,sizeof(struct UserData_Header_t));
        if(p_Data_Header->flag==USER_DATA_FLAG)
            break;
    }

    memorymap=p_Data_Header->map;
    CSW_TRACE(BASE_FFS_TS_ID,  "Read Flash i=%d,memorymap=%x",i,memorymap);
    memorymap = memorymap>>i;
    for(j=0; i<USER_DATA_BLOCK_NUM; i++)
    {
        if((memorymap&1)!=1)
        {
            memorymap=memorymap>>1;
            continue;
        }
        memcpy(&user_data_buff[j*(USER_DATA_BLOCK_SIZE/2)], (UINT32*) (&_user_data_start+i*USER_DATA_BLOCK_SIZE/4), USER_DATA_BLOCK_SIZE);
        j++;
        memorymap=memorymap>>1;
    }
    return ERR_SUCCESS;
}

INT32 DSM_WriteUserData(void)
{
    UINT32 write,err,i,j,memorymap=0,k,head_pos;
    UINT16 *tmp_usr_buff;

#if 0
    hal_HstSendEvent(SYS_EVENT,0x03210010);
    hal_HstSendEvent(SYS_EVENT,&_flash_start);
    hal_HstSendEvent(SYS_EVENT,&_user_data_start);
    hal_HstSendEvent(SYS_EVENT,USER_DATA_BLOCK_NUM);
#endif
    UINT32 status;// = hal_SysEnterCriticalSection();
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
    while(!hal_VocIsClose())
    {
        //hal_HstSendEvent(SYS_EVENT,0x0321001A);
        COS_Sleep(5);
    }
#endif
    g_flash_write_req=1;
    tmp_usr_buff = (UINT16*)COS_Malloc(USER_DATA_BLOCK_SIZE);
#if !defined(_FLASH_PROGRAMMER)
#if CHIP_HAS_BTCPU==1
    //hal_TimDelay(5);
    hal_HstSendEvent(SYS_EVENT,g_flash_write_req);
    if(hal_IsBtStart())
    {
        if(hal_IsBtSleep())
            hal_BtNotifyDataSent();
        while(g_flash_write_req!=2);
    }
#endif
#endif
    status = hal_SysEnterCriticalSection();
    DSM_FlashErase((UINT8*)&_user_data_start, (UINT8*)((UINT32)&_user_data_start)+(USER_DATA_BLOCK_NUM*USER_DATA_BLOCK_SIZE));
    p_Data_Header->flag = 0xffffffff;
    p_Data_Header->map = 0xffff; // write memory map at last
    for(i=0,k=0; i<=USER_DATA_BLOCK_NUM; i++)
    {
        //CSW_TRACE(BASE_FFS_TS_ID,  "Write Flash i=%d,k=%d",i,k);
        if( i >= USER_DATA_BLOCK_NUM)
        {
            p_Data_Header->flag = USER_DATA_FLAG; // restore flag
            g_flash_write_req=0;
            hal_SysExitCriticalSection(status);
            COS_Free(tmp_usr_buff);
            hal_HstSendEvent(SYS_EVENT,0x03210012);
            return 1;//no enough memory
        }

        DSM_FlashWrite((UINT8*)((UINT32)&_user_data_start+i*USER_DATA_BLOCK_SIZE), USER_DATA_BLOCK_SIZE, &write, (UINT8*)&user_data_buff[k*(USER_DATA_BLOCK_SIZE/2)]);
        memcpy((UINT32*)tmp_usr_buff,(UINT32*) (((UINT32)&_user_data_start+i*USER_DATA_BLOCK_SIZE)|0x20000000), USER_DATA_BLOCK_SIZE);
        err = 0;
        for(j=0; j<USER_DATA_BLOCK_SIZE/2; j++)
        {
            if(tmp_usr_buff[j]!=user_data_buff[k*USER_DATA_BLOCK_SIZE/2+j])
            {
                err = 1;
                break;
            }
        }
        //CSW_TRACE(BASE_FFS_TS_ID,  "Check Flash i=%d,k=%d,err=%d",i,k,err);
        if(!err)
        {
            if(k==0)
                head_pos = i;
            memorymap|=(1<<i);
            k++;
            if(k*USER_DATA_BLOCK_SIZE>=p_Data_Header->length*2)
                break;
        }

    }

    CSW_TRACE(BASE_FFS_TS_ID, "Address = %x, map = %x",((UINT32)&_user_data_start),memorymap);
    p_Data_Header->flag = USER_DATA_FLAG;
    p_Data_Header->map = memorymap;
    DSM_FlashWrite((UINT8*)((UINT32)&_user_data_start+head_pos*USER_DATA_BLOCK_SIZE),sizeof(struct UserData_Header_t),&write,(UINT8*)p_Data_Header);

    g_flash_write_req=0;
    hal_SysExitCriticalSection(status);
    COS_Free(tmp_usr_buff);
    hal_HstSendEvent(SYS_EVENT,0x03210011);
    return ERR_SUCCESS;
}
#else
extern void DSM_EnqNvram(void);
INT32 DSM_ReadFlashUserData(UINT16 *p_user_data);

INT32 DSM_InitUserData(void *user_data)
{
    struct UserData_Header_t *p_user_header = NULL;
    if(user_data == NULL)
        return 1;
    p_user_header = (struct UserData_Header_t *)user_data;
    memset((UINT8 *)user_data, 0, USER_DATA_BUFFER_SIZE);

    p_user_header->flag = USER_DATA_FLAG;
    p_user_header->length = sizeof(struct UserData_Header_t)+2;
    p_user_header->map = 1;
    return ERR_SUCCESS;
}
INT32 DSM_UserDataClear(VOID)
{
    DSM_InitUserData(user_data_buff);
    return ERR_SUCCESS;
}
INT32 DSM_CreatUserData(VOID)
{
    if(user_data_buff == NULL)
    {
        user_data_buff = (UINT16 *)COS_Malloc(USER_DATA_BUFFER_SIZE);
        if(user_data_buff == NULL)
            return 1;
    }
    p_Data_Header = (struct UserData_Header_t*)user_data_buff;
    DSM_InitUserData(user_data_buff);
    return ERR_SUCCESS;
}
void *DSM_GetUserData(UINT8 index, UINT32 length)
{
    int pos = sizeof(struct UserData_Header_t)/2;
    while(pos < USER_DATA_BUFFER_SIZE/2)
    {
        if(user_data_buff[pos] == index || user_data_buff[pos] == 0)
        {
            if(user_data_buff[pos] == 0)
            {
                user_data_buff[pos] = index;
                user_data_buff[pos+1] = length;
                p_Data_Header->length += ((length+3)&~3) + 4;
                user_data_buff[p_Data_Header->length/2-1] = 0;
            }
            return &user_data_buff[pos+2];
        }
        pos += ((user_data_buff[pos+1]+3)&~3)/2 + 2;
    }
    hal_HstSendEvent(SYS_EVENT,0xfffffffd); // user data overflow
    return NULL;
}

void *DSM_GetUserDataBackUp(UINT16 *user_data,UINT8 index, UINT32 length)
{
    int pos = sizeof(struct UserData_Header_t)/2;
    struct UserData_Header_t *p_data_header = (struct UserData_Header_t *)user_data;
    while(pos < USER_DATA_BUFFER_SIZE/2)
    {
        if(user_data[pos] == index || user_data[pos] == 0)
        {
            if(user_data[pos] == 0)
            {
                user_data[pos] = index;
                user_data[pos+1] = length;
                p_data_header->length += ((length+3)&~3) + 4;
                user_data[p_data_header->length/2-1] = 0;
            }
            return &user_data[pos+2];
        }
        pos += ((user_data[pos+1]+3)&~3)/2 + 2;
    }
    hal_HstSendEvent(SYS_EVENT,0xfffffffd); // user data overflow
    return NULL;
}
INT32 DSM_ReadUserData(void)
{
#if 0    
    UINT32* src;
    UINT32* dst;
    UINT32 memorymap,i,j;
    UINT32 endOfUserData = 0;
    UINT16 *p_UserData = NULL;
    UINT16 *p_ModifyFlash = NULL;
    struct UserData_Header_t p_DataHeader;
    struct UserData_Modify_t p_DataModify;
#endif
    hal_HstSendEvent(SYS_EVENT,0x20160903);
    if(DSM_CreatUserData())
        return 1;
    DSM_EnqNvram();
    if(DSM_ReadFlashUserData(user_data_buff) == USER_DATA_SIZE)
    {
        DSM_InitUserData(user_data_buff);
    }
    hal_HstSendEvent(SYS_EVENT,0x20160902);
    hal_HstSendEvent(SYS_EVENT,(UINT32) user_data_buff);
    hal_HstSendEvent(SYS_EVENT,(UINT32) &_user_data_start);
    return ERR_SUCCESS;
}

INT32 DSM_ReadFlashUserData(UINT16 *p_user_data)
{
    //UINT32* src;
    //UINT32* dst;
    UINT32 memorymap,i,j;
    UINT32 endOfUserData = 0;
    UINT16 *p_UserData = NULL;
    UINT16 *p_ModifyFlash = NULL;
    struct UserData_Header_t p_DataHeader;
    struct UserData_Modify_t p_DataModify;
    UINT32 modify_start_pos = 0;
    for(i=0; i<USER_DATA_BLOCK_NUM; i++)
    {
        memcpy(&p_DataHeader,&_user_data_start+i*USER_DATA_BLOCK_SIZE/4,sizeof(struct UserData_Header_t));
        if(p_DataHeader.flag == USER_DATA_FLAG)
        {
            memcpy(p_user_data,&p_DataHeader,sizeof(struct UserData_Header_t));
            break;
        }
    }

    hal_HstSendEvent(SYS_EVENT,0x87000000+i);
    memorymap=((struct UserData_Header_t *)p_user_data)->map;
    CSW_TRACE(BASE_FFS_TS_ID,  "Read Flash i=%d,memorymap=%x",i,memorymap);
    memorymap = memorymap>>i;
    for(j=0; i<USER_DATA_BLOCK_NUM; i++)
    {
        if((memorymap&1)!=1)
        {
            memorymap=memorymap>>1;
            continue;
        }
        memcpy(&p_user_data[j*(USER_DATA_BLOCK_SIZE/2)], (UINT32*)(&_user_data_start+i*USER_DATA_BLOCK_SIZE/4), USER_DATA_BLOCK_SIZE);
        j++;
        memorymap=memorymap>>1;
        endOfUserData = i;
    }
    hal_HstSendEvent(SYS_EVENT,0x86000000+endOfUserData);
    if(endOfUserData&&(endOfUserData+1<USER_DATA_BLOCK_NUM))
    {
        p_ModifyFlash = (UINT16*) (&_user_data_start+(endOfUserData+1)*USER_DATA_BLOCK_SIZE/4);
        modify_start_pos = (endOfUserData+1)*USER_DATA_BLOCK_SIZE;
        hal_HstSendEvent(SYS_EVENT,0x88000000+modify_start_pos);
        memcpy(&p_DataModify,p_ModifyFlash,sizeof(struct UserData_Modify_t));
        while((p_DataModify.index != 0xffff)&&(modify_start_pos<USER_DATA_SIZE))
        {
            if(((p_DataModify.index&0xff00) != 0x0)||((p_DataModify.index&0xff) == 0x0))
                goto exitFun;
            if((p_DataModify.offset == 0xffff)||(p_DataModify.length == 0xffff))
                goto exitFun;
            p_UserData = DSM_GetUserDataBackUp(p_user_data,p_DataModify.index,0);
            if(p_DataModify.offset+p_DataModify.length > *(p_UserData-1))
                goto exitFun;
            memcpy(p_UserData+p_DataModify.offset/2,p_ModifyFlash+sizeof(struct UserData_Modify_t)/2,p_DataModify.length);
            p_ModifyFlash += sizeof(struct UserData_Modify_t)/2+p_DataModify.length/2;
            modify_start_pos += sizeof(struct UserData_Modify_t)+p_DataModify.length;
            memcpy(&p_DataModify,p_ModifyFlash,sizeof(struct UserData_Modify_t));
        }
    }
    hal_HstSendEvent(SYS_EVENT,0x8888888);
    hal_HstSendEvent(SYS_EVENT,modify_start_pos);
    g_curModifyPos = modify_start_pos;
    return modify_start_pos;
exitFun:
    hal_HstSendEvent(SYS_EVENT,0xdaeddad);
    hal_HstSendEvent(SYS_EVENT,modify_start_pos);
    modify_start_pos = USER_DATA_SIZE;
    g_curModifyPos = modify_start_pos;
    return modify_start_pos;
}
INT32 DSM_UserDataNeedUpdate(UINT16 *user_data_update)
{
    UINT32 backupLen = 0;
    UINT32 curlen = 0;
    UINT32 curpos = 0;
    UINT16 count = 0;
    UINT16 continueFlag = 0;
    UINT16 modifyPos = 0;
    UINT16 *modifyLen = NULL;
    UINT16 indexLen = 0;
    UINT16 index = 0;
    UINT32 cur_modify_start = 0;
    UINT32 ret = 0;
    struct UserData_Header_t *p_flash_data_header;
    UINT16 *user_data_backup;
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    //COS_Malloc failed in 5856e BT MODE, we use some special memory instead.
    if(hal_IsBtStart())
    {
        p_flash_data_header = (struct UserData_Header_t *)0x81c02070;
    }
    else
    {
        p_flash_data_header =(struct UserData_Header_t *)COS_Malloc_BTOverlay(USER_DATA_BUFFER_SIZE);
    }
#else
    //4 Can NOT use COS_Malloc_BTOverlay in here
    p_flash_data_header =(struct UserData_Header_t *)COS_Malloc(USER_DATA_BUFFER_SIZE);
#endif

    user_data_backup = (UINT16 *)p_flash_data_header;
    cur_modify_start = DSM_ReadFlashUserData(user_data_backup);
    if((cur_modify_start == 0)||(cur_modify_start == USER_DATA_SIZE))
    {
        ret = USER_DATA_SIZE;
        goto exitFun;
    }

    backupLen = p_flash_data_header->length;
    curlen = p_Data_Header->length;
    curpos = sizeof(struct UserData_Header_t);
    if(!memcmp(&user_data_buff[curpos/2],&user_data_backup[curpos/2],curlen-curpos))
    {
        ret = 0;
        goto exitFun;
    }
    while(((index =user_data_buff[curpos/2]) != 0)&&(curpos<curlen))
    {
        continueFlag = 0;
        count = 0;
        modifyLen = NULL;
        indexLen = ((user_data_buff[curpos/2+1]+3)&~3);
        if(((index&0xff00) != 0)||(indexLen == 0xffff))
        {
            ret = 0;
            goto exitFun;
        }
        curpos += 4;
        while(count < indexLen/2)//1count=2byte
        {
            if(user_data_buff[curpos/2+count] != user_data_backup[curpos/2+count])
            {
                if(!continueFlag)
                {
                    if(modifyPos>USER_DATA_BUFFER_SIZE/16-3)
                    {
                    	ret = modifyPos*2;
                    	goto exitFun;
                    }	
                    user_data_update[modifyPos++] = index;
                    user_data_update[modifyPos++] = count*2;
                    modifyLen = &user_data_update[modifyPos++];
                    continueFlag = 1;
                }
		if(modifyPos>USER_DATA_BUFFER_SIZE/16-1)
               	{
                        modifyPos  = modifyLen -user_data_update -2;
			ret = modifyPos*2;
			goto exitFun;
                }	
                user_data_update[modifyPos++] = user_data_buff[curpos/2+count];
            }
            else
            {
                if(modifyLen !=NULL)
                {
                    *modifyLen = count*2-*(modifyLen-1);
                    modifyLen = NULL;
                }
                continueFlag = 0;
            }
            count ++;
        }
        if(modifyLen !=NULL)
        {
            *modifyLen = count*2-*(modifyLen-1);
            modifyLen = NULL;
        }
        continueFlag = 0;
        curpos += indexLen;
    }
    ret = modifyPos*2;
exitFun:
    hal_HstSendEvent(SYS_EVENT,0x81000000+modifyPos*2);
    hal_HstSendEvent(SYS_EVENT,curpos);
    hal_HstSendEvent(SYS_EVENT,ret);



#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    if(0x81c02070 == (UINT32) p_flash_data_header)
    {
        ;
    }
    else if(p_flash_data_header)
    {
        COS_Free_BTOverlay(p_flash_data_header);
    }
#else
    if(p_flash_data_header)
    {
        COS_Free(p_flash_data_header);
    }
#endif


    return ret;
}
INT32 DSM_WriteUserData(void)
{
    UINT32 write,err,i,memorymap=0,k,head_pos = 0;
    UINT16 *tmp_usr_buff = NULL;
    UINT16 updateLen = 0;
    UINT32 status = 0;
#if CHIP_HAS_BTCPU==1
    UINT16 count;
#endif
    UINT32 ret = 0;
    //UINT32 x = 0;
    UINT16 *user_data_update = NULL;
    hal_HstSendEvent(SYS_EVENT,0X20160901);
#if 0
    hal_HstSendEvent(SYS_EVENT,0x03210010);
    hal_HstSendEvent(SYS_EVENT,&_flash_start);
    hal_HstSendEvent(SYS_EVENT,&_user_data_start);
    hal_HstSendEvent(SYS_EVENT,USER_DATA_BLOCK_NUM);
#endif
    if(!hal_VocIsClose())
    {
        hal_HstSendEvent(SYS_EVENT,0x0321001A);
        return 1;
    }
    g_flash_write_req=1;
#if !defined(_FLASH_PROGRAMMER)
#if CHIP_HAS_BTCPU==1
    //hal_TimDelay(5);
    hal_HstSendEvent(SYS_EVENT,g_flash_write_req);
    for(count = 0; hal_IsBtStart(); count++)
    {
        //if(hal_IsBtSleep())
        hal_BtNotifyDataSent();
        if(g_flash_write_req==2)
        {
            break;
        }
        else if(count > 20)
        {
            hal_HstSendEvent(SYS_EVENT,0x0321001B);
            g_flash_write_req = 0;
            return 2;
        }
        else
        {
            COS_Sleep(10);
        }
    }
#endif
#endif

    status= hal_SysEnterCriticalSection();
    user_data_update = (UINT16 *)COS_Malloc(USER_DATA_BUFFER_SIZE/8);
    if(!(updateLen = DSM_UserDataNeedUpdate(user_data_update)))
    {
        goto exitFun;
    }
    hal_HstSendEvent(SYS_EVENT,0x83000000+g_curModifyPos);
    if((g_curModifyPos + updateLen >= USER_DATA_SIZE)||(g_curModifyPos == 0))
    {
        DSM_FlashErase((UINT8*)&_user_data_start, (UINT8*)((UINT32)&_user_data_start)+(USER_DATA_BLOCK_NUM*USER_DATA_BLOCK_SIZE));
        tmp_usr_buff = (UINT16*)COS_Malloc(USER_DATA_BLOCK_SIZE);
        p_Data_Header->flag = 0xffffffff;
        p_Data_Header->map = 0xffff; // write memory map at last
        for(i=0,k=0; i<=USER_DATA_BLOCK_NUM; i++)
        {
            if( i >= USER_DATA_BLOCK_NUM)
            {
                p_Data_Header->flag = USER_DATA_FLAG; // restore flag
                g_curModifyPos = i*USER_DATA_BLOCK_SIZE;
                hal_HstSendEvent(SYS_EVENT,0x43210001);
                goto exitFun;
            }
            ret = DSM_FlashWrite((UINT8*)((UINT32)&_user_data_start+i*USER_DATA_BLOCK_SIZE), USER_DATA_BLOCK_SIZE, &write, (UINT8*)&user_data_buff[k*(USER_DATA_BLOCK_SIZE/2)]);
            memd_FlashCacheFlush();
            memcpy((UINT32*)tmp_usr_buff,(UINT32*) (((UINT32)&_user_data_start+i*USER_DATA_BLOCK_SIZE)|0x20000000), USER_DATA_BLOCK_SIZE);
            err = 0;
            if(memcmp(tmp_usr_buff,&user_data_buff[k*USER_DATA_BLOCK_SIZE/2],USER_DATA_BLOCK_SIZE))
            {
                hal_HstSendEvent(SYS_EVENT,0x43210002);
                err = 1;
            }
            if(!err)
            {
                if(k==0)
                    head_pos = i;
                memorymap|=(1<<i);
                k++;
                if(k*USER_DATA_BLOCK_SIZE>=USER_DATA_BUFFER_SIZE)
                    break;
            }
        }
        CSW_TRACE(BASE_FFS_TS_ID, "Address = %x, map = %x",((UINT32)&_user_data_start),memorymap);
        p_Data_Header->flag = USER_DATA_FLAG;
        p_Data_Header->map = memorymap;
        DSM_FlashWrite((UINT8*)((UINT32)&_user_data_start+head_pos*USER_DATA_BLOCK_SIZE),sizeof(struct UserData_Header_t),&write,(UINT8*)p_Data_Header);
        g_curModifyPos = (i+1)*USER_DATA_BLOCK_SIZE;
        hal_HstSendEvent(SYS_EVENT,0x43210003);
        goto exitFun;
    }
    hal_HstSendEvent(SYS_EVENT,0x89000000+g_curModifyPos);
    DSM_FlashWrite((UINT8*)&_user_data_start+g_curModifyPos, updateLen, &write, (UINT8*)user_data_update);
    memd_FlashCacheFlush();
    if(memcmp((UINT8*)&_user_data_start+g_curModifyPos,(UINT8*)user_data_update,updateLen))
    {
        g_curModifyPos = (USER_DATA_BLOCK_NUM*USER_DATA_BLOCK_SIZE);
        hal_HstSendEvent(SYS_EVENT,0x43210004);
    }
    else
    {
        g_curModifyPos += updateLen;
        hal_HstSendEvent(SYS_EVENT,0x43210005);
    }
exitFun:
    memd_FlashCacheFlush();
    hal_HstSendEvent(SYS_EVENT,0x84000000+g_curModifyPos);
    g_flash_write_req=0;
    if(tmp_usr_buff != NULL)
        COS_Free(tmp_usr_buff);
    if(user_data_update != NULL)
        COS_Free(user_data_update);
    hal_SysExitCriticalSection(status);
    return ERR_SUCCESS;
}
#endif

#define PHONE_BOOK_LENGTH      81920 // 80KB
#define PHONE_BOOK_FLAG        0xD5BC2D18
static UINT32 *phone_book_data_start = NULL;

struct PhoneBook_Header_t
{
    UINT32 flag;
    UINT16 count;
    UINT8  item_size;
    UINT8  index_size;
    UINT32 valid_count;
} phone_header;

void DSM_Erase_Phonebook(void)
{
    int i = 0;
    UINT32 scStatus = 0;

    for(i = 0; i < PHONE_BOOK_LENGTH/4096; i++)
    {
        scStatus = hal_SysEnterCriticalSection();
        DSM_FlashErase((UINT8*)((UINT32)phone_book_data_start)+4096*i, (UINT8*)((UINT32)phone_book_data_start)+4096*(i+1));
        hal_SysExitCriticalSection(scStatus);
    }
}

void DSM_ResetPhoneBookAddress(UINT16 size, UINT8 sizeofItem, UINT8 sizeofIndex)
{
    int i = 0;
    UINT32 scStatus = 0;
    phone_header.flag = PHONE_BOOK_FLAG;
    phone_header.count = size;
    phone_header.item_size = sizeofItem;
    phone_header.index_size = sizeofIndex;
    phone_header.valid_count = 0;

    for(i = 0; i < PHONE_BOOK_LENGTH/4096; i++)
    {
        scStatus = hal_SysEnterCriticalSection();
        DSM_FlashErase((UINT8*)((UINT32)phone_book_data_start)+4096*i, (UINT8*)((UINT32)phone_book_data_start)+4096*(i+1));
        hal_SysExitCriticalSection(scStatus);
    }

}

VOID *DSM_GetPhoneBookAddress(void)
{
    return phone_book_data_start;
}

VOID DSM_GetPhoneBookParam(UINT32 *length, UINT16 *size, UINT16 *valid_count)
{
    memcpy(&phone_header, phone_book_data_start, sizeof(phone_header));

    if(phone_header.flag == PHONE_BOOK_FLAG)
    {
        if(length)
            *length = PHONE_BOOK_LENGTH-sizeof(phone_header);
        if(size)
            *size = phone_header.count;
        if(valid_count)
            *valid_count = phone_header.valid_count;
    }
    else  phone_header.count = 0;
}

int DSM_GetPhoneBookOffset(void)
{
    return sizeof(phone_header)+phone_header.count*phone_header.index_size+512;
}

void DSM_WritePhoneBook(UINT8 *data, UINT16 index)
{
    UINT32 write;

    {
        UINT32 scStatus = hal_SysEnterCriticalSection();
        DSM_FlashWrite((UINT8*)((UINT32)phone_book_data_start)
                       +sizeof(phone_header)+phone_header.count*phone_header.index_size+index*phone_header.item_size+512
                       ,phone_header.item_size,&write,data);
        hal_SysExitCriticalSection(scStatus);
    }
}

void DSM_WritePhoneBookIndex(UINT8 *index, UINT16 size)
{
    UINT32 write;
    // write flash
    //phone_header.count = size;
    phone_header.valid_count = size;
    {
        UINT32 scStatus = hal_SysEnterCriticalSection();
        DSM_FlashWrite((UINT8*)((UINT32)phone_book_data_start),sizeof(phone_header),&write,(UINT8*)&phone_header);
        DSM_FlashWrite((UINT8*)((UINT32)phone_book_data_start)+sizeof(phone_header),size*phone_header.index_size,&write,index);
//  CSW_TRACE(BASE_FFS_TS_ID, "write size :%x", write);
        hal_SysExitCriticalSection(scStatus);
    }
//  while(1);
}

VOID DSM_SemInit(VOID)
{
    g_hDevSem = COS_CreateSemaphore(1);

    if((HANDLE)DEV_INVALID_SEM_HANDLE == g_hDevSem)
    {
        D( ( DL_WARNING, "VDS_ModuleSemInit failed."));
    }
}

VOID DSM_SemFree(VOID)
{
    if((HANDLE)DEV_INVALID_SEM_HANDLE != g_hDevSem)
    {
        COS_DeleteSemaphore(g_hDevSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleSemFree failed."));
    }
}


VOID DSM_SemDown(VOID)
{
    if((HANDLE)DEV_INVALID_SEM_HANDLE == g_hDevSem)
    {
        // Init the semaphore
        DSM_SemInit();
    }

    if((HANDLE)DEV_INVALID_SEM_HANDLE != g_hDevSem)
    {
        COS_WaitForSemaphore(g_hDevSem, 0XFFFFFFFF);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleDown failed."));
    }

}


VOID DSM_SemUp(VOID)
{

    if((HANDLE)DEV_INVALID_SEM_HANDLE != g_hDevSem)
    {
        COS_ReleaseSemaphore(g_hDevSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleUp failed."));
    }
}

INT32 DSM_DeactiveDevice(PCSTR pszDevName)
{
    UINT32 uDevNo = 0;
    uDevNo = DSM_DevName2DevNo(pszDevName);
    if(DSM_IsRemoveableDev(uDevNo))
    {
        DRV_DEV_DEACTIVE(uDevNo);
    }
    return ERR_SUCCESS;
}


