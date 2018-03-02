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
/// @file memd_config.h                                                       //
/// That file defines the configuration structures used by MEMD.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _MEMD_CONFIG_H
#define _MEMD_CONFIG_H

#include "cs_types.h"
#include "chip_id.h"
#include "hal_ebc.h"

// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================
//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857)
#if defined(SPI_FLSH_26M) || defined(__FORCE_WORK_IN_26MHZ__LOCAL)
#define MEMD_SPIFLASH_FREQ HAL_SYS_SPIFLASH_FREQ_52M
#elif defined(SPI_FLSH_52M)
#define MEMD_SPIFLASH_FREQ HAL_SYS_SPIFLASH_FREQ_104M
#elif defined(SPI_FLSH_78M)
#define MEMD_SPIFLASH_FREQ HAL_SYS_SPIFLASH_FREQ_156M
#elif defined(SPI_FLSH_89M)
#define MEMD_SPIFLASH_FREQ HAL_SYS_SPIFLASH_FREQ_178M
#elif defined(SPI_FLSH_104M)
#define MEMD_SPIFLASH_FREQ HAL_SYS_SPIFLASH_FREQ_208M
#else
#error spi clk is out of 104MHz.
#endif

#if defined(dual_spi_flash)
#error Fix me!!
#endif /* dual_spi_flash */
//#endif

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// MEMD_FLASH_CONFIG_T
// -----------------------------------------------------------------------------
/// That type describes the settings to apply to configure the flash driver
/// to get the optimal performances.
// =============================================================================
typedef struct
{
    CONST HAL_EBC_CS_CFG_T csConfig;
} MEMD_FLASH_CONFIG_T;

// =============================================================================
// MEMD_RAM_CONFIG_T
// -----------------------------------------------------------------------------
/// That type describes the settings to apply to configure the ram driver
/// to get the optimal performances.
// =============================================================================
typedef struct
{
    CONST HAL_EBC_CS_CFG_T csConfig;
} MEMD_RAM_CONFIG_T;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//{TGT_MEMD_RAM_CONFIG,TGT_RAM_CONFIG_8808SAP,TGT_RAM_CONFIG_8808S_EMC,TGT_RAM_CONFIG_8808S_WINBOND,TGT_RAM_CONFIG_8809_AP};
//{TGT_MEMD_RAM_CONFIG,TGT_RAM_CONFIG_8808SAP,TGT_RAM_CONFIG_8808S_EMC,TGT_RAM_CONFIG_8808S_WINBOND,TGT_RAM_CONFIG_8809_AP,RDA8808S_ETRON};

typedef enum
{    
    RDA_APTIC,
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)     
    RDA8808S_APTIC,
    RDA8808S_EMC,   //MC323SP16BKR
    RDA8808S_WINBOND, //W955D6G
    RDA8808S_ETRON,    
#else      
    RDA8809_APTIC,
    RDA8809_ETRON,
    RDA8809_WINBOND,
    RDA8809_EMC,
#endif
    RDA_EXTSAM_QTY

}RDA_PSRAM_TYPE_T;
// =============================================================================
//  FUNCTIONS
// =============================================================================


#endif // _MEMD_CONFIG_H

