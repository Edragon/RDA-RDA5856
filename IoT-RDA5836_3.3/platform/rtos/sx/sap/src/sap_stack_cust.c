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
                                                                             
// Instantiate the Stack's configuration structures.
//
//==============================================================================

#define __SAP_CUST_VAR__
#include "sap_cust.h"
#undef  __SAP_CUST_VAR__


/*
==============================================================================
   Function   : sxs_ProtoStackAvail
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : TRUE to enable protocol stack, FALSE otherwise.
==============================================================================
*/
bool sxs_ProtoStackAvail (void)
{
#ifdef NO_PROTOSTACK
    return FALSE;
#else
    return TRUE;
#endif
}


/*
==============================================================================
   Function   : sxs_UsimSupported
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : TRUE to support USIM, FALSE otherwise.
==============================================================================
*/
bool sxs_UsimSupported (void)
{
#ifdef USIM_SUPPORT
    return TRUE;
#else
    return FALSE;
#endif
}


/*
==============================================================================
   Function   : sxs_SimLimitRfTxPower
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : TRUE to allow limit RF Tx power for SIM access, FALSE otherwise.
==============================================================================
*/
bool sxs_SimLimitRfTxPower (void)
{
#ifdef SIM_LIMIT_RF_TX_POWER
    return TRUE;
#else
    return FALSE;
#endif
}


