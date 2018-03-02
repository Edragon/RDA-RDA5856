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
///     @file hal_flash.c                                                     //
///                                                                           //
///     That files contains this function in order not to include             //
///     too much stuff in the flashprogrammer                                 //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 

#include "cs_types.h"

#include "global_macros.h"
#include "mem_bridge.h"
#include "hal_ebc.h"
#include "halp_ebc.h"
#include "halp_sys.h"

UINT32 g_halCs0WriteSetting  = 0;

#ifdef CHIP_HAS_EBC_CS2_BUG
PROTECTED UINT32 g_halCs0TimeReg  = 0;
PROTECTED UINT32 g_halCs0ModeReg  = 0;
#endif

// ============================================================================
// hal_EbcFlashWriteEnable
// ----------------------------------------------------------------------------
/// Enables or disables writing for the flash memory accessible through the 
/// Chip Select 0.
/// This function is normally reserved for the implementation of flash driver
///
/// @param enable If \c TRUE, writing on the CS0 is allowed \n
///               If \c FALSE, the CS0 is read-only
// ============================================================================
PUBLIC VOID hal_EbcFlashWriteEnable(BOOL enable)
{
#if (RAM_SIZE!=0)
#ifndef CHIP_HAS_EBC_CS2_BUG
    UINT32 reg;
#endif
#endif
    if (enable)
    {
        g_halCs0WriteSetting = MEM_BRIDGE_WRITEALLOW;
    }
    else 
    {
        g_halCs0WriteSetting = 0;
    }
#if (RAM_SIZE!=0)
#ifdef CHIP_HAS_EBC_CS2_BUG
    hwp_memBridge->CS_Time_Write = g_halCs0TimeReg;
    hwp_memBridge->CS_Config[0].CS_Mode = (g_halCs0ModeReg & ~MEM_BRIDGE_WRITEALLOW)
                        | g_halCs0WriteSetting;
#else
    reg = hwp_memBridge->CS_Config[0].CS_Time;
    hwp_memBridge->CS_Time_Write = reg;

    reg = hwp_memBridge->CS_Config[0].CS_Mode & ~MEM_BRIDGE_WRITEALLOW; 
    hwp_memBridge->CS_Config[0].CS_Mode = reg | g_halCs0WriteSetting;

#endif
#endif // (RAM_SIZE!=0)
}

// ============================================================================
// hal_EbcFlashWriteEnabled
// ----------------------------------------------------------------------------
/// Test if the flash write is enable
///
/// @return If \c TRUE, writing on the CS0 is allowed \n
///               If \c FALSE, the CS0 is read-only
// ============================================================================
PUBLIC BOOL hal_EbcFlashWriteEnabled(VOID)
{
    // If g_halCs0WriteSetting is 0, the bit allowing
    // to write in CS0 is not set.
    return (g_halCs0WriteSetting != 0);
}








