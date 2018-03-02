/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file lcdd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TSD_CONFIG_H_
#define _TSD_CONFIG_H_

#include "cs_types.h"
#include "hal_gpio.h"

// =============================================================================
// TSD_CONFIG_T
// -----------------------------------------------------------------------------
/// The type TSD_CONFIG_T is defined in tsd_m.h using the structure
/// #TSD_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
/// Make sure to include tsd_config.h before tsd_m.h !
// =============================================================================
#define TSD_CONFIG_STRUCT_T TSD_CONFIG_OPAL_STRUCT_T

#include "tsd_m.h"



// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// TSD_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for the TSD driver CS
// =============================================================================
struct TSD_CONFIG_OPAL_STRUCT_T
{
    /// GPIO Pin used for PEN IRQ Detection
    HAL_GPIO_GPIO_ID_T      penGpio;
    /// Timier value for debouncing check (in ticks) after pen Irq
    /// This is the smaller time unit used in the driver.
    /// #downPeriod, #upPeriod and TSD_REPORTING_MODE_T.repetitionPeriod
    /// are multiple of this value.
    UINT32 debounceTime;
    /// Period of debouncing time ( in # checks ) after what we consider the pen down.
    UINT16 downPeriod;
    /// Period of debouncing time ( in # checks ) after what we consider the pen up.
    UINT16 upPeriod;
    /// Maximum difference between two samples of the same point.
    UINT32 maxError;
    /// Key count
    UINT8 keyCount;
    /// volt when first key press
    UINT16 minVolt;
    /// volt when first key press
    UINT16 maxVolt;
};

PUBLIC CONST TSD_CONFIG_T* tgt_GetTsdConfig(VOID);


#endif // _TSD_CONFIG_H_


