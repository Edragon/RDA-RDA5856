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
/// @file hal_trace.h                                                          //
/// That file provides the HAL trace driver API.                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/// @defgroup trace HAL Trace Config Driver
///
/// @{
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_TRACE_H_
#define _HAL_TRACE_H_

// =============================================================================
// Types
// =============================================================================

// =============================================================================
// HAL_HST_TRACE_PORT_T
// -----------------------------------------------------------------------------
/// Output trace (USB or Host)
// =============================================================================
typedef enum
{
    HAL_HST_TRACE_PORT_HOST,
    HAL_HST_TRACE_PORT_USB
} HAL_HST_TRACE_PORT_T ;


// =============================================================================
// Functions
// =============================================================================


// =======================================================
// hal_HstTraceOpen
// -------------------------------------------------------
/// Open and configure the host trace port.
/// The configuration is set to comply with the 
/// trace tool running on the host.
/// Traces are not enabled after a call to this function.
/// A (positive) call to #hal_HstTraceEnable is needed before
/// it is possible to actually send traces.
///
/// @param setting Not taken into account
/// @param portIdx  Not taken into account
// =======================================================
PUBLIC VOID hal_HstTraceOpen(UINT16 setting, UINT8 portIdx);


// =======================================================
// hal_HstTraceClose
// -------------------------------------------------------
// Close the host trace port
// @param portIdx Not taken into account
// =======================================================
PUBLIC VOID hal_HstTraceClose(UINT8 portIdx);


// =============================================================================
// hal_HstTraceEnable
// -----------------------------------------------------------------------------
/// This fonction enables the actual sending of traces.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// @todo param portIdx Not taken into account.
/// @return TRUE if trace sending could be enabled, FALSE otherwise.
///         Also TRUE if host clock is disabled to ensure we can go to low power.
// =============================================================================
PUBLIC BOOL hal_HstTraceEnable(VOID/*UINT8 portIdx*/);


// =============================================================================
// hal_HstTraceDisable
// -----------------------------------------------------------------------------
/// This function disables the sending of traces.
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
/// @todo param portIdx Not taken into account.
// =============================================================================
PUBLIC VOID hal_HstTraceDisable(VOID/*UINT8 portIdx*/);


// =======================================================
// hal_HstTraceSendData
// -------------------------------------------------------
/// Write data on the host trace port.
/// Traces must be opened and enabled to be properly sent.
/// @param portIdx Not taken into account.
/// @param startAddress Pointer on location to get data to be sent.
/// @param length Number of bytes to be sent.
/// @return Number of bytes taken from the location.
// =======================================================
PUBLIC UINT32 hal_HstTraceSendData(UINT8        portIdx,
                                   CONST UINT8* startAddress,
                                   UINT32       length);


// =======================================================
// hal_HstTraceGetData
// -------------------------------------------------------
/// Read data from the host trace port.
///
/// In order to read data received from the tool, the debug uart
/// IRQ is used to wake up the system from sleep and thus 
/// handle received data in idle.
/// The debug uart IRQ is not used to process data in interrupt
/// mode, thus the IRQ Mask in the IRQ module is disabled 
/// explicitely for the debug uart. Only the wake-up mask
/// is set for this interrupt.
/// This is necessary in the following case: the PC Trace Tool
/// sends a large configuration packet while the chip is in sleep mode,
/// so the data from the fifo of the Trace UART won't be read, the fifo
/// gets saturated. The Host Port sets its RTS line to high. The PC
/// cannot send commands through the Host Port anymore. We are locked outside.
///
/// @param portIdx  Not taken into account.
/// @param destAddress Pointer on location to put the received data.
/// @param length Number max of data to write at the location
/// @return Number of bytes written at the location.
// =======================================================
PUBLIC UINT32 hal_HstTraceGetData(UINT8  portIdx,
                                  UINT8* destAddress,
                                  UINT32 length);


// =============================================================================
// hal_TraceUsbCallback
// -----------------------------------------------------------------------------
/// @todo Lilian has to add a comment here!
// =============================================================================
PUBLIC VOID hal_TraceUsbCallback(VOID* buffer, UINT16 size);

// =============================================================================
// hal_HstTraceSwitch
// -----------------------------------------------------------------------------
/// Change trace output path
/// @param port Trace path (USB, Host)
// =============================================================================
PUBLIC VOID hal_HstTraceSwitch(HAL_HST_TRACE_PORT_T port);

/// @} // end of trace group

#endif // _HAL_TRACE_H_

