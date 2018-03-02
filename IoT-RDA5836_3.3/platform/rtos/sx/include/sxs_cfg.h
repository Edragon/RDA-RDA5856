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
  File       : sxs_cfg.h
--------------------------------------------------------------------------------

  Scope      : System configuration.

  History    :
--------------------------------------------------------------------------------
  Aug 25 03  | ADA   | Creation
================================================================================
*/

#ifndef __SXS_CFG_H__
#define __SXS_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sxs_type.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxs_lib.h"
#include "cmn_defs.h"

typedef struct
{
 void (*TaskInit)(void);
 void (*MsgParse)(void *);
 void (*TimerParse)(u32 *);
 u8   MailBox;
} sxs_TaskParse_t;

typedef struct
{
 const sxr_TaskDesc_t  TaskDesc;
 const sxs_TaskParse_t *TaskParse;
} sxs_TaskCfg_t;

void sxs_Task (sxs_TaskParse_t *TaskParse);


#ifdef __cplusplus
 }
#endif

#endif

