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
#include "ap_calendar.h"
#include "tm.h"

#define  CLNDR_TITLE_LEN            90
#define  CLNDR_MAX_DAYS             31
#define  NUM_MONTHS                 12
#define  NUM_MIN_HR                 60
#define  NUM_HRS_DAY                24
#define  CLNDR_MAX_WEEK_NUM         54
#define  DT_DAYS_OF_WEEK             7

#define  CLNDR_MAX_DATE_NUM         8

#define CALENDAR_TODAY              AP_MSG_USER_CALENDAR+1
#define CALENDAR_SELECTDAY          AP_MSG_USER_CALENDAR+2
#define CALENDAR_LOADDATES          AP_MSG_USER_CALENDAR+3
#define CALENDAR_DATELIST           AP_MSG_USER_CALENDAR+4
#define CALENDAR_FESTIVAL           AP_MSG_USER_CALENDAR+5
#define CALENDAR_TERMLIST           AP_MSG_USER_CALENDAR+6

#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)

//ap使用的系统变量
typedef struct
{
    UINT8 date_count;
    struct {
        UINT8 month;
        UINT8 day;
        UINT8 name[18];
    }dates[CLNDR_MAX_DATE_NUM];
	UINT16 magic;
}calendar_vars_t;

typedef enum
{
	CALENDAR_DRAW_BACKGROUND = 0x1,
	CALENDAR_DRAW_TIME = 0x2,
	CALENDAR_DRAW_ALL = 0xf,
}CALENDAR_DRAW_ENUM;

/***************************************************************************************************************************************************************

首先要能记住十大天干和十二地支，
十天干：     甲、乙、丙、丁、戊、己、庚、辛、壬、癸；
十二地支：子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥；
 
天干地支纪年法首先是天干在前，地支在后，比如今年2005
就为－乙酉年，先来算算天干，
有个公式： 
4、 5、 6、 7、 8、 9、 0、 1、 2、 3 对应的十天干就是 
甲、乙、丙、丁、戊、己、庚、辛、壬、癸， 
数字为年代的最后的一位数字，比如今年是2005，最后一位是5，对应的天干就是乙； 

地支的算法：用年代数除以12，后面的余数就代表某个地支，
余数分别为：4、 5、 6、 7、 8、 9、 10、 11、 0（能整除）、1、 2、3.
代表地支为：子、丑、寅、卯、辰、巳、午、 未、 申、酉、戌、亥.
比如2005年为例：年代末尾数为5，对应的天干为乙，2005除以12，余数为1
，对应的地支为酉，所以2005年为乙酉年。

****************************************************************************************************************************************************************/
 

extern const INT8 month_days[];
extern BOOL ui_auto_update;

static UINT32 g_cur_date;
static BOOL  g_scroll_title;
static BOOL  g_is_today;
static UINT8 g_cur_week;
static UINT8 g_festival_count=0;
static UINT8 g_calendar_title[CLNDR_TITLE_LEN];
static date_t  g_solar_date, g_lunar_date;
static UINT8 g_lunar_flag; // bit0~bit4 - 当月天数， bit7 - 闰月

clndr_table_struct *g_pCalendarInfo;
calendar_vars_t *g_clndr_vars;
    
/* number of accumulated days per solar month, non-leap year and leap year. */
const INT16 gClndrAccuDays[]        = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396};

/*****************************************************************************
* FUNCTION
*	DOW
* DESCRIPTION  判断是否是闰年
*   Function to find out if given year is a leap year
* PARAMETERS
*	y	IN	year of the date to be computed	(example, 2004)
* RETURNS	
*	1 if the year is leap; otherwise 0.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 IsLeapYear(U16 year)
{
	if((year % 400) == 0)
		return 1;
	else if ((year % 100) == 0)
		return 0;
	else if((year % 4) == 0)
		return 1;
	else
		return 0;
}

/*****************************************************************************
* FUNCTION
*	LastDayOfMonth
* DESCRIPTION  根据年月计算给月的最后一天
*   Function to find the last day of specific month.
* PARAMETERS
*	month	IN	month
*	year	IN	year
* RETURNS	
*	number of days
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
UINT8 GetDaysOfMonth(UINT8 month,U16 year) 
{	
	if(month != 2) 
		return month_days[month-1];
	else
		return month_days[1] + IsLeapYear(year);
}

/*****************************************************************************
* FUNCTION                                                            
*	DOW
* DESCRIPTION  根据年月日计算一个一周的索引
*   Function to compute current day of week.
* PARAMETERS
*	y	IN	year of the date to be computed.	(example, 2004)
*	m	IN	month of the date to be computed
*	d	IN	day of the date to be computed
* RETURNS
*	UINT8	index of day of week
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
UINT8 CALENDAR_CmpIndexOfWeek(U16 y, UINT8 m, UINT8 d)
{
    if (m < 3)
    {
        m += 13;
        y--;
    }
    else 
  	{
		m++;
  	}
    
    return (d + 26 * m / 10 + y + y / 4 - y / 100 + y / 400 + 6) % 7;
}


/*****************************************************************************
 * FUNCTION
 *  CALENDAR_GetWeekNumber
 * DESCRIPTION
 *  Compute the week number of a date
 * PARAMETERS
 *  date        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static UINT8 CALENDAR_GetWeekNumber(date_t *date)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT32 i, no_of_days, week_num, numdays_inmonth, day_on_firstjan;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    no_of_days = date->day;

    for (i = date->month - 1; i > 0; i--)
    {
        /* get the num of days in todays month */
        numdays_inmonth = GetDaysOfMonth((UINT8) i, date->year );
        no_of_days += numdays_inmonth;
    }

    day_on_firstjan = CALENDAR_CmpIndexOfWeek(date->year, 1, 1);

    /* Total no of days/ weekdays */
    no_of_days += day_on_firstjan;

    week_num = (no_of_days) / DT_DAYS_OF_WEEK;
    
    if ((no_of_days) % DT_DAYS_OF_WEEK > 0)
    {
        week_num += 1;
    }

    APP_ASSERT(week_num <= CLNDR_MAX_WEEK_NUM);
    return week_num;

}



/*****************************************************************************
* FUNCTION                                                            
*	ClndrComputeSolarTerm
* DESCRIPTION                                                           
*   This function is to calculate the date of solar item in given year. 
* PARAMETERS
*	year 		IN		year in solar calendar, e.g. 2003
*	solar_index	IN		index of solar term, 0 = SC (Slight Cold), 1 = GC,...
* RETURNS
*	the solar day of given solar item
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
INT16 ClndrComputeSolarTerm(INT16 year, INT16 month, INT16 solar_index) 
{
//	return solar_term_table[year-CLNDR_FIRST_NAVI_YEAR][solar_index];
    return g_pCalendarInfo->data[year-g_pCalendarInfo->first_year].solar_term_table[solar_index];
}

/*****************************************************************************
 * FUNCTION
 *  ClndrComputeLunarInfo
 * DESCRIPTION
 *  This function is to convert solar date infomation to lunar date.
 * CALLS
 * PARAMETERS
 *  DI      [IN/OUT]        A struct contains solar date information and lunar information will also be stored here
 * RETURNS
 *  void
 *****************************************************************************/
void CALENDAR_ComputeLunarInfo(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 LeapDay;
    INT16 StartMonth;
    INT16 YearIndex;
    INT16 AccumulateDay;
    INT16 DateDiff;
    INT16 IntercalaryMonth;
    INT16 LunarDays;
    INT16 MonthIndex;
        
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* to check if the month out of boundary */
    StartMonth = g_solar_date.month - 1;
    
    /* to calculate the number of days in Feb. this year */
    if (StartMonth > 1)
    {
        LeapDay = (UINT8) IsLeapYear(g_solar_date.year);
    }
    else
    {
        LeapDay = 0;
    }

    YearIndex = g_solar_date.year - g_pCalendarInfo->first_year;

    /* to calculate weekday */
    AccumulateDay = (gClndrAccuDays[StartMonth] + LeapDay + g_solar_date.day);

    g_cur_week = (AccumulateDay+g_pCalendarInfo->data[YearIndex].BaseWeekday)%7;
    
    /* to calculate the Kan and Chin of today */
    //KanChiDay = AccumulateDay + g_pCalendarInfo->data[YearIndex].BaseKanChi;

    if (AccumulateDay <= g_pCalendarInfo->data[YearIndex].BaseDays)
    {
        YearIndex--;
        g_lunar_date.year = g_solar_date.year - 1;
        
        StartMonth += 12;
        LeapDay = (UINT8) IsLeapYear(g_lunar_date.year);
        AccumulateDay = (gClndrAccuDays[StartMonth] + LeapDay + g_solar_date.day);
    }
    else
    {
        g_lunar_date.year = g_solar_date.year;
    }

    /* to calculate the lunar month and day */
    IntercalaryMonth = g_pCalendarInfo->data[YearIndex].Intercalary;
    DateDiff         = g_pCalendarInfo->data[YearIndex].BaseDays;
    
    for (MonthIndex = 0; MonthIndex < 13; MonthIndex++)
    {
        if (g_pCalendarInfo->data[YearIndex].MonthDays & (0x01 << MonthIndex))
            g_lunar_flag = 30;
        else
            g_lunar_flag = 29;
            
        LunarDays = DateDiff + g_lunar_flag;

        if (AccumulateDay <= LunarDays)
        {
            break;
        }

        DateDiff = LunarDays;
    }

    g_lunar_date.month = (MonthIndex + 1);
    g_lunar_date.day   = AccumulateDay - DateDiff;

    /* to adjust Lunar month if there is a intercalary month in this year */
    if (IntercalaryMonth != 0 && g_lunar_date.month > IntercalaryMonth)
    {
        g_lunar_date.month--;
        if(IntercalaryMonth == g_lunar_date.month)
            g_lunar_flag |= 0x80;
    }

    if (g_lunar_date.month > 12)
    {
        g_lunar_date.month -= 12;
    }
}

void ClndrLunarToSolar(date_t *date)
{
    UINT16 index;
    UINT16 YearIndex = date->year-g_pCalendarInfo->first_year;
    INT16 day, month;
    
    for(index=YearIndex;;index--)
    {
        day = g_pCalendarInfo->data[index].BaseDays;
        
        for (month = 0; month < date->month-1; month++)
        {
            if (g_pCalendarInfo->data[index].MonthDays & (0x01 << month))
                day += 30;
            else
                day += 29;
        }
        if(g_pCalendarInfo->data[index].Intercalary!=0 && g_pCalendarInfo->data[index].Intercalary<date->month)
            if (g_pCalendarInfo->data[index].MonthDays & (0x01 << month))
                day += 30;
            else
                day += 29;

        day += date->day;
        for(month=1; month<=12; month++)
        {
            if(day <= GetDaysOfMonth(month, index+g_pCalendarInfo->first_year))
                break;
            else
                day -= GetDaysOfMonth(month, index+g_pCalendarInfo->first_year);
        }
        if(index!=YearIndex && month>12)
            month-=12;
        if(month<=12)
            break;
    }
    date->month = month;
    date->day = day;
}

UINT8 CALENDAR_GenerateSolarTerm(UINT8 offset)
{
    UINT8 LeapDay;
    INT16 LunarDays;
    INT16 SolarDate1, SolarDate2, SolarDate3;
    INT16 SolarTerm;
    INT16 SolarDate;
    INT8 *string;
        
    /* to calculate the lunar day of solar term in this month */
    SolarDate1 = ClndrComputeSolarTerm(g_solar_date.year, g_solar_date.month, (INT16) ((g_solar_date.month - 1) * 2));

    SolarDate2 = ClndrComputeSolarTerm(g_solar_date.year, g_solar_date.month, (INT16) ((g_solar_date.month - 1) * 2 + 1));

    if (g_solar_date.month < 12)
    {
        SolarDate3 = ClndrComputeSolarTerm(g_solar_date.year, (INT16) (g_solar_date.month + 1), (INT16) ((g_solar_date.month - 1) * 2 + 2));
    }
    else
    {
        SolarDate3 = ClndrComputeSolarTerm((INT16) (g_solar_date.year + 1), (INT16) 1, (INT16) (0));
    }

    if (g_solar_date.day < SolarDate1)
    {
        SolarTerm = 0;
        SolarDate = SolarDate1 - g_solar_date.day + g_lunar_date.day;
    }
    else if (g_solar_date.day == SolarDate1)
    {
        SolarTerm = 0;
        SolarDate = 0;
    }
    else if (g_solar_date.day < SolarDate2)
    {
        SolarTerm = 1;
        SolarDate = SolarDate2 - g_solar_date.day + g_lunar_date.day;
    }
    else if (g_solar_date.day == SolarDate2)
    {
        SolarTerm = 1;
        SolarDate = 0;
    }
    else
    {
        SolarTerm = 2;
        if (g_solar_date.month == 2)
        {
            if (g_solar_date.month + 1 == 1 || (g_solar_date.month + 1 == 2 && SolarDate3 < 29))
            {
                LeapDay = 0;
            }
            else
            {
                LeapDay = (UINT8) IsLeapYear(g_solar_date.year);
            }
            SolarDate = month_days[g_solar_date.month - 1] - g_solar_date.day + SolarDate3 + g_lunar_date.day + LeapDay;
        }
        else
        {
            SolarDate = month_days[g_solar_date.month - 1] - g_solar_date.day + SolarDate3 + g_lunar_date.day;
        }
    }

    if (g_pCalendarInfo->data[g_solar_date.year - g_pCalendarInfo->first_year].MonthDays & (0x01 << (g_lunar_date.month-1)))
    {
        LunarDays = 30;
    }
    else
    {
        LunarDays = 29;
    }

    if (SolarDate > LunarDays)
    {
        SolarDate -= LunarDays;
    }

    /* Solar Term */
    string = GUI_GetString(GUI_STR_TERMS);
    if (SolarDate == 0)
    {
        memcpy(g_calendar_title+offset, string + ((g_solar_date.month - 1) * 2 + SolarTerm)*4, 4);
        offset += 4;
        g_calendar_title[offset++] = ' ';
    }
    /*else
    {
        if (SolarTerm == 2 && g_solar_date.month == 12)
        {
            memcpy(g_calendar_title+offset, string, 4);
        }
        else
        {
            memcpy(g_calendar_title+offset, string + ((g_solar_date.month - 1) * 2 + SolarTerm)*4, 4);
        }
    }*/


    return offset;
}


UINT8 CALENDAR_GenerateLunarString(UINT8 offset)
{
    INT16 KanYear;
    INT16 ChiYear;
    INT8 *string;
    INT8 *lunar_str;
    INT8 *num_str;
        
    /* to calculate the Kan and Chin of this year */
    if (g_lunar_date.year == g_solar_date.year)
    {
        KanYear = (g_solar_date.year - 1900 + 36) % 10;
        ChiYear = (g_solar_date.year - 1900 + 36) % 12;

        /* to calculate the animal of this year */
        //DI->YearAnimal = (g_solar_date.year + 8) % 12;
    }
    else
    {
        KanYear = (g_solar_date.year - 1900 + 36 - 1) % 10;
        ChiYear = (g_solar_date.year - 1900 + 36 - 1) % 12;

        /* to calculate the animal of this year */
        //DI->YearAnimal = (g_solar_date.year + 8 - 1) % 12;
    }

    /* to recode the information to present */
    lunar_str = GUI_GetString(GUI_STR_LUNAR);
    num_str = GUI_GetString(GUI_STR_NUMBERS);
    
    /* Chinese year/month/day information */
    string = GUI_GetString(GUI_STR_KANS);
    memcpy(g_calendar_title, string+KanYear*2, 2);
    string = GUI_GetString(GUI_STR_CHIS);
    memcpy(g_calendar_title+2, string+ChiYear*2, 2);
    memcpy(g_calendar_title+4, lunar_str, 2);

    offset = 6;

    if(g_lunar_flag&0x80)
    {
        memcpy(g_calendar_title+offset, lunar_str+6, 2);
        offset+=2;
    }

    if(g_lunar_date.month == 1)
        memcpy(g_calendar_title+offset, lunar_str+8, 2);
    else if(g_lunar_date.month == 11)
        memcpy(g_calendar_title+offset, lunar_str+10, 2);
    else if(g_lunar_date.month == 12)
        memcpy(g_calendar_title+offset, lunar_str+12, 2);
    else
    {
        memcpy(g_calendar_title+offset, num_str+(g_lunar_date.month-1)*2, 2);
    }

    memcpy(g_calendar_title+offset+2, lunar_str+2, 2);
    offset += 4;

    if (g_lunar_date.day <=10)
    {
        memcpy(g_calendar_title+offset, num_str+20, 2);
    }
    else if (g_lunar_date.day < 20)
    {
        memcpy(g_calendar_title+offset, num_str+18, 2);
    }
    else if (g_lunar_date.day == 20)
    {
        memcpy(g_calendar_title+offset, num_str+2, 2);
    }
    else if (g_lunar_date.day < 30)
    {
        memcpy(g_calendar_title+offset, num_str+22, 2);
    }
    else // if (g_lunar_date.day == 30)
    {
        memcpy(g_calendar_title+offset, num_str+4, 2);
    }
    
    memcpy(g_calendar_title+offset+2, num_str+((g_lunar_date.day - 1)%10)*2, 2);
    offset += 4;

    g_calendar_title[offset++] = ' ';
    
    /* Animal index */
    //strcat(g_calendar_title, GUI_GetString((U16) (GUI_STR_MOUSE + DI->YearAnimal)));

    return offset;
}

INT8 ClndrGenerateFestivalDayString(UINT8 offset, UINT8 *name, UINT8 days)
{
    UINT8 *string = GUI_GetString(GUI_STR_FESTIVALDAY);
    UINT i,j;

    for(i=0;string[i];i++)
    {
        if(string[i] == '%')
        {
            i++;
            if(string[i] == 'd')
            {
                if(days>=10)
                    g_calendar_title[offset++] = '0'+days/10;
                g_calendar_title[offset++] = '0'+days%10;
            }
            else if(string[i] == 's')
            {
                for(j=0;name[j];j++)
                    g_calendar_title[offset++] = name[j];
            }
        }
        else
        {
            g_calendar_title[offset++] = string[i];
        }
    }
    return offset;
}

INT8 CALENDAR_GenerateFestivalString(UINT8 offset, INT8 predays)
{
    INT32 index;
    INT8 days;
    INT8 *string;
    INT8 first_days[4];
    INT8 first_index[4];
    Festival_t festival;

    for(index = 0; index<4; index++)
    {
        first_days[index] = 60;
        first_index[index] = -1;
    }
    
    // get Festival
    for(index=0;;index++)
    {
        if(index<g_clndr_vars->date_count)
        {
            festival.month = g_clndr_vars->dates[index].month;
            festival.day = g_clndr_vars->dates[index].day;
            festival.name = g_clndr_vars->dates[index].name;
            if(festival.month > 12)
            {
                festival.month -= 12;
                festival.style = FESTIVAL_TYPE_LUNAR;
            }
            else
                festival.style = FESTIVAL_TYPE_SOLAR;
        }
        else if(!gui_get_festival(index-g_clndr_vars->date_count, &festival))
            break;
        
        days = 60;
        if(festival.style == FESTIVAL_TYPE_SOLAR)
        {
            if(g_solar_date.month == festival.month && g_solar_date.day <= festival.day)
            {
                days = festival.day - g_solar_date.day;
            }
            else if(g_solar_date.month + 1 == festival.month || (g_solar_date.month ==12 && festival.month == 1))
            {
                days = GetDaysOfMonth(g_solar_date.month, g_solar_date.year) + festival.day - g_solar_date.day;
            }
        }
        if(festival.style == FESTIVAL_TYPE_LUNAR)
        {
            if(g_lunar_date.month == festival.month && g_lunar_date.day <= festival.day)
            {
                days = festival.day - g_lunar_date.day;
            }
            else if((g_lunar_date.month + 1 == festival.month && ((g_lunar_flag&0x80) || g_lunar_date.month != g_pCalendarInfo->data[g_lunar_date.year-g_pCalendarInfo->first_year].Intercalary)) 
                || (g_lunar_date.month ==12 && festival.month == 1))
            {
                days = (g_lunar_flag&0x1f) + festival.day - g_lunar_date.day;
            }
        }
        if(festival.style == FESTIVAL_TYPE_WEEK)
        {
            if(g_solar_date.month == festival.month || g_solar_date.month+1 == festival.month)
            {
                if(festival.week == 0) // last week
                {
                    days = CALENDAR_CmpIndexOfWeek(g_solar_date.year, festival.month, GetDaysOfMonth(festival.month, g_solar_date.year));
                    if(days >= festival.day)
                        days = GetDaysOfMonth(festival.month, g_solar_date.year) + festival.day - days;
                    else
                        days = GetDaysOfMonth(festival.month, g_solar_date.year) + festival.day - days - 7;
                    days = days - g_solar_date.day;
                }
                else
                {
                    days = CALENDAR_CmpIndexOfWeek(g_solar_date.year, festival.month, 1);
                    if(days <= festival.day)
                        days = festival.week * 7 + festival.day - days - 7;
                    else
                        days = festival.week * 7 + festival.day - days;
                    days = days - g_solar_date.day + 1;
                }
                if(g_solar_date.month+1 == festival.month)
                    days += GetDaysOfMonth(g_solar_date.month, g_solar_date.year);
            }
        }

        if(days==predays && predays == 0)
        {
            strcat(g_calendar_title+offset, festival.name);
            offset += strlen(festival.name);

            g_calendar_title[offset++] = ' ';
        }
        if(index<g_clndr_vars->date_count)
        {
            if(days>0 && days+1 < first_days[3])
            {
                first_days[3] = days;
                first_index[3] = index;
            }
        }
        else
        {
            if(days>0 && days+1 < first_days[festival.style])
            {
                first_days[festival.style] = days;
                first_index[festival.style] = index-g_clndr_vars->date_count;
            }
        }
    }
    g_festival_count = index - g_clndr_vars->date_count;
    
    // 除夕
    if(predays == 0 && (g_comval->langid == GUI_LANG_SM_CHINESE || g_comval->langid == GUI_LANG_TR_CHINESE)
        && (g_lunar_date.month==12 && g_lunar_date.day == (g_lunar_flag&0x1f)))
    {
        string = GUI_GetString(GUI_STR_CHUXI);
        strcat(g_calendar_title+offset, string);
        offset += strlen(string);

        g_calendar_title[offset++] = ' ';
    }

    // easter sunday
    if(g_solar_date.month == 3 || g_solar_date.month == 4)
    {
        if(g_solar_date.month == 3)
            days = g_solar_date.day;
        else
            days = g_solar_date.day+31;
        days = g_pCalendarInfo->data[g_solar_date.year-g_pCalendarInfo->first_year].EasterSunday-days;
        if(days == 0 && predays == 0)
        {
            string = GUI_GetString(GUI_STR_EASTER);
            strcat(g_calendar_title+offset, string);
            offset += strlen(string);

            g_calendar_title[offset++] = ' ';
        }
        else if(days >0 && days<predays && predays!=0)
        {
            offset = ClndrGenerateFestivalDayString(offset, GUI_GetString(GUI_STR_EASTER), days);
            g_calendar_title[offset++] = ' ';
        }
    }

    for(index = 3; index>=0; index--)
    {
        if(first_index[index]>=0 && first_days[index]!=0 && first_days[index]<=predays)
        {
            if(index==3)
                string = g_clndr_vars->dates[first_index[index]].name;
            else
            {
                gui_get_festival(first_index[index], &festival);
                string = festival.name;
            }
            offset = ClndrGenerateFestivalDayString(offset, string, first_days[index]);
            g_calendar_title[offset++] = ' ';
        }
    }
    
    g_calendar_title[offset] = 0;

    return offset;
}
INT32 date_list_callback(INT32 type, INT32 value, INT8 **string)
{
    UINT16 index;
    UINT8 month;
    UINT16 day;

    if(g_clndr_vars->dates[value].month <= 12)
    {
        month = g_clndr_vars->dates[value].month;
        day = g_clndr_vars->dates[value].day;
    }
    else
    {
        date_t date;
        date.year = g_solar_date.year;
        date.month = g_clndr_vars->dates[value].month-12;
        date.day = g_clndr_vars->dates[value].day;
        ClndrLunarToSolar(&date);
        month = date.month;
        day = date.day;
    }
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = g_calendar_title;
        sprintf(g_calendar_title, "%02d/%02d:%s", month,day,g_clndr_vars->dates[value].name);

        return 1;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        g_is_today = 0;
        g_solar_date.month = month;
        g_solar_date.day = day;
        return 0;
    }}


INT32 festival_list_callback(INT32 type, INT32 value, INT8 **string)
{
    Festival_t festival;
    UINT16 YearIndex;
    UINT16 index;
    UINT8 month;
    UINT16 day;
    UINT8 *name;
	
    YearIndex = g_solar_date.year-g_pCalendarInfo->first_year;
    if(value==g_festival_count)
    {// easter sunday
        month = 3;
        day = g_pCalendarInfo->data[YearIndex].EasterSunday;
        if(day>31)
        {
            day-=31;
            month++;
        }
        name = GUI_GetString(GUI_STR_EASTER);
    }
    else
    {
        gui_get_festival(g_festival_count-value-1, &festival);
        name = festival.name;
        
        if(festival.style == FESTIVAL_TYPE_SOLAR)
        {
            month = festival.month;
            day = festival.day;
        }
        else if(festival.style == FESTIVAL_TYPE_LUNAR)
        {
            date_t date;
            date.year = g_solar_date.year;
            date.month = festival.month;
            date.day = festival.day;
            ClndrLunarToSolar(&date);
            month = date.month;
            day = date.day;
        }
        else if(festival.style == FESTIVAL_TYPE_WEEK)
        {
            if(festival.week == 0) // last week
            {
                day = CALENDAR_CmpIndexOfWeek(g_solar_date.year, festival.month, GetDaysOfMonth(festival.month, g_solar_date.year));
                if(day >= festival.day)
                    day = GetDaysOfMonth(festival.month, g_solar_date.year) + festival.day - day;
                else
                    day = GetDaysOfMonth(festival.month, g_solar_date.year) + festival.day - day - 7;
            }
            else
            {
                day = CALENDAR_CmpIndexOfWeek(g_solar_date.year, festival.month, 1);
                if(day <= festival.day)
                    day = festival.week * 7 + festival.day - day - 7;
                else
                    day = festival.week * 7 + festival.day - day;
                day = day + 1;
            }
            month = festival.month;
        }
    }
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        *string = g_calendar_title;
        sprintf(g_calendar_title, "%02d/%02d:%s", month,day,name);

        return 1;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        g_is_today = 0;
        g_solar_date.month = month;
        g_solar_date.day = day;
        return 0;
    }
}

INT32 solarterm_list_callback(INT32 type, INT32 value, INT8 **string)
{
    INT8 *str;
    
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        Festival_t festival;

        *string = g_calendar_title;
	    memset(g_calendar_title, 0, 20);
        sprintf(g_calendar_title, "%02d/%02d:", (value/2)+1,g_pCalendarInfo->data[g_solar_date.year-g_pCalendarInfo->first_year].solar_term_table[value]);
        str = GUI_GetString(GUI_STR_TERMS);
        memcpy(g_calendar_title+strlen(g_calendar_title), str + value*4, 4);
        return 1;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        g_is_today = 0;
        g_solar_date.month = value/2+1;
        g_solar_date.day = g_pCalendarInfo->data[g_solar_date.year - g_pCalendarInfo->first_year].solar_term_table[value];
        return 0;
    }
}

void display_date(void)
{
	INT16 LeapDay = 0;
	INT16 StartMonth = 0;
	INT16 YearIndex = 0;
	INT16 AccumulateDay = 0;
	UINT8 i = 0, j = 0;
	UINT8 days = 0;
	UINT8 week = 0;
	char str;
	region_t region = {0};
	
	StartMonth = g_solar_date.month - 1;
    
    if (StartMonth > 1)
    {
        LeapDay = (UINT8) IsLeapYear(g_solar_date.year);
    }
    else
    {
        LeapDay = 0;
    }

    YearIndex = g_solar_date.year - g_pCalendarInfo->first_year;
	days = GetDaysOfMonth(g_solar_date.month, g_solar_date.year);
	for(i = 1; i <= days; i++)
	{
	    /* to calculate weekday */
	    AccumulateDay = (gClndrAccuDays[StartMonth] + LeapDay + i);
     	week = 1+(AccumulateDay+g_pCalendarInfo->data[YearIndex].BaseWeekday)%7;
		if(week == 1)
			week = 8;
		itoa(i, &str, 10);
		
		if(i == 1 && g_solar_date.day==1)
		{
			region.x = 3+(week-2)*22;
			region.y = (j+1)*16;
			region.width = 22;
			region.height = 16;
			if(week == 8)
			{
				region.y = (j+2)*16;
			}
			GUI_InvertRegion(&region);
		}
		if((i + 1) == g_solar_date.day)
		{
			region.x = 3+(week-1)*22;
			region.y = (j+1)*16;
			region.width = 22;
			region.height = 16;
			if(week == 8)
			{
				region.y = (j+2)*16;
                region.x = 3;
			}
			GUI_InvertRegion(&region);
		}
		GUI_DisplayText(5+(week-2)*22, (j+1)*16, &str);	
		if(week == 8)
			j++;
	}
}

void CALENDAR_DisplayGround(void)
{
#if APP_SUPPORT_RGBLCD==1
    const region_t  reg_Scroll = {0, 112, LCD_WIDTH, 16};
#elif APP_SUPPORT_LCD==1
    const region_t  reg_Scroll = {0, 48, LCD_WIDTH, 16};
#endif

    INT title_len;
    INT8 offset = 0;
    GUI_ClearScreen(NULL);
    ui_auto_update = FALSE;           
#if APP_SUPPORT_RGBLCD==1
	GUI_ResShowImage(GUI_IMG_BG_WEEK, 0, 0, 0);
	display_date();
//	GUI_ResShowString((UINT16)(GUI_STR_SUNDAY+g_cur_week),76, 106);
//	GUI_DisplayDateText(36, 0, TRUE, g_cur_date);
#elif APP_SUPPORT_LCD==1
	GUI_ResShowPic(GUI_IMG_CLOCKICON, 4, 0);    //show icon         
    GUI_ResShowPic(GUI_IMG_CLOCKBG, 0, 16);//show backgroud
    GUI_DisplayBattaryLever();
    //BT_DisplayIcon(101,0);

	GUI_ResShowString((UINT16)(GUI_STR_SUNDAY+g_cur_week),76, 24);
	GUI_DisplayDateText(18, 0, TRUE, g_cur_date);
#endif
    

    memset(g_calendar_title, 0, CLNDR_TITLE_LEN);
    if(g_comval->langid == GUI_LANG_SM_CHINESE || g_comval->langid == GUI_LANG_TR_CHINESE)
    {
        offset = CALENDAR_GenerateLunarString(offset);
        offset = CALENDAR_GenerateSolarTerm(offset);
    }
    offset = CALENDAR_GenerateFestivalString(offset, 0);
    offset = CALENDAR_GenerateFestivalString(offset, 15);
    
    app_trace(APP_MAIN_TRC,"calendar string:%s.",g_calendar_title);

    if(strlen(g_calendar_title)>8)
        strcat(g_calendar_title, "    "); // add blank for scroll


    title_len = GUI_GetTextWidth(g_calendar_title);
    if(title_len > LCD_WIDTH/2)
    {
        g_scroll_title = TRUE;
        GUI_SetScrollRegin(&reg_Scroll, 3);
        GUI_Scroll(g_calendar_title, TRUE);
    }    
    else
    {
        g_scroll_title = FALSE;
#if APP_SUPPORT_RGBLCD==1
		GUI_DisplayText((LCD_WIDTH-title_len)/2, 90, g_calendar_title);
#elif APP_SUPPORT_LCD==1
        GUI_DisplayText((LCD_WIDTH-title_len)/2, 48, g_calendar_title);
#endif
    }
    ui_auto_update = TRUE;

}

void CALENDAR_DisplayTime(UINT32 time)
{
#if APP_SUPPORT_RGBLCD==1
//    region_t clock_region={2,96,64,16};   
 //   ui_auto_update = FALSE;           
 //   GUI_ClearScreen(&clock_region);						

    //show time
//    GUI_DisplayTimeText(2, 112, GUI_TIME_DISPLAY_ALL, time);
 //   GUI_InvertRegion(&clock_region);

    //滚屏时的处理    
    if(g_scroll_title)
       GUI_Scroll(g_calendar_title, FALSE);

#elif APP_SUPPORT_LCD==1
    region_t clock_region={2,24,64,16};   
    ui_auto_update = FALSE;           
    GUI_ClearScreen(&clock_region);						

    //show time
    GUI_DisplayTimeText(2, 24, GUI_TIME_DISPLAY_ALL, time);
    GUI_InvertRegion(&clock_region);

    //滚屏时的处理    
    if(g_scroll_title)
        GUI_Scroll(g_calendar_title, FALSE);
#endif
    //to reduce scr flash
    GUI_UpdateScreen(NULL);
    ui_auto_update = TRUE;
}

void CALENDAR_Redraw(UINT32 param)
{
    date_t    date;
    ap_time_t time;
    UINT32 cur_date;
    UINT32 cur_time;

    TM_GetDateAndTime(&date, &time);

    if(g_is_today)
    {
        cur_date = ((date.year)<<9)+(date.month<<5)+date.day;
        g_solar_date = date;
    }
    else{
        cur_date = ((g_solar_date.year)<<9)+(g_solar_date.month<<5)+g_solar_date.day;
        if(cur_date == ((date.year)<<9)+(date.month<<5)+date.day)
            g_is_today = TRUE;
    }
    
    if(cur_date != g_cur_date)
    {
        g_cur_date = cur_date;
        param |= CALENDAR_DRAW_BACKGROUND;

        // get lunar info
        CALENDAR_ComputeLunarInfo();
    }
    
    //显示背景
    if(param&CALENDAR_DRAW_BACKGROUND)
    {
        CALENDAR_DisplayGround();
    }

    //显示时钟
    if(param&CALENDAR_DRAW_TIME)
    {
	    cur_time = ((time.hour*60+time.minute)*60+time.second)*1000;
        CALENDAR_DisplayTime(cur_time);
    }
}

INT32 calendar_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_VISIABLE == type)
    {
        if(value == CALENDAR_TODAY)
            return !g_is_today;
        else if(value == CALENDAR_DATELIST)
            return g_clndr_vars->date_count!=0;
        else if(value == CALENDAR_TERMLIST)
            return (g_comval->langid == GUI_LANG_SM_CHINESE || g_comval->langid == GUI_LANG_TR_CHINESE);
    }
    
    return 0;
}

void clndr_Load_Dates(UINT32 entry)
{
    INT32 fd = FS_OpenDirect(entry, FS_O_RDONLY, 0);
    INT32 len;
    INT32 month,day;
    INT32 i,j,k,count;
    
    if(fd<0)
        return;

    k = count = 0;
    len = FS_Read(fd, g_calendar_title, CLNDR_TITLE_LEN);
    while(len>0)
    {
        app_trace(APP_MAIN_TRC, "Load dates read text: %s", g_calendar_title);
        for(i=0;i<len;i++)
        {
            if(g_calendar_title[i]>='0'&&g_calendar_title[i]<='9')
                break;
        }
        j = sscanf(g_calendar_title+i, "%d", &month);
        app_trace(APP_MAIN_TRC, "Load dates, month=%d, day=%d, ret=%d",month, day, i);
        if(j != 1)
            break;
        for(;i<len;i++)
        {
            if(g_calendar_title[i]==',')
                break;
        }
        j = sscanf(g_calendar_title+i+1, "%d", &day);
        app_trace(APP_MAIN_TRC, "Load dates, month=%d, day=%d, ret=%d",month, day, i);
        if(j != 1)
            break;

        g_clndr_vars->dates[count].month = month;
        g_clndr_vars->dates[count].day = day;
        
        for(j=0,k=0;i<len;i++)
        {
            if(g_calendar_title[i]==':')
            {
                j = i;
                k=0;
                continue;
            }
            if(j==0)
                continue;
            if(g_calendar_title[i]=='\n'||g_calendar_title[i]=='\r')
            {
                FS_Seek(fd, i-len, FS_SEEK_CUR);
                g_clndr_vars->dates[count++].name[k++]=0;
                k=0;
                break;
            }
            if(j>0)
                g_clndr_vars->dates[count].name[k++] = g_calendar_title[i];
        }
        if(count >= CLNDR_MAX_DATE_NUM)
            break;

        len = FS_Read(fd, g_calendar_title, CLNDR_TITLE_LEN);

    };
    if(k!=0 && count < CLNDR_MAX_DATE_NUM)
        g_clndr_vars->dates[count++].name[k++]=0;
    g_clndr_vars->date_count = count;
    g_clndr_vars->magic = MAGIC_CALENDAR;
    //NVRAMWrite(&g_clndr_vars, VM_AP_CALENDAR,sizeof(g_clndr_vars));
    NVRAMWriteData();
    FS_Close(fd);
}

void clndr_change_date(INT step)
{
    INT day = g_solar_date.day;
    day += step;
    if(day <= 0)
    {
        if(g_solar_date.month == 1)
        {
            g_solar_date.month = 12;
            g_solar_date.year --;                
            if(g_solar_date.year < g_pCalendarInfo->first_year)
                g_solar_date.year = g_pCalendarInfo->last_year;
        }
        else
            g_solar_date.month--;
        day += GetDaysOfMonth(g_solar_date.month, g_solar_date.year);
    }
    else if(day > GetDaysOfMonth(g_solar_date.month, g_solar_date.year))
    {
        day -= GetDaysOfMonth(g_solar_date.month, g_solar_date.year);
        if(g_solar_date.month == 12)
        {
            g_solar_date.month = 1;
            g_solar_date.year ++;                
            if(g_solar_date.year > g_pCalendarInfo->last_year)
                g_solar_date.year = g_pCalendarInfo->first_year;
        }
        else
            g_solar_date.month++;
    }
    g_solar_date.day = day;

    g_is_today = 0;
	SendAppEvent(EV_UI_FW_REDRAW, CALENDAR_DRAW_BACKGROUND);
}
/******************************************************************************
 * CALENDAR_DateSetCallback: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
void CALENDAR_DateSetCallback(date_t *date,ap_time_t *time)
{
	g_solar_date = *date;
}
void CALENDAR_SelectFileCb(file_location_t *f, UINT16 result)
{
    if(LIST_CALLBACK_SELECT_ITEM == result)
    {
    	if(f)
	        clndr_Load_Dates(f->file_entry);
    }
}
/******************************************************************************
 * CALENDAR_MsgHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL CALENDAR_MsgHandler(COS_EVENT *pEvent)
{
    UINT16 param;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SendAppEvent(EV_UI_FW_REDRAW, CALENDAR_DRAW_ALL);
            break;

        case EV_UI_FW_REDRAW:
        	CALENDAR_Redraw(pEvent->nParam1);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_RESUME:
            SendAppEvent(EV_UI_FW_REDRAW, CALENDAR_DRAW_ALL);
            break;

        case EV_UI_FW_ON_EXIT:
            NVRAMWriteData();
            break;

        default:
            //key event from timer menu
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
		        case AP_MSG_RTC:
		            SendAppEvent(EV_UI_FW_REDRAW, CALENDAR_DRAW_TIME);
		            break;
		        case CALENDAR_TODAY:
		            g_is_today = 1;
		            break;
		        case CALENDAR_SELECTDAY:
		            g_is_today = 0;
		            GUI_ClockSetting(&g_solar_date, NULL, 0, 0, CALENDAR_DateSetCallback);
		            SendAppEvent(EV_UI_FW_REDRAW, CALENDAR_DRAW_ALL);
		            break;

		        case CALENDAR_LOADDATES:
		            {
		                file_location_t location;
		                memset(&location, 0, sizeof(file_location_t));
		                GUI_EntryFileBrowser(&location, FSEL_TYPE_ALLFILE, 0, CALENDAR_SelectFileCb);
		            }
		            break;
		        case CALENDAR_DATELIST:
		            GUI_Display_List(LIST_TYPE_MULTI_SELECT, 0, g_clndr_vars->date_count, 0, date_list_callback, FALSE);
		            break;

		        case CALENDAR_FESTIVAL:
		            GUI_Display_List(LIST_TYPE_MULTI_SELECT, 0, g_festival_count+1/*easter sunday*/, 0, festival_list_callback, FALSE);
		            break;
		        case CALENDAR_TERMLIST:
		            GUI_Display_List(LIST_TYPE_MULTI_SELECT, 0, 24, 0, solarterm_list_callback, FALSE);
		            break;
		            
		        case AP_MSG_STANDBY:
		        case AP_KEY_PLAY | AP_KEY_LONG:
		            return RESULT_STANDBY;

#if APP_SUPPORT_RGBLCD==1
		        case AP_KEY_VOLADD|AP_KEY_PRESS:
		        case AP_KEY_VOLADD|AP_KEY_HOLD:
		            clndr_change_date(-7);
		            break;
		        case AP_KEY_NEXT|AP_KEY_PRESS:
		        case AP_KEY_NEXT|AP_KEY_HOLD:
		            clndr_change_date(1);
		            break;

		        case AP_KEY_VOLSUB|AP_KEY_PRESS:
		        case AP_KEY_VOLSUB|AP_KEY_HOLD:
		            clndr_change_date(7);
		            break;

		        case AP_KEY_PREV|AP_KEY_PRESS:
		        case AP_KEY_PREV|AP_KEY_HOLD:
		            clndr_change_date(-1);
		            break;
#else
		        case AP_KEY_VOLADD|AP_KEY_PRESS:
		        case AP_KEY_VOLADD|AP_KEY_HOLD:
		            clndr_change_date(1);
		            break;
		        case AP_KEY_NEXT|AP_KEY_PRESS:
		        case AP_KEY_NEXT|AP_KEY_HOLD:
		            g_is_today = 0;
		            if(g_solar_date.month == 12)
		            {
		                g_solar_date.month = 1;
		                g_solar_date.year ++;                
		                if(g_solar_date.year > g_pCalendarInfo->last_year)
		                    g_solar_date.year = g_pCalendarInfo->first_year;
		            }
		            else
		                g_solar_date.month++;
		            break;
		        case AP_KEY_VOLSUB|AP_KEY_PRESS:
		        case AP_KEY_VOLSUB|AP_KEY_HOLD:
					
#if APP_SUPPORT_LCD==1						
		            clndr_change_date(-1);
		            break;
#elif APP_SUPPORT_RGBLCD==1
					g_is_today = 0;
		            if(g_solar_date.day < GetDaysOfMonth(g_solar_date.month, g_solar_date.year))
		            {
		                g_solar_date.day++;
		                break;
		            }
		            g_solar_date.day = 1;
#endif
		        case AP_KEY_PREV|AP_KEY_PRESS:
		        case AP_KEY_PREV|AP_KEY_HOLD:
		            g_is_today = 0;
		            if(g_solar_date.month == 1)
		            {
		                g_solar_date.month = 12;
		                g_solar_date.year --;                
		                if(g_solar_date.year < g_pCalendarInfo->first_year)
		                    g_solar_date.year = g_pCalendarInfo->last_year;
		            }
		            else
		                g_solar_date.month--;
		            if(g_solar_date.day > GetDaysOfMonth(g_solar_date.month, g_solar_date.year))
		                g_solar_date.day = GetDaysOfMonth(g_solar_date.month, g_solar_date.year);
		            break;
#endif
		        case AP_MSG_WAIT_TIMEOUT:
		            APP_Sleep();
		            break;
		        case AP_KEY_MODE | AP_KEY_PRESS:
#if APP_SUPPORT_MENU==1
			        if(AP_Support_MENU())
			        {			
			           GUI_Display_Menu(GUI_MENU_CALENDAR, calendar_menu_callback);
			        }
			        else
#endif
				        AppGobackScreen();
		            break;
		            
		        case AP_KEY_PLAY | AP_KEY_PRESS:
		            return 0;
		            
                default:
                {
                    //Common events, DONOT deal it.
                    return FALSE;
                }
            }
    }

    return TRUE;
}

/*********************************************************************************
* Description : 主函数
*
* Arguments   :
        param:
            0, 开始时关标停在第0项
*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
INT32 CALENDAR_Entry(INT32 param)
{
    INT8   buf[CLNDR_TITLE_LEN];    //date &  time
    INT32  key;
    INT32  result;
    INT8   days;        //本月的天数

    g_is_today = 1;
    g_cur_date = 0; // force to generate date
    g_pCalendarInfo = (clndr_table_struct*)GetCalendarData();

    if(g_pCalendarInfo == NULL)
    {
        return GUI_DisplayMessage(0, GUI_STR_RESERROR, NULL, GUI_MSG_FLAG_WAIT);
    }
    MESSAGE_Initial(g_comval);	

    //NVRAMRead(&g_clndr_vars, VM_AP_CALENDAR,sizeof(g_clndr_vars));
    g_clndr_vars = (calendar_vars_t*)NVRAMGetData(VM_AP_CALENDAR,sizeof(calendar_vars_t));
    if(g_clndr_vars->magic != MAGIC_CALENDAR)
        g_clndr_vars->date_count = 0;


	EntryNewScreen(SCR_ID_CALENDAR,CALENDAR_MsgHandler,NULL, 0, 0);

}

#endif  // APP_SUPPORT_LCD


