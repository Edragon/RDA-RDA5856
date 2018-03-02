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
#ifndef AP_BLUETOOTH_COMMON_H
#define AP_BLUETOOTH_COMMON_H

#include "bt.h"
#include "tgt_app_cfg.h"
#include "cos.h"
#if APP_SUPPORT_BLUETOOTH==1
#include "rdabt_manager.h"

#define RDABT_RECONNECT_HFP		1
#define RDABT_RECONNECT_A2DP 		2


#define BT_OPS_MUSIC_PLAY					(1 << 0) 		  			
#define BT_OPS_MUSIC_PAUSE				(1 << 1)
#define BT_OPS_MUSIC_PLAY_FAILED			(1 << 2)
#define BT_OPS_FILE_TRANS_ABORT 			(1 << 3)
#define BT_OPS_FILE_TRANS_PAUSE			(1 << 4)
#define BT_OPS_FILE_TRANS_COMPLETE		(1 << 5)
#define BT_OPS_PBAP_PHONEBOOK_REQ 		(1 << 6)
#define BT_OPS_PBAP_HISTORY_REQ 			(1 << 7)
#define BT_OPS_MAP_MSG_REQ 				(1 << 8)	
#define BT_OPS_IS_CONNECTED				(1 << 9)					
#define BT_OPS_STOP_RECV					(1 << 10)	
#define BT_OPS_RECV_PAUSE					(1 << 11)
#define BT_OPS_CLOSED					(1 << 12)
#define BT_OPS_CALL_STATUS_OUTGOING		(1 << 13)
#define BT_OPS_AVRCP_STATUS_ERROR		(1 << 14)
#define BT_OPS_BT_MODE_EXIT				(1 << 15)
#define BT_OPS_AVRCP_TEST				(1 << 16)
#define BT_OPS_RECALL					(1 << 17)
#define BT_OPS_MODULE_CHANGE			(1 << 18)
#define BT_OPS_WAIT_RECALL				(1 << 19)
#define BT_OPS_FIRST_START				(1 << 20)
#define BT_OPS_MASTER					(1 << 21)

#define BT_MODE_CLASSIC            0
#define BT_MODE_LE                      1

enum
{
        BT_KEY_MODE_PRESS,
        BT_KEY_PLAY_PRESS,
        BT_KEY_PLAY_LONG_PRESS,
        BT_KEY_VOL_ADD,
        BT_KEY_VOL_SUB,
        BT_KEY_NEXT,
        BT_KEY_PREV,
};

void rdabt_send_msg_up(u_int32 msg_id, u_int32 param);
void rdabt_set_visiable(u_int8);
void rdabt_active_init(void);
void rdabt_mode_switch(u_int8 mode);
void rdabt_common_init(COS_EVENT *event);
void rdabt_le_addr_init(COS_EVENT *event);
void rdabt_change_local_pincode(COS_EVENT *event);
void rdabt_free_buf(struct st_t_dataBuf *p_buf, u_int8 type);
void rdabt_change_local_name(char *name);
#endif

#endif
