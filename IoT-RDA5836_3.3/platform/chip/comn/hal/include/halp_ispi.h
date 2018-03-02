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
/// @file hal_ispi.c                                                         //
/// Implementation of the Internal ISPI romed Driver.                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_ISPI_H_
#define _HALP_ISPI_H_

#include "cs_types.h"

#include "chip_id.h"

#if (CHIP_ISPI_CS_QTY != 0) || (CHIP_SPI3_CS_QTY != 0)
#include "hal_ispi.h"
#include "hal_sys.h"

// =============================================================================
// hal_IspiUpdateDivider
// -----------------------------------------------------------------------------
/// Update the ISPI divider so that the maximum frequency defined is not
/// overpassed.
/// 
/// @param sysFreq Newly applied system frequency
// =============================================================================
PROTECTED VOID hal_IspiUpdateDivider(HAL_SYS_FREQ_T sysFreq);


#endif // CHIP_SPI3_CS_QTY

#endif // _HALP_ISPI_H_


