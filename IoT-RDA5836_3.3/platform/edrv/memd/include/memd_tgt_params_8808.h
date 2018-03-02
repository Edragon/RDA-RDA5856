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
// MEMD config
// =============================================================================

#ifdef RDA_PSRAM_ADAPTIVE
// =============================================================================
// MEMD config
// =============================================================================



#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG

#ifdef RDA_PSRAM_ADAPTIVE

#ifndef GALLITE_IS_8805

#include "memd_tgt_params_8808s_aptic.h"
#include "memd_tgt_params_8808s_emc.h"
#include "memd_tgt_params_8808s_winbond.h"
//#include "memd_tgt_params_8809_aptic.h"
#include "memd_tgt_params_8808s_etron.h"

#endif
#endif
// THIS COMBO IS ADMUX

// -------------------------------
// RAM SETTINGS for 52 MHz
// -------------------------------
#ifdef MEMD_RAM_IS_BURST
#ifdef RAM_CLK_IS_52M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else


#ifdef RAM_CLK_IS_78M

#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_104M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_125M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 9,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 9,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_139M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 10,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 10,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#endif //RAM_CLK_IS_139M
#endif //RAM_CLK_IS_125M
#endif //RAM_CLK_IS_104M
#endif //RAM_CLK_IS_78M
#endif //RAM_CLK_IS_52M
#else //!MEMD_RAM_IS_BURST
#ifndef FPGA
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 4,                                                \
    .oedlyHalfMode  = 1,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 1,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 1}
#else // FPGA
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 4,                                                \
    .oedlyHalfMode  = 1,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 1,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 1}
#endif // FPGA
#endif //!MEMD_RAM_IS_BURST


#define TGT_MEMD_RAM_CONFIG                                             \
    {                                                                   \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                          \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 1,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 5,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}

#define TGT_MEMD_FLASH_CONFIG                                           \
    {                                                                   \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                        \
    }

#endif // TGT_MEMD_CONFIG



#else






#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG


// THIS COMBO IS ADMUX

// -------------------------------
// RAM SETTINGS for 52 MHz
// -------------------------------
#ifdef MEMD_RAM_IS_BURST
#ifdef RAM_CLK_IS_52M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_78M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_104M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_8808S_TIME_SETTING {                                       \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 7,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_125M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 9,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 9,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_139M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 10,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 10,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 1,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 2,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#endif //RAM_CLK_IS_139M
#endif //RAM_CLK_IS_125M
#endif //RAM_CLK_IS_104M
#endif //RAM_CLK_IS_78M
#endif //RAM_CLK_IS_52M
#else //!MEMD_RAM_IS_BURST
#ifndef FPGA
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 4,                                                \
    .oedlyHalfMode  = 1,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 1,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 1}
#else // FPGA
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 4,                                                \
    .oedlyHalfMode  = 1,                                                \
    .oedly          = 2,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 4,                                                \
    .wedlyHalfMode  = 1,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 1}
#endif // FPGA
#endif //!MEMD_RAM_IS_BURST


#define TGT_MEMD_RAM_CONFIG                                             \
    {                                                                   \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                          \
    }

#ifdef RAM_CLK_IS_104M
#define TGT_MEMD_8808S_RAM_CONFIG                                       \
    {                                                                   \
        {SRAM_8808S_TIME_SETTING, SRAM_MODE_SETTING}                    \
    }
#endif//104MHz

// -------------------------------
// FLASH SETTINGS
// -------------------------------
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 1,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 5,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}

#define TGT_MEMD_FLASH_CONFIG                                           \
    {                                                                   \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                        \
    }

#endif // TGT_MEMD_CONFIG


#endif
