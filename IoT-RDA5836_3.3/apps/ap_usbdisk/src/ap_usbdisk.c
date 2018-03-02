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

#include "ap_common.h"
#include "gui.h"
#include "ap_usbdisk.h"
#include "mci.h"
#include "ap_message.h"
#include "resource_id.h"
#include "ap_gui.h"
#include "dm.h"

#ifndef MMI_ON_WIN32
#include "uctls_m.h"
#include "mcd_m.h"
#include "uctls_callback.h"
#include "tgt_app_cfg.h"
#include "hal_debug.h"
// =============================================================================
// g_umssStorageCallbackMc
// -----------------------------------------------------------------------------
/// Callback of MC storage
// =============================================================================
EXPORT PUBLIC  UMSS_STORAGE_CALLBACK_T     g_umssStorageMcCallback;
#endif


#define USB_PLUGIN_STORAGE              (AP_MSG_USER_USBDISK+1)
#define USB_PLUGIN_AUDIO                (AP_MSG_USER_USBDISK+2)


extern BOOL ui_timeout_exit;
extern UINT32 g_uAudIsPlaying;

BOOL g_usb_connected = FALSE;

#if APP_SUPPORT_USBAUDIO==1

#define USB_VOL_BASE 0xe4c2
#define USB_VOL_TOP  0xfff0

//USB HID report descriptor constants
#define USB_HID_USAGE_PAGE 0x05
#define USB_HID_USAGE 0x09
#define USB_HID_COLL_START 0xa1//collection start
#define USB_HID_COLL_END 0xc0//collection end
#define USB_HID_LOGICAL_MIN 0x15
#define USB_HID_LOGICAL_MAX 0x25
#define USB_HID_USAGE_MIN 0x19
#define USB_HID_USAGE_MAX 0x29
#define USB_HID_REPORT_COUNT 0x95
#define USB_HID_REPORT_SIZE 0x75
#define USB_HID_INPUT_DES 0x81//input descriptor
#define USB_HID_OUTPUT_DES 0x91//output descriptor

PRIVATE UINT8 ReportDescriptor[] =
{
    USB_HID_USAGE_PAGE,0x0c,//Usage page:Consumer Device
    USB_HID_USAGE,0x01,//Usage:Consumer Contorl
    USB_HID_COLL_START,0x01,//Collection start
    USB_HID_LOGICAL_MIN,0x00,//Logical Minimum:0
    USB_HID_LOGICAL_MAX,0x01,//Logical Maximun:1
    USB_HID_USAGE_PAGE,0x0c,//Usage page:Consumer Device
    USB_HID_USAGE,0xe9,//Usage:Volume Up 0x02
    USB_HID_USAGE,0xea,//Usage:Volume Down 0x04
    USB_HID_USAGE,0xe2,//Usage:Mute  0x01
    USB_HID_USAGE,0xcd,//Usage:Play/Pause 0x08
    //USB_HID_USAGE,0xb1,//Usage:Pause 0x10
    //USB_HID_USAGE,0xb5,//Usage:Next 0x20
    //USB_HID_USAGE,0xb6,//Usage:Previous 0x40
    USB_HID_REPORT_COUNT,0x04,//Report Count:7
    USB_HID_REPORT_SIZE,0x01,//Report Size :1
    USB_HID_INPUT_DES,0x42,//Input:data,var,abs,null
    USB_HID_REPORT_COUNT,0x04,//Report Count:1
    USB_HID_REPORT_SIZE,0x01,//Report Size:1
    USB_HID_INPUT_DES,0x01,//Input:cnst,ary,abs
    USB_HID_COLL_END//End Collection
};

PRIVATE UINT8 ReportDescriptor1[] =
{
    0x05, 0x0C, // Usage Page (Consumer Devices)
    0x09, 0x01, // Usage (Consumer Control)
    0xA1, 0x01, // Collection (Application)
    0x05, 0x0C, // Usage Page (Consumer Devices)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x75, 0x01, // Report Size (1)
    0x95, 0x07, // Report Count (7)
    0x09, 0xB5, // Usage (Scan Next Track)
    0x09, 0xB6, // Usage (Scan Previous Track)
    0x09, 0xB7, // Usage (Stop)
    0x09, 0xCD, // Usage (Play / Pause)
    0x09, 0xE2, // Usage (Mute)
    0x09, 0xE9, // Usage (Volume Up)
    0x09, 0xEA, // Usage (Volume Down)
    0x81, 0x02, // Input (Data, Variable, Absolute)
    0x95, 0x01, // Report Count (1)
    0x81, 0x01, // Input (Constant)
    0xC0 // End Collection
};
//UINT16 Desc_Length = sizeof(ReportDescriptor);

UINT32 keycode = 0;
#endif

BOOL GetUsbCableStatus(void)
{
    return g_usb_connected;
}

BOOL AppUSBStorageMsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
#ifndef MMI_ON_WIN32
	    hal_UsbSetMode(HAL_USB_MODE_MASS);
	    uctls_Open(0x0000, 0x0000, 0x0101, "USB Stroage Devive 1.02");
	    umss_Open();
	    umss_AddPart(&g_umssStorageMcCallback);
	    uctls_ChargerStatus(0);
#endif
        //   media_PlayInternalAudio(GUI_AUDIO_MASS_STORAGE, 1, FALSE);
#if APP_SUPPORT_USBSTORAGE_BG==1
		mmi_timer_create(10, AppGotoMainMenu, NULL, COS_TIMER_MODE_SINGLE);
#endif
        case EV_UI_FW_ON_RESUME:
        case EV_UI_FW_REDRAW:
		    //GUI_ClearScreen(NULL);//清全屏幕
#if APP_SUPPORT_RGBLCD==1
		    GUI_ResShowPic(GUI_IMG_UDISK, 0, 0);//g_displayconfig.usb_x, g_displayconfig.usb_y);
#elif APP_SUPPORT_LCD==1
		    GUI_ResShowPic(GUI_IMG_UDISK, 0, 16);
#endif
		    //GUI_UpdateScreen(NULL);

#if APP_SUPPORT_LED==1
		    LED_SetPattern(GUI_LED_USB_DISK, LED_LOOP_INFINITE);
#endif
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
            break;

        default:
            //key event
            if(MESSAGE_ConvertToKeyValue(pEvent) == (AP_KEY_MODE | AP_KEY_UP))
            {
            	AppGobackScreen();
            	break;
            }
            else
	        {
	            //Common events, DONOT deal it.
	            return FALSE;
	        }
    }

    return TRUE;

}
/******************************************************************************
 * USB_Entry_Storage:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID USB_Entry_Storage(UINT32 param)
{
	app_trace(APP_USB_TRC, "====USB_Entry_Storage====");
    EntryNewScreen(SCR_ID_USB_STORAGE, AppUSBStorageMsgHandler, NULL, 0, 0);
}

#if APP_SUPPORT_USBAUDIO==1

#define USBAUDIO_MAX_VOLUME        AUD_MAX_LEVEL
INT8        g_usbaudio_volumne;

extern VOID uAudGetVolFromCmd(UINT8*, UINT16*);
extern VOID uAudSetVolFromCmd(UINT8, UINT16);

extern volatile UINT8 reset_flag_hid;
extern volatile UINT8 reset_flag_aud;
extern UINT32 g_uAudIsPlaying;

/******************************************************************************
 * AppUSBAudioKeyHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL AppUSBAudioKeyHandler(UINT32 key)
{
    UINT32 counter = 0;

    switch(key)
    {
#ifndef MMI_ON_WIN32
        case AP_KEY_VOLADD | AP_KEY_DOWN:
            hal_HstSendEvent(USB_EVENT,0xabcd2221);
            // uhid_SendKeycode(0x01);
            // keycode = 0x01;
            keycode = 0x20;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_VOLSUB | AP_KEY_DOWN:
            hal_HstSendEvent(USB_EVENT,0xabcd3333);
            // uhid_SendKeycode(0x02);
            keycode = 0x40;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_EQ | AP_KEY_DOWN: // for mute
            hal_HstSendEvent(USB_EVENT,0xabcd5555);
            //  uhid_SendKeycode(0x04);
            keycode = 0x10;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_LOOP | AP_KEY_DOWN: // for stop
            hal_HstSendEvent(USB_EVENT,0xabcd4444);
            //  uhid_SendKeycode(0x04);
            keycode = 0x04;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_PLAY | AP_KEY_DOWN:
            hal_HstSendEvent(USB_EVENT,0xabcd1111);
            //  uhid_SendKeycode(0x08);
            keycode = 0x08;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_PREV | AP_KEY_DOWN:
            hal_HstSendEvent(SYS_EVENT,0xabcd7788);
            keycode = 0x02;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_NEXT | AP_KEY_DOWN:
            hal_HstSendEvent(SYS_EVENT,0xabcd8877);
            keycode = 0x01;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_VOLADD | AP_KEY_UP:
            //  uhid_SendKeycode(0x00);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_VOLSUB | AP_KEY_UP:
            //   uhid_SendKeycode(0x00);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_PLAY | AP_KEY_UP:
            //   uhid_SendKeycode(0x00);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_EQ | AP_KEY_UP: // for mute
            //   uhid_SendKeycode(0x00);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_LOOP | AP_KEY_UP: // for stop
            //   uhid_SendKeycode(0x00);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_PREV | AP_KEY_UP:
            hal_HstSendEvent(SYS_EVENT,0xabcd1122);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case AP_KEY_NEXT | AP_KEY_UP:
            hal_HstSendEvent(SYS_EVENT,0xabcd2211);
            keycode = 0x00;
            uhid_SendKeycode(&keycode, 1);
            break;
        case EV_FS_USB_VOL_CHANGE:
        {
            UINT8 mute = 0;
            UINT16 vol = 0;
//        hal_HstSendEvent(USB_EVENT, 0x12040001);
            uAudGetVolFromCmd(&mute, &vol);
//        hal_HstSendEvent(USB_EVENT, mute);
//        hal_HstSendEvent(USB_EVENT, vol);

            if(mute == 1)
            {
                SetPAMute(1);
            }
            else
            {
                if(vol < USB_VOL_BASE)
                {
                    SetPAMute(1);
                    break;
                }

                g_usbaudio_volumne = (vol - USB_VOL_BASE)/((USB_VOL_TOP - USB_VOL_BASE)/USBAUDIO_MAX_VOLUME) +1;
                if(g_usbaudio_volumne > USBAUDIO_MAX_VOLUME)
                    g_usbaudio_volumne = USBAUDIO_MAX_VOLUME;
                g_comval->Volume = g_usbaudio_volumne;
                SetPAVolume(g_usbaudio_volumne);
            }
        }
        break;
#endif

        default:
#ifndef MMI_ON_WIN32
            if(reset_flag_hid == 1&&reset_flag_aud == 1)
            {
                if(counter == 0)
                {
                    keycode = 0x04;
                    uhid_SendKeycode(&keycode, 1);
                    counter++;
                }
                else if(counter == 1)
                {
                    keycode = 0x00;
                    uhid_SendKeycode(&keycode, 1);
                    counter++;
                }
            }
            else
#endif
			{
				return FALSE;
			}


    }

    return TRUE;
}
/******************************************************************************
 * AppUSBAudioMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL AppUSBAudioMsgHandler(COS_EVENT *pEvent)
{
#ifdef USB_AUDIO_SUPPORT 
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
#ifndef MMI_ON_WIN32
     
            hal_UsbSetMode(HAL_USB_MODE_AUDIO);
            uctls_Open(0x0000, 0x0000, 0x0101, "USB Audio Device 1.04");
            uaud_Open();
            uhid_Open(ReportDescriptor1, sizeof(ReportDescriptor1));
            mediaSendCommand(MC_PLAY_USB, 0);
     
            uctls_ChargerStatus(0);
#endif
        case EV_UI_FW_REDRAW:
		    GUI_ClearScreen(NULL);//清全屏幕
#if APP_SUPPORT_RGBLCD==1
		    GUI_ResShowPic(GUI_IMG_UAUDIO, 0, 0);//g_displayconfig.usb_x, g_displayconfig.usb_y);
#elif APP_SUPPORT_LCD==1
		    GUI_ResShowPic(GUI_IMG_UAUDIO, 0, 16);
#endif
		    GUI_UpdateScreen(NULL);

#if APP_SUPPORT_LED==1
		    LED_SetPattern(GUI_LED_USB_AUDIO, LED_LOOP_INFINITE);
#endif
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            break;


        case EV_UI_FW_ON_EXIT:
#ifndef MMI_ON_WIN32
            reset_flag_hid = 0;
            reset_flag_aud = 0;
            uctls_Close();
            g_uAudIsPlaying = 0;
#endif
            mediaSendCommand(MC_STOP, 0);
            break;

        default:
            //key event
            if(AppUSBAudioKeyHandler(MESSAGE_ConvertToKeyValue(pEvent)))
            {
				break;
            }
            else
	        {
	            //Common events, DONOT deal it.
	            return FALSE;
	        }
    }
#endif
    return TRUE;

}
/******************************************************************************
 * USB_Entry_Audio:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID USB_Entry_Audio(UINT32 param)
{
	app_trace(APP_USB_TRC, "====USB_Entry_Audio====");
    EntryNewScreen(SCR_ID_USB_AUDIO, AppUSBAudioMsgHandler, NULL, 0, 0);
}
#endif

#if APP_SUPPORT_USBDEVICE==1
BOOL AppUSBMainMsgHandler(COS_EVENT *pEvent)
{
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SetProtocolEventHandler(USB_Entry_Storage, USB_PLUGIN_STORAGE);
#if APP_SUPPORT_USBAUDIO==1
            SetProtocolEventHandler(USB_Entry_Audio, USB_PLUGIN_AUDIO);
		    g_usbaudio_volumne = g_comval->Volume;
		    SetPAVolume(g_usbaudio_volumne);
#ifndef MMI_ON_WIN32
		    uAudSetVolFromCmd(0, g_usbaudio_volumne);
#endif
#endif
#if APP_SUPPORT_MENU==1
            SendAppEvent(EV_UI_FW_REDRAW, 0);
#else
#if APP_SUPPORT_USBAUDIO==1        
            MESSAGE_SetEvent(USB_PLUGIN_AUDIO);
#else
            MESSAGE_SetEvent(USB_PLUGIN_STORAGE);
#endif
#endif
            break;

        case EV_UI_FW_REDRAW:
        case EV_UI_FW_ON_RESUME:
#if APP_SUPPORT_MENU==1
            ui_timeout_exit = FALSE;
            GUI_Display_Menu(GUI_MENU_USBPLUGIN, NULL);
            ui_timeout_exit = TRUE;
#endif
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
#if APP_SUPPORT_USBAUDIO==1
            ClearProtocolEventHandler(USB_PLUGIN_AUDIO);
#endif
            ClearProtocolEventHandler(USB_PLUGIN_STORAGE);
        	g_usb_connected = FALSE;
            break;

        default:
            //key event
            if(MESSAGE_ConvertToKeyValue(pEvent) == (AP_KEY_MODE | AP_KEY_UP))
            {
            	//Catch mode key
            	break;
            }
            else
	        {
	            //Common events, DONOT deal it.
	            return FALSE;
	        }
    }

    return TRUE;

}

INT32 USB_Entry(INT32 param)
{
    EntryNewScreen(SCR_ID_USB_MAIN, AppUSBMainMsgHandler, NULL, 0, 0);
    return 0;
}

#endif

