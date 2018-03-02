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

#ifndef _DMA_ASM_H_
#define _DMA_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'dma'."
#endif



//==============================================================================
// dma
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_DMA_BASE                0x01A08000

#define REG_DMA_BASE_HI             BASE_HI(REG_DMA_BASE)
#define REG_DMA_BASE_LO             BASE_LO(REG_DMA_BASE)

#define REG_DMA_GET_CHANNEL         REG_DMA_BASE_LO + 0x00000000
#define REG_DMA_STATUS              REG_DMA_BASE_LO + 0x00000004
#define REG_DMA_CONTROL             REG_DMA_BASE_LO + 0x00000008
#define REG_DMA_SRC_ADDR            REG_DMA_BASE_LO + 0x0000000C
#define REG_DMA_DST_ADDR            REG_DMA_BASE_LO + 0x00000010
#define REG_DMA_SD_DST_ADDR         REG_DMA_BASE_LO + 0x00000014
#define REG_DMA_XFER_SIZE           REG_DMA_BASE_LO + 0x00000018
#define REG_DMA_PATTERN             REG_DMA_BASE_LO + 0x0000001C
#define REG_DMA_GEA_KC_LOW          REG_DMA_BASE_LO + 0x00000020
#define REG_DMA_GEA_KC_HIGH         REG_DMA_BASE_LO + 0x00000024
#define REG_DMA_GEA_MESSKEY         REG_DMA_BASE_LO + 0x00000028
#define REG_DMA_FCS                 REG_DMA_BASE_LO + 0x0000002C

//get_channel
#define DMA_GET_CHANNEL             (1<<0)

//status
#define DMA_ENABLE                  (1<<0)
#define DMA_INT_DONE_CAUSE          (1<<1)
#define DMA_INT_DONE_STATUS         (1<<2)
#define DMA_CHANNEL_LOCK            (1<<4)

//control
//#define DMA_ENABLE                (1<<0)
#define DMA_INT_DONE_MASK           (1<<1)
#define DMA_INT_DONE_MASK_SHIFT     (1)
#define DMA_INT_DONE_CLEAR          (1<<2)
#define DMA_USE_PATTERN             (1<<4)
#define DMA_STOP_TRANSFER           (1<<8)
#define DMA_GEA_ENABLE              (1<<12)
#define DMA_GEA_ENABLE_DMA          (0<<12)
#define DMA_GEA_ENABLE_GEA          (1<<12)
#define DMA_GEA_ALGORITHM           (1<<13)
#define DMA_GEA_ALGORITHM_GEA1      (0<<13)
#define DMA_GEA_ALGORITHM_GEA2      (1<<13)
#define DMA_GEA_DIRECTION           (1<<14)
#define DMA_GEA_DIRECTION_SHIFT     (14)
#define DMA_FCS_ENABLE              (1<<16)
#define DMA_FCS_ENABLE_NORMAL_DMA   (0<<16)
#define DMA_FCS_ENABLE_FCS_PROCESS  (1<<16)
#define DMA_DST_ADDR_MGT(n)         (((n)&3)<<20)
#define DMA_DST_ADDR_MGT_NORMAL_DMA (0<<20)
#define DMA_DST_ADDR_MGT_CONST_ADDR (1<<20)
#define DMA_DST_ADDR_MGT_ALTERN_ADDR (2<<20)
#define DMA_DST_ADDR_MGT_RESERVED   (3<<20)

//src_addr
#define DMA_SRC_ADDRESS(n)          (((n)&0xFFFFFFFF)<<0)

//dst_addr
#define DMA_DST_ADDRESS(n)          (((n)&0xFFFFFFFF)<<0)

//sd_dst_addr
#define DMA_SD_DST_ADDRESS(n)       (((n)&0xFFFFFFFF)<<0)

//xfer_size
#define DMA_TRANSFER_SIZE(n)        (((n)&0x3FFFF)<<0)

//pattern
#define DMA_PATTERN(n)              (((n)&0xFFFFFFFF)<<0)

//gea_kc_low
#define DMA_KC_LSB(n)               (((n)&0xFFFFFFFF)<<0)

//gea_kc_high
#define DMA_KC_MSB(n)               (((n)&0xFFFFFFFF)<<0)

//gea_messkey
#define DMA_MESSKEY(n)              (((n)&0xFFFFFFFF)<<0)

//fcs
#define DMA_FCS(n)                  (((n)&0xFFFFFF)<<0)
#define DMA_FCS_CORRECT             (1<<31)




#endif
