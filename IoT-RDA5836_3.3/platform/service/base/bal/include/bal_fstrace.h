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
/// @file bal_fstrace.h
///
/// fs Trace service
//
////////////////////////////////////////////////////////////////////////////////

// marco
#define SECOND        * HAL_TICK1S
#define MILLI_SECOND  SECOND / 1000
#define MILLI_SECONDS MILLI_SECOND
#define SECONDS       SECOND
#define MINUTE        * ( 60 SECOND )
#define MINUTES       MINUTE
#define HOUR          * ( 60 MINUTE )
#define HOURS         HOUR
#define FS_TRACE_TIMER_CNT (1 SECOND)

 // (0xabcd| SXS_TIMER_EVT_ID)
#define FSTRACE_TIMER1 PRV_CFW_TRACE_TO_FLASH_TIMER_ID
#define MAX_TRACE_LEN 512

typedef struct
{
  u32 eventId;
  u32 nParam1;
//  u32 nParam2;//for delete nParam2
//  u32 nParam3;//for delete nParam3
} fsTrace_event_t;


// declaration of function

// =============================================================================
// bal_fsTraceProcess
// -----------------------------------------------------------------------------
/// Read a bunch of data from the debug remote buffer and output to tFlash card.
// =============================================================================
PUBLIC VOID bal_fsTraceProcess(VOID);

// =============================================================================
// bal_fstraceStart
// -----------------------------------------------------------------------------
/// start tFlash trace recorsing
// =============================================================================
PUBLIC VOID  bal_fstraceStart(VOID);

// =============================================================================
// bal_fstraceStop
// -----------------------------------------------------------------------------
/// stop tFlash trace recorsing
// =============================================================================
PUBLIC VOID  bal_fstraceStop(VOID);


