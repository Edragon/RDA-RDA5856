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

#ifndef __AT_COMMON_H__
#define __AT_COMMON_H__

#include "cs_types.h"
#include "errorcode.h"

#include "at_define.h"
#include "at_trace.h"
#include "at_errcode.h"
#include "at_utility.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "sul.h"

#define AT_MOD_ID   MOD_APP  //MOD_SYS


#define AT_CMD_BS        0x08 // the ASCII value of <BS>
#define AT_CMD_CR        0x0D // the ASCII value of <CR>
#define AT_CMD_LF        0x0A // the ASCII value of <LF>
#define AT_CMD_CTRL_Z    0x1A // the ASCII value of file ending symbol
#define AT_CMD_ESC       0x1B // the ASCII value of file ending symbol

#if  defined(CODEC_SUPPORT) || defined(__AT_MOD_SPECIAL__)
#define COMMAND_ECHO_MODE_DEFUALT_VAL 0
#else
#define COMMAND_ECHO_MODE_DEFUALT_VAL 1
#endif

// 2: for supportting GBK and USC2 character set
#ifdef WIN32
#define AT_CMD_LINE_BUFF_LEN 512  //2048 move to target.def for different setting
#endif

// System mode/////////
#define AT_MODE_CMD        0
#define AT_MODE_DATA_PPP   1
#define AT_MODE_DATA_TCPIP 2
#define AT_MODE_DATA_WAP   3


// UART status in command mode
#define AT_STATUS_CMD_LINE      0
#define AT_STATUS_CMD_RUNNING   1
#define AT_STATUS_IP_DATA		3
#define AT_STATUS_PPP_DATA		4

// +CMER mode/////////
#define AT_CMER_ONLY_BUF_MODE     0
#define AT_CMER_DATA_DIS_MODE     1
#define AT_CMER_DATA_BUF_MODE     2
#define AT_CMER_DMA_MODE          3


/* Cmd type, used in AT_CMD & AT_CMD_PARA */
#define AT_CMD_SET  0
#define AT_CMD_TEST 1
#define AT_CMD_READ 2
#define AT_CMD_EXE  3

/* ATL_CmdMainFunc return code */
#define CMD_FUNC_SUCC         0 // command sync success
#define CMD_FUNC_SUCC_ASYN    1 // command async success,
// need to be ended by async event
#define CMD_FUNC_FAIL         2 // command fail
#define CMD_FUNC_CONTINUE     3 // command need to be continue
#define CMD_FUNC_WAIT_SMS     4 // waiting for sms
#define CSW_IND_NOTIFICATION  5 // indication from csw
#define CMD_FUNC_WAIT_IP_DATA 6 // waiting for ip data
#define CMD_FUNC_SUCC_ASYN_CLEAR_CMD 7  // waiting for ip data
#define CMD_FUNC_SUCC_NO_CLEAR_CMD 8  // waiting for ip data


/*  DCE Result code             */
#define CMD_RC_OK           		0 // "OK"
#define CMD_RC_CONNECT      		1 // "CONNECT"
#define CMD_RC_RING         		2 // "RING/CRING"
#define CMD_RC_NOCARRIER    		3 // "NO CARRIER"
#define CMD_RC_ERROR        		4 // "ERROR"
#define CMD_RC_NODIALTONE   		5 // "NO DIALTONE"
#define CMD_RC_BUSY         		6 // "BUSY"
#define CMD_RC_NOANSWER     		7 // "NO ANSWER"
#define CMD_RC_NOTSUPPORT   		8 // "NOT SUPPORT"
#define CMD_RC_INVCMDLINE			9 // "INVALID COMMAND LINE"
#define CMD_RC_CR           		10  // "\r\n"
#define CMD_RC_SIMDROP           	11  // "Sim drop"

#define CMD_ERROR_CODE_OK       255
#define CMD_ERROR_CODE_TYPE_CME 0
#define CMD_ERROR_CODE_TYPE_CMS 1

#define ATE_RING_TIME_ELAPSE 1000 // 1s
#define ATE_EXTENSION_TIME_ELAPSE 2000 // 1s
#define AT_DEFAULT_ELAPSE    40
#define ATE_LP_TIME_ELAPSE   10000  // 10s
#define ATE_SMS_TIME_ELAPSE   1000  // 1s


#define AT_Notify2ATM AT_NotifyResult2ATM
#define AT_SIM_ID(x) 0

typedef struct _AT_CMD_RESULT
{
  UINT32 uReturnValue;
  UINT32 uResultCode;
  UINT32 uErrorCode;
  UINT32 uCmdStamp; // command stamp
  UINT16 nDataSize;
  UINT8 nErrorType;
  UINT8 nDelayTime;
  UINT8 nDLCI;
  UINT8 pData[80];
} AT_CMD_RESULT, *PAT_CMD_RESULT;


typedef struct _AT_CMD_PARA
{
  UINT8* pPara; // parameter for command
  UINT32 iExDataLen;  // length of extra data
  UINT8* pExData; // extra data
  UINT8 iType;  // command type
  UINT32 uCmdStamp; // command stamp
  UINT8 nDLCI;
} AT_CMD_PARA;

typedef VOID (*pfn_AtCmdHandler) (AT_CMD_PARA * pParam);

// //////////////////////////////////////
// AT Command Defination
// //////////////////////////////////////
// caoxh [mod] 16->20 2008-5-20
#define AT_CMD_NAME_SIZE  20
#ifdef CODEC_SUPPORT
#define AT_CMD_PARA_SIZE  550
#else
#define AT_CMD_PARA_SIZE  AT_CMD_LINE_BUFF_LEN
#endif

typedef struct _AT_CMD
{
  UINT8 pName[AT_CMD_NAME_SIZE + 1];
  UINT8 pPara[AT_CMD_PARA_SIZE + 1];
  UINT8 iType;
  UINT32 uCmdStamp; // command stamp
  UINT8 pad[3];
  pfn_AtCmdHandler pCmdFunc;
  UINT8 nDLCI;
  struct _AT_CMD* pNext;
} AT_CMD;

typedef struct _AT_IND
{
  AT_CMD_RESULT result;

  struct _AT_IND* pNext;
} AT_IND;

typedef enum
{
    cs_gsm,
    cs_hex,
    cs_ucs2,
    cs_gbk, // equals to PCCP 936 char set
    cs_COUNT_,
    cs_INVALID_ = 0x8F,
} at_chset_t;




/************************************************/
#if defined(WIN32)
#define  I_AM_HERE()
#else
#define I_AM_HERE() \
	do \
	{ \
		AT_OutputText(1, "I am here %s %s:%d", __FILE__,__func__, __LINE__);\
	} while (0)
#endif /* WIN32 */

extern VOID ATE_HandleTimeout(VOID*);
PAT_CMD_RESULT AT_CreateRC(UINT32 uReturnValue, UINT32 uResultCode,UINT32 uErrorCode,UINT8 nErrorType, UINT8 nDelayTime, UINT8* pBuffer, UINT16 nDataSize, UINT8 nDLCI);
INT32 AT_NotifyResult2ATM(PAT_CMD_RESULT pCmdResult, UINT8 nDLCI);
BOOL AT_AddCmdToList(AT_CMD* pCmd);
VOID AT_FreeCurCmd(AT_CMD** ppCmdList, AT_CMD* pCmd);
VOID ATE_HandleException(UINT32 uErrCode, UINT8 nDLCI);
VOID AT_RunNextCommand(VOID);


#endif

