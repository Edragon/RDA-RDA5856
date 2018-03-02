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
/// @file memd_switch_mode.c                                                  //
/// That file implements the function to switch the flash into a RAM mode (where
/// it behaves like a RAM), for Flash model which are not the Romulator (Ie for
/// which this is not possible)
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"

#ifndef MEMD_USE_ROMULATOR 

// =============================================================================
// memd_FlashRomulatorSetRamMode
// -----------------------------------------------------------------------------
/// Simili 'Set the  in RAM mode'. Always return \c FALSE and does nothing.
/// This function is only effective when using a romulator device.
///
/// When the romualtor is in RAM mode, it can be accessed as a RAM, that is to 
/// say read and written without sending any command, but through direct 
/// addressing.
///
/// @param  enable Enable or disable the RAM mode.
/// @return FALSE.
// =============================================================================
PUBLIC BOOL memd_FlashRomulatorSetRamMode(BOOL enable)
{
    MEMD_TRACE(MEMD_WARN_TRC, 0, "MEMD: Ram Mode Feature not available on a combo.");
    return FALSE;
}

#endif // MEMD_USE_ROMULATOR not defined.

