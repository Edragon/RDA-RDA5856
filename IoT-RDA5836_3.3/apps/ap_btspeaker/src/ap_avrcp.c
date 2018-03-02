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

#include "ap_avrcp.h"
#if APP_SUPPORT_AVRCP==1

#include "ap_media.h"

static APP_AVRCP_ADP_INFO *s_avrcp_adp_info = NULL;
static u_int8 *avrcp_vender_ptr = NULL;
static APP_AVRCP_KEY_INFO avrcp_key;
static t_bdaddr avrcp_addr = {{0}};

void app_connect_avrcp(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    t_bdaddr null_addr = {{0}};

    if(!memcmp(pAddr->bytes, null_addr.bytes, SIZE_OF_BDADDR))
    {
        return ;
    }

    event.nEventId = RDABT_AVRCP_CONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_avrcp_sync_volume(u_int8 volume)
{
    COS_EVENT event = {0};
    hal_HstSendEvent(1, 0x20171401);
    hal_HstSendEvent(1, volume);
    event.nEventId = RDABT_AVRCP_VOLUME_SYNC_REQ;
    event.nParam1 = (UINT32)volume;
    COS_SendEvent(MOD_BT, &event,COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_avrcp(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_AVRCP_DISCONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_avrcp_all(void)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_AVRCP_DISCONNECT_ALL_REQ;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_avrcp_get_adp_info_req(void)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_AVRCP_GET_ADP_INFO_REQ;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_avrcp_send_key(u_int8 key, u_int8 press, u_int8 param, t_bdaddr addr)
{
    COS_EVENT event = {0};

    if(avrcp_key.pending == 1)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0003);
        return ;
    }

    avrcp_key.addr = addr;
    avrcp_key.pending = 1;
    avrcp_key.opid = key;
    avrcp_key.press = press;
    avrcp_key.param = param;
    event.nEventId = RDABT_AVRCP_SEND_KEY_REQ;
    event.nParam1 = (u_int32)&avrcp_key;

    hal_HstSendEvent(1, 0x05121019);
    hal_HstSendEvent(1, key);
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

APP_AVRCP_ADP_INFO *app_get_avrcp_adp_info(void)
{
    if(app_get_mgr_state() <= MGR_ACTIVE_PENDING)
    {
        return NULL;
    }
    return s_avrcp_adp_info;
}

void app_clean_avrcp_adp_info(void)
{
    s_avrcp_adp_info = NULL;    
}

APP_AVRCP_DEVICE_INFO *app_find_avrcp_device(t_bdaddr addr)
{
    u_int8 i = 0;
    APP_AVRCP_DEVICE_INFO *ret = NULL;
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
    
    if(avrcp_adp_info == NULL)
    {
        return ret;
    }
    
    for(; i < avrcp_adp_info->avrcp_max_device_num; i++)
    {
        if(avrcp_adp_info->avrcp_device_info_list[i].avrcp_adp_state == AVRCP_ADP_STATE_CONNECTED
            &&
            !memcmp(addr.bytes, avrcp_adp_info->avrcp_device_info_list[i].avrcp_addr.bytes, SIZE_OF_BDADDR))
        {
            ret = &avrcp_adp_info->avrcp_device_info_list[i];
            break;
        }
    }

    return ret;
}

u_int8 app_check_avrcp_connection(void)
{
    u_int8 i = 0;
    u_int8 ret = 0;

    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
    
    if(avrcp_adp_info != NULL)    
    {
        for(; i < avrcp_adp_info->avrcp_max_device_num; i++)
        {
            if(avrcp_adp_info->avrcp_device_info_list[i].avrcp_adp_state >= AVRCP_ADP_STATE_CONNECTED)
            {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

BOOL app_check_avrcp_active(t_bdaddr addr, APP_AVRCP_DEVICE_INFO** device_info)
{
    u_int8 i = 0;
    
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
    
    if(avrcp_adp_info == NULL)
    {
        return FALSE;
    }
    
    for(; i < avrcp_adp_info->avrcp_max_device_num; i++)
    {
        if(avrcp_adp_info->avrcp_device_info_list[i].Cur_Play_Status == AVRCP_STATUS_PLAYING
            &&
            memcmp(addr.bytes, avrcp_adp_info->avrcp_device_info_list[i].avrcp_addr.bytes, SIZE_OF_BDADDR))
        {
            if(device_info != NULL)
            {
                *device_info = &avrcp_adp_info->avrcp_device_info_list[i];
            }
            return TRUE;
        }
    }

    return FALSE;
}

APP_AVRCP_DEVICE_INFO *app_find_active_avrcp_device(void)
{
    u_int8 i = 0;
    APP_AVRCP_DEVICE_INFO *ret = NULL;
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
    
    if(avrcp_adp_info == NULL)
    {
        return ret;
    }
    
    for(; i < avrcp_adp_info->avrcp_max_device_num; i++)
    {
        if(avrcp_adp_info->avrcp_device_info_list[i].Cur_Play_Status == AVRCP_STATUS_PLAYING)
        {
            ret = &avrcp_adp_info->avrcp_device_info_list[i];
        }
    }

    return ret;    
}

void app_avrcp_update_last_device(void)
{
    u_int8 i = 0;
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();

    if(avrcp_adp_info == NULL)
    {
        return ;
    }
    
    for(; i < avrcp_adp_info->avrcp_max_device_num; i++)
    {
        if(avrcp_adp_info->avrcp_device_info_list[i].avrcp_adp_state == AVRCP_ADP_STATE_CONNECTED)
        {
            avrcp_adp_info->last_addr = avrcp_adp_info->avrcp_device_info_list[i].avrcp_addr;
            break;
        }
    }
}

void app_avrcp_switch_mode(void)
{
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();

    if(avrcp_adp_info == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0001);
        return ;
    }

    if(avrcp_adp_info != NULL && avrcp_adp_info->avrcp_connect_index > 0)
    {
        app_disconnect_avrcp_all();
    } 
}


void app_avrcp_handle_key(u_int32 key)
{
    static u_int8 fast_forward = 0;
    APP_AVRCP_DEVICE_INFO *avrcp_device;// = app_find_active_avrcp_device();
    APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();

    if(key == NULL || key == AP_MSG_RTC)
    {
        return ;
    }

    if(avrcp_adp_info == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0001);
        return ;
    }

    //if(avrcp_device == NULL)
    {
        avrcp_device = app_find_avrcp_device(avrcp_adp_info->last_addr);
        if(avrcp_device == NULL)
        {
#if APP_SUPPORT_SOUNDBAR == 0
            hal_HstSendEvent(SYS_EVENT, 0xeeee0002);
#endif
            return ;
        }
    }

    switch(key)
    {
        case AP_KEY_PLAY|AP_KEY_PRESS:
        {
            if(avrcp_device != NULL)
            {

                hal_HstSendEvent(1, 0x05121088);

                hal_HstSendEvent(1, avrcp_device->Cur_Play_pause_key_status);
                if(avrcp_device->Cur_Play_pause_key_status == AVRCP_ADP_STATE_PLAY)
                {
                    app_avrcp_send_key(OPID_PAUSE, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
                    avrcp_device->Cur_Play_pause_key_status=AVRCP_ADP_STATE_PAUSE;
                }
                else
                {
                    app_avrcp_send_key(OPID_PLAY, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
                    avrcp_device->Cur_Play_pause_key_status=AVRCP_ADP_STATE_PLAY;
                }
            }
        }
        break;
        
#if APP_SUPPORT_AVRCP_SYNC_VOL==1        
#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_VOLADD:
#endif
        case AP_KEY_VOLADD|AP_KEY_PRESS:
        case AP_KEY_VOLADD|AP_KEY_HOLD:
        {
            hal_HstSendEvent(SYS_EVENT, 0x08221710);
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume_a2dp);
            app_avrcp_send_key(OPID_VOLUME_UP, AVRCP_NO_PRESS
                        , (u_int8)ap_convert_volume(AUD_MAX_LEVEL, 0x7f, (UINT16)app_bt_vars->volume_a2dp),
                        avrcp_device->avrcp_addr);
        }
        break;

#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_VOLSUB:    
#endif            
        case AP_KEY_VOLSUB|AP_KEY_PRESS:
        case AP_KEY_VOLSUB|AP_KEY_HOLD: 
        {
            hal_HstSendEvent(SYS_EVENT, 0x08221711);
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume_a2dp);
            app_avrcp_send_key(OPID_VOLUME_DOWN, AVRCP_NO_PRESS
                        , (u_int8)ap_convert_volume(AUD_MAX_LEVEL, 0x7f, (UINT16)app_bt_vars->volume_a2dp),
                        avrcp_device->avrcp_addr);
        }
        break;
#endif      

        case AP_KEY_NEXT|AP_KEY_PRESS:
        {
            app_avrcp_send_key(OPID_FORWARD, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
        }
        break;  
        
        case AP_KEY_PREV|AP_KEY_PRESS:
        {
            app_avrcp_send_key(OPID_BACKWARD, AVRCP_NO_PRESS, 0, avrcp_device->avrcp_addr);
        }
        break;

        case AP_KEY_NEXT|AP_KEY_HOLD:
        {
            if(MESSAGE_IsHold())
            {
                if(fast_forward == 0)
                {
                    fast_forward = 1;
                }
                app_avrcp_send_key(OPID_FAST_FORWARD, AVRCP_PRESS, 0, avrcp_device->avrcp_addr);
            }
        }
        break;	
        
        case AP_KEY_PREV|AP_KEY_HOLD:
        {	
            if(MESSAGE_IsHold())
            {
                hal_HstSendEvent(APP_EVENT, 0x20170210);
                if(fast_forward == 0)
                {
                    fast_forward = 1;
                }
                app_avrcp_send_key(OPID_REWIND, AVRCP_PRESS, 0, avrcp_device->avrcp_addr);
            }
        }
        break;
        
        case AP_KEY_NEXT|AP_KEY_UP:
        {
            if(fast_forward)
            {
                app_avrcp_send_key(OPID_FAST_FORWARD, AVRCP_RELEASE, 0, avrcp_device->avrcp_addr);
            }
            fast_forward = 0;
        }
        break;	
        
        case AP_KEY_PREV|AP_KEY_UP:
        {	
            if(fast_forward)
            {
                app_avrcp_send_key(OPID_REWIND, AVRCP_RELEASE, 0, avrcp_device->avrcp_addr);
            }
            fast_forward = 0;
        }
        break;
        
    }
}

void app_avrcp_free_msg_info(APP_AVRCP_MSG_INFO *ptr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_AVRCP_VENDOR_DATA_FINISH_REQ;
    event.nParam1 = (u_int32)ptr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_avrcp_msg_handle(COS_EVENT *ev)
{
    app_trace(APP_BT_TRC, "app_avrcp_msg_handle event:%d, nparam=%d", ev->nEventId, ev->nParam1);
    
    switch(ev->nEventId)
    {
        case RDABT_AVRCP_CONNECT_CNF_MSG:
        case RDABT_AVRCP_CONNECT_IND_MSG:
        {
            APP_AVRCP_MSG_INFO *msg_info = (APP_AVRCP_MSG_INFO *)ev->nParam1;
            APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
            APP_AVRCP_DEVICE_INFO *device_info = app_find_avrcp_device(msg_info->avrcp_addr);
            hal_HstSendEvent(SYS_EVENT, 0x20170122);
            hal_HstSendEvent(SYS_EVENT, msg_info->msg_param);
            if(device_info == NULL)
            {
                app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
                return ;
            }
            //if(msg_info->msg_param == RDABT_NOERROR)
            {
                if (!IS_BT_MODE() || app_get_mgr_state() <= MGR_CLOSE_PENDING)
                {
                    if(device_info != NULL)
                    {      
                        app_disconnect_avrcp(&device_info->avrcp_addr);
                    }
                    break;
                }
                else
                {
                    avrcp_adp_info->last_addr = msg_info->avrcp_addr;
                }
                device_info->Cur_Play_pause_key_status=AVRCP_ADP_STATE_PAUSE;
#if APP_SUPPORT_AVRCP_SYNC_VOL == 1
                app_avrcp_sync_volume((u_int8)ap_convert_volume(AUD_MAX_LEVEL, 127, app_bt_vars->volume_a2dp));
#endif
            }
            app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
        }
        break; 
        
        case RDABT_AVRCP_DISCONNECT_CNF_MSG:
        case RDABT_AVRCP_DISCONNECT_IND_MSG:
        {
            app_avrcp_update_last_device();
            app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
        }
        break;
        
        case RDABT_AVRCP_ADP_INFO_MSG:
        {
            APP_AVRCP_ADP_INFO *msg_info = (APP_AVRCP_ADP_INFO *)ev->nParam1;  
            if(msg_info != NULL)
            {
                s_avrcp_adp_info = msg_info;
            }
        }
        break;   
        
        case RDABT_AVRCP_VOLUME_IND:
        {
#if APP_SUPPORT_AVRCP_SYNC_VOL == 1
            app_bt_vars->volume_a2dp = (int8) ap_convert_volume(127, AUD_MAX_LEVEL, (UINT16) ev->nParam1);

            hal_HstSendEvent(APP_EVENT, 0x07061546);
            hal_HstSendEvent(APP_EVENT, app_bt_vars->volume_a2dp);

            if(no_play_inter())
            {
                mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);      
            }
            else
            {
                g_cur_volume = app_bt_vars->volume_a2dp;
            }
#endif
        }
        break;
        
        case RDABT_AVRCP_PLAYER_STATUS_IND_MSG:
        {
            app_trace(APP_BT_TRC, "app_avrcp_msg_handle current player status :%d,", ev->nParam1);
            hal_HstSendEvent(SYS_EVENT, 0x11211637);
            hal_HstSendEvent(SYS_EVENT, ev->nParam1);
        }
        break;	
        
        case RDABT_AVRCP_NOTIFICATION_IND_MSG:
        {
            APP_AVRCP_MSG_INFO *msg_info = (APP_AVRCP_MSG_INFO *)ev->nParam1;
            APP_AVRCP_DEVICE_INFO *avrcp_info = app_find_avrcp_device(msg_info->avrcp_addr);
            APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
            avrcp_addr = msg_info->avrcp_addr;
            hal_HstSendEvent(1, 0x05121438);
            hal_HstSendEvent(1, msg_info->msg_param);
            hal_HstSendEvent(1, msg_info->msg_param1);
#if APP_SUPPORT_HFP == 1        
            hal_HstSendEvent(1, app_get_hfp_state());

            if(app_get_hfp_state() > HFP_CONNECTED)
            {
                app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
                if(AVRCP_EVENT_PLAYBACK_STATUS_CHANGED == msg_info->msg_param && avrcp_info != NULL && msg_info->msg_param1 == AVRCP_STATUS_PAUSED)
                    avrcp_info->Cur_Play_pause_key_status = AVRCP_ADP_STATE_PAUSE;
                return ;
            }
#endif        
            if(AVRCP_EVENT_TRACK_CHANGED == msg_info->msg_param)
            {

            }
            else if(AVRCP_EVENT_PLAYBACK_STATUS_CHANGED == msg_info->msg_param)
            {            
                
                if(msg_info->msg_param1 == AVRCP_STATUS_PLAYING)
                {
                    if(avrcp_info != NULL)
                    {
                        avrcp_info->Cur_Play_pause_key_status = AVRCP_ADP_STATE_PLAY;
                    }
                }
                else if(msg_info->msg_param1 == AVRCP_STATUS_PAUSED)
                {
                    if(avrcp_info != NULL)
                    {
                        avrcp_info->Cur_Play_pause_key_status = AVRCP_ADP_STATE_PAUSE;
                        if(avrcp_adp_info != NULL && avrcp_adp_info->avrcp_connect_index > 1)
                        {
                            app_a2dp_stop_sbc();
                        }
                    }
                }
            }
            else if(AVRCP_EVENT_PLAYBACK_POS_CHANGED == msg_info->msg_param)
            {

            }
            else
            {

            }
            app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
        }
        break; 
        
        case RDABT_AVRCP_VENDOR_DATA_IND_MSG:
        {
            RDABT_AVRCP_MSG_INFO *msg_info = (RDABT_AVRCP_MSG_INFO *)ev->nParam1;
            hal_HstSendEvent(1, 0x04271358);
            hal_HstSendEvent(1, msg_info->msg_param);
            hal_HstSendEvent(1, msg_info->msg_param1);
            hal_HstSendEvent(1, ev->nEventId);

            if(ev->nEventId != 0)
            {
                avrcp_vender_ptr = (u_int8 *)msg_info->msg_param1;
            }

            if(AVRCP_PDU_LISTPLAYERAPPLICATIONSETTINGATTRIBUTES == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_LISTPLAYERAPPLICATIONSETTINGVALUES == msg_info->msg_param)
            {
            }
            else if(AVRCP_PDU_GETCURRENTPLAYERAPPLICATIONSETTINGVALUE == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_SETPLAYERAPPLICATIONSETTINGVALUE == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_GETPLAYERAPPLICATIONSETTINGATTRIBUTETEXT == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_GETPLAYERAPPLICATIONSETTINGVALUETEXT == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_GETELEMENTATTRIBUTES == msg_info->msg_param)
            {

            }
            else if(AVRCP_PDU_GETPLAYSTATUS == msg_info->msg_param)
            {
           
                u_int8 *param = (u_int8 *)msg_info->msg_param1;
                APP_AVRCP_DEVICE_INFO *avrcp_info = app_find_avrcp_device(msg_info->avrcp_addr);
                APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
                avrcp_addr = msg_info->avrcp_addr;
                hal_HstSendEvent(SYS_EVENT, 0x04181022);
                hal_HstSendEvent(SYS_EVENT, param[8]);  
                
                if(param[8] == AVRCP_STATUS_PLAYING)
                {
                    if(avrcp_info != NULL)
                    {
                        avrcp_info->Cur_Play_pause_key_status = AVRCP_ADP_STATE_PLAY;
                    }
                }
                else if(param[8] == AVRCP_STATUS_PAUSED)
                {
                    if(avrcp_info != NULL)
                    {
                        avrcp_info->Cur_Play_pause_key_status = AVRCP_ADP_STATE_PAUSE;
                        if(avrcp_adp_info != NULL && avrcp_adp_info->avrcp_connect_index > 1)
                        {
                            app_a2dp_stop_sbc();
                        }
                    }
                }
                              
            }
            else
            {
                app_trace(APP_BT_TRC, "app_avrcp_msg_handle nParam1 ERROR ");
            }

            app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)ev->nParam1);
            if(ev->nEventId != 0)
            {
                app_avrcp_free_msg_info((APP_AVRCP_MSG_INFO *)avrcp_vender_ptr);
            }
        }
        break;
        
        case RDABT_AVRCP_BROWSING_DATA_IND_MSG:
        {
            if(AVRCP_SET_BROWSED_PLAYER == ev->nParam1)
            {

            }
        }
        break;
    }
}
#else
void app_connect_avrcp(t_bdaddr *pAddr)
{

}
void app_disconnect_avrcp(t_bdaddr *pAddr)
{

}
#endif

