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
#include "stdio.h"
#ifndef MCU_WIFI_PLAT
#if OS_KERNEL != FREERTOS
#include "sxs_io.h"
extern UINT32 HAL_UNCACHED_BSS sxs_trace_buf[SXS_TRACE_BUFF_SIZE/4];
extern INT32 trace_pos;
#endif
/*
 * Maybe the correct way to manage log, better than sxs_vprintf
 */
int vprintf( const char* fmt, va_list ap)
{
    //va_list args;
    //int i=0;
    int out_buf;
    //va_start(args,fmt);

#ifdef GSM_SUPPORT

    memset(sxs_trace_buf, 0, sizeof(sxs_trace_buf));
    out_buf = vsnprintf(sxs_trace_buf, sizeof(sxs_trace_buf), fmt, ap);

  #if (CHIP_HAS_BTCPU == 1)  
    rdabt_wait_btcpu_busy();  
  #endif
  
    hal_HstSendEvent(0xff,0x72ace000 + out_buf);

    while(sxs_trace_buf[i]!=0)
    {
        hal_HstSendEvent(0xff,sxs_trace_buf[i++]);
    }

  #if (CHIP_HAS_BTCPU == 1)
    rdabt_clear_xcpu_busy();
  #endif
  
#else /* #ifdef GSM_SUPPORT */
#ifdef CHIP_CPU_IS_MIPS
    if(trace_pos<0)
        return 0;

    out_buf = vsnprintf((char*)(sxs_trace_buf+trace_pos), SXS_TRACE_BUFF_SIZE-trace_pos*4, fmt, ap);

    if(out_buf <=0 || trace_pos*4 + out_buf +2 >= SXS_TRACE_BUFF_SIZE)
    {
        trace_pos = 0;
        out_buf = vsnprintf((char*)sxs_trace_buf, SXS_TRACE_BUFF_SIZE, fmt, ap);
    }
    
    //hal_HstSendEvent(0xff,0x73ace000 + out_buf);
    //hal_HstSendEvent(0xff,Id);
    //hal_HstSendEvent(0xff,sxs_trace_buf+trace_pos);

    trace_pos += (out_buf+3)/4;
#endif
#endif
     //va_end(args);    return out_buf;
}
#endif
