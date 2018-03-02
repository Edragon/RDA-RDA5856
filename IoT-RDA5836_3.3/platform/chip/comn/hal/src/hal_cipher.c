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
/// @file hal_cipher                                                          //
/// HAL cipher command driver                                                 //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 

#include "cs_types.h"

#include "global_macros.h"
#include "cipher.h"

#include "hal_cipher.h"
#include "hal_error.h"

// =============================================================================
// hal_CipherKcInit
// -----------------------------------------------------------------------------
/// Initialize Kc
/// @param kcLow   Low part of the Kc Key
/// @param kcHigh  High part of the Kc Key
/// @return #HAL_ERR_RESOURCE_BUSY if the ciphering is already running or 
/// #HAL_ERR_NO if the initialization is properly done.
// =============================================================================

PUBLIC UINT8 hal_CipherKcInit(UINT32 kcLow,    UINT32 kcHigh)
{
    UINT32 error;

    //  Check if the cipher A5 is already processing 
    error = hwp_cipher->status & CIPHER_RUNNING;
    if (error) 
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
    else 
    {
        //  Write low bits of Kc register 
        hwp_cipher->Kc_low = kcLow;
        //  Write high bits of Kc register 
        hwp_cipher->Kc_high = kcHigh;
        return HAL_ERR_NO;
    }
}
