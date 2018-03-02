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
 *	at_trace.c
 * Author
 * 	Felix
 * Date
 * 	2008/01/07
 * Descriptions:
 *	Defines some trace switches and functions for debug trace purpose.
 *
 *********************************************************/

#include "at_common.h"
#include "errorcode.h"
#include "sxs_io.h"

/*
Below are the trace switches. You can change it's value to control the output of any kind
of trace information.
**/



#if !defined(WIN32)
UINT32 AT_OutputText(UINT16 nIndex, PCSTR fmt, ...)
{
    va_list args;

    if(nIndex > 16)
        return ERR_NOT_SUPPORT;

    va_start(args,fmt);
    sxs_vprintf(_APP|TLEVEL(nIndex), fmt, args);
    va_end(args);
    
    return ERR_SUCCESS;
}
#endif /* WIN32 */


#ifdef DEBUG
void AT_TC_MEMBLOCK(UINT16 nIndex, UINT8* buffer, UINT16 len, UINT8 radix)
{
  INT32 i            = 0;
  CHAR s[4]          = { 0 };
  CHAR line_buff[64] = { 0 }; // Temporary buffer for print trace information.

  AT_TC(nIndex, "==================TRACE MEMORY BLOCK=====================>>");

  // Just print part of the data if the data length overrun the temporary buffer size.

  // ///////////////////////////////////////////////////////////////////////////
  // the buffer of vsprintf is not more than 256(the max number chars of each line), so 
  // the size of the buffer to need to print is not limited, line_buffer size can be less 
  // than 256 each line and it can be used circularly, modify by wangqunyang 2008.06.02
  // ///////////////////////////////////////////////////////////////////////////
  /* 
     if (len > sizeof(line_buff))
     len = sizeof(line_buff);
   */

  for (i = 0; i < len; i++)
  {
    if (radix == 10)
      AT_Sprintf(s, "%3d ", buffer[i]);
    else if (radix == 16)
      AT_Sprintf(s, "%02x ", buffer[i]);
    else  // default case
      AT_Sprintf(s, "%02x ", buffer[i]);

    /* we can print 10 chars each line or any others being less than 
       ** line_buff size and in this function max size of line_buff is 64 */
    if (i != 0 && i % 10 == 0)
    {
      AT_TC(nIndex, "%s", line_buff);
      memset(line_buff, 0, sizeof(line_buff));
    }

    strcat(line_buff, s);
  }

  if (strlen(line_buff) > 0)
    AT_TC(nIndex, "%s", line_buff);

  AT_TC(nIndex, "==================TRACE MEMORY BLOCK=====================<<");
}
#endif
 
