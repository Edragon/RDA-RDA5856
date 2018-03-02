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

#ifndef AP_MAP_H
#define AP_MAP_H
#include "tgt_app_cfg.h"
#if APP_SUPPORT_MAP==1
#include "cs_types.h"
#include "bt_msg.h"
#include "ap_mgr.h"
#include "ap_message.h"
#include "manager.h"

#define RDABT_MAP_STATUS_IDLE				(1)
#define RDABT_MAP_STATUS_CONNECT_PENDING	(2)
#define RDABT_MAP_STATUS_CONNECTED			(3)

#define RDABT_MSG_BUF_SIZE		(1024)
#define MAP_LENGTH	(20)
#define MAP_GET_SIZE	(1)
#define MSG_BUFF_LEN  (MAP_LENGTH*16)

typedef struct
{
        u_int32 map_start;
        u_int32 map_end;
        u_int8 map_type;
        t_bdaddr map_addr;
}Rdabt_map_msg;

int find_str(const char *src_str, const char *spec_str, int str_len);
int UTF8_to_Unicode(char *dst, const char *src);
void app_get_map_data(u_int32 start, u_int32 end);
void app_get_map_size(t_bdaddr * pAddr);
void app_map_msg_handle(COS_EVENT *ev);
int rdabt_map_deal_msglist_data(u_int8 *result_data);
#endif
#endif

