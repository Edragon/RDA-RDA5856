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
#ifndef AP_BLE_H
#define AP_BLE_H

#include "tgt_app_cfg.h"

#include "bt_types.h"
#include "bt_msg.h"
#include "bt_config.h"
#include "bt_ctrl.h"

#include "ap_mgr.h"
#include "ap_message.h"

#include "gatt.h"
#include "rdabt_gatt.h"

typedef struct
{
    gatt_uuid_t	uuid;
	u_int8 uuid_type;
    u_int16 length;
    u_int16 acl_handle;
    u_int8 *data;
}APP_BLE_DATA_INFO;

void app_bt_send_notify(u_int16 acl_handle, u_int16 uuid, u_int16 length, u_int8 *data);
void app_bt_send_notify_luuid(u_int16 acl_handle, u_int8 * uuid, u_int16 length, u_int8 *data);
void app_bt_send_indication(u_int16 acl_handle, u_int16 uuid, u_int16 length, u_int8 *data);
void app_bt_set_adv_data(u_int8* name, u_int8* manufacture, u_int8* advData);
void app_bt_add_service_req(void *service, int size);
void app_bt_set_visible_req(void *adv);

#endif

