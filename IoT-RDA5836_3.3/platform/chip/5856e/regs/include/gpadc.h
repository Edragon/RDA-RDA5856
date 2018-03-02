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
// File name      : gpadc                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:49                                                  
// Last modified  : 2016-08-29 14:59:49                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:49 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __GPADC_H__
#define __GPADC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//key_data
typedef union
{
    U32 v;
    struct
    {
        U32 key_value_bit                  :10; /*9 :0 , Key Value.                    */
        U32 key_value_valid                :1 ; /*10:10, Key  Value valid.             */
        U32 key_data_reserved_0            :21; /*31:11, No description.               */
    } b;
} T_GPADC_key_data;

//gpadc_data_ch
typedef union
{
    U32 v;
    struct
    {
        U32 gpadc_value_bit                :10; /*9 :0 , GPADC Value.                  */
        U32 gpadc_value_valid              :1 ; /*10:10, GPADC Value valid.            */
        U32 gpadc_data_ch_reserved_0       :20; /*30:11, No description.               */
        U32 gpadc_en                       :1 ; /*31:31, No description.               */
    } b;
} T_GPADC_gpadc_data_ch;

typedef struct
{
    volatile                T_GPADC_key_data                       key_data; /*0x0  , RO   , 0x00000000, No description.               */
    volatile           T_GPADC_gpadc_data_ch               gpadc_data_ch[8]; /*0x4  , RW   , 0x00000000, No description.               */
} T_HWP_GPADC_T;
#else
typedef struct
{
    volatile                             U32                       key_data; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32               gpadc_data_ch[8]; /*0x4  , RW   , 0x00000000, No description.               */
} T_HWP_GPADC_T;

//key_data
#define GPADC_KEY_VALUE_BIT(n)         (((n)&0x3FF)<<0)    
#define GPADC_KEY_VALUE_BIT_MASK       (0x3FF<<0)          
#define GPADC_KEY_VALUE_BIT_SHIFT      (0)                 

#define GPADC_KEY_VALUE_VALID          (1<<10)             


//gpadc_data_ch
#define GPADC_GPADC_VALUE_BIT(n)       (((n)&0x3FF)<<0)    
#define GPADC_GPADC_VALUE_BIT_MASK     (0x3FF<<0)          
#define GPADC_GPADC_VALUE_BIT_SHIFT    (0)                 

#define GPADC_GPADC_VALUE_VALID        (1<<10)             

#define GPADC_GPADC_EN                 (1<<31)             


#endif

#endif


