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


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calibp_stub.h                                                       //
/// That file provides the private API for the calibration stub               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _CALIBP_STUB_H_
#define _CALIBP_STUB_H_

#include "cs_types.h"
#include "calib_m.h"



// =============================================================================
// g_calibStubGlobalCtxPtr
// -----------------------------------------------------------------------------
/// Pointert to the calibration context written by the Host to send command
/// to the CES.
// =============================================================================
EXPORT PROTECTED VOLATILE CALIB_STUB_CTX_T*    g_calibStubGlobalCtxPtr;



// =============================================================================
// g_calibStubLocalCtx
// -----------------------------------------------------------------------------
/// Context used by the CES, copied from the #g_calibStubGlobalCtx.
// =============================================================================
EXPORT PROTECTED CALIB_STUB_CTX_T            g_calibStubLocalCtx;



// =============================================================================
// calib_DispState
// -----------------------------------------------------------------------------
/// Display depending on the states.
/// @param full If \c TRUE,  clear the screen and rewrite everything.
// =============================================================================
PROTECTED VOID calib_DispState(BOOL full);



// =============================================================================
// calib_PulseLight
// -----------------------------------------------------------------------------
/// I am alive and I claim it ! function, through a PWL (Keyboard backlighting
/// presumably. The pulsing speed is used to display the calibration state.
///
/// @param speed Speed of the glow.
// =============================================================================
PROTECTED VOID calib_PulseLight(INT32 speed);




// =============================================================================
// calib_StubOpen
// -----------------------------------------------------------------------------
/// Initializes the calibration stub. Among operation is the registering of the 
/// calib stub state machine as the FINT Irq handler.
// =============================================================================
PROTECTED VOID calib_StubOpen(VOID);



// =============================================================================
// calib_StubTaskInit
// -----------------------------------------------------------------------------
/// Calib Stub OS task.
// =============================================================================
PROTECTED VOID calib_StubTaskInit(VOID);


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================





#endif // _CALIBP_STUB_H_

