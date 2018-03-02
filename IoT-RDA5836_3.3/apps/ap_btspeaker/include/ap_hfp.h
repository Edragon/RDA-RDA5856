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

#ifndef AP_HFP_H
#define AP_HFP_H

#include "tgt_app_cfg.h"

#include "bt_types.h"

#if APP_SUPPORT_HFP == 1

#include "bt_msg.h"
#include "ap_mgr.h"
#include "ap_common.h"
#include "ap_message.h"
#include "bt.h"
#include "ap_a2dp.h"

#define MAX_HFP_CONNECT_RETRY 1

#define HFP_MAX_MSG_BUF        32

#define HFP_CALL_STATUS_NONE             0x0000
#define HFP_CALL_STATUS_INCOMING         0x0100
#define HFP_CALL_STATUS_OUTGOING         0x0200
#define HFP_CALL_STATUS_ALERT            0x0300
#define HFP_CALL_STATUS_ACTIVE           0x0001
#define HFP_CALL_STATUS_END              0x0400

#define HFP_CALL_REQ_REJECT     0x09
#define HFP_CALL_REQ_ACCEPT     0x08
#define HFP_CALL_REQ_REDIAL     0x07
#define HFP_CALL_REQ_CALLNUMBER  0x05

#define HFP_ADP_STATE_IDLE                    0
#define HFP_ADP_STATE_PENDING             1
#define HFP_ADP_STATE_CONNECTED         2

#define BT_VOICE_NUMBER_COUNT        10

#define HFP_AUDIO_STATE_ON_MASK     0x80
#define HFP_AUDIO_STATE_OFF_MASK    0x7F

#define PHONE_BOOK_PHONE_BOOK           (0)
#define PHONE_BOOK_INCOMING_HISTORY     (1)
#define PHONE_BOOK_OUTCOMING_HISTORY    (2)
#define PHONE_BOOK_MISSED_CALL          (3)
#define PHONE_BOOK_COMBINED_HISTORY     (4)  

typedef enum {
    HFP_IDLE,
    HFP_CONNECTED,
    HFP_CALL_INCOMING,
    HFP_CALL_OUTGOING,
    HFP_CALL_ACTIVE,
    HFP_PLAY_SCO,
    HFP_VOICE_RECOGNITION,
}app_hfp_state_t;

typedef struct HFP_DEVICE_INFO
{
    t_bdaddr hfp_addr;
    u_int16 hfp_call_status;
    u_int8 hfp_adp_state;
    u_int8 hfp_check_timer;
    u_int8 check_cnt;
}APP_HFP_DEVICE_INFO;

typedef struct
{
    APP_HFP_DEVICE_INFO *hfp_device_info_list;
    u_int8 hfp_max_device_num;
    u_int8 hfp_connect_index;
//used for app    
    u_int16 sco_handle;
    t_bdaddr last_addr;
    t_bdaddr first_addr;
}APP_HFP_ADP_INFO;

typedef struct
{
    t_bdaddr hfp_addr;
    u_int32 msg_param1;
    u_int32 msg_param2;
    u_int8 msg_buf[HFP_MAX_MSG_BUF];
}RDABT_HFP_MSG_INFO;

u_int8 app_get_hfp_state(void);
void app_clean_hfp_adp_info(void);
void app_set_hfp_state(u_int8 state);
void app_start_play_tel_number(u_int8 *ptr_num);
int app_find_hfp_device_state(t_bdaddr addr);
u_int8 app_check_hfp_connection(void);
void app_cancel_create_hfp_connection(t_bdaddr addr);
void app_hfp_play_sco(void);
void app_hfp_stop_sco(void);
void app_hfp_get_adp_info_req(void);
void app_hfp_clean_timer(void);
void app_hfp_bt_state_machine(COS_EVENT *ev);
void app_hfp_check_device_conneciton(void *pArgs);
void app_create_hfp_connection(u_int32 timeoutTenths, t_bdaddr addr);
void app_hfp_call_answer_req(t_bdaddr *pAddr, u_int8 call_status);
void app_hfp_call_req(t_bdaddr *pAddr, u_int8 call_type, u_int8 *phone_number);
void app_hfp_send_raw_data(t_bdaddr *pAddr, u_int8 *data, int data_len);
void app_hfp_get_pbap_req(t_bdaddr *pAddr);
void app_hfp_call_terminal_req(t_bdaddr *pAddr);
APP_HFP_DEVICE_INFO *app_find_hfp_device(t_bdaddr addr);

extern int atoi(const char *s);
extern u_int8 app_hfp_handle_key(u_int32 key);
extern void app_hfp_send_vol(t_bdaddr addr, u_int8 vol);
extern void app_connect_hfp(t_bdaddr *pAddr);
extern void app_disconnect_hfp(t_bdaddr *pAddr);
extern void app_hfp_msg_handle(COS_EVENT *ev);
extern void app_hfp_get_pbap_data_req(t_bdaddr *pAddr, u_int32 start, u_int32 end);
extern APP_HFP_ADP_INFO *app_get_hfp_adp_info(void);
extern RDABT_HFP_MSG_INFO app_msg_info;
extern void app_hfp_switch_mode(void);
APP_HFP_DEVICE_INFO *app_find_active_hfp_device(void);


#endif
#endif

