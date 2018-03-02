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

#ifndef AP_SPP_H
#define AP_SPP_H

#include "tgt_app_cfg.h"

#if APP_SUPPORT_SPP ==1
#include "cs_types.h"
#include "bt.h"
#include "bt_msg.h"

#include "spp.h"
#include "sdp.h"
#include "sap.h"
#include "ap_mgr.h"
#include "mci.h"

#define SPP_FLOW_STOP 0
#define SPP_FLOW_GO 1

#define RED_LED 0
#define BLUE_LED 1

#define SOUNDBAR_CONTROL_ONE 1
#define SOUNDBAR_CONTROL_TWO 2


typedef struct {
    t_bdaddr addr;
    u_int8 port;
}RDABT_SPP_ADP_INFO;

typedef struct
{
    t_DataBuf *data_buf;
    u_int16 data_length;
    u_int16 spp_port;
}RDABT_SPP_SEND_DATA_STRUCT;

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
    SOUNDBAR_SBC_STOP,
    SOUNDBAR_SHUTDOWN,
    SOUNDBAR_VOLUME_CHANGE,
    SOUNDBAR_HFP_STATE
};

typedef struct{
    u_int32 play_flag;
    u_int32 sync_clk;
    bt_a2dp_audio_cap_struct audio_cap;
}SOUNDBAR_CONTROL_DATA_STRUCT;


extern RDABT_SPP_SEND_DATA_STRUCT spp_sd;
extern RDABT_SPP_ADP_INFO soundbar_device;
void app_spp_msg_handle(COS_EVENT *ev);
void app_spp_send_data_req(RDABT_SPP_SEND_DATA_STRUCT *spp_sd_struct);
void app_spp_connect_req(t_bdaddr *pAddr);
void app_spp_disconnect_req(u_int16 port);

#if  APP_SUPPORT_SOUNDBAR == 1
void Soundbar_App_Set_Role(u_int8 role);
u_int8 Soundbar_App_Get_Role_Status(void);
int Soundbar_Play_Times(uint8 flag);
void Soundbar_Auto_Connect(void *args);


void app_send_support_soundbar(void);
void app_set_role_led(u_int8 led,u_int8 clr);
void app_send_soundbar_volume(u_int32 vol);
BOOL app_send_soundbar_shutdown(void);
void app_send_soundbar_spp_shutdown(void);  

void app_set_soundbar_last_phone_device(u_int8 phone_last_device);
u_int8 app_get_soundbar_last_phone_device(void);
void app_send_spp_shutdown_complet(void);
void app_send_support_soundbar(void);

void app_send_soundbar_hfp_state(u_int8 state);
void app_connect_soundbar_device(void);
void app_spp_soundbar_receive_start_msg(UINT32 param);
#endif

#endif
#endif

