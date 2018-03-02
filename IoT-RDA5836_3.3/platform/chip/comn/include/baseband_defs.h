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



#ifndef _BASEBAND_DEFS_H_
#define _BASEBAND_DEFS_H_



// =============================================================================
//  MACROS
// =============================================================================
/// -------------------------------------------------------------------- Timings
/// and Sizes These timings are shared by different SW modules and are derived from
/// baseband signal processing capabilities and needs --------------------------------------------------------------------
/// Values in symbols
#define BB_RX_SCH_MARGIN                         (12)
#define BB_RX_NBURST_MARGIN                      (4)
/// One Qb is dropped
#define BB_BURST_TOTAL_SIZE                      (156)
/// Used for EQU_HBURST_MODE'
#define BB_BURST_HALF_SIZE                       (96)
#define BB_BURST_ACTIVE_SIZE                     (148)
#define BB_SCH_TOTAL_SIZE                        ((BB_BURST_ACTIVE_SIZE + 2*BB_RX_SCH_MARGIN))

// =============================================================================
//  TYPES
// =============================================================================



/**
@file
Defines related to the BaseBand signal processing
*/


#endif

