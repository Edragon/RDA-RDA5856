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


#ifndef _RCM_H_
#define _RCM_H_

BOOL SRVAPI RCM_FlashGetSpace(UINT8 nModule, UINT32*pStart, UINT32* pSize);
BOOL SRVAPI RCM_GetRamSpace(UINT32*pRamAddr, UINT32* pRamSize);
BOOL SRVAPI RCM_ReleaseRamSpace(VOID);
BOOL SRVAPI RCM_IsValidHeap(PVOID p); // Check the addres is valid heap.

//
//add by lixp
//

typedef struct _RCM_PBK_CAP {
    UINT32	m_PbkMinNum;      // size in byte
	UINT32	m_PbkMcMaxNumber; // size in byte
	UINT32	m_PbkRcMaxNum;    // size in byte
	UINT32	m_PbkLdNum;       // size in byte.
}RCM_PBK_CAP;

typedef struct _RCM_SMS_CAP {
    UINT32	m_SmsMaxNum;    // size in byte.
	UINT32	m_SmsCbMaxNum;  // size in byte.
	UINT32	m_SmsSrMaxNum;  // size in byte.
}RCM_SMS_CAP;

UINT32 SRVAPI RCM_GetPbkCap(RCM_PBK_CAP* pPbkCap ) ;
UINT32 SRVAPI RCM_GetSmsCap(RCM_SMS_CAP* pSmsCap ) ;


#endif // _H_

