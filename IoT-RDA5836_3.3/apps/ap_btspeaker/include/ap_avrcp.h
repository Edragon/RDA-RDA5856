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
#ifndef AP_AVRCP_H
#define AP_AVRCP_H
#include "tgt_app_cfg.h"

#include "bt_types.h"
#if APP_SUPPORT_AVRCP==1
#include "bt_msg.h"
#include "ap_common.h"
#include "ap_mgr.h"
#include "ap_message.h"
#include "rdabt_avrcp.h"
#include "avrcp.h"

#define AVRCP_STATE_STOP		0
#define AVRCP_STATE_PLAYING	1
#define AVRCP_STATE_PAUSE		2

#define AVRCP_ADP_STATE_IDLE                    0
#define AVRCP_ADP_STATE_PENDING             1
#define AVRCP_ADP_STATE_CONNECTED         2
#define AVRCP_ADP_STATE_PLAY                    3
#define AVRCP_ADP_STATE_PAUSE                  5
#define AVRCP_ADP_STATE_STOP                    6

#define AVRCP_NO_PRESS                  0
#define AVRCP_PRESS                     1
#define AVRCP_RELEASE                   2

typedef struct APP_AVRCP_DEVICE
{
    u_int8 Cur_Play_Status;
    u_int32 Cur_Song_Pos;	
    u_int8 numberofevents;
    u_int8 Registerd_numberofevents;
    u_int8 supported_events_id[13];
    u_int8 numberofattribures;
    u_int8 attribures_index;
    u_int8 supported_attributes_type[4];
    u_int8 supported_attributes_value_type[4][4];
    u_int8 UID_Counter;
    u_int16 avrcp_cid;
    u_int16 avrcp_browse_cid;
    u_int8 NumberofItems;
    u_int8 folder_depth;
    u_int8 avrcp_adp_state;
    t_bdaddr avrcp_addr;    
    u_int8 Cur_Play_pause_key_status;
}APP_AVRCP_DEVICE_INFO;

typedef struct
{
	APP_AVRCP_DEVICE_INFO *avrcp_device_info_list;
	u_int8 avrcp_max_device_num;
    u_int8 avrcp_connect_index;
    t_bdaddr last_addr;
}APP_AVRCP_ADP_INFO;

typedef struct
{
    t_bdaddr avrcp_addr;
    u_int32 msg_param;
    u_int32 msg_param1;
    u_int32 msg_param2;
}APP_AVRCP_MSG_INFO;

typedef struct
{
    u_int8 opid;
    u_int8 press;
    u_int8 pending;
    u_int8 param;
    t_bdaddr addr;
}APP_AVRCP_KEY_INFO;

u_int8 app_check_avrcp_connection(void);
void app_clean_avrcp_adp_info(void);
void app_avrcp_send_key(u_int8 key, u_int8 press, u_int8 param, t_bdaddr addr);
void app_avrcp_get_adp_info_req(void);
void app_avrcp_handle_key(u_int32 key);
void app_avrcp_switch_mode(void);
BOOL app_check_avrcp_active(t_bdaddr addr, APP_AVRCP_DEVICE_INFO** device_info);

APP_AVRCP_ADP_INFO *app_get_avrcp_adp_info(void);
APP_AVRCP_DEVICE_INFO *app_find_avrcp_device(t_bdaddr addr);
APP_AVRCP_DEVICE_INFO *app_find_active_avrcp_device(void);

extern BOOL no_play_inter();
extern UINT8 g_now_is_play_connect_int_voice;
extern INT32 g_current_module;
extern UINT8 g_cur_volume;
extern void app_connect_avrcp(t_bdaddr *pAddr);
extern void app_disconnect_avrcp(t_bdaddr *pAddr);
extern void app_avrcp_msg_handle(COS_EVENT *ev);
void app_avrcp_send_press(u_int8 key);
void app_avrcp_send_release(u_int8 key);


#endif
#endif

