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
// File name      : dma_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:48                                                  
// Last modified  : 2016-08-29 14:59:48                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:48 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DMA_ASM_H__
#define __DMA_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_get_channel                REG_DMA_BASE+0x00   
#define REG_status                     REG_DMA_BASE+0x04   
#define REG_control                    REG_DMA_BASE+0x08   
#define REG_src_addr                   REG_DMA_BASE+0x0C   
#define REG_dst_addr                   REG_DMA_BASE+0x10   
#define REG_sd_dst_addr                REG_DMA_BASE+0x14   
#define REG_xfer_size                  REG_DMA_BASE+0x18   
#define REG_pattern                    REG_DMA_BASE+0x1C   

#define REG_fcs                        REG_DMA_BASE+0x2C   

//get_channel
#define DMA_GET_CHANNEL                (1<<0)              

//status
#define DMA_ENABLE                     (1<<0)              
#define DMA_INT_DONE_CAUSE             (1<<1)              
#define DMA_INT_DONE_STATUS            (1<<2)              
#define DMA_CHANNEL_LOCK               (1<<4)              

//control
#define DMA_CONTROL_ENABLE             (1<<0)              
#define DMA_INT_DONE_MASK              (1<<1)              
#define DMA_INT_DONE_CLEAR             (1<<2)              
#define DMA_USE_PATTERN                (1<<4)              
#define DMA_STOP_TRANSFER              (1<<8)              
#define DMA_FCS_ENABLE                 (1<<16)             
#define DMA_DST_ADDR_MGT(n)            (((n)&0x3)<<20)     

//src_addr
#define DMA_SRC_ADDRESS(n)             (((n)&0xFFFFFFFF)<<0)

//dst_addr
#define DMA_DST_ADDRESS(n)             (((n)&0xFFFFFFFF)<<0)

//sd_dst_addr
#define DMA_SD_DST_ADDRESS(n)          (((n)&0xFFFFFFFF)<<0)

//xfer_size
#define DMA_TRANSFER_SIZE(n)           (((n)&0x3FFFF)<<0)  

//pattern
#define DMA_PATTERN(n)                 (((n)&0xFFFFFFFF)<<0)

//fcs
#define DMA_FCS(n)                     (((n)&0xFFFFFF)<<0) 
#define DMA_FCS_CORRECT                (1<<31)             


#endif


