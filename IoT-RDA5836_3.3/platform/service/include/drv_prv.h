/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    drv_prv.h                                                     */
/*                                                                            */
/* Description:                                                               */
/*    DM service declarations, constant definitions and macros                */
/*                                                                            */
/******************************************************************************/
#ifndef _DRV_PRV_H_
#define _DRV_PRV_H_

//
// Flash Drivers 
//
/*mabo deleted,20070813
typedef enum _DRV_FLASH_MODEL{
	flash_s64=1,
	flash_s127nb,
	flash_s127jb,
	flash_s032,
	flash_sst34,
	flash_k5a6317ctm
}DRV_FLASH_MODEL;
*/
/*
typedef struct _DRV_FLASH_DEV_INFO {
  UINT32 block_size;
  UINT32 sector_size;
  UINT32 total_size;
  BYTE* pBaseAddr;
}DRV_FLASH_DEV_INFO; 

UINT32 SRVAPI DRV_FlashPowerUp (
  VOID
);

UINT32 SRVAPI DRV_FlashPowerDown (
  VOID
);

UINT32 DRV_FlashRegionMap(
  DRV_FLASH_DEV_INFO *pParam
);

UINT32 DRV_EraseFlashSector (
  UINT32 ulAddrOffset
);
*/
/*
UINT32 SRVAPI DRV_ReadFlash (
  UINT32 ulAddrOffset,
  BYTE* pBuffer,
  UINT32 nNumberOfBytesToRead,
  UINT32* pNumberOfBytesRead
);

UINT32 SRVAPI DRV_WriteFlash (
  UINT32 ulAddrOffset,
  CONST BYTE* pBuffer,
  UINT32 nNumberOfBytesToWrite,
  UINT32* pNumberOfBytesWritten
);
*/
#if 0
UINT32 SRVAPI DRV_EraseFlash (
  UINT32 ulAddrOffset
);
#endif
/*
UINT32 DRV_FlashLock(
  UINT32 ulStartAddr,
  UINT32 ulEndAddr
);

UINT32 DRV_FlashUnLock(
  UINT32 ulStartAddr,
  UINT32 ulEndAddr
);
*/
//
// RTC Time
//
#define DRV_CLOCKS_PER_SECOND   256

UINT32 SRVAPI DRV_GetRtcClock(VOID);
UINT32 SRVAPI DRV_SetRtcClock(UINT32 ulClock);


//
// Audio
//
UINT32 SRVAPI DRV_SndPlay(BYTE* pBuffer, UINT32 nSize);


//
// Serial 
//
UINT32 SRVAPI DRV_WriteSerial (
  UINT8 nPort,
  UINT8* pBuffer,
  UINT16 nBufferLen
);

UINT32 SRVAPI DRV_WriteSerial2 (
  UINT8 nPort,
  UINT8* pBuffer,
  UINT16 nBufferLen
);


#define DRV_GPIO_MAX                32
#define DRV_GPIO_INT_LAST			15
#define DRV_GPIO_INT_FIRST			8

#endif // _H_

