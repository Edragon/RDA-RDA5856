
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
//#include "ap_bluetooth.h"
#include "resource_id.h"
#include "bal_resource.h"
#include "device.h"

#include "pm.h"
#ifndef WIN32
#include "dm_audio.h"
#include "pmd_m.h"
#include "sxs_io.h"
#include "hal_overlay.h"
#include "hal_rda_audio.h"
#include "hal_voc.h"
#include "dsm_dev_driver.h"
#include "csw_ver.h"
#else
#define PMD_LEVEL_LCD 1
#endif

#include "cos.h"
#include "hal_host.h"


BOOL  g_cardexistornot=FALSE;
INT8 g_backLight = 0;
UINT8 g_cur_volume = 0;

extern void  hal_SpiCodecVolume(UINT8 vol);
extern void *DSM_GetUserData(UINT8 index, UINT32 length);
extern INT32 DSM_WriteUserData(void);
extern INT32 DSM_UserDataClear(VOID);
extern VOID DSM_Upgrade_Flash(INT file, UINT8 *BTADDR);
extern VOID bal_SetSleepState(BOOL sleep);
extern VOID gpio_SetLED(int index, int on);
#ifndef WIN32
extern PUBLIC AUD_ERR_T aud_CodecCommonStreamStop(SND_ITF_T itf);
#endif
extern UINT8 g_light_time;
extern VOID AppMsgResetRtcCounter(VOID);


/*********************************************************************************
* Description : 打开a/d转换
*
* Arguments   :
*
* Notes       :
*
*********************************************************************************/

//设置音量
BOOL SetPAVolume(INT32 volume)
{
    DM_SetAudioVolume(volume);
    COS_Sleep(1 MS_WAITING); //Make sure bal task had set volume

    g_cur_volume = volume; //bak vol
    return TRUE;
}

void SetPAMute(BOOL mute)
{
    if(mute)
	    DM_SetAudioVolume(0);
    else
	    DM_SetAudioVolume(g_cur_volume);
}

static UINT8 g_sleep_state = 0;

void APP_Sleep(void)
{

#ifdef APP_RECORD_VOICE_INRAM
    return; 
#endif

	extern int   audioItf;
    app_trace(APP_MAIN_TRC, "Enter sleep mode");
#ifndef WIN32
    if(g_sleep_state)
        return; 
    g_sleep_state = 1;

    if(0)//(!IS_FM_MODE())
    {
#ifndef WIN32
        aud_CodecCommonStreamStop(audioItf);
#endif
        //gpio_SetLED(0);
        hal_AudDeepPowerDown();
        // Power off audio module
        hal_AudForcePowerDown();
    }

    // Disable GPADC
    //pmd_GpadcDisable();
    // Restore charger h/w setting
    //pmd_RestoreChargerAtPowerOff();
    //for(int i=1; i<=10;i++)COS_KillTimer(i);
#if APP_SUPPORT_LCD==1
    CloseBacklight();
    lcdd_Sleep();
#endif
#if 0//def MCD_TFCARD_SUPPORT
    mcd_Close();
#endif
	//hal_UartClose(1);
  //  pmd_SetPowerMode(0);//PMD_LOWPOWER);
    //hal_LpsDeepSleep(0);
    //hal_TimWatchDogClose();
    bal_SetSleepState(TRUE);
    //hal_SysRequestFreq(0, 32768, NULL);//sys clock HAL_SYS_FREQ_32K
#endif
}

BOOL APP_Wakeup(void)
{
    BOOL result = FALSE;
#ifndef WIN32
    if(g_sleep_state)
    {
        app_trace(APP_MAIN_TRC, "Wakeup from sleep mode");
        //pmd_SetPowerMode(1);
        bal_SetSleepState(FALSE);
        //hal_SysRequestFreq(0, 78000000, NULL);//HAL_SYS_FREQ_78M
        //hal_TimWatchDogOpen(20 SECOND);
#if APP_SUPPORT_LCD==1
        lcdd_WakeUp();
#endif
        result = TRUE;
    }
    g_sleep_state = 0;
#endif

    if(g_light_time != 0) OpenBacklight();
    AppMsgResetRtcCounter();

    return result;
}

BOOL Check_CardOrUsb(void)//检查卡是否存在，或者是否有U盘
{
	return g_cardexistornot;
}

UINT32 GetBattery(void)
{
    UINT8 percent=0,charging=0;
    UINT16 mpc=0;
    
    PM_GetBatteryInfo(&charging, &percent, &mpc);
 //   app_trace(APP_MAIN_TRC, "GetBattery charge:%d, percent=%d", charging, percent);
/*    if(charging && percent<100)
	    return -1;
    else
*/
        return percent;
}

BOOL IsChargePlugIn(VOID)
{
    if(pmd_GetChargerStatus() >= PMD_CHARGER_PLUGGED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL SetBackLight(INT8 BLight)//设置背光亮度。
{
#if APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        g_backLight = BLight;
        pmd_SetLevel(2,BLight);
    }
#endif
    return TRUE;
}
BOOL OpenBacklight(void)//打开背光
{
#if APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        pmd_SetLevel(PMD_LEVEL_LCD,g_backLight);
    }
#endif
    return TRUE;
}
BOOL CloseBacklight(void)	//关闭背光
{
#if APP_SUPPORT_LCD==1
    if(AP_Support_LCD())
    {
        pmd_SetLevel(PMD_LEVEL_LCD,0);
    }
#endif
    return TRUE;
}

#if APP_SUPPORT_LED==1
UINT8 *g_led_cur_pattern = NULL;
UINT8 g_led_cur_count = 0;
UINT8 g_led_cur_pos = 0;
UINT8 g_led_cur_length = 0;
UINT8 g_led_cur_pattern_id = 0;
UINT8 g_led_cur_timer_id = 0;

// background led pattern, for bluetooth
UINT8 *g_led_bg_pattern = NULL;
UINT8 g_led_bg_count = 0;
UINT8 g_led_bg_pattern_id = GUI_LED_BT_CLOSED;
UINT8 g_led_bg_pos = 0;
UINT8 g_led_bg_length = 0;
UINT8 g_led_bg_timer_id = 0;

void LedSetCurPattern(INT8 pattern_id)
{
    UINT32 length;//, i;
    UINT8 *pattern;
    pattern = gui_get_led_pattern(pattern_id, &length);

    g_led_cur_pattern = pattern;
    g_led_cur_pos = 0;
    g_led_cur_length = strlen(pattern);
    g_led_cur_pattern_id = pattern_id;
}

void LedTimeout(void *param)
{
    int delay;
    char c;

    do{
        delay = 0;
        if(g_led_cur_pattern[g_led_cur_pos]==0 || g_led_cur_pos>=g_led_cur_length) // one loop end
        {
            if(g_led_cur_count <= 1)
                break;
            if(LED_LOOP_INFINITE != g_led_cur_count)
            {
                g_led_cur_count --;
            }
            g_led_cur_pos = 0;
        }
        c = g_led_cur_pattern[g_led_cur_pos];
        if(c>='0' && c<='9') // delay
        {
            delay = (c-'0') * 10 + g_led_cur_pattern[g_led_cur_pos+1] - '0';
            g_led_cur_pos += 2;
            break;
        }
        else if(c>='A' && c <= 'H')
        {
            // open led
            gpio_SetLED(c-'A', 1);
        }
        else if(c>='a' && c <= 'h')
        {
            // close led
            gpio_SetLED(c-'a', 0);
        }
        g_led_cur_pos ++;
    } while(1);

    if(delay != 0)
    {
        g_led_cur_timer_id = COS_SetTimer(delay*100, LedTimeout, param, COS_TIMER_MODE_SINGLE);
    }
    else
        g_led_cur_timer_id = 0;
}


void LED_SetPattern(UINT8 pattern_id, UINT8 count)
{
    UINT32 length, i;
    UINT8 *pattern;
    if((pattern_id == g_led_cur_pattern_id) && (g_led_cur_timer_id != 0))
        return;
    pattern = gui_get_led_pattern(pattern_id, &length);
    if(pattern == NULL || length == 0 || strlen(pattern) == 0)
        return;

    // check pattern for loop
    if(count>1)
    {
        for(i=0;i<length;i++)
        {
            if(pattern[i]>'0' && pattern[i]<='9') // have a number greater than 0
            {
                break;
            }
        }
        if(i==length) // if no delay, force count to 1
            count=1;
    }
    LedSetCurPattern(pattern_id);
    g_led_cur_count = count;

    if(g_led_cur_timer_id != 0)
    {
        COS_KillTimer(g_led_cur_timer_id);
        g_led_cur_timer_id = 0;
    }
    
    LedTimeout(NULL);
}

void LedBgTimeout(void *param)
{
    int delay;
    char c;

    do{
        delay = 0;
        if(g_led_bg_pattern[g_led_bg_pos]==0 || g_led_bg_pos>=g_led_bg_length) // one loop end
        {
            if(g_led_bg_count <= 1)
                break;
            if(LED_LOOP_INFINITE != g_led_bg_count)
            {
                g_led_bg_count --;
            }
            g_led_bg_pos = 0;
        }
        c = g_led_bg_pattern[g_led_bg_pos];
        if(c>='0' && c<='9') // delay
        {
            delay = (c-'0') * 10 + g_led_bg_pattern[g_led_bg_pos+1] - '0';
            g_led_bg_pos += 2;
            break;
        }
        else if(c>='A' && c <= 'H')
        {
            if(0 == g_led_cur_timer_id)
            {
                // open led
                gpio_SetLED(c-'A', 1);
            }
        }
        else if(c>='a' && c <= 'h')
        {
            if(0 == g_led_cur_timer_id)
            {
                // close led
                gpio_SetLED(c-'a', 0);
            }
        }
        g_led_bg_pos ++;
    } while(1);

    if(delay != 0)
    {
        g_led_bg_timer_id = COS_SetTimer(delay*100, LedBgTimeout, param, COS_TIMER_MODE_SINGLE);
    }
    else
        g_led_bg_timer_id = 0;
}

void LED_SetBgPattern(UINT8 pattern_id)
{
    UINT32 length, i;
    UINT8 *pattern;
    if((pattern_id == g_led_bg_pattern_id) && (g_led_bg_timer_id != 0))
        return;
    pattern = gui_get_led_pattern(pattern_id, &length);
    if(pattern == NULL || length == 0 || strlen(pattern) == 0)
        return;

    g_led_bg_pattern = pattern;
    g_led_bg_pos = 0;
    g_led_bg_length = strlen(pattern);
    g_led_bg_pattern_id = pattern_id;

    for(i=0;i<length;i++)
    {
        if(pattern[i]>'0' && pattern[i]<='9') // have a number greater than 0
        {
            break;
        }
    }
    if(i==length) // if no delay, set count to 1
        g_led_bg_count=1;
    else
        g_led_bg_count=LED_LOOP_INFINITE;

    if(g_led_bg_timer_id != 0)
    {
        COS_KillTimer(g_led_bg_timer_id);
        g_led_bg_timer_id = 0;
    }
    
    LedBgTimeout(NULL);
}

#endif


//get nvram data buff address
void *NVRAMGetData(UINT8 index, UINT16 length)
{
    return DSM_GetUserData(index, length);
}

//write nvram data to flash
//write flash will use mci bigbuffer, only can use when voc closed!
PUBLIC VOID boot_InitFlashDriver(VOID);
BOOL NVRAMWriteData(void)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E) && !defined (WIN32)
    hal_HstSendEvent(SYS_EVENT, 0x98985655);
    if(hal_VocIsClose())
    {
        UINT32 status = hal_SysEnterSC();
        hal_HstSendEvent(SYS_EVENT, 0x98985656);
        boot_InitFlashDriver();
        hal_HstSendEvent(SYS_EVENT, 0x98985657);
        DSM_WriteUserData();
        hal_HstSendEvent(SYS_EVENT, 0x98985658);
        hal_SysExitSC(status);
    }
#else
    DSM_WriteUserData();
#endif
    return TRUE;
}

// 清空电话本并设置容量
void NVRAMResetPhoneBook(UINT16 size, UINT8 sizeofItem, UINT8 sizeofIndex)
{
    DSM_ResetPhoneBookAddress(size,sizeofItem,sizeofIndex);
}

void NVRAMErasePhoneBook(void)
{
    DSM_Erase_Phonebook();
}

// 取得电话本存储区域地址和长度
void NVRAMGetPhoneBookParam(UINT32 *length, UINT16 *size, UINT16 *valid_size)
{
    DSM_GetPhoneBookParam(length, size, valid_size);
}

void *NVRAMGetPhoneBookAddress()
{
	return (void*)DSM_GetPhoneBookAddress();
}

int NVRAMGetPhoneBookOffset(void)
{
	return DSM_GetPhoneBookOffset();
}

// 写电话本
void NVRAMWritePhoneBook(UINT8 *data, UINT16 index)
{
    DSM_WritePhoneBook(data, index);
}

// 写电话本索引
void NVRAMWritePhoneBookIndex(UINT8 *index, UINT16 size)
{
    DSM_WritePhoneBookIndex(index, size);
}


void SetContrast(INT8 ContrastValue)              //设置屏幕对比度
{
}

void RestartSystem(void)
{
    hal_SysRestart();
}


/// Return the date of build of platform.
/// The format is 0xYYYYMMDD.
UINT32 GetPlatformBuildDate(void)
{
    return platform_VerGetDate();
}

/// Return the version number
/// The format is 0xAABB, A is the big version, B is little version
UINT32 GetPlatformVersion(void)
{
    return platform_VerGetNumber();
}

/// Return the revision
UINT32 GetPlatformRevision(void)
{
    return platform_VerGetRevision();
}

#ifndef WIN32
#ifndef APP_NO_TRACE
void app_trace(UINT16 level, char* fmt,...)
{
    va_list args;

    va_start(args,fmt);
    #if OS_KERNEL == SXR 
    sxs_vprintf(_APP|TLEVEL(level), fmt, args);
    #else
    vvprintf(_APP|TLEVEL(APP_TCPIP_TRC), fmt, args);
    #endif
    va_end(args);
}
#endif

void lwip_trace(char* fmt,...)
{
    va_list args;

    va_start(args,fmt);
    #if OS_KERNEL == SXR 
    sxs_vprintf(_APP|TLEVEL(APP_TCPIP_TRC), fmt, args);
    #else
    vvprintf(_APP|TLEVEL(APP_TCPIP_TRC), fmt, args);
    #endif
    va_end(args);
}
#endif

