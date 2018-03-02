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
// File name      : bt_core_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:33                                                  
// Last modified  : 2016-12-21 13:55:33                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:33 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __BT_CORE_ASM_H__
#define __BT_CORE_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_common_ctrl                REG_BT_CORE_BASE+0x00
#define REG_tport_ctrl                 REG_BT_CORE_BASE+0x04

#define REG_trx_on_force               REG_BT_CORE_BASE+0x14
#define REG_trx_on_timing              REG_BT_CORE_BASE+0x18

//common_ctrl
#define BT_CORE_RF_EXT_MODE            (1<<1)              

//tport_ctrl
#define BT_CORE_TPORT_DATA0_SEL(n)     (((n)&0xF)<<0)      
#define BT_CORE_TPORT_DATA1_SEL(n)     (((n)&0xF)<<8)      
#define BT_CORE_TPORT_TRIG_SEL(n)      (((n)&0xF)<<16)     
#define BT_CORE_TPORT_CLK_SEL(n)       (((n)&0xF)<<24)     

//trx_on_force
#define BT_CORE_RXON_VALUE             (1<<0)              
#define BT_CORE_RXON_FORCE             (1<<1)              
#define BT_CORE_TXON_VALUE             (1<<2)              
#define BT_CORE_TXON_FORCE             (1<<3)              
#define BT_CORE_RX_CAL_EN              (1<<4)              
#define BT_CORE_TX_CAL_EN              (1<<5)              

//trx_on_timing
#define BT_CORE_MODEM_RF_RXOFF_DELAY(n) (((n)&0xFF)<<0)     
#define BT_CORE_MODEM_RXON_DELAY(n)    (((n)&0xFF)<<8)     
#define BT_CORE_MODEM_RF_TXOFF_DELAY(n) (((n)&0xFF)<<16)    
#define BT_CORE_MODEM_TXON_DELAY(n)    (((n)&0xFF)<<24)    


#endif


