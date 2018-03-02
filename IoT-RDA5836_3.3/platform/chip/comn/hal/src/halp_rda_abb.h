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
/// @file halp_rda_abb.h                                                      //
///                                                                           //
/// This file decribes the registers of RDA Analog Base Band.                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HALP_RDA_ABB_H_
#define _HALP_RDA_ABB_H_

#include "cs_types.h"

// ----------------
// APIs
// ----------------

//=============================================================================
// hal_AbbIspiRead
//-----------------------------------------------------------------------------
/// Read codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @return register value                 0 write 1 read
//=============================================================================
PROTECTED UINT32 hal_AbbIspiRead(UINT32 regIdx);


//=============================================================================
// hal_AbbIspiWrite
//-----------------------------------------------------------------------------
/// Write codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @param value register value
//=============================================================================
PROTECTED VOID hal_AbbIspiWrite(UINT32 regIdx, UINT32 value);


PROTECTED VOID hal_AbbIspiReadOutputSelect(UINT32 addr);

#endif // _HALP_RDA_ABB_H_

