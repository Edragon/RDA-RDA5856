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
#ifndef _RDABT_HFP_H_
#define _RDABT_HFP_H_

#include "tgt_app_cfg.h"

#include "rdabt_common.h"
#include "rdabt_manager.h"

#define HFP_MAX_MSG_BUF    32

#define BT_CALL_STATUS_NONE             0x0000
#define BT_CALL_STATUS_INCOMING         0x0100
#define BT_CALL_STATUS_OUTGOING         0x0200
#define BT_CALL_STATUS_ALERT            0x0300
#define BT_CALL_STATUS_ACTIVE           0x0001
#define BT_CALL_STATUS_END              0x0400

#define HFP_ADP_STATE_IDLE                    0
#define HFP_ADP_STATE_PENDING             1
#define HFP_ADP_STATE_CONNECTED         2

#define HFP_CALL_REQ_REJECT     0x09
#define HFP_CALL_REQ_ACCEPT     0x08
#define HFP_CALL_REQ_REDIAL     0x07
#define HFP_CALL_REQ_CALLNUMBER  0x05
#define HFP_USED                    1
#define HFP_UNUSED                0

typedef struct HFP_DEVICE_INFO
{
    t_bdaddr hfp_addr;
    u_int16 hfp_call_status;
    u_int8 hfp_adp_state;
    u_int8 hfp_check_timer;
    u_int8 check_cnt;
}RDABT_HFP_DEVICE_INFO;

typedef struct
{
    RDABT_HFP_DEVICE_INFO *hfp_info_list;
    UINT8 hfp_max_device_num;
    UINT8 hfp_connect_index;
    u_int16 sco_handle;
    t_bdaddr last_addr;
    t_bdaddr first_addr;
}RDABT_HFP_ADP_INFO;

typedef struct
{
        t_bdaddr hfp_addr;
        u_int32 msg_param1;
        u_int32 msg_param2;
        u_int8 msg_buf[HFP_MAX_MSG_BUF];
}RDABT_HFP_MSG_INFO;

void rdabt_hfp_connect_req_cb(void * pArgs);
void rdabt_hfp_connect_ind_cb(void * pArgs);
void rdabt_hfp_disconnect_ind_cb(void * pArgs);
void rdabt_hfp_gain_ind_cb(void * pArgs);
void rdabt_hfp_call_status_ind_cb(void * pArgs);
void rdabt_hfp_call_clcc_cnf_cb(void *pArgs);
void rdabt_hfp_ring_ind_cb(void *pArgs);
void rdabt_hfp_data_ind_cb(void * pArgs);
void rdabt_hfp_call_req(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_pb_get_code_ind_cb(void *pArgs);
void rdabt_hfp_pb_set_code_ind_cb(void *pArgs);
void rdabt_hfp_pb_set_path_ind_cb(void *pArgs);
void rdabt_hfp_pb_ind_cb(void *pArgs);
void rdabt_hfp_get_pb_data_req(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_register_hf_sdp(void);
void rdabt_hfp_register_ag_sdp(void);
extern void rdabt_hfp_update_indicator(void *pArgs);
u_int8 rdabt_hfp_send_NREC(t_bdaddr addr);
extern RDABT_HFP_ADP_INFO rdabt_hfp_info;
RDABT_HFP_DEVICE_INFO *rdabt_get_hfp_device_info(t_bdaddr addr);
void rdabt_hfp_init(void);
void rdabt_hfp_free(void);
UINT8 rdabt_connect_hfp(t_bdaddr addr);
void rdabt_disconnect_hfp(t_bdaddr addr);
void rdabt_hfp_disconnect_all(void);
void rdabt_hfp_return_adp_info(void);
void rdabt_hfp_call_answer_req(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_terminal_call_req(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_send_raw_data(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_get_pbap_req(RDABT_HFP_MSG_INFO *msg_info);
void rdabt_hfp_send_speaker_gain(RDABT_HFP_MSG_INFO *msg_info);
int HF_Activate_Voice_Recognition(t_bdaddr *address);
void rdabt_hfp_msg_handle(u_int16 msg_id, void* msg_data);
int HF_Change_Device_Name(t_bdaddr *address, char *name);
u_int8 rdabt_connect_hfp_hs(t_bdaddr addr);


#endif
