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
//                                                                            //
//                                                                            //
/// @file btd_config.h
/// That file describes the configuration of the structure used to set up
/// the BTD structure stored in TGT for blue-tooth driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "hal_i2c.h"
#include "hal_gpio.h"


#ifndef _BTD_CONFIG_H_
#define _BTD_CONFIG_H_

// =============================================================================
// BTD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// The type BTD_CONFIG_STRUCT_T is defined in tgt_btd_cfg.h using the structure
/// #BTD_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
// =============================================================================
#define BTD_CONFIG_STRUCT_T BTD_CONFIG_RDABT_STRUCT_T


// =============================================================================
// BTD_CONFIG_ASC3600_STRUCT_T
// -----------------------------------------------------------------------------
/// Describes the board specific configuration of the ASC3600 BT chip
// ============================================================================
struct BTD_CONFIG_RDABT_STRUCT_T
{
    HAL_I2C_BUS_ID_T i2cBusId;
    HAL_APO_ID_T pinReset;
    HAL_APO_ID_T pinWakeUp;
    HAL_APO_ID_T pinSleep;
};


// =============================================================================
//  FUNCTIONS
// =============================================================================

///  @} <- End of the btd group 

#endif // _BTD_CONFIG_H_


