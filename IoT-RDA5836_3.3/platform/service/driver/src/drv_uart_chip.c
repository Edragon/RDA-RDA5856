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
/******************************************************************************/
/* Filename:    drv_uart_chip.c    version1.0.0    @07-08-10                  */
/*                                                                            */
/* Description:                                                               */
/*     driver module based on CSW platform.                                   */
/*     @wangzw 2007-08-10.                                                    */
/******************************************************************************/
#if 0 // this file no used
#include <csw.h>
#include "cswtype.h"
#include "chip_id.h"

#include "hal_uart.h"

#include "csw_csp.h"
#include "ts.h"
#include "string.h"

#include "drv.h"

#define DEFAULT_UART_BUFF_LENGTH	8


enum uart_open{
CLOSE=0,
OPEN=1
};


struct uart_status{
	enum uart_open open;
	enum DRV_UartTransfertMode mode;	
	UARTCALLBACK	callback;
};

static struct uart_status uart_status[DRV_UART_2+1]=
{
	{
		open:	CLOSE,
		mode:	DRV_UART_TRANSFERT_MODE_QTY,
		callback:	NULL
	},

	{
		open:	CLOSE,
		mode:	DRV_UART_TRANSFERT_MODE_QTY,
		callback:	NULL		
	},

	{
		open:	CLOSE,
		mode:	DRV_UART_TRANSFERT_MODE_QTY,
		callback:	NULL		
	}
	
};

//static UINT8 uart_status[DRV_UART_2+1]={ [0 ... DRV_UART_2]=0 };

void uart_api_callback(UINT16 status)
{
#if 0
	COS_EVENT  ev;
	        
	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("\t\t\t\t\tuart interrupt is %04x\n"),status    ); 

	switch(status)
	{
		case HAL_UART_MASK_DMA_RX:
		{ 
			/* The Uart Dma transfer is finished. */
			ev.nEventId  = UART_DMA_RX;
			COS_SendEvent(g_hMmiTask, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_RX\n")    );  
		}
		break;

		case HAL_UART_MASK_RX:  //中断收到后要取出来,否则将不停的引发中断
		{
			hal_uart_GetData(USING_DEFAULT_UART, buff_receive, 0x10);
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_RX\n")    );  
		}
		break;

		case HAL_UART_MASK_DMA_TX:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_TX\n")    );  
		}
		break;

		case HAL_UART_MASK_TIMEOUT:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_TIMEOUT\n")    );  
			hal_uart_ClearErrorStatus(USING_DEFAULT_UART);           
		}
		break;

		default:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("OTHER interrupt occured\n")    );  
			hal_uart_ClearErrorStatus(USING_DEFAULT_UART);
		}
		break;
	}
#endif
}


void uart0_api_callback(UINT16 status)
{
	return ;
}


void uart1_api_callback(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    union 
    {
        HAL_UART_IRQ_STATUS_T mask;
        UINT16 reg;
    } u;

    u.mask.txModemStatus      = 0;
    u.mask.rxDataAvailable    = 0;
    u.mask.txDataNeeded       = 0;
    u.mask.rxTimeout          = 0;
    u.mask.rxLineErr          = 0;
    u.mask.txDmaDone          = 0;
    u.mask.rxDmaDone          = 0;
    u.mask.rxDmaTimeout       = 0;
    hal_UartIrqSetMask(HAL_UART_1, u.mask);
	
    CSW_TRACE(CSW_DRV_TS_ID, TSTXT("uart1_api_callback \n")  );

    u.mask = status;

	if(NULL != uart_status[1].callback )
	{
		(*uart_status[1].callback)(u.reg);
	}

	switch(u.reg)
	{
		case HAL_UART_MASK_DMA_RX:
		{ 
			/* The Uart Dma transfer is finished. */
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_RX\n")    );  
		}
		break;

		case HAL_UART_MASK_RX:  //中断收到后要取出来,否则将不停的引发中断
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_RX\n")    ); 			
		}
		break;

		case HAL_UART_MASK_DMA_TX:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_TX\n")    );			
		}
		break;

		case HAL_UART_MASK_TX:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_TX\n")    );
		}
		break;

		default:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("OTHER interrupt occured\n")    );  
			hal_uart_ClearErrorStatus(1);
		}
		break;
	}
		
	return ;
}

void uart2_api_callback(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    union 
    {
        HAL_UART_IRQ_STATUS_T mask;
        UINT16 reg;
    } u;

    u.mask.txModemStatus      = 0;
    u.mask.rxDataAvailable    = 0;
    u.mask.txDataNeeded       = 0;
    u.mask.rxTimeout          = 0;
    u.mask.rxLineErr          = 0;
    u.mask.txDmaDone          = 0;
    u.mask.rxDmaDone          = 0;
    u.mask.rxDmaTimeout       = 0;
    hal_UartIrqSetMask(HAL_UART_2, u.mask);
	
    CSW_TRACE(CSW_DRV_TS_ID, TSTXT("uart1_api_callback \n")  );

    u.mask = status;

	if(NULL != uart_status[2].callback )
	{
		(*uart_status[2].callback)(u.reg);
	}

	switch(u.reg)
	{
		case HAL_UART_MASK_DMA_RX:
		{ 
			/* The Uart Dma transfer is finished. */
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_RX\n")    );  
		}
		break;

		case HAL_UART_MASK_RX:  //中断收到后要取出来,否则将不停的引发中断
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_RX\n")    ); 			
		}
		break;

		case HAL_UART_MASK_DMA_TX:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_DMA_TX\n")    );			
		}
		break;

		case HAL_UART_MASK_TX:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("HAL_UART_MASK_TX\n")    );
		}
		break;

		default:
		{
			CSW_TRACE(CSW_DRV_TS_ID, TSTXT("OTHER interrupt occured\n")    );  
			hal_uart_ClearErrorStatus(2);
		}
		break;
	}
		
	return ;
}

static void register_transfer_interrupt(DRV_UARTID UartID)
{
	switch(uart_status[UartID].mode)			
	{
		case DRV_UART_TRANSFERT_MODE_DIRECT_IRQ:
		{
			switch(UartID)
			{
				case DRV_UART_0:
				{
                    // TODO error
				}
				break;
	
				case DRV_UART_1:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 1,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_1, uart1_api_callback);
                    hal_UartIrqSetMask(HAL_UART_1, mask);

				}
				break;

				case DRV_UART_2:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 1,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_2, uart2_api_callback);
                    hal_UartIrqSetMask(HAL_UART_2, mask);
                }
				break;

				default:
				{
					;
				}
			}	
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DMA_IRQ:
		{			
			switch(UartID)
			{
				case DRV_UART_0:
				{
                    // TODO Error
				}
				break;
	
				case DRV_UART_1:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 1,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_1, uart1_api_callback);
                    hal_UartIrqSetMask(HAL_UART_1, mask);
                }
				break;

				case DRV_UART_2:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 1,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_2, uart2_api_callback);
                    hal_UartIrqSetMask(HAL_UART_2, mask);
				}
				break;

				default:
				{
					;
				}				
				
			}			
		}
		break;

		default:
		{
            HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
            hal_UartIrqSetMask(UartID, mask);
		}
	}

}


static void register_receive_interrupt( DRV_UARTID UartID)
{
	switch(uart_status[UartID].mode)			
	{
		case DRV_UART_TRANSFERT_MODE_DIRECT_IRQ:
		{
			switch(UartID)
			{
				case DRV_UART_0:
				{
                    // TODO Error
				}
				break;
	
				case DRV_UART_1:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 1,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_1, uart1_api_callback);
                    hal_UartIrqSetMask(HAL_UART_1, mask);
				}
				break;

				case DRV_UART_2:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 1,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_2, uart2_api_callback);
                    hal_UartIrqSetMask(HAL_UART_2, mask);
				}
				break;

				default:
				{
					;
				}				
			}			
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DMA_IRQ:
		{			
			switch(UartID)
			{
				case DRV_UART_0:
				{
                    // TODO Error            
				}
				break;
	
				case DRV_UART_1:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 1,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_1, uart1_api_callback);
                    hal_UartIrqSetMask(HAL_UART_1, mask);
				}
				break;

				case DRV_UART_2:
				{
                    HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 1,
                        .rxDmaTimeout       = 0
                    };
                    hal_UartIrqSetHandler(HAL_UART_2, uart2_api_callback);
                    hal_UartIrqSetMask(HAL_UART_2, mask);
				}
				break;

				default:
				{
					;
				}				
			}			
		}
		break;

		default:
		{
            HAL_UART_IRQ_STATUS_T mask =
                    {
                        .txModemStatus      = 0,
                        .rxDataAvailable    = 0,
                        .txDataNeeded       = 0,
                        .rxTimeout          = 0,
                        .rxLineErr          = 0,
                        .txDmaDone          = 0,
                        .rxDmaDone          = 0,
                        .rxDmaTimeout       = 0
                    };
            hal_UartIrqSetMask(UartID, mask);
        }
	}

}

INT16 DRV_UartOpen(DRV_UARTID UartID, 
	DRV_UARTCFG *drv_uartcfg, UARTCALLBACK user_uart_callback)
{

    HAL_UART_CFG_T uartcfg;

	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if ( (drv_uartcfg->data) >= DRV_UART_DATA_BITS_QTY)
		return DRV_UART_INVALID_DATABIT;

	if ( (drv_uartcfg->stop) >= DRV_UART_STOP_BITS_QTY)
		return DRV_UART_INVALID_STOPBIT;

	if ( (drv_uartcfg->parity) >= DRV_UART_PARITY_QTY)
		return DRV_UART_INVALID_PARITY;

	if ( (drv_uartcfg->rate) >= DRV_UART_BAUD_RATE_QTY)
		return DRV_UART_INVALID_BAUDRATE;

	if ( (drv_uartcfg->mode) >= DRV_UART_TRANSFERT_MODE_QTY)
		return DRV_UART_INVALID_MODE;

	
	if(uart_status[UartID].open ==OPEN )
		return DRV_UART_IS_USING;

	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("open uart[%d] begin\n"),UartID    );  

	
	uartcfg.data		=drv_uartcfg->data;
	uartcfg.stop		=drv_uartcfg->stop;
	uartcfg.parity		=drv_uartcfg->parity;
	uartcfg.rate		=drv_uartcfg->rate;
	uartcfg.afc		    =HAL_UART_AFC_MODE_RX_HALF;
	uartcfg.irda		=HAL_UART_IRDA_MODE_DISABLE;
	uartcfg.tx_trigger	=HAL_UART_TX_TRIG_EMPTY;
	uartcfg.rx_trigger	=HAL_UART_RX_TRIG_1;
	uartcfg.tx_mode	=drv_uartcfg->mode;
	uartcfg.rx_mode	=drv_uartcfg->mode;

	switch(drv_uartcfg->mode)
	{
	case DRV_UART_TRANSFERT_MODE_DMA_IRQ:
	case DRV_UART_TRANSFERT_MODE_DMA_POLLING:
		{
			uartcfg.afc=HAL_UART_AFC_MODE_DISABLE;
		}
		break;

	default:
		{
		}
		break;
	}
		
	hal_uart_Open(UartID, &uartcfg);
	hal_uart_PurgeFifo(UartID);                         //clear the cache data
	
	uart_status[UartID].open=OPEN;
	uart_status[UartID].mode=drv_uartcfg->mode;
	uart_status[UartID].callback=user_uart_callback;

	register_receive_interrupt(UartID);
	
	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("open uart[%d] ok  \n"),UartID  );

	return 0;
	
}

//the length can be certified
INT32 DRV_UartSend(DRV_UARTID UartID, UINT8 *buff, UINT16 length)
{
	UINT16 length_temp;
	UINT8 *buff_temp;
	
	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if(uart_status[UartID].open ==CLOSE )
		return DRV_UART_ISNOT_USING;

	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("begin send data %d\n"),UartID);  
	
    HAL_UART_IRQ_STATUS_T mask =
            {
                .txModemStatus      = 0,
                .rxDataAvailable    = 0,
                .txDataNeeded       = 0,
                .rxTimeout          = 0,
                .rxLineErr          = 0,
                .txDmaDone          = 0,
                .rxDmaDone          = 0,
                .rxDmaTimeout       = 0
            };
    hal_UartIrqSetMask(UartID, mask);

	switch(uart_status[UartID].mode)
	{
		case DRV_UART_TRANSFERT_MODE_DIRECT_POLLING:
		{
			buff_temp=buff;
			length_temp=length;
			while( length_temp >= DEFAULT_UART_BUFF_LENGTH )
			{
				hal_uart_SendData(UartID, buff_temp, DEFAULT_UART_BUFF_LENGTH);
				while( !hal_uart_TxFinished(UartID) );
				buff_temp+=DEFAULT_UART_BUFF_LENGTH;
				length_temp-=DEFAULT_UART_BUFF_LENGTH;
			}
			
			hal_uart_SendData(UartID, buff_temp, length_temp);
			while( !hal_uart_TxFinished(UartID) );
			buff_temp+=length_temp;
			length_temp-=length_temp;	
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DIRECT_IRQ:
		{
			buff_temp=buff;
			length_temp=length;
			while( length_temp > DEFAULT_UART_BUFF_LENGTH )
			{
				hal_uart_SendData(UartID, buff_temp, DEFAULT_UART_BUFF_LENGTH);
                // FIXME ??? This is polling ...
				while( !hal_uart_TxFinished(UartID) );
				buff_temp+=DEFAULT_UART_BUFF_LENGTH;
				length_temp-=DEFAULT_UART_BUFF_LENGTH;
			}
			
			hal_uart_SendData(UartID, buff_temp, length_temp);

			register_transfer_interrupt( UartID	);
			
			while( !hal_uart_TxFinished(UartID) );
			buff_temp+=length_temp;
			length_temp-=length_temp;

			register_receive_interrupt(UartID);	
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DMA_POLLING:
		{
			if (	!hal_uart_TxDmaDone( UartID ) )
				return DRV_UART_DMA_NOT_DONE;
			
			length=hal_uart_SendData(UartID, buff, length);
			
                    while ( (!hal_uart_TxFinished(UartID) )  ||(!hal_uart_TxDmaDone( UartID ) )  )
                    {
                        COS_Sleep(1);
                    }			
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DMA_IRQ:
		{			
			if (	!hal_uart_TxDmaDone( UartID ) )
			{	
				register_receive_interrupt(UartID);
				return DRV_UART_DMA_NOT_DONE;
			}
			
			length=hal_uart_SendData(UartID, buff, length);
	
			register_transfer_interrupt( UartID	);
			
                    while ( (!hal_uart_TxFinished(UartID) )  ||(!hal_uart_TxDmaDone( UartID ) )  )
                    {
                        COS_Sleep(1);
                    }
					
			register_receive_interrupt(UartID);
		}
		break;

		default:
		{
			;
		}	
	}
	
	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("over send data %d\n"),UartID );
	
	return length;
}


//the length can not be certified, the caller must be check if there has data in FIFO
INT32 DRV_UartReceive(DRV_UARTID UartID, UINT8 *buff, UINT16 length)
{
	UINT16 length_temp=0;

	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if(uart_status[UartID].open ==CLOSE )
		return DRV_UART_ISNOT_USING;
	
	if(NULL==buff)
		return DRV_UART_INVALID_BUFFER;

	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("begin receive data %d\n"),UartID);

	
	switch(uart_status[UartID].mode)
	{
		case DRV_UART_TRANSFERT_MODE_DIRECT_POLLING:
		case DRV_UART_TRANSFERT_MODE_DIRECT_IRQ:
		{
			length_temp = hal_uart_GetData(UartID, buff,  length);			
		}
		break;

		case DRV_UART_TRANSFERT_MODE_DMA_POLLING:
		case DRV_UART_TRANSFERT_MODE_DMA_IRQ:
		{
			if (	!hal_uart_RxDmaDone( UartID ) )
				return DRV_UART_DMA_NOT_DONE;

			length_temp = hal_uart_GetData(UartID, buff,  length);

			if ( !length_temp )
				return DRV_UART_NO_DMA_AVIAL;

			hal_UpdateXCpuCache(buff, length_temp);
		}
		break;

		default:
		{
			;
		}

	}

	CSW_TRACE(CSW_DRV_TS_ID, TSTXT("over receive data %d\n"),UartID); 
	
	return length_temp;
	
}


INT16 DRV_UartClose(DRV_UARTID UartID)
{
	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if(uart_status[UartID].open ==CLOSE )
		return DRV_UART_ISNOT_USING;

	hal_uart_Close(UartID);
	
	uart_status[UartID].open=CLOSE;
	uart_status[UartID].mode=DRV_UART_TRANSFERT_MODE_QTY;
	uart_status[UartID].callback=NULL;

    HAL_UART_IRQ_STATUS_T mask =
            {
                .txModemStatus      = 0,
                .rxDataAvailable    = 0,
                .txDataNeeded       = 0,
                .rxTimeout          = 0,
                .rxLineErr          = 0,
                .txDmaDone          = 0,
                .rxDmaDone          = 0,
                .rxDmaTimeout       = 0
            };
    hal_UartIrqSetMask(UartID, mask);
	
	return 0;
}


//get the fifo length, then call receive function to get the data,it just used in 
//no-DMA mode
INT16 DRV_UartGetReceiveLen(DRV_UARTID UartID)
{
	INT16	length=0;
	
	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if(uart_status[UartID].open ==CLOSE )
		return DRV_UART_ISNOT_USING;

	length = hal_uart_RxFifoLevel(/*(enum hal_UartId)*/UartID);
	
	return length;
}


INT16 DRV_uart_RxDmaDone( DRV_UARTID UartID )
{
	if (UartID >= DRV_UART_QTY)
		return DRV_UART_INVALID_UARTID;

	if(uart_status[UartID].open ==CLOSE )
		return DRV_UART_ISNOT_USING;

	return ( hal_uart_RxDmaDone(UartID) );
	
}

#endif

