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
#ifndef __SXS_IO_H__
#define __SXS_IO_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <cswtype.h>
//#include <windows.h>
#include "hal_host.h"

#define TID_POS         0
#define TID_MSK         (0x1F << TID_POS)               /* 5 bits for user Id.  */
#define TID(Id)         ((u8)((Id << TID_POS) & TID_MSK))
#define TGET_ID(Id)     (u8)((Id & TID_MSK) >> TID_POS)
#define TLEVEL_POS      5
#define TLEVEL_MSK      (0x0F << TLEVEL_POS)            /* 4 bits for level.    */
#define TLEVEL(Lev)     (((Lev-1) << TLEVEL_POS) & TLEVEL_MSK)
#define TGET_LEVEL(Id)  (u8)((Id & TLEVEL_MSK) >> TLEVEL_POS)
#define TIDU_POS        9
#define TIDU            (1 << TIDU_POS)                 /* Do not display id.*/
#define TABORT_POS      10
#define TABORT          (1 << TABORT_POS)               /* Abort indicator for abort exception. */
#define TRAISE_POS      11
#define TRAISE          (1 << TRAISE_POS)               /* Raise indicator for catched exception. */
#define TDB_POS         12
#ifdef __SXS_STR_TO_ID__
#define TDB             (1 << TDB_POS)                  /* Data base indicator: format string to be found in data base. */
#else
#define TDB             0
#endif

#define TSMAP_POS       15
#define TSMAP_MSK       (0x3F << TSMAP_POS)             /* 6 bits for string parameter bit map. */
#define TSMAP(SMap)     ((SMap << TSMAP_POS) & TSMAP_MSK)
#define TGET_SMAP(Id)   (u8)((Id & TSMAP_MSK) >> TSMAP_POS)
#define TSTDOUT_POS     21
#define TSTDOUT         (1 << TSTDOUT_POS)              /* Stdout flow, trace setting independent. */
#define TARG_POS        22
#define TARG            (1 << TARG_POS)                 /* Argument number indicator.*/
#define TNB_ARG_POS     23
#define TNB_ARG_MSK     (0x07 << TNB_ARG_POS)           /* 3 bits for number of aguments. */
#define TNB_ARG(Nb)     (((Nb << TNB_ARG_POS) & TNB_ARG_MSK) | TARG)
#define TGET_NB_ARG(Id) (u8)((Id & TNB_ARG_MSK) >> TNB_ARG_POS)

#define SXS_DEFAULT_NB_ARG 6

#ifdef __SXS_STR_TO_ID__
#define TSTR(Str,Id) (void *)Id
#else
#define TSTR(Str,Id) Str
#endif

#define  _MMI      0

#define POSSIBLY_UNUSED 


#define SXS_TRACE app_trace

#define SXS_DUMP(Id, Fmt, Data, Size)
#define sxs_Dump(Id, Fmt, Data, Size)


#define DBG_ASSERT debug_assert

#define hal_DbgAssert debug_assert



extern BOOL sxr_StartFunctionTimer(UINT32 Period, 
		void(*Function)(void *),
		void *callbackArg, 
		UINT8 flags
);


extern BOOL sxr_StopFunctionTimer (void(*Function)(void *)
							);
#ifdef __cplusplus
}
#endif

#endif

