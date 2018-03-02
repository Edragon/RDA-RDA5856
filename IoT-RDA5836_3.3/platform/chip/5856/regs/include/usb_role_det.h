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
// Filename               :usb_role_det.h
// Author                 :tianwq
// Created On             :2015-08-29 14:51:41
// Last Modified          :
// Update Count           :2015-08-29 14:51:41
// Description            :
//
//======================================================================
#ifndef __USB_ROLE_DET_H__
#define __USB_ROLE_DET_H__
//Auto-gen by reg_gen
#include "cs_types.h"

#define REG_USB_ROLE_DET_BASE 0x01a2e000 //shaohuima

typedef volatile struct
{
    REG32 INTERVAL                      ; //0x00    
    REG32 FORCE_CFG                     ; //0x04    
    REG32 INT_CLR                       ; //0x08  
    REG32 INT_STATUS                    ; //0x0c
    REG32 IRQ_MASK                      ; //0x10    
} HWP_USB_ROLE_DET_T;

#define hwp_usb_role_det ((HWP_USB_ROLE_DET_T*)KSEG1(REG_USB_ROLE_DET_BASE))

//INTERVAL
#define USB_ROLE_DET_DET_INTERVAL(n)   (((n)&0x7)<<0)      
#define USB_ROLE_DET_LOOP_INTERVAL(n)  (((n)&0x7)<<4) 
#define USB_ROLE_DET_wait_INTERVAL(n)  (((n)&0x7)<<8) 
#define USB_ROLE_DET_LOOP_TIME(n)  (((n)&0xF)<<12)      
#define USB_ROLE_DET_LOOP_TIME_MASK      (0xF<<12)

//INT_STATUS
#define USB_ROLE_DET_INT_STATUS        (1<<0)              

//INT_CLR
#define USB_ROLE_DET_INT_CLEAR         (1<<0)              

//IRQ_MASK
#define USB_ROLE_DET_INT_MASK_Det          (1<<0)

#define USB_ROLE_DET_INT_MASK_Timeout          (1<<1)



#endif


