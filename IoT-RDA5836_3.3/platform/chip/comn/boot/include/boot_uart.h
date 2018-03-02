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
//                                                                            //
/// @file boot_uart.h                                                         //
/// That file contains types and defines used to exchange with the            //
/// uart for auto-bauding, monitoring and remote execution.                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _BOOT_UART_H_
#define _BOOT_UART_H_

/// @defgroup boot_uart BOOT Uart Port Driver/Client
/// That group describes briefly the exchange interface 
/// to the romed boot uart monitor.
/// @{
///

#include "cs_types.h"
#include "hal_uart.h"

// =============================================================================
// boot_UartMonitorGetBaudRate
// -----------------------------------------------------------------------------
/// Used to recover the value of the detected monitor uart
/// @param baudRate pointer where the detected value, if any, will be 
/// written.
/// @return \c TRUE If a connected Uart was detected and the baudrate
/// discover, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL boot_UartMonitorGetBaudRate(HAL_UART_BAUD_RATE_T* rate);


// =============================================================================
// boot_HstUartDump
// -----------------------------------------------------------------------------
/// Send a buffer of data in a dump packet through the host.
/// @param data Pointer to the data to send.
/// @param length Lenght of the buffer to send.
/// @param access Access width in bytes (1,2 or 4).
/// @return #BOOT_UM_ERR_NO or #BOOT_UM_ERR_TX_FAILED.
// =============================================================================
PUBLIC UINT32 boot_HstUartDump(UINT8* data, UINT32 length, UINT32 access);


/// @} // <-- End of the boot_uart group.

#endif // _BOOT_UART_H_




