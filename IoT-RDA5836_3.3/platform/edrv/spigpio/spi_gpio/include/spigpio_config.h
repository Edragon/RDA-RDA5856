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
*/                                                                        //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
/// @file  
/// That file describes the configuration of the structure used to set up
///  
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIOSPI_CONFIG_H_
#define _GPIOSPI_CONFIG_H_

#include "cs_types.h"

#include "gpio_spi.h"
#include "hal_gpio.h"



// =============================================================================
//  
// -----------------------------------------------------------------------------
///
/// Make sure to include gpioic_config.h before  
// =============================================================================

#define GPIOSPI_CONFIG_STRUCT_T GPIO_SPI_CONFIG_STRUCT_T



// =============================================================================
// 
// -----------------------------------------------------------------------------
///  
// ============================================================================



struct GPIO_SPI_CONFIG_STRUCT_T
{
    /// SPI bus used to configure the chip

    HAL_APO_ID_T      scs_spi;        

    HAL_APO_ID_T      scl_spi;  
    
    HAL_APO_ID_T      sdi_spi;    

    HAL_APO_ID_T      sdo_spi;    

};

#endif // 


