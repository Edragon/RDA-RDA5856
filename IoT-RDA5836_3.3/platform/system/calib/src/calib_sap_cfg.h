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



/*
================================================================================
  File       : sap_cfg.h
--------------------------------------------------------------------------------

  Scope      : System configuration.

  History    :
--------------------------------------------------------------------------------
================================================================================
*/

#ifndef __SAP_CFG_H__
#define __SAP_CFG_H__

#include "sxs_type.h"
#include "sxs_cfg.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxs_lib.h"
#include "cmn_defs.h"

/******************************************************************************
       OS TASKS CONFIGURATION
******************************************************************************/
EXPORT UINT8 g_calibStubTaskId;
EXPORT UINT8 g_calibStubMbx;

EXPORT VOID calib_StubTaskInit (VOID);
EXPORT VOID calib_SxsTaskIdle (VOID);
EXPORT VOID calib_SapInitUsr(VOID);

#define CALIB_CALIB_TASK_STACK_SIZE   4*128
#define CALIB_CALIB_TASK_PRIORITY     200


#define CALIB_IDLE_TASK_STACK_SIZE 2*128
#define CALIB_IDLE_TASK_PRIORITY   255

#ifdef __SAP_CFG_VAR__
CONST sxr_TaskDesc_t g_calibSxsTaskIdleDesc =
{
    (VOID(*)(VOID *))calib_SxsTaskIdle,
    0,
    "Calibration Idle Task.",
    CALIB_IDLE_TASK_STACK_SIZE,
    CALIB_IDLE_TASK_PRIORITY
};

CONST sxr_TaskDesc_t g_calibSxsCalibStubTaskDesc =
{
    (VOID(*)(VOID *))calib_StubTaskInit,
    NIL,
    "Calibration Calib. Task",
    CALIB_CALIB_TASK_STACK_SIZE,
    CALIB_CALIB_TASK_PRIORITY
};

#else

EXPORT CONST sxr_TaskDesc_t g_calibSxsTaskIdleDesc;
EXPORT CONST sxr_TaskDesc_t g_calibSxsCalibTaskDesc;


#endif

#endif


