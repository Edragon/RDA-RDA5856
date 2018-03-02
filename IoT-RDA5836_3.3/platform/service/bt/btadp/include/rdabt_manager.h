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
#ifndef RDABT_MANAGER_H
#define RDABT_MANAGER_H
#include "cs_types.h"
#include "bt.h"
#include "bt_types.h"
#include "bt_msg.h"
#include "manager.h"
#include "rdabt_common.h"

#define SIZE_OF_BDADDR  	6
#define SIZE_OF_LINKKEY 	16
#define SIZE_OF_DHKEY   24

#define MAX_BT_DEVICE_LIST      10
#define MAX_BT_DEVICE_NAME      32


#define MGR_ADP_STATE_IDLE                    0
#define MGR_ADP_STATE_PENDING             1
#define MGR_ADP_STATE_CONNECTED         2

#define MGR_USED                    1
#define MGR_UNUSED                0

#define MGR_DEVACTIVE			0
#define MGR_DEVSNIFF			2
typedef enum {
    BT_CLOSED = 0,
    BT_IDLE,
    BT_SCAN,
    BT_CONNECTED,
    BT_CALL,
    BT_PLAY,
    BT_FILE,
} bt_state_t;


typedef struct
{
    t_bdaddr addr;
    u_int32   cod;
    u_int8 device_name[MAX_BT_DEVICE_NAME];
    u_int8 link_key[SIZE_OF_LINKKEY];
    s_int8 rssi;
    u_int8 adp_state;
    u_int8 acl_state;
    u_int8 key_type;
}RDABT_MGR_DEVICE_INFO;

typedef struct
{
    RDABT_MGR_DEVICE_INFO *inquiry_device_list;
    u_int8 max_inquiry_num;
    u_int8 inquiry_device_count;
    u_int8 inquiry_length;
    u_int8 is_LIAC; //TRUE: limited inquiry, FALSE: General inquiry
}RDABT_ADP_INQUIRY_DATA;

typedef struct
{
    RDABT_MGR_DEVICE_INFO *device_list;
    u_int8 max_device_num;
    u_int8 connect_index;
    u_int8 password[SIZE_OF_LINKKEY];
    t_bdaddr local_addr;
    u_int8 local_name[MAX_BT_DEVICE_NAME];
    u_int8 bt_mode;
    t_bdaddr le_addr;
    u_int32 device_class;
}RDABT_MGR_ADP_INFO;

typedef struct
{
        t_bdaddr device_addr;
        u_int32 msg_param;
}RDABT_MGR_MSG_INFO;

void rdabt_mgr_free(void);
void rdabt_calc_local_key(void);
void rdabt_msg_callback(rdabt_msg_t *message);
void rdabt_calc_local_key(void);
void rdabt_mgr_acl_connect_ind_cb(void *pArgs);
void rdabt_mgr_acl_connect_cnf_cb(void *pArgs);
void rdabt_mgr_acl_disconnect_ind_cb(void *pArgs);
void rdabt_mgr_get_remote_name_cnf_cb(void *pArgs);
void rdabt_mgr_pin_code_ind_cb(void *pArgs);
void rdabt_mgr_link_key_ind_cb(void *pArgs);
void rdabt_mgr_discovery_ind_cb(void *pArgs);
void rdabt_mgr_discovery_cnf_cb(void *pArgs);
void rdabt_mgr_bonding_cnf_cb(void *pArgs);
void rdabt_mgr_clean_linkkey_cb(void *pArgs);
void rdabt_mgr_oob_data_ind_cb(void *pArgs);
void rdabt_mgr_confirm_ind_cb(void *pArgs);
void rdabt_mgr_passkey_ind_cb(void *pArgs);
void rdabt_mgr_localkey_ind_cb(void *pArgs);
void rdabt_mgr_peerkey_ind_cb(void *pArgs);
void rdabt_mgr_dhkey_ind_cb(void *pArgs);
void rdabt_mgr_sco_connect_ind_cb(void *pArgs);
void rdabt_mgr_sco_disconnect_ind_cb(void *pArgs);
void rdabt_mgr_sco_data_ind_cb(void *pArgs);
void rdabt_mgr_register_cnf_cb(void *pArgs);
void rdabt_manager_msg_handle(u_int16 msg_id, void* msg_data);
extern void rdabt_mgr_init(RDABT_MGR_ADP_INFO *info);
extern void rdabt_mgr_sco_connect(t_bdaddr addr);
extern void rdabt_mgr_sco_disconnect(u_int16 sco_handle);
extern void rdabt_removedevice_by_addr(t_bdaddr addr);
extern void rdabt_get_remote_name_req(t_bdaddr addr);
void rdabt_mgr_mode_change_ind_cb(void *pArgs);
void rdabt_get_rssi_by_addr(t_bdaddr addr);
void rdabt_mgr_get_rssi_cnf_cb(void *pArgs);
void rdabt_mgr_return_adp_info(void);
void rdabt_mgr_test(void);
void rdabt_mgr_inquiry_device(RDABT_ADP_INQUIRY_DATA *inquiry_data);
void rdabt_mgr_inquiry_cancel();

extern RDABT_MGR_ADP_INFO mgr_adp_info;
#endif

