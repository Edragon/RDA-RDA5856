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

#ifndef _MDI_VERSION_H_
#define _MDI_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define MDI_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef MDI_VERSION_NUMBER
#define MDI_VERSION_NUMBER                       (0)
#endif

#ifndef MDI_VERSION_DATE
#define MDI_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef MDI_VERSION_STRING
#define MDI_VERSION_STRING                       "MDI version string not defined"
#endif

#ifndef MDI_VERSION_STRING_WITH_BRANCH
#define MDI_VERSION_STRING_WITH_BRANCH           MDI_VERSION_STRING " Branch: " "none"
#endif

#define MDI_VERSION_STRUCT                       {MDI_VERSION_REVISION, \
                                                  MDI_VERSION_NUMBER, \
                                                  MDI_VERSION_DATE, \
                                                  MDI_VERSION_STRING_WITH_BRANCH}

#endif // _MDI_VERSION_H_
