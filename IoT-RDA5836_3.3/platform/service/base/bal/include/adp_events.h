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

#ifndef _ADP_EVENTS_H
#define _ADP_EVENTS_H
#include "ap_message.h"

#if defined(WIN32)
#define __FUNCTION__  __FILE__
#endif /* WIN32 */
typedef BOOL ( *PFN_TCPIP_CALLBACK )( COS_EVENT *pEV );


extern UINT32 CFW_SetTCPIPCallBackEx ( PFN_TCPIP_CALLBACK pfnEv, UINT8 SocketId );


#define ADP_MAX_EVENT 500									

typedef BOOL (*AdpEvHandlerPtr)(COS_EVENT *);

typedef struct _ADP_EVENT_INFO_T
{
	UINT32 EventID;
	AdpEvHandlerPtr EntryFuncPtr;
} ADP_EVENT_INFO_T;

extern BOOL ADP_EventHandler(COS_EVENT *MsgStruct);
#endif

