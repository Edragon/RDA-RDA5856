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
// File name      : wdt_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:26                                                  
// Last modified  : 2016-09-29 11:13:26                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:26 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __WDT_ASM_H__
#define __WDT_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_wdogloada                  REG_WDT_BASE+0x00   
#define REG_wdogvaluea                 REG_WDT_BASE+0x04   
#define REG_wdogcontrola               REG_WDT_BASE+0x08   
#define REG_wdogcleara                 REG_WDT_BASE+0x0C   
#define REG_wdogintrawa                REG_WDT_BASE+0x10   
#define REG_wdoginta                   REG_WDT_BASE+0x14   

#define REG_wdoglocka                  REG_WDT_BASE+0xC00  

#define REG_wdogitcr                   REG_WDT_BASE+0xF00  
#define REG_wdogitop                   REG_WDT_BASE+0xF04  

//wdogloada
#define WDT_LOAD_VALUE(n)              (((n)&0xFFFFFFFF)<<0)

//wdogvaluea
#define WDT_VALUE(n)                   (((n)&0xFFFFFFFF)<<0)

//wdogcontrola
#define WDT_INT_EN                     (1<<0)              
#define WDT_RES_EN                     (1<<1)              

//wdogcleara
#define WDT_ICR                        (1<<0)              

//wdogintrawa
#define WDT_IRSR                       (1<<0)              

//wdoginta
#define WDT_ISR                        (1<<0)              

//wdoglocka
#define WDT_LOCK                       (1<<0)              

//wdogitcr
#define WDT_ITM_EN                     (1<<0)              

//wdogitop
#define WDT_TEST_RES                   (1<<0)              
#define WDT_TEST_INT                   (1<<1)              


#endif


