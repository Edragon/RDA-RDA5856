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
/// @file memdp.h                                                             //
/// Private interface in the flash driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMDP_H_
#define _MEMDP_H_



// =============================================================================
// g_memdFlashLayout
// -----------------------------------------------------------------------------
/// This constant describes the flash layout structure 
/// using the previously defined types
// =============================================================================
EXPORT MEMD_FLASH_LAYOUT_T g_memdFlashLayout;

EXPORT PUBLIC UINT32 get_flash_status(UINT32 flash_addr);
EXPORT PUBLIC BOOL spi_flash_ini(BOOL quard_flag, UINT8 clk_offset_val, UINT8 clkdiv_val, UINT8 dual_mode);
EXPORT PUBLIC void SPI_Flash_Open();
EXPORT PUBLIC BOOL spi_flash_block_erase(UINT32 flash_addr);
EXPORT PUBLIC BOOL spi_flash_chip_erase_dual();
EXPORT PUBLIC BOOL spi_flash_chip_erase(UINT32 flash_addr);
EXPORT PUBLIC BOOL spi_flash_write(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size);
EXPORT PUBLIC void flash_read(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size);

#endif // FLDP_H




