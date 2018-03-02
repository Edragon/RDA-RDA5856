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
// File name      : i2c_master_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:50                                                  
// Last modified  : 2016-08-29 14:59:50                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:50 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __I2C_MASTER_ASM_H__
#define __I2C_MASTER_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ctrl                       REG_I2C_MASTER_BASE+0x00
#define REG_status                     REG_I2C_MASTER_BASE+0x04
#define REG_txrx_buffer                REG_I2C_MASTER_BASE+0x08
#define REG_cmd                        REG_I2C_MASTER_BASE+0x0C
#define REG_irq_clr                    REG_I2C_MASTER_BASE+0x10

//ctrl
#define I2C_MASTER_EN                  (1<<0)              
#define I2C_MASTER_IRQ_MASK            (1<<8)              
#define I2C_MASTER_CLOCK_PRESCALE(n)   (((n)&0xFFFF)<<16)  

//status
#define I2C_MASTER_IRQ_CAUSE           (1<<0)              
#define I2C_MASTER_IRQ_STATUS          (1<<4)              
#define I2C_MASTER_TIP                 (1<<8)              
#define I2C_MASTER_AL                  (1<<12)             
#define I2C_MASTER_BUSY                (1<<16)             
#define I2C_MASTER_RXACK               (1<<20)             

//txrx_buffer
#define I2C_MASTER_TX_DATA_0           (1<<0)              
#define I2C_MASTER_TX_DATA_7_1(n)      (((n)&0x7F)<<1)     

//cmd
#define I2C_MASTER_ACK                 (1<<0)              
#define I2C_MASTER_RD                  (1<<4)              
#define I2C_MASTER_STO                 (1<<8)              
#define I2C_MASTER_WR                  (1<<12)             
#define I2C_MASTER_STA                 (1<<16)             

//irq_clr
#define I2C_MASTER_IRQ_CLR             (1<<0)              


#endif


