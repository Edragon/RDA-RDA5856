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
/*                                                                            */
/* FILE NAME                                                                  */
/*      drv.h                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   drivers API form CSW platform.                                           */
/*   See CSW Development Manual for more.                                     */
/*                                                                            */
/******************************************************************************/

#ifndef _DRV_H_
#define _DRV_H_

typedef struct _DRV_SCREENINFO {
    UINT16 width;
    UINT16 height;
    UINT8 bitdepth;
    UINT8 isMemoryAddrValid;
    UINT16 nResvered;
    VOID* pMemoryAddr;
}DRV_SCREENINFO;

typedef struct _LCD_INFO {
    short  em_x;	
    short  em_y;    
	short  ScreenWidth;
    short  ScreenHeight;
    short  ScreenBitDepth;
    UINT32* pMemoryAdd;
}LCD_INFO;

//
//create 24 bit 8/8/8 format pixel (0xRRGGBB) from RGB triplet
//
#define DDI_RGB2PIXEL888(r,g,b)	\
	(((r) << 16) | (((BYTE)(g)) << 8) | ((BYTE)b))

//
//create 16 bit 5/6/5 format pixel (0xRGB) from RGB triplet
//
#define DRV_RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

#define DRV_LCD_IOCTL_CONTRAST        0x01
#define DRV_LCD_IOCTL_BRIGHTNESS      0x02
#define DRV_LCD_IOCTL_STANDBY         0x03
#define DRV_LCD_IOCTL_UNSTANDBY       0x04
#define DRV_LCD_IOCTL_SLEEP           0x05
#define DRV_LCD_IOCTL_UNSLEEP         0x06
#define DRV_LCD_IOCTL_DEVINFO         0x07
#define DRV_LCD_IOCTL_BACKLIGHT       0x08

UINT32 SRVAPI DRV_LcdInit (
                    VOID
                    );

UINT32 SRVAPI DRV_LcdExit (
                    VOID
                    );

UINT32 SRVAPI DRV_LcdIoControl (
                         UINT32 uCode,
                         UINT32 nParamIn,
                         UINT32* pParamOut
                         );

UINT32 SRVAPI DRV_LcdSetPixel16(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 pPixelData
                  );

UINT32 SRVAPI DRV_LcdBlt16(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 nSrcWidth,
                  UINT16 nSrcHeight,
                  UINT16* pPixelData
                  );

UINT32 SRVAPI DRV_LcdBmpBlt16(
  INT16  nXDest, // LCD position x
  INT16  nYDest, // LCD position y
  INT16  nXSrc,  // bmp   x
  INT16  nYSrc,  // bmp   y
  INT16  nWidth, // bmp   w
  INT16  nHeight, // bmp  h
  UINT16* pBmpData
);

UINT32 SRVAPI DRV_SubLcdInit (
                       VOID
                       );

UINT32 SRVAPI DRV_SubLcdExit (
                       VOID
                       );

UINT32 SRVAPI DRV_SubLcdIoControl (
                            UINT32 uCode,
                            UINT32 nParamIn,
                            UINT32* pParamOut
                            );

UINT32 SRVAPI DRV_SubLcdSetPixel12(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 nPixelData
                  );

UINT32 SRVAPI DRV_SubLcdBlt(
                     UINT8 nXDest,
                     UINT8 nYDest,
                     UINT8 nSrcWidth,
                     UINT8 nSrcHeight,
                     UINT16* pRgbData
                     );

//
// GPIO 
//

#define DRV_GPIO_0          0
#define DRV_GPIO_1          1     //Added by jinzh:20070619
#define DRV_GPIO_2          2
#define DRV_GPIO_6          6
#define DRV_GPIO_7          7
#define DRV_GPIO_8          8
#define DRV_GPIO_9          9
#define DRV_GPIO_10         10
#define DRV_GPIO_11         11
#define DRV_GPIO_12         12
#define DRV_GPIO_13         13
#define DRV_GPIO_14         14
#define DRV_GPIO_15         15
#define DRV_GPIO_16         16
#define DRV_GPIO_17         17
#define DRV_GPIO_18         18
#define DRV_GPIO_19         19

#define DRV_GPIO_20         20
#define DRV_GPIO_21         21
#define DRV_GPIO_22         22
#define DRV_GPIO_23         23
#define DRV_GPIO_24         24
#define DRV_GPIO_25         25
#define DRV_GPIO_26         26
#define DRV_GPIO_27         27
#define DRV_GPIO_28         28
#define DRV_GPIO_29         29
#define DRV_GPIO_30         30
#define DRV_GPIO_31         31

#define  DRV_GPIO_EDGE_R    0
#define  DRV_GPIO_EDGE_F    1
#define  DRV_GPIO_EDGE_RF   2

#define DRV_GPIO_IN 		0
#define DRV_GPIO_OUT 	    1

#define DRV_GPIO_LOW 		0
#define DRV_GPIO_HIGH 	    1

UINT32 SRVAPI DRV_GpioSetDirection(
  UINT8 nGpio,     
  UINT8 nDirection     
);

UINT32 DRV_GpioSetMode( 
    UINT8 nGpio,
    UINT8 nMode
);

UINT32 SRVAPI DRV_GpioGetDirection(
  UINT8 nGpio,     
  UINT8* pDirection     
);

UINT32 SRVAPI DRV_GpioSetLevel(
  UINT8 nGpio,
  UINT8 nLevel
);

UINT32 SRVAPI DRV_GpioGetLevel(
    UINT8 nGgpio,
    UINT8* pLevel
);

UINT32 SRVAPI DRV_GpioSetEdgeIndication(
  UINT8 nGpio,
  UINT8 nEdge,
  UINT32 nDebonce
);

UINT32 SRVAPI DRV_EXTISetEdgeIndication(
    UINT8  nEdge
);


#define DRV_MEMORY_NO_PAGE   0
#define DRV_MEMORY_4HW_PAGE  1
#define DRV_MEMORY_8HW_PAGE  2
#define DRV_MEMORY_16HW_PAGE 3

#define DRV_MEMORY_0_CYCLE   0
#define DRV_MEMORY_1_CYCLE   1
#define DRV_MEMORY_2_CYCLE   2
#define DRV_MEMORY_4_CYCLE   3

typedef enum 
{	
	DRV_EXTI_EDGE_NUL,
	DRV_EXTI_EDGE_R,
	DRV_EXTI_EDGE_F,	
	DRV_EXTI_EDGE_RF,
}DRV_EXTI_EDGE;

typedef enum 
{
  DRV_MEMORY_FLASH=0,
  DRV_MEMORY_SRAM,
  DRV_MEMORY_CS2,
  DRV_MEMORY_CS3, /* this one can be used for main FLASH too */
  DRV_MEMORY_CS4, /* this one can be used for main SRAM too */
  DRV_MEMORY_CS5,
  DRV_MEMORY_CS6,
  DRV_MEMORY_CS_QTY
}DRV_EBC_CS;

typedef struct _DRV_MEM_CFG {
 BOOL writeAllow;
 BOOL writeByteEnable;
 BOOL waitMode;
 UINT8 pageSize;
 UINT8 tACS;
 UINT8 tCAH;
 UINT8 tCOS;
 UINT8 tOCH;
 INT32 tACC;
 INT32 tPMACC;
}DRV_MEM_CFG;

PVOID SRVAPI DRV_MemorySetupCS(
  UINT8 nCS,
  DRV_MEM_CFG*  pMemCfg
);

UINT32 SRVAPI DRV_MemoryDisableCS(
  UINT8 nCS
);

//
// PWML
//
UINT32 SRVAPI DRV_SetPwl1(
  UINT8 nLevel
);

UINT32 DRV_SetKeyBacklight( UINT8 nLevel);

UINT32 SRVAPI DRV_SetPwl0(
  UINT8 nLevel
);

/*************************************************
 *** UART
 *************************************************/
//define the error code 
#define DRV_UART_INVALID_UARTID		-1
#define DRV_UART_INVALID_BAUDRATE	-2
#define DRV_UART_IS_USING				-3
#define DRV_UART_ISNOT_USING			-4
#define DRV_UART_INVALID_BUFFER		-5
#define DRV_UART_INVALID_DATABIT		-6
#define DRV_UART_INVALID_STOPBIT		-7
#define DRV_UART_INVALID_PARITY		-8
#define DRV_UART_INVALID_MODE		-9
#define DRV_UART_DMA_NOT_DONE		-10
#define DRV_UART_NO_DMA_AVIAL		-11


#define DRV_UART_INVALID_TXMODE		-(9+12)
#define DRV_UART_INVALID_RXMODE		-(10+12)


/* Defines for interupts. */
#define DRV_UART_MASK_NIL				0
#define DRV_UART_MASK_RX				(1<<1)
#define DRV_UART_MASK_TX				(1<<2)
#define DRV_UART_MASK_TIMEOUT		(1<<3)
#define DRV_UART_MASK_ERROR			(1<<4)
#define DRV_UART_MASK_DMA_TX			(1<<5)
#define DRV_UART_MASK_DMA_RX			(1<<6)


/* UART number */
typedef enum  DRV_UartId{
  DRV_UART_0 = 0,
  DRV_UART_1 = 1,
  DRV_UART_2 = 2,
  DRV_UART_QTY=3
}DRV_UARTID;


/* data bits. */
enum DRV_UartDataBits {
	DRV_UART_7_DATA_BITS , 
	DRV_UART_8_DATA_BITS ,
	DRV_UART_DATA_BITS_QTY
};

/* Stop bit quantity. */
enum DRV_UartStopBitsQty {  
	DRV_UART_1_STOP_BIT , 
	DRV_UART_2_STOP_BITS ,  
	DRV_UART_STOP_BITS_QTY
};

/* Data parity control selection. */
enum DRV_UartParityCfg {
	DRV_UART_NO_PARITY ,
	DRV_UART_ODD_PARITY ,  
	DRV_UART_EVEN_PARITY ,  
	DRV_UART_SPACE_PARITY ,  
	DRV_UART_MARK_PARITY , 
	DRV_UART_PARITY_QTY
};

/* Baudrate with the modifiable system clock. */
enum  DRV_UartBaudRate{
  DRV_UART_BAUD_RATE_2400		=    2400,
  DRV_UART_BAUD_RATE_4800		=    4800,
  DRV_UART_BAUD_RATE_9600		=    9600,
  DRV_UART_BAUD_RATE_14400		=   14400,
  DRV_UART_BAUD_RATE_19200		=   19200,
  DRV_UART_BAUD_RATE_28800		=   28800,
  DRV_UART_BAUD_RATE_33600		=   33600,
  DRV_UART_BAUD_RATE_38400		=   38400,
  DRV_UART_BAUD_RATE_57600		=   57600,
  DRV_UART_BAUD_RATE_115200		=  115200,
  DRV_UART_BAUD_RATE_230400		=  230400,
  DRV_UART_BAUD_RATE_460800		=  460800,
  DRV_UART_BAUD_RATE_921600		=  921600,
  DRV_UART_BAUD_RATE_1843200	= 1843200,
  DRV_UART_BAUD_RATE_QTY
};

/* Data transfert mode: via DMA or direct. */
enum  DRV_UartTransfertMode{	
  DRV_UART_TRANSFERT_MODE_DIRECT_POLLING = 0,	//direct polling,has no irq
  DRV_UART_TRANSFERT_MODE_DIRECT_IRQ = 1,		//direct transfer with the irq callback
  DRV_UART_TRANSFERT_MODE_DMA_POLLING = 2,		//DMA mode with no irq callback
  DRV_UART_TRANSFERT_MODE_DMA_IRQ = 3,			//DMA mode with irq callback
  DRV_UART_TRANSFERT_MODE_QTY
};


typedef struct DRV_UartCfg {
enum DRV_UartDataBits data;			//data bit. 
enum DRV_UartStopBitsQty stop;  		//stop bit
enum DRV_UartParityCfg parity;  		//parity type
enum DRV_UartBaudRate rate;  			//baudrate
enum DRV_UartTransfertMode mode; 		//transfer mode
}DRV_UARTCFG;

//define for the callback function type
typedef void (*UARTCALLBACK)(UINT16 status);

/*
description:	open the uart
parameters:	UartID 		the uart number.
			drv_uartcfg	the configuration for the specific uart
			user_uart_callback		the callback function point, which will be
called when the transfer is complete, or there has received data in the fifo the first byte.
return value:	0 for successful, negative value for fail
*/
INT16 DRV_UartOpen(DRV_UARTID UartID, 
	DRV_UARTCFG *drv_uartcfg, UARTCALLBACK user_uart_callback);

/*
description :	send the length bytes to uart, before using it, please call open first.
it will not be returned until the total length of the data has been transfered
parameters:		UartID 	the uart number
				buff		the buffer of the data which will be transfered
				length	the length of the data which will be transfered
return value:	positive value for successful, negative value for fail
*/
INT32 DRV_UartSend(DRV_UARTID UartID, UINT8 *buff, UINT16 length);

/*
description:		receive the length bytes from uart.before using it, please call 
DRV_UartGetReceiveLen to check the length bytes in the fifo. in the DMA mode you 
should call DRV_uart_RxDmaDone the check if the DMA has been completed.
parameters:		UartID 	the uart number
				buff		the buffer of the data which will be transfered
				length	the length of the data which will be transfered
return value:	positive value for successful, negative value for fail
*/
INT32 DRV_UartReceive(DRV_UARTID UartID, UINT8 *buff, UINT16 length);

/*
after finish using the uart , we can call it to close the uart
parameters:		UartID 	the uart number
return value:	positive value for successful, negative value for fail
*/
INT16 DRV_UartClose(DRV_UARTID UartID);

/*
get the length bytes in the fifo, then call receive function to get the data,it just works
in no-DMA mode
parameters:		UartID 	the uart number
return value:	positive value for successful, negative value for fail
*/
INT16 DRV_UartGetReceiveLen(DRV_UARTID UartID);

/*
check if the DMA receive has been done, it can only be called when the uart
works in DMA mode
parameters:		UartID 	the uart number
return value:	it will return 1, otherwise it will return negative value
*/
INT16 DRV_uart_RxDmaDone( DRV_UARTID UartID );


typedef enum {
    DRV_SPI_CS0 = 0, ///< Chip Select 0
    DRV_SPI_CS1,     ///< Chip Select 1
    DRV_SPI_CS_QTY
} DRV_SpiCs_t;




typedef enum
{
	DRV_SPI_DIRECT_POLLING,	 	
	DRV_SPI_DIRECT_IRQ 	,		
	DRV_SPI_DMA_POLLING ,			
	DRV_SPI_DMA_IRQ 		,				
	DRV_SPI_OFF 		,			
	DRV_SPI_TM_QTY 				
}DRV_SpiTransfertMode;
typedef enum
{
	DRV_SPI_HALF_CLK_PERIOD_0  ,		
	DRV_SPI_HALF_CLK_PERIOD_1 ,	
	DRV_SPI_HALF_CLK_PERIOD_2,	
      DRV_SPI_HALF_CLK_PERIOD_3		
}DRV_Spi_Delay;
typedef enum
{
	DRV_SPI_RX_TRIGGER_1_BYTE      ,		
	DRV_SPI_RX_TRIGGER_4_BYTE	,	
	DRV_SPI_RX_TRIGGER_8_BYTE	,		
	DRV_SPI_RX_TRIGGER_12_BYTE	,	
	DRV_SPI_RX_TRIGGER_QTY			
} DRV_SpiRxTriggerCfg;
typedef enum
{
	DRV_SPI_TX_TRIGGER_1_EMPTY	,	
	DRV_SPI_TX_TRIGGER_4_EMPTY	,
	DRV_SPI_TX_TRIGGER_8_EMPTY	,	
	DRV_SPI_TX_TRIGGER_12_EMPTY	,
	DRV_SPI_TX_TRIGGER_QTY			
}DRV_SpiTxTriggerCfg;

#define  DRV_SPI_MIN_FRAME_SIZE			4
#define  DRV_SPI_MAX_FRAME_SIZE			32
#define  DRV_SPI_MAX_CLK_DIVIDER			0xFFFFFF

#define  DRV_SPI_IRQ_MSK_RX_OVF			0x1
#define  DRV_SPI_IRQ_MSK_TX_TH				0x2
#define  DRV_SPI_IRQ_MSK_TX_DMA			0x4
#define  DRV_SPI_IRQ_MSK_RX_TH				0x8
#define  DRV_SPI_IRQ_MSK_RX_DMA			0x10

typedef  VOID (*DEVICE_SPI_USERVECTOR)(UINT16);


typedef struct _DRV_SpiCfg {
  DRV_SpiCs_t 		enabledCS;
  BOOL           polarityCS;
  unsigned char  	inputEn;
  unsigned char  	clkFallEdge;
  DRV_Spi_Delay 		clkDelay;
  DRV_Spi_Delay 		doDelay;
  DRV_Spi_Delay 		diDelay;
  DRV_Spi_Delay 		csDelay;
  DRV_Spi_Delay 		csPulse;
  int 			frameSize;
  int 			clkDivider;
  DRV_SpiRxTriggerCfg  		rxTrigger;
  DRV_SpiTxTriggerCfg  		txTrigger;
  DRV_SpiTransfertMode  	rxMode;
  DRV_SpiTransfertMode  	txMode;
  UINT16  irqMask; ///< Irq Mask for this CS
  DEVICE_SPI_USERVECTOR irqHandler; ///< Irq Handler for this CS

} DRV_SpiCfg;/*this struct to user*/

UINT32 DRV_SPIInit( DRV_SpiCfg*spicfg);


VOID DRV_SPIExit(DRV_SpiCs_t cs);

void DRV_WaitSPIRxDmaDone(
	unsigned char *datain,
	unsigned int inlen
	);

UINT32 DRV_SPIWrite(DRV_SpiCs_t cs,UINT8 *data, UINT16 len);

UINT32 DRV_SPIRead(DRV_SpiCs_t cs,UINT8 *outbuf,UINT16 len);

void DRV_SPISetISRmask(
	UINT16 mask
	);

UINT8 DRV_Spi_TxFinished(DRV_SpiCs_t cs);
UINT32 DRV_ISRSpiRead(
	unsigned char *add ,
	unsigned int len
	);
UINT32 DRV_GetSpiIsrFifoLen(
	void
	);

void DRV_SPI_ChangeCs(
	DRV_SpiCfg *spicfg
	);

void hal_HstSendEvent(UINT32,UINT32);

#endif // _H_ 

