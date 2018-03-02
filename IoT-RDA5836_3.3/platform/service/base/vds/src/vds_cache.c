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
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#include "dsm_cf.h"
#ifdef _FS_SIMULATOR_
#else
#include "hal_lps.h"
#include "hal_comregs.h"
#include "hal_fint.h"
#include "hal_timers.h"
#include "hal_dma.h"
#include "hal_rda_audio.h"
#include <base_prv.h>
#include <drv_flash.h>
#include "ts.h"
#include "csw_csp.h"
#include "cfw_multi_sim.h"
#endif
#include "dsm_dbg.h"
#include "vds_cd.h"
#include "vds_dbg.h"
#include "vds_local.h"
#include "vds_tools.h"
#include "vds_api.h"
#include "vds_cache.h"
#include "vdsp_debug.h"
#include "csw_profile_codes.h"

#ifdef _FS_SIMULATOR_
#define COS_FREE free
#define COS_MALLOC malloc
#define hal_SysExitCriticalSection(x) do{}while(0)
#define hal_EnterCriticalSection() 0 
#endif

#ifdef VDS_CACHE_USER_CLUSTER
#define VDS_CACHE_MALLOC COS_MALLOC
#define VDS_CACHE_FREE      COS_FREE
#else
#define VDS_CACHE_MALLOC CSW_Malloc
#define VDS_CACHE_FREE     CSW_Free

#endif
// #define VDS_CACHE_MODE_INCREMENTAL  

// extern function.
extern void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void));
extern VOID hal_BootRegisterBeforeMonCallback(VOID (*funcPtr)(VOID));
extern BOOL tgt_FlushFactorySettings(VOID);
extern PUBLIC BOOL hal_GetVocDma(void);
#ifdef __PRJ_WITH_SPIFLSH__
extern PUBLIC BOOL hal_SpiFlashIsBusy(VOID);
#endif

#if (defined(COMPRESS_3232)||defined(COMPRESS_6464))&&!defined(MMI_ON_WIN32)
extern BOOL cdfu_overlay_2_2_is_inited(void);
#endif

// local function.
VOID vds_CacheSemInit(VOID);
VOID vds_CacheSemWaitFor(VOID);
VOID vds_CacheSemRelease(VOID);
INT32 vds_CacheNodeFlush(UINT32 flush_nr);

// macro.
// cache node status.
#define VDS_CACHE_STATUS_FREE                             0x00
#define VDS_CACHE_STATUS_WRITE                          0x01
#define VDS_CACHE_STATUS_REVERT                         0x02

#define VDS_CACHE_TRACE                                         13
#define vds_CacheTrace CSW_TRACE

// data structure.

// vds cache node structure.
typedef struct _vds_cache_node
{
    UINT32 partid;    
    UINT32 blknr;     
    UINT32 status;
    UINT8 pbuff[VDS_BLOCK_SIZE];
    struct _vds_cache_node* next;
}VDS_CACHE_NODE;

// vds block cache structure].
typedef struct _vds_block_cache
{
    UINT32 blkcnt;    
    VDS_CACHE_NODE* pnode;
}VDS_BLOCK_CACHE;

// global variable.
HANDLE g_hVdsCacheSem = 0;
VDS_BLOCK_CACHE g_cachenode = {0,NULL};


// api fuction body.


INT32 VDS_CacheInit(VOID)
{
    vds_CacheSemInit();
    vds_InitCache();
    vds_SetFlushUnallowed();          
    DSM_HstSendEvent(0xA1000000);
    return ERR_SUCCESS;
}


VOID VDS_CacheFlushAllowed(VOID)
{
    vds_SetFlushAllowed();
    sxs_RegisterFsIdleHookFunc(VDS_CacheFlush);
    hal_BootRegisterBeforeMonCallback(VDS_CacheFlushAll);
    DSM_HstSendEvent(0xA2000000); 
}

//@iKeepTime disable flush time in tick.
VOID VDS_CacheFlushDisable(UINT32 iKeepTime)
{
     vds_DisableFlush(iKeepTime);     
}

VOID VDS_CacheFlushEnable(VOID)
{
     vds_EnableFlush();     
}


INT32 VDS_CacheWriteBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8*pBuff )
{  
    VDS_CACHE_NODE* pnode = NULL;    
    VDS_CACHE_NODE* pprenode = NULL;
    VDS_CACHE_NODE* pnewnode = NULL;    
    INT32 ret = ERR_SUCCESS;
    INT32  result = 0;
    UINT32 cri_status = 0;
    
    if(FALSE == vds_IsCacheInited())
    {
        vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheWriteBlock: vds cache uninit,write to flash.");
        DSM_HstSendEvent(0xB1000000 |(iPartId <<16) | iBlkNr );
        return VDS_WriteBlock( iPartId, iBlkNr, pBuff );
    }    
    
    vds_CacheSemWaitFor();  
    pnode = g_cachenode.pnode;
    pprenode = pnode;
    /*
    if((NULL == pprenode)                            && 
        (VDS_GetFreeBlockNr(iPartId) > 0) &&
        (FALSE == vds_GcIsActive()))           
    {
         cri_status = hal_EnterCriticalSection(); 
         if(!(
#if (defined(COMPRESS_6464) ||defined(COMPRESS_3232))
        !cdfu_overlay_2_1_is_inited() ||
#endif
#if defined(JATAAYU_SUPPORT)
         jWap_GetWapAppStatus() ||
#endif         
#ifdef __PRJ_WITH_SPIFLSH__        
            hal_SpiFlashIsBusy() || 
#endif
            !hal_DmaDone()))
        {
            DSM_HstSendEvent(0xBA000000 | VDS_GetFreeBlockNr(iPartId));             
            result = VDS_WriteBlock( iPartId, iBlkNr, pBuff );
            if(ERR_SUCCESS == result)
            {
                hal_SysExitCriticalSection(cri_status);
                vds_CacheSemRelease();        
                DSM_HstSendEvent(0xBF000000 |(iPartId <<16) | iBlkNr );       
                return result;
            }
            else
            {                
                DSM_HstSendEvent(0xBB000000); 
            }            
       }
       hal_SysExitCriticalSection(cri_status);
    }
    */
    do
    {
        if(NULL == pnode)
        {
            pnewnode = VDS_CACHE_MALLOC(sizeof(VDS_CACHE_NODE));
            if(NULL == pnewnode)
            {
                // vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheWriteBlock: malloc failed. partid = 0x%x,blknr = 0x%x,nodenr = 0x%x.",iPartId,iBlkNr,g_cachenode.blkcnt);
                DSM_HstSendEvent(0xB2000000 |(iPartId <<16) | iBlkNr );
                ret = ERR_VDS_CACHE_VDS_MALLOC_FIALED;
                DSM_ASSERT(0,"VDS_CacheWriteBlock: 1.malloc(0x%x) failed.",(sizeof(VDS_CACHE_NODE))); 
                break;
            }    
            pnewnode->partid = iPartId;
            pnewnode->blknr = iBlkNr;
            pnewnode->status = VDS_CACHE_STATUS_WRITE;
            DSM_MemCpy(pnewnode->pbuff,pBuff,VDS_BLOCK_SIZE);
            pnewnode->next = NULL;   
            
            if(NULL == pprenode)
            {  
               // vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheWriteBlock: add newnode(0x%x,0x%x) to header.",iPartId,iBlkNr);
                g_cachenode.pnode = pnewnode;   
                DSM_HstSendEvent(0xB3000000 |(iPartId <<16) | iBlkNr );
            }
            else
            {                
              //  vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheWriteBlock: add newnode(0x%x,0x%x) to tail.",iPartId,iBlkNr);
                DSM_HstSendEvent(0xB4000000 |(iPartId<<16) | iBlkNr );
                pprenode->next = pnewnode;
            }   
            g_cachenode.blkcnt ++;
            break;
        }
        else if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {
            pnode->status = VDS_CACHE_STATUS_WRITE;
            DSM_MemCpy(pnode->pbuff,pBuff,VDS_BLOCK_SIZE);            
            //  vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheWriteBlock: replace(0x%x,0x%x).",iPartId,iBlkNr);
            DSM_HstSendEvent(0xB5000000 |(iPartId<<16) | iBlkNr);
            break;
        }
        else
        {
            pprenode = pnode;
            pnode = pprenode->next;       
        }        
    }while(1);              
    
    if(FALSE == vds_IsFlushAllowed())
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_CacheFlush: vds cache flush unalloc.");
        DSM_HstSendEvent(0xBC0000EF);        
    }  
    else
    {        
        do
        { 
            break;
            if(NULL == g_cachenode.pnode)
            {
                break;
            }
            cri_status = hal_EnterCriticalSection(); 
            if(
/*
#if (defined(COMPRESS_6464) ||defined(COMPRESS_3232))
              !cdfu_overlay_2_1_is_inited() ||
#endif
#if defined(JATAAYU_SUPPORT) 
               jWap_GetWapAppStatus() ||
#endif
*/
#ifdef __PRJ_WITH_SPIFLSH__        
                 hal_SpiFlashIsBusy() || 
#endif                
#ifndef VOCCODE_IN_PSRAM                
                hal_GetVocDma() ||
#endif  //end VOCCODE_IN_PSRAM
                !hal_DmaDone())
            {
                hal_SysExitCriticalSection(cri_status);
                break;
            }               
            
            if(!hal_LpsRemainingFramesReady(VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER))
            { 
                 hal_SysExitCriticalSection(cri_status);
                 break;               
            }   
            result = vds_CacheNodeFlush(VDS_CACHE_FLUSH_NODE_NUMBER);
            DSM_HstSendEvent(0xBE000000|g_cachenode.blkcnt);  
            hal_SysExitCriticalSection(cri_status);
        }while(0);
    }
    
    vds_CacheSemRelease();
    return ret;
}


INT32 VDS_CacheReadBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8* pBuff )
{
    UINT32 nodenr = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* pprenode = NULL;
    //BOOL bFine = FALSE;
    INT32 ret = ERR_SUCCESS;

    if(FALSE == vds_IsCacheInited())
    {
        ret = VDS_ReadBlock( iPartId, iBlkNr, pBuff );       
        DSM_HstSendEvent(0xC1000000 |(iPartId<<16) | iBlkNr);
        return ret;
    }

    if(g_cachenode.pnode == NULL)
    {           
        ret = VDS_ReadBlock( iPartId, iBlkNr, pBuff );
        return ret;
    }
       
    vds_CacheSemWaitFor();
    
    pnode = g_cachenode.pnode;    
    while(pnode != NULL)
    {
        if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {
            DSM_MemCpy(pBuff,pnode->pbuff,VDS_BLOCK_SIZE);
            //vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheReadBlock: memcpy(0x%x,0x%x).",iPartId,iBlkNr);
            break;
        }
        pprenode = pnode;
        pnode = pprenode->next;
        nodenr ++;
    }  
    if(NULL == pnode)       
    {
        //vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheReadBlock:can't find node,VDS_ReadBlock(0x%x,0x%x)",iPartId,iBlkNr); 
        ret = VDS_ReadBlock(iPartId,iBlkNr,pBuff);
    }   
    
    vds_CacheSemRelease();
    return ret;
}

#if 1
INT32 VDS_CacheRevertBlock( UINT32 iPartId, UINT32 iBlkNr)
{
    iPartId = iPartId;
    iBlkNr = iBlkNr;
    return ERR_SUCCESS; 
}
#else
INT32 VDS_CacheRevertBlock( UINT32 iPartId, UINT32 iBlkNr)
{

    UINT32 nodenr = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* pprenode = NULL;
    
    if(FALSE == vds_IsCacheInited())
    {
        vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheRevertBlock: vds cache uninit.");
        return VDS_RevertBlock(iPartId, iBlkNr);
    }

    vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheRevertBlock: partid = 0x%x,blknr = 0x%x.",iPartId,iBlkNr);
    
    vds_CacheSemWaitFor();
    pnode = g_cachenode.pnode;    
    while(pnode != NULL)
    {
        if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {
            pnode->status = VDS_CACHE_STATUS_REVERT;
            DSM_MemSet(pnode->pbuff,0,VDS_BLOCK_SIZE);
            break;
        }
        pprenode = pnode;
        pnode = pprenode->next;
        nodenr ++;
    }
     if(NULL == pnode)
    {
        pnode = VDS_CACHE_MALLOC(sizeof(VDS_CACHE_NODE));
        if(NULL == pnode)
        {
            vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheRevertBlock: malloc failed. partid = 0x%x,blknr = 0x%x,nodenr = 0x%x.",iPartId,iBlkNr,nodenr);
            vds_CacheSemRelease();
            return ERR_VDS_CACHE_VDS_MALLOC_FIALED;
        }    
        pnode->partid = iPartId;
        pnode->blknr = iBlkNr;
        pnode->status = VDS_CACHE_STATUS_REVERT;     
        DSM_MemSet(pnode->pbuff,0,VDS_BLOCK_SIZE);
        pnode->next = NULL;      
        
         if(NULL != pprenode)
        {  
            pprenode->next = pnode;
        }
        if(NULL == g_cachenode.pnode)
        {
            g_cachenode.pnode = pnode;
        }   
        g_cachenode.blkcnt ++;
    }    
    vds_CacheSemRelease();
  
    return ERR_SUCCESS;
}
#endif

VOID VDS_CacheFlushAll(VOID)
{
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* ptmpnode = NULL;
    INT32 result = 0 ;
    UINT32 cri_status = 0;
    UINT32 flush_count = 0;
    UINT32 wait_time = 0;
    INT32 ret = ERR_SUCCESS;
    UINT32 threshold = (1000 * (16384/1000));
    
    // Uninit.      
    if(FALSE == vds_IsCacheInited() )
    {        
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_CacheFlushAll: vds cache uninit.");
        DSM_HstSendEvent(0xF100001E);
        return;
    }

    if(g_cachenode.pnode == NULL)
    {
        DSM_HstSendEvent(0xF100003E);
        return;
    }
    
    
    hal_FintIrqSetMask(FALSE);
    hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_0, hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_0));
    hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_1, hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_1));
  //  vds_SetFlushUnallowed();
    hal_TimWatchDogClose();
    vds_CacheSemWaitFor(); 
    cri_status = hal_EnterCriticalSection();    
    wait_time = hal_TimGetUpTime();
    hal_AudForcePowerDown();
    do
    {      
        if(g_cachenode.pnode == NULL)
        {             
            hal_SysExitCriticalSection(cri_status);               
            vds_CacheSemRelease();
            DSM_HstSendEvent(0xF100003E);
            return;
        }
        if((hal_TimGetUpTime() - wait_time) > threshold )
        {              
            DSM_HstSendEvent(0xF100004E);              
            break;
        }          
        if(
#ifdef __PRJ_WITH_SPIFLSH__        
            hal_SpiFlashIsBusy() || 
#endif
            !hal_DmaDone())
        {
            continue;
        }
        else
        {
            break;
        }
    }while(1);
    
    DSM_HstSendEvent(0xF2000000 | g_cachenode.blkcnt);     
    pnode = g_cachenode.pnode;    
    do
    {        
        if(TRUE == vds_GcIsActive())
        {
            //DSM_HstSendEvent(0xF20000A0); 
            result = vds_GcExt(0);
             if(_ERR_VDS_GC_FAILED == result)
            {
               DSM_HstSendEvent(0xF100006E);
               break;
            }
             
            if(_ERR_VDS_GC_FINISH == result)
            {
                vds_GcDeactive();
                DSM_HstSendEvent(0xF20000A1); 
            }                
            continue;
        }
        
        if(pnode != NULL)
        {     
            if(VDS_CACHE_STATUS_WRITE == pnode->status)
            {
                result = VDS_WriteBlock(pnode->partid,pnode->blknr,pnode->pbuff);
                if(result == ERR_SUCCESS)
                {        
                    vds_CacheTrace(BASE_VDS_TS_ID, "VDS_CacheFlush: VDS_WriteBlock(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                    DSM_HstSendEvent(0xF3000000 |(pnode->partid << 16) | pnode->blknr );
                    ptmpnode = pnode; 
                    pnode = pnode->next;
                    VDS_CACHE_FREE(ptmpnode);
                    g_cachenode.pnode = pnode;
                    g_cachenode.blkcnt --;
                    flush_count ++;                    
                }
                else
                {
                    if(ERR_VDS_SPACE_FULL == result)
                    {
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_CacheFlush: writeblock(0x%x,0x%x)  no more free pb.",pnode->partid,pnode->blknr);     
                        DSM_HstSendEvent(0xF30000EF);                      
                    }
                    else
                    {
                        ret = ERR_VDS_CACHE_WRITE_FAILED;
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_CacheFlush: writeblock(0x%x,0x%x)  write vds error.result = %d.",pnode->partid,pnode->blknr,result);     
                        DSM_HstSendEvent(0xF30000EE);
                        break;
                    }                    
                }
            }
            else
            {
                ptmpnode = pnode; 
                pnode = pnode->next;
                VDS_CACHE_FREE(ptmpnode);
                g_cachenode.pnode = pnode;
                g_cachenode.blkcnt --;
                flush_count ++;         
                DSM_HstSendEvent(0xF40000EF);
            }
        }
        else
        {
            DSM_HstSendEvent(0xF50000EF);
            break;
        }           
    }while(pnode != NULL);
    
    hal_SysExitCriticalSection(cri_status);  
    vds_CacheSemRelease();
    vds_CacheTrace(VDS_CACHE_TRACE, "VDS_CacheFlush: flush node count = 0x%x.",flush_count);
    DSM_HstSendEvent(0xF5000000 | flush_count);
   
    return;
}

VOID VDS_FlushCacheAllFly(VOID)
{
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* ptmpnode = NULL;
    INT32 result = 0 ;
    UINT32 cri_status = 0;
    UINT32 flush_count = 0;
    UINT32 wait_time = 0;
    INT32 ret = ERR_SUCCESS;
       
    // Uninit.      
    if(FALSE == vds_IsCacheInited() )
    {        
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCacheAllFly: vds cache uninit.");
        DSM_HstSendEvent(0xF100001E);
        return ;
    }

    if(g_cachenode.pnode == NULL)
    {
        DSM_HstSendEvent(0xF100003E);
        return;
    }
    
    
    vds_CacheSemWaitFor(); 
#if 0     
    //cri_status = hal_EnterCriticalSection();    
    wait_time = hal_TimGetUpTime();
    do
    {
        if(g_cachenode.pnode == NULL)
        {
            hal_SysExitCriticalSection(cri_status);               
            vds_CacheSemRelease();
            DSM_HstSendEvent(0xF100003E);
            return ;
        }
 
        //10 second.
        if((hal_TimGetUpTime() - wait_time) > (1000 * (16384/1000)))
        {              
            DSM_HstSendEvent(0xF100004E); 
            break ;
        }
        
        if(
#ifdef __PRJ_WITH_SPIFLSH__
            hal_SpiFlashIsBusy() || 
#endif
            !hal_DmaDone())
        {
            continue;
        }
        else
        {
            break;
        }   
    }while(1);
#endif    
    DSM_HstSendEvent(0xF2000000 | g_cachenode.blkcnt);     
    pnode = g_cachenode.pnode;    
    do
    {         
         if(0 == cri_status)
         {
             cri_status = hal_EnterCriticalSection();
         }
         if((                 

#ifdef __PRJ_WITH_SPIFLSH__        
        hal_SpiFlashIsBusy() || 
#endif
#ifndef VOCCODE_IN_PSRAM        
        hal_GetVocDma() ||          
#endif          
        !hal_DmaDone()                 
        || !hal_LpsRemainingFramesReady(VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER)))
        {
          /*  if(0 != cri_status)
            {
                hal_SysExitCriticalSection(cri_status);  
                cri_status = 0;
            }*/
            if(0 != cri_status)
            {
                hal_SysExitCriticalSection(cri_status);  
                cri_status = 0;
            }
            COS_SleepByTick(20);
            continue;
        }
       
        if(TRUE == vds_GcIsActive())
        {
            //   DSM_HstSendEvent(0xF20000A0); 
            result = vds_GcExt(0);
             if(_ERR_VDS_GC_FAILED == result)
            {
               DSM_HstSendEvent(0xF100006E);
               break;
            }
             
            if(_ERR_VDS_GC_FINISH == result)
            {
                vds_GcDeactive();
                DSM_HstSendEvent(0xF20000A1); 
            }                
            if(0 != cri_status)
            {
                hal_SysExitCriticalSection(cri_status);  
                cri_status = 0;
            }
            continue;
        }
        
        if(pnode != NULL)
        {     
            if(VDS_CACHE_STATUS_WRITE == pnode->status)
            {
                result = VDS_WriteBlock(pnode->partid,pnode->blknr,pnode->pbuff);
                if(result == ERR_SUCCESS)
                {        
                    vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCacheAllFly: VDS_WriteBlock(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                    DSM_HstSendEvent(0xF3000000 |(pnode->partid << 16) | pnode->blknr );
                    ptmpnode = pnode; 
                    pnode = pnode->next;
                    VDS_CACHE_FREE(ptmpnode);
                    g_cachenode.pnode = pnode;
                    g_cachenode.blkcnt --;
                    flush_count ++;                    
                }
                else
                {
                    if(ERR_VDS_SPACE_FULL == result)
                    {
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCacheAllFly: writeblock(0x%x,0x%x)  no more free pb.",pnode->partid,pnode->blknr);     
                        DSM_HstSendEvent(0xF30000EF);                      
                    }
                    else
                    {
                        ret = ERR_VDS_CACHE_WRITE_FAILED;
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCacheAllFly: writeblock(0x%x,0x%x)  write vds error.result = %d.",pnode->partid,pnode->blknr,result);     
                        DSM_HstSendEvent(0xF30000EE);
                        break;
                    }                    
                }
            }
            else
            {                
                ptmpnode = pnode; 
                pnode = pnode->next;
                VDS_CACHE_FREE(ptmpnode);
                g_cachenode.pnode = pnode;
                g_cachenode.blkcnt --;
                flush_count ++;                    
                DSM_HstSendEvent(0xF40000EF);
            }
        }
        else
        {
            DSM_HstSendEvent(0xF50000EF);
            break;
        }           
        if(0 != cri_status)
        {
            hal_SysExitCriticalSection(cri_status);  
            cri_status = 0;
        }
    }while(pnode != NULL);    
    if(0 != cri_status)
    {
        hal_SysExitCriticalSection(cri_status);  
        cri_status = 0;
    }
    vds_CacheSemRelease();
    vds_CacheTrace(VDS_CACHE_TRACE, "VDS_FlushCacheAllFly: flush node count = 0x%x.",flush_count);
    DSM_HstSendEvent(0xF5000000 | flush_count);
   
    return ;
}

VOID VDS_CacheFlushDirect(UINT32 iTime)
{
     UINT32 iBeginTime;
     
     iBeginTime = hal_TimGetUpTime();
     while(g_cachenode.blkcnt > 0)
     {
        if((hal_TimGetUpTime() - iBeginTime)*16384/1000 > iTime)
        {
            break;
        }
        VDS_CacheFlush();
        COS_SleepByTick(5);
     }
}



//UINT32 g_tstCacheFlush0 = 0;
//UINT32 g_tstTimes = 0;
VOID VDS_CacheFlush(VOID)
{     
    UINT32 cri_status = 0;
    
    //g_tstCacheFlush0 = 0;
    // g_tstTimes ++;
    VDS_PROFILE_FUNCTION_ENTER(VDS_CacheFlush);
    cri_status = hal_EnterCriticalSection();
    if(!(                 
/*
#if (defined(COMPRESS_6464) ||defined(COMPRESS_3232))
        !cdfu_overlay_2_1_is_inited() ||
#endif
#if defined(JATAAYU_SUPPORT) 
        jWap_GetWapAppStatus() ||
#endif
*/
#ifdef __PRJ_WITH_SPIFLSH__        
        hal_SpiFlashIsBusy() || 
#endif
#ifndef VOCCODE_IN_PSRAM        
        hal_GetVocDma() ||          
#endif          
        !hal_DmaDone()  ||               
        !hal_LpsRemainingFramesReady(VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER)))
    {
        if(NULL == g_cachenode.pnode)
        {           
            if(TRUE == vds_IsFlushAllowed())
            {
                tgt_FlushFactorySettings();
            }
        }
        else
        {                 
            vds_CacheNodeFlush(VDS_CACHE_FLUSH_NODE_NUMBER);  
        }
    }    
    hal_SysExitCriticalSection(cri_status);
    VDS_PROFILE_FUNCTION_EXIT(VDS_CacheFlush);
    
    return;
}


VOID vds_CacheSemInit(VOID)
{
   
    g_hVdsCacheSem = sxr_NewSemaphore(1);
    if((HANDLE)NULL == g_hVdsCacheSem)
    {
        D( ( DL_WARNING, "VDS_ModuleSemInit failed."));
    }
}


VOID vds_CacheSemWaitFor(VOID)
{    
    
    if((HANDLE)NULL != g_hVdsCacheSem)
    {
        sxr_TakeSemaphore(g_hVdsCacheSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleDown failed."));
    }

}


VOID vds_CacheSemRelease(VOID)
{
    
    if((HANDLE)NULL != g_hVdsCacheSem)
    {
        sxr_ReleaseSemaphore(g_hVdsCacheSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleUp failed."));
    }
}

INT32 vds_CacheNodeFlush(UINT32 flush_nr)
{
    UINT32 flush_count = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* ptmpnode = NULL;
    INT32 result;
    INT32 ret = ERR_SUCCESS;
    
    
    // Uninit.      
    if(FALSE == vds_IsCacheInited() )
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: vds cache uninit.");
        DSM_HstSendEvent(0xE10000AE);
        return flush_count;
    }

    // unallowed flush
    if(FALSE == vds_IsFlushAllowed())
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: vds cache flush unalloc.");
        DSM_HstSendEvent(0xE2000000|g_cachenode.blkcnt);
        return flush_count;
    }  

    if(TRUE == vds_GcIsActive())
    {  
        
        result = vds_GcExt(1);
        if(_ERR_VDS_GC_FINISH == result)
        {
            DSM_HstSendEvent(0xE3000000|g_cachenode.blkcnt);  
            CSW_TRACE(BASE_DSM_TS_ID, "vds_CacheNodeFlush: vds_GcExt() ok.");
            vds_GcDeactive();
        }
        else
        {
           // donoting.
        }        
        return ERR_VDS_CACHE_GCING;
    }
    else
    {                
        pnode = g_cachenode.pnode;
        while(pnode != NULL && flush_count < flush_nr)
        {
            if(VDS_CACHE_STATUS_WRITE == pnode->status)
            {                
                result = VDS_WriteBlock(pnode->partid,pnode->blknr,pnode->pbuff);         
                if(ERR_SUCCESS == result)
                {
                    //vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: writeblock(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                    //DSM_HstSendEvent(0xE4000000 | (pnode->partid << 16) |pnode->blknr);
                    ptmpnode = pnode;
                    pnode =   pnode->next;  
                    VDS_CACHE_FREE(ptmpnode);
                    g_cachenode.pnode = pnode; 
                    g_cachenode.blkcnt --;
                    flush_count ++;                      
                } 
                else
                {
                    if(ERR_VDS_SPACE_FULL == result)
                    {
                        //vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: writeblock(0x%x,0x%x)  no more free pb,to gc.",pnode->partid,pnode->blknr);     
                        // DSM_HstSendEvent(0xE5000000 | (pnode->partid << 16) |pnode->blknr);
                        ret = ERR_SUCCESS;
                    }
                    else
                    {
                        //vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: writeblock(0x%x,0x%x)  write vds error.result = %d.",pnode->partid,pnode->blknr,result);     
                        // DSM_HstSendEvent(0xE6000000 | (pnode->partid << 16) |pnode->blknr);
                        ret = ERR_VDS_CACHE_WRITE_FAILED;
                    }
                    break;
                }
            }            
            else
            {        
                vds_CacheTrace(BASE_VDS_TS_ID, "vds_CacheNodeFlush: free unkown statues block(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                DSM_HstSendEvent(0xE7000000 | (pnode->partid << 16) |pnode->blknr);        
                ptmpnode = pnode;
                pnode =   pnode->next;  
                VDS_CACHE_FREE(ptmpnode);
                g_cachenode.pnode = pnode; 
                g_cachenode.blkcnt --;
                flush_count ++;                          
            }
            
        }
        if(NULL == g_cachenode.pnode)
        {
             ret = ERR_VDS_CACHE_NODE_NULL;
             DSM_HstSendEvent(0xE8000000|g_cachenode.blkcnt);
        }
        else
        {
             DSM_HstSendEvent(0xE9000000|g_cachenode.blkcnt);  
        }
        
     } 
     return ret;
}

BOOL VDS_CacheNodeIsLess(VOID)
{    
     if(g_cachenode.blkcnt >= VDS_CACHE_NODE_NUMBER_MAX)
     {
            return FALSE;
     }
     else
     {
            return TRUE;
     }
}


UINT32 VDS_CacheGetBlkCnt(VOID)
{
	return g_cachenode.blkcnt;
}

#endif

