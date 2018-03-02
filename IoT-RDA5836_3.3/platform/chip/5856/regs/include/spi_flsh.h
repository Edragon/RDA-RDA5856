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
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//==============================================================================
//
/// @file spi flash controller
//
//==============================================================================

#ifndef _SPI_FLSH_H_
#define _SPI_FLSH_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'spi flsh'."
#endif

#include "global_macros.h"


// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPI_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define SPI_FLASH_REG_BASE              0x01AC0000

typedef volatile struct
{
    REG32                          tx_cmd_addr;                   //0x00000000
    REG32                          trans_mode;                    //0x00000004
    REG32                          tx_fifo_data;                  //0x00000008
    REG32                          status;                        //0x0000000C
    REG32                          rx_fifo_data;                   //0x00000010
    REG32                          flash_config;                   //0x00000014
    REG32                          fifo_ctrl;                      //0x00000018
    REG32                          dual_spi;                       //0x0000001C
    REG32                          read_cmd;                       //0x00000020
    REG32                          nand_cfg1;                      //0x00000024
    REG32                          nand_cfg2;                      //0x00000028
    REG32                          dummy_cycle_cfg;                //0x0000002C
    REG32                          diff_128m_cmd;                   //0x00000030
} HWP_SPI_FLSH_T;

#define hwp_spi_flsh                    ((HWP_SPI_FLSH_T*) KSEG1(SPI_FLASH_REG_BASE))


//tx_cmd_addr
#define SPI_FLASH_TX_COMMAND_SHIFT            (0)
#define SPI_FLASH_TX_COMMAND(n)               (((n)&0xff)<<0)
#define SPI_FLASH_TX_COMMAND_MASK             (0xff<<0)
#define SPI_FLASH_TX_ADDRESS_SHIFT            (8)
#define SPI_FLASH_TX_ADDRESS(n)               (((n)&0xffffff)<<8)
#define SPI_FLASH_TX_ADDRESS_MASK             (0xffffff<<8)


//trans_mode
#define SPI_FLASH_TX_MODE_SHIFT            (0)
#define SPI_FLASH_TX_MODE(n)               (((n)&0xff)<<0)
#define SPI_FLASH_TX_MODE_MASK             (0xff<<0)
#define SPI_FLASH_TX_BLOCK_SIZE_SHIFT      (8)
#define SPI_FLASH_TX_BLOCK_SIZE(n)         (((n)&0x1ff)<<8)
#define SPI_FLASH_TX_BLOCK_SIZE_MASK       (0x1ff<<8)
#define SPI_FLASH_TX_CONT_MODE             (1<<24)

//tx_fifo_data
#define SPI_FLASH_TX_DATA_SHIFT            (0)
#define SPI_FLASH_TX_DATA(n)               (((n)&0xff)<<0)
#define SPI_FLASH_TX_DATA_MASK             (0xff<<0)

//status
#define SPI_FLASH_BUSY_STAT					    (1 << 0)
#define SPI_FLASH_TX_FIFO_EMPTY					(1 << 1)
#define SPI_FLASH_TX_FIFO_FULL					(1 << 2)
#define SPI_FLASH_RX_FIFO_EMPTY					(1 << 3)
#define SPI_FLASH_RX_FIFO_CNT_SHIFT             (4)
#define SPI_FLASH_RX_FIFO_CNT(n)                (((n)&0xf)<<4)
#define SPI_FLASH_RX_FIFO_CNT_MASK              (0xf<<4)
#define SPI_FLASH_READ_STATE_BUSY			    (1 << 8)

//rx_fifo_data
#define SPI_FLASH_RX_DATA_SHIFT             (0)
#define SPI_FLASH_RX_DATA(n)                (((n)&0xffffffff)<<0)
#define SPI_FLASH_RX_DATA_MASK              (0xffffffff<<0)


//flash_config
#define SPI_FLASH_QUAD_MODE					    (1 << 0)
#define SPI_FLASH_WPROTECT_PIN					(1 << 1)
#define SPI_FLASH_HOLD_PIN					(1 << 2)
#define SPI_FLASH_DUAL_MODE					(1 << 3)

#define SPI_FLASH_SAMPLE_DELAY_SHIFT             (4)
#define SPI_FLASH_SAMPLE_DELAY(n)                (((n)&0x7)<<4)
#define SPI_FLASH_SAMPLE_DELAY_MASK              (0x7<<4)
#define SPI_FLASH_DIN_CAP_EDGE					(1 << 7)
#define SPI_FLASH_CLK_DIVIDER_SHIFT      (8)
#define SPI_FLASH_CLK_DIVIDER(n)         (((n)&0xff)<<8)
#define SPI_FLASH_CLK_DIVIDER_MASK       (0xff<<8)
#define SPI_FLASH_CMD_QUAD_EN			(1 << 16)

//fifo_ctrl
#define SPI_FLASH_RX_FIFO_CLR			(1 << 0)
#define SPI_FLASH_TX_FIFO_CLR			(1 << 1)

//dual_spi
#define SPI_FLASH_SPI_CS_NUM_ONE		(0 << 0)
#define SPI_FLASH_SPI_CS_NUM_TWO		(1 << 0)
#define SPI_FLASH_SPI_SIZE_SHIFT             (1)
#define SPI_FLASH_SPI_SIZE(n)                (((n)&0x3)<<1)
#define SPI_FLASH_SPI_SIZE_MASK              (0x3<<1)
#define SPI_FLASH_SPI_128M						(1 << 3)
#define SPI_FLASH_AHB_READ_DISABLE				(1 << 4)
#define SPI_FLASH_SPI_CS1_SEL					(1 << 5)
#define SPI_FLASH_SPI_CS1_SEL1					(1 << 6)
#define SPI_FLASH_DIFF_128M_DIFF_CMD_EN			(1 << 7)

//read_cmd
#define SPI_FLASH_QREAD_CMD_SHIFT             (0)
#define SPI_FLASH_QREAD_CMD(n)                (((n)&0xff)<<0)
#define SPI_FLASH_QREAD_CMD_MASK              (0xff<<0)

#define SPI_FLASH_FREAD_CMD_SHIFT             (8)
#define SPI_FLASH_FREAD_CMD(n)                (((n)&0xff)<<8)
#define SPI_FLASH_FREAD_CMD_MASK              (0xff<<8)

#define SPI_FLASH_READ_CMD_SHIFT             (16)
#define SPI_FLASH_READ_CMD(n)                (((n)&0xff)<<16)
#define SPI_FLASH_READ_CMD_MASK              (0xff<<16)

#define SPI_FLASH_PASSWORD_SHIFT             (24)
#define SPI_FLASH_PASSWORD(n)                (((n)&0xff)<<24)
#define SPI_FLASH_PASSWORD_MASK              (0xff<<24)

//nand_cfg1

//nand_cfg2


/**************************** Flash cache ****************************/

#define FLASH_CACHE_REG_BASE            0x01AC1000

typedef volatile struct
{
    REG32                          cache_conf;                  //0x00000000
    REG32                          cache_flush;                 //0x00000004
    REG32                          cache_addr_offset;           //0x00000008
    REG32                          cache_hitcnt;                //0x0000000C
    REG32                          cache_misscnt;               //0x00000010
    REG32                          cache_status;                //0x00000014
    REG32                          cache_irq;                   //0x00000018
    REG32                          cache_irq_mask;              //0x0000001C
} HWP_FLSH_CACHE_T;

#define hwp_flsh_cache                    ((HWP_FLSH_CACHE_T*) KSEG1(FLASH_CACHE_REG_BASE))


//conf
#define FLSH_CACHE_SIZE_SHIFT            (4)
#define FLSH_CACHE_SIZE(n)               (((n)&3)<<4)
#define FLSH_CACHE_SIZE_MASK             (3<<4)
#define FLSH_CACHE_SIZE_32KB             (0)  //32KB Cache; no system RAM
#define FLSH_CACHE_SIZE_16KB             (1) //16KB Cache; 16KB system RAM
#define FLSH_CACHE_SIZE_8KB             (2) //8KB Cache; 24KB system RAM
#define FLSH_CACHE_SIZE_4KB             (3) //4KB Cache; 28KB system RAM

#define FLSH_CACHE_ENABLE            (1 << 6)

#define FLSH_CACHE_FLASH_MAP_SHIFT            (8)
#define FLSH_CACHE_FLASH_MAP(n)               (((n)&7)<<8)
#define FLSH_CACHE_FLASH_MAP_MASK             (7<<8)

#define FLSH_CACHE_MISS_CNT_EN		(1 << 12)
#define FLSH_CACHE_HIT_CNT_EN		(1 << 13)


//flush
#define FLSH_CACHE_LINE_FLUSH		(1 << 0)

//addr_offset
#define FLSH_CACHE_ADDR_OFFSET_SHIFT            (0)
#define FLSH_CACHE_ADDR_OFFSET(n)               (((n)&0xffffff)<<0)
#define FLSH_CACHE_ADDR_OFFSET_MASK             (0xffffff<<0)
#define FLSH_CACHE_ADDR_OFFSET_EN		(1 << 31)

//hitcnt
#define FLSH_CACHE_HIT_CNT_SHIFT            (0)
#define FLSH_CACHE_HIT_CNT(n)               (((n)&0xffff)<<0)
#define FLSH_CACHE_HIT_CNT_MASK             (0xffff<<0)

//misscnt
#define FLSH_CACHE_MISS_CNT_SHIFT            (0)
#define FLSH_CACHE_MISS_CNT(n)               (((n)&0xffff)<<0)
#define FLSH_CACHE_MISS_CNT_MASK             (0xffff<<0)

//status
#define FLSH_CACHE_STATUS_ADDR_RANGE_ERR  		    (1 << 0)
#define FLSH_CACHE_STATUS_WRITE_ERR  		        (1 << 1)
#define FLSH_CACHE_STATUS_CACHE_DISABLE_ERR  		(1 << 2)
#define FLSH_CACHE_STATUS_FLUSH_DONE  		        (1 << 3)

//irq
#define FLSH_CACHE_ADDR_RANGE_ERR_IRQ  		    (1 << 0)
#define FLSH_CACHE_WRITE_ERR_IRQ  		        (1 << 1)
#define FLSH_CACHE_CACHE_DISABLE_ERR_IRQ  		(1 << 2)
#define FLSH_CACHE_FLUSH_DONE_IRQ  		        (1 << 3)

//irq_mask
#define FLSH_CACHE_ADDR_RANGE_ERR_IRQ_MASK  		    (1 << 0)
#define FLSH_CACHE_WRITE_ERR_IRQ_MASK  		            (1 << 1)
#define FLSH_CACHE_CACHE_DISABLE_ERR_IRQ_MASK  		    (1 << 2)
#define FLSH_CACHE_FLUSH_DONE_IRQ_MASK  		        (1 << 3)

#endif

