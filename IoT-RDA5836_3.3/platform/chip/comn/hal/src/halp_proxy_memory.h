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
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_proxy_memory.c
///
/// Implementation of the functions controlling the access to the proxy memory
/// (Fast memory, close to the CPU, and running at the CPU's clock speed) 
/// available on some chips.
//
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

// =============================================================================
// hal_PMemEnableExternalAccess
// -----------------------------------------------------------------------------
/// Enable or disable the external access to the proxy memory.
/// The proxy memory is normally only accessible by its suzerain CPU. In order
/// to program its content (eg Through a DMA loading an overlay), it is necessary
/// to enable the access to this memory from the external (From a CPU 
/// stand point) APB bus. 'Internal' access must be restored before using 
/// this memory from the CPU by a call to #hal_PMemEnableExternalAccess(FALSE).
///
/// @param enable When \c TRUE, enable access to the proxy memory for
/// programmation. When \c FALSE, disable that access. (Only the CPU can access
/// the memory).
// =============================================================================
PROTECTED VOID hal_PMemEnableExternalAccess(BOOL enable);







