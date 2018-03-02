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

#include "resource_id.h"
#include "ap_gui.h"
#include "ap_mgr.h"
#include "ap_hid.h"
#ifndef WIN32
#include "hal_overlay.h"
#endif
#include "ap_media.h"
#include "ap_message.h"

#if APP_SUPPORT_TEST_BOX == 1
#include "ap_bt_test.h"
#endif

#if APP_SUPPORT_BLUETOOTH==1

static u_int8 s_mgr_state;
APP_BT_MANAGER *app_bt_vars = NULL;
APP_INQUIRY_DATA app_inquiry_data = {0}; 
APP_KEY_MSG_INFO key_info;
static u_int32 app_bt_intercept_switch_mode = RESULT_NULL;
static uint8 g_VolAudioMask = 0;//0=none, 1=internal audio finished, 2=key up

APP_BT_GET_RSSI_HANDLER_T app_bt_get_rssi_hdl = NULL;
t_bdaddr app_clean_addr = {{0}};

#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
static t_bdaddr * app_addr_need_getnameTable = NULL;
static UINT8 NULLADDR[MAX_BT_DEVICE_NAME] = "";
APP_INQUIRY_DATA app_store_inquiry_data = {0}; 
#define APP_INQUIRY_TIME  300  //60S  user can change the total inquirying time 
#endif


#if APP_SUPPORT_HID == 1
extern RDABT_Hid_Info app_hid_info;
#endif

#if APP_SUPPORT_HID_MODE==1
extern UINT8 g_current_BT_module;    
#endif

#if APP_SUPPORT_BLE_BR==1
void app_switch_br_le_mode(void *pArgs);
#endif

int find_str(const char *src_str, const char *spec_str, int str_len )
{
    int index = 0;
    int length = strlen(src_str);

    if(src_str == NULL || spec_str == NULL)
        return -1;
    
    if(str_len == 0)
        return -1;

    while(index < length)
    {
        if(!strncmp(&src_str[index], &spec_str[0], str_len))
            return index;
        else index++;
    }
    return -2;
}


int UTF8_to_Unicode(char *dst, const char *src)
{
    int i = 0, unicode = 0, ii, iii;
    int codeLen = 0;
    BOOL is_chinese = FALSE;
    while ( *src )
    {
        //1. UTF-8 ---> Unicode
        if(0 == (src[0] & 0x80))
        {
            // 单字节
            codeLen = 1;
            unicode = src[0];
        }
        else if(0xC0 == (src[0] & 0xE0) && 0x80 == (src[1] & 0xC0))
        {// 双字节
            codeLen = 2;
            unicode = (int)((((int)src[0] & 0x001F) << 6) | ((int)src[1] & 0x003F));
        }
        else if(0xE0 == (src[0] & 0xF0) && 0x80 == (src[1] & 0xC0) && 0x80 == (src[2] & 0xC0))
        {// 三字节
            codeLen = 3;
            is_chinese = TRUE;
            ii = (((int)src[0] & 0x000F) << 12);
            iii = (((int)src[1] & 0x003F) << 6);
            unicode = ii|iii|((int)src[2] & 0x003F);
            unicode = (int)((((int)src[0] & 0x000F) << 12) | (((int)src[1] & 0x003F) << 6) | ((int)src[2] & 0x003F));
        }
        else if(0xF0 == (src[0] & 0xF0) && 0x80 == (src[1] & 0xC0) && 0x80 == (src[2] & 0xC0) && 0x80 == (src[3] & 0xC0))
        {// 四字节
            codeLen = 4;
            unicode = (((int)(src[0] & 0x07)) << 18) | (((int)(src[1] & 0x3F)) << 12) | (((int)(src[2] & 0x3F)) << 6) | (src[3] & 0x3F);
        }
        else
        {
            break;
        }
        
        src += codeLen;
        if (unicode < 0x80)
        {
            if (i == 0 && unicode == 0x20)
            {
                continue;
            }
        }
        
        i += 2;
        *dst++ = (char)(unicode&0xff);
        *dst++ = (char)((unicode>>8)&0xff);
    } // end while
    
    *dst++ = 0;
    *dst = 0;
    //if(!is_chinese)
    //    i = 0;
    
    return i;
}

void app_set_bt_intercept_switch_mode_result(u_int32 next_mode)
{
    hal_HstSendEvent(SYS_EVENT, 0x05121459);
    hal_HstSendEvent(SYS_EVENT, next_mode);
    app_bt_intercept_switch_mode = next_mode;
}

u_int32 app_get_bt_intercept_switch_mode_result(void)
{
    return app_bt_intercept_switch_mode;
}

u_int8 app_get_mgr_state(void)
{
    return s_mgr_state;
}

void app_set_mgr_state(u_int8 state)
{
    hal_HstSendEvent(SYS_EVENT, 0x05121455);
    hal_HstSendEvent(SYS_EVENT, state);
    s_mgr_state = state;
    if(RESULT_BT == g_current_module)
    {
        if(s_mgr_state == MGR_ACTIVED)
        {
            LED_SetBgPattern(GUI_LED_BT_IDLE);//red
        }
        else if(s_mgr_state == MGR_CONNECTED)
        {
            LED_SetBgPattern(GUI_LED_BT_CONNECT);//blue
        }
    }
#if APP_SUPPORT_MENU==1
    SendAppEvent(EV_UI_FW_REDRAW, state);
#endif
}
/***********************************************************************
	play internal audio when Sbc data is playing or Sco data is playing

************************************************************************/
void app_mgr_PlayInternalAudio(u_int8 audio_ID)
{
#if APP_SUPPORT_HFP== 1
    if(app_get_hfp_state() > HFP_CONNECTED)
    {
        app_set_hfp_state(app_get_hfp_state()|0x80);
    }
    else
#endif
    {
#if APP_SUPPORT_A2DP== 1
        if(app_get_a2dp_state() > A2DP_PLAY_AUDIO)
        {
            app_set_a2dp_state(A2DP_PLAY_AUDIO);
        }
#endif
    }

    g_VolAudioMask = 2;
    mediaSendBTCommand(MC_STOP, 0);
    MMC_AudioCleanA2dpData();
    media_PlayInternalAudio(audio_ID, 1, 0);
}

void app_mgr_audio_play(void)
{
#if APP_SUPPORT_HFP == 1
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    hal_HstSendEvent(SYS_EVENT, 0x20170803);
    hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());
    
    if(app_get_hfp_state() > HFP_CONNECTED)
    {
        if((app_get_hfp_state()&HFP_AUDIO_STATE_ON_MASK) == 0)
        {
#if APP_SUPPORT_NUMBER_VOICE == 1
            hal_HstSendEvent(SYS_EVENT, 0x20170714);

            app_start_play_tel_number(NULL);
#endif
        }
        else
        {
            if (g_VolAudioMask == 2)
            {
                g_VolAudioMask = 0;
                app_set_hfp_state(app_get_hfp_state()&HFP_AUDIO_STATE_OFF_MASK);
                if(app_get_hfp_state() == HFP_PLAY_SCO)
                {
                    if(hfp_adp_info == NULL)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0xeeee0004);
                        return ;
                    }
                    
                    mediaSendBTCommand(MC_STOP, 0);
                    mediaSendBTCommand(MC_PLAY_SCO, hfp_adp_info->sco_handle);
                    mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_hfp);
                }
            }
            else
            {
                g_VolAudioMask = 1;
                app_set_hfp_state(app_get_hfp_state()&HFP_AUDIO_STATE_OFF_MASK);
            }
        }
    }
    else
#endif
    {
#if APP_SUPPORT_A2DP == 1
        if (app_check_a2dp_connection())
        {
            hal_HstSendEvent(1, 0x20170719);
            hal_HstSendEvent(1, app_get_a2dp_state());
            hal_HstSendEvent(1, g_VolAudioMask);
            
            if (app_get_a2dp_state() == A2DP_PLAY_AUDIO)
            {
                app_set_a2dp_state(A2DP_CONNECTED);
                app_a2dp_play_sbc_after();
            }
            else if (app_get_a2dp_state() == A2DP_PLAY_AUDIO_VOL)
            {
                app_set_a2dp_state(A2DP_PLAY_AUDIO);
                if (g_VolAudioMask == 2)
                {
                    g_VolAudioMask = 0;
                    app_set_a2dp_state(A2DP_CONNECTED);
                    app_a2dp_play_sbc_after();
                }
                else
                {
                    g_VolAudioMask = 1;
                }
            }

        }
        else
        {
            app_set_a2dp_state(A2DP_IDLE);
        }
#endif    
    }
}

void app_mgr_bt_state_machine(COS_EVENT *ev)
{
    BOOL ret = TRUE;

    switch(ev->nEventId)
    {
        case RDABT_STACK_INIT_MSG:
        {
            if(app_get_mgr_state() == MGR_ACTIVE_PENDING)
            {
                app_set_mgr_state(MGR_ACTIVED);
            }
        }
        break;
        
        case RDABT_ACL_CONNECT_IND_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;    
            if(msg_info->msg_param == RDABT_NOERROR)
            {
                app_set_mgr_state(MGR_CONNECTED);
            }
        }
        break;
        
        case RDABT_ACL_CONNECT_CNF_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;    
            if(msg_info->msg_param == RDABT_NOERROR)
            {
                app_set_mgr_state(MGR_CONNECTED);
            }
        }
        break;
        
        case RDABT_ACL_DISCONNECT_IND_MSG:
        {
            if(app_get_mgr_state() > MGR_ACTIVE_PENDING)
            {
                if(    
#if APP_SUPPORT_A2DP == 1
                !app_check_a2dp_connection()
                &&
#endif

#if APP_SUPPORT_AVRCP == 1
                !app_check_avrcp_connection()
                &&
#endif

#if APP_SUPPORT_HFP == 1
                !app_check_hfp_connection()
                &&
#endif
                ret)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x05090958);
                    app_set_mgr_state(MGR_ACTIVED);
                }
            }
        }
        break;
        
        case RDABT_USER_BT_CLOSE_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x05121448);
            hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
            //if(app_get_mgr_state() == MGR_CLOSE_PENDING)
            {
                app_set_mgr_state(MGR_CLOSED);
            }
#if APP_SUPPORT_A2DP == 1
            app_clean_a2dp_adp_info();
#endif

#if APP_SUPPORT_HFP == 1
            app_clean_hfp_adp_info();
#endif

#if APP_SUPPORT_AVRCP == 1
            app_clean_avrcp_adp_info();
#endif
        }
        break;
    }
}

#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
void app_mgr_free_inquiry_data_func(void)
{
    if(app_store_inquiry_data.app_inquiry_timer)
    {
        mmi_cancel_timer(app_store_inquiry_data.app_inquiry_timer);
    }
    
    if(app_store_inquiry_data.inquiry_device_list != NULL)
    {
        COS_Free(app_store_inquiry_data.inquiry_device_list);
    }
    
    if(app_addr_need_getnameTable)
    {
        COS_Free(app_addr_need_getnameTable);
        app_addr_need_getnameTable = NULL;
    }	
    app_store_inquiry_data.inquiry_device_list = NULL;
    app_store_inquiry_data.inquiry_device_count = 0;
    app_store_inquiry_data.max_inquiry_num = 0;
    app_store_inquiry_data.inquiry_length = 0;
    app_store_inquiry_data.is_LIAC= FALSE;
}
#endif

void app_mgr_deal_discover_cnf_func(void)
{
    COS_Free(app_inquiry_data.inquiry_device_list);
    app_inquiry_data.inquiry_device_list = NULL;
    app_inquiry_data.inquiry_device_count = 0;
    app_inquiry_data.max_inquiry_num = 0;
    app_set_mgr_state(MGR_ACTIVED);
    
#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
    {
        u_int8 i = 0;
        u_int8 maxCount = 0;
        hal_HstSendEvent(SYS_EVENT, 0x20170336);
        hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.inquiry_device_need_getName_count);
        hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.max_inquiry_num);
        hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.inquiry_device_count);

        if((app_store_inquiry_data.max_inquiry_num != 0)
            && app_store_inquiry_data.inquiry_device_need_getName_count != 0)
        {
            if(app_addr_need_getnameTable)
            {
                hal_HstSendEvent(SYS_EVENT, 0x20170340);
                hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.inquiry_device_need_getName_count-1);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[0]);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[1]);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[2]);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[3]);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[4]);
                hal_HstSendEvent(SYS_EVENT, app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1].bytes[5]);
                app_bt_get_remote_name_req(&app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1]);
                return;
            }
        }
        
        if((app_store_inquiry_data.max_inquiry_num != 0) 
            && (app_store_inquiry_data.inquiry_device_count < app_store_inquiry_data.max_inquiry_num))
        {
            //timeout not coming,need search again.
            app_bt_inquiry_req(app_store_inquiry_data.max_inquiry_num, app_store_inquiry_data.is_LIAC);
        }
        else
        {
            app_mgr_free_inquiry_data_func();
        }
    }
#endif
}

void app_bt_connect_second_device(void *pArgs)
{
#if APP_SUPPORT_RECONNECT_SECOND_DEVICE == 1
#if APP_SUPPORT_BT_AUTOVISIBLE == 0
#if APP_SUPPORT_ONLY_ONE_LINK == 0
    u_int32 i=(u_int32 )pArgs;
    u_int32 device_index = 0;
    if(app_bt_vars->last_device_two != 0xff 
        && app_bt_vars->last_device_two != i
        && app_bt_vars->last_device_two != app_bt_vars->last_device
#if	APP_SUPPORT_HFP == 1
        && app_find_hfp_device_state(app_bt_vars->device_list[i].addr) >=  HFP_ADP_STATE_CONNECTED
#endif
#if APP_SUPPORT_A2DP == 1
        && app_find_a2dp_device_state(app_bt_vars->device_list[i].addr) >= A2DP_ADP_STATE_CONNECTED
#endif
#if APP_SUPPORT_HID == 1
        && (app_hid_info.hid_state == HID_ADP_STATE_CONNECTED
#if APP_SUPPORT_HID_MODE == 1
            ||  g_current_BT_module == BT_MODE_HEADSET
#endif
            )
#endif
        && app_bt_vars->device_list[app_bt_vars->last_device_two].adp_state == MGR_ADP_STATE_IDLE
    )
    {
        app_bt_vars->times_num = 1;
        hal_HstSendEvent(SYS_EVENT, 0x20170724);
        device_index = app_bt_vars->last_device_two;
        app_bt_connect_retry((void *)device_index);
    }
#endif
#endif
#endif
}

void app_bt_connect_retry(void *pArgs)
{
    u_int32 i=(u_int32 )pArgs;
    t_bdaddr null_addr = {{0}};

    if(i == 0xff || !memcmp(app_bt_vars->device_list[i].addr.bytes, null_addr.bytes, SIZE_OF_BDADDR))
    {
        hal_HstSendEvent(SYS_EVENT, 0x20170719);
        return ;
    }
        
    app_bt_vars->auto_connect_timer = 0;
    //WANGXIN

    hal_HstSendEvent(SYS_EVENT, 0x20170516);
    hal_HstSendEvent(SYS_EVENT, i);
    hal_HstSendEvent(SYS_EVENT,app_bt_vars->last_device);
    hal_HstSendEvent(SYS_EVENT,app_bt_vars->last_device_two);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->bt_mode);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->device_list[i].adp_state);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->connect_retry_cnt);
#if APP_SUPPORT_HID_MODE==1 
    hal_HstSendEvent(SYS_EVENT, g_current_BT_module);
    hal_HstSendEvent(SYS_EVENT,app_bt_vars->last_hid_device);
#endif

    MESSAGE_SetEvent(AP_MSG_WAKE_REQ);
    APP_Wakeup();
    hal_HstSendEvent(1, 0x06032109);
    hal_HstSendEvent(SYS_EVENT,app_bt_vars->device_list[i].adp_state);

    if(app_bt_vars->bt_mode!=HOST_BR_MODE)
    {
        return ;
    }

    if(app_bt_vars->device_list[i].adp_state != MGR_ADP_STATE_IDLE)
    {
#if APP_SUPPORT_HID == 1
        if(app_hid_info.hid_state == HID_ADP_STATE_CONNECTED 
#if APP_SUPPORT_HID_MODE == 1
            || g_current_BT_module == BT_MODE_HEADSET
#else
            || app_bt_vars->not_support_hid_service == 1
#endif
        )
#endif
        {
            app_bt_vars->connect_retry_cnt = 0;
            hal_HstSendEvent(SYS_EVENT, 0x20170608);
            return;
        }
    }

    if(app_bt_vars->connect_retry_cnt < app_bt_vars->times_num)
    {
        app_bt_vars->connect_retry_cnt++;
#if APP_SUPPORT_A2DP==1
#if APP_SUPPORT_HID_MODE==1
        if(g_current_BT_module==BT_MODE_HEADSET)
#endif
            if(app_find_a2dp_device_state(app_bt_vars->device_list[i].addr) == A2DP_ADP_STATE_IDLE)
                app_connect_a2dp(&app_bt_vars->device_list[i].addr);
#else 

#if APP_SUPPORT_HFP == 1
#if APP_SUPPORT_HID_MODE==1
        if(g_current_BT_module==BT_MODE_HEADSET)
#endif
            if(app_find_hfp_device_state(app_bt_vars->device_list[i].addr) ==  HFP_ADP_STATE_IDLE)
                app_connect_hfp(&app_bt_vars->device_list[i].addr);
#endif
#endif

#if APP_SUPPORT_HID == 1
#if APP_SUPPORT_HID_MODE==1
        if(g_current_BT_module==BT_MODE_HID && app_bt_vars->last_hid_device == 0xFF)
            return;
#endif
        if(1
#if	APP_SUPPORT_HFP == 1
            && app_find_hfp_device_state(app_bt_vars->device_list[i].addr) >=  HFP_ADP_STATE_CONNECTED
#endif
#if APP_SUPPORT_A2DP == 1
            && app_find_a2dp_device_state(app_bt_vars->device_list[i].addr) >= A2DP_ADP_STATE_CONNECTED
#endif
        )
            app_hid_connect_req(&app_bt_vars->device_list[i].addr);
#endif

        if(!app_bt_vars->auto_connect_timer)
            app_bt_vars->auto_connect_timer = mmi_timer_create((50+app_bt_vars->connect_retry_cnt*app_bt_vars->times_value), app_bt_connect_retry, (void *)i, COS_TIMER_MODE_SINGLE);
    }
    else
    {
        app_bt_vars->connect_retry_cnt = 0;
        //memset(&auto_connect,0,sizeof(AutoConnect));
    }
}

void app_bt_connect(int count)
{    
#if APP_SUPPORT_SOUNDBAR == 1
    app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
    app_connect_soundbar_device();
    return;
#endif

    hal_HstSendEvent(1, 0x20160909);
    hal_HstSendEvent(1, app_bt_vars->last_device);
    hal_HstSendEvent(1, app_bt_vars->last_device_two);
#if APP_SUPPORT_HID_MODE==1
    hal_HstSendEvent(1, app_bt_vars->last_hid_device);
    hal_HstSendEvent(1, g_current_BT_module);
#endif

#if APP_SUPPORT_HID_MODE==1	
    if((g_current_BT_module==BT_MODE_HEADSET && app_bt_vars->last_device != 0xff && app_bt_vars->bt_mode==HOST_BR_MODE)
        ||(g_current_BT_module==BT_MODE_HID && app_bt_vars->last_hid_device != 0xff && app_bt_vars->bt_mode==HOST_BR_MODE))
#else
    if(app_bt_vars->last_device != 0xff && app_bt_vars->bt_mode==HOST_BR_MODE)
#endif
    {
#if APP_SUPPORT_HID_MODE==1	
        if(g_current_BT_module==BT_MODE_HID && app_bt_vars->device_list[app_bt_vars->last_hid_device].adp_state != MGR_ADP_STATE_IDLE)
        {
            hal_HstSendEvent(SYS_EVENT, 0x20170608);
            return;
        }
        else
#endif
        if(app_bt_vars->device_list[app_bt_vars->last_device].adp_state != MGR_ADP_STATE_IDLE)
        {
            hal_HstSendEvent(SYS_EVENT, 0x20170607);
            return;
        }

#if APP_SUPPORT_HID_MODE==1
        hal_HstSendEvent(SYS_EVENT, 0x06081112);
        hal_HstSendEvent(1, g_current_BT_module);
        hal_HstSendEvent(1, app_bt_vars->last_device);
        hal_HstSendEvent(1, app_bt_vars->last_hid_device);

        if(g_current_BT_module==BT_MODE_HEADSET)
        {
#if APP_SUPPORT_A2DP==1
            app_connect_a2dp(&app_bt_vars->device_list[app_bt_vars->last_device].addr);
#elif APP_SUPPORT_HFP == 1
            app_connect_hfp(&app_bt_vars->device_list[app_bt_vars->last_device].addr);
#endif
        }
        else if(g_current_BT_module==BT_MODE_HID)
            app_hid_connect_req(&app_bt_vars->device_list[app_bt_vars->last_hid_device].addr);

#else

#if APP_SUPPORT_A2DP==1
        app_connect_a2dp(&app_bt_vars->device_list[app_bt_vars->last_device].addr);
#elif APP_SUPPORT_HFP == 1
        app_connect_hfp(&app_bt_vars->device_list[app_bt_vars->last_device].addr);
#elif APP_SUPPORT_HID == 1
        app_hid_connect_req(&app_bt_vars->device_list[app_bt_vars->last_device].addr);
#endif   

#endif
        app_bt_vars->times_num = count;
        app_bt_vars->times_value = 40;
        hal_HstSendEvent(1, 0x20160911);
        hal_HstSendEvent(1, count);
        if(count>1)
        {
            u_int32 last_device;
#if APP_SUPPORT_HID_MODE==1
            if(g_current_BT_module==BT_MODE_HID)
                last_device = app_bt_vars->last_hid_device;
            else
#endif

            last_device = app_bt_vars->last_device;
            
		    hal_HstSendEvent(1, 0x20160910);
		    hal_HstSendEvent(1, app_bt_vars->auto_connect_timer);
			if(!app_bt_vars->auto_connect_timer)
        		app_bt_vars->auto_connect_timer = mmi_timer_create(100, app_bt_connect_retry, (void *)last_device, COS_TIMER_MODE_SINGLE);
		}
	}
	else
	{
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
		app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#endif
	}

}

void app_mgr_deal_discover_result_func(APP_MGR_DEVICE_INFO_BASE *dbEntry)
{
    if(dbEntry != NULL)
    {	
        app_inquiry_data.inquiry_device_count++;
        hal_HstSendEvent(SYS_EVENT, 0x25025023);
        
#if APP_SUPPORT_SOUNDBAR == 1
        if(dbEntry->addr.bytes[5] == 0x03 && dbEntry->addr.bytes[4] == 0x58 && dbEntry->addr.bytes[3] == 0x56)
        {
            soundbar_device.addr = dbEntry->addr;
            app_bt_inquiry_cancel_req();
            hal_HstSendEvent(SYS_EVENT, 0x25025022);
            COS_Sleep(1000);
            app_spp_connect_req(&soundbar_device.addr);
        }
#else
#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
        if(dbEntry != NULL)
        {
            u_int8 i = 0;
            app_trace(APP_BT_TRC,"app_mgr_deal_discover_result_func:\n\n\n  name:(%p)%s; \naddr:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x\n\n", 
            dbEntry->device_name,dbEntry->device_name,
            dbEntry->addr.bytes[5],
            dbEntry->addr.bytes[4],
            dbEntry->addr.bytes[3],
            dbEntry->addr.bytes[2],
            dbEntry->addr.bytes[1],
            dbEntry->addr.bytes[0]);
            hal_HstSendEvent(SYS_EVENT, 0x25025030);
            hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.inquiry_device_count);
            hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.max_inquiry_num);
            
            if(app_store_inquiry_data.inquiry_device_list != NULL)
            {
                if(memcmp(dbEntry->addr.bytes,NULLADDR,sizeof(t_bdaddr)) != 0)
                {
                    for(i = 0; i<app_store_inquiry_data.max_inquiry_num; i++)
                    {
                        if(memcmp(&(dbEntry->addr),&(app_store_inquiry_data.inquiry_device_list[i].addr),SIZE_OF_BDADDR) == 0)
                        {//find the same addr
                            return;
                        }
                    }

                    if(app_store_inquiry_data.inquiry_device_count < app_store_inquiry_data.max_inquiry_num)
                    {
                        memcpy(app_store_inquiry_data.inquiry_device_list[app_store_inquiry_data.inquiry_device_count].addr.bytes,
                        dbEntry->addr.bytes,SIZE_OF_BDADDR);
                        memcpy(app_store_inquiry_data.inquiry_device_list[app_store_inquiry_data.inquiry_device_count].device_name,
                        dbEntry->device_name,MAX_BT_DEVICE_NAME);
                        app_store_inquiry_data.inquiry_device_count++;

                        if(memcmp(dbEntry->device_name,NULLADDR,MAX_BT_DEVICE_NAME) != 0)//has name
                        {
                            hal_HstSendEvent(SYS_EVENT, 0x20170335); 
                        }
                        else // no name need get name after complite
                        {
                            if(app_addr_need_getnameTable != NULL)
                            {
                                for(i = 0; i<app_store_inquiry_data.max_inquiry_num; i++)
                                {
                                    if(memcmp(app_addr_need_getnameTable[i].bytes,dbEntry->addr.bytes,SIZE_OF_BDADDR) == 0)
                                        return;
                                }
                                memcpy(app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count].bytes,
                                dbEntry->addr.bytes,SIZE_OF_BDADDR);
                                app_store_inquiry_data.inquiry_device_need_getName_count++;
                                hal_HstSendEvent(SYS_EVENT, 0x20170337);
                                hal_HstSendEvent(SYS_EVENT, app_store_inquiry_data.inquiry_device_need_getName_count);
                            }
                        }
                    }
                }
                else
                {
                    return ;
                }
            }
        }
#endif
#endif
    }
}

void app_mgr_deal_namereq_result_func(APP_MGR_DEVICE_INFO_BASE *dbEntry)
{
    if(dbEntry != NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0x25025032);
#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
        app_store_inquiry_data.inquiry_device_need_getName_count--;

        if((memcmp(dbEntry->addr.bytes,NULLADDR,sizeof(t_bdaddr)) != 0)
            && (memcmp(dbEntry->device_name,NULLADDR,MAX_BT_DEVICE_NAME) != 0)
            && (app_store_inquiry_data.inquiry_device_list != NULL))
        {
            hal_HstSendEvent(SYS_EVENT, 0x20170338);
        }

        if(app_store_inquiry_data.inquiry_device_need_getName_count == 0)
        {
            if((app_store_inquiry_data.max_inquiry_num != 0) 
                && (app_store_inquiry_data.inquiry_device_count < app_store_inquiry_data.max_inquiry_num))
            {
                //timeout not coming,need search again.
                app_bt_inquiry_req(app_store_inquiry_data.max_inquiry_num, app_store_inquiry_data.is_LIAC);
            }
            else
            {
                app_mgr_free_inquiry_data_func();
            }
        }
        else
        {
            if(app_addr_need_getnameTable)
            {
                app_bt_get_remote_name_req(&app_addr_need_getnameTable[app_store_inquiry_data.inquiry_device_need_getName_count-1]);
                return;
            }
        }
#endif
    }
}

void app_bt_set_default_addr(void)
{
    UINT32 rand_lap;
	
    do{
        rand_lap = (rand()^(rand()<<8)) & 0xffffff;
    }while(rand_lap ==0 || ((rand_lap&0xffff00) == 0x9E8B00)); // check lap valid
    app_bt_vars->local_addr.bytes[0] = rand_lap&0xff;
    app_bt_vars->local_addr.bytes[1] = (rand_lap>>8)&0xff;
    app_bt_vars->local_addr.bytes[2] = (rand_lap>>16)&0xff;
	
    app_bt_vars->local_addr.bytes[3] = 0x56;
    app_bt_vars->local_addr.bytes[4] = 0x58;
    app_bt_vars->local_addr.bytes[5] = 0x00;
}

APP_BT_FLASH_STORE *flash_info_debug = 0;
void app_bt_init(void)
{
    TGT_BT_INFO_T bt_info;
    t_bdaddr dump_addr = {{0}};
    APP_BT_FLASH_STORE *flash_info = NULL;
    u_int8 i = 0, index=0;
    u_int8 max_device = tgt_GetBtMaxDeviceNum();
    
    app_bt_vars = (APP_BT_MANAGER *)COS_Malloc(sizeof(APP_BT_MANAGER));
    flash_info = (APP_BT_FLASH_STORE *)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
    app_bt_vars->device_list = (APP_MGR_DEVICE_INFO_BASE *)COS_Malloc(sizeof(APP_MGR_DEVICE_INFO_BASE)*max_device);
    
    if(flash_info == NULL || app_bt_vars == NULL || app_bt_vars->device_list == NULL)
    {
        hal_DbgAssert("No bt ram");
    }
    
    hal_HstSendEvent(1, 0x04281349);
    hal_HstSendEvent(1, (u_int32)flash_info);
    hal_HstSendEvent(1, flash_info->magic);

#ifdef WIN32
    memset(&bt_info, 0,sizeof(bt_info));
#else
    tgt_GetBtInfo(&bt_info);
#endif
    
    if(flash_info->magic != MAGIC_BLUETOOTH)
    {
        memset(flash_info, 0, sizeof(APP_BT_FLASH_STORE));
        flash_info->last_device = 0xff;
        flash_info->last_device_two = 0xff;
#if APP_SUPPORT_HID_MODE==1
        flash_info->last_hid_device = 0xff;
#endif
        flash_info->magic = MAGIC_BLUETOOTH;    
        /* invalid bdaddr, get random address */
        memset(dump_addr.bytes, 0xff, SIZE_OF_BDADDR);

        if(!(memcmp(bt_info.DEVICE_ADDR, dump_addr.bytes, SIZE_OF_BDADDR)))
        {
            app_bt_set_default_addr();
        }
        else 
        {
            memcpy(app_bt_vars->local_addr.bytes, bt_info.DEVICE_ADDR, SIZE_OF_BDADDR);
        }

        if(bt_info.DEVICENAME[0] == 0xff||bt_info.DEVICENAME[0] == 0x0)
        {
            memcpy(app_bt_vars->local_name, g_btconfig.local_name, MAX_BT_DEVICE_NAME); // bluetooth name
        }
        else
        {
            memcpy(app_bt_vars->local_name, bt_info.DEVICENAME, MAX_BT_DEVICE_NAME); // bluetooth name
        }
    }
    else 
    {
#if APP_SUPPORT_HID_MODE==1
        if(g_current_BT_module == BT_MODE_HID)
            index=max_device;
        else
            index=0;
#endif

        for(i=0; i < max_device; i++)
        {
            app_bt_vars->device_list[i].addr = flash_info->device_list[i+index].addr;
            app_bt_vars->device_list[i].cod = flash_info->device_list[i+index].cod;
            app_bt_vars->device_list[i].key_type = flash_info->device_list[i+index].key_type;
            memcpy(app_bt_vars->device_list[i].device_name, flash_info->device_list[i+index].device_name, MAX_BT_DEVICE_NAME);
            memcpy(app_bt_vars->device_list[i].link_key, flash_info->device_list[i+index].link_key, SIZE_OF_LINKKEY);
        }
        app_bt_vars->local_addr = flash_info->local_addr;
        memcpy(app_bt_vars->local_name, flash_info->local_name, MAX_BT_DEVICE_NAME);
        flash_info_debug = flash_info;
    }
    
#ifndef WIN32   

#if APP_SUPPORT_HID_MODE==1
    hal_HstSendEvent(SYS_EVENT, 0x06060012);
    hal_HstSendEvent(SYS_EVENT, g_current_BT_module);
    hal_HstSendEvent(SYS_EVENT, flash_info->last_hid_device);
    hal_HstSendEvent(APP_EVENT, app_bt_vars->local_addr.bytes[0]);
        
    if(g_current_BT_module == BT_MODE_HID)
    {
        u_int8 name_len=0;
        name_len = strlen(app_bt_vars->local_name);

        app_bt_vars->local_addr.bytes[0]++;

        hal_HstSendEvent(SYS_EVENT, name_len);
        hal_HstSendEvent(SYS_EVENT, app_bt_vars->local_addr.bytes[0]);
        if(name_len>=MAX_BT_DEVICE_NAME-4)
        {
            app_bt_vars->local_name[27]='-';
            app_bt_vars->local_name[28]='h';
            app_bt_vars->local_name[29]='i';
            app_bt_vars->local_name[30]='d';
            app_bt_vars->local_name[31]='\0';
        }
        else
        {
            app_bt_vars->local_name[name_len]='-';
            app_bt_vars->local_name[name_len+1]='h';
            app_bt_vars->local_name[name_len+2]='i';
            app_bt_vars->local_name[name_len+3]='d';
            app_bt_vars->local_name[name_len+4]='\0';
        }
    }
#endif

    memcpy(app_bt_vars->le_addr.bytes, bt_info.DEVICE_ADDR, SIZE_OF_BDADDR);
    app_bt_vars->le_addr.bytes[0]++;
    app_bt_vars->le_addr.bytes[1]++;
    app_bt_vars->le_addr.bytes[2]++;
    
    memcpy(app_bt_vars->password, g_btconfig.password, SIZE_OF_LINKKEY);
#else
    {
        strcpy(app_bt_vars->local_name, "RDA MP3 BT"); // bluetooth name
        app_bt_set_default_addr();	
        strcpy(app_bt_vars->password, "0000");         // bluetooth password
    }
#endif

    app_bt_vars->magic = MAGIC_BLUETOOTH;
    app_bt_vars->max_device_num = max_device;
    app_bt_vars->volume_a2dp = app_bt_vars->volume_hfp = app_bt_vars->volume = BT_VOLUME_DEFAULT;
    app_bt_vars->last_device = flash_info->last_device;
    app_bt_vars->last_device_two = flash_info->last_device_two;
    app_bt_vars->bt_mode = tgt_GetBtDefaultMode();
#if APP_SUPPORT_SOUNDBAR == 1
    app_set_soundbar_last_phone_device(app_bt_vars->last_device);
#endif

    if(boot_get_testmode())
    {
        app_bt_vars->connect_index = 0xff;
    }
	
	
#if APP_SUPPORT_HID_MODE==1
    app_bt_vars->last_hid_device = flash_info->last_hid_device;
    if(g_current_BT_module==BT_MODE_HEADSET)
        app_bt_vars->device_class = 0x240404;
    else if(g_current_BT_module==BT_MODE_HID)
        app_bt_vars->device_class = 0x2540;   
#else
    app_bt_vars->device_class = BT_DEVICE_OF_CLASS;
#if APP_SUPPORT_HID == 1
    app_bt_vars->not_support_hid_service = flash_info->not_support_hid_service;
    if(app_bt_vars->not_support_hid_service == 1)
        app_bt_vars->device_class = 0x240404;
    else 
        app_bt_vars->device_class = 0x2540; 
#endif
#endif

}

u_int8 *app_bt_get_local_name(void)
{
    if (app_bt_vars == NULL)
    {
        app_bt_init();
    }

    return app_bt_vars->local_name;
}

void app_bt_set_local_name(u_int8 *name)
{
    if(name == NULL)
    {
        return ;
    }
    
    if (app_bt_vars == NULL)
    {
        app_bt_init();
    }

    memset(app_bt_vars->local_name, 0, MAX_BT_DEVICE_NAME);
    strncpy(app_bt_vars->local_name, name, MAX_BT_DEVICE_NAME);

    app_mgr_store();
}

void app_bt_active_req(void)
{
    COS_EVENT ev = {0};

    if(app_bt_vars == NULL)
    {
        app_bt_init();
    }

    hal_HstSendEvent(SYS_EVENT, 0x04191510);
    hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
    mediaSendBTCommand(MC_SETVOLUME, BT_VOLUME_DEFAULT);

    switch(app_get_mgr_state())
    {
        case MGR_CLOSED:
        {
            ev.nEventId = RDABT_USER_BT_INIT_REQ;
            ev.nParam1 = 0;  
            COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            app_set_mgr_state(MGR_ACTIVE_PENDING);
        }
        break;
    }
}

void app_bt_close_req(void)
{
    COS_EVENT ev = {0};

    hal_HstSendEvent(SYS_EVENT, 0x04200945);
    hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
    ClearProtocolEventHandler(RDABT_ACL_DISCONNECT_IND_MSG);

    if(app_bt_vars == NULL || app_get_mgr_state() == MGR_CLOSED || app_get_mgr_state() == MGR_CLOSE_PENDING || app_get_mgr_state() == MGR_ACTIVE_PENDING)
    {
        return ;
    }

    app_exit_timer_check();
    /*
#if APP_SUPPORT_HID_MODE==1  	
    if(g_current_BT_module == BT_MODE_HID)
    {
    u_int8 name_len=0;
    name_len = strlen(app_bt_vars->local_name);

    hal_HstSendEvent(SYS_EVENT, 0x06081188);
    hal_HstSendEvent(SYS_EVENT, name_len);
    if(name_len>4)
    {
    app_bt_vars->local_name[name_len-4]='\0';
    app_bt_vars->local_name[name_len-3]=0;
    app_bt_vars->local_name[name_len-2]=0;
    app_bt_vars->local_name[name_len-1]=0;
    }
    app_bt_vars->local_addr.bytes[0]--;
    }
#endif
    */
    app_mgr_store();
    COS_Sleep(30);
    app_set_mgr_state(MGR_CLOSE_PENDING);
    ev.nEventId = RDABT_USER_BT_CLOSE_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_get_remote_name_req(t_bdaddr *pAddr)
{
    COS_EVENT ev = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170321);
    ev.nEventId = RDABT_GET_REMOTE_NAME_REQ;
    ev.nParam1 = (u_int32)pAddr;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_get_rssi_req(t_bdaddr *pAddr)
{
    COS_EVENT ev = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170325);
    ev.nEventId = RDABT_GET_RSSI_REQ;
    ev.nParam1 = (u_int32)pAddr;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_get_rssi_req2(APP_BT_GET_RSSI_HANDLER_T cb, t_bdaddr *pAddr)
{
    app_bt_get_rssi_req(pAddr);
    app_bt_get_rssi_hdl = cb;
}

void app_bt_change_name_req(char *name_ptr)
{
    COS_EVENT ev = {0};

    if(app_bt_vars == NULL)
    {
        return ;
    }

    strncpy(app_bt_vars->local_name, name_ptr, (strlen(name_ptr) ? MAX_BT_DEVICE_NAME:strlen(name_ptr)));
    ev.nEventId = RDABT_USER_BT_CHANGE_NAME_REQ;
    ev.nParam1 = (u_int32)app_bt_vars->local_name;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_set_bt_connectable_req(u_int8 is_connectable)
{	
    COS_EVENT ev = {0};
    hal_HstSendEvent(APP_EVENT, 0x20150011);
    hal_HstSendEvent(APP_EVENT, is_connectable);
    hal_HstSendEvent(APP_EVENT, g_current_module);

    if (g_current_module!=RESULT_BT)
    {
        is_connectable =FALSE;
    }


    ev.nEventId = RDABT_USER_BT_SET_CONNECTABLE_REQ;
    ev.nParam1 = is_connectable;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_set_bt_visible_req(app_visiable_state is_visiable)
{	
    COS_EVENT ev = {0};
    hal_HstSendEvent(APP_EVENT, 0x20150000);
    hal_HstSendEvent(APP_EVENT, is_visiable);
    hal_HstSendEvent(APP_EVENT, g_current_module);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->connect_index);

 
    if(
#if APP_SUPPORT_ONLY_ONE_LINK==1   
        app_bt_vars->connect_index == 1
#else
        app_bt_vars->connect_index == 2
#endif
    )
    {
        is_visiable=MGR_SET_NO_VISIABLE;
    }
    
#if APP_SUPPORT_A2DP==1
    if(is_visiable != MGR_SET_NO_VISIABLE && app_a2dp_is_active())
    {
        is_visiable = MGR_SET_NO_VISIABLE;
    }
#endif

    if(RESULT_BT == g_current_module && app_get_mgr_state() <= MGR_SCAN)//psc add 7/4  19:27
    {
        if(is_visiable == MGR_SET_ALL_VISIABLE)
        {
            LED_SetBgPattern(GUI_LED_BT_SCAN);//blue
            app_set_mgr_state(MGR_SCAN);
        }
        else
        {
            LED_SetBgPattern(GUI_LED_BT_MODE);//blue
        }
    }

    if (g_current_module!=RESULT_BT)
    {
        is_visiable=MGR_SET_NO_VISIABLE;
    }

    hal_HstSendEvent(APP_EVENT, 0x20151111);
    hal_HstSendEvent(APP_EVENT, is_visiable);
    
    ev.nEventId = RDABT_USER_BT_SET_VISIBLE_REQ;
    ev.nParam1 = is_visiable;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#ifdef WIN32
UINT32 g_halLpsForceNoSleep=0;
#else
extern UINT32 g_halLpsForceNoSleep;
#endif

static UINT8 wakelock_timer_handler = 0;
static UINT32 sleep_pre_state = 0;
static void wakelock_cb(void *pArgs)
{
    wakelock_timer_handler = 0;
    hal_HstSendEvent(SYS_EVENT, 0x04261943);
    hal_HstSendEvent(SYS_EVENT, sleep_pre_state);
    hal_HstSendEvent(SYS_EVENT, g_halLpsForceNoSleep);
    g_halLpsForceNoSleep = sleep_pre_state;	
}

void wakelock(UINT32 timeoutTenths)
{
    if(wakelock_timer_handler == 0)
    {
        sleep_pre_state = g_halLpsForceNoSleep;
        g_halLpsForceNoSleep = 1;
        wakelock_timer_handler = mmi_timer_create(timeoutTenths, wakelock_cb, NULL, COS_TIMER_MODE_SINGLE);
    }
    else
    {
        mmi_cancel_timer(wakelock_timer_handler);
        wakelock_timer_handler = mmi_timer_create(timeoutTenths, wakelock_cb, NULL, COS_TIMER_MODE_SINGLE);
    }
}

void app_bt_mode_switch(u_int8 mode)
{
    COS_EVENT ev = {0};

    if(app_bt_vars == NULL)
    {
        return ;
    }

    ev.nEventId = RDABT_USER_BT_MODE_SWITCH_REQ;
    ev.nParam1 = mode;
    app_bt_vars->bt_mode = mode;
    hal_HstSendEvent(SYS_EVENT, 0x20161056);
    hal_HstSendEvent(SYS_EVENT, mode);

#if APP_SUPPORT_BLE_BR==1
#if APP_SUPPORT_A2DP == 1
    app_clean_a2dp_adp_info();
#endif

#if APP_SUPPORT_HFP == 1
    app_clean_hfp_adp_info();
#endif

#if APP_SUPPORT_AVRCP == 1
    app_clean_avrcp_adp_info();
#endif

    wakelock(3);
#endif
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_bt_inquiry_req(u_int8 max_result, u_int8 is_LIAC)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_USER_BT_INQUIRY_REQ;
    ev.nParam1 = (u_int32)&app_inquiry_data;
    
    if(app_get_mgr_state() == MGR_CONNECTED || app_get_mgr_state() == MGR_INQUIRY_PENDNG
#if APP_SUPPORT_SOUNDBAR == 1
        || Soundbar_App_Get_Role_Status() < SOUNDBAR_NOROLE
#endif
    )
    {
        hal_HstSendEvent(SYS_EVENT, 0x20170502);
        return;
    }

#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
    if(app_addr_need_getnameTable == NULL && app_store_inquiry_data.app_inquiry_timer == NULL)
    {
        app_addr_need_getnameTable = (t_bdaddr *)COS_Malloc(sizeof(t_bdaddr)*max_result);
        memset(app_addr_need_getnameTable,0,sizeof(t_bdaddr)*max_result);
    }
    
    if(app_store_inquiry_data.inquiry_device_list == NULL && app_store_inquiry_data.app_inquiry_timer == NULL)
    {
        app_store_inquiry_data.inquiry_device_list = (APP_MGR_DEVICE_INFO_BASE *)COS_Malloc(sizeof(APP_MGR_DEVICE_INFO_BASE)*max_result);
        memset(app_store_inquiry_data.inquiry_device_list, 0, sizeof(APP_MGR_DEVICE_INFO_BASE)*max_result);
        app_store_inquiry_data.inquiry_device_count = 0;
        app_store_inquiry_data.max_inquiry_num = max_result; 
    }
    
    if(!app_store_inquiry_data.app_inquiry_timer)
        app_store_inquiry_data.app_inquiry_timer = mmi_timer_create(APP_INQUIRY_TIME, app_bt_inquiry_cancel_req, NULL, COS_TIMER_MODE_SINGLE);//after 60s ,we will cannel the inquiry process.

    app_store_inquiry_data.is_LIAC= is_LIAC;     
#endif

    if(app_inquiry_data.inquiry_device_list != NULL)
    {
        COS_Free(app_inquiry_data.inquiry_device_list);
        app_inquiry_data.inquiry_device_list = NULL;
    }
    
    app_inquiry_data.inquiry_device_count = 0;
    app_inquiry_data.inquiry_length = 8;
    app_inquiry_data.max_inquiry_num = max_result; 
    app_inquiry_data.is_LIAC = is_LIAC;
    app_inquiry_data.inquiry_device_list = (APP_MGR_DEVICE_INFO_BASE *)COS_Malloc(sizeof(APP_MGR_DEVICE_INFO_BASE)*max_result);
    memset(app_inquiry_data.inquiry_device_list, 0, sizeof(APP_MGR_DEVICE_INFO_BASE)*max_result); 
    
    if(app_inquiry_data.inquiry_device_list != NULL)
    {
#if APP_SUPPORT_TEST_BOX==1	        
        ap_test_box_inquiry_start();
#endif	
        app_set_mgr_state(MGR_INQUIRY_PENDNG);
        COS_SendEvent(MOD_BT, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
}


void app_bt_inquiry_cancel_req(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_USER_BT_INQUIRT_CANCEL_REQ;
    hal_HstSendEvent(SYS_EVENT, 0x20170405);
    if(app_inquiry_data.inquiry_device_list != NULL)
    {
        COS_Free(app_inquiry_data.inquiry_device_list);
        app_inquiry_data.inquiry_device_list = NULL;
    }
    
#if APP_SUPPORT_TEST_BOX==1		
    ap_test_box_inquiry_cancel();
#endif		

#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
    if(app_addr_need_getnameTable)
    {
        COS_Free(app_addr_need_getnameTable);
    }
    
    if(app_store_inquiry_data.app_inquiry_timer)
        mmi_cancel_timer(app_store_inquiry_data.app_inquiry_timer);
    
    app_store_inquiry_data.app_inquiry_timer = NULL;
    app_store_inquiry_data.inquiry_device_count = 0;
    app_store_inquiry_data.max_inquiry_num = 0;
#endif
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#if APP_SUPPORT_DUT==1
void app_bt_dut_test_req(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_USER_BT_DUT_TEST_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
#endif

#if APP_SUPPORT_VCO==1
void app_bt_vco_test_req(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_VCO_TEST_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
}
#endif

void app_bt_bonding_req(t_bdaddr *pAddr)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_USER_BT_BONDING_REQ;
    ev.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_sco_connect_req(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_SCO_CONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_sco_disconnect_req(u_int16 sco_handle)
{
    COS_EVENT event = {0};

    if(sco_handle != 0)
    {
        event.nEventId = RDABT_SCO_DISCONNECT_REQ;
        event.nParam1 = (UINT32)sco_handle;
        COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
}
/***********************************************************
remove deviceinfo which is stored in host by address
**************************************************************/
void app_remove_device_req(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_REMOVE_DEVICE_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_mgr_add_device(APP_MGR_DEVICE_INFO_BASE *device)
{
    u_int8 i = 0;
    u_int8 max_device_num=app_bt_vars->max_device_num;
    app_trace(APP_BT_TRC, "app_mgr_add_device %d", app_bt_vars->max_device_num);
    app_trace(APP_BT_TRC, "step0 addr is %x",device->addr.bytes);
    app_trace(APP_BT_TRC, "step0 name is %s",device->device_name);
    app_trace(APP_BT_TRC, "step0 cod is 0x%x",device->cod);

#if APP_SUPPORT_HID_MODE==1
    app_trace(APP_BT_TRC, "step0 g_current_BT_module is %d",g_current_BT_module);
    hal_HstSendEvent(SYS_EVENT, 0x06070001);
    hal_HstSendEvent(SYS_EVENT, g_current_BT_module);
#endif

    for(i=0; i < max_device_num; i++)
    {
        app_trace(APP_BT_TRC, "step1-%d addr is %x",i,app_bt_vars->device_list[i].addr.bytes);
        app_trace(APP_BT_TRC, "step1-%d name is %s",i,app_bt_vars->device_list[i].device_name);

        if(!memcmp(device->addr.bytes, app_bt_vars->device_list[i].addr.bytes, SIZE_OF_BDADDR))
        {
            app_bt_vars->device_list[i] = *device;
#if APP_SUPPORT_HID_MODE==1            
            if(g_current_BT_module==BT_MODE_HID)
                app_bt_vars->last_hid_device = i;
            else  
#endif            
            if(app_bt_vars->last_device != i)
                app_bt_vars->last_device_two = app_bt_vars->last_device;
            
            app_bt_vars->last_device = i;
            app_trace(APP_BT_TRC, "step2 addr is %x",app_bt_vars->device_list[i].addr.bytes);
            app_trace(APP_BT_TRC, "step2 addr is %x",device->addr.bytes);
            app_trace(APP_BT_TRC, "step2 cod is 0x%x",app_bt_vars->device_list[i].cod);
            return ;     
        }
    }

    app_trace(APP_BT_TRC, "step4-%d name is %s",app_bt_vars->old_device,app_bt_vars->device_list[app_bt_vars->old_device].device_name);


#if APP_SUPPORT_HID_MODE==1    
    hal_HstSendEvent(SYS_EVENT, 0x06070002);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->old_device);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->old_hid_device);

    if(g_current_BT_module==BT_MODE_HID) 
    {
        app_bt_vars->device_list[app_bt_vars->old_hid_device] = *device;
        app_bt_vars->last_hid_device = app_bt_vars->old_hid_device;
        app_bt_vars->old_hid_device++;

        hal_HstSendEvent(SYS_EVENT, 0x06070005);
        hal_HstSendEvent(SYS_EVENT, app_bt_vars->old_device);
        hal_HstSendEvent(SYS_EVENT, app_bt_vars->old_hid_device);
    }
    else
#endif  
    {
        app_bt_vars->device_list[app_bt_vars->old_device] = *device;
        app_bt_vars->last_device = app_bt_vars->old_device;
        app_bt_vars->old_device++;
    }

#if APP_SUPPORT_HID_MODE==1   
    if(g_current_BT_module==BT_MODE_HID)
    {
        if(app_bt_vars->old_hid_device == max_device_num)
            app_bt_vars->old_hid_device = 0;
    }
    else
#endif
    if(app_bt_vars->old_device == max_device_num)
    {
        app_bt_vars->old_device = 0;
    }

    app_trace(APP_BT_TRC, "step4 addr is %x",app_bt_vars->device_list[app_bt_vars->old_device].addr.bytes);
    app_trace(APP_BT_TRC, "step4 addr is %x",device->addr.bytes);
    app_trace(APP_BT_TRC, "step4 cod is 0x%x",app_bt_vars->device_list[app_bt_vars->old_device].cod);
}

void app_mgr_bonding_cnf_cb(APP_MGR_DEVICE_INFO_BASE *device)
{
    hal_HstSendEvent(SYS_EVENT, 0x20151104);
    hal_HstSendEvent(SYS_EVENT, (u_int32)device);
    
    if(device != NULL)
    {
        app_mgr_add_device(device);
        if(IS_BT_MODE())
        {
            app_mgr_store();
        }
        hal_HstSendEvent(1, 0x04281359);
        hal_HstSendEvent(1, app_bt_vars->magic);
        hal_HstSendEvent(1, app_bt_vars->device_list[0].cod);
        hal_HstSendEvent(1, app_bt_vars->device_list[0].addr.bytes[0]);
        hal_HstSendEvent(1, app_bt_vars->device_list[0].device_name[0]);
    }
}

void app_get_mgr_adp_info_req(void)
{	
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_MGR_ADP_INFO_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hci_msg_handle(COS_EVENT *ev)
{
    COS_EVENT event = {0};

    switch(ev->nEventId)
    {
#ifndef WIN32
        case RDABT_SCO_DATA_IND:
        {
            event.nEventId = RDABT_SCO_DATA_RES;
            event.nParam1 = ev->nParam1;
            rdabt_send_msg_to_bt(&event);
        }
        break;
#endif        
    }
}

u_int8 app_find_bt_device(t_bdaddr addr)
{
    u_int8 i = 0;
    APP_MGR_DEVICE_INFO_BASE *tmp = NULL;
    u_int8 max_device = tgt_GetBtMaxDeviceNum();

    for(i=0; i < max_device; i++)
    {
        tmp = &app_bt_vars->device_list[i];
        if(!memcmp(tmp->addr.bytes, addr.bytes, sizeof(t_bdaddr)))
        {      
            return i;
        }
    }
    return 0xff;
}

#if APP_SUPPORT_BLE_BR==1
void app_switch_br_le_mode(void *pArgs)
{
    u_int8 i = 0;
    static u_int8 mode = 1;
    APP_BT_FLASH_STORE *flash_info;
    u_int8 max_device = tgt_GetBtMaxDeviceNum();

    if(app_bt_vars->switch_timer_handle == 0)
    {
        return;
    }
    
    app_bt_vars->switch_timer_handle = 0;
    if(mode == 0)
    {
        flash_info = (APP_BT_FLASH_STORE *)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
        app_bt_vars->device_list = (APP_MGR_DEVICE_INFO_BASE *)COS_Malloc(sizeof(APP_MGR_DEVICE_INFO_BASE)*max_device);
        
        if(flash_info == NULL || app_bt_vars->device_list == NULL)
        {
            hal_DbgAssert("No bt ram");
        }
        memset(app_bt_vars->device_list, 0, sizeof(APP_MGR_DEVICE_INFO_BASE)*max_device);
        
        if(flash_info->magic == MAGIC_BLUETOOTH)
        {
            for(; i < max_device; i++)
            {
                app_bt_vars->device_list[i].addr = flash_info->device_list[i].addr;
                app_bt_vars->device_list[i].cod = flash_info->device_list[i].cod;
                app_bt_vars->device_list[i].key_type = flash_info->device_list[i].key_type;
                memcpy(app_bt_vars->device_list[i].device_name, flash_info->device_list[i].device_name, MAX_BT_DEVICE_NAME);
                memcpy(app_bt_vars->device_list[i].link_key, flash_info->device_list[i].link_key, SIZE_OF_LINKKEY);
            }
        }
    }
    app_bt_mode_switch(mode);
    mode = !mode;
}

void app_cancel_switch(void)
{
    if(app_bt_vars->switch_timer_handle)
    {
        mmi_cancel_timer(app_bt_vars->switch_timer_handle);
        app_bt_vars->switch_timer_handle = 0;
    }
}

void app_start_switch(void)
{
    if(app_bt_vars != NULL)
    {
        if(app_bt_vars->switch_timer_handle == 0)
        {
            if(app_bt_vars->bt_mode == HOST_BR_MODE)
            {   
                app_bt_vars->switch_timer_handle = mmi_timer_create(45, app_switch_br_le_mode, NULL, COS_TIMER_MODE_SINGLE);
            }
            else
            {
                app_bt_vars->switch_timer_handle = mmi_timer_create(25, app_switch_br_le_mode, NULL, COS_TIMER_MODE_SINGLE);
            }
        }
    }
}
#endif

void app_cancel_reconnect(void)
{
    if(app_bt_vars != NULL)
    {
        app_bt_vars->connect_retry_cnt = 0;
        
        if(app_bt_vars->auto_connect_timer)
        {
            mmi_cancel_timer(app_bt_vars->auto_connect_timer);
            app_bt_vars->auto_connect_timer = 0;
        }
    }
}

void app_mgr_store(void)
{
    APP_BT_FLASH_STORE *flash_info = NULL;
    u_int8 i = 0;

    if(app_bt_vars != NULL)
    {
        flash_info = (APP_BT_FLASH_STORE *)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
        
        if(flash_info == NULL)
        {
            hal_HstSendEvent(SYS_EVENT, 0x04271556);
        }
        else
        {
#if APP_SUPPORT_HID_MODE==1
            u_int8 index=0, max_device_num = app_bt_vars->max_device_num;

            if(g_current_BT_module == BT_MODE_HID)
                index=max_device_num;
            else
                index = 0;

            for(i=0; i < max_device_num; i++)
            {
                flash_info->device_list[i+index].addr = app_bt_vars->device_list[i].addr;
                flash_info->device_list[i+index].cod = app_bt_vars->device_list[i].cod;
                flash_info->device_list[i+index].key_type = app_bt_vars->device_list[i].key_type;
                memcpy(flash_info->device_list[i+index].device_name, app_bt_vars->device_list[i].device_name, MAX_BT_DEVICE_NAME);
                memcpy(flash_info->device_list[i+index].link_key, app_bt_vars->device_list[i].link_key, SIZE_OF_LINKKEY);
            }

            if(g_current_BT_module == BT_MODE_HID)
            {
                hal_HstSendEvent(SYS_EVENT, 0x06060011);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->local_addr.bytes[0]);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_hid_device);
                flash_info->last_hid_device = app_bt_vars->last_hid_device;
            }
            else
                flash_info->last_device = app_bt_vars->last_device;
            
            flash_info->last_device_two= app_bt_vars->last_device_two;
#else
            for(; i < app_bt_vars->max_device_num; i++)
            {
                flash_info->device_list[i].addr = app_bt_vars->device_list[i].addr;
                flash_info->device_list[i].cod = app_bt_vars->device_list[i].cod;
                flash_info->device_list[i].key_type = app_bt_vars->device_list[i].key_type;
                memcpy(flash_info->device_list[i].device_name, app_bt_vars->device_list[i].device_name, MAX_BT_DEVICE_NAME);
                memcpy(flash_info->device_list[i].link_key, app_bt_vars->device_list[i].link_key, SIZE_OF_LINKKEY);
            }
            
            flash_info->last_device = app_bt_vars->last_device;
            flash_info->last_device_two= app_bt_vars->last_device_two;
#if APP_SUPPORT_HID == 1 &&APP_SUPPORT_HID_MODE ==0
            flash_info->not_support_hid_service = app_bt_vars->not_support_hid_service;
#endif
#endif

            flash_info->magic = MAGIC_BLUETOOTH;
            flash_info->local_addr = app_bt_vars->local_addr;
            memcpy(flash_info->local_name, app_bt_vars->local_name, MAX_BT_DEVICE_NAME);
#if  APP_SUPPORT_SOUNDBAR == 1
            flash_info->soundbar_default_role = Soundbar_App_Get_Role_Status();
            flash_info->soundbar_connect_addr = soundbar_device.addr;
#endif
            NVRAMWriteData();   
        }
    }
}

void app_start_reconnect( u_int8 i)
{
    u_int32 index = i;
    hal_HstSendEvent(SYS_EVENT, 0x20170515);
    hal_HstSendEvent(SYS_EVENT, (u_int32)app_bt_vars);  
    
    if(app_bt_vars)
        hal_HstSendEvent(SYS_EVENT, app_bt_vars->auto_connect_timer);//two device discon with 008 status ,will recon first one only  

    hal_HstSendEvent(SYS_EVENT, i);
    if(i == 0xff)
    {
        return ;
    }
    
#if APP_SUPPORT_SOUNDBAR == 1
    if(app_find_bt_device(soundbar_device.addr) == i)
    {
        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
        return;
    }
#endif

#if APP_SUPPORT_A2DP == 1
    if(app_a2dp_check_active(app_bt_vars->device_list[i].addr))
    {
        hal_HstSendEvent(SYS_EVENT, 0x20170810);
        return;
    }
#endif

#if APP_SUPPORT_HFP == 1
    if(app_find_active_hfp_device())
    {
        hal_HstSendEvent(SYS_EVENT, 0x20171810);
        return;
    }
#endif

    if(app_bt_vars != NULL)
    {
        app_bt_vars->times_num = 10;
        app_bt_vars->times_value = 30;      
        
        if(!app_bt_vars->auto_connect_timer)
            app_bt_vars->auto_connect_timer=mmi_timer_create(40, app_bt_connect_retry, (void *)index, COS_TIMER_MODE_SINGLE);
    }
}

void app_mgr_msg_handle(COS_EVENT *ev)
{
    u_int8 i = 0;
    t_bdaddr addr = {{0}};

    switch(ev->nEventId)
    {
        case RDABT_ACL_CONNECT_IND_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;  
            hal_HstSendEvent(SYS_EVENT, 0x05121502);
            hal_HstSendEvent(SYS_EVENT, msg_info->msg_param);
            if(msg_info->msg_param == RDABT_NOERROR)
            {
                addr = *(t_bdaddr *)ev->nParam1;
                app_cancel_reconnect();
#if APP_SUPPORT_BLE_BR==1
                app_cancel_switch();
#endif
                i = app_find_bt_device(addr);
                hal_HstSendEvent(SYS_EVENT, 0x05121505);
                hal_HstSendEvent(SYS_EVENT, i);
                app_bt_vars->connect_index++;
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->connect_index);
                if(i != 0xff)
                {
#if APP_SUPPORT_HID_MODE==1
                    if(g_current_BT_module == BT_MODE_HID)
                        app_bt_vars->last_hid_device = i;
                    else
                    {
                        if(app_bt_vars->last_device != i)
                            app_bt_vars->last_device_two = app_bt_vars->last_device;
                        app_bt_vars->last_device = i;
                    }
#else            
                    if(app_bt_vars->last_device != i)
                        app_bt_vars->last_device_two = app_bt_vars->last_device;
                    
                    app_bt_vars->last_device = i;
#endif                    
                }
                app_mgr_store();
            }
        }
        break;	
        
        case RDABT_ACL_CONNECT_CNF_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;
            hal_HstSendEvent(1, 0x20162224);
            hal_HstSendEvent(1, ev->nParam1);
            hal_HstSendEvent(1, msg_info->msg_param);
#if APP_SUPPORT_BLE_BR==1
            app_cancel_switch();
#endif

            if(g_current_module == RESULT_BT)
            {
                if(msg_info->msg_param == RDABT_NOERROR)
                {
                    app_cancel_reconnect();
                    app_bt_vars->connect_index++;
                    hal_HstSendEvent(1, app_bt_vars->connect_index);
                    //right
                }
                else 
                {
                    //error
                    hal_HstSendEvent(1, 0x20162225);
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
                    app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#else
                    app_set_bt_visible_req(MGR_SET_CONNECTABLE);
#endif
                }
            }
        }
        break;
        
        case RDABT_ACL_DISCONNECT_IND_MSG:
        {       
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO *)ev->nParam1;
            hal_HstSendEvent(1, 0x04141456);
            hal_HstSendEvent(1, msg_info->msg_param);			
            app_cancel_reconnect();
            app_bt_vars->connect_index--;
            hal_HstSendEvent(1, app_bt_vars->connect_index);
            if(IS_BT_MODE() && app_get_mgr_state() > MGR_CLOSE_PENDING && msg_info->msg_param == 0x008)// && app_get_a2dp_state()!=A2DP_PLAY) // auto connect
            {
                hal_HstSendEvent(1, 0x04141457);
#if APP_SUPPORT_TEST_BOX==0
                app_start_reconnect(app_find_bt_device(msg_info->device_addr));
#endif
            }
            else
            {
#if APP_SUPPORT_HFP == 1
                app_cancel_create_hfp_connection(msg_info->device_addr);
#endif

#if APP_SUPPORT_A2DP == 1
                app_cancel_create_a2dp_connection(msg_info->device_addr);
#endif

#if APP_SUPPORT_BLE_BR==1
                app_start_switch();
#endif
            }
            
            memset(addr.bytes, 0, sizeof(t_bdaddr));
#if APP_SUPPORT_SOUNDBAR == 1 || APP_SUPPORT_ONLY_ONE_LINK == 1 || APP_SUPPORT_BT_AUTOVISIBLE == 1
            app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#else
            app_set_bt_visible_req(MGR_SET_CONNECTABLE);
#endif
        }
        break;
        
        case RDABT_SCO_CONNECT_IND_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x20161042);
            hal_HstSendEvent(SYS_EVENT, (u_int16)ev->nParam1);
            
#if APP_SUPPORT_HFP == 1                       
            MESSAGE_SetEvent(AP_MSG_WAKE_REQ);
            APP_Wakeup();
            app_get_hfp_adp_info()->sco_handle = (u_int16)ev->nParam1;
#if APP_SUPPORT_A2DP==1
            if( app_get_a2dp_state() != A2DP_PLAY)
#endif
                app_hfp_play_sco();
#endif            
        }
        break;
        
        case RDABT_SCO_DISCONNECT_IND_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x2016104a);

#if APP_SUPPORT_HFP == 1               
            app_hfp_stop_sco();
            app_get_hfp_adp_info()->sco_handle = 0;
#endif
        }
        break;
        
        case RDABT_DISCOVERY_RESULT_IND:
        {
            app_mgr_deal_discover_result_func((APP_MGR_DEVICE_INFO_BASE *)ev->nParam1);
        }
        break;	
        
        case RDABT_DISCOVERY_RESULT_CNF:
        {
            hal_HstSendEvent(SYS_EVENT, 0x03021758);

            app_mgr_deal_discover_cnf_func();
        }
        break;	
        
        case RDABT_BONDING_CNF:
        {
            app_mgr_bonding_cnf_cb((APP_MGR_DEVICE_INFO_BASE *)ev->nParam1);
        }
        break;	
        
        case RDABT_MGR_ADP_INFO_MSG:
        {
            APP_BT_MANAGER *adp_ptr = (APP_BT_MANAGER *)ev->nParam1;    
            hal_HstSendEvent(1, 0x04180949);
            if(app_bt_vars->device_list)
            {
                COS_Free(app_bt_vars->device_list);
            }
            app_bt_vars->device_list = adp_ptr->device_list;
        }
        break;  
        
        case RDABT_MGR_DEVICE_UPDATE_MSG:
        {
#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
            app_mgr_deal_namereq_result_func((APP_BT_MANAGER *)ev->nParam1);
#endif
        }
        break; 
        
        case RDABT_GET_RSSI_CNF:
        {
            hal_HstSendEvent(SYS_EVENT, 0x20171304);
            app_trace(APP_BT_TRC, "rssi:%d", ev->nParam1);
        }
        break;
    }
}

void app_common_msg_handle(COS_EVENT *ev)
{
    COS_EVENT event = {0};

    switch(ev->nEventId)
    {
        case RDABT_USER_BT_READY_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x01050010);
            hal_HstSendEvent(SYS_EVENT, ev->nParam1);
            
            if(!IS_BT_MODE())
            {
                app_bt_close_req();
                break;
            }
            
            if(ev->nParam1 == 0)
            {
                hal_HstSendEvent(SYS_EVENT, 0x20151111);
                event.nEventId = RDABT_STACK_INIT_REQ;
                event.nParam1 = (UINT32)app_bt_vars;
                COS_SendEvent(MOD_BT,  &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
                hal_HstSendEvent(SYS_EVENT, 0x20152222);
            }
            hal_HstSendEvent(SYS_EVENT, 0x20153333);  
        }
        break;

        case RDABT_USER_BT_MODE_SWITCH_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x20161751);
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->bt_mode);
            
#if APP_SUPPORT_LE==1                        
            app_bas_reset();
#endif
            event.nEventId = RDABT_STACK_INIT_REQ;
            event.nParam1 = (UINT32)app_bt_vars;
            COS_SendEvent(MOD_BT,  &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            
            if(app_bt_vars->bt_mode == HOST_BR_MODE)
            {
                app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
            }
        }
        break;       
        
        case RDABT_USER_BT_CLOSE_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x20161507);
            app_bt_close_complet();
            //AppGotoMainMenu();
        }
        break;		
    }
}


void app_adp_msg_handle(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case RDABT_STACK_INIT_MSG:
        {
            t_bdaddr null_addr = {{0}};
            app_bt_vars->bt_mode = ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x09261114);
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->bt_mode);
            
#if APP_SUPPORT_BLE_BR==1
            app_start_switch();
#endif

            if(app_bt_vars->bt_mode == HOST_BR_MODE)
            {
                app_get_mgr_adp_info_req();
                hal_HstSendEvent(SYS_EVENT, 0x06081111);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_device);

#if APP_SUPPORT_HID_MODE==1
                hal_HstSendEvent(SYS_EVENT, g_current_BT_module);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_hid_device);

                if(g_current_BT_module==BT_MODE_HEADSET)
                {
#endif 
#if APP_SUPPORT_HFP == 1                
                    app_hfp_get_adp_info_req();
#endif

#if APP_SUPPORT_A2DP == 1
                    app_a2dp_get_adp_info_req();
#endif

#if APP_SUPPORT_AVRCP == 1
                    app_avrcp_get_adp_info_req();
#endif

#if APP_SUPPORT_HID_MODE==1
                }
#endif

#if APP_SUPPORT_HID==1                		
#if APP_SUPPORT_HID_MODE==1
                else if(g_current_BT_module==BT_MODE_HID)
#else
                if(app_bt_vars->not_support_hid_service == 0)
#endif
                    app_hid_register_sdp();
#endif  

#if APP_SUPPORT_SOUNDBAR == 1
                app_send_support_soundbar();
#endif

#if APP_SUPPORT_BT_AUTOVISIBLE == 0
                hal_HstSendEvent(SYS_EVENT, 0x20170708);
                hal_HstSendEvent(SYS_EVENT, bal_is_first_keydown());
                
                if(!bal_is_first_keydown())
                {
#endif       
#if APP_SUPPORT_BLE_BR == 0
#if APP_SUPPORT_HID_MODE==1
                    if(g_current_BT_module == BT_MODE_HID)
                    {
                        u_int8 name_len=0;
                        name_len = strlen(app_bt_vars->local_name);

                        hal_HstSendEvent(SYS_EVENT, 0x06081188);
                        hal_HstSendEvent(SYS_EVENT, name_len);
                        if(name_len>4)
                        {
                            app_bt_vars->local_name[name_len-4]='\0';
                            app_bt_vars->local_name[name_len-3]=0;
                            app_bt_vars->local_name[name_len-2]=0;
                            app_bt_vars->local_name[name_len-1]=0;
                        }
                        app_bt_vars->local_addr.bytes[0]--;
                    }

                    if((g_current_BT_module==BT_MODE_HID && app_bt_vars->last_hid_device != 0xff)
                        ||(g_current_BT_module==BT_MODE_HEADSET && app_bt_vars->last_device != 0xff))
                    {
                        if((g_current_BT_module==BT_MODE_HEADSET && memcmp(app_bt_vars->device_list[app_bt_vars->last_device].addr.bytes, null_addr.bytes, SIZE_OF_BDADDR))
                            ||(g_current_BT_module==BT_MODE_HID && memcmp(app_bt_vars->device_list[app_bt_vars->last_hid_device].addr.bytes, null_addr.bytes, SIZE_OF_BDADDR)))
                        {
                            app_bt_connect(4);
                        }
                        else
                        {
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
                            app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#endif
                            //hal_DbgAssert("xxx");
                        }
                    }
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
                    else
                    {
                        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
                    }
#endif

#else
                    if(app_bt_vars->last_device != 0xff)
                    {
                        if(memcmp(app_bt_vars->device_list[app_bt_vars->last_device].addr.bytes, null_addr.bytes, SIZE_OF_BDADDR))
                        {
                            app_bt_connect(4);
                        }
                        else
                        {
                            hal_DbgAssert("xxx");
                        }
                    }
#if APP_SUPPORT_BT_AUTOVISIBLE == 1
                    else
                    {
                        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
                    }
#endif
#endif                
#endif
#if APP_SUPPORT_BT_AUTOVISIBLE == 0
                }
#endif
            }
            else
            {
#if APP_SUPPORT_LE==1                    
                //app_bt_start_ias_server_req();
                app_bt_start_bas_server_req();
#endif
            }
        }
        break;        
    }
}

INT32 BT_ChangeVol(UINT32 key)
{
#if APP_SUPPORT_SOUNDBAR == 0
    BOOL play_tone = TRUE;
#endif
    UINT8 audio_ID = 0;

#if defined(__AT_MOD_COMMON__)
    if (key == AT_COMMON_VOLADD) {
        key = AP_KEY_VOLADD;
    }
    else if (key == AT_COMMON_VOLSUB) {
        key = AP_KEY_VOLSUB;
    }
#endif

    key = key&0xffff0000;

#if APP_SUPPORT_HFP == 1
    if(app_get_hfp_state() > HFP_CONNECTED)
    {
        app_bt_vars->volume = app_bt_vars->volume_hfp;
        hal_HstSendEvent(1, 0x20170033);
    }
    else
#endif
    {
#if APP_SUPPORT_A2DP == 1
        if(app_get_a2dp_state() >= A2DP_PLAY_AUDIO)
        {
            app_bt_vars->volume = app_bt_vars->volume_a2dp;
            hal_HstSendEvent(1, 0x20170034);
        }
#endif
    }
    
    if(key == AP_KEY_VOLADD)
    {
        app_bt_vars->volume++;
        if(app_bt_vars->volume > BT_VOLUME_MAX)
        {
            app_bt_vars->volume = BT_VOLUME_MAX;
            audio_ID = GUI_AUDIO_BT_MAXVOL;
        }
    }

    if(key == AP_KEY_VOLSUB)
    {
        app_bt_vars->volume--;
        if(app_bt_vars->volume <= 0)
        {
#if APP_SUPPORT_HFP == 1        
            if(app_get_hfp_state() > HFP_CONNECTED)//Don't mute in call
            {
                app_bt_vars->volume = 1;
            }
            else
#endif                
            {
                app_bt_vars->volume = 0;
            }
            audio_ID = GUI_AUDIO_BT_MINVOL;
        }
    }		

    hal_HstSendEvent(SYS_EVENT, 0x19884000);
    hal_HstSendEvent(SYS_EVENT, key);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume);
    
#if  APP_SUPPORT_SOUNDBAR == 1
    app_send_soundbar_volume(app_bt_vars->volume);
#endif

    mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume);

#if APP_SUPPORT_SOUNDBAR == 0
    if(audio_ID > 0)
    {
        hal_HstSendEvent(SYS_EVENT, 0x19884001);
        //GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_VOL_MAXORMIN, GUI_MSG_FLAG_WAIT);
#if APP_SUPPORT_A2DP == 1
        if ((A2DP_PLAY_AUDIO != app_get_a2dp_state()) && (A2DP_PLAY_AUDIO_VOL!= app_get_a2dp_state()))        //if (A2DP_PLAY_AUDIO <= app_get_a2dp_state())

        {
            play_tone = TRUE;
        }
        else
        {
            play_tone = FALSE;
        }
#endif

#if APP_SUPPORT_HFP == 1
        if(play_tone && ((app_get_hfp_state()&HFP_AUDIO_STATE_ON_MASK) == 0))
        {
            play_tone = TRUE;
        }
        else
        {
            play_tone = FALSE;
        }
#endif

#if APP_SUPPORT_HFP == 1
        if(app_get_hfp_state() > HFP_CONNECTED)
        {
            app_set_hfp_state(app_get_hfp_state()|HFP_AUDIO_STATE_ON_MASK);
            app_bt_vars->volume_hfp = app_bt_vars->volume;
        }
        else
#endif
        {
#if APP_SUPPORT_A2DP == 1
            if(app_get_a2dp_state() != A2DP_PLAY_AUDIO_VOL)            //if(app_get_a2dp_state() >= A2DP_PLAY_AUDIO)
            {
                app_set_a2dp_state(A2DP_PLAY_AUDIO_VOL);
                app_bt_vars->volume_a2dp = app_bt_vars->volume;
            }
#endif
        }

        if (play_tone)
        {
            g_VolAudioMask = 0;
            mediaSendBTCommand(MC_STOP, 0);
            MMC_AudioCleanA2dpData();
            media_PlayInternalAudio(audio_ID, 1, 0);
        }
    }
    else
    {
#if APP_SUPPORT_HFP == 1
        if(app_get_hfp_state() > HFP_CONNECTED)
        {
            app_bt_vars->volume_hfp = app_bt_vars->volume;
        }
        else
#endif
        {
#if APP_SUPPORT_A2DP == 1
            if(app_get_a2dp_state() >= A2DP_PLAY_AUDIO)
            {
                app_bt_vars->volume_a2dp = app_bt_vars->volume;
            }
#endif
        }
    }
#endif

    return 0;
}

void app_exit_timer_check(void)
{
    if(app_bt_vars->auto_connect_timer)
    {
        mmi_cancel_timer(app_bt_vars->auto_connect_timer);
        app_bt_vars->auto_connect_timer = 0;
    }
    
#if APP_SUPPORT_BLE_BR==1
    if(app_bt_vars->switch_timer_handle)
    {
        mmi_cancel_timer(app_bt_vars->switch_timer_handle);
        app_bt_vars->switch_timer_handle = 0;
    }
#endif

#if APP_SUPPORT_HFP == 1
    app_hfp_clean_timer();    
#endif

#if APP_SUPPORT_A2DP == 1
    app_a2dp_clean_timer();
#endif
} 

void app_mgr_vol_key_up(UINT32 key)
{
#if APP_SUPPORT_HFP == 1
    APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();

    if(app_get_hfp_state() > HFP_CONNECTED)
    {
            app_set_hfp_state(app_get_hfp_state()&HFP_AUDIO_STATE_OFF_MASK);
            if(app_get_hfp_state() == HFP_PLAY_SCO)
            {
                if(hfp_adp_info == NULL)
                {
                    hal_HstSendEvent(SYS_EVENT, 0xeeee0004);
                    return ;
                }
                mediaSendBTCommand(MC_STOP, 0);
                mediaSendBTCommand(MC_PLAY_SCO, hfp_adp_info->sco_handle);
                mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_hfp);
            }
    }
    else
#endif
    {
#if APP_SUPPORT_A2DP == 1
        if((app_get_a2dp_state() == A2DP_PLAY_AUDIO_VOL
            || app_get_a2dp_state() == A2DP_PLAY_AUDIO) && app_check_a2dp_connection())
        {
            app_set_a2dp_state(A2DP_CONNECTED);
            app_a2dp_play_sbc_after();
        }
#endif    
    }
}

INT32 bt_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    return 0;
}

BOOL app_bt_main_key_handle(u_int32 key)
{    
    BOOL ret = TRUE;

    switch(key)
    {   
#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_VOLADD:
        case AT_COMMON_VOLSUB:
#endif
        case AP_KEY_VOLADD|AP_KEY_PRESS:
        case AP_KEY_VOLADD|AP_KEY_HOLD:
        case AP_KEY_VOLSUB|AP_KEY_PRESS:
        case AP_KEY_VOLSUB|AP_KEY_HOLD:    
        {
            BT_ChangeVol(key);
        }
        break;
        
        case AP_KEY_VOLADD|AP_KEY_UP:
        case AP_KEY_VOLSUB|AP_KEY_UP:
        {
#if APP_SUPPORT_HFP == 1
            hal_HstSendEvent(1, 0x20170718);
            hal_HstSendEvent(1, g_VolAudioMask);
            hal_HstSendEvent(1, app_get_hfp_state());
#endif            
            if (g_VolAudioMask == 1)
            {
                app_mgr_vol_key_up(key);
                g_VolAudioMask = 0;
            }
            else
            {
                g_VolAudioMask = 2;
            }
        }
        break;	
        
        case AP_KEY_VOL_COMB|AP_KEY_HOLD: 
        //case AP_KEY_VOL_COMB|AP_KEY_PRESS: 	
        {
            hal_HstSendEvent(1, 0x20170007);
            hal_HstSendEvent(1, app_get_mgr_state());
            if(app_get_mgr_state() == MGR_SCAN)
                app_remove_device_req(&app_clean_addr);
        }
        break;
        
        case AP_KEY_NEXT|AP_KEY_PRESS:
        {
#if APP_SUPPORT_SOUNDBAR ==1
            app_bt_inquiry_req(10, 0);
#else
#if APP_SUPPORT_GET_INQUIRYDEVICENAME == 1
            app_bt_inquiry_req(10, 0);
#endif
#endif
            hal_HstSendEvent(1, 0x20160801);
        }
        break;	
        
        case AP_KEY_EQ | AP_KEY_PRESS:
        {
            BOOL isEnableEQ = FALSE;

#if APP_SUPPORT_HFP == 1
            if (app_get_hfp_state() > HFP_CONNECTED)
            {
                isEnableEQ = TRUE;
            }
#endif

#if APP_SUPPORT_A2DP== 1
            if (app_get_a2dp_state() == A2DP_PLAY)
            {
                isEnableEQ = TRUE;
            }
#endif
            if (isEnableEQ)
            {
                app_bt_vars->eq_type = (app_bt_vars->eq_type+1)%max_eq;
                mediaSendBTCommand(MC_SETEQ, app_bt_vars->eq_type);
            }            
        }
        break;
        
        case AP_KEY_MODE | AP_KEY_PRESS:
        {
            hal_HstSendEvent(SYS_EVENT, 0x05090948);
            hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
#if APP_SUPPORT_MENU==1
            if(AP_Support_MENU())
            {
                GUI_Display_Menu(GUI_MENU_BT, bt_menu_callback);
            }
#endif
            return FALSE;
        }
        break;

        case AP_KEY_PLAY|AP_KEY_HOLD:
        {
            hal_HstSendEvent(SYS_EVENT, 0x05041721);
            if(bal_is_first_keydown() && MESSAGE_IsHold())
            {
                app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
                media_PlayInternalAudio(GUI_AUDIO_BT_SCAN, 1, 0);
                return TRUE;
            }
            ret = FALSE;
        }
        break;

        case AP_MSG_WAIT_TIMEOUT:
        {
            //hal_HstSendEvent(SYS_EVENT, 0x20170601);
            if(TRUE /*app_bt_vars is NOT NULL always in BT mode*/
#if APP_SUPPORT_HFP == 1                    
                &&(app_get_hfp_state() <= HFP_CONNECTED)
#endif

#if APP_SUPPORT_A2DP == 1 
                &&(app_get_a2dp_state() <= A2DP_CONNECTED)
#endif
            )
            {
                APP_Sleep();
            }
            else
            {
                MESSAGE_SetEvent(AP_MSG_WAKE_REQ);
            }
        }
        break;

        /* 
        app_bt_switch_mode_handler will handle standby msg   
        case AP_MSG_STANDBY:
        {
            hal_HstSendEvent(SYS_EVENT, 0x03281049);    
            if(
#if APP_SUPPORT_HFP == 1
                (app_get_hfp_state() >= HFP_CONNECTED)
                ||
#endif

#if APP_SUPPORT_A2DP == 1
                (app_get_a2dp_state() >= A2DP_CONNECTED)
#endif
            )
            {
                break;
            }
        }
        break;
        */

        default:
        {
            ret = FALSE;
        }
        break;    
    }

    if(app_get_mgr_state() > MGR_ACTIVE_PENDING)
    {
#if APP_SUPPORT_TEST_BOX == 1
        app_test_box_handle_key(key);
#else    

#if APP_SUPPORT_HFP == 1
        if(0 == app_hfp_handle_key(key))
#endif
        {
#if APP_SUPPORT_A2DP == 1    
            app_a2dp_handle_key(key);
#endif

#if APP_SUPPORT_AVRCP == 1        
            app_avrcp_handle_key(key);
#endif

#if APP_SUPPORT_HID == 1        
            app_hid_handle_key(key);
#endif
        }
#endif
    }
    return ret;
}

void app_bt_close_complet(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x20170510);
    hal_HstSendEvent(SYS_EVENT, app_get_bt_intercept_switch_mode_result());
    //ClearProtocolEventHandler(RDABT_USER_BT_CLOSE_MSG);

    mediaSendBTCommand(MC_STOP, 0);
    //Execute default switch_mod handler
    ClearProtocolEventHandler(EV_UI_FW_SWITCH_MOD);
    SendAppEvent(EV_UI_FW_SWITCH_MOD, app_get_bt_intercept_switch_mode_result()); 
}

BOOL app_switch_mode_handle(UINT32 nextModule)
{
    hal_HstSendEvent(1, 0x05122052);
    hal_HstSendEvent(1, app_get_mgr_state());
#if APP_SUPPORT_HFP==1        
    hal_HstSendEvent(1, app_get_hfp_state());

    if( app_get_hfp_state() <= HFP_CONNECTED )
    {
#endif
        if(app_get_mgr_state() == MGR_CONNECTED)
        {
#if APP_SUPPORT_A2DP==1
            app_a2dp_switch_mode();
            app_avrcp_switch_mode();
#endif
#if APP_SUPPORT_HFP == 1
            app_hfp_switch_mode();
#endif
#if APP_SUPPORT_HID == 1
            app_hid_disconnect_req();
#endif

            app_bt_close_req();
            //#if APP_SUPPORT_SOUNDBAR == 1
            //                if(Soundbar_App_Get_Role_Status() < SOUNDBAR_NOROLE)
            //                {
            //                    return TRUE;
            //                }
            ///#endif
            //if(app_bt_vars->close_bt == TRUE)
            //{
            //    app_set_mgr_state(MGR_CLOSE_PENDING);
            //    SetProtocolEventHandler(app_bt_close_req, RDABT_ACL_DISCONNECT_IND_MSG);
            //SetProtocolEventHandler(app_bt_close_complet, RDABT_USER_BT_CLOSE_MSG);
            //}
        }
        else
        {		
            //SetProtocolEventHandler(app_bt_close_complet, RDABT_USER_BT_CLOSE_MSG); 
            app_bt_close_req();
        }
        return TRUE;
#if APP_SUPPORT_HFP == 1
        }                  
#endif

    return FALSE;
}

VOID app_bt_switch_mode_handler(UINT32 param)
{
    hal_HstSendEvent(SYS_EVENT, 0x05121002);
    hal_HstSendEvent(SYS_EVENT, app_get_bt_intercept_switch_mode_result());

    app_set_bt_intercept_switch_mode_result(param);

#if  APP_SUPPORT_SOUNDBAR == 1
    if(param == RESULT_STANDBY)
    {
        // set SetProtocolEventHandler(CBK, B SHUTDOWND);
        // send B close                                   
        if(app_send_soundbar_shutdown() == FALSE)
            app_bt_close_complet();
    }
    else
#endif
    {
#if  APP_SUPPORT_TEST_BOX == 0
        app_switch_mode_handle(param);
#endif
    }

}

BOOL app_bt_main_msg_handle(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
        {
#if CHIP_BTOVERLAY_SUPPORTED!=0
            hal_BTOverlayLoad(HAL_BTOVERLAY_BLUETOOTH);            
#endif
            //SendAppEvent(EV_UI_FW_REDRAW, app_get_mgr_state());
            app_bt_active_req();
            app_set_bt_intercept_switch_mode_result(RESULT_NULL);

            //To block default switch_mod handler
            SetProtocolEventHandler(app_bt_switch_mode_handler, EV_UI_FW_SWITCH_MOD);
        }
        break;

        case EV_UI_FW_REDRAW:
        {
#if APP_SUPPORT_LCD==1            
            if(app_bt_vars == NULL)
            {
                break;
            }
            
            switch(pEvent->nParam1)
            {
                case MGR_CLOSED:
                    GUI_DisplayMessage(0, GUI_STR_BTCLOSESUCESS, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_CLOSE_PENDING:
                    GUI_DisplayMessage(0, GUI_STR_BTCLOSEING, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_ACTIVE_PENDING:
                    GUI_DisplayMessage(0, GUI_STR_BTACTIVEING, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_ACTIVED:
                    GUI_DisplayMessage(0, GUI_STR_BTACTIVESUCCESS, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_SCAN:
                    GUI_DisplayMessage(0, GUI_STR_BTVISIABLE, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_INQUIRY_PENDNG:
                    GUI_DisplayMessage(0, GUI_STR_BTINVISIABLE, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                case MGR_CONNECTED:
                    GUI_DisplayMessage(0, GUI_STR_BTPAIREDSUCCESS, 0, GUI_MSG_FLAG_DISPLAY);
                    break;
                default:
                    break;
            }
#endif
        }
        break;
        
#if APP_SUPPORT_A2DP == 1
        case EV_UI_FW_ON_PAUSE:
        break;
        
        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, 0);
        break;
        
#endif

#ifdef __AT_MOD_COMMON__
        case AT_COMMON_VOLUME:
        break;
#endif /* __AT_MOD_COMMON__ */

        case EV_UI_FW_ON_EXIT:
        {
            if(app_bt_vars)
            {
                COS_Free(app_bt_vars);
                app_bt_vars = NULL;
            }
        }
#if CHIP_BTOVERLAY_SUPPORTED!=0
        hal_BTOverlayLoad(HAL_BTOVERLAY_FILESYSTEM);            
#endif        
        break;

        case AP_MSG_STANDBY:
        {
            BOOL ret = FALSE;
            hal_HstSendEvent(SYS_EVENT, 0x03281049);

#if APP_SUPPORT_HFP == 1
            if (app_get_hfp_state() >= HFP_CONNECTED)
            {
                ret = TRUE;
            }
#endif

#if APP_SUPPORT_A2DP== 1
            if (app_get_a2dp_state() >= A2DP_CONNECTED)
            {
                ret = TRUE;
            }
#endif
            return ret;
        }
        break;

        default:
        {
            if(app_bt_main_key_handle(MESSAGE_ConvertToKeyValue(pEvent)))
            {
                break;
            }
            else
            {
                //Common events, DONOT deal it.
                return FALSE;
            }
        }
        break;	
    }

    return TRUE;
}

void app_bt_msg_handle(COS_EVENT *ev)
{	
    u_int32 id = ((ev->nEventId - RDABT_EXTNAL_MSG_FLAG)>>8);

    if(!app_bt_vars) //added to fix bug1453
        return;

    app_mgr_bt_state_machine(ev);
    
#if APP_SUPPORT_HFP == 1	
    app_hfp_bt_state_machine(ev);
#endif

#if APP_SUPPORT_A2DP == 1
    app_a2dp_bt_state_machine(ev);
#endif

    if(ev->nEventId == EV_AUDIO_INTERNAL_PLAY_FINISH_IND 
    ||ev->nEventId == EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND)
    {
        app_mgr_audio_play();
    }

    switch(id)
    {
        case 0:
        {
            app_common_msg_handle(ev);
        }
        break;    
        
        case RDABT_ADP:
        {
            app_adp_msg_handle(ev);
        }
        break;
        
        case RDABT_HCI:
        {
            app_hci_msg_handle(ev);
        }
        break;
        
        case RDABT_MANAGER:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle mgr event:%d, nparam=0x%x", ev->nEventId, ev->nParam1);
            app_mgr_msg_handle(ev);
        }    
        break; 
        
        case RDABT_OBEX_CLI:
        {
#if APP_SUPPORT_PBAP==1
            app_pbap_msg_handle(ev);
#endif
#if APP_SUPPORT_MAP==1
            app_map_msg_handle(ev);
#endif
        }
        break;
        
        case RDABT_OBEX_SRV:
        {
#if APP_SUPPORT_MAP==1
        app_map_msg_handle(ev);
#endif       
        }
        break;  

#if APP_SUPPORT_A2DP==1     
        case RDABT_A2DP:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle a2dp event:%d, nparam=%d", ev->nEventId, ev->nParam1);
            app_a2dp_msg_handle(ev);
        }    
        break; 
#endif        

#if APP_SUPPORT_AVRCP==1
        case RDABT_AVRCP:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle avrcp event:%d, nparam=%d", ev->nEventId, ev->nParam1);
            app_avrcp_msg_handle(ev);
        }    
        break; 
#endif

#if APP_SUPPORT_HFP==1
        case RDABT_HFP:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle hfp event:%d, nparam=%d", ev->nEventId, ev->nParam1);
            app_hfp_msg_handle(ev);
        }    
        break;   
#endif

#if APP_SUPPORT_HID==1
        case RDABT_HID:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle hid event:%d, nparam=%d", ev->nEventId, ev->nParam1);
            app_hid_msg_handle(ev);
        }    
        break;        
#endif

#if APP_SUPPORT_SPP==1
        case RDABT_SPP:
        {
            app_trace(APP_BT_TRC, "app_bt_msg_handle spp event:%d, nparam=%d", ev->nEventId, ev->nParam1);
            app_spp_msg_handle(ev);
        }
        break;    
#endif

#if APP_SUPPORT_LE==1
        case RDABT_GATT:
        {
            app_gatt_process_msg(ev);
        }
        break;
#endif
    }    
    
#if APP_SUPPORT_TEST_BOX == 1
    app_test_box_msg_handle(ev);
#endif
}

void app_bt_set_address(t_bdaddr* addr)
{
    if(addr == NULL)
    {
        return ;
    }

    if(app_bt_vars == NULL)
    {
        app_bt_init();
    }

    memcpy(app_bt_vars->local_addr.bytes, addr->bytes, SIZE_OF_BDADDR);
    memcpy(app_bt_vars->le_addr.bytes, addr->bytes, SIZE_OF_BDADDR);
    app_mgr_store();
}

u_int8 *app_bt_get_address(void)
{
    if (app_bt_vars == NULL)
    {
        app_bt_init();
    }

    return app_bt_vars->local_addr.bytes;
}

void app_bt_entry(void)
{
#if APP_SUPPORT_HID_MODE==1    
    hal_HstSendEvent(SYS_EVENT, 0x06050001);
    hal_HstSendEvent(SYS_EVENT, g_current_BT_module);
#endif    
	EntryNewScreen(SCR_ID_BT_MAIN, app_bt_main_msg_handle, NULL, 0, 0);
}

void app_change_pincode_test(void)
{
    COS_EVENT event = {0};
    //app_get_pbap_size(&app_bt_vars->device_list[app_bt_vars->last_device]);
    app_bt_vars->password[0]++;

    app_bt_vars->password[1]++;
    app_bt_vars->password[2]++;
    app_bt_vars->password[3]++;
    hal_HstSendEvent(SYS_EVENT, 0x20170622);

    hal_HstSendEvent(SYS_EVENT,  app_bt_vars->password[3]);
    hal_HstSendEvent(SYS_EVENT, 0x20170623);

    event.nEventId = RDABT_MODIFY_PINCODE_REQ;
    event.nParam1 = (UINT32)app_bt_vars;
    COS_SendEvent(MOD_BT,  &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    hal_HstSendEvent(SYS_EVENT, 0x20170624);

}
#endif

#if APP_SUPPORT_FCC==1
FCC_TEST_PARAM fcc_param;
void app_fcc_test(u_int32 param, u_int8  hop_frq, u_int8 power_level, u_int8 tx_rx)
{                                       
    COS_EVENT ev = {0};

    ev.nEventId = RDABT_FCC_TEST_REQ;
    fcc_param.param = param;
    fcc_param.hop_frq = hop_frq;
    fcc_param.power_level = power_level;
    fcc_param.tx_rx = tx_rx;
    app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    ev.nParam1 = (u_int32)&fcc_param;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_fcc_test_stop(void)
{                                       
    COS_EVENT ev = {0};

    ev.nEventId = RDABT_FCC_TEST_STOP_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
#endif

