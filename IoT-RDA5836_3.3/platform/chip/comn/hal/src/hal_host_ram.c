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
/// @file hal_host_ram.c                                                      //
/// That file implements the host client for non-boot situation. The idea is  //
/// is to parse possible commands and react accordingly.                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "global_macros.h"
#include "debug_host.h"

#include "boot_host.h"
#include "boot.h"

#include "hal_usb.h"
#include "hal_host.h"
#include "hal_boot.h"
#include "halp_boot_sector_driver.h"
#include "halp_host.h"
#include "halp_sys.h"

#include "sxr_ops.h"
#include "sxs_io.h"


// =============================================================================
//  MACROS
// =============================================================================

#define COMMAND_IN_IDLE(c) ((0xe1<<24)|(c&0xffffff))
#define COMMAND_IN_IRQ(c) ((0xe2<<24)|(c&0xffffff))

// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// hal_HstRamClientIdle
// -----------------------------------------------------------------------------
/// Read the H2P exchange register to retrieve host commands. If any supported
/// one found (type #BOOT_HST_CMD_T), it is executed during the idle task.
// =============================================================================
PROTECTED VOID hal_HstRamClientIdle(VOID)
{
    static BOOT_HST_CMD_T prevCommand = BOOT_HST_NO_COMMAND;
    BOOT_HST_CMD_T hostCommand;

    hostCommand = GET_BITFIELD(hwp_debugHost->h2p_status, DEBUG_HOST_H2P_STATUS);

    switch (hostCommand)
    {
    case BOOT_HST_NO_COMMAND:
        prevCommand = hostCommand;
        break;

    case BOOT_HST_MONITOR_X_CMD:
        prevCommand = hostCommand;
        // Execute a command place in the execution structure.
        // H2P_Status is cleared in the basic handler.
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
        boot_HstCmdBasicHandler(hostCommand);
#else
		boot_HstCmdBasicHandler();
#endif
        break;

// boot_RestartInMonitor being a function 
// from the boot sector, that is not 
// available when ramrunning the code...
#if (CHIP_HAS_USB == 1)
    case BOOT_HST_MONITOR_ENTER:
        prevCommand = hostCommand;
        hal_BootEnterMonitor();
        break;
#endif

        // Add you own commands here:

        // ...

        // -------------------------

    default:
        // Do NOT reset h2p register here since it might be an IRQ command.
        // Complain the case for once only.
        if (prevCommand != hostCommand)
        {
            prevCommand = hostCommand;
            // Unsupported command
            hal_HstSendEvent(SYS_EVENT,BOOT_HST_UNSUPPORTED_CMD);
            // Show current command
            hal_HstSendEvent(SYS_EVENT,COMMAND_IN_IDLE(hostCommand));
        }
        break;
    }
}


// =============================================================================
// hal_HstRamClientIrq
// -----------------------------------------------------------------------------
/// Executed on IRQ from the Host.  WARNING: This interruption is handled as an
/// exception with all its limitations. (No task awareness: Cannot send SX 
/// messages, switch tasks...)
///
/// It reads the H2P exchange register to retrieve host commands. If any 
/// supported one is found (type #BOOT_HST_CMD_T), it is executed immediately.
/// 
/// Takes care of Critical Section breaking and coherence etc.
// =============================================================================
PROTECTED VOID hal_HstRamClientIrq(VOID)
{
    BOOT_HST_CMD_T hostCommand;
    
    hostCommand = GET_BITFIELD(hwp_debugHost->h2p_status, DEBUG_HOST_H2P_STATUS);

    // Clear the Host Irq cause.
    hwp_debugHost->irq = DEBUG_HOST_XCPU_IRQ;

    switch (hostCommand)
    {
        case BOOT_HST_NO_COMMAND:
            break;
        
       
        case BOOT_HST_MONITOR_X_IRQ_CMD:
            // Execute a command place in the execution structure
            // H2P_Status is cleared in the basic handler
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
			boot_HstCmdBasicHandler(hostCommand);
#else
			boot_HstCmdBasicHandler();
#endif
            break;

        case BOOT_HST_CLK_REL:
            hal_HstSleep();
            break;

        case BOOT_HST_CLK_REQ:
            hal_HstWakeup();
            break;

        // Add you own commands here:

        // ...
        
        // -------------------------

        default:
            // unsupported command
            hal_HstSendEvent(SYS_EVENT,BOOT_HST_UNSUPPORTED_CMD);
            // Show current command
            hal_HstSendEvent(SYS_EVENT,COMMAND_IN_IRQ(hostCommand));
            break;
    }
}


PROTECTED VOID hal_HstSleep(VOID)
{
    // Release the resource.
    hal_SysRequestFreq(HAL_SYS_FREQ_DEBUG_HOST, HAL_SYS_FREQ_32K, NULL);
}


PROTECTED VOID hal_HstWakeup(VOID)
{
    // Set the resource as active.
    hal_SysRequestFreq(HAL_SYS_FREQ_DEBUG_HOST, HAL_SYS_FREQ_26M, NULL);
}

