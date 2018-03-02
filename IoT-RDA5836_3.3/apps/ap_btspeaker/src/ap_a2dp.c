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

#include "ap_a2dp.h"
#if APP_SUPPORT_A2DP==1
#include"resource_id.h"
#include "MainTask.h"
#include "ap_gui.h"
#include "ap_hfp.h"
#include "mci.h"
#include "ap_media.h"

static u_int8 s_a2dp_state = A2DP_IDLE;
static APP_A2DP_ADP_INFO *s_a2dp_adp_info = NULL;
u_int8 a2dp_connect_role = 0;//0:cnf  1:ind

#if APP_SUPPORT_SOUNDBAR == 1
#include "dm.h"
#include "ap_spp.h"
#endif

#if APP_SUPPORT_A2DP_SOURCE==1
static RDABT_A2DP_DATA_INFO app_a2dp_data = {{{0,}}, 0};
static u_int8 a2dp_data_send_flag = 1;
#endif

APP_A2DP_ADP_INFO *app_get_a2dp_adp_info(void)
{
    if(app_get_mgr_state() <= MGR_ACTIVE_PENDING)
    {
        return NULL;
    }
    
    return s_a2dp_adp_info;
}

void app_clean_a2dp_adp_info(void)
{
    s_a2dp_adp_info = NULL;    
}

u_int8 app_get_a2dp_state(void)
{
    return s_a2dp_state;
}

void app_set_a2dp_state(u_int8 state)
{
#if APP_SUPPORT_SOUNDBAR == 0 /*&& APP_SUPPORT_BT_AUTOVISIBLE == 1*/
    if(state == A2DP_PLAY)
    {
        app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    }
    else if(s_a2dp_state == A2DP_PLAY && state != A2DP_PLAY)
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
    s_a2dp_state = state;
	hal_HstSendEvent(SYS_EVENT, 0x20170034);
	hal_HstSendEvent(SYS_EVENT, s_a2dp_state);

}

APP_A2DP_DEVICE_INFO *app_get_a2dp_idle_device(void)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    t_bdaddr dump_addr = {{0}};
    
    if(a2dp_adp_info != NULL)
    {
        for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
        {
            if(!memcmp(a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr.bytes, dump_addr.bytes, SIZE_OF_BDADDR)
                &&
                a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state == A2DP_ADP_STATE_IDLE)
            {
                return &a2dp_adp_info->a2dp_device_info_list[i];
            }
        }

        for(i = 0; i < a2dp_adp_info->a2dp_max_device_num; i++)
        {
            if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state == A2DP_ADP_STATE_IDLE)
            {
                return &a2dp_adp_info->a2dp_device_info_list[i];
            }
        }
    }
    return NULL;
}

u_int8 app_check_a2dp_connection(void)
{
    u_int8 i = 0;
    u_int8 ret = 0;

    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    
    if(a2dp_adp_info != NULL)    
    {
        for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
        {
            if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state >= A2DP_ADP_STATE_CONNECTED)
            {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void app_a2dp_check_device_conneciton(void *pArgs)
{
    APP_A2DP_DEVICE_INFO *device_info = (APP_A2DP_DEVICE_INFO *)pArgs;
    device_info->a2dp_check_timer = 0;
    
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

    hal_HstSendEvent(SYS_EVENT, 0x01141055);
    hal_HstSendEvent(SYS_EVENT, device_info->check_cnt);
    if(device_info->a2dp_adp_state == A2DP_IDLE)
    {
        app_connect_a2dp(&device_info->a2dp_addr);
        device_info->check_cnt++;
        if(device_info->check_cnt < 3)
        {
            if(device_info->check_cnt == 0)
            {
                device_info->a2dp_check_timer = mmi_timer_create(60, app_a2dp_check_device_conneciton, device_info, COS_TIMER_MODE_SINGLE);
            }
        }
    }
    else
    {
        device_info->check_cnt = 0;
    }
}

APP_A2DP_DEVICE_INFO *app_find_a2dp_device(t_bdaddr addr)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(a2dp_adp_info == NULL)
    {
        return NULL;
    }
    
    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(!memcmp(addr.bytes, a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr.bytes, SIZE_OF_BDADDR))
        {
            return &a2dp_adp_info->a2dp_device_info_list[i];
        }
    }
    return NULL;
}

void app_create_a2dp_connection(u_int32 timeoutTenths, t_bdaddr addr)
{
    APP_A2DP_DEVICE_INFO *a2dp_device = app_find_a2dp_device(addr);

    if(a2dp_device == NULL)
    {
        a2dp_device = app_get_a2dp_idle_device();
        if(a2dp_device != NULL)
        {
            memset(a2dp_device, 0, sizeof(APP_A2DP_DEVICE_INFO));
            a2dp_device->a2dp_addr = addr;
        }
    }
    
    if(a2dp_device != NULL)
    {
        if(a2dp_device->a2dp_adp_state == A2DP_ADP_STATE_IDLE)
        {
            if(a2dp_device->a2dp_check_timer == 0)
            {
                a2dp_device->check_cnt = 0;
#if APP_SUPPORT_A2DP == 1
                a2dp_device->a2dp_check_timer = 
                    mmi_timer_create(timeoutTenths, app_a2dp_check_device_conneciton, a2dp_device, COS_TIMER_MODE_SINGLE);
#endif
            }
        }
    }
}

void app_cancel_create_a2dp_connection(t_bdaddr addr)
{
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    APP_A2DP_DEVICE_INFO *a2dp_device = app_find_a2dp_device(addr);

    if(a2dp_adp_info == NULL || a2dp_device == NULL)
    {
        return ;
    }

    if(a2dp_device->a2dp_check_timer != 0)
    {   
        mmi_cancel_timer(a2dp_device->a2dp_check_timer);
        a2dp_device->a2dp_check_timer = 0; 
    }
}

void app_a2dp_clean_timer(void)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(a2dp_adp_info == NULL)
    {
        return ;
    }

    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_check_timer != 0)
        {   
            mmi_cancel_timer(a2dp_adp_info->a2dp_device_info_list[i].a2dp_check_timer);
            a2dp_adp_info->a2dp_device_info_list[i].a2dp_check_timer = 0; 
        }
    }
}
void app_a2dp_play_sbc_after(void)
{
    u_int8 i = 0;
    APP_A2DP_DEVICE_INFO *a2dp_device = NULL;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();    
    if(a2dp_adp_info == NULL)
    {
        return ;
    }
    
    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state == A2DP_ADP_STATE_PLAY)
        {
            a2dp_device = &a2dp_adp_info->a2dp_device_info_list[i];
            break;
        }
    }        

    if(a2dp_device != NULL && a2dp_device->a2dp_param != 0)
    {
        mediaSendBTCommand(MC_STOP, 0);
        mediaSendBTCommand(MC_PLAY_A2DP, (u_int32)&a2dp_adp_info->a2dp_config.audio_cap);
        app_set_a2dp_state(A2DP_PLAY);
        mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);
        app_a2dp_drop_other_deivce(a2dp_device->a2dp_addr);
        hal_HstSendEvent(SYS_EVENT, 0x08221712);
        hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume_a2dp);
    }
}

void app_a2dp_play_sbc_filter(t_bdaddr addr)
{
    APP_AVRCP_DEVICE_INFO *avrcp_info = app_find_avrcp_device(addr);
    APP_A2DP_DEVICE_INFO *a2dp_info = app_find_a2dp_device(addr);
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    
    if(a2dp_info != NULL)
    {
        if(avrcp_info == NULL || avrcp_info->Cur_Play_Status == AVRCP_STATE_PLAYING)
        {
            mediaSendBTCommand(MC_STOP, 0);
            mediaSendBTCommand(MC_PLAY_A2DP, (u_int32)&a2dp_adp_info->a2dp_config.audio_cap);
            app_set_a2dp_state(A2DP_PLAY);
            mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);
            hal_HstSendEvent(SYS_EVENT, 0x08221713);
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume_a2dp);
        }
    }
}

void app_a2dp_play_sbc(t_bdaddr addr)
{
    APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(addr);
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    hal_HstSendEvent(SYS_EVENT, 0x05041111);
    hal_HstSendEvent(SYS_EVENT, (u_int32)device_info);
    
#if APP_SUPPORT_SOUNDBAR == 0
        if(device_info != NULL && app_get_a2dp_state() > A2DP_PLAY_AUDIO)
        {
            hal_HstSendEvent(SYS_EVENT, device_info->a2dp_param);
            hal_HstSendEvent(SYS_EVENT, app_get_a2dp_state());
            if(app_get_a2dp_state() != A2DP_PLAY)
            {
                mediaSendBTCommand(MC_STOP, 0);
                mediaSendBTCommand(MC_PLAY_A2DP, (u_int32)&a2dp_adp_info->a2dp_config.audio_cap);
                app_set_a2dp_state(A2DP_PLAY);
                mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);
                hal_HstSendEvent(SYS_EVENT, 0x08221714);
                hal_HstSendEvent(SYS_EVENT, app_bt_vars->volume_a2dp);
            }
        }
#else
        if(device_info != NULL)
        {
            hal_HstSendEvent(SYS_EVENT, device_info->a2dp_param);
            hal_HstSendEvent(SYS_EVENT, app_get_a2dp_state());
            if(app_get_a2dp_state() != A2DP_PLAY)
            {
                mediaSendBTCommand(MC_STOP, 0);
                mediaSendBTCommand(MC_PLAY_A2DP, (u_int32)&a2dp_adp_info->a2dp_config.audio_cap);
                app_set_a2dp_state(A2DP_PLAY);
                mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);
            }
        }
    
#endif

}

void app_a2dp_drop_sbc(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};

    MMC_AudioCleanA2dpData();
    hal_HstSendEvent(1, 0x05051543);
	hal_HstSendEvent(1, pAddr->bytes[0]);
	hal_HstSendEvent(1, pAddr->bytes[1]);
	hal_HstSendEvent(1, pAddr->bytes[2]);
	hal_HstSendEvent(1, pAddr->bytes[3]);
	hal_HstSendEvent(1, pAddr->bytes[4]);
	hal_HstSendEvent(1, pAddr->bytes[5]);
    
	event.nEventId = RDABT_A2DP_DROP_SBC_REQ;
    event.nParam1 = (u_int32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);    
}

void app_a2dp_drop_other_deivce(t_bdaddr addr)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(a2dp_adp_info == NULL)
    {
        return ;
    }

    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state >= A2DP_ADP_STATE_CONNECTED
            &&
            memcmp(a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr.bytes, addr.bytes, SIZE_OF_BDADDR))
        {
            app_a2dp_drop_sbc(&a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr);
            return ;
        }
    }
}

void app_a2dp_resume_sbc(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};

    hal_HstSendEvent(1, 0x05051544);
	hal_HstSendEvent(1, pAddr->bytes[0]);
	hal_HstSendEvent(1, pAddr->bytes[1]);
	hal_HstSendEvent(1, pAddr->bytes[2]);
	hal_HstSendEvent(1, pAddr->bytes[3]);
	hal_HstSendEvent(1, pAddr->bytes[4]);
	hal_HstSendEvent(1, pAddr->bytes[5]);
    
	event.nEventId = RDABT_A2DP_RESUME_SBC_REQ;
    event.nParam1 = (u_int32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_a2dp_update_last_device(void)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(a2dp_adp_info == NULL)
    {
        return ;
    }
    
    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state >= A2DP_ADP_STATE_CONNECTED)
        {
            a2dp_adp_info->last_addr = a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr;
            break;
        }
    }
}

BOOL app_a2dp_is_active(void)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    BOOL ret = FALSE;
    
    if(a2dp_adp_info == NULL)
    {
        return ret;
    }

    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state == A2DP_ADP_STATE_PLAY)
        {
            a2dp_adp_info->last_addr = a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr;
            ret = TRUE;
            break;
        }
    }
    return ret;
}

BOOL app_a2dp_check_active(t_bdaddr addr)
{
    u_int8 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
    BOOL ret = FALSE;
    
    if(a2dp_adp_info == NULL)
    {
        return ret;
    }

    for(; i < a2dp_adp_info->a2dp_max_device_num; i++)
    {
        if(a2dp_adp_info->a2dp_device_info_list[i].a2dp_adp_state == A2DP_ADP_STATE_PLAY
            &&
            memcmp(a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr.bytes, addr.bytes, SIZE_OF_BDADDR))
        {
            a2dp_adp_info->last_addr = a2dp_adp_info->a2dp_device_info_list[i].a2dp_addr;
            ret = TRUE;
            break;
        }
    }
    return ret;
}

int app_find_a2dp_device_state(t_bdaddr addr)
{
	APP_A2DP_DEVICE_INFO *a2dp_device = app_find_a2dp_device(addr);
	if(a2dp_device != NULL)
	{
	    return a2dp_device->a2dp_adp_state;
	}
	return NULL;
}

void app_a2dp_stop_sbc(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x05040000);
    hal_HstSendEvent(SYS_EVENT, app_get_a2dp_state());
    hal_HstSendEvent(SYS_EVENT, app_get_hfp_state());
    if(
#if APP_SUPPORT_HFP == 1
        app_get_hfp_state() != HFP_PLAY_SCO
        &&
#endif
        app_get_a2dp_state() == A2DP_PLAY)
    {
        mediaSendBTCommand(MC_STOP, 0);
        MMC_AudioCleanA2dpData();
        app_set_a2dp_state(A2DP_CONNECTED);
    }
}

void app_a2dp_bt_state_machine(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
        case RDABT_A2DP_SIGNAL_CONNECTED_CNF_MSG:
		case RDABT_A2DP_SIGNAL_CONNECTED_IND_MSG:
        {
			RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;
			if(msg_info->msg_param == RDABT_NOERROR)
			{
	            if(app_get_a2dp_state() == A2DP_IDLE)
	            {
	                app_set_a2dp_state(A2DP_CONNECTED);
	            }	
			}
        }
        break;
        case RDABT_A2DP_SIGNAL_DISCONNECTED_IND_MSG:
		case RDABT_A2DP_SIGNAL_DISCONNECTED_CNF_MSG:
        {
            //app_a2dp_stop_sbc();    
            if(!app_check_a2dp_connection())
            {
                app_set_a2dp_state(A2DP_IDLE);
            }
        }
        break;
    }
}

#if APP_SUPPORT_SOUNDBAR == 1
void app_send_audio_sync_data(SOUNBAR_AUDIO_SYNC_STRUCT* audio_sync)
{
	COS_EVENT event = {0};
	hal_HstSendEvent(1, 0x20170228);
	hal_HstSendEvent(1, audio_sync->current_clk);
	hal_HstSendEvent(1, audio_sync->counterNum);
	event.nEventId = RDABT_A2DP_SEND_AUDIO_SYNC_CLK_REQ;
	event.nParam1 = (UINT32)audio_sync;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
#endif

#if APP_SUPPORT_A2DP_SOURCE==1

void app_a2dp_send_data(t_bdaddr addr, u_int32 length, 
    u_int8 *p_data, u_int32 timestamp, u_int16 seq_num,
    u_int8 payload_type, u_int8 marker, u_int8 frames)
{
    COS_EVENT event = {0};
    
    app_a2dp_data.a2dp_addr = addr;
    app_a2dp_data.length = length;
    app_a2dp_data.p_data = p_data;
    app_a2dp_data.timestamp = timestamp;
    app_a2dp_data.seq_num = seq_num;
    app_a2dp_data.payload_type = payload_type;
    app_a2dp_data.marker = marker;
    app_a2dp_data.frames = frames;

    event.nEventId = RDABT_A2DP_SEND_DATA_REQ;
    event.nParam1 = (u_int32)&app_a2dp_data;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_a2dp_send_start(t_bdaddr *addr)
{
    COS_EVENT event = {0};
	event.nEventId = RDABT_A2DP_SEND_START_REQ;
    event.nParam1 = (u_int32)addr;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_a2dp_send_suspend(t_bdaddr *addr)
{
    COS_EVENT event = {0};
	event.nEventId = RDABT_A2DP_SEND_SUSPEND_REQ;
    event.nParam1 = (u_int32)addr;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_a2dp_send_data_handle(void *pArgs)
{
	static UINT32 timestamp;
	A2DP_SOURCE_MSG *lineindata = (A2DP_SOURCE_MSG *)pArgs;
	//static u_int32 i = 0;
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
	if(a2dp_data_send_flag)
	{
		if(app_a2dp_data.seq_num==0xffff)
		    app_a2dp_data.seq_num = 0;
		app_a2dp_data.seq_num++;
		timestamp += 1152;
		app_a2dp_send_data(a2dp_adp_info->last_addr, lineindata->sbc_data_len, lineindata->sbc_data, timestamp, app_a2dp_data.seq_num, 0x60, 0, lineindata->frame);
		a2dp_data_send_flag = 0;
	}
	else
	{		
		hal_HstSendEvent(SYS_EVENT, 0x10201517);
	}
}

void app_a2dp_send_start_to_voc(u_int16 source_mtu)
{
 	COS_EVENT event={0,0};
	bt_a2dp_audio_cap_struct audio_cap = {0};
	audio_cap.codec_type = AVDTP_MEDIA_CODEC_I2S_PCM;
 	event.nEventId = MSG_LINEIN_START_SEND_SBC;	
 	event.nParam1 = source_mtu;
	hal_HstSendEvent(0xff, 0x20161400);
#if APP_SUPPORT_TEST_BOX == 0
	mediaSendBTCommand(MC_PLAY_A2DP, &audio_cap);
#endif
	COS_SendEvent(MOD_MED, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#endif

void app_connect_a2dp(t_bdaddr *pAddr)
{
	COS_EVENT event = {0};
    t_bdaddr null_addr = {{0}};
    
	hal_HstSendEvent(1, 0x04211411);
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
	event.nEventId = RDABT_A2DP_CONNECT_REQ;
	event.nParam1 = (UINT32)pAddr;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_a2dp(t_bdaddr *pAddr)
{
	COS_EVENT event = {0};

	hal_HstSendEvent(1, 0x04211412);
	hal_HstSendEvent(1, pAddr->bytes[0]);
	hal_HstSendEvent(1, pAddr->bytes[1]);
	hal_HstSendEvent(1, pAddr->bytes[2]);
	hal_HstSendEvent(1, pAddr->bytes[3]);
	hal_HstSendEvent(1, pAddr->bytes[4]);
	hal_HstSendEvent(1, pAddr->bytes[5]);
	event.nEventId = RDABT_A2DP_DISCONNECT_REQ;
	event.nParam1 = (UINT32)pAddr;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_disconnect_a2dp_all(void)
{
	COS_EVENT event = {0};

	event.nEventId = RDABT_A2DP_DISCONNECT_ALL_REQ;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_a2dp_get_adp_info_req(void)
{
	COS_EVENT event = {0};
	event.nEventId = RDABT_A2DP_GET_ADP_INFO_REQ;
	COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


void app_a2dp_switch_mode(void)
{
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(a2dp_adp_info == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0008);
        return ;
    }
    app_a2dp_clean_timer();        
    if(a2dp_adp_info != NULL && a2dp_adp_info->a2dp_connect_index > 0)
    {
        app_disconnect_a2dp_all();
    }   
}


void app_a2dp_handle_key(u_int32 key)
{
    APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();

    if(key == 0 || key == AP_MSG_RTC)
    {
        return ;
    }
    
    if(a2dp_adp_info == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0008);
        return ;
    }

    switch(key)
    {
        
        
    }
}
    
void app_a2dp_msg_handle(COS_EVENT *ev)
{
	COS_EVENT event = {0};
    static u_int16 mtu = 0;

	switch(ev->nEventId)
	{
		case RDABT_A2DP_SIGNAL_CONNECTED_CNF_MSG:
		case RDABT_A2DP_SIGNAL_CONNECTED_IND_MSG:        
		{
			RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;
            APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
            APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(msg_info->a2dp_addr);
			hal_HstSendEvent(1, 0x04211413);
            hal_HstSendEvent(1, msg_info->msg_param);
			if(ev->nEventId == RDABT_A2DP_SIGNAL_CONNECTED_CNF_MSG)
				a2dp_connect_role = 0;
			else
				a2dp_connect_role = 1;
            if(msg_info->msg_param == RDABT_NOERROR)
            {   
                app_cancel_create_a2dp_connection(msg_info->a2dp_addr);
    			if (!IS_BT_MODE() || app_get_mgr_state() <= MGR_CLOSE_PENDING)
    			{
    			    if(device_info != NULL)
                    {         
    		    		app_disconnect_a2dp(&device_info->a2dp_addr);
                    }
                    break;
    			}
                else
                {
                    a2dp_adp_info->last_addr = msg_info->a2dp_addr;
                }
#if  APP_SUPPORT_ONLY_ONE_LINK == 0
#if  APP_SUPPORT_SOUNDBAR == 1 || APP_SUPPORT_BT_AUTOVISIBLE == 0
    			app_set_bt_visible_req(MGR_SET_CONNECTABLE);
#endif
#else
                app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
#endif


#if APP_SUPPORT_HFP == 1
                if(app_get_hfp_state() <= HFP_CONNECTED)
#endif
                {
                    mediaSendBTCommand(MC_STOP, 0);
        			media_PlayInternalAudio(GUI_AUDIO_BT_CONNECT, 1, 0);
                    app_set_a2dp_state(A2DP_PLAY_AUDIO);
                }
            }
		}
		break; 
        case RDABT_A2DP_MEDIA_CONNECTED_CNF_MSG:
        {
            RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;    
            APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(msg_info->a2dp_addr);
            mtu = msg_info->msg_param1;
            hal_HstSendEvent(SYS_EVENT, 0x10201539);
            hal_HstSendEvent(SYS_EVENT, mtu);
            if (!IS_BT_MODE() || app_get_mgr_state() <= MGR_CLOSE_PENDING)
			{
			    if(device_info != NULL)
                {         
		    		app_disconnect_a2dp(&device_info->a2dp_addr);
                }
                break;
			}
#if APP_SUPPORT_TEST_BOX==0	 
#if APP_SUPPORT_A2DP_SOURCE==1
            app_a2dp_send_start(&device_info->a2dp_addr);
#else
#if APP_SUPPORT_HFP==1	
            {
                app_create_hfp_connection(100, device_info->a2dp_addr);
#if APP_SUPPORT_SOUNDBAR == 0
				if(a2dp_connect_role == 0)
				{
                    u_int32 device_index = 0;				
				    device_index = app_bt_vars->last_device;
					app_bt_connect_retry((void *)device_index);
				}
#endif
                hal_HstSendEvent(SYS_EVENT, 0x2017061a);
            }
#endif
#endif
#endif

        }
        break;
		case RDABT_A2DP_SIGNAL_DISCONNECTED_IND_MSG:
		case RDABT_A2DP_SIGNAL_DISCONNECTED_CNF_MSG:
		{
			RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;
            APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(msg_info->a2dp_addr);
			hal_HstSendEvent(1, 0x04141502);
			hal_HstSendEvent(1, msg_info->msg_param);			
            app_cancel_create_hfp_connection((msg_info->a2dp_addr));

			if(
#if APP_SUPPORT_HFP == 1
            app_get_hfp_state() <= HFP_CONNECTED &&
#endif
			!app_a2dp_check_active(msg_info->a2dp_addr))
            {
                mediaSendBTCommand(MC_STOP, 0);
    			media_PlayInternalAudio(GUI_AUDIO_BT_DISCONNECT, 1, 0);
                app_set_a2dp_state(A2DP_PLAY_AUDIO);
                MMC_AudioCleanA2dpData();
            }

            if(device_info != NULL)
            {
                device_info->a2dp_param = 0;
            }
            app_a2dp_update_last_device();
		}
		break;    
        case RDABT_A2DP_SEND_AUDIO_START_MSG:
        {
            hal_HstSendEvent(1, 0x20170208);
#if APP_SUPPORT_SOUNDBAR == 1  
            MMC_SyncAudioPlay();
#endif
        }
        break;
		case RDABT_A2DP_MEDIA_DATA_IND_MSG:
		{
            static u_int8 sbc_counter = 0;
            APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
            RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;
            APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
            if(avrcp_adp_info != NULL)
            {
                avrcp_adp_info->last_addr = msg_info->a2dp_addr;
            }
#ifndef WIN32                
			if(app_get_a2dp_state() == A2DP_PLAY)
			{
				event.nEventId = MSG_AUDIODEC_A2DP;
				event.nParam1 = msg_info->msg_param;
				COS_SendEvent(MOD_MED,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
			}
			else
			{
				hal_HstSendEvent(1, 0x20161036);
				hal_HstSendEvent(1, app_get_a2dp_state());
                hal_HstSendEvent(1, app_get_hfp_state());
                if(sbc_counter++>2 && app_get_hfp_state() <= HFP_CONNECTED && 
                    app_get_a2dp_state() == A2DP_CONNECTED && a2dp_adp_info != NULL)
                {
                    app_a2dp_play_sbc(msg_info->a2dp_addr);
                    sbc_counter = 0;
                }
                //else
                {
    				event.nEventId = RDABT_A2DP_DATA_COMPLETED_MSG;
    				event.nParam1 = msg_info->msg_param;
    				COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    				MMC_AudioCleanA2dpData();
                }
			}
#else
			event.nEventId = RDABT_A2DP_DATA_COMPLETED_MSG;
			event.nParam1 = msg_info->msg_param;
			COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#endif
		}
		break;
		case RDABT_A2DP_SEND_SEP_START_IND_MSG:
		{
			RDABT_A2DP_MSG_INFO *msg_p = (RDABT_A2DP_MSG_INFO *)ev->nParam1;
            APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(msg_p->a2dp_addr);
            APP_AVRCP_ADP_INFO *avrcp_adp_info = app_get_avrcp_adp_info();
            
            hal_HstSendEvent(SYS_EVENT, 0x20150906);
            if(device_info != NULL)
            {
                device_info->a2dp_param = msg_p->msg_param;
            }
            hal_HstSendEvent(SYS_EVENT, (u_int32)device_info);
#if APP_SUPPORT_SOUNDBAR == 1
            csw_SetResourceActivity(3, 104000000);
            if(Soundbar_App_Get_Role_Status() == SOUNDBAR_SLAVE)
                return;
#endif            

            if(device_info != NULL)
            {
                app_a2dp_play_sbc(device_info->a2dp_addr);
                if(avrcp_adp_info != NULL)
                {
                    avrcp_adp_info->last_addr = msg_p->a2dp_addr;
                }
            }
		}
		break;
		case RDABT_A2DP_SEND_SEP_PAUSE_IND_MSG:
		{
            hal_HstSendEvent(SYS_EVENT, 0x20150a06);

            app_a2dp_stop_sbc();
		}
		break;        
		case RDABT_A2DP_ADP_INFO_MSG:
		{
            s_a2dp_adp_info = (APP_A2DP_ADP_INFO *)ev->nParam1;
		}
		break;   
		case RDABT_A2DP_AUDIO_SYNC_CLK_MSG:
		{
            event.nEventId = MSG_AUDIODEC_CONTROL;
            event.nParam1 = ev->nParam1;
#if APP_SUPPORT_SOUNDBAR == 1
            COS_SendEvent(MOD_MED,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#endif
		}
		break;
        case RDABT_A2DP_SEND_SEP_DISCOVER_CNF_MSG:
        {
            
        }
        break;   
#if APP_SUPPORT_A2DP_SOURCE==1
        case RDABT_A2DP_START_RSP_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x10261409);    
            hal_HstSendEvent(SYS_EVENT, mtu);    
            app_a2dp_send_start_to_voc(mtu);
        }
        break;   
        case RDABT_A2DP_SEND_DATA_CNF_MSG:
        {
            a2dp_data_send_flag = 1;
        }
        break;
#endif

	}
}

#endif

