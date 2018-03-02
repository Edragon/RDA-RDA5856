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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_cipher.h                                                        //
/// This file contains HAL cipher command driver API.                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_SHAREMEM_H_
#define _HAL_SHAREMEM_H_

/// @defgroup cipher HAL Ciphering Driver
///
/// @{
///

#include "cs_types.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_CipherKcInit
// -----------------------------------------------------------------------------
/// Initialize Kc
/// @param kcLow   Low part of the Kc Key
/// @param kcHigh  High part of the Kc Key
/// @return #HAL_ERR_RESOURCE_BUSY if the ciphering is already running or 
/// #HAL_ERR_NO if the initialization is properly done.
// =============================================================================

PUBLIC void hal_ShareMemInit(void);


PUBLIC UINT16 *hal_ShareMem_GetHeader(void);

PUBLIC UINT8 *hal_ShareMem_GetData(void);
PUBLIC void hal_ShareMem_NextData(UINT32 len);
PUBLIC UINT8 hal_ShareMem_GetRxPage(void);
PUBLIC UINT8 hal_ShareMem_GetTxPage(void);


PUBLIC void hal_ShareMem_SendHeader(UINT16 *header);

PUBLIC void hal_ShareMem_SendData(UINT8 *data, UINT32 len);

void hal_Sharemem_Interrupt_Handle(void);

/// @} <-- End of the cipher group

#endif // _HAL_SHAREMEM_H_


