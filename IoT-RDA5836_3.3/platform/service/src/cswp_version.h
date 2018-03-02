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




#ifndef _CSW_VERSION_H_
#define _CSW_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define CSW_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef CSW_VERSION_NUMBER
#define CSW_VERSION_NUMBER                       (0)
#endif

#ifndef CSW_VERSION_DATE
#define CSW_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef CSW_VERSION_STRING
#define CSW_VERSION_STRING                       "CSW version string not defined"
#endif

#ifndef CSW_VERSION_STRING_WITH_BRANCH
#define CSW_VERSION_STRING_WITH_BRANCH           CSW_VERSION_STRING " Branch: " "none"
#endif

#define CSW_VERSION_STRUCT                       {CSW_VERSION_REVISION, \
                                                  CSW_VERSION_NUMBER, \
                                                  CSW_VERSION_DATE, \
                                                  CSW_VERSION_STRING_WITH_BRANCH}

#endif // _CSW_VERSION_H_
