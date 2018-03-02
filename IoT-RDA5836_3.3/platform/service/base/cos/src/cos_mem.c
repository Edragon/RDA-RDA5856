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


#include "cos.h"

#include "sxs_srl.h"
#include "sxs_type.h"
#include "sxs_lib.h"
#include "sxr_ops.h"
#include "errorcode.h"
#include "sxr_mem.hp"
//#define HAVE_NO_TS_IF
#include "ts.h"
#include "cswtype.h"
#include "csw.h"
#include "csw_config.h"
#include "sxs_io.h"
#include "sxr_mem.h"
#include "sxr_usrmem.h"
#include "hal_overlay.h"
#include "hal_host.h"


#define COS_MEM_FRAGMENT_RECOVERY
#define COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY


#ifdef COS_MEM_FRAGMENT_RECOVERY
#ifndef CT_NO_DEBUG
#define COS_MEM_FRAGMENT_RECOVERY_CHECK
#define COS_MEM_FRAGMENT_RECOVERY_STATISTICS
#endif
#endif



/*
 * for memory less than 256byte, use memory poll,
 * for memory great than 256byte, use list to reduce times malloc/free
 */

/*
 * memory pool: 64byte * 256block
 * for memory less than 64, alloc from head
 * for memory greate than 64 and less than 256, alloc from bottom
 */

#define MEMORY_BLOCK_SIZE           32

#ifndef COS_MEM_FRAGMENT_RECOVERY
#if defined(GSM_SUPPORT)
#define MEMORY_POOL_SIZE      32
#define MEMORY_BIG_SIZE       1024
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define MEMORY_POOL_SIZE      8
#define MEMORY_BIG_SIZE       256
#else
#define MEMORY_POOL_SIZE      16 ///  32
#define MEMORY_BIG_SIZE       512 // 1024
#endif /* GSM_SUPPORT */
#define HEAP_COUNT               3
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY
#define MEMORY_ALLOC_MIN_SIZE   MEMORY_BLOCK_SIZE
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define TO_CACHED_ADDR(addr) (addr)
#define IS_UNCACHED_ADDR(addr) (FALSE)
#else
#define TO_UNCACHED_ADDR(addr) ((UINT32)(addr)|0x20000000)
#define TO_CACHED_ADDR(addr) ((UINT32)(addr)&0xdfffffff)
#define IS_UNCACHED_ADDR(addr) ((((UINT32)(addr))& 0x20000000) == 1)
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
#define COS_MEM_STATISTIC_CAL_COUNT     500

typedef struct _COS_MEM_USAGE_T
{
    UINT32 count;
    UINT32 totalMem;
    UINT32 usedMaxMemory;
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    UINT32 totalMemOverlay;
    UINT32 usedMaxOverlayMemory;
    UINT64 usedOverlaySum;
    UINT64 listOverlaySum;
    UINT16 usedOverlayPercent;
    UINT16 listOverlayPercent;
#endif
    UINT64 usedSum;
    //UINT64 poolSum;
    UINT64 listSum;
    UINT16 usedPercent;
    //UINT16 poolPercent;
    UINT16 listPercent;
    
    UINT64 mallocTimeSum;
    UINT64 freeTimeSum;
    UINT32 mallocTimeAverage;
    UINT32 freeTimeAverage;
    UINT32 mallocCount;
    UINT32 freeCount;
    UINT32 mallocTimeMax;
    UINT32 mallocTimeMaxSize;
    UINT32 freeTimeMax;
    UINT32 freeTimeMaxSize;
    UINT16 mallocTime[17];  //0x00~0x0f + (>0x0f)
} COS_MEM_USAGE_T;

COS_MEM_USAGE_T cos_mem_usage;
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
typedef struct _COS_MEM_FRAGMENT_RECOVERY_CHECK_T
{
    UINT32 totalMem;
    UINT32 totalMemOverlay;
} COS_MEM_FRAGMENT_RECOVERY_CHECK_T;

COS_MEM_FRAGMENT_RECOVERY_CHECK_T cos_mem_check;
#endif

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
#ifdef COS_MEM_FRAGMENT_RECOVERY

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define OVERLAY_MEMORY_BLOCK_NUM    1
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#define OVERLAY_MEMORY_BLOCK_NUM    2
#endif

typedef struct _t_overlay_mem_info {
    UINT32 heap_start_addr;
    UINT32 heap_end_addr;
}t_overlay_mem_info;

#endif
#endif

typedef struct st_mem_block t_mem_block;

struct st_mem_block {
    UINT32 length;
    t_mem_block *next;
#ifdef COS_MEM_FRAGMENT_RECOVERY
    t_mem_block *addrNext;  // sort by address
#endif
};

typedef struct t_host_mem_info {
    HANDLE mem_mutex;
    BOOL   uncache_independ;

#ifndef COS_MEM_FRAGMENT_RECOVERY
    UINT32 *pHeap_Base[HEAP_COUNT+1]; // add one heap for overaly
    UINT32 heap_Size[HEAP_COUNT+1];
    t_mem_block *mem_pool[MEMORY_POOL_SIZE];
#endif

    t_mem_block *mem_list;          /* memory block list greate than MEMORY_BIG_SIZE */
    t_mem_block *uncache_mem_list;          /* uncache memory block list */
#ifdef COS_MEM_FRAGMENT_RECOVERY
    t_mem_block *addr_mem_list;  // sort by address
#endif
	//t_mem_block *alloc_list;
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
#ifdef COS_MEM_FRAGMENT_RECOVERY
    t_mem_block *mem_list_overlay;
    t_mem_block *addr_list_overlay;
#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
    t_mem_block *normal_mem_list_malloc_by_overlay;
#endif
    t_overlay_mem_info overlay_mem_info[OVERLAY_MEMORY_BLOCK_NUM];
#else
    UINT32 overlay_heap_size;
    t_mem_block *overlay_list;
#endif
#endif
    UINT32 alloc_length;
#ifdef COS_MEM_FRAGMENT_RECOVERY
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    UINT32 alloc_length_overlay;
    UINT32 list_length_overlay;
#endif
    UINT32 list_length;
#else
    UINT32 pool_length;
    UINT32 fragment_length;
#endif
} t_mem_info;

t_mem_info cos_mem_info;

U32 *g_pHeapTop; // for test

/*
cos memory track :

Record memory caller, malloc time, size and address

Print all mem use/free info when assert raise for analyze in EXCEL

It take (COS_MEM_TRACK_MAX_RECORD * 16) bytes 

*/

#ifndef CT_NO_DEBUG
#define  COS_MEM_TRACK
#endif

#ifdef COS_MEM_TRACK

#define COS_MEM_TRACK_MAX_RECORD            25

typedef struct _cos_mem_track_t
{
	VOID * caller;
	VOID * mem_ptr;
	UINT32 size;
	UINT32 timestamp;
}cos_mem_track_t; //size 16

volatile UINT32 s_cos_mem_track_caller = 0;

UINT32 s_cos_mem_track_total_inuse = 0;
cos_mem_track_t s_cos_mem_track[COS_MEM_TRACK_MAX_RECORD] = {{0}};
void *rdabt_malloc(UINT32 size);

/******************************************************************************
 * COSMemTrackAlloc: 
 * DESCRIPTION: - save memory caller, size , timestamp to track array
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID COSMemTrackAlloc(UINT32 caller, UINT32 ptr, UINT32 size)
{
	UINT16 i;

#if CHIP_ASIC_ID == CHIP_ASIC_ID_5856E
#if COMBINED_HOST != 0
    if(caller > (UINT32)rdabt_malloc && caller < (UINT32)rdabt_malloc+0x80)
        return;
#endif    
#endif
    if(size<64)
        return;

	for(i = 0; i < COS_MEM_TRACK_MAX_RECORD; i++)
	{
		if(0 == s_cos_mem_track[i].mem_ptr)
		{
			//find a empty record, save data.
			s_cos_mem_track[i].caller   = (VOID * ) caller;
			s_cos_mem_track[i].mem_ptr  = (VOID * ) ptr;
			s_cos_mem_track[i].size     = size;
			s_cos_mem_track[i].timestamp = hal_TimGetUpTime();

			s_cos_mem_track_total_inuse += size;

			//if(size > 128)
		        //CSW_TRACE(BASE_COS_TS_ID, "COSMemTrackAlloc caller=0x%x, ptr=0x%x, size=0x%x, h_size=%d", caller, ptr, size, cos_mem_info.heap_Size[2]);

			break;
		}
	}
}

VOID COSMemTrackFree(UINT32 caller, UINT32 ptr, UINT32 size)
{
	UINT16 i;

	for(i = 0; i < COS_MEM_TRACK_MAX_RECORD; i++)
	{
		if(ptr == (UINT32)s_cos_mem_track[i].mem_ptr)
		{
			//find cur
			s_cos_mem_track[i].caller   = (VOID * ) 0;
			s_cos_mem_track[i].mem_ptr  = (VOID * ) 0;
			s_cos_mem_track[i].size     = 0;

			s_cos_mem_track_total_inuse -= size;
			
			//if(size > 128)
		        //CSW_TRACE(BASE_COS_TS_ID, "COSMemTrackFree caller=0x%x, ptr=0x%x, size=0x%x,h_size=%d", caller, ptr, size,cos_mem_info.heap_Size[2]);

			break;
		}
	}

}

/******************************************************************************
 * COSMemTrackPrint: 
 * DESCRIPTION: -     Print memory alloc/free infomation (with memory head t_mem_block) to log, 
 *               and we can copy to excel to sort memory address.
 *
 *                 In nomal case, pre-line's #end_addr# is equal to next-line's #mem_ptr#
 *               and, SUM(#size# colume) + SUM(pMemInfo->heap_Size) = Total heap size
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID COSMemTrackPrint(VOID)
{
	//Example:  Alloc, 0xcaller_addr,  0xmem_ptr,  0xsize,  0xend_addr,   0xcall_time
	//          Free, 0x0,            0xmem_ptr,  0xsize,  0xend_addr,   0x0
	#define EXCEL_COLUME_TITLE     "Name,caller_addr,mem_ptr,size,end_addr,call_time"
	#define EXCEL_COLUME_CONTENT "%s,0X%08X,0X%08X,0X%08X,0X%08X,0X%08X"

    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block * ptr_t_mem;
	UINT32 i;


    CSW_TRACE(BASE_COS_TS_ID, "=====COSMemTrackPrint========");
    CSW_TRACE(BASE_COS_TS_ID, "Copy below to excel and use =HEX2DEC(RIGHT(num,8)) to convert dec");
    CSW_TRACE(BASE_COS_TS_ID, EXCEL_COLUME_TITLE);

	for(i = 0; i < COS_MEM_TRACK_MAX_RECORD; i++)
	{
		if(s_cos_mem_track[i].mem_ptr)
		{
	    	CSW_TRACE(BASE_COS_TS_ID, EXCEL_COLUME_CONTENT, "Alloc",
    							s_cos_mem_track[i].caller,
    							((UINT32)s_cos_mem_track[i].mem_ptr) - sizeof(t_mem_block),
    							s_cos_mem_track[i].size + sizeof(t_mem_block),
    							((UINT32)s_cos_mem_track[i].mem_ptr) + s_cos_mem_track[i].size,
    							s_cos_mem_track[i].timestamp);
    	}
	}

	
#ifndef COS_MEM_FRAGMENT_RECOVERY
	for(i = 0; i < MEMORY_POOL_SIZE; i++)
	{
		ptr_t_mem = pMemInfo->mem_pool[i];
		while(ptr_t_mem)
		{
	    	CSW_TRACE(BASE_COS_TS_ID, EXCEL_COLUME_CONTENT, "AFree",
	    							0,
	    							ptr_t_mem,
	    							ptr_t_mem->length + sizeof(t_mem_block),
	    							((UINT32)ptr_t_mem) + ptr_t_mem->length + sizeof(t_mem_block),
	    							0);
			ptr_t_mem = ptr_t_mem->next;
		}

	}
#endif

	ptr_t_mem = pMemInfo->mem_list;
	while(ptr_t_mem)
	{
    	CSW_TRACE(BASE_COS_TS_ID, EXCEL_COLUME_CONTENT, "BFree",
    							0,
    							ptr_t_mem,
    							ptr_t_mem->length + sizeof(t_mem_block),
    							((UINT32)ptr_t_mem) + ptr_t_mem->length + sizeof(t_mem_block),
    							0);
		ptr_t_mem = ptr_t_mem->next;
	}

#ifndef COS_MEM_FRAGMENT_RECOVERY
	for(i = 0; i < HEAP_COUNT; i++)
	{
		if(pMemInfo->pHeap_Base[i] != NULL && pMemInfo->heap_Size[i])
	    CSW_TRACE(BASE_COS_TS_ID, "%d"EXCEL_COLUME_CONTENT,  i, "Heap",
	    						0,
	    						pMemInfo->pHeap_Base[i],
						    	pMemInfo->heap_Size[i],
								((UINT32) pMemInfo->pHeap_Base[i]) + pMemInfo->heap_Size[i],
							    0);
	}
#endif

    CSW_TRACE(BASE_COS_TS_ID, EXCEL_COLUME_CONTENT,
    								"SysRam",
    								0,
    								&_sys_sram_heap_start,
    								&_sys_sram_heap_size,
    								(UINT32) &_sys_sram_heap_start + (UINT32) &_sys_sram_heap_size,
    								0);
    

}
#endif /* COS_MEM_TRACK */
// ============================================================================= 
// COS_Malloc 
// ----------------------------------------------------------------------------- 
/// For application malloc
// ============================================================================= 
void trace_mem()
{
	hal_HstSendEvent(SYS_EVENT,(UINT32)g_pHeapTop);
	//hal_HstSendEvent(SYS_EVENT,g_pHeapBase);
}

#ifndef COS_MEM_FRAGMENT_RECOVERY
t_mem_block *alloc_mem_block(int blk_index, UINT32 size)
{
    t_mem_block *pMem = NULL;
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    
    if(pMemInfo->heap_Size[blk_index] >= size+sizeof(t_mem_block))
    {
        pMem = (t_mem_block*)(pMemInfo->pHeap_Base[blk_index]);
        pMem->length = size;
        pMem->next = NULL;
        pMemInfo->pHeap_Base[blk_index] += (pMem->length+sizeof(t_mem_block))>>2;
        pMemInfo->heap_Size[blk_index] -= (pMem->length+sizeof(t_mem_block));
        //CSW_TRACE(BASE_COS_TS_ID, "COS_Malloc allocate in heap[%d],size=%d, pHeap_Base=%x,heap_size=%d", blk_index, size, pMemInfo->pHeap_Base[blk_index], pMemInfo->heap_Size[blk_index]);
    }
    return pMem;
}
#endif

t_mem_block *find_mem_block(t_mem_block *header, UINT32 size)
{
    t_mem_block *pMem = NULL, *pNext;

    pNext = pMem = header;
    while(pMem)
    {
        if(pMem->length >= size)
        {
            if(pMem->length > (size << 1))
            {
                pMem = NULL;
                break;
            }
            if(pNext != pMem)
                pNext->next = pMem->next;
            break;
        }
        pNext = pMem;
        pMem = pMem->next;
    }
    return pMem;
}

#ifdef COS_MEM_FRAGMENT_RECOVERY
/******************************************************************************
 * mem_addMemoryToAddressList: 
 * DESCRIPTION: -     add memory to the list which is sort by address, such as 
 *               cos_mem_info->addr_mem_list.
 *
 * Input: 
 *          ppList - memory list.
 *          pMem - memory to be added to the list.
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID mem_addMemoryToAddressList(t_mem_block **ppList, t_mem_block *pMem)
{
    t_mem_block *pNext;
    BOOL header = FALSE;

    pNext = *ppList;

    if(pMem == NULL)
        return;

    if(pNext == NULL)
    {
        header = TRUE;
        pMem->addrNext = NULL;
    }
    else
    {
        while( (pNext->addrNext) && ((UINT32)(pNext ->addrNext) < (UINT32)pMem) )
        {
            pNext = pNext->addrNext;
        }

        if((UINT32)pNext < (UINT32)pMem)
        {
            header = FALSE;
            pMem->addrNext = pNext->addrNext;
            pNext->addrNext = pMem;
        }
        else if((UINT32)pNext == (UINT32)pMem)
        {
            hal_DbgAssert("The memory has been released!");
        }
        else
        {
            header = TRUE;
            pMem->addrNext = pNext;
        }
    }
    if(header)
    {
        *ppList = pMem;
    }
}

/******************************************************************************
 * mem_addMemoryToLengthList: 
 * DESCRIPTION: -     add memory to the list which is sort by memory length, such as 
 *               cos_mem_info->mem_list.
 *
 * Input: 
 *          ppList - memory list.
 *          pMem - memory to be added to the list.
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID mem_addMemoryToLengthList(t_mem_block **ppList, t_mem_block *pMem, BOOL isOverlay)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pNext;
    BOOL header;

    pNext = *ppList;

    if(pMem == NULL)
        return;

    if(pMem->length < MEMORY_ALLOC_MIN_SIZE)
    {
        hal_DbgAssert("The memory size is less than MEMORY_ALLOC_MIN_SIZE");
    }

    //if( (!isOverlay) && (pMem->length <= MEMORY_BIG_SIZE) )
    //    return;

    if(pNext == NULL)
    {
        header = TRUE;
        pMem->next= NULL;
    }
    else
    {
        while(pNext->next && pNext ->next->length < pMem->length)
        {
            //pAssert(pNext != pMem);
            pNext = pNext->next;
        }

        if(pNext ->length < pMem->length)
        {
            header = FALSE;
            pMem->next = pNext->next;
            pNext->next = pMem;
        }
        else
        {
            header = TRUE;
            pMem->next = pNext;
        }
    }
    if(header)
    {
        *ppList = pMem;
    }
 #if (CHIP_BTOVERLAY_SUPPORTED!=0)
    if(isOverlay)
    {
        pMemInfo->list_length_overlay+= pMem->length+sizeof(t_mem_block);
    }
    else
#endif
    {
        pMemInfo->list_length += pMem->length+sizeof(t_mem_block);
    }
}

/******************************************************************************
 * mem_removeMemoryFromAddressList: 
 * DESCRIPTION: -     remove memory from the list which is sort by address, such as 
 *               cos_mem_info->addr_mem_list.
 *
 * Input: 
 *          ppList -  memory list.
 *          pMem -  memory to be removed from the list.
 * Output: 
 * Returns: 
 *          TRUE -  remove the memory successfully.
 *          FALSE - not find memory in the list.
 * 
 * 
 ******************************************************************************/
BOOL mem_removeMemoryFromAddressList(t_mem_block **ppList, t_mem_block *pMem)
{
    t_mem_block *pPrev, *pNext;

    if(*ppList == NULL)
        return FALSE;

    if(*ppList == pMem)
    {
        *ppList = (*ppList)->addrNext;
        return TRUE;
    }

    pPrev = *ppList;
    pNext = (*ppList)->addrNext;
    while(pNext)
    {
        if(pNext == pMem)
        {
            pPrev->addrNext = pNext->addrNext;
            return TRUE;
        }
        pPrev = pNext;
        pNext = pNext->addrNext;
    }

    return FALSE;
}

/******************************************************************************
 * mem_removeMemoryFromLengthList: 
 * DESCRIPTION: -     remove memory from the list which is sort by memory length, such as 
 *               cos_mem_info->mem_list.
 *
 * Input: 
 *          ppList -  memory list.
 *          pMem -  memory to be removed from the list.
 * Output: 
 * Returns: 
 *          TRUE -  remove the memory successfully.
 *          FALSE - not find memory in the list.
 * 
 * 
 ******************************************************************************/
BOOL mem_removeMemoryFromLengthList(t_mem_block **ppList, t_mem_block *pMem, BOOL isOverlay)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pPrev, *pNext;

    if(*ppList == NULL)
        return FALSE;

    if(*ppList == pMem)
    {
        *ppList = (*ppList)->next;
 #if (CHIP_BTOVERLAY_SUPPORTED!=0)
        if(isOverlay)
        {
            pMemInfo->list_length_overlay-= pMem->length+sizeof(t_mem_block);
        }
        else
#endif
        {
            pMemInfo->list_length -= pMem->length+sizeof(t_mem_block);
        }
        return TRUE;
    }

    pPrev = *ppList;
    pNext = (*ppList)->next;
    while(pNext)
    {
        if(pNext == pMem)
        {
            pPrev->next = pNext->next;
 #if (CHIP_BTOVERLAY_SUPPORTED!=0)
        if(isOverlay)
        {
            pMemInfo->list_length_overlay-= pMem->length+sizeof(t_mem_block);
        }
        else
#endif
        {
            pMemInfo->list_length -= pMem->length+sizeof(t_mem_block);
        }
            return TRUE;
        }
        pPrev = pNext;
        pNext = pNext->next;
    }

    return FALSE;
}

#if 0
/******************************************************************************
 * mem_removeMemoryFromPool: 
 * DESCRIPTION: -     remove memory from the memory pool, such as cos_mem_info->mem_pool.
 *
 * Input: 
 *          pMem -  memory to be removed from the pool.
 * Output: 
 * Returns: 
 *          TRUE -  remove the memory successfully.
 *          FALSE - not find memory in the pool.
 * 
 * 
 ******************************************************************************/
BOOL mem_removeMemoryFromPool(t_mem_block *pMem)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    UINT32 blk_index;
    t_mem_block *pList;

    if(pMem->length < MEMORY_ALLOC_MIN_SIZE)
    {
        hal_DbgAssert("The memory size is less than MEMORY_ALLOC_MIN_SIZE");
    }

    if(pMem->length > MEMORY_BIG_SIZE)
        return FALSE;

    blk_index = ((pMem->length)/MEMORY_BLOCK_SIZE)-1;
    pList = pMemInfo->mem_pool[blk_index];

    if(pList == NULL)
    {
        return FALSE;
    }
    
    if(pList == pMem)
    {
        pMemInfo->mem_pool[blk_index] = pList->next;
        pMemInfo->pool_length -= pMem->length+sizeof(t_mem_block);
        return TRUE;
    }
    while(pList->next)
    {
        if(pList->next == pMem)
        {
            pList->next = pMem->next;
            pMemInfo->pool_length -= pMem->length+sizeof(t_mem_block);
            return TRUE;
        }
        pList = pList->next;
    }
    return FALSE;
}

/******************************************************************************
 * mem_addMemoryToPool: 
 * DESCRIPTION: -     add memory to the memory pool, such as cos_mem_info->mem_pool.
 *
 * Input: 
 *          pMem -  memory to be removed from the pool.
 * Output: 
 * Returns: 
 *          TRUE -  remove the memory successfully.
 *          FALSE - not find memory in the pool.
 * 
 * 
 ******************************************************************************/
VOID mem_addMemoryToPool(t_mem_block *pMem)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    UINT32 blk_index;

    if(pMem == NULL)
        return;

    if(pMem->length < MEMORY_ALLOC_MIN_SIZE)
    {
        hal_DbgAssert("The memory size is less than MEMORY_ALLOC_MIN_SIZE");
    }

    if(pMem->length > MEMORY_BIG_SIZE)
        return;
    
    blk_index = ((pMem->length)/MEMORY_BLOCK_SIZE)-1;
    pMem->next = pMemInfo->mem_pool[blk_index];
    pMemInfo->mem_pool[blk_index] = pMem;
    pMemInfo->pool_length += pMem->length+sizeof(t_mem_block);
}

t_mem_block *mem_getMemoryFromPool(UINT32 size)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem=NULL;
    UINT32 blk_index;

    if( (size == 0) || (size > MEMORY_BIG_SIZE) )
    {
        return NULL;
    }

    blk_index = ((size+MEMORY_BLOCK_SIZE-1)/MEMORY_BLOCK_SIZE)-1;
    pMem = pMemInfo->mem_pool[blk_index];
    if(pMem)
    {
        mem_removeMemoryFromPool(pMem);
        mem_removeMemoryFromAddressList(&(pMemInfo->addr_mem_list), pMem);
    }
    return pMem;
}
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
#if 0
VOID mem_printPool(VOID)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem;
    UINT32 i;

    hal_HstSendEvent(SYS_EVENT, 0xfee50500);

    for(i=0; i<MEMORY_POOL_SIZE; i++)
    {
        hal_HstSendEvent(SYS_EVENT, 0xfee50500+i);
        pMem = pMemInfo->mem_pool[i];
        while(pMem)
        {
            hal_HstSendEvent(SYS_EVENT, pMem);
            hal_HstSendEvent(SYS_EVENT, pMem->length);
            hal_HstSendEvent(SYS_EVENT, pMem->next);
            pMem = pMem->next;
        }
    }

    hal_HstSendEvent(SYS_EVENT, 0xfee50508);
    
}
#endif

VOID mem_printList(VOID)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem;

    hal_HstSendEvent(SYS_EVENT, 0xfee50510);

    pMem = pMemInfo->mem_list;
    while(pMem)
    {
        hal_HstSendEvent(SYS_EVENT, (UINT32)pMem);
        hal_HstSendEvent(SYS_EVENT, pMem->length);
        hal_HstSendEvent(SYS_EVENT, (UINT32)pMem->next);
        pMem = pMem->next;
    }
    
    hal_HstSendEvent(SYS_EVENT, 0xfee50511);

    pMem = pMemInfo->addr_mem_list;
    while(pMem)
    {
        hal_HstSendEvent(SYS_EVENT, (UINT32)pMem);
        hal_HstSendEvent(SYS_EVENT, pMem->length);
        hal_HstSendEvent(SYS_EVENT, (UINT32)pMem->addrNext);
        pMem = pMem->addrNext;
    }
    hal_HstSendEvent(SYS_EVENT, 0xfee50512);
}
#endif

/******************************************************************************
 * mem_mergeMemoryInAddressList: 
 * DESCRIPTION: -     merge new memory to be free with memories in the address list, such as
 *               cos_mem_info->addr_mem_list.
 *
 * Input: 
 *          ppList -  memory list.
 *          pMem -  memory to be merged with the list.
 * Output: 
 * Returns: 
 *          TRUE -  merge successfully.
 *          FALSE - merge fail.
 * 
 * 
 ******************************************************************************/
BOOL mem_mergeMemoryInAddressList(t_mem_block **ppAddrList, t_mem_block **ppMemList, t_mem_block **ppMem, BOOL isOverlay)
{
    //struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pPrev, *pCur, *pNext;

    if(*ppAddrList == NULL)
        return TRUE;

    pPrev = NULL;
    pCur = *ppAddrList;
    while((pCur->addrNext) && ((UINT32)(pCur->addrNext)<(UINT32)(*ppMem)))
    {
        pPrev = pCur;
        pCur = pCur->addrNext;
    }

    if((UINT32)(pCur)<(UINT32)(*ppMem))
    {
        pNext = pCur->addrNext;
    }
    else
    {
        pNext = pCur;
        pPrev = NULL;
        pCur = NULL;
    }

    if(pCur)
    {
        if( (UINT32)pCur + pCur->length + sizeof(t_mem_block) == (UINT32)(*ppMem))
        {
            //if( (!isOverlay) && (pCur->length <= MEMORY_BIG_SIZE) )
            //    mem_removeMemoryFromPool(pCur);
            //else
            //{
            mem_removeMemoryFromLengthList(ppMemList, pCur, isOverlay);
            //}
        
            pCur->length += (*ppMem)->length + sizeof(t_mem_block);
            if(pPrev)
                pPrev->addrNext = pCur->addrNext;
            else
                *ppAddrList = pCur->addrNext;

            pCur->next = NULL;
            pCur->addrNext = NULL;
            *ppMem = pCur;
            pCur = pPrev;
        }
    }

    if(pNext)
    {
        if( (UINT32)(*ppMem) + (*ppMem)->length + sizeof(t_mem_block) == (UINT32)pNext)
        {
            //if( (!isOverlay) && (pNext->length <= MEMORY_BIG_SIZE) )
            //    mem_removeMemoryFromPool(pNext);
            //else
            //{
            mem_removeMemoryFromLengthList(ppMemList, pNext, isOverlay);
            //}
        
            (*ppMem)->length += pNext->length + sizeof(t_mem_block);
            if(pCur)
                pCur->addrNext = pNext->addrNext;
            else
                *ppAddrList = pNext->addrNext;

            (*ppMem)->next = NULL;
            (*ppMem)->addrNext = NULL;
        }
    }

    return TRUE;
}

BOOL mem_splitMemory(t_mem_block **ppMem, t_mem_block **ppMemRemain, UINT32 size)
{
    if(*ppMem == NULL)
    {
        return FALSE;
    }

    if(  (*ppMem)->length < (size + MEMORY_BLOCK_SIZE + sizeof(t_mem_block)) )
        return FALSE;

    (*ppMemRemain) =  (t_mem_block *)((UINT32)(*ppMem) + size + sizeof(t_mem_block));
    (*ppMemRemain)->length =  (*ppMem)->length - size - sizeof(t_mem_block);
     (*ppMem)->length = size;

    return TRUE;
}

t_mem_block *mem_getMemoryFromLengthList(t_mem_block **ppMemList, t_mem_block **ppAddrList, UINT32 size, BOOL isOverlay)
{
    //struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem, *pMemRemain;
    BOOL ret;

    pMemRemain = NULL;
    pMem = *ppMemList;

    while(pMem)
    {
        if(pMem->length >= size)
        {
            if( pMem->length >= (size + MEMORY_BLOCK_SIZE + sizeof(t_mem_block)) )
            {
                mem_removeMemoryFromAddressList(ppAddrList, pMem);
                mem_removeMemoryFromLengthList(ppMemList, pMem, isOverlay);
                
                ret = mem_splitMemory(&pMem, &pMemRemain, size);
                if(ret)
                {
                    mem_addMemoryToAddressList(ppAddrList, pMemRemain);
                    //if((!isOverlay) && (pMemRemain->length<=MEMORY_BIG_SIZE) && !IS_UNCACHED_ADDR(pMemRemain))
                    //{
                    //    mem_addMemoryToPool(pMemRemain);
                    //}
                    //else
                    //{
                    mem_addMemoryToLengthList(ppMemList, pMemRemain, isOverlay);
                    //}
                    pMem->addrNext = NULL;
                    pMem->next = NULL;
                    return pMem;
                }
                else
                {
                    mem_addMemoryToAddressList(ppAddrList, pMem);
                    mem_addMemoryToLengthList(ppMemList, pMem, isOverlay);
                    
                    return NULL;
                }
            }
            else
            {
                mem_removeMemoryFromAddressList(ppAddrList, pMem);
                mem_removeMemoryFromLengthList(ppMemList, pMem, isOverlay);
                
                pMem->addrNext = NULL;
                pMem->next = NULL;
                return pMem;
            }
        }
        pMem = pMem->next;
    }
    return NULL;
}

#endif


#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
VOID mem_checkMemory(VOID)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    UINT32 calTotalMem = 0;
    //UINT32 calTotalPool = 0;
    UINT32 calTotalList = 0;
    UINT32 calTotalAddrList = 0;
    //UINT32 i;
    t_mem_block *pMem;

    // 1. check normal memory
    // a. check total normal memory length
    //calTotalMem = pMemInfo->alloc_length + pMemInfo->pool_length + pMemInfo->list_length;
    calTotalMem = pMemInfo->alloc_length + pMemInfo->list_length;
    if(calTotalMem != cos_mem_check.totalMem)
    {
        hal_HstSendEvent(SYS_EVENT, 0xfee50400);
        hal_DbgAssert("Total memory error.");
    }

#if 0
    //b. check every pool size
    for(i=0; i<MEMORY_POOL_SIZE; i++)
    {
        pMem = pMemInfo->mem_pool[i];
        while(pMem)
        {
            if( (pMem->length < (i+1)*32) || (pMem->length >= (i+2)*32) )
            {
                hal_HstSendEvent(SYS_EVENT, 0xfee50401);
                hal_DbgAssert("Memory pool error: pool size of %d is in pool[%d]", pMem->length, i);
            }
            calTotalPool += pMem->length + sizeof(t_mem_block);
            pMem = pMem->next;
        }
    }

    // c. check total memory pool length 
    if(calTotalPool != pMemInfo->pool_length)
    {
        hal_HstSendEvent(SYS_EVENT, 0xfee50402);
        hal_DbgAssert("Total memory pool error.");
    }
#endif

    // d. check total mem_list length
    pMem = pMemInfo->mem_list;
    while(pMem)
    {
        calTotalList+= pMem->length + sizeof(t_mem_block);
        pMem = pMem->next;
    }
    if(calTotalList != pMemInfo->list_length)
    {
        hal_HstSendEvent(SYS_EVENT, 0xfee50403);
        hal_DbgAssert("Total mem_list error.");
    }

    // e. check total addr_list length
    pMem = pMemInfo->addr_mem_list;
    while(pMem)
    {
        calTotalAddrList+= pMem->length + sizeof(t_mem_block);
        pMem = pMem->addrNext;
    }
    //if(calTotalAddrList != pMemInfo->pool_length + pMemInfo->list_length)
    if(calTotalAddrList != calTotalList)
    {
        hal_HstSendEvent(SYS_EVENT, 0xfee50404);
        hal_DbgAssert("Total addr_mem_list error.");
    }

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    // 2. check overlay memory
    // a. check total overlay memory length
    if( !(hal_OverlayIsLoaded(HAL_BTOVERLAY_BLUETOOTH)) )
    {
        calTotalMem = pMemInfo->alloc_length_overlay + pMemInfo->list_length_overlay;
        if(calTotalMem != cos_mem_check.totalMemOverlay)
        {
            hal_HstSendEvent(SYS_EVENT, 0xfee50405);
            hal_DbgAssert("Total overlay memory error.");
        }

        // b. check total mem_list_overlay length
        calTotalList = 0;
        pMem = pMemInfo->mem_list_overlay;
        while(pMem)
        {
            calTotalList += pMem->length + sizeof(t_mem_block);
            pMem = pMem->next;
        }
        if(calTotalList != pMemInfo->list_length_overlay)
        {
            hal_HstSendEvent(SYS_EVENT, 0xfee50406);
            hal_DbgAssert("Total mem_list_overlay error.");
        }

        // c. check total addr_list_overlay length
        calTotalAddrList = 0;
        pMem = pMemInfo->addr_list_overlay;
        while(pMem)
        {
            calTotalAddrList += pMem->length + sizeof(t_mem_block);
            pMem = pMem->addrNext;
        }
        if(calTotalAddrList != pMemInfo->list_length_overlay)
        {
            hal_HstSendEvent(SYS_EVENT, 0xfee50407);
            hal_DbgAssert("Total addr_list_overlay error.");
        }
    }
#endif  //#if (CHIP_BTOVERLAY_SUPPORTED!=0)
}
#endif  //#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS

VOID mem_fragmentStatisticsInit(VOID)
{
    cos_mem_usage.count = 0;
    cos_mem_usage.totalMem = cos_mem_info.list_length;
    
    cos_mem_usage.usedMaxMemory = 0;
    
    cos_mem_usage.usedSum = 0;
    //cos_mem_usage.poolSum = 0;
    cos_mem_usage.listSum = 0;
    
    cos_mem_usage.usedPercent = 0;
    //cos_mem_usage.poolPercent = 0;
    cos_mem_usage.listPercent = 100;

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    cos_mem_usage.totalMemOverlay= cos_mem_info.list_length_overlay;

    cos_mem_usage.usedMaxOverlayMemory = 0;
    
    cos_mem_usage.usedOverlaySum = 0;
    cos_mem_usage.listOverlaySum = 0;
    
    cos_mem_usage.usedOverlayPercent = 0;
    cos_mem_usage.listOverlayPercent = 100;
#endif

    cos_mem_usage.mallocTimeSum = 0;
    cos_mem_usage.freeTimeSum = 0;
    cos_mem_usage.mallocTimeAverage = 0;
    cos_mem_usage.freeTimeAverage = 0;
    cos_mem_usage.mallocCount = 0;
    cos_mem_usage.freeCount = 0;
    cos_mem_usage.mallocTimeMax = 0;
    cos_mem_usage.mallocTimeMaxSize = 0;
    cos_mem_usage.freeTimeMax = 0;
    cos_mem_usage.freeTimeMaxSize = 0;
    UINT16 i;
    for(i=0; i<17; i++)
    {
        cos_mem_usage.mallocTime[i] = 0;
    }

}


VOID mem_fragmentStatistics(VOID)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    UINT64 total;
    UINT16 i;

    if(pMemInfo->alloc_length > cos_mem_usage.usedMaxMemory)
    {
        cos_mem_usage.usedMaxMemory = pMemInfo->alloc_length;
    }

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    if(pMemInfo->alloc_length_overlay > cos_mem_usage.usedMaxOverlayMemory)
    {
        cos_mem_usage.usedMaxOverlayMemory = pMemInfo->alloc_length_overlay;
    }
#endif

    cos_mem_usage.usedSum += pMemInfo->alloc_length;
    //cos_mem_usage.poolSum += pMemInfo->pool_length;
    cos_mem_usage.listSum += pMemInfo->list_length;
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    cos_mem_usage.usedOverlaySum += pMemInfo->alloc_length_overlay;
    cos_mem_usage.listOverlaySum += pMemInfo->list_length_overlay;
#endif
    cos_mem_usage.count++;

    if(cos_mem_usage.count >= COS_MEM_STATISTIC_CAL_COUNT)
    {
        total = cos_mem_usage.usedSum* 1000;
        total /= COS_MEM_STATISTIC_CAL_COUNT*cos_mem_usage.totalMem;
        cos_mem_usage.usedPercent = (UINT16)total;
        //cos_mem_usage.heapPercent /= 10;

        //total = cos_mem_usage.poolSum * 1000;
        //total /= COS_MEM_STATISTIC_CAL_COUNT*cos_mem_usage.totalMem;
        //cos_mem_usage.poolPercent = (float)total;
        //cos_mem_usage.heapPercent /= 10;

        total = cos_mem_usage.listSum* 1000;
        total /= COS_MEM_STATISTIC_CAL_COUNT*cos_mem_usage.totalMem;
        cos_mem_usage.listPercent = (UINT16)total;
        //cos_mem_usage.heapPercent /= 10;

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
        total = cos_mem_usage.usedOverlaySum* 1000;
        total /= COS_MEM_STATISTIC_CAL_COUNT*cos_mem_usage.totalMemOverlay;
        cos_mem_usage.usedOverlayPercent = (UINT16)total;
        //cos_mem_usage.usedOverlayPercent /= 10;

        total = cos_mem_usage.listOverlaySum* 1000;
        total /= COS_MEM_STATISTIC_CAL_COUNT*cos_mem_usage.totalMemOverlay;
        cos_mem_usage.listOverlayPercent = (UINT16)total;
        //cos_mem_usage.listOverlayPercent /= 10;
#endif

        
        if(cos_mem_usage.mallocTimeSum > 0xffffffff)
            hal_DbgAssert("cos_mem_usage.mallocTimeSum overflow!");
        if(cos_mem_usage.freeTimeSum> 0xffffffff)
            hal_DbgAssert("cos_mem_usage.freeTimeSum overflow!");

        if(cos_mem_usage.mallocCount > 0)
        {
            total = cos_mem_usage.mallocTimeSum * 10;
            total /= cos_mem_usage.mallocCount;
            cos_mem_usage.mallocTimeAverage = total;
        }

        if(cos_mem_usage.freeCount > 0)
        {
            total = cos_mem_usage.freeTimeSum * 10;
            total /= cos_mem_usage.freeCount;
            cos_mem_usage.freeTimeAverage = total;
        }
        
        /*
        hal_HstSendEvent(SYS_EVENT, 0xfee71700);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.mallocTimeAverage);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.mallocTimeMax);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.mallocTimeMaxSize);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.freeTimeAverage);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.freeTimeMax);
        hal_HstSendEvent(SYS_EVENT, cos_mem_usage.freeTimeMaxSize);
        hal_HstSendEvent(SYS_EVENT, 0xfee71701);
        for(i=0; i<17; i++)
        {
            hal_HstSendEvent(SYS_EVENT, cos_mem_usage.mallocTime[i]);
        }
        hal_HstSendEvent(SYS_EVENT, 0xfee71702);
        */

        cos_mem_usage.count = 0;
        cos_mem_usage.usedSum = 0;
        //cos_mem_usage.poolSum = 0;
        cos_mem_usage.listSum = 0;
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
        cos_mem_usage.usedOverlaySum = 0;
        cos_mem_usage.listOverlaySum = 0;
#endif
        cos_mem_usage.mallocTimeSum = 0;
        cos_mem_usage.freeTimeSum = 0;
        cos_mem_usage.mallocCount = 0;
        cos_mem_usage.freeCount = 0;
        for(i=0; i<17; i++)
        {
            cos_mem_usage.mallocTime[i] = 0;
        }

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
        //CSW_TRACE(BASE_BAL_TS_ID, "COS_MEM_STATISTICS: maxLength %d, maxOverlayLength %d, used %d, pool %d, list %d, usedOverlay %d, listOverlay %d", 
        //    cos_mem_usage.usedMaxMemory, cos_mem_usage.usedMaxOverlayMemory, cos_mem_usage.usedPercent, cos_mem_usage.poolPercent, cos_mem_usage.listPercent, cos_mem_usage.usedOverlayPercent, cos_mem_usage.listOverlayPercent);

        CSW_TRACE(BASE_BAL_TS_ID, "COS_MEM_STATISTICS: maxLength %d, maxOverlayLength %d, used %d, list %d, usedOverlay %d, listOverlay %d", 
            cos_mem_usage.usedMaxMemory, cos_mem_usage.usedMaxOverlayMemory, cos_mem_usage.usedPercent, cos_mem_usage.listPercent, cos_mem_usage.usedOverlayPercent, cos_mem_usage.listOverlayPercent);
        //hal_HstSendEvent(SYS_EVENT, 0xadd70540);
        //hal_HstSendEvent(SYS_EVENT, cos_mem_usage.usedMaxOverlayMemory);
        //hal_HstSendEvent(SYS_EVENT, cos_mem_usage.usedPercent);
        //hal_HstSendEvent(SYS_EVENT, cos_mem_usage.usedOverlayPercent);
#else
        //CSW_TRACE(BASE_BAL_TS_ID, "COS_MEM_STATISTICS: used %d, pool %d, list %d.", cos_mem_usage.usedPercent, cos_mem_usage.poolPercent, cos_mem_usage.listPercent);
        CSW_TRACE(BASE_BAL_TS_ID, "COS_MEM_STATISTICS: used %d, list %d.", cos_mem_usage.usedPercent, cos_mem_usage.poolPercent, cos_mem_usage.listPercent);
#endif
    }
}
#endif  //#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS


PVOID COS_Calloc(UINT32 n, UINT32 size)
{
    return  COS_Malloc(n*size);
}

PVOID COS_Malloc(UINT32 size)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    UINT8 *ptr = NULL;
    t_mem_block *pMem = NULL; //*pNext;
    volatile UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
#ifndef COS_MEM_FRAGMENT_RECOVERY
    UINT32 blk_index;
#endif

    if(size == 0)
        hal_DbgAssert("Size is zero in COS_Malloc!");

#ifdef COS_MEM_TRACK
	if(s_cos_mem_track_caller)
	{
		nCallerAdd = s_cos_mem_track_caller;
		s_cos_mem_track_caller = 0;
	}
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70600);
    mem_checkMemory();
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    UINT32 startTime=0, endTime=0, interval=0;
    startTime = hal_TimGetUpTime();
#endif

    UINT32 csStatus = hal_SysEnterCriticalSection();
    CSW_TRACE(BASE_COS_TS_ID, "COS_Malloc size:%d", size);
    
#ifdef COS_MEM_FRAGMENT_RECOVERY
    size = ((size+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1))); // bytes aligned by MEMORY_BLOCK_SIZE
    //if( size <= MEMORY_BIG_SIZE )
    //{
    //   pMem = mem_getMemoryFromPool(size);
    //}

    //if(pMem == NULL)
    pMem = mem_getMemoryFromLengthList(&pMemInfo->mem_list, &pMemInfo->addr_mem_list, size, FALSE);

    if(pMem == NULL)
    {
#ifdef COS_MEM_TRACK
        COSMemTrackPrint();
#endif /* COS_MEM_TRACK */
        hal_DbgAssert("The memory heap overflow!");
    }
    
#else   //#ifdef COS_MEM_FRAGMENT_RECOVERY
    /* alloc in memory pool */
    if(size<=MEMORY_BIG_SIZE)
    {
        blk_index = (size-1)/MEMORY_BLOCK_SIZE;
        if(pMemInfo->mem_pool[blk_index] != NULL)
        {
            pMem = pMemInfo->mem_pool[blk_index];
            pMemInfo->mem_pool[blk_index] = pMem->next;
        }
    }
    else
    {
        pMem = find_mem_block(pMemInfo->mem_list, size);
        if(pMem != NULL && pMem == pMemInfo->mem_list)
        {
            pMemInfo->mem_list = pMem->next;
        }
    }
    if(pMem)
    {
        pMemInfo->fragment_length-=pMem->length+sizeof(t_mem_block);
    }
	
    if(pMem == NULL)
    {
        size = ((size+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1))); // 32 bytes aligned
        for(blk_index=0;blk_index<HEAP_COUNT;blk_index++)
        {
            pMem = alloc_mem_block(blk_index, size);
            if(pMem != NULL)
                break;
        }
        if(pMem == NULL)
        {
#ifdef COS_MEM_TRACK
        	COSMemTrackPrint();
#endif /* COS_MEM_TRACK */
            hal_DbgAssert("The memory heap overflow!");
        }
    }
#endif//#ifdef COS_MEM_FRAGMENT_RECOVERY

    if(pMem)
        pMemInfo->alloc_length += pMem->length+sizeof(t_mem_block);

    hal_SysExitCriticalSection(csStatus);
    
    pMem->next = NULL;
#ifdef COS_MEM_FRAGMENT_RECOVERY
    pMem->addrNext = NULL;
#endif
    ptr = (UINT8*)(pMem+1);
    
    /* clear the memory */
    if(ptr != NULL)
    {
        memset(ptr, 0, size);
#ifdef COS_MEM_TEST
		if(pMemInfo->alloc_list != NULL)
			pMemInfo->alloc_list->p2 = pMem;
		pMem->p1 = pMemInfo->alloc_list;
		pMem->p2 = NULL;
		pMemInfo->alloc_list = pMem;
		pMemInfo->alloc_length+=pMem->length+16;
#endif
    }
    else
    {
        hal_DbgAssert("ptr is NULL when has got memory from heap!");
    }

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    endTime = hal_TimGetUpTime();
    interval = endTime - startTime;
    cos_mem_usage.mallocTimeSum += interval;
    cos_mem_usage.mallocCount++;
    if(interval > cos_mem_usage.mallocTimeMax)
    {
        cos_mem_usage.mallocTimeMax = interval;
        cos_mem_usage.mallocTimeMaxSize = size;
    }
    
    if(interval > 0x0f)
    {
        cos_mem_usage.mallocTime[16]++;
    }
    else
    {
        cos_mem_usage.mallocTime[interval]++;
    }
    mem_fragmentStatistics();
#endif

#ifdef COS_MEM_TRACK
	COSMemTrackAlloc(nCallerAdd, (UINT32) ptr, pMem->length);
#else
    //CSW_TRACE(BASE_BAL_TS_ID, "COS_Malloc: %d at ptr=0x08%x, caller=0x%x\n", size, ptr, nCallerAdd);  
#endif /* COS_MEM_TRACK */
	//hal_HstSendEvent(SYS_EVENT,g_pHeapTop);


#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70601);
    mem_checkMemory();
#endif

	
    return ptr;
}

PVOID COS_Realloc(VOID *ptr, UINT32 nSize)
{
	void *ptemp = COS_Malloc(nSize);
	if(ptemp == NULL)
	{
		return NULL;
	}

	if(ptr != NULL)
	{
        t_mem_block *pMem;

        if(!cos_mem_info.uncache_independ)
            ptr = (VOID *)(TO_CACHED_ADDR(ptr));

        pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr)-sizeof(t_mem_block));

		memcpy(ptemp, ptr, MIN(nSize,pMem->length));
		COS_Free(ptr);
	}
	return ptemp;
}

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
PVOID SRVAPI COS_Malloc_NoCache ( UINT32 size)
{
#ifdef COS_MEM_TRACK
	COS_GET_RA(&s_cos_mem_track_caller);
#endif /* COS_MEM_TRACK */

	return COS_Malloc(size);
}
#else
PVOID SRVAPI COS_Malloc_NoCache ( UINT32 size)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem = NULL, *pNext;
    UINT8 *ptr = NULL;
    UINT32 blk_index;

    if(!pMemInfo->uncache_independ)
    {
#ifdef COS_MEM_TRACK
	COS_GET_RA(&s_cos_mem_track_caller);
#endif /* COS_MEM_TRACK */

        ptr = COS_Malloc(size);
        if(ptr)
            ptr = TO_UNCACHED_ADDR(ptr);
        return ptr;
    }

    UINT32 csStatus = hal_SysEnterCriticalSection();

    pMem = find_mem_block(pMemInfo->uncache_mem_list, size);
	if(pMem)
		pMemInfo->fragment_length-=pMem->length+sizeof(t_mem_block);
    if(pMem != NULL && pMem == pMemInfo->uncache_mem_list)
    {
        pMemInfo->uncache_mem_list = pMem->next;
    }

    if(pMem == NULL)
    {
        size = (size+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1)); // 32 bytes aligned
        for(blk_index=HEAP_COUNT-1;blk_index>0;blk_index--) // heap 0 can be used by uncache buffer
        {
            if(pMemInfo->heap_Size[blk_index] >= size+sizeof(t_mem_block))
            {
                pMem = alloc_mem_block(blk_index, size);
                if(pMem != NULL)
                    break;
            }
        }
        if(pMem == NULL)
            hal_DbgAssert("The memory heap overflow!");
    }

    hal_SysExitCriticalSection(csStatus);
    
    pMem->next = NULL;
    ptr = (UINT8*) TO_UNCACHED_ADDR(pMem+1);
    
    /* clear the memory */
    if(ptr != NULL)
    {
        memset(ptr, 0, size);
#ifdef COS_MEM_TEST
		if(pMemInfo->alloc_list != NULL)
			pMemInfo->alloc_list->p2 = pMem;
		pMem->p1 = pMemInfo->alloc_list;
		pMem->p2 = NULL;
		pMemInfo->alloc_list = pMem;
		pMemInfo->alloc_length+=pMem->length+16;
#endif
    }
    return ptr;
}
#endif

#if (CHIP_BTOVERLAY_SUPPORTED!=0)
#ifdef COS_MEM_FRAGMENT_RECOVERY

#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
BOOL mem_isOverlayMem(UINT32 addr)
{
    UINT8 i;
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    
    for(i=0; i<OVERLAY_MEMORY_BLOCK_NUM; i++)
    {
        if( ((UINT32)addr >= pMemInfo->overlay_mem_info[i].heap_start_addr) && ((UINT32)addr < pMemInfo->overlay_mem_info[i].heap_end_addr))
            return TRUE;
    }
    return FALSE;
}
#endif

PVOID SRVAPI COS_Malloc_BTOverlay( UINT32 size)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem = NULL;//, *pNext;
    UINT8 *ptr = NULL;
    UINT32 csStatus;
    volatile UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);

#ifdef COS_MEM_TRACK
	if(s_cos_mem_track_caller)
	{
		nCallerAdd = s_cos_mem_track_caller;
		s_cos_mem_track_caller = 0;
	}
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70602);
    mem_checkMemory();
#endif

    csStatus = hal_SysEnterCriticalSection();
    CSW_TRACE(BASE_COS_TS_ID, "COS_Malloc_BTOverlay size:%d", size);
    
    size = ((size+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1))); // bytes aligned by MEMORY_BLOCK_SIZE

    pMem = mem_getMemoryFromLengthList(&pMemInfo->mem_list_overlay, &pMemInfo->addr_list_overlay, size, TRUE);

#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
    if(pMem == NULL)
    {
        ptr = COS_Malloc(size);
        if(ptr != NULL)
        {
            pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr) -sizeof(t_mem_block));
            mem_addMemoryToAddressList(&pMemInfo->normal_mem_list_malloc_by_overlay, pMem);
#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
            //hal_HstSendEvent(SYS_EVENT, 0xfee70603);
            mem_checkMemory();
#endif
            return ptr;
        }
    }
#endif

    if(pMem == NULL)
    {
#ifdef COS_MEM_TRACK
        COSMemTrackPrint();
#endif /* COS_MEM_TRACK */
        hal_DbgAssert("The overlay heap overflow!");
    }

    if(pMem)
        pMemInfo->alloc_length_overlay += pMem->length+sizeof(t_mem_block);

    hal_SysExitCriticalSection(csStatus);

    pMem->next = NULL;
#ifdef COS_MEM_FRAGMENT_RECOVERY
    pMem->addrNext = NULL;
#endif
    ptr = (UINT8*)(pMem+1);
    
    /* clear the memory */
    if(ptr != NULL)
    {
        memset(ptr, 0, size);
#ifdef COS_MEM_TEST
		if(pMemInfo->overlay_list != NULL)
			pMemInfo->overlay_list->p2 = pMem;
		pMem->p1 = pMemInfo->overlay_list;
		pMem->p2 = NULL;
		pMemInfo->overlay_list = pMem;
		pMemInfo->alloc_length+=pMem->length+16;
#endif
    }
    else
    {
        hal_DbgAssert("ptr is NULL when has got memory from heap!");
    }

#ifdef COS_MEM_TRACK
    	COSMemTrackAlloc(nCallerAdd, (UINT32) ptr, pMem->length);
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    mem_fragmentStatistics();
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70604);
    mem_checkMemory();
#endif
    return ptr;
}
BOOL SRVAPI COS_Free_BTOverlay(PVOID ptr)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;

    t_mem_block *pMem;
    //t_mem_block *pNext;
    UINT32 csStatus;
    //BOOL header;

    if(ptr == NULL)
        return FALSE;
    
#ifdef COS_MEM_TRACK
    UINT32  nSize;
    volatile UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70605);
    mem_checkMemory();
#endif

#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
    if( !mem_isOverlayMem((UINT32)ptr) )
    {
        pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr) -sizeof(t_mem_block));
        mem_removeMemoryFromAddressList(&pMemInfo->normal_mem_list_malloc_by_overlay, pMem);
        pMem->addrNext = NULL;
        if( COS_Free(ptr) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
#endif


    //if(!pMemInfo->uncache_independ)
    //    ptr = TO_CACHED_ADDR(ptr);

    pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr) -sizeof(t_mem_block));
    pMemInfo->alloc_length_overlay-= pMem->length+sizeof(t_mem_block);
#ifdef COS_MEM_TRACK
	if(pMem)
	{
		nSize = pMem->length;
	}
#endif /* COS_MEM_TRACK */
#ifdef COS_MEM_TEST
	{
		pMemInfo->alloc_length-=pMem->length+16;
		if(pMem->p2 != NULL)
			 pMem->p2->p1 = pMem->p1;
		if(pMem->p1 != NULL)
			 pMem->p1->p2 = pMem->p2;
		if(pMemInfo->alloc_list == pMem)
			pMemInfo->alloc_list = pMem->p1;
		pMem->p1 = NULL;
		pMem->p2 = NULL;
	}
#endif


    csStatus = hal_SysEnterCriticalSection();

//#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee50412);
    //mem_printPool();
    //mem_printList();
//#endif

    mem_mergeMemoryInAddressList(&pMemInfo->addr_list_overlay, &pMemInfo->mem_list_overlay, &pMem, TRUE);

//#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee50413);
    //mem_printPool();
    //mem_printList();
//#endif

    if(pMem->next != NULL)
    {
        hal_DbgAssert("The free memory may already be freed 0x%x!", pMem);
        return TRUE;
    }
    
    //put memory to pool or list
    if(pMem)
    {
        mem_addMemoryToAddressList(&pMemInfo->addr_list_overlay, pMem);
        mem_addMemoryToLengthList(&pMemInfo->mem_list_overlay, pMem, TRUE);
    }
    hal_SysExitCriticalSection(csStatus);


#ifdef COS_MEM_TRACK
	COSMemTrackFree(nCallerAdd, (UINT32) ptr, nSize);
#else
    //CSW_TRACE(BASE_BAL_TS_ID, "COS_Free: free memory at ptr=0x08%x\n", ptr);  
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70606);
    mem_checkMemory();
#endif

    
  return FALSE;
}

VOID COS_BTOverlay_Free_All(void)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem, *pNext;
    UINT8 *ptr = NULL;
    UINT8 i;

#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
    //free normal memory that malloc by btOverlay
    pMem = pMemInfo->normal_mem_list_malloc_by_overlay;
    while(pMem)
    {
        pNext = pMem->addrNext;
        mem_removeMemoryFromAddressList(&pMemInfo->normal_mem_list_malloc_by_overlay, pMem);
        pMem->addrNext = NULL;
        ptr = (UINT8*)(pMem+1);
        COS_Free(ptr);
        pMem = pNext;
    }
    pMemInfo->normal_mem_list_malloc_by_overlay = NULL;
#endif


    pMemInfo->mem_list_overlay = NULL;
    pMemInfo->addr_list_overlay = NULL;
    pMemInfo->alloc_length_overlay = 0;
    pMemInfo->list_length_overlay = 0;

    for(i=0; i<OVERLAY_MEMORY_BLOCK_NUM; i++)
    {
        
        
        pMem = (t_mem_block *)(pMemInfo->overlay_mem_info[i].heap_start_addr);
        pMem->next = NULL;
        pMem->addrNext = NULL;
        pMem->length = pMemInfo->overlay_mem_info[i].heap_end_addr - pMemInfo->overlay_mem_info[i].heap_start_addr - sizeof(t_mem_block);

        mem_addMemoryToLengthList(&(pMemInfo->mem_list_overlay), pMem, TRUE);

        mem_addMemoryToAddressList(&(pMemInfo->addr_list_overlay), pMem);
        memset(pMem+1, 0x00, pMem->length);
    }
#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70607);
    mem_checkMemory();
#endif
}
#else   //#ifdef COS_MEM_FRAGMENT_RECOVERY
PVOID SRVAPI COS_Malloc_BTOverlay( UINT32 size)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;
    t_mem_block *pMem = NULL, *pNext;
    UINT8 *ptr = NULL;
    UINT32 csStatus;
	volatile UINT32  nCallerAdd = 0x00;
	COS_GET_RA(&nCallerAdd);

    csStatus = hal_SysEnterCriticalSection();

    pMem = find_mem_block(pMemInfo->overlay_list, size);

    if(pMem != NULL && pMem == pMemInfo->overlay_list)
    {
        pMemInfo->overlay_list = pMem->next;
    }

    if(pMem == NULL)
    {
        size = (size+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1)); // 32 bytes aligned
        if(pMemInfo->heap_Size[HEAP_COUNT] >= size+sizeof(t_mem_block))
        {
            pMem = alloc_mem_block(HEAP_COUNT, size);
        }
        if(pMem == NULL)
            hal_DbgAssert("The overaly heap overflow!");
    }

    hal_SysExitCriticalSection(csStatus);
    
    pMem->next = NULL;
    ptr = (UINT8*)(pMem+1);
    
    /* clear the memory */
    if(ptr != NULL)
    {
        memset(ptr, 0, size);
#ifdef COS_MEM_TEST
		if(pMemInfo->overlay_list != NULL)
			pMemInfo->overlay_list->p2 = pMem;
		pMem->p1 = pMemInfo->overlay_list;
		pMem->p2 = NULL;
		pMemInfo->overlay_list = pMem;
		pMemInfo->alloc_length+=pMem->length+16;
#endif
#ifdef COS_MEM_TRACK
    	COSMemTrackAlloc(nCallerAdd, (UINT32) ptr, pMem->length);
#endif
    }
    return ptr;
}

BOOL SRVAPI COS_Free_BTOverlay(PVOID ptr)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;

    t_mem_block *pMem;
    t_mem_block *pNext;
    UINT32 csStatus;
    BOOL header;
    
#ifdef COS_MEM_TRACK
	UINT32  nSize;
	volatile UINT32  nCallerAdd = 0x00;
	COS_GET_RA(&nCallerAdd);
#endif /* COS_MEM_TRACK */

    if(ptr == NULL)
        return FALSE;

    pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr) -sizeof(t_mem_block));
#ifdef COS_MEM_TRACK
	if(pMem)
	{
		nSize = pMem->length;
	}
#endif /* COS_MEM_TRACK */
#ifdef COS_MEM_TEST
	{
		pMemInfo->alloc_length-=pMem->length+16;
		if(pMem->p2 != NULL)
			 pMem->p2->p1 = pMem->p1;
		if(pMem->p1 != NULL)
			 pMem->p1->p2 = pMem->p2;
		if(pMemInfo->overlay_list == pMem)
			pMemInfo->overlay_list = pMem->p1;
		pMem->p1 = NULL;
		pMem->p2 = NULL;
	}
#endif

    {
        if(pMem->next != NULL)
        {
            hal_DbgAssert("The free memory may already be freed 0x%x!", pMem);
            return TRUE;
        }

        //COS_WaitForSemaphore(pMemInfo->mem_mutex, COS_WAIT_FOREVER);
        csStatus = hal_SysEnterCriticalSection();

        {
            if(pMemInfo->pHeap_Base[HEAP_COUNT] == ((UINT8*)ptr+pMem->length))
            {
                pMemInfo->pHeap_Base[HEAP_COUNT] -= (pMem->length+sizeof(t_mem_block))>>2;
                pMemInfo->heap_Size[HEAP_COUNT] += (pMem->length+sizeof(t_mem_block));
                //CSW_TRACE(BASE_COS_TS_ID, "COS_Free return memory block 0x%x to heap[%d], pHeap_Base=%x,heap_size=%d", ptr, blk_index, pMemInfo->pHeap_Base[blk_index], pMemInfo->heap_Size[blk_index]);
                pMem = NULL;
            }
        }

        if(pMem != NULL)
        {
        	//pMemInfo->free_length+=pMem->length+16;
            pNext = pMemInfo->overlay_list;
            
            if(pNext == NULL)
            {
                header = TRUE;
                pMem->next = NULL;
            }
            else
            {
                while(pNext->next && pNext ->next->length < pMem->length)
                {
                    //pAssert(pNext != pMem);
                    pNext = pNext->next;
                };
                
                if(pNext ->length < pMem->length)
                {
                    header = FALSE;
                    pMem->next = pNext->next;
                    pNext->next = pMem;
                }
                else
                {
                    header = TRUE;
                    pMem->next = pNext;
                }
            }
            if(header)
            {
                    pMemInfo->overlay_list = pMem;
            }
                
            //COS_ReleaseSemaphore(pMemInfo->mem_mutex);
            
        }
        hal_SysExitCriticalSection(csStatus);
    }


#ifdef COS_MEM_TRACK
	COSMemTrackFree(nCallerAdd, (UINT32) ptr, nSize);
#else
    CSW_TRACE(BASE_BAL_TS_ID, "COS_Free: free memory at ptr=0x08%x\n", ptr);  
#endif /* COS_MEM_TRACK */

  return FALSE;
}

VOID COS_BTOverlay_Free_All(void)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    pMemInfo->pHeap_Base[3] = &_dualport_heap_start;
#else
    pMemInfo->pHeap_Base[3] = &_sys_sram_heap_start;
#endif
    pMemInfo->heap_Size[3] = pMemInfo->overlay_heap_size;
    pMemInfo->overlay_list = NULL;
}
#endif  //#ifdef COS_MEM_FRAGMENT_RECOVERY
#endif  //#if (CHIP_BTOVERLAY_SUPPORTED!=0)

BOOL COS_Free(PVOID ptr)
{
    struct t_host_mem_info *pMemInfo = &cos_mem_info;

    t_mem_block *pMem;
    UINT32 csStatus;
#ifndef COS_MEM_FRAGMENT_RECOVERY
    t_mem_block *pNext;
    UINT32 blk_index;
    BOOL header;
#endif
    
#ifdef COS_MEM_TRACK
	UINT32  nSize;
	volatile UINT32  nCallerAdd = 0x00;
	COS_GET_RA(&nCallerAdd);
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70608);
    mem_checkMemory();
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    UINT32 startTime=0, endTime=0, interval=0, size = 0;
    startTime = hal_TimGetUpTime();
#endif

    if(ptr == NULL)
        return FALSE;

    if( (UINT32)(ptr) < sizeof(t_mem_block) )
        hal_DbgAssert("The address: 0x%x is not correct in COS_Free", ptr);

    if(!pMemInfo->uncache_independ)
        ptr = (PVOID)(TO_CACHED_ADDR(ptr));
    
    pMem = (t_mem_block*)(TO_CACHED_ADDR(ptr) -sizeof(t_mem_block));

    if( pMem->length == 0)
        hal_DbgAssert("The pMem->length is zero in COS_Free.");

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    size = pMem->length;
#endif
    
    pMemInfo->alloc_length -= pMem->length+sizeof(t_mem_block);
#ifdef COS_MEM_TRACK
	if(pMem)
	{
		nSize = pMem->length;
	}
#endif /* COS_MEM_TRACK */
#ifdef COS_MEM_TEST
	{
		pMemInfo->alloc_length-=pMem->length+16;
		if(pMem->p2 != NULL)
			 pMem->p2->p1 = pMem->p1;
		if(pMem->p1 != NULL)
			 pMem->p1->p2 = pMem->p2;
		if(pMemInfo->alloc_list == pMem)
			pMemInfo->alloc_list = pMem->p1;
		pMem->p1 = NULL;
		pMem->p2 = NULL;
	}
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY
    csStatus = hal_SysEnterCriticalSection();

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee50412);
    //mem_printPool();
    //mem_printList();
#endif

    if(IS_UNCACHED_ADDR(pMem))
        mem_mergeMemoryInAddressList(&pMemInfo->addr_mem_list, &pMemInfo->uncache_mem_list, &pMem, FALSE);
    else
        mem_mergeMemoryInAddressList(&pMemInfo->addr_mem_list, &pMemInfo->mem_list, &pMem, FALSE);


#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee50413);
    //mem_printPool();
    //mem_printList();
#endif

    if(pMem->next != NULL)
    {
        hal_DbgAssert("The free memory may already be freed 0x%x!", pMem);
        return TRUE;
    }
    
    //put memory to pool or list
    if(pMem)
    {
        mem_addMemoryToAddressList(&pMemInfo->addr_mem_list, pMem);
        //if(pMem->length<=MEMORY_BIG_SIZE && !IS_UNCACHED_ADDR(ptr))
        //{
        //    mem_addMemoryToPool(pMem);
        //}
        //else
        //{
        if(IS_UNCACHED_ADDR(ptr))
            mem_addMemoryToLengthList(&pMemInfo->uncache_mem_list, pMem, FALSE);
        else
            mem_addMemoryToLengthList(&pMemInfo->mem_list, pMem, FALSE);

        //}
    }
    hal_SysExitCriticalSection(csStatus);

#else   //#ifdef COS_MEM_FRAGMENT_RECOVERY
    /* memory in pool */
    if(pMem->length<=MEMORY_BIG_SIZE && !IS_UNCACHED_ADDR(ptr))
    {
        blk_index = (pMem->length)/MEMORY_BLOCK_SIZE-1;
        csStatus = hal_SysEnterCriticalSection();
        pMem->next = pMemInfo->mem_pool[blk_index];
        pMemInfo->mem_pool[blk_index] = pMem;
        pMemInfo->fragment_length+=pMem->length+sizeof(t_mem_block);
        hal_SysExitCriticalSection(csStatus);
    }
    else
    {
        if(pMem->next != NULL)
        {
            hal_DbgAssert("The free memory may already be freed 0x%x!", pMem);
            return TRUE;
        }

        //COS_WaitForSemaphore(pMemInfo->mem_mutex, COS_WAIT_FOREVER);
        csStatus = hal_SysEnterCriticalSection();

        for(blk_index=0;blk_index<HEAP_COUNT;blk_index++)
        {
            if(pMemInfo->pHeap_Base[blk_index] == ((UINT8*)ptr+pMem->length))
            {
                pMemInfo->pHeap_Base[blk_index] -= (pMem->length+sizeof(t_mem_block))>>2;
                pMemInfo->heap_Size[blk_index] += (pMem->length+sizeof(t_mem_block));
                //CSW_TRACE(BASE_COS_TS_ID, "COS_Free return memory block 0x%x to heap[%d], pHeap_Base=%x,heap_size=%d", ptr, blk_index, pMemInfo->pHeap_Base[blk_index], pMemInfo->heap_Size[blk_index]);
                pMem = NULL;
                break;
            }
        }

        if(pMem != NULL)
        {
        	pMemInfo->fragment_length+=pMem->length+sizeof(t_mem_block);
            if(IS_UNCACHED_ADDR(ptr))
                pNext = pMemInfo->uncache_mem_list;
            else
                pNext = pMemInfo->mem_list;
            
            if(pNext == NULL)
            {
                header = TRUE;
                pMem->next = NULL;
            }
            else
            {
                while(pNext->next && pNext ->next->length < pMem->length)
                {
                    //pAssert(pNext != pMem);
                    pNext = pNext->next;
                };
                
                if(pNext ->length < pMem->length)
                {
                    header = FALSE;
                    pMem->next = pNext->next;
                    pNext->next = pMem;
                }
                else
                {
                    header = TRUE;
                    pMem->next = pNext;
                }
            }
            if(header)
            {
                if(IS_UNCACHED_ADDR(ptr))
                    pMemInfo->uncache_mem_list = pMem;
                else
                    pMemInfo->mem_list = pMem;
            }
                
            //COS_ReleaseSemaphore(pMemInfo->mem_mutex);
            
        }
        hal_SysExitCriticalSection(csStatus);
    }
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    endTime = hal_TimGetUpTime();
    interval = endTime - startTime;
    cos_mem_usage.freeTimeSum += interval;
    cos_mem_usage.freeCount++;
    if(interval > cos_mem_usage.freeTimeMax)
    {
        cos_mem_usage.freeTimeMax = interval;
        cos_mem_usage.freeTimeMaxSize = size;
    }
#endif

#ifdef COS_MEM_TRACK
	COSMemTrackFree(nCallerAdd, (UINT32) ptr, nSize);
#else
    CSW_TRACE(BASE_BAL_TS_ID, "COS_Free: free memory at ptr=0x08%x\n", ptr);  
#endif /* COS_MEM_TRACK */

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee70609);
    mem_checkMemory();
#endif


  return FALSE;
}
BOOL COS_FreeSetPtrNull(PVOID *ptr)
{
	BOOL ret = COS_Free(*ptr);
	*ptr = NULL;
	return ret;
}

PVOID  COS_MallocDbg (UINT32 nSize, CONST UINT8* pszFileName, UINT32 nLine)
{
	UINT8* p = COS_Malloc(nSize);
    
	CSW_TRACE(BASE_BAL_TS_ID, "COS_MallocDbg+: 0x%08x, size %d, %s[%d] \n", p, nSize, pszFileName, nLine);
    
    return (PVOID)p;
}

BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine)
{
    CSW_TRACE(BASE_BAL_TS_ID, "COS_FreeDbg-: 0x%08x, %s[%d] \n", pMemAddr, pszFileName, nLine);
    
    return COS_Free(pMemAddr);
}

#ifndef COS_MEM_FRAGMENT_RECOVERY
/******************************************************************************
 * COS_PreAlloc: 
 * DESCRIPTION: - allocate memory and save in mem_pool to reduce malloc time.
 * allocate nSize*nCount memory totaly
 * Input: nSize - memory size 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL COS_PreAlloc(UINT32 nSize, UINT32 nCount)
{
    UINT32 blk_index;
    t_mem_block *pMem = NULL;

    if((0 == nSize) || (0 == nCount))
	    return TRUE;

    nSize = ((nSize+MEMORY_BLOCK_SIZE-1)&(~(MEMORY_BLOCK_SIZE-1))); // 32 bytes aligned

    while(nCount-- > 0)
    {
	UINT32 csStatus = hal_SysEnterCriticalSection();
	for(blk_index=0;blk_index<HEAP_COUNT;blk_index++)
	{
	    pMem = alloc_mem_block(blk_index, nSize);
	    if(pMem != NULL)
		break;
	}

	if(pMem == NULL)
	{
	    hal_DbgAssert("The memory heap overflow!");
	}

	hal_SysExitCriticalSection(csStatus);

	pMem->next = NULL;
	COS_Free(pMem+1);
    }

    return TRUE;
}
#endif

VOID COS_MemInit()
{
    VOLATILE UINT32 heap_size;
    t_mem_block *pMem;
    //extern void *(*g_pMalloc_ROM)(size_t size);
    //extern void (*g_pFree_ROM)(void *buffer);
    extern UINT32 SYSMain_Get_DataBuff_Size(UINT32 start_addr_of_databuf);
    
    memset(&cos_mem_info, 0, sizeof(cos_mem_info));

    heap_size = (UINT32)&_heap_size;

#ifdef COS_MEM_FRAGMENT_RECOVERY
    if(heap_size == 0) // no external memory
    {
        cos_mem_info.uncache_independ = TRUE;
        pMem = (t_mem_block *)(&_bcpu_ram_heap_start);
        pMem->next = NULL;
        pMem->addrNext = NULL;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
        pMem->length = 0;
        cos_mem_info.uncache_independ = FALSE;
#else
        pMem->length = (UINT32)(((UINT32)(&_bcpu_ram_heap_size)>sizeof(t_mem_block))?((&_bcpu_ram_heap_size)-sizeof(t_mem_block)):0);
#endif
    }
    else
    {
        pMem = (t_mem_block*)&_heap_start;
        pMem->next = NULL;
        pMem->addrNext = NULL;
        pMem->length = ((UINT32)&_heap_size);
    }
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list), pMem, FALSE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_mem_list), pMem);
    }
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    pMem = NULL;
#else
    pMem = (t_mem_block *)(&_dualport_heap_start);
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length = 0;	//&_dualport_heap_size
#endif
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list), pMem, FALSE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_mem_list), pMem);
    }


    pMem = (t_mem_block *)(&_sys_sram_heap_start);
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length = (UINT32)(&_sys_sram_heap_size);
#ifndef _T_UPGRADE_PROGRAMMER
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    hal_HstSendEvent(BOOT_EVENT, 0xfee71300);
    heap_size = SYSMain_Get_DataBuff_Size((UINT32)(&_sys_sram_heap_start));
    heap_size = (heap_size+4)&~3;
    hal_HstSendEvent(BOOT_EVENT, heap_size);
    hal_HstSendEvent(BOOT_EVENT, (UINT32)&_sys_sram_heap_start);
    pMem = (t_mem_block *)((UINT32)(&_sys_sram_heap_start) + heap_size);
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length  = (UINT32)(&_sys_sram_heap_size) - heap_size;
    
    cos_mem_info.overlay_mem_info[0].heap_start_addr = (UINT32)(&_sys_sram_heap_start);
    cos_mem_info.overlay_mem_info[0].heap_end_addr = (UINT32)(&_sys_sram_heap_start) + heap_size;

#endif
#endif
    //hal_HstSendEvent(BOOT_EVENT, 0xadd70405);
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list), pMem, FALSE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_mem_list), pMem);
    }

#ifndef _T_UPGRADE_PROGRAMMER
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    pMem = (t_mem_block *)(cos_mem_info.overlay_mem_info[0].heap_start_addr);
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length = cos_mem_info.overlay_mem_info[0].heap_end_addr - cos_mem_info.overlay_mem_info[0].heap_start_addr;
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list_overlay), pMem, TRUE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_list_overlay), pMem);
    }

    heap_size = (UINT32)(&_bss_btoverlay_end) - (UINT32)(&_bss_music_overlay_end);
    heap_size = (heap_size)&~3;
    pMem = (t_mem_block *)(&_bss_music_overlay_end);
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length = heap_size;
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list_overlay), pMem, TRUE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_list_overlay), pMem);
        cos_mem_info.overlay_mem_info[1].heap_start_addr = (UINT32)(&_bss_music_overlay_end);
        cos_mem_info.overlay_mem_info[1].heap_end_addr = (UINT32)(&_bss_btoverlay_end);
    }
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    pMem = (t_mem_block*)&_dualport_heap_start;
    pMem->next = NULL;
    pMem->addrNext = NULL;
    pMem->length = (UINT32)&_dualport_heap_size;
    if( (pMem != NULL) && (pMem->length > sizeof(t_mem_block)) )
    {
        pMem->length -= sizeof(t_mem_block);
        mem_addMemoryToLengthList(&(cos_mem_info.mem_list_overlay), pMem, TRUE);
        mem_addMemoryToAddressList(&(cos_mem_info.addr_list_overlay), pMem);
        cos_mem_info.overlay_mem_info[0].heap_start_addr = (UINT32)(&_dualport_heap_start);
        cos_mem_info.overlay_mem_info[0].heap_end_addr = (UINT32)(&_dualport_heap_start) + (UINT32)(&_dualport_heap_size);
        cos_mem_info.alloc_length_overlay = 0;
        cos_mem_info.list_length_overlay = (UINT32)&_dualport_heap_size;
    }
#endif
#endif
#endif
    
#else   //#ifdef COS_MEM_FRAGMENT_RECOVERY

    if(heap_size == 0) // no external memory
    {
        cos_mem_info.uncache_independ = TRUE;
        cos_mem_info.pHeap_Base[0] = &_bcpu_ram_heap_start;
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
        cos_mem_info.heap_Size[0] = 0;
        cos_mem_info.uncache_independ = FALSE;
#else
        cos_mem_info.heap_Size[0] = &_bcpu_ram_heap_size;
#endif
    }
    else
    {
        cos_mem_info.uncache_independ = FALSE;
        cos_mem_info.pHeap_Base[0] = (UINT32*)&_heap_start;
        cos_mem_info.heap_Size[0] = (UINT32)&_heap_size;
    }
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    cos_mem_info.heap_Size[1] = 0;
#else
    cos_mem_info.pHeap_Base[1] = &_dualport_heap_start;
    cos_mem_info.heap_Size[1] = 0;//&_dualport_heap_size;
#endif

    cos_mem_info.pHeap_Base[2] = &_sys_sram_heap_start;
    cos_mem_info.heap_Size[2] = &_sys_sram_heap_size;
#ifndef _T_UPGRADE_PROGRAMMER
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    heap_size = SYSMain_Get_DataBuff_Size(&_sys_sram_heap_start);
    heap_size = (heap_size+4)&~3;
    cos_mem_info.pHeap_Base[2] += heap_size/4;
    cos_mem_info.heap_Size[2]  -= heap_size;

    cos_mem_info.pHeap_Base[3] = &_sys_sram_heap_start;
    cos_mem_info.heap_Size[3] = heap_size;
    #if (CHIP_BTOVERLAY_SUPPORTED!=0)
    cos_mem_info.overlay_heap_size = heap_size;
    #endif
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    cos_mem_info.pHeap_Base[3] = &_dualport_heap_start;
    cos_mem_info.heap_Size[3] = &_dualport_heap_size;
    #if (CHIP_BTOVERLAY_SUPPORTED!=0)
    cos_mem_info.overlay_heap_size = &_dualport_heap_size;
    #endif
#endif
#endif
#endif //#ifdef COS_MEM_FRAGMENT_RECOVERY
    cos_mem_info.mem_mutex = NULL;//COS_CreateSemaphore(1);

    //g_pMalloc_ROM = COS_Malloc;
    //g_pFree_ROM = COS_Free;

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    cos_mem_check.totalMem = cos_mem_info.list_length;
    cos_mem_check.totalMemOverlay = cos_mem_info.list_length_overlay;
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_STATISTICS
    mem_fragmentStatisticsInit();
#endif

#ifdef COS_MEM_MALLOC_NORMAL_MEMORY_IF_NO_OVERLAY_MEMORY
    cos_mem_info.normal_mem_list_malloc_by_overlay = NULL;
#endif

#ifdef COS_MEM_FRAGMENT_RECOVERY_CHECK
    //hal_HstSendEvent(SYS_EVENT, 0xfee7060a);
    mem_checkMemory();
#endif
}


// =============================================================================
// COS_PageProtectSetup
// -----------------------------------------------------------------------------
/// This function setups a protection page
///
/// @param nPageNum Name of the page we want to configure  
/// @param nMode Protection mode
/// @param nStartAddr Address of the beginning of the page
/// @param nEndAddr  End address of the page. This address is not included in 
/// the page
// =============================================================================  
PUBLIC UINT32 COS_PageProtectSetup( COS_PAGE_NUM nPageNum, COS_PAGE_SPY_MODE nMode, UINT32 nStartAddr, UINT32 nEndAddr )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 )|| ( nMode < COS_PAGE_NO_TRIGGER) || ( nMode > COS_PAGE_READWRITE_TRIGGER ))
  {
    return ERR_INVALID_PARAMETER;
  }
  
  hal_DbgPageProtectSetup( nPageNum, nMode, nStartAddr, nEndAddr );

  return ERR_SUCCESS;
}


// =============================================================================
// COS_PageProtectEnable
// -----------------------------------------------------------------------------
/// Enable the protection of a given page.
///
/// @param nPageNum Page to enable
// =============================================================================
PUBLIC UINT32 COS_PageProtectEnable( COS_PAGE_NUM nPageNum )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 ))
  {
    return ERR_INVALID_PARAMETER;
  }
  
  hal_DbgPageProtectEnable(nPageNum);

  return ERR_SUCCESS;
}


// =============================================================================
// COS_PageProtectDisable
// -----------------------------------------------------------------------------
/// Disable the protection of a given page
///
/// @param nPageNum Page to disable
// =============================================================================

PUBLIC UINT32 COS_PageProtectDisable( COS_PAGE_NUM nPageNum )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 ))
  {
    return ERR_INVALID_PARAMETER;
  }

  hal_DbgPageProtectDisable(nPageNum);

  return ERR_SUCCESS;
}


#if defined(_DEBUG)

 #define  COS_MEMCHK_MEMLIST_SUMMARY(p_memBlock, cos_mem_summary)   while(p_memBlock){ \
	      		if(p_memBlock >= &_sys_sram_heap_start)\
	      		{\
	      			cos_mem_summary[2] += p_memBlock->length;\
	      		}\
	      		else if(p_memBlock >= &_dualport_heap_start)\
	      		{\
	      			cos_mem_summary[1] += p_memBlock->length;\
	      		}\
	      		else if(p_memBlock >= &_bcpu_ram_heap_start)\
	      		{\
	      			cos_mem_summary[0] += p_memBlock->length;\
	      		}\
	      		p_memBlock = p_memBlock->next;	      	\
	      		}

	      	
/******************************************************************************
 * COS_MEMChkGetSummary: 
 * DESCRIPTION: - 
 * Get each heap's free memory summary
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 * --------------------
 *    2014/09/17, huangyuxuan create
 ******************************************************************************/
PRIVATE void  COS_MEMChkGetSummary (UINT32 *cos_mem_summary)
{
      struct t_host_mem_info *pMemInfo = &cos_mem_info;
      t_mem_block *p_memBlock;
      UINT16 i;

	//heap size
      cos_mem_summary[0] = pMemInfo->heap_Size[0];
      cos_mem_summary[1] = pMemInfo->heap_Size[1];
      cos_mem_summary[2] = pMemInfo->heap_Size[2];

	//mem pool
      for( i = 0; i < MEMORY_POOL_SIZE; i++)
      {
	      p_memBlock = pMemInfo->mem_pool[i];

	      COS_MEMCHK_MEMLIST_SUMMARY(p_memBlock, cos_mem_summary);
      }

      //mem list
      p_memBlock = pMemInfo->mem_list;
      COS_MEMCHK_MEMLIST_SUMMARY(p_memBlock, cos_mem_summary);

      //mem list
      p_memBlock = pMemInfo->uncache_mem_list;
      COS_MEMCHK_MEMLIST_SUMMARY(p_memBlock, cos_mem_summary);

 }

UINT32 cos_mem_chk_count = 0;
void  _COS_MEMChkEntry (CONST UINT8* pszFileName, UINT32 nLine)
{
      UINT32 cos_mem_summary[HEAP_COUNT];

      COS_MEMChkGetSummary(cos_mem_summary);
    
	CSW_TRACE(CFW_API_TS_ID, "==MChkEntry: %s[%d](%d), %d,%d,%d \n", pszFileName, nLine, cos_mem_chk_count++,
	cos_mem_summary[0], cos_mem_summary[1], cos_mem_summary[2]);
    
}

void _COS_MEMChkExit (CONST UINT8* pszFileName, UINT32 nLine)
{
      UINT32 cos_mem_summary[HEAP_COUNT];

      COS_MEMChkGetSummary(cos_mem_summary);

	CSW_TRACE(CFW_API_TS_ID, "==MChkExit: %s[%d](%d), %d,%d,%d \n", pszFileName, nLine, cos_mem_chk_count--,
	cos_mem_summary[0], cos_mem_summary[1], cos_mem_summary[2]);
    
}
#endif /* _DEBUG */


