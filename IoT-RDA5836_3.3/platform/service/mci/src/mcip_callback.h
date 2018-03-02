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
//
/// @file mcip_callback.h
///
/// MCI callback
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCIP_CALLBACK_H_
#define _MCIP_CALLBACK_H_

#include "cs_types.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

VOID vid_send_play_finish_ind(UINT16 result);

VOID vid_send_file_ready_ind(UINT16 result);

VOID vid_send_seek_done_ind(UINT16 result);

VOID aud_send_media_play_finish_ind(UINT8 result);

VOID vid_file_info(UINT16 width, UINT16 height,
                   UINT32 nbFrames, UINT32 totalTime);

#endif // _MCIP_CALLBACK_H_
