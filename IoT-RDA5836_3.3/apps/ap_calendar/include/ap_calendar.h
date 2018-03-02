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

#ifndef _AP_CALENDAR_H_
#define _AP_CALENDAR_H_


/* struct clndr_lnuar_date_info_struct defines all data needed for lunar calendar */
typedef struct
{
    UINT16 first_year;
    UINT16 last_year;
    struct{
        UINT8 BaseDays;    /* different days from solar Jan 1 to lunar Jan 1. */
        UINT8 BaseWeekday; /* week day of Jan 1 this year. 0=Sunday */
        UINT8 EasterSunday; /* easter sunday from March 1st */
        UINT8 Intercalary; /* an intercalary month in the lunar calendar. 0==no intercalary month in this year. */
        UINT16 MonthDays;  /* the number of days in lunar month, bit-level representation 0=29, 1=30. */
        UINT8 solar_term_table[24];
    }data[1];
} clndr_table_struct;



INT32 CALENDAR_Entry(INT32 param);//进入日历的主要函数
U16 IsLeapYear(U16 year);

#endif /*_AP_CALENDAR_H_*/




