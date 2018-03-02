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
/// @file halp_calendar.h
/// That file contains the calendar private header.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_CALENDAR_H_
#define _HALP_CALENDAR_H_

// =============================================================================
// hal_TimRtcIrqHandler
// -----------------------------------------------------------------------------
/// This function is the IRQ handler for the IRQ module called when a RTC/calendar
/// IRQ occurs. It clears the IRQ and calls the user handler if available.
// =============================================================================
PROTECTED VOID hal_TimRtcIrqHandler(UINT8 interruptId);



// =============================================================================
// hal_TimRtcAlarmReached
// -----------------------------------------------------------------------------
/// @return \c TRUE if the Alarm has been reached.
// =============================================================================
PROTECTED BOOL hal_TimRtcAlarmReached(VOID);


#endif // _HALP_CALENDAR_H_

