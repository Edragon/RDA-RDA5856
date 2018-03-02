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
/// @file lcdd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_gouda.h"


#ifndef _LCDD_CONFIG_H_
#define _LCDD_CONFIG_H_


// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
// LCDD_CONFIG_T
// -----------------------------------------------------------------------------
/// The type TGT_LCDD_CONFIG_T is defined in tgt_lcdd_cfg.h using the structure
/// #LCDD_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
/// Make sure to include fmd_config.h before fmd_m.h or tgt_fmd_cfg.h !
// =============================================================================

// =============================================================================
// LCDD_CONFIG_T
// -----------------------------------------------------------------------------
/// The type LCDD_CONFIG_STRUCT_T is defined in lcdd_config.h 
// =============================================================================
typedef struct LCDD_CONFIG_STRUCT_T   LCDD_CONFIG_T;

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// LCDD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the Ili9320 LCD driver, using the GOUDA LCD controller module of the chip.
// =============================================================================
struct LCDD_CONFIG_STRUCT_T
{
    HAL_GOUDA_LCD_CONFIG_T  config;
    HAL_GOUDA_LCD_TIMINGS_T timings[HAL_GOUDA_LCD_TIMINGS_QTY];
};

// =============================================================================
/// LCDD config
//
// THIS IS ACTUALLY NEPHRITE_FPGA TIMINGS
// =============================================================================
#define LCDD_CONFIG {                                                   \
    {.cs            =   HAL_GOUDA_LCD_CS_0,                             \
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,      \
    .cs0Polarity    =   FALSE,                                          \
    .cs1Polarity    =   FALSE,                                          \
    .rsPolarity     =   FALSE,                                          \
    .wrPolarity     =   FALSE,                                          \
    .rdPolarity     =   FALSE}}

///@toto check bellow timings with LCD spec

#define LCDD_TIMING_32K {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  200,                                                   \
    .pwh        =  200}}                                                  \


#define LCDD_TIMING_13M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  200,                                                   \
    .pwh        =  200}}                                                  \

#define LCDD_TIMING_26M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  200,                                                   \
    .pwh        =  200}}                                                  \

#define LCDD_TIMING_39M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  200,                                                   \
    .pwh        =  200}}                                                  \

#define LCDD_TIMING_52M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  200,                                                   \
    .pwh        =  200}}                                                  \

#define LCDD_TIMING_78M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  300,                                                   \
    .pwh        =  300}}                                                  \

#define LCDD_TIMING_104M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  400,                                                   \
    .pwh        =  400}}                                                  \

#define LCDD_TIMING_156M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  100,                                                   \
    .pwl        =  500,                                                   \
    .pwh        =  500}}                                                  \

#define TGT_LCDD_CONFIG                                                 \
    {                                                                   \
        .config     = LCDD_CONFIG,                                      \
        .timings    = {                                                 \
            LCDD_TIMING_32K,                                            \
            LCDD_TIMING_13M,                                            \
            LCDD_TIMING_26M,                                            \
            LCDD_TIMING_39M,                                            \
            LCDD_TIMING_52M,                                            \
            LCDD_TIMING_78M,                                            \
            LCDD_TIMING_104M,                                           \
            LCDD_TIMING_156M,                                           \
        }                                                               \
    }

// Read:
// tas >=   5 ns
// tah >=   5 ns
// pwl >= 150 ns
// pwh >= 150 ns

#define LCDD_TIMING_READ_32K {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \


#define LCDD_TIMING_READ_13M {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_26M {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_39M {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_52M {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_78M {                                          \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_104M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_156M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \


 #define LCDD_READ_CONFIG                                               \
  {                                                                     \
     .config  =  LCDD_CONFIG,                                           \
     .timings    = {                                                    \
            LCDD_TIMING_READ_32K,                                       \
            LCDD_TIMING_READ_13M,                                       \
            LCDD_TIMING_READ_26M,                                       \
            LCDD_TIMING_READ_39M,                                       \
            LCDD_TIMING_READ_52M,                                       \
            LCDD_TIMING_READ_78M,                                       \
            LCDD_TIMING_READ_104M,                                      \
            LCDD_TIMING_READ_156M,                                      \
            }                                                           \
    }

#ifdef USE_SPI_LCD
// =============================================================================
/// Serial LCDD config
// =============================================================================

#define LCDD_SPI_LINE_TYPE     (HAL_GOUDA_SPI_LINE_4)


#define LCDD_SPI_FREQ          (13000000)
#define LCDD_SPI_FREQ_READ     (1000000)

#endif //USE_SPI_LCD

#endif // _LCDD_CONFIG_H_


