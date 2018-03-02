/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
//TM WIN32
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "cswtype.h"
#include "tm.h"
#include "ap_common.h" 
#include "ap_message.h"

static TM_SYSTEMTIME g_alarm_time;
static UINT8 alarm_enable;

BOOL SRVAPI TM_SystemTimeToFileTime(
    CONST TM_SYSTEMTIME * pSystemTime,
    TM_FILETIME * pFileTime
)
{
   return TRUE;
}

BOOL SRVAPI TM_FileTimeToSystemTime (
    CONST TM_FILETIME nFileTime,
    TM_SYSTEMTIME * pSystemTime
)
{
   return TRUE;
}

BOOL SRVAPI TM_FormatSystemTime(
    CONST TM_SYSTEMTIME * pSystemTime,
    UINT16 uFormat,
    RESID nResFormatID,
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString,
    UINT8 uTimeStringSize
)
{
   return TRUE;
}

BOOL SRVAPI TM_FormatFileTime(
    CONST TM_FILETIME nFileTime,
    UINT16 uFormat, 
    RESID nResFormatID, 
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString, 
    UINT8 uTimeStringSize
)
{
   return TRUE;
}

UINT32 SRVAPI TM_GetTime (
    VOID
)
{
   return 0;
}

BOOL SRVAPI TM_SetSystemTime(
    TM_SYSTEMTIME * pSystemTime
)
{
   return TRUE;
}

BOOL SRVAPI TM_GetSystemTime(
    TM_SYSTEMTIME * pSystemTime
)
{
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ÐÇÆÚ%1d\n"
		,sys.wYear,sys.wMonth,sys.wDay
		,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds
		,sys.wDayOfWeek); 
	if(pSystemTime)
	{
        pSystemTime->uYear         = sys.wYear;
        pSystemTime->uMonth        = (UINT8)sys.wMonth;
        pSystemTime->uDayOfWeek    = (UINT8)sys.wDayOfWeek;
        pSystemTime->uDay          = (UINT8)sys.wDay;
        pSystemTime->uHour         = (UINT8)sys.wHour;
        pSystemTime->uMinute       = (UINT8)sys.wMinute;
        pSystemTime->uSecond       = (UINT8)sys.wSecond;
	
         // No information on the milliseconds is provided. Always set to 0.
        pSystemTime->uMilliseconds = sys.wMilliseconds;
        return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SRVAPI TM_SetTimeFormat(
  UINT16 nTimeFormat	  //The time format parameter to be set.
)
{
   return TRUE;
}

UINT16  SRVAPI TM_GetTimeFormat(void)
{
   return 0;
}

INT8 SRVAPI TM_GetTimeZone(
    VOID
)
{
   return 0;
}

BOOL SRVAPI TM_SetTimeZone(
    INT8 TimeZone
)
{
   return TRUE;
}

BOOL SRVAPI  TM_GetSystemFileTime(
    TM_FILETIME *pFileTime
)
{
	return TRUE;
}

BOOL SRVAPI TM_FormatDateTime(
    TM_FILETIME *pFileTime, 
    UINT16 uFormat, 
    RESID nResFormatID,
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString,
    UINT8 uTimeStringSize
){return TRUE;}

BOOL SRVAPI TM_SetLocalTime(
                           TM_SYSTEMTIME* pSystemTime
                           ){return TRUE;}
BOOL SRVAPI TM_GetLocalTime (
                            TM_SYSTEMTIME* pSystemTime
                            ){return TRUE;}
BOOL SRVAPI TM_FormatDateTimeEx(
    TM_FILETIME *pFileTime,
    PSTR pString
){return TRUE;}

BOOL SRVAPI TM_FormatFileTimeToStringEx(
    TM_FILETIME FileTime,
    UINT8 String[18]
){return TRUE;}


UINT8 SRVAPI TM_SmsTimeStampToFileTime(
    TM_SMS_TIME_STAMP ts,
    TM_FILETIME* pFileTime
){return 0;}

BOOL SRVAPI TM_DisableAlarm (void)
{
    alarm_enable = FALSE;
    return TRUE;
}


BOOL SRVAPI TM_SetAlarm (
    TM_SYSTEMTIME* pAlarmTime)
{
    g_alarm_time = *pAlarmTime;
    alarm_enable = TRUE;
    return TRUE;
}


void TM_CheckAlarm()
{
    if(alarm_enable)
    {
    	SYSTEMTIME sys;
    	GetLocalTime( &sys );
        if(sys.wYear == g_alarm_time.uYear && sys.wMonth ==g_alarm_time.uMonth && sys.wDay == g_alarm_time.uDay
            && sys.wHour == g_alarm_time.uHour && sys.wMinute == g_alarm_time.uMinute)
        {
            MESSAGE_SetEvent(AP_MSG_ALARM);
            alarm_enable = FALSE;
        }
    }
}

void SRVAPI TM_GetLastErr(UINT32* pErrorCode){}
 UINT32 csw_TMGetTick(VOID)
{
  return GetTickCount();
}
 



