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


#ifndef _FLASH_H_
#define _FLASH_H_

#include "cs_types.h"

typedef struct
{
	UINT32 start_addr;
	UINT32 size;
}t_flash_block;
typedef struct
{
	UINT32 version;		
	t_flash_block lcd_drv;
	t_flash_block flash_drv;
	t_flash_block patch;
	t_flash_block dsp;
	t_flash_block display;
	t_flash_block resource;
}t_flash_head;

#endif
