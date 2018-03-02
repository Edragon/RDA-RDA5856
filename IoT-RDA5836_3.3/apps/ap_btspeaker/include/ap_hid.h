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

#ifndef AP_HID_H
#define AP_HID_H

#include "tgt_app_cfg.h"

#if APP_SUPPORT_HID ==1
#include "cs_types.h"
#include "bt.h"
#include "bt_msg.h"
#include "cos.h"
#include "hid.h"
#include "sdp.h"
#include "sap.h"
#include "ap_mgr.h"

#define HID_ADP_STATE_IDLE                    0
#define HID_ADP_STATE_PENDING             1
#define HID_ADP_STATE_CONNECTED         2

typedef struct
{
        t_bdaddr addr;
        u_int8 hid_state;
}RDABT_Hid_Info;

typedef struct
{
        t_hid_attributes *p_hid_attributes;
        t_hidOpcode type;
}RDABT_Hid_Sdp_Info;

typedef struct
{
        u_int8 *data;
        u_int8 length;
}RDABT_Hid_Data_Info;

extern void app_hid_handle_key(u_int32 key);
void app_hid_connect_req(t_bdaddr *addr);
void app_hid_register_sdp(void);
void app_hid_disconnect_req(void);
void app_hid_msg_handle(COS_EVENT *ev);
void app_hid_tp_req(void *args);

#endif
#endif

