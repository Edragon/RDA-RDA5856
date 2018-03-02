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
/// @file  
/// That file describes the configuration of the structure used to set up
///  
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIOI2C_CONFIG_H_
#define _GPIOI2C_CONFIG_H_

#include "cs_types.h"

#include "gpio_i2c.h"
#include "hal_gpio.h"



// =============================================================================
//  
// -----------------------------------------------------------------------------
///
/// Make sure to include gpioic_config.h before  
// =============================================================================

#define GPIOI2C_CONFIG_STRUCT_T GPIOI2C_CONFIG_STRUCT_T



// =============================================================================
//  
// -----------------------------------------------------------------------------
///  
// ============================================================================



struct GPIOI2C_CONFIG_STRUCT_T
{
    /// I2C bus used to configure the chip
    GPIO_I2C_BPS_T           i2c_gpio_Bps;
    
    HAL_GPIO_GPIO_ID_T      scl_i2c_gpio;  // if do not used the gpio SCL ,set it HAL_GPIO_NONE

    HAL_GPIO_GPO_ID_T       scl_i2c_gpo;   // if do not used gpo for SCL ,set it HAL_GPO_NONE
    
    HAL_GPIO_GPIO_ID_T      sda_i2c;    
};

#endif // 


