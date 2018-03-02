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




#ifndef _BAL_CONFIG_H_
#define _BAL_CONFIG_H_

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// BAL_CFG_KEY_T
// -----------------------------------------------------------------------------
/// This structure describes the key configuration for a given target.
// =============================================================================
typedef struct
{
    UINT16 repeat_timeout;       // time to start repeat the key, in millisecond
    UINT16 repeat_interval;      // time of repeat the key, in millisecond
    UINT16 doubleclick_internal;
    CONST UINT8  *keymap;
} BAL_CFG_KEY_T;


// =============================================================================
// BAL_CFG_IRC_T
// -----------------------------------------------------------------------------
/// This structure describes the IRC configuration for a given target.
// =============================================================================
typedef struct
{
    UINT8  irc_styles;       // irc styles
    UINT8  map_size;         // key map size
    UINT8  timeout;          // wait timeout for one press
    CONST UINT16 *style_list;      // style code list
    CONST UINT8  *code_map;        // key map list
} BAL_CFG_IRC_T;



PUBLIC CONST BAL_CFG_KEY_T* tgt_GetKeyCfg(VOID);
PUBLIC CONST BAL_CFG_IRC_T* tgt_GetIRCCfg(VOID);


#endif // _BAL_CONFIG_H_


