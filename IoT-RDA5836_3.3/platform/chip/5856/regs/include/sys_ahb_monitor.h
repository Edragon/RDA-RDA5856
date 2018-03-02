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

#ifndef _SYS_AHB_MONITOR_H_
#define _SYS_AHB_MONITOR_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sys_ahb_monitor'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SYS_AHB_MONITOR_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_SYS_AHB_MONITOR_BASE    0x01A1D000

typedef volatile struct
{
    REG32                          Control;                      //0x00000000
    REG32                          Access_Count;                 //0x00000004
    REG32                          Use;                          //0x00000008
    REG32                          Latency;                      //0x0000000C
} HWP_SYS_AHB_MONITOR_T;

#define hwp_sysAhbMonitor           ((HWP_SYS_AHB_MONITOR_T*) KSEG1(REG_SYS_AHB_MONITOR_BASE))


//Control
#define SYS_AHB_MONITOR_ENABLE      (1<<0)
#define SYS_AHB_MONITOR_RECORD_SINGLE_SHOT (0<<4)
#define SYS_AHB_MONITOR_RECORD_MULTI_SHOT (1<<4)
#define SYS_AHB_MONITOR_ACCUMULATION_MODE_MEAN (0<<5)
#define SYS_AHB_MONITOR_ACCUMULATION_MODE_MAX (1<<5)
#define SYS_AHB_MONITOR_TARGET(n)   (((n)&15)<<8)
#define SYS_AHB_MONITOR_TARGET_MASK (15<<8)
#define SYS_AHB_MONITOR_TARGET_SHIFT (8)
#define SYS_AHB_MONITOR_TARGET_SYS_BUS (0<<8)
#define SYS_AHB_MONITOR_TARGET_SYS_IFC (1<<8)
#define SYS_AHB_MONITOR_TARGET_DMA  (2<<8)
#define SYS_AHB_MONITOR_TARGET_XCPU (3<<8)
#define SYS_AHB_MONITOR_TARGET_SYS_AHB2AHB (4<<8)
#define SYS_AHB_MONITOR_TARGET_SYS_MEM_EXT (5<<8)
#define SYS_AHB_MONITOR_TARGET_SYS_MEM_INT (6<<8)
#define SYS_AHB_MONITOR_TARGET_SYS_AHB2AHB_SLAVE (7<<8)
//#define SYS_AHB_MONITOR_TARGET_SYS_IFC (8<<8)
#define SYS_AHB_MONITOR_TARGET_SIGNAL_SELECT0 (10<<8)
#define SYS_AHB_MONITOR_TARGET_SIGNAL_SELECT1 (11<<8)
#define SYS_AHB_MONITOR_TIME_WINDOW(n) (((n)&7)<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_MASK (7<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_SHIFT (16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_1KC (0<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_32KC (1<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_1MC (2<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_16MC (3<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_1F (4<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_26F (5<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_52F (6<<16)
#define SYS_AHB_MONITOR_TIME_WINDOW_TW_104F (7<<16)

//Access_Count
#define SYS_AHB_MONITOR_ACCESS_COUNT(n) (((n)&0xFFFFFF)<<0)

//Use
#define SYS_AHB_MONITOR_USE(n)      (((n)&0xFFFFFF)<<0)

//Latency
#define SYS_AHB_MONITOR_LATENCY(n)  (((n)&0xFFFFFF)<<0)
#define SYS_AHB_MONITOR_ACCESS_CNT(n) (((n)&0xFF)<<24)





#endif

