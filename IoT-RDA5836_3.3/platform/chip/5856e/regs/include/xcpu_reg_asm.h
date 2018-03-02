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
// File name      : xcpu_reg_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:56:05                                                  
// Last modified  : 2016-12-21 13:56:05                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:56:05 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __XCPU_REG_ASM_H__
#define __XCPU_REG_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_if0_addr                   REG_XCPU_REG_BASE+0x00
#define REG_rf0_addr                   REG_XCPU_REG_BASE+0x04
#define REG_ex0_addr                   REG_XCPU_REG_BASE+0x08
#define REG_dc0_addr                   REG_XCPU_REG_BASE+0x0C

#define REG_ap_cp0cntrl11              REG_XCPU_REG_BASE+0x100
#define REG_ap_cp0cntr12               REG_XCPU_REG_BASE+0x104

#define REG_cp0_adr_BadVAddr           REG_XCPU_REG_BASE+0x18C
#define REG_cp0_cause                  REG_XCPU_REG_BASE+0x190
#define REG_cp0_Status                 REG_XCPU_REG_BASE+0x194
#define REG_cp0_EPC                    REG_XCPU_REG_BASE+0x198

#define REG_ap_debug_multcntrl         REG_XCPU_REG_BASE+0x280

#define REG_regfile_0                  REG_XCPU_REG_BASE+0x380
#define REG_regfile_1                  REG_XCPU_REG_BASE+0x384
#define REG_regfile_2                  REG_XCPU_REG_BASE+0x388
#define REG_regfile_3                  REG_XCPU_REG_BASE+0x38c
#define REG_regfile_4                  REG_XCPU_REG_BASE+0x390
#define REG_regfile_5                  REG_XCPU_REG_BASE+0x394
#define REG_regfile_6                  REG_XCPU_REG_BASE+0x398
#define REG_regfile_7                  REG_XCPU_REG_BASE+0x39c
#define REG_regfile_8                  REG_XCPU_REG_BASE+0x3a0
#define REG_regfile_9                  REG_XCPU_REG_BASE+0x3a4
#define REG_regfile_10                 REG_XCPU_REG_BASE+0x3a8
#define REG_regfile_11                 REG_XCPU_REG_BASE+0x3ac
#define REG_regfile_12                 REG_XCPU_REG_BASE+0x3b0
#define REG_regfile_13                 REG_XCPU_REG_BASE+0x3b4
#define REG_regfile_14                 REG_XCPU_REG_BASE+0x3b8
#define REG_regfile_15                 REG_XCPU_REG_BASE+0x3bc
#define REG_regfile_16                 REG_XCPU_REG_BASE+0x3c0
#define REG_regfile_17                 REG_XCPU_REG_BASE+0x3c4
#define REG_regfile_18                 REG_XCPU_REG_BASE+0x3c8
#define REG_regfile_19                 REG_XCPU_REG_BASE+0x3cc
#define REG_regfile_20                 REG_XCPU_REG_BASE+0x3d0
#define REG_regfile_21                 REG_XCPU_REG_BASE+0x3d4
#define REG_regfile_22                 REG_XCPU_REG_BASE+0x3d8
#define REG_regfile_23                 REG_XCPU_REG_BASE+0x3dc
#define REG_regfile_24                 REG_XCPU_REG_BASE+0x3e0
#define REG_regfile_25                 REG_XCPU_REG_BASE+0x3e4
#define REG_regfile_26                 REG_XCPU_REG_BASE+0x3e8
#define REG_regfile_27                 REG_XCPU_REG_BASE+0x3ec
#define REG_regfile_28                 REG_XCPU_REG_BASE+0x3f0
#define REG_regfile_29                 REG_XCPU_REG_BASE+0x3f4
#define REG_regfile_30                 REG_XCPU_REG_BASE+0x3f8
#define REG_regfile_31                 REG_XCPU_REG_BASE+0x3fc

//if0_addr
#define XCPU_REG_IF0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)

//rf0_addr
#define XCPU_REG_RF0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)

//ex0_addr
#define XCPU_REG_EX0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)

//dc0_addr
#define XCPU_REG_DC0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)

//ap_cp0cntrl11
#define XCPU_REG_CP0_MT_PRID           (1<<0)              
#define XCPU_REG_CP0_MT_EPC            (1<<1)              
#define XCPU_REG_CP0_MT_CAUSE          (1<<2)              
#define XCPU_REG_CP0_MT_STATUTS        (1<<3)              
#define XCPU_REG_CP0_MT_BADVADDR       (1<<5)              

//ap_cp0cntr12
#define XCPU_REG_CP0_UNUSABLE_EXCEPT   (1<<0)              
#define XCPU_REG_DC0_CP3_INSN          (1<<7)              
#define XCPU_REG_DC0_CP2_INSN          (1<<8)              
#define XCPU_REG_DC0_CP1_INSN          (1<<9)              
#define XCPU_REG_CP0_EXCEPT            (1<<10)             
#define XCPU_REG_CP0_VALID             (1<<11)             
#define XCPU_REG_CP0_RFE               (1<<16)             
#define XCPU_REG_CP0_MF_CP0            (1<<17)             
#define XCPU_REG_CP0_CP_SEL(n)         (((n)&0xF)<<18)     

//cp0_adr_BadVAddr
#define XCPU_REG_CP0_ADR_BADVADDR(n)   (((n)&0xFFFFFFFF)<<0)

//cp0_cause
#define XCPU_REG_CP0_CAUSE_EXCCODE(n)  (((n)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_IP_SOFT(n)  (((n)&0x3)<<8)      
#define XCPU_REG_CP0_CAUSE_IP_EXT(n)   (((n)&0x3F)<<10)    
#define XCPU_REG_CP0_CAUSE_CE(n)       (((n)&0x3)<<28)     
#define XCPU_REG_CP0_CAUSE_BD          (1<<31)             

//cp0_Status
#define XCPU_REG_CP0_STATUS_IEC        (1<<0)              
#define XCPU_REG_CP0_STATUS_KUC        (1<<1)              
#define XCPU_REG_CP0_STATUS_IEP        (1<<2)              
#define XCPU_REG_CP0_STATUS_KUP        (1<<3)              
#define XCPU_REG_CP0_STATUS_IEO        (1<<4)              
#define XCPU_REG_CP0_STATUS_KUO        (1<<5)              
#define XCPU_REG_CP0_STATUS_KUI        (1<<6)              
#define XCPU_REG_CP0_STATUS_INTMASK(n) (((n)&0xFF)<<8)     
#define XCPU_REG_CP0_STATUS_SD         (1<<16)             
#define XCPU_REG_CP0_STATUS_CI         (1<<17)             
#define XCPU_REG_CP0_STATUS_CM         (1<<19)             
#define XCPU_REG_CP0_STATUS_TS         (1<<21)             
#define XCPU_REG_CP0_STATUS_BEV        (1<<22)             
#define XCPU_REG_CP0_STATUS_RE         (1<<25)             
#define XCPU_REG_CP0_STATUS_CU(n)      (((n)&0xF)<<28)     

//cp0_EPC
#define XCPU_REG_CP0_EPC(n)            (((n)&0xFFFFFFFF)<<0)

//ap_debug_multcntrl
#define XCPU_REG_AP_DEBUG_MULTCNTRL(n) (((n)&0x7FFF)<<0)   

//regfile
#define XCPU_REG_REGFILE(n)            (((n)&0xFFFFFFFF)<<0)


#endif


