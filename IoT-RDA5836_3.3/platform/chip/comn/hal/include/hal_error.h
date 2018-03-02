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
/// @file hal_error.h
/// That file contains the common error type for the HAL API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



/// @defgroup error HAL Error Type
/// This module implements the common error management in the HAL drivers.
///
/// @{

#ifndef _HAL_ERROR_H_
#define _HAL_ERROR_H_

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_ERR_T
// -----------------------------------------------------------------------------
/// This types is used to describes all possible error returned by HAL's
/// API functions.
// =============================================================================
#ifndef WIN32
typedef enum
{
    /// No error occured
    HAL_ERR_NO,

    /// A resource reset is required
    HAL_ERR_RESOURCE_RESET,

    /// An attempt to access a busy resource failed
    HAL_ERR_RESOURCE_BUSY,
    
    /// Timeout while trying to access the resource
    HAL_ERR_RESOURCE_TIMEOUT,

    /// An attempt to access a resource that is not enabled
    HAL_ERR_RESOURCE_NOT_ENABLED,
    
    /// Invalid parameter
    HAL_ERR_BAD_PARAMETER,

    /// Uart RX FIFO overflowed
    HAL_ERR_UART_RX_OVERFLOW,

    /// Uart TX FIFO overflowed
    HAL_ERR_UART_TX_OVERFLOW,
    HAL_ERR_UART_PARITY,
    HAL_ERR_UART_FRAMING,
    HAL_ERR_UART_BREAK_INT,
    HAL_ERR_TIM_RTC_NOT_VALID,
    HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED,
    HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED,

    /// Communication failure
    HAL_ERR_COMMUNICATION_FAILED,

    HAL_ERR_QTY
} HAL_ERR_T;
#endif

/// @} // End of the error group 


#endif // _HAL_ERROR_H_


