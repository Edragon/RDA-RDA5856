
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
//                                                                            //
/// @file memd_upgrade.c                                                  //
/// That file implements the function to switch the flash into a RAM mode (where
/// it behaves like a RAM), for Flash model which are not the Romulator (Ie for
/// which this is not possible)
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef MCU_WIFI_PLAT
#include "cs_types.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"

#include "ram_run.tab"
#include "hal_boot_sector_reload.h"

// =============================================================================
// memd_FlashUpgrade
// -----------------------------------------------------------------------------
/// Earse the section and write new data.
///
///
/// @param  flashAddress The byte offset within the flash chip. (Take care not 
/// @param  buffer Buffer where to get the data to write in the flash
/// @param  byteSize Number of bytes to write in flash
/// @param  reboot Reboot system or not.
/// @return #MEMD_ERR_NO, #MEMD_ERR_WRITE or #MEMD_ERR_PROTECT
// =============================================================================
#if 0
PUBLIC MEMD_ERR_T memd_FlashUpgrade(UINT8 *flashAddress, UINT8 *buffer, UINT32 byteSize, BOOL finish)
{
    UINT32 ByteSize;
    MEMD_ERR_T result;
    
    result = memd_FlashErase(flashAddress, flashAddress+byteSize);
    if(result != MEMD_ERR_NO)
        return result;
    result = memd_FlashWrite(flashAddress, byteSize, &ByteSize, buffer);
    if(result != MEMD_ERR_NO)
    {
        return result;
    }
    if(finish)
    {
        UINT32 flag= 0xD9EF0045;
        memd_FlashWrite((UINT8*)0x00000000, sizeof(UINT32), &ByteSize, (UINT8*)&flag);
        hal_SysSoftReset();
    }

    return result;
}


PUBLIC void memd_updateFlash(void)
{
    UINT32 fillIdx=0;
    BOOT_SECTOR_RELOAD_CTX_T ctx;
    
    fillIdx++;
    ctx.mapAddress = 0x01980000;
    ctx.storeAddress =ram_run1;
    ctx.size =sizeof(ram_run1)/sizeof(UINT32);
    hal_BootSectorFillReloadCtx(&ctx,fillIdx-1);

    fillIdx++;
    ctx.mapAddress = 0x01c000a0;
    ctx.storeAddress =ram_run2;
    ctx.size =sizeof(ram_run2)/sizeof(UINT32);
    hal_BootSectorFillReloadCtx(&ctx,fillIdx-1);

    fillIdx++;
    ctx.mapAddress = 0x01c0027c;
    ctx.storeAddress =ram_run3;
    ctx.size =sizeof(ram_run3)/sizeof(UINT32);
    hal_BootSectorFillReloadCtx(&ctx,fillIdx-1);  

    hal_BootSectorReloadEnable();    
}
#endif

#endif

