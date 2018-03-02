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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_pmd_cfg.h                                                       //
/// That file provides the TGT API related to PMD configuration.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_PMD_CFG_H
#define _TGT_PMD_CFG_H

#include "cs_types.h"


// =============================================================================
// PMD_CONFIG_T
// -----------------------------------------------------------------------------
/// This is the anonymous declaration of the structure used to describes the 
/// configuration of the PMD driver. The actual content is specifc to the pmd 
/// driver of the actual PM Chip
// =============================================================================
typedef struct PMD_CONFIG_STRUCT_T TGT_PMD_CONFIG_T;

// =============================================================================
// TGT_PMD_VOLT_CAP_T
// -----------------------------------------------------------------------------
/// The structure used to describes a point in the voltage vs capacity
/// curve of the battery.
// =============================================================================
typedef struct
{
    UINT16 volt;
    UINT16 cap;
} TGT_PMD_VOLT_CAP_T;


// =============================================================================
// TGT_PMD_TEMPERATURE_RESISTANCE_T
// -----------------------------------------------------------------------------
/// The structure used to describes a point in the temperature vs resistance
/// curve of the thermistor sensor.
// =============================================================================
typedef struct
{
    UINT16 degree;
    UINT32 ohm;
} TGT_PMD_TEMPERATURE_RESISTANCE_T;

// =============================================================================
// tgt_GetPmdConfig
// -----------------------------------------------------------------------------
/// This function is used by Aud to get access to its configuration structure.
/// This is the only way Aud can get this information.
// =============================================================================
PUBLIC CONST TGT_PMD_CONFIG_T* tgt_GetPmdConfig(VOID);


#endif // _TGT_PMD_CFG_H

