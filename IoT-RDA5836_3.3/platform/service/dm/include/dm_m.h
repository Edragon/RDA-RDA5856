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


#ifndef _DM_M_H_
#define _DM_M_H

#include "cs_types.h"
#include "cos.h"

PUBLIC BOOL dm_Init(VOID);
PUBLIC UINT8 dm_GetStdKey(UINT8 key);
PUBLIC UINT8 DM_BuzzerSongMsg(COS_EVENT* pEvent);

#endif // _DM_M_H_



