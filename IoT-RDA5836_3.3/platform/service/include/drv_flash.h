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

#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_

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

BOOL DRV_FlashInit(
VOID
);

UINT32 DRV_FlashRegionMap(
  DRV_FLASH_DEV_INFO *pParam
);

UINT32 DRV_EraseFlashSector (
  UINT32 ulAddrOffset
);

UINT32 DRV_EraseAllUserDataSector(
    VOID
 );
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

#if 0
UINT32 SRVAPI DRV_EraseFlash (
  UINT32 ulAddrOffset
);
#endif

UINT32 DRV_FlashLock(
  UINT32 ulStartAddr,
  UINT32 ulEndAddr
);

UINT32 DRV_FlashUnLock(
  UINT32 ulStartAddr,
  UINT32 ulEndAddr
);

VOID DRV_FlashSetCtrRegionSize( 
  UINT32 CtrSize 
);
UINT32 DRV_FlashGetCtrRegionSize(
    VOID
);

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
UINT32 DRV_EraseFlashSectorResume(
    UINT32 ulAddrOffset,
    UINT32 uTime
    );
UINT32 DRV_EraseFlashSectorFirst(
    UINT32 ulAddrOffset,
    UINT32 uTime
    );

#define ERR_FLASH_ERASE_SUSPEND           -101
#endif

#endif // _H_

