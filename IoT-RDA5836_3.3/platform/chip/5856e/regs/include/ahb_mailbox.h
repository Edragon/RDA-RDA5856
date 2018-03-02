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
// File name      : ahb_mailbox                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:30                                                  
// Last modified  : 2016-12-21 13:55:30                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:30 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB_MAILBOX_H__
#define __AHB_MAILBOX_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//zsp2mcu_irq_trig
typedef union
{
    U32 v;
    struct
    {
        U32 zsp2mcu_irq_trig               :32; /*31:0 , Write 1 will trig interrupt to MCU, total 32 interrupt sources.*/
    } b;
} T_AHB_MAILBOX_zsp2mcu_irq_trig;

//zsp2mcu_isr
typedef union
{
    U32 v;
    struct
    {
        U32 zsp2mcu_irq_status             :32; /*31:0 , ZSP to MCU interrupt status.  */
    } b;
} T_AHB_MAILBOX_zsp2mcu_isr;

//zsp2mcu_imr
typedef union
{
    U32 v;
    struct
    {
        U32 zsp2mcu_irq_mask               :32; /*31:0 , ZSP toMCU interrupt mask. Interrupt enabled when set to 0.*/
    } b;
} T_AHB_MAILBOX_zsp2mcu_imr;

//zsp2mcu_irsr
typedef union
{
    U32 v;
    struct
    {
        U32 zsp2mcu_irq_status_raw         :32; /*31:0 , ZSP toMCU interrupt  raw status, regardless of mask.*/
    } b;
} T_AHB_MAILBOX_zsp2mcu_irsr;

//zsp2mcu_icr
typedef union
{
    U32 v;
    struct
    {
        U32 zsp2mcu_irq_clear              :32; /*31:0 , ZSP to MCU interrupt  clear.  */
    } b;
} T_AHB_MAILBOX_zsp2mcu_icr;

//mcu2zsp_irq_trig
typedef union
{
    U32 v;
    struct
    {
        U32 mcu2zsp_irq_trig               :32; /*31:0 , Write 1 will trig interrupt to ZSP, total 32 interrupt sources.*/
    } b;
} T_AHB_MAILBOX_mcu2zsp_irq_trig;

//mcu2zsp_isr
typedef union
{
    U32 v;
    struct
    {
        U32 mcu2zsp_irq_status             :32; /*31:0 , MCU to ZSP  interrupt status. */
    } b;
} T_AHB_MAILBOX_mcu2zsp_isr;

//mcu2zsp_imr
typedef union
{
    U32 v;
    struct
    {
        U32 mcu2zsp_irq_mask               :32; /*31:0 , MCU to ZSP  interrupt mask. Interrupt enabled when set to 0.*/
    } b;
} T_AHB_MAILBOX_mcu2zsp_imr;

//mcu2zsp_irsr
typedef union
{
    U32 v;
    struct
    {
        U32 mcu2zsp_irq_status_raw         :32; /*31:0 , MCU to ZSP  interrupt  raw status, regardless of mask.*/
    } b;
} T_AHB_MAILBOX_mcu2zsp_irsr;

//mcu2zsp_icr
typedef union
{
    U32 v;
    struct
    {
        U32 mcu2zsp_irq_clear              :32; /*31:0 , MCU to ZSP  interrupt  clear. */
    } b;
} T_AHB_MAILBOX_mcu2zsp_icr;

typedef struct
{
    volatile  T_AHB_MAILBOX_zsp2mcu_irq_trig               zsp2mcu_irq_trig; /*0x0  , WO   , 0x00000000, ZSP to MCU Interrupt Trigger Register*/
    volatile       T_AHB_MAILBOX_zsp2mcu_isr                    zsp2mcu_isr; /*0x4  , RO   , 0x00000000, ZSP to MCU Interrupt Status Register*/
    volatile       T_AHB_MAILBOX_zsp2mcu_imr                    zsp2mcu_imr; /*0x8  , RW   , 0xFFFFFFFF, ZSP to MCU Interrupt Mask Register*/
    volatile      T_AHB_MAILBOX_zsp2mcu_irsr                   zsp2mcu_irsr; /*0xc  , RO   , 0x00000000, ZSP to MCU Interrupt Raw Status Register*/
    volatile       T_AHB_MAILBOX_zsp2mcu_icr                    zsp2mcu_icr; /*0x10 , RW   , 0x00000000, ZSP to MCU Interrupt Clear Register*/
    volatile  T_AHB_MAILBOX_mcu2zsp_irq_trig               mcu2zsp_irq_trig; /*0x14 , WO   , 0x00000000, ZSP to MCU Interrupt Trigger Register*/
    volatile       T_AHB_MAILBOX_mcu2zsp_isr                    mcu2zsp_isr; /*0x18 , RO   , 0x00000000, ZSP to MCU Interrupt Status Register*/
    volatile       T_AHB_MAILBOX_mcu2zsp_imr                    mcu2zsp_imr; /*0x1c , RW   , 0xFFFFFFFF, ZSP to MCU Interrupt Mask Register*/
    volatile      T_AHB_MAILBOX_mcu2zsp_irsr                   mcu2zsp_irsr; /*0x20 , RO   , 0x00000000, ZSP to MCU Interrupt Raw Status Register*/
    volatile       T_AHB_MAILBOX_mcu2zsp_icr                    mcu2zsp_icr; /*0x24 , RW   , 0x00000000, ZSP to MCU Interrupt Clear Register*/
} T_HWP_AHB_MAILBOX_T;
#else
typedef struct
{
    volatile                             U32               zsp2mcu_irq_trig; /*0x0  , WO   , 0x00000000, ZSP to MCU Interrupt Trigger Register*/
    volatile                             U32                    zsp2mcu_isr; /*0x4  , RO   , 0x00000000, ZSP to MCU Interrupt Status Register*/
    volatile                             U32                    zsp2mcu_imr; /*0x8  , RW   , 0xFFFFFFFF, ZSP to MCU Interrupt Mask Register*/
    volatile                             U32                   zsp2mcu_irsr; /*0xc  , RO   , 0x00000000, ZSP to MCU Interrupt Raw Status Register*/
    volatile                             U32                    zsp2mcu_icr; /*0x10 , RW   , 0x00000000, ZSP to MCU Interrupt Clear Register*/
    volatile                             U32               mcu2zsp_irq_trig; /*0x14 , WO   , 0x00000000, ZSP to MCU Interrupt Trigger Register*/
    volatile                             U32                    mcu2zsp_isr; /*0x18 , RO   , 0x00000000, ZSP to MCU Interrupt Status Register*/
    volatile                             U32                    mcu2zsp_imr; /*0x1c , RW   , 0xFFFFFFFF, ZSP to MCU Interrupt Mask Register*/
    volatile                             U32                   mcu2zsp_irsr; /*0x20 , RO   , 0x00000000, ZSP to MCU Interrupt Raw Status Register*/
    volatile                             U32                    mcu2zsp_icr; /*0x24 , RW   , 0x00000000, ZSP to MCU Interrupt Clear Register*/
} T_HWP_AHB_MAILBOX_T;

#define AHB_MAILBOX_ZSP2MCU_IRQ_TRIG(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_ZSP2MCU_IRQ_TRIG_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_ZSP2MCU_IRQ_TRIG_SHIFT (0)                 


#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_SHIFT (0)                 


#define AHB_MAILBOX_ZSP2MCU_IRQ_MASK(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_ZSP2MCU_IRQ_MASK_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_ZSP2MCU_IRQ_MASK_SHIFT (0)                 


#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_RAW(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_RAW_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_ZSP2MCU_IRQ_STATUS_RAW_SHIFT (0)                 


#define AHB_MAILBOX_ZSP2MCU_IRQ_CLEAR(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_ZSP2MCU_IRQ_CLEAR_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_ZSP2MCU_IRQ_CLEAR_SHIFT (0)                 


#define AHB_MAILBOX_MCU2ZSP_IRQ_TRIG(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_MCU2ZSP_IRQ_TRIG_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_MCU2ZSP_IRQ_TRIG_SHIFT (0)                 


#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_SHIFT (0)                 


#define AHB_MAILBOX_MCU2ZSP_IRQ_MASK(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_MCU2ZSP_IRQ_MASK_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_MCU2ZSP_IRQ_MASK_SHIFT (0)                 


#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_RAW(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_RAW_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_MCU2ZSP_IRQ_STATUS_RAW_SHIFT (0)                 


#define AHB_MAILBOX_MCU2ZSP_IRQ_CLEAR(n) (((n)&0xFFFFFFFF)<<0)
#define AHB_MAILBOX_MCU2ZSP_IRQ_CLEAR_MASK (0xFFFFFFFF<<0)     
#define AHB_MAILBOX_MCU2ZSP_IRQ_CLEAR_SHIFT (0)                 


#endif

#endif


