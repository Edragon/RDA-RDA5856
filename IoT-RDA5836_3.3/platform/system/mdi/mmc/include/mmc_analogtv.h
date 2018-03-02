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
/// @file  mmc_analogtv.h                                                                   //
/// That file describes the analog TV interface.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _MMC_ANALOGTV_H_
#define _MMC_ANALOGTV_H_

#include "cs_types.h"

#include "mci.h"
#include "atvd_m.h"

/// @file mmc_analogtv.h
/// @mainpage analog TV API
/// @page apbs_mainpage  analog TV API

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// =============================================================================
// MMC_ANALOG_MSGID
// -----------------------------------------------------------------------------
/// 
/// 
// =============================================================================

typedef enum
{
	//IFC interrupt
	MMC_ANALOG_TV_REALTIME_PROCESS=0x10,

} MMC_ANALOG_TV_MSGID;


// =============================================================================
// MMC_ANALOG_MSGID
// -----------------------------------------------------------------------------
/// 
/// 
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================
PUBLIC UINT32 mmc_AnalogTvAudioSetup(UINT8 volume);
PUBLIC UINT32 mmc_AnalogTvAudioOpen(VOID);
PUBLIC UINT32 mmc_AnalogTvAudioClose(VOID);

#endif
