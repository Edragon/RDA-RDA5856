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
// File name      : pmu_intf_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:42                                                  
// Last modified  : 2016-12-14 15:57:42                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:42 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __PMU_INTF_ASM_H__
#define __PMU_INTF_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_efuse_ctrl1                REG_PMU_INTF_BASE+0x00
#define REG_efuse_ctrl2                REG_PMU_INTF_BASE+0x04
#define REG_efuse_readout              REG_PMU_INTF_BASE+0x08
#define REG_misc_ctrl                  REG_PMU_INTF_BASE+0x0c
#define REG_buck_clk_cfg               REG_PMU_INTF_BASE+0x10
#define REG_gpadc_ctrl1                REG_PMU_INTF_BASE+0x14
#define REG_gpadc_ctrl2                REG_PMU_INTF_BASE+0x18
#define REG_keysense_ctrl1             REG_PMU_INTF_BASE+0x1c
#define REG_xtal_ctrl                  REG_PMU_INTF_BASE+0x20
#define REG_gpadc_key_value            REG_PMU_INTF_BASE+0x24
#define REG_boost_ctrl                 REG_PMU_INTF_BASE+0x28
#define REG_pmu_intf_icr               REG_PMU_INTF_BASE+0x2c
#define REG_pmu_intf_imr               REG_PMU_INTF_BASE+0x30
#define REG_pmu_intf_irsr              REG_PMU_INTF_BASE+0x34
#define REG_pmu_intf_isr               REG_PMU_INTF_BASE+0x38
#define REG_pmu_ana_rsvd               REG_PMU_INTF_BASE+0x3c

//efuse_ctrl1
#define PMU_INTF_EFUSE_OUT(n)          (((n)&0xFF)<<0)     
#define PMU_INTF_EFUSE_SENSE_EN        (1<<8)              
#define PMU_INTF_EFUSE_SENSE_EN_REG    (1<<9)              
#define PMU_INTF_EFUSE_SENSE_EN_DR     (1<<10)             
#define PMU_INTF_EFUSE_SENSE           (1<<11)             
#define PMU_INTF_EFUSE_SENSE_REG       (1<<12)             
#define PMU_INTF_EFUSE_SENSE_DR        (1<<13)             
#define PMU_INTF_EFUSE_SEL_WORD_REG(n) (((n)&0xF)<<14)     
#define PMU_INTF_EFUSE_SEL_WORD_DR     (1<<18)             
#define PMU_INTF_EFUSE_SEL(n)          (((n)&0xFF)<<19)    
#define PMU_INTF_EFUSE_REF_RES_BIT(n)  (((n)&0x3)<<27)     
#define PMU_INTF_RESETN_EFUSE_REG      (1<<29)             
#define PMU_INTF_EFUSE_RD_DONE         (1<<30)             

//efuse_ctrl2
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_0(n) (((n)&0xF)<<0)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_1(n) (((n)&0xF)<<4)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_2(n) (((n)&0xF)<<8)      
#define PMU_INTF_EFUSE_SEL_WORD_AUTO_3(n) (((n)&0xF)<<12)     
#define PMU_INTF_REG_PROG_EFUSE_EN(n)  (((n)&0xFFFF)<<16)  

//efuse_readout
#define PMU_INTF_EFUSE_READOUT_0(n)    (((n)&0xFF)<<0)     
#define PMU_INTF_EFUSE_READOUT_1(n)    (((n)&0xFF)<<8)     
#define PMU_INTF_EFUSE_READOUT_2(n)    (((n)&0xFF)<<16)    
#define PMU_INTF_EFUSE_READOUT_3(n)    (((n)&0xFF)<<24)    

//misc_ctrl
#define PMU_INTF_PU_XTAL_32K           (1<<0)              
#define PMU_INTF_VBAT_MONITOR_EN       (1<<1)              

//buck_clk_cfg
#define PMU_INTF_SYNC_CLOCK_POLARITY   (1<<0)              
#define PMU_INTF_BUCK_CLK_CFG_LD       (1<<1)              
#define PMU_INTF_PU_BUCK_CLK_REG       (1<<2)              
#define PMU_INTF_PU_BUCK_CLK_DR        (1<<3)              
#define PMU_INTF_REG_SYNC_CLK_NUM(n)   (((n)&0x3)<<4)      
#define PMU_INTF_REG_SYNC_CLK_DENOM(n) (((n)&0x1F)<<6)     
#define PMU_INTF_PFM_MODE_CLK_PRD_BUCK(n) (((n)&0x3)<<11)     

//gpadc_ctrl1
#define PMU_INTF_RESETN_GPADC_REG      (1<<0)              
#define PMU_INTF_RESETN_GPADC_DR       (1<<1)              
#define PMU_INTF_GPADC_START_REG       (1<<2)              
#define PMU_INTF_GPADC_START_DR        (1<<3)              
#define PMU_INTF_GPADC_START           (1<<4)              
#define PMU_INTF_RESETN_GPADC          (1<<5)              
#define PMU_INTF_TIME_SAMP_NEG_GPADC   (1<<6)              
#define PMU_INTF_TIME_SAMP_POS_GPADC   (1<<7)              
#define PMU_INTF_TIMER_SAR_STABLE_SEL(n) (((n)&0x7)<<8)      
#define PMU_INTF_DELAY_BEFORE_SAMP_GPADC(n) (((n)&0x3)<<11)     
#define PMU_INTF_SAR_OUT_POLARITY_GPADC (1<<13)             
#define PMU_INTF_SAR_ADC_MODE_GPADC    (1<<14)             
#define PMU_INTF_CONV_CLK_INV_GPADC    (1<<15)             
#define PMU_INTF_SAMP_CLK_INV_GPADC    (1<<16)             
#define PMU_INTF_GPADC_EOC_PHASE_SEL   (1<<17)             
#define PMU_INTF_GPADC_CHANNEL_EN_REG(n) (((n)&0xFF)<<18)    
#define PMU_INTF_GPADC_CHANNEL_EN_DR   (1<<26)             
#define PMU_INTF_GPADC_ENABLE          (1<<27)             
#define PMU_INTF_GPADC_CLK_DIV_EN      (1<<28)             
#define PMU_INTF_GPADC_START_TIME_SEL(n) (((n)&0x7)<<29)     

//gpadc_ctrl2
#define PMU_INTF_GPADC_VREF_MS_ENABLE  (1<<0)              
#define PMU_INTF_GPADC_VREF_BIT(n)     (((n)&0x3)<<1)      
#define PMU_INTF_GPADC_CHANNEL_EN(n)   (((n)&0xFF)<<3)     
#define PMU_INTF_PU_GPADC              (1<<11)             
#define PMU_INTF_PU_GPADC_REG          (1<<12)             
#define PMU_INTF_PU_GPADC_DR           (1<<13)             
#define PMU_INTF_GPADC_CHANNEL_SEL(n)  (((n)&0xFF)<<14)    

//keysense_ctrl1
#define PMU_INTF_KEY_PULLUP2X          (1<<0)              
#define PMU_INTF_KEY_EN                (1<<1)              
#define PMU_INTF_KEY_EN_REG            (1<<2)              
#define PMU_INTF_KEY_EN_DR             (1<<3)              
#define PMU_INTF_KEY_INT_EN            (1<<4)              
#define PMU_INTF_KEY_INT_EN_REG        (1<<5)              
#define PMU_INTF_KEY_INT_EN_DR         (1<<6)              
#define PMU_INTF_KEY_ENABLE            (1<<7)              
#define PMU_INTF_TIME_SAMP_NEG_TSC     (1<<8)              
#define PMU_INTF_TIME_SAMP_POS_TSC     (1<<9)              
#define PMU_INTF_TIMER_X_TO_Y_SEL(n)   (((n)&0x3)<<10)     
#define PMU_INTF_TIMER_INT_TO_X_SEL(n) (((n)&0x7)<<12)     
#define PMU_INTF_TIMER_PENIRQ_STABLE(n) (((n)&0x7)<<15)     
#define PMU_INTF_DELAY_BEFORE_SAMP_TSC(n) (((n)&0x3)<<18)     
#define PMU_INTF_BYPASS_BB_READ        (1<<20)             
#define PMU_INTF_CONV_CLK_INV          (1<<21)             
#define PMU_INTF_SAMP_CLK_INV          (1<<22)             
#define PMU_INTF_SAR_ADC_MODE          (1<<23)             
#define PMU_INTF_SAR_OUT_POLARITY      (1<<24)             
#define PMU_INTF_WAIT_BB_READ_TIMESEL(n) (((n)&0x3)<<25)     
#define PMU_INTF_KEY_CLK_DIV_EN        (1<<27)             

//xtal_ctrl
#define PMU_INTF_XTAL_CLK_BB_EN_LV     (1<<0)              
#define PMU_INTF_XTAL_CLK_RSVD_EN_LV   (1<<1)              
#define PMU_INTF_XTAL_CLK_AUPLL_EN_LV  (1<<2)              
#define PMU_INTF_XTAL_CLK_ADPLL_EN_LV  (1<<3)              
#define PMU_INTF_XTAL_CLK_ADC_EN_LV    (1<<4)              

//gpadc_key_value
#define PMU_INTF_KEY_VALUE_VALID       (1<<0)              
#define PMU_INTF_KEY_VALUE(n)          (((n)&0x3FF)<<1)    
#define PMU_INTF_GPADC_CHANNEL_NUM(n)  (((n)&0x7)<<11)     
#define PMU_INTF_GPADC_VALUE_VALID     (1<<14)             
#define PMU_INTF_GPADC_VALUE(n)        (((n)&0x3FF)<<15)   

//boost_ctrl
#define PMU_INTF_PU_BOOST              (1<<0)              
#define PMU_INTF_DUTYSEL_BOOST         (1<<1)              
#define PMU_INTF_DUTY_BIT_BOOST(n)     (((n)&0x7)<<2)      
#define PMU_INTF_CURLIMIT_SEL_BOOST(n) (((n)&0x7)<<5)      
#define PMU_INTF_BOOST_VBIT(n)         (((n)&0xF)<<8)      
#define PMU_INTF_REG_RSVD_BOOST(n)     (((n)&0xF)<<12)     
#define PMU_INTF_REG_CLK_FREQ_SEL_BOOST(n) (((n)&0x7)<<16)     
#define PMU_INTF_REG_CLK_DUTY_SEL_BOOST(n) (((n)&0x7)<<19)     
#define PMU_INTF_BOOST_CLK_ENABLE      (1<<22)             

//pmu_intf_icr
#define PMU_INTF_KEY_INT_CLEAR         (1<<0)              
#define PMU_INTF_KEY_MEASURE_INT_CLEAR (1<<1)              
#define PMU_INTF_GPADC_INT_CLEAR       (1<<2)              
#define PMU_INTF_LOWBAT_INT_CLEAR      (1<<3)              
#define PMU_INTF_AC_ON_INT_CLEAR       (1<<4)              

//pmu_intf_imr
#define PMU_INTF_KEY_INT_MASK          (1<<0)              
#define PMU_INTF_KEY_MEASURE_INT_MASK  (1<<1)              
#define PMU_INTF_GPADC_INT_MASK        (1<<2)              
#define PMU_INTF_LOWBAT_INT_MASK       (1<<3)              
#define PMU_INTF_AC_ON_INT_MASK        (1<<4)              
#define PMU_INTF_SWITCH_INT_MASK       (1<<5)              

//pmu_intf_irsr
#define PMU_INTF_KEY_INT_RAW_STATUS    (1<<0)              
#define PMU_INTF_KEY_MEASURE_INT_RAW_STATUS (1<<1)              
#define PMU_INTF_GPADC_INT_RAW_STATUS  (1<<2)              
#define PMU_INTF_LOWBAT_INT_RAW_STATUS (1<<3)              
#define PMU_INTF_AC_ON_INT_RAW_STATUS  (1<<4)              
#define PMU_INTF_SWITCH_INT_RAW_STATUS (1<<5)              

//pmu_intf_isr
#define PMU_INTF_KEY_INT_STATUS        (1<<0)              
#define PMU_INTF_KEY_MEASURE_INT_STATUS (1<<1)              
#define PMU_INTF_GPADC_INT_STATUS      (1<<2)              
#define PMU_INTF_LOWBAT_INT_STATUS     (1<<3)              
#define PMU_INTF_AC_ON_INT_STATUS      (1<<4)              
#define PMU_INTF_SWITCH_INT_STATUS     (1<<5)              

//pmu_ana_rsvd
#define PMU_INTF_PMU_ANA_RSVD_OUT(n)   (((n)&0xFFF)<<0)    
#define PMU_INTF_PMU_ANA_RSVD_IN(n)    (((n)&0x3F)<<12)    


#endif


