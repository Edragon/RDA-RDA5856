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
#ifndef RDABT_PBAP_H
#define RDABT_PBAP_H

#include "tgt_app_cfg.h"

#include "rdabt_common.h"
#include "obex.h"
#include "bt.h"
#include "papi.h"

#define RDABT_PBAP_STATUS_IDLE				(0)
#define RDABT_PBAP_STATUS_CONNECT_PENDING	(2)
#define RDABT_PBAP_STATUS_CONNECTED			(3)

#define RDABT_PBAP_UNUSE_LIST					(0)
#define RDABT_PBAP_USE_LIST					(1)
#define PB_LENGTH                     20

#define PHONEBOOK_SEARCH_PINYIN_MAXLEN (1)
#define PHONEBOOK_NUMBER_COUNT      (3)
#define MAX_CHECK_STR_LEN (3)

#define CALL_UNKNOW	3
#define CALL_MISSED       2
#define CALL_RECEIVED     1
#define CALL_DIALED       0

#define PBAP_PHONEBOOK_TYPE	1
#define PBAP_HISTORY_TYPE		2	

typedef struct
{
	u_int32 pb_cid;
	u_int32 pb_tid;
	u_int32 phonebook_step;
	u_int32 phonebook_size;	
	u_int32 history_step;
	u_int32 history_size;
	u_int16 pb_index;
	u_int16 pb_get_pos;
	u_int8 *pbap_buf;
}Rdabt_pbap;

typedef struct
{
        u_int32 pb_start;
        u_int32 pb_end;
        u_int8 pb_type;
        t_bdaddr pb_addr;
}Rdabt_pbap_msg;

void rdabt_pbap_cli_rf_data_ind_cb(void *pArgs);
void rdabt_pbap_disconnect();
void rdabt_pbap_init(void);
void rdabt_pbap_free(void);
u_int8 rdabt_pbap_download(Rdabt_pbap_msg* pb_msg);
void rdabt_pbap_get_data(Rdabt_pbap_msg* pb_msg);   
void rdabt_pbap_disconnect(void);
void rdabt_pbap_msg_handle(u_int16 msg_id, void* msg_data);

#endif

