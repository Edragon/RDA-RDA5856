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

          
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'calendar'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CALENDAR_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_CALENDAR_BASE           0x01A0A000

typedef volatile struct
{
    REG32                          ctrl;                         //0x00000000
    REG32                          cmd;                          //0x00000004
    REG32                          status;                       //0x00000008
    REG32                          calendar_loadval_l;           //0x0000000C
    REG32                          calendar_loadval_h;           //0x00000010
    REG32                          calendar_curval_l;            //0x00000014
    REG32                          calendar_curval_h;            //0x00000018
    REG32                          alarmval_l;                   //0x0000001C
    REG32                          alarmval_h;                   //0x00000020
} HWP_CALENDAR_T;

#define hwp_calendar                ((HWP_CALENDAR_T*) KSEG1(REG_CALENDAR_BASE))


//Ctrl
#define CALENDAR_INTERVAL(n)        (((n)&3)<<0)
#define CALENDAR_INTERVAL_DISABLE   (0<<0)
#define CALENDAR_INTERVAL_PER_SEC   (1<<0)
#define CALENDAR_INTERVAL_PER_MIN   (2<<0)
#define CALENDAR_INTERVAL_PER_HOUR  (3<<0)

//Cmd
#define CALENDAR_CALENDAR_LOAD      (1<<0)
#define CALENDAR_ALARM_LOAD         (1<<4)
#define CALENDAR_ALARM_ENABLE_SET   (1<<5)
#define CALENDAR_ALARM_ENABLE_CLR   (1<<6)
#define CALENDAR_ALARM_CLR          (1<<8)
#define CALENDAR_ITV_IRQ_CLR        (1<<9)
#define CALENDAR_ITV_IRQ_MASK_SET   (1<<16)
#define CALENDAR_ITV_IRQ_MASK_CLR   (1<<17)
#define CALENDAR_CALENDAR_NOT_VALID (1<<31)

//Status
#define CALENDAR_ITV_IRQ_CAUSE      (1<<0)
#define CALENDAR_ALARM_IRQ_CAUSE    (1<<1)
#define CALENDAR_FORCE_WAKEUP       (1<<8)
#define CALENDAR_CHG_MASK           (1<<12)
#define CALENDAR_ITV_IRQ_STATUS     (1<<16)
#define CALENDAR_ALARM_ENABLE       (1<<20)
#define CALENDAR_CALENDAR_NOT_PROG  (1<<31)

//Calendar_LoadVal_L
#define CALENDAR_SEC(n)             (((n)&0x3F)<<0)
#define CALENDAR_MIN(n)             (((n)&0x3F)<<8)
#define CALENDAR_HOUR(n)            (((n)&31)<<16)

//Calendar_LoadVal_H
#define CALENDAR_DAY(n)             (((n)&31)<<0)
#define CALENDAR_MON(n)             (((n)&15)<<8)
#define CALENDAR_YEAR(n)            (((n)&0x7F)<<16)
#define CALENDAR_WEEKDAY(n)         (((n)&7)<<24)

//Calendar_CurVal_L
//#define CALENDAR_SEC(n)           (((n)&0x3F)<<0)
#define CALENDAR_SEC_MASK           (0x3F<<0)
#define CALENDAR_SEC_SHIFT          (0)
//#define CALENDAR_MIN(n)           (((n)&0x3F)<<8)
#define CALENDAR_MIN_MASK           (0x3F<<8)
#define CALENDAR_MIN_SHIFT          (8)
//#define CALENDAR_HOUR(n)          (((n)&31)<<16)
#define CALENDAR_HOUR_MASK          (31<<16)
#define CALENDAR_HOUR_SHIFT         (16)

//Calendar_CurVal_H
//#define CALENDAR_DAY(n)           (((n)&31)<<0)
#define CALENDAR_DAY_MASK           (31<<0)
#define CALENDAR_DAY_SHIFT          (0)
//#define CALENDAR_MON(n)           (((n)&15)<<8)
#define CALENDAR_MON_MASK           (15<<8)
#define CALENDAR_MON_SHIFT          (8)
//#define CALENDAR_YEAR(n)          (((n)&0x7F)<<16)
#define CALENDAR_YEAR_MASK          (0x7F<<16)
#define CALENDAR_YEAR_SHIFT         (16)
//#define CALENDAR_WEEKDAY(n)       (((n)&7)<<24)
#define CALENDAR_WEEKDAY_MASK       (7<<24)
#define CALENDAR_WEEKDAY_SHIFT      (24)

//AlarmVal_L
//#define CALENDAR_SEC(n)           (((n)&0x3F)<<0)
//#define CALENDAR_MIN(n)           (((n)&0x3F)<<8)
//#define CALENDAR_HOUR(n)          (((n)&31)<<16)

//AlarmVal_H
//#define CALENDAR_DAY(n)           (((n)&31)<<0)
//#define CALENDAR_MON(n)           (((n)&15)<<8)
//#define CALENDAR_YEAR(n)          (((n)&0x7F)<<16)





#endif

