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
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_boot_sector_driver.c
///
/// HAL Boot Sector driver.
/// 
/// Provides functions to allow HAL EBC and HAL Host Monitor to write some
/// informations in the Boot Sector structure and to validate this structure.
/// 
/// The name is very long to avoid to be cached by the *boot_sector.o pattern
/// of the linker script.
//
////////////////////////////////////////////////////////////////////////////////

#include "chip_id.h"
#include "global_macros.h"

#include "cs_types.h"

#include "hal_mem_map.h"

#include "boot_sector.h"
#include "halp_debug.h"



// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

/// Boot Sector reload structure.
/// This structure contains information written by HAL during the normal
/// execution of the code and used by the Boot Sector to reload the ram image
/// when the phone boots. 
PRIVATE BOOT_SECTOR_RELOAD_STRUCT_T 
    HAL_BOOT_SECTOR_RELOAD_STRUCT_SECTION g_halBootSectorReloadStruct;



// =============================================================================
// FUNCTIONS                                                            
// =============================================================================



// =============================================================================
// hal_BootSectorFillReloadCtx
// -----------------------------------------------------------------------------
/// Fill the boot sector reload context.
/// ctx: pointer to the boot sector reload context.
/// id: boot sector reload id
// =============================================================================
PUBLIC BOOL hal_BootSectorFillReloadCtx(BOOT_SECTOR_RELOAD_CTX_T *ctx, BOOT_SECTOR_RELOAD_ID_T id)
{
    if(id >= BOOT_SECTOR_RELOAD_MAX_NUM)
    {
        HAL_ASSERT(FALSE,"boot sector reload: unkown id %d",id);
        return FALSE;
    }

    g_halBootSectorReloadStruct.ctx[id].storeAddress = ctx->storeAddress;
    g_halBootSectorReloadStruct.ctx[id].mapAddress = ctx->mapAddress;
    g_halBootSectorReloadStruct.ctx[id].size = ctx->size;

    return TRUE;
}

// =============================================================================
// hal_BootSectorReloadEnable
// -----------------------------------------------------------------------------
/// Validate the Boot Sector reload structure. This is a way to tell the Boot Sector
/// that the structure contains correct ram image and that it must be loaded
/// and executed when restart.
/// 
/// This is done by putting the address of the Boot Sector reload structure in
/// the Boot Sector reload structure pointer and enabling a flag in it.
// =============================================================================
PUBLIC VOID hal_BootSectorReloadEnable(VOID)
{
    g_halBootSectorReloadStruct.validTag = BOOT_SECTOR_RELOAD_VALID_TAG;
    g_halBootSectorReloadStruct.checkSum = ~BOOT_SECTOR_RELOAD_VALID_TAG;
    *boot_BootSectorGetReloadStructPointer() = &g_halBootSectorReloadStruct;
}


