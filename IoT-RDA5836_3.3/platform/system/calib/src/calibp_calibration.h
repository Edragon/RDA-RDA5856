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
/// @file calibp_calibration.h                                                //
/// That file provides the private interface for the calibration module.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _CALIBP_CALIBRATION_H_
#define _CALIBP_CALIBRATION_H_

#include "cs_types.h"
#include "calib_m.h"

// =============================================================================
// g_calibCalibration
// -----------------------------------------------------------------------------
/// Calibration Structure.
/// As we are in the calibration module, the structure can be directly accessed,
/// without using a pointer, as it is declared in this module.
// =============================================================================
EXPORT PROTECTED CALIB_CALIBRATION_T g_calibCalibration;



// =============================================================================
// calib_SendCalibDoneMsg
// -----------------------------------------------------------------------------
/// Send calib done message to AP.
// =============================================================================
PROTECTED VOID calib_SendCalibDoneMsg(VOID);


#endif // _CALIBP_CALIBRATION_H_



