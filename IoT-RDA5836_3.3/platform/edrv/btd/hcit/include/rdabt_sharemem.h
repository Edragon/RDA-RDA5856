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
//       Define the interface of bt_cpu and xcpu by share memory                                                                     //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




#ifndef _RDABT_SHARE_MEMORY_H_
#define _RDABT_SHARE_MEMORY_H_

#include "cos.h"

void rdabt_sharemem_init();

void rdabt_send_msg_to_bt(COS_EVENT *event);
void rdabt_send_msg_to_xcpu(COS_EVENT *event);

BOOL rdabt_get_bt_msg(COS_EVENT *event);
BOOL rdabt_get_xcpu_msg(COS_EVENT *event);

void rdabt_wait_xcpu_busy();
BOOL rdabt_wait_btcpu_busy();
void rdabt_clear_btcpu_busy();
void rdabt_clear_xcpu_busy();


#endif //_RDABT_SHARE_MEMORY_H_

