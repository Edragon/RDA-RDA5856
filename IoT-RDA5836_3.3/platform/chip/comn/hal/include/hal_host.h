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
/// @file hal_host.h                                                          //
/// That file contains types and defines used to exchange with the            //
/// host for monitoring and remote execution.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_HOST_H_
#define _HAL_HOST_H_

EXTERN_C_START

/// @defgroup host HAL Host Port Driver/Client
/// That group describes briefly the exchange media available
/// betweem HAL and a Host (Personal Computer, Mac, etc) 
/// connected through the Host Port.
///
/// @{
///


#include "cs_types.h"


// =============================================================================
// HAL_HST_SEND_EVENT_TIMEOUT
// -----------------------------------------------------------------------------
/// Timeout after which we give up sending an event, in 16384 Hz ticks.
// =============================================================================
#define HAL_HST_SEND_EVENT_DELAY    5


typedef struct {
    UINT8 gdb_enable;
    UINT8 idle_hook_enable;
    UINT8 force_nosleep;
    UINT8 trace_enable;
    UINT32 event_mask;
    UINT16 *trace_mask;
    UINT32 spi_debug;
    UINT32 spi_data;
    VOID   *audio_calib;
    UINT8  chip_id;
    UINT8  max_gain_level;
    UINT16  reserved2;
    VOID   *bt_debug;
    VOID   *at_cmd;
} HAL_HOST_CONFIG_t;


// =============================================================================
// hal_HstSendEvent
// -----------------------------------------------------------------------------
/// Send an event through the host port
/// @param ch Event sent through the host port.
// =============================================================================
#ifdef CT_NO_DEBUG    
#define   hal_HstSendEvent(level,ch)
#else
PUBLIC BOOL hal_HstSendEvent(UINT32 level,UINT32 ch);
#endif
// =============================================================================
// hal_HstWaitLastEventSent
// -----------------------------------------------------------------------------
/// Check if the last event has been sent on the host port.
/// You can't send new event after this function
// =============================================================================
PUBLIC VOID hal_HstWaitLastEventSent(VOID);



// @} // <-- End of the host group.

EXTERN_C_END

#endif




