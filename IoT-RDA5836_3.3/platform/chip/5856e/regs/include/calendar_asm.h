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
// File name      : calendar_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:45                                                  
// Last modified  : 2016-08-29 14:59:45                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:45 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __CALENDAR_ASM_H__
#define __CALENDAR_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ctrl                       REG_CALENDAR_BASE+0x00
#define REG_cmd                        REG_CALENDAR_BASE+0x04
#define REG_status                     REG_CALENDAR_BASE+0x08
#define REG_calendar_loadval_l         REG_CALENDAR_BASE+0x0c
#define REG_calendar_loadval_h         REG_CALENDAR_BASE+0x10
#define REG_calendar_curval_l          REG_CALENDAR_BASE+0x14
#define REG_calendar_curval_h          REG_CALENDAR_BASE+0x18
#define REG_alarmval_l                 REG_CALENDAR_BASE+0x1c
#define REG_alarmval_h                 REG_CALENDAR_BASE+0x20

//ctrl
#define CALENDAR_INTERVAL(n)           (((n)&0x3)<<0)      

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
#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    

//calendar_loadval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    
#define CALENDAR_WEEKDAY(n)            (((n)&0x7)<<24)     

//calendar_curval_l
#define CALENDAR_SEC(n)                (((n)&0x3F)<<0)     
#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    

//calendar_curval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    
#define CALENDAR_WEEKDAY(n)            (((n)&0x7)<<24)     

//alarmval_l
#define CALENDAR_SEC(n)                (((n)&0x3F)<<0)     
#define CALENDAR_MIN(n)                (((n)&0x3F)<<8)     
#define CALENDAR_HOUR(n)               (((n)&0x1F)<<16)    

//alarmval_h
#define CALENDAR_DAY(n)                (((n)&0x1F)<<0)     
#define CALENDAR_MON(n)                (((n)&0xF)<<8)      
#define CALENDAR_YEAR(n)               (((n)&0x7F)<<16)    


#endif


