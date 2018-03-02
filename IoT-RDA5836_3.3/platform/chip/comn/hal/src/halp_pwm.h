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
/// @file halp_pwm.h                                                          //
/// That file provides the private API for the PWM driver.                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_PWM_H_
#define _HALP_PWM_H_

#include "cs_types.h"
#include "hal_sys.h"




// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================
// =============================================================================
// HAL_PWM_BASE_DIVIDER
// -----------------------------------------------------------------------------
/// This is the base divider used to calculate the PWM frequency from a 13MHZ
/// system clock. This value allow us to keep a constant PWM clock (309523 Hz)
/// whatever the system frequency is --> the done is kept at the same note
/// through system clock changes.
// =============================================================================
#define HAL_PWM_BASE_DIVIDER    32



// =============================================================================
// HAL_PWM_FREQ
// -----------------------------------------------------------------------------
/// This is the value of the PWM clock rate, using the provided base divider.
// =============================================================================
#define HAL_PWM_FREQ   (13000000/HAL_PWM_BASE_DIVIDER)



// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_PwmResourceMgmt
// -----------------------------------------------------------------------------
/// Checks if any components in the PWM module are active and requests or
/// releases the resource.  The return value is TRUE if the resource is
/// active and FALSE otherwise.
// =============================================================================
PROTECTED BOOL hal_PwmResourceMgmt(VOID);

// =============================================================================
// hal_PwlSetGlowing
// -----------------------------------------------------------------------------
/// Set which pin is the glowing pwl
// =============================================================================
PROTECTED VOID hal_PwlSetGlowing(HAL_PWL_ID_T id);



#endif // _HALP_PWM_H_



