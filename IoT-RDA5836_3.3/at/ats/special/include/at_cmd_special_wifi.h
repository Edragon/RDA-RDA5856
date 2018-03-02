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

/*********************************************************
 *
 * File Name
 *	at_cmd_special_wifi.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_WIFI_H_
#define _AT_CMD_EX_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**************************************************************************************************
	 *                                          INCLUDES
	 **************************************************************************************************/
#include "at_common.h"



#ifdef __cplusplus
}
#endif

typedef enum
{
    NO_TEST,
    RX_TEST,
    TX_TEST,
    OTHER
} WIFI_TEST_MODE;

void wifi_test_mode_open(WIFI_TEST_MODE mode);
VOID AT_WIFI_CmdFunc_iOpen(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iClose(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iSet(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iTOpen(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iTClose(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iTSet(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_iTSetReg(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_WPOWER(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_WSCAN(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_WJOIN(AT_CMD_PARA *pParam);
VOID AT_WIFI_CmdFunc_WADDR(AT_CMD_PARA *pParam);
#ifdef USB_WIFI_SUPPORT
extern VOID wifi_PowerOn(UINT8 *local_mac);

#endif

#endif

