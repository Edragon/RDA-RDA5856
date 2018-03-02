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
/// @file hal_ana_gpadc.h                                                     //
///                                                                           //
/// Contains the GPADC driver private API                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_ANA_GPADC_H_
#define  _HALP_ANA_GPADC_H_

#include "cs_types.h"
#include "hal_ana_gpadc.h"




// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_AnaGpadcGpadc2Volt
// -----------------------------------------------------------------------------
/// Convert the value measured by the GPADC into mV, using the calibration
/// parameters.
///
/// @param gpadcVal Value read from the GPADC hardware module.
/// @return The corresponding measured tension, in mV.
// =============================================================================
PROTECTED HAL_ANA_GPADC_MV_T hal_AnaGpadcGpadc2Volt(UINT16 gpadcVal);


// =======================================================
// hal_AnaGpadcSleep
// -------------------------------------------------------
/// called in #hal_LpsSleep to try to shut down the GPADC
/// and release the resource. (only if atp >= 250MS)
// =======================================================
PROTECTED VOID hal_AnaGpadcSleep(VOID);

// =======================================================
// hal_AnaGpadcWakeUp
// -------------------------------------------------------
/// called in #hal_LpsSleep to resume the GPADC operation
/// can take back the resource (if GPADC is open)
// =======================================================
PROTECTED VOID hal_AnaGpadcWakeUp(VOID);



#endif // _HALP_ANA_GPADC_H_



