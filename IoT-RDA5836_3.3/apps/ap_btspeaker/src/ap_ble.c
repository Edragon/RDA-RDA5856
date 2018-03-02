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
#include "ap_ble.h"

#if APP_SUPPORT_LE==1

static adp_gatt_data_msg_t add_service_msg;
static gatt_adv_data_t adv_msg;
APP_BLE_DATA_INFO ap_ble_info;
extern APP_BT_MANAGER *app_bt_vars;

#if APP_SUPPORT_BLE_BR==1
extern UINT8 mmi_timer_create(UINT32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags);
void app_switch_br_le_mode(void *pArgs);
#endif

void app_bt_send_notify(u_int16 acl_handle, u_int16 uuid, u_int16 length, u_int8 *data)
{
    COS_EVENT event = {0};

    ap_ble_info.data = data;
    ap_ble_info.length = length;
    ap_ble_info.uuid.uuid_s = uuid;
	ap_ble_info.uuid_type = 0;
    ap_ble_info.acl_handle = acl_handle;
    
    event.nEventId = RDABT_GATT_SEND_NOTIFY_REQ;
    event.nParam1 = &ap_ble_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_send_indication(u_int16 acl_handle, u_int16 uuid, u_int16 length, u_int8 *data)
{
    COS_EVENT event = {0};

    ap_ble_info.data = data;
    ap_ble_info.length = length;
    ap_ble_info.uuid.uuid_s = uuid;
	ap_ble_info.uuid_type = 0;
    ap_ble_info.acl_handle = acl_handle;
    
    event.nEventId = RDABT_GATT_SEND_INDICATE_REQ;
    event.nParam1 = &ap_ble_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_send_notify_luuid(u_int16 acl_handle, u_int8 * uuid, u_int16 length, u_int8 *data)
{
    COS_EVENT event = {0};

    ap_ble_info.data = data;
    ap_ble_info.length = length;
    ap_ble_info.uuid.uuid_l = uuid;
	ap_ble_info.uuid_type = 1;
    ap_ble_info.acl_handle = acl_handle;
    
    event.nEventId = RDABT_GATT_SEND_NOTIFY_REQ;
    event.nParam1 = &ap_ble_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_set_adv_data(u_int8* name, u_int8* manufacture, u_int8* advData)
{
    COS_EVENT event = {0};
    int index = 0;
    int len = 0;

    u_int8 *adv=adv_msg.advData;
    
    memset(&adv_msg, 0, sizeof(adv_msg));
    if(advData != NULL)
    {
        adv_msg.advLen = 31;
        memcpy(adv, advData, 31);
    }
    else
    {
        if(name != NULL)
        {
            len = strlen(name);
            if(len > 8)
            {
                len = 8;
            }
            if(len > 0)
            {
                adv[index++] = len+1;
                adv[index++] = 0x09; //GAP_ADTYPE_LOCAL_NAME_COMPLETE;
                memcpy(adv+index, name, len);
                index += len;
            }
        }
        
        if(manufacture != NULL)
        {
            len = strlen(manufacture);
            if(len > 19)
            {
                len = 19;
            }
            if(len > 0)
            {
                adv[index++] = len+1;
                adv[index++] = 0xff; //GAP_ADTYPE_MANUFACTURER_SPECIFIC;
                memcpy(adv+index, manufacture, len);
                index += len;
            }
        }
        adv_msg.advLen = index;
    }
    hal_HstSendEvent(SYS_EVENT, 0x17061902);
    hal_HstSendEvent(SYS_EVENT, &adv_msg);
    event.nEventId = RDABT_GATT_SET_ADV_DATA_REQ;
    event.nParam1 = &adv_msg;
    COS_SendEvent(MOD_BT, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


void app_bt_add_service_req(void *service, int size)
{
    COS_EVENT ev;
    add_service_msg.service = (gatt_element_t *)service;
    add_service_msg.service_size = size;

    ev.nEventId = RDABT_GATT_ADD_SERVICE_REQ;
    ev.nParam1 = (UINT32)&add_service_msg;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_set_visible_req(void *adv)
{
    COS_EVENT ev = {0};
    static gatt_adv_param_t adv_param_msg;

    hal_HstSendEvent(SYS_EVENT, 0x17062130);
    if(adv == NULL)
    {
        adv_param_msg.advMin = 0x80;
        adv_param_msg.advMax = 0xa0;
        adv_param_msg.discov_mode = 2;
        adv_param_msg.adv_type = 0; 
        adv_param_msg.noBrEdr = 1;
        adv_param_msg.enableAdv = 1;
    }
    else
    {
        memcpy(&adv_param_msg, adv, sizeof(gatt_adv_param_t));
    }
    
#if APP_SUPPORT_BLE_BR==1
    wakelock(10);
#endif
    ev.nEventId = RDABT_GATT_SET_VISIBLE_REQ;
    ev.nParam1 = (UINT32)&adv_param_msg;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

}

t_api app_gatt_process_msg(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case RDABT_GATT_CONNECT_IND_MSG:
        {
            RDABT_LE_MSG_INFO *pMsg = (RDABT_LE_MSG_INFO*)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x11081416);
            hal_HstSendEvent(SYS_EVENT, pMsg->acl_handle);
            hal_HstSendEvent(SYS_EVENT, pMsg->result);
            if(app_bt_vars->bt_mode != HOST_LE_MODE)
            {
                hal_HstSendEvent(SYS_EVENT, 0x05231732);
                break;
            }
            
#if APP_SUPPORT_BLE_BR==1
            if(app_bt_vars->switch_timer_handle)
            {
                mmi_cancel_timer(app_bt_vars->switch_timer_handle);
                app_bt_vars->switch_timer_handle = 0;
            }
            //hal_SysRequestFreq(20, 104000000, NULL);
#endif
        }
        break;

        case RDABT_GATT_DISCONNECT_IND_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x11081535);
            COS_SetTimer(500, app_bt_set_visible_req, NULL, COS_TIMER_MODE_SINGLE);
#if APP_SUPPORT_BLE_BR==1
            app_bt_vars->switch_timer_handle = mmi_timer_create(25, app_switch_br_le_mode, NULL, COS_TIMER_MODE_SINGLE);
#endif
        }
        break;

        case RDABT_GATT_WRITE_NOTIFY_MSG:
        {
            gatt_srv_write_notify_t *pAttr = (gatt_srv_write_notify_t *)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0xface0001);
            if(pAttr)
            {
                if(pAttr->attribute->cb)
                {
                    pAttr->attribute->cb((void*)pAttr);
                }
            }
        }
        break;

        case RDABT_GATT_INDICATE_CONF_MSG:
        {
            gatt_element_t *pElement = (gatt_element_t *)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0xface0002);
            if(pElement)
            {
                if(pElement->cb)
                {
                    pElement->cb(pElement);
                }
            }
        }
        break;
        
        default:
        break;
    }
    return 0;
}

#endif
