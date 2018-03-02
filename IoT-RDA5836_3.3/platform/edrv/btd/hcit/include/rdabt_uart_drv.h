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


#ifndef DEVICE_DRV_H
#define  DEVICE_DRV_H

#include "hal_uart.h"
#include "hal_gpio.h"

#define  DEV_NOTIFY 		0x81
#define  DEV_SRC			0x88

enum _dev_Action
{
     DEV_READ,
     DEV_WRITE
    
};

struct  CircularBuf
{
    UINT16 max_size;
	UINT8 *Buf;//[CIRCULAR_BUF_LEN];	
	UINT16 Get;
	UINT16 Put;
	UINT16 Buf_len;
	UINT8 NotEmpty;
	UINT8  Name[6];
};

typedef struct  _device
{	
 	UINT8 port;  	
 	UINT8 RecMbx;
 	UINT8 RxPending;  	
	UINT8  RxUseDMA;
	UINT8  TxUseDMA;
	HAL_UART_CFG_T  uartCfg;
	HAL_UART_IRQ_STATUS_T  mask;  
 	void (*registered_func)(UINT8 port);
	struct  CircularBuf  writeBuf;	
	struct  CircularBuf  readBuf;	
	UINT8  Next;
} DEVICE;

typedef enum 
{
	NO_PORT,
    	PORT1,
	PORT2,
	PORT3
}PORT;
/*typedef enum _dev_type
{
     DEV_UART,
     DEV_USB,
     DEV_I2C
  	
}DEV_TYPE;*/
typedef enum 
{
	NO_CLOCK,		
	CLOCK_2HZ, //T =500ms
	CLOCK_5HZ,//T = 200ms
	CLOCK_10HZ,//T=100ms
	CLOCK_100HZ,//T=10ms
	CLOCK_200HZ,//T=5ms
} CLOCK_TYPE;
#define READ_BUFF_NULL   3



BOOL UART_Open(PORT port,UINT16 Ower_Task);
BOOL UART_Close(PORT port);
UINT16  UART_PutBytes( PORT port, UINT8 *pBuff, UINT16 NumToWrite,UINT16 Owner);
UINT16  UART_GetBytes(  PORT port, UINT8 *pBuff,UINT16 NumToRead,UINT8 *pstate,UINT16 Owner);
void UART_SetDCBConfig(PORT port, UINT32 band, UINT8 flowctrl );
void UART_Register_RX_cb(PORT port,UINT16 owner,void(*callfunc)(UINT8 port));
VOID  GPIO_INT_Registration(HAL_APO_ID_T gpio, UINT8 type, void(*pfun)());
VOID GPIO_WriteData(UINT8 gpio, UINT8 Val);
//void  write_uart_log( UINT8 *data, UINT16 num, UINT8 rx_tx);

//BOOL Is_Mcd_exist();

VOID I2C_Open(VOID);
VOID I2C_Close(VOID);
#endif

