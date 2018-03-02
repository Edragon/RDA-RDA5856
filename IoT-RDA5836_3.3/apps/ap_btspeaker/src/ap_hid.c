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

#include "ap_hid.h"


#if APP_SUPPORT_HID==1
#include"resource_id.h"
#include "MainTask.h"

RDABT_Hid_Info app_hid_info;
RDABT_Hid_Data_Info app_hid_data;
RDABT_Hid_Sdp_Info app_hid_sdp_info;
u_int8 tp_data[9] = {0};

u_int8 hid_description[] = 
{
    0x05,0x01,0x09,0x06,0xa1,0x01,0x05,0x07,0x85,0x01,0x19,0xe0,0x29,0xe7,
    0x15,0x00,0x25,0x01,0x75,0x01,0x95,0x08,0x81,0x02,0x95,0x01,0x75,0x08,
    0x81,0x03,0x95,0x05,0x75,0x01,0x05,0x08,0x19,0x01,0x29,0x05,0x91,0x02,
    0x95,0x01,0x75,0x03,0x91,0x03,0x95,0x06,0x75,0x08,0x15,0x00,0x26,0xa4,
    0x00,0x05,0x07,0x19,0x00,0x2a,0xa4,0x00,0x81,0x00,0xc0,0x05,0x0c,0x09,
    0x01,0xa1,0x01,0x85,0x02,0x75,0x10,0x95,0x02,0x15,0x01,0x26,0x8c,0x02,
    0x19,0x01,0x2a,0x8c,0x02,0x81,0x00,0xc0,0x05,0x0c,0x09,0x01,0xa1,0x01,
    0x85,0x03,0x15,0x00,0x25,0x01,0x75,0x01,0x95,0x0d,0x0a,0x23,0x02,0x0a,
    0x21,0x02,0x0a,0xb1,0x01,0x09,0xb8,0x09,0xb6,0x09,0xcd,0x09,0xb5,0x09,
    0xe2,0x09,0xea,0x09,0xe9,0x09,0x30,0x0a,0x07,0x03,0x0a,0x08,0x03,0x81,
    0x02,0x95,0x01,0x75,0x0b,0x81,0x03,0xc0,

};
#if 0
{
    //use for PC
    0x05, 0x01, 0x09, 0x06, 0xa1, 0x01, 0x85, 0x01, 
    0x75, 0x01, 0x95, 0x08, 0x05, 0x07, 0x19, 0xe0, 
    0x29, 0xe7, 0x15, 0x00, 0x25, 0x01, 0x81, 0x02, 
    0x95, 0x01, 0x75, 0x08, 0x81, 0x03, 0x95, 0x05, 
    0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 
    0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x03, 
    0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x26, 0xff, 
    0x00, 0x05, 0x07, 0x19, 0x00, 0x29, 0xff, 0x81, 
    0x00, 0xc0, 0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01, 
    0x85, 0x02, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01,
    0x95, 0x15, 0x0a, 0x94, 0x01, 0x0a, 0x92, 0x01,
    0x0a, 0x83, 0x01, 0x0a, 0x23, 0x02, 0x0a, 0x8a,
    0x01, 0x0a, 0xb1, 0x01, 0x0a, 0x21, 0x02, 0x0a,
    0x24, 0x02, 0x0a, 0x25, 0x02, 0x0a, 0x2a, 0x02,
    0x0a, 0x27, 0x02, 0x09, 0xb6, 0x09, 0xb5, 0x09,
    0xb7, 0x09, 0xb0, 0x09, 0xe9, 0x09, 0xea, 0x09,
    0xe2, 0x09, 0xcd, 0x09, 0x30, 0x09, 0xb8, 0x81,
    0x02, 0x95, 0x01, 0x75, 0x03, 0x81, 0x03, 0xc0,
    0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x05,
    0x05, 0x01, 0x09, 0x06, 0xa1, 0x02, 0x06, 0x00,
    0xff, 0x25, 0x01, 0x75, 0x01, 0x95, 0x02, 0x0a,
    0x03, 0xfe, 0x0a, 0x04, 0xfe, 0x81, 0x02, 0x95,
    0x06, 0x81, 0x03, 0xc0, 0xc0,
};
/*
{
    0x05,0x01,0x09,0x06,0xa1,0x01,0x85,0x01,0x05,0x07,
    0x19,0xe0,0x29,0xe7,0x15,0x00,0x25,0x01,0x75,0x01,
    0x95,0x08,0x81,0x02,0x95,0x01,0x75,0x08,0x81,0x03,
    0x95,0x05,0x75,0x01,0x05,0x08,0x19,0x01,0x29,0x05,
    0x91,0x02,0x95,0x01,0x75,0x03,0x91,0x03,0x95,0x06,
    0x75,0x08,0x15,0x00,0x26,0xff,0x00,0x05,0x07,0x19,
    0x00,0x29,0xff,0x81,0x00,0xc0,0x05,0x0c,0x09,0x01,
    0xa1,0x01,0x85,0x03,0x75,0x10,0x95,0x02,0x15,0x01,
    0x26,0x8c,0x02,0x19,0x01,0x2a,0x8c,0x02,0x81,0x60,
    0xc0,0x05,0x01,0x09,0x02,0xa1,0x01,0x85,0x02,0x09,
    0x01,0xa1,0x00,0x05,0x09,0x19,0x01,0x29,0x05,0x15,
    0x00,0x25,0x01,0x95,0x05,0x75,0x01,0x81,0x02,0x95,
    0x01,0x75,0x03,0x81,0x01,0x05,0x01,0x09,0x30,0x09,
    0x31,0x16,0x00,0x80,0x26,0xff,0x7f,0x75,0x10,0x95,
    0x02,0x81,0x06,0x05,0x01,0x09,0x38,0x15,0x81,0x25,
    0x7f,0x75,0x08,0x95,0x01,0x81,0x06,0x05,0x0c,0x0a,
    0x38,0x02,0x15,0x81,0x25,0x7f,0x75,0x08,0x95,0x01,
    0x81,0x06,0xc0,0xc0,0x05,0x0c,0x09,0x01,0xa1,0x01,
    0x85,0xff,0x05,0x06,0x95,0x01,0x75,0x02,0x19,0x24,
    0x29,0x26,0x81,0x02,0x95,0x01,0x75,0x06,0x81,0x01,
    0xc0,0x05,0x01,0x09,0x80,0xa1,0x01,0x85,0x04,0x15,
    0x01,0x25,0x03,0x75,0x02,0x95,0x01,0x09,0x81,0x09,
    0x82,0x09,0x83,0x81,0x60,0x75,0x06,0x81,0x03,0xc0,
 };*/
//#else
{
    //Keyboard
    0x05, 0x01,  		//Usage Page (Generic Desktop)
    0x09, 0x06,         //Usage (Keyboard)
    0xA1, 0x01,         //Collection (Application)

    0x85, 0x01,			//report ID
    0x05, 0x07,         //    Usage Page (Keyboard/Keypad)
    0x19, 0xE0,         //    Usage Minimum (Keyboard Left Control)
    0x29, 0xE7,         //    Usage Maximum (Keyboard Right GUI)
    0x15, 0x00,         //    Logical Minimum (0)
    0x25, 0x01,         //    Logical Maximum (1)
    0x75, 0x01,         //    Report Size (1)
    0x95, 0x08,         //    Report Count (8)
    0x81, 0x02,         //    Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
    0x95, 0x03,         //    Report Count (3)
    0x75, 0x08,         //    Report Size (8)
    0x15, 0x00,         //    Logical Minimum (0)
    0x25, 0x7f,         //    Logical Maximum (101)
    0x05, 0x07,         //    Usage Page (Keyboard/Keypad)
    0x19, 0x00,         //    Usage Minimum (Undefined)
    0x29, 0x7f,         //    Usage Maximum (Keyboard Application)
    0x81, 0x00,         //    Input (Data,Ary,Abs)


    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)

    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
    0x95, 0x01,         //    Report Count (1)
    0x75, 0x01,         //    Report Size (1)
    0x91, 0x01,         //    Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)

    0xC0,              	//End Collection


    /*add for iphone*/
    0x05, 0x0c, 
    0x09, 0x01, 
    0xa1, 0x01,
    0x85, 0x04,
    0x15, 0x00, 
    0x25, 0x01, 
    0x75, 0x01, 
    0x95, 0x18,
    0x09, 0xb5, 
    0x09, 0xb6,
    0x09, 0xb7, 
    0x09, 0xcd,
    0x09, 0xe2,
    0x09, 0xe5,
    0x09, 0xe7,
    0x09, 0xe9,
    0x09, 0xea,
    0x0a, 0x52,
    0x01, 0x0a,
    0x53, 0x01, 
    0x0a, 0x54, 
    0x01, 0x0a, 
    0x55, 0x01, 
    0x0a, 0x83, 
    0x01, 0x0a,
    0x8a, 0x01, 
    0x0a, 0x92,
    0x01, 0x0a, 
    0x94, 0x01,
    0x0a, 0x21,
    0x02, 0x0a,
    0x23, 0x02, 
    0x0a, 0x24, 
    0x02, 0x0a,
    0x25, 0x02, 
    0x0a, 0x26,
    0x02, 0x0a, 
    0x27, 0x02, 
    0x0a, 0x2a, 
    0x02, 0x81, 
    0x02, 0xc0,


    //Mouse
    0x05, 0x01, 		//Usage Page (Generic Desktop)
    0x09, 0x02,       	//Usage (Mouse)
    0xA1, 0x01,       	//Collection (Application)

    0x85, 0x02,			//report ID
    0x09, 0x01,       	//    Usage (Pointer)
    0xA1, 0x00,       	//    Collection (Physical)
    0x05, 0x09,       	//        Usage Page (Button)
    0x19, 0x01,       	//        Usage Minimum (Button 1)
    0x29, 0x03,       	//        Usage Maximum (Button 3)
    0x15, 0x00,       	//        Logical Minimum (0)
    0x25, 0x01,       	//        Logical Maximum (1)
    0x95, 0x03,       	//        Report Count (3)
    0x75, 0x01,       	//        Report Size (1)
    0x81, 0x02,       	//        Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
    0x95, 0x05,       	//        Report Count (1)
    0x75, 0x01,       	//        Report Size (1)
    0x81, 0x01,       	//        Input (Cnst,Ary,Abs)
    0x05, 0x01,       	//        Usage Page (Generic Desktop)
    0x09, 0x30,       	//        Usage (X)
    0x09, 0x31,       	//        Usage (Y)
    0x09, 0x38,       	//        Usage (Wheel)
    0x15, 0x81,       	//        Logical Minimum (-127)
    0x25, 0x7F,       	//        Logical Maximum (127)
    0x75, 0x08,       	//        Report Size (8)
    0x95, 0x03,       	//        Report Count (3)
    0x81, 0x06,       	//        Input (Data,Var,Rel,NWrp,Lin,Pref,NNul,Bit)
    0xC0,             	//    End Collection
    0xC0,              	//End Collection


    //Multimedia
    0x05, 0x0C, 			//Usage Page (Consumer Devices)                  
    0x09, 0x01,             //Usage (Consumer Control)                       
    0xA1, 0x01,             //Collection (Application)

    0x85, 0x03,				//report ID
    0x75, 0x10,         //    Report Size (16)
    0x95, 0x01,         //    Report Count (1)
    0x15, 0x00,    		//    Logical Minimum
    0x26, 0x3C,0x04,    //    Logical Maximum
    0x19, 0x00,			//    Usage Minimum (Undefined)
    0x2a, 0x3C,0x04,	//    Usage Maximum (Keyboard Application)
    0x81, 0x00,         //    Input (Data,Ary,Abs)
    0xC0,                    //End Collection 


    //system control	                       
    0x05, 0x01,             //Usage Page (Generic Desktop)                   
    0x09, 0x80,             //Usage (System Control)                         
    0xA1, 0x01,             //Collection (Application)
    0x85, 0x04,				//report ID

    0x75, 0x01,         	//    Report Size (1)
    0x95, 0x08,         	//    Report Count (8)
    0x15, 0x00,             //    Logical Minimum (0)                        
    0x25, 0x01,             //    Logical Maximum (1)                        
    0x09, 0x81,             //    Usage (System Power Down)                  
    0x09, 0x82,             //    Usage (System Sleep)                       
    0x09, 0x83,             //    Usage (System Wake Up)                     
    0x09, 0x84,             //    Usage (System Context Menu)                
    0x09, 0x85,             //    Usage (System Main Menu)                   
    0x09, 0x86,             //    Usage (System App Menu)                    
    0x09, 0x87,             //    Usage (System Menu Help)                   
    0x09, 0x88,             //    Usage (System Menu Exit)                   
    0x81, 0x02,             //    Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)

    0x75, 0x01,             //    Report Size (1)                            
    0x95, 0x08,             //    Report Count (8)                           
    0x81, 0x03,             //    Input (constant)
    0xC0,                    //End Collection 

};
#endif

t_hid_attributes hid_attribute = 
{
    0x0100, //u_int16 version;
    HID_VIRTUAL_CABLE|HID_RECONNECT_INITIATE|HID_BATTER_POWER|HID_REMOTE_WAKE|HID_NORMALLY_CONNECTABLE|HID_BOOT_DEVICE, //u_int16 feature;
    0x40, //u_int8  subClass;
    0x21, //u_int8  countryCode;
    0x656e, //u_int16 language;
    0x006a, //u_int16 encoding;
    0x0409, //u_int16 langID;
    0x1f40, //u_int16 supervisionTimeout;
    sizeof(hid_description),//u_int16 desc_length;
    (u_int8 *)hid_description,//u_int8 *descriptor;
    "Human Interface Device"//u_int8 *serviceName;
};

void app_hid_register_sdp(void)
{
    COS_EVENT ev = {0};
    
    ev.nEventId = RDABT_HID_REGISTER_SDP_REQ;
    app_hid_sdp_info.type = HID_OPCODE_KEYBOARD;
    app_hid_sdp_info.p_hid_attributes = &hid_attribute;
    ev.nParam1 = (u_int32)&app_hid_sdp_info;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hid_packet_sends(RDABT_Hid_Data_Info *data_info)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_HID_TXDATA_REQ;
    ev.nParam1 = (u_int32)data_info;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#if 1
void app_hid_android_tp_end(void *args)
{
    tp_data[0] = 0x01;
    tp_data[1] = 0x00;
    tp_data[3] = 0x00;
    app_hid_data.data = tp_data;
    app_hid_data.length = 9;
    app_hid_packet_sends(&app_hid_data);    
    COS_SetTimer(10, app_hid_tp_req, NULL, COS_TIMER_MODE_SINGLE);
}

void app_hid_android_tp_req(void  *args )
{
    tp_data[0] = 0x01;
    tp_data[1] = 0x00;
    tp_data[3] = 0x28;
    app_hid_data.data = tp_data;
    app_hid_data.length = 9;
    app_hid_packet_sends(&app_hid_data);
    COS_SetTimer(10, app_hid_android_tp_end, NULL, COS_TIMER_MODE_SINGLE);
}
#endif

void app_hid_tp_end(void *args)
{
    tp_data[0] = 0x03;
    tp_data[2] = 0x00;
    app_hid_data.data = tp_data;
    app_hid_data.length = 4;
    app_hid_packet_sends(&app_hid_data);
}

void app_hid_tp_req(void *args)
{
    tp_data[0] = 0x03;
    tp_data[2] = 0x02;
    app_hid_data.data = tp_data;
    app_hid_data.length = 4;
    app_hid_packet_sends(&app_hid_data);
    COS_SetTimer(10, app_hid_tp_end, NULL, COS_TIMER_MODE_SINGLE);
}

void app_hid_connect_req(t_bdaddr *addr)
{
    COS_EVENT ev = {0};
    hal_HstSendEvent(SYS_EVENT,0x20161223);
    ev.nEventId = RDABT_HID_CONNECT_REQ;
    ev.nParam1 = (u_int32)addr;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hid_disconnect_req(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_HID_DISCONNECT_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
void app_hid_tp_test(t_bdaddr *addr)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_HID_CONNECT_REQ;
    ev.nParam1 = (u_int32)addr;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_hid_handle_key(u_int32 key)
{
    if(key == NULL || key == AP_MSG_RTC)
    {
        return ;
    }

    if(app_hid_info.hid_state != 2 
#if APP_SUPPORT_HSC_HID == 1
	&& key != (AP_KEY_NEXT|AP_KEY_DOUBLE_CLICK)
#else
    && key != (AP_KEY_PREV|AP_KEY_DOUBLE_CLICK)
#endif
	&& key != (AP_KEY_VOLSUB|AP_KEY_DOUBLE_CLICK))
    {
        //hal_HstSendEvent(SYS_EVENT, 0xeeee0009);
        return ;
    }
    
    switch(key)
    {
#if APP_SUPPORT_HID_MODE==1
        case AP_KEY_PLAY|AP_KEY_PRESS:
        {
            app_hid_android_tp_req(NULL);
        }
        break;
#endif 

#if APP_SUPPORT_HSC_HID == 0
        case AP_KEY_NEXT|AP_KEY_DOUBLE_CLICK: //JUN
#else
        case AP_KEY_PREV|AP_KEY_DOUBLE_CLICK: //HSC
#endif
        case AP_KEY_VOLADD|AP_KEY_DOUBLE_CLICK:
        {
            app_hid_android_tp_req(NULL);
        }
        break;
        
#if APP_SUPPORT_HID_MODE==0
#if APP_SUPPORT_HSC_HID == 1
        case AP_KEY_NEXT|AP_KEY_DOUBLE_CLICK://HSC
#else
        case AP_KEY_PREV|AP_KEY_DOUBLE_CLICK://JUN
#endif
        case AP_KEY_VOLSUB|AP_KEY_DOUBLE_CLICK:
        {
            hal_HstSendEvent(SYS_EVENT, 0x20170700);
            hal_HstSendEvent(SYS_EVENT, app_hid_info.hid_state);
            
            if(app_hid_info.hid_state == HID_ADP_STATE_CONNECTED)
            {
                hal_HstSendEvent(SYS_EVENT, 0x20170704);
                app_hid_disconnect_req();
#if APP_SUPPORT_HSC_HID == 0
                app_bt_vars->not_support_hid_service = 1;
#endif
            }
            else if(app_hid_info.hid_state == HID_ADP_STATE_IDLE)
            {
                hal_HstSendEvent(SYS_EVENT, 0x20170705);
#if APP_SUPPORT_HSC_HID == 1
                const UINT8 NULL_ADDR[6] = "";

                if(memcmp(app_hid_info.addr.bytes,NULL_ADDR,sizeof(t_bdaddr)) != 0)
                {
                    app_hid_connect_req(&app_hid_info.addr);
                }
#else
                if(app_bt_vars->not_support_hid_service)
                    app_bt_vars->not_support_hid_service = 0;
#endif
            }
            app_mgr_PlayInternalAudio(GUI_AUDIO_BT_RECOGNITION);
        }
        break;
#endif
    }
}

void app_hid_msg_handle(COS_EVENT *ev)
{
    switch(ev->nEventId)
    {	
        case RDABT_HID_CONNECTED_IND_MSG:
        case RDABT_HID_CONNECTED_CNF_MSG:        
        {
			u_int32 device_index = 0;
            if (!IS_BT_MODE())
            {
                app_hid_disconnect_req();
                break;
            }
            app_hid_info = *(RDABT_Hid_Info *)ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x20161212);      
            hal_HstSendEvent(SYS_EVENT, app_hid_info.hid_state);
#if APP_SUPPORT_A2DP == 0                    
            media_PlayInternalAudio(GUI_AUDIO_BT_CONNECT, 1, 0);
#endif
            hal_HstSendEvent(SYS_EVENT, 0x20170712);
            app_create_a2dp_connection(100, app_hid_info.addr);
            app_cancel_reconnect();
#if APP_SUPPORT_SOUNDBAR == 0
            if(a2dp_connect_role == 0)
            {
                device_index = app_bt_vars->last_device;
				app_bt_connect_second_device((void *)device_index);
            }
#endif
        }
        break;
        
        case RDABT_HID_DISCONNECTED_IND_MSG:
        case RDABT_HID_DISCONNECTED_CNF_MSG:        
        {
            RDABT_Hid_Info app_hid_info_temp = *(RDABT_Hid_Info *)ev->nParam1;
            app_hid_info.hid_state = app_hid_info_temp.hid_state;
            hal_HstSendEvent(SYS_EVENT, 0x20161213);
            hal_HstSendEvent(SYS_EVENT, app_hid_info.hid_state);
        }
        break;
        
        case RDABT_HID_DATA_IND_MSG:
        {
        //t_hid_input_data *data = (t_hid_input_data *)ev->nParam1;
        }
        break;
    }
}
   
#endif


