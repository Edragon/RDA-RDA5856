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
// File name      : spiflash                                            
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
#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define hwp_spi_flsh      hwp_spiflash
#define HWP_SPI_FLSH_T  T_HWP_SPIFLASH_T

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//r_rev_id
typedef union
{
    U32 v;
    struct
    {
        U32 rev_id                         :8 ; /*7 :0 , Revision ID                   */
        U32 r_rev_id_reserved_0            :24; /*31:8 , reserved                      */
    } b;
} T_SPIFLASH_r_rev_id;

//tx_cmd_addr
typedef union
{
    U32 v;
    struct
    {
        U32 tx_cmd                         :8 ; /*7 :0 , Transmit command              */
        U32 tx_addr                        :24; /*31:8 , Transmit address              */
    } b;
} T_SPIFLASH_tx_cmd_addr;

//trans_mode
typedef union
{
    U32 v;
    struct
    {
        U32 tx_mode                        :8 ; /*7 :0 , transmit mode bits            */
        U32 tx_block_size                  :9 ; /*16:8 , Transmit block size           */
        U32 trans_mode_reserved_1          :7 ; /*23:17, RESERVED                      */
        U32 tx_cont_mode                   :1 ; /*24:24, Tansmit continuse mode enable */
        U32 trans_mode_reserved_0          :7 ; /*31:25, RESERVED                      */
    } b;
} T_SPIFLASH_trans_mode;

//tx_fifo_data
typedef union
{
    U32 v;
    struct
    {
        U32 tx_data                        :8 ; /*7 :0 , Transmit FIFO data            */
        U32 tx_fifo_data_reserved_0        :24; /*31:8 , RESERVED                      */
    } b;
} T_SPIFLASH_tx_fifo_data;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 busy_stat                      :1 ; /*0 :0 , 0:: flash_fsm_idle1:: flash_fsm_busy*/
        U32 tx_fifo_empty                  :1 ; /*1 :1 , 0:: tx_fifo_not_empty1:: tx_fifo_empty*/
        U32 tx_fifo_full                   :1 ; /*2 :2 , 0:: tx_fifo_not_full1:: tx_fifo_full*/
        U32 rx_fifo_empty                  :1 ; /*3 :3 , 0:: rx_fifo_not_empty1:: rx_fifo_empty*/
        U32 rx_fifo_cnt                    :5 ; /*8 :4 , Receive FIFO count            */
        U32 read_state_busy                :1 ; /*9 :9 , 0:: ahb_fsm_idle1:: ahb_fsm_busy*/
        U32 status_reserved_0              :22; /*31:10, RESERVED                      */
    } b;
} T_SPIFLASH_status;

//rx_fifo_data
typedef union
{
    U32 v;
    struct
    {
        U32 rx_data                        :32; /*31:0 , Receive FIFO data             */
    } b;
} T_SPIFLASH_rx_fifo_data;

//flash_config
typedef union
{
    U32 v;
    struct
    {
        U32 quad_mode                      :1 ; /*0 :0 , Quad mode0:: not quad_mode1:: quad_mode*/
        U32 wprotect_pin                   :1 ; /*1 :1 , Set wproctect pin active0:: wpro_pin_as_data_pin1:: wpro_pin_active*/
        U32 hold_pin                       :1 ; /*2 :2 , Set hold pin active0:: hold_pin_as_data_pin1:: hold_pin_active*/
        U32 dual_mode                      :1 ; /*3 :3 , Dual mode0:: not_dual_mode1:: dual_mode*/
        U32 sample_delay                   :3 ; /*6 :4 , spi data input Sample delay; could cowork with din_cap_edge0:: 0_internal_sclk_delay1:: 1_internal_sclk_delay2:: 2_internal_sclk_delay3:: 3_internal_sclk_delay*/
        U32 din_cap_edge                   :1 ; /*7 :7 , spi data input capture edge0:: posedge_of_internal_sclk1:: negedge_of_internal_sclk*/
        U32 clk_divider                    :8 ; /*15:8 , Clock divider0:: divide_by_11:: divide_by_12:: divide_by_23:: divide_by_34:: divide_by_45:: divide_by_56:: divide_by_67:: divide_by_78:: divide_by_8*/
        U32 cmd_quad                       :1 ; /*16:16, Quad Command EnableHardware will regard current command as quad commond if this bit is active*/
        U32 flash_config_reserved_0        :15; /*31:17, RESERVED                      */
    } b;
} T_SPIFLASH_flash_config;

//fifo_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 rx_fifo_clr                    :1 ; /*0 :0 , write 1 to clear rx fifo      */
        U32 tx_fifo_clr                    :1 ; /*1 :1 , write 1 to clear tx fifo      */
        U32 fifo_ctrl_reserved_0           :30; /*31:2 , RESERVED                      */
    } b;
} T_SPIFLASH_fifo_ctrl;

//dual_spi
typedef union
{
    U32 v;
    struct
    {
        U32 spi_cs_num                     :1 ; /*0 :0 , spi cs number1'b0:: one_cs1'b1:: two_cs*/
        U32 spi_size                       :2 ; /*2 :1 , spi flash size 2'b00:: 32M_bits_flash2'b01:: 64M_bits_flash2'b10:: 16M_bits_flash2'b11:: 8M_bits_flash*/
        U32 spi_128m                       :1 ; /*3 :3 , 128M bits flash               */
        U32 ahb_read_disable               :1 ; /*4 :4 , No description.               */
        U32 spi_cs1_sel                    :1 ; /*5 :5 , No description.               */
        U32 spi_cs1_sel1                   :1 ; /*6 :6 , No description.               */
        U32 diff_128m_diff_cmd_en          :1 ; /*7 :7 , No description.               */
        U32 dual_spi_reserved_0            :24; /*31:8 , RESERVED                      */
    } b;
} T_SPIFLASH_dual_spi;

//r_read_cmd
typedef union
{
    U32 v;
    struct
    {
        U32 qread_cmd                      :8 ; /*7 :0 , quad read command             */
        U32 fread_cmd                      :8 ; /*15:8 , Fast read command             */
        U32 read_cmd                       :8 ; /*23:16, Read command                  */
        U32 dread_cmd                      :8 ; /*31:24, Dual read command             */
    } b;
} T_SPIFLASH_r_read_cmd;

//nand_cfg1
typedef union
{
    U32 v;
    struct
    {
        U32 nand_sel                       :1 ; /*0 :0 , Internal nand flash selection.Nand flash also can be selected from pin1'b0:: norflash1'b1:: nandflash*/
        U32 nand_addr_26_25                :2 ; /*2 :1 , RESERVED                      */
        U32 nand_cfg1_reserved_0           :5 ; /*7 :3 , RESERVED                      */
        U32 page_read_cmd                  :8 ; /*15:8 , Page read command             */
        U32 get_sts_cmd                    :8 ; /*23:16, Get status command            */
        U32 ram_read_cmd                   :8 ; /*31:24, RAM read command              */
    } b;
} T_SPIFLASH_nand_cfg1;

//nand_cfg2
typedef union
{
    U32 v;
    struct
    {
        U32 get_sts_addr                   :8 ; /*7 :0 , Get status address            */
        U32 program_exe_cmd                :8 ; /*15:8 , Program execute command       */
        U32 stst_qip                       :8 ; /*23:16, No description.               */
        U32 nand_cfg2_reserved_0           :8 ; /*31:24, RESERVED                      */
    } b;
} T_SPIFLASH_nand_cfg2;

//dummy_cycle_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 four_byte_addr                 :1 ; /*0 :0 , Four bytes addess1'b0:: 3_bytes_address1'b1:: 4_bytes_address*/
        U32 dummy_cycle_en                 :1 ; /*1 :1 , Dummy cycle enable            */
        U32 tpd_sel                        :2 ; /*3 :2 , tport selector                */
        U32 dummy_cycle_cfg_reserved_1     :4 ; /*7 :4 , RESERVED                      */
        U32 dummy_cycle                    :4 ; /*11:8 , Dummy cycle count             */
        U32 dummy_cycle_cfg_reserved_0     :20; /*31:12, RESERVED                      */
    } b;
} T_SPIFLASH_dummy_cycle_cfg;

//diff_128m_cmd
typedef union
{
    U32 v;
    struct
    {
        U32 first_128m_cmd                 :8 ; /*7 :0 , 1st 128M flash command        */
        U32 second_128m_cmd                :8 ; /*15:8 , 2nd 128M flash command        */
        U32 third_128m_cmd                 :8 ; /*23:16, 3rd 128M flash command        */
        U32 fourth_128m_cmd                :8 ; /*31:24, 4th 128M flash command        */
    } b;
} T_SPIFLASH_diff_128m_cmd;

typedef struct
{
    volatile             T_SPIFLASH_r_rev_id                       r_rev_id; /*0x0  , RO   , 0x00000011, Revision ID                   */
    volatile          T_SPIFLASH_tx_cmd_addr                    tx_cmd_addr; /*0x4  , RW   , 0x00000000, TX Command and Address Register*/
    volatile           T_SPIFLASH_trans_mode                     trans_mode; /*0x8  , RW   , 0x00000100, Transfer Mode Register        */
    volatile         T_SPIFLASH_tx_fifo_data                   tx_fifo_data; /*0xc  , RW   , 0x00000000, Transmit FIFO Data Register   */
    volatile               T_SPIFLASH_status                         status; /*0x10 , RO   , 0x0000000A, Status Register               */
    volatile         T_SPIFLASH_rx_fifo_data                   rx_fifo_data; /*0x14 , RO   , 0x00000000, Receive FIFO Data Register    */
    volatile         T_SPIFLASH_flash_config                   flash_config; /*0x18 , RW   , 0x00000110, Flash Configuration Register  */
    volatile            T_SPIFLASH_fifo_ctrl                      fifo_ctrl; /*0x1c , RW   , 0x00000000, FIFO Control Register         */
    volatile             T_SPIFLASH_dual_spi                       dual_spi; /*0x20 , RW   , 0x00000000, Data SPI Register             */
    volatile           T_SPIFLASH_r_read_cmd                     r_read_cmd; /*0x24 , RW   , 0x3B030BEB, Read Command Register         */
    volatile            T_SPIFLASH_nand_cfg1                      nand_cfg1; /*0x28 , RW   , 0x030F1300, Nand Configuration Register 1 */
    volatile            T_SPIFLASH_nand_cfg2                      nand_cfg2; /*0x2c , RW   , 0x000110C0, Nand Configuration Register 2 */
    volatile      T_SPIFLASH_dummy_cycle_cfg                dummy_cycle_cfg; /*0x30 , RW   , 0x00000800, Dummy Cycle Configuration Register*/
    volatile        T_SPIFLASH_diff_128m_cmd                  diff_128m_cmd; /*0x34 , RW   , 0x00008D8C, 256/512M Flash Configuration Register*/
} T_HWP_SPIFLASH_T;
#else
typedef struct
{
    volatile                             U32                       r_rev_id; /*0x0  , RO   , 0x00000011, Revision ID                   */
    volatile                             U32                    tx_cmd_addr; /*0x4  , RW   , 0x00000000, TX Command and Address Register*/
    volatile                             U32                     trans_mode; /*0x8  , RW   , 0x00000100, Transfer Mode Register        */
    volatile                             U32                   tx_fifo_data; /*0xc  , RW   , 0x00000000, Transmit FIFO Data Register   */
    volatile                             U32                         status; /*0x10 , RO   , 0x0000000A, Status Register               */
    volatile                             U32                   rx_fifo_data; /*0x14 , RO   , 0x00000000, Receive FIFO Data Register    */
    volatile                             U32                   flash_config; /*0x18 , RW   , 0x00000110, Flash Configuration Register  */
    volatile                             U32                      fifo_ctrl; /*0x1c , RW   , 0x00000000, FIFO Control Register         */
    volatile                             U32                       dual_spi; /*0x20 , RW   , 0x00000000, Data SPI Register             */
    volatile                             U32                     r_read_cmd; /*0x24 , RW   , 0x3B030BEB, Read Command Register         */
    volatile                             U32                      nand_cfg1; /*0x28 , RW   , 0x030F1300, Nand Configuration Register 1 */
    volatile                             U32                      nand_cfg2; /*0x2c , RW   , 0x000110C0, Nand Configuration Register 2 */
    volatile                             U32                dummy_cycle_cfg; /*0x30 , RW   , 0x00000800, Dummy Cycle Configuration Register*/
    volatile                             U32                  diff_128m_cmd; /*0x34 , RW   , 0x00008D8C, 256/512M Flash Configuration Register*/
} T_HWP_SPIFLASH_T;

//r_rev_id
#define SPIFLASH_REV_ID(n)             (((n)&0xFF)<<0)     
#define SPIFLASH_REV_ID_MASK           (0xFF<<0)           
#define SPIFLASH_REV_ID_SHIFT          (0)                 

//tx_cmd_addr
#define SPIFLASH_TX_CMD(n)             (((n)&0xFF)<<0)     
#define SPIFLASH_TX_CMD_MASK           (0xFF<<0)           
#define SPIFLASH_TX_CMD_SHIFT          (0)                 

#define SPIFLASH_TX_ADDR(n)            (((n)&0xFFFFFF)<<8) 
#define SPIFLASH_TX_ADDR_MASK          (0xFFFFFF<<8)       
#define SPIFLASH_TX_ADDR_SHIFT         (8)                 

//trans_mode
#define SPIFLASH_TX_MODE(n)            (((n)&0xFF)<<0)     
#define SPIFLASH_TX_MODE_MASK          (0xFF<<0)           
#define SPIFLASH_TX_MODE_SHIFT         (0)                 

#define SPIFLASH_TX_BLOCK_SIZE(n)      (((n)&0x1FF)<<8)    
#define SPIFLASH_TX_BLOCK_SIZE_MASK    (0x1FF<<8)          
#define SPIFLASH_TX_BLOCK_SIZE_SHIFT   (8)                 

#define SPIFLASH_TX_CONT_MODE          (1<<24)             

//tx_fifo_data
#define SPIFLASH_TX_DATA(n)            (((n)&0xFF)<<0)     
#define SPIFLASH_TX_DATA_MASK          (0xFF<<0)           
#define SPIFLASH_TX_DATA_SHIFT         (0)                 

//status
#define SPIFLASH_BUSY_STAT             (1<<0)              
#define SPIFLASH_BUSY_STAT_FLASH_FSM_IDLE (0<<0)              
#define SPIFLASH_BUSY_STAT_FLASH_FSM_BUSY (1<<0)              

#define SPIFLASH_TX_FIFO_EMPTY         (1<<1)              
#define SPIFLASH_TX_FIFO_EMPTY_TX_FIFO_NOT_EMPTY (0<<1)              
#define SPIFLASH_TX_FIFO_EMPTY_TX_FIFO_EMPTY (1<<1)              

#define SPIFLASH_TX_FIFO_FULL          (1<<2)              
#define SPIFLASH_TX_FIFO_FULL_TX_FIFO_NOT_FULL (0<<2)              
#define SPIFLASH_TX_FIFO_FULL_TX_FIFO_FULL (1<<2)              

#define SPIFLASH_RX_FIFO_EMPTY         (1<<3)              
#define SPIFLASH_RX_FIFO_EMPTY_RX_FIFO_NOT_EMPTY (0<<3)              
#define SPIFLASH_RX_FIFO_EMPTY_RX_FIFO_EMPTY (1<<3)              

#define SPIFLASH_RX_FIFO_CNT(n)        (((n)&0x1F)<<4)     
#define SPIFLASH_RX_FIFO_CNT_MASK      (0x1F<<4)           
#define SPIFLASH_RX_FIFO_CNT_SHIFT     (4)                 

#define SPIFLASH_READ_STATE_BUSY       (1<<9)              
#define SPIFLASH_READ_STATE_BUSY_AHB_FSM_IDLE (0<<9)              
#define SPIFLASH_READ_STATE_BUSY_AHB_FSM_BUSY (1<<9)              

//rx_fifo_data
#define SPIFLASH_RX_DATA(n)            (((n)&0xFFFFFFFF)<<0)
#define SPIFLASH_RX_DATA_MASK          (0xFFFFFFFF<<0)     
#define SPIFLASH_RX_DATA_SHIFT         (0)                 

//flash_config
#define SPIFLASH_QUAD_MODE             (1<<0)              

#define SPIFLASH_WPROTECT_PIN          (1<<1)              
#define SPIFLASH_WPROTECT_PIN_WPRO_PIN_AS_DATA_PIN (0<<1)              
#define SPIFLASH_WPROTECT_PIN_WPRO_PIN_ACTIVE (1<<1)              

#define SPIFLASH_HOLD_PIN              (1<<2)              
#define SPIFLASH_HOLD_PIN_HOLD_PIN_AS_DATA_PIN (0<<2)              
#define SPIFLASH_HOLD_PIN_HOLD_PIN_ACTIVE (1<<2)              

#define SPIFLASH_DUAL_MODE             (1<<3)              
#define SPIFLASH_DUAL_MODE_NOT_DUAL_MODE (0<<3)              
#define SPIFLASH_DUAL_MODE_DUAL_MODE   (1<<3)              

#define SPIFLASH_SAMPLE_DELAY(n)       (((n)&0x7)<<4)      
#define SPIFLASH_SAMPLE_DELAY_0_INTERNAL_SCLK_DELAY (((0)&0x7)<<4)      
#define SPIFLASH_SAMPLE_DELAY_1_INTERNAL_SCLK_DELAY (((1)&0x7)<<4)      
#define SPIFLASH_SAMPLE_DELAY_2_INTERNAL_SCLK_DELAY (((2)&0x7)<<4)      
#define SPIFLASH_SAMPLE_DELAY_3_INTERNAL_SCLK_DELAY (((3)&0x7)<<4)      
#define SPIFLASH_SAMPLE_DELAY_MASK     (0x7<<4)            
#define SPIFLASH_SAMPLE_DELAY_SHIFT    (4)                 

#define SPIFLASH_DIN_CAP_EDGE          (1<<7)              
#define SPIFLASH_DIN_CAP_EDGE_POSEDGE_OF_INTERNAL_SCLK (0<<7)              
#define SPIFLASH_DIN_CAP_EDGE_NEGEDGE_OF_INTERNAL_SCLK (1<<7)              

#define SPIFLASH_CLK_DIVIDER(n)        (((n)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_0 (((0)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_1 (((1)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_2 (((2)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_3 (((3)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_4 (((4)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_5 (((5)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_6 (((6)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_7 (((7)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_DIVIDE_BY_8 (((8)&0xFF)<<8)     
#define SPIFLASH_CLK_DIVIDER_MASK      (0xFF<<8)           
#define SPIFLASH_CLK_DIVIDER_SHIFT     (8)                 

#define SPIFLASH_CMD_QUAD              (1<<16)             

//fifo_ctrl
#define SPIFLASH_RX_FIFO_CLR           (1<<0)              

#define SPIFLASH_TX_FIFO_CLR           (1<<1)              

//dual_spi
#define SPIFLASH_SPI_CS_NUM            (1<<0)              
#define SPIFLASH_SPI_CS_NUM_ONE_CS     (0<<0)              
#define SPIFLASH_SPI_CS_NUM_TWO_CS     (1<<0)              

#define SPIFLASH_SPI_SIZE(n)           (((n)&0x3)<<1)      
#define SPIFLASH_SPI_SIZE_32M_BITS_FLASH (((0)&0x3)<<1)      
#define SPIFLASH_SPI_SIZE_64M_BITS_FLASH (((1)&0x3)<<1)      
#define SPIFLASH_SPI_SIZE_16M_BITS_FLASH (((2)&0x3)<<1)      
#define SPIFLASH_SPI_SIZE_8M_BITS_FLASH (((3)&0x3)<<1)      
#define SPIFLASH_SPI_SIZE_MASK         (0x3<<1)            
#define SPIFLASH_SPI_SIZE_SHIFT        (1)                 

#define SPIFLASH_SPI_128M              (1<<3)              

#define SPIFLASH_AHB_READ_DISABLE      (1<<4)              

#define SPIFLASH_SPI_CS1_SEL           (1<<5)              

#define SPIFLASH_SPI_CS1_SEL1          (1<<6)              

#define SPIFLASH_DIFF_128M_DIFF_CMD_EN (1<<7)              

//r_read_cmd
#define SPIFLASH_QREAD_CMD(n)          (((n)&0xFF)<<0)     
#define SPIFLASH_QREAD_CMD_MASK        (0xFF<<0)           
#define SPIFLASH_QREAD_CMD_SHIFT       (0)                 

#define SPIFLASH_FREAD_CMD(n)          (((n)&0xFF)<<8)     
#define SPIFLASH_FREAD_CMD_MASK        (0xFF<<8)           
#define SPIFLASH_FREAD_CMD_SHIFT       (8)                 

#define SPIFLASH_READ_CMD(n)           (((n)&0xFF)<<16)    
#define SPIFLASH_READ_CMD_MASK         (0xFF<<16)          
#define SPIFLASH_READ_CMD_SHIFT        (16)                

#define SPIFLASH_DREAD_CMD(n)          (((n)&0xFF)<<24)    
#define SPIFLASH_DREAD_CMD_MASK        (0xFF<<24)          
#define SPIFLASH_DREAD_CMD_SHIFT       (24)                

//nand_cfg1
#define SPIFLASH_NAND_SEL              (1<<0)              
#define SPIFLASH_NAND_SEL_NORFLASH     (0<<0)              
#define SPIFLASH_NAND_SEL_NANDFLASH    (1<<0)              

#define SPIFLASH_NAND_ADDR_26_25(n)    (((n)&0x3)<<1)      
#define SPIFLASH_NAND_ADDR_26_25_MASK  (0x3<<1)            
#define SPIFLASH_NAND_ADDR_26_25_SHIFT (1)                 

#define SPIFLASH_PAGE_READ_CMD(n)      (((n)&0xFF)<<8)     
#define SPIFLASH_PAGE_READ_CMD_MASK    (0xFF<<8)           
#define SPIFLASH_PAGE_READ_CMD_SHIFT   (8)                 

#define SPIFLASH_GET_STS_CMD(n)        (((n)&0xFF)<<16)    
#define SPIFLASH_GET_STS_CMD_MASK      (0xFF<<16)          
#define SPIFLASH_GET_STS_CMD_SHIFT     (16)                

#define SPIFLASH_RAM_READ_CMD(n)       (((n)&0xFF)<<24)    
#define SPIFLASH_RAM_READ_CMD_MASK     (0xFF<<24)          
#define SPIFLASH_RAM_READ_CMD_SHIFT    (24)                

//nand_cfg2
#define SPIFLASH_GET_STS_ADDR(n)       (((n)&0xFF)<<0)     
#define SPIFLASH_GET_STS_ADDR_MASK     (0xFF<<0)           
#define SPIFLASH_GET_STS_ADDR_SHIFT    (0)                 

#define SPIFLASH_PROGRAM_EXE_CMD(n)    (((n)&0xFF)<<8)     
#define SPIFLASH_PROGRAM_EXE_CMD_MASK  (0xFF<<8)           
#define SPIFLASH_PROGRAM_EXE_CMD_SHIFT (8)                 

#define SPIFLASH_STST_QIP(n)           (((n)&0xFF)<<16)    
#define SPIFLASH_STST_QIP_MASK         (0xFF<<16)          
#define SPIFLASH_STST_QIP_SHIFT        (16)                

//dummy_cycle_cfg
#define SPIFLASH_FOUR_BYTE_ADDR        (1<<0)              
#define SPIFLASH_FOUR_BYTE_ADDR_3_BYTES_ADDRESS (0<<0)              
#define SPIFLASH_FOUR_BYTE_ADDR_4_BYTES_ADDRESS (1<<0)              

#define SPIFLASH_DUMMY_CYCLE_EN        (1<<1)              

#define SPIFLASH_TPD_SEL(n)            (((n)&0x3)<<2)      
#define SPIFLASH_TPD_SEL_MASK          (0x3<<2)            
#define SPIFLASH_TPD_SEL_SHIFT         (2)                 

#define SPIFLASH_DUMMY_CYCLE(n)        (((n)&0xF)<<8)      
#define SPIFLASH_DUMMY_CYCLE_MASK      (0xF<<8)            
#define SPIFLASH_DUMMY_CYCLE_SHIFT     (8)                 

//diff_128m_cmd
#define SPIFLASH_FIRST_128M_CMD(n)     (((n)&0xFF)<<0)     
#define SPIFLASH_FIRST_128M_CMD_MASK   (0xFF<<0)           
#define SPIFLASH_FIRST_128M_CMD_SHIFT  (0)                 

#define SPIFLASH_SECOND_128M_CMD(n)    (((n)&0xFF)<<8)     
#define SPIFLASH_SECOND_128M_CMD_MASK  (0xFF<<8)           
#define SPIFLASH_SECOND_128M_CMD_SHIFT (8)                 

#define SPIFLASH_THIRD_128M_CMD(n)     (((n)&0xFF)<<16)    
#define SPIFLASH_THIRD_128M_CMD_MASK   (0xFF<<16)          
#define SPIFLASH_THIRD_128M_CMD_SHIFT  (16)                

#define SPIFLASH_FOURTH_128M_CMD(n)    (((n)&0xFF)<<24)    
#define SPIFLASH_FOURTH_128M_CMD_MASK  (0xFF<<24)          
#define SPIFLASH_FOURTH_128M_CMD_SHIFT (24)                


#endif

#endif


