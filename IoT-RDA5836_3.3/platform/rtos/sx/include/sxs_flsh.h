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

/*
================================================================================
  File         sxs_flsh.h
--------------------------------------------------------------------------------

  Scope      : Flash memory handler header file

  History    :
--------------------------------------------------------------------------------
 Aug 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __SXS_FLSH_H__
#define __SXS_FLSH_H__

#include "sxs_type.h"
#include "sxr_cnf.h"

#define SXS_FLH_VALID_ID        (1 << 30)
#define SXS_FLH_STACKED_BLOCK   (1 << 29)
#define SXS_FLH_STATIC_BLOCK    (1 << 28)
#define SXS_FLH_PROTECTED_BLOCK (1 << 27)

//#define SXS_FLH_EXTRA_BYTE      (1 << 26)

#define SXS_FLH_EXTRA_BYTE_POS       24
#define SXS_FLH_EXTRA_BYTE_MSK       (3 << SXS_FLH_EXTRA_BYTE_POS)
#define SXS_FLH_SET_EXTRA_BYTE(n)  (((n) & 3)  << SXS_FLH_EXTRA_BYTE_POS)
#define SXS_FLH_GET_EXTRA_BYTE(Id) (((Id) >> SXS_FLH_EXTRA_BYTE_POS) & 3)

#define SXS_FILE_IDX_POS        16
#define SXS_FILE_IDX_MSK        (0xFF << SXS_FILE_IDX_POS)
#define SXS_OFFSET_POS          0
#define SXS_OFFSET_MSK          (0xFFFF << SXS_OFFSET_POS)

#define SXS_GET_FILE_IDX(Id)    ((Id & SXS_FILE_IDX_MSK) >> SXS_FILE_IDX_POS)
#define SXS_SET_FILE_IDX(Idx)   ((Idx << SXS_FILE_IDX_POS) & SXS_FILE_IDX_MSK)


u8    sxs_FlashHandlerInit (void);
void  sxs_FlashPurge (u8 Protect, u8 UpdtStatic);
u8    sxs_AddressToSector (void *Address);
void *sxs_SectorToAddress (u8 SectIdx);
void  sxs_SectorErase (u8 Sector);
void  sxs_FlashWrite (u16 *Dest, u16 *Src, u16 Length);
u8    sxs_FlashBlockWrite (u32 Id, u8 *Data, u16 Size);
u8   *sxs_FlashBlockRead (u32 Id, u8 *Data, u16 Size);
void  sxs_GetFilesLimit (u32 *Start, u32 *End);
void  sxs_FlashBlockDelete (u32 Id, u32 Msk, u8 Protect);
void  sxs_UpdateStaticBank (void);
#endif

