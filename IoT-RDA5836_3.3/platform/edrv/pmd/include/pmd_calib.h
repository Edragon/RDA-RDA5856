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
/// @file pmd_calib.h  
/// This file contains the PMD driver calibration API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


/// @addgroup pmd 
/// @{


#ifndef _PMD_CALIB_H_
#define _PMD_CALIB_H_

#include "cs_types.h"
#include "calib_m.h"



// ============================================================================
//  MACROS
// ============================================================================


// =============================================================================
//  TYPES
// =============================================================================


// ============================================================================
//  FUNCTIONS
// ============================================================================


// ============================================================================
// pmd_CalibStartCustom
// ----------------------------------------------------------------------------
/// Runs the custom calibration process of the power management.
/// For example, in Opal, this is used for the GPADC calibration.
/// The function must do the job, possibly using the "command" parameter,
/// and then put the result(s) in "calibStruct" and return a nice status.
///
/// @command Value sent by the calibration script to start the custom calib.
/// It is different from zero and can be used for customization purpose.
///
/// @pmdCalibStruct Calibration structure to fill with the result of
/// the calibration.
///
/// @return Status of the custom calibration. This value is send by the
/// calibration stub to the calibration script. It can be used for
/// customization purpose.
// ============================================================================
PUBLIC UINT32 pmd_CalibStartCustom(UINT32 command,
                                   CALIB_PMD_T* pmdCalibStruct);


// ============================================================================
// pmd_CalibGetDefault
// ----------------------------------------------------------------------------
/// Returns the default values of the PMD calibration.
/// Used by the Calib module to get the default of each PMD implementation.
///
/// @return Default values for the PMD calib parameters.
// ============================================================================
PUBLIC CONST CALIB_PMD_T* pmd_CalibGetDefault(VOID);


/// @} // <-- End of pmd group


#endif //_PMD_CALIB_H_


