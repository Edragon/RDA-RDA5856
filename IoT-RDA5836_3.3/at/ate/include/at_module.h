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
 *	at_module.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef __AT_MODULE_H__
#define __AT_MODULE_H__

#include "at_common.h"
#include "at_uart.h"

// For at module initializing
// TODO: delete them, use the SA flags instead.
#define AT_MODULE_INIT_NO_SIM                       0
#define AT_MODULE_INIT_SIM_AUTH_READY               1
#define AT_MODULE_INIT_SIM_AUTH_WAIT_PUK1           2
#define AT_MODULE_INIT_SIM_AUTH_WAIT_PIN1           3
#define AT_MODULE_INIT_SAT_OK                       4
#define AT_MODULE_INIT_SAT_ERROR                    5
#define AT_MODULE_INIT_NORMAL_SIM_CARD              6
#define AT_MODULE_INIT_CHANGED_SIM_CARD             7
#define AT_MODULE_INIT_NORMAL_CHANGED_SIM_CARD      8
#define AT_MODULE_INIT_TEST_SIM_CARD                9
#define AT_MODULE_INIT_SIM_SMS_OK                   10
#define AT_MODULE_INIT_SIM_SMS_ERROR                11
#define AT_MODULE_INIT_SIM_PBK_OK                   12
#define AT_MODULE_INIT_SIM_PBK_ERROR                13
#define AT_MODULE_INIT_ME_SMS_OK                    14
#define AT_MODULE_INIT_ME_SMS_ERROR                 15
#define AT_MODULE_INIT_ME_PBK_OK                    16
#define AT_MODULE_INIT_ME_PBK_ERROR                 17
#define AT_MODULE_INIT_BATT_OK                      18
#define AT_MODULE_NOT_INIT                          0xff
#define AT_BLUETOOTH_CHANNEL                        9

VOID AT_ModuleSetInitResult(UINT8 uiStatus);
UINT8 AT_ModuleGetInitResult(VOID);

// [+]move here from at_uart.h
typedef enum _PSSM_STATE_T
{
  pssm_W_PFX, // Initial state, waiting for prefix delay.
  pssm_W_STR, // Waiting for "+++" string.
  pssm_W_SFX, // Waiting for suffix delay.
  pssm_GOTTEN,  // A "+++" string token has been found, need to exit from PS data transferring mode.
} PSSM_STATE_T;

typedef struct _AT_COM_BUF
{
  UINT8 padding[2];
  UINT8 Mark; // 0 :the data in buffer have been processed;
  // Other value : the data need to be parse.
  UINT32 DataLen; // This variable is the length of the data in DataBuf;
  UINT8 DataBuf[AT_CMD_LINE_BUFF_LEN];  // The address of the Buffer.
} AT_CMD_BUF;

typedef struct _AT_ModuleInfo
{
  UINT8 mode; // AT Module Mode
  INT8 status;  // status in Command Mode
  UINT8 uiCmerMode; // CMER Mode

  UINT8 uiModuleInitStatus;

  // Counter of success RC.
  // It's a flag to mark whether or not to add a <cr><lf> prefix bofore response information when ATV set to 1.
  // when counter is 0, need to add prefix; otherwize need not.
  // This counter is setted by successful RC handler and clear (to 0) by other type of RC handler.
  UINT8 uSuccRcCounter;
    AT_CMD* pPrevCmdList; // Previous Command
    AT_CMD* pCmdList; // Command List
  AT_CMD_BUF cmd_buf; // Command Buffer
  AT_CMD_BUF cmd_buf_bak; // Backup command buffer for supporting of A/ command.

    AT_IND* pIndList; // Indicator List
  UINT8 AsyncTimerId; // 
  BOOL bLPTimer;
  BOOL bExtensionTimer;

  BOOL bInLPMode;
  BOOL bDtmfTimer;

  BOOL bIsRun;
}
AT_ModuleInfo;

VOID AT_ModuleInit(VOID);

// Operations On AT Module Mode and Cmer Mode[for 27007]
VOID AT_SetMode(UINT8 mode);
UINT8 AT_GetMode(VOID);
UINT8 AT_SetCmerMode(UINT8 mode);
UINT8 AT_GetCmerMode(VOID);

// Operations On UART Mode during Command Mode
VOID AT_SetStatus(INT8 status);
INT8 AT_GetStatus(VOID);


// Operations on Prev Command
AT_CMD** AT_GetPrevCommandList(VOID);

// Operations On Command List
AT_CMD** AT_GetCommandList(VOID);
AT_IND** AT_GetIndicationList(VOID);

// Operations On Command Buffer.
AT_CMD_BUF* AT_GetCmdBuf(VOID);

/*
FUNCTION:
Converts the string from TE to MT by their character set setting.

PARAMETER:
[pTeData][I]Points the data buffer from TE, and the data (string) encoding indicated by +CSCS command.
[len][I]The data length of pTeData.
[pMtDataBuff][O]TE data will be converted and saved into this buffer based on the character set setting of MT.

RESULT:
Returns the data length of converting into pMtDataBuff, in bytes.
**/
extern INT32 AT_ConvertTeData2Mt(UINT8* pTeData, UINT32 len, AT_CMD_BUF* pMtDataBuff);

// Async Timer operation
BOOL AT_IsAsyncTimer();
UINT8 AT_SetAsyncTimer(UINT32 nElapse);
BOOL AT_KillAsyncTimer(VOID);

// Clean Command Line Buffer
VOID AT_CleanCmdBuf(AT_CMD_BUF* pCBuf);
extern UINT16 AT_WriteUart(UINT8* pDataToSend, UINT16 data_len);
VOID AT_ClearCommandList(VOID);
UINT16 AT_WriteUartString(char* fmt,...);

// laixf[+]2008-4-29
typedef enum _ATE_CALLER_T
{
  TIMEOUT,
  DATA_FULL,
} ATE_CALLER_T;


extern INT32 AT_CommandLineProcess(UINT8* pBuf, UINT32 len);
extern BOOL FirstCmdIsBSlash(UINT8 nDLCI);
extern void RestoreCmdBuffer(void);
extern void BackupCmdBuffer(void);
extern INT32 AT_SmsDataProcess(UINT8* pBuf, UINT32 len);
extern VOID uart_SendDataBTCallBack(UINT8* pDataToSend, UINT16 data_len);

// ]]laixf[+]
extern UINT8 g_uAtGprsEnterDataMode;


BOOL AT_SetLPTimer(UINT32 nElapse); //Add by XP 20130507 for Compiling warning

extern VOID AT_FreeCurInd(AT_IND** ppIndList, AT_IND* pInd);
extern VOID AT_AddIndToList(AT_IND** ppIndList, AT_IND* pInd);

#if defined(__WIFI_RADIO_BOX_MUX__)
enum WIFI_RADIO_BOX_MUX_CHANNEL {
	WRB_MUX_CHANNEL_CORE,
	WRB_MUX_CHANNEL_UART,
	WRB_MUX_CHANNEL_BT,
};
#endif


#endif
 
