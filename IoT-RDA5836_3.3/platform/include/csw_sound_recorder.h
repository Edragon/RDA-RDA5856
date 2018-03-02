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
/// 
/// @defgroup cswSoundRec CSW Sound Recorder
/// @{
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _CSW_SOUND_RECORDER_H_
#define _CSW_SOUND_RECORDER_H_

#if (CSW_EXTENDED_API == 1)

#include "cs_types.h"
#include "mci_sound_recorder.h"

// ============================================================================
// Defines 
// ============================================================================

// ============================================================================
// INT32 Mmc_sndRecResume(VOID)
// ============================================================================
#define    Mmc_sndRecResume mci_SndRecResume 

// ============================================================================
// INT32 Mmc_sndRecPause(VOID) 
// ============================================================================
#define    Mmc_sndRecPause mci_SndRecPause  

// ============================================================================
// INT32 Mmc_sndRecStop(VOID)
// ============================================================================
#define    Mmc_sndRecStop mci_SndRecStop   


#endif // CSW_EXTENDED_API

/// @}

#endif //_CSW_SOUND_RECORDER_H_


