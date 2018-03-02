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

#ifndef _STD_VERSION_H_
#define _STD_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define STD_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================
#if	COMPILER == 2   //armcc
#define BUILD_DATE								2016
#endif

#ifndef STD_VERSION_NUMBER
#define STD_VERSION_NUMBER                       (0)
#endif

#ifndef STD_VERSION_DATE
#define STD_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef STD_VERSION_STRING
#define STD_VERSION_STRING                       "STD version string not defined"
#endif

#ifndef STD_VERSION_STRING_WITH_BRANCH
#define STD_VERSION_STRING_WITH_BRANCH           STD_VERSION_STRING " Branch: " "none"
#endif

#define STD_VERSION_STRUCT                       {STD_VERSION_REVISION, \
                                                  STD_VERSION_NUMBER, \
                                                  STD_VERSION_DATE, \
                                                  STD_VERSION_STRING_WITH_BRANCH}

#endif // _STD_VERSION_H_
