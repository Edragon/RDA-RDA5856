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
#ifndef __REG_DEFAULT_H__
#define __REG_DEFAULT_H__
#ifndef NUMBER_OF_SIM
#else
#if(NUMBER_OF_SIM==1)
#include "csw_cfg_default1.h"
#elif(NUMBER_OF_SIM==2)
#include "csw_cfg_default2.h"
#elif(NUMBER_OF_SIM==3)
#include "csw_cfg_default3.h"
#elif(NUMBER_OF_SIM==4)
#include "csw_cfg_default4.h"
#else
// TODO: Add default config when no sim
#endif
#endif
#endif//__REG_DEFAULT_H__





