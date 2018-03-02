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
/*                                                                            */
/* DESCRIPTION                                                                */
/*   drivers API form CSW platform.                                           */
/*   See CSW Development Manual for more.                                     */
/*                                                                            */
/******************************************************************************/
#include "hal_uart.h"


HAL_UART_IRQ_HANDLER_T  win32_uart_irq_handler;

PUBLIC VOID hal_UartOpen(HAL_UART_ID_T id, CONST HAL_UART_CFG_T* uartCfg)
{
}

PUBLIC VOID hal_UartClose(HAL_UART_ID_T id)
{
}

PUBLIC UINT32 hal_UartSendData(HAL_UART_ID_T id, CONST UINT8* data, UINT32 length)
{
	return CATDlg_SendData(data,length);
}

PUBLIC UINT32 hal_UartGetData(HAL_UART_ID_T id, UINT8* destAddress, UINT32 length)
{
	return CATDlg_GetSendData(destAddress,length);
}

PUBLIC VOID hal_UartIrqSetHandler(HAL_UART_ID_T id, HAL_UART_IRQ_HANDLER_T handler)
{
	win32_uart_irq_handler = handler;
}

PUBLIC VOID hal_UartIrqSetMask(HAL_UART_ID_T id, HAL_UART_IRQ_STATUS_T mask)
{
}

PUBLIC VOID hal_UartSetRts(HAL_UART_ID_T id, BOOL ready)
{
}

PUBLIC VOID hal_UartFifoFlush(HAL_UART_ID_T id)
{
}



PUBLIC BOOL hal_UartTxDmaDone(HAL_UART_ID_T id)
{
	return TRUE;

}

PUBLIC UINT8 hal_UartRxFifoLevel(HAL_UART_ID_T id)
{
	return CATDlg_GetSendDataLength();
}

PUBLIC BOOL hal_UartTxFinished(HAL_UART_ID_T id)
{
	return TRUE;
}
// ============================================================================
// hal_UartClearErrorStatus
// ----------------------------------------------------------------------------
/// This function clears all the line error status. The error status is cleared 
/// automatically by the interrupt dispatcher code, but if you do not use the 
/// interrupt, the error status is not cleared. This means that if you check 
/// the error status manually (by polling it with the #hal_UartGetErrorStatus 
/// function), you'll need to call the #hal_UartClearErrorStatus function to 
/// clear it, manually.
///
/// @param id Identifier of the UART for which the function is called.
// ============================================================================
PUBLIC VOID hal_UartClearErrorStatus(HAL_UART_ID_T id)
{
}

void ATDlgIrqHandler(void)
{
	HAL_UART_IRQ_STATUS_T status;
	HAL_UART_ERROR_STATUS_T error;

	memset(&status, 0, sizeof(HAL_UART_IRQ_STATUS_T));
	memset(&error, 0, sizeof(HAL_UART_ERROR_STATUS_T));
	status.rxDataAvailable = 1;

	if(win32_uart_irq_handler)
		win32_uart_irq_handler(status,error);
}

PUBLIC HAL_ERR_T hal_UartGetErrorStatus(HAL_UART_ID_T id)
{
    return HAL_ERR_NO;
}

