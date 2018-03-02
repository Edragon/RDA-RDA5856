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
#include "sxs_idle.h"
#include "sxr_csp.h"


#define SXS_DEBUG_IDLE_HOOK_NUM 5


typedef struct
{
    void (*func)(void);
    bool enable;
} SXS_DEBUG_IDLE_HOOK_ELEM;


//------------------------------------------------------------------
// Idle hook variables

void (*g_sxsFsIdleHookFuncPtr)(void) = NIL;

SXS_DEBUG_IDLE_HOOK_ELEM g_sxsDebugIdleHookElem[SXS_DEBUG_IDLE_HOOK_NUM] =
    { { NIL, FALSE, }, };

volatile u32 __attribute__((section(".ucdata"))) g_sxsDebugIdleHookEnable = 0;


//------------------------------------------------------------------
// FS idle hook functions

void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void))
{
    g_sxsFsIdleHookFuncPtr = funcPtr;
}

void sxs_FsIdleHook(void)
{
    if (g_sxsFsIdleHookFuncPtr != NIL)
    {
        (*g_sxsFsIdleHookFuncPtr)();
    }
}


//------------------------------------------------------------------
// Debug idle hook functions

bool sxs_RegisterDebugIdleHookElem(void (*funcPtr)(void), bool enable)
{
    int i;
    bool result = FALSE;

    u32 status = sxr_EnterSc ();
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookElem[i].func == NIL)
            break;
    }

    if (i < SXS_DEBUG_IDLE_HOOK_NUM)
    {
        g_sxsDebugIdleHookElem[i].func = funcPtr;
        g_sxsDebugIdleHookElem[i].enable = enable;
        result = TRUE;
    }
    sxr_ExitSc (status);

    return result;
}

bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void))
{
    return sxs_RegisterDebugIdleHookElem(funcPtr, FALSE);
}

bool sxs_ActivateDebugIdleHookFunc(void (*funcPtr)(void))
{
    return sxs_RegisterDebugIdleHookElem(funcPtr, TRUE);
}

void sxs_DebugIdleHook(void)
{
    int i;
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookElem[i].func != NIL &&
                (g_sxsDebugIdleHookEnable ||
                 g_sxsDebugIdleHookElem[i].enable))
        {
            (*g_sxsDebugIdleHookElem[i].func)();
        }
    }
}

