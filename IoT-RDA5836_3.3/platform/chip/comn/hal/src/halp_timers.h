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
/// @file halp_timers.h
/// That file contains the timers'module private API
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 
#ifndef  _HALP_TIMERS_H_
#define  _HALP_TIMERS_H_

#include "cs_types.h"
#include "global_macros.h"
#include "timer.h"

#include "hal_debug.h"



// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================

// =============================================================================
//  TYPES
// =============================================================================


// ============================================================================
// HAL_TIM_MODE_T
// ----------------------------------------------------------------------------
/// Timer modes
/// All the timer can operate in three different modes :
/// - single
/// - repetitive
/// - wrap.
// ============================================================================
typedef enum  {
    HAL_TIM_SINGLE_MODE = 0,   ///< Single mode
    HAL_TIM_REPEAT_MODE  = 1,///< Repetitive mode
    HAL_TIM_WRAP_MODE   = 2      ///< Wrap mode
} HAL_TIM_MODE_T;



// ============================================================================
// HAL_TIM_WD_IRQ_HANDLER_T
// ----------------------------------------------------------------------------
/// Type of the user handler function for the Watchdog timer
// ============================================================================
typedef VOID (*HAL_TIM_WD_IRQ_HANDLER_T)(VOID);



// ============================================================================
// HAL_TIM_TICK_IRQ_HANDLER_T
// ----------------------------------------------------------------------------
/// Type of the user handler function for the OS timer
// ============================================================================
typedef VOID (*HAL_TIM_TICK_IRQ_HANDLER_T)(VOID);



// ============================================================================
// 
// ----------------------------------------------------------------------------
// ============================================================================

// ============================================================================
//  FUNCTIONS
// ============================================================================

// ============================================================================
// hal_TimWdIrqSetHandler
// ----------------------------------------------------------------------------
/// Set the function called in case of interrupt after a Watchdog.
/// @param handler Function called when a WatchDog interrupt occurs
// ============================================================================
PROTECTED VOID hal_TimWdIrqSetHandler(HAL_TIM_WD_IRQ_HANDLER_T handler);



// =============================================================================
// hal_TimWdIrqSetMask
// -----------------------------------------------------------------------------
/// @param mask Mask of the WatchDog interrupt. (\c TRUE to have the interrupt).
// ============================================================================-
PROTECTED VOID hal_TimWdIrqSetMask(BOOL mask);



// ============================================================================
// hal_TimWdIrqGetMask
// ----------------------------------------------------------------------------
/// @return The mask for the WatchDog IRQ.
// ============================================================================
PROTECTED BOOL hal_TimWdIrqGetMask(VOID);



// ============================================================================
// hal_TimTimersIrqHandler
// ----------------------------------------------------------------------------
/// Timers module IRQ handler.
// ============================================================================
PROTECTED VOID hal_TimTimersIrqHandler(UINT8 interruptId);




// =============================================================================
// hal_TimWatchDogStop
// -----------------------------------------------------------------------------
/// Stop the watchdog timer 
/// This function stops the watchdog timer. A reset will not occur because
/// of the watchdog, even if #hal_TimWatchDogKeepAlive is not called. The 
/// watchdog can be re-enabled by a call to #hal_TimWatchDogKeepAlive.
// =============================================================================
PROTECTED VOID hal_TimWatchDogStop(VOID);



// ============================================================================
// hal_TickInit
// ----------------------------------------------------------------------------
/// Initializes the OS tick timer and configure its driver.
/// This function is called by hal_Open.
// ============================================================================
PROTECTED VOID hal_TimTickOpen(VOID);



// ============================================================================
// hal_TimTickIrqHandler
// ----------------------------------------------------------------------------
/// OS tick IRQ handler, used by the IRQ module to clear the cause and call 
/// the 'user' function.
// ============================================================================ 
PROTECTED VOID hal_TimTickIrqHandler(UINT8 interruptId);


PROTECTED VOID hal_SelectIrqHandler(UINT8 interruptId);


#endif //_HALP_TIMERS_H_

