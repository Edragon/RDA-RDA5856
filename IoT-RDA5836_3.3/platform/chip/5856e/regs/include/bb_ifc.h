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
// File name      : bb_ifc                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:29                                                  
// Last modified  : 2016-12-14 15:57:29                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:29 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __BB_IFC_H__
#define __BB_IFC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#include "globals.h"

#define hwp_bbIfc hwp_bb_ifc

// =============================================================================
//  MACROS
// =============================================================================
#define BB_IFC_ADDR_LEN                          (15)
#define BB_IFC_ADDR_ALIGN                        (2)
#define BB_IFC_TC_LEN                            (8)

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//control
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Channel Enable, write one in this bit enable the channel. When the channel is enabled, for a peripheral to memory transfer the DMA wait request from peripheral to start transfer.*/
        U32 disable                        :1 ; /*1 :1 , Channel Disable, write one in this bit disable the channel. When writing one in this bit, the current AHB transfer and current APB transfer (if one in progress) is completed and the channel is then disabled.*/
        U32 control_reserved_1             :2 ; /*3 :2 , No description.               */
        U32 auto_disable                   :1 ; /*4 :4 , Automatic channel Disable. When this bit is set, the channel is automatically disabled at the next interrupt.*/
        U32 control_reserved_0             :27; /*31:5 , No description.               */
    } b;
} T_BB_IFC_control;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , When 1 the channel is enabled */
        U32 reserved_4                     :1 ; /*1 :1 ,                               */
        U32 status_reserved_3              :2 ; /*3 :2 , No description.               */
        U32 fifo_empty                     :1 ; /*4 :4 , When 1 the fifo is empty      */
        U32 status_reserved_2              :3 ; /*7 :5 , No description.               */
        U32 cause_ief                      :1 ; /*8 :8 , Cause interrupt End of FIFO.  */
        U32 cause_ihf                      :1 ; /*9 :9 , Cause interrupt Half of FIFO. */
        U32 cause_i4f                      :1 ; /*10:10, Cause interrupt Quarter of FIFO.*/
        U32 cause_i3_4f                    :1 ; /*11:11, Cause interrupt Three Quarter of FIFO.*/
        U32 status_reserved_1              :4 ; /*15:12, No description.               */
        U32 ief                            :1 ; /*16:16, End of FIFO interrupt status bit.*/
        U32 ihf                            :1 ; /*17:17, Half of FIFO interrupt status bit.*/
        U32 i4f                            :1 ; /*18:18, Quarter of FIFO interrupt status bit.*/
        U32 i3_4f                          :1 ; /*19:19, Three Quarter of FIFO interrupt status bit.*/
        U32 status_reserved_0              :12; /*31:20, No description.               */
    } b;
} T_BB_IFC_status;

//start_addr
typedef union
{
    U32 v;
    struct
    {
        U32 start_addr_reserved_0          :2 ; /*1 :0 , No description.               */
        U32 start_addr                     :30; /*31:2 , AHB Start Address. This field represent the start address of the FIFO located in RAM.*/
    } b;
} T_BB_IFC_start_addr;

//fifo_size
typedef union
{
    U32 v;
    struct
    {
        U32 fifo_size_reserved_1           :4 ; /*3 :0 , No description.               */
        U32 fifo_size                      :11; /*14:4 , Fifo size in bytes, max 32kBytes. The size of the fifo must be a multiple of 16 (The four LSB are always zero).*/
        U32 fifo_size_reserved_0           :17; /*31:15, No description.               */
    } b;
} T_BB_IFC_fifo_size;

//reserved
typedef union
{
    U32 v;
    struct
    {
        U32 reserved_reserved_0            :32; /*31:0 , No description.               */
    } b;
} T_BB_IFC_reserved;

//int_mask
typedef union
{
    U32 v;
    struct
    {
        U32 int_mask_reserved_1            :8 ; /*7 :0 , No description.               */
        U32 end_fifo                       :1 ; /*8 :8 , END FIFO Mask interrupt. When one this interrupt is enabled.*/
        U32 half_fifo                      :1 ; /*9 :9 , HALF FIFO Mask interrupt. When one this interrupt is enabled.*/
        U32 quarter_fifo                   :1 ; /*10:10, QUARTER FIFO Mask interrupt. When one this interrupt is enabled.*/
        U32 three_quarter_fifo             :1 ; /*11:11, THREE QUARTER FIFO Mask interrupt. When one this interrupt is enabled.*/
        U32 int_mask_reserved_0            :20; /*31:12, No description.               */
    } b;
} T_BB_IFC_int_mask;

//int_clear
typedef union
{
    U32 v;
    struct
    {
        U32 int_clear_reserved_1           :8 ; /*7 :0 , No description.               */
        U32 end_fifo                       :1 ; /*8 :8 , Write one to clear end of fifo interrupt.*/
        U32 half_fifo                      :1 ; /*9 :9 , Write one to clear half of fifo interrupt.*/
        U32 quarter_fifo                   :1 ; /*10:10, Write one to clear Quarter fifo interrupt.*/
        U32 three_quarter_fifo             :1 ; /*11:11, Write one to clear Three Quarter fifo interrupt.*/
        U32 int_clear_reserved_0           :20; /*31:12, No description.               */
    } b;
} T_BB_IFC_int_clear;

//cur_ahb_addr
typedef union
{
    U32 v;
    struct
    {
        U32 cur_ahb_addr_reserved_0        :2 ; /*1 :0 , No description.               */
        U32 cur_ahb_addr                   :30; /*31:2 , Current AHB address value. The nine MSB bit is constant and equal to the PAGE_ADDR field in the IFC_CH_AHB_START_ADDR register.*/
    } b;
} T_BB_IFC_cur_ahb_addr;

typedef struct
{
    volatile                T_BB_IFC_control                        control;
    volatile                 T_BB_IFC_status                         status;
    volatile             T_BB_IFC_start_addr                     start_addr;
    volatile              T_BB_IFC_fifo_size                      fifo_size;
    volatile               T_BB_IFC_reserved                       reserved;
    volatile               T_BB_IFC_int_mask                       int_mask;
    volatile              T_BB_IFC_int_clear                      int_clear;
    volatile           T_BB_IFC_cur_ahb_addr                   cur_ahb_addr;

} T_bbifc_ch;

typedef struct
{
    volatile                      T_bbifc_ch                          ch[3]; /*     , bbifc_ch, NA        , struct block                  */
} T_HWP_BB_IFC_T;
#else
typedef struct
{
    volatile                U32                        control;
    volatile                U32                         status;
    volatile                U32                     start_addr;
    volatile                U32                      fifo_size;
    volatile                U32                       reserved;
    volatile                U32                       int_mask;
    volatile                U32                      int_clear;
    volatile                U32                   cur_ahb_addr;

} T_bbifc_ch;

typedef struct
{
    volatile                      T_bbifc_ch                          ch[3]; /*     , bbifc_ch, NA        , struct block                  */
} T_HWP_BB_IFC_T;

//ch_0__control
#define BB_IFC_ENABLE                  (1<<0)              

#define BB_IFC_DISABLE                 (1<<1)              

#define BB_IFC_AUTO_DISABLE            (1<<4)              


//ch_0__status
#define BB_IFC_ENABLE                  (1<<0)              

#define BB_IFC_FIFO_EMPTY              (1<<4)              

#define BB_IFC_CAUSE_IEF               (1<<8)              

#define BB_IFC_CAUSE_IHF               (1<<9)              

#define BB_IFC_CAUSE_I4F               (1<<10)             

#define BB_IFC_CAUSE_I3_4F             (1<<11)             

#define BB_IFC_IEF                     (1<<16)             

#define BB_IFC_IHF                     (1<<17)             

#define BB_IFC_I4F                     (1<<18)             

#define BB_IFC_I3_4F                   (1<<19)             


//ch_0__start_addr
#define BB_IFC_START_ADDR(n)           (((n)&0x3FFFFFFF)<<2)
#define BB_IFC_START_ADDR_MASK         (0x3FFFFFFF<<2)     
#define BB_IFC_START_ADDR_SHIFT        (2)                 


//ch_0__fifo_size
#define BB_IFC_FIFO_SIZE(n)            (((n)&0x7FF)<<4)    
#define BB_IFC_FIFO_SIZE_MASK          (0x7FF<<4)          
#define BB_IFC_FIFO_SIZE_SHIFT         (4)                 


//ch_0__reserved

//ch_0__int_mask
#define BB_IFC_END_FIFO                (1<<8)              

#define BB_IFC_HALF_FIFO               (1<<9)              

#define BB_IFC_QUARTER_FIFO            (1<<10)             

#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             


//ch_0__int_clear
#define BB_IFC_END_FIFO                (1<<8)              

#define BB_IFC_HALF_FIFO               (1<<9)              

#define BB_IFC_QUARTER_FIFO            (1<<10)             

#define BB_IFC_THREE_QUARTER_FIFO      (1<<11)             


//ch_0__cur_ahb_addr
#define BB_IFC_CUR_AHB_ADDR(n)         (((n)&0x3FFFFFFF)<<2)
#define BB_IFC_CUR_AHB_ADDR_MASK       (0x3FFFFFFF<<2)     
#define BB_IFC_CUR_AHB_ADDR_SHIFT      (2)                 


#endif

#endif


