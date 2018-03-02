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
// File name      : ahb_mailbox_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:30                                                  
// Last modified  : 2016-12-21 13:55:30                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:30 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB_MAILBOX_ASM_H__
#define __AHB_MAILBOX_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_zsp2mcu_irq_trig           REG_AHB_MAILBOX_BASE+0x00
#define REG_zsp2mcu_isr                REG_AHB_MAILBOX_BASE+0x04
#define REG_zsp2mcu_imr                REG_AHB_MAILBOX_BASE+0x08
#define REG_zsp2mcu_irsr               REG_AHB_MAILBOX_BASE+0x0C
#define REG_zsp2mcu_icr                REG_AHB_MAILBOX_BASE+0x10
#define REG_mcu2zsp_irq_trig           REG_AHB_MAILBOX_BASE+0x14
#define REG_mcu2zsp_isr                REG_AHB_MAILBOX_BASE+0x18
#define REG_mcu2zsp_imr                REG_AHB_MAILBOX_BASE+0x1C
#define REG_mcu2zsp_irsr               REG_AHB_MAILBOX_BASE+0x20
#define REG_mcu2zsp_icr                REG_AHB_MAILBOX_BASE+0x24

//zsp2mcu_irq_trig
#define AHB_MAILBOX_ZSP2MCU_IRQ_TRIG(n) (((n)&0xFFFFFFFF)<<0)

//zsp2mcu_isr
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS(n) (((n)&0xFFFFFFFF)<<0)

//zsp2mcu_imr
#define AHB_MAILBOX_ZSP2MCU_IRQ_MASK(n) (((n)&0xFFFFFFFF)<<0)

//zsp2mcu_irsr
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_RAW(n) (((n)&0xFFFFFFFF)<<0)

//zsp2mcu_icr
#define AHB_MAILBOX_ZSP2MCU_IRQ_CLEAR(n) (((n)&0xFFFFFFFF)<<0)

//mcu2zsp_irq_trig
#define AHB_MAILBOX_MCU2ZSP_IRQ_TRIG(n) (((n)&0xFFFFFFFF)<<0)

//mcu2zsp_isr
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS(n) (((n)&0xFFFFFFFF)<<0)

//mcu2zsp_imr
#define AHB_MAILBOX_MCU2ZSP_IRQ_MASK(n) (((n)&0xFFFFFFFF)<<0)

//mcu2zsp_irsr
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_RAW(n) (((n)&0xFFFFFFFF)<<0)

//mcu2zsp_icr
#define AHB_MAILBOX_MCU2ZSP_IRQ_CLEAR(n) (((n)&0xFFFFFFFF)<<0)


#endif


