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
#ifndef RDABT_MAP_H
#define RDABT_MAP_H

#include "rdabt_common.h"

#define RDABT_MAP_STATUS_IDLE				(1)
#define RDABT_MAP_STATUS_CONNECT_PENDING	(2)
#define RDABT_MAP_STATUS_CONNECTED			(3)

#define RDABT_MSG_BUF_SIZE		(1024)
#define MAP_LENGTH	(20)
#define MAP_GET_SIZE	(1)
#define MSG_BUFF_LEN  (MAP_LENGTH*16)

typedef struct
{
        u_int32 mas_cid;
        u_int32 mas_tid;
        u_int32 mns_cid;
        u_int32 mns_tid;
        u_int32 mns_step;
        u_int32 mas_step;
        u_int32 mas_size;	
        u_int16 mas_index;
        u_int16 mas_get_pos;
        u_int8 *mas_buf;
        u_int8 *mns_buf;
}Rdabt_map;

typedef struct
{
        u_int32 map_start;
        u_int32 map_end;
        u_int8 map_type;
        t_bdaddr map_addr;
}Rdabt_map_msg;

void rdabt_map_ser_auth_ind_cb(void *pArgs);
void rdabt_map_ser_rec_data_ind_cb(void *pArgs);
void rdabt_map_cli_data_ind_cb(void *pArgs);
void rdabt_map_init(void);
#endif

