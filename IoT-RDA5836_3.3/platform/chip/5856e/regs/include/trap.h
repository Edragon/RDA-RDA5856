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
// File name      : trap                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:57                                                  
// Last modified  : 2016-12-21 13:55:57                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:57 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __TRAP_H__
#define __TRAP_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//addr_in
typedef union
{
    U32 v;
    struct
    {
        U32 addr_in                        :16; /*15:0 , address to be trapped         */
        U32 addr_in_reserved_0             :16; /*31:16, No description.               */
    } b;
} T_TRAP_addr_in;

//addr_out
typedef union
{
    U32 v;
    struct
    {
        U32 addr_out                       :28; /*27:0 , address to trap to            */
        U32 addr_out_reserved_0            :4 ; /*31:28, No description.               */
    } b;
} T_TRAP_addr_out;

//en
typedef union
{
    U32 v;
    struct
    {
        U32 en                             :16; /*15:0 , trap enable for 16 channels   */
        U32 en_reserved_0                  :16; /*31:16, No description.               */
    } b;
} T_TRAP_en;

typedef struct
{
    volatile                  T_TRAP_addr_in                    addr_in[16]; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                 T_TRAP_addr_out                   addr_out[16]; /*0x40 , RW   , 0x00000000, No description.               */
    volatile                       T_TRAP_en                             en; /*0x80 , RW   , 0x00000000, No description.               */
} T_HWP_TRAP_T;
#else
typedef struct
{
    volatile                             U32                    addr_in[16]; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                             U32                   addr_out[16]; /*0x40 , RW   , 0x00000000, No description.               */
    volatile                             U32                             en; /*0x80 , RW   , 0x00000000, No description.               */
} T_HWP_TRAP_T;

#define TRAP_ADDR_IN(n)                (((n)&0xFFFF)<<0)   
#define TRAP_ADDR_IN_MASK              (0xFFFF<<0)         
#define TRAP_ADDR_IN_SHIFT             (0)                 


#define TRAP_ADDR_OUT(n)               (((n)&0xFFFFFFF)<<0)
#define TRAP_ADDR_OUT_MASK             (0xFFFFFFF<<0)      
#define TRAP_ADDR_OUT_SHIFT            (0)                 


#define TRAP_EN(n)                     (((n)&0xFFFF)<<0)   
#define TRAP_EN_MASK                   (0xFFFF<<0)         
#define TRAP_EN_SHIFT                  (0)                 


#endif

#endif


