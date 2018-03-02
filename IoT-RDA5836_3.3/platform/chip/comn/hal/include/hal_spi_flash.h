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

#ifndef _HAL_SPI_FLASH_H_
#define _HAL_SPI_FLASH_H_

#include "cs_types.h"

#define FLASH_LIFE_TEST_FOR_PUYA_8M_FLASH      0
// =============================================================================
//  MACROS
// =============================================================================
#if CHIP_HAS_FLSH_CACHE
#define HAL_FLASH_CACHE_TO_MAP(cache_size, flash_map) ((cache_size) << 8 | (flash_map))

#define HAL_FLASH_CACHE_TO_CACHE_SIZE(map) (((map)>>8) & 0xFF)
#define HAL_FLASH_CACHE_TO_FLASH_MAP(map) ((map) & 0xFF)
#endif /* CHIP_HAS_FLSH_CACHE */
// =============================================================================
//  TYPES
// =============================================================================
typedef enum
{
    HAL_SPI_FLASH_READ_SERIAL_MODE   = 0x0,
    HAL_SPI_FLASH_READ_DUAL_MODE     = 0x1,
    HAL_SPI_FLASH_READ_QUARD_MODE    = 0x2,
    HAL_SPI_FLASH_READ_MAX_MODE,
} HAL_SPI_FLASH_READ_MODE_T;


typedef enum
{
    HAL_SPI_FLASH_CMD_SERIAL_MODE   = 0x0,
    HAL_SPI_FLASH_CMD_DUAL_MODE     = 0x1,
    HAL_SPI_FLASH_CMD_QUARD_MODE    = 0x2,
    HAL_SPI_FLASH_CMD_MAX_MODE,
} HAL_SPI_FLASH_CMD_MODE_T;

typedef struct
{
    UINT32              reserved;
} HAL_SPI_FLASH_DUAL_CFG_T;

#if CHIP_HAS_FLSH_CACHE
typedef enum
{
    HAL_FLSH_CACHE_SIZE_32KB = 0,
    HAL_FLSH_CACHE_SIZE_16KB  ,
    HAL_FLSH_CACHE_SIZE_8KB,
    HAL_FLSH_CACHE_SIZE_4KB,
} HAL_FLASH_CACHE_SIZE_T;

typedef enum
{
	// 32 KB Cache
	FLASH_CACHE_MAP_00_16MB_TO_32KB = HAL_FLASH_CACHE_TO_MAP(HAL_FLSH_CACHE_SIZE_32KB,0),

	// 16 KB Cache
	FLASH_CACHE_MAP_00_08MB_TO_16KB = HAL_FLASH_CACHE_TO_MAP(HAL_FLSH_CACHE_SIZE_16KB,0),
	FLASH_CACHE_MAP_08_16MB_TO_16KB,
	
	// 8 KB Cache
	FLASH_CACHE_MAP_00_04MB_TO_8KB = HAL_FLASH_CACHE_TO_MAP(HAL_FLSH_CACHE_SIZE_8KB,0),
	FLASH_CACHE_MAP_04_08MB_TO_8KB,
	FLASH_CACHE_MAP_08_12MB_TO_8KB,
	FLASH_CACHE_MAP_12_16MB_TO_8KB,

	// 4 KB Cache	
	FLASH_CACHE_MAP_00_02MB_TO_4KB = HAL_FLASH_CACHE_TO_MAP(HAL_FLSH_CACHE_SIZE_4KB,0),
	FLASH_CACHE_MAP_02_04MB_TO_4KB,
	FLASH_CACHE_MAP_04_06MB_TO_4KB,
	FLASH_CACHE_MAP_06_08MB_TO_4KB,
	FLASH_CACHE_MAP_08_10MB_TO_4KB,
	FLASH_CACHE_MAP_10_12MB_TO_4KB,
	FLASH_CACHE_MAP_12_14MB_TO_4KB,
	FLASH_CACHE_MAP_14_16MB_TO_4KB,

} HAL_FLASH_CACHE_MAP_T;

#endif /* CHIP_HAS_FLSH_CACHE */


typedef struct
{
    HAL_SPI_FLASH_CMD_MODE_T   cmdMode;
    HAL_SPI_FLASH_READ_MODE_T readMode;

    //spi_clk = ${SYS_MEMD_FLSH_SPI_CLK}/clkDiv
    UINT8 clkDiv;
    //stands for SPI clock delay cycles when reading data, 0~7
    UINT8 sampleDelay;

    BOOL dinCapEdge;

    //Set hold pin active
    BOOL holdPin;
    //Set wprotect pin active
    BOOL wpPin;

} HAL_SPI_FLASH_CFG_T, *HAL_SPI_FLASH_CFG_PTR;



// =============================================================================
//  FUNCTIONS
// =============================================================================
PUBLIC VOID hal_SpiFlashDisableRead(VOID);
PUBLIC VOID hal_SpiFlashEnableRead(VOID);
PUBLIC BOOL hal_SpiFlashIsTxFifoEmpty(VOID);
PUBLIC BOOL hal_SpiFlashPushTxFifoData(UINT8 data_array[], UINT32 data_cnt, BOOL quard_flag);
PUBLIC UINT32 hal_SpiFlashGetRxData(UINT8* destAddress, UINT32 length);
PROTECTED BOOL hal_SpiFlashClearFifoData(BOOL clr_read_flag,BOOL clr_write_flag);
PROTECTED VOID hal_SpiFlashSendCmd(UINT8 cmd, UINT32 addr, UINT8 opmode, UINT16 blocksize);
PUBLIC BOOL hal_SpiFlashInit(HAL_SPI_FLASH_CFG_T *cfg);
PUBLIC VOID hal_SpiFlashContinuousMode(BOOL enable);

#if CHIP_HAS_FLSH_CACHE
PUBLIC VOID hal_FlashCacheOn(HAL_FLASH_CACHE_MAP_T map);
PUBLIC VOID hal_FlashCacheOff(VOID);
PUBLIC VOID hal_FlashCacheFlush(VOID);
PUBLIC VOID hal_FlashCacheDebugOn(VOID);
PROTECTED VOID hal_FlashCacheIrqHandler(UINT8 interruptId);
#endif /* CHIP_HAS_FLSH_CACHE */

#endif //_HAL_SPI_FLASH_H_

