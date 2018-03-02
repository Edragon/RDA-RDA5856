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
#ifndef AP_BT_TEST_H
#define AP_BT_TEST_H

#include "bt_types.h"
#include "manager.h"
#include "cs_types.h"
#include "cos.h"
#include "ap_mgr.h"

typedef struct
{
    t_bdaddr addr;
    char device_name[MAX_BT_DEVICE_NAME];
}APP_CHANGE_DEVICE_NAME_INFO;

void ap_test_box_inquiry_start(void);
void ap_test_box_inquiry_cancel(void);
void app_test_box_handle_key(u_int32 key);
void app_test_box_msg_handle(COS_EVENT *ev);

void app_test_box_handle_key(u_int32 key);
void app_test_box_open_a2dp_music_source(void);
void app_test_box_sned_internal_music(UINT32 retry);
u_int8 app_bt_test_func(APP_MGR_DEVICE_INFO_BASE *dbEntry);
void app_test_box_play_next_music( void );
void app_test_box_close_a2dp_music_source(void);





#endif

