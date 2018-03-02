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
/// @file hal_btoverlay.c
///
/// This file implements the overlaying mechanism used to share fast memory
/// between several codes requiring high performance memory access, but not
/// at the same time.
/// 
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "string.h"
#include "sxr_tls.h"

#include "hal_overlay.h"
#include "hal_sys.h"
#include "hal_dma.h"
#include "hal_host.h"

#include "halp_debug.h"
#include "halp_gdb_stub.h"
#include "halp_proxy_memory.h"

// =============================================================================
// MACROS
// =============================================================================

#define HAL_OVERLAY_ID_NONE                 0xFFFFFFFF

// =============================================================================
// TYPES
// =============================================================================

// =============================================================================
// EXTERNAL VARIABLES
// -----------------------------------------------------------------------------
/// They come from the linker script.
// =============================================================================
// Internal Sram Overlay
extern UINT32 _sys_sram_overlay_start;
extern UINT32 _sys_sram_overlay_end;
extern UINT32 __load_start_sys_sram_overlay_0;
extern UINT32 __load_start_sys_sram_overlay_1;
extern UINT32 __load_start_sys_sram_overlay_2;
extern UINT32 __load_start_sys_sram_overlay_3;
extern UINT32 __load_start_sys_sram_overlay_4;
extern UINT32 __load_start_sys_sram_overlay_5;
extern UINT32 __load_stop_sys_sram_overlay_0;
extern UINT32 __load_stop_sys_sram_overlay_1;
extern UINT32 __load_stop_sys_sram_overlay_2;
extern UINT32 __load_stop_sys_sram_overlay_3;
extern UINT32 __load_stop_sys_sram_overlay_4;
extern UINT32 __load_stop_sys_sram_overlay_5;


#if (CHIP_HAS_PROXY_MEMORY == 1)
// Fast Cpu Memory overlay
extern UINT32 _fast_cpu_memory_overlay_start;
extern UINT32 _fast_cpu_memory_overlay_end;
extern UINT32 __load_start_fast_cpu_memory_overlay_0;
extern UINT32 __load_start_fast_cpu_memory_overlay_1;
extern UINT32 __load_start_fast_cpu_memory_overlay_2;
extern UINT32 __load_start_fast_cpu_memory_overlay_3;
extern UINT32 __load_start_fast_cpu_memory_overlay_4;
extern UINT32 __load_start_fast_cpu_memory_overlay_5;
extern UINT32 __load_stop_fast_cpu_memory_overlay_0;
extern UINT32 __load_stop_fast_cpu_memory_overlay_1;
extern UINT32 __load_stop_fast_cpu_memory_overlay_2;
extern UINT32 __load_stop_fast_cpu_memory_overlay_3;
extern UINT32 __load_stop_fast_cpu_memory_overlay_4;
extern UINT32 __load_stop_fast_cpu_memory_overlay_5;
#endif



extern UINT32 _sramtext_btoverlay_start_location;
extern UINT32 _sramtext_btoverlay_end_location;
extern UINT32 _sramtext_btoverlay_start;
extern UINT32 _sramtext_btoverlay_end;
extern UINT32 _sys_rom_btoverly_start;
extern UINT32 _bss_btoverlay_end;
extern UINT32 _bss_btoverlay_start;
extern UINT32 _bss_overlay_end;
extern VOID COS_BTOverlay_Free_All(void);


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

/// Each memory location has only one owner.
PROTECTED HAL_BTOVERLAY_ID_T g_halBTOverlayOwners = HAL_OVERLAY_ID_NONE;

// =============================================================================
// FUNCTIONS
// =============================================================================

// =============================================================================
// hal_OverlayLoad
// -----------------------------------------------------------------------------
/// Load the \c id overlay in memory. It is loaded at the memory specified by
/// the id value (Internal SRAM, etc ...)
///
/// @param id Id of the overlay of \c location memory to load.
/// @return #HAL_ERR_NO when everything is fine.
///         #HAL_ERR_RESOURCE_BUSY when this memory location is already
/// being used by another overlay.
// =============================================================================
PUBLIC HAL_ERR_T hal_BTOverlayLoad(HAL_BTOVERLAY_ID_T id)
{
    HAL_ERR_T result = HAL_ERR_NO;
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    //UINT32* src;
    UINT32* dst;

    hal_HstSendEvent(SYS_EVENT, 0x02e20000+id);
    
    if(id == g_halBTOverlayOwners)
        return;

    g_halBTOverlayOwners = id;
    
    // Clear bt overlay in Internal SRAM.
    for (dst = (UINT32*) &_dualport_heap_start;
        dst < (UINT32*) &_dualport_heap_end;
        dst++)
    {
        *dst = 0;
    }  
         
    if(id != HAL_BTOVERLAY_BLUETOOTH)
    {
        COS_BTOverlay_Free_All();
    }
    
    //if(id!=HAL_BTOVERLAY_FM)
    /*
    {
        // Clear bt overlay in Internal SRAM.
        for (dst = (UINT32*) &_dualport_heap_start;
             dst < (UINT32*) &_dualport_heap_end;
             dst++)
        {
            *dst = 0;
        }  
    }
    */
#endif

#if 0
    HAL_OVERLAY_MEMORY_LOCATION_ID_T memId = hal_OverlayGetMemoryLocationId(id);
    UINT32  descId = hal_OverlayGetDescriptorId(id);
    HAL_DMA_CFG_T transferCfg;

    // Protect access to global variables.
    UINT32  status = hal_SysEnterCriticalSection();
    hal_HstSendEvent(SYS_EVENT, 0x02e20000+id);
    if (g_halOverlayOwners[memId] == id)
    {
        // FIXME What happens if own code 
        // is already there ?
        result = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Space free take it.
        g_halOverlayOwners[memId] = id;
        result = HAL_ERR_NO;
    }

    // Unprotect.
    hal_SysExitCriticalSection(status);
    
    if (result == HAL_ERR_NO)
    {
        // We got the place, loading ...
        HAL_TRACE(HAL_DBG_TRC, 0, "ram start %08x", g_halOverlayDesc[memId][descId].ramStart);
        HAL_TRACE(HAL_DBG_TRC, 0, "flash start  %08x", g_halOverlayDesc[memId][descId].flashStart);
        HAL_TRACE(HAL_DBG_TRC, 0, "flash end %08x", g_halOverlayDesc[memId][descId].flashEnd);
        HAL_TRACE(HAL_DBG_TRC, 0, "size %08x", (UINT32)g_halOverlayDesc[memId][descId].flashEnd -
            (UINT32)g_halOverlayDesc[memId][descId].flashStart);

        if(g_halOverlayDesc[memId][descId].flashStart)
        {
            HAL_ASSERT((UINT32)g_halOverlayDesc[memId][descId].flashEnd >
                (UINT32)g_halOverlayDesc[memId][descId].flashStart,
                "HAL Overlay: Attempt to load an empty overlay, id:%d", id);
            HAL_ASSERT((UINT32)g_halOverlayDesc[memId][descId].flashEnd -
                (UINT32)g_halOverlayDesc[memId][descId].flashStart,
                "HAL Overlay: Overlay too big to be loaded by DMA: %d",
                (UINT32)g_halOverlayDesc[memId][descId].flashEnd -
                (UINT32)g_halOverlayDesc[memId][descId].flashStart);

            // Use a DMA transfer as it will be faster than the CPU.
            transferCfg.srcAddr         = (UINT8*)g_halOverlayDesc[memId][descId].flashStart;
            transferCfg.dstAddr         = (UINT8*)g_halOverlayDesc[memId][descId].ramStart;
            transferCfg.alterDstAddr    = NULL;
            transferCfg.pattern         = 0;
            transferCfg.transferSize    = (UINT32)g_halOverlayDesc[memId][descId].flashEnd -
                (UINT32)g_halOverlayDesc[memId][descId].flashStart;
            transferCfg.mode            = HAL_DMA_MODE_NORMAL;
            transferCfg.userHandler     = NULL;

#if (CHIP_HAS_PROXY_MEMORY == 1)
            if (memId == HAL_OVERLAY_MEMORY_LOCATION_FAST_CPU_RAM_ID)
            {
                // Allow loading access to the proxy memory.
                hal_PMemEnableExternalAccess(TRUE);
            }
#endif
            // Start the DMA
            while (HAL_ERR_NO != hal_DmaStart(&transferCfg))
            {
                //COS_SleepByTick(10);
            }

            // Wait for the DMA to finish
            // (hal_OverlayLoad is a blocking function).
            while (!hal_DmaDone())
            {
                //COS_SleepByTick(10);
            }
        }

        // Use a DMA transfer as it will be faster than the CPU.
        transferCfg.srcAddr         = NULL;
        transferCfg.dstAddr         = (UINT8*)g_halOverlayDesc[memId][descId].bssStart;
        transferCfg.alterDstAddr    = NULL;
        transferCfg.pattern         = 0;
        transferCfg.transferSize    = (UINT32)g_halOverlayDesc[memId][descId].overlayEnd - (UINT32)g_halOverlayDesc[memId][descId].bssStart;
        transferCfg.mode            = HAL_DMA_MODE_PATTERN;
        transferCfg.userHandler     = NULL;

#if (CHIP_HAS_PROXY_MEMORY == 1)
        if (memId == HAL_OVERLAY_MEMORY_LOCATION_FAST_CPU_RAM_ID)
        {
            // Allow loading access to the proxy memory.
            hal_PMemEnableExternalAccess(TRUE);
        }
#endif
        // Start the DMA
        while (HAL_ERR_NO != hal_DmaStart(&transferCfg))
        {
            //COS_SleepByTick(10);
        }

        // Wait for the DMA to finish
        // (hal_OverlayLoad is a blocking function).
        while (!hal_DmaDone())
        {
            //COS_SleepByTick(10);
        }        

        
        // TODO Add a fallback in case of too busy DMA
        //memcpy(g_halOverlayDesc[memId][descId].ramStart,
        //        g_halOverlayDesc[memId][descId].flashStart,
        //        (UINT32)g_halOverlayDesc[memId][descId].flashEnd - (UINT32)g_halOverlayDesc[memId][descId].flashStart);

#if (CHIP_HAS_PROXY_MEMORY == 1)
        if (memId == HAL_OVERLAY_MEMORY_LOCATION_FAST_CPU_RAM_ID)
        {
            // Disable access to the proxy memory.
            hal_PMemEnableExternalAccess(FALSE);
        }
#endif

    }
#endif    
    // Fluch Caches
    hal_GdbFlushCache();

    return result;
}


// =============================================================================
// hal_OverlayUnload
// -----------------------------------------------------------------------------
/// Unload the \c id overlay from memory.
///
/// @param id Id of the overlay of \c location memory to load.
/// @return #HAL_ERR_NO when everything is fine.
///         #HAL_ERR_RESOURCE_BUSY when this memory location is already
/// being used by another overlay.
// =============================================================================
PUBLIC HAL_ERR_T hal_OverlayUnload(HAL_OVERLAY_ID_T id)
{
    HAL_ERR_T result = HAL_ERR_NO;
    
    
    return result;
}



// ========================================================================
// hal_OverlayIsLoaded
// -----------------------------------------------------------------------------
/// Check if the \c id overlay is loaded into memory.
/// 
/// @param id Id of the overlay we check if it is loaded.
/// @return \c TRUE if \c Id is loaded,
///         \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_OverlayIsLoaded(HAL_OVERLAY_ID_T id)
{
    return (g_halBTOverlayOwners == id);
}



