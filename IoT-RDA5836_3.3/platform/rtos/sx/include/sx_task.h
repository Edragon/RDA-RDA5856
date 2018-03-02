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
#ifndef _SX_TASK_H_
#define _SX_TASK_H_



// =============================================================================
//  MACROS
// =============================================================================
#define SXR_NO_TASK                              (0XFF)
#define SXR_FREE_TSK                             ((1 << 0))
#define SXR_ALLOCATED_TSK                        ((1 << 1))
#define SXR_PENDING_TSK                          ((1 << 2))
#define SXR_SUSPENDED_TSK                        ((1 << 3))
#define SXR_ACTIVE_TSK                           ((1 << 4))
#define SXR_WAIT_MSG_TSK                         ((1 << 5))
#define SXR_STOPPED_TSK                          (SXR_ALLOCATED_TSK)
#define SXR_NB_MAX_TASK                          (40)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SXR_TASKDESC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32*                        TaskBody;                     //0x00000000
//    UINT32*                        TaskExit;                     //0x00000004
    CONST INT8*                    Name;                         //0x00000008
    UINT16                         StackSize;                    //0x0000000C
    UINT8                          Priority;                     //0x0000000E
} SXR_TASKDESC_T; //Size : 0x10



// ============================================================================
// SXR_TASKCTX_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32*                        Sp;                           //0x00000000
    VOID*                          Pc;                           //0x00000004
    UINT32*                        StackTop;                     //0x00000008
    CONST SXR_TASKDESC_T*          Desc;                         //0x0000000C
    VOID*                          DataCtx;                      //0x00000010
    UINT8                          State;                        //0x00000014
    UINT8                          Id;                           //0x00000015
    UINT8                          Next;                         //0x00000016
    UINT8                          Free;                         //0x00000017
} SXR_TASKCTX_T; //Size : 0x18





#endif

