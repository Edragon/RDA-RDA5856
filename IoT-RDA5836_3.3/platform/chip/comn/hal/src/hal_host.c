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
/// @file hal_send_event.c                                                    //
/// That file is used to contain only hal_DebugSendEvent so
/// that as few things as possible are linked to the flash programmer
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "global_macros.h"
#include "debug_host.h"
#include "debug_uart.h"
#include "sys_irq.h"

#include "hal_mem_map.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_bt.h"

#include "boot_usb_monitor.h"
#include "hal_host.h"
#include "tgt_hal_cfg.h"
#include "stdarg.h"
#include "register.h"
#if CHIP_HAS_BTCPU==1
#include "rdabt_sharemem.h"
#endif

PROTECTED VOLATILE UINT32 HAL_UNCACHED_BSS g_halEventMask = 0;

// TODO: re-implement the timeout in hal_HstSendEvent.
// It should not be done in critical section:
// Now: EnterCS, Loop { Test }, ExitCS.
// Should be: Loop { EnterCS, Test, IfOk { SendEvent, ExitLoop=1 } ExitCS [Sleep,] }

// =============================================================================
// hal_HstSendEvent
// -----------------------------------------------------------------------------
/// Send an event through the host port
/// @param ch Event sent through the host port.
// =============================================================================
#ifndef CT_NO_DEBUG
PUBLIC BOOL HAL_BOOT_FUNC hal_HstSendEvent(UINT32 level,UINT32 ch)
{
#ifdef _FLASH_PROGRAMMER // don't send event for programmer
    return 0;
#endif
#ifdef CT_NO_DEBUG    
    if(level != BOOT_EVENT && (g_halEventMask & level)==0)
        return 0;
#endif
#if !(CHIP_HAS_USB == 1) && defined(HAL_EVENT_USB)
  #error "You can't use HAL_EVENT_USB without CHIP_HAS_USB"
#endif

#ifdef HAL_EVENT_USB
    boot_HostUsbEvent(ch);
    return 0;

#else // HAL_EVENT_USB

    UINT32 status    = hal_SysEnterCriticalSection();
    UINT32 startTime;
    BOOL   timeOut   = FALSE;
#ifndef _FLASH_PROGRAMMER
#if CHIP_HAS_BTCPU==1
    if(hal_IsBtStart())
    {
    	timeOut = rdabt_wait_btcpu_busy();
    }
#endif
#endif

    if(level != BOOT_EVENT)//add for helping the tools judge the event which the boot_event print
    {
        if(ch <= 0xff)
            ch |= 0x80000000;
    }
    startTime = hal_TimGetUpTime();
    //  wait for host sema to be free
    //  or time out.
    while (!timeOut && (hwp_debugHost->event & 1) == 0 )
    {
        if ((hal_TimGetUpTime() - startTime) >= HAL_HST_SEND_EVENT_DELAY)
        {
            timeOut = TRUE;
            break;
        }
    }

    //  Write the byte in the host event register if possible
    if (!timeOut)
    {
        hwp_debugHost->event = ch;
    }
#ifndef _FLASH_PROGRAMMER
#if CHIP_HAS_BTCPU==1
    if(hal_IsBtStart())
    	rdabt_clear_xcpu_busy();
#endif
#endif

    hal_SysExitCriticalSection(status);
    return timeOut;
#endif // HAL_EVENT_USB
}
#endif

// =============================================================================
// hal_HstWaitLastEventSent
// -----------------------------------------------------------------------------
/// Check if the last event has been sent on the host port.
/// You can't send new event after this function
// =============================================================================
PUBLIC VOID hal_HstWaitLastEventSent(VOID)
{
#if !(CHIP_HAS_USB == 1) && defined(HAL_EVENT_USB)
  #error "You can't use HAL_EVENT_USB without CHIP_HAS_USB"
#endif

#ifndef HAL_EVENT_USB
    // There is a pending event in the event fifo (!= UART fifo).
    while((hwp_debugHost->event & DEBUG_HOST_EVENT0_SEMA) == 0);

    // Is there anything left in the UART fifo or in the Tx machine?
    while((hwp_debug_uart->status &
           (DEBUG_UART_TX_ACTIVE | DEBUG_UART_TX_FIFO_LEVEL_MASK)) != 0);
#endif // !HAL_EVENT_USB
}
