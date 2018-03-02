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
  File         sxs_spy.h
--------------------------------------------------------------------------------

  Scope      : Spy window header file.

  History    :
--------------------------------------------------------------------------------
 Aug 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __SXS_SPY_H__
#define __SXS_SPY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sxs_type.h"

typedef struct
{
 u8  SpyIdx;
 u8  Id;
 u16 Data;
} sxs_SpyInfo_t;

typedef struct
{
 u16 Id;
 s16 Data;
} sxs_SpyData_t;


#define SXS_SPY_NAME_ID     0
#define SXS_SPY_STR_ID      1
#define SXS_SPY_DATA_ID     2
#define SXS_SPY_ENABLE_ID   3
#define SXS_SPY_MULTI_ID    4

#define SXS_NB_MAX_SPY      32

void sxs_InitSpy(void);
u8   sxs_NewSpy  (const ascii *Name, u8 UsrId, const ascii *const *Str, s16 *Data, u8 Size);
void sxs_SpyData (void);
void sxs_SpyStr  (u8 Idx);
void sxs_SpyEnable (u8 SpyIdx, u8 Enable);
void sxs_UsrSpyEnable (u8 UsrId, u8 Enable);
void sxs_SpyDispatchRegistration (u8 UsrId, void (*DispatchFunction)(u8, u8, u16 *), u32 DispatchFields, u8 DispatchDataOffset);

#ifdef __cplusplus
}
#endif

#endif

