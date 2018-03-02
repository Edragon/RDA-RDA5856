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
#if APP_SUPPORT_LE==1
#include "ap_gui.h"

#include "bt_types.h"
#include "bt_msg.h"
#include "bt_config.h"
#include "bt_ctrl.h"

#include "ap_mgr.h"
#include "ap_ble.h"
#include "ap_message.h"
#include "mci.h"

#include "gatt.h"
#include "rdabt_gatt.h"

#define ATT_UUID_BAS	0x180F

u_int8 bas_timer;

static const char bas_dn[] = "BAS";
//battery service
static u_int8 hids_battery_uuid[2] = {ATT_UUID_BAS&0xff, ATT_UUID_BAS>>8};

static gatt_chara_def_short_t hids_char_bl = {{ATT_CHARA_PROP_READ|ATT_CHARA_PROP_NOTIFY, 0, 0, ATT_UUID_BATTERY_LEVEL&0xff, ATT_UUID_BATTERY_LEVEL>>8}};
static gatt_chara_def_short_t hids_char_bl2 = {{ATT_CHARA_PROP_READ | ATT_CHARA_PROP_WRITE |ATT_CHARA_PROP_NOTIFY, 0, 0, ATT_UUID_BATTERY_LEVEL&0xff, ATT_UUID_BATTERY_LEVEL>>8}};

static u_int8 hids_bl = 10;
static u_int8 hids_value = 5;

static const gatt_element_t hids_battery[] =
{
    {sizeof(hids_battery_uuid), ATT_PM_READABLE, {ATT_UUID_PRIMARY}, ATT_FMT_SHORT_UUID|ATT_FMT_GROUPED, (void*)hids_battery_uuid, NULL}, //primary service declaration
    {sizeof(hids_char_bl), ATT_PM_READABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&hids_char_bl, NULL},
    {sizeof(hids_bl), ATT_PM_READABLE, {ATT_UUID_BATTERY_LEVEL}, ATT_FMT_WRITE_NOTIFY|ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&hids_bl, NULL},
    {sizeof(hids_char_bl2), ATT_PM_READABLE | ATT_PM_WRITEABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&hids_char_bl2, NULL},
    {sizeof(hids_value), ATT_PM_READABLE | ATT_PM_WRITEABLE, {ATT_UUID_BATTERY_LEVEL}, ATT_FMT_WRITE_NOTIFY|ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&hids_value, NULL},
};

void app_bt_start_bas_server_req(void)
{ 
    gatt_adv_param_t adv;
    
    hal_HstSendEvent(SYS_EVENT, 0x20160622);
    adv.advMin = 0x20;
    adv.advMax = 0x40;
    adv.discov_mode = 2;
    adv.adv_type = 0; 
	adv.noBrEdr = 1;
    adv.enableAdv = 1;

    app_bt_set_adv_data("bas", NULL, NULL);

    app_bt_add_service_req((void*)hids_battery, sizeof(hids_battery)/sizeof(gatt_element_t));

    app_bt_set_visible_req(&adv);
}

void app_gatt_send_notification_test(void *pArgs)
{
    hal_HstSendEvent(SYS_EVENT, 0x11081638);
    hal_HstSendEvent(SYS_EVENT, hids_bl);
    hids_bl++;
    //app_bt_send_notify(acl_handle, 0x2a19, 1, &hids_bl);
    //bas_timer = COS_SetTimer(3000, app_gatt_send_notification_test, NULL, COS_TIMER_MODE_SINGLE);
}

void app_bas_reset(void)
{
    if(bas_timer)
    {
        COS_KillTimer(bas_timer);
        bas_timer = 0;
        hids_bl = 0;
    }
}
#endif
