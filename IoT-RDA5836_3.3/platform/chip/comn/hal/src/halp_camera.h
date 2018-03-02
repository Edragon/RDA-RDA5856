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
/// @file halp_camera.h
/// This file contains the camera private header.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_CAMERA_H_
#define _HALP_CAMERA_H_

#include "hal_sys.h"
// =============================================================================
// hal_CameraIrqHandler
// -----------------------------------------------------------------------------
/// Handler called by the IRQ module when the camera module generates an
/// interrupt.
// =============================================================================
PROTECTED VOID hal_CameraIrqHandler(UINT8 interruptId);

#endif // _HALP_CAMERA_H_


