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
/// @file hal_bt.h                                                           //
/// Prototypes for BT IRQ handler                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_BT_HOST_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// This is a generic type for a user irq handler, called when BT has some
/// data to send to host.
// =============================================================================
typedef VOID (*HAL_BT_HOST_IRQ_HANDLER_T)(VOID);


// =============================================================================
//  FUNCTIONS 
// =============================================================================

// =============================================================================
// hal_BtSetIrqHandler
// -----------------------------------------------------------------------------
/// Set an function called when an irq indicating that BT has sent some data
/// to host.
// =============================================================================
PUBLIC VOID hal_BtSetHostIrqHandler(HAL_BT_HOST_IRQ_HANDLER_T handler);


// =============================================================================
// hal_BtStart
// -----------------------------------------------------------------------------
/// Start BT CPU.
// =============================================================================
PUBLIC VOID hal_BtStart(VOID *main, VOID *irqHdlr, VOID *stackStartAddr);


// =============================================================================
// hal_BtStop
// -----------------------------------------------------------------------------
/// Stop BT CPU.
// =============================================================================
PUBLIC VOID hal_BtStop(VOID);


// =============================================================================
// hal_BtSleep
// -----------------------------------------------------------------------------
/// Try to release frequence for BT if it is in sleep state.
// =============================================================================
PUBLIC VOID hal_BtSleep(VOID);

// =============================================================================
// hal_BtWakeUp
// -----------------------------------------------------------------------------
/// BT CPU want wake up.
// =============================================================================
PUBLIC VOID hal_BtWakeUp(VOID);


// =============================================================================
// hal_IsBtStart
// -----------------------------------------------------------------------------
/// Check BT CPU status.
// =============================================================================
PUBLIC BOOL hal_IsBtStart(VOID);

PUBLIC BOOL hal_IsBtDead(VOID);

// =============================================================================
// hal_IsBtSleep
// -----------------------------------------------------------------------------
/// Tell if BT is sleeped.
// =============================================================================
PUBLIC BOOL hal_IsBtSleep(VOID);

// =============================================================================
// hal_BtNotifyDataSent
// -----------------------------------------------------------------------------
/// Tell BT that host just sends some data.
// =============================================================================
PUBLIC VOID hal_BtNotifyDataSent(VOID);
PUBLIC void hal_BtEnableSleep(BOOL sleep);
PUBLIC UINT32 hal_BtReadBTClock(void);
PUBLIC UINT32 hal_BtReadNativeClock(void);

