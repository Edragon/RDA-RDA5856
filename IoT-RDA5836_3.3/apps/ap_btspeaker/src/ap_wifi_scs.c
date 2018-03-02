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
#ifdef CODEC_SUPPORT
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

static int configState = 0;
static int apInfoLen = 0;

#define ATT_UUID_WIFI	        0xFEE7
#define ATT_UUID_WIFI_APINFO    0xFEC7
#define ATT_UUID_WIFI_STATE     0xFEC8
#define ATT_UUID_WIFI_INFO      0xFEC9

#define ATT_APINFO_MAX_SIZE     128
#define ATT_WIFI_STATE_SIZE     20
#define ATT_WIFI_INFO_SIZE      30
#define ATT_WIFI_ADV_SIZE       31

void stringTohex(u_int8 *destValue, const char* src, int stringLen)
{
    int i = 0;
    int j = 0;

	for(; i<stringLen; i+=2, j++)
	{
		if(src[i] >= '0' && src[i] <= '9')
			destValue[j] = (src[i] - 0x30) << 4;
		else if(src[i] >= 'A' && src[i] <= 'F')
			destValue[j] = (src[i] - 0x37) << 4;
		else if(src[i] >= 'a' && src[i] <= 'f')
			destValue[j] = (src[i] - 0x57) << 4;

		if(src[1+i] >= '0' && src[1+i] <= '9')
			destValue[j] |= src[1+i]-0x30;
		else if(src[1+i] >= 'A' && src[1+i] <= 'F')
			destValue[j] |= src[1+i]-0x37;
		else if(src[1+i] >= 'a' && src[1+i] <= 'f')
			destValue[j] |= src[1+i]-0x57;
	}
}

void hexToString(char *destValue, const u_int8* src, int hexLen)
{
    int i = 0; 
    int j = 0;
    int temp;
    for(; i<hexLen; i++,j+=2)
    {
        temp = (src[i]&0xf0)>>4;
        if(temp >=0 && temp <=9)
        {
            destValue[j] = 0x30+temp;
        }
        else
        {
            destValue[j] = temp+0x37;
        }
        
        temp = (src[i]&0x0f);
        if(temp >=0 && temp <=9)
        {
            destValue[j+1] = 0x30+temp;
        }
        else
        {
            destValue[j+1] = temp+0x37;
        }
    }

}


static u_int8 wifi_service_uuid[2] = {ATT_UUID_WIFI&0xff, ATT_UUID_WIFI>>8};

// characteristics
static gatt_chara_def_short_t wifi_char_apinfo = {{ATT_CHARA_PROP_WRITE, 0, 0, ATT_UUID_WIFI_APINFO&0xff, ATT_UUID_WIFI_APINFO>>8}};
static gatt_chara_def_short_t wifi_char_state = {{ATT_CHARA_PROP_INDICATE, 0, 0, ATT_UUID_WIFI_STATE&0xff, ATT_UUID_WIFI_STATE>>8}};
static gatt_chara_def_short_t wifi_char_info = {{ATT_CHARA_PROP_READ, 0, 0, ATT_UUID_WIFI_INFO&0xff, ATT_UUID_WIFI_INFO>>8}};

static gatt_chara_ccb_t wifi_state_ccb;

// characteristics value
static u_int8 wifi_ap_info[ATT_APINFO_MAX_SIZE] = {0};
static u_int8 wifi_state[ATT_WIFI_STATE_SIZE] = {0};
static u_int8 wifi_info[ATT_WIFI_INFO_SIZE] = {0};
static u_int8 wifi_adv[31] = {0};
static u_int8 wifi_ind_state = 0;
static gatt_srv_write_notify_t *g_pApinfoAttr;


static void app_bt_wifiscs_apinfo_cb(void *param)
{
    gatt_srv_write_notify_t *pAttr = (gatt_srv_write_notify_t *)param;
    g_pApinfoAttr = pAttr;
    apInfoLen = (u_int16)pAttr->valueLen;
    configState = 1;
    hal_HstSendEvent(SYS_EVENT, 0x2017059b);
	hal_HstSendEvent(SYS_EVENT, apInfoLen);
    hal_HstSendEvent(SYS_EVENT, wifi_ap_info);
}

static void app_bt_wifiscs_ind_confirmation_cb(void *param)
{
    hal_HstSendEvent(SYS_EVENT, 0x17061422);
    hal_HstSendEvent(SYS_EVENT, wifi_state_ccb.configurationBits);

    wifi_ind_state = 1;
}

static void app_bt_wifiscs_enable_ind_cb(void *param)
{
    hal_HstSendEvent(SYS_EVENT, 0x17061420);
    hal_HstSendEvent(SYS_EVENT, wifi_state_ccb.configurationBits);
    if(wifi_state_ccb.configurationBits > 0)
    {
        wifi_ind_state = 1;
    }
    else
    {
        wifi_ind_state = 0;
    }
}


static const gatt_element_t wifi_5981[] =
{
	// primary service declaration
	{sizeof(wifi_service_uuid), ATT_PM_READABLE, {ATT_UUID_PRIMARY}, ATT_FMT_SHORT_UUID|ATT_FMT_GROUPED|ATT_FMT_FIXED_LENGTH, (void*)wifi_service_uuid, NULL},

    // characteristic: wifi ap info
	{sizeof(wifi_char_apinfo), ATT_PM_READABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&wifi_char_apinfo, NULL},
	{sizeof(wifi_ap_info), ATT_PM_WRITEABLE, {ATT_UUID_WIFI_APINFO}, ATT_FMT_SHORT_UUID|ATT_FMT_WRITE_NOTIFY, (void*)wifi_ap_info, app_bt_wifiscs_apinfo_cb},

    // characteristic: wifi features info
	{sizeof(wifi_char_info), ATT_PM_READABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&wifi_char_info, NULL},
	{sizeof(wifi_info), ATT_PM_READABLE, {ATT_UUID_WIFI_INFO}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)wifi_info, NULL},

    // characteristic: wifi state
	{sizeof(wifi_char_state), ATT_PM_READABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&wifi_char_state, NULL},
	{sizeof(wifi_state), 0, {ATT_UUID_WIFI_STATE}, ATT_FMT_SHORT_UUID|ATT_FMT_CONFIRM_NOTITY, (void*)wifi_state, app_bt_wifiscs_ind_confirmation_cb},
    {sizeof(wifi_state_ccb), ATT_PM_READABLE|ATT_PM_WRITEABLE, {ATT_UUID_CLIENT}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH|ATT_FMT_WRITE_NOTIFY, (void*)&wifi_state_ccb, app_bt_wifiscs_enable_ind_cb},
};


void app_bt_start_wifiscs_server_req(u_int8 *adv)
{
    int len = 0;

    hal_HstSendEvent(SYS_EVENT, 0x20170501);

    configState = 0;
    memset(wifi_info, 0, sizeof(wifi_info));
    memset(wifi_state, 0, sizeof(wifi_state));
    memset(wifi_ap_info, 0, sizeof(wifi_ap_info));

    if(adv != NULL)
    {
    	len = strlen(adv);
		len = len>>1;
		len = len > ATT_WIFI_ADV_SIZE ? ATT_WIFI_ADV_SIZE:len;

		stringTohex(wifi_adv, adv, len<<1);
    }
}

void app_bt_start_wifiscs()
{
    hal_HstSendEvent(SYS_EVENT, 0x20170502);
    hal_HstSendEvent(SYS_EVENT, wifi_info);
    hal_HstSendEvent(SYS_EVENT, wifi_ap_info);

    configState = 0;
    memset(wifi_state, 0, sizeof(wifi_state));
    memset(wifi_ap_info, 0, sizeof(wifi_ap_info));

    app_bt_set_adv_data(NULL, NULL, wifi_adv);
    app_bt_set_visible_req(NULL);

    app_bt_add_service_req((void*)wifi_5981, sizeof(wifi_5981)/sizeof(gatt_element_t));
}

void app_bt_disconnect_wifiscs()
{
    configState = 0;
    wifi_ind_state = 0;
    memset(wifi_state, 0, sizeof(wifi_state));
    memset(wifi_ap_info, 0, sizeof(wifi_ap_info));    
}

int app_bt_wifiscs_get_configState(void)
{
	return configState;
}

void app_bt_wifiscs_get_apinfo(u_int8 *str)
{
    configState = 0;
    hexToString(str, wifi_ap_info, apInfoLen);
}

int app_bt_wifiscs_get_ind_state(void)
{
    return wifi_ind_state;
}

void app_bt_wifiscs_set_feature(u_int8* str)
{
    if(str != NULL)
    {
        int len = 0;
        memset(wifi_info, 0, sizeof(wifi_info));
        len = strlen(str);
        len = len>>1;
        len = len > ATT_WIFI_INFO_SIZE ? ATT_WIFI_INFO_SIZE:len;
        stringTohex(wifi_info, str, len<<1);
    }
}

void app_bt_wifiscs_set_state(u_int8 *str)
{
    int len = 0;
    u_int16 aclHandle = 0;
    hal_HstSendEvent(SYS_EVENT, 0x17060902);
    hal_HstSendEvent(SYS_EVENT, wifi_state);

    if(wifi_ind_state == 1)
    {
        memset(wifi_state, 0, sizeof(wifi_state));
        len = strlen(str);
        len = (len>>1);
        len = len > ATT_WIFI_STATE_SIZE ? ATT_WIFI_STATE_SIZE:len;
 
        stringTohex(wifi_state, str, len<<1);
        wifi_ind_state = 2;
        aclHandle = wifi_state_ccb.aclHandle;
        app_bt_send_indication(aclHandle, ATT_UUID_WIFI_STATE, len, wifi_state);
    }
}

void app_bt_wifiscs_clear_apinfo()
{
    hal_HstSendEvent(SYS_EVENT, 0x17062602);
    if(g_pApinfoAttr != NULL)
    {
        g_pApinfoAttr->valueLen = 0;
    }
}

#endif
#endif

