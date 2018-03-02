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

// ================================================================================
/// @file         sxr_sbx.h
/// @par Systeme executif synchronization boxes header file.
// --------------------------------------------------------------------------------
/// @defgroup sx_sbx SX Synchronization Boxes
/// @par Synchronization boxes management
/// A synchronization box is an object used for task synchronization,
/// message and event exchange between task and semaphore management.
/// The following rules apply:
///  - Any task may wait for any synchronization box.
///  - Several tasks may wait for the same synchronization box.
///  - Task priority is not considered when several tasks are waiting for a 
///    synchronization box. The task which first wait for a defined 
///    synchronization box is served first.
///  .
/// @{

/*
================================================================================
  History    :
--------------------------------------------------------------------------------
 Aug 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __SXR_SBX_H__
#define __SXR_SBX_H__

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// sxr_SbxInit
// -----------------------------------------------------------------------------
/// Synchronization boxes data context initialization.
// =============================================================================
void  sxr_SbxInit (void);
//void  sxr_SbxInit (u16 sxr_Nb_Max_Mbx_Env, u16 sxr_Nb_Max_Mbx);

// =============================================================================
// sxr_NewMailBox
// -----------------------------------------------------------------------------
/// provide a free mail box Id.
/// @return Mail box Id.
// =============================================================================
u8    sxr_NewMailBox (void);

// =============================================================================
// sxr_NewMailBoxWithCondition
// -----------------------------------------------------------------------------
/// provide a free mail box Id which does or does not support timer messages.
/// @param TimerSupport Whether to support timer messages.
/// @return Mail box Id.
// =============================================================================
u8    sxr_NewMailBoxWithCondition (bool TimerSupport);

// =============================================================================
// sxr_FreeMailBox
// -----------------------------------------------------------------------------
/// Free the previously allocated mail box.
/// @param Sbx  Mail box Id.
// =============================================================================
void  sxr_FreeMailBox (u8 Sbx);

// =============================================================================
// sxr_NewSemaphore
// -----------------------------------------------------------------------------
/// Provide a free semaphore.
/// @param InitValue    Semaphore initialization value:
///                     - 0 Means semaphore is taken,
///                     - n Means semaphore is free for n users.
///                     .
/// @return             Semaphore Id.
// =============================================================================
u8    sxr_NewSemaphore (u8 InitValue);

// =============================================================================
// sxr_FreeSemaphore
// -----------------------------------------------------------------------------
/// Free a previously allocated semaphore.
/// @param Id   Semaphore Id.
// =============================================================================
void  sxr_FreeSemaphore (u8 Id);

// =============================================================================
// sxr_SetRmtMbx
// -----------------------------------------------------------------------------
/// Set or unset a mail box in remote state.
/// @param Sbx  Sbx index.
/// @param Set  boolean Set.
// =============================================================================
void  sxr_SetRmtMbx (u8 Sbx, u8 Set);

// =============================================================================
// sxr_IsRmtSbx
// -----------------------------------------------------------------------------
/// Get a mail box in remote state.
/// @param Sbx  Sbx index.
/// @return     remote state.
// =============================================================================
u8    sxr_IsRmtSbx (u8 Sbx);

// =============================================================================
// sxr_SetRxTxRmtMsg
// -----------------------------------------------------------------------------
/// Set the remote conversion function pointers.
/// @param TxMsg    Pointer on Tx conversion function.
/// @param RxMsg    Pointer on Rx conversion function.
// =============================================================================
void  sxr_SetRxTxRmtMsg (void *(*TxMsg)(void *), void *(*RxMsg)(void *));

// =============================================================================
// sxr_SbxRxMsg
// -----------------------------------------------------------------------------
/// Call remote conversion function for received messages.
/// @param Msg  Pointer on received message.
/// @return     Pointer on converted message.
// =============================================================================
void *sxr_SbxRxMsg (void *Msg);

#define SXR_SEND_EVT    (1 << 0)
#define SXR_SEND_MSG    0
#define SXR_QUEUE_FIRST (1 << 1)
#define SXR_QUEUE_LAST  0

// =============================================================================
// sxr_Send
// -----------------------------------------------------------------------------
/// Send a message or an event to a synchronization box.
/// @param Msg          Pointer onto message or event.
/// @param Sbx          Box number.
/// @param MsgStatus    Message status.
// =============================================================================
void  sxr_Send (void *Msg, u8 Sbx, u8 MsgStatus);

// =============================================================================
// sxr_Wait
// -----------------------------------------------------------------------------
/// Wait for a message or an event on a synchronization box.
/// @param Evt  Pointer onto Event to be received.
/// @param Sbx  Synchronization box number.
/// @return     Pointer onto message or NIL for event.
// =============================================================================
void *sxr_Wait (u32 *Evt, u8 Sbx
#ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
,u32 period
#endif
);

// =============================================================================
// sxr_SbxHot
// -----------------------------------------------------------------------------
/// Check if at least a message or an event is in the synchronisation box.
/// @param Sbx  Synchronization box number.
/// @return     TRUE when box is hot, FALSE otherwise.
// =============================================================================
u8    sxr_SbxHot (u8 Sbx);

// =============================================================================
// sxr_RmvTaskFromSbx
// -----------------------------------------------------------------------------
/// Remove a task waiting on a SBX.
/// @param TaskId   Task Id.
/// @return         TRUE when task has been removed, FALSE otherwise.
// =============================================================================
u8    sxr_RmvTaskFromSbx (u8 TaskId);

// =============================================================================
// sxr_RmvMsgFromMbx
// -----------------------------------------------------------------------------
/// Remove a message or an event from a mail box.
/// @param Msg          Pointer onto a message.
/// @param Evt          Event description.
/// @param Sbx          Mail box number.
/// @param EvtCtxSize   Event size(0 for message).
/// @return             TRUE when message or event has been found, FALSE otherwise.
// =============================================================================
u8    sxr_RmvMsgFromMbx (void *Msg, u8 Evt, u8 Sbx, u8 EvtCtxSize);

// =============================================================================
// sxr_TakeSemaphore
// -----------------------------------------------------------------------------
/// Take the semaphore.
/// @param Id   Semaphore Id.
// =============================================================================
void  sxr_TakeSemaphore (u8 Id);

// =============================================================================
// sxr_ReleaseSemamphore
// -----------------------------------------------------------------------------
/// Release a semaphore.
/// @param Id   Semaphore Id.
void  sxr_ReleaseSemaphore (u8 Id);

// =============================================================================
// sxr_SemaphoreHot
// -----------------------------------------------------------------------------
/// Check if a semaphore is free.
/// @param Id   Semaphore Id.
/// @return     TRUE when the semaphore is free, FALSE when it is hot.
// =============================================================================
u8    sxr_SemaphoreHot (u8 Id);

// =============================================================================
// sxr_SemaphoreCount
// -----------------------------------------------------------------------------
/// Get current value of semaphore 
/// @param Id   Semaphore Id.
/// @return     
// =============================================================================
s8 sxr_SemaphoreGetValue(u8 id); 

// =============================================================================
// sxr_GetTaskWaitingForSbx
// -----------------------------------------------------------------------------
/// Return the Id of the task waiting on the Sbx.
/// @param Sbx  Sbx index.
/// @return     Id of the task.
// =============================================================================
u8    sxr_GetTaskWaitingForSbx (u8 Sbx);


// =============================================================================
// sxr_CheckSbx
// -----------------------------------------------------------------------------
/// Trace out a diagnostic information for all sbx
// =============================================================================
void  sxr_CheckSbx (void);

// =============================================================================
// sxr_ChkSbx
// -----------------------------------------------------------------------------
/// Trace out a diagnostic information for sbx
/// @param Sbx  Sbx index.
// =============================================================================
void  sxr_ChkSbx   (u8 Sbx);

// =============================================================================
// sxr_TraceSbx
// -----------------------------------------------------------------------------
/// Set trace diagnostic for sbx action (very verbose)
/// @param Sbx      Sbx index.
/// @param Level    Verbosity level, currently 0: off, else on.
// =============================================================================
void  sxr_TraceSbx (u8 Sbx, u8 Level);

typedef struct
{
 u8  NbAvailMsg;    // Number of message available in the Sbx.
 u8  NbWaitingTask; // Number of task waiting on the Sbx
 u8  FirstTaskId;   // Identifier of the first task waiting on the Sbx.
 u8  Pad;
 u32 FirstMsg;      // First message available in the Sbx.
} sxr_SbxInfo_t;


// =============================================================================
// sxr_GetSbxInfo
// -----------------------------------------------------------------------------
/// Get information about the identified synchronization box.
/// @param Sbx      Sbx index.
/// @param SbxInfo  Pointer to sxr_SbxInfo_t to fill.
// =============================================================================
void sxr_GetSbxInfo (u8 Sbx, sxr_SbxInfo_t *SbxInfo);

#ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
#include "cos.h"
#endif

#ifdef __cplusplus
}
#endif

#endif

/// @} // <-- End of sx_sbx group
