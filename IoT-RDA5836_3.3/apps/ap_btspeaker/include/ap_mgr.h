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

#include "tgt_app_cfg.h"

#ifndef AP_BT_COMMON_H
#define AP_BT_COMMON_H

#if APP_SUPPORT_BLUETOOTH==1
#include "ap_common.h"
#include "ap_message.h"
#include "bt_types.h"
#include "bt_msg.h"

//#include "mci.h"
#include "manager.h"
#include "rdabt_manager.h"

#include "ap_pbap.h"
#include "ap_map.h"
#include "ap_hfp.h"
#include "ap_spp.h"
#include "ap_a2dp.h"
#include "ap_hid.h"
#include "MainTask.h"

#include "tgt_m.h"
#include "aud_main.h"
#include "hal_host.h"

#define MAGIC_BLUETOOTH      0x180e

#define SIZE_OF_BDADDR  	6
#define SIZE_OF_LINKKEY 	16
#define SIZE_OF_DHKEY   24

#define MAX_BT_DEVICE_LIST      10
#define MAX_BT_DEVICE_NAME      32

#define RDABT_RECONNECT_HFP		1
#define RDABT_RECONNECT_A2DP 		2

#define HOST_BR_MODE	0
#define HOST_LE_MODE	1

#ifndef APP_SUPPORT_SOUNDBAR
#define APP_SUPPORT_SOUNDBAR 0
#endif

#ifndef APP_SUPPORT_BLE_BR
#define APP_SUPPORT_BLE_BR 0
#endif

#ifndef APP_SUPPORT_ONLY_ONE_LINK
#define APP_SUPPORT_ONLY_ONE_LINK 0
#endif

#ifndef APP_SUPPORT_VOICE_RECOGNITION
#define APP_SUPPORT_VOICE_RECOGNITION 0
#endif

#define BT_VOLUME_MAX           AUD_MAX_LEVEL
#define BT_VOLUME_DEFAULT       7

typedef enum {
    MGR_CLOSED,
    MGR_CLOSE_PENDING,    
    MGR_ACTIVE_PENDING,    
    MGR_ACTIVED,
    MGR_SCAN,
    MGR_INQUIRY_PENDNG,
    MGR_CONNECTED,
}app_mgr_state_t;

typedef enum {
    MGR_SET_NO_VISIABLE  = 0x00,
    MGR_SET_CONNECTABLE  = 0x01,    
    MGR_SET_DISCOVERABLE = 0x10,    
    MGR_SET_ALL_VISIABLE = 0x11,
    MGR_SET_LIMITEDDISCOVERY_NOCONNECTABLE = 0x20,
    MGR_SET_LIMITEDDISCOVERY_CONNECTABLE = 0x21,
}app_visiable_state;


typedef struct
{
    t_bdaddr addr;
    u_int32 cod;
    u_int8 device_name[MAX_BT_DEVICE_NAME];
    u_int8 link_key[SIZE_OF_LINKKEY];
    u_int8 key_type;
}APP_MGR_DEVICE_INFO_FLASH;

typedef struct
{
    t_bdaddr addr;
    u_int32 cod;
    u_int8 device_name[MAX_BT_DEVICE_NAME];
    u_int8 link_key[SIZE_OF_LINKKEY];
    s_int8 rssi;
    u_int8 adp_state;
    u_int8 acl_state;
    u_int8 key_type;
}APP_MGR_DEVICE_INFO_BASE;

#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
typedef struct
{
    APP_MGR_DEVICE_INFO_BASE *inquiry_device_list;
    u_int8 max_inquiry_num;
    u_int8 inquiry_device_count;
    u_int8 inquiry_length;
    u_int8 inquiry_device_need_getName_count;
    u_int8 app_inquiry_timer;
    u_int8 is_LIAC;
}APP_INQUIRY_DATA;
#else
typedef struct
{
    APP_MGR_DEVICE_INFO_BASE *inquiry_device_list;
    u_int8 max_inquiry_num;
    u_int8 inquiry_device_count;
    u_int8 inquiry_length;
    u_int8 is_LIAC;
}APP_INQUIRY_DATA;

#endif

typedef struct
{
    u_int16 magic;
    u_int8 last_device;
    t_bdaddr local_addr;
    u_int8 local_name[MAX_BT_DEVICE_NAME];
	APP_MGR_DEVICE_INFO_FLASH device_list[MAX_BT_DEVICE_LIST];
    u_int8 soundbar_default_role;
    t_bdaddr soundbar_connect_addr;
#if APP_SUPPORT_HID_MODE==1    
    u_int8 last_hid_device;
#endif
	u_int8 last_device_two;
#if APP_SUPPORT_HID == 1 &&APP_SUPPORT_HID_MODE ==0
	u_int8 not_support_hid_service;
#endif
}APP_BT_FLASH_STORE;

typedef struct
{
//do not modify these 
	APP_MGR_DEVICE_INFO_BASE *device_list;
	u_int8 max_device_num;
	u_int8 connect_index;
	u_int8 password[SIZE_OF_LINKKEY];
	t_bdaddr local_addr;
	u_int8 local_name[MAX_BT_DEVICE_NAME];
	u_int8 bt_mode;
    t_bdaddr le_addr;
    u_int32 device_class;
////////////////////////////////////
//used for app    
	int8 volume;
    u_int8 last_device;
    u_int16 magic;
	u_int8 old_device;
    u_int16 times_num;          //reconnect start
    u_int16 times_value;
    u_int8 connect_retry_cnt;
	u_int8 auto_connect_timer;  //reconnect end
    eq_t eq_type;
#if APP_SUPPORT_BLE_BR==1
    u_int8 switch_timer_handle;
#endif  
#if APP_SUPPORT_HID_MODE==1 
    u_int8 last_hid_device;
    u_int8 old_hid_device;
#endif
	u_int8 last_device_two;
#if APP_SUPPORT_HID == 1&&APP_SUPPORT_HID_MODE ==0
	u_int8 not_support_hid_service;
#endif
    int8 volume_hfp;
    int8 volume_a2dp;
}APP_BT_MANAGER;

typedef struct
{
        t_bdaddr device_addr;
        u_int32 msg_param;
}APP_MGR_MSG_INFO;

typedef struct
{
    u_int32 key;
    u_int32 param;
}APP_KEY_MSG_INFO;

typedef struct
{
    u_int32 key;
    u_int32 param;
	t_bdaddr addr;
}APP_AVRCP_KEY_MSG_INFO;

#if APP_SUPPORT_FCC==1

typedef struct
{
    u_int32 param;
    u_int8  hop_frq;
    u_int8 power_level;
    u_int8 tx_rx;
}FCC_TEST_PARAM;

extern FCC_TEST_PARAM fcc_param;

#endif

#define BT_INQUIRY_DEVICE   13501

typedef VOID (*APP_BT_GET_RSSI_HANDLER_T)(signed int rssi);
int find_str(
			 const char *src_str, 
			 const char *spec_str, 
			 int str_len 
			 );
u_int8 app_get_mgr_state(void);
void app_bt_msg_handle(COS_EVENT *ev);
void app_bt_change_name_req(char *name_ptr);
void app_bt_inquiry_cancel_req(void);
void app_mgr_deal_discover_result_func(APP_MGR_DEVICE_INFO_BASE *dbEntry);
void app_bt_active_req(void);
void app_bt_close_req(void);
void app_set_bt_connectable_req(u_int8 is_connectable);
void app_set_bt_visible_req(app_visiable_state is_visiable);
void app_bt_mode_switch(u_int8 mode);
void app_bt_inquiry_req(u_int8 max_result, u_int8 is_LIAC);
void app_bt_bonding_req(t_bdaddr *pAddr);
void app_sco_connect_req(t_bdaddr *pAddr);
void app_sco_disconnect_req(u_int16 sco_handle);
void app_mgr_bonding_cnf_cb(APP_MGR_DEVICE_INFO_BASE *device);
void app_bt_connect_retry(void *pArgs);
void app_bt_connect_second_device(void *pArgs);
BOOL app_bt_main_msg_handle(COS_EVENT *pEvent);
void app_mgr_store(void);
BOOL app_switch_mode_handle(UINT32 nextModule);
void app_set_bt_intercept_switch_mode_result(u_int32 next_mode);
u_int32 app_get_bt_intercept_switch_mode_result(void);
void app_bt_close_complet(void);
void app_exit_timer_check(void);
void app_mgr_PlayInternalAudio(u_int8 audio_ID);
void app_bt_connect(int count);
void app_cancel_reconnect(void);
void app_bt_get_rssi_req2(APP_BT_GET_RSSI_HANDLER_T cb, t_bdaddr *pAddr);
void app_bt_set_address(t_bdaddr* addr);
u_int8 *app_bt_get_address(void);
void app_bt_set_local_name(u_int8 *name);
u_int8 *app_bt_get_local_name(void);
u_int8 app_find_bt_device(t_bdaddr addr);

#if APP_SUPPORT_FCC == 1
void app_fcc_test_stop(void);
void app_fcc_test(u_int32 param, u_int8  hop_frq, u_int8 power_level, u_int8 tx_rx);
#endif
#if APP_SUPPORT_DUT==1
void app_bt_dut_test_req(void);
#endif
#if APP_SUPPORT_VCO==1
void app_bt_vco_test_req(void);
#endif
void app_bt_connect_second_device(void *pArgs);
extern int rand();
extern BOOL boot_get_testmode(void);
extern u_int8 tgt_GetBtMaxDeviceNum(void);
extern u_int8 tgt_GetBtDefaultMode(void);
extern void hal_DbgAssert(const char* format, ...);
extern void MMC_AudioCleanA2dpData(void);
extern void rdabt_send_msg_to_bt(COS_EVENT *event);
extern BOOL bal_is_first_keydown();

extern btconfig_t g_btconfig;
extern APP_BT_MANAGER *app_bt_vars;
#endif

void app_bt_entry(void);

#endif

