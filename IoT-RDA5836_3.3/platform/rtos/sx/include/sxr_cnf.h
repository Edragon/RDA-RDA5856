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

/*
================================================================================
  File         sxr_cnf.h
--------------------------------------------------------------------------------

  Scope      : Systeme executif configuration header file.

  History    :
--------------------------------------------------------------------------------
  Aug 13 03  |  ADA  | Creation
  Sep 13 04  |  ADA  | SXR_TK_STCK_HEAP for task stack allocation.
  Nov 20 04  |  ADA  | SXR_JB_STCK_HEAP for job stack allocation.
             |       | Flag __SXR_RELEASED_PTR_TO_NIL__
  Jun 07 05  |  ADA  | SXR_FRAMED_TIMER and SXR_REGULAR_TIMER: pal defines.
  Dec 15 05  |  ADA  | Add SXS_SPY_NB_MAX
================================================================================
*/

#ifndef __SXR_CONF_H__
#define __SXR_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sxr_csp.h"
#include "../../../target/include/tgt_m.h"

#ifndef __TARGET__
#define __SXR_DEBUG__
#endif

#define __SXR_STACK_CHECK__
#define NO__SXR_MEM_CHECK__
#define NO__SXR_CLUSTER__               /* Cluster memory is handle with heap.                 */
//#define __SXR_RELEASED_PTR_TO_NIL__     /* A called to Free(Ptr) provoke Ptr to be set to NIL. */
#define __SXR_MEM_TRACKER__

/* ---------------- OS configuration defines. ------------------------ */
#define SXR_NB_MAX_TASK        (tgt_GetSxNbConfig()->sxr_NB_MAX_TASK)//8 //   60     /* Must be < SXR_NB_MAX_MBX + SXR_NB_MAX_SEM */

#ifndef __TARGET__
#define SXR_STACK_SIZE_MULTIPLIER     15    // when not on target increase all the stacks by 15
#else
#define SXR_STACK_SIZE_MULTIPLIER     (tgt_GetSxNbConfig()->sxr_STACK_SIZE_MULTIPLIER)// 1
#endif

#define SXR_SET_STACK(Size)       (Size*SXR_STACK_SIZE_MULTIPLIER)

#define SXR_TK_STCK_HEAP           (tgt_GetSxNbConfig()->sxr_TK_STCK_HEAP)//SXR_CSP_TK_STCK_HEAP   /* Heap to be considered for task stacks allocation. */
#define SXR_JB_STCK_HEAP           (tgt_GetSxNbConfig()->sxr_JB_STCK_HEAP)//SXR_CSP_JB_STCK_HEAP //0   /* Heap to be considered for job stacks allocation. */
#define SXR_JB_CTX_HEAP            (tgt_GetSxNbConfig()->sxr_JB_CTX_HEAP)//SXR_CSP_JB_CTX_HEAP   /* Heap used for the job context. */

/* ---------- Mail boxes & semaphores configuration defines. -------------- */
#define SXR_NB_MAX_MBX_ENV      (tgt_GetSxNbConfig()->sxr_NB_MAX_MBX_ENV)//150 // 200  /* Number max of enveloppes.        */
#ifdef GSM_SUPPORT
#define SXR_EVT_MBX_SIZE   4
#else
#define SXR_EVT_MBX_SIZE        (tgt_GetSxNbConfig()->sxr_EVT_MBX_SIZE)// 2    /* Event size. Min = 2 u32          */
#endif
#define SXR_NB_MAX_MBX           (tgt_GetSxNbConfig()->sxr_NB_MAX_MBX)//20 //  160  /* Number max of mail boxes.        */
#define SXR_NB_MAX_SEM            (tgt_GetSxNbConfig()->sxr_NB_MAX_SEM)//6// 40   /* Number max of semaphores.        */
#define SXR_NB_MAX_MUTEX           (tgt_GetSxNbConfig()->sxr_NB_MAX_MUTEX)//1// 6    /* Number max of mutex.             */

/* ----------------- Timer configuration defines. ------------------------  */
#define SXR_TIMER_NB_INSTANCE      (tgt_GetSxNbConfig()->sxr_TIMER_NB_INSTANCE)// 1 // 3      /* Number of timer instances.       */
#define SXR_NB_MAX_TIMER_ENVELOPPE  (tgt_GetSxNbConfig()->sxr_NB_MAX_TIMER_ENVELOPPE)//20 // 200    /* Number Max of actif timerS.      */
#define SXR_TIMER_CTX_SIZE         2//(tgt_GetSxNbConfig()->sxr_TIMER_CTX_SIZE)// Difficult to replace.     /* Min = 2 !!!                      */
#define SXR_TIMER_LIST_SIZE        8  //Don't need to replace.    /* Maximum timer list size.         */

#define SXR_REGULAR_TIMER          PAL_REGULAR_TIMER
#define SXR_FRAMED_TIMER           PAL_FRAMED_TIMER

/* ------------- Memory management configuration defines. ----------------- */
#define SXR_NB_MAX_POOL            (tgt_GetSxNbConfig()->sxr_NB_MAX_POOL)//7
#define SXR_NB_HEAP_USER           (tgt_GetSxNbConfig()->sxr_NB_HEAP_USER)//5// 6

#define SXR_MEM_PATTERN            0xFD
#define SXR_WD_MEM_PATTERN         0xFDFD
#define SXR_DW_MEM_PATTERN         0xFDFDFDFD


/* ------------------- Queues configuration defines. ---------------------- */
#define SXR_NB_MAX_QUEUE                 (tgt_GetSxNbConfig()->sxr_NB_MAX_QUEUE)//120 //           240
#define SXR_NB_MAX_USR_QUEUE             (tgt_GetSxNbConfig()->sxr_NB_MAX_USR_QUEUE)//           2
#define SXR_NB_QUEUE_ENVELOPPE           (tgt_GetSxNbConfig()->sxr_NB_QUEUE_ENVELOPPE)//300 //           500
/// The queue are considered overloaded (Reserved for high priority usage) if
/// the number of free envelops fall beneath that value.
#define SXR_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_START (tgt_GetSxNbConfig()->sxr_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_START)//100//  200
/// The queue are considered not overloaded anymore (Low priority usage is
/// allowed again) once the number of free envelops reaches that value.
#define SXR_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_END   (tgt_GetSxNbConfig()->sxr_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_END)//250 // 350

/* ------------------- Trace configuration defines. ---------------------- */
#define SXR_NB_MAX_TRACEID         (tgt_GetSxNbConfig()->sxr_NB_MAX_TRACEID)//30

/* --------------------------- Files defines. ----------------------------- */
#define SXS_FLH_NB_FILES_MAX       (tgt_GetSxNbConfig()->sxs_FLH_NB_FILES_MAX)//16

/* --------------------------- Spy defines. ------------------------------- */
#define SXS_SPY_NB_MAX             (tgt_GetSxNbConfig()->sxs_SPY_NB_MAX)//8


#ifdef __cplusplus
}
#endif

#endif

