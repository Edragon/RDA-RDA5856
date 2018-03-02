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


//
/// @file
/// This file contains the portion of the module's memory map that will be accessible
/// through CoolWatcher (or any CoolXml tool). It is also used for the get version
/// mechanism.
//
//==============================================================================

#ifndef _AT_MAP_H_
#define _AT_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================
// =============================================================================
//  TYPES
// =============================================================================
typedef enum
{
    AT_DBG_HST_INVAILD              = 0x0,
    AT_DBG_HST_PC_READ_ENABLE       = 0x1,
    AT_DBG_HST_PC_WRITE_ENABLE      = 0x2,
    AT_DBG_HST_HST_READ_ENABLE      = 0x4,
    AT_DBG_HST_HST_WRITE_ENABLE     = 0x8,
} AT_IN_DBG_HST_STATE;
// ============================================================================
// AT_IN_DBG_HST_ACCESS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct
{
    AT_IN_DBG_HST_STATE state;            //0x00000000
    UINT16  rxBufLen;                     //0x00000004
    UINT16  txBufLen;                     //0x00000006
    UINT8*  rxBufPtr;                     //0x00000008
    UINT8*  txBufPtr;                     //0x0000000C
} AT_IN_DBG_HST_ACCESS_T; //Size : 0x10


// ============================================================================
// AT_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of the module.
// =============================================================================
typedef struct
{
    AT_IN_DBG_HST_ACCESS_T *atInDbgHstPtr; //0x00000000
} AT_MAP_ACCESS_T; //Size : 0x4


// =============================================================================
// at_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the module get version function and the
/// CoolWatcher get version command will work.
// =============================================================================
PUBLIC VOID at_RegisterYourself(VOID);



#endif

