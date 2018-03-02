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
#include "base_address.h"

#define REG_det_cfg                    REG_USB_ROLE_DET_BASE+0x00
#define REG_force_cfg                  REG_USB_ROLE_DET_BASE+0x04
#define REG_irq_clear                  REG_USB_ROLE_DET_BASE+0x08
#define REG_irq_status                 REG_USB_ROLE_DET_BASE+0x0c
#define REG_irq_mask                   REG_USB_ROLE_DET_BASE+0x10

//det_cfg
#define USB_ROLE_DET_DET_INTERVAL(n)   (((n)&0xF)<<0)      
#define USB_ROLE_DET_LOOP_INTERVAL(n)  (((n)&0x7)<<4)      
#define USB_ROLE_DET_WAIT_INTERVAL(n)  (((n)&0xF)<<8)      
#define USB_ROLE_DET_LOOP_TIME(n)      (((n)&0xF)<<12)     

//force_cfg
#define USB_ROLE_DET_FORCE_EN          (1<<0)              
#define USB_ROLE_DET_FORCE_ID          (1<<1)              

//irq_clear
#define USB_ROLE_DET_ROLE_DET          (1<<0)              
#define USB_ROLE_DET_TIMEOUT           (1<<1)              

//irq_status
#define USB_ROLE_DET_DET_INT           (1<<0)              
#define USB_ROLE_DET_TIMEOUT_INT       (1<<1)              
#define USB_ROLE_DET_HOST_MODE         (1<<2)              
#define USB_ROLE_DET_DEVICE_MODE       (1<<3)              
#define USB_ROLE_DET_USB_ID            (1<<4)              

//irq_mask
#define USB_ROLE_DET_DET_MASK          (1<<0)              
#define USB_ROLE_DET_TIMEOUT_MASK      (1<<1)              


#endif
