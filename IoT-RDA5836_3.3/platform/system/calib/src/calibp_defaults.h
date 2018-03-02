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
/// @file calibp_defaults.h                                                   //
/// This file defines the default calibration parameters for GPADC, RF, etc   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _CALIBP_DEFAULTS_H_
#define  _CALIBP_DEFAULTS_H_


//  -------------------------------------------------- 
//  ----- Common calibration parmeters. 

#include "calibp_audio_defaults.h"

///  GP ADC params, sensor A
///  Value 0 indicates an error when using the default values
#define DEFAULT_GPADC_SENSOR_A              (0) //(0x271)
///  GP ADC params, sensor B 
#define DEFAULT_GPADC_SENSOR_B              (0) //(0x309)

///  GP ADC default calib. 
#define DEFAULT_CALIB_GPADC                 { DEFAULT_GPADC_SENSOR_A,\
                                              DEFAULT_GPADC_SENSOR_B }

///  RF analog params. 
#define DEFAULT_TX_REF_VOLT                 0x0
///  RF analog params. 
#define DEFAULT_TX_DAC_CURQ                 0x0
///  RF analog params. 
#define DEFAULT_TX_DAC_CURI                 0x0
///  RF analog params. 
#define DEFAULT_RX_REF_CAL                  0x3

/// @todo DON'T do that... Temporary things to get hal to compile
//  -------------------------------------------------- 
//  ----- Default calibration values. 

// Default calibration times.

//  Tx Up 
#define DEFAULT_CALIB_GMSK_UP               -16
#define DEFAULT_CALIB_DAC_UP                -316
#define DEFAULT_CALIB_TX_OEN_UP             -216

//  Tx Down 
#define DEFAULT_CALIB_GMSK_DN               0
#define DEFAULT_CALIB_DAC_DN                30
#define DEFAULT_CALIB_TX_OEN_DN             15
#define DEFAULT_CALIB_DTX_OFF_DN            8

//  Rx Up 
#define DEFAULT_CALIB_RF_IN_UP              0
#define DEFAULT_CALIB_ADC_UP                -200
#define DEFAULT_CALIB_FIRST_GAIN            -100
#define DEFAULT_CALIB_NEXT_GAIN             4

//  Rx Down 
#define DEFAULT_CALIB_RF_IN_DN              0
#define DEFAULT_CALIB_ADC_DN                1

#define DEFAULT_CALIB_PAL_TIMES         {\
                                          DEFAULT_CALIB_GMSK_UP, \
                                          DEFAULT_CALIB_DAC_UP, \
                                          DEFAULT_CALIB_TX_OEN_UP, \
                                         \
                                          DEFAULT_CALIB_GMSK_DN, \
                                          DEFAULT_CALIB_DAC_DN, \
                                          DEFAULT_CALIB_TX_OEN_DN, \
                                          DEFAULT_CALIB_DTX_OFF_DN, \
                                          \
                                          DEFAULT_CALIB_RF_IN_UP, \
                                          DEFAULT_CALIB_ADC_UP, \
                                          DEFAULT_CALIB_FIRST_GAIN, \
                                          DEFAULT_CALIB_NEXT_GAIN, \
                                          \
                                          DEFAULT_CALIB_RF_IN_DN, \
                                          DEFAULT_CALIB_ADC_DN }

#endif //  CALIB_DEFAULTS_H

