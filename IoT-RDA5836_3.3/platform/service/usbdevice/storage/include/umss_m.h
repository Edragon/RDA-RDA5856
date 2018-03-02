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
////////////////////////////////////////////////////////////////////////////////
//
/// @file umss_m.h
///
/// USB Mass Storage Service
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UMSS_M_H_
#define _UMSS_M_H_

#include "cs_types.h"

#include "hal_usb.h"

#include "umss_config.h"

/// @file umss_m.h
/// @mainpage USB Mass Storage service
///
/// This service provides Mass Storage service on usb
///
/// This document is composed of:
/// - @ref umss
///
/// @addtogroup umss USB Mass Storage Service (umss)
/// @{
///

// =============================================================================
// MACROS                                                                       
// =============================================================================

#ifndef UMSS_STORAGE_MAX
#define UMSS_STORAGE_MAX 4
#endif /* UMSS_STORAGE_MAX */

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// FUNCTIONS
// =============================================================================

typedef INT32 (*UFLASH_WRTBLK_FUNC_P)( UINT32 iPartId, UINT32 iVBNum, UINT8*pBuff );
typedef INT32 (*UFLASH_RDBLK_FUNC_P)( UINT32 iPartId, UINT32 iVBNum, UINT8* pBuff );

void init_uFlash_Func (UFLASH_WRTBLK_FUNC_P  pf_wrtblk_func,  
            UFLASH_RDBLK_FUNC_P  pf_rdblk_func);
void init_uFlash_Param(UINT32 devNo, UINT32 nrBlock, UINT32 blkSize);


PUBLIC UINT8 umss_GetUsbVirtualStatus();

PUBLIC UINT8 umss_ResetUsbVirtualStatus();

// =============================================================================
// umss_Open
// -----------------------------------------------------------------------------
/// Open USB mass storage service
PUBLIC VOID umss_Open(void);

// =============================================================================
// umss_AddPart
// -----------------------------------------------------------------------------
/// Add a part to USB mass storage service
/// @param storageCallback callback function for this storage.
PUBLIC  VOID umss_AddPart(  UMSS_STORAGE_CALLBACK_T *storageCallback);//shaohuima 20160708

// =============================================================================
// umss_Run
// -----------------------------------------------------------------------------
/// Run USB mass storage service
PUBLIC VOID umss_Run(void);

// =============================================================================
// umss_Close
// -----------------------------------------------------------------------------
/// Close USB mass storage service
PUBLIC VOID umss_Close(void);


///  @} <- End of the umss group

#endif // _UMSS_M_H_
