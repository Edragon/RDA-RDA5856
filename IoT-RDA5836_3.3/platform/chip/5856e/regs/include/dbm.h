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
// File name      : dbm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:41                                                  
// Last modified  : 2016-12-21 13:55:41                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:41 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DBM_H__
#define __DBM_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//sel
typedef union
{
    U32 v;
    struct
    {
        U32 dbm_en                         :1 ; /*0 :0 , debug bus master enable       */
        U32 sel_reserved_0                 :31; /*31:1 , No description.               */
    } b;
} T_DBM_sel;

//trans
typedef union
{
    U32 v;
    struct
    {
        U32 burst_len                      :4 ; /*3 :0 , length of burst when incrxmax incr16*/
        U32 burst_type                     :2 ; /*5 :4 , burst type2'b00:: single2'b01:: incr42'b10:: incr82'b11:: incrx*/
        U32 wrap_en                        :1 ; /*6 :6 , wrap the whole fifo, when EOF, keep on write by the start addr*/
        U32 dump_trig_sel                  :1 ; /*7 :7 , dump trigger selectionselect trigger from soft or hardware*/
        U32 data_sel                       :4 ; /*11:8 , data selection                */
        U32 word_len                       :20; /*31:12, transfer word length          */
    } b;
} T_DBM_trans;

//start_addr
typedef union
{
    U32 v;
    struct
    {
        U32 start_addr                     :32; /*31:0 , start address of transfer     */
    } b;
} T_DBM_start_addr;

//mask
typedef union
{
    U32 v;
    struct
    {
        U32 mask_comp_end                  :1 ; /*0 :0 , mask for comp end             */
        U32 mask_comp_half                 :1 ; /*1 :1 , mask for comp half            */
        U32 mask_ovfl                      :1 ; /*2 :2 , mask for ovfl                 */
        U32 mask_trans_err                 :1 ; /*3 :3 , mask for transfer error       */
        U32 mask_reserved_0                :28; /*31:4 , No description.               */
    } b;
} T_DBM_mask;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 srst_done_status               :1 ; /*0 :0 , data_packer soft reset done   */
        U32 int_comp_end_status            :1 ; /*1 :1 , end interrupt from transfer complete*/
        U32 int_comp_half_status           :1 ; /*2 :2 , half interrupt from transfer complete*/
        U32 int_ovfl_status                :1 ; /*3 :3 , interrupt from fifo overfolw  */
        U32 int_trans_err_status           :1 ; /*4 :4 , interrupt for ahb transfer error*/
        U32 status_reserved_0              :27; /*31:5 , No description.               */
    } b;
} T_DBM_status;

//soft_reset
typedef union
{
    U32 v;
    struct
    {
        U32 s_reset                        :1 ; /*0 :0 , soft reset                    */
        U32 soft_reset_reserved_0          :31; /*31:1 , No description.               */
    } b;
} T_DBM_soft_reset;

//soft_trig
typedef union
{
    U32 v;
    struct
    {
        U32 s_dp_trig                      :1 ; /*0 :0 , dump trigger from soft        */
        U32 soft_trig_reserved_0           :31; /*31:1 , No description.               */
    } b;
} T_DBM_soft_trig;

typedef struct
{
    volatile                       T_DBM_sel                            sel; /*0x0  , RW   , 0x00000000, Bluetooth device address      */
    volatile                     T_DBM_trans                          trans; /*0x4  , RW   , 0x00000000, Bluetooth device address      */
    volatile                T_DBM_start_addr                     start_addr; /*0x8  , RW   , 0x00000000, Bluetooth device address      */
    volatile                      T_DBM_mask                           mask; /*0xc  , RW   , 0x00000000, Bluetooth device address      */
    volatile                    T_DBM_status                         status; /*0x10 , RW   , 0x00000000, Bluetooth device address      */
    volatile                T_DBM_soft_reset                     soft_reset; /*0x14 , RW   , 0x00000000, Bluetooth device address      */
    volatile                 T_DBM_soft_trig                      soft_trig; /*0x18 , RW   , 0x00000000, Bluetooth device address      */
} T_HWP_DBM_T;
#else
typedef struct
{
    volatile                             U32                            sel; /*0x0  , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                          trans; /*0x4  , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                     start_addr; /*0x8  , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                           mask; /*0xc  , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                         status; /*0x10 , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                     soft_reset; /*0x14 , RW   , 0x00000000, Bluetooth device address      */
    volatile                             U32                      soft_trig; /*0x18 , RW   , 0x00000000, Bluetooth device address      */
} T_HWP_DBM_T;

#define DBM_DBM_EN                     (1<<0)              


#define DBM_BURST_LEN(n)               (((n)&0xF)<<0)      
#define DBM_BURST_LEN_MASK             (0xF<<0)            
#define DBM_BURST_LEN_SHIFT            (0)                 

#define DBM_BURST_TYPE(n)              (((n)&0x3)<<4)      
#define DBM_BURST_TYPE_SINGLE          (((0)&0x3)<<4)      
#define DBM_BURST_TYPE_INCR4           (((1)&0x3)<<4)      
#define DBM_BURST_TYPE_INCR8           (((2)&0x3)<<4)      
#define DBM_BURST_TYPE_INCRX           (((3)&0x3)<<4)      
#define DBM_BURST_TYPE_MASK            (0x3<<4)            
#define DBM_BURST_TYPE_SHIFT           (4)                 

#define DBM_WRAP_EN                    (1<<6)              

#define DBM_DUMP_TRIG_SEL              (1<<7)              

#define DBM_DATA_SEL(n)                (((n)&0xF)<<8)      
#define DBM_DATA_SEL_MASK              (0xF<<8)            
#define DBM_DATA_SEL_SHIFT             (8)                 

#define DBM_WORD_LEN(n)                (((n)&0xFFFFF)<<12) 
#define DBM_WORD_LEN_MASK              (0xFFFFF<<12)       
#define DBM_WORD_LEN_SHIFT             (12)                


#define DBM_START_ADDR(n)              (((n)&0xFFFFFFFF)<<0)
#define DBM_START_ADDR_MASK            (0xFFFFFFFF<<0)     
#define DBM_START_ADDR_SHIFT           (0)                 


#define DBM_MASK_COMP_END              (1<<0)              

#define DBM_MASK_COMP_HALF             (1<<1)              

#define DBM_MASK_OVFL                  (1<<2)              

#define DBM_MASK_TRANS_ERR             (1<<3)              


#define DBM_SRST_DONE_STATUS           (1<<0)              

#define DBM_INT_COMP_END_STATUS        (1<<1)              

#define DBM_INT_COMP_HALF_STATUS       (1<<2)              

#define DBM_INT_OVFL_STATUS            (1<<3)              

#define DBM_INT_TRANS_ERR_STATUS       (1<<4)              


#define DBM_S_RESET                    (1<<0)              


#define DBM_S_DP_TRIG                  (1<<0)              


#endif

#endif


