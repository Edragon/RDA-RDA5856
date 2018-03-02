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

                                                                            
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                                                                                                         
//==============================================================================
//
/// @file
/// This file contains the portion of the module's memory map that will be accessible
/// through CoolWatcher (or any CoolXml tool). It is also used for the get version
/// mechanism.
//
//==============================================================================

#ifndef _RFD_MAP_H_
#define _RFD_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RFD_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of the module.
// =============================================================================
typedef struct
{
    UINT32              reserved;
} RFD_MAP_ACCESS_T; //Size : 0x0






// =============================================================================
// rfd_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
// =============================================================================
PUBLIC VOID rfd_RegisterYourself(VOID);

    

#endif

