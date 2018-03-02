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
#ifndef _VDS_CACHE_H_
#define _VDS_CACHE_H_

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)


// error code define.
// vds uninit.
#define ERR_VDS_CACHE_VDS_UNINIT                 -1

// malloc failed.
#define ERR_VDS_CACHE_VDS_MALLOC_FIALED          -2

// read failed.
#define ERR_VDS_CACHE_READ_FAILED                -3

// write failed.
#define ERR_VDS_CACHE_WRITE_FAILED               -4

// cache node is null.
#define ERR_VDS_CACHE_NODE_NULL                  -5

// vds cache uninit.
#define ERR_VDS_CACHE_UNINIT                     -6

// unallowed flush
#define ERR_VDS_CACHE_UNALLOWED_FLUSH            -7

//// no more free pb
#define _ERR_VDS_CACHE_NO_MORE_FREE_PB           -8

#define ERR_VDS_CACHE_GCING                                  -9
#define ERR_VDS_FLUSH_TIMEOUT                              -10

INT32 VDS_CacheInit(VOID);
INT32 VDS_CacheWriteBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8*pBuff );
INT32 VDS_CacheReadBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8* pBuff );
INT32 VDS_CacheRevertBlock( UINT32 iPartId, UINT32 iBlkNr);

VOID VDS_CacheFlush(VOID);
VOID VDS_CacheFlushAll(VOID);
VOID VDS_CacheFlushAllDirect(UINT32 iTime);
VOID VDS_FlushCacheAllFly(VOID);

VOID VDS_CacheFlushAllowed(VOID);
BOOL VDS_CacheNodeIsLess(VOID);
VOID VDS_CacheFlushDisable(UINT32 iKeepTime);
VOID VDS_CacheFlushEnable(VOID);
UINT32 VDS_CacheGetBlkCnt(VOID);


#else


#endif
#endif

