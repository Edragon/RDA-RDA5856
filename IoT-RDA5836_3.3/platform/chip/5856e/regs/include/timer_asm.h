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
// File name      : timer_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:59                                                  
// Last modified  : 2016-08-29 14:59:59                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:59 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __TIMER_ASM_H__
#define __TIMER_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ostimer_ctrl               REG_TIMER_BASE+0x00 
#define REG_ostimer_curval             REG_TIMER_BASE+0x04 
#define REG_wdtimer_ctrl               REG_TIMER_BASE+0x8  
#define REG_wdtimer_loadval            REG_TIMER_BASE+0x0c 
#define REG_hwtimer_ctrl               REG_TIMER_BASE+0x10 
#define REG_hwtimer_curval             REG_TIMER_BASE+0x14 
#define REG_timer_irq_mask_set         REG_TIMER_BASE+0x18 
#define REG_timer_irq_mask_clr         REG_TIMER_BASE+0x1c 
#define REG_timer_irq_clr              REG_TIMER_BASE+0x20 
#define REG_timer_irq_cause            REG_TIMER_BASE+0x24 

//ostimer_ctrl
#define TIMER_LOADVAL(n)               (((n)&0xFFFFFF)<<0) 
#define TIMER_ENABLE                   (1<<24)             
#define TIMER_ENABLED                  (1<<25)             
#define TIMER_CLEARED                  (1<<26)             
#define TIMER_REPEAT                   (1<<28)             
#define TIMER_WRAP                     (1<<29)             
#define TIMER_LOAD                     (1<<30)             

//ostimer_curval
#define TIMER_CURVAL(n)                (((n)&0xFFFFFFFF)<<0)

//wdtimer_ctrl
#define TIMER_START                    (1<<0)              
#define TIMER_WDENABLED                (1<<3)              
#define TIMER_STOP                     (1<<4)              
#define TIMER_RELOAD                   (1<<16)             

//wdtimer_loadval
#define TIMER_LOADVAL(n)               (((n)&0xFFFFFF)<<0) 

//hwtimer_ctrl
#define TIMER_INTERVAL(n)              (((n)&0x3)<<0)      
#define TIMER_INTERVAL_EN              (1<<8)              

//hwtimer_curval
#define TIMER_CURVAL(n)                (((n)&0xFFFFFFFF)<<0)

//timer_irq_mask_set
#define TIMER_OSTIMER_MASK             (1<<0)              
#define TIMER_HWTIMER_WRAP_MASK        (1<<1)              
#define TIMER_HWTIMER_ITV_MASK         (1<<2)              

//timer_irq_mask_clr
#define TIMER_OSTIMER_MASK             (1<<0)              
#define TIMER_HWTIMER_WRAP_MASK        (1<<1)              
#define TIMER_HWTIMER_ITV_MASK         (1<<2)              

//timer_irq_clr
#define TIMER_OSTIMER_CLR              (1<<0)              
#define TIMER_HWTIMER_WRAP_CLR         (1<<1)              
#define TIMER_HWTIMER_ITV_CLR          (1<<2)              

//timer_irq_cause
#define TIMER_OSTIMER_CAUSE            (1<<0)              
#define TIMER_HWTIMER_WRAP_CAUSE       (1<<1)              
#define TIMER_HWTIMER_ITV_CAUSE        (1<<2)              
#define TIMER_OSTIMER_STATUS           (1<<16)             
#define TIMER_HWTIMER_WRAP_STATUS      (1<<17)             
#define TIMER_HWTIMER_ITV_STATUS       (1<<18)             


#endif


