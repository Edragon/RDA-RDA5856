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
#include "ap_gui.h"
#include "dm.h"
#include "hal_debug.h"

#if APP_SUPPORT_CHARGING==1 
#include "ap_charging.h"
#include "device.h"
#include "gpio_edrv.h"

/******************************************************************************
 * APPChargingMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL APPChargingMsgHandler(COS_EVENT *pEvent)
{
    UINT32 battery = GetBattery();
    
    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            break;

        case EV_UI_FW_REDRAW:
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
            if(AP_Support_LCD())
            {
                GUI_ClearScreen(NULL);
                if(battery == 100)
                    GUI_DisplayMessage(0, GUI_STR_FULL_CHARGING, 0, GUI_MSG_FLAG_DISPLAY);
                else
                    GUI_DisplayMessage(0, GUI_STR_CHARGING, 0, GUI_MSG_FLAG_DISPLAY);

                GUI_DisplayBattaryLever();
                GUI_UpdateScreen(NULL);
            }
#elif APP_SUPPORT_LED8S==1
            {
            }
#elif APP_SUPPORT_LED==1
            if(battery == 100)
            {
                //LED_SetPattern(GUI_LED_POWERON, 1);
                gpio_SetLED(0, TRUE);
            }
            else
            {
                LED_SetPattern(GUI_LED_NONE, 1);
            }
#endif
            break;


        case AP_MSG_WAIT_TIMEOUT:
            APP_Sleep();
            break;

        case EV_UI_FW_ON_EXIT:
            break;
/*
        case AP_KEY_PRESS:
            if(KEY_POWER == pEvent->nParam1)
            {
                SendAppEvent(EV_DM_POWER_ON_IND, DM_POWRN_ON_CAUSE_KEY);
            }
            break;
*/
        case EV_PM_BC_IND:
            app_trace(0, "EV_PM_BC_IND bcl=%d,bcs=%d", LOUINT16(pEvent->nParam1), HIUINT16(pEvent->nParam1));
#if APP_SUPPORT_USBDEVICE==1
            if(GetUsbCableStatus())
            {
                SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_UDISK);
            }
            else
#endif
                /*Charge plug out*/
                if(!IsChargePlugIn())
                {
                    app_trace(APP_MAIN_TRC, "Charge plug out, shutdown");
                    LED_SetPattern(GUI_LED_NONE, 1);
                    DM_DeviceSwithOff();
                }
                else
                {
                    SendAppEvent(EV_UI_FW_REDRAW, 0);
                }
            APP_Sleep();
            break;


        default:
            /*Block all messages in charge mode
            
			  undefine APP_SUPPORT_CHARGING to support background charge
			*/
            return TRUE;
    }

    return TRUE;
}

/*********************************************************************************
* Front charging scence  , You can undefine APP_SUPPORT_CHARGING to support background charge
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
INT32 Charging_Entry(void)
{
    app_trace(APP_MAIN_TRC, "====Charging_Entry====");

    EntryNewScreen(SCR_ID_CHARGING, APPChargingMsgHandler, NULL, 0, 0);

    return 0;
}

#endif
