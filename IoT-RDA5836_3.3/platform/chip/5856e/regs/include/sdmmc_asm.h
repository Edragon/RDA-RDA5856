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
// File name      : sdmmc_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:54                                                  
// Last modified  : 2016-08-29 14:59:54                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:54 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SDMMC_ASM_H__
#define __SDMMC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_sd_ctrl                    REG_SDMMC_BASE+0x000

#define REG_sd_txfifo                  REG_SDMMC_BASE+0x008
#define REG_sd_rxfifo                  REG_SDMMC_BASE+0x00C

#define REG_sd_config                  REG_SDMMC_BASE+0x800
#define REG_sd_status                  REG_SDMMC_BASE+0x804
#define REG_sd_cmd_index               REG_SDMMC_BASE+0x808
#define REG_sd_cmd_arg                 REG_SDMMC_BASE+0x80C
#define REG_sd_rsp_index               REG_SDMMC_BASE+0x810
#define REG_sd_rsp_arg3                REG_SDMMC_BASE+0x814
#define REG_sd_rsp_arg2                REG_SDMMC_BASE+0x818
#define REG_sd_rsp_arg1                REG_SDMMC_BASE+0x81C
#define REG_sd_rsp_arg0                REG_SDMMC_BASE+0x820
#define REG_sd_data_width              REG_SDMMC_BASE+0x824
#define REG_sd_block_size              REG_SDMMC_BASE+0x828
#define REG_sd_block_cnt               REG_SDMMC_BASE+0x82c
#define REG_sd_int_status              REG_SDMMC_BASE+0x830
#define REG_sd_int_mask                REG_SDMMC_BASE+0x834
#define REG_sd_int_clear               REG_SDMMC_BASE+0x838
#define REG_sd_trans_speed             REG_SDMMC_BASE+0x83c
#define REG_sd_mclk_adjust             REG_SDMMC_BASE+0x840

//sd_ctrl
#define SDMMC_L_ENDIAN(n)              (((n)&0x7)<<0)      
#define SDMMC_SOFT_RST_L               (1<<3)              

//sd_txfifo
#define SDMMC_TXFIFO_WDATA(n)          (((n)&0xFFFFFFFF)<<0)

//sd_rxfifo
#define SDMMC_RXFIFO_RDATA(n)          (((n)&0xFFFFFFFF)<<0)

//sd_config
#define SDMMC_SDMMC_SENDCMD            (1<<0)              
#define SDMMC_SDMMC_SUSPEND            (1<<1)              
#define SDMMC_RSP_EN                   (1<<4)              
#define SDMMC_RSP_SEL(n)               (((n)&0x3)<<5)      
#define SDMMC_RD_WT_EN                 (1<<8)              
#define SDMMC_RD_WT_SEL                (1<<9)              
#define SDMMC_S_M_SEL                  (1<<10)             
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
#define SDMMC_DATA_ERROR(n)            (((n)&0xFF)<<16)    
#define SDMMC_DAT3_VAL                 (1<<24)             
#define SDMMC_DAT0_VAL                 (1<<25)             

//sd_cmd_index
#define SDMMC_COMMAND(n)               (((n)&0x3F)<<0)     

//sd_cmd_arg
#define SDMMC_ARGUMENT(n)              (((n)&0xFFFFFFFF)<<0)

//sd_rsp_index
#define SDMMC_RESPONSE(n)              (((n)&0x3F)<<0)     

//sd_rsp_arg3
#define SDMMC_ARGUMENT3(n)             (((n)&0xFFFFFFFF)<<0)

//sd_rsp_arg2
#define SDMMC_ARGUMENT2(n)             (((n)&0xFFFFFFFF)<<0)

//sd_rsp_arg1
#define SDMMC_ARGUMENT1(n)             (((n)&0xFFFFFFFF)<<0)

//sd_rsp_arg0
#define SDMMC_ARGUMENT0(n)             (((n)&0xFFFFFFFF)<<0)

//sd_data_width
#define SDMMC_SDMMC_DATA_WIDTH(n)      (((n)&0xF)<<0)      

//sd_block_size
#define SDMMC_SDMMC_BLOCK_SIZE(n)      (((n)&0xF)<<0)      

//sd_block_cnt
#define SDMMC_SDMMC_BLOCK_CNT(n)       (((n)&0xFFFF)<<0)   

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

//sd_mclk_adjust
#define SDMMC_SDMMC_MCLK_ADJUST(n)     (((n)&0xF)<<0)      
#define SDMMC_CLK_INV                  (1<<4)              
#define SDMMC_MCLK_DIS                 (1<<8)              


#endif


