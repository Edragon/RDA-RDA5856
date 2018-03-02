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
// File name      : dma                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:48                                                  
// Last modified  : 2016-08-29 14:59:48                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:48 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DMA_H__
#define __DMA_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//get_channel
typedef union
{
    U32 v;
    struct
    {
        U32 get_channel                    :1 ; /*0 :0 , Returns 1 and locks the DMA channel for a transaction if it is available. Else returns 0. Clear the transfer done interrupt status.*/
        U32 get_channel_reserved_0         :31; /*31:1 , No description.               */
    } b;
} T_DMA_get_channel;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Status of the DMA: 1 if enabled, 0 if disabled.*/
        U32 int_done_cause                 :1 ; /*1 :1 , Cause of the interrupt. This bit is set when the transfer is done and the interrupt mask bit is set. Write one in the Int Clear or write 0 in Enable control bits to clear Int Done Cause bit.*/
        U32 int_done_status                :1 ; /*2 :2 , Status of the interrupt. Status of the transfer: 1 if the transfer is finished, 0 if it is not finished. Write one in the Int Clear or write 0 in Enable control bits to clear Int Done Status bit.*/
        U32 status_reserved_1              :1 ; /*3 :3 , No description.               */
        U32 channel_lock                   :1 ; /*4 :4 , Actual status of channel lock. Channel is unlocked at the end of transaction or when the DMA is disabled.*/
        U32 status_reserved_0              :27; /*31:5 , No description.               */
    } b;
} T_DMA_status;

//control
typedef union
{
    U32 v;
    struct
    {
        U32 control_enable                 :1 ; /*0 :0 , Controls the DMA. Write 1 to enable the DMA, write 0 to disable it. When 0 is written in this register, the Int Done Status and Cause bits are reset.*/
        U32 int_done_mask                  :1 ; /*1 :1 , End of transfer interrupt generation. When 1, the DMA will send an interrupt at transaction completion.*/
        U32 int_done_clear                 :1 ; /*2 :2 , Clear the transfer done interruption (this will clear Int Done Status and Int Done Cause). This bit is auto-clear. You will always read 0 here.*/
        U32 control_reserved_4             :1 ; /*3 :3 , No description.               */
        U32 use_pattern                    :1 ; /*4 :4 , If this bit is set, the source address will be ignored and the memory will be fill with the value of the pattern register.*/
        U32 control_reserved_3             :3 ; /*7 :5 , No description.               */
        U32 stop_transfer                  :1 ; /*8 :8 , The DMA stop the current transfer and flush his FIFO (write only bit). When the FIFO is empty and last write performed, the DMA is disabled and available for a next transfer. The number of bytes copied is readable on DMA_XFER_SIZE register.*/
        U32 control_reserved_2             :7 ; /*15:9 , No description.               */
        U32 fcs_enable                     :1 ; /*16:16, Enable FCS process when 1.0: NORMAL_DMA1:   FCS_PROCESS*/
        U32 control_reserved_1             :3 ; /*19:17, No description.               */
        U32 dst_addr_mgt                   :2 ; /*21:20, Destination address management.0: Normal_DMA, DMA_DST_ADDR register define the destination address.1: CONST_ADDR DMA write address is constant (no incremented) and defined by the DMA_DST_ADDR register. All data write are in 16-bit.2: ALTERN_ADDR DMA write address is alternatively defined by DMA_DST_ADDR and DMA_SD_DST_ADDR registers. All data write are in 16-bit. In this configuration, DMA write operation is alternatively: DMA_DST_ADDR &lt;= DMA_PATTERN register DMA_SD_DST_ADDR &lt;= Data[DMA_SRC_ADDR] 3: reserved*/
        U32 control_reserved_0             :10; /*31:22, No description.               */
    } b;
} T_DMA_control;

//src_addr
typedef union
{
    U32 v;
    struct
    {
        U32 src_address                    :32; /*31:0 , Source start read byte address. When a transfer is stalled by the Stop_Transfer bit, this register give the next current source address, which is directly the value to re-program to complete the transfer stopped.*/
    } b;
} T_DMA_src_addr;

//dst_addr
typedef union
{
    U32 v;
    struct
    {
        U32 dst_address                    :32; /*31:0 , Destination start read byte address. When a transfer is stalled by the Stop_Transfer bit, this register give the next current destination address, which is directly the value to re-program to complete the transfer stopped.*/
    } b;
} T_DMA_dst_addr;

//sd_dst_addr
typedef union
{
    U32 v;
    struct
    {
        U32 sd_dst_address                 :32; /*31:0 , Second destination address. This register is only used when Dst_Address_Mgt=10.*/
    } b;
} T_DMA_sd_dst_addr;

//xfer_size
typedef union
{
    U32 v;
    struct
    {
        U32 transfer_size                  :18; /*17:0 , Transfer size in bytes. Maximum: 262144 bytes. When a transfer is stopped by the Stop_Transfer bit, this register give the number of remainder bytes to transfer*/
        U32 xfer_size_reserved_0           :14; /*31:18, No description.               */
    } b;
} T_DMA_xfer_size;

//pattern
typedef union
{
    U32 v;
    struct
    {
        U32 pattern                        :32; /*31:0 , Value taken to fill the memory when the configuration bit Use Pattern is set. When the pattern mode is used the destination address must be 32-bit aligned and the transfer size multiple of 4. when Dst_Address_Mgt=10 Pattern is the data written at the address given by the Dst_Address register.*/
    } b;
} T_DMA_pattern;

//fcs
typedef union
{
    U32 v;
    struct
    {
        U32 fcs                            :24; /*23:0 , Frame Check Sequence.         */
        U32 fcs_reserved_0                 :7 ; /*30:24, No description.               */
        U32 fcs_correct                    :1 ; /*31:31, The FCS is correct in reception when the final remainder is equal to C(x)= x^22 + x^21 + x^19 + x^18 + x^16 + x^15 + x^11 + x^8 + x^5 + x^4*/
    } b;
} T_DMA_fcs;

typedef struct
{
    volatile               T_DMA_get_channel                    get_channel; /*0x0  , RO   , 0x00000001, No description.               */
    volatile                    T_DMA_status                         status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                   T_DMA_control                        control; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                  T_DMA_src_addr                       src_addr; /*0xc  , RW   , 0xFFFFFFFF, No description.               */
    volatile                  T_DMA_dst_addr                       dst_addr; /*0x10 , RW   , 0xFFFFFFFF, No description.               */
    volatile               T_DMA_sd_dst_addr                    sd_dst_addr; /*0x14 , RW   , 0xFFFFFFFF, No description.               */
    volatile                 T_DMA_xfer_size                      xfer_size; /*0x18 , RW   , 0x0003FFFF, No description.               */
    volatile                   T_DMA_pattern                        pattern; /*0x1c , RW   , 0xFFFFFFFF, No description.               */
    volatile                             U32                  reserved_1[3];
    volatile                       T_DMA_fcs                            fcs; /*0x2c , RO   , 0x00000000, No description.               */
} T_HWP_DMA_T;
#else
typedef struct
{
    volatile                             U32                    get_channel; /*0x0  , RO   , 0x00000001, No description.               */
    volatile                             U32                         status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                        control; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                       src_addr; /*0xc  , RW   , 0xFFFFFFFF, No description.               */
    volatile                             U32                       dst_addr; /*0x10 , RW   , 0xFFFFFFFF, No description.               */
    volatile                             U32                    sd_dst_addr; /*0x14 , RW   , 0xFFFFFFFF, No description.               */
    volatile                             U32                      xfer_size; /*0x18 , RW   , 0x0003FFFF, No description.               */
    volatile                             U32                        pattern; /*0x1c , RW   , 0xFFFFFFFF, No description.               */
    volatile                             U32                  reserved_2[3];
    volatile                             U32                            fcs; /*0x2c , RO   , 0x00000000, No description.               */
} T_HWP_DMA_T;

//get_channel
#define DMA_GET_CHANNEL                (1<<0)              


//status
#define DMA_ENABLE                     (1<<0)              

#define DMA_INT_DONE_CAUSE             (1<<1)              

#define DMA_INT_DONE_STATUS            (1<<2)              

#define DMA_CHANNEL_LOCK               (1<<4)              


//control
#define DMA_CONTROL_ENABLE             (1<<0)              

#define DMA_INT_DONE_MASK              (1<<1)              
#define DMA_INT_DONE_MASK_SHIFT     (1)
#define DMA_INT_DONE_CLEAR             (1<<2)              

#define DMA_USE_PATTERN                (1<<4)              

#define DMA_STOP_TRANSFER              (1<<8)              

#define DMA_FCS_ENABLE                 (1<<16)             
#define DMA_FCS_ENABLE_NORMAL_DMA      (0<<16)             
#define DMA_FCS_ENABLE_FCS_PROCESS     (1<<16)             

#define DMA_DST_ADDR_MGT(n)            (((n)&0x3)<<20)     
#define DMA_DST_ADDR_MGT_NORMAL_DMA   (((0)&0x3)<<20)     
#define DMA_DST_ADDR_MGT_CONST_ADDR    (((1)&0x3)<<20)     
#define DMA_DST_ADDR_MGT_ALTERN_ADDR   (((2)&0x3)<<20)     
#define DMA_DST_ADDR_MGT_RESERVED      (((3)&0x3)<<20)     
#define DMA_DST_ADDR_MGT_MASK          (0x3<<20)           
#define DMA_DST_ADDR_MGT_SHIFT         (20)                


//src_addr
#define DMA_SRC_ADDRESS(n)             (((n)&0xFFFFFFFF)<<0)
#define DMA_SRC_ADDRESS_MASK           (0xFFFFFFFF<<0)     
#define DMA_SRC_ADDRESS_SHIFT          (0)                 


//dst_addr
#define DMA_DST_ADDRESS(n)             (((n)&0xFFFFFFFF)<<0)
#define DMA_DST_ADDRESS_MASK           (0xFFFFFFFF<<0)     
#define DMA_DST_ADDRESS_SHIFT          (0)                 


//sd_dst_addr
#define DMA_SD_DST_ADDRESS(n)          (((n)&0xFFFFFFFF)<<0)
#define DMA_SD_DST_ADDRESS_MASK        (0xFFFFFFFF<<0)     
#define DMA_SD_DST_ADDRESS_SHIFT       (0)                 


//xfer_size
#define DMA_TRANSFER_SIZE(n)           (((n)&0x3FFFF)<<0)  
#define DMA_TRANSFER_SIZE_MASK         (0x3FFFF<<0)        
#define DMA_TRANSFER_SIZE_SHIFT        (0)                 


//pattern
#define DMA_PATTERN(n)                 (((n)&0xFFFFFFFF)<<0)
#define DMA_PATTERN_MASK               (0xFFFFFFFF<<0)     
#define DMA_PATTERN_SHIFT              (0)                 


//fcs
#define DMA_FCS(n)                     (((n)&0xFFFFFF)<<0) 
#define DMA_FCS_MASK                   (0xFFFFFF<<0)       
#define DMA_FCS_SHIFT                  (0)                 

#define DMA_FCS_CORRECT                (1<<31)             


#endif

#endif


