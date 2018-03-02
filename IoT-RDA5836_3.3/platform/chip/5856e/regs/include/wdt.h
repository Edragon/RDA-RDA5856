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
// File name      : wdt                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:26                                                  
// Last modified  : 2016-09-29 11:13:26                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:26 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __WDT_H__
#define __WDT_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//wdogloada
typedef union
{
    U32 v;
    struct
    {
        U32 load_value                     :32; /*31:0 , watchdog counter load value   */
    } b;
} T_WDT_wdogloada;

//wdogvaluea
typedef union
{
    U32 v;
    struct
    {
        U32 value                          :32; /*31:0 , watchdog current counter value*/
    } b;
} T_WDT_wdogvaluea;

//wdogcontrola
typedef union
{
    U32 v;
    struct
    {
        U32 int_en                         :1 ; /*0 :0 , watch dog interrupt enable    */
        U32 res_en                         :1 ; /*1 :1 , watch dog reset enable        */
        U32 wdogcontrola_reserved_0        :30; /*31:2 , No description.               */
    } b;
} T_WDT_wdogcontrola;

//wdogcleara
typedef union
{
    U32 v;
    struct
    {
        U32 icr                            :1 ; /*0 :0 , 1: clear watchdog interrupt   */
        U32 wdogcleara_reserved_0          :31; /*31:1 , No description.               */
    } b;
} T_WDT_wdogcleara;

//wdogintrawa
typedef union
{
    U32 v;
    struct
    {
        U32 irsr                           :1 ; /*0 :0 , raw interrupt status1: interrupt is generated0: interrupt is not generated*/
        U32 wdogintrawa_reserved_0         :31; /*31:1 , No description.               */
    } b;
} T_WDT_wdogintrawa;

//wdoginta
typedef union
{
    U32 v;
    struct
    {
        U32 isr                            :1 ; /*0 :0 , interrupt status(gate raw interrupt with watch dog interrupt enable)1: interrupt is generated0: interrupt is not generated*/
        U32 wdoginta_reserved_0            :31; /*31:1 , No description.               */
    } b;
} T_WDT_wdoginta;

//wdoglocka
typedef union
{
    U32 v;
    struct
    {
        U32 lock                           :1 ; /*0 :0 , watch dog lock status, if locked, the watch dog register cannot be accessed1: watch dog is lock0: watch dog is unlock*/
        U32 wdoglocka_reserved_0           :31; /*31:1 , No description.               */
    } b;
} T_WDT_wdoglocka;

//wdogitcr
typedef union
{
    U32 v;
    struct
    {
        U32 itm_en                         :1 ; /*0 :0 , 1: integration test mode enable */
        U32 wdogitcr_reserved_0            :31; /*31:1 , No description.               */
    } b;
} T_WDT_wdogitcr;

//wdogitop
typedef union
{
    U32 v;
    struct
    {
        U32 test_res                       :1 ; /*0 :0 , watch dog reset value in integration test mode*/
        U32 test_int                       :1 ; /*1 :1 , watch dog interrupt value in integration test mode*/
        U32 wdogitop_reserved_0            :30; /*31:2 , No description.               */
    } b;
} T_WDT_wdogitop;

typedef struct
{
    volatile                 T_WDT_wdogloada                      wdogloada; /*0x0  , RW   , 0xFFFFFFFF, Watchdog Load Register        */
    volatile                T_WDT_wdogvaluea                     wdogvaluea; /*0x4  , RO   , 0xFFFFFFFF, Watchdog Valuee Register      */
    volatile              T_WDT_wdogcontrola                   wdogcontrola; /*0x8  , RW   , 0x00000000, Watchdog Control Register     */
    volatile                T_WDT_wdogcleara                     wdogcleara; /*0xc  , RW   , 0x00000000, Watchdog Interrupt Clear Register*/
    volatile               T_WDT_wdogintrawa                    wdogintrawa; /*0x10 , RO   , 0x00000000, Watchdog Raw Interrupt Status Register*/
    volatile                  T_WDT_wdoginta                       wdoginta; /*0x14 , RO   , 0x00000000, Watchdog Interrupt Status Register*/
    volatile                             U32                reserved_1[762];
    volatile                 T_WDT_wdoglocka                      wdoglocka; /*0xc00, RW   , 0x00000000, Watchdog Lock Register        */
    volatile                             U32                reserved_2[191];
    volatile                  T_WDT_wdogitcr                       wdogitcr; /*0xf00, RW   , 0x00000000, Watchdog Integration Test Control Register*/
    volatile                  T_WDT_wdogitop                       wdogitop; /*0xf04, RW   , 0x00000000, Watchdog Integration Test Output Set Register*/
} T_HWP_WDT_T;
#else
typedef struct
{
    volatile                             U32                      wdogloada; /*0x0  , RW   , 0xFFFFFFFF, Watchdog Load Register        */
    volatile                             U32                     wdogvaluea; /*0x4  , RO   , 0xFFFFFFFF, Watchdog Valuee Register      */
    volatile                             U32                   wdogcontrola; /*0x8  , RW   , 0x00000000, Watchdog Control Register     */
    volatile                             U32                     wdogcleara; /*0xc  , RW   , 0x00000000, Watchdog Interrupt Clear Register*/
    volatile                             U32                    wdogintrawa; /*0x10 , RO   , 0x00000000, Watchdog Raw Interrupt Status Register*/
    volatile                             U32                       wdoginta; /*0x14 , RO   , 0x00000000, Watchdog Interrupt Status Register*/
    volatile                             U32                reserved_3[762];
    volatile                             U32                      wdoglocka; /*0xc00, RW   , 0x00000000, Watchdog Lock Register        */
    volatile                             U32                reserved_4[191];
    volatile                             U32                       wdogitcr; /*0xf00, RW   , 0x00000000, Watchdog Integration Test Control Register*/
    volatile                             U32                       wdogitop; /*0xf04, RW   , 0x00000000, Watchdog Integration Test Output Set Register*/
} T_HWP_WDT_T;

//wdogloada
#define WDT_LOAD_VALUE(n)              (((n)&0xFFFFFFFF)<<0)
#define WDT_LOAD_VALUE_MASK            (0xFFFFFFFF<<0)     
#define WDT_LOAD_VALUE_SHIFT           (0)                 


//wdogvaluea
#define WDT_VALUE(n)                   (((n)&0xFFFFFFFF)<<0)
#define WDT_VALUE_MASK                 (0xFFFFFFFF<<0)     
#define WDT_VALUE_SHIFT                (0)                 


//wdogcontrola
#define WDT_INT_EN                     (1<<0)              

#define WDT_RES_EN                     (1<<1)              


//wdogcleara
#define WDT_ICR                        (1<<0)              

//wdogintrawa
#define WDT_IRSR                       (1<<0)              
#define WDT_IRSR_INT_GEN               (1<<0)              
#define WDT_IRSR_INT_NO                (0<<0)              

//wdoginta
#define WDT_ISR                        (1<<0)              
#define WDT_ISR_INT_GEN                (1<<0)              
#define WDT_ISR_INT_NO                 (0<<0)              

//wdoglocka
#define WDT_LOCK                       (1<<0)              
#define WDT_LOCK_LOCK                  (1<<0)              
#define WDT_LOCK_UNLOCK                (0<<0)              


//wdogitcr
#define WDT_ITM_EN                     (1<<0)              
#define WDT_ITM_EN_ENABLE              (1<<0)              
#define WDT_ITM_EN_DISABLE             (0<<0)              

//wdogitop
#define WDT_TEST_RES                   (1<<0)              

#define WDT_TEST_INT                   (1<<1)              


#endif

#endif


