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
// File name      : debug_host                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:47                                                  
// Last modified  : 2016-08-29 14:59:47                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:47 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __DEBUG_HOST_H__
#define __DEBUG_HOST_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define hwp_debugHost hwp_debug_host

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//cmd
typedef union
{
    U32 v;
    struct
    {
        U32 addr                           :29; /*28:0 , Note:Address of data to be read or written.*/
        U32 size                           :2 ; /*30:29, Note:These two bits indicates element data size. 0:byte1:half_word 2:word*/
        U32 write_h                        :1 ; /*31:31, Note:This bit indicates command is read or write. 0:Read 1:Write*/
    } b;
} T_DEBUG_HOST_cmd;

//data
typedef union
{
    U32 v;
    struct
    {
        U32 data                           :32; /*31:0 , Note:Those bits are data to be read or written by IFC.*/
    } b;
} T_DEBUG_HOST_data;

//event
typedef union
{
    U32 v;
    struct
    {
        U32 event0_sema                    :1 ; /*0 :0 , Note:When read, this bit is used for event semaphore. If host is not enabled, this bit is always '1'. However in this case, any event written will be ignored. When Write, this bit is the least significant bit for a 32-bit event.0:event_new_n no new event should be programed1:event_pending_nno pending event, new event is authorised. */
        U32 event31_1                      :31; /*31:1 , Note:These bits combined with bit0 consists a 32-bit event number. If a new event is written before the previous event has been sent, it will be ignored.*/
    } b;
} T_DEBUG_HOST_event;

//mode
typedef union
{
    U32 v;
    struct
    {
        U32 force_on                       :1 ; /*0 :0 , Note:When '1', force the debug host on, use clock UART if clock host is not detected.0:force_dbg_hst_n1:force_dbg_hst*/
        U32 clk_host_on                    :1 ; /*1 :1 , Note:This bit indicates if clock host is detected to be on or not. 0:clk_hst_no 1:clk_hst_det*/
        U32 mode_reserved_0                :30; /*31:2 , No description.               */
    } b;
} T_DEBUG_HOST_mode;

//h2p_status
typedef union
{
    U32 v;
    struct
    {
        U32 h2p_status                     :8 ; /*7 :0 , Note:Status which can be written through debug uart interface into a debug host internal register and read by APB.*/
        U32 h2p_status_reserved_1          :8 ; /*15:8 , No description.               */
        U32 h2p_status_rst                 :1 ; /*16:16, write in this bit will reset h2p status register.*/
        U32 h2p_status_reserved_0          :15; /*31:17, No description.               */
    } b;
} T_DEBUG_HOST_h2p_status;

//p2h_status
typedef union
{
    U32 v;
    struct
    {
        U32 p2h_status                     :8 ; /*7 :0 , Note:Status which can be written by APB and read through debug uart interface as a debug host internal register.*/
        U32 p2h_status_reserved_0          :24; /*31:8 , No description.               */
    } b;
} T_DEBUG_HOST_p2h_status;

//irq
typedef union
{
    U32 v;
    struct
    {
        U32 xcpu_irq                       :1 ; /*0 :0 , Note:when Read:get the bcpu irq status.When Write 1:clear the bcpu irq level which is programmed in a debug host internal register, this bit is automatic cleared.*/
        U32 bcpu_irq                       :1 ; /*1 :1 , Note:when Read: get the xcpu irq status.When Write 1: clear the xcpu irq level which is programmed in a debug host internal register, this bit is automatic cleared*/
        U32 irq_reserved_0                 :30; /*31:2 , No description.               */
    } b;
} T_DEBUG_HOST_irq;

typedef struct
{
    volatile                T_DEBUG_HOST_cmd                            cmd; /*0x0  , RO   , 0x00000000, No description.               */
    volatile               T_DEBUG_HOST_data                           data; /*0x4  , RW   , 0x00000000, No description.               */
    volatile              T_DEBUG_HOST_event                          event; /*0x8  , RW   , 0x00000000, No description.               */
    volatile               T_DEBUG_HOST_mode                           mode; /*0xc  , RW   , 0x00000001, No description.               */
    volatile         T_DEBUG_HOST_h2p_status                     h2p_status; /*0x10 , RW   , 0x00000000, No description.               */
    volatile         T_DEBUG_HOST_p2h_status                     p2h_status; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                T_DEBUG_HOST_irq                            irq; /*0x18 , RW   , 0x00000000, No description.               */
} T_HWP_DEBUG_HOST_T;
#else
typedef struct
{
    volatile                             U32                            cmd; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32                           data; /*0x4  , RW   , 0x00000000, No description.               */
    volatile                             U32                          event; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                           mode; /*0xc  , RW   , 0x00000001, No description.               */
    volatile                             U32                     h2p_status; /*0x10 , RW   , 0x00000000, No description.               */
    volatile                             U32                     p2h_status; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                             U32                            irq; /*0x18 , RW   , 0x00000000, No description.               */
} T_HWP_DEBUG_HOST_T;

//cmd
#define DEBUG_HOST_ADDR(n)             (((n)&0x1FFFFFFF)<<0)
#define DEBUG_HOST_ADDR_MASK           (0x1FFFFFFF<<0)     
#define DEBUG_HOST_ADDR_SHIFT          (0)                 

#define DEBUG_HOST_SIZE(n)             (((n)&0x3)<<29)     
#define DEBUG_HOST_SIZE_BYTE           (((0)&0x3)<<29)     
#define DEBUG_HOST_SIZE_HALF_WORD      (((1)&0x3)<<29)     
#define DEBUG_HOST_SIZE_WORD           (((2)&0x3)<<29)     
#define DEBUG_HOST_SIZE_MASK           (0x3<<29)           
#define DEBUG_HOST_SIZE_SHIFT          (29)                

#define DEBUG_HOST_WRITE_H             (1<<31)             
#define DEBUG_HOST_WRITE_H_READ        (0<<31)             
#define DEBUG_HOST_WRITE_H_WRITE       (1<<31)             


//data
#define DEBUG_HOST_DATA(n)             (((n)&0xFFFFFFFF)<<0)
#define DEBUG_HOST_DATA_MASK           (0xFFFFFFFF<<0)     
#define DEBUG_HOST_DATA_SHIFT          (0)                 


//event
#define DEBUG_HOST_EVENT0_SEMA         (1<<0)              
#define DEBUG_HOST_EVENT0_SEMA_EVENT_NEW_N (0<<0)              
#define DEBUG_HOST_EVENT0_SEMA_EVENT_PENDING_N (1<<0)              

#define DEBUG_HOST_EVENT31_1(n)        (((n)&0x7FFFFFFF)<<1)
#define DEBUG_HOST_EVENT31_1_MASK      (0x7FFFFFFF<<1)     
#define DEBUG_HOST_EVENT31_1_SHIFT     (1)                 


//mode
#define DEBUG_HOST_FORCE_ON            (1<<0)              
#define DEBUG_HOST_FORCE_ON_FORCE_DBG_HST_N (0<<0)              
#define DEBUG_HOST_FORCE_ON_FORCE_DBG_HST (1<<0)              

#define DEBUG_HOST_CLK_HOST_ON         (1<<1)              
#define DEBUG_HOST_CLK_HOST_ON_CLK_HST_NO (0<<1)              
#define DEBUG_HOST_CLK_HOST_ON_CLK_HST_DET (1<<1)              


//h2p_status
#define DEBUG_HOST_H2P_STATUS(n)       (((n)&0xFF)<<0)     
#define DEBUG_HOST_H2P_STATUS_MASK     (0xFF<<0)           
#define DEBUG_HOST_H2P_STATUS_SHIFT    (0)                 

#define DEBUG_HOST_H2P_STATUS_RST      (1<<16)             


//p2h_status
#define DEBUG_HOST_P2H_STATUS(n)       (((n)&0xFF)<<0)     
#define DEBUG_HOST_P2H_STATUS_MASK     (0xFF<<0)           
#define DEBUG_HOST_P2H_STATUS_SHIFT    (0)                 


//irq
#define DEBUG_HOST_XCPU_IRQ            (1<<0)              

#define DEBUG_HOST_BCPU_IRQ            (1<<1)              


#endif

#endif


