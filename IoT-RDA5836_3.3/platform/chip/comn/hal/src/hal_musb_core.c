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

#include "pmd_m.h"

#include "sxr_ops.h"
#include "halp_sys.h"

#include "halp_usb.h"
#include "halp_debug.h"
//#include "halp_version.h"
#include "hal_usb.h"

#include "string.h"

#include "cfg_regs.h"
#include "global_macros.h"
#include "usbc.h"
#include "sys_ctrl.h"
#include "event.h"
#include "cos.h"

#include "musb_core.h"

unsigned char* gUsbRegs = (unsigned char*)0xa1a80000;

void musb_select_ep(unsigned char endnum)
{
	if(endnum < 15)
	{
		musb_writeb( MUSB_INDEX, endnum);
	} 
}

void musb_write_fifo_ram(unsigned char endnum, unsigned char* pSrc, unsigned short len)
{
	/* byte aligned */
	int i = 0;
	void * fifo  = (unsigned char*)gUsbRegs+(endnum*4+0x20);
	
	if ((0x01 & (unsigned long)pSrc) == 0)
	{
		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) pSrc) == 0) 
		{
			if (len >= 4) 
			{
				i = len >>2;
				for( ; i > 0; i--)
				{
					*(unsigned int*)fifo	 = *(unsigned int*)pSrc;
					pSrc+=4;
					len -= 4;
				}
			}
			if (len & 0x02) 
			{
				*(unsigned short*)fifo = *(unsigned short*)pSrc;
				pSrc += 2;
				len-=2;
			}
		}
	} 

	for( i = 0; i<len; i++)
	{
		*(unsigned char*)fifo = *pSrc++;
	}
	
}

void musb_read_fifo_ram(unsigned char endnum, unsigned char *dst,  unsigned short len)
{	
	int i = 0;
	void * fifo  = (unsigned char*)gUsbRegs+(endnum*4+0x20);
	
	if ((0x01 & (unsigned long)dst) == 0)
	{
		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) dst) == 0) 
		{
			if (len >= 4) 
			{
				i = len >>2;
				for( ; i > 0; i--)
				{
					*(unsigned int*)dst = *(unsigned int*)fifo;
					dst+=4;
					len -= 4;
				}
			}
			if (len & 0x02) 
			{
				*(unsigned short*)dst = *(unsigned short*)fifo ;
				dst += 2;
				len-=2;
			}
		}
	} 

	for( i = 0; i<len; i++)
	{
		*dst++ = *(unsigned char*)fifo;
	}
}


