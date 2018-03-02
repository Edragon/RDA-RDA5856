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


/******************************************************************************/
/* FILE NAME                                                                  */
/*      pm_win32.c                                                            */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      PM service declarations, constant definitions and macros.             */
/*                                                                            */
/******************************************************************************/
#include "cswtype.h"

#include "pm.h"


UINT8  g_sim_pm_Charging = FALSE;
UINT8  g_sim_pm_Battery = 100;
UINT8  g_sim_pm_Line_In = FALSE;

BOOL SRVAPI PM_GetACPowerStatus (
  UINT8* pACStatus
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetBatteryState(
  PM_BATTERY_INFO* pBatInfo
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetBatteryInfo (
  UINT8* pBcs,
  UINT8* pBcl,
  UINT16* pMpc
)
{
  *pBcs =  g_sim_pm_Charging;
  *pBcl = g_sim_pm_Battery;
  return  TRUE;
}


BOOL SRVAPI PM_BatteryChemistry (
  UINT8* pChemistryStatus
)
{
  return FALSE;
}

BOOL SRVAPI PM_SetBatteryChargeMode (
  UINT8 nMode
)
{
  return FALSE;
}

BOOL SRVAPI PM_SetPhoneFunctionality(
  UINT16 nMode,
  UINT16 nValue
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetPhoneFunctionality(
  UINT16* pMode
)
{
  return FALSE;
}

UINT16 PM_GetGpadcValue(PM_GPADC_CHAN_T channel)
{
  return 0;
}

PUBLIC PMD_CHARGER_STATUS_T pmd_GetChargerStatus(VOID)
{
    return PMD_CHARGER_FULL_CHARGE;
}