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
//------------------------------------------------------------------------
// File name      : spiflash_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:55                                                  
// Last modified  : 2016-12-21 13:55:55                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:55 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SPIFLASH_ASM_H__
#define __SPIFLASH_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_r_rev_id                   REG_SPIFLASH_BASE+0x00
#define REG_tx_cmd_addr                REG_SPIFLASH_BASE+0x04
#define REG_trans_mode                 REG_SPIFLASH_BASE+0x08
#define REG_tx_fifo_data               REG_SPIFLASH_BASE+0x0C
#define REG_status                     REG_SPIFLASH_BASE+0x10
#define REG_rx_fifo_data               REG_SPIFLASH_BASE+0x14
#define REG_flash_config               REG_SPIFLASH_BASE+0x18
#define REG_fifo_ctrl                  REG_SPIFLASH_BASE+0x1C
#define REG_dual_spi                   REG_SPIFLASH_BASE+0x20
#define REG_r_read_cmd                 REG_SPIFLASH_BASE+0x24
#define REG_nand_cfg1                  REG_SPIFLASH_BASE+0x28
#define REG_nand_cfg2                  REG_SPIFLASH_BASE+0x2c
#define REG_dummy_cycle_cfg            REG_SPIFLASH_BASE+0x30
#define REG_diff_128m_cmd              REG_SPIFLASH_BASE+0x34

//r_rev_id
#define SPIFLASH_REV_ID(n)             (((n)&0xFF)<<0)     

//tx_cmd_addr
#define SPIFLASH_TX_CMD(n)             (((n)&0xFF)<<0)     
#define SPIFLASH_TX_ADDR(n)            (((n)&0xFFFFFF)<<8) 

//trans_mode
#define SPIFLASH_TX_MODE(n)            (((n)&0xFF)<<0)     
#define SPIFLASH_TX_BLOCK_SIZE(n)      (((n)&0x1FF)<<8)    
#define SPIFLASH_TX_CONT_MODE          (1<<24)             

//tx_fifo_data
#define SPIFLASH_TX_DATA(n)            (((n)&0xFF)<<0)     

//status
#define SPIFLASH_BUSY_STAT             (1<<0)              
#define SPIFLASH_TX_FIFO_EMPTY         (1<<1)              
#define SPIFLASH_TX_FIFO_FULL          (1<<2)              
#define SPIFLASH_RX_FIFO_EMPTY         (1<<3)              
#define SPIFLASH_RX_FIFO_CNT(n)        (((n)&0x1F)<<4)     
#define SPIFLASH_READ_STATE_BUSY       (1<<9)              

//rx_fifo_data
#define SPIFLASH_RX_DATA(n)            (((n)&0xFFFFFFFF)<<0)

//flash_config
#define SPIFLASH_QUAD_MODE             (1<<0)              
#define SPIFLASH_WPROTECT_PIN          (1<<1)              
#define SPIFLASH_HOLD_PIN              (1<<2)              
#define SPIFLASH_DUAL_MODE             (1<<3)              
#define SPIFLASH_SAMPLE_DELAY(n)       (((n)&0x7)<<4)      
#define SPIFLASH_DIN_CAP_EDGE          (1<<7)              
#define SPIFLASH_CLK_DIVIDER(n)        (((n)&0xFF)<<8)     
#define SPIFLASH_CMD_QUAD              (1<<16)             

//fifo_ctrl
#define SPIFLASH_RX_FIFO_CLR           (1<<0)              
#define SPIFLASH_TX_FIFO_CLR           (1<<1)              

//dual_spi
#define SPIFLASH_SPI_CS_NUM            (1<<0)              
#define SPIFLASH_SPI_SIZE(n)           (((n)&0x3)<<1)      
#define SPIFLASH_SPI_128M              (1<<3)              
#define SPIFLASH_AHB_READ_DISABLE      (1<<4)              
#define SPIFLASH_SPI_CS1_SEL           (1<<5)              
#define SPIFLASH_SPI_CS1_SEL1          (1<<6)              
#define SPIFLASH_DIFF_128M_DIFF_CMD_EN (1<<7)              

//r_read_cmd
#define SPIFLASH_QREAD_CMD(n)          (((n)&0xFF)<<0)     
#define SPIFLASH_FREAD_CMD(n)          (((n)&0xFF)<<8)     
#define SPIFLASH_READ_CMD(n)           (((n)&0xFF)<<16)    
#define SPIFLASH_DREAD_CMD(n)          (((n)&0xFF)<<24)    

//nand_cfg1
#define SPIFLASH_NAND_SEL              (1<<0)              
#define SPIFLASH_NAND_ADDR_26_25(n)    (((n)&0x3)<<1)      
#define SPIFLASH_PAGE_READ_CMD(n)      (((n)&0xFF)<<8)     
#define SPIFLASH_GET_STS_CMD(n)        (((n)&0xFF)<<16)    
#define SPIFLASH_RAM_READ_CMD(n)       (((n)&0xFF)<<24)    

//nand_cfg2
#define SPIFLASH_GET_STS_ADDR(n)       (((n)&0xFF)<<0)     
#define SPIFLASH_PROGRAM_EXE_CMD(n)    (((n)&0xFF)<<8)     
#define SPIFLASH_STST_QIP(n)           (((n)&0xFF)<<16)    

//dummy_cycle_cfg
#define SPIFLASH_FOUR_BYTE_ADDR        (1<<0)              
#define SPIFLASH_DUMMY_CYCLE_EN        (1<<1)              
#define SPIFLASH_TPD_SEL(n)            (((n)&0x3)<<2)      
#define SPIFLASH_DUMMY_CYCLE(n)        (((n)&0xF)<<8)      

//diff_128m_cmd
#define SPIFLASH_FIRST_128M_CMD(n)     (((n)&0xFF)<<0)     
#define SPIFLASH_SECOND_128M_CMD(n)    (((n)&0xFF)<<8)     
#define SPIFLASH_THIRD_128M_CMD(n)     (((n)&0xFF)<<16)    
#define SPIFLASH_FOURTH_128M_CMD(n)    (((n)&0xFF)<<24)    


#endif


