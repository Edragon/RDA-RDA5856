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


#ifndef __AHB_MONITOR_H__
#define __AHB_MONITOR_H__
//Auto-gen by reg_gen
#include "cs_types.h"

#define REG_AHB_MONITOR_BASE 0x0

typedef volatile struct
{
    REG32 AHB_Monitor_Ctrl              ; //0x00    
    REG32 AHB_Monitor_Count             ; //0x04    
    REG32 AHB_Monitor_Use               ; //0x08    
    REG32 AHB_Monitor_Latency           ; //0x0c    
} HWP_AHB_MONITOR_T;

#define hwp_ahb_monitor ((HWP_AHB_MONITOR_T*)KSEG1(REG_AHB_MONITOR_BASE))

//AHB_Monitor_Ctrl
#define AHB_MONITOR_ENABLE             (1<<0)              
#define AHB_MONITOR_RECORD             (1<<4)              
#define AHB_MONITOR_ACCUMULATION_MODE  (1<<5)              
#define AHB_MONITOR_TARGET(n)          (((n)&0xF)<<8)      
#define AHB_MONITOR_TIME_WINDOW(n)     (((n)&0x7)<<16)     

//AHB_Monitor_Count
#define AHB_MONITOR_ACCESS_COUNT(n)    (((n)&0xFFFFFF)<<0) 

//AHB_Monitor_Use
#define AHB_MONITOR_USE(n)             (((n)&0xFFFFFF)<<0) 

//AHB_Monitor_Latency
#define AHB_MONITOR_LATENCY(n)         (((n)&0xFFFFFF)<<0) 
#define AHB_MONITOR_ACCESS_CNT(n)      (((n)&0xFF)<<24)    


#endif


