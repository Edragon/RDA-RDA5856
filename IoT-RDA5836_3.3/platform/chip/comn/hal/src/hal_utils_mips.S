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
/// @file    hal_utils_mips.S
/// 
/// This file contains utility functions not directly linked to an 
/// hardware module
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "regdef.h"
#include "cp0.h"

//  Code works both in mips16 and mips32 

// mips16 assembler is cold with debugging feature.
// We are consequently forced to use mips32 for 
// assembly files.
    .set nomips16
    .text
    .set    reorder

// ***********************************************************************
// ***********************************************************************
//                                                                        
//  CPU context utilities                                                 
//                                                                        
// ***********************************************************************
// ***********************************************************************

// =============================================================================
// hal_SysGetSp
// -----------------------------------------------------------------------------
/// This function returns the current value of the stack register
/// @return The current value of the stack pointer.
// =============================================================================
// ***********************************************************************
/// 
/// 
///     Get current Stack Pointer value      
///                                            
///     CALLED BY
///     
///         Application                                           
///                                                             
///     CALLS 
///     
///         None
///     
///     INPUTS
///     
///         None                                 
///                                                             
///     OUTPUTS
///         
///         Stack Pointer value                             
/// 
// ***********************************************************************
//UINT32 hal_SysGetSp(void)
//{
    .globl  hal_SysGetSp
    .globl  hal_SysGetSp
    .ent    hal_SysGetSp
hal_SysGetSp:
hal_SysGetSp:    
    .frame sp, 0, ra

    move    v0,sp

    j    ra

    .type   hal_SysGetSp,@function
    .size   hal_SysGetSp,.-hal_SysGetSp

    .end hal_SysGetSp
//}

