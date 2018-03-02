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
#ifndef _RDABT_SPP_H_
#define _RDABT_SPP_H_

#include "tgt_app_cfg.h"

#include "cs_types.h"
#include "bt.h"
#include "bt_msg.h"
#include "spp.h"
#include "cos.h"
#include "a2dp.h"

typedef struct {
    t_bdaddr addr;
    u_int8 port;
    u_int8 spp_state;
}RDABT_SPP_ADP_INFO;

typedef struct
{
    t_DataBuf *data;
    u_int16 data_length;
    u_int16 spp_port;
}RDABT_SPP_SEND_DATA_STRUCT;


extern void rdabt_spp_connect_ind_cb(void *pArgs);
extern void rdabt_spp_connect_cnf_cb(void *pArgs);
extern void rdabt_spp_data_ind_cb(void *pArgs);
extern void rdabt_spp_data_cnf_cb(void *pArgs);
extern void rdabt_spp_flowctrl_ind_cb(void *pArgs);
extern void rdabt_spp_disconnect_ind_cb(void *pArgs);
extern void rdabt_spp_disconnect_cnf_cb(void *pArgs);
extern void rdabt_spp_msg_handle(u_int16 msg_id, void *msg_data);
extern void rdabt_spp_connect(t_bdaddr addr);
extern void rdabt_spp_free_buf(t_DataBuf *buf);
extern void rdabt_adp_spp_send_data(RDABT_SPP_SEND_DATA_STRUCT *spp_sd_struct);
extern void rdabt_spp_disconnect(u_int16 spp_port);
extern void rdabt_spp_return_adp_info(void);
extern void rdabt_spp_msg_handle(u_int16 msg_id, void *msg_data);

void rdabt_set_support_soundbar(void);
void rdabt_set_local_soundbar_volume(u_int32 volume);
void rdabt_spp_register_services(void);
void rdabt_spp_adp_init(void);
void rdabt_spp_free(void);
void rdabt_spp_free_buf(t_DataBuf *buf);


enum{
    SOUNDBAR_SLAVE = 0,
    SOUNDBAR_MASTER,
    SOUNDBAR_NOROLE,
    SOUNDBAR_NOSUPPORT,
};

enum{
    SOUNDBAR_SBC_FIRST_TRANS = 1,
    SOUNDBAR_SBC_MIN_SIZE,
    SOUNDBAR_SBC_MAX_SIZE,
    SOUNDBAR_SBC_START,
    SOUNDBAR_SBC_STOP
};
typedef struct{
    u_int32 play_flag;
    u_int32 sync_clk;
    bt_a2dp_audio_cap_struct audio_cap;
}SOUNDBAR_CONTROL_DATA_STRUCT;

#define SYS_EVENT 0x01
#define APP_BT_TRC 0x03

#define SPP_ADP_STATE_IDLE                    0
#define SPP_ADP_STATE_PENDING             1
#define SPP_ADP_STATE_CONNECTED         2
extern RDABT_SPP_ADP_INFO spp_adp_info;
#endif

