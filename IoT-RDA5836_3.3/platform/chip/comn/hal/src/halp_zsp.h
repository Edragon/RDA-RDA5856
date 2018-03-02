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
///     @file hal_voc_irq.h
///     Definition of the private IRQ functions of Garnet's VoC Driver
///     Caution: This file is strictly confidential!!!
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef  _HALP_ZSP_IRQ_H_
#define  _HALP_ZSP_IRQ_H_


#include "cs_types.h"

// ============================================================================
// TYPES
// ============================================================================



// ============================================================================
// HAL_ZSP_STATE_T
// ----------------------------------------------------------------------------
/// Describes the VoC state.
// ============================================================================
typedef enum
{
    HAL_ZSP_CLOSE_DONE,
    HAL_ZSP_CLOSE_STARTED,
    HAL_ZSP_OPEN_STARTED,
    HAL_ZSP_OPEN_IN_PROGRESS,
    HAL_ZSP_OPEN_FAILED,
    HAL_ZSP_OPEN_DONE,
} HAL_ZSP_STATE_T;

extern HAL_ZSP_CFG_T   ZSP_DTCM_SHARE_MEM g_zspCfg;
// ============================================================================
// FUNCTIONS
// ============================================================================

// ============================================================================
// hal_VocIrqHandler
// ----------------------------------------------------------------------------
/// VoC IRQ handler, clearing the IRQ cause regiter and calling the user handler
/// defined by hal_VocIrqSetHandler. It also checks if the open procedure is finished
/// when the user chooses to confirm the end of the open procedure with an VoC DMA 
/// interrupt
/// @param interruptId Id with which the handler is entered
// ============================================================================
PROTECTED VOID hal_ZspIrqHandler(UINT8 interruptId);


#endif //  HAL_ZSP_IRQ_H 

