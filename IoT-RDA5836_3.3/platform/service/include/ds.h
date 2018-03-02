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
/*      ds.h                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    Debug service declarations, constant definitions and macros             */
/*                                                                            */
/******************************************************************************/
#ifndef _DS_H_
#define _DS_H_
  
#ifdef __cplusplus
CPP_START
#endif

#include <ts.h>


UINT32 SRVAPI DS_FatalErr (
  PCSTR pszFileName,
  UINT32 nLineNum,
  PCSTR pszErrStr
); 

//
// Redefine the Debug API to trace in default level.
// 
#if defined(DEBUG)
#if defined(_WIN32)
#define DS_ASSERT(x) 
#define DS_FATAL_ERR(x) 
#define DS_CHECKHANDLE(handle) 
#else // _WIN32
#define DS_CHECKHANDLE(handle)  
#define DS_ASSERT(x) 
#endif
#else // DEBUG
#define DS_CHECKHANDLE(handle)      {	 }
#define DS_ASSERT(x)                {	 }
#endif // DEBUG

//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
CPP_END
#endif

#endif // _H_
