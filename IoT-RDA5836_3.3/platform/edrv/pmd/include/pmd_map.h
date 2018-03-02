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

#ifndef _PMD_MAP_H_
#define _PMD_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
#define PMD_MAP_POWER_ID_QTY                     (14)
#else
#define PMD_MAP_POWER_ID_QTY                     (15)
#endif
#define PMD_MAP_LEVEL_ID_QTY                     (9)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// PMD_MAP_CHARGER_STATE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32                         status;                       //0x00000000
    UINT32                         current;                      //0x00000004
    VOID*                          handler;                      //0x00000008
    UINT16                         batteryLevel;                 //0x0000000C
    UINT16                         pulsedOnCount;                //0x0000000E
    UINT16                         pulsedOffCount;               //0x00000010
    UINT16                         pulsedCycleCount;             //0x00000012
    UINT16                         pulsedOnMeanCount;            //0x00000014
    UINT16                         pulsedOffMeanCount;           //0x00000016
    UINT32                         startTime;                    //0x00000018
    UINT32                         lastWTime;                    //0x0000001C
    BOOL                           highActivityState;            //0x00000020
} PMD_MAP_CHARGER_STATE_T; //Size : 0x24



// ============================================================================
// PMD_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of the module.
// =============================================================================
typedef struct
{
    UINT32                         powerInfo[PMD_MAP_POWER_ID_QTY]; //0x00000000
    /// Current level value.
    UINT32                         levelInfo[PMD_MAP_LEVEL_ID_QTY]; //0x00000038
    PMD_MAP_CHARGER_STATE_T*       chargerState;                 //0x00000060
} PMD_MAP_ACCESS_T; //Size : 0x64

//powerInfo
#define PMD_MAP_ACCESS_T_POWER_SHARED_LINK_FLAG (1<<31)
#define PMD_MAP_ACCESS_T_POWER_VALUE(n) (((n)&0x7FFFFFFF)<<0)






// =============================================================================
// pmd_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
// =============================================================================
PUBLIC VOID pmd_RegisterYourself(VOID);

    

#endif

