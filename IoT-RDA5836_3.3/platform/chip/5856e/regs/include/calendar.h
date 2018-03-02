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
// File name      : calendar                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:45                                                  
// Last modified  : 2016-08-29 14:59:45                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:45 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __CALENDAR_H__
#define __CALENDAR_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 interval                       :2 ; /*1 :0 , These 2 bits configure the interval of generating an IRQ status.0: DISABLE1: PER_SEC2: PER_MIN3: PER_HOUR*/
        U32 reserved_0                     :1 ; /*2 :2 ,                               */
        U32 reserved_1                     :1 ; /*3 :3 ,                               */
        U32 reserved_2                     :1 ; /*4 :4 ,                               */
        U32 reserved_3                     :1 ; /*5 :5 ,                               */
        U32 reserved_4                     :1 ; /*6 :6 ,                               */
        U32 reserved_5                     :1 ; /*7 :7 ,                               */
        U32 reserved_6                     :1 ; /*8 :8 ,                               */
        U32 reserved_7                     :1 ; /*9 :9 ,                               */
        U32 reserved_8                     :1 ; /*10:10,                               */
        U32 reserved_9                     :1 ; /*11:11,                               */
        U32 reserved_10                    :1 ; /*12:12,                               */
        U32 reserved_11                    :1 ; /*13:13,                               */
        U32 reserved_12                    :1 ; /*14:14,                               */
        U32 reserved_13                    :1 ; /*15:15,                               */
        U32 reserved_14                    :1 ; /*16:16,                               */
        U32 reserved_15                    :1 ; /*17:17,                               */
        U32 reserved_16                    :1 ; /*18:18,                               */
        U32 reserved_17                    :1 ; /*19:19,                               */
        U32 reserved_18                    :1 ; /*20:20,                               */
        U32 reserved_19                    :1 ; /*21:21,                               */
        U32 reserved_20                    :1 ; /*22:22,                               */
        U32 reserved_21                    :1 ; /*23:23,                               */
        U32 reserved_22                    :1 ; /*24:24,                               */
        U32 reserved_23                    :1 ; /*25:25,                               */
        U32 reserved_24                    :1 ; /*26:26,                               */
        U32 reserved_25                    :1 ; /*27:27,                               */
        U32 reserved_26                    :1 ; /*28:28,                               */
        U32 reserved_27                    :1 ; /*29:29,                               */
        U32 reserved_28                    :1 ; /*30:30,                               */
        U32 reserved_29                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_ctrl;

//cmd
typedef union
{
    U32 v;
    struct
    {
        U32 calendar_load                  :1 ; /*0 :0 , When write, command to program calendar with a new value (sec, min, hour, day, month, year, day of week) previously written in registers Calendar_LoadVal_H and Calendar_LoadVal_L. This bit is auto cleared. '1' = load calendar timer. When read, Calendar timer load status. '1' = Calendar load has not finished. '0' = Calendar load has finished.*/
        U32 reserved_0                     :1 ; /*1 :1 ,                               */
        U32 reserved_1                     :1 ; /*2 :2 ,                               */
        U32 reserved_2                     :1 ; /*3 :3 ,                               */
        U32 alarm_load                     :1 ; /*4 :4 , When write, command to program alarm with a new value (sec, min, hour, day, month, year, day of week) prviously written in registers AlarmVal_H and AlarmVal_L. This bit is auto cleared. '1' = load alarm. When read, alarm load status. '1' = alarm load has not finished. '0' = alarm load has finished.*/
        U32 alarm_enable_set               :1 ; /*5 :5 , command to enable alarm. When alarm is triggered, it will generate a wakup. '1' = enable alarm. When read, alarm enable status. '1' = alarm enable operation is on going, not finished. '0' = alarm is enabled.*/
        U32 alarm_enable_clr               :1 ; /*6 :6 , command to disable alarm. '1' = disable alarm. When read, alarm enable status. '1' = alarm disable operation is on going, not finished. '0' = alarm is disabled.*/
        U32 reserved_3                     :1 ; /*7 :7 ,                               */
        U32 alarm_clr                      :1 ; /*8 :8 , writing '1', clear Alarm triggered signal (connect to wakeup) and alarm triggered IRQ. When read, get alarm clear status. '1' = alarm clear operation is on going, not finished. '0' = alarm is cleared.*/
        U32 itv_irq_clr                    :1 ; /*9 :9 , writing '1', clear interval IRQ.*/
        U32 reserved_4                     :1 ; /*10:10,                               */
        U32 reserved_5                     :1 ; /*11:11,                               */
        U32 reserved_6                     :1 ; /*12:12,                               */
        U32 reserved_7                     :1 ; /*13:13,                               */
        U32 reserved_8                     :1 ; /*14:14,                               */
        U32 reserved_9                     :1 ; /*15:15,                               */
        U32 itv_irq_mask_set               :1 ; /*16:16, When write '1', Set interval Irq Mask. When read, get interval Irq mask.*/
        U32 itv_irq_mask_clr               :1 ; /*17:17, When write '1', Clear interval Irq Mask. When read, get inteval Irq mask.*/
        U32 reserved_10                    :1 ; /*18:18,                               */
        U32 reserved_11                    :1 ; /*19:19,                               */
        U32 reserved_12                    :1 ; /*20:20,                               */
        U32 reserved_13                    :1 ; /*21:21,                               */
        U32 reserved_14                    :1 ; /*22:22,                               */
        U32 reserved_15                    :1 ; /*23:23,                               */
        U32 reserved_16                    :1 ; /*24:24,                               */
        U32 reserved_17                    :1 ; /*25:25,                               */
        U32 reserved_18                    :1 ; /*26:26,                               */
        U32 reserved_19                    :1 ; /*27:27,                               */
        U32 reserved_20                    :1 ; /*28:28,                               */
        U32 reserved_21                    :1 ; /*29:29,                               */
        U32 reserved_22                    :1 ; /*30:30,                               */
        U32 calendar_not_valid             :1 ; /*31:31, When write '1', mark calendar value to be not valid. When read, Indicate if the Calendar value is valid or not. The calendar value is not valid in case of mismatch between the calendar counter and the APB register, which is the case of wakeup the phone after shut down. This mismatch disappear after one RTC cycle or after re-porgramming a new calendar value. '1' = not valid.*/
    } b;
} T_CALENDAR_cmd;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 itv_irq_cause                  :1 ; /*0 :0 , Interval Irq Cause.           */
        U32 alarm_irq_cause                :1 ; /*1 :1 , Alarm Irq Cause.              */
        U32 reserved_0                     :1 ; /*2 :2 ,                               */
        U32 reserved_1                     :1 ; /*3 :3 ,                               */
        U32 reserved_2                     :1 ; /*4 :4 ,                               */
        U32 reserved_3                     :1 ; /*5 :5 ,                               */
        U32 reserved_4                     :1 ; /*6 :6 ,                               */
        U32 reserved_5                     :1 ; /*7 :7 ,                               */
        U32 force_wakeup                   :1 ; /*8 :8 , Force Wakeup status. After set "Force_Wakeup" to '1' in sys_ctrl, the real force_wakeup is not set immediatly, this bit indicates when the force wakeup is really set. This bits also indicates if the interface between Calendar domain and Core domain is enabled. '1': force wakeup set.*/
        U32 reserved_6                     :1 ; /*9 :9 ,                               */
        U32 reserved_7                     :1 ; /*10:10,                               */
        U32 reserved_8                     :1 ; /*11:11,                               */
        U32 chg_mask                       :1 ; /*12:12, Charger Mask status. After set "Chg_Mask" to '1' in sys_ctrl, the real Chg_Mask line is not set immediatly, this bit indicates when the Chg_Mask line is really set. '1': Chg_Mask line set.*/
        U32 reserved_9                     :1 ; /*13:13,                               */
        U32 reserved_10                    :1 ; /*14:14,                               */
        U32 reserved_11                    :1 ; /*15:15,                               */
        U32 itv_irq_status                 :1 ; /*16:16, Interval Irq Status.          */
        U32 reserved_12                    :1 ; /*17:17,                               */
        U32 reserved_13                    :1 ; /*18:18,                               */
        U32 reserved_14                    :1 ; /*19:19,                               */
        U32 alarm_enable                   :1 ; /*20:20, Alarm Enable Status. Note: When calendar is not programmed, Alarm can be enabled or not. It is suggested to clear Alarm Enable when program RTC.*/
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
        U32 calendar_not_prog              :1 ; /*31:31, 1' = Calendar has not been programmed. This bit keep value '0' after the calendar is programmed once.*/
    } b;
} T_CALENDAR_status;

//calendar_loadval_l
typedef union
{
    U32 v;
    struct
    {
        U32 sec                            :6 ; /*5 :0 , Second value loaded to calendar, ranged from 0 to 59.*/
        U32 reserved_0                     :1 ; /*6 :6 ,                               */
        U32 reserved_1                     :1 ; /*7 :7 ,                               */
        U32 min                            :6 ; /*13:8 , Minute value loaded to calendar, ranged from 0 to 59.*/
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 hour                           :5 ; /*20:16, Hour value loaded to calendar, ranged from 0 to 23.*/
        U32 reserved_4                     :1 ; /*21:21,                               */
        U32 reserved_5                     :1 ; /*22:22,                               */
        U32 reserved_6                     :1 ; /*23:23,                               */
        U32 reserved_7                     :1 ; /*24:24,                               */
        U32 reserved_8                     :1 ; /*25:25,                               */
        U32 reserved_9                     :1 ; /*26:26,                               */
        U32 reserved_10                    :1 ; /*27:27,                               */
        U32 reserved_11                    :1 ; /*28:28,                               */
        U32 reserved_12                    :1 ; /*29:29,                               */
        U32 reserved_13                    :1 ; /*30:30,                               */
        U32 reserved_14                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_calendar_loadval_l;

//calendar_loadval_h
typedef union
{
    U32 v;
    struct
    {
        U32 day                            :5 ; /*4 :0 , Day value loaded to calendar, ranged from 1 to 31.*/
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 mon                            :4 ; /*11:8 , Month value loaded to calendar, ranged from 1 to 12.*/
        U32 reserved_3                     :1 ; /*12:12,                               */
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 reserved_5                     :1 ; /*14:14,                               */
        U32 reserved_6                     :1 ; /*15:15,                               */
        U32 year                           :7 ; /*22:16, Year value loaded to calendar, ranged from 0 to 127. Represent year 2000 to 2127.*/
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 weekday                        :3 ; /*26:24, Day of the week value loaded to calendar, ranged from 1 to 7. Represent Monday, Tuesday etc.*/
        U32 reserved_8                     :1 ; /*27:27,                               */
        U32 reserved_9                     :1 ; /*28:28,                               */
        U32 reserved_10                    :1 ; /*29:29,                               */
        U32 reserved_11                    :1 ; /*30:30,                               */
        U32 reserved_12                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_calendar_loadval_h;

//calendar_curval_l
typedef union
{
    U32 v;
    struct
    {
        U32 sec                            :6 ; /*5 :0 , Current Second value of calendar, ranged from 0 to 59.*/
        U32 reserved_0                     :1 ; /*6 :6 ,                               */
        U32 reserved_1                     :1 ; /*7 :7 ,                               */
        U32 min                            :6 ; /*13:8 , Current Minute value of calendar, ranged from 0 to 59.*/
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 hour                           :5 ; /*20:16, Current Hour value of calendar, ranged from 0 to 23.*/
        U32 reserved_4                     :1 ; /*21:21,                               */
        U32 reserved_5                     :1 ; /*22:22,                               */
        U32 reserved_6                     :1 ; /*23:23,                               */
        U32 reserved_7                     :1 ; /*24:24,                               */
        U32 reserved_8                     :1 ; /*25:25,                               */
        U32 reserved_9                     :1 ; /*26:26,                               */
        U32 reserved_10                    :1 ; /*27:27,                               */
        U32 reserved_11                    :1 ; /*28:28,                               */
        U32 reserved_12                    :1 ; /*29:29,                               */
        U32 reserved_13                    :1 ; /*30:30,                               */
        U32 reserved_14                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_calendar_curval_l;

//calendar_curval_h
typedef union
{
    U32 v;
    struct
    {
        U32 day                            :5 ; /*4 :0 , Current Day value of calendar, ranged from 1 to 31. Maximum number of days in each month are stored in the module, and leap year is supported, so February can have 28 or 29 days.*/
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 mon                            :4 ; /*11:8 , Current Month value of calendar, ranged from 1 to 12.*/
        U32 reserved_3                     :1 ; /*12:12,                               */
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 reserved_5                     :1 ; /*14:14,                               */
        U32 reserved_6                     :1 ; /*15:15,                               */
        U32 year                           :7 ; /*22:16, Current Year value of calendar, ranged from 0 to 127. Represent year 2000 to 2127.*/
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 weekday                        :3 ; /*26:24, Current Day of the week value of calendar, ranged from 1 to 7. Represent Monday, Tuesday etc.*/
        U32 reserved_8                     :1 ; /*27:27,                               */
        U32 reserved_9                     :1 ; /*28:28,                               */
        U32 reserved_10                    :1 ; /*29:29,                               */
        U32 reserved_11                    :1 ; /*30:30,                               */
        U32 reserved_12                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_calendar_curval_h;

//alarmval_l
typedef union
{
    U32 v;
    struct
    {
        U32 sec                            :6 ; /*5 :0 , Second value loaded to alarm, ranged from 0 to 59.*/
        U32 reserved_0                     :1 ; /*6 :6 ,                               */
        U32 reserved_1                     :1 ; /*7 :7 ,                               */
        U32 min                            :6 ; /*13:8 , Minute value loaded to alarm, ranged from 0 to 59.*/
        U32 reserved_2                     :1 ; /*14:14,                               */
        U32 reserved_3                     :1 ; /*15:15,                               */
        U32 hour                           :5 ; /*20:16, Hour value loaded to alarm, ranged from 0 to 23.*/
        U32 reserved_4                     :1 ; /*21:21,                               */
        U32 reserved_5                     :1 ; /*22:22,                               */
        U32 reserved_6                     :1 ; /*23:23,                               */
        U32 reserved_7                     :1 ; /*24:24,                               */
        U32 reserved_8                     :1 ; /*25:25,                               */
        U32 reserved_9                     :1 ; /*26:26,                               */
        U32 reserved_10                    :1 ; /*27:27,                               */
        U32 reserved_11                    :1 ; /*28:28,                               */
        U32 reserved_12                    :1 ; /*29:29,                               */
        U32 reserved_13                    :1 ; /*30:30,                               */
        U32 reserved_14                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_alarmval_l;

//alarmval_h
typedef union
{
    U32 v;
    struct
    {
        U32 day                            :5 ; /*4 :0 , Day value loaded to alarm, ranged from 1 to 31.*/
        U32 reserved_0                     :1 ; /*5 :5 ,                               */
        U32 reserved_1                     :1 ; /*6 :6 ,                               */
        U32 reserved_2                     :1 ; /*7 :7 ,                               */
        U32 mon                            :4 ; /*11:8 , Month value loaded to alarm, ranged from 1 to 12.*/
        U32 reserved_3                     :1 ; /*12:12,                               */
        U32 reserved_4                     :1 ; /*13:13,                               */
        U32 reserved_5                     :1 ; /*14:14,                               */
        U32 reserved_6                     :1 ; /*15:15,                               */
        U32 year                           :7 ; /*22:16, Year value loaded to alarm, ranged from 0 to 127. Represent year 2000 to 2127.*/
        U32 reserved_7                     :1 ; /*23:23,                               */
        U32 reserved_8                     :1 ; /*24:24,                               */
        U32 reserved_9                     :1 ; /*25:25,                               */
        U32 reserved_10                    :1 ; /*26:26,                               */
        U32 reserved_11                    :1 ; /*27:27,                               */
        U32 reserved_12                    :1 ; /*28:28,                               */
        U32 reserved_13                    :1 ; /*29:29,                               */
        U32 reserved_14                    :1 ; /*30:30,                               */
        U32 reserved_15                    :1 ; /*31:31,                               */
    } b;
} T_CALENDAR_alarmval_h;

typedef struct
{
    volatile                 T_CALENDAR_ctrl                           ctrl; /*0x0  , RW   , 0x00000000, description                   */
    volatile                  T_CALENDAR_cmd                            cmd; /*0x4  , RW   , 0x80000000, description                   */
    volatile               T_CALENDAR_status                         status; /*0x8  , RO   , 0x00000000, description                   */
    volatile   T_CALENDAR_calendar_loadval_l             calendar_loadval_l; /*0xc  , RW   , 0x00000000, description                   */
    volatile   T_CALENDAR_calendar_loadval_h             calendar_loadval_h; /*0x10 , RW   , 0x00000000, description                   */
    volatile    T_CALENDAR_calendar_curval_l              calendar_curval_l; /*0x14 , RO   , 0x00000000, description                   */
    volatile    T_CALENDAR_calendar_curval_h              calendar_curval_h; /*0x18 , RO   , 0x00000000, description                   */
    volatile           T_CALENDAR_alarmval_l                     alarmval_l; /*0x1c , RW   , 0x00000000, description                   */
    volatile           T_CALENDAR_alarmval_h                     alarmval_h; /*0x20 , RW   , 0x00000000, description                   */
} T_HWP_CALENDAR_T;
#else
typedef struct
{
    volatile                             U32                           ctrl; /*0x0  , RW   , 0x00000000, description                   */
    volatile                             U32                            cmd; /*0x4  , RW   , 0x80000000, description                   */
    volatile                             U32                         status; /*0x8  , RO   , 0x00000000, description                   */
    volatile                             U32             calendar_loadval_l; /*0xc  , RW   , 0x00000000, description                   */
    volatile                             U32             calendar_loadval_h; /*0x10 , RW   , 0x00000000, description                   */
    volatile                             U32              calendar_curval_l; /*0x14 , RO   , 0x00000000, description                   */
    volatile                             U32              calendar_curval_h; /*0x18 , RO   , 0x00000000, description                   */
    volatile                             U32                     alarmval_l; /*0x1c , RW   , 0x00000000, description                   */
    volatile                             U32                     alarmval_h; /*0x20 , RW   , 0x00000000, description                   */
} T_HWP_CALENDAR_T;

//ctrl
#define CALENDAR_INTERVAL(n)           (((n)&0x3)<<0)      
#define CALENDAR_INTERVAL_DISABLE      (((0)&0x3)<<0)      
#define CALENDAR_INTERVAL_PER_SEC      (((1)&0x3)<<0)      
#define CALENDAR_INTERVAL_PER_MIN      (((2)&0x3)<<0)      
#define CALENDAR_INTERVAL_PER_HOUR     (((3)&0x3)<<0)      
#define CALENDAR_INTERVAL_MASK         (0x3<<0)            
#define CALENDAR_INTERVAL_SHIFT        (0)                 


//cmd
#define CALENDAR_CALENDAR_LOAD         (1<<0)              

#define CALENDAR_ALARM_LOAD            (1<<4)              

#define CALENDAR_ALARM_ENABLE_SET      (1<<5)              

#define CALENDAR_ALARM_ENABLE_CLR      (1<<6)              

#define CALENDAR_ALARM_CLR             (1<<8)              

#define CALENDAR_ITV_IRQ_CLR           (1<<9)              

#define CALENDAR_ITV_IRQ_MASK_SET      (1<<16)             

#define CALENDAR_ITV_IRQ_MASK_CLR      (1<<17)             

#define CALENDAR_CALENDAR_NOT_VALID    (1<<31)             


//status
#define CALENDAR_ITV_IRQ_CAUSE         (1<<0)              

#define CALENDAR_ALARM_IRQ_CAUSE       (1<<1)              

#define CALENDAR_FORCE_WAKEUP          (1<<8)              

#define CALENDAR_CHG_MASK              (1<<12)             

#define CALENDAR_ITV_IRQ_STATUS        (1<<16)             

#define CALENDAR_ALARM_ENABLE          (1<<20)             

#define CALENDAR_CALENDAR_NOT_PROG     (1<<31)             


//calendar_loadval_l
#define CALENDAR_SEC(n)                (((n)&0x3F)<<0)     
#define CALENDAR_SEC_MASK              (0x3F<<0)           
#define CALENDAR_SEC_SHIFT             (0)                 

#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_MIN_MASK              (0x3F<<8)           
#define CALENDAR_MIN_SHIFT             (8)                 

#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    
#define CALENDAR_HOUR_MASK             (0x1F<<16)          
#define CALENDAR_HOUR_SHIFT            (16)                


//calendar_loadval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_DAY_MASK              (0x1F<<0)           
#define CALENDAR_DAY_SHIFT             (0)                 

#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_MON_MASK              (0xF<<8)            
#define CALENDAR_MON_SHIFT             (8)                 

#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    
#define CALENDAR_YEAR_MASK             (0x7F<<16)          
#define CALENDAR_YEAR_SHIFT            (16)                

#define CALENDAR_WEEKDAY(n)            (((n)&0x7)<<24)     
#define CALENDAR_WEEKDAY_MASK          (0x7<<24)           
#define CALENDAR_WEEKDAY_SHIFT         (24)                


//calendar_curval_l
#define CALENDAR_SEC(n)                (((n)&0x3F)<<0)     
#define CALENDAR_SEC_MASK              (0x3F<<0)           
#define CALENDAR_SEC_SHIFT             (0)                 

#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_MIN_MASK              (0x3F<<8)           
#define CALENDAR_MIN_SHIFT             (8)                 

#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    
#define CALENDAR_HOUR_MASK             (0x1F<<16)          
#define CALENDAR_HOUR_SHIFT            (16)                


//calendar_curval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_DAY_MASK              (0x1F<<0)           
#define CALENDAR_DAY_SHIFT             (0)                 

#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_MON_MASK              (0xF<<8)            
#define CALENDAR_MON_SHIFT             (8)                 

#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    
#define CALENDAR_YEAR_MASK             (0x7F<<16)          
#define CALENDAR_YEAR_SHIFT            (16)                

#define CALENDAR_WEEKDAY(n)            (((n)&0x7)<<24)     
#define CALENDAR_WEEKDAY_MASK          (0x7<<24)           
#define CALENDAR_WEEKDAY_SHIFT         (24)                


//alarmval_l
#define CALENDAR_SEC(n)                (((n)&0x3F)<<0)     
#define CALENDAR_SEC_MASK              (0x3F<<0)           
#define CALENDAR_SEC_SHIFT             (0)                 

#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_MIN_MASK              (0x3F<<8)           
#define CALENDAR_MIN_SHIFT             (8)                 

#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    
#define CALENDAR_HOUR_MASK             (0x1F<<16)          
#define CALENDAR_HOUR_SHIFT            (16)                


//alarmval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_DAY_MASK              (0x1F<<0)           
#define CALENDAR_DAY_SHIFT             (0)                 

#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_MON_MASK              (0xF<<8)            
#define CALENDAR_MON_SHIFT             (8)                 

#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    
#define CALENDAR_YEAR_MASK             (0x7F<<16)          
#define CALENDAR_YEAR_SHIFT            (16)                


#endif

#endif


