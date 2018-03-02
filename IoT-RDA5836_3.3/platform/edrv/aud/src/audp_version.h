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



#ifndef _AUD_VERSION_H_
#define _AUD_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define AUD_VERSION_REVISION                     (-1)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef AUD_VERSION_NUMBER
#define AUD_VERSION_NUMBER                       (0)
#endif

#ifndef AUD_VERSION_DATE
#define AUD_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef AUD_VERSION_STRING
#define AUD_VERSION_STRING                       "AUD version string not defined"
#endif

#ifndef AUD_VERSION_STRING_WITH_BRANCH
#define AUD_VERSION_STRING_WITH_BRANCH           AUD_VERSION_STRING " Branch: " "none"
#endif

#define AUD_VERSION_STRUCT                       {AUD_VERSION_REVISION, \
                                                  AUD_VERSION_NUMBER, \
                                                  AUD_VERSION_DATE, \
                                                  AUD_VERSION_STRING_WITH_BRANCH}

#endif // _AUD_VERSION_H_
