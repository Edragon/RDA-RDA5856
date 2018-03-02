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
 *	?
 * Author
 * 	?
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#include "at_common.h"
#include "at_sa.h"
#include "at_module.h"

#include "sxs_io.h"
#include "cos.h"

extern PUBLIC VOID sync_bp_ap_sync_init(VOID);

BOOL AT_IsRingTimer(VOID);
BOOL AT_KillRingTimer(VOID);

AT_ModuleInfo g_AtModule = { 0 };

/*----------------------------------------------------------------------------------------------------------------------
									COMMON SECTION
----------------------------------------------------------------------------------------------------------------------*/
#if !defined(WIN32)
#include "hal_gpio.h"
#endif /* WIN32 */
// VOID  AT_GPIOWakeup(HAL_GPIO_GPIO_ID_T gpio);
// VOID  AT_GPIOSleep(HAL_GPIO_GPIO_ID_T gpio);
extern UART_INFO_T g_UartInfo;
//UINT8 g_AtSemaphore = 0;
#ifdef GDB_DEBUG_DISPLAY_MODE_SUPPORT
extern BOOL hal_GdbRegisterInfoToATFunc(void (*funcPtr)(void));

VOID GDBBlueinfoTOAT(VOID)
{
     UINT8 blue_string[20]={"AST_BLUESCREEN\n"};
     uart_SendDataToTe(blue_string, strlen(blue_string));

}
#endif

VOID AT_ModuleInit(VOID)
{
  UART_CFG ucfg = { UART_BAUD_RATE_115200, UART_8_DATA_BITS, UART_1_STOP_BIT, UART_NO_PARITY, UART_AFC_MODE_DISABLE
  };  // laixf no flow control
  //BOOL bCfg = 0;  // hameina[+]20080326 for cfg part

  AT_TC(1, "VOID AT_ModuleInit(VOID)");
  g_AtModule.mode               = AT_MODE_CMD;
  g_AtModule.status             = AT_STATUS_CMD_LINE;
  g_AtModule.uiCmerMode         = AT_CMER_DATA_BUF_MODE;
  g_AtModule.pCmdList           = NULL;
  g_AtModule.uiModuleInitStatus = 0xff;

  

  /* 
     Init UART module and open default UART device.
     * */
  //g_AtSemaphore = sxr_NewSemaphore(1);
  AT_UartInit(&ucfg);
  AT_UartOpen(UART_DEFAULT_UART, ds_low);

  /* 
     After the initialization of whole AT modem, need to send a hint to TE, as 'AT Ready'.
     * */
  AT_TC(1, "AT  Initialize AT_WriteUart");
#if defined(CFW_PPP_SUPPORT)&&!defined(AT_NO_GPRS)
  CFW_SetPppSendFun(uart_SendDataPPPCallBack);
#endif /* CFW_PPP_SUPPORT */

  AT_WriteUart((UINT8*)"\r\nAT Ready\r\n\r\nAST_POWERON\r\n", strlen("\r\nAT Ready\r\n\r\nAST_POWERON\r\n"));
  g_AtModule.bIsRun = TRUE;

}

VOID AT_ModuleSetInitResult(UINT8 uiStatus)
{
  g_AtModule.uiModuleInitStatus = uiStatus;
}

UINT8 AT_ModuleGetInitResult(VOID)
{
  return g_AtModule.uiModuleInitStatus;
}

UINT8 AT_GetMode(VOID)
{
  return g_AtModule.mode;
}

UINT8 AT_SetCmerMode(UINT8 mode)
{

  g_AtModule.uiCmerMode = mode;

  return g_AtModule.uiCmerMode; // For checking the result
}

UINT8 AT_GetCmerMode(VOID)
{
  return g_AtModule.uiCmerMode;
}
VOID AT_SetStatus(INT8 status)
{
  g_AtModule.status = status;
}
INT8 AT_GetStatus(VOID)
{
  return g_AtModule.status;
}

// Operations on Prev Command
AT_CMD** AT_GetPrevCommandList(VOID)
{
  return& (g_AtModule.pPrevCmdList);
}

// Opeations on Command List
AT_CMD** AT_GetCommandList(VOID)
{
  // AT_TC(g_sw_ATE, "----The addr of the g_AtModule.pCmdList is 0x%x.----", g_AtModule.pCmdList);
  return& (g_AtModule.pCmdList);
}
VOID AT_ClearCommandList(VOID)
{
  AT_CMD** ppCmdList = AT_GetCommandList();
  AT_CMD* pCurr = *ppCmdList;

  // clean command list when meet error command

  while (pCurr != NULL)
  {
    AT_FreeCurCmd(ppCmdList, pCurr);
    pCurr     = NULL;
    ppCmdList = AT_GetCommandList();
    pCurr     = *ppCmdList;
  }

  SA_SetSysStatus(SA_INNER_RUNCOMMAND_STATE_, 0);

  AT_SetStatus(AT_STATUS_CMD_LINE);
  return;
}

AT_IND** AT_GetIndicationList(VOID)
{
  return& (g_AtModule.pIndList);
}
AT_CMD_BUF* AT_GetCmdBuf(VOID)
{
  return& (g_AtModule.cmd_buf);
}

// Clean Command Line Buffer
VOID AT_CleanCmdBuf(AT_CMD_BUF* pCBuf)
{
  AT_MemSet(pCBuf->DataBuf, 0, AT_CMD_LINE_BUFF_LEN);
  pCBuf->DataLen = 0;
  return;
}

// Async timer operations
BOOL AT_IsAsyncTimer()
{
  return (g_AtModule.AsyncTimerId != 0) ? TRUE: FALSE;
}

UINT8 AT_SetAsyncTimer(UINT32 nElapse)
{
	AT_KillAsyncTimer();
	g_AtModule.AsyncTimerId = COS_SetTimer(nElapse SECOND, ATE_HandleTimeout, NULL, COS_TIMER_MODE_SINGLE);

	return g_AtModule.AsyncTimerId;
}
BOOL AT_KillAsyncTimer(VOID)
{
  if (AT_IsAsyncTimer())
  {
  	COS_KillTimer(g_AtModule.AsyncTimerId);
  	g_AtModule.AsyncTimerId = 0;
  }

  return TRUE;
}


#if 0 // ZXB: Disable the TA support this time.
UINT16 AT_WriteUart(UINT8* pDataToSend, UINT16 data_len)
{
  UINT8* p                    = 0;
  UINT16 len                  = 0;
  ta_chset_convert_result_t r = cs_cvt_INVALID_;
  UINT16 send_r               = 0;

  AT_ASSERT(pDataToSend != NULL);

  /* 
     if MT is UCS2 then
     len = data_len + 2
     else
     len = data_len * 2 + 2
     end
     * */

  if (cfg_GetMtChset() == cs_ucs2)
  {
    p = AT_MALLOC(data_len + 2);
    memset(p, 0, sizeof(data_len + 2));
  }
  else
  {
    p = AT_MALLOC(data_len* 2 + 2);
    memset(p, 0, sizeof(data_len* 2 + 2));
  }

  r = ate_TaConvert(mode_mt2te, pDataToSend, data_len, p, &len);

  if (r == cs_cvt_needless)
  {
    memcpy(p, pDataToSend, data_len);
    len = data_len;
  }
  else if (r == cs_cvt_failed)
  {
    AT_TC(g_sw_ATE, "Convert string failed by calling ate_TaConvert().");
    return 0;
  }

  send_r = uart_SendDataToTe(p, len);

  free(p);
  return send_r;
}
#else
UINT16 AT_WriteUart(UINT8* pDataToSend, UINT16 data_len)
{
//    AT_TC(g_sw_ATE, "AT_WriteUart %s",pDataToSend);
  return uart_SendDataToTe(pDataToSend, data_len);
}

UINT16 AT_WriteUartString(char* fmt,...)
{
	va_list args;
	UINT8 str[256];

	memset(str, 0, sizeof(str));

	va_start(args,fmt);
	vsprintf(str, fmt, args);
	va_end(args);
	

  return uart_SendDataToTe(str, strlen(str));
}
#endif

// ]]laixf [+] 2008-4-29: wait for 2 seconds delay
// TODO: this function provided by ATS.PS module. Erase this stub after they provide it.

// confirming a value and moving it  to cfw.h
// Caoxh [Add] 2008-4-17
#define EV_CFW_GPRS_DATA_REQ    1
HANDLE g_hPPPTask = NULL;

#ifdef CFW_PPP_SUPPORT
extern INT32 CFW_PppSendData(UINT8* pData, UINT16 nDataSize, UINT8 MuxId);
#endif

BOOL AT_IsRun()
{
	 return g_AtModule.bIsRun;
}


static BOOL gInLPMode = FALSE; //for sleep
BOOL  AT_GETLPMode()
{
    return gInLPMode;
}

VOID AT_SetLPMode(BOOL bMode)
{
  AT_TC(g_sw_UART, " AT_SetLPMode %d", bMode);
  //static UINT8 gInLPMode = 0; for sleep

  if (gInLPMode == bMode)
    return;
  else
    gInLPMode = bMode;
  if (bMode)
  {
    // gInLPMode = 1;
    // if(g_UartInfo.CommOpened)    
    AT_UartClose(g_UartInfo.Scom_UartId);
  }
  else
  {
    // gInLPMode = 0;
    // if(!g_UartInfo.CommOpened)    
    AT_UartOpen(g_UartInfo.Scom_UartId, ds_low);
    
  }

  return;

}
//#endif

