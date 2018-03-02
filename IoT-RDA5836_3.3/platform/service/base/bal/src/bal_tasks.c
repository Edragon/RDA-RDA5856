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


#include "chip_id.h"
#include "platform.h"
#include "event.h"
#include "base_prv.h"
#include "ap_common.h"
#include "ap_message.h"
#include "globalconstants.h"
#include "bal_config.h"
#include "hal_host.h"
#include "hal_usb.h"
#include "hal_timers.h"
#include "sul.h"
#include "ts.h"
#include "tgt_m.h"
#include "tgt_aud_cfg.h"
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5996)
#include "opal.h"
#include "sys_ctrl.h"
#include "global_macros.h"
#include "sys_irq.h"
#endif
//#include "ir.h"
#include "cos.h"
#include "hal_i2c.h"
#include "aud_m.h"
#include "dm.h"
#include "dm_m.h"
#include "dm_audio_ex.h"
#include "pm.h"
#include "gpio_edrv.h"
#include "dsm_dev_driver.h"

UINT16 g_PreKeyCode = 0;
UINT16 g_PreKeyPressTime = 0;
extern VOID DM_CheckPowerOnCause();
extern BOOL g_test_mode;

#define DM_REGISTER_HANDLER_ARRAY_SIZE DM_DEVID_END_+1

//PRIVATE PFN_DEVICE_HANDLER g_devHandlerArray[DM_REGISTER_HANDLER_ARRAY_SIZE];

extern UINT8 dm_GetStdKey(UINT8 key);
extern BOOL tm_AlramProc(COS_EVENT* pEv);
extern BOOL pm_PrvInfoInit();
extern BOOL pm_BatteryInit();
extern BOOL dm_PowerOnCheck(UINT16* pCause);
extern BOOL TM_PowerOn();
extern BOOL pm_BatteryMonitorProc(COS_EVENT * pEv);
extern VOID audio_ParseMsg(COS_EVENT* pEvent);
extern UINT8 DM_BuzzerSongMsg(COS_EVENT* pEvent);
extern void COS_HandleExpiredTimers(UINT8 timerHandle);
extern VOID BTSco_WriteData(INT16 *pMicData, INT16 *pSpkData, INT16 *pResData);
extern int Mcd_USBHostInit();
extern int Mcd_USBHostSchOperation(void *pData, uint8 status);
extern int Mcd_UsbHostDeInit(uint8 port);
extern BOOL boot_get_testmode(VOID);
extern void hal_musb_start();
extern void IR_Init();

#ifdef SUPPORT_KEY_FILTER
VOID BAL_ClearKeyFilter(VOID);
BOOL BAL_SetKeyFilter(UINT8 filter);
#endif /* SUPPORT_KEY_FILTER */
static UINT8 g_current_key;
static UINT8 g_key_timer;
static UINT8 s_key_hold_flag;
#ifdef SUPPORT_DOUBLECLICK
static UINT8 g_double_click_block_key = KEY_INVALID;
#endif
#if defined(__IRC_SUPPORT__)
static UINT8 g_irc_current_key;
static UINT8 g_irc_repeat_count;
static UINT8 g_irc_timer;
static UINT8 g_irc_hold_flag;
#endif

static UINT8 g_sys_timer_id;
UINT8 g_sys_sleep_flag = 0;
#if APP_SUPPORT_USB==1
static UINT8 g_sys_timer_id_usb;
static UINT8 UsbStatus = 0;
#endif

UINT8 g_rtc_count = 0;

UINT8 g_usb_active_after_bt;
//extern UINT8 usb_first_frame;

extern UINT8 mcd_usbHostInitOpCompleteCb_status; //added for delete nParam2
UINT32 irc_press_mode = 0;
extern UINT32 g_uAudIsPlaying;
extern INT8* Get_BtvoisEncOutput(void);
extern INT8* Get_BtAgcPcmBufferReceiver(void);
extern BOOL AT_AsyncEventProcess(COS_EVENT* pEvent);
extern VOID SA_UartRecevDataInd(COS_EVENT* pEvent);

void keyfilter_send(UINT32 keytype,UINT32 keycode);

/*
 PUBLIC VOID bal_IRhandle()
 {
 UINT32 ir_date_value = 0;
 UINT32 g_ircustomcode;
 COS_EVENT ev;

 ev.nEventId  = 0;
 ev.nParam1  = 0;

 hal_HstSendEvent(SYS_EVENT, 0x56059004);

 READ_REG(IR_CODE, g_ircustomcode);
 hal_HstSendEvent(SYS_EVENT, g_ircustomcode);
 READ_REG(IR_STATUS, ir_date_value);
 hal_HstSendEvent(SYS_EVENT, ir_date_value);

 if(1)
 {

 //ev.nParam1  = ((~(g_ircustomcode>>8)&0xff)<<16)+((~(g_ircustomcode>>16)&0xff)<<8)+(((g_ircustomcode>>16))&0xff);

 irc_press_mode = ir_date_value & 0xff;
 ev.nParam1  = g_ircustomcode;
 
 ev.nEventId  = EV_IRC_PRESS;
 COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);


 hal_HstSendEvent(SYS_EVENT,ev.nParam1 );

 }

 }
 */

#define SYS_TIMER_IDLE_INTERVAL_MS    (500)
#if APP_SUPPORT_LCD==0
#define SYS_TIMER_SLEEP_INTERVAL_MS   (20000)
#else
#define SYS_TIMER_SLEEP_INTERVAL_MS   (1000)
#endif

PUBLIC VOID bal_SetSleepState(BOOL sleep)
{
	if (sleep != g_sys_sleep_flag)
	{
		g_sys_sleep_flag = sleep;
		if (sleep)
			COS_ChangeTimer(g_sys_timer_id, SYS_TIMER_SLEEP_INTERVAL_MS); // 1s
		else
			COS_ChangeTimer(g_sys_timer_id, SYS_TIMER_IDLE_INTERVAL_MS);
	}
}

PUBLIC BOOL bal_GetSleepState(VOID)
{
    return g_sys_sleep_flag;
}

/******************************************************************************
 * BAL_SysTimerTimeout: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void BAL_SysTimerTimeout(void *param)
{
	COS_EVENT ev;

#if !defined(_T_UPGRADE_PROGRAMMER)
	static int BatteryStatue = 0;
	if (++BatteryStatue > 4)
	{
		BatteryStatue = 0;
		pm_BatteryMonitorProc(&ev);            // by ZM for 5855
	}

	if (boot_get_testmode())
		hal_HstSendEvent(SYS_EVENT, 0x7e570010);
#endif
    if(g_rtc_count < 10)
    {
        ev.nEventId = AP_MSG_RTC;
        ev.nParam1= 0;
        COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER,COS_EVENT_PRI_NORMAL);
        g_rtc_count ++;
     }

    if(bal_GetSleepState())
    {
        g_sys_timer_id = COS_SetTimer(SYS_TIMER_SLEEP_INTERVAL_MS, BAL_SysTimerTimeout, NULL,COS_TIMER_MODE_SINGLE);
    }
    else
    {
        g_sys_timer_id = COS_SetTimer(SYS_TIMER_IDLE_INTERVAL_MS, BAL_SysTimerTimeout, NULL,COS_TIMER_MODE_SINGLE);
    }
}


UINT8 g_linein_detect_flag;

void SendMessageForLineinDetect(void *param)
{
	COS_EVENT ev;

	g_linein_detect_flag = 0;
	//if(param)
	{
		ev.nEventId = EV_LINE_IN;
		ev.nParam1 = gpio_detect_linein();
//        ev.nParam2  = 0;//for delete nParam2
//        ev.nParam3  = 0;//for delete nParam3
		COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}
}

BOOL turn_key = TRUE;
BOOL dm_SendKeyMessage(COS_EVENT* pEv)
{
#if 0
	if(g_devHandlerArray[DM_DEVID_KEY])
	g_devHandlerArray[DM_DEVID_KEY](pEv);
	else
	COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	return TRUE;

	sxr_SbxInfo_t Mail;
	Mail.NbAvailMsg=0;
	if (COS_GetDefaultMmiTaskHandle())
	sxr_GetSbxInfo( ((TASK_HANDLE*)COS_GetDefaultMmiTaskHandle())->nMailBoxId, &Mail);

	if ((Mail.NbAvailMsg >30)&&(turn_key==FALSE))
	{
		CSW_TRACE(BASE_BAL_TS_ID, TSTXT("\n !!keyboard mailbox full \n") );
		//turn_key=TRUE; // simon suggest that discard the all key message when the mail box nearly full
		return FALSE;
	}
	if(pEv->nEventId==EV_KEY_DOWN)
	turn_key=TRUE;
	if(pEv->nEventId==EV_KEY_UP)
	turn_key=FALSE;

	if(g_devHandlerArray[DM_DEVID_KEY])
	g_devHandlerArray[DM_DEVID_KEY](pEv);
	else
	{
		COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}
#endif
	return TRUE;
}

BOOL dm_SendPowerOnMessage(COS_EVENT* pEv)
{
#if 0
	if(g_devHandlerArray[DM_DEVID_POWER_ON])
	g_devHandlerArray[DM_DEVID_POWER_ON](pEv);
	else
#endif
	COS_SendEvent(MOD_APP, pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

	return TRUE;
}

BOOL dm_SendArlarmMessage(COS_EVENT* pEv)
{
#if 0
	if(g_devHandlerArray[DM_DEVID_ALARM])
	g_devHandlerArray[DM_DEVID_ALARM](pEv);
	else
#endif
	COS_SendEvent(MOD_APP, pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

	return TRUE;
}

BOOL dm_SendPMMessage(COS_EVENT* pEv)
{
#if 0
	if(g_devHandlerArray[DM_DEVID_PM])
	{
		g_devHandlerArray[DM_DEVID_PM](pEv);
	}
	else
#endif
	{
		COS_SendEvent(MOD_APP, pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}

	return TRUE;
}

void handle_key_timeout(void *param)
{
    COS_EVENT ev;
    CONST BAL_CFG_KEY_T *p_KeyCfg = tgt_GetKeyCfg();


    switch(g_current_key)
    {
#if APP_SUPPORT_HOLD_NEXT_FOR_VOL == 1
        case KEY_PREV:
            ev.nParam1 = KEY_VOL_DOWN;
            ev.nEventId = EV_KEY_PRESS;
            break;

        case KEY_NEXT:
            ev.nParam1 = KEY_VOL_UP;
            ev.nEventId = EV_KEY_PRESS;
            break;

#elif APP_SUPPORT_HOLD_VOL_FOR_NEXT == 1
        case KEY_VOL_DOWN:
            ev.nParam1 = KEY_PREV;
            ev.nEventId = EV_KEY_PRESS;
            break;

        case KEY_VOL_UP:
            ev.nParam1 = KEY_NEXT;
            ev.nEventId = EV_KEY_PRESS;
            break;
#endif

        default:
            ev.nEventId = EV_KEY_LONG_PRESS;
            ev.nParam1 = g_current_key;
            break;
            
    }

	g_current_key = ev.nParam1;

#ifdef SUPPORT_DOUBLECLICK
    /*Not double click, send block key before current key*/
	if((g_current_key != g_double_click_block_key) && (KEY_INVALID != g_double_click_block_key))
	{
		keyfilter_send(EV_KEY_PRESS, g_double_click_block_key);
    	keyfilter_send(EV_KEY_UP, g_double_click_block_key);
    	g_double_click_block_key = KEY_INVALID;
	}
	else
	{
	    ;
	}
#endif /* SUPPORT_DOUBLECLICK */

#if APP_SUPPORT_HOLD_VOL_FOR_NEXT == 1
	if((0 == s_key_hold_flag) )
	{
		keyfilter_send(ev.nEventId, ev.nParam1);
	}
	else
	{
	        if ( (ev.nParam1 == KEY_NEXT) || (ev.nParam1 == KEY_PREV))
	        {
	            //hal_HstSendEvent(BOOT_EVENT,0x9999992);
	        }
	       else
	       {
	    	     keyfilter_send(ev.nEventId, ev.nParam1);
	       }
	}
#else
    keyfilter_send(ev.nEventId, ev.nParam1);
#endif

    s_key_hold_flag = 1;
    COS_ChangeTimer(g_key_timer, p_KeyCfg->repeat_interval);
}

#ifdef SUPPORT_KEY_FILTER
volatile UINT8 g_key_filter = 0;
VOID BAL_ClearKeyFilter(VOID)
{
	app_trace(APP_SETTING_TRC, "Clear key filter mask %d", g_key_filter);
	UINT32 csStatus;
	csStatus = hal_SysEnterCriticalSection();
	g_key_filter = 0;
	hal_SysExitCriticalSection(csStatus);
}

/*
  return TRUE - if filter is not set   FALSE - filter was set
*/
BOOL BAL_SetKeyFilter(UINT8 filter)
{
	UINT32 csStatus;

	if(g_key_filter&filter)
	{
		return FALSE;
	}

	csStatus = hal_SysEnterCriticalSection();
	g_key_filter |= filter;
	hal_SysExitCriticalSection(csStatus);

	return TRUE;
}
#endif

#ifdef SUPPORT_DOUBLECLICK
static UINT8 g_doubleclick_key_timer;
void doubleclick_key_timeout(void *param)
{
    if((KEY_INVALID != g_double_click_block_key) && (0 == s_key_hold_flag))
    {
        hal_HstSendEvent(SYS_EVENT,0x66660002);
    	keyfilter_send(EV_KEY_PRESS, g_double_click_block_key);
    	keyfilter_send(EV_KEY_UP, g_double_click_block_key);
    	g_double_click_block_key = KEY_INVALID;
    }

    g_doubleclick_key_timer = 0;
}

#endif

void keyfilter_send(UINT32 keytype,UINT32 keycode)
{
    COS_EVENT ev; 
	
    #ifdef SUPPORT_KEY_FILTER
    UINT8 key_filter=0;

    switch(keytype)
    {
        case EV_KEY_DOWN:
            key_filter = (0x1);
            break;
        case EV_KEY_PRESS:
            key_filter = (0x2);
            break;
        case EV_KEY_UP:
            key_filter = (0x4);
            break;
        case EV_KEY_LONG_PRESS:
            key_filter = (0x8);
            break;
		case EV_KEY_DOUBLE_CLICK:
            key_filter = (0x10);
            break;
			
    }
	if(!BAL_SetKeyFilter(key_filter))
	{
	    hal_HstSendEvent(SYS_EVENT,0x66660006);
		//app_trace(APP_SETTING_TRC, "Key PRESS message %d is ignored for %d key is unhandled", key, g_current_key);
		return;
	}
    #endif
	
	ev.nParam1 = keycode;
	ev.nEventId = keytype;
	COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	app_trace(APP_SETTING_TRC, "Key %d, Type %d is sended", keycode, keytype);
	
}

u8 global_first_keydown=0;

BOOL bal_is_first_keydown()
{
	if(global_first_keydown==1)
		return	TRUE;
	else
	{
		global_first_keydown = 2;
		return	FALSE;
	}
}

void handle_Key_Event(UINT32 event, UINT32 key)
{
	UINT8 k;
	CONST BAL_CFG_KEY_T *p_KeyCfg = tgt_GetKeyCfg();

	k = p_KeyCfg->keymap[key];

    app_trace(APP_SETTING_TRC, "Receive Key message: event=%d, key=%d, current_key=%d", event, key, g_current_key);
    
	if (key == 0 && k == KEY_INVALID) // power key is not used
		event = EV_KEY_UP;

	if (event == EV_KEY_UP || g_current_key != KEY_INVALID)
	{


		global_first_keydown = 2;


        //Kill longpress, double click timer
		COS_KillTimer(g_key_timer);
		g_key_timer = 0;
#ifdef SUPPORT_DOUBLECLICK
	    COS_KillTimer(g_doubleclick_key_timer);
        g_doubleclick_key_timer = 0;
#endif /* SUPPORT_DOUBLECLICK */

		if (s_key_hold_flag == 0)
		{
#ifdef SUPPORT_DOUBLECLICK
            /*double click is NOT pending*/
	        if((g_current_key != KEY_INVALID) && (KEY_INVALID == g_double_click_block_key))
			{
    	        hal_HstSendEvent(SYS_EVENT,0x66660000);
    	        g_double_click_block_key = g_current_key;
    	        g_doubleclick_key_timer = COS_SetTimer(p_KeyCfg->doubleclick_internal, 
    	                                        doubleclick_key_timeout, NULL, COS_TIMER_MODE_SINGLE);
			}
			/**/
		    else if((g_current_key == g_double_click_block_key) && (KEY_INVALID != g_double_click_block_key))
		    {
                hal_HstSendEvent(SYS_EVENT,0x66660001);
        		keyfilter_send(EV_KEY_DOUBLE_CLICK, g_double_click_block_key);
            	keyfilter_send(EV_KEY_UP, g_double_click_block_key);
            	g_double_click_block_key = KEY_INVALID;
		    }
			else
#endif
			{
			    keyfilter_send(EV_KEY_PRESS, g_current_key);
	    		keyfilter_send(EV_KEY_UP, g_current_key);
			}
		}
		else
		{
    		keyfilter_send(EV_KEY_UP, g_current_key);
			s_key_hold_flag = 0;
		}

		g_current_key = KEY_INVALID;
	}
	if (event == EV_KEY_DOWN)
	{
		if (g_current_key == KEY_INVALID) //ignore the current key press,because of more than one key are pressing,xzc
		{
			s_key_hold_flag = 0;
			g_current_key = k;


            if(k == tgt_GetKeyCode(0) && (global_first_keydown == 0))
            {
			    global_first_keydown =1;
            }

#ifdef SUPPORT_DOUBLECLICK
            /*Not double click, send block key before current key*/
			if((g_current_key != g_double_click_block_key) && (KEY_INVALID != g_double_click_block_key))
			{
        		keyfilter_send(EV_KEY_PRESS, g_double_click_block_key);
            	keyfilter_send(EV_KEY_UP, g_double_click_block_key);
                keyfilter_send(EV_KEY_DOWN, g_current_key);
            	g_double_click_block_key = KEY_INVALID;
			}
			else if((g_current_key == g_double_click_block_key) && (KEY_INVALID != g_double_click_block_key))
			{
			    ;
			}
			else
#endif /* SUPPORT_DOUBLECLICK */
            {
                keyfilter_send(EV_KEY_DOWN, k);
            }

            g_key_timer = COS_SetTimer(p_KeyCfg->repeat_timeout, handle_key_timeout, NULL, COS_TIMER_MODE_PERIODIC);
        }
    }
}

#if defined(__IRC_SUPPORT__)
void handle_irc_timeout(void *param)
{
	COS_EVENT ev;

	ev.nParam1 = g_irc_current_key;

	if (g_irc_hold_flag == 0)
	{
		ev.nEventId = EV_KEY_PRESS;
		COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        app_trace(APP_SETTING_TRC, "Key PRESS in handle_irc_timeout of %d is sended", ev.nParam1);
    }
	ev.nEventId = EV_KEY_UP;
	COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    app_trace(APP_SETTING_TRC, "Key UP in handle_irc_timeout of %d is sended", ev.nParam1);

	g_irc_timer = 0;
}


void handle_irc_event(UINT32 code, UINT32 mode)
{
	int i, j = 0;
	int key, c;
	COS_EVENT ev;
	CONST BAL_CFG_IRC_T *pIRCCfg = tgt_GetIRCCfg();
	CONST UINT8 *irc_map;

	//hal_HstSendEvent(SYS_EVENT,code);
	hal_HstSendEvent(SYS_EVENT, code);
	hal_HstSendEvent(SYS_EVENT, 0x90049005);

	c = (code & 0xffff);
	for (j = 0; j < pIRCCfg->irc_styles; j++)
	{
		if (pIRCCfg->style_list[j] == c)
			break;
	}
	if (j == pIRCCfg->irc_styles)
		return;

	key = KEY_INVALID;
	c = (code >> 16) & 0xff;
	irc_map = pIRCCfg->code_map + j;
	for (i = 0; i < pIRCCfg->map_size; i++)
	{
		if (irc_map[i * pIRCCfg->irc_styles] == c)
		{
			key = i + 1;
			break;
		}
	}
	//hal_HstSendEvent(SYS_EVENT,key);
	hal_HstSendEvent(SYS_EVENT, key);

	if (g_irc_timer == 0)
		mode = 1;
	ev.nParam1 = key;
	if (mode == 1)
	{
		g_irc_repeat_count = 0;
		g_irc_current_key = key;
		g_irc_hold_flag = 0;
		COS_KillTimer(g_irc_timer);
		g_irc_timer = 0;

		if (key != KEY_INVALID)
		{
			g_irc_timer = COS_SetTimer(pIRCCfg->timeout, handle_irc_timeout,
			NULL, COS_TIMER_MODE_SINGLE);
			ev.nEventId = EV_KEY_DOWN;
			COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
			app_trace(APP_SETTING_TRC,
					"Key DOWN in handle_irc_event of %d is sended", ev.nParam1);
		}
	} else if (mode == 8)
	{
		if (g_irc_current_key == key && key != KEY_INVALID)
		{
			COS_ChangeTimer(g_irc_timer, pIRCCfg->timeout);
			g_irc_repeat_count++;
			if (g_irc_repeat_count > 10)
			{
				g_irc_hold_flag = 1;
				g_irc_repeat_count = 0;
				ev.nEventId = EV_KEY_LONG_PRESS;
				COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER,
				COS_EVENT_PRI_NORMAL);
				app_trace(APP_SETTING_TRC,
						"Key LONG_PRESS in handle_irc_event of %d is sended",
						ev.nParam1);
			}
		}
	}
}
#endif

//
// Internal Tasks for BAL
//
VOID umss_TransportEventProcess(UINT32 nParam1);

#if APP_SUPPORT_USB==1
void hal_UsbDisconnCb(BOOL isHost)
{
	extern BYTE g_usb_connected;

	//hal_HstSendEvent(SYS_EVENT,0x22223333);
	g_usb_connected = 0;
#ifdef MCD_USB_HOST_SUPPORT
	if(isHost)
	{
		COS_EVENT ev;
		Mcd_UsbHostDeInit(0);

		
		#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
	       hwp_sysCtrl->per_module_en &= ~SYS_CTRL_USBC_EN ; /**usbc hclk & usbphy clock en**/
		hal_musb_start_synops();
		#endif
		ev.nEventId = EV_FS_USB_PLUG_OUT;
		ev.nParam1 = 0;
		COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}
#endif
}

extern UINT32 TEST_M;

int count=0;
volatile int uart_tx_test=0;
UINT32 system_usb_ticks = 0;
extern BYTE g_usb_connected;
VOID BAL_SysTimerTimeout_USB()
{
	//COS_EVENT ev;

	if(gpio_detect_usb()) //
	{
		if( g_usb_connected==1 && UsbStatus == 1)
		{
			hal_HstSendEvent(SYS_EVENT, 0xa1b2c3d4);
			hal_HstSendEvent(SYS_EVENT, hal_TimGetUpTime() - system_usb_ticks);
			hal_UsbClose();
			COS_KillTimer(g_sys_timer_id_usb);

			//            ev.nEventId = EV_DM_USB_PLUG_IN;
			//            COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		}
	}

//        if(g_usb_connected==1 && UsbStatus == 2)
//        {
//            COS_KillTimer(g_sys_timer_id_usb);
//        }

}

#endif 

VOID BAL_SysTask(void* pData)
{
	COS_EVENT ev;
	BOOL bRet;
	UINT16 Cause;
#ifndef DISABLE_USB_SUPPORT
#ifdef  MCD_MGC_USB	
	hal_musb_start();
#endif
#endif
	CONST AUD_STREAM_INIT_T AudInit = tgt_GetAudInit();
	g_key_timer = 0;
	g_current_key = KEY_INVALID;
	s_key_hold_flag = 0;

	bRet = TRUE;
	SUL_ZeroMemory32(&ev, SIZEOF(COS_EVENT));
	DSM_ReadUserData();

#ifdef UART2_SUPPORT
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
	hwp_sysCtrl->per_module_en |= SYS_CTRL_UART_EN; 
#endif
	uart2_start();
#endif

#if !defined(_T_UPGRADE_PROGRAMMER)
	pm_BatteryInit();
	dm_Init();

	dm_PowerOnCheck(&Cause);
	TM_PowerOn();
#if defined(__IRC_SUPPORT__)
	IR_Init();
#endif /* __IRC_SUPPORT__ */

	//update by wangxin
	(*AudInit)();

    //DM_Audio_Side_test(1); 
    //while(1)COS_Sleep(100);
	if (gpio_detect_earpiece())
		DM_SetAudioMode(1); // default set audio path earpiece
	else
		DM_SetAudioMode(2); // default set audio path loud speaker
#endif

	g_sys_timer_id = COS_SetTimer(SYS_TIMER_IDLE_INTERVAL_MS, BAL_SysTimerTimeout, NULL,
	COS_TIMER_MODE_SINGLE);
	g_sys_sleep_flag = FALSE;
	//hal_SysRequestFreq(0, HAL_SYS_FREQ_26M, NULL);
	CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask Start. \n"));
#ifndef DISABLE_USB_SUPPORT
//    hal_UsbOpen(NULL);
#ifdef  MCD_MGC_USB	
	//hal_musb_start();
#else
  hal_musb_start_synops();
#endif
#endif
	for (;;)
	{
		COS_WaitEvent(MOD_SYS, &ev, COS_WAIT_FOREVER);

		switch (ev.nEventId)
		{
#ifdef UART2_SUPPORT
			case EV_UART2_DATA:
			hal_HstSendEvent(SYS_EVENT, 0x00275854);
			uart2_deal_data(ev.nParam1);
			break;
#endif

#if !defined(_T_UPGRADE_PROGRAMMER)
#if defined(__AT_SUPPORT__)
			case EV_DM_UART_RECEIVE_DATA_IND:
			SA_UartRecevDataInd(&ev);
			break;
#endif /* __AT_SUPPORT__ */
			case EV_KEY_UP:
			case EV_KEY_DOWN:
			case EV_KEY_PRESS:
			case EV_KEY_LONG_PRESS:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
				//key layout on MainBoard,
				//define in KEY_MAP
				//Only using on 5855, by xzc
				// KEY_INVALID  KEY_PLAY/18    KEY_NEXT/19        KEY_PREV/21
				// KEY_INVALID  KEY_MODE/22    KEY_REC/23         KEY_INVALID/24
				// KEY_POWER    KEY_EQ/25      KEY_VOL_UP/26      KEY_VOL_DOWN/27
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)
				//key layout on MainBoard,
				//define in KEY_MAP
				//Only using on 8809P, by xzc
				// KEY_INVALID  KEY_LOOP/18    KEY_NEXT/19        KEY_PREV/20
				// KEY_INVALID  KEY_EQ/21       KEY_REC/22         KEY_MODE/23
				// KEY_POWER    KEY_PLAY/24      KEY_VOL_UP/25      KEY_VOL_DOWN/26
#endif
				handle_Key_Event(ev.nEventId, ev.nParam1);
				break;
			case EV_KEY_CALIB:
				ev.nEventId = AP_MSG_KEY_CALIB;
				ev.nParam1 = 0;
				COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER,
						COS_EVENT_PRI_NORMAL);
				break;
#if defined(__IRC_SUPPORT__)
			case EV_IRC_PRESS:
				handle_irc_event(ev.nParam1, irc_press_mode);
				break;
#endif
#endif
			case EV_TIMER:
				COS_HandleExpiredTimers(ev.nParam1);
				break;

			case EV_TIM_ALRAM_IND:
			case EV_ALARM:
				ev.nEventId = EV_ALARM;
				COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER,
						COS_EVENT_PRI_NORMAL);
				break;
			case EV_FS_TFLASH_DETECT:
				{
					COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
					CSW_TRACE(BASE_BAL_TS_ID, "SendMessageForTFlashDetect, param1=%d.", ev.nParam1);
				}
				break;
#if 0
				case EV_DM_USB_PLUG_IN:

				if(UsbStatus == 0)
				{
					//                   mediaSendCommand(MC_STOP, 0);
					hal_HstSendEvent(SYS_EVENT, 0x2013ffee);
					system_usb_ticks = hal_TimGetUpTime();
					//      hal_UsbOpen(NULL);
					UsbStatus = 1;
					g_sys_timer_id_usb = COS_SetTimer(1, BAL_SysTimerTimeout_USB, NULL, COS_TIMER_MODE_PERIODIC);

				}
				else if( (g_usb_connected==1 && UsbStatus == 1)||(g_usb_active_after_bt == 1))
				{
					//                hal_HstSendEvent(SYS_EVENT,UsbStatus);
					//                hal_HstSendEvent(SYS_EVENT,g_usb_active_after_bt);
					//     	 hal_UsbClose();//
					UsbStatus = 2;
					hal_HstSendEvent(SYS_EVENT,0x04170055);
					//       hal_UsbClose();
					//g_usb_connected = 1;
					ev.nEventId = EV_FS_USB_PLUG_DETECT;
					ev.nParam1 = 1;
					ev.nParam2 = 0;
					COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

				}

				break;
#endif

#if APP_SUPPORT_USB==1
				case EV_FS_USB_PLUG_DETECT:
				{
#ifndef 	MCD_MGC_USB		
					COS_Sleep(60); /*wait a moment, or else we cann't get the right info*/
#endif
					hal_UsbSetDisconnCb(hal_UsbDisconnCb);
					if(hal_UsbIsWorkAsHost() == FALSE) // device
					{
						g_usb_connected = ev.nParam1;

						hal_HstSendEvent(USB_EVENT, 0x09061001);
						hal_HstSendEvent(USB_EVENT, g_usb_connected);
						g_usb_connected=1;
						if(g_usb_connected)
						{
							ev.nEventId = EV_FS_UMSS_DETECT;
							ev.nParam1 = 0;
							COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
						}
						else
						{
							ev.nEventId = EV_FS_UMSS_PLUG_OUT;
							ev.nParam1 = 0;
							COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
						}
					}
#ifdef MCD_USB_HOST_SUPPORT
					else // host
					{
						hal_HstSendEvent(SYS_EVENT,0x08150003);
						hal_HstSendEvent(SYS_EVENT,ev.nParam1);
						/*usb device plugged in*/

						Mcd_USBHostInit();
					}
#endif
				}
				break;
				case EV_FS_USB_PLUG_OUT:
				{
					hal_HstSendEvent(SYS_EVENT,0x09061002);
					ev.nEventId = EV_FS_UDISK_REMOVE;
					ev.nParam1 = 0;
					#if 0
#ifdef MCD_USB_HOST_SUPPORT				
					Mcd_UsbHostDeInit(0);
#ifdef  MCD_MGC_USB			
					hal_musb_start();
#endif			
#endif			
                    #endif
                    hal_HstSendEvent(USB_EVENT,0x09061002);
					//hal_IrqMaskCon(3);
					COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
				}
				break;
#endif
			case EV_LINE_IN:
				//hal_HstSendEvent(SYS_EVENT,0x08075000);
				//hal_HstSendEvent(SYS_EVENT,ev.nParam1);
				if (g_linein_detect_flag == 0)
				{
					COS_SetTimer(500, SendMessageForLineinDetect, NULL,
					COS_TIMER_MODE_SINGLE);
					g_linein_detect_flag = 1;
				}
				break;
#if !defined(_T_UPGRADE_PROGRAMMER)
			case EV_DM_EAR_PEACE_DETECT_IND:
				//hal_HstSendEvent(SYS_EVENT,0x06140100);
				//hal_HstSendEvent(SYS_EVENT,ev.nParam1);
				if (ev.nParam1)
				{
					DM_SetAudioMode(1); // ear peace
				} else
				{
					DM_SetAudioMode(2); // loud speaker
				}
				break;
			case SYS_EV_MASK_AUDIO_TONE:
				{
					//DM_ToneMsgHandlerEx(&ev);
				}
				break;
			case EV_DM_AUDIO_CONTROL_IND:
				{
					if (0x01 == ev.nParam1)
					{
						hal_HstSendEvent(SYS_EVENT,0xba105c00);
						DM_StartAudioEX();
						hal_HstSendEvent(SYS_EVENT,0xba105c01);
					} else
					{
						hal_HstSendEvent(SYS_EVENT,0xba105c02);
						DM_StopAudioEX();
						hal_HstSendEvent(SYS_EVENT,0xba105c03);
					}
				}
				break;
			case EV_DM_SET_AUDIO_VOLUME_IND:
				{
					DM_SetAudioVolumeEX(ev.nParam1);
				}
				break;
			case EV_DM_SET_MIC_GAIN_IND:
				{
					DM_SetMicGainEX(ev.nParam1);
				}
				break;
			case EV_DM_SET_AUDIO_MODE_IND:
				{
					DM_SetAudioModeEX((UINT8) ev.nParam1);
				}
				break;
#endif
#if 0
				case EV_FS_ASYN_READ_REQ:
				FS_AsynReadPrc(ev.nParam1);
				break;
				case EV_FS_ASYN_WRITE_REQ:
				FS_AsynWritePrc(ev.nParam1);
				break;
#endif
				/*
				 case EV_FS_USB_PLUG_IN:
				 if( RDAUsb_ServiceAsHost())
				 {
				 Mcd_USBHostInit();
				 }
				 else
				 {
				 uctls_Open(0, 0x0000, 0x0000, 0x0000, "USB Controller 1.0");
				 }
				 CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask: %d \n"), ev.nEventId );
				 break;
				 }
				 */
#ifdef MCD_USB_HOST_SUPPORT
				case EV_FS_USB_INIT:
				Mcd_USBHostSchOperation((void*)ev.nParam1, mcd_usbHostInitOpCompleteCb_status);
				COS_Sleep(60);
				//hal_HstSendEvent(SYS_EVENT,0x11070001);
				hal_HstSendEvent(SYS_EVENT,0x11070002);
				break;
#endif
#if APP_SUPPORT_USB==1
			case EV_FS_USB_TRANSFERRED:
				{
					//COS_ReleaseSemaphore((HANDLE)ev.nParam1);
				}
				break;
#if !defined(_T_UPGRADE_PROGRAMMER)
			case EV_USB_MSS_EVENT_ID:
				umss_TransportEventProcess(ev.nParam1);
				break;
#ifdef USB_AUDIO_SUPPORT
				case EV_USB_AUD_EVENT_ID:
				uaud_TransportEventProcess(ev.nParam1);
				break;
#endif           
#endif // APP_SUPPORT_USB==1
#endif

#ifdef BT_SCO_RECORD_SUPPORT
				case EV_BT_SCO_REC:
				if(ev.nParam1 == 0)
				{	
					BTSco_WriteData(Get_BtRecordMicPcmBuffer()+320, Get_BtAgcPcmBufferReceiver()+320, Get_BtvoisEncOutput()+320);
				}
				else
				{ 
					BTSco_WriteData(Get_BtRecordMicPcmBuffer(), Get_BtAgcPcmBufferReceiver(), Get_BtvoisEncOutput());
				}
				break;
#endif

			default:
#if defined(__AT_SUPPORT__)
				AT_AsyncEventProcess(&ev);

#endif /* __AT_SUPPORT__ */

				break;

		}

	} // for(;;) loop

}

