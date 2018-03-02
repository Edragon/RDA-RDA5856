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
 *	at_uart.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	API definition of UART module.
 *
 *********************************************************/

#ifndef _AT_DATAPROC_H_
#define _AT_DATAPROC_H_

//#include "csw.h"
#ifdef CHIP_IS_MODEM
typedef UINT32 HAL_UART_ID_T;
#else
#include "hal_uart.h"
#endif

typedef struct _UART_DATA
{
  UINT32 DataLen;
  //UINT8 DataBuf[MIN_ARRAY_SIZE];
  UINT8 DataBuf[1];
} UART_DATA, *PUART_DATA, **PPUART_DATA;

/* UART number */
typedef enum _UART_ID
{
  UART_1 = 1,
  UART_2 = 2,
} UART_ID;

typedef enum _UART_BAUDRATE
{
  UART_BAUD_RATE_2400 = 2400,
  UART_BAUD_RATE_4800 = 4800,
  UART_BAUD_RATE_9600 = 9600,
  UART_BAUD_RATE_14400 = 14400,
  UART_BAUD_RATE_19200 = 19200,
  UART_BAUD_RATE_28800 = 28800,
  UART_BAUD_RATE_33600 = 33600,
  UART_BAUD_RATE_38400 = 38400,
  UART_BAUD_RATE_57600 = 57600,
  UART_BAUD_RATE_115200 = 115200,
  UART_BAUD_RATE_230400 = 230400,
  UART_BAUD_RATE_460800 = 460800,
  UART_BAUD_RATE_921600 = 921600,
  UART_BAUD_RATE_1843200 = 1843200,
} UART_BAUDRATE;

typedef enum _UART_DATABITS
{
  UART_7_DATA_BITS,
  UART_8_DATA_BITS,
} UART_DATABITS;

typedef enum _UART_STOPBITS
{
  UART_1_STOP_BIT,
  UART_2_STOP_BITS,
} UART_STOPBITS;

typedef enum _UART_PARITY
{
  UART_NO_PARITY,
  UART_ODD_PARITY,
  UART_EVEN_PARITY,
  UART_SPACE_PARITY,
  UART_MARK_PARITY,
} UART_PARITY;

typedef enum _UART_AFC_MODE
{
  UART_AFC_MODE_DISABLE,
  UART_AFC_MODE_RX_HALF,
  UART_AFC_MODE_RX_FULL,
} UART_AFC_MODE;

typedef struct _UART_CFG
{
  UART_BAUDRATE br;
  UART_DATABITS db;
  UART_STOPBITS sb;
  UART_PARITY pr;
  UART_AFC_MODE fc;
} UART_CFG;

typedef enum _UART_DS_MODE_T
{
  ds_low,// Low data transfering mode, for command and online command mode to process command line.
  ds_high,// High data transfering mode, for online data mode.
} UART_DS_MODE_T;

#if defined(WIN32)
#define UART_DEFAULT_UART 2
#else
#define UART_DEFAULT_UART AT_DEFAULT_UART
#endif /* WIN32 */

// Programmable receive and transmit fifos (16 bytes deep).
// #define UART_MAX_FIFO_LEVEL 32
#define UART_MAX_FIFO_LEVEL     512

// [[laixf[+]2008-4-29
#define UART_MAX_ONE_DMA_LEN    512

#ifdef CHIP_IS_MODEM
// AP->Modem AT comm buffer is 2K, which means the possible max fifo level is 2K
#define UART_PS_RX_BUFF_LEN     (8192)
#else
// PPP package size, 2k bytes.
// Rx buffer len, double of PPP package size, 4k. 
#define UART_PS_RX_BUFF_LEN     (4096)
#endif

// #define UART_PS_RX_BUFF_LEN  4096*10 //by wulc
// Timeout setting of Rx buffer, about 200ms.
#define UART_PS_TIMER_ELPASE    (50 * 16)

#define UART_PS_MAX_FIFO_LEVEL  (UART_PS_RX_BUFF_LEN / 4)

// ]]laixf [+] 2008-4-29
// Write (send) GPRS data to Tx buffer for sending to TE.
// <pData> [in] Points to a block of data want to write to Tx buffer.
// <uDataSize> [in] Data size in bytes.
// <RETURN> Returns TRUE when success, else returns FALSE. When it's returns FALSE, you need to send data again.
// extern UINT16 uart_PsSendData(UINT8 *pData, UINT16 uDataSize);
extern void Ip_data_TimeOut();
extern UINT16 uart_PsSendData(UINT8* pData, UINT16 uDataSize);

// laixf [change]  the buffer management style from polling to interrupt;2008-4-29
extern UINT16 uart_PsReadRxData(UINT8* pBuff, UINT16 uBuffSize);

// A group of utility functions to get the left space of Rx and Tx buffer in bytes, for the PS data service.

extern VOID AT_UartInit(UART_CFG* cfg);
extern VOID AT_UartReset(VOID);
extern BOOL AT_UartOpen(HAL_UART_ID_T id, UART_DS_MODE_T DataSpeedMode);
extern BOOL AT_UartReopen(UART_DS_MODE_T DataSpeedMode);
extern BOOL AT_UartClose(UART_ID id);

/*
Now, just support clear Tx buffer.
**/
extern VOID uart_ClearBuffer(VOID);
extern UINT16 uart_SendDataToTe(UINT8* pBuf, UINT16 len);
extern BOOL uart_SendDataPPPCallBack(UINT8* pData, UINT16 size, UINT8 nDLCI);

extern UINT16 SendUIH(UINT8 bAdd, UINT8* pInBuf, UINT16 dwLen, UINT8 bUIHControl);
extern VOID MUX_ParseRecBytes(UINT8* pData, UINT32 nDataLen, UINT8 data_mode);
extern INT32 CFW_PppSendData(UINT8* pData, UINT16 nDataSize, UINT8 MuxId);


/*
NOTES:
	After the calling the following APIs, you must call AT_UartReopen() by yourself in order to enable the new settings.
**/

extern UART_BAUDRATE uart_GetCfgBaudrate(void);
extern BOOL uart_SetCfgBaudrate(UART_BAUDRATE br);
extern void SendCmdToATTask(UINT8* rxData,UINT16 length);


/* Read and Write flag */

typedef struct
{
  INT8 FlagToAT;  // inform At get data from com module
  INT8 FlagToCom; // inform com having written the data
  INT8 RSPFlag; // show the package status
  INT8 FlagToPPP;
} FLAGSTRU;

typedef struct _UART_PS_DATA_BUFF_T
{
  UINT8 DataBuffer[UART_PS_RX_BUFF_LEN];  // Data buffer for upload and download data.
  UINT16 uDataLen;  // Data length in bytes.
  UINT16 uReadPointer;  // Read pointer.
  UINT16 uWritePointer; // Write pointer.
} UART_PS_DATA_BUFF_T;

typedef struct _UART_INFO_T
{
  // TODO: need to confirm.
  FLAGSTRU ComAtflag;
  UART_ID Scom_UartId;
  BOOL CommOpened;  // show uart staus
  INT8 CRvalue_Default; // set CR value
  INT8 LFvalue_Default; // set LF value
  INT8 ReqPackStyp; // data type from the uart
  UINT8 PPPRecStatus;
  UINT8 CheckDataFlag;
  UINT8 Is_data;
  UINT8 padding[2];
  UINT32 for_rx_buff_align32;
  UINT8 rx_buff[AT_CMD_LINE_BUFF_LEN];
  UINT16 rx_buff_p;
  UINT8 data_Mode;
  UART_DS_MODE_T DataSpeedMode;
  //UART_PS_DATA_BUFF_T PsRxBuffer;
  //UART_PS_DATA_BUFF_T PsTxBuffer;

  // Rx timer (start/stop) flag. TRUE: timer is started, FALSE: timer stopped.
  BOOL fPsRxTimer;
} UART_INFO_T;

#define START_RX_TIMER() \
    do { \
        COS_KillTimer(g_hAtTask, UART_PS_RX_TIMER_ID); \
        COS_SetTimer(g_hAtTask, UART_PS_RX_TIMER_ID, COS_TIMER_MODE_SINGLE, UART_PS_TIMER_ELPASE); \
        g_UartInfo.fPsRxTimer = TRUE; \
    } while (0)

#define STOP_RX_TIMER() \
    do { \
        COS_KillTimer(g_hAtTask, UART_PS_RX_TIMER_ID); \
        g_UartInfo.fPsRxTimer = FALSE; \
    } while (0)


// A patch switch. defined as 1, means compatible with <cr><lf> suffix command line; 0 means use the standard format of <cr> suffix.
#define LF_PATCH 1

// the least length of CMD
#define LEAST_LEN1       0x03 // the least length of the request CMD
#define LEAST_LEN2       0x04 // the least length of the request CMD

// packet type
#define SCM_REQ_CMD                         0x05  // only the cmd
#define SCM_REQ_SMS                         0x06  // the SMS contents
#define SCM_GPRS_DATA                       0x07  // the GPRS data, forethougt
#define ERR_SCM_UNKNOWN_TYPE                0x08  // the unknown type

// the CMD
#define AT_CMD_A         0x41 // the ASCII value of char A
#define AT_CMD_a         0x61 // the ASCII value of char a
#define AT_CMD_T         0x54 // the ASCII value of char T
#define AT_CMD_t         0x74 // the ASCII value of char t
#define AT_CMD_ThreePlus 0x2B // special CMD,for example +++<>
#define AT_CMD_SLASH     0x2F // special CMD,for example +++, and so on

#define HDLC_SYN_BYTE    0x7E // the HDLC escape code

// Flag of SCM
#define DATA_TO_AT_CL    0x00 // AT clear flag to clear status
#define DATA_TO_ATE       0x01  // Inform At get the data

// Flag of SCM
#define DATA_TO_PPP_CL   0x00 // AT clear flag to clear status
#define DATA_TO_PPP      0x01 // Inform At get the data

#define DATA_TO_COM      0x01 // Inform Com module data is written
#define DATA_TO_COM_CL   0x00 // Com module clear flag to clear status

#define SCM_STATUS_READ_PREMIT              0x01  // reading is permited
#define SCM_STATUS_WRITE_PREMIT            (0x01 << 1)  // writing is permited
#define SCM_STATUS_BUSY                    (0x01 << 2)  // uart is busy
#define SCM_STATUS_CLOSE                   (0x01 << 3)  // not open the uart

#define CHECK_DATA_OF_GPRS ( 0x01 )
#define CHECK_DATA_OF_FAX  ( 0x01<<1 )

#define UART_SEND_SUCCESS           1
#define UART_SEND_FAIL              2

/*
   Get the data length of a ring buffer.
*/
#define RB_DATA_LEN(buff, len, r, w) ((w) >= (r) ? (w )- (r) : ((len) - (r)) + (w))
#define RB_INC_IDX(len, i) ((i) = ((i) == (len) - 1 ? 0 : (i) + 1))
#define RB_DEC_IDX(len, i) ((i) = ((i) == 0 ? len - 1 : (i) - 1))

#define MOD_BUFF_LEN(_val)  ((_val) & (UART_PS_RX_BUFF_LEN-1))
#define MOD_BUFF_LEN1(_val, _length)  ((_val) & (_length-1))

#define HAL_SYS_GET_UNCACHED_ADDR1(addr) (((UINT32)(addr))|0x20000000)
#define GET_DATA_BLOCK_LEN(_start,_end,_cycle) ((_start<=_end)?(_end-_start):(_end+_cycle - _start) )

typedef struct _UART_SEND_RESULE
{
    UINT32 uSendStatus;
} UART_SEND_RESULE, *PUART_SEND_RESULE, **PPUART_SEND_RESULE;

#define EXTERN extern

#endif
 
