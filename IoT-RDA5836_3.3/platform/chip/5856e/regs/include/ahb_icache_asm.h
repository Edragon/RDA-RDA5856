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
// File name      : ahb_icache_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:29                                                  
// Last modified  : 2016-12-21 13:55:29                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:29 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB_ICACHE_ASM_H__
#define __AHB_ICACHE_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_r_rev_id                   REG_AHB_ICACHE_BASE+0x00
#define REG_cache_conf                 REG_AHB_ICACHE_BASE+0x04
#define REG_cache_flush                REG_AHB_ICACHE_BASE+0x08

#define REG_cache_hitcnt               REG_AHB_ICACHE_BASE+0x10
#define REG_cache_misscnt              REG_AHB_ICACHE_BASE+0x14
#define REG_cache_status               REG_AHB_ICACHE_BASE+0x18
#define REG_cache_irq                  REG_AHB_ICACHE_BASE+0x1C
#define REG_cache_irq_mask             REG_AHB_ICACHE_BASE+0x20

//r_rev_id
#define AHB_ICACHE_REV_ID(n)           (((n)&0xFF)<<0)     

//cache_conf
#define AHB_ICACHE_RSVD(n)             (((n)&0xF)<<0)      
#define AHB_ICACHE_CACHE_SIZE(n)       (((n)&0x3)<<4)      
#define AHB_ICACHE_CACHE_EN            (1<<6)              
#define AHB_ICACHE_IRQ_QUICK_RESP      (1<<7)              
#define AHB_ICACHE_FLASH_MAP(n)        (((n)&0x7)<<8)      
#define AHB_ICACHE_MISS_CNT_EN         (1<<12)             
#define AHB_ICACHE_HIT_CNT_EN          (1<<13)             

//cache_flush
#define AHB_ICACHE_CACHE_LINE_FLUSH    (1<<0)              

//cache_hitcnt
#define AHB_ICACHE_HIT_CNT(n)          (((n)&0xFFFFFFFF)<<0)

//cache_misscnt
#define AHB_ICACHE_MISS_CNT(n)         (((n)&0xFFFFFFFF)<<0)

//cache_status
#define AHB_ICACHE_ADDR_RANGE_ERROR    (1<<0)              
#define AHB_ICACHE_WRITE_ERROR         (1<<1)              
#define AHB_ICACHE_CACHE_DISABLE_ERROR (1<<2)              
#define AHB_ICACHE_FLUSH_DONE          (1<<3)              
#define AHB_ICACHE_DEV_READ_ERROR      (1<<4)              

//cache_irq
#define AHB_ICACHE_ADDR_RANGE_ERROR_IRQ (1<<0)              
#define AHB_ICACHE_WRITE_ERROR_IRQ     (1<<1)              
#define AHB_ICACHE_CACHE_DISABLE_ERROR_IRQ (1<<2)              
#define AHB_ICACHE_FLUSH_DONE_IRQ      (1<<3)              
#define AHB_ICACHE_DEV_READ_ERROR_IRQ  (1<<4)              

//cache_irq_mask
#define AHB_ICACHE_ADDR_RANGE_ERROR_MASK (1<<0)              
#define AHB_ICACHE_WRITE_ERROR_MASK    (1<<1)              
#define AHB_ICACHE_CACHE_DISABLE_ERROR_MASK (1<<2)              
#define AHB_ICACHE_FLUSH_DONE_MASK     (1<<3)              
#define AHB_ICACHE_DEV_READ_ERROR_MASK (1<<4)              


#endif


