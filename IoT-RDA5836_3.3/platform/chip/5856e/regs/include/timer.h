//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : timer                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:59                                                  
// Last modified  : 2016-08-29 14:59:59                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:59 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __TIMER_H__
#define __TIMER_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ostimer_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 loadval                        :24; /*23:0 , Value loaded to OS timer.     */
        U32 enable                         :1 ; /*24:24, Write '1' to this bit will enable OS timer. When read, the value is what we have written to this bit, it changes immediately after been written.*/
        U32 enabled                        :1 ; /*25:25, Read this bit will get the information if OS timer is really enabled or not. This bit will change only after the next front of 16 KHz system clock.  0: Disable Indicates OS timer not enabled.1: Enable Indicates OS timer enabled.*/
        U32 cleared                        :1 ; /*26:26, Read this bit will get the information if OS timer interruption clear operation is finished or not. 0: Clear_finish Indicates no OS timer interruption clear operation is on going1: Clear_ongoing Indicates OS timer interruption clear operation is on going. .*/
        U32 reserved_0                     :1 ; /*27:27,                               */
        U32 repeat                         :1 ; /*28:28, Write '1' to this bit will set OS timer to repeat mode. When read, get the information if OS timer is in repeat mode. 0: No_repeat_md Indicates OS timer not in repeat mode.1: Repeat_md Indicates OS timer in repeat mode. */
        U32 wrap                           :1 ; /*29:29, Write '1' to this bit will set OS timer to wrap mode. When read, get the information if OS timer is in wrap mode. 0: No_wrap_md Indicates OS timer not in wrap mode.1: Wrap_md Indicates OS timer in wrap mode. */
        U32 load                           :1 ; /*30:30, Write '1' to this bit will load the initial value to OS timer.*/
        U32 reserved_1                     :1 ; /*31:31,                               */
    } b;
} T_TIMER_ostimer_ctrl;

//ostimer_curval
typedef union
{
    U32 v;
    struct
    {
        U32 curval                         :32; /*31:0 , Current value of OS timer. The value is 24 bits and the first 8 bits are sign extension of the most important bit. A negative value indicates that the timer has wraped.*/
    } b;
} T_TIMER_ostimer_curval;

//wdtimer_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 start                          :1 ; /*0 :0 , Write '1' to this bit will enable watchdog timer and Load it with WDTimer_LoadVal.*/
        U32 reserved_0                     :1 ; /*1 :1 ,                               */
        U32 reserved_1                     :1 ; /*2 :2 ,                               */
        U32 wdenabled                      :1 ; /*3 :3 , Read this bit will get the information if watchdog timer is really enabled or not. This bit will change only after the next front of 32 KHz system clock. 0: Disable Indicates watchdog timer is not enabled.1: Enable Indicates watchdog timer is enabled, if current watchdog timer value reaches 0, the system will be reseted. */
        U32 stop                           :1 ; /*4 :4 , Write '1' to this bit will stop watchdog timer.*/
        U32 reserved_2                     :1 ; /*5 :5 ,                               */
        U32 reserved_3                     :1 ; /*6 :6 ,                               */
        U32 reserved_4                     :1 ; /*7 :7 ,                               */
        U32 reserved_5                     :1 ; /*8 :8 ,                               */
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 reload                         :1 ; /*16:16, Write '1' to this bit will load WDTimer_LoadVal value to watchdog timer. Use this bit to implement the watchog keep alive.*/
        U32 reserved_13                    :1 ; /*17:17,                               */
        U32 reserved_14                    :1 ; /*18:18,                               */
        U32 reserved_15                    :1 ; /*19:19,                               */
        U32 reserved_16                    :1 ; /*20:20,                               */
        U32 reserved_17                    :1 ; /*21:21,                               */
        U32 reserved_18                    :1 ; /*22:22,                               */
        U32 reserved_19                    :1 ; /*23:23,                               */
        U32 reserved_20                    :1 ; /*24:24,                               */
        U32 reserved_21                    :1 ; /*25:25,                               */
        U32 reserved_22                    :1 ; /*26:26,                               */
        U32 reserved_23                    :1 ; /*27:27,                               */
        U32 reserved_24                    :1 ; /*28:28,                               */
        U32 reserved_25                    :1 ; /*29:29,                               */
        U32 reserved_26                    :1 ; /*30:30,                               */
        U32 reserved_27                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_wdtimer_ctrl;

//wdtimer_loadval
typedef union
{
    U32 v;
    struct
    {
        U32 loadval                        :24; /*23:0 , Load value of watchdog timer. Number of 32kHz Clock before Reset. */
        U32 reserved_0                     :1 ; /*24:24,                               */
        U32 reserved_1                     :1 ; /*25:25,                               */
        U32 reserved_2                     :1 ; /*26:26,                               */
        U32 reserved_3                     :1 ; /*27:27,                               */
        U32 reserved_4                     :1 ; /*28:28,                               */
        U32 reserved_5                     :1 ; /*29:29,                               */
        U32 reserved_6                     :1 ; /*30:30,                               */
        U32 reserved_7                     :1 ; /*31:31,                               */
    } b;
} T_TIMER_wdtimer_loadval;

//hwtimer_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 interval                       :2 ; /*1 :0 , interval of generating an HwTimer IRQ. 2'b00: one_eighth Interval of 1/8 second. 2'b01: one_quarter Interval of 1/4 second. 2'b10: half Interval of 1/2 second. 2'b11: one Interval of 1 second.*/
        U32 reserved_0                     :1 ; /*2 :2 ,                               */
        U32 reserved_1                     :1 ; /*3 :3 ,                               */
        U32 reserved_2                     :1 ; /*4 :4 ,                               */
        U32 reserved_3                     :1 ; /*5 :5 ,                               */
        U32 reserved_4                     :1 ; /*6 :6 ,                               */
        U32 reserved_5                     :1 ; /*7 :7 ,                               */
        U32 interval_en                    :1 ; /*8 :8 , This bit enables interval IRQ mode. 0: Disable HW delay timer does not generate interval IRQ. 1: Enable HW delay timer generate an IRQ each interval.*/
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 reserved_13                    :1 ; /*16:16,                               */
        U32 reserved_14                    :1 ; /*17:17,                               */
        U32 reserved_15                    :1 ; /*18:18,                               */
        U32 reserved_16                    :1 ; /*19:19,                               */
        U32 reserved_17                    :1 ; /*20:20,                               */
        U32 reserved_18                    :1 ; /*21:21,                               */
        U32 reserved_19                    :1 ; /*22:22,                               */
        U32 reserved_20                    :1 ; /*23:23,                               */
        U32 reserved_21                    :1 ; /*24:24,                               */
        U32 reserved_22                    :1 ; /*25:25,                               */
        U32 reserved_23                    :1 ; /*26:26,                               */
        U32 reserved_24                    :1 ; /*27:27,                               */
        U32 reserved_25                    :1 ; /*28:28,                               */
        U32 reserved_26                    :1 ; /*29:29,                               */
        U32 reserved_27                    :1 ; /*30:30,                               */
        U32 reserved_28                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_hwtimer_ctrl;

//hwtimer_curval
typedef union
{
    U32 v;
    struct
    {
        U32 curval                         :32; /*31:0 , Current value of the hardware delay timer. The value is incremented every 61 us. This timer is running all the time and wrap at value 0xFFFFFFFF.*/
    } b;
} T_TIMER_hwtimer_curval;

//timer_irq_mask_set
typedef union
{
    U32 v;
    struct
    {
        U32 ostimer_mask                   :1 ; /*0 :0 , Set mask for OS timer IRQ.    */
        U32 hwtimer_wrap_mask              :1 ; /*1 :1 , Set mask for hardwre delay timer wrap IRQ.*/
        U32 hwtimer_itv_mask               :1 ; /*2 :2 , Set mask for hardwre delay timer interval IRQ.*/
        U32 reserved_0                     :1 ; /*3 :3 ,                               */
        U32 reserved_1                     :1 ; /*4 :4 ,                               */
        U32 reserved_2                     :1 ; /*5 :5 ,                               */
        U32 reserved_3                     :1 ; /*6 :6 ,                               */
        U32 reserved_4                     :1 ; /*7 :7 ,                               */
        U32 reserved_5                     :1 ; /*8 :8 ,                               */
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 reserved_13                    :1 ; /*16:16,                               */
        U32 reserved_14                    :1 ; /*17:17,                               */
        U32 reserved_15                    :1 ; /*18:18,                               */
        U32 reserved_16                    :1 ; /*19:19,                               */
        U32 reserved_17                    :1 ; /*20:20,                               */
        U32 reserved_18                    :1 ; /*21:21,                               */
        U32 reserved_19                    :1 ; /*22:22,                               */
        U32 reserved_20                    :1 ; /*23:23,                               */
        U32 reserved_21                    :1 ; /*24:24,                               */
        U32 reserved_22                    :1 ; /*25:25,                               */
        U32 reserved_23                    :1 ; /*26:26,                               */
        U32 reserved_24                    :1 ; /*27:27,                               */
        U32 reserved_25                    :1 ; /*28:28,                               */
        U32 reserved_26                    :1 ; /*29:29,                               */
        U32 reserved_27                    :1 ; /*30:30,                               */
        U32 reserved_28                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_timer_irq_mask_set;

//timer_irq_mask_clr
typedef union
{
    U32 v;
    struct
    {
        U32 ostimer_mask                   :1 ; /*0 :0 , Clear mask for OS timer IRQ.  */
        U32 hwtimer_wrap_mask              :1 ; /*1 :1 , Clear mask for hardwre delay timer wrap IRQ.*/
        U32 hwtimer_itv_mask               :1 ; /*2 :2 , Clear mask for hardwre delay timer interval IRQ.*/
        U32 reserved_0                     :1 ; /*3 :3 ,                               */
        U32 reserved_1                     :1 ; /*4 :4 ,                               */
        U32 reserved_2                     :1 ; /*5 :5 ,                               */
        U32 reserved_3                     :1 ; /*6 :6 ,                               */
        U32 reserved_4                     :1 ; /*7 :7 ,                               */
        U32 reserved_5                     :1 ; /*8 :8 ,                               */
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 reserved_13                    :1 ; /*16:16,                               */
        U32 reserved_14                    :1 ; /*17:17,                               */
        U32 reserved_15                    :1 ; /*18:18,                               */
        U32 reserved_16                    :1 ; /*19:19,                               */
        U32 reserved_17                    :1 ; /*20:20,                               */
        U32 reserved_18                    :1 ; /*21:21,                               */
        U32 reserved_19                    :1 ; /*22:22,                               */
        U32 reserved_20                    :1 ; /*23:23,                               */
        U32 reserved_21                    :1 ; /*24:24,                               */
        U32 reserved_22                    :1 ; /*25:25,                               */
        U32 reserved_23                    :1 ; /*26:26,                               */
        U32 reserved_24                    :1 ; /*27:27,                               */
        U32 reserved_25                    :1 ; /*28:28,                               */
        U32 reserved_26                    :1 ; /*29:29,                               */
        U32 reserved_27                    :1 ; /*30:30,                               */
        U32 reserved_28                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_timer_irq_mask_clr;

//timer_irq_clr
typedef union
{
    U32 v;
    struct
    {
        U32 ostimer_clr                    :1 ; /*0 :0 , Clear OS timer IRQ.           */
        U32 hwtimer_wrap_clr               :1 ; /*1 :1 , Clear hardware delay timer wrap IRQ.*/
        U32 hwtimer_itv_clr                :1 ; /*2 :2 , Clear hardware delay timer interval IRQ.*/
        U32 reserved_0                     :1 ; /*3 :3 ,                               */
        U32 reserved_1                     :1 ; /*4 :4 ,                               */
        U32 reserved_2                     :1 ; /*5 :5 ,                               */
        U32 reserved_3                     :1 ; /*6 :6 ,                               */
        U32 reserved_4                     :1 ; /*7 :7 ,                               */
        U32 reserved_5                     :1 ; /*8 :8 ,                               */
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 reserved_13                    :1 ; /*16:16,                               */
        U32 reserved_14                    :1 ; /*17:17,                               */
        U32 reserved_15                    :1 ; /*18:18,                               */
        U32 reserved_16                    :1 ; /*19:19,                               */
        U32 reserved_17                    :1 ; /*20:20,                               */
        U32 reserved_18                    :1 ; /*21:21,                               */
        U32 reserved_19                    :1 ; /*22:22,                               */
        U32 reserved_20                    :1 ; /*23:23,                               */
        U32 reserved_21                    :1 ; /*24:24,                               */
        U32 reserved_22                    :1 ; /*25:25,                               */
        U32 reserved_23                    :1 ; /*26:26,                               */
        U32 reserved_24                    :1 ; /*27:27,                               */
        U32 reserved_25                    :1 ; /*28:28,                               */
        U32 reserved_26                    :1 ; /*29:29,                               */
        U32 reserved_27                    :1 ; /*30:30,                               */
        U32 reserved_28                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_timer_irq_clr;

//timer_irq_cause
typedef union
{
    U32 v;
    struct
    {
        U32 ostimer_cause                  :1 ; /*0 :0 , OS timer IRQ cause.           */
        U32 hwtimer_wrap_cause             :1 ; /*1 :1 , hardware delay timer wrap IRQ cause.*/
        U32 hwtimer_itv_cause              :1 ; /*2 :2 , hardware delay timer interval IRQ cause.*/
        U32 reserved_0                     :1 ; /*3 :3 ,                               */
        U32 reserved_1                     :1 ; /*4 :4 ,                               */
        U32 reserved_2                     :1 ; /*5 :5 ,                               */
        U32 reserved_3                     :1 ; /*6 :6 ,                               */
        U32 reserved_4                     :1 ; /*7 :7 ,                               */
        U32 reserved_5                     :1 ; /*8 :8 ,                               */
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 reserved_9                     :1 ; /*12:12,                               */
        U32 reserved_10                    :1 ; /*13:13,                               */
        U32 reserved_11                    :1 ; /*14:14,                               */
        U32 reserved_12                    :1 ; /*15:15,                               */
        U32 ostimer_status                 :1 ; /*16:16, OS timer IRQ status.          */
        U32 hwtimer_wrap_status            :1 ; /*17:17, hardware delay timer wrap IRQ status.*/
        U32 hwtimer_itv_status             :1 ; /*18:18, hardware delay timer interval IRQ status.*/
        U32 reserved_13                    :1 ; /*19:19,                               */
        U32 reserved_14                    :1 ; /*20:20,                               */
        U32 reserved_15                    :1 ; /*21:21,                               */
        U32 reserved_16                    :1 ; /*22:22,                               */
        U32 reserved_17                    :1 ; /*23:23,                               */
        U32 reserved_18                    :1 ; /*24:24,                               */
        U32 reserved_19                    :1 ; /*25:25,                               */
        U32 reserved_20                    :1 ; /*26:26,                               */
        U32 reserved_21                    :1 ; /*27:27,                               */
        U32 reserved_22                    :1 ; /*28:28,                               */
        U32 reserved_23                    :1 ; /*29:29,                               */
        U32 reserved_24                    :1 ; /*30:30,                               */
        U32 reserved_25                    :1 ; /*31:31,                               */
    } b;
} T_TIMER_timer_irq_cause;

typedef struct
{
    volatile            T_TIMER_ostimer_ctrl                   ostimer_ctrl; /*0x0  , RW   , 0x00000000, No description.               */
    volatile          T_TIMER_ostimer_curval                 ostimer_curval; /*0x4  , RO   , 0x00000000, No description.               */
    volatile            T_TIMER_wdtimer_ctrl                   wdtimer_ctrl; /*0x8  , RW   , 0x00000000, No description.               */
    volatile         T_TIMER_wdtimer_loadval                wdtimer_loadval; /*0xc  , RW   , 0x00000000, No description.               */
    volatile            T_TIMER_hwtimer_ctrl                   hwtimer_ctrl; /*0x10 , RW   , 0x00000000, No description.               */
    volatile          T_TIMER_hwtimer_curval                 hwtimer_curval; /*0x14 , RO   , 0x00000000, No description.               */
    volatile      T_TIMER_timer_irq_mask_set             timer_irq_mask_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile      T_TIMER_timer_irq_mask_clr             timer_irq_mask_clr; /*0x1c , RW   , 0x00000000, No description.               */
    volatile           T_TIMER_timer_irq_clr                  timer_irq_clr; /*0x20 , RW   , 0x00000000, No description.               */
    volatile         T_TIMER_timer_irq_cause                timer_irq_cause; /*0x24 , RO   , 0x00000000, No description.               */
} T_HWP_TIMER_T;
#else
typedef struct
{
    volatile                             U32                   ostimer_ctrl; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                             U32                 ostimer_curval; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                   wdtimer_ctrl; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                wdtimer_loadval; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                   hwtimer_ctrl; /*0x10 , RW   , 0x00000000, No description.               */
    volatile                             U32                 hwtimer_curval; /*0x14 , RO   , 0x00000000, No description.               */
    volatile                             U32             timer_irq_mask_set; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                             U32             timer_irq_mask_clr; /*0x1c , RW   , 0x00000000, No description.               */
    volatile                             U32                  timer_irq_clr; /*0x20 , RW   , 0x00000000, No description.               */
    volatile                             U32                timer_irq_cause; /*0x24 , RO   , 0x00000000, No description.               */
} T_HWP_TIMER_T;

//ostimer_ctrl
#define TIMER_LOADVAL(n)               (((n)&0xFFFFFF)<<0) 
#define TIMER_LOADVAL_MASK             (0xFFFFFF<<0)       
#define TIMER_LOADVAL_SHIFT            (0)                 

#define TIMER_ENABLE                   (1<<24)             

#define TIMER_ENABLED                  (1<<25)             
#define TIMER_ENABLED_DISABLED         (0<<25)             
#define TIMER_ENABLED_ENABLED          (1<<25)             

#define TIMER_CLEARED                  (1<<26)             
#define TIMER_CLEARED_CLEAR_FINISH     (0<<26)             
#define TIMER_CLEARED_CLEAR_ONGOING    (1<<26)             

#define TIMER_REPEAT                   (1<<28)             
#define TIMER_REPEAT_NO_REPEAT_MD      (0<<28)             
#define TIMER_REPEAT_REPEAT_MD         (1<<28)             

#define TIMER_WRAP                     (1<<29)             
#define TIMER_WRAP_NO_WRAP_MD          (0<<29)             
#define TIMER_WRAP_WRAP_MD             (1<<29)             

#define TIMER_LOAD                     (1<<30)             


//ostimer_curval
#define TIMER_CURVAL(n)                (((n)&0xFFFFFFFF)<<0)
#define TIMER_CURVAL_MASK              (0xFFFFFFFF<<0)     
#define TIMER_CURVAL_SHIFT             (0)                 


//wdtimer_ctrl
#define TIMER_START                    (1<<0)              

#define TIMER_WDENABLED                (1<<3)              
#define TIMER_WDENABLED_DISABLE        (1<<3)              
#define TIMER_WDENABLED_ENABLE         (1<<3)              

#define TIMER_STOP                     (1<<4)              

#define TIMER_RELOAD                   (1<<16)             


//wdtimer_loadval
#define TIMER_LOADVAL(n)               (((n)&0xFFFFFF)<<0) 
#define TIMER_LOADVAL_MASK             (0xFFFFFF<<0)       
#define TIMER_LOADVAL_SHIFT            (0)                 


//hwtimer_ctrl
#define TIMER_INTERVAL(n)              (((n)&0x3)<<0)      
#define TIMER_INTERVAL_ONE_EIGHTH      (((0)&0x3)<<0)      
#define TIMER_INTERVAL_ONE_QUARTER     (((1)&0x3)<<0)      
#define TIMER_INTERVAL_HALF_INTERVAL   (((2)&0x3)<<0)      
#define TIMER_INTERVAL_ONE_INTERVAL    (((3)&0x3)<<0)      
#define TIMER_INTERVAL_MASK            (0x3<<0)            
#define TIMER_INTERVAL_SHIFT           (0)                 

#define TIMER_INTERVAL_EN              (1<<8)              
#define TIMER_INTERVAL_EN_DISABLE      (1<<8)              
#define TIMER_INTERVAL_EN_ENABLE       (1<<8)              


//hwtimer_curval
#define TIMER_CURVAL(n)                (((n)&0xFFFFFFFF)<<0)
#define TIMER_CURVAL_MASK              (0xFFFFFFFF<<0)     
#define TIMER_CURVAL_SHIFT             (0)                 


//timer_irq_mask_set
#define TIMER_OSTIMER_MASK             (1<<0)              

#define TIMER_HWTIMER_WRAP_MASK        (1<<1)              

#define TIMER_HWTIMER_ITV_MASK         (1<<2)              


//timer_irq_mask_clr
#define TIMER_OSTIMER_MASK             (1<<0)              

#define TIMER_HWTIMER_WRAP_MASK        (1<<1)              

#define TIMER_HWTIMER_ITV_MASK         (1<<2)              


//timer_irq_clr
#define TIMER_OSTIMER_CLR              (1<<0)              

#define TIMER_HWTIMER_WRAP_CLR         (1<<1)              

#define TIMER_HWTIMER_ITV_CLR          (1<<2)              


//timer_irq_cause
#define TIMER_OSTIMER_CAUSE            (1<<0)              

#define TIMER_HWTIMER_WRAP_CAUSE       (1<<1)              

#define TIMER_HWTIMER_ITV_CAUSE        (1<<2)              

#define TIMER_OSTIMER_STATUS           (1<<16)             

#define TIMER_HWTIMER_WRAP_STATUS      (1<<17)             

#define TIMER_HWTIMER_ITV_STATUS       (1<<18)             


#endif

#endif


