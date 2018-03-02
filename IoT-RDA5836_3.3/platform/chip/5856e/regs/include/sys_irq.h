//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : sys_irq                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:15                                                  
// Last modified  : 2016-09-29 11:13:15                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:15 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SYS_IRQ_H__
#define __SYS_IRQ_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#include "globals.h"

#define hwp_sysIrq hwp_sys_irq

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//cause
typedef union
{
    U32 v;
    struct
    {
        U32 cause                          :32; /*31:0 , If cause is not null and interrupt are enabled then the interrupt line 0 is driven on the XCPU.The cause for the Irq sources, one bit for each module's irq source.The cause is the actual Irq source masked by the mask register.*/
    } b;
} T_SYS_IRQ_cause;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 status                         :32; /*31:0 , The status for the level Irq sources, one bit for each module's irq source.The status reflect the actual Irq source.*/
    } b;
} T_SYS_IRQ_status;

//mask_set
typedef union
{
    U32 v;
    struct
    {
        U32 mask_set                       :32; /*31:0 , Writing '1' sets the corresponding bit in the mask register to '1'.Reading gives the value of the mask register.*/
    } b;
} T_SYS_IRQ_mask_set;

//mask_clear
typedef union
{
    U32 v;
    struct
    {
        U32 mask_clr                       :32; /*31:0 , Writing '1' clears the corresponding bit in the mask register to '0'.Reading gives the value of the mask register.*/
    } b;
} T_SYS_IRQ_mask_clear;

//nonmaskable
typedef union
{
    U32 v;
    struct
    {
        U32 nonmaskable_reserved_2         :10; /*9 :0 , No description.               */
        U32 main_irq                       :1 ; /*10:10, This is the Main Irq source it drive the system CPU interrupt line 0.This bit comes from the modules irq and is masked by the Mask and SC registers.*/
        U32 wdt_irq                        :1 ; /*11:11, This is the icache Irq source it drive the system CPU interrupt line 1.This bit is controlled by the WDT module.*/
        U32 nonmaskable_reserved_1         :1 ; /*12:12, No description.               */
        U32 dbm_irq                        :1 ; /*13:13, This is the debug bus mater Irq source it drive the system CPU interrupt line 3.This bit is controlled by the dbm module.*/
        U32 debug_irq                      :1 ; /*14:14, This is the debug Irq source, the value written here drives the system CPU interrupt line 4.*/
        U32 host_irq                       :1 ; /*15:15, This is the Host Irq source it drive the system CPU interrupt line 5.This bit is controlled by the host internal register.*/
        U32 nonmaskable_reserved_0         :15; /*30:16, No description.               */
        U32 intenable_status               :1 ; /*31:31, Status of the Interrupt enable semaphore bit.*/
    } b;
} T_SYS_IRQ_nonmaskable;

//sc
typedef union
{
    U32 v;
    struct
    {
        U32 intenable                      :1 ; /*0 :0 , Interrupt enable semaphore, used for critical section.Read returns its value and then clears it to '0' disabling interrupts.Write the read value to restore the previous state, this will exit the critical section.*/
        U32 sc_reserved_0                  :31; /*31:1 , No description.               */
    } b;
} T_SYS_IRQ_sc;

//wakeup_mask
typedef union
{
    U32 v;
    struct
    {
        U32 wakeup_mask                    :32; /*31:0 , Each bit to '1' in that registers allows the correcponding interrupt to wake up the XCPU (i.e.: Reenable it's clock, see CLOCK_SYS_ENABLE and CLOCK_SYS_DISABLE registers in sys_ctrl registers section)*/
    } b;
} T_SYS_IRQ_wakeup_mask;

//cpu_sleep
typedef union
{
    U32 v;
    struct
    {
        U32 sleep                          :1 ; /*0 :0 , Writing '1' to this bit will put the XCPU to sleep (i.e.: Disable it's clock, see CLOCK_SYS_ENABLE and CLOCK_SYS_DISABLE registers in sys_ctrl registers section)*/
        U32 cpu_sleep_reserved_0           :31; /*31:1 , No description.               */
    } b;
} T_SYS_IRQ_cpu_sleep;

typedef struct
{
    volatile                 T_SYS_IRQ_cause                          cause; /*0x0  , RO   , 0x00000000, Interrupt Cause Register      */
    volatile                T_SYS_IRQ_status                         status; /*0x4  , RO   , 0x00000000, Interrupt Status Register     */
    volatile              T_SYS_IRQ_mask_set                       mask_set; /*0x8  , RW   , 0x00000000, Interrupt Mask Set Register   */
    volatile            T_SYS_IRQ_mask_clear                     mask_clear; /*0xc  , RW   , 0x00000000, Interrupt Mask Clear Register */
    volatile           T_SYS_IRQ_nonmaskable                    nonmaskable; /*0x10 , RW   , 0x80000000, Non-maskable Interrupt Register*/
    volatile                    T_SYS_IRQ_sc                             sc; /*0x14 , RW   , 0x00000001, Seamphore Control Register    */
    volatile           T_SYS_IRQ_wakeup_mask                    wakeup_mask; /*0x18 , RW   , 0x00000000, Wakeup Mask Register          */
    volatile             T_SYS_IRQ_cpu_sleep                      cpu_sleep; /*0x1c , RW   , 0x00000000, CPU Sleep Control Register    */
} T_HWP_SYS_IRQ_T;
#else
typedef struct
{
    volatile                             U32                          cause; /*0x0  , RO   , 0x00000000, Interrupt Cause Register      */
    volatile                             U32                         status; /*0x4  , RO   , 0x00000000, Interrupt Status Register     */
    volatile                             U32                       mask_set; /*0x8  , RW   , 0x00000000, Interrupt Mask Set Register   */
    volatile                             U32                     mask_clear; /*0xc  , RW   , 0x00000000, Interrupt Mask Clear Register */
    volatile                             U32                    nonmaskable; /*0x10 , RW   , 0x80000000, Non-maskable Interrupt Register*/
    volatile                             U32                             sc; /*0x14 , RW   , 0x00000001, Seamphore Control Register    */
    volatile                             U32                    wakeup_mask; /*0x18 , RW   , 0x00000000, Wakeup Mask Register          */
    volatile                             U32                      cpu_sleep; /*0x1c , RW   , 0x00000000, CPU Sleep Control Register    */
} T_HWP_SYS_IRQ_T;

typedef enum{
	BT_TIM1_IRQ = 0,
	BT_TIM0_IRQ = 1,
	BT_TIM3_IRQ = 2,
	BT_TIM2_IRQ = 3,
	BT_AUX_TIM_IRQ = 4,
	BT_PKA_IRQ = 5,
	BT_NO_PKT_RCVD_IRQ = 6,
	BT_PKD_IRQ = 7,
	BT_PKD_RX_HDR_IRQ = 8,
	BT_SYNC_DET_IRQ = 9,
}bt_irq_num;
typedef enum{
	BT_IRQ_START = 0,
	BT_IRQ_END = 1
}bt_irq_position;
typedef enum{
	BT_IRQ_NO_ERROR,
	BT_IRQ_RELOAD,
	BT_IRQ_IGNORE
}bt_irq_status;

//cause
#define SYS_IRQ_SYS_IRQ_BT_TIMER1                        (1<<0)
#define SYS_IRQ_SYS_IRQ_BT_TIMER0                        (1<<1)
#define SYS_IRQ_SYS_IRQ_BT_TIMER3                        (1<<2)
#define SYS_IRQ_SYS_IRQ_BT_TIMER2                        (1<<3)
#define SYS_IRQ_SYS_IRQ_BT_AUX_TIMER                  (1<<4)
#define SYS_IRQ_SYS_IRQ_BT_PKA                              (1<<5)
#define SYS_IRQ_SYS_IRQ_BT_RX_TIMEOUT                (1<<6)
#define SYS_IRQ_SYS_IRQ_BT_PKD                              (1<<7)
#define SYS_IRQ_SYS_IRQ_BT_RX_HEADER                  (1<<8)
#define SYS_IRQ_SYS_IRQ_BT_RX_SYNC                      (1<<9)
#define SYS_IRQ_SYS_IRQ_BT_PCM_SYNC                    (1<<10)
#define SSYS_IRQ_SYS_IRQ_BT_MULTI_LINK0             (1<<11)
#define SYS_IRQ_SYS_IRQ_BT_MULTI_LINK1               (1<<12)
#define SYS_IRQ_SYS_IRQ_USBC                                  (1<<13)
#define SYS_IRQ_SYS_IRQ_VOC                                    (1<<14)
#define SYS_IRQ_SYS_IRQ_USELESS                            (1<<15)
#define SYS_IRQ_SYS_IRQ_BBIFC0                              (1<<16)
#define SYS_IRQ_SYS_IRQ_BBIFC1                              (1<<17)
#define SYS_IRQ_SYS_IRQ_FM_DSP                             (1<<18)
#define SYS_IRQ_SYS_IRQ_GPIO                                  (1<<19)
#define SYS_IRQ_SYS_IRQ_UART                                 (1<<20)
#define SYS_IRQ_SYS_IRQ_IR                                      (1<<21)
#define SYS_IRQ_SYS_IRQ_USB_DET                           (1<<22)
#define SYS_IRQ_SYS_IRQ_I2C                                    (1<<23)
#define SYS_IRQ_SYS_IRQ_CALENDAR                         (1<<24)
#define SYS_IRQ_SYS_IRQ_PWM                                  (1<<25)
#define SYS_IRQ_SYS_IRQ_SDMMC                              (1<<26)
#define SYS_IRQ_SYS_IRQ_SPI1                                  (1<<27)
#define SYS_IRQ_SYS_IRQ_DEBUG_UART                    (1<<28)
#define SYS_IRQ_SYS_IRQ_DMA                                  (1<<29)
#define SYS_IRQ_SYS_IRQ_TIMERS                             (1<<30)
#define SYS_IRQ_SYS_IRQ_PMU                                  (1<<31)

#define SYS_IRQ_CAUSE(n)               (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_CAUSE_MASK             (0xFFFFFFFF<<0)     
#define SYS_IRQ_CAUSE_SHIFT            (0)                 


//status
#define SYS_IRQ_STATUS(n)              (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_STATUS_MASK            (0xFFFFFFFF<<0)     
#define SYS_IRQ_STATUS_SHIFT           (0)                 


//mask_set
#define SYS_IRQ_MASK_SET(n)            (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_MASK_SET_MASK          (0xFFFFFFFF<<0)     
#define SYS_IRQ_MASK_SET_SHIFT         (0)                 


//mask_clear
#define SYS_IRQ_MASK_CLR(n)            (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_MASK_CLR_MASK          (0xFFFFFFFF<<0)     
#define SYS_IRQ_MASK_CLR_SHIFT         (0)                 


//nonmaskable
#define SYS_IRQ_MAIN_IRQ               (1<<10)             

#define SYS_IRQ_WDT_IRQ                (1<<11)             

#define SYS_IRQ_DBM_IRQ                (1<<13)             

#define SYS_IRQ_DEBUG_IRQ              (1<<14)             

#define SYS_IRQ_HOST_IRQ               (1<<15)             

#define SYS_IRQ_INTENABLE_STATUS       (1<<31)             


//sc
#define SYS_IRQ_INTENABLE              (1<<0)              


//wakeup_mask
#define SYS_IRQ_WAKEUP_MASK(n)         (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_WAKEUP_MASK_MASK       (0xFFFFFFFF<<0)     
#define SYS_IRQ_WAKEUP_MASK_SHIFT      (0)                 


//cpu_sleep
#define SYS_IRQ_SLEEP                  (1<<0)              


#endif

#endif


