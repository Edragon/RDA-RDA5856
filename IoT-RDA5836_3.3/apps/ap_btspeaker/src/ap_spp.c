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

#include "ap_spp.h"
#include"resource_id.h"
#ifndef WIN32
#include "hal_gpio.h"
#endif
#include "ap_mgr.h"
#include "ap_media.h"
#include "dm.h"

#if APP_SUPPORT_SPP ==1

u_int8 spp_data_buf[128] = {0};
RDABT_SPP_SEND_DATA_STRUCT spp_sd = {0};


#if  APP_SUPPORT_SOUNDBAR == 1
RDABT_SPP_ADP_INFO soundbar_device = {{{0}}, 0};
static u_int8 soundbar_role = SOUNDBAR_NOROLE;
static UINT8 soundbar_auto_connect_timer = 0;
static u_int8 soundbar_shutdown_ok = 0;
static u_int8 soundbar_last_phone_device = 0xff;
extern PUBLIC VOID csw_SetResourceActivity(CSW_LP_RESOURCE_T resource, CSW_SYS_FREQ_T freq);
extern PUBLIC VOID hal_TimWatchDogClose(VOID);

void app_set_soundbar_last_phone_device(u_int8 phone_last_device)
{
    soundbar_last_phone_device = phone_last_device;
}

u_int8 app_get_soundbar_last_phone_device(void)
{
    return soundbar_last_phone_device;
}


void app_send_soundbar_volume(u_int32 vol)
{
    COS_EVENT event = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170021);
    hal_HstSendEvent(SYS_EVENT, vol);
    
    if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
    {
        event.nEventId = RDABT_SPP_SEND_SOUNDBAR_VOLUME;
        event.nParam1 = vol;
        COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL); 
    }
    else if(Soundbar_App_Get_Role_Status() == SOUNDBAR_SLAVE)
    {
        spp_data_buf[0] = SOUNDBAR_VOLUME_CHANGE;
        spp_data_buf[1] = (u_int8)vol;
        spp_sd.data_length = SOUNDBAR_CONTROL_TWO;
        
        if(spp_sd.data_buf == NULL)
        {
            hal_HstSendEvent(SYS_EVENT, 0xeeee0012);
            return;
        }
        spp_sd.data_buf->buf = spp_data_buf;
        app_spp_send_data_req(&spp_sd);
    }
}

void app_send_soundbar_hfp_state(u_int8 state)
{
    //COS_EVENT event = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170058);
    hal_HstSendEvent(SYS_EVENT, state);
    if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
    {
        spp_data_buf[0] = SOUNDBAR_HFP_STATE;
        spp_data_buf[1] = state;
        spp_sd.data_length = SOUNDBAR_CONTROL_TWO;
        
        if(spp_sd.data_buf == NULL)
        {
            hal_HstSendEvent(SYS_EVENT, 0xeeee0011);
            return;
        }
        spp_sd.data_buf->buf = spp_data_buf;
        app_spp_send_data_req(&spp_sd);
    }

}

void app_send_spp_shutdown_complet(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x20170008);
    hal_HstSendEvent(SYS_EVENT, soundbar_shutdown_ok);
    ClearProtocolEventHandler(RDABT_SPP_DATA_FINISHED_MSG);
    
    if(soundbar_shutdown_ok)
    {
        COS_Sleep(100);
        if(app_switch_mode_handle(RESULT_STANDBY))
            app_set_bt_intercept_switch_mode_result(RESULT_STANDBY);
    }
    else
    {
        app_send_soundbar_spp_shutdown();
    }
}

void app_send_soundbar_spp_shutdown(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x20170009);
    spp_data_buf[0] = SOUNDBAR_SHUTDOWN;
    spp_sd.data_length = sizeof(u_int8);
    if(spp_sd.data_buf == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0xeeee0010);
        return;
    }
    spp_sd.data_buf->buf = spp_data_buf;
    SetProtocolEventHandler((PsFuncPtr)app_send_spp_shutdown_complet, RDABT_SPP_DATA_FINISHED_MSG);
    app_spp_send_data_req(&spp_sd);

}

BOOL app_send_soundbar_shutdown(void)
{
    //COS_EVENT event = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170004);
    
    if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER || Soundbar_App_Get_Role_Status() == SOUNDBAR_SLAVE)
    {
        app_send_soundbar_spp_shutdown();
        return TRUE;
    }    
    return FALSE;
}


void app_set_role_led(u_int8 led,u_int8 clr)
{
    HAL_APO_ID_T Led_blue = {
                                .gpioId= HAL_GPIO_3_0            
                            };
    HAL_APO_ID_T Led_red = {
                                .gpioId= HAL_GPIO_3_1            
                            };
    
    hal_HstSendEvent(SYS_EVENT, 0x20170000);
    hal_HstSendEvent(SYS_EVENT, led);
    hal_HstSendEvent(SYS_EVENT, clr);
    
    switch(led)
    {
        case BLUE_LED:
            if(clr)
                hal_GpioClr(Led_blue);
            else
                hal_GpioSet(Led_blue);
            break;
            
        case RED_LED:
            if(clr)
                hal_GpioClr(Led_red);
            else
                hal_GpioSet(Led_red);
            break;
    }
}

void app_send_support_soundbar(void)
{
    COS_EVENT event = {0};
    hal_HstSendEvent(SYS_EVENT, 0x20170313);
    event.nEventId = RDABT_SPP_SUPPORT_SOUNDBAR;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);    
}

void Soundbar_App_Set_Role(u_int8 role)
{
    soundbar_role = role;   
}
u_int8 Soundbar_App_Get_Role_Status(void)
{
    return soundbar_role;
}

int Soundbar_Play_Times(uint8 flag)
{
    static int soundbar_play_times = 0;
    if(flag == 0)
        return soundbar_play_times;
    else if(flag)
        soundbar_play_times++;
    else
        soundbar_play_times = 0;
    return soundbar_play_times;  
}

void Soundbar_Auto_Connect(void *args)
{
    static u_int8 connect_times = 0;
    soundbar_auto_connect_timer = NULL;
    if(app_get_mgr_state() < MGR_CONNECTED)
    {
        app_spp_connect_req(&soundbar_device.addr);
        connect_times++;
    }
    if(connect_times < 10)
        soundbar_auto_connect_timer = COS_SetTimer(10000, Soundbar_Auto_Connect, NULL, COS_TIMER_MODE_SINGLE);
    else
        connect_times = 0;
}

void app_connect_soundbar_device(void)
{
    APP_BT_FLASH_STORE *flash_info; 
    flash_info = (APP_BT_FLASH_STORE *)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
    
    if(flash_info == NULL || app_bt_vars == NULL)
    {
        hal_DbgAssert("No bt ram");
    }
    
    hal_HstSendEvent(SYS_EVENT, 0x20170508);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_default_role);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[5]);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[4]);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[3]);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[2]);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[1]);
    hal_HstSendEvent(SYS_EVENT, flash_info->soundbar_connect_addr.bytes[0]);
    hal_HstSendEvent(SYS_EVENT, app_get_mgr_state());
    
    if(flash_info->soundbar_default_role == SOUNDBAR_SLAVE && app_get_mgr_state() >= MGR_ACTIVED)
    {
        soundbar_device.addr = flash_info->soundbar_connect_addr;
        Soundbar_Auto_Connect(NULL);
    }
}

void app_spp_soundbar_receive_start_msg(UINT32 param)
{
    RDABT_A2DP_MSG_INFO *msg_p = (RDABT_A2DP_MSG_INFO *)param; 
    //hal_DbgAssert("11");
    if(app_get_a2dp_state() != A2DP_PLAY)
    {
        mediaSendBTCommand(MC_STOP, 0);
        mediaSendBTCommand(MC_PLAY_A2DP, msg_p->msg_param);
        app_set_a2dp_state(A2DP_PLAY);
        mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume_a2dp);
    }
}

#endif
void app_spp_connect_req(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_SPP_CONNECT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_spp_disconnect_req(u_int16 port)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_SPP_DISCONNECT_REQ;
    event.nParam1 = port;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_spp_get_adp_info_req(void)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_SPP_GET_ADP_INFO_REQ;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_spp_send_data_req(RDABT_SPP_SEND_DATA_STRUCT *spp_sd_struct)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_SPP_SEND_DATA_REQ;
    event.nParam1 = (u_int32)spp_sd_struct;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

/*u_int8 datavle[880] = {0};
void product_spp_data(int value)
{
    int i = 0;
    for(;i < value; i++)
    {
        datavle[i] = '1';
    }
}*/
SOUNDBAR_CONTROL_DATA_STRUCT app_soundbar_control_data = {0};
void app_spp_msg_handle(COS_EVENT *ev)
{
    COS_EVENT event = {0};

    switch(ev->nEventId)
    {
        case RDABT_SPP_CONNECTED_CNF_MSG:    
        {
            if(ev->nParam1 == 0)
            {
                app_trace(APP_BT_TRC, "RDABT_SPP_CONNECTED_CNF_MSG");
                u_int16 spp_port = ev->nParam1;
                if(spp_sd.data_buf == NULL)
                {
                    spp_sd.data_buf = (t_DataBuf *)COS_Malloc(sizeof(t_DataBuf));
                }
                //product_spp_data(880);
                //spp_sd.data_buf->buf = datavle;
                //spp_sd.data_length = sizeof(datavle);
                spp_sd.spp_port = spp_port;
                hal_HstSendEvent(1, 0x20160621);
                hal_HstSendEvent(1,spp_sd.spp_port);

                //app_spp_send_data_req(&spp_sd);
#if  APP_SUPPORT_SOUNDBAR == 1
                
                csw_SetResourceActivity(3, 104000000);
                COS_Sleep(500);
                hal_TimWatchDogClose();
                app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
                app_spp_get_adp_info_req();
                Soundbar_App_Set_Role(SOUNDBAR_SLAVE);
                SetProtocolEventHandler(app_spp_soundbar_receive_start_msg, RDABT_A2DP_SEND_SEP_START_IND_MSG);
                if(soundbar_auto_connect_timer)
                {
                    COS_KillTimer(soundbar_auto_connect_timer);
                    soundbar_auto_connect_timer = 0;
                }
                app_set_role_led(RED_LED,0);
#endif
            }
        }
        break;  
        
        case RDABT_SPP_CONNECTED_IND_MSG:   
        {
            hal_HstSendEvent(1, 0x20160620);
            //app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
            if(ev->nParam1 != -1)
            {
                app_trace(APP_BT_TRC, "RDABT_SPP_CONNECTED_IND_MSG");
                u_int16 spp_port = ev->nParam1;
                if(spp_sd.data_buf == NULL)
                {
                    spp_sd.data_buf = (t_DataBuf *)COS_Malloc(sizeof(t_DataBuf));
                }
                spp_sd.spp_port = spp_port;

#if  APP_SUPPORT_SOUNDBAR == 1
                
                csw_SetResourceActivity(3, 104000000);
                COS_Sleep(500);
                hal_TimWatchDogClose();
                app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
                app_spp_get_adp_info_req();
                Soundbar_App_Set_Role(SOUNDBAR_MASTER);
                app_set_role_led(BLUE_LED,0);
#endif
            }
        }
        break;    
        
        case RDABT_SPP_DISCONNECTED_IND_MSG:
        case RDABT_SPP_DISCONNECTED_CNF_MSG:    
        {
            if(spp_sd.data_buf != NULL)
            {
                COS_Free(spp_sd.data_buf);
                spp_sd.data_buf = NULL;
            }
#if  APP_SUPPORT_SOUNDBAR == 1
            csw_SetResourceActivity(3, 32768);
            hal_HstSendEvent(SYS_EVENT, 0x20160329);
            app_set_role_led(BLUE_LED,1);
            app_set_role_led(RED_LED,1);
            if(soundbar_role == SOUNDBAR_SLAVE && soundbar_auto_connect_timer == 0)
            {
                soundbar_auto_connect_timer = COS_SetTimer(10000, Soundbar_Auto_Connect, NULL, COS_TIMER_MODE_SINGLE);
            }
            app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
#endif
        }
        break;
        
        case RDABT_SPP_ADP_INFO_MSG:
        {
            RDABT_SPP_ADP_INFO *msg = (RDABT_SPP_ADP_INFO *)ev->nParam1;
#if  APP_SUPPORT_SOUNDBAR == 1
            memcpy(&soundbar_device,msg,sizeof(RDABT_SPP_ADP_INFO));
            app_mgr_store();
            hal_HstSendEvent(SYS_EVENT, 0x20170505);
            hal_HstSendEvent(SYS_EVENT, app_find_bt_device(soundbar_device.addr));
            hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_device);
            media_PlayInternalAudio(GUI_AUDIO_RECALL, 1, 0);
            if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
            {
                if(app_get_soundbar_last_phone_device() !=0xff 
                    && app_get_soundbar_last_phone_device() != app_find_bt_device(soundbar_device.addr))
                {
                    u_int32 last_device = (u_int32)app_get_soundbar_last_phone_device();
                    hal_HstSendEvent(SYS_EVENT, 0x20170525);
                    app_bt_vars->times_num = 4;
                    app_bt_vars->times_value = 40;
                    app_bt_connect_retry((void*)last_device);
                }
            }
#endif
        }
        break;  
        
        case RDABT_SPP_DATA_IND_MSG:
        {
            //static u_int32 counter = 0;
            RDABT_SPP_SEND_DATA_STRUCT *buf = (RDABT_SPP_SEND_DATA_STRUCT *)ev->nParam1; 
            t_DataBuf *spp_data = buf->data_buf; 
            app_trace(APP_BT_TRC, "RDABT_SPP_data_ind");
            //use spp_data
#if  APP_SUPPORT_SOUNDBAR == 1

            if(Soundbar_App_Get_Role_Status() == SOUNDBAR_MASTER)
            {
                if(spp_data->dataLen == sizeof(UINT8))
			    {
                    if(*(spp_data->buf) == SOUNDBAR_SHUTDOWN)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20170011);
                        if(app_switch_mode_handle(RESULT_STANDBY))
                            app_set_bt_intercept_switch_mode_result(RESULT_STANDBY);
                    }
			    }  
                else if(spp_data->dataLen == SOUNDBAR_CONTROL_TWO)
                {
                    if(*(spp_data->buf) == SOUNDBAR_VOLUME_CHANGE)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20170013);
                        hal_HstSendEvent(SYS_EVENT, (spp_data->buf)[1]);
                        app_bt_vars->volume = (spp_data->buf)[1];
                        mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume);
                    }
                }
            }
            else if(Soundbar_App_Get_Role_Status() == SOUNDBAR_SLAVE)
            {
                if(spp_data->dataLen == sizeof(UINT8))
                {
                    if(*(spp_data->buf) == SOUNDBAR_SBC_MAX_SIZE)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20161111);
                    }else if(*(spp_data->buf) == SOUNDBAR_SBC_MIN_SIZE)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20161100);
                    }else if(*(spp_data->buf) == SOUNDBAR_SHUTDOWN)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20170006);
                        if(app_switch_mode_handle(RESULT_STANDBY))
                            app_set_bt_intercept_switch_mode_result(RESULT_STANDBY);
                    }
                }
                else if(spp_data->dataLen == SOUNDBAR_CONTROL_TWO)
                {
                    if(*(spp_data->buf) == SOUNDBAR_HFP_STATE)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x20170059);
                        hal_HstSendEvent(SYS_EVENT, (spp_data->buf)[1]);
                        app_set_hfp_state((spp_data->buf)[1]);
                    }
                }

                hal_HstSendEvent(SYS_EVENT, 0x20161456);

                if(spp_data->dataLen == sizeof(SOUNDBAR_CONTROL_DATA_STRUCT))
                {
                    memcpy(&app_soundbar_control_data ,(SOUNDBAR_CONTROL_DATA_STRUCT *)spp_data->buf ,sizeof(SOUNDBAR_CONTROL_DATA_STRUCT));
                    if(app_soundbar_control_data.play_flag == SOUNDBAR_SBC_STOP)
                    {
                        hal_HstSendEvent(1, 0x20160723);
                        app_set_a2dp_state(A2DP_CONNECTED);
                        mediaSendBTCommand(MC_STOP, 0);
                    }
                    else if(app_soundbar_control_data.play_flag == SOUNDBAR_SBC_START)
                    {
                        hal_HstSendEvent(1, 0x20160722);
                    }

                }

            }
#endif
            hal_HstSendEvent(1, spp_data->dataLen);
            //use spp_data end
            event.nEventId = RDABT_SPP_FREE_DATA_REQ;
            event.nParam1 = (u_int32)buf->data_buf;
	     COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        }
        break; 
        
        case RDABT_SPP_DATA_FINISHED_MSG:
        {
            int ret = ev->nParam1;
            
#if  APP_SUPPORT_SOUNDBAR == 1   
            if(ret == RDABT_NOERROR)
                soundbar_shutdown_ok = 1;
            else
                soundbar_shutdown_ok = 0;
#endif
            hal_HstSendEvent(1, 0x20160623);
            hal_HstSendEvent(1, ev->nParam1);
        }
        break;
        
        case RDABT_SPP_FLOWCTRL_IND_MSG:
        {
            if(ev->nParam1 == SPP_FLOW_STOP)
            {
            
            }
            else if(ev->nParam1 == SPP_FLOW_GO)
            {
                
            }
        }
        break;  
        
#if  APP_SUPPORT_SOUNDBAR == 1
        case RDABT_SPP_RECEIVE_SOUNDBAR_VOLUME_IND:
        {
            app_bt_vars->volume = ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x20170012);
            mediaSendBTCommand(MC_SETVOLUME, app_bt_vars->volume);
        }
        break;
#endif

    }
}

#endif
