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
/*
================================================================================
  File         sxr_tls.c
--------------------------------------------------------------------------------

  Scope      : Tools for system executif.

  History    :
--------------------------------------------------------------------------------
  Jun 13 2003   |  ADA  | Creation
  Nov 10 2005   |  ADA  | sxr_Sleep: critical section.
  Feb 27 2006   |  ADA  | Random generated through a polynom generator.
================================================================================
*/
#define __SXR_TLS_VAR__
#include "sxr_tls.h"
#undef __SXR_TLS_VAR__

#include "sxs_type.h"
#include "sxs_io.h"
#include "sxs_lib.h"
#include "sxr_csp.h"
#include "sxr_ops.h"
#include "sxr_tksd.hp"




/*
==============================================================================
   Function   : sxr_RandomInit
 ----------------------------------------------------------------------------
   Scope      : Initialize random table pointer. Pointed table must give access
                to a 128 bytes array containing 7 bits random values.
   Parameters : Random Table address.
   Return     : none
==============================================================================
*/
void sxr_RandomInit (u8 *RandomTab)
{
}


/*
==============================================================================
   Function   : sxr_Random
 ----------------------------------------------------------------------------
   Scope      : Generate a random value in the required interval.
   Parameters : Maximum value in the interval.
   Return     : none
==============================================================================
*/
u16 _sxr_Random (u16 Max, u8 Usr)
{
 static u16 Xn [SXR_RND_NB_MAX_USR];

 Xn [Usr] = ((9477 * Xn [Usr]) + 39) & 0xFFFF;

 return (Xn [Usr] % (Max + 1));
}



void sxr_TestRaise (void)
{
 sxs_Raise (_SXR|TABORT|TDB,TSTR("Raise...\n",0x06ac0001));
}


void sxr_DeadLock (void)
{
 while (1);
}

/*
================================================================================
  Function   : sxr_ComputeFcs
--------------------------------------------------------------------------------

  Scope      : Compute the FCS (CRC 24 bits) for LLC or RLP frames

  Parameters : Length over which to calculate the FCS, pointer to the data,
               pointer where to store the FCS calculated

  Return     : none
================================================================================
*/
void sxr_ComputeFcs (u16 Len, u8 *Frm, u8 *Fcs)
{
u32 R = 0xFFFFFFFF;   // Remainder
u16 i;


  SXS_TRACE(_SXR|TDB|TNB_ARG(1),TSTR("Compute FCS over %d bytes",0x06ac0002), Len);

  for (i = 0; i < Len; i++)
  {
    R = (R<<8) ^ sxr_FcsTab[(u8)((R>>16)^(sxr_MsbToLsb[*Frm++]))];
  }
  R = ~R;  // ones complement of the remainder

  Fcs[0] = sxr_MsbToLsb[(u8)(R >> 16)];
  Fcs[1] = sxr_MsbToLsb[(u8)(R >> 8)];
  Fcs[2] = sxr_MsbToLsb[(u8)(R)];

  SXS_TRACE(_SXR|TDB|TNB_ARG(3),TSTR("Calculated FCS : 0x%02X 0x%02X 0x%02X",0x06ac0003), Fcs[0], Fcs[1], Fcs[2]);

} // sxr_ComputeFcs

/*
==============================================================================
   Function   : sxr_Sleep
 -----------------------------------------------------------------------------
   Scope      : Suspend the active task for the required period.
   Parameters : Period expressed in regular timer basic unit.
   Return     : None.
==============================================================================
*/

#if (0)
VOID HAL_FUNC_INTERNAL hal_GetK1(UINT32* k1)
{
    asm volatile("move $2, $27\n\tsw $2, 0(%0)"::"r"((k1)):"$2");
}
#else /*!0*/
#endif /*0*/




void sxr_Sleep (u32 Period)
{
 u32 Status;

 Status = sxr_EnterSc ();

#ifndef CT_NO_DEBUG
    extern VOID hal_GetK1(UINT32* k1);
    UINT32 k1 = 1;
    hal_GetK1(&k1);
    if (k1 != 0)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("sxr_Sleep() must be run in task context! (k1=0x%08x)\n",0x06ac0004),
                            k1));
    }
    if (sxr_Task.Active == sxr_Task.Idle)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(2), TSTR("sxr_Sleep() cannot be run in idle task! (active=%d, idle=%d)\n",0x06ac0005),
                            sxr_Task.Active, sxr_Task.Idle));
    }
    if (sxr_Task.ScheduleDisable != 0)
    {
        SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1), TSTR("sxr_Sleep() cannot suspend current task if schedule disabled! (schDis=%d)\n",0x06ac0006),
                            sxr_Task.ScheduleDisable));
    }
#endif // ! CT_NO_DEBUG

 if (Period != 0)
 {
  // The parameter passed to sxr_ResummeTask() should is a u8. But it is passed
  // through the SXR function timer mechanism, which only passes a void* parameter
  // to the called function. This is why we need the double casting.
  sxr_StartFunctionTimer (Period, sxr_ResumeTask, (void*)(u32)sxr_Task.Active, _SXR);
  
  sxr_SuspendTask (sxr_Task.Active);
 }
 sxr_ExitSc (Status);
}


