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

////////////////////////////////////////////////////////////////////////////////
// FILENAME: BTSCO.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BTSCO_H
#define BTSCO_H

PUBLIC VOIS_ERR_T BTSco_Start(CONST AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg);

PUBLIC VOIS_ERR_T BTSco_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg);

PUBLIC VOIS_ERR_T BTSco_RecordStart( INT32 file);

PUBLIC VOIS_ERR_T BTSco_Stop(VOID);

VOID MMC_SCOReceiveData(INT32 length);


#endif


