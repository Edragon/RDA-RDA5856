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
// File name      : sys_irq_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:15                                                  
// Last modified  : 2016-09-29 11:13:15                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:15 --  Create by liqiu                                               
//------------------------------------------------------------------------


#ifndef __SYS_IRQ_ASM_H__
#define __SYS_IRQ_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"



/// XCPU Irq Lines
#define XCPU_MAIN_IRQ_LINE                       (0)
#define XCPU_WDT_IRQ_LINE                       (1)
#define XCPU_PAGE_SPY_IRQ_LINE                   (3)
#define XCPU_DBM_IRQ_LINE                   	(3)
#define XCPU_DEBUG_IRQ_LINE                      (4)
#define XCPU_HOST_IRQ_LINE                       (5)


#define REG_cause                      REG_SYS_IRQ_BASE+0x0
#define REG_status                     REG_SYS_IRQ_BASE+0x4
#define REG_mask_set                   REG_SYS_IRQ_BASE+0x8
#define REG_mask_clear                 REG_SYS_IRQ_BASE+0xc
#define REG_nonmaskable                REG_SYS_IRQ_BASE+0x10
#define REG_sc                         REG_SYS_IRQ_BASE+0x14
#define REG_wakeup_mask                REG_SYS_IRQ_BASE+0x18
#define REG_cpu_sleep                  REG_SYS_IRQ_BASE+0x1c

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
#define SYS_IRQ_CAUSE_MASK          (0xFFFFFFFF<<0)
#define SYS_IRQ_CAUSE_SHIFT         (0)

//status
#define SYS_IRQ_STATUS(n)              (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_STATUS_MASK         (0xFFFFFFFF<<0)
#define SYS_IRQ_STATUS_SHIFT        (0)

//mask_set
#define SYS_IRQ_MASK_SET(n)            (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_MASK_SET_MASK       (0xFFFFFFFF<<0)
#define SYS_IRQ_MASK_SET_SHIFT      (0)

//mask_clear
#define SYS_IRQ_MASK_CLR(n)            (((n)&0xFFFFFFFF)<<0)
#define SYS_IRQ_MASK_CLR_MASK       (0xFFFFFFFF<<0)
#define SYS_IRQ_MASK_CLR_SHIFT      (0)

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

//cpu_sleep
#define SYS_IRQ_SLEEP                  (1<<0)              


#endif


