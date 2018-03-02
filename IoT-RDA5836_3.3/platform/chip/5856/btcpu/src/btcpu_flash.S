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
/// @file bcpu_rom.S                                                          //
/// That file provides the boot code                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "regdef.h"
#include "cp0.h"

#include "global_macros.h"

    
        
///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

    

.extern _res_data_flash_start_location
.extern _bt_flash_end


.text
.set at
//############################################
//# Flash start
//############################################
.globl  _flash_btcpu_start
_flash_btcpu_start:
    flash_valid_flag:     		.word 0xffffffff  // 0xD9EF0045
    flash_testmode_flag:    	.word 0xffffffff  // testmode
    flash_cdacdelta_value:    .word 0xffffffff  // g_CdacDelta
    flash_resource_addr:      .word _res_data_flash_start_location // start of resource bin
    j _bt_flash_end
    nop
    
    flash_xcpu_start_addr:    .word _bt_flash_end


