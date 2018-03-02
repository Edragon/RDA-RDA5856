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
  File         sxs_defs.h
--------------------------------------------------------------------------------

  Scope      : Compilation flags definition.

  History    :
--------------------------------------------------------------------------------
 Aug 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __SXS_DEFS_H__
#define __SXS_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__arm) || defined (__TARGET__) || defined (__SERIAL__)
#define __EMBEDDED__
#endif

#ifdef __cplusplus
}
#endif

#endif

