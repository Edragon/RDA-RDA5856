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
/// @file hal_rda_abb.h                                                       //
///                                                                           //
/// This file decribes the registers of RDA Analog Base Band.                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_RDA_ABB_H_
#define _HAL_RDA_ABB_H_

#include "cs_types.h"

// ----------------
// Default volumes
// ----------------

// Default ABB DAC digital gain is -2dB for receiver
#define DEFAULT_ABB_DAC_GAIN_RECEIVER (-2)
// Default ABB DAC digital gain is -2dB for speaker
#define DEFAULT_ABB_DAC_GAIN_SPEAKER (-2)
// Earpiece output energy is too large
// Reduce default ABB DAC digital gain to -2dB
//#define DEFAULT_ABB_DAC_GAIN_HEAD (-2)
// Default ABB ADC (MIC) digital gain is 0dB
#define DEFAULT_ABB_ADC_GAIN (0)

// Max ABB DAC digital gain is -2dB for earpiece
#define MAX_ABB_DAC_GAIN_HEAD (-2)
// Max ABB ADC (MIC) digital gain is 12dB
#define MAX_ABB_ADC_GAIN (12)


// ----------------
// Types
// ----------------

// SIM selection state
typedef enum
{
    HAL_ABB_SIM_SELECT_SIM0 = (1<<0),
    HAL_ABB_SIM_SELECT_SIM1 = (1<<1),
    HAL_ABB_SIM_SELECT_SIM2 = (1<<2),
    HAL_ABB_SIM_SELECT_SIM3 = (1<<3),
} HAL_ABB_SIM_SELECT_T;


// ----------------
// APIs
// ----------------

PUBLIC VOID hal_AbbOpen(VOID);
PUBLIC BOOL hal_AbbRegRead(UINT32 addr, UINT32* pData);
PUBLIC VOID hal_AbbRegBlockingRead(UINT32 addr, UINT32* pData);
PUBLIC BOOL hal_AbbRegWrite(UINT32 addr, UINT32 data);

PUBLIC VOID hal_AbbEnableUsbPhy(BOOL enable);

PUBLIC VOID hal_AbbEnableSim(UINT32 simMask);

void hal_abbLineInMode(BOOL isLineInMode);

#endif // _HAL_RDA_ABB_H_

