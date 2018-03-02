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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_boot_asm.h                                                     //
/// That file provides defines used by the assembly boot code.                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_BOOT_MODE_H_
#define _HALP_BOOT_MODE_H_


#define BOOT_MODE_STD                   0

#define BOOT_MODE_NO_AUTO_PU          (1<<0)
#define BOOT_MODE_FORCE_MONITOR       (1<<1)
#define BOOT_MODE_UART_MONITOR_ENABLE (1<<2)
#define BOOT_MODE_USB_MONITOR_DISABLE (1<<3)
#define BOOT_MODE_NAND_FLASH          (1<<4)
#define BOOT_MODE_ADMUX_OR_NAND_PAGE_2K (1<<5)


#endif // _HALP_BOOT_MODE_H_


