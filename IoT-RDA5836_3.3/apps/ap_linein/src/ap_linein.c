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

#include "ap_gui.h"
#include "ap_linein.h"
#include "hal_host.h"
#include "hal_debug.h"

#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define LINEIN_MAX_VOLUME               7
#else
#define LINEIN_MAX_VOLUME               AUD_MAX_LEVEL
#endif

extern slider_t              slider;
extern BOOL     ui_auto_update;

typedef enum
{
    LINEIN_STATUS_STOP,
    LINEIN_STATUS_PLAY,
    LINEIN_STATUS_MAX,
}APP_LINEIN_STATUS;


#if APP_SUPPORT_INDEPEND_VOLUMNE==0   
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define LINEIN_DEFAULT_VOLUME     2
INT8   linein_volume=LINEIN_DEFAULT_VOLUME;
#define g_linein_volume     linein_volume
#else
#define g_linein_volume     (g_comval->Volume)
#endif
#else
INT8        g_linein_volume = 8;
#endif

static APP_LINEIN_STATUS linein_status = LINEIN_STATUS_STOP;
extern BOOL mediaSendLineInCommand(UINT8 cmd, UINT32 param);

/******************************************************************************
 * LineinEntryStatus: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
VOID LineinEntryStatus(APP_LINEIN_STATUS new_status)
{
	app_trace(APP_LINEIN_TRC, "LineinEntryStatus old=%d,new=%d", linein_status, new_status);

    switch(new_status)
    {
        case LINEIN_STATUS_PLAY:
            mediaSendLineInCommand(MC_OPEN, 0);
#if APP_SUPPORT_DELAY_VOLUME==1
		    mediaSendLineInCommand(MC_SETVOLUME, 0);
		    COS_Sleep(1000);
#endif
		    mediaSendLineInCommand(MC_SETVOLUME, g_linein_volume);
            break;

        case LINEIN_STATUS_STOP:
	     #if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
            mediaSendLineInCommand(MC_SETVOLUME, LINEIN_DEFAULT_VOLUME);
	     #else
	     mediaSendLineInCommand(MC_SETVOLUME, 0);
	     #endif
            mediaSendLineInCommand(MC_CLOSE, 0);
            break;

        default:
            APP_ASSERT(0);
            break;
    }

    linein_status = new_status;
}

#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)

void LineIn_VolumeCb( INT32 Vol )
{
    mediaSendLineInCommand(MC_SETVOLUME, Vol);
}
#endif

/******************************************************************************
 * LineIn_ChangeVolume: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
INT32 LineIn_ChangeVolume(INT8 offset)
{
    //INT32 result;
	BOOL play_tone = FALSE;
	
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    if(AP_Support_LCD())
    {
        //show background
        GUI_ClearScreen(NULL);
        GUI_ResShowPic(GUI_IMG_VOLUMEBK,0,0);
            
        slider.min = 0;
        slider.max = LINEIN_MAX_VOLUME;
        slider.value = g_linein_volume;
        slider.step = 1;
            
        ui_auto_update = FALSE;
        GUI_Slider(& slider, LineIn_VolumeCb);
        ui_auto_update = TRUE;
    }
    else
#endif
    {
        g_linein_volume += offset;

        if(g_linein_volume > LINEIN_MAX_VOLUME)
        {
            g_linein_volume = LINEIN_MAX_VOLUME;
			play_tone = TRUE;
        }
        else if(g_linein_volume < 0)
        {
            g_linein_volume = 0;
			play_tone = TRUE;
        }
        mediaSendLineInCommand(MC_SETVOLUME, g_linein_volume);
    }
    if(play_tone)
    {
    	GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_VOL_MAXORMIN, GUI_MSG_FLAG_WAIT);
    }


    return 0;
}

BOOL AppLineinKeyHandler(UINT32 key)
{
	switch(key)
	{
        case AP_KEY_VOLADD | AP_KEY_DOWN:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
		case AP_KEY_PREV   | AP_KEY_HOLD:
        case AP_KEY_PREV   | AP_KEY_DOWN:
            LineIn_ChangeVolume(1);
            break;

        case AP_KEY_VOLSUB | AP_KEY_DOWN:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
      	case AP_KEY_NEXT   | AP_KEY_HOLD:
        case AP_KEY_NEXT   | AP_KEY_DOWN:
            LineIn_ChangeVolume(-1);
            break;

        case AP_KEY_PLAY| AP_KEY_PRESS:
            if(LINEIN_STATUS_STOP == linein_status)
            {
                LineinEntryStatus(LINEIN_STATUS_PLAY);
            }
            else
            {
                LineinEntryStatus(LINEIN_STATUS_STOP);
            }
            break;
        default:
        	return FALSE;
     }

	return TRUE;
}
/******************************************************************************
 * AppLineinMsgHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL AppLineinMsgHandler(COS_EVENT *pEvent)
{

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
		    ui_auto_update = FALSE;
#endif
		    hal_HstSendEvent(SYS_EVENT,0x08078000); 
            LineinEntryStatus(LINEIN_STATUS_PLAY);
			SendAppEvent(EV_UI_FW_REDRAW, 0);
			break;
			
        case EV_UI_FW_ON_RESUME:
            LineinEntryStatus(linein_status);
			break;

        case EV_UI_FW_REDRAW:
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);
                GUI_ResShowPic(GUI_IMG_LINEIN, 0, 0);
                GUI_DisplayBattaryLever();
                GUI_UpdateScreen(NULL);
            }
#elif APP_SUPPORT_LED8S==1
            {
                GUI_ClearScreen(NULL);
                GUI_ResShowPic(GUI_ICON_PLAY|GUI_ICON_LINEIN,0,0);
                GUI_DisplayText(0, 0, "LINE");
                GUI_UpdateScreen(NULL);
            }
#endif
            LED_SetPattern((LINEIN_STATUS_STOP == linein_status) ? GUI_LED_LINEIN_STOP : GUI_LED_LINEIN_PLAY, 
                    LED_LOOP_INFINITE);
            break;

        case AP_MSG_STANDBY:
            break;

        case AP_MSG_REMOVE_LINE_IN:
        	AppGotoMainMenu();
        	break;

        case AP_MSG_WAIT_TIMEOUT:
            APP_Sleep();
            break;

        case EV_UI_FW_ON_PAUSE:
            {
                APP_LINEIN_STATUS prev_status = linein_status;
                LineinEntryStatus(LINEIN_STATUS_STOP);
                linein_status = prev_status;
            }
            break;

        case EV_UI_FW_ON_EXIT:
            LineinEntryStatus(LINEIN_STATUS_STOP);
            break;

        default:
            //key event
            if(AppLineinKeyHandler(MESSAGE_ConvertToKeyValue(pEvent)))
            {
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
/*********************************************************************************
* Description : LINE IN o¡¥¨ºy¡ê??¨²???¡ê¨º???
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 LINEIN_Entry(INT32 param)
{
	app_trace(APP_LINEIN_TRC, "====LINEIN_Entry====");

    EntryNewScreen(SCR_ID_LINEIN_MAIN, AppLineinMsgHandler, NULL, 0, 0);
	return 0;
}






     

