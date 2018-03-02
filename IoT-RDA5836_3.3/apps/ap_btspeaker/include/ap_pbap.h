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

#ifndef AP_PBAP_H
#define AP_PBAP_H
#include "tgt_app_cfg.h"
#if APP_SUPPORT_PBAP==1
#include "bt_types.h"
#include "bt_msg.h"
#include "ap_mgr.h"
#include "ap_message.h"
#include "manager.h"

#define MAX_PINYIN_LEN (6)
#define MAX_PINYIN_COUNT (4)
#define PB_LENGTH                     20
#define CALL_UNKNOW	3
#define CALL_MISSED       2
#define CALL_RECEIVED     1
#define CALL_DIALED       0
#define TOTAL_CHINESE_WORD_COUNT (6766)

#define PBAP_PHONEBOOK_TYPE	1
#define PBAP_HISTORY_TYPE		2

typedef struct
{
        u_int32 pb_start;
        u_int32 pb_end;
        u_int8 pb_type;
        t_bdaddr pb_addr;
}Rdabt_pbap_msg;

void app_pbap_deal_pb_data(u_int8 *data);
void app_get_pbap_data(u_int8 type, u_int32 start, u_int32 end);
void app_get_pbap_size(t_bdaddr * pAddr);
int UTF8_to_Unicode(char *dst, const char *src);
void app_pbap_msg_handle(COS_EVENT *ev);
void app_get_call_recends_size(t_bdaddr *pAddr);

#endif
#endif

