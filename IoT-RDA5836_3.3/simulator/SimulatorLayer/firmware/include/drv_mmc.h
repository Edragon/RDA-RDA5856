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
#define DRV_DEF_CACHE_CNT 4

INT32 DRV_OpenTFlash( PCSTR pszDevName, UINT32* pDevHandle );
INT32 DRV_CloseTFlash(PCSTR pszDevName);
INT32 DRV_ReadTFlash(UINT32 dev_handle, UINT32 blk_nr, BYTE* pBuffer);
INT32 DRV_WriteTFlash(UINT32 dev_handle, UINT32 blk_nr, CONST BYTE* pBuffer);
INT32 DRV_GetDevInfoTFlash( UINT32 dev_handle, UINT32 *pBlockNr, UINT32 *pBlockSize );
VOID  DRV_DeactiveTFlash(UINT32 dev_handle);
INT32 DRV_ActiveTFlash(UINT32 dev_handle);
VOID DRV_SetStatusSleepInTFlash(BOOL bStatus);
UINT32 DRV_GetRCacheSizeTFlash(HANDLE dev_handle);
VOID DRV_SetRCacheSizeTFlash(HANDLE dev_handle,UINT32 cache_size);
UINT32 DRV_GetWCacheSizeTFlash(HANDLE dev_handle);
VOID DRV_SetWCacheSizeTFlash (HANDLE dev_handle,UINT32 cache_size);
INT32 DRV_FlushTFlash(VOID);

#endif // _H_

