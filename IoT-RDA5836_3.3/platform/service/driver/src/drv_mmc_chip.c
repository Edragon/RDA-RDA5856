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
#include "dsm_cf.h"
#include "mcd_m.h"
#include "drv_mmc.h"
#else
#include <cswtype.h>
#include <string.h>
#include <errorcode.h>
#include <drv_mmc.h>
#include "ts.h"
#include "cos.h"
#include "sxr_tls.h"
#include "hal_debug.h"
#include "hal_host.h"
#include "sxr_tim.h"
#include "chip_id.h"
#include "ts.h"
#include "csw_mem_prv.h"
#include "pmd_m.h"
#include "mcd_m.h"
#endif

#ifdef _FS_SIMULATOR_
#define DRV_MMC_ASSERT DSM_Assert
#else
#define DRV_MMC_ASSERT(BOOL, format, ...)                   \
{if (!(BOOL)) {                                                            \
    hal_DbgAssert(format, ##__VA_ARGS__);           \
}}

#endif
// Try times
#define TFLASH_TRYING_TIMES    3

//Invalid Dev Id.
#define DRV_INVALID_DEV_ID    0xffff

#ifdef DUAL_TFLASH_SUPPORT
// TFlash number.
#define DRV_TFLASH_NUMBER      2

// max cache buffer size in byte
#define TFLASH_CACHE_BUFFER_MAX_SIZE  (2*1024)

#else
// TFlash number.
#define DRV_TFLASH_NUMBER      1

// max cache buffer size in byte
#define TFLASH_CACHE_BUFFER_MAX_SIZE  (2*1024)
#endif

// Invalid block number.
#define TFLASH_INVALID_BLOCK_NUMBER 0xffffffff

// Device name none. 
#define TFLASH_DEVICE_NAME_NONE        "NONE"

// cache sign define
// dirty
#define DRV_CACHE_DIRTY 1
// clean
#define DRV_CACHE_CLEAN 0


// mmc cache structure.
typedef struct _drv_cache_info
{     
     UINT32 uSign;             //sign for dirty or clean.
     UINT64 uFstBlkNo;     // the first block number of cache.
     UINT32 uSize;             // cache buffer size.
     UINT16 uNum;              // cache used sectors num
     UINT8* pBuff;             // cache buffer.
}DRV_CACHE_INFO;

// t-flash device description structure.
typedef struct _drv_mmc
{
    // UINT16                         dev_id;                                                    // device id, 0,1...
    UINT16                          is_open;                                                  // device status,0:deactive,1,active.
    UINT8                           dev_name[DRV_DEV_NAME_SIZE];           // device name.
    MCD_CSD_T                 mcd_csd;                                                 // MCD_CSD.
    DRV_CACHE_INFO        r_cache;                                                  // r_cache structure.
    DRV_CACHE_INFO        w_cache;                                                 // w_cache structure.
}DRV_TFLASH;

// System statues.
// The MMI shall call the function DRV_SetStatusSleepIn(TRUE) to declare that the LCD is closed when LCD close,
// the g_bSleepIn change to TRUE.
// The MMI shall call the function DRV_SetStatusSleepIn(FALSE) to declare that the LCD is opened when LCD open,
// the g_bSleepIn change to FALSE.
PRIVATE BOOL                  g_bSleepIn = FALSE;

// TFlash device discription chain.
PRIVATE DRV_TFLASH      g_pTflashDev[] = {{0,{0},{0,},{0,},{0,},},{0,{0},{0,},{0,},{0,},}}; 

PRIVATE UINT32 g_hMmcSem = DEV_INVALID_SEM_HANDLE; // __attribute__((section(".bootbss")))

PRIVATE UINT32 g_cacheBuff[MMC_DEF_SEC_SIZE*3/2];

#ifdef _FS_SIMULATOR_

#define CSW_Malloc malloc
#define CSW_Free free

#else
#define CSW_Malloc COS_Malloc
#define CSW_Free COS_Free

#ifndef strnicmp
extern INT16  SUL_StrNCaselessCompare (CONST TCHAR* stringa,CONST TCHAR* stringb, UINT32  count);
#define strnicmp SUL_StrNCaselessCompare
#endif
extern u8 sxr_NewSemaphore (u8 InitValue);
extern void sxr_TakeSemaphore (u8 Id);
extern void sxr_ReleaseSemaphore (u8 Id);
#endif



PRIVATE VOID drv_WaitSemTFlash(VOID)
{    
    if (DEV_INVALID_SEM_HANDLE == g_hMmcSem)
    {
    #if OS_KERNEL==SXR
        g_hMmcSem = sxr_NewSemaphore(1); 
    #else
        g_hMmcSem = COS_CreateSemaphore(1); 
    #endif
    }
    if(g_hMmcSem != DEV_INVALID_SEM_HANDLE)
    {
     #if OS_KERNEL==SXR
        sxr_TakeSemaphore(g_hMmcSem); 
      #else
         COS_WaitForSemaphore(g_hMmcSem,1000);
      #endif
    }
  
}


PRIVATE VOID drv_ReleaseSemTFlash(VOID)
{
    if(g_hMmcSem != DEV_INVALID_SEM_HANDLE)
    {
    #if OS_KERNEL==SXR
        sxr_ReleaseSemaphore(g_hMmcSem);
    #else
        COS_ReleaseSemaphore(g_hMmcSem);
    #endif
    }
     
}

PRIVATE VOID drv_GetCardSize(UINT64* pblk_nr,UINT32* pblk_sz)
{
     MCD_CARD_SIZE_T  card_size;
     
     mcd_GetCardSize(&card_size);
    *pblk_nr = (UINT64)(card_size.nbBlock*(card_size.blockLen/MMC_DEF_SEC_SIZE));
    *pblk_sz = MMC_DEF_SEC_SIZE;
     return;
}
    
  

//-----------------------------------------------------------------------------------------
// drv_OpenDeviceTFlash
// Open the tflash device.
// @dev_handle[in]
//   Device handle.
// Return 
//   Upon successful completion, return ERR_SUCCESS,
//   If have not find the tflash device,shall return the err code ERR_DRV_DEVICE_NOT_FOUND.
//   If have not initialize tflash device before using other this function, shall return err code ERR_DRV_INVALID_INITIALIZATION.
//   If The system cannot perform the mmc device at specified time,  shall return err code ERR_BUSY_DRIVE.
//-----------------------------------------------------------------------------------------
PRIVATE INT32 drv_OpenDeviceTFlash(HANDLE dev_handle)
{  
    DRV_TFLASH* tflash_dev = NULL;
    MCD_ERR_T result;
    UINT8 try_times = 0;
    BOOL rest_volt = FALSE;
    MCD_CARD_VER mcdVer = MCD_CARD_V2;
    tflash_dev = (DRV_TFLASH*)dev_handle;
    BOOL vMMC = FALSE;

    if(1 == tflash_dev->is_open)
    {
#ifdef DUAL_TFLASH_SUPPORT    
        if((HANDLE)g_pTflashDev == dev_handle)
        {
            result = mcd_Close(MCD_CARD_ID_0);
        }
        else
        {
            result = mcd_Close(MCD_CARD_ID_1);
        }         
#else
       result = mcd_Close(); // sxr_Sleep(1800);
#endif  
        tflash_dev->is_open = 0;
    }
    
    if(0 == tflash_dev->is_open)
    {
#ifdef OLD_SDMMC_VMMC1_SUPPORT
VMMC_TRUE:
#endif
        do
        {
            /*
             * try_times: 0,1: V2, 2,3: V1, 4: V2+rst, 5: V1+rst
             */
            if(2 == try_times || 5 == try_times) 
            {
                mcdVer = MCD_CARD_V1;
            }
            if(4 == try_times)
            {
                mcdVer = MCD_CARD_V2;
                rest_volt = TRUE;
            }

#ifndef MCD_SPI_SUPPORT 
#ifdef DUAL_TFLASH_SUPPORT
            if((HANDLE)g_pTflashDev == dev_handle)
            {
                result = mcd_Open(MCD_CARD_ID_0,&(tflash_dev->mcd_csd),mcdVer,rest_volt);
            }
            else
            {
                result = mcd_Open(MCD_CARD_ID_1,&(tflash_dev->mcd_csd),mcdVer,rest_volt);
            }
#else
            result = mcd_Open(&(tflash_dev->mcd_csd),mcdVer,rest_volt, vMMC);
#endif
#else
           result = mcd_Open(&(tflash_dev->mcd_csd));
#endif
 
            hal_HstSendEvent(SYS_EVENT,0xbb0000aa);
            hal_HstSendEvent(SYS_EVENT,try_times + (mcdVer<<8) + (rest_volt<<16));
            hal_HstSendEvent(SYS_EVENT,result);

            if(result == MCD_ERR_NO)
            {
                break;
            }
            // sxr_Sleep(10);
            try_times++;
        }while((result == MCD_ERR ||
                result == MCD_ERR_UNUSABLE_CARD ||
                result == MCD_ERR_CARD_TIMEOUT ||
                result == MCD_ERR_DMA_BUSY ||
                result == MCD_ERR_SPI_BUSY) &&
               (try_times < TFLASH_TRYING_TIMES));
#ifdef OLD_SDMMC_VMMC1_SUPPORT
		if((result == MCD_ERR ||
                result == MCD_ERR_UNUSABLE_CARD ||
                result == MCD_ERR_CARD_TIMEOUT ||
                result == MCD_ERR_DMA_BUSY ||
                result == MCD_ERR_SPI_BUSY) &&(try_times == TFLASH_TRYING_TIMES))
		{
			if(!vMMC)
			{
				vMMC = TRUE;
				try_times = 0;
				result = MCD_ERR_NO;
				goto VMMC_TRUE;
			}
			
		}
#endif        
        if(result != MCD_ERR_NO)
        {
            CSW_TRACE(BASE_FFS_TS_ID, " mcd Open failed.Trying times:%d,error code:%d",try_times,result);

            if(result == MCD_ERR_CARD_TIMEOUT ||
            result == MCD_ERR_CARD_TIMEOUT ||
            result == MCD_ERR_DMA_BUSY     ||
            result == MCD_ERR_SPI_BUSY)
            {
                return ERR_TFLASH_DEV_BUSY;
            }
            else
            {
                return ERR_DRV_DEVICE_NOT_FOUND;
            }
        }
        else
        {
            CSW_TRACE(BASE_FFS_TS_ID, " mcd Open ok.Trying times:%d,dev_handle = 0x%x.",try_times,dev_handle);
        }
        tflash_dev->is_open = 1;  

    }  
    return ERR_SUCCESS;    
}

//-----------------------------------------------------------------------------------------
// drv_CloseDeviceTFlash
// 	Close the tflash device.
// @dev_handle[in]
//   Device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
// 	If have not find the tflash  device, shall return the err code ERR_DRV_DEVICE_NOT_FOUND.
//	If have not initialize tflash device  before using  this function, shall return err code ERR_DRV_INVALID_INITIALIZATION.
// 	If The system cannot close tflash device at specified time,s shall return err code ERR_BUSY_DRIVE.
//-----------------------------------------------------------------------------------------	
PRIVATE INT32 drv_CloseDeviceTFlash(HANDLE dev_handle)
{      
    DRV_TFLASH* tflash_dev = NULL;
    MCD_ERR_T result;
    //MCD_ERR_T result1;
    UINT8 try_times = 0;
    tflash_dev = (DRV_TFLASH*)dev_handle;

    if(tflash_dev->is_open == 1)
    {       
        do
        {
            try_times++;
#ifdef DUAL_TFLASH_SUPPORT        
            // CSW_TRACE(BASE_FFS_TS_ID, "drv_CloseDeviceTFlash:begin.dev_handle= 0x%x.",dev_handle);
            if((HANDLE)g_pTflashDev == dev_handle)
            {
                result = mcd_Close(MCD_CARD_ID_0);
            }
            else
            {
                result = mcd_Close(MCD_CARD_ID_1);
            }              
            CSW_TRACE(BASE_FFS_TS_ID, "drv_CloseDeviceTFlash:dev_handle= 0x%x, close tflash result = %d.",dev_handle,result);
            
#else
            result = mcd_Close();
#endif

            if(result == MCD_ERR_NO)
            {
                break;
            }
            else
            {
                CSW_TRACE(BASE_FFS_TS_ID, "DRV_CloseMMC:try to mcd_Close() failed.Trying times:%d,dev_handle = 0x%x,error code:%d.",
                    try_times,dev_handle,result);
            }
            //sxr_Sleep(10);            
        }while ((result == MCD_ERR_CARD_TIMEOUT ||
             result == MCD_ERR_CARD_TIMEOUT ||
             result == MCD_ERR_DMA_BUSY     ||
             result == MCD_ERR_SPI_BUSY)&&
             (try_times<TFLASH_TRYING_TIMES));

        if (result != MCD_ERR_NO)
        {
            CSW_TRACE(BASE_FFS_TS_ID, " mcd_Close() failed.Trying times:%d,dev_handle = 0x%x,error code:%d.",try_times,dev_handle,result);
            if(result == MCD_ERR_CARD_TIMEOUT ||
             result == MCD_ERR_CARD_TIMEOUT ||
             result == MCD_ERR_DMA_BUSY     ||
             result == MCD_ERR_SPI_BUSY)
            {                
                return ERR_TFLASH_DEV_BUSY;
            }
            else
            {
                return ERR_DRV_DEVICE_NOT_FOUND;
            }
        }
        tflash_dev->is_open = 0;
    }
#ifdef DUAL_TFLASH_SUPPORT      
    if(0 == g_pTflashDev[0].is_open &&
        0 == g_pTflashDev[1].is_open)
        {
            // padding for community power.
            mcd_LowPower();
        }   
#endif		
    return ERR_SUCCESS;
}


//-----------------------------------------------------------------------------------------
// drv_FlushCacheTFlash
// 	Flush the cache buffer.
// @dev_handle[in]
//   Device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//    else return error code.
//-----------------------------------------------------------------------------------------	
PRIVATE INT32 drv_FlushCacheTFlash(HANDLE dev_handle)
{
    DRV_TFLASH* tflash_dev = NULL;
    INT32 ret;
    MCD_ERR_T result;
    UINT32 try_times = 0;
    UINT32 size = 0;
    UINT64 blk_nr = 0;
    UINT64 all_blk_nr = 0;
    UINT32 sec_sz = 0;

    tflash_dev = (DRV_TFLASH*)dev_handle;  
    
    // flush old region
    if(NULL != tflash_dev->w_cache.pBuff                          &&
        DRV_CACHE_DIRTY == tflash_dev->w_cache.uSign   &&
        TFLASH_INVALID_BLOCK_NUMBER != tflash_dev->w_cache.uFstBlkNo)
    {        
#ifdef DUAL_TFLASH_SUPPORT
        if((HANDLE)g_pTflashDev == dev_handle)
        {
            mcd_SwitchOperationTo(MCD_CARD_ID_0);
        }
        else
        {
            mcd_SwitchOperationTo(MCD_CARD_ID_1);
        }
#endif
        do
        {           
            blk_nr = tflash_dev->w_cache.uFstBlkNo;
            drv_GetCardSize(&all_blk_nr,&sec_sz);  
            if(blk_nr + (UINT64)(tflash_dev->w_cache.uSize/sec_sz) < all_blk_nr)
            {
                size =  tflash_dev->w_cache.uSize;
            }
            else if(blk_nr < all_blk_nr)
            {
                size = (UINT32)(all_blk_nr-blk_nr)*sec_sz;
            }
            else
            {
                size = 0;
                ret = ERR_TFLASH_DEV_BLK_OVERFLOW;
                break;
            }
            result = mcd_Write((UINT32)blk_nr, tflash_dev->w_cache.pBuff,size);
            if (result != MCD_ERR_NO)
            {
                
                try_times ++; 
                CSW_TRACE(BASE_FFS_TS_ID, "DRV_FlushTFlash:call mcd_Write(0x%x) returns error(%d)!try times = %d.",tflash_dev->w_cache.uFstBlkNo, result,try_times);
                if(try_times == TFLASH_TRYING_TIMES)
                {
                    ret = ERR_TFLASH_WRITE_FAILED;
                    goto _func_end;
                }                
                drv_CloseDeviceTFlash(dev_handle);
                ret=drv_OpenDeviceTFlash(dev_handle);
                if (ret != ERR_SUCCESS)
                {
                    goto _func_end;          
                }             
            }    
            else
            {   
                // CSW_TRACE(BASE_FFS_TS_ID, "DRV_FlushTFlash:called mcd_Write() ok. blk_nr = 0x%x,write_sz = 0x%x.",g_WCacheInfo.uFstBlkNo,g_WCacheInfo.uSize);         
                break;
            }     
        }while(try_times < TFLASH_TRYING_TIMES);     
    }
    // g_WCacheInfo.uFstBlkNo= TFLASH_INVALID_BLOCK_NUMBER;    
    tflash_dev->w_cache.uSign = DRV_CACHE_CLEAN;
    tflash_dev->w_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
    tflash_dev->r_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
    return ERR_SUCCESS;	
    
_func_end:
    
    return ret;
        
}


//-----------------------------------------------------------------------------------------
// DRV_OpenTFlash
// 	Open the device.
// @dev_name[in]
//  TFlash device name.
// @pdev_handle[out]
//   Output device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//    else return error code.
//-----------------------------------------------------------------------------------------	
INT32 DRV_OpenTFlash(UINT8* dev_name,HANDLE* pdev_handle)
{
    DRV_TFLASH* pTDev =NULL;
    UINT16 i;
    UINT16 dev_id;
    
    // drv_InitSemTFlash();
    CSW_TRACE(BASE_FFS_TS_ID, "DRV_OpenTFlash:dev_name = %s,pdev_handle = 0x%x.",dev_name,pdev_handle);
    drv_WaitSemTFlash();
    for(i = 0; i < DRV_TFLASH_NUMBER; i++)
    {
        if(strlen(g_pTflashDev[i].dev_name) == 0)
            strcpy(g_pTflashDev[i].dev_name, TFLASH_DEVICE_NAME_NONE);
        if(strnicmp((UINT8*)g_pTflashDev[i].dev_name,dev_name,strlen(dev_name)) == 0)
        {
            pTDev = &g_pTflashDev[i];
            *pdev_handle = (HANDLE)pTDev;  
            drv_ReleaseSemTFlash();
            return  ERR_SUCCESS;
        }
               
    }
     
    dev_id = DRV_TFLASH_NUMBER;
    for(i = 0; i < DRV_TFLASH_NUMBER; i++)
    {        
        if(strnicmp((UINT8*)g_pTflashDev[i].dev_name,TFLASH_DEVICE_NAME_NONE,strlen(TFLASH_DEVICE_NAME_NONE)) == 0)
        {
            pTDev = &g_pTflashDev[i];           
            break;
        }            
    }

    if(NULL == pTDev)
    {
        drv_ReleaseSemTFlash();
        return  ERR_DRV_DEVICE_NOT_FOUND;        
    }  
    /*if(g_cacheBuff == NULL)
    {
        g_cacheBuff = COS_Malloc_NoCache(MMC_DEF_SEC_SIZE*8);
    }*/
   
    memset(pTDev,0,SIZEOF(DRV_TFLASH));
    strcpy(pTDev->dev_name,dev_name);
    pTDev->is_open = 0;
    pTDev->r_cache.uSign = 0;
    pTDev->r_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
    pTDev->r_cache.pBuff = (UINT8*)g_cacheBuff;
    pTDev->r_cache.uSize = MMC_DEF_SEC_SIZE*4;

    pTDev->w_cache.uSign = 0;
    pTDev->w_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
    pTDev->w_cache.pBuff = (UINT8*)(g_cacheBuff+MMC_DEF_SEC_SIZE);
    pTDev->w_cache.uSize = MMC_DEF_SEC_SIZE*2;//sizeof(g_mmc_write_cache);
    pTDev->w_cache.uNum = 0;
#ifdef _FS_SIMULATOR_            
    strcpy(pTDev->mcd_csd.img_name,dev_name);   
    pTDev->mcd_csd.fd = 0;
    pTDev->mcd_csd.block_number = 0;
    pTDev->mcd_csd.block_size = 0;
    pTDev->mcd_csd.is_open = 0;     
#endif         

    *pdev_handle = (HANDLE)pTDev;
    drv_ReleaseSemTFlash();
    return ERR_SUCCESS;
}


//-----------------------------------------------------------------------------------------
// DRV_CloseTFlash
// 	Close the device.
// @pdev_handle[out]
//   Device handle.
// Return 
// 	Upon successful completion, return ERR_SUCCESS,
//    else return error code.
//-----------------------------------------------------------------------------------------	
INT32 DRV_CloseTFlash(HANDLE dev_handle)
{    
    DRV_TFLASH* pTDev =NULL;
    UINT32 i;
    INT32 result;

    CSW_TRACE(BASE_FFS_TS_ID, "DRV_CloseTFlash:pdev_handle = 0x%x.",dev_handle);
    drv_WaitSemTFlash();
    for(i = 0; i < DRV_TFLASH_NUMBER; i++)
    {
        pTDev = &g_pTflashDev[i];
        if((HANDLE)pTDev == dev_handle)
        {            
            result = drv_CloseDeviceTFlash(dev_handle);
            if(ERR_SUCCESS == result)
            {             
           
            }
            else
            {
                // donoting.
            }            
            drv_ReleaseSemTFlash();
            return result;
        }             
    }         
    drv_ReleaseSemTFlash();
    return ERR_TFLASH_DEV_INVALID_HANDLE;    
}


//-----------------------------------------------------------------------------------------
// DRV_FlushTFlash
//	 Flush the cache.
// @pdev_handle[out]
//   Device handle.
// Return 
//	 Upon successful completion, return ERR_SUCCESS.
//	 If flush failed, shall return err code.
//-----------------------------------------------------------------------------------------
INT32 DRV_FlushTFlash(HANDLE dev_handle)
{
    DRV_TFLASH* tflash_dev = NULL;
    INT32 ret;

    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;

    if(tflash_dev->is_open == 0)
    {
        ret=drv_OpenDeviceTFlash(dev_handle);
        if (ret != ERR_SUCCESS)
        {
            drv_ReleaseSemTFlash();
            return ret;          
        }
     }  
     
     ret = drv_FlushCacheTFlash(dev_handle);
     drv_ReleaseSemTFlash();
     return ret;
}



//-----------------------------------------------------------------------------------------
// DRV_ReadTFlash
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

INT32 DRV_ReadTFlash(HANDLE dev_handle, UINT64 blk_nr, BYTE* pBuffer)
{    
    INT32 ret = 0;    
    UINT32 try_times = 0;
    MCD_ERR_T result;
    DRV_TFLASH* tflash_dev = NULL;
    UINT32 size = 0;
    UINT64 all_blk_nr = 0;
    UINT32 sec_sz = 0;
    UINT64 local_blk;
    // UINT32 tst_begin_time;
    // UINT32 tst_read_time;

    // tst_begin_time = hal_TimGetUpTime();
    local_blk = blk_nr;
    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;
    // CSW_TRACE(BASE_FFS_TS_ID, "DRV_ReadTFlash:pdev_handle = 0x%x,blk_nr = 0x%x.",dev_handle,blk_nr);
    if(tflash_dev->is_open == 0)
    {
        ret=drv_OpenDeviceTFlash(dev_handle);
        if (ret != ERR_SUCCESS)
        {
            CSW_TRACE(BASE_FFS_TS_ID, "DRV_ReadTFlash:open tflash failed. dev_handle = 0x%x,ret = %d.",dev_handle,ret);
            goto func_end;         
        }
    }  

    // try to read the block from w_cache buffer.if hit the target,goto end.
    if(NULL != tflash_dev->w_cache.pBuff            &&
       local_blk >= tflash_dev->w_cache.uFstBlkNo  && 
       local_blk <   tflash_dev->w_cache.uFstBlkNo + tflash_dev->w_cache.uSize/MMC_DEF_SEC_SIZE 
      )
    {           
        memcpy(pBuffer,tflash_dev->w_cache.pBuff + (UINT32)(local_blk - tflash_dev->w_cache.uFstBlkNo)* MMC_DEF_SEC_SIZE,MMC_DEF_SEC_SIZE);
        // CSW_TRACE(BASE_FFS_TS_ID, "cache:DRV_ReadTFlash(0x%x) read form w_cache, fst_blk_no = 0x%x.\n",blk_nr,tflash_dev->w_cache.uFstBlkNo);
        goto func_end;            
    }
   
    
    // try to read the block from r_cache buffer. if hit the target,goto end.
    if(NULL != tflash_dev->r_cache.pBuff            &&
       local_blk >= tflash_dev->r_cache.uFstBlkNo  && 
       local_blk <   tflash_dev->r_cache.uFstBlkNo + tflash_dev->r_cache.uSize/MMC_DEF_SEC_SIZE
      )
    {
        memcpy(pBuffer,tflash_dev->r_cache.pBuff  + (UINT32)(local_blk - tflash_dev->r_cache.uFstBlkNo)* MMC_DEF_SEC_SIZE,MMC_DEF_SEC_SIZE); 
        // CSW_TRACE(BASE_FFS_TS_ID, "cache:DRV_ReadTFlash(0x%x) read form r_cache, fst_blk_no = %d.\n",blk_nr,tflash_dev->r_cache.uFstBlkNo);
        // DSM_HstSendEvent(0xaabb6604);
        goto func_end;
    }

    // read from t-flash to cache.
    // if read failed ,try to do it times without number.
#ifdef DUAL_TFLASH_SUPPORT
    if((HANDLE)g_pTflashDev == dev_handle)
    {
        mcd_SwitchOperationTo(MCD_CARD_ID_0);
    }
    else
    {
        mcd_SwitchOperationTo(MCD_CARD_ID_1);
    }
#endif
    do
    { 
        drv_GetCardSize(&all_blk_nr,&sec_sz);  
        if(local_blk + (tflash_dev->r_cache.uSize/sec_sz) < all_blk_nr)
        {
            size =  tflash_dev->r_cache.uSize;
        }
        else if(local_blk < all_blk_nr)
        {
            size = (UINT32)(all_blk_nr - local_blk)*sec_sz;
        }
        else
        {
            size = 0;
            ret = ERR_TFLASH_DEV_BLK_OVERFLOW;
            break;
        }
        
        result = mcd_Read(local_blk, tflash_dev->r_cache.pBuff, size);
        if (result != MCD_ERR_NO )
        {            
            try_times ++;
            if(try_times == TFLASH_TRYING_TIMES)
            {
                ret = result;
                break;
            }
            
            CSW_TRACE(BASE_FFS_TS_ID, "DRV_ReadMMC:call mcd_Read(0x%08x) returns error(%d)!try times = %d.",local_blk, result,try_times);

            drv_CloseDeviceTFlash(dev_handle);
            ret=drv_OpenDeviceTFlash(dev_handle);
            if (ret != ERR_SUCCESS)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "DRV_ReadMMC:open device failed!ret = %d,try times = %d.",ret, try_times);
                break;
            }             
        }    
        else
        {
            tflash_dev->r_cache.uFstBlkNo = local_blk;
            memcpy(pBuffer,tflash_dev->r_cache.pBuff,MMC_DEF_SEC_SIZE);            
            // CSW_TRACE(BASE_FFS_TS_ID, "DRV_ReadTFlash:recalled mcd_Read() ok. blk_nr = 0x%x.",blk_nr);         
            // sxr_Sleep(5);
            break;
        }     
    }while(try_times < TFLASH_TRYING_TIMES);
 func_end:
    // tst_read_time = hal_TimGetUpTime() - tst_begin_time; 
    // CSW_TRACE(BASE_DSM_TS_ID, "drv read blk_nr = 0x%x,time = 0x%x.",blk_nr,tst_read_time);
    drv_ReleaseSemTFlash();
    return ret;
}


//-----------------------------------------------------------------------------------------
// DRV_WriteTFlash
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
INT32 DRV_WriteTFlash (HANDLE dev_handle, UINT64 blk_nr, CONST BYTE* pBuffer)
{
    INT32 ret;
    MCD_ERR_T result;
    UINT32 try_times = 0;
    DRV_TFLASH* tflash_dev = NULL;
    UINT32 size = 0;
    UINT64 all_blk_nr = 0;
    UINT32 sec_sz = 0;
    UINT64 local_blk;

    local_blk = blk_nr;
    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;

    if(tflash_dev->is_open == 0)
    {
        ret=drv_OpenDeviceTFlash(dev_handle);
        if (ret != ERR_SUCCESS)
        {
            goto _func_end;        
        }
    }  

   // write to r_cache if the blk in r_cache.
   if(NULL != tflash_dev->r_cache.pBuff            &&
      local_blk >= tflash_dev->r_cache.uFstBlkNo  && 
      local_blk < tflash_dev->r_cache.uFstBlkNo + tflash_dev->r_cache.uSize/MMC_DEF_SEC_SIZE
     )   
   {
       memcpy(tflash_dev->r_cache.pBuff + (UINT32)(local_blk -tflash_dev->r_cache.uFstBlkNo) * MMC_DEF_SEC_SIZE,pBuffer,MMC_DEF_SEC_SIZE);
       // CSW_TRACE(BASE_FFS_TS_ID, "cache: write to r_cache buffer,blk_nr = %d.\n",blk_nr);
   }

   // write to w_cache if the blk in w_cache.
   if(NULL !=  tflash_dev->w_cache.pBuff           &&
      local_blk >= tflash_dev->w_cache.uFstBlkNo  && 
      local_blk < tflash_dev->w_cache.uFstBlkNo + tflash_dev->w_cache.uSize/MMC_DEF_SEC_SIZE)
   {
       memcpy(tflash_dev->w_cache.pBuff + (UINT32)(local_blk - tflash_dev->w_cache.uFstBlkNo) * MMC_DEF_SEC_SIZE,pBuffer,MMC_DEF_SEC_SIZE);
       tflash_dev->w_cache.uSign = DRV_CACHE_DIRTY;
       // CSW_TRACE(BASE_FFS_TS_ID, "cache: write to w_cache buffer,blk_nr = %d.\n",blk_nr);       
   }      
   else    
   {   
#ifdef DUAL_TFLASH_SUPPORT
        if((HANDLE)g_pTflashDev == dev_handle)
        {
            mcd_SwitchOperationTo(MCD_CARD_ID_0);
        }
        else
        {
            mcd_SwitchOperationTo(MCD_CARD_ID_1);
        }
#endif
        // flush old region
        if(NULL != tflash_dev->w_cache.pBuff                         &&
            DRV_CACHE_DIRTY == tflash_dev->w_cache.uSign &&
            TFLASH_INVALID_BLOCK_NUMBER != tflash_dev->w_cache.uFstBlkNo)
        {        
            do
            { 
                drv_GetCardSize(&all_blk_nr,&sec_sz);
                if(local_blk + (UINT64)(tflash_dev->w_cache.uSize/sec_sz) < all_blk_nr)
                {
                    size =  tflash_dev->w_cache.uSize;
                }
                else if(local_blk < all_blk_nr)
                {
                    size = (UINT32)(all_blk_nr-local_blk)*sec_sz;
                }
                else
                {
                    size = 0;
                    ret = ERR_TFLASH_DEV_BLK_OVERFLOW;
                    break;
                }
                result = mcd_Write(tflash_dev->w_cache.uFstBlkNo, tflash_dev->w_cache.pBuff,size);
                if (result != MCD_ERR_NO)
                {
                    try_times ++; 
                    CSW_TRACE(BASE_FFS_TS_ID, "DRV_WriteTFlash:call mcd_Write(0x%x) returns error(0x%x)!try times = %d.",tflash_dev->w_cache.uFstBlkNo, result,try_times);
                    if(try_times == TFLASH_TRYING_TIMES)
                    {
                        ret = result;
                        goto _func_end;
                    }

                    drv_CloseDeviceTFlash(dev_handle);
                    ret=drv_OpenDeviceTFlash(dev_handle);
                    if (ret != ERR_SUCCESS)
                    {
                        goto _func_end;          
                    }             
                }    
                else
                {   
                    tflash_dev->w_cache.uSign = DRV_CACHE_CLEAN;
                    // CSW_TRACE(BASE_FFS_TS_ID, "DRV_WriteMMC:called mcd_Write() ok. blk_nr = 0x%x,write_sz = 0x%x.",tflash_dev->w_cache.uFstBlkNo,tflash_dev->w_cache.uSize);         
                    break;
                }     
            }while(try_times < TFLASH_TRYING_TIMES);     
        }
        // cache the new region 
        do
        {
            drv_GetCardSize(&all_blk_nr,&sec_sz);
            if(local_blk + (UINT64)(tflash_dev->w_cache.uSize/sec_sz) < all_blk_nr)
            {
                size =  tflash_dev->w_cache.uSize;
            }
            else if(local_blk < all_blk_nr)
            {
                size = (UINT32)(all_blk_nr-local_blk)*sec_sz;
            }
            else
            {
                size = 0;
                ret = ERR_TFLASH_DEV_BLK_OVERFLOW;
                break;
            }
            result = mcd_Read(local_blk, tflash_dev->w_cache.pBuff, size);
            if (result != MCD_ERR_NO)
            {
                try_times ++;
                CSW_TRACE(BASE_FFS_TS_ID, "DRV_WriteTFlash:call mcd_Read(0x%x) returns error(%d)!try times = %d.",local_blk, result,try_times);
                if(try_times == TFLASH_TRYING_TIMES)
                {
                    ret = result;
                    goto _func_end;
                }
                drv_CloseDeviceTFlash(dev_handle);
                result = drv_OpenDeviceTFlash(dev_handle);
                if (result != ERR_SUCCESS)
                {
                     ret = result;          
                }             
            }    
            else
            {
                // CSW_TRACE(BASE_FFS_TS_ID, "DRV_WriteTFlash:called mcd_Read() ok. blk_nr = 0x%x,read_sz = 0x%x.",blk_nr,g_WCacheInfo.uSize);
                tflash_dev->w_cache.uFstBlkNo = local_blk;
                // update the specified block.
                memcpy(tflash_dev->w_cache.pBuff + (UINT32)(local_blk -tflash_dev->w_cache.uFstBlkNo) * MMC_DEF_SEC_SIZE,pBuffer,MMC_DEF_SEC_SIZE);
                tflash_dev->w_cache.uSign = DRV_CACHE_DIRTY; 
                break;
            }     
        }while(try_times < TFLASH_TRYING_TIMES);     
        
    }       

   ret = ERR_SUCCESS;

_func_end:
    drv_ReleaseSemTFlash();
    return ret;
}

//-----------------------------------------------------------------------------------------
// DRV_GetDevInfoTFlash
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
INT32 DRV_GetDevInfoTFlash (HANDLE dev_handle, UINT64 *pBlockNr, UINT32 *pBlockSize)
{
    INT32 ret;
    DRV_TFLASH* tflash_dev = NULL;

    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;

    if(tflash_dev->is_open == 0)        
    {
        ret = drv_OpenDeviceTFlash(dev_handle);
        if (ret != ERR_SUCCESS)
        {
            drv_ReleaseSemTFlash();
            return ERR_DRV_DEVICE_NOT_FOUND;
        }
    }
    
#ifdef DUAL_TFLASH_SUPPORT
     if((HANDLE)g_pTflashDev == dev_handle)
    {
        mcd_SwitchOperationTo(MCD_CARD_ID_0);
    }
    else
    {
        mcd_SwitchOperationTo(MCD_CARD_ID_1);
    }
#endif   
    drv_GetCardSize(pBlockNr,pBlockSize);    
    CSW_TRACE(BASE_FFS_TS_ID, "DRV_GetDevInfoTFlash:handle = %d,block size = 0x%x, block number = 0x%x.",tflash_dev,*pBlockSize,*pBlockNr);
    drv_CloseDeviceTFlash(dev_handle);
    drv_ReleaseSemTFlash();
    return ERR_SUCCESS;
}

BOOL g_sd_sleepIn = FALSE;
//-----------------------------------------------------------------------------------------
//DRV_DeactiveTFlash
//   Deactive the device.
// @dev_handle[in]
// 	Device handle.
//Return
//	VOID
//-----------------------------------------------------------------------------------------
VOID DRV_DeactiveTFlash(HANDLE dev_handle)
{      
    INT32 ret;   
    DRV_TFLASH* tflash_dev = NULL;
    
    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;

    if(tflash_dev->is_open == 0)        
    {
        drv_ReleaseSemTFlash();
        return;
    }
    
    ret = drv_FlushCacheTFlash(dev_handle);
    if(ERR_SUCCESS != ret)
    {
        CSW_TRACE(BASE_FFS_TS_ID, " cache:FS_Close flush cache error.  return error = 0x%x.", ret);            
    }
    else
    {
       // CSW_TRACE(BASE_FFS_TS_ID, " cache:FS_Close flush cache ok.");             
    }    
    //if(g_bSleepIn == TRUE)
    {   
        hal_HstSendEvent(SYS_EVENT,0x12347777);
        g_sd_sleepIn = TRUE;
        drv_CloseDeviceTFlash(dev_handle);
        g_sd_sleepIn = FALSE;
        CSW_TRACE(BASE_FFS_TS_ID, "Close MMC device.");           
    }
    drv_ReleaseSemTFlash();
    return;
}


//-----------------------------------------------------------------------------------------
//DRV_ActiveTFlash
// Active the device.
// @dev_handle[in]
// 	Device handle.
// Return 
// See the description abount of drv_OpenDeviceTFlash().
//-----------------------------------------------------------------------------------------
INT32 DRV_ActiveTFlash(HANDLE dev_handle)
{  
    INT32 iRet;
    drv_WaitSemTFlash();
    iRet = drv_OpenDeviceTFlash(dev_handle);    
    drv_ReleaseSemTFlash();
    return iRet;
    
}


//-----------------------------------------------------------------------------------------
//DRV_SetStatusSleepIn
// Set LCD status.
// @dev_handle[in]
// 	Device handle.
// Return 
// VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetStatusSleepIn(BOOL dev_status)
{      
    g_bSleepIn = dev_status; 
}


//-----------------------------------------------------------------------------------------
//DRV_GetRCacheSizeTFlash
//Get the size of rcache buffer.
//@dev_handle[in]
// 	Device handle.
//Return
//   return size of rcache buffer.
//-----------------------------------------------------------------------------------------
UINT32 DRV_GetRCacheSizeTFlash (HANDLE dev_handle)
{
    DRV_TFLASH* tflash_dev = NULL;
    
    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;
    if(NULL != tflash_dev->r_cache.pBuff)
    {
        drv_ReleaseSemTFlash();
        return tflash_dev->r_cache.uSize;
    }
    else
    {
        drv_ReleaseSemTFlash();
        return 0;
    }
}


//-----------------------------------------------------------------------------------------
//DRV_SetRCacheSizeTFlash
//Set the size of rcache buffer.
//@dev_handle[in]
// 	Device handle.
//@cache_size[in]
//   Size of rcache buffer.
//Return Value:
//   VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetRCacheSizeTFlash(HANDLE dev_handle,UINT32 cache_size)
{
    // reset the cache size when the current cache size not equal to the input parameter.
    // malloc a buffer with (cache size ) in byte,

    UINT32 align_size = 0;
    UINT32 size_old = 0;
    DRV_TFLASH* tflash_dev = NULL;
    
    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;
    size_old = tflash_dev->r_cache.uSize;

     // align in MMC_DEF_SEC_SIZE(512) byte:     

#ifdef DUAL_TFLASH_SUPPORT
    if((HANDLE)g_pTflashDev == dev_handle)
    {
        cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
        align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE;   
    }
    else
    {

       #if USE_SPI_FOR_SD_CARD2
       align_size =  MMC_DEF_SEC_SIZE;
       #error "we have do not use spi for sd card in 8806 and 8808 "
       #else
       cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
       align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE; 
       #endif
    }
#else
    cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
    align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE;   
#endif
    // Set write cache structure.
    if(size_old != align_size)
    {
        // Set read cache structure.

        if(NULL != tflash_dev->r_cache.pBuff)
        {
            CSW_Free(tflash_dev->r_cache.pBuff);
            tflash_dev->r_cache.pBuff = NULL;
        }
        
        // try to malloc with align_size, reduce by half the size if fail.        
        do
        {            
            tflash_dev->r_cache.pBuff = CSW_Malloc(align_size);
            if(NULL != tflash_dev->r_cache.pBuff)
            {
                break;
            }
            else
            {
                DRV_MMC_ASSERT(0,"DRV_SetRCacheSizeTFlash:malloc(0x%x) failed.",align_size);
                align_size = align_size/2;
            }

        }while(align_size >= MMC_DEF_SEC_SIZE);
        
        if(NULL == tflash_dev->r_cache.pBuff )
        {                 
            CSW_TRACE(BASE_FFS_TS_ID, "Set r_cache size:malloc failed, cache size = %d byte.",align_size);
            tflash_dev->r_cache.uSize = 0;
            tflash_dev->r_cache.uSign = DRV_CACHE_CLEAN;          
        }
        else
        {             
            tflash_dev->r_cache.uSize = align_size;
            tflash_dev->r_cache.uSign = DRV_CACHE_CLEAN;          
        }
        tflash_dev->r_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
        CSW_TRACE(BASE_FFS_TS_ID, "cache:change r_cache size to 0x%x bytes,old_size = 0x%x.",align_size,size_old);   
    }
    drv_ReleaseSemTFlash();
    return;
}


//-----------------------------------------------------------------------------------------
//DRV_GetWCacheSizeTFlash
//Get the size of wcache buffer.
//@dev_handle[in]
// 	Device handle.
//Return
//   return size of wcache buffer.
//-----------------------------------------------------------------------------------------
UINT32 DRV_GetWCacheSizeTFlash(HANDLE dev_handle)
{
    DRV_TFLASH* tflash_dev = NULL;

    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;
    if(NULL != tflash_dev->w_cache.pBuff)
    {
        drv_ReleaseSemTFlash();
        return tflash_dev->w_cache.uSize;
    }
    else
    {
        drv_ReleaseSemTFlash();
        return 0;
    }
}


//-----------------------------------------------------------------------------------------
//DRV_SetWCacheSizeTFlash
//Set the size of wcache buffer.
//@dev_handle[in]
// 	Device handle.
//@cache_size[in]
//   Size of wcache buffer.
//Return Value:
//   VOID
//-----------------------------------------------------------------------------------------
VOID DRV_SetWCacheSizeTFlash(HANDLE dev_handle,UINT32 cache_size)
{

    // reset the cache size when the current cache size not equal to the input parameter.
    // malloc a buffer with (cache size ) in byte,

    UINT32 align_size = 0;
    UINT32 size_old = 0;
    DRV_TFLASH* tflash_dev = NULL;
    INT32 ret;

    drv_WaitSemTFlash();
    tflash_dev = (DRV_TFLASH*)dev_handle;
    size_old = tflash_dev->w_cache.uSize;

    // align in MMC_DEF_SEC_SIZE(512) byte:     
 #ifdef DUAL_TFLASH_SUPPORT
    if((HANDLE)g_pTflashDev == dev_handle)
    {
        cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
        align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE;   
    }
    else
    {

        #if USE_SPI_FOR_SD_CARD2
        align_size =  MMC_DEF_SEC_SIZE;
        #error "we have do not use spi for sd card in 8806 and 8808 "
        #else
        cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
        align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE;   
        #endif

    }
#else
    cache_size = cache_size > TFLASH_CACHE_BUFFER_MAX_SIZE ? TFLASH_CACHE_BUFFER_MAX_SIZE :cache_size;
    align_size = ((cache_size + (MMC_DEF_SEC_SIZE - 1))/MMC_DEF_SEC_SIZE) *MMC_DEF_SEC_SIZE;   
#endif

    // Set write cache structure.
    if(size_old != align_size)
    {
        if(NULL != tflash_dev->w_cache.pBuff)
        {
           if(tflash_dev->is_open == 0)
            {
                ret=drv_OpenDeviceTFlash(dev_handle);
                if (ret != ERR_SUCCESS)
                {
                    drv_ReleaseSemTFlash();
                    return;          
                }
            }  
            drv_FlushCacheTFlash(dev_handle);
            CSW_Free(tflash_dev->w_cache.pBuff);
            tflash_dev->w_cache.pBuff = NULL;
        }
        
        // try to malloc with align_size, reduce by half the size if fail.       
        do
        {            
            tflash_dev->w_cache.pBuff = CSW_Malloc(align_size);
            if(NULL != tflash_dev->w_cache.pBuff)
            {
                break;
            }
            else
            {
                DRV_MMC_ASSERT(0,"DRV_SetRCacheSizeTFlash:malloc(0x%x) failed.",align_size);
                align_size = align_size/2;
            }

        }while(align_size >= MMC_DEF_SEC_SIZE);        
         
        if(NULL == tflash_dev->w_cache.pBuff )
        {                 
            CSW_TRACE(BASE_FFS_TS_ID, "Set w_cache size:malloc failed, cache size = %d byte.",align_size);
            tflash_dev->w_cache.uSize = 0;
            tflash_dev->w_cache.uSign= DRV_CACHE_CLEAN;          
        }
        else
        {             
            tflash_dev->w_cache.uSize = align_size;
            tflash_dev->w_cache.uSign = DRV_CACHE_CLEAN;                       
        }
        tflash_dev->w_cache.uFstBlkNo = TFLASH_INVALID_BLOCK_NUMBER;
        CSW_TRACE(BASE_FFS_TS_ID, "cache:change the w_cache size to 0x%x bytes,old_size = 0x%x.",align_size,size_old);   
    }
    drv_ReleaseSemTFlash();
    return;
}

extern VOID pm_TFlashDetect(BOOL cardPlugged);

VOID TFcardPlugHandler(BOOL cardPlugged)
{
#ifdef _T_UPGRADE_PROGRAMMER
      cardPlugged = cardPlugged;
#else // _T_UPGRADE_PROGRAMMER
#ifndef _FS_SIMULATOR_
    pm_TFlashDetect(cardPlugged);
#endif
#endif // _T_UPGRADE_PROGRAMMER
}

void McdSetCardDetectHandler(void)
{
    drv_WaitSemTFlash();
    //mcd_SetCardDetectHandler((MCD_CARD_DETECT_HANDLER_T)TFcardPlugHandler );
    drv_ReleaseSemTFlash();
    return;
}



