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

/*********************************************************
 *
 * File Name
 *  at_trace.h
 * Author
 *  Felix
 * Date
 *  2007/11/25
 * Descriptions:
 *  ...
 *
 *********************************************************/

#ifndef AT_TRACE_H
#define AT_TRACE_H

// Debug Index for AT layer, and maybe for CFW, etc.
#if 1//def AT_DEBUG_SUPPORT
#define DEBUG
#else
#undef DEBUG
#endif

// Trace Index for AT entry module
#define g_sw_NONE       0
#define g_sw_SA         1
#define g_sw_ATE        2
#define g_sw_UART       3
#define g_sw_AT_NW      4
#define g_sw_AT_SIM     5
#define g_sw_AT_CC      6
#define g_sw_AT_SMS     7
#define g_sw_AT_SS      8
#define g_sw_GPRS       9
#define g_sw_AT_SAT     10
#define g_sw_WIFI       11
#define g_sw_GC         12
#define g_sw_ID         13
#define g_sw_SPEC       14
#define g_sw_TCPIP      15
#define g_sw_UTIL       16
#define g_sw_DFMS       16

/*
FUNC:
    Intialization for MMI trace function.
**/
#ifdef DEBUG
#define AT_TC_INIT() //AT_OutputText(AT_TS_ID, 1)
#else
#define AT_TC_INIT()
#endif

#define AT_TS_ID 1

#if defined(WIN32)
#define AT_TC  app_trace//trace_null

#elif COMPILER == 2
#define AT_TC  TS_OutputText

#elif defined(DEBUG)

#define AT_TRACE_VERBOSE 0

/*
FUNC:
    Output trace information to UART.
PARAM:
[bSwtich]
    Output switch. TRUE, means output; FALSE, means DO NOT output.
[fmt]
    The trace information that you want to output. Refer the usage of ANSI C printf().
**/
UINT32 AT_OutputText(UINT16 nIndex, PCSTR fmt, ...);
#if (AT_TRACE_VERBOSE == 1)

#define AT_TC(nIndex, fmt, ...) \
    do \
    { \
        if ((nIndex)) \
            AT_OutputText(nIndex, "[AT]["#bSwitch"][%s][%s][%d]>>"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#else
#define AT_TC(nIndex, fmt, ...) \
    do \
    { \
        if ((nIndex)) \
            AT_OutputText(nIndex, "[AT]>>"#fmt, ##__VA_ARGS__); \
    } while (0)
#endif
#else
#define AT_TC(nIndex, fmt, ...)
#endif

/*
Trace一个内存块中的数据.
[bSwitch]
[buffer]
[len]
[format] 16, hexidecimal format; 10, decimal format.
**/
#ifdef DEBUG
extern void AT_TC_MEMBLOCK(UINT16 nIndex, UINT8* buffer, UINT16 len, UINT8 radix);

#define AT_TC_ENTER_FUNC(nIndex) AT_TC(nIndex, "ENTER %s()...", __FUNCTION__)
#define AT_TC_EXIT_FUNC(nIndex) AT_TC(nIndex, "EXIT %s()...", __FUNCTION__)

/*
Because the CSW macro DS_ASSERT is invalid, so at this moment we just do it by ourselvies.

@RUBY STYLE PSEUDOCODE@
trace out the assert expression
sleep for a while to make the idle task print trace information
make system crash
**/
#if defined(WIN32)
#define AT_ASSERT(x)  
#elif defined(CHIP_CPU_IS_ARM)
#define AT_ASSERT(x)
#elif defined(CHIP_CPU_IS_RISCV)
#define AT_ASSERT(x)
#else
#define AT_ASSERT(x) if (!(x)) \
        do { \
            AT_TC(0, "[AT_ASSERT]assert expresion = %s", #x); \
            COS_Sleep(1000); \
            __asm__("break 5"); \
        } while(0)
#endif /* WIN32 */
#else
#define AT_TC_MEMBLOCK(nIndex, buffer, len, radix)
#define AT_TC_ENTER_FUNC(nIndex)
#define AT_TC_EXIT_FUNC(nIndex)
#define AT_ASSERT(x)
#endif

#endif
 
