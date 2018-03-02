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

//
// Created by             :RDA SH-DIGITAL
// Filename               :iomux_asm.h
// Author                 :tianwq
// Created On             :2015-08-29 14:51:07
// Last Modified          :
// Update Count           :2015-08-29 14:51:07
// Description            :
//
//======================================================================
#ifndef __IOMUX_ASM_H__
#define __IOMUX_ASM_H__
//Auto-gen by reg_gen
#define REG_IOMUX_BASE 0x0

#define REG_IOMUX_BASE_HI BASE_HI(REG_IOMUX_BASE)
#define REG_IOMUX_BASE_LO BASE_LO(REG_IOMUX_BASE)

#define REG_P00_CTRL                   REG_IOMUX_BASE_LO+0x00
#define REG_P01_CTRL                   REG_IOMUX_BASE_LO+0x04
#define REG_P02_CTRL                   REG_IOMUX_BASE_LO+0x08
#define REG_P03_CTRL                   REG_IOMUX_BASE_LO+0x0C
#define REG_P04_CTRL                   REG_IOMUX_BASE_LO+0x10
#define REG_P05_CTRL                   REG_IOMUX_BASE_LO+0x14
#define REG_P06_CTRL                   REG_IOMUX_BASE_LO+0x18
#define REG_P07_CTRL                   REG_IOMUX_BASE_LO+0x1C
#define REG_P10_CTRL                   REG_IOMUX_BASE_LO+0x20
#define REG_P11_CTRL                   REG_IOMUX_BASE_LO+0x24
#define REG_P12_CTRL                   REG_IOMUX_BASE_LO+0x28
#define REG_P13_CTRL                   REG_IOMUX_BASE_LO+0x2C
#define REG_P14_CTRL                   REG_IOMUX_BASE_LO+0x30
#define REG_P15_CTRL                   REG_IOMUX_BASE_LO+0x34
#define REG_P16_CTRL                   REG_IOMUX_BASE_LO+0x38
#define REG_P17_CTRL                   REG_IOMUX_BASE_LO+0x3C
#define REG_P20_CTRL                   REG_IOMUX_BASE_LO+0x40
#define REG_P21_CTRL                   REG_IOMUX_BASE_LO+0x44
#define REG_P22_CTRL                   REG_IOMUX_BASE_LO+0x48
#define REG_P23_CTRL                   REG_IOMUX_BASE_LO+0x4C
#define REG_P24_CTRL                   REG_IOMUX_BASE_LO+0x50
#define REG_P25_CTRL                   REG_IOMUX_BASE_LO+0x54
#define REG_P26_CTRL                   REG_IOMUX_BASE_LO+0x58
#define REG_P27_CTRL                   REG_IOMUX_BASE_LO+0x5C
#define REG_P30_CTRL                   REG_IOMUX_BASE_LO+0x60
#define REG_P31_CTRL                   REG_IOMUX_BASE_LO+0x64
#define REG_P32_CTRL                   REG_IOMUX_BASE_LO+0x68
#define REG_P33_CTRL                   REG_IOMUX_BASE_LO+0x6C
#define REG_P34_CTRL                   REG_IOMUX_BASE_LO+0x70
#define REG_P35_CTRL                   REG_IOMUX_BASE_LO+0x74
#define REG_P36_CTRL                   REG_IOMUX_BASE_LO+0x78
#define REG_P37_CTRL                   REG_IOMUX_BASE_LO+0x7C
#define REG_P40_CTRL                   REG_IOMUX_BASE_LO+0x80
#define REG_P41_CTRL                   REG_IOMUX_BASE_LO+0x84
#define REG_P42_CTRL                   REG_IOMUX_BASE_LO+0x88
#define REG_P43_CTRL                   REG_IOMUX_BASE_LO+0x8C
#define REG_P44_CTRL                   REG_IOMUX_BASE_LO+0x90
#define REG_P45_CTRL                   REG_IOMUX_BASE_LO+0x94
#define REG_P46_CTRL                   REG_IOMUX_BASE_LO+0x98
#define REG_P47_CTRL                   REG_IOMUX_BASE_LO+0x9C

//P00_CTRL
#define IOMUX_P00_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_ALT0_CFG                 (1<<4)              
#define IOMUX_P00_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P00_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P00_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[0]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_0(n)         (((n)&0x3)<<18)     
#define IOMUX_P00_ENABLE_FORCE         (1<<31)             

//P01_CTRL
#define IOMUX_P01_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_ALT1_CFG                 (1<<4)              
#define IOMUX_P01_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P01_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P01_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[1]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_1(n)         (((n)&0x3)<<18)     
#define IOMUX_P01_ENABLE_FORCE         (1<<31)             

//P02_CTRL
#define IOMUX_P02_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P02_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P02_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P02_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[2]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_2(n)         (((n)&0x3)<<18)     
#define IOMUX_P02_ENABLE_FORCE         (1<<31)             

//P03_CTRL
#define IOMUX_P03_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P03_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P03_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P03_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[3]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_3(n)         (((n)&0x3)<<18)     
#define IOMUX_P03_ENABLE_FORCE         (1<<31)             

//P04_CTRL
#define IOMUX_P04_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_ALT4_CFG                 (1<<4)              
#define IOMUX_SEL4_CFG                 (1<<5)              
#define IOMUX_SEL5_CFG                 (1<<6)              
#define IOMUX_P04_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P04_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P04_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[4]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_4(n)         (((n)&0x3)<<18)     
#define IOMUX_P04_ENABLE_FORCE         (1<<31)             

//P05_CTRL
#define IOMUX_P05_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_ALT5_CFG                 (1<<4)              
#define IOMUX_SEL0_CFG                 (1<<5)              
#define IOMUX_SEL1_CFG                 (1<<6)              
#define IOMUX_P05_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P05_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P05_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[5]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_5(n)         (((n)&0x3)<<18)     
#define IOMUX_P05_ENABLE_FORCE         (1<<31)             

//P06_CTRL
#define IOMUX_P06_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P06_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P06_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P06_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[6]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_6(n)         (((n)&0x3)<<18)     
#define IOMUX_P06_ENABLE_FORCE         (1<<31)             

//P07_CTRL
#define IOMUX_P07_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P07_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P07_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P07_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P0_VIO2P8_SEL[7]    (1<<17)             
#define IOMUX_GPIO_P0_DRV_7(n)         (((n)&0x3)<<18)     
#define IOMUX_P07_ENABLE_FORCE         (1<<31)             

//P10_CTRL
#define IOMUX_P10_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P10_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P10_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P10_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[0]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_0(n)         (((n)&0x3)<<18)     
#define IOMUX_P10_ENABLE_FORCE         (1<<31)             

//P11_CTRL
#define IOMUX_P11_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P11_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P11_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P11_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[1]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_1(n)         (((n)&0x3)<<18)     
#define IOMUX_P11_ENABLE_FORCE         (1<<31)             

//P12_CTRL
#define IOMUX_P12_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P12_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P12_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P12_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[2]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_2(n)         (((n)&0x3)<<18)     
#define IOMUX_P12_ENABLE_FORCE         (1<<31)             

//P13_CTRL
#define IOMUX_P13_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P13_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P13_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P13_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[3]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_3(n)         (((n)&0x3)<<18)     
#define IOMUX_P13_ENABLE_FORCE         (1<<31)             

//P14_CTRL
#define IOMUX_P14_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P14_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P14_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P14_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[4]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_4(n)         (((n)&0x3)<<18)     
#define IOMUX_P14_ENABLE_FORCE         (1<<31)             

//P15_CTRL
#define IOMUX_P15_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P15_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P15_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P15_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[5]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_5(n)         (((n)&0x3)<<18)     
#define IOMUX_P15_ENABLE_FORCE         (1<<31)             

//P16_CTRL
#define IOMUX_P16_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P16_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P16_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P16_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[6]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_6(n)         (((n)&0x3)<<18)     
#define IOMUX_P16_ENABLE_FORCE         (1<<31)             

//P17_CTRL
#define IOMUX_P17_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P17_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P17_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P17_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P1_VIO2P8_SEL[7]    (1<<17)             
#define IOMUX_GPIO_P1_DRV_7(n)         (((n)&0x3)<<18)     
#define IOMUX_P17_ENABLE_FORCE         (1<<31)             

//P20_CTRL
#define IOMUX_P20_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_SEL2_CFG                 (1<<5)              
#define IOMUX_SEL3_CFG                 (1<<6)              
#define IOMUX_P20_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P20_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P20_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[0]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_0(n)         (((n)&0x3)<<18)     
#define IOMUX_P20_ENABLE_FORCE         (1<<31)             

//P21_CTRL
#define IOMUX_P21_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P21_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P21_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P21_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[1]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_1(n)         (((n)&0x3)<<18)     
#define IOMUX_P21_ENABLE_FORCE         (1<<31)             

//P22_CTRL
#define IOMUX_P22_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P22_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P22_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P22_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[2]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_2(n)         (((n)&0x3)<<18)     
#define IOMUX_P22_ENABLE_FORCE         (1<<31)             

//P23_CTRL
#define IOMUX_P23_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P23_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P23_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P23_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[3]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_3(n)         (((n)&0x3)<<18)     
#define IOMUX_P23_ENABLE_FORCE         (1<<31)             

//P24_CTRL
#define IOMUX_P24_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P24_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P24_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P24_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[4]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_4(n)         (((n)&0x3)<<18)     
#define IOMUX_P24_ENABLE_FORCE         (1<<31)             

//P25_CTRL
#define IOMUX_P25_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P25_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P25_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P25_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[5]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_5(n)         (((n)&0x3)<<18)     
#define IOMUX_P25_ENABLE_FORCE         (1<<31)             

//P26_CTRL
#define IOMUX_P26_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P26_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P26_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P26_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[6]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_6(n)         (((n)&0x3)<<18)     
#define IOMUX_P26_ENABLE_FORCE         (1<<31)             

//P27_CTRL
#define IOMUX_P27_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P27_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P27_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P27_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P2_VIO2P8_SEL[7]    (1<<17)             
#define IOMUX_GPIO_P2_DRV_7(n)         (((n)&0x3)<<18)     
#define IOMUX_P27_ENABLE_FORCE         (1<<31)             

//P30_CTRL
#define IOMUX_P30_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P30_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P30_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P30_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[0]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_0(n)         (((n)&0x3)<<18)     
#define IOMUX_P30_ENABLE_FORCE         (1<<31)             

//P31_CTRL
#define IOMUX_P31_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P31_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P31_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P31_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[1]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_1(n)         (((n)&0x3)<<18)     
#define IOMUX_P31_ENABLE_FORCE         (1<<31)             

//P32_CTRL
#define IOMUX_P32_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P32_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P32_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P32_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[2]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_2(n)         (((n)&0x3)<<18)     
#define IOMUX_P32_ENABLE_FORCE         (1<<31)             

//P33_CTRL
#define IOMUX_P33_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P33_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P33_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P33_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[3]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_3(n)         (((n)&0x3)<<18)     
#define IOMUX_P33_ENABLE_FORCE         (1<<31)             

//P34_CTRL
#define IOMUX_P34_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P34_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P34_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P34_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[4]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_4(n)         (((n)&0x3)<<18)     
#define IOMUX_P34_ENABLE_FORCE         (1<<31)             

//P35_CTRL
#define IOMUX_P35_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P35_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P35_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P35_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[5]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_5(n)         (((n)&0x3)<<18)     
#define IOMUX_P35_ENABLE_FORCE         (1<<31)             

//P36_CTRL
#define IOMUX_P36_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P36_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P36_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P36_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[6]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_6(n)         (((n)&0x3)<<18)     
#define IOMUX_P36_ENABLE_FORCE         (1<<31)             

//P37_CTRL
#define IOMUX_P37_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P37_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P37_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P37_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P3_VIO2P8_SEL[7]    (1<<17)             
#define IOMUX_GPIO_P3_DRV_7(n)         (((n)&0x3)<<18)     
#define IOMUX_P37_ENABLE_FORCE         (1<<31)             

//P40_CTRL
#define IOMUX_P40_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P40_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P40_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P40_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[0]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_0(n)         (((n)&0x3)<<18)     
#define IOMUX_P40_ENABLE_FORCE         (1<<31)             

//P41_CTRL
#define IOMUX_P41_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P41_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P41_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P41_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[1]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_1(n)         (((n)&0x3)<<18)     
#define IOMUX_P41_ENABLE_FORCE         (1<<31)             

//P42_CTRL
#define IOMUX_P42_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P42_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P42_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P42_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[2]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_2(n)         (((n)&0x3)<<18)     
#define IOMUX_P42_ENABLE_FORCE         (1<<31)             

//P43_CTRL
#define IOMUX_P43_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P43_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P43_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P43_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[3]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_3(n)         (((n)&0x3)<<18)     
#define IOMUX_P43_ENABLE_FORCE         (1<<31)             

//P44_CTRL
#define IOMUX_P44_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P44_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P44_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P44_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[4]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_4(n)         (((n)&0x3)<<18)     
#define IOMUX_P44_ENABLE_FORCE         (1<<31)             

//P45_CTRL
#define IOMUX_P45_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P45_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P45_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P45_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[5]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_5(n)         (((n)&0x3)<<18)     
#define IOMUX_P45_ENABLE_FORCE         (1<<31)             

//P46_CTRL
#define IOMUX_P46_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P46_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P46_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P46_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[6]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_6(n)         (((n)&0x3)<<18)     
#define IOMUX_P46_ENABLE_FORCE         (1<<31)             

//P47_CTRL
#define IOMUX_P47_CFG(n)               (((n)&0xF)<<0)      
#define IOMUX_P47_DN_ACTIVE_FORCE      (1<<8)              
#define IOMUX_P47_UP_ACTIVE_FORCE      (1<<9)              
#define IOMUX_P47_DIR_FORCE            (1<<16)             
#define IOMUX_GPIO_P4_VIO2P8_SEL[7]    (1<<17)             
#define IOMUX_GPIO_P4_DRV_7(n)         (((n)&0x3)<<18)     
#define IOMUX_P47_ENABLE_FORCE         (1<<31)             


#endif


