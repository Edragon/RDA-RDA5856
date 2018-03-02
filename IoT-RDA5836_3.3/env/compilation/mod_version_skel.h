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


#ifndef _@{MOD}_VERSION_H_
#define _@{MOD}_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define @{MOD}_VERSION_REVISION                     (@{LOCAL_REVISION})

// =============================================================================
//  TYPES
// =============================================================================

#ifndef @{MOD}_VERSION_NUMBER
#define @{MOD}_VERSION_NUMBER                       (0)
#endif

#ifndef @{MOD}_VERSION_DATE
#define @{MOD}_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef @{MOD}_VERSION_STRING
#define @{MOD}_VERSION_STRING                       "@{MOD} version string not defined"
#endif

#ifndef @{MOD}_VERSION_STRING_WITH_BRANCH
#define @{MOD}_VERSION_STRING_WITH_BRANCH           @{MOD}_VERSION_STRING " Branch: " @{LOCAL_BRANCH}
#endif

#define @{MOD}_VERSION_STRUCT                       {@{MOD}_VERSION_REVISION, \
                                                  @{MOD}_VERSION_NUMBER, \
                                                  @{MOD}_VERSION_DATE, \
                                                  @{MOD}_VERSION_STRING_WITH_BRANCH}

#endif // _@{MOD}_VERSION_H_
