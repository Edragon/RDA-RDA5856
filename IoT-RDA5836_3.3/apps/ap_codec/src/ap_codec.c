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
#include "ap_media.h"
#include "at_common.h"
#include "ap_codec.h"

#include "event.h"
#include "hal_debug.h"
#include "hal_host.h"

UINT8 *g_codec_buffer = NULL;

//static UINT8 codec_poweron_init = 0;
extern UINT8 g_flash_write_req;

extern int rdabt_poweron_init(void);
extern void RDA_bt_Power_Off(void);

/******************************************************************************
 * UartPlayMainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL UartPlayMainMsgHandler(COS_EVENT *pEvent)
{
	UINT32 key;
	BOOL ret = TRUE;

	APP_ASSERT(pEvent);
	//app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, start 11111");

	switch (pEvent->nEventId) {
		case EV_UI_FW_ON_START:
#ifdef AP_CODEC_USE_BT_SRAM
			g_flash_write_req = 1;

			hal_HstSendEvent(SYS_EVENT, 0x20170315);
			rdabt_poweron_init();
			hal_HstSendEvent(SYS_EVENT, 0x20170325);
			RDA_WRITE_REG(0xa1614004, 0xffff0000);
			RDA_WRITE_REG(0xa1614000, 0x1);
			hal_HstSendEvent(SYS_EVENT, 0x20170316);

			g_codec_buffer = (UINT8*)AP_CODEC_BT_SRAM_BASE;
#else
			g_codec_buffer = (UINT8 *)COS_Malloc(AP_CODEC_BUFFER_SIZE_MAX);
			if (g_codec_buffer == NULL)
			{
				ret = FALSE;
				app_trace(APP_MUSIC_TRC, "ap_codec: malloc codec buffer fail.");
			}
#endif
			/*if (codec_poweron_init == 0) {
				BAL_ATInit();
				codec_poweron_init = 1;
			};*/
			mediaSendCommand(MC_SETVOLUME, g_comval->Volume);
			AT_WriteUart((UINT8*)"\r\nCODEC_READY\r\n", AT_StrLen("\r\nCODEC_READY\r\n"));
			hal_HstSendEvent(SYS_EVENT, 0x20170317);
			app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, start");
		break;

		case EV_UI_FW_ON_RESUME:
			app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, resume");
		break;

		case EV_UI_FW_REDRAW:
			app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, redraw");
		break;

		case EV_UI_FW_ON_PAUSE:
			app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, pause");
		break;

		case EV_UI_FW_ON_EXIT:
#ifdef AP_CODEC_USE_BT_SRAM
			RDA_bt_Power_Off();
			//RDA_WRITE_REG(0xa1614000, 0x0);
			g_flash_write_req = 0;

			g_codec_buffer = NULL;
#else
			if (g_codec_buffer != NULL){
				COS_Free(g_codec_buffer);
				g_codec_buffer = NULL;
			}
#endif
			app_trace(APP_MUSIC_TRC, "UartPlayMainMsgHandler, exit");
		break;

		case AP_MSG_STANDBY:
		break;

#if defined(__AT_MOD_COMMON__)
		case AT_COMMON_VOLUME:
			g_comval->Volume = MIN(pEvent->nParam1, AUD_MAX_LEVEL);
			mediaSendCommand(MC_SETVOLUME, g_comval->Volume);
			break;
#endif

		default:
			//key event
			key = MESSAGE_ConvertToKeyValue(pEvent);

			switch (key) {
				case AP_KEY_VOLSUB | AP_KEY_DOWN:
				case AP_KEY_VOLSUB | AP_KEY_HOLD:
#if defined(__AT_MOD_COMMON__)
				case AT_COMMON_VOLSUB:
#endif
					g_comval->Volume--;
					if (g_comval->Volume < 0) {
						g_comval->Volume = 0;
					}

					mediaSendCommand(MC_SETVOLUME, g_comval->Volume);
				break;

				case AP_KEY_VOLADD | AP_KEY_DOWN:
				case AP_KEY_VOLADD | AP_KEY_HOLD:
#if defined(__AT_MOD_COMMON__)
				case AT_COMMON_VOLADD:
#endif
					g_comval->Volume++;
					if (g_comval->Volume > AUD_MAX_LEVEL) {
						g_comval->Volume = AUD_MAX_LEVEL;
					}
					mediaSendCommand(MC_SETVOLUME, g_comval->Volume);
				break;

				default:
					ret = FALSE;
				break;
			}
		break;
	}

	return ret;
}


/******************************************************************************
 * UartRecMainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL UartRecMainMsgHandler(COS_EVENT *pEvent)
{
	BOOL ret = TRUE;

	APP_ASSERT(pEvent);

	switch (pEvent->nEventId){
		case EV_UI_FW_ON_START:
		break;

		case EV_UI_FW_ON_RESUME:
		break;

		case EV_UI_FW_REDRAW:
		break;

		case EV_UI_FW_ON_PAUSE:
		break;

		case EV_UI_FW_ON_EXIT:
		break;

		case AP_MSG_STANDBY:
		break;

		default:
			ret = FALSE;
		break;
	}

	return ret;
}


INT32 UartPlayEntry(INT32 param)
{
	app_trace(APP_MUSIC_TRC, "====UartPlayEntry====");

	EntryNewScreen(SCR_ID_UART_PLAY_MAIN,UartPlayMainMsgHandler,NULL, 0, 0);
	return 0;
}


INT32 UartRecEntry(INT32 param)
{
	app_trace(APP_MUSIC_TRC, "====UartRecEntry====");

	EntryNewScreen(SCR_ID_UART_REC_MAIN,UartRecMainMsgHandler,NULL, 0, 0);
	return 0;
}

