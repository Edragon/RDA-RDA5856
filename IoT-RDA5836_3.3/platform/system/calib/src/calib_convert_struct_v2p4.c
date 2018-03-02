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

#include "cs_types.h"
#include "chip_id.h"

#include "snd_types.h"
#include "calib_m.h"

#include "string.h"


/// Number of power management driver parameters.
#define CALIB_PMD_QTY_V2P4                       (2)


// ============================================================================
// CALIB_GPADC_T
// -----------------------------------------------------------------------------
/// Calib GPADC analog type.
// =============================================================================
typedef struct
{
    UINT8                          bandgap;                      //0x00000000
    UINT16                         sensorGainA;                  //0x00000002
    UINT16                         sensorGainB;                  //0x00000004
} CALIB_GPADC_V2P4_T; //Size : 0x6



// ============================================================================
// CALIB_RF_ANALOG_T
// -----------------------------------------------------------------------------
/// Calib RF analog type.
// =============================================================================
typedef struct
{
    UINT8                          txRefVolt;                    //0x00000000
    UINT8                          txDacCurQ;                    //0x00000001
    UINT8                          txDacCurI;                    //0x00000002
    UINT8                          rxRefCal;                     //0x00000003
} CALIB_RF_ANALOG_V2P4_T; //Size : 0x4



// ============================================================================
// CALIB_PMD_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 CALIB_PMD_V2P4_T[CALIB_PMD_QTY_V2P4];



// ============================================================================
// CALIB_BB_T
// -----------------------------------------------------------------------------
/// Baseband calibration.
// =============================================================================
typedef struct
{
    CALIB_PAL_TIMES_T              times;                        //0x00000000
    /// Audio calibration, for each interface
    CALIB_AUDIO_ITF_T              audio[CALIB_AUDIO_ITF_QTY];   //0x00000028
    /// Analog macros calibration: GPADC.
    CALIB_GPADC_V2P4_T             gpadc;                        //0x00000988
    /// Analog macros calibration: RF modules.
    CALIB_RF_ANALOG_V2P4_T         rfAnalog;                     //0x0000098E
    CALIB_PMD_V2P4_T               pmd;                          //0x00000994
} CALIB_BB_V2P4_T; //Size : 0x99C



PUBLIC VOID calib_ConvertStructV2p4(CALIB_BB_T *pCalibBb)
{
    CALIB_BB_V2P4_T *pCalibBbV2p4 = (CALIB_BB_V2P4_T *)pCalibBb;
    CALIB_GPADC_T gpadc;

    gpadc.sensorGainA = pCalibBbV2p4->gpadc.sensorGainA;
    gpadc.sensorGainB = pCalibBbV2p4->gpadc.sensorGainB;

    memcpy(&pCalibBb->gpadc, &gpadc, sizeof(pCalibBb->gpadc));
}


