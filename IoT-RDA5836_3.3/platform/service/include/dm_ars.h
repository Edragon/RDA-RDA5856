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

#if !defined(__DM_ARS_H__)
#define __DM_ARS_H__

#include "ars_m.h"


BOOL DM_ArsSetup(CONST ARS_AUDIO_CFG_T* cfg);
BOOL DM_ArsRecord(CONST ARS_ENC_STREAM_T* stream, CONST ARS_AUDIO_CFG_T* cfg, BOOL loop);
BOOL DM_ArsPause(BOOL pause);
BOOL DM_ArsStop(VOID);

UINT32* DM_ArsGetBufPosition(VOID);
BOOL DM_ArsReadData(UINT32 addedDataBytes);
UINT32 DM_ArsAvailableData(VOID);

#endif // __DM_ARS_H__

