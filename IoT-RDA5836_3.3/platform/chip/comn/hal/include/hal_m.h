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
/// @file hal_m.h                                                             //
/// That file This file contains HAL API..                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_M_H_
#define _HAL_M_H_

#warning "Are you sure you wanted to include hal.h and not a hal_module.h ?"

/// @defgroup hal HAL Mainpage
/// @{
/// 
/// Hardware Abstraction Layer (HAL) API description.
///
/// @version 
/// @date   
/// 
/// @par Purpose
/// 
/// This document describes the characteristics of the chip's modules and how to
/// use them via its Hardware Abstraction Layer (HAL) API.
/// 
/// @par Module List
/// 
/// For a list of modules, please click on the Modules tab above.
/// .
/// The debug features rely the following module:
/// - @ref debugPort
/// - @ref debugExl 
/// - @ref debugPxts
/// - @ref signalSpy
/// - @ref error_handling
/// - @ref hwBkpt
/// - @ref pageProtection
/// - @ref palTrace
/// - @ref map
/// 

#include "hal_aif.h"
#include "hal_ana.h"
#include "hal_boot.h"
#include "hal_cipher.h"
#include "hal_comregs.h"
#include "hal_config.h"
#include "hal_debug.h"
#include "hal_debugpaltrc.h"
#include "hal_dma.h"
#include "hal_ebc.h"
#include "hal_error.h"
#include "hal_fint.h"
#include "hal_gpadc.h"
#include "hal_gpio.h"
#include "hal_host.h"
#include "hal_i2c.h"
#include "hal_key.h"
#include "hal_lps.h"
#include "hal_mem_map.h"
#include "hal_padac.h"
#include "hal_pwm.h"
#include "hal_rfspi.h"
#include "hal_sim.h"
#include "hal_speech.h"
#include "hal_spi.h"
#include "hal_sys.h"
#include "hal_tcu.h"
#include "hal_timers.h"
#include "hal_trace.h"
#include "hal_uart.h"
#include "hal_voc.h"
#include "hal_map_engine.h"
#include "hal_overlay.h"


/// @}

#endif //  _HAL_M_H_ 



