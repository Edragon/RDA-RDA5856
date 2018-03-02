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



#ifndef _CALIB_VERSION_H_
#define _CALIB_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define CALIB_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef CALIB_VERSION_NUMBER
#define CALIB_VERSION_NUMBER                       (0)
#endif

#ifndef CALIB_VERSION_DATE
#define CALIB_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef CALIB_VERSION_STRING
#define CALIB_VERSION_STRING                       "CALIB version string not defined"
#endif

#ifndef CALIB_VERSION_STRING_WITH_BRANCH
#define CALIB_VERSION_STRING_WITH_BRANCH           CALIB_VERSION_STRING " Branch: " "none"
#endif

#define CALIB_VERSION_STRUCT                       {CALIB_VERSION_REVISION, \
                                                  CALIB_VERSION_NUMBER, \
                                                  CALIB_VERSION_DATE, \
                                                  CALIB_VERSION_STRING_WITH_BRANCH}

#endif // _CALIB_VERSION_H_
