//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : sdmmc                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:53                                                  
// Last modified  : 2016-08-29 14:59:53                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:53 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SDMMC_H__
#define __SDMMC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//sd_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 l_endian                       :3 ; /*2 :0 , Endian TypeSDMMC TX FIFO Endian3'b000: {pwdata_byte[3],pwdata_byte[2],pwdata_byte[1],pwdata_byte[0]}3'b001: {pwdata_byte[0],pwdata_byte[1],pwdata_byte[2],pwdata_byte[3]}3'b010: {pwdata_byte[1],pwdata_byte[0],pwdata_byte[3],pwdata_byte[2]}3'b011: pwdata_bit[0-31]3'b100: {pwdata_byte[2],pwdata_byte[3],pwdata_byte[0],pwdata_byte[1]}default: 32'h0*/
        U32 soft_rst_l                     :1 ; /*3 :3 , Soft Reset Lowsoftware reset for TX &amp; RX FIFOs of SDMMC, Low active*/
        U32 sd_ctrl_reserved_0             :28; /*31:4 , RESERVED                      */
    } b;
} T_SDMMC_sd_ctrl;

//sd_txfifo
typedef union
{
    U32 v;
    struct
    {
        U32 txfifo_wdata                   :32; /*31:0 , Tx FIFO Wdatawrite data for 4x32 bit TxFIFO*/
    } b;
} T_SDMMC_sd_txfifo;

//sd_rxfifo
typedef union
{
    U32 v;
    struct
    {
        U32 rxfifo_rdata                   :32; /*31:0 , Rx FIFO RdataRead data from 4x32 bit RxFIFO*/
    } b;
} T_SDMMC_sd_rxfifo;

//sd_config
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_sendcmd                  :1 ; /*0 :0 , configuration begin           */
        U32 sdmmc_suspend                  :1 ; /*1 :1 , sdmmc begin                   */
        U32 sd_config_reserved_3           :2 ; /*3 :2 , RESERVED                      */
        U32 rsp_en                         :1 ; /*4 :4 , rsp en                        */
        U32 rsp_sel                        :2 ; /*6 :5 , Response select register,"10" means R2 response, "01" means R3 response, "00" means others response, "11" is reserved.2:R21:R30:OTHER*/
        U32 sd_config_reserved_2           :1 ; /*7 :7 , RESERVED                      */
        U32 rd_wt_en                       :1 ; /*8 :8 , 1' means write operation,'0' means read operation.*/
        U32 rd_wt_sel                      :1 ; /*9 :9 , 1' means write operation,'0' means read operation.0:READ1:WRITE*/
        U32 s_m_sel                        :1 ; /*10:10, 1'means multiple block data operation.0:SIMPLE1:MULTIPLE*/
        U32 sd_config_reserved_1           :5 ; /*15:11, RESERVED                      */
        U32 auto_flag_en                   :1 ; /*16:16, auto cmd12 enable             */
        U32 sd_config_reserved_0           :15; /*31:17, RESERVED                      */
    } b;
} T_SDMMC_sd_config;

//sd_status
typedef union
{
    U32 v;
    struct
    {
        U32 not_sdmmc_over                 :1 ; /*0 :0 , 1' means the SD/MMC operation is not over.*/
        U32 busy                           :1 ; /*1 :1 , 1' means SD/MMC is busy.      */
        U32 dl_busy                        :1 ; /*2 :2 , 1' means the data line is busy.*/
        U32 suspend                        :1 ; /*3 :3 , 1' means the controller will not perform the new command when SDMMC_SENDCMD= '1'.*/
        U32 sd_status_reserved_3           :3 ; /*6 :4 , RESERVED                      */
        U32 not_sdmmc_over_mask            :1 ; /*7 :7 , mask not_sdmmc_over bit       */
        U32 rsp_error                      :1 ; /*8 :8 , Response CRC checks error register '1' means response CRC check error.*/
        U32 no_rsp_error                   :1 ; /*9 :9 , 1' means the card has no response to command.*/
        U32 sd_status_reserved_2           :2 ; /*11:10, RESERVED                      */
        U32 crc_status                     :3 ; /*14:12, CRC check for SD/MMC write operation "101" transmission error"010" transmission right"111" flash programming error*/
        U32 sd_status_reserved_1           :1 ; /*15:15, RESERVED                      */
        U32 data_error                     :8 ; /*23:16, 8 bits data CRC check, "00000000" means no data error, "00000001" means DATA0 CRC check error, "10000000" means DATA7 CRC check error, each bit match one data line.*/
        U32 dat3_val                       :1 ; /*24:24, data input bit3               */
        U32 dat0_val                       :1 ; /*25:25, data input bit0               */
        U32 sd_status_reserved_0           :6 ; /*31:26, RESERVED                      */
    } b;
} T_SDMMC_sd_status;

//sd_cmd_index
typedef union
{
    U32 v;
    struct
    {
        U32 command                        :6 ; /*5 :0 , SD/MMC command register.      */
        U32 sd_cmd_index_reserved_0        :26; /*31:6 , RESERVED                      */
    } b;
} T_SDMMC_sd_cmd_index;

//sd_cmd_arg
typedef union
{
    U32 v;
    struct
    {
        U32 argument                       :32; /*31:0 , SD/MMC command argument register, write data to the SD/MMC card.*/
    } b;
} T_SDMMC_sd_cmd_arg;

//sd_rsp_index
typedef union
{
    U32 v;
    struct
    {
        U32 response                       :6 ; /*5 :0 , SD/MMC response index register.*/
        U32 sd_rsp_index_reserved_0        :26; /*31:6 , RESERVED                      */
    } b;
} T_SDMMC_sd_rsp_index;

//sd_rsp_arg3
typedef union
{
    U32 v;
    struct
    {
        U32 argument3                      :32; /*31:0 , Response argument of R1, R3 and R6, or 127 to 96 bit response argument of R2.*/
    } b;
} T_SDMMC_sd_rsp_arg3;

//sd_rsp_arg2
typedef union
{
    U32 v;
    struct
    {
        U32 argument2                      :32; /*31:0 , 95 to 64 bit response argument of R2.*/
    } b;
} T_SDMMC_sd_rsp_arg2;

//sd_rsp_arg1
typedef union
{
    U32 v;
    struct
    {
        U32 argument1                      :32; /*31:0 , 63 to 32 bit response argument of R2.*/
    } b;
} T_SDMMC_sd_rsp_arg1;

//sd_rsp_arg0
typedef union
{
    U32 v;
    struct
    {
        U32 argument0                      :32; /*31:0 , 31 to 0 bit response argument of R2.*/
    } b;
} T_SDMMC_sd_rsp_arg0;

//sd_data_width
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_data_width               :4 ; /*3 :0 , SD/MMC data width:0x1: 1 data line0x2: 2 reserved0x4: 4 data lines0x8: 8 data lines*/
        U32 sd_data_width_reserved_0       :28; /*31:4 , RESERVED                      */
    } b;
} T_SDMMC_sd_data_width;

//sd_block_size
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_block_size               :4 ; /*3 :0 , SD/MMC size of one block:0-1:reserved2: 1 word3: 2 words4: 4 words5: 8 words6: 16 words11: 512 words12-15 reserved*/
        U32 sd_block_size_reserved_0       :28; /*31:4 , RESERVED                      */
    } b;
} T_SDMMC_sd_block_size;

//sd_block_cnt
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_block_cnt                :16; /*15:0 , Block number that wants to transfer.*/
        U32 sd_block_cnt_reserved_0        :16; /*31:16, RESERVED                      */
    } b;
} T_SDMMC_sd_block_cnt;

//sd_int_status
typedef union
{
    U32 v;
    struct
    {
        U32 no_rsp_int                     :1 ; /*0 :0 , 1' means no response.         */
        U32 rsp_err_int                    :1 ; /*1 :1 , 1' means CRC error of response.*/
        U32 rd_err_int                     :1 ; /*2 :2 , 1' means CRC error of reading data.*/
        U32 wr_err_int                     :1 ; /*3 :3 , 1' means CRC error of writing data.*/
        U32 dat_over_int                   :1 ; /*4 :4 , read write correct int raw status*/
        U32 txdma_done_int                 :1 ; /*5 :5 , 1' means rx dma done.         */
        U32 rxdma_done_int                 :1 ; /*6 :6 , 1' means tx dma done.         */
        U32 sd_int_status_reserved_2       :1 ; /*7 :7 , RESERVED                      */
        U32 no_rsp_sc                      :1 ; /*8 :8 , 1' means no response is the source of interrupt.*/
        U32 rsp_err_sc                     :1 ; /*9 :9 , 1' means CRC error of response is the source of interrupt.*/
        U32 rd_err_sc                      :1 ; /*10:10, 1' means CRC error of reading data is the source of interrupt.*/
        U32 wr_err_sc                      :1 ; /*11:11, 1' means CRC error of writing data is the source of interrupt.*/
        U32 dat_over_sc                    :1 ; /*12:12, read write correct int status after masking*/
        U32 txdma_done_sc                  :1 ; /*13:13, 1' means rx dma done is the source of interrupt.*/
        U32 rxdma_done_sc                  :1 ; /*14:14, 1' means tx dma done is the source of interrupt.*/
        U32 sd_int_status_reserved_1       :1 ; /*15:15, RESERVED                      */
        U32 sd_int_status_reserved_0       :16; /*31:16, RESERVED                      */
    } b;
} T_SDMMC_sd_int_status;

//sd_int_mask
typedef union
{
    U32 v;
    struct
    {
        U32 no_rsp_mk                      :1 ; /*0 :0 , When no response, '1' means INT is disable.*/
        U32 rsp_err_mk                     :1 ; /*1 :1 , When CRC error of response, '1' means INT is disable.*/
        U32 rd_err_mk                      :1 ; /*2 :2 , When CRC error of reading data, '1' means INT is disable.*/
        U32 wr_err_mk                      :1 ; /*3 :3 , When CRC error of writing data, '1' means INT is disable.*/
        U32 dat_over_mk                    :1 ; /*4 :4 , When data transmission is over, '1' means INT is disable.*/
        U32 txdma_done_mk                  :1 ; /*5 :5 , when tx dma done, '1' means INT is disabled.*/
        U32 rxdma_done_mk                  :1 ; /*6 :6 , 1' means rx dma done, '1' means INT is disabled.*/
        U32 sd_int_mask_reserved_0         :25; /*31:7 , RESERVED                      */
    } b;
} T_SDMMC_sd_int_mask;

//sd_int_clear
typedef union
{
    U32 v;
    struct
    {
        U32 no_rsp_cl                      :1 ; /*0 :0 , Write a '1' to this bit to clear the source of interrupt in NO_RSP_SC.*/
        U32 rsp_err_cl                     :1 ; /*1 :1 , Write a '1' to this bit to clear the source of interrupt in RSP_ERR_SC.*/
        U32 rd_err_cl                      :1 ; /*2 :2 , Write a '1' to this bit to clear the source of interrupt in RD_ERR_SC.*/
        U32 wr_err_cl                      :1 ; /*3 :3 , Write a '1' to this bit to clear the source of interrupt in WR_ERR_SC.*/
        U32 dat_over_cl                    :1 ; /*4 :4 , No description.               */
        U32 txdma_done_cl                  :1 ; /*5 :5 , Write a '1' to this bit to clear the source of interrupt in RXDMA_DONE_SC.*/
        U32 rxdma_done_cl                  :1 ; /*6 :6 , Write a '1' to this bit to clear the source of interrupt in TXDMA_DONE_SC.*/
        U32 sd_int_clear_reserved_0        :25; /*31:7 , RESERVED                      */
    } b;
} T_SDMMC_sd_int_clear;

//sd_trans_speed
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_trans_speed              :8 ; /*7 :0 , Mclk = Pclk/(2*(SDMMC_TRANS_SPEED +1)).*/
        U32 sd_trans_speed_reserved_0      :24; /*31:8 , RESERVED                      */
    } b;
} T_SDMMC_sd_trans_speed;

//sd_mclk_adjust
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_mclk_adjust              :4 ; /*3 :0 , This register may delay the mclk output. When MCLK_ADJUSTER = n, Mclk is outputted with n Pclk.*/
        U32 clk_inv                        :1 ; /*4 :4 , mclk polarity                 */
        U32 sd_mclk_adjust_reserved_1      :3 ; /*7 :5 , RESERVED                      */
        U32 mclk_dis                       :1 ; /*8 :8 , mclk disable                  */
        U32 sd_mclk_adjust_reserved_0      :23; /*31:9 , RESERVED                      */
    } b;
} T_SDMMC_sd_mclk_adjust;

typedef struct
{
    volatile                 T_SDMMC_sd_ctrl                        sd_ctrl; /*0x0  , RW   , 0x00000008, SDMMC Control Register        */
    volatile                             U32                     reserved_1;
    volatile               T_SDMMC_sd_txfifo                      sd_txfifo; /*0x8  , WO   , 0x00000000, SDMMC Tx FIFO Register        */
    volatile               T_SDMMC_sd_rxfifo                      sd_rxfifo; /*0xc  , RO   , 0x00000000, SDMMC Rx FIFO Register        */
    volatile                             U32                reserved_2[508];
    volatile               T_SDMMC_sd_config                      sd_config; /*0x800, RW   , 0x00010002, SDMMC Configuration Register  */
    volatile               T_SDMMC_sd_status                      sd_status; /*0x804, RO   , 0x00000008, SDMMC Status Register         */
    volatile            T_SDMMC_sd_cmd_index                   sd_cmd_index; /*0x808, RW   , 0x00000000, SDMMC Command Index Register  */
    volatile              T_SDMMC_sd_cmd_arg                     sd_cmd_arg; /*0x80c, RW   , 0x00000000, SDMMC Command Argument Register*/
    volatile            T_SDMMC_sd_rsp_index                   sd_rsp_index; /*0x810, RO   , 0x00000000, SDMMC Response Index Register */
    volatile             T_SDMMC_sd_rsp_arg3                    sd_rsp_arg3; /*0x814, RO   , 0x00000000, SDMMC Response Argument3 Register*/
    volatile             T_SDMMC_sd_rsp_arg2                    sd_rsp_arg2; /*0x818, RO   , 0x00000000, SDMMC Response Argument2 Register*/
    volatile             T_SDMMC_sd_rsp_arg1                    sd_rsp_arg1; /*0x81c, RO   , 0x00000000, SDMMC Response Argument1 Register*/
    volatile             T_SDMMC_sd_rsp_arg0                    sd_rsp_arg0; /*0x820, RO   , 0x00000000, SDMMC Response Argument0 Register*/
    volatile           T_SDMMC_sd_data_width                  sd_data_width; /*0x824, RW   , 0x00000000, SDMMC Data Width Register     */
    volatile           T_SDMMC_sd_block_size                  sd_block_size; /*0x828, RW   , 0x00000000, SDMMC Block Size Register     */
    volatile            T_SDMMC_sd_block_cnt                   sd_block_cnt; /*0x82c, RW   , 0x00000000, SDMMC Block Count Register    */
    volatile           T_SDMMC_sd_int_status                  sd_int_status; /*0x830, RW   , 0x00000000, SDMMC Interrupt Status Register*/
    volatile             T_SDMMC_sd_int_mask                    sd_int_mask; /*0x834, RW   , 0x00000000, SDMMC Interrupt Mask Register */
    volatile            T_SDMMC_sd_int_clear                   sd_int_clear; /*0x838, WO   , 0x00000000, SDMMC Interrupt Clear Register*/
    volatile          T_SDMMC_sd_trans_speed                 sd_trans_speed; /*0x83c, RW   , 0x00000000, SDMMC Transfer Speed Register */
    volatile          T_SDMMC_sd_mclk_adjust                 sd_mclk_adjust; /*0x840, RW   , 0x00000100, SDMMC MCLK Adjust Register    */
} T_HWP_SDMMC_T;
#else
typedef struct
{
    volatile                             U32                        sd_ctrl; /*0x0  , RW   , 0x00000008, SDMMC Control Register        */
    volatile                             U32                     reserved_3;
    volatile                             U32                      sd_txfifo; /*0x8  , WO   , 0x00000000, SDMMC Tx FIFO Register        */
    volatile                             U32                      sd_rxfifo; /*0xc  , RO   , 0x00000000, SDMMC Rx FIFO Register        */
    volatile                             U32                reserved_4[508];
    volatile                             U32                      sd_config; /*0x800, RW   , 0x00010002, SDMMC Configuration Register  */
    volatile                             U32                      sd_status; /*0x804, RO   , 0x00000008, SDMMC Status Register         */
    volatile                             U32                   sd_cmd_index; /*0x808, RW   , 0x00000000, SDMMC Command Index Register  */
    volatile                             U32                     sd_cmd_arg; /*0x80c, RW   , 0x00000000, SDMMC Command Argument Register*/
    volatile                             U32                   sd_rsp_index; /*0x810, RO   , 0x00000000, SDMMC Response Index Register */
    volatile                             U32                    sd_rsp_arg3; /*0x814, RO   , 0x00000000, SDMMC Response Argument3 Register*/
    volatile                             U32                    sd_rsp_arg2; /*0x818, RO   , 0x00000000, SDMMC Response Argument2 Register*/
    volatile                             U32                    sd_rsp_arg1; /*0x81c, RO   , 0x00000000, SDMMC Response Argument1 Register*/
    volatile                             U32                    sd_rsp_arg0; /*0x820, RO   , 0x00000000, SDMMC Response Argument0 Register*/
    volatile                             U32                  sd_data_width; /*0x824, RW   , 0x00000000, SDMMC Data Width Register     */
    volatile                             U32                  sd_block_size; /*0x828, RW   , 0x00000000, SDMMC Block Size Register     */
    volatile                             U32                   sd_block_cnt; /*0x82c, RW   , 0x00000000, SDMMC Block Count Register    */
    volatile                             U32                  sd_int_status; /*0x830, RW   , 0x00000000, SDMMC Interrupt Status Register*/
    volatile                             U32                    sd_int_mask; /*0x834, RW   , 0x00000000, SDMMC Interrupt Mask Register */
    volatile                             U32                   sd_int_clear; /*0x838, WO   , 0x00000000, SDMMC Interrupt Clear Register*/
    volatile                             U32                 sd_trans_speed; /*0x83c, RW   , 0x00000000, SDMMC Transfer Speed Register */
    volatile                             U32                 sd_mclk_adjust; /*0x840, RW   , 0x00000100, SDMMC MCLK Adjust Register    */
} T_HWP_SDMMC_T;

//sd_ctrl
#define SDMMC_L_ENDIAN(n)              (((n)&0x7)<<0)      
#define SDMMC_L_ENDIAN_MASK            (0x7<<0)            
#define SDMMC_L_ENDIAN_SHIFT           (0)                 

#define SDMMC_SOFT_RST_L               (1<<3)              


//sd_txfifo
#define SDMMC_TXFIFO_WDATA(n)          (((n)&0xFFFFFFFF)<<0)
#define SDMMC_TXFIFO_WDATA_MASK        (0xFFFFFFFF<<0)     
#define SDMMC_TXFIFO_WDATA_SHIFT       (0)                 


//sd_rxfifo
#define SDMMC_RXFIFO_RDATA(n)          (((n)&0xFFFFFFFF)<<0)
#define SDMMC_RXFIFO_RDATA_MASK        (0xFFFFFFFF<<0)     
#define SDMMC_RXFIFO_RDATA_SHIFT       (0)                 


//sd_config
#define SDMMC_SDMMC_SENDCMD            (1<<0)              

#define SDMMC_SDMMC_SUSPEND            (1<<1)              

#define SDMMC_RSP_EN                   (1<<4)              

#define SDMMC_RSP_SEL(n)               (((n)&0x3)<<5)      
#define SDMMC_RSP_SEL_R2               (((2)&0x3)<<5)      
#define SDMMC_RSP_SEL_R3               (((1)&0x3)<<5)      
#define SDMMC_RSP_SEL_OTHER            (((0)&0x3)<<5)      
#define SDMMC_RSP_SEL_MASK             (0x3<<5)            
#define SDMMC_RSP_SEL_SHIFT            (5)                 

#define SDMMC_RD_WT_EN                 (1<<8)              

#define SDMMC_RD_WT_SEL                (1<<9)              
#define SDMMC_RD_WT_SEL_READ           (0<<9)              
#define SDMMC_RD_WT_SEL_WRITE          (1<<9)              

#define SDMMC_S_M_SEL                  (1<<10)             
#define SDMMC_S_M_SEL_SIMPLE           (0<<10)             
#define SDMMC_S_M_SEL_MULTIPLE         (1<<10)             

#define SDMMC_AUTO_FLAG_EN             (1<<16)             


//sd_status
#define SDMMC_NOT_SDMMC_OVER           (1<<0)              

#define SDMMC_BUSY                     (1<<1)              

#define SDMMC_DL_BUSY                  (1<<2)              

#define SDMMC_SUSPEND                  (1<<3)              

#define SDMMC_NOT_SDMMC_OVER_MASK      (1<<7)              

#define SDMMC_RSP_ERROR                (1<<8)              

#define SDMMC_NO_RSP_ERROR             (1<<9)              

#define SDMMC_CRC_STATUS(n)            (((n)&0x7)<<12)     
#define SDMMC_CRC_STATUS_MASK          (0x7<<12)           
#define SDMMC_CRC_STATUS_SHIFT         (12)                

#define SDMMC_DATA_ERROR(n)            (((n)&0xFF)<<16)    
#define SDMMC_DATA_ERROR_MASK          (0xFF<<16)          
#define SDMMC_DATA_ERROR_SHIFT         (16)                

#define SDMMC_DAT3_VAL                 (1<<24)             

#define SDMMC_DAT0_VAL                 (1<<25)             


//sd_cmd_index
#define SDMMC_COMMAND(n)               (((n)&0x3F)<<0)     
#define SDMMC_COMMAND_MASK             (0x3F<<0)           
#define SDMMC_COMMAND_SHIFT            (0)                 


//sd_cmd_arg
#define SDMMC_ARGUMENT(n)              (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT_MASK            (0xFFFFFFFF<<0)     
#define SDMMC_ARGUMENT_SHIFT           (0)                 


//sd_rsp_index
#define SDMMC_RESPONSE(n)              (((n)&0x3F)<<0)     
#define SDMMC_RESPONSE_MASK            (0x3F<<0)           
#define SDMMC_RESPONSE_SHIFT           (0)                 


//sd_rsp_arg3
#define SDMMC_ARGUMENT3(n)             (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT3_MASK           (0xFFFFFFFF<<0)     
#define SDMMC_ARGUMENT3_SHIFT          (0)                 


//sd_rsp_arg2
#define SDMMC_ARGUMENT2(n)             (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT2_MASK           (0xFFFFFFFF<<0)     
#define SDMMC_ARGUMENT2_SHIFT          (0)                 


//sd_rsp_arg1
#define SDMMC_ARGUMENT1(n)             (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT1_MASK           (0xFFFFFFFF<<0)     
#define SDMMC_ARGUMENT1_SHIFT          (0)                 


//sd_rsp_arg0
#define SDMMC_ARGUMENT0(n)             (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT0_MASK           (0xFFFFFFFF<<0)     
#define SDMMC_ARGUMENT0_SHIFT          (0)                 


//sd_data_width
#define SDMMC_SDMMC_DATA_WIDTH(n)      (((n)&0xF)<<0)      
#define SDMMC_SDMMC_DATA_WIDTH_MASK    (0xF<<0)            
#define SDMMC_SDMMC_DATA_WIDTH_SHIFT   (0)                 


//sd_block_size
#define SDMMC_SDMMC_BLOCK_SIZE(n)      (((n)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_1       (((2)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_2       (((3)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_4       (((4)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_8       (((5)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_16      (((6)&0xF)<<0)      
#define SDMMC_SDMMC_BLOCK_SIZE_512     (((11)&0xF)<<0)     
#define SDMMC_SDMMC_BLOCK_SIZE_MASK    (0xF<<0)            
#define SDMMC_SDMMC_BLOCK_SIZE_SHIFT   (0)                 


//sd_block_cnt
#define SDMMC_SDMMC_BLOCK_CNT(n)       (((n)&0xFFFF)<<0)   
#define SDMMC_SDMMC_BLOCK_CNT_MASK     (0xFFFF<<0)         
#define SDMMC_SDMMC_BLOCK_CNT_SHIFT    (0)                 


//sd_int_status
#define SDMMC_NO_RSP_INT               (1<<0)              

#define SDMMC_RSP_ERR_INT              (1<<1)              

#define SDMMC_RD_ERR_INT               (1<<2)              

#define SDMMC_WR_ERR_INT               (1<<3)              

#define SDMMC_DAT_OVER_INT             (1<<4)              

#define SDMMC_TXDMA_DONE_INT           (1<<5)              

#define SDMMC_RXDMA_DONE_INT           (1<<6)              

#define SDMMC_NO_RSP_SC                (1<<8)              

#define SDMMC_RSP_ERR_SC               (1<<9)              

#define SDMMC_RD_ERR_SC                (1<<10)             

#define SDMMC_WR_ERR_SC                (1<<11)             

#define SDMMC_DAT_OVER_SC              (1<<12)             

#define SDMMC_TXDMA_DONE_SC            (1<<13)             

#define SDMMC_RXDMA_DONE_SC            (1<<14)             


//sd_int_mask
#define SDMMC_NO_RSP_MK                (1<<0)              

#define SDMMC_RSP_ERR_MK               (1<<1)              

#define SDMMC_RD_ERR_MK                (1<<2)              

#define SDMMC_WR_ERR_MK                (1<<3)              

#define SDMMC_DAT_OVER_MK              (1<<4)              

#define SDMMC_TXDMA_DONE_MK            (1<<5)              

#define SDMMC_RXDMA_DONE_MK            (1<<6)              


//sd_int_clear
#define SDMMC_NO_RSP_CL                (1<<0)              

#define SDMMC_RSP_ERR_CL               (1<<1)              

#define SDMMC_RD_ERR_CL                (1<<2)              

#define SDMMC_WR_ERR_CL                (1<<3)              

#define SDMMC_DAT_OVER_CL              (1<<4)              

#define SDMMC_TXDMA_DONE_CL            (1<<5)              

#define SDMMC_RXDMA_DONE_CL            (1<<6)              


//sd_trans_speed
#define SDMMC_SDMMC_TRANS_SPEED(n)     (((n)&0xFF)<<0)     
#define SDMMC_SDMMC_TRANS_SPEED_MASK   (0xFF<<0)           
#define SDMMC_SDMMC_TRANS_SPEED_SHIFT  (0)                 


//sd_mclk_adjust
#define SDMMC_SDMMC_MCLK_ADJUST(n)     (((n)&0xF)<<0)      
#define SDMMC_SDMMC_MCLK_ADJUST_MASK   (0xF<<0)            
#define SDMMC_SDMMC_MCLK_ADJUST_SHIFT  (0)                 

#define SDMMC_CLK_INV                  (1<<4)              

#define SDMMC_MCLK_DIS                 (1<<8)              


#endif

#endif


