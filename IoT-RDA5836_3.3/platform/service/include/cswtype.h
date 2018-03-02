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



/*                                                                            */
/* FILE NAME                                                                  */
/*      cswtype.h                                                             */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      all basic types or constant definitions on CSW.                       */
/*                                                                            */
/******************************************************************************/

//#define CFW_PBK_SYNC_VER  

#if !defined(__CSWTYPE_H__)
#define __CSWTYPE_H__

#define HAVE_TM_MODULE    1
#define HAVE_LIBC_MALLOC  0
#include "cs_types.h"

// CSW specific types
typedef UINT16              RESID;

typedef HANDLE              HRES;
typedef UINT32              EVID;

typedef UINT32              EVPARAM;
typedef UINT32              HAO;

/*function point type*/
typedef BOOL (*PFN_BOOL)(VOID);
typedef VOID (*PTASK_ENTRY)(PVOID pParameter);

#endif // _H_

