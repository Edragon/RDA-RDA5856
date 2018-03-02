/*************************************************************************/
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
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*                                                                       */
/*      singen.h                                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      Generate a sine wave at a specified frequency and amplitude.     */
/*                                                                       */
/*                                          */
/*                                                                       */
/*************************************************************************/

#ifndef _SINGEN_H_
#define _SINGEN_H_


#include "cs_types.h"



// =============================================================================
// sinGen
// -----------------------------------------------------------------------------
/// Generate a sine wave
/// @param f Frequency
/// @param ampl Amplitude
/// @param sinBuf Pointer to the buffer where the sinwave is stored.
/// @param bufSize Size of the buffer, in bytes.
// =============================================================================
PROTECTED VOID sinGen(UINT16 f, UINT8 ampl, UINT32 *sinBuf, UINT32 bufSize);



#endif /* _SINGEN_H_ */

