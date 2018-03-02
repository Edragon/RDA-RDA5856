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


#ifndef _TGT_EXPKEY_CFG_H_
#define _TGT_EXPKEY_CFG_H_

#include "cs_types.h"
#include "hal_gpio.h"


// =============================================================================
// TGT_CFG_EXPKEY_T
// -----------------------------------------------------------------------------
/// This structure describes the expand key configuration for a given target.
/// The first field identify if expand key is used.
/// The second and third field is the mapped pin when expand key flag is set.
// =============================================================================
typedef struct
{
    /// \c TRUE if expand key is used
    BOOL            expKeyUsed;
    /// The mapped GPO when expKeyUsed is true
    HAL_APO_ID_T    expKeyOut6;
    /// The mapped GPO when expKeyUsed is true
    HAL_APO_ID_T    expKeyOut7;
} TGT_CFG_EXPKEY_T;


// =============================================================================
// tgt_GetExpKeyConfig
// -----------------------------------------------------------------------------
/// This function is used by HAL to get access to expand key configuration.
// =============================================================================
PUBLIC CONST TGT_CFG_EXPKEY_T* tgt_GetExpKeyConfig(VOID);


#endif // _TGT_CAMS_CFG_H_

