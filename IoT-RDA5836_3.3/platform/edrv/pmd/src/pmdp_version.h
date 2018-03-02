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

                                                                             
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED AUTOMATICALLY BY THE MAKE PROCESS.
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//==============================================================================

#ifndef _PMD_VERSION_H_
#define _PMD_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define PMD_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef PMD_VERSION_NUMBER
#define PMD_VERSION_NUMBER                       (0)
#endif

#ifndef PMD_VERSION_DATE
#define PMD_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef PMD_VERSION_STRING
#define PMD_VERSION_STRING                       "PMD version string not defined"
#endif

#ifndef PMD_VERSION_STRING_WITH_BRANCH
#define PMD_VERSION_STRING_WITH_BRANCH           PMD_VERSION_STRING " Branch: " "none"
#endif

#define PMD_VERSION_STRUCT                       {PMD_VERSION_REVISION, \
                                                  PMD_VERSION_NUMBER, \
                                                  PMD_VERSION_DATE, \
                                                  PMD_VERSION_STRING_WITH_BRANCH}

#endif // _PMD_VERSION_H_
