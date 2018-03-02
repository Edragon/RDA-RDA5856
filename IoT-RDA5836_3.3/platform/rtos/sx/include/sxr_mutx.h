/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/// @defgroup sx_mutex SX Simple Mutex
/// @par Simple Mutex
/// mutex are binary semaphores used to implement mutual exclusion at task level.
/// Compared to semaphores, the mutex can be taken more than once by the same 
/// task, in that case it must be released as many times to be actually released.
/// 
/// @{

#ifndef __SXR_MUTX_H__
#define __SXR_MUTX_H__

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// sxr_MutexInit
// -----------------------------------------------------------------------------
/// Mutual exclusion data context initialization.
// =============================================================================
void  sxr_MutexInit (void);


// =============================================================================
// sxr_NewMutex
// -----------------------------------------------------------------------------
/// provide a free mutex.
/// @return mutex Id.
// =============================================================================
u8    sxr_NewMutex (void);
// =============================================================================
// sxr_FreeMutex
// -----------------------------------------------------------------------------
/// Free a previously allocated mutex.
/// @param Id mutex Id.
// =============================================================================
void  sxr_FreeMutex (u8 Id);

// =============================================================================
// sxr_TakeMutex
// -----------------------------------------------------------------------------
/// Take the mutex.
/// @param Id mutex Id.
/// @return UserId to give to sxr_ReleaseMutex()
// =============================================================================
u8    sxr_TakeMutex (u8 Id);

// =============================================================================
// sxr_ReleaseMutex
// -----------------------------------------------------------------------------
/// Release a mutex.
/// @param Id mutex Id.
/// @param UserId from sxr_TakeMutex()
// =============================================================================
void  sxr_ReleaseMutex (u8 Id, u8 UserId);

// =============================================================================
// sxr_MutexHot
// -----------------------------------------------------------------------------
/// Check if a mutex is free.
/// @param Id mutex Id.
/// @return \c TRUE when the mutex is free, FALSE when it is hot.
// =============================================================================
u8    sxr_MutexHot (u8 Id);

// =============================================================================
// sxr_ChkMutex
// -----------------------------------------------------------------------------
/// Display debug information about mutexes
// =============================================================================
void  sxr_CheckMutex (void);

// =============================================================================
// sxr_ChkMutex
// -----------------------------------------------------------------------------
/// Display debug information about mutex
/// @param Id mutex Id.
// =============================================================================
void  sxr_ChkMutex (u8 Id);


#ifdef __cplusplus
}
#endif

#endif
/// @} // <-- End of sx_mutex group

