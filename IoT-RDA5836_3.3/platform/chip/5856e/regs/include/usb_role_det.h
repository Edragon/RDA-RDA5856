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
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//det_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 det_interval                   :4 ; /*3 :0 , interval of detect  
0: 250us
1:500us
2: 750us
3: 1ms
4:  1.25ms
5:1.5ms
6: 1.75ms
7:2ms*/
        U32 loop_interval                  :3 ; /*6 :4 , interval of loop when no valid mode detected  
0: 0.1s
1:0.15s
2: 0.25s
3: 0.30s
4:  0.5s
5:0.75s
6: 0.85s
7:1.00s*/
        U32 det_cfg_reserved_1             :1 ; /*7 :7 , No description.               */
        U32 wait_interval                  :4 ; /*11:8 , interval of loop when no valid mode detected  
0: 0.1s
1:0.15s
2: 0.25s
3: 0.30s
4:  0.5s
5:0.75s
6: 0.85s
7:1.00s*/
        U32 loop_time                      :4 ; /*15:12, interval of loop when no valid mode detected  
0: always loop if need
1:1time
2: 2times
3: 3times
4: 4times
5: 5times
6: 6times
7: 7times
8: 8times
9: 9times
10: 10times
11: 11times
12: 12time
13: 13times
14: 14times
15: 15times*/
        U32 det_cfg_reserved_0             :16; /*31:16, No description.               */
    } b;
} T_USB_ROLE_DET_det_cfg;

//force_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 force_en                       :1 ; /*0 :0 , No description.               */
        U32 force_id                       :1 ; /*1 :1 , No description.               */
        U32 force_cfg_reserved_0           :30; /*31:2 , No description.               */
    } b;
} T_USB_ROLE_DET_force_cfg;

//irq_clear
typedef union
{
    U32 v;
    struct
    {
        U32 role_det                       :1 ; /*0 :0 , 1'indicates the usb mode is detected*/
        U32 timeout                        :1 ; /*1 :1 , 1'indicates the usb mode is timeout*/
        U32 irq_clear_reserved_0           :30; /*31:2 , No description.               */
    } b;
} T_USB_ROLE_DET_irq_clear;

//irq_status
typedef union
{
    U32 v;
    struct
    {
        U32 det_int                        :1 ; /*0 :0 , No description.               */
        U32 timeout_int                    :1 ; /*1 :1 , No description.               */
        U32 host_mode                      :1 ; /*2 :2 , No description.               */
        U32 device_mode                    :1 ; /*3 :3 , No description.               */
        U32 usb_id                         :1 ; /*4 :4 , No description.               */
        U32 irq_status_reserved_0          :27; /*31:5 , No description.               */
    } b;
} T_USB_ROLE_DET_irq_status;

//irq_mask
typedef union
{
    U32 v;
    struct
    {
        U32 det_mask                       :1 ; /*0 :0 , No description.               */
        U32 timeout_mask                   :1 ; /*1 :1 , No description.               */
        U32 irq_mask_reserved_0            :30; /*31:2 , No description.               */
    } b;
} T_USB_ROLE_DET_irq_mask;

typedef struct
{
    volatile          T_USB_ROLE_DET_det_cfg                        det_cfg; /*0x0  , RW   , 0x00000000, No description.               */
    volatile        T_USB_ROLE_DET_force_cfg                      force_cfg; /*0x4  , RW   , 0x00000000, No description.               */
    volatile        T_USB_ROLE_DET_irq_clear                      irq_clear; /*0x8  , RW   , 0x00000000, No description.               */
    volatile       T_USB_ROLE_DET_irq_status                     irq_status; /*0xc  , RO   , 0x00000018, No description.               */
    volatile         T_USB_ROLE_DET_irq_mask                       irq_mask; /*0x10 , RW   , 0x00000000, No description.               */
} T_HWP_USB_ROLE_DET_T;
#else
typedef struct
{
    volatile                             U32                        det_cfg; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                             U32                      force_cfg; /*0x4  , RW   , 0x00000000, No description.               */
    volatile                             U32                      irq_clear; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                     irq_status; /*0xc  , RO   , 0x00000018, No description.               */
    volatile                             U32                       irq_mask; /*0x10 , RW   , 0x00000000, No description.               */
} T_HWP_USB_ROLE_DET_T;

#define USB_ROLE_DET_DET_INTERVAL(n)   (((n)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_250US (((0)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_500US (((1)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_750US (((2)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_1MS  (((3)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_1P25MS (((4)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_1P5MS (((5)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_1P75MS (((6)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_2MS  (((7)&0xF)<<0)      
#define USB_ROLE_DET_DET_INTERVAL_MASK (0xF<<0)            
#define USB_ROLE_DET_DET_INTERVAL_SHIFT (0)                 

#define USB_ROLE_DET_LOOP_INTERVAL(n)  (((n)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P1S (((0)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P15S (((1)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P25S (((2)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P30S (((3)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P5S (((4)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P75S (((5)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_0P85S (((6)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_1S  (((7)&0x7)<<4)      
#define USB_ROLE_DET_LOOP_INTERVAL_MASK (0x7<<4)            
#define USB_ROLE_DET_LOOP_INTERVAL_SHIFT (4)                 

#define USB_ROLE_DET_WAIT_INTERVAL(n)  (((n)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P1S (((0)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P15S (((1)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P25S (((2)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P30S (((3)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P5S (((4)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P75S (((5)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_0P85S (((6)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_1S  (((7)&0xF)<<8)      
#define USB_ROLE_DET_WAIT_INTERVAL_MASK (0xF<<8)            
#define USB_ROLE_DET_WAIT_INTERVAL_SHIFT (8)                 

#define USB_ROLE_DET_LOOP_TIME(n)      (((n)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_1TIME   (((1)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_2TIMES  (((2)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_3TIMES  (((3)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_4TIMES  (((4)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_5TIMES  (((5)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_6TIMES  (((6)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_7TIMES  (((7)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_8TIMES  (((8)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_9TIMES  (((9)&0xF)<<12)     
#define USB_ROLE_DET_LOOP_TIME_10TIMES (((10)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_11TIMES (((11)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_12TIME  (((12)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_13TIMES (((13)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_14TIMES (((14)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_15TIMES (((15)&0xF)<<12)    
#define USB_ROLE_DET_LOOP_TIME_MASK    (0xF<<12)           
#define USB_ROLE_DET_LOOP_TIME_SHIFT   (12)                


#define USB_ROLE_DET_FORCE_EN          (1<<0)              

#define USB_ROLE_DET_FORCE_ID          (1<<1)              


#define USB_ROLE_DET_ROLE_DET          (1<<0)              

#define USB_ROLE_DET_TIMEOUT           (1<<1)              


#define USB_ROLE_DET_DET_INT           (1<<0)              

#define USB_ROLE_DET_TIMEOUT_INT       (1<<1)              

#define USB_ROLE_DET_HOST_MODE         (1<<2)              

#define USB_ROLE_DET_DEVICE_MODE       (1<<3)              

#define USB_ROLE_DET_USB_ID            (1<<4)              


#define USB_ROLE_DET_DET_MASK          (1<<0)              

#define USB_ROLE_DET_TIMEOUT_MASK      (1<<1)              


#endif

#endif
