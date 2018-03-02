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

#include "ap_hfp.h"
#include"resource_id.h"
#include "ap_media.h"
#include "ap_message.h"

#if APP_SUPPORT_HFP == 1

static u_int8 s_hfp_state = HFP_IDLE;
static APP_HFP_ADP_INFO *s_hfp_adp_info = NULL;
RDABT_HFP_MSG_INFO app_msg_info;
char  iphone_str[32] = {0};
static u_int32 phonebook_size = 0;
static u_int32 phonebook_start = 0;
static u_int32 phonebook_end = 0;
static t_bdaddr pb_addr = {{0}};

u_int8 app_get_hfp_state(void)
{
    return s_hfp_state;
}

void app_clean_hfp_adp_info(void)
{
    s_hfp_adp_info = NULL;    
}

void app_set_hfp_state(u_int8 state)
{
    hal_HstSendEvent(SYS_EVENT, 0x05161521);
    hal_HstSendEvent(SYS_EVENT, s_hfp_state);
    hal_HstSendEvent(SYS_EVENT, state);
    hal_HstSendEvent(SYS_EVENT, app_get_hfp_adp_info()->sco_handle);
    
    if(app_get_hfp_adp_info()->sco_handle != 0 && 
        s_hfp_state == HFP_PLAY_SCO && 
        (state > HFP_CONNECTED) && ((state&HFP_AUDIO_STATE_ON_MASK) == 0))
    {
        return ;
    }

#if APP_SUPPORT_SOUNDBAR == 0
    if(state == HFP_PLAY_SCO)
    {
        app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    }
    else if(s_hfp_state == HFP_PLAY_SCO && state != HFP_PLAY_SCO)
    {
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#else
#if APP_SUPPORT_ONLY_ONE_LINK == 0
        app_set_bt_visible_req(MGR_SET_CONNECTABLE);
#endif
#endif
    }
#endif
    hal_HstSendEvent(SYS_EVENT, 0x05161520);
    hal_HstSendEvent(SYS_EVENT, s_hfp_state);
    hal_HstSendEvent(SYS_EVENT, state);
    s_hfp_state = state;
#if APP_SUPPORT_SOUNDBAR == 1
    app_send_soundbar_hfp_state(state);
#endif

    if(s_hfp_state == HFP_CALL_INCOMING)
    {
        LED_SetBgPattern(GUI_LED_BT_CALL_INCOMING);//blue
    }
    else if(s_hfp_state == HFP_CALL_OUTGOING || s_hfp_state == HFP_CALL_ACTIVE)
    {
        LED_SetBgPattern(GUI_LED_BT_CALL);//blue
    }
    else if(s_hfp_state == HFP_CONNECTED)
    {
        LED_SetBgPattern(GUI_LED_BT_CONNECT);//blue
    }

}


#if APP_SUPPORT_NUMBER_VOICE==1
void app_start_play_tel_number(u_int8 *ptr_num)
{
    u_int8 i = 0;
    static u_int8 tel_num[16] = 
                        {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                         0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    static u_int8 voice_index = 0;
    static u_int8 tel_len = 0;
    static const u_int8 voice_map[BT_VOICE_NUMBER_COUNT] = 
                                {GUI_AUDIO_NUMBER_0, GUI_AUDIO_NUMBER_1, GUI_AUDIO_NUMBER_2, 
                                 GUI_AUDIO_NUMBER_3, GUI_AUDIO_NUMBER_4,GUI_AUDIO_NUMBER_5,
                                 GUI_AUDIO_NUMBER_6,GUI_AUDIO_NUMBER_7,GUI_AUDIO_NUMBER_8, 
                                 GUI_AUDIO_NUMBER_9};

    hal_HstSendEvent(SYS_EVENT, 0x05041456);
    hal_HstSendEvent(SYS_EVENT, (u_int32)ptr_num);
    hal_HstSendEvent(SYS_EVENT, s_hfp_state);
    hal_HstSendEvent(SYS_EVENT, tel_num[0]);
    hal_HstSendEvent(SYS_EVENT, voice_index);
    hal_HstSendEvent(SYS_EVENT, tel_len);

    if(app_get_hfp_state() == HFP_PLAY_SCO)
    {
        voice_index = 0;
        tel_len = 0;
        memset(tel_num, 0xff, sizeof(tel_num));
        return ;
    }
    
    if(ptr_num != NULL)
    {
        if(tel_len > 0)
        {
            hal_HstSendEvent(SYS_EVENT, 0xeeee000a);
            return ;
        }
        
        while(ptr_num[i] != 0)
    	{
    		tel_num[i] = ptr_num[i];
    		i++;
    	}
        tel_len = i;
    }

    if((app_get_hfp_state() == HFP_CALL_INCOMING))
    {
        if(tel_num[voice_index] != 0xff)
        {
            hal_HstSendEvent(1, 0x08121730);
            hal_HstSendEvent(1, voice_index);
            media_PlayInternalAudio(voice_map[tel_num[voice_index]-0x30], 1, FALSE);
            voice_index++;
        }
        else if(voice_index >= tel_len && tel_len != 0)
        {
            media_PlayInternalAudio(GUI_AUDIO_BT_RING, 1, FALSE);
        }
    }
    else if(tel_num[0] != 0xff)
    {
        media_StopInternalAudio();
        voice_index = 0;
        tel_len = 0;
        memset(tel_num, 0xff, sizeof(tel_num));
    }
}

#endif

APP_HFP_ADP_INFO *app_get_hfp_adp_info(void)
{
    if(app_get_mgr_state() <= MGR_ACTIVE_PENDING)
    {
        return NULL;
    }
    return s_hfp_adp_info;
}

APP_HFP_DEVICE_INFO *app_find_hfp_device(t_bdaddr addr)
{
    u_int8 i = 0;
    t_bdaddr null_addr = {{0}};
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    if(hfp_adp_info == NULL
    || !memcmp(addr.bytes, null_addr.bytes, SIZE_OF_BDADDR))
    {
        return NULL;
    }
    
    for(; i < hfp_adp_info->hfp_max_device_num; i++)
    {
        if(!memcmp(addr.bytes, hfp_adp_info->hfp_device_info_list[i].hfp_addr.bytes, SIZE_OF_BDADDR))
        {
            return &hfp_adp_info->hfp_device_info_list[i];
        }
    }
    return NULL;
}

int app_find_hfp_device_state(t_bdaddr addr)
{
    APP_HFP_DEVICE_INFO *hfp_device = app_find_hfp_device(addr);
    if(hfp_device != NULL)
    {
        return hfp_device->hfp_adp_state;
    }
    return NULL;
}


APP_HFP_DEVICE_INFO *app_find_active_hfp_device(void)
{
    u_int8 i = 0;
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    
    if(hfp_adp_info == NULL)
    {
        return NULL;
    }
    
    for(; i < hfp_adp_info->hfp_max_device_num; i++)
    {
        if(hfp_adp_info->hfp_device_info_list[i].hfp_call_status != 0)
        {
            return &hfp_adp_info->hfp_device_info_list[i];
        }
    }
    return NULL;
}

APP_HFP_DEVICE_INFO *app_get_hfp_idle_device(void)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    u_int8 i = 0;
    t_bdaddr dump_addr = {{0}};
    
    if(hfp_adp_info != NULL)
    {
        for(; i < hfp_adp_info->hfp_max_device_num; i++)
        {
            if(!memcmp(hfp_adp_info->hfp_device_info_list[i].hfp_addr.bytes, dump_addr.bytes, SIZE_OF_BDADDR)
                &&
                hfp_adp_info->hfp_device_info_list[i].hfp_adp_state == HFP_ADP_STATE_IDLE)
            {
                return &hfp_adp_info->hfp_device_info_list[i];
            }
        }

        for(i = 0; i < hfp_adp_info->hfp_max_device_num; i++)
        {
            if(hfp_adp_info->hfp_device_info_list[i].hfp_adp_state == HFP_ADP_STATE_IDLE)
            {
                return &hfp_adp_info->hfp_device_info_list[i];
            }
        }
    }
    return NULL;
}

void app_create_hfp_connection(u_int32 timeoutTenths, t_bdaddr addr)
{
    APP_HFP_DEVICE_INFO *hfp_device = app_find_hfp_device(addr);
    
    hal_HstSendEvent(1, 0x20170620);
    hal_HstSendEvent(1, app_get_a2dp_state());
	
    if(hfp_device == NULL)
    {
        hfp_device = app_get_hfp_idle_device();
        if(hfp_device != NULL)
        {
            memset(hfp_device, 0, sizeof(APP_HFP_DEVICE_INFO));
            hfp_device->hfp_addr = addr;
        }
    }
    
    hal_HstSendEvent(1, (u_int32)hfp_device);
    hal_HstSendEvent(1, hfp_device->hfp_adp_state);
    hal_HstSendEvent(1, 0x20170620);
    
    if(hfp_device != NULL)
    {
        if(hfp_device->hfp_adp_state == HFP_IDLE)
        {
            if(hfp_device->hfp_check_timer == 0)
            {
                hfp_device->check_cnt = 0;
                hfp_device->hfp_check_timer = 
                mmi_timer_create(timeoutTenths, app_hfp_check_device_conneciton, (void *)hfp_device, COS_TIMER_MODE_SINGLE);
            }
        }
    }
}

void app_cancel_create_hfp_connection(t_bdaddr addr)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    APP_HFP_DEVICE_INFO *hfp_device = app_find_hfp_device(addr);
    hal_HstSendEvent(SYS_EVENT, 0x20170618);
    hal_HstSendEvent(SYS_EVENT, (u_int32)hfp_adp_info);
    hal_HstSendEvent(SYS_EVENT, (u_int32)hfp_device);
  
    if(hfp_adp_info == NULL || hfp_device == NULL)
    {
        return ;
    }
    
    hal_HstSendEvent(SYS_EVENT, 0x20170681);
    hal_HstSendEvent(SYS_EVENT, hfp_device->hfp_check_timer);

    if(hfp_device->hfp_check_timer != 0)
    {   
        mmi_cancel_timer(hfp_device->hfp_check_timer);
        hfp_device->hfp_check_timer = 0; 
    }
    
}

void app_hfp_clean_timer(void)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    u_int8 i = 0;
    
    if(hfp_adp_info == NULL)
    {
        return ;
    }

    for(; i < hfp_adp_info->hfp_max_device_num; i++)
    {
        if(hfp_adp_info->hfp_device_info_list[i].hfp_check_timer)
        {
            mmi_cancel_timer(hfp_adp_info->hfp_device_info_list[i].hfp_check_timer);
            hfp_adp_info->hfp_device_info_list[i].hfp_check_timer = 0; 
        }
    }
}

u_int8 app_check_hfp_connection(void)
{
    u_int8 i = 0;
    u_int8 ret = 0;

    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    if(hfp_adp_info == NULL)
    {
        return ret;
    }
        
    if(hfp_adp_info != NULL)    
    {
        for(; i < hfp_adp_info->hfp_max_device_num; i++)
        {
            if(hfp_adp_info->hfp_device_info_list[i].hfp_adp_state == HFP_ADP_STATE_CONNECTED)
            {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void app_hfp_play_sco(void)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    
    hal_HstSendEvent(SYS_EVENT, 0x05041342);
    hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());
    
    if(hfp_adp_info == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0004);
        return ;
    }
    
    hal_HstSendEvent(SYS_EVENT, hfp_adp_info->sco_handle);
    
    if(app_get_hfp_state()==HFP_CONNECTED)
    {
        APP_HFP_DEVICE_INFO *hfp_device = NULL;
        hfp_device = app_find_hfp_device(hfp_adp_info->first_addr);
        if(hfp_device)
        {
            if( hfp_device->hfp_call_status==HFP_CALL_STATUS_ACTIVE)
               {
                   app_set_hfp_state(HFP_CALL_ACTIVE);
                   
                   hal_HstSendEvent(SYS_EVENT, 0x0504134a);
                }

        }
    }
    
    if(hfp_adp_info->sco_handle != 0 
        &&
       (app_get_hfp_state() == HFP_CALL_ACTIVE || 
       app_get_hfp_state() == HFP_CALL_OUTGOING ||
       app_get_hfp_state() == HFP_VOICE_RECOGNITION
#if APP_SUPPORT_NUMBER_VOICE==0        //if  APP_SUPPORT_NUMBER_VOICE==0    ,con iphone 6S, incoming call ,no ring 
       ||  app_get_hfp_state() == HFP_CALL_INCOMING
#endif
        ))
    {
#if APP_SUPPORT_NUMBER_VOICE==1
        media_StopInternalAudio();
        app_start_play_tel_number(NULL);
#endif
        app_set_hfp_state(HFP_PLAY_SCO);
        mediaSendBTCommand(MC_STOP, 0);
        mediaSendBTCommand(MC_PLAY_SCO, hfp_adp_info->sco_handle);
        mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_hfp);
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0005);
    }
}

void app_hfp_stop_sco(void)
{
#if APP_SUPPORT_A2DP == 1
    hal_HstSendEvent(SYS_EVENT, 0x05041344);
    hal_HstSendEvent(SYS_EVENT, app_get_a2dp_state());
    if(app_get_a2dp_state() != A2DP_PLAY)
#endif
    {
        mediaSendBTCommand(MC_STOP, 0);
    }
}

void app_hfp_update_last_device(void)
{
    u_int8 i = 0;
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    if(hfp_adp_info == NULL)
    {
        return ;
    }
    for(; i < hfp_adp_info->hfp_max_device_num; i++)
    {
        if(hfp_adp_info->hfp_device_info_list[i].hfp_adp_state == HFP_ADP_STATE_CONNECTED)
        {
            hfp_adp_info->last_addr = hfp_adp_info->hfp_device_info_list[i].hfp_addr;
            break;
        }
    }
}

void app_hfp_bt_state_machine(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case RDABT_HFP_CONNECTED_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;
            if(hfp_msg->msg_param1 == RDABT_NOERROR)
            {
                if(app_get_hfp_state() == HFP_IDLE)
                {
                app_set_hfp_state(HFP_CONNECTED);
                    }
            }
        }
        break;
        
        case RDABT_HFP_ACL_DISCONNECT_IND_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x05161636);
            if(!app_check_hfp_connection())
            {
                hal_HstSendEvent(SYS_EVENT, 0x05161637);
                app_set_hfp_state(HFP_IDLE);
            }
        }
        break;
        
        case RDABT_SCO_DISCONNECT_IND_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;
            APP_HFP_DEVICE_INFO *hfp_device = app_find_hfp_device(msg_info->device_addr);

            app_get_hfp_adp_info()->sco_handle = 0;
            
            if(!app_check_hfp_connection())
            {
                hal_HstSendEvent(SYS_EVENT, 0x05161638);
                app_set_hfp_state(HFP_IDLE);
            }
            else
            {
                hal_HstSendEvent(SYS_EVENT, 0x05161639);
                hal_HstSendEvent(SYS_EVENT, (u_int32)hfp_device);
                
                if(hfp_device != NULL)
                {
                    hal_HstSendEvent(SYS_EVENT, hfp_device->hfp_call_status);
                    switch(hfp_device->hfp_call_status)
                    {
                        case HFP_CALL_STATUS_NONE:
                            app_set_hfp_state(HFP_CONNECTED);
                        break;    
                        case HFP_CALL_STATUS_INCOMING:
                            app_set_hfp_state(HFP_CALL_INCOMING);
                        break;
                        case HFP_CALL_STATUS_OUTGOING:
                        case HFP_CALL_STATUS_ALERT:
                            app_set_hfp_state(HFP_CALL_OUTGOING); 
                        break;
                        case HFP_CALL_STATUS_ACTIVE:
                            app_set_hfp_state(HFP_CALL_ACTIVE);
                        break;
                    }
                }
            }
        }
        break;
        
        case RDABT_HFP_CALL_STATUS_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x05041448);
            hal_HstSendEvent(SYS_EVENT, s_hfp_state);
            hal_HstSendEvent(SYS_EVENT, hfp_msg->msg_param1);
            if(hfp_msg->msg_param1 == HFP_CALL_STATUS_INCOMING)
            {
                app_set_hfp_state(HFP_CALL_INCOMING);
            }
            else if(hfp_msg->msg_param1 == HFP_CALL_STATUS_OUTGOING||hfp_msg->msg_param1 == HFP_CALL_STATUS_ALERT)
            {
                app_set_hfp_state(HFP_CALL_OUTGOING);
            }
            else if(hfp_msg->msg_param1 == HFP_CALL_STATUS_ACTIVE)
            {
                app_set_hfp_state(HFP_CALL_ACTIVE);
            }
            else if(hfp_msg->msg_param1 == HFP_CALL_STATUS_END
                        ||hfp_msg->msg_param1 == HFP_CALL_STATUS_NONE)
            {
                app_set_hfp_state(HFP_CONNECTED);
            }
        }
        break;
    }
}

static void deal_pbap_hf_data(char *data, u_int32 type)
{
    int ret = 0;
    int index = 0;
    int tmp_index  = 0;
    char pb_size_data[4] = {0};
    char number[20] = {0};
    char name[40] = {0};
    int size_tmp = 0;
    char cpbr_size = 0;

    ret = find_str(&data[index], ":", 1);
    if(ret >= 0)
    {
        if(data[index + ret + 2] == '(')
        {
            cpbr_size = 1;
        }
    }

    if(cpbr_size)
    {
        index += ret;
        index += 2;
        ret = find_str(&data[index], "-", 1);
        
        if(ret >= 0)
        {
            index += ret;
            index += 1;
            tmp_index = index;
            ret = find_str(&data[index], ")", 1);
            if(ret > 0)
            {
                index += ret;
                if((index - tmp_index) > sizeof(pb_size_data))
                {
                    strncpy(pb_size_data, &data[tmp_index], sizeof(pb_size_data));
                }
                else
                {
                    strncpy(pb_size_data, &data[tmp_index], index - tmp_index);
                }
                
                size_tmp = atoi(pb_size_data);
                switch(type)
                {
                    case PHONE_BOOK_PHONE_BOOK:
                    {
                        phonebook_size = size_tmp;
                        phonebook_start = 1;
                        phonebook_end = 1;
                        app_hfp_get_pbap_data_req(&pb_addr, phonebook_start, phonebook_end);
                    }
                    break;
                    
                    case PHONE_BOOK_INCOMING_HISTORY:
                    case PHONE_BOOK_OUTCOMING_HISTORY:
                    case PHONE_BOOK_MISSED_CALL:
                    {
                        /*
                        history_size = size_tmp;
                        history_step = 3;
                        last_size = history_size > 4 ? 4 : history_size;
                        HF_Get_PhoneBook(1, last_size);*/
                    }
                    break;
                }
            }
        }
    } 
    else
    {
        while(1)
        {
            ret = find_str(&data[index], "+CPBR", 5);
            if(ret < 0)
            {
                break ;
            }
            
            index += ret;
            index += 5;
            ret = find_str(&data[index], "\"", 1);
            if(ret < 0)
            {
                break ;
            }
            
            index += ret;
            index += 1;
            tmp_index = index;
            ret = find_str(&data[index], "\"", 1);
            if(ret < 0)
            {
                break ;
            }
            
            index += ret;
            if((index-tmp_index) > sizeof(number))
            {
                strncpy(number, &data[tmp_index], sizeof(number));
            }
            else
            {
                strncpy(number, &data[tmp_index], index-tmp_index);
            }
            
            //号码存在number数组里
            index += 1;
            ret = find_str(&data[index], "\"", 1);
            if(ret < 0)
            {
                break ;
            }
            
            index += ret;
            index += 1;
            tmp_index = index;
            ret = find_str(&data[index], "\"", 1);
                
            if(ret < 0)
            {
                break ;
            }
            
            index += ret;
            if((index-tmp_index) > sizeof(name))
            {
                strncpy(name, &data[tmp_index], sizeof(name));
            }
            else
            {
                strncpy(name, &data[tmp_index], index-tmp_index);
            }
            index += 1;
            //姓名存在name数组里
            //此处需要自行转换文字编码

            //原始数据为UTF-8编码
            switch(type)
            {
                case PHONE_BOOK_PHONE_BOOK:
                break;
                
                case PHONE_BOOK_INCOMING_HISTORY:
                case PHONE_BOOK_OUTCOMING_HISTORY:
                case PHONE_BOOK_MISSED_CALL:
                break;
            }
        }

        if(type == PHONE_BOOK_PHONE_BOOK)
        {
            if(phonebook_end < phonebook_size)
            {
                phonebook_start += 1;
                phonebook_end += 1;
                app_hfp_get_pbap_data_req(&pb_addr, phonebook_start, phonebook_end);
            }
            else
            {
                //同步完毕
            }
        }
    } 
}

void app_hfp_check_device_conneciton(void *pArgs)
{
    APP_HFP_DEVICE_INFO *device_info = (APP_HFP_DEVICE_INFO *)pArgs;
    device_info->hfp_check_timer = 0;
    
    hal_HstSendEvent(SYS_EVENT, 0x20170612);
    hal_HstSendEvent(SYS_EVENT, device_info->hfp_adp_state);
    if(!IS_BT_MODE() || app_bt_vars == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0x01141053);
        //device_info->check_cnt = 0;
        return ;
    }

    if(app_bt_vars->bt_mode != HOST_BR_MODE)
    {
        hal_HstSendEvent(SYS_EVENT, 0x01141053);
        //device_info->check_cnt = 0;
        return ;
    }

    if(device_info->hfp_adp_state == HFP_IDLE)
    {
        app_connect_hfp(&device_info->hfp_addr);
        device_info->check_cnt++;
        if(device_info->check_cnt < 3)
        {
            if(device_info->check_cnt == 0)
            {
                device_info->hfp_check_timer = 
                mmi_timer_create(60, app_hfp_check_device_conneciton, (void *)device_info, COS_TIMER_MODE_SINGLE);
            }
        }
    }
    else
    {
        device_info->check_cnt = 0;
    }
}

void app_hfp_send_nrec(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_HFP_SEND_NREC_MSG;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_connect_hfp(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    t_bdaddr null_addr = {{0}};
    
    hal_HstSendEvent(1, 0x04201028);
    hal_HstSendEvent(1, pAddr->bytes[0]);
    hal_HstSendEvent(1, pAddr->bytes[1]);
    hal_HstSendEvent(1, pAddr->bytes[2]);
    hal_HstSendEvent(1, pAddr->bytes[3]);
    hal_HstSendEvent(1, pAddr->bytes[4]);
    hal_HstSendEvent(1, pAddr->bytes[5]);

    if(!memcmp(pAddr->bytes, null_addr.bytes, SIZE_OF_BDADDR))
    {
        return ;
    }

    event.nEventId = RDABT_HFG_CONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_hfp(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    
    hal_HstSendEvent(1, 0x04201029);
    event.nEventId = RDABT_HFG_DISCONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_hfp_all(void)
{
    COS_EVENT event = {0};

    event.nEventId = RDABT_HFG_DISCONNECT_ALL_REQ;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_call_answer_req(t_bdaddr *pAddr, u_int8 call_status)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HFG_CALL_ANSWER_REQ;
    memcpy(app_msg_info.hfp_addr.bytes, pAddr->bytes, sizeof(t_bdaddr));
    app_msg_info.msg_param1 = call_status;
    event.nParam1 =(UINT32) &app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_call_terminal_req(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HFG_CALL_TERMINAL_REQ;
    memcpy(app_msg_info.hfp_addr.bytes, pAddr->bytes, sizeof(t_bdaddr));
    event.nParam1 = (UINT32)&app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_call_req(t_bdaddr *pAddr, u_int8 call_type, u_int8 *phone_number)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HFG_CALL_REQ;
    hal_HstSendEvent(1, 0x04221400);
    hal_HstSendEvent(1, call_type);
    hal_HstSendEvent(1, pAddr->bytes[5]);
    hal_HstSendEvent(1, pAddr->bytes[4]);
    hal_HstSendEvent(1, pAddr->bytes[3]);
    hal_HstSendEvent(1, pAddr->bytes[2]);
    hal_HstSendEvent(1, pAddr->bytes[1]);
    hal_HstSendEvent(1, pAddr->bytes[0]);
    if(phone_number != NULL)
    {
        hal_HstSendEvent(1, phone_number[0]);
        hal_HstSendEvent(1, phone_number[1]);
    }
    
    memcpy(app_msg_info.hfp_addr.bytes, pAddr->bytes, sizeof(t_bdaddr));
    
    if(call_type == HFP_CALL_REQ_CALLNUMBER)
    {
        memcpy(app_msg_info.msg_buf, phone_number, HFP_MAX_MSG_BUF);
    }
    app_msg_info.msg_param1 = call_type;
    event.nParam1 = (UINT32)&app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_send_raw_data(t_bdaddr *pAddr, u_int8 *data, int data_len)
{
    COS_EVENT event = {0};

    event.nEventId = RDABT_HFG_SEND_DATA_REQ;
    app_msg_info.hfp_addr = *pAddr;
    memcpy(app_msg_info.msg_buf, data, data_len >= HFP_MAX_MSG_BUF ? HFP_MAX_MSG_BUF:data_len );
    app_msg_info.msg_param2 = data_len;
    event.nParam1 = (UINT32)&app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_get_adp_info_req(void)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HFP_GET_ADP_INFO_REQ;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_get_pbap_req(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};

    event.nEventId = RDABT_HF_GET_PBAP_REQ;
    app_msg_info.hfp_addr = *pAddr;
    pb_addr = *pAddr;
    event.nParam1 = (UINT32)&app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_get_pbap_data_req(t_bdaddr *pAddr, u_int32 start, u_int32 end)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HF_GET_PBAP_DATA_REQ;
    app_msg_info.hfp_addr = *pAddr;
    app_msg_info.msg_param1 = start;
    app_msg_info.msg_param2 = end;
    event.nParam1 = (UINT32)&app_msg_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_register_battery(APP_HFP_DEVICE_INFO *device_info)
{
    char *tmp = "AT+XAPL=ffff-9621-0100,7\r\n";
    
    hal_HstSendEvent(1, 0x06231349);
    strncpy(iphone_str, tmp, strlen(tmp));
    app_hfp_send_raw_data(&device_info->hfp_addr, iphone_str, strlen(iphone_str));
}

void app_hfp_send_battery(u_int8 battery)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
    u_int8 i = 0;

    hal_HstSendEvent(SYS_EVENT, 0x05251044);
    if(hfp_adp_info == NULL)
    {
        return ;
    }
    
    for(; i < hfp_adp_info->hfp_max_device_num; i++)
    {
        if(hfp_adp_info->hfp_device_info_list[i].hfp_adp_state == HFP_ADP_STATE_CONNECTED)
        {
            hal_HstSendEvent(SYS_EVENT, 0x05301050);
            memset(iphone_str, 0, sizeof(iphone_str));
            sprintf(iphone_str, "AT+IPHONEACCEV=1,1,%d\r\n", battery);
            app_hfp_send_raw_data(&hfp_adp_info->hfp_device_info_list[i].hfp_addr, iphone_str, strlen(iphone_str));
            COS_Sleep(50);
        }
    }
}

void app_hfp_send_battery_delay_timer(void *pArgs)
{
    u_int32 batt = GetBattery();
    u_int32 hfp_batt;

    hfp_batt = batt/10;
    if(hfp_batt>=10)
    {   
        hfp_batt = 9;
    }
    hal_HstSendEvent(1, 0x20161147);
    app_hfp_send_battery((u_int8)hfp_batt);
}

void app_hfp_send_vol(t_bdaddr addr, u_int8 vol)
{
    COS_EVENT event = {0};
    
    event.nEventId = RDABT_HFP_SEND_SPEAKER_GAIN_REQ;
    app_msg_info.hfp_addr = addr;
    app_msg_info.msg_param1 = (UINT32) ap_convert_volume(AUD_MAX_LEVEL, 15, vol);
    event.nParam1 = (u_int32)&app_msg_info;
    hal_HstSendEvent(SYS_EVENT, 0x20161299);
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_send_voice_recognition(t_bdaddr *addr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_HFP_SEND_VOICE_RECOGNITION_REQ;
    event.nParam1 = (u_int32)addr;
    hal_HstSendEvent(SYS_EVENT, 0x20170629);
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_register_battery_timer(void *pArgs)
{
    app_hfp_register_battery(pArgs);
}


void app_hfp_switch_mode(void)
{
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    if(hfp_adp_info == NULL)
    {
        //hal_HstSendEvent(SYS_EVENT, 0xeeee0006);
        return ;
    }
    
    app_hfp_clean_timer();
    if(hfp_adp_info != NULL && hfp_adp_info->hfp_connect_index > 0)
    {
        app_disconnect_hfp_all();
    }  
}

static BOOL app_hfp_play_finished = FALSE;

VOID app_hfp_poweroff(UINT32 args)
{	
    hal_HstSendEvent(BOOT_EVENT, 0x8899889a);
    app_hfp_play_finished = TRUE;
}

u_int8 app_hfp_handle_key(u_int32 key)
{
    APP_HFP_DEVICE_INFO *hfp_device = NULL;
    u_int8 ret = 0;
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
#if APP_SUPPORT_BTBATTERY==1
    u_int32 batt;
    static u_int32 batt_old;
    u_int32 hfp_batt;
#endif
    t_bdaddr null_addr = {{0}};

    hfp_device = app_find_active_hfp_device();

    if(key == 0)
    {
        return ret;
    }

    if(hfp_adp_info == NULL)
    {
        //hal_HstSendEvent(SYS_EVENT, 0xeeee0006);
        return ret;
    }

    if(hfp_device == NULL)
    {
        hfp_device = app_find_hfp_device(hfp_adp_info->first_addr);

        if(hfp_device == NULL)
            hfp_device = app_find_hfp_device(hfp_adp_info->last_addr);
        
        if(key != (AP_KEY_PLAY|AP_KEY_PRESS))
        {
            if(hfp_device == NULL)
            {
                hal_HstSendEvent(SYS_EVENT, 0xeeee0007);
                return ret;
            }
        }
    }

    switch(key)
    {
        case AP_KEY_MODE|AP_KEY_PRESS:
        {
        }
        break;

        case AP_KEY_PLAY|AP_KEY_PRESS:
        {  
            hal_HstSendEvent(SYS_EVENT, 0x07071413);
            hal_HstSendEvent(SYS_EVENT, (u_int32)hfp_device);
            hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
            if(hfp_device)
            {
                hal_HstSendEvent(SYS_EVENT, hfp_device->hfp_adp_state);
                hal_HstSendEvent(SYS_EVENT, hfp_device->hfp_call_status);
                hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());
                
            }
            
            if(hfp_device != NULL && hfp_device->hfp_adp_state == HFP_ADP_STATE_CONNECTED)
            {
                if(hfp_device->hfp_call_status == HFP_CALL_STATUS_ACTIVE ||
                    hfp_device->hfp_call_status == HFP_CALL_STATUS_OUTGOING ||
                    hfp_device->hfp_call_status == HFP_CALL_STATUS_ALERT)
                {
                    app_hfp_call_terminal_req(&hfp_device->hfp_addr);
                    ret = 1;
                }
                else if(hfp_device->hfp_call_status & HFP_CALL_STATUS_INCOMING)
                {
                    app_hfp_call_req(&hfp_device->hfp_addr, HFP_CALL_REQ_ACCEPT, NULL);
                    ret = 1;
                }
            }
            else if(app_get_mgr_state() == MGR_ACTIVED)
            {
                hal_HstSendEvent(SYS_EVENT, 0x07071414);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_device);
                if(app_bt_vars->last_device != 0xff)
                {
                    if(memcmp(app_bt_vars->device_list[app_bt_vars->last_device].addr.bytes, null_addr.bytes, SIZE_OF_BDADDR))
                    {
                        app_bt_connect(4);
                    }
                }
            }
        }
        break; 

        case AP_KEY_PLAY | AP_KEY_UP:
        {
            hal_HstSendEvent(SYS_EVENT, 0x06291643);
            hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());
            if(app_get_hfp_state() == HFP_VOICE_RECOGNITION)	
            {
                hal_HstSendEvent(BOOT_EVENT, 0x8899889c);
                ClearProtocolEventHandler(EV_AUDIO_INTERNAL_PLAY_FINISH_IND);
                app_hfp_send_voice_recognition(&hfp_device->hfp_addr);
                app_set_hfp_state(HFP_CONNECTED);
            }
        }
        break; 

#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_VOLADD:
        case AT_COMMON_VOLSUB:
#endif
        case AP_KEY_VOLADD|AP_KEY_PRESS:
        case AP_KEY_VOLADD|AP_KEY_HOLD:
        case AP_KEY_VOLSUB|AP_KEY_PRESS:
        case AP_KEY_VOLSUB|AP_KEY_HOLD:            
        {
            if(hfp_device != NULL && hfp_device->hfp_call_status != HFP_CALL_STATUS_NONE)    
            {
                app_hfp_send_vol(hfp_device->hfp_addr, app_bt_vars->volume_hfp);
                ret = 1;
            }
        }
        break;

        case AP_KEY_PLAY|AP_KEY_DOUBLE_CLICK:
        {
            hal_HstSendEvent(SYS_EVENT, 0x08231435);
            hal_HstSendEvent(SYS_EVENT, (u_int32)hfp_device);
            if(hfp_device != NULL)
            {
                hal_HstSendEvent(SYS_EVENT, hfp_device->hfp_call_status);
                if(hfp_device->hfp_call_status == HFP_CALL_STATUS_INCOMING)
                {
                    media_PlayInternalAudio(GUI_AUDIO_REJECT_CALL, 1, FALSE);
                    app_hfp_call_terminal_req(&hfp_device->hfp_addr);
                    ret = 1;
                }
                else if(hfp_adp_info != NULL && hfp_adp_info->hfp_connect_index > 0)
                {
                    media_PlayInternalAudio(GUI_AUDIO_CALL_WARNING, 1, FALSE);
                    COS_Sleep(40);
                    app_hfp_call_req(&hfp_device->hfp_addr, HFP_CALL_REQ_REDIAL, NULL);
                    ret = 1;
                }
            }
        }
        break;
        
#if APP_SUPPORT_VOICE_RECOGNITION == 1
        case AP_KEY_PLAY|AP_KEY_HOLD:
        {
            static u_int8 delay_hold_counter = 0;
            hal_HstSendEvent(SYS_EVENT, 0x05041721);
            hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());

            if(hfp_device != NULL)
            {
                if(hfp_device->hfp_call_status == HFP_CALL_STATUS_INCOMING)
                {
                    if(MESSAGE_IsHold())
                    {
                        media_PlayInternalAudio(GUI_AUDIO_REJECT_CALL, 1, FALSE);
                        app_hfp_call_terminal_req(&hfp_device->hfp_addr);
                        ret = 1;
                    }
                }
                else if(app_get_hfp_state() == HFP_CONNECTED)
                {
                    if(MESSAGE_IsHold())
                    {
                        hal_HstSendEvent(BOOT_EVENT, 0x88998899);
                        app_set_hfp_state(HFP_VOICE_RECOGNITION);
                        SetProtocolEventHandler(app_hfp_poweroff, EV_AUDIO_INTERNAL_PLAY_FINISH_IND);
                        media_PlayInternalAudio(GUI_AUDIO_BT_RECOGNITION, 1, FALSE);
                        delay_hold_counter = 0;
                        ret = 1;
                    }
                }
                else if(app_get_hfp_state() == HFP_VOICE_RECOGNITION)
                {  
                    if(app_hfp_play_finished ==TRUE)
                    {	
                        delay_hold_counter++;
                        if(delay_hold_counter == 3)
                        {
                            app_hfp_play_finished = FALSE;  
                            hal_HstSendEvent(BOOT_EVENT, 0x8899889b);
                            app_set_hfp_state(HFP_CONNECTED);
                            MESSAGE_HoldFlagClr();
                        }
                        ret = 0;  
                    }
                }
            }
        }
        break;
#endif

#if APP_SUPPORT_BTBATTERY==1
        case AP_MSG_RTC:
        {            
            batt = GetBattery();
            hfp_batt = batt/10;
            if(hfp_batt>=10)
            {
                hfp_batt = 9;
            }
            
            if(batt_old != batt)
            {
                batt_old = batt;
                app_hfp_send_battery((u_int8)hfp_batt);
            }
        }
        break;
#endif
        default:
        {
            if(app_get_hfp_state() > HFP_CONNECTED)
            {
                ret = 1;
            }
        }
        break;
    }
    
    return ret;
}

void app_hfp_msg_free(RDABT_HFP_MSG_INFO *ptr)
{
    COS_EVENT event = {0};

    event.nEventId = RDABT_HFP_FREE_MSG_REQ;
    event.nParam1 = (u_int32)ptr;

    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_msg_handle(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case RDABT_HFP_CONNECTED_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;
            APP_HFP_DEVICE_INFO *device_info = app_find_hfp_device(hfp_msg->hfp_addr);
            APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

            hal_HstSendEvent(SYS_EVENT, 0x20170614);
            hal_HstSendEvent(1, 0x04201702);
            hal_HstSendEvent(1, hfp_msg->msg_param1);
            if(hfp_msg->msg_param1 == RDABT_NOERROR && device_info != NULL)
            {
                app_cancel_create_hfp_connection(hfp_msg->hfp_addr);
                if (!IS_BT_MODE() || app_get_mgr_state() <= MGR_CLOSE_PENDING)
                {
                    if(device_info != NULL)
                    {            
                        app_disconnect_hfp(&device_info->hfp_addr);
                    }
                    app_hfp_msg_free(hfp_msg);
                    break;
                }
                else
                {
                    hfp_adp_info->last_addr = hfp_msg->hfp_addr;
                }
                
#if APP_SUPPORT_TEST_BOX==0
                mmi_timer_create(30, app_hfp_register_battery_timer, device_info, COS_TIMER_MODE_SINGLE);	
#endif

#ifdef BT_SET_NREC_0
                app_hfp_send_nrec(&hfp_msg->hfp_addr);//nrec = 0;
#endif

#if APP_SUPPORT_A2DP==1	
                {
                    app_create_a2dp_connection(100, hfp_msg->hfp_addr);
#if APP_SUPPORT_SOUNDBAR == 0
                    if(a2dp_connect_role == 0)
                    {
                        u_int32 device_index = 0;
                        device_index = app_bt_vars->last_device;
						app_cancel_reconnect();
                        app_bt_connect_retry((void *)device_index);
						app_bt_connect_second_device((void *)device_index);
                    }
#endif
                }
#endif
            }
            app_hfp_msg_free(hfp_msg);
        }
        break; 
        
        case RDABT_HFP_ACL_DISCONNECT_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;

            hal_HstSendEvent(SYS_EVENT, 0x20170613);

            app_cancel_create_hfp_connection(hfp_msg->hfp_addr);
#if APP_SUPPORT_A2DP == 1
            app_cancel_create_a2dp_connection(hfp_msg->hfp_addr);
#endif
            app_hfp_update_last_device();
            app_hfp_msg_free(hfp_msg);
            APP_Wakeup();
        }
        break;   
        
        case RDABT_HFP_CALL_STATUS_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;
            u_int32 call_status = hfp_msg->msg_param1;
            APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
            
            hal_HstSendEvent(1, 0x20161637);
            hal_HstSendEvent(1, hfp_msg->msg_param1);
            hal_HstSendEvent(1, hfp_msg->hfp_addr.bytes[0]);
            MESSAGE_SetEvent(AP_MSG_WAKE_REQ);
            APP_Wakeup();

            if(!(IS_BT_MODE()))
            {
                if(call_status != HFP_CALL_STATUS_NONE && call_status != HFP_CALL_STATUS_END)
                {
                    SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_BT_PENDING);
                }
            }

#if APP_SUPPORT_A2DP == 1
            if(call_status == HFP_CALL_STATUS_END || call_status == HFP_CALL_STATUS_NONE)
            {   
#if APP_SUPPORT_NUMBER_VOICE==1            
                app_start_play_tel_number(NULL);
#endif
                {
                    APP_HFP_ADP_INFO *hfp_adp_info_tmp = app_get_hfp_adp_info();
                    hal_HstSendEvent(SYS_EVENT, 0x201509aa);
                    hal_HstSendEvent(SYS_EVENT, hfp_adp_info->sco_handle );
                    
                    if(hfp_adp_info_tmp != NULL && hfp_adp_info_tmp->sco_handle != 0)
                    {
                        app_sco_disconnect_req(hfp_adp_info->sco_handle);
                    }
                }
            }
#endif
            if(call_status == HFP_CALL_STATUS_ACTIVE || call_status == HFP_CALL_STATUS_OUTGOING||call_status == HFP_CALL_STATUS_ALERT)
            {
#if APP_SUPPORT_NUMBER_VOICE==0
                media_StopInternalAudio();
#endif
                hal_HstSendEvent(1, 0x2016163c);
                hal_HstSendEvent(1, app_get_a2dp_state());
#if APP_SUPPORT_A2DP == 1 
                if( app_get_a2dp_state() != A2DP_PLAY)
#endif
                    app_hfp_play_sco();
                hfp_adp_info->last_addr = hfp_msg->hfp_addr;
            }
            else if(call_status == HFP_CALL_STATUS_INCOMING)
            // Other state not play phone number, but need clear internal state of player
            {
                hfp_adp_info->last_addr = hfp_msg->hfp_addr;
#if APP_SUPPORT_A2DP == 1            
                app_a2dp_stop_sbc();
#endif
#if APP_SUPPORT_NUMBER_VOICE==0             
                media_PlayInternalAudio(GUI_AUDIO_BT_RING, 1, FALSE);              
#endif 
            }      
            app_hfp_msg_free(hfp_msg);
        }
        break; 
        
        case RDABT_HFP_RING_IND_MSG:
        {
#if APP_SUPPORT_NUMBER_VOICE==0
            media_PlayInternalAudio(GUI_AUDIO_BT_RING, 1, FALSE);
#endif
        }            
        break; 
        
        case RDABT_HFP_ADP_INFO_MSG:
        {
            APP_HFP_ADP_INFO *hfp_adp_info = (APP_HFP_ADP_INFO *)ev->nParam1;
            if(hfp_adp_info != NULL)
            {
                s_hfp_adp_info = hfp_adp_info;
            }
        }
        break;    
        
        case RDABT_HFP_PB_DATA_MSG:
        {
            deal_pbap_hf_data((u_int8 *)ev->nParam1, 0);
        }
        break;    
        
#if APP_SUPPORT_BT_REMOTE_VOL==1        
        case RDABT_HFP_SPEAKER_GAIN_IND_MSG:
        {
            hal_HstSendEvent(APP_EVENT, 0x06231343);
            hal_HstSendEvent(APP_EVENT, ev->nParam1);
            app_bt_vars->volume_hfp = (int8) ap_convert_volume(15, AUD_MAX_LEVEL, (UINT16) ev->nParam1);

            if(app_bt_vars->volume_hfp == 0)
            {
                app_bt_vars->volume_hfp = 1;
            }

            mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_hfp);
        }
        break;
#endif

        case RDABT_HFP_DATA_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *hfp_msg = (RDABT_HFP_MSG_INFO *)ev->nParam1;
            app_msg_info.hfp_addr = hfp_msg->hfp_addr;

            memcpy(app_msg_info.msg_buf, hfp_msg->msg_buf, HFP_MAX_MSG_BUF);
            hal_HstSendEvent(1, 0x06231107);
            hal_HstSendEvent(1, app_msg_info.msg_buf[0]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[1]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[2]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[3]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[4]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[5]);
            hal_HstSendEvent(1, app_msg_info.msg_buf[6]);
            
#if APP_SUPPORT_TEST_BOX==0            
            if(strncmp(app_msg_info.msg_buf+2, "+XAPL=", 6)==0)
            {
                hal_HstSendEvent(1, 0x06231342);
                mmi_timer_create(10, app_hfp_send_battery_delay_timer, NULL, COS_TIMER_MODE_SINGLE);
                //app_hfp_send_battery(hfp_batt);
            }
            else if(strncmp(app_msg_info.msg_buf+2, "+CDN", 4)==0)
            {
                hal_HstSendEvent(1, 0x05161548);
                memset(app_bt_vars->local_name, 0, MAX_BT_DEVICE_NAME);
                memcpy(app_bt_vars->local_name, &app_msg_info.msg_buf[6], strlen(&app_msg_info.msg_buf[6])-1);
                app_mgr_store();
            }
#endif  

#if APP_SUPPORT_NUMBER_VOICE==1
            if(strncmp(app_msg_info.msg_buf, "+TEL=", 5) == 0)
            {
                hal_HstSendEvent(SYS_EVENT, 0x05041346);
                app_start_play_tel_number(&app_msg_info.msg_buf[6]);
            }
#endif
            app_hfp_msg_free(hfp_msg);
        }
        break;        
    }
}

#if APP_SUPPORT_NUMBER_VOICE==1
#endif


#else
void app_connect_hfp(t_bdaddr *pAddr)
{

}
void app_disconnect_hfp(t_bdaddr *pAddr)
{

}

#endif

