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
#include "ap_bt_test.h"
#include "ap_a2dp.h"
#include "ap_hfp.h"
#include "ap_gui.h"
#include "mci.h"
#ifndef WIN32
#include "hal_sys.h"
#endif

#if APP_SUPPORT_TEST_BOX == 1

static t_bdaddr box_addr = {{0}};
static u_int8 bt_test_handle = 0;
//static RDABT_A2DP_DATA_INFO app_a2dp_data = {{{0}}, 0};
static APP_CHANGE_DEVICE_NAME_INFO name_info = {{{0}}, {0}};
static const INT8 a2dp_data[] = //77 bytes/frame
{
#include "ceshihezi.tab"
};
//u_int8 a2dp_data1[] = {0x9C,	0xBF,	0x20,	0x14,	0x98,	0xB0,	0x01,	0x10,	0x01,	0x00,	0x10,	0x00,	0x01,	0x22,	0xE8,	0xA8,	0x8C,	0x0F,	0x61,	0x50,	0x8C,	0x3D,	0xC8,	0xD0,	0x90,	0x92,	0xF0,	0x28,	0x8C,	0xDC,	0xF2,	0x02,	0xC9,	0xF0,	0x84,	0x04,	0xC2,	0xC2,	0x24,	0x04,	0xC6,	0x6C,	0xFA,	0x02,	0xC5,	0x22,	0xF0,	0xA8,	0x8C,	0x0F,	0x59,	0x50,	0x10,	0x3D,	0xB8,	0xD0,	0x8C,	0x92,	0xE0,	0xA8,	0x8C,	0xDC,	0xEA,	0x02,	0xC9,	0xF0,	0x84,	0x04,	0xC6,	0xC2,	0x2C,	0x04,	0xC6,	0x6D,	0x0C,	0x02,	0xA5,	0x9C,	0xBF,	0x20,	0x0C,	0x98,	0xB0,	0x01,	0x10,	0x01,	0x00,	0x00,	0x00,	0x01,	0x22,	0xF8,	0xC4,	0x44,	0x0F,	0x59,	0x68,	0x48,	0x3D,	0xA8,	0xE8,	0x44,	0x92,	0xD0,	0xC4,	0x44,	0xDC,	0xE2,	0x11,	0x51,	0xF0,	0x8C,	0x02,	0x62,	0xC2,	0x3C,	0x02,	0x72,	0x6D,	0x1A,	0x11,	0x21,	0x23,	0x00,	0xC4,	0x45,	0x0F,	0x58,	0xE8,	0x48,	0x3D,	0x99,	0x68,	0x04,	0x92,	0xB8,	0xC4,	0x44,	0xDC,	0xDA,	0x11,	0x51,	0xF0,	0x8C,	0x02,	0x62,	0xC2,	0x4C,	0x02,	0x62,	0x6D,	0x2A,	0x11,	0x51};
//u_int8 a2dp_data2[] = {0x9C,	0xBF,	0x20,	0x2B,	0xD8,	0xB1,	0x01,	0x10,	0x01,	0x00,	0x00,	0x00,	0x01,	0xE8,	0x49,	0xA8,	0x54,	0xCB,	0x40,	0xA8,	0x14,	0x17,	0x9C,	0x02,	0x42,	0x34,	0x96,	0x02,	0x52,	0xE8,	0x41,	0xA8,	0x54,	0xCB,	0x50,	0xA8,	0x54,	0x17,	0xA4,	0x02,	0x11,	0x34,	0x8E,	0x02,	0x52,	0xE8,	0x31,	0xA8,	0x18,	0xCB,	0x60,	0x68,	0x58,	0x17,	0xB4,	0x02,	0x51,	0x34,	0x7E,	0x02,	0x52,	0xE8,	0x29,	0xA8,	0x58,	0xCB,	0x70,	0xA8,	0x58,	0x17,	0xBC,	0x02,	0x52,	0x34,	0x6E,	0x02,	0x52,	0x9C,	0xBF,	0x20,	0x2B,	0xD8,	0xB1,	0x01,	0x10,	0x01,	0x00,	0x00,	0x00,	0x01,	0xE8,	0x49,	0xA8,	0x54,	0xCB,	0x40,	0xA8,	0x14,	0x17,	0x9C,	0x02,	0x42,	0x34,	0x96,	0x02,	0x52,	0xE8,	0x41,	0xA8,	0x54,	0xCB,	0x50,	0xA8,	0x54,	0x17,	0xA4,	0x02,	0x11,	0x34,	0x8E,	0x02,	0x52,	0xE8,	0x31,	0xA8,	0x18,	0xCB,	0x60,	0x68,	0x58,	0x17,	0xB4,	0x02,	0x51,	0x34,	0x7E,	0x02,	0x52,	0xE8,	0x29,	0xA8,	0x58,	0xCB,	0x70,	0xA8,	0x58,	0x17,	0xBC,	0x02,	0x52,	0x34,	0x6E,	0x02,	0x52};
//u_int8 *a2dp_data = NULL;
static u_int32 a2dp_send_cnt = 0;
//static u_int8 a2dp_send_timeout = 0;
static u_int8 a2dp_stop_send = 0;
static u_int8 inquiry_cancel = 0;
static u_int8 device_list_index = 0;
static u_int8 device_addr[18] = {0};
static APP_MGR_DEVICE_INFO_BASE device_list[10];
A2DP_SOURCE_MSG test_box_data = {4, (u_int8*)a2dp_data, 308};

static u_int32 g_test_box_entry = 0;
extern INT32  g_current_fd;
static uint8 disk_type = FS_DEV_TYPE_INVALID;
//static uint32 g_testfile_entry = 0;
BOOL play_tcard_status = FALSE;
BOOL tcard_plug_enable = FALSE;

#ifndef WIN32
extern PUBLIC VOID hal_SysRequestFreq(HAL_SYS_FREQ_USER_ID_T userId, HAL_SYS_FREQ_T minFreq,
                                HAL_SYS_FREQ_CHANGE_HANDLER_T handler);
#endif
extern void MediaSendAppEvent(UINT32 event, UINT32 param);

void ap_test_box_inquiry_start(void)
{
    app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    inquiry_cancel = 0;
}

void ap_test_box_inquiry_cancel(void)
{
    app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
    inquiry_cancel = 1;
}

void ap_send_call_noactive(void *pArgs)
{
    char *call_setup = "\r\n+CIEV:2,0\r\n";

    hal_HstSendEvent(APP_EVENT,0x33333333);    
    app_hfp_send_raw_data(&box_addr, call_setup, strlen(call_setup));
    //app_sco_connect_req(&a2dp_addr);
}

void ap_send_call_active(void *pArgs)
{
    char *call_setup = "\r\n+CIEV:1,1\r\n";

    hal_HstSendEvent(APP_EVENT,0x33333322);  
    app_hfp_send_raw_data(&box_addr, call_setup, strlen(call_setup));
    //COS_SetTimer(300, ap_send_call_noactive, NULL, COS_TIMER_MODE_SINGLE);
}

void ap_send_ring(void *pArgs)
{
    char *call_setup = "\r\nRING\r\n";
    
    hal_HstSendEvent(APP_EVENT,0x33333311);    
    app_hfp_send_raw_data(&box_addr, call_setup, strlen(call_setup));
    COS_SetTimer(100, ap_send_call_active, NULL, COS_TIMER_MODE_SINGLE);
}

void ap_start_sco(void *pArgs)
{
    char *call_setup = "\r\n+CIEV:2,1\r\n";
    
    app_hfp_send_raw_data(&box_addr, call_setup, strlen(call_setup));    
    COS_SetTimer(100, ap_send_call_active, NULL, COS_TIMER_MODE_SINGLE);
}

void app_hfp_set_link_timeout(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_HFP_SET_LINK_TIMEOUT_REQ;
    event.nParam1 = (UINT32)pAddr;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hfp_change_device_name(char *name)
{
    COS_EVENT event = {0};

    hal_HstSendEvent(SYS_EVENT, 0x05161511);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[5]);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[4]);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[3]);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[2]);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[1]);
    hal_HstSendEvent(SYS_EVENT, box_addr.bytes[0]);

    app_a2dp_send_suspend(&box_addr);
    
    if(name != NULL)
    {
        name_info.addr = box_addr;
        memcpy(name_info.device_name, name, MAX_BT_DEVICE_NAME);
    }
    else
    {
        return ;
    }
    
    event.nEventId = RDABT_HFP_CHANGE_DEVICE_NAME_REQ;
    event.nParam1 = (UINT32)&name_info;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void ap_start_a2dp(void *pArgs)
{
    ap_send_call_noactive(NULL);
    app_a2dp_send_start(&box_addr);    
}

void app_bt_timer_handle(void *pArgs)
{
    hal_HstSendEvent(SYS_EVENT, 0x09220943);
    hal_HstSendEvent(SYS_EVENT, app_bt_vars->last_device);
    app_bt_inquiry_cancel_req();
    if(app_bt_vars->last_device == 0xff)
    {
        app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
    }
    else
    {
        app_bt_connect(4);
    }
}

#if APP_SUPPORT_LCD == 1
void app_print_bt_addr(t_bdaddr address, char *outBuffer) 
{
    int i;

    for (i=6; i>0; i--) 
    {
        /* upper nybble */
        outBuffer[12-i*2]=(((address.bytes[i])&0xF0)>>4)+48;
        outBuffer[12-i*2]+=outBuffer[12-i*2]>57?7:0;
        /* lower nybble */
        outBuffer[12+1-i*2]=((address.bytes[i])&0x0F)+48;
        outBuffer[12+1-i*2]+=outBuffer[12+1-i*2]>57?7:0;
        /* delimiter */
        //outBuffer[15+2-i*3]=':';
    }
    outBuffer[17]='\0';
}


INT32 inquiry_device_list_callback(INT32 type, INT32 value, INT8 **string)
{	
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        app_print_bt_addr(device_list[value].addr, device_addr);
        *string = (u_int8*)&device_addr;
        
        switch((device_list[value].cod>>8)&0x1f) // bit12 ~ bit8, device class
        {
            case 2: // phone
            return 1;
            
            case 1: // pc
            return 2;
            
            case 4: // headset
            return 3;
            
            default:
            return 0; // default display bt icon
        }

    }
    return 0;
}
#endif


u_int8 app_bt_test_func(APP_MGR_DEVICE_INFO_BASE *dbEntry)
{
    t_bdaddr dest_addr;

    if(dbEntry == NULL)
    {
        return 0;
    }
/*
    dest_addr.bytes[0] = 0x3c;
    dest_addr.bytes[1] = 0xf2;
    dest_addr.bytes[2] = 0x34;
    dest_addr.bytes[3] = 0x61;
    dest_addr.bytes[4] = 0xaf;
    dest_addr.bytes[5] = 0x18;
*/
    dest_addr.bytes[0] = 0x07;
    dest_addr.bytes[1] = 0x07;
    dest_addr.bytes[2] = 0x05;
    dest_addr.bytes[3] = 0x56;
    dest_addr.bytes[4] = 0x58;
    dest_addr.bytes[5] = 0x00;
    
    hal_HstSendEvent(SYS_EVENT, 0x09211656);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[0]);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[1]);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[2]);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[3]);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[4]);
    hal_HstSendEvent(SYS_EVENT, dbEntry->addr.bytes[5]);
    
    if(!memcmp(dbEntry->addr.bytes, dest_addr.bytes, sizeof(t_bdaddr)))
    {
        hal_HstSendEvent(SYS_EVENT, 0x09211635);
        app_bt_inquiry_cancel_req();
        COS_Sleep(20);
#if APP_SUPPORT_A2DP==1
        app_set_bt_visible_req(MGR_SET_NO_VISIABLE);
        box_addr = dest_addr;
        app_connect_a2dp(&box_addr);
#endif

        if(bt_test_handle)
        {
            COS_KillTimer(bt_test_handle);
            bt_test_handle = 0;
        }
        return 1;
    }
    return 0;
}

void app_test_box_msg_handle(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {
#if APP_SUPPORT_HFP == 1
        case RDABT_HFP_DATA_IND_MSG:
        {
            RDABT_HFP_MSG_INFO *tmp = (RDABT_HFP_MSG_INFO *)ev->nParam1;    
            app_msg_info.hfp_addr = tmp->hfp_addr;
			
            memcpy(app_msg_info.msg_buf, tmp->msg_buf, HFP_MAX_MSG_BUF);
            if(strncmp(app_msg_info.msg_buf+2, "+CLIP=", 6)==0)
            {
                app_hfp_set_link_timeout(&app_msg_info.hfp_addr);
            }
        }
        break;
#endif

#if APP_SUPPORT_A2DP == 1    
        case RDABT_A2DP_MEDIA_CONNECTED_CNF_MSG:
        {
            RDABT_A2DP_MSG_INFO *msg_info = (RDABT_A2DP_MSG_INFO *)ev->nParam1;    
            APP_A2DP_DEVICE_INFO *device_info = app_find_a2dp_device(msg_info->a2dp_addr);
            
            hal_HstSendEvent(SYS_EVENT, 0x10201539);
            if(device_info != NULL)
            {
                if (!IS_BT_MODE())
                {
                    app_disconnect_a2dp(&device_info->a2dp_addr);
                    break;
                }
                
#if APP_SUPPORT_TEST_BOX==1			
                app_test_box_open_a2dp_music_source();
                tcard_plug_enable = TRUE;

                app_a2dp_send_start(&device_info->a2dp_addr);
#if APP_SUPPORT_HFP==1
                app_create_hfp_connection(5, device_info->a2dp_addr);
#endif            
#endif            
            }
        }
        break;
        
        case RDABT_A2DP_START_RSP_MSG:
        {
            hal_HstSendEvent(SYS_EVENT, 0x10261409);    
            a2dp_send_cnt = 0;
            //a2dp_data = a2dp_data;
            app_a2dp_send_data(box_addr, 308, (u_int8*)a2dp_data, 0, 1, 0x60, 0, 4);
        }
        break;   
        
        case RDABT_A2DP_SEND_DATA_CNF_MSG:
             app_test_box_sned_internal_music(ev->nParam1);
        break;
        
        case RDABT_A2DP_SIGNAL_DISCONNECTED_IND_MSG:
        case RDABT_A2DP_SIGNAL_DISCONNECTED_CNF_MSG:
        {
            a2dp_stop_send = 0;
            a2dp_send_cnt = 0;
        }
        break;
#endif  

        case RDABT_DISCOVERY_RESULT_CNF:
        {
            memset(device_list, 0, sizeof(device_list));
            device_list_index = 0;
            GUI_ClearScreen(NULL);
            DeleteScreenIfPresent(SCR_ID_GUI_LIST);
            GUI_UpdateScreen(NULL);
            hal_HstSendEvent(SYS_EVENT, inquiry_cancel);
            if(!inquiry_cancel)
            {
                a2dp_stop_send = 0;
                app_bt_inquiry_req(10, 0);
            }
        }
        break;  
        
        case RDABT_DISCOVERY_RESULT_IND:
        {
            APP_MGR_DEVICE_INFO_BASE *dbEntry = (APP_MGR_DEVICE_INFO_BASE *)ev->nParam1;
            if(dbEntry != NULL)
            {
                if(device_list_index == 5)
                {
                    memset(device_list, 0, sizeof(device_list));
                    device_list_index = 0;
                }
                device_list[device_list_index].addr = dbEntry->addr;
                device_list[device_list_index].rssi = dbEntry->rssi;
                device_list[device_list_index].cod = dbEntry->cod;
                device_list_index++;

                DeleteScreenIfPresent(SCR_ID_GUI_LIST);
                //SendAppEvent(EV_UI_FW_REDRAW, 0);
                GUI_Display_List(LIST_TYPE_CUSTOM_IMAGE, 
                GUI_IMG_BTDEVICELIST, device_list_index, device_list_index-1, inquiry_device_list_callback, TRUE);
                
                if(0 != app_bt_test_func(dbEntry))
                {
                    memset(device_list, 0, sizeof(device_list));
                    device_list_index = 0;
                    DeleteScreenIfPresent(SCR_ID_GUI_LIST);
                }
            }
        }
        break;	
        
        case RDABT_ACL_CONNECT_CNF_MSG:
        {
            APP_MGR_MSG_INFO *msg_info = (APP_MGR_MSG_INFO*)ev->nParam1;    
            if(msg_info->msg_param != RDABT_NOERROR)
            {
                a2dp_stop_send = 0;
                app_bt_inquiry_req(10, 0);
            }
        }
        break;
        
        case RDABT_ACL_DISCONNECT_IND_MSG:
        {
            a2dp_stop_send = 0;
            app_bt_inquiry_req(10, 0);
        }
        break; 
        
        case RDABT_MGR_DEVICE_UPDATE_MSG:
        {
            char *name,*bt_addr;
            UINT8 cur_name[32] = {0};  
            UINT8 cur_addr[32] = {0};
            APP_MGR_DEVICE_INFO_BASE *device = (APP_MGR_DEVICE_INFO_BASE *)ev->nParam1;
            name = device->device_name;
            bt_addr = device->addr.bytes;
            strcpy(cur_name, name);
            app_print_bt_addr(*(t_bdaddr*)bt_addr, cur_addr);
            GUI_ClearScreen(NULL);
            GUI_DisplayText(0, 0, cur_name);
            GUI_DisplayText(0, 16, cur_addr);
            GUI_UpdateScreen(NULL);
        }
        break; 
        
        case RDABT_STACK_INIT_MSG:
        {
#ifndef WIN32                    
            hal_SysRequestFreq(20, 104000000, NULL);
#endif
            if(ev->nParam1 == HOST_BR_MODE)
            {
                app_set_bt_visible_req(MGR_SET_ALL_VISIABLE);
                app_bt_inquiry_req(10, 0);
            }
        }
        break;   
        
        case AP_MSG_SD_OUT:
        {
            hal_HstSendEvent(SYS_EVENT,0x2017e600);
            hal_HstSendEvent(SYS_EVENT,ev->nParam1);
            hal_HstSendEvent(SYS_EVENT,tcard_plug_enable);

            if(ev->nParam1 == 1)  //plugin
            {    
                if(tcard_plug_enable == TRUE)
                {
                    app_test_box_open_a2dp_music_source();
                }
            }
            else                  //plugout
            {
                app_test_box_close_a2dp_music_source();
                app_test_box_sned_internal_music(0);
            }
        }
        break;    
        
        case AP_MSG_USB_PLUGIN:
        {
            hal_HstSendEvent(SYS_EVENT,0x2017e601);
            hal_HstSendEvent(SYS_EVENT,ev->nParam1);
            if(tcard_plug_enable == TRUE)
            {
                app_test_box_open_a2dp_music_source();
            }
        }
        break;
        
        case AP_MSG_USB_PULLOUT:
        {
            hal_HstSendEvent(SYS_EVENT,0x2017e602);
            hal_HstSendEvent(SYS_EVENT,ev->nParam1);

            app_test_box_close_a2dp_music_source();
            app_test_box_sned_internal_music(0);
        }
        break;

        case EV_AUDIO_PLAY_FINISH_IND:
        {
            hal_HstSendEvent(SYS_EVENT,0x2017e700);
            hal_HstSendEvent(SYS_EVENT,ev->nEventId);
            app_test_box_play_next_music();
            if(play_tcard_status == FALSE)
            {
                app_test_box_sned_internal_music(0);
            }
        }
        break;

    }
}

void app_test_box_handle_key(u_int32 key)
{
    switch(key)
    {
        case AP_KEY_PLAY|AP_KEY_PRESS:
        {
#if APP_SUPPORT_HFP_AG == 1
            APP_HFP_ADP_INFO *hfp_adp_info = app_get_hfp_adp_info();
            APP_A2DP_ADP_INFO *a2dp_adp_info = app_get_a2dp_adp_info();
            
            if(hfp_adp_info == NULL || a2dp_adp_info == NULL)
            {
                break;
            }
            
            if(hfp_adp_info->sco_handle == 0)
            {
                a2dp_stop_send = 1;
                app_sco_connect_req(&hfp_adp_info->last_addr);
                app_a2dp_send_suspend(&a2dp_adp_info->last_addr);
                hal_HstSendEvent(APP_EVENT,0x33333300);
                COS_SetTimer(200, ap_start_sco, NULL, COS_TIMER_MODE_SINGLE);
            }
            else
            {
                app_sco_disconnect_req(hfp_adp_info->sco_handle);
                COS_SetTimer(200, ap_start_a2dp, NULL, COS_TIMER_MODE_SINGLE);
            }
#endif 
        }
        break;
        
        case AP_KEY_MODE|AP_KEY_PRESS:
        {
            app_bt_inquiry_cancel_req();
#if APP_SUPPORT_A2DP_SOURCE==1
            a2dp_stop_send = 1;
#endif            
        }
        break;
    }
}

void app_test_box_sned_internal_music(UINT32 retry)
{
    //hal_HstSendEvent(SYS_EVENT, 0x10281020);
    //hal_HstSendEvent(SYS_EVENT, ev->nParam1);
    if(a2dp_stop_send == 1 || play_tcard_status == TRUE)
    {
        return;
    }
    
    if(retry == 0)
    {
        uint32 sbc_send_data_offset = a2dp_send_cnt * test_box_data.sbc_data_len;
        if(sbc_send_data_offset > (sizeof(a2dp_data) - test_box_data.sbc_data_len))
        {
            a2dp_send_cnt = 0;
            sbc_send_data_offset = 0;
        }
        
        test_box_data.sbc_data = (u_int8*)a2dp_data + sbc_send_data_offset;
        COS_Sleep(10);
        app_a2dp_send_data_handle(&test_box_data);
        a2dp_send_cnt++;
    }
    else
    {
        COS_SetTimer(10, app_a2dp_send_data_handle, &test_box_data, COS_TIMER_MODE_SINGLE);
    }
}

void app_test_box_play_finished( void )
{
    hal_HstSendEvent(SYS_EVENT,0x20171ddd); 
    play_tcard_status = FALSE;
    if(g_current_fd >= 0)
    {
         FS_Close(g_current_fd);
         g_current_fd = -1;
    }
    
    hal_HstSendEvent(SYS_EVENT,0x20172ddd); 
    MediaSendAppEvent(EV_AUDIO_PLAY_FINISH_IND, play_tcard_status);
}

void app_test_box_play_next_music( void )
{
    if(fselGetNextFile((UINT64*)&g_test_box_entry))
    {
        hal_HstSendEvent(SYS_EVENT,0x20173ddd);

        g_current_fd = FS_OpenDirect(g_test_box_entry, FS_O_RDONLY, 0);
        if(g_current_fd >= 0)
        {           
            hal_HstSendEvent(SYS_EVENT,0x20174ddd); 
            MCI_AudioStop();
            if(MCI_ERR_NO == MCI_AudioPlay(0,g_current_fd,MCI_TYPE_WAV, (MCI_AUDIO_PLAY_CALLBACK_T)app_test_box_play_finished, 0))
            {
                play_tcard_status = TRUE;
            }
        }
        else
        {
            hal_HstSendEvent(SYS_EVENT,0x20175ddd);
            hal_HstSendEvent(SYS_EVENT,g_current_fd);
        }
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT,0x20176ddd);
    }
    hal_HstSendEvent(SYS_EVENT,0x20177ddd);
    hal_HstSendEvent(SYS_EVENT,play_tcard_status);
}

void app_test_box_open_a2dp_music_source(void)
{
    play_tcard_status = FALSE;

    hal_HstSendEvent(SYS_EVENT,0x2017e500);
    if(MountDisk(FS_DEV_TYPE_TFLASH))
    {
        hal_HstSendEvent(SYS_EVENT,0x2017e501);
        disk_type = FS_DEV_TYPE_TFLASH;
    }
    else if(MountDisk(FS_DEV_TYPE_USBDISK))
    {
        hal_HstSendEvent(SYS_EVENT,0x2017e502);
        disk_type = FS_DEV_TYPE_USBDISK;
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT,0x2017e503);
        goto test_fail; // test_fail
    }
    
    hal_HstSendEvent(SYS_EVENT,0x2017e504);
    if(!fselInit(FSEL_TYPE_MUSIC, FSEL_ALL_REPEATALL, FSEL_TYPE_COMMONDIR, disk_type))
        goto test_fail; // test_fail
        
    if(!fselGetNextFile((UINT64*)&g_test_box_entry))
        goto test_fail; // test_fail
        
    hal_HstSendEvent(SYS_EVENT,0x2017e505);
    hal_HstSendEvent(SYS_EVENT,g_test_box_entry);
    g_current_fd = FS_OpenDirect(g_test_box_entry, FS_O_RDONLY, 0);
    hal_HstSendEvent(SYS_EVENT,g_current_fd);
    
    if(g_current_fd < 0)
        goto test_fail; // test_fail

    hal_HstSendEvent(SYS_EVENT,0x2017e506);
    MCI_AudioStop();
    
    if(MCI_ERR_NO != MCI_AudioPlay(0,g_current_fd,MCI_TYPE_WAV, (MCI_AUDIO_PLAY_CALLBACK_T)app_test_box_play_finished, 0))
        goto test_fail;
        
    SetPAVolume(10);
    play_tcard_status = TRUE;
#if 0
    while(1)
    {
        key =  APP_Test_Mode_MESSAGE_Wait();
        if(play_end)
        {
            play_end = FALSE;
            MCI_AudioStop();
            MCI_AudioPlay(0,g_current_fd,MCI_TYPE_DAF, play_finished, 0);
        }
        if(key==(AP_KEY_PLAY|AP_KEY_DOWN))
        {
            SetPAVolume(7);
        }
        else if(key==(AP_KEY_PLAY|AP_KEY_UP))
        {
            SetPAVolume(0);
            hal_HstSendEvent(APP_EVENT, 0x7e5752cc); // test success
            //g_test_mode = FALSE;
            boot_set_testmode(FALSE); 
        }
        else if(key==(AP_KEY_MODE|AP_KEY_DOWN))
        {
            MCI_AudioStop();
            FS_Close(g_current_fd);
            RestartSystem();
        }
    };
#endif
test_fail:
    
    hal_HstSendEvent(SYS_EVENT,0x2017e507);
    hal_HstSendEvent(APP_EVENT, play_tcard_status);
}
void app_test_box_close_a2dp_music_source(void)
{
    
    hal_HstSendEvent(SYS_EVENT,0x2017fff1); 
    play_tcard_status = FALSE;
    if(g_current_fd >= 0)
    {
         FS_Close(g_current_fd);
         g_current_fd = -1;
    }
    MCI_AudioStop();
}
#endif

