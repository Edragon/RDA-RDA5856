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
// Filename               :iomux.h
// Author                 :tianwq
// Created On             :2015-08-29 14:51:07
// Last Modified          :
// Update Count           :2015-08-29 14:51:07
// Description            :
//
//======================================================================
#ifndef __IOMUX_H__
#define __IOMUX_H__

//Auto-gen by reg_gen
#include "cs_types.h"

#define REG_IOMUX_BASE 0x01a05000

typedef volatile struct
{
    REG32 IOMUX_PXX_CTRL[5][8];      

} HWP_IOMUX_T;

#define hwp_iomux ((HWP_IOMUX_T*)KSEG1(REG_IOMUX_BASE))

#define IOMUX_PXX_CFG(n)                        (((n)&0xF)<<0)      
#define IOMUX_PXX_ALT0_CFG                      (1<<4)              
#define IOMUX_PXX_DN_ACTIVE_FORCE               (1<<8)              
#define IOMUX_PXX_UP_ACTIVE_FORCE               (1<<9)              
#define IOMUX_PXX_DIR_FORCE                     (1<<16)
#define IOMUX_PXX_VIO2P8_SEL(n)                 ((n)<<17)   
#define IOMUX_PXX_DRV(n)                        (((n)&0x3)<<18)  
#define IOMUX_PXX_ENABLE_FORCE                  (1<<31)  

#endif


