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
#include "cswtype.h"
#include "chip_id.h"

#include "stdarg.h"
#include "stdio.h"

#include "sxs_io.h"
#include "sxs_srl.h"
#include "cfw.h"
#include "errorcode.h"
#include "event.h"
#include "ts.h"
#include "sul.h"

#define TS_MAX_ARRAY_SIZE  256
static UINT8 g_IxdArray[TS_MAX_ARRAY_SIZE] = {0, };
static UINT8 g_trace_start = 0;


#define TS_Init_Inner() TS_Open(TRUE)

// Open Trace Module
UINT32 TS_Open(BOOL clearLevels) 
{
    g_trace_start       = 1;
    if (clearLevels)
    {
       // memset(g_IxdArray, 0, TS_MAX_ARRAY_SIZE);
		SUL_ZeroMemory8(g_IxdArray, TS_MAX_ARRAY_SIZE);
    }
    return ERR_SUCCESS;
}

// Close Trace Module
UINT32 TS_Close() 
{
    g_trace_start       = 0;
    return ERR_SUCCESS;
}

// Set enabled trace level
UINT32 TS_SetOutputMask(UINT16 nIndex, BOOL bMode)
{
    if(nIndex >= TS_MAX_ARRAY_SIZE)
        return ERR_NOT_SUPPORT;
    
    g_IxdArray[nIndex] = bMode;
    return ERR_SUCCESS;
}
#if 0
// Output a trace
UINT32 TS_OutputText(UINT16 nIndex, PCSTR fmt, ...)
{
    if(g_trace_start == 0)
        return ERR_NOT_SUPPORT;
    
    if(nIndex >= TS_MAX_ARRAY_SIZE)
        return ERR_NOT_SUPPORT;
    
    //----------------------------------------------------------------------
	//Modify by lixp at 2007-06-28
    //----------------------------------------------------------------------
    //if( (g_IxdArray[(UINT8)nIndex] == 1) )
	if( (g_IxdArray[(UINT8)nIndex] == 1) || (nIndex == 0x00) )
    {
		char uart_buf[256];
		va_list ap;

		va_start (ap, fmt);
		vsprintf(uart_buf, fmt, ap);
		va_end (ap);

        if (nIndex == 0x00)
        {
            // Forced trace
		    SXS_TRACE(_CSW | TSTDOUT,uart_buf);		
        }
        else
        {
            // Regular trace
            SXS_TRACE(_CSW | TSTDOUT,uart_buf);		
		
        }
    }
    return ERR_SUCCESS;
}
#else
UINT32 TS_OutputText(UINT16 nIndex, PCSTR fmt, ...)
{
//    if(g_trace_start == 0)
//        return ERR_NOT_SUPPORT;

#ifndef COMPILE_ERROR_API
    
    if(nIndex > 16)
        return ERR_NOT_SUPPORT;
    
    //----------------------------------------------------------------------
	//Modify by lixp at 2007-06-28
    //----------------------------------------------------------------------
		char uart_buf[256];
		va_list ap;

		va_start (ap, fmt);
		vsnprintf(uart_buf, sizeof(uart_buf), fmt, ap);
		va_end (ap);

        if (nIndex == 0x00)
        {
            // Forced trace
            sxs_fprintf(_CSW | TSTDOUT,uart_buf);
        }
        else
        {
            // Regular trace
            sxs_fprintf(_CSW | TLEVEL(nIndex),uart_buf);
		
        }
     #endif
    return ERR_SUCCESS;
}

#endif


//param radix:	10, output memery using %d
//			  	16, output memery using %x
//		uMask: 	trace ID defined in CSW
VOID CSW_TC_MEMBLOCK(UINT16 uMask, UINT8 *buffer, UINT16 len, UINT8 radix)
{
	INT32 i = 0;
	CHAR s[4] = {0};
	CHAR line_buff[64] = {0}; // Temporary buffer for print trace information.

	CSW_TRACE(uMask, "==================TRACE MEMORY BLOCK=====================>>");

	// Just print part of the data if the data length overrun the temporary buffer size.

	/////////////////////////////////////////////////////////////////////////////
	// the buffer of vsprintf is not more than 256(the max number chars of each line), so 
	// the size of the buffer to need to print is not limited, line_buffer size can be less 
	//than 256 each line and it can be used circularly, modify by wangqunyang 2008.06.02
	/////////////////////////////////////////////////////////////////////////////
	
	for (i = 0; i < len; i++)
	{
		if (radix == 10)
			SUL_StrPrint(s, "%3d ", buffer[i]);
		else if (radix == 16)
			SUL_StrPrint(s, "%02x ", buffer[i]);
		else //default case
			SUL_StrPrint(s, "%02x ", buffer[i]);

		/* we can print 10 chars each line or any others being less than 
		** line_buff size and in this function max size of line_buff is 64 */
		if (i != 0 && i % 10 == 0)
		{
			CSW_TRACE(uMask, "%s", line_buff);
			SUL_MemSet8(line_buff, 0, sizeof(line_buff));
		}

		SUL_StrCat(line_buff, s);
	}

	if (SUL_Strlen(line_buff) > 0)
		CSW_TRACE(uMask, "%s", line_buff);

	CSW_TRACE(uMask, "==================TRACE MEMORY BLOCK=====================<<");
	return;
}

//----------------------------------------------------------------------------
//author: Hameina
// this function called by CFW_PostNotifyEvent to output trace about message send from CSW to MMI.
//--------------------------------------------------------------------------
VOID TS_OutPutEvId(
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSIMID,
#endif
	UINT32 EvID
	)
{
#ifdef CFW_MULTI_SIM	
	CSW_TRACE(CFW_API_TS_ID, "CSW --->MMI, %s, SIM ID:%d\n", TS_GetEventName(EvID),  nSIMID);
#else
	CSW_TRACE(CFW_API_TS_ID, "CSW --->MMI, %s\n", TS_GetEventName(EvID));
#endif
	return;
}

