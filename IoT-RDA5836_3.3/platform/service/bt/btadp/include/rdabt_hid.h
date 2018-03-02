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
#ifndef _RDABT_HID_H_
#define _RDABT_HID_H_

#include "tgt_app_cfg.h"

#include "cs_types.h"
#include "bt.h"
#include "bt_msg.h"
#include "spp.h"
#include "cos.h"
#include "hid.h"

typedef struct
{
    t_bdaddr addr;
    u_int8 hid_state;
}RDABT_Hid_Info;

typedef struct
{
        u_int8 *data;
        u_int8 length;
}RDABT_Hid_Data_Info;
typedef struct
{
        t_hid_attributes *p_hid_attributes;
        t_hidOpcode type;
}RDABT_Hid_Sdp_Info;

#define SYS_EVENT 0x01
#define APP_BT_TRC 0x03

#define HID_ADP_STATE_IDLE                    0
#define HID_ADP_STATE_PENDING             1
#define HID_ADP_STATE_CONNECTED         2

void rdabt_hid_connect_ind_cb(void *pArgs);
void rdabt_hid_connect_cnf_cb(void *pArgs);
void rdabt_hid_disconnect_ind_cb(void *pArgs);
void rdabt_hid_disconnect_cnf_cb(void *pArgs);
void rdabt_hid_data_ind_cb(void *pArgs);
void rdabt_hid_connect_req(t_bdaddr *addr);
void rdabt_hid_disconnect_req(void);

#endif
