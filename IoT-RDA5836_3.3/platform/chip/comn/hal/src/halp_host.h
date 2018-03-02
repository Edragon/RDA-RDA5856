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
/// @file halp_host.h                                                         //
/// That file provides the private API for the host monitor and host client.  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_HOST_H_
#define _HALP_HOST_H_

#include "cs_types.h"



// =============================================================================
// hal_HstMonitor
// -----------------------------------------------------------------------------
/// Main host monitor function. Read the command passed to the platform through
/// the Host port and call the host command handler if appropriate.
/// It read the H2P register to execute commands
/// until the Exit command is received (BOOT_HST_MONITOR_END_CMD).
// =============================================================================
PROTECTED VOID hal_HstMonitor(VOID);



// =============================================================================
// hal_HstMonitorX
// -----------------------------------------------------------------------------
/// That function execute the function pointed by the execution context
/// #hal_HstMonitorXCtx.
// =============================================================================
PROTECTED  VOID hal_HstMonitorX(VOID); 




// =============================================================================
// hal_HstRamClientIdle
// -----------------------------------------------------------------------------
/// Read the H2P exchange register to retrieve host commands. If any supported
/// one found (type #BOOT_HST_CMD_T), it is executed during the idle task.
// =============================================================================
PROTECTED VOID hal_HstRamClientIdle(VOID);

// =============================================================================
// hal_HstRamClientIrq
// -----------------------------------------------------------------------------
/// Executed on IRQ from the Host. It reads the H2P exchange register to retrieve
/// host commands. If any supported one is found (type #BOOT_HST_CMD_T), it is
/// executed immediatly. Takes care of Critical Section breaking and coherence
/// etc ...
// =============================================================================
PROTECTED VOID hal_HstRamClientIrq(VOID);

// =============================================================================
// hal_HstSleep
// -----------------------------------------------------------------------------
/// Release the Host Clock Resource so system can go to low power
// =============================================================================
PROTECTED VOID hal_HstSleep(VOID);

// =============================================================================
// hal_HstWakeup
// -----------------------------------------------------------------------------
/// Set the Host Clock Resource as active.  Request 26M clock.
// =============================================================================
PROTECTED VOID hal_HstWakeup(VOID);


#endif // _HALP_HOST_H_
