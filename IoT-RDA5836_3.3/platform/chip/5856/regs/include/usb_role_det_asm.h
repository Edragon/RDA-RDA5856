/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
//
// Created by             :RDA SH-DIGITAL
// Filename               :usb_role_det_asm.h
// Author                 :tianwq
// Created On             :2015-08-29 14:51:41
// Last Modified          :
// Update Count           :2015-08-29 14:51:41
// Description            :
//
//======================================================================
#ifndef __USB_ROLE_DET_ASM_H__
#define __USB_ROLE_DET_ASM_H__
//Auto-gen by reg_gen
#define REG_USB_ROLE_DET_BASE 0x0

#define REG_USB_ROLE_DET_BASE_HI BASE_HI(REG_USB_ROLE_DET_BASE)
#define REG_USB_ROLE_DET_BASE_LO BASE_LO(REG_USB_ROLE_DET_BASE)

#define REG_INTERVAL                   REG_USB_ROLE_DET_BASE_LO+0x00
#define REG_INT_STATUS                 REG_USB_ROLE_DET_BASE_LO+0x04
#define REG_INT_CLR                    REG_USB_ROLE_DET_BASE_LO+0x08
#define REG_IRQ_MASK                   REG_USB_ROLE_DET_BASE_LO+0x0c

//INTERVAL
#define USB_ROLE_DET_DET_INTERVAL(n)   (((n)&0x7)<<0)      
#define USB_ROLE_DET_LOOP_INTERVAL(n)  (((n)&0x7)<<3)      

//INT_STATUS
#define USB_ROLE_DET_INT_STATUS        (1<<0)              

//INT_CLR
#define USB_ROLE_DET_INT_CLEAR         (1<<1)              

//IRQ_MASK
#define USB_ROLE_DET_INT_MASK          (1<<0)              


#endif


