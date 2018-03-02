/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : sap_cfg.h
--------------------------------------------------------------------------------

  Scope      : System configuration.

  History    :
--------------------------------------------------------------------------------
  Aug 05 03  | ADA   | Creation
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

#define IDLE_TASK_STACK_SIZE 2*128
#define IDLE_TASK_PRIORITY   255

extern void sxs_TaskIdle (void);


const sxs_TaskCfg_t *sxs_TaskCfg = 0;

const sxr_TaskDesc_t sxs_TaskIdleDesc =
{
 (void(*)(void *))sxs_TaskIdle,
 0,
 "Task Idle.",
 IDLE_TASK_STACK_SIZE,
 IDLE_TASK_PRIORITY
};


const u8 sxs_NbStaticTasksDesc = 0;

#endif
