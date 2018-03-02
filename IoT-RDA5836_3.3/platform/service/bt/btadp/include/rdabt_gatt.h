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
#ifndef RDABT_GATT_H
#define RDABT_GATT_H

#include "platform_config.h"
#include "bt_types.h"
#include "gatt.h"

typedef struct
{
    gatt_uuid_t uuid;
    u_int8 uuid_type;
    u_int16 length;
    u_int16 acl_handle;
    u_int8 *data;
}RDABT_LE_DATA_INFO;

typedef struct
{
    t_bdaddr addr;
    u_int16 acl_handle;
    u_int8 result;
}RDABT_LE_MSG_INFO;

typedef struct
{
    u_int16 offset;
    u_int16 length;
	gatt_element_t *attribute;
	u_int16 valueHandle;
	u_int16 in_use;
}RDABT_LE_WRITE_NOTIFY_INFO;


void rdabt_add_ble_service(adp_gatt_data_msg_t *msg);

#endif
