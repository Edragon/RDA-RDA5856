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
//
/// @file bootp_uart_monitor.h
///
/// Uart monitor function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOTP_UART_MONITOR_H_
#define _BOOTP_UART_MONITOR_H_

#include "cs_types.h"
#include "hal_uart.h"
#include "boot.h"
// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================
// =============================================================================
// g_bootUartMonitorBaudRate;
// -----------------------------------------------------------------------------
/// Holds the detected baud rate if any, or 0.
// =============================================================================
EXPORT PROTECTED HAL_UART_BAUD_RATE_T g_bootUartMonitorBaudRate;



// =============================================================================
// g_bootUartMonitorBrDetected
// -----------------------------------------------------------------------------
/// Is true if, and only if, a communication baudrate has been detected.
// =============================================================================
EXPORT PROTECTED BOOL g_bootUartMonitorBrDetected;


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

PROTECTED VOID boot_UartMonitorOpen(VOID);

PROTECTED VOID boot_UartMonitorClose(VOID);

PROTECTED BOOT_MONITOR_OP_STATUS_T boot_UartMonitor(VOID);

#endif // _BOOTP_UART_MONITOR_H_
