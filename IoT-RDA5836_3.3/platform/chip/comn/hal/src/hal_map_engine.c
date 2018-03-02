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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_map_modules.c
///
/// This file contains the implementation of the map mechanism to register
/// a module in order to access its information from a remote PC (e.g. through
/// CoolWatcher).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_error.h"
#include "hal_map_engine.h"

#include "halp_debug.h"



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================



// =============================================================================
// g_halFixedPointer
// -----------------------------------------------------------------------------
/// Address of the fixed pointer. At this address, the remote tools (e.g.
/// CoolWatcher) will find a pointer to the map structure.
// =============================================================================
PROTECTED HAL_MAP_TABLE_T* g_halFixedPointer __attribute__((section (".fixptr")));



// =============================================================================
// g_halMapTable
// -----------------------------------------------------------------------------
/// Structure containing the map informations of the software modules of the
/// system: version of the module and access to the map description.
// =============================================================================
PROTECTED HAL_MAP_TABLE_T g_halMapTable __attribute__((section(".bss")));



// =============================================================================
//  FUNCTIONS
// =============================================================================



// =============================================================================
// hal_MapEngineOpen
// -----------------------------------------------------------------------------
/// Initialize the map engine so that the remote tools can access the map
/// structures of the software modules.
// =============================================================================
VOID hal_MapEngineOpen(VOID)
{
    UINT16 i;
    
    // Init the map table.
    for (i = 0; i < HAL_MAP_ID_COUNT; i++)
    {
        g_halMapTable.modules[i].version = NULL;
        g_halMapTable.modules[i].access = NULL;
    }
    
    // Init the fixed pointer with the address of the map table.
    g_halFixedPointer = &g_halMapTable;
}



// =============================================================================
// hal_MapEngineRegisterModule
// -----------------------------------------------------------------------------
/// Configure the map access field for the specified module in the table
/// located at the address pointer by the fixed pointer.
/// @param id Id of the module for which the version and the access
/// pointer will be specified.
/// @param version Pointer to a structure giving the version information for
/// the module 'id'. This structure must be instantiated by the module 'id'.
/// @param access Pointer to a structure containing the informations that will
/// be accessible from the remote PC (e.g. through CoolWatcher). This strucutre
/// must be instantiated by the module 'id'.
/// @return HAL_ERR_NO if no error or HAL_ERR_BAD_PARAMETER if the module 'id'
/// cannot be found.
// =============================================================================
HAL_ERR_T hal_MapEngineRegisterModule(HAL_MAP_ID_T id, HAL_MAP_VERSION_T* version, VOID* access)
{
    // Check the id range.
    if (id < 0 || id >= HAL_MAP_ID_COUNT)
    {
        return HAL_ERR_BAD_PARAMETER;
    }
    
    // Register the version and the access. 
    g_halMapTable.modules[id].version = version;
    g_halMapTable.modules[id].access  = access;
    
    // Report the version string of the module
//    HAL_TRACE(TSTDOUT, 0, "Module :%d, %08x, %d", id, version->number, version->string);
    return HAL_ERR_NO;
}

// =============================================================================
// hal_MapEnginePrintModuleVersion
// -----------------------------------------------------------------------------
/// Print a module version's string on the Trace, if the module has been
/// registered.
/// @param id Number of the Module whose version is to be put
/// on the Trace.
// =============================================================================
PUBLIC VOID hal_MapEnginePrintModuleVersion(HAL_MAP_ID_T id)
{
    HAL_ASSERT((id >= 0 && id < HAL_MAP_ID_COUNT),
                "Bad Hal Engine Module Number");

    if (g_halMapTable.modules[id].version != NULL)
    {
#ifdef CHIP_HAS_BCPU_ROM
#ifdef REWRITE_ROM_AT_BOOT
#ifdef FPGA
        // BCPU ROM is not updated yet, and the contents of version is determined
        // by previous BCPU ROM.
        if (id==HAL_MAP_ID_SPAL || id==HAL_MAP_ID_SPP || id==HAL_MAP_ID_SPC)
        {
            HAL_TRACE(TSTDOUT, 0, "Skpping module in BCPU ROM: %u", id);
        }
        else
#endif // FPGA
#endif // REWRITE_ROM_AT_BOOT
#endif // CHIP_HAS_BCPU_ROM
        {
            HAL_TRACE(TSTDOUT, 1, "%s\n number: %d - date: %d - revision: %d.",
                                g_halMapTable.modules[id].version->string,
                                g_halMapTable.modules[id].version->number,
                                g_halMapTable.modules[id].version->date,
                                g_halMapTable.modules[id].version->revision
                                );
        }
    }
    else
    {
        HAL_TRACE(TSTDOUT, 0, "Module %d not registered !", id);
    }

}


// =============================================================================
// hal_MapEnginePrintAllModulesVersion
// -----------------------------------------------------------------------------
/// Print all module version's string on the Trace.
// =============================================================================
PUBLIC VOID hal_MapEnginePrintAllModulesVersion(VOID)
{
    UINT32 i;
    for (i=0 ; i < HAL_MAP_ID_COUNT ; i++)
    {
        hal_MapEnginePrintModuleVersion(i);
    }
}


// =============================================================================
// hal_MapEngineGetVersion
// -----------------------------------------------------------------------------
/// Get the pointer to the version structure of a given module.
///
/// @param id Number of the Module whose version is to be got.
/// @return Pointer to the version structure of said module.
// =============================================================================
PUBLIC HAL_MAP_VERSION_T* hal_MapEngineGetVersion(HAL_MAP_ID_T id)
{
    HAL_ASSERT((id >= 0 && id < HAL_MAP_ID_COUNT),
                "Bad Hal Engine Module Number");

    return g_halMapTable.modules[id].version;
}

