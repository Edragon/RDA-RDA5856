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


#ifndef _RAM_H_ 
#define _RAM_H_

#include <csw.h>


#define EV_RAM_MODEM_COM_OPENED_IND                         1213
#define EV_RAM_MODEM_COM_CLOSED_IND                         1214
#define EV_RAM_MODEM_TRANS_BEGIN_IND                        1215
#define EV_RAM_MODEM_TRANS_OVER_IND                         1216



typedef enum _COMM_STATE_
{
	COMM_IDLE=0,
	COMM_USED=1,
}COMM_STATE;


typedef struct _RAM_COMMUNICATION_MANAGE
{
	HANDLE RamManageSem ;
	DRV_UARTID UartID;
	COMM_STATE CommState;
	DRV_UARTCFG drv_uartcfg;
	VOID* pRamModemApiMsg;
}RAM_COMMUNICATION_MANAGE;



INT32 RAM_ModemTaskStart(UINT8  ModemTaskPriority);
VOID  RAM_MmiEventProc(CFW_EVENT* pEvent);
INT32 RAM_ModemOpen(VOID );
INT32 RAM_ModemClose(VOID );
VOID RAM_GetModemManageInfo(RAM_COMMUNICATION_MANAGE * pModemCmuManage);
VOID RAM_StopTrans(VOID);

#endif



