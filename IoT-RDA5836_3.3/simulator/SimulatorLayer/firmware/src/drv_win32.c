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
/*      drv_win32.c                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   drivers API form CSW platform.                                           */
/*   See CSW Development Manual for more.                                     */
/*                                                                            */
/******************************************************************************/
#include <assert.h>
#include "cswtype.h"

#include "drv.h"
#include "windows.h"
#include "Tm.h"

PUBLIC BOOL g_halLpsSleptBefore = FALSE;

typedef enum
{
    CAM_FLASH_NONE = 0x00,
    CAM_FLASH_AUTO = 0x01,
    CAM_FLASH_FORCED = 0x02,
    CAM_FLASH_FILLIN = 0x04,
    CAM_FLASH_REDEYEREDUCE = 0x08
} CAM_FLASH_T;

UINT32 lcdd_Open(VOID)
{
}

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


UINT32 SRVAPI DRV_GpioSetDirection(
  UINT8 nGpio,     
  UINT8 nDirection     
){return 1;}

UINT32 DRV_GpioSetMode( 
    UINT8 nGpio,
    UINT8 nMode
){return 1;}

UINT32 SRVAPI DRV_GpioGetDirection(
  UINT8 nGpio,     
  UINT8* pDirection     
);

UINT32 SRVAPI DRV_GpioSetLevel(
  UINT8 nGpio,
  UINT8 nLevel
){return 1;}

UINT32 SRVAPI DRV_GpioGetLevel(
    UINT8 nGgpio,
    UINT8* pLevel
){return 1;}

UINT32 SRVAPI DRV_GpioSetEdgeIndication(
  UINT8 nGpio,
  UINT8 nEdge,
  UINT32 nDebonce
){return 1;}

UINT32 SRVAPI DRV_EXTISetEdgeIndication(
    UINT8  nEdge
);



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
){return 1;}

UINT32 SRVAPI DRV_SetPwl0(
  UINT8 nLevel
){return 1;}
/*************************************************
 *** UART
 *************************************************/



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
UINT32 DRV_SetKeyBacklight( UINT8 nLevel)
{
return 0;
}

void camera_SetFlashL(CAM_FLASH_T Flash)
{
}

void  hal_HstSendEvent(UINT32 level, UINT32 event)
{
    char tch[100]; 

    sprintf(tch, "Detected Event: 0x%08x\n", event);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),tch,strlen(tch),NULL,NULL);
}

#define USER_DATA_FLAG     (0xa53e)
#define USER_DATA_SIZE    (1024)
HANDLE g_hDataFile;
UINT16 data_buff[USER_DATA_SIZE/2];

INT32 DSM_UserDataClear(VOID)
{
    memset(data_buff, 0, USER_DATA_SIZE);
    data_buff[0] = USER_DATA_FLAG;
}

void *DSM_GetUserData(UINT8 index, UINT32 length)
{
    int pos = 2;
    if(data_buff[0] != USER_DATA_FLAG)
    {
        DSM_UserDataClear();
    }

    while(pos < USER_DATA_SIZE/2)
    {
        if(data_buff[pos] == index || data_buff[pos] == 0)
        {
            if(data_buff[pos] == 0)
            {
                data_buff[pos] = index;
                data_buff[pos+1] = length;
                data_buff[pos+((length+3)&~3)/2 + 2] = 0;
            }
            return &data_buff[pos+2];
        }
        pos += ((data_buff[pos+1]+3)&~3)/2 + 2;
    }
    while(1); // user data overflow
}

INT32 DSM_ReadUserData(void)
{
    DWORD cbRead;
    memset(data_buff, 0, USER_DATA_SIZE);
    SetFilePointer(g_hDataFile, 0, NULL, FILE_BEGIN);
    ReadFile(g_hDataFile, data_buff, USER_DATA_SIZE, &cbRead, NULL);
}

INT32 DSM_WriteUserData(void)
{
    DWORD cbRead;
    SetFilePointer(g_hDataFile, 0, NULL, FILE_BEGIN);
    WriteFile(g_hDataFile, data_buff, USER_DATA_SIZE, &cbRead, NULL);

    return 0;
}

#define PHONE_BOOK_LENGTH       81920
HANDLE g_hPhoneDataFile;
UINT8 phone_book_buff[PHONE_BOOK_LENGTH];
UINT16 phone_book_size,item_size,index_size;
UINT16 valid_pb_size = 0;

void DSM_DeletePbFile(void)
{
	DeleteFile("pbap.data");
}

void DSM_Erase_Phonebook(void)
{
    DeleteFile("pbap.data");
}

void DSM_ResetPhoneBookAddress(UINT16 size, UINT8 sizeofItem, UINT8 sizeofIndex)
{
    DWORD cbRead;
    memset(phone_book_buff, 0xff, PHONE_BOOK_LENGTH);
    phone_book_size = size;	
	item_size = sizeofItem;
	index_size = sizeofIndex;

    SetFilePointer(g_hPhoneDataFile, 0, NULL, FILE_BEGIN);
    WriteFile(g_hPhoneDataFile, &phone_book_size, 2, &cbRead, NULL);
    WriteFile(g_hPhoneDataFile, &sizeofItem, 2, &cbRead, NULL);
    WriteFile(g_hPhoneDataFile, &sizeofIndex, 2, &cbRead, NULL);
	WriteFile(g_hPhoneDataFile, &valid_pb_size, 2, &cbRead, NULL);
    WriteFile(g_hPhoneDataFile, phone_book_buff, PHONE_BOOK_LENGTH, &cbRead, NULL);
}

VOID DSM_GetPhoneBookParam(UINT32 *length, UINT16 *size, UINT16 *valid_size)
{
    DWORD cbRead;
    memset(phone_book_buff, 0xff, PHONE_BOOK_LENGTH);
    SetFilePointer(g_hPhoneDataFile, 0, NULL, FILE_BEGIN);
    ReadFile(g_hPhoneDataFile, &phone_book_size, 2, &cbRead, NULL);
    ReadFile(g_hPhoneDataFile, &item_size, 2, &cbRead, NULL);
    ReadFile(g_hPhoneDataFile, &index_size, 2, &cbRead, NULL);
	ReadFile(g_hPhoneDataFile, &valid_pb_size, 2, &cbRead, NULL);
    ReadFile(g_hPhoneDataFile, phone_book_buff, PHONE_BOOK_LENGTH, &cbRead, NULL);
    
    if(length)
        *length = PHONE_BOOK_LENGTH;
    if(size)
        *size = phone_book_size;
}

void *DSM_GetPhoneBookAddress()
{
	return phone_book_buff;
}
int DSM_GetPhoneBookOffset(void)
{
	return phone_book_size*index_size;
}

int DSM_GetValidPbOffset(void)
{
	return valid_pb_size*index_size;
}

void DSM_WritePhoneBook(UINT8 *data, UINT16 index)
{
    DWORD cbRead;
    SetFilePointer(g_hPhoneDataFile, 8+phone_book_size*index_size+index*item_size, NULL, FILE_BEGIN);
    WriteFile(g_hPhoneDataFile, data, item_size, &cbRead, NULL);
    memcpy(phone_book_buff+phone_book_size*index_size+index*item_size, data, item_size);
}

void DSM_WritePhoneBookIndex(UINT8 *index, UINT16 size)
{
    DWORD cbRead;
	valid_pb_size = size;
	SetFilePointer(g_hPhoneDataFile, 6, NULL, FILE_BEGIN);
	WriteFile(g_hPhoneDataFile, &size, 2, &cbRead, NULL);
    SetFilePointer(g_hPhoneDataFile, 8, NULL, FILE_BEGIN);
    WriteFile(g_hPhoneDataFile, index, size*index_size, &cbRead, NULL);
    memcpy(phone_book_buff, index, size*index_size); 
}


VOID DSM_Upgrade_Flash(INT file)
{
    FS_Close(file);
    hal_SysRestart();
}

VOID hal_AifSelectI2SPath(UINT8 isOutputFM)
{
}

PUBLIC VOID uctls_Close(VOID)
{
}

PUBLIC VOID pmd_SetLevel(int id, UINT32 level)
{
}

PUBLIC VOID pmd_SetPowerMode(int powerMode)
{
}

PUBLIC UINT32 platform_VerGetRevision(void)
{
    return 1;
}

PUBLIC UINT32 platform_VerGetNumber(void)
{
    return 0x0200;  // 2.0
}

PUBLIC UINT32 platform_VerGetDate(void)
{
    return BUILD_DATE;
}

static const char* APP_LOGLEVEL[]=
{
    "Main",	
    "GUI",
    "BT",
    "Music",	
    "Record",
    "FM",
    "Setting",
    "Media",
    "8",
    "9",
    "Multi_main",
    "Multi_AudMain",
};

static win32_printf(char *pre_str, char *buff)
{
	char tch[1024] = {0};
	
	TM_SYSTEMTIME systemTime;
	
	TM_GetSystemTime(&systemTime);
	sprintf(tch, "%2d:%2d:%3d %s%s\n",
					systemTime.uMinute,
					systemTime.uSecond,
					systemTime.uMilliseconds,
					pre_str, buff);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),tch,strlen(tch),NULL,NULL);
}


void app_trace(UINT16 level, char* fmt,...)
{
      char tch[1024];
      char buff[1024];
      va_list var;

	  if(level < 1 || level > (sizeof(APP_LOGLEVEL)/sizeof(APP_LOGLEVEL[0])))
		  return; // error level
      va_start(var, fmt);
      vsprintf(tch,fmt,var);
      va_end(var);

      sprintf(buff, "[APP][%s]:", APP_LOGLEVEL[level-1]);

      win32_printf(buff, tch);
}

void debug_assert(BOOL condition, char* fmt,...)
{
	if(!condition)
	{
      char tch[1024];
      va_list var;

      va_start(var, fmt);
      vsprintf(tch,fmt,var);
      va_end(var);


      win32_printf("[ASSERT]",tch);
      
      assert(0);
   }

}

void trace_null(UINT16 level, char* fmt,...)
{
	  return;
}


void lwip_trace(char* fmt,...)
{
	char tch[1024];
	va_list var;

	va_start(var, fmt);
	vsprintf(tch,fmt,var);
	va_end(var);


	win32_printf("[Lwip]",tch);
}


void win32_trace(char* fmt,...)
{
	char tch[1024];
	va_list var;

	va_start(var, fmt);
	vsprintf(tch,fmt,var);
	va_end(var);


	win32_printf("",tch);
}


int tsd_Calib(UINT start)
{
	return 0;
}

UINT8 g_usb_active_after_bt;

int hal_UsbGetMode(void)
{
	return 0;
}

void SetMicVal(UINT32 val)
{

}

UINT8 tgt_GetKeyCode(UINT8 key)
{
	return 0;
}

INT32 Audio_SBCStop (void) 	
{
	return 0;
}

BOOL BTSco_GetStatus(void)
{
	return FALSE;
}

UINT8 umss_GetUsbVirtualStatus()
{
	return 0;
}

void hal_TimWatchDogKeepAlive(void)
{
}

BOOL g_test_mode = FALSE; 
BOOL boot_get_testmode(VOID); 
VOID boot_set_testmode(BOOL mode); 
BOOL boot_get_testmode(VOID)
{
  return g_test_mode; 
}
VOID boot_set_testmode(BOOL mode)
{
  g_test_mode = mode; 
}

PUBLIC VOID hal_AudSetAdcVolume(UINT32 vol)
{

}

PUBLIC void hal_SysSetVocClockDefault(void)
{

}

PUBLIC VOID hal_AudSetDacVolume(UINT32 vol)
{

}

INT32 DSM_DeactiveDevice(PCSTR pszDevName)
{

}