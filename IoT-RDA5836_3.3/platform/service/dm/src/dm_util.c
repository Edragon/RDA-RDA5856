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

#include <cswtype.h>
#include <base.h>
#include "chip_id.h"
#include "cfw.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_key.h"
#include "hal_gpio.h"
#include "hal_host.h"

#include "csw_csp.h"
#include <cos.h>
#include <dm.h>
#include <base_prv.h>
#include <stdkey.h>
#include <tm.h>
#include <drv.h>
#include <ts.h>
#include <sul.h>
#include "tgt_pmd_cfg.h"

#include "pmd_m.h"
#include "pmd_config.h"
#include "tgt_app_cfg.h"

extern void lcd_present_CS_log(void);
#if defined(COMPRESS_3264)||defined(COMPRESS_6464) 
extern void lcd_log_and_Animation(void);
#endif
#if defined(COMPRESS_3232)
extern void lcd_present_cs_log_ani(void);
#endif

#define PWR_KEY_DEBOUNCE    3

/*wangrui add   macro  2008.12.17   for fixbug10731 */	
//#define CHARGERING_CANNOT_POWEROFF 1

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
extern INT32 VDS_CacheFlushAll(VOID);
#endif
#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
extern  BOOL tgt_FlushFactorySettings(VOID);
#endif

PRIVATE UINT8	PD_Trig     = FALSE;
UINT32  PowerOn_Cause = DM_POWRN_ON_CAUSE_UNKOWN;
//
// Soft Reset system.
//
BOOL  DM_Reset(VOID)
{

   hal_HstSendEvent(SYS_EVENT,0xaabb1212);
    #if 0
    #define REG_SYS_GLOBAL_BASE	0x00e00000 
    volatile UINT32 *reg = ( *(volatile UINT32*)((REG_SYS_GLOBAL_BASE+0x04) | (0xa0000000)) );
    *reg |= 1<<30;
    #endif
    pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
   
    hal_SoftReset();
    while(1)
       ; // only dump loop here.
    return TRUE;
}

extern BOOL TM_PowerOff(void);
/*wangrui add    2008.12.17 begin,  for fixbug10731*/	
//extern UINT32 CFW_CfgSetSoftReset(UINT8 cause);
//extern UINT32 CFW_CfgGetSoftReset(UINT8 *cause);
/*wangrui add     2008.12.17 end*/	
//
// Power Off.
//

// 1 mean charging power on than plug out adapter
// other reserved
UINT8 g_powerdown_cause = 0;
VOID DM_SetPowerDown_Cause(UINT8 cause)
{
    g_powerdown_cause = cause;
}
BOOL DM_DeviceSwithOff (VOID)
{
#ifdef CHARGERING_CANNOT_POWEROFF
    UINT8 resetCause = 2;
#endif /* CHARGERING_CANNOT_POWEROFF */

    TM_PowerOff();
    pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
#ifdef CHARGERING_CANNOT_POWEROFF
    switch(pmd_GetChargerStatus())
    {
    case PMD_CHARGER_UNKNOWN           :
    case PMD_CHARGER_UNPLUGGED         :
        resetCause = 2;
        break;
    case PMD_CHARGER_PLUGGED           :
    case PMD_CHARGER_PRECHARGE         :
    case PMD_CHARGER_FAST_CHARGE       :
    case PMD_CHARGER_PULSED_CHARGE     :
    case PMD_CHARGER_PULSED_CHARGE_WAIT:
    case PMD_CHARGER_FULL_CHARGE       :
    case PMD_CHARGER_ERROR             :
    case PMD_CHARGER_TIMED_OUT         :
    case PMD_CHARGER_TEMPERATURE_ERROR :
    case PMD_CHARGER_VOLTAGE_ERROR     :
    default                            :
        resetCause = 1;
        break;
    }
#endif

#if 0
    if (g_powerdown_cause == 1)
    {
        CFW_CfgSetSoftReset(3);
    }
    else
    {
        CFW_CfgSetSoftReset(resetCause);
    }
    g_powerdown_cause = 0;

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    VDS_CacheFlushAll(); 
#endif

#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    tgt_FlushFactorySettings();
#endif
#endif

#ifdef CHARGERING_CANNOT_POWEROFF
    if (resetCause == 1)
    {
        DM_Reset();
    }
    else
#endif
    {
        hal_ShutDown();
    }

    return TRUE;
}

//////////////////////////disable watchdog///////
BOOL dm_DisableWatchDog()
{
	hal_timWatchDog_Disable();
	return TRUE;
}
BOOL dm_SetWathcDogTime(UINT32 time)
{
	hal_timWatchDog_Start(time);
	
	return TRUE;
}
BOOL dm_SetWathcDog_KeepAlive()
{
	hal_timWatchDog_KeepAlive();
	return TRUE;

}
extern UINT32 dm_AlarmDisposeInEvt( UINT8 nCause, COS_EVENT* pEvent) ;
extern BOOL DM_KeyboardInit();
extern BOOL PM_PowerOnChck(void);
extern BOOL DRV_RegSetDbgVal(UINT16 nVal);
extern VOID DM_CheckPowerOnCause(VOID);
#if defined(_USED_TSC_KEY_)
extern BOOL tsd_KeyOnOffStateAtPowerOn(VOID);
extern BOOL tsd_IsKeyPowerOn(VOID);
#endif /* _USED_TSC_KEY_ */


UINT32 CFW_ShellControl(UINT8 nCommand);

BOOL dm_PowerOnCheck(UINT16* pCause)
{
  COS_EVENT ev;
  BOOL bRet;
  UINT8 POSSIBLY_UNUSED Causeflag;

  bRet = TRUE;
  SUL_ZeroMemory32( (VOID *)&ev, SIZEOF(COS_EVENT) );

  DM_CheckPowerOnCause();

#ifdef CHARGERING_CANNOT_POWEROFF
  CFW_CfgGetSoftReset(&Causeflag);
  if (Causeflag > 0)
  {
      CFW_CfgSetSoftReset(0);
  }
#endif

#ifdef __221_UNIT_TEST__
  CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON);  
#endif

  ev.nEventId = EV_DM_POWER_ON_IND;
  ev.nParam1 = PowerOn_Cause;

  if(PowerOn_Cause&DM_POWRN_ON_CAUSE_ALARM)
  {
      #if HAVE_TM_MODULE==1
      //dm_AlarmDisposeInEvt(PowerOn_Cause, &ev);
      #endif
#if 0  // for delete nParam3
      TM_ALARM* p = NULL;
      p = (TM_ALARM*)ev.nParam3;
      if( p && (p->nIndex == 17) )
      {
          ;//
      }
#endif
  }
  if(PowerOn_Cause&DM_POWRN_ON_CAUSE_CHARGE)
  {
      ;//DM_KeyboardInit() ;
  }
  if(pCause)
    *pCause = PowerOn_Cause;
  dm_SendPowerOnMessage(&ev);
      
  return TRUE; 
}

VOID DM_CheckPowerOnCause(VOID)
{
#ifdef PMD_TWO_BATTARY
    UINT32 i = 0;
    UINT32 keytime = 0;
#endif
    UINT8 POSSIBLY_UNUSED Causeflag;
    CONST PMD_CONFIG_T* pmdCfg = tgt_GetPmdConfig();
    HAL_SYS_RESET_CAUSE_T resetCause;
    resetCause = hal_SysGetResetCause(); 
    // If the system is restarted, check whether it just went through a
    // shutdown process. If yes, this should be a normal boot.
    if (resetCause == HAL_SYS_RESET_CAUSE_RESTART &&
        hal_SysGetShutdownFlag())
    {
        resetCause = HAL_SYS_RESET_CAUSE_NORMAL;
    }
    hal_HstSendEvent(SYS_EVENT,0xFFFFFF00|resetCause);
#if defined(APP_SUPPORT_HOLD_PREV_FOR_DOWNLOAD) && (APP_SUPPORT_HOLD_PREV_FOR_DOWNLOAD == 1)
    if(tsd_KeyIsEnterDownloadMode())
    {
          hal_HstSendEvent(SYS_EVENT,0X88118810);
          hal_GpioSetPinMux(HAL_GPIO_0_1,HAL_GPIO_ALTFUNC_CFG_9);
          hal_GpioSetPinMux(HAL_GPIO_0_2,HAL_GPIO_ALTFUNC_CFG_9);
          hal_TimDelay(8000 MS_WAITING);
	   hal_HstSendEvent(SYS_EVENT,0X88118811);
	   hal_ShutDown();
	   //resetCause = HAL_SYS_RESET_CAUSE_NORMAL;
	   //return;
    }
    hal_BoardConfigP01P02();
#endif
    switch (resetCause)
    {
        case HAL_SYS_RESET_CAUSE_NORMAL:
#if defined(_USED_TSC_KEY_)
            if(!tsd_KeyOnOffStateAtPowerOn())
            {
                hal_ShutDown();
            }
#endif /* _USED_TSC_KEY_ */

#ifndef PMD_TWO_BATTARY
            if(pmdCfg->holdPowerOn)
            {
                hal_TimDelay(16384);//delay for 1s
            }

#if defined(_USED_TSC_KEY_)
			if(tsd_IsKeyPowerOn())
#else
            if (hal_KeyOnOffStateAtPowerOn())
#endif /* _USED_TSC_KEY_ */
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
#if defined(COMPRESS_3264)||defined(COMPRESS_6464) 
                lcd_log_and_Animation();
#endif
            }
            else
            {
                switch(pmd_GetChargerStatus())
                {
                case PMD_CHARGER_UNKNOWN           :
                case PMD_CHARGER_UNPLUGGED         :
                    if(pmdCfg->holdPowerOn)
                        hal_ShutDown();
                    else
                    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) // 5856 
                        PowerOn_Cause = DM_POWRN_ON_CAUSE_AUTOPWRON;
#else
                        PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
#endif
                    }
                    break;
                case PMD_CHARGER_PLUGGED           :
                case PMD_CHARGER_PRECHARGE         :
                case PMD_CHARGER_FAST_CHARGE       :
                case PMD_CHARGER_PULSED_CHARGE     :
                case PMD_CHARGER_PULSED_CHARGE_WAIT:
                case PMD_CHARGER_FULL_CHARGE       :
                case PMD_CHARGER_ERROR             :
                case PMD_CHARGER_TIMED_OUT         :
                case PMD_CHARGER_TEMPERATURE_ERROR :
                case PMD_CHARGER_VOLTAGE_ERROR     :
                default                            :
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
#if defined(COMPRESS_3264)||defined(COMPRESS_6464)
                    lcd_log_and_Animation();
#endif
                    break;
                }
            }

#else // PMD_TWO_BATTARY

            for ( i = 0;i<2000;i++)
            {
               if(i == 1999)
               {
                   hal_ShutDown();
               }
                if (hal_KeyOnOffStateAtPowerOn())
                {
                   if (i == 0)
                   {
                       PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
                       i = 2000;
                   }
                   else
                   {
                       keytime ++;
                   }
                   if (keytime > 400)
                   {
                       PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
                       i = 2000;
                   }
    
                }
                else
                {
                    keytime = 0;
                    switch(pmd_GetChargerStatus())
                    {
                    case PMD_CHARGER_UNKNOWN           :
                    case PMD_CHARGER_UNPLUGGED         :
                        break;
                    case PMD_CHARGER_PLUGGED           :
                    case PMD_CHARGER_PRECHARGE         :
                    case PMD_CHARGER_FAST_CHARGE       :
                    case PMD_CHARGER_PULSED_CHARGE     :
                    case PMD_CHARGER_PULSED_CHARGE_WAIT:
                    case PMD_CHARGER_FULL_CHARGE       :
                    case PMD_CHARGER_ERROR             :
                    case PMD_CHARGER_TIMED_OUT         :
                    case PMD_CHARGER_TEMPERATURE_ERROR :
                    case PMD_CHARGER_VOLTAGE_ERROR     :
                    default                            :
                        PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
                        i = 2000;
                        break;
                    }
                }
                hal_TimDelay(5 MS_WAITING);
            }

#endif // PMD_TWO_BATTARY

            break;

        case HAL_SYS_RESET_CAUSE_ALARM:
            PowerOn_Cause = DM_POWRN_ON_CAUSE_ALARM;
            break;

        case HAL_SYS_RESET_CAUSE_RESTART:
        case HAL_SYS_RESET_CAUSE_HOST_DEBUG:

       	/*wangrui add    2008.12.17 begin,  for fixbug10731*/	
#ifdef CHARGERING_CANNOT_POWEROFF
            CFW_CfgGetSoftReset(&Causeflag);
            if((Causeflag == 1) &&
               (pmd_GetChargerStatus() != PMD_CHARGER_UNPLUGGED))
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
            }
            else
                PowerOn_Cause = DM_POWRN_ON_CAUSE_RESET;
            if(Causeflag == 2)
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
            }
            //if((Causeflag == 3)&&(pmd_GetChargerStatus() != PMD_CHARGER_UNPLUGGED))
            if( Causeflag == 3)
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
            }
#endif
/*wangrui add    2008.12.17  end*/	
            switch(pmd_GetChargerStatus())
            {
                case PMD_CHARGER_PLUGGED           :
                case PMD_CHARGER_PRECHARGE         :
                case PMD_CHARGER_FAST_CHARGE       :
                case PMD_CHARGER_PULSED_CHARGE     :
                case PMD_CHARGER_PULSED_CHARGE_WAIT:
                case PMD_CHARGER_FULL_CHARGE       :
                case PMD_CHARGER_ERROR             :
                case PMD_CHARGER_TIMED_OUT         :
                case PMD_CHARGER_TEMPERATURE_ERROR :
                case PMD_CHARGER_VOLTAGE_ERROR     :
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
#if defined(COMPRESS_3264)||defined(COMPRESS_6464)
                    lcd_log_and_Animation();
#endif
                    break;
                case PMD_CHARGER_UNKNOWN           :
                case PMD_CHARGER_UNPLUGGED         :
                default                            :
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_RESET;
                    break;
            }
            
            break;

        case HAL_SYS_RESET_CAUSE_WATCHDOG:
        default:
            PowerOn_Cause = DM_POWRN_ON_CAUSE_EXCEPTION;
            break;
    }
}

//
// Power down initialization.
// set up the on-off key trigger.
//
BOOL dm_PowerdownInit(VOID)
{
	PD_Trig = FALSE;
#ifdef HAVE_TIME_TEST
  long_time = 0;
#endif 

  return TRUE;
}


UINT32 DRV_GetRtcClock(VOID)
{
    // TODO Implement
    return 0;
}

UINT32 DRV_SetRtcClock(UINT32 ulClock)
{
    // TODO Implement
    return 0;
}

BOOL DM_Delay(UINT32 nSec)
{
    hal_TimDelay(nSec * HAL_TICK1S);
    return TRUE;
}


// Very basic delay for non-critical timing
// This will wait n/256 sec +/- 1/256 sec, so
// for a delay of at _least_ m/256, use (m+1)

BOOL DM_Wait(UINT32 nTick)
{
  hal_TimDelay(nTick);
    return TRUE;
}
BOOL DM_SendDBGEvent()
{
  // hal_HST_SendEvent(0xc0);
  return TRUE;

}



#if 0
PRIVATE VOID DM_PowerdownHandler(UINT16 cause)
{
	// wait a bit to debounce (especially in the case of power on)
	UINT8 nLevel;
	COS_EVENT ev;
	
//	CSW_TRACE(100, "DM_PowerdownHandler!!!\n");
	//return;
	DRV_GpioGetLevel(ON_OFF_BUTTON,&nLevel );
	if (nLevel)
	{
//	hal_pwl1_Threshold(0);
	ev.nEventId =  EV_KEY_DOWN;
	ev.nParam1	= SK_POWER_OFF;
//	ev.nParam2	= 0;//for delete nParam2
//	ev.nParam3	= 0;//for delete nParam3
	dm_SendKeyMessage(&ev);
	}
	else
	{
//	hal_pwl1_Threshold(0xff);	

	ev.nEventId =  EV_KEY_UP;
	ev.nParam1	= SK_POWER_OFF;
//	ev.nParam2	= 0;//for delete nParam2
//	ev.nParam3	= 0;//for delete nParam3
	dm_SendKeyMessage(&ev);
	
	}
	#if 0
    #ifdef HAVE_TIME_TEST
	UINT32 Cur_Time = 0;
    #endif 
    
	COS_EVENT ev;

	if (PD_Trig == TRUE)
	{
        #ifdef HAVE_TIME_TEST
	    Cur_Time = hal_timRTC_GetTime()/256;
        #endif 
        
      //  hal_SetUsrVectorMsk(HAL_GPIO_IRQ, HAL_GPIO_RISE(ON_OFF_BUTTON)); 
        PD_Trig = FALSE; 
        
        #if 0 
        //TS_Print(" power down: %d, %d \n", Cur_Time, long_time);
        
	    if( Cur_Time > long_time + 2)
        {
            ev.nEventId = EV_KEY_PRESS; //EV_DM_POWER_OFF_NORMAL_IND;
            ev.nParam2  = 1; // long press 2 s
            //PD_Trig = FALSE;
        }
        else
        { 
            ev.nEventId = EV_KEY_UP; //EV_DM_POWER_OFF_NORMAL_IND;
            ev.nParam2  = 0; // normal press
        }
        #endif        

        #if 0 
        ev.nParam1  = SK_POWER_OFF;
//        ev.nParam3  = 0;//for delete nParam3
        dm_SendKeyMessage(&ev);
        #else
        ev.nEventId = EV_CSW_PRV_KEY;
        ev.nParam1  = MAKEUINT32(0, SK_POWER_OFF);
//        ev.nParam3  = 0;//for delete nParam3
        BAL_SendTaskEvent(&ev, CSW_TASK_ID_SYS);
        #endif 
	}
	else
	{        
        #ifdef HAVE_TIME_TEST
		Cur_Time = hal_timRTC_GetTime();        
		while (hal_timRTC_GetTime() < (Cur_Time + 1))
            ;
        #endif
        
        DM_Wait(1);

        #ifdef HAVE_TIME_TEST
        long_time = hal_timRTC_GetTime()/256;
        #endif 
        
		if (hal_gpio_GetVal(ON_OFF_BUTTON) != 0)
		{
			PD_Trig = TRUE;
		//	hal_SetUsrVectorMsk(HAL_GPIO_IRQ, HAL_GPIO_FALL(ON_OFF_BUTTON));            

            #if 0 
        	ev.nEventId = EV_KEY_DOWN;
            ev.nParam1  = SK_POWER_OFF;
//            ev.nParam2  = 0;//for delete nParam2
//            ev.nParam3  = 0;//for delete nParam3
            dm_SendKeyMessage(&ev);
            #else
        	ev.nEventId = EV_CSW_PRV_KEY;
            ev.nParam1  = MAKEUINT32(1, SK_POWER_OFF);
//            ev.nParam2  = 0;//for delete nParam2
//            ev.nParam3  = 0;//for delete nParam3
            BAL_SendTaskEvent(&ev, CSW_TASK_ID_SYS);
            #endif 
		} 
	}
	#endif
}
#endif



