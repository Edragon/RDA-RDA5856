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


//**********************************************************************************
// change record:
// 2006.8.16  hameina get the original file, with the [REG_HKEY_SYSTEM_ROOT/Time]
//            and the values defined below.
// 2006.8.25  hameina add key REG_HKEY_CURRENT_COMM include both subkeys and values
//
// 2006.8.31  chenwenfeng changed the defaulte value of REG_HKEY_SYSTEM_ROOT
//            /Time/TM_FORMAT 0->538
//2006.9.9   chenwenfeng changed the defaulte value of TM_FORMAT 02,1a->1a,02
//2006.9.21  nieyongfeng [mod] 0000->00
//2006.9.22  chenwenfeng [mod] "\"TM_FORMAT\"=hex: (1a,02->  1a,02,00,00)\n"
//2006.10.13  wangxiaojing [+] EMOD_NWNM   EMOD_SIMNM_1(len:48)   EMOD_SIMNM_2(len:44)   EMOD_SIGN
//2007.8.17  hameina[+] "[REG_HKEY_CURRENT_COMM/general]\n" ---->subvalue IMSI
//***************************************************************************************
#ifndef __REG_DEFAULT_H1__
#define __REG_DEFAULT_H1__
const char *default_registry =
    "[REG_HKEY_SYSTEM_ROOT/Time]\n"\
    "\"TM_FORMAT\"=hex:1a,02,00,00\n"\
    "\"TM_ZONE\"=hex:00\n"\
    "\"TM_ALARM\"=hex:01\n"\
    "\"TM_BASE\"=hex:80,43,6D,38\n"\
    "\"TM_TIME\"=hex:80,43,6D,38\n"\
    "\"TM_ALARM1\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM2\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM3\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM4\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM5\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM6\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM7\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM8\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM9\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM10\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM11\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM12\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM13\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM14\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"TM_ALARM15\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "[REG_HKEY_SYSTEM_ROOT/FS]\n"\
    "\"PSTATUS\"=int8:0\n"\
    "[REG_HKEY_CURRENT_COMM/configure]\n"\
    "\"SMS_PARAM\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "[REG_HKEY_CURRENT_COMM/cm]\n"\
    "\"PUBLIC\"=int32:3\n"\
    "\"CCWA\"=int8:1\n"\
    "\"CLIR\"=int8:0\n"\
    "\"COLP\"=int8:0\n"\
    "[REG_HKEY_CURRENT_COMM/general]\n"\
    "\"MEMORYINFO\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,00\n"\
    "\"IND_EVENT\"=hex:ff,ff\n"\
    "\"IMSI\"=hex:00,ff,ff,ff,ff,ff,ff,ff,ff,ff\n"\
    "[REG_HKEY_CURRENT_COMM/nw]\n"\
    "\"NW_WORK_MODE\"=int8:0\n"\
    "\"NW_FLIGHT_MODE\"=int8:0\n"\
    "\"NW_FREQ_BAND\"=int8:76\n"\
    "\"NW_SIM0_PLMN\"=hex:00,00,00\n"\
    "\"NW_SIM0_BA_COUNT\"=int8:0\n"\
    "\"NW_SIM0_ARFCN_LIST\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"NW_SIM0_STORE_LAST\"=hex:00,00\n"\
    "\"GPRS_UP_SUM_SIM0\"=int32:0\n"\
    "\"GPRS_DWN_SUM_SIM0\"=int32:0\n"\
    "[REG_HKEY_CURRENT_COMM/sim]\n"\
    "\"SimCardSwitch\"=int8:0\n"\
    "\"SoftReset\"=int8:0\n"\
    "\"IsFirstPowerOn\"=int8:1\n"\
    "[REG_HKEY_CURRENT_COMM/sms]\n"\
    "\"SMS_STORAGE\"=int8:2\n"\
    "\"SMS_OVER_IND\"=int8:0\n"\
    "\"FULL_FOR_NEW\"=int8:0\n"\
    "\"DEF_PARAM_IND\"=int8:0\n"\
    "\"SMS_PARAM1\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_PARAM2\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_PARAM3\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_PARAM4\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_PARAM5\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_STORAGEINFO1\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_STORAGEINFO2\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_STORAGEINFO3\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_STORAGEINFO4\"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00\n"\
    "\"SMS_OPTION\"=int8:4\n"\
    "\"SMS_OPT_STORAGE\"=int8:2\n"\
    "[REG_HKEY_CURRENT_COMM/emod]\n"\
    "\"EMOD_PARAM\"=hex:00\n"\
    "\"EMOD_NWNM\"=hex:ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff\n"\
    "\"EMOD_SIGN\"=int8:0\n"\
    "\"EMOD_SIMNM_1\"=hex:ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff\n"\
    "\"EMOD_SIMNM_2\"=hex:ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff\n"\
    "[REG_HKEY_CURRENT_COMM/TCPIP]\n"\
    "\"DNS_ADR1\"=\"211.136.17.107\"\n"\
    "\"DNS_ADR2\"=\"211.136.17.107\"\n"\
    "[REG_HKEY_CURRENT_COMM/JMETOC]\n"\
    "\"APN\"=\"cmnet\"\n"\
    "\"USER\"=\"wap\"\n"\
    "\"PWD\"=\"wap\"\n"\
    "\"PROXY_ADR\"=hex:0a,00,00,ac\n"\
    "\"PROXY_PORT\"=hex:50,00\n"\
    "\"SIM_APN_NUM\"=hex:00\n"\
    "[REG_HKEY_CURRENT_COMM/DBS]\n"\
    "\"SEED\"=\"0000000\"\n";

#endif//__REG_DEFAULT_H1__





