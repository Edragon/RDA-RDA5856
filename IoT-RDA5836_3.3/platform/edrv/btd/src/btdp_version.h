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


#ifndef _BTD_VERSION_H_
#define _BTD_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define BTD_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef BTD_VERSION_NUMBER
#define BTD_VERSION_NUMBER                       (0)
#endif

#ifndef BTD_VERSION_DATE
#define BTD_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef BTD_VERSION_STRING
#define BTD_VERSION_STRING                       "BTD version string not defined"
#endif

#ifndef BTD_VERSION_STRING_WITH_BRANCH
#define BTD_VERSION_STRING_WITH_BRANCH           BTD_VERSION_STRING " Branch: " "none"
#endif

#define BTD_VERSION_STRUCT                       {BTD_VERSION_REVISION, \
                                                  BTD_VERSION_NUMBER, \
                                                  BTD_VERSION_DATE, \
                                                  BTD_VERSION_STRING_WITH_BRANCH}

#endif // _BTD_VERSION_H_
