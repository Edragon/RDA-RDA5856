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
#include "sxs_type.h"

//------------------------------------------------------------------
// FS idle hook functions

extern void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void));

extern void sxs_FsIdleHook(void);

//------------------------------------------------------------------
// Debug idle hook functions

extern bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void));

extern bool sxs_ActivateDebugIdleHookFunc(void (*funcPtr)(void));

extern void sxs_DebugIdleHook(void);

