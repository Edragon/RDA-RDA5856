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
/// 
///     @file hal_os_stub.h
/// 
///     That file is the header permetting access to 
///     the OS function in case there is a OS, and smoothing 
///     things in case there's not.
///     That file is deprecated and fated to disappear.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_OS_STUB_H_
#define  _HALP_OS_STUB_H_

// Then there's an OS somewhere and those
// functions exist
extern void sxr_EnterScSchedule();
extern void sxr_ExitScSchedule();

#endif // _HALP_OS_STUB_H_
