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
#ifndef _SMS_DM_LOCAL_H_
#define _SMS_DM_LOCAL_H_

//#include "sms_dm_cd.h"

#define SMS_DM_VERSION                     0x090220      // the version number

extern UINT16 g_SMS_DM_InitFlag;
extern HANDLE g_SMS_DM_Sem;

#ifdef _FS_SIMULATOR_
#define SMS_DM_SemInit()
#define SMS_DM_Down()
#define SMS_DM_Up()

#else

#define SMS_DM_SemInit() do { g_SMS_DM_Sem = COS_CreateSemaphore(1); } while(0)
#define SMS_DM_Down()    do { COS_WaitForSemaphore(g_SMS_DM_Sem, COS_WAIT_FOREVER); } while(0)
#define SMS_DM_Up()        do { COS_ReleaseSemaphore(g_SMS_DM_Sem); } while(0)

#endif

INT32 DM_Read(UINT32 ulAddrOffset, UINT8 *pBuffer, UINT32 ulBytesToRead, UINT32 *pBytesRead);
INT32 DM_Write(UINT32 ulAddrOffset, CONST UINT8 *pBuffer, UINT32 ulBytesToWrite, UINT32  *pBytesWritten);


BOOL  SMS_DM_Existed();
INT32 SMS_DM_Create(VOID);
INT32 SMS_DM_VersionCompare();
INT32 SMS_DM_GetFreeIndex(UINT16 *pIndex, CFW_SIM_ID nSimId);
//Hameina[+] for 221 project, but the normal project will also need this function.
INT32 SMS_DM_GetRange(CFW_SIM_ID nSimId, UINT16 *nStartIndex, UINT16 *nEndIndex);

INT32 SMS_DM_SetRecordStatus(UINT16 iIndex, UINT16 iStatus );
INT32 SMS_DM_GetRecordStatus(UINT16 iIndex, UINT16 *pStatus );
INT32 SMS_DM_GetDatadStatus(UINT16 iIndex, UINT8 *pStatus);
INT32 SMS_DM_ReadRecordData(UINT16 iIndex, PVOID pData);
INT32 SMS_DM_WriteRecordData(UINT16 iIndex, PVOID pData);
INT32 SMS_DM_ReadHeadInfo(SMS_DM_H_INFO *psHeadInfo);
INT32 SMS_DM_WriteHeadInfo(SMS_DM_H_INFO *psHeadInfo);
INT32 SMS_DM_GetFreeRecord(UINT16 *pIndex);
INT32 SMS_DM_MakeLog(UINT16 iIndex, UINT16 iStatus, PVOID pData);
INT32 SMS_DM_ClearLog(VOID);
INT32 SMS_DM_IsLogActive(BOOL *pResult);
INT32 SMS_DM_LogRecove(VOID);

#endif
