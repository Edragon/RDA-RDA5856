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

// =============================================================================
/// LCDD config
// =============================================================================
#ifndef TGT_LCDD_CONFIG

#ifdef lcd_line_8_low
#define LCDD_CONFIG {                                                   \
    {.cs            =   HAL_GOUDA_LCD_CS_0,                             \
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_8_bit_RGB565,      \
    .cs0Polarity    =   FALSE,                                          \
    .cs1Polarity    =   FALSE,                                          \
    .rsPolarity     =   FALSE,                                          \
    .wrPolarity     =   FALSE,                                          \
    .rdPolarity     =   FALSE}}

#else


//ascii
#define LCDD_CONFIG {                                                   \
    {.cs            =   HAL_GOUDA_LCD_CS_0,                             \
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,      \
    .cs0Polarity    =   FALSE,                                          \
    .cs1Polarity    =   FALSE,                                          \
    .rsPolarity     =   FALSE,                                          \
    .wrPolarity     =   FALSE,                                          \
    .rdPolarity     =   FALSE}}
#endif
///@todo check bellow timings with LCD spec

// Write:
// tas >=  0
// tah >=  2 ns
// pwl >= 40 ns
// pwh >= 50 ns

#define LCDD_TIMING_32K {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \


#define LCDD_TIMING_13M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_26M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_39M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  3,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_52M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  3,                                                   \
    .pwh        =  4}}                                                  \

#define LCDD_TIMING_78M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  4,                                                   \
    .pwh        =  5}}                                                  \

#define LCDD_TIMING_104M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  3,                                                   \
    .pwl        =  6,                                                   \
    .pwh        =  6}}                                                  \

#define LCDD_TIMING_156M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  8,                                                   \
    .pwh        =  9}}                                                  \

#if (CHIP_HAS_ASYNC_TCU)
#define LCDD_TIMING_178M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  5,                                                   \
    .pwl        =  10,                                                   \
    .pwh        =  10}}                                                  \

#define LCDD_TIMING_208M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  6,                                                   \
    .pwl        =  12,                                                   \
    .pwh        =  12}}                                                  \

#define LCDD_TIMING_250M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  12,                                                   \
    .pwl        =  24,                                                   \
    .pwh        =  24}}                                                  \

#define LCDD_TIMING_312M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  15,                                                   \
    .pwl        =  30,                                                   \
    .pwh        =  30}}                                                  \

#endif

#if (CHIP_HAS_ASYNC_TCU)
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
            LCDD_TIMING_178M,                                           \
            LCDD_TIMING_208M,                                           \
            LCDD_TIMING_250M,                                           \
            LCDD_TIMING_312M,                                           \
        }                                                               \
    }
#else
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
#endif

// Read:
// tas >=  10 ns
// tah >=   2 ns
// pwl >= 200 ns
// pwh >= 250 ns

#define LCDD_TIMING_READ_32K {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \


#define LCDD_TIMING_READ_13M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_26M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_39M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_52M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_78M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_104M {                                         \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_156M {                                         \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  50}}                                                 \

#if (CHIP_HAS_ASYNC_TCU)
#define LCDD_TIMING_READ_178M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_208M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_250M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_312M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#endif

#if (CHIP_HAS_ASYNC_TCU)
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
            LCDD_TIMING_READ_178M,                                      \
            LCDD_TIMING_READ_208M,                                      \
            LCDD_TIMING_READ_250M,                                      \
            LCDD_TIMING_READ_312M,                                      \
            }                                                           \
    }
#else
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
#endif

#ifdef USE_SPI_LCD
// =============================================================================
/// Serial LCDD config
// =============================================================================

#define LCDD_SPI_LINE_TYPE     (HAL_GOUDA_SPI_LINE_4)


#define LCDD_SPI_FREQ          (15000000)
#define LCDD_SPI_FREQ_READ     (5000000)

#endif //USE_SPI_LCD
#endif // TGT_LCDD_CONFIG

