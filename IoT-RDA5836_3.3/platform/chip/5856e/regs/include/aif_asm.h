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


#ifndef __AIF_ASM_H__
#define __AIF_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_data                       REG_AIF_BASE+0x00   
#define REG_ctrl                       REG_AIF_BASE+0x04   
#define REG_serial_ctrl                REG_AIF_BASE+0x08   
#define REG_tone                       REG_AIF_BASE+0x0c   
#define REG_side_tone                  REG_AIF_BASE+0x10   

//data
#define AIF_DATA0(n)                   (((n)&0xFFFF)<<0)   
#define AIF_DATA1(n)                   (((n)&0xFFFF)<<16)  

//ctrl
#define AIF_ENABLE                     (1<<0)              
#define AIF_TX_OFF                     (1<<4)              
#define AIF_PARALLEL_OUT_SET           (1<<8)              
#define AIF_PARALLEL_OUT_CLR           (1<<9)              
#define AIF_PARALLEL_IN_SET            (1<<10)             
#define AIF_PARALLEL_IN_CLR            (1<<11)             
#define AIF_TX_STB_MODE                (1<<12)             
#define AIF_OUT_UNDERFLOW              (1<<16)             
#define AIF_IN_OVERFLOW                (1<<17)             
#define AIF_LOOP_BACK                  (1<<31)             

//serial_ctrl
#define AIF_SERIAL_MODE(n)             (((n)&0x3)<<0)      
#define AIF_I2S_IN_SEL(n)              (((n)&0x3)<<2)      
#define AIF_MASTER_MODE                (1<<4)              
#define AIF_LSB                        (1<<5)              
#define AIF_LRCK_POL                   (1<<6)              
#define AIF_RX_DLY(n)                  (((n)&0x3)<<8)      
#define AIF_TX_DLY                     (1<<10)             
#define AIF_TX_DLY_S                   (1<<11)             
#define AIF_TX_MODE(n)                 (((n)&0x3)<<12)     
#define AIF_RX_MODE                    (1<<14)             
#define AIF_BCK_LRCK(n)                (((n)&0x1F)<<16)    
#define AIF_BCK_POL                    (1<<24)             
#define AIF_OUTPUT_HALF_CYCLE_DLY      (1<<25)             
#define AIF_INPUT_HALF_CYCLE_DLY       (1<<26)             
#define AIF_BCKOUT_GATE                (1<<28)             

//tone
#define AIF_ENABLE_H                   (1<<0)              
#define AIF_TONE_SELECT                (1<<1)              
#define AIF_DTMF_FREQ_COL(n)           (((n)&0x3)<<4)      
#define AIF_DTMF_FREQ_ROW(n)           (((n)&0x3)<<6)      
#define AIF_COMFORT_FREQ(n)            (((n)&0x3)<<8)      
#define AIF_TONE_GAIN(n)               (((n)&0x3)<<12)     

//side_tone
#define AIF_SIDE_TONE_GAIN(n)          (((n)&0xF)<<0)      


#endif


