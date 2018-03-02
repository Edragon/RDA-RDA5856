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
// File name      : debug_host_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:47                                                  
// Last modified  : 2016-08-29 14:59:47                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:47 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DEBUG_HOST_ASM_H__
#define __DEBUG_HOST_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_cmd                        REG_DEBUG_HOST_BASE+0x00
#define REG_data                       REG_DEBUG_HOST_BASE+0x04
#define REG_event                      REG_DEBUG_HOST_BASE+0x08
#define REG_mode                       REG_DEBUG_HOST_BASE+0x0C
#define REG_h2p_status                 REG_DEBUG_HOST_BASE+0x10
#define REG_p2h_status                 REG_DEBUG_HOST_BASE+0x14
#define REG_irq                        REG_DEBUG_HOST_BASE+0x18

//cmd
#define DEBUG_HOST_ADDR(n)             (((n)&0x1FFFFFFF)<<0)
#define DEBUG_HOST_SIZE(n)             (((n)&0x3)<<29)     
#define DEBUG_HOST_WRITE_H             (1<<31)             

//data
#define DEBUG_HOST_DATA(n)             (((n)&0xFFFFFFFF)<<0)

//event
#define DEBUG_HOST_EVENT0_SEMA         (1<<0)              
#define DEBUG_HOST_EVENT31_1(n)        (((n)&0x7FFFFFFF)<<1)

//mode
#define DEBUG_HOST_FORCE_ON            (1<<0)              
#define DEBUG_HOST_CLK_HOST_ON         (1<<1)              

//h2p_status
#define DEBUG_HOST_H2P_STATUS(n)       (((n)&0xFF)<<0)     
#define DEBUG_HOST_H2P_STATUS_RST      (1<<16)             

//p2h_status
#define DEBUG_HOST_P2H_STATUS(n)       (((n)&0xFF)<<0)     

//irq
#define DEBUG_HOST_XCPU_IRQ            (1<<0)              
#define DEBUG_HOST_BCPU_IRQ            (1<<1)              


#endif


