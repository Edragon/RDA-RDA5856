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
#ifndef RDABT_AVRCP_H
#define RDABT_AVRCP_H
#include "tgt_app_cfg.h"

#include "rdabt_common.h"
#include "avrcp.h"

#define AVRCP0_CTYPE_CONTROL			0x00
#define AVRCP0_CTYPE_STATUS		       0x01
#define AVRCP0_CTYPE_NOTIFY                   0x03

#define AVRCP_REGISTER_NOTIFICATION     0x31
#define AVRCP_SET_ABSOLUET_VOLUME      0x50
#define AVRCP_SET_ADDRESSED_PLAYER      0x60
#define AVRCP_SET_BROWSED_PLAYER      0x70
#define AVRCP_GET_FOLDER_ITEMS             0x71
#define AVRCP_CHANGE_PATH                     0x72
#define AVRCP_GET_ITEM_ATTRIBUTES      0x73
#define AVRCP_PLAY_ITEM                           0x74
#define AVRCP_SEARCH                                 0x80
#define AVRCP_ADD_TO_NOW_PLAYING       0x90
#define AVRCP_GENERAL_REJECT       0xa0

#define AVRCP_MAX_MEDIA_ITEM_NUMBER  8
#define AVRCP_MAX_MEDIA_ELEMENT_NUMBER 8

#define AVRCP_USED                    1
#define AVRCP_UNUSED                0

typedef enum
{
    AVRCP_NOTIFY_INTERM = 0,
    AVRCP_NOTIFY_CHANGED,
} t_AVRCP_NOTIFICATION_TYPE;


typedef struct 
{
    u_int8 title[32];
    u_int8 artist[32];
    UINT32 Totle_Song_Len;
    UINT32 Cur_Song_Pos; 
    u_int8 Cur_Play_Status;
}t_avrcp_current_media_info;

typedef struct
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

	   /*
	t_avrcp_current_media_info cur_media_info;
	t_avrcp_folder_items_info folder_info[1];
	t_avrcp_media_element_info media_info;
	t_avrcp_media_playlist_info play_info;*/
}RDABT_AVRCP_DEVICE_INFO;

#define AVRCP_ADP_STATE_IDLE                    0
#define AVRCP_ADP_STATE_PENDING             1
#define AVRCP_ADP_STATE_CONNECTED         2
#define AVRCP_ADP_STATE_PLAY                    3
#define AVRCP_ADP_STATE_PAUSE                  5
#define AVRCP_ADP_STATE_STOP                    6

typedef struct
{
    RDABT_AVRCP_DEVICE_INFO *avrcp_device_info_list;
    u_int8 avrcp_max_device_num;
    u_int8 avrcp_connect_index;
    t_bdaddr last_addr;
    
}RDABT_AVRCP_ADP_INFO;

typedef struct
{
    t_bdaddr avrcp_addr;
    u_int32 msg_param;
    u_int32 msg_param1;
    u_int32 msg_param2;
}RDABT_AVRCP_MSG_INFO;

typedef struct
{
    u_int8 opid;
    u_int8 press;
    u_int8 pending;
    u_int8 param;
    t_bdaddr addr;
}RDABT_AVRCP_KEY_INFO;

void rdabt_avrcp_active_cnf_cb(void *pArgs);
void rdabt_avrcp_deactive_cnf_cb(void *pArgs);
void rdabt_avrcp_connect_cnf_cb(void *pArgs);
void rdabt_avrcp_connect_ind_cb(void *pArgs);
void rdabt_avrcp_disconnect_cnf_cb(void *pArgs);
void rdabt_avrcp_disconnect_ind_cb(void *pArgs);
void rdabt_avrcp_cmd_frame_cnf_cb(void *pArgs);
void rdabt_avrcp_cmd_frame_ind_cb(void *pArgs);
void rdabt_avrcp_player_status_ind_cb(void *pArgs);
void rdabt_avrcp_notification_ind_cb(void *pArgs);
void rdabt_avrcp_vendor_data_ind_cb(void *pArgs);
void rdabt_avrcp_browsing_cmd_ind_cb(void *pArgs);
void rdabt_avrcp_browsing_data_ind_cb(void *pArgs);
void rdabt_avrcp_send_press(u_int8 key);
void rdabt_avrcp_send_release(u_int8 key);
void rdabt_avrcp_register_controller_sdp(void);
void rdabt_avrcp_register_target_sdp(void);
void rdabt_avrcp_init(void);
void rdabt_avrcp_free(void);
void rdabt_avrcp_connect(t_bdaddr addr);
void rdabt_avrcp_disconnect(t_bdaddr addr);
void rdabt_avrcp_disconnect_all(void);
void rdabt_avrcp_send_key(u_int8 key, u_int8 press, u_int8 param, t_bdaddr addr);
void rdabt_avrcp_return_adp_info(void);
void rdabt_avrcp_finish_vendor_data(u_int8 *data_ptr);
void rdabt_avrcp_sync_volume(u_int8 volume);
void rdabt_avrcp_msg_handle(u_int16 msg_id, void* msg_data);



#endif

