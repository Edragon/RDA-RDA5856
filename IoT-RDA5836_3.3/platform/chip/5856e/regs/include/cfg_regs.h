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
/// @file
//
//==============================================================================

#ifndef _CFG_REGS_H_
#define _CFG_REGS_H_
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define hwp_configRegs hwp_cfg_regs

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//chip_id
typedef union
{
    U32 v;
    struct
    {
        U32 metal_id                       :12; /*11:0 , ECO identification number.    */
        U32 ext_prod_id                    :4 ; /*15:12, Extended product identification number.*/
        U32 prod_id                        :16; /*31:16, Product identification number.*/
    } b;
} T_CFG_REGS_chip_id;

//build_version
typedef union
{
    U32 v;
    struct
    {
        U32 day                            :8 ; /*7 :0 , Day                           */
        U32 month                          :8 ; /*15:8 , Month                         */
        U32 year                           :8 ; /*23:16, Minor part of year            */
        U32 major                          :8 ; /*31:24, Major part of year            */
    } b;
} T_CFG_REGS_build_version;

typedef struct
{
    volatile              T_CFG_REGS_chip_id                        chip_id; /*0x0  , RO   , 0x5856E000, Chip Identify Register        */
    volatile        T_CFG_REGS_build_version                  build_version; /*0x4  , RO   , 0x20150730, Build Version Register        */
} T_HWP_CFG_REGS_T;
#else
typedef struct
{
    volatile                             U32                        chip_id; /*0x0  , RO   , 0x5856E000, Chip Identify Register        */
    volatile                             U32                  build_version; /*0x4  , RO   , 0x20150730, Build Version Register        */
} T_HWP_CFG_REGS_T;

#define CFG_REGS_METAL_ID(n)           (((n)&0xFFF)<<0)    
#define CFG_REGS_METAL_ID_MASK         (0xFFF<<0)          
#define CFG_REGS_METAL_ID_SHIFT        (0)                 

#define CFG_REGS_EXT_PROD_ID(n)        (((n)&0xF)<<12)     
#define CFG_REGS_EXT_PROD_ID_MASK      (0xF<<12)           
#define CFG_REGS_EXT_PROD_ID_SHIFT     (12)                

#define CFG_REGS_PROD_ID(n)            (((n)&0xFFFF)<<16)  
#define CFG_REGS_PROD_ID_MASK          (0xFFFF<<16)        
#define CFG_REGS_PROD_ID_SHIFT         (16)                


#define CFG_REGS_DAY(n)                (((n)&0xFF)<<0)     
#define CFG_REGS_DAY_MASK              (0xFF<<0)           
#define CFG_REGS_DAY_SHIFT             (0)                 

#define CFG_REGS_MONTH(n)              (((n)&0xFF)<<8)     
#define CFG_REGS_MONTH_MASK            (0xFF<<8)           
#define CFG_REGS_MONTH_SHIFT           (8)                 

#define CFG_REGS_YEAR(n)               (((n)&0xFF)<<16)    
#define CFG_REGS_YEAR_MASK             (0xFF<<16)          
#define CFG_REGS_YEAR_SHIFT            (16)                

#define CFG_REGS_MAJOR(n)              (((n)&0xFF)<<24)    
#define CFG_REGS_MAJOR_MASK            (0xFF<<24)          
#define CFG_REGS_MAJOR_SHIFT           (24)                


#endif

#endif


