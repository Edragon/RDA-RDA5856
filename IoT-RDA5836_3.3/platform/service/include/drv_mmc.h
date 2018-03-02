/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    drv_mmc.h                                                     */
/*                                                                            */
/* Description:                                                               */
/*    DM service declarations, constant definitions and macros                */
/*                                                                            */
/******************************************************************************/
#ifndef _DRV_MMC_H_
#define _DRV_MMC_H_

// Default sector size.
#define MMC_DEF_SEC_SIZE 512

// Defult t-flash dev cache block count.
#define DRV_DEF_CACHE_BLOCK_CNT 4

// t-flash dev name size.
#define DRV_DEV_NAME_SIZE 16


INT32 DRV_OpenTFlash(UINT8* dev_name,HANDLE* pdev_handle);
INT32 DRV_CloseTFlash(HANDLE dev_handle);
INT32 DRV_ReadTFlash(HANDLE dev_handle, UINT64 blk_nr, BYTE *pBuffer);
INT32 DRV_WriteTFlash(HANDLE dev_handle, UINT64 blk_nr, CONST BYTE *pBuffer);
INT32 DRV_GetDevInfoTFlash( HANDLE dev_handle, UINT64 *pBlockNr, UINT32 *pBlockSize );
VOID  DRV_DeactiveTFlash(HANDLE dev_handle);
INT32 DRV_ActiveTFlash(HANDLE dev_handle);
VOID DRV_SetStatusSleepIn(BOOL bStatus);
UINT32 DRV_GetRCacheSizeTFlash(HANDLE dev_handle);
UINT32 DRV_GetWCacheSizeTFlash(HANDLE dev_handle);
VOID DRV_SetWCacheSizeTFlash(HANDLE dev_handle,UINT32 cache_size);
VOID DRV_SetRCacheSizeTFlash(HANDLE dev_handle,UINT32 cache_size);
INT32 DRV_FlushTFlash(HANDLE dev_handle);
#define ERR_TFLASH_READ_FAILED                          -104
#define ERR_TFLASH_WRITE_FAILED                         -105

#define ERR_DRV_DEVICE_NOT_FOUND                     -108
#define ERR_TFLASH_DEV_INVALID_HANDLE            -110
#define ERR_TFLASH_DEV_BUSY                                -111
#define ERR_TFLASH_DEV_BLK_OVERFLOW               -112

#endif  // _H_

