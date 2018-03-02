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


#ifndef COS_CONFIG_H
#define COS_CONFIG_H

// =============================================================================
// CSW_CONFIG_T
// -----------------------------------------------------------------------------
/// This structure describes the user heap size
/// user heap is defined by SXR_NB_HEAP_USER which is currently set to 3
/// Any other heap would require to change the define in sxr_cnf.h

/// cswHeapSize: Size of the heap available for csw
/// cosHeapSize: Size of the heap available for mmi

// =============================================================================
// Only the memory part is represented but more config could be added
typedef struct 
{
// Memory configuration
    UINT32 cswHeapSize;
    UINT32 cosHeapSize;
} CSW_CONFIG_T;

#endif //end of COS_CONFIG_H

