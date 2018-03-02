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



#ifndef __PM_H__
#define __PM_H__

BOOL SRVAPI PM_GetACPowerStatus (
  UINT8* pACStatus
);

#define PM_BATTERY_STATE_NORMAL      0
#define PM_BATTERY_STATE_LOW         1
#define PM_BATTERY_STATE_CRITICAL    2
#define PM_BATTERY_STATE_SHUTDOWN    3
#define PM_BATTERY_STATE_UNKNOWN     4

#define PM_CHARGER_DISCONNECTED      0
#define PM_CHARGER_CONNECTED         1
#define PM_CHARGER_CHRGING           2
#define PM_CHARGER_FINISHED          3
#define PM_CHARGER_ERROR_TEMPERATURE 4
#define PM_CHARGER_ERROR_VOLTAGE     5
#define PM_CHARGER_ERROR_UNKNOWN     9

//get ADC information,Gp_adc_In0 has been ocupied by battery voltage
typedef struct _PM_GPADC_INFO {
 UINT16 Gpadc_In1;
 UINT16 Gpadc_In2;
 UINT16 Gpadc_In3;
 UINT8 pading[2];
} PM_GPADC_INFO;

typedef struct _PM_BATTERY_INFO {
  UINT8  nBatteryFlag;
  UINT8  nBatteryLifePercent;
  UINT16 nBatteryRealTimeVoltage;
  UINT16 nBatteryVoltage;
  UINT16 nBatteryCurrent;
  UINT16 nBatteryTemperature;
  UINT8  padding[2];
  UINT32 nBatteryLifeTime;
  UINT32 nBatteryFullLifeTime;
} PM_BATTERY_INFO;

BOOL SRVAPI PM_GetBatteryState(
  PM_BATTERY_INFO* pBatInfo
);

void PM_SetBattery(UINT8 value);

BOOL SRVAPI PM_GetBatteryInfo (
  UINT8* pBcs,
  UINT8* pBcl,
  UINT16* pMpc
);

#define PM_BATTERY_CHEMISTRY_ALKALINE   0
#define PM_BATTERY_CHEMISTRY_NICD       1
#define PM_BATTERY_CHEMISTRY_HIMH       2
#define PM_BATTERY_CHEMISTRY_LION       3
#define PM_BATTERY_CHEMISTRY_LIPOLY     4
#define PM_BATTERY_CHEMISTRY_UNKNOWN    5

BOOL SRVAPI PM_BatteryChemistry (
  UINT8* pChemistryStatus
);

#define PM_BATTERY_IND_ENABLE    0
#define PM_BATTERY_IND_DISABLE   1

VOID SRVAPI PM_StartEarPieceGpadcMonitor(BOOL start);
BOOL SRVAPI PM_GetEarPieceGpadcMonitorStatus(VOID);
VOID SRVAPI PM_DisableEarPieceGpadcMonitor(BOOL disable);

#define PM_BATTER_FLAG_GENERAL     0
#define PM_BATTER_FLAG_BACKUP      1

#define PM_SLEEP_MODE              0x01
#define PM_FULL_MODE               0x02
#define PM_DISABLE_T_RF_MODE       0x04
#define PM_DISABLE_R_RF_MODE       0x08
#define PM_DISABLE_TR_RF_MODE      0x10
#define PM_RST_FULL_MODE           0x20
#define PM_WATCHDOG_MODE           0x40

BOOL SRVAPI PM_SetPhoneFunctionality(
  UINT16 nMode,
  UINT16 nValue
);

BOOL SRVAPI PM_GetPhoneFunctionality(
  UINT16* pMode
);

typedef enum
{
	PM_GPADC_CHAN_0,
	PM_GPADC_CHAN_1,
	PM_GPADC_CHAN_2,
	PM_GPADC_CHAN_3,
	
  PM_GPADC_CHAN_QTY
} PM_GPADC_CHAN_T;


// just for the factory mode now
// note: the battery volt is double of the return value.

UINT16 PM_GetGpadcValue(PM_GPADC_CHAN_T channel);


#endif // _H_

