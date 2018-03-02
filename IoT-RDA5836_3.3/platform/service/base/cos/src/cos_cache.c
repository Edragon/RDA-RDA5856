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



#include <csw.h>
#include <stdkey.h>
#include <base_prv.h>
#include <ts.h>
#include<sxr_tksd.h>
#include<sxr_tim.h>
#include <event.h>
#include "cos.h"
#include "csw_csp.h"

void  COS_CleanDCache() 
{
#ifdef CHIP_CPU_IS_MIPS
	asm volatile(".set noreorder");
	asm volatile(".align 4");
	asm volatile("cache 2,0");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile(".set reorder");
#endif
}
void  COS_CleanICache()
{
#ifdef CHIP_CPU_IS_MIPS
	asm volatile(".set noreorder");
	asm volatile(".align 4");
	asm volatile("cache 1,0");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile(".set reorder");
#endif
}
void  COS_CleanALLCach()
{
#ifdef CHIP_CPU_IS_MIPS
	asm volatile(".set noreorder");
	asm volatile(".align 4");
	asm volatile("cache 0,0");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile(".set reorder");
#endif
}

