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



#include "cos.h"
#include "cs_types.h"
#include "chip_id.h"
#include "portable.h"
/******************************************
 * 5996 memory map:
 * top memory address : 0x180000
 * top 4k for IRQ stack
 * following 8k for SVC stack
 *****************************************/
void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions );

HeapRegion_t pxHeapRegions[] ={
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5991H)
    {(unsigned char*)0x00180000, 64*1024},
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
    //{(unsigned char*)0x00010000, 30*1024},
   // {(unsigned char*)0x00018000, 30*1024},
   {(unsigned char*)0x00038300, 30*1024},
#else
    {(unsigned char*)0x00100000, 30*1024},
    {(unsigned char*)0x0016f000, 60*1024},
#endif
    {NULL, 0},
};

VOID COS_MemInit()
{
    vPortDefineHeapRegions(pxHeapRegions);
}

PVOID COS_Malloc(UINT32 size)
{
    return pvPortMalloc(size);
}

PVOID COS_Realloc(VOID *ptr, UINT32 nSize)
{
	VOID *p;
	p = pvPortMalloc(nSize);
	if(p && ptr)
	{
		memcpy(p, ptr, nSize);
		vPortFree(ptr);
	}
	return p;
}

PVOID SRVAPI COS_Malloc_NoCache ( UINT32 size)
{
    return COS_Malloc(size);
}

BOOL COS_Free(PVOID ptr)
{
    vPortFree(ptr);
    return TRUE;
}


