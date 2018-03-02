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
/*
================================================================================
  File       : SXR_CSP.H
--------------------------------------------------------------------------------

  Scope      : CPU defines

  History    :
--------------------------------------------------------------------------------
  Jun 12 03  | ADA   | Creation
================================================================================
*/
#ifndef __SXR_CSP_H__
#define  __SXR_CSP_H__

#include "chip_id.h"

#ifndef SXR_CSP_TK_STCK_HEAP
#error "Location for task stack heap must be defined"
#endif
#ifndef SXR_CSP_JB_STCK_HEAP
#error "Location for job stack heap must be defined"
#endif
#ifndef SXR_CSP_JB_CTX_HEAP
#error "Location for job context heap must be defined"
#endif

#include "hal_mem_map.h"
#include "hal_trace.h"
#include "hal_timers.h"
#include "hal_sys.h"
#include "hal_sxr.h"

#define SXS_BOOT_DIAG_AREA HAL_HEAP_START(0)

#define sxr_Reset(a)        hal_SysRestart() 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define sxr_EnterSc()       hal_SysEnterCriticalSection()
#define sxr_ExitSc(a)       hal_SysExitCriticalSection(a)
#else
#define sxr_EnterSc()       hal_SysEnterSC()
#define sxr_ExitSc(a)       hal_SysExitSC(a)
#endif

#define SXS_GET_RA(a) HAL_SYS_GET_RA(a)
#define SXS_GET_SP(a) a=hal_SysGetSp()

#define pal_WDogCntRst()            hal_TimWatchDogKeepAlive()
#define pal_SoftAbort()             hal_SoftAbort()
#define pal_SoftBreak()             asm volatile("break 2\n"            \
"nop\n"                                                                 \
"nop\n"                                                                 \
"nop\n")

#define pal_SetIrqSpy(IrqId,Start)  
#define pal_SetTaskSpy(TskNb)    //   hal_DbgPxtsProfileTask(HAL_DBG_PXTS_SX, TskNb)

#define PAL_NB_RAM_AREA HAL_NB_RAM_AREA
#define PAL_HEAP_START(area)    HAL_HEAP_START(area)
#define PAL_HEAP_SIZE(area)     HAL_HEAP_SIZE(area)
#define PAL_HEAP_END(area)      HAL_HEAP_END(area)
#define PAL_IS_ROM(addr)        HAL_IS_CODE(addr)

#define PAL_FRAMED_TIMER 1
#define PAL_REGULAR_TIMER 0
#define SECOND                  * HAL_TICK1S
#define pal_TimerSetCnt         hal_TimSetExpirationTime
#define pal_TimerGetCnt         hal_TimGetReminderTime
#define pal_TickInit()          hal_TimTickOpen
#define pal_TimDelay			hal_TimDelay

#define PAL_NB_MAX_SERIAL 1
//#define PAL_NB_MAX_SERIAL HAL_UART_QTY
#define __SXR_SRL_F__
#define pal_OpenSerial(a)       
#define pal_CloseSerial         hal_HstTraceClose
#define pal_SetSerialCfg        hal_HstTraceOpen
#define pal_Write               hal_HstTraceSendData
#define pal_Read                hal_HstTraceGetData
#define pal_SleepSerial         hal_HstTraceDisable
#define pal_WakeUpSerial        hal_HstTraceEnable

// low level transfer functions for flow control handled by sx - not implemented in this PAL
#define pal_SetDsr(a,b)     
#define pal_SetCts(a,b)
#define pal_SetRing(a,b)
#define pal_SetDcd(a,b)


#define PAL_IS_TX_EMPTY(PortIdx) 
#define PAL_TX_FIFO_SIZE(PortIdx) 
#define PAL_WRITEDATA(Data, PortIdx) 
#define PAL_TX_ENABLED(PortIdx)    TRUE
#define PAL_ENABLE_RX(PortIdx)      
#define PAL_DISABLE_RX(PortIdx)     
#define PAL_RX_FIFO_SIZE(PortIdx)   1

#define pal_GetModemStatus(a) 0
#define PAL_ECHO_ON  1
#define PAL_ECHO_OFF 0
#define pal_SetEcho( _t, _s ) FALSE

#endif

