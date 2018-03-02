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
 *	at_cp.h
 * Author
 * 	ZY,ZXB
 * Date
 * 	2008/04/17
 * Descriptions:
 *	Coolprofile tool stub definition
 *
 *********************************************************/

#ifndef _AT_CP_H
#define _AT_CP_H

#  include "cswtype.h"

// Switch of enable or disable CoolProfile support. 1:enable; 0:diable.
// Hints: A CoolProfile lib must be present when it's enabled!
#define AT_CP_ENABLE 0

// Defines the CP ID range from every module.
typedef enum _AT_CP_ID_T
{
  CPID_UART_START_ = 0x1000,
  CPID_UART_PR, // Polling Rx data function.
  CPID_UART_PT, // Polling Tx data function.
  CPID_UART_END_ = 0x1999,
  CPID_ATE_START_ = 0x2000,
  CPID_ATE_END_ = 0x2999,
  CPID_ATS_START_ = 0x3000,
  CPID_ATS_END_ = 0x3999,
} AT_CP_ID_T;

#  if (AT_CP_ENABLE == 1)
#    define AT_CP_MASK 0x3FFF
#    define AT_CP_EXIT_FLAG 0x8000
extern void hal_PXTS_SendProfilingCode(UINT16 code);

#    define AT_CP_ENTRY(id) hal_PXTS_SendProfilingCode((id) & AT_CP_MASK)
#    define AT_CP_EXIT(id) hal_PXTS_SendProfilingCode(((id) & AT_CP_MASK) | AT_CP_EXIT_FLAG)
#else
#    define AT_CP_ENTRY(id)
#    define AT_CP_EXIT(id)
#endif

#endif
 
