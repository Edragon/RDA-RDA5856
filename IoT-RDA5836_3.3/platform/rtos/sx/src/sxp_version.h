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

#ifndef _SX_VERSION_H_
#define _SX_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define SX_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef SX_VERSION_NUMBER
#define SX_VERSION_NUMBER                       (0)
#endif

#ifndef SX_VERSION_DATE
#define SX_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef SX_VERSION_STRING
#define SX_VERSION_STRING                       "SX version string not defined"
#endif

#ifndef SX_VERSION_STRING_WITH_BRANCH
#define SX_VERSION_STRING_WITH_BRANCH           SX_VERSION_STRING " Branch: " "none"
#endif

#define SX_VERSION_STRUCT                       {SX_VERSION_REVISION, \
                                                  SX_VERSION_NUMBER, \
                                                  SX_VERSION_DATE, \
                                                  SX_VERSION_STRING_WITH_BRANCH}

#endif // _SX_VERSION_H_
