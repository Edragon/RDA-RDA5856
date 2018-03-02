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
#include "ap_calendar.h"
#include "ap_idle.h"
#include "ap_linein.h"
#include "ap_record.h"
#include "ap_setting.h"
#include "ap_fm.h"
#include "ap_music.h"
//#include "ap_bluetooth.h"
#include "ap_usbdisk.h"
#include "tm.h"
#include "dm.h"
#include "MainTask.h"
#include "event.h"
#include "gpio_edrv.h"
#include "mci.h"
#include "ap_mgr.h"
#include "hal_host.h"

#define AP_TEST_FM_FREQ1             87900
#define AP_TEST_FM_FREQ2             99700
#define AP_TEST_FM_FREQ3            103700


//BOOL g_test_mode = 0;
extern INT32  g_current_fd;
UINT64 g_testfile_entry = 0;
BOOL play_end = 0;

extern void boot_set_testmode(BOOL testmode); 

#if APP_SUPPORT_MUSIC==1
static void play_finished(MCI_ERR_T state)
{
    hal_HstSendEvent(SYS_EVENT,0x11111111);    
    play_end = TRUE;
}
#endif

UINT32 APP_Test_Mode_MESSAGE_Wait(void)
{
    COS_EVENT ev;

    COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);

    return (ev.nParam1<<16) |((ev.nEventId)&0xffff);
}

void APP_Test_Mode(void)
{
    UINT32 key;
    
    //g_test_mode = TRUE;
    boot_set_testmode(TRUE); 
    
    // test leds    
    COS_Sleep(1);
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_TEST_PATTERN1, 1);
    #endif 
    COS_Sleep(1);
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_TEST_PATTERN2, 1);
    #endif 
    SetPAVolume(0);
    
    hal_HstSendEvent(BOOT_EVENT, 0x7e570100); // 10%

    // test lcd
#if APP_SUPPORT_RGBLCD==1
    if(AP_Support_LCD())
    {
        GUI_ClearScreen(NULL);
        GUI_InvertRegion(NULL);
        GUI_UpdateScreen(NULL);
        COS_Sleep(1);
        GUI_InvertRegion(NULL);
        GUI_UpdateScreen(NULL);
        COS_Sleep(1);
    }
#elif APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        GUI_ClearScreen(NULL);
        GUI_InvertRegion(NULL);
        GUI_UpdateScreen(NULL);
        COS_Sleep(1);
        GUI_InvertRegion(NULL);
        GUI_UpdateScreen(NULL);
        COS_Sleep(1);
    }
#elif APP_SUPPORT_LED8S==1
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(0xffff,0,0);
    GUI_UpdateScreen(NULL);
    COS_Sleep(200);
    GUI_DisplayText(0,0,"8");
    GUI_UpdateScreen(NULL);
    COS_Sleep(200);
    GUI_DisplayText(1,0,"8");
    GUI_UpdateScreen(NULL);
    COS_Sleep(200);
    GUI_DisplayText(2,0,"8");
    GUI_UpdateScreen(NULL);
    COS_Sleep(200);
    GUI_DisplayText(3,0,"8");
    GUI_UpdateScreen(NULL);
    COS_Sleep(200);
#endif

    hal_HstSendEvent(BOOT_EVENT, 0x7e570300); // 30%
    
    // test fm
#if APP_SUPPORT_FM
    hal_HstSendEvent(SYS_EVENT,0x11220010);
    {
        extern FM_play_status_t    *FMStatus; 
        FMStatus = (FM_play_status_t*)NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));
        AppFMDo(MC_OPEN,0);
        FMStatus->freq = AP_TEST_FM_FREQ1;
        AppFMDo(MC_PLAY,0);
        if(!fmd_ValidStop(FMStatus->freq))
            goto test_fail;
        SetPAVolume(0);
        AppFMDo(MC_CLOSE,0);
    }
#endif

    hal_HstSendEvent(BOOT_EVENT, 0x7e570500); // 50%
#if 0
    // test bluetooth
#if APP_SUPPORT_BLUETOOTH==1
    hal_HstSendEvent(SYS_EVENT,0x11220020);
    {
        extern bt_vars_t *g_pBT_vars;
        extern INT8 g_bt_cur_device;
        g_bt_cur_device = -1;
        g_pBT_vars = (bt_vars_t*)NVRAMGetData(VM_AP_BLUETOOTH,sizeof(bt_vars_t));
        if(BT_Active_Bluetooth()!=0)
            goto test_fail; // test_fail
    }
#endif
#endif

#if APP_SUPPORT_BLUETOOTH==1
    hal_HstSendEvent(SYS_EVENT,0x20170605);
    app_bt_active_req();
    COS_Sleep(2000);
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_STACK_INIT_REQ;
    event.nParam1 = (UINT32)app_bt_vars;
    COS_SendEvent(MOD_BT,  &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
    while(1)
    {
        static u_int8 time = 0;
        COS_Sleep(2000);
        if(app_bt_vars->connect_index == 0xfe)
            break;
        time++;
        if(time > 6)
            goto test_fail;
    }
#endif
    hal_HstSendEvent(BOOT_EVENT, 0x7e570800); // 80%
    // test audio
#if APP_SUPPORT_MUSIC==1
    {
        hal_HstSendEvent(SYS_EVENT,0x11220030);
        if(!MountDisk(FS_DEV_TYPE_TFLASH))
            goto test_fail; // test_fail

        hal_HstSendEvent(SYS_EVENT,0x11220040);
        if(!fselInit(FSEL_TYPE_MUSIC, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, FS_DEV_TYPE_TFLASH))
            goto test_fail; // test_fail
        if(!fselGetNextFile(&g_testfile_entry))
            goto test_fail; // test_fail
        hal_HstSendEvent(SYS_EVENT,0x11220042);
        hal_HstSendEvent(SYS_EVENT,g_testfile_entry);
        g_current_fd = FS_OpenDirect(g_testfile_entry, FS_O_RDONLY, 0);
        hal_HstSendEvent(SYS_EVENT,g_current_fd);
        if(g_current_fd < 0)
            goto test_fail; // test_fail

        hal_HstSendEvent(SYS_EVENT,0x11220050);
        if(MCI_ERR_NO != MCI_AudioPlay(0,g_current_fd,MCI_TYPE_DAF, play_finished, 0))
            goto test_fail;
        SetPAVolume(0);
        play_end = FALSE;
        GUI_DisplayMessage(0, 0, "Test Sucess!", GUI_MSG_FLAG_DISPLAY);
        hal_HstSendEvent(SYS_EVENT,0x11220060);
        hal_HstSendEvent(BOOT_EVENT, 0x7e570a00); // 100%   
   #if APP_SUPPORT_LED==1
        LED_SetPattern(GUI_LED_TEST_SUCESS, LED_LOOP_INFINITE);
   #endif 

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
                hal_HstSendEvent(BOOT_EVENT, 0x7e5752cc); // test success
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
    }
#else
    hal_HstSendEvent(BOOT_EVENT, 0x7e570a00); // 100%
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_TEST_SUCESS, LED_LOOP_INFINITE);
    #endif 

    hal_HstSendEvent(BOOT_EVENT, 0x7e5752cc); // test success
    //g_test_mode = FALSE;
    boot_set_testmode(FALSE); 
    while(1)
    {
        key =  APP_Test_Mode_MESSAGE_Wait();
        if(key==(AP_KEY_MODE|AP_KEY_DOWN))
        {
            RestartSystem();
        }
        if(key==(AP_KEY_POWER|AP_KEY_DOWN))
            DM_DeviceSwithOff();
    };
#endif

test_fail:
    GUI_DisplayMessage(0, 0, "Test Fail!", GUI_MSG_FLAG_DISPLAY);
    #if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_TEST_FAIL, LED_LOOP_INFINITE);
    #endif
    hal_HstSendEvent(BOOT_EVENT, 0x7e57fa11); // test fail
    //g_test_mode = FALSE;
    boot_set_testmode(FALSE); 
    
    while(1)
    {
        key =  APP_Test_Mode_MESSAGE_Wait();
        if(key==(AP_KEY_MODE|AP_KEY_DOWN))
            RestartSystem();
        if(key==(AP_KEY_POWER|AP_KEY_DOWN))
            DM_DeviceSwithOff();
    }
}
