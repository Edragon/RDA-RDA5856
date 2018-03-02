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
//------------------------------------------------------------------------
// File name      : iomux                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:41                                                  
// Last modified  : 2016-12-14 15:57:41                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:41 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __IOMUX_H__
#define __IOMUX_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//cfg
typedef union
{
    U32 v;
    struct
    {
        U32 func_sel                       :4 ; /*3 :0 , pad functional selection      */
        U32 out_reg                        :1 ; /*4 :4 , out force value               */
        U32 out_force                      :1 ; /*5 :5 , out value force selection
1'b1: use out_reg
1'b0: use functional out*/
        U32 cfg_reserved_4                 :2 ; /*7 :6 , No description.               */
        U32 oen_reg                        :1 ; /*8 :8 , oen force value               */
        U32 oen_force                      :1 ; /*9 :9 , oen value force selection
1'b1: use oen_reg
1'b0: use functional oen*/
        U32 cfg_reserved_3                 :2 ; /*11:10, No description.               */
        U32 pu_reg                         :1 ; /*12:12, pu force value                */
        U32 pd_reg                         :1 ; /*13:13, pd force value                */
        U32 pull_force                     :1 ; /*14:14, pull up/down value force selection
1'b1: use pu_reg/pd_reg
1'b0: use functional pu/pd*/
        U32 cfg_reserved_2                 :5 ; /*19:15, No description.               */
        U32 drv                            :2 ; /*21:20, drive strength                */
        U32 cfg_reserved_1                 :2 ; /*23:22, No description.               */
        U32 pdn                            :1 ; /*24:24, power up                      */
        U32 cfg_reserved_0                 :7 ; /*31:25, No description.               */
    } b;
} T_IOMUX_cfg;

typedef struct
{
    volatile                     T_IOMUX_cfg                        cfg[14]; /*0x0  , RW   , 0x01200000, No description.               */
} T_HWP_IOMUX_T;
#else
typedef struct
{
    volatile                             U32                        cfg[14]; /*0x0  , RW   , 0x01200000, No description.               */
} T_HWP_IOMUX_T;

//cfg
#define IOMUX_FUNC_SEL(n)              (((n)&0xF)<<0)      
#define IOMUX_FUNC_SEL_MASK            (0xF<<0)            
#define IOMUX_FUNC_SEL_SHIFT           (0)                 

#define IOMUX_OUT_REG                  (1<<4)              

#define IOMUX_OUT_FORCE                (1<<5)              
#define IOMUX_OUT_FORCE_USE_OUT_REG            (1<<5)              
#define IOMUX_OUT_FORCE_USE_FUNC_OUT            (0<<5)              

#define IOMUX_OEN_REG                  (1<<8)              

#define IOMUX_OEN_FORCE                (1<<9)              
#define IOMUX_OEN_FORCE_USE_OEN_REG            (1<<9)              
#define IOMUX_OEN_FORCE_USE_FUNC_OEN            (0<<9)              

#define IOMUX_PU_REG                   (1<<12)             

#define IOMUX_PD_REG                   (1<<13)             

#define IOMUX_PULL_FORCE               (1<<14)             
#define IOMUX_PULL_FORCE_USE_PU_PD_REG           (1<<14)             
#define IOMUX_PULL_FORCE_USE_FUNC_PU_PD           (0<<14)             

#define IOMUX_DRV(n)                   (((n)&0x3)<<20)     
#define IOMUX_DRV_MASK                 (0x3<<20)           
#define IOMUX_DRV_SHIFT                (20)                

#define IOMUX_PDN                      (1<<24)         


#endif

#endif


